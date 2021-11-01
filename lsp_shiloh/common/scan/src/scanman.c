/**
 * \file scanman.c
 *
 * \brief Scan Manager 
 *
 *  This file's entire reason for existence is to sit between Scantask and
 *  whatever architecture the rest of The System might create.  
 *
 *  ScanMan exists to keep Scantask software architecture independent.
 */
/*
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
    
/* davep 23-Jul-2008 ; added -D__AGMESSAGE_H_ to the makefile to prevent
 * agMessage.h from being included in the scan code.  But we need agMessage.h in
 * this file.
 */
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <string.h> 
#include <pthread.h>

#include "posix_ostools.h"
#include "scos.h"

#include "ATypes.h"
#include "list.h"           
#include "lassert.h"
#include "agResourceMgr.h"
#include "agMessage.h"
#include "agRouter.h"
#include "memAPI.h"
#include "utils.h"
#include "agConnectMgr.h"
#include "sys_init_api.h"
#include "data_access_api.h"
#include "error_types.h"
#include "platform.h"
#include "event_observer.h"
#include "response_subject.h"

#include "scancore.h"
#include "scantypes.h"
#include "scanvars.h"
#include "scanlib.h"
#include "scanman.h"
#include "scantools.h"
#include "scandbg.h"
#include "scanhw.h"
#include "scanhwerr.h"
#include "scan.h"
#include "scandbg.h"
#include "cal.h"
#include "calstats.h"
#include "piehw.h"
#include "pie.h"
#include "scanACL.h"
#include "smirb.h"
#include "scantask.h"
#include "cmdline.h"
#include "scands.h"
#include "demo_scanapp.h"
#include "icefileapp.h"
#include "scanmech.h"
#include "scanmsg.h"
#include "safetylock.h"
#include "icedma.h"
#include "logger.h"
#include "a3mergeInterface.h"
#include "rollereraseMain.h"
#include "SysUI_Interface.h"
#include "SysServiceMgr.h"

//#include "../../../../common/include/spec.h"
//#include "spec.h"

/* enable to add more debug messages */
#define SCANMAN_DEBUG

#ifdef SCANMAN_DEBUG
    #define SCANMAN_ASSERT(x)  ASSERT((x))
#else
    #define SCANMAN_ASSERT(x)
#endif

/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "[S_M_U] "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(4)
///Using Method.
// for Error Log Level Display : DBG_PRINTF_SDH_ERR(...)
// for Debug Log Level Display : DBG_PRINTF_SDH_DEBUG(...)

#define HAVE_SCAN_ERASE_ROLLER				1
#define USE_DELAY_FOR_DLT_MERGE_IOCTL		1

#define SCANMAN_QUEUE_SIZE 20 
#define SCANMAN_TASK_PRI     THR_PRI_NORMAL
#define SCANMAN_STACK_SIZE   (POSIX_MIN_STACK_SIZE  * 4)// has to be big enough for pthreads
#define NUMSCANMODULES 2

/** handy macro to track the ScanMan state machine transitions */
#define STATE_TRANSITION(new_state)  (state_transition( (new_state), __LINE__ ))

/* davep 16-Apr-2013 ; test/debug feature; pretend we're scanning on a dual
 * scan system (sensor on both sides of paper)
 */
//#define FAKE_DUAL_SCAN 1

static scan_err_t scanman_clear_roller_erase(void);

// align the stack
ALIGN(8)
static UINT8 scanman_stack[SCANMAN_STACK_SIZE];
static pthread_t scanman_task_id; 
static mqd_t scanman_msgq;
static UINT16 scanPaperSizeCode;
//static MESSAGE scanman_msgq_buffer[SCANMAN_QUEUE_SIZE];

typedef enum {
    SMSTATE_IDLE=1,
    SMSTATE_STARTING=2,
    SMSTATE_SCANNING=3,
    SMSTATE_CANCELING=4,
    SMSTATE_FAILING=5,
    SMSTATE_WAITING=6,
    SMSTATE_WAITING_FOR_SCANTASK_1=7,
    SMSTATE_WAITING_FOR_SJM=8,
    SMSTATE_WAITING_FOR_APP=9,
    SMSTATE_WAITING_FOR_SCANTASK_2=10,
} scanman_state;

typedef enum {
	SMCALSTATE_CALIBRATION_NONE=1,
	SMCALSTATE_CALIBRATION_PROCESSING=2,
	SMCALSTATE_CALIBRATION_WAIT_JOBCANCEL=3,
} scanman_calibration_state;

static scanman_state g_curr_state;

typedef enum {
	SCAN_DOCUMENT_SIMPLEX	= 0,
	SCAN_DOCUMENT_DUPLEX	= 1,
} scan_document;

typedef struct scanman_stats_s {
	uint8_t	scan_job_type;
	uint32_t	page_ends;
	uint8_t	calibration_state;
	uint8_t	powersave_state;
}scanman_stats_t;
static scanman_stats_t scanman_stats;

/* davep 06-Dec-2005 ; state transition table
 * davep 13-Nov-2008 ; new state transition table
 * davep 24-Jun-2009 ; scanman v2 with no stopy copy support.
 * davep 27-Jun-2009 ; http://10.71.116.60/view/Main/ScanManv2 
 *
 * Scanman needs to maintain state in order to know when to start/stop the
 * scantask. Cancel brought out a lot of problems with scanman/scantask's
 * interaction so I had to make this state machine. 
 *
 */

/* ScanMan vs Scantask.
 *
 * ScanMan exists to protect Scantask from the rest of the system.  ScanTask
 * controls all the scan hardware and, as such, is extremely complex, containing
 * several message queues and state machines.  ScanMan prevents other threads
 * from interfering with Scantask.
 *
 * ScanMan 'blackout' period. With the previous paragraph in mind, there are
 * times when Scantask cannot talk to the rest of the system. A cancel in a bad
 * spot could be misinterpreted and cause certain operations (e.g., physical paper
 * present flag scans) to fail. This is the "Scantask blackout period".  The
 * 'waiting4scantask' state is the Scantask blackout. We wait for the
 * SMSG_SCAN_READY before we can talk to Scantask again.
 *
 *
 * Some Random Notes.
 *
 * 'scanning' receives SMSG_SCAN_FAILED when Scantask fails (paper jam, mispick,
 * etc.)  ScanMan will call the SJM to cancel the current job. 
 *
 * When 'scanning' receives MSG_CANCEL, ScanMan will ask Scantask to cancel.
 * ScanMan waits for the SMSG_SCAN_FAILED indicating Scantask has successfully
 * canceled. Then ScanMan waits for 
 *                                                     
 * In the 'scanning' state, we could receive multiple SMSG_PAGE_START,
 * SMSG_PAGE_END when we're scanning from ADF.
 *
 * waiting4scantask vs waiting4sjm. On leaving the 'scanning' state, ScanMan has
 * to rendezvous between two different threads: the SJM (System Job Manager) and
 * Scantask. On SCAN_FAILED, ScanMan asks SJM to cancel the job then goes to the
 * 'failing' state. There is no need to cancel Scantask. At this point, ScanMan
 * has to wait for both Scantask and SJM. ScanMan waits for the MSG_CANCEL from
 * SJM and the SMSG_SCAN_READY from Scantask. Those messages can arrive in
 * either order but both have to be received in order to go back to IDLE.
 *
 *
 */


/** When Application Request Merge, we control the state of merge progressing */
typedef enum {	
	MERGE_PROGRESS_NONE = 0,
	MERGE_PROGRESS_STARTED = 1,
    MERGE_PROGRESS_PARAM_SETED = 2,
    MERGE_PROGRESS_FIRST_PAGE_STARTED = 3,
    MERGE_PROGRESS_FIRST_PAGE_DONE = 4,
    MERGE_PROGRESS_SECOND_PAGE_STARTED = 5,
    MERGE_PROGRESS_SECOND_PAGE_DONE = 6,
} scan_merge_state_t;

typedef enum {
	MERGE_IOCTL_PARAM_A3_TO_BACK = 0,
	MERGE_IOCTL_PARAM_A3_EJECT = 1,
	MERGE_IOCTL_PARAM_DLT_TO_BACK_LITTLE = 2,
	MERGE_IOCTL_PARAM_DLT_TO_BACK = 3,
	MERGE_IOCTL_PARAM_DLT_EJECT = 4,
} scan_merge_ioctl_param_t;

typedef struct merge_function_status_s {
    /* Regarding merge control variable */
    uint8_t	merge_requestAppSrc;
    uint8_t	merge_resultSize;
    uint8_t	merge_progressState;
    uint8_t	merge_scannedPageNum;
}merge_function_status_t;
static merge_function_status_t		merge_state;

#if USE_DELAY_FOR_DLT_MERGE_IOCTL
typedef enum {
	MERGE_DLT_FIRST_PAGE_NONE = 0,
	MERGE_DLT_FIRST_PAGE_IOCTL_REQUEST = 1,
	MERGE_DLT_FIRST_PAGE_IOCTL_DONE = 2,	
	MERGE_DLT_FIRST_PAGE_JOBSTART_PENDED = 3,
} scan_merge_dlt_job_control;
static uint8_t	g_special_flag_DLT_merge_pending_jobStart;
#endif

#if HAVE_SCAN_ERASE_ROLLER
typedef enum {	
	ERASE_PROGRESS_NONE = 0,
	ERASE_PROGRESS_PARAM_SETED = 1,
	ERASE_PROGRESS_STARTED = 2,
	ERASE_PROGRESS_END = 3,
} scan_erase_state_t;

typedef struct rollerErase_status_s {
	/* Regarding erase roller control variable */
	uint8_t	enable;
	uint8_t	state;
	uint8_t	scanJobType;
}rollerErase_status_t;
static rollerErase_status_t		rollerErase_state;
static uint32_t g_roller_adjust_y_value;
#endif
static bool fskipfail=false;

/**
 * \brief  track our state transitions 
 *
 * \author David Poole
 * \date 21-Sep-2007
 *
 */

static void state_transition( scanman_state new_state, int line_number )
{
    DBG_PRINTF_SDH_DEBUG( "scanman state from %d to %d at %d\n", 
                g_curr_state, new_state, line_number );
    g_curr_state = new_state;

    /* davep 01-Jun-2012 ; adding observer events to make control panel happy */
    switch( new_state ) {
        case SMSTATE_IDLE :
            scanman_observer_notify_event( SCANMAN_EVENT_IDLE, 1 );
            break;

        case SMSTATE_STARTING :
            scanman_observer_notify_event( SCANMAN_EVENT_SCAN_START, 1 );
            break;

        case SMSTATE_SCANNING :
            scanman_observer_notify_event( SCANMAN_EVENT_SCAN_RUNNING, 1 );
            break;

        case SMSTATE_CANCELING : 
        case SMSTATE_FAILING : 
            scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CANCEL, 1 );
            break;

        case SMSTATE_WAITING : 
            /* we get into the waiting state on a successful completion of a
             * scan (i.e., a scan that wasn't canceled or failed due to some
             * error)
             */
            scanman_observer_notify_event( SCANMAN_EVENT_SCAN_COMPLETE, 1 );
            break;

        default : 
            /* huh? */
            break;
    }

}

int scanman_get_powersave_state( void )
{
	int isPowersaveMode = 0;
	isPowersaveMode = scanman_stats.powersave_state;
	return isPowersaveMode;
}

void scanman_set_powersave_state( int isPowerSave )
{
	scanman_stats.powersave_state = isPowerSave;	
}

/**
 * \brief return pointer to my pxthread id 
 *
 *  Originally created to verify function caller thread context.
 *
 * \author David Poole
 * \date 15-Sep-2011
 *
 */

void scanman_set_skipmsgfail(bool flag)
{
	fskipfail = flag;	
}

bool scanman_get_skipmsgfail(void)
{
	return fskipfail;	
}

pthread_t scanman_thread_identify( void )
{
    return scanman_task_id;
}

 /**
  * \brief  Blocking send a message to ScanMan
  *
  * Wrapped the Message Router in a function so we can separate the Router from
  * Scantask.  (Working on the Scan/Copy SDK.)
  *
  * This could be called from interrupt context!
  *
  * \author David Poole
  * \date 23-Jun-2008
  *
  * davep 23-Jul-2008 ; moving the scan subsystem (scantask) to using its
  * own message (scan_msg_t) as part of the BSDK. 
  *
  * Scantask uses scanman_msg_send() to communicate messages outside his
  * own scan_task() thread.  scanman_msg_send() translates the
  * scan_msg_t structure to a MESSAGE structure.
  *
  * The fields of scan_msg_t and MESSAGE are exactly the same. 
  * 
  * ScanMan will receive MESSAGE but the values in msgType will be the SMSG_xxx
  * values from scancore.h and scantypes.h which leaves us with the possibility
  * of collision with agMessage.h AGMSG values.
  *
  * So cheat and set the high bit on our outgoing SMSG_xxx msgType so ScanMan
  * can tell where the message came from.
  *
  * THIS IS A KLUDGE!  Yeah, yeah, yeah.  
  *
  */

scan_err_t scanman_msg_send( scan_msg_t *scan_msg )
{
    error_type_t err;
    MESSAGE sys_msg;

#define MESSAGE_FROM_SCANTASK_FLAG (1<<31)

    /* NOTE! This function can be called from interrupt context! */

    /* Another note. This is the ONE and ONLY ONE place where we move a
     * scan_msg_t to a MESSAGE.
     */
    sys_msg.msgType = MESSAGE_FROM_SCANTASK_FLAG | scan_msg->msgType;
    sys_msg.param1 = scan_msg->param1;
    sys_msg.param2 = scan_msg->param2;
    sys_msg.param3 = scan_msg->param3;

    err = SYMsgSend( SCANMANID, &sys_msg );
    if( err != OK ) {
        XASSERT( err==OK, err );
        return SCANERR_MSG_FAIL;
    }

    return SCANERR_NONE;
}

/**
 * \brief Updates a int data store varaiable
 *
 * \author Alan Smithee 
 *
 * We cannot update the scands from interrupt context (e.g., the paper present flag). 
 *
 * Using ScanMan instead of ScanTask because ST has many message loops and ST
 * has the taskqueue.
 *
 **/

static void handle_paper_event_msg( scan_adf_paper_event_t adfevt, UINT32 value  )
{
    scan_err_t scerr;

    DBG_PRINTF_SDH_DEBUG( "scanman received adf paper event evt=%d value=%d\n", adfevt, value );

    switch( adfevt ) {
        case SCAN_ADF_PAPER_PRESENT :
            /* value contains the current status 
             * value==0 paper not present
             * value==1 paper present
             */

            scerr = scands_set_integer( "adf_paper_present", value && 1 );
            XASSERT( scerr==SCANERR_NONE, scerr );

            scanman_observer_notify_event( SCANMAN_EVENT_ADF_PAPER_PRESENT, value );
            break;

        case SCAN_ADF_PAPER_NOPICK:
            scerr = scands_set_integer( "adf_paper_nopick", value && 1 );
            XASSERT( scerr==SCANERR_NONE, scerr );

            scanman_observer_notify_event( SCANMAN_EVENT_ADF_PAPER_NOPICK, value );
            break;

        case SCAN_ADF_PAPER_JAM : 
		case SCAN_ADF_PAPER_SHORT :
            /* value==1 paper jammed
             * value==0 paper jam cleared
             */
            scerr = scands_set_integer( "adf_paper_jam", value && 1 );
            XASSERT( scerr==SCANERR_NONE, scerr );

            scanman_observer_notify_event( SCANMAN_EVENT_ADF_PAPER_JAM, value );
            break;
		case SCAN_ADF_TOP_COVER_OPEN : 
            /* value==1 top cover open
             * value==0 top cover close
             */
            scerr = scands_set_integer( "adf_top_cover_open", value && 1 );
            XASSERT( scerr==SCANERR_NONE, scerr );

            scanman_observer_notify_event( SCANMAN_EVENT_ADF_TOP_COVER_OPEN, value );
            break;
            
		case SCAN_FB_PAPER_EMPTY :
            scanman_observer_notify_event( SCANMAN_EVENT_FB_PAPER_EMPTY, value );
            break;

		/* Scanner SP Mech Test */
		case SCAN_SP_PAPER_PRESENT :
            scerr = scands_set_integer( "scanner_sp_test", value && 1 );
			XASSERT( scerr==SCANERR_NONE, scerr );
			SPResponseScannerSensorTest(SP_MODE_SCANNER_CHECK_PRESENT_SENSOR, value);
			break;
		case SCAN_SP_PAPER_INTERVAL :
            scerr = scands_set_integer( "scanner_sp_test", value && 1 );
			XASSERT( scerr==SCANERR_NONE, scerr );
			SPResponseScannerSensorTest(SP_MODE_SCANNER_CHECK_INTERVAL_SENSOR, value);
			break;
		case SCAN_SP_PAPER_1STSCAN :
            scerr = scands_set_integer( "scanner_sp_test", value && 1 );
			XASSERT( scerr==SCANERR_NONE, scerr );
			SPResponseScannerSensorTest(SP_MODE_SCANNER_CHECK_FIRSTSCAN_SENSOR, value);
			break;
		case SCAN_SP_PAPER_TOPCOVER :
            scerr = scands_set_integer( "scanner_sp_test", value && 1 );
			XASSERT( scerr==SCANERR_NONE, scerr );
			SPResponseScannerSensorTest(SP_MODE_SCANNER_CHECK_TOPCOVER_OPEN, value);
			break;
		case SCAN_SP_PAPER_HOME :
            scerr = scands_set_integer( "scanner_sp_test", value && 1 );
			XASSERT( scerr==SCANERR_NONE, scerr );
			SPResponseScannerSensorTest(SP_MODE_SCANNER_CHECK_HOME_SENSOR, value);
			break;
		case SCAN_ADF_POWERSAVE_DONE:
			DBG_PRINTF_SDH_DEBUG( "scanman received adf paper event evt= SCAN_ADF_POWERSAVE_DONE\n");
            scerr = scands_set_integer( "scanner_adf_powersave_done", value && 1 );
			XASSERT( scerr==SCANERR_NONE, scerr );
			break;
		case SCAN_ADF_WAKEUP_DONE:
			DBG_PRINTF_SDH_DEBUG( "scanman received adf paper event evt= SCAN_ADF_WAKEUP_DONE\n");
            scerr = scands_set_integer( "scanner_adf_wakeup_done", value && 1 );
			XASSERT( scerr==SCANERR_NONE, scerr );
			break;

		default :
            /* ignore */
            break;
    }
}

/**
 * \brief Blocking wait for calibration_done
 *
 * This function handles the "scantask blackout period".
 *
 * Blocks waiting for Scantask's SMSG_SCAN_READY message.
 *
 * \author Yhlee
 * \date 23-Apr-2015
 *
 */

static void blocking_wait_for_calibration_done( void )
{
	UINT pxretcode;
	MESSAGE sys_msg;
	BOOL calibration_done;
	
    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    calibration_done = FALSE;

    /* Wait for scantask to finish canceling; Scantask will reply with an
     * SMSG_SCAN_FAILED message
     */
    while( !calibration_done) {
        pxretcode = posix_wait_for_message( scanman_msgq, (char *)&sys_msg, sizeof(MESSAGE),
                                            POSIX_WAIT_FOREVER );
        XASSERT( pxretcode == 0 || pxretcode == ETIMEDOUT, pxretcode );

        if( pxretcode != 0 ) {
            /* scanman must wait for a message from scantask telling us he's calibration is dont */
            DBG_PRINTF_SDH_ERR("%s no messages; wait some more\n", __FUNCTION__ );

            /* stay in message loop */
            continue;
        }
        
        switch( sys_msg.msgType ) {
            case (SMSG_CAL_CALIBRATION_IN_PROGRESS | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG( "%s received SMSG_CAL_CALIBRATION_IN_PROGRESS in state=%d\n",__FUNCTION__ , g_curr_state);
				scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_PROCESSING;
                scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CALIBRATION_RUNNING, 1 );
                break;

            case (SMSG_CAL_CALIBRATION_DONE | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG( "%s received SMSG_CAL_CALIBRATION_DONE in state=%d\n", __FUNCTION__, g_curr_state);
				/*
				 * SINDOH yhlee add this routine
				 * During machine wake-up, user can start copy/scan... job,
				 * scanman wait until Calibration done
				 */
				calibration_done = TRUE;			
				scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_NONE;

				scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CALIBRATION_COMPLETE, 1 );
				scanman_observer_notify_event( SCANMAN_EVENT_SCAN_RUNNING, 1 );
                break;
                
            default:
                /* we check unknown message */
				DBG_PRINTF_SDH_DEBUG( "%s  received known message msgType=%d\n", __FUNCTION__, sys_msg.msgType);
				//XASSERT( 0, sys_msg.msgType );
				break;
        }
    } /* end while */

    DBG_PRINTF_SDH_DEBUG( "%s done\n", __FUNCTION__ );
}

/**
 * \brief Blocking wait for Scantask to be ready
 *
 * This function handles the "scantask blackout period".
 *
 * Blocks waiting for Scantask's SMSG_SCAN_READY message.
 *
 * \author David Poole
 * \date 05-Dec-2005
 * \date 27-Jun-2009 (removed the cancel, turned into a general blocking wait)
 *
 */

static void blocking_wait_for_scantask_ready( void )
{
	UINT pxretcode;
	MESSAGE sys_msg;
	scan_err_t scerr;
	BOOL scantask_done;
	struct scanman_ioctl_request_block *smirb;
	
    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    scantask_done = FALSE;

    /* Wait for scantask to finish canceling; Scantask will reply with an
     * SMSG_SCAN_FAILED message
     */
    while( !scantask_done) {
        pxretcode = posix_wait_for_message( scanman_msgq, (char *)&sys_msg, sizeof(MESSAGE),
                                            POSIX_WAIT_FOREVER );
        XASSERT( pxretcode == 0 || pxretcode == ETIMEDOUT, pxretcode );

        if( pxretcode != 0 ) {
            /* scanman must wait for a message from scantask telling us he's
             * done before we can leave
             */
            DBG_PRINTF_SDH_ERR("%s no messages; wait some more\n", __FUNCTION__ );

            /* stay in message loop */
            continue;
        }
        
        /* We should only be getting messages from scantask! Once we enter a
         * cancel state, nothing else should be talking to us.
         */
        /* davep 05-Aug-2009 ; (Bug 13698) we can get into this state with
         * cancel. Cancel is anything but clean. Have to handle (and ignore)
         * messages from the app.
         */
//        XASSERT( sys_msg.msgType & MESSAGE_FROM_SCANTASK_FLAG, 
//                    sys_msg.msgType );

        switch( sys_msg.msgType ) {
            case MSG_SCAN_JOBSTART:
                /* ignore */
                DBG_PRINTF_SDH_DEBUG("scanman received jobstart in state=%d\n", g_curr_state );
                break;

            case MSG_SCAN_JOBEND :
                /* ignore */
                DBG_PRINTF_SDH_DEBUG( "scanman received jobend in state=%d\n", g_curr_state );
                break;

            case MSG_CANCELJOB: 
                /* we're already canceling or finishing so ignore */
                DBG_PRINTF_SDH_DEBUG( "scanman received canceljob in state=%d\n", g_curr_state );
                break;

            case (SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG) :
            case MSG_ERASE_ROLLER_PAGE_DATA:
                DBG_PRINTF_SDH_DEBUG( "scanman cancel received pagedata\n" );
                /* free the data; ignore the rest of the message */
                PTR_FREE( sys_msg.param3 );
                break;

            case (SMSG_PAGE_START | MESSAGE_FROM_SCANTASK_FLAG) :
            case MSG_ERASE_ROLLER_PAGE_START:
                /* page_start is scantask telling us he's starting data (sort of
                 * like a "file open")
                 */
                DBG_PRINTF_SDH_DEBUG( "scanman cancel received pagestart\n");
                break;

            case (SMSG_PAGE_END | MESSAGE_FROM_SCANTASK_FLAG):
            case MSG_ERASE_ROLLER_PAGE_END:
                /* page_end is scantask telling us he's done sending data (sort
                 * of like a "file close")
                 */
                DBG_PRINTF_SDH_DEBUG( "scanman cancel received pageend\n");
                break;
            
            case (SMSG_SCAN_END | MESSAGE_FROM_SCANTASK_FLAG):
            case MSG_ERASE_ROLLER_SCAN_END:
                /* scan_end is scantask telling us he's done with the scan but
                 * he's not yet ready to accept another scan
                 */
                DBG_PRINTF_SDH_DEBUG( "scanman cancel received scanend\n");
                break;
            
            case (SMSG_SCAN_READY | MESSAGE_FROM_SCANTASK_FLAG):
            case MSG_ERASE_ROLLER_SCAN_READY:
                /* scan_ready is the "ack" from scantask telling us he's done */
                DBG_PRINTF_SDH_DEBUG( "scanman cancel received scan_ready\n");
                scantask_done = TRUE;
                STATE_TRANSITION( SMSTATE_IDLE );
#if HAVE_SCAN_ERASE_ROLLER
				//Change state of 'roller erase'
				if( rollerErase_state.enable == 1 ) {
					scanman_clear_roller_erase();
				}
#endif
                break;

            case (SMSG_SCAN_FAILED | MESSAGE_FROM_SCANTASK_FLAG):
                DBG_PRINTF_SDH_DEBUG( "scanman cancel received scan_failed\n");
                STATE_TRANSITION( SMSTATE_WAITING_FOR_SCANTASK_1 );
                break;

            /* the following are messages that can occur normally during a
             * cancel; the default case will assert so we can catch anything we
             * don't expect 
             *
             * XXX do we want to go back to simply ignoring the unexpected
             * messages?
             */
            case (SMSG_SCAN_SIZE | MESSAGE_FROM_SCANTASK_FLAG) :
                DBG_PRINTF_SDH_DEBUG( "scanman cancel received scan_size\n");
                break;

            case (SMSG_DATA_BLOB | MESSAGE_FROM_SCANTASK_FLAG):
                /* davep 27-Jun-2009 ; oops. Amazing this bug slipped through
                 * for so long.  The data blob contains a malloc'd buffer which
                 * needs free.
                 */
                PTR_FREE( sys_msg.param3 );
                break;

            case (SMSG_SCAN_ADF_PAPER_EVENT | MESSAGE_FROM_SCANTASK_FLAG) :
            	DBG_PRINTF_SDH_DEBUG( "(cancel) scanman received SMSG_SCAN_ADF_PAPER_EVENT in state=%d %d %d\n", 
            					g_curr_state, sys_msg.param1, sys_msg.param2 );
                handle_paper_event_msg( sys_msg.param1, sys_msg.param2 );
                break;

            case (SMSG_CAL_CALIBRATION_IN_PROGRESS | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG( "scanman cancel received SMSG_CAL_CALIBRATION_IN_PROGRESS in state=%d\n", g_curr_state);
				scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_PROCESSING;
                scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CALIBRATION_RUNNING, 1 );
                break;

            case (SMSG_CAL_CALIBRATION_DONE | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG( "scanman cancel received SMSG_CAL_CALIBRATION_DONE in state=%d\n", g_curr_state);
				if(scanman_stats.calibration_state == SMCALSTATE_CALIBRATION_WAIT_JOBCANCEL) {					
					/*
					 * SINDOH yhlee add this routine
					 * During machine wake-up, user can start copy/scan... job,
					 * After job started, user could cancel job immediately.
					 * For this situation, scan task released before calibration done.
					 * and scanman can't receive 'SCAN_READY' message from scantask.
					 */
					scantask_done = TRUE;					
				}
				scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_NONE;
				scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CALIBRATION_COMPLETE, 1 );
				
				/* send another 'scanning' status to make the status transition
				 * a little more intuitive
				 * scan running -> calibration running -> cal complete -> scan running 
				 */
				scanman_observer_notify_event( SCANMAN_EVENT_SCAN_RUNNING, 1 );
                break;

			case MSG_SCAN_POWERSAVE:
				DBG_PRINTF_SDH_DEBUG( "scanman cancel received  MSG_SCAN_POWERSAVE in state=%d, %d\n", g_curr_state, sys_msg.param1);
				if(sys_msg.param1 == ESM_WAKE_UP && g_curr_state == SMSTATE_IDLE) {	
					// mod.lsh.for calibration only idle state
					uint32_t req_id;
					scerr = scanman_dev_request(SCANMECH_IOCTL_WAKE_UP_MODE, 0, NULL, &req_id);
					scanman_set_powersave_state(ESM_WAKE_UP);
				}
				else if(sys_msg.param1 == ESM_ENERGY_SAVE) {
					uint32_t req_id;
					scerr = scanman_dev_request(SCANMECH_IOCTL_SLEEP_MODE, 0, NULL, &req_id); //MotorDriver Sleep mode mod.kks 2014.08.07
					scanman_set_powersave_state(ESM_ENERGY_SAVE);
				}
				break;
				                
		//add kmg
			case (SMSG_SC_AFE_OFFSET_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_AFE_OFFSET_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_AFE_OFFSET_FAIL, 1 );
				break;
				
			case (SMSG_SC_AFE_GAIN_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_AFE_GAIN_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_AFE_GAIN_FAIL, 1 );
				break;
				
			case (SMSG_SC_AFE_SETTING_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_AFE_SETTING_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_AFE_SETTING_FAIL, 1 );
				break; 
				
			case (SMSG_SC_CIS_HOME_POSITION_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CIS_HOME_POSITION_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CIS_HOME_POSITION_FAIL, 1 );
				break; 
				
			case (SMSG_SC_WHITE_STRIP_READ_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_WHITE_STRIP_READ_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_WHITE_STRIP_READ_FAIL, 1 );
				break;       
				
			case (SMSG_SC_BLACK_STRIP_READ_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_BLACK_STRIP_READ_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_BLACK_STRIP_READ_FAIL, 1 );
				break;    
				
			case (SMSG_SC_BAD_PIXEL_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_BAD_PIXEL_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_BAD_PIXEL_FAIL, 1 );
				break;                      
				
			case (SMSG_SC_CIS_HOME_OFF_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CIS_HOME_OFF_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CIS_HOME_OFF_FAIL, 1 );
				break; 
				
			case (SMSG_SC_CIS_HOME_ON_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CIS_HOME_ON_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CIS_HOME_ON_FAIL, 1 );
				break;         			
			
			case (SMSG_SC_PIC_OVF_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_PIC_OVF_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_PIC_OVF_FAIL, 1 );
				break;  

			case (SMSG_SC_CMD_QUEUE_OVR_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CMD_QUEUE_OVR_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CMD_QUEUE_OVR_FAIL, 1 );
				break;  
				
			case (SMSG_SC_CMD_COMPLETE_OVR_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CMD_COMPLETE_OVR_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CMD_COMPLETE_OVR_FAIL, 1 );
				break;  
			
			case (SMSG_SC_CMD_MEM_MALLOC_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CMD_MEM_MALLOC_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_MEM_MALLOC_FAIL, 1 );
				break;  

			case (SMSG_SC_BAD_PIXEL_FAIL_B | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_BAD_PIXEL_FAIL_B(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_BAD_PIXEL_FAIL_B, 1 );
				break;    

			case MSG_A3MERGE_NOTIFY_MERGE_END:
				DBG_PRINTF_SDH_DEBUG("scanman received MSG_A3MERGE_NOTIFY_MERGE_END for Merge\n");
				merge_state.merge_progressState = MERGE_PROGRESS_NONE;
				break;

            case (SMSG_SCAN_MECH_IOCTL | MESSAGE_FROM_SCANTASK_FLAG) :
                DBG_PRINTF_SDH_DEBUG( "scanman received mech ioctl response in state=%d\n", g_curr_state );

                smirb = sys_msg.param3;

                /* is this one of my smirbs? */
                scanman_smirb_sanity( smirb );

                if( smirb->scerr==SCANERR_NONE ) {
                    scanman_observer_notify_event( SCANMAN_EVENT_DEV_REQUEST_SUCCESS, smirb->id );
                }
                else {
                    scanman_observer_notify_event( SCANMAN_EVENT_DEV_REQUEST_FAIL, smirb->id );
                }
                scanman_smirb_free( &smirb );

				/* For job cancel in A3/DLT Merge (1st page scan end & move paper into back) */
				if(merge_state.merge_progressState == MERGE_PROGRESS_FIRST_PAGE_DONE) {					
					scantask_done = TRUE;
					STATE_TRANSITION( SMSTATE_IDLE );
#if HAVE_SCAN_ERASE_ROLLER
					//Change state of 'roller erase'
					if( rollerErase_state.enable == 1 ) {
						scanman_clear_roller_erase();
					}
#endif
				}
                break;
			
			case (SMSG_SC_SCAN_ASSERT_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("(blocking) SMSG_SC_SCAN_ASSERT_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_SCAN_ASSERT_FAIL, 1 );
				break;
            
            case (SMSG_SC_PIPE_WAIT_TIMEOUT | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("(blocking) SMSG_SC_PIPE_WAIT_TIMEOUT(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_PIPE_WAIT_TIMEOUT, 1 );
				break;
				
            default:
                /* assert here so we can get a quick notice if we get any weird
                 * messages we should handle in the cancel state
                 */
				DBG_PRINTF_SDH_DEBUG( "scanman cancel received known message msgType=%d\n", sys_msg.msgType);
				XASSERT( 0, sys_msg.msgType );
				break;
        }
    } /* end while */

    DBG_PRINTF_SDH_DEBUG( "%s done\n", __FUNCTION__ );
}

/**
 * \brief send message to job manager acking our cancel message 
 *
 * I have no clue what weird_unknown_parameter is but it comes from the cancel
 * message and we have to send it along in the ack.
 *
 * \author David Poole
 * \date 12-Jun-2007
 *
 */

static void
ack_cancel_msg( void *weird_unknown_parameter )
{
    MESSAGE send_msg;
    error_type_t err;

    /* tell job manager we've successfully cancelled */
    send_msg.msgType = MSG_CANCELJOB;
    send_msg.param1 = SYS_ACK;
    send_msg.param2 = SCANMANID;
    send_msg.param3 = weird_unknown_parameter;
    err = SYMsgSend( SJMID, &send_msg);
    XASSERT( err==OK, err );
}


static void
free_recipe_msg( CURRENT_RESOURCE *job_resources )
{
    MESSAGE send_msg;
    error_type_t err;

    send_msg.msgType = MSG_FREERECIPE;
    send_msg.param1 = 0;
    send_msg.param2 = 0;
    send_msg.param3 = job_resources;
    err = SYMsgSend( SJMID, &send_msg );
    XASSERT( err==OK, err );
}

/**
 * \brief Finish the scanman cancel
 *
 * Finish the cancel process.  ACK the System Job Manager, release my resources,
 * sanity check Scantask.
 *
 * \author David Poole
 * \date 10-Nov-2005
 *
 */

static void cancel_scanman( void *weird_unknown_parameter, 
                            CURRENT_RESOURCE *job_resources )
{
//    ULONG msgcnt;

    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    /* tell job manager we've successfully cancelled */
    ack_cancel_msg( weird_unknown_parameter );

    /* release myself as a resource */
    free_recipe_msg( job_resources );

    /* our message queue should be EMPTY (this is not foolproof; we could be
     * racing something)
     */
    /* davep 07-Apr-2006 ; tweak for bug 928  */
//    msgcnt = get_queue_pending( &scanman_msgq );
//    XASSERT( msgcnt==0, msgcnt );

    /* davep 27-Jun-2009 ; XXX temp remove this assert; hitting it with a cancel
     * during N-UP. Not sure this assert is valid anymore (SMECH_POLL?)
     */
    /* it's a bit rude to be peeking into scantask's message queue but we need
     * to make sure scantask is idle as well (again, this is not foolproof)
     */
//    msgcnt = scantask_get_queue_pending();
//    XASSERT( msgcnt==0, msgcnt );

    DBG_PRINTF_SDH_DEBUG( "%s done\n", __FUNCTION__ );
}

/**
 * \brief  Cancel the current job
 *
 * Sends the "cancel job" message to the system job manager.
 *
 * \author David Poole
 * \date 26-Jan-2007
 *
 */

static void scanman_cancel_job( void )
{
    MESSAGE sys_msg;
    error_type_t err;

    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    sys_msg.msgType = MSG_CANCELJOB;
    sys_msg.param1 = SYS_REQUEST;
    sys_msg.param2 = 0;
    sys_msg.param3 = (void*)e_Scanner;

    err = SYMsgSend( SJMID, &sys_msg );
    XASSERT( err==OK, err );
}

/**
 * \brief  register scanner with The System
 *
 * Copy/Paste code from ScanManInit() after a race condition was discovered. I
 * Have no idea what this code means or what it does.
 *
 * The Scantask thread is now dynamically allocated and isn't known until the
 * thread is fully up and running.  We now have to wait until Scantask is fully
 * running before calling the ResourceRegister().
 *
 * \author David Poole
 * \date 08-Jul-2008
 *
 */

static void register_scanman_with_system( void )
{
    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );

    /* everyone who allocates memory must be in the list of threads registered
     * with the resource manager
     */

    /* davep 29-Dec-2008 ; (Bug 11386) Don't register Scantask as a resource
     * anymore. No longer necessary since we went to the new memory manager. Not
     * registering Scantask means we can register ScanMan before the entire
     * system is running. 
     * */
    rm_register(e_Scanner, SCANMANID);
}

/**************************************************
 * Function name   :  posix_initialize
 *    returns      :  none
 * Created by      :  David Poole
 * Date created    :  26-Aug-2008
 * Description     : 
 *
 *  Tell tx_posix to initialize the ThreadX fake POSIX subsystem.
 *
 * Notes           : 
 *
 **************************************************/

//#ifdef HAVE_TX_POSIX_2008
//static void posix_initialize( void )
//{
//    UINT txretcode;

//    txretcode = tx_posix_initialize();
//    XASSERT( txretcode==TX_SUCCESS, txretcode );
//}
//#endif

/**
 * \brief  Do the steps necessary for scanman's clean job completion.
 *
 * By clean, I mean "not a cancel". 
 *
 *
 * \author David Poole
 * \date 16-Nov-2008
 *
 */

static void finish_job( CURRENT_RESOURCE **p_job_resources )
{
    CURRENT_RESOURCE *job_resources;

    /* use a temporary so code is a little less pointer confusing */
    PTR_ASSIGN( job_resources, *p_job_resources );

    ASSERT( job_resources != NULL );

    /* davep 17-Oct-2007 ; Bug 7498 ; disable possible calibration
     * between pages of a single job ; turn it back on now that the
     * job is done
     */
    scan_set_config_no_calcache_timeout( FALSE );

    /* release myself as a resource */
    free_recipe_msg( job_resources );

    /* we're now released */
    job_resources = NULL;
}

#ifdef FAKE_DUAL_SCAN
/**
 * \brief  Duplicate a data message payload into a newly malloc'd buffer.
 *
 * Created to mimic dual scan (sensor on both side of paper) by sending the
 * same data twice. Because the downstream code will always free() the data,
 * cannot simply send the same pointer twice.
 *
 * \author David Poole
 * \date 16-Apr-2013
 */

static scan_err_t make_dup_data_msg( MESSAGE *msg, uint32_t bytes_per_row ) 
{
    int datalen;
    uint16_t num_rows;
    scan_data_type dtype;
    uint8_t *src_ptr, *dst_ptr;
    int malloc_fail_counter;
    bool last;

    msg_scan_planedata_decode( msg, &num_rows, &dtype, &src_ptr, &last );

    msg->param2 |= SMSG_DATA_SET_PAGE_SIDE( 1 );

    datalen = num_rows * bytes_per_row;

    /* dup the data */
    malloc_fail_counter= 0;
    while( 1 ) {
        dst_ptr = MEM_MALLOC_ALIGN( datalen, cpu_get_dcache_line_size() );
        if( dst_ptr ) {
            break;
        }

        malloc_fail_counter += 1;
        if( malloc_fail_counter > 10 ) {
            DBG_PRINTF_SDH_ERR( "%s unable to allocate bytes=%d; cancelling scan\n", 
                    __FUNCTION__, datalen);
            scantask_cancel_msg();
            return SCANERR_OUT_OF_MEMORY;
        }
        posix_sleep_seconds(1);
    }

    memcpy( dst_ptr, src_ptr, datalen );
    PTR_ASSIGN(msg->param3,dst_ptr);
    return SCANERR_NONE;
}
#endif

#if HAVE_SCAN_ERASE_ROLLER
static scan_err_t scanman_setup_roller_erase(struct scanvars *scanvar, UINT32 jobType)
{
	MESSAGE send_msg;
	error_type_t err;
	memset( &send_msg, 0, sizeof(send_msg) );
	
	DBG_PRINTF_SDH_DEBUG("%s JobType(%d)\n", __FUNCTION__, jobType);
	
	send_msg.msgType = MSG_ERASE_ROLLER_SET_PARAM;
	send_msg.param1 = jobType;
	send_msg.param2 = g_roller_adjust_y_value;
	send_msg.param3 = scanvar;					
	err = SYMsgSend( ERASE_ROLLER_ID, &send_msg );
	XASSERT( err==OK, err );
	
	rollerErase_state.scanJobType = jobType;
	
	return SCANERR_NONE;
}

static scan_err_t scanman_clear_roller_erase()
{
	rollerErase_state.scanJobType = ERASE_JOB_TYPE_NONE;
	DBG_PRINTF_SDH_DEBUG("%s JobType (To:%d)\n", __FUNCTION__, rollerErase_state.scanJobType);

	return SCANERR_NONE;
}
#endif

/**
 * \brief scanman_scanner_counter_clear
 *
 * \author Yhlee
 * \date 
 *
 **/
static scan_err_t scanman_scanner_counter_clear()
{
	scanman_stats.page_ends = 0;

	return SCANERR_NONE;	
}

/**
 * \brief scanman_scanner_counter_up
 *
 * \author Yhlee
 * \date 
 *
 **/
static scan_err_t scanman_scanner_counter_up()
{
	++scanman_stats.page_ends;
	
	return SCANERR_NONE;
}

/**
 * \brief scanman_scanner_counter_read
 *
 * \author Yhlee
 * \date 
 *
 **/
uint32_t scanman_scanner_counter_read()
{	
	return scanman_stats.page_ends;
}

/**
 * \brief scanman_scanner_counter_write
 *
 * \author Yhlee
 * \date 
 *
 **/
static scan_err_t scanman_scanner_counter_write( struct scanvars *scan_var )
{
	uint8_t documentSource = 0;
	if(scan_var->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED) {
		if(rollerErase_state.scanJobType == ERASE_JOB_TYPE_NORMAL) {
			documentSource = FB_SCAN_COUNT;			
		}
		else {
			documentSource = FB_A3_SCAN_COUNT;
		}
	}
	else if(scan_var->doc_src == SCAN_DOCUMENT_SOURCE_ADF) {	
		documentSource = ADF_SIMPLEX_SCAN_COUNT;
	}
	else if(scan_var->doc_src == SCAN_DOCUMENT_SOURCE_ADF_DUPLEX) {
		documentSource = ADF_DUPLEX_SCAN_COUNT;
	}
	scanman_observer_notify_event(SCANMAN_EVENT_PAGE_END_COUNT, documentSource);	
	
	return SCANERR_NONE;
}


void scanman_set_scanpapersize(uint16_t scanPaperSize)
{
	scanPaperSizeCode = scanPaperSize;
}
/**
 * \brief Nofity scan job end msg to SSM with userID, job type and scan count info.
 *
 * \author Lalit
 * \date	20140506
 *
 **/
static void scanman_scanner_notify_jobend( struct scanvars *pScan_var, CURRENT_RESOURCE *pResourcePtr )
{
	SYS_UI_MESSAGE msg;
	error_type_t err;

	if(pScan_var){
		msg.msgSender = SCANMANID;
		msg.cmd = CMD_INT_JOB_DONE_INFO;
		memset(msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);

		stIC_JobDoneInfo stJobDoneInfo = {0};
		//stJobDoneInfo.jobNum = pScan_var->id;
		JOB_RESOURCES *tempJobResourcePtr = ((CURRENT_RESOURCE *)pResourcePtr)->JobResources;
		DBG_PRINTF_CRIT("Resource=0x%08X userID Address=0x%08X\n", tempJobResourcePtr, tempJobResourcePtr->UserId);

		strcpy(stJobDoneInfo.userID, tempJobResourcePtr->UserId);
		stJobDoneInfo.jobNum = tempJobResourcePtr->JobId;
		DBG_PRINTF_CRIT("userID=%s strlen=%d\n", stJobDoneInfo.userID, strlen(stJobDoneInfo.userID));

		stJobDoneInfo.jobDocType = tempJobResourcePtr->JobType;
		stJobDoneInfo.totalPages = scanman_scanner_counter_read();

		if(pScan_var->doc_src == SCAN_DOCUMENT_SOURCE_ADF_DUPLEX) {
			stJobDoneInfo.duplex = SCAN_DOCUMENT_DUPLEX;
		}
		else {
			stJobDoneInfo.duplex = SCAN_DOCUMENT_SIMPLEX;
		}
		
		stJobDoneInfo.paperSize = scanPaperSizeCode;
		
		DBG_PRINTF_CRIT("dpx=%d size=%d\n", stJobDoneInfo.duplex, stJobDoneInfo.paperSize);
		
		memcpy(msg.data, &stJobDoneInfo, sizeof(stIC_JobDoneInfo));
		if(tempJobResourcePtr->JobType != ejob_ScanFax)
		{
			err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
		}
		else
		{
			MESSAGE faxmsg;
			void *stJobDoneInfoFax = MEM_MALLOC(sizeof(stIC_JobDoneInfo));
			memcpy(stJobDoneInfoFax, &stJobDoneInfo, sizeof(stIC_JobDoneInfo));
			faxmsg.msgType = FAXAPP_MSG_SCAN_JOB_INFO;
			faxmsg.param3 = stJobDoneInfoFax;
			err = SYMsgSend(SCANTOFAXID, &faxmsg);
		}
		XASSERT( err==OK, err );
	}
}

/**
 * \brief ScanMan task
 *
 *
 * \author Brad Smith
 * \date 
 *
 *
 **/

static void* scanman_task(void* unused)
{
    UINT pxretcode;
    MESSAGE sys_msg;
    error_type_t err;
    CURRENT_RESOURCE *job_resources;
    scan_err_t scerr;
    ScannerHWError scantask_hw_err;
    struct scanvars *sv;
    struct scanvars *cached_sv;
    uint32_t running_scan_task_id;
    struct scanman_ioctl_request_block *smirb;
    uint32_t tmp_task_id;
    uint32_t pixels_per_row_padded, pixels_per_row, total_rows,
             bits_per_pixel, bytes_per_row;

    /* wait for rest of system to become fully initialized */
    SysWaitForInit();
    
    scerr = scanman_obs_navel_gazer_init();
    /* if my test/debug navel gazer fails to init, we don't care */

    scanman_observer_notify_event( SCANMAN_EVENT_INITIALIZING, 1 );

    scan_cmdline_init();

    // Start scan worker task.
    scerr = scantask_init( scanman_msg_send );
    XASSERT( scerr==SCANERR_NONE, scerr );

    scantask_hw_err = ScannerHWError_None;

    cached_sv = NULL;

    memset( &sys_msg, 0, sizeof(sys_msg) );
    
    // Wait for scantask to initialize.
    // Don't do anything else until that happens.
    while( sys_msg.msgType != (SMSG_SCAN_INIT_DONE | MESSAGE_FROM_SCANTASK_FLAG) )
    {
        pxretcode = posix_wait_for_message( scanman_msgq, (char *)&sys_msg, sizeof(MESSAGE),
                                            POSIX_WAIT_FOREVER );
        XASSERT( pxretcode == 0, pxretcode );

        switch( sys_msg.msgType ) {
            case (SMSG_SCAN_INIT_DONE | MESSAGE_FROM_SCANTASK_FLAG) :
                /* check for an error */
                scantask_hw_err = (ScannerHWError)sys_msg.param1;

                if( scantask_hw_err != ScannerHWError_None ) {
                    DBG_PRINTF_SDH_ERR( "%s scantask failed to init! err=%d\n", 
                                __FUNCTION__, scantask_hw_err );
                    scanman_observer_notify_event( SCANMAN_EVENT_SCAN_OFFLINE, 1 );
                }
                else {
                    /* scantask is up and running successfully */
                    DBG_PRINTF_SDH_DEBUG( "%s scantask initialized successfully\n", __FUNCTION__ );
                    /* the state transition to idle below will send the observer notify
                     * event 
                     */
                    //send a different event to let UI knows that boot initialization has finished
                    scanman_observer_notify_event( SCANMAN_EVENT_INITIALIZED, 1 );
                }

                /* Note we continue on from here whether or not Scantask
                 * successfully initialized. Why? Well, why not? What else should we
                 * do? 
                 */
                break;

            /* davep 04-Feb-2011 ; add handling of paper event; need this early
             * in the boot process (event before INIT_DONE) so the hardware can
             * send us an initial state
             */
            case (SMSG_SCAN_ADF_PAPER_EVENT | MESSAGE_FROM_SCANTASK_FLAG) :
            	DBG_PRINTF_SDH_DEBUG( "(init) %s scanman received SMSG_SCAN_ADF_PAPER_EVENT in state=%d %d %d\n",
            					 __FUNCTION__, g_curr_state, sys_msg.param1, sys_msg.param2 );
                handle_paper_event_msg( sys_msg.param1, sys_msg.param2 );
                break;

			case (SMSG_CAL_CALIBRATION_IN_PROGRESS | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG( "(init) %s scanman received SMSG_CAL_CALIBRATION_IN_PROGRESS in state=%d %d %d\n",
            					 __FUNCTION__, g_curr_state, sys_msg.param1, sys_msg.param2 );
				scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_PROCESSING;
				scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CALIBRATION_RUNNING, 1 );
				break;

			case (SMSG_CAL_CALIBRATION_DONE | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG( "(init) %s scanman received SMSG_CAL_CALIBRATION_DONE in state=%d %d %d\n", 
            					 __FUNCTION__, g_curr_state, sys_msg.param1, sys_msg.param2 );
				scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_NONE;
				scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CALIBRATION_COMPLETE, 1 );
				break;

            default : 
                DBG_PRINTF_SDH_ERR( "%s unwanted message msgType=%d param1=%d param2=%d param3=%p\n",
                            __FUNCTION__, sys_msg.msgType, sys_msg.param1,
                            sys_msg.param2, sys_msg.param3 );
                break;
        }
    }

#ifdef HAVE_ACL
    registerScanACLCmds();
#endif

    job_resources = NULL;

    STATE_TRANSITION(SMSTATE_IDLE);

    icefileapp_onetime_init();

    while( 1 )
    {
        pxretcode = posix_wait_for_message( scanman_msgq, (char *)&sys_msg, sizeof(MESSAGE),
                                            POSIX_WAIT_FOREVER );
        XASSERT( pxretcode == 0, pxretcode );

        if( pxretcode != 0 ) {
            continue;
        }
                
		/*
		 ************************************************************************************************************************
		 * Only Regarding Merge Application (SINDOH Added for A3/DLT/Legal Scan,Copy)
		 * 		- When scanman is in A3/DLT/Legal merge state, bypass belows message to 'A3_MERGE_ID'
		 * 			-SMSG_SCAN_SIZE
		 * 			-SMSG_PAGE_START
		 * 			-SMSG_PAGE_DATA
		 * 			-SMSG_PAGE_END
		 * 			-SMSG_SCAN_END
		 * 			-SMSG_SCAN_READY
		 ************************************************************************************************************************
		 */
		if(merge_state.merge_progressState != MERGE_PROGRESS_NONE) {
			MODULE_ID	msgDest;
#if HAVE_SCAN_ERASE_ROLLER
			if(rollerErase_state.enable == 1) {
				msgDest = ERASE_ROLLER_ID;
			}
			else {
				msgDest = A3_MERGE_ID;
			}
#else
			msgDest = A3_MERGE_ID;
#endif	
			switch (sys_msg.msgType)
			{
				case MSG_SCAN_DEV_REQUEST :
					DBG_PRINTF_SDH_DEBUG( "scanman received dev request in State=%d\n", g_curr_state );					
					smirb = sys_msg.param3;					
					scanman_smirb_sanity( smirb );
					scerr = scantask_ioctl_request( smirb, &tmp_task_id );
					if( scerr != SCANERR_NONE ) {
						DBG_PRINTF_SDH_DEBUG( "%s scantask refused request id=%d scerr=%d\n", __FUNCTION__, smirb->id, scerr );
						scanman_observer_notify_event( SCANMAN_EVENT_DEV_REQUEST_REFUSE, smirb->id );
						scanman_smirb_free( &smirb );
					}
					else { 
						DBG_PRINTF_SDH_DEBUG( "%s queued ioctl request id=%d scantaskid=%d\n", __FUNCTION__, smirb->id, tmp_task_id );
					}
					continue;
					break;
				case MSG_SCAN_JOBSTART:
					//Only 1st page job start is executed in original 'scanman', 2nd page job start is filtered for merge
					if(merge_state.merge_progressState == MERGE_PROGRESS_FIRST_PAGE_DONE) {
						DBG_PRINTF_SDH_DEBUG("scanman received MSG_SCAN_JOBSTART for 2nd Page\n");
						//Request next page Scan
						scan_msg_t send_msg;
						error_type_t err;
						memset( &send_msg, 0, sizeof(scan_msg_t) );
						send_msg.msgType = MSG_A3MERGE_REQUEST_2ND_JOB_START;
						err = scanman_msg_send(&send_msg);
						XASSERT( err==OK, err );
						continue;
						break;
					}
					else {
						DBG_PRINTF_SDH_DEBUG("scanman received MSG_SCAN_JOBSTART for 1st Page\n");
						break;
					}
					break;
				case (SMSG_SCAN_SIZE | MESSAGE_FROM_SCANTASK_FLAG):
					DBG_PRINTF_SDH_DEBUG("scanman received SMSG_SCAN_SIZE for Merge\n");
					err = SYMsgSend(A3_MERGE_ID, &sys_msg );
					XASSERT( err==OK, err );
#if HAVE_SCAN_ERASE_ROLLER
					if(rollerErase_state.enable == 1) {
						//Setup Erase Roller
						scanman_setup_roller_erase(sv, ERASE_JOB_TYPE_MERGE);

						//Send Scan Size
						sys_msg.msgType = SMSG_SCAN_SIZE;
						err = SYMsgSend( ERASE_ROLLER_ID, &sys_msg);
						XASSERT( err==OK, err );
					}
#endif
					continue;
					break;
				case (SMSG_PAGE_START | MESSAGE_FROM_SCANTASK_FLAG):
					DBG_PRINTF_SDH_DEBUG("scanman received SMSG_PAGE_START for Merge\n");
					err = SYMsgSend(msgDest, &sys_msg );
					XASSERT( err==OK, err );
					continue;
					break;
				case (SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG):
					//DBG_PRINTF_SDH_DEBUG("scanman received SMSG_PAGE_DATA for Merge\n");
					err = SYMsgSend(msgDest, &sys_msg );
					XASSERT( err==OK, err );		
					continue;
					break;
				case (SMSG_PAGE_END | MESSAGE_FROM_SCANTASK_FLAG):
					DBG_PRINTF_SDH_DEBUG("scanman received SMSG_PAGE_END for Merge\n");
					err = SYMsgSend(msgDest, &sys_msg );
					XASSERT( err==OK, err );
					continue;
					break;
				case (SMSG_SCAN_END | MESSAGE_FROM_SCANTASK_FLAG):
					DBG_PRINTF_SDH_DEBUG("scanman received SMSG_SCAN_END for A3Merge, pageNum:%d, ResultSize:%d\n", merge_state.merge_scannedPageNum, merge_state.merge_resultSize);
					merge_state.merge_scannedPageNum++;
					if(merge_state.merge_resultSize == A3MERGE_LG) {
						if(merge_state.merge_scannedPageNum == 1) {
							merge_state.merge_progressState = MERGE_PROGRESS_FIRST_PAGE_DONE;
						}
						else if(merge_state.merge_scannedPageNum == 2) {
							merge_state.merge_progressState = MERGE_PROGRESS_SECOND_PAGE_DONE;
						}
					}
					else {
						if(merge_state.merge_scannedPageNum == 1) {							
							merge_state.merge_progressState = MERGE_PROGRESS_FIRST_PAGE_DONE;
						}
						else if(merge_state.merge_scannedPageNum == 2) {
							//Move to Front the documnet set on the FB Glass for eject the paper.
							uint32_t req_id;
							if((merge_state.merge_resultSize == A3MERGE_A3) || (merge_state.merge_resultSize == A3SEPARATE_A3)){
								scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, MERGE_IOCTL_PARAM_A3_EJECT, NULL, &req_id);
							}
							else if((merge_state.merge_resultSize == A3MERGE_DLT) || (merge_state.merge_resultSize == A3SEPARATE_DLT)) {
								scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, MERGE_IOCTL_PARAM_DLT_EJECT, NULL, &req_id);
							}

							if( scerr == SCANERR_NONE ) {
								DBG_PRINTF_SDH_DEBUG("scanmech_move_to_A3scanpoint FOWARD > direction:1 : Success\n");
							}
							else {
								DBG_PRINTF_SDH_DEBUG("scanmech_move_to_A3scanpoint FOWARD : FAIL : %d\n", scerr);						
							}
							merge_state.merge_progressState = MERGE_PROGRESS_SECOND_PAGE_DONE;
						}
					}
					err = SYMsgSend(msgDest, &sys_msg );
					XASSERT( err==OK, err );
					continue;
					break;
				case (SMSG_SCAN_READY | MESSAGE_FROM_SCANTASK_FLAG):
					DBG_PRINTF_SDH_DEBUG("scanman received SMSG_SCAN_READY for Merge\n");
					err = SYMsgSend(msgDest, &sys_msg );
					XASSERT( err==OK, err );
					continue;
					break;
				case (MSG_A3MERGE_REQUEST_2ND_JOB_START  | MESSAGE_FROM_SCANTASK_FLAG) :
					DBG_PRINTF_SDH_DEBUG("scanman received MSG_A3MERGE_REQUEST_2ND_JOB_START for Merge\n");
					scerr = scantask_scan_start( sv, &running_scan_task_id );
					XASSERT( scerr==0, scerr );
					continue;
					break;
				case MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START:
					DBG_PRINTF_SDH_DEBUG("scanman received MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START for Merge\n");
					if(sys_msg.param1 == A3MERGE_STATUS_1PageEnd) {
						if(merge_state.merge_resultSize == A3MERGE_LG) {
							/*
							 *******************************************************************************************************************
							 * For Legal Merge 
							 *		- When 1st Page scan end : send 'MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START' message to Applicaiton
							 *******************************************************************************************************************
							 */
							if(merge_state.merge_scannedPageNum == 1) {
								DBG_PRINTF_SDH_DEBUG("send_msg : MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START\n");
								MESSAGE send_msg;
								error_type_t err;
								memset( &send_msg, 0, sizeof(send_msg) );
								send_msg.msgType = MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START;
								err = SYMsgSend( job_resources->Destination, &send_msg);
								XASSERT( err==OK, err );
							}
						}
						else {
							/*
							 *******************************************************************************************************************
							 * For A3/DL Merge
							 * 		- When 1st Page scan end
							 * 			1) Move A3/DLT original document into back using Mech Control
							 * 			2) Request 2nd page scan
							 *******************************************************************************************************************
							 */
							if(merge_state.merge_scannedPageNum == 1) {
								//Move to Back the documnet set on the FB Glass for next FB Scan.
								uint32_t req_id;
								if ((merge_state.merge_resultSize == A3MERGE_A3) || (merge_state.merge_resultSize == A3SEPARATE_A3) ){
									scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, MERGE_IOCTL_PARAM_A3_TO_BACK, NULL, &req_id);
								}
								else if((merge_state.merge_resultSize == A3MERGE_DLT) || (merge_state.merge_resultSize == A3SEPARATE_DLT)){
									scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, MERGE_IOCTL_PARAM_DLT_TO_BACK, NULL, &req_id);
								}
								if( scerr == SCANERR_NONE ) {
									DBG_PRINTF_SDH_DEBUG("scanmech_move_to_A3scanpoint BACK > direction:0 : Success\n");
								}
								else {
									DBG_PRINTF_SDH_DEBUG("scanmech_move_to_A3scanpoint BACK : FAIL : %d\n", scerr);						
								}
								//Request Next Flatbed Scan
								DBG_PRINTF_SDH_DEBUG("scanman send MSG_A3MERGE_REQUEST_2ND_JOB_START\n");
								
								//Request next page Scan
								scan_msg_t send_msg;
								error_type_t err;
								memset( &send_msg, 0, sizeof(scan_msg_t) );
								send_msg.msgType = MSG_A3MERGE_REQUEST_2ND_JOB_START;
								err = scanman_msg_send(&send_msg);
								XASSERT( err==OK, err );
							}
						}
					}
					continue;
					break;
				case MSG_A3MERGE_NOTIFY_MERGE_END:
					DBG_PRINTF_SDH_DEBUG("scanman received MSG_A3MERGE_NOTIFY_MERGE_END for Merge\n");
					merge_state.merge_progressState = MERGE_PROGRESS_NONE;
					if(sys_msg.param1 == A3MERGE_STATUS_RollerDetected) {
#if 0
						MESSAGE send_msg;
						error_type_t err;
						memset( &send_msg, 0, sizeof(send_msg) );
						send_msg.msgType = MSG_A3MERGE_NOTIFY_MERGE_END;
						send_msg.param1 = A3MERGE_STATUS_RollerDetected;
						err = SYMsgSend( job_resources->Destination, &send_msg);
						XASSERT( err==OK, err );
#else
						handle_paper_event_msg(SCAN_FB_PAPER_EMPTY, 1);
#endif
					}
					continue;
					break;
				default:
					DBG_PRINTF_SDH_DEBUG("scanman message filtered. not for Merge\n");
					break;
			}
		}

#if HAVE_SCAN_ERASE_ROLLER
		if( rollerErase_state.enable == 1 && rollerErase_state.scanJobType == ERASE_JOB_TYPE_NORMAL) {
			//DBG_PRINTF_SDH_DEBUG("scanman received SMSG_#### for Erase, ScanJobType:(%d)\n", rollerErase_state.scanJobType);
			switch (sys_msg.msgType)
			{
			case (SMSG_PAGE_START | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("scanman received SMSG_PAGE_START for Erase\n");
				err = SYMsgSend(ERASE_ROLLER_ID, &sys_msg );
				XASSERT( err==OK, err );
				continue;
				break;
			case (SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG):
				//DBG_PRINTF_SDH_DEBUG("scanman received SMSG_PAGE_DATA for Erase\n");
				err = SYMsgSend(ERASE_ROLLER_ID, &sys_msg );
				XASSERT( err==OK, err );
				continue;
				break;
			case (SMSG_PAGE_END | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("scanman received SMSG_PAGE_END for Erase\n");
				err = SYMsgSend(ERASE_ROLLER_ID, &sys_msg );
				XASSERT( err==OK, err );
				continue;
				break;
			case (SMSG_SCAN_END | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("scanman received SMSG_SCAN_END for Erase\n");
				err = SYMsgSend(ERASE_ROLLER_ID, &sys_msg );
				XASSERT( err==OK, err );
				continue;
				break;
			case (SMSG_SCAN_READY | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("scanman received SMSG_SCAN_READY for Erase\n");
				err = SYMsgSend(ERASE_ROLLER_ID, &sys_msg );
				XASSERT( err==OK, err );
				continue;
				break;
			default:
				//DBG_PRINTF_SDH_DEBUG("scanman received unknownmessage for Erase\n");
				break;
			}
		}
#endif

        switch (sys_msg.msgType)
        {
			/*
			******************************************************************************************
			* MSG_RESOURCES
			******************************************************************************************
			*/
            case MSG_RESOURCES:
                DBG_PRINTF_SDH_DEBUG("scanman received resources in state=%d\n", g_curr_state );

                DBG_PRINTF_SDH_DEBUG("%s resource msg: param1=%#lx param2=%#lx param3=%p\n", 
                            __FUNCTION__, sys_msg.param1, sys_msg.param2,
                            sys_msg.param3 );

                /* make sure we correctly ended the last job */
                XASSERT( g_curr_state==SMSTATE_IDLE, (UINT32)g_curr_state );
                XASSERT( job_resources==NULL, (UINT32)job_resources );

                job_resources = (CURRENT_RESOURCE *) sys_msg.param3;
                DBG_PRINTF_SDH_DEBUG("%s src=%d dst=%d\n", __FUNCTION__, 
                            job_resources->Source,
                            job_resources->Destination );

                STATE_TRANSITION( SMSTATE_STARTING );
                break; 

			/*
			******************************************************************************************
			* MSG_ACKRECIPE
			******************************************************************************************
			*/
            case MSG_ACKRECIPE :
                DBG_PRINTF_SDH_DEBUG("scanman received ackrecipe in state=%d\n", g_curr_state );

                /* ignore for now; this message is from the System Job Manager
                 * telling us all Resource messages have been sent (avoids race
                 * conditions)
                 */

                /* davep 17-Jan-2012 ; don't start job if we're in an
                 * error state 
                 */
                if( !scanner_is_alive() ) {
                    DBG_PRINTF_SDH_ERR( "%s scanner is offline hwerr=%d\n", __FUNCTION__, scan_get_hwerr() );
                    scanman_cancel_job();
                    break;
                }

                /* davep 07-Apr-2006 ; bug 928; gently ignore unwanted messages */
                if( g_curr_state!=SMSTATE_STARTING ) {
                    DBG_PRINTF_SDH_ERR( "%s unexpected message type=%d in state=%d\n", 
                                __FUNCTION__, sys_msg.msgType, g_curr_state );
                    SCANMAN_ASSERT(0);
                    break;
                }
                
				fskipfail = false;
                /* no need to change state */
                break;

			/*
			******************************************************************************************
			* MSG_SCAN_JOBSTART
			******************************************************************************************
			*/
            case MSG_SCAN_JOBSTART:
                DBG_PRINTF_SDH_DEBUG("scanman received jobstart in state=%d\n", g_curr_state );

                /* davep 17-Jan-2012 ; don't start job if we're in an
                 * error state (duplicate of same check in MSG_ACKRECIPE to
                 * handle race conditions)
                 */
                if( !scanner_is_alive() ) {
                    DBG_PRINTF_SDH_ERR( "%s scanner is offline hwerr=%d\n", __FUNCTION__, scan_get_hwerr() );
                    scanman_cancel_job();
                    break;
                }

                switch( g_curr_state ) {
                    case SMSTATE_STARTING :
                    case SMSTATE_WAITING_FOR_APP :
                        /* app wants another scan */
                        ASSERT( job_resources != NULL );

                        /* davep 11-Nov-2008 ; Bug 11161 ; require scanvar in a message */
                        ASSERT( sys_msg.param3 != NULL );
                        PTR_ASSIGN( sv, sys_msg.param3 );
                        XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
#if HAVE_SCAN_ERASE_ROLLER
						g_roller_adjust_y_value = sys_msg.param2;
						DBG_PRINTF_SDH_DEBUG("g_roller_adjust_y_value : %d\n", g_roller_adjust_y_value);
#endif
                        /* davep 02-Sep-2009 ;  claim the scantask data path for myself */
                        scanvar_set_scan_callback( sv, scanman_msg_send );

                        /* tell Scantask to get off his butt and send us some data */
#if USE_DELAY_FOR_DLT_MERGE_IOCTL
						if( (merge_state.merge_progressState == MERGE_PROGRESS_STARTED) && 
							((merge_state.merge_resultSize == A3MERGE_DLT) ||  (merge_state.merge_resultSize == A3SEPARATE_DLT) )&&
							(g_special_flag_DLT_merge_pending_jobStart == MERGE_DLT_FIRST_PAGE_IOCTL_REQUEST)
						) {
							/*
							 * For Temporary Code (2014.09.25) Regarding DLT(11x17) Merge Job (Copy, Scan, Fax)
							 * 		--> Have to setup new protocol to protect ioctl execute
							 * 		1) Correct Flow :
							 * 			1. App Send 'MSG_A3MERGE_SET_START' to ScanMan
							 * 			2. In DLT Merge state move original to back 6mm call 'scanman_dev_request()'
							 * 			3. ScanMan receive 'MSG_SCAN_DEV_REQUEST' from smirb--> ioctl executed
							 * 			4. ScanMan receive 'MSG_SCAN_JOBSTART' from App. --> scantask_scan_start()
							 * 		2) Problem Case : MSG_SCAN_JOBSTART is arrived ahead MSG_SCAN_DEV_REQUEST
							 */
							DBG_PRINTF_SDH_DEBUG("MSG_SCAN_JOBSTART arrived. But we wait until ioctl called\n");
							g_special_flag_DLT_merge_pending_jobStart = MERGE_DLT_FIRST_PAGE_JOBSTART_PENDED;
						}
						else {
							/* 
							 * When Job Start arrived, check calibration is done or not,
							 * If calibration is processing, wait until calibration is done
							 */
							if(scanman_stats.calibration_state != SMCALSTATE_CALIBRATION_NONE) {
								blocking_wait_for_calibration_done();
							}
							scerr = scantask_scan_start( sv, &running_scan_task_id );
							XASSERT( scerr==0, scerr );
						}
#else
						if(scanman_stats.calibration_state != SMCALSTATE_CALIBRATION_NONE) {
							blocking_wait_for_calibration_done();
						}
                        scerr = scantask_scan_start( sv, &running_scan_task_id );
                        XASSERT( scerr==0, scerr );
#endif

						/* clear scanner counter */
						scanman_scanner_counter_clear();

                        STATE_TRANSITION( SMSTATE_SCANNING );
                        break;

                    case SMSTATE_WAITING :
                        /* davep 11-Nov-2008 ; Bug 11161 ; require scanvar in a message */
                        ASSERT( sys_msg.param3 != NULL );

                        /* davep 01-Nov-2011 ; make sure we don't wind up with
                         * /two/ pending cached scanvars (bad bad thing
                         * happened)
                         */
                        XASSERT( cached_sv==NULL, cached_sv->id );

                        /* We get a scanvar in this message but the scanner is
                         * still busy (we have to wait for the SMSG_SCAN_READY).
                         * So save the scanvar for when we get the
                         * SMSG_SCAN_READY.
                         */
                        PTR_ASSIGN( cached_sv, sys_msg.param3 );
                        XASSERT( cached_sv->cookie==SCANVAR_COOKIE, cached_sv->cookie );

                        /* davep 01-Nov-2011 ;  claim the scantask data path for myself */
                        scanvar_set_scan_callback( cached_sv, scanman_msg_send );

                        STATE_TRANSITION( SMSTATE_WAITING_FOR_SCANTASK_2 );
                        break;

                    case SMSTATE_IDLE :
                        /* davep 21-Sep-2009 ; Bug 12224 ; race condition
                         * between scanman and copyapp on a cancel. Scanman
                         * handles his cancel, ack's the cancel, then goes back
                         * to idle. Meanwhile, copyapp has already sent the
                         * jobstart.
                         *
                         * Simply ignore the jobstart. 
                         *
                         * Note the jobstart contains alloc'd memory (the
                         * scanvar).  But the downstream thread (e.g., copyapp)
                         * is required to free that on a cancel.
                         */
                        DBG_PRINTF_SDH_DEBUG( "scanman ignore jobstart in state=%d\n", g_curr_state );
                        break;

                    case SMSTATE_WAITING_FOR_SCANTASK_2 :
                        /* davep 16-Oct-2009 ; (Bug 14574) kludge to ignore
                         * double-tap button presses causing extra PAGESTART
                         * messages.
                         *
                         * Ignore the message but verify the same scanvar is
                         * sent.
                         */
                        DBG_PRINTF_SDH_DEBUG( "%s ignore extra jobstart while in state=%d\n", 
                                    __FUNCTION__, g_curr_state );
                        ASSERT( sys_msg.param3 != NULL );
                        PTR_ASSIGN( sv, sys_msg.param3 );
                        XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
                        XASSERT( cached_sv==sv, (UINT32)sv );
                        break;

                    default :
                        /* wtf!? */
                        XASSERT( 0, g_curr_state );
                        break;
                }
                break;

			/*
			******************************************************************************************
			* MSG_SCAN_JOBEND
			******************************************************************************************
			*/
            case MSG_SCAN_JOBEND :
                DBG_PRINTF_SDH_DEBUG( "scanman received jobend in state=%d\n", g_curr_state );

                switch( g_curr_state ) {
                    case SMSTATE_WAITING :
                        STATE_TRANSITION( SMSTATE_WAITING_FOR_SCANTASK_1 );
                        break;

                    case SMSTATE_WAITING_FOR_APP :
                        /* we are now done done done; clean up ourselves with
                         * the SysJobMgr 
                         */
                        finish_job( &job_resources );
                        STATE_TRANSITION( SMSTATE_IDLE );
                        break;

                    case SMSTATE_IDLE :
                        /* davep 21-Sep-2009 ; Bug 12224 ; see commments in
                         * MSG_SCAN_JOBSTART for more detail
                         */
                        DBG_PRINTF_SDH_DEBUG( "scanman ignore jobend in state=%d\n", g_curr_state );
                        break;

                    default :
							DBG_PRINTF_SDH_DEBUG( "scanman ignore jobend in default, state=%d\n", g_curr_state );
							XASSERT( 0, g_curr_state );
                        break;
                }
                break;

			/*
			******************************************************************************************
			* MSG_CANCELJOB
			******************************************************************************************
			*/
            case MSG_CANCELJOB:
                DBG_PRINTF_SDH_DEBUG( "scanman received canceljob in state=%d\n", g_curr_state );

                /* lotsa places we can receive a cancel so easier to specify
                 * where we shouldn't get a cancel
                 */
                if( g_curr_state==SMSTATE_IDLE ) {
                    DBG_PRINTF_SDH_ERR( "%s unexpected message type=%d in state=%d\n", 
                                __FUNCTION__, sys_msg.msgType, g_curr_state );
                    SCANMAN_ASSERT(0);
                    break;
                }

                /* davep 17-Oct-2007 ; Bug 7498 ; disable possible calibration
                 * between pages of a single job ; turn it back on now that the
                 * job is done
                 */
                scan_set_config_no_calcache_timeout( FALSE );

				/* 
				 * check when Job Cancel arrived Calibration is done or not,
				 * If calibration is processing, wait until calibration is done
				 */
				if(scanman_stats.calibration_state != SMCALSTATE_CALIBRATION_NONE) {
					DBG_PRINTF_SDH_DEBUG( "scanman calibration processing so wait until done\n");
					scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_WAIT_JOBCANCEL;
					blocking_wait_for_scantask_ready();
				}

                /* Only cancel scantask if scantask is actually running.
                 * Scantask should only be running when we told it to.
                 */
                switch( g_curr_state ) {
                    case SMSTATE_SCANNING :
                        STATE_TRANSITION( SMSTATE_CANCELING );

                        /* tell scantask to cancel then wait for it to complete */
                        scan_task_status_t task_status;
                        scerr = scantask_scan_cancel( running_scan_task_id,
                                                        &task_status );

                        /* the cancel will tell us the status of the task. If
                         * the task was pending (i.e., scantask had never even
                         * seen it yet), we don't have to wait for scantask.
                         */
                        if( task_status==SCAN_TASK_STATUS_RUNNING ) {
                            XASSERT( scerr==SCANERR_NONE, scerr );
                            blocking_wait_for_scantask_ready();
                        }
                        else if (merge_state.merge_progressState != MERGE_PROGRESS_NONE)
                        {
//                            XASSERT( scerr==SCANERR_NONE, scerr );
                            if( scerr == SCANERR_NONE ) {
                                blocking_wait_for_scantask_ready();
                            }
                            else {
                                DBG_PRINTF_SDH_DEBUG( "scanman received MSG_CANCELJOB in state=%d, But ScanTask have Error=%d\n", g_curr_state, scerr );
								STATE_TRANSITION( SMSTATE_IDLE );
							}
						}
                        else {
                            /* jump straight to idle since we don't have to wait
                             * for scantask (we canceled before we even started
                             * or scantask has already completed the task we
                             * tried to cancel)
                             */
#if 1
							/* 
							 * SINDOH Added (Yhlee)
							 *  When User Cancel Request arrived at 'ScanMan' through SJM,
							 * 'ScanMan' check scantask is running or not.
							 * if scantask not running 'ScanMan' have to wait 'SCANREADY' insted of chage into 'SMSTATE_IDLE'
							 */
							blocking_wait_for_scantask_ready();
#else
							STATE_TRANSITION( SMSTATE_IDLE );
#endif
                        }

                        /* If we get here, we should have passed from the
                         * 'canceling' state to 'waiting' state then to 'idle'
                         * state.  The blocking_cancel_scantask() function blocks
                         * waiting for the SMSG_SCAN_READY.
                         */
                        XASSERT( g_curr_state==SMSTATE_IDLE, g_curr_state );
                        break;

                    case SMSTATE_FAILING :
                    case SMSTATE_WAITING :
                    case SMSTATE_WAITING_FOR_SCANTASK_1 :
                    case SMSTATE_WAITING_FOR_SCANTASK_2 :
                        /* everything is almost clean but we have to wait for
                         * scantask to go back to his ready state
                         */
                        STATE_TRANSITION( SMSTATE_WAITING_FOR_SCANTASK_1 );

                        /* Blocking wait for scantask's SMSG_SCAN_READY. We don't
                         * send the cancel to scantask because scantask is already
                         * in his blackout period. 
                         */
                        blocking_wait_for_scantask_ready();
                        XASSERT( g_curr_state==SMSTATE_IDLE, g_curr_state );
                        break;

                    default :
                        /* keep going, clean up scanman */
                        break;
                }

                /** \bug  WTF is param3? See Cory V. Atkin for all the exciting
                 * details!
                 */
                cancel_scanman( sys_msg.param3, job_resources );
#if 1
				if (merge_state.merge_progressState != MERGE_PROGRESS_NONE)
				{
					MESSAGE send_msg;
					error_type_t err;
					memset( &send_msg, 0, sizeof(send_msg) );
					send_msg.msgType = MSG_CANCELJOB;
					err = SYMsgSend(ERASE_ROLLER_ID, &send_msg );
					XASSERT( err==OK, err );
					
					err = SYMsgSend(A3_MERGE_ID, &send_msg );
					XASSERT( err==OK, err );

					merge_state.merge_progressState = MERGE_PROGRESS_NONE;
				}
#endif
                /* we're now released */
                job_resources = NULL;

                if( g_curr_state != SMSTATE_IDLE ) {
                    STATE_TRANSITION( SMSTATE_IDLE );
                }
                break;

			/*
			******************************************************************************************
			* MSG_SCAN_DEV_REQUEST
			******************************************************************************************
			*/
            case MSG_SCAN_DEV_REQUEST :
                DBG_PRINTF_SDH_DEBUG( "scanman received dev request in state=%d\n", g_curr_state );

                smirb = sys_msg.param3;

                /* verify the smirb */
                scanman_smirb_sanity( smirb );

//                /* test/debug--refuse it */
//                scanman_observer_notify_event( SCANMAN_EVENT_DEV_REQUEST_REFUSE, request_id );
//                break;

                scerr = scantask_ioctl_request( smirb, &tmp_task_id );
                if( scerr != SCANERR_NONE ) {
                    /* scantask doesn't want it so we must refuse it */
                    DBG_PRINTF_SDH_DEBUG( "%s scantask refused request id=%d scerr=%d\n", __FUNCTION__, smirb->id, scerr );
                    scanman_observer_notify_event( SCANMAN_EVENT_DEV_REQUEST_REFUSE, smirb->id );

                    scanman_smirb_free( &smirb );
                }
                else { 
                    DBG_PRINTF_SDH_DEBUG( "%s queued ioctl request id=%d scantaskid=%d\n", __FUNCTION__, 
                                smirb->id, tmp_task_id );
                }
                break;

            /*
             *
             *  The following are all messages from Scantask to me.  Need to
             *  rewrite the message then resend it to The Cloud Beyond.
             * 
             */
			/*
			******************************************************************************************
			* SMSG_SCAN_SIZE
			******************************************************************************************
			*/
            case (SMSG_SCAN_SIZE | MESSAGE_FROM_SCANTASK_FLAG) :
                DBG_PRINTF_SDH_DEBUG( "scanman received scansize in state=%d\n", g_curr_state );

                ASSERT( job_resources != NULL );
                XASSERT( g_curr_state==SMSTATE_SCANNING, (UINT32)g_curr_state );

                /* rewrite and pass on the msg to my downstream */
                sys_msg.msgType = MSG_SCAN_SIZE;
                err = SYMsgSend( job_resources->Destination, &sys_msg);
                XASSERT( err==OK, err );

                /* davep 16-Apr-2013 ; testing dual sided scan using single
                 * sided ADF; need to duplicate every buffer coming through so
                 * must know the scan size
                 */
                msg_scan_size_decode( &sys_msg, &pixels_per_row_padded,
                        &pixels_per_row, &total_rows, &bits_per_pixel );
                bytes_per_row = (bits_per_pixel/8) * pixels_per_row;
                    
#if HAVE_SCAN_ERASE_ROLLER
				//check use erase roller
				rollerErase_state.enable = SDH_eraseRoller_isUse();
				if( rollerErase_state.enable == 1 && sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED 
														&& rollerErase_state.scanJobType == ERASE_JOB_TYPE_NONE) {
					//Setup Erase Roller
					scanman_setup_roller_erase(sv, ERASE_JOB_TYPE_NORMAL);

					//Send Scan Size
					sys_msg.msgType = SMSG_SCAN_SIZE;
					err = SYMsgSend( ERASE_ROLLER_ID, &sys_msg);
					XASSERT( err==OK, err );
				}
#endif
                break;

			/*
			******************************************************************************************
			* SMSG_PAGE_START
			******************************************************************************************
			*/
            case (SMSG_PAGE_START | MESSAGE_FROM_SCANTASK_FLAG) :
            case MSG_ERASE_ROLLER_PAGE_START:
                DBG_PRINTF_SDH_DEBUG( "scanman received pagestart in state=%d\n", g_curr_state );

                /* davep 07-Jun-2009 ; adding SMSG_PAGE_START for better ADF
                 * support.
                 */
                XASSERT( g_curr_state==SMSTATE_SCANNING, (UINT32)g_curr_state );
                ASSERT( job_resources != NULL );

                sys_msg.msgType = MSG_SCAN_PAGESTART;
                err = SYMsgSend( job_resources->Destination, &sys_msg);
                XASSERT( err==OK, err );

#ifdef FAKE_DUAL_SCAN
                /* davep 16-Apr-2013 ; test/debug ; duplicate every
                 * start/end/pagedata message for a side2 experiment
                 */
                sys_msg.param1 |= SMSG_PAGE_START_SET_PAGE_SIDE(1);
                err = SYMsgSend( job_resources->Destination, &sys_msg);
                XASSERT( err==OK, err );
#endif
                break;

			/*
			******************************************************************************************
			* SMSG_PAGE_DATA
			******************************************************************************************
			*/
            case (SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG) :
			case MSG_ERASE_ROLLER_PAGE_DATA:
                /* davep 07-Apr-2006 ; bug 928; we'll still assert here
                 * because we don't want to leak memory. Also, we shouldn't get
                 * any abberant data messages from scantask because we're
                 * responsible for stopping scantask
                 */
                XASSERT( g_curr_state==SMSTATE_SCANNING, (UINT32)g_curr_state );
                ASSERT( job_resources != NULL );
                sys_msg.msgType = MSG_SCAN_PLANEDATA;
                err = SYMsgSend( job_resources->Destination, &sys_msg);
                XASSERT( err==OK, err );

#ifdef FAKE_DUAL_SCAN
                /* davep 16-Apr-2013 ; test/debug; duplicate every
                 * start/end/pagedata message for a side2 experiment
                 */
                scerr = make_dup_data_msg(&sys_msg,bytes_per_row);
                if( scerr==SCANERR_NONE ) {
                    err = SYMsgSend( job_resources->Destination, &sys_msg);
                    XASSERT( err==OK, err );
                }
#endif

                /* davep 18-May-2012 ; XXX temp debug (commenting out but
                 * leaving in code because quite useful)
                 */
#if 0
                if( 1 ) { 
                    uint16_t num_rows;
                    scan_data_type dtype;
                    uint8_t *data_ptr;
                    bool last_buffer;
                    msg_scan_planedata_decode( &sys_msg, &num_rows, &dtype, &data_ptr, &last_buffer );

                    DBG_PRINTF_SDH_DEBUG( "%s nr=%d dtype=%d data_ptr=%p last=%d\n", __FUNCTION__, num_rows, dtype, data_ptr, last_buffer );
                } 
#endif
                /* don't change state */
                break;
                			
			/*
			******************************************************************************************
			* SMSG_PAGE_END
			******************************************************************************************
			*/
            case (SMSG_PAGE_END | MESSAGE_FROM_SCANTASK_FLAG):
			case MSG_ERASE_ROLLER_PAGE_END:
                DBG_PRINTF_SDH_DEBUG( "scanman received pageend in state=%d\n", g_curr_state );
                XASSERT( g_curr_state==SMSTATE_SCANNING, (UINT32)g_curr_state );
                ASSERT( job_resources != NULL );

                sys_msg.msgType = MSG_SCAN_PAGEEND;
                err = SYMsgSend(job_resources->Destination, &sys_msg);
                XASSERT( err==OK, err );

				//Notify to increase scanner used count which will be written DB
				scanman_scanner_counter_up();

#ifdef FAKE_DUAL_SCAN
                /* davep 16-Apr-2013 ; test/debug ; duplicate every
                 * start/end/pagedata message for a side2 experiment
                 */
                sys_msg.param1 |= SMSG_PAGE_START_SET_PAGE_SIDE(1);
                err = SYMsgSend( job_resources->Destination, &sys_msg);
                XASSERT( err==OK, err );
#endif
                break;

			/*
			******************************************************************************************
			* SMSG_SCAN_END
			******************************************************************************************
			*/
            case (SMSG_SCAN_END | MESSAGE_FROM_SCANTASK_FLAG):
			case MSG_ERASE_ROLLER_SCAN_END:
                /* davep 07-Jun-2009 ; better ADF support ; use SCAN_END instead
                 * of PAGE_END to indicate end of scantask
                 */
                DBG_PRINTF_SDH_DEBUG( "scanman received scanend in state=%d\n", g_curr_state );
                
				/* 
				* SINDOH Added : 
				* (We have consider more : when Jam occured, ScanMan sometimes receive two message 'SMSG_SCAN_FAILED' and 'SMSG_SCAN_END') 
				* 	When Jam occured ScanTask send 'SMSG_SCAN_FAILED' message to ScanMan, 
				* Then ScanMan start 'scanman_cancel_job'
				* Then System Job Manager send 'MSG_CANCELJOB' to ScanMan with waiting ack message
				* Then ScanMan Send 'ack' message and 'release request' to System Job Manager
				*/
				if(( g_curr_state == SMSTATE_FAILING ) || (fskipfail == true)){
					DBG_PRINTF_SDH_DEBUG( "%s unexpected message 1 type=%d in state=%d\n", 
								__FUNCTION__, sys_msg.msgType, g_curr_state );

					/* write scanned count into DB */
					scanman_scanner_counter_write( sv );
					
					if( ( g_curr_state == SMSTATE_WAITING ) || ( g_curr_state == SMSTATE_WAITING_FOR_SCANTASK_1))
					{
						/* Send job end notification to SSM*/
						scanman_scanner_notify_jobend(sv, job_resources);
					}
					break;
				}

                /* davep 07-Apr-2006 ; bug 928; gently ignore unwanted messages */
                if( g_curr_state!=SMSTATE_SCANNING ) {
                    DBG_PRINTF_SDH_DEBUG( "%s unexpected message 2 type=%d in state=%d\n", 
                                __FUNCTION__, sys_msg.msgType, g_curr_state );
                    SCANMAN_ASSERT(0);
                    break;
                }

				/* write scanned count into DB */
				scanman_scanner_counter_write( sv );

				/* Send job end notification to SSM*/
				scanman_scanner_notify_jobend(sv, job_resources);

                /* davep 11-Nov-2008 ; Bug 11161 ; now must have a pointer to
                 * scanvar in our response
                 */
                ASSERT( sys_msg.param3 != NULL );
                sv = sys_msg.param3;
                XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

                ASSERT( job_resources != NULL );

                /* rewrite and pass on the pageend msg to my downstream */
                sys_msg.msgType = MSG_SCAN_JOBEND;
                err = SYMsgSend( job_resources->Destination, &sys_msg);
                XASSERT( err==OK, err );

                /* davep 17-Oct-2007 ; Bug 7498 ; disable calibration between
                 * pages of a single job; after the first page finishes (so the
                 * first page can trigger a cal if necessary), stop cal. When
                 * the job finshes, restart ability to cal if the cache entry
                 * has timed out.
                 */
                scan_set_config_no_calcache_timeout( TRUE );

                /* davep 07-Jun-2009 ; now we enter the blackout state -- don't
                 * send anything to Scantask!  TODO add a bunch of asserts to
                 * make sure ScanMan doesn't talk to Scantask when in the
                 * blackout
                 */
                STATE_TRANSITION( SMSTATE_WAITING );
                break;

			/*
			******************************************************************************************
			* SMSG_SCAN_READY
			******************************************************************************************
			*/
            case (SMSG_SCAN_READY | MESSAGE_FROM_SCANTASK_FLAG):
			case MSG_ERASE_ROLLER_SCAN_READY:
                DBG_PRINTF_SDH_DEBUG( "scanman received scanready in state=%d\n", g_curr_state );
#if HAVE_SCAN_ERASE_ROLLER
				//Change state of 'roller erase'
				if( rollerErase_state.enable == 1 ) {
					scanman_clear_roller_erase();
				}
#endif
                switch( g_curr_state ) {
                    case SMSTATE_FAILING :
                        /* During a failed scan, we have to sync the SCAN_READY
                         * and the SystemJobManager sending us the CANCEL. They 
                         * can arrive in either order so have to handle both.
                         */
                        STATE_TRANSITION( SMSTATE_WAITING_FOR_SJM );
                        break;

                    case SMSTATE_WAITING :
                        /* Scantask tells us he's ready. Now we wait for the App
                         * to tell us want he wants us to do.  We could get a
                         * MSG_SCAN_JOBSTART and we'll do another scan. Or we
                         * could get a MSG_SCAN_JOBEND and we're done.
                         */
                        STATE_TRANSITION( SMSTATE_WAITING_FOR_APP );
                        break;

                    case SMSTATE_WAITING_FOR_SCANTASK_1 :
                        /* We're done. Clean up ourselves with the SysJobMgr */
                        finish_job( &job_resources );
                        STATE_TRANSITION( SMSTATE_IDLE );
                        break;

                    case SMSTATE_WAITING_FOR_SCANTASK_2 :
                        /* we should have a cached scanvar from a previously
                         * received MSG_SCAN_JOBSTART 
                         */
                        ASSERT( cached_sv != NULL );

                        PTR_ASSIGN( sv, cached_sv );

                        /* make sure the callback is still us (better safe than
                         * sorry)
                         */
                        XASSERT( sv->scan_callback_fn==scanman_msg_send,
                                (UINT32)sv->scan_callback_fn );

                        /* tell Scantask to get off his butt and send us some data */
                        scerr = scantask_scan_start( sv, &running_scan_task_id );
                        XASSERT( scerr==0, scerr );

                        STATE_TRANSITION( SMSTATE_SCANNING );
                        break;

                    default :
                        /* wtf!? */
                        XASSERT( 0, g_curr_state );
                        break;
                }
                break;

			/*
			******************************************************************************************
			* SMSG_SCAN_FAILED
			******************************************************************************************
			*/
            case (SMSG_SCAN_FAILED | MESSAGE_FROM_SCANTASK_FLAG):
                DBG_PRINTF_SDH_DEBUG( "scanman received scanfailed in state=%d\n", g_curr_state );

				if (fskipfail == true)
				{
					sys_msg.msgType = MSG_SCAN_JOBEND;
					err = SYMsgSend( job_resources->Destination, &sys_msg);
					XASSERT( err==OK, err );
					STATE_TRANSITION( SMSTATE_WAITING );
					break;
				}
				
                switch( g_curr_state ) {
                    case SMSTATE_SCANNING :
                    case SMSTATE_WAITING :
                        scanman_cancel_job();
                        /* wait for SMSG_SCAN_READY and MSG_CANCEL */
                        STATE_TRANSITION( SMSTATE_FAILING );
                        break;

                    case SMSTATE_CANCELING :
                        /* wait for SMSG_SCAN_READY */
                        STATE_TRANSITION( SMSTATE_WAITING_FOR_SCANTASK_1 );
                        break;

                    /* davep 30-Apr-2009 ; handle fail message while waiting for
                     * scantask. Is happening if the PP fails after a
                     * successful scan. 
                     */
                    case SMSTATE_WAITING_FOR_SCANTASK_1 :
                        /* stay calm, don't worry. wait for the ready */
                        break;

                    default :
                        /* wtf!? */
                        XASSERT( 0, g_curr_state );
                        break;
                }
                break;

			/*
			******************************************************************************************
			* SMSG_DATA_BLOB
			******************************************************************************************
			*/
            case (SMSG_DATA_BLOB | MESSAGE_FROM_SCANTASK_FLAG):
                DBG_PRINTF_SDH_DEBUG( "scanman received data blob in state=%d\n", g_curr_state );
                /* rewrite to an agMessage/scanmsg.h message and resend */
                sys_msg.msgType = MSG_SCAN_DATA_BLOB;

                err = SYMsgSend( job_resources->Destination, &sys_msg);
                XASSERT( err==OK, err );

                /* don't change state */
                break;

			/*
			******************************************************************************************
			* SMSG_SCAN_ADF_PAPER_EVENT
			******************************************************************************************
			*/
            case (SMSG_SCAN_ADF_PAPER_EVENT | MESSAGE_FROM_SCANTASK_FLAG) :
            	DBG_PRINTF_SDH_DEBUG( "%s scanman received SMSG_SCAN_ADF_PAPER_EVENT in state=%d, %d %d\n",
            					 __FUNCTION__, g_curr_state, sys_msg.param1, sys_msg.param2 );
                handle_paper_event_msg( sys_msg.param1, sys_msg.param2 );
                break;

			/*
			******************************************************************************************
			* SMSG_SCAN_MECH_IOCTL
			******************************************************************************************
			*/
            case (SMSG_SCAN_MECH_IOCTL | MESSAGE_FROM_SCANTASK_FLAG) :
                DBG_PRINTF_SDH_DEBUG( "scanman received mech ioctl response in state=%d\n", g_curr_state );

                smirb = sys_msg.param3;

                /* is this one of my smirbs? */
                scanman_smirb_sanity( smirb );

                if( smirb->scerr==SCANERR_NONE ) {
                    scanman_observer_notify_event( SCANMAN_EVENT_DEV_REQUEST_SUCCESS, smirb->id );
                }
                else {
                    scanman_observer_notify_event( SCANMAN_EVENT_DEV_REQUEST_FAIL, smirb->id );
                }
                scanman_smirb_free( &smirb );

#if USE_DELAY_FOR_DLT_MERGE_IOCTL
				if( (merge_state.merge_progressState == MERGE_PROGRESS_STARTED) && ( (merge_state.merge_resultSize == A3MERGE_DLT) || (merge_state.merge_resultSize == A3SEPARATE_DLT) ) ) {
					if( g_special_flag_DLT_merge_pending_jobStart == MERGE_DLT_FIRST_PAGE_JOBSTART_PENDED ) {
						DBG_PRINTF_SDH_DEBUG("Pended 'MSG_SCAN_JOBSTART' execute here. state:%d\n", g_special_flag_DLT_merge_pending_jobStart);
						scerr = scantask_scan_start( sv, &running_scan_task_id );
						XASSERT( scerr==0, scerr );
					}
					else {
						DBG_PRINTF_SDH_DEBUG("MERGE_DLT_FIRST_PAGE_IOCTL_DONE! state:%d\n", g_special_flag_DLT_merge_pending_jobStart);
					}
					g_special_flag_DLT_merge_pending_jobStart = MERGE_DLT_FIRST_PAGE_IOCTL_DONE;
				}
#endif
                break;

			/*
			******************************************************************************************
			* SMSG_CAL_CALIBRATION_IN_PROGRESS
			******************************************************************************************
			*/
            case (SMSG_CAL_CALIBRATION_IN_PROGRESS | MESSAGE_FROM_SCANTASK_FLAG) :
 			    DBG_PRINTF_SDH_DEBUG("[SM_CAL] SMSG_CAL_CALIBRATION_IN_PROGRESS(SM)\n");
 			    scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_PROCESSING;
                scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CALIBRATION_RUNNING, 1 );
                break;
			/*
			******************************************************************************************
			* SMSG_CAL_CALIBRATION_DONE
			******************************************************************************************
			*/
            case (SMSG_CAL_CALIBRATION_DONE | MESSAGE_FROM_SCANTASK_FLAG) :
 			    DBG_PRINTF_SDH_DEBUG("[SM_CAL] SMSG_CAL_CALIBRATION_DONE(SM) \n");
				scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_NONE;
                scanman_observer_notify_event( SCANMAN_EVENT_SCAN_CALIBRATION_COMPLETE, 1 );
                /* send another 'scanning' status to make the status transition
                 * a little more intuitive
                 * scan running -> calibration running -> cal complete -> scan running 
                 */
                scanman_observer_notify_event( SCANMAN_EVENT_SCAN_RUNNING, 1 );
                break;
                
			/*
			******************************************************************************************
			* MSG_A3MERGE_SET_START
			******************************************************************************************
			*/
			case MSG_A3MERGE_SET_START:	//sent from Application for setup the Merge
                DBG_PRINTF_SDH_DEBUG( "scanman A3_Merge 'MSG_A3MERGE_SET_START received: (App:%d),(Size:%d)\n",sys_msg.param1, sys_msg.param2);		
				//Initialize merge state variable
				memset(&merge_state, 0x00, sizeof(merge_state));

				//Setup the parameter regarding 'Merge' & change the state of 'Merge Progress'
				merge_state.merge_requestAppSrc = sys_msg.param1;
				merge_state.merge_resultSize = sys_msg.param2;
				merge_state.merge_progressState = MERGE_PROGRESS_STARTED;	
				
				//for DLT(11'X17') move to back a little
				if ((merge_state.merge_resultSize == A3MERGE_DLT) || (merge_state.merge_resultSize == A3SEPARATE_DLT)) {
					uint32_t req_id;
					scerr = scanman_dev_request(SCANMECH_IOCTL_MOVE_TO_A3POSITION, MERGE_IOCTL_PARAM_DLT_TO_BACK_LITTLE, NULL, &req_id);
#if USE_DELAY_FOR_DLT_MERGE_IOCTL
					g_special_flag_DLT_merge_pending_jobStart = MERGE_DLT_FIRST_PAGE_IOCTL_REQUEST;
#endif					
					if( scerr == SCANERR_NONE ) {
						DBG_PRINTF_SDH_DEBUG("scanmech_move_to_DLT back to little\n");
					}
					else {
						DBG_PRINTF_SDH_DEBUG("scanmech_move_to_DLT back to little : FAIL : %d\n", scerr);												
					}
				}
				
				//Transfer the message to Merge Algorithm
				MESSAGE send_msg;
				error_type_t err;
				memset( &send_msg, 0, sizeof(send_msg) );
				send_msg.msgType = MSG_A3MERGE_SET_START;
				send_msg.param1 = sys_msg.param1;
				send_msg.param2 = sys_msg.param2;
				send_msg.param3 = sys_msg.param3;
				err = SYMsgSend(A3_MERGE_ID, &send_msg );
				XASSERT( err==OK, err );
#if HAVE_SCAN_ERASE_ROLLER
				//check use erare roller
				rollerErase_state.enable = SDH_eraseRoller_isUse();
				if(rollerErase_state.enable == 1) {
					err = SYMsgSend(ERASE_ROLLER_ID, &send_msg );
					XASSERT( err==OK, err );
				}
#endif
				break;

			/*
			******************************************************************************************
			* MSG_SCAN_POWERSAVE (Received from 'SystemService')
			******************************************************************************************
			*/
			case MSG_SCAN_POWERSAVE:
				DBG_PRINTF_SDH_DEBUG( "%s scanman received MSG_SCAN_POWERSAVE in state=%d, %d\n", __FUNCTION__, g_curr_state, sys_msg.param1);
				if(sys_msg.param1 == ESM_WAKE_UP && g_curr_state == SMSTATE_IDLE) {	
					// mod.lsh.for calibration only idle state
					uint32_t req_id;
					scerr = scanman_dev_request(SCANMECH_IOCTL_WAKE_UP_MODE, 0, NULL, &req_id);
					scanman_set_powersave_state(ESM_WAKE_UP);
				}
				else if(sys_msg.param1 == ESM_ENERGY_SAVE) {
					uint32_t req_id;
					scerr = scanman_dev_request(SCANMECH_IOCTL_SLEEP_MODE, 0, NULL, &req_id); //MotorDriver Sleep mode mod.kks 2014.08.07
					scanman_set_powersave_state(ESM_ENERGY_SAVE);
				}
				break;
				
			/*
			******************************************************************************************
			* SCAN SC
			******************************************************************************************
			*/
			//add kmg
			case (SMSG_SC_AFE_OFFSET_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_AFE_OFFSET_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_AFE_OFFSET_FAIL, 1 );
				break;
				
			case (SMSG_SC_AFE_GAIN_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_AFE_GAIN_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_AFE_GAIN_FAIL, 1 );
				break;
				
			case (SMSG_SC_AFE_SETTING_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_AFE_SETTING_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_AFE_SETTING_FAIL, 1 );
				break; 
				
			case (SMSG_SC_CIS_HOME_POSITION_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CIS_HOME_POSITION_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CIS_HOME_POSITION_FAIL, 1 );
				break; 
				
			case (SMSG_SC_WHITE_STRIP_READ_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_WHITE_STRIP_READ_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_WHITE_STRIP_READ_FAIL, 1 );
				break;       
				
			case (SMSG_SC_BLACK_STRIP_READ_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_BLACK_STRIP_READ_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_BLACK_STRIP_READ_FAIL, 1 );
				break;    
				
			case (SMSG_SC_BAD_PIXEL_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_BAD_PIXEL_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_BAD_PIXEL_FAIL, 1 );
				break;                      
				
			case (SMSG_SC_CIS_HOME_OFF_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CIS_HOME_OFF_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CIS_HOME_OFF_FAIL, 1 );
				break; 
				
			case (SMSG_SC_CIS_HOME_ON_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CIS_HOME_ON_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CIS_HOME_ON_FAIL, 1 );
				break;         			
			
			case (SMSG_SC_PIC_OVF_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_PIC_OVF_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_PIC_OVF_FAIL, 1 );
				break;
				
			case (SMSG_SC_BAD_PIXEL_FAIL_B | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_BAD_PIXEL_FAIL_B(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_BAD_PIXEL_FAIL_B, 1 );
				break;                      				  	

			case (SMSG_SC_CMD_QUEUE_OVR_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CMD_QUEUE_OVR_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CMD_QUEUE_OVR_FAIL, 1 );
				break;  
				
			case (SMSG_SC_CMD_MEM_MALLOC_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CMD_MEM_MALLOC_FAIL(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_MEM_MALLOC_FAIL, 1 );
				break;  
				
			case (SMSG_SC_CMD_COMPLETE_OVR_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_CMD_COMPLETE_OVR_FAIL(SM)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_CMD_COMPLETE_OVR_FAIL, 1 );
				break;  

			case (SMSG_SC_SCAN_ASSERT_FAIL | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_SCAN_ASSERT_FAIL(SM_BK)\n");
				scan_set_hwerr( ScannerHWError_GeneralFailure );
                scanman_observer_notify_event( SCANMAN_EVENT_SC_SCAN_ASSERT_FAIL, 1 );
				break;
				
			case (SMSG_SC_PIPE_WAIT_TIMEOUT | MESSAGE_FROM_SCANTASK_FLAG) :
				DBG_PRINTF_SDH_DEBUG("[SM_SC] SMSG_SC_PIPE_WAIT_TIMEOUT(SM_BK)\n");
                scanman_observer_notify_event( SCANMAN_EVENT_SC_PIPE_WAIT_TIMEOUT, 1 );
				break;
			/*
			******************************************************************************************
			* MSG_A3MERGE_NOTIFY_MERGE_END
			******************************************************************************************
			*/
			case MSG_A3MERGE_NOTIFY_MERGE_END:
				DBG_PRINTF_SDH_DEBUG("scanman received MSG_A3MERGE_NOTIFY_MERGE_END after cancel\n");
				merge_state.merge_progressState = MERGE_PROGRESS_NONE;
				break;

			/*
			******************************************************************************************
			* MSG_SCANNER_SP_MODE_START_MSG
			* MSG_SCANNER_SP_MODE_STOP_MSG
			******************************************************************************************
			*/
			case MSG_SCANNER_SP_MODE_START_MSG:
			case MSG_SCANNER_SP_MODE_STOP_MSG:
				DBG_PRINTF_SDH_DEBUG("scanman received MSG_SCANNER_SP_MODE_MSG. msgType=%d, param1=%d, param2=%d\n", sys_msg.msgType, sys_msg.param1, sys_msg.param2);
				uint32_t req_id;
				UINT32 test_target = 0;
				UINT32 test_start = 0;
				UINT32 test_start_argument = 0;

				if(sys_msg.msgType == MSG_SCANNER_SP_MODE_START_MSG) {
					test_start = 1;
				}
				else {
					test_start = 0;
				}
				test_target = sys_msg.param1; 
				test_start_argument = sys_msg.param2;
				
				switch(test_target) {
				case SP_MODE_SCANNER_FB_FREE_RUN:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_FB_FREE_RUN, test_start, NULL, &req_id);
					break;
				case SP_MODE_SCANNER_ADF_FREE_RUN:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_ADF_FREE_RUN, test_start, NULL, &req_id);
					break;
				case SP_MODE_SCANNER_FEED_MOTOR:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_FEED_MOTOR, test_start_argument, NULL, &req_id);
					break;
				case SP_MODE_SCANNER_TRANSPORT_MOTOR:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_TRANS_MOTOR, test_start_argument, NULL, &req_id);
					break;
				case SP_MODE_SCANNER_CHECK_PRESENT_SENSOR:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_PRESENT_SENSOR, test_start, NULL, &req_id);
					break;
				case SP_MODE_SCANNER_CHECK_INTERVAL_SENSOR:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_INTERVAL_SENSOR, test_start, NULL, &req_id);
					break;
				case SP_MODE_SCANNER_CHECK_FIRSTSCAN_SENSOR:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_1STSCAN_SENSOR, test_start, NULL, &req_id);
					break;
				case SP_MODE_SCANNER_CHECK_TOPCOVER_OPEN:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_TOPCOVER_SENSOR, test_start, NULL, &req_id);
					break;
				case SP_MODE_SCANNER_CHECK_HOME_SENSOR:
					scerr = scanman_dev_request(SCANMECH_IOCTL_SP_HOMESENSOR, test_start, NULL, &req_id);
					break;
				default:
					break;
				}
				if( scerr != SCANERR_NONE ) {
					DBG_PRINTF_SDH_DEBUG("MSG_SCANNER_SP_MODE_MSG scanman_dev_request FAIL!\n");
				}
				break;

            default:
                DBG_PRINTF_SDH_ERR("scanman unknown message type=%#x param1=%#lx\n", 
                            sys_msg.msgType, sys_msg.param1);

                /* davep 24-Jul-2008 ; add the assert back while I move
                 * Scantask from MESSAGE to scan_msg_t
                 */
                XASSERT( 0, sys_msg.msgType );
                break;
        }

    } /* end while(1) message loop */

    /* should never get here! */
    ASSERT(0);
    return 0;
}

void ScanManInit(void)
{
    UINT pxretcode;
    scan_err_t scerr;

    /* davep 23-Jul-2008 ; I'm moving internal use of MESSAGE to scan_msg_t so
     * we don't have to ship agMessage.h to the BSDK customers.
     *
     * But I'm still using the mq_send(), etc, functions which are hardwired to
     * send/recieve only MESSAGE sized chunks.
     */
    XASSERT( sizeof(scan_msg_t)==sizeof(MESSAGE), sizeof(scan_msg_t) );

    /* create the message queue we'll use to interact with the rest of the
     * system 
     */
    posix_create_message_queue( &scanman_msgq, "/scanman", SCANMAN_QUEUE_SIZE,
                                sizeof(MESSAGE));   

    router_register_queue(SCANMANID, scanman_msgq);  // Register a message queue.

    scerr = scanman_obs_onetime_init();
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = scanman_smirb_onetime_init();
    XASSERT( scerr==SCANERR_NONE, scerr );

    pxretcode = posix_create_thread( &scanman_task_id, &scanman_task, 0, "scanman",
                                    scanman_stack, SCANMAN_STACK_SIZE, POSIX_THR_PRI_NORMAL );
    XASSERT(pxretcode==0, pxretcode);

    register_scanman_with_system();

    /* unless we're building with very specific debug code, all of demo_scanapp
     * is #def'd to a no-op
     */
    demo_scanapp_init();

	//initial variable
	scanman_stats.calibration_state = SMCALSTATE_CALIBRATION_NONE;

	SDH_A3MergeLib_init();
	memset(&merge_state, 0x00, sizeof(merge_state));

#if HAVE_SCAN_ERASE_ROLLER
	SDH_eraseRoller_init();
	rollerErase_state.enable = SDH_eraseRoller_isUse();
	rollerErase_state.scanJobType = ERASE_JOB_TYPE_NONE;
#endif
}
 
