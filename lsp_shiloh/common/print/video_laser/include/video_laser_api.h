/*
 *
 * ============================================================================
 * Copyright (c) 2006-2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file video_laser_api.h
 *
 * \brief API for the color and monochrome laser video application.
 *
 */
 
#ifndef _VIDEO_LASER_API_H
#define _VIDEO_LASER_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "print_job_types.h"
#include "error_types.h"
#include "laser_video_types.h"
#include "spec.h"

/// Video Error Codes
#define VIDEO_OK        (OK)        ///< Success
#define VIDEO_FAIL      (FAIL)      ///< Generic Failure
#define VIDEO_NOMEM     (-11)       ///< Memory allocation failed

/// Page completion status codes.  Passed back in the status field of the video_laser_page_done_t calback.
#define VIDEO_PAGE_STATUS_OK        (OK)        ///< Page completed successfully.
#define VIDEO_PAGE_STATUS_ERROR     (FAIL)      ///< Page completed with error.  Video imaging failed.
#define VIDEO_PAGE_STATUS_CANCELLED (-10)       ///< Page completed due to cancel request.

typedef void (*video_laser_pagedone_callback_t)(PAGE_DATA *page, error_type_t status);

error_type_t video_print_page(PAGE_DATA *page);
error_type_t video_cancel_pages(void);
error_type_t video_register_page_done_callback(video_laser_pagedone_callback_t func);
error_type_t video_set_paperless_mode(bool enable);
error_type_t video_eng_pcode_config(uint32_t *vid_eng_config, uint32_t num_entries, e_pcode_index p_index);
error_type_t video_set_pcode(int32_t mode, int32_t one);
error_type_t video_set_pixel_freq(int32_t mode, int32_t one);

#define PWM_DEFAULT 0x30  // 75%
#if MAC_TYPE_M
#define PIXEL_FREQ_DEFAULT  22524692        // 40ppm
#define PIXEL_FREQ_47PPM 24829692  // 24.989692 MHz
#else
#define PIXEL_FREQ_DEFAULT  24829692        // 47ppm          // from lscan_45ppm.xls spreadsheet
#endif

#define PWM_DB_SET_MODE      0  // Beam Power Adjust DB Set Mode
#define PWM_LOW_SPEED_MODE 1  // Low Speed Mode(25ppm)
#define PWM_ORG_SPEED_MODE 2  // Original Speed Mode(47ppm)
#define PWM_SET_MODE      3  // Beam Power Adjust manual setting mode

#define PIXEL_FREQ_HMAG_MODE      0  // Horizontal Magnification Mode
#define PIXEL_FREQ_LOW_SPEED_MODE 1  // Low Speed Mode(25ppm)
#define PIXEL_FREQ_ORG_SPEED_MODE 2  // Original Speed Mode(45ppm)

#ifdef __cplusplus
}
#endif

#endif  // _VIDEO_LASER_API_H
