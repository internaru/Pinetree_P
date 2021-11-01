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
 * \file consumables_common_api.h
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

#ifndef _CONSUMABLES_COMMON_API_H
#define _CONSUMABLES_COMMON_API_H

#include <stdbool.h>
#include "consumables_config.h"
#include "error_types.h"

// Change CM_.. to CONSUMABLE_...
// Add OTHER selection to consumable_color_t
// all routines will assert if they don't explicitly check error conditions
// Document that is_installed is identical to CM_STATUS_REMOVED.
// Comment that consumables that are containers count down, recepticle's count up.
// Do something about the event macros.
// Consumable units - add drops, pixels, kilopixels.


/**
 * \brief Custom error_type_t defines for this module
 */
#define CONSUMABLE_ERROR_UNKNOWN        -11     ///< The information requested about the consumable is not known.

/**
 * \brief The type of the consumable.  As required by the WSD Print Service specification 
 * ( a subset of the RFC1759 ). 
 */
typedef enum _consumable_type
{
    CONSUMABLE_TYPE_TONER,
    CONSUMABLE_TYPE_WASTE_TONER,
    CONSUMABLE_TYPE_INK,
    CONSUMABLE_TYPE_WASTE_INK,
    CONSUMABLE_TYPE_OPC,
    CONSUMABLE_TYPE_DEVELOPER,
    CONSUMABLE_TYPE_FUSER_OIL,
    CONSUMABLE_TYPE_WAX,
    CONSUMABLE_TYPE_WASTE_WAX,
    CONSUMABLE_TYPE_STAPLES
}consumable_type_t;

/**
 * \brief The color of the consumable.  As required by the WSD Print Service specification 
 * ( a subset of the RFC1759 ). 
 */
typedef enum _consumable_color
{
    CONSUMABLE_COLOR_BLACK,
    CONSUMABLE_COLOR_CYAN,
    CONSUMABLE_COLOR_MAGENTA,
    CONSUMABLE_COLOR_YELLOW,
    CONSUMABLE_COLOR_RED,
    CONSUMABLE_COLOR_GREEN,
    CONSUMABLE_COLOR_BLUE,
    CONSUMABLE_COLOR_COLOR, 
    CONSUMABLE_COLOR_GRAY,
    CONSUMABLE_COLOR_WHITE,
    CONSUMABLE_COLOR_PHOTO_BLACK,
    CONSUMABLE_COLOR_PHOTO_BLUE,
    CONSUMABLE_COLOR_PHOTO_COLOR,
    CONSUMABLE_COLOR_PHOTO_CYAN,
    CONSUMABLE_COLOR_PHOTO_MAGENTA,
    CONSUMABLE_COLOR_PHOTO_YELLOW,
    CONSUMABLE_COLOR_OTHER,
    CONSUMABLE_COLOR_NONE
}consumable_color_t;

/**
 * \brief The units of the consumable.  This is the full set specified by RFC3805 and 
 * is a superset of RFC1759. 
 */
typedef enum _consumable_units
{
    CONSUMABLE_UNITS_OTHER,             // -- New, not in RFC 1759
    CONSUMABLE_UNITS_UNKNOWN,           // -- New, not in RFC 1759
    CONSUMABLE_UNITS_TEN_THOUSANDTHS_OF_INCHES = 3,
    CONSUMABLE_UNITS_MICROMETERS,
    CONSUMABLE_UNITS_IMPRESSIONS,       // -- New, not in RFC 1759
    CONSUMABLE_UNITS_SHEETS,            // -- New, not in RFC 1759
    CONSUMABLE_UNITS_HOURS,             // -- New, not in RFC 1759
    CONSUMABLE_UNITS_THOUSANDTHS_OF_OUNCES = 12,
    CONSUMABLE_UNITS_TENTHS_OF_GRAMS,
    CONSUMABLE_UNITS_HUNDREDTHS_OF_FLUID_OUNCES,
    CONSUMABLE_UNITS_TENTHS_OF_MILLILITERS = 15,
    CONSUMABLE_UNITS_FEET,              // -- New, not in RFC 1759
    CONSUMABLE_UNITS_METERS,            // -- New, not in RFC 1759
    CONSUMABLE_UNITS_ITEMS,             // -- e.g., #staples. New, not in RFC 1759
    CONSUMABLE_UNITS_PERCENT,           // -- New, not in RFC 1759
    CONSUMABLE_UNITS_DROPS,             // -- Not in RFC3805 or RFC1759
    CONSUMABLE_UNITS_PIXELS,            // -- Not in RFC3805 or RFC1759
    CONSUMABLE_UNITS_KILOPIXELS = 18,   // -- Not in RFC3805 or RFC1759 (see Printer-MIB.txt)
}consumable_units_t;

/**
 * \brief The class of the consumable.  From RFC1759. 
 */
typedef enum _consumable_class
{
    CONSUMABLE_CLASS_CONSUMED_SUPPLY,
    CONSUMABLE_CLASS_FILLED_RECEPTACLE,
    CONSUMABLE_CLASS_OTHER
}consumable_class_t;

/**
 * \brief The current status of the consumable.
 */
typedef enum _consumable_status
{
    CONSUMABLE_STATUS_UNKNOWN,
    CONSUMABLE_STATUS_REMOVED,  ///< consumable not installed in the device
    CONSUMABLE_STATUS_NORMAL,   ///< consumable installed in the device; operating normally
    CONSUMABLE_STATUS_LOW,      ///< consumable installed; low
    CONSUMABLE_STATUS_OUT,      ///< consumable installed; out
    CONSUMABLE_STATUS_ERROR     ///< consumable installed; but in error state call consumable_get_error for details
}consumable_status_t;

/**
 * \brief The role of the consumable.  As specified by snmp's Printer-MIB.
 */
typedef enum _colorant_role
{
    CONSUMABLE_COLORANT_OTHER = 1,
    CONSUMABLE_COLORANT_PROCESS = 3,
    CONSUMABLE_COLORANT_SPOT = 4
} colorant_role_t;

/**
 * \brief Get the number of consumables supported by the device.
 *
 * \retval int The number of consumables supported by the device.
 *
 **/
int consumable_get_num_consumables();

/**
 * \brief Get the type of the specfied consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_type_t The type of the consumable.
 *
 **/
consumable_type_t consumable_get_type(int consumable_num);

/**
 * \brief Get the color of the specfied consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_color_t The color of the consumable.
 *
 **/
consumable_color_t consumable_get_color(int consumable_num);

/**
 * \brief Get the class of the specfied consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_class_t The class of the consumable.
 *
 **/
consumable_class_t consumable_get_class(int consumable_num);


/**
 * \brief Get the installed state of the consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval bool True if installed; else false.
 *  
 * Note that consumable_is_installed is a convenience wrapper 
 * around consumable_get_status. consuamble_is_installed 
 * returns true when consumable_get_status != REMOVED. 
 **/
bool consumable_is_installed(int consumable_num);

/**
 * \brief Get the status of the specfied consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_status_t The status of the consumable. 
 *  
 * If the status indicates that the consumable is in error condition 
 * call consumable_get_error for details of the error. 
 *
 **/
consumable_status_t consumable_get_status(int consumable_num);

/**
 * \brief Get the error information of the specfied consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_error_t The error condition of the consumable. 
 *  
 * If the cartridge is not in an error state CONSUMABLE_ERROR_NONE is returned. 
 * Otherwise an oem defined error is returned as specified in 
 * consumables_config.h. 
 *
 **/
consumable_error_t consumable_get_error(int consumable_num);

/**
 * \brief Get the name of the specfied consumable as a 
 * null terminated string.
 *  
 * \param[in]  consumable_num  The number of the consumable. 
 *  
 * \param[out] buf Buffer to be filled with the consumable name. 
 *  
 * \param[in][out] size On entry contains the size of the passed in 
 * buffer specfied by buf.  On return contains the size of the name 
 * including the terminating NULL.
 *
 * \retval error_type_t OK on success 
 *                      FAIL if buf is too small.
 *  
 * If the passed in buf is too small FAIL will be returned and the 
 * size parameter will be set to the size required. 
 *  
 **/
error_type_t consumable_get_name(int consumable_num, char* buf, int* size);

/**
 * \brief Get the model of the specfied consumable as a 
 * null terminated string.
 *  
 * \param[in]  consumable_num  The number of the consumable. 
 *  
 * \param[out] buf Buffer to be filled with the consumable model. 
 *  
 * \param[in][out] size On entry contains the size of the passed in 
 * buffer specfied by buf.  On return contains the size of the string 
 * including the terminating NULL.
 *
 * \retval error_type_t OK on success 
 *                      FAIL if buf is too small.
 *  
 * If the passed in buf is too small FAIL will be returned and the 
 * size parameter will be set to the size required. 
 *  
 **/
error_type_t consumable_get_model(int consumable_num, char* buf, int* size);

/**
 * \brief Get the percent of the consumable remaining.  This is the 
 * the accurate percentage in units of 1%. 
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval int The percent of the consumable remaining [0..100]. 
 * -1 is returned if the level cannot be determined. 
 *
 **/
int consumable_get_percent_remaining(int consumable_num);

/**
 * \brief Get the stepped percent of the consumable remaining.  This is the 
 * the percentage is in units of 1%, but is stepped down according 
 * to an algorithm determined by the oem implementer.  For example, 
 * the percent may step down in units of 10%, 20% or some other 
 * value desired by the oem. 
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval int The percent of the consumable remaining [0..100]. 
 * -1 is returned if the level cannot be determined. 
 *
 **/
int consumable_get_percent_remaining_stepped(int consumable_num);


/**
 * \brief Get the actual units of measurement of the consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_units_t The units tracked by the consumable.
 *
 **/
consumable_units_t consumable_get_units(int consumable_num);

/**
 * \brief Get the maximum capacity of the consumable in units 
 * defined by the consumable_get_units( ) routine.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval int The maximum capacity of the consumable. 
 * -1 means other and indicates that there are no restrictions on this parameter. 
 * -2 means unknown. 
 *
 **/
int consumable_get_max_capacity_in_units(int consumable_num);

/**
 * \brief Get the current level of the consumable in units 
 * defined by the consumable_get_units( ) routine.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval int The current level of the consumable. 
 * -1 means other and indicates that there are no restrictions on this parameter. 
 * -2 means unknown. 
 * -3 means that the consumable is not out, but no other level information can be determined. 
 *
 **/
int consumable_get_level_in_units(int consumable_num);

/**
 * \brief Get the serial number of the specfied consumable as a 
 * null terminated string.
 *  
 * \param[in]  consumable_num  The number of the consumable. 
 *  
 * \param[out] buf Buffer to be filled with the consumable serial number. 
 *  
 * \param[in][out] size On entry contains the size of the passed in 
 * buffer specfied by buf.  On return contains the size of the string
 * including the terminating NULL.
 *
 * \retval error_type_t OK on success 
 *                      FAIL if buf is too small.
 *  
 * If the passed in buf is too small FAIL will be returned and the 
 * size parameter will be set to the size required. 
 *  
 **/
error_type_t consumable_get_serial_number(int consumable_num, char* buf, int* size);

/**
 * \brief Get the number of impressions of the consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval int The number of impressions of the consumable. 
 * -1 means unknown
 *
 **/
int consumable_get_num_impressions(int consumable_num);

/**
 * \brief Get the date the consumable was first installed as a
 * null terminated string of the form YYYYMMDD.
 *  
 * \param[in]  consumable_num  The number of the consumable. 
 *  
 * \param[out] buf Buffer to be filled with the install date. 
 *  
 * \param[in][out] size On entry contains the size of the passed in 
 * buffer specfied by buf.  On return contains the size of the returned string 
 * including the terminating NULL.
 *
 * \retval error_type_t OK on success 
 *                      FAIL if buf is too small.
 *                      CONSUMABLE_ERROR_UNKNOWN if the date is not known
 *  
 * If the passed in buf is too small FAIL will be returned and the 
 * size parameter will be set to the size required. 
 *  
 **/
error_type_t consumable_get_date_installed(int consumable_num, char* buf, int* size);

/**
 * \brief Get the date the consumable was last used as a
 * null terminated string of the form YYYYMMDD.
 *  
 * \param[in]  consumable_num  The number of the consumable. 
 *  
 * \param[out] buf Buffer to be filled with the last used date. 
 *  
 * \param[in][out] size On entry contains the size of the passed in 
 * buffer specfied by buf.  On return contains the size of the returned string 
 * including the terminating NULL.
 *
 * \retval error_type_t OK on success 
 *                      FAIL if buf is too small.
 *                      CONSUMABLE_ERROR_UNKNOWN if the date is not known
 *  
 * If the passed in buf is too small FAIL will be returned and the 
 * size parameter will be set to the size required. 
 *  
 **/
error_type_t consumable_get_date_last_used(int consumable_num, char* buf, int* size);

/****************************************************************************/
/** Consumables Events                                                      */
/****************************************************************************/

/**
 * \brief consumable_event_callback prototype
 * This is the function prototype for all consumable event callbacks.  When 
 * the callback occurs the number of the consumable for which the event occurred 
 * is passed to the callback.
 */
typedef void (* consumable_event_callback) (uint32_t consumable_num);

/**
 * \brief Consumables_status_change
 * This event means that the status of a consumable has been updated.
 * The event provides the number of the consumable that changed.
 * Users interested in the status of the consumable when this
 * event occurs should query the consumable using the consumables api.
 * This event is triggered for all consumable status changes other
 * than level changes.  This includes installed, removed, and error
 * notifications.
 */
void consumable_status_change_event_attach(consumable_event_callback pfn_event_callback);
error_type_t consumable_status_change_event_detach(consumable_event_callback pfn_event_callback);

/**
 * \brief consumables_level_change
 * This event means that the level of a consumable has been updated.
 * The event provides the number of the consumable that changed.
 * Users interested in the level of the consumable when this
 * event occurs should query the consumable using the consumables api.
 */
void consumable_level_change_event_attach(consumable_event_callback pfn_event_callback);
error_type_t consumable_level_change_event_detach(consumable_event_callback pfn_event_callback);

/**
 * \brief consumables_level_change_stepped
 * This event means that the stepped level of a consumable has been updated.
 * The event provides the number of the consumable that changed.
 * Users interested in the level of the consumable when this
 * event occurs should query the consumable using the consumables api.
 */
void consumable_level_change_stepped_event_attach(consumable_event_callback pfn_event_callback);
error_type_t consumable_level_change_stepped_event_detach(consumable_event_callback pfn_event_callback);

/**
 * \brief consumable_get_supply_tonality
 * Returns specified consumable's tonality (see snmp Printer-MIB)
 */
error_type_t consumable_get_supply_tonality(int consumable_num, uint32_t* tonality);

/**
 * \brief consumable_get_colorant_role
 * Returns specified consumable's role (see snmp Printer-MIB)
 */
error_type_t consumable_get_colorant_role(int consumable_num, colorant_role_t* role);

/**
 * \brief consumable_get_default_orientation
 * Returns specified consumable's (interpreter's?) orientation (see snmp Printer-MIB)
 */
error_type_t consumable_get_default_orientation(int consumable_num, uint32_t* default_orientation);

/**
 * \brief consumable_set_default_orientation
 * Returns specified consumable's (interpreter's) orientation (see snmp Printer-MIB)
 */
error_type_t consumable_set_default_orientation(int consumable_num, uint32_t update);

#endif // _CONSUMABLES_COMMON_API_H

