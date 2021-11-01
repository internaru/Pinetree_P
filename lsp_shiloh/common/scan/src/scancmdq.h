/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004-2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef SCANCMDQ_H
#define SCANCMDQ_H

/* values returned from scan_cmdq_isr() telling scan interrupt handler how we
 * handled the interrupt
 */
#define SCAN_CMDQ_SUCCESS 0
#define SCAN_CMDQ_DONE    1
#define SCAN_CMDQ_EMPTY   2
#define SCAN_CMDQ_PAUSED  3

/* +1 to the num_rows because the SCANY field is actually the number of
 * rows minus 1
 */
#define GET_SCANY(cmd) (SCIF_SCMD_SCANY( cmd )+1)

/* davep 06-Jul-2009 ; adding on-demand pause/resume */
typedef enum {
    SCAN_CMDQ_PAUSE_CAUSE_MEMORY=(1<<0), ///< low on memory 
    SCAN_CMDQ_PAUSE_CAUSE_SCANPIPE=(1<<1), ///< scanpipe requests pause
} scan_cmdq_pause_cause_t;

typedef struct {
    int rs_expected_rows;
    int rs_pending_rows;
    int rs_running_rows;
    int rs_completed_rows;
    int rs_queued_rows; 
} scan_cmdq_row_status_t;

scan_err_t scan_cmdq_onetime_init( void );
scan_err_t scan_cmdq_open( uint8_t channels[], int num_channels, int expected_rows );
void scan_cmdq_channel_add_rows( uint8_t channel, uint16_t scan_lines );
void scan_cmdq_launch( void );
void scan_cmdq_cancel( void );
void scan_cmdq_close( void );
int scan_cmdq_isr( void );
void scan_cmdq_dump(void);
void scan_cmdq_emergency_halt( void );
scan_err_t scan_cmdq_wait_for_ready(void);
scan_err_t scan_cmdq_setup( void );
#ifdef SCANMECH_H
void scan_cmdq_mech_status_change(t_ScanMechStatus mech_status);
#endif
void scan_cmdq_run_forever( void );
scan_err_t scan_cmdq_start_pause( scan_cmdq_pause_cause_t pause_cause );
void       scan_cmdq_resume( scan_cmdq_pause_cause_t pause_cause );
uint32_t   scan_cmdq_get_sclkper( void );

void scan_cmdq_save_row_status(void);
scan_cmdq_row_status_t scan_cmdq_get_row_status(void);

#endif

