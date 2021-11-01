/******************************************************************************
 * Copyright (c) 2006-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pf_parser.c
 *
 * \brief Scans the input pf stream and parses it.
 *
 */

#include <stdio.h>
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
#include "jsonrpc.h"	/* 2013.09.12		req. from jung jil su */

/*** local definitions ***/
#define DBGPFP_SOFT     (DBG_SOFT|DBG_OUTPUT)   // Activate this line for LOTS of debug output. **********
//#define DBGPFP_SOFT     0                       // Activate this line for HARDLY ANY debug output. **********
#define DBGPFP_ITEM     (DBG_LOUD|DBG_OUTPUT)   // Activate this line for LOTS of debug output re PF_ITEM. **********
//#define DBGPFP_ITEM     0                       // Activate this line for HARDLY ANY debug output re PF_ITEM. **********
#define DBGPFP_LOUD     (DBG_LOUD|DBG_OUTPUT)
#define DBGPFP_ERR      -1  // Important error conditions that should always be flagged.
//#define DBGPFP_ERR      (DBG_ERROR|DBG_OUTPUT)  // Important error conditions that should always be flagged.

#define DBG_PRFX	 "pf:"
#define LOGGER_MODULE_MASK  ( DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT(24) )

/* PF parser specific defines		2013.09.05	by Juny */
#define PF_MORE			(1)
#define PF_CHOKE			(-1)
#define SIZE_OF_PFPARSE_BUFFER 	10000

#define PFI_QUALITY_DRAFT -3
#define PFI_QUALITY_BEST -4

// Compare strings for both big and little endian data formats.
#define PF_CMP_STRING_BE_DATA "FPFP"    ///< PF data sent in big endian format
#define PF_CMP_STRING_LE_DATA "PFPF"    ///< PF data sent in little endian format

#define	UEL_PF		0
#define	START_PF	1
#define	STRING_PF	3
#define	COMMAND_PF	100
#define	JSON_PF	5
#define	BINARY_PF	250
#define	END_PF		255

static int pf_json_test = 0;
////static char PFUel[]="\x1b%-12345";
static char PFJson[]="@PJL JSON=";

/* 2013.12.02		req. from version	 by Juny */
// first : SINDOH release version, second : SINDOH parser version, third : update version
/* history
 * 1.1.00		2013.12.02 set version
 * 1.1.01 (2014.02.20): pcfax parser 신규 추가
 * 1.1.02 (2014.03.11): pcfax: fax api I/F 추가
 * 1.1.03 (2014.03.27): pcfax: pageend, jobend 처리 추가
 * 1.1.04 (2014.04.01): pcfax: parser end 처리 추가 (기본동작 구현완료)
 * 1.1.05 (2014.04.01): pcfax: parser end 처리 수정
 * 1.1.06 (2014.04.02): pf, pcfax: parser end 처리 재수정
 * 1.1.07 (2014.05.08): pcfax: cancel 처리
 * 1.1.08 (2014.05.12): pcfax: 시작시 command_state 초기화
 * 1.1.09 (2014.05.20): pcfax: PJL Command 1byte 누락되는거 처리
 * 1.1.10 (2014.05.23): pcfax: PCFax_job_cancel() call 추가
 * 1.1.11 (2014.05.26): pcfax: PCFax_job_cancel() call 한군데 더 추가
 * 1.1.12 (2014.05.28): pcfax: Fax return FAIL(-1) 이면 PCFax_job_cancel() call 안함
*/
char SINDOHPARSER_VERSION[]={"1.1.12"};
char* get_version_SindohParser()
{
   return (&SINDOHPARSER_VERSION[0]);
}

typedef struct pf_parse_message_wrapper_s
{
    ATLISTENTRY node;
    MESSAGE msg;
}pf_parse_message_wrapper_t;

/**
 * \brief Enumeration of PF endian formats (big/little endian).
 */
typedef enum
{
    PF_BIG_ENDIAN = 0,
    PF_LITTLE_ENDIAN
} pf_endian_t;

#ifdef PIXELCNT_BYHOST
static PIXELCNT pixels;
#endif // PIXELCNT_BYHOST

/*** default settings ***/

/*** external functions ***/

/*** external data ***/

/** local definitions **/



/*** internal functions ***/
int DoPFParsing(IOReg *, MODULE_ID );
void* pfparser_thread( void* unused );

/*** public data ***/

/*** private data ***/

static mlimiter_t *mlimiter = 0;

char ParseStringKARA[]="KARA";
pf_endian_t pf_current_endian_format = PF_BIG_ENDIAN;  // default to big endian; will be set by job parse start

#define NUM_MESSAGES 10

static pthread_t    pfparser_thread_id;
static mqd_t        pfparser_msgq;

#define PFSTACK_SIZE POSIX_MIN_STACK_SIZE // 6kB stack req'd for wireless and WSD
uint8_t  PFStack[PFSTACK_SIZE] ALIGN8;

ATLISTENTRY PFParseQueue;         // a list of parse stuff to do next.

typedef struct __ZPF
{
  int State;
  int inWord;
  char szWord[256];
  char cPrev;		// from int
}
_ZPF;
typedef struct __ZPF* ZPF;

static int BytesRead;


/* This is mapping table used to convert from the incoming PFStream color order
 * to our internal system color order.
 */ 

/*** public functions ***/


/*** public functions ***/

uint32_t agpfParserInit(void)
{
    PARSE_SETUP Parser;

    DBG_PRINTF_ERR(" Sindoh Parser init done.. ver=%s\n", &SINDOHPARSER_VERSION[0] );
    DBG_PRINTF_ERR("===========================================\n");
    
    Parser.ParserID = PFPARSER;

    // register to support incoming data in big endian format
    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1;		//
    Parser.ParseString = ParseStringKARA;		// "PJL" might be OK or ParseStringKARA		2013.08.29	by Juny
    Parser.ParseType = e_Print;
    ParserRegister(&Parser);

    rm_register(e_PFParser, PFPARSER);

    ATInitList(&PFParseQueue);

    posix_create_message_queue(&pfparser_msgq, "/PFParseQ", NUM_MESSAGES, sizeof(MESSAGE));
    router_register_queue(PFPARSER, pfparser_msgq);
    posix_create_thread(&pfparser_thread_id, pfparser_thread, 0, "pfparser", PFStack, PFSTACK_SIZE, POSIX_THR_PRI_NORMAL);
    
    return(0);
}

/*** private functions ***/

static inline uint32_t pf32_to_cpu(uint32_t value)
{
    return (pf_current_endian_format == PF_BIG_ENDIAN) ? be32_to_cpu(value) : le32_to_cpu(value);
}

static inline uint16_t pf16_to_cpu(uint16_t value)
{
    return (pf_current_endian_format == PF_BIG_ENDIAN) ? be16_to_cpu(value) : le16_to_cpu(value);
}

// FUNCTION NAME: pfparser_thread
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
void* pfparser_thread( void* unused)
{
    MESSAGE     nextMsg;
    CURRENT_RESOURCE*   OurResource = NULL;
    IOReg*              Pipe = NULL;
    bool Parsing = FALSE;
    pf_parse_message_wrapper_t* parse_msg;
    ATLISTENTRY* node;

    SysWaitForInit ();

    if (mlimiter == 0)
    {
        mlimiter = mlimiter_by_name("print");
    }

    DBG_PRINTF_ERR("PFPARSE: initialize\n");

    while (1)
    {
        if ((node = ATRemoveHeadList(&PFParseQueue)) != NULL)
        {
            parse_msg = CONTAINING_RECORD(node, pf_parse_message_wrapper_t, node);
            nextMsg = parse_msg->msg;
            MEM_FREE_AND_NULL(parse_msg);
        }
        else    
        {
            mq_receive(pfparser_msgq, (char*)&nextMsg, sizeof(MESSAGE), 0);
        }
        DBG_PRINTF_INFO("PFPARSE: got message '%s'=0x%X=%d; param3=0x%08X\n",
                              DecodeSysMsgEnum(nextMsg.msgType), nextMsg.msgType,
                              nextMsg.msgType, nextMsg.param3 );
        switch (nextMsg.msgType)
        {
            case MSG_START_PARSE:
                {
                    //
                    // If we are curently parsing, save the message till we are done.
                    //
                    if (Parsing)
                    {
                        parse_msg = (pf_parse_message_wrapper_t*)MEM_MALLOC(sizeof(pf_parse_message_wrapper_t));
                        ASSERT(parse_msg != NULL);
                        if (parse_msg)
                        {
                            parse_msg->msg = nextMsg;
                            ATInsertTailList(&PFParseQueue, &parse_msg->node);
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
                        StartIt->Job = ejob_ParseMisc;			// @@ might be changed with SYSTEM and job type		2013.08.30	by Juny
 
                        StartIt->AdditionalResource = e_PFParser;
                        StartIt->Pipe = Pipe;
                        StartIt->Wait = e_WaitForever;
                        nextMsg.param3 = StartIt;       // freed by sjm

                        SYMsgSend(SJMID, &nextMsg);         // send the message.
                        Parsing = TRUE;
                    }
                    break;
                }

            case MSG_RESOURCES:
                if ( Parsing )
                {
                    OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
                    DBG_PRINTF_INFO("PFPARSE: Resource %#x\n",OurResource);
                }
                else
                {
                    nextMsg.msgType = MSG_FREERECIPE;
                    SYMsgSend( SJMID, &nextMsg );
                }
                break;

            case MSG_ACKRECIPE:
                {
                    int32_t ParseRet;
                    pf_json_test = 0;

                    // Update status to indicate that a print job is being received.
                    // This is necessary to cause the "processing" status message
                    // to be displayed.

                    // We have the resources, start the job.
                    mlimiter_start( mlimiter_by_name("print"));

                    ParseRet = DoPFParsing(Pipe, OurResource->Destination);	

                    mlimiter_stop( mlimiter_by_name("print"));

                    DBG_PRINTF_INFO("PFPARSE: Parse Done: free recipe: %#x, pipe: %#x\n", OurResource, Pipe);

                    // finished parsing, free the resources.
                    Parsing = FALSE;
                    nextMsg.msgType = MSG_FREERECIPE;
                    nextMsg.param3 = OurResource;
                    SYMsgSend(SJMID, &nextMsg);
                    //OurResource = NULL;

                    // Tell the connection manager the results of the parse.
                    nextMsg.param1 = ParseRet;			// @@ different part with PJL or GGS		by Juny
                    nextMsg.msgType = MSG_END_PARSE;
                    nextMsg.param3 = Pipe;          // The pipe that we are done parsing.
                    SYMsgSend(CMPARSER, &nextMsg);      // send the response.
                    
                    //nextMsg.msgType = MSG_FREEIO;
                    //nextMsg.param3 = OurResource;
                    //SYMsgSend(SJMID, &nextMsg);
                    OurResource = NULL;
                    Pipe = NULL;
                    
                    DBG_PRINTF_INFO("agzparser: Done Parsing\n");
                    break;      // done
                }
            case MSG_CANCELJOB:
                DBG_PRINTF_ERR("PFPARSE: agpfParser: got MSG_CANCELJOB param1=%d=%s param2=%d=%s param3=0x%08X\n",
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
                  nextMsg.param2 = PFPARSER;
                  SYMsgSend( SJMID, &nextMsg ); // param3 preserved.
                  }
                // else // Ignore SYS_ACK coming back to us.
                break;

            default:
              DBG_PRINTF_ERR("PFPARSE: agpfParser: message ignored!\n" );
              break;
        } // end switch ( nextMsg.msgType )

    } // end while (1)

    return 0;
} // end agpfParser()

/* -------------------------------------------------------------------------- */
#if 1
#if 0
static int strcmpi_pf( const char *pStrOne, const char *pStrTwo )
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


int  pfPutc(ZPF zpf, int c)
{
    int  rc = -1;

    DBG_PRINTF_INFO("compare 0x%d %c and %c\n", pf_json_test, PFJson[pf_json_test], c );	// 2013.09.10 	by Juny
    if( c == PFJson[pf_json_test] )
    {
        pf_json_test++;
		  zpf->cPrev = c;		
        if(pf_json_test >= strlen(PFJson))
        {
            DBG_PRINTF_INFO("JSON checked 0x%x %c\n", c, c );	// 2013.09.10 	by Juny
            pf_json_test = 0;
            zpf->inWord = 0;

  		     zpf->State = JSON_PF; /* @@ needed */

            return PF_MORE; // UEL followed by @ is a comment don't exit we keep parsing and return what we don't grok.
        }
		return PF_MORE;
    }
//// else
////	{
////        DBG_PRINTF_INFO("JSON mis-checked 0x%x %c\n", c, c );	// 2013.09.10 	by Juny
////        pf_json_test = 0;
////	}


    DBG_PRINTF_INFO("cPrev 0x%d %c and state %d\n", zpf->cPrev, zpf->cPrev, zpf->State );	// 2013.09.10 	by Juny
/* @@ find strings */
	if (zpf->cPrev != 0x0A )
	{
		switch ( zpf->State )
		{
			case JSON_PF:
					if ( c == 0x0D )
					{
   					 	zpf->cPrev = c;
		           	DBG_PRINTF_INFO("CR %d \n", c );
					}
					else if ( c == 0x0A || c == 0x0C )
					{
						/* number */
   		             	zpf->inWord = atoi( zpf->szWord );
		           	DBG_PRINTF_INFO("size is : %d \n", zpf->inWord );
					
						zpf->State = BINARY_PF;
					}					
					else if ( c > 0x39 || c < 0x30 /* @@ out of number */)
					{
			           	DBG_PRINTF_INFO("CHOKE : %d \n", zpf->inWord );
							zpf->inWord = 0;
							zpf->State = END_PF;
					   		return PF_CHOKE;						
					}
					else
					{
	   		       	DBG_PRINTF_INFO(" string pf : 0x%x %c (%d)\n", c, c, zpf->State );	// 2013.09.10 	by Juny
						zpf->szWord[ zpf->inWord++ ] = (uint8_t) c;
   					 	zpf->cPrev = c;
					}
				break;
			default:
			{
	       	DBG_PRINTF_INFO("default is wrong \n");	// 2013.09.10 	by Juny
				zpf->inWord = 0;
				zpf->State = END_PF;
		   		return PF_CHOKE;	
				break;
			}
		}
       zpf->cPrev = c;
	}
    else
    {
	     DBG_PRINTF_INFO("cPrev is LF 0x10 \n");	// 2013.09.10 	by Juny
        zpf->cPrev = -1;
        return PF_CHOKE;
    }

/*TODO: check for parse errors*/
/*when should we return PF_CHOKE, ever?*/
		  rc = PF_MORE;

/*always just clear it so it's ready the next time*/

    return(rc);
}

#endif
/* -------------------------------------------------------------------------- */

//	static unsigned char gPFBuffer[512] ALIGN16;    
int DoPFParsing(IOReg *Pipe, MODULE_ID Destination)
{
/* starting */
    ZPF zpf;
    char *stream, *start, *pf_pointer, *pf_ptr;
	 int err, num, nbinary;

	 zpf = MEM_MALLOC( sizeof(_ZPF) );
	 memset( zpf, 0, sizeof(_ZPF) );

	 zpf->cPrev = 0;
	 zpf->State = -1;
	 zpf->inWord = 0;
	 zpf->szWord[0] = 0x00;

    DBG_PRINTF_INFO("zpf locate %d %d %d %d \n", &(zpf->State), &(zpf->inWord), &(zpf->szWord[0]), &(zpf->cPrev) );	// 2013.09.13 	by Juny

    start = MEM_MALLOC( SIZE_OF_PFPARSE_BUFFER );
    ASSERT(start);

    stream = start;
	 BytesRead = Pipe->ReadConnect(Pipe, stream, SIZE_OF_PFPARSE_BUFFER, PARSEREAD_ANY);

	 err = PF_MORE;
    while(err == PF_MORE)
     {
    	for (; (err == PF_MORE) && ( BytesRead > 0); --BytesRead, ++stream)
		{
			err = pfPutc(zpf, *stream);

			if (zpf -> State == BINARY_PF)
			{
				stream++;	// get rid of LF		2013.09.11	by Juny
				num = zpf->inWord;
				pf_pointer = pf_ptr = MEM_MALLOC( num );
				memset(pf_ptr, 0, num );

				/* num or BytesRead */
				nbinary = ( ( (num - BytesRead) > 0 )? BytesRead: num );
			   DBG_PRINTF_INFO("binary round is %d \n", nbinary );	// 2013.09.10 	by Juny
				for ( ; nbinary > 0 ; --nbinary, ++stream, ++pf_pointer, --BytesRead, --num)
				{
					(*pf_pointer) = *stream;
				   DBG_PRINTF_INFO("store 0x%x %c %d\n", *stream, *stream, stream );	// 2013.09.10 	by Juny
				}

//									zpf->cPrev = *stream;
				--stream;
				DBG_PRINTF_INFO("finished 0x%x %c\n", *stream, *stream );	// 2013.09.10 	by Juny
				zpf->inWord = 0;

				if ( num <= 0)
				{
					DBG_PRINTF_INFO("go JSON \n");	// 2013.09.12 	by Juny
                    char response[32*1024];
					ParseJSONRPC(pf_ptr, response);
                    if (response != NULL) {
                        char *res_temp;
                        res_temp = MEM_MALLOC(32*1024);
                        sprintf(res_temp, "%s", response);
                        DBG_PRINTF_ERR("%s",res_temp);
                        Pipe->WriteConnect(Pipe, res_temp, strlen(response));
                        sleep(2);
                    }
					memFree(pf_ptr);
                    //DBG_PRINTF_ERR("free pf_ptr\n");
					zpf -> State = START_PF;
				}
			}
		}
		DBG_PRINTF_INFO("out of for \n");	// 2013.09.12 	by Juny
       if(err == PF_MORE)
        {
       	// we ran out of input stream, read more.
			memset (start, 0, SIZE_OF_PFPARSE_BUFFER );
          BytesRead = Pipe->ReadConnect(Pipe, start, SIZE_OF_PFPARSE_BUFFER, PARSEREAD_ANY);

          if(BytesRead <= 0)  // Must be done.
           {
          	err = PF_CHOKE;   // get out of here.
             BytesRead = 0;      // make sure rewind will work.
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
    //DBG_PRINTF_ERR("free start\n");
	memFree(zpf);
    //DBG_PRINTF_ERR("free zpf\n");
	if ( BytesRead )
	{
		DBG_PRINTF_ERR("PF parser rewind %d bytes.. \n", BytesRead);	// 2013.08.20 	by Juny
		Pipe->RewindData(Pipe, stream, BytesRead);// push the data back, if any left
	}
   

   return PARSER_SUCCESS;
}
