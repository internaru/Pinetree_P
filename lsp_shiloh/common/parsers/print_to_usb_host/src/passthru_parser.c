
/*
 * ============================================================================
 * Copyright (c) 2011   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#include <string.h>

#include "lassert.h"
#include "posix_ostools.h"
#include "agRouter.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "mlimiter.h"
#include "logger.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "sys_init_api.h"

#define NUM_PT_MESSAGES 10

#define PT_STACK_SIZE POSIX_MIN_STACK_SIZE

static pthread_t px_PTParser_thd_id;
static mqd_t px_PTParseQ;

static uint8_t pt_stack[PT_STACK_SIZE] __attribute__ ((aligned (8)));

#define DBG_PRFX "passthru: "                      // simple string prepend to the front of all strings.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT(10)  // 

#define PT_IDLE_STATE 0
#define PT_ACTIVE_STATE 1

// HACK- NEEDS TO BE REMOVER WHEN CPU cpu_get_dcache_line_size function is ported
#define CACHE_LINE_SIZE 32
#define cpu_get_dcache_line_size() CACHE_LINE_SIZE

static void *pt_entry(void * input);

// Start this parser.
//
void passthru_parser_init(void)
{
//    uint32_t status;
    PARSE_SETUP Parser;

    memset(&Parser, 0, sizeof(PARSE_SETUP));
    Parser.ParseCmpString = NULL;
    Parser.StringSize = 0;      // garbage parser
    Parser.ParserID = PASSTHRUID;   // router id
    Parser.ParseType = e_Print;
    Parser.ParseString = "passthru"; // NULL;
    ParserRegister(&Parser);
    rm_register(e_PassThruParser, PASSTHRUID);

    posix_create_MESSAGE_queue( &px_PTParseQ, "/PT", NUM_PT_MESSAGES );

    router_register_queue(PASSTHRUID, px_PTParseQ);   // Register a message queue.

    posix_create_thread(&px_PTParser_thd_id, pt_entry, 0, "PTPar", 
                         pt_stack, PT_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    
}
#define PT_READ_BUF_SIZE 16*1024

static void *pt_entry(void * input)
{
    MESSAGE     nextMsg;
    IOReg *Pipe = NULL;
    CURRENT_RESOURCE  *OurResource = 0;
    uint32_t status;
    uint8_t *buffer = 0;
    uint32_t pt_state = PT_IDLE_STATE;
    int32_t size;  // read size
    uint32_t timeout;
    mlimiter_t *print_limiter;
    int iRET;


    SysWaitForInit();

    print_limiter = mlimiter_by_name("print");

    while (1)
    {
        timeout = 0;
        if (pt_state == PT_IDLE_STATE)
            timeout = POSIX_WAIT_FOREVER;

        iRET = posix_wait_for_message(px_PTParseQ, (char*)&nextMsg, sizeof(MESSAGE),
                                      timeout);
        if (iRET == 0)
        {
            switch (nextMsg.msgType)
            {
                case MSG_START_PARSE:
                    {
                        // cm says we have something to do, do it now.
                        STARTRECIPE *StartIt;

                        StartIt = (STARTRECIPE *)MEM_MALLOC(sizeof(STARTRECIPE));
                        ASSERT(StartIt != NULL);
                        // we have a connection, get the resources and start.
                        //
                        Pipe = (IOReg *) nextMsg.param3;
                        nextMsg.msgType = MSG_STARTRECIPE;
                        StartIt->Job = ejob_PrintIO;
                        StartIt->AdditionalResource = e_PassThruParser;
                        StartIt->Pipe = NULL;    // no status reported back by this method.
                        StartIt->Wait = e_WaitForever;  // don't need sending module if waitforever
                        nextMsg.param3 = StartIt;           // will be freed later

                        SYMsgSend(SJMID, &nextMsg);         // send the message.

                        break;
                    }
                case MSG_RESOURCES:
                    {
                        OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
                        mlimiter_start(print_limiter);
                        break;
                    }
                case MSG_ACKRECIPE:
                    {
                        // job has started, set the state and get the limiter going.
                        pt_state = PT_ACTIVE_STATE;
                        break;      // done
                    }
                case MSG_PRINT_DATA:
                    {
                        DBG_PRINTF_ERR("return channel buffer %x length %d\n",nextMsg.param3, nextMsg.param2);
                        Pipe->pipe_write(Pipe, nextMsg.param3, nextMsg.param2);
                        break;
                    }
                default:
                    break;
            }
        }
        if (pt_state != PT_IDLE_STATE)
        {
            ASSERT(Pipe != NULL);
            //
            // allocate a chunk of memory for the data.
            //
            if ( buffer == 0 ) 
            {
#ifdef __linux__
                buffer = MEM_MALLOC_LIMITED(print_limiter, PT_READ_BUF_SIZE); 
#else
                buffer = MEM_MALLOC_ALIGN_LIMITED(print_limiter, PT_READ_BUF_SIZE , cpu_get_dcache_line_size() );  // threadx dma alignment 
#endif
            }
            ASSERT(buffer);  // print limiter will block waiting for memory.
            // read something from the pipe, this will wait for up to 100 msec to
            // fill the buffer, returns on timeout or buffer full.
            //
            size = Pipe->pipe_read(Pipe, buffer, PT_READ_BUF_SIZE, PARSEREAD_SOME);
            if (size > 0)
            {
                // realloc the buffer, this will return the unused memory to
                // the memory manager. so will the free.
                //
                // buffer = MEM_REALLOC_LIMITED(print_limiter, buffer, size);
                // send the data on.
                nextMsg.msgType = MSG_PRINT_DATA;
                nextMsg.param2 = size;
                nextMsg.param3 = buffer;

                status = SYMsgSend(OurResource->Destination, &nextMsg);  // send the data on.
                if (status) // free on error
                {
                    DBG_PRINTF_ERR("msg send error: buffer %x length %d\n",buffer, size);

                    MEM_FREE_AND_NULL(buffer); 
                }
                else 
                {
                    DBG_PRINTF_ERR("Sent/passed buffer %x length %d\n",buffer, size);
                    buffer = 0; // ownership passed in message
                }
            } 
            else 
            {
                nextMsg.msgType = MSG_JOBEND;
                nextMsg.param3 = OurResource;
                SYMsgSend(OurResource->Destination, &nextMsg);

                nextMsg.msgType = MSG_FREERECIPE;
                nextMsg.param1 = 0; // in_message.param1;
                nextMsg.param2 = 0; // in_message.param2;
                nextMsg.param3 = OurResource;
                SYMsgSend(SJMID, &nextMsg);


                nextMsg.msgType = MSG_END_PARSE;
                nextMsg.param1 = 0;
                nextMsg.param2 = 0;
                nextMsg.param3 = Pipe;          // The pipe that we are done parsing.
                SYMsgSend(CMPARSER, &nextMsg); 


                OurResource = 0;
                pt_state = PT_IDLE_STATE;
            }
        }
    }
    return 0;
}
