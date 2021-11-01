/******************************************************************************
 * Copyright (c) 2008-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 * \file string_mgr_config.h
 *
 * \brief This file is a configration file for the string manager.
 *
 * To configure the string manager:
 *
 * The languages supported in this product should be added (and unused
 * languages removed) in the string_mgr_language_e enumeration.
 *
 * The string_mgr_code_e should be edited to add all strings in English
 * that the product supports.  These strings are then read by a
 * "string blobber" tool to generate binary files containing the strings
 * and embedded into the system for use.  The "string blobber" or another
 * tool shall also use translated files for each language to generate
 * an equivalent binary for each language.  In this way the entire set of
 * product strings can be localized for a product.
 *
 */

#ifndef STRING_MGR_CONFIG_H 
#define STRING_MGR_CONFIG_H

#include "string_mgr_api.h"
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 *
 * \brief This is a list of the langauges supported in this product
 *
 * languages must be in a format so that they can be used as 
 * an index into an array.  Any additional labels, such as
 * LANG_END_ALL_LOCAL, must be set to the next actual language
 * so that we don't have a gap in the array. 
 *
 * Language categories: Table definitions specify a category as well
 * FA - firmware all, do firmware localize on all tags for this lang
 * FP - firmware partial, do firmware localize on some of the tags (depends on table)
 * PC - pc language, do PC localize on all tags 
 *
 */
/* #BEGIN LANGUAGES */
enum string_mgr_language_e
{                                        /*!< Name        Abrev   Category File with localized strings */
    LANG_ENGLISH = 0,                    /*!< English     ENG     FA    lenglish.c */
    LANG_SPANISH = 1,                    /*!< Spanish     ES      FA    lspanish.c */
    LANG_END_PRINTER_LANG,                                                            
    LANG_NUM_PRINTER_LANG = LANG_END_PRINTER_LANG,    
    LANG_END_ALL_LOCAL,    
    LANG_END_PC_LANG,
    LANG_NUM_PC_LANG = LANG_END_PC_LANG,

    // languages below this are placeholders & not supported for the current configuration
    LANG_HEBREW,
    LANG_ARABIC,
};
/* #END LANGUAGES */


/**
 * \brief The strings are seperated into different tables.  The following
 * define specifies how many tables there are in this product
 */
typedef enum
{
    TABLE_STRING_STATUS_TABLE,
    TABLE_STRING_GENERIC_TABLE,
    TABLE_STRING_CTRL_PANEL_TABLE,
    TABLE_STRING_REPORT_PAGE_TABLE,
    TABLE_STRING_EWS_TABLE,
    /*
     * Do not add after this point
     */
    NUM_STRING_TABLES,             
} e_TABLE_LIST;


//#BEGIN TABLES                                  //Abrev  category
typedef enum
{
    STRING_STATUS_TABLE        = 0x00000000,   ///< STAT   FA
    STRING_GENERIC_TABLE       = 0x01000000,   ///< GEN    FA 
    STRING_CTRL_PANEL_TABLE    = 0x02000000,   ///< CP     FA 
    STRING_REPORT_PAGE_TABLE   = 0x03000000,   ///< RPT    FA 
    STRING_EWS_TABLE           = 0x04000000,   ///< EWS   FA 
} STRMGR_TABLES;
//#END TABLES  


/**
 * \brief The following is a list of String Code Defines.  There is one code
 * for every unique string in the product
 *
 * Each string is defined by a 32 bit code. 
 * The first byte is used to determine which table will
 * be used. The other three bytes are an index into that table.
 * The table code must be numbered sequentially from 0x00000000 
 * with 0x10000000 interval between two table codes
 *
 */
/* #BEGIN ENUMS */
enum string_mgr_code_e
{

    /****************************
     * Status Strings
     ****************************/
    STRING_STAT_NONE = STRING_STATUS_TABLE,        // OSFA DEF: ""
    STRING_STAT_READY,                             // OSFA DEF: "Ready"
    STRING_STAT_INIT,                              // OSFA DEF: "Initializing..."

    STRING_STAT_END,

    /****************************
     * Generic Strings
     ****************************/

    STRING_GEN_YES=STRING_GENERIC_TABLE,           // OSFA DEF: "Yes"
    STRING_GEN_NO,                                 // OSFA DEF: "No"
    STRING_GEN_OFF,                                // OSFA DEF: "Off"
    STRING_GEN_ON,                                 // OSFA DEF: "On"
    STRING_GEN_AUTO,                               // OSFA DEF: "Auto"
    STRING_GEN_AUTOMATIC,                          // OSFA DEF: "Automatic"
    STRING_GEN_END,

    /****************************
     * Control Panel Strings
     ****************************/
    STRING_CP_MAINMENU = STRING_CTRL_PANEL_TABLE,  // OSFA DEF: "Main menu"
    STRING_CP_ENGLISH,                             // OSFA DEF: "English"
    STRING_CP_FRENCH,                              // OSFA DEF: "French"
    STRING_CP_GERMAN,                              // OSFA DEF: "German"
    STRING_CP_CTRY_USA,                            // OSFA DEF: "USA"
    STRING_CP_CTRY_FRANCE,                         // OSFA DEF: "France"
    STRING_CP_CTRY_GERMANY,                        // OSFA DEF: "Germany"
    STRING_CP_SIZE_ANY_SIZE,                       // OSFA DEF: "Any size"
    STRING_CP_SIZE_LETTER,                         // OSFA DEF: "Letter"
    STRING_CP_SIZE_A4,                             // OSFA DEF: "A4"
    STRING_CP_SIZE_A5,                             // OSFA DEF: "A5"
    STRING_CP_SIZE_B5,                             // OSFA DEF: "B5"
    STRING_CP_SIZE_CUS_POSTCARD,                   // OSFA DEF: "Custom PC JIS"
    STRING_CP_SIZE_CUS_DBL_PC,                     // OSFA DEF: "Dbl Custom PC JIS"
    STRING_CP_SIZE_JAP_PC,                         // OSFA DEF: "Jap PC"
    STRING_CP_SIZE_DBL_JAP_PC,                     // OSFA DEF: "Dbl Jap PC"
    STRING_CP_SIZE_CUS16K_184,                     // OSFA DEF: "Custom 16K 184"
    STRING_CP_SIZE_CUS16K_195,                     // OSFA DEF: "Custom 16K 195"
    STRING_CP_SIZE_CUS_4x6,                        // OSFA DEF: "Custom 4x6"
    STRING_CP_SIZE_CUS_5x8,                        // OSFA DEF: "Custom 5x8"
    STRING_CP_SIZE_CUS_10x15,                      // OSFA DEF: "Custom 10x15"
    STRING_CP_SIZE_ENV_PERSONAL,                   // OSFA DEF: "Envelope Personal"
    STRING_CP_SIZE_16K,                            // OSFA DEF: "16K"
    STRING_CP_SIZE_ENV_10,                         // OSFA DEF: "Envelope 10"
    STRING_CP_SIZE_ENV_MONARCH,                    // OSFA DEF: "Envelope Monarch"
    STRING_CP_SIZE_ENV_C5,                         // OSFA DEF: "Envelope C5"
    STRING_CP_SIZE_ENV_DL,                         // OSFA DEF: "Envelope DL"
    STRING_CP_SIZE_ENV_B5,                         // OSFA DEF: "Envelope B5"
    STRING_CP_SIZE_LEGAL,                          // OSFA DEF: "Legal"
    STRING_CP_SIZE_EXECUTIVE,                      // OSFA DEF: "Executive"
    STRING_CP_SIZE_CUSTOM85x13,                    // OSFA DEF: "Custom 8.5x13"
    STRING_CP_TYPE_ANY_TYPE,                       // OSFA DEF: "Any Type"
    STRING_CP_TYPE_PLAIN,                          // OSFA DEF: "Plain"
    STRING_CP_TYPE_PREPRINT,                       // OSFA DEF: "Preprinted"
    STRING_CP_TYPE_LETTERHEAD,                     // OSFA DEF: "Letterhead"
    STRING_CP_TYPE_TRANSP,                         // OSFA DEF: "Transparency"
    STRING_CP_TYPE_PREPUNCH,                       // OSFA DEF: "Prepunched"
    STRING_CP_TYPE_LABEL,                          // OSFA DEF: "Labels"
    STRING_CP_ORG_100PCNT,                         // OSFA DEF: "Original Size"
    STRING_CP_FULL_PAGE_91PCNT,                    // OSFA DEF: "Full Page"
    STRING_CP_A4_2_LTR_94PCNT,                     // OSFA DEF: "A4 -> Letter"
    STRING_CP_LTR_2_A4_97PCNT,                     // OSFA DEF: "Letter -> A4"
    STRING_CP_2_PAGES_PER_SHEET,                   // OSFA DEF: "2 Copies Per Page"
    STRING_CP_4_PAGES_PER_SHEET,                   // OSFA DEF: "4 Copies Per Page"
    STRING_CP_MANUAL,                              // OSFA DEF: "Manual"
    STRING_CP_LINK_SPEED,                          // OSFA DEF: "Link Speed"
    STRING_CP_10T_FULL,                            // OSFA DEF: "10T Full"
    STRING_CP_10T_HALF,                            // OSFA DEF: "10T Half"
    STRING_CP_100TX_FULL,                          // OSFA DEF: "100TX Full"
    STRING_CP_100TX_HALF,                          // OSFA DEF: "100TX Half"
    STRING_CP_USB_FULL,                            // OSFA DEF: "Full"
    STRING_CP_USB_HIGH,                            // OSFA DEF: "High"
    STRING_CP_PRESS_OK_TO_CONTINUE,                // OSFA DEF: "Press OK to continue"
    STRING_CP_RELOAD_PAPER,                        // OSFA DEF: "Reload Paper"
    STRING_CP_PAPER_OUT,                           // OSFA DEF: "Paper Out"
    STRING_CP_PAPER_JAMMED,                        // OSFA DEF: "Paper Jammed"
    STRING_CP_AM,                                  // OSFA DEF:"AM"
    STRING_CP_PM,                                  // OSFA DEF:"PM"
    STRING_CP_END,

    /****************************
     * ReportPage Strings
     ****************************/
    STRING_RPT_TEST = STRING_REPORT_PAGE_TABLE,    // OSFA DEF: "Report"
    STRING_RPT_END,

    /****************************
     * EWS Strings
     ****************************/
    STRING_EWS_TEST = STRING_EWS_TABLE,            // OSFA DEF: "Embedded Web Server Strings"

    STRING_EWS_LANG_ENGLISH_ABBR,                  // OSFA DEF: "en"
    STRING_EWS_LANG_GERMAN_ABBR,                   // OSFA DEF: "de"
    STRING_EWS_LANG_ITALIAN_ABBR,                  // OSFA DEF: "it"
    STRING_EWS_LANG_SPANISH_ABBR,                  // OSFA DEF: "es"
    STRING_EWS_LANG_FRENCH_ABBR,                   // OSFA DEF: "fr"
    STRING_EWS_ERROR_BONJOUR_NAME_REQUIRED,        // OSFA DEF: "Please enter a valid bonjour service name"
    STRING_EWS_ERROR_DOMAIN_NAME_REQUIRED,         // OSFA DEF: "Please enter a valid domain name"
    STRING_EWS_ERROR_HOST_NAME_REQUIRED,           // OSFA DEF: "Please enter a valid host name"
    STRING_EWS_ERROR_IPV6_ADDRESS_REQUIRED,        // OSFA DEF: "Please enter a valid IPV6 address"
    STRING_EWS_ERROR_IPV6_PREFIX_REQUIRED,         // OSFA DEF: "Please enter a valid IPV6 prefix"
    STRING_EWS_ERROR_IPV4_ADDRESS_REQUIRED,        // OSFA DEF: "Please enter a valid IPV4 address"
    STRING_EWS_ERROR_NON_NEGATIVE_REQUIRED,        // OSFA DEF: "Please enter a non-negative value in the range requested"
    STRING_EWS_ERROR_NUMERIC_RANGE_REQUIRED,       // OSFA DEF: "Please enter a numeric value in the range requested"
    STRING_EWS_ERROR_NUMERIC_VALUE_REQUIRED,       // OSFA DEF: "Please enter a numeric value"
    STRING_EWS_ERROR_SNMP_COMMUNITY_NAME_REQUIRED, // OSFA DEF: "Please enter a valid SNMP Community Name"
    STRING_EWS_A4,                                 // OSFA DEF: "Letter/A4"
    STRING_EWS_A6,                                 // OSFA DEF: "A6"
    STRING_EWS_ALERT_OK_BUTTON_LABEL,              // OSFA DEF: "OK"
    STRING_EWS_ALL_QUALITIES,                      // OSFA DEF: "All Mono and Color:"
    STRING_EWS_AUTOIP_CONFIG,                      // OSFA DEF: "Enable AutoIP:"
    STRING_EWS_BLACK_DETAILS_OPTION,               // OSFA DEF: "Black Marker Details"
    STRING_EWS_BLACK_MARKER_NAV_LABEL,             // OSFA DEF: "Black Marker level:"
    STRING_EWS_BNJR_DOMAIN,                        // OSFA DEF: "Bonjour domain name:"
    STRING_EWS_BNJR_ENABLED,                       // OSFA DEF: "Bonjour enabled:"
    STRING_EWS_BNJR_NAME,                          // OSFA DEF: "Bonjour service name:"
    STRING_EWS_BONJOUR_CONFIG_HEADING,             // OSFA DEF: "Bonjour Configuration"
    STRING_EWS_BOOTP_CONFIG,                       // OSFA DEF: "Enable BOOTP:"
    STRING_EWS_CLIENT_SOFTWARE_INSTALL_WARNING,    // OSFA DEF: "This function requires special PC software to execute properly.  Please review your printer installation and operation instructions to ensure this function is properly enabled."
    STRING_EWS_COLOR_BEST_QUALITY,                 // OSFA DEF: "Color Best Quality:"
    STRING_EWS_COLOR_GOOD_QUALITY,                 // OSFA DEF: "Color Good Quality:"
    STRING_EWS_CON_TIMEOUT,                        // OSFA DEF: "Connection timeout (seconds):"
    STRING_EWS_CONFIRM_GET_COM,                    // OSFA DEF: "Confirm GET Community string:"
    STRING_EWS_CONFIRM_SET_COM,                    // OSFA DEF: "Confirm SET Community string:"
    STRING_EWS_COPIES,                             // OSFA DEF: "Copies"
    STRING_EWS_COPY_SETTINGS,                      // OSFA DEF: "Copy Settings"
    STRING_EWS_COPY_SETTINGS_NAV_LABEL,            // OSFA DEF: "View copy default settings."
    STRING_EWS_CYAN_DETAILS_OPTION,                // OSFA DEF: "Cyan Marker Details"
    STRING_EWS_CYAN_MARKER_NAV_LABEL,              // OSFA DEF: "Cyan Marker level:"
    STRING_EWS_DARKEN,                             // OSFA DEF: "Light/Dark (0-100):"
    STRING_EWS_DEFAULT_GATE,                       // OSFA DEF: "Default gateway:"
    STRING_EWS_DEV_SERIAL,                         // OSFA DEF: "Device serial number:"
    STRING_EWS_DHCP_CONFIG,                        // OSFA DEF: "Enable DHCP:"
    STRING_EWS_DHCP_LEASE,                         // OSFA DEF: "DHCP lease time (seconds):"
    STRING_EWS_DOMAIN_IPV4,                        // OSFA DEF: "Domain name:"
    STRING_EWS_EVENT_LOG,                          // OSFA DEF: "Event Log"
    STRING_EWS_EVENT_LOG_DATE_HEADING,             // OSFA DEF: "Date"
    STRING_EWS_EVENT_LOG_ID_HEADING,               // OSFA DEF: "ID"
    STRING_EWS_EVENT_LOG_DATA_HEADING,             // OSFA DEF: "Data"
    STRING_EWS_EVENT_LOG_NAV_LABEL,                // OSFA DEF: "View recent printer events."
    STRING_EWS_FIRMWARE,                           // OSFA DEF: "Firmware version:"
    STRING_EWS_FIRST_INSTALL,                      // OSFA DEF: "First install date:"
    STRING_EWS_GENERAL_SETTINGS,                   // OSFA DEF: "General Settings"
    STRING_EWS_GENERAL_SETTINGS_NAV_LABEL,         // OSFA DEF: "General printer configuration."
    STRING_EWS_GET_COM,                            // OSFA DEF: "GET Community string:"
    STRING_EWS_GLOSSY,                             // OSFA DEF: "Glossy"
    STRING_EWS_HOME,                               // OSFA DEF: "Home Page"
    STRING_EWS_HOME_ALONE,                         // OSFA DEF: "Home"
    STRING_EWS_HOST,                               // OSFA DEF: "Host name:"
    STRING_EWS_INSTALL,                            // OSFA DEF: "Install Date:"
    STRING_EWS_IPV4_ADDRESS,                       // OSFA DEF: "IPV4 address:"
    STRING_EWS_LANG_ENGLISH_OPTION,                // OSFA DEF: "English"
    STRING_EWS_LANG_FRENCH_OPTION,                 // OSFA DEF: "French"
    STRING_EWS_LANG_GERMAN_OPTION,                 // OSFA DEF: "Gernan"
    STRING_EWS_LANG_ITALIAN_OPTION,                // OSFA DEF: "Italian"
    STRING_EWS_LANG_SPANISH_OPTION,                // OSFA DEF: "Spanish"
    STRING_EWS_LANGUAGE,                           // OSFA DEF: "Language:"
    STRING_EWS_LAST_USED,                          // OSFA DEF: "Last used date:"
    STRING_EWS_LEFT_NAV_HEADING,                   // OSFA DEF: "Printer Toolbox"
    STRING_EWS_LEGAL,                              // OSFA DEF: "Legal"
    STRING_EWS_LINK_SPEED,                         // OSFA DEF: "Link speed:"
    STRING_EWS_LINK_SPEED_1000F_OPTION,            // OSFA DEF: "1000F"
    STRING_EWS_LINK_SPEED_1000H_OPTION,            // OSFA DEF: "1000H"
    STRING_EWS_LINK_SPEED_100F_OPTION,             // OSFA DEF: "100F"
    STRING_EWS_LINK_SPEED_100H_OPTION,             // OSFA DEF: "100H"
    STRING_EWS_LINK_SPEED_10F_OPTION,              // OSFA DEF: "10F"
    STRING_EWS_LINK_SPEED_10H_OPTION,              // OSFA DEF: "10H"
    STRING_EWS_LINK_SPEED_AUTO_OPTION,             // OSFA DEF: "auto"
    STRING_EWS_MAC_ADDRESS,                        // OSFA DEF: "Mac address:"
    STRING_EWS_MAGENTA_DETAILS_OPTION,             // OSFA DEF: "Magenta Marker Details"
    STRING_EWS_MAGENTA_MARKER_NAV_LABEL,           // OSFA DEF: "Magenta Marker level:"
    STRING_EWS_MAINT_PRINT_CONFIG_BUTTON_LABEL,    // OSFA DEF: "Print Configuration"
    STRING_EWS_MAINT_PRINT_DIAG_BUTTON_LABEL,      // OSFA DEF: "Print Diagnostics"
    STRING_EWS_MAINT_PRINT_QUALITY_BUTTON_LABEL,   // OSFA DEF: "Print Quality Test"
    STRING_EWS_MAINT_VIEW_MANUAL_BUTTON_LABEL,     // OSFA DEF: "View Manual"
    STRING_EWS_MAINTENANCE,                        // OSFA DEF: "Maintenance"
    STRING_EWS_MAINTENANCE_CYCLES,                 // OSFA DEF: "Maintenance Cycles:"
    STRING_EWS_MAINTENANCE_NAV_LABEL,              // OSFA DEF: "View manual and print test pages."
    STRING_EWS_MEDIA_SIZE_A4_OPTION,               // OSFA DEF: "media a4"
    STRING_EWS_MEDIA_SIZE_LEGAL_OPTION,            // OSFA DEF: "media legal"
    STRING_EWS_MEDIA_SIZE_LETTER_OPTION,           // OSFA DEF: "media letter"
    STRING_EWS_MEDIA_TYPE_GLOSSY_OPTION,           // OSFA DEF: "glossy"
    STRING_EWS_MEDIA_TYPE_PLAIN_OPTION,            // OSFA DEF: "plain"
    STRING_EWS_MEDIA_TYPE_TRANSPARENT_OPTION,      // OSFA DEF: "transparent"
    STRING_EWS_MEMORY,                             // OSFA DEF: "Internal memory (MB):"
    STRING_EWS_MODEL,                              // OSFA DEF: "Model Number:"
    STRING_EWS_MONO_BEST_QUALITY,                  // OSFA DEF: "Mono Best Quality:"
    STRING_EWS_MONO_GOOD_QUALITY,                  // OSFA DEF: "Mono Good Quality:"
    STRING_EWS_NETWORK_ADMIN_OPTION,               // OSFA DEF: "Network Settings"
    STRING_EWS_NETWORK_BONJOUR_OPTION,             // OSFA DEF: "Bonjour Configuration"
    STRING_EWS_NETWORK_CONFIG_HEADING,             // OSFA DEF: "Network status and configuration"
    STRING_EWS_NETWORK_IPV4_OPTION,                // OSFA DEF: "IPV4 Configuration"
    STRING_EWS_NETWORK_SELECT_PAGE,                // OSFA DEF: "Select a Network Configuration Page Below:"
    STRING_EWS_NETWORK_SETTINGS_NAV_LABEL,         // OSFA DEF: "View and change network settings."
    STRING_EWS_NETWORK_SNMP_OPTION,                // OSFA DEF: "SNMP Configuration"
    STRING_EWS_NET_SETTINGS,                       // OSFA DEF: "Network Settings"
    STRING_EWS_NUM_COPIES,                         // OSFA DEF: "Number of copies:"
    STRING_EWS_NUM_REFILLS,                        // OSFA DEF: "Number of refills:"
    STRING_EWS_ONLINE_MANUAL,                      // OSFA DEF: "Review the user manual online"
    STRING_EWS_ORDER_SUPPLIES,                     // OSFA DEF: "Buy supplies at:"
    STRING_EWS_OTHER,                              // OSFA DEF: "Other"
    STRING_EWS_PAGES,                              // OSFA DEF: "Pages printed:"
    STRING_EWS_PAGES_REMAINING,                    // OSFA DEF: "Pages remaining:"
    STRING_EWS_PAGES_SCANNED,                      // OSFA DEF: "Pages Scanned (for copy and scan)"
    STRING_EWS_PAGES_SIZES,                        // OSFA DEF: "Pages Output by Paper Size"
    STRING_EWS_PAGES_TYPE,                         // OSFA DEF: "Pages Output by Paper Type"
    STRING_EWS_PAPER_SIZE,                         // OSFA DEF: "Paper size:"
    STRING_EWS_PAPER_TRAY,                         // OSFA DEF: "Paper source:"
    STRING_EWS_PAPER_TYPE,                         // OSFA DEF: "Paper type:"
    STRING_EWS_PART_NUMBER,                        // OSFA DEF: "Part Number:"
    STRING_EWS_PERCENT_REMAIN,                     // OSFA DEF: "Percent remaining:"
    STRING_EWS_PLAIN,                              // OSFA DEF: "Plain"
    STRING_EWS_PRINT_CONFIG_PAGE,                  // OSFA DEF: "Print a printer configuration page"
    STRING_EWS_PRINT_DIAGNOSTIC,                   // OSFA DEF: "Start a print diagnostics test"
    STRING_EWS_PRINT_HEAD_NAV_LABEL,               // OSFA DEF: "Print head (pages printed):"
    STRING_EWS_PRINT_HEAD_OPTION,                  // OSFA DEF: "Print Head Details"
    STRING_EWS_PRINT_ONLY,                         // OSFA DEF: "Printing"
    STRING_EWS_PRINT_QUALITY_PAGE,                 // OSFA DEF: "Print a print quality test page"
    STRING_EWS_PRINT_SCAN,                         // OSFA DEF: "Printing and Scanning"
    STRING_EWS_PRINT_SETTINGS,                     // OSFA DEF: "Print Settings"
    STRING_EWS_PRINT_SETTINGS_NAV_LABEL,           // OSFA DEF: "Default settings for printing."
    STRING_EWS_PRINTS,                             // OSFA DEF: "Prints"
    STRING_EWS_PUBLIC_ACCESS,                      // OSFA DEF: "Enable public access:"
    STRING_EWS_QUALITY,                            // OSFA DEF: "Quality:"
    STRING_EWS_READ_ONLY,                          // OSFA DEF: "Read-only access:"
    STRING_EWS_READ_WRITE,                         // OSFA DEF: "Read-write access:"
    STRING_EWS_REDUCE,                             // OSFA DEF: "Reduce/enlarge % (10-400):"
    STRING_EWS_REFILL_BY,                          // OSFA DEF: "Refilled by:"
    STRING_EWS_REFRESH_BUTTON_LABEL,               // OSFA DEF: "Refresh"
    STRING_EWS_REMOVABLE,                          // OSFA DEF: "Removable storage devices:"
    STRING_EWS_SAVE_BUTTON_LABEL,                  // OSFA DEF: "Save"
    STRING_EWS_SAVE_FAILED,                        // OSFA DEF: "An error was detected saving the values from this page to the printer.  Please ensure the printer is connected and then review the values you entered on the page.  Press the OK button and try again."
    STRING_EWS_SAVE_SUCCESSFUL,                    // OSFA DEF: "The values on this page have been successfully saved to the printer.  <p>Press OK to continue.</p>"
    STRING_EWS_SCAN_HOST_NAME_LABEL,               // OSFA DEF: "Scan Host Name"
    STRING_EWS_SCAN_HOST_PENDING_LABEL,            // OSFA DEF: "Scan Pending (Y/N)?"
    STRING_EWS_SCAN_SETTINGS,                      // OSFA DEF: "Scan Settings"
    STRING_EWS_SCAN_SETTINGS_NAV_LABEL,            // OSFA DEF: "View host scan settings."
    STRING_EWS_SERIAL_NUM,                         // OSFA DEF: "Serial Number"
    STRING_EWS_SET_COM,                            // OSFA DEF: "SET Community string:"
    STRING_EWS_SETUP,                              // OSFA DEF: "Setup and Status"
    STRING_EWS_SHOW_DEFAULTS_BUTTON_LABEL,         // OSFA DEF: "Show Defaults"
    STRING_EWS_SHOW_DEFAULTS_MESSAGE,              // OSFA DEF: "Some of the values on this page have been updated to reflect factory default values.  The values will not be saved to the printer until you press the SAVE button.  Press OK to continue.  You may also press the REFRESH button to review the current values of properties in the printer."
    STRING_EWS_SNMP_ENABLE,                        // OSFA DEF: "SNMP enabled:"
    STRING_EWS_SPECIAL_DETAILS_OPTION,             // OSFA DEF: "Black 2 Ink Details"
    STRING_EWS_SPECIAL_MARKER_NAV_LABEL,           // OSFA DEF: "Black 2 ink level:"
    STRING_EWS_PRINT_STATUS,                       // OSFA DEF: "Printer status:"
    STRING_EWS_NETWORK_STATUS,                     // OSFA DEF: "Network status:"
    STRING_EWS_SUBNET_MASK,                        // OSFA DEF: "Subnet mask:"
    STRING_EWS_SUPPLIES,                           // OSFA DEF: "Supplies"
    STRING_EWS_SUPPLIES_INFO,                      // OSFA DEF: "Supplies Info"
    STRING_EWS_SUPPLIES_INFORMATION,               // OSFA DEF: "Supplies Information"
    STRING_EWS_SUPPLIES_SELECT_PAGE,               // OSFA DEF: "Select a Supplies Page Below:"
    STRING_EWS_SUPPLIES_SUMMARY,                   // OSFA DEF: "Supplies Summary"
    STRING_EWS_SUPPLIES_SUMMARY_OPTION,            // OSFA DEF: "Supplies Information"
    STRING_EWS_SUPPLIES_VIEW_STATUS_NAV_LABEL,     // OSFA DEF: "View marker and print head status."
    STRING_EWS_TOTAL,                              // OSFA DEF: "Total"
    STRING_EWS_TOTAL_PAGES,                        // OSFA DEF: "Total pages printed:"
    STRING_EWS_TOTAL_SCANS,                        // OSFA DEF: "Total Scans:"
    STRING_EWS_TRAY_AUTO_OPTION,                   // OSFA DEF: "auto select"
    STRING_EWS_TRAY_CASETTE_OPTION,                // OSFA DEF: "casette"
    STRING_EWS_TRAY_MANUAL_OPTION,                 // OSFA DEF: "manual"
    STRING_EWS_TROUBLE,                            // OSFA DEF: "Troubleshooting"
    STRING_EWS_USAGE_INFO,                         // OSFA DEF: "Usage Info"
    STRING_EWS_USAGE_INFORMATION,                  // OSFA DEF: "Usage Information"
    STRING_EWS_VIEW_PAGE_INSTRUCTIONS,             // OSFA DEF: "Click a link below to view status or to change settings."
    STRING_EWS_VIEW_USAGE_MONO_COLOR_HEADING,      // OSFA DEF: "Monochrome and Color Pages Summary"
    STRING_EWS_VIEW_USAGE_NAV_MFP_LABEL,           // OSFA DEF: "View paper and scanner usage."
    STRING_EWS_VIEW_USAGE_NAV_SFP_LABEL,           // OSFA DEF: "View paper usage."
    STRING_EWS_WINS_SERVER,                        // OSFA DEF: "WINS server:"
    STRING_EWS_YELLOW_DETAILS_OPTION,              // OSFA DEF: "Yellow Marker Details"
    STRING_EWS_YELLOW_MARKER_NAV_LABEL,            // OSFA DEF: "Yellow Marker level:"
    STRING_EWS_MEDIATYPE_GLOSSY_OPTION,            // OSFA DEF: "glossy"
    STRING_EWS_MEDIATYPE_TRANSPARENT_OPTION,       // OSFA DEF: "transparent"
    STRING_EWS_MEDIASIZE_LETTER_OPTION,            // OSFA DEF: "letter"
    STRING_EWS_MEDIASIZE_LEGAL_OPTION,             // OSFA DEF: "legal"
    STRING_EWS_MEDIASIZE_A4_OPTION,                // OSFA DEF: "a4"
    STRING_EWS_NETWORK_CONNECTED,                  // OSFA DEF: "connected"
    STRING_EWS_NETWORK_DISCONNECTED,               // OSFA DEF: "disconnected"
    STRING_EWS_NETWORK_ERROR,                      // OSFA DEF: "error"
    STRING_EWS_DOOR_OPEN,                          // OSFA DEF: "Door Open"

    /* GCPP EWS Strings */
    STRING_EWS_GOOGLE_CLOUD_PRINT,              //LH042005 DEF:"Google Cloud Print"
    STRING_EWS_GCP_REGISTRATION,                //LH042006 DEF: "Google Cloud Print Registration"
    STRING_EWS_REGISTRATION_SETTINGS,           //LH042007 DEF: "Registration Settings"
    STRING_EWS_REGISTERING_INFO,                //LH042008 DEF: "Registering Printer. Please wait..."
    STRING_EWS_REGISTER,                        //LH042009 DEF: "Register"
    STRING_EWS_UNREGISTER,                      //LH042010 DEF: "Unregister"
    STRING_EWS_REGISTERED,                      //LH042011 DEF: "Registered"
    STRING_EWS_NOT_REGISTERED,                  //LH042012 DEF: "Not Registered"
    STRING_EWS_CANCEL_REG,                      //LH042013 DEF: "Cancel Registration"
    STRING_EWS_REGISTRATION_FAILED,             //LH042014 DEF: "Registration Failed or Canceled."
    STRING_EWS_DO_NOT_REFRESH_OR_CLOSE,         //LH042015 DEF: "Do not refresh or close browser."
    STRING_EWS_UNREGISTERING,                   //LH042016 DEF: "Unregistering..."
    STRING_EWS_PRINTER_NAME,                    //LH042018 DEF: "Printer Name"
    STRING_EWS_GCP_PLEASE_GO_TO,                //LH042019 DEF: "and go to "
    STRING_EWS_GCP_USE_TOKEN,                   //LH042020 DEF: "To complete the registration, please copy the token"
    STRING_EWS_PRINT_REGISTRATION_PAGE,         //LH042021 DEF: "Print Registration Page"
    STRING_EWS_INVALID_GCP_NAME_INFO,           //LH042022 DEF: "Please use a valid Printer Name."

  STRING_EWS_STATUS,                             // LH040078 DEF: "Status"
  STRING_EWS_ENABLED,                            // LH040079 DEF: "Enabled"
  STRING_EWS_DISABLED,                           // LH040080 DEF: "Disabled"

  STRING_EWS_ADV_SETTINGS,                    // LH040128 DEF: "Advanced Settings"
  STRING_EWS_PROXY_STATUS,                    // LH040168 DEF: "Proxy Status"
  STRING_EWS_PROXY_NAME,                      // LH040169 DEF: "Proxy Name"
  STRING_EWS_PROXY_SERVER,                    // LH040170 DEF: "Proxy Server"
  STRING_EWS_PROXY_PORT,                      // LH040171 DEF: "Proxy Port"
  STRING_EWS_PROXY_AUTH_REQ,                  // LH040172 DEF: "Authorization Required"
  STRING_EWS_PROXY_USERNAME,                  // LH040173 DEF: "Username"
  STRING_EWS_PROXY_PASSWORD,                  // LH040174 DEF: "Password"
  STRING_EWS_SHOW_HIDE,                       // LH040175 DEF: "Show/Hide"
  STRING_EWS_BACK,                            // LH040176 DEF: "Back"

    STRING_EWS_END

};
/* #END ENUMS */
 
/* #BEGIN type declarations */
typedef enum string_mgr_code_e string_mgr_code_t;
typedef enum string_mgr_language_e string_mgr_language_t;
/* #END type declarations */
  
/* #BEGIN TABLE_SIZES */
#define STRING_MAX_STAT  (STRING_STAT_END - STRING_STATUS_TABLE)
#define STRING_MAX_GEN   (STRING_GEN_END - STRING_GENERIC_TABLE)
#define STRING_MAX_CP    (STRING_CP_END - STRING_CTRL_PANEL_TABLE)
#define STRING_MAX_RPT   (STRING_RPT_END - STRING_REPORT_PAGE_TABLE)
#define STRING_MAX_EWS   (STRING_EWS_END - STRING_EWS_TABLE)
/* #END TABLE_SIZES */ 
   
#define STRING_MAX_STATUS        STRING_MAX_STAT
#define STRING_MAX_GENERIC       STRING_MAX_GEN 
#define STRING_MAX_CTRL_PANEL    STRING_MAX_CP  
#define STRING_MAX_REPORT_PAGE   STRING_MAX_RPT 
#define STRING_MAX_EWS_STRINGS   STRING_MAX_EWS 


/**
 * 
 * \brief Initialize the string manager configuration layer
 *
 */
void string_mgr_config_init( void );


/**
 *
 * \brief Get the string based on the code from the config layer
 *
 * \param code A valid string_mgr_code_t enum value
 *
 * \param localization One of the string_mgr_localization_type_t types
 *
 * \param[out] string_ptr pointer to destination for the char * to be returned
 *
 * \return OK if the code is valid, otherwise FAIL
 *
 */
error_type_t string_mgr_config_get_string_from_code( string_mgr_code_t code,
        string_mgr_localization_type_t localization,
        char ** string_ptr );


/**
 * \brief Change the language that the translations are done in.  This
 * value stays resident in NVRAM, but can be reset to the factory default
 * upon a user request.
 *
 * \param language One of the string langauge ids found in string_config.h
 *
 * \return error_type_t 
 * \retval OK
 * \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if there is a problem with the
 * permanent storage device
 *
 */
error_type_t string_mgr_config_set_language( string_mgr_language_t language );


/**
 * \brief Get the current language that the translations are done in.
 *
 * \param language One of the string langauge ids found in string_config.h
 *
 * \return error_type_t 
 * \retval OK
 * \retval DATA_ACC_ERR_NULL_PTR if incoming pointer is NULL
 *
 */
error_type_t string_mgr_config_get_language( string_mgr_language_t * language );


/**
 * \brief Change the NVRAM default language.  This value is intended to
 * be copied to the current language when the end-user requests restoring the
 * language to defaults.  This value should be set to the factory default when
 * restore to factory defaults is requested (not normally available to an end 
 * user) and set to the value requested in manufacturing or by the end-user when
 * exiting the "unconfigured" mode. 
 *
 * \param language One of the string language ids found in string_config.h
 *
 * \return error_type_t 
 * \retval OK
 * \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if there is a problem with the
 * permanent storage device
 *
 */
error_type_t string_mgr_config_set_default_language( string_mgr_language_t language );


/**
 * \brief Get the NVRAM default language.
 *
 * \param language One of the string language ids found in string_config.h
 *
 * \return error_type_t 
 * \retval OK
 * \retval DATA_ACC_ERR_NULL_PTR if incoming pointer is NULL
 *
 */
error_type_t string_mgr_config_get_default_language( string_mgr_language_t * language );


#ifdef __cplusplus
}
#endif

#endif


