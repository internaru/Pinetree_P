/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file hwconfig_api.h
 *
 * \brief This file exposes a set of functions that allow access to some hardware
 * specific values, like CPU clock speed, and memory size.
 *
 **/


#ifndef HWCONFIGAPI_H
#define HWCONFIGAPI_H
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \brief Call init functions for platform specific modules.
 * 
 * \retval None
 * 
 **/
void hwConfigInit(void);

void hwTimebaseInit( void );
/**
 * \brief get the ASIC id and version numbers 
 * 
 * \param[out] platform
 * \param[out] major_rev
 * \param[out] minor_rev
 *
 * \retval None
 */

void hwGetASICId( uint32_t *platform, uint8_t *major_rev, uint8_t *minor_rev );



/** 
 * \brief return processor clock speed in megahertz.
 * 
 * \retval Processor speed in megahertz
 * 
 **/
uint32_t hwGetProcSpeed(void);

/**
 * \brief return the bus speed in MHz
 * \retval Bus speed in MHz
 */
uint32_t hwGetBusSpeed(void);

uint32_t hw_get_scan_clk_speed(void);

/**
 * \brief return the debug uart number
 * \retval Debug uart number
 */

uint32_t hwGetDebugUARTNumber(void);

/**
 * \brief Get the address where ram starts
 * \retval Ram start address
 */

uint32_t hwGetRamStartAddress(void);

/// return beginning address of uncached ram.
uint32_t hwGetUncachedRamStartAddress(void);

/// return number of bytes of uncached ram.
/// 0 if no uncached ram is available.
uint32_t hwGetUncachedRamSize(void);

/// true if a cached address false if uncached.
bool hwAddressIsCached( void *startAddr );


/** 
 * \brief return the size of ram in bytes
 * 
 * \retval ram size in bytes
 * 
**/
uint32_t hwGetRamSize(void);

/**
 * \brief reset the board
 * 
 */
void hwPlatformReset(void);

/**
 * \brief initialize the debug interface for the hwconfig module
 * The init for this module is called long before the debug system is initialized
 * This api allows the debug commands to be accessed by the rest of the system.
 */
void hwConfigDebugInit(void);



#ifdef __cplusplus
}
#endif

#endif
