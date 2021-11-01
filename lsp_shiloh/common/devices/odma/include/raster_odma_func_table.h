/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file raster_odma_func_table.h
 * 
 * \brief  This file contains the function pointer and function pointer table 
 *         definitions for the common output DMA interface. This interface
 *         supports the JBIG write DMA and the raster monochrome image pipe
 *         output DMA.
 *
 **/

#ifndef _RASTER_ODMA_FUNC_TABLE_H
#define _RASTER_ODMA_FUNC_TABLE_H

#include <stdbool.h>
#include <stdint.h>
#include "BigBuffer.h"

// #include "error_types.h"

typedef error_type_t (*add_big_buffer_t)( void *handle,
					  BigBuffer_t *bigbuffer,
					  uint32_t transfer_size,
					  uint32_t user_callback_data,
					  bool SOI,
					  bool EOI );

typedef error_type_t (*add_data_buffer_t)( void *handle,
				           uint8_t *buffer_addr,
                                           uint32_t buffer_size,
                                           uint32_t user_callback_data,
                                           bool SOI,
                                           bool EOI );


typedef void (*abort_dma_t)(void *handle);

typedef void (*set_line_width_in_bytes_t)( void *handle,
                                           uint32_t line_width_in_bytes );


typedef void (*odma_event_callback_t)( void *user_data );

typedef error_type_t (*register_event_callback_t)( void *handle,
                                                   odma_event_callback_t func );

typedef struct odma_fn_ptr_s
{
    add_big_buffer_t            add_bigbuffer;
    add_data_buffer_t           add_data_buffer;
    abort_dma_t                 abort_dma;
    set_line_width_in_bytes_t   set_output_dma_line_width_in_bytes;
    register_event_callback_t   register_event_callback;
} odma_func_ptr_t;

#endif
