/*
 * ============================================================================
 * (C) Copyright 2009-2010  Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef OID_PRODUCER_API_H
#define OID_PRODUCER_API_H


#include <stdint.h> 
#include "error_types.h"
#include "oid_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 
 * \file oid_producer_api.h
 *
 * \brief This is an internal OID API for registration of back-end OID
 * callback functions.  This interface allows for registration of a module's
 * data (a producer).  This makes the data accessible through the front-end
 * OID API (oid_api.h) for the consumer.
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 * 
 * \brief A producer registers an object that is a null-terminated string.
 * This function pointer type is used to get the data value on the back-end.
 *
 * \param[in] oid A valid oid_t representing the object
 * registered with this callback function
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[out] data_ptr The data_ptr char array will be filled with the NULL
 * terminated string representing the object ID given with the callback at
 * registration time.
 * The caller is responsible for allocating the memory and the array should be
 * large enough to hold the data requested.  In the case that
 * "max_string_len_in_bytes" is less than the actual back-end string length,
 * then the data_ptr MUST not be filled past
 * data_ptr + max_string_len_in_bytes + 1 including the NULL termination 
 * character.
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
typedef error_type_t ( *oid_callback_get_string_t )( oid_t oid,
	uint32_t index,
        char * data_ptr,
        uint32_t max_string_len_in_bytes );


/**
 * 
 * \brief A producer registers an object that is a null-terminated string.
 * This function pointer type is used to set the data value on the back-end.
 *
 * \param[in] oid A valid oid_t representing the object
 * registered with this callback function
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[in] data A pointer to a null-terminated string ("C" character array)
 * representing the object ID given with the callback at registration time.
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
 * FAIL 					 - bad oid, other error
 *
 */
typedef error_type_t ( *oid_callback_set_string_t )( oid_t oid,
	uint32_t index,
        const char * data,
        uint32_t string_len_in_bytes );

/**
 * 
 * \brief A producer registers an object that is a null-terminated string.
 * This function pointer type is used to test the data value on the back-end.
 *
 * \param[in] oid A valid oid_t representing the object
 * registered with this callback function
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 *
 * \param[in] data A pointer to a null-terminated string ("C" character array)
 * representing the object ID given with the callback at registration time.
 *
 * \param[in] string_len_in_bytes The length in bytes that the 
 * data buffer ("value") is, NOT the null terminating character.
 * The buffer must be at least string_len_in_bytes + 1 bytes long.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * OID_ERROR_NOT_IMPLEMENTED - indicates that there is no validation
 *                             routine for this oid string object
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
 * FAIL 					 - bad oid, other error
 *
 */
typedef error_type_t ( *oid_callback_test_string_t )( oid_t oid,
	uint32_t index,
        const char * data,
        uint32_t string_len_in_bytes );

/**
 *
 * \brief A producer registers a get object that is a uint32.
 * This function pointer type is used to get the data value on the back-end.
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
 * FAIL                      - other error
 *
 */
typedef error_type_t ( *oid_callback_get_uint32_t )( oid_t oid,
        uint32_t index,
        uint32_t * value);


/**
 *
 * \brief A producer registers a set object that is a uint32.
 * This function pointer type is used to set the data value on the back-end.
 *
 * \brief Gets an oid's uint32_t value.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 * 
 * \param[out] value A uint32_t passed value.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * FAIL                      - other error
 *
 */
typedef error_type_t ( *oid_callback_set_uint32_t )( oid_t oid,
        uint32_t index,
        uint32_t value);


/**
 *
 * \brief A producer registers a test object that is a uint32.
 * This function pointer type is used to test the data value on the back-end.
 *
 * \brief Gets an oid's uint32_t value.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index The index of the OID being requested. Passed as 0 for
 * unindexed OIDs.
 * 
 * \param[out] value A uint32_t passed value.
 *
 * \return One of the error_type_t return codes:
 * OK                        - successful
 *
 * FAIL                      - other error
 *
 */
typedef error_type_t ( *oid_callback_test_uint32_t )( oid_t oid,
        uint32_t index,
        uint32_t value);

/**
 *
 * \brief A producer registers a get byte array (binary buffer) value.
 * This function pointer type is used to get the data value on the back-end.
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
typedef error_type_t ( *oid_callback_get_bytearray_t )( oid_t oid,
        uint32_t index,
        uint8_t * bytearray,
        uint32_t * array_len_in_bytes );

/**
 *
 * \brief A producer registers a set byte array (binary buffer) value.
 * This function pointer type is used to set the data value on the back-end.
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
typedef error_type_t ( *oid_callback_set_bytearray_t )( oid_t oid,
        uint32_t index,
        uint8_t * bytearray,
        uint32_t array_len_in_bytes );


/**
 *
 * \brief A producer registers a test byte array (binary buffer) value.
 * This function pointer type is used to test the data value on the back-end.
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
typedef error_type_t ( *oid_callback_test_bytearray_t )( oid_t oid,
        uint32_t index,
        uint8_t * bytearray,
        uint32_t array_len_in_bytes );


/**
 * 
 * \brief Retrieve the producer's get back-end function or return
 * NULL if no back-end function is registered with this OID value
 *
 * \param[in] oid A valid oid_t created using the
 * "oid_make()" function
 *
 * \return A valid function pointer to the oid's back-end function
 * or NULL if no back-end function is registered with this OID value
 *
 */
void * oid_producer_get_func( oid_t oid );


/**
 * 
 * \brief Retrieve the producer's set back-end function or return
 * NULL if no back-end function is registered with this OID value
 *
 * \param[in] oid A valid oid_t created using the
 * "oid_make()" function
 *
 * \return A valid function pointer to the oid's back-end function
 * or NULL if no back-end function is registered with this OID value
 *
 */
void * oid_producer_set_func( oid_t oid );


/**
 * 
 * \brief Retrieve the producer's test back-end function or return
 * NULL if no back-end function is registered with this OID value
 *
 * \param[in] oid A valid oid_t created using the
 * "oid_make()" function
 *
 * \return A valid function pointer to the oid's back-end function
 * or NULL if no back-end function is registered with this OID value
 *
 */
void * oid_producer_test_func( oid_t oid );


/**
 *
 * \brief This function allows a producer to register a set of
 * callback objects with the oid manager
 *
 * \param[in] oid A valid oid_t created using the
 * "oid_make()" function
 *
 * \param[in] get_callback A valid pointer to a callback function to get the
 * object's data value on the back-end
 *
 * \param[in] set_callback A valid pointer to a callback function to set the
 * object's data value on the back-end
 *
 * \param[in] test_callback A valid pointer to a callback function to test the
 * object's data value on the back-end
 *
 * \return One of the error_type_t return codes:
 * OK, If successful, otherwise one of the following error codes:
 * FAIL
 *
 */
error_type_t oid_register_callbacks( oid_t oid,
        void * get_callback,
        void * set_callback,
        void * test_callback );


/**
 *
 * \brief Register the back-end functions for each module with the OID API
 *
 */
void oid_producer_register( void );


/**
 * 
 * \brief The backend oid module function for a module without a get function.
 * This function should be used in place of a valid function and will always
 * return FAIL.
 *
 * \return error_type_t FAIL is always returned
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_null_get( void );


/**
 * 
 * \brief The backend oid module function for a module
 * without a set function.  This function will be registered in place
 * of one that a module would have registered and will always return fail.
 *
 * \return error_type_t FAIL is always returned
 * FAIL 					 - bad oid, other error
 *
 */
error_type_t oid_module_null_set( void );


/**
 * 
 * \brief The backend oid module function for a module
 * without a test function.  This function will be registered in place
 * of one that a module would have registered and will always return unimplemented.
 *
 * \return error_type_t OID_ERROR_NOT_IMPLEMENTED is always returned
 *
 */
error_type_t oid_module_null_test( void );

#ifndef UNUSED_VAR 
 /*!
 * \def UNUSED_VAR(x) 
 * a simple macro that will turn off the unused var compiler warning for a given var
 **/
  #define UNUSED_VAR(x) ((x) = (x))  
#endif


#if 0

/** \example usage of the producer API in terms of an imaginary network module */

#include <stdint.h>
#include "lassert.h"
#include "error_types.h"
#include "oid_api.h"
#include "oid_producer_api.h"


/* OID registration and helper functions */
static error_type_t network_ipv4_oid_callback_set( oid_t oid,
        char * data,
        uint32_t string_len_in_bytes )
{
    network_ipv4_address_t new_address;
    error_type e_res;

    e_res = network_ipv4_verify_oid(oid);
    if ( OK != e_res )
    {
        return FAIL;
    }

    e_res = network_ipv4_parse_address_from_string( data,
            &new_address,
            string_len_in_bytes );
    if ( OK != e_res )
    {
        return FAIL;
    }

    /* new_address is filled with the ip address from "data" */
    network_ipv4_address_internal_set( &new_address );

    return OK;
}

static error_type_t network_ipv4_oid_callback_get( oid_t oid,
        char * data_ptr,
        uint32_t max_string_len_in_bytes )
{
    network_ipv4_address_t current_address;
    error_type_t e_res;

    ASSERT( NULL != data_ptr );

    e_res = network_ipv4_verify_oid(oid);
    if ( OK != e_res )
    {
        return FAIL;
    }

    current_address = network_ipv4_address_internal_get();
    /* convert the ipv4 address struct to a string keeping in mind the max len */
    network_ipv4_address_to_string( &current_address,
            data_ptr,
            max_string_len_in_bytes );

    /* the string is now populated and we can return so the consumer can use the
     * data */

    return OK;
}


void oid_network_init( void )
{
    error_type_t e_res;
    oid_t oid;

    oid = OID_NETWORK_IPV4_ADDRESS;

    /* Register as a producer with oid manager */ 
    e_res = oid_register_string( oid,
        network_ipv4_oid_callback_get,
        network_ipv4_oid_callback_set);

    ASSERT( OK == e_res );

}
#endif


#ifdef __cplusplus
}
#endif

#endif
