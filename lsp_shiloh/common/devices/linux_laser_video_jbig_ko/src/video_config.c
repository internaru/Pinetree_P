/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file video_config.c
 * 
 * \brief
 * 
 **/
#include "video_config.h"
#include "error_types.h"
#include "asic.h"

#if defined(ASIC_gemstone)
// why isn't this in intnums.h?
#define INTNUM_HVSYNC INTNUM_BDMAN  
#endif

static video_config_t video_config = 
{
    .apb_config_base_addr = (void*)APB_CONFIG_BASE,
    .video_base_addr = {(void*)MVDO_SVDO0_BASE, 
                        (void*)MVDO_SVDO1_BASE,
#ifdef MVDO_SVDO2_BASE
                        (void*)MVDO_SVDO2_BASE,
                        (void*)MVDO_SVDO3_BASE,
#endif
#ifdef MVDO_SVDO4_BASE
                        (void*)MVDO_SVDO4_BASE,
                        (void*)MVDO_SVDO5_BASE,
#endif
#ifdef MVDO_SVDO6_BASE
                        (void*)MVDO_SVDO6_BASE,
                        (void*)MVDO_SVDO7_BASE
#endif
                        }, // base_addresses of each video channel 

    .hipwm_base_addr = {(void*)MVDO_HIPWM0_BASE,
                        (void*)MVDO_HIPWM1_BASE,
#ifdef MVDO_HIPWM2_BASE
                        (void*)MVDO_HIPWM2_BASE,
                        (void*)MVDO_HIPWM3_BASE,
#endif
#ifdef MVDO_HIPWM4_BASE
                        (void*)MVDO_HIPWM4_BASE,
                        (void*)MVDO_HIPWM5_BASE,
#endif
#ifdef MVDO_HIPWM6_BASE
                        (void*)MVDO_HIPWM6_BASE,
                        (void*)MVDO_HIPWM7_BASE
#endif
        },  

    .hips_base_addr = (void*)MVDO_HIPS_BASE,

    .hvsync_base_addr = (void*)MVDO_HVSYNC_BASE,

    .video_int_num = {INTNUM_LASER_VIDEO_0,
                      INTNUM_LASER_VIDEO_1,
#ifdef INTNUM_LASER_VIDEO_2
                      INTNUM_LASER_VIDEO_2,
                      INTNUM_LASER_VIDEO_3,
#endif
#ifdef INTNUM_LASER_VIDEO_4
                      INTNUM_LASER_VIDEO_4,
                      INTNUM_LASER_VIDEO_5,
#endif
#ifdef INTNUM_LASER_VIDEO_6
                      INTNUM_LASER_VIDEO_6,
                      INTNUM_LASER_VIDEO_7,
#endif
                     },  

    .hvsync_int_num = INTNUM_HVSYNC,

    .num_video_channels = NUM_VIDEO_CHAN,
};

// LSPTODO: these values really should be pushed to us from the kernel config via probe
// but this works for now.

error_type_t video_get_config(video_config_t **config)
{    
    *config = &video_config;
    return OK;
}

