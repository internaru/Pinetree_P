/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Agilent Technologies, Inc. All Rights Reserved
 *                      
 *                         Agilent Confidential
 * ============================================================================
 *
 * Description:
 *
 * Header file for scanpipe.c, the source which sets up PIC, PIE, and HalfPack.
 *
 *****************************************************************************/

#ifndef SCANPIPE_H
#define SCANPIPE_H

/* pipeline functions in scanpipe.c */

/* flags passed to pipe_start() to enable debug/test behavior in scanpipe.c */
#define PIPE_FLAG_NULL          0
#define PIPE_FLAG_LINK_RESULTS  (1<<1)
#define PIPE_FLAG_ONE_BUFFER    (1<<2)
/* 1<<3 available */
#define PIPE_FLAG_DATA_FROM_NET (1<<4)
#define PIPE_FLAG_DATA_FROM_ICEFILE (1<<5)

/* davep 20-Apr-2009 ; creating pipetap so we can more carefully tell where we
 * want to see the scan-to-net data  (pipetap named after pcap/ethertap)
 */
#define PIPETAP_NONE 0
#define PIPETAP_PIC  1
#define PIPETAP_PIE  2
#define PIPETAP_HP   3
#define PIPETAP_CUTTER 4

scan_err_t pipe_onetime_init( void ); // one time cold init
void pipe_calc_rows( int expected_rows, int *scan_expected_rows, 
                     int *pie_expected_rows );
scan_err_t pipe_start( int pixels_per_row, int scan_expected_rows, 
                 int pie_expected_rows, uint32_t pipe_flags );
scan_err_t pipe_wait( void );
void pipe_cancel( void );
void pipe_done( void );
int pipe_set_pipetap( int pipetap_location );
void init_pipe_dgb_cnt( void );
void print_pipe_dgb_cnt( void );

/* davep 07-Jan-2013 ; allow pipecut direct access to scanpipe rather than
 * function pointers
 */
#ifdef ICEDMA_H
scan_err_t pipecut_callback_send_to_scanman( uint8_t page_side, 
                                struct ice_dma_buffer *icebuf, 
                                bool last_buffer );
//=========================================================================================
void background_detect( struct ice_dma_buffer *icebuf, uint32_t valid_rows);	// add.lsh for background removal
void background_init(void);
void background_open(void);
void background_close(void);
//=========================================================================================
#endif

/* davep 08-Sep-2011 ; changing old old old old old extern to an accessor */
ATLISTENTRY *pipe_get_pipe_results( void );

#endif

