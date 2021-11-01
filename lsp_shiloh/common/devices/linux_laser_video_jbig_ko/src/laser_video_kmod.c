/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <linux/module.h>
#include "laser_video_jbig_control_api.h"

EXPORT_SYMBOL(vidctl_init);
EXPORT_SYMBOL(vidctl_start_page);
EXPORT_SYMBOL(vidctl_end_page);
EXPORT_SYMBOL(vidctl_add_plane_data_to_current_page);
EXPORT_SYMBOL(vidctl_set_paperless_mode);
EXPORT_SYMBOL(vidctl_get_paperless_mode);
EXPORT_SYMBOL(vidctl_register_page_done_callback);
EXPORT_SYMBOL(vidctl_cancel_pages);
EXPORT_SYMBOL(vidctl_set_pcodes);
EXPORT_SYMBOL(vidctl_query_pixel_count);

static int __init laser_video_init(void)
{
    printk(KERN_INFO "laser_video_init\n");
    return 0;
}

static void __exit laser_video_exit(void)
{
    printk(KERN_INFO "laser_video_exit\n");
}


module_init(laser_video_init);
module_exit(laser_video_exit);

MODULE_AUTHOR("Marvell Semiconductor, Inc.");
MODULE_DESCRIPTION("Kernel Laser Video JBIG");
MODULE_LICENSE("Proprietary");

