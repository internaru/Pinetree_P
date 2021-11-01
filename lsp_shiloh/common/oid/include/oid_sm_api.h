/*
 * ============================================================================
 * (C) Copyright 2012   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * @file oid_sm_api.h  
 *
 * @brief Contains definitions for the core set of OIDs belonging to OID namespace
 * OID_NAMESPACE_PWG_SM.
 * 
 * These OIDs map one-to-one to the device (e.g. printer) object attributes defined by the
 * Printer Working Group (PWG) Semantic Model. These OIDs do not map to the job and
 * document object attributes defined by the PWG Semantic Model. For job and document
 * object mappings see sm_job_api.h. For vendor-specific or proprietary 3rd-party device
 * object mappings see the OID_NAMESPACE_PWG_SM extension APIs in the corresponding 'oem'
 * or 'third_party' folders.
 * 
 * OIDs in this namespace MUST conform semantically to the PWG Semantic Model defined
 * here: http://www.pwg.org/mfd/index.html (The PWG Semantic Model Schema v2 can be found
 * here: http://www.pwg.org/mfd/navigate/PwgSmRev1-181_System.html )
 * 
 * Additionally, OIDs in this namespace MUST conform syntatically to the IPP specification
 * defined by RFC 2911 and the various IPP extension draft and candidate standards located
 * here: http://www.pwg.org/ipp/
 * 
 * Protocols other than IPP that are also based on the PWG Semantic Model, such as
 * WS-Print, SNMP Printer MIB, etc. will also map semantically to the OIDs in this
 * namespace, but will also likely require a syntax translation layer.
 * 
 * For example, for a printer in an idle state, the value of OID_SM_PRINTER_STATE will be
 * the integer value '3', an enumeration defined by IPP RFC 2911 to mean "printer state
 * idle". An implementation of the IPP protocol would therefore require no syntax
 * translation. On the other hand, WS-Print would need to translate this enumeration value
 * to the keyword string 'idle'. However, in both cases the two values are semantically
 * equivalent.
 */

#ifndef OID_SM_API_H
#define OID_SM_API_H

#include "oid_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name OID area ID's for the OID_NAMESPACE_PWG_SM namespace
 *
 * Use one of these OID_AREA_SM_X macros for the oid_area_id param when building OIDs in
 * the OID_NAMESPACE_PWG_SM namespace using the oid_make macro.
 */

/** 
 * @brief Using this area ID means the OID maps to a printer attribute defined by IPP RFC 
 *        2911:  http://tools.ietf.org/html/rfc2911
 */
#define OID_AREA_SM_RFC2911         ((uint8_t)0)

/** 
 * @brief Using this area ID means the OID maps to a printer attribute defined by one of 
 *        the IPP extension draft or candidate standards located here:
 *        http://www.pwg.org/ipp/
 */
#define OID_AREA_SM_PWG5100         ((uint8_t)1)
/** 
 *  @brief Using this area ID means the OID maps to a scanner attribute defined by the PWG
 *         Semantic Model.
 */
#define OID_AREA_SM_SCAN            ((uint8_t)2)

/** 
 *  @brief Using this area ID means the object is associated with the proprietary Apple
 *         AirPrint specification, which is an extension of the PWG IPP specification.
 *         OIDs of this type must be defined in oid_airprint_api.h located in the
 *         'third_party' folder.
 */
#define OID_AREA_AIRPRINT           ((uint8_t)253)

/** 
 *  @brief Using this area ID means the object is associated with an OEM's vendor-specific
 *         extensions to the PWG Semantic Model. OIDs of this type must be defined in the
 *         OEM's oid_sm_vendor_api.h located in the 'oem' folder.
 */
#define OID_AREA_SM_VENDOR          ((uint8_t)254)

//@}


/** 
 * @brief Maximum byte size of a string of type 'charset'
 *  
 * Definition of 'charset': http://tools.ietf.org/html/rfc2911#section-4.1.7
 *  
 * http://www.iana.org/assignments/character-sets states that character set names may be 
 * up to 40 characters taken from the printable characters of US-ASCII (plus one for 
 * NULL-terminator). 
 */
#define OID_SM_CHARSET_SIZE         41

/** 
 * @brief Maximum byte size of a string of type 'naturalLanguage'
 *  
 * Definition of 'naturalLanguage': http://tools.ietf.org/html/rfc2911#section-4.1.8 
 *  
 * http://tools.ietf.org/html/rfc2911#section-4.1.8 limits the maximum length of 
 * 'naturalLanguage' values to 63 octets (plus one for NULL-terminator). 
 */
#define OID_SM_LANGUAGE_SIZE        64


/** 
 * @brief The number of octets for rangeOfInteger
 *  
 *  See RFC 2910, section 3.9.
*/
#define NUM_OCTETS_FOR_RANGE_OF_INTEGER 8

/**
 * @name Printer State
 *  
 * Use the SM_PRINTER_STATE_X macros below to interpret the value of this uint32_t. 
 *  
 * These values correspond to the value of OID_SM_PRINTER_STATE.
 *  
 * IMPORTANT:  These enum values must match the IPP enum values defined by the 
 * 'printer-state' attribute: http://tools.ietf.org/html/rfc2911#section-4.4.11
 */
typedef uint32_t sm_printer_state_t;

/**
 * @brief Idle
 */
#define SM_PRINTER_STATE_IDLE         3

/**
 * @brief Processing
 */
#define SM_PRINTER_STATE_PROCESSING   4

/**
 * @brief Stopped
 */
#define SM_PRINTER_STATE_STOPPED      5

/**
 * @brief values 1000-2000 are reserved for vendor-specific 'printer-state' defines. 
 */

//@}


/**
 * @name Printer State Reasons
 *  
 * Use the SM_PRINTER_STATE_REASON_X macros below to interpret the value of this uint32_t.
 *  
 * These flags are used internally to represent the keyword values of 
 * OID_SM_PRINTER_STATE_REASONS. 
 *  
 * IMPORTANT:  These values must map one-to-one with the IPP keywords defined in the 
 * g_sm_printer_state_reasons_map initializer list. Any modification of these values
 * requires a corresponding change to g_sm_printer_state_reasons_map. 
 */
typedef uint32_t sm_printer_state_reasons_t;

/**                                                 
 * @brief Stopped                                   
 */                                                 
#define SM_PRINTER_STATE_REASON_NONE                (1<<0)

/**                                                 
 * @brief Door Open                                 
 */                                                 
#define SM_PRINTER_STATE_REASON_DOOR_OPEN           (1<<1)

/**                                                 
 * @brief Media Jam                                 
 */                                                 
#define SM_PRINTER_STATE_REASON_MEDIA_JAM           (1<<2)

/**                                                 
 * @brief Media Empty                                 
 */                                                 
#define SM_PRINTER_STATE_REASON_MEDIA_EMPTY         (1<<3)

/**                                                 
 * @brief Media Needed
 */                                                 
#define SM_PRINTER_STATE_REASON_MEDIA_NEEDED        (1<<4)

/**
 * @brief Toner Empty
 */
#define SM_PRINTER_STATE_REASON_MARKER_SUPPLY_EMPTY (1<<5)

/**
 * @brief Toner Low
 */
#define SM_PRINTER_STATE_REASON_MARKER_SUPPLY_LOW   (1<<6)

/**                                                 
 * @brief User Intervention Req'd                                
 */                                                 
#define SM_PRINTER_STATE_REASON_OTHER               (1<<7)

//@}


/**
 * @brief Scanner State
 *  
 * These enums are used internally to represent the keyword values of 
 * OID_SM_SCANNER_STATE. 
 *  
 * IMPORTANT:  These values must map one-to-one with the PWG keywords defined in the 
 * g_sm_scanner_state_map initializer list. Any modification of these enum values requires
 * a corresponding change to g_sm_scanner_state_map. 
 */
typedef enum {
    SM_SCANNER_STATE_IDLE = 0, ///< Idle  The Scan Service is available and can start processing a new job.
    SM_SCANNER_STATE_PROCESSING, ///< Processing  The Scan Service is currently processing jobs.
    SM_SCANNER_STATE_STOPPED, ///< Stopped - No jobs can be processed and intervention is needed.
    SM_SCANNER_STATE_TESTING,  ///< Testing - The Scan Service is in testing or maintenance mode (e.g. calibrating).
    SM_SCANNER_STATE_DOWN, ///< Down - The Scan Service is unavailable for jobs.
    SM_SCANNER_STATE_UNKNOWN, ///< Unknown  The state of the Scan Service is not known.
} sm_scanner_state_t;

/**
 * @name Scanner State Reasons
 *  
 * Use the SM_SCANNER_STATE_REASON_X macros below to interpret the value of this 
 * uint32_t. 
 *  
 * These flags are used internally to represent the keyword values of 
 * OID_SM_SCANNER_STATE_REASONS. 
 *  
 * IMPORTANT:  These values must map one-to-one with the PWG keywords defined in the 
 * g_sm_scanner_state_reasons_map initializer list. Any modification of these values
 * requires a corresponding change to g_sm_scanner_state_reasons_map. 
 */
typedef uint32_t sm_scanner_state_reasons_t;

/**                                                 
 * @brief AttentionRequired
 */                                                 
#define SM_SCANNER_STATE_REASON_ATTENTION_REQUIRED  (1<<0)

/**                                                 
 * @brief Calibrating
 */                                                 
#define SM_SCANNER_STATE_REASON_CALIBRATING         (1<<1)

//@}


/**
 * @brief register the PWG Semantic Model API oids with the OID backend
 */
void oid_register_module_sm( void );


//=========================================================================================================================
// OID_AREA_SM_RFC2911
//=========================================================================================================================

/** 
 * @brief 'charset-configured' printer description attribute 
 *  
 * value: 
 *  
 *     An official IANA character set identifier string representing the charset (e.g.
 *     'utf-8') that the device has been configured to internally represent 'text' and
 *     'name' attributes such as 'printer-name' (name), 'printer-location' (text),
 *     'printer-info' (text), and 'printer-make-and-model' (text).
 *  
 *     The device may be required to convert from this charset to another charset as
 *     requested by a client via the 'attributes-charset' operation attribute (limited to
 *     the set of supported charsets advertised by the device via the 'charset-supported'
 *     printer description attribute).
 *  
 *     The character set identifier string will be no longer than OID_SM_CHARSET_SIZE
 *     bytes including NULL-terminator.
 *     
 *     When a character-set in the IANA registry has more than one name (alias), the name
 *     labeled as "(preferred MIME name)", if present, will be used.
 *      
 *     Although RFC 2046 requires that identifiers be case-insensitive US-ASCII, this OID
 *     will always provide lower case identifiers.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'charset-configured' IPP printer description attribute: 
 *       http://tools.ietf.org/html/rfc2911#section-4.4.17
 *     * 'attributes-charset' IPP operation attribute:
 *       http://tools.ietf.org/html/rfc2911#section-3.1.4.1
 *     * 'charset' datatype:  http://tools.ietf.org/html/rfc2911#section-4.1.7
 *     *  comprehensive list of official IANA character set identifiers: 
 *       http://www.iana.org/assignments/character-sets
 */
#define OID_SM_CHARSET_CONFIGURED       oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 0, OID_TYPE_STRING)

/**
 * @brief 'charset-supported' printer description attribute 
 *  
 * value: 
 *  
 *     Array of official IANA character set identifier strings representing charsets supported
 *     by the device.
 *  
 *     A character set identifier string will be no longer than OID_SM_CHARSET_SIZE bytes 
 *     including NULL-terminator. 
 *  
 *     If the device supports a charset, it means that for all attributes of type 'text'
 *     and 'name' the device must be able to accept the charset in requests and return the
 *     charset in responses, converting as needed from the internal charset advertized by
 *     the 'charset-configured' printer description attribute.
 *  
 *     The 'attributes-charset' operation attribute selected in client requests must be
 *     limited to this set of values, or the device must reject the request.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'charset-supported' IPP printer description attribute: 
 *       http://tools.ietf.org/html/rfc2911#section-4.4.18
 *     * 'attributes-charset' IPP operation attribute:
 *       http://tools.ietf.org/html/rfc2911#section-3.1.4.1
 *     * 'charset' datatype:  http://tools.ietf.org/html/rfc2911#section-4.1.7
 *     *  comprehensive list of official IANA character set identifiers: 
 *       http://www.iana.org/assignments/character-sets
 */
#define OID_SM_CHARSET_SUPPORTED        oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 1, OID_TYPE_STRING)

/** 
 *  @brief 'natural-language-configured' printer description attribute
 *  
 * value: 
 *  
 *     An official RFC 1766 language tag string representing the natural language (e.g.
 *     'en' for English) that the device has been configured to represent 'text' and
 *     'name' attributes.
 *  
 *     A language tag string will be no longer than OID_SM_LANGUAGE_SIZE bytes including
 *     NULL-terminator.
 *  
 *     Although RFC 1766 specifies language tags as case insensitive, this OID will always
 *     provide lower case identifiers.
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'generated-natural-language-supported' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.20
 *     * 'naturalLanguage' datatype: http://tools.ietf.org/html/rfc2911#section-4.1.8
 */
#define OID_SM_NATURAL_LANGUAGE_CONFIGURED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 2, OID_TYPE_STRING)

/** 
 *  @brief 'generated-natural-language-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of official RFC 1766 language tag strings representing the set of natural
 *     languages supported by the device.
 *  
 *     A language tag string will be no longer than OID_SM_LANGUAGE_SIZE bytes including
 *     NULL-terminator.
 *  
 *     Although RFC 1766 specifies language tags as case insensitive, this OID will always
 *     provide lower case identifiers.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'generated-natural-language-supported' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.20
 *     * 'naturalLanguage' datatype: http://tools.ietf.org/html/rfc2911#section-4.1.8
 */
#define OID_SM_GENERATED_NATURAL_LANGUAGE_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 3, OID_TYPE_STRING)

/** 
 * @brief 'copies-default' printer description attribute
 *  
 * value: 
 *  
 *     Integer representing default value for the 'copies' job template attribute.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'copies' IPP job template attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.5
 */
#define OID_SM_COPIES_DEFAULT           oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 4, OID_TYPE_UINT32)

/** 
 * @brief 'copies-supported' printer description attribute
 *  
 * value: 
 *  
 *     Range of integers (upper bound and lower bound, inclusive) representing the set of
 *     possible values for the 'copies' job template attribute.
 *  
 * index: 
 *  
 *     '0' for lower bound, '1' for upper bound 
 *  
 * definitions: 
 *  
 *     * 'copies' IPP job template attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.5
 *     * 'rangeOfInteger' datatype:  http://tools.ietf.org/html/rfc2911#section-4.1.13 
 *  
 */
#define OID_SM_COPIES_SUPPORTED         oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 5, OID_TYPE_BYTE_ARRAY)

/** 
 * @brief 'document-format-default' printer description attribute
 *  
 * value: 
 *  
 *     MIME-type string of type 'mimeMediaType' representing the default value of the
 *     printer 'document-format' operation attribute.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'document-format-default' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.21
 *     * 'document-format' IPP operation attribute:
 *       http://tools.ietf.org/html/rfc2911#section-3.2.1.1
 *     * 'mimeMediaType' datatype:  http://tools.ietf.org/html/rfc2911#section-4.1.9 
 */
#define OID_SM_PRINT_DOC_FORMAT_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 6, OID_TYPE_STRING)

/** 
 * @brief 'document-format-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of MIME-type strings of type 'mimeMediaType' representing the set of
 *     possible values for the 'document-format' operation attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'document-format-supported' IPP printer description attribute: 
 *       http://tools.ietf.org/html/rfc2911#section-4.4.22
 *     * 'document-format' IPP operation attribute:
 *       http://tools.ietf.org/html/rfc2911#section-3.2.1.1
 *     * 'mimeMediaType' datatype: http://tools.ietf.org/html/rfc2911#section-4.1.9 
 */
#define OID_SM_PRINT_DOC_FORMAT_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 7, OID_TYPE_STRING)

/** 
 * @brief 'orientation-requested-default' printer description attribute
 *  
 * value: 
 *  
 *     An enum (integer) representing the default value of the printer
 *     'orientation-requested' job template attribute.
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'orientation-requested' IPP enum values:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.10
 */
#define OID_SM_PRINT_ORIENTATION_REQUESTED_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 8, OID_TYPE_UINT32)

/** 
 *  @brief 'orientation-requested-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of enums (integers) representing the set of possible values for the
 *     'orientation-requested' job template attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'orientation-requested' IPP enum values:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.10
 */
#define OID_SM_PRINT_ORIENTATION_REQUESTED_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 9, OID_TYPE_UINT32)

/** 
 * @brief 'print-quality-default' printer description attribute
 *  
 * value: 
 *  
 *     An enum (integer) representing the default value of the printer 'print-quality' job
 *     template attribute.
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'print-quality' IPP enum values:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.13
 */
#define OID_SM_PRINT_QUALITY_DEFAULT    oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 10, OID_TYPE_UINT32)

/** 
 *  @brief 'print-quality-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of enums (integers) representing the set of possible values for the
 *     'print-quality' job template attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'print-quality' IPP enum values:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.13
 */
#define OID_SM_PRINT_QUALITY_SUPPORTED  oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 11, OID_TYPE_UINT32)

/** 
 * @brief 'sides-default' printer description attribute
 *  
 * value: 
 *  
 *     IPP keyword string representing the default value of the printer 'document-format'
 *     operation attribute.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'sides' IPP job template attribute definiton and keyword values:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.8
 */
#define OID_SM_PRINT_SIDES_DEFAULT      oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 12, OID_TYPE_STRING)

/** 
 * @brief 'sides-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of keyword strings representing the set of possible values for the printer
 *     'sides' job template attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'sides' IPP job template attribute definiton and keyword values:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.8
 */
#define OID_SM_PRINT_SIDES_SUPPORTED    oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 13, OID_TYPE_STRING)

/** 
 * @brief 'printer-resolution-default' printer description attribute
 *  
 * value: 
 *  
 *     Binary value of type 'resolution' representing the default value of the
 *     'printer-resolution' job template attribute.  The binary value has the following
 *     format, delivered in network-byte order (big-endian):
 *  
 * *    ------------------------------------------------------- 
 * *    |  cross-feed dir res   |  feed direction res   |units| 
 * *    ------------------------------------------------------- 
 * *    |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  | 
 * *    ------------------------------------------------------- 
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'printer-resolution' IPP job template attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.12
 *     * 'resolution' datatype:  http://tools.ietf.org/html/rfc2911#section-4.1.15 
 */
#define OID_SM_PRINT_RESOLUTION_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 14, OID_TYPE_BYTE_ARRAY)

/** 
 * @brief 'printer-resolution-supported' printer description attribute
 *  
 * value: 
 *  
 *     Indexed array of binary values of type 'resolution' representing the default value
 *     of the 'printer-resolution' job template attribute.  Each binary value has the
 *     following format, delivered in network-byte order (big-endian):
 *  
 * *    ------------------------------------------------------- 
 * *    |  cross-feed dir res   |  feed direction res   |units| 
 * *    ------------------------------------------------------- 
 * *    |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  | 
 * *    ------------------------------------------------------- 
 *  
 * index: 
 *  
 *     0-based index into array of resolution values. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'printer-resolution' IPP job template attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.12
 *     * 'resolution' datatype:  http://tools.ietf.org/html/rfc2911#section-4.1.15 
 */
#define OID_SM_PRINT_RESOLUTION_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 15, OID_TYPE_BYTE_ARRAY)

/** 
 * @brief 'media-default' printer description attribute
 *  
 * value: 
 *  
 *     Keyword string representing the default value of the printer 'media' job template
 *     attribute.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'media' job template attribute: http://tools.ietf.org/html/rfc2911#section-4.2.11
 *     * Standardized Media Size keywords: http://www.pwg.org/ipp/ PWG 5101.1-2002
 *       "Standard for Media Standardized Names"
 */
#define OID_SM_PRINT_MEDIA_DEFAULT      oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 16, OID_TYPE_STRING)

/** 
 * @brief 'media-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of keyword strings representing the set of possible values for the printer
 *     'media' job template attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'media' IPP job template attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.2.11
 *     * Standardized Media Size keywords: http://www.pwg.org/ipp/ PWG 5101.1-2002
 *       "Standard for Media Standardized Names"
 */
#define OID_SM_PRINT_MEDIA_SUPPORTED    oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 17, OID_TYPE_STRING)

/** 
 * @brief 'printer-is-accepting-jobs' printer description attribute
 *  
 * value: 
 *  
 *     A boolean (integer) indicating whether or not the printer is currently accepting
 *     jobs ('1' for true, '0' for false).
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'printer-is-accepting-jobs' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.23
 */
#define OID_SM_PRINT_ACCEPTING_JOBS     oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 18, OID_TYPE_UINT32)


/** 
 * @brief 'printer-name' printer description attribute
 *  
 * value: 
 *  
 *     User-assigned name of the system.  It is a name that is more end-user friendly than
 *     a URI.  This name may be the last part of the printer's URI or it may be unrelated.
 *     In non-US-English locales, a name may contain characters that are not allowed in a
 *     URI.
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'printer-name' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.4
 */
#define OID_SM_SYSTEM_NAME              oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 19, OID_TYPE_STRING)


/** 
 * @brief 'printer-location' printer description attribute
 *  
 * value: 
 *  
 *     User-assigned text describing the location of the system. This could include things
 *     like: "in Room 123A, second floor of building XYZ".
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'printer-location' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.5
 */
#define OID_SM_SYSTEM_LOCATION          oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 20, OID_TYPE_STRING)

/** 
 * @brief 'printer-more-info' printer description attribute
 *  
 * value: 
 *  
 *     Provides a URI to the web interface on the device.
 * 
 *     In this case the device's IPv4 address is used for the URL hostname, relative to
 *     the incoming interface (as identified by if_index via the OID 'index' param). A
 *     user might request this OID when the corresponding HTTP request uses IPv4.
 * 
 * index: 
 *  
 *     if_index (1-based) interface index. Error code OID_ERROR_INDEX_INVALID means no
 *     more interfaces.
 *  
 * definitions: 
 *  
 *     * 'printer-more-info' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.7
 */
#define OID_SM_SYSTEM_MORE_INFO         oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 21, OID_TYPE_STRING) 

/** 
 * @brief 'printer-more-info' printer description attribute
 *  
 * value: 
 *  
 *     Provides a URI to the web interface on the device.
 * 
 *     In this case the device's IPv6 address is used for the URL hostname, relative to
 *     the incoming interface (as identified by if_index via the OID 'index' param). A
 *     user might request this OID when the corresponding HTTP request uses IPv6.
 * 
 * index: 
 *  
 *     if_index (1-based) interface index -- OID_ERROR_INDEX_INVALID means no
 *     more interfaces.
 *  
 * definitions: 
 *  
 *     * 'printer-more-info' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.7
 */
#define OID_SM_SYSTEM_MORE_INFO_IPV6    oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 22, OID_TYPE_STRING) 

/** 
 * @brief 'printer-make-and-model' printer description attribute(text(127))
 *  
 * value: 
 *  
 *      A string identifying the make and model of the device (max 127 octets + null terminator)
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'printer-make-and-model' IPP printer description attribute:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.9
 */
#define OID_SM_SYSTEM_MAKE_AND_MODEL    oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 23, OID_TYPE_STRING)

/** 
 * @brief 'printer-state' printer description attribute
 *  
 * value: 
 *  
 *     An enum (integer) representing the current state of the printer.
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'printer-state' IPP enum values:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.11
 */
#define OID_SM_PRINTER_STATE            oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 24, OID_TYPE_UINT32)

/** 
 * @brief 'printer-state-reasons' printer description attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the 'printer-state' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'printer-state-reasons' IPP attribute keywords:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.12
 */
#define OID_SM_PRINTER_STATE_REASONS    oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 25, OID_TYPE_STRING)

/** 
 * @brief 'compression-supported' printer description attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings representing the set of supported compression
 *     algorithms for document data. Only applies to the document data, not to the
 *     encoding of the operation itself.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'compression-supported' IPP attribute keywords:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.32
 */
#define OID_SM_COMPRESSION_SUPPORTED    oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 26, OID_TYPE_STRING)

/** 
 * @brief 'ipp-versions-supported' printer description attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings representing the IPP protocol version(s) that this
 *     Printer supports, including major and minor versions, i.e., the version numbers for
 *     which this Printer implementation meets the conformance requirements.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'ipp-versions-supported' IPP attribute keywords:
 *       http://tools.ietf.org/html/rfc2911#section-4.4.14
 */
#define OID_SM_IPP_VERSIONS_SUPPORTED   oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_RFC2911, 27, OID_TYPE_STRING)


//=========================================================================================================================
// OID_AREA_SM_PWG5100
//=========================================================================================================================

/** 
 * @brief 'finishings-default' printer description attribute
 *  
 * value: 
 *  
 *     An enum (integer) representing the default value of the printer 'finishings'
 *     job template attribute.
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * 'finishings' enum values: http://tools.ietf.org/html/rfc2911#section-4.2.6
 *     * Extended 'finishings' enum values: PWG 5100.1 "'finishings' attribute values
 *       extension" section 2: http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_FINISHINGS_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 0, OID_TYPE_UINT32)

/** 
 *  @brief 'finishings-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of enums (integers) representing the set of possible values for the
 *     'finishings' job template attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'finishings' enum values: http://tools.ietf.org/html/rfc2911#section-4.2.6
 *     * Extended 'finishings' enum values: PWG 5100.1 "'finishings' attribute values
 *       extension" section 2: http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_FINISHINGS_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 1, OID_TYPE_UINT32)

/** 
 * @brief 'media-col-default':'media-size':'x-dimension' printer description attribute
 *  
 * value: 
 *  
 *     Integer representing the default value of the 'x-dimension' member attribute of the
 *     'media-size' member collection attribute of the 'media-col' job template collection
 *     attribute.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'x-dimension' member attribute: PWG 5100.3-2001 "Production Printing Attributes -
 *       Set1" section 3.13.8.1: http://www.pwg.org/ipp/
 *     * 'media-size' member collection attribute: PWG 5100.3-2001 "Production Printing
 *       Attributes - Set1" section 3.13.8: http://www.pwg.org/ipp/
 *     * 'media-col' job template collection attribute: PWG 5100.3-2001 "Production
 *       Printing Attributes - Set1" section 3.13: http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_MEDIA_X_DIM_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 2, OID_TYPE_UINT32)

/** 
 * @brief 'media-size-supported':'x-dimension' printer description attribute
 *  
 * value: 
 *  
 *     Array of integers representing the set of possible values for the 'x-dimension'
 *     member attribute of the 'media-size-supported' printer description collection
 *     attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'x-dimension' member of the 'media-size-supported' attribute: PWG 5100.3-2001
 *       "Production Printing Attributes - Set1" section 3.13.8.3.1:
 *       http://www.pwg.org/ipp/
 *     * 'media-size-supported' member collection attribute definition: PWG 5100.3-2001
 *       "Production Printing Attributes - Set1" section 3.13.8.3: http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_MEDIA_X_DIM_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 3, OID_TYPE_UINT32)

/** 
 * @brief 'media-col-default':'media-size':'y-dimension' printer description attribute
 *  
 * value: 
 *  
 *     Integer representing the default value of the 'y-dimension' member attribute of the
 *     'media-size' member collection attribute of the 'media-col' job template collection
 *     attribute.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'y-dimension' member attribute definition: PWG 5100.3-2001 "Production Printing
 *       Attributes - Set1" section 3.13.8.2: http://www.pwg.org/ipp/
 *     * 'media-size' member collection attribute definition: PWG 5100.3-2001 "Production
 *       Printing Attributes - Set1" section 3.13.8: http://www.pwg.org/ipp/
 *     * 'media-col' job template collection attribute definiton: PWG 5100.3-2001
 *       "Production Printing Attributes - Set1" section 3.13: http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_MEDIA_Y_DIM_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 4, OID_TYPE_UINT32)

/** 
 * @brief 'media-size-supported':'y-dimension' printer description attribute
 *  
 * value: 
 *  
 *     Array of integers representing the set of possible values for the 'y-dimension'
 *     member attribute of the 'media-size-supported' printer description collection
 *     attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *     The i'th value of this OID corresponds to the i'th value of
 *     OID_SM_PRINT_MEDIA_X_DIM_SUPPORTED, so that the user can correlate member
 *     attributes of the 'media-size-supported' collection attribute.
 *  
 * definitions: 
 *  
 *     * 'y-dimension' member of the 'media-size-supported' attribute: PWG 5100.3-2001
 *       "Production Printing Attributes - Set1" section 3.13.8.3.2:
 *       http://www.pwg.org/ipp/
 *     * 'media-size-supported' member collection attribute: PWG 5100.3-2001 "Production
 *       Printing Attributes - Set1" section 3.13.8.3: http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_MEDIA_Y_DIM_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 5, OID_TYPE_UINT32)

/** 
 * @brief 'media-col-default':'media-type' printer description attribute
 *  
 * value: 
 *  
 *     Keyword string representing the default value of the printer 'media-type' member
 *     attribute of the 'media-col' job template collection attribute.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'media-type' member attribute: PWG 5100.3-2001 "Production Printing Attributes -
 *       Set1" section 3.13.2: http://www.pwg.org/ipp/
 *     * 'media-col' job template collection attribute: PWG 5100.3-2001 "Production
 *       Printing Attributes - Set1" section 3.13: http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_MEDIA_TYPE_DEFAULT     oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 6, OID_TYPE_STRING)

/** 
 * @brief 'media-type-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of keyword strings representing the set of possible values for the printer
 *     'media-type' member attribute of the 'media-col' job template collection attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'media-type-supported' printer description attribute: PWG 5100.3-2001 "Production
 *       Printing Attributes - Set1" section 3.13.2: http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_MEDIA_TYPE_SUPPORTED   oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 7, OID_TYPE_STRING)

/** 
 * @brief 'print-color-mode-default' printer description attribute
 *  
 * value: 
 *  
 *     IPP keyword string representing the default value of the 'print-color-mode' job
 *     template attribute.
 *  
 * index: 
 *  
 *     n/a 
 *  
 * definitions: 
 *  
 *     * 'print-color-mode' job template attribute definiton and keyword values:
 *       PWG 5100.13-2012 "IPP: Job and Printer Extensions - Set 3 (JPS3)" section 5.2.3:
 *       http://www.pwg.org/ipp/
 *  
 */
#define OID_SM_PRINT_COLOR_MODE_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 8, OID_TYPE_STRING)

/** 
 * @brief 'print-color-mode-supported' printer description attribute
 *  
 * value: 
 *  
 *     Array of keyword strings representing the set of possible values for the
 *     'print-color-mode' job template attribute.
 *  
 * index: 
 *  
 *     0-based index into array. An error code OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * 'print-color-mode' job template attribute definiton and keyword values:
 *       PWG 5100.13-2012 "IPP: Job and Printer Extensions - Set 3 (JPS3)" section 5.2.3:
 *       http://www.pwg.org/ipp/
 */
#define OID_SM_PRINT_COLOR_MODE_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 9, OID_TYPE_STRING)

/**
 * @brief Fetch the printer icon paths from a table for "printer-icons"
 *  
 * value: 
 *  
 *     Path to printer icons, relative to http server root
 *  
 * index: 
 *  
 *     0-based index into table of printer icon paths. An error code of OID_ERROR_INDEX_INVALID means end of table.
 *  
 * definitions: 
 *  
 */
#define OID_SM_PRINTER_ICON  oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 10, OID_TYPE_STRING)

/** 
 * @brief IPP attributes representing the location of the Printer 
 *  
 * value: 
 *  
 *     A string representing the location of a Printer.
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     "printer-geo-location" IPP attribute: http://www.pwg.org/ipp/ PWG 5100.13-2012
 *     "IPP: Job and Printer Extensions - Set 3 (JPS3)" section 5.6.28
 *  
 */
#define OID_SM_PRINTER_GEO_LOCATION  oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 11, OID_TYPE_STRING)

/** 
 * @brief 'job-creation-attributes-supported' announcing which 
 *        job creation attributes values that can be set during
 *        Create-Job, Validate-Job, or Print-URI
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'job-creation-attributes-supported' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_JOB_CREATION_ATTR_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 12, OID_TYPE_STRING)

/** 
 * @brief 'media-col-supported' the media collections supported 
 *        attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'media-col-supported' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_MEDIA_COL_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 13, OID_TYPE_STRING)

/** 
 * @brief 'media-left-margin-supported' the media left margins 
 *        supported attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'media-left-margin-supported' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_MEDIA_LEFT_MARGIN_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 14, OID_TYPE_UINT32)

/** 
 * @brief 'media-right-margin-supported' the media right margins 
 *        supported attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'media-left-margin-supported' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_MEDIA_RIGHT_MARGIN_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 15, OID_TYPE_UINT32)

/** 
 * @brief 'media-top-margin-supported' the media top margins 
 *        supported attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'media-top-margin-supported' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_MEDIA_TOP_MARGIN_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 16, OID_TYPE_UINT32)

/** 
 * @brief 'media-bottom-margin-supported' the media bottom 
 *        margins supported attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'media-bottom-margin-supported' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_MEDIA_BOTTOM_MARGIN_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 17, OID_TYPE_UINT32)

/** 
 * @brief 'output-bin-supported' the output-bins that are 
 *        supported attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'output-bin-supported' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_OUTPUT_BIN_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 18, OID_TYPE_STRING)

/** 
 * @brief 'output-bin-default' the default output-bin attribute 
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'output-bin-default' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_OUTPUT_BIN_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 19, OID_TYPE_STRING)

/** 
 * @brief 'media-col-default' the default media collection 
 *        attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'media-col-default' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_MEDIA_COL_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 20, OID_TYPE_STRING)

/** 
 * @brief 'media-source-supported' the media sources that are 
 *        supported attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     'media-source-supported' attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_MEDIA_SOURCE_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 21, OID_TYPE_STRING)

/** 
 * @brief Fetch media parameters for 'media-col-database' from a media table 
 *  
 * value: 
 *  
 *     Media parameters (name, width, height, margins, source, type, etc.)
 *  
 * index: 
 *  
 *     0-based index into table of media parameters (the index identifies the row in the media
 *     table). An error code of OID_ERROR_INDEX_INVALID means end of table.
 *  
 * definitions: 
 *  
 */
#define OID_SM_PRINT_MEDIA_TABLE_SOURCE oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 22, OID_TYPE_STRING)
#define OID_SM_PRINT_MEDIA_TABLE_TYPE   oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 23, OID_TYPE_STRING)
#define OID_SM_PRINT_MEDIA_TABLE_DUPLEX oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 24, OID_TYPE_UINT32)

// TODO move OID_SM_AIRPRINT_ENABLED to OID_NAMESPACE_SYSTEM
/**
 * @brief Setting as to whether Airprint is enabled or not.
 *  
 * value: 
 *  
 *     1 for enabled, 0 for not enabled.
 *  
 * index: 
 *  
 *     0-based index into array.
 *  
 * definitions: 
 *  
 */
#define OID_SM_AIRPRINT_ENABLED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_PWG5100, 25, OID_TYPE_UINT32)


//=========================================================================================================================
// OID_AREA_SM_SCAN
//=========================================================================================================================

/**
 * @brief Scanner state
 *  
 * value: 
 *  
 *     A keyword (string) representing the current state of the scanner.
 *  
 * * Keywords: 
 * * Idle  The Scan Service is available and can start processing a new job.
 * * Processing  The Scan Service is currently processing jobs.
 * * Stopped - No jobs can be processed and intervention is needed.
 * * Testing - The Scan Service is in testing or maintenance mode (e.g. calibrating).
 * * Down - The Scan Service is unavailable for jobs.
 * * Unknown  The state of the Scan Service is not known.
 *  
 * index: 
 *  
 *     n/a
 *  
 * definitions: 
 *  
 *     * Network Scan Service Semantic Model and Service Interface v1.0 [PWG 5108.02-2009]
 *     section 7.1.6.10:
 *     ftp://ftp.pwg.org/pub/pwg/candidates/cs-sm20-scan10-20090410-5108.02.pdf
 * 
 * The following figure shows the normal scanner state transitions (PWG 5108.02-2009):
 *  
 * *                    -------------------------------------- 
 * *                    |                                    |
 * *                    v                                    v
 * *  Online phase:   Idle <-------> Processing <-------> Stopped 
 * *                    ^                |                   |
 * *                    |                |                   |
 * *                    |     -----------+-------------------+ 
 * *                    |   /
 * *                    v v  
 * *  Offline phase:  Down <-------> Testing 
 * *                    ^ 
 * *                    | 
 * *                    | 
 * *  Initial phase: Unknown 
 */
#define OID_SM_SCANNER_STATE oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_SCAN, 1, OID_TYPE_STRING)

/** 
 * @brief Scanner state reasons
 *  
 * value: 
 *  
 *     An array of one or more IPP keywords (strings) further qualifying the "scanner
 *     state" attribute.
 *  
 *     Keywords: AttentionRequired, Calibrating, ConnectingToDevice, CoverClosed,
 *     CoverOpen, Deactivated, DoorOpen, HoldNewJobs, InputCannotFeedSizeSelected,
 *     InputManualInputRequest, InputTrayEmpty, InputTrayMissing,
 *     InputTrayPositionFailure, InterpreterResourceUnavailable, InterlockClosed,
 *     InterlockOpen, InternalStorageFull, LampError, LampWarming, MediaJam, MediaLow,
 *     MediaNeeded, MovingToPaused, MultipleFeedError, OutputAreaAlmostFull,
 *     OutputAreaFull, OutputTrayMissing, Paused, Shutdown, Spool-areaFull, Stopping,
 *     StoppedPartly, TimedOut, SubunitAdded, SubunitAlmostEmpty, SubunitAlmostFull,
 *     SubunitAtLimit, SubunitClosed, SubunitEmpty, SubunitFull, SubunitLifeAlmostOver,
 *     SubunitLifeOver, SubunitMemoryExhausted, SubunitMissing, SubunitMotorFailure,
 *     SubunitNearLimit, SubunitOffline, SubunitOpened, SubunitOverTemperature,
 *     SubunitPowerSaver, SubunitRecoverableFailure, SubunitRecoverableStorageError,
 *     SubunitRemoved, SubunitResourceAdded, SubunitResourceRemoved,
 *     SubunitThermistorFailure, SubunitTimingFailure, SubunitTurnedOff, SubunitTurnedOn,
 *     SubunitUnderTemperature, SubunitUnrecoverableFailure,
 *     SubunitUnrecoverableStorageError, SubunitWarmingUp, AlertRemovalOfBinaryChangeEntry
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Network Scan Service Semantic Model and Service Interface v1.0 [PWG 5108.02-2009]
 *     section 7.1.6.11:
 *     ftp://ftp.pwg.org/pub/pwg/candidates/cs-sm20-scan10-20090410-5108.02.pdf
 */
#define OID_SM_SCANNER_STATE_REASONS oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_SM_SCAN, 2, OID_TYPE_STRING)

#ifdef __cplusplus
}
#endif

#endif // OID_SM_API_H

