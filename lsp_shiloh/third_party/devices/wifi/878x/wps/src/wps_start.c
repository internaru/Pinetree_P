/** @file wps_start.c
 *  @brief This file contains WPS application program entry function
 *           and functions for initialization setting.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#include "common.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "wps_msg.h"
#include "wps_eapol.h"
#include "wps_util.h"
#include "wps_def.h"
#include "wps_wlan.h"
#include "wps_os.h"
#include "wps_version.h"
#include "wireless_copy.h"
#include "wlanInterface.h"
#include "encrypt.h"
#include "wps_l2.h"
#include "wps_events.h"
#include "delay_api.h"
#include "dprintf.h"
#include "tx_api.h"
#include "platform_api.h"
#include "logger.h"

void do_gettimeofday(struct timeval *time);
int cancelWPSIfRequested(void);

/********************************************************
        Local Variables
********************************************************/

/********************************************************
        Global Variables
********************************************************/
/** IE buffer index */
extern short ie_index;
extern PWPS_INFO gpwps_info;

/** IE buffer index */
extern short ap_assocresp_ie_index;
extern short probe_ie_index;
/*Global WPS timeval*/
struct timeval gWPSStarttime;
bool g_wps_session_active = false;
static bool g_wps_timeout_event_flag = false;
static int g_wps_countdown_val = 0;
delay_msec_timer_t   *wps_timer = NULL;

#define WPS_TIMER_CALLBACK_TIME  1  

/********************************************************
        Local Functions
********************************************************/
void CancelWPSSession(void)
{
    g_wps_session_active = false;
    CancelWPStimer();
}    

#if 0
/** 
 *  @brief WPS PIN value Generate
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             None
 */
void
wps_generate_PIN(PWPS_INFO pwps_info)
{
    int i;
    char pin_str[20];
    ENTER();

    if (pwps_info->enrollee.device_password_id == DEVICE_PASSWORD_ID_PIN
        || pwps_info->registrar.device_password_id == DEVICE_PASSWORD_ID_PIN) {
        if (pwps_info->static_pin == 0) {
            /* Generate PIN Number */
            u32 pin_number;
            pin_number = get_wps_pin(pwps_info);
            {
                u32 j, temppin = pin_number;
                u8 quotient;

                j = ((pwps_info->PINLen == 8) ? 10000000 : 1000);

                for (i = 0; i < pwps_info->PINLen; i++) {
                    quotient = temppin / j;
                    pwps_info->PIN[i] = quotient;
                    pwps_info->PIN[i] += 0x30;  /* Convert to numeric digit */
                    temppin -= j * quotient;
                    j = j / 10;
                }
            }
        }
        wps_hexdump(DEBUG_INIT, "PIN Number", (u8 *) pwps_info->PIN,
                    pwps_info->PINLen);

        printf("\nPIN Number is : ");
        for (i = 0; i < pwps_info->PINLen; i++)
            printf("%c", pwps_info->PIN[i]);
        printf("\n");
    }

    LEAVE();
}

/**
 *  @brief  Validate checksum of PIN
 *
 *  @param PIN      PIN value
 *  @return         Validation result 1 - Success 0 - Failure
 */
int
ValidateChecksum(unsigned long int PIN)
{
    u32 accum = 0;

    ENTER();

    accum += 3 * ((PIN / 10000000) % 10);
    accum += 1 * ((PIN / 1000000) % 10);
    accum += 3 * ((PIN / 100000) % 10);
    accum += 1 * ((PIN / 10000) % 10);
    accum += 3 * ((PIN / 1000) % 10);
    accum += 1 * ((PIN / 100) % 10);
    accum += 3 * ((PIN / 10) % 10);
    accum += 1 * ((PIN / 1) % 10);
    LEAVE();

    return (0 == (accum % 10));
}

/** 
 *  @brief Process user input PIN value
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param input        A pointer to character array containing input string
 *  @return             None
 */
int
wps_registrar_input_PIN(PWPS_INFO pwps_info, char *input_pin2)
{
    char inp[20], input[20];
    u32 wps_pin;
    int i, ret = WPS_STATUS_SUCCESS, original_pin_len = 0;

    ENTER();

    memset(input, 0, sizeof(input));

    ret = wps_sanitize_input(input_pin2, input);

    wps_printf(DEBUG_ALL, "Input PIN received %s", input);
    original_pin_len = pwps_info->PINLen;

    if (pwps_info->registrar.device_password_id == DEVICE_PASSWORD_ID_PIN) {
        do {
            int len;

            if (wps_non_interactive)
                len = (int) strlen(input);
            else
                len = (int) strlen(input) - 1;

            if (len != original_pin_len) {

                if (len == 4 || len == 8) {
                    wps_printf(DEBUG_INIT, "Setting PIN length to %d", len);
                    original_pin_len = len;
                } else {

                    wps_printf(MSG_ERROR, "PIN length can be either 4 or 8!!\n");
                    return WPS_STATUS_FAIL;
                }
            }

            inp[len] = '\0';
            for (i = 0; i < original_pin_len; i++) {
                if (isdigit(input[i]))
                    pwps_info->PIN[i] = input[i];
                else
                    break;
            }
            if (i != original_pin_len) {
                wps_printf(MSG_ERROR,
                    "Please input correct PIN value, letters are not allowed\n");
                return WPS_STATUS_FAIL;
            }

            if (!wps_non_interactive)
                printf("\nInput PIN : %s", input);

            wps_pin = (u32) (strtoul((char *) pwps_info->PIN, NULL, 10));

            if (original_pin_len == 8) {
                if (ValidateChecksum(wps_pin)) {
                    wps_printf(DEBUG_INPUT, "\nChecksum validation successful");
                    wps_write_status
                        ("WPS PIN session: Checksum validation successful");
                } else {
                    if (wps_non_interactive) {
                        wps_write_status
                            ("WPS PIN session: Checksum validation failed!!");
                        pwps_info->input_state = WPS_INPUT_STATE_METHOD;

                        /* Cancel WPS Registration Timer */
                        wps_cancel_timer(wps_registration_time_handler,
                                         pwps_info);

                        pwps_info->mode = IEEE80211_MODE_INFRA;

                        ret = wps_wlan_ie_config(SET_WPS_IE, &ie_index);
                        if (ret != WPS_STATUS_SUCCESS) {
                            wps_printf(MSG_WARNING, "Setting WPS IE failed!!!");
                        }
                        pwps_info->pin_pbc_set = WPS_CANCEL;
                        wps_user_input_handler(STDIN_FILENO, pwps_info);
                    } else {
                        printf("\nChecksum validation failed");
                        wps_print_registration_method_menu(pwps_info);
                        pwps_info->input_state = WPS_INPUT_STATE_METHOD;
                        ret = wps_wlan_ie_config(SET_WPS_IE, &ie_index);
                        if (ret != WPS_STATUS_SUCCESS) {
                            wps_printf(MSG_WARNING, "Setting WPS IE failed!!!");
                            return ret;
                        }
                    }

                    if (pwps_info->registrar.version >= WPS_VERSION_2DOT0) {
                        ret =
                            wps_wlan_ie_config
                            (SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                             &ap_assocresp_ie_index);
                        if (ret != WPS_STATUS_SUCCESS) {
                            wps_printf(MSG_WARNING,
                                       "WPSE IE configuration failure.\n");
                        }
                    }
                    fflush(stdout);
                    return WPS_STATUS_FAIL;

                }
            }
            break;

        } while (1);

        pwps_info->PINLen = original_pin_len;
    } else {
        /* PBC, Set PIN to all '0' */
        for (i = 0; i < 8; i++)
            pwps_info->PIN[i] = 0x30;
    }

    pwps_info->input_state = WPS_INPUT_STATE_NO_INPUT;
    pwps_info->pin_pbc_set = WPS_SET;

    printf("\n");
    printf("WPS Registration Protocol Starting ..... \n");
    printf("Wait for Enrollee to Connect.\n");
    printf("Press [q/Q] to abort this program.\n\n");

    LEAVE();
    return ret;
}
#endif

/** 
 *  @brief Process user input PIN value
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             None
 */
void
wps_input_PIN(PWPS_INFO pwps_info)
{
#if 0
    char inp[10];
    u32 wps_pin;
    int i, len = 0, original_pin_len = 0;

    ENTER();

    original_pin_len = pwps_info->PINLen;

    if (pwps_info->registrar.device_password_id == DEVICE_PASSWORD_ID_PIN
        || pwps_info->enrollee.device_password_id == DEVICE_PASSWORD_ID_PIN) {
        do {

            /* Request User to input PIN */
            printf("\nPlease Input %d-digit Numeric PIN : ", original_pin_len);
            fgets(inp, 10, stdin);
            len = (int) strlen(inp) - 1;
            fflush(stdin);

            if (len == original_pin_len) {
                inp[len] = '\0';
                printf("\nInput PIN : %s", inp);
                for (i = 0; i < len; i++) {
                    pwps_info->PIN[i] = inp[i];
                }

                wps_pin = (u32) (strtoul((char *) pwps_info->PIN, NULL, 10));

                if (ValidateChecksum(wps_pin))
                    wps_printf(DEBUG_INPUT, "\nChecksum validation successful");
                else {
                    printf("\nChecksum validation failed");
                    continue;

                }
                break;
            } else {
                printf
                    ("Input PIN length and configuration file PIN length doesn't match.\n");
                fflush(stdin);
            }

        } while (1);

    } else {
        /* PBC, Set PIN to all '0' */
        for (i = 0; i < 8; i++)
            pwps_info->PIN[i] = 0x30;
    }

    pwps_info->input_state = WPS_INPUT_STATE_NO_INPUT;
    pwps_info->pin_pbc_set = WPS_SET;
    LEAVE();
#else
    int i;

    ENTER();

    /* PBC, Set PIN to all '0' */
    for (i = 0; i < 8; i++)
        pwps_info->PIN[i] = 0x30;

    LEAVE();

#endif
}

#if 0
/** 
 *  @brief Check if scan result contain any dual band AP entry
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_wlan_check_for_dual_band_ap(PWPS_INFO pwps_info,
                                SCAN_RESULTS * pdual_band_scan_results)
{
#define BYPASS_DUAL_BAND_CHECK 1
    int ret = WPS_STATUS_SUCCESS;
    u8 pbc_registrar_count = 0;
    int i = 0;
    u16 device_password_id = 0xffff;
#if BYPASS_DUAL_BAND_CHECK
    u8 uuid_e[2][16];
    u16 ie_length;
#endif

    ENTER();
#if BYPASS_DUAL_BAND_CHECK

    for (i = 0; i < 2; i++) {

        ie_length = pdual_band_scan_results[i].wps_ie_len;
        if (ie_length > 0) {
            ret =
                wps_probe_response_uuid_parser(pdual_band_scan_results[i].
                                               wps_ie,
                                               pdual_band_scan_results[i].
                                               wps_ie_len, &uuid_e[i][0]);
            if (ret != WPS_STATUS_SUCCESS) {
                printf("\n");
                printf("\nCould not retrieve UUID from scan result!");
                ret = WPS_STATUS_FAIL;
                LEAVE();
                return ret;
            }
        }
    }

    wps_hexdump(DEBUG_WPS_MSG, "UUID[0]", &uuid_e[0][0], 16);
    wps_hexdump(DEBUG_WPS_MSG, "UUID[1]", &uuid_e[1][0], 16);

    if (memcmp(&uuid_e[0][0], &uuid_e[1][0], 16) == 0) {
        /* Same UUID, dual-band AP */
        wps_printf(MSG_INFO, "Dual-band AP detected in PBC auto mode");
    } else {
#endif
        for (i = 0; i < 2; i++) {
            device_password_id =
                wps_probe_response_device_password_id_parser
                (pdual_band_scan_results[i].wps_ie,
                 pdual_band_scan_results[i].wps_ie_len);
            if (device_password_id == pwps_info->enrollee.device_password_id) {

                pbc_registrar_count++;
            }
        }
        if (pbc_registrar_count > 1) {
            switch (pwps_info->enrollee.device_password_id) {
            case DEVICE_PASSWORD_PUSH_BUTTON:
                printf("\nPBC Session Overlap Detected!");
                printf("\n");
                break;
            case DEVICE_PASSWORD_SMPBC:
                printf("\nSMPBC Session Overlap Detected!");
                printf("\n");
                break;
            }
            ret = WPS_STATUS_FAIL;
        }
#if BYPASS_DUAL_BAND_CHECK
    }
#endif
    LEAVE();
    return ret;
}

/**
 * @brief Checks wheather PBC overlap condition has occured during scan
 * @param wps_s        A pointer to global WPS structure
 * @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 *
 * */
int
wps_wlan_check_for_PBC_overlap(PWPS_INFO pwps_info, WPS_DATA * wps_s)
{
    int i = 0, ret = WPS_STATUS_SUCCESS;
    u8 pbc_registrar_count = 0;
    u16 device_password_id = 0xffff;
    SCAN_RESULTS *pscan_results = NULL;
    ENTER();
    if (wps_s->num_scan_results == 1) {
        printf("\nOne AP with Active PBC Session Detected !\n");
        return WPS_STATUS_SUCCESS;
    }

    /* Check for dual-band AP */
    if (wps_s->num_scan_results == 2) {

        pscan_results = (SCAN_RESULTS *) malloc(sizeof(SCAN_RESULTS) * 2);
        for (i = 0; i < 2; i++)
            memcpy(&pscan_results[i], &wps_s->scan_results[i],
                   sizeof(SCAN_RESULTS));
        ret = wps_wlan_check_for_dual_band_ap(pwps_info, pscan_results);
    }

    else {
        for (i = 0; i < wps_s->num_scan_results; i++) {
            device_password_id =
                wps_probe_response_device_password_id_parser(wps_s->
                                                             scan_results
                                                             [i].wps_ie,
                                                             wps_s->
                                                             scan_results
                                                             [i].wps_ie_len);

            if (device_password_id == pwps_info->enrollee.device_password_id) {
                pbc_registrar_count++;
                pscan_results =
                    realloc(pscan_results,
                            sizeof(SCAN_RESULTS) * pbc_registrar_count);
                memcpy(&pscan_results[pbc_registrar_count - 1],
                       &wps_s->scan_results[i], sizeof(SCAN_RESULTS));
            }
        }
        if (pbc_registrar_count > 2) {
            printf("\n");
            printf("PBC Session Overlap Detected!");
            printf("\n");
            ret = WPS_STATUS_FAIL;
        } else if (pbc_registrar_count == 2) {
            ret = wps_wlan_check_for_dual_band_ap(pwps_info, pscan_results);
        }
    }
    LEAVE();
    return ret;
}
#endif


void wps_timer_expired(ULONG unused)
{
   struct timeval now, res;
   struct timeval pbcWalkTime;
   
   if (g_wps_session_active)
   { 
         if (gpwps_info->enrollee.device_password_id == DEVICE_PASSWORD_ID_PIN)
    	 {
    	     /* Set PBC Walk Timer to 240 seconds */	     	 
             pbcWalkTime.tv_sec = PIN_WALK_TIME;
             pbcWalkTime.tv_usec = 0;              	     
   	 }
   	 else
   	 {
   	     /* Set PBC Walk Timer to 120 seconds */	 
   	     pbcWalkTime.tv_sec = PBC_WALK_TIME;
   	     pbcWalkTime.tv_usec = 0;	     
   	 }
	 
   	os_get_time(&now);
   	timersub(&now, &gWPSStarttime, &res);
        if (!(timercmp(&res, &pbcWalkTime, >=))) 
	{
	   if (g_wps_countdown_val)
	   {	   
	   	wps_notify_event(WPS_PROCESSING_EVENT, g_wps_countdown_val);
           	g_wps_countdown_val--;
	   }	
	   else
	        wps_notify_event(WPS_PROCESSING_EVENT, 0);	   
	}
        else
	{
           if (g_wps_timeout_event_flag == false)		   
	   {	      
	      if (g_wps_countdown_val)
	      {	   
	   	wps_notify_event(WPS_PROCESSING_EVENT, g_wps_countdown_val);
           	g_wps_countdown_val--;
	      }	
	      else
	      {	      
	        wps_notify_event(WPS_PROCESSING_EVENT, 0);	      
	        g_wps_timeout_event_flag = true;
	      }	
           }
           else
	   {              		   
	      updateWPSOutcome(WPS_STATE_TIMEOUT);
           } 	      
	}	
   }	
}


/** 
 *  @brief Process WPS Enrollee Push Button or PIN auto
 *         detection and connection operations
 * 
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to global WPS structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_enrollee_start(PWPS_INFO pwps_info, WPS_DATA * wps_s, char *ssid)
{
    int ret = WPS_STATUS_SUCCESS;
#if 0
    int flags;
#endif
    int retry_cnt = 50;
    int scan_cnt = 0;
    u8 bssid_get[ETH_ALEN];
    struct timeval start, now, res;
    struct timeval pbcWalkTime, pinStopTime;
    int i;
    u8 uuid_e[2][16];

    ENTER();

    /* 
     * For PBC-auto, STA will maintain 2-min walk time to scan PBC 
     *  AP periodically.
     * 1) If no PBC AP found, app exits when 2-min timer timeout.
     * 2) If overlapped PBC found, app prints an error message, cancels 
     *     timer and exits.
     * 3) If one PBC AP found (including dual-band AP), app cancels timer 
     *     and starts WPS connection/registration automatically.
     */

    if (!pwps_info->enrollee.auth_type_flag)
        pwps_info->enrollee.auth_type_flag = AUTHENTICATION_TYPE_ALL;
    if (!pwps_info->enrollee.encry_type_flag)
        pwps_info->enrollee.encry_type_flag = ENCRYPTION_TYPE_ALL;

    wps_printf(MSG_INFO, "\n");
    wps_printf(MSG_INFO, "Start Active Scan ...\n");
    DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Start Active Scan ...\n"));

    if (pwps_info->enrollee.device_password_id == DEVICE_PASSWORD_ID_PIN)
    {
        /* Set PBC Walk Timer to 240 seconds */
        pbcWalkTime.tv_sec = PIN_WALK_TIME;
        pbcWalkTime.tv_usec = 0;

        /* Set Pin Stop Timer to 30 seconds */
        pinStopTime.tv_sec = PIN_WALK_TIME - 30;
        pinStopTime.tv_usec = 0;
    }
    else
    {
        /* Set PBC Walk Timer to 120 seconds */
        pbcWalkTime.tv_sec = PBC_WALK_TIME;
        pbcWalkTime.tv_usec = 0;
    }

    g_wps_session_active = true;
    g_wps_timeout_event_flag = false;
    g_wps_countdown_val = pbcWalkTime.tv_sec; 
    wps_timer =  delay_msec_timer_non_blocking(1000 * WPS_TIMER_CALLBACK_TIME, 
			                  (void *)wps_timer_expired, NULL, true);

    /* Mark the start of PBC Walk Timer */
    os_get_time(&start);

    /*Reset the global WPS Start time*/
    gWPSStarttime.tv_sec = start.tv_sec;
    gWPSStarttime.tv_usec = start.tv_usec;

    /* Enable driver WPS session checking */
    wps_wlan_session_control(WPS_SESSION_ON);

    /* Enrollee need to do the scanning function */
  pbc_scan:

    if (cancelWPSIfRequested() == 2)
    {
        // returns 2 to refresh timer (instead of cancel, they want to run longer)
        wps_printf(MSG_INFO, "Resetting PBC timer\n");
        os_get_time(&start);
    }
    if (wps_s->userAbort == WPS_SET)
    {
        wps_printf(MSG_INFO, "Exit WPS due to user abort.\n");
        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Exit WPS due to user abort.\n"));

        ret = WPS_STATUS_FAIL;
        goto exit;
    }

    /* Compare time of PBC Walk Timer */
    os_get_time(&now);
    timersub(&now, &start, &res);
    if (timercmp(&res, &pbcWalkTime, >=)) {
        wps_printf(MSG_INFO, "\n");
        wps_printf(MSG_INFO, "\nExit program for PBC walk time expired.\n");
        wps_printf(MSG_INFO, "\n\n");
        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("Exit program for PBC walk time expired.\n"));

        updateWPSOutcome(WPS_STATE_TIMEOUT);

        ret = WPS_STATUS_FAIL;
        goto exit;
    }
    if (pwps_info->role == WPS_ADHOC_EXTENTION
        || (pwps_info->enrollee.version >= WPS_VERSION_2DOT0)
        ) {
        if (probe_ie_index == -1) {
            wps_printf(DEBUG_INIT, "GET_PROBE_REQ_WPS_IE\n");
            ret = wps_wlan_probe_request_ie_config(GET_WPS_IE, &probe_ie_index);
            if (ret != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_ERROR, "Failed to set STA Probe Req IEs\n");
                goto exit;
            }
        }
        /* configure Probe request IE */
        wps_wlan_probe_request_ie_config(SET_WPS_STA_SESSION_ACTIVE_IE,
                                         &probe_ie_index);
    }
    wps_wlan_scan(wps_s, ssid);

    tx_thread_sleep(50);

    if (pwps_info->enrollee.device_password_id == DEVICE_PASSWORD_ID_PIN) {
        // PIN mode
		wps_wlan_scan_results(wps_s, FILTER_PIN, ssid);
    } else {
        // PBC mode
        wps_wlan_scan_results(wps_s, FILTER_PBC, ssid);
    }
    if (pwps_info->role == WPS_ADHOC_EXTENTION
        || (pwps_info->enrollee.version >= WPS_VERSION_2DOT0)
        ) {
        /* De-configure Probe request IE */
        wps_wlan_probe_request_ie_config(CLEAR_WPS_IE, &probe_ie_index);
    }
    if (wps_s->num_scan_results == 1 || wps_s->num_scan_results == 2) {
        /* Check for dual-band AP */
        if (wps_s->num_scan_results == 2) {
            for (i = 0; i < wps_s->num_scan_results; i++) {
                if (wps_s->scan_results[i].wps_ie_len > 0) {
                    ret =
                        wps_probe_response_uuid_parser(wps_s->scan_results[i].
                                                       wps_ie,
                                                       wps_s->scan_results[i].
                                                       wps_ie_len,
                                                       &uuid_e[i][0]);

                    if (ret != WPS_STATUS_SUCCESS) {
                        wps_printf(MSG_INFO, "\n");
                        wps_printf(MSG_INFO, "\nPBC Session Overlapped Detected !");
                        wps_printf(MSG_INFO, "\n\n");
                        ret = WPS_STATUS_FAIL;
                        goto exit;
                    }
                }
            }

            wps_hexdump(MSG_INFO, "UUID[0]", &uuid_e[0][0], 16);
            wps_hexdump(MSG_INFO, "UUID[1]", &uuid_e[1][0], 16);

            if (memcmp(&uuid_e[0][0], &uuid_e[1][0], 16) == 0) {
                /* Same UUID, dual-band AP */
                wps_printf(MSG_ERROR, "Dual-band AP detected in PBC auto mode");
            } else {
                wps_printf(MSG_INFO, "\n");
                wps_printf(MSG_INFO, "\nPBC Session Overlapped Detected !");
                wps_printf(MSG_INFO, "\n\n");
                ret = WPS_STATUS_FAIL;
                goto exit;
            }
        }

        /* mode: 0 = infra/BSS, 1 = adhoc/IBSS */
        if (wps_s->scan_results[0].caps & IEEE80211_CAP_ESS) {
            wps_wlan_set_mode(IEEE80211_MODE_INFRA);
            wps_s->current_ssid.mode = IEEE80211_MODE_INFRA;
            pwps_info->mode = IEEE80211_MODE_INFRA;
        } else if (wps_s->scan_results[0].caps & IEEE80211_CAP_IBSS) {
            wps_wlan_set_mode(IEEE80211_MODE_IBSS);
            wps_s->current_ssid.mode = IEEE80211_MODE_IBSS;
            pwps_info->mode = IEEE80211_MODE_IBSS;
        }

        if (wps_s->scan_results[0].caps & IEEE80211_CAP_ESS) {
            if (pwps_info->role == WPS_ENROLLEE &&
                pwps_info->enrollee.version >= WPS_VERSION_2DOT0) {
                /* Parsing Authorized MAC address field for ENROLLEE mac
                   address */
                if (wps_s->scan_results[0].wps_ie_len != 0) {
                    ret =
                        wps_probe_response_authorized_enrollee_mac_parser
                        (pwps_info, (u8 *) & wps_s->scan_results[0].wps_ie,
                         wps_s->scan_results[0].wps_ie_len);
                }
                if (pwps_info->enrollee_in_authorized_mac == WPS_CANCEL) {
                    wps_printf(MSG_INFO, 
							   "Enrollee is not authorized to register with this Registrar.\n");
                    ret = WPS_STATUS_FAIL;
                    goto exit;
                } else {
                    wps_printf(DEBUG_EAPOL,
                               "Enrollee's Mac Address found in Registrar's Authorized Enrollee list.Continue...");
                }
            }
        }
        do {
            wps_printf(MSG_INFO, "\nConnecting ..... \n");

            //wps_wlan_set_authentication(AUTHENTICATION_TYPE_OPEN);

            wps_wlan_set_wap((u8 *) wps_s->scan_results[0].bssid);

			tx_thread_sleep(10);

            memset(bssid_get, 0x00, ETH_ALEN);
            wps_wlan_get_wap(bssid_get);
            if ((memcmp(bssid_get, wps_s->scan_results[0].bssid, ETH_ALEN) ==
                 0)) {

                wps_printf(MSG_INFO, "\nConnected to following BSS (or IBSS) :\n");
                wps_printf(MSG_INFO,
                     "SSID = [%s], BSSID = [%02x:%02x:%02x:%02x:%02x:%02x]\n\n",
                     wps_s->scan_results[0].ssid,
                     wps_s->scan_results[0].bssid[0],
                     wps_s->scan_results[0].bssid[1],
                     wps_s->scan_results[0].bssid[2],
                     wps_s->scan_results[0].bssid[3],
                     wps_s->scan_results[0].bssid[4],
                     wps_s->scan_results[0].bssid[5]);

                /* Save information to global structure */
                wps_s->current_ssid.ssid_len =
				     wps_s->scan_results[0].ssid_len;
                memcpy(wps_s->current_ssid.ssid,
				       wps_s->scan_results[0].ssid,
                       wps_s->current_ssid.ssid_len);

                memcpy(wps_s->current_ssid.bssid,
                       wps_s->scan_results[0].bssid, ETH_ALEN);

                /* Store Peer MAC Address */
                if (pwps_info->role == WPS_ENROLLEE
                    || (IS_DISCOVERY_ENROLLEE(pwps_info))
                    ) {
                    memcpy(pwps_info->registrar.mac_address,
                           wps_s->scan_results[0].bssid, ETH_ALEN);
                }
                /* Save RF Band information for M1 message */
                wps_printf(MSG_ERROR, "START ENROLLEE rf_band %d %d\n", pwps_info->enrollee.rf_bands, wps_s->scan_results[0].freq);

                pwps_info->enrollee.rf_bands =
                    (u8) wps_wlan_freq_to_band(wps_s->scan_results[0].freq);

                wps_printf(MSG_ERROR, "START ENROLLEE2 rf_band %d\n", pwps_info->enrollee.rf_bands);

                /* Disable driver WPS session checking */
                wps_wlan_session_control(WPS_SESSION_OFF);

                wps_printf(MSG_INFO, "WPS Registration Protocol Starting ..... \n");
#if 0
                wps_printf(MSG_INFO, "Press [q/Q] to abort this program.\n\n");

                if (wps_wlan_get_ifflags(&flags, wps_s->ifname) != 0 ||
                    wps_wlan_set_ifflags(flags | IFF_UP, wps_s->ifname) != 0) {
                    wps_printf(MSG_WARNING, "Could not set interface '%s' UP\n",
                               wps_s->ifname);
                }
#endif
                /* Start WPS registration timer */
                wps_start_registration_timer(pwps_info);

                /* Starting WPS Message Exchange Engine */
                wps_state_machine_start(pwps_info);

                ret = WPS_STATUS_SUCCESS;
                break;
            } else {
                retry_cnt--;
                wps_printf(DEBUG_WLAN | MSG_ERROR,
                           "Connect to AP FAIL ! Retrying ..... ");
                wps_printf(DEBUG_WLAN | DEBUG_INIT, "Retry Count = %d",
                           retry_cnt);

                /* Compare time of PBC Walk Timer */
                os_get_time(&now);
                timersub(&now, &start, &res);
                if (timercmp(&res, &pbcWalkTime, >=)) {
                    ret = WPS_STATUS_FAIL;
                    goto exit;
                }

                /* Enable driver WPS session checking */
                wps_wlan_session_control(WPS_SESSION_ON);
            }

            cancelWPSIfRequested();

        } while ((retry_cnt != 0) && (wps_s->userAbort != WPS_SET));

        if (retry_cnt == 0) {
            wps_printf(MSG_INFO, "\n");
            wps_printf(MSG_INFO, "\nTry to connect to AP FAIL !");
            wps_printf(MSG_INFO, "\n\n");

            ret = WPS_STATUS_FAIL;
        }
        if (wps_s->userAbort == WPS_SET)
        {
            wps_printf(MSG_INFO, "\n");
            wps_printf(MSG_INFO, "\nWPS cancelled per user's request");
            wps_printf(MSG_INFO, "\n\n");

            ret = WPS_STATUS_FAIL;
        }
    } else if (wps_s->num_scan_results == 0) {
        if (pwps_info->enrollee.device_password_id == DEVICE_PASSWORD_ID_PIN)
        {
            // If we have not found an AP after X scans
            // initiate a sequence with all found access points
            if (scan_cnt++ >= 10)
            {
                u8 orig_wps_msg_max_retry = pwps_info->wps_msg_max_retry;
                u32 orig_wps_msg_timeout = pwps_info->wps_msg_timeout;
                scan_cnt = 0;
    
                wps_wlan_get_all_scan_results(wps_s);

                if (wps_s->num_scan_results > 0) {
                    int k;
                    SCAN_RESULTS *loop_scan_results;
                    int loop_num_scan_results;

                    for (k = 0; k < wps_s->num_scan_results; k++) {
                        os_get_time(&now);
                        timersub(&now, &start, &res);
                        if (timercmp(&res, &pinStopTime, >=))
                        {
                            // Not enough time left
                            break;
                        }

                        // Check for Infrastructure (not Adhoc)
                        if ((wps_s->scan_results[k].caps & IEEE80211_CAP_IBSS) == 0) {
                            // Check for WPS support
                            if (wps_s->scan_results[k].wps_ie_len > 0) {
                                wps_printf(MSG_INFO, "\nTry SSID = [%s], BSSID = [%02x:%02x:%02x:%02x:%02x:%02x]\n\n",
                                     wps_s->scan_results[k].ssid,
                                     wps_s->scan_results[k].bssid[0],
                                     wps_s->scan_results[k].bssid[1],
                                     wps_s->scan_results[k].bssid[2],
                                     wps_s->scan_results[k].bssid[3],
                                     wps_s->scan_results[k].bssid[4],
                                     wps_s->scan_results[k].bssid[5]);

                                WlanSetWAP((char *)wps_s->scan_results[k].bssid);

                                memset(bssid_get, 0x00, ETH_ALEN);
                                wps_wlan_get_wap(bssid_get);
                                if ((memcmp(bssid_get, wps_s->scan_results[k].bssid, ETH_ALEN) == 0)) {
                                    /* Save information to global structure */
                                    wps_s->current_ssid.ssid_len = wps_s->scan_results[k].ssid_len;
                                    memcpy(wps_s->current_ssid.ssid, wps_s->scan_results[k].ssid,
                                           wps_s->current_ssid.ssid_len);

                                    memcpy(wps_s->current_ssid.bssid,
                                           wps_s->scan_results[k].bssid, ETH_ALEN);

                                    /* Save RF Band information for M1 message */
                                    pwps_info->enrollee.rf_bands =
                                        (u8) wps_wlan_freq_to_band(wps_s->scan_results[k].freq);

                                    pwps_info->wps_msg_max_retry = 1;
                                    pwps_info->wps_msg_timeout = 2000;
                                    pwps_info->wps_non_advertise = WPS_SET;

                                    /* Disable driver WPS session checking */
                                    wps_wlan_session_control(WPS_SESSION_OFF);

                                    /* Starting WPS Message Exchange Engine */
                                    wps_state_machine_start(pwps_info);

                                    // Enable main loop procedure 
                                    wps_main_loop_enable();

                                    // Main loop for socket read and timeout function
                                    // to wait for WPS Message Exchange Engine to complete
                                    wps_main_loop_proc();

                                    /* Enable driver WPS session checking */
                                    wps_wlan_session_control(WPS_SESSION_ON);

                                    pwps_info->wps_msg_max_retry = orig_wps_msg_max_retry;
                                    pwps_info->wps_msg_timeout = orig_wps_msg_timeout;
                                    pwps_info->wps_non_advertise = WPS_CANCEL;

                                    // Clean up
                                    if (wps_s->l2 != NULL) 
                                    {
                                        wps_l2_deinit(wps_s->l2);
                                        wps_s->l2 = NULL;
                                    }

                                    wps_main_loop_free();

                                    if (pwps_info->wps_valid_data == WPS_SET)
                                    {
                                        // Successfully registered with someone
                                        ret = WPS_SET;
                                        goto exit;
                                    }

                                    // Re-initialize WPS parameters
                                    wps_s->l2 = wps_l2_init(wps_s->ifname, WPS_ENROLLEE, ETH_P_EAPOL, wps_rx_eapol, 0);
                                    wps_parameter_initialization(gpwps_info, wps_s, WPS_ENROLLEE);
                                    
                                    if (cancelWPSIfRequested() == 2)
                                    {
                                        // returns 2 to refresh timer (instead of cancel, they want to run longer)
                                        wps_printf(MSG_INFO, "Resetting PBC timer\n");
                                        os_get_time(&start);
                                    }

                                    if (wps_s->userAbort == WPS_SET)
                                    {
                                        // Abort
                                        break;
                                    }

                                    os_get_time(&now);
                                    timersub(&now, &start, &res);
                                    if (timercmp(&res, &pbcWalkTime, >=))
                                    {
                                        // Time expired
                                        break;
                                    }

                                    // Store WPS Scan
                                    loop_scan_results = wps_s->scan_results;
                                    wps_s->scan_results = NULL;
                                    loop_num_scan_results = wps_s->num_scan_results;

                                    // Rescan
                                    wps_wlan_scan(wps_s, ssid);
                                    tx_thread_sleep(50);
                                    wps_wlan_scan_results(wps_s, FILTER_PIN, ssid);

                                    if (wps_s->num_scan_results == 1)
                                    {
                                        // Found an Advertising Access Point 
                                        os_free(loop_scan_results);
                                        break;
                                    }
                                    else
                                    {
                                        // Restore WPS Scan
                                        if (wps_s->scan_results)
                                            os_free(wps_s->scan_results);

                                        wps_s->scan_results = loop_scan_results;
                                        wps_s->num_scan_results = loop_num_scan_results;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /* Scanning again */
        goto pbc_scan;
    } else {
        wps_printf(MSG_INFO, "\n");
        wps_printf(MSG_INFO, "\nPBC Session Overlapped Detected !");
        wps_printf(MSG_INFO, "\n\n");
        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("\nPBC Session Overlapped Detected !\n\n"));

        updateWPSOutcome(WPS_STATE_SESSION_OVERLAP);
        
        ret = WPS_STATUS_FAIL;
    }

  exit:
    LEAVE();
    return ret;
}

/** 
 *  @brief Process WPS Registrar operations
 * 
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to global WPS structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_registrar_start(PWPS_INFO pwps_info, WPS_DATA * wps_s)
{
    int ret = WPS_STATUS_SUCCESS;

    ENTER();

    if (!pwps_info->registrar.auth_type_flag)
        pwps_info->registrar.auth_type_flag = AUTHENTICATION_TYPE_ALL;
    if (!pwps_info->registrar.encry_type_flag)
        pwps_info->registrar.encry_type_flag = ENCRYPTION_TYPE_ALL;

    if (wps_s->bss_type == BSS_TYPE_STA
        && pwps_info->role == WPS_ADHOC_EXTENTION) {
        /* For STA mode */
        wps_s->current_ssid.mode = IEEE80211_MODE_IBSS;
        pwps_info->mode = IEEE80211_MODE_IBSS;

        /* REMOVED ::: Send adhoc registrar config command ADDED ::: MGMT_IE
           command */
        /* Get the free IE buffer index */

        if (ie_index == -1) {
            ret = wps_wlan_ie_config(GET_WPS_IE, &ie_index);
            if (ret != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_ERROR, "WPS Registrar failed to start\n");
                goto _exit_;
            }
        }

    }
#ifdef UAP_SUPPORT
    else if (wps_s->bss_type == BSS_TYPE_UAP) {
        /* For UAP mode */
        wps_s->current_ssid.mode = IEEE80211_MODE_INFRA;
        pwps_info->mode = IEEE80211_MODE_INFRA;

        if (!wps_non_interactive) {
            if (ie_index == -1) {
                /* Get the free IE buffer index */
                ret = wps_wlan_ie_config(GET_WPS_IE, &ie_index);
                if (ret != WPS_STATUS_SUCCESS) {
		            wps_printf(MSG_ERROR, "WPS Registrar failed to start\n");
                    goto _exit_;
                }
            }
            /* 
             * Send APCMD_SYS_CONFIGURE command to set WPS IE
             */
			if (strcmp(WIFI_IFACE_NAME_WFD, wps_s->ifname) == 0) {
                pwps_info->wps_session_active = WPS_SET;
                ret =
                    wps_wlan_ie_config(SET_WPS_AP_SESSION_ACTIVE_IE, &ie_index);
            } else {
                ret =
                    wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_IE,
                                       &ie_index);
            }
            if (ret != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_ERROR, "WPS Registrar failed to start.\n");
                goto _exit_;
            }
        }
        if (pwps_info->registrar.version >= WPS_VERSION_2DOT0) {
            if (ap_assocresp_ie_index == -1) {
                /* Get the free IE buffer index */
                ret = wps_wlan_ie_config(GET_WPS_IE, &ap_assocresp_ie_index);
                if (ret != WPS_STATUS_SUCCESS) {
                    wps_printf(MSG_ERROR, "WPS Registrar failed to start.\n");
                    goto _exit_;
                }
            }
			if (strcmp(WIFI_IFACE_NAME_WFD, wps_s->ifname) == 0) {
                ret =
                    wps_wlan_ie_config(SET_WPS_AP_SESSION_ACTIVE_AR_IE,
                                       &ap_assocresp_ie_index);
            } else {
                ret =
                    wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                                       &ap_assocresp_ie_index);
			}

            if (ret != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_WARNING, "WPSE IE configuration failure.\n");
            }
        }
    }
#endif
#if 0
    ret = wps_wlan_update_interface_flags(wps_s);
    if (ret != WPS_STATUS_SUCCESS)
        return ret;
#endif

    wps_printf(MSG_INFO, "\n");
    wps_printf(MSG_INFO, "WPS Registration Protocol Starting ..... \n");
    wps_printf(MSG_INFO, "Wait for Enrollee to Connect.\n");

	/* Start registration timer for WFD registrar */
	//if (strcmp(WIFI_IFACE_NAME_WFD, wps_s->ifname) == 0) 
	// Timeout on WFD or UAP registrar	
	{
		wps_start_registration_timer(pwps_info);
	}

    /* Starting WPS Message Exchange Engine */
    wps_state_machine_start(pwps_info);

  _exit_:
    LEAVE();
    return ret;
}

void CancelWPStimer(void)
{
    if (wps_timer != NULL)
    {	    
       wps_timer = delay_msec_timer_cancel(wps_timer);
    } 	
}	
