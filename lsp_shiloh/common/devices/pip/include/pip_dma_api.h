/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains header definitions for the PIP DMA API.
  * 
 **/

#ifndef __PIP_DMA_H
#define __PIP_DMA_H

#include <stdint.h>
#include "error_types.h"



typedef enum
{
    PIP_DMA_FINISHED_WITH_INPUT_BUF    // finished with the oldest active input buffer
} pip_dma_notification_t;


/**
 *
 * \brief pip_dma handle
 *
 */
typedef struct pip_dma_handle_s pip_dma_handle_t;

/**
 * \brief pip_dma Callback function type
 *
 * \param notification Notification from pip_dma to the client
 * 
 * This is the function prototype for the pip_dma callback.
 **/
typedef void (*pip_dma_callback_t)(pip_dma_notification_t notification);


error_type_t pip_dma_open(pip_dma_handle_t **handle);

error_type_t pip_dma_add_input_buffer(pip_dma_handle_t *handle,
                                      uint8_t *buffer,
                                      uint32_t width,
                                      uint32_t height);

error_type_t pip_dma_start(pip_dma_handle_t *handle,
                           uint32_t image_width_in_pixels,
                           uint32_t image_height_in_lines,
                           pip_dma_callback_t callback);

error_type_t pip_dma_generate_output_buffers_blocking(pip_dma_handle_t *handle,
                                                      uint8_t **buffers);

error_type_t pip_dma_close(pip_dma_handle_t *handle);


#endif


