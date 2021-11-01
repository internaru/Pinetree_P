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

#ifndef _FUSER_ZC_CHARDEV_API_H
#define _FUSER_ZC_CHARDEV_API_H

// IOCTLS to Fuser Zc Character Driver
#define FZC_IOCTL_BASE 'Z'
#define FZC_IOCTL_SET_CONFIG        _IOW (FZC_IOCTL_BASE, 0, void*) ///

typedef struct _page_done_notify
{
    uint32_t page_status;
    void* page_context;
}page_done_notify_t;

typedef struct _pcode_config_request
{
    uint32_t index;
    uint32_t num_entries;
}pcode_config_request_t;
#endif
