/*
 ***********************************************************
 *(c) Copyright Sindoh Co. Ltd. 
 *
 *               SINDOH Confidential
 * @Author : Kilsu Jung
 * @Date   : 25th April 2014
 * 
 ***********************************************************
 **/
#include <stdio.h>
#include <sys/prctl.h>	
#include <sys/poll.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>
//#include "AType.h"
#define SYSCARD_MAIN_C
#include "SysCardReaderMgr.h"
//#define SYSCARD_MAIN_C
//#include "SysCardReaderMsg.h"
#undef SYSCARD_MAIN_C
#include "SysUI_Interface.h"
#include "agRouter.h"
#include "posix_ostools.h"
#include "logger.h"
#include "lassert.h"
#include "db_api.h"
#include "NetworkIFModule.h"
#include "memAPI.h"
#include "jsmn.c"
#include "spec.h"
#ifdef HAVE_PAPERCUT
#define SYSCARD_MAIN_C
#include "PaperCut_NW_Reader.h"
extern int authenticateCardID(char * papercut_code);
#undef SYSCARD_MAIN_C
#endif
#define DBG_PRFX "AUTH: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 1 )  // System Service Layer Mask

#define DBG_AUTH_PRINTF_SDH_ERR DBG_PRINTF_SDH_ERR
#define DBG_AUTH_PRINTF_SDH_DEBUG DBG_PRINTF_SDH_ERR

#define KARAS_SOLUTION_JOB_LOG_PATH "/mnt/karas_solution_job_log"

#define KBD_DEV "/dev/hidraw0"
#define CARD_READER_STACK POSIX_MIN_STACK_SIZE

#define JOB_LOG_LENGTH 1024
#define NUM_MEM 500
#define JSON_TOK_MAX 30

/* TODO:: This length may change as per UIManager receiver end */
#define	SYS_CARD_MESSAGE_DATA_LEN	32
#define NUM_MESSAGES 20
#define UP_PORT 58830

#define CARD_AUTH_UPRINT
static ALIGN8 UINT8 CardReaderStack[CARD_READER_STACK];
static ALIGN8 UINT8 CardMsgStack[CARD_READER_STACK];      
static pthread_t Card_reader_thd_id;
static pthread_t Card_msg_thd_id;
static mqd_t ScardRecv_queue;
static mqd_t ScardSend_queue;
//static mqd_t ScardSend_int_queue;

static int ipfilter_on = 0;
// for storing Personal Data from webserver via JSON parser

// it only write in JsonParser func.
// and only access in WriteSharedMemory func.
static stAuth_ParsedData PsData;
static int upResult = 0;
static int upPermission = 0;
static char code[MAX_CODE_LENGTH];

//Auth DB Status Checker
static int CheckerAuth = 0;
static int CheckerPrintAuth = 0;
static int CheckerCard = 0;
static int CheckerPup = 0;
static int CheckerKARAS = 0;
static int CheckerPaperCut = 0;

// Auth internal loginchecker
static int intCheckerLogin = 0; //0: logouted, 1: logined

// for storing Webdata(response) from webserver
struct MemoryStruct {
	char *memory;
	size_t size;
};

//#define MAX_CODE_LENGTH 200
static void do_card_code(int counter, char *code, int conType, int jobType, int jobCount);
static void do_card_code_uprint(int len, char *code);
static int ConnectServer(char *server, char **memory);
static void sendMessageToSysmgr(int webStatus, int action);
static void sendMessageTest(int testStatus); //temp
static void sendMessageCardLogin(void); //temp
static int JsonParser(char *memory);
static int JsonParserVerify(char *memory);
static int keycode(int);
static void writeGlobalVar(int codeLength);
static void initParsedData(void);
static void initGlobalVar(void);
static void printGlobalVar(void);
static int uPrintCmd(char *upIP, char *upID, char *mfpIP);
static int DBReader(int readType);
static int AccChecker(void);
static void setAdmin(int adminResult);
static void cardUp(void);
extern uint32_t GetJobNumber(void);

void GoJobLog(stPrintInformationJobLog* inform)
{
	if(CheckerKARAS != 1 || CheckerAuth != 2 || CheckerPrintAuth != 1) {
		DBG_PRINTF_SDH_ERR("[%s] this machine is not kara-s or setting diffrent\n", __func__);
		return;
	}
	char *ser_a1 = NULL;
    char *ser_a2 = NULL;
	char server[JOB_LOG_LENGTH] = {0};
    char server2[JOB_LOG_LENGTH] = {0};
	char *userId  	= "?userId=";
	char *mfpIp	  	= "&mfpIp=";
	char *job	  	= "&job=1";
	char *count	  	= "&count=";
	char *mach 	  	= "&mach=3";
	char *serial  	= "&serial=";
	char *jobId	  	= "&jobId=";
	char *docName 	= "&docName=";
	char *paper	  	= "&paperSize=";
	char *duplex  	= "&duplex=";
	char *jobResult = "&jobResult=0";

	char *ip_addr = NULL;
    char ipAddr[16] = {0};
    memset(ipAddr, 0x00, 16);

	char *webData;
	int webStatus = 1; //web Status Checker
	int webStatusSave = 1;
    int netStatus = 0;

	bool sendJobLogFlag = true;

	error_type_t db_retval = FAIL;
	ipfilter_on = db_get_var_int("CONF_SYSTEM", "IPFilterOn", &db_retval);
	if(db_retval == FAIL) {
		ipfilter_on=0;
	}

	ser_a1 = db_get_var("INST_SERVERIPADDR", "ServerIPAddr", &db_retval);
	if(db_retval == FAIL) {
		DBG_PRINTF_SDH_ERR("(Auth_Job) ServerIPAddr: %s\n", ser_a1);
		return;
	}

	ser_a2 = db_get_var("INST_SERVERIPADDR", "ServerIPAddr2", &db_retval);
	if(db_retval == FAIL) {
		DBG_PRINTF_SDH_ERR("(Auth_Job) ServerIPAddr2: %s\n", ser_a2);
		return;
	}

	if(ser_a1 == NULL) {
		DBG_PRINTF_SDH_ERR("(Auth_Job) no server address\n");
		return;
	}

	if(NIF_getStatus_wifi() == NIF_Status_IfUp){
        DBG_PRINTF_SDH_ERR("(Auth_Job) wifi is up\n");
        netStatus = NIF_getIPAddrV4_str_wifi(&ip_addr);
        if (netStatus == NIF_Status_NotFound) {
            DBG_PRINTF_SDH_ERR("(Auth_Job) but status not found !!!\n");
            netStatus = NIF_getIPAddrV4_str_wired(&ip_addr);
            if (netStatus == NIF_Status_NotFound) {
                webStatus = 2;
                sendMessageToSysmgr(webStatus, 1);
                DBG_PRINTF_SDH_ERR("(Auth_Job) Wired status not found !!!\n");
                return;
            }
        }
        strncpy(ipAddr, ip_addr, 15);
        DBG_PRINTF_SDH_ERR("(Auth_Job) Get IP %d: %s : %s \n", netStatus, ip_addr, ipAddr);
	}
	else {
		netStatus = NIF_getIPAddrV4_str_wired(&ip_addr);
	    strncpy(ipAddr, ip_addr, 15);
        DBG_PRINTF_SDH_ERR("(Auth_Job) Get IP %d: %s : %s \n", netStatus, ip_addr, ipAddr);
        if (netStatus == NIF_Status_NotFound) {
            webStatus = 2;
            sendMessageToSysmgr(webStatus, 1);
            DBG_PRINTF_SDH_ERR("(Auth_Job) Wired status not found !!!\n");
            return;
        }
	}

	webStatusSave = webStatus;

	if(access(KARAS_SOLUTION_JOB_LOG_PATH, F_OK) < 0)
		system("mkdir -p " KARAS_SOLUTION_JOB_LOG_PATH);

	char buff[30] = {0};
	char filePath[128] = {0};
	char sendToServerBuff[JOB_LOG_LENGTH] = {0};
	FILE *fp;
	fp = popen("ls -tr " KARAS_SOLUTION_JOB_LOG_PATH, "r");
	if(fp == NULL) {
		DBG_PRINTF_SDH_ERR("[%s] Don't have saved job log hehe\n", __func__);
	}
	else {
		DBG_PRINTF_SDH_ERR("[%s] Folder open success\n", __func__);
		while(fgets(buff, sizeof(buff), fp)) {
			if(strlen(buff) < 1)
				break;
			DBG_PRINTF_SDH_ERR("[%s] buff = %s\n", __func__, buff);
			if(buff[strlen(buff)-1] == '\n')
				buff[strlen(buff)-1] = '\0';
			sprintf(filePath, KARAS_SOLUTION_JOB_LOG_PATH "/%s", buff);
			FILE* fr;
			fr = fopen(filePath, "r");
			fgets(sendToServerBuff, sizeof(sendToServerBuff), fr);
			fclose(fr);
			if(strlen(sendToServerBuff) < 5) {
				remove(filePath);
				continue;
			}
			if(sendToServerBuff[strlen(sendToServerBuff)-1] == '\n')
				sendToServerBuff[strlen(sendToServerBuff)-1] = '\0';
			webData = (char *)malloc(1);
			webStatus = ConnectServer(sendToServerBuff, &webData);
			if(webStatus == 1) {
				DBG_PRINTF_SDH_ERR("[%s] Send fail : %s", __func__, sendToServerBuff);
				sendJobLogFlag = false;
				break;
			}
			else {
				DBG_PRINTF_SDH_ERR("[%s] Success : %s", __func__, sendToServerBuff);
				remove(filePath);
			}
			if(webData)
				free(webData);
		}
		pclose(fp);
		webStatus = webStatusSave;
	}
	
	if(inform == NULL) {
		if(ser_a1) 
			MEM_FREE_AND_NULL(ser_a1);
		if(ser_a2) 
			MEM_FREE_AND_NULL(ser_a2);
		if(ip_addr) 
			MEM_FREE_AND_NULL(ip_addr);
		return;
	}

	stPrintInformationJobLog printInform = {0};
	memcpy(&printInform, inform, sizeof(stPrintInformationJobLog));

	/* Change this */
	sprintf(server,"%s%s%s%s%s%s%s%d%s%s%s%s%d%s%s%s%s%s%d%s",
			ser_a1, userId, AuthInfo.printAuthID, mfpIp, ip_addr, job, count, printInform.count, mach, serial, printInform.serial, jobId, printInform.jobId, 
			docName, printInform.fileName, paper, printInform.paperSize, duplex, printInform.duplex, jobResult);
	sprintf(server2,"%s%s%s%s%s%s%s%d%s%s%s%s%d%s%s%s%s%s%d%s",
			ser_a2, userId, AuthInfo.printAuthID, mfpIp, ip_addr, job, count, printInform.count, mach, serial, printInform.serial, jobId, printInform.jobId, 
			docName, printInform.fileName, paper, printInform.paperSize, duplex, printInform.duplex, jobResult);
	/* Change this */

	
	webData = (char *)malloc(1);
	if(sendJobLogFlag)
		webStatus = ConnectServer(server, &webData);
	else {
		webStatus = 1;
		DBG_PRINTF_SDH_ERR("(Auth_Job) Previous job send fail, so current job don't send and file save\n");
	}

	DBG_PRINTF_SDH_ERR("(Auth_Job) webStatus : %d\n", webStatus);
	DBG_PRINTF_SDH_ERR("(Auth_Job) server request : %s\n", server);
    if (webStatus == 1 && ser_a2 != NULL){
		DBG_PRINTF_SDH_ERR("(Auth_Job) Send Secondary Server yet (code : %d)\n", webStatus);
    }

	if (webStatus == 0) {
		DBG_PRINTF_SDH_ERR("Success send log\n");
	}
	else { // Fail -> Save and later send to server
		DBG_PRINTF_SDH_ERR("Fail , start save the joblog\n");
		time_t timer = time(NULL);
		char timeString[30] = {0};
		char saveFile[64] = {0};
		snprintf(timeString, sizeof(timeString), "%ld", timer);
		snprintf(saveFile, sizeof(saveFile), "%s/%s", KARAS_SOLUTION_JOB_LOG_PATH, timeString);
		FILE *fw;
		fw = fopen(saveFile, "w");
		fputs(server, fw);
		fclose(fw);
	}

	if(ser_a1) 
		MEM_FREE_AND_NULL(ser_a1);
    if(ser_a2) 
		MEM_FREE_AND_NULL(ser_a2);
    if(ip_addr) 
		MEM_FREE_AND_NULL(ip_addr);
	if(webData) {
		free(webData);
		DBG_PRINTF_SDH_ERR("(Auth_Job) Free WebData Memory\n");
	}

}

void card_reader_action(void)
{
	int fd = 0;
	int resReaderFD = 0;
	int counter = 0;
	char read_buffer[3]={0,};
	int codeLength = MAX_CODE_LENGTH;
    memset(code, 0x00, MAX_CODE_LENGTH);
	
    if( (fd = open(KBD_DEV, O_RDWR)) < 0) {
		return;
	}

	while (1)
	{
		if(0 != access(KBD_DEV, F_OK)) {
			break;
		}

		resReaderFD = read(fd, read_buffer, 3);
		if( resReaderFD < 0 ) {
			break;
		}

		if (read_buffer[2] != 0)
		{
			code[counter]=keycode(read_buffer[2]);
			if (code[counter] == 10) { // enter
				codeLength = counter;
				DBG_PRINTF_SDH_ERR("(CardReader) Code Length is updated: %d\n", codeLength);
			}
			counter++;
		}
		if (counter == (codeLength+1))
		{
            DBG_PRINTF_SDH_ERR("(CardReader) Card ID: %s", code);
            code[codeLength] = 0x00;
            char papercut_code[PAPERCUT_MAX_CODE_LENGTH];
            memset(papercut_code,0,sizeof(papercut_code));
            if(CheckerCard == 1 && CheckerPaperCut == 1) {
                strcpy(papercut_code, code);
                papercut_code[codeLength] = '\r';
                if (codeLength < PAPERCUT_MAX_CODE_LENGTH){
                    papercut_code[codeLength+1] = 0x00;
                }
                if(authenticateCardID(papercut_code) != OK)
                    DBG_PRINTF_SDH_ERR("(CardReader) Unable to Authenticate card %d", code);
            }
            else if(CheckerCard == 1 && CheckerKARAS == 1){
				if(CheckerPrintAuth == 0)
					do_card_code_uprint(counter, code);
				else
					sendMessageCardLogin();
			}
            else if(CheckerCard == 1 && intCheckerLogin == 0)
			    sendMessageCardLogin(); // Action, Type, Count
            else if (CheckerCard != 1)
                DBG_PRINTF_SDH_ERR("(CardReader) Card SP off !!! \n");
            else if (intCheckerLogin != 0)
                DBG_PRINTF_SDH_ERR("(CardReader) Already Logined !!! \n");
			counter=0;
			codeLength = MAX_CODE_LENGTH;
		}
	}
	close(fd);
}

void *CardReaderThread(void *unused)
{
	while(1)
	{
		if(0 == access("/dev/hidraw0", F_OK))
		{ 
			card_reader_action();
		}
		usleep(2000*1000); //2S check for if device node does not exists
	}
	return NULL;
}

void *CardMsgThread(void *unused)
{
    SYS_UI_MESSAGE      nextMsg;
    int                 retcode;
    while (1)
    {
        retcode = mq_receive(ScardRecv_queue, (char*)&nextMsg, sizeof(SYS_UI_MESSAGE), 0); 
        DBG_PRINTF_SDH_ERR("(MsgThr) [%s]: Received Message\n", __FUNCTION__);
        XASSERT( retcode == sizeof(SYS_UI_MESSAGE), thread_errno);        
        if(nextMsg.msgSender == SCARD_SENDER_ID){
            DBG_PRINTF_SDH_ERR("(MsgThr) SCard Reader! cmd: %04x, data: %02d, %02d, %02d\n", nextMsg.cmd, nextMsg.data[0], nextMsg.data[1], nextMsg.data[2]);
            stAuth_Req stAuthReq;
            memcpy(&stAuthReq, (&nextMsg)->data, sizeof(stAuth_Req));
            DBG_PRINTF_SDH_ERR("(MsgThr) Recv Req Message, action: %d, type: %d, count: %d\n", stAuthReq.action, stAuthReq.jobType, stAuthReq.jobCount);
            if(stAuthReq.action == AUTH_INT_LOGIN_CARD){
                DBG_PRINTF_SDH_ERR("(MsgThr) Card Login \n");
				if(CheckerKARAS == 1 && CheckerPrintAuth == 1) {
					if(GetJobNumber() == 0)
						intCheckerLogin = 0;
				}
                if ( intCheckerLogin == 0 )
						if (CheckerAuth == 1) {
							cardUp();
						}
						else {
							do_card_code((strlen(AuthInfo.userID)+1), code, AUTH_INT_LOGIN_CARD, 0, 0);
						}
                else {
					DBG_PRINTF_SDH_ERR("(MsgThr) Already Logined !!! \n");		
				}
            }
        }   
        else if(nextMsg.msgSender == SYSTEM_SERVICE_ID && nextMsg.cmd == CMD_INT_SC_AUTH_REQ){
            DBG_PRINTF_SDH_ERR("(MsgThr) Msg from SysMgr! cmd: %04x, data: %02d, %02d, %02d\n", nextMsg.cmd, nextMsg.data[0], nextMsg.data[1], nextMsg.data[2]);
            stAuth_Req stAuthReq;
            memcpy(&stAuthReq, (&nextMsg)->data, sizeof(stAuth_Req));
            if (stAuthReq.action == AUTH_LOGIN){
                int AccRes = 0;
                DBG_PRINTF_SDH_ERR("(MsgThr) Panel Login \n");
                AccRes = AccChecker();
                if (AccRes == 1)
                    do_card_code((strlen(AuthInfo.userID)+1), AuthInfo.userID, AUTH_INT_LOGIN_PANEL, 0, 0);
                else if (AccRes == 2 || AccRes == 0){
                    setAdmin(AccRes);
                }
                    
            }
            else if (stAuthReq.action == AUTH_LOGOUT){
                DBG_PRINTF_SDH_ERR("(MsgThr) Logout \n");
                stAuthReq.jobType = 16;
                intCheckerLogin = 0;
                do_card_code((strlen(AuthInfo.userID)+1), AuthInfo.userID, AUTH_INT_LOGOUT, stAuthReq.jobType, 0);
            }
            else if (stAuthReq.action == AUTH_COUNTUP){
                DBG_PRINTF_SDH_ERR("(MsgThr) Job countup, job: %d, count: %d\n", stAuthReq.jobType, stAuthReq.jobCount);
                if (stAuthReq.jobType == 1){
                    stAuthReq.jobType = 1;
                    do_card_code((strlen(AuthInfo.printJobID)+1), AuthInfo.printJobID, AUTH_INT_COUNTUP, stAuthReq.jobType, stAuthReq.jobCount);
                }
                else if (stAuthReq.jobType == 2){
                    stAuthReq.jobType = 2;
                    do_card_code((strlen(AuthInfo.scanJobID)+1), AuthInfo.scanJobID, AUTH_INT_COUNTUP, stAuthReq.jobType, stAuthReq.jobCount);
                }
                else if (stAuthReq.jobType == 3){
                    stAuthReq.jobType = 4;
                    do_card_code((strlen(AuthInfo.faxJobID)+1), AuthInfo.faxJobID, AUTH_INT_COUNTUP, stAuthReq.jobType, stAuthReq.jobCount);
                }
                else if (stAuthReq.jobType == 4){
                    stAuthReq.jobType = 8;
                    do_card_code((strlen(AuthInfo.copyJobID)+1), AuthInfo.copyJobID, AUTH_INT_COUNTUP, stAuthReq.jobType, stAuthReq.jobCount);
                }
            }
            else if (stAuthReq.action == AUTH_NET_PRINT) {
                DBG_PRINTF_SDH_ERR("(MsgThr) Print Auth Verify! Mod: %d\n", stAuthReq.jobType);
                AuthVerify(AuthInfo.printAuthID, AuthInfo.printAuthPW, stAuthReq.jobType);
            }
        }
        else if(nextMsg.msgSender == SYSTEM_SERVICE_ID && nextMsg.cmd == CMD_INT_SC_AUTH_CONF_CHANGED){
            DBG_PRINTF_SDH_ERR("(MsgThr) Auth Configs are changed\n");
            int dbResult = 0;
            dbResult = DBReader(0);
            if (dbResult == 1)
                DBG_PRINTF_SDH_ERR("(MsgThr) DB Read FAIL!!!!\n");
        }
    }
}

void SysServiceCardReaderInit(void)
{
	//DBG_SSL_PRINTF_ERR(" [SSL] -> %s  called \n", __FUNCTION__);

	/* This thread will run as daemon thread till device shutsdown and doesn't associated with any kind of detach, join now. */

    posix_create_message_queue( &ScardRecv_queue, "/mq_sys_to_scard", NUM_MESSAGES, sizeof(SYS_UI_MESSAGE) );
    router_register_queue(SCARD_RECEIVER_ID, ScardRecv_queue);  // Register a message queue.  
    DBG_PRINTF_SDH_ERR("(SCARD_INIT) Create Scard MQ\n");
    
    //ScardSend_int_queue = mq_open( "/mq_sys_to_scard", O_RDWR );
    //router_register_queue(SCARD_SENDER_ID, ScardSend_int_queue);  // Register a message queue.
    //printf("(SCARD_INIT) Open Scard MQ (for intanal)\n");
    
    ScardSend_queue = mq_open( "/mq_ui_to_sys", O_RDWR );
    router_register_queue(SCARD_SENDER_ID, ScardSend_queue);  // Register a message queue.
    DBG_PRINTF_SDH_ERR("(SCARD_INIT) Open SysMgr MQ \n");

	posix_create_thread(&Card_reader_thd_id, CardReaderThread, 0, "cardreader", CardReaderStack, CARD_READER_STACK, POSIX_THR_PRI_NORMAL);
    posix_create_thread(&Card_msg_thd_id, CardMsgThread, 0, "cardmsgrecv", CardMsgStack, CARD_READER_STACK, POSIX_THR_PRI_NORMAL);
    
    int dbResult = 0;
    dbResult = DBReader(0);
    if (dbResult == 1)
        DBG_PRINTF_SDH_ERR("(SCARD_INIT) DB Read FAIL!!!!\n");

#ifdef HAVE_PAPERCUT
    if (CheckerPaperCut == 1) {
        DBG_PRINTF_SDH_ERR("(SCARD_INIT) PaperCut Init!!!!\n");
    	SysServicePaperCutInit();
    }
#endif
	GoJobLog(NULL);	
}

void do_card_code(int counter, char *code, int conType, int jobType, int jobCount) {
	// for initialize the server address
	char *ser_a1;
    char *ser_a2;
	char server[1000];
    char server2[1000];
	char *ser_b1 = {"?cardNumber="};
	char *ser_b2 = {"&mfpIp="};
	char *ser_b3 = {"&job="};
	char *ser_b4 = {"&count="};
	char *ser_b5 = {"&mach="};
    char *ser_c1 = {"?userId="};
    char *ser_c2 = {"&password="};
	char *ip_addr;
    char ipAddr[16];
    memset(ipAddr, 0x00, 16);
    memset(server, 0x00, 1000);
    memset(server2, 0x00, 1000);

	char *webData;
	int webStatus = 1; //web Status Checker
    int netStatus = 0;
	
    int mtype = 3;	//KARA-S
    if(IS_PINETREE)
        mtype = 4;
    else if(IS_PINETREES)
        mtype = 5; 
#if 0
	int i;
	for ( i = 0; i < counter ; i++ ) {
		printf("%c, %d\n", code[i], code[i]);
	}
#endif
	error_type_t db_retval = FAIL;
	ipfilter_on = db_get_var_int("CONF_SYSTEM", "IPFilterOn", &db_retval);
	if ( db_retval == FAIL ) {
		ipfilter_on=0;
	}

	ser_a1=db_get_var("INST_SERVERIPADDR", "ServerIPAddr", &db_retval);
	if(db_retval ==FAIL) {
		DBG_PRINTF_SDH_ERR("(Auth) ServerIPAddr: %s\n", ser_a1);
		return;
	}

	ser_a2=db_get_var("INST_SERVERIPADDR", "ServerIPAddr2", &db_retval);
	if(db_retval ==FAIL) {
		DBG_PRINTF_SDH_ERR("(Auth) ServerIPAddr2: %s\n", ser_a2);
		return;
	}

	if ( ser_a1 == NULL ) {
		DBG_PRINTF_SDH_ERR("(Auth) no server address\n");
		return;
	}

	if ( NIF_getStatus_wifi() == NIF_Status_IfUp ){
        DBG_PRINTF_SDH_ERR("(Auth) wifi is up\n");
        netStatus = NIF_getIPAddrV4_str_wifi(&ip_addr);
        if (netStatus == NIF_Status_NotFound) {
            DBG_PRINTF_SDH_ERR("(Auth) but status not found !!!\n");
            netStatus = NIF_getIPAddrV4_str_wired(&ip_addr);
            if (netStatus == NIF_Status_NotFound) {
                webStatus = 2;
                sendMessageToSysmgr(webStatus, 1);
                DBG_PRINTF_SDH_ERR("(auth) Wired status not found !!!\n");
                return;
            }
        }
        strncpy(ipAddr, ip_addr, 15);
        DBG_PRINTF_SDH_ERR("(Auth) Get IP %d: %s : %s \n", netStatus, ip_addr, ipAddr);
	}
	else{
		netStatus = NIF_getIPAddrV4_str_wired(&ip_addr);
	    strncpy(ipAddr, ip_addr, 15);
        DBG_PRINTF_SDH_ERR("(Auth) Get IP %d: %s : %s \n", netStatus, ip_addr, ipAddr);
        if (netStatus == NIF_Status_NotFound) {
            webStatus = 2;
            sendMessageToSysmgr(webStatus, 1);
            DBG_PRINTF_SDH_ERR("(auth) Wired status not found !!!\n");
            return;
        }
	}

    if(conType == AUTH_INT_LOGIN_PANEL){
        DBG_PRINTF_SDH_ERR("(Auth) Panel Login: %s\n", code);
        sprintf(server,"%s%s%s%s%s%s%s%s%d%s%d%s%d", ser_a1, ser_c1, AuthInfo.userID, ser_c2, AuthInfo.userPwd, ser_b2, ip_addr, ser_b3, jobType, ser_b4, jobCount, ser_b5, mtype);
        sprintf(server2,"%s%s%s%s%s%s%s%s%d%s%d%s%d", ser_a2, ser_c1, AuthInfo.userID, ser_c2, AuthInfo.userPwd, ser_b2, ip_addr, ser_b3, jobType, ser_b4, jobCount, ser_b5, mtype);

    }
    else if(conType == AUTH_INT_VERIFY){
        DBG_PRINTF_SDH_ERR("(Auth) uPrint Verify: %s\n", code);
        sprintf(server,"%s%s%s%s%s%s%d%s%d", ser_a1, ser_c1, code, ser_b2, ip_addr, ser_b3, jobType, ser_b5, mtype);
        sprintf(server2,"%s%s%s%s%s%s%d%s%d", ser_a2, ser_c1, code, ser_b2, ip_addr, ser_b3, jobType, ser_b5, mtype);
    }
    else if(conType == AUTH_INT_COUNTUP || conType == AUTH_INT_LOGOUT){
		if(conType == AUTH_INT_COUNTUP && CheckerKARAS == 1 && CheckerPrintAuth == 1)
			return;
        DBG_PRINTF_SDH_ERR("(Auth) Print Countup: %s\n", code);
        sprintf(server,"%s%s%s%s%s%s%d%s%d%s%d", ser_a1, ser_c1, code, ser_b2, ip_addr, ser_b3, jobType, ser_b4, jobCount, ser_b5, mtype);
        sprintf(server2,"%s%s%s%s%s%s%d%s%d%s%d", ser_a2, ser_c1, code, ser_b2, ip_addr, ser_b3, jobType, ser_b4, jobCount, ser_b5, mtype);
    }
    else if(conType == AUTH_INT_LOGIN_CARD){
        DBG_PRINTF_SDH_ERR("(Auth) Card Login: %s\n", code);
        sprintf(server,"%s%s%s%s%s%s%d%s%d%s%d", ser_a1, ser_b1, code, ser_b2, ip_addr, ser_b3, jobType, ser_b4, jobCount, ser_b5, mtype);
        sprintf(server2,"%s%s%s%s%s%s%d%s%d%s%d", ser_a2, ser_b1, code, ser_b2, ip_addr, ser_b3, jobType, ser_b4, jobCount, ser_b5, mtype);
	}
    //printf("server1: %s\n", server);
    //printf("server2: %s\n", server2);
    DBG_PRINTF_SDH_ERR("(Auth) send to Server\n");
	webData = (char *)malloc(1);
	//webStatus = ConnectServer("https://10.15.101.107:8443/uPrint/printUPrint.do?cardNumber=m3306b798&mfpIp=10.15.102.123&job=0&count=0&mach=5", &webData);
    webStatus = ConnectServer(server, &webData);
	DBG_PRINTF_SDH_ERR("(Auth) Send Primary Server (code : %d)\n", webStatus);
    if (webStatus == 1 && ser_a2 != NULL){
//        webStatus = ConnectServer(server2, &webData);
//        DBG_PRINTF_SDH_ERR("(Auth) Send Secondary Server (code : %d)\n", webStatus);
	    DBG_PRINTF_SDH_ERR("(Auth) Send Secondary Server yet (code : %d)\n", webStatus);
    }
	
    if (webStatus == 0)
	{
		//		if( 0 == strcmp(code, "m2e919011")|| 0 == strcmp(code, "1002870385")) {
		//			RxPermission = 16; printf("(SCard) ADMIN\n");
		//		}
		if(conType == AUTH_INT_LOGIN_CARD || conType == AUTH_INT_LOGIN_PANEL){
            DBG_PRINTF_SDH_ERR("(Auth) %s, size: %d\n", webData, strlen(webData));
            JsonParser(webData);
            writeGlobalVar((strlen(code)+1));
        }
        if(conType == AUTH_INT_VERIFY){
            DBG_PRINTF_SDH_ERR("(Auth) %s, size: %d\n", webData, strlen(webData));
            JsonParserVerify(webData);
        }
	}
    else {
        if(conType == AUTH_INT_VERIFY){
            DBG_PRINTF_SDH_ERR("(Auth) Print Verify Fail because connection error\n");
            if (webStatus == 2)
                upResult = AUTH_FAIL_INTERFACE;
            else
                upResult = AUTH_FAIL_CON_SERVER;
            upPermission = 0;
        }
    }
    if(conType == AUTH_INT_LOGIN_CARD || conType == AUTH_INT_LOGIN_PANEL){
        sendMessageToSysmgr(webStatus, 1);
        if(PsData.upIP[0] != 0x00){
            //if(CheckerPnp == 1)
            DBG_PRINTF_SDH_ERR("(Auth) IPCheck: %s \n", ip_addr);
            if(PsData.result == 0 && PsData.permission >= 1)
                uPrintCmd(PsData.upIP, PsData.upID, ip_addr);
            else
                DBG_PRINTF_SDH_ERR("(Auth) Personal uPrint Auth Fail!! - result: %d, permission: %d \n", PsData.result, PsData.permission);
        }
    }
    else if (conType == AUTH_INT_VERIFY) {
        DBG_PRINTF_SDH_ERR("(Auth) Print Verify! web result: %d\n", webStatus);
        sendMessageToSysmgr(webStatus, AUTH_NET_PRINT);
        
    }
    
    // test 
    #define KT_TEST 0
    if(KT_TEST){
        sendMessageTest(1); 
        //uPrintCmd("10.15.23.46", "ksjung", ip_addr); 
    }
	if ( ser_a1 ) 
		MEM_FREE_AND_NULL(ser_a1);
    if ( ser_a2 ) 
		MEM_FREE_AND_NULL(ser_a2);
    if ( ip_addr ) 
		MEM_FREE_AND_NULL(ip_addr);
	if(webData) {
		free(webData);
		DBG_PRINTF_SDH_ERR("(Auth) Free WebData Memory\n");
	}
}

void do_card_code_uprint(int len, char *code) {
    // for initialize the server address
    char *ser_a1;
    char *ser_a2;
    char server[1000];
    char server2[1000];
    char *ser_b1 = {"?cardNumber="};
    char *ser_b2 = {"&mfpIp="};
    char *ser_b3 = {"&job=0"};
    char *ser_b4 = {"&count=0"};
    char *ser_b5 = {"&mach="};
    char *ip_addr;
    memset(server, 0x00, 1000);
    memset(server2, 0x00, 1000);

    char *webData;
    int webStatus = 1; //web Status Checker
    int mtype = 3;  //KARA-S
#if 0
    int i;
    for ( i = 0; i < len ; i++ ) {
        printf("%c, %d\n", code[i], code[i]);
    }
#endif
    error_type_t db_retval = FAIL;
    ipfilter_on = db_get_var_int("CONF_SYSTEM", "IPFilterOn", &db_retval);
    if ( db_retval == FAIL ) {
        ipfilter_on=0;
    }

    ser_a1=db_get_var("INST_SERVERIPADDR", "ServerIPAddr", &db_retval);
    if(db_retval ==FAIL) {
        DBG_PRINTF_SDH_ERR("(uPrint) ServerIPAddr: %s\n", ser_a1);
        return;
    }
    
    ser_a2=db_get_var("INST_SERVERIPADDR", "ServerIPAddr2", &db_retval);
    if(db_retval ==FAIL) {
        DBG_PRINTF_SDH_ERR("(uPrint) ServerIPAddr2: %s\n", ser_a2);
        return;
    }
    
    if ( ser_a1 == NULL ) {
        DBG_PRINTF_SDH_ERR("(uPrint) no server address\n");
        return;
    }

    if ( NIF_getStatus_wifi() == NIF_Status_IfUp ){
        NIF_getIPAddrV4_str_wifi(&ip_addr);
    }
    else{
        NIF_getIPAddrV4_str_wired(&ip_addr);
    }
    sprintf(server,"%s%s%s%s%s%s%s%s%d", ser_a1, ser_b1, code, ser_b2, ip_addr, ser_b3, ser_b4, ser_b5, mtype);
    sprintf(server2,"%s%s%s%s%s%s%s%s%d", ser_a2, ser_b1, code, ser_b2, ip_addr, ser_b3, ser_b4, ser_b5, mtype);

    DBG_PRINTF_SDH_ERR("(uPrint) send to Server : %s\n", server);
    webData = (char *)malloc(1);
    webStatus = ConnectServer(server, &webData);
    DBG_PRINTF_SDH_ERR("(uPrint) Send Primary Server (code : %d)\n", webStatus);
    if (webStatus == 1 && ser_a2 != NULL){
//        webStatus = ConnectServer(server2, &webData);
        DBG_PRINTF_SDH_ERR("(uPrint) Send Secondary Server yet(code : %d)\n", webStatus);
    }
    if (webStatus == 0)
    {
        //      if( 0 == strcmp(code, "m2e919011")|| 0 == strcmp(code, "1002870385")) {
        //          RxPermission = 17; printf("(SCard) ADMIN\n");
        //      }
        DBG_PRINTF_SDH_ERR("(uPrint) %s, size: %d\n", webData, strlen(webData));
        //JsonParser(webData);
        //      if (RxPermission >= 0) {
        //          writeSharedMemory(code, codeLength, ShmKey, ShmSize);
        //          sendMessageToSysmgr(ShmKey, ShmSize);
        //          //printSharedMemory(ShmKey, ShmSize);
        //      }
    }
    if ( ser_a1 ) {
        MEM_FREE_AND_NULL(ser_a1);
    }    
    if ( ser_a2 ) {
        MEM_FREE_AND_NULL(ser_a2);
    }

    if(webData) {
        free(webData);
        DBG_PRINTF_SDH_ERR("(uPrint) Free WebData Memory\n");
    }
}

void sendMessageToSysmgr(int webStatus, int action)
{
    error_type_t err;
    SYS_UI_MESSAGE msg;
    stAuth_Result stAuthResult;
    stAuthResult.act = action;
    //in this time this function is using only login

    if(action == AUTH_CARD_TAG || action == AUTH_SOL_AUTH)
    {
        stAuthResult.result = 0;
        stAuthResult.permission = 0;
    }
    else if(action == AUTH_NET_PRINT){
        stAuthResult.result = upResult;
        stAuthResult.permission = upPermission;
        if (upResult > 0)
            stAuthResult.permission = 0;
    }
    else if(webStatus == 1){
        stAuthResult.result = AUTH_FAIL_CON_SERVER;
        stAuthResult.permission = 0;
    }
    else if(webStatus == 2){
        stAuthResult.result = AUTH_FAIL_INTERFACE;
        stAuthResult.permission = 0;
    }
    else if(PsData.result > 0){
        stAuthResult.result = PsData.result;
        stAuthResult.permission = 0;
    }
    else if (PsData.permission <= 16){
        //stAuthResult.act = 1;
        stAuthResult.result = AUTH_SUCCESS;
        stAuthResult.permission = PsData.permission;
        intCheckerLogin = 1;
    }
    memset(&msg, 0, sizeof(SYS_UI_MESSAGE));
    msg.msgSender = SCARD_SENDER_ID;
    msg.cmd = CMD_INT_CS_AUTH_RESULT;
    memcpy(msg.data, &stAuthResult, sizeof(stAuth_Result));
    DBG_PRINTF_SDH_ERR("(SendSysMgr) Sent Result Message, act: %d, res: %d, per: %d\n", stAuthResult.act, stAuthResult.result, stAuthResult.permission);
    err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
    XASSERT( err==OK, err );
}

void sendMessageTest(int testStatus)
{
    error_type_t err;
    SYS_UI_MESSAGE msg;
    stAuth_Req stAuthReqTest;
    stAuthReqTest.action = testStatus;
    stAuthReqTest.jobType = 1;
    stAuthReqTest.jobCount = 12;

    memset(&msg, 0, sizeof(SYS_UI_MESSAGE));
    msg.msgSender = SCARD_SENDER_ID;
    msg.cmd = CMD_INT_SC_AUTH_REQ;
    memcpy(msg.data, &stAuthReqTest, sizeof(stAuth_Req));
    err = SysUI_MsgSend(SCARD_RECEIVER_ID, &msg);
    XASSERT( err==OK, err );
    DBG_PRINTF_SDH_ERR("(SendTest) Sent Req Message, action: %d, type: %d, count: %d\n", stAuthReqTest.action, stAuthReqTest.jobType, stAuthReqTest.jobCount);
}

void sendMessageCardLogin(void)
{   
    if(CheckerAuth == 2)
        sendMessageToSysmgr(0, 4);

    error_type_t err;
    SYS_UI_MESSAGE msg;
    stAuth_Req stAuthReqTest;
    stAuthReqTest.action = AUTH_INT_LOGIN_CARD;
    stAuthReqTest.jobType = 0;
    stAuthReqTest.jobCount = 0;

    memset(&msg, 0, sizeof(SYS_UI_MESSAGE));
    msg.msgSender = SCARD_SENDER_ID;
    msg.cmd = CMD_INT_SC_AUTH_REQ;
    memcpy(msg.data, &stAuthReqTest, sizeof(stAuth_Req));
    err = SysUI_MsgSend(SCARD_RECEIVER_ID, &msg);
    XASSERT( err==OK, err );
    DBG_PRINTF_SDH_ERR("(SendMsgThr) Sent Req Message, action: %d, type: %d, count: %d\n", stAuthReqTest.action, stAuthReqTest.jobType, stAuthReqTest.jobCount);
}

int AuthVerify(char *id, char *passwd, int authMod){
    
    if(CheckerPrintAuth == 0){
        DBG_PRINTF_SDH_ERR("(uPrintCmd) Print Auth Verify SP off\n");
        return 0;    
    }
    
    char auth[100];
    char *sub = {"&password="};
    memset(auth, 0x00, 100);
    if (authMod == 0)
        sprintf(auth,"%s%s%s",id, sub, passwd);
    else if (authMod == 1)
        strncpy(auth, id, strlen(id));
    
    DBG_PRINTF_SDH_ERR("(AuthVerify) authMod: %d, string: %s, size: %d\n", authMod, auth, strlen(auth));

    do_card_code((strlen(auth)+1), auth, AUTH_INT_VERIFY, 32, 0);

    if(upResult == 0 && upPermission == 1)
        return 0;
    else
        return 1;
}

int DBReader(int readType){
    error_type_t db_retval = FAIL;
	CheckerAuth = db_get_var_int("CONF_SOLUTIONSP", "AuthType", &db_retval);
    if ( db_retval == FAIL ) { return 1;}
            
    if ( CheckerAuth > 0) {
        CheckerPrintAuth = db_get_var_int("CONF_SOLUTIONSP", "PrintAuthVerify", &db_retval);
        if ( db_retval == FAIL ) { return 1;}
        CheckerCard = db_get_var_int("CONF_SOLUTIONSP", "CardReader", &db_retval);
        if ( db_retval == FAIL ) { return 1;}
        CheckerPup = db_get_var_int("CONF_SOLUTIONSP", "PersonalUPrint", &db_retval);
        if ( db_retval == FAIL ) { return 1;}
        CheckerPaperCut = db_get_var_int("CONF_SOLUTIONSP", "PaperCut", &db_retval);
        if ( db_retval == FAIL ) { return 1;}
    }
    else if (CheckerAuth == 0){
        CheckerPrintAuth = 0;
        CheckerCard = 0;
        CheckerPup = 0;
        CheckerPaperCut = 0;
        CheckerKARAS = 0;
    }
    DBG_PRINTF_SDH_ERR("(DBReader) DB Read complete \n");
    if (IS_KARAS){
        if (CheckerPaperCut == 1) {
            CheckerKARAS = 0;
        }
        else {
            CheckerKARAS = 1;
        }
    }
    else {
        CheckerKARAS = 0;
        CheckerPaperCut = 0;
    }
    return 0; 
}

int AccChecker(void){
    if (CheckerAuth > 0){
	    error_type_t db_retval = FAIL;
        /*char IntAdminID[AUTH_BASIC_LENGTH];
        char IntAdminPwd[AUTH_BASIC_LENGTH];
        memset(IntAdminID, 0x00, AUTH_BASIC_LENGTH);
        memset(IntAdminPwd, 0x00, AUTH_BASIC_LENGTH);
        */
        DBG_PRINTF_SDH_ERR("(Account) Check ID  %s : %s\n", AuthInfo.userID, db_get_var("CONF_SOLUTIONUP", "AdminID", &db_retval));
        DBG_PRINTF_SDH_ERR("(Account) Check Password  %s : %s\n", AuthInfo.userPwd, db_get_var("CONF_SOLUTIONUP", "AdminID", &db_retval));
    
        if( 0 == strcmp(AuthInfo.userID, db_get_var("CONF_SOLUTIONUP", "AdminID", &db_retval))) {
            if ( 0 == strcmp(AuthInfo.userPwd, db_get_var("CONF_SOLUTIONUP", "AdminPW", &db_retval))){
                DBG_PRINTF_SDH_ERR("(Account) Admin !! \n");
                return 0; // AUTH SUCCESS
            }
            DBG_PRINTF_SDH_ERR("(Account) Admin password WRONG!! \n");
            return 2; // AUTH_FAIL_WRONGID
        }
        else 
            DBG_PRINTF_SDH_ERR("(Account) No Admin \n");
            return 1; // AUTH_FAIL_NOID

    }
    else
        return -1; // NOW Support just auth mode
}

void setAdmin(int adminResult){
    DBG_PRINTF_SDH_ERR("(Admin) setting admin info.\n");
    	// init PsData Variables
    initParsedData();

    PsData.permission = 16;
    PsData.result = adminResult;
    strcpy(PsData.upID, "ADMIN");
    writeGlobalVar((strlen(PsData.upID)+1));
    sendMessageToSysmgr(0, 1); 
}

void cardUp(void){
    DBG_PRINTF_SDH_ERR("(cardUp) Send card info to sysmgr.\n");
    	// init PsData Variables
    initParsedData();
    initGlobalVar();
    strncpy(AuthInfo.userID, code, MAX_CODE_LENGTH);
    sendMessageToSysmgr(0, 5); 
}


// Callback Func. for CURL. allocating Memory for storing web-response
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */
		DBG_PRINTF_SDH_ERR("(curl_callback) not enough memory (realloc returned NULL)\n");
		exit(EXIT_FAILURE);
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

int uPrintCmd(char *upIP, char *upID, char *mfpIP){
    if(CheckerPup == 0) {
        DBG_PRINTF_SDH_ERR("(uPrintCmd) Personal UPrint SP off\n");
        return 0;
    }

    CURL *curl;
    CURLcode res;
    char server[1000];
    /* Minimalistic http request */
    char request[100];
    curl_socket_t sockfd; /* socket */
    long sockextr;
    size_t iolen;
    char ipAddr[16];
    memset(ipAddr, 0x00, 16);
	strncpy(ipAddr, mfpIP, 15);
    DBG_PRINTF_SDH_ERR("(uPrintCmd) IP Verify - %s : %s | %d : %d\n", mfpIP, ipAddr, strlen(mfpIP), strlen(ipAddr));

    memset(server, 0x00, 1000);
    memset(request, 0x00, 100);
    sprintf(server,"%s:%d", upIP, UP_PORT);
    sprintf(request,"%s_%s",upID, mfpIP);
    DBG_PRINTF_SDH_ERR("(uPrintCmd) Server: %s, Msg: %s\n", server, request);

    curl = curl_easy_init();
    if(curl) {
        //curl_easy_setopt(curl, CURLOPT_URL, "10.15.20.93:9100");
        curl_easy_setopt(curl, CURLOPT_URL, server);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
        /* Do not do the transfer - only connect to host */
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    
        res = curl_easy_perform(curl);

        if(CURLE_OK != res)
        {
            DBG_PRINTF_SDH_ERR("(uPrintCmd) Error: %d, %s\n", res, curl_easy_strerror(res));
            return 1;
        }
        res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sockextr);

        if(CURLE_OK != res)
        {
            DBG_PRINTF_SDH_ERR("(uPrintCmd) Error 2: %s\n", curl_easy_strerror(res));
            return 1;
        }
        DBG_PRINTF_SDH_ERR("(uPrintCmd) port: %ld \n", sockextr);
        sockfd = sockextr;
        res = curl_easy_send(curl, request, strlen(request), &iolen);

        if(CURLE_OK != res)
        {
            DBG_PRINTF_SDH_ERR("(uPrintCmd) Error 3: %s\n", curl_easy_strerror(res));
            return 1;
        }
        DBG_PRINTF_SDH_ERR("(uPrintCmd) Sending Succeed: %d bytes\n", iolen);
        curl_easy_cleanup(curl);
    }
    return 0;
}

// Connecting Server Using CURL
int ConnectServer(char *server, char **memory)
{
	//    printf("(SCard) Call Server \n");
	//    printf("%s(%d)\n", __FILE__, __LINE__);
	//initialize the curl variables
	CURL *curl;
	CURLcode res;

	//initialize the String for get the web-data
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0;
	//initialize the curl
	curl = curl_easy_init();
	if(curl) {
		//load URL
		curl_easy_setopt(curl, CURLOPT_URL, server);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		//ignore certificates error
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		//allocate the memory for web-data
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		//write the data
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
        //printf("(SCard) Set opt \n");
		res = curl_easy_perform(curl);
        //printf("(SCard) Perform \n");
		if(res != CURLE_OK) {
			fprintf(stderr, "(SCard) curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            DBG_PRINTF_SDH_ERR("\r(Curl) Full Address: %s\terror\n", server);
            curl_easy_cleanup(curl);
			if(chunk.memory)
				free(chunk.memory);
			return 1;
		}
		//printf("(SCard_temp) %s, size: %d \n", chunk.memory, chunk.size);
		if (chunk.size > 0) {
			*memory = realloc(*memory, chunk.size+1);
			memset(*memory, 0x00, chunk.size+1);
			memcpy(*memory, chunk.memory, chunk.size+1);
			//            printf("(SCard_temp)WebData realloc %s, size: %d \n", *memory, strlen(*memory));
            //printf("(SCard) Memory write\n");
		}
		curl_easy_cleanup(curl);
		if(chunk.memory) {
			free(chunk.memory);
            //printf("(SCard) Memory free\n");
		}
	}
	DBG_PRINTF_SDH_ERR("\r(Curl) Full Address: %s\tcomplete\n", server);
	return 0;
}

int JsonParser(char *memory)
{
	// init. the jsmn vars.
	int r=0;
	jsmn_parser p;
	jsmntok_t tok[JSON_TOK_MAX];

	// var for string process
	int i = 0;
	int k = 0;
	char *data;
	int size = 0;

	// init PsData Variables
    initParsedData();

	// run json parser
	jsmn_init(&p);
	r = jsmn_parse(&p, memory, tok, JSON_TOK_MAX);
	//    printf("(SCard_JSON) Size of contents : %d \n", tok[0].size);


	for(i = 0; i <= tok[0].size; i++) {
		k = 0;
		size = 0;
		data = (char *)malloc(NUM_MEM);
		memset(data, 0x00, NUM_MEM);
		if ((tok[i].type != JSMN_OBJECT) && (tok[i].type != JSMN_ARRAY) && (tok[i].type != JSMN_STRING) && (tok[i].type != JSMN_PRIMITIVE)) 
			break;
		/*
		   for(j = tok[i].start; j <= tok[i].end; j++) {
		   if (j == tok[i].end)
		   data[k] = NULL;                    
		   else
		   data[k] = memory[j];
		   k++;
		   }
		 */
		strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
		if(tok[i].type == JSMN_STRING && 0 == strcmp(data, "allow")) {
			i++;
			memset(data, 0x00, NUM_MEM);
			strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(data);
			PsData.permission = atoi(data);
			DBG_PRINTF_SDH_ERR("(JSON) Num: %d, Type: Allow, Numcast is %d, size is %d.\n", i, PsData.permission, size);
		}
		else if( 0 == strcmp(data, "Auth")){
			i++;
            memset(data, 0x00, NUM_MEM);
			strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(data);
			PsData.result = atoi(data);
            DBG_PRINTF_SDH_ERR("(JSON) Num: %d, Auth: %d, Size: %d\n", i, PsData.result, size);
		}
		else if( 0 == strcmp(data, "Email")){
			i++;
			strncpy(PsData.email, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(PsData.email);
			DBG_PRINTF_SDH_ERR("(JSON) Num: %d, Email: %s, Size: %d\n", i, PsData.email, size);
			//            memset(data, 0x00, NUM_MEM);
			//            strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
			//            size = strlen(data);
			//            printf("Object number is %d, Email Address is %s, size is %d\n", i, data, size);
		}
		else if( 0 == strcmp(data, "FtpServer")){
			i++;
			strncpy(PsData.ftpServer, memory+tok[i].start, tok[i].end-tok[i].start); 
			size = strlen(PsData.ftpServer);
			DBG_PRINTF_SDH_ERR("(JSON) Num: %d, FtpPath: %s, Size: %d\n", i, PsData.ftpServer, size);
		}
		else if( 0 == strcmp(data, "FtpPort")){
			i++;
            memset(data, 0x00, NUM_MEM);
			strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(data);
			PsData.ftpPort = atoi(data);
            DBG_PRINTF_SDH_ERR("(JSON) Num: %d, FtpPort: %d, Size: %d\n", i, PsData.ftpPort, size);
		}
		else if( 0 == strcmp(data, "UserID")){
			i++;
			strncpy(PsData.ftpID, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(PsData.ftpID);
			DBG_PRINTF_SDH_ERR("(JSON) Num: %d, FtpID: %s, Size: %d\n", i, PsData.ftpID, size);
		}
		else if( 0 == strcmp(data, "Passwd")){
			i++;
			strncpy(PsData.ftpPwd, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(PsData.ftpPwd);
			DBG_PRINTF_SDH_ERR("(JSON) Num: %d, Passwd: %s, Size: %d\n", i, PsData.ftpPwd, size);
		}
		else if( 0 == strcmp(data, "Prefer")){
			i++;
            memset(data, 0x00, NUM_MEM);
			strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
            if( 0 == strcmp(data, "EMAIL"))
	    		PsData.prefer = 0;
            else 
                PsData.prefer = 1;
			DBG_PRINTF_SDH_ERR("(JSON) Num: %d, Prefer: %s, Code: %d, Size: %d\n", i, data, PsData.prefer, size);
		}
        else if( 0 == strcmp(data, "Quota")){
			i++;
            memset(data, 0x00, NUM_MEM);
			strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(data);
			PsData.quotaRemain = atoi(data);
            DBG_PRINTF_SDH_ERR("(JSON) Num: %d, FtpPort: %d, Size: %d\n", i, PsData.quotaRemain, size);
		}
        else if( 0 == strcmp(data, "upID")){
			i++;
			strncpy(PsData.upID, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(PsData.upID);
			DBG_PRINTF_SDH_ERR("(JSON) Num: %d, uPrintID: %s, Size: %d\n", i, PsData.upID, size);
		}
		else if( 0 == strcmp(data, "upIP")){
			i++;
			strncpy(PsData.upIP, memory+tok[i].start, tok[i].end-tok[i].start);
			size = strlen(PsData.upIP);
			DBG_PRINTF_SDH_ERR("(JSON) Num: %d, uPrintIP: %s, Size: %d\n", i, PsData.upIP, size);
		}
 
		else {
			size = strlen(data);
			DBG_PRINTF_SDH_ERR("(JSON) Object number is %d, Value is %s, size is %d\n", i, data, size);
		}
		free(data);
	}
	return 0;
}

int JsonParserVerify(char *memory)
{
    // init. the jsmn vars.
    int r=0;
    jsmn_parser p;
    jsmntok_t tok[JSON_TOK_MAX];

    // var for string process
    int i = 0;
    int k = 0;
    char *data;
    int size = 0;

    // init up Variables
    upResult = 0;
    upPermission = 0;

    // run json parser
    jsmn_init(&p);
    r = jsmn_parse(&p, memory, tok, JSON_TOK_MAX);
    //    printf("(SCard_JSON) Size of contents : %d \n", tok[0].size);


    for(i = 0; i <= tok[0].size; i++) {
        k = 0;
        size = 0;
        data = (char *)malloc(NUM_MEM);
        memset(data, 0x00, NUM_MEM);
        if ((tok[i].type != JSMN_OBJECT) && (tok[i].type != JSMN_ARRAY) && (tok[i].type != JSMN_STRING) && (tok[i].type != JSMN_PRIMITIVE)) 
            break;
        /*
           for(j = tok[i].start; j <= tok[i].end; j++) {
           if (j == tok[i].end)
           data[k] = NULL;                    
           else
           data[k] = memory[j];
           k++;
           }
         */
        strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
        if(tok[i].type == JSMN_STRING && 0 == strcmp(data, "allow")) {
            i++;
            memset(data, 0x00, NUM_MEM);
            strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
            size = strlen(data);
            upPermission = atoi(data);
            DBG_PRINTF_SDH_ERR("(JSON_Verify) Num: %d, Type: Allow, Numcast is %d, size is %d.\n", i, upPermission, size);
        }
        else if( 0 == strcmp(data, "Auth")){
            i++;
            memset(data, 0x00, NUM_MEM);
            strncpy(data, memory+tok[i].start, tok[i].end-tok[i].start);
            size = strlen(data);
            upResult = atoi(data);
            DBG_PRINTF_SDH_ERR("(JSON_Verify) Num: %d, Auth: %d, Size: %d\n", i, upResult, size);
        }
        else {
            size = strlen(data);
            DBG_PRINTF_SDH_ERR("(JSON_Verify) Object number is %d, Value is %s, size is %d\n", i, data, size);
        }
        free(data);
    }
    return 0;
}


void writeGlobalVar(int codeLength){
    initGlobalVar();
//    printf("(SCard_write) code: %s, codeLength: %d\n", code, codeLength);
//    strncpy(AuthInfo.authID, code, codeLength);
    if(PsData.upID[0] != 0x00)
        strcpy(AuthInfo.authID, PsData.upID);
    if(PsData.email)
        strcpy(AuthInfo.email, PsData.email);
    if(PsData.ftpServer)
        strcpy(AuthInfo.ftpServer, PsData.ftpServer);
    if(PsData.ftpID)
        strcpy(AuthInfo.ftpID, PsData.ftpID);
    if(PsData.ftpPwd)
        strcpy(AuthInfo.ftpPwd, PsData.ftpPwd);
    if(PsData.ftpPort != 0)
        AuthInfo.ftpPort = PsData.ftpPort;
    if(PsData.quotaRemain != 0)
        AuthInfo.quotaRemain = PsData.quotaRemain;
    if(PsData.prefer != 0)
        AuthInfo.prefer = PsData.prefer;
    printGlobalVar();
}

void initParsedData(void){
    memset(PsData.upID, 0x00, AUTH_BASIC_LENGTH);
    memset(PsData.upIP, 0x00, AUTH_BASIC_LENGTH);
    memset(PsData.email, 0x00, AUTH_EMAIL_LENGTH);
    memset(PsData.ftpServer, 0x00, AUTH_FTPADDR_LENGTH);
    memset(PsData.ftpID, 0x00, AUTH_BASIC_LENGTH);
    memset(PsData.ftpPwd, 0x00, AUTH_BASIC_LENGTH);
    PsData.ftpPort = 0;
    PsData.quotaRemain = 0;
    PsData.prefer = 0;
    PsData.permission = 0;
    PsData.result = 4;
}

void initGlobalVar(void){
    memset(AuthInfo.authID, 0x00, AUTH_BASIC_LENGTH);
    memset(AuthInfo.email, 0x00, AUTH_EMAIL_LENGTH);
    memset(AuthInfo.ftpServer, 0x00, AUTH_FTPADDR_LENGTH);
    memset(AuthInfo.ftpID, 0x00, AUTH_BASIC_LENGTH);
    memset(AuthInfo.ftpPwd, 0x00, AUTH_BASIC_LENGTH);
    AuthInfo.ftpPort = 0;
    AuthInfo.quotaRemain = 0;
    AuthInfo.prefer = 0;
}

void printGlobalVar(void){
    DBG_PRINTF_SDH_ERR("(Gloval) AuthID: %s \n", AuthInfo.authID);
    DBG_PRINTF_SDH_ERR("(Gloval) Email: %s  \n", AuthInfo.email);    
    DBG_PRINTF_SDH_ERR("(Gloval) FtpServer: %s \n", AuthInfo.ftpServer);
    DBG_PRINTF_SDH_ERR("(Gloval) FtpID: %s \n", AuthInfo.ftpID);
    DBG_PRINTF_SDH_ERR("(Gloval) FtpPwd: %s \n", AuthInfo.ftpPwd);
    DBG_PRINTF_SDH_ERR("(Gloval) FtpPort: %d \n", AuthInfo.ftpPort);
    DBG_PRINTF_SDH_ERR("(Gloval) Remain Quota: %d \n", AuthInfo.quotaRemain);
    DBG_PRINTF_SDH_ERR("(Gloval) Prefer(0:email/1:ftp) : %d \n", AuthInfo.prefer);
}

int keycode(int code)
{
	switch(code){
		case 4 : return 97; // a
		case 5 : return 98;
		case 6 : return 99;
		case 7 : return 100;
		case 8 : return 101;
		case 9 : return 102;
		case 10 : return 103;
		case 11 : return 104;
		case 12 : return 105;
		case 13 : return 106;
		case 14 : return 107;
		case 15 : return 108;
		case 16 : return 109;
		case 17 : return 110;
		case 18 : return 111;
		case 19 : return 112;
		case 20 : return 113;
		case 21 : return 114;
		case 22 : return 115;
		case 23 : return 116;
		case 24 : return 117;
		case 25 : return 118;
		case 26 : return 119;
		case 27 : return 120;
		case 28 : return 121;
		case 29 : return 122;
		case 30 : return 49; // 1
		case 31 : return 50;
		case 32 : return 51;
		case 33 : return 52;
		case 34 : return 53;
		case 35 : return 54;
		case 36 : return 55;
		case 37 : return 56;
		case 38 : return 57;
		case 39 : return 48; // 0    
		case 40 : return 10; // enter   
				  //        case  : return ;    
				  //        case  : return ;    
	}
	return 0;
}

