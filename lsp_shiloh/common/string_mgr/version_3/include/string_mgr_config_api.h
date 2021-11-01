/*
 * ============================================================================
 * (C) Copyright 2011  Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *
 * \file string_mgr_config_api.h
 *
 * \brief This file contains the types (and directions) that are needed for
 *        the OEM specific configuration of the strings_mgr.
 *
 * To configure the string manager:
 *
 * 1. Each language should be symbolically defined by the enumeration,
 *    string_mgr_language_t, in string_mgr_config.h.
 *
 * 2. Each language should have an entry in the table in
 *    string_mgr_config_languages.c. This table maps the enumeration value to
 *    a string identifier and includes other information about each language.
 *
 * 3. Each language should have a translation file named strings.<lang id>
 *    added to the ROMFILES list and stored at strings/strings.<lang id>.
 *    The <lang id> for each language is the same as the string identifier
 *    assigned in the language table (see step 2 above). The format of these
 *    files is one translation per line of the format <string id>,<translation>.
 *    The <string id> is defined below (see step 5). <Translation> should be a
 *    UTF-8 encoded string. Note that not every string must be translated.
 *    Missing translations will be served by the OSFA translations provided
 *    in the string table (see step 5 below).
 *
 * 4. Strings should be symbolically defined by the enumeration,
 *    string_mgr_code_t, in string_mgr_config.h. Each string used by the
 *    firmware should have an entry in this enumeration. There is no longer
 *    any need to place strings into sub-tables or in any defined order.
 *
 * 5. Each string should have an entry in the table in
 *    string_mgr_config_strings.c. This table maps the enumeration value to
 *    a string identifier and includes other information about each string.
 *    These string identifiers, <string id>, are used in the tranlation files
 *    as well as in the embedded web server html. In the latter case the prefix
 *    STRING_ is prepended to each ID (for legacy reasons).
 *
 */


#ifndef STRING_MGR_CONFIG_API_H 
#define STRING_MGR_CONFIG_API_H

#include "string_mgr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* A string_mgr_code_entry_t holds a mapping from a single string code or
   language code to a string identifier. The string identifiers are used
   for various purposes including:

      1. Language identifiers are used in naming translation files
         (i.e. strings.<identifier>)
      2. String identifiers are used in the strings.<language> files
         as the 1st column (i.e. the key column).
      3. String identifiers are embedded as ssi directives in the EWS html
         files. In this case they are prefixed as STRING_<identifier> for
         legacy reasons.
*/
typedef struct
{
    union
    {
        string_mgr_code_t     str_code;
        string_mgr_language_t lang_code;
    };
    const char *identifier;
    union
    {
        const char*           osfa_str;        /* One size fits all string */
        unsigned int          right_to_left:1; /* Right to left lang */
    };
} string_mgr_code_entry_t;

/* A string_mgr_code_tbl_t contains a pointer to an array of
   string_mgr_code_entry_t's and an indication of the number of elements */
typedef struct
{
    const string_mgr_code_entry_t *tbl; /* Pointer to array of code_entry's */
    unsigned int                  size; /* How many array elements are there */
} string_mgr_code_tbl_t;

/* These two pointers must be provided by the OEM config layer */
extern const string_mgr_code_tbl_t* string_mgr_config_string_codes;
extern const string_mgr_code_tbl_t* string_mgr_config_language_codes;

#ifdef __cplusplus
}
#endif

#endif /* STRING_MGR_CONFIG_API_H */
