/**
 * \file pipenet.h
 *
 * \brief Header file for pipenet.
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef PIPENET_H
#define PIPENET_H

#ifdef HAVE_NETWORK

void pipenet_init( void );

int pipenet_multichannel_open( 
                   uint8_t read_channels[],
                   uint8_t num_read_channels,

                   uint8_t write_channels[],
                   uint8_t num_write_channels,

                   int total_rows_in,
                   int total_rows_out,
                   int read_buffers_per_channel,
                   int read_rows_per_buffer,
                   int read_bytes_per_row );

void pipenet_multichannel_close( void );

void pipenet_rdma_buffer( struct ice_dma_buffer **icebuf );
bool pipenet_message( scan_msg_t *msg, uint32_t *wait_flags );

#else

/* no network, no pipenet functions. */
static inline void pipenet_init( void ) {}

static inline int pipenet_multichannel_open( 
                   uint8_t read_channels[],
                   uint8_t num_read_channels,

                   uint8_t write_channels[],
                   uint8_t num_write_channels,

                   int total_rows_in,
                   int total_rows_out,
                   int read_buffers_per_channel,
                   int read_rows_per_buffer,
                   int read_bytes_per_row ) { return 0; }

static inline void pipenet_multichannel_close( void ) {}

static inline void pipenet_rdma_buffer( struct ice_dma_buffer **icebuf ) {}
static inline bool pipenet_message( scan_msg_t *msg, uint32_t *wait_flags ) 
    { return false; }

#endif /* #ifdef HAVE_NETWORK */

#endif

