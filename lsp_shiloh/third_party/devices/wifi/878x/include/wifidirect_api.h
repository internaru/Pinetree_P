/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file wifidirect_api.h
 * 
 * @brief Wireless Wifi-Direct API
 * 
 * This API exposes routines for Wifi-Direct
 */

#ifndef WIFIDIRECT_API_H
#define WIFIDIRECT_API_H

/** WPS Device Name maximum length */
#define WPS_DEVICE_NAME_MAX_LEN         32
/** WPS Model maximum length */
#define WPS_MODEL_MAX_LEN               32
/** WPS Serial maximum length */
#define WPS_SERIAL_MAX_LEN              32
/** WPS Manufacturer maximum length */
#define WPS_MANUFACT_MAX_LEN            64
/** WPS Device Info OUI+Type+SubType Length */
#define WPS_DEVICE_TYPE_LEN             8
/** Maximum channels */
#define MAX_CHANNEL_LIST_CHANNELS       14
/** Maximum number of NoA descriptors */
#define MAX_NOA_DESCRIPTORS             8

#define WIFIDIRECT_MODE_STOP_STATUS         0
#define WIFIDIRECT_MODE_START_STATUS        1 //listen?
#define WIFIDIRECT_MODE_START_GROUP_OWNER   2
#define WIFIDIRECT_MODE_START_CLIENT        3
#define WIFIDIRECT_MODE_START_FIND_PHASE    4
#define WIFIDIRECT_MODE_STOP_FIND_PHASE     5

typedef struct _wifidirect_capability_t
{
    /** WIFIDIRECT device capability */
    t_u8 dev_capability;
    /** WIFIDIRECT group capability */
    t_u8 group_capability;
} wifidirect_capability_t;

typedef struct _wifidirect_channel_t
{
    /** WIFIDIRECT country string */
    t_u8 country_string[3];
    /** WIFIDIRECT regulatory class */
    t_u8 regulatory_class;
    /** WIFIDIRECT channel number */
    t_u8 channel_number;
} wifidirect_channel_t;

typedef struct _wifidirect_channel_entry_t
{
    /** WIFIDIRECT regulatory class */
    t_u8 regulatory_class;
    /** WIFIDIRECT no of channels */
    t_u8 num_of_channels;
    /** WIFIDIRECT channel number */
    t_u8 chan_list[MAX_CHANNEL_LIST_CHANNELS];
} wifidirect_channel_entry_t;

#define WIFIDIRECT_CONFIG_NUMBER_MAX_CHANNEL_LIST_ENTRIES   2

typedef struct _wifidirect_channel_list_t
{
    /** WIFIDIRECT country string */
    t_u8 country_string[3];
    /** WIFIDIRECT channel entry list number */
    t_u8 num_channel_list_entries;
    /** WIFIDIRECT channel entry list */
    wifidirect_channel_entry_t chan_entry_list[WIFIDIRECT_CONFIG_NUMBER_MAX_CHANNEL_LIST_ENTRIES];
} wifidirect_channel_list_t;


typedef struct _wifidirect_noa_descriptor_t
{
    /** WIFIDIRECT count OR type */
    t_u8 count_type;
    /** WIFIDIRECT duration */
    t_u32 duration;
    /** WIFIDIRECT interval */
    t_u32 interval;
    /** WIFIDIRECT start time */
    t_u32 start_time;
} wifidirect_noa_descriptor_t;

typedef struct _wifidirect_notice_of_absence_t
{
    /** WIFIDIRECT NoA Index */
    t_u8 noa_index;
    /** WIFIDIRECT CTWindow and OppPS parameters */
    t_u8 ctwindow_opp_ps;
    /** WIFIDIRECT NoA Descriptor list count */
    t_u8 noa_descriptor_count;
    /** WIFIDIRECT NoA Descriptor list */
    wifidirect_noa_descriptor_t noa_descriptor_list[MAX_NOA_DESCRIPTORS];
} wifidirect_notice_of_absence_t;

typedef struct _wifidirect_secondary_device_t
{
    /** Secondary device type : category */
    t_u16 secondary_category;
    /** Secondary device type : OUI */
    t_u8 secondary_oui[4];
    /** Secondary device type : sub-category */
    t_u16 secondary_subcategory;
} wifidirect_secondary_device_t;

#define WIFIDIRECT_CONFIG_DEVICE_INFO_MAX_SECONDARY_DEV_ENTRIES   1

typedef struct _wifidirect_device_info_t
{
    /** WIFIDIRECT device address */
    t_u8 dev_address[ETH_ALEN];
    /** WPS config methods */
    t_u16 config_methods;
    /** Primary device type : category */
    t_u16 primary_category;
    /** Primary device type : OUI */
    t_u8 primary_oui[4];
    /** Primary device type : sub-category */
    t_u16 primary_subcategory;
    /** Secondary Device Count */
    t_u8 secondary_dev_count;
    /** Secondary Device Info */
    wifidirect_secondary_device_t secondary_dev_info[WIFIDIRECT_CONFIG_DEVICE_INFO_MAX_SECONDARY_DEV_ENTRIES];
    /** WPS Device Name Tag */
    t_u16 device_name_type;
    /** WPS Device Name Length */
    t_u16 device_name_len;
    /** Device name */
    t_u8 device_name[WPS_DEVICE_NAME_MAX_LEN + 1];
} wifidirect_device_info_t;

typedef struct _wifidirect_group_id_t
{
    /** WIFIDIRECT group MAC address */
    t_u8 group_address[ETH_ALEN];
    /** WIFIDIRECT group SSID */
    t_u8 group_ssid[WPS_DEVICE_NAME_MAX_LEN + 1];
} wifidirect_group_id_t;

typedef struct _wifidirect_interface_id_t
{
    /** WIFIDIRECT interface Id */
    t_u8 interface_id[ETH_ALEN];
} wifidirect_interface_id_t;

#define WIFIDIRECT_CONFIG_INTERFACE_MAX_LIST_ENTRIES   2

typedef struct _wifidirect_interface_t
{
    /** WIFIDIRECT interface Id */
    t_u8 interface_id[ETH_ALEN];
    /** WIFIDIRECT interface count */
    t_u8 interface_count;
    /** WIFIDIRECT interface addresslist */
    wifidirect_interface_id_t interface_idlist[WIFIDIRECT_CONFIG_INTERFACE_MAX_LIST_ENTRIES];
} wifidirect_interface_t;

typedef struct _wifidirect_config_timeout_t
{
    /** Group configuration timeout */
    t_u8 group_config_timeout;
    /** Device configuration timeout */
    t_u8 device_config_timeout;
} wifidirect_config_timeout_t;

typedef struct _wifidirect_ext_listen_time_t
{
    /** Availability period */
    t_u16 availability_period;
    /** Availability interval */
    t_u16 availability_interval;
} wifidirect_ext_listen_time_t;

typedef struct _wifidirect_wps_config_t
{
    t_u8    WPSVersion;
    t_u8    WPSSetupState;
    t_u8    WPSRequestType;
    t_u8    WPSResponseType;
    t_u16   WPSSpecConfigMethods;
    t_u8    WPSUUID[WPS_UUID_MAX_LEN];
    t_u8    WPSPrimaryDeviceType[WPS_DEVICE_TYPE_LEN];
    t_u8    WPSRFBand;
    t_u16   WPSAssociationState;
    t_u16   WPSConfigurationError;
    t_u16   WPSDevicePassword;
    t_u8    WPSDeviceName[WPS_DEVICE_NAME_MAX_LEN + 1];
    t_u8    WPSManufacturer[WPS_MANUFACT_MAX_LEN + 1];
    t_u8    WPSModelName[WPS_MODEL_MAX_LEN + 1];
    t_u8    WPSModelNumber[WPS_MODEL_MAX_LEN + 1];
    t_u8    WPSSerialNumber[WPS_SERIAL_MAX_LEN + 1];
} wifidirect_wps_config_t;

typedef struct _wifidirect_config_t
{
    wifidirect_capability_t         capability;
    t_u8                            group_owner_intent;
    wifidirect_channel_t            channel;
    t_u8                            manageability;
    wifidirect_channel_list_t       channel_list;
    wifidirect_notice_of_absence_t  notice_of_absence;
    wifidirect_device_info_t        device_info;
    wifidirect_group_id_t           group_id;
    t_u8                            group_bss_id[ETH_ALEN];
    t_u8                            device_id_mac_addr[ETH_ALEN];
    wifidirect_interface_t          interface;
    wifidirect_config_timeout_t     config_timeout;
    wifidirect_ext_listen_time_t    ext_listen_time;
    t_u8                            intended_addr[ETH_ALEN];
    wifidirect_channel_t            operating_channel;
    t_u8                            invitation_flag;
    wifidirect_wps_config_t         wps_config;
} wifidirect_config_t;

typedef struct _wifidirect_discovery_intervals_t
{
    t_u16   min_disc_interval;
    t_u16   max_disc_interval;
} wifidirect_discovery_intervals_t;

typedef struct _wifidirect_param_config_t
{
    wifidirect_discovery_intervals_t    discovery_intervals;
    t_u8                                enable_scan;
    t_u16                               device_state;
} wifidirect_param_config_t;

typedef struct _wifidirect_action_frame_t 
{
    /** Peer mac address */
    t_u8 peer_mac_addr[ETH_ALEN];
    /** Category */
    t_u8 category;
    /** Action */
    t_u8 action;
    /** OUI */
    t_u8 oui[3];
    /** OUI type */
    t_u8 oui_type;
    /** OUI sub type */
    t_u8 oui_sub_type;
    /** Dialog token */
    t_u8 dialog_token;
    /** IE List of TLVs */
    t_u8 * ie_list;
    /** IE List Length of TLVs */
    t_u16 ie_list_length;
} wifidirect_action_frame_t;

typedef struct wifidirect_disc_data_t
{
    union
    {
        struct upnp_specific_data
        {
            /** version field */
            t_u8 version;
            /** value */
            t_u8 *value;
        } upnp;

        struct bonjour_specific_data
        {
            /** DNS name */
            t_u8 *dns;
            /** DNS type */
            t_u8 dns_type;
            /** version field */
            t_u8 version;
            /** DNS name */
            t_u8 *record;
        } bonjour;
    } u;
} wifidirect_disc_data_t;


typedef struct _wifidirect_discovery_request_t
{
    /** Peer mac address */
    t_u8 peer_mac_addr[ETH_ALEN];
    /** Category */
    t_u8 category;
    /** Action */
    t_u8 action;
    /** Dialog token */
    t_u8 dialog_token;
    /** Advertize protocol IE */
    t_u8 advertize_protocol_ie[4];
    /** Query request Length */
    t_u16 query_len;
    /** Information identifier */
    t_u8 info_id[2];
    /** Request Length */
    t_u16 request_len;
    /** OUI */
    t_u8 oui[3];
    /** OUI sub type */
    t_u8 oui_sub_type;
    /** Service update indicator */
    t_u16 service_update_indicator;
    /** Vendor Length */
    t_u16 vendor_len;
    /** Service protocol */
    t_u8 service_protocol;
    /** Service transaction Id */
    t_u8 service_transaction_id;
    /** Query Data */
    wifidirect_disc_data_t disc_data;
} wifidirect_discovery_request_t;

typedef struct _wifidirect_discovery_response_t
{
    /** Peer mac address */
    t_u8 peer_mac_addr[ETH_ALEN];
    /** Category */
    t_u8 category;
    /** Action */
    t_u8 action;
    /** Dialog token */
    t_u8 dialog_token;
    /** Status code */
    t_u8 status_code;
    /** GAS comback reply */
    t_u16 gas_reply;
    /** Advertize protocol IE */
    t_u8 advertize_protocol_ie[4];
    /** Query response Length */
    t_u16 query_len;
    /** Information identifier */
    t_u8 info_id[2];
    /** Response Length */
    t_u16 response_len;
    /** OUI */
    t_u8 oui[3];
    /** OUI sub type */
    t_u8 oui_sub_type;
    /** Service update indicator */
    t_u16 service_update_indicator;
    /** Vendor Length */
    t_u16 vendor_len;
    /** Service protocol */
    t_u8 service_protocol;
    /** Service transaction Id */
    t_u8 service_transaction_id;
    /** Discovery status code (Service Status) */
    t_u8 disc_status_code;
    /** Response Data */
    wifidirect_disc_data_t disc_data;
} wifidirect_discovery_response_t;

typedef struct _wifidirect_gas_comeback_request_t
{
    /** Peer mac address */
    t_u8 peer_mac_addr[ETH_ALEN];
    /** Category */
    t_u8 category;
    /** Action */
    t_u8 action;
    /** Dialog token */
    t_u8 dialog_token;
} wifidirect_gas_comeback_request_t;


typedef struct _wifidirect_gas_comeback_response_t
{
    /** Peer mac address */
    t_u8 peer_mac_addr[ETH_ALEN];
    /** Category */
    t_u8 category;
    /** Action */
    t_u8 action;
    /** Dialog token */
    t_u8 dialog_token;
    /** Status code */
    t_u8 status_code;
    /** Gas response fragment ID */
    t_u8 gas_fragment_id;
    /** GAS comback reply */
    t_u16 gas_reply;
    /** Advertize protocol IE */
    t_u8 advertize_protocol_ie[4];
    /** Query response Length */
    t_u16 query_len;
    /** Information identifier */
    t_u8 info_id[2];
    /** Response Length */
    t_u16 response_len;
    /** Response status code */
    t_u8 resp_status_code;
    /** OUI */
    t_u8 oui[3];
    /** OUI sub type */
    t_u8 oui_sub_type;
    /** Service update indicator */
    t_u16 service_update_indicator;
    /** Vendor Length */
    t_u16 vendor_len;
    /** Service protocol */
    t_u8 service_protocol;
    /** Service transaction Id */
    t_u8 service_transaction_id;
    /** Discovery status code */
    t_u8 disc_status_code;
    /** Response Data */
    wifidirect_disc_data_t disc_data;
} wifidirect_gas_comeback_response_t;

#define WIFIDIRECT_SET_DEV_CAPABILITY(handle, tmp)      handle->capability.dev_capability = tmp
#define WIFIDIRECT_SET_GROUP_CAPABILITY(handle, tmp)    handle->capability.group_capability = tmp
#define WIFIDIRECT_SET_GROUP_OWNER_INTENT(handle, tmp)  handle->group_owner_intent = tmp
#define WIFIDIRECT_SET_CHANNEL_NUMBER(handle, tmp)      handle->channel.channel_number = tmp
#define WIFIDIRECT_SET_CHANNEL_COUNTRY_STRING(handle, tmp)  memcpy(handle->channel.country_string, tmp, 3)
#define WIFIDIRECT_SET_CHANNEL_REGULATORY_CLASS(handle, tmp)    handle->channel.regulatory_class = tmp
#define WIFIDIRECT_SET_MANAGEABILITY(handle, tmp)       handle->manageability = tmp
#define WIFIDIRECT_SET_CHANNEL_LIST_NUMBER_OF_ENTRIES(handle, tmp)  handle->channel_list.num_channel_list_entries = tmp
#define WIFIDIRECT_SET_CHANNEL_LIST_COUNTRY_STRING(handle, tmp)     memcpy(handle->channel_list.country_string, tmp, 3)
#define WIFIDIRECT_SET_CHANNEL_LIST_REGULATORY_CLASS(handle, index, tmp)    handle->channel_list.chan_entry_list[index].regulatory_class = tmp
#define WIFIDIRECT_SET_CHANNEL_LIST_NUMBER_OF_CHANNELS(handle, index, tmp)  handle->channel_list.chan_entry_list[index].num_of_channels = tmp
#define WIFIDIRECT_SET_CHANNEL_LIST_CHANNEL_LIST(handle, index, number, tmp)    memcpy(handle->channel_list.chan_entry_list[index].chan_list, tmp, number)
#define WIFIDIRECT_SET_NOTICE_OF_ABSENCE_NOA_INDEX(handle, tmp) handle->notice_of_absence.noa_index = tmp
#define WIFIDIRECT_SET_NOTICE_OF_ABSENCE_CTWINDOW_OPP_PS(handle, tmp)   handle->notice_of_absence.ctwindow_opp_ps = tmp
#define WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_COUNT(handle, tmp)  handle->notice_of_absence.noa_descriptor_count = tmp
#define WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_COUNT_TYPE(handle, index, tmp)  handle->notice_of_absence.noa_descriptor_list[index].count_type = tmp
#define WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_DURATION(handle, index, tmp)    handle->notice_of_absence.noa_descriptor_list[index].duration = tmp
#define WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_INTERVAL(handle, index, tmp)    handle->notice_of_absence.noa_descriptor_list[index].interval = tmp
#define WIFIDIRECT_SET_NOTICE_OF_ABSENCE_DESCRIPTOR_START_TIME(handle, index, tmp)  handle->notice_of_absence.noa_descriptor_list[index].start_time = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_DEV_ADDR(handle, tmp)    memcpy(handle->device_info.dev_address, tmp, ETH_ALEN)
#define WIFIDIRECT_SET_DEVICE_INFO_CONFIG_METHODS(handle, tmp)  handle->device_info.config_methods = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_PRIMARY_CATEGORY(handle, tmp)    handle->device_info.primary_category = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_PRIMARY_OUI(handle, tmp) memcpy(handle->device_info.primary_oui, tmp, 4)
#define WIFIDIRECT_SET_DEVICE_INFO_PRIMARY_SUB_CATEGORY(handle, tmp)    handle->device_info.primary_subcategory = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_SECONDARY_DEV_COUNT(handle, tmp)     handle->device_info.secondary_dev_count = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_SECONDARY_CATEGORY(handle, index, tmp)   handle->device_info.secondary_dev_info[index].secondary_category = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_SECONDARY_OUI(handle, index, tmp)    memcpy(handle->device_info.secondary_dev_info[index].secondary_oui, tmp, 4)
#define WIFIDIRECT_SET_DEVICE_INFO_SECONDARY_SUB_CATEGORY(handle, index, tmp)   handle->device_info.secondary_dev_info[index].secondary_subcategory = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_DEVICE_NAME_TYPE(handle, tmp)    handle->device_info.device_name_type = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_DEVICE_NAME_LENGTH(handle, tmp)    handle->device_info.device_name_len = tmp
#define WIFIDIRECT_SET_DEVICE_INFO_DEVICE_NAME(handle, tmp)    memcpy(handle->device_info.device_name, tmp, strlen(tmp))
#define WIFIDIRECT_SET_GROUP_ADDR(handle, tmp)          memcpy(handle->group_id.group_address, tmp, ETH_ALEN)
#define WIFIDIRECT_SET_GROUP_SSID(handle, tmp)          memcpy(handle->group_id.group_ssid, tmp, strlen(tmp))
#define WIFIDIRECT_SET_GROUP_BSS_ID(handle, tmp)        memcpy(handle->group_bss_id, tmp, ETH_ALEN)
#define WIFIDIRECT_SET_DEVICE_ID_MAC_ADDR(handle, tmp)  memcpy(handle->device_id_mac_addr, tmp, ETH_ALEN)
#define WIFIDIRECT_SET_INTERFACE_ID(handle, tmp)        memcpy(handle->interface.interface_id, tmp, ETH_ALEN)
#define WIFIDIRECT_SET_INTERFACE_COUNT(handle, tmp)     handle->interface.interface_count = tmp
#define WIFIDIRECT_SET_INTERFACE_LIST_ID(handle, index, tmp)    memcpy(handle->interface.interface_idlist[index].interface_id, tmp, ETH_ALEN)
#define WIFIDIRECT_SET_GROUP_CONFIG_TIMEOUT(handle, tmp) handle->config_timeout.group_config_timeout = tmp
#define WIFIDIRECT_SET_DEVICE_CONFIG_TIMEOUT(handle, tmp) handle->config_timeout.device_config_timeout = tmp
#define WIFIDIRECT_SET_EXT_LISTEN_AVAIL_PERIOD(handle, tmp) handle->ext_listen_time.availability_period = tmp
#define WIFIDIRECT_SET_EXT_LISTEN_AVAIL_INTERVAL(handle, tmp)   handle->ext_listen_time.availability_interval = tmp
#define WIFIDIRECT_SET_INTENDED_ADDR(handle, tmp)       memcpy(handle->intended_addr, tmp, ETH_ALEN)
#define WIFIDIRECT_SET_OPERATING_CHANNEL_NUMBER(handle, tmp)    handle->operating_channel.channel_number = tmp
#define WIFIDIRECT_SET_OPERATING_CHANNEL_COUNTRY_STRING(handle, tmp)    memcpy(handle->operating_channel.country_string, tmp, 3)
#define WIFIDIRECT_SET_OPERATING_CHANNEL_REGULATORY_CLASS(handle, tmp)  handle->operating_channel.regulatory_class = tmp
#define WIFIDIRECT_SET_INVATION_FLAG(handle, tmp)       handle->invitation_flag = tmp

#define WIFIDIRECT_SET_WPS_VERSION(handle, tmp)         handle->wps_config.WPSVersion = tmp
#define WIFIDIRECT_SET_WPS_SETUP_STATE(handle, tmp)     handle->wps_config.WPSSetupState = tmp
#define WIFIDIRECT_SET_WPS_REQUEST_TYPE(handle, tmp)    handle->wps_config.WPSRequestType = tmp
#define WIFIDIRECT_SET_WPS_RESPONSE_TYPE(handle, tmp)   handle->wps_config.WPSResponseType = tmp
#define WIFIDIRECT_SET_WPS_SPEC_CONFIG_METHODS(handle, tmp) handle->wps_config.WPSSpecConfigMethods = tmp
#define WIFIDIRECT_SET_WPS_UUID(handle, tmp)            memcpy(handle->wps_config.WPSUUID, tmp, strlen(tmp))
#define WIFIDIRECT_SET_WPS_PRIMARY_DEVICE_TYPE(handle, tmp) memcpy(handle->wps_config.WPSPrimaryDeviceType, tmp, WPS_DEVICE_TYPE_LEN)
#define WIFIDIRECT_SET_WPS_RFBAND(handle, tmp)          handle->wps_config.WPSRFBand = tmp
#define WIFIDIRECT_SET_WPS_ASSOCIATION_STATE(handle, tmp)   handle->wps_config.WPSAssociationState = tmp
#define WIFIDIRECT_SET_WPS_CONFIGURATION_ERROR(handle, tmp) handle->wps_config.WPSConfigurationError = tmp
#define WIFIDIRECT_SET_WPS_DEVICE_PASSWORD(handle, tmp) handle->wps_config.WPSDevicePassword = tmp

#define WIFIDIRECT_SET_DISC_MIN_INTERVAL(handle, tmp)   handle->discovery_intervals.min_disc_interval = tmp
#define WIFIDIRECT_SET_DISC_MAX_INTERVAL(handle, tmp)   handle->discovery_intervals.max_disc_interval = tmp
#define WIFIDIRECT_SET_ENABLE_SCAN(handle, tmp)         handle->enable_scan = tmp
#define WIFIDIRECT_SET_DEVICE_STATE(handle, tmp)        handle->device_state = tmp

#define WIFIDIRECT_SET_PEER_MAC_ADDR(handle, tmp)       memcpy(handle->peer_mac_addr, tmp, ETH_ALEN)
#define WIFIDIRECT_SET_CATEGORY(handle, tmp)            handle->category = tmp
#define WIFIDIRECT_SET_ACTION(handle, tmp)              handle->action = tmp
#define WIFIDIRECT_SET_DIALOG_TOKEN(handle, tmp)        handle->dialog_token = tmp
#define WIFIDIRECT_SET_OUI(handle, tmp)                 memcpy(handle->oui, tmp, 3)
#define WIFIDIRECT_SET_OUI_TYPE(handle, tmp)            handle->oui_type = tmp
#define WIFIDIRECT_SET_OUI_SUBTYPE(handle, tmp)         handle->oui_sub_type = tmp
#define WIFIDIRECT_SET_IE_LIST_PTR(handle, tmp)         handle->ie_list = tmp
#define WIFIDIRECT_SET_IE_LIST_LENGTH(handle, tmp)      handle->ie_list_length = tmp

#define WIFIDIRECT_SET_ADVERTIZE_PROTOCOL(handle, tmp)          memcpy(handle->advertize_protocol_ie, tmp, 4)
#define WIFIDIRECT_SET_INFO_ID(handle, tmp)                     memcpy(handle->info_id, tmp, 2)
#define WIFIDIRECT_SET_SERVICE_UPDATE_INDICATOR(handle, tmp)    handle->service_update_indicator = tmp
#define WIFIDIRECT_SET_SERVICE_PROTOCOL(handle, tmp)            handle->service_protocol = tmp
#define WIFIDIRECT_SET_SERVICE_TRANSACTION_ID(handle, tmp)      handle->service_transaction_id = tmp
#define WIFIDIRECT_SET_BONJOUR_VERSION(handle, tmp)     handle->disc_data.u.bonjour.version = tmp
#define WIFIDIRECT_SET_BONJOUR_DNS_PTR(handle, tmp)     handle->disc_data.u.bonjour.dns = tmp
#define WIFIDIRECT_SET_BONJOUR_DNS_TYPE(handle, tmp)    handle->disc_data.u.bonjour.dns_type = tmp
#define WIFIDIRECT_SET_UPNP_VERSION(handle, tmp)        handle->disc_data.u.upnp.version = tmp
#define WIFIDIRECT_SET_UPNP_QUERY_VALUE_PTR(handle, tmp)   handle->disc_data.u.upnp.value = tmp

#define WIFIDIRECT_SET_STATUS_CODE(handle, tmp)         handle->status_code = tmp
#define WIFIDIRECT_SET_GAS_REPLY(handle, tmp)           handle->gas_reply = tmp
#define WIFIDIRECT_SET_RESP_STATUS_CODE(handle, tmp)    handle->resp_status_code = tmp
#define WIFIDIRECT_SET_DISC_STATUS_CODE(handle, tmp)    handle->disc_status_code = tmp
#define WIFIDIRECT_SET_BONJOUR_RECORD_PTR(handle, tmp)  handle->disc_data.u.bonjour.record = tmp

#define WIFIDIRECT_SET_GAS_FRAGMENT_ID_REPLY(handle, tmp)   handle->gas_fragment_id = tmp


/** Function Prototype Declaration */

/**
 * @brief Create Wifi-Direct Configuration
 *
 * @param config    Pointer to an allocated buffer with 
 *                  configuration
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_config(wifidirect_config_t **config);

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
wifidirect_delete_config(wifidirect_config_t **config);

/**
 * @brief Create Wifi-Direct Parameter Configuration
 *
 * @param param_config  Pointer to an allocated buffer with 
 *                      parameter configuration
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_param_config(wifidirect_param_config_t **param_config);

/**
 * @brief Delete Wifi-Direct Parameter Configuration
 *
 * @param param_config  Pointer to a previously allocated buffer
 *                      with parameter configuration from
 *                      wifidirect_create_param_config()
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_delete_param_config(wifidirect_param_config_t **param_config);

/**
 * @brief Create Wifi-Direct Action Frame
 *
 * @param param_config  Pointer to an allocated buffer with 
 *                      action frame
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_action_frame(wifidirect_action_frame_t **action_frame);

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
wifidirect_delete_action_frame(wifidirect_action_frame_t **action_frame);

/**
 * @brief Create Wifi-Direct Discovery Request
 *
 * @param param_config  Pointer to an allocated buffer for
 *                      discovery request
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_discovery_request(wifidirect_discovery_request_t **discovery_request);

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
wifidirect_delete_discovery_request(wifidirect_discovery_request_t **discovery_request);


/**
 * @brief Create Wifi-Direct Discovery Response
 *
 * @param param_config  Pointer to an allocated buffer for
 *                      discovery response
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_discovery_response(wifidirect_discovery_response_t **discovery_response);

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
wifidirect_delete_discovery_response(wifidirect_discovery_response_t **discovery_response);

/**
 * @brief Create Wifi-Direct Gas Comeback Request
 *
 * @param param_config  Pointer to an allocated buffer for
 *                      Gas Comeback request
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_gas_comeback_request(wifidirect_gas_comeback_request_t **gas_comeback_request);

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
wifidirect_delete_gas_comeback_request(wifidirect_gas_comeback_request_t **gas_comeback_request);

/**
 * @brief Create Wifi-Direct Gas Comeback Response
 *
 * @param param_config  Pointer to an allocated buffer for
 *                      Gas Comeback response
 *
 * @return OK or FAIL.
 */
error_type_t
wifidirect_create_gas_comeback_response(wifidirect_gas_comeback_response_t **gas_comeback_response);

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
wifidirect_delete_gas_comeback_response(wifidirect_gas_comeback_response_t **gas_comeback_response);

/** 
 *  @brief wifidirect_gas_comeback_service_discovery_request
 *
 *  @param gas_comeback_request Pointer to gas comeback request structure
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirectcmd_gas_comeback_discovery_request(wifidirect_gas_comeback_request_t *gas_comeback_request);

/** 
 *  @brief wifidirect_gas_comeback_service_discovery_response
 *
 *  @param gas_comeback_request Pointer to gas comeback response structure
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirectcmd_gas_comeback_discovery_response(wifidirect_gas_comeback_response_t *gas_comeback_response);

/** 
 *  @brief wifidirect_service_discovery request
 *
 *  @param discovery_request    Pointer to discovery request
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirectcmd_service_discovery_request(wifidirect_discovery_request_t *discovery_request);

/** 
 *  @brief wifidirect_service_discovery response
 *
 *  @param discovery_response   Pointer to discovery response
 *  
 *  @return         OK or FAIL
 **/
error_type_t
wifidirectcmd_service_discovery_response(wifidirect_discovery_response_t *discovery_response);

/** 
 *  @brief wifidirect_config request
 *
 *  @param config   Pointer to configuration structure
 *  @param ie_index IE Index
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirectcmd_config(wifidirect_config_t *config, t_u16 ie_index);

/** 
 *  @brief Set wifidirect_params_config
 *
 *  @param  param_config    Pointer to parameter configuration
 *                          struct
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirectcmd_params_config(wifidirect_param_config_t *param_config);

/** 
 *  @brief wifidirect_action_frame request
 *
 *  @param action_frame     Pointer to Action Frame Struct
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirectcmd_action_frame(wifidirect_action_frame_t * action_frame);

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
wifidirectcmd_set_status(t_u16 status);

/** 
 *  @brief Get wifidirect mode status (start or stop requests)
 *
 *
 *   STATUS Modes:         0 - start wifidirect status
 *                         1 - stop  wifidirect status
 *                         2 - start wifidirect group owner mode
 *                         3 - start wifidirect client mode
 *                         4 - start wifidirect find phase
 *                         5 - stop  wifidirect find phase
 *
 *  @param status   Wifi-Direct mode status command pointer
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirectcmd_get_status(t_u16 *status);

/** 
 *  @brief Set wifidirect discovery period
 *
 *  @param min_disc_interval     Minimum Discovery Interval
 *  @param max_disc_interval     Maximum Discovery Interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_discovery_period(t_u16 min_disc_interval, t_u16 max_disc_interval);

/** 
 *  @brief Get wifidirect discovery period
 *
 *  @param min_disc_interval     Pointer to Minimum Discovery Interval
 *  @param max_disc_interval     Pointer to Maximum Discovery Interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_discovery_period(t_u16 *min_disc_interval, t_u16 *max_disc_interval);

/** 
 *  @brief Set wifidirect Intent
 *
 *  @param intent   Wifi-Direct Intent
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_intent(t_u8 intent);

/** 
 *  @brief Get wifidirect Intent
 *
 *  @param intent   Pointer Wifi-Direct Intent
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_intent(t_u8 *intent);

/** 
 *  @brief Set wifidirect ListenChannel
 *
 *  @param listen_channel   Wifi-Direct Listen Channel
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_set_cfg_cmd_listen_channel(t_u8 listen_channel);

/** 
 *  @brief Get wifidirect ListenChannel
 *
 *  @param listen_channel   Pointer to Wifi-Direct Listen Channel
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_get_cfg_cmd_listen_channel(t_u8 *listen_channel);

/** 
 *  @brief Set wifidirect OperatingChannel
 *
 *  @param operating_channel    Wifi-Direct Operating Channel
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_set_cfg_cmd_op_channel(t_u8 operating_channel);

/** 
 *  @brief Get wifidirect OperatingChannel
 *
 *  @param operating_channel    Pointer to Wifi-Direct Operating Channel
 *  
 *  @return         OK or FAIL
 */
error_type_t
wifidirect_get_cfg_cmd_op_channel(t_u8 *operating_channel);

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
                           t_u32 interval);

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
                           t_u32 *interval);

/** 
 *  @brief Set wifidirect Opportunistic Power Save
 *
 *  @param enable_ps    Enable/Disable boolean
 *  @param ctwindow_val ctwindow
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_opp_ps(bool enable_opp_ps, t_u8 ctwindow_val);

/** 
 *  @brief Get wifidirect Opportunistic Power Save
 *
 *  @param enable_ps    Pointer to Enable/Disable boolean
 *  @param ctwindow_val Pointer to ctwindow
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_opp_ps(bool *enable_opp_ps, t_u8 *ctwindow_val);

/** 
 *  @brief Set wifidirect Capability
 *  
 *  @param dev_capability       Dev Capability
 *  @param group_capability     Group Capability
 *  
 *  @return         None 
 */
error_type_t
wifidirect_set_cfg_cmd_capability(t_u8 dev_capability, t_u8 group_capability);

/** 
 *  @brief Get wifidirect Capability
 *  
 *  @param dev_capability       Pointer to Dev Capability
 *  @param group_capability     Pointer to Group Capability
 *  
 *  @return         None 
 */
error_type_t
wifidirect_get_cfg_cmd_capability(t_u8 *dev_capability, t_u8 *group_capability);

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
                                               char *peermac2);

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
                                               char *peermac2);

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
wifidirect_set_cfg_cmd_persistent_group_invoke_cancel(t_u8 index);

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
                                           t_u32 interval);

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
                                           t_u32 *interval);

/** 
 *  @brief Set wifidirect extended listen time parameters
 *
 *  @param duration Duration
 *  @param interval Interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_ext_listen_time(t_u16 duration, t_u16 interval);

/** 
 *  @brief Get wifidirect extended listen time parameters
 *
 *  @param duration Pointer to duration
 *  @param interval Pointer to interval
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_ext_listen_time(t_u16 *duration, t_u16 *interval);

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
                                           t_u16 dev_password);

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
                                           t_u16 *dev_password);

/** 
 *  @brief Set wifidirect WPS parametters 
 *
 *  @param wps_pin_pbc  WPS Pin (1) or PBC (2)
 *  
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_set_cfg_cmd_wps_params(int wps_pin_pbc);

/** 
 *  @brief Get wifidirect WPS parametters 
 *
 *  @param wps_pin_pbc  Pointer to WPS Pin (1) or PBC (2)
 *  @return         OK or FAIL 
 */
error_type_t
wifidirect_get_cfg_cmd_wps_params(int *wps_pin_pbc);

/**
 * @brief WFD Configure
 *
 * @param N/A
 *
 * @return OK or FAIL.
 */
error_type_t
wfd_configure(void);

/**
 * @brief WFD Parameters Configure
 *
 * @param N/A
 *
 * @return OK or FAIL.
 */
error_type_t
wfd_params_configure(void);

/**
 * @brief Get WFD configuration
 *
 * @param N/A
 *
 * @return Pointer to the wfd configuration struct.
 */
wifidirect_config_t *
get_wfd_config(void);

/**
 * @brief Get WFD Parameters configuration
 *
 * @param N/A
 *
 * @return Pointer to the wfd params configuration struct.
 */
wifidirect_param_config_t *
get_wfd_params_config(void);

#endif //WIFIDIRECT_API_H
