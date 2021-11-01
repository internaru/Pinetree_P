/** @file wfd_main.c
 *  @brief This file contains core WFD state machine code.
 *           and functions for implementation.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "wps_util.h"
#include "wps_def.h"
#include "wps_wlan.h"
#include "wps_os.h"
#include "wlan_hostcmd.h"
#include "wlan_wfd.h"

#include "wps_msg.h"
#include "wireless_copy.h"
#include "memAPI.h"

#include "tx_api.h"
#include "logger.h"
#include "wlanInterface.h"
#include "wfd_api.h"
#include "common.h"

//extern int wps_registrar_start(PWPS_INFO pwps_info, WPS_DATA * wps_s);
void do_gettimeofday(struct timeval *time);

/********************************************************
        Local Variables
********************************************************/

/********************************************************
        Global Variables
********************************************************/
/** Global pwps information */
extern PWPS_INFO gpwps_info;
/** wps global */
extern WPS_DATA wps_global;

/** Interface switch required or not */
extern int switch_intf;

u8 wps_device_name[WFD_MAX_FIND_DEVICES][MAX_DEVICE_NAME_LEN + 1];
u16 device_name_len[WFD_MAX_FIND_DEVICES];

void wfd_reprint_find_table(void);
void wfd_start_peer_ageout_timer(WFD_DATA * pwfd_data);
void wfd_peer_ageout_time_handler(void *user_data);
int wfd_handle_persistent_group_invoked_event(PWPS_INFO pwps_info,
                                              WPS_DATA * wps_s,
                                              apeventbuf_wfd_generic *
                                              wfd_event);

static int wfd_update_find_table(apeventbuf_wfd_generic * wfd_event);
/********************************************************
        Local Functions
********************************************************/
/**
 *  @brief Prints a MAC address in colon separated form from raw data
 *
 *  @param raw      A pointer to the hex data buffer
 *  @return         N/A
 */
void
print_mac(u8 * raw)
{
    wps_printf(MSG_INFO, "%02x:%02x:%02x:%02x:%02x:%02x", (unsigned int) raw[0],
           (unsigned int) raw[1], (unsigned int) raw[2], (unsigned int) raw[3],
           (unsigned int) raw[4], (unsigned int) raw[5]);
    return;
}

/** 
 *  @brief  Get the GO config timeout for the peer GO.
 *  @param  wfd_event  New event data pointer
 *
 *  @return timeout in usecs. 
 */
static int
wfd_read_peer_config_timeout(apeventbuf_wfd_generic * wfd_event)
{
    tlvbuf_wfd_config_timeout *wfd_tlv;
    wfd_ie_header *wfd_wps_header;
    u16 wfd_wps_len = 0;
    u8 *ptr;
    u8 type = 0;
    u16 len = 0;
    s16 left_len = 0;

    ENTER();
    wfd_wps_header = (wfd_ie_header *) (wfd_event->entire_ie_list);
    wfd_wps_len = wlan_le16_to_cpu(wfd_event->event_length)
        - sizeof(apeventbuf_wfd_generic);

    if (wfd_wps_header->oui_type == WFD_OUI_TYPE) {
        ptr = wfd_wps_header->ie_list;
        left_len = wfd_wps_header->ie_length - sizeof(wfd_wps_header->oui)
            - sizeof(wfd_wps_header->oui_type);
        while (left_len > WFD_IE_HEADER_LEN) {
            type = *ptr;
            memcpy(&len, ptr + 1, sizeof(u16));
            len = wlan_le16_to_cpu(len);
            switch (type) {
            case TLV_TYPE_WFD_CONFIG_TIMEOUT:
                wfd_tlv = (tlvbuf_wfd_config_timeout *) ptr;
                /* Return value in 10s of msecs */
                return (wfd_tlv->group_config_timeout);
                break;
            default:
                break;
            }
            ptr += len + WFD_IE_HEADER_LEN;
            left_len -= len + WFD_IE_HEADER_LEN;
        }
    }
    LEAVE();
    return 0;
}

#if 1
/** 
 *  @brief  Clean up an existing entry from DB, if error found.
 *  @param  pwfd_data  Data pointer to clean.
 *  @param  index      Index into the DB table
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
static void
wfd_cleanup_peer_entry(WFD_DATA * pwfd_data, int index)
{
    ENTER();
    memset(pwfd_data->find_results[index].device_id, 0, ETH_ALEN);
    memset(pwfd_data->find_results[index].device_address, 0, ETH_ALEN);
    memset(pwfd_data->find_results[index].go_ssid, 0, MAX_SSID_LEN + 1);
    memset(pwfd_data->find_results[index].wps_device_name, 0, MAX_DEVICE_NAME_LEN + 1);
    pwfd_data->num_find_results--;
    LEAVE();
}

/** 
 *  @brief  Check if peer event has status attribute and print its value
 *  @param  req_resp Request or response flag
 *  @param  wfd_event  New event data pointer
 *
 *  @return  None.
 */
static void
wfd_check_go_neg_status(u8 req_resp, apeventbuf_wfd_generic * wfd_event)
{
    const u8 wifi_oui[3] = { 0x50, 0x6F, 0x9A };
    tlvbuf_wfd_status *wfd_tlv;
    wfd_ie_header *wfd_wps_header;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    u8 *ptr;
    s16 left_len = 0;
    u16 len = 0, wfd_wps_len = 0;

    ENTER();

    wfd_wps_header = (wfd_ie_header *) (wfd_event->entire_ie_list);
    wfd_wps_len = wlan_le16_to_cpu(wfd_event->event_length)
        - sizeof(apeventbuf_wfd_generic);

    while (wfd_wps_len >= sizeof(wfd_ie_header)) {

        /* 50:6F:9A:09 */
        if (!memcmp(wfd_wps_header->oui, wifi_oui, sizeof(wifi_oui)) &&
            (wfd_wps_header->oui_type == WFD_OUI_TYPE)) {

            ptr = wfd_wps_header->ie_list;
            left_len = wfd_wps_header->ie_length - sizeof(wfd_wps_header->oui)
                - sizeof(wfd_wps_header->oui_type);

            while (left_len > WFD_IE_HEADER_LEN) {

                if (*ptr == TLV_TYPE_WFD_STATUS) {
                    wfd_tlv = (tlvbuf_wfd_status *) ptr;

                    /* print status Code */
                    if (wfd_tlv->status_code)
                        wps_printf(MSG_INFO, "\nGo Negotiation %s result = %d.\n",
                               (req_resp) ? "response" : "request",
                               wfd_tlv->status_code);

                    /* reset dev_index selected here */
                    if (wfd_tlv->status_code)
                        pwfd_data->dev_index = -1;

                    LEAVE();
                    return;
                }

                memcpy(&len, ptr + 1, sizeof(u16));
                len = wlan_le16_to_cpu(len);

                ptr += len + WFD_IE_HEADER_LEN;
                left_len -= len + WFD_IE_HEADER_LEN;
            }
        }
        wfd_wps_len -= wfd_wps_header->ie_length + IE_HEADER_LEN;
        wfd_wps_header = (wfd_ie_header *) (((u8 *) wfd_wps_header) +
                                            wfd_wps_header->ie_length +
                                            IE_HEADER_LEN);
    }

    LEAVE();
    return;
}

/** 
 *  @brief  Check if peer event has intended address field.
 *          If yes, update the corresponding device ID.
 *  @param  wifi_direct_event  New event data pointer
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
//static 
int
wfd_check_peer_intended_addr(apeventbuf_wfd_generic * wifi_direct_event)
{
    const u8 wifi_oui[3] = { 0x00, 0x50, 0xF2 };
    tlvbuf_wfd_intended_addr *wifi_direct_tlv;
    wfd_ie_header *wfd_wps_header;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    u8 *ptr;
    s16 left_len = 0;
    u16 len = 0, wfd_wps_len = 0;

    ENTER();

    wfd_wps_header =
        (wfd_ie_header *) (wifi_direct_event->entire_ie_list);
    wfd_wps_len = wlan_le16_to_cpu(wifi_direct_event->event_length)
        - sizeof(apeventbuf_wfd_generic);

    while (wfd_wps_len >= sizeof(wfd_ie_header)) {

        /* 00:50:F2:09 */
        if (!memcmp(wfd_wps_header->oui, wifi_oui, sizeof(wifi_oui)) &&
            (wfd_wps_header->oui_type == WFD_OUI_TYPE)) {

            ptr = wfd_wps_header->ie_list;
            left_len = wfd_wps_header->ie_length - sizeof(wfd_wps_header->oui)
                - sizeof(wfd_wps_header->oui_type);

            while (left_len > WFD_IE_HEADER_LEN) {

                if (*ptr == TLV_TYPE_WFD_INTENDED_ADDRESS) {
                    wifi_direct_tlv = (tlvbuf_wfd_intended_addr *) ptr;

                    memcpy(pwfd_data->find_results[pwfd_data->dev_index].
                           device_id, wifi_direct_tlv->group_address, ETH_ALEN);

                    /* Address is found, Break out now */
                    wps_printf(DEBUG_WFD_DISCOVERY, "Intended address. " MACSTR,
                               MAC2STR(pwfd_data->
                                       find_results[pwfd_data->dev_index].
                                       device_id));
                    LEAVE();
                    return WPS_STATUS_SUCCESS;
                }

                memcpy(&len, ptr + 1, sizeof(u16));
                len = wlan_le16_to_cpu(len);

                ptr += len + WFD_IE_HEADER_LEN;
                left_len -= len + WFD_IE_HEADER_LEN;
            }
        }
        wfd_wps_len -= wfd_wps_header->ie_length + IE_HEADER_LEN;
        wfd_wps_header = (wfd_ie_header *) (((u8 *) wfd_wps_header) +
                                            wfd_wps_header->ie_length +
                                            IE_HEADER_LEN);
    }

    LEAVE();
    return WPS_STATUS_FAIL;
}

void
wfd_init_find_phase(void)
{
    wps_global.wfd_data.dev_index = -1;
    wps_global.wfd_data.num_find_results = 0;
    //wps_wlan_update_password_ie_config();
}


/**
 * @brief Get number of detected peers
 *
 * @param num_peers Pointer to number of peer detected
  *
 * @return OK or FAIL.
 */
error_type_t
wfd_num_of_peers(int * num_peers)
{
    *num_peers = wps_global.wfd_data.num_find_results;
    return OK;
}

/**
 * @brief Get peer by index
 *
 * @param index     Peer index
 * @param peer_info Pointer to peer information
 *
 * @return OK or FAIL.
 */
error_type_t
wfd_peer_device(int index, wfd_peer_info_t *peer_info)
{
    error_type_t retval = FAIL;
    if (index < wps_global.wfd_data.num_find_results) {
        peer_info->group_capability = (wps_global.wfd_data.find_results[index].group_capability & WFD_GROUP_OWNER_MASK);
        peer_info->device_id = wps_global.wfd_data.find_results[index].device_id;
        peer_info->device_addr = wps_global.wfd_data.find_results[index].device_address;
        peer_info->go_ssid = wps_global.wfd_data.find_results[index].go_ssid;
        peer_info->intended_addr = wps_global.wfd_data.find_results[index].intended_address;
        peer_info->wps_device_name = wps_global.wfd_data.find_results[index].wps_device_name;
        retval = OK;
    }
    return retval;
}


/** 
 *  @brief  Update the find table from FW events information. 
 *  @param  wfd_event  New event data pointer
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
static int
wfd_update_find_table(apeventbuf_wfd_generic * wfd_event)
{
    const u8 wifi_oui[3] = { 0x50, 0x6F, 0x9A };
    const u8 wps_oui[3] = { 0x00, 0x50, 0xF2 };
    int ret = WPS_STATUS_SUCCESS;
    int i, index = 0;
    int found = 0, first_WFD_WPS_IE = 0, updates = 0;
    tlvbuf_wfd_capability *wfd_tlv1 = NULL;
    tlvbuf_wfd_device_info *wfd_tlv2;
    tlvbuf_wfd_device_id *wfd_tlv3;
    tlvbuf_wfd_group_id *wfd_tlv4;
    tlvbuf_wfd_intended_addr *wfd_tlv5;
    tlvbuf_wfd_channel *wfd_tlv6;
    tlvbuf_wfd_channel_list *wfd_tlv7;
    wfd_ie_header *wfd_wps_header;
    IEEE_DsParamSet *ds_param = NULL;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    u8 *ptr, *array_ptr;
    u8 type = 0;
    s16 left_len = 0;
    u16 len = 0, wfd_wps_len = 0, wps_len = 0, wps_type = 0, len_wifidirect =
        0, event_type = 0;

    ENTER();

    event_type = wlan_le16_to_cpu(wfd_event->event_type);
    /* FW error ignored */
#if 0
    if (!memcmp(wfd_event->peer_mac_addr, "\x00\x00\x00\x00\x00\x00", ETH_ALEN)) {
#else
    char test_addr[ETH_ALEN] = {0x00,0x00,0x00,0x00,0x00,0x00};
    if (!memcmp(wfd_event->peer_mac_addr, test_addr, ETH_ALEN)) {
#endif
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    if (pwfd_data->num_find_results >= WFD_MAX_FIND_DEVICES) {
        wps_printf(MSG_ERROR, "Peer Device limit reached, dropping the information.\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    /* IF peer is Auto-GO, device_id (src from FW) = Intended Intf address */
    /* IF peer is Device , device_id (src from FW) = Device address */

    /* Look up the existing table and add if !found */
    for (i = 0; i < pwfd_data->num_find_results; i++) {
        if (!(memcmp(wfd_event->peer_mac_addr,
                     pwfd_data->find_results[i].device_id, ETH_ALEN))
            && pwfd_data->find_results[i].is_valid == 1) {
            found = 1;
            index = i;
            if (event_type == EV_TYPE_WFD_PEER_DETECTED) {
                pwfd_data->find_results[i].is_valid = 1;
                /** Update the current timestamp for the event */
				os_get_time(&pwfd_data->find_results[i].timestamp);
            }
            break;
        }
    }

    wfd_wps_header = (wfd_ie_header *) (wfd_event->entire_ie_list);
    wfd_wps_len = wlan_le16_to_cpu(wfd_event->event_length)
        - sizeof(apeventbuf_wfd_generic);

    while (wfd_wps_len >= sizeof(wfd_ie_header)) {

        if (wfd_wps_header->element_id == DS_PARAM_SET &&
            wfd_wps_header->ie_length == 1) {
            ds_param = (IEEE_DsParamSet *) wfd_wps_header;
            wfd_wps_header =
                (wfd_ie_header *) (((u8 *) wfd_wps_header) +
                                   sizeof(IEEE_DsParamSet));
            wfd_wps_len -= sizeof(IEEE_DsParamSet);
        }

        if (!memcmp(wfd_wps_header->oui, wifi_oui, sizeof(wifi_oui)) ||
            !(memcmp(wfd_wps_header->oui, wps_oui, sizeof(wps_oui)))
            ) {

            if (first_WFD_WPS_IE == 0 && !found) {
                /* copy for first P2P IEs */
                first_WFD_WPS_IE = 1;
                index = pwfd_data->num_find_results;
                device_name_len[index] = 0;

                pwfd_data->find_results[index].is_valid = 1;
                memcpy(pwfd_data->find_results[index].device_id,
                       wfd_event->peer_mac_addr, ETH_ALEN);
                pwfd_data->num_find_results++;
            }

            switch (wfd_wps_header->oui_type) {
            case WFD_OUI_TYPE:

                ptr = wfd_wps_header->ie_list;
                left_len =
                    wfd_wps_header->ie_length - sizeof(wfd_wps_header->oui)
                    - sizeof(wfd_wps_header->oui_type);

                while (left_len > WFD_IE_HEADER_LEN) {
                    type = *ptr;
                    memcpy(&len, ptr + 1, sizeof(u16));
                    len = wlan_le16_to_cpu(len);

                    /* Sanity check on data length */
                    if (left_len < WFD_IE_HEADER_LEN + len) {
                        wps_printf(MSG_WARNING,
                                   "Corrupt IE, type = 0x%x, len = %d, left = %d\n",
                                   type, len, left_len);
                        wfd_cleanup_peer_entry(pwfd_data, index);
                        return WPS_STATUS_FAIL;
                    }

                    switch (type) {
                    case TLV_TYPE_WFD_CAPABILITY:
                        wfd_tlv1 = (tlvbuf_wfd_capability *) ptr;
                        pwfd_data->find_results[index].group_capability =
                            wfd_tlv1->group_capability;
                        break;

                    case TLV_TYPE_WFD_DEVICE_ID:
                        wfd_tlv3 = (tlvbuf_wfd_device_id *) ptr;
                        memcpy(pwfd_data->find_results[index].device_address,
                               wfd_tlv3->dev_mac_address, ETH_ALEN);
                        break;

                    case TLV_TYPE_WFD_INTENDED_ADDRESS:
                        wfd_tlv5 = (tlvbuf_wfd_intended_addr *) ptr;
                        memcpy(pwfd_data->find_results[index].intended_address,
                               wfd_tlv5->group_address, ETH_ALEN);
                        break;

                    case TLV_TYPE_WFD_GROUP_ID:
                        wfd_tlv4 = (tlvbuf_wfd_group_id *) ptr;
                        if (found &&
                            !strlen((char *) pwfd_data->find_results[index].
                                    go_ssid))
                            updates = 1;
                        memcpy(pwfd_data->find_results[index].go_ssid,
                               wfd_tlv4->group_ssid, len -
                               (sizeof(tlvbuf_wfd_group_id) -
                                WFD_IE_HEADER_LEN));
                        break;

                    case TLV_TYPE_WFD_DEVICE_INFO:
                        wfd_tlv2 = (tlvbuf_wfd_device_info *) ptr;

                        memcpy(pwfd_data->find_results[index].device_address,
                               wfd_tlv2->dev_address, ETH_ALEN);
                        /* device name */
                        array_ptr = wfd_tlv2->device_name +
                            wfd_tlv2->secondary_dev_count * WPS_DEVICE_TYPE_LEN;

                        memcpy(&device_name_len[index],
                               (((u8 *) (&wfd_tlv2->device_name_len))
                                +
                                wfd_tlv2->secondary_dev_count *
                                WPS_DEVICE_TYPE_LEN), sizeof(u16));
                        device_name_len[index] = ntohs(device_name_len[index]);

                        memset(wps_device_name[index], 0, MAX_DEVICE_NAME_LEN);
                        if (device_name_len[index] <= MAX_DEVICE_NAME_LEN)
                            memcpy(wps_device_name[index], array_ptr,
                                   device_name_len[index]);
                        else {
                            wfd_cleanup_peer_entry(pwfd_data, index);
                            return WPS_STATUS_FAIL;
                        }
                        break;
                    case TLV_TYPE_WFD_OPCHANNEL:
                        wfd_tlv6 = (tlvbuf_wfd_channel *) ptr;
                        pwfd_data->find_results[index].op_channel =
                            wfd_tlv6->channel_number;
                        break;
                    case TLV_TYPE_WFD_CHANNEL:
                        wfd_tlv6 = (tlvbuf_wfd_channel *) ptr;
                        pwfd_data->find_results[index].listen_channel =
                            wfd_tlv6->channel_number;
                        break;
                    case TLV_TYPE_WFD_CHANNEL_LIST:
                        wfd_tlv7 = (tlvbuf_wfd_channel_list *) ptr;
                        chan_entry *temp_ptr;
                        temp_ptr = (chan_entry *) wfd_tlv7->wfd_chan_entry_list;
                        len_wifidirect = wlan_le16_to_cpu(wfd_tlv7->length) -
                            (sizeof(tlvbuf_wfd_channel_list) -
                             WFD_IE_HEADER_LEN);
                        /* Multiple channel entries */
                        if (len_wifidirect >
                            (sizeof(chan_entry) + temp_ptr->num_of_channels)) {
                            while (len_wifidirect) {
                                if ((int) (temp_ptr->regulatory_class) ==
                                    WIFI_REG_CLASS_81) {
                                    pwfd_data->find_results[index].
                                        peer_channel_list.num_of_chan =
                                        temp_ptr->num_of_channels;
                                    for (i = 0; i < temp_ptr->num_of_channels;
                                         i++) {
                                        pwfd_data->find_results[index].
                                            peer_channel_list.chan[i] =
                                            *(temp_ptr->chan_list + i);
                                    }
                                    break;
                                }
                                len_wifidirect -=
                                    sizeof(chan_entry) +
                                    temp_ptr->num_of_channels;
                                temp_ptr +=
                                    sizeof(chan_entry) +
                                    temp_ptr->num_of_channels;
                            }
                        } else {
                            /* Only one channel entry */
                            pwfd_data->find_results[index].peer_channel_list.
                                num_of_chan = temp_ptr->num_of_channels;
                            for (i = 0; i < temp_ptr->num_of_channels; i++) {
                                pwfd_data->find_results[index].
                                    peer_channel_list.chan[i] =
                                    *(temp_ptr->chan_list + i);
                            }
                        }
                        break;
                    default:
                        break;
                    }
                    ptr += len + WFD_IE_HEADER_LEN;
                    left_len -= len + WFD_IE_HEADER_LEN;
                }
                break;
            case WIFI_WPS_OUI_TYPE:
                /* 
                 * If device name is not found, check it in the WPS IEs
                 */
                if (device_name_len[index] == 0) {

                    if (found)
                        updates = 1;
                    ptr = wfd_wps_header->ie_list;
                    left_len =
                        wfd_wps_header->ie_length - sizeof(wfd_wps_header->oui)
                        - sizeof(wfd_wps_header->oui_type);

                    /* Look for WPS Device Name Len */
                    while (left_len > sizeof(tlvbuf_wps_ie)) {
                        memcpy(&wps_type, ptr, sizeof(u16));
                        wps_type = ntohs(wps_type);
                        memcpy(&wps_len, ptr + 2, sizeof(u16));
                        wps_len = ntohs(wps_len);

                        /* sanity check */
                        if ((left_len < sizeof(tlvbuf_wps_ie) + wps_len)
                            || (wps_len > MAX_DEVICE_NAME_LEN)) {
                            wps_printf(MSG_WARNING,
                                       "Corrupt WPS IE, type = 0x%x, len %d\n",
                                       wps_type, wps_len);
                            wfd_cleanup_peer_entry(pwfd_data, index);
                            return WPS_STATUS_FAIL;
                        }
                        if (wps_type == SC_Device_Name) {
                            tlvbuf_wps_ie *wps_tlv = (tlvbuf_wps_ie *) ptr;
                            array_ptr = wps_tlv->data;
                            device_name_len[index] = wps_len;
                            memset(wps_device_name[index], 0,
                                   MAX_DEVICE_NAME_LEN);
                            memcpy(wps_device_name[index], array_ptr, wps_len);
                            wps_printf(DEBUG_WFD_DISCOVERY,
                                       "Read Device name from WPS parameters.\n");
                        }
                        ptr += wps_len + sizeof(tlvbuf_wps_ie);
                        left_len -= wps_len + sizeof(tlvbuf_wps_ie);
                    }
                }
                break;
            }
        }
        wfd_wps_len -= wfd_wps_header->ie_length + IE_HEADER_LEN;
        wfd_wps_header = (wfd_ie_header *) (((u8 *) wfd_wps_header) +
                                            wfd_wps_header->ie_length +
                                            IE_HEADER_LEN);
    }

    if (ds_param) {
        if (wfd_tlv1 && wfd_tlv1->group_capability & WFD_GROUP_OWNER_MASK)
            pwfd_data->find_results[index].op_channel = ds_param->current_chan;
        else
            pwfd_data->find_results[index].listen_channel =
                ds_param->current_chan;
    }
    if (found && !updates) {
        LEAVE();
        return ret;
    }

    wfd_reprint_find_table();

    LEAVE();
    return ret;
}

/*set/get wifidirect provisioning protocol related parameters
 *
 *  Usage: "Usage : wfd_cfg_provisioning_params [<action> <configMethod>
 *                  <devPassword>]"
 *
 *  @param action         : ACTION_GET/ACTION_SET
 *  @param config_method  : WPS Config Method
 *  @param dev_password   : WPS Device Password ID
 *  @return         None
 */
void
wfd_cfg_cmd_provisioning_params(u16 action, u16 config_method, u16 dev_password)
{
    wfd_params_config *cmd_buf = NULL;
    wfd_provisioning_params *tlv = NULL;
    u8 *buffer = NULL;
    u16 cmd_len = 0;
    int ret = 0;

    cmd_len = sizeof(wfd_params_config) + sizeof(wfd_provisioning_params);

    buffer = (u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (!buffer) {
        wps_printf(MSG_ERROR, "ERR:Cannot allocate memory!\n");
        return;
    }

    memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    cmd_buf = (wfd_params_config *) buffer;
    tlv = (wfd_provisioning_params *) (buffer + sizeof(wfd_params_config));

    cmd_buf->cmd_head.cmd_code = HostCmd_CMD_WFD_PARAMS_CONFIG;
    cmd_buf->cmd_head.size = cmd_len - BUF_HEADER_SIZE;
    cmd_buf->cmd_head.seq_num = 0;
    cmd_buf->cmd_head.result = 0;

    tlv->tag = MRVL_WFD_PROVISIONING_PARAMS_TLV_ID;
    tlv->length = sizeof(wfd_provisioning_params) - TLV_HEADER_SIZE;
    if (action == ACTION_GET) {
        cmd_buf->action = ACTION_GET;
    } else {
        cmd_buf->action = wlan_cpu_to_le16(ACTION_SET);
        tlv->action = wlan_cpu_to_le16((u16) action);
        tlv->config_methods = wlan_cpu_to_le16((u16) config_method);
        tlv->dev_password = wlan_cpu_to_le16((u16) dev_password);
    }
    endian_convert_tlv_header_out(tlv);
    ret = wfd_ioctl((u8 *) buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);
    endian_convert_tlv_header_in(tlv);
    if (buffer)
        free(buffer);
    return;
}

/**
 *  @brief Set/get wifidirect WPS parametters
 *
 *  Usage: "Usage : wfd_cfg_wps_params [pin/pbc/none]"
 *
 *  @param action : Action GET/SET parameter
 *  @param wps_param : Set pin/pbc or none wps parameter based on user action
 *  @return         None
 */
void
wfd_cfg_cmd_wps_params(u16 action, char *wps_param)
{
    wfd_params_config *cmd_buf = NULL;
    wfd_wps_params *tlv = NULL;
    u8 *buffer = NULL;
    u16 cmd_len = 0;
    int ret = 0, param = 0;

    cmd_len = sizeof(wfd_params_config) + sizeof(wfd_wps_params);

    buffer = (u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (!buffer) {
        wps_printf(MSG_ERROR, "ERR:Cannot allocate memory!\n");
        return;
    }

    memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    cmd_buf = (wfd_params_config *) buffer;
    tlv = (wfd_wps_params *) (buffer + sizeof(wfd_params_config));

    cmd_buf->cmd_head.cmd_code = HostCmd_CMD_WFD_PARAMS_CONFIG;
    cmd_buf->cmd_head.size = cmd_len - BUF_HEADER_SIZE;
    cmd_buf->cmd_head.seq_num = 0;
    cmd_buf->cmd_head.result = 0;

    tlv->tag = MRVL_WFD_WPS_PARAMS_TLV_ID;
    tlv->length = sizeof(wfd_wps_params) - TLV_HEADER_SIZE;

    if (action == ACTION_GET) {
        cmd_buf->action = ACTION_GET;
    } else {
        if (strncmp(wps_param, "pin", 3) == 0)
            param = 1;
        else if (strncmp(wps_param, "pbc", 3) == 0)
            param = 2;
        else if (strncmp(wps_param, "none", 4)) {
            param = 0;
        }
        cmd_buf->action = wlan_cpu_to_le16(ACTION_SET);
        tlv->action = wlan_cpu_to_le16(param);
    }
    endian_convert_tlv_header_out(tlv);
    ret = wfd_ioctl((u8 *) buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);
    endian_convert_tlv_header_in(tlv);
    if (buffer)
        free(buffer);
    return;
}

/** 
 *  @brief  Check the find table for peer
 *  @param  peer Peer mac address
 *
 *  @return  index --success, -1 --fail
 */
static int
wfd_corelate_event_peer(u8 * peer)
{
    int i;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    ENTER();
    /* Look up the existing table for this peer */
    for (i = 0; i < pwfd_data->num_find_results; i++) {
        if (!(memcmp(peer, pwfd_data->find_results[i].device_id, ETH_ALEN))) {
            LEAVE();
            return i;
        }
    }
    LEAVE();
    return -1;
}

/*
 * @brief process Device selection input from user
 *
 * @param sel_index Index of device selected.
 * 
 * @return          OK or FAIL
 */
error_type_t
wfd_peer_device_selected(int sel_index)
{
    int ret = WPS_STATUS_SUCCESS;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    error_type_t retval = FAIL;

    ENTER();

    /* Service only if find is in progress */
    if (pwfd_data->dev_index != -1) {
        LEAVE();
        return ret;
    }

    if (sel_index < pwfd_data->num_find_results) {
        /** Cancel the peer ageout timer here */
        wps_cancel_timer(wfd_peer_ageout_time_handler, pwfd_data);
        wps_printf(MSG_INFO, "\n\tIndex Selected : %d\n", sel_index);
        pwfd_data->dev_index = sel_index;

        wps_printf(DEBUG_WFD_DISCOVERY, "Device %d] selected for connection.\n",
                   pwfd_data->dev_index);

        // Stop find phase ???
        wfd_wlan_update_mode(WFD_MODE_STOP_FIND_PHASE);

        /* Update beacon IE, probe response IE for this device password */
        wps_wlan_update_password_ie_config();

        if (!(pwfd_data->find_results[pwfd_data->dev_index].group_capability &
              WFD_GROUP_OWNER_MASK)) {
            /* start FIND phase */
            wps_printf(DEBUG_WFD_DISCOVERY, "Starting find phase now.\n");

            ret =
                wfdcmd_action_frame(pwfd_data->dev_index,
                                    WFD_FIND_ACTION_SUB_TYPE);

        } else {
            /* start provision discovery */
            wps_printf(DEBUG_WFD_DISCOVERY, "Sending provision discovery.\n");

            ret =
                wfdcmd_action_frame(pwfd_data->dev_index,
                                    WFD_PROVISION_DISCOVERY_REQ_SUB_TYPE);

            if (ret == WPS_STATUS_SUCCESS) {
                retval = OK;
            }
        }
    }
    LEAVE();
    return retval;
}


/*
 * @brief process WFD generic event data
 *
 * @param buffer   Pointer to received buffer
 * @param size     Length of the received event data
 * @return         N/A
 */
void
wfd_process_generic_event(u8 * buffer, u16 size)
{
    int ret = WPS_STATUS_SUCCESS;
    //int sel_index = 0;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    apeventbuf_wfd_generic *wfd_event;
    u16 event_type = 0;
    u16 event_sub_type = 0;
    u8 device_role = 0;
    //bss_config_t bss_config;
    wfd_event = (apeventbuf_wfd_generic *) (buffer);
    event_type = wlan_le16_to_cpu(wfd_event->event_type);
    //WPS_DATA *wps_s = (WPS_DATA *) & wps_global;

    ENTER();

    memcpy(&event_sub_type, (u8 *) & (wfd_event->event_sub_type), 2);
    event_sub_type = wlan_le16_to_cpu(event_sub_type);
    device_role = event_sub_type & DEVICE_ROLE_MASK;

    switch (event_type) {

    case EV_TYPE_WFD_PEER_DETECTED:
        /* Service event only if find is in progress */
        if (pwfd_data->dev_index != -1) {
            break;
        }

        /* Update find table information for this event */
        wfd_update_find_table(wfd_event);
        break;

    case EV_TYPE_WFD_NEGOTIATION_REQUEST:
    case EV_TYPE_WFD_NEGOTIATION_RESPONSE:
        if (device_role == EV_SUBTYPE_WFD_GO_NEG_FAILED) {
            /* Go back to discovery */
            pwfd_data->dev_index = -1;
            break;
        }

        wfd_check_go_neg_status(event_type, wfd_event);

        /* 
         * Update find table information for this event, if
         * device was in silent mode during find phase 
         */
        wfd_update_find_table(wfd_event);

        /* Find device index correspoding to this peer */
        if (pwfd_data->dev_index == -1) {
            pwfd_data->dev_index = wfd_corelate_event_peer
                (wfd_event->peer_mac_addr);
        }
        /* 
         * The intended interface address is also updated while updating
         * find phase table, copy it to registrar, enrollee mac address.
         * Use it based on the GO negotiation result event.
         */
        memcpy(gpwps_info->registrar.mac_address,
               pwfd_data->find_results[pwfd_data->dev_index].intended_address,
               ETH_ALEN);
        memcpy(gpwps_info->enrollee.mac_address,
               pwfd_data->find_results[pwfd_data->dev_index].intended_address,
               ETH_ALEN);
        wps_printf(DEBUG_WFD_DISCOVERY, "Peer Enrollee/Registrar: " MACSTR,
                   MAC2STR(gpwps_info->registrar.mac_address));
        break;

    case EV_TYPE_WFD_NEGOTIATION_RESULT:

        /* 
         * Update find table information for this event, if
         * device was in silent mode during find phase
         */
        wfd_update_find_table(wfd_event);

        if (device_role == EV_SUBTYPE_WFD_GO_ROLE) {

            /* WFD Registrar */
            wps_printf(MSG_INFO, "Wfd : Group Owner Role\n");
            wps_printf(DEBUG_WFD_DISCOVERY,
                       "Event SubType = Group Owner Role\n");
#if 0
            gpwps_info->discovery_role = WPS_REGISTRAR;

            if ((gpwps_info->registrar.device_password_id ==
                 DEVICE_PASSWORD_ID_PIN)
                && (gpwps_info->pin_generator == WFD_AUTO_DISPLAY_PIN)) {

                gpwps_info->enrollee.updated_device_password_id =
                    DEVICE_PASSWORD_REG_SPECIFIED;
                gpwps_info->registrar.updated_device_password_id =
                    DEVICE_PASSWORD_REG_SPECIFIED;
            }
            /* TODO */
            tx_thread_sleep(1);
#if 0
            /* 
             * Update Group formation bit when starting as registrar
             */
            if (wps_wlan_update_group_formation_config(WPS_SET) !=
                WPS_STATUS_SUCCESS) {
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "Failed to update Group formation bit.\n");
                wps_printf(MSG_INFO, "Failed to update Group formation bit.\n");
                break;
            }
            wps_printf(DEBUG_WFD_DISCOVERY, "Updated Group formation bit.\n");
            wps_printf(MSG_INFO, "Updated Group formation bit.\n");
#endif
            /* Find device index correspoding to this peer */
            if (pwfd_data->dev_index == -1) {
                pwfd_data->dev_index = wfd_corelate_event_peer
                    (wfd_event->peer_mac_addr);
            }
            if (!memcmp
                (pwfd_data->find_results[pwfd_data->dev_index].intended_address,
                 "\x00\x00\x00\x00\x00\x00", ETH_ALEN)) {
                memcpy(gpwps_info->enrollee.mac_address,
                       wfd_event->peer_mac_addr, ETH_ALEN);
            }

#if 0
            /* 
             * Read AP configuration now and switch to AP interface
             */
            if (switch_intf) {
                wps_intf_deinit(&wps_global);
                strncpy(wps_global.ifname, "uap0", IFNAMSIZ + 1);
                wps_loop_init(&wps_global);
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "Changed interface to uap now.\n");
            } else {
                if (wps_wlan_set_bss_type(BSS_TYPE_UAP) != WPS_STATUS_SUCCESS) {
                    wps_printf(DEBUG_WFD_DISCOVERY,
                               "Failed to set uAP role.\n");
                    break;
                }
                /* Update BSS type */
                wps_s->bss_type = BSS_TYPE_UAP;
            }
#else
            if (wlan_set_bss_type(WIFI_IFACE_NAME_WFD, BSS_TYPE_UAP) != WPS_STATUS_SUCCESS) {
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "Failed to set uAP role.\n");
                wps_printf(MSG_INFO, "Failed to set uAP role.\n");
                break;
            }
#endif

            /* Set intended address before starting GO */
            if (memcmp
                (gpwps_info->wfd_intended_addr, "\x00\x00\x00\x00\x00\x00",
                 ETH_ALEN)) {
                if (wlan_set_intended_mac_addr(gpwps_info->wfd_intended_addr)) {
                    wps_printf(DEBUG_WFD_DISCOVERY,
                               "Failed to set mac address.\n");
                    break;
                }
                memcpy(gpwps_info->registrar.mac_address,
                       gpwps_info->wfd_intended_addr, ETH_ALEN);
            }

            /* 
             * Send command to AP for starting in GO mode, [mode 2]
             */
            if (wfd_wlan_update_mode(WFD_MODE_START_GROUP_OWNER) !=
                WPS_STATUS_SUCCESS) {
                wps_printf(DEBUG_WFD_DISCOVERY, "Failed to start GO.\n");
                wps_printf(MSG_INFO, "Failed to start GO.\n");
                break;
            }

            wps_printf(DEBUG_WFD_DISCOVERY, "Issued Start GO mode command.\n");
            wps_printf(MSG_INFO, "Issued Start GO mode command.\n");
            if (!wps_global.current_ssid.ssid_len) {
                if (apcmd_get_bss_config(&bss_config, gpwps_info) != WPS_STATUS_SUCCESS) {
                    wps_printf(DEBUG_WFD_DISCOVERY,
                               "Failed to get BSS Config.\n");
                    break;
                }

                load_cred_info(wps_s, gpwps_info, &bss_config);

                /* 
                 * Start BSS now.
                 */
                if (apcmd_start_bss(&wps_global, gpwps_info) == WPS_STATUS_FAIL) {
                    wps_printf(DEBUG_WFD_DISCOVERY, "Failed to start GO AP.\n");
                    wps_printf(MSG_INFO, "Failed to start GO AP.\n");
                    break;
                }
            }

            wps_hexdump(DEBUG_WFD_DISCOVERY, "bss_config.psk", bss_config.psk,
                        32);
            /* Checking PSK validity by Comparing first 4bytes of PSK to 0 */
            if (1) {
                /* PSK = ssid * passphrase using SHA1 */
                wlan_wfd_generate_psk(gpwps_info);
            } else {
                /* Get PSK from firmware */
                wlan_wfd_get_psk(gpwps_info, bss_config.psk);
            }

            //ret = wps_registrar_start(gpwps_info, &wps_global);
            wirelessStartWFDWPSRegistrar(NULL, false);
#else
            extern void wlan_wfd_start_group_owner(void);
            wlan_wfd_start_group_owner();
#endif
        }

        else if (device_role == EV_SUBTYPE_WFD_CLIENT_ROLE) {

            /* WFD Enrollee */
            wps_printf(MSG_INFO, "Wfd : Device Role\n");
            wps_printf(DEBUG_WFD_DISCOVERY, "Event SubType = Client Role\n");
#if 0
            gpwps_info->discovery_role = WPS_ENROLLEE;

            if ((gpwps_info->enrollee.device_password_id ==
                 DEVICE_PASSWORD_ID_PIN)
                && (gpwps_info->pin_generator == WFD_AUTO_ENTER_PIN)) {
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "Changing Device Password ID to REG_SPEC\n");
                gpwps_info->enrollee.updated_device_password_id =
                    DEVICE_PASSWORD_REG_SPECIFIED;
                gpwps_info->registrar.updated_device_password_id =
                    DEVICE_PASSWORD_REG_SPECIFIED;

            }
            /* Changed from 5 to 2 during WFD certification */
            tx_thread_sleep(5);

            /* Find device index correspoding to this peer */
            if (pwfd_data->dev_index == -1) {
                pwfd_data->dev_index = wfd_corelate_event_peer
                    (wfd_event->peer_mac_addr);
            }
            /* 
             * Check for Intended Interface address
             * and update the corresponding device Id.
             */
            if (memcmp
                (gpwps_info->wfd_intended_addr, "\x00\x00\x00\x00\x00\x00",
                 ETH_ALEN)) {
                if (wlan_set_intended_mac_addr(gpwps_info->wfd_intended_addr)) {
                    wps_printf(DEBUG_WFD_DISCOVERY,
                               "Failed to set mac address.\n");
                }
                memcpy(gpwps_info->enrollee.mac_address,
                       gpwps_info->wfd_intended_addr, ETH_ALEN);
            }

            /* Check peer intended address before connection */
            if (memcmp
                (pwfd_data->find_results[pwfd_data->dev_index].intended_address,
                 "\x00\x00\x00\x00\x00\x00", ETH_ALEN) == 0) {
                memcpy(pwfd_data->find_results[pwfd_data->dev_index].
                       intended_address,
                       pwfd_data->find_results[pwfd_data->dev_index].device_id,
                       ETH_ALEN);
            }

            /* 
             * Send command to Station for starting as Device, [mode 3]
             */
            if (wfd_wlan_update_mode(WFD_MODE_START_DEVICE) !=
                WPS_STATUS_SUCCESS) {
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "Failed to start device mode.\n");
                break;
            }

            /* sleep as per peer's GO config timeout */
            tx_thread_sleep(wfd_read_peer_config_timeout(wfd_event));

            wps_printf(DEBUG_WFD_DISCOVERY, "Starting enrollee now.\n");

            //ret = wps_enrollee_start(gpwps_info, &wps_global, NULL);
            wirelessStartWFDWPSEnrollee(NULL, NULL, false);
#else
            /* Find device index correspoding to this peer */
            if (pwfd_data->dev_index == -1) {
                pwfd_data->dev_index = wfd_corelate_event_peer
                    (wfd_event->peer_mac_addr);
            }

            extern void wlan_wfd_start_client(int peer_config_timeout);
            wlan_wfd_start_client(wfd_read_peer_config_timeout(wfd_event));
#endif
        } else if (device_role == EV_SUBTYPE_WFD_GO_NEG_FAILED) {
            wps_printf(MSG_ERROR, "WFD GO Negotiation Failed!\n");
            ret = WPS_STATUS_FAIL;
            /** Restart peer ageout timer */
            wfd_start_peer_ageout_timer(pwfd_data);
            /* Go back to discovery */
            pwfd_data->dev_index = -1;
        }

        if (pwfd_data->dev_index != -1 && ret != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_ERROR, "WFD Registrar, Enrollee failure...\n");

            /* Shutdown the main processing loop */
            wps_main_loop_shutdown();
        }
        break;

    case EV_TYPE_WFD_PROVISION_DISCOVERY_RESP:

        if (pwfd_data->dev_index != -1) {
            break;
        }
        /* 
         * Check if provision discovery response is successful and 
         * has right config methods.
         */
        // ret = ;
        if (ret == WPS_STATUS_SUCCESS) {
#if 0
            sel_index = pwfd_data->dev_index;
            /* 
             * Start an enrollee, as peer has GO capability and
             * Provision discovery response is +ve
             */
            gpwps_info->discovery_role = WPS_ENROLLEE;

            /* sleep as per peer's GO config timeout, not needed. */
            wps_printf(DEBUG_WFD_DISCOVERY, "Starting enrollee now.\n");
            /* Intended address will be used for connection */
            memcpy(pwfd_data->find_results[sel_index].intended_address,
                   pwfd_data->find_results[sel_index].device_id, ETH_ALEN);
            memcpy(gpwps_info->registrar.mac_address,
                   pwfd_data->find_results[sel_index].device_id, ETH_ALEN);

            /* 
             * Send command to FW to stop Find phase, [mode 5]; else bg_scan
             * causes issues in 802_11_ret_scan
             */
            if (wfd_wlan_update_mode(WFD_MODE_STOP_FIND_PHASE) !=
                WPS_STATUS_SUCCESS) {
                wps_printf(DEBUG_WFD_DISCOVERY, "Failed to stop find phase.\n");
                return;
            }
            tx_thread_sleep(25);

            /* 
             * Check for Intended Interface address
             * and update the corresponding device Id.
             */
            if (memcmp
                (gpwps_info->wfd_intended_addr, "\x00\x00\x00\x00\x00\x00",
                 ETH_ALEN)) {
                if (wlan_set_intended_mac_addr(gpwps_info->wfd_intended_addr)) {
                    wps_printf(DEBUG_WFD_DISCOVERY,
                               "Failed to set mac address.\n");
                }
                memcpy(gpwps_info->enrollee.mac_address,
                       gpwps_info->wfd_intended_addr, ETH_ALEN);
            }

            /* 
             * Send command to Station for starting as Device, [mode 3]
             */
            if (wfd_wlan_update_mode(WFD_MODE_START_DEVICE) !=
                WPS_STATUS_SUCCESS) {
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "Failed to start device mode.\n");
                return;
            }
            if ((gpwps_info->enrollee.device_password_id ==
                 DEVICE_PASSWORD_ID_PIN)
                && (gpwps_info->pin_generator == WFD_AUTO_ENTER_PIN)) {
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "Changing Device Password ID to REG_SPECIFIED\n");
                gpwps_info->enrollee.updated_device_password_id =
                    DEVICE_PASSWORD_REG_SPECIFIED;
                gpwps_info->registrar.updated_device_password_id =
                    DEVICE_PASSWORD_REG_SPECIFIED;

            }

            ret = wps_enrollee_start(gpwps_info, &wps_global);
#else
            extern void wlan_wfd_start_client(int peer_config_timeout);
            wlan_wfd_start_client(0);
#endif			
        }
        break;

    case EV_TYPE_WFD_INVITATION_REQ:
    case EV_TYPE_WFD_INVITATION_RESP:
        {
            ret = wfd_handle_persistent_group_invoked_event(gpwps_info,
                                                            &wps_global,
                                                            wfd_event);

            if (ret == WPS_STATUS_SUCCESS) {
                /* We are done now, exit gracefully */
                /* Unregister user abort handler */
                wps_unregister_rdsock_handler(STDIN_FILENO);

                /* Shutdown the main processing loop */
                wps_main_loop_shutdown();
            }
            break;
        }

    default:
        break;
    }
    LEAVE();
}
#endif

/*
 *  @brief Process WFD Persistent group invoked event
 *  @param pwps_info A pointer to the PWPS_INFO structure
 *  @param wps_s     A pointer to the WPS_DATA structure
 *  @param wfd_event Event data pointer
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfd_handle_persistent_group_invoked_event(PWPS_INFO pwps_info,
                                          WPS_DATA * wps_s,
                                          apeventbuf_wfd_generic * wfd_event)
{
#ifdef WIFI_DIRECT_SUPPORT
    u8 index, dev_role, process_state;
    int ret = WPS_STATUS_SUCCESS;
    wfd_persistent_record prec;
    //s8 psk_hexstr[64];
    u16 event_sub_type = 0;

    ENTER();

    memcpy(&event_sub_type, (u8 *) & (wfd_event->event_sub_type), 2);
    event_sub_type = wlan_le16_to_cpu(event_sub_type);

    dev_role = event_sub_type & DEVICE_ROLE_MASK;

    process_state = (event_sub_type & PKT_PROCESS_STATE_MASK) >> 2;

    index = wlan_le32_to_cpu(*(u32 *) wfd_event->entire_ie_list);

    wps_printf(DEBUG_WFD_DISCOVERY,
               "EV: PERSISTENT_GROUP_INVOKED dev_role: %d, state: %d, index: %d\n",
               dev_role, process_state, index);

    if (process_state != EV_SUBTYPE_WFD_REQRECV_PROCESSING_SUCCESS) {
        /* Ignore other states now */
        ret = WPS_STATUS_FAIL;
        goto done;
    }

    /* 1st read the persistent ssid/psk from the FW */
    memset(&prec, 0, sizeof(wfd_persistent_record));
    prec.index = index;

    if ((ret = wfd_cfg_cmd_persistent_group_record(ACTION_GET, &prec)) !=
        WPS_STATUS_SUCCESS) {
        wps_printf(MSG_ERROR,
                   "EV: PERSISTENT_GROUP_INVOKED read group rec failed\n");
        goto done;
    }

#if 0
    /* Convert PSK in bin form to hex str */
    bin2hexstr(prec.psk, psk_hexstr, prec.psk_len);

    if (prec.psk_len != 64) {
        wps_create_supp_file((u8 *) "wfd.conf", prec.ssid, prec.ssid_len,
                             (u8 *) psk_hexstr, prec.psk_len);
    } else {
        wps_create_supp_file((u8 *) "wfd.conf", prec.ssid, prec.ssid_len,
                             (u8 *) psk_hexstr, 64);
    }
#else
    // store in NVRAM
#endif

    wps_printf(DEBUG_WFD_DISCOVERY, "got per rec: dev_role: %d\n",
               prec.dev_role);

    if (prec.dev_role == EV_SUBTYPE_WFD_CLIENT_ROLE) {
        gpwps_info->discovery_role = WPS_ENROLLEE;
        /* Send command to Station for starting as Device, [mode 3] */
        if ((ret = wfd_wlan_update_mode(WFD_MODE_START_DEVICE)) !=
            WPS_STATUS_SUCCESS) {
            wps_printf(MSG_ERROR, "Failed to start device mode.\n");
            goto done;
        }
    } else if (prec.dev_role == EV_SUBTYPE_WFD_GO_ROLE) {
        gpwps_info->discovery_role = WPS_REGISTRAR;
        wps_printf(DEBUG_WLAN, "Set BSS Type to UAP: %d\n", prec.dev_role);
        //if (wps_wlan_set_bss_type(BSS_TYPE_UAP) != WPS_STATUS_SUCCESS) {
        if (wlan_set_bss_type(WIFI_IFACE_NAME_WFD, BSS_TYPE_UAP) != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_ERROR, "Failed to set uAP role.\n");
            goto done;
        }

        /* Update BSS type */
        wps_s->bss_type = BSS_TYPE_UAP;

        wps_printf(DEBUG_WLAN, "Set WFD Mode to GO\n");
        /* Send command to AP for starting in GO mode, [mode 2] */
        if ((ret = wfd_wlan_update_mode(WFD_MODE_START_GROUP_OWNER)) !=
            WPS_STATUS_SUCCESS) {
            wps_printf(MSG_ERROR, "Failed to start GO.\n");
            goto done;
        }

        wps_printf(DEBUG_WLAN, "Starting BSS!\n");
        tx_thread_sleep(3);
        /* Start BSS now. */
        if ((ret = apcmd_start_bss(wps_s)) == WPS_STATUS_FAIL) {
            wps_printf(MSG_ERROR, "Failed to start GO AP.\n");
            goto done;
        }
    }

  done:
    LEAVE();
    return ret;
#else
    return WPS_STATUS_SUCCESS;
#endif
}

#if 0
/*
 *  @brief       Create Supplicant Style config file 
 *  @file_name   Name of config file to create
 *  @ssid        Pointer to SSID string
 *  @ssid_len    Length of SSID string
 *  @psk         Pointer to PSK string
 *  @psk_len     Length of PSK string
 *  @return      WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
//XXX: Enhance this function, and use for all wpa_supp config file creation
int
wps_create_supp_file(u8 * file_name, u8 * ssid, u32 ssid_len,
                     u8 * psk, u32 psk_len)
{
    char quotation_mark = '\"';
    int i;
    FILE *stream = NULL;

    if ((stream = fopen((char *) file_name, "w+")) == NULL) {
        wps_printf(MSG_ERROR, "Error writing configuration file.!!!\n");
        return WPS_STATUS_FAIL;
    }
    fprintf(stream, "%s%s\n", "ctrl_interface=", WPAS_CTRL_PATH);
    fprintf(stream, "%s\n", "ap_scan=2");
    fprintf(stream, "%s\n", "network={");

    /* SSID */
    fprintf(stream, "%s%c", "ssid=", quotation_mark);
    for (i = 0; i < ssid_len; i++)
        fprintf(stream, "%c", ssid[i]);
    fprintf(stream, "%c\n", quotation_mark);

    /* proto */
    fprintf(stream, "proto=RSN\n");
    /* key_mgmt */
    fprintf(stream, "key_mgmt=WPA-PSK\n");
    /* pairwise */
    fprintf(stream, "pairwise=CCMP\n");

    /* PSK */
    fprintf(stream, "%s", "psk=");
    if (psk_len == 64) {
        for (i = 0; i < psk_len; i++)
            fprintf(stream, "%c", (char) psk[i]);
        fprintf(stream, "\n");
    } else {
        fprintf(stream, "%c", quotation_mark);
        for (i = 0; i < psk_len; i++)
            fprintf(stream, "%c", (char) psk[i]);
        fprintf(stream, "%c\n", quotation_mark);
    }

    fprintf(stream, "}\n");

    fclose(stream);
    return WPS_STATUS_SUCCESS;
}
#endif

/*
 *  @brief       Read/Write WFD Persistent group database in FW
 *  @action      ACTION_GET or ACTION_SET   
 *  @param prec  Pointer to wfd_persistent_record structure
 *  @return      WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfd_cfg_cmd_persistent_group_record(int action, wfd_persistent_record * prec)
{
    wfd_params_config *cmd_buf = NULL;
    tlvbuf_wfd_persistent_group *tlv = NULL;
    u8 index = 0, var_len = 0;
    u8 *buffer = NULL;
    u16 cmd_len = 0, buf_len = 0;
    int pi;
    ENTER();

    /* error checks */
    if (prec->index >= WFD_PERSISTENT_GROUP_MAX) {
        wps_printf(MSG_ERROR, "ERR:wrong index. Value values are [0-3]\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    cmd_len = sizeof(wfd_params_config);
    buffer = (u8 *) malloc(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (!buffer) {
        wps_printf(MSG_ERROR, "ERR:Cannot allocate memory!\n");
        LEAVE();
        return WPS_STATUS_FAIL;
    }

    memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    cmd_buf = (wfd_params_config *) buffer;
    cmd_buf->cmd_head.cmd_code = HostCmd_CMD_WFD_PARAMS_CONFIG;
    cmd_buf->cmd_head.seq_num = 0;
    cmd_buf->cmd_head.result = 0;
    tlv = (tlvbuf_wfd_persistent_group *) (buffer + sizeof(wfd_params_config));

    tlv->tag = MRVL_WFD_PERSISTENT_GROUP_TLV_ID;

    if (action == ACTION_GET) {
        cmd_buf->action = ACTION_GET;
        tlv->rec_index = prec->index;
        tlv->length = sizeof(prec->index);
        cmd_len += TLV_HEADER_SIZE + tlv->length;
        cmd_buf->cmd_head.size = cmd_len - BUF_HEADER_SIZE;
        buf_len = MRVDRV_SIZE_OF_CMD_BUFFER;
        goto send_cmd;
    }

    cmd_buf->action = wlan_cpu_to_le16(ACTION_SET);
    tlv->rec_index = prec->index;
    tlv->dev_role = prec->dev_role;
    memcpy(tlv->group_bssid, prec->bssid, ETH_ALEN);
    memcpy(tlv->dev_mac_address, prec->groupdevid, ETH_ALEN);

    /* ssid */
    tlv->group_ssid_len = prec->ssid_len;
    var_len += tlv->group_ssid_len;
    memcpy(tlv->group_ssid, prec->ssid, tlv->group_ssid_len);

    /* adjust pointer from here */
    /* psk */
    *(&tlv->psk_len + var_len) = prec->psk_len;
    memcpy(tlv->psk + var_len, prec->psk, prec->psk_len);
    var_len += prec->psk_len;

    tlv->length = sizeof(tlvbuf_wfd_persistent_group) -
        TLV_HEADER_SIZE + var_len;

    *(&tlv->num_peers + var_len) = prec->num_peers;

    for (pi = 0; pi < prec->num_peers; pi++) {
        memcpy(tlv->peer_mac_addrs[pi] + var_len, prec->peers[pi], ETH_ALEN);
    }

    tlv->length += prec->num_peers * ETH_ALEN;

    cmd_len += TLV_HEADER_SIZE + tlv->length;
    buf_len = cmd_len;
    cmd_buf->cmd_head.size = cmd_len - BUF_HEADER_SIZE;

  send_cmd:
    endian_convert_tlv_header_out(tlv);

    /* Send collective command */
    wfd_ioctl((u8 *) buffer, &cmd_len, buf_len);

    endian_convert_tlv_header_in(tlv);

    if (action == ACTION_GET) {
        if (tlv->length > (sizeof(tlvbuf_wfd_persistent_group) - sizeof(u8)))
            wps_printf(MSG_INFO, "Persistent group information =>\n");
        else
            wps_printf(DEBUG_WFD_DISCOVERY,
                       "Persistent group information is empty.\n");
        buf_len = tlv->length;
        while (buf_len > (sizeof(tlvbuf_wfd_persistent_group) - sizeof(u8))) {
            wps_printf(MSG_INFO, "\n\t Index = [%d]\n", tlv->rec_index);
            prec->index = tlv->rec_index;
            wps_printf(MSG_INFO, "\n\t Dev_Role = [%d]\n", tlv->dev_role);
            wps_printf(MSG_INFO, "\t Role  = %s\n",
                   (tlv->dev_role ==
                    EV_SUBTYPE_WFD_GO_ROLE) ? "Group owner" : "Client");
            prec->dev_role = tlv->dev_role;
            wps_printf(MSG_INFO, "\t GroupBssId - ");
            print_mac(tlv->group_bssid);
            memcpy(prec->bssid, tlv->group_bssid, ETH_ALEN);
            wps_printf(MSG_INFO, "\n\t DeviceId - ");
            print_mac(tlv->dev_mac_address);
            memcpy(prec->groupdevid, tlv->dev_mac_address, ETH_ALEN);
            wps_printf(MSG_INFO, "\n\t SSID = ");
            for (index = 0; index < tlv->group_ssid_len; index++)
                wps_printf(MSG_INFO, "%c", tlv->group_ssid[index]);

            prec->ssid_len = tlv->group_ssid_len;
            memcpy(prec->ssid, tlv->group_ssid, tlv->group_ssid_len);

            var_len = tlv->group_ssid_len;
            wps_printf(MSG_INFO, "\n\t PSK = ");
            for (index = 0; index < *(&tlv->psk_len + var_len); index++) {
                if (index == 16)
                    wps_printf(MSG_INFO, "\n\t       ");
                wps_printf(MSG_INFO, "%02x ", *(&tlv->psk[index] + var_len));
            }

            prec->psk_len = *(&tlv->psk_len + var_len);
            memcpy(prec->psk, tlv->psk + var_len, prec->psk_len);

            var_len += *(&tlv->psk_len + var_len);
            if (tlv->dev_role == EV_SUBTYPE_WFD_GO_ROLE) {
                prec->num_peers = *(&tlv->num_peers + var_len);
                for (pi = 0; pi < prec->num_peers; pi++) {
                    memcpy(prec->peers[pi],
                           tlv->peer_mac_addrs[pi] + var_len, ETH_ALEN);
                    wps_printf(MSG_INFO, "\n\t Peer Mac address(%d) = ", pi);
                    print_mac(prec->peers[pi]);
                }
            }
            if (action == ACTION_GET)
                wps_printf(MSG_INFO, "\n\t -----------------------------------------");
            if (tlv->dev_role == EV_SUBTYPE_WFD_GO_ROLE) {
                buf_len -= sizeof(tlvbuf_wfd_persistent_group) -
                    TLV_HEADER_SIZE + var_len;
                tlv = (tlvbuf_wfd_persistent_group *) (((u8 *) (tlv)) +
                                                       (sizeof
                                                        (tlvbuf_wfd_persistent_group)
                                                        - TLV_HEADER_SIZE +
                                                        var_len));
            } else {
                /* num_peer field willnt be present */
                buf_len -= sizeof(tlvbuf_wfd_persistent_group) -
                    TLV_HEADER_SIZE - sizeof(u8) + var_len;
                tlv = (tlvbuf_wfd_persistent_group *) (((u8 *) (tlv)) +
                                                       (sizeof
                                                        (tlvbuf_wfd_persistent_group)
                                                        - TLV_HEADER_SIZE -
                                                        sizeof(u8) + var_len));
            }
        }
        wps_printf(MSG_INFO, "\n");
    } else {
        wps_printf(DEBUG_WFD_DISCOVERY,
                   "Setting persistent group information successful!\n");
    }

    if (buffer)
        free(buffer);

    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/*
 *  @brief Return index of persistent group record matching the given SSID
 *  @param ssid_length Length of ssid to lookup
 *  @param prec        ssid to lookup
 *  @param pr_index    Index of matching record
 *  @return      WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfd_get_persistent_rec_ind_by_ssid(int ssid_length, u8 * ssid, s8 * pr_index)
{
    int index, ret = WPS_STATUS_SUCCESS;
    wfd_persistent_record prec;

    ENTER();

    for (index = 0; index < WFD_PERSISTENT_GROUP_MAX; index++) {
        memset(&prec, 0, sizeof(wfd_persistent_record));

        prec.index = index;
        if ((ret =
             wfd_cfg_cmd_persistent_group_record(ACTION_GET,
                                                 &prec)) !=
            WPS_STATUS_SUCCESS) {
            goto done;
        }

        if ((prec.ssid_len == ssid_length) &&
            !memcmp(prec.ssid, ssid, ssid_length)) {
            /* correct record found */
            *pr_index = index;
            goto done;
        }
    }

    /* record not found */
    ret = WPS_STATUS_FAIL;

  done:
    LEAVE();
    return ret;
}

/*
 *  @brief Return index of a free persistent group record
 *  @param pr_index    Index of matching record
 *  @return      WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfd_get_free_persistent_rec_index(s8 * pr_index)
{
    int index, ret = WPS_STATUS_SUCCESS;
    wfd_persistent_record prec;

    ENTER();

    for (index = 0; index < WFD_PERSISTENT_GROUP_MAX; index++) {
        memset(&prec, 0, sizeof(wfd_persistent_record));

        prec.index = index;
        if ((ret =
             wfd_cfg_cmd_persistent_group_record(ACTION_GET,
                                                 &prec)) !=
            WPS_STATUS_SUCCESS) {
            goto done;
        }

        if (prec.ssid_len == 0) {
            /* free record found */
            *pr_index = index;
            goto done;
        }
    }

    /* record not found */
    ret = WPS_STATUS_FAIL;

  done:
    LEAVE();
    return ret;
}

/*
 *  @brief    Update a peristent record based on current info
 *  @param pwps_info   A pointer to the PWPS_INFO structure
 *  @return            WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */

int
wfd_update_persistent_record(PWPS_INFO pwps_info)
{
    int ret = WPS_STATUS_SUCCESS;
    int registrar = (IS_DISCOVERY_REGISTRAR(pwps_info)) ? 1 : 0;
    wfd_persistent_record prec;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    CREDENTIAL_DATA *reg_cred = (CREDENTIAL_DATA *)
        & pwps_info->registrar.cred_data[0];

    ENTER();

//XXX: Check disabled for now
#if 0
    if (!(pwps_info->self_p2p_grp_cap & WFD_GROUP_CAP_PERSIST_GROUP_BIT)) {
        /* Persistent group not enabled */
        return WPS_STATUS_FAIL;
    }
#endif

    memset(&prec, 0, sizeof(wfd_persistent_record));

    /* Pick the correct record */
    if ((ret = wfd_get_persistent_rec_ind_by_ssid(reg_cred->ssid_length,
                                                  reg_cred->ssid,
                                                  &prec.index)) !=
        WPS_STATUS_SUCCESS) {
        /* FW didnt create a Persistent record entry */
        wps_printf(DEBUG_WFD_DISCOVERY,
                   "No persistent rec with req ssid found\n");
        prec.index = -1;
        if (pwps_info->discovery_role != WPS_ENROLLEE) {
            /** This is wrong - can occur only when
             *  enrollee connects to existing GO 
             **/
            return ret;
        }
    }

    if (prec.index != -1) {
        /* First, read the record from FW */
        wps_printf(DEBUG_WFD_DISCOVERY, "Reading Rec from FW\n");
        wfd_cfg_cmd_persistent_group_record(ACTION_GET, &prec);
        wps_hexdump(MSG_INFO, "Prec:", (u8 *) & prec, sizeof(prec));
    } else {
        if (wfd_get_free_persistent_rec_index(&prec.index) == WPS_STATUS_FAIL) {
            /* No free persistent record entry found */
            return ret;
        }
        wps_printf(DEBUG_WFD_DISCOVERY,
                   "Creating new Persistent record @ index: %d\n", prec.index);

        prec.dev_role = EV_SUBTYPE_WFD_CLIENT_ROLE;
        memcpy(prec.groupdevid, pwps_info->registrar.mac_address, ETH_ALEN);
        prec.ssid_len = reg_cred->ssid_length;
        memcpy(prec.ssid, reg_cred->ssid, reg_cred->ssid_length);
    }

    if (!registrar) {
        /* For Enrollee, just update the PSK info */
        if (reg_cred->network_key_len == 64) {
            prec.psk_len = reg_cred->network_key_len / 2;
            hexstr2bin((s8 *) reg_cred->network_key,
                       prec.psk, reg_cred->network_key_len);
        } else {
            prec.psk_len = reg_cred->network_key_len;
            memcpy(prec.psk, reg_cred->network_key, prec.psk_len);
        }
    } else {
        /* For Registrar, update the Client Device Address * Do not use
           intended address */
        if (pwfd_data->dev_index != -1) {
            prec.num_peers = 1;
            memcpy(prec.peers[0],
                   pwfd_data->find_results[pwfd_data->dev_index].device_address,
                   ETH_ALEN);
        }
    }

    wps_printf(DEBUG_WFD_DISCOVERY, "Update Rec to FW\n");
    /* Write back to the FW */
    wfd_cfg_cmd_persistent_group_record(ACTION_SET, &prec);

    LEAVE();
    return ret;
}

/** 
 *  @brief Start WFD Peer ageout timer
 *  
 *  @param pwfd_data     A pointer to WFD_DATA
 */
void
wfd_start_peer_ageout_timer(WFD_DATA * pwfd_data)
{
    u32 sec, usec;
    sec = WFD_PEER_AGEOUT_TIME;
    usec = 0;

    ENTER();

    wps_start_timer(sec, usec, wfd_peer_ageout_time_handler, pwfd_data);

    LEAVE();

    return;
}

/** 
 *  @brief WFD peer ageout time time-out handler
 *  
 *  @param user_data    User private data
 *  @return             None
 */
void
wfd_peer_ageout_time_handler(void *user_data)
{
    WFD_DATA *pwfd_data = user_data;
    struct timeval current_time, ageout_time, res;
    int i = 0;

    ENTER();

    ageout_time.tv_sec = WFD_PEER_AGEOUT_TIME;
    ageout_time.tv_usec = 0;

    os_get_time(&current_time);
    timersub(&current_time, &pwfd_data->find_results[i].timestamp, &res);

    for (i = 0; i < pwfd_data->num_find_results; i++) {
	    timersub(&current_time, &pwfd_data->find_results[i].timestamp, &res);
        if (timercmp(&res, &ageout_time, >=)) {
            pwfd_data->find_results[i].is_valid = 0;
            wps_printf(DEBUG_WLAN, "Last event from ");
            wps_printf(DEBUG_WLAN, "%02x:%02x:%02x:%02x:%02x:%02x",
                       (unsigned int) pwfd_data->find_results[i].device_id[0],
                       (unsigned int) pwfd_data->find_results[i].device_id[1],
                       (unsigned int) pwfd_data->find_results[i].device_id[2],
                       (unsigned int) pwfd_data->find_results[i].device_id[3],
                       (unsigned int) pwfd_data->find_results[i].device_id[4],
                       (unsigned int) pwfd_data->find_results[i].device_id[5]);
            wps_printf(DEBUG_WLAN, " came %d seconds ago.\n", res.tv_sec);
            /** Re print the table */
            wfd_reprint_find_table();
        }
    }

    /** Restart the timer */
    wps_start_timer(WFD_PEER_AGEOUT_TIME, 0, wfd_peer_ageout_time_handler,
                    pwfd_data);

    LEAVE();

    return;
}

/** 
 *  @brief Re-print the find table
 *  
 *  @return             None
 */
void
wfd_reprint_find_table(void)
{

    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    int i = 0, j = 0, updated_num_find_results = 0, len = 0, display_index;

    int k = 0;

    ENTER();

    for (i = 0; i < pwfd_data->num_find_results; i++) {
        if (pwfd_data->find_results[i].is_valid)
            updated_num_find_results++;
    }
    /* Clear screen system("clear"); */
    if (updated_num_find_results > 0) {
        //wps_printf(MSG_INFO, "\E[H\E[J");
        wps_printf(MSG_INFO, "\nFind Result : %d entries in List\n",
               updated_num_find_results);

        wps_printf(MSG_INFO, 
             "--------------------------------------------------------------"
             "-------------------------------------------\n");
        wps_printf(MSG_INFO, 
             "  # |    Intf Address   |          Device Name           |  Op "
             "mode |   Dev Address     |  SSID         |\n");
        wps_printf(MSG_INFO, 
             "--------------------------------------------------------------"
             "-------------------------------------------\n");

        for (j = 0, display_index = 0; j < pwfd_data->num_find_results; j++) {
            if (pwfd_data->find_results[j].is_valid) {

                /* Index Number */
                wps_printf(MSG_INFO, " %02d | ", display_index);

                /* Device ID */
                print_mac(pwfd_data->find_results[j].device_id);
                wps_printf(MSG_INFO, " |");

                /** Device Name printing in smart way  */
                len = (MAX_DEVICE_NAME_LEN - device_name_len[j]) / 2;
                for (i = 0; i < len; i++)
                    wps_printf(MSG_INFO, " ");
                for (i = 0; i < device_name_len[j]; i++)
                    wps_printf(MSG_INFO, "%c", wps_device_name[j][i]);
                for (i = 0; i < len; i++)
                    wps_printf(MSG_INFO, " ");

                wps_printf(MSG_INFO, "|");

                /* Group Capability */
                if (!
                    (pwfd_data->find_results[j].
                     group_capability & WFD_GROUP_OWNER_MASK))
                    wps_printf(MSG_INFO, "  Device ");
                else
                    wps_printf(MSG_INFO, "    GO   ");

                if (memcmp(pwfd_data->find_results[j].device_address,
                           "\x00\x00\x00\x00\x00\x00", ETH_ALEN)) {
                    wps_printf(MSG_INFO, " | ");
                    print_mac(pwfd_data->find_results[j].device_address);
                    wps_printf(MSG_INFO, " |");
                }
                if (pwfd_data->find_results[j].group_capability &
                    WFD_GROUP_OWNER_MASK)
                    wps_printf(MSG_INFO, "  %s", pwfd_data->find_results[j].go_ssid);

                wps_printf(MSG_INFO, "\n");
                display_index++;
            }
        }

        if (gpwps_info->enrollee.device_password_id == DEVICE_PASSWORD_ID_PIN) {
            if (gpwps_info->wfd_pin_generated == WPS_SET) {
                wps_printf(MSG_INFO, "\nPIN Number is : ");
                for (k = 0; k < gpwps_info->PINLen; k++)
                    wps_printf(MSG_INFO, "%c", gpwps_info->PIN[k]);
                wps_printf(MSG_INFO, "\n");
            }
        }
        wps_printf(MSG_INFO, "\nSelect Device Id to Connect:\n");
    }
    LEAVE();
}
