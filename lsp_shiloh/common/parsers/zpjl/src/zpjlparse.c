/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file zpjlparse.c
 *
 * \brief
 *
 */

#include <stdbool.h>
#include <string.h>
#include "posix_ostools.h"
#include "agRouter.h"
#include "agConnectMgr.h"
#include "agMessage.h"
#include "memAPI.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "sys_init_api.h"
#include "print_events_common_api.h"
#include "lassert.h"
#include "stdio.h"
//#include "cpu_api.h"

#include "logger.h"
#ifdef HAVE_PAPER
#include "paper_size_api.h"
#endif
#include "PrintJobInfo.h"

#include "pjl_api.h"
#include "zPJL.h"
#include "zPJL_sym.h"
#include "zPJL_test.h"
#include "zpjlparser.h"
#include "pjl_ustatus.h"
#include "pjl_extra_api.h"
#include "utils.h"
#include "ctype.h" // toupper
#include "db_api.h"
#include <sys/statfs.h> // for checking file system 

#ifndef __linux__
#include "cpu_api.h"
#endif

#include "SysServiceMgr.h"
#include "SysUI_Interface.h"
#include "SysCardReaderMsg.h"

#define DBG_PRFX "pjl: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(9)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

extern char PJLPARSER_VERSION[];
extern WORD_MATCH *pwm_SetEnv, *pwm_GetEnv, *pwm_OnOff, *pwm_Paper;
extern WORD_MATCH *pwm_Commands;
extern bool g_bSystemAuthConfChanged;
int uPrint_available = 0;

extern char gJobName[81]; /*Job name*/
char _File_To_Save[256];//complete path of file to be saved in disk; size need to be modified
extern int PRNDUMPENABLE; /* defined in PrintMgr.c*/
extern Int32 authCheck;
extern Int32 state_auth;

#define FILE_SAVE 1

#include "pjl_variables.h"  // this header file needs the extern's above.


char * (*LocalSymbols)[] = NULL; // pointer to an array of pointers.
PJL_VARS *LocalVars[i_elements + s_elements];
uint16_t NumLocalSymbols = 0;     ///< How many symbols we have registered.
uint16_t NumSetVars=0;        ///< Number of variables defined for set.
static uint16_t NumGetVars=0;        ///< Number of variables defined for get.
static uint16_t NumVariables=0;
static uint16_t NumNewGrammar=0;
int auth_result=-1;

// hopes to observer at init
//uint16_t USBStatus_is_ON = 0;		// USB Status Monitor 		2013.12.19	by Juny 

typedef struct
{
    ATLISTENTRY llist;      // must be first or things will fail
    pjl_process_t *process_function;///< The function called to process this.
    uint16_t key;
} pjl_new_grammar_entry_t;

ATLISTENTRY NewGrammar;

static void*  MemAlloc(Uint32 numBytes, Uint32 flags, RefCon refcon)
    { return MEM_MALLOC(numBytes); }

static int  MemFree(void* p, RefCon refcon)
    { memFree(p); return true; }
 

char PJLCmpString[]="@PJL";
char PJLCmpString1[]="\x1b%-12345X";
char PJLParseString[]="PJL";
#define NUM_PJL_MESSAGES 3
#define PJL_STACK_SIZE POSIX_MIN_STACK_SIZE
#define SIZE_OF_PARSE_BUFFER 10000

static pthread_t    pjlparser_thread_id;
static mqd_t        pjlparser_msgq;
static int          usb_sm_set_flag = 0;		// 2014.01.09		by Juny
int auth_available = 1;

uint8_t PJLStack[PJL_STACK_SIZE] ALIGN8;
static IOReg *Pipe = 0;

static void GrowInWordMatchArray(WORD_MATCH **Ptr, uint8_t Action[], int GramState, uint16_t *VarCnt, WORD_MATCH **NextMatch);
void* zPJLparser(void* input);

static int  DiagnosticProc(void* p, RefCon refcon)
{
    return true;
}

int Search_Directory(const char* path)
{
	struct statfs buf;
	int ret;
	DBG_PRINTF_ERR("complete path =%s\n",path);
	ret=statfs(path,&buf);
	if(ret==0)
	{
		DBG_PRINTF_ERR("file exists \n");
	}
	else
	{
		DBG_PRINTF_ERR("file not present\n");
	}
	return ret;
}

void replacefilename(char* orgFileName, char* _File_To_Save)
{
	char tmp_part1[256]; /*for temporary keeping file name*/
	int retVal=0;
	int i=0;

	do
	{
		retVal=Search_Directory(_File_To_Save);
		if(retVal==0)
		{
			DBG_PRINTF_ERR("Need to create a new file name\n");
			i++;
			memset(tmp_part1,0,256);
			char*p,*q;
			p=strrchr(orgFileName,'.'); /*search for file extension*/
			if(p!=NULL)
			{
				q=orgFileName;
				strncpy(tmp_part1,orgFileName,p-q);
				memset(_File_To_Save,0,256);
				snprintf(_File_To_Save,256,"%s%c%d%s",tmp_part1,'_',i,p);
			}
			else
			{
				memset(_File_To_Save,0,256);
				snprintf(_File_To_Save,256,"%s%c%d",orgFileName,'_',i);
			}
		}
	}while(retVal==0);
}
/// simple get function only works in the pjl parser thread.
PrintJobInfo_t *pji() 
{
	ASSERT(Pipe != 0);
	return Pipe->pjob_info;
}


/// Setup the connection manager to run the parser associated with the LANG string upon exit of pjl.
///
///
bool FindParser(char *str)
{
    ASSERT(Pipe && str);
    return SetNextParserToRunOnThisPipe(Pipe, str);
}


/// This needs to be implemented, as a way to wrap unwrapped pdl jobs with mimimal pjl.
/// In a host based only system this isn't needed. 
///
void MaybeResetPJLVars(void)
{
#ifdef HAVE_SOME_RENDERING_PARSER  
   // NB assumption:Since PDL's and PJL are prevented from running at the same time this needs no mutex.
   // static bool gMarkPJLdefaultsAsDirty = true;
   // Called prior to running an unwrapped PDL job and upon entry to PJL
   // this effectively wraps all jobs with the absolute minimum PJL of copying the DEFAULTS to SET variables.
   // if (gMarkPJLdefaultsAsDirty) 
   // {
   //     ResetPJLVars();
   // }
#endif
}

void pjl_init( void )
{ 
    PARSE_SETUP Parser;

    Parser.ParserID = PJLPARSER;
    Parser.ParseCmpString = PJLCmpString;
    Parser.StringSize = strlen(PJLCmpString);
    Parser.ParseString = PJLParseString;
    Parser.ParseType = e_Print;

    ParserRegister(&Parser);
    Parser.ParseCmpString = PJLCmpString1;
    Parser.StringSize = strlen(PJLCmpString1);

    ParserRegister(&Parser);

    posix_create_message_queue(&pjlparser_msgq, "/PJLParseQ", NUM_PJL_MESSAGES, sizeof(MESSAGE));
    router_register_queue(PJLPARSER, pjlparser_msgq);
    rm_register(e_PJLParser, PJLPARSER);

    ATInitList(&NewGrammar);  /// lsptodo: don't think grammar's are tested.

    posix_create_thread(&pjlparser_thread_id, zPJLparser, 0, "pjlparser", PJLStack, PJL_STACK_SIZE, POSIX_THR_PRI_NORMAL);
} 

static pthread_mutex_t pjl_lock = POSIX_MUTEX_INITIALIZER;

void zpjl_init_pwm_papersize()
{
    uint32_t i;
    const char * my_str;

    static bool once = true;
    //
    // Register all our paper types. and build the wm_Paper and the enum list for paper.
    // See paper_config.c for the table.
    pthread_mutex_lock( &pjl_lock );
    if ( once && pwm_Paper == NULL ) {
	    uint16_t NumPapers = 0;
	    for(i = 0; i < paper_get_num_media_sizes() ; i++) {
		    my_str = paper_get_pjl_name( paper_get_entry(i) );
		    if (my_str) {
			    DBG_PRINTF_INFO( " pjl papers %s %d\n", my_str, NumPapers);
			    RegisterVarEnum( (char*)my_str, &pwm_Paper, &NumPapers);  // cast const away ...
		    } else { // not all papers are pjl selectable.
			    char str[20];
			    sprintf( str, "ZZZ%d", i  );
			    RegisterVarEnum( str, &pwm_Paper, &NumPapers);  // cast const away ...
		    }
	    }
	    once = false;
    } else {
	    DBG_PRINTF_ERR( " pjl papers init done.. ver=%s\n", &PJLPARSER_VERSION[0] );
	    DBG_PRINTF_ERR("===========================================\n");
    }
    pthread_mutex_unlock( &pjl_lock );
}

void zpjl_init_pwm_mediatype()
{
}


/**
 * \brief Config the pjl system with the specifics we need.
 * This adds the papers and variables to the pjl system.  After this the pjl system
 * will handle everything else.
 * This uses the media size driver for the papers and the variables defined in
 * the file pjl_variables.h to setup the variables.
 */
static void LocalPJLRegister(void)
{
	zpjl_init_pwm_papersize();
	zpjl_init_pwm_mediatype();
}

void* zPJLparser(void* input)
{
    MESSAGE nextMsg;
    // bool Parsing=false;
    // CURRENT_RESOURCE  *OurResource = 0;
    int BytesRead;
    char *stream, *start;
    STARTRECIPE *StartIt;

  
    Int32  err;
    ZPJL  zpjl;
    zpjlContextInfo  zpc;

    ZPJLENV  shared_ROM_env = BAD_ZPJLENV;
    ZPJLENV  shared_DEFAULT_env = BAD_ZPJLENV;
    ZPJLENV  shared_CURRENT_env = BAD_ZPJLENV;
    ZPJLENV  shared_USTATUS_env = BAD_ZPJLENV;

    SysWaitForInit();    // wait for the rest of the system to initialize
    var_module_pjl_init();
    LocalPJLRegister();

    memset(&zpc, 0, sizeof(zpjlContextInfo));
    zpc.system.memAllocProc = MemAlloc;
    zpc.system.memFreeProc = MemFree;
    zpc.system.diagnosticProc = DiagnosticProc;
    zpc.system.refcon = 0;

    /*** create the shared environments 
     * lsptodo : remove the vestages of shared environments, replaced with PrintJobInfo system/pipe/job scoped variables.
     */
    err = zpjlConstructEnv(&zpc.system, &shared_ROM_env, NULL,(ZPJL_EnvItem *)__ZPJL_ROM_ENV__, __ZPJL_ROM_ENVSIZE__,0);
    err = (ZPJL_OK != err) ? err :
	    zpjlConstructEnv(&zpc.system, &shared_DEFAULT_env, NULL, (ZPJL_EnvItem *)(uintptr_t)__ZPJL_ROM_ENVSIZE__, 1,0);
    err = (ZPJL_OK != err) ? err :
	    zpjlConstructEnv(&zpc.system, &shared_CURRENT_env, NULL, (ZPJL_EnvItem *)(uintptr_t)__ZPJL_ROM_ENVSIZE__, 1,0);
    err = (ZPJL_OK != err) ? err :
	    zpjlConstructEnv(&zpc.system, &shared_USTATUS_env, NULL,(ZPJL_EnvItem *)__ZPJL_USTATUS_ENV__, __ZPJL_USTATUS_ENVSIZE__,0);
    ASSERT(ZPJL_OK == err);
    zpc.zpenv_ROM = shared_ROM_env;
    zpc.zpenv_DEFAULT = shared_DEFAULT_env;
    zpc.zpenv_CURRENT = shared_CURRENT_env;
    zpc.zpenv_USTATUS = shared_USTATUS_env;
    
    /*** specify the local PJL grammar */
    zpc.rootGrammar = wm_PJL;
    
    /*** specify the max number of args a command can have */
    zpc.maxArgs = 4;

    /*** specify the size of number the outbound message stack */
    zpc.maxOutbound = 4;
    
    zpc.flags = ZPJL_CONTEXT_DEFAULTFLAGS;


    
    err = zpjlConstruct(&zpc, &zpjl);
    ASSERT(ZPJL_OK == err);
    /*** okay, run PJL machinery! */
    /*...*/
    start = MEM_MALLOC(SIZE_OF_PARSE_BUFFER);
    ASSERT(start);


/* hopes to observer at init 
   move setUstatusDevice() for USB Status Monitor 	@@ 2013.12.19		by Juny */
/*	seg fault !!!
        if ( 0 == Pipe->pdevicestatus ) 
        {
            /// each pipe can have different settings 
            /// assumes pipes do not get unregistered and get registered at system startup.
            Pipe->pdevicestatus = 
                PipeObserver_Constructor( Pipe, PipeObserver_ID, pjl_ustatus_device_status_update);

	    /// attaching observer to a singleton so he knows who the subject is.
        DBG_PRINTF_ERR( "init Ustatus : %d \n", Pipe->pdevicestatus);
	    pec_attachobserver_printstatus( Pipe->pdevicestatus );
        }
		else
         DBG_PRINTF_ERR( " CAN'T set Ustatus!!! \n");	// @@ 2013.12.19 by Juny
*/

	error_type_t db_retval = FAIL;
	uPrint_available = db_get_var_int("CONF_SOLUTIONSP", "PrintAuthVerify", &db_retval);
	if(db_retval == FAIL) // fail case
		DBG_PRINTF_ERR( "ERR: DB read failed ! (PrintAuthVerify)\n");
	uPrint_available = uPrint_available && db_get_var_int("CONF_SOLUTIONSP", "AuthType", &db_retval);
	if(db_retval == FAIL) // fail case
		DBG_PRINTF_ERR( "ERR: DB read failed ! (AuthType)\n");
	DBG_PRINTF_ERR( "uPrint_available = [%d]\n", uPrint_available);

    while(1)
    {
        mq_receive(pjlparser_msgq, (char*)&nextMsg, sizeof(MESSAGE), 0);
        switch(nextMsg.msgType)
        {
			case MSG_AUTH_RESULT:
				{
					auth_result = nextMsg.param1;
					break;
				}
            case MSG_START_PARSE:
                {
                    // we have a connection, get the resources and start.
                    //
                    DBG_PRINTF_INFO( "@@ msg START PARSE -> START RECIPE \n" );	// @@ 2013.08.22 by Juny
                    StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
                    ASSERT(StartIt != NULL);
                    Pipe = (IOReg *) nextMsg.param3;        // save the pipe info.
                    nextMsg.msgType = MSG_STARTRECIPE;
                    StartIt->Job = ejob_PJL;
                    StartIt->AdditionalResource = e_PJLParser;
                    StartIt->Pipe = NULL;    // no status to return
                    StartIt->Wait = e_WaitForever;
                    nextMsg.param3 = StartIt;       // freed by sjm.

                    SYMsgSend(SJMID, &nextMsg);         // send the message.
                    // Parsing = true;
                    usb_sm_set_flag = 0;		// reset always before PARSE
                    
                    if ( (Pipe->pjob_info->jobDocType == e_NETIOPAGE) || (Pipe->pjob_info->jobDocType == e_PIPEIOPAGE) )
                    {
						auth_available = 1;
						
						if ( g_bSystemAuthConfChanged )
						{
							error_type_t db_retval = FAIL;
							uPrint_available = db_get_var_int("CONF_SOLUTIONSP", "PrintAuthVerify", &db_retval);
							if(db_retval == FAIL) // fail case
								DBG_PRINTF_ERR( "ERR: DB read failed ! (PrintAuthVerify)\n");
							else
								DBG_PRINTF_ERR( "uPrint_available was changed to [%d]\n", uPrint_available);
								
							uPrint_available = uPrint_available && db_get_var_int("CONF_SOLUTIONSP", "AuthType", &db_retval);
							if(db_retval == FAIL) // fail case
								DBG_PRINTF_ERR( "ERR: DB read failed ! (AuthType)\n");
							else
								DBG_PRINTF_ERR( "uPrint_available was changed to [%d]\n", uPrint_available);
								
							g_bSystemAuthConfChanged = false;
						}
					}
					else
						auth_available = 0;

                    break;
                }
            case MSG_RESOURCES:
                {

                    // OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
                    break;
                }
            case MSG_ACKRECIPE:
                {
                    // We have the resources, start the job.
                    //
                    DBG_PRINTF_INFO( "@@ msg ACK RECIPE : read \n" );	// @@ 2013.08.22 by Juny
#if FILE_SAVE
                    char* temp;
					int diff=0;
					FILE * fp=NULL;
					char tmp_part1[256]; /*for temporary keeping file name*/
					char orgFileName[256];
					int retVal=0;
					int i=0;
#endif
                    stream = start;
                    BytesRead = Pipe->ReadConnect(Pipe, stream, SIZE_OF_PARSE_BUFFER, PARSEREAD_ANY);
#if FILE_SAVE
                    temp=stream;
					diff=BytesRead;
#endif
                    err = ZPJL_MORE;
                    while(err == ZPJL_MORE)
                    {
                        for (; (err == ZPJL_MORE) && ( BytesRead > 0); --BytesRead, ++stream)
                        {
                           err = zpjlPutc(zpjl, *stream);
                        }
                        DBG_PRINTF_ERR( "@@ ByteRead %d \n", BytesRead );	// @@ 2014.01.07 by Juny
#if FILE_SAVE
                        if( (BytesRead>0) && (PRNDUMPENABLE) )
                        {
							diff=diff-BytesRead;
							memset(_File_To_Save, 0, sizeof(_File_To_Save));
							memset(orgFileName,0,sizeof(orgFileName));
							snprintf(_File_To_Save, sizeof(_File_To_Save),"%s%s","/tmp/",gJobName);
							
							strncpy(orgFileName,_File_To_Save,sizeof(orgFileName));
							
							do
							{
								retVal=Search_Directory(_File_To_Save);
								if(retVal==0)
								{
									DBG_PRINTF_ERR("Need to create a new file name\n");
									i++;
									memset(tmp_part1,0,sizeof(tmp_part1));
									char*p,*q;
									p=strrchr(orgFileName,'.'); /*search for file extension*/
									if(p!=NULL)
									{
										q=orgFileName;
										strncpy(tmp_part1,orgFileName,p-q);
										memset(_File_To_Save,0,sizeof(_File_To_Save));
										snprintf(_File_To_Save,sizeof(_File_To_Save),"%s%c%d%s",tmp_part1,'_',i,p);
									}
									else
									{
										memset(_File_To_Save,0,sizeof(_File_To_Save));
										snprintf(_File_To_Save,sizeof(_File_To_Save),"%s%c%d",orgFileName,'_',i);
									}
								}
							}while(retVal==0);

							DBG_PRINTF_ERR("final file name =%s \n",_File_To_Save);
							fp =fopen(_File_To_Save,"wb");
							if(fp)
							{
								fwrite(temp,1,diff,fp);
								fclose(fp);
								fp=NULL;
								DBG_PRINTF_ERR("written header info in parser \n");
							}
							else
							{
								DBG_PRINTF_ERR("file open failed\n");
								char*ptr =NULL;
								ptr=strrchr(_File_To_Save,'\\');
								if( ptr != NULL)
								{
									ptr++;
									memset(orgFileName, 0, sizeof(orgFileName));
									snprintf(orgFileName, sizeof(orgFileName),"%s%s","/tmp/",ptr);
									memset(_File_To_Save,0,sizeof(_File_To_Save));
									strncpy(_File_To_Save,orgFileName,sizeof(_File_To_Save));
									replacefilename(orgFileName,_File_To_Save);
								}
								else
								{
									DBG_PRINTF_ERR("file open failed using default name \n");
									memset(_File_To_Save,0,sizeof(_File_To_Save));
									memset(orgFileName,0,sizeof(orgFileName));
									snprintf(_File_To_Save, sizeof(_File_To_Save),"%s","/tmp/default");
									strncpy(orgFileName,_File_To_Save,sizeof(orgFileName));
									replacefilename(orgFileName,_File_To_Save);
								}
								DBG_PRINTF_ERR("final file name =%s\n",_File_To_Save);
								fp =fopen(_File_To_Save,"wb");
								if(fp)
								{
									fwrite(temp,1,diff,fp);
									fclose(fp);
									fp=NULL;
									DBG_PRINTF_ERR("written header info in parser \n");
								}
							}
						}
#endif
                        if(err == ZPJL_MORE && BytesRead == 0 && usb_sm_set_flag == 1 )
                        {
                    	   DBG_PRINTF_ERR( "@@ USB SM UEL : close \n" );
                           err = ZPJL_CHOKE;   // get out of here.
                           BytesRead = 0;      // make sure rewind will work.
                           usb_sm_set_flag = 0;
                        }
                        else
                           if(err == ZPJL_MORE)
                           {
                               // we ran out of input stream, read more.
                               //
                               BytesRead = Pipe->ReadConnect(Pipe, start, SIZE_OF_PARSE_BUFFER, PARSEREAD_SOME);
                               if(BytesRead <= 0)  // Must be done.
                               {
                                  DBG_PRINTF_INFO( "@@ in msg ACK RECIPE : close \n" );	// @@ 2013.08.22 by Juny
                                  err = ZPJL_CHOKE;   // get out of here.
                                  BytesRead = 0;      // make sure rewind will work.
                               }
                               else
                               {
                                  DBG_PRINTF_ERR( "@@ in msg ACK RECIPE : maybe abnormal \n" );	// @@ 2013.08.22 by Juny
                                  stream = start; // reset buffer pointer.
                               }
                           }
			               else if (err != ZPJL_PDL && stream != start)// put back the character we just choked on. 
                           {  
                              DBG_PRINTF_ERR( "@@ in msg ACK RECIPE : no PJL, maybe abnormal \n" );	// @@ 2013.08.22 by Juny
                              stream--;
                              BytesRead++; 
                           }
                           usb_sm_set_flag = 0;		// reset always before rewinding
                    }

                    DBG_PRINTF_INFO( "@@ in msg ACK RECIPE : rewind -> FREE RECIPE -> END PARSE \n" );	// @@ 2013.08.22 by Juny
                    //
                    // We are finished parsing, put the excess bytes back into the pipe.
                    //
                    Pipe->RewindData(Pipe, stream, BytesRead);// push the data back, if any left

                    if (err == ZPJL_CANCEL)
                    {
						//Pipe->CancelStatus = e_Cancel;
						Pipe->pipe_cancel(Pipe);
						DBG_PRINTF_ERR( "inhwan5: CANCEL STATUS !!!\n");
					}
					else
						Pipe->CancelStatus = e_Accept;

                    // finished parsing, free the resources.
                    // Parsing = false;
                    nextMsg.msgType = MSG_FREERECIPE;
                    SYMsgSend (SJMID, &nextMsg); // param3 is untouched and now sent back.
                    nextMsg.param1 = 0;         // everything was good.
                    nextMsg.param3 = Pipe;      // the pipe we are working on.
                    nextMsg.msgType = MSG_END_PARSE;
                    state_auth = 0;
                    authCheck = 0;
                    SYMsgSend (CMPARSER, &nextMsg);      // send the response to connection mgr.
                    break;      // done
                }
            
            default:
                break;
        }
    }
    return 0;
}

 
 
/* FUNCTION NAME: doOutput */
 
/** 
 * \brief Output the string to the pipe.
 * 
 * \param string
 * 
 * \retval None
 * 
 * \author Dirk Bodily
 * 
 * \date 1/8/2008
 * 
 **/
int doOutput(char *string)
{
    return Pipe->WriteConnect(Pipe, string, strlen(string));
}

 
/* FUNCTION NAME: RegisterVarEnum */
 
/** 
 * \brief Add another enum value to a variable enum list.
 * 
 * \param String
 * 
 * \param Match
 * 
 * \param VarCnt
 * 
 * \retval int
 * 
 * 
 **/
int RegisterVarEnum(char *String, WORD_MATCH **Match, uint16_t *VarCnt)
{
    uint8_t Action[]={A_VALUE,0};

    RegisterGramString(String);
    GrowInWordMatchArray(Match, Action, S_EOL, VarCnt, NULL);
    NumLocalSymbols++;
    return NumLocalSymbols-1;
}
 
/* FUNCTION NAME: RegisterCmpString */
 
/** 
 * \brief This puts the string into the pjl parser grammer table.
 * 
 * \param String
 * 
 * \retval int
 * 
 * 
 **/
void RegisterGramString(char *String)
{
    if(LocalSymbols == NULL)
    {
        LocalSymbols = MEM_MALLOC(sizeof(char*)*32);// need 2 for null terminator.
    }
    else if (NumLocalSymbols + 2 > 32)  // prealloc 32 pointers...
    {
        // get the room for the new variable array.
        LocalSymbols = MEM_REALLOC(LocalSymbols,sizeof(char*)*(NumLocalSymbols + 2));
    }
    ASSERT( NULL != LocalSymbols );
    (*LocalSymbols)[NumLocalSymbols] = String;
    (*LocalSymbols)[NumLocalSymbols+1] = NULL;      // terminate the list.
}

 
 
/* FUNCTION NAME: GrowInWordMatchArray */
 
/** 
 * \brief Given a variable, put in a new entry into the word match array for the variable.
 * 
 * \param[in/out] Ptr This is an array that is reallocated here.
 * 
 * \param Action This is the action to perform for this only 2 entries
 * 
 * \param GramState Zeno defined variable
 * 
 * \param VarCnt The number of variables that have been allocated.
 * 
 * \param NextMatch The next grammar to check.
 * 
 * \retval None
 * 
 **/
static void GrowInWordMatchArray(WORD_MATCH **Ptr, uint8_t Action[], int GramState, uint16_t *VarCnt, WORD_MATCH **NextMatch)
{
    WORD_MATCH *Temp;

    Temp = MEM_REALLOC(*Ptr, sizeof(WORD_MATCH) * (*VarCnt+2));
    ASSERT(Temp != NULL);       // make sure we have memory.  *Ptr is a leak.
    Temp[*VarCnt].key = NumLocalSymbols + __DEFAULT_SYMBOL_TABLE_SIZE__;  // our currentkey
    Temp[*VarCnt].action[0] = Action[0];
    Temp[*VarCnt].action[1] = Action[1];
    Temp[*VarCnt].NextState = GramState;
    Temp[*VarCnt].wm_Next = NextMatch;
    Temp[++(*VarCnt)].key = NULL_SYMBOL;  // terminate the table by writing to the next
    *Ptr = Temp;        // assign return value
}
 
/* FUNCTION NAME: RegisterVariable */
 
/** 
 * \brief Register a new enum variable with the pjl parser.
 * 
 * \param enum_var The info about this variable.
 * 
 * \returns void
 * 
 **/

void pjl_register_variable( pjl_register_variable_t *var, uint32_t index )
{
    uint8_t Action[2];
    PJL_VARS *MyVar;

    // Action specifies what is done.
    Action[1] = 0;
    Action[0] = A_KEY;
    // register this name as part of the grammar.
    RegisterGramString(var->var_name);
    // if set then put this into the set wordmatch location.
    if(var->var_type == e_Set || var->var_type == e_GetSet ) {
	    if ( var->NextMatch ) {   
		    GrowInWordMatchArray(&pwm_SetEnv, Action, S_START_WORDMATCH, &NumSetVars, var->NextMatch);
	    } else {
		    GrowInWordMatchArray(&pwm_SetEnv,Action, S_START_NUMBER, &NumSetVars, 0);
	    }
    }
    // if get then put this into the get workmatch location.
    if(var->var_type == e_Get ||
       var->var_type == e_GetSet)
    {
	    if ( var->NextMatch )    
		    GrowInWordMatchArray(&pwm_GetEnv, Action, S_START_WORDMATCH, &NumGetVars, var->NextMatch);
	    else
		    GrowInWordMatchArray(&pwm_GetEnv, Action, S_START_NUMBER, &NumGetVars, 0);
    }
    // Get the memory for storing this info
    MyVar = MEM_CALLOC(1,sizeof(PJL_VARS));
    ASSERT(MyVar != NULL);      // only happens at startup
    if ( var->NextMatch ) {
	    MyVar->VarUse = e_VarEnum;      // mark this as an enum var.
	    MyVar->enum_object = pjl_enum_open( *var->NextMatch, var->enum_start, var->enum_increment );
    } else {
	    MyVar->VarUse = e_VarNumber;      // mark this as an number var.
    }
    MyVar->pji_index = index; // var->pji_index;


    // 
    // Put in the variable key.
    // The variable key is the next available slot in the grammar table.
    //
    MyVar->VarKey = NumLocalSymbols + (int)__DEFAULT_SYMBOL_TABLE_SIZE__;
    LocalVars[NumVariables] = MyVar;

    NumVariables++;     // now we have a new variable, increment it.
    NumLocalSymbols++;  // increment the pointer

}

/* FUNCTION NAME: GetNumVars */
 
/** 
 * \brief return the number of variables in the list.
 * 
 * \retval None
 * 
 **/
uint32_t GetNumVars(void)
{
    return NumVariables;
}
 
void pjl_register_new_grammar(pjl_reg_grammar_t *new_grammar)
{
    int size =0, i,j;
    WORD_MATCH *tmpCmds;
    pjl_new_grammar_entry_t *my_grammar;

    // the variables already exist, find out how big an allocation is needed.
    i = 0;
    while (pwm_Commands[i++].key != 0xffff)
        size += sizeof(WORD_MATCH);
    tmpCmds = MEM_MALLOC(size + 2 * sizeof(WORD_MATCH));
    ASSERT(tmpCmds);        // during init should not fail
    for (j = 0; j < i; j++)
    {
        memcpy(&tmpCmds[j], &pwm_Commands[j], sizeof(WORD_MATCH));
    }

    RegisterGramString(new_grammar->grammar_entry);
    if(NumNewGrammar++)
    {
        memFree(pwm_Commands);
    }
    i--;  // put this record over the previous termination.
    tmpCmds[i].key = NumLocalSymbols++ + (int)__DEFAULT_SYMBOL_TABLE_SIZE__;
    tmpCmds[i].NextState = S_COMMENT;
    tmpCmds[i].action[0] = A_COMMAND;
    tmpCmds[i].action[1] = 0;
    tmpCmds[i].wm_Next = NULL;
    tmpCmds[i+1].key = 0xffff;
    pwm_Commands = tmpCmds;
    my_grammar = MEM_MALLOC(sizeof(pjl_new_grammar_entry_t));
    ASSERT(my_grammar);
    my_grammar->process_function = new_grammar->process_function;
    my_grammar->key = tmpCmds[i].key;
    ATInitNode(&my_grammar->llist);
    ATInsertHeadList(&NewGrammar,&my_grammar->llist);
}

/**
 * \brief Return the number of grammar words that have been processed.
 * \returns uint32_t
 * \retval The number of grammar words registered.
 */
uint32_t num_new_grammar(void)
{
    return ATNumListNodes(&NewGrammar);   
}
/**
 * \brief Find the count grammar word in the linked list
 * \parampin] count The number of the grammar word to fetch
 * \returns pjl_new_grammar_entry_t
 * \retval Address of the grammar word structure.
 */
pjl_new_grammar_entry_t *FindGrammar(uint32_t count)
{
    ATLISTENTRY *list_ptr;
    uint32_t i;

    ASSERT(count < num_new_grammar());
    list_ptr = ATListHead(&NewGrammar);
    for(i = 0; i < count; i++)
        list_ptr = ATListNext(&NewGrammar, list_ptr);
    return CONTAINING_RECORD(list_ptr, pjl_new_grammar_entry_t, llist);  // since the entry is first this will work
}
/**
 * \brief Get the key for a given grammar word.
 * \param[in] count The number of the grammar word to find.
 * \returns uint16_t
 * \retval The key value
 */
uint16_t get_grammar_key(uint32_t count)
{
    pjl_new_grammar_entry_t *my_grammar;

    my_grammar = FindGrammar(count);    // get the grammar word I am interested in
    ASSERT( NULL != my_grammar );
    return my_grammar->key;
}
/**
 * \brief Run the program registered with this grammar word
 * \param count The count of the variable to run
 * \param InputString The rest of the input string after the grammar word
 * \returns void
 */
void run_grammar_program(uint32_t count, char *InputString)
{
    pjl_new_grammar_entry_t *my_grammar;

    my_grammar = FindGrammar(count);
    ASSERT( NULL != my_grammar );
    my_grammar->process_function(Pipe, InputString);

}
/* FUNCTION NAME: GetVarName */
 
/** 
 * \brief Given an index from the start of a variable, return the variable name in ascii.
 * 
 * \param Index How far to go into the variable list.
 * 
 * \retval char* The variable name
 * 
 * 
 **/
char *GetVarName(uint32_t Index)
{
    return SYM(LocalVars[Index]->VarKey);
}
 
 
/* FUNCTION NAME: GetVarKey */
 
/** 
 * \brief Given an index from the start of the variables, return the variable key.
 * 
 * \param Index The index from the start of the list.
 * 
 * \retval UIN16 The variable key at that index.
 * 
 * \author Dirk Bodily
 * 
 * \date 1/8/2008
 * 
 **/
uint16_t GetVarKey(uint32_t Index)
{
    return LocalVars[Index]->VarKey;
}
 
 
/* FUNCTION NAME: FindVariable */
 
/** 
 * \brief Given a variable find the specifics for the variable.
 * 
 * \param Key The variable key to look for.
 * 
 * \retval PJL_VARS* The variable information
 * 
 * 
 **/
PJL_VARS *FindVariable(uint16_t Key)
{
    uint32_t i;
    for(i = 0; i < NumVariables; i++)
    {
        if(LocalVars[i]->VarKey == Key)
            return LocalVars[i];
    }
    return NULL;
}
 
 
/* FUNCTION NAME: GetVarEnum */
 
/** 
 * \brief return the enum string value for a variable.
 * 
 * \param Key The variable key value.
 * 
 * \param index The index for the enum to return
 * 
 * \retval char* The enum value for this variable.  Must be freed
 **/
char *GetVarEnum(uint16_t Key, uint32_t index)
{
    PJL_VARS    *TempVar;
    
    TempVar = FindVariable(Key);
    if((TempVar == NULL) || pjl_set_val(TempVar->enum_object, index) != OK)
        return NULL;
    
    return pjl_get_string_val(TempVar->enum_object);
}
 
 
/* FUNCTION NAME: FindVariableType */
 
/** 
 * \brief Given a variable key, return the type of variable.
 * 
 * \param Key The variable key
 * 
 * \retval PJL_VARIABLE_PROCESS The type of the variable.
 * 
 **/
PJL_VARIABLE_PROCESS FindVariableType(unsigned short Key)
{
    PJL_VARS *TempVar;
    TempVar = FindVariable(Key);
    if(TempVar)
        return TempVar->VarUse;
    else
        return e_NotDefined;
}
 
 
/** 
 * \brief Get a variable, turn it into a string, and return it.
 * 
 * \param var The variable to look up
 * 
 * \param VarStorage The type of variable.
 * 
 * \retval char* The string, null if value not found.
 * 
 **/
char *get_var_as_str(uint16_t var, PJL_VARIABLE_STORAGE VarStorage)
{
    PJL_VARS *TempVar;
    char *str = 0;

    TempVar = FindVariable(var);
// 		2013.12.17	by Juny
    DBG_PRINTF_ERR("%s inquire var %d finds %d \n", __FUNCTION__, var, TempVar );	// 2013.12.17 	by Juny */
    if( TempVar ) {
	    bool is_default = ( TempVar->scope == e_job_scope || VarStorage == e_DefaultVar );
	    str = MEM_MALLOC(40);
	    if ( TempVar->VarUse == e_VarString ) {
		    const char *cptr = pji_str_field_get_by_index( pji(), TempVar->pji_index, is_default );
		    strncpy( str, cptr, 40 );
	    } else {
		    uint32_t ivalue = pji_int_field_get_by_index( pji(), TempVar->pji_index,  is_default );
		    minSprintf(str, "%d", ivalue);
	    }
    }
    return str;
}

/* USB Status Monitor		2013.12.18	by Juny */
int get_var_as_PJL_papersize_Sindoh(uint32_t var, char *str )
{
    int ret_val = 0;

	switch (var)
	{
		case MEDIASIZE_A4:
			strncpy( str, "A4", 40 );
			break;
		case MEDIASIZE_B5:
			strncpy( str, "B5", 40 );		// from JISB5 req. from gsJung	2013.12.20	by Juny
			break;
		case MEDIASIZE_A5:
			strncpy( str, "A5", 40 );
			break;
		case MEDIASIZE_A5_ROTATED:					// distinguish string   2014.02.26 	by Juny
			strncpy( str, "A5LEF", 40 );
			break;
		case MEDIASIZE_B6_JIS:
			strncpy( str, "B6", 40 );		// from JISB6 req. from gsJung	2013.12.20	by Juny
			break;
		case MEDIASIZE_A6:
			strncpy( str, "A6", 40 );
			break;
		case MEDIASIZE_LEGAL:
			strncpy( str, "LEGAL", 40 );
			break;
		case MEDIASIZE_LETTER:
			strncpy( str, "LETTER", 40 );
			break;
		case MEDIASIZE_STATEMENT:
			strncpy( str, "STATEMENT", 40 );
			break;
		case MEDIASIZE_EXECUTIVE:
			strncpy( str, "EXECUTIVE", 40 );		// from EXE req. from gsJung	2013.12.20	by Juny
			break;
		case MEDIASIZE_CUSTOM_85x13:
			strncpy( str, "US_FOLIO", 40 );
			break;
		case MEDIASIZE_CUSTOM_8x10:
			strncpy( str, "ENGQUATRO", 40 );
			break;
		case MEDIASIZE_CUSTOM_825x14:
			strncpy( str, "G_LG", 40 );
			break;
		case MEDIASIZE_CUSTOM_8x105:	// maybe "G_LT"
			strncpy( str, "G_LT", 40 );		// from GLT req. from gsJung	2013.12.20	by Juny
			break;
		case MEDIASIZE_JAPANESE_POSTCARD:	// 100x148
			strncpy( str, "POSTCARD100X148", 40 );		// fix string   2014.02.26 	by Juny
			break;

		case MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED:	// 148x200
			strncpy( str, "OFUKU148x200", 40 );
			break;
		case MEDIASIZE_DBL_JAPANESE_POSTCARD:
			strncpy( str, "OFUKU200x148", 40 );			// distinguish string   2014.02.26 	by Juny
			break;

		case MEDIASIZE_ENV_9:	// maybe "COM9ENV"
			strncpy( str, "COM9_ENV", 40 );
			break;
		case MEDIASIZE_ENV_10:
			strncpy( str, "COM10_ENV", 40 );
			break;
		case MEDIASIZE_ENV_MONARCH:
			strncpy( str, "MONARCH_ENV", 40 );
			break;
		case MEDIASIZE_ENV_C5:
			strncpy( str, "C5_ENV", 40 );
			break;
		case MEDIASIZE_ENV_C6:
			strncpy( str, "C6_ENV", 40 );
			break;
		case MEDIASIZE_ENV_DL:	//DL_ENV
			strncpy( str, "DL_ENV", 40 );
			break;
		default:
			strncpy( str, "NONE", 40 );
			ret_val = 1;
			break;
	}
	return ret_val;
}


char *get_var_as_str_Sindoh(uint16_t var, PJL_VARIABLE_STORAGE VarStorage)
{
    uint32_t TempVar;
    int ret_Var = 0;
    error_type_t db_retval = FAIL;

    char *str = 0;
    char *Tempstr = 0;
    Tempstr = MEM_MALLOC(40);
    str = MEM_MALLOC(40);

    usb_sm_set_flag = 1;     // 2014.01.09		by Juny
    
    switch (var-3)
    {
	    case 150: 	// INTRAY1SIZE
			TempVar = (uint32_t) db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", &db_retval);
           // change value to paper size then string
			ret_Var = get_var_as_PJL_papersize_Sindoh(TempVar, str);
			DBG_PRINTF_ERR("get  %s \n", str );
			break;
	    case 151: 	// INTRAY2SIZE
			TempVar = (uint32_t) db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", &db_retval);
			ret_Var = get_var_as_PJL_papersize_Sindoh(TempVar, str);
			DBG_PRINTF_ERR("get  %s \n", str );
			break;
	    case 152: 	// INTRAY3SIZE
			TempVar = (uint32_t) db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", &db_retval);
			ret_Var = get_var_as_PJL_papersize_Sindoh(TempVar, str);
			DBG_PRINTF_ERR("get  %s \n", str );
			break;
	    case 153: 	// MPTRAYSIZE
			TempVar = (uint32_t) db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", &db_retval);
			ret_Var = get_var_as_PJL_papersize_Sindoh(TempVar, str);
			DBG_PRINTF_ERR("get  %s \n", str );
			break;
	    case 154: 	// DRUM
			TempVar = (uint32_t) db_get_var_int("INST_DRUMCOUNT", "CountInPercent", &db_retval);
			minSprintf(str, "%d", TempVar);
			break;
	    case 155: 	// TONER
			TempVar = (uint32_t) db_get_var_int("INST_TONERCOUNT", "CountInPercent", &db_retval);
			minSprintf(str, "%d", TempVar);
			break;
	    case 156: 	// DUPLEXER
	    case 157: 	// INTRAY1
	    case 160: 	// MPTRAY
			/* always ON */
			strncpy( str, "ON", 40 );
			break;
	    case 158: 	// INTRAY2
			Tempstr = db_get_var("INST_MACHINE", "T2Version", &db_retval);
			DBG_PRINTF_ERR("T2 %c \n", Tempstr[0] );	// 2013.12.19 	by Juny */
			if (db_retval != OK || Tempstr[0] == '0' || Tempstr == NULL)
				strncpy( str, "OFF", 40 );
			else
				strncpy( str, "ON", 40 );
			break;
	    case 159: 	// INTRAY3
			Tempstr = db_get_var("INST_MACHINE", "T3Version", &db_retval);
			DBG_PRINTF_ERR("T3 %c \n", Tempstr[0] );	// 2013.12.19 	by Juny */
			if (db_retval != OK || Tempstr[0] == '0' || Tempstr == NULL )
				strncpy( str, "OFF", 40 );
			else
				strncpy( str, "ON", 40 );
			break;
        default:
			TempVar = 1;
			break;
    }
    DBG_PRINTF_ERR("%s inquire var %d finds %d \n", __FUNCTION__, var, TempVar );	// 2013.12.17 	by Juny */
    memFree(Tempstr);
    return str;
}



/* FUNCTION NAME: ReturnNumVarMax */
 
/** 
 * \brief Given a variable key, return the max allowed value for that variable.
 * 
 * \param var The variable to find.
 * 
 * \retval int The max value.
 * 
 **/
int ReturnNumVarMax(uint16_t var)
{
    PJL_VARS *TempVar;

    TempVar = FindVariable(var);
    if(TempVar)
    {
        return TempVar->MaxValue;
    }
    return 0;
}
 
 
/* FUNCTION NAME: ReturnNumVarMin */
 
/** 
 * \brief Given a variable return the min allowed value for the variable.
 * 
 * \param var The variable to look up.
 * 
 * \retval int The min value allowed.
 * 
 **/
int ReturnNumVarMin(uint16_t var)
{
    PJL_VARS *TempVar;

    TempVar = FindVariable(var);
    if(TempVar)
        return TempVar->MinValue;
    return 0;
}

 
 
/* FUNCTION NAME: SetNormalEnumVar */
 
/** 
 * \brief Given an enum variable and new value, store the new value.
 * 
 * \param var The variable to store.
 * 
 * \param TempValue The new value to store.
 * 
 * \param VarStorage The type of variable, where to store it.
 * 
 * \retval None
 * 
 **/
void SetNormalEnumVar(uint16_t var, char *TempValue,PJL_VARIABLE_STORAGE VarStorage)
{
    PJL_VARS *TempVar;
#ifdef HAVE_OID
    char string[40];
#endif
    uint32_t index;
    error_type_t status;

    index = 0;

    TempVar = FindVariable(var);
    if (TempVar)
    {
        status = pjl_set_string_val(TempVar->enum_object, TempValue);
        if (status != OK)
            return;
        if (e_DefaultVar == VarStorage)
        {
#ifdef HAVE_OID
            switch( oid_type_id_get( TempVar->oid ) )
            {
                case OID_TYPE_STRING:
                    minSprintf(string, "%d", TempVar->DefaultValue);
                    status = oid_set_string(TempVar->oid, index, string, strlen(string));
                    break;
                case OID_TYPE_UINT32:
                    status = oid_set_uint32( TempVar->oid, index, (uint32_t)TempVar->DefaultValue );
                    break;
                case OID_TYPE_BYTE_ARRAY:
                default:
                    // Need to define how this data will be sent via PJL
                    ASSERT(0);
                    break;
            }
#endif
        }
        else
        {
            //
            // The variable is valid.
            // we found the value, now save it.
            

#ifdef HAVE_OID
            switch( oid_type_id_get( TempVar->oid ) )
            {
                case OID_TYPE_STRING:
                    minSprintf(string, "%d", pjl_get_val(TempVar->enum_object));
                    status = oid_set_string(TempVar->oid, index, string, strlen(string));
                    break;
                case OID_TYPE_UINT32:
                    status = oid_set_uint32( TempVar->oid, index, (uint32_t)pjl_get_val(TempVar->enum_object) );
                    break;
                case OID_TYPE_BYTE_ARRAY:
                default:
                    // Need to define how this data will be sent via PJL
                    ASSERT(0);
                    break;
            }
#endif
        }

        // done.
    }
}

 
/* FUNCTION NAME: SetNormalNumVar */
 
/** 
 * \brief Given a numeric variable and its value, store it.
 * 
 * \param var The variable to store.
 * 
 * \param TempValue The value to store.
 * 
 * \param VarStorage The type of variable.
 * 
 * \retval None
 * 
 **/
void SetNormalNumVar(uint16_t var, int TempValue,PJL_VARIABLE_STORAGE VarStorage)
{
    PJL_VARS *TempVar;
    uint32_t index;
#ifdef HAVE_OID
    char string[40];
#endif
    int32_t oidVal;

    index = 0;

    TempVar = FindVariable(var);
    if(TempVar)
    {
        if(e_DefaultVar == VarStorage)
        {
            oidVal = TempVar->DefaultValue;
            
        } else
        {
            //
            // The variable is valid.
            // 
            // Check to see if it is within range.
            // 
            if(TempValue < TempVar->MinValue ||
               TempValue > TempVar->MaxValue)
                return;         // out of range, don't change it.
            // Just store it.
            oidVal = TempValue;
        }
        #ifdef HAVE_OID
        switch( oid_type_id_get( TempVar->oid ) )
        {
            case OID_TYPE_STRING:
                minSprintf(string, "%d", oidVal );
                oid_set_string(TempVar->oid, index, string, strlen(string));
                break;
            case OID_TYPE_UINT32:
                oid_set_uint32( TempVar->oid, index, (uint32_t)oidVal );
                break;
            case OID_TYPE_BYTE_ARRAY:
            default:
                // Need to define how this data will be sent via PJL
                ASSERT(0);
                break;
        }
        #endif
        // done.
    }
}

/**
 * \brief Setup or tear down observers for the ustatus pjl function
 * \param[in] Value true to setup the ustatus, false to tear it down.
 * 
 */
void setUstatusJob(bool Value)
{
    Observer_t *o = 0;
    if(Value)
    {
        o = Observer_find_by_update(&Pipe->pjob_info->jobstart_observers, 
                                    pjl_ustatus_jobstart_update);
        // as a group so one find is enough.
        if (!o) 
        {
            o = PipeObserver_Constructor(Pipe, pjl_ustatus_jobstart_ID, pjl_ustatus_jobstart_update);
            ATTACH_OBSERVER(Pipe->pjob_info->jobstart_observers, o );
            
            o = PipeObserver_Constructor(Pipe, pjl_ustatus_jobend_ID, pjl_ustatus_jobend_update);            
            ATTACH_OBSERVER(Pipe->pjob_info->jobend_observers, o );

            o = PipeObserver_Constructor(Pipe, pjl_ustatus_canceljob_ID, pjl_ustatus_canceljob_update);
            ATTACH_OBSERVER(Pipe->pjob_info->canceljobend_observers, o );
        }
    }
    else
    {
        o = Observer_find_by_update(&Pipe->pjob_info->jobstart_observers, 
                                    pjl_ustatus_jobstart_update);
        if (o) 
        {
            DETACH_OBSERVER(Pipe->pjob_info->jobstart_observers, o);
            memFree(o);
        }

        o = Observer_find_by_update(&Pipe->pjob_info->jobend_observers, 
                                    pjl_ustatus_jobend_update);
        if (o) 
        {
            DETACH_OBSERVER(Pipe->pjob_info->jobend_observers, o);
            memFree(o);
        }

        o = Observer_find_by_update(&Pipe->pjob_info->canceljobend_observers, 
                                    pjl_ustatus_canceljob_update);
        if (o) 
        {
            DETACH_OBSERVER(Pipe->pjob_info->canceljobend_observers, o);
            memFree(o);
        }
    }
}
/**
 * \brief Set up a ustatus observer for a timed status
 * If Time is not within range this will do nothing.
 * \brief Time The number of seconds between updates.
 */
void setUstatusTimed(uint32_t Time)
{

    set_observer_pjl_ustatus_timed(Pipe, Time); // 0 is off, else time in seconds.
}
/**
 * \brief Return the time value for the current ustatus timed state.
 * \return unsigned int
 * \retval # of seconds between updates, 0 is off.
 */
unsigned int UstatusTimedState(void)
{

    if (Pipe->tdevicestatus)
        return get_observer_pjl_ustatus_timed_seconds(Pipe);
    else
        return 0;
}
/**
 * \brief Return the current state of the ustatus job
 * \return bool
 * \retval true Ustatus job is on
 * \retval false Ustatus job is off
 */
bool isUstatusJobOn(void)
{
    return( 0 != Observer_find_by_update(&Pipe->pjob_info->jobstart_observers, 
                                         pjl_ustatus_jobstart_update));
}
/**
 * \brief Return the current state of the ustatus page
 * \return bool
 * \retval true Ustatus page is on
 * \retval false Ustatus page is off
 */
bool isUstatusPageOn(void)
{
    return( 0 != Observer_find_by_update(&Pipe->pjob_info->pageend_observers, 
                                         pjl_ustatus_pageend_update));
}

/**
 * \brief Return the current state of the ustatus device
 * \return bool
 * \retval true Ustatus device is on
 * \retval false Ustatus device is off
 */
bool isUstatusDeviceOn(void)
{
    if(Pipe->pdevicestatus &&
                            pec_is_observing_printstatus(Pipe->pdevicestatus))
    {
        return true;
    }
    else
        return false;
}
/**
 * string is passed in and must contain at least 6 characters for the status to fit.  This returns
 * an ascii string of the current status word.
 */
void getCurStatus(char *string)
{
//    uint32_t status = pec_get_current_printstatus();    // 2014.01.13	req. from Lalit		by Juny
    uint32_t status = GetPrintSystemStatus();    
    DBG_PRINTF_ERR( "%s status %d\n", __FUNCTION__, status );		// 2013.12.18		by Juny
    status = map_print_status_to_pjl(status);
    minSprintf(string, "%d",status);

}
/**
 * \brief setup the ustatus for page.
 * \param[in] Value -- true turn it on by connection an observer, false, turn it off.
 * 
 */
void setUstatusPage(bool Value)
{
    Observer_t *o = 0;
    o = Observer_find_by_update(&Pipe->pjob_info->pageend_observers, 
                                pjl_ustatus_pageend_update);
    if(Value)
    {
        if (!o)
        {
            o = PipeObserver_Constructor(Pipe, 
                                         pjl_ustatus_pageend_ID,
                                         pjl_ustatus_pageend_update);
            ATTACH_OBSERVER(Pipe->pjob_info->pageend_observers, o);
        }
    }
    else
    {
        if (o) 
        {
            DETACH_OBSERVER(Pipe->pjob_info->pageend_observers, o);
            memFree(o);
        }
    }
}
/**
 * \brief Setup the ustatus device observers
 * \param[in] value true turn on ustatus for device, false turn it off.
 */
void setUstatusDevice(bool value)
{
/* hopes to observer at init */
/*
	USBStatus_is_ON = value;	// USB Status monitor	2013.12.19	by Juny
   DBG_PRINTF_INFO( "USB status is : %d \n", value);
*/

/* hopes to observer at init 
   remove her for observer_constructor */

    if( value )                    // ON or VERBOSE
    {
        if ( 0 == Pipe->pdevicestatus ) 
        {
            /// each pipe can have different settings 
            /// assumes pipes do not get unregistered and get registered at system startup.
            Pipe->pdevicestatus = 
                PipeObserver_Constructor( Pipe, PipeObserver_ID, pjl_ustatus_device_status_update);

	    /// attaching observer to a singleton so he knows who the subject is.
        DBG_PRINTF_INFO( " set Ustatus : %d %d \n", value, Pipe->pdevicestatus);	// @@ 2013.08.22 by Juny
	    pec_attachobserver_printstatus( Pipe->pdevicestatus );
        }
    }
    else       
    {
        if ( Pipe->pdevicestatus ) 
	{
            pec_detachobserver_printstatus( Pipe->pdevicestatus );
	    memFree(Pipe->pdevicestatus);
	    Pipe->pdevicestatus = 0;
	}
    }

}

extern stAuth_AuthInfo AuthInfo;	// from SysCardReaderMsg.h
int AuthRequestSend(char *auth_id, char *auth_pw, int auth_mode)	// auth_mode: 0 = ID & PW, 1 = ID only
{
	SYS_UI_MESSAGE sendMsg;
	error_type_t err;

	sendMsg.msgSender = PJLPARSER;
	sendMsg.cmd = CMD_INT_PARSER_AUTH_REQ;
	memset(sendMsg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

	DBG_PRINTF_ERR( "%s: send auth_id=%s ,auth_pw=%s auth_mod=%d \n", __FUNCTION__, auth_id, auth_pw, auth_mode );
	
	sendMsg.data[0] = (UINT8)auth_mode;	// auth_mode: 0 = ID & PW, 1 = ID only
	strcpy(AuthInfo.printAuthID, auth_id);
	strcpy(AuthInfo.printAuthPW, auth_pw);

	auth_result = -1;
	err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &sendMsg);
	XASSERT( err==OK, err );
	
	// wait for result from SSM
	while(1)
	{
		if ( auth_result >= 0 )
		{
			DBG_PRINTF_ERR("%s: AuthResult was received (%d)\n", __FUNCTION__, auth_result);
			break;
		}
		else
		{
			DBG_PRINTF_ERR("%s: Waiting for AuthResult...\n", __FUNCTION__);
			sleep(1);
		}
	}
	return auth_result;
}
