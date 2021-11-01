/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/init.h>

#include "scos.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scantask.h"
#include "scanvars.h"
#include "chipgap.h"
#include "scanmech.h"
#include "adfsensor.h"
#include "scanmem.h"

#include "cal.h"
#include "cal_common.h"
#include "scanlib.h"

#include "../../../../common/include/spec.h"


static int __init scantask_init_module(void)
{
    dbg2( "%s\n", __FUNCTION__ );

    /* 
     * A non 0 return means init_module failed; module can't be loaded. 
     */
    return 0;
}

static void __exit scantask_cleanup_module(void)
{
    dbg2( "%s\n", __FUNCTION__ );
}

module_init( scantask_init_module );
module_exit( scantask_cleanup_module );

EXPORT_SYMBOL(scantask_init);
EXPORT_SYMBOL(scantask_scan_start);
EXPORT_SYMBOL(scantask_cmd_line_run);

EXPORT_SYMBOL(scanvar_delete);
EXPORT_SYMBOL(scanvar_sanity);
EXPORT_SYMBOL(scanvar_set_scan_callback);
EXPORT_SYMBOL(scanvar_set_dpi);
EXPORT_SYMBOL(scanvar_from_siqfile);

EXPORT_SYMBOL(chipgap_set_positions);
EXPORT_SYMBOL(chipgap_get_positions);

EXPORT_SYMBOL(scanmech_adf_client_request_pause);
EXPORT_SYMBOL(scanmech_adf_client_clear_pause);
EXPORT_SYMBOL(scanmech_spmode_request_topcover_sensor);
EXPORT_SYMBOL(scanmech_spmode_request_present_sensor);
EXPORT_SYMBOL(scanmech_spmode_request_home_sensor);
EXPORT_SYMBOL(scanmech_spmode_request_firstscan_sensor);
EXPORT_SYMBOL(scanmech_spmode_request_interval_sensor);
EXPORT_SYMBOL(scanmech_spmode_request_fb_free_run);
EXPORT_SYMBOL(scanmech_spmode_request_trans_motor);
EXPORT_SYMBOL(scanmech_spmode_request_adf_free_run);
EXPORT_SYMBOL(scanmech_spmode_request_feed_motor);
EXPORT_SYMBOL(scanmech_sleep_request);
EXPORT_SYMBOL(scanmech_wakeup_request);

EXPORT_SYMBOL(scanmech_get_fb_margins);
EXPORT_SYMBOL(scanmech_debug_set_fb_margins);
EXPORT_SYMBOL(scanmech_get_adf_margins);
EXPORT_SYMBOL(scanmech_debug_set_adf_margins);

EXPORT_SYMBOL(smech_ioctl);
EXPORT_SYMBOL(scantask_cancel_msg);	// add.lsh.to support user cancel
EXPORT_SYMBOL(scanmech_selftest); //mod.kks support the sc218, 219 on POR.

EXPORT_SYMBOL(adf_sensor_paper_present);

EXPORT_SYMBOL(scanmem_free_aligned);
EXPORT_SYMBOL(scanmech_move_to_A3scanpoint);

EXPORT_SYMBOL(scanvar_mono_cal_new);
//EXPORT_SYMBOL(smech_ioctl);
EXPORT_SYMBOL(scanvar_pop);
EXPORT_SYMBOL(calibrate);
EXPORT_SYMBOL(scanvar_color_cal_new);
EXPORT_SYMBOL(scanvar_push);
EXPORT_SYMBOL(set_scanlib_sensor_bitmask);
EXPORT_SYMBOL(clear_scanlib_sensor_bitmask);
EXPORT_SYMBOL(cal_cache_flush_all);
EXPORT_SYMBOL(cal_done);
EXPORT_SYMBOL(cal_cache_flush_mode);
EXPORT_SYMBOL(calibration_is_running);
EXPORT_SYMBOL(calibration_is_finished);

EXPORT_SYMBOL(scanlib2_done);
EXPORT_SYMBOL(scanlib_cancel_self);


