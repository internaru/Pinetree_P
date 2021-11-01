/*
 * Left/Right margins in firmware.
 *
 * davep 08-Dec-2010
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef FWMARGIN_H
#define FWMARGIN_H

//#define FW_RMARGIN_NUM_CHANNELS 3
//#define FW_WMARGIN_NUM_CHANNELS 3
#define FW_MARGIN_NUM_CHANNELS 6

void fw_margin_onetime_init( void );

/* read margin channel(s) */
int fw_rmargin_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row );
scan_err_t fw_rmargin_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row );
void fw_rmargin_add_ready( uint8_t channel, struct ice_dma_buffer **addme );
void fw_rmargin_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
void fw_rmargin_channel_launch( uint8_t channel );
void fw_rmargin_close( void );
void fw_rmargin_cancel( void );
void fw_rmargin_sanity( void );

void fw_rmargin_channel_reset( uint8_t channel );
void fw_rmargin_channel_start( uint8_t channel );
void fw_rmargin_channel_enable( uint8_t channel );
bool fw_rmargin_channel_is_enabled( uint8_t channel );
void fw_rmargin_channel_disable( uint8_t channel );
void fw_rmargin_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_src, 
                                uint32_t rows, uint32_t bytes_per_row );
void fw_rmargin_isr( uint8_t channel );
void fw_rmargin_debug_log( void );

/* write margin channel(s) */
int fw_wmargin_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row );
scan_err_t fw_wmargin_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row );
void fw_wmargin_add_ready( uint8_t channel, struct ice_dma_buffer **addme );
void fw_wmargin_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
void fw_wmargin_channel_launch( uint8_t channel );
void fw_wmargin_close( void );
void fw_wmargin_cancel( void );
void fw_wmargin_sanity( void );

void fw_wmargin_channel_reset( uint8_t channel );
void fw_wmargin_channel_start( uint8_t channel );
void fw_wmargin_channel_enable( uint8_t channel );
bool fw_wmargin_channel_is_enabled( uint8_t channel );
void fw_wmargin_channel_disable( uint8_t channel );
void fw_wmargin_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                              uint32_t rows, uint32_t bytes_per_row );
void fw_wmargin_isr( uint8_t channel );
void fw_wmargin_debug_log( void );

void fw_wmargin_refill( int rows_per_buffer );

int fw_margin_setup_write_buffers( uint8_t channel, int num_buffers, 
                                    int rows_per_buffer, int bytes_per_row );

scan_err_t fw_margin_setup_margins( const struct scanvars *sv,
                                    int pixels_in, int *pixels_out );

scan_err_t fw_margin_open( void );
scan_err_t fw_margin_close( void );

#endif

