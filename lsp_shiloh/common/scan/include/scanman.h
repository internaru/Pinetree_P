/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell Technologies, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef SCANMAN_H
#define SCANMAN_H

/* davep 13-Sep-2011 ; RIP Status Manager. Replace with something new. */
typedef enum {
    SCANMAN_EVENT_NULL=0, /* invalid/uninitialized */
    SCANMAN_EVENT_IDLE=1,
    SCANMAN_EVENT_SCAN_START=2,
    SCANMAN_EVENT_SCAN_RUNNING=3,
    SCANMAN_EVENT_SCAN_CALIBRATION_RUNNING=4,
    SCANMAN_EVENT_SCAN_CALIBRATION_COMPLETE=5,
    SCANMAN_EVENT_SCAN_COMPLETE=6,

    SCANMAN_EVENT_SCAN_CANCEL=7,
    SCANMAN_EVENT_SCAN_OFFLINE=8,

    /* scan subsystem is initializing, not ready for scanning */
    SCANMAN_EVENT_INITIALIZING=10,
    SCANMAN_EVENT_INITIALIZED=11,

    SCANMAN_EVENT_ADF_PAPER_PRESENT=20,
    SCANMAN_EVENT_ADF_PAPER_JAM=21,
    SCANMAN_EVENT_ADF_PAPER_NOPICK=22,
    SCANMAN_EVENT_ADF_TOP_COVER_OPEN=23,
    SCANMAN_EVENT_FB_PAPER_EMPTY=24,
    SCANMAN_EVENT_PAGE_END_COUNT=25,
    SCANMAN_EVENT_ADF_PAPER_SHORT=26,
    
    /* the dev request id will be in the 'value' field of the callback data */
    SCANMAN_EVENT_DEV_REQUEST_SUCCESS=100, /* dev request accepted and successfully completed */
    SCANMAN_EVENT_DEV_REQUEST_REFUSE=101,  /* dev request refused */
    SCANMAN_EVENT_DEV_REQUEST_FAIL=102,    /* dev request accepted but failed to execute */
    /* more later */

    /* Scan SC */
    SCANMAN_EVENT_SC_AFE_OFFSET_FAIL = 211,
    SCANMAN_EVENT_SC_AFE_GAIN_FAIL = 212,
    SCANMAN_EVENT_SC_AFE_SETTING_FAIL = 213,
    SCANMAN_EVENT_SC_CIS_HOME_POSITION_FAIL = 214,
    SCANMAN_EVENT_SC_WHITE_STRIP_READ_FAIL = 215,
    SCANMAN_EVENT_SC_BLACK_STRIP_READ_FAIL = 216,
    SCANMAN_EVENT_SC_BAD_PIXEL_FAIL = 217,
    SCANMAN_EVENT_SC_CIS_HOME_OFF_FAIL = 218,
    SCANMAN_EVENT_SC_CIS_HOME_ON_FAIL = 219,
    SCANMAN_EVENT_SC_BAD_PIXEL_FAIL_B = 220,
    SCANMAN_EVENT_SC_PIC_OVF_FAIL = 221,
    SCANMAN_EVENT_SC_CMD_QUEUE_OVR_FAIL = 222,
    SCANMAN_EVENT_SC_CMD_COMPLETE_OVR_FAIL = 223,
    SCANMAN_EVENT_SC_MEM_MALLOC_FAIL = 224,
    SCANMAN_EVENT_SC_PIPE_WAIT_TIMEOUT = 228,
    SCANMAN_EVENT_SC_SCAN_ASSERT_FAIL = 240,
    
} scanman_event_t;


/* function prototypes */
void ScanManInit(void);

scan_err_t scanman_msg_send( scan_msg_t *scan_msg );
scan_err_t scanman_msg_send_nowait( scan_msg_t *scan_msg );
scan_err_t scanman_dev_request( uint32_t request, int arg, void *ptr, uint32_t *request_id );

bool scanner_is_alive( void ); 
uint32_t scanman_scanner_counter_read( void );
int scanman_get_powersave_state( void );
void scanman_set_powersave_state( int isPowerSave );
void scanman_set_scanpapersize(uint16_t scanPaperSize);
void scanman_set_skipmsgfail(bool flag);
bool scanman_get_skipmsgfail(void);
#ifdef _PTHREAD_H
pthread_t scanman_thread_identify( void );
#endif

#ifdef EVENT_OBSERVER_H
#define SCANMAN_OBSERVER_ID 0x736d616e

typedef struct 
{
    Response_Subject_T_BaseClassMembers;
    scanman_event_t event;
    uint32_t value;
} scanman_observer_data_t;

scan_err_t scanman_obs_onetime_init( void );
scan_err_t scanman_obs_navel_gazer_init( void );

void scanman_observer_attach( Observer_t *o );
void scanman_observer_detach(Observer_t *o);
void scanman_observer_notify_event( scanman_event_t event, uint32_t value );
void scanman_observer_get_last_event( scanman_event_t *event, uint32_t *value );
#endif

#endif

