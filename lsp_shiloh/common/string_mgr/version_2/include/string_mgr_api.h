/*
 * ============================================================================
 * (C) Copyright 2008-2010   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file string_mgr_api.h
 *
 * \brief This file is the public API for the string manager.  The string
 * manager allows a caller to access a localized string within the system.
 *
 * STRING MANAGER THEORY OF OPERATION:
 *
 * Every localizable string has to have an enum defined in the string_mgr_code_t
 * enum within the string manager configuration layer (string_mgr_config.h).
 * This enum is used by the string manager and it's users to get a string. It is
 * constant for all languages. The string value associated with the enum will
 * change based on the current language.
 *
 * A user of the string manager does the following to get a string:
 * -#   \#include "string_mgr_api.h"
 * -#   Calls string_mgr_get_string() with the string code and localization
 *      type
 * 
 * To change the language a user of the string manager does the following:
 * -#   \#include "string_mgr_api.h"
 * -#   Calls string_mgr_set_language() with the language id corresponding to
 *      the desired language
 * 
 * The language id enum list is also stored in the configuration layer.  Both
 * the language id and the string_mgr_code are generated based on the input
 * languages and strings required for the specific product.
 * 
 */


#ifndef STRING_MGR_API_H 
#define STRING_MGR_API_H

#include <stdint.h>
#include <stdbool.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief A list of string manager localization types used to specify how
 * the string manager should translate the requested string in a call to
 * string_mgr_get_string()
 *
 * NOTE: that a limited translation used to be available.  This feature
 * was removed because it wasn't used and porting it would have been
 * useless at the time. KAP 6/11/2009
 */
typedef enum string_mgr_localization_type_e
{
    string_mgr_loc_english, /*!< Always return the English only translation */

    string_mgr_loc_full     /*!< Always return the localized string. */


} string_mgr_localization_type_t;

#include "string_mgr_config.h" // needs string_mgr_localization_type_t

/**
 *
 * \brief Initialize the string manager with the default settings.  This
 * function must be called only once.
 *
 */
void string_mgr_init( void );


/**
 * \brief Get a string (corresponding to the string code) from the string
 * manager with the specified localization type.
 *
 * \param code A valid string_mgr_code_t value from the enum list in
 * string_mgr_config.h.  This value is product specific.
 *
 * \param type A valid localization type based on what type of translation is
 * required for this string.  See the enumeration list for more information
 * on how the strings are translated.
 *
 * \return char * A pointer to the string requested.  The user does not
 * need to worry about freeing the string pointer when he is done using the 
 * string.
 *
 * \warning If the localization type is set so that a translation is done then
 * the language that the string is returned in is based on the current set
 * language.  The language can be set using the string_mgr_set_language()
 * function
 *
 * \b Example:
 *\code
 * const char * my_str;
 * my_str = string_mgr_get_string( STRING_STAT_READY, string_mgr_loc_full );
 * DPRINTF( (DBG_LOUD|DBG_OUTPUT), ("Look at my string %s\n", my_str) );
 * // can be done with my_str now, no need to worry about memory
 * \endcode
 *
 */
const char * string_mgr_get_string( string_mgr_code_t code, string_mgr_localization_type_t type );


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
error_type_t string_mgr_set_language( string_mgr_language_t language );


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
error_type_t string_mgr_get_language( string_mgr_language_t * language );


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
error_type_t string_mgr_set_default_language( string_mgr_language_t language );


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
error_type_t string_mgr_get_default_language( string_mgr_language_t * language );


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
bool is_right_to_left_lang( string_mgr_language_t language );

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
char *convert_bidi_string_to_display_order(char *input_str);

#ifdef __cplusplus
}
#endif
#include "string_mgr_config.h" 

#endif

