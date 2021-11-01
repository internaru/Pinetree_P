/*
 * ============================================================================
 * Copyright (c) 2008-2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * \file wifi_debug_serial.c
 * 
 * \brief Wireless's support for commands
 * 
 * Provides the interface that allows developers to control wireless settings
 * via the command interface.
 * 
 **/

#include "trsocket.h"
#include "wlanInterface.h"
#include "wifi_intf.h"
#include "wifi_intf_api.h"
#include "wifi_intf_link_api.h"
#include "wifi_intf_event_api.h"
#include "wifi_intf_vars.h"
#include "wifi_intf_config.h"
#include "memAPI.h"
#include "utils.h"
#include "tx_api.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h> // for atoi
#include "wifi_config.h"
#include "platform.h"
#include "uuid.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#ifdef HAVE_WIFI_CERT_TEST
#include "sigma_interface.h"
#endif

#ifdef WIFI_DIRECT_SUPPORT
#include "wfd_api.h"
#include "model_info.h"
#endif

static void WifiShowUUID(void)
{
    char device_uuid[UUID_STRING_LENGTH] = "";

    // UUID
    get_device_uuid_string(device_uuid, sizeof(device_uuid));
    cmd_printf( "\nUUID: %s\n\n", device_uuid);
}


static void WifiIfaceShowInfo(int iface_id)
{
    wifi_intf_security_mode_t sec_mode;
    bool     enabled;
    uint8_t  ssid[MAX_SSID_NAME_LEN+1];    
    uint8_t  ssid_prefix[MAX_SSID_NAME_LEN+1];
    uint8_t  role = 0;

    switch(iface_id)
    {
        case WIFI_IFACE_ID_STATION:
            cmd_printf("\n  Station:\n");
            break;
        case WIFI_IFACE_ID_UAP:
            cmd_printf("\n  Micro-AP:\n");
            break;
#ifdef WIFI_DIRECT_SUPPORT
        case WIFI_IFACE_ID_WFD:
            cmd_printf("\n  Wifi-Direct:\n");
            wifi_intf_get_var_wfd_bss_role(iface_id, &role);
            break;
#endif
        case WIFI_IFACE_ID_UAP_PRI0:
            cmd_printf("\n  Micro-AP Pri0:\n");
            break;

        default:
            cmd_printf("\n  ?? Requested info for invalid interface ID ??\n");
            return;
    }

    wifi_intf_get_var_is_enabled(iface_id, &enabled);
    wifi_intf_get_var_ssid(iface_id, ssid, MAX_SSID_NAME_LEN);
    wifi_intf_get_var_sec_mode(iface_id, &sec_mode);

    cmd_printf("    Currently: %s\n", enabled ? "Enabled" : "Disabled" );
    if (iface_id == WIFI_IFACE_ID_UAP || iface_id == WIFI_IFACE_ID_UAP_PRI0)
    {
        wifi_intf_get_var_ssid_prefix(iface_id, ssid_prefix, MAX_SSID_NAME_LEN);
    }
#ifdef WIFI_DIRECT_SUPPORT
    else if (iface_id == WIFI_IFACE_ID_WFD)
    {
        if (role == WIFIDIRECT_GO_ROLE)
            cmd_printf("    Role: Autonomous Group Owner (GO)\n");
        else if (role == WIFIDIRECT_CLIENT_ROLE)
            cmd_printf("    Role: Client\n");
        else
            cmd_printf("    Role: Unknown\n");

        wifi_intf_get_var_ssid_prefix(iface_id, ssid_prefix, MAX_SSID_NAME_LEN);

        if ((enabled) && (role == WIFIDIRECT_GO_ROLE))
        {
            if ((strlen((char *)ssid) == 0) || (strcmp((char *)ssid, " ") == 0))
            {    
                char *tmpstr;
                wifi_get_active_wfd_ssid_prefix((char*)ssid_prefix);                               
                get_model_number(&tmpstr);
                strncpy((char *)&ssid[0], tmpstr, MAX_SSID_NAME_LEN);                  
                wifi_intf_set_var_ssid(iface_id, ssid, MAX_SSID_NAME_LEN);
            }    
        }            
    }
#endif
    else
    {
        strcpy((char*)ssid_prefix, "");
    }
    cmd_printf("    SSID: %s%s\n", ssid_prefix, (strlen((char*)ssid) > 0) ? (char*)ssid : "[Unconfigured]");
    if (iface_id == WIFI_IFACE_ID_STATION)
    {
        wifi_intf_comm_mode_t     comm_mode;
        int32_t                   rssi;

        wifi_intf_get_var_comm_mode(iface_id, &comm_mode);
        cmd_printf("    Mode: %s\n", comm_mode == AD_HOC ? "Ad Hoc" : "Infrastructure");

        if (wifi_intf_get_var_rssi_dbm(iface_id, &rssi) == WIFI_INTF_OK)
        {
            cmd_printf("    Signal[1-5]: %d    (dbm: %ld)\n", rssi_dbm_to_signal_strength(rssi), rssi);
        }
    }
    cmd_printf("    Security: ");
    switch(sec_mode)
    {
        case WPA_PSK:
        case WPA2_PSK:
        {
            uint8_t passphrase[MAX_PASS_PHRASE + 1];

            wifi_intf_get_var_wpa_passphrase(iface_id, passphrase, MAX_PASS_PHRASE + 1);

            cmd_printf("WPA/WPA2 ");
            cmd_printf("[PassPhrase: %s]\n", passphrase);
            break;
        }
        case WEP : 
        {
            wifi_intf_wep_auth_mode_t auth_mode;
            uint8_t cur_key;

            wifi_intf_get_var_wep_auth_mode(iface_id, &auth_mode);
            wifi_intf_get_var_wep_cur_key(iface_id, &cur_key);

            cmd_printf("WEP ");
            if ( auth_mode == AUTH_SHARED_KEY )
            {
                cmd_printf("(Shared Auth" );
            }
            else if (auth_mode == AUTH_OPEN )
            {
                cmd_printf("(Open Auth" );
            }
            else if ((auth_mode == AUTH_AUTO) && (iface_id == WIFI_IFACE_ID_STATION))
            {
                cmd_printf("(Auto Auth" );
            }
            else
            {
                cmd_printf("(Unknown Auth" );
            }

            cmd_printf(" using key %d)\n", cur_key );

            // TODO: show keys
            break;
        }
        default:    // Intentionally fall through
        case OPEN_SYSTEM: cmd_printf("Open Access\n"); break;
    }

    if (((iface_id == WIFI_IFACE_ID_WFD) && (role == WIFIDIRECT_GO_ROLE)) || (iface_id == WIFI_IFACE_ID_UAP) || (iface_id == WIFI_IFACE_ID_UAP_PRI0))
    {
        wifi_intf_mac_filter_mode_t filter_mode;
        uint8_t uap_active_config;
        bool hw_active;
        bool display_info = false;

        wifi_intf_get_var_mac_filter_mode(iface_id, &filter_mode);

        cmd_printf("    MAC Filter mode: ");

        if (filter_mode == MAC_FILTER_MODE_DISABLED)
            cmd_printf("Disabled\n");            
        else if (filter_mode == MAC_FILTER_MODE_WHITELIST)
            cmd_printf("Whitelist\n");
        else if (filter_mode == MAC_FILTER_MODE_BLACKLIST)
            cmd_printf("Blacklist\n");
            
        wifi_hw_get_var_is_active(&hw_active);

        uap_active_config = wifi_get_uap_active_config();
        
        if (hw_active && enabled)
            display_info = true;
        if ((iface_id == WIFI_IFACE_ID_UAP) || (iface_id == WIFI_IFACE_ID_UAP_PRI0))
        {
            if (iface_id != uap_active_config)                
                display_info = false;
        }    
         
        if (display_info)
        {
            uint8_t  chan;
            wifi_intf_get_var_channel(iface_id, &chan);
        
            cmd_printf("    Current Channel: %d\n", chan);
            cmd_printf("    Default Channel: %d\n", WIFI_UAP_CHANNEL_DEFAULT);
        
            uap_mlan_ds_sta_list *sta_list = MEM_MALLOC(sizeof(uap_mlan_ds_sta_list));
            if (sta_list != NULL)
            {
                if (wifi_intf_get_var_station_list(iface_id, sta_list) < 0)
                {
                    cmd_printf("    (Error) Could not retrieve station list\n" );
                }
                else
                {
                    int i;
                    cmd_printf("    Attached Stations\n");
                    if (sta_list->sta_count == 0)
                    {
                        cmd_printf("      (none)\n");
                    }
                    else
                    {
                        for (i = 0; i < sta_list->sta_count; i++)
                        {
                            cmd_printf("      %02x:%02x:%02x:%02x:%02x:%02x    RSSI: %d\n",
                                    sta_list->info[i].mac_address[0], sta_list->info[i].mac_address[1],
                                    sta_list->info[i].mac_address[2], sta_list->info[i].mac_address[3],
                                    sta_list->info[i].mac_address[4], sta_list->info[i].mac_address[5], 
                                    sta_list->info[i].rssi);
                        }
                    }
                }
                MEM_FREE_AND_NULL(sta_list);
            }
        }

        cmd_printf("    MAC Filter List:\n");
        uint8_t MACFilterList[MAX_FILTERED_ADDRS][MAC_ADDR_LENGTH];
        uint8_t list_count; 
        int i; 

        wifi_intf_get_var_mac_filter_list(iface_id, MACFilterList);
        wifi_intf_get_var_mac_filter_count(iface_id, &list_count);

        for (i = 0; i< list_count ; i++)
        {
            cmd_printf("      %02x:%02x:%02x:%02x:%02x:%02x\n",
                                    MACFilterList[i][0], MACFilterList[i][1],
                                    MACFilterList[i][2], MACFilterList[i][3],
                                    MACFilterList[i][4], MACFilterList[i][5]);                           
        }    
    }        
}

static void WifiHWShowInfo( void )
{
    uint8_t macOnBoard[MAC_ADDR_LENGTH];
    bool hw_enabled;
    bool hw_active;
    bool radio_present;
    uint8_t region_code;
    uint8_t wws_config;
    uint8_t uap_active_config;

    wifi_hw_get_var_is_enabled(&hw_enabled);
    wifi_hw_get_var_is_active(&hw_active);
    wifi_hw_get_var_is_radio_present(&radio_present);
    wifi_hw_get_var_region_code(&region_code);
    wifi_hw_get_var_wws_config(&wws_config);

    cmd_printf("\nWifi status:\n\n");

    if (!radio_present)
    {
        cmd_printf("  WARNING: WIRELESS BOARD IS NOT ENUMERATED\n");
    }
    cmd_printf("  Radio is %s", hw_enabled ? "Enabled" : "Disabled");
    cmd_printf(" (power is %s)\n", hw_active ? "on" : "off");
    cmd_printf("  Region code : ");
    if (region_code == REG_US)
        cmd_printf(" US\n");
    else if (region_code == REG_CAN)
        cmd_printf(" Canada\n");
    else if (region_code == REG_EUR)
        cmd_printf(" Europe\n");
    else if (region_code == REG_ESP)
        cmd_printf(" Spain\n");
    else if (region_code == REG_FRA)
        cmd_printf(" France\n");
    else if (region_code == REG_JPN)
        cmd_printf(" Japan\n");
    else
        cmd_printf(" US\n");

    cmd_printf("  World Wide Safe (WWS) : ");
    if (wws_config)
        cmd_printf(" Enabled\n");
    else
        cmd_printf(" Disabled\n");

    wifi_intf_get_var_mac_addr(WIFI_IFACE_ID_STATION, macOnBoard, MAC_ADDR_LENGTH);
    cmd_printf("\n  STA MAC: ");
    cmd_printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                                     macOnBoard[0], macOnBoard[1], macOnBoard[2],
                                     macOnBoard[3], macOnBoard[4], macOnBoard[5]);

    WifiIfaceShowInfo(WIFI_IFACE_ID_STATION);

    wifi_intf_get_var_mac_addr(WIFI_IFACE_ID_UAP, macOnBoard, MAC_ADDR_LENGTH);
    uap_active_config = wifi_get_uap_active_config();
    cmd_printf("\n  Currently active uAP configuration: ");
    if (uap_active_config == WIFI_IFACE_ID_UAP)
        cmd_printf(" uAP\n");
    else
        cmd_printf(" uAP_Pri0\n");

    cmd_printf("\n  UAP MAC: ");
    cmd_printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                                     macOnBoard[0], macOnBoard[1], macOnBoard[2],
                                     macOnBoard[3], macOnBoard[4], macOnBoard[5]);

    WifiIfaceShowInfo(WIFI_IFACE_ID_UAP);

    wifi_intf_get_var_mac_addr(WIFI_IFACE_ID_UAP_PRI0, macOnBoard, MAC_ADDR_LENGTH);
    cmd_printf("\n  UAP Pri0 MAC: ");
    cmd_printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                                     macOnBoard[0], macOnBoard[1], macOnBoard[2],
                                     macOnBoard[3], macOnBoard[4], macOnBoard[5]);

    WifiIfaceShowInfo(WIFI_IFACE_ID_UAP_PRI0);
#ifdef WIFI_DIRECT_SUPPORT
    wifi_intf_get_var_mac_addr(WIFI_IFACE_ID_WFD, macOnBoard, MAC_ADDR_LENGTH);
    cmd_printf("\n  WFD MAC: ");
    cmd_printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                                     macOnBoard[0], macOnBoard[1], macOnBoard[2],
                                     macOnBoard[3], macOnBoard[4], macOnBoard[5]);

    WifiIfaceShowInfo(WIFI_IFACE_ID_WFD);
#endif

    WifiShowUUID();
}

static const char *wireless_cmd_wps_cancel_desc = "Cancel WPS if it is running";
static const char *wireless_cmd_wps_cancel_usage = NULL;
static const char *wireless_cmd_wps_cancel_notes = NULL;
static int wireless_cmd_wps_cancel_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_intf_stop_wps();

    return CMD_OK;
}

static const char *wireless_cmd_sta_wps_start_desc = "Run wps enrollee (optionally specify pin and ssid)";
static const char *wireless_cmd_sta_wps_start_usage = "[ssid <ssid>] [pin <pin>]";
static const char *wireless_cmd_sta_wps_start_notes = "<ssid> names with spaces must be escaped with a slash (\\)";
static int wireless_cmd_sta_wps_start_cb(int argc, char *argv[])
{
    uint32_t pos;
    char *ssid = NULL;
    int   ssid_len = 0;
    char *pin  = NULL;
    int   pin_len = 0;

    pos = 1;

    if ( argc > 1 )
    {
        while (pos < argc)
        {
            if (strcmp("pin", argv[pos]) == 0)
            {
                pos++;
                if (pos < argc)
                {
                    pin = argv[pos];
                    pin_len = strlen(pin);
                }
                else
                {
                    return CMD_USAGE_ERROR;
                }
            }
            else if (strcmp("ssid", argv[pos]) == 0)
            {
                pos++;
                if (pos < argc)
                {
                    ssid = argv[pos];
                    ssid_len = strlen(ssid);
                }
                else
                {
                    return CMD_USAGE_ERROR;
                }
            }
            else
            {
                return CMD_USAGE_ERROR;
            }

            // Check the next term
            pos++;
        }
    }

    wifi_intf_start_wps((uint8_t *)ssid, ssid_len, (uint8_t *)pin, pin_len);

    return CMD_OK;
}

static const char *wireless_cmd_sta_wps_genpin_desc = "Generate WPS PIN";
static const char *wireless_cmd_sta_wps_genpin_usage = NULL;
static const char *wireless_cmd_sta_wps_genpin_notes = NULL;
static int wireless_cmd_sta_wps_genpin_cb(int argc, char *argv[])
{
    cmd_printf("Generated WPS PIN is : %s\n", generate_wps_pin());	
    return CMD_OK;	
}

static const char *wireless_cmd_uap_wps_start_desc = "Run wps registrar";
static const char *wireless_cmd_uap_wps_start_usage = NULL;
static const char *wireless_cmd_uap_wps_start_notes = NULL;
static int wireless_cmd_uap_wps_start_cb(int argc, char *argv[])
{
    uint32_t pos;
    char *pin  = NULL;
    int   pin_len = 0;

    pos = 1;

    if ( argc > 1 )
    {
        while (pos < argc)
        {
            if (strcmp("pin", argv[pos]) == 0)
            {
                pos++;
                if (pos < argc)
                {
                    pin = argv[pos];
                    pin_len = strlen(pin);
                }
                else
                {
                    return CMD_USAGE_ERROR;
                }
            }
            else
            {
                return CMD_USAGE_ERROR;
            }

            // Check the next term
            pos++;
        }
    }

    wirelessStartWPSRegistrar(NULL);
    return CMD_OK;
}

#ifdef WIFI_DIRECT_SUPPORT
static const char *wireless_cmd_wfd_wps_start_desc = "Run wps registrar";
static const char *wireless_cmd_wfd_wps_start_usage = NULL;
static const char *wireless_cmd_wfd_wps_start_notes = NULL;
static int wireless_cmd_wfd_wps_start_cb(int argc, char *argv[])
{
    uint32_t pos;
    char *pin  = NULL;
    int   pin_len = 0;

    pos = 1;

    if ( argc > 1 )
    {
        while (pos < argc)
        {
            if (strcmp("pin", argv[pos]) == 0)
            {
                pos++;
                if (pos < argc)
                {
                    pin = argv[pos];
                    pin_len = strlen(pin);
                }
                else
                {
                    return CMD_USAGE_ERROR;
                }
            }
            else
            {
                return CMD_USAGE_ERROR;
            }

            // Check the next term
            pos++;
        }
    }

    wirelessStartWFDWPSRegistrar(NULL);
    return CMD_OK;
}
#endif

static const char *wireless_cmd_intf_enable_desc = "enable this interface";
static const char *wireless_cmd_intf_enable_usage = NULL;
static const char *wireless_cmd_intf_enable_notes = NULL;
static int wireless_cmd_intf_enable_cb(int argc, char *argv[], int iface_id)
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_intf_set_var_is_enabled(iface_id, true);

    return CMD_OK;
}

static int wireless_cmd_uap_enable_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_intf_enable_cb( argc, argv, iface_id );

    return i_res;
}

#ifdef WIFI_DIRECT_SUPPORT
static int wireless_cmd_wfd_enable_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;
    bool enabled = FALSE;    

    iface_id = WIFI_IFACE_ID_WFD;

    i_res = wireless_cmd_intf_enable_cb( argc, argv, iface_id );

    wifi_intf_get_var_is_enabled(iface_id, &enabled);

    cmd_printf("WiFi WFD: %s\n", enabled ? "Enabled" : "Disabled" );

    return i_res;
}
#endif

static int wireless_cmd_sta_enable_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;
    bool enabled = FALSE;

    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_intf_enable_cb( argc, argv, iface_id );
    
    wifi_intf_get_var_is_enabled(iface_id, &enabled);

    cmd_printf("WiFi STA: %s\n", enabled ? "Enabled" : "Disabled" );
 
    return i_res;
}

static const char *wireless_cmd_intf_disable_desc = "disable this interface";
static const char *wireless_cmd_intf_disable_usage = NULL;
static const char *wireless_cmd_intf_disable_notes = NULL;
static int wireless_cmd_intf_disable_cb(int argc, char *argv[], int iface_id)
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_intf_set_var_is_enabled(iface_id, false);

    return CMD_OK;
}

static int wireless_cmd_uap_disable_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_intf_disable_cb( argc, argv, iface_id );

    return i_res;
}

#ifdef WIFI_DIRECT_SUPPORT
static int wireless_cmd_wfd_disable_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;
    bool enabled = TRUE;    

    iface_id = WIFI_IFACE_ID_WFD;

    i_res = wireless_cmd_intf_disable_cb( argc, argv, iface_id );

    wifi_intf_get_var_is_enabled(iface_id, &enabled);

    cmd_printf("WiFi WFD: %s\n", enabled ? "Enabled" : "Disabled" );    

    return i_res;
}
#endif

static int wireless_cmd_sta_disable_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;
    bool enabled = TRUE;
    
    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_intf_disable_cb( argc, argv, iface_id );
    
    wifi_intf_get_var_is_enabled(iface_id, &enabled);

    cmd_printf("WiFi STA: %s\n", enabled ? "Enabled" : "Disabled" );    

    return i_res;
}

static const char *wireless_cmd_intf_defaults_desc = "Restore the default settings";
static const char *wireless_cmd_intf_defaults_usage = NULL;
static const char *wireless_cmd_intf_defaults_notes = NULL;
static int wireless_cmd_intf_defaults_cb(int argc, char *argv[], int iface_id)
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_intf_var_restore_defaults(iface_id, WIFI_INTF_VAR_ALL);

    return CMD_OK;
}

static int wireless_cmd_uap_defaults_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_intf_defaults_cb( argc, argv, iface_id );

    if ( CMD_OK == i_res )
    {
        notify_wifi_uap_vars_changed();
    }

    return i_res;
}

#ifdef WIFI_DIRECT_SUPPORT
static int wireless_cmd_wfd_defaults_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_WFD;

    i_res = wireless_cmd_intf_defaults_cb( argc, argv, iface_id );

    if ( CMD_OK == i_res )
    {
        notify_wifi_wfd_vars_changed();
    }

    return i_res;
}
#endif

static int wireless_cmd_sta_defaults_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_intf_defaults_cb( argc, argv, iface_id );

    if ( CMD_OK == i_res )
    {
        notify_wifi_sta_vars_changed();
    }

    return i_res;
}

static const char *wireless_cmd_intf_info_desc = "Print current values of assorted configuration info";
static const char *wireless_cmd_intf_info_usage = NULL;
static const char *wireless_cmd_intf_info_notes = NULL;

static int wireless_cmd_uap_info_cb(int argc, char *argv[])
{
    int iface_id;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP;
    WifiIfaceShowInfo(iface_id);

    return CMD_OK;
}

#ifdef WIFI_DIRECT_SUPPORT
static int wireless_cmd_wfd_info_cb(int argc, char *argv[])
{
    int iface_id;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_WFD;
    WifiIfaceShowInfo(iface_id);

    return CMD_OK;
}
#endif

static int wireless_cmd_sta_info_cb(int argc, char *argv[])
{
    int iface_id;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_STATION;
    WifiIfaceShowInfo(iface_id);

    return CMD_OK;
}

static const char *wireless_cmd_ssid_desc = "Set (or view) the current SSID";
static const char *wireless_cmd_ssid_usage = "[<ssid>]";
static const char *wireless_cmd_ssid_notes = NULL;
static int wireless_cmd_ssid_cb(int argc, char *argv[], int iface_id)
{
    uint8_t temp_data[128];

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc )
    {
        wifi_intf_set_var_ssid(iface_id, (uint8_t*)argv[1], strlen(argv[1]) + 1);
    }

    /* get */
    wifi_intf_get_var_ssid(iface_id, temp_data, sizeof(temp_data));
    cmd_append_result("%s", temp_data);

    return CMD_OK;
}

static int wireless_cmd_uap_ssid_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_ssid_cb( argc, argv, iface_id );

    return i_res;
}

#ifdef WIFI_DIRECT_SUPPORT
static int wireless_cmd_wfd_ssid_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_WFD;

    i_res = wireless_cmd_ssid_cb( argc, argv, iface_id );

    return i_res;
}
#endif

static int wireless_cmd_sta_ssid_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_ssid_cb( argc, argv, iface_id );

    return i_res;
}

static const char *wireless_cmd_ssid_prefix_desc = "Set (or view) the SSID prefix";
static const char *wireless_cmd_ssid_prefix_usage = "[<ssid_prefix>]";
static const char *wireless_cmd_ssid_prefix_notes = NULL;
static int wireless_cmd_ssid_prefix_cb(int argc, char *argv[], int iface_id)
{
    uint8_t temp_data[128];

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc )
    {
        wifi_intf_set_var_ssid_prefix(iface_id, (uint8_t*)argv[1], strlen(argv[1]) + 1);
    }

    /* get */
    wifi_intf_get_var_ssid_prefix(iface_id, temp_data, sizeof(temp_data));
    cmd_append_result("%s", temp_data);

    return CMD_OK;
}

static int wireless_cmd_uap_ssid_prefix_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_ssid_prefix_cb( argc, argv, iface_id );

    return i_res;
}

#ifdef WIFI_DIRECT_SUPPORT
static int wireless_cmd_wfd_ssid_prefix_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_WFD;

    i_res = wireless_cmd_ssid_prefix_cb( argc, argv, iface_id );

    return i_res;
}
#endif

static const char *wireless_cmd_security_desc = "Set (or view) the security mode";
static const char *wireless_cmd_security_usage = "[open|wep|wpa|wpa2]";
static const char *wireless_cmd_security_notes = NULL;
static int wireless_cmd_security_cb(int argc, char *argv[], int iface_id)
{
    wifi_intf_security_mode_t sec_mode;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc )
    {
        if (strcmp( "wep", argv[1] ) == 0)
        {
            sec_mode = WEP;
        }
        else if (strcmp( "wpa", argv[1] ) == 0)
        {
            sec_mode = WPA_PSK;
        }
        else if (strcmp( "wpa2", argv[1] ) == 0)
        {
            sec_mode = WPA2_PSK;
        }
        else if (strcmp( "open", argv[1] ) == 0)
        {
            sec_mode = OPEN_SYSTEM;
        }
        else
        {
            return CMD_USAGE_ERROR;
        }

        wifi_intf_set_var_sec_mode(iface_id, sec_mode);
    }

    /* get */
    wifi_intf_get_var_sec_mode(iface_id, &sec_mode);

    switch(sec_mode)
    {
        case OPEN_SYSTEM: cmd_append_result("open"); break;
        case WEP        : cmd_append_result("wep"); break;
        case WPA_PSK    : cmd_append_result("wpa"); break;
        case WPA2_PSK   : cmd_append_result("wpa2"); break;
        default         : cmd_append_result("error (%d)", sec_mode); break;
    }

    return CMD_OK;
}

static int wireless_cmd_uap_security_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_security_cb( argc, argv, iface_id );

    return i_res;
}

#ifdef WIFI_DIRECT_SUPPORT
static int wireless_cmd_wfd_security_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_WFD;

    i_res = wireless_cmd_security_cb( argc, argv, iface_id );

    return i_res;
}
#endif

static int wireless_cmd_sta_security_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_security_cb( argc, argv, iface_id );

    return i_res;
}

static const char *wireless_cmd_wep_auth_mode_desc = "Set (or view) the wep auth mode";
static const char *wireless_cmd_wep_auth_mode_usage = "[shared|open|auto]";
static const char *wireless_cmd_wep_auth_mode_notes = NULL;
static int wireless_cmd_wep_auth_mode_cb(int argc, char *argv[], int iface_id)
{
    wifi_intf_wep_auth_mode_t auth_mode;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc )
    {
        if ( strcmp( "shared", argv[1] ) == 0)
        {
            auth_mode = AUTH_SHARED_KEY;
        }
        else if ( strcmp( "open", argv[1] ) == 0)
        {
            auth_mode = AUTH_OPEN;
        }
        else if ( strcmp( "auto", argv[1] ) == 0)
        {
            auth_mode = AUTH_AUTO;
        }
        else
        {
            return CMD_USAGE_ERROR;
        }
        wifi_intf_set_var_wep_auth_mode(iface_id, auth_mode);
    }

    /* get */
    wifi_intf_get_var_wep_auth_mode(iface_id, &auth_mode);

    switch(auth_mode)
    {
        case AUTH_OPEN       : cmd_append_result("open"); break;
        case AUTH_SHARED_KEY : cmd_append_result("shared"); break;
        case AUTH_AUTO       : cmd_append_result("auto"); break; 
        default              : cmd_append_result("error (%d)", auth_mode); break;
    }

    return CMD_OK;
}

static int wireless_cmd_uap_wep_auth_mode_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_wep_auth_mode_cb( argc, argv, iface_id );

    return i_res;
}

static int wireless_cmd_sta_wep_auth_mode_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_wep_auth_mode_cb( argc, argv, iface_id );

    return i_res;
}

static const char *wireless_cmd_wep_key_desc = "Set (or view) wep key";
static const char *wireless_cmd_wep_key_usage = "<index> [<key value>]";
static const char *wireless_cmd_wep_key_notes = "<index> valid range is 0-3";
static int wireless_cmd_wep_key_cb(int argc, char *argv[], int iface_id)
{
    uint8_t temp_data[128];
    int which_key;

    if ( ( argc != 2 ) && ( argc != 3 ) )
    {
        return CMD_USAGE_ERROR;
    }

    which_key = atoi(argv[1]);

    if ( which_key < 0 || which_key > 3)
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 3 == argc )
    {
        if (strlen(argv[2]) > 0)
        {
            wifi_intf_set_var_sec_mode(iface_id, WEP);
            wifi_intf_set_var_wep_key(iface_id, which_key, (uint8_t*)argv[2], strlen(argv[2]) + 1);
            wifi_intf_set_var_wep_key_len(iface_id, strlen(argv[2]));
        }
        else
        {
            /* TODO ask Ray if this if makes sense, if statement doesn't seem logical, this code should
             * never be executed */
            wifi_intf_set_var_sec_mode(iface_id, OPEN_SYSTEM);
            wifi_intf_set_var_wep_key(iface_id, which_key, (uint8_t*)argv[2], strlen(argv[2]) + 1);
        }
    }

    /* get */
    wifi_intf_get_var_wep_key(iface_id, which_key, temp_data, sizeof(temp_data));
    cmd_append_result("WEP Key[%d]: %s", which_key, temp_data);

    return CMD_OK;
}

static int wireless_cmd_uap_wep_key_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_wep_key_cb( argc, argv, iface_id );

    return i_res;
}

static int wireless_cmd_sta_wep_key_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_wep_key_cb( argc, argv, iface_id );

    return i_res;
}

static const char *wireless_cmd_wep_cur_key_desc = "Set (or view) index of active wep key";
static const char *wireless_cmd_wep_cur_key_usage = "[<index>]";
static const char *wireless_cmd_wep_cur_key_notes = "<index> is optional, allowable values are 0-3";
static int wireless_cmd_wep_cur_key_cb(int argc, char *argv[], int iface_id)
{
    uint8_t cur_key;
    int i_res;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if ( 2 == argc )
    {
        i_res = atoi(argv[1]);
        if ( ( i_res < 0 ) || ( i_res > 3 ) )
        {
            return CMD_USAGE_ERROR;
        }

        cur_key = (uint8_t)i_res;
        wifi_intf_set_var_wep_cur_key(iface_id, cur_key);
    }

    wifi_intf_get_var_wep_cur_key(iface_id, &cur_key);

    cmd_append_result("%u", cur_key);

    return CMD_OK;
}

static int wireless_cmd_uap_wep_cur_key_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_wep_cur_key_cb( argc, argv, iface_id );

    return i_res;
}

static int wireless_cmd_sta_wep_cur_key_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_wep_cur_key_cb( argc, argv, iface_id );

    return i_res;
}

#ifdef WIFI_DIRECT_SUPPORT

static const char *wireless_cmd_wfd_start_find_phase_desc = "Start WFD Find Phase";
static const char *wireless_cmd_wfd_start_find_phase_usage = NULL;
static const char *wireless_cmd_wfd_start_find_phase_notes = "Cancel with command wifi wfd disable";
static int wireless_cmd_wfd_start_find_phase_cb(int argc, char *argv[])
{
    wlan_wfd_start_find_phase();
    return CMD_OK;
}


static const char *wireless_cmd_wfd_role_desc = "Set (or view) role (1 - GO, 2 - Client";
static const char *wireless_cmd_wfd_role_usage = "[<role>]";
static const char *wireless_cmd_wfd_role_notes = "<role> is optional, allowable values are 1-2";
static int wireless_cmd_wfd_role_cb(int argc, char *argv[])
{
    int i_res;
    uint8_t role;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if ( 2 == argc )
    {
        i_res = atoi(argv[1]);
        if ( ( i_res < WIFIDIRECT_GO_ROLE ) || ( i_res > WIFIDIRECT_CLIENT_ROLE ) )
        {
            return CMD_USAGE_ERROR;
        }

        role = (uint8_t)i_res;

        wifi_intf_set_var_wfd_bss_role(WIFI_IFACE_ID_WFD, role);
        wifi_intf_set_var_is_enabled(WIFI_IFACE_ID_WFD, false);
        wifi_intf_set_var_is_enabled(WIFI_IFACE_ID_WFD, true);
    }

    wifi_intf_get_var_wfd_bss_role(WIFI_IFACE_ID_WFD, &role);

    cmd_append_result("%u", role);

    switch (role)
    {
    case WIFIDIRECT_GO_ROLE:
        cmd_append_result(" - Autonomous Group Owner (GO)");
        break;
    case WIFIDIRECT_CLIENT_ROLE:
        cmd_append_result(" - Wifi-Direct P2P");
        break;
    default:
        cmd_append_result(" - Unknown");
        break;
    };

    return CMD_OK;
}

static const char *wireless_cmd_wfd_peer_desc = "Set (or view) peer";
static const char *wireless_cmd_wfd_peer_usage = "[<peer>]";
static const char *wireless_cmd_wfd_peer_notes = "<peer> is optional index";
static int wireless_cmd_wfd_peer_cb(int argc, char *argv[])
{
    int peer;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if ( 2 == argc )
    {
        peer = atoi(argv[1]);

        wfd_peer_device_selected(peer);
    } else {
        int num_peers = 0;
        int j;
        wfd_peer_info_t peer_info;

        wfd_num_of_peers(&num_peers);
        if (num_peers > 0) {
            cmd_printf("Find Result : %d entries in List\n", num_peers);

            if (num_peers > 0) {
                cmd_printf
                    ("--------------------------------------------------------------"
                     "-------------------------------------------\n");
                cmd_printf
                    ("  # |    Intf Address   |  Op "
                     "mode |   Dev Address     |  GO SSID/Device Name    |  Intended Address |\n");
                cmd_printf
                    ("--------------------------------------------------------------"
                     "-------------------------------------------\n");

                for (j = 0; j < num_peers; j++) {
                    if (wfd_peer_device(j, &peer_info) == OK)
                    {
                        /* Index Number */
                        cmd_append_result(" %02d | ", j);

                        /* Device ID */
                        if (peer_info.device_id) {
                            cmd_append_result("%02X:%02X:%02X:%02X:%02X:%02X",
                                              peer_info.device_id[0], peer_info.device_id[1], peer_info.device_id[2],
                                              peer_info.device_id[3], peer_info.device_id[4], peer_info.device_id[5]);
                        }
                        cmd_append_result(" |");

                        /* Group Capability */
                        if (peer_info.group_capability == 0)
                            cmd_append_result("  Device ");
                        else
                            cmd_append_result("    GO   ");

                        if (peer_info.device_addr) {
                            cmd_append_result(" | ");
                            cmd_append_result("%02X:%02X:%02X:%02X:%02X:%02X",
                                              peer_info.device_addr[0], peer_info.device_addr[1], peer_info.device_addr[2],
                                              peer_info.device_addr[3], peer_info.device_addr[4], peer_info.device_addr[5]);
                            cmd_append_result(" |");
                        }
                        if (peer_info.group_capability != 0){
                            if (peer_info.go_ssid) {
                                cmd_append_result("  %s", peer_info.go_ssid);
                            } else {
                                cmd_append_result("                         ");
                            }                      
                        } else {
                            if (peer_info.wps_device_name) {
                                cmd_append_result("  %s", peer_info.wps_device_name);
                            } else {
                                cmd_append_result("                         ");
                            }
                        }
                        if (peer_info.intended_addr) {
                            cmd_append_result(" | ");
                            cmd_append_result("%02X:%02X:%02X:%02X:%02X:%02X",
                                              peer_info.intended_addr[0], peer_info.intended_addr[1], peer_info.intended_addr[2],
                                              peer_info.intended_addr[3], peer_info.intended_addr[4], peer_info.intended_addr[5]);
                            cmd_append_result(" |");
                        }

                        cmd_append_result("\n");
                    }
                }
            }
        } else {
            cmd_printf("No peers\n");
        }

    }

    return CMD_OK;
}

#endif

static const char *wireless_cmd_wpa_passphrase_desc = "Set (or view) the wpa passphrase";
static const char *wireless_cmd_wpa_passphrase_usage = "[<new passphrase>]";
static const char *wireless_cmd_wpa_passphrase_notes = "This command automatically sets the security mode to WPA/WPA2 if it is not already set and you give a supply a <new passphrase>";
static int wireless_cmd_wpa_passphrase_cb(int argc, char *argv[], int iface_id)
{
    uint8_t temp_data[128];
    int len;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc )
    {
        len = strlen(argv[1]);
        wifi_intf_set_var_wpa_passphrase(iface_id, (uint8_t*)argv[1], len + 1);
        if (len > 0)
        {
            wifi_intf_security_mode_t sec_mode;
            wifi_intf_get_var_sec_mode(iface_id, &sec_mode);
            if ((sec_mode != WPA2_PSK) && (sec_mode != WPA_PSK))
            {
                wifi_intf_set_var_sec_mode(iface_id, WPA2_PSK);
            }
        }
        else
        {
            wifi_intf_set_var_sec_mode(iface_id, OPEN_SYSTEM);
        }
    }

    /* get */
    wifi_intf_get_var_wpa_passphrase(iface_id, temp_data, sizeof(temp_data));
    cmd_append_result("%s", temp_data);

    return CMD_OK;
}

static int wireless_cmd_uap_wpa_passphrase_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_UAP;

    i_res = wireless_cmd_wpa_passphrase_cb( argc, argv, iface_id );

    return i_res;
}

#ifdef WIFI_DIRECT_SUPPORT
static int wireless_cmd_wfd_wpa_passphrase_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_WFD;

    i_res = wireless_cmd_wpa_passphrase_cb( argc, argv, iface_id );

    return i_res;
}
#endif

static int wireless_cmd_sta_wpa_passphrase_cb(int argc, char *argv[])
{
    int iface_id;
    int i_res;

    iface_id = WIFI_IFACE_ID_STATION;

    i_res = wireless_cmd_wpa_passphrase_cb( argc, argv, iface_id );

    return i_res;
}

static const char *wireless_cmd_sta_comm_mode_desc = "Set (or view) communication mode";
static const char *wireless_cmd_sta_comm_mode_usage = "[infrastructure|adhoc]";
static const char *wireless_cmd_sta_comm_mode_notes = NULL;
static int wireless_cmd_sta_comm_mode_cb(int argc, char *argv[])
{
    wifi_intf_comm_mode_t comm_mode;
    int iface_id;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_STATION;

    /* set */
    if ( 2 == argc )
    {
        if ( strcmp( "adhoc", argv[1] ) == 0 )
        {
            comm_mode = AD_HOC;
        }
        else if ( strcmp( "infrastructure", argv[1] ) == 0 )
        {
            comm_mode = INFRASTRUCTURE;
        }
        else
        {
            return CMD_USAGE_ERROR;
        }
        wifi_intf_set_var_comm_mode(iface_id, comm_mode);
    }

    /* get */
    wifi_intf_get_var_comm_mode(iface_id, &comm_mode);
    cmd_append_result("%s", comm_mode == AD_HOC ? "adhoc" : "infrastructure");

    return CMD_OK;
}

static const char *wireless_cmd_uap_wpa_enc_mode_desc = "Set (or view) the wpa encryption mode";
static const char *wireless_cmd_uap_wpa_enc_mode_usage = "[tkip|aes|both]";
static const char *wireless_cmd_uap_wpa_enc_mode_notes = "both means tkip and aes. aes may also be known as ccmp";
static int wireless_cmd_uap_wpa_enc_mode_cb(int argc, char *argv[])
{
    wifi_intf_wpa_encryp_mode_t enc_mode;
    int iface_id;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP;

    /* set */
    if ( 2 == argc )
    {
        if ( strcmp( "tkip", argv[1] ) == 0 )
        {
            enc_mode = WPA_TKIP;
        }
        else if ( ( strcmp( "aes", argv[1] ) == 0 ) ||
                  ( strcmp( "ccmp", argv[1] ) == 0 ) )
        {
            enc_mode = WPA_AES;
        }
        else if ( strcmp( "both", argv[1] ) == 0 )
        {
            enc_mode = WPA_TKIP_AND_AES;
        }
        else
        {
            return CMD_USAGE_ERROR;
        }
        wifi_intf_set_var_wpa_encryp_mode(iface_id, enc_mode);
    }

    /* get */
    wifi_intf_get_var_wpa_encryp_mode(iface_id, &enc_mode);

    switch(enc_mode)
    {
        case WPA_TKIP        : cmd_append_result("tkip"); break;
        case WPA_AES         : cmd_append_result("aes"); break;
        case WPA_TKIP_AND_AES: cmd_append_result("both"); break;
        default              : cmd_append_result("error %d", enc_mode); break;
    }

    return CMD_OK;
}

static const char *wireless_cmd_uap_channel_desc = "Set (or view) the current uAP channel";
static const char *wireless_cmd_uap_channel_usage = "wifi uap channel <channel_number>";
static const char *wireless_cmd_uap_channel_notes = NULL;
static int wireless_cmd_uap_channel_cb(int argc, char *argv[])
{
    uint8_t chan;
    uint8_t iface_id;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP;

    /* set */
    if ( 2 == argc )
    {        
        wifi_intf_set_var_channel(iface_id, atoi(argv[1]));
    }

    /* get */
    wifi_intf_get_var_channel(iface_id, &chan);

    cmd_printf("Current uAP channel is set to %d\n", chan);

    return CMD_OK;
}

static const char *wireless_cmd_uap_pri0_channel_desc = "Set (or view) the current uAP Pri0 channel";
static const char *wireless_cmd_uap_pri0_channel_usage = "wifi_uap_pri0_channel <channel_number>";
static const char *wireless_cmd_uap_pri0_channel_notes = NULL;
static int wireless_cmd_uap_pri0_channel_cb(int argc, char *argv[])
{
    uint8_t chan;
    uint8_t iface_id;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP_PRI0;

    /* set */
    if ( 2 == argc )
    {        
        wifi_intf_set_var_channel(iface_id, atoi(argv[1]));
    }

    /* get */
    wifi_intf_get_var_channel(iface_id, &chan);

    cmd_printf("Current uAP Pri0 channel is set to %d\n", chan);

    return CMD_OK;
}

static const char *wireless_cmd_uap_mac_filter_mode_desc = "Set (or view) the uAP MAC filter mode";
static const char *wireless_cmd_uap_mac_filter_mode_usage = "[disable|whitelist|blacklist]";
static const char *wireless_cmd_uap_mac_filter_mode_notes = NULL;
static int wireless_cmd_uap_mac_filter_mode_cb(int argc, char *argv[])
{
    wifi_intf_mac_filter_mode_t filter_mode;
    int iface_id;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP;

    /* set */
    if ( 2 == argc )
    {
        if ( strcmp( "disable", argv[1] ) == 0 )
        {
            filter_mode = MAC_FILTER_MODE_DISABLED;
        }
        else if (strcmp( "whitelist", argv[1] ) == 0 )
        {
            filter_mode = MAC_FILTER_MODE_WHITELIST;
        }
        else if ( strcmp( "blacklist", argv[1] ) == 0 )
        {
            filter_mode = MAC_FILTER_MODE_BLACKLIST;
        }
        else
        {
            return CMD_USAGE_ERROR;
        }
        wifi_intf_set_var_mac_filter_mode(iface_id, filter_mode);
    }

    /* get */
    wifi_intf_get_var_mac_filter_mode(iface_id, &filter_mode);

    switch(filter_mode)
    {
        case MAC_FILTER_MODE_DISABLED  : cmd_append_result("disable"); break;
        case MAC_FILTER_MODE_WHITELIST : cmd_append_result("whitelist"); break;
        case MAC_FILTER_MODE_BLACKLIST : cmd_append_result("blacklist"); break;
        default                        : cmd_append_result("error %d", filter_mode); break;
    }

    return CMD_OK;
}

static const char *wireless_cmd_uap_pri0_mac_filter_mode_desc = "Set (or view) the uAP Pri0 MAC filter mode";
static const char *wireless_cmd_uap_pri0_mac_filter_mode_usage = "[disable|whitelist|blacklist]";
static const char *wireless_cmd_uap_pri0_mac_filter_mode_notes = NULL;
static int wireless_cmd_uap_pri0_mac_filter_mode_cb(int argc, char *argv[])
{
    wifi_intf_mac_filter_mode_t filter_mode;
    int iface_id;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP_PRI0;

    /* set */
    if ( 2 == argc )
    {
        if ( strcmp( "disable", argv[1] ) == 0 )
        {
            filter_mode = MAC_FILTER_MODE_DISABLED;
        }
        else if (strcmp( "whitelist", argv[1] ) == 0 )
        {
            filter_mode = MAC_FILTER_MODE_WHITELIST;
        }
        else if ( strcmp( "both", argv[1] ) == 0 )
        {
            filter_mode = MAC_FILTER_MODE_BLACKLIST;
        }
        else
        {
            return CMD_USAGE_ERROR;
        }
        wifi_intf_set_var_mac_filter_mode(iface_id, filter_mode);
    }

    /* get */
    wifi_intf_get_var_mac_filter_mode(iface_id, &filter_mode);

    switch(filter_mode)
    {
        case MAC_FILTER_MODE_DISABLED  : cmd_append_result("disable"); break;
        case MAC_FILTER_MODE_WHITELIST : cmd_append_result("whitelist"); break;
        case MAC_FILTER_MODE_BLACKLIST : cmd_append_result("blacklist"); break;
        default                        : cmd_append_result("error %d", filter_mode); break;
    }

    return CMD_OK;
}

static const char *wireless_cmd_uap_mac_filter_add_desc = "Add entry into the uAP MAC filter list";
static const char *wireless_cmd_uap_mac_filter_add_usage = "command followed by MAC address to be added";
static const char *wireless_cmd_uap_mac_filter_add_notes = NULL;
static int wireless_cmd_uap_mac_filter_add_cb(int argc, char *argv[])
{
    int iface_id;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP;

    wifi_intf_var_mac_filter_add( iface_id, argv[1]);

    return CMD_OK;
}

static const char *wireless_cmd_uap_mac_filter_remove_desc = "Remove entry into the uAP MAC filter list";
static const char *wireless_cmd_uap_mac_filter_remove_usage = "command followed by MAC address to be removed";
static const char *wireless_cmd_uap_mac_filter_remove_notes = NULL;
static int wireless_cmd_uap_mac_filter_remove_cb(int argc, char *argv[])
{
    int iface_id;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP;

    wifi_intf_var_mac_filter_remove( iface_id, argv[1]);

    return CMD_OK;
}

static const char *wireless_cmd_uap_pri0_mac_filter_add_desc = "Add entry into the uAP Pri0 MAC filter list";
static const char *wireless_cmd_uap_pri0_mac_filter_add_usage = "command followed by MAC address to be added";
static const char *wireless_cmd_uap_pri0_mac_filter_add_notes = NULL;
static int wireless_cmd_uap_pri0_mac_filter_add_cb(int argc, char *argv[])
{
    int iface_id;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP_PRI0;

    wifi_intf_var_mac_filter_add( iface_id, argv[1]);

    return CMD_OK;
}

static const char *wireless_cmd_uap_pri0_mac_filter_remove_desc = "Remove entry into the uAP Pri0 MAC filter list";
static const char *wireless_cmd_uap_pri0_mac_filter_remove_usage = "command followed by MAC address to be removed";
static const char *wireless_cmd_uap_pri0_mac_filter_remove_notes = NULL;
static int wireless_cmd_uap_pri0_mac_filter_remove_cb(int argc, char *argv[])
{
    int iface_id;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP_PRI0;

    wifi_intf_var_mac_filter_remove( iface_id, argv[1]);

    return CMD_OK;
}

static const char *wireless_cmd_uap_mac_filter_count_desc = "Count for uAP MAC filter list";
static const char *wireless_cmd_uap_mac_filter_count_usage = NULL;
static const char *wireless_cmd_uap_mac_filter_count_notes = NULL;
static int wireless_cmd_uap_mac_filter_count_cb(int argc, char *argv[])
{
    int iface_id;
    uint8_t count;

    iface_id = WIFI_IFACE_ID_UAP;

    wifi_intf_get_var_mac_filter_count( iface_id, &count);
    cmd_printf("Count for the uAP MAC filter list is %d\n", count);

    return CMD_OK;
}

static const char *wireless_cmd_uap_pri0_mac_filter_count_desc = "Count for uAP Pri0 MAC filter list";
static const char *wireless_cmd_uap_pri0_mac_filter_count_usage = NULL;
static const char *wireless_cmd_uap_pri0_mac_filter_count_notes = NULL;
static int wireless_cmd_uap_pri0_mac_filter_count_cb(int argc, char *argv[])
{
    int iface_id;
    uint8_t count;
    
    iface_id = WIFI_IFACE_ID_UAP_PRI0;

    wifi_intf_get_var_mac_filter_count( iface_id, &count);
    cmd_printf("Count for the uAP Pri0 MAC filter list is %d\n", count);

    return CMD_OK;
}

static const char *wireless_cmd_uap_mac_filter_flush_desc = "Flush for uAP MAC filter list";
static const char *wireless_cmd_uap_mac_filter_flush_usage = NULL;
static const char *wireless_cmd_uap_mac_filter_flush_notes = NULL;
static int wireless_cmd_uap_mac_filter_flush_cb(int argc, char *argv[])
{
    int iface_id;
    
    iface_id = WIFI_IFACE_ID_UAP;

    wifi_intf_var_mac_filter_flush( iface_id);
    cmd_printf("Flushed the uAP MAC filter list.\n");

    return CMD_OK;
}

static const char *wireless_cmd_uap_pri0_mac_filter_flush_desc = "Flush for uAP Pri0 MAC filter list";
static const char *wireless_cmd_uap_pri0_mac_filter_flush_usage = NULL;
static const char *wireless_cmd_uap_pri0_mac_filter_flush_notes = NULL;
static int wireless_cmd_uap_pri0_mac_filter_flush_cb(int argc, char *argv[])
{
    int iface_id;
    
    iface_id = WIFI_IFACE_ID_UAP_PRI0;

    wifi_intf_var_mac_filter_flush( iface_id);
    cmd_printf("Flushed the uAP MAC filter list.\n");

    return CMD_OK;    
}

static const char *wireless_cmd_info_desc = "Print current values of assorted configuration info";
static const char *wireless_cmd_info_usage = NULL;
static const char *wireless_cmd_info_notes = NULL;
static int wireless_cmd_info_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    WifiHWShowInfo();

    return CMD_OK;
}

static const char *wireless_cmd_enable_desc = "Enable the radio";
static const char *wireless_cmd_enable_usage = NULL;
static const char *wireless_cmd_enable_notes = NULL;
static int wireless_cmd_enable_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_hw_set_var_is_enabled(true);

    return CMD_OK;
}

static const char *wireless_cmd_disable_desc = "Disable the radio";
static const char *wireless_cmd_disable_usage = NULL;
static const char *wireless_cmd_disable_notes = NULL;
static int wireless_cmd_disable_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_hw_set_var_is_enabled(false);

    return CMD_OK;
}

#ifdef HAVE_HOTSPOT_SUPPORT
static const char *wireless_cmd_hotspot_desc  = "Enable or Disable the wifi hotspot";
static const char *wireless_cmd_hotspot_usage = "[enable|disable]";
static const char *wireless_cmd_hotspot_notes = NULL;
static int wireless_cmd_hotspot_cb(int argc, char *argv[])
{
    bool enabled;
    if ( argc > 1 )
    {
        if (strcmpi(argv[1], "enable") == 0)
        {
            enabled = true;
        }
        else
        {
            enabled = false;
        }
        wifi_set_var_hotspot_enabled(enabled);
    }

    wifi_get_var_hotspot_enabled(&enabled);

    cmd_printf("Hotspot is: ");
    cmd_append_result(enabled ? "enabled" : "disabled");

    return CMD_OK;
}
#endif // HAVE_HOTSPOT_SUPPORT

static const char *wireless_cmd_set_customie_desc = "Set CUSTOM IE";
static const char *wireless_cmd_set_customie_usage = "<iface_id> <index> <mask> <buffer>";
static const char *wireless_cmd_set_customie_notes = "iface_id : 1-> uAP, 2-> WFD";
static int wireless_cmd_set_customie_cb(int argc, char *argv[])
{
    #define MAX_IE_SIZE 256
    uint8_t ie_array[MAX_IE_SIZE];
    int iface_id;
    int i = 0;
    int index;    
    int length;
    uint16_t mask;
    uint32_t temp;
    wifi_intf_customie_info_t *p_customie_info = NULL;

    if ( 5 == argc )
    {
        iface_id = atoi(argv[1]);
        index = atoi(argv[2]);
        str_mkint( argv[3], &temp );
        mask = (uint16_t) temp;

        hex_str_to_raw( argv[4], (char *) ie_array, MAX_IE_SIZE);

        length = strlen(argv[4])/2;

        cmd_printf("Set Custom IE for index %d mask 0x%X length %d\n", index, mask, length);
        for (i=0; i<length; i++)
        {
            cmd_printf("%02X", ie_array[i]);
        }
        
        wifi_intf_set_custom_ie(iface_id, index, mask, ie_array, length);                   
    }

    cmd_printf("\n\nGlobal Custom IE array contents are as follows:\n");
    for (i=0; i < MAX_NUM_CUSTOM_IE; i++)
    {
        p_customie_info = wifi_intf_get_customie_info(i);
        
        if (p_customie_info != NULL)
        {
            cmd_printf("IE Index: %x, Mask: %x, IE_length: %x\n",i , p_customie_info->mgmt_subtype_mask, p_customie_info->ie_length);
            cmd_printf("\n");
        }    
    }
    
    return CMD_OK;
}

static const char *wireless_cmd_defaults_desc = "Restore default settings";
static const char *wireless_cmd_defaults_usage = NULL;
static const char *wireless_cmd_defaults_notes = NULL;
static int wireless_cmd_defaults_cb(int argc, char *argv[])
{
    int i;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_hw_var_restore_defaults(WIFI_HW_VAR_ALL);
    for (i = 0; i < WIFI_NUM_IFACES; i++)
    {
        wifi_intf_var_restore_defaults(i, WIFI_INTF_VAR_ALL);
    }

    return CMD_OK;
}

static const char *wireless_cmd_power_desc = "View/set the wifi board's power setting";
static const char *wireless_cmd_power_usage = "[on|off]";
static const char *wireless_cmd_power_notes = NULL;
static int wireless_cmd_power_cb(int argc, char *argv[])
{
    bool radio_on;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc )
    {
        if (strcmp(argv[1], "off") == 0) 
        {
            wifi_intf_turn_radio_off();
        }
        else if (strcmp(argv[1], "on") == 0) 
        {
            wifi_intf_turn_radio_on();
        }
        else
        {
            return CMD_USAGE_ERROR;
        }
    }

    /* get */
    wifi_intf_get_radio_on_status(&radio_on);

    cmd_append_result("%s", radio_on ? "on" : "off");

    return CMD_OK;
}

static const char *wireless_cmd_region_desc = "View/set region";
static const char *wireless_cmd_region_usage = "[<region>]";
static const char *wireless_cmd_region_notes = "<region> may be one of US, CAN, EUR, ESP, FRA, JPN or a numeric value";
static int wireless_cmd_region_cb(int argc, char *argv[])
{
    uint8_t region;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc)
    {
        if (strcmp(argv[1], "US") == 0) {
            region = REG_US;
        }
        else if (strcmp(argv[1], "CAN") == 0) {
            region = REG_CAN;
        }
        else if (strcmp(argv[1], "EUR") == 0) {
            region = REG_EUR;
        }
        else if (strcmp(argv[1], "ESP") == 0) {
            region = REG_ESP;
        }
        else if (strcmp(argv[1], "FRA") == 0) {
            region = REG_FRA;
        }
        else if (strcmp(argv[1], "JPN") == 0) {
            region = REG_JPN;
        }
        else
        {
            int i_res;
            uint32_t num;
            i_res = str_mkint( argv[1], &num );
            if ( 0 != i_res )
            {
                return CMD_USAGE_ERROR;
            }
            if ( num > UINT8_MAX )
            {
                return CMD_USAGE_ERROR;
            }
            region = (uint8_t)num;
        }
        wifi_hw_set_var_region_code(region);
    }

    /* get */
    wifi_hw_get_var_region_code(&region);
    switch(region)
    {
        case REG_US:  cmd_append_result("US"); break;
        case REG_CAN: cmd_append_result("CAN"); break;
        case REG_EUR: cmd_append_result("EUR"); break;
        case REG_ESP: cmd_append_result("ESP"); break;
        case REG_FRA: cmd_append_result("FRA"); break;
        case REG_JPN: cmd_append_result("JPN"); break;
        default:      cmd_append_result("%d", region); break;
    }

    return CMD_OK;
}

static const char *wireless_cmd_wws_config_desc = "View/set World Wide Safe Configuration";
static const char *wireless_cmd_wws_config_usage = "";
static const char *wireless_cmd_wws_config_notes = "";
static int wireless_cmd_wws_config_cb(int argc, char *argv[])
{
    uint8_t wws_config;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc)
    {
        if (strcmp(argv[1], "enable") == 0)
        {
            wws_config = 1;
            wifi_hw_set_var_wws_config(wws_config);
        }
        else if (strcmp(argv[1], "disable") == 0)
        {
            wws_config = 0;
            wifi_hw_set_var_wws_config(wws_config);
        }    
    }

    /* get */
    wifi_hw_get_var_wws_config(&wws_config);
    if (wws_config)
       cmd_printf("Enabled\n");
    else
       cmd_printf("Disabled\n"); 

    return CMD_OK;
}

static const char *wireless_cmd_uap_active_config_desc = "View/set the currently active uAP Configuratio, 0: uAP, 1: uAP_Pri0\n";
static const char *wireless_cmd_uap_active_config_usage = "";
static const char *wireless_cmd_uap_active_config_notes = "";
static int wireless_cmd_uap_active_config_cb(int argc, char *argv[])
{
    uint8_t uap_active_config = WIFI_IFACE_ID_UAP;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* set */
    if ( 2 == argc)
    {
        if (strcmp(argv[1], "0") == 0)
        {
            uap_active_config = WIFI_IFACE_ID_UAP;            
        }
        else if (strcmp(argv[1], "1") == 0)
        {
            uap_active_config = WIFI_IFACE_ID_UAP_PRI0;
        }

        wifi_change_uap_active_config(uap_active_config);    
    }

    /* get */
    uap_active_config = wifi_get_uap_active_config();
    if (uap_active_config == WIFI_IFACE_ID_UAP)
       cmd_printf("uAP\n");
    else if (uap_active_config == WIFI_IFACE_ID_UAP_PRI0)
       cmd_printf("uAP_Pri0\n"); 

    return CMD_OK;
}

static int wireless_cmd_scan_dump( void )
{
    bool scanning;
    wifi_intf_scan_info_t *scan_info;
    int i, counter = 0;

    scanning = true;

    wifi_intf_get_var_is_wireless_scanning(&scanning);
    while (scanning)
    {
        counter++; 
        if (counter >= 100)
        {
            cmd_printf("Wireless Scan Timeout!!\n");
            break;
        }
        
        tx_thread_sleep(10);
        wifi_intf_get_var_is_wireless_scanning(&scanning);
    }

    //Something went really wrong on the wireless scan
    if (counter >= 100)
        return CMD_ERROR;

    scan_info = (wifi_intf_scan_info_t *) MEM_MALLOC(sizeof(wifi_intf_scan_info_t));
    if (scan_info != NULL)
    {
        wifi_intf_get_var_scan_info(scan_info);

        // Print results
        cmd_printf("Scan Results\n");
        cmd_printf("Number SSIDs: %d\n", scan_info->NumSSID);
        for (i=0; i<scan_info->NumSSID; i++)
        {
            cmd_printf(" SSID ..      %s \n", scan_info->networks[i].SSIDNames);
            cmd_printf(" BSSID ..     [%02x%02x%02x%02x%02x%02x] \n", 
                       scan_info->networks[i].BSSID[0],
                       scan_info->networks[i].BSSID[1],
                       scan_info->networks[i].BSSID[2],
                       scan_info->networks[i].BSSID[3],
                       scan_info->networks[i].BSSID[4],
                       scan_info->networks[i].BSSID[5]);
            cmd_printf(" type:        %s \n", scan_info->networks[i].communicationMode == INFRASTRUCTURE ? "INFRASTRUCTURE" : "AD HOC");
            cmd_printf(" encryption:  ");
            switch(scan_info->networks[i].securityMode)
            {
                case WPA_ENTERPRISE:     cmd_printf("WPA Enterprise");  break;
                case WPA2_ENTERPRISE:    cmd_printf("WPA2 Enterprise"); break;
                case WPA_PSK:     cmd_printf("WPA");  break;
                case WPA2_PSK:    cmd_printf("WPA2"); break;
                case WEP:         cmd_printf("WEP");  break;
                case OPEN_SYSTEM: // Intentionally fall through
                default:          cmd_printf("None"); break;
            }
            cmd_printf("\n");

            cmd_printf(" dbm:         %d \n", scan_info->networks[i].dBm);
            cmd_printf(" signal[1-5]: %d \n", scan_info->networks[i].signalStrength);
            cmd_printf(" channel:     %d \n", scan_info->networks[i].channel);

            cmd_printf(" WPS support? ");
            if (scan_info->networks[i].wifiOptions & WPS_PIN)
                cmd_printf("PIN ");
            if (scan_info->networks[i].wifiOptions & WPS_PUSH_BUTTON)
                cmd_printf("PBC ");
            if (scan_info->networks[i].wifiOptions & WPS_PBC_ACTIVE)
                cmd_printf("PBC IS ACTIVE");

            cmd_printf("\n\n");
        }
        MEM_FREE_AND_NULL(scan_info);
    }
    else
    {
        return CMD_ERROR;
    }

    return CMD_OK;
}

static const char *wireless_cmd_scan_desc = "Scan to find available SSIDs";
static const char *wireless_cmd_scan_usage = "[<ssid>]";
static const char *wireless_cmd_scan_notes = "specifying <ssid> does a directed scan for that ssid";
static int wireless_cmd_scan_cb(int argc, char *argv[])
{
    int i_res;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if ( 2 == argc )
    {
        /* directed scan */
        wifi_intf_start_scan( (uint8_t*)argv[1], strlen((char*)argv[1]) );
    }
    else
    {
        /* generic scan */
        wifi_intf_start_scan(NULL, 0);
    }

    i_res = wireless_cmd_scan_dump();

    return i_res;
}

static const char *wireless_cmd_storedscan_desc = "View results of most recent scan";
static const char *wireless_cmd_storedscan_usage = NULL;
static const char *wireless_cmd_storedscan_notes = NULL;
static int wireless_cmd_storedscan_cb(int argc, char *argv[])
{
    int i_res;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_printf("Stored ");

    i_res = wireless_cmd_scan_dump();

    return i_res;
}

static const char *wireless_cmd_echo_desc = "Test command to echo argc/argv";
static const char *wireless_cmd_echo_usage = "[<arg 1>] [<arg 2>] ...";
static const char *wireless_cmd_echo_notes = "specify as many arguments as you would like";
static int wireless_cmd_echo_cb(int argc, char *argv[])
{
    uint32_t pos;
    pos = 0;

    while (pos < argc)
    {
        cmd_printf("argv[%lu] (%2d bytes): %s\n", pos, strlen(argv[pos]), argv[pos]);
        pos++;
    }

    return CMD_OK;
}

static const char *wireless_cmd_usbsuspend_desc = "";
static const char *wireless_cmd_usbsuspend_usage = NULL;
static const char *wireless_cmd_usbsuspend_notes = NULL;
static int wireless_cmd_usbsuspend_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_intf_usb_suspend(NULL);

    return CMD_OK;
}

static const char *wireless_cmd_usbresume_desc = "";
static const char *wireless_cmd_usbresume_usage = NULL;
static const char *wireless_cmd_usbresume_notes = NULL;
static int wireless_cmd_usbresume_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_intf_usb_resume();

    return CMD_OK;
}

static const char *wireless_cmd_sta_get_link_speed_desc = "";
static const char *wireless_cmd_sta_get_link_speed_usage = NULL;
static const char *wireless_cmd_sta_get_link_speed_notes = NULL;
static int wireless_cmd_sta_get_link_speed_cb(int argc, char *argv[])
{
    int link_speed;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    wifi_intf_sta_get_link_speed(&link_speed);

    cmd_printf("Link Speed: %d kbps\n", link_speed);

    return CMD_OK;
}

static const char *wireless_cmd_sta_link_status_desc = "";
static const char *wireless_cmd_sta_link_status_usage = NULL;
static const char *wireless_cmd_sta_link_status_notes = NULL;
static int wireless_cmd_sta_link_status_cb(int argc, char *argv[])
{
    uint32_t link_state = wifi_intf_sta_get_current_link_status();

    dbg_printf( "\nWifi Current Link Status: %s\n", 
                (link_state == Link_Down ? "Down" : 
                link_state == Link_Connected ? "Connected" : 
                link_state == Link_Connecting ? "Connecting" : "ERROR"));
    return CMD_OK;
}


#ifdef HAVE_WIFI_CERT_TEST
static const char *wireless_cmd_sigmacmd_desc = "Used to issue sigma commands";
static const char *wireless_cmd_sigmacmd_usage = "[<sigma command goes here>]";
static const char *wireless_cmd_sigmacmd_notes = NULL;
static int wireless_cmd_sigmacmd_cb(int argc, char *argv[])
{
    char *response_buffer;
    char *cmd_buffer; 
    int response_len = 0;
    int cmd_len = 0;
    int i;

    //Allocate memory for the response and command buffers
    response_buffer = MEM_MALLOC (1024 * sizeof (char));    
    ASSERT(response_buffer != NULL);
  
    memset(response_buffer, 0 , 1024 * sizeof (char));

    cmd_buffer = MEM_MALLOC (1024 * sizeof (char));    
    ASSERT(cmd_buffer != NULL);
  
    memset(cmd_buffer, 0 , 1024 * sizeof (char));
  
    //Build up the command buffer
    strncpy(cmd_buffer, argv[1], strlen(argv[1]));
    cmd_len = strlen(argv[1]);

    for (i = 2; i < argc; i++)
    {
        strcat(cmd_buffer, " ");
        strcat(cmd_buffer, argv[i]);
        cmd_len += strlen(argv[i]) + 1;
    }

    //Send command to Sigma CA
    sigma_cmd_handler(cmd_buffer, cmd_len, response_buffer, &response_len);

    cmd_printf("%s\n", response_buffer); 
 
    MEM_FREE_AND_NULL(response_buffer);
    MEM_FREE_AND_NULL(cmd_buffer);

    return CMD_OK;  
}
#endif

static const char *wireless_cmd_uap_pri0_disable_desc = "disable this interface";
static const char *wireless_cmd_uap_pri0_disable_usage = NULL;
static const char *wireless_cmd_uap_pri0_disable_notes = NULL;
static int wireless_cmd_uap_pri0_disable_cb(int argc, char *argv[])
{
    return wireless_cmd_intf_disable_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);      
}

static const char *wireless_cmd_uap_pri0_enable_desc = "enable this interface";
static const char *wireless_cmd_uap_pri0_enable_usage = NULL;
static const char *wireless_cmd_uap_pri0_enable_notes = NULL;
static int wireless_cmd_uap_pri0_enable_cb(int argc, char *argv[])
{
    return wireless_cmd_intf_enable_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);  
}

static const char *wireless_cmd_uap_pri0_defaults_desc = "restore to defaults";
static const char *wireless_cmd_uap_pri0_defaults_usage = NULL;
static const char *wireless_cmd_uap_pri0_defaults_notes = NULL;
static int wireless_cmd_uap_pri0_defaults_cb(int argc, char *argv[])
{
    return wireless_cmd_intf_defaults_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);      
}

static const char *wireless_cmd_uap_pri0_info_desc = "Print current values of assorted configuration info";
static const char *wireless_cmd_uap_pri0_info_usage = NULL;
static const char *wireless_cmd_uap_pri0_info_notes = NULL;
static int wireless_cmd_uap_pri0_info_cb(int argc, char *argv[])
{
    WifiIfaceShowInfo(WIFI_IFACE_ID_UAP_PRI0);
    return CMD_OK;  
}

static const char *wireless_cmd_uap_pri0_ssid_desc = "Set (or view) the current SSID";
static const char *wireless_cmd_uap_pri0_ssid_usage = NULL;
static const char *wireless_cmd_uap_pri0_ssid_notes = NULL;
static int wireless_cmd_uap_pri0_ssid_cb(int argc, char *argv[])
{
    return wireless_cmd_ssid_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);      
}

static const char *wireless_cmd_uap_pri0_ssid_prefix_desc = "Set (or view) the current SSID prefix";
static const char *wireless_cmd_uap_pri0_ssid_prefix_usage = NULL;
static const char *wireless_cmd_uap_pri0_ssid_prefix_notes = NULL;
static int wireless_cmd_uap_pri0_ssid_prefix_cb(int argc, char *argv[])
{
    return wireless_cmd_ssid_prefix_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);  
}

static const char *wireless_cmd_uap_pri0_security_desc = "Set (or view) the current security";
static const char *wireless_cmd_uap_pri0_security_usage = NULL;
static const char *wireless_cmd_uap_pri0_security_notes = NULL;
static int wireless_cmd_uap_pri0_security_cb(int argc, char *argv[])
{
    return wireless_cmd_security_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);     
}

static const char *wireless_cmd_uap_pri0_wep_auth_mode_desc = "Set (or view) the current WEP auth mode";
static const char *wireless_cmd_uap_pri0_wep_auth_mode_usage = NULL;
static const char *wireless_cmd_uap_pri0_wep_auth_mode_notes = NULL;
static int wireless_cmd_uap_pri0_wep_auth_mode_cb(int argc, char *argv[])
{
    return wireless_cmd_wep_auth_mode_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);      
}

static const char *wireless_cmd_uap_pri0_wep_key_desc = "Set (or view) WEP key";
static const char *wireless_cmd_uap_pri0_wep_key_usage = NULL;
static const char *wireless_cmd_uap_pri0_wep_key_notes = NULL;
static int wireless_cmd_uap_pri0_wep_key_cb(int argc, char *argv[])
{
    return wireless_cmd_wep_key_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);  
}

static const char *wireless_cmd_uap_pri0_wep_cur_key_desc = "Set (or view) WEP current key";
static const char *wireless_cmd_uap_pri0_wep_cur_key_usage = NULL;
static const char *wireless_cmd_uap_pri0_wep_cur_key_notes = NULL;
static int wireless_cmd_uap_pri0_wep_cur_key_cb(int argc, char *argv[])
{
    return wireless_cmd_wep_cur_key_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);  
}

static const char *wireless_cmd_uap_pri0_wpa_passphrase_desc = "Set (or view) WPA passphrase";
static const char *wireless_cmd_uap_pri0_wpa_passphrase_usage = NULL;
static const char *wireless_cmd_uap_pri0_wpa_passphrase_notes = NULL;
static int wireless_cmd_uap_pri0_wpa_passphrase_cb(int argc, char *argv[])
{
    return wireless_cmd_wpa_passphrase_cb( argc, argv, WIFI_IFACE_ID_UAP_PRI0);  
}

static const char *wireless_cmd_uap_pri0_wpa_enc_mode_desc = "Set (or view) the wpa encryption mode";
static const char *wireless_cmd_uap_pri0_wpa_enc_mode_usage = "[tkip|aes|both]";
static const char *wireless_cmd_uap_pri0_wpa_enc_mode_notes = "both means tkip and aes. aes may also be known as ccmp";
static int wireless_cmd_uap_pri0_wpa_enc_mode_cb(int argc, char *argv[])
{
    wifi_intf_wpa_encryp_mode_t enc_mode;
    int iface_id;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    iface_id = WIFI_IFACE_ID_UAP_PRI0;

    /* set */
    if ( 2 == argc )
    {
        if ( strcmp( "tkip", argv[1] ) == 0 )
        {
            enc_mode = WPA_TKIP;
        }
        else if ( ( strcmp( "aes", argv[1] ) == 0 ) ||
                  ( strcmp( "ccmp", argv[1] ) == 0 ) )
        {
            enc_mode = WPA_AES;
        }
        else if ( strcmp( "both", argv[1] ) == 0 )
        {
            enc_mode = WPA_TKIP_AND_AES;
        }
        else
        {
            return CMD_USAGE_ERROR;
        }
        wifi_intf_set_var_wpa_encryp_mode(iface_id, enc_mode);
    }

    /* get */
    wifi_intf_get_var_wpa_encryp_mode(iface_id, &enc_mode);

    switch(enc_mode)
    {
        case WPA_TKIP        : cmd_append_result("tkip"); break;
        case WPA_AES         : cmd_append_result("aes"); break;
        case WPA_TKIP_AND_AES: cmd_append_result("both"); break;
        default              : cmd_append_result("error %d", enc_mode); break;
    }

    return CMD_OK;    
}


void wireless_register_cmds( void )
{
#ifdef HAVE_CMD
    int result;

    result = cmd_register_cmd( "wifi",
                               NULL,
                               NULL,
                               NULL,
                               NULL,
                               NULL );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "sta",
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap",
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL );
    ASSERT( CMD_OK == result );
#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi",
                                  "wfd",
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi sta",
                                  "wps",
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi sta wps",
                                  "cancel",
                                  wireless_cmd_wps_cancel_desc,
                                  wireless_cmd_wps_cancel_usage,
                                  wireless_cmd_wps_cancel_notes,
                                  wireless_cmd_wps_cancel_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi sta wps",
                                  "start",
                                  wireless_cmd_sta_wps_start_desc,
                                  wireless_cmd_sta_wps_start_usage,
                                  wireless_cmd_sta_wps_start_notes,
                                  wireless_cmd_sta_wps_start_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi sta wps",
                                  "genpin",
                                  wireless_cmd_sta_wps_genpin_desc,
                                  wireless_cmd_sta_wps_genpin_usage,
                                  wireless_cmd_sta_wps_genpin_notes,
                                  wireless_cmd_sta_wps_genpin_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "wps",
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi uap wps",
                                  "cancel",
                                  wireless_cmd_wps_cancel_desc,
                                  wireless_cmd_wps_cancel_usage,
                                  wireless_cmd_wps_cancel_notes,
                                  wireless_cmd_wps_cancel_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap wps",
                                  "start",
                                  wireless_cmd_uap_wps_start_desc,
                                  wireless_cmd_uap_wps_start_usage,
                                  wireless_cmd_uap_wps_start_notes,
                                  wireless_cmd_uap_wps_start_cb );
    ASSERT( CMD_OK == result );
#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "wps",
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi wfd wps",
                                  "cancel",
                                  wireless_cmd_wps_cancel_desc,
                                  wireless_cmd_wps_cancel_usage,
                                  wireless_cmd_wps_cancel_notes,
                                  wireless_cmd_wps_cancel_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi wfd wps",
                                  "start",
                                  wireless_cmd_wfd_wps_start_desc,
                                  wireless_cmd_wfd_wps_start_usage,
                                  wireless_cmd_wfd_wps_start_notes,
                                  wireless_cmd_wfd_wps_start_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi uap",
                                  "enable",
                                  wireless_cmd_intf_enable_desc,
                                  wireless_cmd_intf_enable_usage,
                                  wireless_cmd_intf_enable_notes,
                                  wireless_cmd_uap_enable_cb );
    ASSERT( CMD_OK == result );

#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "enable",
                                  wireless_cmd_intf_enable_desc,
                                  wireless_cmd_intf_enable_usage,
                                  wireless_cmd_intf_enable_notes,
                                  wireless_cmd_wfd_enable_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi sta",
                                  "enable",
                                  wireless_cmd_intf_enable_desc,
                                  wireless_cmd_intf_enable_usage,
                                  wireless_cmd_intf_enable_notes,
                                  wireless_cmd_sta_enable_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "disable",
                                  wireless_cmd_intf_disable_desc,
                                  wireless_cmd_intf_disable_usage,
                                  wireless_cmd_intf_disable_notes,
                                  wireless_cmd_uap_disable_cb );
    ASSERT( CMD_OK == result );    
#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "disable",
                                  wireless_cmd_intf_disable_desc,
                                  wireless_cmd_intf_disable_usage,
                                  wireless_cmd_intf_disable_notes,
                                  wireless_cmd_wfd_disable_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi sta",
                                  "disable",
                                  wireless_cmd_intf_disable_desc,
                                  wireless_cmd_intf_disable_usage,
                                  wireless_cmd_intf_disable_notes,
                                  wireless_cmd_sta_disable_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "defaults",
                                  wireless_cmd_intf_defaults_desc,
                                  wireless_cmd_intf_defaults_usage,
                                  wireless_cmd_intf_defaults_notes,
                                  wireless_cmd_uap_defaults_cb );
    ASSERT( CMD_OK == result );

#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "defaults",
                                  wireless_cmd_intf_defaults_desc,
                                  wireless_cmd_intf_defaults_usage,
                                  wireless_cmd_intf_defaults_notes,
                                  wireless_cmd_wfd_defaults_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi sta",
                                  "defaults",
                                  wireless_cmd_intf_defaults_desc,
                                  wireless_cmd_intf_defaults_usage,
                                  wireless_cmd_intf_defaults_notes,
                                  wireless_cmd_sta_defaults_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "info",
                                  wireless_cmd_intf_info_desc,
                                  wireless_cmd_intf_info_usage,
                                  wireless_cmd_intf_info_notes,
                                  wireless_cmd_uap_info_cb );
    ASSERT( CMD_OK == result );

#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "info",
                                  wireless_cmd_intf_info_desc,
                                  wireless_cmd_intf_info_usage,
                                  wireless_cmd_intf_info_notes,
                                  wireless_cmd_wfd_info_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi sta",
                                  "info",
                                  wireless_cmd_intf_info_desc,
                                  wireless_cmd_intf_info_usage,
                                  wireless_cmd_intf_info_notes,
                                  wireless_cmd_sta_info_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "ssid",
                                  wireless_cmd_ssid_desc,
                                  wireless_cmd_ssid_usage,
                                  wireless_cmd_ssid_notes,
                                  wireless_cmd_uap_ssid_cb );
    ASSERT( CMD_OK == result );

#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "ssid",
                                  wireless_cmd_ssid_desc,
                                  wireless_cmd_ssid_usage,
                                  wireless_cmd_ssid_notes,
                                  wireless_cmd_wfd_ssid_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi sta",
                                  "ssid",
                                  wireless_cmd_ssid_desc,
                                  wireless_cmd_ssid_usage,
                                  wireless_cmd_ssid_notes,
                                  wireless_cmd_sta_ssid_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "ssid_prefix",
                                  wireless_cmd_ssid_prefix_desc,
                                  wireless_cmd_ssid_prefix_usage,
                                  wireless_cmd_ssid_prefix_notes,
                                  wireless_cmd_uap_ssid_prefix_cb );
    ASSERT( CMD_OK == result );

#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "ssid_prefix",
                                  wireless_cmd_ssid_prefix_desc,
                                  wireless_cmd_ssid_prefix_usage,
                                  wireless_cmd_ssid_prefix_notes,
                                  wireless_cmd_wfd_ssid_prefix_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi uap",
                                  "security",
                                  wireless_cmd_security_desc,
                                  wireless_cmd_security_usage,
                                  wireless_cmd_security_notes,
                                  wireless_cmd_uap_security_cb );
    ASSERT( CMD_OK == result );

#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "security",
                                  wireless_cmd_security_desc,
                                  wireless_cmd_security_usage,
                                  wireless_cmd_security_notes,
                                  wireless_cmd_wfd_security_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi sta",
                                  "security",
                                  wireless_cmd_security_desc,
                                  wireless_cmd_security_usage,
                                  wireless_cmd_security_notes,
                                  wireless_cmd_sta_security_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "wep_auth_mode",
                                  wireless_cmd_wep_auth_mode_desc,
                                  wireless_cmd_wep_auth_mode_usage,
                                  wireless_cmd_wep_auth_mode_notes,
                                  wireless_cmd_uap_wep_auth_mode_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi sta",
                                  "wep_auth_mode",
                                  wireless_cmd_wep_auth_mode_desc,
                                  wireless_cmd_wep_auth_mode_usage,
                                  wireless_cmd_wep_auth_mode_notes,
                                  wireless_cmd_sta_wep_auth_mode_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "wep_key",
                                  wireless_cmd_wep_key_desc,
                                  wireless_cmd_wep_key_usage,
                                  wireless_cmd_wep_key_notes,
                                  wireless_cmd_uap_wep_key_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi sta",
                                  "wep_key",
                                  wireless_cmd_wep_key_desc,
                                  wireless_cmd_wep_key_usage,
                                  wireless_cmd_wep_key_notes,
                                  wireless_cmd_sta_wep_key_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "wep_cur_key",
                                  wireless_cmd_wep_cur_key_desc,
                                  wireless_cmd_wep_cur_key_usage,
                                  wireless_cmd_wep_cur_key_notes,
                                  wireless_cmd_uap_wep_cur_key_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi sta",
                                  "wep_cur_key",
                                  wireless_cmd_wep_cur_key_desc,
                                  wireless_cmd_wep_cur_key_usage,
                                  wireless_cmd_wep_cur_key_notes,
                                  wireless_cmd_sta_wep_cur_key_cb );
    ASSERT( CMD_OK == result );
#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "find",
                                  wireless_cmd_wfd_start_find_phase_desc,
                                  wireless_cmd_wfd_start_find_phase_usage,
                                  wireless_cmd_wfd_start_find_phase_notes,
                                  wireless_cmd_wfd_start_find_phase_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi wfd",
                                  "role",
                                  wireless_cmd_wfd_role_desc,
                                  wireless_cmd_wfd_role_usage,
                                  wireless_cmd_wfd_role_notes,
                                  wireless_cmd_wfd_role_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi wfd",
                                  "peer",
                                  wireless_cmd_wfd_peer_desc,
                                  wireless_cmd_wfd_peer_usage,
                                  wireless_cmd_wfd_peer_notes,
                                  wireless_cmd_wfd_peer_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi uap",
                                  "wpa_passphrase",
                                  wireless_cmd_wpa_passphrase_desc,
                                  wireless_cmd_wpa_passphrase_usage,
                                  wireless_cmd_wpa_passphrase_notes,
                                  wireless_cmd_uap_wpa_passphrase_cb );
    ASSERT( CMD_OK == result );

#ifdef WIFI_DIRECT_SUPPORT
    result = cmd_register_subcmd( "wifi wfd",
                                  "wpa_passphrase",
                                  wireless_cmd_wpa_passphrase_desc,
                                  wireless_cmd_wpa_passphrase_usage,
                                  wireless_cmd_wpa_passphrase_notes,
                                  wireless_cmd_wfd_wpa_passphrase_cb );
    ASSERT( CMD_OK == result );
#endif
    result = cmd_register_subcmd( "wifi sta",
                                  "wpa_passphrase",
                                  wireless_cmd_wpa_passphrase_desc,
                                  wireless_cmd_wpa_passphrase_usage,
                                  wireless_cmd_wpa_passphrase_notes,
                                  wireless_cmd_sta_wpa_passphrase_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi sta",
                                  "comm_mode",
                                  wireless_cmd_sta_comm_mode_desc,
                                  wireless_cmd_sta_comm_mode_usage,
                                  wireless_cmd_sta_comm_mode_notes,
                                  wireless_cmd_sta_comm_mode_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi uap",
                                  "wpa_enc_mode",
                                  wireless_cmd_uap_wpa_enc_mode_desc,
                                  wireless_cmd_uap_wpa_enc_mode_usage,
                                  wireless_cmd_uap_wpa_enc_mode_notes,
                                  wireless_cmd_uap_wpa_enc_mode_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi uap",
                                  "channel",
                                  wireless_cmd_uap_channel_desc,
                                  wireless_cmd_uap_channel_usage,
                                  wireless_cmd_uap_channel_notes,
                                  wireless_cmd_uap_channel_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi uap",
                                  "mac_filter_mode",
                                  wireless_cmd_uap_mac_filter_mode_desc,
                                  wireless_cmd_uap_mac_filter_mode_usage,
                                  wireless_cmd_uap_mac_filter_mode_notes,
                                  wireless_cmd_uap_mac_filter_mode_cb );
    ASSERT( CMD_OK == result );
   
    result = cmd_register_subcmd( "wifi uap",
                                  "mac_filter_add",
                                  wireless_cmd_uap_mac_filter_add_desc,
                                  wireless_cmd_uap_mac_filter_add_usage,
                                  wireless_cmd_uap_mac_filter_add_notes,
                                  wireless_cmd_uap_mac_filter_add_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi uap",
                                  "mac_filter_remove",
                                  wireless_cmd_uap_mac_filter_remove_desc,
                                  wireless_cmd_uap_mac_filter_remove_usage,
                                  wireless_cmd_uap_mac_filter_remove_notes,
                                  wireless_cmd_uap_mac_filter_remove_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi uap",
                                  "mac_filter_count",
                                  wireless_cmd_uap_mac_filter_count_desc,
                                  wireless_cmd_uap_mac_filter_count_usage,
                                  wireless_cmd_uap_mac_filter_count_notes,
                                  wireless_cmd_uap_mac_filter_count_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi uap",
                                  "mac_filter_flush",
                                  wireless_cmd_uap_mac_filter_flush_desc,
                                  wireless_cmd_uap_mac_filter_flush_usage,
                                  wireless_cmd_uap_mac_filter_flush_notes,
                                  wireless_cmd_uap_mac_filter_flush_cb );
    ASSERT( CMD_OK == result );

    
    result = cmd_register_subcmd( "wifi",
                                  "info",
                                  wireless_cmd_info_desc,
                                  wireless_cmd_info_usage,
                                  wireless_cmd_info_notes,
                                  wireless_cmd_info_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "enable",
                                  wireless_cmd_enable_desc,
                                  wireless_cmd_enable_usage,
                                  wireless_cmd_enable_notes,
                                  wireless_cmd_enable_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "disable",
                                  wireless_cmd_disable_desc,
                                  wireless_cmd_disable_usage,
                                  wireless_cmd_disable_notes,
                                  wireless_cmd_disable_cb );
    ASSERT( CMD_OK == result );
    #ifdef HAVE_HOTSPOT_SUPPORT
    if (wifi_hotspot_allowed_by_platform())
    {
        result = cmd_register_subcmd( "wifi",
                                      "hotspot",
                                      wireless_cmd_hotspot_desc,
                                      wireless_cmd_hotspot_usage,
                                      wireless_cmd_hotspot_notes,
                                      wireless_cmd_hotspot_cb );
    }
    #endif // HAVE_HOTSPOT_SUPPORT

    result = cmd_register_subcmd( "wifi",
                                  "customie",
                                  wireless_cmd_set_customie_desc,
                                  wireless_cmd_set_customie_usage,
                                  wireless_cmd_set_customie_notes,
                                  wireless_cmd_set_customie_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi",
                                  "defaults",
                                  wireless_cmd_defaults_desc,
                                  wireless_cmd_defaults_usage,
                                  wireless_cmd_defaults_notes,
                                  wireless_cmd_defaults_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "power",
                                  wireless_cmd_power_desc,
                                  wireless_cmd_power_usage,
                                  wireless_cmd_power_notes,
                                  wireless_cmd_power_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "region",
                                  wireless_cmd_region_desc,
                                  wireless_cmd_region_usage,
                                  wireless_cmd_region_notes,
                                  wireless_cmd_region_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "scan",
                                  wireless_cmd_scan_desc,
                                  wireless_cmd_scan_usage,
                                  wireless_cmd_scan_notes,
                                  wireless_cmd_scan_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "storedscan",
                                  wireless_cmd_storedscan_desc,
                                  wireless_cmd_storedscan_usage,
                                  wireless_cmd_storedscan_notes,
                                  wireless_cmd_storedscan_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "echo",
                                  wireless_cmd_echo_desc,
                                  wireless_cmd_echo_usage,
                                  wireless_cmd_echo_notes,
                                  wireless_cmd_echo_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "usbsuspend",
                                  wireless_cmd_usbsuspend_desc,
                                  wireless_cmd_usbsuspend_usage,
                                  wireless_cmd_usbsuspend_notes,
                                  wireless_cmd_usbsuspend_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "usbresume",
                                  wireless_cmd_usbresume_desc,
                                  wireless_cmd_usbresume_usage,
                                  wireless_cmd_usbresume_notes,
                                  wireless_cmd_usbresume_cb );
    ASSERT( CMD_OK == result );

#ifdef HAVE_WIFI_CERT_TEST    
    result = cmd_register_subcmd( "wifi",
                                  "sigmacmd",
                                  wireless_cmd_sigmacmd_desc,
                                  wireless_cmd_sigmacmd_usage,
                                  wireless_cmd_sigmacmd_notes,
                                  wireless_cmd_sigmacmd_cb );
    ASSERT( CMD_OK == result );
#endif

    result = cmd_register_subcmd( "wifi sta",
                                  "speed",
                                  wireless_cmd_sta_get_link_speed_desc,
                                  wireless_cmd_sta_get_link_speed_usage,
                                  wireless_cmd_sta_get_link_speed_notes,
                                  wireless_cmd_sta_get_link_speed_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi sta",
                                  "link",
                                  wireless_cmd_sta_link_status_desc,
                                  wireless_cmd_sta_link_status_usage,
                                  wireless_cmd_sta_link_status_notes,
                                  wireless_cmd_sta_link_status_cb );
    ASSERT( CMD_OK == result );
    
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_disable",
                                  wireless_cmd_uap_pri0_disable_desc,
                                  wireless_cmd_uap_pri0_disable_usage,
                                  wireless_cmd_uap_pri0_disable_notes,
                                  wireless_cmd_uap_pri0_disable_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_enable",
                                  wireless_cmd_uap_pri0_enable_desc,
                                  wireless_cmd_uap_pri0_enable_usage,
                                  wireless_cmd_uap_pri0_enable_notes,
                                  wireless_cmd_uap_pri0_enable_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_defaults",
                                  wireless_cmd_uap_pri0_defaults_desc,
                                  wireless_cmd_uap_pri0_defaults_usage,
                                  wireless_cmd_uap_pri0_defaults_notes,
                                  wireless_cmd_uap_pri0_defaults_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_info",
                                  wireless_cmd_uap_pri0_info_desc,
                                  wireless_cmd_uap_pri0_info_usage,
                                  wireless_cmd_uap_pri0_info_notes,
                                  wireless_cmd_uap_pri0_info_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_ssid",
                                  wireless_cmd_uap_pri0_ssid_desc,
                                  wireless_cmd_uap_pri0_ssid_usage,
                                  wireless_cmd_uap_pri0_ssid_notes,
                                  wireless_cmd_uap_pri0_ssid_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_ssid_prefix",
                                  wireless_cmd_uap_pri0_ssid_prefix_desc,
                                  wireless_cmd_uap_pri0_ssid_prefix_usage,
                                  wireless_cmd_uap_pri0_ssid_prefix_notes,
                                  wireless_cmd_uap_pri0_ssid_prefix_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_security",
                                  wireless_cmd_uap_pri0_security_desc,
                                  wireless_cmd_uap_pri0_security_usage,
                                  wireless_cmd_uap_pri0_security_notes,
                                  wireless_cmd_uap_pri0_security_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_wep_auth_mode",
                                  wireless_cmd_uap_pri0_wep_auth_mode_desc,
                                  wireless_cmd_uap_pri0_wep_auth_mode_usage,
                                  wireless_cmd_uap_pri0_wep_auth_mode_notes,
                                  wireless_cmd_uap_pri0_wep_auth_mode_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_wep_key",
                                  wireless_cmd_uap_pri0_wep_key_desc,
                                  wireless_cmd_uap_pri0_wep_key_usage,
                                  wireless_cmd_uap_pri0_wep_key_notes,
                                  wireless_cmd_uap_pri0_wep_key_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_wep_cur_key",
                                  wireless_cmd_uap_pri0_wep_cur_key_desc,
                                  wireless_cmd_uap_pri0_wep_cur_key_usage,
                                  wireless_cmd_uap_pri0_wep_cur_key_notes,
                                  wireless_cmd_uap_pri0_wep_cur_key_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_wpa_passphrase",
                                  wireless_cmd_uap_pri0_wpa_passphrase_desc,
                                  wireless_cmd_uap_pri0_wpa_passphrase_usage,
                                  wireless_cmd_uap_pri0_wpa_passphrase_notes,
                                  wireless_cmd_uap_pri0_wpa_passphrase_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_wpa_enc_mode",
                                  wireless_cmd_uap_pri0_wpa_enc_mode_desc,
                                  wireless_cmd_uap_pri0_wpa_enc_mode_usage,
                                  wireless_cmd_uap_pri0_wpa_enc_mode_notes,
                                  wireless_cmd_uap_pri0_wpa_enc_mode_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_channel",
                                  wireless_cmd_uap_pri0_channel_desc,
                                  wireless_cmd_uap_pri0_channel_usage,
                                  wireless_cmd_uap_pri0_channel_notes,
                                  wireless_cmd_uap_pri0_channel_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_mac_filter_mode",
                                  wireless_cmd_uap_pri0_mac_filter_mode_desc,
                                  wireless_cmd_uap_pri0_mac_filter_mode_usage,
                                  wireless_cmd_uap_pri0_mac_filter_mode_notes,
                                  wireless_cmd_uap_pri0_mac_filter_mode_cb );
    ASSERT( CMD_OK == result );
   
    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_mac_filter_add",
                                  wireless_cmd_uap_pri0_mac_filter_add_desc,
                                  wireless_cmd_uap_pri0_mac_filter_add_usage,
                                  wireless_cmd_uap_pri0_mac_filter_add_notes,
                                  wireless_cmd_uap_pri0_mac_filter_add_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_mac_filter_remove",
                                  wireless_cmd_uap_pri0_mac_filter_remove_desc,
                                  wireless_cmd_uap_pri0_mac_filter_remove_usage,
                                  wireless_cmd_uap_pri0_mac_filter_remove_notes,
                                  wireless_cmd_uap_pri0_mac_filter_remove_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_mac_filter_count",
                                  wireless_cmd_uap_pri0_mac_filter_count_desc,
                                  wireless_cmd_uap_pri0_mac_filter_count_usage,
                                  wireless_cmd_uap_pri0_mac_filter_count_notes,
                                  wireless_cmd_uap_pri0_mac_filter_count_cb );
    ASSERT( CMD_OK == result );

    result = cmd_register_subcmd( "wifi",
                                  "uap_pri0_mac_filter_flush",
                                  wireless_cmd_uap_pri0_mac_filter_flush_desc,
                                  wireless_cmd_uap_pri0_mac_filter_flush_usage,
                                  wireless_cmd_uap_pri0_mac_filter_flush_notes,
                                  wireless_cmd_uap_pri0_mac_filter_flush_cb );
    ASSERT( CMD_OK == result );
    
    result = cmd_register_subcmd( "wifi",
                                  "wws",
                                  wireless_cmd_wws_config_desc,
                                  wireless_cmd_wws_config_usage,
                                  wireless_cmd_wws_config_notes,
                                  wireless_cmd_wws_config_cb );
    ASSERT( CMD_OK == result );
    result = cmd_register_subcmd( "wifi",
                                  "uap_active_config",
                                  wireless_cmd_uap_active_config_desc,
                                  wireless_cmd_uap_active_config_usage,
                                  wireless_cmd_uap_active_config_notes,
                                  wireless_cmd_uap_active_config_cb );
    ASSERT( CMD_OK == result );
    
#endif
}
