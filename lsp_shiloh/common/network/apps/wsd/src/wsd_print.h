/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef WSD_PRINT_H
#define WSD_PRINT_H

#include <stdint.h>
#include <stdbool.h>
#include "net_io_api.h"
#include "wsd.h"
#include "wprtStub.h"
#include "posix_ostools.h"
#include "paper_size_config.h"
#include "paper_types_config.h"


typedef enum 
{                                               // value            data
    WSD_PRINTER_ELEMENT_NUM_COPIES,             // # Copies         --
    WSD_PRINTER_ELEMENT_COLLATE,                // bool on/off      --
    WSD_PRINTER_ELEMENT_DEFAULT_MEDIA_SIZE,     // New size         --
    WSD_PRINTER_ELEMENT_DEFAULT_MEDIA_TYPE,     // New type         --
    WSD_PRINTER_ELEMENT_EVENT_RATE,             // New rate         --
    WSD_PRINTER_ELEMENT_CONSUMABLE_LEVEL,       // Color            int Level
    WSD_PRINTER_ELEMENT_MEDIA_IN_TRAY,          // Tray             --
    WSD_PRINTER_ELEMENT_LOCATION,               // --               char *Location
} wsd_printer_elements_changed_t;

typedef struct wsd_element_change_s
{
    wsd_printer_elements_changed_t  element;
    uint32_t                        value;
    void                            *data;
} wsd_print_element_change_t;

typedef struct wsd_get_printer_status_options_s
{
    uint32_t        status_category;
    char            *printer_state;
    char            *printer_primary_state_reason;
    char            *printer_state_reason;
} wsd_get_printer_status_options_t;

typedef struct
{
    uint32_t                                status;
    enum wprt__ComponentGroupBaseType       component_group;
    char                                    *component_name;
    enum wprt__ConditionNameBaseType        name;
    enum wprt__ConditionSeverityBaseType    severity;
} wsd_get_printer_status_condition_options_t;

//=========================================================================================================================
// Private function prototypes
//=========================================================================================================================

void wsd_print_events_init( void );
void wsd_print_events_shutdown( void );
wsd_get_printer_status_options_t *wsd_get_status_values( uint32_t status );
void wsd_populate_consumable_structure( struct dpws* ws, struct wprt__ConsumableEntryType *consumable, char *color, uint32_t level, bool event );
void wsd_populate_input_bins_structure( struct dpws* ws, struct wprt__InputBinEntryType *bin, uint32_t capacity, uint32_t level, mediasize_t size, mediatype_t type, uint32_t index );

// called from threads
#define wsd_print_send_msg(type, data)         wsd_print_send_msg_wait_opt(type, data, POSIX_WAIT_FOREVER)

// called from non-threads (e.g. timers)
#define wsd_print_send_msg_no_wait(type, data) wsd_print_send_msg_wait_opt(type, data, 0)

/**
 * @brief Sends intra-wsd meesage
 * 
 * @param[in] type Message type
 * 
 * @param[in] data message data
 * 
 * @param[in] threadx_wait_opt How long to wait for messgae to
 *       send
 * 
 * @retval NET_WSD_OK Message has been sent
 * 
 * @retval NET_WSD_WOULDBLOCK Queue is full
 * 
 */
int32_t wsd_print_send_msg_wait_opt( net_wsd_msg_type_t type, void *data, unsigned long threadx_wait_opt);

#endif // WSD_PRINT_H

