/** @file  wifidirectutl.c
 *
 *  @brief Program to configure WifiDirect parameters.
 *
 * (C) Copyright 2011-2013 Marvell International Ltd. All Rights
 * Reserved 
 *
 * MARVELL CONFIDENTIAL
 * Copyright 2008 ~ 2011 Marvell International Ltd All Rights Reserved.
 * The source code contained or described herein and all documents related to
 * the source code ("Material") are owned by Marvell International Ltd or its
 * suppliers or licensors. Title to the Material remains with Marvell International Ltd
 * or its suppliers and licensors. The Material contains trade secrets and
 * proprietary and confidential information of Marvell or its suppliers and
 * licensors. The Material is protected by worldwide copyright and trade secret
 * laws and treaty provisions. No part of the Material may be used, copied,
 * reproduced, modified, published, uploaded, posted, transmitted, distributed,
 * or disclosed in any way without Marvell's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual
 * property right is granted to or conferred upon you by disclosure or delivery
 * of the Materials, either expressly, by implication, inducement, estoppel or
 * otherwise. Any license under such intellectual property rights must be
 * express and approved by Marvell in writing.
 *
 */
/****************************************************************************
  Change log:
  07/10/09: Initial creation
 ****************************************************************************/

/****************************************************************************
  Header files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "memAPI.h"
#include "os_net_device.h"
#include "os_wireless.h"
#include "wlanInterface.h"
#include "logger.h"

#include "platform.h"
#include "platform_api.h"
#include "wifi_config.h"
#include "wifidirectutl.h"
#include "wifidirect_api.h"
#include "uuid.h"
#include "model_info.h"

/****************************************************************************
  Definitions
 ****************************************************************************/

/****************************************************************************
  Global variables
 ****************************************************************************/
/** Device name */
static char dev_name[] = WIFI_IFACE_NAME_WFD;
static t_u8 *wfd_discover_request_dns_ptr = NULL;
static t_u8 *wfd_discover_response_dns_ptr = NULL;
static t_u8 *wfd_gas_comeback_response_dns_ptr = NULL;

/* Global WFD configuration and WFD Params Configuration*/
static wifidirect_config_t *g_wfd_config = NULL;
static wifidirect_param_config_t *g_wfd_params_config = NULL;

/****************************************************************************
  Local functions
 ***************************************************************************/
/** 
 *  @brief Prints a MAC address in colon separated form from hex data
 *
 *  @param raw      A pointer to the hex data buffer
 *  @return         N/A
 */
static void
print_mac(t_u8 * raw)
{
    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("%02x:%02x:%02x:%02x:%02x:%02x", (unsigned int) raw[0],
           (unsigned int) raw[1], (unsigned int) raw[2], (unsigned int) raw[3],
           (unsigned int) raw[4], (unsigned int) raw[5]));
    return;
}

/** 
 *  @brief Performs the ioctl operation to send the command to
 *  the driver.
 *
 *  @param cmd           Pointer to the command buffer
 *  @param size          Pointer to the command size. This value is
 *                       overwritten by the function with the size of the
 *                       received response.
 *  @param buf_size      Size of the allocated command buffer
 *  @return              SUCCESS or FAILURE
 */
static int
wifidirect_ioctl(t_u8 * cmd, t_u16 * size, t_u16 buf_size)
{
    struct ifreq ifr;
    wifidirectcmdbuf *header = NULL;

    if (buf_size < *size) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("buf_size should not less than cmd buffer size\n"));
        return FAILURE;
    }

    *(t_u32 *) cmd = buf_size - BUF_HEADER_SIZE;

    /* Initialize the ifr structure */
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_ifrn.ifrn_name, dev_name, strlen(dev_name));
    ifr.ifr_ifru.ifru_data = (void *) cmd;
    header = (wifidirectcmdbuf *) cmd;
    header->size = *size - BUF_HEADER_SIZE;
#if 0
    if (header->cmd_code == HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG) {
        wifidirect_params_config *param_cfg;
        param_cfg = (wifidirect_params_config *) cmd;
        param_cfg->action = cpu_to_le16(param_cfg->action);
    }
#endif
    endian_convert_request_header(header);

    /* Perform ioctl */
    if (WlanIoctl(WIFIDIRECT_HOSTCMD, &ifr)) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("ERR:WIFIDIRECT_HOSTCMD is not supported by %s\n", dev_name));
        return FAILURE;
    }
    endian_convert_response_header(header);
    header->cmd_code &= HostCmd_CMD_ID_MASK;
    header->cmd_code |= WIFIDIRECTCMD_RESP_CHECK;
    *size = header->size;

    /* Validate response size */
    if (*size > (buf_size - BUF_HEADER_SIZE)) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), 
                ("ERR:Response size (%d) greater than buffer size (%d)! Aborting!\n",
                 *size, buf_size));
        return FAILURE;
    }

    return SUCCESS;
}


/** 
 *  @brief Read the wifidirect parameters and sends to the driver
 *
 *  @param config               Pointer to config structure
 *  @param pbuf                 Pointer to output buffer
 *  @param ie_len_wifidirect    Length of wifidirect parameters to return 
 *  @param ie_len_wps           Length of WPS parameters to return 
 *  @return          SUCCESS or FAILURE
 */
static int
wifidirect_file_params_config(wifidirect_config_t *config, t_u8 * pbuf, t_u16 * ie_len_wifidirect, t_u16 * ie_len_wps)
{
    int retval = FAILURE;
    int i, offset;
    t_u16 temp = 0;
    t_u8 *buffer = pbuf;
    t_u16 cmd_len_wifidirect = 0, cmd_len_wps = 0, tlv_len = 0;

    // WIFIDIRECT_CAPABILITY_CONFIG:
    {
        tlvbuf_wifidirect_capability *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_capability);
        tlv =
            (tlvbuf_wifidirect_capability *) (buffer +
                                              cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_CAPABILITY;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        tlv->dev_capability = config->capability.dev_capability;
        tlv->group_capability = config->capability.group_capability;
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_GROUP_OWNER_INTENT_CONFIG:
    {
        tlvbuf_wifidirect_group_owner_intent *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_group_owner_intent);
        tlv =
            (tlvbuf_wifidirect_group_owner_intent *) (buffer +
                                                      cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_GROUPOWNER_INTENT;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        tlv->dev_intent = config->group_owner_intent;
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_MANAGEABILITY_CONFIG:
    {
        tlvbuf_wifidirect_manageability *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_manageability);
        tlv =
            (tlvbuf_wifidirect_manageability *) (buffer +
                                                 cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_MANAGEABILITY;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        tlv->manageability = config->manageability;
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_INVITATION_FLAG_CONFIG:
    if (config->invitation_flag)
    {
        tlvbuf_wifidirect_invitation_flag *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_invitation_flag);
        tlv =
            (tlvbuf_wifidirect_invitation_flag *) (buffer +
                                                   cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_INVITATION_FLAG;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        tlv->invitation_flag = config->invitation_flag;
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_CHANNEL_LIST_CONFIG:
    {
        tlvbuf_wifidirect_channel_list *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_channel_list)
            + config->channel_list.num_channel_list_entries * sizeof(chan_entry);
        for(i=0; i<config->channel_list.num_channel_list_entries; i++){
            tlv_len += config->channel_list.chan_entry_list[i].num_of_channels * sizeof(t_u8);
        }
        tlv =
            (tlvbuf_wifidirect_channel_list *) (buffer +
                                                cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_CHANNEL_LIST;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->country_string, config->channel_list.country_string, 3);
        if (tlv->country_string[2] == 0)
            tlv->country_string[2] = WIFIDIRECT_COUNTRY_LAST_BYTE;

        for (i=0, offset=0; i<config->channel_list.num_channel_list_entries; i++) {
            memcpy(((t_u8 *) (&tlv->wifidirect_chan_entry_list)) + offset,
                   &config->channel_list.chan_entry_list[i].regulatory_class, 
                   sizeof(t_u8));
            offset += sizeof(t_u8);
            memcpy(((t_u8 *) (&tlv->wifidirect_chan_entry_list)) + offset,
                   &config->channel_list.chan_entry_list[i].num_of_channels, 
                   sizeof(t_u8));
            offset += sizeof(t_u8);
            memcpy(((t_u8 *) (&tlv->wifidirect_chan_entry_list)) + offset,
                   &config->channel_list.chan_entry_list[i].chan_list, 
                   config->channel_list.chan_entry_list[i].num_of_channels * sizeof(t_u8));
            offset += config->channel_list.chan_entry_list[i].num_of_channels * sizeof(t_u8);
        }
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_NOTICE_OF_ABSENCE:
    {
        tlvbuf_wifidirect_notice_of_absence *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_notice_of_absence)
            + config->notice_of_absence.noa_descriptor_count * sizeof(noa_descriptor);
        tlv =
            (tlvbuf_wifidirect_notice_of_absence *) (buffer +
                                                     cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_NOTICE_OF_ABSENCE;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        tlv->noa_index = config->notice_of_absence.noa_index;
        tlv->ctwindow_opp_ps = config->notice_of_absence.ctwindow_opp_ps;

        for (i=0, offset=0; i<config->notice_of_absence.noa_descriptor_count; i++) {
            memcpy(((t_u8 *) (&tlv->wifidirect_noa_descriptor_list)) + offset,
                   &config->notice_of_absence.noa_descriptor_list[i].count_type,
                   sizeof(t_u8));
            offset += sizeof(t_u8);
            memcpy(((t_u8 *) (&tlv->wifidirect_noa_descriptor_list)) + offset,
                   &config->notice_of_absence.noa_descriptor_list[i].duration,
                   sizeof(t_u32));
            offset += sizeof(t_u32);
            memcpy(((t_u8 *) (&tlv->wifidirect_noa_descriptor_list)) + offset,
                   &config->notice_of_absence.noa_descriptor_list[i].interval,
                   sizeof(t_u32));
            offset += sizeof(t_u32);
            memcpy(((t_u8 *) (&tlv->wifidirect_noa_descriptor_list)) + offset,
                   &config->notice_of_absence.noa_descriptor_list[i].start_time,
                   sizeof(t_u32));
            offset += sizeof(t_u32);
        }
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_CHANNEL_CONFIG:
    {
        tlvbuf_wifidirect_channel *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_channel);
        tlv =
            (tlvbuf_wifidirect_channel *) (buffer +
                                           cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_CHANNEL;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->country_string, config->channel.country_string, 3);
        if (tlv->country_string[2] == 0)
            tlv->country_string[2] = WIFIDIRECT_COUNTRY_LAST_BYTE;
        tlv->regulatory_class = config->channel.regulatory_class;
        tlv->channel_number = config->channel.channel_number;
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_OPCHANNEL_CONFIG:
    {
        tlvbuf_wifidirect_channel *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_channel);
        tlv =
            (tlvbuf_wifidirect_channel *) (buffer +
                                           cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_OPCHANNEL;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->country_string, config->operating_channel.country_string, 3);
        if (tlv->country_string[2] == 0)
            tlv->country_string[2] = WIFIDIRECT_COUNTRY_LAST_BYTE;
        tlv->regulatory_class = config->operating_channel.regulatory_class;
        tlv->channel_number = config->operating_channel.channel_number;
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    //WIFIDIRECT_DEVICE_INFO_CONFIG:
    {
        tlvbuf_wifidirect_device_info *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_device_info) +
            config->device_info.secondary_dev_count * WPS_DEVICE_TYPE_LEN +
            strlen((char *)config->device_info.device_name);
        tlv =
            (tlvbuf_wifidirect_device_info *) (buffer +
                                               cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_DEVICE_INFO;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->dev_address, config->device_info.dev_address, ETH_ALEN);
        tlv->config_methods = htons(config->device_info.config_methods);
        tlv->primary_category = htons(config->device_info.primary_category);
        memcpy(tlv->primary_oui, config->device_info.primary_oui, 4);
        tlv->primary_subcategory = htons(config->device_info.primary_subcategory);
        tlv->secondary_dev_count = config->device_info.secondary_dev_count;
        endian_convert_tlv_wifidirect_header_out(tlv);

        for (i=0, offset=0; i<config->device_info.secondary_dev_count; i++) {
            temp = htons(config->device_info.secondary_dev_info[i].secondary_category);
            memcpy(((t_u8 *) (&tlv->secondary_dev_info)) + offset, &temp, sizeof(temp));
            offset += sizeof(temp);
            memcpy(((t_u8 *) (&tlv->secondary_dev_info)) + offset,
                   &config->device_info.secondary_dev_info[i].secondary_oui,
                   sizeof(config->device_info.secondary_dev_info[i].secondary_oui));
            offset += sizeof(config->device_info.secondary_dev_info[i].secondary_oui);
            temp = htons(config->device_info.secondary_dev_info[i].secondary_subcategory);
            memcpy(((t_u8 *) (&tlv->secondary_dev_info)) + offset, &temp, sizeof(temp));
            offset += sizeof(temp);
        }
        temp = htons(SC_Device_Name);
        memcpy(((t_u8 *) (&tlv->device_name_type)) + offset, &temp, sizeof(temp));
        temp = htons(strlen((char *)config->device_info.device_name));
        memcpy(((t_u8 *) (&tlv->device_name_len)) + offset, &temp, sizeof(temp));
        memcpy(((t_u8 *) (&tlv->device_name)) + offset,
               config->device_info.device_name, strlen((char *)config->device_info.device_name));
    }
#if 0
        case WIFIDIRECT_GROUP_INFO_CONFIG:
            {
                tlvbuf_wifidirect_group_info *tlv = NULL;
                /* Append a new TLV */
                tlv_offset = wifidirect_group_total_ssid_len
                    +
                    wifidirect_total_secondary_dev_count *
                    WPS_DEVICE_TYPE_LEN;
                tlv_len = sizeof(tlvbuf_wifidirect_group_info)
                    +
                    wifidirect_client_dev_count *
                    sizeof(wifidirect_client_dev_info) + tlv_offset;
                tlv =
                    (tlvbuf_wifidirect_group_info *) (buffer +
                                                      cmd_len_wifidirect);
                cmd_len_wifidirect += tlv_len;
                /* Set TLV fields */
                tlv->tag = TLV_TYPE_WIFIDIRECT_GROUP_INFO;
                tlv->length = tlv_len;
                memcpy(tlv->wifidirect_client_dev_list,
                       wifidirect_client_dev_info_list,
                       wifidirect_client_dev_count *
                       sizeof(wifidirect_client_dev_info) + tlv_offset);
                /* Parameters within secondary_dev_info are already
                   htons'ed */
                // wps_hexdump(DEBUG_WLAN,"Group Info Hexdump:",
                // (t_u8*)tlv, tlv_len);
                endian_convert_tlv_wifidirect_header_out(tlv);
                flag = 1;
                break;
            }

        case WIFIDIRECT_GROUP_SEC_INFO_CONFIG:
            {
                wifidirect_level = WIFIDIRECT_GROUP_CLIENT_INFO_CONFIG;

                if (wifidirect_client_dev_index && group_secondary_index) {
                    memset(((t_u8 *) &
                            wifidirect_client_dev_info_list
                            [wifidirect_client_dev_index -
                             1].wifidirect_secondary_dev_info + tlv_offset),
                           0, group_secondary_index * WPS_DEVICE_TYPE_LEN);
                    memcpy(((t_u8 *) &
                            wifidirect_client_dev_info_list
                            [wifidirect_client_dev_index -
                             1].wifidirect_secondary_dev_info + tlv_offset),
                           &group_secondary_dev_info,
                           group_secondary_index * WPS_DEVICE_TYPE_LEN);
                }
                tlv_offset = wifidirect_group_total_ssid_len
                    +
                    wifidirect_total_secondary_dev_count *
                    WPS_DEVICE_TYPE_LEN;
                flag = 0;
                break;
            }
        case WIFIDIRECT_GROUP_CLIENT_INFO_CONFIG:
            {
                wifidirect_level = WIFIDIRECT_GROUP_INFO_CONFIG;
                flag = 0;
                break;
            }
        case WIFIDIRECT_DEVICE_SEC_INFO_CONFIG:
            {
                wifidirect_level = WIFIDIRECT_DEVICE_INFO_CONFIG;
                flag = 0;
                break;
            }
#endif
    // WIFIDIRECT_GROUP_ID_CONFIG:
    {
        tlvbuf_wifidirect_group_id *tlv = NULL;
        /* Append a new TLV */
        tlv_len =
            sizeof(tlvbuf_wifidirect_group_id) +
            strlen((char *)config->group_id.group_ssid);
        tlv =
            (tlvbuf_wifidirect_group_id *) (buffer +
                                            cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_GROUP_ID;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->group_address, config->group_id.group_address, ETH_ALEN);
        memcpy(tlv->group_ssid, config->group_id.group_ssid,
               strlen((char *)config->group_id.group_ssid));
        endian_convert_tlv_wifidirect_header_out(tlv);
    }
    // WIFIDIRECT_GROUP_BSS_ID_CONFIG:
    {
        tlvbuf_wifidirect_group_bss_id *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_group_bss_id);
        tlv =
            (tlvbuf_wifidirect_group_bss_id *) (buffer +
                                                cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_GROUP_BSS_ID;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->group_bssid, config->group_bss_id, ETH_ALEN);
        endian_convert_tlv_wifidirect_header_out(tlv);
    }
    // WIFIDIRECT_DEVICE_ID_CONFIG:
    {
        tlvbuf_wifidirect_device_id *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_device_id);
        tlv =
            (tlvbuf_wifidirect_device_id *) (buffer +
                                             cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_DEVICE_ID;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->dev_mac_address, config->device_id_mac_addr, ETH_ALEN);
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_INTERFACE_CONFIG:
    {
        tlvbuf_wifidirect_interface *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_interface)
            + config->interface.interface_count * ETH_ALEN;
        tlv =
            (tlvbuf_wifidirect_interface *) (buffer +
                                             cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_INTERFACE;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->interface_id, config->interface.interface_id, ETH_ALEN);
        tlv->interface_count = config->interface.interface_count;
        for (i=0,offset=0; i<config->interface.interface_count; i++) {
            memcpy(((t_u8 *) (&tlv->interface_idlist)) + offset, 
                   config->interface.interface_idlist[i].interface_id, ETH_ALEN);
            offset += ETH_ALEN;
        }
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_TIMEOUT_CONFIG:
    {
        tlvbuf_wifidirect_config_timeout *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_config_timeout);
        tlv =
            (tlvbuf_wifidirect_config_timeout *) (buffer +
                                                  cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_CONFIG_TIMEOUT;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        tlv->group_config_timeout = config->config_timeout.group_config_timeout;
        tlv->device_config_timeout = config->config_timeout.device_config_timeout;
        endian_convert_tlv_wifidirect_header_out(tlv);
    }
    // WIFIDIRECT_EXTENDED_TIME_CONFIG:
    {
        tlvbuf_wifidirect_ext_listen_time *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_ext_listen_time);
        tlv =
            (tlvbuf_wifidirect_ext_listen_time *) (buffer +
                                                   cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_EXTENDED_LISTEN_TIME;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        tlv->availability_period = le16_to_cpu(config->ext_listen_time.availability_period);
        tlv->availability_interval = le16_to_cpu(config->ext_listen_time.availability_interval);
        endian_convert_tlv_wifidirect_header_out(tlv);
    }
    // WIFIDIRECT_INTENDED_ADDR_CONFIG:
    {
        tlvbuf_wifidirect_intended_addr *tlv = NULL;
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wifidirect_intended_addr);
        tlv =
            (tlvbuf_wifidirect_intended_addr *) (buffer +
                                                 cmd_len_wifidirect);
        cmd_len_wifidirect += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WIFIDIRECT_INTENDED_ADDRESS;
        tlv->length = tlv_len - (sizeof(t_u8) + sizeof(t_u16));
        memcpy(tlv->group_address, config->intended_addr, ETH_ALEN);
        endian_convert_tlv_wifidirect_header_out(tlv);
    }

    // WIFIDIRECT_WPSIE:
    {
        /* Append TLV for WPSVersion */
        tlvbuf_wps_ie *tlv = NULL;
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSVersion);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Version;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        *(tlv->data) = config->wps_config.WPSVersion;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSSetupState */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSSetupState);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Simple_Config_State;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        *(tlv->data) = config->wps_config.WPSSetupState;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSRequestType */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSRequestType);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Request_Type;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        *(tlv->data) = config->wps_config.WPSRequestType;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSResponseType */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSResponseType);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Response_Type;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        *(tlv->data) = config->wps_config.WPSResponseType;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSSpecConfigMethods */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSSpecConfigMethods);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Config_Methods;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        temp = htons(config->wps_config.WPSSpecConfigMethods);
        memcpy((t_u16 *) tlv->data, &temp, sizeof(temp));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSUUID */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSUUID);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_UUID_E;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        memcpy(tlv->data, config->wps_config.WPSUUID, WPS_UUID_MAX_LEN);
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSPrimaryDeviceType */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSPrimaryDeviceType);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Primary_Device_Type;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        memcpy(tlv->data, config->wps_config.WPSPrimaryDeviceType,
               WPS_DEVICE_TYPE_MAX_LEN);
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSRFBand */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSRFBand);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_RF_Band;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        *(tlv->data) = config->wps_config.WPSRFBand;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSAssociationState */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSAssociationState);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Association_State;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        temp = htons(config->wps_config.WPSAssociationState);
        memcpy((t_u16 *) tlv->data, &temp, sizeof(temp));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSConfigurationError */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSConfigurationError);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Configuration_Error;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        temp = htons(config->wps_config.WPSConfigurationError);
        memcpy((t_u16 *) tlv->data, &temp, sizeof(temp));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSDevicePassword */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(config->wps_config.WPSDevicePassword);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Device_Password_ID;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        temp = htons(config->wps_config.WPSDevicePassword);
        memcpy((t_u16 *) tlv->data, &temp, sizeof(temp));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSDeviceName */
        tlv_len = sizeof(tlvbuf_wps_ie) + strlen((char *)config->wps_config.WPSDeviceName);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Device_Name;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        memcpy(tlv->data, config->wps_config.WPSDeviceName, 
               strlen((char *)config->wps_config.WPSDeviceName));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSManufacturer */
        tlv_len = sizeof(tlvbuf_wps_ie) + strlen((char *)config->wps_config.WPSManufacturer);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Manufacturer;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        memcpy(tlv->data, config->wps_config.WPSManufacturer,
               strlen((char *)config->wps_config.WPSManufacturer));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSModelName */
        tlv_len = sizeof(tlvbuf_wps_ie) + strlen((char *)config->wps_config.WPSModelName);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Model_Name;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        memcpy(tlv->data, config->wps_config.WPSModelName, 
               strlen((char *)config->wps_config.WPSModelName));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSModelNumber */
        tlv_len = sizeof(tlvbuf_wps_ie) + strlen((char *)config->wps_config.WPSModelNumber);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Model_Number;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        memcpy(tlv->data, config->wps_config.WPSModelNumber, 
               strlen((char *)config->wps_config.WPSModelNumber));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSSerialNumber */
        tlv_len = sizeof(tlvbuf_wps_ie) + strlen((char *)config->wps_config.WPSSerialNumber);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wifidirect +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Serial_Number;
        tlv->length = tlv_len - 2 * sizeof(t_u16);
        memcpy(tlv->data, config->wps_config.WPSSerialNumber, 
               strlen((char *)config->wps_config.WPSSerialNumber));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;
    }
#if 0
        case WIFIDIRECT_EXTRA:
            {
                memcpy(buffer + cmd_len_wifidirect, extra, extra_len);
                cmd_len_wifidirect += extra_len;
                break;
            }
#endif

    if (ie_len_wifidirect)
        *ie_len_wifidirect = cmd_len_wifidirect;
    if (ie_len_wps)
        *ie_len_wps = cmd_len_wps;

#if 0
    DPRINTF((DBG_ERROR | DBG_LOUD), ("wifidirect_file_params_config: cmd_len_wifidirect = %d cmd_len_wps = %d\n",
                                      cmd_len_wifidirect, cmd_len_wps));

    DPRINTF((DBG_ERROR | DBG_LOUD), ("P2P_IE\n"));
    for (i=0;i<(cmd_len_wifidirect);i++) {
        if (i%16 == 0) {
            DPRINTF((DBG_ERROR | DBG_LOUD), ("\n"));
        }
        DPRINTF((DBG_ERROR | DBG_LOUD), ("0x%02X ", *(pbuf+i)));
    }
    DPRINTF((DBG_ERROR | DBG_LOUD), ("\n\n"));

    DPRINTF((DBG_ERROR | DBG_LOUD), ("WPS_IE\n"));
    for (i=0;i<(cmd_len_wps);i++) {
        if (i%16 == 0) {
            DPRINTF((DBG_ERROR | DBG_LOUD), ("\n"));
        }
        DPRINTF((DBG_ERROR | DBG_LOUD), ("0x%02X ", *(pbuf+i+cmd_len_wifidirect+sizeof(custom_ie))));
    }
    DPRINTF((DBG_ERROR | DBG_LOUD), ("\n\n"));
#endif
    if ((cmd_len_wifidirect > 0) || (cmd_len_wps > 0)) {
        retval = SUCCESS;
    }

    return retval;
}


/** 
 *  @brief Process and send ie config command 
 *  @param ie_index             A pointer to the IE buffer index
 *  @param data_len_wifidirect  Length of WIFIDIRECT data, 0 to get, else set.
 *  @param data_len_wps         Length of WPS data, 0 to get, else set.
 *  @param buf                  Pointer to buffer to set.
 *  @return          SUCCESS--success, FAILURE--fail
 */
static int
wifidirect_ie_config(t_u16 * ie_index, t_u16 data_len_wifidirect,
                     t_u16 data_len_wps, t_u8 * buf)
{
    struct iwreq iwr;
    int i, ret = SUCCESS;
    tlvbuf_custom_ie *tlv = NULL;
    custom_ie *ie_ptr = NULL;

    tlv = (tlvbuf_custom_ie *) buf;
    tlv->tag = MRVL_MGMT_IE_LIST_TLV_ID;
    /* Locate headers */
    ie_ptr = (custom_ie *) (tlv->ie_data);

    /* Set TLV fields : WIFIDIRECT IE parameters */
    if (data_len_wifidirect) {
        /* Set IE */
#define MGMT_MASK_AUTO	0xffff
        ie_ptr->mgmt_subtype_mask = MGMT_MASK_AUTO;
        tlv->length = sizeof(custom_ie) + data_len_wifidirect;
        ie_ptr->ie_length = data_len_wifidirect;
        ie_ptr->ie_index = *ie_index;
    } else {
        /* Get WPS IE */
        tlv->length = 0;
    }

    (*ie_index)++;

    /* Locate headers */
    ie_ptr =
        (custom_ie *) ((t_u8 *) (tlv->ie_data) + sizeof(custom_ie) +
                       data_len_wifidirect);

    /* Set WPS IE parameters */
    if (data_len_wps) {
        /* Set IE */
        /* Firmware Handled IE - mask should be set to -1 */
        ie_ptr->mgmt_subtype_mask = MGMT_MASK_AUTO;
        tlv->length += sizeof(custom_ie) + data_len_wps;
        ie_ptr->ie_length = data_len_wps;
        ie_ptr->ie_index = *ie_index;
    }

    memset(&iwr, 0, sizeof(iwr));
    strncpy(iwr.ifr_name, (char *) dev_name, IFNAMSIZ);

    iwr.u.data.pointer = (void *) buf;
    iwr.u.data.length = ((2 * sizeof(custom_ie)) + sizeof(tlvbuf_custom_ie) +
                         data_len_wifidirect + data_len_wps);
    iwr.u.data.flags = 0;
#if 0
    DPRINTF((DBG_ERROR | DBG_LOUD), ("wifidirect_ie_config: %d\n", iwr.u.data.length));

    for (i=0;i<(iwr.u.data.length);i++) {
        if (i%16 == 0) {
            DPRINTF((DBG_ERROR | DBG_LOUD), ("\n"));
        }
        DPRINTF((DBG_ERROR | DBG_LOUD), ("0x%02X ", *(buf+i)));
    }
    DPRINTF((DBG_ERROR | DBG_LOUD), ("\n\n"));
#endif
    if (WlanIoctl(CUSTOM_IE, &iwr)) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT), ("Failed to set/get/clear the IE buffer\n"));
        ret = FAILURE;
        goto _exit_;
    }

/** Max IE index */
#define MAX_MGMT_IE_INDEX   12

    if (!data_len_wifidirect) {
        /* Get the IE buffer index number for MGMT_IE_LIST_TLV */
        tlv = (tlvbuf_custom_ie *) buf;
        *ie_index = 0xFFFF;
        if (tlv->tag == MRVL_MGMT_IE_LIST_TLV_ID) {
            ie_ptr = (custom_ie *) (tlv->ie_data);
            for (i = 0; i < MAX_MGMT_IE_INDEX; i++) {
                /* zero mask indicates a wps IE, return previous index */
                if (ie_ptr->mgmt_subtype_mask == MGMT_MASK_AUTO &&
                    ie_ptr->ie_length) {
                    *ie_index = ie_ptr->ie_index;
                    break;
                }
                if (i < (MAX_MGMT_IE_INDEX - 1))
                    ie_ptr =
                        (custom_ie *) ((t_u8 *) ie_ptr + sizeof(custom_ie) +
                                       ie_ptr->ie_length);
            }
        }
        if (*ie_index == 0xFFFF) {
            DPRINTF((DBG_ERROR | DBG_LOUD), ("No MEM_FREE_AND_NULL IE buffer available\n"));
            ret = FAILURE;
        }
    }
  _exit_:

    return ret;
}


/**
 * @brief Create Wifi-Direct Configuration
 *
 * @param config    Pointer to an allocated buffer with 
 *                  configuration
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_config(wifidirect_config_t **config)
{
    int length;
    t_u8* buffer;
    wifidirect_config_t * ptr;
    error_type_t retval = FAIL;
    char *tmpstr;
    uint8_t  ssid[MAX_SSID_NAME_LEN+1];
    uint8_t  group_ssid[MAX_SSID_NAME_LEN+1];

    buffer = (t_u8 *) MEM_MALLOC(sizeof(wifidirect_config_t));
    if (buffer)
    {
        memset(buffer, 0, sizeof(wifidirect_config_t));

        ptr = (wifidirect_config_t *) buffer;

        WIFIDIRECT_SET_DEV_CAPABILITY(ptr, WIFIDIRECT_CONFIG_DEV_CAPABILITY_DEFAULT);
        WIFIDIRECT_SET_GROUP_CAPABILITY(ptr, WIFIDIRECT_CONFIG_GROUP_CAPABILITY_DEFAULT);
        WIFIDIRECT_SET_GROUP_OWNER_INTENT(ptr, WIFIDIRECT_CONFIG_GROUP_OWNER_INTENT_DEFAULT);
        WIFIDIRECT_SET_CHANNEL_COUNTRY_STRING(ptr, WIFIDIRECT_CONFIG_CHANNEL_COUNTRY_STRING_DEFAULT);
        WIFIDIRECT_SET_CHANNEL_REGULATORY_CLASS(ptr, WIFIDIRECT_CONFIG_CHANNEL_REGULATORY_CLASS_DEFAULT);
        WIFIDIRECT_SET_CHANNEL_NUMBER(ptr, WIFIDIRECT_CONFIG_CHANNEL_CHANNEL_NUMBER_DEFAULT);
        WIFIDIRECT_SET_MANAGEABILITY(ptr, WIFIDIRECT_CONFIG_MANAGEABILITY_DEFAULT);

        WIFIDIRECT_SET_CHANNEL_LIST_COUNTRY_STRING(ptr, WIFIDIRECT_CONFIG_CHANNEL_LIST_COUNTRY_STRING_DEFAULT);
        WIFIDIRECT_SET_CHANNEL_LIST_NUMBER_OF_ENTRIES(ptr, WIFIDIRECT_CONFIG_NUMBER_CHANNEL_LIST_ENTRIES_DEFAULT);
        WIFIDIRECT_SET_CHANNEL_LIST_REGULATORY_CLASS(ptr, 0, WIFIDIRECT_CONFIG_CHANNEL_LIST_REGULATORY_CLASS_DEFAULT);
        WIFIDIRECT_SET_CHANNEL_LIST_NUMBER_OF_CHANNELS(ptr, 0, WIFIDIRECT_CONFIG_CHANNEL_LIST_NUMBER_OF_CHANNELS_DEFAULT);
        {   
            char array[] = WIFIDIRECT_CONFIG_CHANNEL_LIST_CHANNELS_DEFAULT; 
            WIFIDIRECT_SET_CHANNEL_LIST_CHANNEL_LIST(ptr, 0, 
                                                     WIFIDIRECT_CONFIG_CHANNEL_LIST_NUMBER_OF_CHANNELS_DEFAULT,
                                                     array);
        }
        WIFIDIRECT_SET_NOTICE_OF_ABSENCE_NOA_INDEX(ptr, WIFIDIRECT_CONFIG_NOA_INDEX_DEFAULT);
        WIFIDIRECT_SET_NOTICE_OF_ABSENCE_CTWINDOW_OPP_PS(ptr, 
            (SET_OPP_PS(WIFIDIRECT_CONFIG_NOA_OPP_PS_DEFAULT) | WIFIDIRECT_CONFIG_NOA_CTWINDOW_DEFAULT));
        WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_COUNT_TYPE(ptr, 0, WIFIDIRECT_CONFIG_NOA_DESCRIPTOR_COUNT_TYPE_DEFAULT);
        WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_DURATION(ptr, 0, WIFIDIRECT_CONFIG_NOA_DESCRIPTOR_DURATION_DEFAULT);
        WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_INTERVAL(ptr, 0, WIFIDIRECT_CONFIG_NOA_DESCRIPTOR_INTERVAL_DEFAULT);
        WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_START_TIME(ptr, 0, WIFIDIRECT_CONFIG_NOA_DESCRIPTOR_START_TIME_DEFAULT);
        WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_COUNT(ptr, WIFIDIRECT_CONFIG_NOA_DESCRIPTOR_LIST_COUNT_DEFAULT);

        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->device_info.dev_address);
        WIFIDIRECT_SET_DEVICE_INFO_CONFIG_METHODS(ptr, WIFIDIRECT_CONFIG_DEVICE_INFO_CONFIG_METHODS_DEFAULT);
        WIFIDIRECT_SET_DEVICE_INFO_PRIMARY_CATEGORY(ptr, WIFIDIRECT_CONFIG_DEVICE_INFO_PRIMARY_CATEGORY_DEFAULT);
        {
            char array[] = WIFIDIRECT_CONFIG_DEVICE_INFO_PRIMARY_OUI_DEFAULT;
            WIFIDIRECT_SET_DEVICE_INFO_PRIMARY_OUI(ptr, array);
        }
        WIFIDIRECT_SET_DEVICE_INFO_PRIMARY_SUB_CATEGORY(ptr, WIFIDIRECT_CONFIG_DEVICE_INFO_PRIMARY_SUBCATEGORY_DEFAULT);
        WIFIDIRECT_SET_DEVICE_INFO_SECONDARY_DEV_COUNT(ptr, WIFIDIRECT_CONFIG_DEVICE_INFO_SECONDARY_DEVICE_COUNT_DEFAULT);
        WIFIDIRECT_SET_DEVICE_INFO_SECONDARY_CATEGORY(ptr, 0, WIFIDIRECT_CONFIG_DEVICE_INFO_SECONDARY_CATEGORY_DEFAULT);
        {
            char array[] = WIFIDIRECT_CONFIG_DEVICE_INFO_SECONDARY_OUI_DEFAULT;
            WIFIDIRECT_SET_DEVICE_INFO_SECONDARY_OUI(ptr, 0, array);
        }
        WIFIDIRECT_SET_DEVICE_INFO_SECONDARY_SUB_CATEGORY(ptr, 0, WIFIDIRECT_CONFIG_DEVICE_INFO_SECONDARY_SUBCATEGORY_DEFAULT);
        WIFIDIRECT_SET_DEVICE_INFO_DEVICE_NAME_TYPE(ptr, WIFIDIRECT_CONFIG_DEVICE_INFO_DEVICE_NAME_TYPE_DEFAULT);
        get_model_name(&tmpstr);
        strncpy((char *)ptr->device_info.device_name, tmpstr, WPS_DEVICE_NAME_MAX_LEN);
        WIFIDIRECT_SET_DEVICE_INFO_DEVICE_NAME_LENGTH(ptr, strlen((char *)ptr->device_info.device_name));

        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->group_id.group_address);
        wifi_intf_get_var_ssid(WIFI_IFACE_ID_WFD, ssid, MAX_SSID_NAME_LEN);
        wifi_intf_get_var_ssid_prefix(WIFI_IFACE_ID_WFD, group_ssid, MAX_SSID_NAME_LEN);
        strcat((char *)group_ssid,(char *)ssid);          
        WIFIDIRECT_SET_GROUP_SSID(ptr, (char *)group_ssid);
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->group_bss_id);
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->device_id_mac_addr);

        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->interface.interface_id);
        WIFIDIRECT_SET_INTERFACE_COUNT(ptr, WIFIDIRECT_CONFIG_INTERFACE_ID_COUNT_DEFAULT);
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->interface.interface_idlist[0].interface_id);
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->interface.interface_idlist[1].interface_id);

        WIFIDIRECT_SET_GROUP_CONFIG_TIMEOUT(ptr, WIFIDIRECT_CONFIG_GROUP_CONFIGURATION_TIMEOUT_DEFAULT);
        WIFIDIRECT_SET_DEVICE_CONFIG_TIMEOUT(ptr, WIFIDIRECT_CONFIG_DEVICE_CONFIGURATION_TIMEOUT_DEFAULT);
        WIFIDIRECT_SET_EXT_LISTEN_AVAIL_PERIOD(ptr, WIFIDIRECT_CONFIG_EXT_LISTEN_TIME_AVAIL_PERIOD_DEFAULT);
        WIFIDIRECT_SET_EXT_LISTEN_AVAIL_INTERVAL(ptr, WIFIDIRECT_CONFIG_EXT_LISTEN_TIME_AVAIL_INTERVAL_DEFAULT);
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->intended_addr);
        WIFIDIRECT_SET_OPERATING_CHANNEL_NUMBER(ptr, WIFIDIRECT_CONFIG_OPERATING_CHANNEL_CHANNEL_DEFAULT);
        WIFIDIRECT_SET_OPERATING_CHANNEL_COUNTRY_STRING(ptr,WIFIDIRECT_CONFIG_OPERATING_CHANNEL_COUNTRY_DEFAULT);
        WIFIDIRECT_SET_OPERATING_CHANNEL_REGULATORY_CLASS(ptr, WIFIDIRECT_CONFIG_OPERATING_CHANNEL_REGULATORY_CLASS_DEFAULT);
        WIFIDIRECT_SET_INVATION_FLAG(ptr, WIFIDIRECT_CONFIG_INVITATION_FLAG_DEFAULT); 

        // Set WPS
        WIFIDIRECT_SET_WPS_VERSION(ptr, WIFIDIRECT_WPS_VERSION_DEFAULT);
        WIFIDIRECT_SET_WPS_SETUP_STATE(ptr, WIFIDIRECT_WPS_SETUP_STATE_DEFAULT);
        WIFIDIRECT_SET_WPS_REQUEST_TYPE(ptr, WIFIDIRECT_WPS_REQUEST_TYPE_DEFAULT);
        WIFIDIRECT_SET_WPS_RESPONSE_TYPE(ptr, WIFIDIRECT_WPS_RESPONSE_TYPE_DEFAULT);
        WIFIDIRECT_SET_WPS_SPEC_CONFIG_METHODS(ptr, WIFIDIRECT_WPS_SPEC_CONFIG_METHODS_DEFAULT);

        length = WPS_UUID_MAX_LEN;
        get_device_uuid_binary((char *) ptr->wps_config.WPSUUID, &length);

        {
            char array[] = WIFIDIRECT_WPS_PRIMARY_DEVICE_TYPE_DEFAULT;
            WIFIDIRECT_SET_WPS_PRIMARY_DEVICE_TYPE(ptr, array);
        }
        WIFIDIRECT_SET_WPS_RFBAND(ptr, WIFIDIRECT_WPS_RFBAND_DEFAULT); 
        WIFIDIRECT_SET_WPS_ASSOCIATION_STATE(ptr, WIFIDIRECT_WPS_ASSOCIATION_STATE_DEFAULT);
        WIFIDIRECT_SET_WPS_CONFIGURATION_ERROR(ptr, WIFIDIRECT_WPS_CONFIGURATION_ERROR_DEFAULT);
        WIFIDIRECT_SET_WPS_DEVICE_PASSWORD(ptr, WIFIDIRECT_WPS_DEVICE_PASSWORD_DEFAULT);

        get_model_name(&tmpstr);
        // TODO: Seeing issues when the WPSDeviceName is 23 characters long. Specifically, Get Custom IE ioctls 
        // seem to be failing. From that point on, the module stops responding to all ioctls !!
        // Anything more (<=32 characters) or less seems to work fine. 
        // For now, padding the WPS Device name with an extra space at the end, if the length is 23 characters
        if (strlen(tmpstr) == 23)
        {
            strcat(tmpstr, " ");
        }          
        strncpy((char *)ptr->wps_config.WPSDeviceName, tmpstr, WPS_DEVICE_NAME_MAX_LEN);

        platvars_get_usb_mfgstr(&tmpstr);
        strncpy((char *)ptr->wps_config.WPSManufacturer, tmpstr, WPS_MANUFACT_MAX_LEN);

        get_model_name(&tmpstr);
        strncpy((char *)ptr->wps_config.WPSModelName, tmpstr, WPS_MODEL_MAX_LEN);

        get_model_number(&tmpstr);
        strncpy((char *)ptr->wps_config.WPSModelNumber, tmpstr, WPS_MODEL_MAX_LEN);

        platvars_get_prod_serialnum(&tmpstr);
        strncpy((char *)ptr->wps_config.WPSSerialNumber, tmpstr, WPS_SERIAL_MAX_LEN);

        *config = (wifidirect_config_t *) buffer;;

        retval = OK;
    }

    return retval;
}


/**
 * @brief Delete Wifi-Direct Configuration
 *
 * @param config    Pointer to a previously allocated buffer 
 *                  with configuration from
 *                  wifidirect_create_config()
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_delete_config(wifidirect_config_t **config)
{
    void * ptr;
    error_type_t retval = FAIL;

    if (config) {
        ptr = (void *) *config;
        if (ptr) {
            MEM_FREE_AND_NULL(ptr);
            *config = NULL;
            retval = OK;
        }
    }
    return retval;
}


/**
 * @brief Create Wifi-Direct Parameters Configuration
 *
 * @param param_config  Pointer to an allocated buffer with 
 *                      parameter configuration
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_params_config(wifidirect_param_config_t **param_config)
{
    t_u8* buffer;
    wifidirect_param_config_t * ptr;
    error_type_t retval = FAIL;

    if (param_config) {
        buffer = (t_u8 *) MEM_MALLOC(sizeof(wifidirect_param_config_t));

        if (buffer)
        {
            memset(buffer, 0, sizeof(wifidirect_param_config_t));

            ptr = (wifidirect_param_config_t *) buffer;

            WIFIDIRECT_SET_DISC_MIN_INTERVAL(ptr, WIFIDIRECT_PARAM_CONFIG_MIN_DISCOVERY_INTERVAL_DEFAULT);
            WIFIDIRECT_SET_DISC_MAX_INTERVAL(ptr, WIFIDIRECT_PARAM_CONFIG_MAX_DISCOVERY_INTERVAL_DEFAULT);
            WIFIDIRECT_SET_ENABLE_SCAN(ptr, WIFIDIRECT_PARAM_CONFIG_ENABLE_SCAN_DEFAULT);
            WIFIDIRECT_SET_DEVICE_STATE(ptr, WIFIDIRECT_PARAM_CONFIG_DEVICE_STATE_DEFAULT);

            *param_config = (wifidirect_param_config_t *) buffer;;

            retval = OK;
        }
    }

    return retval;
}


/**
 * @brief Delete Wifi-Direct Parameters Configuration
 *
 * @param param_config  Pointer to a previously allocated buffer
 *                      with parameter configuration from
 *                      wifidirect_create_param_config()
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_delete_params_config(wifidirect_param_config_t **param_config)
{
    void * ptr;
    error_type_t retval = FAIL;

    if (param_config) {
        ptr = (void *) *param_config;
        if (ptr) {
            MEM_FREE_AND_NULL(ptr);
            *param_config = NULL;
            retval = OK;
        }
    }
    return retval;
}


/**
 * @brief Create Wifi-Direct Action Frame
 *
 * @param param_config  Pointer to an allocated buffer with 
 *                      action frame
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_action_frame(wifidirect_action_frame_t **action_frame)
{
    t_u8* buffer;
    wifidirect_action_frame_t * ptr;
    error_type_t retval = FAIL;

    if (action_frame) {
        buffer = (t_u8 *) MEM_MALLOC(sizeof(wifidirect_action_frame_t));

        if (buffer)
        {
            memset(buffer, 0, sizeof(wifidirect_action_frame_t));

            ptr = (wifidirect_action_frame_t *) buffer;

            WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->peer_mac_addr);
            WIFIDIRECT_SET_CATEGORY(ptr, WIFIDIRECT_ACTION_FRAME_CATEGORY_DEFAULT);
            WIFIDIRECT_SET_ACTION(ptr, WIFIDIRECT_ACTION_FRAME_ACTION_DEFAULT);
            {
                char array[] = WIFIDIRECT_ACTION_FRAME_OUI_DEFAULT;
                WIFIDIRECT_SET_OUI(ptr, array);
            }
            WIFIDIRECT_SET_OUI_TYPE(ptr, WIFIDIRECT_ACTION_FRAME_OUI_TYPE_DEFAULT);
            WIFIDIRECT_SET_OUI_SUBTYPE(ptr, WIFIDIRECT_ACTION_FRAME_OUI_SUBTYPE_DEFAULT);
            WIFIDIRECT_SET_DIALOG_TOKEN(ptr, WIFIDIRECT_ACTION_FRAME_DIALOG_TOKEN_DEFAULT);
            WIFIDIRECT_SET_IE_LIST_PTR(ptr, NULL);
            WIFIDIRECT_SET_IE_LIST_LENGTH(ptr, 0);

            *action_frame = (wifidirect_action_frame_t *) buffer;
            retval = OK;
        }
    }

    return retval;
}


/**
 * @brief Delete Wifi-Direct Action Frame
 *
 * @param param_config  Pointer to a previously allocated buffer
 *                      with action frame from
 *                      wifidirect_create_action_frame()
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_delete_action_frame(wifidirect_action_frame_t **action_frame)
{
    void * ptr;
    error_type_t retval = FAIL;

    if (action_frame) {
        ptr = (void *) *action_frame;
        if (ptr) {
            MEM_FREE_AND_NULL(ptr);
            *action_frame = NULL;
            retval = OK;
        }
    }
    return retval;
}


/**
 * @brief Create Wifi-Direct Discovery Request
 *
 * @param param_config  Pointer to an allocated buffer for
 *                      discovery request
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_discovery_request(wifidirect_discovery_request_t **discovery_request)
{
    t_u8* buffer;
    wifidirect_discovery_request_t * ptr;
    error_type_t retval = FAIL;

    if (discovery_request) {
        buffer = (t_u8 *) MEM_MALLOC(sizeof(wifidirect_discovery_request_t));

        if (buffer)
        {
            memset(buffer, 0, sizeof(wifidirect_discovery_request_t));

            ptr = (wifidirect_discovery_request_t *) buffer;

            WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->peer_mac_addr);
            WIFIDIRECT_SET_CATEGORY(ptr, WIFIDIRECT_DISCOVER_REQUEST_CATEGORY_DEFAULT);
            WIFIDIRECT_SET_ACTION(ptr, WIFIDIRECT_DISCOVER_REQUEST_ACTION_DEFAULT);
            {
                char array[] = WIFIDIRECT_DISCOVER_REQUEST_OUI_DEFAULT;
                WIFIDIRECT_SET_OUI(ptr, array);
            }
            WIFIDIRECT_SET_OUI_SUBTYPE(ptr, WIFIDIRECT_DISCOVER_REQUEST_OUI_SUBTYPE_DEFAULT);
            WIFIDIRECT_SET_DIALOG_TOKEN(ptr, WIFIDIRECT_DISCOVER_REQUEST_DIALOG_TOKEN_DEFAULT);
            {
                char array[] = WIFIDIRECT_DISCOVER_REQUEST_ADVERTIZE_PROTOCOL_DEFAULT;
                WIFIDIRECT_SET_ADVERTIZE_PROTOCOL(ptr, array);
            }
            {
                char array[] = WIFIDIRECT_DISCOVER_REQUEST_INFO_ID_DEFAULT;
                WIFIDIRECT_SET_INFO_ID(ptr, array);
            }
            WIFIDIRECT_SET_SERVICE_UPDATE_INDICATOR(ptr, WIFIDIRECT_DISCOVER_REQUEST_SERVICE_UPDATE_INDICATOR_DEFAULT);
            WIFIDIRECT_SET_SERVICE_PROTOCOL(ptr, WIFIDIRECT_DISCOVER_REQUEST_SERVICE_PROTOCOL_DEFAULT);
            WIFIDIRECT_SET_SERVICE_TRANSACTION_ID(ptr, WIFIDIRECT_DISCOVER_REQUEST_SERVICE_TRANSACTION_ID_DEFAULT);
            WIFIDIRECT_SET_BONJOUR_VERSION(ptr, WIFIDIRECT_DISCOVER_REQUEST_BONJOUR_VERSION_DEFAULT);
            {
                if (!wfd_discover_request_dns_ptr) {
                    wfd_discover_request_dns_ptr = (t_u8 *) MEM_MALLOC(strlen(WIFIDIRECT_DISCOVER_REQUEST_BONJOUR_DNS_DEFAULT)+1);
                    if (wfd_discover_request_dns_ptr) {
                        strcpy((char *)wfd_discover_request_dns_ptr, 
                               WIFIDIRECT_DISCOVER_REQUEST_BONJOUR_DNS_DEFAULT);
                    }
                }
            }
            WIFIDIRECT_SET_BONJOUR_DNS_PTR(ptr, wfd_discover_request_dns_ptr);
            WIFIDIRECT_SET_BONJOUR_DNS_TYPE(ptr, WIFIDIRECT_DISCOVER_REQUEST_BONJOUR_DNS_TYPE_DEFAULT);

            *discovery_request = (wifidirect_discovery_request_t *) buffer;

            retval = OK;
        }
    }

    return retval;
}


/**
 * @brief Delete Wifi-Direct Discovery Request
 *
 * @param param_config  Pointer to a previously allocated buffer
 *                      for discovery request from
 *                      wifidirect_create_discovery_request()
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_delete_discovery_request(wifidirect_discovery_request_t **discovery_request)
{
    void *ptr;
    error_type_t retval = FAIL;

    if (discovery_request) {
        ptr = (void *) *discovery_request;
        if (ptr) {
            MEM_FREE_AND_NULL(ptr);
            *discovery_request = NULL;
            retval = OK;
        }
    }
    return retval;
}


/**
 * @brief Create Wifi-Direct Discovery Response
 *
 * @param param_config  Pointer to an allocated buffer for
 *                      discovery response
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_discovery_response(wifidirect_discovery_response_t **discovery_response)
{
    t_u8* buffer;
    wifidirect_discovery_response_t * ptr;
    error_type_t retval = FAIL;

    if (discovery_response) {
        buffer = (t_u8 *) MEM_MALLOC(sizeof(wifidirect_discovery_response_t));

        if (buffer)
        {
            memset(buffer, 0, sizeof(wifidirect_discovery_response_t));

            ptr = (wifidirect_discovery_response_t *) buffer;

            WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->peer_mac_addr);
            WIFIDIRECT_SET_CATEGORY(ptr, WIFIDIRECT_DISCOVER_RESPONSE_CATEGORY_DEFAULT);
            WIFIDIRECT_SET_ACTION(ptr, WIFIDIRECT_DISCOVER_RESPONSE_ACTION_DEFAULT);
            {
                char array[] = WIFIDIRECT_DISCOVER_RESPONSE_OUI_DEFAULT;
                WIFIDIRECT_SET_OUI(ptr, array);
            }
            WIFIDIRECT_SET_OUI_SUBTYPE(ptr, WIFIDIRECT_DISCOVER_RESPONSE_OUI_SUBTYPE_DEFAULT);
            WIFIDIRECT_SET_DIALOG_TOKEN(ptr, WIFIDIRECT_DISCOVER_RESPONSE_DIALOG_TOKEN_DEFAULT);
            WIFIDIRECT_SET_STATUS_CODE(ptr, WIFIDIRECT_DISCOVER_RESPONSE_STATUS_CODE_DEFAULT);
            WIFIDIRECT_SET_GAS_REPLY(ptr, WIFIDIRECT_DISCOVER_RESPONSE_GAS_REPLY_DEFAULT);
            {
                char array[] = WIFIDIRECT_DISCOVER_RESPONSE_ADVERTIZE_PROTOCOL_DEFAULT;
                WIFIDIRECT_SET_ADVERTIZE_PROTOCOL(ptr, array);
            }
            {
                char array[] = WIFIDIRECT_DISCOVER_RESPONSE_INFO_ID_DEFAULT;
                WIFIDIRECT_SET_INFO_ID(ptr, array);
            }
            WIFIDIRECT_SET_SERVICE_UPDATE_INDICATOR(ptr, WIFIDIRECT_DISCOVER_RESPONSE_SERVICE_UPDATE_INDICATOR_DEFAULT);
            WIFIDIRECT_SET_SERVICE_PROTOCOL(ptr, WIFIDIRECT_DISCOVER_RESPONSE_SERVICE_PROTOCOL_DEFAULT);
            WIFIDIRECT_SET_SERVICE_TRANSACTION_ID(ptr, WIFIDIRECT_DISCOVER_RESPONSE_SERVICE_TRANSACTION_ID_DEFAULT);
            WIFIDIRECT_SET_DISC_STATUS_CODE(ptr, WIFIDIRECT_DISCOVER_RESPONSE_DISC_STATUS_CODE_DEFAULT);
            WIFIDIRECT_SET_BONJOUR_VERSION(ptr, WIFIDIRECT_DISCOVER_RESPONSE_BONJOUR_VERSION_DEFAULT);
            {
                if (!wfd_discover_response_dns_ptr) {
                    wfd_discover_response_dns_ptr = (t_u8 *) MEM_MALLOC(strlen(WIFIDIRECT_DISCOVER_RESPONSE_BONJOUR_DNS_DEFAULT)+1);
                    if (wfd_discover_response_dns_ptr) {
                        strcpy((char *)wfd_discover_response_dns_ptr, 
                               WIFIDIRECT_DISCOVER_RESPONSE_BONJOUR_DNS_DEFAULT);
                    }
                }
            }
            WIFIDIRECT_SET_BONJOUR_DNS_PTR(ptr, wfd_discover_response_dns_ptr);
            WIFIDIRECT_SET_BONJOUR_DNS_TYPE(ptr, WIFIDIRECT_DISCOVER_RESPONSE_BONJOUR_DNS_TYPE_DEFAULT);
            WIFIDIRECT_SET_BONJOUR_RECORD_PTR(ptr, NULL);

            *discovery_response = (wifidirect_discovery_response_t *) buffer;

            retval = OK;
        }
    }

    return retval;
}


/**
 * @brief Delete Wifi-Direct Discovery Response
 *
 * @param param_config  Pointer to a previously allocated buffer
 *                      for discovery response from
 *                      wifidirect_create_discovery_response()
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_delete_discovery_response(wifidirect_discovery_response_t **discovery_response)
{
    void * ptr;
    error_type_t retval = FAIL;

    if (discovery_response) {
        ptr = (void *) *discovery_response;
        if (ptr) {
            MEM_FREE_AND_NULL(ptr);
            *discovery_response = NULL;
            retval = OK;
        }
    }
    return retval;
}


/**
 * @brief Create Wifi-Direct Gas Comeback Request
 *
 * @param param_config  Pointer to an allocated buffer for
 *                      Gas Comeback request
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_gas_comeback_request(wifidirect_gas_comeback_request_t **gas_comeback_request)
{
    t_u8* buffer;
    wifidirect_gas_comeback_request_t * ptr;
    error_type_t retval = FAIL;

    if (gas_comeback_request) {
        buffer = (t_u8 *) MEM_MALLOC(sizeof(wifidirect_gas_comeback_request_t));

        if (buffer)
        {
            memset(buffer, 0, sizeof(wifidirect_gas_comeback_request_t));

            ptr = (wifidirect_gas_comeback_request_t *) buffer;

            WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->peer_mac_addr);
            WIFIDIRECT_SET_CATEGORY(ptr, WIFIDIRECT_GAS_COMEBACK_REQUEST_CATEGORY_DEFAULT);
            WIFIDIRECT_SET_ACTION(ptr, WIFIDIRECT_GAS_COMEBACK_REQUEST_ACTION_DEFAULT);
            WIFIDIRECT_SET_DIALOG_TOKEN(ptr, WIFIDIRECT_GAS_COMEBACK_REQUEST_DIALOG_TOKEN_DEFAULT);

            *gas_comeback_request = (wifidirect_gas_comeback_request_t *) buffer;

            retval = OK;
        }
    }

    return retval;
}


/**
 * @brief Delete Wifi-Direct Gas Comeback Request
 *
 * @param param_config  Pointer to a previously allocated buffer
 *                      for Gas Comeback request from
 *                      wifidirect_create_gas_comeback_request()
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_delete_gas_comeback_request(wifidirect_gas_comeback_request_t **gas_comeback_request)
{
    void * ptr;
    error_type_t retval = FAIL;

    if (gas_comeback_request) {
        ptr = (void *) *gas_comeback_request;
        if (ptr) {
            MEM_FREE_AND_NULL(ptr);
            *gas_comeback_request = NULL;
            retval = OK;
        }
    }
    return retval;
}


/**
 * @brief Create Wifi-Direct Gas Comeback Response
 *
 * @param param_config  Pointer to an allocated buffer for
 *                      Gas Comeback response
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_gas_comeback_response(wifidirect_gas_comeback_response_t **gas_comeback_response)
{
    t_u8* buffer;
    wifidirect_gas_comeback_response_t * ptr;
    error_type_t retval = FAIL;

    if (gas_comeback_response) {
        buffer = (t_u8 *) MEM_MALLOC(sizeof(wifidirect_gas_comeback_response_t));

        if (buffer)
        {
            memset(buffer, 0, sizeof(wifidirect_gas_comeback_response_t));

            ptr = (wifidirect_gas_comeback_response_t *) buffer;

            WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *) ptr->peer_mac_addr);
            WIFIDIRECT_SET_CATEGORY(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_CATEGORY_DEFAULT);
            WIFIDIRECT_SET_ACTION(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_ACTION_DEFAULT);
            WIFIDIRECT_SET_DIALOG_TOKEN(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_DIALOG_TOKEN_DEFAULT);
            WIFIDIRECT_SET_STATUS_CODE(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_STATUS_CODE_DEFAULT);
            WIFIDIRECT_SET_GAS_FRAGMENT_ID_REPLY(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_GAS_FRAGMENT_ID_DEFAULT);
            WIFIDIRECT_SET_GAS_REPLY(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_GAS_REPLY_DEFAULT);
            {
                char array[] = WIFIDIRECT_GAS_COMEBACK_RESPONSE_ADVERTIZE_PROTOCOL_DEFAULT;
                WIFIDIRECT_SET_ADVERTIZE_PROTOCOL(ptr, array);
            }
            {
                char array[] = WIFIDIRECT_GAS_COMEBACK_RESPONSE_INFO_ID_DEFAULT;
                WIFIDIRECT_SET_INFO_ID(ptr, array);
            }
            WIFIDIRECT_SET_RESP_STATUS_CODE(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_RESP_STATUS_CODE_DEFAULT);
            {
                char array[] = WIFIDIRECT_GAS_COMEBACK_RESPONSE_OUI_DEFAULT;
                WIFIDIRECT_SET_OUI(ptr, array);
            }
            WIFIDIRECT_SET_OUI_SUBTYPE(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_OUI_SUBTYPE_DEFAULT);
            WIFIDIRECT_SET_SERVICE_UPDATE_INDICATOR(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_SERVICE_UPDATE_INDICATOR_DEFAULT);
            WIFIDIRECT_SET_SERVICE_PROTOCOL(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_SERVICE_PROTOCOL_DEFAULT);
            WIFIDIRECT_SET_SERVICE_TRANSACTION_ID(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_SERVICE_TRANSACTION_ID_DEFAULT);
            WIFIDIRECT_SET_DISC_STATUS_CODE(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_DISC_STATUS_CODE_DEFAULT);
            WIFIDIRECT_SET_BONJOUR_VERSION(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_BONJOUR_VERSION_DEFAULT);
            {
                if (!wfd_gas_comeback_response_dns_ptr) {
                    wfd_gas_comeback_response_dns_ptr = (t_u8 *) MEM_MALLOC(strlen(WIFIDIRECT_GAS_COMEBACK_RESPONSE_BONJOUR_DNS_DEFAULT)+1);
                    if (wfd_gas_comeback_response_dns_ptr) {
                        strcpy((char *)wfd_gas_comeback_response_dns_ptr, 
                               WIFIDIRECT_GAS_COMEBACK_RESPONSE_BONJOUR_DNS_DEFAULT);
                    }
                }
            }
            WIFIDIRECT_SET_BONJOUR_DNS_PTR(ptr, wfd_gas_comeback_response_dns_ptr);
            WIFIDIRECT_SET_BONJOUR_DNS_TYPE(ptr, WIFIDIRECT_GAS_COMEBACK_RESPONSE_BONJOUR_DNS_TYPE_DEFAULT);
            WIFIDIRECT_SET_BONJOUR_RECORD_PTR(ptr, NULL);

            *gas_comeback_response = (wifidirect_gas_comeback_response_t *) buffer;

            retval = OK;
        }
    }

    return retval;
}


/**
 * @brief Delete Wifi-Direct Gas Comeback Response
 *
 * @param param_config  Pointer to a previously allocated buffer
 *                      for Gas Comeback response from
 *                      wifidirect_create_gas_comeback_response()
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_delete_gas_comeback_response(wifidirect_gas_comeback_response_t **gas_comeback_response)
{
    void * ptr;
    error_type_t retval = FAIL;

    if (gas_comeback_response) {
        ptr = (void *) *gas_comeback_response;
        if (ptr) {
            MEM_FREE_AND_NULL(ptr);
            *gas_comeback_response = NULL;
            retval = OK;
        }
    }
    return retval;
}


/** 
 *  @brief wifidirect_gas_comeback_service_discovery_request
 *
 *  @param gas_comeback_request Pointer to gas comeback request structure
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirectcmd_gas_comeback_discovery_request(wifidirect_gas_comeback_request_t *gas_comeback_request)
{
    wifidirect_gas_comeback_request *req_buf = NULL;
    int ret;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    error_type_t retval = FAIL;

    cmd_len = sizeof(wifidirect_gas_comeback_request);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        req_buf = (wifidirect_gas_comeback_request *) buffer;
        req_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_SERVICE_DISCOVERY;
        req_buf->size = cmd_len;
        req_buf->seq_num = 0;
        req_buf->result = 0;

        memcpy(req_buf->peer_mac_addr, gas_comeback_request->peer_mac_addr, ETH_ALEN);
        req_buf->category = gas_comeback_request->category;
        req_buf->action = gas_comeback_request->action;
        req_buf->dialog_token = gas_comeback_request->dialog_token;

        /* Send collective command */
        ret = wifidirect_ioctl(buffer, &cmd_len, cmd_len);

        if (ret == SUCCESS) {
            retval = OK;
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Ioctl error!\n"));
        }

        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief wifidirect_gas_comeback_service_discovery_response
 *
 *  @param gas_comeback_request Pointer to gas comeback response structure
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirectcmd_gas_comeback_discovery_response(wifidirect_gas_comeback_response_t *gas_comeback_response)
{
    wifidirect_gas_comeback_response *resp_buf = NULL;
    int i;
    int ret;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0, query_len = 0, vendor_len = 0, service_len = 0;
    t_u16 dns_len = 0, record_len = 0, upnp_len = 0;
    error_type_t retval = FAIL;

    cmd_len = sizeof(wifidirect_gas_comeback_response);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        resp_buf = (wifidirect_gas_comeback_response *) buffer;
        resp_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_SERVICE_DISCOVERY;
        resp_buf->size = cmd_len;
        resp_buf->seq_num = 0;
        resp_buf->result = 0;

        memcpy(resp_buf->peer_mac_addr, gas_comeback_response->peer_mac_addr, ETH_ALEN);
        resp_buf->category = gas_comeback_response->category;
        resp_buf->action = gas_comeback_response->action;
        resp_buf->dialog_token = gas_comeback_response->dialog_token;
        resp_buf->status_code = gas_comeback_response->status_code;
        resp_buf->gas_reply = cpu_to_le16(gas_comeback_response->gas_reply);
        resp_buf->gas_fragment_id = gas_comeback_response->gas_fragment_id;

        for (i=0; i<4; i++) {
            resp_buf->advertize_protocol_ie[i] = gas_comeback_response->advertize_protocol_ie[i];
        }

        for (i=0; i<2; i++) {
            resp_buf->info_id[i] = gas_comeback_response->info_id[i];
            query_len++;
        }

        for (i=0; i<3; i++) {
            resp_buf->oui[i] = gas_comeback_response->oui[i];
            service_len++;
            query_len++;
        }

        resp_buf->resp_status_code = gas_comeback_response->resp_status_code;
        service_len++;
        query_len++;

        resp_buf->oui_sub_type = gas_comeback_response->oui_sub_type;
        service_len++;
        query_len++;

        resp_buf->service_update_indicator =
            cpu_to_le16(gas_comeback_response->service_update_indicator);
        service_len += 2;
        query_len += 2;

        resp_buf->service_protocol = gas_comeback_response->service_protocol;
        vendor_len++;
        service_len++;
        query_len++;

        resp_buf->service_transaction_id = gas_comeback_response->service_transaction_id;
        vendor_len++;
        service_len++;
        query_len++;

        resp_buf->disc_status_code = gas_comeback_response->disc_status_code;
        vendor_len++;
        service_len++;
        query_len++;

        switch(resp_buf->service_protocol)
        {
        case 1: //Bonjour
            dns_len = strlen((char *)gas_comeback_response->disc_data.u.bonjour.dns);
            strncpy((char *) resp_buf->disc_resp.u.bonjour.dns, 
                    (char *) gas_comeback_response->disc_data.u.bonjour.dns,
                    dns_len);

            cmd_len += dns_len;
            vendor_len += dns_len;
            service_len += dns_len;
            query_len += dns_len;

            *(&resp_buf->disc_resp.u.bonjour.dns_type + dns_len) =
                gas_comeback_response->disc_data.u.bonjour.dns_type;
            vendor_len++;
            service_len++;
            query_len++;

            *(&resp_buf->disc_resp.u.bonjour.version + dns_len) =
                gas_comeback_response->disc_data.u.bonjour.version;
            vendor_len++;
            service_len++;
            query_len++;

            if (gas_comeback_response->disc_data.u.bonjour.record) {
                record_len = strlen((char *)gas_comeback_response->disc_data.u.bonjour.record);
                if (record_len) {
                    strncpy((char *) resp_buf->disc_resp.u.bonjour.record + dns_len,
                            (char *) gas_comeback_response->disc_data.u.bonjour.record, 
                            record_len);
                    cmd_len += record_len;
                    vendor_len += record_len;
                    service_len += record_len;
                    query_len += record_len;
                }
            }
            break;
        case 2: // uPnP
            /*
            * For uPnP, due to union allocation, a extra byte
            * is allocated reduce it here for uPnP
            */
            cmd_len--;

            resp_buf->disc_resp.u.upnp.version = gas_comeback_response->disc_data.u.upnp.version;
            vendor_len++;
            service_len++;
            query_len++;

            upnp_len = strlen((char *)gas_comeback_response->disc_data.u.upnp.value);
            strncpy((char *) resp_buf->disc_resp.u.upnp.value, 
                    (char *) gas_comeback_response->disc_data.u.upnp.value,
                    upnp_len);

            cmd_len += upnp_len;
            vendor_len += upnp_len;
            service_len += upnp_len;
            query_len += upnp_len;

            break;
        default:
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Unknown or unsupport service protocol!\n"));
            break;
        }

        query_len += 2;
        service_len += 2;
        query_len += 2;

        resp_buf->query_len = cpu_to_le16(query_len);
        resp_buf->response_len = cpu_to_le16(service_len);
        resp_buf->vendor_len = cpu_to_le16(vendor_len);

        /* Send collective command */
        ret = wifidirect_ioctl(buffer, &cmd_len, cmd_len);
        if (ret == SUCCESS) {
            retval = OK;
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Ioctl error!\n"));
        }

        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief wifidirect_service_discovery request
 *
 *  @param discovery_request    Pointer to discovery request
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirectcmd_service_discovery_request(wifidirect_discovery_request_t *discovery_request)
{
    wifidirect_discovery_request *req_buf = NULL;
    int i;
    int ret;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0, query_len = 0, vendor_len = 0, service_len = 0;
    t_u16 dns_len = 0, upnp_len = 0;
    error_type_t retval = FAIL;

    cmd_len = sizeof(wifidirect_discovery_request);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        req_buf = (wifidirect_discovery_request *) buffer;
        req_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_SERVICE_DISCOVERY;
        req_buf->size = cmd_len;
        req_buf->seq_num = 0;
        req_buf->result = 0;

        memcpy(req_buf->peer_mac_addr, discovery_request->peer_mac_addr, ETH_ALEN);
        req_buf->category = discovery_request->category; 
        req_buf->action = discovery_request->action;
        req_buf->dialog_token = discovery_request->dialog_token;

        for (i = 0; i < 4; i++) {
            req_buf->advertize_protocol_ie[i] =  discovery_request->advertize_protocol_ie[i];
        }

        for (i = 0; i < 2; i++){
            req_buf->info_id[i] = discovery_request->info_id[i];
            query_len++;
        }

        for (i = 0; i < 3; i++){
            req_buf->oui[i] = discovery_request->oui[i];
            query_len++;
            service_len++;
        }
            
        req_buf->oui_sub_type = discovery_request->oui_sub_type;
        service_len++;
        query_len++;

        req_buf->service_update_indicator = 
            cpu_to_le16(discovery_request->service_update_indicator);
        service_len += 2;
        query_len += 2;

        req_buf->service_transaction_id = discovery_request->service_transaction_id;

        vendor_len++;
        service_len++;
        query_len++;

        req_buf->service_protocol = discovery_request->service_protocol;
        vendor_len++;
        service_len++;
        query_len++;

        switch(req_buf->service_protocol)
        {
        case 1: //Bonjour
            dns_len = strlen((char *)discovery_request->disc_data.u.bonjour.dns);
            strncpy((char *) req_buf->disc_query.u.bonjour.dns, 
                    (char *) discovery_request->disc_data.u.bonjour.dns,
                    dns_len);

            cmd_len += dns_len;
            vendor_len += dns_len;
            service_len += dns_len;
            query_len += dns_len;

            *(&req_buf->disc_query.u.bonjour.dns_type + dns_len) =
                discovery_request->disc_data.u.bonjour.dns_type;
            vendor_len++;
            service_len++;
            query_len++;

            *(&req_buf->disc_query.u.bonjour.version + dns_len) =
                discovery_request->disc_data.u.bonjour.version;
            vendor_len++;
            service_len++;
            query_len++;

            break;
        case 2: // uPnP
            /*
            * For uPnP, due to union allocation, a extra byte
            * is allocated reduce it here for uPnP
            */
            cmd_len--;

            req_buf->disc_query.u.upnp.version = discovery_request->disc_data.u.upnp.version;
            vendor_len++;
            service_len++;
            query_len++;

            upnp_len = strlen((char *)discovery_request->disc_data.u.upnp.value);
            strncpy((char *) req_buf->disc_query.u.upnp.value, 
                    (char *) discovery_request->disc_data.u.upnp.value,
                    upnp_len);

            cmd_len += upnp_len;
            vendor_len += upnp_len;
            service_len += upnp_len;
            query_len += upnp_len;
            break;
        default:
            break;
        }

        query_len += 2;
        service_len += 2;
        query_len += 2;

        req_buf->query_len = cpu_to_le16(query_len);
        req_buf->request_len = cpu_to_le16(service_len);
        req_buf->vendor_len = cpu_to_le16(vendor_len);

        /* Send collective command */
        ret = wifidirect_ioctl(buffer, &cmd_len, cmd_len);
        if (ret == SUCCESS) {
            retval = OK;
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Ioctl error!\n"));
        }

        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief wifidirect_service_discovery response
 *
 *  @param discovery_response   Pointer to discovery response
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirectcmd_service_discovery_response(wifidirect_discovery_response_t *discovery_response)
{
    wifidirect_discovery_response *resp_buf = NULL;
    int i;
    int ret;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0, query_len = 0, vendor_len = 0, service_len = 0;
    t_u16 dns_len = 0, record_len = 0, upnp_len = 0;
    error_type_t retval = FAIL;

    cmd_len = sizeof(wifidirect_discovery_response);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        resp_buf = (wifidirect_discovery_response *) buffer;
        resp_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_SERVICE_DISCOVERY;
        resp_buf->size = cmd_len;
        resp_buf->seq_num = 0;
        resp_buf->result = 0;

        memcpy(resp_buf->peer_mac_addr, discovery_response->peer_mac_addr, ETH_ALEN);
        resp_buf->category = discovery_response->category;
        resp_buf->action = discovery_response->action;
        resp_buf->dialog_token = discovery_response->dialog_token;
        resp_buf->status_code = discovery_response->status_code;
        resp_buf->gas_reply = cpu_to_le16(discovery_response->gas_reply);

        for (i=0; i<4; i++) {
            resp_buf->advertize_protocol_ie[i] = discovery_response->advertize_protocol_ie[i];
        }

        for (i=0; i<2; i++) {
            resp_buf->info_id[i] = discovery_response->info_id[i];
            query_len++;
        }

        for (i=0; i<3; i++) {
            resp_buf->oui[i] = discovery_response->oui[i];
            service_len++;
            query_len++;
        }

        resp_buf->oui_sub_type = discovery_response->oui_sub_type;
        service_len++;
        query_len++;

        resp_buf->service_update_indicator =
             cpu_to_le16(discovery_response->service_update_indicator);
        service_len += 2;
        query_len += 2;

        resp_buf->service_protocol = discovery_response->service_protocol;
        vendor_len++;
        service_len++;
        query_len++;

        resp_buf->service_transaction_id = discovery_response->service_transaction_id;
        vendor_len++;
        service_len++;
        query_len++;

        resp_buf->disc_status_code = discovery_response->disc_status_code;
        vendor_len++;
        service_len++;
        query_len++;

        switch(resp_buf->service_protocol)
        {
        case 1: //Bonjour
            dns_len = strlen((char *)discovery_response->disc_data.u.bonjour.dns);
            strncpy((char *) resp_buf->disc_resp.u.bonjour.dns, 
                    (char *) discovery_response->disc_data.u.bonjour.dns,
                    dns_len);

            cmd_len += dns_len;
            vendor_len += dns_len;
            service_len += dns_len;
            query_len += dns_len;

            *(&resp_buf->disc_resp.u.bonjour.dns_type + dns_len) =
                discovery_response->disc_data.u.bonjour.dns_type;
            vendor_len++;
            service_len++;
            query_len++;

            *(&resp_buf->disc_resp.u.bonjour.version + dns_len) =
                discovery_response->disc_data.u.bonjour.version;
            vendor_len++;
            service_len++;
            query_len++;

            if (discovery_response->disc_data.u.bonjour.record) {
                record_len = strlen((char *)discovery_response->disc_data.u.bonjour.record);
                if (record_len) {
                    strncpy((char *) resp_buf->disc_resp.u.bonjour.record + dns_len,
                            (char *) discovery_response->disc_data.u.bonjour.record, 
                            record_len);
                    cmd_len += record_len;
                    vendor_len += record_len;
                    service_len += record_len;
                    query_len += record_len;
                }
            }
            break;
        case 2: // uPnP
            /*
            * For uPnP, due to union allocation, a extra byte
            * is allocated reduce it here for uPnP
            */
            cmd_len--;

            resp_buf->disc_resp.u.upnp.version = discovery_response->disc_data.u.upnp.version;
            vendor_len++;
            service_len++;
            query_len++;

            upnp_len = strlen((char *)discovery_response->disc_data.u.upnp.value);
            strncpy((char *) resp_buf->disc_resp.u.upnp.value, 
                    (char *) discovery_response->disc_data.u.upnp.value,
                    upnp_len);

            cmd_len += upnp_len;
            vendor_len += upnp_len;
            service_len += upnp_len;
            query_len += upnp_len;

            break;
        default:
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Unknown or unsupport service protocol!\n"));
            break;
        }

        query_len += 2;
        service_len += 2;
        query_len += 2;

        resp_buf->query_len = cpu_to_le16(query_len);
        resp_buf->response_len = cpu_to_le16(service_len);
        resp_buf->vendor_len = cpu_to_le16(vendor_len);

        /* Send collective command */
        ret = wifidirect_ioctl(buffer, &cmd_len, cmd_len);

        if (ret == SUCCESS) {
            retval = OK;
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Ioctl error!\n"));
        }

        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief wifidirect_config request
 *
 *  @param config   Pointer to configuration structure
 *  @param ie_index IE Index
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirectcmd_config(wifidirect_config_t *config, t_u16 ie_index)
{
    t_u8 *buf = NULL;
    t_u16 ie_len_wifidirect = 0, ie_len_wps = 0, ie_len;
    int ret = SUCCESS;
    error_type_t retval = FAIL;

    if (config) {
        buf = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
        if (buf) {
            memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

            /* Read parameters and send command to firmware */
            ret = wifidirect_file_params_config(config, buf + sizeof(tlvbuf_custom_ie) +
                                          sizeof(custom_ie), &ie_len_wifidirect,
                                          &ie_len_wps);
            if (ret == SUCCESS) {
                if (ie_len_wifidirect > MAX_SIZE_IE_BUFFER ||
                    ie_len_wps > MAX_SIZE_IE_BUFFER) {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:IE parameter size exceeds limit.\n"));
                    MEM_FREE_AND_NULL(buf);
                    return retval;
                }
                ie_len =
                    ie_len_wifidirect + ie_len_wps + sizeof(tlvbuf_custom_ie) +
                    (2 * sizeof(custom_ie));
                if (ie_len >= MRVDRV_SIZE_OF_CMD_BUFFER) {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Too much data in configuration.\n"));
                    MEM_FREE_AND_NULL(buf);
                    return retval;
                }

                ret = wifidirect_ie_config(&ie_index, ie_len_wifidirect, ie_len_wps, buf);

                if (ret == SUCCESS) {
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Could not set wifidirect config\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Could not build wifidirect config\n"));
            }
            MEM_FREE_AND_NULL(buf);
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
        }
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:wifidirect config NULL!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect_params_config
 *
 *  @param  param_config    Pointer to parameter configuration
 *                          struct
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirectcmd_params_config(wifidirect_param_config_t *param_config)
{
    wifidirect_params_config *param_buf = NULL;
    tlvbuf_wps_ie *new_tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 tlv_len = 0, cmd_len = 0, temp_u16;
    int ret = 0;
    error_type_t retval = FAIL;
    
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
        cmd_len = sizeof(wifidirect_params_config);
        param_buf = (wifidirect_params_config *) buffer;
        param_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        param_buf->seq_num = 0;
        param_buf->result = 0;

        param_buf->action = cpu_to_le16(ACTION_SET);

        // Enable Scan
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(t_u8);
        new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
        cmd_len += tlv_len;
        /* Set TLV fields */
        new_tlv->tag = MRVL_WIFIDIRECT_SCAN_ENABLE_TLV_ID;
        new_tlv->length = tlv_len - 2 * sizeof(t_u16);
        memcpy(new_tlv->data, &param_config->enable_scan, sizeof(t_u8));
        endian_convert_tlv_header_out(new_tlv);

#if 0
        if ((memcmp(????, ????NULL_MAC, ETH_ALEN) == 0) {
            // Scan Peer Device ID
            /* Append a new TLV */
            tlv_len = sizeof(tlvbuf_wps_ie) + ETH_ALEN;
            new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
            cmd_len += tlv_len;
            /* Set TLV fields */
            new_tlv->tag = MRVL_WIFIDIRECT_PEER_DEVICE_TLV_ID;
            new_tlv->length = tlv_len - 2 * sizeof(t_u16);
            memcpy(new_tlv->data, ????, ETH_ALEN);
            endian_convert_tlv_header_out(new_tlv);

            // Scan Request Device Type
            /* Append a new TLV */
            tlv_len = sizeof(tlvbuf_wps_ie) + WPS_DEVICE_TYPE_MAX_LEN;
            new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
            cmd_len += tlv_len;
            /* Set TLV fields */
            new_tlv->tag = MRVL_WIFIDIRECT_SCAN_REQ_DEVICE_TLV_ID;
            new_tlv->length = tlv_len - 2 * sizeof(t_u16);
            memcpy(&new_tlv->data, ????,
                   WPS_DEVICE_TYPE_MAX_LEN);
            endian_convert_tlv_header_out(new_tlv);
        }
#endif
        // Minimum and Maximum Discovery Intervals
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wps_ie) + 2 * sizeof(t_u16);
        new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
        cmd_len += tlv_len;
        /* Set TLV fields */
        new_tlv->tag = MRVL_WIFIDIRECT_DISC_PERIOD_TLV_ID;
        new_tlv->length = tlv_len - 2 * sizeof(t_u16);
        temp_u16 = cpu_to_le16(param_config->discovery_intervals.min_disc_interval);
        memcpy(&new_tlv->data, &temp_u16, sizeof(t_u16));
        temp_u16 = cpu_to_le16(param_config->discovery_intervals.max_disc_interval);
        memcpy((((t_u8 *) & new_tlv->data) + sizeof(t_u16)), &temp_u16, sizeof(t_u16));
        endian_convert_tlv_header_out(new_tlv);

        //Device State
        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(t_u16);
        new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
        cmd_len += tlv_len;
        /* Set TLV fields */
        new_tlv->tag = MRVL_WIFIDIRECT_DEVICE_STATE_TLV_ID;
        new_tlv->length = tlv_len - 2 * sizeof(t_u16);
        temp_u16 = cpu_to_le16(param_config->device_state);
        memcpy(new_tlv->data, &temp_u16, sizeof(t_u16));
        endian_convert_tlv_header_out(new_tlv);

        param_buf->size = cmd_len;

        /* Send collective command */
        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process response */
        if (ret == SUCCESS) {
            /* Verify response */
            if (param_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) {
                if (param_buf->result == CMD_SUCCESS) {
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Could not set wifidirect parameters!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief wifidirect_action_frame request
 *
 *  @param action_frame     Pointer to Action Frame Struct
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirectcmd_action_frame(wifidirect_action_frame_t * action_frame)
{
    wifidirect_action_frame *action_buf = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len = sizeof(wifidirect_action_frame);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
        action_buf = (wifidirect_action_frame *) buffer;
        action_buf->cmd_code = HostCmd_CMD_802_11_ACTION_FRAME;
        action_buf->seq_num = 0;
        action_buf->result = 0;

        memcpy(action_buf->peer_mac_addr, action_frame->peer_mac_addr, ETH_ALEN);

        action_buf->category = action_frame->category;
        action_buf->action = action_frame->action;

        memcpy(action_buf->oui, action_frame->oui, 3);

        action_buf->oui_type = action_frame->oui_type;
        action_buf->oui_sub_type = action_frame->oui_sub_type;
        action_buf->dialog_token = action_frame->dialog_token;

        action_buf->size = sizeof(wifidirect_action_frame);

        if ((action_frame->ie_list_length > 0) && (action_frame->ie_list != NULL) && 
            (action_frame->ie_list_length + cmd_len <= MRVDRV_SIZE_OF_CMD_BUFFER))  {
            memcpy(action_buf->ie_list, action_frame->ie_list, action_frame->ie_list_length);
            action_buf->size += action_frame->ie_list_length;
        }

        /* Send collective command */
        ret = wifidirect_ioctl(buffer, &cmd_len, cmd_len);

        if (ret == SUCCESS) {
            DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Action Frame successful\n"));
            retval = OK;
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Error executing Action Frame command\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect mode status (start or stop request)
 *
 *
 *   STATUS Modes:         0 - start wifidirect status
 *                         1 - stop  wifidirect status
 *                         2 - start wifidirect group owner mode
 *                         3 - start wifidirect client mode
 *                         4 - start wifidirect find phase
 *                         5 - stop  wifidirect find phase
 *
 *  @param status   Wifi-Direct mode status command
 *  @return         OK or FAIL
 */
error_type_t
wifidirectcmd_set_status(t_u16 status)
{
    int ret;
    t_u16 cmd_len = 0;
    t_u8 *buffer = NULL;
    wifidirect_mode_config *cmd_buf = NULL;
    error_type_t retval = FAIL;

    /* send hostcmd now */
    cmd_len = sizeof(wifidirect_mode_config);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        memset(buffer, 0x0, MRVDRV_SIZE_OF_CMD_BUFFER);
        cmd_buf = (wifidirect_mode_config *) buffer;
        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_MODE_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;
        cmd_buf->action = cpu_to_le16(ACTION_SET);
        cmd_buf->mode = cpu_to_le16(status);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        if (ret == SUCCESS) {
            DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Set wifidirect status %d successful\n", status));
            retval = OK;
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Error executing wifidirect set mode command\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect mode status (start or stop requests)
 *
 *
 *   STATUS Modes:         0 - stop wifidirect status
 *                         1 - start  wifidirect status (listen)
 *                         2 - start wifidirect group owner mode
 *                         3 - start wifidirect client mode
 *                         4 - start wifidirect find phase
 *                         5 - stop wifidirect find phase
 *
 *  @param status   Wifi-Direct mode status command pointer
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirectcmd_get_status(t_u16 *status)
{
    int ret;
    t_u16 data;
    t_u16 cmd_len = 0;
    t_u8 *buffer = NULL;
    wifidirect_mode_config *cmd_buf = NULL;
    error_type_t retval = FAIL;

    /* send hostcmd now */
    cmd_len = sizeof(wifidirect_mode_config);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        cmd_buf = (wifidirect_mode_config *) buffer;
        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_MODE_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;
        cmd_buf->action = ACTION_GET;
        cmd_buf->action = cpu_to_le16(cmd_buf->action);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process result */
        if (ret == SUCCESS) {
            data = le16_to_cpu(cmd_buf->mode);
            switch (data) {
            case 0:
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("wifidirect status = stopped\n"));
                break;
            case 1:
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("wifidirect status = started\n"));
                break;
            case 2:
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("wifidirect status = wifidirect group owner\n"));
                break;
            case 3:
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("wifidirect status = wifidirect client\n"));
                break;
            case 4:
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("wifidirect status = Find phase started\n"));
                break;
            case 5:
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("wifidirect status = Find phase stopped\n"));
                break;
            default:
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("wifidirect status = %d\n", data));
                break;
            }

            if (status)
                *status = data;

            retval = OK;
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Error executing wifidirect get mode command\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}



/** 
 *  @brief Set wifidirect discovery period
 *
 *  @param min_disc_interval     Minimum Discovery Interval
 *  @param max_disc_interval     Maximum Discovery Interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_discovery_period(t_u16 min_disc_interval, t_u16 max_disc_interval)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_discovery_period *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_discovery_period);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_discovery_period *) (buffer +
                                                    sizeof
                                                    (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_DISC_PERIOD_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_discovery_period) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->min_disc_interval = cpu_to_le16(min_disc_interval);
        tlv->max_disc_interval = cpu_to_le16(max_disc_interval);

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) 
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Discovery period set successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Couldn't set min and max discovery period!\n"));
                }
            } else {
                DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect discovery period
 *
 *  @param min_disc_interval     Pointer to Minimum Discovery Interval
 *  @param max_disc_interval     Pointer to Maximum Discovery Interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_discovery_period(t_u16 *min_disc_interval, t_u16 *max_disc_interval)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_discovery_period *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_discovery_period);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_discovery_period *) (buffer +
                                                    sizeof
                                                    (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_DISC_PERIOD_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_discovery_period) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) 
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    *min_disc_interval = le16_to_cpu(tlv->min_disc_interval);
                    *max_disc_interval = le16_to_cpu(tlv->max_disc_interval);
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Discovery period get successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Couldn't get min and max discovery period!\n"));
                }
            } else {
                DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect Intent
 *
 *  @param intent   Wifi-Direct Intent
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_intent(t_u8 intent)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_intent *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) + sizeof(tlvbuf_wifidirect_intent);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_intent *) (buffer +
                                          sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_INTENT_TLV_ID;
        tlv->length = sizeof(tlvbuf_wifidirect_intent) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->intent = intent;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Intent value set successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't set Intent value!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect Intent
 *
 *  @param intent   Pointer Wifi-Direct Intent
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_intent(t_u8 *intent)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_intent *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) + sizeof(tlvbuf_wifidirect_intent);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_intent *) (buffer +
                                          sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_INTENT_TLV_ID;
        tlv->length = sizeof(tlvbuf_wifidirect_intent) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    *intent = tlv->intent;
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Intent Value = %d\n", (int) tlv->intent));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't get Intent value!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect ListenChannel
 *
 *  @param listen_channel   Wifi-Direct Listen Channel
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_set_cfg_cmd_listen_channel(t_u8 listen_channel)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_listen_channel *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_listen_channel);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) 
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_listen_channel *) (buffer +
                                                  sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_LISTEN_CHANNEL_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_listen_channel) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->listen_channel = listen_channel;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Listen Channel set successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't set Listen Channel!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect ListenChannel
 *
 *  @param listen_channel   Pointer to Wifi-Direct Listen Channel
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_get_cfg_cmd_listen_channel(t_u8 *listen_channel)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_listen_channel *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_listen_channel);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) 
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_listen_channel *) (buffer +
                                                  sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_LISTEN_CHANNEL_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_listen_channel) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    *listen_channel = tlv->listen_channel;
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Listen Channel = %d\n", (int) tlv->listen_channel));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't get Listen Channel!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect OperatingChannel
 *
 *  @param operating_channel    Wifi-Direct Operating Channel
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_set_cfg_cmd_op_channel(t_u8 operating_channel)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_operating_channel *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_operating_channel);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_operating_channel *) (buffer +
                                                     sizeof
                                                     (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_OPERATING_CHANNEL_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_operating_channel) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->operating_channel = operating_channel;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {

            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) 
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Operating Channel set successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't set Operating Channel!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect OperatingChannel
 *
 *  @param operating_channel    Pointer to Wifi-Direct Operating Channel
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_get_cfg_cmd_op_channel(t_u8 *operating_channel)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_operating_channel *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_operating_channel);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_operating_channel *) (buffer +
                                                     sizeof
                                                     (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_OPERATING_CHANNEL_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_operating_channel) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {

            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) 
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    *operating_channel = tlv->operating_channel;
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,
                            ("Operating Channel = %d\n", (int) tlv->operating_channel));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't get Operating Channel!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}






#if 0

/**
 *  @brief Show usage information for the wifidirect Invitation List command
 *
 *  @return         N/A
 */
static void
print_wifidirect_cfg_invitation_list_usage(void)
{
    DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("\nUsage : wifidirect_cfg_invitation_list [mac_addr]");
    DPRINTF((DBG_LOUD | DBG_OUTPUT) ,
        ("\nIf mac_addr parameter is provided, 'set' is performed otherwise 'get' is performed.\n");
    return;
}


/** 
 *  @brief Set/get wifidirect InvitationList
 *
 *  Usage: "Usage : wifidirect_cfg_invitation_list [mac_addr]"
 *
 *  @param argc     Number of arguments
 *  @param argv     Pointer to the arguments
 *  @return         None 
 */
static void
wifidirect_cfg_cmd_invitation_list(int argc, char *argv[])
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_invitation_list *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int opt, ret = 0, ctr = 1;

    while ((opt = getopt_long(argc, argv, "+", cmd_options, NULL)) != -1) {
        switch (opt) {
        default:
            print_wifidirect_cfg_invitation_list_usage();
            return;
        }
    }
    argc -= optind;
    argv += optind;

    /* Check arguments */
    if ((argc < 2) || (argc > 3)) {
        DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Wrong number of arguments!\n");
        print_wifidirect_cfg_invitation_list_usage();
        return;
    }

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_invitation_list);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (!buffer) {
        DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n");
        goto done;
    }

    memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    cmd_buf = (wifidirect_params_config *) buffer;
    tlv =
        (tlvbuf_wifidirect_invitation_list *) (buffer +
                                               sizeof
                                               (wifidirect_params_config));

    cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
    cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
    cmd_buf->seq_num = 0;
    cmd_buf->result = 0;

    tlv->tag = MRVL_WIFIDIRECT_INVITATION_LIST_TLV_ID;
    tlv->length =
        sizeof(tlvbuf_wifidirect_invitation_list) - MRVL_TLV_HEADER_SIZE;

    if (argc == 2) {
        cmd_buf->action = ACTION_GET;
        cmd_len += (WIFIDIRECT_INVITATION_LIST_MAX - 1) * ETH_ALEN;
    } else {
        cmd_buf->action = cpu_to_le16(ACTION_SET);
        if ((ret = mac2raw(argv[2], tlv->inv_peer_addr)) != SUCCESS) {
            DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Invalid MAC address %s\n", argv[2]);
            goto done;
        }
    }
    endian_convert_tlv_header_out(tlv);
#ifdef DEBUG
    hexdump(buffer, cmd_len, ' ');
#endif
    ret =
        wifidirect_ioctl((t_u8 *) buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);
    endian_convert_tlv_header_in(tlv);
#ifdef DEBUG
    hexdump(buffer, cmd_len + BUF_HEADER_SIZE, ' ');
#endif
    /* Process Response */
    if (ret == SUCCESS) {
        /* Verify response */
        if (cmd_buf->cmd_code !=
            (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) {
            DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Corrupted response!\n");
            goto done;
        }
        /* Print response */
        if (cmd_buf->result == CMD_SUCCESS) {
            if (argc == 2) {
                /* reusing variable cmd_len */
                cmd_len = tlv->length;
                if (cmd_len > 0) {
                    DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("Invitation List =>");
                    while (cmd_len >= ETH_ALEN) {
                        if (memcmp
                            (tlv->inv_peer_addr, "\x00\x00\x00\x00\x00\x00",
                             ETH_ALEN)) {
                            DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("\n\t [%d] ", ctr++);
                            print_mac(tlv->inv_peer_addr);
                        }
                        cmd_len -= ETH_ALEN;
                        tlv =
                            (tlvbuf_wifidirect_invitation_list
                             *) (((t_u8 *) (tlv)) + ETH_ALEN);
                    }
                    if (ctr == 1)
                        DPRINTF((DBG_LOUD | DBG_OUTPUT) ,(" empty.");
                } else
                    DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("Invitation List is empty.\n");
                DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("\n");
            } else {
                DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("Invitation List setting successful!\n");
            }
        } else {
            if (argc == 2) {
                DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Couldn't get Invitation List!\n");
            } else {
                DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Couldn't set Invitation List!\n");
            }
        }
    } else {
        DPRINTF((DBG_LOUD | DBG_OUTPUT) ,("ERR:Command sending failed!\n");
    }
  done:
    if (buffer)
        MEM_FREE_AND_NULL(buffer);
    return;
}


/** 
 *  @brief Set wifidirect InvitationList
 *
 *  @param inv_peer_addr    Pointer to Invitation Address to add
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_invitation_list(char *inv_peer_addr)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_invitation_list *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0, ctr = 1;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_invitation_list);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) 
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_invitation_list *) (buffer +
                                                   sizeof
                                                   (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_INVITATION_LIST_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_invitation_list) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);

???? what format is the inv_peer_addr????
        memcpy(tlv->inv_peer_addr, inv_peer_addr, ETH_ALEN);
        
        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl((t_u8 *) buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS)
        {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS)
                {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Invitation List setting successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't set Invitation List!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect InvitationList
 *
 *  @param inv_list         Pointer to Invitation List
 *  @param inv_list_length  Pointer to Invitation List Length
 * 
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_invitation_list(char *inv_list, t_u8 *inv_list_length)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_invitation_list *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0, ctr = 1;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_invitation_list);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) 
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_invitation_list *) (buffer +
                                                   sizeof
                                                   (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_INVITATION_LIST_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_invitation_list) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;
        cmd_len += (WIFIDIRECT_INVITATION_LIST_MAX - 1) * ETH_ALEN;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl((t_u8 *) buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS)
        {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    /* reusing variable cmd_len */
                    cmd_len = tlv->length;
                    inv_list_length = cmd_len;
???? What format is the list????
                    if (cmd_len > 0) {
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Invitation List =>"));
                        while (cmd_len >= ETH_ALEN) {
                            if (memcmp
                                (tlv->inv_peer_addr, "\x00\x00\x00\x00\x00\x00",
                                 ETH_ALEN)) {
                                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n\t [%d] ", ctr++));
                                print_mac(tlv->inv_peer_addr);
                            }
                            cmd_len -= ETH_ALEN;
                            tlv =
                                (tlvbuf_wifidirect_invitation_list
                                 *) (((t_u8 *) (tlv)) + ETH_ALEN);
                        }
                        if (ctr == 1)
                            DPRINTF((DBG_SOFT | DBG_OUTPUT) ,(" empty."));
                    } else {
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Invitation List is empty.\n"));
                    }
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n");
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't get Invitation List!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}
#endif

/** 
 *  @brief Set wifidirect NoA parameters
 *
 *  @param enable_noa   NoA Enable/Disable boolean
 *  @param index        NoA index
 *  @param counttype    NoA count type
 *  @param duration     NoA duration
 *  @param interval     NoA interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_noa(bool enable_noa, 
                           t_u8 noa_index,
                           t_u8 count_type,
                           t_u32 duration,
                           t_u32 interval)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_noa_config *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) + sizeof(tlvbuf_wifidirect_noa_config);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_noa_config *) (buffer +
                                              sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_NOA_TLV_ID;
        tlv->length = sizeof(tlvbuf_wifidirect_noa_config) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        if (enable_noa) 
        {
            /* enable case */
            tlv->enable_noa = 1;
            tlv->enable_noa = cpu_to_le16(tlv->enable_noa);
        }
        tlv->noa_index = noa_index;

        if (tlv->enable_noa) {
            tlv->count_type = count_type;
            tlv->duration = cpu_to_le32(duration);
            tlv->interval = cpu_to_le32(interval);
        }

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS)
        {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("NoA setting successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't set NoA parameters!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect NoA parameters for specified NoA Index
 *
 *  @param enable_noa   Pointer to NoA Enable/Disable boolean
 *  @param index        NoA index
 *  @param counttype    Pointer to NoA count type
 *  @param duration     Pointer to NoA duration
 *  @param interval     Pointer to NoA interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_noa(bool *enable_noa, 
                           t_u8 noa_index,
                           t_u8 *count_type,
                           t_u32 *duration,
                           t_u32 *interval)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_noa_config *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) + sizeof(tlvbuf_wifidirect_noa_config);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_noa_config *) (buffer +
                                              sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_NOA_TLV_ID;
        tlv->length = sizeof(tlvbuf_wifidirect_noa_config) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;
        cmd_len += (sizeof(tlvbuf_wifidirect_noa_config) - MRVL_TLV_HEADER_SIZE)
                * (WIFIDIRECT_NOA_DESC_MAX - 1);

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS)
        {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS)
                {
                    if (tlv->length){
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("NoA settings:\n"));
                    } else {
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("NoA parameters are not configured.\n"));
                    }

                    while (tlv->length >
                           sizeof(tlvbuf_wifidirect_noa_config) -
                           MRVL_TLV_HEADER_SIZE) {
                        tlv->enable_noa = le16_to_cpu(tlv->enable_noa);
                        if (tlv->noa_index == noa_index)
                        {
                            if (tlv->enable_noa)
                            {
                                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("[%d] NoA is enabled!\n", (int) tlv->noa_index));
                                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("CountType = %d\n", (int) tlv->count_type));
                                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Duration = %ldms\n",
                                       le32_to_cpu(tlv->duration)));
                                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Interval = %ldms\n",
                                       le32_to_cpu(tlv->interval)));

                                if (enable_noa)
                                    *enable_noa = true;
                                if (count_type)
                                    *count_type = tlv->count_type;
                                if (duration)
                                    *duration = le32_to_cpu(tlv->duration);
                                if (interval)
                                    *interval = le32_to_cpu(tlv->interval);
                            } else {
                                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("[%d] NoA is disabled!\n", (int) tlv->noa_index));

                                if (enable_noa)
                                    *enable_noa = false;
                            }
                        }
                    }
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't get NoA parameters!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect Opportunistic Power Save
 *
 *  @param enable_ps    Enable/Disable boolean
 *  @param ctwindow_val ctwindow
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_opp_ps(bool enable_opp_ps, t_u8 ctwindow_val)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_opp_ps_config *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    t_u8 opp_ps = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_opp_ps_config);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_opp_ps_config *) (buffer +
                                                 sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_OPP_PS_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_opp_ps_config) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        if (enable_opp_ps)
        {
            opp_ps = 1;
            tlv->ctwindow_opp_ps = (t_u8) ctwindow_val | SET_OPP_PS(opp_ps);;
        }
        else
        {
            tlv->ctwindow_opp_ps = (t_u8) SET_OPP_PS(opp_ps);;
        }

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS)
        {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Opportunistic power save setting successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,
                            ("ERR:Couldn't set Opportunistic power save and CTWindow!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }
    return retval;
}


/** 
 *  @brief Get wifidirect Opportunistic Power Save
 *
 *  @param enable_ps    Pointer to Enable/Disable boolean
 *  @param ctwindow_val Pointer to ctwindow
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_opp_ps(bool *enable_opp_ps, t_u8 *ctwindow_val)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_opp_ps_config *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    t_u8 opp_ps = 0, ctwindow = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_opp_ps_config);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_opp_ps_config *) (buffer +
                                                 sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_OPP_PS_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_opp_ps_config) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS)
        {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS) {
                    opp_ps = GET_OPP_PS(tlv->ctwindow_opp_ps);
                    if (opp_ps) 
                    {
                        ctwindow = (tlv->ctwindow_opp_ps) & CT_WINDOW_MASK;
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Opportunistic Power Save enabled!\n"));
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("CTWindow  = %d\n", ctwindow));

                        if (enable_opp_ps)
                            *enable_opp_ps = true;
                        if (ctwindow_val)
                            *ctwindow_val = ctwindow;
                    } else {
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Opportunistic Power Save disabled!\n"));
                        if (enable_opp_ps)
                            *enable_opp_ps = false;
                    }
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,
                        ("ERR:Couldn't get Opportunistic power save and CTWindow!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect Capability
 *  
 *  @param dev_capability       Dev Capability
 *  @param group_capability     Group Capability
 *  
 *  @return         None 
 */
error_type_t
wifidirect_set_cfg_cmd_capability(t_u8 dev_capability, t_u8 group_capability)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_capability_config *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_capability_config);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_capability_config *) (buffer +
                                                     sizeof
                                                     (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_CAPABILITY_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_capability_config) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->dev_capability = dev_capability;
        tlv->group_capability = group_capability;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS)
        {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) 
            { 
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Capability set successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("ERR:Couldn't set Capability!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect Capability
 *  
 *  @param dev_capability       Pointer to Dev Capability
 *  @param group_capability     Pointer to Group Capability
 *  
 *  @return         None 
 */
error_type_t
wifidirect_get_cfg_cmd_capability(t_u8 *dev_capability, t_u8 *group_capability)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_capability_config *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_capability_config);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_capability_config *) (buffer +
                                                     sizeof
                                                     (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_CAPABILITY_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_capability_config) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS)
        {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) 
            { 
                /* Print response */
                if (cmd_buf->result == CMD_SUCCESS)
                {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Device Capability = %d\n", (int) tlv->dev_capability));
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Group Capability = %d\n", (int) tlv->group_capability));
                    if (dev_capability)
                        *dev_capability = tlv->dev_capability;
                    if (group_capability)
                        *group_capability = tlv->group_capability;
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't get Capability info!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Set a wifidirect_persistent group record for a specific index
 *
 *  @param rec_index        Record Index
 *  @param dev_role         Device Role (1 - Group Owner, 2 - Client)
 *  @param group_bssid      Group BSSID Pointer
 *  @param dev_mac_address  Device MAC Address Pointer
 *  @param group_ssid_len   Group SSID Length
 *  @param group_ssid       Group SSID Pointer
 *  @param psk_len          PSK Length
 *  @param psk              PSK Pointer
 *  @param num_peers        Number of Peers (0,1,2)
 *  @param peermac1         Peer MAC 1 Pointer
 *  @param peermac2         Peer MAC 2 Pointer
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirect_set_cfg_cmd_persistent_group_record(t_u8 rec_index,
                                               t_u8 dev_role,
                                               char *group_bssid,
                                               char *dev_mac_address,
                                               t_u8 group_ssid_len,
                                               char *group_ssid,
                                               t_u8 psk_len,
                                               char *psk,
                                               t_u8 num_peers,
                                               char *peermac1,
                                               char *peermac2)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_persistent_group *tlv = NULL;
    t_u8 var_len = 0;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0, buf_len = 0;
    int ret;
    error_type_t retval = FAIL;

    cmd_len = sizeof(wifidirect_params_config);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
        cmd_buf = (wifidirect_params_config *) buffer;
        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;
        tlv =
            (tlvbuf_wifidirect_persistent_group *) (buffer +
                                                    sizeof
                                                    (wifidirect_params_config));

        tlv->tag = MRVL_WIFIDIRECT_PERSISTENT_GROUP_TLV_ID;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->rec_index = rec_index;
        tlv->dev_role = dev_role;
        if (group_bssid)
            memcpy(tlv->group_bssid, group_bssid, ETH_ALEN);
        if (dev_mac_address)
            memcpy(tlv->dev_mac_address, dev_mac_address, ETH_ALEN);

        /* ssid */
        tlv->group_ssid_len = group_ssid_len;
        var_len += tlv->group_ssid_len;
        if (group_ssid)
            memcpy(tlv->group_ssid, group_ssid, tlv->group_ssid_len);

        /* adjust pointer from here */
        /* psk */
        if (psk_len > 0) {
            if (psk)
                memcpy(tlv->psk + var_len, psk, psk_len);
            *(&tlv->psk_len + var_len) = psk_len;
            var_len += psk_len;
        }

        tlv->length =
            sizeof(tlvbuf_wifidirect_persistent_group) - MRVL_TLV_HEADER_SIZE +
            var_len;
        *(&tlv->num_peers + var_len) = num_peers;

        if ((peermac1) && (num_peers > 0))
            memcpy(tlv->peer_mac_addrs[0] + var_len, peermac1, ETH_ALEN);

        if ((peermac2) && (num_peers > 1))
            memcpy(tlv->peer_mac_addrs[1] + var_len, peermac2, ETH_ALEN);

        tlv->length += *(&tlv->num_peers + var_len) * ETH_ALEN;
        cmd_len += MRVL_TLV_HEADER_SIZE + tlv->length;
        buf_len = cmd_len;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;

        endian_convert_tlv_header_out(tlv);

        /* Send collective command */
        ret = wifidirect_ioctl(buffer, &cmd_len, buf_len);

        /* Process Response */
        if (ret == SUCCESS) 
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Setting persistent group information successful!\n"));
            retval = OK;
        } else {
            DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get a wifidirect_persistent group record for a specific index
 *
 *  @param rec_index        Record Index
 *  @param dev_role         Device Role (1 - Group Owner, 2 - Client) Pointer
 *  @param group_bssid      Group BSSID Pointer
 *  @param dev_mac_address  Device MAC Address Pointer
 *  @param group_ssid_len   Group SSID Length Pointer
 *  @param group_ssid       Group SSID Pointer
 *  @param psk_len          PSK Length Pointer
 *  @param psk              PSK Pointer
 *  @param num_peers        Number of Peers (0,1,2) Pointer
 *  @param peermac1         Peer MAC 1 Pointer
 *  @param peermac2         Peer MAC 2 Pointer
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirect_get_cfg_cmd_persistent_group_record(t_u8 rec_index,
                                               t_u8 *dev_role,
                                               char *group_bssid,
                                               char *dev_mac_address,
                                               t_u8 *group_ssid_len,
                                               char *group_ssid,
                                               t_u8 *psk_len,
                                               char *psk,
                                               t_u8 *num_peers,
                                               char *peermac1,
                                               char *peermac2)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_persistent_group *tlv = NULL;
    t_u8 index = 0, var_len = 0;
    t_u8 *buffer = NULL;
    int pi;
    t_u16 cmd_len = 0, buf_len = 0;
    int ret;
    error_type_t retval = FAIL;

    cmd_len = sizeof(wifidirect_params_config);
    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buffer) {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
        cmd_buf = (wifidirect_params_config *) buffer;
        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;
        tlv =
            (tlvbuf_wifidirect_persistent_group *) (buffer +
                                                    sizeof
                                                    (wifidirect_params_config));

        tlv->tag = MRVL_WIFIDIRECT_PERSISTENT_GROUP_TLV_ID;

        cmd_buf->action = ACTION_GET;
        tlv->rec_index = rec_index;
        tlv->length = sizeof(rec_index);
        cmd_len += MRVL_TLV_HEADER_SIZE + tlv->length;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        buf_len = MRVDRV_SIZE_OF_CMD_BUFFER;

        endian_convert_tlv_header_out(tlv);

        /* Send collective command */
        ret = wifidirect_ioctl(buffer, &cmd_len, buf_len);

        /* Process Response */
        if (ret == SUCCESS) 
        {
            endian_convert_tlv_header_in(tlv);

            if (tlv->length > 0) {
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Persistent group information =>\n"));
                buf_len = tlv->length;
                while (buf_len >
                       (sizeof(tlvbuf_wifidirect_persistent_group) - 2 * ETH_ALEN)) {
                    if (tlv->rec_index == rec_index) {
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n\t Index = [%d]\n", tlv->rec_index));
                        if (tlv->dev_role == 1) {
                            DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\t Role  = Group owner\n"));
                        } else {
                            DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\t Role  = Client\n"));
                        }

                        if (dev_role)
                            *dev_role = tlv->dev_role;

                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\t GroupBssId - "));
                        print_mac(tlv->group_bssid);
                        if (group_bssid)
                            memcpy(group_bssid, tlv->group_bssid, ETH_ALEN);

                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n\t DeviceId - "));
                        print_mac(tlv->dev_mac_address);
                        if (dev_mac_address)
                            memcpy(dev_mac_address, tlv->dev_mac_address, ETH_ALEN);

                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n\t SSID = "));
                        for (index = 0; index < tlv->group_ssid_len; index++) {
                            DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("%c", tlv->group_ssid[index]));
                        }
                            
                        if (group_ssid)
                            memcpy(group_ssid, &tlv->group_ssid[0], tlv->group_ssid_len);
                        if (group_ssid_len)
                            *group_ssid_len = tlv->group_ssid_len;
                        var_len = tlv->group_ssid_len;

                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n\t PSK = "));
                        for (index = 0; index < *(&tlv->psk_len + var_len); index++) {
                            if (index == 16) {
                                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n\t       "));
                            }
                            DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("%02x ", *(&tlv->psk[index] + var_len)));
                        }
                        if (psk)
                            memcpy(psk, (&tlv->psk[0] + var_len), *(&tlv->psk_len + var_len));
                        if (psk_len)
                            *psk_len = *(&tlv->psk_len + var_len);
                        var_len += *(&tlv->psk_len + var_len);

                        if (tlv->dev_role == 1) {
                            for (pi = 0; pi < *(&tlv->num_peers + var_len); pi++) {
                                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n\t Peer Mac address(%d) = ", pi));
                                print_mac(tlv->peer_mac_addrs[pi] + var_len);
                            }
                            if (num_peers)
                                *num_peers = *(&tlv->num_peers + var_len);
                            if ((peermac1) && (*(&tlv->num_peers + var_len) > 0))
                                memcpy(peermac1, (tlv->peer_mac_addrs[pi] + var_len), ETH_ALEN);
                            if ((peermac2) && (*(&tlv->num_peers + var_len) > 1))
                                memcpy(peermac2, (tlv->peer_mac_addrs[pi] + var_len), ETH_ALEN);

                            var_len += (*(&tlv->num_peers + var_len) * ETH_ALEN);
                        }
                    }
                    if (tlv->dev_role == 1) {
                        buf_len -= sizeof(tlvbuf_wifidirect_persistent_group) -
                            MRVL_TLV_HEADER_SIZE + var_len;
                        tlv = (tlvbuf_wifidirect_persistent_group *) (((t_u8 *) (tlv)) +
                                                                      (sizeof
                                                                       (tlvbuf_wifidirect_persistent_group)
                                                                       -
                                                                       MRVL_TLV_HEADER_SIZE
                                                                       + var_len));
                    } else {
                        /* num_peer field will not be present */
                        buf_len -= sizeof(tlvbuf_wifidirect_persistent_group) -
                            MRVL_TLV_HEADER_SIZE - sizeof(t_u8) + var_len;
                        tlv = (tlvbuf_wifidirect_persistent_group *) (((t_u8 *) (tlv)) +
                                                                      (sizeof
                                                                       (tlvbuf_wifidirect_persistent_group)
                                                                       -
                                                                       MRVL_TLV_HEADER_SIZE
                                                                       - sizeof(t_u8) +
                                                                       var_len));
                    }
                }
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("\n"));
            } else {
                DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Persistent group information is empty.\n"));
            }
            retval = OK;
        } else {
            DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Creates a wifidirect_persistent group Invoke request
 *         or Cancel
 *
 *  @param index   Index (0-3) or 
 *                 WIFIDIRECT_PERSISTENT_RECORD_CANCEL (0xFF)
 *  
 *  @return     OK or FAIL
 **/
error_type_t
wifidirect_set_cfg_cmd_persistent_group_invoke_cancel(t_u8 index)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_persistent_group *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;
    
    if ((index < WIFIDIRECT_PERSISTENT_GROUP_MAX) || (index == WIFIDIRECT_PERSISTENT_RECORD_CANCEL))
    {
        cmd_len = sizeof(wifidirect_params_config);
        buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
        if (buffer)
        {
            memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
            cmd_buf = (wifidirect_params_config *) buffer;
            cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
            cmd_buf->seq_num = 0;
            cmd_buf->result = 0;
            cmd_buf->action = cpu_to_le16(ACTION_SET);
            tlv =
                (tlvbuf_wifidirect_persistent_group *) (buffer +
                                                        sizeof
                                                        (wifidirect_params_config));

            tlv->tag = MRVL_WIFIDIRECT_PERSISTENT_GROUP_TLV_ID;
            tlv->length = sizeof(index);
            tlv->rec_index = index;

            cmd_len += MRVL_TLV_HEADER_SIZE + tlv->length;
            cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
            endian_convert_tlv_header_out(tlv);

            /* Send collective command */
            ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

            /* Process Response */
            if (ret == SUCCESS) 
            {
                DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Persistent group %s successful!\n",
                         (index == WIFIDIRECT_PERSISTENT_RECORD_CANCEL) ? "cancel" : "invoke"));
                retval = OK;
            } else {
                DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Persistent group %s failed!\n",
                         (index == WIFIDIRECT_PERSISTENT_RECORD_CANCEL) ? "cancel" : "invoke"));
            }
            MEM_FREE_AND_NULL(buffer);
        } else {
            DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Cannot allocate memory!\n"));
        }
    } else {
        DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Invalid index!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect presence request parameters
 *
 *  @param presence_req_type    Presence request type
 *  @param duration             Duration
 *  @param interval             Interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_presence_req_params(t_u8 presence_req_type, 
                                           t_u32 duration, 
                                           t_u32 interval)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_presence_req_params *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_presence_req_params);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_presence_req_params *) (buffer +
                                                       sizeof
                                                       (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_PRESENCE_REQ_PARAMS_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_presence_req_params) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->presence_req_type = presence_req_type;
        tlv->duration = cpu_to_le32(duration);
        tlv->interval = cpu_to_le32(interval);

        endian_convert_tlv_header_out(tlv);
        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) 
        {
            endian_convert_tlv_header_in(tlv);
            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
            {
                if (cmd_buf->result == CMD_SUCCESS)
                {
                    DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Presence request parameters set successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Couldn't set Presence request parameters!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect presence request parameters
 *
 *  @param presence_req_type    Pointer to presence request
 *                              type
 *  @param duration             Pointer to duration
 *  @param interval             Pointer to interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_presence_req_params(t_u8 *presence_req_type, 
                                           t_u32 *duration, 
                                           t_u32 *interval)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_presence_req_params *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    if ((presence_req_type != NULL) && (duration != NULL) && (interval != NULL))
    {
        cmd_len =
            sizeof(wifidirect_params_config) +
            sizeof(tlvbuf_wifidirect_presence_req_params);

        buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

        if (buffer)
        {
            memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

            cmd_buf = (wifidirect_params_config *) buffer;
            tlv =
                (tlvbuf_wifidirect_presence_req_params *) (buffer +
                                                           sizeof
                                                           (wifidirect_params_config));

            cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
            cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
            cmd_buf->seq_num = 0;
            cmd_buf->result = 0;

            tlv->tag = MRVL_WIFIDIRECT_PRESENCE_REQ_PARAMS_TLV_ID;
            tlv->length = sizeof(tlvbuf_wifidirect_presence_req_params) - MRVL_TLV_HEADER_SIZE;

            cmd_buf->action = ACTION_GET;
            endian_convert_tlv_header_out(tlv);
            ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

            /* Process Response */
            if (ret == SUCCESS) 
            {
                endian_convert_tlv_header_in(tlv);
                /* Verify response */
                if (cmd_buf->cmd_code ==
                    (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK))
                {
                    if (cmd_buf->result == CMD_SUCCESS)
                    {
                        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Presence request type = %d\n",
                               (int) tlv->presence_req_type));
                        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Duration of NoA descriptor = %ld\n",
                               le32_to_cpu(tlv->duration)));
                        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Interval of NoA descriptor = %ld\n",
                               le32_to_cpu(tlv->interval)));

                        if (presence_req_type)
                            *presence_req_type = tlv->presence_req_type;
                        if (duration)
                            *duration = le32_to_cpu(tlv->duration);
                        if (interval)
                            *interval = le32_to_cpu(tlv->interval);
                        retval = OK;
                    } else {
                        DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Couldn't get Presence request parameters!\n"));
                    }
                } else {
                    DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Corrupted response!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Command sending failed!\n"));
            }
            MEM_FREE_AND_NULL(buffer);
        } else {
            DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Cannot allocate memory!\n"));
        }
    } else {
        DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Invalid parameter pointer(s)!\n"));
    }

    return retval;
}



/** 
 *  @brief Set wifidirect extended listen time parameters
 *
 *  @param duration Duration
 *  @param interval Interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_ext_listen_time(t_u16 duration, t_u16 interval)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_mrvl_ext_listen_time *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_mrvl_ext_listen_time);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer)
    {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_mrvl_ext_listen_time *) (buffer +
                                                        sizeof
                                                        (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_EXTENDED_LISTEN_TIME_TLV_ID;
        tlv->length = sizeof(tlvbuf_wifidirect_mrvl_ext_listen_time) - MRVL_TLV_HEADER_SIZE;
        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->duration = cpu_to_le16(duration);
        tlv->interval = cpu_to_le16(interval);

        endian_convert_tlv_header_out(tlv);
        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) 
        {
            endian_convert_tlv_header_in(tlv);
            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) 
            {
                if (cmd_buf->result == CMD_SUCCESS)
                {
                    DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Extended listen timing parameters set successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Couldn't set Extended listen time!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect extended listen time parameters
 *
 *  @param duration Pointer to duration
 *  @param interval Pointer to interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_ext_listen_time(t_u16 *duration, t_u16 *interval)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_mrvl_ext_listen_time *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    if ((duration != NULL) && (interval != NULL))
    {
        cmd_len =
            sizeof(wifidirect_params_config) +
            sizeof(tlvbuf_wifidirect_mrvl_ext_listen_time);

        buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

        if (buffer)
        {
            memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

            cmd_buf = (wifidirect_params_config *) buffer;
            tlv =
                (tlvbuf_wifidirect_mrvl_ext_listen_time *) (buffer +
                                                            sizeof
                                                            (wifidirect_params_config));

            cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
            cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
            cmd_buf->seq_num = 0;
            cmd_buf->result = 0;

            tlv->tag = MRVL_WIFIDIRECT_EXTENDED_LISTEN_TIME_TLV_ID;
            tlv->length = sizeof(tlvbuf_wifidirect_mrvl_ext_listen_time) - MRVL_TLV_HEADER_SIZE;
            cmd_buf->action = ACTION_GET;

            endian_convert_tlv_header_out(tlv);
            ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);
            
            /* Process Response */
            if (ret == SUCCESS)
            {
                endian_convert_tlv_header_in(tlv);

                /* Verify response */
                if (cmd_buf->cmd_code ==
                    (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) 
                {
                    if (cmd_buf->result == CMD_SUCCESS) 
                    {
                        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Availability Period = %d\n",
                               le16_to_cpu(tlv->duration)));
                        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Availability Interval = %d\n",
                               le16_to_cpu(tlv->interval)));

                        *duration = le16_to_cpu(tlv->duration);
                        *interval = le16_to_cpu(tlv->interval);
                        retval = OK;
                    } else {
                        DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Couldn't get Extended listen time!\n"));
                    }
                } else {
                    DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Corrupted response!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Command sending failed!\n"));
            }
            MEM_FREE_AND_NULL(buffer);
        } else {
            DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Cannot allocate memory!\n"));
        }
    } else {
        DPRINTF((DBG_ERROR|DBG_OUTPUT), ("ERR:Invalid parameter pointer(s)!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect provisioning protocol related parameters
 *
 *  @param action           Action - 1: Request parameters 2: Response parameters
 *  @param config_methods   Config Methods
 *  @param dev_password     Device Password
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_set_cfg_cmd_provisioning_params(t_u16 action,
                                           t_u16 config_methods,
                                           t_u16 dev_password)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_provisioning_params *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_provisioning_params);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) { 
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_provisioning_params *) (buffer +
                                                       sizeof
                                                       (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_PROVISIONING_PARAMS_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_provisioning_params) - MRVL_TLV_HEADER_SIZE;
        cmd_buf->action = cpu_to_le16(ACTION_SET);
        tlv->action = cpu_to_le16(action);
        tlv->config_methods = cpu_to_le16(config_methods);
        tlv->dev_password = cpu_to_le16(dev_password);

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) { 
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,
                            ("Provisioning protocol parameters setting successful!\n"));
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't set provisioing parameters!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect provisioning protocol related parameters
 *
 *  @param action           Action Pointer - 1: Request parameters 2: Response parameters
 *  @param config_methods   Config Methods Pointer
 *  @param dev_password     Device Password Pointer
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_get_cfg_cmd_provisioning_params(t_u16 *action,
                                           t_u16 *config_methods,
                                           t_u16 *dev_password)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_provisioning_params *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) +
        sizeof(tlvbuf_wifidirect_provisioning_params);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) { 
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_provisioning_params *) (buffer +
                                                       sizeof
                                                       (wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_PROVISIONING_PARAMS_TLV_ID;
        tlv->length =
            sizeof(tlvbuf_wifidirect_provisioning_params) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);

            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) { 
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Action = %s %s\n", (le16_to_cpu(tlv->action)) == 1 ?
                           "Request" : "Response", "parameters"));
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Config Methods = %02X\n",
                           le16_to_cpu(tlv->config_methods)));
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Device Password ID = %02X\n",
                           le16_to_cpu(tlv->dev_password)));
                    if (action)
                        *action = le16_to_cpu(tlv->action);
                    if (config_methods)
                        *config_methods = le16_to_cpu(tlv->config_methods);
                    if (dev_password)
                        *dev_password = le16_to_cpu(tlv->dev_password);
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't get provisioing parameters!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}


/** 
 *  @brief Set wifidirect WPS parametters 
 *
 *  @param wps_pin_pbc  WPS Pin (1) or PBC (2)
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_wps_params(int wps_pin_pbc)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_wps_params *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    if ((wps_pin_pbc == 1) && (wps_pin_pbc == 2)) {
        cmd_len =
            sizeof(wifidirect_params_config) + sizeof(tlvbuf_wifidirect_wps_params);

        buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

        if (buffer) {
            memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

            cmd_buf = (wifidirect_params_config *) buffer;
            tlv =
                (tlvbuf_wifidirect_wps_params *) (buffer +
                                                  sizeof(wifidirect_params_config));

            cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
            cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
            cmd_buf->seq_num = 0;
            cmd_buf->result = 0;

            tlv->tag = MRVL_WIFIDIRECT_WPS_PARAMS_TLV_ID;
            tlv->length = sizeof(tlvbuf_wifidirect_wps_params) - MRVL_TLV_HEADER_SIZE;

            cmd_buf->action = cpu_to_le16(ACTION_SET);
            tlv->action = cpu_to_le16(wps_pin_pbc);

            endian_convert_tlv_header_out(tlv);

            ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

            /* Process Response */
            if (ret == SUCCESS) {
                endian_convert_tlv_header_in(tlv);
                /* Verify response */
                if (cmd_buf->cmd_code ==
                    (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) {
                    /* Print response */
                    if (cmd_buf->result == CMD_SUCCESS) {
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("WPS parameter setting successful!\n"));
                        retval = OK;
                    } else {
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("ERR:Couldn't set WPS parameters !\n"));
                    }
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
            }
            MEM_FREE_AND_NULL(buffer);
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
        }
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Invalid input, should be pin (1) or pbc (2)!\n"));
    }

    return retval;
}


/** 
 *  @brief Get wifidirect WPS parametters 
 *
 *  @param wps_pin_pbc  Pointer to WPS Pin (1) or PBC (2)
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_wps_params(int *wps_pin_pbc)
{
    wifidirect_params_config *cmd_buf = NULL;
    tlvbuf_wifidirect_wps_params *tlv = NULL;
    t_u8 *buffer = NULL;
    t_u16 cmd_len = 0;
    int ret = 0;
    error_type_t retval = FAIL;

    cmd_len =
        sizeof(wifidirect_params_config) + sizeof(tlvbuf_wifidirect_wps_params);

    buffer = (t_u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);

    if (buffer) {
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

        cmd_buf = (wifidirect_params_config *) buffer;
        tlv =
            (tlvbuf_wifidirect_wps_params *) (buffer +
                                              sizeof(wifidirect_params_config));

        cmd_buf->cmd_code = HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG;
        cmd_buf->size = cmd_len - BUF_HEADER_SIZE;
        cmd_buf->seq_num = 0;
        cmd_buf->result = 0;

        tlv->tag = MRVL_WIFIDIRECT_WPS_PARAMS_TLV_ID;
        tlv->length = sizeof(tlvbuf_wifidirect_wps_params) - MRVL_TLV_HEADER_SIZE;

        cmd_buf->action = ACTION_GET;

        endian_convert_tlv_header_out(tlv);

        ret = wifidirect_ioctl(buffer, &cmd_len, MRVDRV_SIZE_OF_CMD_BUFFER);

        /* Process Response */
        if (ret == SUCCESS) {
            endian_convert_tlv_header_in(tlv);
            /* Verify response */
            if (cmd_buf->cmd_code ==
                (HostCmd_CMD_WIFIDIRECT_PARAMS_CONFIG | WIFIDIRECTCMD_RESP_CHECK)) {
                /* Print response */
                if (cmd_buf->result == CMD_SUCCESS) {
                    DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("WPS password parameter =>"));
                    switch (le16_to_cpu(tlv->action)) {
                    case 1:
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("Pin\n"));
                        break;
                    case 2:
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("PBC\n"));
                        break;
                    default:
                        DPRINTF((DBG_SOFT | DBG_OUTPUT) ,("None\n"));
                        break;
                    }
                    if (wps_pin_pbc)
                        *wps_pin_pbc = le16_to_cpu(tlv->action);
                    retval = OK;
                } else {
                    DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Couldn't get WPS parameters !\n"));
                }
            } else {
                DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Corrupted response!\n"));
            }
        } else {
            DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Command sending failed!\n"));
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT) ,("ERR:Cannot allocate memory!\n"));
    }

    return retval;
}

/**
 * @brief WFD Configure
 *
 * @param N/A
 *
 * @return OK or FAIL.
 */
error_type_t
wfd_configure(void)
{
#if 0    
    error_type_t retval;

    // Create Wifi-Direct configuration with defaults if it has not been created yet
    if (!g_wfd_config)
    {
        if ((retval = wifidirect_create_config(&g_wfd_config)) != OK){
            DPRINTF((DBG_ERROR | DBG_OUTPUT),("ERR:Cannot create configuration wfd_configure!\n"));
            return retval;
        }
    }

    // Send Wifi-Direct configuration
    if ((retval = wifidirectcmd_config(g_wfd_config, 0)) != OK) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("ERR:Cannot send configuration wfd_configure!\n"));
    }
#endif

    wifidirect_config_t *p_wfd_config;
    error_type_t retval;

    // Create Wifi-Direct configuration
    if ((retval = wifidirect_create_config(&p_wfd_config)) == OK) {
        // Send Wifi-Direct configuration
        if ((retval = wifidirectcmd_config(p_wfd_config, 0)) != OK) {
            DPRINTF((DBG_ERROR | DBG_OUTPUT),("ERR:Cannot send configuration wfd_configure!\n"));
        }
        wifidirect_delete_config(&p_wfd_config);
    } else {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("ERR:Cannot create configuration wfd_configure!\n"));
    }

    return retval;
}

/**
 * @brief WFD Parameters Configure
 *
 * @param N/A
 *
 * @return OK or FAIL.
 */
error_type_t
wfd_params_configure(void)
{
    error_type_t retval;

    // Create Wifi-Direct params configuration with defaults if it has not been created yet
    if (!g_wfd_params_config)
    {
        if ((retval = wifidirect_create_params_config(&g_wfd_params_config)) != OK){
            DPRINTF((DBG_ERROR | DBG_OUTPUT),("ERR:Cannot create params configuration wfd_params_configure!\n"));
            return retval;     
        }
    }

    // Send Wifi-Direct params configuration
    if ((retval = wifidirectcmd_params_config(g_wfd_params_config)) != OK) {
        DPRINTF((DBG_ERROR | DBG_OUTPUT),("ERR:Cannot send params configuration wfd_params_configure!\n"));
    }
   
    return retval;
}

/**
 * @brief Get WFD configuration
 *
 * @param N/A
 *
 * @return Pointer to the wfd configuration struct.
 */
wifidirect_config_t *get_wfd_config(void)
{
    //if WFD is not yet configured, first create a configuration
    if (!g_wfd_config)
    {
        if (OK !=  wifidirect_create_config(&g_wfd_config)){
            DPRINTF((DBG_ERROR | DBG_OUTPUT),("ERR:Cannot create configuration get_wfd_config!\n"));
            return NULL;
        }
    }
    
    return g_wfd_config;
}

/**
 * @brief Get WFD Parameters configuration
 *
 * @param N/A
 *
 * @return Pointer to the wfd params configuration struct.
 */
wifidirect_param_config_t *get_wfd_params_config(void)
{
    return g_wfd_params_config;
}

