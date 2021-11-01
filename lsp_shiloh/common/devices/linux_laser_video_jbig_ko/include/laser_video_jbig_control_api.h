/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file laser_video_jbig_control_api.h
 *
 * \brief API for direct interface to the laser video control driver.
 *
 */
 
#ifndef _LASER_VIDEO_JBIG_CONTROL_API
#define _LASER_VIDEO_JBIG_CONTROL_API

#include "laser_engine_config_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void vidctl_init(laser_engine_video_config_t* eng_video_config);
int vidctl_start_page(laser_video_page_t* lvpage);
int vidctl_end_page(uint32_t page_id);
int vidctl_add_plane_data_to_current_page(laser_video_plane_t* lvplane);
int vidctl_cancel_pages(void);
void vidctl_set_pcodes(e_pcode_index index, uint32_t* eng_video_pcode_config, uint32_t num_entries);

void vidctl_set_paperless_mode(bool enable);
bool vidctl_get_paperless_mode(void);

void vidctl_query_pixel_count(uint32_t* non_white_pixels);

#define VIDCTL_PAGE_STATUS_OK            0  ///< page completed with no errors
#define VIDCTL_PAGE_STATUS_CANCELLED    -1  ///< page cancelled by uppper level SW request
#define VIDCTL_PAGE_STATUS_ERROR        -2  ///< page completed with error; image not fully printed
typedef void (*vidctl_page_done_callback_t)(void* callback_context, void* page_context, uint32_t page_status);
int vidctl_register_page_done_callback(vidctl_page_done_callback_t func, void* context);

#ifdef __cplusplus
}
#endif

#endif  // _LASER_VIDEO_JBIG_CONTROL_API

