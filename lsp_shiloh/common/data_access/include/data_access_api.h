/*
 * ============================================================================
 * Copyright (c) 2008-2011   Marvell International, Ltd. All Rights Reserved.
 *
 *                      Marvell Confidential
 * ============================================================================
 */
/** 
 * \file data_access_api.h
 * 
 * \brief Public interface and definitions needed to support data variables.
 * 
 * This file contains the public interface and definitions needed to support
 * the access model for the data variables owned by individual components.
 * 
 **/

#ifndef INC_DATA_ACC_API_H
#define INC_DATA_ACC_API_H

#ifdef __cplusplus
extern "C" {
#endif

// dataAccess specific error_type_t values
#define DATA_ACC_STRING_TRUNCATED          1    ///< SetVar string too long, truncated to max chars for variable
#define DATA_ACC_ERR_NULL_PTR            -11    ///< incoming pointer is NULL
#define DATA_ACC_ERR_INVALID_VALUE       -12    ///< SetVar value not valid for variable
#define DATA_ACC_MEM_ALLOC_FAILED        -13    ///< memory allocation failed
#define DATA_ACC_ERR_STORAGE_DEV_FAILURE -14    ///< permanent storage device not accessible, values stored in RAM only

// 4-character permanent storage block identifiers (for use when registering a block with the appropriate
// driver).  New identifiers should be added as needed but must be unique.  A '*' character represents a
// wildcard reservation.
// THIS IS THE COMPREHENSIVE LIST THAT ALL PRODUCTS SHOULD USE TO MAINTAIN CONSISTENT ID USAGE
// ----- Let's try adding in alphabetical order since each MUST BE UNIQUE! -----
#define AOAO_VAR_ID          "AOAO"  // AutoOn AutoOff pwr schema mgr
#define CAL_NVM_VAR_ID       "CAL "  // Calibration
#define COPYDOC_VARS_ID      "CDOC"  // Copy Documents information
#define CLOUD_VARS_ID        "CLOP"  // Cloud ePrint
#define CONNECT_VARS_ID      "CONN"  ///< connection manager
#define CONSUM_VARS_ID       "CONS"  ///< consumables
#define COPYPHOTO_VARS_ID    "CPHO"  // Copy Photos information
#define CP_VARS_ID           "CTLP"  ///< control panel
#define DEVICE_ID_VARS_ID    "DEVI"  /// Device Identification information
#define DHCPD_VAR_ID         "DHCD"  // DHCP server (dhcpd) application
#define EMAIL_VARS_ID        "MAIL"  // GUI email information
#define ENG_VARS_ID          "ENG "  ///< engine
#define NET_ETH_VARS_ID      "ETH "  ///< Ethernet link
#define EVENT_LOGGER_VARS_ID "EVNT"  // Event Logger
#define FAXADDRESSES_VARS_ID "FADR"  // fax addresses
#define FAX_APP_VARS_ID      "FAXA"  // Fax Application Layer Vars
#define FAX_VARS_ID          "FAX "  // fax information
#define GCPP_VARS_ID         "GCPP"  // GCPP network applicaton
#define HTTP_VARS_ID         "HTTP"  ///< HTTP
#define INK_CONSUM_VARS_ID   "INKC"  // ink counts
#define INK_HISTORY_VARS_ID  "INKH"  // Ink tank history
#define JOB_VARS_ID          "JOB "  ///< job manager
#define NET_MDNS_VARS_ID     "MDN*"  // mDNS network applicaton ('*' represents wildcard reservation)
#define MFG_NVM_VAR_ID       "MFG "  // Manufacturing
#define NET_IFACE_VARS_ID    "NET*"  ///< network interface ('*' represents wildcard reservation)
#define PAPER_VARS_ID        "PAPR"  // paper handling
#define PCL_VARS_ID          "PCL "  // PCL
#define PRINT_JOB_CNTS_ID    "PCNT"  // Print counts
#define PEN_NVM_VAR_ID       "PEN "  // Pen maintenance information
#define PRINTHEAD_STATS_ID   "PH  "  // Printhead
#define PHOTO_VARS_ID        "PHOT"  // photo information
#define PJL_VARS_ID          "PJL "  // PJL
#define PJL_OEM_VARS_ID      "PJLO"  // PJL OEM Vendor Unique
#define PLAT_VARS_ID         "PLAT"  // platform
#define PRINT_VARS_ID        "PRNT"  // print manager
#define PS_VAR_ID            "PS  "  // Postscript
#define PAPERLESS_PRINTING_VAR_ID   "PLPR"      // PaperLess PRinting
#define PRINT_MODE_VAR_ID     "PRMD"            // PRint MoDe                                     
#define PWR_VAR_ID           "PWRM"  // Power manager
#define RTC_VARS_ID          "RTC "  // Real Time Clock
#define SCAN_VAR_ID          "SCAN"
#define NET_SLP_VARS_ID      "SLP*"  // SLP network applicaton ('*' represents wildcard reservation)
#define SNMP_VARS_ID         "SNM*"  // SNMP network applicaton ('*' represents wildcard reservation)
#define STRING_MGR_VAR_ID    "STRM"  // String Manager 
#define UAP_IFACE_VAR_ID     "UAP "  // uAP information
#define UI_VAR_ID            "UI  "  // ui information
#define USB_VAR_ID           "USB "  // USB
#define USBMS_VAR_ID         "USBMS" // USB mass storage
#define NET_WSD_VARS_ID      "WSD*"  // Web Services on Devices ('*' represents wildcard reservation)
#define WEBSERVICES_VAR_ID   "WSRV"  // web services information
#define WIFI_IFACE_VAR_ID    "WIF "  // Info for each WiFi Iface (last char added later providing index of iface)
#define WIFI_HW_VAR_ID       "WIFH"  // WiFi hardware info
#define WIRELESS_VAR_ID      "WIRE"  // wireless

// ----- Let's try adding in alphabetical order since each MUST BE UNIQUE! -----




#ifdef __cplusplus
}
#endif

#endif // INC_DATA_ACC_API_H
