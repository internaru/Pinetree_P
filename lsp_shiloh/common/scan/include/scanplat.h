/**
 * \file scanplat.h
 *
 * \brief  Platform-specific scanner and motor settings interface
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell Semiconductor Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef SCANPLAT_H
#define SCANPLAT_H

/* forward decl; see scancap.h */
struct scan_platform_capabilities;

/**
 * \brief  Initialize the motor hardware
 *
 * One-time initialization for motors - create and initialize the specific mech
 * driver.
 *
 * \remark A Null motor driver is supported for debug and early bring-up
 * functionality.  The null motor driver does not have a motor attached,
 * but has enough of the Scanmech interface to let the scan system think it's
 * talking to a motor, and allow a scan to proceed
 */
void scanplat_init_mech(void);


/**
 * \brief  Turn on the GPIOs to allow communication to the scanner
 *
 * The ASIC, by default, may have all comm lines to the scanners
 * turned off.  This function is called by the scanner initialization
 * code to turn on the lines.
 */
void scanplat_init_scan_gpio( void );

/**
 * \brief  Gets the maximum scan length
 *
 * Created for configuring scannable area between ADF and
 * flatbed designs.
 *
 * \retval uint32_t  the maximum scan length in 1/100's of an inch
 */
uint32_t scanplat_get_max_scan_length( scan_document_source_t doc_src );

/**
 * \brief  Return a structure indicating hardware capabilities of the current
 * scan platform.
 *
 * Members of this structure will not change at run-time.
 *
 * \author David Poole
 * \date 04-Jan-2009
 *
 */

void scanplat_get_capabilities( struct scan_platform_capabilities *caps );


/*\brief Get/set PIC ESD settings
 *
 *
 */
scan_err_t scanplat_get_esd_settings(bool *p_esd_dir,
                         uint16_t *p_esd_accum_size,
                                     uint16_t *p_esd_start_offset,
                                     uint8_t *p_esd_change,
                                     uint8_t *p_esd_tolerance);

scan_err_t scanplat_debug_set_esd_settings(bool *p_esd_dir,
                                           uint16_t *p_esd_accum_size,
                                           uint16_t *p_esd_start_offset,
                                           uint8_t *p_esd_change,
                                           uint8_t *p_esd_tolerance);


scan_err_t scanplat_get_psense_settings(uint16_t *p_start_pixel,
                                           uint8_t  *p_region_width,
                                           uint8_t *p_region_tolerance,
                                           uint8_t *p_total_region_tolerance,
                                           uint8_t *p_toggle01,
                                           uint8_t *p_toggle10,
                                           uint8_t *p_count0,
                                           uint8_t *p_count1 );

scan_err_t scanplat_debug_set_psense_settings(uint16_t *p_start_pixel,
                                              uint8_t  *p_reg_tolerance, uint8_t *p_total_reg_tolerance,
                                              uint8_t  *p_toggle01, uint8_t *p_toggle10,
                                              uint8_t  *p_count0,   uint8_t *p_count1);

/* davep 18-Nov-2011 ; very strange special case; see
 * scif_calc_sclkper()-scanif.c 
 */
uint32_t scanplat_calc_sclkper( scan_cmode_t cmode, uint32_t dpi, uint32_t pixeltimes_per_line );

#endif /* SCANPLAT_H */

