/******************************************************************************
 * Copyright (c) 2006-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pcfax_parser.c
 *
 * \brief Scans the input pcfax stream and parses it.
 *
 */

#include <string.h>
#include <stdint.h>
#include <stdlib.h> /* for atoi */
#include <stdbool.h>
#include "pthread.h"
#include "posix_ostools.h"
#include "dprintf.h"
#include "lassert.h"
#include "sys_init_api.h"
#include "SJMgr.h"
#include "agResourceMgr.h"
#include "agConnectMgr.h"
#include "jbig.h"
#include "print_job_types.h"
#include "agMessage.h"
#include "memAPI.h"
#include "debug.h"
#include "logger.h"
#include "ioutils.h"
#include "print_job_api.h"
#include "fsu_pcfax.h"

#define FAX_SUPPORT	1

/*** local definitions ***/
#define DBGPCFAXP_SOFT     (DBG_SOFT|DBG_OUTPUT)   // Activate this line for LOTS of debug output. **********
//#define DBGPCFAXP_SOFT     0                       // Activate this line for HARDLY ANY debug output. **********
#define DBGPCFAXP_ITEM     (DBG_LOUD|DBG_OUTPUT)   // Activate this line for LOTS of debug output re PCFAX_ITEM. **********
//#define DBGPCFAXP_ITEM     0                       // Activate this line for HARDLY ANY debug output re PCFAX_ITEM. **********
#define DBGPCFAXP_LOUD     (DBG_LOUD|DBG_OUTPUT)
#define DBGPCFAXP_ERR      -1  // Important error conditions that should always be flagged.
//#define DBGPCFAXP_ERR      (DBG_ERROR|DBG_OUTPUT)  // Important error conditions that should always be flagged.

#define DBG_PRFX	 "pcfax:"
#define LOGGER_MODULE_MASK  ( DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT(25) )

/* PCFAX parser specific defines*/
#define PCFAX_MORE			(1)
#define PCFAX_CHOKE			(-1)
#define SIZE_OF_PCFAXPARSE_BUFFER 	1024


#define	STATE_START			10
#define	STATE_COMMAND			20
#define	STATE_FAXHEAD			21
#define	STATE_FAXIMAGEHEAD	22
#define	STATE_FAXIMAGEWIDTH	23
#define	STATE_SIZE				30
#define	STATE_BINARY			40
#define	STATE_PAGEEND			50
#define	STATE_JOBEND			55
#define	STATE_END				60

static int count=0, command_state = 0, inPCFAXCommand_State = 0;
static char PCFAXUel[]=			"\x1b%-12345";
static char PCFAXCommand[]=		"@PJL FAX FORMAT:BINARY NAME=";
static char PCFAXHEAD[]=			"FAXHEAD";
static char PCFAXIMAGEHEAD[]	=	"FAXIMAGEHEAD";
static char PCFAXIMAGEWIDTH[]=	"FAXIMAGEWIDTH";
static char PCFAXSIZE[]=			"SIZE=";
static char PCFAXPAGEEND[]=		"PAGEEND";
static char PCFAXJOBEND[]=		"JOBEND";


typedef struct pcfax_parse_message_wrapper_s
{
    ATLISTENTRY node;
    MESSAGE msg;
}pcfax_parse_message_wrapper_t;

/**
 * \brief Enumeration of PCFAX endian formats (big/little endian).
 */
typedef enum
{
    PCFAX_BIG_ENDIAN = 0,
    PCFAX_LITTLE_ENDIAN
} pcfax_endian_t;


/*** default settings ***/

/*** external functions ***/

/*** external data ***/

/** local definitions **/



/*** internal functions ***/
int DoPCFAXParsing(IOReg *, MODULE_ID );
void* pcfaxparser_thread( void* unused );

/*** public data ***/

/*** private data ***/

static mlimiter_t *mlimiter = 0;

char ParseStringPCFAX[]="PCFAX";
pcfax_endian_t pcfax_current_endian_format = PCFAX_BIG_ENDIAN;  // default to big endian; will be set by job parse start

#define NUM_MESSAGES 10

static pthread_t    pcfaxparser_thread_id;
static mqd_t        pcfaxparser_msgq;

#define PCFAXSTACK_SIZE POSIX_MIN_STACK_SIZE // 6kB stack req'd for wireless and WSD
uint8_t  PCFAXStack[PCFAXSTACK_SIZE] ALIGN8;

ATLISTENTRY PCFAXParseQueue;         // a list of parse stuff to do next.

typedef struct __ZPCFAX
{
	int State;
	int inWord;
	char szWord[1024];
	char cPrev;		// from int
}
_ZPCFAX;
typedef struct __ZPCFAX* ZPCFAX;

static int BytesRead;


/* This is mapping table used to convert from the incoming PCFAXStream color order
 * to our internal system color order.
 */ 

/*** public functions ***/


/*** public functions ***/

uint32_t agpcfaxParserInit(void)
{
	PARSE_SETUP Parser;

	DBG_PRINTF_INFO("%s is DBG\n",__FUNCTION__);

	Parser.ParserID = PCFAXPARSER;

	// register to support incoming data in big endian format
	Parser.ParseCmpString = NULL;
	Parser.StringSize = 1;		//
	Parser.ParseString = ParseStringPCFAX;		// "PJL" might be OK or ParseStringPCFAX
	Parser.ParseType = e_Print;	// ???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
	ParserRegister(&Parser);

	rm_register(e_PCFAXParser, PCFAXPARSER);

	ATInitList(&PCFAXParseQueue);

	posix_create_message_queue(&pcfaxparser_msgq, "/PCFAXParseQ", NUM_MESSAGES, sizeof(MESSAGE));
	router_register_queue(PCFAXPARSER, pcfaxparser_msgq);
	posix_create_thread(&pcfaxparser_thread_id, pcfaxparser_thread, 0, "pcfaxparser", PCFAXStack, PCFAXSTACK_SIZE, POSIX_THR_PRI_NORMAL);

	return(0);
}

/*** private functions ***/


// FUNCTION NAME: pcfaxparser_thread
/**
 * \brief Parse a KARA specific Stream print job.
 *
 *  This function is called by the system parser when a Parse string is located.  
 *  The data in the buffer is parsed and then more data is read until it is finished,
 *  when this function will return to waiting for another job to parse.
 *
 *         2008/10/24 - Restructured this "outer loop" to handle a
 *         Cancel Job message. This closed a window whereby such a message was
 *         lost if it arrived between certain other messages. Also restructured
 *         handling of the OurResource and Pipe pointers to plug other possible
 *         windows. The whole system should be redesigned away from the current
 *         message-based scheme!
 */
void* pcfaxparser_thread( void* unused)
{
	MESSAGE     nextMsg;
	CURRENT_RESOURCE*   OurResource = NULL;
	IOReg*              Pipe = NULL;
	bool Parsing = FALSE;
	pcfax_parse_message_wrapper_t* parse_msg;
	ATLISTENTRY* node;
	int32_t ParseRet;
	int err = 0;

	SysWaitForInit ();

	if (mlimiter == 0)
	{
		mlimiter = mlimiter_by_name("print");
	}

	DBG_PRINTF_ERR("PCFAXPARSE: initialize\n");

	while (1)
	{
		if ((node = ATRemoveHeadList(&PCFAXParseQueue)) != NULL)
		{
			parse_msg = CONTAINING_RECORD(node, pcfax_parse_message_wrapper_t, node);
			nextMsg = parse_msg->msg;
			MEM_FREE_AND_NULL(parse_msg);
		}
		else    
		{
			err = mq_receive(pcfaxparser_msgq, (char*)&nextMsg, sizeof(MESSAGE), 0);
			if (err != sizeof(MESSAGE))	continue;
		}
		DBG_PRINTF_INFO("PCFAXPARSE: got message '%s'=0x%X=%d; param3=0x%08X\n",
								DecodeSysMsgEnum(nextMsg.msgType), nextMsg.msgType,
								nextMsg.msgType, nextMsg.param3 );
		switch (nextMsg.msgType)
		{
			case MSG_START_PARSE:
				//
				// If we are curently parsing, save the message till we are done.
				//
				if (Parsing)
				{
					parse_msg = (pcfax_parse_message_wrapper_t*)MEM_MALLOC(sizeof(pcfax_parse_message_wrapper_t));
					ASSERT(parse_msg != NULL);
					if (parse_msg)
					{
						parse_msg->msg = nextMsg;
						ATInsertTailList(&PCFAXParseQueue, &parse_msg->node);
					}
				}
				else
				{
					STARTRECIPE *StartIt;

					StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
					ASSERT(StartIt != NULL);
					// we have a connection, get the resources and start.

					Pipe = (IOReg *) nextMsg.param3;        // save the pipe info.
					nextMsg.msgType = MSG_STARTRECIPE;
					StartIt->Job = ejob_ParseMisc;			// ????????????????????????????????????????????????????????????????????????????????????????????????????????????

					StartIt->AdditionalResource = e_PCFAXParser;
					StartIt->Pipe = Pipe;
					StartIt->Wait = e_WaitForever;
					nextMsg.param3 = StartIt;       // freed by sjm

					SYMsgSend(SJMID, &nextMsg);         // send the message.
					Parsing = TRUE;
				}
			break;

			case MSG_RESOURCES:
				if ( Parsing )
				{
					OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
					DBG_PRINTF_INFO("PCFAXPARSE: Resource %#x\n",OurResource);
				}
				else
				{
					nextMsg.msgType = MSG_FREERECIPE;
					SYMsgSend( SJMID, &nextMsg );
				}
			break;

			case MSG_ACKRECIPE:
				// Update status to indicate that a print job is being received.
				// This is necessary to cause the "processing" status message
				// to be displayed.

				// We have the resources, start the job.
				mlimiter_start( mlimiter_by_name("print"));
				count = 0;
				command_state = 0;

				ParseRet = DoPCFAXParsing(Pipe, OurResource->Destination);	

				mlimiter_stop( mlimiter_by_name("print"));

				DBG_PRINTF_INFO("PCFAXPARSE: Parse Done: free recipe: %#x, pipe: %#x\n", OurResource, Pipe);

				// finished parsing, free the resources.
				Parsing = FALSE;
				nextMsg.msgType = MSG_FREERECIPE;
				nextMsg.param3 = OurResource;
				SYMsgSend(SJMID, &nextMsg);
				//OurResource = NULL;

				// Tell the connection manager the results of the parse.
				nextMsg.param1 = ParseRet;			// @@ different part with PJL or GGS
				nextMsg.msgType = MSG_END_PARSE;
				nextMsg.param3 = Pipe;          // The pipe that we are done parsing.
				SYMsgSend(CMPARSER, &nextMsg);      // send the response.
				
				//nextMsg.msgType = MSG_FREEIO;
				//nextMsg.param3 = OurResource;
				//SYMsgSend(SJMID, &nextMsg);
				OurResource = NULL;
				Pipe = NULL;
				
				DBG_PRINTF_INFO("agpcfaxParser: Done Parsing\n");
			break;      // done

			case MSG_CANCELJOB:
				DBG_PRINTF_ERR("PCFAXPARSE: agpcfaxParser: got MSG_CANCELJOB param1=%d=%s param2=%d=%s param3=0x%08X\n",
									nextMsg.param1, ToString_DOACTION((DOACTION)nextMsg.param1),
									nextMsg.param2, ToString_MODULE_ID((MODULE_ID)nextMsg.param2),
									nextMsg.param3 );
				if ( nextMsg.param1 == SYS_START )
				{
					MESSAGE outMsg;
					Parsing = FALSE;

					if ( OurResource )
					{
						outMsg.msgType = MSG_FREERECIPE;
						outMsg.param1 = 0;
						outMsg.param2 = 0;
						outMsg.param3 = OurResource;
						SYMsgSend( SJMID, &outMsg );
						OurResource = NULL;
					}

					if ( Pipe )
					{
						outMsg.msgType = MSG_END_PARSE;
						outMsg.param1 = (uint32_t)PARSER_ERROR;
						outMsg.param2 = 0;
						outMsg.param3 = Pipe;
						SYMsgSend( CMPARSER, &outMsg );
						Pipe = NULL;
					}
					mlimiter_stop( mlimiter_by_name("print"));

					nextMsg.param1 = SYS_ACK;
					nextMsg.param2 = PCFAXPARSER;
					SYMsgSend( SJMID, &nextMsg ); // param3 preserved.
				}
			// else // Ignore SYS_ACK coming back to us.
			break;

			default:
				DBG_PRINTF_ERR("PCFAXPARSE: agpcfaxParser: message ignored!\n" );
			break;
		} // end switch ( nextMsg.msgType )

	} // end while (1)

	return 0;
} // end agpcfaxParser()

/* -------------------------------------------------------------------------- */
#if 1
#if 0
static int strcmpi_pcfax( const char *pStrOne, const char *pStrTwo )
{
	char one, two;
	while (1)
	{
		one = *pStrOne++,  two = *pStrTwo++;

		/*	make upper-case */
		if (one >= 'a' && one <= 'z')
			one -= ('a' - 'A');
		if (two >= 'a' && two <= 'z')
			two -= ('a' - 'A');

		/* compare */
		if (one < two)
			return (-1);
		if (one > two)
			return (1);
		if (!one && !two)
			return (0);
	}
}
#endif

int  pcfaxPutc(ZPCFAX zpcfax, int c)
{
	int  rc = -1;

	if( c == PCFAXUel[count] )
	{
		DBG_PRINTF_INFO("compare %d: %c and %c\n", count, PCFAXUel[count], c );
		count++;
		zpcfax->cPrev = c;		
		if(count >= strlen(PCFAXUel))
		{
			DBG_PRINTF_INFO("PCFAX Uel was checked 0x%x %c\n", c, c );
			zpcfax->inWord = 0;
			zpcfax->State = STATE_END;
		}
		return PCFAX_CHOKE;
	}
	
	if( inPCFAXCommand_State || (c == PCFAXCommand[count]) )
	{
		DBG_PRINTF_INFO("compare %d: %c and %c\n", count, PCFAXCommand[count], c );
		
		if ( (c != PCFAXCommand[count]) && (c == PCFAXCommand[count+1]) )
		{
			DBG_PRINTF_ERR("missed 1 character: [%c] , I am [%c]\n", PCFAXCommand[count], c );
			count++;
			DBG_PRINTF_ERR("count(%d) was increased one more\n", count );
		}
		
		count++;
		inPCFAXCommand_State = count;
		zpcfax->cPrev = c;		
		if(count >= strlen(PCFAXCommand))
		{
			DBG_PRINTF_INFO("PCFAX Command was checked 0x%x %c\n", c, c );
			count = 0;
			zpcfax->inWord = 0;
			zpcfax->State = STATE_COMMAND; /* @@ needed */
			inPCFAXCommand_State = 0;
		}
		return PCFAX_MORE;
	}

	DBG_PRINTF_INFO("count:%d cPrev 0x%x %c and state %d\n", count, zpcfax->cPrev, zpcfax->cPrev, zpcfax->State );
/* @@ find strings */
	if (zpcfax->cPrev != 0x0A )	// LF
	{
		switch ( zpcfax->State )
		{
			case STATE_COMMAND:
				if ( c == 0x0D )	// CR
				{
					zpcfax->cPrev = c;
					DBG_PRINTF_INFO("CR match (%d) %c \n", count, c );
				}
				else if ( c == 0x22 )	// "
				{
					zpcfax->cPrev = c;
					DBG_PRINTF_INFO("\" match (%d) %c \n", c );
				}
				else if ( (command_state<=STATE_FAXHEAD) && (c==PCFAXHEAD[count]) )
				{
					DBG_PRINTF_INFO("PCFAXHEAD[%d] match %c \n", count, c );
					count++;
					zpcfax->cPrev = c;		
					if(count >= strlen(PCFAXHEAD))
					{
						DBG_PRINTF_INFO("PCFAX HEAD was checked 0x%x %c\n", c, c );
						count = 0;
						zpcfax->inWord = 0;
						command_state = STATE_FAXHEAD; /* @@ needed */
					}
					return PCFAX_MORE;
				}
				else if ( (command_state<=STATE_FAXIMAGEHEAD) && (c==PCFAXIMAGEHEAD[count]) )
				{
					DBG_PRINTF_INFO("PCFAXIMAGEHEAD[%d] match %c \n", count, c );
					count++;
					zpcfax->cPrev = c;		
					if(count >= strlen(PCFAXIMAGEHEAD))
					{
						DBG_PRINTF_INFO("PCFAX IMAGEHEAD was checked 0x%x %c\n", c, c );
						count = 0;
						zpcfax->inWord = 0;
						command_state = STATE_FAXIMAGEHEAD; /* @@ needed */
					}
					return PCFAX_MORE;
				}
				else if ( (command_state<=STATE_FAXIMAGEWIDTH) && (c==PCFAXIMAGEWIDTH[count]) )
				{
					DBG_PRINTF_INFO("PCFAXIMAGEWIDTH[%d] match %c \n", count, c );
					count++;
					zpcfax->cPrev = c;		
					if(count >= strlen(PCFAXIMAGEWIDTH))
					{
						DBG_PRINTF_INFO("PCFAX IMAGEWIDTH was checked 0x%x %c\n", c, c );
						count = 0;
						zpcfax->inWord = 0;
						command_state = STATE_FAXIMAGEWIDTH; /* @@ needed */
					}
					return PCFAX_MORE;
				}
				else if ( c == PCFAXPAGEEND[count] )
				{
					DBG_PRINTF_INFO("PCFAXPAGEEND[%d] match %c \n", count, c );
					count++;
					zpcfax->cPrev = c;		
					if(count >= strlen(PCFAXPAGEEND))
					{
						DBG_PRINTF_INFO("PCFAX PAGEEND was checked 0x%x %c\n", c, c );
						count = 0;
						zpcfax->inWord = 0;
						command_state = STATE_PAGEEND; /* @@ needed */
					}
					return PCFAX_MORE;
				}
				else if ( c == PCFAXJOBEND[count] )
				{
					DBG_PRINTF_INFO("PCFAXJOBEND[%d] match %c \n", count, c );
					count++;
					zpcfax->cPrev = c;		
					if(count >= strlen(PCFAXJOBEND))
					{
						DBG_PRINTF_INFO("PCFAX JOBEND was checked 0x%x %c\n", c, c );
						count = 0;
						zpcfax->inWord = 0;
						command_state = STATE_JOBEND; /* @@ needed */
					}
					return PCFAX_MORE;
				}
				else if ( c == PCFAXSIZE[count] )
				{
					count++;
					zpcfax->cPrev = c;		
					if(count >= strlen(PCFAXSIZE))
					{
						DBG_PRINTF_INFO("PCFAX SIZE was checked 0x%x %c\n", c, c );
						count = 0;
						zpcfax->inWord = 0;
						zpcfax->State = STATE_SIZE; /* @@ needed */
					}
					return PCFAX_MORE;
				}
				else
				{
					DBG_PRINTF_INFO(" string pcfax : 0x%x %c inWord(%d) State(%d)\n", c, c, zpcfax->inWord, zpcfax->State );
					//if ( atoi( zpcfax->szWord )!=0 && c!=0 )
					//{
						//zpcfax->szWord[ zpcfax->inWord++ ] = (uint8_t) c;
						zpcfax->cPrev = c;
					//}
				}
			break;
			
			case STATE_SIZE:
				if ( c == 0x0D )	// CR
				{
					zpcfax->cPrev = c;
					DBG_PRINTF_INFO("CR %d \n", c );
				}
				else if ( c == 0x0A || c == 0x0C )	// LF, FF
				{
					/* number */
					zpcfax->szWord[ zpcfax->inWord++ ] = 0;
					zpcfax->inWord = atoi( zpcfax->szWord );
					DBG_PRINTF_INFO("size is : %d \n", zpcfax->inWord );

					zpcfax->State = STATE_BINARY;
					return PCFAX_MORE;
				}
				else if ( c > 0x39 || c < 0x30 /* @@ out of number */)	
				{
					DBG_PRINTF_INFO("CHOKE : %d \n", zpcfax->inWord );
					zpcfax->inWord = 0;
					zpcfax->State = STATE_END;
					return PCFAX_CHOKE;						
				}
				else
				{
					DBG_PRINTF_INFO(" string pcfax : 0x%x %c inWord(%d) State(%d)\n", c, c, zpcfax->inWord, zpcfax->State );
					zpcfax->szWord[ zpcfax->inWord++ ] = (uint8_t) c;
					zpcfax->cPrev = c;
				}
			break;
			
			default:
				DBG_PRINTF_INFO("Unknown state %d \n", zpcfax->State );
				zpcfax->inWord = 0;
				zpcfax->State = STATE_END;
				return PCFAX_CHOKE;	
			break;
		}
		zpcfax->cPrev = c;
	}
	else
	{
		DBG_PRINTF_INFO("cPrev is LF 0x10 \n");
		zpcfax->cPrev = -1;
		return PCFAX_CHOKE;
	}

/*TODO: check for parse errors*/
/*when should we return PCFAX_CHOKE, ever?*/
	rc = PCFAX_MORE;

/*always just clear it so it's ready the next time*/

	return(rc);
}

#endif
/* -------------------------------------------------------------------------- */

//	static unsigned char gPCFAXBuffer[512] ALIGN16;    
int DoPCFAXParsing(IOReg *Pipe, MODULE_ID Destination)
{
/* starting */
	ZPCFAX zpcfax;
	char *stream, *start = 0;
	int err, num, ret;

	zpcfax = MEM_MALLOC( sizeof(_ZPCFAX) );
	memset( zpcfax, 0, sizeof(_ZPCFAX) );

	zpcfax->cPrev = 0;
	zpcfax->State = -1;
	zpcfax->inWord = 0;
	zpcfax->szWord[0] = 0x00;

	DBG_PRINTF_INFO("zpcfax locate %d %d %d %d \n", &(zpcfax->State), &(zpcfax->inWord), &(zpcfax->szWord[0]), &(zpcfax->cPrev) );

	start = MEM_MALLOC( SIZE_OF_PCFAXPARSE_BUFFER );
	ASSERT(start);

	stream = start;
	BytesRead = Pipe->ReadConnect(Pipe, stream, SIZE_OF_PCFAXPARSE_BUFFER, PARSEREAD_ANY);

	err = PCFAX_MORE;
	while(err == PCFAX_MORE)
	{
		for (; (err == PCFAX_MORE) && ( BytesRead > 0); --BytesRead, ++stream)
		{
			err = pcfaxPutc(zpcfax, *stream);

			if (zpcfax -> State == STATE_BINARY )
			{
				stream++;	// get rid of LF
				num = zpcfax->inWord;
				DBG_PRINTF_INFO("after Putc 0x%x %c\n", *stream, *stream );
				DBG_PRINTF_ERR("STATE_BINARY: command_state=%d num=%d\n", command_state, num );
				
				Pipe->RewindData(Pipe, stream, BytesRead);// push the data back, if any left
				zpcfax -> State = STATE_COMMAND;

#if FAX_SUPPORT				
				switch ( command_state )
				{
					case STATE_FAXHEAD:
						ret = PCFax_set_pcfax_header(Pipe, num);
					break;
					
					case STATE_FAXIMAGEHEAD:
						ret = PCFax_set_pcfax_imageheader(Pipe, num);
					break;
					
					case STATE_FAXIMAGEWIDTH:
						ret = PCFax_set_pcfax_image(Pipe, num);
					break;
					
					case STATE_PAGEEND:
						ret = PCFax_set_pcfax_pageend();
						command_state = 0;
					break;
					
					case STATE_JOBEND:
						ret = PCFax_set_pcfax_jobend();
						command_state = 0;
					break;
					
					default:
						DBG_PRINTF_ERR("Err: Unknown command state %d\n", command_state );
					break;
				}
				DBG_PRINTF_ERR("STATE_BINARY: command_state=%d ret=%d\n", command_state, ret );
				if ( ret == -1 )
				{
					Pipe->pipe_cancel(Pipe);
					command_state = 0;
					DBG_PRINTF_ERR("[%d] command_state=%d\n", __LINE__, command_state);
				}
#else
				if ( num > SIZE_OF_PCFAXPARSE_BUFFER )
				{
					int trial=0;
					while ( num > 0 )
					{
						memset (start, 0, SIZE_OF_PCFAXPARSE_BUFFER );
						if ( num > SIZE_OF_PCFAXPARSE_BUFFER )
							ret = Pipe->ReadConnect(Pipe, start, SIZE_OF_PCFAXPARSE_BUFFER, PARSEREAD_ANY);
						else
							ret = Pipe->ReadConnect(Pipe, start, num, PARSEREAD_ANY);
							
						num -= ret;
						trial++;
						DBG_PRINTF_INFO("trial=%d read=%d remain=%d (0x%x %c)\n", trial, ret, num, *start, *start );
						usleep(1000);
					}
				}
				else
				{
					memset (start, 0, SIZE_OF_PCFAXPARSE_BUFFER );
					ret = Pipe->ReadConnect(Pipe, start, num, PARSEREAD_ANY);
					DBG_PRINTF_INFO("read=%d (0x%x %c)\n", ret, *start, *start );
				}
				DBG_PRINTF_INFO("Consumed binary data...\n");
				//for ( ; num >0 ; stream++, num-- )
					//DBG_PRINTF_INFO("%c\n", *stream );
#endif
			
				// we ran out of input stream, read more.
				memset (start, 0, SIZE_OF_PCFAXPARSE_BUFFER );
				//BytesRead = Pipe->ReadConnect(Pipe, start, SIZE_OF_PCFAXPARSE_BUFFER, PARSEREAD_ANY);
				BytesRead = Pipe->ReadConnect(Pipe, start, SIZE_OF_PCFAXPARSE_BUFFER, PARSEREAD_SOME);

				if(BytesRead <= 0)  // Must be done.
				{
					err = PCFAX_CHOKE;   // get out of here.
					BytesRead = 0;      // make sure rewind will work.
					if ( command_state )
					{
						PCFax_job_cancel();
						command_state = 0;
					}
					Pipe->io.ioctl(Pipe->io.data_read_handle, e_close, NULL);       // tell the guy we are done.
				}
				else
				{
					stream = start; // reset buffer pointer.
					DBG_PRINTF_INFO("after ReadConnect one more 0x%x %c\n", *stream, *stream );
					stream--;
					BytesRead++;
				}
			}
		}
		
		if(err == PCFAX_MORE)
		{
			DBG_PRINTF_ERR("Any more command ???\n");
			// we ran out of input stream, read more.
			memset (start, 0, SIZE_OF_PCFAXPARSE_BUFFER );
			//BytesRead = Pipe->ReadConnect(Pipe, start, SIZE_OF_PCFAXPARSE_BUFFER, PARSEREAD_ANY);
			BytesRead = Pipe->ReadConnect(Pipe, start, SIZE_OF_PCFAXPARSE_BUFFER, PARSEREAD_SOME);

			if(BytesRead <= 0)  // Must be done.
			{
				err = PCFAX_CHOKE;   // get out of here.
				BytesRead = 0;      // make sure rewind will work.
				if ( command_state )
				{
					PCFax_job_cancel();
					command_state = 0;
				}
				Pipe->io.ioctl(Pipe->io.data_read_handle, e_close, NULL);       // tell the guy we are done.
			}
			else
			{
				stream = start; // reset buffer pointer.
			}
		}
		else if (stream != start)// put back the character we just choked on. 
		{  
			stream--;
			BytesRead++; 
		}
	}
	//
	// We are finished parsing, put the excess bytes back into the pipe.
	//
	memFree(start);
	memFree(zpcfax);
	if ( BytesRead )
	{
		DBG_PRINTF_ERR("PCFAX parser rewind %d bytes.. \n", BytesRead+count);
		Pipe->RewindData(Pipe, stream, BytesRead+count);// push the data back, if any left
	}

	return PARSER_SUCCESS;
}
