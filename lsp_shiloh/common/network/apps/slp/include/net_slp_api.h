/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_SLP_API_H
#define NET_SLP_API_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file net_slp_api.h
 * 
 * @brief Service Location Protocol (SLP) network application
 *
 * An implementation of SLPv1 and SLPv2 over UDP port 427 (default) conforming to RFCs 2165 and
 * 2608, respectively.
 */

/**
 * @brief Initialize the NVRAM block for the SLP network application.
 *
 * @param[in] init_type Type of init to perform (use stored values, manufacturer defaults, etc.)
 *
 * This routine should be called exactly once following a system startup.  It initializes the
 * module's persistent variables to either the values previously stored in NVRAM or manufacturer
 * defaults.
 *
 * PREREQUISITES: Requires prior initialization of the NVRAM driver and partition manager.
 */
void net_slp_var_init(void);

/**
 * @brief Initialize the SLP network application.
 * 
 * This routine should be called exactly once following a system startup. It initializes the
 * application state machine and then, if the SLP "enabled" variable is set to "true", begins
 * responding to SLPv1 and SLPv2 discovery requests over UDP port 427 (default) that match the
 * device's registered services. Service advertisements are defined at compile time via the SLP
 * platform configuration file.
 * 
 * PREREQUISITES: Requires prior initialization of the network interface module.
 */
void net_slp_init(void);

#ifdef __cplusplus
}
#endif

#endif // NET_SLP_API_H

