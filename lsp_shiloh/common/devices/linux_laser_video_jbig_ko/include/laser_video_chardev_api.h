/*
 **************************************************************************************
 *
 * Copyright (c) 2013 Marvell International, Ltd.
 *
 **************************************************************************************
 *
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell as part of a proprietary software release,
 * the File is considered Marvell Proprietary and Confidential Information, and is
 * licensed to you under the terms of the applicable Commercial License.
 *
 **************************************************************************************
 *
 * Marvell GPL License Option
 *
 * If you received this File from Marvell as part of a Linux distribution, this File
 * is licensed to you in accordance with the terms and conditions of the General Public
 * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
 * modify it under the terms of the GPL License; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GPL License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 *
 **************************************************************************************
*/

#include <stdint.h>
#include "laser_video_defs.h"

#ifndef _LASER_VIDEO_CHARDEV_API_H
#define _LASER_VIDEO_CHARDEV_API_H

// IOCTLS to Laser Video Character Driver
#define LVC_IOCTL_BASE 'V'
#define LVC_IOCTL_SET_CONFIG        _IOW (LVC_IOCTL_BASE, 0, void*) ///< Send the laser_engine_video_config_t struct to the driver.
#define LVC_IOCTL_START_PAGE        _IOW (LVC_IOCTL_BASE, 1, void*) ///< Send a new page and associated page config to the driver.
#define LVC_IOCTL_END_PAGE          _IOW (LVC_IOCTL_BASE, 2, void*) ///< Indicate end of page to driver.  Page is now ready to print.
#define LVC_IOCTL_ADD_PLANE_DATA    _IOW (LVC_IOCTL_BASE, 3, void*) ///< Add plane data to the current 
#define LVC_IOCTL_CANCEL_PAGES      _IOW (LVC_IOCTL_BASE, 4, void*) ///< Cancel all outstanding pages
#define LVC_IOCTL_SET_PCODE_CONFIG  _IOW (LVC_IOCTL_BASE, 5, void*) ///< Send the pcode_config_request_t struct to the driver.
#define LVC_IOCTL_QUERY_PIXEL_COUNT _IOW (LVC_IOCTL_BASE, 6, void*) ///< Get the pixel count.

typedef struct _page_done_notify
{
    uint32_t page_status;
    void* page_context;
}page_done_notify_t;

typedef struct _pcode_config_request
{
    uint32_t index;
    uint32_t num_entries;
    uint32_t pcode_array[MAX_NUM_PCODE_REGS];
}pcode_config_request_t;
#endif
