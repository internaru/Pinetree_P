/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                      Marvell Confidential
 * ============================================================================
 */

/** 
 * \file http_vars.h
 * 
 * \brief Public interface for the HTTP component specific variables.
 * 
 * This file contains the public interface for the HTTP component specific variables.
 * 
 **/

#ifndef INC_HTTP_VARS_H
#define INC_HTTP_VARS_H

//--------------------------------------
// Included Files
//--------------------------------------

#include "ATypes.h"
#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief The maximum length of the password string variable */
#define HTTP_VAR_PASSWORD_STRING_MAX_LEN 33 // 32 plus null

//--------------------------------------
// API Function Declarations
//--------------------------------------

// FUNCTION NAME: http_vars_init 
/** 
 * \brief Boot time initialization function for the HTTP variables.
 * 
 * This function is called at system startup after the NVRAM driver and partition manager have been
 * initialized.  It sets all its variables to either default values or previously stored values from
 * NVRAM.
 * 
 * \param[in] initType type of initialization being done:  dataAccessInit_t enum value from dataAccAPI.h 
 * 
 **/ 
 
void http_vars_init( void );

//--------------------------------------
// API Declarations and Type Definitions
// API Function Declarations
//--------------------------------------


// FUNCTION NAME: http_get_xml_version_string 
/** 
 * \brief Get function for a data variable.
 * 
 * This function returns the current value of the variable by means of the incoming
 * pointer.  It returns FAIL if the incoming pointer is NULL.
 * 
 * \param[out] data pointer to destination for data to be returned
 *
 * \param[in] string_len The data buffer size in bytes
 * 
 * \return error_type_t
 * \retval OK
 * \retval FAIL if incoming pointer is NULL
 * 
 **/ 
error_type_t http_get_xml_version_string( char * data, uint32_t string_len );


// FUNCTION NAME: http_get_order_supplies_link 
/** 
 * \brief Get function for the order supplies link string.
 * 
 * This function returns the current value of the variable by means of the incoming
 * pointer.  It returns FAIL if the incoming pointer is NULL.
 * 
 * \param[out] data pointer to destination for data to be returned
 *
 * \param[in] string_len The data buffer size in bytes
 * 
 * \return error_type_t
 * \retval OK
 * \retval FAIL if incoming pointer is NULL
 * 
 **/ 
error_type_t http_get_order_supplies_link( char * data, uint32_t string_len );



/**
 *
 * \brief Get the current HTTP password used to protect the EWS pages
 *
 * \param[out] data pointer to destination for data to be returned
 *
 * \param[in] string_len the data buffer size in bytes
 *
 * \return error_type_t
 * \retval OK upon success
 * \retval FAIL if incoming data pointer is NULL
 *
 */
error_type_t http_get_var_password( char * data, uint32_t string_len );


/**
 *
 * \brief Set the current HTTP password used to protect the EWS pages
 *
 * \param[out] data pointer to source for data to be saved 
 *
 * \param[in] string_len the data buffer size in bytes
 *
 * \return error_type_t
 * \retval OK upon success
 * \retval FAIL if incoming data pointer is NULL
 *
 */
error_type_t http_set_var_password( char * data, uint32_t string_len );


#ifdef __cplusplus
}
#endif

#endif // INC_HTTP_VARS_H

