/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include "error_types.h"
#include "memAPI.h"
#include "zx_idma_api.h"
#include "pip_api.h"
#include "pip_cfg.h"
#include "pip_dma_api.h"

#define PIP_DMA_COOKIE 0x50444d41


struct pip_dma_handle_s
{
    uint32_t cookie;
    uint32_t output_width_in_pixels;
    uint32_t rows_per_strip; 
    bool open;
    pip_dma_callback_t callback;
};


error_type_t pip_dma_open(pip_dma_handle_t **handle)
{
    *handle = MEM_MALLOC(sizeof(pip_dma_handle_t));
    if (NULL == *handle) return FAIL;

    memset(*handle, 0, sizeof(pip_dma_handle_t));
    (*handle)->cookie = PIP_DMA_COOKIE;
    (*handle)->open = true;

    return zx_idma_open();
}

error_type_t pip_dma_add_input_buffer(pip_dma_handle_t *handle,
                                      uint8_t *buffer,
                                      uint32_t width,
                                      uint32_t height)
{
    if (NULL == handle) return FAIL;
    if (false == handle->open) return FAIL;

    return zx_idma_add_buffer(buffer, width, height);
}

error_type_t pip_dma_start(pip_dma_handle_t *handle,
                           uint32_t image_width_in_pixels,
                           uint32_t image_height_in_lines,
                           pip_dma_callback_t callback)
{
    if (NULL == handle) return FAIL;
    if (false == handle->open) return FAIL;

    handle->callback = callback;

    return zx_idma_start(image_width_in_pixels, image_height_in_lines);
}

error_type_t pip_dma_generate_output_buffers_blocking(pip_dma_handle_t *handle,
                                                      uint8_t **buffers)
{
    uint32_t i;
    static uint32_t rows_generated = 0;
    uint32_t *output_planes[6];
    pip_stats_entry_t combined_stats;
    pip_stats_entry_t chan_stats[PIP_MAX_OUTPUT_CHANS];
    error_type_t ret;

    ret = FAIL;

    if (NULL == handle) return FAIL;
    if (false == handle->open) return FAIL;
    if (0 == handle->rows_per_strip) return FAIL;
    if (0 == handle->output_width_in_pixels) return FAIL;

    for (i = 0; i < handle->rows_per_strip; i++) 
    {
        // Generate one row of output data
        output_planes[0] = NULL; 
        output_planes[1] = NULL; 
        output_planes[2] = NULL; 
        output_planes[3] = NULL; 
        output_planes[4] = (uint32_t *) (buffers[4] + (i*handle->output_width_in_pixels)); 
        output_planes[5] = NULL; 
	rows_generated += 1;
        pip_generate_rows(output_planes,
                          1,
                          &combined_stats,
                          chan_stats,
			  true, true,
                          rows_generated /// totally broken... 
	    );
    }

    // We are definitely done with the corresponding input buffer here. Call
    // the callback so we can inform the client.
    ASSERT(NULL != handle->callback);
    handle->callback(PIP_DMA_FINISHED_WITH_INPUT_BUF);

    return ret;
}

error_type_t pip_dma_close(pip_dma_handle_t *handle)
{
    if (NULL == handle) return FAIL;
    if (false == handle->open) return FAIL;

    handle->open = false;
    MEM_FREE_AND_NULL(handle);

    return zx_idma_close();
}

