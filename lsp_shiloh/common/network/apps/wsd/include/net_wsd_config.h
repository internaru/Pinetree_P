/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_wsd_config.h
 *
 * \brief This file implements the public configuration API for
 * the WSD.
 * 
 **/

#ifndef NET_WSD_CONFIG_H
#define NET_WSD_CONFIG_H

//--------------------------------------
// Included files
//--------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <error_types.h>
#include "net_wsd_api.h"
#include "dc/dc_Types.h"	

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------
// API Declarations and Type Definitions
//--------------------------------------

/**
 * @brief WS-Transfer Custom Namespace structure
 * 
 * This structure allows for custom namespaces to be included
 * any WS-Transfer requests received by the device. 
 *  
 * If localname is NULL then the namespace will be added to 
 * any discovery messages.  Otherwise a wsd:Types entry will be 
 * added and the namespace will be added. 
 */
typedef struct net_wsd_wstransfer_custom_namespace
{
    char *ns;           // Custom namespace
    char *prefix;       // Prefix for namespace
    char *localname;    // Local name
} net_wsd_wstransfer_custom_namespace_t;

/**
 * @brief WS-Transfer Custom Hosted Services structure
 * 
 * This structure allows for custom hosted services to be
 * included any WS-Transfer requests received by the device.
 */
typedef struct net_wsd_wstransfer_custom_hosted_service
{
    net_wsd_wstransfer_custom_namespace_t type; // Namespace for service
    char **addresses;                           // Addresses to access service
    uint32_t num_addresses;                     // How many addresses in array above
    char *service_id;                           // Service ID
} net_wsd_wstransfer_custom_hosted_service_t;

/**
 *@brief A function table structure for various custom functions
 *
 *This structure contains custom function pointers for wsd related
 *OEM customizations.
 */
typedef struct wsd_custom_function_table_type
{
    error_type_t (*wsd_get_wstransfer_custom_namespace)( net_wsd_wstransfer_custom_namespace_t **ns, uint32_t *num ); //Retrieve any custom namespaces to be sent in WS-Transfer response
    void (*wsd_free_wstransfer_custom_namespace)( net_wsd_wstransfer_custom_namespace_t *ns, uint32_t num ); //Free any memory malloced in call to wsd_get_wstransfer_custom_namespace
    error_type_t (*wsd_get_wstransfer_custom_hosted_service)( net_wsd_wstransfer_custom_hosted_service_t ***hs, uint32_t *num );//Provide the information necessary to add any custom hosted services to the WS-Transfer requests
    void (*wsd_free_wstransfer_custom_hosted_service)( net_wsd_wstransfer_custom_hosted_service_t **hs, uint32_t num );//Free any memory malloced for the custom hosted services call
    error_type_t (*directed_probes_custom_header)( uint32_t request );//Add custom http header in directed probe response
    void (*wsd_add_vendor_extension_to_probe_match)( struct soap *soap, const void *hello_info );//Add vendor extension to discovery responses
}wsd_custom_func_table_t;

/**
 * @brief WSD Configuration structure
 * 
 * This structure contains the configuration options that apply
 * to WS-Print and WS-Scan.  
 */
typedef struct net_wsd_config
{
    bool        sys_restore_wsd_enabled;                 // Is WSD enabled by default on the system?
    uint32_t    port;                                    // Port used as WSD listener
    uint32_t    default_event_rate;                      // Delay in seconds between CompleteState events
    uint32_t    transactions_per_socket;                 // If greater than one socket keep alive is used
    uint8_t     num_dispatcher_threads;                  // Number of threads waiting to respond to WSD requests
    uint8_t     num_job_history_elements;                // Number of jobs kept in job history
    char        *manufacturer_url;                       // URL of the manufacturer
    char        *support_url;                            // URL of the manufacturer's service support site
    char        **service_name;                          // scan and print service name
    char        *model_num;                              // Abbreviated name of the device
    char        *serial_num;                             // Serial number of the device
    wsd_custom_func_table_t *wsd_custom_func_table;      // A function table pointer for wsd related OEM customizations
} net_wsd_config_t;

//--------------------------------------
// API Function Declarations
//--------------------------------------

/**
 * @brief Get the current config values for WSD
 *
 * @param[out] config Pointer to storage allocated by caller
 *       used to pass back the config structure.
 *
 * @return void
 *
 */
void net_wsd_config( net_wsd_config_t *config );

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // NET_WSD_CONFIG_H
