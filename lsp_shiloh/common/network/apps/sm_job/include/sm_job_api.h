/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * @file sm_job_api.h
 * 
 * @brief Common API for the PWG Semantic Model Job layer
 *
 * This API exposes the Job and Document objects defined by the Printer Working Group 
 * (PWG) Semantic Model. Specifically, the API leverages concepts from the following PWG 
 * standards documents: 
 *  
 * - MFD Model and Common Semantics v1.0 [PWG 5108.1-2011] 
 * - Network Scan Service Semantic Model and Service Interface v1.0 [PWG 5108.02-2009]
 * - FaxOut Service Semantic Model and Service Interface v1.0 [PWG 5108.05-2011] 
 * - FaxIn Service Semantic Model and Service Interface [November 30, 2011 - Interim 
 *   Draft]
 *  
 * The PWG Semantic Model defines the common and essential capabilities, configuration, 
 * operations, and states of Multifunction Device (MFD) services such as print, scan, and
 * fax. Each of these services is modeled in terms of three distinct objects: the Service 
 * object, the Job object, and the Document object. A Service object contains zero or more 
 * Job objects, which contains zero or more Document objects 
 *  
 * The goal of the PWG Semantic Model is to expose a consistent view of an MFD so as to 
 * maximize interoperability between diverse clients and devices. With this goal in mind, 
 * many standard network-oriented MFD protocols have been designed around the PWG 
 * Semantic Model, including the SNMP Printer MIB, IPP, AirPrint, WS-Print, WS-Scan, UPnP 
 * PrintBasic & PrintEnhanced DCPs, Google Cloud Print (GCP), etc. Broad acceptance by 
 * industry standard protocols together with ongoing support from the Printer Working 
 * Group make the Semantic Model a natural choice for comprehensive and interoperable 
 * modeling of multifunction devices. 
 *  
 * The goal of the Semantic Model Job API defined by this file is to deliver a concrete 
 * interface conforming to the abstract semantics defined by the PWG Semantic Model, 
 * allowing overlying implementations of the standard MFD protocols based on the Model to 
 * be more efficiently and consistently integrated into the system. Additionally, the API 
 * presents an abstract representation of MFD services to other internal system 
 * components, such as the Kinoma extension, encouraging code reuse and greatly 
 * simplifying the task of porting these system components to other platforms that provide
 * this API. 
 *  
 * The API adopts the maintenance strategy of initially exposing only the subset of the 
 * PWG Semantic Model required to satisfy immediate and/or high priority needs of existing
 * API users, built on a comprehensive framework designed to accommodate future expansion 
 * of the Model as need arises. To maintain backward compatibility with earlier versions 
 * of this API, future modifications should not modify data types of, rename, or remove 
 * pre-existing items. It is not necessary to preserve ordering of structures, enums, etc.
 * as the goal is to maintain compile-time backward compatibility not binary 
 * compatibility. 
 *  
 * The API explicitly models only standard attributes and operations defined by the PWG
 * Semantic Model. However, it does provide several methods for integrating OEM-specific 
 * vendor extensions without compromising interoperability with the standard attributes. 
 *  
 * Non-standard vendor-specific attributes may be added at various insertion points within
 * the model hierarchy as indicated by the 'vnd_extensions' components present in various 
 * structures defined below. The API treats the 'vnd_extensions' components as opaque 
 * values that function as a pass-through mechanism for overlying and underlying 
 * vendor-specific logic. Enumerated values of pre-defined standard attributes may also be 
 * extended by first instructing the device to ignore the standard attribute value 
 * definitions via the 'omitted' bitmask component of the Job and Document Ticket 
 * structures. Vendor specific values can then be passed via the 'vnd_extensions' 
 * components in the respective structures. 
 *  
 * The following diagram illustrates the position of SM Job API in the system relative to 
 * other system components, including the example clients IPP, WS-Print, and SNMP:
 *  
 * *                            -----    ----------    ------
 * *                           | IPP |  | WS-Print |  | SNMP | 
 * *                            -----    ----------    ------ 
 * *                               \         |        /
 * *                                -------------------- 
 * *                               |  ** SM Job API **  | 
 * *                                -------------------- 
 * *                                         |
 * *                                 ------------------ 
 * *                                | I/O Device Pipes | 
 * *                                 ------------------ 
 * *                                         |
 * *                                --------------------
 * *                               | Connection Manager | 
 * *                                -------------------- 
 * *                                         |
 * *                                    ----------- 
 * *                                   |  Parsers  | 
 * *                                    ----------- 
 * *                                         |
 * *                                  --------------- 
 * *                                 | Print Job API | 
 * *                                  --------------- 
 * *                                         |
 * *                               ---------------------
 * *                              | System HW Resources | 
 * *                               --------------------- 
 */

#ifndef SMJOB_API_H
#define SMJOB_API_H

#include <stdint.h>             /* for uint32_t, ... */
#include <stdbool.h>            /* for bool */
#include <string.h>             /* for memset() */
#include "lassert.h"            /* for ASSERT() & XASSERT() */
#include "error_types.h"        /* for error_type_t */
#include "paper_size_config.h"  /* for mediasize_t */
#include "paper_types_config.h" /* for mediatype_t */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
typedef error_type_t smjob_rcode_t;

#define SMJOB_OK                            OK
#define SMJOB_FAIL                          FAIL
#define SM_BAD_INDEX                        -11 ///< index out of range
#define SMJOB_IO_TIMEOUT                    -12 ///< I/O operation timed out
#define SMJOB_IO_ABORTED                    -13 ///< I/O operation was aborted prior to EOF
#define SMJOB_WOULDBLOCK                    -14 ///< operation would have blocked but requested non-blocking

//@}


/**
 * @brief Maximum length of names including NULL terminator.
 *  
 * Maximum number of 8-bit characters per name (e.g. job name, doc name, user name, etc.) 
 */
#define SMJOB_MAX_NAME_LEN          256

/**
 * @brief Maximum number of job state reasons associated with a job state.
 *  
 * Limits the number of smjob_state_reason_t values that can be reported along with a 
 * given smjob_state_t value. 
 */
#define SMJOB_MAX_STATE_REASONS     4

/**
 * @brief Invalid Job ID.
 */
#define SMJOB_INVALID_JOB_ID        0

/**
 * @brief Invalid Document ID.
 */
#define SMJOB_INVALID_DOC_ID        0


/**
 * @name Attribute Mask management definitions. 
 *  
 * Attribute IDs and bitmasks provide an efficient mechanism for representing multiple 
 * selections of attributes as an aggregate data type that can, for example, be passed as 
 * a single parameter in API functions. 
 */

/**
 * @brief Maximum number of Attribute ID groups. 
 *  
 * Each attribute group supports up to 32 attribute IDs. Attributes from the same group 
 * can be bitwise OR'ed together before being passed to the attribute ID management 
 * macros.
 */
#define SMJOB_NUM_ATTRIBUTE_GROUPS          2

/**
 * @brief Initialize the Attribute ID bitmask. 
 *  
 * The user should call this macro after allocating an Attribute ID bitmask of type 
 * smjob_attribute_mask_t, to initialize all bit values to 0.  The macro must be called
 * before applying any other SMJOB_ATTRIBUTE_MASK_XX macros to the Attribute ID bitmask.
 *  
 * @param mask - User-allocated bitmask of type smjob_attribute_mask_t (NOT a pointer). 
 *  
 * @retval none 
 */
#define SMJOB_ATTRIBUTE_MASK_INIT(mask) \
    memset(&mask, 0, sizeof(mask));

/**
 * @brief Add attribute IDs to an attribute bitmask.
 *  
 * After allocating an attribute bitmask of type smjob_attribute_mask_t and initializing
 * the bitmask via the SMJOB_ATTRIBUTE_MASK_INIT() macro, the user can use this macro to
 * set the bit(s) corresponding to one or more attribute IDs. Attribute IDs within the
 * same uint32_t group may be bitwise OR'ed together prior to calling the macro.
 * Attribute IDs from different groups must be added via separate calls to the macro.
 *  
 * @param mask - User-allocated bitmask of type smjob_attribute_mask_t. The bitmask must
 *             have been initialized via SMJOB_ATTRIBUTE_MASK_INIT().
 *  
 * @param attribute_ids - Bitwise OR of one or more uint32_t SMJOB_ATTRIBUTE_ID_XX 
 *                      values from same attribute group.
 *  
 * @param group - Group number of the attribute IDs, origin 0. 
 *  
 * @retval none 
 */
#define SMJOB_ATTRIBUTE_MASK_ADD_IDS(mask, attribute_ids, group) \
    XASSERT(group < SMJOB_NUM_ATTRIBUTE_GROUPS, group); \
    (mask.mask[group] |= attributes)
  
//@}
  

/**
 * @brief Aggregate Attribute ID mask. 
 *  
 * A contiguous set of uint32_t values, each representing a bitmask of the attribute IDs 
 * for a corresponding attribute group. The combined bitmasks represent an aggregate 
 * bitmask of all attribute IDs defined by the API. 
 */
typedef struct
{ 
    uint32_t mask[SMJOB_NUM_ATTRIBUTE_GROUPS];
} smjob_attribute_mask_t;


/**
 * @name Attribute IDs Group 0
 *  
 * The following attribute IDs may be OR'ed together and passed as a uint32_t bitmap group
 * to the attribute bitmask management macros. Each Attribute ID represents a distinct bit
 * in a uint32_t bitmap, useful for representing multiple attribute selections. 
 */
#define SMJOB_ATTRIBUTE_ID_JOB_NAME                 0x00000001
#define SMJOB_ATTRIBUTE_ID_ORIGINATING_USER_NAME    0x00000002
#define SMJOB_ATTRIBUTE_ID_JOB_PRIORITY             0x00000004
#define SMJOB_ATTRIBUTE_ID_MEDIA_SIZE               0x00000008
#define SMJOB_ATTRIBUTE_ID_MEDIA_TYPE               0x00000010
#define SMJOB_ATTRIBUTE_ID_ORIENTATION              0x00000020
#define SMJOB_ATTRIBUTE_ID_QUALITY                  0x00000040
#define SMJOB_ATTRIBUTE_ID_SIDES                    0x00000080
#define SMJOB_ATTRIBUTE_ID_RESOLUTION               0x00000100
#define SMJOB_ATTRIBUTE_ID_COPIES                   0x00000200
#define SMJOB_ATTRIBUTE_ID_COLLATE                  0x00000400
#define SMJOB_ATTRIBUTE_ID_DOC_NAME                 0x00000800
#define SMJOB_ATTRIBUTE_ID_COMPRESSION              0x00001000
#define SMJOB_ATTRIBUTE_ID_FORMAT                   0x00002000
#define SMJOB_ATTRIBUTE_ID_INPUT_SOURCE             0x00004000
//@}


/**
 * @brief Job type.
 *  
 * Corresponds to the Service object type defined in "MFD Model and Common Semantics".
 * Affects how certain objects in this API are interpreted, e.g. the "type" union in
 * smjob_doc_processing_t.
 */
typedef enum {
    SMJOB_TYPE_UNSPECIFIED = 0,
    SMJOB_TYPE_PRINT,
    SMJOB_TYPE_SCAN,
    SMJOB_TYPE_FAXIN,
    SMJOB_TYPE_FAXOUT,
} smjob_type_t;


/**
 * @name MIME types for document compression. 
 *  
 * Use the SMJOB_COMPRESSION_X defines to interpret the value of this uint32_t. 
 *  
 * IMPORTANT:  These enum values must map one-to-one with the strings defined by the
 * g_smjob_compression_map initializer list.
 *  
 * These enum values must correspond to the keywords defined by the 
 * 'compression-supported' IPP printer description attribute: 
 * http://tools.ietf.org/html/rfc2911#section-4.4.32 
 *  
 * Document compression is applied independent of any compression inherent in the document
 * format itself (see smjob_format_t). 
 *  
 * The device may support only a subset of these values. Query the corresponding 
 * smjob_get_supported_XX() routine to determine the set of values supported by the 
 * device. 
 */
typedef char smjob_compression_t;

/**
 * @brief Represents an unknown or unspecified document compression.
 */
#define SMJOB_COMPRESSION_UNKNOWN                   0

/**
 * @brief The document data is not compressed.
 */
#define SMJOB_COMPRESSION_NONE                      "none"

/**
 * @brief Represents the application/gzip or application/x-gzip MIME type.
 */
#define SMJOB_COMPRESSION_APPLICATION_GZIP          "gzip"

/**
 * @brief Represents the maximum string length of the string 
 *        describing the compression.
 */
#define COMPRESSION_STR_LEN 16

/**
 * @brief values 1000-1999 are reserved for vendor-specific formats
 */

//@}


/**
 * @name MIME types for document format. 
 *  
 * Use the SMJOB_FORMAT_X defines to interpret the value of this uint32_t.  
 *  
 * IMPORTANT:  These enum values must map one-to-one with the strings defined by the
 * g_smjob_format_map initializer list.
 *  
 * The device may support only a subset of these values. Query the corresponding 
 * smjob_get_supported_XX() routine to determine the set of values supported by the 
 * device. 
 */
typedef char smjob_format_t;

/**
 * @brief Represents an unknown or unspecified document format.
 */
#define SMJOB_FORMAT_UNKNOWN                        0

/**
 * @brief Represents the application/octet-stream MIME type.
 *  
 * This MIME type implies that the document format is to be auto-sensed upon 
 * examination of the PDL document data itself. 
 */
#define SMJOB_FORMAT_APPLICATION_OCTET_STREAM       "application/octet-stream"

/**
 * @brief Represents the text/plain MIME type.
 */
#define SMJOB_FORMAT_TEXT_PLAIN                     "text/plain"

/**
 * @brief Represents the image/jpeg MIME type.
 */
#define SMJOB_FORMAT_IMAGE_JPEG                     "image/jpeg"

/**
 * @brief Represents the application/pdf MIME type.
 */
#define SMJOB_FORMAT_PDF                            "application/pdf"

/**
 * @brief Represents the application/postscript MIME type.
 */
#define SMJOB_FORMAT_APPLICATION_POSTSCRIPT         "application/postscript"

/**
 * @brief Represents the application/vnd.hp-PCL MIME type.
 */
#define SMJOB_FORMAT_APPLICATION_PCL                "application/vnd.hp-PCL"

/**
 * @brief Represents the application/PCLm MIME type.
 */
#define SMJOB_FORMAT_APPLICATION_PCLM               "application/PCLm"

/**
 * @brief Represents the image/urf MIME type.
 */
#define SMJOB_FORMAT_IMAGE_URF                      "image/urf"

/**
 * @brief Number of MIME type entries defined above -- NOT A MIME TYPE
 */
#define SMJOB_FORMAT_NUM_ENTRIES                    9

/**
 * @brief Represents the maximum string length of the string 
 *        describing the format.
 */
#define FORMAT_STR_LEN 64

/**
 * @brief values 1000-1999 are reserved for vendor-specific formats
 */

//@}


/**
 * @name Finishings 
 *  
 * Use the SMJOB_FINISHING_X defines to interpret the value of this uint32_t. 
 *  
 * IMPORTANT: These enum values must match the IPP enum values defined by the 'finishing' 
 * job template attribute: http://tools.ietf.org/html/rfc2911#section-4.2.6 
 *  
 * The device may support only a subset of these values. Query the corresponding 
 * smjob_get_supported_XX() routine to determine the set of values supported by the 
 * device. 
 */
typedef uint32_t smjob_finishing_t;

/**
 * @brief Perform no finishing.
 */
#define SMJOB_FINISHING_NONE                        3

/**
 * @brief Bind the document(s) with one or more staples.
 */
#define SMJOB_FINISHING_STAPLE                      4

/**
 * @brief values 1000-1999 are reserved for vendor-specific 'finishing' defines
 */

//@}


/**
 * @name Orientation
 *  
 * Use the SMJOB_ORIENTATION_X defines to interpret the value of this uint32_t. 
 *  
 * IMPORTANT: These enum values must match the IPP enum values defined by the 
 * 'orientation-requested' job template attribute: 
 * http://tools.ietf.org/html/rfc2911#section-4.2.10 
 *  
 * The device may support only a subset of these values. Query the corresponding 
 * smjob_get_supported_XX() routine to determine the set of values supported by the 
 * device. 
 */
typedef uint32_t smjob_orientation_t;

/**
 * @brief Portrait
 */
#define SMJOB_ORIENTATION_PORTRAIT                  3

/**                                                 
 * @brief Landscape                                 
 */                                                 
#define SMJOB_ORIENTATION_LANDSCAPE                 4

/**                                                 
 * @brief Reverse Landscape                         
 */                                                 
#define SMJOB_ORIENTATION_REVERSE_LANDSCAPE         5

/**                                                 
 * @brief Reverse Portrait                          
 */                                                 
#define SMJOB_ORIENTATION_REVERSE_PORTRAIT          6

/**
 * @brief values 1000-1999 are reserved for vendor-specific 'orientation-requested' 
 *        defines
 */

//@}


/**
 * @name Hardcopy sides
 *  
 * Use the SMJOB_SIDES_X defines to interpret the value of this uint32_t. 
 *  
 * IMPORTANT:  These enum values must map one-to-one with the IPP keyword strings defined 
 * in the g_smjob_sides_map initializer list 
 *  
 * These enum values must correspond to the 'sides' IPP job template attribute definition 
 * and keyword values: http://tools.ietf.org/html/rfc2911#section-4.2.8 
 *  
 * The device may support only a subset of these values. Query the corresponding 
 * smjob_get_supported_XX() routine to determine the set of values supported by the 
 * device. 
 */
typedef char smjob_sides_t;

/**
 * @brief placeholder for unknown/invalid value
 */
#define SMJOB_SIDES_UNKNOWN                         "unknown"

/**                                                 
 * @brief Portrait                                  
 */                                                 
#define SMJOB_SIDES_ONE_SIDED                       "one-sided"

/**                                                 
 * @brief Landscape                                 
 */                                                 
#define SMJOB_SIDES_TWO_SIDED_LONG_EDGE             "two-sided-long-edge"

/**                                                 
 * @brief Reverse Landscape                         
 */                                                 
#define SMJOB_SIDES_TWO_SIDED_SHORT_EDGE            "two-sided-short-edge"

/**
 * @brief Represents the maximum string length of the string 
 *        describing the sides.
 */
#define SIDES_STR_LEN 32


/**
 * @brief values 1000-1999 are reserved for vendor-specific 'sides' definitions
 */

//@}


/**
 * @name Color mode
 *  
 * The device may support only a subset of these values. Query the corresponding 
 * smjob_get_supported_XX() routine to determine the set of values supported by the 
 * device. 
 */
typedef char smjob_color_mode_t;

/**
 * @brief placeholder for unknown/invalid value
 */
#define SMJOB_COLOR_MODE_UNKNOWN                    "unknown"

/**                                                 
 * @brief Monochrome                                
 */                                                 
#define SMJOB_COLOR_MODE_MONO                       "monochrome"

/**                                                 
 * @brief Color                                     
 */                                                 
#define SMJOB_COLOR_MODE_COLOR                      "color"

/**                                                 
 * @brief Auto-detect based on document             
 */                                                 
#define SMJOB_COLOR_MODE_AUTO                       "auto"

/**
 * @brief Represents the maximum string length of the string 
 *        describing the color mode.
 */
#define COLOR_MODE_STR_LEN 16

/**
 * @brief values 1000-1999 are reserved for vendor-specific 'color-mode' definitions
 */

//@}


/**
 * @name print content optimize
 *  
 * The device may support only a subset of these values. Query the corresponding 
 * smjob_get_supported_XX() routine to determine the set of values supported by the 
 * device. 
 */
typedef char smjob_print_content_optimize_t;

/**                                                 
 * @brief auto
 */                                                 
#define SMJOB_PRINT_CONTENT_OPTIMIZE_AUTO              "auto"

/**                                                 
 * @brief graphics
 */                                                 
#define SMJOB_PRINT_CONTENT_OPTIMIZE_GRAPHICS          "graphics"

/**                                                 
 * @brief photo
 */                                                 
#define SMJOB_PRINT_CONTENT_OPTIMIZE_PHOTO             "photo"

/**                                                 
 * @brief text
 */                                                 
#define SMJOB_PRINT_CONTENT_OPTIMIZE_TEXT              "text"

/**                                                 
 * @brief text-and-graphics
 */                                                 
#define SMJOB_PRINT_CONTENT_OPTIMIZE_TEXT_AND_GRAPHICS "text-and-graphics"

/**
 * @brief Represents the maximum string length of the string 
 *        describing the print content optimize.
 */
#define PRINT_CONTENT_OPTIMIZE_STR_LEN 32

//@}


/**
 * @name Hardcopy image quality 
 *  
 * Use the SMJOB_QUALITY_X defines to interpret the value of this uint32_t. 
 *  
 * IMPORTANT: These enum values must match the IPP enum values defined by the 
 * 'print-quality' job template attribute: 
 * http://tools.ietf.org/html/rfc2911#section-4.2.13 
 *  
 * The device may support only a subset of these values. Query the corresponding 
 * smjob_get_supported_XX() routine to determine the set of values supported by the 
 * device. 
 */
typedef uint32_t smjob_quality_t;

/**
 * @brief Portrait
 */
#define SMJOB_QUALITY_DRAFT                         3

/**                                                 
 * @brief Landscape                                 
 */                                                 
#define SMJOB_QUALITY_NORMAL                        4

/**                                                 
 * @brief Reverse Landscape                         
 */                                                 
#define SMJOB_QUALITY_HIGH                          5

/**
 * @brief values 1000-1999 are reserved for vendor-specific 'print-quality' defines
 */

//@}


/**
 * @brief Source of the document to be scanned.
 */
typedef enum
{
    SMJOB_INPUT_SOURCE_ADF,
    SMJOB_INPUT_SOURCE_ADF_DUPLEX,
    SMJOB_INPUT_SOURCE_PLATEN,
} smjob_input_source_t;


/**
 * @name Complex attribute definitions.
 *  
 *  Some attributes are best represented in C as struct types. The structs for all such
 *  attributes in the API are defined below.
 */

/**
 * @brief Resolution Units
 * 
 * IMPORTANT: These enum values must match the IPP enum values defined by the 'resolution'
 * datatype:  http://tools.ietf.org/html/rfc2911#section-4.1.15
 */
typedef enum
{
    SMJOB_RES_UNIT_DOTS_PER_INCH  = 3,
    SMJOB_RES_UNIT_DOTS_PER_CM    = 4
} smjob_resolution_units_t;

/**
 * @brief Resolution
 */
typedef struct
{
    uint32_t                    xfeed_dir; // cross-feed direction (width)
    uint32_t                    feed_dir; // feed direction (height)
    smjob_resolution_units_t    units;
} smjob_resolution_t;

/**
 * @name Hardcopy media description 
 *  
 * Describes print media parameters, for those times that AirPrint does not
 * provide a media name. Need a place to store the incoming info (even if
 * someone eventually tries to map it to a known mediasize_t).
 *  
 * IMPORTANT: Dimension values match IPP, which are in hundredths of a millimeter
 */
typedef struct 
{
    uint32_t  x_dimension;
    uint32_t  y_dimension;
    uint32_t  bottom_margin;
    uint32_t  left_margin;
    uint32_t  right_margin;
    uint32_t  top_margin;

    // AirPrint may specify these with the job attributes (media-col), but 
    // usually they just send the dimensions above. Not using our enums here,
    // just storing the raw string inputs.
    char      source[SMJOB_MAX_NAME_LEN];
    char      type[SMJOB_MAX_NAME_LEN];
    char      output_bin[SMJOB_MAX_NAME_LEN];
} smjob_media_col_t;

//@}


/**
 * @name Job state 
 *  
 * Use the SMJOB_STATE_X defines to interpret the value of this uint32_t. 
 *  
 * IMPORTANT: These enum values must match the IPP enum values defined by the 'job-state'
 * job description attribute: http://tools.ietf.org/html/rfc2911#section-4.3.7
 * 
 * The following figure shows the normal job state transitions:
 * 
 * *                                                      +----> canceled 
 * *                                                     /                
 * *       +----> pending --------> processing ---------+------> completed
 * *       |         ^                   ^               \                
 * *   --->+         |                   |                +----> aborted  
 * *       |         v                   v               /                
 * *       +----> pending-held    processing-stopped ---+                 
 */
typedef uint32_t smjob_state_t;

/**
 * @brief Pending
 */
#define SMJOB_STATE_PENDING                             3

/**                                                     
 * @brief Pending Held                                  
 */                                                     
#define SMJOB_STATE_PENDING_HELD                        4

/**                                                     
 * @brief Processing                                    
 */                                                     
#define SMJOB_STATE_PROCESSING                          5

/**                                                     
 * @brief Processing stopped                            
 */                                                     
#define SMJOB_STATE_PROCESSING_STOPPED                  6

/**                                                     
 * @brief Canceled                                      
 */                                                     
#define SMJOB_STATE_CANCELED                            7

/**                                                     
 * @brief Aborted                                       
 */                                                     
#define SMJOB_STATE_ABORTED                             8

/**                                                     
 * @brief Completed                                     
 */                                                     
#define SMJOB_STATE_COMPLETED                           9


/**
 * @brief values 1000-1999 are reserved for vendor-specific 'job-state' defines
 */

//@}

/**
 * @brief Job State Reason 
 *  
 * IMPORTANT: These enum values must map one-to-one with the IPP keyword strings defined 
 * in the g_smjob_state_reason_map initializer list 
 */
typedef enum
{
    SMJOB_STATE_REASON_UNKNOWN = 0,
    SMJOB_STATE_REASON_NONE,                            // WSD, IPP
    SMJOB_STATE_REASON_JOB_INCOMING,                    // WSD, IPP
    SMJOB_STATE_REASON_JOB_DATA_INSUFFICIENT,           // IPP, 
    SMJOB_STATE_REASON_DOC_ACCESS_ERROR,                // WSD, IPP
    SMJOB_STATE_REASON_DOC_TRANSFER_ERROR,              // WSD, IPP submission-interrupted
    SMJOB_STATE_REASON_JOB_OUTGOING,                    // IPP,
    SMJOB_STATE_REASON_JOB_HOLD_UNTIL_SPECIFIED,        // IPP,
    SMJOB_STATE_REASON_RESOURCES_ARE_NOT_READY,         // IPP,
    SMJOB_STATE_REASON_PRINTER_STOPPED_PARTLY,          // IPP,
    SMJOB_STATE_REASON_PRINTER_STOPPED,                 // WSD, IPP
    SMJOB_STATE_REASON_JOB_INTERPRETING,                // IPP,
    SMJOB_STATE_REASON_JOB_QUEUED,                      // IPP,
    SMJOB_STATE_REASON_JOB_TRANSFORMING,                // IPP,
    SMJOB_STATE_REASON_JOB_QUEUED_FOR_MARKER,           // IPP,
    SMJOB_STATE_REASON_JOB_PRINTING,                    // WSD, IPP
    SMJOB_STATE_REASON_JOB_CANCELED_BY_USER,            // WSD, IPP
    SMJOB_STATE_REASON_JOB_CANCELED_BY_OPERATOR,        // IPP,
    SMJOB_STATE_REASON_JOB_CANCELED_AT_DEVICE,          // WSD, IPP
    SMJOB_STATE_REASON_ABORTED_BY_SYSTEM,               // IPP,
    SMJOB_STATE_REASON_UNSUPPORTED_COMPRESSION,         // WSD, IPP
    SMJOB_STATE_REASON_COMPRESSION_ERROR,               // WSD, IPP
    SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT,          // WSD, IPP
    SMJOB_STATE_REASON_DOC_FORMAT_ERROR,                // WSD, IPP
    SMJOB_STATE_REASON_PROCESSING_TO_STOP_POINT,        // IPP,
    SMJOB_STATE_REASON_SERVICE_OFF_LINE,                // IPP,
    SMJOB_STATE_REASON_JOB_COMPLETED_SUCCESSFULLY,      // WSD, IPP
    SMJOB_STATE_REASON_JOB_COMPLETED_WITH_WARNINGS,     // WSD, IPP
    SMJOB_STATE_REASON_JOB_COMPLETED_WITH_ERRORS,       // WSD, IPP
    SMJOB_STATE_REASON_JOB_RESTARTABLE,                 // IPP,
    SMJOB_STATE_REASON_QUEUE_IN_DEVICE,                 // IPP,
    SMJOB_STATE_REASON_DOC_TIMEOUT_ERROR,               // WSD, 
    SMJOB_STATE_REASON_JOB_PASSWORD_WAIT,               // WSD, 
} smjob_state_reason_t;


/** @name Job status events 
 *
 * Use the SMJOB_STATUS_EVENT_X flags to access the bit fields of this uint32_t. A user 
 * can register to receive one or more of these event flags via smjob_create().  When 
 * one or more of the events occur, the corresponding event flags are logically OR'ed 
 * together and passed to the registered smjob_status_func_t callback function. 
 */
typedef uint32_t smjob_status_events_t;

/**
 * @brief Indicates a change in the 'state' element of the job's smjob_status_t struct.
 */
#define SMJOB_STATUS_EVENT_STATE                0x00000001

/**
 * @brief Indicates a change in the 'num_reasons' and/or 'reasons' elements of the job's 
 *        smjob_status_t struct.
 */
#define SMJOB_STATUS_EVENT_STATE_REASONS        0x00000002

/**
 * @brief Indicates a change in the 'koctets_processed' element of the job's 
 *        smjob_status_t struct.
 */
#define SMJOB_STATUS_EVENT_KOCTETS_PROCESSED    0x00000100

/**
 * @brief Indicates a change in the 'sheets_completed' element of the job's 
 *        smjob_status_t struct.
 */
#define SMJOB_STATUS_EVENT_SHEETS_COMPLETED     0x00000200

/**
 * @brief Indicates that new scan data has arrivied in the previously empty scan image 
 *        data queue. The user may call smjob_retrieve_image() to acquire the scan data.
 *        The event will be generated multiple times for the same job if the user consumes
 *        data faster than it is produced.
 */
#define SMJOB_STATUS_EVENT_SCAN_DATA            0x00010000

//@}


// 
// WSD job object hierarchy
// 
//  Job 
//      PrintTicket
//          JobDescription  (IPP 3.2.1.1 op attrib)
//          JobProcessing 
//          DocumentProcessing  (IPP 4.2 job template)
//      Documents
//          Document
//              DocumentDescription  (IPP 3.2.1.1 op attrib)
//              DocumentProcessing  (IPP 4.2 job template)
//      JobStatus
//          JobId
//          JobState
//          JobStateReasons
//              JobStateReason
//          KOctetsProcessed
//          MediaSheetsCompleted
//

/** 
 * Receive raw Page Description Language (PDL) document data from the user.
 *  
 * This user-defined callback function is registered by the user in the document ticket
 * and is called by the device to acquire the raw PDL document data from the user. The 
 * device will call the function repeatedly to stream data for a document of arbitrary
 * size. 
 *  
 * The device may initiate the data transfer at any time following submission of the 
 * document via smjob_add_document(). However, the user can assume the device will not
 * call the registered callback from the context of the user's call to 
 * smjob_add_document(). 
 *  
 * In certain situations, the device may wait "a long time" before the initial callback or
 * in between callbacks while document transfer is in progress, e.g. following a paper out
 * condition. This can result in timeouts in the overlying user data stream. In these 
 * cases the user may be required to re-authenticate and/or re-establish an overlying 
 * connection before the document transfer can continue.  The device must account for 
 * cases like this, e.g. by adjusting the callback timeout for the initial callback or an 
 * intermediate callback following a long delay. If the user is unable to re-establish the 
 * overlying connection following a long delay between callbacks then the user should 
 * report an error condition by returning -1. 
 *  
 * @param [out] buf - Pointer to a buffer owned by the device into which document data is 
 *        to be copied.
 *  
 * @param len - Length in bytes of the buffer.
 *  
 * @param [in,out] timeout_ms - Pointer to storage allocated by the device populated 
 *       with maximum time in milliseconds that the function should block waiting for
 *       data. A value of -1 means 'infinite', in which case the function should block
 *       until 'len' bytes of data have been copied into 'buf' or until an EOF or error
 *       condition has occurred.
 *  
 *       The function should always return immediately after copying 'len' bytes of data
 *       into 'buf' or to report an EOF or error condition. 
 *  
 *       If the timeout expires before any data is acquired, or before an EOF or
 *       error condition occurs, the user must signal a timeout by resetting the value
 *       pointed to by timeout_ms to 0 and then return a value of -1 to indicate an error
 *       condition.
 * 
 *       If the timeout expires after partial data has been acquired, the user should
 *       simply return the number of bytes received and NOT signal a timeout.
 * 
 *       If only partial data has been acquired when an EOF condition is detected, the
 *       user should immediately return the number of bytes received and NOT wait for the
 *       timeout to expire and NOT signal a timeout. Note that in this case the smjob API
 *       is not aware of the EOF condition and will likely call back again, at which point
 *       the user can report the EOF by returning 0. 
 * 
 * @param [in] user_data - The opaque user context provided with the callback in the 
 *        document ticket.
 *       
 * @return Standard BSD socket recv return value.
 * 
 * @retval >0 Number of bytes actually received.
 * 
 * @retval 0 EOF condition detected. No bytes were copied to the receive buffer and no
 *         more incoming data is available for this stream.
 * 
 * @retval -1 An error or timeout occurred. If timeout, then the value pointed to by
 *         timeout_ms must also be reset to 0 to distinguish from a general error
 *         condition. Otherwise, the device assumes an unrecoverable error has occurred
 *         resulting in premature termination of the incoming data stream.
 */
typedef int32_t (*smjob_recv_func_t)(char *buf, uint32_t len, 
                                      uint32_t *timeout_ms, void *user_data);

/** 
 * Send raw document data to the user.
 *  
 * This user-defined callback function is registered by the user in the document ticket
 * and is called by the device to deliver the raw document data to the user. The device
 * will call the function repeatedly to stream data for a document of arbitrary size. 
 *  
 * The device may initiate the data transfer at any time following submission of the 
 * document via smjob_add_document(). However, the user can assume the device will not
 * call the registered callback from the context of the user's call to 
 * smjob_add_document(). 
 *  
 * @param [out] buf - Pointer to a buffer owned by the device from which the 
 *       user must copy document data. The device retains ownership of the buffer upon
 *       return from the callback.
 *  
 * @param len - Length in bytes of the buffer.
 *  
 * @param [in,out] timeout_ms - Pointer to storage allocated by the device populated 
 *       with maximum time in milliseconds that the function should block waiting to send
 *       data. A value of -1 means 'infinite', or no timeout requested. In this case, the
 *       function should block until 'len' bytes of data have been copied from 'buf' or
 *       until an error condition has occurred.
 *  
 *       If the timeout expires before the requested data can be acquired or an error
 *       condition occurs, the user should reset the value pointed to by timeout_ms to 0
 *       to signal a timeout and return a value of -1 to indicate an error condition (i.e.
 *       timeout).
 * 
 * @param [in] user_data - The opaque user context provided with the callback in the 
 *        document ticket.
 *       
 * @return Standard BSD socket send return value.
 * 
 * @retval >0 Number of bytes actually copied.
 * 
 * @retval -1 An error or timeout occurred. If timeout, then the value pointed to by 
 *         timeout_ms must also be reset to 0 to distinguish from a general error
 *         condition. Otherwise, the device assumes an unrecoverable error has occurred
 *         resulting in premature termination of the outgoing data stream.
 */
typedef int32_t (*smjob_send_func_t)(char *buf, uint32_t len, 
                                      uint32_t *timeout_ms, void *user_data);

/**
 * @brief Job Description attributes (IPP 3.2.1.1 Operation Attributes; WSD 3.5.2.1 
 *       JobDescription)
 *  
 * The following attributes are used for job identification. They do not directly affect
 * how the job is processed. An instance of these attributes are included in the job
 * ticket.
 */
typedef struct
{
    /** 
     * @brief Job Name (IPP job-name; WSD JobName)
     */
    char                        job_name[SMJOB_MAX_NAME_LEN];

    /** 
     * @brief Originating User Name (IPP requesting-user-name; WSD JobOriginatingUserName)
     */
    char                        originating_user_name[SMJOB_MAX_NAME_LEN];

    /** 
     * @brief Vendor-extended Job Description attributes.
     *  
     * An opaque pass-through value for vendor-specific attributes. 
     */
    void                        *vnd_extensions;

} smjob_description_t;

/** 
 * @brief Job Processing Attributes (IPP 4.2 Job Template; WSD 3.5.2.2 JobProcessing)
 * 
 * These attributes apply to processing of the job as a whole including any attached
 * documents. An instance of these attributes are included in the job ticket.
 */ 
typedef struct
{
    /** 
     * @brief IPP job-priority (1:100); WSD Priority(1:100 default 50)
     */
    uint8_t                     priority;

    /** 
     * @brief Vendor-extended Job Processing attributes.
     *  
     * An opaque pass-through value for vendor-specific attributes. 
     */
    void                        *vnd_extensions;

} smjob_processing_t;         

/**
 * @brief Document description attributes unique to the Print Service type.
 */
typedef struct
{
    /**
     * @brief User-supplied callback function used by the device to acquire raw document 
     * data, e.g. Page Description Language (PDL) document data for a print job. 
     */
    smjob_recv_func_t           recv_func;

    /**
     * @brief Opaque user context returned to the user in the recv_func callback.
     */
    void                        *user_data;

    /** 
     * @brief Compression algorithm used on document contents (IPP compression (keyword); 
     * WSD Compression (default "None"))
     *  
     * Refers to compression applied independent of the Page Description Language (PDL). 
     * Does not include compression inherent to the PDL. 
     */
    smjob_compression_t         compression[COMPRESSION_STR_LEN];

    /** 
     * @brief Format of document contents expressed as an enumerated MIME media type (IPP 
     * document-format; WSD Format (default "unknown")) 
     */
    smjob_format_t              format[FORMAT_STR_LEN];

} smjob_print_doc_description_t;

/**
 * @brief Document description attributes unique to the Scan Service type.
 */
typedef struct
{
    /**
     * @brief User-supplied callback function used by the device to deliver raw 
     * document data, e.g. image data from a scanned document. 
     */
    smjob_send_func_t          send_func;

    /**
     * @brief Opaque user context returned to the user in the recv/send callback.
     */
    void                        *user_data;

} smjob_scan_doc_description_t;

/**
 * @brief Document Description (IPP 3.2.1.1 Operation Attributes; WSD 3.6.1 
 * DocumentDescription)
 *  
 * The following attributes are used for identification, interpretation, and delivery of
 * document data. They do not directly affect how the document is rendered, finished, or 
 * otherwise processed. An instance of these attributes are included in the document 
 * ticket. 
 */
typedef struct
{
    /** 
     * @brief Document Name (IPP document-name; WSD DocumentName) 
     *  
     * May be NULL if document is not assigned a name. 
     */
    char                        doc_name[SMJOB_MAX_NAME_LEN];

    /**
     * @brief Job-type-specific attributes 
     *  
     * Interpretation of these attributes depends on the job type of the containing job.
     */
    union {
        smjob_print_doc_description_t print;
        smjob_scan_doc_description_t scan;
    } type;

    /** 
     * @brief Vendor-extended Document Processing attributes.
     *  
     * An opaque pass-through value for vendor-specific attributes. 
     */
    void                        *vnd_extensions;

} smjob_doc_description_t;

/**
 * @brief Document processing attributes common among impression-outputting Services such 
 *        as Print, FaxIn and EmailIn.
 */
typedef struct
{
    /** 
     * @brief Number of copies (IPP copies (1:MAX); WSD Copies (1:(2^15)-1 default 1)) 
     *  
     * @note The supported number copies when sheet_collate is 'true' may be 
     * limited by the number of physical output bins on the device, or by the size of the 
     * device's spooler memory. 
     */
    uint32_t                    copies;

    /** 
     * @brief Hardcopy media size (IPP media (keyword); WSD MediaSizeName (PWG5101.1 
     *        defines values))
     */
    mediasize_t                 media_size;

    /** 
     * @brief Hardcopy media type (IPP media (keyword); WSD MediaType (default "unknown"))
     */
    mediatype_t                 media_type;

    /** 
     * @brief Hardcopy media parameters sent by AirPrint (when no media name provided)
     */
    smjob_media_col_t           media_col;

    /** 
     * @brief Hardcopy sheet collation (IPP (RFC 3381) sheet-collate (keyword); WSD
     *        Collate (bool default false))
     *  
     * Valid only when the value of 'copies' is greater than one, ignored otherwise.
     */
    bool                        sheet_collate;

    /** 
     * @brief Hardcopy sides (IPP sides (keyword); WSD sides (default "OneSided"))
     */
    smjob_sides_t               sides[SIDES_STR_LEN];

    /** 
     * @brief Hardcopy image quality (IPP print-quality (enum); WSD PrintQuality (default 
     *        "Normal"))
     */
    smjob_quality_t             quality;

    /** 
     * @brief color mode to use when printing a document
     */
    smjob_color_mode_t          color_mode[COLOR_MODE_STR_LEN];

    /** 
     * @brief print content optimize to use when printing a document
     */
    smjob_print_content_optimize_t print_content_optimize[PRINT_CONTENT_OPTIMIZE_STR_LEN];

} smjob_print_doc_processing_t;

/**
 * @brief Document processing attributes common among image-outputting Services such as 
 * Scan, FaxOut and EmailOut 
 */
typedef struct
{
    /**
     * @brief Source of the document to be scanned.
     */
    smjob_input_source_t        input_source;

    /** 
     * @brief Compression algorithm used on document contents 
     *  
     * Refers to compression applied independent of the document format. 
     */
    smjob_compression_t         compression[COMPRESSION_STR_LEN];

    /** 
     * @brief Desired document format of the rendered image data, expressed as an 
     *        enumerated MIME media type.
     */
    smjob_format_t              format[FORMAT_STR_LEN]; 

} smjob_scan_doc_processing_t;

/** 
 * @brief Document Processing Attributes (IPP 4.2 Job Template; WSD 3.5.2.3 
 *        DocumentProcessing)
 * 
 * These attributes apply to processing of job documents. A separate instance of these 
 * attributes are included in each document ticket.  An instance is also included in the 
 * encapsulating job ticket which serves as per-job default values. 
 *  
 * Document Processing attributes exist at two levels: 
 *  
 * 1) Attributes inherited from the Imaging Service super class (e.g. Number Up, 
 * Presentation Direction, etc.). 
 *  
 * 2) Service-specific attributes that may be common to two or more Services. Generally 
 * these attributes are common among image-outputting Services such as Scan, FaxOut and 
 * EmailOut, and among impression-outputting Services such as Print, FaxIn and EmailIn. 
 */
typedef struct                  
{  
    /** 
     * @brief Hardcopy orientation, or orientation of scan media box (IPP 
     *        orientation-requested (enum); WSD Orientation (default "Portrait"))
     */
    smjob_orientation_t         orientation;

    /** 
     * @brief Resolution 
     *  
     * Resolution in the Feed and Cross Feed directions at which to impress or capture the
     * image (IPP printer-resolution; WSD Resolution) 
     *  
     * If only one component is specified, a square resolution is assumed (e.g. 100x100). 
     */
    smjob_resolution_t          resolution;

 
    /**
     * @brief Job-type-specific attributes 
     *  
     * Interpretation of these attributes depends on the job type of the containing job.
     */
    union {
        smjob_print_doc_processing_t print;
        smjob_scan_doc_processing_t scan;
    } type;

    /** 
     * @brief Vendor-extended Document Processing attributes.
     *  
     * An opaque pass-through value for vendor-specific attributes. 
     */
    void                        *vnd_extensions;

} smjob_doc_processing_t;     

/** 
 * @brief Job Ticket
 *  
 * The job ticket contains all job attributes applicable to a job as a whole, including job
 * description attributes, job processing attributes, and default document processing 
 * attributes. 
 *  
 * The job ticket does not contain raw job data (e.g. Page Description Language (PDL) 
 * data). Raw job data is streamed to the device via the smjob_recv_func_t function 
 * callback. Job settings in the job ticket take precedence over any conflicting job 
 * settings embedded within the raw job data (per RFC 2911 Section 15.2). 
 */
typedef struct                  
{                               
    smjob_type_t                job_type;

    /** 
     * @brief Job Description attributes
     */ 
    smjob_description_t         job_description;

    /** 
     * @brief Job Processing attributes
     */ 
    smjob_processing_t          job_processing;

    /** 
     * @brief Default Document Processing attributes
     *  
     * These values represent per-job document processing defaults.
     */ 
    smjob_doc_processing_t      doc_processing;

    /** 
     * @brief the actual ipp-attribute-fidelity
     *  
     * If this is true, the job must be rejected if we do not 
     * support any of the requested attributes. If false, print the 
     * job with a best effort. 
     */ 
    uint32_t                    ipp_attribute_fidelity;

    /** 
     * @brief Must Honor (IPP ipp-attribute-fidelity (except in IPP it applies wholesale 
     *        to all Job Ticket values); WSD MustHonor attribute)
     *  
     * Bitmask identifying attributes within the Job Ticket that the user requires to be
     * processed exactly according to the values requested. If any of the selected 
     * attributes or attribute values are either unsupported by the device or conflict 
     * with other attributes, the Job Ticket is considered invalid and the job will be 
     * aborted. Otherwise, "best effort" processing will be attempted for any unsupported 
     * or conflicting attributes.  In such cases the attributes may be ignored, 
     * substituted with default values, or substitued with values that are as similar as 
     * possible to those requested. (For further details, reference RFC 2911 section 15.1)
     *  
     * Must be initialized to an empty value using the SMJOB_ATTRIBUTE_MASK_INIT() 
     * macro. The bitmask may then be populated with any desired SMJOB_ATTRIBUTE_ID_XX 
     * values using the SMJOB_ATTRIBUTE_MASK_ADD_IDS() macro. 
     */
    smjob_attribute_mask_t      must_honor;

    /**
     * @brief Omitted attributes 
     *  
     * Bitmask identifying attributes within the Job Ticket that have not been populated 
     * with a valid value, implying that either a default value is desired or that a 
     * vendor-specific value will be supplied via the 'vnd_extensions' component. 
     *  
     * Must be initialized to an empty value using the SMJOB_ATTRIBUTE_MASK_INIT() 
     * macro. The bitmask may then be populated with any desired SMJOB_ATTRIBUTE_ID_XX 
     * values using the SMJOB_ATTRIBUTE_MASK_ADD_IDS() macro. 
     */
    smjob_attribute_mask_t      omitted;

    /** 
     * @brief Vendor-extended Job Ticket attributes.
     *  
     * An opaque pass-through value for vendor-specific attributes. 
     */
    void                        *vnd_extensions;

} smjob_ticket_t;             

/** 
 * @brief Document Ticket
 */
typedef struct                  
{
    /** 
     * @brief Document Description attributes 
     */
    smjob_doc_description_t     doc_description;

    /** 
     * @brief Document Processing attributes 
     *  
     * The following values instruct the device on how the document is to be rendered and 
     * finished. 
     *  
     * Note that default Document Processing attribute values are derived from the 
     * encapsulating job ticket rather than from device-specific default values. 
     */
    smjob_doc_processing_t      doc_processing;

    /** 
     * @brief Must Honor attributes (IPP ipp-attribute-fidelity (except in that case 
     * applies to wholesale to all Job Ticket values); WSD MustHonor attribute) 
     *  
     * Bitmask of attributes within the Document Ticket which must be processed according 
     * to the exact values that the user has requested. If any of the selected attributes 
     * or their values are unsupported by the device or conflict with other attributes, the
     * Job Ticket is considered invalid and the job will be aborted. Otherwise, "best 
     * effort" processing is allowed, and unsupported or conflicting attributes will be 
     * ignored or substituted with default values or values that are as similar as 
     * possible to those requested. (For further details reference RFC 2911 section 15.1)
     *  
     * Must be initialized to an empty value using the SMJOB_ATTRIBUTE_MASK_INIT() 
     * macro. The bitmask may then be populated with any desired SMJOB_ATTRIBUTE_ID_XX 
     * values using the SMJOB_ATTRIBUTE_MASK_ADD_IDS() macro. 
     */
    smjob_attribute_mask_t      must_honor;

    /**
     * @brief Omitted attributes 
     *  
     * Bitmask of attributes within the Document Ticket that have not been populated with 
     * a valid value, implying that either a default value is desired or that a 
     * vendor-specific value will be supplied via the 'vnd_extensions' component. 
     *  
     * Must be initialized to an empty value using the SMJOB_ATTRIBUTE_MASK_INIT() 
     * macro. The bitmask may then be populated with any desired SMJOB_ATTRIBUTE_ID_XX 
     * values using the SMJOB_ATTRIBUTE_MASK_ADD_IDS() macro. 
     */
    smjob_attribute_mask_t      omitted;

    /** 
     * @brief Vendor-extended Document Description attributes.
     *  
     * An opaque pass-through value for vendor-specific attributes and/or attribute 
     * values.  Standard attributes for which values are being extended must be indicated 
     * as such in the vnd_ext_value bitmask.
     */
    void                        *vnd_extensions;

} smjob_doc_ticket_t;

/** 
 * @brief Job Status (IPP 4.3 Job Description Attributes; WSD 3.5.1 JobStatus)
 */
typedef struct                  
{
    /** 
     * @brief Unique job identifier (IPP job-id (1:MAX); WSD JobId (0:(2^15)-1) )
     */
    uint32_t                    job_id;

    /** 
     * @brief Number of attached documents (IPP number-of-documents (0:MAX); WSD 
     *        NumberOfDocuments)
     */
    uint32_t                    num_docs; 

    /** 
     * @brief Current job state (IPP job-state (enum); WSD JobState)
     */
    smjob_state_t               state;

    /** 
     * @brief Current job state reasons (IPP job-state-reasons (setOf keyword); WSD 
     * JobStateReasons JobStateReason (collection) ) 
     *  
     * API owns "reasons" memory.  User must copy.
     */
    uint32_t                    num_reasons; 
    smjob_state_reason_t        reasons[SMJOB_MAX_STATE_REASONS]; 

    /** 
     *  @brief Kilo-octets Processed
     *  
     *  IPP job-k-octets-processed (0:MAX); WSD KOctetsProcessed (0:(2^15)-1)
     */
    uint32_t                    koctets_processed;

    /** 
     *  @brief Media Sheets Completed
     *  
     *  IPP job-media-sheets-completed (0:MAX); WSD MediaSheetsCompleted (0:(2^15)-1)
     */
    uint32_t                    sheets_completed;

    /** 
     * @brief Vendor-extended status attributes. 
     *  
     * An opaque pass-through value for vendor-specific attributes. 
     */
    void                        *vnd_extensions;

} smjob_status_t;

/** 
 * @brief Document Status
 */
typedef struct {
    /** 
     * @brief Unique document identifier (IPP n/a; WSD DocumentId (1:(2^15)-1) ) 
     *  
     * @note The document identifier is guaranteed to be unique only within the 
     * context of a particular job. 
     */
    uint32_t                    doc_id; 

} smjob_doc_status_t;


typedef struct 
{
    mediasize_t size_enum;
    char        *name;
    uint32_t    width;
    uint32_t    height;
} sm_media_size_entry_t;


uint32_t sm_media_size_get_table(sm_media_size_entry_t **table);


typedef struct 
{
    mediatype_t type_enum;
    char        *name;
} sm_media_type_map_t;

typedef sm_media_type_map_t *sm_media_type_entry_t;

uint32_t sm_media_type_get_table(sm_media_type_entry_t **table);

/** 
 * Notify the user of asynchronous job status events.
 *  
 * The device will use this callback to notify the user of job related status changes. The
 * user can then request current job status via smjob_get_status() to get the status 
 * changes indicated by the 'job_events' flags. 
 *  
 * The user may query job status from the context of this callback.
 *  
 * @param job_id - job for which the status change applies 
 *  
 * @param job_events - a subset of the status event flags requested by the user during 
 *        callback registration.
 *  
 * @param [in] user_data - The opaque user context provided in the call to 
 *       smjob_create_job().
 */
typedef void (*smjob_status_func_t)(uint32_t job_id, smjob_status_events_t job_events,
                                     void *user_data);

/** 
 * @brief Get job_id of the currently running job.
 *  
 * This function returns the job_id of the currently running job (if available).
 * This is a bit of a hack, but allows certain areas of the code to access things
 * like the current job ticket. Ideally the system would have a better way to
 * pass around status and ticket info.
 *
 * @return Job ID of the currently running job, or SMJOB_INVALID_JOB_ID if
 *         the job_ticket struct is invalid or no job is running.
 */
uint32_t smjob_get_current_job_id(void);

/** 
 * @brief Create a job.
 *  
 * This function creates a job and adds it to the job queue. 
 *  
 * To create a job, a user will typically call smjob_get_default_ticket(), modify any 
 * attribute values that differ from default values, and then pass the updated job ticket 
 * as a parameter to this function. 
 *  
 * The user must add one or more documents via smjob_add_document() before the device 
 * will begin processing the job. 
 * 
 * @param [in] job_ticket - Pointer to storage that has been populated by the user with 
 *        the desired job ticket attribute values.  Storage may be freed upon return from
 *        this function.
 *  
 * @param status_func - User-supplied callback function used by the device to notify the 
 *        user of changes to job status, or NULL if don't care about job status events.
 *  
 * @param job_event_mask - One or more event flags logically OR'ed together, indicating 
 *                       the job status events for which the user desires callback
 *                       notification.
 *  
 * @param [in] user_data - Opaque user context returned to the user in the status_func
 *        callback.
 *  
 * @return Job ID of the created job for use in subsequent API calls on success, or
 *         SMJOB_INVALID_JOB_ID if the job_ticket struct is invalid or could not
 *         otherwise be processed.
 */
uint32_t smjob_create(const smjob_ticket_t *job_ticket, smjob_status_func_t status_func,
                       smjob_status_events_t job_event_mask, void *user_data);

/** 
 * @brief Add a document to a job.
 *  
 * After creating a job via smjob_create(), the user may use this function to attach one
 * or more documents. 
 *  
 * To add a document to a job, a user will typically call smjob_get_default_document(), 
 * modify any attribute values that differ from default values, and then pass the updated 
 * document ticket as a parameter to this function.
 *  
 * The user should wait to call this function until some document data is immediately
 * available for consumption by the smjob_recv_func_t callback. Doing so will avoid a 
 * potential timeout in the recv callback and delays in underlying device layers due to 
 * unnecessary locking of high-value resources. 
 *  
 * For example, an API user who must acquire document data over HTTP via a document URI 
 * should wait until the HTTP request has been issued, HTTP headers are processed, and 
 * some initial raw document data has been buffered before calling smjob_add_document().
 * 
 * @param job_id - The job ID returned by smjob_create(). 
 *  
 * @param [in] doc_ticket - Pointer to storage that has been populated by the user with 
 *        the document ticket attributes. Storage may be freed upon return from this
 *        function.
 *  
 * @param last_doc - "true" if this is the last document to be added to the job, 
 *        "false" otherwise
 *  
 * @return Document ID of the added document for use in subsequent API calls on success, 
 *         or SMJOB_INVALID_DOC_ID if the job ID is invalid, or if the doc_ticket struct is
 *         invalid or could not otherwise be processed.
 */
uint32_t smjob_add_document(uint32_t job_id, smjob_doc_ticket_t *doc_ticket, bool last_doc);

/**
 * @brief Enumerate job documents.
 *  
 * The user may call this routine repeatedly to acquire a list of all documents added to
 * the specified job, given in the same order in which the documents were added. 
 *  
 * These document IDs are identical to the IDs returned in the original call to 
 * smjob_add_document(). 
 *  
 * @param job_id - The job ID, e.g. as returned by smjob_create(). 
 *  
 * @param doc_id - The document ID returned in the previous call to 
 *        smjob_get_next_document(), or SMJOB_INVALID_DOC_ID for the first iteration.
 * 
 * @return Document ID of the next document in the queue on success, or 
 *         SMJOB_INVALID_DOC_ID if either no more documents or if the job ID or document ID
 *         is not recognized.
 */
uint32_t smjob_get_next_document(uint32_t job_id, uint32_t doc_id);

/** 
 * @brief Get document status information.
 * 
 * @param doc_id - The document ID, e.g. as returned by smjob_add_document(). 
 *  
 * @param doc_status - Pointer to user-provided storage into which the function will copy 
 *                   status information about the requested document.
 *  
 * @retval SMJOB_OK - Document status struct was successfully populated.
 *  
 * @retval SMJOB_FAIL - Invalid document ID or struct pointer.
 */
smjob_rcode_t smjob_get_document_status(uint32_t doc_id, smjob_doc_status_t *doc_status);

/**
 * @brief Get the document ticket for the specified document. 
 *  
 * Returns a copy of the document ticket that has been applied to the document. 
 *  
 * @note The attribute values returned by this function represent the values actually 
 *       applied to the document and not necessarily those explicitly provided by the user
 *       in the call to smjob_add_document(). Attributes that were assigned unsupported
 *       or conflicting values and did not have the must_honor flag set by the user may
 *       have been substituted with default values or values as similar as possible to
 *       those originally requested. 
 *  
 * @param doc_id - The document ID, e.g. as returned by smjob_add_document(). 
 * 
 * @param [out] doc_ticket - Pointer to user-provided storage into which the function will
 *        copy the document ticket for the requested document. It is important that the
 *        user properly initialize the doc_processing component of the doc_ticket struct,
 *        either assigning a pointer to user-owned storage for receiving doc_processing
 *        attributes or setting the pointer to NULL if these values are not needed.
 *  
 * @retval SMJOB_OK - The doc_ticket struct was successfully populated.
 *  
 * @retval SMJOB_FAIL - Invalid document ID or doc_ticket pointer.
 */
smjob_rcode_t smjob_get_document_ticket(uint32_t doc_id, smjob_doc_ticket_t *doc_ticket);

/** 
 * @brief Retrieve image data.
 *  
 * Retrieves all or part of the image data associated with the specified job. Applies to 
 * jobs that generate image data, such as Scan and FaxIn. To retrieve all data for a given
 * image, the function should be called repeatedly with the same job ID until either an 
 * EOF or error condition occurs. The function may be called anytime following a 
 * successful call to smjob_create() but must be called before job timeout. 
 *  
 * If the job ticket specifies that multiple images are to be transferred and that the 
 * image file format is a single image format type (e.g. SMJOB_FORMAT_IMAGE_JPEG), then 
 * the function will generate multiple EOF conditions per job. In this case the user 
 * must continue to call the function repeatedly with the same job ID until a separate EOF
 * condition has been generated for each successive image. If more than one EOF is 
 * generated back-to-back, not separated by image data, this is an indication that all 
 * images have been retrieved for the job. 
 *  
 * @param job_id - The job ID, e.g. as returned by smjob_create(). 
 *  
 * @param [out] buf - Pointer to user-provided storage into which the function will store 
 *        a pointer to a buffer containing image data. The user is given ownership of the
 *        buffer and must free the buffer once processing of the data is completed. If the
 *        value returned by 'len' is <= 0, the pointer will be set to NULL to indicate
 *        that no buffer was delivered.
 *  
 * @param [out] len - Pointer to user-provided storage into which the function will store 
 *        the number of bytes of data stored in the buffer, or 0 (EOF) if no more data
 *        exists for the given image, or -1 if a timeout or other I/O error occurred while
 *        waiting for data (see return code for error types).
 *  
 * @param [in,out] timeout_ms - the maximum time in milliseconds that the function should 
 *        block waiting for data. A value of -1 requests 'infinite', in which case the
 *        function will block until either a buffer has been acquired or an EOF, timeout,
 *        or error condition has occurred (disambiguated by the 'len' param). A value of 0
 *        requests that the function timeout immediately unless a buffer is already
 *        available when the call is made. If the timeout expires before a buffer becomes
 *        available then the function returns SMJOB_IO_TIMEOUT.
 *  
 * @retval SMJOB_OK - The request was completed successfully. 
 *  
 * @retval SMJOB_FAIL - Invalid job ID 
 *  
 * @retval SMJOB_IO_ABORTED - Job was canceled prior to EOF. 
 *  
 * @retval SMJOB_IO_TIMEOUT - User-provided timeout expired before data became available.
 */
smjob_rcode_t smjob_retrieve_image(uint32_t job_id, uint8_t **buf, uint32_t *len, uint32_t timeout_ms);

/** 
 * @brief Cancel the specified job.
 *  
 * @param job_id - The job ID, e.g. as returned by smjob_create(). 
 *  
 * @retval SMJOB_OK - The cancel job request was successfully submitted.
 *  
 * @retval SMJOB_FAIL - Invalid job ID.
 */
smjob_rcode_t smjob_cancel(uint32_t job_id);

/** 
 * @brief Get job status information.
 * 
 * The user may call this routine to poll for job status, or to query job status following
 * asynchronous status event notification from the smjob_status_func_t callback. 
 *  
 * This call may be made from the context of the smjob_status_func_t callback.
 *  
 * @param job_id - The job ID, e.g. as returned by smjob_create(). 
 *  
 * @param job_status - Pointer to user-provided storage into which the function will copy 
 *                   current status and other information about the requested job.
 *  
 * @retval SMJOB_OK - Job status struct was successfully populated.
 *  
 * @retval SMJOB_FAIL - Invalid job ID or struct pointer.
 */
smjob_rcode_t smjob_get_status(uint32_t job_id, smjob_status_t *job_status);

/**
 * @brief Get the job ticket for the specified job. 
 *  
 * Returns a copy of the job ticket that has been applied to the job. 
 *  
 * @note The attribute values returned by this function represent the values actually 
 *       applied to the job and not necessarily those explicitly provided by the user
 *       in the call to smjob_create(). Attributes that were assigned unsupported or
 *       conflicting values and did not have the must_honor flag set by the user may have
 *       been substituted with default values or values as similar as possible to those
 *       originally requested.
 *  
 * @param job_id - The job ID, e.g. as returned by smjob_create(). 
 * 
 * @param [out] job_ticket - Pointer to user-provided storage into which the function will
 *        copy the job ticket for the requested job.
 *  
 * @retval SMJOB_OK - The job ticket struct was successfully populated.
 *  
 * @retval SMJOB_FAIL - Invalid job ID or job_ticket pointer.
 */
smjob_rcode_t smjob_get_ticket(uint32_t job_id, smjob_ticket_t *job_ticket);

/**
 * @brief Get a list of submitted jobs.
 *  
 * The user may call this routine repeatedly to acquire a list of all jobs that have been 
 * submitted to the device, in order of submission time from newest to oldest, including
 * both active and completed jobs. The number of jobs in the list is dependent on the size
 * of the job history queue and/or the lifetime of completed jobs in the queue. 
 *  
 * These job IDs are identical to the IDs returned in the original call to 
 * smjob_create(). 
 *  
 * @note The list includes jobs submitted by all users of the API, and the list may 
 *       therefore contain jobs that were not submitted by the caller.
 *  
 * @param job_id - The job ID returned in the previous call to smjob_get_next(), or 
 *        SMJOB_INVALID_JOB_ID for the first iteration.
 * 
 * @return The ID of the next job in the job history queue on success, or 
 *         SMJOB_INVALID_JOB_ID if either no more jobs or job_id not recognized.
 */
uint32_t smjob_get_next(uint32_t job_id);

/**
 * @brief Get the default job ticket.
 *  
 * This function allows the user to determine the default values of the job ticket. 
 * Default ticket values are usually constant for a given device. 
 *  
 * To create a job, a user will typically first call this routine, modify any attribute 
 * values that differ from default values, and then pass the updated job ticket as a 
 * parameter to smjob_create(). 
 *  
 * @param job_type - The type of default job ticket to retrieve, e.g. print, scan, 
 *                 fax-out, etc.
 *  
 * @param [out] job_ticket - Pointer to user-provided storage into which this function 
 *        will copy the default job ticket for the requested job type.
 *  
 * @retval SMJOB_OK - The job ticket was successfully populated with default values. 
 *  
 * @retval SMJOB_FAIL - Invalid job_type or job_ticket pointer.
 */
smjob_rcode_t smjob_get_default_ticket(smjob_type_t job_type, smjob_ticket_t *job_ticket);

/**
 * @brief Get default document ticket.
 *  
 * This function allows the user to determine default values for the document ticket of 
 * a particular job. Note that default values for the Document Processing attributes are 
 * job-specific (i.e. copied from the encapsulating job ticket), while defaults for the
 * other document attributes are device-specific. 
 *  
 * To add a document to a job, a user will typically call this routine, modify any 
 * attribute values that differ from default values, and then pass the updated document 
 * ticket as a parameter to smjob_add_document(). 
 *  
 * @param job_id - The job ID, e.g. as returned by smjob_create(). 
 *  
 * @param [out] doc_ticket - Pointer to user-provided storage into which this function 
 *        will copy default document ticket values for the requested job.
 *  
 * @retval SMJOB_OK - The doc_ticket struct was successfully populated with default values.
 *  
 * @retval SMJOB_FAIL - Invalid job ID or doc_ticket pointer.
 */
smjob_rcode_t smjob_get_default_doc_ticket(uint32_t job_id, smjob_doc_ticket_t *doc_ticket);

/** 
 * @name Device Capabilities 
 *  
 * Use these routines to determine the subset of standard attribute values supported by 
 * the device. The user may select from these values when populating the job and document 
 * tickets. 
 *  
 * To get supported values of enumerated attribute types, iterate through indexes starting
 * at 0 until the function returns SM_BAD_INDEX.  The indexes are useful only for 
 * enumeration of device capabilities and unless otherwise indicated are assigned in no 
 * particular order or priority. 
 */

smjob_rcode_t smjob_get_icons(smjob_type_t job_type, uint32_t index, char **value);

smjob_rcode_t smjob_get_supported_compression(smjob_type_t job_type, uint32_t index,
                                              smjob_compression_t **value);

smjob_rcode_t smjob_get_default_format(smjob_type_t job_type, smjob_format_t **value);
smjob_rcode_t smjob_get_supported_format(smjob_type_t job_type, uint32_t index,
                                         smjob_format_t **value);
char *smjob_get_mime_type(smjob_format_t doc_format);

smjob_rcode_t smjob_get_supported_finishings(smjob_type_t job_type, uint32_t index, smjob_finishing_t *value);

smjob_rcode_t smjob_get_default_orientation(smjob_type_t job_type, smjob_orientation_t *value);
smjob_rcode_t smjob_get_supported_orientation(smjob_type_t job_type, uint32_t index, smjob_orientation_t *value);

smjob_rcode_t smjob_get_default_resolution(smjob_type_t job_type, smjob_resolution_t *value);
smjob_rcode_t smjob_get_supported_resolution(smjob_type_t job_type, uint32_t index, smjob_resolution_t *value);

smjob_rcode_t smjob_get_default_sides(smjob_type_t job_type, smjob_sides_t **value);
smjob_rcode_t smjob_get_supported_sides(smjob_type_t job_type, uint32_t index, 
                                        smjob_sides_t **value);
smjob_rcode_t smjob_get_default_color_mode(smjob_type_t job_type, 
                                           smjob_color_mode_t **value);
smjob_rcode_t smjob_get_supported_color_mode(smjob_type_t job_type, uint32_t index,
                                             smjob_color_mode_t **value);
smjob_rcode_t smjob_get_supported_media_size(smjob_type_t job_type, uint32_t index, mediasize_t *value);
// 'name' - returns pointer to storage allocated and owned by routine
smjob_rcode_t smjob_get_supported_media_size_name(smjob_type_t job_type, uint32_t index, char** name);
smjob_rcode_t smjob_get_supported_default_media_size_name(smjob_type_t job_type, char** name);
smjob_rcode_t smjob_set_supported_default_media_size_name(smjob_type_t job_type, char* name, uint32_t name_len);
char *sm_media_size_enum_to_name( mediasize_t size_enum);
mediasize_t sm_media_size_name_to_enum(const char * size_name);

smjob_rcode_t smjob_get_supported_media_size_x_dim(smjob_type_t job_type, uint32_t index, uint32_t *value);
// return of 0 means 'not found'
uint32_t sm_media_size_enum_to_x_dim(mediasize_t size_enum);

smjob_rcode_t smjob_get_supported_media_size_y_dim(smjob_type_t job_type, uint32_t index, uint32_t *value);
// return of 0 means 'not found'
uint32_t sm_media_size_enum_to_y_dim(mediasize_t size_enum);

smjob_rcode_t smjob_get_supported_media_type(smjob_type_t job_type, uint32_t index, mediatype_t *value);
// 'name' - returns pointer to storage allocated and owned by routine
smjob_rcode_t smjob_get_supported_media_type_name(smjob_type_t job_type, uint32_t index, char** name);
smjob_rcode_t smjob_get_supported_default_media_type_name(smjob_type_t job_type, char** name);
char *sm_media_type_enum_to_name( mediatype_t type_enum);

smjob_rcode_t smjob_get_supported_media_top_margin(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_get_supported_media_bottom_margin(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_get_supported_media_left_margin(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_get_supported_media_right_margin(smjob_type_t job_type, uint32_t index, uint32_t *value);
smjob_rcode_t smjob_get_aiprint_enabled(smjob_type_t job_type, uint32_t index, 
                                        uint32_t *value);
smjob_rcode_t smjob_add_user_data_to_job_ticket(uint32_t job_id, void *user_data);
char *smjob_get_job_state_reasons_str(smjob_state_reason_t job_state_reason);

/// (print-specific)
smjob_rcode_t smjob_get_supported_media_source(smjob_type_t job_type, uint32_t index, char** source);
smjob_rcode_t smjob_get_media_table_source(smjob_type_t job_type, uint32_t index, char** source);
smjob_rcode_t smjob_get_media_table_type(smjob_type_t job_type, uint32_t index, char** name);
smjob_rcode_t smjob_get_media_table_duplex(smjob_type_t job_type, uint32_t index, uint32_t *value);

smjob_rcode_t smjob_get_default_media_col(smjob_type_t job_type, smjob_media_col_t *value);

smjob_rcode_t smjob_get_default_quality(smjob_quality_t *value);
smjob_rcode_t smjob_get_supported_quality(uint32_t index, smjob_quality_t *value);

smjob_rcode_t smjob_get_output_bin_default(smjob_type_t job_type, char** name);
smjob_rcode_t smjob_get_output_bin_supported(smjob_type_t job_type, uint32_t index, char** name);

smjob_rcode_t smjob_markers_available(smjob_type_t job_type, uint32_t index, uint32_t *value);

smjob_rcode_t smjob_get_printer_geo_location(smjob_type_t job_type, char** location);

smjob_rcode_t smjob_get_default_print_content_optimize(smjob_type_t job_type, 
                                                       smjob_print_content_optimize_t **value);
smjob_rcode_t smjob_get_supported_print_content_optimize(smjob_type_t job_type, uint32_t index, 
                                                smjob_print_content_optimize_t **value);

/// (scan-specific)
smjob_rcode_t smjob_get_supported_input_source(uint32_t index, smjob_input_source_t *value);

/// A range bounded by min and max values inclusive (print-specific).
smjob_rcode_t smjob_get_supported_copies(uint32_t *min_value, uint32_t *max_value);
smjob_rcode_t smjob_get_default_copies(uint32_t *value);

/// "true" if sheet collation is supported, "false" otherwise (print-specific).
smjob_rcode_t smjob_get_supported_sheet_collate(bool *is_supported);

uint32_t smjob_get_queued_job_count();
bool smjob_accepting_jobs();

void dump_doc_ticket(smjob_doc_ticket_t *doc_ticket, smjob_type_t job_type);
void dump_job_ticket(smjob_ticket_t *job_ticket);

//@}


/**
 * \brief Initialize the smjob module
 *
 */
void smjob_init( void );

#ifdef __cplusplus
}
#endif

#endif /* SMJOB_API_H */
