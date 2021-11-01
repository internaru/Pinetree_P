/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2006-2008   Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan Application Lib
 *
 *****************************************************************************/

#ifndef SCANLIB_H
#define SCANLIB_H

#define SENSOR_BITMASK_OWNER_NONE  0    /* uninitialized */
#define SENSOR_BITMASK_OWNER_OUTER 1    /* scanlib2_setup_and_run */
#define SENSOR_BITMASK_OWNER_INNER 2    /* scanlib2_setup / scanlib2_done */

/* forward declaration */
struct scanvars;

scan_err_t scanlib_onetime_init( void );

#ifndef __KERNEL__
scan_err_t handle_send_ioctl( uint32_t request, int arg, void *ptr );
void scanlib_kmsgs_task_init( void );
#endif

scan_err_t scanlib2_setup( void );
void scanlib2_done( void );
scan_err_t scanlib2_run( uint32_t pipe_flags );
scan_err_t scanlib2_setup_and_run( struct scanvars **psv );
bool scanlib_is_locked( void );
scan_err_t scanlib_cancel_self( void );

void scanlib_send_page_start( uint8_t page_side );
void scanlib_send_page_end( uint8_t page_side );
void scanlib_send_scan_size( uint32_t pixels_per_row_padded, 
                             uint32_t pixels_per_row, uint32_t total_rows, uint32_t bits_per_pixel );
void scanlib_send_page_data( uint8_t **buf, scan_data_type dtype, 
                             uint32_t num_rows, uint32_t max_rows, 
                             int page_side, bool last_buffer );
void scanlib_send_adf_jam( uint8_t jam );

void scanlib_send_sc(uint8_t sc );

void set_scanlib_sensor_bitmask( int owner, const struct scanvars *sv ); 
void clear_scanlib_sensor_bitmask( int owner );

scan_err_t scanlib_call_msg_callback( scan_msg_t *msg );

void calibration_is_running( void );
void calibration_is_finished( void );

#endif // SCANLIB_H

