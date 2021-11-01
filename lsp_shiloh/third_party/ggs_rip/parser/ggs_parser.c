/* Copyright (C) 2011-2014 Global Graphics Software Ltd. All rights reserved.
*
* This example is provided on an "as is" basis and without
* warranty of any kind. Global Graphics Software Ltd. does not
* warrant or make any representations regarding the use or results
* of use of this example.
*
* Portions Copyright (c) 2011-2014 Marvell International, Ltd. All Rights Reserved
*/
 
 /**
 * Description: Potentially interprets the following streams, depending upon core RIP build configuration.
 *   PostScript
 *   PCL5c
 *   PCL5e
 *   PCL6
 *   PDF
 *   XPS
 *   TIFF
 *   JPEG
 *   PNG
 *
 */

// If you don't define HAVE_GGSLIB then you won't need to link with GGSlibs, and 
// this parser module can still be initialised
#define HAVE_GGSLIB

// Overriding tx_semaphore_create with PMS_tx_create_semaphore... see function at end of this source code
//#define OVERRIDE_TXE_SEMAPHORE_CREATE
#ifdef OVERRIDE_TXE_SEMAPHORE_CREATE
#undef _txe_semaphore_create
#endif



#include <string.h>
#include "lassert.h"
#include "agRouter.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "agMessage.h"
#include "posix_ostools.h"
#include "memAPI.h"
#ifdef OLIVE
#include "agiLogger.h"
#else
#include "logger.h"
#endif
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "sys_init_api.h"
#ifdef HAVE_PJL
#ifdef OLIVE
#include "agpjlAPI.h"
#else
#include "pjl_api.h"
#endif
#endif
#include "debug.h"
#include "ResourceMap.h"
#ifdef OLIVE
#include "pepPageParams.h"
#endif
#include "BigBuffer.h"
#include "PrintJobInfo.h"
#include "print_job_api.h"

#include "panda_api.h"
#include "platform.h"
#include "SYS_UI_msg_interface.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif


#define DBG_PRFX "ggs: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 22 ) 

// Multi-level debug output macros; allows easy increase/decrease of debug
// output printf statements.
#define dbg1 DBG_PRINTF_NOTICE
#define dbg2 DBG_PRINTF_NOTICE
#define dbg3 DBG_PRINTF_NOTICE
#define dbg_log DBG_PRINTF_ERR	// @@	2013.08.28	by Juny

char PDLPARSER_VERSION[]={"3.2.32"};

/* 2013.12.02		req. from version	 by Juny */
// first : PDL lib version, second : PDL parser version, third : update version
/* history
 * 
 * 3.2.32 : Landscape A4 PDF File 출력 시 EC312 대책
 * 3.2.31 : Insert_LastBlankPage when job is [Odd # page, Duplex, LETTERHEAD]
 * 3.2.30 : QTY=1 일때 Collate On->Off 로 변경 (Juniper rev.23663)
 * 3.2.29 이하는 Juniper 와의 구분을 위해 사용 안함
 * 
 * 3.2.3-1 : 2018.11.15: Custom Media Size 대책 최소한만 적용 (PDL4.2r12 연동)
 * 3.2.3 : 2017.11.06: add two more media size for duplex print (Legal, US Folio(8.5*13))
 * 3.2.2 : 2017.10.24: added saving and then printing image file for pc solution print
 * 3.2.1 : 2016.09.09: stack size of ggs_parser(message loop) thread was fixed (256K -> 16K)
 * 3.2.0 : 2016.05.31: GGS PDL4.2 적용 (define FREETYPE 적용) - 일단 적용 안함
 * 3.1.06-Tray: 2015.03.27: PJL MEDIASOURCE 를 사용 (특주!!! define PJL_TRAY 사용해야함)
 * 3.1.06: 2014.08.29: MEDIATYPE_RECYCLED 사용
 * 3.1.05: 2014.08.28: USB PDF File Print 시 A4/LT 가 아니면 duplex 강제 해제
 * 3.1.04: 2014.08.28: sPrint rotate 동작 추가
 * 
 3.1.03a		2014.04.09 fix a function not used from Marvell's bug
 3.1.03		2013.12.18 update e_Doc type (req. jcshin)
------------------------------------------------------------------------------------------
 3.1.02		2013.12.13 update tray issue (PS N-up tray goes tray1)
 3.1.01		2013.12.12 distinguish GDI/PDL
 3.1.00		2013.12.02 set version
*/

char* get_version_PDLParser()
{
   return (&PDLPARSER_VERSION[0]);
}

/** local definitions **/
//#define GGS_TESTPAGE /* Hardcoded test page to test API usage */
bool ggs_global_cancel = false;  ///< cancel flag from parser message loop to ggs system. Cleared by parser.
bool ggs_global_cancel_request = false; ///< cancel flag from parser message loop to ggs system. Cleared by ggs.
int ggs_global_testpage = 0;  ///< test page flag from parser message loop to ggs system.
MODULE_ID gDestination; // For callback \todo Tidy this up                 
static CURRENT_RESOURCE  *OurResource;
UINT32  GGS_LASTPARSEERROR=0;

extern UINT32 CalRequested;

int ggs_resolution = 600;
static bool gray_output = true;
bool copy_output = false;
static char szConfig[128];

char gszFileOnDisk[SDH_FILEPATH_MAX]="";

extern int IsPCImagePrint;
/*
static char *cmyk_4_cfg = "ebdwrapper -m 128 -x 600 -h 4 -d 1 -r 2 -e bypass -v on -z 10 -o PANDA -t 0";
static char *rgb_24_cfg = "ebdwrapper -m 256 -x 600 -d 8 -r 6 -e bypass -v on -z 10 -o PANDA -t 0";
// Dan: enable PDF and backchannel
//static char *mono_8_cfg = "ebdwrapper -m 64 -x 600 -h 4 -d 8 -r 1 -e bypass -v on -z 10 -o PANDA -t 0";
//static char *mono_8_cfg = "ebdwrapper -m 64 -x 600 -h 4 -d 8 -r 1 -e bypass -v on -z 10 -o PANDAPDF -t 2";
static char *mono_8_cfg = "ebdwrapper -m 96 -x 600 -h 4 -d 8 -r 1 -e bypass -v on -z 10 -o PDF -t 2";
static char *copy_cfg = "ebdwrapper -m 128 -x 300 -h 4 -d 8 -r 1 -e bypass -v on -z 10 -o PANDA -t 0"; 
*/

//static char *mono_1_cfg = "ebdwrapper -h 4 -d 1 -r 1 -e bypass -v on -z 10 ";
//static char *cmyk_4_cfg = "ebdwrapper -h 4 -d 1 -r 2 -e bypass -v on -z 10 ";
static char *rgb_24_cfg = "ebdwrapper -d 8 -r 6 -e bypass -v on -z 10 -p 2";
static char *mono_8_cfg = "ebdwrapper -h 4 -d 8 -r 1 -e bypass -v on -z 10 -p 2";
static char *copy_cfg   = "ebdwrapper -h 4 -d 8 -r 1 -e bypass -v on -z 10 -p 2";
/* default settings */

int l_nArg_t = 0; /* Unidirectional comms */
//int l_nArg_t = 1; /* backchannel data */
//int l_nArg_t = 2; /* Tagged backchannel data (to allow raster and debug message to be returned via pipe */
int l_nArg_m = 64;
int l_nArg_x = 600; 
int l_nArg_y = 600;
int l_nArg_o = 4; /* PANDA (target) */
//int l_nArg_o = 0; /* NONETRAD (for traditional job performance timing)... NONEAPI includes time to pass data through API */
//int l_nArg_o = 2; /* PDF (raster sent back via backchannel, requires -t 2) */
//int l_nArg_o = 3; /* PANDAPDF (paperless path AND raster sent back via backchannel, requires -t 2) */
char l_szArgs_o[5][16] = { "NONETRAD", "NONEAPI", "PDF", "PANDAPDF", "PANDA" };
int l_nArg_pdl = 4; /* PCL5 (default) */
int fileprint = 0;

bool l_bRestartGGS = true; /* used to control total restart of GGS */


/** external functions **/

/** external data **/

/** internal functions **/
void *ggs_Parser( void *unused );
void ggs_cmd_debug_init();

/** public data **/
IOReg * gGGSPipe = NULL;

/** private data **/
static char CmpStringPS[]="%!";
static char ParseStringPS[]="POSTSCRIPT";
static char ParseStringPCL[]="PCL";
static char ParseStringXL[]="PCLXL";
static char ParseStringImage[]="IMAGE_FILE";
static char ParseStringImagePC[]="IMAGE_PC";
static char ParseStringPDF[]="PDF_FILE";     ///< pdf from filename is different from pdf from stream.
#define GGS_TESTPAGE_VIA_ENTERLANGUAGE
#ifdef GGS_TESTPAGE_VIA_ENTERLANGUAGE
static char ParseStringPSFontList[]="PSFONTLIST";
static char ParseStringPCLFontList[]="PCLFONTLIST";
#endif
static mqd_t GGSParseQ;
#define NUM_MESSAGES 10

static pthread_t GGSParser; ///< main message loop thread 
static ALIGN8 UINT8  GGSStack[POSIX_MIN_STACK_SIZE];

#ifdef HAVE_GGSLIB
#define GGSSTACK_SIZE (256 * 1024)  
static pthread_t GGSSDK;  ///< rip thread
static ALIGN8 UINT8  GGSSDKStack[GGSSTACK_SIZE];
#endif
//extern void DebugPageData(char *pszModule, PAGE_DATA *Page_Data);
//static unsigned char gGGSBuffer[16000] ALIGN16;    


extern void * PMS_CreateSemaphore(int initialValue);
extern int PMS_IncrementSemaphore(void * semaHandle);
extern int PMS_WaitOnSemaphore(void * semaHandle, unsigned int nMilliSeconds);
extern int PMS_WaitOnSemaphore_Forever(void * semaHandle);
extern void PMS_Delay(int nMilliSeconds);
extern void PMS_DestroySemaphore(void * semaHandle);
//extern void PMS_DisplaySemas(TX_SEMAPHORE * pSema);

extern int GGSmain(char *szCL);
extern int GGSconfig(char *szCL);


#ifdef threadx
extern void PMS_UnresolvedInit();
#endif

sem_t semGGSRIPRun;     ///< post once per job from main message loop.
void *g_semGGSRIPRun = (void*)&semGGSRIPRun;

/** public functions **/
bool force_copy = false;
static void set_ggs_config()
{
    if(copy_output) {
	force_copy = true;
	ggs_resolution = 600; // copy resolution testing.
	minSprintf(szConfig, "%s -t %d -o %s -m %d -x %d -y %d -pdl %d", 
		   copy_cfg, l_nArg_t, l_szArgs_o[l_nArg_o], l_nArg_m, ggs_resolution, ggs_resolution, l_nArg_pdl ); 
    } else {
	if (gray_output) { 
	    ggs_resolution = l_nArg_x = l_nArg_y = 600;
	    // ggs_resolution = l_nArg_x = l_nArg_y = 300;  // for testing use.
	    minSprintf(szConfig, "%s -t %d -o %s -m %d -x %d -y %d -pdl %d", 
		       mono_8_cfg, l_nArg_t, l_szArgs_o[l_nArg_o], l_nArg_m, l_nArg_x, l_nArg_y, l_nArg_pdl); 
	} else {
	    ggs_resolution = 600;
	    minSprintf(szConfig, "%s -t %d -o %s -m %d -x %d -y %d -pdl %d", 
		       rgb_24_cfg, l_nArg_t, l_szArgs_o[l_nArg_o], 256, l_nArg_x, l_nArg_y, l_nArg_pdl); 
	}
    }
}

void *ggs_ParserInit(void * unused)
{
    PARSE_SETUP Parser;

#ifdef threadx 
    PMS_UnresolvedInit();
#endif

    dbg2("ggs_ParserInit\n");
    DBG_PRINTF_ERR("pdl parser ver=%s\n", &PDLPARSER_VERSION[0] );
    DBG_PRINTF_ERR("===========================================\n");

    Parser.ParserID = GGS_RIP_ID;
    /* Identify GGS parser as the 'garbage' parser to catch PCL (and text) jobs
       as well as PS, XPS, and PDF */
    Parser.ParseCmpString = NULL;
    Parser.StringSize = 0;
    Parser.ParseString = ParseStringPCL;
#ifdef OLIVE
    Parser.ParseType = e_Printing;      // parser type.
    Parser.PipeType = e_Print;
#else
    Parser.ParseType = e_Print;      // parser type.
#endif
    ParserRegister(&Parser);        // register the parser.

    Parser.ParseCmpString = CmpStringPS;
    Parser.StringSize = strlen(CmpStringPS);
    Parser.ParseString = ParseStringPS;
    ParserRegister(&Parser);        // register the parser.

    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1; // not zero that is PCL
    Parser.ParseString = ParseStringXL;
    ParserRegister(&Parser);        // register the parser.

    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1; // not zero that is PCL
    Parser.ParseString = ParseStringPDF;
    ParserRegister(&Parser);        // register the parser.

    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1; // not zero that is PCL
    Parser.ParseString = ParseStringImage;
    ParserRegister(&Parser);        // register the parser.

    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1; // not zero that is PCL
    Parser.ParseString = ParseStringImagePC;
    ParserRegister(&Parser);        // register the parser.

#ifdef GGS_TESTPAGE_VIA_ENTERLANGUAGE
    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1; // not zero that is PCL
    Parser.ParseString = ParseStringPCLFontList;
    ParserRegister(&Parser);        // register the parser.

    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1; // not zero that is PCL
    Parser.ParseString = ParseStringPSFontList;
    ParserRegister(&Parser);        // register the parser.
#endif
    
    rm_register(e_GGSParser, GGS_RIP_ID);

    gray_output = ( NUM_MARKING_COLOR_CHANNELS == 4 ) ? false : true; // rgb/gray output

    ggs_cmd_debug_init();

    posix_create_MESSAGE_queue( &GGSParseQ, "/ggs_q", 10 );
    router_register_queue(GGS_RIP_ID, GGSParseQ); 
    posix_create_thread(&GGSParser, ggs_Parser, 0, "ggs_parser", GGSStack,
                        POSIX_MIN_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    return(0);
}



/** private functions **/






int ggs_Write(IOReg *Pipe, char *pBuffer, int nLength)
{
  char * pSendBuf;
  if(!Pipe)
    return 0;

  pSendBuf = (char*)MEM_MALLOC(nLength);
  memcpy(pSendBuf, pBuffer, nLength);
  return Pipe->WriteConnect(Pipe, pSendBuf, nLength);
}

void *ggs_sdk( void *unused ) 
{
#ifdef HAVE_GGSLIB
  while(1) {

    // initialise. config can change before job (see GGSconfig call in message loop)
    set_ggs_config();
    dbg3("Setting rip config \"%s\"\n", szConfig);

    if(l_bRestartGGS) {
      l_bRestartGGS = false;
      dbg3("calling GGSmain(\"%s\")\n", szConfig);
      GGSmain(szConfig);
      dbg3("GGSmain returned\n");
    } 

  }
#endif
  return 0;
}

void parse_str_path( char *str, uint32_t str_len, IOReg *Pipe )
{
    int cnt;
    int i;

    cnt = Pipe->pipe_read(Pipe, str, str_len, PARSEREAD_SOME);
    for (i = 0; cnt > 0; i++, cnt--)
    {
        if (str[i] == 0 || str[i] == '\n') break;
    }
    str[i] = 0;
    if (cnt > 1)  // no need to rewind NULL/cr/lf
    {
        Pipe->pipe_read_rewind(Pipe, &str[i], cnt);
    }
}

void save_image_pc_solution(char *str, uint32_t str_len, IOReg *Pipe )
{
	IsPCImagePrint =1;
	dbg_log("starting save_image_pc_solution\n");
	FILE* fp =NULL;
	int count=0;
	int totalCount =0;
	char* prnBuffer = MEM_MALLOC(65536);
	ASSERT(prnBuffer);
	fp =fopen("/tmp/save_image.jpg","wb");
	do
	{
		if(fp)
		{
			count =Pipe->ReadConnect(Pipe,prnBuffer, 16384, PARSEREAD_SOME);
			totalCount+=count;
			if(count>0)
				fwrite(prnBuffer,1,count,fp);
			posix_sleep_ms(10);
		}
	}while(count>0);

	if(fp)
	{
		fclose(fp);
	}
	else
	{
		dbg_log("file open failed for PC image print\n");
	}
	MEM_FREE_AND_NULL(prnBuffer);
	strncpy(str,"/tmp/save_image.jpg",str_len);
}

void enter_language_parser_compare_string( const char * str, IOReg *Pipe )
{
    ggs_global_testpage = 0;
    fileprint = 0;
    strcpy(gszFileOnDisk, "");
    /* l_nArg_pdl (1=PS/Image, 2=XPS, 3=PDF, 4=PCL5c, 5=PCL5e, 6=PCLXL) */
    if (0 == str) {
	dbg3(" %s %s != %s \n", __FUNCTION__, ParseStringPCL, "error NULL" );
	l_nArg_pdl=4;
#ifdef GGS_TESTPAGE_VIA_ENTERLANGUAGE
	/* PCL is a substring of PCLFONTLIST, so check for font list strings first */
    } else if ( 0 == (strncmp(ParseStringPSFontList, str, strlen(ParseStringPSFontList) ) ) ) {
	dbg3(" %s %s != %s \n", __FUNCTION__, ParseStringPSFontList, "error NULL" );
	l_nArg_pdl=1;
	ggs_global_testpage = 2;
    } else if ( 0 == (strncmp(ParseStringPCLFontList, str, strlen(ParseStringPCLFontList) ) ) ) {
	dbg3(" %s %s != %s \n", __FUNCTION__, ParseStringPCLFontList, "error NULL" );
	l_nArg_pdl=4;
	ggs_global_testpage = 3;
#endif
    } else if ( 0 == (strncmp(ParseStringXL, str, strlen(ParseStringXL) ) ) ) {	
	dbg3(" %s %s == %s \n", __FUNCTION__, ParseStringXL, str ); 
	l_nArg_pdl=6;
    } else if ( 0 == (strncmp(ParseStringPS, str, strlen(ParseStringPS) ) ) ) {
	dbg3(" %s %s == %s \n", __FUNCTION__, ParseStringPS, str );
	l_nArg_pdl=1;
    } else if ( 0 == (strncmp(ParseStringPDF, str, strlen(ParseStringPDF) ) ) ) {
	dbg3(" %s %s == %s \n", __FUNCTION__, ParseStringPDF, str );
	l_nArg_pdl=3;
	parse_str_path( gszFileOnDisk, SDH_FILEPATH_MAX, Pipe );
	fileprint = 1;
    } else if ( 0 == (strncmp(ParseStringPCL, str, strlen(ParseStringPCL) ) ) ) {	
	dbg3(" %s %s == %s \n", __FUNCTION__, ParseStringPCL, str ); 
	l_nArg_pdl=4;
    } else if ( 0 == (strncmp(ParseStringImage, str, strlen(ParseStringImage) ) ) ) {
	dbg3(" %s %s == %s \n", __FUNCTION__, ParseStringImage, str );
	l_nArg_pdl=1; /* Images handled by PostScript interpreter */
	parse_str_path( gszFileOnDisk, SDH_FILEPATH_MAX, Pipe );
	//fileprint = 1;
    }else if(0 == (strncmp(ParseStringImagePC, str, strlen(ParseStringImagePC) ) ) ){
	dbg3(" %s %s == %s \n", __func__, ParseStringImagePC, str );
	dbg_log(" %s %s == %s \n", __func__, ParseStringImagePC, str );
	l_nArg_pdl=1; /* Images handled by PostScript interpreter */
	save_image_pc_solution(gszFileOnDisk, SDH_FILEPATH_MAX, Pipe);
	} else {
	dbg3(" %s %s != %s \n", __FUNCTION__, str, "error UNSUPPORTED enter language" );
	l_nArg_pdl=4;
    }
}

/*
 ** ggs_Parser
 *
 *  PARAMETERS:
 *      unused      Unused param to match threadx API
 *      
 *  DESCRIPTION:  This function is called by the system parser when a Parse string is located.  
 *                   The data in the buffer is parsed and then more data is read until it is finished 
 *                   when this function will return to the calling program.  
 *
 *  RETURNS:
 *
 */
void *ggs_Parser( void * unused )
{
    MESSAGE     nextMsg;
    IOReg *Pipe = NULL;
    MODULE_ID Destination;
    int32_t ParseRet = 0;
    void *nextMsg_param3 = 0;

    SysWaitForInit ();

#ifdef HAVE_GGSLIB

    sem_init(g_semGGSRIPRun, 0, 0);

    dbg2("thread starting\n");
    //tx_thread_create ( &GGSSDK, "GGS_SDK", ggs_sdk, 0, GGSSDKStack, GGSSTACK_SIZE, THR_PRI_NORMAL, THR_PRI_NORMAL, DEFAULT_TIME_SLICE, TX_AUTO_START );
    posix_create_thread(&GGSSDK, ggs_sdk, 0, "ggs_sdk", GGSSDKStack, GGSSTACK_SIZE, POSIX_THR_PRI_NORMAL);
#endif

    while (1)
    {
	posix_wait_for_message(GGSParseQ, (char*)&nextMsg, sizeof(MESSAGE), POSIX_WAIT_FOREVER );

        switch (nextMsg.msgType)
        {
	    // \TODO This is work in progress, there's just enough to get the GGS RIP demo working 
	    case MSG_START_PARSE:
	    {
		STARTRECIPE *StartIt;
	       
		dbg_log(" MSG_START_PARSE Parsing\n");
		
		StartIt = (STARTRECIPE *) MEM_MALLOC( /* get_memory_with_job_mgr_help( */ sizeof(STARTRECIPE));
		ASSERT(StartIt != NULL);
		// we have a connection, get the resources and start.
		//
		Pipe = (IOReg *) nextMsg.param3;        // save the pipe info.
		
		enter_language_parser_compare_string( (char *)nextMsg.param2, Pipe );
		

		nextMsg.msgType = MSG_STARTRECIPE;
		
		if ( fileprint == 1 )
			StartIt->Job = ejob_PrintFile;
		else
			StartIt->Job = ejob_PrintIO;
			
		StartIt->AdditionalResource = e_GGSParser;
		StartIt->Pipe = Pipe;
		StartIt->Wait = e_WaitForever;
		nextMsg.param3 = StartIt;       // freed by sjm
		
		SYMsgSend(SJMID, &nextMsg);         // send the message.
		break;
	    }
	    case MSG_RESOURCES:
	    {
	    
		OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
		dbg3(" Resource %x\n",OurResource);
		break;
	    }
	    case MSG_ACKRECIPE:
	    {

	    
		ASSERT( NULL != Pipe );
            
		// Update status to indicate that a print job is being received.
		// This is necessary to cause the "processing" status message
		// to be displayed.
		// have to figure out what this means.  
		//             nextMsg.param1 = STATUS_INFO_PRINTING;
		//             nextMsg.msgType = MSG_STATUS_EVENT;
		//             nextMsg.param2 = GGSPARSER;
		//             SYMsgSend(STATMGRID, &nextMsg);
		dbg3(" Calling DoParsing %x\n",OurResource);
		
		mlimiter_start( mlimiter_by_name("print"));
		// We have the resources, start the job.
		// ggs_global_cancel = false;

		Destination = OurResource->Destination;
		gDestination = Destination; // For callback \todo Tidy this up

		dbg2(" DoParsing() start job\n");
		gGGSPipe = Pipe;
		dbg_log(" gGGSPipe %p\n", gGGSPipe);
                // Update config and let it go.
		set_ggs_config();
		dbg3("calling GGSconfig(\"%s\")\n", szConfig);
		GGSconfig(szConfig);
		dbg3("GGSconfig returned\n");

		// GGSmain called from ggs_sdk thread is waiting for this semaphore.
		PMS_IncrementSemaphore(g_semGGSRIPRun);

		break;
	    }	
	    case MSG_CANCELJOB:
	    {
		dbg_log(" MSG_CANCELJOB \n" );
		ggs_global_cancel_request = true;
		ggs_global_cancel = true;
		nextMsg_param3 = nextMsg.param3;  // save to return in ACK
/* NG : only for immediatly ACK response		2013.11.20 by Juny */
		break;
	    }

	    case MSG_END_PARSE: // param1 contains ParserReturn 
	    {
		mlimiter_stop( mlimiter_by_name("print"));
/* NG : only for immediatly ACK response		2013.11.20 by Juny */
		if ( ggs_global_cancel ) { // cancel foo
		    DBG_PRINTF_NOTICE(" MSG_CANCELJOB ACK %d \n", ggs_global_cancel_request );
		    nextMsg.msgType = MSG_CANCELJOB;
		    nextMsg.param1 = SYS_ACK;
		    nextMsg.param2 = GGS_RIP_ID;
		    nextMsg.param3 = nextMsg_param3;

		    SYMsgSend( SJMID, &nextMsg ); // param3 preserved.
		    ggs_global_cancel = false;
		}	    

		dbg3(" MSG_FREERECIPE %x\n",OurResource);
		// finished parsing, free the resources.
		nextMsg.msgType = MSG_FREERECIPE;
		nextMsg.param1 = 0;
		nextMsg.param2 = 0;
		nextMsg.param3 = OurResource;
		SYMsgSend(SJMID, &nextMsg); // param3 is untouched and now sent back.

            
		dbg_log(" MSG_END_PARSE %x\n",Pipe);
		// Tell the connection manager the results of the parse.
		nextMsg.msgType = MSG_END_PARSE;
		nextMsg.param1 = (UINT32)ParseRet;
		nextMsg.param2 = 0;		// fix 	2013.11.04	by Juny
		nextMsg.param3 = Pipe;          // The pipe that we are parsing.
		SYMsgSend(CMPARSER, &nextMsg);      // send the response.
		OurResource = NULL;
		Pipe = NULL;


		break;      // end_parse
	    }
	    default:
		break;
	} // switch
    } // while 1
    return 0;
}


#ifdef OVERRIDE_TXE_SEMAPHORE_CREATE
UINT PMS_tx_create_semaphore(TX_SEMAPHORE *semaphore_ptr, CHAR *name_ptr, ULONG initial_count) 
{
  static int nSemaCount = 0;
  UINT nResult;

  if(gnGGSDebug_DisplaySemas>=4) {
    dbg3("ggs_parser.o: sizeof(TX_SEMAPHORE)=%d\n", sizeof(TX_SEMAPHORE));
  }

#ifdef HAVE_GGSLIB
//  PMS_DisplaySemas(NULL); // can't use semaphore_ptr it hasn't been created to use last known one if it hasn't been destroyed
#endif

  if(gnGGSDebug_DisplaySemas>=4) {
    dbg3("PMS_tx_semaphore_create(%p, %s, %d) %d\n", semaphore_ptr, name_ptr, initial_count, ++nSemaCount);
  }
  nResult = _txe_semaphore_create(semaphore_ptr, name_ptr, initial_count);
  if(gnGGSDebug_DisplaySemas>=4) {
    dbg3("PMS_tx_semaphore_created (%p, %s, %d) %d, result %d\n", semaphore_ptr, name_ptr, initial_count, nSemaCount, nResult);
  }

#ifdef HAVE_GGSLIB
//  PMS_DisplaySemas(semaphore_ptr);
#endif

  return nResult;
}
#endif

/* calls moved to panda_api.c */

void MyBreakout() {
}

int chmod(const char *__path, mode_t __mode )
{
  return 0;
}

static int ggs_arg_t_cb( int argc, char *argv[] )
{
	int value;
	if ( argc == 2 ) {
		value = atoi( argv[1] );
		cmd_printf("ggs -t arg was %d is now %d", l_nArg_t, value );
		l_nArg_t = value;
	}
	return CMD_OK;
}

static int ggs_arg_o_cb( int argc, char *argv[] )
{
	int value;
	if ( argc == 2 ) {
		value = atoi( argv[1] );
		if (!(value >= 0 && value < 5)) 
		    return -1;
		cmd_printf("ggs -o arg was %d (%s) is now %d (%s)", 
			   l_nArg_o, l_szArgs_o[l_nArg_o], value, l_szArgs_o[value]);
		l_nArg_o = value;
	}
	return CMD_OK;
}

static int ggs_arg_m_cb( int argc, char *argv[] )
{
	int value;
	if ( argc == 2 ) {
		value = atoi( argv[1] );
		cmd_printf("ggs -m arg was %d is now %d", l_nArg_m, value );
		l_nArg_t = value;
	}
	return CMD_OK;
}

static int ggs_show_config_cb( int argc, char *argv[] )
{
  set_ggs_config();
  cmd_printf("ggs config is %s", szConfig );
	return CMD_OK;
}

static int ggs_gray_cb( int argc, char *argv[] )
{
	gray_output = !gray_output;
	cmd_printf("ggs creates %s", gray_output ? "GRAY" : "RGB");
	return CMD_OK;
}

static int ggs_copy_cb( int argc, char *argv[] )
{
	copy_output = !copy_output;
	cmd_printf("ggs %s", copy_output ? "COPY" : "RGB");
	return CMD_OK;
}


extern int gnBandHeight;
static int ggs_lines_cb( int argc, char *argv[] )
{
	int lines;
	if ( argc == 2 ) {
		lines = atoi( argv[1] );
		cmd_printf("ggs band height was %d is %d", lines, gnBandHeight );
		gnBandHeight = lines;
	}
	return CMD_OK;
}


static int ggs_test_on_disk_cb( int argc, char *argv[] )
{
  int test;
	if ( argc == 2 ) {
		test = atoi( argv[1] );
    switch(test) {
    case 1:
      strcpy(gszFileOnDisk, "/data/ONDISK.pdf");
      break;
    case 2:
      strcpy(gszFileOnDisk, "/data/ONDISK.jpg");
      break;
    case 3:
      strcpy(gszFileOnDisk, "/data/ONDISK.tif");
      break;
    case 4:
      strcpy(gszFileOnDisk, "/data/ONDISK.png");
      break;
    default:
      strcpy(gszFileOnDisk, "");
      break;
    }
		cmd_printf("ggs gszFileOnDisk is \"%s\"", gszFileOnDisk );
	}

	return CMD_OK;
}

void ggs_cmd_debug_init()
{
#ifdef HAVE_CMD
    int i_res = 0; UNUSED_VAR(i_res);
    i_res = cmd_register( "ggs",
                          NULL,
                          NULL,
                          NULL,
                          NULL );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "ggs",
				 "gray",
				 "set gray or color input",
				 NULL,
				 NULL,
				 ggs_gray_cb );
    XASSERT( i_res == CMD_OK, i_res );
    i_res = cmd_register_subcmd( "ggs",
				 "copy",
				 "setup for copy 300dpi gray",
				 NULL,
				 NULL,
				 ggs_copy_cb );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "ggs",
				 "bandheight",
				 "set lines per band",
				 NULL,
				 NULL,
				 ggs_lines_cb );
    XASSERT( i_res == CMD_OK, i_res );

    
    i_res = cmd_register_subcmd( "ggs",
				 "backchannel_mode",
				 "set HqnRip SDK -t arg",
				 NULL,
				 NULL,
				 ggs_arg_t_cb );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "ggs",
				 "output_method",
				 "set HqnRip SDK -o arg",
				 NULL,
				 NULL,
				 ggs_arg_o_cb );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "ggs",
				 "rip_mem",
				 "set HqnRip SDK -m arg",
				 NULL,
				 NULL,
				 ggs_arg_m_cb );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "ggs",
				 "test_on_disk",
				 "Test on disk (1=/data/ONDISK.pdf, 2=/data/ONDISK.jpg, 3=/data/ONDISK.tif, 4=/data/ONDISK.png)",
				 NULL,
				 NULL,
				 ggs_test_on_disk_cb );
    XASSERT( i_res == CMD_OK, i_res );
    
    i_res = cmd_register_subcmd( "ggs",
				 "show_config",
				 "Display rip args",
				 NULL,
				 NULL,
				 ggs_show_config_cb );
    XASSERT( i_res == CMD_OK, i_res );
    
#endif
}
