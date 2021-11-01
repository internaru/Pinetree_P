/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef ICETEST_H
#define ICETEST_H

typedef enum {
    /* data sent into ICETest sent to top of scan block */
    ICETEST_CONFIG_DATA_TO_SCAN=1,
    
    /* data sent into ICEtest sent to top of CISX block */
    ICETEST_CONFIG_DATA_TO_CISX=2
} icetest_mode_t;

#define ICETEST_IDMA_NUM_CHANNELS 1

scan_err_t icetest_onetime_init( void );
scan_err_t icetest_cleanup_module( void );
scan_err_t icetest_soft_setup( void );
void icetest_reset( void );
scan_err_t icetest_set_test_mode( icetest_mode_t mode );
void icetest_dump( void );
void icetest_ddma_dump( void );
void icetest_enable( bool enable );

uint32_t icetest_interrupt_disable( void );
void icetest_interrupt_enable( void );

void icetest_idma_init( void );

int icetest_idma_open( uint8_t channels[],
                   uint8_t num_channels,
                   int num_empties,
                   int total_rows,
                   int bytes_per_row );

void icetest_idma_close( void );

scan_err_t icetest_idma_add_buffer( uint8_t channel, 
                                uint8_t *data, 
                                uint32_t datalen,
                                uint32_t rows, 
                                uint32_t bytes_per_row );

#ifdef ICEDMA_H
void icetest_idma_add_ready( uint8_t channel, 
                                struct ice_dma_buffer **addme );

void icetest_idma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
#endif

int icetest_idma_setup_buffers( uint8_t channel, int num_buffers, 
                            int rows_per_buffer, int bytes_per_row );

void icetest_idma_late_launch( void );

void icetest_idma_init( void );

void icetest_idma_reset( void );
#ifdef DDMA_H
void icetest_idma_desc_write( struct ddma_descriptor *desc, uint32_t line_width_bytes );
void icetest_idma_desc_write_and_wait( struct ddma_descriptor *desc, uint32_t line_width_bytes );
#endif
void icetest_wait_for_fin( void );
void icetest_idma_interrupt_enable( void );
uint32_t icetest_idma_interrupt_disable( void );
void icetest_idma_enable( void );
void icetest_idma_disable( void );

void icetest_idma_channel_reset( uint8_t channel );
void icetest_idma_channel_enable( uint8_t channel );
bool icetest_idma_channel_is_enabled( uint8_t channel );
void icetest_idma_channel_disable( uint8_t channel );
void icetest_idma_channel_start( uint8_t channel );
void icetest_idma_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                                uint32_t rows, uint32_t bytes_per_row );

void icetest_idma_channel_launch( uint8_t channel );

void icetest_idma_cancel( void );
void icetest_idma_sanity( void );

void icetest_interrupt_rtos_wait( void );
void icetest_interrupt_rtos_signal( void );
void icetest_rtos_init( void );

#endif

