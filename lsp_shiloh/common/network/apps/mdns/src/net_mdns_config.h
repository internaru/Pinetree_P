/*
 * ============================================================================
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  @file net_mdns_config.h
 *  
 *  @brief This file provides an interface for OEM's to add specific
 *         customizations to the mDNS service broadcasts.
 * 
 */

#ifndef NET_MDNS_CONFIG_H
#define NET_MDNS_CONFIG_H

#include "error_types.h"
#include "platform.h"


#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------
// API Function Declarations
//--------------------------------------

/**
 * @brief Get the current config values for mDNS
 *
 * @param[out] config Pointer to storage allocated by caller
 *       used to pass back the config structure.
 *
 * @return void
 *
 */
void net_mdns_config( void );


#ifdef __cplusplus
}
#endif

#endif // NET_MDNS_API_H

