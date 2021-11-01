/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * @file sm_job_config.h
 *
 * @brief OEM-specific smjob configuration options
 *  
 * The purpose of the OEM configuration files for this module is to provide
 * a mechanism for the user to modify configurable options without having 
 * to recompile the core module that depend on them. The goal is to isolate
 * OEM and core specific configuration values, and provide an abstraction layer
 * to simplify the porting.
 *
 * Refer to the sm_job_api.h file for descriptions of the capabilities and
 * supporting functions.
 *
 */ 

#ifndef SMJOB_OEM_CONFIG_H
#define SMJOB_OEM_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sm_job_api.h"

//================================================================================
// OEM Macros
//================================================================================

/**
 * @brief This will control the size of the job history stack.
 *
 */
#define SMJOB_HISTORY_MAX_ENTRIES 100

/**
 * @brief The length of time, seconds, before a job will timeout and be
 *        removed from the job history stack.
 *
 */
#define SMJOB_ENTRY_LIFETIME_SEC 60*60*24*3 // 3 days

//================================================================================
// OEM Device Capabilities
//================================================================================
/**
 * @brief The following returns the oem settings. 
 *        These support the smjob_get_supported_* and smjob_get_default_*functions.
 *
 */
smjob_rcode_t smjob_oem_get_icons(smjob_type_t job_type, uint32_t index, char** icon);

smjob_rcode_t smjob_oem_get_supported_compression(smjob_type_t job_type, uint32_t index,
						  smjob_compression_t **value);
smjob_rcode_t smjob_oem_get_default_format(smjob_type_t job_type, smjob_format_t **value);
smjob_rcode_t smjob_oem_get_supported_format(smjob_type_t job_type, uint32_t index,
                                             smjob_format_t **value);
smjob_rcode_t smjob_oem_get_supported_finishings(smjob_type_t job_type, uint32_t index, smjob_finishing_t *value);

smjob_rcode_t smjob_oem_get_default_orientation(smjob_type_t job_type, smjob_orientation_t *value);
smjob_rcode_t smjob_oem_get_supported_orientation(smjob_type_t job_type, uint32_t index, smjob_orientation_t *value);

smjob_rcode_t smjob_oem_get_default_resolution(smjob_type_t job_type, smjob_resolution_t *value);
smjob_rcode_t smjob_oem_get_supported_resolution(smjob_type_t job_type, uint32_t index, smjob_resolution_t *value);
smjob_rcode_t smjob_oem_get_supported_input_source(uint32_t index, smjob_input_source_t *value);

smjob_rcode_t smjob_oem_get_default_copies(uint32_t *value);
smjob_rcode_t smjob_oem_get_supported_copies(uint32_t *min_value, uint32_t *max_value);

smjob_rcode_t smjob_oem_get_supported_sheet_collate(bool *is_supported);

smjob_rcode_t smjob_oem_get_default_sides(smjob_sides_t **value);
smjob_rcode_t smjob_oem_get_supported_sides(uint32_t index, smjob_sides_t **value);

smjob_rcode_t smjob_oem_get_default_print_content_optimize(smjob_print_content_optimize_t **value);
smjob_rcode_t smjob_oem_get_supported_print_content_optimize(uint32_t index, 
                                                             smjob_print_content_optimize_t **value);
smjob_rcode_t smjob_oem_get_default_quality(smjob_quality_t *value);
smjob_rcode_t smjob_oem_get_supported_quality(uint32_t index, smjob_quality_t *value);

smjob_rcode_t smjob_oem_get_default_color_mode(smjob_type_t job_type, 
                                               smjob_color_mode_t **value);
smjob_rcode_t smjob_oem_get_supported_color_mode(smjob_type_t job_type, uint32_t index,
                                                 smjob_color_mode_t **value);

smjob_rcode_t smjob_oem_get_default_media_col(smjob_type_t job_type, smjob_media_col_t *value);

smjob_rcode_t smjob_oem_get_supported_media_type_name(smjob_type_t job_type, uint32_t index, char** name);
smjob_rcode_t smjob_oem_get_supported_default_media_type_name(smjob_type_t job_type, char** name);
smjob_rcode_t smjob_oem_get_supported_media_size_x_dim(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_oem_get_supported_media_size_y_dim(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_oem_get_supported_media_top_margin(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_oem_get_supported_media_bottom_margin(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_oem_get_supported_media_left_margin(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_oem_get_supported_media_right_margin(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_oem_get_aiprint_enabled(smjob_type_t job_type, uint32_t index, 
                                            uint32_t *value);
smjob_rcode_t smjob_oem_get_supported_media_size_name(smjob_type_t job_type, uint32_t index, char** name);
smjob_rcode_t smjob_oem_get_supported_default_media_size_name(smjob_type_t job_type, char** name);
smjob_rcode_t smjob_oem_set_supported_default_media_size_name(smjob_type_t job_type, char* name, uint32_t name_len);
smjob_rcode_t smjob_oem_get_supported_media_source(smjob_type_t job_type, uint32_t index, char** source);

smjob_rcode_t smjob_oem_get_media_table_source(smjob_type_t job_type, uint32_t index, char** source);
smjob_rcode_t smjob_oem_get_media_table_type(smjob_type_t job_type, uint32_t index, char** name);
smjob_rcode_t smjob_oem_get_media_table_duplex(smjob_type_t job_type, uint32_t index, uint32_t *value);

smjob_rcode_t smjob_oem_get_output_bin_default(smjob_type_t job_type, char** name);
smjob_rcode_t smjob_oem_get_output_bin_supported(smjob_type_t job_type, uint32_t index, char** name);

smjob_rcode_t smjob_oem_markers_available(smjob_type_t job_type, uint32_t index, uint32_t *value);

smjob_rcode_t smjob_oem_get_printer_geo_location(smjob_type_t job_type, char** location);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // SMJOB_OEM_CONFIG_H
