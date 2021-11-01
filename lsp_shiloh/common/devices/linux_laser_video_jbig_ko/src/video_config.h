/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file video_config.h
 * 
 * \brief
 * 
 **/
 
#ifndef MRVL_VIDEO_CONFIG_H
#define MRVL_VIDEO_CONFIG_H
 
#include <stdint.h>
#include <stdbool.h>

#include "asic.h"
#include "error_types.h"
//-jrs#include "video_api.h"
//-jrs#include "regAddrs.h"
//-jrs#include "intnums.h"

//-jrs#define video_regs_t SVDO0_REGS_t
//-jrs#define hipwm_regs_t HIPWM0_REGS_t
//-jrs#define hips_regs_t HIPS_REGS_t
//-jrs#define hvsync_regs_t HVSYNC_REGS_t


/** 
 * \brief Platform configuration. This is a collection of values that do not
 * change at run time and therefore need to only be configured at power up.
 */
typedef struct {
    // Hardware base addresses
    void* apb_config_base_addr;
    void* video_base_addr[NUM_VIDEO_CHAN]; 
    void* hipwm_base_addr[NUM_VIDEO_CHAN]; 
    void* hips_base_addr;
    void* hvsync_base_addr;

    uint32_t video_int_num[NUM_VIDEO_CHAN];
    uint32_t hvsync_int_num;
    uint32_t num_video_channels;

} video_config_t;


error_type_t video_get_config(video_config_t **config);

#endif
