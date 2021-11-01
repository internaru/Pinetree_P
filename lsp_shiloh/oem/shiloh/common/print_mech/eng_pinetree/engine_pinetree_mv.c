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

 
#include <stdbool.h>
#include "memAPI.h"
#include "delay_api.h"
#include "logger.h"
#include "debug.h"
#include "utils.h"
#include "print_status_codes.h"
#include "engine_config.h"
#include "laser_engine_config_api.h"
#if defined(VIDEO_APP)
#include "video_laser_api.h"
#endif
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#include "pthread.h"
#include "posix_ostools.h"

#include "gpio_api.h"
#include "dcmotor_ol.h" // dcmotor_openloop.h
#include "pwm_api.h"
#include "dec_sensor.h"
#include "dec_fuser.h"
#include "dec_laser.h"
#include "dec_adc_api.h"

#include "sys_init_api.h"
#include "lassert.h"
#include "agMessage.h"

#include "print_job_types.h"

#define DBG_PRFX "ENG: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING


// debug logging tool 
#if 1
#define slogit(e) e,  logStringImpl("%s\n", #e )
// #define slogit(e) e,  dbg_printf("%s\n", #e )
#else
#define slogit(e) e
#endif


#define NUM_CREDITS 3

// ThreadX thread object
static pthread_t print_engine_thread_id;
static pthread_t mech_thread_id;

// Stack
#define PRINT_ENGINE_STACK_SIZE POSIX_MIN_STACK_SIZE
static ALIGN8 UINT8 print_engine_stack[PRINT_ENGINE_STACK_SIZE];
static ALIGN8 UINT8 mech_stack[PRINT_ENGINE_STACK_SIZE];

// Message queue
mqd_t print_engine_mq;
#define NUM_PRINT_ENGINE_MESSAGES 4 
bool bottom_done = true;
#define MAX_NUM_PAGE_TOKENS 20
static mqd_t page_mq; 

typedef struct page_handle_s page_handle_t;

const uint32_t engine_state_cookie_val = 0x14567fd1;

// FAN DUTY CYCLES
#define MAIN_FAN_LOW  5
#define MAIN_FAN_HALF 50
#define MAIN_FAN_FULL 100

// engine timing
 // 618 + extra
#define PAPER_ENTRY_TOP_TIMEOUT 2000
#define PAPER_ENTRY_BOTTOM_TIMEOUT 500
#define PAPER_FEED_TOP_TIMEOUT 300
#define PAPER_FEED_BOTTOM_TIMEOUT 300
#define PAPER_EXIT_TOP_TIMEOUT 600
#define PAPER_EXIT_BOTTOM_TIMEOUT 600
#define PAPER_MOTOR_FINISH_TIMEOUT 108

/** High voltage defines; note that these are duplicated from pwm_config.c
  * This is BAD.  Need to develop common header file and improve API. */
    // The Transfer PWM has a Print duty cycle of 70.1% (180 of 255).
    // The Transfer PWM has a IP duty cycle of 24.1% (69 of 255).
    // The Transfer PWM has a clean duty cycle of 0% (0 of 255).
    // The Charge PWM has a duty cycle of 81.6% (208 of 255).
    // The Developer Print PWM has a duty cycle of 62.4% (159 of 255).
    // The Developer No-Print PWM has a duty cycle of 7.8% (20 of 255)

    #define TRANSFER_PRINT_DUTY_CYCLE 70
    #define TRANSFER_IP_DUTY_CYCLE 24
    #define TRANSFER_CLEAN_DUTY_CYCLE 0

    #define DEVELOPER_PRINT_DUTY_CYCLE 62
    #define DEVELOPER_NO_PRINT_DUTY_CYCLE 8  


typedef struct engine_state_s
{
    uint32_t cookie;
    bool paper_out;
    bool in_job;
    page_handle_t *pick_page;      ///< next page to be picked.
    page_handle_t *picked_page;      ///< page just picked.
    page_handle_t *active_page;    ///< not null from page_recv till bottom
    page_handle_t *page_exiting;   ///< not null from bottom till safe in output bin

    page_handle_t *send_msg_page;  ///< temp incoming page
    page_handle_t *video_done;  ///< video is done page 
    uint32_t page_queue[MAX_NUM_PAGE_TOKENS]; ///< page msg queue 
    uint32_t pm_statusrequest; ///< print_status_codes.h 
    uint32_t top_to_bottom_delay_msecs;
    uint32_t bottom_to_safe_delay_msecs;
    delay_msec_timer_t *page_safe_timer;
    bool input_jam;
    bool exit_jam;
    int engine_fatal_error;

  
} engine_state_t;

/// todo: engine error codes need to be in print_status_codes.h
#define ENGINE_FATAL_LASER_FAIL 12 | STATUS_ERROR
#define ENGINE_FATAL_PAPER_MOTOR_FAIL 13 | STATUS_ERROR


static engine_state_t engine_state;

typedef struct engine_hardware_s
{
    dec_sensor_t *paper_avail;  
    dec_sensor_t *paper_avail_mpt;
    dec_sensor_t *paper_entry;
    dec_sensor_t *paper_exit;
  // dec_sensor_t *door_open;  // access to jam clear 
    gpio_handle_t *pick_pin;        // paper pick clutch
    dec_fuser_t *fuser;
    dcmotor_ol_t* paper_motor;
    dcmotor_speed_t *speed_off;
    dcmotor_speed_t *speed_full;
    pwm_t *main_fan;

    gpio_handle_t *thv_en;      ///< transfer voltage enable
    pwm_t *transfer_voltage;    ///< (toner to paper)
    pwm_t *developer_voltage;   ///< (toner to drum)
    pwm_t *charging_voltage;    ///< (drum charge)
 
    dec_laser_t *laser;

    gpio_handle_t *pin_24v_power; 
}engine_hardware_t;

static engine_hardware_t *eng_hardware;

static page_handle_t *queue_page_for_print(page_handle_t* page);
static void engine_cancel_page_msg( MESSAGE *msg );
void engine_dbg_init();
void dec_printer_feed_paper( bool feed );

void engine_state_init(void)
{
    engine_state.cookie = engine_state_cookie_val;
    engine_state.in_job = false;
    engine_state.paper_out = false;
    engine_state.pick_page = 0;
    engine_state.picked_page = 0;
    engine_state.active_page = 0;
    engine_state.page_exiting = 0;

    engine_state.video_done = 0;
    engine_state.pm_statusrequest = STATUS_ONLINE;

    // pick solenoid disengage timer = 250 ms
    // 
    engine_state.top_to_bottom_delay_msecs = 1000; // A4 was: 950; // letter.
    // letter + input to drum - interpage gap (11 * 25.4 + 45.5) / 230.6
    // engine_state.page_bottom_timer = 0;
    // revisit: tune me to be time from bottom to just past exit sensor
    engine_state.bottom_to_safe_delay_msecs = 1389;
    engine_state.page_safe_timer = 0; 
}


// Local function prototypes
/// message api thread talks to printmgr 
static void* engine_thread(void* unused);

bool engine_print_prepare(engine_hardware_t* eng_hw);
void engine_start_high_voltage(engine_hardware_t* eng_hw);
void engine_stop_high_voltage(engine_hardware_t* eng_hw);
void engine_print_stop(engine_hardware_t* eng_hw);

/// mech control thread
static void* engine_ctrl_loop(void* unused);

uint32_t engine_get_time_msec()
{
    return timer_get_time_usec() / 1000;
}

void engine_init( void )
{
    posix_create_message_queue(&print_engine_mq, "/PrintEngQ", NUM_PRINT_ENGINE_MESSAGES, sizeof(MESSAGE));
    router_register_queue(ENGINEID, print_engine_mq);

    posix_create_message_queue(&page_mq, "/PrintEngPgQ", NUM_PRINT_ENGINE_MESSAGES, sizeof(void*));

    posix_create_thread(&mech_thread_id, engine_ctrl_loop, 0, "eng_ctrl_task", mech_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);
    posix_create_thread(&print_engine_thread_id, engine_thread, 0, "eng_msg_task", print_engine_stack, PRINT_ENGINE_STACK_SIZE, POSIX_THR_PRI_ENGINE);

    MSG("Initialized\n");
}

void engine_shutdown( void )
{
	/* Nothing yet -- This is just a stub */
    DBG1("Engine: Shutting down\n");
}


bool engine_try_to_clear_errors( ) 
{
    MESSAGE message;
    bool clear;

    clear = engine_state.pm_statusrequest == STATUS_ONLINE ? true : false;
  
    message.msgType = MSG_STATUSUPDATE;
    message.msgType = STATUS_ONLINE;

    if (engine_state.exit_jam) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_OUTPUT; 
        SYMsgSend(PRINTMGRID, &message);
    }
    if (engine_state.paper_out) {
        engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_OUT_TRAY1; 
        SYMsgSend(PRINTMGRID, &message);
    }

    if ( message.msgType == STATUS_ONLINE ) {
        if ( !clear ) {
            SYMsgSend(PRINTMGRID, &message);
	}
    } else { 
        clear = false;
    }
    return 1; // clear; // false on error
}


/// video calls this
static void engine_pagedone_callback(PAGE_DATA *page, error_type_t page_status)
{
    engine_state.video_done = (page_handle_t*)page; // really didn't want to know about PAGE_DATA pointers...  revisit making this an abstract type
    SLOG_ERR("%s: got video done callback %p, status = %d\n", __func__, page, page_status);
    ERR("%s: got video done callback %p, status = %d\n", __func__, page, page_status);
}


// Private function definitions
static void* engine_thread(void* unused)
{
    MESSAGE message;

    // Wait for system initialization to complete
    SysWaitForInit();
    engine_state_init();   
    SLOG_ERR("engine thread entry\n");

    while(1)
    {
        
        mq_receive(print_engine_mq, (char*)&message, sizeof(MESSAGE), 0);

        switch(message.msgType)
        {

        case MSG_JOBSTART:
        {
            // No action required           

            //---JOB_DATA* jobPtr = (JOB_DATA*)msg->param3;

                //this means a page is on the way soon. Call product specific to
                //see if it needs to warm up or anything.
            //---EngStartJob(jobPtr->jobDataType);

            slogit(   engine_state.in_job = true );
            DBG1("Engine: MSG_JOBSTART\n");
            break;
        }

        case MSG_WAKEUP:
        {
            message.msgType = MSG_STATUSUPDATE;
            message.param1 = STATUS_ONLINE;
            SYMsgSend(PRINTMGRID, &message);
            DBG1("Engine: MSG_WAKEUP\n");
            break;
        }

        case MSG_STATUSREQUEST:
        {
            message.msgType = MSG_STATUSUPDATE;
            // message.param1 = STATUS_ONLINE or an error condition
            // use print_status_codes.h to report engine errors up
	    message.param1 = engine_state.pm_statusrequest;
            SYMsgSend(PRINTMGRID, &message);
            DBG1("Engine: MSG_STATUSREQUEST\n");
            break;
        }

        case MSG_ERRCLEAR:
        {
            message.msgType = MSG_CREDITUPDATE;	  

	    // check sensors perhaps the user has:
	    // added paper or cleared a jam or closed the door
	    // if all the static checks pass we then 

	    if ( engine_try_to_clear_errors( ) )
	        message.param1 = NUM_CREDITS;
	    else 
	        message.param1 = 0;
            SYMsgSend(PRINTMGRID, &message);

            DBG1("MSG_ERRCLEAR credits %d \n", message.param1 );
            break;
        }
        
        case MSG_PAGESTART:
        {
            if (0) // stub out engine by deleting the data. 
            {
                PAGE_DATA *current_page = (PAGE_DATA *)message.param3;
                PLANED* plane;
                int i;

                // decrement use count so data is freed
                for (i = 0; i < NUM_MARKING_COLOR_CHANNELS; i++)
                {
                    ATLISTENTRY* node = MListHead(&current_page->planeDataPendingPrintList[i]);
                    while(NULL != node)
                    {
                        plane = CONTAINING_RECORD(node, PLANED, listNode);
                        plane->UseCount--;
                        node = MListNext(&current_page->planeDataPendingPrintList[i], node);
                    }
                }
            }
            else
            {
                slogit(   queue_page_for_print( message.param3 ));
            }
            // sk: This shouldn't be here.
            // revisit: set / clear interface
            if(engine_state.pm_statusrequest != STATUS_ONLINE)
            {
                message.msgType = MSG_STATUSUPDATE;
                message.param1 = engine_state.pm_statusrequest;
                SYMsgSend(PRINTMGRID, &message);
                break;  // done here.
            }
            DBG1("Engine: Page start\n");
            break;
        }
	
        case MSG_CANCELPAGE:
        {
            engine_cancel_page_msg(&message);
            break;
        }
        
        case MSG_JOBEND:
        {
            slogit(   engine_state.in_job = true );
            // No response required
            DBG1("Engine: Job end\n");
            break;
        }
        
        default:
        {
            DBG1("Engine: Unhandled Message %d received \n", message.msgType);
        }
        } // end switch
    } // end while
    return 0;
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

/*
//engine is done canceling, now cancel video
sendMsg.msgType = MSG_CANCELPAGE;
sendMsg.param1 = LOCAL_START;
sendMsg.param2 = PRINTMGRID;
SYMsgSend(VIDEOMGRID, &sendMsg);
*/

    while(0 == posix_wait_for_message(page_mq, (char*)&cleanup, sizeof(void*), 0))
    {
        SLOG_ERR("canceling page %p\n", cleanup);
        ERR("canceling page %p\n", cleanup);
    }

    message->msgType = MSG_CANCELPAGE;
    message->param1 = LOCAL_ACK;
    SYMsgSend(PRINTMGRID, message);
    DBG1("Engine: MSG_CANCELPAGE\n");

}

/// who calls when and why?
/// *** FIRST PAGE OUT TIMING POINT ***
/// from :  MSG_TIME_TO_END,   
/// Param1 is the number of 10msec till the end of a page.estimate. /// Used by copy
/// This message really means "time to end of page data". When the time
/// ticks down to within the length of time required for engine pre-warmup,
/// issue the prewarmup command.
/// 
void EngPlatPrewarmupCmd(
    uint32_t ten_msecs_till_end_of_page
    )
{ 
    DBG1(" EngPlatPrewarmupCmd(%ld milliseconds)"
                " @ %ld usecs\n", 
                ten_msecs_till_end_of_page, 
                timer_get_time_usec());

    // convert to engine_warmup_signal if needed in usecs.
}


/// Cancel any pending engine print command.
void EngCancelJob(void)
{
    DBG1(" %s ", __FUNCTION__ );

}

static page_handle_t *queue_page_for_print(page_handle_t* page)
{
    MSG("queue page  %p \n", page);

    engine_state.send_msg_page = page;    

    if(0 != posix_message_send(page_mq, (void*)&engine_state.send_msg_page, sizeof(void*), MQ_DEFAULT_PRIORITY, 0))
    {
        ERR(" failed to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
        SLOG_ERR(" failed to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
        return 0;
    }
    ERR(" success to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
    SLOG_ERR(" success to send page to engine max of %d is supported\n", MAX_NUM_PAGE_TOKENS);
    return page;
}


/// wait for page from above to be ready to print.
///
bool printer_get_page(uint32_t ticks)
{
    if(0 == posix_wait_for_message(page_mq, (void*)&engine_state.pick_page, sizeof(void*), ticks*(MILLISEC_PER_TICK*USEC_PER_MILLISEC)))
    {  
        MSG("rec page  %p \n", engine_state.pick_page);
        SLOG_ERR("rec page  %p \n", engine_state.pick_page);
        return true;
    }
    MSG("rec page  null \n");
    SLOG_ERR("rec page  null \n");
    return false;
}

/// wait for job from above to give us a hint for when to start 
///
uint32_t block_page_avail( uint32_t timeout_msec )
{
    if (engine_state.in_job)
        return 100;
    else
    {
        delay_msec_timer(timeout_msec);

        if (engine_state.in_job)
            return 100;
    }
    return 0;
}


/// indicate the page is safe in the output bin, this is forwarded
/// on to the printmgr.
/// ONE_SHOT timer callback.
///
/// pinetree engine logic description:
/// Input sensor is used for page bottom this causes a timer to be fired after the 
/// paper should have passed the Exit sensor, we check the sensor here to determine if 
/// the paper has jammed or if it is sucessfull.
void engine_state_set_page_safe_cb(void *page_asvoid)
{
    MESSAGE message;
    engine_hardware_t *eng_hw = eng_hardware;


    // timer must fire after paper would have passed exit sensor.
    if (dec_sensor_poll( eng_hw->paper_exit ))  
    {
      ERR("paper exit jam @ %d\n", engine_get_time_msec());   
	goto fakeit;
        dec_printer_feed_paper(false);
	message.msgType = MSG_STATUSUPDATE;
	slogit( message.param1 = STATUS_PAPER_JAM_OUTPUT ); 
        message.param3 = page_asvoid; //engine_state.page_exiting; 
	engine_state.exit_jam = true;
        SLOG_ERR("EXIT jam \n");
    } 
    else 
    {
    fakeit:
        // tell print manager the page is safe in the output bin.
        slogit( message.msgType = MSG_PAGEDELIVERYDONE );
	message.param3 = page_asvoid; // engine_state.page_exiting;
        // engine_state.page_exiting = 0;
    }
    // engine_state.page_safe_timer = 0;  // only allow 1 page between BOTTOM and EXIT ie 1 between laser and output tray.
    SYMsgSend(PRINTMGRID, &message);
}

/// bottom is a sensor that needs a time delay in front of it so that the video actually completes 
/// before the video done message arrives in print/job mgr above us.
/// a timer is started that will send a page is safe in output bin message if no jams occur.
/// tbd: have video tell engine when it is video done then engine can forward this on.
///
void engine_state_set_page_bottom_cb(void *page_asvoid)
{
    engine_hardware_t *eng_hw = eng_hardware;
    MESSAGE message;

    SLOG_ERR("_cb BOTTOM for page %p \n", page_asvoid);
    if (1 || engine_state.picked_page == 0 ) {
        slogit( dec_laser_set_top_sync( eng_hw->laser, false /* disable hvsync */));
	slogit( dec_enable_vdo_top_sensor( eng_hw->laser, eng_hw->paper_entry, false )); // false for off
	slogit( dec_enable_vdo( eng_hw->laser, false ));
        slogit( pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_IP_DUTY_CYCLE) );
    }

    if (engine_state.video_done == 0 )
    {
        // video is still running we have to cancel fault the page.
      SLOG_ERR("_cb video page is longer than physical paper!\n");   
      ERR("video page is longer than physical paper! %p %p\n", 
	  engine_state.active_page, engine_state.video_done );   
    }

    // tell print manager the page is done.
    message.msgType = MSG_PAGEVIDEODONE;
    message.param3 = page_asvoid; // engine_state.active_page;

    engine_state.page_exiting = page_asvoid; // engine_state.active_page; 
    engine_state.active_page = 0;
    engine_state.video_done = 0;
    
    delay_msec_timer_non_blocking(engine_state.bottom_to_safe_delay_msecs,
				  engine_state_set_page_safe_cb,
				  page_asvoid,
				  false /* non-repeating */
				  );
    // SLOG_ERR("page is video done pre msg\n");
    SYMsgSend(PRINTMGRID, &message);  // try no wait or thread priority to fix 1.7sec delay here.         
    // SLOG_ERR("page is video done post msg\n");
}

void engine_state_set_page_at_bottom_cb( void *page_as_void )
{
    engine_hardware_t *eng_hw = eng_hardware; 

    if (false == dec_sensor_poll( eng_hw->paper_entry )) 
    {
        SLOG_ERR("_cb paper feed jam %s %p \n", __FUNCTION__, page_as_void );   ERR("paper feed jam \n");   
        //dec_printer_feed_paper(false); 
    }
    // else
    {
	/// msecs between sensor and laser
        delay_msec_timer_non_blocking( 154, engine_state_set_page_bottom_cb, page_as_void, false );
    }
}


void printer_set_paper_exit_jam()
{
    ERR("paper exit jam \n"); 
    // send message with engine_state.active_page is jammed
    // send message 

    //gpio_led_on(gpio_led_by_index(1));
    //gpio_led_on(gpio_led_by_index(2));
    ERR("paper jam\n");    
}

void engine_state_set_paperout( bool out_of_paper )
{
    if (engine_state.paper_out != out_of_paper) // changed
    {
        engine_state.paper_out = out_of_paper;
	engine_try_to_clear_errors();
    }
}


/// disable pick solenoid
void pick_off_cb( void *cb_data )
{
    gpio_handle_t *pick_pin = (gpio_handle_t*)cb_data;
    
    DBG_ASSERT(pick_pin);
    SLOG_ERR( "_cb disengage the pick solenoid.\n" );
    REL_ASSERT( OK == gpio_write( pick_pin, 0 ) );
}

/// shiloh: main tray pick only.
/// pick paper solenoid starts picking, and auto disables.
void dec_printer_pick_paper( )
{
    REL_ASSERT( OK == gpio_write( eng_hardware->pick_pin, 1 ) );
    delay_msec_timer_non_blocking(
        250, /// msecs to leave solenoid engaged then timer will disengage the solenoid.
        pick_off_cb, eng_hardware->pick_pin, false );
}

/// activate/deactivate paper feed clutch (REGI_CLUTCH)
void dec_printer_feed_paper(bool feed)
{
#ifdef TODO
    REL_ASSERT( OK == gpio_write( eng_hardware->regi_clutch_pin, feed ? 1 : 0) );
#endif
}

/// activate/deactivate THV_EN (transfer voltage)
void dec_printer_enable_thv(bool enable)
{
    REL_ASSERT( OK == gpio_write( eng_hardware->thv_en, enable ? 1 : 0) );
}

void engine_atassert( void )
{
    // fuser and stepper have atassert
    // pwm can't have an atassert "off" varies
    pwm_off(eng_hardware->transfer_voltage);  
    dec_printer_enable_thv(false);
    pwm_off(eng_hardware->charging_voltage);
    pwm_off(eng_hardware->developer_voltage);
    dec_stop_laser_mirror(eng_hardware->laser);
    pwm_off(eng_hardware->main_fan);
}

void engine_init_hw(engine_hardware_t* eng_hw)
{
    eng_hw->paper_avail = dec_sensor_by_name("paper_main_tray_sensor");
    eng_hw->paper_avail_mpt = dec_sensor_by_name("paper_mpt_tray_sensor");

    eng_hw->paper_entry = dec_sensor_by_name("paper_entry_sensor"); 
    eng_hw->paper_exit = dec_sensor_by_name("paper_exit_sensor");

    REL_ASSERT( 
        OK == 
        gpio_open( &eng_hw->pick_pin, GPIO_BANK_PICK, GPIO_PIN_PICK, 
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 ) 
        );  

    eng_hw->fuser = dec_fuser_by_name("main_fuser");
    eng_hw->main_fan = pwm_by_name("main_fan");

    eng_hw->paper_motor = dcmotor_by_name("dcmotor_paper_motor");
    eng_hw->speed_off = dcmotor_speed_by_name("speed_off");
    eng_hw->speed_full = dcmotor_speed_by_name("speed_full");

    eng_hw->laser = dec_laser_by_index(0);


    eng_hw->transfer_voltage = pwm_by_name("pwm_transfer");
    eng_hw->charging_voltage = pwm_by_name("pwm_charge");
    eng_hw->developer_voltage = pwm_by_name("pwm_developer");

    REL_ASSERT( 
        OK == 
        gpio_open( &eng_hw->thv_en, GPIO_BANK_THV_EN, GPIO_PIN_THV_EN, 
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_LOW, 0 ) 
        ); 

/*
    LSPTODO: Shiloh hardware will have this soon.
    REL_ASSERT( 
        OK == 
        gpio_open( &eng_hw->pin_24v_power, GPIO_BANK_G, 1, 
                   GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_LOW, 0 ) 
        );  
    
*/ 
}


/**
 * \brief Get the engine ready to print.  Ramp motors, fans, 
 *        fuser, etc.
 *
 * \param eng_hw - engine hardware instance
 *
 * \return bool - true if ramp successful; false else
 */
bool engine_print_prepare(engine_hardware_t* eng_hw)
{
    uint32_t err;
    uint32_t motor_ramp_delay;

    MSG("==> %s\n", __func__);

    slogit(    pwm_run_dutycycle_percent(eng_hw->main_fan, MAIN_FAN_HALF) );
    slogit(    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(2) )); // hot

    // TODO: Need timeout and error return to handle fuser that won't heat.
    slogit(    dec_fuser_block_till_at_temp( eng_hw->fuser )); 

    motor_ramp_delay = dcmotor_speed_msecs_to_ramp(eng_hw->paper_motor, eng_hw->speed_full);
    motor_ramp_delay = MAX(motor_ramp_delay, 1750); // 1750 is the measured delay from the real unit.
                                                    // not sure if this is needed, but for the time being
                                                    // we will wait the max of the measured vs estimated ramp time



    slogit(    pwm_run_dutycycle_percent(eng_hw->main_fan, MAIN_FAN_FULL) );
    slogit(    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(3) ) ); // full heat
    slogit(    dec_fuser_block_till_at_temp( eng_hw->fuser )); // fuser fail timeout.

    slogit(    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full ));  // motor should be running during laser ramp and high voltage startup
    slogit(    dec_laser_start( eng_hw->laser ));
    slogit(    dec_laser_set_top_sync( eng_hw->laser, false ));
    engine_start_high_voltage(eng_hw);

    slogit(     err = dec_laser_wait( eng_hw->laser ) );
    if ( err )
    {
        engine_state.engine_fatal_error = ENGINE_FATAL_LASER_FAIL;
        ERR(" laser motor has failed\n");
        goto EXIT;
    }
    slogit(     err = dcmotor_wait( eng_hw->paper_motor, motor_ramp_delay )); 
    if ( err ) 
    {
	    // engine_state.engine_fatal_error = ENGINE_FATAL_PAPER_MOTOR_FAIL;
        ERR("paper motor watchdog, ramp fail\n");
        // goto EXIT;
    }

    // after motor at speed run IP voltage for 260ms
    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_IP_DUTY_CYCLE);
    delay_msec_timer(260); // or assume the fuser is slower than the motor 


    return true;
EXIT:
    MSG("<== %s = %d\n", __func__, engine_state.engine_fatal_error);
    // fatal engine error 
    engine_print_stop(eng_hw);
    return false;  // error
}

/**
 * \brief Stop engine.  Turn off motors, fans, fuser, high voltage, etc.
 */
void engine_print_stop(engine_hardware_t* eng_hw)
{
    MSG("==> %s\n", __func__);

    engine_stop_high_voltage(eng_hw);
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off
    dec_stop_laser_mirror(eng_hw->laser);
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off);
    pwm_off(eng_hw->main_fan);
    MSG("<== %s\n", __func__);
}

void engine_power_enable(engine_hardware_t* eng_hw, bool enable)
{
  /// lsptodo: 
  ///  REL_ASSERT( OK == gpio_write( eng_hardware->pin_24v_power, enable?1:0 ) );
}

/**
 * \brief High voltage startup sequence.
 */
void engine_start_high_voltage(engine_hardware_t* eng_hw)
{
    SLOG_ERR("==> %s\n", __func__);


    dec_printer_enable_thv(true);
    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_CLEAN_DUTY_CYCLE);
    pwm_run_dutycycle_percent(eng_hw->developer_voltage, DEVELOPER_NO_PRINT_DUTY_CYCLE);

    // switch from no-print to print duty cycle after 170 ms and start charge
    posix_sleep_ms(170);
    pwm_run_dutycycle_percent(eng_hw->developer_voltage, DEVELOPER_PRINT_DUTY_CYCLE);
    pwm_on(eng_hw->charging_voltage);

    SLOG_ERR("<== %s\n", __func__);
}

void engine_stop_high_voltage(engine_hardware_t* eng_hw)
{
    SLOG_ERR("==> %s\n", __func__);
    pwm_off(eng_hw->developer_voltage);
    pwm_off(eng_hw->transfer_voltage);
    pwm_off(eng_hw->charging_voltage);
    dec_printer_enable_thv(false);
    SLOG_ERR("<== %s\n", __func__);
}

void engine_ramp_down(engine_hardware_t* eng_hw)
{
    SLOG_ERR("==> %s\n", __func__);

    // cleaning voltage for 790ms
    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_CLEAN_DUTY_CYCLE);
    posix_sleep_ms(790);

    // ip voltage for 200ms
    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_IP_DUTY_CYCLE);
    posix_sleep_ms(200);

    // disable paper motor and wait 400ms with ip voltage on
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off);
    posix_sleep_ms(400);

    engine_stop_high_voltage(eng_hw);

    SLOG_ERR("<== %s\n", __func__);
}

// 
bool engine_print_page(engine_hardware_t* eng_hw)
{
    uint32_t tstart;

    MSG("engine_print_page\n");

    if ( !engine_try_to_clear_errors( ) ) {
	SLOG_ERR( "engine error\n" );
        return false;
    }

    // fuser is hot, motors are turning, laser is ready
    // high voltage is on, so move some paper and tell
    // video to do it's thing
    engine_state.picked_page = engine_state.pick_page;
    tstart = engine_get_time_msec();
    SLOG_ERR("PICK T = 0 @ %d \n", tstart, engine_state.picked_page);

    dec_printer_pick_paper( );
    delay_msec_timer(420); // pick to register roller: 428 = (( 62.5 + 63.5 )) / 294


    video_print_page((PAGE_DATA*)engine_state.picked_page);
    slogit(  dec_enable_vdo( eng_hw->laser, true ));
    slogit(  dec_enable_vdo_top_sensor( eng_hw->laser, eng_hw->paper_entry, true )); // false for off
    SLOG_ERR("pre TOP @ %d  top on entry %d\n", engine_get_time_msec()-tstart, dec_sensor_poll( eng_hw->paper_entry ));

    // 
    // if (dec_sensor_block_state( eng_hw->paper_entry, false, PAPER_ENTRY_TOP_TIMEOUT ))  // wait for paper TOP'
    if (dec_sensor_block( eng_hw->paper_entry,  1000 ))  // wait for paper TOP
    {
        ERR("paper pick failure \n");       
        SLOG_ERR("paper pick failure\n");       
        return false;
    }

    engine_state.active_page = engine_state.picked_page;
    SLOG_ERR("==> %s active page %#x\n", __func__, engine_state.active_page);
    delay_msec_timer_non_blocking(engine_state.top_to_bottom_delay_msecs,
				  engine_state_set_page_at_bottom_cb,
				  engine_state.active_page,
				  false /* non-repeating */
				  );
    // 294 mm / sec engine speed
    // 80 mm interpage gap 


    //slogit(  dec_laser_set_top_sync( eng_hw->laser, true ));  // fake top
    SLOG_ERR("TOP @ %d\n", engine_get_time_msec()-tstart);

    delay_msec_timer(154); // 45.5mm input to transfer / 294 mm/sec  

    pwm_run_dutycycle_percent(eng_hw->transfer_voltage, TRANSFER_PRINT_DUTY_CYCLE);
    
    /// todo: input of page size currently LETTER
    /// 
    delay_msec_timer( 584 ) ; // function of page size  (( 11 * 25.4 )  - ( 62.5 + 63.5 + 16.5) + 80)/ 294 - .154
    SLOG_ERR("get next page @ %d\n", engine_get_time_msec()-tstart);
    return true; // ok to pick again
    
}
 


void* engine_ctrl_loop(void* unused)
{  
    engine_hardware_t* eng_hw;
    bool page = false;
    int timeout_10msecs = 10;
    uint32_t page_avail_in_x_msec = 0;

    SysWaitForInit();

    eng_hardware = (engine_hardware_t*)MEM_MALLOC(sizeof(engine_hardware_t));
    REL_ASSERT(eng_hardware != NULL);

    eng_hw = eng_hardware;

    engine_init_hw(eng_hw);

    engine_dbg_init();
    
    atassert( engine_atassert );
    MSG("mech thread init\n");

    video_register_page_done_callback(&engine_pagedone_callback);

    pwm_off(eng_hw->main_fan);    // turn off main fan

    dec_fuser_enable_power(true);   // enable 24v power supply

    // TODO: remove this; force state for the moment until the sensor works
    //engine_state_set_paperout( false ); // have paper 

    //slogit(     pwm_run(thv_en, 1));  // cal thv_en
    while (1)
    {
        // block here doing nothing until we have a page

        page_avail_in_x_msec = block_page_avail( timeout_10msecs );

        if (!dec_sensor_poll(eng_hw->paper_avail)) 
        { 
            engine_state_set_paperout( true );
            timeout_10msecs = 100; // 
            continue;  // loop waiting for jobs/paper added
        }
        else 
        {
	    engine_state_set_paperout( false ); // have paper 
        }

        if ( page_avail_in_x_msec == 0 )
        {
            // block waiting for page wanting to be printed, 
            // unblock is just to test for paper added.
            // not in a job so lets cool down.

            dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0)); // off
            continue; // loop waiting for jobs
        }
        MSG("PAGE AVAILABLE\n");
        if (!page)
            page = printer_get_page(400);  // 4 seconds is rather long!
                                                                       
        if (!page)
        {
            MSG("%s: Page not found; wait for a page again\n", __func__);
            continue;
        }
        MSG("HAVE PAGE\n");

        if(!engine_print_prepare(eng_hw))
        {
            continue;
        }

        //print pages  // todo: add mpt_sensor poll

        while ( (true == dec_sensor_poll(eng_hw->paper_avail)) && page )
        {
            if(!engine_print_page(eng_hw))
            {
	        SLOG_ERR( " pick failure " );
                break;
            }
            slogit( page = printer_get_page(50) );            
        }
	// todo: need to wait for pages to finish with variable delay based
	// on number of pages active and simplex duplex.
	
        // TODO: need a real sequence here, but this hopefully works for now
        // engine stop
        slogit( dec_stop_laser_mirror(eng_hw->laser) );
        posix_sleep_ms(PAPER_MOTOR_FINISH_TIMEOUT*MILLISEC_PER_TICK);

        engine_ramp_down(eng_hw);

        engine_print_stop(eng_hw);

        slogit(    engine_state.in_job = false);

    } // end of while (1)
    return 0;
}

void engine_clean(engine_hardware_t* eng_hw)
{
    SLOG_ERR("==> %s\n", __func__);
#ifdef TODO
    dec_printer_enable_thv(true);

    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
 
    delay_msec_timer(10);
    pwm_run(eng_hw->charging_voltage, 1);

    delay_msec_timer(232);

    pwm_run(eng_hw->developer_voltage, 1);

    delay_msec_timer(750);

    dec_printer_enable_thv(false);
    pwm_run(eng_hw->transfer_voltage, 1);

    delay_msec_timer(2008);

    engine_stop_high_voltage(eng_hw);

    delay_msec_timer(250);

    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off );
#endif
    SLOG_ERR("<== %s\n", __func__);
    
    return;
}

uint32_t engine_get_top_margin_in_lines( PAGE_DATA *page, COLOR process_color )
{
    // mono engine so no modification of margin is needed.
    return engine_get_default_top_margin_in_lines(page, process_color);
}

uint32_t engine_get_side_margin_in_subpixels( PAGE_DATA *page,
                                              uint32_t channel_number,
                                              uint32_t image_width_in_pixels,
                                              resolution_t page_x_resolution )
{
    // mono engine so no modification of margin is needed.
    return engine_get_default_side_margin_in_subpixels(page, channel_number, image_width_in_pixels, page_x_resolution);
}

/**************************************************************************/
/**************************************************************************/
/* DEBUG & TEST                                                           */
/**************************************************************************/
/**************************************************************************/

#ifdef HAVE_CMD
void eng_test_paper_path(engine_hardware_t* eng_hw, bool test_fuser)
{
    uint32_t err;
    uint32_t tstart;
    uint32_t motor_ramp_delay;

    ASSERT(eng_hw != NULL);

    dbg_printf("Let's %s some paper!\n", test_fuser ? "fuse" : "move");

    slogit(    pwm_run_dutycycle_percent(eng_hw->main_fan, MAIN_FAN_FULL) );

    if(test_fuser)
    {
        dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(1) ); // warm
        dbg_printf("Wait fuser for fuser warm.\n");
        dec_fuser_block_till_at_temp( eng_hw->fuser );
	// dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(2) );
        dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(3) ); 
    }

    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_full );
    delay_msec_timer(1750);

    motor_ramp_delay = dcmotor_speed_msecs_to_ramp(eng_hw->paper_motor, eng_hw->speed_full)*2;
    dbg_printf("Motor ramp delay = %d\n", motor_ramp_delay);
    err = dcmotor_wait( eng_hw->paper_motor, motor_ramp_delay ); /// should be at speed within this time
    if ( err ) 
    {
        dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off
        dbg_printf("paper motor watchdog, jammed = %#x\n", err);
        goto EXIT;
    }

    // dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(3) ); 
    tstart = engine_get_time_msec();
    dbg_printf("PICK T = 0 @ %d\n", tstart);
    dec_printer_pick_paper( );

    if(test_fuser)
    {
        dbg_printf("Wait fuser for fuser hot.\n");
        dec_fuser_block_till_at_temp( eng_hw->fuser );
    }

    //    if (dec_sensor_block_state( eng_hw->paper_entry, true, PAPER_ENTRY_TOP_TIMEOUT ))  // wait for paper TOP
    if (dec_sensor_block_state( eng_hw->paper_entry, true, PAPER_ENTRY_TOP_TIMEOUT ))  // wait for paper TOP
    {
        dbg_printf("paper pick failure \n");       
        goto EXIT;
    }

    dbg_printf("PICK done. FEED @ %d\n", engine_get_time_msec()-tstart);


    // wait for paper entry to go inactive
    if (dec_sensor_block_state( eng_hw->paper_entry, false, PAPER_ENTRY_BOTTOM_TIMEOUT ))  // bottom
    {
        dbg_printf("paper feed jam \n");   
        dec_printer_feed_paper(false);
        goto EXIT;
    }    

    dbg_printf("FEED done. deactivate FEED clutch @ %d\n", engine_get_time_msec()-tstart);


    if( dec_sensor_block( eng_hw->paper_exit, PAPER_EXIT_TOP_TIMEOUT ) )
    {            
        dbg_printf("Exit sensor indicates paper jam at laser \n");
        goto EXIT;
    }

    dbg_printf("F sensor inactive @ %d\n", engine_get_time_msec()-tstart);

    // wait for paper exit sensor to go inactive
    if( dec_sensor_block( eng_hw->paper_exit, PAPER_EXIT_BOTTOM_TIMEOUT ) )
    {            
        dbg_printf("EXIT sensor indicates paper exit jam \n");
    }

    dbg_printf("EXIT done @ %d\n", engine_get_time_msec()-tstart);

    posix_sleep_ms(PAPER_MOTOR_FINISH_TIMEOUT*MILLISEC_PER_TICK);
EXIT:
    /// turn it all off.
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off
    dcmotor_run(eng_hw->paper_motor, eng_hw->speed_off);
    pwm_off(eng_hw->main_fan);

    dbg_printf("Paper Path test done.\n");
}

void eng_test_fuser(engine_hardware_t* eng_hw)
{
    dbg_printf("Start Fuser Test.\n");

    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(1) ); // pre-heat

    dbg_printf("Wait fuser for fuser pre-heat.\n");
    dec_fuser_block_till_at_temp( eng_hw->fuser );
    dbg_printf("Pre-heat done\n");

    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(2) ); // warm
    delay_msec_timer(1750);
    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(3) ); // hot

    dbg_printf("Wait fuser for fuser hot.\n");
    dec_fuser_block_till_at_temp( eng_hw->fuser );
    dbg_printf("Hot done\n");

    delay_msec_timer(1000);

    dec_fuser_set_temp( eng_hw->fuser, dec_fuser_temp_by_index(0) ); // off

    dbg_printf("Fuser test done.\n");
}

void dec_printer_pick_test(bool pick)
{
//TODO    REL_ASSERT( OK == gpio_write( eng_hardware->pick_pin, pick?1:0 ) );
}

void dec_printer_feed_test(bool feed)
{
    dec_laser_t *laser;
    dec_sensor_t *paper_entry;

    laser = dec_laser_by_index(0);
    paper_entry = dec_sensor_by_name("paper_entry_sensor"); 

//TODO    REL_ASSERT( OK == gpio_write( eng_hardware->regi_clutch_pin, feed?1:0 ) );

    SLOG_ERR(" vdo dma TOP test \n");
    slogit(  dec_enable_vdo_top_sensor( laser, paper_entry, true )); // false for off
    slogit(  dec_enable_vdo( laser, true ));
}

void engine_debug_usage()
{
    dbg_printf("Engine command options:\n");
    dbg_printf("    test ppath - attempt to run a sheet through the paper path\n");
    dbg_printf("    test ppathf - attempt to run a sheet through the paper path with fuser active\n");
    dbg_printf("    test fuser - ramp fuser to hot, leave hot for 1s, then disable\n");
    dbg_printf("    fan [on|off] - turn main engine fan on or off\n");
    dbg_printf("    power [on|off] - turn 24v power supply on or off\n");
    dbg_printf("    pick [on|off] - engage/disengage paper pick clutch\n");
    dbg_printf("    feed [on|off] - engage/disengage paper feed clutch\n");
    dbg_printf("    clean - run engine cleaning cycle\n");
}

void engine_debug_cmd_err()
{
    dbg_printf("Engine command error or invalid command.\n");
    engine_debug_usage();
}

int engine_debug_callback( int argc, char *argv[] )
{
    engine_hardware_t* eng_hw = eng_hardware;
    int pos = 1;

    if( argc < 2 || str_match( argv[pos], "help", 4 )) 
    {
        engine_debug_usage();
        /* not enough parameters */
        return 0;
    }

    ASSERT(eng_hw != NULL);
/*
    int i;
    dbg_printf("command args %d : ", argc);
    for (i = 0; i < argc; i++)
    {
        dbg_printf("%s ", argv[i]);
    }
    dbg_printf("\n\n");
*/
    if( str_match(argv[pos], "test", 4 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "ppath", 5 ) )
        {
            eng_test_paper_path(eng_hw, false);
        }
        else if(str_match(argv[pos], "ppathf", 6 ) )
        {
            eng_test_paper_path(eng_hw, true);
        }
        else if(str_match(argv[pos], "fuser", 5 ) )
        {
            eng_test_fuser(eng_hw);
        }
        else
        {
            engine_debug_cmd_err();
        }
    }
    else if( str_match(argv[pos], "fan", 3 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "on", 2 ) )
        {
            pwm_run_dutycycle_percent(eng_hw->main_fan, MAIN_FAN_FULL);
        }
        else if(str_match(argv[pos], "off", 3 ) )
        {
            pwm_off(eng_hw->main_fan);
        }
        else
        {
            engine_debug_cmd_err();
        }
    }
    else if( str_match(argv[pos], "power", 5 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "on", 2 ) )
        {
            engine_power_enable(eng_hw, true);
        }
        else if(str_match(argv[pos], "off", 3 ) )
        {
            engine_power_enable(eng_hw, false);
        }
        else
        {
            engine_debug_cmd_err();
        }
    }

    else if( str_match(argv[pos], "pick", 4 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "on", 2 ) )
        {
            dec_printer_pick_test(true);
        }
        else if(str_match(argv[pos], "off", 3 ) )
        {
            dec_printer_pick_test(false);
        }
        else
        {
            engine_debug_cmd_err();
        }

    }
    else if( str_match(argv[pos], "feed", 4 ) ) 
    {
        if(argc < 3)
        {
            engine_debug_cmd_err();
            goto DONE;
        }

        pos++;
        if(str_match(argv[pos], "on", 2 ) )
        {
            dec_printer_feed_test(true);
        }
        else if(str_match(argv[pos], "off", 3 ) )
        {
            dec_printer_feed_test(false);
        }
        else
        {
            engine_debug_cmd_err();
        }

    }
    else if( str_match(argv[pos], "clean", 5 ) ) 
    {
        engine_clean(eng_hw);
    }
    else
    {
        dbg_printf("Failed to match command %s", argv[pos]);
        engine_debug_cmd_err();
    }
    
DONE:
    return 0;
}
#endif

void engine_dbg_init()
{
#ifdef HAVE_CMD
    int i_res;
    i_res = cmd_register( "eng",
                          NULL,
                          NULL,
                          NULL,
                          engine_debug_callback );
    XASSERT( i_res == CMD_OK, i_res );
#endif
}




