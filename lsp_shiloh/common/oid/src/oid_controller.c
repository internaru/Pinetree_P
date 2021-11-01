/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_controller.c
 *
 * \brief This file implements the OID API.  See oid_api.h for details. 
 * 
 **/

#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "posix_ostools.h"
#include "utils.h"
#include "memAPI.h"
#include "logger.h"
#include "lassert.h"
#include "error_types.h"
#include "oid_api.h"
#include "oid_producer_api.h"

#ifdef HAVE_UNIT_TEST
#include "oid_unit_test.h"
#include "unit_test_api.h"
#endif

/**
 *
 * \brief This is the OID mutex data structure declaration and put/get
 * function definitions
 *
 * \param[in] none Neither interface lock or unlock pass any parameters
 *
 * \return none Neither interface lock or unlock return any parameters
 *
 */

static pthread_mutex_t oidMutex;

void oid_interface_lock( void )
{
    int px_status;

    px_status = pthread_mutex_lock( &oidMutex );
    XASSERT( px_status==0, px_status );
}


void oid_interface_unlock( void )
{
    int px_status;

    px_status = pthread_mutex_unlock( &oidMutex );
    XASSERT( px_status==0, px_status );
}

/**
 *
 * \brief This is the OID subsystem init entry point.  The mutex is
 * created, then used to protect the entire OID registration process
 * during the call to each and every component registration entry point
 *
 * \param[in] none
 *
 * \return nothing
 *
 */

void oid_controller_init( void )
{
    int px_status;

    px_status = posix_mutex_init( &oidMutex );
    XASSERT( px_status==0, px_status );
    
    oid_interface_lock();

    #ifdef HAVE_UNIT_TEST
    error_type_t test_res;
    #endif

    oid_producer_register();

    #ifdef HAVE_UNIT_TEST
    test_res = unit_test_register_testcase("oid", oid_unit_test);
    XASSERT( ( OK == test_res ), test_res );
    #endif

    oid_interface_unlock();
}


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
inline uint8_t oid_namespace_get( oid_t oid )
{
    return (uint8_t)( oid >> 24 );
}


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
inline uint8_t oid_area_id_get( oid_t oid )
{
    return (uint8_t)( oid >> 16 );
}


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
inline uint8_t oid_value_id_get( oid_t oid )
{
    return (uint8_t)( oid >> 8 );
}


/**
 *
 * \brief Returns the object id associated with the oid_t value
 *
 * \param[in] oid A valid oid_t returned by the "oid_make" macro 
 *
 * \return uint8_t The same object ID parameter that was used during the
 * oid_make() macro.
 *
 */
inline uint8_t oid_type_id_get( oid_t oid )
{
    return (uint8_t)( oid >> 0 );
}


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
        uint32_t max_string_len_in_bytes )
{
    oid_callback_get_string_t get_callback;
    error_type_t e_res;

    oid_interface_lock();

    e_res = OK;

    if ( ( NULL == value ) ||
         ( 0 == max_string_len_in_bytes ) 
//         || ( max_string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
          )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        get_callback = oid_producer_get_func( oid );

        if ( NULL == get_callback || ( OID_TYPE_STRING != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }
    
    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = get_callback( oid, index, value, max_string_len_in_bytes );
    }

    oid_interface_unlock();

    return e_res;
}

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
        uint32_t string_len_in_bytes )
{
    oid_callback_set_string_t set_callback;
    error_type_t e_res;

    oid_interface_lock();
    
    e_res = OK;

    if ( NULL == value )
    {
        e_res = FAIL;
    }
    if ( ( OK == e_res ) && ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES ) )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }
    if ( ( OK == e_res ) && ( value[ string_len_in_bytes ] != '\0' ) )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        set_callback = oid_producer_set_func( oid );

        if ( NULL == set_callback || ( OID_TYPE_STRING != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = set_callback( oid, index, value, string_len_in_bytes );
    }

    oid_interface_unlock();

    return e_res;
}

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
        uint32_t string_len_in_bytes )
{
    oid_callback_test_string_t test_callback;
    error_type_t e_res;

    oid_interface_lock();
    
    e_res = OK;

    if ( NULL == value )
    {
        e_res = FAIL;
    }
    if ( ( OK == e_res ) && ( 0 == string_len_in_bytes ) )
    {
        e_res = OID_ERROR_VALUE_TOO_SMALL;
    }
    if ( ( OK == e_res ) && ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES ) )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }
    if ( ( OK == e_res ) && ( value[ string_len_in_bytes ] != '\0' ) )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        test_callback = oid_producer_test_func( oid );

        if ( NULL == test_callback || ( OID_TYPE_BYTE_ARRAY != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = test_callback( oid, index, value, string_len_in_bytes );
    }

    oid_interface_unlock();

    return e_res;
}

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
        uint32_t * value)

{
    oid_callback_get_uint32_t get_callback;
    error_type_t e_res;

    oid_interface_lock();

    e_res = OK;

    if ( NULL == value )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        get_callback = oid_producer_get_func( oid );

        if ( NULL == get_callback || ( OID_TYPE_UINT32 != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }
    
    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = get_callback( oid, index, value );
    }

    oid_interface_unlock();

    return e_res;
}

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
        uint32_t value)
{
    oid_callback_set_uint32_t set_callback;
    error_type_t e_res;

    oid_interface_lock();
    
    e_res = OK;

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        set_callback = oid_producer_set_func( oid );

        if ( NULL == set_callback || ( OID_TYPE_UINT32 != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = set_callback( oid, index, value );
    }

    oid_interface_unlock();

    return e_res;
}

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
        uint32_t value)
{
    oid_callback_test_uint32_t test_callback;
    error_type_t e_res;

    oid_interface_lock();
    
    e_res = OK;

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        test_callback = oid_producer_test_func( oid );

        if ( NULL == test_callback || ( OID_TYPE_UINT32 != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = test_callback( oid, index, value );
    }

    oid_interface_unlock();

    return e_res;
}

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
        uint32_t * array_len_in_bytes )
{
    oid_callback_get_bytearray_t get_callback;
    error_type_t e_res;

    oid_interface_lock();

    e_res = OK;

    if ( ( NULL == bytearray ) ||
         ( NULL == array_len_in_bytes ) )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        get_callback = oid_producer_get_func( oid );

        if ( NULL == get_callback || ( OID_TYPE_BYTE_ARRAY != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }
    
    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = get_callback( oid, index, bytearray, array_len_in_bytes );
    }

    oid_interface_unlock();

    return e_res;
}

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
        uint32_t array_len_in_bytes )
{
    oid_callback_set_bytearray_t set_callback;
    error_type_t e_res;

    oid_interface_lock();
    
    e_res = OK;

    if ( NULL == bytearray )
    {
        e_res = FAIL;
    }
    if ( ( OK == e_res ) && ( 0 == array_len_in_bytes ) )
    {
        e_res = OID_ERROR_VALUE_TOO_SMALL;
    }

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        set_callback = oid_producer_set_func( oid );

        if ( NULL == set_callback || ( OID_TYPE_BYTE_ARRAY != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = set_callback( oid, index, bytearray, array_len_in_bytes );
    }

    oid_interface_unlock();

    return e_res;
}

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
        uint32_t array_len_in_bytes )
{
    oid_callback_test_bytearray_t test_callback;
    error_type_t e_res;

    oid_interface_lock();
    
    e_res = OK;

    if ( NULL == bytearray )
    {
        e_res = FAIL;
    }
    if ( ( OK == e_res ) && ( 0 == array_len_in_bytes ) )
    {
        e_res = OID_ERROR_VALUE_TOO_SMALL;
    }

    if ( OK == e_res )
    {
        /* attempt to retrieve a back-end function for this oid */
        test_callback = oid_producer_test_func( oid );

        if ( NULL == test_callback || ( OID_TYPE_BYTE_ARRAY != oid_type_id_get( oid )) )
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        /* use the retrieved back-end function pointer */
        e_res = test_callback( oid, index, bytearray, array_len_in_bytes );
    }

    oid_interface_unlock();

    return e_res;
}

/*
 * The following functions and helper functions implement the TCL interface into
 * the entire OID module/component functionality and operations.  They handle
 * interfacing to all allowable GET, SET, TEST, LOCK, and UNLOCK features.
 *
 * =================================== WARNING =================================
 *
 * Please be VERY careful when using these functions via TCL!  You can leave the
 * system in an upredictable state, or even totally locked up, i.e. if you issue
 * a TCL lockoid command without a paired unlockoid command, the entire system
 * can become unresponsive as these command pairs control a global mutex on all
 * OIDs operations.
 *
 */

#define DBG_PRFX            "OID ERR: "
#define LOGGER_SUBMODULE    0x00080000
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE

#define OID_PARSE_ERROR     1

static void noRegisteredOid( oid_t oid)
{
    DBG_PRINTF(LOG_ERR, DBG_PRFX "No currently registered oid: %u %u %u %u\n",
        oid_namespace_get( oid),
        oid_area_id_get( oid),
        oid_value_id_get( oid),
        oid_type_id_get( oid));
}

static error_type_t parseOidParms( int argc, char **argv, uint32_t *parms)
{
    if( argc < 6) {
        DBG_PRINTF(LOG_ERR, DBG_PRFX "Invalid number of parameters: %u  Must be 'namespace', 'area', 'value', 'type', and 'index'\n", argc - 1);
        return( OID_PARSE_ERROR);
    } else
    if( str_mkint( argv[1], &parms[0])) {
        DBG_PRINTF(LOG_ERR, DBG_PRFX "Invalid 'namespace' parameter: %s\n", argv[1]);
        return( OID_PARSE_ERROR);
    } else
    if( str_mkint( argv[2], &parms[1])) {
        DBG_PRINTF(LOG_ERR, DBG_PRFX "Invalid 'area' parameter: %s\n", argv[2]);
        return( OID_PARSE_ERROR);
    } else
    if( str_mkint( argv[3], &parms[2])) {
        DBG_PRINTF(LOG_ERR, DBG_PRFX "Invalid 'value' parameter: %s\n", argv[3]);
        return( OID_PARSE_ERROR);
    } else
    if( str_mkint( argv[4], &parms[3])) {
        DBG_PRINTF(LOG_ERR, DBG_PRFX "Invalid 'type' parameter: %s\n", argv[4]);
        return( OID_PARSE_ERROR);
    } else
    if( str_mkint( argv[5], &parms[4])) {
        DBG_PRINTF(LOG_ERR, DBG_PRFX "Invalid 'index' parameter: %s\n", argv[5]);
        return( OID_PARSE_ERROR);
    } else
    if( ((uint8_t)parms[3] != OID_TYPE_STRING) &&
        ((uint8_t)parms[3] != OID_TYPE_UINT32) &&
        ((uint8_t)parms[3] != OID_TYPE_BYTE_ARRAY)) {
        DBG_PRINTF(LOG_ERR, DBG_PRFX "Invalid type: %u  Must be 'string = %u', 'uint32 = %u', or 'byte array = %u'\n", parms[3], OID_TYPE_STRING, OID_TYPE_UINT32, OID_TYPE_BYTE_ARRAY);
        return( OID_PARSE_ERROR);
    } else {
        return( OK);
    }
}

char *oid_tcl_get( int argc, char **argv)
{
    oid_t oid = 0;
    char *retString;
    uint32_t parms[5];
    error_type_t status;

    retString = MEM_MALLOC( OID_MAX_STRING_SIZE_IN_BYTES);
    ASSERT( retString != NULL);

    status = parseOidParms( argc, argv, &parms[0]);
    if( status == OK) {
        oid = oid_make( (uint8_t)parms[0], (uint8_t)parms[1], (uint8_t)parms[2], (uint8_t)parms[3]);
        
        switch( parms[3]) {
            case OID_TYPE_STRING:
                status = oid_get_string( oid, parms[4], retString, OID_MAX_STRING_SIZE_IN_BYTES);
            break;

            case OID_TYPE_UINT32:
                status = FAIL;
            break;

            case OID_TYPE_BYTE_ARRAY:
                status = FAIL;
            break;

            default:
                status = FAIL;
            break;
        }
    }

    if( status == OK) {
        return( retString);
    } else {
        MEM_FREE_AND_NULL( retString);
        if( status != OID_PARSE_ERROR) noRegisteredOid( oid);
        return(NULL);
    }
}

static char *oidTclSetTest( int argc, char **argv, char *type)
{
    oid_t oid;
    char *retString;
    uint32_t parms[5];
    error_type_t status;

    retString = MEM_MALLOC(10);
    ASSERT( retString != NULL);

    status = parseOidParms( argc, argv, &parms[0]);
    if( status == OK) {
        oid = oid_make( (uint8_t)parms[0], (uint8_t)parms[1], (uint8_t)parms[2], (uint8_t)parms[3]);
        
        switch( parms[3]) {
            case OID_TYPE_STRING:
                if( strstr(type,"SET"))
                    status = oid_set_string( oid, parms[4], argv[6], strlen(argv[6]));
                else
                    status = oid_test_string( oid, parms[4], argv[6], strlen(argv[6]));
            break;

            case OID_TYPE_UINT32:
                status = FAIL;
            break;

            case OID_TYPE_BYTE_ARRAY:
                status = FAIL;
            break;

            default:
                status = FAIL;
            break;
        }
    }

    if( status == OK) {
        retString[0] = 0;
        return( retString);
    } else {
        MEM_FREE_AND_NULL( retString);
        return(NULL);
    }
}

char *oid_tcl_set( int argc, char **argv)
{
    return( oidTclSetTest( argc, argv, "SET"));
}

char *oid_tcl_test( int argc, char **argv)
{
    return( oidTclSetTest( argc, argv, "TEST"));
}

static char *oidTclLockUnlock( int argc, char **argv, char *type)
{
    char *retString;

    retString = MEM_MALLOC(10);
    ASSERT( retString != NULL);

    if( strstr(type,"LOCK"))
        oid_interface_lock();
    else
        oid_interface_unlock();

    retString[0] = 0;
    return( retString);
}

char *oid_tcl_lock( int argc, char **argv)
{
    return( oidTclLockUnlock( argc, argv, "LOCK"));
}

char *oid_tcl_unlock( int argc, char **argv)
{
    return( oidTclLockUnlock( argc, argv, "UNLOCK"));
}

