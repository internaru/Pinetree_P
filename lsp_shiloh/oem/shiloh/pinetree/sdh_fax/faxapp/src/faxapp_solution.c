#include <dirent.h>
#include <string.h>
#include "agRouter.h"
#include "agResourceMgr.h"
#include "sys_init_api.h"
#include "faxapp_solution.h"
#include "SDHFaxCommonHeader.h"
#include "../../../../../../common/network/apps/ports/include/solution.h"
#include "fsu.h"
#include "db_api.h"
#if __HAVE_FAXSOLUTION_DK__
#include "faxapp_job.h"
#endif

static unsigned char FaxAppSolutionStack[FAXAPP_SOLUTION_STACK_SIZE] ALIGN8;
static mqd_t faxapp_solution_msgq;
static pthread_t faxapp_solution_thd_id;
void *faxapp_solution_mgr (void *unused);

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern int fsu_delete_solution_job(UINT16 jnum);
#endif

char gFaxPathToFile[MAX_STRING_LEN] = {0, };

extern char gFaxEmailUserName[MAX_STRING_LEN];
extern char gFaxEmailID[MAX_STRING_LEN];
extern char gFaxEmailPW[MAX_STRING_LEN];
extern char gFaxSenderEmailAddress[MAX_STRING_LEN];
extern char gFaxSenderServerAddress[MAX_STRING_LEN];
extern char gFaxReceiverEmailAddress[MAX_STRING_LEN];
extern char gFaxEmailSubject[MAX_STRING_LEN];
extern char gFaxEmailText[MAX_STRING_LEN];
extern char gFaxFTPID[MAX_STRING_LEN];
extern char gFaxFTPPW[MAX_STRING_LEN];
extern char gFaxFTPAddress[MAX_STRING_LEN];
extern char gFaxFTPPath[MAX_STRING_LEN];
extern char gFaxSMBID[MAX_STRING_LEN];
extern char gFaxSMBPW[MAX_STRING_LEN];
extern char gFaxSMBAddress[MAX_STRING_LEN];
extern char gFaxSMBPath[MAX_STRING_LEN];
extern char gFaxWebDavID[MAX_STRING_LEN];
extern char gFaxWebDavPW[MAX_STRING_LEN];
extern char gFaxWebDavAddress[MAX_STRING_LEN];
extern char gFaxWebDavPath[MAX_STRING_LEN];
extern char *pCurrentDirName;
extern char *pPreviousDirName;
extern char *pRootFolderNameSolDocMan;
extern int gFaxSendDestination;
extern int gFaxEmailPort;
extern int gFaxFTPPort;
extern int gFaxSMBPort;
extern int gFaxWebDavPort;

extern UINT8 fsu_get_status (UINT16 jnum);
extern int fsu_get_memory_full_status_nand(void);


void faxapp_solution_request_send (int jnum)
{
    printf("\n(YHCHO) %s() - jnum: %d, gFaxSendDestination: %d\n", __FUNCTION__, jnum, gFaxSendDestination);

    memset(gFaxPathToFile, 0x00, sizeof(gFaxPathToFile));

    snprintf(gFaxPathToFile, sizeof(gFaxPathToFile), "%s%cImgDoc%04X", pRootFolderNameSolDocMan, PATH_SEPARATOR, jnum);

    switch (gFaxSendDestination)
    {
        case SEND_DESTINATION_EMAIL:
        {
            faxapp_solutioin_send_to_email(jnum);

            break;
        }

        case SEND_DESTINATION_FTP:
        {
            faxapp_solutioin_send_to_ftp(jnum);

            break;
        }

        case SEND_DESTINATION_SMB:
        {
            faxapp_solutioin_send_to_smb(jnum);

            break;
        }

        case SEND_DESTINATION_WEBDAV:
        {
            faxapp_solutioin_send_to_webdav(jnum);

            break;
        }

        default:
        {
            printf("\n(YHCHO) %s() - Invalid gFaxSendDestination: %d\n", __FUNCTION__, gFaxSendDestination);

            break;
        }
    }
}


void faxapp_solutioin_send_to_email (int jnum)
{
    int sendResult = 0;
#if 0
    int SecureType = 1;
    int AuthType = 1;
#endif
    int isDebugMode = 0;
    stSolution_Request sendRequestMsg;
    error_type_t db_retval = FAIL;

    /* Request Message Queue */
    memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));

    sendRequestMsg.sendRequestAppMsgQueueId = FAXAPPSOLUTIONID;

    sendRequestMsg.sendJobId = jnum;

    /* Subject */
    if(strlen(gFaxEmailSubject) == 0)
    {
        snprintf(gFaxEmailSubject, sizeof(gFaxEmailSubject), "Received Fax Image");
    }

    /* Text */
    if(strlen(gFaxEmailText) == 0)
    {
        snprintf(gFaxEmailText, sizeof(gFaxEmailText), "The attached is received fax image.");
    }

    /* Request Info. */
    printf("\n(YHCHO) %s() - gFaxEmailUserName: %s", __FUNCTION__, gFaxEmailUserName);
    printf("\n(YHCHO) %s() - gFaxEmailID: %s", __FUNCTION__, gFaxEmailID);
    printf("\n(YHCHO) %s() - gFaxEmailPW: %s", __FUNCTION__, gFaxEmailPW);
    printf("\n(YHCHO) %s() - gFaxSenderEmailAddress: %s", __FUNCTION__, gFaxSenderEmailAddress);
    printf("\n(YHCHO) %s() - gFaxSenderServerAddress: %s", __FUNCTION__, gFaxSenderServerAddress);
    printf("\n(YHCHO) %s() - gFaxEmailPort: %d", __FUNCTION__, gFaxEmailPort);
    printf("\n(YHCHO) %s() - gFaxPathToFile: %s", __FUNCTION__, gFaxPathToFile);
    printf("\n(YHCHO) %s() - gFaxReceiverEmailAddress: %s", __FUNCTION__, gFaxReceiverEmailAddress);
    printf("\n(YHCHO) %s() - gFaxEmailSubject: %s", __FUNCTION__, gFaxEmailSubject);
    printf("\n(YHCHO) %s() - gFaxEmailText: %s", __FUNCTION__, gFaxEmailText);
    //printf("\n(YHCHO) %s() - SecureType: %d", __FUNCTION__, SecureType);
    //printf("\n(YHCHO) %s() - AuthType: %d", __FUNCTION__, AuthType);
    printf("\n(YHCHO) %s() - isDebugMode: %d\n", __FUNCTION__, isDebugMode);
#if 0
    /* Send Request */
    sendResult = sendMailWrapper(gFaxEmailID, gFaxEmailPW, gFaxSenderEmailAddress, gFaxSenderServerAddress, gFaxEmailPort, gFaxPathToFile, gFaxReceiverEmailAddress, gFaxEmailSubject, gFaxEmailText, SecureType, AuthType, isDebugMode, &sendRequestMsg);

    if(sendResult == -1)
    {
        printf("\n(YHCHO) %s() - sendMailWrapper() Error!\n", __FUNCTION__);
    }
    else
    {
        printf("\n(YHCHO) %s() - sendMailWrapper() OK!\n", __FUNCTION__);
    }
#else

    email_info_message_t *email_info=malloc(sizeof(email_info_message_t));
    if(email_info)
    {
        memset(email_info,0,sizeof(email_info_message_t));
        /* Mail ID, username */
        email_info->username = (char *)malloc(strlen(gFaxEmailID)+1);
        if(email_info->username != NULL)
        {
			strcpy(email_info->username, gFaxEmailID);
        }

        /* Mail Password, password */
        email_info->password = (char *)malloc(strlen(gFaxEmailPW)+1);
        if(email_info->password != NULL)
        {
			strcpy(email_info->password, gFaxEmailPW);
        }

        /* senderEmail, DB:DefaultMailAddress */
        email_info->senderEmail = (char *)malloc(strlen(gFaxSenderEmailAddress)+1);
        if(email_info->senderEmail != NULL)
        {
			strcpy(email_info->senderEmail, gFaxSenderEmailAddress);
        }

        /* senderName, DB:DefaultMailUserName */
        email_info->senderName = (char *)malloc(strlen(gFaxEmailUserName)+1);
        if(email_info->senderName != NULL)
        {
			strcpy(email_info->senderName, gFaxEmailUserName);
        }
        
        /* mailhost, DB:DefaultMailServer */
        email_info->mailhost = (char *)malloc(strlen(gFaxSenderServerAddress)+1);
        if(email_info->mailhost != NULL)
        {
			strcpy(email_info->mailhost, gFaxSenderServerAddress);
        }

        /* port, Mail Port, DB:DefaultMailPort*/
        email_info->port = gFaxEmailPort;
        
        /* attachmentPath */
        email_info->attachmentPath = (char *)malloc(strlen(gFaxPathToFile)+1);
        if(email_info->attachmentPath != NULL)
        {
			strcpy(email_info->attachmentPath, gFaxPathToFile);
        }

        /* Receiver */
        email_info->toAddress = (char *)malloc(strlen(gFaxReceiverEmailAddress)+1);
        if(email_info->toAddress != NULL)
        {
			strcpy(email_info->toAddress, gFaxReceiverEmailAddress);
        }

        /* Receiver:CC */
        //email_info->ccAddress = NULL;
        
        /* Receiver:BCC */
        //email_info->bccAddress = NULL;
        
        /* Subject (Title) */
        email_info->subject = (char *)malloc(SDH_EMAIL_SUBJECT_STR_MAX + 1);
        if(email_info->subject != NULL)
        {
			strncpy(email_info->subject, gFaxEmailSubject, SDH_EMAIL_SUBJECT_STR_MAX);
        }

        /* Text */
        email_info->content = (char *)malloc(SDH_EMAIL_TEXT_STR_MAX+1);
        if(email_info->content != NULL)
        {
			strncpy(email_info->content, gFaxEmailText, SDH_EMAIL_TEXT_STR_MAX);
        }

        /* secureType, DB:DefaultMailSecurity */
        email_info->secureType = db_get_var_int("CONF_SYSTEM", "DefaultMailSecurity", &db_retval);
        if (db_retval == FAIL) {
            email_info->secureType = 0;
        }
        
        /* authType, DB:DefaultMailAuth */
        email_info->authType = db_get_var_int("CONF_SYSTEM", "DefaultMailAuth", &db_retval);
        if (db_retval == FAIL) {
            email_info->authType = 0;
        }
        
        /* email_msg */
        email_info->email_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
        if(email_info->email_msg != NULL)
        {
			memset(email_info->email_msg, 0x00, sizeof(stSolution_Request));
			email_info->email_msg->sendRequestAppMsgQueueId = FAXAPPSOLUTIONID;
			email_info->email_msg->sendJobId = jnum;
        }
        
        /* etc */
        email_info->isDebugMode = isDebugMode;
        //email_info->email_type = EMAIL_OTHER;
        //email_info->email_report = NULL;
        //email_info->ifax_request = NULL;
        
        printf("\n(YHCHO) %s() - email_info->subject: %s\n", __FUNCTION__, email_info->subject);
        printf("\n(YHCHO) %s() - email_info->content: %s\n", __FUNCTION__, email_info->content);
        
        sendResult = sendMail(email_info);
        if(sendResult != 0) {
        printf(" [%s]: Email Send[#1] Fail(%d)!\n", __FUNCTION__, sendResult);
        }
        else {
        printf(" [%s]: Email Send[#1] Success!\n",__FUNCTION__);
        }
        
        deleteEmailInfo(email_info);
    }
#endif
}


void faxapp_solutioin_send_to_ftp (int jnum)
{
    int sendResult = 0;
    stSolution_Request sendRequestMsg;
    char FTPServerFullPath[MAX_STRING_LEN * 2] = {0, };

    /* Server Full Path (Address + Path) */
    snprintf(FTPServerFullPath, sizeof(FTPServerFullPath), "%s/", gFaxFTPAddress);

    strncat(FTPServerFullPath, gFaxFTPPath, sizeof(gFaxFTPPath));

    /* Request Message Queue */
    memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));

    sendRequestMsg.sendRequestAppMsgQueueId = FAXAPPSOLUTIONID;

    sendRequestMsg.sendJobId = jnum;

    /* Request Info. */
    printf("\n(YHCHO) %s() - gFaxFTPID: %s", __FUNCTION__, gFaxFTPID);
    printf("\n(YHCHO) %s() - gFaxFTPPW: %s", __FUNCTION__, gFaxFTPPW);
    printf("\n(YHCHO) %s() - gFaxFTPAddress: %s", __FUNCTION__, gFaxFTPAddress);
    printf("\n(YHCHO) %s() - gFaxFTPPath: %s", __FUNCTION__, gFaxFTPPath);
    printf("\n(YHCHO) %s() - FTPServerFullPath: %s", __FUNCTION__, FTPServerFullPath);
    printf("\n(YHCHO) %s() - gFaxFTPPort: %d", __FUNCTION__, gFaxFTPPort);
    printf("\n(YHCHO) %s() - gFaxPathToFile: %s\n", __FUNCTION__, gFaxPathToFile);

    /* Send Request */
    sendResult = ftpUploadWrapper(gFaxFTPID, gFaxFTPPW, FTPServerFullPath, gFaxFTPPort, gFaxPathToFile, 0, NULL, 0, &sendRequestMsg, TYPESYSMGR);

    if(sendResult == -1)
    {
        printf("\n(YHCHO) %s() - ftpUploadWrapper() Error!\n", __FUNCTION__);
    }
    else
    {
        printf("\n(YHCHO) %s() - ftpUploadWrapper() OK!\n", __FUNCTION__);
    }
}

#if __HAVE_FAXSOLUTION_DK__
void faxapp_solution_send_to_ftp_dk (char *path, UINT16 jnum)
{
    int sendResult = 0;
    stSolution_Request sendRequestMsg;
#if 0
    char FTPServerFullPath[MAX_STRING_LEN * 2] = {0, };

    /* Server Full Path (Address + Path) */
    snprintf(FTPServerFullPath, sizeof(FTPServerFullPath), "%s/", gFaxFTPAddress);

    strncat(FTPServerFullPath, gFaxFTPPath, sizeof(gFaxFTPPath));
#endif
    /* Request Message Queue */
    memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));

    sendRequestMsg.sendRequestAppMsgQueueId = FAXAPPSOLUTIONID;

    sendRequestMsg.sendJobId = jnum;

    /* Request Info. */
    printf("\n(YHCHO) %s() - gFaxFTPID: %s", __FUNCTION__, faxapp_get_ftpID_fax_solution());
    printf("\n(YHCHO) %s() - gFaxFTPPW: %s", __FUNCTION__, faxapp_get_ftpPassword_fax_solution());
    printf("\n(YHCHO) %s() - FTPAddress: %s", __FUNCTION__, faxapp_get_ftpAddress_fax_solution());
    printf("\n(YHCHO) %s() - gFaxFTPPort: %d", __FUNCTION__, faxapp_get_ftpPort_fax_solution());
    printf("\n(YHCHO) %s() - gFaxPathToFile: %s\n", __FUNCTION__, path);

    /* Send Request */
    sendResult = ftpUploadWrapper(faxapp_get_ftpID_fax_solution(),
									faxapp_get_ftpPassword_fax_solution(),
									faxapp_get_ftpAddress_fax_solution(),
									faxapp_get_ftpPort_fax_solution(),
									path, 0, NULL, 0, &sendRequestMsg, TYPESYSMGR);

    if(sendResult == -1)
    {
        printf("\n(YHCHO) %s() - ftpUploadWrapper() Error!\n", __FUNCTION__);
    }
    else
    {
        printf("\n(YHCHO) %s() - ftpUploadWrapper() OK!\n", __FUNCTION__);
    }
}
#endif

void faxapp_solutioin_send_to_smb (int jnum)
{
    int sendResult = 0;
    int isDebugMode = 0;
    stSolution_Request sendRequestMsg;

    /* Request Message Queue */
    memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));

    sendRequestMsg.sendRequestAppMsgQueueId = FAXAPPSOLUTIONID;

    sendRequestMsg.sendJobId = jnum;

    /* Request Info. */
    printf("\n(YHCHO) %s() - gFaxSMBID: %s", __FUNCTION__, gFaxSMBID);
    printf("\n(YHCHO) %s() - gFaxSMBPW: %s", __FUNCTION__, gFaxSMBPW);
    printf("\n(YHCHO) %s() - gFaxSMBAddress: %s", __FUNCTION__, gFaxSMBAddress);
    printf("\n(YHCHO) %s() - gFaxSMBPort: %d", __FUNCTION__, gFaxSMBPort);
    printf("\n(YHCHO) %s() - gFaxSMBPath: %s", __FUNCTION__, gFaxSMBPath);
    printf("\n(YHCHO) %s() - gFaxPathToFile: %s", __FUNCTION__, gFaxPathToFile);
    printf("\n(YHCHO) %s() - isDebugMode: %d\n", __FUNCTION__, isDebugMode);

    /* Send Request */
    sendResult = smbUploadWrapper(gFaxSMBID, gFaxSMBPW, gFaxSMBAddress, gFaxSMBPort, gFaxSMBPath, gFaxPathToFile, isDebugMode, &sendRequestMsg, TYPESYSMGR);

    if(sendResult == -1)
    {
        printf("\n(YHCHO) %s() - smbUploadWrapper() Error!\n", __FUNCTION__);
    }
    else
    {
        printf("\n(YHCHO) %s() - smbUploadWrapper() OK!\n", __FUNCTION__);
    }
}


void faxapp_solutioin_send_to_webdav (int jnum)
{
    int sendResult = 0;
    stSolution_Request sendRequestMsg;
    char WebDavServerFullPath[MAX_STRING_LEN * 2] = {0, };

    /* Request Message Queue */
    memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));

    sendRequestMsg.sendRequestAppMsgQueueId = FAXAPPSOLUTIONID;

    sendRequestMsg.sendJobId = jnum;

    /* Server Full Path (Address + Path) */
    snprintf(WebDavServerFullPath, sizeof(WebDavServerFullPath), "%s/", gFaxWebDavAddress);

    strncat(WebDavServerFullPath, gFaxWebDavPath, sizeof(gFaxWebDavPath));

    /* Request Info. */
    printf("\n(YHCHO) %s() - gFaxWebDavID: %s", __FUNCTION__, gFaxWebDavID);
    printf("\n(YHCHO) %s() - gFaxWebDavPW: %s", __FUNCTION__, gFaxWebDavPW);
    printf("\n(YHCHO) %s() - gFaxWebDavAddress: %s", __FUNCTION__, gFaxWebDavAddress);
    printf("\n(YHCHO) %s() - gFaxWebDavPath: %s", __FUNCTION__, gFaxWebDavPath);
    printf("\n(YHCHO) %s() - WebDavServerFullPath: %s", __FUNCTION__, WebDavServerFullPath);
    printf("\n(YHCHO) %s() - gFaxWebDavPort: %d", __FUNCTION__, gFaxWebDavPort);
    printf("\n(YHCHO) %s() - gFaxPathToFile: %s\n", __FUNCTION__, gFaxPathToFile);

    sendResult = webDAVUploadWrapper(gFaxWebDavID, gFaxWebDavPW, WebDavServerFullPath, gFaxWebDavPort, gFaxPathToFile, 0, NULL, 0, &sendRequestMsg, TYPESYSMGR);

    if(sendResult == -1)
    {
        printf("\n(YHCHO) %s() - webDAVUploadWrapper() Error!\n", __FUNCTION__);
    }
    else
    {
        printf("\n(YHCHO) %s() - webDAVUploadWrapper() OK!\n", __FUNCTION__);
    }
}


void faxapp_solution_job_begin (void)
{
    char ImgDocPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    int IndexToJobNo = 0;
    int FileNo = 0;
    struct dirent *pFileExists;
    DIR *pDir = (DIR *)NULL;

    // Step 1. Search for all the /SolDocMan/ImgDocXXXX/ where XXXX is jnum...
    for (IndexToJobNo = 0; IndexToJobNo < TX_JOBS; IndexToJobNo++)
    {
        snprintf(ImgDocPathName, sizeof(ImgDocPathName), "%s%cImgDoc%04X", pRootFolderNameSolDocMan, PATH_SEPARATOR, IndexToJobNo);

        pDir = opendir(ImgDocPathName);

        // Step 2. Check to see whether the /SolDocMan/ImgDocXXXX exists...
        if (pDir == (DIR *)NULL)
        {
            ; // No Action Here...
        }
        else
        {
            while ( (pFileExists = readdir(pDir)) != NULL )
            {
                // Step 3. Don't Care: '.' or '..'
                if (  (strcmp (pFileExists->d_name, pCurrentDirName) == 0) || (strcmp (pFileExists->d_name, pPreviousDirName) == 0) )
                {
                    ; // No Action Here...
                }
                else
                {
                    FileNo++;
                }
            }

            // Step 4. Close the /SolDocMan/ImgDocXXXX
            closedir(pDir);

            if (FileNo == 0)
            {
                // Step 5-1-1. Delete /SolDocMan/ImgXXXX/ since there is no files...

                printf("\n(YHCHO) %s() - There Is No File(s) To Request To Send...\n", __FUNCTION__);

                fsu_delete_dir_on_file_system(ImgDocPathName);
            }
            else
            {
                // Step 5-1-1. Update status to FSU_STATE_SENDING_PENDING...
                fsu_request_status(IndexToJobNo, 8);

                // Step 5-1-2. Request To Send...
                faxapp_solution_request_send(IndexToJobNo);
            }
        }
    }

    if(fsu_get_status((UINT16)0) != FSU_STATE_NULL)
    {
		//Point to the next txq.nxt for the next job
		set_txq_nxt();
	}
}


void faxapp_solution_purge_sol_doc_man (unsigned int JobID, unsigned int Result)
{
    char targetPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char targetFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    DIR *pDir = (DIR *)NULL;
    struct dirent *FileInfo;

    printf("\n(YHCHO) %s -  JobID:%d, Result:%d\n", __FUNCTION__ , JobID, Result);

    snprintf(targetPathName, sizeof(targetPathName), "%s%cImgDoc%04X", pRootFolderNameSolDocMan, PATH_SEPARATOR, JobID);

    printf("\n(YHCHO) %s() - targetPathName: %s\n", __FUNCTION__, targetPathName);

    pDir = opendir(targetPathName);

    if (pDir == (DIR *)NULL)
    {
        printf("\n(YHCHO) %s() - Oops...\n", __FUNCTION__);

        XASSERT(0,0);
    }
    else
    {
        while ( (FileInfo = readdir(pDir)) != NULL )
        {
            if ( (strcmp (FileInfo->d_name, pCurrentDirName) == 0) || (strcmp (FileInfo->d_name, pPreviousDirName) == 0) )
            {
                ; // No Action Here...
            }
            else
            {
                memset(targetFileName, 0x00, sizeof(targetFileName));

                snprintf(targetFileName, sizeof(targetFileName), "%s%cImgDoc%04X%c%s", pRootFolderNameSolDocMan, PATH_SEPARATOR, JobID, PATH_SEPARATOR, FileInfo->d_name);

                fsu_delete_file_on_file_system(targetFileName);
            }
        }

        closedir(pDir);
    }

    fsu_delete_dir_on_file_system(targetPathName);
}


void FaxSolutionJobMgrInit(void)
{
    int retcode = 0;

    //
    // Create the queue
    //
    posix_create_message_queue(&faxapp_solution_msgq, "/FaxAppSolutionQueue", FAXSOLUTIONTASK_QUEUE_SIZE, sizeof(MESSAGE));

    //
    // Register a message queue.
    //
    router_register_queue(FAXAPPSOLUTIONID, faxapp_solution_msgq);

    //
    // Register the resource
    //
    rm_register(e_FaxAppSolution, FAXAPPSOLUTIONID);

    //
    // start the fax app job mgr task
    //
    retcode = posix_create_thread(&faxapp_solution_thd_id, faxapp_solution_mgr, 0, "FaxAppSolutionMgr", FaxAppSolutionStack, FAXAPP_SOLUTION_STACK_SIZE, POSIX_THR_PRI_NORMAL);

    XASSERT( retcode==0, retcode );
}


void *faxapp_solution_mgr (void *unused)
{
    MESSAGE msg;
    UINT pxretcode;

	memset( &msg, 0, sizeof(msg));

    //wait for system init to finish
    SysWaitForInit();

    while (1)
    {
        //
        // Get the message from the queue.
        //
        pxretcode = posix_wait_for_sys_message(faxapp_solution_msgq, (char *)&msg, POSIX_WAIT_FOREVER);

        printf("\n(YHCHO) %s() - msg.msgType: 0x%02X\n", __FUNCTION__, msg.msgType);

        switch (msg.msgType)
        {
            case MSG_SENDJOB_RESULT:
            {
                faxapp_solution_purge_sol_doc_man(msg.param1, msg.param2);
                
                fsu_get_memory_full_status_nand();

                break;
            }

            case MSG_SENDJOB_FOR_AUTH_FAX_RESULT:
            {
                /*
                *	Result sending request of FAX Auth job 
                * 
                *	param1 : Result Send Job ID of ScanApp
                * 	param2 : ErrorCode --> '0' means no error (OK)
                * 
                *  Coding Here
                */
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
                if(msg.param2 != 0)
                {
                    //Delete Fax Job
                    fsu_delete_solution_job((UINT16)msg.param1);
                }
#endif
                break;
            }

            default:
                break;
        }

    } //while (1)

    return 0;
}

