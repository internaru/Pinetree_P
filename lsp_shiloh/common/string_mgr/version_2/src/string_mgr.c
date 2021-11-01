/*
 * ============================================================================
 * (C) Copyright 2008-2009   Marvell International Ltd.
 *                          All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *
 * \file string_mgr.c
 *
 * \brief This file implements the string_mgr_api.h header file and provides
 * access to the strings within the system.
 *
 */


#include "string_mgr_api.h"
#include "lassert.h"
#include "error_types.h"
#include <stdint.h>
#include <stdbool.h>
#include "ATypes.h" /* For "NULL" definition */


/** \brief A flag indicating (true) if the string manager has been
 * initialized */
static bool string_mgr_init_done = false;


void string_mgr_init( void )
{
    /* init the config layer where all the data is */
    string_mgr_config_init();
    string_mgr_init_done = true;
}

static char str_error[]="NO STRING!";
const char * string_mgr_get_string( string_mgr_code_t code, string_mgr_localization_type_t type )
{
    char * string_ptr;
	error_type_t e_res;
    
	
	string_ptr = NULL;

    if( true != string_mgr_init_done )
        return str_error;
    
	e_res = string_mgr_config_get_string_from_code( code, 
		type,
		&string_ptr );

	if ( OK != e_res ||
         (NULL == string_ptr))
	{
		string_ptr = str_error;
	}

	ASSERT( NULL != string_ptr );

    return (const char *)string_ptr;
}


/**
 * \brief Change the language that the translations are done in.  This
 * value stays resident in NVRAM, but can be reset to the factory default
 * upon a user request.
 *
 * \param language One of the string language ids found in string_config.h
 *
 * \return error_type_t 
 * \retval OK
 * \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if there is a problem with the
 * permanent storage device
 *
 */
error_type_t string_mgr_set_language( string_mgr_language_t language )
{
    error_type_t e_res;

    ASSERT( true == string_mgr_init_done );

    /* send the request down to the config layer */
    e_res = string_mgr_config_set_language( language );
    

    return e_res;
}


/**
 * \brief Get the current language that the translations are done in.
 *
 * \param language One of the string language ids found in string_config.h
 *
 * \return error_type_t 
 * \retval OK
 * \retval DATA_ACC_ERR_NULL_PTR if incoming pointer is NULL
 *
 */
error_type_t string_mgr_get_language( string_mgr_language_t * language )
{
    error_type_t e_res;

    ASSERT( true == string_mgr_init_done );

    /* send the request down to the config layer */
    e_res = string_mgr_config_get_language( language );
    
    return e_res;
}


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
error_type_t string_mgr_set_default_language( string_mgr_language_t language )
{
    error_type_t e_res;

    ASSERT( true == string_mgr_init_done );

    /* send the request down to the config layer */
    e_res = string_mgr_config_set_default_language( language );
    

    return e_res;
}


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
error_type_t string_mgr_get_default_language( string_mgr_language_t * language )
{
    error_type_t e_res;

    ASSERT( true == string_mgr_init_done );

    /* send the request down to the config layer */
    e_res = string_mgr_config_get_default_language( language );
    
    return e_res;
}


/**
 * \brief Utility function to indicate whether passed in language is intended to
 *        be read from right to left.
 *
 * \param language One of the string language ids found in 
 *        string_mgr_config.h
 *
 * \return bool
 * \retval true  Passed in language is a right to left language.
 * \retval false Passed in language is a left to right language.
 *
 */
bool is_right_to_left_lang( string_mgr_language_t language )
{
    if ( ( LANG_HEBREW == language ) || ( LANG_ARABIC == language ) )
    {
        return true;
    }

    return false;
}

/**
 * \brief Perform the Unicode Bidirectional Algorithm (Unicode Standard Annex 
 *        #9) on the passed in "logical order" UTF8 string and return the
 *        resulting "display order" UTF8 string. 
 *
 * \param input_str The logical order UTF8 to be reordered to be converted to 
 *                  display order.
 *  
 * \return char *   The display order UTF8 string after applying bidirectional 
 *                  reordering on the input string. 
 *
 */
char *convert_bidi_string_to_display_order(char *input_str)
{
    // for now just a stub, so return the original string
    return input_str;
}

