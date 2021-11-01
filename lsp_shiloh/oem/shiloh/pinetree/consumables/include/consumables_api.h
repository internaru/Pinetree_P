/*
 *
 * ============================================================================
 * Copyright (c) 2012   Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: This file is the external API for common consumables interactions.   
 *
 **/
 
/** 
 * \file consumables_api.h
 * 
 * \brief This api is the common api for consumables notifications and interactions. 
 * This api is based on the requirements of the snmp printer mib and WSD.  It serves 
 * as a common api for these protocols as well as other elements of the system to 
 * obtain needed consumables information. 
 *  
 * It is expected that oems will need to create their own custom interfaces for 
 * consumables.   This should be done by creating a new oem specific API, not by 
 * extending this API.  OEMs must implement this API in order to support the requirements 
 * of the network protocols. 
 *  
 * \details Implementation Details: 
 * Any routine in this API that does not explicitly return  an error code should assert 
 * on error conditions. 
 *  
 * There are two major classes of consumables: filled supplies and recepticles.  A filled 
 * supply is something like a toner cartridge or ink tank.  A recepticle is something 
 * like a waste toner hopper.  When reporting max capacity all consumables should report 
 * their maximum "filled" capacity.  When reporting level filled supplies should count 
 * the level down from full to empty.  Recepticles should count the level up from empty 
 * to full.  Consumables of class other should behave the same as a filled supply. 
 *  
 * If an oem wishes to count down the level of a consumable such as a waste toner hopper 
 * it should be designated as a filled supply or as class other. 
 *  
 * Note also that the percent remaining routines always count down from 100% to 0%. 
 */

#ifndef _CONSUMABLES_API_H
#define _CONSUMABLES_API_H

#if defined HAVE_COMMON_CONSUMABLES_API
#include "consumables_common_api.h"
#endif

#endif // _CONSUMABLES_API_H

