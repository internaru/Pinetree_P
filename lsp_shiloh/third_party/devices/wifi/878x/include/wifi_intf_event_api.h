/** 
 * ============================================================================
 * Copyright (c) 2009-2012   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file wifi_intf_event_api.h
 * 
 * @brief Wireless link status API
 * 
 * Observers are notified of link status changes.
 */

#ifndef wifi_intf_event_api_h
#define wifi_intf_event_api_h

#include "event_observer.h"

// Wifi Interface type
typedef enum wifi_intf_type_e
{
	WIFI_INTF_STA,
	WIFI_INTF_UAP,
} wifi_intf_type_t;

// Control Panel link status defines
typedef enum wifi_intf_link_status_e
{
	Link_Down,
	Link_Connected,
	Link_Connecting,
	Link_Error,
	Link_Device_Connecting
} wifi_intf_link_status_t;

void wifi_intf_attach_observer_event( Observer_t *o);
void wifi_intf_detach_observer_event( Observer_t *o);
wifi_intf_type_t wifi_intf_get_intf_type( Subject_t *s);
wifi_intf_link_status_t wifi_intf_get_link_status( Subject_t *s);
error_type_t wifi_intf_get_device_name( Subject_t *s, char * device_name, unsigned int *device_name_len);
error_type_t wifi_intf_get_mac_addr( Subject_t *s, unsigned char * mac_addr);
error_type_t wifi_intf_get_ip_addr( Subject_t *s, unsigned char * ip_addr);
#endif // wifi_intf_event_api_h
