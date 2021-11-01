/** 
 *
 * ============================================================================
 * Copyright (c) 2006-2012   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *****************************************************************
 *
 * @file            This file holds the main system I/O parser.  I/Os and 
 *                  downstream parsers register here for service.  All the 
 *                  I/Os of the system register and all the parsers register. 
                    When the I/O requires service it calls a routine that 
 *                  signals the manager that data is available.  When the 
 *                  connection manager is ready it will read from the channel
 *                  and start parsing the data.  When parsing the data it looks
                    for the compare strings with which the parsers registered. 
 *                  When the connection manager finds the string it calls the 
 *                  parser and passes the parser the pipe for the parser to 
 *                  use.  When the parser returns the connection manager looks
                    to see if another parser is required from running this 
 *                  parser.  If so that parser is called, if not it continues
 *                  to parse this I/O channel till the data is gone.  Then the 
 *                  connection manager looks for the next I/O channel that 
                    needs service.
                    This routine supplies read, write, and rewind routines for 
 *                  use by the downstream applications.  This allows us to grab
 *                  the data stream, if required, and do other processing.  The 
 *                  read supplies a given number of bytes to the calling 
 *                  application There is a buffer from which the data is sourced.
 *                  When rewind is called the data being rewound is placed into
 *                  the buffer to allow the next read to
                    get this data.
 *
                    
                    Parsers no longer keep a list of pipes.  A pipe is run till an
                    end of job condition occurs only then will a different pipe be
                    allowed to run.  This way a diffent pipe can't come in between
                    a pjl wrapped pcl job that needs to be sent to pjl then pcl then
                    pjl parsers sequentially.  

 */

/* include files */
#include <string.h>              // declares memmove()
#include <stdbool.h>
#include <stdint.h>
#include "agRouter.h"
#include "memAPI.h"
#include "debug.h"
#include "lassert.h"
#include "agMessage.h"
#include "agConnectMgr.h"
#include "sys_init_api.h"
#include "mlist.h"
#include "voterApi.h"
#include "db_api.h"
#ifdef HAVE_PRINT_SUPPORT
#include "PrintJobInfo.h"
#endif
#ifdef HAVE_PLATFORM
#include "platform_api.h"
#include "platform.h"
#endif
#include "logger.h"
#include "debug.h"
#include "error_types.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#include "posix_ostools.h"

/* local definitions */
#define DBG_PRFX "CM: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 14 ) 

#define PAR_STACK_SIZE POSIX_MIN_STACK_SIZE ///< The size of the task stack. 
#define PARSE_BUFFER_SIZE 1024  ///< The initial size of the rewind buffer.
#define CANCEL_BUFFER 10000 //ksjung

/* internal functions */
static void *SysParser(void *unused);
void agPipeInit(IOReg *Pipe);
int32_t agPipeRead(IOReg *Pipe, void *Buffer, uint32_t Length, PARSE_READ_TYPE ReadType);    
int32_t agPipeWrite(IOReg *Pipe, void *Buffer, uint32_t Length);    
int32_t agPipeIoctl(IOReg *pipe, ioctl_cmd_t Cmd, void *Details);
int32_t agPipeCancel(IOReg *Pipe); //ksjung
uint32_t agPipeRewind(IOReg *pPipe, void *DataBuffer, uint32_t Length );
static void conmgr_cmd_register( void );

/* private data */


/// Private connection manager view of a parser, clients use opaque PARSEReg ptr.
///
struct PARSEReg_s
{
   ATLISTENTRY registeredList;  ///< all registered parsers
   ATLISTENTRY uniqueList;      ///< unique by name parsers
   MODULE_ID ParserModuleID;    ///< parser id for messaging.
   char *ParseString;           ///< string send in PJL ENTER LANGUAGE="HERE"
   char *CompareString;         ///< what will cause parser to be invoked.
   uint32_t  CmpStringSize;     ///< if null, it gets anything that is not taken by another parser.
   Channel_type_t PipeType;     ///< only accepts this type of input, print,scan fax etc.
}; 




/// All pipes are in the RegisteredPipes list, every pipe is either in the Active or the InActive list but not both.
static MLISTENTRY RegisteredPipes; /// All Pipes, active and inactive.
static MLISTENTRY ActivePipes; /// active pipes head of list is active!, remainder of list is waiting.
static MLISTENTRY InActivePipes; /// registered pipes with no outstanding data on the pipe.
static bool SysParserInitialized = false; /// protect against calls prior to link lists being initialized

//device timeout 5 minutes
#define DEVICE_IO_NOW_TIMEOUT (50)  ///< immediate timeout 
#define DEVICE_IO_TIMEOUT     (80 * 100) ///< Used for timeout in write.
#define DEVICE_IOERR_TIMEOUT  (60 * 100)    ///< give up on io channel timeout 
#define NUM_MESSAGES 10             ///< Number of messages in queue.
#define REWIND_SLOP 0x100           ///< The excess number of bytes to get if 
                                    /// we need a new rewind buffer.
static MLISTENTRY RegisteredParsers; ///< Registered Parsers.
static MLISTENTRY uniqueParsers;     ///< unique Parsers by ParseString

static pthread_t connect_mgr_thd_id;
//TX_THREAD  ParseThread;                ///< The thread structure.
ALIGN8 unsigned char ParseStack[PAR_STACK_SIZE]; ///< thread stack.
static char ParseBuffer[PARSE_BUFFER_SIZE];
static pthread_mutex_t my_mutex;

static mqd_t CMParseQ;
//TX_QUEUE CMParseQ;             ///< Message control block
//MESSAGE CMMess[NUM_MESSAGES];   ///< Message queue.

static int GarbageParserEnable = 0;

/* public functions */

void *PipeRegister(PIPE_INFO *Info)
{
    IOReg *Pipe;

    pthread_mutex_lock( &my_mutex );
    //
    // Get the memory for a new pipe structure.

    Pipe = (IOReg *) MEM_CALLOC(sizeof(IOReg), 1); // cast allocation.
    ASSERT(Pipe);
    //
    // Store the I/O routines used by this pipe.
    agPipeInit(Pipe);            // init the pipe structure

    Pipe->io.data_read = Info->io.data_read;
    Pipe->io.data_write = Info->io.data_write;
    Pipe->io.cmd_read = Info->io.cmd_read;    // unused
    Pipe->io.cmd_write = Info->io.cmd_write;  // usb interrupt write
    Pipe->io.ioctl = Info->io.ioctl;
     
    Pipe->PipeType = Info->channel_type;

    if (Info->channel_type == e_Print) 
    {
#ifdef HAVE_PRINT_SUPPORT
        Pipe->pjob_info = PrintJobInfo_construct(Pipe);
        Pipe->pjob_info->jobDocType = Info->DocType;		//LALIT: set document type of current job.
#endif
    }
    else if (Info->channel_type == e_Scan) //Used for Host Scan. All channel Have to be set 'jobDocType' 
    {
#ifdef HAVE_SCAN_SUPPORT
        Pipe->pjob_info = PrintJobInfo_construct(Pipe);
        Pipe->pjob_info->jobDocType = e_NONE;
#endif		
	} 
    else  //Default.
    {
        Pipe->pjob_info = 0;
    }

    Pipe->pdevicestatus = 0; // pjl might initialize this for device status
    Pipe->tdevicestatus = 0; // pjl might initialize this for timed device status

    Pipe->nextParserToRun = 0; 

    Pipe->PipeStatus = e_NoError;
    Pipe->PipeState = e_Idle;
    Pipe->CancelStatus = e_Accept;

    Pipe->io.data_read_handle = Info->io.data_read_handle;
    Pipe->io.data_write_handle = Info->io.data_write_handle;
    Pipe->io.cmd_read_handle = Info->io.cmd_read_handle;
    Pipe->io.cmd_write_handle = Info->io.cmd_write_handle;
    Pipe->idCookie = PIPE_ID_COOKIE;

    MInsertTailList(&RegisteredPipes, &Pipe->registeredList);
    MInsertTailList(&InActivePipes, &Pipe->waitingList);

    pthread_mutex_unlock( &my_mutex );
    return Pipe;   
}

bool pipe_remove(IOReg *Pipe)
{
    pthread_mutex_lock( &my_mutex );
    // if the pipe is active, it can not be removed.  So first see if on the inactive list
    if(MRemoveEntryList(&InActivePipes, &Pipe->waitingList))
    {
        // found the pipe on the inactive list, now get rid of it on the registered list
        if(MRemoveEntryList(&RegisteredPipes, &Pipe->registeredList))
        {
            // was on registered, if not assert, not remove the job info stuff if it exists.
            if(Pipe->pjob_info != NULL)
            {
 
#ifdef HAVE_PRINT_SUPPORT
               PrintJobInfo_destructor(Pipe->pjob_info);
#endif
            }
            // free the pipe memory
            MEM_FREE_AND_NULL(Pipe);
            pthread_mutex_unlock( &my_mutex );
            return true;
        } else
        {
            ASSERT(0);
        }
    }
    pthread_mutex_unlock( &my_mutex );
    return false;       // did not get rid of the pipe.
}
 
void PipeConnect(void *handle)
{
    MESSAGE nextMsg;

    DBG_PRINTF_INFO("--PipeConnect 0x%x \n", handle);

    ASSERT( ((IOReg*)handle)->idCookie == PIPE_ID_COOKIE ); // test that the cast was to a valid object.

    nextMsg.msgType =  MSG_START_PARSE;
    nextMsg.param3 = handle;

    SYMsgSend(CMPARSER, &nextMsg);      
}
 
void ConnectionMgr_ParserIsNowDone(
    IOReg *Pipe
    )
{
    MESSAGE msg;

    msg.msgType = MSG_END_PARSE;
    msg.param1 = 0; // ParserReturnCode;
    msg.param2 = 0; // unused
    msg.param3 = Pipe;          // The PSPipe that we are parsing.
    SYMsgSend(CMPARSER, &msg);      // send the response.
} 


uint32_t ParserRegister(PARSE_SETUP *ParseEntry)
{
    PARSEReg *Parser;

    Parser = (PARSEReg *)MEM_CALLOC(sizeof(PARSEReg), 1);
    ASSERT(Parser);

    Parser->ParserModuleID = ParseEntry->ParserID;
    Parser->CmpStringSize = ParseEntry->StringSize;
    Parser->ParseString = ParseEntry->ParseString;
    Parser->CompareString = ParseEntry->ParseCmpString;
    Parser->PipeType = ParseEntry->ParseType;

    ATInitNode(&Parser->uniqueList);

    if (Parser->ParseString)
    {
        // maintain a list of unique parsers
        bool found = false;
        ATLISTENTRY *next = MListHead(&uniqueParsers); 

        for (; next; next = MListNext(&uniqueParsers, next))
        {
            PARSEReg *existingParser = CONTAINING_RECORD(next, PARSEReg, uniqueList);
            ASSERT(existingParser && existingParser->ParseString);
            if (0 == strncmp(existingParser->ParseString, Parser->ParseString, strlen(Parser->ParseString)))
            {
                // found same string not unique 
                found = true;
                break;
            }
        }
        if (false == found) 
        {
            MInsertTailList(&uniqueParsers, &Parser->uniqueList);
        }
    }

    MInsertTailList(&RegisteredParsers, &Parser->registeredList);  // all parsers are registered.
    return(0);
}
/** Initialize the parser.
*   This should be called before init of parsers or I/O
*/
void SysParserInit(void )
{
    MInitList(&RegisteredParsers);  // all parsers 
    MInitList(&uniqueParsers);      // unique parsers with ParseString's 
    MInitList(&RegisteredPipes);    // all pipes are in this list
    MInitList(&ActivePipes);
    MInitList(&InActivePipes);

    SysParserInitialized = true;    // Yikes other threads can use the lists until they are initialized!
    
    posix_create_thread(&connect_mgr_thd_id, SysParser, 0, "conn_mgr", ParseStack, PAR_STACK_SIZE, POSIX_THR_PRI_NORMAL);


    conmgr_cmd_register();
}


/* private functions */

/// strcspn that also matches NULL characters. not a normal cstring ...
/// This gem is to satify customer test files with NULLS between UEL and the next language, YUCK.
/// Note: the NULL is implied in the cset. 
static int strcspn_includingnulls(const char *str, const char *cset, int size)
{
    uint32_t cnt = 0;
    uint32_t i;

 
    while (size--) {
        bool match = false;
        for (i=0; i < strlen(cset); i++) {
            if ( str[cnt] == cset[i] || str[cnt] == 0) {
                match = true;
                break;
            }
        }
        if (match == false)
            break;
        else
            ++cnt;
    }
    return cnt;
}

/// FindGarbageParser
/// 
/// Should just return PCL but this actually votes between ps and pcl
/// depending on the data found.
/// We only do a vote for postscript since virtually anything else is pcl.
/// Voting in general is a backward compatible customer requirement.
///
/// disable voting if pcl isn't compiled in.
#if !( defined(PCL5E) || defined(PCL5C) || defined( HAVE_USB_HOST_PARSER ) || defined(HAVE_GGS_RIP_SUPPORT) )
#define FindGarbageParser(p,l) 0
#else
#define FindGarbageParser(p,l) FindGarbageParser_impl(p, l)
static PARSEReg *FindGarbageParser_impl(const char *startPtr, int len)
{
    static PARSEReg *Parser = 0; // NB: this implies no dynamic addition of parsers.
    static PARSEReg *PSparser = 0;
    static int32_t lastPSparseStartSum = -1; // ps error determined by not consuming data.
    int is_ps;
    const char * ps_id_str = "%!";

    // one time initialization of statics.
    if (Parser == 0)
    {
        PARSEReg *tempParser = 0;
        ATLISTENTRY *next = NULL;
        // find the pcl and ps parsers and hang on to them in subsequent calls
        // NOTE that only 1 parser should have a 0 length CmpStringSize !!
        // NOTE that parser is normally PCL
        for (next = MListHead(&RegisteredParsers); 
             next; 
             next = MListNext(&RegisteredParsers, next))
        {
            tempParser = CONTAINING_RECORD(next, PARSEReg, registeredList);
            // find the pcl parser
            if (tempParser->CmpStringSize == 0)
            {
                Parser = tempParser;
                break;
            }
        }
        
        // iff postscript is in the system and we haven't already found the ps parser.
        if ( -1 != PS_VOTER(ps_id_str, ps_id_str+strlen(ps_id_str)) && PSparser == 0)
        {
            for (next = MListHead(&RegisteredParsers); 
                 next; 
                 next = MListNext(&RegisteredParsers, next))
            {
                tempParser = CONTAINING_RECORD(next, PARSEReg, registeredList);
                // find the postscript parser, using the enter postscript command %! 
                if (tempParser->CompareString && 0 == strncmp(ps_id_str, tempParser->CompareString, strlen(ps_id_str))) 
                {
                    PSparser = tempParser;
                    break;
                }
            }
            ASSERT( PSparser );
        }
    }

    // lets vote to see if the garbage is postscript, -1 return on no postscript in system.
    is_ps = PS_VOTER(startPtr, startPtr + len);  // postitive vote is Postscript 

#ifdef HAVE_ZJS_SUPPORT
    // Could be called from the parsers, but only garbage selected languages may need this.
    //MaybeResetPJLVars(); // defaults may have changed outside of PJL.
#endif

    // NB: needs a conditional for when postscript isn't in the system.
    if (PSparser && is_ps > 0 ) { 
        int32_t sum;
        while (len > 0) { 
            sum = *startPtr++;   // wouldn't be nicer if parser returned an error instead of this sum the data hack.
            --len;
        }
        if ( lastPSparseStartSum == sum ) {
            // since no data was consumed by PS the last time 
            // we assume that PS will never consume this data and that the voter was wrong and send it to pcl.
            is_ps = -1;  
        }
        else {
            lastPSparseStartSum = sum;
            DBG_PRINTF_INFO("--garbage parser language. %x %s\n", PSparser, PSparser->ParseString);
            return PSparser; 
        }
    } 

    ASSERT (Parser && is_ps <= 0 ); // Not postscript must be pcl.

    lastPSparseStartSum = -1;
    DBG_PRINTF_INFO("--garbage parser language. %x %s\n", Parser, Parser->ParseString);
    return Parser; 
}
#endif
/** 
 ** CallParser
 *
 * helper routine for SysParser. 
 */
static void CallParser(PARSEReg *Parser, IOReg *Pipe)
{
    bool runNow = true;

    ASSERT(Parser);
    ASSERT(Pipe);

    if (runNow) 
    {
        Pipe->Parser = Parser;  // set the parser
        Pipe->PipeState = e_InParser;

        MESSAGE nextMsg;

        nextMsg.msgType =  MSG_START_PARSE;
	nextMsg.param1 = (uint32_t)Parser->CompareString; // pass compare info to parser
	nextMsg.param2 = (uint32_t)Parser->ParseString;   
        nextMsg.param3 = Pipe;
        
        SYMsgSend(Parser->ParserModuleID, &nextMsg);      
    }
    else
    {
        ATLISTENTRY *headNode = MRemoveHeadList(&ActivePipes);
        IOReg *headPipe = CONTAINING_RECORD(headNode, IOReg, waitingList);

        ASSERT(headPipe == Pipe); 

        // no need to reinspect the data we already know which one to run.
        headPipe->nextParserToRun = Parser;

        // move from front to back, and wait for connect or end parse to check permission again.
        MInsertTailList(&ActivePipes, headNode);  
    }
}


/// handle the end parse message from the parser to the connection manager.
/// Pipe->lang is used by PJL's enter language command for explicit language/parser switching.
/// parsers are expected to exit with PARSER_SUCCESS unless they want the system to reboot!
/// they should eat all the data up until they find a UEL or other language specific terminator.
///
static void CM_do_msg_end_parse(void *PipeHandle, int parserReturn)
{
    DBG_PRINTF_INFO("--msg_end_parse \n");
    IOReg *Pipe = (IOReg*)PipeHandle; // cast 

    ASSERT(Pipe->idCookie == PIPE_ID_COOKIE); // test that the cast was to a valid object.
    //Removed to fix crash with WIA driver. Uncomment whenever release build enabled
    //XASSERT(Pipe->PipeState == e_InParser, (uint32_t)(uintptr_t)Pipe);  // who called me and why??? //cast ptr to int
    
    //ksjung For Cancle
    if (Pipe->CancelStatus == e_Cancel){
        int BytesRead = 0;
        char *cancelBuffer;
        int err = 0;
        cancelBuffer = MEM_MALLOC(CANCEL_BUFFER);
        DBG_PRINTF_INFO("--ksjung-- e_Cancel Accepted.\n");
        if (Pipe->Length > 0) {
            DBG_PRINTF_INFO("--ksjung-- Pipe have remained data.\n");
            BytesRead = Pipe->ReadConnect(Pipe, cancelBuffer, CANCEL_BUFFER, PARSEREAD_SOME);
            while(err == 0){
                if(BytesRead <= 0){
                    err = 1;
                }
                else {
                BytesRead = Pipe->ReadConnect(Pipe, cancelBuffer, CANCEL_BUFFER, PARSEREAD_SOME);
                }
            }
        }
		if (cancelBuffer != NULL)
            MEM_FREE_AND_NULL(cancelBuffer);
        Pipe->CancelStatus = e_Accept;
    }
    
    Pipe->PipeState = e_Idle;   // pipe is now idle    
    
    if (parserReturn != PARSER_SUCCESS) 
    {
        // NB: host based parsers don't cleanup and do return error should we cleanup for them?
        // Is it the parsers responsibility to cleanup the data stream, ie find the UEL.
        // The parser bailed out with an error
        DBG_PRINTF_ERR("CONMGR: parser error %x\n", parserReturn);
        Pipe->nextParserToRun = 0;      // Done here, go on.
        PipeConnect(Pipe);  // kick pipe to start things up. 
        return;
    }
    
    if (Pipe->nextParserToRun != 0)
    {
        // If this parser requested a swap to a different
        // parser handle that here.
        PARSEReg *Parser = Pipe->nextParserToRun;  // the parser to run next.
        Pipe->nextParserToRun = 0; // make it a one shot, really only valid right after a pjl enter language

        DBG_PRINTF_INFO("--pipekick pjl has set the next parser language. %x \n", Pipe);
        CallParser(Parser, Pipe);
    }
    else
    {
        // 
        // The sysparser needs to be kicked, do that here.
        // The parser is finished now we need to check
        // this pipe to see if anything is waiting.
        //
        
        if (Pipe->Length == 0 && Pipe->PipeStatus <= 0) // no more data on input pipe.
        {
            ATLISTENTRY *next = 0;
           
            if (Pipe->PipeType == e_Print)
            {
		        pji_local_copy_init( &Pipe->pjob_info->local, &system_pji->local );
		        MInsertTailList(&InActivePipes, MRemoveHeadList(&ActivePipes));
                next = MRemoveHeadList(&ActivePipes);
                if (next) 
                {
                    IOReg *savePipe = CONTAINING_RECORD(next, IOReg, waitingList);
                    MInsertHeadList(&ActivePipes, &savePipe->waitingList);
                    DBG_PRINTF_INFO("---Start saved pipe %x \n", savePipe);
                    PipeConnect(savePipe);
                }
            }
            else
            {
	        DBG_PRINTF_INFO("---All pipes are empty wait for more io. \n");
            }

            DBG_PRINTF_INFO("---Empty pipe send accept %x\n", Pipe);

            if (Pipe->io.ioctl != NULL)
            {
                // tell network to accept; on new data do a pipe connect.
                Pipe->io.ioctl(Pipe->io.data_read_handle, e_close, NULL);
            }
            Pipe->PipeStatus = e_NoError; 
        }
        else
        {
       	    DBG_PRINTF_INFO(
			    "---pipekick on endparse with more data in pipe %x len %d stat %d\n", 
			    Pipe, Pipe->Length, Pipe->PipeStatus);
            PipeConnect(Pipe);
        }
    }
}

static void SendStartParse(IOReg *Pipe)
{
    PARSEReg *Parser=NULL;
    PARSE_READ_TYPE timeout;  
    int32_t Size; 
    uint32_t nonWhiteStart=0;
    uint32_t dataLen;
    bool partialMatch;
    uint32_t nonWhiteLen=0;
    ATLISTENTRY* next = NULL;
    int32_t status;
    //
    // See if this is a pipe that does not require a verification of a parse string
    // such as fax and scan.  If this is that type of pipe, go 
    // directly to that parser.  
    //
    if (Pipe->PipeType != e_Print)
    {
        for (next = MListHead(&RegisteredParsers); 
             next; 
             next = MListNext(&RegisteredParsers, next))
        {
            Parser = CONTAINING_RECORD(next, PARSEReg, registeredList);
            //
            // Check to see if this parser should even be looking at this pipe.
            //
            if (Parser->PipeType != Pipe->PipeType) {
                continue;
            }
            //
            // If the parser matches this type and there is not a compare string
            // go to the parser directly.
            //
            if ((Pipe->PipeState == e_Idle) && (Parser->CompareString == NULL))
            {
                // Check to see if this is really ready to send.
                Size = Pipe->ReadConnect(Pipe,&ParseBuffer[0], PARSE_BUFFER_SIZE, PARSEREAD_ANY);
                if (Size <= 0) {    // we did not read anything, get out of here.
                    return;
                }
                Pipe->RewindData(Pipe, &ParseBuffer[0], Size);   // put the data back
                CallParser(Parser, Pipe);
                break;
            }
        }
    }


    DBG_PRINTF_INFO("---connect: %x len %d stat %d \n", Pipe, Pipe->Length, Pipe->PipeStatus);

    // now check this pipes data to see which parser to call.            
    timeout = PARSEREAD_ANY;
    dataLen = 0;
    while (Pipe->PipeState == e_Idle)   // keep pulling data from this pipe till it is empty.
    {
        int readLength = MIN(100, PARSE_BUFFER_SIZE-dataLen);
        partialMatch = false;

        Size = Pipe->ReadConnect(Pipe, &ParseBuffer[dataLen], readLength, timeout); // get everything it has. 
        if(Size > 0) // read ok?
        {
            dataLen += Size; 
            // Now scan through the data looking for matching strings.
            // skip whitespace iff a parser signature is found
            // else garbage parser gets this white space

            nonWhiteStart = strcspn_includingnulls(&ParseBuffer[0], " \t\r\n", dataLen);
            nonWhiteLen = dataLen - nonWhiteStart; 
            if(nonWhiteLen <=0)
            {
                int32_t i;
                timeout = PARSEREAD_SOME;

                // optimize reading NULLs as fast as possible
                // host parsers use large block of nulls transfer to cancel 
                // and if we are not in the parser this can be tossed.
                for(i=0; i < dataLen; i++)
                    if( ParseBuffer[i] != 0 )
                        break;

                if(i == dataLen)
                {
                    do 
                    {
                        status = Pipe->ReadConnect(Pipe, &ParseBuffer[0], PARSE_BUFFER_SIZE, timeout);
                        if(status > 0)
                        {
                            dataLen = status;
                        } else
                        {
                            dataLen = 0;
                            Size = 0;
                            goto error_timeout; // need a double break, skip all that crap and jump there.
                        }
                        for(i=0; i < dataLen; i++)
                            if( ParseBuffer[i] != 0 )
                                break;
                    }
                    while(i == dataLen) ;  // keep eating nulls.

                    nonWhiteStart = i;
                    nonWhiteLen = dataLen - nonWhiteStart; 

                }
                continue; // all white space wait for more data.
            }
            for(next = MListHead(&RegisteredParsers); 
               next; 
               next = MListNext(&RegisteredParsers, next))
            {
                Parser = CONTAINING_RECORD(next, PARSEReg, registeredList);
                if((Parser->CmpStringSize != 0) && 
                   (Parser->CompareString != NULL) &&
                   (Parser->PipeType == Pipe->PipeType))
                {
                    if(0 == strncmp(&ParseBuffer[nonWhiteStart], 
                                    Parser->CompareString, 
                                    MIN(nonWhiteLen, Parser->CmpStringSize)))
                    {
                        if(nonWhiteLen < Parser->CmpStringSize)
                        {
                            partialMatch = true;      
                        }
                        else
                        {
                            // we have a match.  Push remaining data into pipe and the data we just matched
                            DBG_PRINTF_INFO("SysParser() found CompareString(%s) for Parser %x Pipe %x\n", 
                                            Parser->CompareString, Parser, Pipe );
                            // rewind to remove whitespace
                            agPipeRewind(Pipe, &ParseBuffer[nonWhiteStart], dataLen - nonWhiteStart);
                            CallParser(Parser, Pipe);
                            break; // Pipe->PipeState is e_InParser not e_Idle will exit while loop
                        }
                    }
                }
            }   
        }
        else 
        { // error timeout 
	error_timeout: 
            DBG_PRINTF_ERR("----------Pipe %x dataLen %d Size %d Pipe->PipeStatus %d timeout %d\n", 
                             Pipe, dataLen, Size, Pipe->PipeStatus, timeout);
            if (dataLen == 0 &&             
                (Size < 0 || Pipe->PipeStatus < 0 || (PARSEREAD_ANY != timeout && Size == 0) ) )
            {
                if (Pipe->io.ioctl != NULL) 
                {
                    Pipe->io.ioctl(Pipe->io.data_read_handle, e_close, NULL);       // tell the guy we are done.
                }
                Pipe->PipeStatus = e_NoError;

                if (Pipe->PipeType == e_Print)
                {
                    MInsertTailList(&InActivePipes, MRemoveHeadList(&ActivePipes));
                    next = MRemoveHeadList(&ActivePipes);
                    if (next) 
                    {
                        IOReg *savePipe = CONTAINING_RECORD(next, IOReg, waitingList);
                        MInsertHeadList(&ActivePipes, &savePipe->waitingList);
                        DBG_PRINTF_INFO("-------pipe connect on %d \n", savePipe);
                        PipeConnect(savePipe);
                    }
                }
                break; // exit while and function.
            }
        }

        if (dataLen && Pipe->PipeState == e_Idle && partialMatch == false) // NO MATCH FOUND
        {
            if (GarbageParserEnable == 1 && Pipe->PipeType == e_Print && (Parser = FindGarbageParser(&ParseBuffer[0], dataLen))) 
            {
	        DBG_PRINTF_INFO("SysParser() switching to garbage disposal parser %x \n%s\n", 
                                 Pipe, &ParseBuffer[nonWhiteStart]);
                agPipeRewind(Pipe, &ParseBuffer[0], dataLen);
                CallParser(Parser, Pipe);
            }
            else
            {
	        DBG_PRINTF_INFO("SysParser() no disposal parser %x \n%s\n", 
                                 Pipe, &ParseBuffer[0]);
                // if nonWhiteStart != 0 then we have a bunch of nulls, throw them all out
                // now rather than doing this a byte at a time.
                //
                if(nonWhiteStart != 0) {
                    agPipeRewind(Pipe, &ParseBuffer[nonWhiteStart], nonWhiteLen);
                } else {
		    agPipeRewind(Pipe, &ParseBuffer[1], dataLen - 1);
		}
                // Since there is no garbage disposal parser we drop the
                // unclaimed bytes on the floor.
                // Clarity over efficency we ignore/toss one byte at a time.
                if (dataLen > 10) 
                {
		    dataLen = 0;
                    timeout = PARSEREAD_ANY;
                    continue;
                }
                dataLen = 0;
            }
        }
        timeout = PARSEREAD_SOME;
    } // while e_Idle
}

/// Pipe->Connect() "calls" this.
/// 
static void CM_do_msg_start_parse( void *pipeHandle )
{
    IOReg *Pipe = (IOReg*)pipeHandle; // cast a void parameter

    ASSERT(Pipe->idCookie == PIPE_ID_COOKIE); // test that the cast was to a valid object.

    ATLISTENTRY *headNode = MRemoveHeadList(&ActivePipes);
    if (headNode)
    {
        IOReg *headPipe = CONTAINING_RECORD(headNode, IOReg, waitingList);
        MInsertHeadList(&ActivePipes, headNode);  // more data on the current pipe
        if (Pipe == headPipe || Pipe->PipeType != e_Print )
        {
	    DBG_PRINTF_INFO("--more on current pipe %x is e_Print %d\n", 
                       Pipe, Pipe->PipeType == e_Print);
            SendStartParse(Pipe);
        }
        else 
        {
	    DBG_PRINTF_INFO("--data on pipe that can't run yet queue it %x\n", Pipe);
            if (MRemoveEntryList(&InActivePipes, &Pipe->waitingList))
            {
                // have active, if pipe is inactive move it to active list
                MInsertTailList(&ActivePipes, &Pipe->waitingList);
            }
            // else pipe must be in active list but not at head of list.
        }
    }
    else
    {
        DBG_PRINTF_INFO("--data on inactive pipe %x is e_Print %d\n", 
                   Pipe, Pipe->PipeType == e_Print);
        if (Pipe->PipeType == e_Print) 
        {
        // connections empty must be on InActivePipes list and  make it the next pipe to run.
        REL_ASSERT(MRemoveEntryList(&InActivePipes, &Pipe->waitingList));
        MInsertHeadList(&ActivePipes, &Pipe->waitingList);
        }
        SendStartParse(Pipe); // start it
    }
}


/** Main thread loop for Connection manager.
 * new pipes or new data on existing pipes cause Connects, start parse 
 * a parser only returns end parse when it is willing to allow other parsers to run.
 * this means it blocks other parsers by not returning until done and 
 * allows parrallel parsing by returning end parse before its finished.
 * print parsers always block on systems that have one physical print engine.
 */
static void *SysParser(void *unused)
{
    int err;
    MESSAGE msg;

    posix_create_MESSAGE_queue( &CMParseQ, "/connectionmgrQueue", NUM_MESSAGES );
    posix_mutex_init( &my_mutex );
    router_register_queue(CMPARSER, CMParseQ);    // register with the router.

    SysWaitForInit();

	error_type_t db_retval = FAIL;
	int dbversion=0;
	DBG_PRINTF_INFO("Garbage parser check. %d\n", GarbageParserEnable);
    dbversion = db_get_var_int("INST_MACHINE", "DBversion", &db_retval);
    DBG_PRINTF_INFO("DB Version check. %d\n", dbversion);
    if(dbversion >= 209) {
        DBG_PRINTF_INFO("DB version check %d >= 209\n", dbversion);
        GarbageParserEnable = db_get_var_int("CONF_SYSTEMSP", "GarbageParserEnable", &db_retval);
    }
    if ( GarbageParserEnable == 1 )
        DBG_PRINTF_INFO("Garbage parser enable. %d\n", GarbageParserEnable);
    else
        DBG_PRINTF_INFO("Garbage parser disable. %d\n", GarbageParserEnable);

    while(1) 
    {
        if (sizeof(MESSAGE) != (err = mq_receive(CMParseQ, (char*)&msg, sizeof(MESSAGE), 0)))
        {        
           // else case on error shouldn't happen we just ignore and continue on.???
           XASSERT( err == sizeof(MESSAGE), thread_errno);
        }
        else
        { 
            pthread_mutex_lock( &my_mutex );
            switch (msg.msgType)
            {
            case MSG_START_PARSE: // pipe::Connect does this
                CM_do_msg_start_parse(msg.param3 /*pipe*/);
                break;
                
            case MSG_END_PARSE: // parser sends this
                CM_do_msg_end_parse(msg.param3 /*pipe*/, msg.param1/*parser result*/);
                break;
                
            default:
	        DBG_PRINTF_ERR("CONNMGR: Bad message %x\n",msg.msgType);
            }
            pthread_mutex_unlock( &my_mutex );
        }

    } // end while(1) thread loop.
    return 0;
}

/** Initialize the pipe for the i/o channel.  
 *
 *  \param Pipe The IO structure for this channel.
 */
void agPipeInit(IOReg *Pipe)
{
    Pipe->DataBuffer = MEM_MALLOC(PARSE_BUFFER_SIZE);
    Pipe->BufferPointer = Pipe->DataBuffer;
    Pipe->RdBufLen = PARSE_BUFFER_SIZE;
    Pipe->Length = 0;
    Pipe->pipe_read_rewind = &agPipeRewind;                  // address of rewind value
    Pipe->pipe_read = &agPipeRead;
    Pipe->pipe_write = &agPipeWrite;
    Pipe->pipe_ioctl = &agPipeIoctl;
    Pipe->pipe_cancel = &agPipeCancel; //ksjung
}
/** Rewind data back into the pipe.
 *  Rewind the data in Buffer into the channel.  The Length could be anything.
 *  This works by seeing if the rewinded data will fit in the buffer.  If it will
 *  then the data is moved so the new data will fit and the new data is placed in
 *  the buffer.  If the buffer is not large enough.  A buffer that is big enough is
 *  allocated and the old + new data is moved into the buffer.  The old buffer
 *  is then freed.
 *
 *  @return The amount of data rewinded or 0 if an error.  The only error is if there is not
 *           any room in the buffer.
 *
 *  \param Pipe The I/O Pipe into which to rewind the data.
 *  \param DataBuffer The buffer that contains the data to rewind.
 *  \param Length The amount of data to rewind.
 * 
 */
uint32_t agPipeRewind (IOReg *Pipe, void *DataBuffer, uint32_t Length)
{

    DBG_PRINTF_INFO("agPipeRewind() rewinding %u bytes & moving %u bytes\n", Length, Pipe->Length );
//  ksjung log for test
//    char TestBuffer[12];
    //memcpy((char *)Pipe->DataBuffer, TestBuffer, 10);
//    DBG_PRINTF_INFO("agPipeRewind() rewinding %u bytes & moving %u bytes\n", Length, Pipe->Length);
    //DBG_PRINTF_INFO("agPipeRewind() buffer: %s\n", TestBuffer);
    ASSERT(Pipe);

    if (Length == 0 || DataBuffer == NULL)
        return 0; // 
    //
    // See if we have enough room to rewind the data into the buffer
    //
    if ((Pipe->RdBufLen - Pipe->Length) >= Length)
    {
        //
        // We have enough room in the buffer.  Move the data in.
        //
        memmove ((char *)Pipe->DataBuffer + Length, (char *) Pipe->BufferPointer, Pipe->Length);
        memcpy ((char *)Pipe->DataBuffer, DataBuffer, Length);
    } 
    else
    {
        unsigned char *TempBuf;
        // We do not have enought buffer to hold everything.  Allocate more buffer now.
        //
        TempBuf = (unsigned char *) MEM_MALLOC(Pipe->Length + Length + REWIND_SLOP);
        ASSERT(TempBuf);        // in trouble if we can't get the memory.
        memcpy ((char *) TempBuf, DataBuffer, Length);
        if (Pipe->DataBuffer != NULL)
        {
            memcpy ((char *) TempBuf + Length, Pipe->BufferPointer, Pipe->Length);   // put the old data at the end.
            MEM_FREE_AND_NULL (Pipe->DataBuffer);            // return the old buffer.
        }
        Pipe->DataBuffer = TempBuf;          // the new start of the buffer
        Pipe->RdBufLen = Pipe->Length + Length + REWIND_SLOP;
    }
    Pipe->Length += Length;                 // amount of data in the buffer now.
    Pipe->BufferPointer = Pipe->DataBuffer;             // reset the pointer
    //ksjung for log XL error
    //memcpy((char *)Pipe->DataBuffer, TestBuffer, 10);
    //DBG_PRINTF_INFO("agPipeRewind() buffer: %s\n", TestBuffer);

    return(Length);                         // done.
}


  
/**
 * \brief Read from the I/O Pipe.
 *  DESCRIPTION: Read data from the I/O channel depending on the ReadType.
        ReadType selects the timeout to use when doing the read.  When doing
        a read the data that is stored in the buffer is copied into the buffer,
        and then a call is made to the I/O system to fill the rest of the
        buffer.  If the buffer is empty and is greater than 2*PARSE_BUFFER_LENGTH
        then the buffer is released and a new one allocated that is
        of length 2*PARSE_BUFFER_LENGTH.  Try to keep memory usage low.
 *
 *  @return 0 if no bytes available, else num bytes read
 *
 * \param Pipe The I/O Pipe to read from
 * \param Buffer The buffer in which to place the data.
 * \param Length The size of the buffer.
 * \param ReadType Timeout options for this read.
 * 
 */
int32_t agPipeRead(IOReg *Pipe, void *Buffer, uint32_t Length, PARSE_READ_TYPE ReadType)
{
    uint32_t bytesRead = 0;
    uint16_t timeout = 0;
    uint32_t timeoutTotal = 0;
    BOOL done = FALSE;
    int32_t result = 0;
    uint16_t IOTimeout = 0;

#ifdef HAVE_PLATFORM
    platvars_get_io_timeout(&IOTimeout); // DEFAULT_IO_TIMEOUT
    IOTimeout *= SYS_TICK_FREQ;  // convert from seconds to centiseconds the OS tick interval;
    DBG_ASSERT(100 == SYS_TICK_FREQ); // OS timer tick check
#else
    IOTimeout = 10 * 100;
#endif

    IOTimeout = IOTimeout < 1000 ? 1000 : IOTimeout;

    //IOTimeout = 30*100; //ksjung temp
    
	if (Pipe->pjob_info->jobDocType == e_NETIOPAGE) // ksjung Network
	{   
		//DBG_PRINTF_INFO("Network- IOTimeout changed\n");
		IOTimeout = 300*100;
	}

    switch (ReadType)
    {
        case PARSEREAD_EXACT:
            {
                //wait until get exact amount requested or give up on channel
                timeout = IOTimeout; // centiseconds
                break;
            }
        case PARSEREAD_SOME:
            //wait until get some data or give up on channel
            //we will iterate waiting on this timeout until we hit
            //the max timeout defined by IOTimeout
            timeout = DEVICE_IO_NOW_TIMEOUT; // centiseconds
            break;
        case PARSEREAD_DELAY:
            timeout = DEVICE_IO_NOW_TIMEOUT; // centiseconds
            break;

        case PARSEREAD_ANY:
            //get any data, don't wait
            timeout = 0;
            break;
    }


    //
    //  If we have something in the pipe, copy it out first.
    //  This was rewinded into the pipe.
    //
    if (Pipe->Length > 0)
    {
        const char UEL_CONST_STR[] = "\x1B%-12345X";
        uint32_t Temp;
        Temp = MIN(Pipe->Length, Length);
        memcpy(Buffer, Pipe->BufferPointer, Temp);      // move in the data
        bytesRead = Temp;
        Pipe->Length -= Temp;
        Pipe->BufferPointer = (char *)Pipe->BufferPointer + Temp;                // move the buffer pointer

        if (Pipe->Length == 0)
        {
            // The pipe is completely empty which is a good moment to
            // see if we need to free the rewind buffer.  This will only
            // happen if the rewind buffer is bigger than
            // 2*PARSE_BUFFER_SIZE.  After the free we will
            // reallocate the buffer to PARSE_BUFFER_SIZE

            if (Pipe->RdBufLen > 2 * PARSE_BUFFER_SIZE)
            {
                MEM_FREE_AND_NULL(Pipe->DataBuffer);
                Pipe->DataBuffer = MEM_MALLOC(PARSE_BUFFER_SIZE);
                ASSERT(Pipe->DataBuffer);
                Pipe->RdBufLen = PARSE_BUFFER_SIZE;
                Pipe->BufferPointer = Pipe->DataBuffer;
            }
        }

        if ((ReadType != PARSEREAD_EXACT) && (Temp >= sizeof(UEL_CONST_STR) - 1))
        {
            // if we have rewind data return it and potentially a little more.
            // try to avoid zero time reads as they thread thrash on slow input.
	    // so we wait a little bit even on an ANY zero timeout read.
            timeout = DEVICE_IO_NOW_TIMEOUT;
        }
    }
    //
    // PipeStatus is set to error when we got an error from the io. Once we get that we
    // need to return -1 to the caller after all the data in the rewind buffer has been
    // consumed.
    if (Pipe->PipeStatus != e_NoError)
    {
        //we had an error, if no bytes read from buffer set bytesRead to the error code
        //else return the number of bytes read from the buffer
        if (bytesRead == 0)
        {
            bytesRead = Pipe->PipeStatus;
            Pipe->PipeStatus = e_NoError;        // clear the error
        }
	DBG_PRINTF_INFO("error io.data_read result = %d status %d bytesread %d length %d timeout %d\n", 
			result, Pipe->PipeStatus, bytesRead, Length, timeout );
        return bytesRead;
    }
    DBG_PRINTF_DEBUG("no error io.data_read result = %d status %d bytesread %d length %d timeout %d\n", 
		    result, Pipe->PipeStatus, bytesRead, Length, timeout );
    //
    // Get the rest of the data from the interface if we need more
    //
    if (bytesRead < Length)
    {
        // Keep reading from interface until have all the bytes or the interface indicates
        // there is no more to get
        while (done == FALSE)
        {
            result = Pipe->io.data_read(Pipe->io.data_read_handle, ((char *)Buffer) + bytesRead, Length - bytesRead, timeout);
//            DBG_PRINTF_INFO("io.data_read result = %d\n", result);
            DBG_PRINTF_DEBUG("io.data_read result = %d status %d bytesread %d length %d timeout %d\n", result, Pipe->PipeStatus, bytesRead, Length, timeout );
            // BYREQUEST connections return negative values when they know there is no more
            // data
            if (ReadType == PARSEREAD_SOME && result == 0 && bytesRead == 0) 
               result = CONIO_TIMEOUT;
            
            if (result < 0)
            {
                //we have read data then bytesRead will be the number of bytes
                //else it will be 0
                done = TRUE;
                if (result == CONIO_TIMEOUT && ReadType == PARSEREAD_SOME)
                {
                    timeoutTotal += timeout;
                    if (timeoutTotal < IOTimeout)
                    {
                        ASSERT(timeout > 0);
                        done = FALSE;  // read some more till IOTimeout
                    }
                    else if (timeoutTotal >= IOTimeout)
                    {
                        DBG_PRINTF_DEBUG_M("CONMGR: io timeout\n");
                    }
                }
            }
            else
            {
                bytesRead += result;

                switch (ReadType)
                {
                    case PARSEREAD_EXACT:
                        if (bytesRead == Length)
                        {
                            done = TRUE;
                        } // else !done read more till length is satisfied
                        break;
                    case PARSEREAD_SOME:
                        //wait until get some data or give up on channel
                    case PARSEREAD_ANY:
                    case PARSEREAD_DELAY:
                        //get any data, don't wait
                        done = TRUE;
                        break;

                }

            }
        }
    }
    //Return either the number of bytes read or error
    if (result < 0)
    {
        //if the IO returned an error we need to make sure and return error
        //once on the read. If we read bytes we want to give those back
        //and set the error so the next time read is called it will
        //return the error. After that the error is cleared and this read will
        //try to read the IO again and will report whatever it returns.
        if (bytesRead > 0)
        //&&
        //!(result == CONIO_TIMEOUT && timeout == 0 ) &&
        //!(result == CONIO_CONDONE ))
        {
            DBG_PRINTF_INFO("--Pipe temp result %d timeout %d", result, timeout);
            Pipe->PipeStatus = e_Error; // result;
            result = bytesRead;
        }

        //else return result
        DBG_PRINTF_INFO("--Pipe agRead %x cnt %d \n", Pipe, result);
    }
    else
    {
        //no error, return #bytes read
        result = bytesRead;
    }
    return result;
}
    
/**
 * \brief Write data to the I/O Channel.
 *  DESCRIPTION: Write the data in the buffer to the channel.  Several
            things are expected.  This is a non-blocking write.  It must
            not wait for the write to finish.  Second the Buffer is
            allocated using agMalloc.  When the write is finished then
            the buffer will be freed.  Last the Buffer is 16 byte aligned.
            This is awful, however we then do not have to copy the buffer 
            which is what we want.
 *
 *  RETURNS:
 *
 * \param Pipe The I/O Pipe to write the data.
 * \param Buffer The Buffer containing the data to write.
 * \param Length The amount of data to write.
 * 
 */

int32_t agPipeWrite(IOReg *Pipe, void *Buffer, uint32_t Length)    
{
    if(Pipe->io.data_write) 
    {
        //DBG_PRINTF_INFO( " len %d %s\n", Length, (char*) Buffer );
        DBG_PRINTF_DEBUG( " len %d %s\n", Length, (char*) Buffer );
        return Pipe->io.data_write(Pipe->io.data_write_handle, Buffer, Length, DEVICE_IO_TIMEOUT);
    }
    MEM_FREE_AND_NULL(Buffer);
    return Length;
}

int32_t agPipeIoctl(IOReg *Pipe, ioctl_cmd_t Cmd, void *Details)
{
    if(Pipe->io.ioctl) 
    {
	    return Pipe->io.ioctl(Pipe->io.data_read_handle, Cmd, Details);
    }
    return 0;
}

// ksjung For Cancle
int32_t agPipeCancel(IOReg *Pipe)    
{   
    Pipe->CancelStatus = e_Cancel;
    return 0;
}


// If a parser exists that matches CmpString set the pipe to run that language next
// Used by PJL SET LANGUAGE=PCL
// returns true if a matching parser was found, false if no matching parser was found.
//
uint32_t SetNextParserToRunOnThisPipe(IOReg *Pipe, char *CmpString)
{
    PARSEReg    *parser = NULL;
    ATLISTENTRY *next = NULL;

    if (SysParserInitialized == false) 
        return 0;
    // Scan through the parsers looking for the one that is requested.
    //
    for (next = MListHead(&RegisteredParsers); 
         next; 
         next = MListNext(&RegisteredParsers, next))
    {
        parser = CONTAINING_RECORD(next, PARSEReg, registeredList);

        if ((parser->PipeType == e_Print) && parser->ParseString && (0 == strcmp(CmpString, parser->ParseString)))
        {
	    DBG_PRINTF_INFO("found language: %s\n", CmpString);
            // Found the parser.  Get out of here and into the new language.
            Pipe->nextParserToRun = parser;
            return 1;
        }
    }
    return 0;
}

/** 
 * \brief Return the number of unique parsers we have
 * 
 * \retval uint32_t The number of parsers.
 * 
 **/
uint32_t GetNumLanguages(void)
{
    uint32_t cnt = 0;
    ATLISTENTRY *next = NULL;

    if (SysParserInitialized == false) 
        return 0;

    for (next = MListHead(&uniqueParsers); 
         next; 
         next = MListNext(&uniqueParsers, next))
    {
        cnt++;     // we didn't find any others, this is a valid parser.
    }
    return cnt;
}
 
/** 
 * \brief Return the pjl language string corresponding to parser val
 * 
 * \param Val   The count of the parser whose string is returned
 * 
 * \retval char* The pjl parse string Note only the first parser will return non-null.
 * 
 **/
char *GetPJLLanguageString(uint32_t Val)
{
    uint32_t cnt = Val+1; // 0 is first element.
    PARSEReg *parser = NULL;
    ATLISTENTRY *next = NULL;

    if (SysParserInitialized == false) 
        return 0;

    // find the N'th one its array indexing.
    for (next = MListHead(&uniqueParsers); 
         next && cnt; 
         next = MListNext(&uniqueParsers, next), cnt--)
    {
        parser = CONTAINING_RECORD(next, PARSEReg, uniqueList);
    }
    if (parser) {
        DBG_PRINTF_INFO("--- get PJLLanguageString %s \n", parser->ParseString);
        return parser->ParseString;
    }
    return NULL;
}


#if defined(HAVE_CMD)

static char* CHAN_TYPE_DBGSTR[] = 
{
   "UNI",
   "BIDI",
   "UNKNWN"
};

static char* CHAN_FUNCT_DBGSTR[] = 
{
    "e_Print",
    "e_Scan",
    "e_MassStorage",
    "e_HTTP",
    "e_FaxChan",
    "e_PictBridgeChan"
};

const char* DOCTYPE_DBGSTR[] =
{
    "e_NONE", 
    "e_DOCUMENT", 
    "e_DEMOPAGE", 
    "e_CONFIGPAGE", 
    "e_MENUPAGE", 
    "e_SUPPLYPAGE", 
    "e_CALPAGE",
    "e_ALIGNMENTPAGE",
    "e_CLEANINGPAGE",
    "e_COPYPAGE",
    "e_ERRORREPORT", 
    "e_SCANCALPAGE" 
};

static char* PIPE_STATUS_DBGSTR[] =
{
    "e_Error",  //= -1,
    "e_NoError"
};

//
// State variable used by connection manager.
//
static char* PIPE_STATE_DBGSTR[] =
{
    "e_Idle",     // waiting for something to happen.
    "e_InParser" // running a parser, waiting for response.
};


/** 
 * \brief display a list of connection manager pipes
 * 
 */
static void conmgr_dump_pipes( void )
{
    uint32_t i;
    IOReg *pPipe; 
    ATLISTENTRY* next = NULL;


    for (next = MListHead(&RegisteredPipes), i = 0;
         next; 
         next = MListNext(&RegisteredPipes, next), ++i)
    { 
        pPipe = CONTAINING_RECORD(next, IOReg, registeredList);

        cmd_printf("Pipe %d:\n", i);

        cmd_printf("\tDataBuffer, @ %p\n", pPipe->DataBuffer);
        cmd_printf("\tBufferPointer = %p\n", pPipe->BufferPointer);
        cmd_printf("\tLength (of rewind data) = %d\n", pPipe->Length);
        cmd_printf("\tRdBufLen = %d\n", pPipe->RdBufLen);
        cmd_printf("\tChanFunct = %d: %s\n", pPipe->PipeType, CHAN_TYPE_DBGSTR[pPipe->PipeType]);
        cmd_printf("\tnextParserToRun = %p\n", pPipe->nextParserToRun);
        cmd_printf("\tPipeStatus = %d: %s\n", pPipe->PipeStatus, PIPE_STATUS_DBGSTR[pPipe->PipeStatus+1]);
        cmd_printf("\tPipeState = %d: %s\n", pPipe->PipeState, PIPE_STATE_DBGSTR[pPipe->PipeState]);
        cmd_printf("\tFileHandle = %p\n", pPipe->io.data_read_handle);
        cmd_printf("\tWriteHandle = %p\n", pPipe->io.data_write_handle);
        cmd_printf("\tStatusHandle = %p\n", pPipe->io.cmd_write_handle);
        cmd_printf("\tParser = %p\n", pPipe->Parser);
	PrintJobInfo_cmd_dump(pPipe->pjob_info, cmd_printf);
    }
}

/** 
 * \brief Display a list of the connection manager's parsers
 * 
 */
static void conmgr_dump_parsers( void )
{
    PARSEReg *pParser = NULL;
    ATLISTENTRY *next = NULL;

    // Scan through the parsers looking for the one that is requested.
    //
    for (next = MListHead(&RegisteredParsers); 
         next; 
         next = MListNext(&RegisteredParsers, next))
    {
        pParser = CONTAINING_RECORD(next, PARSEReg, registeredList);
        cmd_printf("Parser %p:\n", pParser);
        cmd_printf("\tParser ModuleID = %d\n", pParser->ParserModuleID);
        if(pParser->ParseString != NULL)
        {
            cmd_printf("\tParseString = %s\n", pParser->ParseString);
        }
        if(pParser->CompareString != NULL)
        {
            cmd_printf("\tCompareString = %s\n", pParser->CompareString);
        }
        cmd_printf("\tCmpStringSize = %d\n", pParser->CmpStringSize);
        cmd_printf("\tPipeType = %d: %s\n", pParser->PipeType, CHAN_FUNCT_DBGSTR[pParser->PipeType]);
    }
}

static char *conmgr_cmd_dumppipes_desc = "print the connection manager's pipe list";
static char *conmgr_cmd_dumpparsers_desc = "print the connection manager's parser list";

static int conmgr_cmd_dumppipes_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    conmgr_dump_pipes();

    return CMD_OK;
}
static int conmgr_cmd_dumpparsers_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    conmgr_dump_parsers();

    return CMD_OK;
}
static int conmgr_cmd_closeall_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    
    uint32_t i;
    IOReg *Pipe; 
    ATLISTENTRY* next = NULL;

    for (next = MListHead(&RegisteredPipes), i = 0;
         next; 
         next = MListNext(&RegisteredPipes, next), ++i)
    { 
        Pipe = CONTAINING_RECORD(next, IOReg, registeredList);

        if (Pipe->Length == 0
            && Pipe->PipeStatus <= 0 // no more data on input pipe.
            && Pipe->io.ioctl != NULL)
        {
            cmd_printf(" Closeing = %p\n", Pipe);
            Pipe->io.ioctl(Pipe->io.data_read_handle, e_close, NULL);
            Pipe->PipeStatus = e_NoError; 
        }
    }

    return CMD_OK;
}
#endif

static void conmgr_cmd_register(void)
{
#ifdef HAVE_CMD 
    int cmd_res = 0; UNUSED_VAR(cmd_res); // release ignore registration failure
    cmd_res = cmd_register_cmd( "cm",
                                NULL,
                                "connection manager commands",
                                NULL,
                                NULL,
                                NULL );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "cm",
                                "dumppipes",
                                conmgr_cmd_dumppipes_desc,
                                NULL,
                                NULL,
                                conmgr_cmd_dumppipes_cb );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "cm",
                                "close",
                                "close all pipes",
                                NULL,
                                NULL,
                                conmgr_cmd_closeall_cb );
    ASSERT( CMD_OK == cmd_res );
    cmd_res = cmd_register_subcmd( "cm",
                                "dumpparsers",
                                conmgr_cmd_dumpparsers_desc,
                                NULL,
                                NULL,
                                conmgr_cmd_dumpparsers_cb );
    ASSERT( CMD_OK == cmd_res );
#endif
}



