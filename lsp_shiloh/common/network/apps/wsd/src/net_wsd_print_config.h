/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_wsd_print_config.h
 *
 * \brief This file implements the public configuration API for
 * WSD Print.
 * 
 **/

#ifndef INC_NET_WSD_PRINT_CONFIG_H
#define INC_NET_WSD_PRINT_CONFIG_H

//--------------------------------------
// Included files
//--------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <error_types.h>
#include "printvars_api.h"
#include "wsd_print.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------
// API Declarations and Type Definitions
//--------------------------------------

/**
 * @brief WSD Print Configuration structure
 * 
 * This structure contains the configuration options that apply
 * to WS-Print.
 */
typedef struct net_wsd_print_config
{
    uint32_t    signal_io_ready_threshold;   // Signal CM that we have data ready after receiving this many bytes from the attachment
    uint32_t    max_recv_bytes_to_buffer;    // Buffer up to this many bytes before blocking for parser to read the data
    uint32_t    signal_to_recv_threshold;    // If the buffered data level falls below this value, start reading more from the attachment
} net_wsd_print_config_t;

/**
 * @brief Get the current config values for WS Print
 *
 * @param[out] config Pointer to storage allocated by caller
 *       used to pass back the config structure.
 *
 * @return void
 *
 */
void net_wsd_print_config( net_wsd_print_config_t *config );

/**********************************************
 *  This section maps internal FW values to
 *  their WSD equivalent values
 **********************************************/

/**
 * @brief Map fw value for mediatype to WSD name
 * 
 * @param[in] input FW value for the mediatype
 * 
 * @param[out] output Pointer to storage allocated by caller
 *       used to pass back the variable value.
 * 
 * @retval WSD_OK output has valid data
 * 
 * @retval WSD_FAIL Failed to find match
 * 
 * This will return the string equivalent of the mediatype code
 * passed into the function.  If no match is found *output will
 * be NULL and the return type will indicate a failure
 */
error_type_t net_wsd_map_mediatype( mediatype_t input, char **output );

/**
 * @brief Map fw value for mediasize to WSD name
 * 
 * @param[in] input FW value for the mediatype
 * 
 * @param[out] output Pointer to storage allocated by caller
 *       used to pass back the variable value.
 * 
 * @retval WSD_OK output has valid data
 * 
 * @retval WSD_FAIL Failed to find match
 * 
 * This will return the string equivalent of the mediasize code
 * passed into the function.  If no match is found *output will be
 * NULL and the return type will indicate a failure
 */
error_type_t net_wsd_map_mediasize( mediasize_t input, char **output );

/**
 * @brief Map fw value for color enum to WSD name
 * 
 * @param[in] color FW value for the color
 * 
 * @param[out] output Pointer to storage allocated by caller
 *       used to pass back the variable value.
 * 
 * @retval WSD_OK output has valid data
 * 
 * @retval WSD_FAIL Failed to find match
 * 
 * This will return the string equivalent of the color enum
 * passed into the function.  If no match is found *output will
 * be NULL and the return type will indicate a failure
 */
error_type_t net_wsd_map_color( COLOR color, char **output );

/**
 * 
 * @brief Retrieve status conditions table for WSD Print events
 * 
 * @param[out] table Pointer to table
 * @param[out] elements Number of entries in the table
 * 
 * @return error_type_t OK 
 *  
 * This function will retrieve the product specific table which 
 * maps system status values to WSD values.
 */
error_type_t net_wsd_get_printer_status_condition_table( wsd_get_printer_status_condition_options_t **table, uint32_t *elements );

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // INC_WSD_PRINT_CONFIG_H 
