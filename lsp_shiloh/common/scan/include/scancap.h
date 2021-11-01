/**
 * \file scancap.h
 *
 * \brief scan hardware capabilities (scancaps)
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANCAP_H
#define SCANCAP_H

/**
 * \brief Information about the current scan hardware platform.
 *
 * These are static values based on the hardware platform.
 *
 * Should not change at run-time. Losely based on DrvDeviceCapabilities()
 * but returning everything in one structure.
 */

#define SCAN_CAP_DPI_MAX        6
#define SCAN_CAP_DATA_TYPE_MAX 20

struct scan_platform_capabilities
{
    bool has_adf;
    bool has_flatbed;
    bool has_duplex;

    /* support for multiple simultaneous sensors */
    bool has_nsensor;

    /* These are the DPIs the hardware supports. Other DPI values will be
     * scaled, e.g., 75 DPI will be scanned at 300 DPI then scaled 1/4.
     *
     * List ends with an entry of zero.
     * example_list = [ 300, 600, 1200, 2400, 0 ];
     */
    uint32_t fb_supported_horiz_dpi[SCAN_CAP_DPI_MAX];
    uint32_t fb_supported_vert_dpi[SCAN_CAP_DPI_MAX];

    uint32_t adf_supported_horiz_dpi[SCAN_CAP_DPI_MAX];
    uint32_t adf_supported_vert_dpi[SCAN_CAP_DPI_MAX];

    /* The scan_data_type (see scantypes.h) the hardware/firmware supports.
     * Using the same zero terminated list as the supported_dpi lists above
     * because 0 is a invalid scan data type (NULL).  See kids, that's why we
     * never start enumerations at zero.
     *
     * example_list = [ SCAN_DATA_TYPE_XRGB, SCAN_DATA_TYPE_PLANE,
     *                  SCAN_DATA_TYPE_MONO, SCAN_DATA_NULL_TYPE ];
     */
    scan_data_type scan_data_type_list[SCAN_CAP_DATA_TYPE_MAX];

    /* davep 24-May-2013 ; number of image sensors we have on board (requires
     * has_nsensor also be true)
     */
    int8_t num_sensors;

    /* yadda yadda yadda */
};

bool scancap_has_sensor_dpi( scan_document_source_t docsrc, uint32_t dpi );
bool scancap_has_data_type( scan_data_type dtype );
void scancap_get_capabilities( struct scan_platform_capabilities *caps );

scan_err_t scancap_get_nearest_horiz_dpi( scan_document_source_t docsrc, uint32_t dpi_in, uint32_t *dpi_out );
scan_err_t scancap_get_nearest_vert_dpi( scan_document_source_t docsrc, uint32_t dpi_in, uint32_t *dpi_out );

void scancap_sanity_check( void );

void scancap_debug_dump( struct scan_platform_capabilities *scancap );

#endif

