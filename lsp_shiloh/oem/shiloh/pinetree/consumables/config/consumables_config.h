/**
 *
 * ============================================================================
 * Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

/** 
 * \file consumables_config.h
 * 
 * \brief Configuration file for the common consumables api.
 */
#ifndef _CONSUMABLES_CONFIG_H
#define _CONSUMABLES_CONFIG_H

// oem expandable list of consumable errors
typedef enum _consumable_error
{
    CM_ERROR_NONE,          // consumable not in error condition
    CM_ERROR_WRONG_COLOR,   // a consumable of the wrong color is installed in the position e.g. cyan in magenta position
}consumable_error_t;

#endif // _CONSUMABLES_CONFIG_H
