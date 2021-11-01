/*
 * ============================================================================
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file pipr_odma_cfg.h
 * 
 * \brief
 * 
 **/
 
#ifndef RASTER_ODMA_CFG_H
#define RASTER_ODMA_CFG_H
 
#include <stdbool.h>
#include <stdint.h>
#include "asic.h"
#include "error_types.h"

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define NUM_RASTER_ODMA_CHANNELS 9

#define PIPR_ODMA_BIT_WIDTH_1  0
#define PIPR_ODMA_BIT_WIDTH_2  1
#define PIPR_ODMA_BIT_WIDTH_4  2
#define PIPR_ODMA_BIT_WIDTH_8  3
#define PIPR_ODMA_BIT_WIDTH_16 4
#define PIPR_ODMA_BIT_WIDTH_24 6
#define PIPR_ODMA_BIT_WIDTH_32 5

/**
 *
 * \brief Output DMA channel type to define all possible DMA channels by name.
 *
 **/
typedef enum {
    ODMA_0 = 0,
    ODMA_1 = 1,
    ODMA_2 = 2,
    ODMA_3 = 3,
    ODMA_4 = 4,
    ODMA_5 = 5,
    ODMA_6 = 6,
    ODMA_7 = 7,
    ODMA_PIPE_ACCESS = 6
} odma_channel_num_t;

typedef struct raster_odma_cfg_s 
{
    bool serpentine;
    bool line_reverse;
    bool transfer_reverse;
    bool flip_horizontal;
    bool input_on_upper_half;
    bool input_on_msb;
    bool enable_dma_block;
    uint8_t input_data_width;
    uint8_t burst_length;
} raster_odma_cfg_t;


extern raster_odma_cfg_t raster_odma_default_cfg;

extern raster_odma_cfg_t raster_odma_1_bpp_cfg;
extern raster_odma_cfg_t raster_odma_2_bpp_cfg;
extern raster_odma_cfg_t raster_odma_4_bpp_cfg;
extern raster_odma_cfg_t raster_odma_8_bpp_cfg;
extern raster_odma_cfg_t raster_odma_32_bpp_cfg;

extern raster_odma_cfg_t raster_odma_1_bpp_no_serp_cfg;
extern raster_odma_cfg_t raster_odma_2_bpp_no_serp_cfg;
extern raster_odma_cfg_t raster_odma_4_bpp_no_serp_cfg;


/**
 * \brief Returns the base address of the specified raster odma channel.
 *  
 * @param chan
 * @param addr_ptr
 * 
 * @return error_type_t 
 */
error_type_t raster_odma_get_base_addr(uint32_t channel, ODMA_REGS_t **addr_ptr);


#endif
