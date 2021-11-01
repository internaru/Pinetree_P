/*
 * ============================================================================
 * (C) Copyright 2013   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * @file oid_airprint_api.h  
 *
 * @brief Contains OID definitions for the proprietary Apple AirPrint specification, which
 * is an extension of the PWG IPP specification.  We expose these OIDs as an extension of
 * the OID namespace OID_NAMESPACE_PWG_SM.
 * 
 * NOTE: Release of this source file to SDK customers is restricted.
 */

#ifndef OID_AIRPRINT_API_H
#define OID_AIRPRINT_API_H

#include "oid_sm_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief register the proprietary Apple AirPrint OIDs with the OID backend
 */
void oid_register_module_airprint( void );

/** 
 * @brief AirPrint 'marker-colors' printer description attribute
 *  
 * value: 
 *  
 *     Array of strings representing the color of each supply in the printer. Name is
 *     'none' for supplies that have no specific color. Otherwise name is a single
 *     hex-encoded SRGB color of the form "#RRGGBB" or multiple colors of the form
 *     "#RRGGBB#RRGGBB...#RRGGBB"
 *  
 *     Derived from SNMP Printer MIB v2 (RFC 3805) prtMarkerSuppliesColorantIndex and
 *     prtMarkerColorantValue definitions.
 *  
 * index: 
 *  
 *     0-based index into an array of markers/supplies. A given index number must
 *     correspond to the the same supply for each marker-xxx attribute.  An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.14
 *     * RFC 3805 SNMP Printer MIB v2 prtMarkerSuppliesColorantIndex:
 *       http://tools.ietf.org/html/rfc3805#page-105 and prtMarkerColorantValue:
 *       http://tools.ietf.org/html/rfc3805#page-108
 *  
 */
#define OID_AIRPRINT_MARKER_COLORS          oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 0, OID_TYPE_STRING)

/** 
 * @brief AirPrint 'marker-high-levels' printer description attribute
 *  
 * value: 
 *  
 *     Integer representing the supply level that indicates a near-full confdition for a
 *     given marker. A value of 100 should be used for supplies that are consumed/emptied
 *     (e.g. toner cartridges) while a value less than 100 should be used for supplies
 *     that are filled (e.g. waste bin).
 *  
 * index: 
 *  
 *     0-based index into an array of markers/supplies. A given index number must
 *     correspond to the the same supply for each marker-xxx attribute.  An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.15 
 */
#define OID_AIRPRINT_MARKER_HIGH_LEVELS     oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 1, OID_TYPE_UINT32)

/** 
 * @brief AirPrint 'marker-levels' printer description attribute
 *  
 * value: 
 *  
 *     Integer representing the current supply level of a given marker. Values from 0 to
 *     100 indicate the corresponding percentage.  A value of -1 indicates the level is
 *     unavailable, -2 indicates unknown, and -3 indicates the level is unknown but has
 *     not yet reached capacity.
 *  
 *     Derived from SNMP Printer MIB v2 (RFC 3805) prtMarkerSuppliesLevel and
 *     prtMarkerSuppliesMaxCapacity definitions.
 *  
 * index: 
 *  
 *     0-based index into an array of markers/supplies. A given index number must
 *     correspond to the the same supply for each marker-xxx attribute.  An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.16
 *     * SNMP Printer MIB v2 (RFC 3805) prtMarkerSuppliesLevel and
 *       prtMarkerSuppliesMaxCapacity: http://tools.ietf.org/html/rfc3805#page-106
 *      
 */
#define OID_AIRPRINT_MARKER_LEVELS          oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 2, OID_TYPE_UINT32)

/** 
 * @brief AirPrint 'marker-low-levels' printer description attribute
 *  
 * value: 
 *  
 *     Integer representing the supply level that indicates a near-empty condition for a
 *     given marker. A value of 0 should be used for supplies that are filled (e.g. waste
 *     bin) while a value greater than 0 should be used for supplies that are
 *     consumed/emptied (e.g. toner cartridges).
 *  
 * index: 
 *  
 *     0-based index into an array of markers/supplies. A given index number must
 *     correspond to the the same supply for each marker-xxx attribute.  An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.17 
 */
#define OID_AIRPRINT_MARKER_LOW_LEVELS      oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 3, OID_TYPE_UINT32)

/** 
 * @brief AirPrint 'marker-names' printer description attribute
 *  
 * value: 
 *  
 *     Array of strings representing the name of each supply in the printer.
 *  
 *     Derived from SNMP Printer MIB v2 (RFC 3805) prtMarkerSuppliesDescription
 *     definition.
 *  
 * index: 
 *  
 *     0-based index into an array of markers/supplies. A given index number must
 *     correspond to the the same supply for each marker-xxx attribute.  An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.18
 *     * RFC 3805 SNMP Printer MIB v2 prtMarkerSuppliesDescription:
 *       http://tools.ietf.org/html/rfc3805#page-106
 */
#define OID_AIRPRINT_MARKER_NAMES           oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 4, OID_TYPE_STRING)

/** 
 * @brief AirPrint 'marker-types' printer description attribute
 *  
 * value: 
 *  
 *     Array of keyword strings representing the type of each supply in the printer.
 *  
 *     Keywords are derived from a subset of the values defined by SNMP
 *     Printer MIB v2 (RFC 3805) prtMarkerSuppliesTypeTC.
 *  
 * index: 
 *  
 *     0-based index into an array of markers/supplies. A given index number must
 *     correspond to the the same supply for each marker-xxx attribute.  An
 *     error code of OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.19
 *     * SNMP Printer MIB v2 (RFC 3805) prtMarkerSuppliesTypeTC:
 *       http://tools.ietf.org/html/rfc3805#page-48
 */
#define OID_AIRPRINT_MARKER_TYPES           oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 5, OID_TYPE_STRING)

/** 
 * @brief AirPrint 'pdf-versions-supported' printer description attribute 
 *  
 * value: 
 *  
 *     Array of keyword strings representing the PDF versions that are supported for
 *     printing. Must report either 'none' to indicate no PDF support or one or more
 *     keywords as defined by the AirPrint specification representing supported standards,
 *     e.g. "iso-32000-1-2008" to indicate support for ISO 32000-1:2008.
 *  
 * index: 
 *  
 *     0-based index into array of keyword strings. An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.38
 */
#define OID_AIRPRINT_PDF_VERSIONS_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 6, OID_TYPE_STRING)


/** 
 * @brief AirPrint 'print-scaling-default' printer description attribute
 *  
 * value: 
 *  
 *     Keyword string representing the default value of the 'print-scaling' job template
 *     attribute.  Must initially have the value 'auto' for compatibility with prior
 *     versions of AirPrint.
 *  
 * index: 
 *  
 *     A singleton value. An index >0 will result in error code OID_ERROR_INDEX_INVALID.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.40
 */
#define OID_AIRPRINT_PRINT_SCALING_DEFAULT oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 7, OID_TYPE_STRING)

/** 
 * @brief AirPrint 'print-scaling-supported' printer description attribute attribute 
 *  
 * value: 
 *  
 *     Array of keyword strings representing the set of possible values for the printer
 *     'print-scaling' job template attribute.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.41
 */
#define OID_AIRPRINT_PRINT_SCALING_SUPPORTED oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 8, OID_TYPE_STRING)

/** 
 * @brief AirPrint 'printer-kind' printer description attribute
 *  
 * value: 
 *  
 *     An array of keyword strings representing the kinds of printing supported by the
 *     printer.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code of
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.52
 */
#define OID_AIRPRINT_PRINTER_KIND           oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 9, OID_TYPE_STRING)


/** 
 * @brief AirPrint 'urf-supported' printer description attribute
 *  
 * value: 
 *  
 *     An array of strings representing the supported Universal Raster Format page header
 *     values.
 *  
 * index: 
 *  
 *     0-based index into array of strings. An error code of OID_ERROR_INDEX_INVALID means
 *     end of array.
 *  
 * definitions: 
 *  
 *     * Airprint specification 1.3 section 6.7.63
 */
#define OID_AIRPRINT_URF_SUPPORTED          oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 10, OID_TYPE_STRING)


/** 
 * @brief 'urf-configured' URF page header values configured 
 *        attribute
 *  
 * value: 
 *  
 *     An array of IPP keyword strings further qualifying the
 *     URF string returned to Bonjour.
 *  
 * index: 
 *  
 *     0-based index into array of IPP keyword strings. An error code
 *     OID_ERROR_INDEX_INVALID means end of array.
 *  
 * definitions: 
 *  
 */
 #define OID_AIRPRINT_PRINT_URF_CONFIGURED   oid_make(OID_NAMESPACE_PWG_SM, OID_AREA_AIRPRINT, 11, OID_TYPE_STRING)


#ifdef __cplusplus
}
#endif

#endif // OID_AIRPRINT_API_H

