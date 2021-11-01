/**
 * \file scantask.h
 *
 * \brief Header file for scantask.c
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2008 Marvell Semiconductor Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANTASK_H
#define SCANTASK_H

struct scanvars;
struct scos_mq;

/* return values from scantask_get_task_status() */
typedef enum {
    SCAN_TASK_STATUS_NULL=0, /* uninitialized value, zip, zero, nada */
    SCAN_TASK_STATUS_NO_SUCH_TASK=1,
    SCAN_TASK_STATUS_RUNNING=2,
    SCAN_TASK_STATUS_PENDING=3,
} scan_task_status_t;

/**
 * \brief  Creates Scantask.
 *
 * Scan hardware is NOT initialized here.  This function only starts the
 * scantask thread.
 *
 * The scantask thread next exits. Even if the hardware fails, the scantask
 * thread will continue running, allowing a chance to talk to the hardware for
 * further debugging.
 *
 * \author David Poole
 * \date 26-Aug-2008
 *
 */

scan_err_t scantask_init( scan_callback_fn_t init_callback_fn );

/**
 * \brief Returns the Scantask message queue 
 *
 * \retval Scantask's message queue
 *
 * A simple convenience function so message loop code (and ONLY message loop
 * code!) can grab Scantask's message queue. 
 *
 * This function is for internal use only!  
 *
 * \author David Poole
 * \date 27-Mar-2005
 *
 */

struct scos_mq * scantask_get_msgq( void );

/**
 * \brief Gets pointer to scantask's threadid
 *
 *
 * \author David Poole
 * \date 11-Aug-2005
 *
 */

pthread_t scantask_get_id( void );

/**
 * \brief Returns the number of pending messages in the ScanTask queue 
 *
 * Originally decided I needed to peek into the message queues back when we were
 * implementing cancel.  I needed to make sure ScanTask was idle before I
 * canceled ScanMan.
 *
 * Created this function so we can hide the POSIX message queue from non-POSIX
 * source.
 *
 *
 * \author David Poole
 * \date 08-Jul-2008
 *
 */

int scantask_get_queue_pending( void );

/**
 * \brief Sends a message to ScanTask with no wait
 *
 * NOTE! This function is designed to be called from interrupt context!  We
 * CANNOT block here.
 *
 * \author David Poole
 * \date 30-Jun-2008
 *
 */

scan_err_t scantask_msg_send_nowait( scan_msg_t *msg );

/**
 * \brief Sends a message to ScanTask with a timeout 
 *
 * \param msg ; message to send
 * \param wait_usec ; number of microseconds to wait
 *
 * If wait_usecs==SCOS_WAIT_FOREVER, we could block indefinitely on the message
 * queue.
 *
 * \author David Poole
 * \date 30-Jun-2008
 *
 */

scan_err_t scantask_msg_send( scan_msg_t *msg, uint32_t wait_usec );

/**
 * \brief Cancel Scantask.
 *
 * Sends a cancel message to Scantask. When cancel is finished, Scantask will
 * ACK the cancel by sending a SMSG_SCAN_CANCEL via scantask_msg_callback.
 *
 * Cannot be called from interrupt context.
 *
 * TODO: Need more documentation here.
 *
 * \author David Poole
 * \date 15-Oct-2008
 */

scan_err_t scantask_cancel_msg( void );

/* \brief Cancel a scan task.
 *
 * Attempts to remove the scan task from the pending task queue. 
 *
 * Returns SCANERR_BUSY if the task is already running.
 * Returns SCANERR_NO_SUCH_ENTRY if the task is not in the list
 * Returns SCANERR_NONE if the task is successfully cancelled.
 *
 * Note this does NOT immediately cancel a running scan. If the task is running,
 * a SMSG_SCAN_CANCEL is sent to Scantask.
 *
 * \param task_id the id to cancel (id from scantask_scan_start())
 * \param task_status the status of the task we just cancelled
 *
 * We return the task_status so the caller will know whether or not to wait for
 * messages from Scantask. If the task had never been started, there is no
 * reason to wait for Scantask (i.e., we canceled a queued but not running
 * task).
 *
 */

scan_err_t scantask_scan_cancel( uint32_t task_id, scan_task_status_t *task_status );

/**
 * \brief Start Scantask scanning. 
 *
 * Call after initializing a scanvar to start a scan.
 *
 * TODO: Need more documentation here.
 *
 * \author David Poole
 * \date 16-Oct-2008
 *
 */

scan_err_t scantask_scan_start( const struct scanvars *sv, uint32_t *task_id );

/**
 * \brief Read the status of a queued scan task.
 *
 * See SCANTASK_TASK_xxx #def's for return values
 *
 * \author David Poole
 * \date 20-Sep-2009
 *
 */

scan_task_status_t scantask_get_task_status( uint32_t task_id );

/**
 * \brief Send the SMSG_SCAN_END from Scantask to the scanning client thread
 *
 * \author David Poole
 * \date 16-Oct-2008
 *
 */
void scantask_scan_end( struct scanvars **sv );

/**
 * \brief Send a test/debug message to scantask.
 *
 * For internal testing/debugging only.  Sends a scandbg.h message to scantask.
 *
 * While scantask is running, the test/debug messages will cause scantask to
 * assert. This function will queue the message in a "voicemail box" which
 * scantask checks when he returns to an idle state
 *
 * \author David Poole
 * \date 23-Jul-2009
 *
 */

scan_err_t scantask_debug_msg_send( scan_msg_t *scan_dbg_msg );

/**
 * \brief Check if ScanTask is busy.
 *
 * Call to check if ScanTask is busy or idle. If idle, ScanTask is accepting new
 * scan requests.
 *
 * \author David Poole
 * \date 18-Dec-2008
 *
 */
bool scantask_is_busy( void );

/**
 * \brief Ask Scantask to do the mech poll.
 *
 * Send the SMSG_SMECH_POLL message to Scantask 
 *
 * Originally created to handle mechanical ADF paper present hardware. You know
 * the one I mean.
 *
 * \author David Poole
 *
 */
scan_err_t scantask_smech_poll( void );

/**
 * \brief Ask Scantask to power up.
 *
 * Sends the SMSG_SCAN_MECH_PWR_ON message to Scantask 
 *
 * \author David Poole
 * \date 10-Aug-2009
 *
 */
scan_err_t scantask_power_on( void );

/**
 * \brief Ask Scantask to power down.
 *
 * Sends the SMSG_SCAN_MECH_PWR_OFF message to Scantask 
 *
 * \author David Poole
 * \date 10-Aug-2009
 *
 */
scan_err_t scantask_power_off( void );

/**
 * \brief Scanmech ioctl 
 *
 * \param in smirb - ioctl request block
 * \param out task_id - scantask task id of the queued ioctl request
 *
 * \author David Poole
 * \date 28-Sep-2011
 *
 */

#ifndef __KERNEL__
scan_err_t scan_ready_from_ioctl( void );
void scan_ready_scantask( void );
#endif


#ifdef SMIRB_H 
scan_err_t scantask_ioctl_request( struct scanman_ioctl_request_block *smirb,
                                   uint32_t *task_id );
#endif

scan_err_t scantask_msg_callback( scan_msg_t *msg );
int scantask_cmd_line_run( const char *cmdline ) ;

#endif

