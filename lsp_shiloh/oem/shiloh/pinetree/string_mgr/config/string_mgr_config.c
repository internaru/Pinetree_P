/*
 * ============================================================================
 * (C) Copyright 2008-2009   Marvell International Ltd.
 *                          All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h> 
#include <stdbool.h>
#include "memAPI.h"
#include "string_mgr_api.h"
#include "dprintf.h"
#include "logger.h"
#include "lassert.h"
#ifdef HAVE_NVRAM
#include "nvram_api.h"
#endif
#include "data_access_api.h"
#include "error_types.h"
#include "platform_api.h"
#include "ioutils.h"
#include "string.h"

/**
 * \file string_mgr_config.c
 *
 * \brief This file contains the main configuration parameters and related
 * functions for the string manager.
 *
 */
  
/* This define makes the blobs run compressed out of ROM. If you remove this
 * define, you will need to hit the projenv.mk to include the blobXXX.c files
 * for all languages you want to compile in. (Run the compiled version when
 * strings are changing a lot. Once frozen, switch to run out of ROM to save
 * space.) */
/* #define USE_BLOB_STRINGS */


/** \brief The string manager nvram manager block version number.
 * This number should be incremented every time the structure
 * "string_mgr_vars_t" changes */
#define STRING_MGR_VARS_VERSION 3


/** \brief the factory default language */
#define FACTORY_DEFAULT_LANGUAGE LANG_ENGLISH


typedef struct string_mgr_vars_s
{
    string_mgr_language_t current_language; /*!< The current system language */
    string_mgr_language_t default_language; /*!< The default system language */

} string_mgr_vars_t;


/** \brief A pointer used to store the pointers to the current language's
 * string tables
 */
static char ** current_language_string_tables[NUM_STRING_TABLES];


/** \brief A pointer used to store the pointers to the default language's
 * string tables
 */
static char ** default_language_string_tables[NUM_STRING_TABLES];

/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for default language */
static char * default_stat_table[STRING_MAX_STATUS]; 
/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for default language */
static char * default_report_table[STRING_MAX_REPORT_PAGE];
/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for default language */
static char * default_cp_table[STRING_MAX_CTRL_PANEL]; 
/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for default language */
static char * default_generic_table[STRING_MAX_GENERIC];
/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for default language */
static char * default_ews_table[STRING_MAX_EWS];


/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for current language */
static char *stat_table[STRING_MAX_STATUS]; 
/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for current language */
static char *report_table[STRING_MAX_REPORT_PAGE];
/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for current language */
static char *cp_table[STRING_MAX_CTRL_PANEL]; 
/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for current language */
static char *generic_table[STRING_MAX_GENERIC];
/** \brief A table for each of the NUM_STRING_TABLES, corresponding to each
 * type supported, for current language */
static char *ews_table[STRING_MAX_EWS];


/** \brief A flag to let us know if the config initialization is complete
 * or not */
static bool string_mgr_config_init_done = false;

/** \brief The NUM_BLOBS_FROM_ROM define is based on the two tables we use:
 * current_language_string_tables and default_language_string_tables.  There
 * is one blob from ROM possible for each table */
#define NUM_BLOBS_FROM_ROM 2


/** \brief We keep track of the raw blobs from ROM based on the two tables we
 * use:
 * current_language_string_tables and default_language_string_tables.  There
 * is one blob from ROM possible for each table */
static unsigned char *blob_from_rom_table[NUM_BLOBS_FROM_ROM]; 

#ifdef HAVE_NVRAM
/** \brief The NVM manager handle, used to store language variables in NVram */
static nvram_handle_t * string_mgr_nvm_handle = NULL;
#endif


static string_mgr_vars_t string_mgr_vars;


/**
 *
 * \brief Load the strings from a specified language from the raw blob to 
 * the specified table
 *
 * \param language One of the valid string_mgr_language_t enums
 *
 * \param table One of the two arrays:
 * current_language_string_tables or default_language_string_tables
 *
 *
 * \return error_type_t OK upon sucess, FAIL otherwise
 *
 */
static error_type_t string_mgr_config_load_blob_to_table(
        string_mgr_language_t language,
        char *** table);


#ifdef STRING_MGR_TEST
/**
 * 
 * \brief A test function for the string manager config layer
 *
 * \return int 0 upon success, negative value if there was an error
 *
 */
static int string_mgr_config_test( void );
#endif




// blob files
#ifndef USE_BLOB_STRINGS
extern char BlobEnglishStrings[];
//extern char BlobFrenchStrings[];
//extern char BlobItalianStrings[];
//extern char BlobSpanishStrings[];
//extern char BlobDutchStrings[];
//extern char BlobSwedishStrings[];
//extern char BlobDanishStrings[];
//extern char BlobNorwegianStrings[];
//extern char BlobFinnishStrings[];
//extern char BlobHungarianStrings[];
//extern char BlobPolishStrings[];
//extern char BlobGermanStrings[];
//extern char BlobPortugueseStrings[];
//extern char BlobCzechStrings[];
//extern char BlobTurkishStrings[];
//extern char BlobRussianStrings[];
//extern char BlobSimpchineseStrings[];
//extern char BlobTradchineseStrings[];
//extern char BlobThaiStrings[];
#endif

 /************************************
  *  Max Values 
  ***********************************/

uint32_t string_mgr_max_strings_table[NUM_STRING_TABLES] = 
{
     STRING_MAX_STATUS,     
     STRING_MAX_GENERIC,    
     STRING_MAX_CTRL_PANEL, 
     STRING_MAX_REPORT_PAGE,
     STRING_MAX_EWS_STRINGS,
};
 
/*********************************
 * Blob format
 * 
 * U32 NumTables
 * U32 TableOffset1
 * U32 TableOffset2
 * ...
 * ---------------------
 * U32 NumStringsTable1
 * U32 StrOffset1
 * U32 StrOffset2
 * --------------------
 * U32 NumStringsTable2
 * U32 StrOffset3
 * U32 StrOffset4
 * --------------------
 * char str1, str2, str3...
 ********************************/


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
error_type_t string_mgr_config_set_language( string_mgr_language_t language )
{
    error_type_t e_res;

    /* when the language is set, we need to load a new table of strings for
     * that language */
    e_res = string_mgr_config_load_blob_to_table(
            language,
            current_language_string_tables );

    if ( OK == e_res )
    {
        string_mgr_vars.current_language = language;
#ifdef HAVE_NVRAM
        e_res = nvram_set_var( string_mgr_nvm_handle, &string_mgr_vars );
#endif
    }

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
error_type_t string_mgr_config_get_language( string_mgr_language_t * language )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == language )
    {
        e_res = DATA_ACC_ERR_NULL_PTR;
    }

    if ( OK == e_res )
    {
        *language = string_mgr_vars.current_language;
    }

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
error_type_t string_mgr_config_set_default_language( string_mgr_language_t language )
{
    error_type_t e_res;

    // set the default language
    string_mgr_vars.default_language = language;
#ifdef HAVE_NVRAM
    e_res = nvram_set_var( string_mgr_nvm_handle, &string_mgr_vars );
#endif

    // set the current language to match & load the string table(s)
    if ( OK == e_res )
    {
        e_res = string_mgr_config_set_language(language);
    }

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
error_type_t string_mgr_config_get_default_language( string_mgr_language_t * language )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == language )
    {
        e_res = DATA_ACC_ERR_NULL_PTR;
    }

    if ( OK == e_res )
    {
        *language = string_mgr_vars.default_language;
    }

    return e_res;
}


/**
 *
 * \brief Get the string based on the code from the config layer
 *
 * \param code A valid string_mgr_code_t enum value
 *
 * \param localization One of the string_mgr_localization_type_t types
 *
 * \param[out] string_ptr pointer to destination for the char * to be returned
 *
 * \return OK if the code is valid, otherwise FAIL
 *
 */
error_type_t string_mgr_config_get_string_from_code( string_mgr_code_t code,
        string_mgr_localization_type_t localization,
        char ** string_ptr )
{
    char *** table;
    uint32_t table_index;
    uint32_t string_index;

    if ( localization == string_mgr_loc_english )
    {
        table = default_language_string_tables;
    }
    else
    {
        ASSERT( localization == string_mgr_loc_full );
        table = current_language_string_tables;
    }

    table_index = (uint32_t)( code >> 24 );
    string_index = ( (uint32_t)( code ) ) & 0x00FFFFFF;

    /* verify parameters */
    ASSERT( table_index < NUM_STRING_TABLES );
    ASSERT( string_index < string_mgr_max_strings_table[table_index] );

    *string_ptr = table[table_index][string_index];

    return OK;
}


static error_type_t string_mgr_config_load_blob_to_table(
        string_mgr_language_t language,
        char *** table)
{
    error_type_t e_res;
    uint32_t * num_tables;
    uint32_t * table_offset;
    uint32_t * num_strings;
    uint32_t * string_offset;
    uint32_t table_index;
    uint32_t string_index;
    uint32_t blob_index;

    e_res = OK;
    
    /* check input */
    if ( ( NULL == table ) || ( language >= LANG_END_PRINTER_LANG ) )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        if ( table == current_language_string_tables )
        {
            table[0] = (char **)&stat_table;
            table[1] = (char **)&generic_table;
            table[2] = (char **)&cp_table;
            table[3] = (char **)&report_table;
            table[4] = (char **)&ews_table;
            blob_index = 0;
        }
        else
        {
            ASSERT( table == default_language_string_tables );
            table[0] = (char **)&default_stat_table;
            table[1] = (char **)&default_generic_table;
            table[2] = (char **)&default_cp_table;
            table[3] = (char **)&default_report_table;
            table[4] = (char **)&default_ews_table;
            blob_index = 1;
        }

        #ifdef USE_BLOB_STRINGS

        if ( blob_from_rom_table[blob_index] != NULL )
        {
            /* if we previously had a language set,
             * free it before getting a new one. */
            MEM_FREE_AND_NULL(blob_from_rom_table[blob_index]);
        }

        /* get the compressed language out of ROM */
        blob_from_rom_table[blob_index] = RomMgr( ( e_English + language ), 0 );
        #else
        switch ( language )
        {
            #if 0
            case LANG_FRENCH  :
                blob_from_rom_table[blob_index] = BlobFrenchStrings;
                break;
            case LANG_ITALIAN :
                blob_from_rom_table[blob_index] = BlobItalianStrings;
                break;
            case LANG_SPANISH :
                blob_from_rom_table[blob_index] = BlobSpanishStrings;
                break;
            case LANG_DUTCH   :
                blob_from_rom_table[blob_index] = BlobDutchStrings;
                break;
            case LANG_SWEDISH :
                blob_from_rom_table[blob_index] = BlobSwedishStrings;
                break;
            case LANG_DANISH  :
                blob_from_rom_table[blob_index] = BlobDanishStrings;
                break;
            case LANG_NORWEGIAN:
                blob_from_rom_table[blob_index] = BlobNorwegianStrings;
                break;
            case LANG_FINNISH :
                blob_from_rom_table[blob_index] = BlobFinnishStrings;
                break;
            case LANG_HUNGARIAN:
                blob_from_rom_table[blob_index] = BlobHungarianStrings;
                break;
            case LANG_POLISH  :
                blob_from_rom_table[blob_index] = BlobPolishStrings;
                break;
            case LANG_GERMAN  :
                blob_from_rom_table[blob_index] = BlobGermanStrings;
                break;
            case LANG_PORTUGUESE:
                blob_from_rom_table[blob_index] = BlobPortugueseStrings;
                break;
            case LANG_CZECH   :
                blob_from_rom_table[blob_index] = BlobCzechStrings;
                break;
            case LANG_TURKISH :
                blob_from_rom_table[blob_index] = BlobTurkishStrings;
                break;
            case LANG_RUSSIAN :
                blob_from_rom_table[blob_index] = BlobRussianStrings;
                break;
            case LANG_SIMPCHINESE:    
                blob_from_rom_table[blob_index] = BlobSimpchineseStrings;
                break;
            case LANG_TRADCHINESE:    
                blob_from_rom_table[blob_index] = BlobTradchineseStrings;
                break;            
            case LANG_THAI:
                blob_from_rom_table[blob_index] = BlobThaiStrings;
                break;            
            #endif
            case LANG_ENGLISH:
                /* fall through */
            default:
                blob_from_rom_table[blob_index] = (unsigned char *)BlobEnglishStrings;
                break;
        }
        #endif

        if ( NULL == blob_from_rom_table[blob_index] )
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        num_tables = (uint32_t *) blob_from_rom_table[blob_index];
        table_offset = (uint32_t *) (blob_from_rom_table[blob_index] + 4);
        table_index = 0;

        while ( ( table_index < be32_to_cpu( *num_tables ) ) && ( table_index < NUM_STRING_TABLES ) )
        {
            num_strings = (uint32_t *)( ( (uintptr_t)blob_from_rom_table[blob_index] ) +
                    ( be32_to_cpu( *table_offset ) ) );

            string_offset = (uint32_t *) (num_strings + 1);

            string_index = 0;

            while ( ( string_index < be32_to_cpu(*num_strings) ) && ( string_index < string_mgr_max_strings_table[table_index] ) )
            {
                table[table_index][string_index] =
                    (char *) ( blob_from_rom_table[blob_index] + be32_to_cpu( *string_offset ) );

                string_index++;
                string_offset++;
            }

            table_offset++;
            table_index++;
        }
    }

    return e_res;
}


#ifdef HAVE_NVRAM
error_type_t string_mgr_vars_init( nvram_init_type_t init_type,
        uint16_t version,
        void * init_location,
        uint32_t init_location_size,
        void * unused )
{
    switch( init_type )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */

            // set default and current to factory default language
            string_mgr_vars.default_language = FACTORY_DEFAULT_LANGUAGE;
            string_mgr_vars.current_language = FACTORY_DEFAULT_LANGUAGE;

            ASSERT(sizeof(string_mgr_vars) <= init_location_size);
            memcpy( init_location, &string_mgr_vars, sizeof(string_mgr_vars));
            break;

        case NVRAM_LANG_DEFAULTS:
            // revert current language to default language
            string_mgr_vars.current_language = string_mgr_vars.default_language;
            break;

        case NVRAM_USER_DEFAULTS:
            // nothing to do since not changing any language settintgs
            break;

        default:
            ASSERT( NVRAM_OK == init_type );
            // read out current values
            memcpy(&string_mgr_vars, init_location, sizeof(string_mgr_vars));
            break;
    }
    return ( OK );
}
#endif


void string_mgr_config_init(void )
{
    error_type_t e_res;
    uint32_t index;
    string_mgr_language_t default_language;


    DBG_ASSERT( false == string_mgr_config_init_done );

    #ifdef DEBUG
    string_mgr_config_init_done = true;
    #endif

    DPRINTF( (DBG_SOFT | DBG_OUTPUT), ("String Manager: Start Default Strings\n"));

#ifdef HAVE_NVRAM
    e_res = nvram_variable_register( &string_mgr_nvm_handle,
            STRING_MGR_VAR_ID,
            STRING_MGR_VARS_VERSION,
            sizeof(string_mgr_vars),
            string_mgr_vars_init,
            NULL );
    ASSERT( OK == e_res );
#else
	e_res = FAIL;
#endif

    for ( index = 0; index < NUM_BLOBS_FROM_ROM; index++ )
    {
        blob_from_rom_table[index] = NULL;
    }

    /* the default language is stored in the platform API,
     * so get it from there */
    e_res = platvars_get_default_lang( &default_language );
    ASSERT( OK == e_res );

    /* load the default language strings into the default language table */
    e_res = string_mgr_config_load_blob_to_table(
            default_language,
            default_language_string_tables );
    ASSERT( OK == e_res );

    /* load the current language strings into the current language table */
    e_res = string_mgr_config_load_blob_to_table(
            string_mgr_vars.current_language,
            current_language_string_tables );
    ASSERT( OK == e_res );

    #ifdef STRING_MGR_TEST
    string_mgr_config_test();
    #endif

    DPRINTF( (DBG_SOFT | DBG_OUTPUT), ("StringManager: Default Done\n"));
}


#ifdef STRING_MGR_TEST
static int string_mgr_config_test( void )
{
    uint32_t index;
    uint32_t index2;

    /* print each string in the default string table */
    for (index = 0; index < NUM_STRING_TABLES; index++)
    {
        for (index2 = 0; index2 < string_mgr_max_strings_table[index]; index2++)
        {
            DPRINTF( (DBG_LOUD|DBG_OUTPUT), ("StrMgr Test Dump: Table %d: Str %d: %s\n",
                        index,
                        index2,
                        default_language_string_tables[index][index2] ) );
        }
    }

    return 0;
}
#endif


