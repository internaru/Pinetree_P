/** @file wfd_action.c
 *  @brief This file contains WFD action frame, WLAN commands
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "wps_util.h"
#include "wps_def.h"
#include "wps_wlan.h"
#include "wps_os.h"
#include "wlan_hostcmd.h"
#include "wlan_wfd.h"
#include "wireless_copy.h"
#include "memAPI.h"
#include "wlanInterface.h"

#include "logger.h"

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
#if 0
/********************************************************
  Local Functions
 ********************************************************/
/** 
 *  @brief Converts colon separated MAC address to hex value
 *
 *  @param mac      A pointer to the colon separated MAC string
 *  @param raw      A pointer to the hex data buffer
 *  @return         SUCCESS or FAILURE
 *                  WFD_RET_MAC_BROADCAST  - if broadcast mac
 *                  WFD_RET_MAC_MULTICAST - if multicast mac
 */
static int
mac2raw(char *mac, u8 * raw)
{
    unsigned int temp_raw[ETH_ALEN];
    int num_tokens = 0;
    int i;
    if (strlen(mac) != ((2 * ETH_ALEN) + (ETH_ALEN - 1))) {
        return FAILURE;
    }
    num_tokens = sscanf(mac, "%2x:%2x:%2x:%2x:%2x:%2x",
                        temp_raw + 0, temp_raw + 1, temp_raw + 2, temp_raw + 3,
                        temp_raw + 4, temp_raw + 5);
    if (num_tokens != ETH_ALEN) {
        return FAILURE;
    }
    for (i = 0; i < num_tokens; i++)
        raw[i] = (u8) temp_raw[i];

    if (memcmp(raw, "\xff\xff\xff\xff\xff\xff", ETH_ALEN) == 0) {
        return WFD_RET_MAC_BROADCAST;
    } else if (raw[0] & 0x01) {
        return WFD_RET_MAC_MULTICAST;
    }
    return SUCCESS;
}

/**
 *    @brief isdigit for String.
 *   
 *    @param x            Char string
 *    @return             FAILURE for non-digit.
 *                        SUCCESS for digit
 */
static inline int
ISDIGIT(char *x)
{
    int i;
    for (i = 0; i < strlen(x); i++)
        if (isdigit(x[i]) == 0)
            return FAILURE;
    return SUCCESS;
}

/** 
 *  @brief              Check hex string
 *  
 *  @param hex          A pointer to hex string
 *  @return             SUCCESS or FAILURE
 */
static int
ishexstring(void *hex)
{
    int i, a;
    char *p = hex;
    int len = strlen(p);
    if (!strncasecmp("0x", p, 2)) {
        p += 2;
        len -= 2;
    }
    for (i = 0; i < len; i++) {
        a = hex2num(*p);
        if (a < 0)
            return FAILURE;
        p++;
    }
    return SUCCESS;
}

/**
 *  @brief  Detects duplicates channel in array of strings
 *          
 *  @param  argc    Number of elements
 *  @param  argv    Array of strings
 *  @return FAILURE or SUCCESS
 */
static inline int
has_dup_channel(int argc, char *argv[])
{
    int i, j;
    /* Check for duplicate */
    for (i = 0; i < (argc - 1); i++) {
        for (j = i + 1; j < argc; j++) {
            if (atoi(argv[i]) == atoi(argv[j])) {
                return FAILURE;
            }
        }
    }
    return SUCCESS;
}

/** 
 *  @brief Checkes a particular input for validatation.
 *
 *  @param cmd      Type of input
 *  @param argc     Number of arguments
 *  @param argv     Pointer to the arguments
 *  @return         SUCCESS or FAILURE
 */
int
is_input_valid(valid_inputs cmd, int argc, char *argv[])
{
    int i;
    int ret = SUCCESS;
    char country[6] = { ' ', ' ', 0, 0, 0, 0 };
    char wfd_dev_name[34];
    if (argc == 0)
        return FAILURE;
    switch (cmd) {
    case WFD_MINDISCOVERYINT:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for MinDiscoveryInterval\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) >= (1 << 16))) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:MinDiscoveryInterval must be 2 bytes\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_MAXDISCOVERYINT:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for MaxDiscoveryInterval\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) >= (1 << 16))) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:MaxDiscoveryInterval must be 2 bytes\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_DEVICECAPABILITY:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for DeviceCapability\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > MAX_DEV_CAPABILITY)
                || (atoi(argv[0]) < 0)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:DeviceCapabilty must be in the range [0:%d]\n",
                       MAX_DEV_CAPABILITY));
                ret = FAILURE;
            }
        }
        break;
    case WFD_GROUPCAPABILITY:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for GroupCapability\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > MAX_GRP_CAPABILITY)
                || (atoi(argv[0]) < 0)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:GroupCapabilty must be in the range [0:%d]\n",
                       MAX_GRP_CAPABILITY));
                ret = FAILURE;
            }
        }
        break;
    case CHANNEL:
        if ((argc != 1) && (argc != 2)) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Incorrect arguments for channel.\n"));
            ret = FAILURE;
        } else {
            if (argc == 2) {
                if ((ISDIGIT(argv[1]) == 0) || (atoi(argv[1]) < 0) ||
                    (atoi(argv[1]) > 1)) {
                    DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: MODE must be either 0 or 1\n"));
                    ret = FAILURE;
                }
                if ((atoi(argv[1]) == 1) && (atoi(argv[0]) != 0)) {
                    DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Channel must be 0 for ACS; MODE = 1.\n"));
                    ret = FAILURE;
                }
            }
            if ((argc == 1) || (atoi(argv[1]) == 0)) {
                if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) < 1) ||
                    (atoi(argv[0]) > MAX_CHANNELS)) {
                    DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Channel must be in the range of 1 to %d\n",
                           MAX_CHANNELS));
                    ret = FAILURE;
                }
            }
        }
        break;
    case SCANCHANNELS:
        if (argc > MAX_CHANNELS) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Invalid List of Channels\n"));
            ret = FAILURE;
        } else {
            for (i = 0; i < argc; i++) {
                if ((ISDIGIT(argv[i]) == 0) || (atoi(argv[i]) < 1) ||
                    (atoi(argv[i]) > MAX_CHANNELS)) {
                    DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Channel must be in the range of 1 to %d\n",
                           MAX_CHANNELS));
                    ret = FAILURE;
                    break;
                }
            }
            if ((ret != FAILURE) && (has_dup_channel(argc, argv) != SUCCESS)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Duplicate channel values entered\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_INTENT:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for intent\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > MAX_INTENT) ||
                (atoi(argv[0]) < 0)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Intent must be in the range [0:%d]\n", MAX_INTENT));
                ret = FAILURE;
            }
        }
        break;
    case WFD_MANAGEABILITY:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for manageability\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) < 0) ||
                (atoi(argv[0]) > 1)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Manageability must be either 0 or 1\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_INVITATIONFLAG:
        if (argc != 1) {
            printf("ERR:Incorrect number of arguments for Invitation Flag\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) < 0) ||
                (atoi(argv[0]) > 1)) {
                printf("ERR:Invitation Flag must be either 0 or 1\n");
                ret = FAILURE;
            }
        }
        break;
    case WFD_GROUP_WFD_DEVICE_NAME:
        /* 2 extra characters for quotes around device name */
        if ((strlen(argv[0]) > 34)) {
            printf
                ("ERR:WFD Device name string length must not be more than 32\n");
            ret = FAILURE;
        } else {
            strcpy(wfd_dev_name, argv[0]);
            if ((wfd_dev_name[0] != '"') ||
                (wfd_dev_name[strlen(wfd_dev_name) - 1] != '"')) {
                printf("ERR:WFD Device name must be within double quotes!\n");
                ret = FAILURE;
            }
        }
        break;
    case WFD_COUNTRY:
        /* 2 extra characters for quotes around country */
        if ((strlen(argv[0]) > 5) || (strlen(argv[0]) < 4)) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Country string must have length 2 or 3\n"));
            ret = FAILURE;
        } else {
            strcpy(country, argv[0]);
            if ((country[0] != '"') || (country[strlen(country) - 1] != '"')) {
                printf("ERR:country code must be within double quotes!\n");
                ret = FAILURE;
            } else {
                for (i = 1; i < strlen(country) - 2; i++) {
                    if ((toupper(country[i]) < 'A') ||
                        (toupper(country[i]) > 'Z')) {
	                    DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Invalid Country Code\n"));
                        ret = FAILURE;
                    }
                }
            }
        }
        break;
    case WFD_NO_OF_CHANNELS:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for num of channels\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > MAX_CHANNELS)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Number of channels should be less than %d\n",
                       MAX_CHANNELS));
                ret = FAILURE;
            }
        }
        break;
    case WFD_NOA_INDEX:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for NoA Index\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) < MIN_NOA_INDEX) ||
                (atoi(argv[0]) > MAX_NOA_INDEX)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:NoA index should be in the range [%d:%d]\n",
                       MIN_NOA_INDEX, MAX_NOA_INDEX));
                ret = FAILURE;
            }
        }
        break;
    case WFD_OPP_PS:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for Opp PS\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || ((atoi(argv[0]) != 0) &&
                                            (atoi(argv[0]) != 1))) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Opp PS must be either 0 or 1\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_CTWINDOW:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for CTWindow\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > MAX_CTWINDOW) ||
                (atoi(argv[0]) < MIN_CTWINDOW)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:CT Window must be in the range [%d:%d]\n",
                       MIN_CTWINDOW, MAX_CTWINDOW));
                ret = FAILURE;
            }
        }
        break;
    case WFD_COUNT_TYPE:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for Count/Type\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) == 0) ||
                (atoi(argv[0]) > MAX_COUNT_TYPE) ||
                (atoi(argv[0]) < MIN_COUNT_TYPE)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Count/Type must be in the range [%d:%d] or 255\n",
                       MIN_COUNT_TYPE, MAX_COUNT_TYPE));
                ret = FAILURE;
            }
        }
        break;
    case WFD_DURATION:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for Duration\n"));
            ret = FAILURE;
        }
        break;
    case WFD_INTERVAL:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for Interval\n"));
            ret = FAILURE;
        }
        break;
    case WFD_START_TIME:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for Start Time\n"));
            ret = FAILURE;
        }
        break;
    case WFD_PRIDEVTYPEOUI:
        if (argc > MAX_PRIMARY_OUI_LEN) {
            dprintf
                ("ERR: Incorrect number of PrimaryDeviceTypeOUI arguments.\n");
            ret = FAILURE;
            break;
        }
        for (i = 0; i < argc; i++) {
            if (IS_HEX_OR_DIGIT(argv[i]) == FAILURE) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Unsupported OUI\n"));
                ret = FAILURE;
                break;
            }
        }
        if (!((A2HEXDECIMAL(argv[0]) == 0x00) && (A2HEXDECIMAL(argv[1]) == 0x50)
              && (A2HEXDECIMAL(argv[2]) == 0xF2) &&
              (A2HEXDECIMAL(argv[3]) == 0x04))) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Unsupported OUI\n"));
            ret = FAILURE;
            break;
        }
        break;
    case WFD_REGULATORYCLASS:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for RegulatoryClass\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > MAX_REG_CLASS) ||
                (atoi(argv[0]) < MIN_REG_CLASS)) {
                dprintf
                    ("ERR:RegulatoryClass must be in the range [%d:%d] or 255\n",
                     MIN_REG_CLASS, MAX_REG_CLASS);
                ret = FAILURE;
            }
        }
        break;
    case WFD_PRIDEVTYPECATEGORY:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for PrimaryDeviceTypeCategory\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > MAX_PRIDEV_TYPE_CAT)
                || (atoi(argv[0]) < MIN_PRIDEV_TYPE_CAT)) {
                dprintf
                    ("ERR:PrimaryDeviceTypeCategory must be in the range [%d:%d]\n",
                     MIN_PRIDEV_TYPE_CAT, MAX_PRIDEV_TYPE_CAT);
                ret = FAILURE;
            }
        }
        break;
    case WFD_SECONDARYDEVCOUNT:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for SecondaryDeviceCount\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) ||
                (atoi(argv[0]) > MAX_SECONDARY_DEVICE_COUNT)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:SecondaryDeviceCount must be less than 15.\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_GROUP_SECONDARYDEVCOUNT:
        if (argc != 1) {
            printf
                ("ERR:Incorrect number of arguments for SecondaryDeviceCount\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) ||
                (atoi(argv[0]) > MAX_GROUP_SECONDARY_DEVICE_COUNT)) {
                printf("ERR:SecondaryDeviceCount must be less than 2.\n");
                ret = FAILURE;
            }
        }
        break;
    case WFD_INTERFACECOUNT:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for InterfaceCount\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) ||
                (atoi(argv[0]) > MAX_INTERFACE_ADDR_COUNT)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:IntefaceCount must be in range.[0-41]\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_ATTR_CONFIG_TIMEOUT:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for Timeout Configuration\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > 255)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:TimeoutConfig must be in the range [0:255]\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_ATTR_EXTENDED_TIME:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for Extended time.\n"));
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || (atoi(argv[0]) > 65535) ||
                (atoi(argv[0]) < 1)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Extended Time must be in the range [1:65535]\n"));
                ret = FAILURE;
            }
        }
        break;

    case WFD_PRIDEVTYPESUBCATEGORY:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for PrimaryDeviceTypeSubCategory\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) ||
                (atoi(argv[0]) > MAX_PRIDEV_TYPE_SUBCATEGORY) ||
                (atoi(argv[0]) < MIN_PRIDEV_TYPE_SUBCATEGORY)) {
                dprintf
                    ("ERR:PrimaryDeviceTypeSubCategory must be in the range [%d:%d]\n",
                     MIN_PRIDEV_TYPE_SUBCATEGORY, MAX_PRIDEV_TYPE_SUBCATEGORY);
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSCONFMETHODS:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for WPSConfigMethods\n"));
            ret = FAILURE;
        } else {
            if ((IS_HEX_OR_DIGIT(argv[0]) == 0) ||
                (A2HEXDECIMAL(argv[0]) > MAX_WPS_CONF_METHODS) ||
                (A2HEXDECIMAL(argv[0]) < MIN_WPS_CONF_METHODS)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:WPSConfigMethods must be in the range [%d:%d]\n",
                       MIN_WPS_CONF_METHODS, MAX_WPS_CONF_METHODS));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSVERSION:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for WPSVersion\n"));
            ret = FAILURE;
        } else {
            if ((A2HEXDECIMAL(argv[0]) < 0x10) &&
                (A2HEXDECIMAL(argv[0]) > 0x20)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect WPS Version %s\n", argv[0]));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSSETUPSTATE:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for WPSSetupState\n"));
            ret = FAILURE;
        } else {
            if ((IS_HEX_OR_DIGIT(argv[0]) == 0) ||
                ((A2HEXDECIMAL(argv[0]) != 0x01)
                 && (A2HEXDECIMAL(argv[0]) != 0x02))) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect WPSSetupState %s\n", argv[0]));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSREQRESPTYPE:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for WPSRequestType\n"));
            ret = FAILURE;
        } else {
            if ((IS_HEX_OR_DIGIT(argv[0]) == 0) ||
                (A2HEXDECIMAL(argv[0]) < WPS_MIN_REQUESTTYPE) ||
                (A2HEXDECIMAL(argv[0]) > WPS_MAX_REQUESTTYPE)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect WPSRequestType %s\n", argv[0]));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSSPECCONFMETHODS:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for WPSSpecConfigMethods\n");
            ret = FAILURE;
        } else {
            if ((IS_HEX_OR_DIGIT(argv[0]) == 0) ||
                ((u16) A2HEXDECIMAL(argv[0]) > MAX_WPS_CONF_METHODS) ||
                ((u16) A2HEXDECIMAL(argv[0]) < MIN_WPS_CONF_METHODS)) {
                dprintf
                    ("ERR:WPSSpecConfigMethods must be in the range [%d:%d]\n",
                     MIN_WPS_CONF_METHODS, MAX_WPS_CONF_METHODS);
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSDEVICENAME:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments\n"));
            ret = FAILURE;
        } else {
            if (strlen(argv[0]) > WPS_DEVICE_NAME_MAX_LEN) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Device name should contain"
                       " less than 32 charactors.\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSMANUFACTURER:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments\n"));
            ret = FAILURE;
        } else {
            if (strlen(argv[0]) > WPS_MANUFACT_MAX_LEN) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Manufacturer name should contain"
                       "less than 64 charactors.\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSMODELNAME:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments\n"));
            ret = FAILURE;
        } else {
            if (strlen(argv[0]) > WPS_MODEL_MAX_LEN) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Model name should contain"
                       " less than 64 charactors.\n"));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSUUID:
        if (argc > WPS_UUID_MAX_LEN) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Incorrect number of WPSUUID arguments.\n"));
            ret = FAILURE;
        } else {
            for (i = 0; i < argc; i++) {
                if (IS_HEX_OR_DIGIT(argv[i]) == FAILURE) {
                    DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Unsupported UUID\n"));
                    ret = FAILURE;
                    break;
                }
            }
        }
        break;
    case WFD_WPSPRIMARYDEVICETYPE:
        if (argc > WPS_DEVICE_TYPE_MAX_LEN) {
            dprintf
                ("ERR: Incorrect number of WPSPrimaryDeviceType arguments.\n");
            ret = FAILURE;
            break;
        }
        for (i = 0; i < argc; i++) {
            if (IS_HEX_OR_DIGIT(argv[i]) == FAILURE) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Unsupported primary device type\n"));
                ret = FAILURE;
                break;
            }
        }
        if (!((A2HEXDECIMAL(argv[1]) == 0x00) && (A2HEXDECIMAL(argv[2]) == 0x50)
              && (A2HEXDECIMAL(argv[3]) == 0xF2) &&
              (A2HEXDECIMAL(argv[4]) == 0x04))) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Unsupported OUI\n"));
            ret = FAILURE;
            break;
        }
        break;
    case WFD_WPSRFBAND:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for WPSRFBand\n"));
            ret = FAILURE;
        } else {
            if ((IS_HEX_OR_DIGIT(argv[0]) == 0) ||
                ((A2HEXDECIMAL(argv[0]) != 0x01)
                 && (A2HEXDECIMAL(argv[0]) != 0x02))) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect WPSRFBand %s\n", argv[0]));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSASSOCIATIONSTATE:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for WPSAssociationState\n");
            ret = FAILURE;
        } else {
            if ((IS_HEX_OR_DIGIT(argv[0]) == 0) ||
                ((u16) A2HEXDECIMAL(argv[0]) > WPS_MAX_ASSOCIATIONSTATE) ||
                ((u16) A2HEXDECIMAL(argv[0]) < WPS_MIN_ASSOCIATIONSTATE)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:WPSAssociationState must be in the range [%d:%d]\n",
                       WPS_MIN_ASSOCIATIONSTATE, WPS_MAX_ASSOCIATIONSTATE));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSCONFIGURATIONERROR:
        if (argc != 1) {
            dprintf
                ("ERR:Incorrect number of arguments for WPSConfigurationError\n");
            ret = FAILURE;
        } else {
            if ((IS_HEX_OR_DIGIT(argv[0]) == 0) ||
                ((u16) A2HEXDECIMAL(argv[0]) > WPS_MAX_CONFIGURATIONERROR) ||
                ((u16) A2HEXDECIMAL(argv[0]) < WPS_MIN_CONFIGURATIONERROR)) {
                dprintf
                    ("ERR:WPSConfigurationError must be in the range [%d:%d]\n",
                     WPS_MIN_CONFIGURATIONERROR, WPS_MAX_CONFIGURATIONERROR);
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSDEVICEPASSWORD:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for WPSDevicePassword\n"));
            ret = FAILURE;
        } else {
            if ((IS_HEX_OR_DIGIT(argv[0]) == 0) ||
                ((u16) A2HEXDECIMAL(argv[0]) > WPS_MAX_DEVICEPASSWORD) ||
                ((u16) A2HEXDECIMAL(argv[0]) < WPS_MIN_DEVICEPASSWORD)) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:WPSDevicePassword must be in the range [%d:%d]\n",
                       WPS_MIN_DEVICEPASSWORD, WPS_MAX_DEVICEPASSWORD));
                ret = FAILURE;
            }
        }
        break;
    case WFD_WPSMODELNUMBER:
        if (argc > WPS_MODEL_MAX_LEN) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Incorrect number of WPSModelNumber arguments.\n"));
            ret = FAILURE;
        } else {
            for (i = 0; i < argc; i++) {
                if (IS_HEX_OR_DIGIT(argv[i]) == FAILURE) {
                    DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Unsupported WPSModelNumber\n"));
                    ret = FAILURE;
                    break;
                }
            }
        }
        break;
    case WFD_WPSSERIALNUMBER:
        if (argc > WPS_SERIAL_MAX_LEN) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: Incorrect number of WPSSerialNumber arguments.\n"));
            ret = FAILURE;
        } else {
            for (i = 0; i < argc; i++) {
                if (IS_HEX_OR_DIGIT(argv[i]) == FAILURE) {
                    DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Unsupported WPSSerialNumber\n"));
                    ret = FAILURE;
                    break;
                }
            }
        }
        break;
    case WFD_ENABLE_SCAN:
        if (argc != 1) {
            printf("ERR:Incorrect number of arguments for EnableScan\n");
            ret = FAILURE;
        } else {
            if ((ISDIGIT(argv[0]) == 0) || ((atoi(argv[0]) != 0) &&
                                            (atoi(argv[0]) != 1))) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:EnableScan must be 0 or 1.\n"));
                ret = FAILURE;
            }
        }
        break;

    case WFD_DEVICE_STATE:
        if (argc != 1) {
            DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect number of arguments for DeviceState\n"));
            ret = FAILURE;
        } else {
            if (ISDIGIT(argv[0]) == 0) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Incorrect DeviceState.\n"));
                ret = FAILURE;
            }
        }
        break;

    default:
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("Parameter %d ignored\n", cmd));
        break;
    }
    return ret;
}

/** 
 *  @brief Parses a command line
 *
 *  @param line     The line to parse
 *  @param args     Pointer to the argument buffer to be filled in
 *  @return         Number of arguments in the line or EOF
 */
static int
parse_line(char *line, char *args[])
{
    int arg_num = 0;
    int is_start = 0;
    int is_quote = 0;
    int length = 0;
    int i = 0;

    arg_num = 0;
    length = strlen(line);
    /* Process line */

    /* Find number of arguments */
    is_start = 0;
    is_quote = 0;
    for (i = 0; i < length; i++) {
        /* Ignore leading spaces */
        if (is_start == 0) {
            if (line[i] == ' ') {
                continue;
            } else if (line[i] == '\t') {
                continue;
            } else if (line[i] == '\n') {
                break;
            } else {
                is_start = 1;
                args[arg_num] = &line[i];
                arg_num++;
            }
        }
        if (is_start == 1) {
            /* Ignore comments */
            if (line[i] == '#') {
                if (is_quote == 0) {
                    line[i] = '\0';
                    arg_num--;
                }
                break;
            }
            /* Separate by '=' */
            if (line[i] == '=') {
                line[i] = '\0';
                is_start = 0;
                continue;
            }
            /* Separate by ',' */
            if (line[i] == ',') {
                line[i] = '\0';
                is_start = 0;
                continue;
            }
            /* Change ',' to ' ', but not inside quotes */
            if ((line[i] == ',') && (is_quote == 0)) {
                line[i] = ' ';
                continue;
            }
        }
        /* Remove newlines */
        if (line[i] == '\n') {
            line[i] = '\0';
        }
        /* Check for quotes */
        if (line[i] == '"') {
            is_quote = (is_quote == 1) ? 0 : 1;
            continue;
        }
        if (((line[i] == ' ') || (line[i] == '\t')) && (is_quote == 0)) {
            line[i] = '\0';
            is_start = 0;
            continue;
        }
    }
    return arg_num;
}

/** 
 *  @brief Get one line from the File
 *  
 *  @param fp       File handler
 *  @param str	    Storage location for data.
 *  @param size 	Maximum number of characters to read. 
 *  @param lineno	A pointer to return current line number
 *  @return         returns string or NULL 
 */
char *
mlan_config_get_line(FILE * fp, s8 * str, s32 size, int *lineno)
{
    char *start, *end;
    int out, next_line;

    if (!fp || !str)
        return NULL;

    do {
      read_line:
        if (!fgets((char *) str, size, fp))
            break;
        start = (char *) str;
        start[size - 1] = '\0';
        end = start + strlen((char *) str);
        (*lineno)++;

        out = 1;
        while (out && (start < end)) {
            next_line = 0;
            /* Remove empty lines and lines starting with # */
            switch (start[0]) {
            case ' ':          /* White space */
            case '\t':         /* Tab */
                start++;
                break;
            case '#':
            case '\n':
            case '\0':
                next_line = 1;
                break;
            case '\r':
                if (start[1] == '\n')
                    next_line = 1;
                else
                    start++;
                break;
            default:
                out = 0;
                break;
            }
            if (next_line)
                goto read_line;
        }

        /* Remove # comments unless they are within a double quoted string.
           Remove trailing white space. */
        if ((end = strstr(start, "\""))) {
            if (!(end = strstr(end + 1, "\"")))
                end = start;
        } else
            end = start;

        if ((end = strstr(end + 1, "#")))
            *end-- = '\0';
        else
            end = start + strlen(start) - 1;

        out = 1;
        while (out && (start < end)) {
            switch (*end) {
            case ' ':          /* White space */
            case '\t':         /* Tab */
            case '\n':
            case '\r':
                *end = '\0';
                end--;
                break;
            default:
                out = 0;
                break;
            }
        }

        if (start == '\0')
            continue;

        return start;
    } while (1);

    return NULL;
}

/** 
 *  @brief          Parse function for a configuration line  
 *
 *  @param s        Storage buffer for data
 *  @param size     Maximum size of data
 *  @param stream   File stream pointer
 *  @param line     Pointer to current line within the file
 *  @param _pos     Output string or NULL
 *  @return         String or NULL
 */
static char *
config_get_line(char *s, int size, FILE * stream, int *line, char **_pos)
{
    *_pos = mlan_config_get_line(stream, (s8 *) s, size, line);
    return *_pos;
}
#endif

/** 
 *  @brief Process and send ie config command 
 *  @param ie_index  A pointer to the IE buffer index
 *  @param data_len_wfd  Length of P2P data, 0 to get, else set.
 *  @param data_len_wps  Length of WPS data, 0 to get, else set.
 *  @param buf      Pointer to buffer to set.
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfd_ie_config(u16 * ie_index, u16 data_len_wfd, u16 data_len_wps, u8 * buf)
{
    int i, ret = WPS_STATUS_SUCCESS;
    tlvbuf_custom_ie *tlv = NULL;
    custom_ie *ie_ptr = NULL;

    ENTER();
    tlv = (tlvbuf_custom_ie *) buf;
    tlv->tag = MRVL_MGMT_IE_LIST_TLV_ID;
    /* Locate headers */
    ie_ptr = (custom_ie *) (tlv->ie_data);

    /* Set TLV fields : WFD IE parameters */
    if (data_len_wfd) {
        /* Set IE */
#define MASK_WFD_AUTO 0xFFFF
        ie_ptr->mgmt_subtype_mask = MASK_WFD_AUTO;
        tlv->length = sizeof(custom_ie) + data_len_wfd;
        ie_ptr->ie_length = data_len_wfd;
        ie_ptr->ie_index = *ie_index;
    } else {
        /* Get WPS IE */
        tlv->length = 0;
    }

    (*ie_index)++;

    /* Locate headers */
    ie_ptr =
        (custom_ie *) ((u8 *) (tlv->ie_data) + sizeof(custom_ie) +
                       data_len_wfd);

    /* Set WPS IE parameters */
    if (data_len_wps) {
        /* Set IE */
        ie_ptr->mgmt_subtype_mask = MASK_WFD_AUTO;
        tlv->length += sizeof(custom_ie) + data_len_wps;
        ie_ptr->ie_length = data_len_wps;
        ie_ptr->ie_index = *ie_index;
    }

    ret = wlan_custom_ie_config(buf);

    if (ret == WPS_STATUS_SUCCESS) {
        wps_printf(DEBUG_WLAN, "custom IE ioctl success.\n");

        if (!data_len_wfd) {
            /* Get the IE buffer index number for MGMT_IE_LIST_TLV */
            tlv = (tlvbuf_custom_ie *) buf;
            *ie_index = 0xFFFF;
            if (tlv->tag == MRVL_MGMT_IE_LIST_TLV_ID) {
                ie_ptr = (custom_ie *) (tlv->ie_data);
                for (i = 0; i < MAX_MGMT_IE_INDEX; i++) {
                    /* mask 0xFFFF indicates a wfd IE, return index */
                    if (ie_ptr->mgmt_subtype_mask == MASK_WFD_AUTO &&
                        ie_ptr->ie_length) {
                        *ie_index = ie_ptr->ie_index;
                        wps_printf(DEBUG_WLAN, "Found Index %d\n", i);
                        break;
                    }
                    if (i < (MAX_MGMT_IE_INDEX - 1))
                        ie_ptr =
                            (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
                                           ie_ptr->ie_length);
                }
            }
            if (*ie_index == 0xFFFF) {
                wps_printf(MSG_WARNING, "No free IE buffer available\n");
                ret = WPS_STATUS_FAIL;
            }
        }
    } else {
        wps_printf(DEBUG_WLAN, "custom IE ioctl failed.\n");
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief  Check if intended address field is configured.
 *          If yes, update the corresponding device ID.
 *  @param  wfd_buf      WFD buffer pointer
 *  @param  wfd_buf_len  WFD buffer length 
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
static int
wfd_check_set_intended_addr(u8 * wfd_buf, u16 wfd_buf_len)
{
    tlvbuf_wfd_intended_addr *wfd_tlv;
    u8 *ptr = wfd_buf;
    s16 left_len = wfd_buf_len;
    u16 len = 0;

    ENTER();

    while (left_len > WFD_IE_HEADER_LEN) {

        if (*ptr == TLV_TYPE_WFD_INTENDED_ADDRESS) {
            wfd_tlv = (tlvbuf_wfd_intended_addr *) ptr;

            /* Address is found, set it now */
            wps_printf(DEBUG_WFD_DISCOVERY, "Intended address. " MACSTR,
                       MAC2STR(wfd_tlv->group_address));

            memcpy(gpwps_info->wfd_intended_addr, wfd_tlv->group_address,
                   ETH_ALEN);
            LEAVE();
            return WPS_STATUS_SUCCESS;
        }

        memcpy(&len, ptr + 1, sizeof(u16));
        len = wlan_le16_to_cpu(len);

        ptr += len + WFD_IE_HEADER_LEN;
        left_len -= len + WFD_IE_HEADER_LEN;
    }

    LEAVE();
    return WPS_STATUS_FAIL;
}

/** 
 *  @brief  Check if channel list is configured.
 *          If yes, update the self_channel_list of global wfd_data structure.
 *  @param  wfd_buf      WFD buffer pointer
 *  @param  wfd_buf_len  WFD buffer length 
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
static int
wfd_check_self_channel_list(u8 * wfd_buf, u16 wfd_buf_len)
{
    tlvbuf_wfd_channel_list *wfd_tlv;
    u8 *ptr = wfd_buf;
    s16 left_len = wfd_buf_len;
    u16 len = 0, len_wifidirect = 0;
    chan_entry *temp_ptr;
    WFD_DATA *pwfd_data = &(wps_global.wfd_data);
    int i = 0;

    ENTER();

    while (left_len > WFD_IE_HEADER_LEN) {

        if (*ptr == TLV_TYPE_WFD_CHANNEL_LIST) {
            wfd_tlv = (tlvbuf_wfd_channel_list *) ptr;
            temp_ptr = (chan_entry *) wfd_tlv->wfd_chan_entry_list;

            len_wifidirect = wlan_le16_to_cpu(wfd_tlv->length) -
                (sizeof(tlvbuf_wfd_channel_list) - WFD_IE_HEADER_LEN);
            /* Multiple channel entries */
            if (len_wifidirect >
                (sizeof(chan_entry) + temp_ptr->num_of_channels)) {
                while (len_wifidirect) {
                    if ((int) (temp_ptr->regulatory_class) == WIFI_REG_CLASS_81) {
                        pwfd_data->self_channel_list.num_of_chan =
                            temp_ptr->num_of_channels;
                        for (i = 0; i < temp_ptr->num_of_channels; i++) {
                            pwfd_data->self_channel_list.chan[i] =
                                *(temp_ptr->chan_list + i);
                        }
                        break;
                    }
                    len_wifidirect -=
                        sizeof(chan_entry) + temp_ptr->num_of_channels;
                    temp_ptr += sizeof(chan_entry) + temp_ptr->num_of_channels;
                }
            } else {
                /* Only one channel entry */
                pwfd_data->self_channel_list.num_of_chan =
                    temp_ptr->num_of_channels;
                for (i = 0; i < temp_ptr->num_of_channels; i++) {
                    pwfd_data->self_channel_list.chan[i] =
                        *(temp_ptr->chan_list + i);
                }
            }

            LEAVE();
            return WPS_STATUS_SUCCESS;
        }

        memcpy(&len, ptr + 1, sizeof(u16));
        len = wlan_le16_to_cpu(len);
        ptr += len + WFD_IE_HEADER_LEN;
    }

    LEAVE();
    return WPS_STATUS_FAIL;
}

/** 
 *  @brief  Check if P2P capability attr is present in the P2P IE.
 *          If yes, return the value of it.
 *  @param  wfd_buf      WFD buffer pointer
 *  @param  wfd_buf_len  WFD buffer length 
 *  @param  dev_cap pointer to device capability
 *  @param  grp_cap pointer to group capability
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
static int
wfd_get_p2p_cap_attribute(u8 * wfd_buf, u16 wfd_buf_len,
                          u8 * dev_cap, u8 * grp_cap)
{
    tlvbuf_wfd_capability *wfd_tlv;
    u8 *ptr = wfd_buf;
    s16 left_len = wfd_buf_len;
    u16 len = 0;

    ENTER();

    wps_hexdump(DEBUG_WLAN, "WFD IE:", (u8 *) wfd_buf, wfd_buf_len);
    while (left_len > WFD_IE_HEADER_LEN) {

        if (*ptr == TLV_TYPE_WFD_CAPABILITY) {
            wfd_tlv = (tlvbuf_wfd_capability *) ptr;
            *dev_cap = wfd_tlv->dev_capability;
            *grp_cap = wfd_tlv->group_capability;

            LEAVE();
            return WPS_STATUS_SUCCESS;
        }

        memcpy(&len, ptr + 1, sizeof(u16));
        len = wlan_le16_to_cpu(len);

        ptr += len + WFD_IE_HEADER_LEN;
        left_len -= len + WFD_IE_HEADER_LEN;
    }

    LEAVE();
    return WPS_STATUS_FAIL;
}

/** 
 *  @brief  Check if device password is received from FW.
 *          If yes, update the corresponding device ID.
 *  @param  wps_buf      WPS buffer pointer
 *  @param  wps_buf_len  WPS buffer length 
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
static int
wfd_wps_check_device_password(u8 * wps_buf, u16 wps_buf_len)
{
    u8 *ptr = wps_buf;
    s16 left_len = wps_buf_len;
    u16 wps_len = 0, wps_type = 0, WPS_devicepassword = 0;

    ENTER();

    /* Look for WPS Device Password */
    while (left_len > sizeof(tlvbuf_wps_ie)) {
        memcpy(&wps_type, ptr, sizeof(u16));
        wps_type = ntohs(wps_type);
        memcpy(&wps_len, ptr + 2, sizeof(u16));
        wps_len = ntohs(wps_len);

        if (wps_type == SC_Device_Password_ID) {
            tlvbuf_wps_ie *wps_tlv = (tlvbuf_wps_ie *) ptr;
            WPS_devicepassword = htons(gpwps_info->enrollee.device_password_id);

            /* htons done, now do memcpy */
            memcpy(wps_tlv->data, &WPS_devicepassword, sizeof(u16));
            wps_printf(DEBUG_WFD_DISCOVERY,
                       "Updated Device Password directly in FW response.\n");
            LEAVE();
            return WPS_STATUS_SUCCESS;
        }
        ptr += wps_len + sizeof(tlvbuf_wps_ie);
        left_len -= wps_len + sizeof(tlvbuf_wps_ie);
    }

    LEAVE();
    return WPS_STATUS_FAIL;
}

/** 
 *  @brief  Update the password IE in the device configuration.
 *          First Get the config and update with password. 
 *  @param  None
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_update_password_ie_config(void)
{
    int i, ret = WPS_STATUS_SUCCESS;
    short ie_index = 0;
    u8 *buf;
    u16 WPS_devicepassword, tlv_len;
    u16 ie_len_wfd, ie_len_wps;
    tlvbuf_wps_ie *wps_ptr;
    tlvbuf_custom_ie *tlv = NULL;
    custom_ie *ie_ptr = NULL;

    ENTER();

    buf = (u8*) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (buf) {
        memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
        ret = wfd_ie_config((u16 *) & ie_index, 0, 0, buf);

        if (ret == WPS_STATUS_SUCCESS && ie_index < (MAX_MGMT_IE_INDEX - 1)) {
            tlv = (tlvbuf_custom_ie *) buf;
            if (tlv->tag == MRVL_MGMT_IE_LIST_TLV_ID) {
                ie_ptr = (custom_ie *) (tlv->ie_data);
                /* Goto appropriate Ie Index */
                for (i = 0; i < ie_index; i++) {
                    ie_ptr = (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
                                            ie_ptr->ie_length);
                }
                ie_len_wfd = ie_ptr->ie_length;
                /* 
                 * Find out if the interface address is configured, it needs to be
                 * set using SIOCSIFHWADDR
                 */
                wfd_check_set_intended_addr((u8 *) (ie_ptr->ie_buffer), ie_len_wfd);

	            /* 
	             * Store the self channel list to compare with peer.
	             */
	            wfd_check_self_channel_list((u8 *) (ie_ptr->ie_buffer), ie_len_wfd);

	            /* 
	             * Store the self dev, grp capabilities.
	             */
	            wfd_get_p2p_cap_attribute((u8 *) (ie_ptr->ie_buffer), ie_len_wfd,
	                                      &gpwps_info->self_p2p_dev_cap,
	                                      &gpwps_info->self_p2p_grp_cap);

	            ie_ptr =
	                (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) + ie_len_wfd);
	            if (!
	                (ie_ptr->mgmt_subtype_mask == MASK_WFD_AUTO &&
	                 ie_ptr->ie_length)) {
	                wps_printf(MSG_WARNING,
	                           "Failed to get WPS IE to update device password.\n");
	                return WPS_STATUS_FAIL;
	            }

                ie_len_wps = ie_ptr->ie_length;
                wps_ptr =
                    (tlvbuf_wps_ie *) ((u8 *) (ie_ptr->ie_buffer) + ie_len_wps);

                /* Search for Device passwork in existing WPS attributes, if not
                   found append password IE here, update ie_len_wps */
                ret =
                    wfd_wps_check_device_password((u8 *) (ie_ptr->ie_buffer),
                                                  ie_len_wps);

                if (ret != WPS_STATUS_SUCCESS) {
                    WPS_devicepassword =
                        htons(gpwps_info->enrollee.device_password_id);
                    tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_devicepassword);
                    wps_ptr->tag = htons(SC_Device_Password_ID);
                    wps_ptr->length = htons(sizeof(WPS_devicepassword));

                    /* htons done, now do memcpy */
                    memcpy(wps_ptr->data, &WPS_devicepassword, sizeof(u16));
                    ie_len_wps += tlv_len;
                }

                /* Update New IE now */
                ret =
                    wfd_ie_config((u16 *) & ie_index, ie_len_wfd, ie_len_wps, buf);
                if (ret != WPS_STATUS_SUCCESS) {
                    wps_printf(MSG_WARNING,
                               "Failed to update WFD device password.\n");
                }
            }
        }
        MEM_FREE_AND_NULL(buf);
    } else {
        wps_printf(MSG_ERROR, "ERR:Cannot allocate memory!\n");
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return ret;
}
#if 1
/** 
 *  @brief Performs the ioctl operation to send the command to
 *  the driver.
 *
 *  @param cmd           Pointer to the command buffer
 *  @param size          Pointer to the command size. This value is overwritten 
 *                      by the function with the size of the received response.
 *  @param buf_size      Size of the allocated command buffer
 *  @return              WPS_STATUS_SUCCESS or WPS_STATUS_FAIL
 */
int
wfd_ioctl(u8 * cmd, u16 * size, u16 buf_size)
{
    int ret = WPS_STATUS_SUCCESS;
    wfdcmdbuf *header = NULL;

    ENTER();
    if (buf_size < *size) {
        wps_printf(MSG_WARNING,
                   "buf_size should not less than cmd buffer size\n");
        return WPS_STATUS_FAIL;
    }

    *(u32 *) cmd = buf_size - BUF_HEADER_SIZE;

    header = (wfdcmdbuf *) cmd;
    header->cmd_head.size = *size - BUF_HEADER_SIZE;
    endian_convert_request_header(header->cmd_head);

    ret = wfd_wlan_hostcmd(cmd);

    if (ret == WPS_STATUS_SUCCESS) {
        endian_convert_response_header(header->cmd_head);
        header->cmd_head.cmd_code &= HostCmd_CMD_ID_MASK;
        header->cmd_head.cmd_code |= WFDCMD_RESP_CHECK;
        *size = header->cmd_head.size;

        /* Validate response size */
        if (*size > (buf_size - BUF_HEADER_SIZE)) {
            wps_printf(MSG_WARNING,
                       "ERR:Response size (%d) greater than buffer size (%d)! Aborting!\n",
                       *size, buf_size);
            return WPS_STATUS_FAIL;
        }
    }
    LEAVE();
    return ret;
}

/** 
 *  @brief Performs the ioctl operation to send the command to
 *  the driver for various modes.
 *
 *  @param  mode Mode value to set
 *  @return WPS_STATUS_SUCCESS or WPS_STATUS_FAIL
 */
int
wfd_wlan_update_mode(u16 mode)
{
    int ret = WPS_STATUS_SUCCESS;
    u16 cmd_len = 0;
    u8 *buffer = NULL;
    wfd_mode_config *cmd_buf = NULL;
    u16 data = mode;

    wps_printf(MSG_INFO, "wfd_wlan_update_mode: mode %d\n", mode);

    ENTER();

    if (data > WFD_MODE_QUERY)
        data = WFD_MODE_QUERY;

    /* send hostcmd now */
    cmd_len = sizeof(wfd_mode_config);
    buffer = (u8 *) malloc(cmd_len);
    if (!buffer) {
        wps_printf(MSG_ERROR, "ERR:Cannot allocate memory!\n");
        return WPS_STATUS_FAIL;
    }

    cmd_buf = (wfd_mode_config *) buffer;
    cmd_buf->cmd_head.cmd_code = HostCmd_CMD_WFD_MODE_CONFIG;
    cmd_buf->cmd_head.size = cmd_len - BUF_HEADER_SIZE;
    cmd_buf->cmd_head.seq_num = 0;
    cmd_buf->cmd_head.result = 0;

    if (data == WFD_MODE_QUERY) {
        cmd_buf->action = ACTION_GET;
    } else {
        cmd_buf->action = ACTION_SET;
        cmd_buf->mode = wlan_cpu_to_le16(data);
    }
    cmd_buf->action = wlan_cpu_to_le16(cmd_buf->action);
    ret = wfd_ioctl((u8 *) buffer, &cmd_len, cmd_len);
    data = wlan_le16_to_cpu(cmd_buf->mode);

    /* 
     * For a GET command, if result is stopped, then issue start command.
     */
    if (ret == WPS_STATUS_SUCCESS && data == WFD_MODE_STOP) {
        wps_printf(DEBUG_WLAN, "WFD is not running, starting now.\n");
        ret = wfd_wlan_update_mode(WFD_MODE_START);
        if (ret != WPS_STATUS_SUCCESS)
            wps_printf(MSG_ERROR, "Failed to start WFD after successful Get.\n");
        free(buffer);
        LEAVE();
        return ret;
    }

    /* 
     * For a GO_QUERY command, if result is started, then issue GO start command.
     */
    if (ret == WPS_STATUS_SUCCESS && mode == WFD_GO_QUERY &&
        data == WFD_MODE_START) {
        wps_printf(DEBUG_WLAN, "WFD is running, starting GO now.\n");
        ret = wfd_wlan_update_mode(WFD_MODE_START_GROUP_OWNER);
        if (ret != WPS_STATUS_SUCCESS)
            wps_printf(MSG_ERROR, "Failed to start GO after successful start.\n");
        LEAVE();
        free(buffer);
        return ret;
    }

    if (buffer)
        free(buffer);
    LEAVE();
    wps_printf(DEBUG_WLAN, "WFD action %d, result %d.\n", mode, ret);
    return ret;
}

/** 
 *  @brief Creates a wfd_action_frame request and sends to the driver
 *
 *  Usage: "Usage : wfd_action_frame "
 *
 *  @param sel_index Index selected by user
 *  @param frame_sub_type  Frame Sub type for generic action frame
 *  @return          WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfdcmd_action_frame(int sel_index, int frame_sub_type)
{
    int ret = WPS_STATUS_SUCCESS;
    wfd_action_frame *action_buf = NULL;
    tlvbuf_mrvl_wfd_channel *ch_tlv;
    WFD_DATA *pwfd_data = &wps_global.wfd_data;
    u8 *buffer;
    u16 cmd_len = 0, ie_len = 0;

    ENTER();
    
	buffer = (u8*) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
	
    if (buffer) {
	    cmd_len = sizeof(wfd_action_frame);
    
        memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
	    action_buf = (wfd_action_frame *) buffer;
        /* update the command header parameters */
        action_buf->cmd_head.cmd_code = HostCmd_CMD_802_11_ACTION_FRAME;
        action_buf->cmd_head.seq_num = 0;
        action_buf->cmd_head.result = 0;
    
        /* update the command data parameters */
	    action_buf->category = WIFI_CATEGORY_PUBLIC_ACTION_FRAME;

	    if (memcmp(pwfd_data->find_results[sel_index].device_address,
               "\x00\x00\x00\x00\x00\x00", ETH_ALEN))
	        /* 
	         * For action frame processing device_address should be used 
	         */
	        memcpy(action_buf->peer_mac_addr,
	               pwfd_data->find_results[sel_index].device_address, ETH_ALEN);
	    else
	        memcpy(action_buf->peer_mac_addr,
	               pwfd_data->find_results[sel_index].device_id, ETH_ALEN);
    
        action_buf->action = 0;
    
	    action_buf->dialog_taken = WIFI_DIALOG_TOKEN_IGNORE + 1;
    
	    action_buf->oui[0] = 0x50;
	    action_buf->oui[1] = 0x6F;
	    action_buf->oui[2] = 0x9A;

	    action_buf->oui_type = WFD_OUI_TYPE;

	    action_buf->oui_sub_type = frame_sub_type;
	    /* set interval to 200 ms and count to 3, as per spec */
	    // action_buf->interval = wlan_cpu_to_le16(WIFI_ACTION_FRAME_INTERVAL);
	    // action_buf->count = WIFI_ACTION_FRAME_COUNT;

	    if (pwfd_data->find_results[sel_index].op_channel) {
	        ch_tlv = (tlvbuf_mrvl_wfd_channel *) & (action_buf->ie_list[ie_len]);
	        ch_tlv->tag = wlan_cpu_to_le16(MRVL_WFD_OPERATING_CHANNEL_TLV_ID);
	        ch_tlv->length =
	            wlan_cpu_to_le16(sizeof(tlvbuf_mrvl_wfd_channel) - 2 * sizeof(u16));
	        ch_tlv->channel_num = pwfd_data->find_results[sel_index].op_channel;
	        ie_len += sizeof(tlvbuf_mrvl_wfd_channel);
	        cmd_len += sizeof(tlvbuf_mrvl_wfd_channel);
	    }

	    if (pwfd_data->find_results[sel_index].listen_channel) {
	        ch_tlv = (tlvbuf_mrvl_wfd_channel *) & (action_buf->ie_list[ie_len]);
	        ch_tlv->tag = wlan_cpu_to_le16(MRVL_WFD_LISTEN_CHANNEL_TLV_ID);
	        ch_tlv->length =
	            wlan_cpu_to_le16(sizeof(tlvbuf_mrvl_wfd_channel) - 2 * sizeof(u16));
	        ch_tlv->channel_num = pwfd_data->find_results[sel_index].listen_channel;
	        ie_len += sizeof(tlvbuf_mrvl_wfd_channel);
	        cmd_len += sizeof(tlvbuf_mrvl_wfd_channel);
	    }

        action_buf->cmd_head.size = cmd_len;
    
        /* Send collective command */
	    if (wfd_ioctl((u8 *) buffer, &cmd_len, cmd_len) == WPS_STATUS_SUCCESS)
            wps_printf(DEBUG_WFD_DISCOVERY,
                       "WFD action frame successfully sent.\n");
        else {
            wps_printf(MSG_WARNING, "WFD action frame send failed.\n");
            ret = WPS_STATUS_FAIL;
        }
        MEM_FREE_AND_NULL(buffer);
    } else {
        wps_printf(MSG_WARNING, "wfdcmd_action_frame: out of memory\n");
        ret = WPS_STATUS_FAIL;
    }

    LEAVE();
    return ret;
}

/** 
 *  @brief Read the wfd parameters and sends to the driver
 *
 *  @param file_name File to open for configuration parameters. 
 *  @param pbuf      Pointer to output buffer
 *  @param ie_len_wfd Length of p2p parameters to return 
 *  @param ie_len_wps Length of WPS parameters to return 
 *  @return          SUCCESS or FAILURE
 */
static void
wfd_file_params_config(char *file_name, u8 * pbuf,
                       u16 * ie_len_wfd, u16 * ie_len_wps)
{
    u8 *buffer = pbuf;
    u16 tlv_len = 0;
    u16 cmd_len_wfd = 0;
    u16 cmd_len_wps = 0;
    u16 temp;
    u8 extra_len = 0;
    u8 country[] = "US";

    // WFD_DEVICE_ID_CONFIG:
    {
        tlvbuf_wfd_device_id *tlv = NULL;
        u8 dev_mac_address[ETH_ALEN];

        // Device ID
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)dev_mac_address);

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_device_id);
        tlv = (tlvbuf_wfd_device_id *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_DEVICE_ID;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->dev_mac_address, dev_mac_address, ETH_ALEN);
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_CAPABILITY_CONFIG:
    {
        tlvbuf_wfd_capability *tlv = NULL;
        u8 dev_capability = 1;
        u8 group_capability = 0;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_capability);
        tlv = (tlvbuf_wfd_capability *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_CAPABILITY;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        tlv->dev_capability = dev_capability;
        tlv->group_capability = group_capability;
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_GROUP_OWNER_INTENT_CONFIG:
    {
        tlvbuf_wfd_group_owner_intent *tlv = NULL;
        u8 group_owner_intent = 15;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_group_owner_intent);
        tlv =
            (tlvbuf_wfd_group_owner_intent *) (buffer +
                                               cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_GROUPOWNER_INTENT;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        tlv->dev_intent = group_owner_intent;
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_MANAGEABILITY_CONFIG:
    {
        tlvbuf_wfd_manageability *tlv = NULL;
        u8 manageability = 0;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_manageability);
        tlv = (tlvbuf_wfd_manageability *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_MANAGEABILITY;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        tlv->manageability = manageability;
        endian_convert_tlv_wfd_header_out(tlv);
    }
    //WFD_INVITATION_FLAG_CONFIG:
    {
        tlvbuf_wfd_invitation_flag *tlv = NULL;
		u8 invitation_flag = 0;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_invitation_flag);
        tlv = (tlvbuf_wfd_invitation_flag *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_INVITATION_FLAG;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        tlv->invitation_flag |= invitation_flag;
        endian_convert_tlv_wfd_header_out(tlv);
    }
    // WFD_CHANNEL_LIST_CONFIG:
    {
        tlvbuf_wfd_channel_list *tlv = NULL;
        u8 no_of_chan_entries = 1;
        u16 total_chan_len = 3;
        u8 chan_entry_list[] = {12,3,1,6,11};

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_channel_list) +
            no_of_chan_entries * sizeof(chan_entry) +
            total_chan_len;
        tlv = (tlvbuf_wfd_channel_list *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_CHANNEL_LIST;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->country_string, country, sizeof(country));
                    if (tlv->country_string[2] == 0)
                        tlv->country_string[2] = WFD_COUNTRY_LAST_BYTE;
        memcpy(tlv->wfd_chan_entry_list, chan_entry_list,
               (tlv->length - 3));
        endian_convert_tlv_wfd_header_out(tlv);
    }
    
    // WFD_NOTICE_OF_ABSENCE:
    {
        tlvbuf_wfd_notice_of_absence *tlv = NULL;
        u8 noa_index = 0;
        u8 opp_ps = 1;
        u8 ctwindow_opp_ps = (u8) 10 | SET_OPP_PS(opp_ps);
        u8 no_of_noa = 2;
        noa_descriptor noa_descriptor_list[MAX_NOA_DESCRIPTORS];

        noa_descriptor_list[0].count_type = 1;
        noa_descriptor_list[0].duration = 0;
        noa_descriptor_list[0].interval = 0;
        noa_descriptor_list[0].start_time = 0;
        noa_descriptor_list[1].count_type = 1;
        noa_descriptor_list[1].duration = 0;
        noa_descriptor_list[1].interval = 0;
        noa_descriptor_list[1].start_time = 0;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_notice_of_absence) +
            no_of_noa * sizeof(noa_descriptor);
        tlv =
            (tlvbuf_wfd_notice_of_absence *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_NOTICE_OF_ABSENCE;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        tlv->noa_index = noa_index;
        tlv->ctwindow_opp_ps = ctwindow_opp_ps;
        /* If Opp_PS is set, NoA Descriptors are not set */
        if (!opp_ps) {
            memcpy(tlv->wfd_noa_descriptor_list,
                   noa_descriptor_list,
                   no_of_noa * sizeof(noa_descriptor));
        }
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_CHANNEL_CONFIG:
    {
        tlvbuf_wfd_channel *tlv = NULL;
        u8 regulatory_class = 12;
        u8 channel_number = 6;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_channel);
        tlv = (tlvbuf_wfd_channel *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_CHANNEL;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->country_string, country, 3);
        if (tlv->country_string[2] == 0)
            tlv->country_string[2] = WFD_COUNTRY_LAST_BYTE;
        tlv->regulatory_class = regulatory_class;
        tlv->channel_number = channel_number;
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_OPCHANNEL_CONFIG:
    {
        tlvbuf_wfd_channel *tlv = NULL;
        u8 op_regulatory_class = 12;
        u8 op_channel_number = 6;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_channel);
        tlv = (tlvbuf_wfd_channel *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_OPCHANNEL;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->country_string, country, 3);
        if (tlv->country_string[2] == 0)
            tlv->country_string[2] = WFD_COUNTRY_LAST_BYTE;
        tlv->regulatory_class = op_regulatory_class;
        tlv->channel_number = op_channel_number;
        endian_convert_tlv_wfd_header_out(tlv);
    }
    // WFD_DEVICE_INFO_CONFIG:
    {
        tlvbuf_wfd_device_info *tlv = NULL;
        u8 wfd_ssid[] = "\"WIFI_DIRECT_WPS_AP\"";
        //u16 pri_category = 6;
        // u16 pri_sub_category = 1;
        u16 pri_category = 3; // 3: Category - Printers, Scanners, Faxes and Copiers
        //u16 pri_sub_category = 1; // 5: Sub-category - Printer or Print Server
        u16 pri_sub_category = 5; // 5: Sub-category - All-in-one (Printer, Scanner, Fax, Copier)
        u8 primary_oui[] = {0x00,0x50,0xF2,0x04};
        u8 secondary_dev_count = 2;
        u8 secondary_dev_info[] = {0x00,0x03,0x00,0x50,0xF2,0x4,0x00,0x05,0x00,0x03,0x00,0x50,0xF2,0x4,0x00,0x01};
        u16 config_methods = 0x0086;
        u8 dev_address[ETH_ALEN];

        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)dev_address);

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_device_info) +
            secondary_dev_count * WPS_DEVICE_TYPE_LEN +
            strlen((char *)wfd_ssid);
        tlv = (tlvbuf_wfd_device_info *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_DEVICE_INFO;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->dev_address, dev_address, ETH_ALEN);
        tlv->config_methods = htons(config_methods);
        tlv->primary_category = htons(pri_category);
        memcpy(tlv->primary_oui, primary_oui, sizeof(primary_oui));
        tlv->primary_subcategory = htons(pri_sub_category);
        tlv->secondary_dev_count = secondary_dev_count;
        endian_convert_tlv_wfd_header_out(tlv);
        /* Parameters within secondary_dev_info are already
           htons'ed */
        memcpy(tlv->secondary_dev_info, secondary_dev_info,
               secondary_dev_count * WPS_DEVICE_TYPE_LEN);
        temp = htons(SC_Device_Name);
        memcpy(((u8 *) (&tlv->device_name_type)) +
               secondary_dev_count * WPS_DEVICE_TYPE_LEN, &temp,
               sizeof(temp));
        temp = htons(strlen((char *)wfd_ssid));
        memcpy(((u8 *) (&tlv->device_name_len)) +
               secondary_dev_count * WPS_DEVICE_TYPE_LEN, &temp,
               sizeof(temp));
        memcpy(((u8 *) (&tlv->device_name)) +
               secondary_dev_count * WPS_DEVICE_TYPE_LEN,
               wfd_ssid, strlen((char *)wfd_ssid));
    }

    // WFD_GROUP_ID_CONFIG:
    {
        tlvbuf_wfd_group_id *tlv = NULL;
        u8 wifi_direct_group_id_ssid[] = "\"WIFI_DIRECT_SSID\"";
        u8 group_address[ETH_ALEN];

        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)group_address);

        /* Append a new TLV */
        tlv_len =
            sizeof(tlvbuf_wfd_group_id) +
            strlen((char *)wifi_direct_group_id_ssid);
        tlv = (tlvbuf_wfd_group_id *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_GROUP_ID;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->group_address, group_address, ETH_ALEN);
        memcpy(tlv->group_ssid, wifi_direct_group_id_ssid,
               strlen((char *)wifi_direct_group_id_ssid));
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_GROUP_BSS_ID_CONFIG:
    {
        tlvbuf_wfd_group_bss_id *tlv = NULL;
        u8 group_bssid[ETH_ALEN];

        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)group_bssid);

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_group_bss_id);
        tlv = (tlvbuf_wfd_group_bss_id *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_GROUP_BSS_ID;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->group_bssid, group_bssid, ETH_ALEN);
        endian_convert_tlv_wfd_header_out(tlv);
    }


    // WFD_INTERFACE_CONFIG:
    {
        tlvbuf_wfd_interface *tlv = NULL;
        u8 iface_count = 2;
        u8 interface_id_addr[ETH_ALEN];
        u8 iface_list[MAX_INTERFACE_ADDR_COUNT][ETH_ALEN];

        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)interface_id_addr);
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)(&iface_list[0][0]));
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)(&iface_list[1][0]));

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_interface)
            + iface_count * ETH_ALEN;
        tlv = (tlvbuf_wfd_interface *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_INTERFACE;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->interface_id, interface_id_addr, ETH_ALEN);
        tlv->interface_count = iface_count;
        memcpy(tlv->interface_idlist, iface_list,
               iface_count * ETH_ALEN);
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_TIMEOUT_CONFIG:
    {
        tlvbuf_wfd_config_timeout *tlv = NULL;
        u8 go_config_timeout = 100;
        u8 client_config_timeout = 150;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_config_timeout);
        tlv = (tlvbuf_wfd_config_timeout *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_CONFIG_TIMEOUT;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        tlv->group_config_timeout = go_config_timeout;
        tlv->device_config_timeout = client_config_timeout;
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_EXTENDED_TIME_CONFIG:
    {
        tlvbuf_wfd_ext_listen_time *tlv = NULL;
        u16 avail_period = 1000;
        u16 avail_interval = 1500;

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_ext_listen_time);
        tlv = (tlvbuf_wfd_ext_listen_time *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_EXTENDED_LISTEN_TIME;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        tlv->availability_period = wlan_le16_to_cpu(avail_period);
        tlv->availability_interval =
            wlan_le16_to_cpu(avail_interval);
        endian_convert_tlv_wfd_header_out(tlv);
    }

    // WFD_INTENDED_ADDR_CONFIG:
    {
        tlvbuf_wfd_intended_addr *tlv = NULL;
        u8 intended_address[ETH_ALEN];

        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)intended_address);

        /* Append a new TLV */
        tlv_len = sizeof(tlvbuf_wfd_intended_addr);
        tlv = (tlvbuf_wfd_intended_addr *) (buffer + cmd_len_wfd);
        cmd_len_wfd += tlv_len;
        /* Set TLV fields */
        tlv->tag = TLV_TYPE_WFD_INTENDED_ADDRESS;
        tlv->length = tlv_len - (sizeof(u8) + sizeof(u16));
        memcpy(tlv->group_address, intended_address, ETH_ALEN);
        endian_convert_tlv_wfd_header_out(tlv);
    }


    // WFD_EXTRA:
    extra_len = 0;

    if (extra_len > 0) 
    {
        u8 extra[] = "TBD";
        memcpy(buffer + cmd_len_wfd, extra, extra_len);
        cmd_len_wfd += extra_len;
    }

    // WFD_WPSIE:
    {
        /* Append TLV for WPSVersion */
        tlvbuf_wps_ie *tlv = NULL;
        u8 WPS_version = 0x10;
        u8 WPS_setupstate = 0x1;
        u8 WPS_requesttype = 0x0;
        u8 WPS_responsetype = 0x0;
        u16 WPS_specconfigmethods = 0x0086;
        u8 WPS_UUID[] = {0x12,0x34,0x56,0x78,0x12,0x34,0x56,0x78,0x12,0x34,0x56,0x78,0x12,0x34,0x56,0x78};
        u8 WPS_primarydevicetype[] = {0x01,0x00,0x50,0xF2,0x04,0x01,0x3C,0x10};
        u8 WPS_RFband = 0x01;
        u16 WPS_associationstate = 0x00;
        u16 WPS_configurationerror = 0x00;
        u16 WPS_devicepassword = 0x00;
        u8 WPS_devicename[] = "\"WIFI_DIRECT_WPS_AP\"";
        u8 WPS_manufacturer[] = "\"Marvell\"";
        u8 WPS_modelname[] = "\"88W8782\"";
        u8 WPS_modelnumber[] = {0x01,0x02,0x03,0x04};
        u8 WPS_serialnumber[] = {0x01,0x02,0x03,0x11};

        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_version);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Version;
        tlv->length = tlv_len - 2 * sizeof(u16);
        *(tlv->data) = WPS_version;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSSetupState */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_setupstate);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Simple_Config_State;
        tlv->length = tlv_len - 2 * sizeof(u16);
        *(tlv->data) = WPS_setupstate;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSRequestType */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_requesttype);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Request_Type;
        tlv->length = tlv_len - 2 * sizeof(u16);
        *(tlv->data) = WPS_requesttype;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSResponseType */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_responsetype);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Response_Type;
        tlv->length = tlv_len - 2 * sizeof(u16);
        *(tlv->data) = WPS_responsetype;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSSpecConfigMethods */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(WPS_specconfigmethods);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Config_Methods;
        tlv->length = tlv_len - 2 * sizeof(u16);
        temp = htons(WPS_specconfigmethods);
        memcpy((u16 *) tlv->data, &temp, sizeof(temp));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSUUID */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_UUID);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_UUID_E;
        tlv->length = tlv_len - 2 * sizeof(u16);
        memcpy(tlv->data, WPS_UUID, sizeof(WPS_UUID));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSPrimaryDeviceType */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(WPS_primarydevicetype);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Primary_Device_Type;
        tlv->length = tlv_len - 2 * sizeof(u16);
        memcpy(tlv->data, WPS_primarydevicetype,
               sizeof(WPS_primarydevicetype));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSRFBand */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_RFband);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_RF_Band;
        tlv->length = tlv_len - 2 * sizeof(u16);
        *(tlv->data) = WPS_RFband;
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSAssociationState */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(WPS_associationstate);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Association_State;
        tlv->length = tlv_len - 2 * sizeof(u16);
        temp = htons(WPS_associationstate);
        memcpy((u16 *) tlv->data, &temp, sizeof(temp));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSConfigurationError */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(WPS_configurationerror);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Configuration_Error;
        tlv->length = tlv_len - 2 * sizeof(u16);
        temp = htons(WPS_configurationerror);
        memcpy((u16 *) tlv->data, &temp, sizeof(temp));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSDevicePassword */
        tlv_len =
            sizeof(tlvbuf_wps_ie) + sizeof(WPS_devicepassword);
        tlv =
            (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                               sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Device_Password_ID;
        tlv->length = tlv_len - 2 * sizeof(u16);
        temp = htons(WPS_devicepassword);
        memcpy((u16 *) tlv->data, &temp, sizeof(temp));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSDeviceName */
        tlv_len = sizeof(tlvbuf_wps_ie) + strlen((char *)WPS_devicename);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Device_Name;
        tlv->length = tlv_len - 2 * sizeof(u16);
        memcpy(tlv->data, WPS_devicename, strlen((char *)WPS_devicename));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSManufacturer */
        tlv_len = sizeof(tlvbuf_wps_ie) + strlen((char *)WPS_manufacturer);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Manufacturer;
        tlv->length = tlv_len - 2 * sizeof(u16);
        memcpy(tlv->data, WPS_manufacturer,
               strlen((char *)WPS_manufacturer));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSModelName */
        tlv_len = sizeof(tlvbuf_wps_ie) + strlen((char *)WPS_modelname);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Model_Name;
        tlv->length = tlv_len - 2 * sizeof(u16);
        memcpy(tlv->data, WPS_modelname, strlen((char *)WPS_modelname));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSModelNumber */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_modelnumber);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Model_Number;
        tlv->length = tlv_len - 2 * sizeof(u16);
        memcpy(tlv->data, WPS_modelnumber, sizeof(WPS_modelnumber));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;

        /* Append TLV for WPSSerialNumber */
        tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(WPS_serialnumber);
        tlv = (tlvbuf_wps_ie *) (buffer + cmd_len_wfd +
                                 sizeof(custom_ie) + cmd_len_wps);
        tlv->tag = SC_Serial_Number;
        tlv->length = tlv_len - 2 * sizeof(u16);
        memcpy(tlv->data, WPS_serialnumber, sizeof(WPS_serialnumber));
        endian_convert_tlv_wps_header_out(tlv);
        cmd_len_wps += tlv_len;
    }

    *ie_len_wfd = cmd_len_wfd;
    if (ie_len_wps)
        *ie_len_wps = cmd_len_wps;

    return;
}

/** 
 *  @brief Creates a wfd_config request and sends to the driver
 *
 *  Usage: "Usage : wfd_config <CONFIG_FILE>"
 *
 *  @param filename Config file Name
 *  @return         WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfd_config_download(char *filename)
{
    int ret = WPS_STATUS_FAIL;
    u8 *buf = NULL;
    u16 ie_len_wfd = 0, ie_len_wps = 0, ie_len;
    u16 ie_index = 0;

    ENTER();

    buf = (u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (!buf) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Cannot allocate memory!\n"));
        return WPS_STATUS_FAIL;
    }
    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);

    /* Read parameters and send command to firmware */
    wfd_file_params_config(filename, buf
                           + sizeof(tlvbuf_custom_ie) + sizeof(custom_ie),
                           &ie_len_wfd, &ie_len_wps);
    if (ie_len_wfd > MAX_SIZE_IE_BUFFER || ie_len_wps > MAX_SIZE_IE_BUFFER) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:IE parameter size exceeds limit in %s.\n", filename));
        MEM_FREE_AND_NULL(buf);
        return WPS_STATUS_FAIL;
    }
    ie_len =
        ie_len_wfd + ie_len_wps + sizeof(tlvbuf_custom_ie) +
        (2 * sizeof(custom_ie));
    if (ie_len >= MRVDRV_SIZE_OF_CMD_BUFFER) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Too much data in configuration file %s.\n", filename));
        MEM_FREE_AND_NULL(buf);
        return WPS_STATUS_FAIL;
    }

    wps_hexdump(DEBUG_WLAN, "Config Download", buf, ie_len);

    ret = wfd_ie_config(&ie_index, ie_len_wfd, ie_len_wps, buf);
    if (ret != WPS_STATUS_SUCCESS) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Could not set wfd parameters\n"));
    }

    wps_printf(DEBUG_WLAN, "Result of config download %d.\n", ret);

    MEM_FREE_AND_NULL(buf);
    LEAVE();
    return ret;
}

/** 
 *  @brief Creates a wfd_params_config request and sends to the driver
 *
 *  Usage: "Usage : wfd_params_config <CONFIG_FILE>"
 *
 *  @param filename Config file Name
 *  @return         WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfd_params_config_download(char *filename)
{
#if 1
    int ret = WPS_STATUS_FAIL;
    wfd_params_config *param_buf = NULL;
    tlvbuf_wps_ie *new_tlv = NULL;
    //char *line = NULL, wifi_direct_mac[20];
    //FILE *config_file = NULL;
    //int i, li = 0, arg_num = 0;
    //char *args[30], *pos = NULL;
    //u8 dev_address[ETH_ALEN];
    u8 enable_scan;
    u8 *buffer = NULL;
    //u8 WPS_primarydevicetype[WPS_DEVICE_TYPE_MAX_LEN];
    u16 device_state = 0, tlv_len = 0, max_disc_int, min_disc_int, cmd_len = 0;

    ENTER();

    // MinDiscoveryInterval
    min_disc_int = wlan_cpu_to_le16(1);

    // MaxDiscoveryInterval
    max_disc_int = wlan_cpu_to_le16(3);

    // EnableScan
    enable_scan = 1;

    // DeviceState
    device_state = wlan_cpu_to_le16(4);


    cmd_len = sizeof(wfd_params_config);
    buffer = (u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (!buffer) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Cannot allocate memory!\n"));
        return WPS_STATUS_FAIL;
    }
    memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    param_buf = (wfd_params_config *) buffer;
    param_buf->cmd_head.cmd_code = HostCmd_CMD_WFD_PARAMS_CONFIG;
    param_buf->cmd_head.seq_num = 0;
    param_buf->cmd_head.result = 0;
    param_buf->action = wlan_cpu_to_le16(ACTION_SET);

    /* Append a new TLV */
    tlv_len = sizeof(tlvbuf_wps_ie) + 2 * sizeof(u16);
    new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
    cmd_len += tlv_len;
    /* Set TLV fields */
    new_tlv->tag = MRVL_WFD_DISC_PERIOD_TLV_ID;
    new_tlv->length = tlv_len - 2 * sizeof(u16);
    memcpy(&new_tlv->data, &min_disc_int, sizeof(u16));
    memcpy((((u8 *) & new_tlv->data) + sizeof(u16)),
           &max_disc_int, sizeof(u16));
    endian_convert_tlv_header_out(new_tlv);

    /* Append a new TLV */
    tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(u8);
    new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
    cmd_len += tlv_len;
    /* Set TLV fields */
    new_tlv->tag = MRVL_WFD_SCAN_ENABLE_TLV_ID;
    new_tlv->length = tlv_len - 2 * sizeof(u16);
    memcpy(new_tlv->data, &enable_scan, sizeof(u8));
    endian_convert_tlv_header_out(new_tlv);

    /* Append a new TLV */
    tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(u16);
    new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
    cmd_len += tlv_len;
    /* Set TLV fields */
    new_tlv->tag = MRVL_WFD_DEVICE_STATE_TLV_ID;
    new_tlv->length = tlv_len - 2 * sizeof(u16);
    memcpy(new_tlv->data, &device_state, sizeof(u16));
    endian_convert_tlv_header_out(new_tlv);

    param_buf->cmd_head.size = cmd_len;

    wps_hexdump(DEBUG_WLAN, "Parameters Download", buffer, cmd_len);

    /* Send collective command */
    ret = wfd_ioctl((u8 *) buffer, &cmd_len, cmd_len);
    wps_printf(DEBUG_WLAN, "Result of parameter download %d.\n", ret);

    if (buffer)
        MEM_FREE_AND_NULL(buffer);

    LEAVE();
    return ret;
#else
    int ret = WPS_STATUS_FAIL;
    wifi_direct_params_config *param_buf = NULL;
    tlvbuf_wps_ie *new_tlv = NULL;
    char *line = NULL, wifi_direct_mac[20];
    FILE *config_file = NULL;
    int i, li = 0, arg_num = 0;
    char *args[30], *pos = NULL;
    u8 dev_address[ETH_ALEN], enable_scan;
    u8 *buffer = NULL, WPS_primarydevicetype[WPS_DEVICE_TYPE_MAX_LEN];
    u16 device_state = 0, tlv_len = 0, max_disc_int, min_disc_int, cmd_len = 0;

    ENTER();
    cmd_len = sizeof(wfd_params_config);
    buffer = (u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
    if (!buffer) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Cannot allocate memory!\n"));
        return WPS_STATUS_FAIL;
    }
    memset(buffer, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    param_buf = (wfd_params_config *) buffer;
    param_buf->cmd_head.cmd_code = HostCmd_CMD_WIFI_DIRECT_PARAMS_CONFIG;
    param_buf->cmd_head.seq_num = 0;
    param_buf->cmd_head.result = 0;

    /* Check if file exists */
    config_file = fopen(filename, "r");
    if (config_file == NULL) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("\nERR:Config file can not be opened.\n"));
        goto done;
    }
    line = (char *) MEM_MALLOC(MAX_CONFIG_LINE);
    if (!line) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Cannot allocate memory for line\n"));
        goto done;
    }
    memset(line, 0, MAX_CONFIG_LINE);
    param_buf->action = wlan_cpu_to_le16(ACTION_SET);

    /* Parse file and process */
    while (config_get_line(line, MAX_CONFIG_LINE, config_file, &li, &pos)) {
        arg_num = parse_line(line, args);

        if (strcmp(args[0], "EnableScan") == 0) {
            if (is_input_valid(WFD_ENABLE_SCAN, arg_num - 1, args + 1)
                != SUCCESS) {
                goto done;
            }
            enable_scan = (u8) atoi(args[1]);
            /* Append a new TLV */
            tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(u8);
            new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
            cmd_len += tlv_len;
            /* Set TLV fields */
            new_tlv->tag = MRVL_WFD_SCAN_ENABLE_TLV_ID;
            new_tlv->length = tlv_len - 2 * sizeof(u16);
            memcpy(new_tlv->data, &enable_scan, sizeof(u8));
            endian_convert_tlv_header_out(new_tlv);

        } else if (strcmp(args[0], "ScanPeerDeviceId") == 0) {
            strncpy(wfd_mac, args[1], 20);
            if ((ret = mac2raw(wfd_mac, dev_address)) != SUCCESS) {
                DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR: %s Address \n",
                       ret == WPS_STATUS_FAIL ? "Invalid MAC" : ret ==
                       WFD_RET_MAC_BROADCAST ? "Broadcast" : "Multicast");
                goto done;
            }
            /* Append a new TLV */
            tlv_len = sizeof(tlvbuf_wps_ie) + ETH_ALEN;
            new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
            cmd_len += tlv_len;
            /* Set TLV fields */
            new_tlv->tag = MRVL_WFD_PEER_DEVICE_TLV_ID;
            new_tlv->length = tlv_len - 2 * sizeof(u16);
            memcpy(new_tlv->data, dev_address, ETH_ALEN);
            endian_convert_tlv_header_out(new_tlv);

        } else if (strcmp(args[0], "MinDiscoveryInterval") == 0) {
            if (is_input_valid(WFD_MINDISCOVERYINT, arg_num - 1, args + 1) !=
                SUCCESS) {
                goto done;
            }
            min_disc_int = wlan_cpu_to_le16(atoi(args[1]));
        } else if (strcmp(args[0], "MaxDiscoveryInterval") == 0) {
            if (is_input_valid(WFD_MAXDISCOVERYINT, arg_num - 1, args + 1) !=
                SUCCESS) {
                goto done;
            }
            max_disc_int = wlan_cpu_to_le16(atoi(args[1]));

            /* Append a new TLV */
            tlv_len = sizeof(tlvbuf_wps_ie) + 2 * sizeof(u16);
            new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
            cmd_len += tlv_len;
            /* Set TLV fields */
            new_tlv->tag = MRVL_WFD_DISC_PERIOD_TLV_ID;
            new_tlv->length = tlv_len - 2 * sizeof(u16);
            memcpy(&new_tlv->data, &min_disc_int, sizeof(u16));
            memcpy((((u8 *) & new_tlv->data) + sizeof(u16)),
                   &max_disc_int, sizeof(u16));
            endian_convert_tlv_header_out(new_tlv);

        } else if (strcmp(args[0], "ScanRequestDeviceType") == 0) {
            if (is_input_valid(WFD_WPSPRIMARYDEVICETYPE, arg_num - 1, args + 1)
                != SUCCESS) {
                goto done;
            }
            for (i = 0; i < WPS_DEVICE_TYPE_MAX_LEN; i++)
                WPS_primarydevicetype[i] = (u8) A2HEXDECIMAL(args[i + 1]);

            /* Append a new TLV */
            tlv_len = sizeof(tlvbuf_wps_ie) + WPS_DEVICE_TYPE_MAX_LEN;
            new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
            cmd_len += tlv_len;
            /* Set TLV fields */
            new_tlv->tag = MRVL_WFD_SCAN_REQ_DEVICE_TLV_ID;
            new_tlv->length = tlv_len - 2 * sizeof(u16);
            memcpy(&new_tlv->data, WPS_primarydevicetype,
                   WPS_DEVICE_TYPE_MAX_LEN);
            endian_convert_tlv_header_out(new_tlv);

        } else if (strcmp(args[0], "DeviceState") == 0) {
            if (is_input_valid(WFD_DEVICE_STATE, arg_num - 1, args + 1)
                != SUCCESS) {
                goto done;
            }
            device_state = wlan_cpu_to_le16((u16) atoi(args[1]));

            /* Append a new TLV */
            tlv_len = sizeof(tlvbuf_wps_ie) + sizeof(u16);
            new_tlv = (tlvbuf_wps_ie *) (buffer + cmd_len);
            cmd_len += tlv_len;
            /* Set TLV fields */
            new_tlv->tag = MRVL_WFD_DEVICE_STATE_TLV_ID;
            new_tlv->length = tlv_len - 2 * sizeof(u16);
            memcpy(new_tlv->data, &device_state, sizeof(u16));
            endian_convert_tlv_header_out(new_tlv);
        }
    }
    param_buf->cmd_head.size = cmd_len;

    wps_hexdump(DEBUG_WLAN, "Parameters Download", buffer, cmd_len);

    /* Send collective command */
    ret = wfd_ioctl((u8 *) buffer, &cmd_len, cmd_len);
    wps_printf(DEBUG_WLAN, "Result of parameter download %d.\n", ret);

  done:
    if (config_file)
        fclose(config_file);
    if (line)
        MEM_FREE_AND_NULL(line);
    if (buffer)
        MEM_FREE_AND_NULL(buffer);
    LEAVE();
    return ret;
#endif
}

/** 
 *  @brief Creates a wfd_config, wfd_params_config
 *          request and sends to the driver.
 *
 *         ./wfdutl mlan0 wfd_config wfd.conf
 *         ./wfdutl mlan0 wfd_params_config wfd.conf
 *
 *  @param filename Config file Name
 *  @return         WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfdcmd_config_download(char *filename)
{
    int if_switch = 0;

    wps_global.bss_type = BSS_TYPE_UAP;

    if (wps_global.bss_type == BSS_TYPE_UAP && switch_intf) {
        /* For config download, switch to "mlan0" interface */
        wps_wlan_deinit(&wps_global);
        strncpy(wps_global.ifname, "mlan0", IFNAMSIZ);
        wps_wlan_init(&wps_global);
        if_switch = 1;
    }

    if (wfd_config_download(filename) != WPS_STATUS_SUCCESS) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("WFD configuration download failed.\n"));
        return WPS_STATUS_FAIL;
    }
    if (wfd_params_config_download(filename) != WPS_STATUS_SUCCESS) {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("WFD parameter configuration failed.\n"));
        return WPS_STATUS_FAIL;
    }

    if (if_switch) {
        /* switch back to "uap0" interface */
        wps_wlan_deinit(&wps_global);
        strncpy(wps_global.ifname, "uap0", IFNAMSIZ);
        wps_wlan_init(&wps_global);
    }
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief Checks if WFD is started or not and starts it in later case.
 *         Start Find phase as well.
 *
 *         ./wfdutl mlan0 wfd_mode 1
 *         ./wfdutl mlan0 wfd_mode 4
 *
 *  @return         WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfdcmd_start_find_phase(void)
{
    /* Get WFD mode and start if needed */
    if (wfd_wlan_update_mode(WFD_MODE_QUERY) != WPS_STATUS_SUCCESS) {
        wps_printf(MSG_ERROR, "WFD start failed.\n");
        return WPS_STATUS_FAIL;
    }

    /* Start find phase */
    if (wfd_wlan_update_mode(WFD_MODE_START_FIND_PHASE) != WPS_STATUS_SUCCESS) {
        wps_printf(MSG_ERROR, "WFD find phase start failed.\n");
        return WPS_STATUS_FAIL;
    }
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief Checks if WFD is started or not and starts it in later case.
 *         Start Group power as well.
 *
 *         ./wfdutl mlan0 wfd 1
 *         ./wfdutl mlan0 wfd 2
 *
 *  @return         WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wfdcmd_start_group_owner(void)
{

    /* Get WFD mode and start if needed */
    if (wfd_wlan_update_mode(WFD_MODE_QUERY) != WPS_STATUS_SUCCESS) {
        wps_printf(MSG_ERROR, "WFD start failed.\n");
        return WPS_STATUS_FAIL;
    }

    /* Start GO mode */
    if (wfd_wlan_update_mode(WFD_GO_QUERY) != WPS_STATUS_SUCCESS) {
        wps_printf(MSG_ERROR, "WFD start GO failed.\n");
        return WPS_STATUS_FAIL;
    }
    /* 
     * Start BSS now.
     */
    if (apcmd_start_bss(&wps_global) == WPS_STATUS_FAIL) {
        wps_printf(MSG_INFO, "GO AP is already running.\n");
    }
    return WPS_STATUS_SUCCESS;
}
#endif

/** 
 *  @brief  Remove the password IE in the device configuration.
 *          First Get the config and remove password if it exists. 
 *  @param  None
 *
 *  @return  WPS_STATUS_SUCCESS--success, WPS_STATUS_FAIL--fail
 */
int
wps_wlan_remove_password_ie_config(void)
{
    int i, ret = WPS_STATUS_SUCCESS;
    short ie_index = 0;
    u8 *buf;
    u16 type, length;
    u16 ie_len_wfd, ie_len_wps;
    tlvbuf_custom_ie *tlv = NULL;
    custom_ie *ie_ptr = NULL;
    u8 *curr_ptr = NULL;
    s16 left_len = 0;

    ENTER();
    buf = (u8 *) MEM_MALLOC(MRVDRV_SIZE_OF_CMD_BUFFER);
	if (buf) {
	    memset(buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
	    ret = wfd_ie_config((u16 *) & ie_index, 0, 0, buf);

	    if (ret == WPS_STATUS_SUCCESS && ie_index < (MAX_MGMT_IE_INDEX - 1)) {
	        tlv = (tlvbuf_custom_ie *) buf;
	        if (tlv->tag == MRVL_MGMT_IE_LIST_TLV_ID) {
	            ie_ptr = (custom_ie *) (tlv->ie_data);
	            /* Goto appropriate Ie Index */
	            for (i = 0; i < ie_index; i++) {
	                ie_ptr = (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) +
	                                        ie_ptr->ie_length);
	            }
	            ie_len_wfd = ie_ptr->ie_length;

	            ie_ptr =
	                (custom_ie *) ((u8 *) ie_ptr + sizeof(custom_ie) + ie_len_wfd);
	            if (!
	                (ie_ptr->mgmt_subtype_mask == MASK_WFD_AUTO &&
	                 ie_ptr->ie_length)) {
	                wps_printf(MSG_WARNING,
	                           "Failed to get WPS IE to remove device password.\n");
	                return WPS_STATUS_FAIL;
	            }
	            ie_len_wps = ie_ptr->ie_length;

	            wps_hexdump(DEBUG_WLAN, "IE Buffer without device password IE",
	                        ie_ptr->ie_buffer, ie_len_wps);

	            curr_ptr = (u8 *) ie_ptr->ie_buffer;
	            left_len = ie_len_wps;
	            while (left_len > sizeof(tlvbuf_wps_ie)) {
	                memcpy(&type, curr_ptr, sizeof(u16));
	                type = ntohs(type);
	                memcpy(&length, curr_ptr + 2, sizeof(u16));
	                length = ntohs(length);
	                if (type == SC_Device_Password_ID) {
	                    memmove(curr_ptr, curr_ptr + SZ_DEVICE_PASSWORD_ID_TLV,
	                            left_len - SZ_DEVICE_PASSWORD_ID_TLV);
	                    ie_len_wps -= 6;
	                    break;
	                }
	                curr_ptr += length + sizeof(tlvbuf_wps_ie);
	                left_len -= length + sizeof(tlvbuf_wps_ie);
	            }

	            wps_hexdump(DEBUG_WLAN, "IE Buffer without device passwod ie",
	                        ie_ptr->ie_buffer, ie_len_wps);

	            /* Update New IE now */
	            ret =
	                wfd_ie_config((u16 *) & ie_index, ie_len_wfd, ie_len_wps, buf);
	            if (ret != WPS_STATUS_SUCCESS) {
	                wps_printf(MSG_WARNING,
	                           "Failed to update WPS IE after removing device password.\n");
	            }
	        }
	    }
        MEM_FREE_AND_NULL(buf);
	} else {
        DPRINTF(DBG_OUTPUT|DBG_SOFT, ("ERR:Cannot allocate memory!\n"));
		ret = WPS_STATUS_FAIL;
	}
    LEAVE();
    return ret;
}
