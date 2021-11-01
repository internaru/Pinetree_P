/*
 *
 * ============================================================================
 * Copyright (c) 2012   Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: This file is the reference implmentation of the
 * consumables common API.
 *
 **/
 
/** 
 * \file consumables_common_stub.c
 * 
 * \brief This is a stub or reference implementation of the consumables API. 
 * It is provided as a sample on which to base other implementations and to allow 
 * code dependent on the consumables api to function and be tested in the absense 
 * of an actual consumables implementation.  This reference implementation is for 
 * a color printer with 4 consumables. 
 *  
 * Actual implementation of the api should not be done in this directory.  OEMs 
 * should copy this file to an appropriate location in the oem directory tree 
 * and implement the detail of their consumables implementation in that location. 
 *  
 * This api is the common api for consumables notifications and interactions. 
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
 * There are two major classes of consumables: filled supplies and receptacles.  A filled 
 * supply is something like a toner cartridge or ink tank.  A receptacle is something 
 * like a waste toner hopper.  When reporting max capacity all consumables should report 
 * their maximum "filled" capacity.  When reporting level filled supplies should count 
 * the level down from full to empty.  Receptacles should count the level up from empty 
 * to full.  Consumables of class other should behave the same as a filled supply. 
 *  
 * If an oem wishes to count down the level of a consumable such as a waste toner hopper 
 * it should be designated as a filled supply or as class other. 
 *  
 * Note also that the percent remaining routines always count down from 100% to 0%. 
 */

#include <string.h>
#include "consumables_common_api.h"
#include "consumables_config.h"
#include "lassert.h"
#include "ATypes.h"
#include "mlist.h"
#include "memAPI.h"

#include "oid_api.h"

#ifdef HAVE_SM_JOB
#include "oid_sm_api.h" // OID_SM_PRINT_ORIENTATION_REQUESTED_DEFAULT
#include "sm_job_api.h" // SMJOB_ORIENTATION_PORTRAIT
#endif

// #define NUM_CONSUMABLES 4 //  to block CMY toner remaining by mspyo, 2021-03-10
#define NUM_CONSUMABLES 1
#define MAX_NAME_LEN 32
#define MAX_MODEL_LEN 32
#define MAX_SERIAL_NUM_LEN 32
#define MAX_DATE_LEN 9

typedef struct _consumable_data
{
    consumable_type_t type;
    consumable_color_t color;
    consumable_class_t c_class;
    consumable_status_t current_status;
    consumable_error_t current_error;
    char name[MAX_NAME_LEN];
    char model[MAX_MODEL_LEN];
    int percent_remaining;
    char serial_num[MAX_SERIAL_NUM_LEN];
    int num_impressions;
    char install_date[MAX_DATE_LEN];
    char last_used_date[MAX_DATE_LEN];
    colorant_role_t role;
    uint32_t tonality; // .. 2**(bits per pixel)
}consumable_data_t;

static consumable_data_t consumable[NUM_CONSUMABLES] =
{
    {
        .type = CONSUMABLE_TYPE_TONER,
        .color = CONSUMABLE_COLOR_BLACK,
        .c_class = CONSUMABLE_CLASS_CONSUMED_SUPPLY,
        .current_status = CONSUMABLE_STATUS_NORMAL,
        .current_error = CM_ERROR_NONE,
        .name = "OEM_BLACK",
        .model = "OEM_K01",
        .percent_remaining = 100,
        .serial_num = "5551212",
        .num_impressions = 0,
        .install_date = "20010101",
        .last_used_date = "20010102",
        .role = CONSUMABLE_COLORANT_SPOT,
        .tonality = 2,
    }// , //  to block CMY toner remaining by mspyo, 2021-03-10
/*    {
        .type = CONSUMABLE_TYPE_TONER,
        .color = CONSUMABLE_COLOR_CYAN,
        .c_class = CONSUMABLE_CLASS_CONSUMED_SUPPLY,
        .current_status = CONSUMABLE_STATUS_NORMAL,
        .current_error = CM_ERROR_NONE,
        .name = "OEM_CYAN",
        .model = "OEM_C01",
        .percent_remaining = 100,
        .serial_num = "5551212",
        .num_impressions = 0,
        .install_date = "20010101",
        .last_used_date = "20010102",
        .role = CONSUMABLE_COLORANT_SPOT,
        .tonality = 2,
    },
    {
        .type = CONSUMABLE_TYPE_TONER,
        .color = CONSUMABLE_COLOR_MAGENTA,
        .c_class = CONSUMABLE_CLASS_CONSUMED_SUPPLY,
        .current_status = CONSUMABLE_STATUS_NORMAL,
        .current_error = CM_ERROR_NONE,
        .name = "OEM_MAGENTA",
        .model = "OEM_M01",
        .percent_remaining = 100,
        .serial_num = "5551212",
        .num_impressions = 0,
        .install_date = "20010101",
        .last_used_date = "20010102",
        .role = CONSUMABLE_COLORANT_SPOT,
        .tonality = 2,	
    },
    {
        .type = CONSUMABLE_TYPE_TONER,
        .color = CONSUMABLE_COLOR_YELLOW,
        .c_class = CONSUMABLE_CLASS_CONSUMED_SUPPLY,
        .current_status = CONSUMABLE_STATUS_NORMAL,
        .current_error = CM_ERROR_NONE,
        .name = "OEM_YELLOW",
        .model = "OEM_Y01",
        .percent_remaining = 100,
        .serial_num = "5551212",
        .num_impressions = 0,
        .install_date = "20010101",
        .last_used_date = "20010102",
        .role = CONSUMABLE_COLORANT_SPOT,
        .tonality = 2,
    }
*/
};

/**
 * \brief Get the number of consumables supported by the device.
 *
 * \retval int The number of consumables supported by the device.
 *
 **/
int consumable_get_num_consumables()
{
    return NUM_CONSUMABLES;
}

/**
 * \brief Get the type of the specfied consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_type_t The type of the consumable.
 *
 **/
consumable_type_t consumable_get_type(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return consumable[consumable_num].type;
}

/**
 * \brief Get the color of the specfied consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_color_t The color of the consumable.
 *
 **/
consumable_color_t consumable_get_color(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return consumable[consumable_num].color;
}

/**
 * \brief Get the class of the specfied consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_class_t The class of the consumable.
 *
 **/
consumable_class_t consumable_get_class(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return consumable[consumable_num].c_class;
}


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
bool consumable_is_installed(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return (consumable_get_status(consumable_num) != CONSUMABLE_STATUS_REMOVED);
}

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
consumable_status_t consumable_get_status(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return consumable[consumable_num].current_status;
}

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
consumable_error_t consumable_get_error(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return consumable[consumable_num].current_error;
}

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
error_type_t consumable_get_name(int consumable_num, char* buf, int* size)
{
    int name_len;

    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    ASSERT(buf != NULL);

    name_len = strlen(consumable[consumable_num].name)+1;
    if (*size < name_len)
    {
        *size = name_len;
        return FAIL;
    }

    *size = name_len;
    memcpy(buf, consumable[consumable_num].name, name_len);
    return OK;
}

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
error_type_t consumable_get_model(int consumable_num, char* buf, int* size)
{
    int model_len;

    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    ASSERT(buf != NULL);

    model_len = strlen(consumable[consumable_num].model)+1;
    if (*size < model_len)
    {
        *size = model_len;
        return FAIL;
    }

    *size = model_len;
    memcpy(buf, consumable[consumable_num].model, model_len);
    return OK;
}


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
int consumable_get_percent_remaining(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return consumable[consumable_num].percent_remaining;
}

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
int consumable_get_percent_remaining_stepped(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return (consumable[consumable_num].current_error/10)*10;
}


/**
 * \brief Get the actual units of measurement of the consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval consumable_units_t The units tracked by the consumable.
 *
 **/
consumable_units_t consumable_get_units(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return CONSUMABLE_UNITS_PERCENT;
}

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
int consumable_get_max_capacity_in_units(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return 100;
}

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
int consumable_get_level_in_units(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return consumable[consumable_num].percent_remaining;
}

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
error_type_t consumable_get_serial_number(int consumable_num, char* buf, int* size)
{
    int ser_len;

    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    ASSERT(buf != NULL);

    ser_len = strlen(consumable[consumable_num].serial_num)+1;
    if (*size < ser_len)
    {
        *size = ser_len;
        return FAIL;
    }

    *size = ser_len;
    memcpy(buf, consumable[consumable_num].serial_num, ser_len);
    return OK;
}


/**
 * \brief Get the number of impressions of the consumable.
 *  
 * \param[in]  consumable_num  The number of the consumable.
 *
 * \retval int The number of impressions of the consumable. 
 * -1 means unknown
 *
 **/
int consumable_get_num_impressions(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return consumable[consumable_num].num_impressions;
}

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
error_type_t consumable_get_date_installed(int consumable_num, char* buf, int* size)
{
    int date_len;

    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    ASSERT(buf != NULL);

    date_len = strlen(consumable[consumable_num].install_date)+1;
    if (*size < date_len)
    {
        *size = date_len;
        return FAIL;
    }

    *size = date_len;
    memcpy(buf, consumable[consumable_num].install_date, date_len);
    return OK;
}

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
error_type_t consumable_get_date_last_used(int consumable_num, char* buf, int* size)
{
    int date_len;

    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    ASSERT(buf != NULL);

    date_len = strlen(consumable[consumable_num].last_used_date)+1;
    if (*size < date_len)
    {
        *size = date_len;
        return FAIL;
    }

    *size = date_len;
    memcpy(buf, consumable[consumable_num].last_used_date, date_len);
    return OK;
}

/**
 * \brief consumable_get_supply_tonality
 *  
 * \param[in]  consumable_num  The number of the consumable. 
 *  
 * \param[out] tonality Buffer to be filled with the consumable's tonality
 *  
 * \retval error_type_t OK on success 
 *                      FAIL if return buffer is not provided.
 *  
 **/
error_type_t consumable_get_supply_tonality(int consumable_num, uint32_t* tonality)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    ASSERT(tonality);
    if (tonality) 
    {
        *tonality = consumable[consumable_num].tonality;
        return OK;
    }
    return FAIL;
}

error_type_t consumable_get_supply_name(int consumable_num, char* buf, int* size)
{
    int str_len;

    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    ASSERT(buf != NULL);

    str_len = strlen(consumable[consumable_num].name)+1;
    if (*size < str_len)
    {
        *size = str_len;
        return FAIL;
    }

    *size = str_len;
    memcpy(buf, consumable[consumable_num].name, str_len);
    return OK;
}

/**
 * \brief Get the colorant role for the indexed supply
 *  
 * \param[in]  consumable_num  The number of the consumable. 
 *  
 * \retval uint32_t role value (1-other, 3-process, 4-spot)
 *  
 **/
error_type_t consumable_get_colorant_role(int consumable_num, uint32_t* role)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    ASSERT(role);
    if (role)
    {
        *role = consumable[consumable_num].role;
        return OK;
    }
    return FAIL;
}

/**
 * \brief Get the current 'default' orientation (as in 'current print-pipe' (regardless of interpreter/parser))
 *  
 * \param[in]  consumable_num  The number of the consumable.  (revisit personality rather than consumable?)
 *  
 * \retval uint32_t orientation value (1-other, 3-portrait, 4-landscape)
 *  
 **/

#if HAVE_SM_JOB
// SM_JOB_API returns snmp/mib translation -- prtInterpreterDefaultOrientation_convert() unused
static uint32_t DefaultOrientation = SMJOB_ORIENTATION_PORTRAIT;

// lookup default from sm_job's current default 'print' job_ticket
error_type_t consumable_get_default_orientation(int index /* consumable or parser? */, uint32_t* default_orientation)
{
    if (index < 0 || index > NUM_CONSUMABLES) // out-of-range?
    {
        XASSERT(index >= 0 && index < NUM_CONSUMABLES, index);
        *default_orientation = SMJOB_ORIENTATION_PORTRAIT; // revisit portrait default
        return OK;
    }

    return oid_get_uint32(OID_SM_PRINT_ORIENTATION_REQUESTED_DEFAULT, index, default_orientation);
}

error_type_t consumable_set_default_orientation(int consumable_num, uint32_t value)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    DefaultOrientation = value; // revisit consumable or interpreter/parser (currently interpreter # is unavailable)
    return OK;
}
#else // without HAVE_SM_JOB
enum { default_Portrait = 1, default_Landscape = 2 };
static uint32_t DefaultOrientation = default_Portrait; // revisit typical personal-printer default
int consumable_get_default_orientation(int consumable_num)
{
    XASSERT(consumable_num >= 0 && consumable_num < NUM_CONSUMABLES, consumable_num);
    return DefaultOrientation;
}

error_type_t consumable_set_default_orientation(int index, uint32_t value)
{
    // check against consumables or parsers?
    XASSERT(index >= 0 && index < NUM_CONSUMABLES, index);
    DefaultOrientation = value;
    return OK;
}
#endif // HAVE_SM_JOB

/****************************************************************************/
/** Consumables Events                                                      */
/****************************************************************************/

typedef struct _consumable_event_node
{
    ATLISTENTRY list_node;
    consumable_event_callback pfn_event;
}consumable_event_node_t;

MLISTENTRY status_listeners;
MLISTENTRY level_listeners;
MLISTENTRY stepped_level_listeners;

void consumable_attach_to_listener_list(MLISTENTRY* listeners, consumable_event_callback pfn_event_callback)
{
    consumable_event_node_t* node = (consumable_event_node_t*)MEM_MALLOC(sizeof(consumable_event_node_t));
    ASSERT(node != NULL);
    ASSERT(pfn_event_callback != NULL);

    node->pfn_event = pfn_event_callback;
    MInsertTailList(listeners, &node->list_node);
}

error_type_t consumable_detach_from_listener_list(MLISTENTRY* listeners, consumable_event_callback pfn_event_callback)
{
    ATLISTENTRY *next = NULL;
    consumable_event_node_t* node = NULL;
    ASSERT(pfn_event_callback != NULL);

    // search the list to find the node with the callback to be detached
    MList_Lock(listeners);
    for (next = MListHead(listeners); next != NULL; next = MListNext(listeners, next)) 
    {
        node = CONTAINING_RECORD(next, consumable_event_node_t, list_node);
        ASSERT(node != NULL);
        if(node->pfn_event == pfn_event_callback)
        {
            MRemoveEntryList(listeners, &node->list_node);
            memFree(node);
            break;
        }
    }
    MList_UnLock(listeners);

    // if found node has now been freed and is now simply used as a bool flag 
    // to indicate if node was found; do not access
    return (node != NULL) ? OK : FAIL;
}

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
void consumable_status_change_event_attach(consumable_event_callback pfn_event_callback)
{
    consumable_attach_to_listener_list(&status_listeners, pfn_event_callback);
}

error_type_t consumable_status_change_event_detach(consumable_event_callback pfn_event_callback)
{
    return consumable_detach_from_listener_list(&status_listeners, pfn_event_callback);
}

/**
 * \brief consumables_level_change
 * This event means that the level of a consumable has been updated.
 * The event provides the number of the consumable that changed.
 * Users interested in the level of the consumable when this
 * event occurs should query the consumable using the consumables api.
 */
void consumable_level_change_event_attach(consumable_event_callback pfn_event_callback)
{
    consumable_attach_to_listener_list(&level_listeners, pfn_event_callback);
}

error_type_t consumable_level_change_event_detach(consumable_event_callback pfn_event_callback)
{
    return consumable_detach_from_listener_list(&level_listeners, pfn_event_callback);
}

/**
 * \brief consumables_level_change_stepped
 * This event means that the stepped level of a consumable has been updated.
 * The event provides the number of the consumable that changed.
 * Users interested in the level of the consumable when this
 * event occurs should query the consumable using the consumables api.
 */
void consumable_level_change_stepped_event_attach(consumable_event_callback pfn_event_callback)
{
    consumable_attach_to_listener_list(&stepped_level_listeners, pfn_event_callback);
}

error_type_t consumable_level_change_stepped_event_detach(consumable_event_callback pfn_event_callback)
{
    return consumable_detach_from_listener_list(&stepped_level_listeners, pfn_event_callback);
}

/** 
 * \brief Notify all listeners on the specified list that the even occurred 
 * on the specified consumable.
 */ 
void consumable_notify_event(MLISTENTRY* listeners, int consumable_num)
{
    ATLISTENTRY *next = NULL;
    consumable_event_node_t* node = NULL;
    ASSERT(listeners != NULL);

    // for each listener node call the callback
    MList_Lock(listeners);
    for (next = MListHead(listeners); next != NULL; next = MListNext(listeners, next)) 
    {
        node = CONTAINING_RECORD(next, consumable_event_node_t, list_node);
        ASSERT(node != NULL);
        ASSERT(node->pfn_event != NULL);
        node->pfn_event(consumable_num);
    }
    MList_UnLock(listeners);
}

/****************************************************************************/
/** Init                                                                    */
/****************************************************************************/

void consumable_api_init()
{
    MInitList(&status_listeners);
    MInitList(&level_listeners);
    MInitList(&stepped_level_listeners);
}

// eof
