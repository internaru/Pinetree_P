/*
 * ============================================================================
 * (C) Copyright 2009-2013   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file oid_api.h
 *
 * \brief The OID API allows a module (the consumer) to use an OID
 * to access an object from another module (the producer) without knowing
 * anything about the producer.  An OID is an "Object ID."  The ID allows
 * addressing of a data value through this API using a number, the "OID."
 *
 * WARNING: For most modules within the system, it is NOT necessary to use this
 * API to access objects.  For example a "control panel" module can directly
 * include a "network" module's API to get the system's current IPV4 address.
 * There is, however, no restriction on use of this API.
 *
 * This API is an abstraction layer to simplify the access of public objects.
 * It provides a consistent interface to get the object's values for modules
 * like an embedded web server.
 *
 * The OID API supports test set routines to allow users to verify that a set
 * will succeed prior to actually performing the set operation.  This is done
 * to simplify handling of error cases when setting multiple values on
 * a web page.  Without the test routines a web page would need to remember all
 * value and if any set fails would need to restore the previous values.  With
 * the test routines the web page implmentation can test values ahead of time
 * and thus not need to revert if sets fail.
 *
 * NOTE however that test routines are not implemented for all OIDs since the
 * test routines are a late addition to the API and require support from the
 * underlying vars APIs.  The test routines will be implemented as needed.
 * If the test APIs are called on an OID where the producer has not implemented
 * a test routine the test routine will return OID_ERROR_NOT_IMPLEMENTED.  It is
 * up to the user of the API to determine if they consider this to be a pass or
 * fail of the test.
 *
 * Typical usage of the OID API is as follows:
 *
 * 1. An object ID is created and documented in the format aaa,bbb,ccc,ddd and
 * associated with a particular object, for example a network IPV4 address
 * could be given the ID 5,7,1,0.  The oid_t represents this ID and in
 * practice is a 32-bit unsigned integer.
 *
 * 2. The OID helper layer registers (a) helper function(s) internally to
 * make an object available through the OID API.
 *
 * 3. A data-consuming module calls a "get" or "set" function using the OID API
 * to read or write an object using the same OID, previously documented.
 *
 * Example Use Case:
 * A network module is a producer and provides a version 4 IP address as a
 * string.  The producer example code can be seen in "oid_producer_api.h".
 * The consumer is an XML template containing the network IPV4 OID that is
 * parsed and presented to the host-based driver through an embedded web
 * server.  The consumer example code is listed in oid_api.h.
 *
 */


#ifndef OID_API_H
#define OID_API_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 *
 * \brief The object id (OID) is represented by this typedef. It is used during
 * registration, get, set, and test time.
 * A producer or consumer can use the oid_make() macro to initialize an oid_t
 * but does not have to.
 *
 * An object ID is created and documented in the format aaa,bbb,ccc,ddd and
 * associated with a particular object, for example a network IPV4 address
 * could be given the ID 5,7,1,0.  The oid_t represents this ID and in
 * practice is a 32-bit unsigned integer.
 *
 */
typedef uint32_t oid_t;

#define OID_INVALID     0

/*
 * The following #defines are a list of supported OID namspaces.
 * This value will be used when building an OID as a portion of the OID.
 * The purpose of the namespace is to specify the semantics associated with
 * the OID.
 * 
 * IMPORTANT: Namespace value 0 is reserved for defining the 'invalid' OID.  Do not use!
 */

/** \brief System wide namespace */
#define OID_NAMESPACE_SYSTEM        (uint8_t)1

/** \brief Printer Working Group (PWG) Semantic Model namespace. OIDs in this
 * namespace are guaranteed to conform to the PWG Semantic Model defined here:
 * http://www.pwg.org/mfd/index.html . The PWG Semantic Model Schema v2 can be browsed 
 * here: http://www.pwg.org/mfd/navigate/PwgSmRev1-181_System.html 
 */
#define OID_NAMESPACE_PWG_SM        (uint8_t)2

/** Vendor specific namespace extensions
 */
#define OID_NAMESPACE_VENDOR        (uint8_t)3

/** \brief SNMP oids */
#define OID_NAMESPACE_SNMP          (uint8_t)4

/*
 *
 * The following #defines are a list of area IDs associated with an object ID
 * (OID).  This value will be used when building an OID as a portion of the OID.
 *
 * Warning: Do NOT change the values of the #define without careful
 * consideration to consumers of OIDs.
 *
 */

/** \brief Using this ID means the object is associated with
  the print area */
#define OID_AREA_PRINT (uint8_t)1

/** \brief Using this ID means the object is associated with
  the scan area */
#define OID_AREA_SCAN (uint8_t)2

/** \brief Using this ID means the object is associated with
  the fax area */
#define OID_AREA_FAX (uint8_t)3

/** \brief Using this ID means the object is associated with
  the network area */
#define OID_AREA_NETWORK (uint8_t)4

/** \brief Using this ID means the object is associated with
  the network area */
#define OID_AREA_WIFI (uint8_t)5

/** \brief Using this ID means the object is associated with
  the string manager area */
#define OID_AREA_STRING_MGR (uint8_t)6

/** \brief Using this ID means the object is associated with
  the memory manager area */
#define OID_AREA_MEMORY (uint8_t)7

/** \brief Using this ID means the object is associated with
  the platform API area */
#define OID_AREA_PLATFORM (uint8_t)8

/** \brief Using this ID means the object is associated with
  the status area */
#define OID_AREA_STATUS (uint8_t)9

/** \brief Using this ID means the object is associated with
  the http area */
#define OID_AREA_HTTP (uint8_t)10

/** \brief Using this ID means the object is associated with
  the removable storage area */
#define OID_AREA_REM_STORAGE (uint8_t)11

/** \brief Using this ID means the object is associated with
  the consumable manager area */
#define OID_AREA_CONSUMABLE (uint8_t)12

/** \brief Using this ID means the object is associated with
  the USB Device driver area */
#define OID_AREA_USB_DEVICE (uint8_t)13

/** \brief Using this ID means the object will specify if a
 *  feature is supported or not */
#define OID_AREA_FEATURE_SUPPORT (uint8_t)14

/** \brief Using this ID means the object is associated with
    the GCPP API area Google cloud print protocol */
#define OID_AREA_GCPP (uint8_t)15

/** \brief Using this ID means the object is associated with
    the PJL area  */
#define OID_AREA_PJL     (uint8_t)16

/** \brief Using this ID means the object is associated with
 *  the SNMP Printer-MIB area */
#define OID_AREA_SNMP (uint8_t)101


/*
 *
 * The following #defines are a list of data type values associated with
 * the OIDs.  The data type denotes the type that is produced by the
 * registered producer of the OID.  Note that this does not necessarily
 * mean that the consumer of the OID needs to access the OID as the declared
 * type. The OID API will support blind type conversions where possible.
 *
 * Specifically any OID may be accessed as a string.  If a uint32 or
 * bytearray is accessed as a string the value will be blindly converted.
 * In the case of uint32s the uint32 value is converted to and from its
 * decimal string representation.  In the case of a byte_array each octect of
 * the byte_array is converted to and from its hexadecimal string
 * string representation.
 *
 * Conversion between strings and byte_arrays is not supported.
 *
 */

/** \brief The OID is a NULL terminated UTF8 string */
#define OID_TYPE_STRING     (uint8_t)0

/** \brief The OID is a UINT32 */
#define OID_TYPE_UINT32     (uint8_t)1

/** \brief The OID is an array of bytes */
#define OID_TYPE_BYTE_ARRAY (uint8_t)2


/*
 *
 * The following #defines are a list of error_type_t values associated with
 * the OID API calls.  We extend the error_type_t capability within our
 * module by adding more error values that can be returned by an API.
 *
 */

/**
 * \brief This error code indicates that the consumer passed some illegal
 * characters to a string set function */
#define OID_ERROR_ILLEGAL_CHARS (error_type_t)-11

/**
 * \brief This error code indicates that the consumer passed a value that is
 * too big for the producer to handle */
#define OID_ERROR_VALUE_TOO_BIG (error_type_t)-12

/**
 * \brief This error code indicates that the consumer passed a value that is
 * too small for the producer to handle */
#define OID_ERROR_VALUE_TOO_SMALL (error_type_t)-13

/**
 * \brief This error code indicates that the producer had data that was not
 * able to fit in the buffer passed to a "get" function */
#define OID_ERROR_VALUE_CAPPED (error_type_t)-14

/**
 * \brief This error code indicates that the data value passed to the set
 * routine failed producer validation tests */
#define OID_ERROR_VALUE_INVALID (error_type_t)-15

/**
 * \brief This error code indicates that the OID indicated was not a valid
 * registered OID */
#define OID_ERROR_OID_INVALID (error_type_t)-16

/**
 * \brief This error code indicates that the index requested was not within
 * the valid range for the OID */
#define OID_ERROR_INDEX_INVALID (error_type_t)-17

/**
 * \brief This error code indicates that the requested data cannot fit in the
 * buffer provided. */
#define OID_ERROR_BUFFER_TOO_SMALL (error_type_t)-18

/**
 * \brief This error code indicates that the validation routine for the OID
 * has not been implemented. */
#define OID_ERROR_NOT_IMPLEMENTED (error_type_t)-19

/**
 * \brief This error code indicates that an OID value is not available at the requested 
 * index, but values may be available at successive indicies. */ 
#define OID_ERROR_INDEX_UNAVAILABLE (error_type_t)-20

/**
 * \brief This error code indicates that an OID value is not available at this time, 
 *        e.g. because a subsystem has been temporarily disabled. */
#define OID_ERROR_NOT_AVAILABLE (error_type_t)-21

/**
 * \brief This value represents the largest string either the consumer or
 * producer can handle.  It is intended to be used to create static string
 * buffers so that no dynamic memory allocations need to be used.
 */
#define OID_MAX_STRING_SIZE_IN_BYTES (uint32_t)2048


// Some oids are only visable to the end user when they are first set
// the rest of the tiem they are hidden with a special string
// This is the string to hide the text
#define MASKED_TEXT "*******"


/**
 *
 * \brief builds an OID (oid_t) from the parameters for use with the rest of
 * the OID API.  An oid_t just logically "OR"s and shifts the set of
 * parameters given to produce the 32-bit value.
 *
 * \param[in] oid_namespace A uint8_t, one of the OID_NAMESPACE \#defines.
 *
 * \param[in] oid_area_id A uint8_t, one of the OID_AREA \#defines.
 *
 * \param[in] oid_value_id A uint8_t defined by the area registering the
 * OID representing the value identifier.
 *
 * \param[in] object_type A uint8_t defined by the module either registering
 * the OID.  The object_type denotes the type of data supplied by the producer
 * of the OID.
 *
 * \return oid An oid_t generated from the four parameter given.
 * CAUTION: No checking is done on the parameters given.
 *
 */
#define oid_make(  \
        oid_namespace, \
        oid_area_id, \
        oid_value_id, \
        oid_object_type ) \
        (oid_t)( ( ( (uint8_t)oid_namespace )   << 24 ) | \
                 ( ( (uint8_t)oid_area_id )     << 16 ) | \
                 ( ( (uint8_t)oid_value_id )    << 8  ) | \
                 ( ( (uint8_t)oid_object_type ) << 0  ) )


/**
 *
 * \brief Initialize the OID controller.  Must be called before any other OID API
 * functions are called and during system initialization.
 *
 */
void oid_controller_init( void );


/**
 *
 * \brief Global lock of the OID interface.  Locks the OID interface to
 * prevent simultaneous modifications by other oid interface users.
 * Implemented as a mutex.  Typical usage is to lock when getting or setting
 * a group of values.  It is not necessary to lock when getting or setting
 * a single value as this level of locking is handled internally.
 *
 * NOTE: These lock/unlock operators are automatically applied for all OID
 * GET, SET, and TEST operations, i.e. string, uint32, and byte array so
 * there is no need to invoke them in any underlying applications.  However,
 * you may wish to use them as they are nestable.
 *
 */
void oid_interface_lock();


/**
 *
 * \brief Unlock the global OID interface lock.
 */
void oid_interface_unlock();


/**
 *
 * \brief Returns the namespace associated with the oid_t value
 *
 * \param[in] oid A valid oid_t returned by the "oid_make" macro
 *
 * \return uint8_t The same namespace parameter that was used during the
 * oid_make() macro.
 *
 */
inline uint8_t oid_namespace_get( oid_t oid );


/**
 *
 * \brief Returns the area id associated with the oid_t value
 *
 * \param[in] oid A valid oid_t returned by the "oid_make" macro
 *
 * \return uint8_t The same area ID parameter that was used during the
 * oid_make() macro.
 *
 */
inline uint8_t oid_area_id_get( oid_t oid );


/**
 *
 * \brief Returns the value id associated with the oid_t value
 *
 * \param[in] oid A valid oid_t returned by the "oid_make" macro
 *
 * \return uint8_t The same value ID parameter that was used during the
 * oid_make() macro.
 *
 */
inline uint8_t oid_value_id_get( oid_t oid );


/**
 *
 * \brief Returns the object type id associated with the oid_t value
 *
 * \param[in] oid A valid oid_t returned by the "oid_make" macro
 *
 * \return uint8_t The same type ID parameter that was used during the
 * oid_make() macro.
 *
 */
inline uint8_t oid_type_id_get( oid_t oid );


/**
 *
 * \brief Get an oid's string data (a "C" null-terminated character array)
 * value based on the valid oid_t passed.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[in] value A pointer to the data buffer that receives the
 * string data.  This buffer must be at least "max_string_len_in_bytes" long.
 *
 * \param[in] max_string_len_in_bytes The maximum length in bytes that the
 * character buffer can hold, NOT including the NULL terminating character. The
 * buffer won't be filled past this length + 1 for the NULL terminating
 * character.
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 *
 * OID_ERROR_VALUE_CAPPED - producer string was not able to fit into buffer
 *                          max_string_len_in_bytes was too small to fit the
 *                          entire string.
 *
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_get_string( oid_t oid,
        uint32_t index,
        char * value,
        uint32_t max_string_len_in_bytes );


/**
 *
 * \brief Set an oid's string data (a "C" null-terminated character array)
 * value.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[out] value A pointer to the data buffer that contains the
 * string data.  This buffer must be at least "string_len_in_bytes" long.
 *
 * \param[in] string_len_in_bytes The length in bytes that the
 * data buffer ("value") is, NOT the null terminating character.
 * The buffer must be at least string_len_in_bytes + 1 bytes long.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_ILLEGAL_CHARS   - the string data had characters that the
 *                             consumer could not use or interpret
 *
 * OID_ERROR_VALUE_TOO_BIG   - the sring_len_in_bytes was too large
 *                             or the data value represented by the string
 *                             was too large for the intended use
 *
 * OID_ERROR_VALUE_TOO_SMALL - the string_len_in_bytes was too small
 *                             or the data value represented by the
 *                             string was too small for the intended use
 *
 * FAIL                      - bad oid, other error
 *
 */
error_type_t oid_set_string( oid_t oid,
        uint32_t index,
        const char * value,
        uint32_t string_len_in_bytes );


/**
 *
 * \brief Tests to see if the value specified for the OID passes the
 * OID's validation checks.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[out] value A pointer to the data buffer that contains the
 * string data.  This buffer must be at least "string_len_in_bytes" long.
 *
 * \param[in] string_len_in_bytes The length in bytes that the
 * data buffer ("value") is, NOT the null terminating character.
 * The buffer must be at least string_len_in_bytes + 1 bytes long.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_NOT_IMPLEMENTED - the test routine has not been implemented
 *                             for this OID.
 *
 * OID_ERROR_ILLEGAL_CHARS   - the string data had characters that the
 *                             consumer could not use or interpret
 *
 * OID_ERROR_VALUE_TOO_BIG   - the sring_len_in_bytes was too large
 *                             or the data value represented by the string
 *                             was too large for the intended use
 *
 * OID_ERROR_VALUE_TOO_SMALL - the string_len_in_bytes was too small
 *                             or the data value represented by the
 *                             string was too small for the intended use
 *
 * OID_ERROR_OID_INVALID     - the oid parameter contained an INVALID OID.
 *
 * OID_ERROR_INDEX_INVALID   - the oid index parameter is outside the
 *                             supported range for the OID
 *
 * OID_ERROR_VALUE_INVALID   - the value passed in failed the OID specific
 *                              validation check
 *
 * FAIL                      - bad oid, other error
 *
 */
error_type_t oid_test_string( oid_t oid,
        uint32_t index,
        const char * value,
        uint32_t string_len_in_bytes );


/**
 *
 * \brief Gets an oid's uint32_t value.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[out] value A pointer to a uint32_t storage location.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_OID_INVALID     - the oid parameter contained an INVALID OID.
 *
 * OID_ERROR_INDEX_INVALID   - the oid index parameter is outside the
 *                             supported range for the OID
 *
 * FAIL                      - other error
 *
 */
error_type_t oid_get_uint32( oid_t oid,
        uint32_t index,
        uint32_t * value);


/**
 *
 * \brief Sets an oid's uint32_t value.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[in] value A uint32_t containing the value to set.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_OID_INVALID     - the oid parameter contained an INVALID OID.
 *
 * OID_ERROR_INDEX_INVALID   - the oid index parameter is outside the
 *                             supported range for the OID
 *
 * OID_ERROR_VALUE_INVALID   - the value passed is outside the valid range
 *
 * FAIL                      - other error
 *
 */
error_type_t oid_set_uint32( oid_t oid,
        uint32_t index,
        uint32_t value);


/**
 *
 * \brief Tests to see if the value specified for the OID passes the
 * OID's validation checks.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[in] value A uint32_t containing the value to test.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_NOT_IMPLEMENTED - the test routine has not been implemented
 *                             for this OID.
 *
 * OID_ERROR_OID_INVALID     - the oid parameter contained an INVALID OID.
 *
 * OID_ERROR_INDEX_INVALID   - the oid index parameter is outside the
 *                             supported range for the OID
 *
 * OID_ERROR_VALUE_INVALID   - the value passed is outside the valid range
 *
 * FAIL                      - other error
 *
 */
error_type_t oid_test_uint32( oid_t oid,
        uint32_t index,
        uint32_t value);


/**
 *
 * \brief Get an oid's byte array (binary buffer) value.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[out] bytearray A pointer to the data buffer to be filled with the
 * byte array data.  This buffer must be at least "array_len_in_bytes" long.
 *
 * \param[in][out] array_len_in_bytes The length in bytes of the
 * byte array buffer.  On calling this must be set to the length of the
 * passed in buffer.  On successful return this parameter is set to the
 * size of the data returned.  On OID_ERROR_BUFFER_TOO_SMALL this value
 * is set to the size of the buffer needed to contain the data.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_BUFFER_TOO_SMALL - the passed in buffer is too small to contain the
 *                              requested data.  The size of the data is contained
 *                              in the array_len_in_bytes parameter.
 *
 * OID_ERROR_OID_INVALID     - the oid parameter contained an INVALID OID.
 *
 * OID_ERROR_INDEX_INVALID   - the oid index parameter is outside the
 *                             supported range for the OID
 *
 * FAIL                      - other error
 *
 */
error_type_t oid_get_bytearray( oid_t oid,
        uint32_t index,
        uint8_t * bytearray,
        uint32_t * array_len_in_bytes );


/**
 *
 * \brief Set an oid's byte array (binary buffer) value.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[out] bytearray A pointer to the data buffer containing the
 * byte array data.
 *
 * \param[in] array_len_in_bytes The length in bytes of the
 * byte array buffer.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_OID_INVALID     - the oid parameter contained an INVALID OID.
 *
 * OID_ERROR_INDEX_INVALID   - the oid index parameter is outside the
 *                             supported range for the OID
 *
 * OID_ERROR_VALUE_INVALID   - the value passed in failed the OID specific
 *                              validation check
 *
 * FAIL                      - other error
 *
 */
error_type_t oid_set_bytearray( oid_t oid,
        uint32_t index,
        uint8_t * bytearray,
        uint32_t array_len_in_bytes );


/**
 *
 * \brief Tests to see if the value specified for the OID passes the
 * OID's validation checks.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[out] bytearray A pointer to the data buffer containing the
 * byte array data.
 *
 * \param[in] array_len_in_bytes The length in bytes of the
 * byte array buffer.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_NOT_IMPLEMENTED - the test routine has not been implemented
 *                             for this OID.
 *
 * OID_ERROR_OID_INVALID     - the oid parameter contained an INVALID OID.
 *
 * OID_ERROR_INDEX_INVALID   - the oid index parameter is outside the
 *                             supported range for the OID
 *
 * OID_ERROR_VALUE_INVALID   - the value passed in failed the OID specific
 *                              validation check
 *
 * FAIL                      - other error
 *
 */
error_type_t oid_test_bytearray( oid_t oid,
        uint32_t index,
        uint8_t * bytearray,
        uint32_t array_len_in_bytes );


/**
 *
 * \brief These function definitions should only be used by TCL for access
 * to the GET, SET, TEST, LOCK, and UNLOCK capabilities!
 *
 * \param[in] argc - As defined by the C reference manual.
 *
 * \param[in] argv - As defined by the C reference manual.
 *
 * \return A char pointer to a string which can contain any of the errors
 * defined in the above functions above for all OID operations.
 *
 */
char *oid_tcl_get(    int argc, char **argv);
char *oid_tcl_set(    int argc, char **argv);
char *oid_tcl_test(   int argc, char **argv);
char *oid_tcl_lock(   int argc, char **argv);
char *oid_tcl_unlock( int argc, char **argv);


#if 0

/* Example # 1:
 *
 * \example of OID Consumer API in action using a network IPV4 address
 * within an XML file: */

#include <stdint.h>
#include "lassert.h"
#include "error_types.h"
#include "oid_api.h"

static char * xml_displaying_ipv4 = "<config>\
                              <ipv4>#PARSE_HERE OID_NETWORK_IPV4_ADDRESS</ipv4>\
                              </config>";

static char string[OID_MAX_STRING_SIZE_IN_BYTES];

void xml_parser( void )
{
    oid_t oid;
    error_type_t e_res;
    uint8_t module_id;
    uint8_t module_area_id;
    uint8_t module_value_id;
    uint8_t object_type;
    uint32_t string_len;

    /* parse the oid handle from the xml string  */
    oid = xml_parse_oid( xml_displaying_ipv4 );

    /* get the object's string value */
    e_res = oid_get_string( oid, index, string, OID_MAX_STRING_SIZE_IN_BYTES );
    ASSERT( OK == e_res );

    /* concatenate and output xml */
    xml_output( xml_displaying_ipv4, string );

    /* copy a new IP address */
    #define NETWORK_NEW_IPV4 "192.162.56.1"
    strncpy( string, NETWORK_NEW_IPV4, OID_MAX_STRING_SIZE_IN_BYTES );

    string_len = strnlen( string, OID_MAX_STRING_SIZE_IN_BYTES );

    /* set the new IP address */
    e_res = oid_set_string( oid, index, string, string_len );
    ASSERT( OK == e_res );

}

#endif


/*
 * Below This point, the API consists of the actual object ids represented by
 * the last four parameters to the "oid_make()" macro.  All OIDS will be
 * gathered to this one file so we have a common #include file to access all
 * of the OIDS in a system.
 *
 */


#if 0

/* Example # 2:
 * Generating an OID */

#include "oid_api.h"
{
    /* Example usage of the OID macros listed below */
    oid_t oid_mem_size_in_bytes;

    oid_mem_size_in_bytes = oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_MEMORY, 1, OID_TYPE_STRING);

    /* now oid_mem_size_in_bytes is the equal to the macro
     * OID_MEMORY_TOTAL_SIZE_IN_MBYTES either method can be used to generate
     * an OID */

}

#endif


/**
 * Memory Module OIDS
 */
#define OID_MEMORY_TOTAL_SIZE_IN_MBYTES     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_MEMORY, 0, OID_TYPE_UINT32)
#define OID_MEMORY_SYS_HEAP_SIZE_IN_MBYTES  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_MEMORY, 1, OID_TYPE_UINT32)
#define OID_MEMORY_SYS_HEAP_AVAIL_IN_MBYTES oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_MEMORY, 2, OID_TYPE_UINT32)


/**
 * Platform API Module OIDS
 */
#define OID_PLATFORM_PRINT_NAME_STRING      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 0, OID_TYPE_STRING)
#define OID_PLATFORM_SKU                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 1, OID_TYPE_STRING)
#define OID_PLATFORM_PRODUCT_SERIAL_NUM     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 2, OID_TYPE_STRING)
#define OID_PLATFORM_FW_VERSION             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 3, OID_TYPE_STRING)
#define OID_PLATFORM_FORMATTER_SERIAL_NUM   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 4, OID_TYPE_STRING)
#define OID_PLATFORM_COUNTRY_CODE           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 5, OID_TYPE_UINT32)
#define OID_PLATFORM_COUNTRY_NAME           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 6, OID_TYPE_UINT32)
#define OID_PLATFORM_INSTALL_DATE           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 7, OID_TYPE_STRING)
#define OID_PLATFORM_IO_TIMEOUT             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 8, OID_TYPE_UINT32)
#define OID_PLATFORM_UUID                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 9, OID_TYPE_STRING)
#define OID_PLATFORM_UUID_BINARY            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM, 109, OID_TYPE_STRING)
#define OID_PLATFORM_RESTORE_DEFAULT        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM,10, OID_TYPE_STRING)
#define OID_PLATFORM_1284_STRING            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM,12, OID_TYPE_STRING)
#define OID_PLATFORM_USB_MFG_STRING         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM,13, OID_TYPE_STRING)
#define OID_PLATFORM_USB_PROD_STRING        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM,14, OID_TYPE_STRING)
#define OID_PLATFORM_BONJOUR_PROD_STRING    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PLATFORM,15, OID_TYPE_STRING)


/**
 * String Manager API Module OIDS
 */
#define OID_STRING_MGR_LANGUAGE_CODE        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_STRING_MGR, 0, OID_TYPE_UINT32)
#define OID_STRING_MGR_LANGUAGE_IS_RTL      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_STRING_MGR, 1, OID_TYPE_UINT32)
#define OID_STRING_MGR_DEF_LANGUAGE_CODE    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_STRING_MGR, 2, OID_TYPE_UINT32)


/**
 * Print Module OIDS
 */
#define OID_PRINT_LOG_DATE                          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  0, OID_TYPE_STRING) // INDEXED
#define OID_PRINT_LOG_CODE                          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  1, OID_TYPE_UINT32) // INDEXED
#define OID_PRINT_LOG_DISPLAY_ID                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,101, OID_TYPE_UINT32) // INDEXED
#define OID_PRINT_LOG_PAGE_NUM                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,102, OID_TYPE_UINT32) // INDEXED

#define OID_PRINT_INPUT_TRAY_TRAY_ID                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  2, OID_TYPE_UINT32) // INDEXED
#define OID_PRINT_INPUT_TRAY_MEDIA_SIZE             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  3, OID_TYPE_UINT32) // INDEXED
#define OID_PRINT_INPUT_TRAY_MEDIA_TYPE             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  4, OID_TYPE_UINT32) // INDEXED
#define OID_PRINT_INPUT_TYPE_DEFAULT                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  5, OID_TYPE_UINT32)
#define OID_PRINT_NUM_INPUT_TRAYS                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,105, OID_TYPE_UINT32)

#define OID_PRINT_HEAD_MODEL_NAME                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  6, OID_TYPE_STRING)
#define OID_PRINT_HEAD_SERIAL_NUMBER                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  7, OID_TYPE_STRING)
#define OID_PRINT_HEAD_PART_NUMBER                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  8, OID_TYPE_STRING)
#define OID_PRINT_HEAD_INSTALL_DATE                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,  9, OID_TYPE_STRING)
#define OID_PRINT_HEAD_TOTAL_PAGES_PRINTED          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 10, OID_TYPE_STRING)
#define OID_PRINT_HEAD_LAST_DATE_USED               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 11, OID_TYPE_STRING)
#define OID_PRINT_HEAD_TOTAL_MAINT_CYCLES           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 12, OID_TYPE_STRING)
#define OID_PRINT_HEAD_FACTORY_DATA                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 13, OID_TYPE_STRING)

#define OID_PRINT_NUM_TOTAL_PAGES                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,114, OID_TYPE_UINT32)
#define OID_PRINT_NUM_COLOR_PAGES                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,115, OID_TYPE_UINT32)
#define OID_PRINT_NUM_COLOR_BEST                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 14, OID_TYPE_UINT32)
#define OID_PRINT_NUM_COLOR_NORM                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 15, OID_TYPE_UINT32)
#define OID_PRINT_NUM_MONO_PAGES                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT,116, OID_TYPE_UINT32)
#define OID_PRINT_NUM_MONO_BEST                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 16, OID_TYPE_UINT32)
#define OID_PRINT_NUM_MONO_NORM                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 17, OID_TYPE_UINT32)
#define OID_PRINT_SIMPLEX_PAGES                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 117, OID_TYPE_UINT32)
#define OID_PRINT_DUPLEX_PAGES                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 118, OID_TYPE_UINT32)
#define OID_PRINT_PCL_PAGES                         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 119, OID_TYPE_UINT32)


#define OID_PRINT_OUTPUT_TRAY_TRAY_ID               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 18, OID_TYPE_UINT32) // INDEXED
#define OID_PRINT_OUTPUT_TRAY_MEDIA_SIZE            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 19, OID_TYPE_UINT32) // INDEXED
#define OID_PRINT_OUTPUT_TRAY_MEDIA_TYPE            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 20, OID_TYPE_UINT32) // INDEXED
#define OID_PRINT_OUTPUT_JAM_COUNT                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 120, OID_TYPE_UINT32)

/** Print Media Size OIDS */
#define OID_PRINT_MEDIA_SIZE_DEFAULT                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 21, OID_TYPE_UINT32)

/** Letter */
#define OID_PRINT_MEDIA_SIZE_LETTER_ID              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 22, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_LETTER_NUM_BEST        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 23, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_LETTER_NUM_NORM        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 24, OID_TYPE_UINT32)
/** Legal */
#define OID_PRINT_MEDIA_SIZE_LEGAL_ID               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 25, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_LEGAL_NUM_BEST         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 26, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_LEGAL_NUM_NORM         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 27, OID_TYPE_UINT32)
/** Custom */
#define OID_PRINT_MEDIA_SIZE_CUSTOM_ID              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 28, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_CUSTOM_NUM_BEST        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 29, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_CUSTOM_NUM_NORM        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 30, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_CUSTOM_WIDTH           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 31, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_CUSTOM_HEIGHT          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 32, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_SIZE_CUSTOM_UNIT_OF_MEASURE oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 33, OID_TYPE_UINT32)

/** Print Media Type OIDS */
#define OID_PRINT_MEDIA_TYPE_DEFAULT                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 34, OID_TYPE_UINT32)

/** Plain */
#define OID_PRINT_MEDIA_TYPE_PLAIN_ID               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 35, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_TYPE_PLAIN_NUM_BEST         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 36, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_TYPE_PLAIN_NUM_NORM         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 37, OID_TYPE_UINT32)

/** Glossy */
#define OID_PRINT_MEDIA_TYPE_GLOSSY_ID              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 38, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_TYPE_GLOSSY_NUM_BEST        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 39, OID_TYPE_UINT32)
#define OID_PRINT_MEDIA_TYPE_GLOSSY_NUM_NORM        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 40, OID_TYPE_UINT32)

/** Print settings oids */
#define OID_PRINT_JAM_RECOVERY                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 41, OID_TYPE_UINT32)
#define OID_PRINT_GENERAL_STATUS                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 42, OID_TYPE_STRING)
#define OID_PRINT_NUM_MISPICKS                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 44, OID_TYPE_UINT32)
#define OID_PRINT_NUM_JAMS                          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 45, OID_TYPE_UINT32)
#define OID_PRINT_PPM_MONO                          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 46, OID_TYPE_UINT32)
#define OID_PRINT_PPM_COLOR                         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 47, OID_TYPE_UINT32)

#define OID_PRINT_MEDIA_SOURCE_DEFAULT              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PRINT, 50, OID_TYPE_UINT32)

/**
 * Network Module OIDS
 */
#define OID_NETWORK_STATUS                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  0, OID_TYPE_UINT32)
#define OID_NETWORK_MAC_ADDRESS               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  1, OID_TYPE_STRING)
#define OID_NETWORK_MAC_ADDRESS_BYTES         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,101, OID_TYPE_STRING) // uint8_t[]
#define OID_NETWORK_HOST_NAME                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  2, OID_TYPE_STRING)
#define OID_NETWORK_HOST_NAME_ORIGIN          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  3, OID_TYPE_UINT32)
#define OID_NETWORK_HOST_NAME_PREF_ORIGIN     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,113, OID_TYPE_UINT32)
#define OID_NETWORK_HOST_NAME_DEFAULT         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  4, OID_TYPE_STRING)
#define OID_NETWORK_DOMAIN_NAME               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  5, OID_TYPE_STRING)
#define OID_NETWORK_DOMAIN_NAME_ORIGIN        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  6, OID_TYPE_UINT32)
#define OID_NETWORK_DOMAIN_NAME_PREF_ORIGIN   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  7, OID_TYPE_UINT32)
#define OID_NETWORK_DOMAIN_NAME_MANUAL        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  8, OID_TYPE_STRING)
#define OID_NETWORK_RESTORE_HOST_DOMAIN_NAMES oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,  9, OID_TYPE_UINT32)
#define OID_NETWORK_CONNECTION_TIMEOUT        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 10, OID_TYPE_UINT32)
#define OID_NETWORK_LINK_SPEED                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 11, OID_TYPE_UINT32)
#define OID_NETWORK_LINK_CONFIG               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 12, OID_TYPE_UINT32)
#define OID_NETWORK_LPD_ENABLED               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 13, OID_TYPE_UINT32)
#define OID_NETWORK_SLP_ENABLED               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 14, OID_TYPE_UINT32)

/** Network IPV4 area */
#define OID_NETWORK_IPV4_ENABLED              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 15, OID_TYPE_UINT32)
#define OID_NETWORK_IPV4_DOMAIN_NAME          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 16, OID_TYPE_STRING)
#define OID_NETWORK_IPV4_HOST_NAME            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 17, OID_TYPE_STRING)
#define OID_NETWORK_IPV4_LAST_CONFIGURED_BY   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 18, OID_TYPE_UINT32)
#define OID_NETWORK_IPV4_MANUAL_CONFIG        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 19, OID_TYPE_UINT32)
#define OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 20, OID_TYPE_UINT32)
#define OID_NETWORK_IPV4_ALLOW_BOOTP_CONFIG   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 21, OID_TYPE_UINT32)
#define OID_NETWORK_IPV4_ALLOW_AUTOIP_CONFIG  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 22, OID_TYPE_UINT32)
#define OID_NETWORK_IPV4_DHCP_SERVER_ADDR     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 23, OID_TYPE_STRING)
#define OID_NETWORK_IPV4_DHCP_LEASE_TIME      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 24, OID_TYPE_UINT32)
#define OID_NETWORK_IPV4_ADDRESS              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 25, OID_TYPE_STRING)
#define OID_NETWORK_IPV4_SUBNET_MASK          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 26, OID_TYPE_STRING)
#define OID_NETWORK_IPV4_WINS_SERVER          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 27, OID_TYPE_STRING)
#define OID_NETWORK_IPV4_DEFAULT_GATEWAY      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 28, OID_TYPE_STRING)
#define OID_NETWORK_IPV4_WINS_CONFIGURED      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,128, OID_TYPE_UINT32)

/** Network interface info */
#define OID_NETWORK_INTF_COUNT                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,129, OID_TYPE_UINT32)
#define OID_NETWORK_INTF_NAME                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,130, OID_TYPE_STRING)
#define OID_NETWORK_INTF_MAP                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,131, OID_TYPE_UINT32) // app->if_index
#define OID_NETWORK_INTF_ACTIVE               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,132, OID_TYPE_UINT32)
#define OID_NETWORK_REACTIVATE_IFACE          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,151, OID_TYPE_UINT32)
#define OID_NETWORK_RESET                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,152, OID_TYPE_UINT32)

/** Network IPV6 area */
#define OID_NETWORK_IPV6_SUPPORT                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 29, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_ENABLED                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 30, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_DOMAIN_NAME                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 31, OID_TYPE_STRING)
#define OID_NETWORK_IPV6_HOST_NAME                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 32, OID_TYPE_STRING)
#define OID_NETWORK_IPV6_LINK_LOCAL_ADDRESS         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 33, OID_TYPE_STRING)
#define OID_NETWORK_IPV6_LINK_LOCAL_PREFIX_LEN      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 34, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_DHCPV6_ADDRESS             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 35, OID_TYPE_STRING)
#define OID_NETWORK_IPV6_DHCPV6_PREFIX_LEN          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 36, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_DHCPV6_PREFERRED_LIFETIME  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 37, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_DHCPV6_VALID_LIFETIME      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 38, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_STATELESS_ADDRESS          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 39, OID_TYPE_STRING)
#define OID_NETWORK_IPV6_STATELESS_PREFIX_LEN       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 40, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_STATELESS_PREFERRED_LIFETIME oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 41, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_STATELESS_VALID_LIFETIME   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 42, OID_TYPE_UINT32)
#define OID_NETWORK_DHCPV6_ENABLED                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 43, OID_TYPE_UINT32)
#define OID_NETWORK_IPV6_PREFERRED_ADDR             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 144, OID_TYPE_STRING)
#define OID_NETWORK_IPV6_STATUS                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 145, OID_TYPE_UINT32)

/** Network Bonjour/Airprint area */
#define OID_NETWORK_BONJOUR_ENABLED           	 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 44, OID_TYPE_UINT32)
#define OID_NETWORK_BONJOUR_SERVICE_NAME      	 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 45, OID_TYPE_STRING)
#define OID_NETWORK_BONJOUR_DOMAIN_NAME        	 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 46, OID_TYPE_STRING)
#define OID_NETWORK_AIRPRINT_CONFIG_URL          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,153, OID_TYPE_STRING)
#define OID_NETWORK_BONJOUR_SERVICE_NAME_WITH_PREFIX oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK,154, OID_TYPE_STRING)

/** Network Statistics area */
#define OID_NETWORK_STAT_TOT_PACKETS             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 47, OID_TYPE_UINT32)
// total number of frames (packets) received without error, including broadcast, multicast, and unicast
#define OID_NETWORK_STAT_TOT_PACKETS_RECV        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 48, OID_TYPE_UINT32)
// total number of frames (packets) transmitted without error
#define OID_NETWORK_STAT_TOT_PACKETS_XMITTED     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 49, OID_TYPE_UINT32)
#define OID_NETWORK_STAT_BCAST_PACKETS_XMITTED   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 149, OID_TYPE_UINT32)
#define OID_NETWORK_STAT_BCAST_PACKETS_RECV      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 150, OID_TYPE_UINT32)
#define OID_NETWORK_STAT_UNICAST_PACKETS         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 50, OID_TYPE_UINT32)
// unicast frames received without error (does not include broadcast or multicast)
#define OID_NETWORK_STAT_UNICAST_PACKETS_RECV    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 51, OID_TYPE_UINT32)
#define OID_NETWORK_STAT_UNICAST_PACKETS_XMITTED oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 52, OID_TYPE_UINT32)
#define OID_NETWORK_STAT_FRAMING_ERRORS          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 53, OID_TYPE_UINT32)
#define OID_NETWORK_STAT_FRAMING_ERRORS_RECV     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 54, OID_TYPE_UINT32)
#define OID_NETWORK_STAT_FRAMING_ERRORS_XMITTED  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 55, OID_TYPE_UINT32)
#define OID_NETWORK_STAT_BAD_PACKETS             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 56, OID_TYPE_UINT32)
// total number of frames (packets) received with errors
#define OID_NETWORK_STAT_BAD_PACKETS_RECV        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 57, OID_TYPE_UINT32)
// total number of frames (packets) not transmitted because of errors, including collisions
#define OID_NETWORK_STAT_BAD_PACKETS_XMITTED     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 58, OID_TYPE_UINT32)
// number of frames not transmitted because of repeated collisions
#define OID_NETWORK_STAT_COLLISIONS_XMITTED      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 59, OID_TYPE_UINT32)
// number of frames not transmitted because a late collision occurred
#define OID_NETWORK_STAT_COLLISIONS_XMITTED_LATE oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 60, OID_TYPE_UINT32)

/** Network SNMP area */
#define OID_NETWORK_SNMP_ENABLED              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 61, OID_TYPE_UINT32)
#define OID_NETWORK_SNMP_RW_ENABLED           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 62, OID_TYPE_UINT32)
#define OID_NETWORK_SNMP_COMMUNITY_SET_STRING oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 63, OID_TYPE_STRING)
#define OID_NETWORK_SNMP_COMMUNITY_GET_STRING oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 64, OID_TYPE_STRING)
#define OID_NETWORK_SNMP_PUBLIC_ACCESS_EN     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 65, OID_TYPE_UINT32)

/** Network misc */
#define OID_NETWORK_IP_ADDRESS_FROM_CONNECTION oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 66, OID_TYPE_STRING)
#define OID_NETWORK_UAP_SUPPORT                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 67, OID_TYPE_UINT32)
#define OID_NETWORK_WSD_ENABLED                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 68, OID_TYPE_UINT32)
#define OID_NETWORK_IN_USE                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 69, OID_TYPE_UINT32)
#define OID_NETWORK_DNS_ADDR                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 70, OID_TYPE_STRING) 

/** Network Printer MIBs for SNMP Sub-Agent */
#define OID_PRTMIB_TEST_STRING                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 71, OID_TYPE_STRING)
#define OID_PRTMIB_TEST_UNSIGNEDNUM             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 72, OID_TYPE_UINT32)
#define OID_PRTMIB_TEST_SIGNEDNUM               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 73, OID_TYPE_UINT32)
#define OID_PRTMIB_TEST_NUMLIST                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 74, OID_TYPE_UINT32)
#define OID_PRTMIB_TEST_ROWCOUNT                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 75, OID_TYPE_UINT32)
#define OID_PRTMIB_DEFAULT_ORIENTATION          oid_make(OID_NAMESPACE_SNMP,     OID_AREA_PRINT, 76, OID_TYPE_UINT32)

/*** PPPoE */
#define OID_NETWORK_PPPOE_ENABLE               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 77, OID_TYPE_UINT32)
#define OID_NETWORK_PPPOE_STATE                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 78, OID_TYPE_UINT32)
#define OID_NETWORK_PPPOE_USERNAME             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 79, OID_TYPE_STRING)
#define OID_NETWORK_PPPOE_PASSWORD             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_NETWORK, 80, OID_TYPE_STRING)

/**
 * wifi Module OIDS
 */
#define OID_WIFI_INTF_HW_ENABLED           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  0, OID_TYPE_UINT32)
#define OID_WIFI_INTF_ENABLED              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  1, OID_TYPE_UINT32)
#define OID_UAP_INTF_ENABLED               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 201, OID_TYPE_UINT32)
#define OID_WIFI_INTF_SSID                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  2, OID_TYPE_STRING)
#define OID_UAP_INTF_SSID                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 202, OID_TYPE_STRING)
#define OID_WIFI_INTF_SSID_PREFIX          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  3, OID_TYPE_STRING)
#define OID_WIFI_INTF_SEC_MODE             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  4, OID_TYPE_UINT32)
#define OID_WIFI_INTF_WPA_ENCR_MODE        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  5, OID_TYPE_UINT32)
#define OID_WIFI_INTF_WPA_PASSPHRASE       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  6, OID_TYPE_STRING)
#define OID_WIFI_INTF_WEP_AUTH_MODE        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  7, OID_TYPE_UINT32)
#define OID_WIFI_INTF_WEP_KEY_LEN          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  8, OID_TYPE_UINT32)
#define OID_WIFI_INTF_WEP_KEY_1            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI,  9, OID_TYPE_STRING)
#define OID_WIFI_INTF_WEP_KEY_2            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 10, OID_TYPE_STRING)
#define OID_WIFI_INTF_WEP_KEY_3            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 11, OID_TYPE_STRING)
#define OID_WIFI_INTF_WEP_KEY_4            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 12, OID_TYPE_STRING)
#define OID_WIFI_INTF_WEP_CUR_KEY          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 13, OID_TYPE_UINT32)
#define OID_WIFI_INTF_WEP_CUR_KEY_VALUE    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 14, OID_TYPE_STRING)
#define OID_WIFI_INTF_CHANNEL              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 15, OID_TYPE_UINT32)
#define OID_WIFI_INTF_SIGNAL_STRENGTH      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 16, OID_TYPE_UINT32)
#define OID_WIFI_INTF_MAC_ADDR             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 17, OID_TYPE_STRING)
#define OID_WIFI_INTF_BSSID                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 18, OID_TYPE_STRING)
#define OID_WIFI_INTF_BSSID_BYTES          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 118, OID_TYPE_STRING)
#define OID_WIFI_UAP_GET_DHCP_ADDR         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 19, OID_TYPE_STRING)
#define OID_WIFI_INTF_LOCALE               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 20, OID_TYPE_UINT32)
#define OID_WIFI_INTF_NUM_ATTACHED_DEVICES oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 21, OID_TYPE_UINT32)
#define OID_WIFI_HOTSPOT_ENABLED           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 22, OID_TYPE_UINT32)
#define OID_WIFI_HOTSPOT_RESTORE           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 23, OID_TYPE_UINT32)
#define OID_WIFI_HOTSPOT_REBOOT            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 24, OID_TYPE_UINT32)

/** wps feature*/
#define OID_WIFI_INTF_START_WPS            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 30, OID_TYPE_STRING)
#define OID_WIFI_INTF_STOP_WPS             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 31, OID_TYPE_STRING)
#define OID_WIFI_INTF_WPS_PIN              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 32, OID_TYPE_STRING)

#define OID_WIFI_INTF_COMM_MODE            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 40, OID_TYPE_UINT32)
#define OID_WIFI_INTF_SSID_SUFFIX_LEN      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 41, OID_TYPE_UINT32)
#define OID_WIFI_INTF_GENERATE_WPS_PIN     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 42, OID_TYPE_STRING)
#define OID_WIFI_INTF_WLAN_STATUS          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 43, OID_TYPE_UINT32)
#define OID_WIFI_INTF_DOWNLINK_COUNT       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 44, OID_TYPE_UINT32)

/** DHCPD Oids */
#define OID_WIFI_UAP_DHCP_ADDR             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 51, OID_TYPE_STRING)
#define OID_WIFI_UAP_DHCP_ADDR_PREFIX      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 52, OID_TYPE_STRING)
#define OID_WIFI_UAP_DHCP_SUBNET_MASK      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 53, OID_TYPE_STRING)
#define OID_WIFI_UAP_DHCP_START_ADDR       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 54, OID_TYPE_STRING)
#define OID_WIFI_UAP_DHCP_END_ADDR         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 55, OID_TYPE_STRING)
#define OID_WIFI_UAP_DHCP_NUM_ADDRS        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 56, OID_TYPE_UINT32)
#define OID_WIFI_UAP_DHCP_LEASE_TIME       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 57, OID_TYPE_UINT32)

/** MAC Filter */
#define OID_WIFI_INTF_MAC_FILTER_MODE      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 60, OID_TYPE_UINT32)
#define OID_WIFI_INTF_MAC_ADD_FILTER       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 61, OID_TYPE_STRING)
#define OID_WIFI_INTF_MAC_REMOVE_FILTER    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 62, OID_TYPE_STRING)
#define OID_WIFI_INTF_MAC_FILTER_COUNT     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 63, OID_TYPE_UINT32)
#define OID_WIFI_INTF_MAC_FILTER_LIST_UAP  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 64, OID_TYPE_STRING)
#define OID_WIFI_INTF_MAC_FILTER_LIST      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 65, OID_TYPE_STRING)

/** Device Identify */
#define OID_WIFI_DEVICE_IDENTIFY         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_WIFI, 70, OID_TYPE_UINT32)

/**
 * GCPP Module OIDS
 */
/** gcpp get-only boolean **/
#define OID_GCPP_INITIALIZED                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 0, OID_TYPE_UINT32)
/** gcpp get/set booleans **/
#define OID_GCPP_ENABLED                       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 1, OID_TYPE_UINT32)
/** gcpp get-only booleans **/
#define OID_GCPP_REGISTERED                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 2, OID_TYPE_UINT32)
#define OID_GCPP_NETWORK_CONNECTED             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 3, OID_TYPE_UINT32)
#define OID_GCPP_XMPP_CONNECTED                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 4, OID_TYPE_UINT32)
/** gcpp get/set strings **/
#define OID_GCPP_PROXY                         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 5, OID_TYPE_STRING)
#define OID_GCPP_PRINTER_NAME                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 6, OID_TYPE_STRING)
#define OID_GCPP_DISPLAY_NAME                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 7, OID_TYPE_STRING)
/** gcpp get-only strings **/
#define OID_GCPP_PRINTER_ID                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 8, OID_TYPE_STRING)
/** gcpp get/set registration features **/
#define OID_GCPP_REGISTRATION_START_ANONYMOUS  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 9, OID_TYPE_UINT32)
#define OID_GCPP_REGISTRATION_CLAIM_PRINTER    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 10, OID_TYPE_UINT32)
#define OID_GCPP_REGISTRATION_CANCEL           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 11, OID_TYPE_UINT32)
/** gcpp get-only registration strings **/
#define OID_GCPP_REGISTRATION_TOKEN            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 12, OID_TYPE_STRING)
#define OID_GCPP_TOKEN_DURATION                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 13, OID_TYPE_UINT32)
#define OID_GCPP_POLLING_URL                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 14, OID_TYPE_STRING)
#define OID_GCPP_INVITE_URL                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 15, OID_TYPE_STRING)
#define OID_GCPP_INVITE_PAGE_URL               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 16, OID_TYPE_STRING)
#define OID_GCPP_CONFIRMATION_PAGE_URL         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 17, OID_TYPE_STRING)
/** gcpp get/set printer features **/
#define OID_GCPP_PRINTER_ADD                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 18, OID_TYPE_UINT32)
#define OID_GCPP_PRINTER_REMOVE                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 19, OID_TYPE_UINT32)
#define OID_GCPP_PRINTER_UPDATE                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 20, OID_TYPE_UINT32)
#define OID_GCPP_PRINTER_RESET                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 21, OID_TYPE_UINT32)
/** gcpp get/set http proxy values **/
#define OID_GCPP_HTTP_PROXY_ENABLED            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 22, OID_TYPE_UINT32)
#define OID_GCPP_HTTP_PROXY_SERVER             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 23, OID_TYPE_STRING)
#define OID_GCPP_HTTP_PROXY_PORT               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 24, OID_TYPE_UINT32)
#define OID_GCPP_HTTP_PROXY_AUTH_REQ           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 25, OID_TYPE_UINT32)
#define OID_GCPP_HTTP_PROXY_USERNAME           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 26, OID_TYPE_STRING)
#define OID_GCPP_HTTP_PROXY_PASSWORD           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_GCPP, 27, OID_TYPE_STRING)

/** PJL Module OIDS
 *
 */

#define OID_PJL_DEFAULT_COPIES                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 0, OID_TYPE_UINT32)
#define OID_PJL_DEFAULT_DUPLEX                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 1, OID_TYPE_UINT32)
#define OID_PJL_DEFAULT_BINDING                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 2, OID_TYPE_UINT32)
#define OID_PJL_DEFAULT_COLLATED_COPIES          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 3, OID_TYPE_UINT32)
#define OID_PJL_JOBNAME                          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 4, OID_TYPE_STRING)
/* 2013.11.08		by Juny */
#define OID_PJL_ECONOMODE                        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 5, OID_TYPE_UINT32)
#define OID_PJL_TONERDARKNESS                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 6, OID_TYPE_UINT32)
/* USB Status Monitor		2013.12.17		by Juny */
#define OID_PJL_INTRAY1SIZE                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 7, OID_TYPE_UINT32)
#define OID_PJL_INTRAY2SIZE                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 8, OID_TYPE_UINT32)
#define OID_PJL_INTRAY3SIZE                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 9, OID_TYPE_UINT32)
#define OID_PJL_MPTRAYSIZE                       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 10, OID_TYPE_UINT32)
#define OID_PJL_DRUM		                       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 11, OID_TYPE_UINT32)
#define OID_PJL_TONER		                       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 12, OID_TYPE_UINT32)
#define OID_PJL_DUPLEXER		                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 13, OID_TYPE_UINT32)
#define OID_PJL_INTRAY1		                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 14, OID_TYPE_UINT32)
#define OID_PJL_INTRAY2		                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 15, OID_TYPE_UINT32)
#define OID_PJL_INTRAY3		                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 16, OID_TYPE_UINT32)
#define OID_PJL_MPTRAY		                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 17, OID_TYPE_UINT32)
#define OID_PJL_SINDOHUSERNAME                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 18, OID_TYPE_UINT32)
/* for extension		@@ 2013.12.18		by Juny */
/*
#define OID_PJL_INTRAY1QUANTITY                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 18, OID_TYPE_UINT32)
#define OID_PJL_INTRAY2QUANTITY                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 19, OID_TYPE_UINT32)
#define OID_PJL_INTRAY3QUANTITY                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 20, OID_TYPE_UINT32)
#define OID_PJL_INMPTRAYQUANTITY                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 21, OID_TYPE_UINT32)
#define OID_PJL_INTRAY1TYPE              		   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 22, OID_TYPE_UINT32)
#define OID_PJL_INTRAY2TYPE                	   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 23, OID_TYPE_UINT32)
#define OID_PJL_INTRAY3TYPE                	   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 24, OID_TYPE_UINT32)
#define OID_PJL_INMPTRAYTYPE               	   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_PJL, 25, OID_TYPE_UINT32)
*/

#define OID_JOB_INFO_JOBNAME                     OID_PJL_JOBNAME

/**
 * Status Module OIDS
 */
#define OID_STATUS_GENERAL                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_STATUS, 0, OID_TYPE_UINT32)


/**
 * HTTP Module OIDS
 */
#define OID_HTTP_XML_VERSION                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_HTTP, 0, OID_TYPE_STRING)
#define OID_HTTP_ORDER_SUPPLIES_LINK        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_HTTP, 1, OID_TYPE_STRING)
#define OID_HTTP_PASSWORD                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_HTTP, 2, OID_TYPE_STRING)


/**
 * Removable Storage Module OIDS
 */
#define OID_REM_STORAGE_DEVICE_PRESENT      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_REM_STORAGE, 0, OID_TYPE_UINT32)
#define OID_REM_STORAGE_DEVICE_STATUS       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_REM_STORAGE, 1, OID_TYPE_STRING)
#define OID_REM_STORAGE_DEVICE_MEM_CAP      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_REM_STORAGE, 2, OID_TYPE_UINT32)
#define OID_REM_STORAGE_DEVICE_MEM_AVAIL    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_REM_STORAGE, 3, OID_TYPE_UINT32)
#define OID_REM_STORAGE_LIST                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_REM_STORAGE, 4, OID_TYPE_STRING)


/**
 * Consumable Manager Module OIDS - use index field to define
 * which consumable you want (COLOR_BLACK, COLOR_YELLOW,...)
 */
#define OID_CONSUMABLE_MODEL_NAME                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  0, OID_TYPE_STRING)
#define OID_CONSUMABLE_SERIAL_NUMBER             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  1, OID_TYPE_STRING)
#define OID_CONSUMABLE_INSTALL_DATE              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  2, OID_TYPE_STRING)
#define OID_CONSUMABLE_PERCENT_REMAINING         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  3, OID_TYPE_UINT32)
#define OID_CONSUMABLE_PERCENT_REMAINING_STEPPED oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  4, OID_TYPE_UINT32)
#define OID_CONSUMABLE_PERCENT_USED              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  5, OID_TYPE_UINT32)
#define OID_CONSUMABLE_TOTAL_PAGES_PRINTED       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  6, OID_TYPE_UINT32)
#define OID_CONSUMABLE_LOW_LEVEL_INDICATOR       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  7, OID_TYPE_UINT32)
#define OID_CONSUMABLE_LAST_DATE_USED            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  8, OID_TYPE_STRING)
#define OID_CONSUMABLE_SUPPLY_OUT                oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE,  9, OID_TYPE_UINT32)
#define OID_CONSUMABLE_TERNARY                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 10, OID_TYPE_UINT32)
#define OID_CONSUMABLE_ANY_MARKER_TERNARY        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 11, OID_TYPE_UINT32)

/** These OIDS are deprecated and not supported - please remove any
 *  references and replace with OEM oids if necessary */
    #define OID_CONSUMABLES_PRINT_HEAD_SUPPLY_LOCATION   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 200, OID_TYPE_UINT32)
    #define OID_CONSUMABLE_PART_NUMBER           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 201, OID_TYPE_STRING)
    #define OID_CONSUMABLE_PAGES_REMAINING       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 202, OID_TYPE_STRING)
    #define OID_CONSUMABLE_NUMBER_OF_REFILLS     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 203, OID_TYPE_STRING)
    #define OID_CONSUMABLE_REFILL_VENDOR         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 204, OID_TYPE_STRING)
    #define OID_CONSUMABLE_TOTAL_MAINT_CYCLES    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 205, OID_TYPE_UINT32)
    #define OID_CONSUMABLE_FACTORY_DATA          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 206, OID_TYPE_STRING)
    #define OID_CONSUMABLE_SUPPLY_POSITION       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 207, OID_TYPE_UINT32)
    #define OID_CONSUMABLE_COLOR_NAME            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_CONSUMABLE, 208, OID_TYPE_STRING)


/**
 * Scan Module OIDS
 */
/** Scan - Scan Area */
#define OID_ADF_SUPPORT                           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN, 100, OID_TYPE_UINT32)
#define OID_FLATBED_SUPPORT                       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN, 101, OID_TYPE_UINT32)
#define OID_SCAN_DUPLEX_SUPPORT                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN, 102, OID_TYPE_UINT32)
#define OID_SCAN_NUM_SCANS                        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  0, OID_TYPE_UINT32)
#define OID_SCAN_TARGET_HOST_NAME_0               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  1, OID_TYPE_STRING)
#define OID_SCAN_TARGET_HOST_SCAN_PENDING_0       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  2, OID_TYPE_STRING)

/** Scan - Copy Area */
#define OID_SCAN_NUM_COLOR_BEST_COPIES            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  3, OID_TYPE_UINT32)
#define OID_SCAN_NUM_COLOR_NORM_COPIES            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  4, OID_TYPE_UINT32)
#define OID_SCAN_NUM_MONO_BEST_COPIES             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  5, OID_TYPE_UINT32)
#define OID_SCAN_NUM_MONO_NORM_COPIES             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  6, OID_TYPE_UINT32)
// Can use index on set to specify the custom percent - or use seperate oid
#define OID_SCAN_COPY_REDUCE_ENLARGE              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  7, OID_TYPE_UINT32)
#define OID_SCAN_COPY_REDUCE_ENLARGE_PERCENT      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  8, OID_TYPE_UINT32)
#define OID_SCAN_COPY_QUALITY                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN,  9, OID_TYPE_UINT32)
#define OID_SCAN_COPY_LIGHT_DARK                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN, 10, OID_TYPE_UINT32)
#define OID_SCAN_COPY_NUMBER_OF_COPIES            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN, 11, OID_TYPE_UINT32)
#define OID_SCAN_COPY_COLLATE                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN, 12, OID_TYPE_UINT32)
#define OID_SCAN_COPY_DEFAULT_MEDIA_SIZE          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN, 13, OID_TYPE_UINT32)
#define OID_SCAN_COPY_DEFAULT_MEDIA_TYPE          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_SCAN, 14, OID_TYPE_UINT32)


/** Fax - Fax Area */
//Fax phonebook
#define OID_FAX_PHONEBOOK_FIRST_EMPTY_INDEX       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   0, OID_TYPE_UINT32)
#define OID_FAX_PHONEBOOK_FIRST_EMPTY_NAME        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   1, OID_TYPE_STRING)
#define OID_FAX_PHONEBOOK_FIRST_EMPTY_NUMBER      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   2, OID_TYPE_STRING)
#define OID_FAX_PHONEBOOK_RANGE                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   3, OID_TYPE_UINT32)

//Fax upload
#define OID_FAX_UPLOAD_FEATURE                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   5, OID_TYPE_UINT32)
#define OID_FAX_UPLOAD_HOSTNAME                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   6, OID_TYPE_STRING)
#define OID_FAX_UPLOAD_BACKUP                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   7, OID_TYPE_UINT32)

//Fax config
    //SYS
#define OID_FAX_CONFIG_SYS_TELECOM_VER            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   8, OID_TYPE_STRING)
#define OID_FAX_CONFIG_SYS_COMPANY_NAME           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,   9, OID_TYPE_STRING)
#define OID_FAX_CONFIG_SYS_PHONE_NUMBER           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  10, OID_TYPE_STRING)
#define OID_FAX_CONFIG_SYS_SPEED                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  11, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_MODEM_VOL              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  12, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_RINGER_VOL             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  13, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_AUTOPRINT_CONFIRM      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  14, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_AUTOPRINT_ERROR        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  15, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_AUTOPRINT_T30          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  16, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_AUTOPRINT_ACTIVITY     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  17, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_DEDICATED_FAXLINE      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  18, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_ECM_MODE               oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  19, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_EXT_PHONE              oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  20, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_THUMBNAILONCALL        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  21, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_SETUP_COMPLETE         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  22, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SYS_ADDITIONAL_PHONE       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  23, OID_TYPE_UINT32)

    //SEND
#define OID_FAX_CONFIG_SEND_NUMBER_PREFIX         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  24, OID_TYPE_STRING)
#define OID_FAX_CONFIG_SEND_LIGHTERDARKER         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  25, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_RESOLUTION            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  26, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_GLASSSIZE             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  27, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_DIALINGMODE           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  28, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_CONFIRMFAXNUMBER      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  29, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_BILLINGCODE           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  30, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_NUMBERPRIFIX_USED     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  31, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_PRINTTOFAX            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  32, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_REDIALIFBUSY          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  33, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_REDIALIFERROR         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  34, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_REDIALIFNOANS         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  35, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_SEND_SCANANDFAX            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  36, OID_TYPE_UINT32)

    //RECEIVE
#define OID_FAX_CONFIG_RECEIVE_ANS_MODE           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  37, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_RINGTIME           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  38, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_RINGSTOANS         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  39, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_DISTINCRING        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  40, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_FORWARDNUMBER      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  41, OID_TYPE_STRING)
#define OID_FAX_CONFIG_RECEIVE_OUTPUTPLEX         oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  42, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_ALLOWREPRINT       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  43, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_BLOCKJUNKFAX       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  44, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_DETECTDIALTONE     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  45, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_FITTOPAGE          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  46, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_PRIVATERECEIVE     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  47, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_STAMPFAX           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  48, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_PRINTCHECKMEDIA    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  49, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_MEDIASIZE          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  50, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_BACKUP             oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  51, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_FORWARD            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  52, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_FORWARDSTART       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  53, OID_TYPE_UINT32)
#define OID_FAX_CONFIG_RECEIVE_FORWARDSTOP        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FAX,  54, OID_TYPE_UINT32)



/**
 * USB Device Driver Module OIDS
 */
#define OID_USB_DEVICE_VID                        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_USB_DEVICE, 0, OID_TYPE_STRING)
#define OID_USB_DEVICE_PID                        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_USB_DEVICE, 1, OID_TYPE_STRING)


/**
 *  if a feature is supported or not
 */
#define OID_PRINT_HAVE_PCL                        oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 0, OID_TYPE_UINT32)
#define OID_PRINT_HAVE_COLOR                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 1, OID_TYPE_UINT32)
#define OID_NETWORK_SUPPORT                       oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 2, OID_TYPE_UINT32)
#define OID_WIFI_INFT_SUPPORT                     oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 3, OID_TYPE_UINT32)
#define OID_FAX_SUPPORT                           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 4, OID_TYPE_UINT32)
#define OID_DUPLEXER_SUPPORT                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 5, OID_TYPE_UINT32)
#define OID_SCAN_SUPPORT                          oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 6, OID_TYPE_UINT32)
#define OID_RTC_SUPPORT                           oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 7, OID_TYPE_UINT32)
#define OID_PRINT_COLLATE_SUPPORT                 oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 8, OID_TYPE_UINT32)
#define OID_PRINT_BIND_SUPPORT                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 9, OID_TYPE_UINT32)
#define OID_PRINT_SORT_SUPPORT                    oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 10, OID_TYPE_UINT32)
#define OID_PRINT_STAPLE_SUPPORT                  oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 11, OID_TYPE_UINT32)
#define OID_PRINT_PUNCH_SUPPORT                   oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 12, OID_TYPE_STRING)
#define OID_ETHERNET_SUPPORT                      oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 13,OID_TYPE_UINT32)
#define OID_NETWORK_EXTERNAL_INTERFACE            oid_make(OID_NAMESPACE_SYSTEM, OID_AREA_FEATURE_SUPPORT, 14,OID_TYPE_UINT32)


/** SNMP Module OIDS (via net-snmp sub-agentx)
 *    as defined in Printer-MIB.txt
 */

#define OID_SNMP_prtConsoleOnTime                   oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 3, OID_TYPE_UINT32)
#define OID_SNMP_prtConsoleOffTime                  oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 4, OID_TYPE_UINT32)
#define OID_SNMP_prtGeneralReset                    oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 5, OID_TYPE_UINT32)
#define OID_SNMP_prtGeneralCurrentLocalization      oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 6, OID_TYPE_UINT32)
#define OID_SNMP_prtGeneralCurrentOperator          oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 7, OID_TYPE_UINT32)
#define OID_SNMP_prtGeneralServicePerson            oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 8, OID_TYPE_UINT32)
#define OID_SNMP_prtGeneralPrinterName              oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 9, OID_TYPE_UINT32)
#define OID_SNMP_prtGeneralSerialNumber             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 10, OID_TYPE_UINT32)
#define OID_SNMP_prtInputDefaultIndex                 /* alias */ OID_CONSUMABLE_SUPPLY_CLASS
#define OID_SNMP_prtInputMediaDimFeedDirDeclared    oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 12, OID_TYPE_UINT32)
#define OID_SNMP_prtInputMaxCapacity                oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 13, OID_TYPE_UINT32)
#define OID_SNMP_prtInputCurrentLevel               oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 14, OID_TYPE_UINT32)
#define OID_SNMP_prtInputMediaName                    /* alias */ OID_PRINT_MEDIA_SIZE_DEFAULT
#define OID_SNMP_prtInputName                       oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 16, OID_TYPE_UINT32)
#define OID_SNMP_prtInputSecurity                   oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 17, OID_TYPE_UINT32)
#define OID_SNMP_prtInputMediaWeight                oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 18, OID_TYPE_UINT32)
#define OID_SNMP_prtInputMediaType                  oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 19, OID_TYPE_UINT32)
#define OID_SNMP_prtInputMediaColor                 oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 20, OID_TYPE_UINT32)
#define OID_SNMP_prtInputMediaFormParts             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 21, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputDefaultIndex              oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 22, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputMaxCapacity               oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 23, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputRemainingCapacity         oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 24, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputName                      oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 25, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputSecurity                  oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 26, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputMaxDimFeedDir             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 27, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputMaxDimXFeedDir            oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 28, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputMinDimFeedDir             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 29, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputMinDimXFeedDir            oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 30, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputStackingOrder             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 31, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputPageDeliveryOrientation   oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 32, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputBursting                  oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 33, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputDecollating               oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 34, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputPageCollated              oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 35, OID_TYPE_UINT32)
#define OID_SNMP_prtOutputOffsetStacking            oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 36, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerDefaultIndex              oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 37, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerLifeCount                 oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 40, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerSuppliesTable             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP,140, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerSuppliesIndex             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 41, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerSuppliesMarkerIndex       oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP,141, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerColorantMarkerIndex       oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 42, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerSuppliesClass               /* alias */ OID_CONSUMABLE_SUPPLY_CLASS
#define OID_SNMP_prtMarkerSuppliesType              oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 44, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerSuppliesDescription       oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 45, OID_TYPE_STRING)
#define OID_SNMP_prtMarkerSuppliesSupplyUnit        oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 46, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerSuppliesMaxCapacity       oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 47, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerSuppliesLevel             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 48, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerColorantIndex             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 49, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerColorantRole              oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 51, OID_TYPE_UINT32)
#define OID_SNMP_prtMarkerColorantValue             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 52, OID_TYPE_STRING)
#define OID_SNMP_prtMarkerColorantTonality          oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 53, OID_TYPE_UINT32)
#define OID_SNMP_prtMediaPathDefaultIndex           oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 54, OID_TYPE_UINT32)
#define OID_SNMP_prtChannelCurrentJobCntlLangIndex  oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 55, OID_TYPE_UINT32)
#define OID_SNMP_prtChannelDefaultPageDescLangIndex oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 56, OID_TYPE_UINT32)
#define OID_SNMP_prtChannelState                    oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 57, OID_TYPE_UINT32)
#define OID_SNMP_prtChannelIfIndex                  oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 58, OID_TYPE_UINT32)
#define OID_SNMP_prtInterpreterDefaultOrientation   oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 59, OID_TYPE_UINT32)
#define OID_SNMP_prtInterpreterDefaultCharSetIn     oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 60, OID_TYPE_UINT32)
#define OID_SNMP_prtInterpreterDefaultCharSetOut    oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 61, OID_TYPE_UINT32)
#define OID_SNMP_prtConsoleLocalization             oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 62, OID_TYPE_UINT32)
#define OID_SNMP_prtConsoleDisable                  oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 63, OID_TYPE_UINT32)
#define OID_SNMP_prtConsoleDisplayBufferText        oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 64, OID_TYPE_UINT32)
#define OID_SNMP_prtAlertIndex                      oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 65, OID_TYPE_UINT32)
#define OID_SNMP_prtAuxiliarySheetStartupPage       oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 66, OID_TYPE_UINT32)
#define OID_SNMP_prtAuxiliarySheetBannerPage        oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 67, OID_TYPE_UINT32)
#define OID_SNMP_prtInputMediaLoadTimeout           oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 68, OID_TYPE_UINT32)

// SNMP Marker (Consumables)

#define OID_CONSUMABLE_SUPPLY_CLASS                 oid_make(OID_NAMESPACE_SNMP, OID_AREA_CONSUMABLE, 1, OID_TYPE_UINT32)

// Job pjl (job) info

#define OID_SNMP_PJL_DEFAULT_COPIES                   /* alias */ OID_PJL_DEFAULT_COPIES
#define OID_SNMP_PJL_DEFAULT_DUPLEX                   /* alias */ OID_PJL_DEFAULT_DUPLEX
#define OID_SNMP_PJL_DEFAULT_BINDING                  /* alias */ OID_PJL_DEFAULT_BINDING

// Job Info (pml-like)

#define OID_JOB_INFO_PAGES_PROCESSED                  /* alias */ OID_PRINT_NUM_TOTAL_PAGES
#define OID_JOB_INFO_PAGES_PROCESSED_MONO             /* alias */ OID_PRINT_NUM_MONO_PAGES
#define OID_JOB_INFO_PAGES_PROCESSED_COLOR            /* alias */ OID_PRINT_NUM_COLOR_PAGES
#define OID_JOB_INFO_NAME1                          oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 70, OID_TYPE_STRING) /* [job-indexed] */
#define OID_JOB_INFO_NAME2                          oid_make(OID_NAMESPACE_SNMP, OID_AREA_SNMP, 71, OID_TYPE_STRING) /* [job-indexed] */
#define OID_JOB_INFO_PAGES_PRINTED                    /* alias */ OID_PRINT_LOG_PAGE_NUM        /*[job-indexed]*/
#define OID_JOB_INFO_OUTCOME                          /* alias */ OID_PRINT_GENERAL_STATUS
#define OID_JOB_INFO_OUTBINS_USED                     /* alias */ OID_PRINT_OUTPUT_TRAY_TRAY_ID /*[job-indexed]*/
#define OID_JOB_INFO_PHYSICAL_OUTBINS_USED            /* alias */ OID_PRINT_OUTPUT_TRAY_TRAY_ID /*[job-indexed]*/
#define OID_ERROR1_CODE                               /* alias */ OID_PRINT_LOG_CODE            /*[job-indexed]*/


// add oem-specific oids in oid_vendor_api.h..
#include "oid_vendor_api.h"

#ifdef __cplusplus
}
#endif

#endif

