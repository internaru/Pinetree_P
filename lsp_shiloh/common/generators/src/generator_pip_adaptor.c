/**
 *
 * ============================================================================
 * Copyright (c) 2011-2014 Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file generator_pip.c 
 * \brief Print Image Pipeline generator.  Transforms incoming 
 *        raw contone page data to print ready halftoned data.
 */

/** include files **/
#include <stdint.h>
#include "pthread.h"
#include "posix_ostools.h"
#include "agRouter.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "lassert.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "sys_init_api.h"
#include "SJMgr.h"
#include "generator_inits.h"

#include "pip_PAGE_page_adaptor.h"
#include "pip_PLANE_strip_adaptor.h"

#include "dbg_pnm.h"

#ifndef cp_mono_only
// decouple for builds without cp_strip.h
// #include "cp_strip.h"

#if (NUM_MARKING_COLOR_CHANNELS == 1) 
#define cp_mono_only() (true)
#else
//
#define cp_mono_only() (false)
#endif
#endif // end cp_mono_only

extern error_type_t pip_pqcmd_init(void); // PQ commands

#if defined(HAVE_CMD) && !defined(COLOR_ENG)
#include <string.h>
#include "cmd_proc_api.h"

extern COLOR next_forced_color( COLOR c );
#define map_plane_color_to_output_color(x) COLOR_BLACK 
#else 
/// lsp todo want to define this for a real color printer.
#define next_forced_color( x ) (x)
#define map_plane_color_to_output_color(x) (x)
#endif


/** local definitions **/
#define GENPIP_STACK_SIZE POSIX_MIN_STACK_SIZE
#define GENPIP_QUEUE_SIZE 10

#define DBG_PRFX "GENPIP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT(4)


/** default settings */
 

/** external data **/

/** internal functions **/
void* genpip_task( void* );

/** public data **/

/** private data **/
static pthread_t    genpip_thread_id;
static mqd_t        genpip_msgq;
static ALIGN8 uint8_t genpip_stack[GENPIP_STACK_SIZE];
static mlimiter_t* limiter = NULL;

/** public functions **/                              

/** 
 * \brief Initialize the PIP generator.
 * 
 * \param None.
 *
 * \retval uint32_t 0
 * 
 **/
uint32_t genpip_init( void )
{
    // Create a mail box and register it.
    posix_create_message_queue(&genpip_msgq, "/GenpipQ", GENPIP_QUEUE_SIZE, sizeof(MESSAGE));
      
    // Register the queue with the router.
    router_register_queue(GENPIP_ID, genpip_msgq);
      
    // Register the resource
    rm_register(e_GenPip,  GENPIP_ID);

   // Register 'pq' commands, don't worry about result.
   (void) pip_pqcmd_init();

   // start the generator task
    posix_create_thread(&genpip_thread_id, genpip_task, 0, "genpip_task", genpip_stack, GENPIP_STACK_SIZE, POSIX_THR_PRI_REALTIME);


   return(0);
}


/// todo: cleanup pip_cfg mode adaptor 
#include "print_mode.h"
#include "pip_api.h"
static uint8_t  use_override_pipmode = 0;
static uint32_t override_pipmode = IPC_UNUSED;

int genpip_force_mode(int value)
{
    if (value < 0) {
       use_override_pipmode = 0;
       override_pipmode = IPC_UNUSED;
       return 1;
    } else if (pip_is_mode_supported(value)) {
       use_override_pipmode = 1;
       override_pipmode = value;
       return 1;
    }
    return 0;
}
int genpip_get_mode(void)
{
   return (use_override_pipmode == 0) ? -1 : override_pipmode;
}

static inline PLANE_TYPE map_color_to_plane_type(COLOR color)
{
    static PLANE_TYPE plane_type_map[] = {PLANAR_K, PLANAR_C, PLANAR_Y, PLANAR_M};
    // map declaration above relies on the order enforced by this assert
    ASSERT(COLOR_BLACK == 0 && COLOR_CYAN == 1 && COLOR_YELLOW == 2 && COLOR_MAGENTA == 3);

    return plane_type_map[color];
}

/**
 *  \brief PIP generator task.
 *  The task sits between the print_job_api interface and the print job manager. 
 *  It processes the print job manager messages and data and then passes the
 *  processed messages and data on to the print job manager.
 * 
 * \param temp Dummy.
 * 
 * \retval None
 *
 * 
 **/
void* genpip_task(void* temp)
{
    MESSAGE msg, new_msg;
    CURRENT_RESOURCE  *our_resource = NULL;
    error_type_t ret;
    int mq_ret;
    PAGE_DATA *page = NULL;
    PLANED *plane = NULL;
    static int pagecnt = 0; //debug output counter
    static int planecnt; //debug output counter

    SysWaitForInit();

    DBG_PRINTF_NOTICE("%s\n", __func__);

    struct pip_page_adaptor * page_adaptor = construct_pip_PAGE_page_adaptor( 0 );
    struct pip_strip_adaptor * strip_adaptor = construct_pip_PLANE_strip_adaptor( 0 );
    // todo: remove subclass cast usage...
    struct pip_PAGE_page_adaptor * PAGE_adaptor = (struct pip_PAGE_page_adaptor *) page_adaptor; 
    struct pip_PLANE_strip_adaptor * plane_adaptor = (struct pip_PLANE_strip_adaptor *) strip_adaptor; 


    // main loop
    while (1)
    {
        mq_ret = mq_receive(genpip_msgq, (char*)&msg, sizeof(MESSAGE), 0);
        DBG_XASSERT(mq_ret == sizeof(MESSAGE), mq_ret);
	if (mq_ret != sizeof(MESSAGE)) 
	    continue; /// will try again but we are probably very dead
        switch (msg.msgType)
        {
            case MSG_RESOURCES:
            {
                DBG_PRINTF_NOTICE("MSG_RESOURCES resource = %#x\n", msg.param3);
                page = NULL;
                plane = NULL;

                our_resource = (CURRENT_RESOURCE *) msg.param3;
		plane_adaptor->destination = our_resource->Destination; // todo a better way than derived class 
		PAGE_adaptor->destination = our_resource->Destination;  // holding onto the variable ...
                DBG_PRINTF_NOTICE("%s: our_resource.destination = %d\n", __func__, our_resource->Destination);
                break;
            }
            case MSG_ACKRECIPE:
            {
                // Job is starting, we do nothing here, just wait for messages.
                break;
            }
            case MSG_JOBSTART:
            {
                DBG_PRINTF_NOTICE("MSG_JOBSTART\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );

                // set the limiter
                // BUGBUG: this should really be passed along with the job since we may not be using the print limiter
                limiter = mlimiter_by_name("print");
                mlimiter_start(limiter);
                DBG_ASSERT( OK==ret );  UNUSED_VAR(ret);
                break;
            }
            case MSG_JOBSHEETSTART:
            {
                DBG_PRINTF_NOTICE("MSG_JOBSHEETSTART\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }
            case MSG_JOBPAGESTART:
            {
                ASSERT( NULL != our_resource );
                planecnt = 0;
                pagecnt++;
                page = (PAGE_DATA *)msg.param3;
                DBG_PRINTF_NOTICE("MSG_JOBPAGESTART page = %#x\n", (uint32_t)page);
                ASSERT(page != NULL);
                ASSERT(IS_PAGE_DATA(page));

                // Adjust output settings
                page->image_output_info.image_engshift = 0;
                page->image_output_info.image_scale_right_padding *= (600/page->image_output_info.image_dpi.x);
                page->image_output_info.image_top_padding *= (600/page->image_output_info.image_dpi.x);

                DBG_PRINTF_ERR("es:%d srp:%d tp:%d\n", page->image_output_info.image_engshift, page->image_output_info.image_scale_right_padding, page->image_output_info.image_top_padding);

                pip_recv_page( page_adaptor, page );
                pip_new_page( strip_adaptor );

                break;
            }
            case MSG_JOBPLANESTART:
            {
                DBG_PRINTF_NOTICE("MSG_JOBPLANESTART\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }
            case MSG_JOBPLANEDATA:
            {
                ASSERT(NULL != our_resource);

                plane = (PLANED *)msg.param3;
                ASSERT(IS_PLANED(plane));

                DBG_PRINTF_NOTICE("MSG_JOBPLANEDATA plane = %#x\n", (uint32_t)plane);

#if 1
                // process data as needed and pass to destination
                if (plane->BlockDataType == PACKED_RGB)
                    dbg_dump_stripdata_to_ppm_file( pagecnt, plane, planecnt );
                else
                    dbg_dump_stripdata_to_pgm_file( pagecnt, plane, planecnt );
#endif
				pip_recv_strip( page_adaptor, strip_adaptor, plane );

                planecnt++;
                break;
            }
            case MSG_JOBPLANEEND:
            {
                DBG_PRINTF_NOTICE("MSG_JOBPLANEEND\n");
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }
            case MSG_JOBPAGEEND:
            {
                DBG_PRINTF_NOTICE("MSG_JOBPAGEEND\n");
                ASSERT( NULL != our_resource );
                page = NULL;
                from_page_close( page_adaptor );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );  
                break;
            }
            case MSG_JOBEND:
            {
                DBG_PRINTF_NOTICE("MSG_JOBEND\n");
                                            
                ASSERT( NULL != our_resource );
                ret = SYMsgSend( our_resource->Destination, &msg );
                ASSERT( OK==ret );                         
                
                // free resources
                new_msg.msgType = MSG_FREERECIPE;
                new_msg.param1 = 0;
                new_msg.param2 = 0;
                new_msg.param3 = our_resource;
                if (limiter)
                {
                    mlimiter_stop(limiter);
                    limiter=NULL;
                }
                our_resource = NULL;
                page = NULL;
                plane = NULL;


                ret = SYMsgSend( SJMID, &new_msg );
                ASSERT( OK==ret );
                break;
            }
            case MSG_CANCELJOB: 
            {
                DBG_PRINTF_NOTICE("MSG_CANCELJOB\n");
                if (limiter)
                {
                    mlimiter_stop(limiter);
                    limiter=NULL;
                }
                // ASSERT( NULL != our_resource );
                // ACK the system job mgr
                new_msg.msgType = MSG_CANCELJOB;
                new_msg.param1 = SYS_ACK;
                new_msg.param2 = GENPIP_ID;
                new_msg.param3 = msg.param3;
                ret = SYMsgSend( SJMID, &new_msg);
                ASSERT( OK == ret );

                // free resources
                new_msg.msgType = MSG_FREERECIPE;
                new_msg.param1 = 0;
                new_msg.param2 = 0;
                new_msg.param3 = our_resource;

                if (page != NULL)
                {
		    from_page_close( page_adaptor );
                }
                our_resource = NULL;
                page = NULL;
                plane = NULL;


                ret = SYMsgSend( SJMID, &new_msg );
                ASSERT( OK==ret );
                break;
            }
            case MSG_JOBABORT:
            {
                DBG_PRINTF_ERR("Unexpected message JOB_ABORT\n");
                ASSERT(0); // we should never see this message!

                break;
            }
            default:
            {
                DBG_PRINTF_ERR("Unknown message %d\n", msg.msgType);
                ASSERT(0);  // we need to identify all of these messages
                break;
            }
        }
    }
    return 0;
}
