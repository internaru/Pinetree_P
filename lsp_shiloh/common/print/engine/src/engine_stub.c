/*
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file engine_stub.c
 *
 * \brief This file contains a Print Engine stub.
 *
 * This print engine stub always presents itself as being online and will
 * immediately acknowledge all requests as if the pages were being printed.
 *
 */

#include "sys_init_api.h"
#include "posix_ostools.h"
#include "lassert.h"
#include "logger.h"
#include "agMessage.h"
#include "print_status_codes.h"
#include "print_job_types.h"
#if defined(VIDEO_APP)
#include "video_laser_api.h"
#endif
#include "debug.h"
#include "dbg_pnm.h"
#include "cmd_proc_api.h"

#define DBG_PRFX "ENG_STUB: "
#define LOGGER_SUBMOD LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR DBG_PRINTF_ERR
#define MSG DBG_PRINTF_DEBUG
#define SLOG_ERR DBG_MEMLOG_ERR
//#define SLOG_ERR DBG_PRINTF_ERR


#define NUM_CREDITS 2

#define PRINT_ENGINE_STACK_SIZE POSIX_MIN_STACK_SIZE

// #define HAVE_ENGINE_PAPERLESS_MODE 1
#ifdef HAVE_ENGINE_PAPERLESS_MODE

// debug logging tool 
#if 1
#define slogit(e) e,  logStringImpl("%s\n", #e )
#else
#define slogit(e) e
#endif

#define MAX_NUM_PAGE_TOKENS 20

const uint32_t engine_state_cookie_val = 0x14567fd1;

typedef struct page_handle_s page_handle_t;

typedef struct engine_state_s
{
    uint32_t cookie;
    bool in_job;
    page_handle_t *page_exiting;   ///< not null from bottom till safe in output bin
    page_handle_t *active_page;    ///< not null from page_recv till bottom
    page_handle_t *send_msg_page;  ///< temp incoming page 
    page_handle_t *video_done;  ///< video is done page 
    uint32_t page_queue[MAX_NUM_PAGE_TOKENS]; ///< page msg queue 
    
} engine_state_t;

static engine_state_t engine_state;

static mqd_t page_mq; 
static pthread_t mech_thread_id;
static ALIGN8 UINT8 mech_stack[PRINT_ENGINE_STACK_SIZE];

void* engine_ctrl_loop(void* unused);

#endif //ifdef HAVE_ENGINE_PAPERLESS_MODE

// Stack
static ALIGN8 char print_engine_stack[PRINT_ENGINE_STACK_SIZE];

// Message queue
static mqd_t print_engine_queue;

static pthread_t print_engine_thread_id;

#define NUM_PRINT_ENGINE_MESSAGES 4 

// Local function prototypes
static void *engine_thread(void *unused);

static bool force_paper_out = false;

int dbg_paperout_cb( int argc, char *argv[] )
{						
    force_paper_out = !force_paper_out;
    return 0;
}

void engine_dbg_init()
{
#ifdef HAVE_CMD
#ifdef DEBUG
    int i_res;
    i_res = cmd_register( "eng",
                          NULL,
                          NULL,
                          NULL,
                          NULL );
    XASSERT( i_res == 0, i_res );

    i_res = cmd_register_subcmd( "eng",
				 "dumpstrips",
				 "dump strip data to supplied path/file name",
				 NULL,
				 NULL,
				 dbg_dump_stripdata_filename_cb );
    i_res = cmd_register_subcmd( "eng",
				 "dumpgray",
				 "dump gray strip data to supplied path/file name",
				 NULL,
				 NULL,
				 dbg_dump_gray_filename_cb );
    i_res = cmd_register_subcmd( "eng",
				 "dumpcolor",
				 "dump color strip data to supplied path/file name",
				 NULL,
				 NULL,
				 dbg_dump_color_filename_cb );
    i_res = cmd_register_subcmd( "eng",
				 "dumpmono",
				 "dump mono strip data to supplied path/file name",
				 NULL,
				 NULL,
				 dbg_dump_mono_filename_cb );
    i_res = cmd_register_subcmd( "eng",
				 "paperout",
				 "force paperout on engine",
				 NULL,
				 NULL,
				 dbg_paperout_cb );
    XASSERT( i_res == 0, i_res );
#endif
#endif
}



void engine_init( void )
{
#ifdef HAVE_ENGINE_PAPERLESS_MODE
    engine_state.cookie = engine_state_cookie_val;
    engine_state.in_job = false;
    engine_state.page_exiting = 0;
    engine_state.active_page = 0;
    engine_state.video_done = 0;

    posix_create_message_queue(&page_mq, "/print_engine_page_Q", NUM_PRINT_ENGINE_MESSAGES, sizeof(void*));
    posix_create_thread(&mech_thread_id, engine_ctrl_loop, 0, "eng_ctrl_task", mech_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);
#endif    

    // Create and register a message queue 
    posix_create_message_queue(&print_engine_queue, "/print_engine_Q", NUM_PRINT_ENGINE_MESSAGES, sizeof(MESSAGE));
    router_register_queue(ENGINEID, print_engine_queue);

    engine_dbg_init();
    
    //Create the Video Manager Thread
    posix_create_thread(&print_engine_thread_id, engine_thread, 0, "tVideoMgr",
                        print_engine_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_NORMAL);


    MSG("Engine: Initialized\n");
}

void engine_shutdown( void )
{
	/* Nothing yet -- This is just a stub */
    MSG("Engine: Shutting down\n");
}


#ifdef HAVE_ENGINE_PAPERLESS_MODE
static uint32_t EngError = 0;
void set_engine_error(uint32_t error)
{
    EngError = error;
}

static page_handle_t *queue_page_for_print(page_handle_t* page)
{
    MSG("queue page  %p \n", page);

    engine_state.send_msg_page = page;    

    if(0 != posix_message_send(page_mq, (void*)&engine_state.send_msg_page, sizeof(void*), MQ_DEFAULT_PRIORITY, 0))
    {
        MSG(" failed to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
        SLOG_ERR(" failed to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
        return 0;
    }
    ERR(" success to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
    SLOG_ERR(" success to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
    return page;
}

static void engine_cancel_page_msg( MESSAGE *message )
{
    // revisit
    void *cleanup = 0;

    if (engine_state.active_page) 
    {
        // this is an inflight page! it is up to the video to do its thing
        // we will continue moving the paper.
        SLOG_ERR("canceling active page %p\n", engine_state.active_page);
        ERR("canceling active page %p\n", engine_state.active_page);
    }

    while(0 == posix_wait_for_message(page_mq, (char*)&cleanup, sizeof(void*), 0))
    {
        SLOG_ERR("canceling page %p\n", cleanup);
        ERR("canceling page %p\n", cleanup);
    }
}


void engine_state_set_page_bottom()
{
    ASSERT( engine_state.active_page && !engine_state.page_exiting);

//    MESSAGE message;
    bool dbg_out_done = false;
    
    if (engine_state.video_done == 0)
    {
        // video is still running we have to cancel fault the page.
        if(!dbg_out_done)
        {
            dbg_out_done = true;
            
            SLOG_ERR("video page is longer than physical paper!\n");   
            ERR("video page is longer than physical paper! %p %p\n", 
                engine_state.active_page, engine_state.video_done );   
        }
    }

    // tell print manager the page is done.
//    message.msgType = MSG_PAGEVIDEODONE;
//    message.param3 = engine_state.active_page;

    engine_state.page_exiting = engine_state.active_page; 
    engine_state.active_page = 0;
    engine_state.video_done = 0;
    
//    SYMsgSend(PRINTMGRID, &message);  // try no wait or thread priority to fix 1.7sec delay here.         
}

void engine_state_set_page_safe()
{
    MESSAGE message;
    ASSERT( engine_state.cookie == engine_state_cookie_val );

    // tell print manager the page is safe in the output bin.
    message.msgType = MSG_PAGEDONE;
    message.param3 = engine_state.page_exiting;

    engine_state.page_exiting = 0;

    SYMsgSend(PRINTMGRID, &message);
}

static void engine_pagedone_callback(PAGE_DATA *page, error_type_t page_status )
{
    engine_state.video_done = (page_handle_t*)page; // really didn't want to know about PAGE_DATA pointers...  revisit making this an abstract type
    SLOG_ERR("video done callback %p\n", page);
    ERR("video done callback %p status %d\n", page, page_status);
    
    engine_state_set_page_bottom();
    if ( page_status == OK )
    {
	engine_state_set_page_safe();
    }
}

// Private function definitions

/// wait for job from above to give us a hint for when to start 
///
uint32_t block_page_avail( uint32_t timeout_msec )
{
    if (engine_state.in_job)
        return 100;
    else
    {
        posix_sleep_ms(timeout_msec);

        if (engine_state.in_job)
            return 100;
    }
    return 0;
}


bool engine_print_page()
{
    bool page_printed = false;

    SLOG_ERR("==> %s page %#x\n", __func__, engine_state.active_page);

    if (engine_state.active_page)
    {
	video_print_page((PAGE_DATA*)engine_state.active_page);


    //engine_state_set_page_bottom();
    //engine_state_set_page_safe();

	page_printed = true;
    }
    return page_printed;
}

/// wait for page from above to be ready to print.
///
bool printer_get_page(uint32_t ticks)
{
    if (engine_state.active_page) 
    {
         MSG("active page error \n");
	 return false; // true;
    }

    if(0 == posix_wait_for_message(page_mq, (void*)&engine_state.active_page, sizeof(void*), ticks*(MILLISEC_PER_TICK*USEC_PER_MILLISEC)))
    {  
        ERR ("rec page  %p \n", engine_state.active_page);
        SLOG_ERR("rec page  %p \n", engine_state.active_page);
        return true;
    }
    
    ERR ("rec page  null \n");
    SLOG_ERR("rec page  null \n");
    return false;
}

void* engine_ctrl_loop(void* unused)
{  
    bool page = false;

    int timeout_10msecs = 10;
    uint32_t page_avail_in_x_msec = 0;

    SysWaitForInit();

    MSG("mech thread init\n");

    video_register_page_done_callback(&engine_pagedone_callback);

    while (1)
    {
        // block here doing nothing until we have a page

        page_avail_in_x_msec = block_page_avail( timeout_10msecs );
        if ( page_avail_in_x_msec == 0 )
        {
            // block waiting for page wanting to be printed, 
            // unblock is just to test for paper added.
            // not in a job so lets cool down.
            
            continue; // loop waiting for jobs
        }
        
        if (!page)
            slogit( page = printer_get_page(400) );  // 4 seconds is rather long!
                                                                       
        if (!page)
        {
            ERR("%s: Page not found; wait for a page again\n", __func__);
            continue;
        }

        //print pages
        if(!engine_print_page())
        {

            break;
        }

        slogit(    engine_state.in_job = false);

    } // end of while (1)
    
    return 0;
}

#endif //#ifdef HAVE_ENGINE_PAPERLESS_MODE

static void *engine_thread(void *unused)
{
    MESSAGE message;

    // Wait for system initialization to complete
    SysWaitForInit();
    
    while(1)
    {
        mq_receive(print_engine_queue, (char*)&message, sizeof(MESSAGE), 0);
	DBG_PRINTF_DEBUG(" message '%s'=0x%X=%d; param1=0x%X param2=0x%X param3=0x%08X\n",
			 DecodeSysMsgEnum(message.msgType), message.msgType, message.msgType,
			 message.param1, message.param2, message.param3);
	switch(message.msgType)
	{
	case MSG_JOBSTART:
	{
#ifdef HAVE_ENGINE_PAPERLESS_MODE
	    slogit(   engine_state.in_job = true );
	    video_set_paperless_mode( true );
#endif                    
	    MSG("Engine: Job start\n");
	    break;
	}
	case MSG_WAKEUP:
	{
	    message.msgType = MSG_STATUSUPDATE;
	    message.param1 = STATUS_ONLINE;
	    SYMsgSend(PRINTMGRID, &message);
	    MSG("Engine: Wake up\n");
	    break;
	}
	case MSG_STATUSREQUEST:
	{
	    message.msgType = MSG_STATUSUPDATE;
	    message.param1 = force_paper_out ? STATUS_PAPER_OUT_ALL : STATUS_ONLINE;
	    MSG("Engine: Status request 0x%08x force %d \n", message.param1, force_paper_out);
	    SYMsgSend(PRINTMGRID, &message);
	    break;
	}
	case MSG_ERRCLEAR:
	{
	    message.msgType = MSG_CREDITUPDATE;
	    message.param1 = force_paper_out ? 0 : NUM_CREDITS;
	    MSG("Engine: Error clear 0x08x \n", message.param1);
	    SYMsgSend(PRINTMGRID, &message);

	    break;
	}
	
	case MSG_PAGESTART:
	{
#ifdef HAVE_ENGINE_PAPERLESS_MODE
	    slogit(   queue_page_for_print( message.param3 ));
            
	    // revisit: set / clear interface
	    if(EngError)
	    {
		message.msgType = MSG_STATUSUPDATE;
		message.param1 = EngError;
		EngError = 0;
		SYMsgSend(PRINTMGRID, &message);
		break;  // done here.
	    }
#else
	    {
		PAGE_DATA *current_page = (PAGE_DATA *)message.param3;;
		PLANED* plane;
		int color, stripcnt;
		ATLISTENTRY* node;
		static int pagecnt = 0;
		PAGE_DATA *inpage = current_page;
		
		// Need to decrement UseCount so that the memory
		// for this page will be freed
		
		ASSERT( current_page && current_page->sheet );
		MSG( " sheet ready %d\n", current_page->sheet->sheetReady );
		ERR( " stub paper %d tray %d mediatype %d\n", 
		     current_page->PAPER, 
		     current_page->DEFAULTSOURCE,
		     current_page->mediaType );
		
		if ( current_page->sheet->sheetReady ) { // then delete sheet else wait for complete sheet before printing
		    if (current_page->sheet->BackPage) {
			pagecnt++;
			current_page = inpage->sheet->BackPage;
			for (color = 0; color < NUM_MARKING_COLOR_CHANNELS; color++) {
			    for ( stripcnt = 0, node = MListHead(&current_page->planeDataPendingPrintList[color]) ;
				  NULL != node ;
				  stripcnt++, node = MListNext(&current_page->planeDataPendingPrintList[color], node)) {
					plane = CONTAINING_RECORD(node, PLANED, listNode);
					plane->UseCount--;
					MSG( " page %d color %d plane %d page %p front %p back %p duplex %x\n",
					     pagecnt, color, stripcnt, 
					     current_page, current_page->sheet->FrontPage, current_page->sheet->BackPage, current_page->DUPLEX );
					dbg_dump_stripdata_to_pbm_file( pagecnt, color, plane, stripcnt );
			    }
			}
		    }
		    current_page = inpage->sheet->FrontPage;
		    if (current_page) { // have front page.
			pagecnt++;
			for (color = 0; color < NUM_MARKING_COLOR_CHANNELS; color++) {
			    for ( stripcnt = 0, node = MListHead(&current_page->planeDataPendingPrintList[color]) ;
				  NULL != node ;
				  stripcnt++, node = MListNext(&current_page->planeDataPendingPrintList[color], node)) {
				plane = CONTAINING_RECORD(node, PLANED, listNode);
				plane->UseCount--;
				MSG( " page %d color %d plane %d page %p front %p back %p duplex %x \n",
				     pagecnt, color, stripcnt, 
				     current_page, current_page->sheet->FrontPage, current_page->sheet->BackPage, current_page->DUPLEX );
				dbg_dump_stripdata_to_pbm_file( pagecnt, color, plane, stripcnt );
			    }
			}
		    }
		    posix_sleep_ms( 1000 ); // 1 page(s) a second 
		    //sleep(1); // fake a slower than infinite speed engine
		    message.msgType = MSG_PAGEDONE;  // NOTE this is a sheet done sent when the physical paper is safe in the output bin, 
		    SYMsgSend(PRINTMGRID, &message); // sent once per sheet not once per side.
		}
	    }
#endif
	    
	    MSG("Engine: Page start\n");
	    break;
	}
	case MSG_CANCELJOB:
	    MSG("Engine: Cancel job %d \n", message.msgType);
	    message.msgType = MSG_CANCELJOB;
	    message.param1 = LOCAL_ACK;
	    SYMsgSend(PRINTMGRID, &message);
	    message.msgType = MSG_STATUSUPDATE;
	    message.param1 = STATUS_ONLINE;
	    SYMsgSend(PRINTMGRID, &message);
	    message.msgType = MSG_CREDITUPDATE;
	    message.param1 = force_paper_out ? 0 : NUM_CREDITS;
	    SYMsgSend(PRINTMGRID, &message);

	    MSG("Engine: Error clear 0x08x \n", message.param1);
	    MSG("Engine: Cancel job %d \n");
	    break;
	case MSG_CANCELPAGE:
	{
#ifdef HAVE_ENGINE_PAPERLESS_MODE
	    slogit( engine_cancel_page_msg(&message) );
#endif
	    message.msgType = MSG_CANCELPAGE;
	    message.param1 = LOCAL_ACK;
	    SYMsgSend(PRINTMGRID, &message);
	    MSG("Engine: Cancel page\n");
	    break;
	}
	case MSG_JOBEND:
	{
#ifdef HAVE_ENGINE_PAPERLESS_MODE
	    slogit(   engine_state.in_job = false );
#endif
	    MSG("Engine: Job end\n");
	    break;
	}
	default:
	{
	    ERR("Engine: Message %d received\n", message.msgType);
	}
	}
    }
    return 0;
}

uint32_t engine_get_top_margin_in_lines( PAGE_DATA *page, COLOR process_color )
{
    // mono engine so no modification of margin is needed.
    return 100;
}



uint32_t engine_get_side_margin_in_subpixels( PAGE_DATA *page,
                                              uint32_t channel_number,
                                              uint32_t image_width_in_pixels,
                                              resolution_t page_x_resolution )
{
	return 100;
}
