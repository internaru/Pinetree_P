/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANMARGINS_H
#define SCANMARGINS_H

scan_err_t scanmargin_get_flatbed_margins( struct scan_flatbed_margins fb_margin_table[],
                                    int dpi, scan_cmode_t cmode,
                                    struct scan_flatbed_margins *fb_margins);

scan_err_t scanmargin_debug_set_fb_margins( struct scan_flatbed_margins fb_margin_table[],
                                            struct scan_flatbed_margins *new_fb_margins );

scan_err_t scanmargin_get_adf_margins(struct scan_adf_margins adf_margin_table[],
                                      int dpi, scan_cmode_t cmode,
                                      struct scan_adf_margins *adf_margins);

scan_err_t scanmargin_debug_set_adf_margins(struct scan_adf_margins adf_margin_table[],
                                            struct scan_adf_margins *new_adf_margins);

#endif

