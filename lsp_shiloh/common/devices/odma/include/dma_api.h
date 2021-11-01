/*
 * ============================================================================
 * Copyright (c) 2010-2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef DMA_API_H
#define DMA_API_H

#include <stdint.h>
#include <stdbool.h>
#include "BigBuffer.h"
// Define a container to tag onto the DMA descriptor. Use it to keep track of
// context for that buffer; e.g. was this the SOI buffer? how many lines? Any
// information that is needed further down the pipe should be put here. Keep the
// pointer to the data buffer be first in this structure a pointer to the
// structure will be handed to the JBIG write DMA as the address to fill with
// data. When the buffer is handed back, the information pertaining to the
// buffer will be available.

typedef struct dma_config_data_s
{
    BigBuffer_t *bigbuffer;
	//char *data;		           // pointer to the data buffer start address
	//uint32_t size_in_bytes;	   // size of the data buffer, in bytes
    uint32_t size_of_transfer; // Size of this DMA transfer, in bytes. May be smaller than buffer size for EOI case.
    void *handle;              // Use to hold the handle of the "owner" of this dma
    uint16_t image_width_in_bytes;  
    uint16_t num_lines;	       // Number of lines in this transaction		
    bool SOI;			       // Is this a Start of Image buffer?	
    bool EOI;			       // Is this an End of Image buffer?
    struct dma_config_data_s *next; // Pointer to manage list used for memory management
    uint32_t channel;          // The DMA channel number
} dma_config_data_t;


// Define the function pointer for the notification callback. 
typedef void (* dma_callback_t)(void *handle, dma_config_data_t *);

#endif
