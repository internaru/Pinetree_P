/******************************************************************************
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <stdio.h>
#include "platform.h"
#include "ATypes.h"
#include "lassert.h"
#include "memAPI.h"
#include "dprintf.h"
#include "debug.h"
#include "logger.h"
#include "paper_size_api.h"
#include "paper_types_api.h"
#include "net_logger.h"
#include "sm_job_api.h"
#include "sm_job_support_api.h"
#include "sm_job_internal_api.h"
#include "sm_job.h"
#include "pthread.h"
#include "posix_ostools.h"
//#include "timer_api.h"
//#include "error_types.h"

/** \brief This file implements the core functionality for the smjob API. */

//=============================================================================
// Defines
//=============================================================================
#define DBG_PRFX "SMJOB: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_SM_JOB

//// TODO TEMP release debug
//#define DBG_ERR(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_MSG(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) dbg_printf(DBG_PRFX __VA_ARGS__)

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)


//rdj TODO allow to be modified/overridden by OEM config
/* Strings in this list must have a one-to-one correspondence with the values defined by
 * the smjob_format_t enum.
 * 
 * These strings must conform to the IPP datatype definition of 'mimeMediaType':
 * http://tools.ietf.org/html/rfc2911#section-4.1.9
 */
static char *g_smjob_format_map[] = 
{
    "", // SMJOB_FORMAT_UNKNOWN
    "application/octet-stream", // SMJOB_FORMAT_APPLICATION_OCTET_STREAM
    "text/plain", // SMJOB_FORMAT_TEXT_PLAIN
    "image/jpeg", // SMJOB_FORMAT_IMAGE_JPEG
    "application/pdf", // SMJOB_FORMAT_PDF
    "application/postscript", // SMJOB_FORMAT_APPLICATION_POSTSCRIPT
    "application/vnd.hp-PCL", // SMJOB_FORMAT_APPLICATION_PCL
    "application/PCLm", // SMJOB_FORMAT_APPLICATION_PCLM
    "image/urf", // SMJOB_FORMAT_IMAGE_URF
};
uint32_t g_smjob_format_map_size = sizeof(g_smjob_format_map)/sizeof(char *);

/* Strings in this list must have a one-to-one correspondence with the values defined by
 * the smjob_sides_t enum.
 * 
 * These strings must match the IPP keyword strings defined by the 'sides' job template
 * attribute: http://tools.ietf.org/html/rfc2911#section-4.2.8
 */
static char *g_smjob_sides_map[] =
{
    "", // SMJOB_SIDES_UNKNOWN
    "one-sided", // SMJOB_SIDES_ONE_SIDED
    "two-sided-long-edge", // SMJOB_SIDES_TWO_SIDED_LONG_EDGE
    "two-sided-short-edge", // SMJOB_SIDES_TWO_SIDED_SHORT_EDGE
};
uint32_t g_smjob_sides_map_size = sizeof(g_smjob_sides_map)/sizeof(char *);

/* Strings in this list must have a one-to-one correspondence with the values defined by
 * the smjob_color_mode_t enum.
 * 
 * These strings must match the IPP keyword strings defined by the 'print-color-mode' job
 * template attribute: PWG 5100.13-2012 "IPP: Job and Printer Extensions -- Set 3 (JPS3)"
 * section 5.2.3: http://www.pwg.org/ipp/
 */
static char *g_smjob_color_mode_map[] =
{
    "", // SMJOB_COLOR_MODE_UNKNOWN
    "monochrome", // SMJOB_COLOR_MODE_MONO
    "color", // SMJOB_COLOR_MODE_COLOR
    "auto", // SMJOB_COLOR_MODE_AUTO
};
uint32_t g_smjob_color_mode_map_size = sizeof(g_smjob_color_mode_map)/sizeof(char *);

/* Strings in this list must have a one-to-one correspondence with the values defined by
 * the smjob_state_reason_t enum.
 * 
 * These strings must match the IPP keyword strings defined by the 'job-state-reasons'
 * attribute: http://tools.ietf.org/html/rfc2911#section-4.3.8
 */
static char *g_smjob_state_reason_map[] = 
{
    "", // SMJOB_FORMAT_UNKNOWN
    "none", // SMJOB_STATE_REASON_NONE
    "job-incoming", // SMJOB_STATE_REASON_JOB_INCOMING
    "job-data-insufficient", // SMJOB_STATE_REASON_JOB_DATA_INSUFFICIENT
    "document-access-error", // SMJOB_STATE_REASON_DOC_ACCESS_ERROR
    "submission-interrupted", // SMJOB_STATE_REASON_DOC_TRANSFER_ERROR
    "job-outgoing", // SMJOB_STATE_REASON_JOB_OUTGOING
    "job-hold-until-specified", // SMJOB_STATE_REASON_JOB_HOLD_UNTIL_SPECIFIED
    "resources-are-not-ready", // SMJOB_STATE_REASON_RESOURCES_ARE_NOT_READY
    "printer-stopped-partly", // SMJOB_STATE_REASON_PRINTER_STOPPED_PARTLY
    "printer-stopped", // SMJOB_STATE_REASON_PRINTER_STOPPED
    "job-interpreting", // SMJOB_STATE_REASON_JOB_INTERPRETING
    "job-queued", // SMJOB_STATE_REASON_JOB_QUEUED
    "job-transforming", // SMJOB_STATE_REASON_JOB_TRANSFORMING
    "job-queued-for-marker", // SMJOB_STATE_REASON_JOB_QUEUED_FOR_MARKER
    "job-printing", // SMJOB_STATE_REASON_JOB_PRINTING
    "job-canceled-by-user", // SMJOB_STATE_REASON_JOB_CANCELED_BY_USER
    "job-canceled-by-operator", // SMJOB_STATE_REASON_JOB_CANCELED_BY_OPERATOR
    "job-canceled-at-device", // SMJOB_STATE_REASON_JOB_CANCELED_AT_DEVICE
    "aborted-by-system", // SMJOB_STATE_REASON_ABORTED_BY_SYSTEM
    "unsupported-compression", // SMJOB_STATE_REASON_UNSUPPORTED_COMPRESSION
    "compression-error", // SMJOB_STATE_REASON_COMPRESSION_ERROR
    "unsupported-document-format", // SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT
    "document-format-error", // SMJOB_STATE_REASON_DOC_FORMAT_ERROR
    "processing-to-stop-point", // SMJOB_STATE_REASON_PROCESSING_TO_STOP_POINT
    "service-off-line", // SMJOB_STATE_REASON_SERVICE_OFF_LINE
    "job-completed-successfully", // SMJOB_STATE_REASON_JOB_COMPLETED_SUCCESSFULLY
    "job-completed-with-warnings", // SMJOB_STATE_REASON_JOB_COMPLETED_WITH_WARNINGS
    "job-completed-with-errors", // SMJOB_STATE_REASON_JOB_COMPLETED_WITH_ERRORS
    "job-restartable", // SMJOB_STATE_REASON_JOB_RESTARTABLE
    "queued-in-device", // SMJOB_STATE_REASON_QUEUE_IN_DEVICE
    "", // SMJOB_STATE_REASON_DOC_TIMEOUT_ERROR (WSD-specific)
    "", // SMJOB_STATE_REASON_JOB_PASSWORD_WAIT (WSD-specific)
};
uint32_t g_smjob_state_reason_map_size = sizeof(g_smjob_state_reason_map)/sizeof(char *);

typedef struct 
{
    mediasize_t size_enum;
    char        *name;
} sm_media_size_map_t;

//typedef struct
//{
//    mediasize_t size_enum;
//    char        *name;
//    uint32_t    width;
//    uint32_t    height;
//} sm_media_size_entry_t;

/* Standardized media size names
 * 
 * This list represents the superset of all PWG media sizes supported by the SDK.  A
 * device will typically support only a subset of the media sizes in this list.
 * 
 * The list is derived from SDK-defined media sizes that conform to the standardized PWG
 * namespace definition (PWG 5101.1-2002 "Standard for Media Standardized Names" section
 * 5: http://www.pwg.org/ipp/ ). The list excludes any SDK-defined media sizes which are
 * not also valid PWG sizes.
 */
// TODO add all the IPP media sizes and ask the engine if it is supported instead of assuming
// 
sm_media_size_map_t g_sm_media_size_map[] =
{
    { MEDIASIZE_LETTER,                     "na_letter_8.5x11in"                },
    { MEDIASIZE_LEGAL,                      "na_legal_8.5x14in"                 },
    { MEDIASIZE_EXECUTIVE,                  "na_executive_7.25x10.5in"          },
    { MEDIASIZE_CUSTOM_85x13,               "na_foolscap_8.5x13in"              },
    { MEDIASIZE_CUSTOM_4x6,                 "na_index-4x6_4x6in"                },
    { MEDIASIZE_CUSTOM_5x8,                 "na_index-5x8_5x8in"                },
    { MEDIASIZE_ENV_10,                     "na_number-10_4.125x9.5in"          },
    { MEDIASIZE_ENV_MONARCH,                "na_monarch_3.875x7.5in"            },
    { MEDIASIZE_A4,                         "iso_a4_210x297mm"                  },
    { MEDIASIZE_A5,                         "iso_a5_148x210mm"                  },
    { MEDIASIZE_ENV_C5,                     "iso_c5_162x229mm"                  },
    { MEDIASIZE_ENV_DL,                     "iso_dl_110x220mm"                  },
    { MEDIASIZE_ENV_B5,                     "iso_b5_176x250mm"                  },
    { MEDIASIZE_A6,                         "iso_a6_105x148mm"                  },
    { MEDIASIZE_B5,                         "jis_b5_182x257mm"                  },
    { MEDIASIZE_CUSTOM_16K,                 "roc_16k_7.75x10.75in"              },
    { MEDIASIZE_JAPANESE_POSTCARD,          "jpn_hagaki_100x148mm"              },
    { MEDIASIZE_DBL_JAPANESE_POSTCARD,      "jpn_oufuku_148x200mm"              },
    { MEDIASIZE_OFICIO_216x340,             "om_govt-legal_216x340mm"           }, // TODO confirm correct name
    { MEDIASIZE_CUSTOM_16K_184_260,         "om_16k_184x260mm"                  },
    { MEDIASIZE_CUSTOM_16K_195_270,         "om_16k_195x270mm"                  },
    { MEDIASIZE_CUSTOM_10x15cm,             "om_index-card_100x150mm"           },
    { MEDIASIZE_CUSTOM_DBL_POSTCARD_JIS,    "om_dbl-pc_148x200mm"               }, // TODO confirm correct name
    { MEDIASIZE_STATEMENT,                  "na_invoice_5.5x8.5in"              },
    { MEDIASIZE_FANFOLD_LGL_GERMAN,         "na_foolscap_8.5x13in"              },
    { MEDIASIZE_QUARTO,                     "na_quarto_8.5x10.83in"             },
};
#define SM_MEDIA_SIZE_MAP_CNT (sizeof(g_sm_media_size_map)/sizeof(sm_media_size_map_t))

#ifdef DEBUG    
mediasize_t g_sm_media_size_exclusions[] =
{
   MEDIASIZE_ANY,
   MEDIASIZE_USER_DEFINED,
   MEDIASIZE_CUSTOM_POSTCARD_JIS,
   MEDIASIZE_ENV_PERSONAL,
};
#define SM_MEDIA_SIZE_EXCLUSIONS_CNT (sizeof(g_sm_media_size_exclusions)/sizeof(mediasize_t))
#endif // DEBUG    



//typedef struct 
//{
//    mediatype_t type_enum;
//    char        *name;
//} sm_media_type_map_t;
//
//typedef sm_media_type_map_t *sm_media_type_entry_t;

/* Standardized media type names
 * 
 * This list represents the superset of all PWG media types supported by the SDK.  A
 * device will typically support only a subset of the media types in this list.
 * 
 * The list is derived from SDK-defined media types that conform to the standardized PWG
 * namespace definition (PWG 5101.1-2002 "Standard for Media Standardized Names" section
 * 3: http://www.pwg.org/ipp/ ). The list excludes any SDK-defined media types which are
 * not also valid PWG types.
 */
sm_media_type_map_t g_sm_media_type_map[] =
{
    {MEDIATYPE_TRANSPARENCY,         "transparency"             },
    {MEDIATYPE_LIGHTPAPER,           "stationery-lightweight"   },
    {MEDIATYPE_MIDWEIGHT_96_110G,    "stationery"               },
    {MEDIATYPE_PLAIN_PAPER,          "stationery"               },
    {MEDIATYPE_HEAVY1,               "stationery-heavyweight"   },
    {MEDIATYPE_EXTRA_HEAVY,          "cardstock"                },
    {MEDIATYPE_LABEL,                "labels"                   },
    {MEDIATYPE_ENVELOPE,             "envelope"                 },
    {MEDIATYPE_LETTERHEAD,           "stationery-letterhead"    }, 
    {MEDIATYPE_PREPRINTED,           "stationery-preprinted"    }, 
    {MEDIATYPE_PREPUNCHED,           "stationery-prepunched"    }, 
    {MEDIATYPE_MIDWEIGHT_GLOSSY,     "photographic-semi-gloss"  },
    {MEDIATYPE_HEAVY_GLOSSY,         "photographic-glossy"      },
    {MEDIATYPE_EXTRA_HEAVY_GLOSSY,   "photographic-high-gloss"  },
    {MEDIATYPE_GLOSSFILM,            "photographic-film"        },
};
#define SM_MEDIA_TYPE_MAP_CNT (sizeof(g_sm_media_type_map)/sizeof(sm_media_type_map_t))

#ifdef DEBUG    
mediatype_t g_sm_media_type_exclusions[] =
{
   MEDIATYPE_ANY,
   MEDIATYPE_PLAIN_PAPER,
   MEDIATYPE_BOND,
   MEDIATYPE_CARDSTOCK,
   MEDIATYPE_ROUGHPAPER,
   MEDIATYPE_THINPAPER,
   MEDIATYPE_GLOSSY1,
   MEDIATYPE_GLOSSY2,
   MEDIATYPE_GLOSSY3,
   MEDIATYPE_VELLUM,
   MEDIATYPE_PREMIUMCOVER,
   MEDIATYPE_PREMIUMPHOTO,
   MEDIATYPE_ENVELOPE_10,
   MEDIATYPE_ARCHIVE,
   MEDIATYPE_PLAIN75_89G,
   MEDIATYPE_CARD_GLOSSY,
   MEDIATYPE_HEAVY_ENVELOPE,
   MEDIATYPE_HEAVY_ROUGH,
   MEDIATYPE_PRESENTATION,
   MEDIATYPE_LIGHT_MATTE_COATED,
   MEDIATYPE_MEDIUM_MATTE_COATED,
   MEDIATYPE_HEAVY_MATTE_INKJET,
   MEDIATYPE_HEAVY_GLOSSY_INKJET,
   MEDIATYPE_RECYCLED_INKJET,
   MEDIATYPE_HP_ECOSMART_LITE,
   MEDIATYPE_HP_LASERJET_90,
   MEDIATYPE_HP_COLOR_LASER_105,
   MEDIATYPE_HP_PREMIUM_120,
   MEDIATYPE_HP_MATTE_160,
   MEDIATYPE_HP_MATTE_COVER_200,
   MEDIATYPE_HP_MATTE_PHOTO_200,
   MEDIATYPE_HP_GLOSSY_130,
   MEDIATYPE_HP_GLOSSY_160,
   MEDIATYPE_HP_TRIFOLD_GLOSSY,
   MEDIATYPE_HP_GLOSS_PHOTO_220,
   MEDIATYPE_HP_INTERMEDIATE,
   MEDIATYPE_HP_GLOSS_220,
   MEDIATYPE_HP_GLOSS_220_FAST,
   MEDIATYPE_COLORED,
   MEDIATYPE_RECYCLED,
   MEDIATYPE_UNKNOWN,
};
#define SM_MEDIA_TYPE_EXCLUSIONS_CNT (sizeof(g_sm_media_type_exclusions)/sizeof(mediatype_t))
#endif // DEBUG    



//=============================================================================
// Local prototypes
//=============================================================================

//static uint32_t sm_media_size_get_table(sm_media_size_entry_t **table);
//static uint32_t sm_media_type_get_table(sm_media_type_entry_t **table);
static void copy_doc_ticket(smjob_doc_ticket_t *orig_ticket, smjob_doc_ticket_t **new_ticket, smjob_type_t job_type);
static void copy_job_ticket(smjob_ticket_t *orig_ticket, smjob_ticket_t **new_ticket);


//=========================================================================================================================
// Parameterized macros
//=========================================================================================================================

pthread_mutex_t  g_smjob_mtx;

#define SMJOB_LOCK_CONTEXT() \
        { \
            UINT pthread_rcode = pthread_mutex_lock(&g_smjob_mtx); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
            UNUSED_VAR(pthread_rcode); \
        } 

#define SMJOB_UNLOCK_CONTEXT() \
        { \
            UINT pthread_rcode = pthread_mutex_unlock(&g_smjob_mtx); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
            UNUSED_VAR(pthread_rcode); \
        } 


//=============================================================================
// Main API function definitions
//=============================================================================

//-----------------------------------------------------------------------------
void smjob_init( void )
{
    // verify correct number of doc-format-to-mime-string mappings
    XASSERT(SMJOB_FORMAT_NUM_ENTRIES == g_smjob_format_map_size, g_smjob_format_map_size);

    UINT pthread_rcode = posix_mutex_init(&g_smjob_mtx);
    ASSERT(pthread_rcode == 0);
    UNUSED_VAR(pthread_rcode);

#ifdef DEBUG
    /* initialize the sm job helper/test commands */
    smjob_cmd_init();
#endif

    /* initialize the sm job print interface */
    smjob_pif_init( e_Print, "/smjob_pif" );

#ifdef HAVE_SCAN_SUPPORT
    smjob_sif_init();
#endif // HAVE_SCAN_SUPPORT

    /* initialize the sm job mgr */
    smjob_mgr_init( "smjob_mgr" );
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_default_ticket(smjob_type_t job_type, smjob_ticket_t *job_ticket)
{
    smjob_rcode_t                sm_res = SMJOB_OK;
    smjob_rcode_t                rcode;
    smjob_print_doc_processing_t *print_proc;
    smjob_scan_doc_processing_t  *scan_proc;
    char                         *compress_str;
    char                         *format_str;


    // Check inputs
    if ( ( 0 == job_ticket ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_ERR("%s: Invalid job_type or job_ticket pointer provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Start with an initialized job ticket
    memset(job_ticket, 0, sizeof(smjob_ticket_t));

    // Take care of the easy items first
    job_ticket->job_type = job_type;
    SMJOB_ATTRIBUTE_MASK_INIT(job_ticket->must_honor);
    SMJOB_ATTRIBUTE_MASK_INIT(job_ticket->omitted);

    switch ( job_type )
    {
        case SMJOB_TYPE_PRINT:
            print_proc = &job_ticket->doc_processing.type.print;

            job_ticket->job_processing.priority = 1;

            // Fetch the default doc processing settings (that way the oem layer has a vote)
            rcode = smjob_get_default_orientation(SMJOB_TYPE_PRINT, 
                                                  &job_ticket->doc_processing.orientation);
            ASSERT(rcode == SMJOB_OK);

            rcode = smjob_get_default_resolution(SMJOB_TYPE_PRINT, 
                                                 &job_ticket->doc_processing.resolution);
            ASSERT(rcode == SMJOB_OK);

            rcode = smjob_get_default_copies(&print_proc->copies);
            ASSERT(rcode == SMJOB_OK);

            smjob_sides_t *sides;
            rcode = smjob_get_default_sides(SMJOB_TYPE_PRINT, &sides);
            ASSERT(rcode == SMJOB_OK);
            ASSERT(strlen(sides) < SIDES_STR_LEN);
            strncpy(&print_proc->sides[0], sides, strlen(sides));
            

            rcode = smjob_get_default_quality(&print_proc->quality);
            ASSERT(rcode == SMJOB_OK);

            smjob_color_mode_t *color_mode;
            rcode = smjob_get_default_color_mode(SMJOB_TYPE_PRINT, &color_mode);
            ASSERT(rcode == SMJOB_OK);
            ASSERT(strlen(color_mode) < COLOR_MODE_STR_LEN);
            strncpy(print_proc->color_mode, color_mode, strlen(color_mode));

            rcode = smjob_get_default_media_col(SMJOB_TYPE_PRINT, &print_proc->media_col);
            ASSERT(rcode == SMJOB_OK);

            print_proc->sheet_collate = FALSE;
            print_proc->media_size    = MEDIASIZE_LETTER;
            print_proc->media_type    = MEDIATYPE_PLAIN_PAPER;

            smjob_print_content_optimize_t *print_content_optimize;
            rcode = smjob_get_default_print_content_optimize(SMJOB_TYPE_PRINT, 
                                                             &print_content_optimize);
            ASSERT(rcode == SMJOB_OK);
            ASSERT(strlen(print_content_optimize) < PRINT_CONTENT_OPTIMIZE_STR_LEN);
            strncpy(print_proc->print_content_optimize,
                    print_content_optimize,
                    strlen(print_content_optimize));

            break;

        case SMJOB_TYPE_SCAN:
            scan_proc = &job_ticket->doc_processing.type.scan;

            job_ticket->job_processing.priority = 1;
            job_ticket->doc_processing.orientation = SMJOB_ORIENTATION_PORTRAIT;
            job_ticket->doc_processing.resolution.xfeed_dir = 300;
            job_ticket->doc_processing.resolution.feed_dir = 300;
            job_ticket->doc_processing.resolution.units = SMJOB_RES_UNIT_DOTS_PER_INCH;
            scan_proc->input_source = SMJOB_INPUT_SOURCE_PLATEN;

            compress_str = SMJOB_COMPRESSION_NONE;
            format_str   = SMJOB_FORMAT_APPLICATION_OCTET_STREAM;

            ASSERT(strlen(compress_str) < COMPRESSION_STR_LEN);
            ASSERT(strlen(format_str) < FORMAT_STR_LEN);

            strcpy(scan_proc->compression, compress_str);
            strcpy(scan_proc->format, SMJOB_FORMAT_APPLICATION_OCTET_STREAM);

            break;

        case SMJOB_TYPE_FAXIN: // Similar to print
            print_proc = &job_ticket->doc_processing.type.print;

            job_ticket->job_processing.priority = 1;
            job_ticket->doc_processing.orientation = SMJOB_ORIENTATION_PORTRAIT;
            job_ticket->doc_processing.resolution.xfeed_dir = 300;
            job_ticket->doc_processing.resolution.feed_dir = 300;
            job_ticket->doc_processing.resolution.units = SMJOB_RES_UNIT_DOTS_PER_INCH;
            print_proc->copies = 1;
            print_proc->media_size = MEDIASIZE_LETTER;
            print_proc->media_type = MEDIATYPE_PLAIN_PAPER;
            print_proc->sheet_collate = FALSE;

            strncpy(print_proc->sides, SMJOB_SIDES_ONE_SIDED, strlen(SMJOB_SIDES_ONE_SIDED));

            print_proc->quality = SMJOB_QUALITY_NORMAL;
            break;

        case SMJOB_TYPE_FAXOUT:  // Similar to scan
            print_proc = &job_ticket->doc_processing.type.print;

            job_ticket->job_processing.priority = 1;
            job_ticket->doc_processing.orientation = SMJOB_ORIENTATION_PORTRAIT;
            job_ticket->doc_processing.resolution.xfeed_dir = 300;
            job_ticket->doc_processing.resolution.feed_dir = 300;
            job_ticket->doc_processing.resolution.units = SMJOB_RES_UNIT_DOTS_PER_INCH;
            print_proc->copies = 1;
            print_proc->media_size = MEDIASIZE_LETTER;
            print_proc->media_type = MEDIATYPE_PLAIN_PAPER;
            print_proc->sheet_collate = FALSE;

            strncpy(print_proc->sides, SMJOB_SIDES_ONE_SIDED, strlen(SMJOB_SIDES_ONE_SIDED));

            print_proc->quality = SMJOB_QUALITY_NORMAL;
            break;

        default:
            ASSERT( SMJOB_TYPE_UNSPECIFIED == job_type );
            break;
    }

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_default_doc_ticket(uint32_t job_id, smjob_doc_ticket_t *doc_ticket)
{
    smjob_rcode_t                 sm_res;
    smjob_t                       *sm_job;
    smjob_type_t                  job_type;
    smjob_print_doc_description_t *print_desc;
    smjob_print_doc_processing_t  *print_proc;
    smjob_scan_doc_processing_t   *scan_proc;
    char                          *compress_str;
    char                          *format_str;


    // Check inputs
    if ( ( !job_id ) || ( 0 == doc_ticket ) )
    {
        DBG_ERR("%s: Invalid job_id or doc_ticket pointer provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }
    
    // Start with an initialized doc ticket
    memset(doc_ticket, 0, sizeof(smjob_doc_ticket_t));

    // Find the sm_job that correlates with the job_id
    sm_res = smjob_mgr_job_lookup_stack(job_id, &sm_job);
    if ( SMJOB_OK != sm_res )
    {
        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, job_id);
        return sm_res;
    }

    job_type = sm_job->job_ticket->job_type;
    smjob_doc_processing_t *doc_processing_dflt = &sm_job->job_ticket->doc_processing;

    // Intialize the document attributes
    SMJOB_ATTRIBUTE_MASK_INIT(doc_ticket->must_honor);
    SMJOB_ATTRIBUTE_MASK_INIT(doc_ticket->omitted);

    // The default doc_description and doc_processing structs
    // are dependent on the type of job
    switch ( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {

            print_desc = &doc_ticket->doc_description.type.print;
            print_proc = &doc_ticket->doc_processing.type.print;
            
            compress_str = SMJOB_COMPRESSION_NONE;
            format_str   = SMJOB_FORMAT_APPLICATION_OCTET_STREAM;
            
            ASSERT(strlen(compress_str) < COMPRESSION_STR_LEN);
            ASSERT(strlen(format_str) < FORMAT_STR_LEN);

            strcpy(print_desc->compression, compress_str);
            strcpy(print_desc->format, SMJOB_FORMAT_APPLICATION_OCTET_STREAM);

            doc_ticket->doc_processing.orientation = doc_processing_dflt->orientation;
            doc_ticket->doc_processing.resolution.xfeed_dir = doc_processing_dflt->resolution.xfeed_dir;
            doc_ticket->doc_processing.resolution.feed_dir = doc_processing_dflt->resolution.feed_dir;
            doc_ticket->doc_processing.resolution.units = doc_processing_dflt->resolution.units;
            print_proc->copies = 1;
            print_proc->media_size = MEDIASIZE_LETTER;
            print_proc->media_type = MEDIATYPE_PLAIN_PAPER;
            print_proc->sheet_collate = FALSE;

            strncpy(print_proc->sides, SMJOB_SIDES_ONE_SIDED, strlen(SMJOB_SIDES_ONE_SIDED));

            print_proc->quality = SMJOB_QUALITY_NORMAL;
            break;
        }

        case SMJOB_TYPE_SCAN:
        {
            scan_proc = &doc_ticket->doc_processing.type.scan;

            doc_ticket->doc_processing.orientation = SMJOB_ORIENTATION_PORTRAIT;
            doc_ticket->doc_processing.resolution.xfeed_dir = 150;
            doc_ticket->doc_processing.resolution.feed_dir = 150;
            doc_ticket->doc_processing.resolution.units = SMJOB_RES_UNIT_DOTS_PER_INCH;
            scan_proc->input_source = SMJOB_INPUT_SOURCE_PLATEN;

            compress_str = SMJOB_COMPRESSION_NONE;
            format_str   = SMJOB_FORMAT_APPLICATION_OCTET_STREAM;

            ASSERT(strlen(compress_str) < COMPRESSION_STR_LEN);
            ASSERT(strlen(format_str) < FORMAT_STR_LEN);

            strcpy(scan_proc->compression, compress_str);
            strcpy(scan_proc->format, SMJOB_FORMAT_APPLICATION_OCTET_STREAM);
            break;
        }

        case SMJOB_TYPE_FAXIN: // Similar to print
        {

            print_desc = &doc_ticket->doc_description.type.print;
            print_proc = &doc_ticket->doc_processing.type.print;

            compress_str = SMJOB_COMPRESSION_NONE;
            format_str   = SMJOB_FORMAT_APPLICATION_OCTET_STREAM;

            ASSERT(strlen(compress_str) < COMPRESSION_STR_LEN);
            ASSERT(strlen(format_str) < FORMAT_STR_LEN);

            strcpy(print_desc->compression, compress_str);
            strcpy(print_desc->format, SMJOB_FORMAT_APPLICATION_OCTET_STREAM);

            doc_ticket->doc_processing.orientation = SMJOB_ORIENTATION_PORTRAIT;
            doc_ticket->doc_processing.resolution.xfeed_dir = 300;
            doc_ticket->doc_processing.resolution.feed_dir = 300;
            doc_ticket->doc_processing.resolution.units = SMJOB_RES_UNIT_DOTS_PER_INCH;
            print_proc->copies = 1;
            print_proc->media_size = MEDIASIZE_LETTER;
            print_proc->media_type = MEDIATYPE_PLAIN_PAPER;
            print_proc->sheet_collate = FALSE;

            strncpy(print_proc->sides, SMJOB_SIDES_ONE_SIDED, strlen(SMJOB_SIDES_ONE_SIDED));

            print_proc->quality = SMJOB_QUALITY_NORMAL;
            break;
        }

        case SMJOB_TYPE_FAXOUT:  // Similar to scan
        {
            scan_proc = &doc_ticket->doc_processing.type.scan;

            doc_ticket->doc_processing.orientation = SMJOB_ORIENTATION_PORTRAIT;
            doc_ticket->doc_processing.resolution.xfeed_dir = 150;
            doc_ticket->doc_processing.resolution.feed_dir = 150;
            doc_ticket->doc_processing.resolution.units = SMJOB_RES_UNIT_DOTS_PER_INCH;
            scan_proc->input_source = SMJOB_INPUT_SOURCE_PLATEN;

            compress_str = SMJOB_COMPRESSION_NONE;
            format_str   = SMJOB_FORMAT_APPLICATION_OCTET_STREAM;

            ASSERT(strlen(compress_str) < COMPRESSION_STR_LEN);
            ASSERT(strlen(format_str) < FORMAT_STR_LEN);

            strcpy(scan_proc->compression, compress_str);
            strcpy(scan_proc->format, SMJOB_FORMAT_APPLICATION_OCTET_STREAM);

            break;
        }

        default:
            ASSERT( SMJOB_TYPE_UNSPECIFIED == job_type );
            break;
    }

    return sm_res;
}

//-----------------------------------------------------------------------------
uint32_t smjob_create(const smjob_ticket_t *job_ticket, smjob_status_func_t status_func, 
                       smjob_status_events_t job_event_mask, void *user_data)
{
    smjob_rcode_t sm_res;
    smjob_t *sm_job;
    smjob_ctxt_t *job = NULL; // for scan
    smjob_ticket_t *orig_job_ticket;
    smjob_ticket_t *copied_job_ticket;
    smjob_status_t *job_status;
    uint32_t job_id = SMJOB_INVALID_JOB_ID;

    // Check inputs
    if ( 0 == job_ticket )
    {
        DBG_ERR("%s: Invalid job_ticket\n", __func__);
        goto done;
    }

    switch(job_ticket->job_type)
    {
        case SMJOB_TYPE_SCAN:
        {

#ifdef HAVE_SCAN_SUPPORT

            UINT i;
            UINT pthread_rcode;

            job = (smjob_ctxt_t *)MEM_MALLOC(sizeof(smjob_ctxt_t));
            if(!job)
            {
                DBG_ERR("%s: Failed to allocate job context\n", __func__);
                goto done;
            }
            job->ticket = *job_ticket;
            job->status_func = status_func;
            job->event_mask = job_event_mask;
            job->user_data = user_data;
            job->data_done = false;
            job->data_abort = false;
            job->data_queue = NULL;
            job->data_queue_tail = &job->data_queue;
            job->data_queue_emptied = true;

            job->state = SMJOB_STATE_PENDING;
            for (i=0; i<SMJOB_MAX_STATE_REASONS; i++)
            {
                job->reasons[i] = SMJOB_STATE_REASON_UNKNOWN;
            }
            job->num_reasons = 0;

            // TODO TEMP
            job->is_color = true;

            // tx_event_flags_delete() is called when job completes
//            pthread_rcode = tx_event_flags_create(&job->flgs, "sm_scan_job");
            pthread_rcode = sem_init(&job->semaphore, 0, 1);
            if(pthread_rcode != 0)
            {
                DBG_ERR("error creating tx flags (pthread_rcode=0x%02x)\n", pthread_rcode);
                goto done; // TODO cleanup
            }

            sm_res = smjob_mgr_insert_job_in_stack(NULL, job);
            if(SMJOB_OK != sm_res)
            {
                DBG_ERR("%s:  Failure to insert scan job into job stack \n", __func__);
                goto done; // TODO cleanup
            }
            job_id = job->job_id;

            sm_res = smjob_sif_start_scan(job);
            if(SMJOB_OK != sm_res)
            {
                DBG_ERR("%s: start_scan operation failed\n", __func__);
                // rdj TODO remove from job stack
                goto done; // TODO cleanup
            }

#endif // HAVE_SCAN_SUPPORT

            break;
        }

        case SMJOB_TYPE_PRINT:
        {
            // Create the components of the job handle and the handle itself
            job_status = (smjob_status_t *)MEM_MALLOC(sizeof(smjob_status_t));
            sm_job = (smjob_t *)MEM_MALLOC(sizeof(struct smjob_s));
            if(!job_status || !sm_job)
            {
                DBG_ERR("%s:  Failure to allocate memory for smjob or its components \n",
                         __func__);
                MEM_FREE_IF_NOT_NULL( job_status );
                MEM_FREE_IF_NOT_NULL( sm_job );
                job_id = SMJOB_INVALID_JOB_ID;
                goto done;
            }

            // Initialize the new job
            memset( job_status, 0, sizeof(smjob_status_t) );
            memset( sm_job, 0, sizeof(smjob_t) );

            // Copy the job ticket so that it is persistent
            orig_job_ticket = (smjob_ticket_t *)job_ticket;
            copy_job_ticket(orig_job_ticket, &copied_job_ticket);

            // Build up the job
            sm_job->job_ticket = copied_job_ticket;
            sm_job->job_status_func = status_func; 
            sm_job->job_status = job_status;
            sm_job->job_event_mask = job_event_mask;
            sm_job->sm_doc = NULL;
            sm_job->eof = false;
            sm_job->errors = false;
            sm_job->cancelled = false;

        #ifndef NET_SM_STREAMING
            sm_job->bytes_read = 0;
        #endif
            sm_job->user_data = user_data;

            // Add the job to the job stack
            // This will also assign the job id
            sm_res = smjob_mgr_insert_job_in_stack( sm_job, NULL );
            if ( SMJOB_OK != sm_res )
            {
                DBG_ERR("%s:  Failure to insert job into job stack \n", __func__);
                MEM_FREE_AND_NULL( copied_job_ticket );
                MEM_FREE_AND_NULL( job_status );
                MEM_FREE_AND_NULL( sm_job );
                goto done;
            }

            // Add the job to the job queue 
            sm_res = smjob_mgr_insert_job_in_list( sm_job );
            if ( SMJOB_OK != sm_res )
            {
                DBG_ERR("%s:  Failure to insert job into job queue \n", __func__);
                MEM_FREE_AND_NULL( copied_job_ticket );
                MEM_FREE_AND_NULL( job_status );
                MEM_FREE_AND_NULL( sm_job );
                goto done;
            }

            job_id = sm_job->job_status->job_id;
            break;
        }

        default:
            // unsupported job type
            DBG_ERR("%s: Unsupported job type %d\n", __func__, job_ticket->job_type);
            job_id = SMJOB_INVALID_JOB_ID;
            goto done;
            break;
    }

done:
    if(job_id == SMJOB_INVALID_JOB_ID)
    {
        // cleanup
        MEM_FREE_IF_NOT_NULL( job );
    }
    return job_id;
}

//-----------------------------------------------------------------------------
uint32_t smjob_add_document(uint32_t job_id, smjob_doc_ticket_t *doc_ticket, bool last_doc)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_doc_status_t *doc_status;
    smjob_doc_ticket_t *copied_doc_ticket;
    smjob_doc_t *sm_doc;
    smjob_t *sm_job;
    uint32_t doc_id; 

    // Check inputs
    if ( ( !job_id ) || ( 0 == doc_ticket ) )
    {
        DBG_ERR("%s: Invalid job_id or doc_ticket pointer provided \n", __func__);
        doc_id = SMJOB_INVALID_DOC_ID;
        return doc_id;
    }

    // Create the components of the doc handle and the handle itself
    doc_status = (smjob_doc_status_t *)MEM_MALLOC(sizeof(smjob_doc_status_t));
    sm_doc = (smjob_doc_t *)MEM_MALLOC(sizeof(smjob_doc_t)); 
    if ( !doc_status || !sm_doc )
    {
        DBG_ERR("%s:  Failure to allocate memory for doc_handle \n", __func__);
        MEM_FREE_IF_NOT_NULL( doc_status );
        MEM_FREE_IF_NOT_NULL( sm_doc );
        doc_id = SMJOB_INVALID_DOC_ID;
    }
    else
    {
        // Initialize the new doc handle
        memset( doc_status, 0, sizeof(smjob_doc_status_t) );
        memset( sm_doc, 0, sizeof(smjob_doc_t) );

        // Find the sm_job that correlates with the job_id
        sm_res = smjob_mgr_job_lookup_stack(job_id, &sm_job);
        XASSERT(sm_res == SMJOB_OK, sm_res);
    
        // Copy the doc ticket so that it is persistent
        copy_doc_ticket(doc_ticket, &copied_doc_ticket, sm_job->job_ticket->job_type);
    
        // Set up the new doc handle
        sm_doc->next = NULL;
        sm_doc->sm_job = sm_job;
        sm_doc->doc_status = doc_status;
        sm_doc->doc_ticket = copied_doc_ticket;

        // Add it to the job and the job queue
        if ( SMJOB_OK == sm_res )
        {
            // This will also add the doc_id to sm_doc->doc_status
            sm_res = smjob_mgr_add_document( sm_job, last_doc, sm_doc );
            doc_id = sm_doc->doc_status->doc_id;
        }

        if ( SMJOB_OK != sm_res )
        {
            MEM_FREE_AND_NULL( doc_status );
            MEM_FREE_AND_NULL( copied_doc_ticket );
            MEM_FREE_AND_NULL( sm_doc );
            doc_id = SMJOB_INVALID_DOC_ID;
        }
    }

    return doc_id;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_status(uint32_t job_id, smjob_status_t *job_status)
{
    smjob_rcode_t sm_res;
    smjob_t *sm_job = NULL;
    smjob_ctxt_t *job = NULL;
    int8_t i;

    DBG_VERBOSE("==>%s (job %d)\n", __func__, job_id); 

    // Check inputs
    if ( ( !job_id ) || ( 0 == job_status ) )
    {
        DBG_ERR("%s: Invalid job_id or job_status pointer provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Find the sm_job that correlates with the job_id
    sm_res = smjob_mgr_job_lookup_stack_ctxt(job_id, &sm_job, &job);
    if ( SMJOB_OK != sm_res )
    {
        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, job_id);
        return sm_res;
    }

    XASSERT(!sm_job || !job, (unsigned int)sm_job);
    
   smjob_lock_ctxt();
    if(sm_job)
    {
        // Copy the job status from sm_job into job_status
        job_status->job_id = sm_job->job_status->job_id;
        job_status->num_docs = sm_job->job_status->num_docs;
        job_status->state = sm_job->job_status->state;
        job_status->num_reasons = sm_job->job_status->num_reasons;
        for ( i = 0; i < SMJOB_MAX_STATE_REASONS; i++ )
        {
            job_status->reasons[i] = sm_job->job_status->reasons[i]; 
        }
        job_status->koctets_processed = sm_job->job_status->koctets_processed;
        job_status->sheets_completed = sm_job->job_status->sheets_completed;
        job_status->vnd_extensions = sm_job->job_status->vnd_extensions;
        
        DBG_VERBOSE("%s: job %d: state=%d num_reasons=%d\n", __func__, job_id, 
                    job_status->state, job_status->num_reasons);
        for (i=0; i<job_status->num_reasons; i++)
        {
            DBG_VERBOSE("%s:    reason[%d]=%d\n", __func__, i, job_status->reasons[i]);
        }
    }
    else
    {
        ASSERT(job);

        DBG_MSG("%s: No sm_job for job_id = %ld \n", __func__, job_id);
        
        job_status->job_id = job->job_id;
        job_status->state = job->state;
        job_status->num_reasons = job->num_reasons;
        XASSERT(job->num_reasons <= SMJOB_MAX_STATE_REASONS, job->num_reasons); 
        for ( i = 0; i < job->num_reasons; i++ )
        {
            job_status->reasons[i] = job->reasons[i]; 
        }

        // TODO not currently implemented
        job_status->num_docs = 0;
        job_status->koctets_processed = 0;
        job_status->sheets_completed = 0;
        job_status->vnd_extensions = NULL;

    }
    smjob_unlock_ctxt();

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_retrieve_image(uint32_t job_id, uint8_t **buf, uint32_t *len, uint32_t timeout_ms)
{
    smjob_rcode_t smjob_rcode = SMJOB_OK;
    smjob_ctxt_t *job;
    
    ASSERT(job_id);
    ASSERT(buf);
    ASSERT(len);

    // default to error
    *buf = NULL;
    *len = -1; 

    // Find the sm_job that correlates with the job_id
    smjob_rcode = smjob_mgr_job_lookup_stack_ctxt(job_id, NULL, &job);
    if( smjob_rcode != SMJOB_OK )
    {
        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, job_id);
        return SMJOB_FAIL;
    }
    ASSERT(job);

    if(job->ticket.job_type != SMJOB_TYPE_SCAN)
    {
        DBG_ERR("%s: invalid job type for job_id = %ld \n", __func__, job_id);
        return SMJOB_FAIL;
    }

#ifdef HAVE_SCAN_SUPPORT

    UINT pthread_rcode;
//    uint32_t wait_option;
//    uint32_t actual_flags;
    struct timespec abs_timeout;

    smjob_lock_ctxt();

    if(job->data_queue_emptied)
    {
        if(job->data_abort)
        {
            DBG_MSG("%s: got ABORT\n", __func__);
            ASSERT(job->data_done);
            *len = -1;
            smjob_rcode = SMJOB_IO_ABORTED;
            smjob_unlock_ctxt();
            goto done;
        }

        if(job->data_done)
        {
            // data queue is empty and no more job data is to be added for this job, so return final EOF
            DBG_MSG("%s: got EOF (final)\n", __func__);
            *len = 0;
            smjob_unlock_ctxt();
            goto done;
        }

        if( timeout_ms == 0 )
        {
            // timeout of 0 and no data available -- return immediately
            *len = -1;
            smjob_rcode = SMJOB_IO_TIMEOUT;
            smjob_unlock_ctxt();
            goto done;
        }

        // timeout >0 and no data available -- block and wait for data 

        smjob_unlock_ctxt();

        if(timeout_ms == -1)
        {
//            wait_option = 0xFFFFFFFF;
            // Assuming this means to block
            pthread_rcode = sem_wait(&job->semaphore);
        }
        else
        {
//            wait_option = ((SYS_TICK_FREQ * timeout_ms)/1000);
            
            // convert from relative OS tick timeout to absolute time.
            clock_gettime( CLOCK_REALTIME, &abs_timeout );
            posix_calc_future_usec( &abs_timeout, timeout_ms * 10 );

            pthread_rcode = sem_timedwait(&job->semaphore, &abs_timeout);
        }
//        threadx_rcode = tx_event_flags_get(&job->flgs, SMJOB_TX_FLAG_SCAN_DATA_AVAIL, TX_OR,
//                                           &actual_flags, wait_option);

//        if(pthread_rcode == TX_NO_EVENTS)
        if(pthread_rcode == ETIMEDOUT)
        {
//            if(job->data_done)
//            {
//                DBG_MSG("%s: data_done TIMEOUT!!!!!!)\n", __func__);
//            }

            // timeout
            smjob_rcode = SMJOB_IO_TIMEOUT;
            goto done;
        }

        smjob_lock_ctxt();

//        if(job->data_done)
//        {
//            DBG_MSG("%s: data_done unblocked (data_queue_emptied=%d data_abort=%d)\n", __func__,
//                     job->data_queue_emptied, job->data_abort);
//        }

        // check if we unblocked due to ABORT or EOF
        if(job->data_queue_emptied)
        {
            if(job->data_abort)
            {
                DBG_MSG("%s: got ABORT (while blocked on read)\n", __func__);
                ASSERT(job->data_done);
                *len = -1;
                smjob_rcode = SMJOB_IO_ABORTED;
                smjob_unlock_ctxt();
                goto done;
            }
            if(job->data_done)
            {
                // data is no longer available, e.g. due to job cancel -- return EOF
                DBG_MSG("%s: got EOF (while blocked on read)\n", __func__);
                *len = 0;
                smjob_unlock_ctxt();
                goto done;
            }
        }
    }

    // if we get here we should have data
    ASSERT(!job->data_queue_emptied);
    ASSERT(job->data_queue);

    smjob_sif_buf_node_t *node = job->data_queue;
    job->data_queue = job->data_queue->next;
    if(!job->data_queue)
    {
        // queue is now empty
        job->data_queue_tail = &job->data_queue;
        job->data_queue_emptied = true;

        // clear flag
        pthread_rcode = sem_post(&job->semaphore);
        XASSERT(pthread_rcode == 0, pthread_rcode);
        UNUSED_VAR(pthread_rcode);
    }

    smjob_unlock_ctxt();

    if(node->buf)
    {
        *buf = node->buf;
        *len = node->len;
    }
    else
    {
        // null buf signals EOF for current page
        // NOTE: This does NOT imply end of job. For multi-page jobs, a successive call to 
        // smjob_retrieve_image() will return data for the next page. Two back-to-back EOFs
        // signal end of job.
        XASSERT(!node->len, node->len);
        DBG_MSG("%s: got EOF (end of page)\n", __func__);
        *len = 0;
    }

//    // TODO TEMP DEBUG
//    if(job->data_done)
//    {
//        DBG_MSG("%s: data_done len=%d)\n", __func__, *len);
//    }

    memFree(node);

#else // !HAVE_SCAN_SUPPORT
    smjob_rcode = SMJOB_FAIL;
    goto done;
#endif // HAVE_SCAN_SUPPORT

done:
//    dbg_printf("##### retrieve_image len=%d ret=%d\n", *len, smjob_rcode);
    return smjob_rcode;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_cancel(uint32_t job_id)
{
    smjob_rcode_t sm_res;
    smjob_t *sm_job;
    smjob_ctxt_t *job;

    // Check inputs
    if ( !job_id )
    {
        DBG_ERR("%s: Invalid job_id provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Find the sm_job that correlates with the job_id
    sm_res = smjob_mgr_job_lookup_stack_ctxt(job_id, &sm_job, &job);
    if ( SMJOB_OK != sm_res )
    {
        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, job_id);
        return sm_res;
    }

    XASSERT(!sm_job || !job, (unsigned int)sm_job);
    if(sm_job)
    {
        sm_res = smjob_mgr_cancel_job( job_id );
    }
    else
    {
#ifdef HAVE_SCAN_SUPPORT
        ASSERT(job);
        smjob_sif_cancel_scan(job);
#else // !HAVE_SCAN_SUPPORT
        sm_res = SMJOB_FAIL;
        return sm_res;
#endif // HAVE_SCAN_SUPPORT
    }

    return sm_res;
}

//-----------------------------------------------------------------------------
uint32_t smjob_get_next_document(uint32_t job_id, uint32_t doc_id)
{
    smjob_rcode_t sm_res;
    smjob_t *sm_job;
    smjob_doc_t *sm_doc = NULL;
    uint32_t next_doc_id;
    bool found;

    // Check inputs
    if ( ( !job_id ) | ( !doc_id ) )
    {
        DBG_ERR("%s: Invalid job_id or doc_id provided \n", __func__);
        return SMJOB_INVALID_DOC_ID;
    }

    // Find the sm_job that correlates with the job_id
    sm_res = smjob_mgr_job_lookup_stack(job_id, &sm_job);
    if ( SMJOB_OK != sm_res )
    {
        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, job_id);
        return SMJOB_INVALID_DOC_ID;
    }

    // No docs were added to the job yet
    sm_doc = sm_job->sm_doc;
    if ( NULL == sm_doc )
    {
        DBG_ERR("%s: No docs added to the job yet = %ld \n", __func__, job_id);
        return SMJOB_INVALID_DOC_ID;
    }

    // Find the doc after the one requested
    if ( SMJOB_INVALID_DOC_ID == doc_id )
    {
        // The first doc in the job was requested
        next_doc_id = sm_doc->doc_status->doc_id;
        return next_doc_id;
    }
    else
    {
        // Otherwise keep looking
        found = FALSE;
        while ( ( !found ) && ( NULL != sm_doc ) )
        {
            if ( doc_id == sm_doc->doc_status->doc_id )
            {
                found = TRUE;
            }
            else
            {
                sm_doc = sm_doc->next;
            }
        }
    }

    // Check if found the initial doc id requested;
    // if so, return the doc_id of the next document
    if ( ( TRUE == found ) && ( NULL != sm_doc->next ) )
    {
        next_doc_id = sm_doc->next->doc_status->doc_id;
    }
    else
    {
        // Either we didn't find the doc requested, 
        // or we did find it but there was no next document after it
        DBG_ERR("%s: Either no matching doc_id found, or it was found at end of list = %ld \n", __func__, job_id);
        next_doc_id = SMJOB_INVALID_DOC_ID;
    }

    // All done
    return next_doc_id;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_document_status(uint32_t doc_id, smjob_doc_status_t *doc_status)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_doc_t *sm_doc;

    // Check inputs
    if ( ( !doc_id ) | ( 0 == doc_status ) )
    {
        DBG_ERR("%s: Invalid doc_id or doc_status pointer provided \n", __func__);
        return SMJOB_FAIL;
    }

    // Find the doc_handle that correlates with the doc_id
    sm_res = smjob_mgr_doc_lookup_stack(doc_id, &sm_doc);
    if ( SMJOB_OK != sm_res )
    {
        DBG_ERR("%s: Failure getting doc_handle for doc_id = %ld \n", __func__, doc_id);
        return SMJOB_FAIL;
    }

    // Copy over the document status
    doc_status->doc_id = sm_doc->doc_status->doc_id;

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_document_ticket(uint32_t doc_id, smjob_doc_ticket_t *doc_ticket)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_doc_t *sm_doc;

    // Check inputs
    if ( ( !doc_id ) | ( 0 == doc_ticket ) )
    {
        DBG_ERR("%s: Invalid doc_id or doc_status pointer provided \n", __func__);
        return SMJOB_FAIL;
    }

    // Find the doc_handle that correlates with the doc_id
    sm_res = smjob_mgr_doc_lookup_stack(doc_id, &sm_doc);
    if ( SMJOB_OK != sm_res )
    {
        DBG_ERR("%s: Failure getting doc_handle for doc_id = %ld \n", __func__, doc_id);
        return SMJOB_FAIL;
    }

    // Copy over the document ticket information
    strncpy( doc_ticket->doc_description.doc_name, sm_doc->doc_ticket->doc_description.doc_name, SMJOB_MAX_NAME_LEN );
    doc_ticket->doc_description.doc_name[SMJOB_MAX_NAME_LEN-1] = '\0';
    doc_ticket->doc_description.type = sm_doc->doc_ticket->doc_description.type;
    doc_ticket->doc_description.vnd_extensions = sm_doc->doc_ticket->doc_description.vnd_extensions;
    doc_ticket->doc_processing.orientation = sm_doc->doc_ticket->doc_processing.orientation;
    doc_ticket->doc_processing.resolution = sm_doc->doc_ticket->doc_processing.resolution;
    doc_ticket->doc_processing.type = sm_doc->doc_ticket->doc_processing.type;
    doc_ticket->doc_processing.vnd_extensions = sm_doc->doc_ticket->doc_processing.vnd_extensions;
    doc_ticket->must_honor = sm_doc->doc_ticket->must_honor;
    doc_ticket->omitted = sm_doc->doc_ticket->omitted;
    doc_ticket->vnd_extensions = sm_doc->doc_ticket->vnd_extensions;

    ASSERT(sm_doc->sm_job);
    switch(sm_doc->sm_job->job_ticket->job_type)
    {
        case SMJOB_TYPE_PRINT:
        {
            doc_ticket->doc_description.type.print.recv_func = sm_doc->doc_ticket->doc_description.type.print.recv_func;
            doc_ticket->doc_description.type.print.user_data = sm_doc->doc_ticket->doc_description.type.print.user_data;
            break;
        }
        case SMJOB_TYPE_SCAN:
        {
            doc_ticket->doc_description.type.scan.send_func = sm_doc->doc_ticket->doc_description.type.scan.send_func;
            doc_ticket->doc_description.type.scan.user_data = sm_doc->doc_ticket->doc_description.type.scan.user_data;
            break;
        }
        default:
        {
            // invalid or not-yet-supported job type
            XASSERT(0, sm_doc->sm_job->job_ticket->job_type);
            break;
        }
    }

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_ticket(uint32_t job_id, smjob_ticket_t *job_ticket)
{
    smjob_rcode_t sm_res = SMJOB_OK;
    smjob_t *sm_job = NULL; // TODO old context - remove
    smjob_ctxt_t *job = NULL;

    // Check inputs
    if ( ( !job_id ) || ( 0 == job_ticket ) )
    {
        DBG_ERR("%s: Invalid job_id or job_ticket pointer provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Find the sm_job that correlates with the job_id
    sm_res = smjob_mgr_job_lookup_stack_ctxt(job_id, &sm_job, &job);
    if ( SMJOB_OK != sm_res )
    {
        // TODO TEMP check new job context

        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, job_id);
        return sm_res;
    }

    XASSERT(!sm_job || !job, (unsigned int)sm_job);
    if(sm_job)
    {
        // Copy over the job ticket information
        job_ticket->job_type = sm_job->job_ticket->job_type;
        strncpy( job_ticket->job_description.job_name, sm_job->job_ticket->job_description.job_name, 
                 SMJOB_MAX_NAME_LEN );
        job_ticket->job_description.job_name[SMJOB_MAX_NAME_LEN-1] = '\0';
        strncpy( job_ticket->job_description.originating_user_name, 
                 sm_job->job_ticket->job_description.originating_user_name, SMJOB_MAX_NAME_LEN );
        job_ticket->job_description.originating_user_name[SMJOB_MAX_NAME_LEN-1] = '\0';
        job_ticket->job_description.vnd_extensions = sm_job->job_ticket->job_description.vnd_extensions;
        job_ticket->job_processing         = sm_job->job_ticket->job_processing;
        job_ticket->doc_processing         = sm_job->job_ticket->doc_processing;
        job_ticket->must_honor             = sm_job->job_ticket->must_honor;
        job_ticket->ipp_attribute_fidelity = sm_job->job_ticket->ipp_attribute_fidelity;
        job_ticket->omitted                = sm_job->job_ticket->omitted;
        job_ticket->vnd_extensions         = sm_job->job_ticket->vnd_extensions;
    }
    else
    {
        ASSERT(job);
        *job_ticket = job->ticket;
    }

    return sm_res;
}

void dump_job_ticket(smjob_ticket_t *job_ticket)
{

   dbg_printf("Job type: %s\n", smjob_type_to_string(job_ticket->job_type));
   dbg_printf("\n");

   dbg_printf("Start Job Description\n");
   dbg_printf("\n");
   dbg_printf("\tJob name: %s\n", job_ticket->job_description.job_name);
   dbg_printf("\tOriginating user name: %s\n", 
              job_ticket->job_description.originating_user_name);
   dbg_printf("\n");
   dbg_printf("End Job Description\n");

   dbg_printf("Start Job Processing\n");
   dbg_printf("\n");
   dbg_printf("\tPriority: %d\n", job_ticket->job_processing.priority);
   dbg_printf("\n");
   dbg_printf("End Job Processing\n");


   dbg_printf("Start Document Processing\n");
   dbg_printf("\n");
   dbg_printf("\tOrientation: %s\n", 
              smjob_orientation_to_string(job_ticket->doc_processing.orientation));

   dbg_printf("\tStart Resolution\n");
   dbg_printf("\t\tWidth: %d\n", job_ticket->doc_processing.resolution.xfeed_dir);
   dbg_printf("\t\tHeight: %d\n", job_ticket->doc_processing.resolution.feed_dir);
   dbg_printf("\t\tUints: %s\n", 
              smjob_resunits_to_string(job_ticket->doc_processing.resolution.units));
   dbg_printf("\n");
   dbg_printf("\tEnd Resolution\n");

   if (job_ticket->job_type == SMJOB_TYPE_PRINT)
   {

      dbg_printf("\tCopies: %d\n", job_ticket->doc_processing.type.print.copies);
      dbg_printf("\tMedia size: %d\n", job_ticket->doc_processing.type.print.media_size);


      dbg_printf("\tMedia type: %d\n", job_ticket->doc_processing.type.print.media_type);

      dbg_printf("\tStart media collection\n");
      dbg_printf("\n");

      dbg_printf("\t\tX dimension: %d\n", 
                 job_ticket->doc_processing.type.print.media_col.x_dimension);
      dbg_printf("\t\tY dimension: %d\n", 
                 job_ticket->doc_processing.type.print.media_col.y_dimension);
      dbg_printf("\t\tBottom margin: %d\n", 
                 job_ticket->doc_processing.type.print.media_col.bottom_margin);
      dbg_printf("\t\tLeft margin: %d\n", 
                 job_ticket->doc_processing.type.print.media_col.left_margin);
      dbg_printf("\t\tRight margin: %d\n", 
                 job_ticket->doc_processing.type.print.media_col.right_margin);
      dbg_printf("\t\tTop margin: %d\n", 
                 job_ticket->doc_processing.type.print.media_col.top_margin);

      dbg_printf("\t\tSource: %s\n", 
                 job_ticket->doc_processing.type.print.media_col.source);
      dbg_printf("\t\tType: %s\n", job_ticket->doc_processing.type.print.media_col.type);
      dbg_printf("\t\tOutput bin: %s\n", job_ticket->doc_processing.type.print.media_col.output_bin);
      dbg_printf("\n");

      dbg_printf("\tEnd media collection\n");

   }
   else
   {
      dbg_printf("Cannot display document processing for job type: %d\n", 
                 job_ticket->job_type);
   }

   dbg_printf("\tCollate: %d\n", job_ticket->doc_processing.type.print.sheet_collate);
   dbg_printf("\tSides: %s\n", job_ticket->doc_processing.type.print.sides);
   dbg_printf("\tQuality: %s\n", 
              smjob_quality_to_string(job_ticket->doc_processing.type.print.quality));
   dbg_printf("\tColor mode: %s\n", job_ticket->doc_processing.type.print.color_mode);


   dbg_printf("\tMust honor mask\n");
   dbg_printf("\n");

   uint32_t i;
   for (i = 0; i < SMJOB_NUM_ATTRIBUTE_GROUPS; ++i)
   {
      dbg_printf("\t\tMask[%d]: %#x\n", i, job_ticket->must_honor.mask[i]);
   }

   dbg_printf("End Document Processing\n");
}

//-----------------------------------------------------------------------------
uint32_t smjob_get_next(uint32_t job_id)
{
    smjob_rcode_t sm_res;
    smjob_t *sm_job;
    uint32_t next_job_id;

    // Check inputs
    if ( ( SMJOB_INVALID_JOB_ID != job_id) && 
         ( ( INITIAL_JOB_ID_VALUE > job_id ) || ( MAX_JOB_ID_VALUE < job_id ) ) )
    {
        DBG_ERR("%s: Invalid job_id provided \n", __func__);
        return SMJOB_INVALID_JOB_ID;
    }

    // Find the sm_job that correlates with the job_id
    // Note that if job_id == SMJOB_INVALID_JOB_ID, this will return
    // the job at first_valid_job_index.
    sm_res = smjob_mgr_job_lookup_stack(job_id, &sm_job);
    if ( SMJOB_OK != sm_res )
    {
        DBG_ERR("%s: Failure getting sm_job for job_id = %ld \n", __func__, job_id);
        return SMJOB_INVALID_JOB_ID;
    }

    // Search for the job_id
    if ( SMJOB_INVALID_JOB_ID == job_id )
    {
        // The first job in the stack was requested
        next_job_id = sm_job->job_status->job_id;
    }
    else
    {
        // Find the job after the one requested; ensure it's a valid job
        next_job_id = sm_job->job_status->job_id + 1;
        if ( MAX_JOB_ID_VALUE < next_job_id )
        {
            // Rollover if necessary
            next_job_id = INITIAL_JOB_ID_VALUE;
        }
        sm_res = smjob_mgr_job_lookup_stack(next_job_id, &sm_job);
        if ( SMJOB_OK != sm_res )
        {
            return SMJOB_INVALID_JOB_ID;
        }
    }

    return next_job_id;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_icons(smjob_type_t     job_type, 
			      uint32_t         index, 
			      char           **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Call the oem config function
    sm_res = smjob_oem_get_icons(job_type, index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_compression(smjob_type_t        job_type, 
					      uint32_t            index, 
					      smjob_compression_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Call the oem config function
    sm_res = smjob_oem_get_supported_compression(job_type, index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_default_format(smjob_type_t job_type, smjob_format_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_PRINTF_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }
    
    // Start with an initialized format
    memset(value, 0, sizeof(smjob_format_t));

    // Call the oem config function
    sm_res = smjob_oem_get_default_format(job_type, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_format(smjob_type_t   job_type, 
                                         uint32_t       index, 
                                         smjob_format_t **value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Call the oem config function
    sm_res = smjob_oem_get_supported_format(job_type, index, value);

    return sm_res;
}

char *smjob_get_mime_type(smjob_format_t doc_format)
{
    uint32_t index = (uint32_t)doc_format;
    XASSERT(index < g_smjob_format_map_size, index);
    return g_smjob_format_map[index];
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_finishings(smjob_type_t job_type, uint32_t index, smjob_finishing_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( ( 0 == value ) ||
        (( SMJOB_TYPE_UNSPECIFIED != job_type ) &&
         ( SMJOB_TYPE_PRINT != job_type ) &&
         ( SMJOB_TYPE_SCAN != job_type ) &&
         ( SMJOB_TYPE_FAXIN != job_type ) &&
         ( SMJOB_TYPE_FAXOUT != job_type )) )
    {
        DBG_ERR("%s: Invalid job_type or value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Call the oem config function
    sm_res = smjob_oem_get_supported_finishings(job_type, index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_default_orientation(smjob_type_t job_type, smjob_orientation_t *value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (which must check the inputs)
    sm_res = smjob_oem_get_default_orientation(job_type, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_orientation(smjob_type_t job_type, uint32_t index, smjob_orientation_t *value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (which must check the inputs)
    sm_res = smjob_oem_get_supported_orientation(job_type, index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_default_resolution(smjob_type_t job_type, smjob_resolution_t *value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_default_resolution(job_type, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_resolution(smjob_type_t job_type, uint32_t index, smjob_resolution_t *value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_supported_resolution(job_type, index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_input_source(uint32_t index, smjob_input_source_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( 0 == value )
    {
        DBG_ERR("%s: Invalid value provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Call the oem config function
    sm_res = smjob_oem_get_supported_input_source(index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_default_copies(uint32_t *value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_default_copies(value);

    return sm_res;
}

//-----------------------------------------------------------------------------
/// A range bounded by min and max values inclusive (print-specific).
smjob_rcode_t smjob_get_supported_copies(uint32_t *min_value, uint32_t *max_value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_supported_copies(min_value, max_value);

    return sm_res;
}

//-----------------------------------------------------------------------------
/// "true" if sheet collation is supported, "false" otherwise (print-specific).
smjob_rcode_t smjob_get_supported_sheet_collate(bool *is_supported)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if ( 0 == is_supported )
    {
        DBG_ERR("%s: Invalid bool provided \n", __func__);
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    // Call the oem config function
    sm_res = smjob_oem_get_supported_sheet_collate(is_supported);

    return sm_res;
}

//-----------------------------------------------------------------------------
/// (print-specific)
smjob_rcode_t smjob_get_default_sides(smjob_type_t job_type, smjob_sides_t **value)
{
    smjob_rcode_t sm_res;

    XASSERT(job_type == SMJOB_TYPE_PRINT, job_type); // currently only PRINT supported

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_default_sides(value);

    return sm_res;
}

//-----------------------------------------------------------------------------
/// (print-specific)
smjob_rcode_t 
smjob_get_supported_print_content_optimize(smjob_type_t                   job_type, 
                                           uint32_t                       index, 
                                           smjob_print_content_optimize_t **value)
{
    smjob_rcode_t sm_res;

    XASSERT(job_type == SMJOB_TYPE_PRINT, job_type); // currently only PRINT supported

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_supported_print_content_optimize(index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
/// (print-specific)
smjob_rcode_t 
smjob_get_default_print_content_optimize(smjob_type_t                   job_type, 
                                         smjob_print_content_optimize_t **value)
{
    smjob_rcode_t sm_res;

    XASSERT(job_type == SMJOB_TYPE_PRINT, job_type); // currently only PRINT supported

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_default_print_content_optimize(value);

    return sm_res;
}

//-----------------------------------------------------------------------------
/// (print-specific)
smjob_rcode_t smjob_get_supported_sides(smjob_type_t  job_type, 
                                        uint32_t      index, 
                                        smjob_sides_t **value)
{
    smjob_rcode_t sm_res;

    XASSERT(job_type == SMJOB_TYPE_PRINT, job_type); // currently only PRINT supported

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_supported_sides(index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_default_color_mode(smjob_type_t       job_type, 
                                           smjob_color_mode_t **value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_default_color_mode(job_type, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_color_mode(smjob_type_t       job_type, 
                                             uint32_t           index, 
                                             smjob_color_mode_t **value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_supported_color_mode(job_type, index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
/// (print-specific)
smjob_rcode_t smjob_get_default_quality(smjob_quality_t *value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_default_quality(value);

    return sm_res;
}

//-----------------------------------------------------------------------------
/// (print-specific)
smjob_rcode_t smjob_get_supported_quality(uint32_t index, smjob_quality_t *value)
{
    smjob_rcode_t sm_res;

    // Call the oem config function (oem layer must validate inputs)
    sm_res = smjob_oem_get_supported_quality(index, value);

    return sm_res;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_default_media_col(smjob_type_t job_type, smjob_media_col_t *value)
{
    return smjob_oem_get_default_media_col(job_type, value);
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_media_size(smjob_type_t job_type, uint32_t index, mediasize_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_size_entry_t *table;
            uint32_t cnt = sm_media_size_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if(index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
                break;
            }
            *value = table[index].size_enum;
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

    return sm_res;
}

smjob_rcode_t smjob_get_supported_media_size_name(smjob_type_t job_type, uint32_t index, char** name)
{
    return smjob_oem_get_supported_media_size_name(job_type, index, name);
}

smjob_rcode_t smjob_get_supported_default_media_size_name(smjob_type_t job_type, char** name)
{
    return smjob_oem_get_supported_default_media_size_name(job_type, name);
}
smjob_rcode_t smjob_set_supported_default_media_size_name(smjob_type_t job_type, char* name, uint32_t name_len)
{
    return smjob_oem_set_supported_default_media_size_name(job_type, name, name_len);
}

smjob_rcode_t smjob_get_supported_media_source(smjob_type_t job_type, uint32_t index, char** source)
{
    return smjob_oem_get_supported_media_source(job_type, index, source);
}

smjob_rcode_t smjob_get_media_table_source(smjob_type_t job_type, uint32_t index, char** source)
{
    return smjob_oem_get_media_table_source(job_type, index, source);
}

char *sm_media_size_enum_to_name(mediasize_t size_enum)
{
    int i;

    for( i = 0; i < SM_MEDIA_SIZE_MAP_CNT; i++ )
    {
        if( g_sm_media_size_map[i].size_enum == size_enum )
        {
            ASSERT(g_sm_media_size_map[i].name);
            return g_sm_media_size_map[i].name;
        }
    }
    return "";
}

mediasize_t sm_media_size_name_to_enum( const char * pwg_name )
{
    int i;

    ASSERT( pwg_name );

    for( i = 0; i < SM_MEDIA_SIZE_MAP_CNT; i++ )
    {
        ASSERT(g_sm_media_size_map[i].name);
        if( 0 == strcmp( g_sm_media_size_map[i].name, pwg_name ) )
        {
            return g_sm_media_size_map[i].size_enum;
        }
    }

    return MEDIASIZE_BAD;
}

smjob_rcode_t smjob_get_supported_media_size_x_dim(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    ASSERT(value != NULL);

    return smjob_oem_get_supported_media_size_x_dim(job_type, index, value);
}

uint32_t sm_media_size_enum_to_x_dim(mediasize_t size_enum)
{
    sm_media_size_entry_t *table;
    uint32_t cnt = sm_media_size_get_table(&table);
    ASSERT(table);
    ASSERT(cnt);

    uint32_t i;
    for( i = 0; i < cnt; i++ )
    {
        if( table[i].size_enum == size_enum )
        {
            ASSERT(table[i].width);
            return table[i].width;
        }
    }
    return 0;
}

smjob_rcode_t smjob_get_supported_media_size_y_dim(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    ASSERT(value != NULL);

    return smjob_oem_get_supported_media_size_y_dim(job_type, index, value);
}

uint32_t sm_media_size_enum_to_y_dim(mediasize_t size_enum)
{
    sm_media_size_entry_t *table;
    uint32_t cnt = sm_media_size_get_table(&table);
    uint32_t i;

    for( i = 0; i < cnt; i++ )
    {
        if( table[i].size_enum == size_enum )
        {
            ASSERT(table[i].height);
            return table[i].height;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_media_top_margin(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    ASSERT(value != NULL);
    return smjob_oem_get_supported_media_top_margin(job_type, index, value);
}

smjob_rcode_t smjob_get_supported_media_bottom_margin(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    ASSERT(value != NULL);
    return smjob_oem_get_supported_media_bottom_margin(job_type, index, value);
}

smjob_rcode_t smjob_get_supported_media_left_margin(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    ASSERT(value != NULL);
    return smjob_oem_get_supported_media_left_margin(job_type, index, value);
}

smjob_rcode_t smjob_get_supported_media_right_margin(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    ASSERT(value != NULL);
    return smjob_oem_get_supported_media_right_margin(job_type, index, value);
}

smjob_rcode_t smjob_get_aiprint_enabled(smjob_type_t job_type, 
                                        uint32_t     index, 
                                        uint32_t     *value)
{
   ASSERT(value != NULL);
   
   return(smjob_oem_get_aiprint_enabled(job_type, index, value));
}


//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_output_bin_default(smjob_type_t job_type, char** name)
{
    return smjob_oem_get_output_bin_default(job_type, name);
}

smjob_rcode_t smjob_get_output_bin_supported(smjob_type_t job_type, uint32_t index, char** name)
{
    return smjob_oem_get_output_bin_supported(job_type, index, name);
}

//-----------------------------------------------------------------------------
smjob_rcode_t smjob_get_supported_media_type(smjob_type_t job_type, uint32_t index, mediatype_t *value)
{
    smjob_rcode_t sm_res = SMJOB_OK;

    // Check inputs
    if( !value )
    {
        sm_res = SMJOB_FAIL;
        return sm_res;
    }

    switch( job_type )
    {
        case SMJOB_TYPE_PRINT:
        {
            sm_media_type_entry_t *table;
            uint32_t cnt = sm_media_type_get_table(&table);
            ASSERT(table);
            ASSERT(cnt);

            if(index >= cnt)
            {
                // index out of range
                sm_res = SM_BAD_INDEX;
                break;
            }
            *value = table[index]->type_enum;
            break;
        }

        default:
            sm_res = SMJOB_FAIL;
            break;
    }

    return sm_res;
}

smjob_rcode_t smjob_get_supported_media_type_name(smjob_type_t job_type, uint32_t index, char** name)
{
    return smjob_oem_get_supported_media_type_name(job_type, index, name);
    }

smjob_rcode_t smjob_get_supported_default_media_type_name(smjob_type_t job_type, char** name)
{
    return smjob_oem_get_supported_default_media_type_name(job_type, name);
}

char *sm_media_type_enum_to_name(mediatype_t type_enum)
{
    int i;

    for( i = 0; i < SM_MEDIA_TYPE_MAP_CNT; i++ )
    {
        if( g_sm_media_type_map[i].type_enum == type_enum )
        {
            ASSERT(g_sm_media_type_map[i].name);
            return g_sm_media_type_map[i].name;
        }
    }
    return "";
}

smjob_rcode_t smjob_get_media_table_type(smjob_type_t job_type, uint32_t index, char** name)
{
    return smjob_oem_get_media_table_type(job_type, index, name);
}

smjob_rcode_t smjob_get_media_table_duplex(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    return smjob_oem_get_media_table_duplex(job_type, index, value);
}

smjob_rcode_t smjob_markers_available(smjob_type_t job_type, uint32_t index, uint32_t *value)
{
    return smjob_oem_markers_available(job_type, index, value);
}

smjob_rcode_t smjob_add_user_data_to_job_ticket(uint32_t job_id, void *user_data)
{
    smjob_rcode_t smjob_rcode    = SMJOB_FAIL;
    smjob_t       *ret_job_entry = NULL;

    smjob_rcode = smjob_mgr_get_job_entry(job_id, &ret_job_entry);

    if (smjob_rcode == SMJOB_OK)
    {
        ret_job_entry->user_data = user_data;
    }

    return(smjob_rcode);
}

char *smjob_get_job_state_reasons_str(smjob_state_reason_t job_state_reason)
{
    return(g_smjob_state_reason_map[job_state_reason]); 
}

uint32_t smjob_get_queued_job_count()
{
    return smjob_mgr_get_queued_job_count();
}

bool smjob_accepting_jobs()
{
    return smjob_mgr_accepting_jobs();
}

smjob_rcode_t smjob_get_printer_geo_location(smjob_type_t job_type, char** location)
{
    return smjob_oem_get_printer_geo_location(job_type, location);
}



//=============================================================================
// Supporting functions
//=============================================================================


void smjob_lock_ctxt(void)
{
    SMJOB_LOCK_CONTEXT();
}

void smjob_unlock_ctxt(void)
{
    SMJOB_UNLOCK_CONTEXT();
}

// get pointer to the singleton media size table, creating if initial access
// returns number of entries in the table
uint32_t sm_media_size_get_table(sm_media_size_entry_t **table)
{
    /* Table representing an array of media sizes supported by the SDK's current OEM
     * configuration settings.  The array is a subset of the media sizes defined by the
     * g_sm_media_size_map initializer list.
     */
    static sm_media_size_entry_t *sm_media_size_table = NULL;
    static uint32_t sm_media_size_cnt = 0;

    SMJOB_LOCK_CONTEXT();
    if(sm_media_size_table)
    {
        // singleton already exists
        goto done;
    }

    // initial access -- singleton does not yet exist
    XASSERT(sm_media_size_cnt == 0, sm_media_size_cnt); 
    uint32_t num_reported;  // media sizes reported by paper module but not yet validated
    num_reported = paper_get_num_media_sizes();
    sm_media_size_table = (sm_media_size_entry_t *)MEM_MALLOC( num_reported * sizeof(sm_media_size_entry_t) );
    if(!sm_media_size_table)
    {
        // mem allocation failed
        goto done;
    }

    error_type_t error_type;
    uint32_t i;
    uint32_t map_idx;
    uint32_t entry_idx = 0;
    mediasize_t mediasize;
    bool supported;
#ifdef DEBUG   
    bool excluded;
    uint32_t exclude_idx;
#endif // DEBUG   

    for( i = 0; i < num_reported; i++ )
    {
        mediasize = paper_get_entry(i);

        // check if this is a supported media size
        supported = false;
        for( map_idx = 0; map_idx < SM_MEDIA_SIZE_MAP_CNT; map_idx++ )
        {
            if( mediasize == g_sm_media_size_map[map_idx].size_enum )
            {
                supported = true;
                break;
            }
        }

        if(!supported)
        {
#ifdef DEBUG    
            // confirm current mediasize is in exclusions list
            for( exclude_idx = 0; exclude_idx < SM_MEDIA_SIZE_EXCLUSIONS_CNT; exclude_idx++ )
            {
                excluded = false;
                if( mediasize == g_sm_media_size_exclusions[exclude_idx] )
                {
                    excluded = true;
                    break;
                }
            }
            // an assertion here means current mediasize is unaccounted for and must be added to
            // either sm_media_size_map or sm_media_size_exclusions list above
//            XASSERT(supported || excluded, mediasize);
#endif // DEBUG    
            continue;
        }

        // add entry for this media size
        sm_media_size_table[entry_idx].size_enum = mediasize;
        sm_media_size_table[entry_idx].name = g_sm_media_size_map[map_idx].name;
        error_type = paper_get_media_size(mediasize, 
                                          &sm_media_size_table[entry_idx].width,
                                          &sm_media_size_table[entry_idx].height);
        ASSERT(error_type == OK);
        entry_idx++;
    }
    sm_media_size_cnt = entry_idx;
    XASSERT(sm_media_size_cnt <= num_reported, sm_media_size_cnt);

done:
    SMJOB_UNLOCK_CONTEXT();
    *table = sm_media_size_table;
    return sm_media_size_cnt;
}

uint32_t sm_media_type_get_table(sm_media_type_entry_t **table)
{
    /* Table representing an array of media types supported by the SDK's current OEM
     * configuration settings.  The array is a subset of the media types defined by the
     * g_sm_media_type_map initializer list.
     */
    static sm_media_type_entry_t *sm_media_type_table = NULL;
    static uint32_t sm_media_type_cnt = 0;

    SMJOB_LOCK_CONTEXT();
    if(sm_media_type_table)
    {
        // singleton already exists
        goto done;
    }

    // initial access -- singleton does not yet exist
    XASSERT(sm_media_type_cnt == 0, sm_media_type_cnt); 
    uint32_t num_reported;  // media types reported by paper module but not yet validated
    num_reported = paper_get_num_media_types();
    sm_media_type_table = (sm_media_type_entry_t *)MEM_MALLOC( num_reported * sizeof(sm_media_type_entry_t) );
    if(!sm_media_type_table)
    {
        // mem allocation failed
        goto done;
    }

    uint32_t i;
    uint32_t map_idx;
    uint32_t entry_idx = 0;
    mediatype_t mediatype;
    bool supported;
#ifdef DEBUG   
    bool excluded;
    uint32_t exclude_idx;
#endif // DEBUG   

    for( i = 0; i < num_reported; i++ )
    {
        mediatype = papertypes_get_entry(i);

        // check if this is a supported media type
        supported = false;
        for( map_idx = 0; map_idx < SM_MEDIA_TYPE_MAP_CNT; map_idx++ )
        {
            if( mediatype == g_sm_media_type_map[map_idx].type_enum )
            {
                supported = true;
                break;
            }
        }

        if(!supported)
        {
#ifdef DEBUG    
            // confirm current mediasize is in exclusions list
            for( exclude_idx = 0; exclude_idx < SM_MEDIA_TYPE_EXCLUSIONS_CNT; exclude_idx++ )
            {
                excluded = false;
                if( mediatype == g_sm_media_type_exclusions[exclude_idx] )
                {
                    excluded = true;
                    break;
                }
            }
            // an assertion here means current mediatype is unaccounted for and must be added to
            // either sm_media_type_map or sm_media_type_exclusions list above
            XASSERT(supported || excluded, mediatype);
#endif // DEBUG    
            continue;
        }

        // add entry for this media size
        sm_media_type_table[entry_idx] = &g_sm_media_type_map[map_idx];
        entry_idx++;
    }
    sm_media_type_cnt = entry_idx;
    XASSERT(sm_media_type_cnt <= num_reported, sm_media_type_cnt);

done:
    SMJOB_UNLOCK_CONTEXT();
    *table = sm_media_type_table;
    return sm_media_type_cnt;
}

// Copy the doc ticket that is passed in
static void copy_doc_ticket(smjob_doc_ticket_t *orig_ticket, smjob_doc_ticket_t **new_ticket, smjob_type_t job_type)
{
    smjob_doc_ticket_t     *doc_ticket = NULL;

    doc_ticket = (smjob_doc_ticket_t *)MEM_MALLOC(sizeof(smjob_doc_ticket_t));
    ASSERT(doc_ticket);

    // Set to zeros...
    memset(doc_ticket, 0, sizeof(smjob_doc_ticket_t));

    // Copy over the info...
    strncpy( doc_ticket->doc_description.doc_name, orig_ticket->doc_description.doc_name, 
             SMJOB_MAX_NAME_LEN );
    doc_ticket->doc_description.doc_name[SMJOB_MAX_NAME_LEN-1] = '\0';
    doc_ticket->doc_description.type = orig_ticket->doc_description.type;
    doc_ticket->doc_description.vnd_extensions = orig_ticket->doc_description.vnd_extensions;
    doc_ticket->doc_processing.orientation = orig_ticket->doc_processing.orientation;
    doc_ticket->doc_processing.resolution = orig_ticket->doc_processing.resolution;
    doc_ticket->doc_processing.type = orig_ticket->doc_processing.type;
    doc_ticket->doc_processing.vnd_extensions = orig_ticket->doc_processing.vnd_extensions;
    doc_ticket->must_honor = orig_ticket->must_honor;
    doc_ticket->omitted = orig_ticket->omitted;
    doc_ticket->vnd_extensions = orig_ticket->vnd_extensions;

    switch(job_type)
    {
        case SMJOB_TYPE_PRINT:
        {
            doc_ticket->doc_description.type.print.recv_func = orig_ticket->doc_description.type.print.recv_func;
            doc_ticket->doc_description.type.print.user_data = orig_ticket->doc_description.type.print.user_data;
            break;
        }
        case SMJOB_TYPE_SCAN:
        {
            doc_ticket->doc_description.type.scan.send_func = orig_ticket->doc_description.type.scan.send_func;
            doc_ticket->doc_description.type.scan.user_data = orig_ticket->doc_description.type.scan.user_data;
            break;
        }
        default:
        {
            // invalid or not-yet-supported job type
            XASSERT(0, job_type);
            break;
        }
    }

    // ...and send it off.
    *new_ticket = doc_ticket;
}

// Copy the job ticket that is passed in
static void copy_job_ticket(smjob_ticket_t *orig_ticket, smjob_ticket_t **new_ticket)
{
    smjob_ticket_t         *job_ticket      = NULL;

    // Get the memory...
    job_ticket = (smjob_ticket_t *)MEM_MALLOC(sizeof(smjob_ticket_t));
    ASSERT(job_ticket);
    
    // Set to zeros...
    memset(job_ticket, 0, sizeof(smjob_ticket_t));

    // Copy over the info...
    job_ticket->job_type = orig_ticket->job_type;
    strncpy( job_ticket->job_description.job_name, orig_ticket->job_description.job_name, 
             SMJOB_MAX_NAME_LEN );
    job_ticket->job_description.job_name[SMJOB_MAX_NAME_LEN-1] = '\0';
    strncpy( job_ticket->job_description.originating_user_name, 
             orig_ticket->job_description.originating_user_name, SMJOB_MAX_NAME_LEN );
    job_ticket->job_description.originating_user_name[SMJOB_MAX_NAME_LEN-1] = '\0';
    job_ticket->job_processing         = orig_ticket->job_processing;
    job_ticket->doc_processing         = orig_ticket->doc_processing;
    job_ticket->must_honor             = orig_ticket->must_honor;
    job_ticket->ipp_attribute_fidelity = orig_ticket->ipp_attribute_fidelity;
    job_ticket->omitted                = orig_ticket->omitted;
    job_ticket->vnd_extensions         = orig_ticket->vnd_extensions;

    // ...and send it off.
    *new_ticket = job_ticket;
}

void dump_doc_ticket(smjob_doc_ticket_t *doc_ticket, smjob_type_t job_type)
{

   dbg_printf("Document Description\n");
   dbg_printf("\n");
   
   dbg_printf("Name: %s\n", doc_ticket->doc_description.doc_name);
   dbg_printf("Job Type: %s\n", smjob_type_to_string(job_type));
   dbg_printf("\n");

   if (job_type == SMJOB_TYPE_PRINT)
   {
      dbg_printf("Print description\n");
      dbg_printf("\n");
      dbg_printf("\tCompression: %s\n", doc_ticket->doc_description.type.print.compression);
      dbg_printf("\tFormat: %s\n", doc_ticket->doc_description.type.print.format);
      dbg_printf("\n");
   }
   else
   {
      dbg_printf("Cannot display document description for job type: %d\n", job_type);
   }

   dbg_printf("Document Processing\n");
   dbg_printf("\n");

   dbg_printf("\tOrientation: %s\n", 
              smjob_orientation_to_string(doc_ticket->doc_processing.orientation));

   dbg_printf("\tStart Resolution\n");
   dbg_printf("\n");
   dbg_printf("\t\tWidth: %d\n", doc_ticket->doc_processing.resolution.xfeed_dir);
   dbg_printf("\t\tHeight: %d\n", doc_ticket->doc_processing.resolution.feed_dir);
   dbg_printf("\t\tUints: %s\n", 
              smjob_resunits_to_string(doc_ticket->doc_processing.resolution.units));
   dbg_printf("\n");
   dbg_printf("\tEnd Resolution\n");


   if (job_type == SMJOB_TYPE_PRINT)
   {

      dbg_printf("\tCopies: %d\n", doc_ticket->doc_processing.type.print.copies);
      dbg_printf("\tMedia size: %d\n", doc_ticket->doc_processing.type.print.media_size);


      dbg_printf("\tMedia type: %d\n", doc_ticket->doc_processing.type.print.media_type);

      dbg_printf("\tStart media collection\n");
      dbg_printf("\n");
      dbg_printf("\t\tX dimension: %d\n", 
                 doc_ticket->doc_processing.type.print.media_col.x_dimension);
      dbg_printf("\t\tY dimension: %d\n", 
                 doc_ticket->doc_processing.type.print.media_col.y_dimension);
      dbg_printf("\t\tBottom margin: %d\n", 
                 doc_ticket->doc_processing.type.print.media_col.bottom_margin);
      dbg_printf("\t\tLeft margin: %d\n", 
                 doc_ticket->doc_processing.type.print.media_col.left_margin);
      dbg_printf("\t\tRight margin: %d\n", 
                 doc_ticket->doc_processing.type.print.media_col.right_margin);
      dbg_printf("\t\tTop margin: %d\n", 
                 doc_ticket->doc_processing.type.print.media_col.top_margin);

      dbg_printf("\t\tSource: %s\n", 
                 doc_ticket->doc_processing.type.print.media_col.source);
      dbg_printf("\t\tType: %s\n", doc_ticket->doc_processing.type.print.media_col.type);
      dbg_printf("\t\tOutput bin: %s\n", doc_ticket->doc_processing.type.print.media_col.output_bin);
      dbg_printf("\n");
      dbg_printf("\tEnd media collection\n");

   }
   else
   {
      dbg_printf("Cannot display document processing for job type: %d\n", job_type);
   }

   dbg_printf("\tCollate: %d\n", doc_ticket->doc_processing.type.print.sheet_collate);
   dbg_printf("\tSides: %s\n", doc_ticket->doc_processing.type.print.sides);
   dbg_printf("\tQuality: %s\n", 
              smjob_quality_to_string(doc_ticket->doc_processing.type.print.quality));
   dbg_printf("\tColor mode: %s\n", doc_ticket->doc_processing.type.print.color_mode);


   dbg_printf("\tMust honor mask\n");
   dbg_printf("\n");
   uint32_t i;
   for (i = 0; i < SMJOB_NUM_ATTRIBUTE_GROUPS; ++i)
   {
      dbg_printf("\t\tMask[%d]: %#x\n", i, doc_ticket->must_honor.mask[i]);
   }
}

