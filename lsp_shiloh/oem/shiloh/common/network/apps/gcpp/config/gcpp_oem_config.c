
/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file gcpp_oem_config.c
 *
 * @brief OEM-specific GCPP print configuration options
 *  
 * The purpose of the OEM configuration files for this module is to provide
 * a mechanism for the user to modify configurable options without having 
 * to recompile the core module that depend on them. The goal is to isolate
 * OEM and core specific configuration values, and provide an abstraction layer
 * to simplify the porting
 **/

//--------------------------------------
// Included files
//--------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <error_types.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include "dprintf.h"
#include "lassert.h"
#include "memAPI.h"
#include "SysInit.h"
#include "base64_api.h"
#include "expat.h"
#include "platform.h"
#include "platform_api.h"
#include "dbg_api.h"
#include "json_api.h"

#include "cmd_proc_api.h"
#include "cmd_wrapper_api.h"

#include "net_gcpp_vars.h"
#include "gcpp_oem_config.h"

extern gcpp_error_t gcpp_client_notification_func (gcpp_client_event_t event, void *event_context);

static pthread_t        gcpp_oem_print_job_request_thread;
static ALIGN8 uint8_t   gcpp_oem_print_job_request_stack[ OEM_THREAD_STACK_SIZE ];
static FIFOLIST         gcpp_oem_print_job_list;

//=========================================================================================================================
// OEM-specific configuration values
//=========================================================================================================================

static gcpp_oem_print_settings_t g_gcpp_oem_print_settings;

GCP_REG_STATUS gcpp_oem_reg_status = GCP_NOT_REGISTERED;
//=========================================================================================================================
// OEM-specific types and definitions
//=========================================================================================================================

#define GCPP_OEM_GLOBAL_LOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&g_gcpp_oem_print_settings.oem_lock_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define GCPP_OEM_GLOBAL_UNLOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&g_gcpp_oem_print_settings.oem_lock_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

// Supported MIME Types
#define GCPP_OEM_MIMETYPE_IMAGE         GCPP_MIMETYPE_IMAGE
#define GCPP_OEM_MIMETYPE_JPEG          GCPP_MIMETYPE_JPEG
#define GCPP_OEM_MIMETYPE_XPS           "application/xml"

// Uncomment to enable JPEG jobs
//#define GCPP_OEM_SUPPORT_JPEG_JOBS      1

// HTTP Header Names
#define GCPP_OEM_HEADER_CONTENT_TYPE    "Content-Type"

#define GCPP_OEM_PRINT_HEADER           GCPP_ACCEPT_HEADER "\r\n" GCPP_PROXY_HEADER

// Default GCPP Delay Timeout Values
#define GCPP_OEM_DEFAULT_XMPP_CONNECT_TIMEOUT_MS    5000
#define GCPP_OEM_DEFAULT_XMPP_EVENT_TIMEOUT_MS      500
#define GCPP_OEM_DEFAULT_KEEP_ALIVE_DELAY_MS        (300*1000)
#define GCPP_OEM_DEFAULT_TOKEN_TIMEOUT_SECS         1800
#define GCPP_OEM_DEFAULT_POLLING_TIMEOUT_SECS       30
#define GCPP_OEM_DEFAULT_START_TIMEOUT_MS           POSIX_WAIT_FOREVER

// Default maximum retry value
#define GCPP_OEM_DEFAULT_MAX_RETRIES                3

// In order to obtain a OAuth2 authorization code (access token), the GCPP application must provide a 
// Client ID.  The Client ID is how Google identifies your application/printer.  Google will give you a 
// Client ID when you register your app/printer with Google.  Client IDs don't need to be unique per 
// printer and it is expected there would be one Client ID per printer manufacturer.  
//
// Client credentials can be obtained as explained here: 
//     http://code.google.com/apis/accounts/docs/OAuth2.html#Registering
//
// The Client Secret is obtained along with Client ID as part of the app/printer client credentials
//
// An example of a Client ID and Secret would be:
//     Client ID:       66762873415.apps.googleusercontent.com
//     Client Secret:   U7qWvdsWFs8steLvye8PeYhu
#define GCPP_OEM_OAUTH_CLIENT_ID            "884957551131.apps.googleusercontent.com"
#define GCPP_OEM_OAUTH_CLIENT_SECRET        "-Doi_HNKHVicPw7AR_Fp358y"

#define GCPP_PROXY_NAME_LEN     128+1
#define GCPP_MODEL_NAME_LEN     64+1
#define GCPP_KSN_LEN            16+1

#define TRAY_CAPACITY           250
#define TRAY_LEVEL              100

#define COLLATION               false
#define JOG_OFFSET              false
#define DUPLEXER                false
#define STAPLER                 false
#define HOLE_PUNCH              false

#define MAX_COPIES              999
#define MIN_COPIES              1
#define MAX_PRIORITY            100
#define MIN_PRIORITY            1

#define PPM_MONO                15
#define PPM_COLOR               0

#define DEFAULT_COLOR_SPACE             COLOR_SPACE_GRAY
#define DEFAULT_LANDSCAPE_ORIENTATION   LANDSCAPE_PLUS90
#define DEFAULT_RASTERIZER              RASTERIZER_NONE
#define DEFAULT_RESOLUTION              RESOLUTION_300
#define DEFAULT_MEDIATYPE               MEDIATYPE_ANY
#define DEFAULT_MEDIASIZE               MEDIASIZE_ANY

#define DEFAULT_EMPTY                   ""

#define PPD_ADOBE_VERSION           "4.3"
#define PPD_FORMAT_VERSION          "4.3"
#define PPD_FILE_VERSION            "1.0"
#define PPD_LANGUAGE_ENCODING       "ISOLatin1"
#define PPD_LANGUAGE_VERSION        "English"
#define PPD_PS_VERSON               "(2016.0) 0"
//#define PPD_LANGUAGE_LEVEL          "2"
#define PPD_LANGUAGE_LEVEL          "3"

static gcpp_oem_mediatype_mapping_t gcpp_oem_mediatypes [] =
{
    //type                          name                description         manualFeed
    { MEDIATYPE_ANY,                "ANY",              "Any",              FALSE}, 
    { MEDIATYPE_PLAIN_PAPER,        "PLAIN",            "Plain",            FALSE},
    { MEDIATYPE_TRANSPARENCY,       "TRANSPARENCY",     "Transparency",     FALSE},
    { MEDIATYPE_LIGHTPAPER,         "LIGHTPAPER",       "Light",            FALSE},
    { MEDIATYPE_CARDSTOCK,          "CARDSTOCK",        "Cardstock",        FALSE},
    { MEDIATYPE_HEAVY1,             "HEAVY",            "Heavy",            FALSE},
    { MEDIATYPE_LABEL,              "LABELS",           "Labels",           FALSE},
    { MEDIATYPE_ENVELOPE,           "ENVELOPE",         "Envelope",         FALSE},
    { MEDIATYPE_PREMIUMPHOTO,       "PREMIUMPHOTO",     "Premium Photo",    FALSE},
    { MEDIATYPE_LETTERHEAD,         "LETTERHEAD",       "Letterhead",       FALSE},
    { MEDIATYPE_PREPRINTED,         "PREPRINTED",       "Preprinted",       FALSE},
    { MEDIATYPE_PREPUNCHED,         "PREPUNCHED",       "Prepunched",       FALSE},
    { MEDIATYPE_UNKNOWN,            "UNKNOWN",          "Unknown",          FALSE},
};
#define GCPP_OEM_NUM_MEDIATYPES  ( sizeof( gcpp_oem_mediatypes ) / sizeof( gcpp_oem_mediatype_mapping_t ) )

static gcpp_oem_mediatype_invocation_t gcpp_oem_mediatype_invocations [] =
{
    //type                              invocation
    { MEDIATYPE_ANY,                    ""},
    { MEDIATYPE_PLAIN_PAPER,            ""}, 
    { MEDIATYPE_TRANSPARENCY,           ""},
    { MEDIATYPE_LIGHTPAPER,             ""},
    { MEDIATYPE_CARDSTOCK,              ""},
    { MEDIATYPE_HEAVY1,                 ""},
    { MEDIATYPE_LABEL,                  ""},
    { MEDIATYPE_ENVELOPE,               ""},
    { MEDIATYPE_PREMIUMPHOTO,           ""},
    { MEDIATYPE_LETTERHEAD,             ""},
    { MEDIATYPE_PREPRINTED,             ""},
    { MEDIATYPE_PREPUNCHED,             ""},
    { MEDIATYPE_UNKNOWN,                ""},
};
#define GCPP_OEM_NUM_MEDIATYPE_INVOCATIONS  ( sizeof( gcpp_oem_mediatype_invocations ) / sizeof( gcpp_oem_mediatype_invocation_t ) )

static gcpp_oem_mediasize_mapping_t gcpp_oem_mediasizes [] =
{
    //type                              name            description                 width   height  marginX marginY
    { MEDIASIZE_ANY,                    "ANY",          "Any",                      612,    792,    0,      0},
    { MEDIASIZE_LETTER,                 "LETTER",       "Letter",                   612,    792,    0,      0}, 
    { MEDIASIZE_LEGAL,                  "LEGAL",        "Legal",                    612,    1008,   0,      0},
    { MEDIASIZE_EXECUTIVE,              "EXECUTIVE",    "Executive",                522,    756,    0,      0},
    { MEDIASIZE_CUSTOM_85x13,           "8_5X13",       "8.5x13in",                 612,    864,    0,      0},
    { MEDIASIZE_A4,                     "A4",           "A4",                       595,    842,    0,      0},
    { MEDIASIZE_A5,                     "A5",           "A5",                       420,    595,    0,      0},
    { MEDIASIZE_B5,                     "B5",           "B5 (JIS)",                 516,    729,    0,      0},
    { MEDIASIZE_CUSTOM_16K,             "16K",          "16K 197x273mm",            558,    774,    0,      0},
    { MEDIASIZE_ENV_10,                 "ENV_10",       "Envelope #10",             297,    684,    0,      0},
    { MEDIASIZE_ENV_MONARCH,            "ENV_MONARCH",  "Envelope Monarch",         279,    540,    0,      0},
    { MEDIASIZE_ENV_C5,                 "ENV_C5",       "Envelope C5",              459,    649,    0,      0},
    { MEDIASIZE_ENV_DL,                 "ENV_DL",       "Envelope DL",              312,    624,    0,      0},
    { MEDIASIZE_ENV_B5,                 "ENV_B5",       "Envelope B5",              499,    709,    0,      0},
    { MEDIASIZE_A6,                     "A6",           "A6",                       298,    420,    0,      0},
    { MEDIASIZE_JAPANESE_POSTCARD,      "POSTCARD",     "Postcard (JIS)",           283,    420,    0,      0},
    { MEDIASIZE_DBL_JAPANESE_POSTCARD,  "DBL_POSTCARD", "Double Postcard (JIS)",    420,    567,    0,      0},
    { MEDIASIZE_CUSTOM_16K_184_260,     "16K_184_260",  "16K 184x260mm",            522,    737,    0,      0},
    { MEDIASIZE_CUSTOM_16K_195_270,     "16K_195_270",  "16K 195x270mm",            553,    765,    0,      0},
    { MEDIASIZE_CUSTOM_4x6,             "4x6",          "4x6",                      288,    432,    0,      0},
    { MEDIASIZE_CUSTOM_5x8,             "5X8",          "5X8",                      360,    576,    0,      0},
    { MEDIASIZE_CUSTOM_10x15cm,         "10x15cm",      "10x15cm",                  288,    432,    0,      0},
};
#define GCPP_OEM_NUM_MEDIASIZES  ( sizeof( gcpp_oem_mediasizes ) / sizeof( gcpp_oem_mediasize_mapping_t ) )

static gcpp_oem_mediasize_invocation_t gcpp_oem_pagesize_invocations [] =
{
    //type                              invocation
    { MEDIASIZE_ANY,                    ""},
    { MEDIASIZE_LETTER,                 ""}, 
    { MEDIASIZE_LEGAL,                  ""},
    { MEDIASIZE_EXECUTIVE,              ""},
    { MEDIASIZE_CUSTOM_85x13,           ""},
    { MEDIASIZE_A4,                     ""},
    { MEDIASIZE_A5,                     ""},
    { MEDIASIZE_B5,                     ""},
    { MEDIASIZE_CUSTOM_16K,             ""},
    { MEDIASIZE_ENV_10,                 ""},
    { MEDIASIZE_ENV_MONARCH,            ""},
    { MEDIASIZE_ENV_C5,                 ""},
    { MEDIASIZE_ENV_DL,                 ""},
    { MEDIASIZE_ENV_B5,                 ""},
    { MEDIASIZE_A6,                     ""},
    { MEDIASIZE_JAPANESE_POSTCARD,      ""},
    { MEDIASIZE_DBL_JAPANESE_POSTCARD,  ""},
    { MEDIASIZE_CUSTOM_16K_184_260,     ""},
    { MEDIASIZE_CUSTOM_16K_195_270,     ""},
    { MEDIASIZE_CUSTOM_4x6,             ""},
    { MEDIASIZE_CUSTOM_5x8,             ""},
    { MEDIASIZE_CUSTOM_10x15cm,         ""},
};
#define GCPP_OEM_NUM_PAGESIZE_INVOCATIONS  ( sizeof( gcpp_oem_pagesize_invocations ) / sizeof( gcpp_oem_mediasize_invocation_t ) )

static gcpp_oem_mediasize_invocation_t gcpp_oem_pageregion_invocations [] =
{
    //type                              invocation
    { MEDIASIZE_ANY,                    ""},
    { MEDIASIZE_LETTER,                 ""}, 
    { MEDIASIZE_LEGAL,                  ""},
    { MEDIASIZE_EXECUTIVE,              ""},
    { MEDIASIZE_CUSTOM_85x13,           ""},
    { MEDIASIZE_A4,                     ""},
    { MEDIASIZE_A5,                     ""},
    { MEDIASIZE_B5,                     ""},
    { MEDIASIZE_CUSTOM_16K,             ""},
    { MEDIASIZE_ENV_10,                 ""},
    { MEDIASIZE_ENV_MONARCH,            ""},
    { MEDIASIZE_ENV_C5,                 ""},
    { MEDIASIZE_ENV_DL,                 ""},
    { MEDIASIZE_ENV_B5,                 ""},
    { MEDIASIZE_A6,                     ""},
    { MEDIASIZE_JAPANESE_POSTCARD,      ""},
    { MEDIASIZE_DBL_JAPANESE_POSTCARD,  ""},
    { MEDIASIZE_CUSTOM_16K_184_260,     ""},
    { MEDIASIZE_CUSTOM_16K_195_270,     ""},
    { MEDIASIZE_CUSTOM_4x6,             ""},
    { MEDIASIZE_CUSTOM_5x8,             ""},
    { MEDIASIZE_CUSTOM_10x15cm,         ""},
};
#define GCPP_OEM_NUM_PAGEREGION_INVOCATIONS  ( sizeof( gcpp_oem_pageregion_invocations ) / sizeof( gcpp_oem_mediasize_invocation_t ) )

static gcpp_oem_resolution_mapping_t gcpp_oem_resolutions [] = 
{
        //type              name        description     width   height  
    {   RESOLUTION_300,    "300dpi",    "300 dpi",      300,    300},
    {   RESOLUTION_600,    "600dpi",    "300 dpi",      600,    600},
};
#define GCPP_OEM_NUM_RESOLUTIONS  ( sizeof( gcpp_oem_resolutions ) / sizeof( gcpp_oem_resolution_mapping_t ) ) 

static gcpp_oem_resolution_invocation_t gcpp_oem_resolution_invocations [] = 
{
        //type              invocation
    {   RESOLUTION_300,    ""},
    {   RESOLUTION_600,    ""},
};
#define GCPP_OEM_NUM_RESOLUTION_INVOCATIONS  ( sizeof( gcpp_oem_resolution_invocations ) / sizeof( gcpp_oem_resolution_invocation_t ) ) 

static gcpp_oem_color_space_mapping_t gcpp_oem_color_spaces [] = 
{
    {   COLOR_SPACE_GRAY,   "Gray"  , "GrayScale"},
    {   COLOR_SPACE_RGB,    "RGB"   , "RGB Color"},
    {   COLOR_SPACE_CMY,    "CMY"   , "CMY Color"},
    {   COLOR_SPACE_CMYK,   "CMYK"  , "CMYK Color"},
};
//#define GCPP_OEM_NUM_COLOR_SPACES  ( sizeof( gcpp_oem_color_spaces ) / sizeof( gcpp_oem_color_space_mapping_t ) ) 
#define GCPP_OEM_NUM_COLOR_SPACES  1  // GrayScale only

static gcpp_oem_color_space_invocation_t gcpp_oem_color_space_invocations [] = 
{
//    {   COLOR_SPACE_GRAY,   "<</ColorSpace 18>>setpagedevice"  },
    {   COLOR_SPACE_GRAY,   "<</cupsColorOrder 0/cupsColorSpace 3/cupsCompression 1>>setpagedevice"  },
    {   COLOR_SPACE_RGB,    "<</ColorSpace 19>>setpagedevice"   },
    {   COLOR_SPACE_CMY,    ""  },
    {   COLOR_SPACE_CMYK,   ""  },
};
#define GCPP_OEM_NUM_COLOR_SPACES_INVOCATIONS  ( sizeof( gcpp_oem_color_space_invocations ) / sizeof( gcpp_oem_color_space_mapping_t ) ) 

static gcpp_oem_landscape_mapping_t gcpp_oem_landscapes [] = 
{
    {   LANDSCAPE_ANY,      "Any"       },
    {   LANDSCAPE_PLUS90,   "Plus90"    },
    {   LANDSCAPE_MINUS90,  "Minus90"   },
};
#define GCPP_OEM_NUM_LANDSCAPES  ( sizeof( gcpp_oem_landscapes ) / sizeof( gcpp_oem_landscape_mapping_t ) ) 

static gcpp_oem_rasterizer_mapping_t gcpp_oem_rasterizers [] = 
{
    {   RASTERIZER_NONE,        "None"      },
    {   RASTERIZER_ACCEPT64K,   "Accept64K" },
    {   RASTERIZER_TYPE42,      "Type42"    },
    {   RASTERIZER_TRUEIMAGE,   "TrueImage" },
};
#define GCPP_OEM_NUM_RASTERIZERS  ( sizeof( gcpp_oem_rasterizers ) / sizeof( gcpp_oem_rasterizer_mapping_t ) ) 

#define GCP_CLIENT_INTERFACE_TESTING


//=========================================================================================================================
// OEM-specific local function definitions
//=========================================================================================================================
#ifdef DEBUG
#define GCPP_HEX_DUMP_CHAR_WIDTH 32
void gcpp_oem_hex_dump( char * buffer, uint32_t length )
{
    //error_type_t e_res;
    ASSERT( NULL != buffer );
        
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    //e_res = dbg_memory_dump( buffer, length, DBG_DUMP_WORDSIZE_1BYTE, DBG_DUMP_FORMAT_ALL );
    // don't care about the return value.
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}
#endif

#define GCPP_REG_COMPLETED_EVENT_SET  0x01
pthread_mutex_t gcpp_reg_mutex = POSIX_MUTEX_INITIALIZER;
pthread_cond_t  gcpp_reg_cond  = PTHREAD_COND_INITIALIZER;
uint32_t gcpp_reg_flags = 0;

bool bOperationSuspended = false;

void gcpp_event_set(uint32_t flag_to_set)
{
    pthread_mutex_lock(&gcpp_reg_mutex);

    gcpp_reg_flags |= flag_to_set;
    pthread_cond_broadcast(&gcpp_reg_cond);

    pthread_mutex_unlock(&gcpp_reg_mutex);
}

void gcpp_event_clear(uint32_t flag_to_clear)
{
    pthread_mutex_lock(&gcpp_reg_mutex);

    gcpp_reg_flags &= ~flag_to_clear;
    pthread_cond_broadcast(&gcpp_reg_cond);

    pthread_mutex_unlock(&gcpp_reg_mutex);
}

void gcpp_event_wait(uint32_t flag_to_wait)
{
    uint32_t status;
    status = 0;

    pthread_mutex_lock(&gcpp_reg_mutex);
    if (!(gcpp_reg_flags & flag_to_wait))
    {
       status = pthread_cond_wait(&gcpp_reg_cond, &gcpp_reg_mutex);
    }

    if( status != 0)
    {
        dbg_printf("gcpp_reg_complete_event Get failed 0x%x", status);
    }
}
//Set the event when gcp reg is done
void gcpp_reg_complete_event_set(void)
{
    gcpp_event_set((uint32_t)GCPP_REG_COMPLETED_EVENT_SET);
}
//Clear the event when you start a gcp
void gcpp_reg_complete_event_clear(void)
{
    gcpp_event_clear((uint32_t)GCPP_REG_COMPLETED_EVENT_SET);
}
//Wait for event for event 
void gcpp_reg_complete_event_wait(void)
{
    gcpp_event_wait((uint32_t)GCPP_REG_COMPLETED_EVENT_SET);
}


char* gcpp_oem_strndup(char* string, int length)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (string == NULL)
        return NULL;

    if (length == 0)
        return NULL;

    char *newStr = (char*)MEM_MALLOC(length+1);
    if (newStr)
    {
        memset(newStr, 0, length+1);
        strncpy(newStr, string, length);
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return newStr;
}

char* gcpp_oem_strdup(char* string)
{
    return gcpp_oem_strndup(string, strlen(string));
}

int gcpp_oem_num_mediatypes( void ) 
{
    return GCPP_OEM_NUM_MEDIATYPES;
}

gcpp_error_t gcpp_oem_get_mediatype( int index, gcpp_oem_mediatype_mapping_t **output)
{
    *output = NULL;
    
    if (index < GCPP_OEM_NUM_MEDIATYPES)
    {
        *output = &gcpp_oem_mediatypes[ index ];
        return NET_GCPP_SUCCESS;
    }

    return NET_GCPP_FAIL;
}

gcpp_error_t gcpp_oem_map_mediatype( mediatype_t input, gcpp_oem_mediatype_mapping_t **output)
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < GCPP_OEM_NUM_MEDIATYPES; i++ )
    {
        if ( gcpp_oem_mediatypes[ i ].type == input )
        {
            *output = &gcpp_oem_mediatypes[ i ];
            return NET_GCPP_SUCCESS;
        }
    }

    return NET_GCPP_FAIL;
}

char *gcpp_oem_mediatype_invocation( mediatype_t input)
{
    uint32_t i;

    for ( i = 0; i < GCPP_OEM_NUM_MEDIATYPE_INVOCATIONS; i++ )
    {
        if ( gcpp_oem_mediatype_invocations[ i ].type == input )
        {
            return gcpp_oem_mediatype_invocations[ i ].invocation;
        }
    }

    return DEFAULT_EMPTY;
}


int gcpp_oem_num_mediasizes( void )
{
    return GCPP_OEM_NUM_MEDIASIZES;
}

gcpp_error_t gcpp_oem_get_mediasize( int index, gcpp_oem_mediasize_mapping_t **output)
{
    *output = NULL;
    
    if (index < GCPP_OEM_NUM_MEDIASIZES)
    {
        *output = &gcpp_oem_mediasizes[ index ];
        return NET_GCPP_SUCCESS;
    }

    return NET_GCPP_FAIL;
}

gcpp_error_t gcpp_oem_map_mediasize( mediasize_t input, gcpp_oem_mediasize_mapping_t **output)
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < GCPP_OEM_NUM_MEDIASIZES; i++ )
    {
        if ( gcpp_oem_mediasizes[ i ].type == input )
        {
            *output = &gcpp_oem_mediasizes[ i ];
            return NET_GCPP_SUCCESS;
        }
    }

    return NET_GCPP_FAIL;
}

char *gcpp_oem_pagesize_invocation( mediasize_t input)
{
    uint32_t i;

    for ( i = 0; i < GCPP_OEM_NUM_PAGESIZE_INVOCATIONS; i++ )
    {
        if ( gcpp_oem_pagesize_invocations[ i ].type == input )
        {
            return gcpp_oem_pagesize_invocations[ i ].invocation;
        }
    }

    return DEFAULT_EMPTY;
}

char *gcpp_oem_pageregion_invocation( mediasize_t input)
{
    uint32_t i;

    for ( i = 0; i < GCPP_OEM_NUM_PAGESIZE_INVOCATIONS; i++ )
    {
        if ( gcpp_oem_pageregion_invocations[ i ].type == input )
        {
            return gcpp_oem_pageregion_invocations[ i ].invocation;
        }
    }

    return DEFAULT_EMPTY;
}


int gcpp_oem_num_resolutions( void )
{
    return GCPP_OEM_NUM_RESOLUTIONS;
}

gcpp_error_t gcpp_oem_get_resolution( int index, gcpp_oem_resolution_mapping_t **output)
{
    *output = NULL;
    
    if (index < GCPP_OEM_NUM_RESOLUTIONS)
    {
        *output = &gcpp_oem_resolutions[ index ];
        return NET_GCPP_SUCCESS;
    }

    return NET_GCPP_FAIL;
}

gcpp_error_t gcpp_oem_map_resolution( gcpp_oem_resolution_t input, gcpp_oem_resolution_mapping_t **output )
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < GCPP_OEM_NUM_RESOLUTIONS; i++ )
    {
        if ( gcpp_oem_resolutions[ i ].resolution == input )
        {
            *output = &gcpp_oem_resolutions[ i ];
            return NET_GCPP_SUCCESS;
        }
    }

    return NET_GCPP_FAIL;
} 

char *gcpp_oem_resolution_invocation( gcpp_oem_resolution_t input)
{
    uint32_t i;

    for ( i = 0; i < GCPP_OEM_NUM_RESOLUTION_INVOCATIONS; i++ )
    {
        if ( gcpp_oem_resolution_invocations[ i ].resolution == input )
        {
            return gcpp_oem_resolution_invocations[ i ].invocation;
        }
    }

    return DEFAULT_EMPTY;
}


int gcpp_oem_num_color_spaces( void )
{
    return GCPP_OEM_NUM_COLOR_SPACES;
}

gcpp_error_t gcpp_oem_get_color_space( int index, gcpp_oem_color_space_mapping_t **output)
{
    *output = NULL;
    
    if (index < GCPP_OEM_NUM_COLOR_SPACES)
    {
        *output = &gcpp_oem_color_spaces[ index ];
        return NET_GCPP_SUCCESS;
    }

    return NET_GCPP_FAIL;
}

gcpp_error_t gcpp_oem_map_color_space( gcpp_oem_color_space_t input, gcpp_oem_color_space_mapping_t **output )
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < GCPP_OEM_NUM_COLOR_SPACES; i++ )
    {
        if ( gcpp_oem_color_spaces[ i ].color_space == input )
        {
            *output = &gcpp_oem_color_spaces[ i ];
            return NET_GCPP_SUCCESS;
        }
    }

    return NET_GCPP_FAIL;
} 

char *gcpp_oem_color_space_invocation( gcpp_oem_color_space_t input)
{
    uint32_t i;

    for ( i = 0; i < GCPP_OEM_NUM_COLOR_SPACES_INVOCATIONS; i++ )
    {
        if ( gcpp_oem_color_space_invocations[ i ].color_space == input )
        {
            return gcpp_oem_color_space_invocations[ i ].invocation;
        }
    }

    return DEFAULT_EMPTY;
}

int gcpp_oem_num_landscapes( void )
{
    return GCPP_OEM_NUM_LANDSCAPES;
}

gcpp_error_t gcpp_oem_get_landscape( int index, gcpp_oem_landscape_mapping_t **output)
{
    *output = NULL;
    
    if (index < GCPP_OEM_NUM_LANDSCAPES)
    {
        *output = &gcpp_oem_landscapes[ index ];
        return NET_GCPP_SUCCESS;
    }

    return NET_GCPP_FAIL;
}

gcpp_error_t gcpp_oem_map_landscape( gcpp_oem_landscape_t input, gcpp_oem_landscape_mapping_t **output )
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < GCPP_OEM_NUM_LANDSCAPES; i++ )
    {
        if ( gcpp_oem_landscapes[ i ].landscape == input )
        {
            *output = &gcpp_oem_landscapes[ i ];
            return NET_GCPP_SUCCESS;
        }
    }

    return NET_GCPP_FAIL;
} 


int gcpp_oem_num_rasterizers( void )
{
    return GCPP_OEM_NUM_RASTERIZERS;
}

gcpp_error_t gcpp_oem_get_rasterizer( int index, gcpp_oem_rasterizer_mapping_t **output)
{
    *output = NULL;
    
    if (index < GCPP_OEM_NUM_RASTERIZERS)
    {
        *output = &gcpp_oem_rasterizers[ index ];
        return NET_GCPP_SUCCESS;
    }

    return NET_GCPP_FAIL;
}

gcpp_error_t gcpp_oem_map_rasterizer( gcpp_oem_rasterizer_t input, gcpp_oem_rasterizer_mapping_t **output )
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < GCPP_OEM_NUM_RASTERIZERS; i++ )
    {
        if ( gcpp_oem_rasterizers[ i ].rasterizer == input )
        {
            *output = &gcpp_oem_rasterizers[ i ];
            return NET_GCPP_SUCCESS;
        }
    }

    return NET_GCPP_FAIL;
} 


void gcpp_oem_default_print_settings( gcpp_oem_print_settings_t *settings )
{
    if (settings == NULL)
        return;
        
    memset(settings, 0, sizeof(gcpp_oem_print_settings_t));
    
    /* get required platform strings */
    platvars_get_usb_mfgstr(&settings->manufacturer);
    platvars_get_prod_serialnum(&settings->serial_number);
    
    settings->auto_print_registration_page = FALSE;
    settings->auto_claim_printer_for_registration_complete = TRUE;
    
    settings->max_retries = GCPP_OEM_DEFAULT_MAX_RETRIES;
    
    /* printer supported mime types */
#ifdef GCPP_OEM_SUPPORT_JPEG_JOBS
    snprintf(settings->supported_mime_types, 128, "%s, %s", GCPP_OEM_MIMETYPE_IMAGE, GCPP_OEM_MIMETYPE_JPEG);
#else
    snprintf(settings->supported_mime_types, 128, "%s", GCPP_OEM_MIMETYPE_IMAGE);
#endif    
    
    settings->colors_supported = FALSE;
    
    settings->ppm_mono = PPM_MONO;
    settings->ppm_color = PPM_COLOR;
    
    /* The first index in any of the arrays below is the default value for that field */
    settings->collation_supported     = COLLATION;
    settings->jog_offset_supported    = JOG_OFFSET;
    settings->duplexer_supported      = DUPLEXER;
    settings->stapler_supported       = STAPLER;
    settings->hole_punch_supported    = HOLE_PUNCH;
    
    settings->min_copies              = MIN_COPIES;
    settings->max_copies              = MAX_COPIES;
    settings->min_priority            = MIN_PRIORITY;
    settings->max_priority            = MAX_PRIORITY;

    settings->default_colorspace      = DEFAULT_COLOR_SPACE;
    settings->default_landscape_orientation = DEFAULT_LANDSCAPE_ORIENTATION;
    settings->default_rasterizer      = DEFAULT_RASTERIZER;
    settings->default_resolution      = DEFAULT_RESOLUTION;
    settings->default_mediatype       = DEFAULT_MEDIATYPE;
    settings->default_mediasize       = DEFAULT_MEDIASIZE;
    settings->cancel_req              = FALSE;
    settings->client_notify_func      = NULL;
}

char* gcpp_oem_base64(char *src, int len)
{
    size_t length = (len*4+2)/3+2;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (length == 0)
        return NULL;

    char *dst = (char*)MEM_MALLOC(length+1);
    if (dst)
    {
        memset(dst, 0, length+1);
        base64_enc((void*)dst, (void*)src, len);    //make base 64 encoding
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return dst;
}


//=========================================================================================================================
// PPD parsing functions
//=========================================================================================================================
char *gcpp_oem_ppd_appendString(char *buf, char *str)
{
//    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (str != NULL)
    {
        size_t size = (buf == NULL) ? 0 : strlen(buf);
        size_t count = strlen(str);
        
        if (buf == NULL)
        {
            buf = MEM_MALLOC(count+1);
        }
        else
        {
            buf = MEM_REALLOC(buf, size+count+1);
        }
        
        memcpy(&buf[size], str, count);
        size += count;
        buf[size] = 0;
    }
    
//    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return buf;
}

char *gcpp_oem_ppd_appendKeyValue(char *buf, char *mainKey, char *optionKey, bool quotes)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (mainKey != NULL)
    {
        buf = gcpp_oem_ppd_appendString(buf, mainKey);
        buf = gcpp_oem_ppd_appendString(buf, ": ");
        
        if (quotes) buf = gcpp_oem_ppd_appendString(buf, "\"");
        buf = gcpp_oem_ppd_appendString(buf, optionKey);
        if (quotes) buf = gcpp_oem_ppd_appendString(buf, "\"");
        buf = gcpp_oem_ppd_appendString(buf, "\r\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return buf;
}

char *gcpp_oem_ppd_appendKeyValueBool(char *buf, char *mainKey, bool value)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (mainKey != NULL)
    {
        buf = gcpp_oem_ppd_appendKeyValue(buf, mainKey, (value ? "True" : "False"), FALSE);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return buf;
}

char *gcpp_oem_ppd_get_printer_capabilities(gcpp_oem_print_config_t *config, gcpp_oem_print_settings_t *settings)
{
    char strKey[256];
    char strValue[256];
    int i;
    char *ppd = NULL;
    
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    /* The capabilities and defaults parameters can be in XPS (XML Paper Specification)     */
    /* or PPD (Postscript Printer Description) formats.                                     */
    /*                                                                                      */
    /* For the purpose of this exercise, we generate the printer capabilities in PPD format */
    
    if (config == NULL || settings == NULL)
        return NULL;

    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*PPD-Adobe", PPD_ADOBE_VERSION, TRUE);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*FormatVersion", PPD_FORMAT_VERSION, TRUE);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*FileVersion", PPD_FILE_VERSION, TRUE);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*LanguageEncoding", PPD_LANGUAGE_ENCODING, FALSE);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*LanguageVersion", PPD_LANGUAGE_VERSION, FALSE);
    
    sprintf(strKey, "(%s)", net_gcpp_get_printer_name(false));
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*Product", strKey, TRUE);
    
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*PSVersion", PPD_PS_VERSON, TRUE);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*Manufacturer", g_gcpp_oem_print_settings.manufacturer, TRUE);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*ModelName", net_gcpp_get_printer_name(false), TRUE);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*ShortNickName", net_gcpp_get_printer_name(false), TRUE);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*NickName", net_gcpp_get_printer_name(false), TRUE);
    
    sprintf(strKey, "%.6s01.ppd", net_gcpp_get_printer_name(false));
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*PCFileName", strKey, TRUE);

    ppd = gcpp_oem_ppd_appendString(ppd, "*%=== Basic Capabilities ==========================\r\n");
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*LanguageLevel", PPD_LANGUAGE_LEVEL, TRUE);
    
    ppd = gcpp_oem_ppd_appendString(ppd, "\r\n");

    ppd = gcpp_oem_ppd_appendString(ppd, "*%=== Color Information ===========================\r\n");
    ppd = gcpp_oem_ppd_appendKeyValueBool(ppd, "*ColorDevice", settings->colors_supported);
    gcpp_oem_color_space_mapping_t *colorspace = NULL;
    if (gcpp_oem_map_color_space(settings->default_colorspace, &colorspace) == NET_GCPP_SUCCESS && colorspace != NULL)
    { 
      ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*DefaultColorSpace", colorspace->name, FALSE);
    }

    ppd = gcpp_oem_ppd_appendString(ppd, "*OpenUI *ColorModel/Color Mode: PickOne\r\n");
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*OrderDependency", "10 AnySetup *Color", FALSE);
    for (i=0; i<gcpp_oem_num_color_spaces(); i++)
    { 
      if (gcpp_oem_get_color_space(i, &colorspace) == NET_GCPP_SUCCESS && colorspace != NULL)
      { 
        sprintf(strKey, "*ColorModel %s/%s", colorspace->name, colorspace->description);
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, strKey, 
                                          gcpp_oem_color_space_invocation(colorspace->color_space), TRUE);
      }
    }
    if (gcpp_oem_map_color_space(settings->default_colorspace, &colorspace) == NET_GCPP_SUCCESS 
        && colorspace != NULL)
    { 
      ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*DefaultColorModel", colorspace->name, FALSE);
    }
    ppd = gcpp_oem_ppd_appendString(ppd, "*CloseUI: *ColorModel\r\n");
    ppd = gcpp_oem_ppd_appendString(ppd, "\r\n");
    
    gcpp_oem_landscape_mapping_t *landscape = NULL;
    if (gcpp_oem_map_landscape(settings->default_landscape_orientation, &landscape) == NET_GCPP_SUCCESS && landscape != NULL)
    {
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*LandscapeOrientation", landscape->name, FALSE);
    }
    
    gcpp_oem_rasterizer_mapping_t *rasterizer = NULL;
    if (gcpp_oem_map_rasterizer(settings->default_rasterizer, &rasterizer) == NET_GCPP_SUCCESS && rasterizer != NULL)
    {
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*TTRasterizer", rasterizer->name, FALSE);
    }
    
    sprintf(strKey, "%d", settings->ppm_mono);
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*Throughput", strKey, TRUE);

    
    ppd = gcpp_oem_ppd_appendString(ppd, "*%=== Media Type ==================================\r\n");
    ppd = gcpp_oem_ppd_appendString(ppd, "*OpenUI *MediaType: PickOne\r\n");
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*OrderDependency", "20 AnySetup *MediaType", FALSE);
    
    gcpp_oem_mediatype_mapping_t *mediaType_entry = NULL;
    for (i=0; i<gcpp_oem_num_mediatypes(); i++)
    {
        if (gcpp_oem_get_mediatype(i, &mediaType_entry) == NET_GCPP_SUCCESS && mediaType_entry != NULL)
        {
            sprintf(strKey, "*MediaType %s/%s", mediaType_entry->name, mediaType_entry->description);
            ppd = gcpp_oem_ppd_appendKeyValue(ppd, strKey, gcpp_oem_mediatype_invocation(mediaType_entry->type), TRUE);
        }
    }
    
    if (gcpp_oem_map_mediatype(settings->default_mediatype, &mediaType_entry) == NET_GCPP_SUCCESS && mediaType_entry != NULL)
    {
        // set default
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*DefaultMediaType", mediaType_entry->name, FALSE);
    }

    ppd = gcpp_oem_ppd_appendString(ppd, "*CloseUI: *MediaType\r\n");
    ppd = gcpp_oem_ppd_appendString(ppd, "\r\n");
    
    
    ppd = gcpp_oem_ppd_appendString(ppd, "*%=== Resolution Information ======================\r\n");
    ppd = gcpp_oem_ppd_appendString(ppd, "*OpenUI *Resolution: PickOne\r\n");
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*OrderDependency", "10 AnySetup *Resolution", FALSE);
    
    gcpp_oem_resolution_mapping_t *resolution_entry = NULL;
    for (i=0; i<gcpp_oem_num_resolutions(); i++)
    {
        if (gcpp_oem_get_resolution(i, &resolution_entry) == NET_GCPP_SUCCESS && resolution_entry != NULL)
        {
            sprintf(strKey, "*Resolution %s/%s", resolution_entry->name, resolution_entry->description);
            ppd = gcpp_oem_ppd_appendKeyValue(ppd, strKey, gcpp_oem_resolution_invocation(resolution_entry->resolution), TRUE);
        }
    }
    
    if (gcpp_oem_map_resolution(settings->default_resolution, &resolution_entry) == NET_GCPP_SUCCESS && resolution_entry != NULL)
    {
        // set default
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*DefaultResolution", resolution_entry->name, FALSE);
    }
    
    ppd = gcpp_oem_ppd_appendString(ppd, "*CloseUI: *Resolution\r\n");

    
    ppd = gcpp_oem_ppd_appendString(ppd, "*%=== Paper Handling ==============================\r\n");
    ppd = gcpp_oem_ppd_appendString(ppd, "*OpenUI *PageSize: PickOne\r\n");
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*OrderDependency", "20 AnySetup *PageSize", FALSE);
    
    gcpp_oem_mediasize_mapping_t *mediaSize_entry = NULL;
    for (i=0; i<gcpp_oem_num_mediasizes(); i++)
    {
        if (gcpp_oem_get_mediasize(i, &mediaSize_entry) == NET_GCPP_SUCCESS && mediaSize_entry != NULL)
        {
            sprintf(strKey, "*PageSize %s/%s", mediaSize_entry->name, mediaSize_entry->description);
            ppd = gcpp_oem_ppd_appendKeyValue(ppd, strKey, gcpp_oem_pagesize_invocation(mediaSize_entry->type), TRUE);
        }
    }
    
    if (gcpp_oem_map_mediasize(settings->default_mediasize, &mediaSize_entry) == NET_GCPP_SUCCESS && mediaSize_entry != NULL)
    {
        // set default
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*DefaultPageSize", mediaSize_entry->name, FALSE);
    }

    ppd = gcpp_oem_ppd_appendString(ppd, "*CloseUI: *PageSize\r\n");
    ppd = gcpp_oem_ppd_appendString(ppd, "\r\n");
    
    ppd = gcpp_oem_ppd_appendString(ppd, "*OpenUI *PageRegion: PickOne\r\n");
    ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*OrderDependency", "30 AnySetup *PageRegion", FALSE);
    for (i=0; i<gcpp_oem_num_mediasizes(); i++)
    {
        if (gcpp_oem_get_mediasize(i, &mediaSize_entry) == NET_GCPP_SUCCESS && mediaSize_entry != NULL)
        {
            sprintf(strKey, "*PageRegion %s/%s", mediaSize_entry->name, mediaSize_entry->description);
            ppd = gcpp_oem_ppd_appendKeyValue(ppd, strKey, gcpp_oem_pageregion_invocation(mediaSize_entry->type), TRUE);
        }
    }
    
    if (gcpp_oem_map_mediasize(settings->default_mediasize, &mediaSize_entry) == NET_GCPP_SUCCESS && mediaSize_entry != NULL)
    {
        // set default
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*DefaultPageRegion", mediaSize_entry->name, FALSE);
    }

    ppd = gcpp_oem_ppd_appendString(ppd, "*CloseUI: *PageRegion\r\n");
    ppd = gcpp_oem_ppd_appendString(ppd, "\r\n");
    
    for (i=0; i<gcpp_oem_num_mediasizes(); i++)
    {
        gcpp_oem_mediasize_mapping_t *mediaSize_entry = NULL;
        if (gcpp_oem_get_mediasize(i, &mediaSize_entry) == NET_GCPP_SUCCESS && mediaSize_entry != NULL)
        {
            sprintf(strKey, "*ImageableArea %s/%s", mediaSize_entry->name, mediaSize_entry->description);
            sprintf(strValue, "%d %d %d %d", mediaSize_entry->marginX, mediaSize_entry->marginY, (mediaSize_entry->width-mediaSize_entry->marginX), (mediaSize_entry->height-mediaSize_entry->marginY));
            ppd = gcpp_oem_ppd_appendKeyValue(ppd, strKey, strValue, TRUE);
        }
    }
    
    if (gcpp_oem_map_mediasize(settings->default_mediasize, &mediaSize_entry) == NET_GCPP_SUCCESS && mediaSize_entry != NULL)
    {
        // set default
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*DefaultImageableArea", mediaSize_entry->name, FALSE);
    }
    ppd = gcpp_oem_ppd_appendString(ppd, "\r\n");
    
    for (i=0; i<gcpp_oem_num_mediasizes(); i++)
    {
        gcpp_oem_mediasize_mapping_t *mediaSize_entry = NULL;
        if (gcpp_oem_get_mediasize(i, &mediaSize_entry) == NET_GCPP_SUCCESS && mediaSize_entry != NULL)
        {
            sprintf(strKey, "*PaperDimension %s/%s", mediaSize_entry->name, mediaSize_entry->description);
            sprintf(strValue, "%d %d", mediaSize_entry->width, mediaSize_entry->height);
            ppd = gcpp_oem_ppd_appendKeyValue(ppd, strKey, strValue, TRUE);
        }
    }
    
    if (gcpp_oem_map_mediasize(settings->default_mediasize, &mediaSize_entry) == NET_GCPP_SUCCESS && mediaSize_entry != NULL)
    {
        // set default
        ppd = gcpp_oem_ppd_appendKeyValue(ppd, "*DefaultPaperDimension", mediaSize_entry->name, FALSE);
    }
    ppd = gcpp_oem_ppd_appendString(ppd, "\r\n");
    
    ppd = gcpp_oem_ppd_appendString(ppd, "*%\r\n");
    ppd = gcpp_oem_ppd_appendString(ppd, "\r\n");
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return ppd;
}


smjob_ticket_t* gcpp_oem_sm_print_get_default_ticket(void)
{
    smjob_rcode_t smjob_rcode;
    smjob_ticket_t     *job_ticket = NULL;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Get Default SM Ticket()\n" );

    
    job_ticket = (smjob_ticket_t *)MEM_MALLOC(sizeof(smjob_ticket_t));
    ASSERT(job_ticket);
    
    memset(job_ticket, 0, sizeof(smjob_ticket_t));
    
    smjob_rcode = smjob_get_default_ticket(SMJOB_TYPE_PRINT, job_ticket);
    XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return job_ticket;
}

smjob_ticket_t* gcpp_oem_sm_print_get_ticket_from_data(char *data, uint32_t size)
{
    smjob_ticket_t *job_ticket = NULL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Get SM Ticket from Job Ticket(), size = %d\n", size );
    
    /* generating default SM job ticket */
    job_ticket = gcpp_oem_sm_print_get_default_ticket();
    ASSERT(job_ticket);

    /* This is where parsing of the job ticket data would be done.    */
    /*                                                                */
    /* Job ticket can be in either XPS (XML Paper Specification)      */
    /* or PPD (Postscript Printer Description) format, determined by  */
    /* the format in which the printer originally registered its      */
    /* capabilities. In the future, other job ticket formats may be   */
    /* supported.                                                     */
    /*                                                                */
    /* For the purpose of this exercise, only using the defaulit SM   */
    /* ticket so no parsing is done.                                  */

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return job_ticket;
}

/** 
 * \brief free resources associated with the sm job ticket
 * 
 **/
void gcpp_oem_sm_delete_job_ticket(smjob_ticket_t *job_ticket)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (job_ticket)
    {
        MEM_FREE_AND_NULL(job_ticket);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief add new sm job to active job list
 * 
 **/
bool gcpp_oem_sm_add_job( gcpp_job_t *job, uint32_t sm_id )
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (job == NULL)
    {
        DBG_ERR("Invalid Job parameter!\n");
        return FALSE;
    }
    
    GCPP_OEM_GLOBAL_LOCK();
    gcpp_oem_job_entry_t *new_job = (gcpp_oem_job_entry_t *)MEM_MALLOC(sizeof(gcpp_oem_job_entry_t));
    
    if (new_job == NULL)
    {
        DBG_ERR("Unable to allocate new job entry structure!\n");
        return FALSE;
    }
    
    memset(new_job, 0, sizeof(gcpp_oem_job_entry_t));
    
    new_job->job = net_gcpp_job_copy(job);
    new_job->sm_id = sm_id;
    
    if (g_gcpp_oem_print_settings.sm_active_job_list == NULL)
    {
        g_gcpp_oem_print_settings.sm_active_job_list = new_job;
    }
    else
    {
        gcpp_oem_job_entry_t *last_job = g_gcpp_oem_print_settings.sm_active_job_list;
        
        while (last_job->next != NULL)
        {
            last_job = last_job->next;
        }
        
        last_job->next = new_job;
    }
    GCPP_OEM_GLOBAL_UNLOCK();

    DBG_MSG("Added Job %s, SM Job ID = %d\n", job->id, sm_id);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return TRUE;
}

/** 
 * \brief get job entry (pass NULL for head entry)
 * 
 **/
gcpp_oem_job_entry_t *gcpp_oem_sm_get_active_jobs( gcpp_oem_job_entry_t* prev )
{
    //DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (prev == NULL)
    {
        DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
        return g_gcpp_oem_print_settings.sm_active_job_list;
    }

    //DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return prev->next;
}

/** 
 * \brief get sm job from list matching job id
 * 
 **/
gcpp_job_t *gcpp_oem_smjob_exist( char *id )
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (id == NULL)
    {
        DBG_ERR("Invalid ID parameter!\n");
        return NULL;
    }
    
    GCPP_OEM_GLOBAL_LOCK();
    gcpp_oem_job_entry_t *job_entry = g_gcpp_oem_print_settings.sm_active_job_list;
    while (job_entry != NULL)
    {
        if ((job_entry->job != NULL) && (strcmp(job_entry->job->id, id) == 0))
        {
            DBG_MSG("Found Job %s, id = %d\n", job_entry->job->id, job_entry->sm_id);
            
            GCPP_OEM_GLOBAL_UNLOCK();
            return job_entry->job;
        }
        
        job_entry = job_entry->next;
    }
    GCPP_OEM_GLOBAL_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NULL;
}

/** 
 * \brief get sm job from list matching sm id
 * 
 **/
gcpp_job_t *gcpp_oem_smjob_exist_by_id( uint32_t id )
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (id == 0)
    {
        DBG_ERR("Invalid ID parameter!\n");
        return NULL;
    }
    
    GCPP_OEM_GLOBAL_LOCK();
    gcpp_oem_job_entry_t *job_entry = g_gcpp_oem_print_settings.sm_active_job_list;
    while (job_entry != NULL)
    {
        if (job_entry->sm_id == id)
        {
            DBG_MSG("Found Job %s, id = %d\n", job_entry->job->id, job_entry->sm_id);
            
            GCPP_OEM_GLOBAL_UNLOCK();
            return job_entry->job;
        }
        
        job_entry = job_entry->next;
    }
    GCPP_OEM_GLOBAL_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NULL;
}

/** 
 * \brief remove sm job from list matching sm id
 * 
 **/
void gcpp_oem_sm_remove_job_sm_id( uint32_t id )
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG("job %d: ==>gcpp_oem_sm_remove_job_sm_id - remove job\n", 
                id);
    
    if (id == 0)
    {
        DBG_ERR("Invalid ID parameter!\n");
        return;
    }
    
    GCPP_OEM_GLOBAL_LOCK();
    gcpp_oem_job_entry_t *job_entry = g_gcpp_oem_print_settings.sm_active_job_list;
    gcpp_oem_job_entry_t *prev_entry = NULL;
    
    while (job_entry != NULL)
    {
        if (job_entry->sm_id == id)
        {
            if (prev_entry == NULL)
            {
                g_gcpp_oem_print_settings.sm_active_job_list = job_entry->next;
            }
            else
            {
                prev_entry->next = job_entry->next;
            }
            
            DBG_MSG("Removing Job %s, id = %d\n", job_entry->job->id, job_entry->sm_id);

            if (job_entry->job != NULL)
            {
                net_gcpp_job_delete(job_entry->job);
            }
            
            MEM_FREE_AND_NULL(job_entry);

            GCPP_OEM_GLOBAL_UNLOCK();
            
            return;
        }
        
        prev_entry = job_entry;
        job_entry = job_entry->next;
    }
    GCPP_OEM_GLOBAL_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return;
}

/** 
 * \brief remove all sm jobs from list
 * 
 **/
void gcpp_oem_sm_remove_all_jobs( void )
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    GCPP_OEM_GLOBAL_LOCK();
    gcpp_oem_job_entry_t *job_entry = g_gcpp_oem_print_settings.sm_active_job_list;
    
    while (job_entry != NULL)
    {
        gcpp_oem_job_entry_t *job_next = job_entry->next;
        
        DBG_MSG("Remove Job %s, id = %d\n", job_entry->job->id, job_entry->sm_id);

        if (job_entry->job != NULL)
        {
            net_gcpp_job_delete(job_entry->job);
        }
        
        MEM_FREE_AND_NULL(job_entry);
        
        job_entry = job_next;
    }
    
    g_gcpp_oem_print_settings.sm_active_job_list = NULL;
    
    GCPP_OEM_GLOBAL_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return;
}

int32_t gcpp_oem_sm_get_job_data_recv(char *buffer, uint32_t size, void *context)
{
    uint32_t num_bytes = 0;
    
    ASSERT(context);
    gcpp_print_job_request_t *job_request = (gcpp_print_job_request_t *)context;
    
    DBG_VERBOSE("job %d: ==>gcpp_oem_sm_get_job_data_recv - %d bytes recieved\n", 
                job_request->job_id, size);

    if (job_request->doc_id == SMJOB_INVALID_DOC_ID)
    {
        job_request->doc_id = smjob_add_document(job_request->job_id, &job_request->doc_ticket, true);
        if(job_request->doc_id != SMJOB_INVALID_DOC_ID)
        {
            DBG_MSG( "Doc %d added to Job %d\n", job_request->doc_id, job_request->job_id );
        }
    }
    
    if(job_request->doc_id != SMJOB_INVALID_DOC_ID)
    {
        gcpp_print_job_data_t* data = (gcpp_print_job_data_t*)MEM_MALLOC(sizeof(gcpp_print_job_data_t));
        ASSERT(data != NULL);
        
        data->buf = MEM_MALLOC(size);
        ASSERT(data->buf != NULL);
        
        memcpy(data->buf, buffer, size);
        data->len = size;
        data->read = 0;
        
        num_bytes = size;
        
        DBG_VERBOSE("job %d: ==>gcpp_oem_sm_get_job_data_recv - adding %d bytes to fifo)\n", 
                    job_request->job_id, data->len);
                    
        FListAdd(&job_request->datafifo, data, FIFO_WAIT_FOREVER);
    }
    else
    {
        DBG_ERR( "Invalid Doc for Job %d\n", job_request->job_id );
        ASSERT(0);
    }
    
    return num_bytes;
}

/* parse ticket header */ 
int32_t gcpp_oem_sm_parse_ticket_headers(char *header, uint32_t size, void *context)
{
    bool *xps_formatted = (bool *)context;
    ASSERT(xps_formatted);

    // check for valid header and size values
    if (header == NULL || size == 0)
        return size;
        
    int copylen = size;

     // remove any trailing CR/LF
    char *ptr = header + (copylen-1);
    while (copylen > 0 && (*ptr == '\r' || *ptr == '\n'))
    {
        ptr--;
        copylen--;
    }
        
    // copylen cannot be zero
    if (copylen == 0)
        return size;
    
    char *hdr = gcpp_oem_strndup(header, copylen);
    if (!hdr) 
    {
        DBG_ERR("Unable to allocation memory for Ticket header!\n");
        return 0;
    }
     
    // separate key/value
    ptr = strstr(hdr, ": ");
    if(ptr != NULL) 
    {
        *ptr = '\0';
        char *name = hdr; 
        char *value = ptr + 2;
        
        // look for Content-Type
        if (strcasecmp(GCPP_OEM_HEADER_CONTENT_TYPE, name) == 0)
        {
            // check for XPS content type
            if (strcasecmp(value, GCPP_OEM_MIMETYPE_XPS) == 0)
            {
                DBG_MSG("Located XPS content type (%s)\n", value);
                *xps_formatted = true;
            }
            else
            {
                DBG_MSG("Content type is not XPS (%s)\n", value);
                *xps_formatted = false;
            }
        }
    }
    else
    {
        // Job header is missing ':' separator
    }
    
    MEM_FREE_AND_NULL(hdr);
    return size;
}

/* parse job headers and update job ticket */ 
int32_t gcpp_oem_sm_parse_job_headers(char *header, uint32_t size, void *context)
{
    //smjob_ticket_t *job_ticket= (smjob_ticket_t *)context;
    //ASSERT(job_ticket);

    // check for valid header and size values
    if (header == NULL || size == 0)
        return size;
        
    int copylen = size;

     // remove any trailing CR/LF
    char *ptr = header + (copylen-1);
    while (copylen > 0 && (*ptr == '\r' || *ptr == '\n'))
    {
        ptr--;
        copylen--;
    }
        
    // copylen cannot be zero
    if (copylen == 0)
        return size;
    
    char *hdr = gcpp_oem_strndup(header, copylen);
    if (!hdr) 
    {
        DBG_ERR("Unable to allocation memory for Job header!\n");
        return 0;
    }
     
    // separate key/value
    ptr = strstr(hdr, ": ");
    if(ptr != NULL) 
    {
        *ptr = '\0';
        char *name = hdr; 
        char *value = ptr + 2;
        
        // look for Content-Type
        if (strcasecmp(GCPP_OEM_HEADER_CONTENT_TYPE, name) == 0)
        {
            // check for PWG Image
            if (strcasecmp(value, GCPP_OEM_MIMETYPE_IMAGE) == 0)
            {
                DBG_MSG("Located PWG RASTER IMAGE type print job\n");
            }
#ifdef GCPP_OEM_SUPPORT_JPEG_JOBS
            // else check if JPEG job
            else if (strcasecmp(value, GCPP_OEM_MIMETYPE_JPEG) == 0)
            {
                DBG_MSG("Located JPEG type print job\n");
            }
#endif
            else
            {
                DBG_ERR("Unsupported MIME Type = %s\n", value);
                
                // by setting size to 0, this will cause the request to fail
                size = 0;
            }
        }
    }
    else
    {
        // Job header is missing ':' separator
    }
    
    MEM_FREE_AND_NULL(hdr);
    return size;
}

gcpp_print_job_request_t* gcpp_oem_sm_new_job_request(gcpp_job_t *job)
{
    gcpp_print_job_request_t* job_request = (gcpp_print_job_request_t*)MEM_MALLOC(sizeof(gcpp_print_job_request_t));
    ASSERT(job_request);
    
    memset(job_request, 0, sizeof(gcpp_print_job_request_t));
    job_request->job = net_gcpp_job_copy(job);
    job_request->job_id = SMJOB_INVALID_JOB_ID;
    job_request->doc_id = SMJOB_INVALID_DOC_ID;
    
    return job_request;
}

void gcpp_oem_sm_delete_job_request(gcpp_print_job_request_t *job_request)
{
    ASSERT(job_request);
    
    // if datafifo is not empty, empty it
    while (!FListIsEmpty(&job_request->datafifo))
    {
        gcpp_print_job_data_t *data = FListRemove(&job_request->datafifo, 5000);
        if (data)
        {
            memFree(data);
        }
        else
        {
            // timed out - break out of loop
            break;
        }
    }

    FListDestroy(&job_request->datafifo);
    net_gcpp_job_delete(job_request->job);
    memFree(job_request);
}

int32_t gcpp_oem_smjob_reader(char *buf, uint32_t len, uint32_t *timeout_ms, void *user_data)
{
    int32_t num_bytes = 0;
    
    ASSERT(user_data);
    gcpp_print_job_request_t *job_request = (gcpp_print_job_request_t *)user_data;
    
    DBG_VERBOSE("job %d: ==>gcpp_oem_smjob_reader (timeout_ms=%d, buf=%08x, len=%d)\n", 
                job_request->job_id, *timeout_ms, buf, len);
                
    if (job_request->data == NULL)
    {
        job_request->data = FListRemove(&job_request->datafifo, *timeout_ms);                
    }


    if (job_request->data)
    {
        uint32_t to_copy = (job_request->data->len < len) ? job_request->data->len : len;
        void * from_buf = job_request->data->buf+job_request->data->read;
        
        DBG_VERBOSE("job %d: ==>gcpp_oem_smjob_reader passing data (buf=%08x, len=%d)\n", job_request->job_id, from_buf, to_copy);
        memcpy(buf, from_buf, to_copy);
        job_request->data->read += to_copy;
        job_request->data->len -= to_copy;
        num_bytes = to_copy;    

        if (job_request->data->len == 0)
        {
            memFree(job_request->data);
            job_request->data = NULL;
        }
    }
                
    return num_bytes;
}

void gcpp_oem_smjob_process_status_change(uint32_t job_id, void *user_data)
{
    bool jobCleanup = false;
    smjob_rcode_t  smjob_rcode;
    smjob_status_t job_status;
    
    DBG_MSG("job %d: ==>gcpp_oem_smjob_process_status_change - job state change\n", 
                job_id);
                
    ASSERT(user_data);
    gcpp_print_job_request_t *job_request = (gcpp_print_job_request_t *)user_data;
    gcpp_job_t *job = job_request->job;
                
    smjob_rcode = smjob_get_status(job_id, &job_status);
    XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);
    DBG_MSG("Status event for job %d, state = 0x%x\n", job_id, job_status.state);

    switch (job_status.state)
    {
        case SMJOB_STATE_PROCESSING:
        {
            DBG_MSG("Job state changed to processing for job %d\n", job_id);
            
            /* indicate GCPP print job is now in process */
            net_gcpp_job_status_update(job->id, GCPP_JOB_STATUS_IN_PROGRESS, NULL, NULL);
            break;
        }
        
        case SMJOB_STATE_CANCELED:
        case SMJOB_STATE_ABORTED:
        {
            /* job failed to print, job error */
            DBG_ERR("Job cancelled or aborted for job %d\n", job_id);
            
            /* indicate job completed with error */
            net_gcpp_job_status_update(job->id, GCPP_JOB_STATUS_ERROR, NULL, NULL);
            jobCleanup = true;
            break;
        }
        
        case SMJOB_STATE_COMPLETED:
        {
            /* job completed */
            DBG_MSG("Job completed for job %d\n", job_id);
            
            /* indicate job complete */
            net_gcpp_job_status_update(job->id, GCPP_JOB_STATUS_DONE, NULL, NULL);
            jobCleanup = true;
            break;
        }
        
        default:
        {
            DBG_MSG("New state for job %d, state = 0x%x\n", job_id, job_status.state);
            break;
        }
    }
    
    if (jobCleanup)
    {
        DBG_MSG("Clean up job %d\n", job_id);
        gcpp_oem_sm_remove_job_sm_id(job_id);
        gcpp_oem_sm_delete_job_request(job_request);
    }
}

void gcpp_oem_smjob_status_event(uint32_t job_id, smjob_status_events_t job_events, void *user_data)
{
    ASSERT(user_data);

    DBG_MSG("job %d: ==>gcpp_oem_smjob_status_event - job event = 0x%x\n", 
                job_id, job_events);

    if (job_events & SMJOB_STATUS_EVENT_STATE)
    {
        gcpp_oem_smjob_process_status_change(job_id, user_data);
    }
}

/** 
 * \brief pass GCPP job onto SMJOB interface
 * 
 **/
uint32_t gcpp_oem_sm_submit_job(gcpp_job_t *job, smjob_ticket_t *job_ticket)
{
    gcpp_error_t    rcode;
    smjob_rcode_t  smjob_rcode;
    uint32_t        job_id = SMJOB_INVALID_JOB_ID;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    ASSERT(job);
    ASSERT(job_ticket);
    
    /* pass job onto SMJOB interface */
    DBG_MSG("Passing job to SM JOB\n");
    
    /* parse job header for content type and update the job ticket */
    rcode = net_gcpp_printer_get_job(job->file_url, job->skip_pages, g_gcpp_oem_print_settings.supported_mime_types, (gcpp_recv_job_header_t)&gcpp_oem_sm_parse_job_headers, NULL, job_ticket);
    if (rcode != NET_GCPP_SUCCESS)
    {
        DBG_ERR("Failed to parse header for job %s\n", job->id);
    }
    else
    {
        gcpp_print_job_request_t* job_request = gcpp_oem_sm_new_job_request(job);
        ASSERT(job_request != NULL);
        
        smjob_rcode = smjob_get_default_ticket(SMJOB_TYPE_PRINT, &job_request->job_ticket);
        XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);

        //_extract_string(cntxt->reqattr,"job-name", job_ticket.job_description.job_name, SMJOB_MAX_NAME_LEN);
        //_extract_string(cntxt->reqattr,"requesting-user-name", job_ticket.job_description.originating_user_name, SMJOB_MAX_NAME_LEN);

        job_request->job_id = smjob_create(&job_request->job_ticket, gcpp_oem_smjob_status_event, SMJOB_STATUS_EVENT_STATE, (void *)job_request);
        if(job_request->job_id == SMJOB_INVALID_JOB_ID)
        {
            DBG_ERR("Failed to create job %s\n", job->id);
            ASSERT(0);
            
            net_gcpp_job_delete(job_request->job);
            MEM_FREE_AND_NULL(job_request);
            job_id = SMJOB_INVALID_JOB_ID;
        }
        else
        {
            job_id = job_request->job_id;
            
            smjob_rcode = smjob_get_default_doc_ticket(job_request->job_id, &job_request->doc_ticket);
            XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);

            job_request->doc_ticket.doc_description.type.print.recv_func = gcpp_oem_smjob_reader;
            job_request->doc_ticket.doc_description.type.print.user_data = job_request;
            
            DBG_MSG("Adding job %s to print job fifo, ID recieved = %d\n", job_request->job->id, job_request->job_id);
            FListAdd(&gcpp_oem_print_job_list, job_request, FIFO_WAIT_FOREVER);
        }
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return job_id;
}

/** 
 * \brief handles any print jobs in the specified GCPP printer job queue
 * 
 **/
bool gcpp_oem_sm_process_print_jobs( void )
{
    bool ret = FALSE;
    int failures = 0;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    while (net_gcpp_is_enabled(false))
    {
        /* get job from GCPP printer */
        gcpp_job_t **jobs = NULL;
        gcpp_error_t rcode = net_gcpp_printer_get_jobs_list(&jobs);
        
        if (rcode == NET_GCPP_SUCCESS)
        {
            /* indicate success */
            failures = 0;
            ret = TRUE;
            
            if (jobs)
            {
                gcpp_job_t *job = NULL;
                int index = 0;
                int retries = 0;
                
                while ((job = jobs[index]) != NULL)
                {
                    /* only process new (non-existing) jobs */
                        if (gcpp_oem_smjob_exist(job->id) == NULL)
                        {
                            uint32_t new_job_id = SMJOB_INVALID_JOB_ID;
                    
                            DBG_MSG("Processing new Job %s\n", job->id);

                            /* generate sm job ticket */
                            char *ticket_data = NULL;
                            uint32_t ticket_size = 0;
                            bool xmp_formatted = true;
                            if (net_gcpp_printer_get_job_ticket(job->ticket_url, (gcpp_recv_job_header_t)&gcpp_oem_sm_parse_ticket_headers, (char **)&ticket_data, &ticket_size, &xmp_formatted) == NET_GCPP_SUCCESS && ticket_data != NULL)
                            {
                                smjob_ticket_t *ticket = gcpp_oem_sm_print_get_ticket_from_data(ticket_data, ticket_size);
                                
                                if (ticket != NULL)
                                {
                                    /* send job to SMJOB interface */
                                    new_job_id = gcpp_oem_sm_submit_job(job, ticket);
                                    if (new_job_id != SMJOB_INVALID_JOB_ID)
                                    {
                                        /* add to sm job list */
                                        gcpp_oem_sm_add_job(job, new_job_id);
                                    }
                                    
                                    /* delete job ticket */
                                    gcpp_oem_sm_delete_job_ticket(ticket);
                                }
                                
                                MEM_FREE_AND_NULL(ticket_data);
                            }
                            else
                            {
                                DBG_ERR("ERROR : Failed to retrieve sm job ticket!\n");
                            }
                            
                        if (new_job_id == SMJOB_INVALID_JOB_ID)
                        {
                            // failed to submit job
                            if (++retries < 3)
                            {
                                // retry this job
                                DBG_MSG("Retrying job %d\n", job->id);
                                continue;
                        }
                        else
                        {
                                // fail job, update job status with error */
                                if (net_gcpp_job_status_update(job->id, GCPP_JOB_STATUS_ERROR, NULL, NULL) != NET_GCPP_SUCCESS)
                                {
                                    DBG_ERR("ERROR: Failed to set GCP job status to ERROR!\n");
                                }
                            }
                        }
                    }

                    retries = 0;
                    index++;
                }
                                
                /* clean up jobs */
                net_gcpp_job_delete_list(jobs);
            }
                    
            /* exit while loop */
            break;
        }
        else
        {
            failures++;
            
            if (rcode == NET_GCPP_HTTP_FORBIDDEN) // may need to update access code
            {
                DBG_ERR("ERROR : Failed to get job, updating access token!\n");
                
                if (failures > g_gcpp_oem_print_settings.max_retries)
                {
                    DBG_ERR("ERROR : Max failures reached!\n");
                    
                    /* indicate failure */
                    ret = FALSE;
                    
                    break;
                }
                
                DBG_MSG("Refresh auth tokens\n");
                if (net_gcpp_issue_refresh_authorization_tokens_request(NULL, false) != NET_GCPP_SUCCESS)
                {
                    DBG_ERR("ERROR : Unable to issue reques to update access token!\n");
                    
                    /* indicate failure */
                    ret = FALSE;
                
                    /* exit while loop */
                    break;
                }
            }
            else
            {
                DBG_ERR("ERROR : Failed request to get job!\n");
                
                /* indicate failure */
                ret = FALSE;
            
                /* exit while loop */
                break;
            }
        }
    }
        
    if (!ret)
    {
        DBG_ERR("ERROR : returning failure!\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

void gcpp_oem_sm_check_active_jobs(bool waitForComplete)
{
    //DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    /* checking for jobs to complete */
    if (waitForComplete)
    {
        DBG_MSG("Waiting for all jobs to complete...\n");
    }
    
    do
    {
        gcpp_oem_job_entry_t *job_entry = gcpp_oem_sm_get_active_jobs(NULL);

        if (job_entry != NULL)
        {
            while (job_entry)
            {
                /* checking for any currently processing jobs to complete*/
                if (waitForComplete && job_entry != NULL && job_entry->state != SMJOB_STATE_PROCESSING)
                {
                    /* if job is not pending, then set GCP status to ERROR */
                    if (job_entry->state != SMJOB_STATE_PENDING && job_entry->state != SMJOB_STATE_PENDING_HELD)
                    {
                        if (job_entry->job)
                        {
                            /* indicate GCP job completed with error */
                            DBG_MSG("Setting GCP status to ERROR for job %s\n", job_entry->job->id);
                            if (net_gcpp_job_status_update(job_entry->job->id, GCPP_JOB_STATUS_ERROR, NULL, NULL) != NET_GCPP_SUCCESS)
                            {
                                DBG_ERR("ERROR: Failed to set GCP job status to ERROR!\n");
                            }
                        }
                    }

                    /* remove job from SM JOB queue */
                    DBG_MSG("Forcing removal of SM job id = %d\n", job_entry->sm_id);
                    gcpp_oem_sm_remove_job_sm_id(job_entry->sm_id);
                    job_entry = NULL;
                }
                
                job_entry = gcpp_oem_sm_get_active_jobs(job_entry);
            }
        }
        else
        {
            //DBG_VERBOSE("No more jobs!\n");
            break;
        }
        
        if (waitForComplete)
        {
            posix_sleep_ticks( GCPP_OEM_DEFAULT_XMPP_EVENT_TIMEOUT_MS/10 );
        }
    }
    while (waitForComplete && net_gcpp_is_enabled(false));  
    
    //DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief process and print url directly
 * 
 **/
bool gcpp_oem_sm_print_url_direct(char *url)
{
    bool ret = FALSE;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    gcpp_job_t *job = net_gcpp_job_new();
    if (job)
    {
        smjob_ticket_t *default_ticket = NULL;
        
        job->id = gcpp_oem_strdup("INTERNAL_PAGE");
        job->printer_id = gcpp_oem_strdup("INTERNAL_PAGE_PRINT");
        job->title = gcpp_oem_strdup("INTERNAL PAGE");
        job->file_url = gcpp_oem_strdup(url);
        
        /* generate a default sm job ticket */
        default_ticket = gcpp_oem_sm_print_get_default_ticket();
        if (default_ticket)
        {
            uint32_t new_job_id = SMJOB_INVALID_JOB_ID;
            
            /* send job to SMJOB interface */
            new_job_id = gcpp_oem_sm_submit_job(job, default_ticket);
            if (new_job_id != SMJOB_INVALID_JOB_ID)
            {
                /* add to sm job list */
                gcpp_oem_sm_add_job(job, new_job_id);
            }
            else
            {
                DBG_ERR("ERROR : Failed to submit internal print job!\n");
                net_gcpp_job_delete(job);
            }
            
            /* delete job ticket */
            gcpp_oem_sm_delete_job_ticket(default_ticket);
        }
        else
        {
            DBG_ERR("ERROR : Failed to retrieve default sm job ticket!\n");
            net_gcpp_job_delete(job);
        }
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

/**
 *
 * \brief Client registers the event notification with the GCPP OEM module
 *
 */
void gcpp_oem_register_client_notification_func( gcpp_client_notification_func_t func)
{
    if (func != NULL)
    {
        g_gcpp_oem_print_settings.client_notify_func = func;
        
        DBG_ERR ("gcpp client notification function registered\n");
    }
    else
    {
        DBG_ERR("gcpp client notification function registered FAILED!\n");
    }
}

/** 
 * \brief start anonymous registration
 * 
 **/
bool gcpp_oem_register(void)
{
    // make sure that we are not in the midst of cancel 
    if (g_gcpp_oem_print_settings.cancel_req)
    {
        DBG_ERR("Cancel operation ongoing !\n");
        return FALSE;
    }
        
    // lower level APIs will return FAILURE if another registration is taking place
    // so we dont need to check that here again
    gcpp_error_t rcode = net_gcpp_issue_register_anonymous_request(NULL, true);
    if (rcode != NET_GCPP_SUCCESS)
    {
        DBG_ERR("Anonymous Registration request Failed, result = %d!\n", rcode);
        return FALSE;
    }
    gcpp_oem_set_gcp_reg_status(GCP_REG_IN_PROGRESS);
    
    return TRUE;
}

/** 
 * \brief cancel the whole registration process
 * 
 **/
bool gcpp_oem_cancel_registration(void)
{
    // make sure that we are in the middle of registration
    if (net_gcpp_is_registration_in_process())    
        g_gcpp_oem_print_settings.cancel_req = TRUE;

    return TRUE;
}

/** 
 * \brief un-register the existing GCP printer
 * 
 **/
bool gcpp_oem_unregister_printer(void)
{
    if (net_gcpp_is_registered(false))
        net_gcpp_registered(false);        

    gcpp_oem_set_gcp_reg_status(GCP_NOT_REGISTERED);

    return TRUE;
}

/** 
 * \brief suspend the GCPP operation
 * 
 **/
bool gcpp_oem_suspend_operation(void)
{
    if (net_gcpp_is_enabled(false))
    {
        if(net_gcpp_enable(false) == NET_GCPP_SUCCESS)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        DBG_MSG ("GCP operation is already disabled\n");
        return TRUE;
    }
}

/** 
 * \brief resume the GCP operation
 * 
 **/
bool gcpp_oem_resume_operation(void)
{
    if (!(net_gcpp_is_enabled(false)))
    {
        if(net_gcpp_enable(true) == NET_GCPP_SUCCESS)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        DBG_MSG ("GCP operation is already disabled\n");
        return TRUE;
    }
}

/** 
 * \brief suspend_resume the GCPP operation
 * 
 **/
bool gcpp_oem_suspend_resume_operation(void)
{
    if (net_gcpp_is_enabled(false))
    {
        gcpp_oem_set_suspend_resume_state(true);
        DBG_MSG ("GCP suspend operation\n");
        if(net_gcpp_enable(false) == NET_GCPP_SUCCESS)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        gcpp_oem_set_suspend_resume_state(false);
        DBG_MSG ("GCP resume operation \n");
        if(net_gcpp_enable(true) == NET_GCPP_SUCCESS)
            return TRUE;
        else
            return FALSE;
    }
    return FALSE;
}


static void* gcpp_oem_print_job_process_thread(void* entry_input)
{
    gcpp_print_job_request_t *job_request = NULL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    while (1)
    {
        DBG_MSG( "Waiting for print job request\n" );
        
        job_request = FListRemove(&gcpp_oem_print_job_list, FIFO_WAIT_FOREVER);
        if (job_request)
        {
            gcpp_error_t rcode;
            
            DBG_MSG( "Processing print job request, job id = %d\n", job_request->job_id );

            FListInit(&job_request->datafifo, 10);

            DBG_MSG( "Job %s, getting data from URL = %s\n", job_request->job->id, job_request->job->file_url );
            rcode = net_gcpp_printer_get_job(job_request->job->file_url, job_request->job->skip_pages, g_gcpp_oem_print_settings.supported_mime_types, NULL, (gcpp_recv_job_data_t)&gcpp_oem_sm_get_job_data_recv, job_request);
            if (rcode != NET_GCPP_SUCCESS)
            {
                DBG_ERR("Failed - net_gcpp_printer_get_job returned %d\n", rcode);
            }
        }
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );    
    return 0;
}


#ifdef HAVE_CMD

static const char* _autoprint_desc = "Toggle auto printing of registration pages";
static const char* _autoprint_usage = "";
static const char* _autoprint_notes = "";
static int _autoprint(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    if (g_gcpp_oem_print_settings.auto_print_registration_page)
    {
        DBG_MSG("Turning OFF auto print of registration pages\n");
        g_gcpp_oem_print_settings.auto_print_registration_page = FALSE;
    }
    else
    {
        DBG_MSG("Turning ON auto print of registration pages\n");
        g_gcpp_oem_print_settings.auto_print_registration_page = TRUE;
    }
    
    return CMD_OK;
}

static const char* _autoclaim_desc = "Toggle automatic Claim Printer requests till registration is complete or timeout expires";
static const char* _autoclaim_usage = "";
static const char* _autoclaim_notes = "";
static int _autopoll(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    if (g_gcpp_oem_print_settings.auto_claim_printer_for_registration_complete)
    {
        DBG_MSG("Turning OFF automatic Claim Printer requests\n");
        g_gcpp_oem_print_settings.auto_claim_printer_for_registration_complete = FALSE;
    }
    else
    {
        DBG_MSG("Turning ON automatic Claim Printer requests\n");
        g_gcpp_oem_print_settings.auto_claim_printer_for_registration_complete = TRUE;
    }
    
    return CMD_OK;
}

static const char* _joblist_desc = "List jobs currently in SM job queue";
static const char* _joblist_usage = "";
static const char* _joblist_notes = "";
static int _joblist(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    DBG_MSG("SM Jobs in Queue:\n");
    gcpp_oem_job_entry_t *job_entry = gcpp_oem_sm_get_active_jobs(NULL);
    if (job_entry != NULL)
    {
        while (job_entry)
        {
            smjob_rcode_t smjob_rcode;
            smjob_status_t job_status;
            
            smjob_rcode = smjob_get_status(job_entry->sm_id, &job_status);
            if(smjob_rcode == SMJOB_OK)
            {
                char *status_desc = "";
                
                ASSERT(job_entry->job);
                
                switch (job_status.state)
                {
                    case SMJOB_STATE_PENDING:
                        status_desc = "Pending";
                        break;
                    
                    case SMJOB_STATE_PENDING_HELD:
                        status_desc = "Pending Held";
                        break;
                    
                    case SMJOB_STATE_PROCESSING:
                        status_desc = "Processing";
                        break;
                    
                    case SMJOB_STATE_PROCESSING_STOPPED:
                        status_desc = "Processing Stopped";
                        break;
                    
                    case SMJOB_STATE_CANCELED:
                        status_desc = "Canceled";
                        break;
                        
                    case SMJOB_STATE_ABORTED:
                        status_desc = "Aborted";
                        break;
                    
                    case SMJOB_STATE_COMPLETED:
                        status_desc = "Completed";
                        break;
                    
                    default:
                        status_desc = "Unknown";
                        break;
                }
                
                DBG_MSG("  SM Job %d, GCP ID = %s, Status(0x%x) = %s\n", job_entry->sm_id, job_entry->job->id, job_status.state, status_desc);
            }
            else
            {
                DBG_MSG("  SM Job %d, GCP ID = %s, No Status!\n", job_entry->sm_id, job_entry->job->id);
            }
            

            job_entry = gcpp_oem_sm_get_active_jobs(job_entry);
        }
    }
    else
    {
        DBG_MSG("  *** No jobs found ***\n");
    }
    
    return CMD_OK;
}

#ifdef GCP_CLIENT_INTERFACE_TESTING
static const char* _gcp_client_reg_desc = "GCP Registeration";
static const char* _gcp_client_reg_usage = "";
static const char* _gcp_client_reg_notes = "";
static int _gcp_client_reg(int argc, char* argv[])
{
    bool ret;

    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    ret = gcpp_oem_register(); 
    if (ret == true)
        DBG_CMND ("Initiate GCP Registration successful\n");
    else
        DBG_CMND ("Initiate GCP Registration failed\n");    

    return CMD_OK;
}

static const char* _gcp_client_cancel_desc = "cancel GCP registration";
static const char* _gcp_client_cancel_usage = "";
static const char* _gcp_client_cancel_notes = "";
static int _gcp_client_cancel(int argc, char* argv[])
{
    bool ret;

    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    ret = gcpp_oem_cancel_registration(); 
    if (ret == true)
        DBG_CMND ("Initiate GCP cancellation successful");
    else
        DBG_CMND ("Initiate GCP cancellation failed");  

    return CMD_OK;
}

static const char* _gcp_client_unregister_desc = "unregister GCP registration";
static const char* _gcp_client_unregister_usage = "";
static const char* _gcp_client_unregister_notes = "";
static int _gcp_client_unregister(int argc, char* argv[])
{
    bool ret;

    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    ret = gcpp_oem_unregister_printer(); 
    if (ret == true)
        DBG_CMND ("Initiate GCP unregistration successful");
    else
        DBG_CMND ("Initiate GCP unregistration failed");    

    return CMD_OK;
}

#endif

#endif

//=========================================================================================================================
// OEM-specific function table definitions
//=========================================================================================================================

static gcpp_error_t gcpp_oem_print_init(void)
{
    uint32_t pthread_rcode;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Init()\n" );
    
    /* set default printer settings */
    gcpp_oem_default_print_settings( &g_gcpp_oem_print_settings );
    
    /* create OEM GCPP context lock mutex */
    pthread_rcode = posix_mutex_init(&g_gcpp_oem_print_settings.oem_lock_mutex);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating OEM GCPP mutex (pthread_rcode=0x%02x)\n", pthread_rcode);
        XASSERT(pthread_rcode == 0, pthread_rcode);
    }

    //For power off problem
    gcpp_oem_register_client_notification_func (&gcpp_client_notification_func);

    // Initialize the GCP OEM NVRAM module
    gcpp_oem_var_init ();

//    gcpp_event_create();

#ifdef HAVE_CMD
    cmd_register_subcmd("gcpp", "autoprint",
                            _autoprint_desc, _autoprint_usage, _autoprint_notes, _autoprint);
    cmd_register_subcmd("gcpp", "autopoll",
                            _autoclaim_desc, _autoclaim_usage, _autoclaim_notes, _autopoll);
    cmd_register_subcmd("gcpp", "joblist",
                            _joblist_desc, _joblist_usage, _joblist_notes, _joblist);
#ifdef GCP_CLIENT_INTERFACE_TESTING
    cmd_register_subcmd("gcpp", "client_reg",
                            _gcp_client_reg_desc, _gcp_client_reg_usage, _gcp_client_reg_notes, _gcp_client_reg);
    cmd_register_subcmd("gcpp", "client_cancel",
                            _gcp_client_cancel_desc, _gcp_client_cancel_usage, _gcp_client_cancel_notes, _gcp_client_cancel);
    cmd_register_subcmd("gcpp", "client_unregister",
                            _gcp_client_unregister_desc, _gcp_client_unregister_usage, _gcp_client_unregister_notes, _gcp_client_unregister);
#endif    
#endif

    /* create and kick-off GCPP print job request thread */
    FListInit(&gcpp_oem_print_job_list, 0);

    posix_create_thread( &gcpp_oem_print_job_request_thread,
                         gcpp_oem_print_job_process_thread,
                         0,
                         "gcppJobRequest",
                         gcpp_oem_print_job_request_stack,
                         OEM_THREAD_STACK_SIZE,
                         POSIX_THR_PRI_NORMAL);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

static gcpp_error_t gcpp_oem_print_setup(void)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Setup()\n" );

    if (net_gcpp_get_printer_name(true) == NULL)
    {
        char *printer_name;
        platvars_get_prtnamestr(&printer_name);
        net_gcpp_set_printer_name(printer_name);
    }

    if (net_gcpp_get_proxy(true) == NULL)
    {
        char *proxy;
        char temp[GCPP_MAX_AUTH_CREDENTIALS_LEN];
        
        /* 
           generating a unique proxy ID by base64 encoding serial number and printer name
           which should be unique for each printer.
         */
        memset(temp, 0, sizeof(temp));
        strncpy(temp, g_gcpp_oem_print_settings.serial_number, sizeof(temp)-1);
        strncat(temp, net_gcpp_get_printer_name(true), sizeof(temp)-1);
        proxy = gcpp_oem_base64(temp, strlen(temp));
        
        DBG_MSG ("Proxy ID is %s", proxy);                    
        
        net_gcpp_set_proxy(proxy);
        MEM_FREE_AND_NULL(proxy);
    }
    
    /* To avoid possible heavy GCPP polling traffic, adding delay here   */
    /* as a safe guard against a tight loop (like if the XMPP Connection */
    /* always fails)                                                     */
    time_t foo;
    time_t now = time(&foo);
    if (now < g_gcpp_oem_print_settings.start_expiration)
    {
        uint32_t delay = g_gcpp_oem_print_settings.start_expiration - now;
        
        /* make sure we don't delay longer than max required */
        if (delay > GCPP_OEM_DEFAULT_POLLING_TIMEOUT_SECS)
            delay = GCPP_OEM_DEFAULT_POLLING_TIMEOUT_SECS;
        
        DBG_MSG("Waiting for %d seconds before starting...\n", delay);
        posix_sleep_ticks(delay*100);
    }

    /* reset indication if job check has been done after XMPP Connection */
    g_gcpp_oem_print_settings.xmpp_connect_pre_job_check = false;
    g_gcpp_oem_print_settings.token_expiration = 0;
    g_gcpp_oem_print_settings.start_expiration = 0;

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

static gcpp_error_t gcpp_oem_print_start(void)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Start()\n" );

    if (g_gcpp_oem_print_settings.printer_id)
    {
        /* update printer attributes */
        net_gcpp_printer_update( );
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

static gcpp_error_t gcpp_oem_print_stop(void)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Stop()\n" );

    /* wait for all print jobs to complete, then remove all sm jobs from queue */
    gcpp_oem_sm_check_active_jobs(TRUE);
    gcpp_oem_sm_remove_all_jobs();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

static gcpp_error_t gcpp_oem_print_cleanup(void)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Cleanup()\n" );

    if (g_gcpp_oem_print_settings.user_email)
    {   
        MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.user_email);
        g_gcpp_oem_print_settings.user_email = NULL;
    }
    
    if (g_gcpp_oem_print_settings.registration_token)
    {   
        MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.registration_token);
        g_gcpp_oem_print_settings.registration_token = NULL;
    }
    
    if (g_gcpp_oem_print_settings.invite_page_url)
    {   
        MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.invite_page_url);
        g_gcpp_oem_print_settings.invite_page_url = NULL;
    }
    
    if (g_gcpp_oem_print_settings.complete_invite_url)
    {   
        MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.complete_invite_url);
        g_gcpp_oem_print_settings.complete_invite_url = NULL;
    }
    
    if (g_gcpp_oem_print_settings.polling_url)
    {   
        MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.polling_url);
        g_gcpp_oem_print_settings.polling_url= NULL;
    }
    
    if (g_gcpp_oem_print_settings.invite_url)
    {
        MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.invite_url);
        g_gcpp_oem_print_settings.invite_url = NULL;
    }

    if (g_gcpp_oem_print_settings.printer_id)
    {   
        MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.printer_id);
        g_gcpp_oem_print_settings.printer_id= NULL;
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}


static gcpp_error_t gcpp_oem_print_event_data(gcpp_event_t event, char *data, size_t size, void *callback_context)
{
    switch (event)
    {
        case NET_GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE:
        {
            DBG_MSG( "Anonymous Registration Data Event received:\n" );
        }
        break;
        
        /* completion of the polling request to determine if registration has been completed */
        case NET_GCPP_EVENT_REGISTRATION_CLAIM_PRINTER_COMPLETE:
        {
            DBG_MSG( "Registration Claim Printer Data Event recieved:\n" );
        }
        break;
        
        case NET_GCPP_EVENT_AUTH_TOKENS:
        {
            DBG_MSG( "Authorization Tokens Data Event recieved:\n" );
        }
        break;

        case NET_GCPP_EVENT_GET_PRINTER_ID:
        {
            DBG_MSG( "Get Printer ID Data Event recieved:\n" );
        }
        break;

        case NET_GCPP_EVENT_PRINTER_ADDED:
        {
            DBG_MSG( "Add Printer Data Event recieved:\n" );
        }
        break;
        
        case NET_GCPP_EVENT_PRINTER_UPDATED:
        {
            DBG_MSG( "Update Printer Data Event recieved:\n" );
        }
        break;

        case NET_GCPP_EVENT_PRINTER_REMOVED:
        {
            DBG_MSG( "Remove Printer Data Event recieved:\n" );
        }
        break;

        case NET_GCPP_EVENT_PRINTER_STATUS_UPDATED:
        {
            DBG_MSG( "Printer Status Update Data Event recieved:\n" );
        }
        break;

        case NET_GCPP_EVENT_XSRF_TOKEN_UPDATED:
        {
            DBG_MSG( "XSRF Token Update Data Event recieved:\n" );
        }
        break;

        case NET_GCPP_EVENT_PRINTER_JOBS:
        {
            DBG_MSG( "Printer Jobs Data Event recieved:\n" );
        }
        break;

        default:
        {
            DBG_ERR( "Unsupported GCPP Event = 0x%x!\n", event );
        }
        break;
    }
    
#ifdef DEBUG
    gcpp_oem_hex_dump( data, size );
#endif
    
    return NET_GCPP_SUCCESS;
}

static gcpp_error_t gcpp_oem_print_event_notification(gcpp_event_t event, void *event_context, void *callback_context)
{
    bool failure = FALSE;
    //DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    //DBG_VERBOSE( "Event Notification()\n" );
    switch (event)
    {
        /* completion of the anonymous registration request */
        case NET_GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE:
        {
            gcpp_registration_anonymous_context_t *anon_reg = (gcpp_registration_anonymous_context_t *)event_context;
            DBG_MSG( "Anonymous Registration Complete Event received.\n" );

            if (g_gcpp_oem_print_settings.cancel_req)
            {                
                /* throw away all the data for current reg */
                net_gcpp_registered (FALSE);

                /* notify client that the cancel is done */
                if (g_gcpp_oem_print_settings.client_notify_func)
                {
                    g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_PRINTER_REMOVED, NULL);
                }
                g_gcpp_oem_print_settings.cancel_req = FALSE;
                DBG_VERBOSE ("%s: Cancellation complete at end of Anon Reg\n", __FUNCTION__);                
                break;
            }
            
            if (anon_reg)
            {
                switch (anon_reg->hdr.result)
                {
                    case NET_GCPP_SUCCESS:
                    {
                        DBG_ERR("Anonymous Registration response values:\n");
                        if (anon_reg->registration_token)
                            DBG_ERR("  registration_token = %s\n", anon_reg->registration_token);
                            
                        if (anon_reg->token_duration)
                            DBG_ERR("  token_duration = %s\n", anon_reg->token_duration);
                            
                        if (anon_reg->invite_url)
                            DBG_ERR("  invite_url = %s\n", anon_reg->invite_url);
                            
                        if (anon_reg->complete_invite_url)
                            DBG_ERR("  complete_invite_url = %s\n", anon_reg->complete_invite_url);
                            
                        if (anon_reg->invite_page_url)
                            DBG_ERR("  invite_page_url = %s\n", anon_reg->invite_page_url);
                            
                        if (anon_reg->polling_url)
                            DBG_ERR("  polling_url = %s\n", anon_reg->polling_url);

                        if (anon_reg->printer_id)
                            DBG_ERR("  printer_id = %s\n", anon_reg->printer_id);                        
                        /* keep latest registration token */
                        if (g_gcpp_oem_print_settings.registration_token)
                        {   
                            MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.registration_token);
                        }
                        g_gcpp_oem_print_settings.registration_token = gcpp_oem_strdup(anon_reg->registration_token);

                        /* keep latest invite page url */
                        if (g_gcpp_oem_print_settings.invite_page_url)
                        {   
                            MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.invite_page_url);
                        }
                        g_gcpp_oem_print_settings.invite_page_url = gcpp_oem_strdup(anon_reg->invite_page_url);

                        /* keep complete invite url */
                        if (g_gcpp_oem_print_settings.complete_invite_url)
                        {   
                            MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.complete_invite_url);
                        }
                        g_gcpp_oem_print_settings.complete_invite_url = gcpp_oem_strdup(anon_reg->complete_invite_url);

                        /* keep latest polling url */
                        if (g_gcpp_oem_print_settings.polling_url)
                        {   
                            MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.polling_url);
                        }
                        g_gcpp_oem_print_settings.polling_url = gcpp_oem_strdup(anon_reg->polling_url);

                        /* keep latest invite url */
                        if (g_gcpp_oem_print_settings.invite_url)
                        {
                            MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.invite_url);
                        }
                        g_gcpp_oem_print_settings.invite_url = gcpp_oem_strdup(anon_reg->invite_url);

                        /* send invite page url to printer, if required */
                        if (g_gcpp_oem_print_settings.invite_page_url && g_gcpp_oem_print_settings.auto_print_registration_page)
                        {
                            DBG_MSG("Send invite page url to printer...\n");
                            gcpp_oem_sm_print_url_direct(g_gcpp_oem_print_settings.invite_page_url);
                        }

                        /* printer ID */
                        if (anon_reg->printer_id)
                        {
                            if (g_gcpp_oem_print_settings.printer_id)
                            {   
                                MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.printer_id);
                            }
                            g_gcpp_oem_print_settings.printer_id = gcpp_oem_strdup(anon_reg->printer_id);
                            net_gcpp_set_printer_id(anon_reg->printer_id);
                        }
                        
                        /* store the token duration by adding it to current system time */
                        time_t foo;
                        time_t now = time(&foo);
                        g_gcpp_oem_print_settings.reg_token_duration = now + atoi(anon_reg->token_duration);
                        DBG_MSG("  reg_token_duration in %ld seconds\n", (g_gcpp_oem_print_settings.reg_token_duration));
                        
                        /* notify reg client so that it can show the claim URL */
                        /* client will use _get API to get all the details     */
                        if (g_gcpp_oem_print_settings.client_notify_func)
                        {
                            g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE, NULL);
                        }
                        
                        /* start polling to determine if registration is complete, if required */
                        if (g_gcpp_oem_print_settings.polling_url && 
                            g_gcpp_oem_print_settings.auto_claim_printer_for_registration_complete)
                        {
                            if (net_gcpp_is_registration_in_process())
                            {
                                gcpp_error_t rcode;
                                
                                DBG_MSG("Starting polling to determine if GCP has been claimed...\n");
                                rcode = net_gcpp_issue_claim_printer_request(g_gcpp_oem_print_settings.polling_url, NULL, true);
                                if (rcode != NET_GCPP_SUCCESS)
                                {
                                    DBG_MSG("Register Completion request Failed, result = %d!\n", rcode);
                                }
                            }
                            else
                            {
                                DBG_MSG("Registration no longer in process!\n");
                            }
                        }
                        break;
                    }
                    
                    case NET_GCPP_FAIL:
                    {
                        DBG_ERR( "Anonymous Registration failed!\n");
                        failure = TRUE;
                        break;
                    }
                    
                    case NET_GCPP_HTTP_FILE_NOT_FOUND:
                    {
                        DBG_ERR( "Anonymous Registration error - File not found!\n" );
                        failure = TRUE;
                        break;
                    }
                    
                    case NET_GCPP_HTTP_FILE_CONVERSION_FAILED:
                    {
                        DBG_ERR( "Anonymous Registration error - File conversion failed!\n" );
                        failure = TRUE;
                        break;
                    }
                    
                    case NET_GCPP_HTTP_UNSUPPORTED_RESPONSE_CODE:
                    {
                        DBG_ERR( "Anonymous Registration error - Server response code %d!\n", anon_reg->hdr.response_code );
                        failure = TRUE;                        
                        break;
                    }
                    
                    default:
                        DBG_ERR( "Anonymous Registration failed, result = %d!\n", anon_reg->hdr.result);
                        failure = TRUE;                        
                        break;
                }
            }
            else
            {
                DBG_ERR( "Anonymous Registration failure!\n" );
                failure = TRUE;                
            }

            /* notify client that registration has failed */
            if (failure)
            {
                net_gcpp_registered (FALSE);
                if (g_gcpp_oem_print_settings.client_notify_func)
                    g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_REGISTRATION_FAILED, NULL);
            }            
        }
        break;
        
        /* completion of the polling request to determine if registration has been completed */
        case NET_GCPP_EVENT_REGISTRATION_CLAIM_PRINTER_COMPLETE:
        {
            gcpp_registration_claim_printer_context_t *reg_completion = (gcpp_registration_claim_printer_context_t *)event_context;
            DBG_MSG( "Registration Claim Printer Event recieved.\n" );

            if (g_gcpp_oem_print_settings.cancel_req)
            {                
                /* throw away all the data for current reg */
                net_gcpp_registered (FALSE);

                /* notify client that the cancel is done */
                if (g_gcpp_oem_print_settings.client_notify_func)
                {
                    g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_PRINTER_REMOVED, NULL);
                }
                g_gcpp_oem_print_settings.cancel_req = FALSE;
                DBG_VERBOSE ("%s: Cancellation complete at end of Claim Printer return\n", __FUNCTION__);                
                break;
            }
            
            if (reg_completion)
            {
                //switch (reg_completion->hdr.result)
                switch (reg_completion->hdr.success)
                {
                    //case NET_GCPP_SUCCESS:
                    case TRUE:
                    {
                        gcpp_error_t rcode;
                        
                        DBG_MSG("Registration Claim Printer response values:\n");
                        if (reg_completion->xmpp_jid)
                        {
                            DBG_MSG("  xmpp_jid = %s\n", reg_completion->xmpp_jid);
                            
                            /* update and store xmpp JID var */
                            net_gcpp_set_xmpp_jid(reg_completion->xmpp_jid);
                        }
                        if (reg_completion->authorization_code)
                            DBG_MSG("  authorization_code = %s\n", reg_completion->authorization_code);
                            
                        if (reg_completion->confirmation_page_url)
                            DBG_MSG("  confirmation_page_url = %s\n", reg_completion->confirmation_page_url);

                        if (reg_completion->user_email)
                            DBG_MSG("  user_email = %s\n", reg_completion->user_email);

                        /* keep user email address */
                        if (g_gcpp_oem_print_settings.user_email)
                        {   
                            MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.user_email);
                        }
                        g_gcpp_oem_print_settings.user_email = gcpp_oem_strdup(reg_completion->user_email);

                        /* issue request to obtain all authorization tokens needed for GCP */
                        DBG_MSG("Issuing request for Authorization Tokens...\n");
                        bool registration_request = true;
                        rcode = net_gcpp_issue_authorization_tokens_request(reg_completion->authorization_code, &registration_request, false);
                        if (rcode != NET_GCPP_SUCCESS)
                        {
                            DBG_MSG("Authorization Tokens request Failed, result = %d!\n", rcode);
                        }

                        break;
                    }
                    
                    //case NET_GCPP_FAIL:
                    case FALSE:
                    {
                        DBG_ERR( "Registration Claim Printer not successful!\n");

                        /* check against the token_duration before raising the request again */
                        time_t foo;
                        time_t now = time(&foo);
                        if (now > g_gcpp_oem_print_settings.reg_token_duration &&
                            net_gcpp_is_registration_in_process())
                        {
                            failure = TRUE;
                            break;
                        }
                        
                        /* retry Claim Printer if auto poll for registration is enabled */
                        if (g_gcpp_oem_print_settings.polling_url && 
                            g_gcpp_oem_print_settings.auto_claim_printer_for_registration_complete &&
                            net_gcpp_is_registration_in_process())
                        {
                            gcpp_error_t rcode;
                            
                            DBG_MSG("Registration Token = %s\n", g_gcpp_oem_print_settings.registration_token);
                            DBG_MSG("Retry Claim Printer after %d second delay...\n", GCPP_OEM_DEFAULT_POLLING_TIMEOUT_SECS);
                            
                            int countdown = GCPP_OEM_DEFAULT_POLLING_TIMEOUT_SECS;
                            while (countdown-- > 0 && net_gcpp_is_registration_in_process() &&
                                    !g_gcpp_oem_print_settings.cancel_req)
                            {
                                posix_sleep_ticks(100);
                            }

                            if (g_gcpp_oem_print_settings.cancel_req)
                            {                
                                /* throw away all the data for current reg */
                                net_gcpp_registered (FALSE);
                            
                                /* notify client that the cancel is done */
                                if (g_gcpp_oem_print_settings.client_notify_func)
                                {
                                    g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_PRINTER_REMOVED, NULL);
                                }
                                g_gcpp_oem_print_settings.cancel_req = FALSE;
                                DBG_VERBOSE ("%s: Cancellation complete during the middle of claim printer\n", __FUNCTION__);                
                                break;
                            }

                            if (net_gcpp_is_registration_in_process())
                            {
                                rcode = net_gcpp_issue_claim_printer_request(g_gcpp_oem_print_settings.polling_url, NULL, true);
                                if (rcode != NET_GCPP_SUCCESS)
                                {
                                    DBG_MSG("Register Completion retry request Failed, result = %d!\n", rcode);
                                }
                            }
                            else
                            {
                                DBG_MSG("Registration no longer in process!\n");
                            }
                        }
                        break;
                    }
#if 0                   
                    case NET_GCPP_HTTP_FILE_NOT_FOUND:
                    {
                        DBG_ERR( "Registration Claim Printer error - File not found!\n" );
                        failure = TRUE;                        
                        break;
                    }
                    
                    case NET_GCPP_HTTP_FILE_CONVERSION_FAILED:
                    {
                        DBG_ERR( "Registration Claim Printer error - File conversion failed!\n" );
                        failure = TRUE;                        
                        break;
                    }
                    
                    case NET_GCPP_HTTP_UNSUPPORTED_RESPONSE_CODE:
                    {
                        DBG_ERR( "Registration Claim Printer error - Server response code %d!\n", reg_completion->hdr.response_code );
                        failure = TRUE;                        
                        break;
                    }
#endif                  
                    default:
                        DBG_ERR( "Registration Claim Printer failed, result = %d!\n", reg_completion->hdr.result);
                        failure = TRUE;                        
                        break;
                }
            }
            else
            {
                DBG_ERR( "Registration Claim Printer failure!\n" );
                failure = TRUE;
            }

            /* notify client that registration has failed */
            if (failure)
            {
                net_gcpp_registered (FALSE);
                if (g_gcpp_oem_print_settings.client_notify_func)
                    g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_REGISTRATION_FAILED, NULL);
            }            
        }
        break;
        
        case NET_GCPP_EVENT_AUTH_TOKENS:
        {
            gcpp_authorization_tokens_context_t *auth_tokens = (gcpp_authorization_tokens_context_t *)event_context;
            bool *registration_request = (callback_context == NULL) ? false : (bool *)callback_context;
            
            DBG_MSG( "Authorization Tokens Event recieved.\n" );

            if ( (registration_request && *registration_request) && g_gcpp_oem_print_settings.cancel_req )
            {                
                /* throw away all the data for current reg */
                net_gcpp_registered (FALSE);

                /* notify client that the cancel is done */
                if (g_gcpp_oem_print_settings.client_notify_func)
                {
                    g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_PRINTER_REMOVED, NULL);
                }
                g_gcpp_oem_print_settings.cancel_req = FALSE;
                DBG_VERBOSE ("%s: Cancellation complete at end of Auth token recv\n", __FUNCTION__);                
                break;
            }
            
            if (auth_tokens)
            {
                switch (auth_tokens->hdr.result)
                {
                    case NET_GCPP_SUCCESS:
                    {
                        time_t foo;
                        time_t now = time(&foo);
                        
                        if (auth_tokens->access_token)
                        {
                            DBG_MSG("  access_token = %s\n", auth_tokens->access_token);
                        }
                            
                        if (auth_tokens->refresh_token)
                        {
                            DBG_MSG("  refresh_token = %s\n", auth_tokens->refresh_token);
                            
                            /* update and store refresh token var */
                            net_gcpp_set_refresh_token(auth_tokens->refresh_token);
                        }
                            
                        if (auth_tokens->expires_in)
                        {
                            g_gcpp_oem_print_settings.token_expiration = now + auth_tokens->expires_in;
                        }
                        else
                        {
                            g_gcpp_oem_print_settings.token_expiration = now + GCPP_OEM_DEFAULT_TOKEN_TIMEOUT_SECS;
                        }
                        DBG_MSG("  token_expiration in %d seconds\n", (g_gcpp_oem_print_settings.token_expiration-now));

                        if (registration_request && *registration_request)
                        {
                            /* indicate registration has completed */
                            DBG_MSG("Registration completed successfully!\n");
                            net_gcpp_registered(true);
                        }
                        
                        break;
                    }
                    
                    case NET_GCPP_FAIL:
                    {
                        DBG_ERR( "Authorization Tokens not successful!\n");
                        failure = TRUE;
                        break;
                    }
                    
                    case NET_GCPP_HTTP_FILE_NOT_FOUND:
                    {
                        DBG_ERR( "Authorization Tokens error - File not found!\n" );
                        failure = TRUE;                        
                        break;
                    }
                    
                    case NET_GCPP_HTTP_FILE_CONVERSION_FAILED:
                    {
                        DBG_ERR( "Authorization Tokens error - File conversion failed!\n" );
                        failure = TRUE;                        
                        break;
                    }
                    
                    case NET_GCPP_HTTP_UNSUPPORTED_RESPONSE_CODE:
                    {
                        DBG_ERR( "Authorization Tokens error - Server response code %d!\n", auth_tokens->hdr.response_code );
                        failure = TRUE;                        
                        break;
                    }
                    
                    default:
                        DBG_ERR( "Authorization Tokens failed, result = %d!\n", auth_tokens->hdr.result);
                        failure = TRUE;                        
                        break;
                }
            }
            else
            {
                DBG_ERR( "Authorization Tokens failure!\n" );
                failure = TRUE;                
            }

            /* notify client that registration has failed */
            if (failure)
            {
                if (registration_request && *registration_request)
                {
                    net_gcpp_registered (FALSE);
                    if (g_gcpp_oem_print_settings.client_notify_func)
                        g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_REGISTRATION_FAILED, NULL);
                }            
            }
        }
        break;

        case NET_GCPP_EVENT_REGISTERED:
        {
            DBG_MSG( "GCPP Registered\n" );
            /* notify client that registration is SUCCESSFUL */
            if (g_gcpp_oem_print_settings.client_notify_func)
                g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_REGISTRATION_SUCCESS, NULL);
        }
        break;

        case NET_GCPP_EVENT_UNREGISTERED:
        {
            DBG_MSG( "GCPP Unregistered\n" );
        }
        break;

        case NET_GCPP_EVENT_NETWORK_CONNECTED:
        {
            DBG_MSG( "GCPP Network Connected\n" );
        }
        break;

        case NET_GCPP_EVENT_NETWORK_DISCONNECTED:
        {
            DBG_MSG( "GCPP Network Disconnected\n" );
        }
        break;

        case NET_GCPP_EVENT_XMPP_CONNECTED:
        {
            DBG_MSG( "GCPP XMPP Connected\n" );
        }
        break;

        case NET_GCPP_EVENT_XMPP_DISCONNECTED:
        {
            DBG_MSG( "GCPP XMPP Disconnected\n" );
        }
        break;

        case NET_GCPP_EVENT_XMPP_IDLE:
        {
            //DBG_VERBOSE( "GCPP XMPP Idle\n" );

            /* After XMPP Connection, check jobs once after start_expiration - unless pending job 
                notification happens beforehand.
            */
            time_t foo;
            time_t now = time(&foo);
            if (!g_gcpp_oem_print_settings.xmpp_connect_pre_job_check && 
               (g_gcpp_oem_print_settings.start_expiration == 0 || now > g_gcpp_oem_print_settings.start_expiration) )
            {
                g_gcpp_oem_print_settings.xmpp_connect_pre_job_check = true;
                
                DBG_MSG("Processing any existing print jobs in queue\n");
                if (!gcpp_oem_sm_process_print_jobs())
                {
                    DBG_ERR("ERROR : Unable to process print jobs!\n");
                    
                    /* hard failure, attempting to restart gcpp */
                    net_gcpp_printer_restart();
                }
                
                g_gcpp_oem_print_settings.start_expiration = now + GCPP_OEM_DEFAULT_POLLING_TIMEOUT_SECS;
            }
            
            now = time(&foo);
            if (g_gcpp_oem_print_settings.token_expiration == 0 || now > g_gcpp_oem_print_settings.token_expiration)
            {
                DBG_MSG("Refresh authorization tokens\n");
                if (net_gcpp_issue_refresh_authorization_tokens_request(NULL, false) != NET_GCPP_SUCCESS)
                {
                    DBG_ERR("ERROR : Unable to issue request to update tokens!\n");
                    
                    /* hard failure, attempting to restart gcpp */
                    net_gcpp_printer_restart();
                }
            }
            
            /* check active sm jobs */
            //gcpp_oem_sm_check_active_jobs(FALSE);
        }
        break;

        case NET_GCPP_EVENT_GET_PRINTER_ID:
        {
            char *printer_id = (char *)event_context;
            
            if (printer_id)
            {
                DBG_MSG( "Get GCPP Printer ID = %s\n", printer_id );
                
                if (g_gcpp_oem_print_settings.printer_id)
                    MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.printer_id);
                    
                g_gcpp_oem_print_settings.printer_id = gcpp_oem_strdup(printer_id);
            }
        }
        break;

        case NET_GCPP_EVENT_PRINTER_ADDED:
        {
            char *printer_id = (char *)event_context;
            
            if (printer_id)
            {
                DBG_MSG( "GCPP Printer added, ID = %s\n", printer_id );
                
                if (g_gcpp_oem_print_settings.printer_id)
                    MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.printer_id);
                    
                g_gcpp_oem_print_settings.printer_id = gcpp_oem_strdup(printer_id);
            }
        }
        break;
        
        case NET_GCPP_EVENT_PRINTER_UPDATED:
        {
            char *printer_id = (char *)event_context;
            
            if (printer_id)
            {
                DBG_MSG( "GCPP Printer updated, ID = %s\n", printer_id );
                
                if (g_gcpp_oem_print_settings.printer_id)
                    MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.printer_id);
                    
                g_gcpp_oem_print_settings.printer_id = gcpp_oem_strdup(printer_id);
            }
        }
        break;

        case NET_GCPP_EVENT_PRINTER_REMOVED:
        {
            char *printer_id = (char *)event_context;
            
            if (printer_id)
            {
                DBG_MSG( "GCPP Printer removed, ID = %s\n", printer_id );
                
                if (g_gcpp_oem_print_settings.printer_id)
                    MEM_FREE_AND_NULL(g_gcpp_oem_print_settings.printer_id);
                    
                g_gcpp_oem_print_settings.printer_id = NULL;
            }

            /* notify client that the printer is removed */
            if (g_gcpp_oem_print_settings.client_notify_func)
            {
                g_gcpp_oem_print_settings.client_notify_func (GCPP_EVENT_PRINTER_REMOVED, NULL);
            }
        }
        break;

        case NET_GCPP_EVENT_PRINTER_STATUS_UPDATED:
        {
            gcpp_printer_status_context_t *printer_status = (gcpp_printer_status_context_t *)event_context;
            
            if (printer_status)
            {
                DBG_MSG( "GCPP Printer Status Updated:\n" );
                DBG_MSG("   Job Status - ID = %s, Status = %s\n", printer_status->job_id, printer_status->status );
            }
        }
        break;
        
        case NET_GCPP_EVENT_PRINTER_STATE_CHANGED:
        {
            gcpp_state_context_t *states = (gcpp_state_context_t *)event_context;
            
            if (states)
            {
                DBG_MSG( "GCPP States Changed\n" );
                
                switch(states->hdr.result)
                {
                    case NET_GCPP_SUCCESS:
                        break;
                        
                    case NET_GCPP_XMPP_CONNECTION_FAILED:
                        DBG_MSG("   XMPP Connection Failed!\n" );
                        break;
                        
                    case NET_GCPP_PROCESS_PRINT_JOBS_FAILED:
                        DBG_MSG("   Failed to process print jobs!\n" );
                        break;
                        
                    case NET_GCPP_NETWORK_DISCONNECTED:
                        DBG_MSG("   Network was disconnected!\n" );
                        break;
                        
                    case NET_GCPP_NETWORK_CHANGED:
                        DBG_MSG("   Network state changed!\n" );
                        break;
                        
                    default :
                        DBG_MSG("   Unknown state error!\n" );
                        break;
                }
                
                switch(states->run_state)
                {
                    case GCPP_UNCHANGED:
                        break;
                    
                    case GCPP_INIT:
                        DBG_MSG("   Run State = Initializing\n" );
                        break;
                        
                    case GCPP_WAIT_FOR_CONFIGURED:
                        DBG_MSG("   Run State = Waiting to be configured\n" );
                        break;

                    case GCPP_CONFIGURED:
                        DBG_MSG("   Run State = Configured\n" );
                        
                        /* refresh auth tokens */
                        DBG_MSG("Refreshing auth tokens\n");
                        net_gcpp_issue_refresh_authorization_tokens_request(NULL, false);
                        break;
                        
                    case GCPP_STARTED:
                        DBG_MSG("   Run State = Started\n" );
                        break;
                        
                    case GCPP_STOPPED:
                        DBG_MSG("   Run State = Stopped\n" );
                        break;
                        
                    default :
                        DBG_MSG("   Run State = Unknown\n" );
                }
                
                switch(states->job_state)
                {
                    case GCPP_JOBS_UNCHANGED:
                        break;
                    
                    case GCPP_JOBS_INIT:
                        DBG_MSG("   Job State = Initializing\n" );
                        break;
                        
                    case GCPP_JOBS_IDLE:
                        DBG_MSG("   Job State = Idle\n" );
                        break;
                        
                    case GCPP_JOBS_PENDING:
                        DBG_MSG("   Job State = Jobs Pending\n" );
                        break;
                        
                    case GCPP_JOBS_END:
                        DBG_MSG("   Job State = Done\n" );
                        break;
                        
                    default :
                        DBG_MSG("   Job State = Unknown" );
                }
                
                if (states->run_state == GCPP_STARTED && states->job_state == GCPP_JOBS_IDLE)
                {
                    /* attempt to handle any existing jobs in the queue ... checking now in case any
                        jobs were missed which were sumbitted before xmpp notification was setup, or if xmpp
                        fails to connect for some reason (like ports being blocked).
                    */

                    /* polling for jobs should only happen once every 30 seconds */
                    time_t foo;
                    time_t now = time(&foo);
                    if (g_gcpp_oem_print_settings.start_expiration == 0 || now > g_gcpp_oem_print_settings.start_expiration)
                    {
                        DBG_MSG("Processing any existing print jobs in queue\n");
                        if (!gcpp_oem_sm_process_print_jobs())
                        {
                            /* hard failure, attempting to restart gcpp */
                            net_gcpp_printer_restart();
                        }
                        
                        g_gcpp_oem_print_settings.start_expiration = now + GCPP_OEM_DEFAULT_POLLING_TIMEOUT_SECS;
                    }
                }
                else if (states->job_state == GCPP_JOBS_PENDING)
                {
                    g_gcpp_oem_print_settings.xmpp_connect_pre_job_check = true;
                    
                    /* Event indicates new print job(s) have been submitted to the queue */
                    DBG_MSG("Processing new print jobs in queue\n");
                    if (!gcpp_oem_sm_process_print_jobs())
                    {
                        /* hard failure, attempting to restart gcpp */
                        net_gcpp_printer_restart();
                    }
                }
            }
            else
            {
            }
        }
        break;

        case NET_GCPP_EVENT_XSRF_TOKEN_UPDATED:
        {
            DBG_MSG( "XSRF Token Updated\n" );
            
#ifdef DEBUG
            char *xsrf_token = (char *)event_context;
            
            if (xsrf_token)
            {
                DBG_MSG( "New XSRF Token = %s\n", xsrf_token );
            }
#endif            
        }
        break;

        case NET_GCPP_EVENT_PRINTER_JOBS:
        {
            gcpp_job_t **jobs = (gcpp_job_t **)event_context;;
            if (jobs)
            {
                DBG_MSG( "Printer Jobs Recieved\n" );
                
#ifdef DEBUG
                gcpp_job_t *job = NULL;
                int index = 0;
                
                while ((job = jobs[index++]) != NULL)
                {
                    DBG_MSG("Job[%d] %s\n", index, job->id);
                }
#endif            
            }
            else
            {
                DBG_MSG( "No Printer Jobs Recieved\n" );
            }
        }
        break;
        
        default:
        {
            DBG_ERR( "Unsupported GCPP Event = 0x%x!\n", event );
        }
        break;
    }
    
    //DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

static char* gcpp_oem_print_get_printer_capabilities(void)
{
    char *data = NULL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Get Printer Capabilities()\n" );

    gcpp_oem_print_config_t* config = net_gcpp_get_oem_print_config();
    ASSERT(config);
    
    data = gcpp_oem_ppd_get_printer_capabilities(config, &g_gcpp_oem_print_settings);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return data;
}

static char* gcpp_oem_print_get_printer_defaults(void)
{
    char *data = NULL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Get Printer Defaults()\n" );

    gcpp_oem_print_config_t* config = net_gcpp_get_oem_print_config();
    ASSERT(config);
    
    data = gcpp_oem_ppd_get_printer_capabilities(config, &g_gcpp_oem_print_settings);

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return data;
}


//=========================================================================================================================
// Public configuration routines
//=========================================================================================================================

/*
 * populate any of the OEM-specific functions below that are needed
 * NOTE: any unused function pointers MUST be initialized to NULL. 
 */
gcpp_error_t gcpp_oem_print_populate_func_table(gcpp_oem_print_func_table_t* func_table)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Populate Function Table()\n" );

    ASSERT( func_table );

    func_table->init                           = &gcpp_oem_print_init;
    func_table->setup                          = &gcpp_oem_print_setup;
    func_table->start                          = &gcpp_oem_print_start;
    func_table->stop                           = &gcpp_oem_print_stop;
    func_table->cleanup                        = &gcpp_oem_print_cleanup;
    func_table->event_notification             = &gcpp_oem_print_event_notification;
    func_table->event_data                     = &gcpp_oem_print_event_data;
    func_table->get_printer_capabilities       = &gcpp_oem_print_get_printer_capabilities;
    func_table->get_printer_defaults           = &gcpp_oem_print_get_printer_defaults;

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

/*
 * populate any of the OEM-specific configuration values below
 */
gcpp_error_t gcpp_oem_print_populate_config(gcpp_oem_print_config_t* config)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG( "Populate Config()\n" );

    ASSERT( config );
        
    /* setup list of alternate XMPP ports as fallback */
    config->xmpp_num_alt_ports = 0;
    config->xmpp_alternate_ports[config->xmpp_num_alt_ports++] = 5223;

    config->oauth_client_id = GCPP_OEM_OAUTH_CLIENT_ID;
    config->oauth_client_secret = GCPP_OEM_OAUTH_CLIENT_SECRET;
    
    config->xmpp_connect_timeout_ms = GCPP_OEM_DEFAULT_XMPP_CONNECT_TIMEOUT_MS;
    config->xmpp_event_timeout_ms = GCPP_OEM_DEFAULT_XMPP_EVENT_TIMEOUT_MS;
    config->xmpp_keep_alive_delay_ms = GCPP_OEM_DEFAULT_KEEP_ALIVE_DELAY_MS;
    config->start_timeout_ms = GCPP_OEM_DEFAULT_START_TIMEOUT_MS;
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return NET_GCPP_SUCCESS;
}

char* gcpp_oem_get_printer_id( void )
{
    return (g_gcpp_oem_print_settings.printer_id);
}

char* gcpp_oem_get_registration_token( void )
{
    return (g_gcpp_oem_print_settings.registration_token);
}

char* gcpp_oem_get_invite_page_url( void )
{
    return (g_gcpp_oem_print_settings.invite_page_url);
}
char* gcpp_oem_get_invite_url( void )
{
    return (g_gcpp_oem_print_settings.invite_url);
}

char* gcpp_oem_get_complete_invite_url( void )
{
    return (g_gcpp_oem_print_settings.complete_invite_url);
}

unsigned long gcpp_oem_get_token_duration( void )
{
    return ((unsigned long)g_gcpp_oem_print_settings.reg_token_duration);
}

char* gcpp_oem_get_user_email( void )
{
    return (g_gcpp_oem_print_settings.user_email);
}

bool gcpp_oem_get_network_status(void)
{
    if(net_gcpp_is_network_connected())
    {
        return true;
    }
    return false;
}

void gcpp_oem_set_gcp_reg_status(GCP_REG_STATUS status)
{
    gcpp_oem_reg_status = status;
}

GCP_REG_STATUS gcpp_oem_get_gcp_reg_status(void)
{
    return gcpp_oem_reg_status;
}

bool gcpp_oem_is_registration_in_process( void )
{
    return net_gcpp_is_registration_in_process();
}

/** 
 * \brief is gcp registered
 * 
 **/
bool gcpp_oem_is_registered(bool val)
{
    return net_gcpp_is_registered(val);
}
/** 
 * \brief return suspend resume status
 * 
 *  true if suspended false if resumed
 **/
bool gcpp_oem_get_suspend_resume_state(void)
{
    return bOperationSuspended;
}
void gcpp_oem_set_suspend_resume_state(bool state)
{
    bOperationSuspended = state;
}

