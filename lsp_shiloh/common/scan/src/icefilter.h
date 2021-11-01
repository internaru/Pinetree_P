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
 * Copyright (c) 2004, 2005 Avago Technologies, Inc. All Rights Reserved
 *                      
 *                         Avago Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef ICEFILTER_H
#define ICEFILTER_H

/* we have 6 channels max: red,green,blue and even,odd */
#define ICE_FILTER_NUM_CHANNELS  6

void ice_filter_init( void );
int ice_filter_open( uint8_t channels[], 
                            uint8_t num_channels,
                            int total_rows,
                            int bytes_per_row );
void ice_filter_add_ready( uint8_t channel, struct ice_dma_buffer **addme );
void ice_filter_channel_launch( uint8_t channel );
void ice_filter_sanity( void );
void ice_filter_close( void );
//void ice_filter_close( void );
void ice_filter_cancel( void );

/* The following functions are necessary to plug into the ICEDMA architecture
 * but aren't actually used in the software filter plug-in because we're 
 * software. Most of the functions are NOPs.
 */ 
void ice_filter_channel_reset( uint8_t channel );
void ice_filter_channel_start( uint8_t channel );
void ice_filter_channel_enable( uint8_t channel );
bool ice_filter_channel_is_enabled( uint8_t channel );
void ice_filter_channel_disable( uint8_t channel );
void ice_filter_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                              uint32_t rows, uint32_t bytes_per_row );
void ice_filter_isr( uint8_t channel );

#endif

