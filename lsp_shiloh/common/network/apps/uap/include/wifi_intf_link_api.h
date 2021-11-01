/*
 * ============================================================================
 * Copyright (c) 2010-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file wifi_intf_link_api.h
 * 
 * @brief Wireless link API
 * 
 * This API exposes the wireless interface routines to the network link layer.
 */

#ifndef WIFI_INTF_LINK_API_H
#define WIFI_INTF_LINK_API_H

#include "data_access_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Network Link events
 */
typedef enum
{
    WIFI_INTF_LINK_EVENT_DOWN_PENDING,
    WIFI_INTF_LINK_EVENT_DOWN,
    WIFI_INTF_LINK_EVENT_UP,
    WIFI_INTF_LINK_EVENT_RESET
} wifi_intf_link_event_t;

/** Wireless Interface IOCTLs bitfield value
 */
#define WIFI_INTF_IOCTL_LINK_ENABLE         0x1
#define WIFI_INTF_IOCTL_LINK_DISABLE        0x2
#define WIFI_INTF_IOCTL_LINK_DOWN_ACK       0x4
#define WIFI_INTF_IOCTL_SET_MCAST_LIST      0x8
#define WIFI_INTF_IOCTL_IP_TRAFFIC_DETECTED 0x10
#define WIFI_INTF_IOCTL_SET_AUTO_ARP        0x20

/** Callback functions used in wifi_intf_init()
 */
typedef void (*wifi_intf_set_link_status_t)(wifi_intf_link_event_t link_event);
typedef void (*wifi_intf_get_rx_buffer_t)(char** rx_buff, int* buff_len, void** buff_desc);
typedef void (*wifi_intf_free_buffer_t)(void* buff_desc);
typedef int (*wifi_intf_recv_buffer_t)(void);
typedef void (*wifi_intf_complete_tx_packet_t)(void * handle, bool app_call);

/** Callback structure for initialization
 */
typedef struct 
{
    wifi_intf_set_link_status_t set_link_status;
    wifi_intf_get_rx_buffer_t get_rx_buffer;
    wifi_intf_free_buffer_t free_buffer;
    wifi_intf_recv_buffer_t recv_buffer;
    wifi_intf_complete_tx_packet_t complete_tx_packet;
} wifi_intf_callback_funcs_t;


/** Network handle struct
 */
typedef struct wifi_handle_s wifi_handle_t;

/**
 * @brief Get wireless interface current link status
 *
 * This routine gets the wireless interface current link status
 *
 */
uint32_t wifi_intf_sta_get_current_link_status(void);

/**
 * @brief Initialize the wireless interface
 *
 * This routine initialize the wireless interface and WLAN callback functions.
 *
 */
error_type_t wifi_intf_init(const wifi_intf_callback_funcs_t * wifi_intf_callback_funcs);

/**
 * @brief Initialize the NVRAM block for the wireless interface.
 *
 * @param init_type Type of init to perform (use stored values, manufacturer defaults, etc.)
 *
 * This routine should be called exactly once following a system startup.  It initializes the
 * module's persistent variables to either the values previously stored in NVRAM or manufacturer
 * defaults.
 *
 * PREREQUISITES: Requires prior initialization of the NVRAM driver and partition manager.
 */
void wifi_intf_var_init(void);

/*
 * @brief Open Wifi Interface
 *
 * @param handle Network handle
 */
error_type_t wifi_intf_open(wifi_handle_t * handle);

/*
 * @brief Close Wifi Interface
 *
 * @param handle Network handle
 */
error_type_t wifi_intf_close(wifi_handle_t * handle);

/*
 * @brief Send Wifi data
 *
 * @param handle Network handle
 * @param data_ptr Pointer to data to send
 * @param data_length Length of data to send
 */
error_type_t wifi_intf_send(wifi_handle_t * handle, char *data_ptr, int data_length);

/*
 * @brief Receive Wifi data
 *
 * @param handle Network handle
 * @param data_ptr Pointer to data to receive
 * @param data_length Length of data to receive
 * @param buf_handle_ptr User buffer handle pointer
 */
error_type_t wifi_intf_recv(wifi_handle_t * handle, char** data_ptr, uint32_t * data_length, void** buf_handle_ptr);

/*
 * @brief Wifi Ioctls
 *
 * @param handle Network handle
 * @param flag Ioctl flag
 * @param option_ptr Pointer to optional data for ioctl
 * @param option_len Length of optional data for ioctl
 */
error_type_t wifi_intf_ioctl(wifi_handle_t * handle, int flag, void *option_ptr, int option_len);

/*
 * @brief Get Wifi Physical MAC Address
 *
 * @param handle Network handle
 * @param physical_address Pointer to physical MAC address
 */
error_type_t wifi_intf_get_phys_addr(wifi_handle_t * handle,  char * physical_address);

/*
 * @brief Open Wifi Interface
 *
 * @param handle Network handle
 */
error_type_t wifi_uap_intf_open(wifi_handle_t * handle);

/*
 * @brief Close Wifi Interface
 *
 * @param handle Network handle
 */
error_type_t wifi_uap_intf_close(wifi_handle_t * handle);

/*
 * @brief Wifi Ioctls
 *
 * @param handle Network handle
 * @param flag Ioctl flag
 * @param option_ptr Pointer to optional data for ioctl
 * @param option_len Length of optional data for ioctl
 */
error_type_t wifi_uap_intf_ioctl(wifi_handle_t * handle, int flag, void *option_ptr, int option_len);


//// ToDo: These need to be better organized
//#include <trsocket.h>
void wifi_uap_intf_init(void);

//int wlan_link_uap_open(ttUserInterface interfaceHandle);
//int wlan_link_uap_close(ttUserInterface interfaceHandle);
//int wlan_link_uap_send(ttUserInterface interface_handle, char TM_FAR *data_ptr, int data_length, int flag);
//int wlan_link_uap_recv(ttUserInterface interface_handle, char** data_ptr, int *data_length, ttUserBufferPtr buf_handle_ptr
//#ifdef TM_DEV_RECV_OFFLOAD
//  , ttDevRecvOffloadPtr        devRecvOffloadPtr
//#endif /* TM_DEV_RECV_OFFLOAD */
//                       );
//int wlan_link_uap_ioctl(ttUserInterface interfaceHandle, int flag, void TM_FAR *optionPtr, int optionLen);
//int wlan_link_uap_get_phys_addr(void * handle,  char * physicalAddress);

#ifdef __cplusplus
}
#endif

#endif // WIFI_INTF_LINK_API_H
