/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file raster_odma_cfg.c
 * 
 * \brief
 * 
 **/

#include "ATypes.h"
#include "raster_odma_cfg.h"

raster_odma_cfg_t  raster_odma_default_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_2,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = false,
    .transfer_reverse = false,
    .flip_horizontal = false
};

raster_odma_cfg_t  raster_odma_1_bpp_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_1,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = true,
    .transfer_reverse = false,
    .flip_horizontal = false
};


raster_odma_cfg_t  raster_odma_2_bpp_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_2,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = true,
    .transfer_reverse = false,
    .flip_horizontal = false
};


raster_odma_cfg_t  raster_odma_4_bpp_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_4,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = true,
    .transfer_reverse = false,
    .flip_horizontal = false
};


raster_odma_cfg_t  raster_odma_8_bpp_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_8,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = false,
    .transfer_reverse = false,
    .flip_horizontal = false
};


raster_odma_cfg_t  raster_odma_32_bpp_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_32,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = false,
    .transfer_reverse = false,
    .flip_horizontal = false
};


raster_odma_cfg_t  raster_odma_1_bpp_no_serp_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_1,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = false,
    .transfer_reverse = false,
    .flip_horizontal = false
};


raster_odma_cfg_t  raster_odma_2_bpp_no_serp_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_2,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = false,
    .transfer_reverse = false,
    .flip_horizontal = false
};


raster_odma_cfg_t  raster_odma_4_bpp_no_serp_cfg =
{
    .burst_length = 0,
    .enable_dma_block = true,
    .input_data_width = PIPR_ODMA_BIT_WIDTH_4,
    .input_on_msb = false,
    .input_on_upper_half = false,
    .line_reverse = false,
    .serpentine = false,
    .transfer_reverse = false,
    .flip_horizontal = false
};


static uint32_t raster_odma_base_addrs[NUM_RASTER_ODMA_CHANNELS] = 
{
    PIP_ODMA0_BASE, 
    PIP_ODMA1_BASE,
    PIP_ODMA2_BASE,
    PIP_ODMA3_BASE,
    PIP_ODMA4_BASE,
    PIP_ODMA5_BASE,
    PIP_PA_PA_ODMA_BASE    // base_addresses of each channel
};


error_type_t raster_odma_get_base_addr(uint32_t channel, ODMA_REGS_t **addr_ptr)
{
    error_type_t ret = FAIL;
    ODMA_REGS_t *addr = NULL;

    if (channel<NUM_RASTER_ODMA_CHANNELS && 
        channel<(sizeof(raster_odma_base_addrs)/sizeof(ODMA_REGS_t *)))
    {
        addr = (ODMA_REGS_t *)raster_odma_base_addrs[channel];
        ret = OK;
    }

    *addr_ptr = addr;
    return ret;
}
