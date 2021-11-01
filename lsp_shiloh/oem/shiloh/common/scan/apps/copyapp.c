/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2008-2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* davep 23-Jul-2008 ; added -D__AGMESSAGE_H_ to makefile to prevent agMessage.h
 * from being included in the scan code.  But we need agMessage.h in this file.
 */
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>

#include "posix_ostools.h"

#include "list.h"
#include "memAPI.h"
#include "lassert.h"
#include "agMessage.h"
#include "agConnectMgr.h"
#include "SJMgr.h"
#include "agJobUtils.h"
#include "ResourceMap.h"
#include "utils.h"
#include "debug.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "BigBuffer.h"
#include "event_observer.h"
#include "response_subject.h"
#include "cmd_proc_api.h"
#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scantools.h"
#include "scanvars.h"
#include "copyapp_api.h"
#include "copyapp.h"
#include "copy_config.h"
#include "scanhwerr.h"
#include "scanman.h"
#include "scanmech.h"
#include "scanplat.h"
#include "scanvars.h"
#include "scantypes.h"
#include "fraction.h"
#include "safeint.h"
#include "scancap.h"
#include "scanmsg.h"
#include "adfsensor.h"
#include "print_job_api.h"
#include "PrintJobInfo.h"
#include "printvars_api.h"
#include "data_access_api.h"
#include "dma_buffer.h"
#include "nvram_api.h"
#include "logger.h"
#include "siqdata.h"
#include "mbuffer.h"
#include "db_api.h"
#include "a3mergeInterface.h"
#include "scanapp_margins.h"

#define DBG_PRFX "COPY: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(2)
// debugging method
/* In code using below...
 * DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
 * DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)
 * In Console : CMD ==> logger set SCAN copy LOG_SDH_DEBUG
*/

CopyConf nvram_shadow_copy_config;

static int copylog_level = 1;
#define COPYLOG(P) \
    va_list ap;\
    if( (P) > copylog_level ) { return; }\
    va_start( ap, format );  \
    minVsprintf( NULL, (char *)format, ap ); \
    va_end( ap );

void copylog1( const char *format, ... ) { COPYLOG(1) }
void copylog2( const char *format, ... ) { COPYLOG(2) }
void copylog3( const char *format, ... ) { COPYLOG(3) }
#ifdef __KERNEL__
  #define cdbg1(...) printk( KERN_INFO __VA_ARGS__ ) 
  #define cdbg2(...) printk( KERN_INFO __VA_ARGS__ ) 
#else
#if 0
  #define cdbg1(...) copylog1( __VA_ARGS__ ) 
  #define cdbg2(...) copylog2( __VA_ARGS__ ) 
  #define cdbg3(...) copylog3( __VA_ARGS__ ) 
#else
  #define cdbg1(...) dbg1("[COPY] " __VA_ARGS__ ) 
  #define cdbg2(...) dbg1("[COPY] " __VA_ARGS__ ) 
  #define cdbg3(...) copylog3( __VA_ARGS__ ) 
#endif
#endif

#define COPYTASK_QUEUE_SIZE 20  ///< Size of the message queue.
#define COPYTASK_STACK_SIZE (PTHREAD_STACK_MIN*4) ///< Size of the stack.

#define PRINTER_PIXEL_DEPTH 2
#define COPYAPP_VARS_VERSION 4
#define MAX_PADDING_STRIP_LINES 64

#define MAX_STRIP_ROWNUMBER_DELTA 64 // needs to be moved into header file

#define PAGE_OTHER_SIDE(pg) ((pg) ^ 1)  // xor to flip to other page side (flips between 0 and 1)
#define PAGE_SIDE_FRONT 0
#define PAGE_SIDE_BACK  1
/*********** SIQ Tuning **************/
#define MAX_TUNING_FILE_NAME	35
#define TUNING_SIQ_FILE_FOLDER	"/tuning/"

static char siq_file_name[MAX_TUNING_FILE_NAME];
static scan_err_t load_siqfile(void);		// from siq file
static bool find_siq_name(uint8_t quality);
static scan_err_t file_to_buffer( const char *filename, struct mbuffer *buf );
/***************************************/
static uint32_t number_of_scan_originals=0;

/* Maximum height (length) of flatbed glass is A4 size (11.7 inches). Note
 * that this value should probably live in scanplat.
 */
//#define MAX_FB_HEIGHT_HINCHES 1170
//#define MAX_FB_HEIGHT_HINCHES 1130
#define MAX_FB_HEIGHT_HINCHES 1156 // mod.kks. 1162 -> 1156
#define MAX_FB_HEIGHT_HINCHES_WITH_MARGIN 1210 // mod.kks. 1156 + 54
#define BORDER_LR_MM	40
#define BORDER_TB_MM	40
#define ALIGN32_H(a)	((int)((a+31)/32)*32)
#define ALIGN32_L(a)	(a&(~0x1F))
#define BOTTOM_PADDING_OPEN		0xFFFF

#define OFF 0
#define ON  1

#define SDH_PINETREE_YANGSAN_IC 1

typedef enum{mainUsage=0, copyJob, copyIDJob, copyContinuous, copyQuality, copyNext,
    copyResize, copyCancel, copyBrightness, copyOrient, copyCollate, copyPageSize,
    copyNumCopies, copyPduplex, copySduplex, copyColor} usage_type;
void usage(usage_type);

/* davep 20-Sep-2010 ; ENGMEDIASIZES was apparently removed from the engine
 * code (Changelist 73746). This is the first time I noticed we now have this
 * structure in copyapp instead of the print subsystem. WTF?
 */
typedef struct {
    uint32_t width;
    uint32_t height;
} ENGMEDIASIZES;

/* davep 09-Sep-2009 ; limit our memory usage during copy+collation. If we're
 * collating and another page starts and we have less than this amount of memory
 * free, abandon the collation.
 */
#define COLLATE_REQUIRE_MEMORY_FOR_NEXT_PAGE  (13*1024*1024)
uint32_t memGetHeapFreeMemorySize(mlimiter_t *heap); //LSPTODO: TEMP FOR LINKER ISSUES

typedef struct {
    uint32_t X_numerator; ///< Scale factors
    uint32_t X_denominator;  ///< Scale factors
    uint32_t Y_numerator;
    uint32_t Y_denominator;
    uint32_t PlaceMargin;
} COPY_SETTINGS;

struct copyapp_stats {
    int job_starts; /* number of jobs started */
    int pagestarts; /* number of PAGESTART messages received */
    int planedatas; /* number of PLANEDATA messages received */
    int rows_received; /* total number of rows of data received so far */
};

typedef struct _copyapp_data_strip_s
{
    ATLISTENTRY node;
    uint8_t* data;
    uint32_t num_rows;
}copyapp_data_strip_t;

typedef struct _copyapp_page_s
{
    print_page_t* print_page;
    ATLISTENTRY data_strips;
    uint32_t current_lines;
    BOOL print_page_active;
}copyapp_page_t;

// When scanning duplex the page boundaries may overlap because the front and back
// scan sensors are offset from each other.  For example we may get the start page 
// indication on the front of page 2 before we get the end page indication on the
// back of page 1.  The print pipeline does not allow us to source data from both
// pages at the same time.  Thus we use the copy_duplex_pages structure to buffer
// the page data.
static copyapp_page_t copy_duplex_pages[2];

static COPY_SETTINGS CopySettings;
static uint32_t N_up;        ///< Temporary variable to hold the number of pages/sheet
static uint32_t Time;            ///< Total time.
static uint32_t IntervalTime;         ///< Intermediate time
static uint32_t LastTimeSent;        ///< Time the last update was sent.
//static uint32_t CurrentLines;    ///< Number of lines currently.
static struct timespec start_ts;
uint32_t g_contscan_time =0;
static int copy_bpp;
// align the stack
ALIGN(8) static UINT8 copy_continuous_task_stack[COPYTASK_STACK_SIZE];
static pthread_t copy_continuous_task_id;
static pthread_mutex_t copy_continuous_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t copy_continuous_cond = PTHREAD_COND_INITIALIZER;

static uint32_t copy_continuous_flag = 0;
#define WAIT_TIME_SECONDS       1

// align the stack
ALIGN(8) static UINT8 copytask_stack[COPYTASK_STACK_SIZE];
mqd_t copytask_msgq;
static pthread_t copytask_id;
static struct copyapp_stats copyapp_stats;
static PAGEINFO g_page_info;
static BOOL g_copying_continuously;
static e_ORIENTATION CopyOrientation;
ROTATE_DIR rotation;
static BOOL duplex_scan_enabled;
static BOOL copy_started_from_adf;
static BOOL copy_cancel_invalid;

static uint32_t continuous_mono_num_copies;
static uint32_t continuous_color_num_copies;
static uint8_t old_jam_recovery;
#ifdef HAVE_NVRAM
static nvram_handle_t *nvram_handle;
#endif

static uint32_t g_minnumrows;
static uint32_t g_maxnumrows;
static uint32_t g_top_padding;
static uint32_t g_bottom_padding;
static uint32_t g_right_padding;
static uint32_t g_top_erase;
static uint32_t g_fb_bottom_padding_add;
static uint32_t g_roller_erase_adjust;
//static uint32_t g_scan_height_for_padding;

static BOOL g_legal_merge;
static BOOL g_a3_merge;
static BOOL g_dlt_merge;
//-jrsstatic uint32_t copy_resolution_x = COPY_RESOLUTION;
//-jrsstatic uint32_t copy_resolution_y = COPY_RESOLUTION;
//-jrsstatic uint32_t scan_resolution = COPY_RESOLUTION;
  
static print_job_t *pjob;
// NullPipe stubs
static int32_t NullPipeRead(IOReg *Pipe, void *DataBuffer, uint32_t Length);
static int32_t NullPipeWrite(IOReg *Pipe , 
			       void *DataBuffer, uint32_t Length);
static int32_t NullPipeIOCmd(IOReg *Pipe, ioctl_cmd_t Cmd, void *Details);

typedef struct
{
	mediasize_t code;
	uint32_t width;         ///< width in mm/100 
	uint32_t height;        ///< height in mm/100 
	mediasize_t codeLEF;
} paper_t;                      

enum CODE_SIZE {
	CODE_A4,
	CODE_A5,
	CODE_A6,
	CODE_B5,
	CODE_B6,
	CODE_LT,
	CODE_LG,
	CODE_EXECU,
	CODE_STATE,
	CODE_HLT = CODE_STATE,
	CODE_FOLIO,
	CODE_QUARTO,
	CODE_GLT,
	CODE_GLG,
	CODE_CARD,
	CODE_DCARD,
	CODE_MONARCH,
	CODE_COM9,
	CODE_COM10,
	CODE_C5,
	CODE_C6,
	CODE_DL,
	CODE_NO_PAPER,
	CODE_LEF = 0x80,
	CODE_A5LEF 	  = CODE_LEF | CODE_A5,
	CODE_DCARDLEF = CODE_LEF | CODE_DCARD,
};

static paper_t paper[] =
{
	{MEDIASIZE_A4,                           21000,  29700 },
	{MEDIASIZE_A5,                           14850,  21000, MEDIASIZE_A5_ROTATED },
	{MEDIASIZE_A6,                           10500,  14850 },
	{MEDIASIZE_B5,                           18200,  25700 },
	{MEDIASIZE_B6_JIS,                       12800,  18200 },
	{MEDIASIZE_LETTER,                       21590,  27940 },
	{MEDIASIZE_LEGAL,                        21590,  35560 },
	{MEDIASIZE_EXECUTIVE,                    18420,  26670 },
	{MEDIASIZE_STATEMENT,                    13970,  21590 },
	{MEDIASIZE_CUSTOM_85x13,                 21590,  33020 },
	{MEDIASIZE_QUARTO,						 21500,  27500 },
	{MEDIASIZE_CUSTOM_8x105,				 20320,  26670 },
	{MEDIASIZE_CUSTOM_825x14,				 20955,  35560 },
	{MEDIASIZE_JAPANESE_POSTCARD,            10000,  14800 },
	{MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED,14800,  20000, MEDIASIZE_DBL_JAPANESE_POSTCARD },
	{MEDIASIZE_ENV_MONARCH,                   9840,  19050 },
	{MEDIASIZE_ENV_9,                         9840,  22510 },
	{MEDIASIZE_ENV_10,                       10480,  24130 },
	{MEDIASIZE_ENV_C5,                       16200,  22900 },
	{MEDIASIZE_ENV_C6,                       11400,  16200 },
	{MEDIASIZE_ENV_DL,                       11000,  22000 },
};

static int32_t NullPipeRead(IOReg *Pipe, void *DataBuffer, uint32_t Length)
{
      return 0;
} 

static int32_t NullPipeWrite(IOReg *Pipe , 
			       void *DataBuffer, uint32_t Length)
{
    MEM_FREE_AND_NULL(DataBuffer);    // chuck the data.
    return(Length);
}
   
static int32_t NullPipeIOCmd(IOReg *Pipe, ioctl_cmd_t Cmd, void *Details)
{
    return 0;
}
static IOReg NullPipeMEMORY;
static IOReg *NullPipe = 0;

#ifdef HAVE_ID_COPY
  static BOOL idCopyEnable  = FALSE;
//  static BOOL idCopyPending = FALSE;
#endif

uint8_t copy_to_host_job_active();

static copyapp_observer_data_t copyapp_observer_data;

static void copyapp_observer_constructor( void )
{
    copyapp_observer_data.event = COPYAPP_EVENT_NULL;
    copyapp_observer_data.subject_observer_oid_key = COPYAPP_OBSERVER_ID;
    MInitList(&copyapp_observer_data.observers);
}

static void copyapp_observer_notify_event( copyapp_event_t event, 
                                           uint32_t number_of_scans )
{
    copyapp_observer_data_t * my_status;
    copyapp_observer_data.event = event;
    copyapp_observer_data.number_of_scans = number_of_scans;
    my_status = &copyapp_observer_data;

    NOTIFY_OBSERVERS( my_status, observers );
}

copyapp_event_t copyapp_observer_get_event( Subject_t *s )
{
    ASSERT( VALIDATE_OBJECT_KEY( s,
                                 copyapp_observer_data_t, 
                                 subject_observer_oid_key,
                                 COPYAPP_OBSERVER_ID ) );

    // message causes notify causes get so no lock needed. 
    return copyapp_observer_data.event;
}

void copyapp_observer_attach( Observer_t *o )
{
    ATTACH_OBSERVER( copyapp_observer_data.observers, o );
}

void copyapp_observer_detach(Observer_t *o)
{
    DETACH_OBSERVER( copyapp_observer_data.observers, o );
}

/* The following print_xxx() functions are for debug printing various data
 * structures used through this source file.
 */

static void
print_media_size( const ENGMEDIASIZES *m )
{
    cdbg2(  "%s width=%ld height=%ld \n", 
            __FUNCTION__, m->width, m->height );
}

static void
print_page_info( const PAGEINFO *p )
{
    cdbg2(  "%s pagetype=%d pagesize=%d bpp=%ld ppl=%ld xres=%ld yres=%ld vidx=%ld vidy=%ld duplex=%#x collate=%d\n", 
           __FUNCTION__, p->PageType, p->PageSize, p->BitsPerPixel,
           p->PixelsPerLine, p->X_Resolution, p->Y_Resolution, p->Video_X,
           p->Video_Y, p->duplexInfo, p->collate
         );
}

#if 0
static void
print_copy_settings( const COPY_SETTINGS *c )
{
    cdbg2(  "%s x=%ld/%ld y=%ld/%ld margin=%ld\n", __FUNCTION__, c->X_numerator,
                c->X_denominator, c->Y_numerator, c->Y_denominator,
                c->PlaceMargin );
}
#endif

static void
print_copyconf( const CopyConf *p )
{
    cdbg2(  "%s brightness=%d quality=%d num=%d resize=%d papertype=%d papersize=%d\n",
                __FUNCTION__,
                p->copyBrightness, p->copyQuality, p->copyNumber, p->copyResize,
                p->printPaperType, p->printPaperSize );
}

#ifdef HAVE_CMD
void print_copy_config_settings(bool readFromNvram)
{
    CopyConf copy_config;

    if(readFromNvram)
    {
	cdbg1( "nvram copy_config:\n");
	copyapp_get_copy_config(&copy_config);
    }
    else
    {
	cdbg1( "job copy_config:\n");
	copyapp_get_copyjob_config(&copy_config);
    }

    cdbg1( "  brightness=%d\n", copy_config.copyBrightness);
    cdbg1( "  contrast=%d\n", copy_config.copyContrast);
    cdbg1( "  sharp=%d\n", copy_config.copySharp);
    cdbg1( "  background=%d\n", copy_config.copyBackground);
    cdbg1( "  resize=%d (Org=0, A42Ltr=1, Ltr2A4=2, Full=3, 2up=4, 4up=5, Cust=6, Lgl2Ltr=9, Lgl2A4=10)\n", copy_config.copyResize);
    cdbg1( "  resize percentage=%d\n", copy_config.ResizePercentage);
    cdbg1( "  copy number=%d\n", copy_config.copyNumber);
    cdbg1( "  tray=%d\n", copy_config.inputTray);
    cdbg1( "  print quality=%d (draft=0, normal=1, better=2, best=3)\n", copy_config.printQuality);
    cdbg1( "  copy quality=%d (text=0, draft=1, mixed=2, film=3, picture=4)\n", 
	             copy_config.copyQuality);
    cdbg1( "  colormode=%d (1=mono, 2=color)\n", copy_config.ccmode);
    cdbg1( "  color balance (tint) r=%d, g=%d, b=%d\n", copy_config.redAdjust, 
	              copy_config.greenAdjust, copy_config.blueAdjust);
    cdbg1( "  paper size=%d (any=0, letter=1, A4=9, Legal=5)\n", copy_config.printPaperSize);
    cdbg1( "  collate=%d\n", copy_config.copyCollation);
    cdbg1( "  scan duplex=%d\n", copy_config.scanDuplex);
    cdbg1( "  print duplex=%d\n", copy_config.printDuplex);
    cdbg1( "  print borderless=%d\n", copy_config.printBorderless);
    cdbg1( "  2up orientation=%d (portrait=0, landscape=1)\n", copy_config.Orientation2p);
    cdbg1( "  4up orientation=%d\n", copy_config.Orientation4p);
	cdbg1( "  scan size code=%d\n", copy_config.scanPaperSize);
    cdbg1( "  print paper type=%d\n", copy_config.printPaperType);
    cdbg1( "  print size code=%d\n", copy_config.printPaperSize);
    cdbg1( "  scan region=(%d,%d) w:%d, h:%d\n", copy_config.region.x, copy_config.region.y,
	              copy_config.region.width, copy_config.region.height);
    cdbg1( "  scan resolution = %d, copy_resolution_x = %d, copy_resolution_y = %d\n", 
           copy_config.scan_resolution, copy_config.copy_resolution_x, copy_config.copy_resolution_y);
}
#endif

static BOOL adf_paper_is_present( void )
{
    struct scan_platform_capabilities scan_capabilities;

    scancap_get_capabilities( &scan_capabilities );
    if( !scan_capabilities.has_adf ) {
        return FALSE;
    }

    /* ballen 10/4 -- make this read directly from the sensor */
    return adf_sensor_paper_present();
}

void set_copylog_level(int n)
{
    copylog_level = n;
}


/**
 * \brief  Intialize copy config settings 
 *
 * \author Dave Watkins
 * \date 28-Nov-2011
 */
void copyapp_init_settings(CopyConf *copy_config)
{
    mediacnfg_t traycnfg;

    /* set to some sane defaults */
    cdbg2( "%s\n", __FUNCTION__);

    copy_config->copyBackground = 6;
    copy_config->copyBrightness = 6;
    copy_config->copyContrast = 6;
    copy_config->copySharp = 6;
    #ifdef COLOR_ENG
    copy_config->ccmode = COPY_CMODE_COLOR; 
    #else
    copy_config->ccmode = COPY_CMODE_MONO;
    #endif
    copy_config->copyQuality = e_MIXED;
    copy_config->printQuality = PRINTQUALITY_NORMAL;
    copy_config->copyResize = e_ONE_NORMAL;
    copy_config->ResizePercentage = 100;
    copy_config->copyNumber = 1;
    copy_config->inputTray = TRAY_ANY;
    copy_config->printQuality = PRINTQUALITY_NORMAL;
    copy_config->Orientation2p = e_PORTRAIT;
    copy_config->Orientation4p = e_PORTRAIT;
    copy_config->printPaperType = MEDIATYPE_ANY;
    copy_config->printPaperSize = MEDIASIZE_ANY;
    copy_config->scanPaperSize = MEDIASIZE_ANY;
    copy_config->copyCollation = FALSE;
    copy_config->printDuplex = FALSE;
    copy_config->scanDuplex = FALSE;
    copy_config->printBorderless = FALSE;
    copy_config->region.x = 0;
    copy_config->region.y = 0;
    copy_config->region.width = 0;
    copy_config->region.height = 0;
    copy_config->redAdjust = 6;
    copy_config->greenAdjust = 6;
    copy_config->blueAdjust = 6;
    copy_config->copy_resolution_x = COPY_RESOLUTION;
    copy_config->copy_resolution_y = COPY_RESOLUTION;
    copy_config->scan_resolution   = COPY_RESOLUTION;
    copy_config->darkness = 6;

    printvar_get_default_papersize(&(copy_config->printPaperSize));
    if(printvar_get_trayconfig(&traycnfg) == OK)
    {
       copy_config->printPaperType = MEDIATYPE_ANY;//traycnfg.mediaType[0];
    }
}

#ifdef HAVE_NVRAM
error_type_t copyapp_NVInitFunction(nvram_init_type_t InitType, uint16_t Version, void *InitLocation, uint32_t InitLocationSize, void *unused)
{
    cdbg2( "%s\n",__FUNCTION__);

    switch( InitType )
    {
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        case NVRAM_NEW:                /**< New block */
        case NVRAM_ERROR:              /**< Block error */
        case NVRAM_VERSION_CHANGE:     /**< Version change */
 
            copyapp_init_settings(&nvram_shadow_copy_config);

            ASSERT(sizeof(CopyConf) <= InitLocationSize);
            memcpy( InitLocation, &nvram_shadow_copy_config, sizeof(CopyConf));
            break;

        default:
            ASSERT( NVRAM_OK == InitType );
            // read out current values
            memcpy(&nvram_shadow_copy_config, InitLocation, sizeof(CopyConf));
            break;
    }
    return ( OK );
}
#endif

/**
 * \brief  Retrieve nonvolatile copy congif settings from nvram
 *
 * \author Dave Watkins
 * \date 28-Nov-2011
 */

void copyapp_copy_config_access( CopyConf *conf, BOOL getConfig, BOOL useNvram )
{
    static CopyConf localCopyConfig;
    static BOOL copyConfigInitialized=false;

    //cdbg2( "%s get:%d useNvram:%d\n", __FUNCTION__, getConfig, useNvram);

    if(getConfig)   // Read
    {
        if(useNvram || ! copyConfigInitialized)
		{
	    #ifdef HAVE_NVRAM
            mediacnfg_t traycnfg;

	    // set the nvram
		    ASSERT(nvram_handle != NULL);
	    //cdbg2( "%s Get copy of nvram shadow\n", __FUNCTION__);
            memcpy(&localCopyConfig, &nvram_shadow_copy_config, sizeof(CopyConf));
            
            // get the paper size and type
//          printvar_get_default_papersize(&(localCopyConfig.printPaperSize));
            if(printvar_get_trayconfig(&traycnfg) == OK)
            {
               localCopyConfig.printPaperType = MEDIATYPE_ANY;//traycnfg.mediaType[0];
            }
	    #else
		    if(! copyConfigInitialized) copyapp_init_settings(&localCopyConfig);
	    #endif
	    	copyConfigInitialized = true;
		}
		memcpy(conf, &localCopyConfig, sizeof(CopyConf));

		return;
    }
    else  // must be a setConfig  (Write)
    {
		// copy settings into local storage
		memcpy(&localCopyConfig, conf, sizeof(CopyConf));
		copyConfigInitialized = true;

        if(useNvram)
		{
	    #ifdef HAVE_NVRAM
		    // set the nvram
		    ASSERT(nvram_handle != NULL);
		    //cdbg2( "%s Save to nvram\n", __FUNCTION__);
		    nvram_set_var(nvram_handle, conf);
    	    // Keep the nvram shadow up to date with what is being stored in nvram
            memcpy(&nvram_shadow_copy_config, conf, sizeof(CopyConf));
	    #endif
		}
    }
}

void copyapp_init_copy_config( void )
{
    CopyConf copy_config;
    cdbg2( "%s\n", __FUNCTION__);
    memset( &copy_config, 0, sizeof(CopyConf) );
    #ifdef HAVE_NVRAM
    nvram_variable_register(&nvram_handle, COPYDOC_VARS_ID, COPYAPP_VARS_VERSION, 
                            sizeof(CopyConf), copyapp_NVInitFunction, NULL);
    ASSERT(nvram_handle != NULL);
    #else
    // initialize local settings
    copyapp_copy_config_access(&copy_config, true, false);
    #endif

    // if this is not a color engine force color mode to mono
    #ifndef COLOR_ENG
    copyapp_set_fp_colormode( COPY_CMODE_MONO );
    #endif
}

UINT8 copyapp_check_param_valid(stCopy_GoCopy stGoCopy)
{
	uint32_t width, height;
	
	if (stGoCopy.action >= COPY_ACTION_MAX)						return e_COPY_VALID_FAIL_ACTION;
	if (stGoCopy.scanResolution >= e_SCANQUALITY_MAX)			return e_COPY_VALID_FAIL_SCANRESOLUTION;
	if (stGoCopy.collation >= e_COPY_FUNCTION_ON_OFF_MAX )		return e_COPY_VALID_FAIL_COLLATION;
	if (stGoCopy.numberOfCopies > COPY_MAX_NUM_OF_COPY )		return e_COPY_VALID_FAIL_NUMOFCOPIES;
	if (stGoCopy.copyQuality > e_COPYQUALITY_MAX )				return e_COPY_VALID_FAIL_COPYQUALITY;
	
	if (stGoCopy.scanDuplex >= e_COPY_FUNCTION_ON_OFF_MAX )		return e_COPY_VALID_FAIL_SCANDUPLEX;
	if (stGoCopy.printDuplex >= e_COPY_FUNCTION_ON_OFF_MAX )	return e_COPY_VALID_FAIL_PRINTDUPLEX;

	if (stGoCopy.brightness > COPY_BRIGHTNESS_MAX )				return e_COPY_VALID_FAIL_BRIGHTNESS;
	if (stGoCopy.copyBackground >= e_COPY_FUNCTION_ON_OFF_MAX )	return e_COPY_VALID_FAIL_BACKGROUNDREMOVE;

	if (stGoCopy.copyResize >= e_A3_MERGE )						return e_COPY_VALID_FAIL_COPYRESIZE;
	
	if (stGoCopy.scale != COPY_SCALE_AUTO )
	{
		if (stGoCopy.scale < COPY_SCALE_MIN )					return e_COPY_VALID_FAIL_SCALE_MIN;
		if (stGoCopy.scale > COPY_SCALE_MAX )					return e_COPY_VALID_FAIL_SCALE_MAX;
	}
	
	if (stGoCopy.inputTray != e_COPY_INTRAY_AUTO)
		if (stGoCopy.inputTray >= e_COPY_INTRAY_MAX )			return e_COPY_VALID_FAIL_INPUTTRAY;
	
	if ( (stGoCopy.scale == COPY_SCALE_AUTO ) && (stGoCopy.inputTray == e_COPY_INTRAY_AUTO))
		return e_COPY_VALID_FAIL_AUTOPARAM;
	
	if (stGoCopy.printBordering > COPY_BORDERING_MAX )			return e_COPY_VALID_FAIL_PRINTBORDRING;

	if ((paper_get_media_size(stGoCopy.scanPaperSize, &width, &height)) != OK)
		return e_COPY_VALID_FAIL_SCANPAPERSIZE;

#if 1
	if (stGoCopy.inputTray != e_COPY_INTRAY_AUTO)
	{
		if ((paper_get_media_size(stGoCopy.trayPaperInfo[stGoCopy.inputTray], &width, &height)) != OK)
			return (e_COPY_VALID_FAIL_MPTPAPERSIZE+stGoCopy.inputTray);
	}
#else
	if (stGoCopy.inputTray == e_COPY_INTRAY_AUTO)
	{
		uint8_t nLoop;
		for (nLoop=e_COPY_INTRAY_MPT; nLoop<e_COPY_INTRAY_MAX; nLoop++)
			if ((paper_get_media_size(stGoCopy.trayPaperInfo[nLoop], &width, &height)) != OK)
				if (stGoCopy.trayPaperInfo[nLoop]!=0xFFFF)
					return (e_COPY_VALID_FAIL_MPTPAPERSIZE+nLoop);
	}
	else
	{
		if ((paper_get_media_size(stGoCopy.trayPaperInfo[stGoCopy.inputTray], &width, &height)) != OK)
			return (e_COPY_VALID_FAIL_MPTPAPERSIZE+stGoCopy.inputTray);
	}
#endif

//	if (((stGoCopy.scanPaperSize == MEDIASIZE_LEGAL) || (stGoCopy.scanPaperSize ==MEDIASIZE_CUSTOM_85x13)) && (!adf_paper_is_present()))
	if (!adf_paper_is_present())
	{
		if (stGoCopy.scanPaperSize == MEDIASIZE_CUSTOM_85x13)
		{
			return e_COPY_VALID_FAIL_FB_PAPER;
		}
		else if ( (stGoCopy.scanPaperSize == MEDIASIZE_LEGAL) && ((stGoCopy.inputTray == e_COPY_INTRAY_AUTO)) )
		{
			return e_COPY_VALID_FAIL_SELECT_TRAY;
		}
	}
#if 0
	stGoCopy.printPaperType
#endif	
	return e_COPY_VALID_OK;
}

UINT8 copyapp_check_nUP_valid(UINT16 scanPaperSize, UINT16 printPaperSize)
{
    ENGMEDIASIZES scan;
    ENGMEDIASIZES print;
    
	paper_get_media_size(scanPaperSize, &scan.width, &scan.height);
    paper_get_media_size(printPaperSize, &print.width, &print.height);
	
	if ((scan.width < scan.height) && (print.width <print.height))
		return e_COPY_VALID_OK;
	else
		return e_COPY_VALID_FAIL_NUP_PAPER;
}

BOOL copyapp_check_adf_paper(void)
{
	return adf_paper_is_present();
}

void copyapp_get_copyjob_config( CopyConf *conf )
{
    // get copyapp config from static ram
    copyapp_copy_config_access( conf, true, false );
}

void copyapp_get_copy_config( CopyConf *conf )
{
    // get copyapp config from nvram
    copyapp_copy_config_access( conf, true, true );
}

void copyapp_set_copyjob_config( CopyConf *conf )
{
    // set copyapp config into static ram
    copyapp_copy_config_access( conf, false, false );
}

void copyapp_set_copy_config( CopyConf *conf )
{
    // set copyapp config into nvram
    copyapp_copy_config_access( conf, false, true );
}

void copyapp_set_copy_config_defaults( )
{
    CopyConf copy_config;
    copyapp_init_settings(&copy_config);
    copyapp_set_copy_config(&copy_config);
}

/******************************************************************************
 * return value
 *  0 - 3 : TRAY info
 *  255 : failed to find out paper size
 *
 * spec :	A4 => 100-88:A4,	87-72:B5,	71- :A5
 *       	B5 => 115-101:A4,	101-83:B5,	82- :A5
 *			A5 => 141-123:A4,	122-101:B5,	100-:A5
 *			LG => 100-79:LG,	78-61:LT,	60- :HLT 
 *			LT => 				100-66:LT,	65- :HLT
 *			HLT=> 155-129:LG,	128-101:LT,	100-:HLT
 *****************************************************************************/
UINT8 copyapp_find_tray(UINT16 doc_code, UINT16 *tray_code, UINT8 scale, ROTATE_DIR *rot, UINT16 *paper_code)
{
	int i = 0;
	int tray = 0;
	unsigned char *search = NULL;
	unsigned char code;
	unsigned char rotation = 0;
	unsigned char found = 0;
	int media_code;
	int width, height, direction;
	extern int g_nDefaultTray;
	int default_tray = g_nDefaultTray;
	
//	result->tray_num = default_tray;
//	result->paper_code = tray_code[default_tray];
	*paper_code = tray_code[default_tray];
	*rot = e_NoRotate;

//	printf("doc_code: %d, scale : %d\n", doc_code, scale);

	if ( scale == 100 ) {
		*paper_code = doc_code;
		if ( default_tray > TRAY_MAX ) default_tray = TRAY1;

		for ( i = 0; i <= TRAY_MAX ; i ++){
			if ( i == 0 ) tray = default_tray;
			else if ( i == TRAY_MAX ) tray = TRAY_MPT;
			else tray = i;

			if ( doc_code == tray_code[tray] ) {
//				result->tray_num = tray;
//				result->paper_code = doc_code;
//				*paper_code = doc_code;
				*rot = e_NoRotate;
				return tray;
			}
		}
	}

	const unsigned char searchAB[] = {
		CODE_A4, CODE_B5, CODE_A5, /*CODE_B6, CODE_A6,*/ CODE_A5LEF, CODE_NO_PAPER
	};

	const unsigned char searchLT[] = {
		CODE_LG, CODE_LT, CODE_HLT, CODE_NO_PAPER
	};

	for ( code = 0 ; code < CODE_NO_PAPER ; code++ ) {
		if( doc_code == paper[code].code ) {
			break;
		}
		else if( doc_code == paper[code].codeLEF ) {
			rotation = ON;
			break;
		}
	}

	if ( code == CODE_LG || code == CODE_LT || code == CODE_HLT ) {
		search = (unsigned char *)searchLT;
	}
	else {
		for ( i = 0; searchAB[i] != CODE_NO_PAPER ; i++ ) {
			if ( searchAB[i] == code ) {
				search = (unsigned char *)searchAB;
			}
		}
	}
	if ( search == NULL ) {
		return e_COPY_VALID_FAIL_FIND_TRAY;
	}
	if ( code == CODE_A4 || code == CODE_B5 || code == CODE_A5 ) {
		width  = (paper[code].width  * (scale * 10 - 5) )/1000;
		height = (paper[code].height * (scale * 10 - 5) )/1000;
	}
	else {
		width  = paper[code].width  * scale / 100;
		height = paper[code].height * scale / 100;
	}

	for ( i = 0 ; search[i] != CODE_NO_PAPER ; i++ ) {
//		printf("i = %d, search[i]: %d\n", i, search[i]);
//		printf("width %d <= %d\n", width,  paper[search[i]].width );
//		printf("width %d <= %d\n", height, paper[search[i]].height );
		if ( search[i] < CODE_LEF		     &&
				width <= paper[search[i]].width &&
				height <= paper[search[i]].height ) {
//			printf("found\n");
			found = ON;
		}
		else {
			break;
		}
	}

	if ( found == ON ) {
		code = search[i-1];
		direction = rotation;

		for ( i = 0; i <= TRAY_MAX ; i ++){
			if ( i == 0 ) tray = default_tray;
			else if ( i == TRAY_MAX ) tray = TRAY_MPT;
			else tray = i;

			media_code = -1;
			if ( rotation == ON ) {
				if ( paper[code].codeLEF != 0 ) {
					media_code = paper[code].codeLEF;
					direction = OFF;
				}
			}
			else {
				media_code = paper[code].code;
			}
			if ( media_code != -1 && media_code == tray_code[tray] ) {
//				result->tray_num = tray;
//				result->paper_code = media_code;
				*paper_code = media_code;
				*rot = direction;
				return tray;
			}
		}

		direction = rotation;
		for ( i = 0; i <= TRAY_MAX ; i ++){
			if ( i == 0 ) tray = default_tray;
			else if ( i == TRAY_MAX ) tray = TRAY_MPT;
			else tray = i;

			media_code = -1;
			if ( rotation == ON ) {
				media_code = paper[code].code;
			}
			else {
				if ( paper[code].codeLEF != 0 ) {
					media_code = paper[code].codeLEF;
					direction = ON;
				}
			}
			if ( media_code != -1 && media_code == tray_code[tray] ) {
//				result->tray_num = tray;
//				result->paper_code = media_code;
				*paper_code = media_code;
				*rot = direction;
				return tray;
			}
		}

//		result->tray_num = default_tray;
		if ( rotation == ON && paper[code].codeLEF != 0 ) {
			media_code = paper[code].codeLEF;
			rotation = OFF;
		}
		else {
			media_code = paper[code].code;
		}
		*paper_code = media_code;
		*rot = rotation;
		return e_COPY_VALID_FAIL_FIND_TRAY;

	}
	else {
		return e_COPY_VALID_FAIL_FIND_TRAY;
	}
}

UINT8 copyapp_find_tray_A4_LT_LG(UINT16 *tray_code, UINT8 scope)
{
	int nLoop, nLoop2, default_tray, search_cnt=3;
	UINT16 search_order[3] = {MEDIASIZE_A4, MEDIASIZE_LETTER, MEDIASIZE_LEGAL};
	error_type_t db_retval = FAIL;

	switch(scope)
	{
		case e_COPY_FIND_TRAY_SCOPE_A4 : 
			search_order[0] = MEDIASIZE_A4;
			search_cnt = 1;
		break;
		case e_COPY_FIND_TRAY_SCOPE_A4_LT :
			search_order[0] = MEDIASIZE_A4;
			search_order[1] = MEDIASIZE_LETTER;
			search_cnt = 2;
		break;
		case e_COPY_FIND_TRAY_SCOPE_A4_LT_LG :
			search_order[0] = MEDIASIZE_A4;
			search_order[1] = MEDIASIZE_LETTER;
			search_order[2] = MEDIASIZE_LEGAL;
			search_cnt = 3;
		break;
		case e_COPY_FIND_TRAY_SCOPE_LT :
			search_order[0] = MEDIASIZE_LETTER;
			search_cnt = 1;
		break;
		case e_COPY_FIND_TRAY_SCOPE_LT_A4 :
			search_order[0] = MEDIASIZE_LETTER;
			search_order[1] = MEDIASIZE_A4;
			search_cnt = 2;
		break;
		case e_COPY_FIND_TRAY_SCOPE_LT_LG_A4 :
			search_order[0] = MEDIASIZE_LETTER;
			search_order[1] = MEDIASIZE_LEGAL;
			search_order[2] = MEDIASIZE_A4;
			search_cnt = 3;
		break;
		case e_COPY_FIND_TRAY_SCOPE_LG_LT_A4 :
			search_order[0] = MEDIASIZE_LEGAL;
			search_order[1] = MEDIASIZE_LETTER;
			search_order[2] = MEDIASIZE_A4;
			search_cnt = 3;
		break;
		default:
		break;
	}
	
	default_tray = db_get_var_int("CONF_COPYSET", "PrintSrcTray", &db_retval);
	for (nLoop=0; nLoop<search_cnt; nLoop++)
	{
		if ((db_retval != FAIL) && (tray_code[default_tray] == search_order[nLoop]))
			return (UINT8)default_tray;
	}
	
	for (nLoop=0; nLoop<search_cnt; nLoop++)
	{
		for (nLoop2=TRAY1; nLoop2<TRAY_MAX; nLoop2++)
		{
			if (tray_code[nLoop2] == search_order[nLoop])
				break;
		}
		
		if (nLoop2<TRAY_MAX)
			return nLoop2;
		
		if (tray_code[TRAY_MPT] == search_order[nLoop])
			return TRAY_MPT;
	}
	
	return e_COPY_VALID_FAIL_FIND_TRAY;
}

UINT8 copyapp_find_scale(UINT16 scanPaperSize, UINT16 printPaperSize, ROTATE_DIR *rot)
{
    ENGMEDIASIZES scan;
    ENGMEDIASIZES print;
    UINT8	retScale;
    
	paper_get_media_size(scanPaperSize, &scan.width, &scan.height);
    paper_get_media_size(printPaperSize, &print.width, &print.height);

    cdbg1("find scale : scan(w%d, h%d), print(w%d, h%d)\n", scan.width, scan.height, print.width, print.height);
    
	if (((scan.width > scan.height) && ( print.width > print.height)) ||
		((scan.width < scan.height) && ( print.width < print.height)))
	{
		*rot = e_NoRotate;
		retScale = ((print.width*100 / scan.width) > (print.height*100 / scan.height)) ?
		(print.height*100 / scan.height) : (print.width*100 / scan.width);
	}
	else
	{
		*rot = e_Rotate90;
		retScale = ((print.width*100 / scan.height) > (print.height*100 / scan.width)) ?
		(print.height*100 / scan.width) : (print.width*100 / scan.height);
	}
	
	if ((retScale > 45) && (retScale < 220))
		return retScale;
	else
		return COPY_SCALE_ERROR;
}

UINT8 copyapp_find_scale_NUP(UINT8 nUP, UINT16 scanPaperSize, UINT16 printPaperSize, ROTATE_DIR *rot)
{
    ENGMEDIASIZES scan;
    ENGMEDIASIZES print;
    UINT8	retScale;
    
	paper_get_media_size(scanPaperSize, &scan.width, &scan.height);
    paper_get_media_size(printPaperSize, &print.width, &print.height);

    cdbg1("find scale : scan(w%d, h%d), print(w%d, h%d)\n", scan.width, scan.height, print.width, print.height);

	if (nUP == e_TWO_PAGES_PER_SHEET)
	{
		if (((scan.width > scan.height) && ( print.width > print.height)) ||		//LEF->LEF, SEF->SEF
			((scan.width < scan.height) && ( print.width < print.height)))
		{
			*rot = e_Rotate90;
			if ( print.width > print.height)				// print LEF
				retScale = ((print.width*50 / scan.height) > (print.height*100 / scan.width)) ?
					(print.height*100 / scan.width) : (print.width*50 / scan.height);
			else											// print SEF
				retScale = ((print.width*100 / scan.height) > (print.height*50 / scan.width)) ?
					(print.height*50 / scan.width) : (print.width*100 / scan.height);
		}
		else																		// LEF->SEF, SEF->LEF
		{
			*rot = e_NoRotate;
			if ( print.width > print.height)				// print LEF
				retScale = ((print.width*50 / scan.width) > (print.height*100 / scan.height)) ?
					(print.height*100 / scan.height) : (print.width*50 / scan.width);
			else											// print SEF
				retScale = ((print.width*100 / scan.width) > (print.height*50 / scan.height)) ?
					(print.height*50 / scan.height) : (print.width*100 / scan.width);
		}
	}
	else if (nUP == e_FOUR_PAGES_PER_SHEET)
	{
		if (((scan.width > scan.height) && ( print.width > print.height)) ||		//LEF->LEF, SEF->SEF
			((scan.width < scan.height) && ( print.width < print.height)))
		{
			*rot = e_NoRotate;
			retScale = ((print.width*50 / scan.width) > (print.height*50 / scan.height)) ?
			(print.height*50 / scan.height) : (print.width*50 / scan.width);
		}
		else																		// LEF->SEF, SEF->LEF
		{
			*rot = e_Rotate90;
			retScale = ((print.width*50 / scan.height) > (print.height*50 / scan.width)) ?
				(print.height*50 / scan.width) : (print.width*50 / scan.height);
		}
	}
	else
	{
		return COPY_SCALE_ERROR;
	}
/*
	if ((retScale > 45) && (retScale < 220))
		return retScale;
	else
		return COPY_SCALE_ERROR;
*/
	return retScale;
}

void copyapp_set_fp_rotation( ROTATE_DIR copy_rotation )
{
	cdbg1("set rotation %d\n", rotation);
	rotation = copy_rotation;
}

/**
 * \brief  Change the front panel settings for copy resize
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 * \author David Poole
 * \date 14-Sep-2006
 *
 */

void
copyapp_set_fp_resize( e_COPYRESIZE resize_var, uint32_t custom_resize )
{
    CopyConf copy_config;

    memset( &copy_config, 0, sizeof(copy_config) );

#if 0
    if( resize_var != e_CUSTOM_PCNT ) {
        /* if we don't have a custom resize, force the size to 100 just 
         * in case
         */
        custom_resize = 100;
    }
#endif
    copyapp_get_copy_config(&copy_config);
    copy_config.copyResize = resize_var;
    copy_config.ResizePercentage = custom_resize;
    copyapp_set_copy_config(&copy_config);

    cdbg1(  "copy scale resize=%d, custom resize percentage=%ld\n", resize_var, custom_resize );
}

/**
 * \brief  Change the front panel settings for color vs mono
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 * \author David Poole
 * \date 13-Sep-2006
 *
 */

void
copyapp_set_fp_colormode( copy_cmode_t ccmode )
{
    CopyConf copy_config;

    memset( &copy_config, 0, sizeof(copy_config) );

    XASSERT( ccmode==COPY_CMODE_MONO || ccmode==COPY_CMODE_COLOR, ccmode );

    copyapp_get_copy_config(&copy_config);
    copy_config.ccmode = ccmode;
    copyapp_set_copy_config(&copy_config);

    cdbg1(  "copy mode set to %d\n", ccmode );
}

/**
 * \brief  Change the front panel settings for copy quality
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 * \author David Poole
 * \date 13-Sep-2006
 *
 */

void
copyapp_set_fp_qualitymode( e_COPYQUALITY quality )
{
    CopyConf copy_config;

    memset( &copy_config, 0, sizeof(copy_config) );

    XASSERT( quality<e_COPYQUALITY_MAX, quality );

    copyapp_get_copy_config(&copy_config);
    copy_config.copyQuality = quality;
    copyapp_set_copy_config(&copy_config);

    cdbg1(  "copy quality set to %d\n", quality );
}

void
copyapp_set_fp_quality( e_COPYQUALITY quality, copy_cmode_t ccmode )
{
    copyapp_set_fp_qualitymode(quality);
    copyapp_set_fp_colormode(ccmode);
}

/**
 * \brief  Change the front panel settings for copy n-up orientation
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 * \author David Poole
 * \date 03-Aug-2007
 *
 */

void
copyapp_set_fp_orientation( e_ORIENTATION orientation )
{
    CopyConf copy_config;

    memset( &copy_config, 0, sizeof(copy_config) );

    /* cheap, lazy hack - set both 2-up and 4-up to the same orientation */

    copyapp_get_copy_config(&copy_config);
    copy_config.Orientation2p = orientation;
    copy_config.Orientation4p = orientation;
    copyapp_set_copy_config(&copy_config);

    cdbg1(  "copy orientation set to %d\n", orientation );
}

/**
 * \brief  Change the front panel settings for copy's number of copies
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 * \author David Poole
 * \date 20-Feb-2009
 *
 */

void copyapp_set_fp_num_copies( int num_copies )
{
    CopyConf copy_config;

    if( num_copies < 1 || num_copies > 99 ) {
        /* ignore */
        return;
    }

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.copyNumber = num_copies;
    copyapp_set_copy_config(&copy_config);    
}

/**
 * \brief  Change the front panel settings for copy's brightness
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void copyapp_set_fp_brightness( int brightness )
{
    CopyConf copy_config;

    if( brightness < COPY_BRIGHTNESS_MIN || brightness > COPY_BRIGHTNESS_MAX ) {
        /* ignore */
        return;
    }

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.copyBrightness = brightness;
    copyapp_set_copy_config(&copy_config);
}

/**
 * \brief  Change the front panel settings for copy's background
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */
void copyapp_set_fp_background( int background )
{
    CopyConf copy_config;

    if( background <  COPY_BACKGROUND_MIN || background >  COPY_BACKGROUND_MAX )
    {
        /* ignore */
	return;
    }

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.copyBackground = background;
    copyapp_set_copy_config(&copy_config);    
}

/**
 * \brief  Change the front panel settings for copy's contrast
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void copyapp_set_fp_contrast( int contrast )
{
    CopyConf copy_config;

    if( contrast < COPY_CONTRAST_MIN || contrast > COPY_CONTRAST_MAX ) {
        /* ignore */
        return;
    }

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.copyContrast = contrast; 
    copyapp_set_copy_config(&copy_config);    
}

/**
 * \brief  Change the front panel settings for copy's sharpness
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void copyapp_set_fp_sharpness( int sharpness )
{
    CopyConf copy_config;

    if( sharpness < COPY_SHARP_MIN || sharpness > COPY_SHARP_MAX ) {
        /* ignore */
        return;
    }

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.copySharp = sharpness; 
    copyapp_set_copy_config(&copy_config);    
}

/**
 * \brief  Change the front panel settings for copy's RGB colors
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void copyapp_set_fp_color_balance( int r, int g, int b )
{
    CopyConf copy_config;

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.redAdjust = r;
    copy_config.greenAdjust = g;
    copy_config.blueAdjust = b;
    copyapp_set_copy_config(&copy_config);    
}

/**
 * \brief  Change the front panel settings for copy's print quality
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void copyapp_set_fp_print_quality( PRINT_QUALITY print_quality )
{
    CopyConf copy_config;

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.printQuality = print_quality;
    copyapp_set_copy_config(&copy_config);    
}

/**
 * \brief  Change the front panel settings for copy's input tray
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void copyapp_set_fp_input_tray( trayenum_t input_tray )
{
    CopyConf copy_config;

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.inputTray = input_tray;
    copyapp_set_copy_config(&copy_config);    
}

/**
 * \brief  Change the front panel settings for copy collation enable/disable
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void
copyapp_set_fp_collation( BOOL enable )
{
    CopyConf copy_config;

    cdbg2(  "%s enable=%d\n", __FUNCTION__, enable );

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.copyCollation = enable;
    copyapp_set_copy_config(&copy_config);    
} 

/**
 * \brief  Change the front panel settings for copy print auto duplex
 * enable/disable
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void
copyapp_set_fp_print_duplex( BOOL enable )
{
    CopyConf copy_config;

    cdbg2(  "%s enable=%d\n", __FUNCTION__, enable );
    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
	if (enable)
	    copy_config.printDuplex = DMDUP_HORIZONTAL;
	else
	    copy_config.printDuplex = DMDUP_SIMPLEX;
    copyapp_set_copy_config(&copy_config);    
} 

/**
 * \brief  Change the front panel settings for copy print resolution. 
 * Currently resolutions of 300 and 600 are supported. 
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void
copyapp_set_fp_resolution( int dpi )
{
    CopyConf copy_config;

    if (dpi == 300 || dpi == 600)
    {
        cdbg2(  "%s = %d\n", __FUNCTION__, dpi );
    }
    else
    {
        cdbg2(  "%s = %d FAILED; supported resolutions are 300 and 600\n", __FUNCTION__, dpi );
        return;
    }

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.scan_resolution = dpi;
    copy_config.copy_resolution_x = dpi;
    copy_config.copy_resolution_y = dpi;
    copyapp_set_copy_config(&copy_config);    
}

/**
 * \brief  Change the front panel settings for copy scan auto duplex
 * enable/disable
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 */

void
copyapp_set_fp_scan_duplex( BOOL enable )
{
    CopyConf copy_config;

    cdbg2(  "%s enable=%d\n", __FUNCTION__, enable );

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.scanDuplex = enable;
    copyapp_set_copy_config(&copy_config);    
} 

void
copyapp_set_fp_print_paper_size( mediasize_t papersize )
{
    CopyConf copy_config;

    cdbg2(  "%s print paper size=%d\n", __FUNCTION__, papersize );

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.printPaperSize = papersize;
    copyapp_set_copy_config(&copy_config);
} 

void
copyapp_set_fp_print_paper_size_info( UINT16 *tray_code )
{
    CopyConf copy_config;

    memset( &copy_config, 0, sizeof(copy_config) );

	copyapp_get_copy_config(&copy_config);
    copy_config.printTrayPaperInfo[TRAY_MPT] = tray_code[TRAY_MPT];
    copy_config.printTrayPaperInfo[TRAY1] = tray_code[TRAY1];
    copy_config.printTrayPaperInfo[TRAY2] = tray_code[TRAY2];
    copy_config.printTrayPaperInfo[TRAY3] = tray_code[TRAY3];
    copyapp_set_copy_config(&copy_config);
} 

void
copyapp_set_fp_scan_paper_size( mediasize_t papersize )
{
    CopyConf copy_config;

    cdbg2(  "%s scan paper size=%d\n", __FUNCTION__, papersize );

    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.scanPaperSize = papersize;
    copyapp_set_copy_config(&copy_config);
} 

void
copyapp_set_fp_print_density( int density )
{
    CopyConf copy_config;
    cdbg2(  "%s print density=%d\n", __FUNCTION__, density );
 
    memset( &copy_config, 0, sizeof(copy_config) );

    copyapp_get_copy_config(&copy_config);
    copy_config.darkness = density;
    copyapp_set_copy_config(&copy_config);
}

void copyapp_set_id_copy(bool val)
{
	idCopyEnable = val;
}

#ifdef HAVE_ID_COPY
/**
 * \brief set ID copy pending flag and start a copy job
 *
 *
 * \author Dave Watkins
 * \date 19-Dec-2011
 *
 */

void copyapp_id_copy_start_job( void )
{
    cdbg2("%s\n", __FUNCTION__);
//    g_page_info.nup_image_params.ID_nup = IDNUP_ACTIVE;
//    idCopyPending = TRUE;
	idCopyEnable = TRUE;

    print_page_info( &g_page_info );
    copyapp_start_job( );
}

/**
 * \brief send message to copy task indicating how to continue 
 *        with ID copy. 
 *
 *
 * \author Dave Watkins
 * \date 19-Dec-2011
 *
 */
void copyapp_id_copy_input( uint32_t action )
{
   MESSAGE TempMsg;
   cdbg2("%s\n", __FUNCTION__);

//   TempMsg.msgType = MSG_COPY_ID_COPY_INPUT;
	TempMsg.msgType = MSG_USER_INPUT;

   TempMsg.param1 = 0;
   TempMsg.param2 = 0;
   TempMsg.param3 = 0;

   switch(action)
   {
        case e_COPY_NEXT:
            TempMsg.param1 = PARAM_USER_OK;
            break;
        case e_COPY_FINISH:
            TempMsg.param1 = PARAM_USER_FINISH_JOB;
            break;
        case e_COPY_CANCEL:
            TempMsg.param1 = PARAM_USER_CANCEL_JOB;
            break;
        default:
            return; // ignore, dont send msg
            break;
   }

   (void)SYMsgSend( COPYAPPID, &TempMsg );
}
#endif

/**
 * \brief send a start recipe for a copy job to the system job manager 
 *
 *
 * \author David Poole
 * \date 06-Oct-2005
 *
 */

void copyapp_start_job( void )
{
    MESSAGE msg = {0};
    error_type_t err;
    STARTRECIPE *StartIt;

    cdbg2( "%s\n", __FUNCTION__);

    if( !scanner_is_alive()) {
        cdbg1(  "%s scanner offline hwerr=%d\n", __FUNCTION__, scan_get_hwerr() );
        return;
    }

    StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
    ASSERT(StartIt != NULL);
    // we have a connection, get the resources and start.
    //
    msg.msgType = MSG_STARTRECIPE;
    StartIt->Job = ejob_Copy;
    StartIt->AdditionalResource = 0;
    StartIt->Pipe = 0;
    StartIt->Wait = e_WaitForever;
    msg.param3 = StartIt;       // freed by sjm
  
    /* send a message to the System Job Manager telling it to 
     * kick off a copy
     */
    err = SYMsgSend(SJMID, &msg);         // send the message.
    XASSERT( err==OK, err );
}

/**
 * \brief Log a copy event.
 *
 * RIP Status Manager.  
 *
 * Report a copy event. For example, we're starting a copy, finishing a copy,
 * etc. Originally created to replace the Status Manager. The SM was used to
 * interface with the control panel.
 *
 * This plugs the copy app events into the event_observer.h framework,
 * now anyone can register for copy app events but typically only the
 * user interface would do this.
 *
 */

static void copyapp_log_event( copyapp_event_t evt )
{
    /* do the event observer/notifier thing */
    copyapp_observer_notify_event( evt, number_of_scan_originals );
}

/* FUNCTION NAME: FreeInputBuffer */
 
/** 
 * \brief Free all the data on the linked list passed in.
 * 
 * \param strip_list - the head of the list of data strips to free
 * 
 * \retval None
 * 
 **/
void free_strip_list(ATLISTENTRY* strip_list)
{
    copyapp_data_strip_t* strip;
    ATLISTENTRY* node;

    cdbg2("%s strip_list:%p\n", __FUNCTION__, strip_list);

    while((node = ATRemoveHeadList(strip_list)) != NULL)
    {
        strip = CONTAINING_RECORD(node, copyapp_data_strip_t, node);

        memFree(strip->data);
        /* be paranoid */
        DBG_ZEROMEM(strip, sizeof(copyapp_data_strip_t));
        memFree(strip);
    }
}

void copyapp_copy_input( uint32_t action )
{
    MESSAGE TempMsg;
    error_type_t err;
    scan_err_t scerr = SCANERR_NONE;

    cdbg2("%s action:%d\n", __FUNCTION__, action);

    TempMsg.msgType = MSG_USER_INPUT;
    TempMsg.param1 = 0;
    TempMsg.param2 = 0;
    TempMsg.param3 = 0;

    switch(action)
    {
         case e_COPY_NEXT:
             TempMsg.param1 = PARAM_USER_OK;
             break;
         case e_COPY_FINISH:
             TempMsg.param1 = PARAM_USER_FINISH_JOB;
             break;
         case e_COPY_CANCEL:
			dbg1("[S_CANCEL] step1. pause!! copyapp_copy_input\n");
			if(copy_started_from_adf)
				scerr = scanmech_adf_client_request_pause();	// add.lsh for USER CANCEL
             TempMsg.msgType = MSG_CANCELJOB;
             TempMsg.param1 = SYS_REQUEST;
             TempMsg.param3 = (void*) e_CopyApp;
             err = SYMsgSend( SJMID, &TempMsg );
             XASSERT( err==OK, err ); 
             return;
             break;
         default:
             return; // ignore, dont send msg
             break;
    }

    err = SYMsgSend( COPYAPPID, &TempMsg );
    XASSERT( err==OK, err ); 
}

void copyapp_cancel_job( void )
{
    copyapp_copy_input(e_COPY_CANCEL);
}

void copyapp_continue_job( void )
{
    copyapp_copy_input(e_COPY_NEXT);
}

bool is_idcopy_enabled( void )
{
#ifdef HAVE_ID_COPY
    return idCopyEnable;
#else
    return false;
#endif
}

void copyapp_OOM( void )
{
    MESSAGE msg;
    error_type_t err;

    /* Tell the Print Job Manager about out-of-memory. The PJM will attempt to
     * free memory.
     *
     * As of this writing (09-Sep-2009), only useful during a collated copy and
     * we run low on memory. The PJM will free the saved pages' memory.
     */

    /* inform collation has been abandoned */
    copyapp_log_event( COPYAPP_EVENT_OUT_OF_MEMORY );

    /* pages from this point on will be non-collated */
    g_page_info.copies = 1;
    g_page_info.collate = false;

    /* tell PJM to give up his saved pages' memory */
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_OUT_OF_MEMORY;
    msg.param1 = SYS_REQUSER;
    msg.param2 = 0;
    msg.param3 = (void*) e_CopyApp;
    err = SYMsgSend( JOBMGRID, &msg );
    XASSERT( err==OK, err );
}

struct copy_scanvar_maker {
    /* lookup table keys */
    e_COPYQUALITY quality;
    copy_cmode_t ccmode;

    /* pointer to the scanvar constructor for a given quality+ccmode */
    struct scanvars * (*scanvar_constructor)(void);
};

struct copy_scanvar_maker copy_scanvar_maker_lut[] = {
    { e_DRAFT, COPY_CMODE_MONO, scanplat_sv_draft_copy_new },
    { e_DRAFT, COPY_CMODE_COLOR, scanplat_sv_draft_color_copy_new },

    { e_TEXT, COPY_CMODE_MONO, scanplat_sv_text_copy_new },
    { e_TEXT, COPY_CMODE_COLOR, scanplat_sv_text_color_copy_new },

    { e_MIXED, COPY_CMODE_MONO, scanplat_sv_mixed_copy_new },
    { e_MIXED, COPY_CMODE_COLOR, scanplat_sv_mixed_color_copy_new },

    { e_FILM_PHOTO, COPY_CMODE_MONO, scanplat_sv_filmphoto_copy_new },
    { e_FILM_PHOTO, COPY_CMODE_COLOR, scanplat_sv_filmphoto_color_copy_new },

    { e_PICTURE, COPY_CMODE_MONO, scanplat_sv_picture_copy_new },
    { e_PICTURE, COPY_CMODE_COLOR, scanplat_sv_picture_color_copy_new },

    { e_FAX_TEXT, COPY_CMODE_MONO, scanplat_sv_text_fax_new },
    { e_FAX_MIXED, COPY_CMODE_MONO, scanplat_sv_mixed_fax_new },

    /* end of list marker */
    { 0, 0, NULL },
};

const int num_copy_scanvar_maker_lut_entries = sizeof(copy_scanvar_maker_lut)/sizeof(struct copy_scanvar_maker);

static scan_err_t copy_scanvar_lookup( e_COPYQUALITY quality, copy_cmode_t ccmode, 
                                  struct copy_scanvar_maker **copy_sv_maker )
{
    int i;

    cdbg2(  "%s quality=%d ccmode=%d\n", __FUNCTION__, quality, ccmode );

    for( i=0 ; i<num_copy_scanvar_maker_lut_entries ; i++ ) {
        if( !copy_scanvar_maker_lut[i].scanvar_constructor ) {
            return SCANERR_NO_ENTRY;
        }

        if( copy_scanvar_maker_lut[i].quality==quality &&
            copy_scanvar_maker_lut[i].ccmode==ccmode ) {

            /* found it! return now */
            *copy_sv_maker = &copy_scanvar_maker_lut[i];
            return SCANERR_NONE;
        }

    }
    /* should not get here; your list is broken */
    XASSERT( 0, i );

    return SCANERR_NO_ENTRY;
}
#if 0
static void
increment_copy_scanned_page_count( BOOL adf_copy )
{
    uint32_t numScans=0;

    /* TODO store the counters somewhere / somehow in NV memory */
    if (adf_copy == TRUE) {
//        scands_get_copy_pages_scanned_ADF(&numScans);
        numScans++;
//        scands_set_copy_pages_scanned_ADF(&numScans);
    }
    else {
//        scands_get_copy_pages_scanned_flatbed(&numScans);
        numScans++;
//        scands_set_copy_pages_scanned_flatbed(&numScans);
    }
}

static void
increment_copy_pages_printed( void )
{
    uint32_t     Copies=0;

    /* TODO store the counters somewhere / somehow in NV memory */
//    scands_get_copy_pages_printed(&Copies);
    Copies++;
//    scands_set_copy_pages_printed(&Copies);
}
#endif

/**
 * \brief recalculate the scale for 600x400 text copies
 *
 * We run the engine faster for text mode copies. We have to scale the input
 * data down by 2/3 because the engine will do line replication.
 *
 * We need adjust the copy scale to also take the 2/3 into account.
 *
 * Go go Gadget Fractions!
 *
 * \author David Poole
 * \date 07-Aug-2009
 *
 */
#if 0
static void rescale_for_text_mode( struct scanvars *sv )
{
    struct fraction x, y;
    struct fraction text_mode_scale;
    struct fraction new_scale;
	scan_err_t scerr;
	
    text_mode_scale.numer = 2;
    text_mode_scale.denom = 3;

    scanvar_get_xyscale( sv, &x.numer, &x.denom, &y.numer, &y.denom );

    fraction_multiply( &y, &text_mode_scale, &new_scale );

    cdbg2(  "%s from=%d/%d to=%d/%d\n", __FUNCTION__, y.numer, y.denom,
                new_scale.numer, new_scale.denom );

    scerr = scanvar_set_xyscale( sv, x.numer, x.denom, new_scale.numer, new_scale.denom);
	XASSERT( scerr==SCANERR_NONE, scerr ); 
}
#endif
 
/**
 * \brief Setup the copy parameters necessary for scan.
 *
 * \param FPCopyConfig copy configuration from front panel settings
 * \author David Poole
 * \author Dirk Bodily
 * \date 25-Aug-2005
 *
 */

static scan_err_t 
setup_scan_params( struct scanvars **psv, const CopyConf *FPCopyConfig, PAGEINFO *page_info )
{
    struct scanvars *sv;
    e_COPYRESIZE ResizeVar;
    struct copy_scanvar_maker *copy_sv_maker=NULL;
    scan_err_t scerr;

    cdbg2( "%s\n", __FUNCTION__);

    /* demand incoming pointer be NULL (sanity check for memory leaks) */
    sv = *psv;
    XASSERT( sv==NULL, (uint32_t)sv );

    // save the id nup info
    uint32_t id_nup = page_info->nup_image_params.ID_nup;
    memset( page_info, 0, sizeof( PAGEINFO ) );
    // restore the id nup info
    page_info->nup_image_params.ID_nup = id_nup;

    // Set up values depending on copy quality.
    //
    page_info->TonerSaveMode = FALSE;

    page_info->Y_Resolution = FPCopyConfig->copy_resolution_y;
    page_info->X_Resolution = FPCopyConfig->copy_resolution_x;
    page_info->copies = FPCopyConfig->copyNumber;  // set the number of copies.
    page_info->collate = FPCopyConfig->copyCollation;  // set collate on off.
    page_info->InputTray = FPCopyConfig->inputTray;
    page_info->printQuality = FPCopyConfig->printQuality;
    page_info->duplexInfo = FPCopyConfig->printDuplex;
    page_info->darkness = FPCopyConfig->darkness;
    
	cdbg2("%s, %d\n", __FUNCTION__, FPCopyConfig->printDuplex);

    // Resize variable.
    ResizeVar = FPCopyConfig->copyResize;

    scerr = copy_scanvar_lookup( FPCopyConfig->copyQuality, 
                                  FPCopyConfig->ccmode, &copy_sv_maker );
    XASSERT( scerr==SCANERR_NONE, scerr );

/******** SIQ Tuning Start ***********/
	if (find_siq_name(FPCopyConfig->copyQuality))		// TRUE : success, FALSE : fail
	{
		if(load_siqfile() != OK)		// 0 : SCANERR_NONE, X : ERROR
		{
			dbg1("[SIQ] : load_siqfile false\n");
		}
	}
	else
	{
		dbg1("[SIQ] : find_siq_name false\n");
	}
/******** SIQ Tuning End ***********/
    sv = copy_sv_maker->scanvar_constructor();
    if( sv==NULL ) {
        cdbg1(  "copyapp failed to get scanvars\n" );
        return SCANERR_OUT_OF_MEMORY;
    }

    switch(FPCopyConfig->copyQuality)
    {
        case e_TEXT: // Text
            if( sv==NULL ) {
                cdbg1(  "copyapp failed to get scanvars\n" );
                return SCANERR_OUT_OF_MEMORY;
            }
            //copy_bpp = sv->hp_bpp;
            break;

        case e_DRAFT: // Draft mode.
            cdbg1( "Copy in DRAFT!\n");
            page_info->TonerSaveMode = TRUE;       // turn on toner save for draft mode.
            if( sv==NULL ) {
                cdbg1(  "copyapp failed to get scanvars\n" );
                return SCANERR_OUT_OF_MEMORY;
            }
            //copy_bpp = sv->hp_bpp;
            break;
            
        case e_MIXED: // Mixed mode
            /* set up copy parameters */
            if( sv==NULL ) {
                cdbg1(  "copyapp failed to get scanvars\n" );
                return SCANERR_OUT_OF_MEMORY;
            }
            //copy_bpp = sv->hp_bpp;
            break;

        case e_FILM_PHOTO: // Film photo
            /* set up copy parameters */
            if( sv==NULL ) {
                cdbg1(  "copyapp failed to get scanvars\n" );
                return SCANERR_OUT_OF_MEMORY;
            }
            //copy_bpp = sv->hp_bpp;
            break;

        case e_PICTURE: // picture
            /* set up copy parameters */
            if( sv==NULL ) {
                cdbg1(  "copyapp failed to get scanvars\n" );
                return SCANERR_OUT_OF_MEMORY;
            }
            //copy_bpp = sv->hp_bpp;
            break;
            
        case e_FAX_TEXT :
            /* can't get to this through the control panel, only through
             * copy-to-host
             */
            if( sv==NULL ) {
                cdbg1(  "copyapp failed to get scanvars\n" );
                return SCANERR_OUT_OF_MEMORY;
            }
            //copy_bpp = sv->hp_bpp;
            break;

        case e_FAX_MIXED :
            /* can't get to this through the control panel, only through
             * copy-to-host
             */
            if( sv==NULL ) {
                cdbg1(  "copyapp failed to get scanvars\n" );
                return SCANERR_OUT_OF_MEMORY;
            }
            //copy_bpp = sv->hp_bpp;
            break;

        default:
            XASSERT(0,FPCopyConfig->copyQuality);
            sv = NULL;
            break;
    }

    if( sv==NULL ) {
        cdbg1(  "copyapp failed to get scanvars\n" );
        return SCANERR_OUT_OF_MEMORY;
    }

    #ifdef COLOR_ENG
       // map the 1..11 rgb values to -200..200
       int r, g, b;
       r = ((FPCopyConfig->redAdjust   -1) * 40) -200;
       g = ((FPCopyConfig->greenAdjust -1) * 40) -200;
       b = ((FPCopyConfig->blueAdjust  -1) * 40) -200;
       scanvar_set_rgb_tint(sv, r, g, b);

       // map the 1..11 sharp values to 0..200
       int sh;
       sh = ((FPCopyConfig->copySharp -1) * 20);
       scanvar_set_sharpness(sv, sh );

       // map the 1..11 bkgrnd values to 105..255
       uint16_t w;
       uint16_t bc[3], wc[3];
       w = ((FPCopyConfig->copyBackground -1) * 15) +105;
       bc[0] = bc[1] = bc[2] = 20;
       wc[0] = wc[1] = wc[2] = w;
       scanvar_set_black_white_clip( sv, bc, wc );
    #else
       if (FPCopyConfig->copyBrightness >= COPY_BRIGHTNESS_MIN &&
            FPCopyConfig->copyBrightness <= COPY_BRIGHTNESS_MAX )
           scanvar_set_copy_brightness(sv, FPCopyConfig->copyBrightness );
    #endif
	
	scanvar_set_copy_contrast(sv, FPCopyConfig->copyContrast );
	scanvar_set_background(sv, FPCopyConfig->copyBackground );
	
    if(FPCopyConfig->ccmode == COPY_CMODE_COLOR )
    {
	scanvar_set_dpi( sv, FPCopyConfig->scan_resolution );
	page_info->monoOrColor = e_Color;
	scanvar_set_pieout(sv, PIE_PIXEL_RGB);
	copy_bpp = 24;
    }
    else  // TODO: for now assume all qualities other than e_COLORCOPY are mono
    {
	scanvar_set_dpi( sv, FPCopyConfig->scan_resolution );
	page_info->monoOrColor = e_Mono;
	/* need the bpp so we can tell the job manager how much data to expect */
	copy_bpp = 8;
    }

    page_info->Y_Resolution = FPCopyConfig->copy_resolution_y;
    page_info->X_Resolution = FPCopyConfig->copy_resolution_x;

    if(FPCopyConfig->copyQuality == e_FAX_TEXT ||   
       FPCopyConfig->copyQuality == e_FAX_MIXED )  copy_bpp = 1;

    /* NOW can call scanvar_set_xxx functions */

    CopySettings.X_denominator = 100;
    CopySettings.Y_denominator = 100;
    N_up = 1;

    /* davep 17-Apr-2006 ; (Bug 1158) force page to upper left by adding extra
     * padding to scaler; only want to do this when scaling and not rotating
     */
    sv->pie_pad_to_pixel_width = 0;

    switch(ResizeVar)
    {
        case e_ONE_NORMAL:
            CopySettings.X_numerator = FPCopyConfig->ResizePercentage;
            CopySettings.Y_numerator = FPCopyConfig->ResizePercentage;
	        break;
        case e_TWO_PAGES_PER_SHEET:
			if (idCopyEnable)
			{
				CopySettings.X_numerator = 100;
				CopySettings.Y_numerator = 100;
			}
			else
			{
				CopySettings.X_numerator = copyapp_find_scale_NUP(e_TWO_PAGES_PER_SHEET, FPCopyConfig->scanPaperSize, FPCopyConfig->printPaperSize, &rotation);
				CopySettings.Y_numerator = CopySettings.X_numerator;
			}
            N_up = 2;
            break;
        case e_FOUR_PAGES_PER_SHEET:
            CopySettings.X_numerator = copyapp_find_scale_NUP(e_FOUR_PAGES_PER_SHEET, FPCopyConfig->scanPaperSize, FPCopyConfig->printPaperSize, &rotation);
            CopySettings.Y_numerator = CopySettings.X_numerator;
            N_up = 4;
            break;
		case e_A3_MERGE:
		case e_DLT_MERGE:
            CopySettings.X_numerator = FPCopyConfig->ResizePercentage-1;
            CopySettings.Y_numerator = FPCopyConfig->ResizePercentage-1;
            
            if (rotation)
	            page_info->nup_image_params.ID_nup = IDNUP_A3MERGE_ROTATE;
	        else
	            page_info->nup_image_params.ID_nup = IDNUP_A3MERGE_NOROTATE;

            break;
        case e_A3_SEPARATE:
        case e_DLT_SEPARATE:
            CopySettings.X_numerator = FPCopyConfig->ResizePercentage;
            CopySettings.Y_numerator = FPCopyConfig->ResizePercentage;
            
            if (rotation)
	            page_info->nup_image_params.ID_nup = IDNUP_A3MERGE_ROTATE;
	        else
	            page_info->nup_image_params.ID_nup = IDNUP_A3MERGE_NOROTATE;
	        break;
	        
        default :
			CopySettings.X_numerator = 100;
			CopySettings.Y_numerator = 100;
            break;
    }

    CopyOrientation = FPCopyConfig->Orientation2p;
    if(N_up == 4)
        CopyOrientation = FPCopyConfig->Orientation4p;

    //cdbg1("%d/%d  %d/%d\n", CopySettings.X_numerator, CopySettings.X_denominator,
    //                     CopySettings.Y_numerator, CopySettings.Y_denominator );
    scerr = scanvar_set_xyscale( sv, CopySettings.X_numerator, CopySettings.X_denominator,
                         CopySettings.Y_numerator, CopySettings.Y_denominator );
    XASSERT( scerr==SCANERR_NONE, scerr ); 

#if 0
#ifdef HAVE_ID_COPY
    if ( idCopyEnable == TRUE )
    {
        //Scaling and n-up are disabled in id copy mode
        scerr = scanvar_set_xyscale( sv, 1, 1, 1, 1 );
        XASSERT( scerr==SCANERR_NONE, scerr );
 //     if( FPCopyConfig->copyQuality==e_TEXT || FPCopyConfig->copyQuality==e_DRAFT ){
 //         scanvar_set_xyscale( sv, 1, 1, 2, 3 );
 //         page_info->Y_Resolution = 200;
 //     }
        N_up = 1;
    }
#endif
#endif
    page_info->nup_image_params.num_images = N_up;
    page_info->nup_image_params.page_orientation = CopyOrientation;

    PTR_ASSIGN( *psv, sv );

    return 0;
}

static scan_err_t load_siqfile( )
{
    scan_err_t scerr;
    struct mbuffer buf;

    memset( &buf, 0, sizeof(struct mbuffer) );
    scerr = file_to_buffer( siq_file_name, &buf );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    scerr = siqdata_add_siqfile( buf.buf, buf.len );

	if (buf.buf)
	{
		mbuffer_free(&buf);
	}

    return scerr;
}

static scan_err_t file_to_buffer( const char *filename, struct mbuffer *buf )
{
    scan_err_t scerr, final_scerr=SCANERR_NONE;
    int retcode;
    struct stat statbuf;
    FILE *f;

#ifdef DEBUG
//    printf( "%s %s\n", __FUNCTION__, filename );
#endif

    f = NULL;
    ASSERT( buf->buf==NULL );

    retcode = stat( filename, &statbuf );
    if( retcode != 0 ) {
//        fprintf( stderr, "unable to stat \"%s\" : %s\n", filename, strerror(errno) );
        return SCANERR_INVALID_PARAM;
    }

#ifdef DEBUG
    printf( "%s %s is %d bytes\n", __FUNCTION__, filename, (int)statbuf.st_size );
#endif

#define BUFFER_SIZE_SANITY_CHECK_MEGABYTES 1

    /* sanity check for really large files; cap for my own peace of mind */
    if( statbuf.st_size > (BUFFER_SIZE_SANITY_CHECK_MEGABYTES*1024*1024) ) {
        fprintf( stderr, "cowardly refusing to allocate more than %dM bytes of memory\n", 
                BUFFER_SIZE_SANITY_CHECK_MEGABYTES );
        return SCANERR_NOT_PERMITTED;
    }

    scerr = mbuffer_new( buf, statbuf.st_size );
    if( scerr != SCANERR_NONE ) {
        fprintf( stderr, "out of memory for buffer\n" );
        goto fail;
    }

    /* read the entire file into a buffer */
    f = fopen( filename, "rb" );
    if( f==NULL ) {
        fprintf( stderr, "unable to open \"%s\" for reading : %s\n", 
                filename, strerror(errno) );
        final_scerr = SCANERR_NO_ENTRY;
        goto fail;
    }

    retcode = fread( buf->buf, 1, buf->max, f );
    if( retcode < 0 ) {
        fprintf( stderr, "failed to read \"%s\" : %s\n", 
                filename, strerror(errno));
        final_scerr = SCANERR_NO_ENTRY;
        goto fail;
    }

    buf->len = retcode;

    fclose( f );

#ifdef DEBUG
    printf( "%s read %d bytes of %s into buffer\n", __FUNCTION__,
                buf->len, filename );
#endif

    return SCANERR_NONE;
fail:
    if( f ) {
        fclose(f);
    }
    if( buf->buf ) {
        mbuffer_free( buf );
    }

    return final_scerr;
}

bool find_siq_name(uint8_t quality)
{
	memset(siq_file_name, 0, sizeof(siq_file_name));

	switch(quality)
	{
		case e_TEXT :
			snprintf(siq_file_name, MAX_TUNING_FILE_NAME, "%sgray_copy_text.siq", TUNING_SIQ_FILE_FOLDER);
			dbg1("[SV] : text siq open (copy)\n");
		break;
		case e_MIXED :
			snprintf(siq_file_name, MAX_TUNING_FILE_NAME, "%sgray_copy_mixed.siq", TUNING_SIQ_FILE_FOLDER);
			dbg1("[SV] : mixed siq open (copy)\n");
		break;
		case e_FILM_PHOTO :
			snprintf(siq_file_name, MAX_TUNING_FILE_NAME, "%sgray_copy_picture.siq", TUNING_SIQ_FILE_FOLDER);
			dbg1("[SV] : photo siq  open (copy)\n");
		break;
	}
	
	if (siq_file_name[0] != 0)
	{
		dbg1("[SV] : SIQ file %s\n", siq_file_name);
		return TRUE;
	}
	else
	{
		dbg1("[SV] : No SIQ file\n");
		return FALSE;
	}
}


/**
 * \brief start the Printer half of the copy job 
 *
 * Had to split this from a simple copy_start_job() because the scanner needs
 * to tell me how big the actual data will be. The data size CopyApp sends down
 * to the scan thread will be changed to match the DMA requirements, scaling,
 * etc.
 *
 * \author David Poole
 * \date 06-Sep-2005
 *
 */

static void
copyapp_printer_start_job( CURRENT_RESOURCE *job_resources, 
                           int page_pixels_per_row, 
                           int page_total_rows,
                           int image_pixels_per_row, 
                           int image_total_rows,
                           PAGEINFO *page_info )
{
    JOBINFO job_info;
    uint8_t jam_recovery;
    CopyConf scan_job_conf;
     
    g_minnumrows=9999; // init min/max numrows
    g_maxnumrows=0;

// BUGBUG: Removed during NUP merge; verify
//-jrs    if(copy_started_from_adf)
//-jrs    {
        // the scanner has no way of knowing the length of the
        // target when scanning from adf, so set the length to the max
        // length (legal paper size) of 14"
//-jrs        page_total_rows = 14 * page_info->Y_Resolution;
//-jrs    }
    cdbg2(  "%s start page_ppr=%d page_total_rows=%d image_ppr=%d image_total_rows=%d\n", 
           __FUNCTION__, page_pixels_per_row,  page_total_rows, 
           image_pixels_per_row,  image_total_rows );

    copyapp_get_copyjob_config(&scan_job_conf);

	if ( (scan_job_conf.scan_resolution == 600) && (page_pixels_per_row & 0x1f) )
	{
	    page_pixels_per_row = (page_pixels_per_row & 0xffffffe0) + 32;
        cdbg2(  "%s change page_ppr=%d for high quality mode\n", __FUNCTION__, page_pixels_per_row);
	}
	
    switch(scan_job_conf.copyQuality)
    {
    case e_TEXT:
    case e_FAX_TEXT:
        page_info->printQuality = PRINTQUALITY_NORMAL;
        break;

    case e_DRAFT:
        page_info->printQuality = PRINTQUALITY_DRAFT;
        break;

    case e_FILM_PHOTO:
    case e_PICTURE:
        page_info->printQuality = PRINTQUALITY_BEST; 
        break;

    case e_MIXED:
    case e_FAX_MIXED:
        page_info->printQuality = PRINTQUALITY_BETTER;
        break;

    default:
        ASSERT(0);
        break;
    }

    /* go through JobUtils to startjob the printer */
    memset( &job_info, 0, sizeof( JOBINFO ) );
    job_info.DocType = e_COPYPAGE;
    job_info.JobName = NULL;
    job_info.JobMode = e_Normal;
    job_info.SendingModule = COPYAPPID;
//    job_info.JobData = e_CopyJob;

    /* davep 16-Aug-2005 ; TODO don't hardwire these parameters */
    page_info->BitsPerPixel = copy_bpp;
    page_info->DocType = e_COPYPAGE;

    page_info->Video_Y = page_total_rows;

    page_info->PixelsPerLine = image_pixels_per_row;
    page_info->Video_X = image_pixels_per_row;

    page_info->nup_image_params.image_height = page_total_rows;
    page_info->nup_image_params.image_width = page_pixels_per_row;

    page_info->RawData = TRUE;

    /* TODO - need to pass the destination modid down to JobUtils */

    mlimiter_start(mlimiter_by_name("print"));

    // Create an empty job pipe with a null job info
    if (NullPipe == 0) 
    {
	NullPipe = &NullPipeMEMORY;
	memset(NullPipe, 0, sizeof(IOReg));
	NullPipe->pipe_cmd_read  = NullPipeRead;
	NullPipe->pipe_cmd_write = NullPipeWrite;
	NullPipe->pipe_ioctl     = NullPipeIOCmd;
	NullPipe->PipeType       = e_Print;
	NullPipe->pjob_info = PrintJobInfo_construct(NullPipe);
	NullPipe->pjob_info->jobDocType = e_COPYPAGE;
    }
    printvar_get_jamrecovery(&old_jam_recovery);
    jam_recovery = 0;
    printvar_set_jamrecovery(&jam_recovery);

    pjob = print_job_construct(mlimiter_by_name("print"), NullPipe);

    // the following two functions are obsolete and should not be used
    // the media size and type should be set at the page level. paw
    //print_job_set_media_size(pjob, page_info->PageSize);
    //print_job_set_media_type(pjob, page_info->PageType);

    print_job_set_doc_type(pjob, page_info->DocType);
    print_job_set_collation(pjob, page_info->collate);
	print_job_set_darkness_level(pjob, page_info->darkness);
	print_job_set_num_copies(pjob, page_info->copies);
	print_job_set_job_info(pjob, COPYJOB);
	print_job_set_copy_tray_setting_info(pjob, scan_job_conf.printTrayPaperInfo[TRAY_MPT], scan_job_conf.printTrayPaperInfo[TRAY1],
										scan_job_conf.printTrayPaperInfo[TRAY2],scan_job_conf.printTrayPaperInfo[TRAY3]);

    if (scan_job_conf.printDuplex != DMDUP_SIMPLEX)
    {
        cdbg2(  "%s: Enable Print Duplex\n", __FUNCTION__ );
        print_job_set_duplex(pjob, DMDUP_VERTICAL);
        print_job_set_duplex_rotate_backside(pjob, true);
    }
    print_job_start(pjob, job_resources->Destination);
}

/**
 * \brief Setup a start page for the downstream (printer) resource.  
 *
 *
 * \author David Poole
 * \date 16-Aug-2005
 *
 */

static void
scanner_start_page( struct scanvars *sv, CURRENT_RESOURCE *job_resources )
{
    MESSAGE msg;
    error_type_t err;
    uint32_t PageCnt=0;
    CopyConf copy_config;

    cdbg2(  "%s job_resources=%p src=%d\n", __FUNCTION__, job_resources, job_resources->Source );

	memset( &copy_config, 0, sizeof(copy_config) );
    copyapp_get_copy_config(&copy_config);
    /* send the startjob to my upstream; we pass control of the scanvar to the
     * scanner itself
     */
	scanman_set_scanpapersize(copy_config.scanPaperSize);
	
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_SCAN_JOBSTART;
	msg.param1 = COPYAPPID;
    msg.param2 = g_roller_erase_adjust;
    msg.param3 = sv;
    err = SYMsgSend( job_resources->Source, &msg );
    XASSERT( err==OK, err );

    // Increment the copy count.
//    scands_get_copy_pages(&PageCnt);
    PageCnt++;
//    scands_set_copy_pages(&PageCnt);
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 26-Jul-2007
 *
 *  Moved some of Dirk's code into a function.
 */

print_page_t* copyapp_printer_create_page( PAGEINFO *page_info ) 
{
    CopyConf copy_config;
    print_page_t *print_page;

    cdbg2(  "%s\n", __FUNCTION__ );

    copyapp_get_copyjob_config(&copy_config);

    print_page_info( page_info );

    print_page = print_page_construct(pjob);
    ASSERT(print_page != NULL);

    uint32_t brightness, contrast;
    // map the brightness and contrast to range [0,200]
    brightness = (copy_config.copyBrightness -1) * 20;
    contrast = (copy_config.copyContrast -1) * 20; 
    print_page_set_brightness( print_page, brightness );
    print_page_set_contrast( print_page, contrast );

    print_page_set_media_size( print_page, page_info->PageSize );
    print_page_set_media_type( print_page, page_info->PageType );
    print_page_set_input_source(print_page, page_info->InputTray );
    print_page_set_duplex(print_page, page_info->duplexInfo );
    cdbg2("%s, %d\n", __FUNCTION__, page_info->duplexInfo);

    print_page_set_print_quality( print_page, page_info->printQuality );

    print_page_set_dpi_x(print_page, page_info->X_Resolution);
    print_page_set_dpi_y(print_page, page_info->Y_Resolution);
    print_page_set_raster_x(print_page, page_info->Video_X * 
			    page_info->BitsPerPixel);
//    print_page_set_image_height_lines(print_page, page_info->Video_Y);
    print_page_set_video_x(print_page, page_info->Video_X);
    print_page_set_output_bpp(print_page, page_info->BitsPerPixel); //PRINTER_PIXEL_DEPTH);

    print_page_set_num_copies(print_page, page_info->copies);
    print_page_set_color_mode(print_page, page_info->monoOrColor);
    print_page_set_colorspace(print_page, (page_info->monoOrColor == e_Mono) ? e_additive_gray : e_sRGB);

    print_page_set_oneup_rotation_params(print_page, rotation);
    print_page_set_nup_params(print_page, &(page_info->nup_image_params));

	print_page_set_app(print_page, (uint8_t)APPTYPE_COPY);
	
    return print_page;
}

void copyapp_printer_start_page( print_page_t* print_page, const PAGEINFO *page_info ) 
{
    print_page_start(print_page);
#if 1 // def SHILOH
    if ( print_page_start_would_block() )
    {
	    // next page will block so pause the scanner 
	    // scanmech_adf_client_request_pause();
	    // scanmech_adf_client_clear_pause();
    }
#endif

    print_plane_start(print_page, COLOR_BLACK);
    if( page_info->monoOrColor == e_Color )
    {
        print_plane_start(print_page, COLOR_YELLOW);
        print_plane_start(print_page, COLOR_MAGENTA);
        print_plane_start(print_page, COLOR_CYAN);
    }
}

/**
 * \brief Send a strip to the downstream (printer) resource. 
 *
 *
 * \author David Poole
 * \date 16-Aug-2005
 *
 */
void copyapp_send_data( print_page_t *print_page,
                   int num_rows, 
                   uint32_t current_lines,
                   bool last_strip, const PAGEINFO *page_info, void *data,
                   uint32_t comp_type, uint32_t buf_size, 
                   bool print_right_to_left, uint32_t image_num)
{
    PLANE_TYPE planetype;
    UINT8 colorplanes; 
    compression_info_t compression_info;
    BigBuffer_t *big_buffer;
    COLOR color=COLOR_BLACK;
    mlimiter_t* scan_limiter = NULL;
//    uint32_t padrowscnt = 0;
//    uint32_t padrowslen = 0;

    uint32_t num_pad_rows = 0;
	cdbg3( "%s: %d, %d\n", __func__, current_lines, num_rows );
	
    if( (! last_strip) && (current_lines-num_rows) )
    {
       // in function video_start_plane in video_laser_color_mono.c 
       // there is an assumption for JBIG compression that the entire
       // page will contain strips that don't change num_rows more than
       // 64.  assert here if the delta exceeds 64 (MAX_STRIP_ROWNUMBER_DELTA).
       if(num_rows < g_minnumrows) g_minnumrows = num_rows;
       if(num_rows > g_maxnumrows) g_maxnumrows = num_rows;
       if(g_maxnumrows - g_minnumrows > MAX_STRIP_ROWNUMBER_DELTA)
       {
	   cdbg1( "ERROR: g_maxnumrows - g_minnumrows  TOO LARGE\n");
	   cdbg1( "       g_maxnumrows: %d g_minnumrows: %d\n", 
	                 g_maxnumrows, g_minnumrows);
       }
       ASSERT(g_maxnumrows - g_minnumrows <= MAX_STRIP_ROWNUMBER_DELTA);
    }
    else
    {
        cdbg1("%s: FIRST/LAST_STRIP current_lines = %d, num_rows = %d\n", __func__, current_lines, num_rows);
    }

    ASSERT(num_rows > 0);

	if (g_top_padding && (current_lines-num_rows < g_top_padding) )
	{
		cdbg3("padding 2 %d, %d\n",  num_rows, current_lines-num_rows);
		if (current_lines <= g_top_padding )
		{
			memset(data, 0xFF, (g_page_info.PixelsPerLine*num_rows));
			cdbg1("padding : T1 (%d lines)\n",  num_rows);
		}
		else
		{
			memset(data, 0xFF, (g_page_info.PixelsPerLine * (g_top_padding - (current_lines-num_rows))));
			cdbg1("padding : T2 (%d lines)\n",  (g_top_padding - (current_lines-num_rows)));
		}
	}
	else if ( (g_bottom_padding) && (g_bottom_padding < current_lines) )
	{
		if (g_bottom_padding < (current_lines - num_rows))
		{
			memset( data, 0xFF, g_page_info.PixelsPerLine * num_rows);
			cdbg1("padding : B1(%d lines)\n",num_rows);
		}
		else
		{
			memset( data+( (g_bottom_padding-(current_lines-num_rows)) * g_page_info.PixelsPerLine), 0xFF,
					g_page_info.PixelsPerLine * (current_lines - g_bottom_padding));
			cdbg1("padding : B2(%d lines)\n",(current_lines - g_bottom_padding));
		}
	}
	
#if 0
	if (g_right_padding < page_info->PixelsPerLine)
	{
		int nLoop;
		
		for (nLoop=0; nLoop<num_rows; nLoop++)
			memset( data+(nLoop*page_info->PixelsPerLine)+g_right_padding, 0xFF, page_info->PixelsPerLine - g_right_padding);
	}
#endif
#if 0		//	skip for gray level
    // pad the last strip if it is from adf and not a multiple of VIDEOMULTIPLE
    if (last_strip && current_lines % VIDEOMULTIPLE != 0)
    {
        mlimiter_t *scanlimiter = mlimiter_by_name("scan");
        padrowscnt = VIDEOMULTIPLE - (current_lines % VIDEOMULTIPLE);
        num_rows += padrowscnt;
        padrowslen = padrowscnt * page_info->BitsPerPixel * page_info->PixelsPerLine / 8;

        mlimiter_start(scanlimiter);
        data = MEM_REALLOC_LIMITED(scanlimiter, data, padrowslen + buf_size);
        mlimiter_stop(scanlimiter);
        XASSERT(data != 0, (uint32_t)data);

        memset(data + buf_size, 0xff, padrowslen); // pad with RGB white
        buf_size += padrowslen;
        cdbg1("padding strip new numrows=%d buf_len=%d\n", num_rows, buf_size);
    }
#endif
    cdbg3( "%s num_rows=%d ppl:%d bufsize:%d, last_strip=%d\n", 
	    __FUNCTION__, num_rows, page_info->PixelsPerLine,
	   buf_size, last_strip );

    /* davep 30-Jul-2007 ; check our fencepost */
    /* davep 31-Jul-2007 ; only check if we're using job memory (memory
     * allocated in allocate_nup_memory())
     */
#ifdef JOB_MEMORY_FENCEPOST
    if (N_up != 1)
    {
        UINT8 *ptr;
        ptr = (UINT8 *)data;
        /* skip to end of buffer then back up 64 bytes; look for fencepost */
        ptr += g_job_memory_buffer_size;
        if ( *ptr != 0xee )
        {
            cdbg1(  "bad pointer: datalen=%d data=%p ptr=%p\n",
                        g_job_memory_buffer_size, data, ptr );
            scanlog_hex_dump( (unsigned char *)data, g_job_memory_buffer_size+64 ); 
        }

        XASSERT( *ptr==0xee, *ptr );
    }
#endif

    /* davep 20-Sep-2010 ; new architecture code merged into trunk, breaking a
     * few things.  Scan code needs to work under old regime as well.
     * Eventually the JobUtils structures will be merged backward to old code.
     */
#if 0 // hack for RFI testing.  will fix later...
#ifdef STRIP_INFO_2010
    /* new code; via merge with newer firmware architecture */
    strip_info.big_buffer = BigBuffer_adopt(data, buf_size);
    strip_info.stripDataType = GRAYSCALE;
#else
    /* old code  (pre Sep-2010) */
    strip_info.big_buffer = BigBuffer_StructMalloc();
    strip_info.big_buffer->data = data;

    strip_info.realData = TRUE;
#endif
#endif
//
    if (page_info->monoOrColor == e_Mono)
    {
        planetype = GRAYSCALE;
        colorplanes = 1;
        compression_info.comp_type = COMPTYPE_NONE;
        compression_info.comp_details.rgb.bits_per_pixel = 8;
    }
    else
    {
        planetype = PACKED_RGB;
        //planetype = PACKED_RGBX;
        colorplanes = 4;
        compression_info.comp_type = COMPTYPE_NONE;
        compression_info.comp_details.rgb.bits_per_pixel = 24;
    }
#if 1
    if ((last_strip) && (g_fb_bottom_padding_add))
    {
        // we pad all pages to the full number of lines for the page.
        // This is done so duplex output has consistent image placement on the back side
        // of pages.  This is required because the back side is typically rotated and
        // we need the padding to push the inverted image to the correct page location.
#if 0
        ASSERT(g_page_info.Video_Y >= current_lines);
        if (g_page_info.Video_Y > current_lines)
        {
            scan_limiter = mlimiter_by_name("scan");
            mlimiter_start(scan_limiter);
            num_pad_rows = g_page_info.Video_Y - current_lines;
            cdbg1("padding page num_pad_rows=%d\n", num_pad_rows);
        }
#else
		scan_limiter = mlimiter_by_name("scan");
		mlimiter_start(scan_limiter);
		
#if 0
		if (g_fb_bottom_padding_add == BOTTOM_PADDING_OPEN)
		{
			if (((g_scan_height_for_padding * g_page_info.Y_Resolution)/100) > current_lines)
			{
				num_pad_rows = ((g_scan_height_for_padding * g_page_info.Y_Resolution)/100) - current_lines;
				if (num_pad_rows < 5)
					num_pad_rows = 0;
			}
			else
			{
				num_pad_rows = 0;
			}
			cdbg1("padding page num_pad_rows=%d\n", num_pad_rows);
		}
		else
#endif
		{
			num_pad_rows = g_fb_bottom_padding_add;
			cdbg1("padding page num_pad_rows FB=%d\n", num_pad_rows);
		}
#endif
    }
#endif
    copyapp_stats.rows_received += num_rows; 

    cdbg3( "%s num_rows=%d ppl:%d bufsize:%d, last_strip=%d\n", 
	    __FUNCTION__, num_rows, page_info->PixelsPerLine,
	   buf_size, last_strip );

	if (num_rows == 1)
	{
		cdbg2("buffer num_rows is 1... So skip\n");
		memFree(data);
    	if (scan_limiter != NULL)
    	{
        	mlimiter_stop(scan_limiter);
    	}
		return;
	}
	
    big_buffer = dma_buffer_adopt(data, buf_size);    

    cdbg3( "print_plane_add_data color=%d data=%p\n", color, big_buffer );
    cdbg3( "  ss:%d pt:%d ls:%d res:%d ppl:%d bpp:%d csr:%d tr:%d\n",
           buf_size, planetype, last_strip,  page_info->X_Resolution,
           page_info->PixelsPerLine, page_info->BitsPerPixel, num_rows,
           copyapp_stats.rows_received);

    print_plane_add_data( print_page, 
                          color, 
                          big_buffer, 
                          buf_size, 
                          planetype, 
                          (num_pad_rows == 0 ? last_strip : false), // only pass along last_strip if we're not padding; otherwise done below.
                          page_info->X_Resolution, page_info->Y_Resolution,
                          page_info->PixelsPerLine, 
                          num_rows, 
                          page_info->BitsPerPixel, 
                          (bool)print_right_to_left, &compression_info);

#if 1
    while (num_pad_rows > 0)
    {
        uint32_t  padding_bytes;
        char     *padding_buffer;

        ASSERT(scan_limiter != NULL);

        if ( num_pad_rows > MAX_PADDING_STRIP_LINES )
        {
            num_rows = MAX_PADDING_STRIP_LINES;
            last_strip = false;
        }
        else
        {
            num_rows = num_pad_rows;
            last_strip = true;
        }
        num_pad_rows -= num_rows;

        ASSERT(num_pad_rows == 0 ? (last_strip) : true);

        copyapp_stats.rows_received += num_rows; 

        padding_bytes = ((page_info->PixelsPerLine * page_info->BitsPerPixel) / CHARSIZE) * num_rows;

        padding_buffer = MEM_MALLOC_LIMITED(scan_limiter, padding_bytes);
        ASSERT(padding_buffer != NULL);

        memset(padding_buffer, 0xff, padding_bytes); // pad with RGB white

		if (num_rows == 1)
		{
			cdbg2("buffer num_rows is 1... So skip2\n");
			memFree(padding_buffer);
			break;
		}
		
        big_buffer = dma_buffer_adopt(padding_buffer, padding_bytes);    

        cdbg1("\n%s: PADDING Page %#x lines of pad = %d\n", __func__, (uint32_t)print_page, num_rows);
//        cdbg2("adding %d lines of padding\n", num_rows);

        cdbg3("print_plane_add_data color=%d data=%p\n", color, big_buffer);
        cdbg3("  ss:%d pt:%d ls:%d res:%d ppl:%d bpp:%d csr:%d tr:%d\n",
              buf_size, planetype, last_strip,  page_info->X_Resolution,
              page_info->PixelsPerLine, page_info->BitsPerPixel, num_rows,
              copyapp_stats.rows_received);

        print_plane_add_data(print_page,
                             color,
                             big_buffer,
                             padding_bytes,
                             planetype,
                             last_strip,
                             page_info->X_Resolution, page_info->Y_Resolution,
                             page_info->PixelsPerLine,
                             num_rows,
                             page_info->BitsPerPixel,
                             print_right_to_left, &compression_info);
    }
    if (scan_limiter != NULL)
    {
        mlimiter_stop(scan_limiter);
    }
#endif
}

/**
 * \brief Handle end of a copy page.
 *
 * Does all the necessary pageend tasks in a copy. Calls the downstream
 * (printer) JobUtils function to finish a page. 
 *
 * \author David Poole
 * \date 19-Aug-2005
 *
 */

void copyapp_printer_end_page( print_page_t *print_page, const PAGEINFO *page_info)
{
    cdbg2("%s\n", __FUNCTION__);
    print_plane_end(print_page, COLOR_BLACK);
    if (page_info->monoOrColor == e_Color)
    {
        print_plane_end(print_page, COLOR_CYAN);
        print_plane_end(print_page, COLOR_YELLOW);
        print_plane_end(print_page, COLOR_MAGENTA);
    }
    print_page_end(print_page, NULL);
    print_page_free(print_page);

    /* Keep track of the actual number of copy pages printed */
//    increment_copy_pages_printed();
}

/**
 * \brief End a copy job. 
 *
 * Calls the downstream (printer) JobUtils function to end the job. Sends the
 * necessary message to the system job manager to release myself as a
 * resource.
 *
 * \author David Poole
 * \date 19-Aug-2005
 *
 */

static void
copy_end_job( struct scanvars **psv, CURRENT_RESOURCE *job_resources )
{
    MESSAGE msg;
    error_type_t err;
    struct scanvars *sv;

    copy_cancel_invalid = true;

    /* use a temporary to avoid pointer confusion */
    PTR_ASSIGN( sv, *psv );

    cdbg2(  "%s src=%d\n", __FUNCTION__, job_resources->Source );
    if(pjob != NULL)
    {
        print_job_end(pjob);
        mlimiter_stop(mlimiter_by_name("print"));
        pjob = NULL;
    }

    /* Tell the source that we are done talking to him. */
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_SCAN_JOBEND;
    msg.param3 = job_resources;
    msg.param2 = COPYAPPID;
    err = SYMsgSend(job_resources->Source, &msg); // tell the scanner to finish.
    XASSERT( err==OK, err );

    /* davep 26-Jul-2007 ; moved sending the FREERECIPE from above sending the
     * JOBEND_EX (shotgun debug)
     */
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_FREERECIPE;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = job_resources;
    err = SYMsgSend( SJMID, &msg );
    XASSERT( err==OK, err );

    copyapp_log_event( COPYAPP_EVENT_COPY_COMPLETE );

    if( g_copying_continuously ) {
        cdbg2(  "copy continuously starting next job\n" );
        copyapp_start_job();
    }

    printvar_set_jamrecovery(&old_jam_recovery);

    /* free the configuration memory */
    cdbg2(  "%s free scanvars\n", __FUNCTION__ );
    scanvar_delete( &sv );

    // clear any nup flags
    g_page_info.nup_image_params.ID_nup = IDNUP_INACTIVE;
    idCopyEnable = FALSE;
}

/**
 * \brief  Copy Continuous 
 *
 * \author David Poole
 * \date  16-Aug-2011 
 *
 */

#define COPY_CONTINUOUS_RUN 0x01

void* copy_continuous_task( void* unused )
{
    UINT pxretcode;
    int total_num_jobs;
    uint32_t noncontinuous_num_copies;
    CopyConf copy_config;

    while( 1 ) 
    {
        /* wait for the run flag to be set */
        pxretcode = pthread_mutex_lock(&copy_continuous_mutex);
        XASSERT(pxretcode==0, pxretcode);
        while(!(copy_continuous_flag & COPY_CONTINUOUS_RUN))
        {
          pxretcode = pthread_cond_wait(&copy_continuous_cond, &copy_continuous_mutex);
          XASSERT(pxretcode==0, pxretcode);
        }
        pxretcode = pthread_mutex_unlock(&copy_continuous_mutex);
        XASSERT(pxretcode==0, pxretcode);

    	copyapp_get_copyjob_config(&copy_config);
	    noncontinuous_num_copies = copy_config.copyNumber; // save old value
    	#ifdef COLOR_ENG
	    copy_config.copyNumber = continuous_color_num_copies;
	    #else
	    copy_config.copyNumber = continuous_mono_num_copies;
	    #endif
	    copyapp_set_copy_config(&copy_config);    

        /* run copies until the RUN flag is cleared */
        while( 1 ) 
        {
            struct timespec curr_ts;
            clock_gettime( CLOCK_REALTIME, &curr_ts );
                 
            if(g_contscan_time > 0 &&
               (curr_ts.tv_sec - start_ts.tv_sec > g_contscan_time * 60)) 
            {
                cdbg1(" STOP TIMED CONTINUOUS SCAN\n");

                pxretcode = pthread_mutex_lock(&copy_continuous_mutex);
                XASSERT( pxretcode==0, pxretcode );
                copy_continuous_flag &= ~COPY_CONTINUOUS_RUN;
                pxretcode = pthread_cond_broadcast(&copy_continuous_cond);
                XASSERT( pxretcode==0, pxretcode );
                pxretcode = pthread_mutex_unlock(&copy_continuous_mutex);
                XASSERT(pxretcode==0, pxretcode);
            }

            total_num_jobs = SJMGetTotalNumJobs();
            if( total_num_jobs==0 ) {
                copyapp_start_job();
            }
            posix_sleep_ticks( 5*TIMER_INTS_PER_SECOND );

            /* get new value of my run bit */
            pxretcode = pthread_mutex_lock(&copy_continuous_mutex);
            XASSERT(pxretcode==0, pxretcode);
            if(!(copy_continuous_flag & COPY_CONTINUOUS_RUN))
            {

                struct timespec   ts;
                clock_gettime( CLOCK_REALTIME, &ts );
                ts.tv_sec += WAIT_TIME_SECONDS;

                // get new value of run bit
                pxretcode = pthread_cond_timedwait(&copy_continuous_cond, 
                                                   &copy_continuous_mutex,
                                                   &ts);
                if (pxretcode == ETIMEDOUT)
                {
                  // leave inner while(1) loop
                  break;
                }
                XASSERT(pxretcode==0, pxretcode);
            }
            pxretcode = pthread_mutex_unlock(&copy_continuous_mutex);

            XASSERT(pxretcode==0, pxretcode);

        } /* end while(flag) */

	copyapp_get_copyjob_config(&copy_config);
	copy_config.copyNumber = noncontinuous_num_copies; // restore old value
	copyapp_set_copyjob_config(&copy_config);    

    } /* end while(1) */

}

void copy_continuous_stop( void )
{
    UINT pxretcode;

    cdbg2(  "%s\n", __FUNCTION__ );

    /* clear the run bit */
    pxretcode = pthread_mutex_lock(&copy_continuous_mutex);
    XASSERT( pxretcode==0, pxretcode );
    copy_continuous_flag &= ~COPY_CONTINUOUS_RUN;
    pxretcode = pthread_cond_broadcast(&copy_continuous_cond);
    XASSERT( pxretcode==0, pxretcode );
    pxretcode = pthread_mutex_unlock(&copy_continuous_mutex);
    XASSERT(pxretcode==0, pxretcode);
}

void copy_continuous_start( void )
{
    UINT pxretcode;

    cdbg2(  "%s\n", __FUNCTION__ );

    clock_gettime( CLOCK_REALTIME, &start_ts );

    /* set the run bit */
    pxretcode = pthread_mutex_lock(&copy_continuous_mutex);
    XASSERT( pxretcode==0, pxretcode );
    copy_continuous_flag |= COPY_CONTINUOUS_RUN;
    pxretcode = pthread_cond_broadcast(&copy_continuous_cond);
    XASSERT( pxretcode==0, pxretcode );
    pxretcode = pthread_mutex_unlock(&copy_continuous_mutex);
    XASSERT(pxretcode==0, pxretcode);
}

static void copy_continuous_onetime_init( void )
{
    UINT pxretcode;

    copy_continuous_flag = 0;

    pxretcode = posix_create_thread(&copy_continuous_task_id, &copy_continuous_task, 0,
                                    "copycont", copy_continuous_task_stack, COPYTASK_STACK_SIZE,
                                     POSIX_THR_PRI_NORMAL); 
    XASSERT(pxretcode==0, pxretcode);
}

/**
 * \brief cancel the copyapp's current copy job
 *
 *
 * \author David Poole
 * \date 10-Nov-2005
 *
 */

static void
copy_cancel( void *weird_unknown_parameter, 
             CURRENT_RESOURCE *job_resources)
{
    MESSAGE msg;
    error_type_t err;
    int page_side;

    cdbg2(  "%s\n", __FUNCTION__ );

    //
    // Free memory that we are storing internal.
    //
    for (page_side = 0; page_side < 2; page_side++) 
    {
        free_strip_list(&copy_duplex_pages[page_side].data_strips);
        copy_duplex_pages[page_side].print_page_active = 0;
        if (copy_duplex_pages[page_side].print_page != NULL)
        {
            print_page_free(copy_duplex_pages[page_side].print_page);
            copy_duplex_pages[page_side].print_page = NULL;
        }
    }

    /* send an ACK to the job manager */
    msg.msgType = MSG_CANCELJOB;
    msg.param1 = SYS_ACK;
    msg.param2 = COPYAPPID;
    msg.param3 = weird_unknown_parameter;
    err = SYMsgSend( SJMID, &msg);
    XASSERT( err==OK, err );

    g_copying_continuously = FALSE;     // if we are doing a continuous copy, abort it also.

    if(copy_cancel_invalid)
    {
	cdbg1("%s - cancel received at an invalid time, ignoring...\n", __FUNCTION__);
	return;
    }

    copyapp_log_event( COPYAPP_EVENT_CANCEL );

    /* release myself as a resource */
    msg.msgType = MSG_FREERECIPE;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = job_resources;
    err = SYMsgSend( SJMID, &msg );
    XASSERT( err==OK, err );

    cdbg2(  "%s done\n", __FUNCTION__ );
}

/**
 * \brief  simple helper function to find the max scan area 
 *
 * \param max
 * maximum number we want
 *
 * \param numer
 * scale factor numerator
 *
 * \param denom
 * scale factor denominator
 *
 * \retval maximum size based on input and scaling
 *
 * Run a naive scale against the input. If the output is larger than the
 * allowed max, find the new input area we need to fill the allowed output
 * area. At last, a legitimate use for algebra!
 *
 * \author David Poole
 * \date 19-Jan-2006
 *
 * BIG FAT NOTE! 
 * (count*numer)/denom is NOT NOT NOT the actual way the PIE XYScale works but
 * it's close enough we can do some rough approximations. The lower level scan
 * code will take these numbers and adjust them for the actual hardware. We
 * might very well get out MORE rows|pixels based on the XYScale behavior than
 * we calculate here.
 *
 */

static int
calc_size( int scan_size, int print_max, int numer, int denom )
{
    int out;

    /* based on our input, how much do we get out? */
    out = safeint_scale_int( scan_size, numer, denom );
//    out = (max * numer) / denom;

    /* if we get out more than we can fit, only capture what we can fit */
    if( out > print_max ) {
        /* assume the number we want out is max; now solve for how many we want
         * in 
         */
        return safeint_scale_int( print_max, denom, numer );
//        return (max * denom) / numer;
    }
    return scan_size;
}

/**
 * \brief  set the scan area based on user's copy settings
 *
 * \param FPCopyConfig
 *  the current copy settings
 *
 * \author David Poole
 * \date 19-Jan-2006
 *
 */

static void 
set_scan_area( struct scanvars *sv, const CopyConf *FPCopyConfig, mediasize_t scan_page_size )
{
    ENGMEDIASIZES scan_size;
    ENGMEDIASIZES print_size;
	error_type_t db_retval = FAIL;
    int x_num, x_denom, y_num, y_denom;
    int max_rows, rows_in, max_cols, cols_in;
    int left_margin_hinches_x, top_margin_hinches_y;
    int scan_width_hinches, scan_height_hinches;
    int print_width_hinches, print_height_hinches;
    int left_border_hinches=0, top_border_hinches=0;
    int x_offset;
    int CopyEraseMargin;

	g_top_padding = 0;
	g_bottom_padding = 0;
	g_right_padding = 0;
	g_top_erase = 0;
	g_fb_bottom_padding_add = 0;
//	g_scan_height_for_padding = 0;
	
    // set the media size in the structure.
    memset( &scan_size, 0, sizeof(ENGMEDIASIZES));
    memset( &print_size, 0, sizeof(ENGMEDIASIZES));
    paper_get_media_size(scan_page_size, &scan_size.width, &scan_size.height);
    paper_get_media_size(FPCopyConfig->printPaperSize, &print_size.width, &print_size.height);
/*
	if (idCopyEnable)
		scan_size.width = 18460;
*/
    /* davep 03-Aug-2007 ; XXX temp debug */
    print_media_size( &scan_size );

    /* davep 19-Jan-2006 ; the media sizes are in millimeters so we need to
     * convert to inches;  scanvar_set_area wants 1/100ths of an inch 
    */
    scan_width_hinches  = (scan_size.width*100)/2540;
    scan_height_hinches = (scan_size.height*100)/2540;
    print_width_hinches  = (print_size.width*100)/2540;
    print_height_hinches = (print_size.height*100)/2540;
    
    scanvar_get_xyscale(sv, &x_num, &x_denom, &y_num, &y_denom);
	cdbg2("scale x(%d)(%d), y(%d)(%d)\n", x_num, x_denom, y_num, y_denom);

    /* We need to get the platform margin data based on the document source, dpi, 
     * and color mode. Note that the platform margins deal with scanner hardware
     * issues (like hiding the scan bezel) ... the copy application needs to deal
     * with its own margin issues separately.
     */
	
	//Sync Margin Table with DB
	scanner_service_sync_marginTable();
     
    left_margin_hinches_x = 0;
    top_margin_hinches_y = 0;
    g_roller_erase_adjust = 0;
    
    /* Get copy config of 'scan DPI' value for setup the Margin */
	CopyConf config_value;
	memset( &config_value, 0, sizeof(config_value) );
	copyapp_get_copy_config(&config_value);

    if (sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED) {
		scan_err_t scerr = SCANERR_NONE;
		struct scan_flatbed_margins fb_margins;
        scerr = scanmech_get_fb_margins( config_value.scan_resolution, SCAN_CMODE_MONO, &fb_margins );
		if( scerr==SCANERR_NONE ) {                                 
			left_margin_hinches_x = fb_margins.left_margin_hinches_x;
			top_margin_hinches_y = fb_margins.top_margin_hinches_y;
/*
			if (idCopyEnable) {
				left_margin_hinches_x += 100;
			}
*/
		}
		else {
			cdbg1("Can't get FB Margin. ERR: %d\n", scerr);		
		}
    }
    else {
		scan_err_t scerr = SCANERR_NONE;
		struct scan_adf_margins adf_margins;
		scerr = scanmech_get_adf_margins( config_value.scan_resolution, SCAN_CMODE_MONO, &adf_margins );
		if( scerr==SCANERR_NONE ) {
			left_margin_hinches_x = adf_margins.left_margin_hinches;
		}
		else {
			cdbg1("Can't get ADF Margin. ERR: %d\n", scerr);
		}
		/* SINDOH */
		//For ADF top margin, we use margin table located in Kernel
	}
	cdbg1("margin : x(%d), y(%d)\n", left_margin_hinches_x, top_margin_hinches_y);

    /* burtp 06-Oct-2009;  Had a long talk with Paul about how copy data gets
     *  printed on the page.  Short version: our scan data gets centered on the
     *  printed page.  So as we add platform margins we will throw our data off 
     *  center.  Adjust the media width/height to get the data centered again.
     */
	
    if ((FPCopyConfig->copyResize == e_A3_MERGE) || (FPCopyConfig->copyResize == e_DLT_MERGE) || (g_legal_merge) 
    	|| (FPCopyConfig->copyResize == e_A3_SEPARATE) || (FPCopyConfig->copyResize == e_DLT_SEPARATE) )
    {
		scan_width_hinches = 850;

		if (g_legal_merge)
		{
			scan_height_hinches = 725;
			if (FPCopyConfig->printPaperSize == MEDIASIZE_LETTER)
				scan_height_hinches = 730;
			cdbg1( "scan_height_hinches %d\n", scan_height_hinches);
		}

		if ((scan_height_hinches+top_margin_hinches_y) > MAX_FB_HEIGHT_HINCHES_WITH_MARGIN)
		{
			cdbg2( "%s: limiting flatbed scan height of %d to %d\n", __FUNCTION__,
					scan_height_hinches, MAX_FB_HEIGHT_HINCHES_WITH_MARGIN);
			scan_height_hinches = MAX_FB_HEIGHT_HINCHES_WITH_MARGIN - top_margin_hinches_y;
		}
		
		if (scan_height_hinches > MAX_FB_HEIGHT_HINCHES)
		{
			scan_height_hinches = MAX_FB_HEIGHT_HINCHES;
		}
			
		scanvar_set_area( sv, left_margin_hinches_x, top_margin_hinches_y, scan_width_hinches, scan_height_hinches);
		cdbg1("scanvar_set_area(A3M-FB) : %d, %d, %d, %d, LG(%d)\n", left_margin_hinches_x, top_margin_hinches_y, scan_width_hinches, scan_height_hinches, g_legal_merge );
	}
    else if (FPCopyConfig->copyResize == e_TWO_PAGES_PER_SHEET || FPCopyConfig->copyResize == e_FOUR_PAGES_PER_SHEET || rotation )
    {
/*
		left_border_hinches = (BORDER_LR_MM*1000*x_denom)/ (2540*x_num);
		top_border_hinches = (BORDER_TB_MM*1000*y_denom)/ (2540*y_num);
*/
		CopyEraseMargin = db_get_var_int("CONF_SCANSP", "CopyEraseMargin", &db_retval);
		if (db_retval != FAIL) 
		{
			left_border_hinches = (CopyEraseMargin * x_denom) / (x_num);
			top_border_hinches = (CopyEraseMargin * y_denom) / (y_num);
			cdbg1("get CopyEraseMargin : %d\n", CopyEraseMargin);
		}
		else
		{
			left_border_hinches = (BORDER_LR_MM*1000*x_denom)/ (2540*x_num);
			top_border_hinches = (BORDER_TB_MM*1000*y_denom)/ (2540*y_num);
			cdbg1("ERROR get CopyEraseMargin\n");
		}
	
		scan_width_hinches -= (left_border_hinches*2);
		scan_height_hinches -= (top_border_hinches*2);

		cdbg1("border L(%d), T(%d), W(%d), H(%d)\n", left_border_hinches, top_border_hinches, scan_width_hinches, scan_height_hinches);

		if (rotation)
        {
			max_rows = scan_height_hinches;
			scan_height_hinches = calc_size( max_rows, (print_width_hinches-(CopyEraseMargin*2)), y_num, y_denom );
			
			max_cols = scan_width_hinches;
			scan_width_hinches = calc_size( max_cols, (print_height_hinches-(CopyEraseMargin*2)), x_num, x_denom );
			if (x_num > x_denom)
				scan_width_hinches = (ALIGN32_L((scan_width_hinches*FPCopyConfig->copy_resolution_y)/100) * 100 ) / (FPCopyConfig->copy_resolution_y);
        }
        
		if( copy_started_from_adf )
		{
			g_top_erase = (FPCopyConfig->copy_resolution_y*(CopyEraseMargin))/(100);
			scan_height_hinches += top_border_hinches;

			x_offset = ( 850 - scan_width_hinches) / 2;	// 8.5inch : max scan width, (0.08 * 2)inch : mech margin
			scanvar_set_area( sv, left_margin_hinches_x+x_offset, 0, scan_width_hinches, scan_height_hinches);
			cdbg1("scanvar_set_area(nUP-ADF) : %d, %d, %d, %d, e(%d)\n", left_margin_hinches_x+x_offset, top_border_hinches, scan_width_hinches, scan_height_hinches, g_top_erase);
			/* SINDOH */
			//For ADF top margin, we use margin table located in Kernel
		}
		else
		{
			/* burtp 02-Nov-2009; Don't allow flatbed scans longer than the glass or bad 
			* things will happen: the motor will stop short and the output will smear.
			*/
			if ( (sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED) &&
				((scan_height_hinches+top_margin_hinches_y+top_border_hinches) > MAX_FB_HEIGHT_HINCHES_WITH_MARGIN ) )
			{
				cdbg2( "%s: limiting flatbed scan height of %d to %d\n", __FUNCTION__,
				scan_height_hinches, MAX_FB_HEIGHT_HINCHES_WITH_MARGIN);
				scan_height_hinches = MAX_FB_HEIGHT_HINCHES_WITH_MARGIN - top_border_hinches - top_margin_hinches_y;
			}
			scanvar_set_area( sv, left_margin_hinches_x+left_border_hinches, top_margin_hinches_y+top_border_hinches, 
				scan_width_hinches, scan_height_hinches);
				
			g_roller_erase_adjust = (FPCopyConfig->copy_resolution_y*(CopyEraseMargin))/(100);
			cdbg1("scanvar_set_area(nUP-FB) : %d, %d, %d, %d, %d\n", left_margin_hinches_x+left_border_hinches, top_margin_hinches_y+top_border_hinches,
					scan_width_hinches, scan_height_hinches, g_roller_erase_adjust);
		}
    }
    else 
    {
        // Scan normal area
        
        /* davep 19-Jan-2006 ; (bug 498) factor the scale into the
         * output size so we only scan as much as we need 
         */

        /* start assuming we'll scan exactly what we need; calculate how much
         * we'll get out based on the scale factor; if the output is too big,
         * adjust the input so we'll fit
         */
		
/*
		left_border_hinches = (BORDER_LR_MM*1000*x_denom)/ (2540*x_num);
		top_border_hinches = (BORDER_TB_MM*1000*y_denom)/ (2540*y_num);
*/
		CopyEraseMargin = db_get_var_int("CONF_SCANSP", "CopyEraseMargin", &db_retval);
		if (db_retval != FAIL) 
		{
			left_border_hinches = (CopyEraseMargin * x_denom) / (x_num);
			top_border_hinches = (CopyEraseMargin * y_denom) / (y_num);
			cdbg1("get CopyEraseMargin : %d\n", CopyEraseMargin);
		}
		else
		{
			left_border_hinches = (BORDER_LR_MM*1000*x_denom)/ (2540*x_num);
			top_border_hinches = (BORDER_TB_MM*1000*y_denom)/ (2540*y_num);
			cdbg1("ERROR get CopyEraseMargin\n");
		}
		
		g_top_padding = (FPCopyConfig->copy_resolution_y*CopyEraseMargin)/(100);
		
		scan_width_hinches -= (left_border_hinches*2);
		cdbg1("border L(%d), T(%d), W(%d), H(%d)\n", left_border_hinches, top_border_hinches, scan_width_hinches, scan_height_hinches);
        
        max_rows = scan_height_hinches;
        if( copy_started_from_adf ) {
            /* scanplat_get_max_scan_length() returns 1/100's of an inch so no
             * need to fiddle with cm
             */
             
            if (max_rows > scanplat_get_max_scan_length(SCAN_DOCUMENT_SOURCE_ADF))
				max_rows = scanplat_get_max_scan_length(SCAN_DOCUMENT_SOURCE_ADF);
        }
        else {
            /* davep 20-Mar-2007 ; WTF is the -10 for?? */
            if (max_rows > scanplat_get_max_scan_length(SCAN_DOCUMENT_SOURCE_FLATBED))
				max_rows = scanplat_get_max_scan_length(SCAN_DOCUMENT_SOURCE_FLATBED);
        }

        rows_in = calc_size( max_rows, print_height_hinches, y_num, y_denom );    
        if( rows_in < max_rows ) {
            cdbg1(  "shrinking copy scan height from %d to %d\n", max_rows, rows_in );
        } 

        max_cols = scan_width_hinches;
        cols_in = calc_size( max_cols, (print_width_hinches - (CopyEraseMargin*2)), x_num, x_denom );
        if( cols_in < max_cols ) {
            cdbg1(  "shrinking copy scan width from %d to %d\n", max_cols, cols_in );
        }
        
        if (x_num > x_denom)
			cols_in = (ALIGN32_L((cols_in*FPCopyConfig->copy_resolution_y)/100) * 100 ) / (FPCopyConfig->copy_resolution_y);
		
		g_bottom_padding = (FPCopyConfig->copy_resolution_y*(((rows_in*y_num)/y_denom)-(CopyEraseMargin))) / (100);
		
		if( copy_started_from_adf )
		{
//			x_offset = ( 850 - scan_width_hinches) / 2;	// 8.5inch : max scan width, (0.08 * 2)inch : mech margin
//			scan area  : left side of paper
			x_offset = (left_margin_hinches_x + 850) - ((850 - ((scan_size.width*100)/2540)) / 2) - left_border_hinches - cols_in;
//			cdbg1("TEST: scanvar_set_area(ADF) : %d = (%d+850) -%d(%d) - %d -%d\n", x_offset, left_margin_hinches_x, ((850 - ((scan_size.width*100)/2540)) / 2),
//									 scan_size.width, left_border_hinches, cols_in );
			if (x_offset < 0)
				x_offset = 0;
			scanvar_set_area( sv, x_offset, 0, cols_in, rows_in); 
			cdbg1("scanvar_set_area(ADF) : %d, %d, %d, %d\n", x_offset, 0, cols_in, rows_in );
//			g_fb_bottom_padding_add = BOTTOM_PADDING_OPEN;
//			g_scan_height_for_padding = rows_in;
			/* SINDOH */
			//For ADF top margin, we use margin table located in Kernel
		}
		else
		{
			/* burtp 02-Nov-2009; Don't allow flatbed scans longer than the glass or bad 
			* things will happen: the motor will stop short and the output will smear.
			*/
			int scanvar_rows_in;
			scanvar_rows_in = rows_in;
			
			if( (scanvar_rows_in + top_margin_hinches_y) > MAX_FB_HEIGHT_HINCHES_WITH_MARGIN )
			{
				cdbg2( "%s: limiting flatbed scan height of %d to %d\n", __FUNCTION__,
				rows_in, MAX_FB_HEIGHT_HINCHES_WITH_MARGIN);
				scanvar_rows_in = MAX_FB_HEIGHT_HINCHES_WITH_MARGIN - top_margin_hinches_y;
			}
			if (scanvar_rows_in > MAX_FB_HEIGHT_HINCHES)
			{
				scanvar_rows_in = MAX_FB_HEIGHT_HINCHES;
			}
			scanvar_set_area( sv, left_margin_hinches_x+left_border_hinches, top_margin_hinches_y, cols_in, scanvar_rows_in); 
			if (rows_in > scanvar_rows_in)
				g_fb_bottom_padding_add = ((FPCopyConfig->copy_resolution_y*(((rows_in*y_num)/y_denom))) / (100) )
											- ((FPCopyConfig->copy_resolution_y*(((scanvar_rows_in*y_num)/y_denom))) / (100));
				
			cdbg1("scanvar_set_area(FB) : %d, %d, %d, %d, %d\n", left_margin_hinches_x+left_border_hinches, top_margin_hinches_y, cols_in, scanvar_rows_in, g_fb_bottom_padding_add );
		}
		g_right_padding = (cols_in*FPCopyConfig->copy_resolution_y*x_num)/(100*x_denom);
		cdbg1("padding : T(%d), B(%d), R(%d)\n",g_top_padding, g_bottom_padding, g_right_padding);
    }
}

/**
 * \brief  Start setting up scanning for a copy job.
 *
 * NOTE! Can be from outside copyapp's thread context! Do NOT assume will
 * always be called from copyapp!
 *
 * This function was specifically created to allow scanapp.c to set up a scan
 * for copy-to-host (capturing copy data and sending it to the host rather than
 * the print path).
 *
 * \author David Poole
 * \date 13-Sep-2006
 *
 */

scan_err_t
copyapp_setup_scanning( struct scanvars **psv )
{
    scan_err_t scerr;
    CopyConf FPCopyConfig;
    struct scanvars *sv;
    mediasize_t scan_page_size;
    mediacnfg_t traycnfg;

    cdbg2( "%s\n", __FUNCTION__ );

    /* demand incoming pointer be NULL (sanity check for memory leaks) */
    sv = *psv;
    XASSERT( sv==NULL, (uint32_t)sv );

    //
    // Get the front panel values for copy settings.
    //
    copyapp_get_copyjob_config(&FPCopyConfig);
	
    // get the paper size and type
//    printvar_get_default_papersize(&(FPCopyConfig.printPaperSize));
	
    if(printvar_get_trayconfig(&traycnfg) == OK)
    {
       FPCopyConfig.printPaperType = MEDIATYPE_ANY;//traycnfg.mediaType[0];
    }

    // if this is not a color engine force color mode to mono
    #ifndef COLOR_ENG
    FPCopyConfig.ccmode = COPY_CMODE_MONO;
    #endif

    /* davep 03-Aug-2007 ; XXX temp debug */
    print_copyconf( &FPCopyConfig );
    
    /* davep 08-Oct-2007 ; start this global clean (XXX want to get rid of this
     * global! leftover from Dirk) 
     */
    memset( &CopySettings, 0, sizeof(CopySettings) );
#if SDH_PINETREE_YANGSAN_IC
	// add.lsh to change FB default resolution (300dpi -> 600dpi)
	if (FPCopyConfig.scan_resolution == 300 && !adf_paper_is_present() && FPCopyConfig.copyQuality != e_FILM_PHOTO &&
		FPCopyConfig.copyResize != e_A3_MERGE && FPCopyConfig.copyResize != e_A3_SEPARATE &&
		FPCopyConfig.copyResize != e_DLT_MERGE && FPCopyConfig.copyResize != e_DLT_SEPARATE &&
		FPCopyConfig.scanPaperSize != MEDIASIZE_LEGAL
		)
    {
    	dbg1( "[LSH] %s 300 -> 600", __FUNCTION__ );
        copyapp_set_fp_resolution(600);
        copyapp_get_copyjob_config(&FPCopyConfig);
    }
#endif
    scerr = setup_scan_params( &sv, &FPCopyConfig, &g_page_info );
    if( scerr != 0 ) {
        /* setup scan_params() should have logged error message */
        return scerr;
    }

    /* default duplex scan off; will be enabled below if needed */
    duplex_scan_enabled = FALSE;

    struct scan_platform_capabilities scan_capabilities;
    scancap_get_capabilities( &scan_capabilities );
    if( !scan_capabilities.has_adf ) {
        scanvar_set_document_source( sv, SCAN_DOCUMENT_SOURCE_FLATBED);
        copy_started_from_adf = FALSE;
    }
    else { 
//        /* burtp 01-Oct-2009; We are going to read the ADF sensor now, pause a little
//         *   while to try and reduce PP sensing race conditions.
//         */
//        posix_sleep_ticks( 1.25 * TIMER_INTS_PER_SECOND );

        /* Note: this function can't use copy_started_from_adf because it can be called
         * from scanapp for copy-to-host.  Just happens this is a good place to set the
         * copy_started_from_adf variable ...
         */
        if( adf_paper_is_present() ) {
            if (FPCopyConfig.scanDuplex)
            {
                scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_ADF_DUPLEX);
                duplex_scan_enabled = TRUE;
                cdbg2( "%s Enabled Duplex Scan", __FUNCTION__ );
            }
            else
            {
                scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_ADF);
            }
            copy_started_from_adf = TRUE;
        }
        else {
            scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_FLATBED);
            copy_started_from_adf = FALSE;
        }
    }

    if ( ( idCopyEnable == TRUE ) || ( FPCopyConfig.copyResize == e_A3_MERGE) || ( FPCopyConfig.copyResize == e_DLT_MERGE) 
		|| ( FPCopyConfig.copyResize == e_A3_SEPARATE) || ( FPCopyConfig.copyResize == e_DLT_SEPARATE) )
    {   // we force to run flatbed for ID copy according to SPEC
        scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_FLATBED);
        copy_started_from_adf = FALSE;
        duplex_scan_enabled = FALSE;
    }

    /* Set the print page size (system default paper size) */
    g_page_info.PageSize = FPCopyConfig.printPaperSize; 
    g_page_info.PageType = FPCopyConfig.printPaperType;
    cdbg1( "CPYAPP: Physical paper size %x\n",g_page_info.PageSize);

    /* burtp 29-Oct-2009;  At this point we have g_page_info set to match
     *  the expected printed page size, which is great.  set_scan_area needs
     *  to know the page size to determine the scan size.  Most of the time 
     *  these two match, but not always:
     *   - some scale settings may require a larger scan area (like scaling
     *     A4 to print on letter) ... but not a different print area
     *   - the flatbed glass is usually smaller than the biggest paper size
     *
     *  Start by assuming print/scan match, then handle the special cases.
     */
//    scan_page_size = g_page_info.PageSize;
    scan_page_size = FPCopyConfig.scanPaperSize;
    
    cdbg2("scan page size  From: %d\n", scan_page_size );
	//scan_page_size = MEDIASIZE_A4;
    //cdbg2("scan page size  To: %d\n", MEDIASIZE_A4 );
    
	if ((scan_page_size == MEDIASIZE_LEGAL) && (!copy_started_from_adf))
	{
		g_legal_merge = TRUE;
		scan_page_size = MEDIASIZE_LETTER;
//		g_legal_merge = FALSE;
//		scan_page_size = MEDIASIZE_A4;
//		copyapp_set_fp_resize(e_ONE_NORMAL, 100);
//		copyapp_get_copyjob_config(&FPCopyConfig);
//		scerr = scanvar_set_xyscale( sv, 1, 1, 1, 1 );
//		XASSERT( scerr==SCANERR_NONE, scerr );
	}
	else
	{
		g_legal_merge = FALSE;
	}
	g_a3_merge = FALSE;
	g_dlt_merge = FALSE;
//	copyapp_set_fp_scan_paper_size(scan_page_size);
	
    cdbg1( "copyapp: Scan page size %x\n",scan_page_size);
    set_scan_area( sv, &FPCopyConfig, scan_page_size );

 	if ( (FPCopyConfig.copyResize == e_A3_MERGE) || (FPCopyConfig.copyResize == e_DLT_MERGE) || (g_legal_merge) 
	 	|| (FPCopyConfig.copyResize == e_A3_SEPARATE) || (FPCopyConfig.copyResize == e_DLT_SEPARATE) )
 	{
		MESSAGE send_msg;
		error_type_t err;
		memset( &send_msg, 0x00, sizeof(send_msg) );

		send_msg.msgType = MSG_A3MERGE_SET_START;
		send_msg.param1 = A3MERGE_COPY;	 
		if (g_legal_merge)
			send_msg.param2 = A3MERGE_LG;
		else
		{
			if (FPCopyConfig.copyResize == e_A3_MERGE)
			{
				send_msg.param2 = A3MERGE_A3;
				g_a3_merge = TRUE;
			}
			else if  (FPCopyConfig.copyResize == e_DLT_MERGE)
			{
				send_msg.param2 = A3MERGE_DLT;
				g_dlt_merge = TRUE;
			}
			else if  (FPCopyConfig.copyResize == e_A3_SEPARATE)
			{
				send_msg.param2 = A3SEPARATE_A3;
				g_a3_merge = TRUE;
			}
			else if  (FPCopyConfig.copyResize == e_DLT_SEPARATE)
			{
				send_msg.param2 = A3SEPARATE_DLT;
				g_dlt_merge = TRUE;
			}
		}
		
		if ((FPCopyConfig.copyResize == e_A3_SEPARATE) || (FPCopyConfig.copyResize == e_DLT_SEPARATE))
		{
		    ENGMEDIASIZES print_size;
		    paper_get_media_size(FPCopyConfig.printPaperSize, &print_size.width, &print_size.height);
			send_msg.param3 = (print_size.height > print_size.width)? (void *)print_size.width : (void *)print_size.height;
		}
		
		cdbg1( "Send MSG_A3MERGE_SET_START\n");
		err = SYMsgSend(SCANMANID, &send_msg );
		XASSERT( err==OK, err );
	}
 
    PTR_ASSIGN( *psv, sv );

    return 0;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 01-Aug-2007
 *
 *  Moved some of Dirk's code into a function
 */

//static 
void update_interval_time( CURRENT_RESOURCE *job_resources, uint32_t lines_per_sheet ) 
{
#if 0
    uint32_t RemainTime;
    MESSAGE msg;
#endif
    
    /* davep 29-Oct-2008 ; disable for now because it causes the print manager
     * to spew debug messages
     */
    return;
    
#if 0
    //
    // Figure how much time is left till we are complete.
    //
//    cdbg2(  "%s lps=%ld cl=%ld\n", __FUNCTION__, lines_per_sheet, CurrentLines );

    Time += cpu_get_ccount() - IntervalTime; // add in the time elapsed.
    IntervalTime = cpu_get_ccount();  // set the start of the next time
    
    // wait for 30 to let things settle.
    if(copyapp_stats.planedatas > 30)  {
        // Figure the amount of time till we are complete.  This gives
        // the values in terms of 10msec counts.  
        RemainTime = ((((((FLOAT) Time / (FLOAT)hwGetProcSpeed())/1000000) // Time in seconds
                      /(FLOAT) CurrentLines) // average time/line
                      * (FLOAT)(lines_per_sheet- CurrentLines)) // time left 
                      *100.00); // scale to 10msec counts.
        //
        // Send the message to the next level.
        // We send one of these every 10 ticks or 100msecs.
        //
        if(abs((SINT32)LastTimeSent - (SINT32)RemainTime) > 10/*10 10msec ticks*/)
        {
            LastTimeSent = RemainTime;

            memset( &msg, 0, sizeof(MESSAGE) );
            msg.msgType = MSG_TIME_TO_END;
            msg.param1 = RemainTime;
            SYMsgSend(job_resources->Destination,&msg);
        }
    }
#endif

}


/**
 * \brief Send buffered strips when performing a duplex scan.
 */
static void copyapp_printer_send_strips(print_page_t* print_page, ATLISTENTRY* strip_list, uint32_t* current_lines, uint32_t LinesPerSheet)
{
    ATLISTENTRY* node;
    copyapp_data_strip_t* strip;
    BOOL LastStrip;
    int num_rows;

    cdbg2("%s start\n", __FUNCTION__);

    // send the page data
    while ((node = ATRemoveHeadList(strip_list)) != NULL)
    {
        strip = CONTAINING_RECORD(node, copyapp_data_strip_t, node);

        num_rows = strip->num_rows;
        LastStrip = FALSE;

        // if total num rows exceeds video Y truncate the data
        if (*current_lines + num_rows > LinesPerSheet)
        {
            num_rows = LinesPerSheet - *current_lines;
            LastStrip = TRUE;
        }

        if (num_rows > 0)
        {

			copyapp_stats.planedatas++;
            *current_lines += num_rows; // update the line counter.

//            if ((*current_lines >= g_page_info.Video_Y)
			if (*current_lines >= (LinesPerSheet-1))
            {
                LastStrip = TRUE;
            }

            if(ATIsListEmpty(strip_list) && !LastStrip)
            {
                // This is a low level warning since usually it is expected behavior because
                // we will send the buffered strips in the page before we have received the
                // final few strips at the end of a page.  -jrs
                cdbg3("%s: WARNING: strip list empty and no last strip found.\n", __func__);
            }

            copyapp_send_data(print_page, num_rows, *current_lines,
                              LastStrip, &g_page_info, strip->data,
                              SCAN_DATA_TYPE_RGB,
                              ((g_page_info.PixelsPerLine * g_page_info.BitsPerPixel)
                               / CHARSIZE) * num_rows,
                              TRUE, 0);
        }
        else
        {
            memFree(strip->data);
        }

        /* be paranoid */
        strip->data = NULL;
        DBG_ZEROMEM(strip, sizeof(copyapp_data_strip_t));
        memFree(strip);
    }

    cdbg2("%s end\n", __FUNCTION__);
}

/**
 * \brief The thread that manages the copy feature.
 *
 * Creates the software link between Scan and Print. 
 *
 *
 * \author David Poole
 * \date 08-Aug-2005
 *
 */

void* copytask(void *unused)
{
    MESSAGE msg;
    BOOL WaitingForInput;
    BOOL g_legal_merge_wait;
    uint32_t image_pixels_per_row;
    uint32_t dont_care;
    UINT pxretcode;
    BOOL printer_started;
    uint32_t BufferCount, BufferLine, BitsPerLine;
    uint32_t NumCopies;
    uint32_t media_width_mmx100, media_height_mmx100;
    ENGMEDIASIZES CopyMediaSizePixels = {0,0};
    scan_err_t scerr;
//    MESSAGE nextMsg;
    uint32_t LinesPerSheet;   ///< Number of lines per sheet.
    struct scanvars *sv;
    CURRENT_RESOURCE *job_resources;
    UINT16 num_rows = 0;
    scan_data_type dtype = SCAN_DATA_TYPE_RGB;
    UINT8 *data;
    bool last_buffer;
    uint8_t page_side;
	uint32_t top_erase[2];
	CopyConf copy_config;
	
    //wait for system init to finish
    SysWaitForInit();

    cdbg1("%s start\n", __FUNCTION__);

    /* start with clean slate */
    WaitingForInput = FALSE;
    g_legal_merge_wait = FALSE;
    image_pixels_per_row = 0;
	dont_care = 0;
    job_resources = NULL;
    printer_started = FALSE;
    BufferCount = 0;
    BufferLine = 0;
    BitsPerLine = 0;
    NumCopies = 0;
    LinesPerSheet = 0;
    sv = NULL;

    // the number of copies printed per scan during continuous copy
    continuous_mono_num_copies = 1;
    continuous_color_num_copies = 1;

    copyapp_init_copy_config();

    copyapp_log_event(COPYAPP_EVENT_IDLE);

    ATInitList(&copy_duplex_pages[0].data_strips);
    ATInitList(&copy_duplex_pages[1].data_strips);

    while (1)
    {

        // get a message
        pxretcode = posix_wait_for_message(copytask_msgq, (char *)&msg, sizeof(MESSAGE), POSIX_WAIT_FOREVER);
        XASSERT(pxretcode == 0, pxretcode);

#ifndef HAVE_FAKESCAN_SUPPORT
        //Do nothing if no scanner
        if (!scanner_is_alive())
        {
            /* bummer! */
            cdbg2("%s Scanner is NOT alive\n", __FUNCTION__);

            copyapp_log_event(COPYAPP_EVENT_COPY_OFFLINE);
            continue;
        }
#endif
        switch (msg.msgType)
        {
#if 0
#ifdef HAVE_ID_COPY
            case MSG_COPY_ID_COPY_INPUT:
                cdbg2(  "copyapp received COPY_ID_COPY_INPUT\n" );
                // process message for next step to go: 1 = OK 2 = COPY to print 0 = CANCEL
                cdbg2(  "%s copy_id copy input param1=%d\n", __FUNCTION__, msg.param1 );

                // Barf if ID copy is not enabled 
                //XASSERT( idCopyEnable==TRUE, idCopyEnable );
                //XASSERT( g_page_info.nup_image_params.ID_nup == IDNUP_ACTIVE,
                              //g_page_info.nup_image_params.ID_nup);

                if ( msg.param1 == PARAM_USER_OK )  // Next
                {
                    NumCopies++;
                    //curBuf = agGetHead(&InputBuffer);
                    g_page_info.nup_image_params.ID_nup = IDNUP_ACTIVE;

                    copyapp_log_event( COPYAPP_EVENT_COPY_RUNNING );
                    
                    /* tell upstream to start another page */
                    scanner_start_page( sv, job_resources );
                }
                else if ( msg.param1 == PARAM_USER_FINISH_JOB )
                {
                    copyapp_log_event( COPYAPP_EVENT_COPY_RUNNING );

                    // tell the nup generator that we finishing
                    // an id copy
                    copy_end_job( &sv, job_resources );
                    job_resources = NULL;
                    idCopyEnable = FALSE;
                }
                else if ( msg.param1 == PARAM_USER_CANCEL_JOB )
                {
                    // cancel the job.
                    copyapp_copy_input(e_COPY_CANCEL);
                    g_page_info.nup_image_params.ID_nup = 
                                                 IDNUP_INACTIVE;
                    idCopyEnable = FALSE;
                }
                else
                {
                    XASSERT( 0, msg.param1 );
                    idCopyEnable = FALSE;
                    g_page_info.nup_image_params.ID_nup = 
                                                 IDNUP_INACTIVE;
                }
                break;
#endif
#endif
            case MSG_STARTRECIPE:
                cdbg2("copyapp received STARTRECIPE\n");

                // canceling a copy is now valid
                copy_cancel_invalid = false;

                // Update the status to copy pending and forward
                // the message to the system job mgr.
                copyapp_log_event(COPYAPP_EVENT_COPY_START);

                msg.param2 = COPYAPPID;
                SYMsgSend(SJMID, &msg);
                break;

            case MSG_RESOURCES:
                /* be wary of memory leaks */
                XASSERT(sv == NULL, sv->id);

                job_resources = (CURRENT_RESOURCE *)msg.param3;
                cdbg2("copyapp received resources src=%d dst=%d\n",
                      job_resources->Source,
                      job_resources->Destination);

                /* We have the resources, wait for the job manager to tell us
                 * to go
                 */
                break;

            case MSG_ACKRECIPE :
                cdbg2(  "copyapp received ackrecipe\n" );

                // canceling a copy is now valid
                copy_cancel_invalid = false;

                // Update the status to copy pending and forward
                // the message to the system job mgr.
                copyapp_log_event(COPYAPP_EVENT_COPY_START);

                /* tell the scanner to start working; scanner will tell me how
                 * big the scan data will be in an internal message so I can
                 * start the printer
                 */

                number_of_scan_originals = 0;
                WaitingForInput = FALSE;
                g_legal_merge_wait = FALSE;

#if 0
#ifdef HAVE_ID_COPY
                /* See if we had an ID copy job pending, if so turn it on now
                 * that we are cleared to run.  This is a HACK and is full of
                 * possible race conditions.
                 */
                if (idCopyPending == TRUE)
                {
                    cdbg2("ID Copy was pending, enable now\n");
                    idCopyPending = FALSE;
                    idCopyEnable  = TRUE;
                }
#endif
#endif

				if (idCopyEnable == TRUE)
				{
					copyapp_set_fp_resize(e_TWO_PAGES_PER_SHEET, 100);
					copyapp_set_fp_scan_paper_size(MEDIASIZE_A5_ROTATED);
					copyapp_set_fp_print_duplex(FALSE);
				    g_page_info.nup_image_params.ID_nup = IDNUP_SDH_ACTIVE;
				}

                copyapp_stats.rows_received = 0;

                copyapp_log_event(COPYAPP_EVENT_COPY_RUNNING);

                NumCopies = 0;
                scerr = copyapp_setup_scanning(&sv);
                if (scerr != 0)
                {
                    break;
                }
                ASSERT(sv != NULL);
                scanner_start_page(sv, job_resources);

                /* we only want to start the printer once */
                /* XXX printer_started is a kludge ; fix this */
                printer_started = FALSE;

                /* Note: g_page_info is initialized in copyapp_setup_scanning()
                 * and setup_scan_params().
                 * sets num copies and collation.
                 */

                /* davep 26-Feb-2009 ; if copying from the ADF, we have to flip
                 * the data horizontally
                 */
                if (copy_started_from_adf)
                {
                    copyapp_log_event(COPYAPP_EVENT_COPY_FROM_ADF);
#ifdef HAVE_ROTATE
                    SetRotation(g_page_info.duplexInfo, e_Rotate180);
#endif
                }

                break;

            case MSG_SCAN_SIZE :
                cdbg2("copyapp received scan_size\n");

                memset( &copy_config, 0, sizeof(copy_config) );
			    copyapp_get_copy_config(&copy_config);
			    
                /* davep 06-Sep-2005 ; receive data from scan telling me how
                 * big the data will be once all DMA requirements met, scaling
                 * applied, etc.
                 */

                /* We will be getting this scan size message after every start
                 * page message but we only want to startjob the printer once.
                 */
                /* XXX printer_started is a kludge ; fix this */
                if (printer_started)
                {
                    break;
                }

                //
                // Initialize the variables used to calculate how much is left.
                //
                Time = 0;
                copy_duplex_pages[0].current_lines = copy_duplex_pages[1].current_lines = 0;
                IntervalTime = 0;
                LastTimeSent = 0;
                copyapp_stats.planedatas = 0;

                /* Don't care about the unpadded pixels_per_row since we only want the
                 * real width of the data. The scan subsystem will have padded
                 * out our data with white.
                 */
                msg_scan_size_decode(&msg, &image_pixels_per_row, &dont_care, &LinesPerSheet,
                                     (uint32_t *)&g_page_info.BitsPerPixel);

                cdbg1( "%s ppr=%d pprp=%d tr=%d bpp=%d\n", __FUNCTION__,
                    dont_care, image_pixels_per_row, LinesPerSheet,  g_page_info.BitsPerPixel);

				if (g_legal_merge != TRUE)
				{
					long int x, y, width, height;
					int x_num, x_denom, y_num, y_denom;
					
					scanvar_get_area(sv, &x, &y, &width, &height);
					scanvar_get_xyscale(sv, &x_num, &x_denom, &y_num, &y_denom);
					
					LinesPerSheet = (height * g_page_info.Y_Resolution * y_num ) / (100 * y_denom);
				}
				
				if (((LinesPerSheet%64) == 1) && (copy_config.scan_resolution == 600))
					LinesPerSheet--;
					
				cdbg1( "%s modified trr=%d\n", __FUNCTION__, LinesPerSheet);
				
                g_page_info.PixelsPerLine = image_pixels_per_row;

                memset(&CopyMediaSizePixels, 0, sizeof(ENGMEDIASIZES));
                paper_get_media_size(g_page_info.PageSize,  &media_width_mmx100, &media_height_mmx100);
                CopyMediaSizePixels.width = MMx100_TO_DOTS(media_width_mmx100, g_page_info.X_Resolution);
                CopyMediaSizePixels.height = MMx100_TO_DOTS(media_height_mmx100, g_page_info.Y_Resolution);

// video laser constrain... if (last strip size == 1) then ..... dead....
				if ((CopyMediaSizePixels.height%64) == 1)
					CopyMediaSizePixels.height--;
				
                // adjust for PIXEL_PAD pixel boundary
                if(CopyMediaSizePixels.width % ((PIXEL_PAD*g_page_info.Y_Resolution) / 600) != 0)
                {
                    CopyMediaSizePixels.width -= 
                            CopyMediaSizePixels.width % ((PIXEL_PAD*g_page_info.Y_Resolution) / 600);
                }

                print_media_size(&CopyMediaSizePixels);
                print_page_info(&g_page_info);

                copyapp_printer_start_job( job_resources, 
                                 CopyMediaSizePixels.width,
                                 CopyMediaSizePixels.height,  /* total rows */
                                 image_pixels_per_row, 
                                 (int)LinesPerSheet,
                                 &g_page_info  );

                /* XXX printer_started is a kludge ; fix this */
                printer_started = TRUE;
                break;

            case MSG_SCAN_PAGESTART :
                copyapp_stats.pagestarts++;
                msg_scan_page_start_decode( &msg, &page_side );
                XASSERT(page_side < 2, page_side);

                cdbg2("copyapp received pagestart side %d\n", page_side);

                copy_duplex_pages[page_side].current_lines = 0;
				top_erase[page_side] = g_top_erase;
				
                if(N_up == 1) 
                {
                    number_of_scan_originals++;
                }
                else
                {
                    if (copyapp_stats.pagestarts % N_up == 1)
                    {
                        number_of_scan_originals++;
                    }
                }
//-jrs                copyapp_log_event( COPYAPP_EVENT_COPY_PAGE_START );

                /* davep 09-Sep-2009 ; Check for low memory. Note I'm only doing
                 * this if collation is enabled.
                 */
                if (g_page_info.collate &&
                    memGetHeapFreeMemorySize(NULL) < COLLATE_REQUIRE_MEMORY_FOR_NEXT_PAGE)
                {
                    copyapp_OOM();
                }

                ASSERT(page_side == 0 || duplex_scan_enabled);  // if page side != 0 must be doing duplex scan
                ASSERT(copy_duplex_pages[page_side].print_page == NULL);
                ASSERT(copy_duplex_pages[page_side].print_page_active == FALSE);
                ASSERT(ATIsListEmpty(&copy_duplex_pages[page_side].data_strips));
                ATInitList(&copy_duplex_pages[page_side].data_strips);

                // create the page
                copy_duplex_pages[page_side].print_page = copyapp_printer_create_page(&g_page_info);
                print_page_set_image_height_lines(copy_duplex_pages[page_side].print_page, LinesPerSheet+g_fb_bottom_padding_add);
                ASSERT(copy_duplex_pages[page_side].print_page);

                // if the opposite page is not currently active start this page
                if (!copy_duplex_pages[PAGE_OTHER_SIDE(page_side)].print_page_active)
                {
					copy_duplex_pages[page_side].print_page->page_data->image_output_info.image_scale_right_padding = image_pixels_per_row - dont_care;
					copy_duplex_pages[page_side].print_page->page_data->image_output_info.image_top_padding = g_top_erase;
                    copyapp_printer_start_page(copy_duplex_pages[page_side].print_page, &g_page_info);
                    copy_duplex_pages[page_side].print_page_active = TRUE;
                }
                break; 

            case MSG_SCAN_PLANEDATA :

                msg_scan_planedata_decode(&msg, &num_rows, &dtype, &data, &last_buffer);

                msg_scan_planedata_get_page_side( &msg, &page_side );
                XASSERT(page_side < 2, page_side);

                /* poison the msg so we can catch someone below using it */
                memset(&msg, 0xaa, sizeof(MESSAGE));

/*	fault in rotation case.... (SEF->LEF)
                if ((copy_duplex_pages[page_side].current_lines >= CopyMediaSizePixels.height) ||
                    (num_rows == 0))
                {
                    cdbg2("side=%d rows=%d media=%d discard data past page bottom border\n",
                          page_side, copy_duplex_pages[page_side].current_lines, CopyMediaSizePixels.height);
                    memFree(data);
                    break;
                }
                else
*/
                
                if (copy_duplex_pages[page_side].current_lines >= (LinesPerSheet-1))
	            {
	                cdbg2("side=%d rows=%d LPS=%d discard data past page bottom border\n",
                          page_side, copy_duplex_pages[page_side].current_lines, LinesPerSheet);
                    memFree(data);
                    break;
                }

                //if(last_buffer)
                {
                    // if total num rows exceeds video Y truncate the data
/*	fault in rotation case.... (SEF->LEF)
                    if (copy_duplex_pages[page_side].current_lines + num_rows >= g_page_info.Video_Y)
                    {
                        cdbg2("copyapp truncating planedata from %d rows to %d rows, last = %d\n", num_rows, g_page_info.Video_Y - copy_duplex_pages[page_side].current_lines, last_buffer);
                        num_rows = g_page_info.Video_Y - copy_duplex_pages[page_side].current_lines;
                        last_buffer = true;
                    }
                    else 
*/
                    if (copy_duplex_pages[page_side].current_lines+ num_rows >= LinesPerSheet)
                    {
                        cdbg2("copyapp truncating planedata2 from %d rows to %d rows, last = %d\n", num_rows,LinesPerSheet - copy_duplex_pages[page_side].current_lines, last_buffer);
                        num_rows = LinesPerSheet - copy_duplex_pages[page_side].current_lines;
                        last_buffer = true;
					} 
                    cdbg3("copyapp received planedata side=%d rows=%d dtype=%d last=%d data=%p Video_Y=%d/%d\n",
                          page_side, num_rows, dtype, last_buffer, data, copy_duplex_pages[page_side].current_lines + num_rows, g_page_info.Video_Y);
                }

                if (N_up != 1)
                {
                    // The idea of buffer count is that we can put the data into the
                    // memory as we go.  We don't need to do this since we have enough
                    // memory.  So I set it to 3000 to take the entire page.
                    //
                    if ((BufferCount++ > 3000) || last_buffer)
//                    if((BufferCount++ > 3000) || ( PLANEDATA_FLAG_LASTSTRIP(msg.param2)))
                    {
                        /* davep 06-Aug-2009 ; do we hit this code? */
                        XASSERT(BufferCount < 3000, BufferCount);

                        /* start over capturing data buffers */
                        BufferLine = 0;         // reset the number of lines in the buffer.
                        BufferCount = 0;
                    }
                }
                else
                {
                    // This next section figures the amount of time left till end of job
                    // this is done by figuring the average time per line and
                    // multiplying that by the number of lines left.  This is then
                    // send to the next module.
                    //
#if 0
                    if(CurrentLines == 0)
                    {
                        // This is the first time through.  Initialize the time
                        // and go on.
                        IntervalTime = cpu_get_ccount();
                    } else
                    {
                        update_interval_time( job_resources, LinesPerSheet );
                    }
#endif
                }

                if ((last_buffer) && (num_rows == 1))
	            {
	                cdbg2("last buffer and num_rows is 1... So skip\n");
                    memFree(data);
                    break;
                }
				// top erase for ADF & rotation case
				if (top_erase[page_side])
				{
					if (top_erase[page_side] >= num_rows)
					{
						memset(data, 0xFF, num_rows * g_page_info.PixelsPerLine);
						cdbg2("top erase lines1(%d)\n", num_rows);
						top_erase[page_side] -= num_rows;
					}
					else
					{
						memset(data, 0xFF, top_erase[page_side] * g_page_info.PixelsPerLine);
						cdbg2("top erase lines2(%d)\n", top_erase[page_side]);
						top_erase[page_side] = 0;
					}
				}
                // if this is the active page side send the data on to the printer
                // otherwise buffer it to be sent once the currently active side is complete
                if (copy_duplex_pages[page_side].print_page_active)
                {
                    copyapp_stats.planedatas++;
                    copy_duplex_pages[page_side].current_lines += num_rows; // update the line counter.

                    /* davep 28-Oct-2009 ;  */
/*	fault in rotation case.... (SEF->LEF)
                    if ((copy_duplex_pages[page_side].current_lines >= CopyMediaSizePixels.height) ||
                        (copy_duplex_pages[page_side].current_lines >= g_page_info.Video_Y))

                    {
                        cdbg2("side = %d, rows=%d media=%d early trigger last buffer\n",
                              page_side, copy_duplex_pages[page_side].current_lines, CopyMediaSizePixels.height);
                        last_buffer = true;
                    }
                    else 
*/
                    if (copy_duplex_pages[page_side].current_lines >= (LinesPerSheet-1))
                    {
                        cdbg2("side = %d, current_lines=%d expected_scan_rows=%d set last_buffer\n",
                              page_side, copy_duplex_pages[page_side].current_lines, LinesPerSheet);
                        if (copy_duplex_pages[page_side].current_lines > LinesPerSheet)
                        {
						   cdbg2("change num_rows %d -> %d\n", num_rows, num_rows - (copy_duplex_pages[page_side].current_lines - LinesPerSheet));
                           num_rows = num_rows - (copy_duplex_pages[page_side].current_lines - LinesPerSheet);
                        }
                        last_buffer = true;
                    }

                    copyapp_send_data(copy_duplex_pages[page_side].print_page, num_rows, copy_duplex_pages[page_side].current_lines,
                                      last_buffer,
                                      &g_page_info, data,
                                      dtype,  /* formerly the comp datatype */
                                      ((g_page_info.PixelsPerLine * g_page_info.BitsPerPixel) / CHARSIZE) * num_rows,
                                      TRUE, NumCopies);
                } 
                else
                {
                    copyapp_data_strip_t* strip;

                    // shouldn't need to buffer unless doing duplex scan
                    ASSERT(duplex_scan_enabled);

                    strip = (copyapp_data_strip_t*)MEM_MALLOC_LIMITED(mlimiter_by_name("print"), sizeof(copyapp_data_strip_t));
                    ASSERT(strip != NULL);

                    PTR_ASSIGN(strip->data, data);
                    strip->num_rows = num_rows;
                    ATInsertTailList(&copy_duplex_pages[page_side].data_strips, &strip->node);
                }

                break; 

            case MSG_SCAN_PAGEEND :
                msg_scan_page_end_decode( &msg, &page_side );
                XASSERT(page_side < 2, page_side);

                cdbg2("copyapp received pageend side %d\n", page_side);

                /* Keep track of the actual number of scans copy has done (even if ID 
                 * copy does multiple scans, they are still scans)
                 */
//                increment_copy_scanned_page_count(copy_started_from_adf);

#if 0
#ifdef HAVE_ID_COPY
                /* we'll wait for the JOBEND_EX, thank you. */
                if (idCopyEnable == TRUE)
                {
                    number_of_scan_originals = 1;
                    break;
                }
#endif
#endif
                if ((N_up != 1) || ((!copy_started_from_adf) && (g_page_info.duplexInfo != DMDUP_SIMPLEX)))
                {
                    NumCopies++;

                    cdbg2("%s NumCopies=%d N_up=%d\n",
                          __FUNCTION__, NumCopies, N_up);
                }

                cdbg2("copyapp pageend completing side %d\n", page_side);
                // we always expect to receive the page end for the active side
                ASSERT(copy_duplex_pages[page_side].print_page_active);
                ASSERT(page_side == 0 || duplex_scan_enabled);  // if page side != 0 must be doing duplex scan
                ASSERT(copy_duplex_pages[page_side].print_page != NULL);
                ASSERT(copy_duplex_pages[PAGE_OTHER_SIDE(page_side)].print_page_active == FALSE);
                ASSERT(ATIsListEmpty(&copy_duplex_pages[page_side].data_strips));

                // there probably aren't any buffered strips, but try to send just in case
                copyapp_printer_send_strips(copy_duplex_pages[page_side].print_page, &copy_duplex_pages[page_side].data_strips, &copy_duplex_pages[page_side].current_lines, LinesPerSheet);
                copyapp_printer_end_page(copy_duplex_pages[page_side].print_page, &g_page_info);

                // this page is no longer active, so clear it out
                copy_duplex_pages[page_side].print_page = NULL;
                copy_duplex_pages[page_side].print_page_active = FALSE;
                copy_duplex_pages[page_side].current_lines = 0;

                // now check to see if the other page side has been created and if so
                // send the start and any buffered data
                page_side = PAGE_OTHER_SIDE(page_side);  // switch to other page side
                if (copy_duplex_pages[page_side].print_page != NULL)
                {
                    // shouldn't need to process other side unless doing duplex scan
                    ASSERT(duplex_scan_enabled);

                    cdbg2("copyapp pageend starting side %d\n", page_side);

                    // send the start
					copy_duplex_pages[page_side].print_page->page_data->image_output_info.image_scale_right_padding = image_pixels_per_row - dont_care;
					copy_duplex_pages[page_side].print_page->page_data->image_output_info.image_top_padding = g_top_erase;
                    copyapp_printer_start_page(copy_duplex_pages[page_side].print_page, &g_page_info);
                    copy_duplex_pages[page_side].print_page_active = TRUE;

                    // send any buffered strips
                    copyapp_printer_send_strips(copy_duplex_pages[page_side].print_page, &copy_duplex_pages[page_side].data_strips, &copy_duplex_pages[page_side].current_lines, LinesPerSheet);
                }

                if (NumCopies == N_up)
                {
                    /* Setting NumCopies to zero here prevents copyapp from ending a 
                     * flatbed job
                     */
                    if (copy_started_from_adf)
                    {
                        NumCopies = 0;
                    }
                }
                break;

            case MSG_SCAN_JOBEND:
                cdbg2("copyapp received jobend\n");

                /* davep 11-Nov-2008 ; we'll get back the scanvar pointer.
                 * Should be the same pointer we send down.
                 */
                struct scanvars *jobend_sv;
                ASSERT(msg.param3 != NULL);
                PTR_ASSIGN(jobend_sv, msg.param3);
                XASSERT(jobend_sv->cookie == SCANVAR_COOKIE, (uint32_t)jobend_sv);
                XASSERT(jobend_sv == sv, (uint32_t)jobend_sv);
                jobend_sv = NULL;

                copyapp_stats.pagestarts = 0; //reset it to be 0 for new jobs.

                /* davep 26-Jun-2009 ; when we're scanning from flatbed, we'll
                 * receive MSG_SCAN_PAGEEND then MSG_SCAN_JOBEND.  (ID Copy forces a
                 * scan from the flatbed)
                 */
#if 0
#ifdef HAVE_ID_COPY
                g_page_info.nup_image_params.ID_nup = IDNUP_INACTIVE;
                if (idCopyEnable == TRUE)
                {   //  1 page scan is finished. prompt user to select scan another or print
                    cdbg2("%s idcopy prompt user for another page\n", __FUNCTION__);
                    copyapp_log_event(COPYAPP_EVENT_WAITING_FOR_USER_INPUT);
                    break;
                }
#endif
#endif
                if (N_up == 1)
                {
					if ((!copy_started_from_adf) && (g_page_info.duplexInfo != DMDUP_SIMPLEX) && (NumCopies<2))
					{
						cdbg2("FB scan for duplex print\n");
						copyapp_log_event(COPYAPP_EVENT_WAITING_FOR_USER_INPUT);
						WaitingForInput = TRUE;
					}
					else
                    {
						copy_end_job(&sv, job_resources);
						job_resources = NULL;
	                    ASSERT(sv == NULL);
	                }
                } 
                else
                {
                    /* davep 01-Aug-2007 ; if we don't have any more pages,
                     * pretend we have all we need 
                     */
                     if (copy_started_from_adf)
                     {
                        cdbg2("no more pages for N-up so we're done\n");
                        
                        copy_end_job( &sv, job_resources );
                        job_resources = NULL;
                        ASSERT(sv == NULL);
                    } 
                    else if (NumCopies < (N_up * ((g_page_info.duplexInfo == DMDUP_SIMPLEX)?1:2)))
                    {
                        /* scanning from flatbed, we don't have all the pages we
                         * need, so prompt for next page  (see MSG_USER_INPUT
                         * for next step)
                         */
                        copyapp_log_event(COPYAPP_EVENT_WAITING_FOR_USER_INPUT);

                        WaitingForInput = TRUE;
                        // if this is a copy_to_host job start
                        // the next page automatically
#ifdef HAVE_COPY_TO_HOST
                        if (copy_to_host_job_active())
                        {
                            cdbg2("----CopyToHost: Automatically starting next job\n");
                            copyapp_continue_job();
                        }
#endif
                    } 
                    else
                    {
                        /* our N-Up job is done so free the scanvar */
                        dbg2("======copy_end_job....\n");
                        copy_end_job(&sv, job_resources);
                        job_resources = NULL;
                        ASSERT(sv == NULL);
                    }
                }
                break;

            case MSG_USER_INPUT:
                cdbg2("copyapp received user_input\n");
                // We have the user input, put us back on-line

                if (!WaitingForInput)
                {
                    break;          // we should not be here, get out
                }
                WaitingForInput = FALSE;

                //
                // Change status to copying.
                //
                if (msg.param1 == PARAM_USER_OK)
                {
                    copyapp_log_event(COPYAPP_EVENT_COPY_RUNNING);

                    // We are ready to go with the next copy.
                    ASSERT(sv != NULL);

					if (g_legal_merge)
					{
						long int x, y, width, height;
						
						scanvar_get_area(sv, &x, &y, &width, &height);
						if (g_legal_merge_wait)
						{
							g_legal_merge_wait = FALSE;
							y+=445;
						}
						else
						{
							MESSAGE send_msg;
							error_type_t err;
							memset( &send_msg, 0x00, sizeof(send_msg) );

							send_msg.msgType = MSG_A3MERGE_SET_START;
							send_msg.param1 = A3MERGE_COPY;	 
							send_msg.param2 = A3MERGE_LG;
							
							cdbg1( "Send MSG_A3MERGE_SET_START\n");
							err = SYMsgSend(SCANMANID, &send_msg );
							XASSERT( err==OK, err );
						
							y-=445;
							height = 725;
							
							if (g_page_info.PageSize == MEDIASIZE_LETTER)
								height = 730;
						}
						if ((height+y) > MAX_FB_HEIGHT_HINCHES_WITH_MARGIN)
						{
							cdbg2( "%s: limiting flatbed scan height of %d to %d\n", __FUNCTION__,
								height, MAX_FB_HEIGHT_HINCHES_WITH_MARGIN - y);
							height = MAX_FB_HEIGHT_HINCHES_WITH_MARGIN - y;
						}
						scanvar_set_area(sv, (int)x, (int)y, (int)width, (int)height);
					}
					else if (g_a3_merge || g_dlt_merge)
					{
						MESSAGE send_msg;
						error_type_t err;
						memset( &send_msg, 0x00, sizeof(send_msg) );

						send_msg.msgType = MSG_A3MERGE_SET_START;
						send_msg.param1 = A3MERGE_COPY;	 
						if (g_a3_merge)
							send_msg.param2 = A3MERGE_A3;
						else
							send_msg.param2 = A3MERGE_DLT;
						
						cdbg1( "Send MSG_A3MERGE_SET_START\n");
						err = SYMsgSend(SCANMANID, &send_msg );
						XASSERT( err==OK, err );
					}
                    scanner_start_page(sv, job_resources);
                    break;
                } 
                else if (msg.param1 == PARAM_USER_FINISH_JOB)
                {
                    // tell the nup generator that we are finishing
                    // an nup copy prematurely
                    copy_end_job( &sv, job_resources );
                    job_resources = NULL;
                    break;
                }

                //
                // See if this user input was correct.  If not handle it.
                //
                else
                {
                    //
                    // Change status to canceling.
                    //
                    copyapp_log_event(COPYAPP_EVENT_CANCEL);

                    //
                    // We did not get an ok, cancel the job.
                    //
                    copyapp_copy_input(e_COPY_CANCEL);
                    break;
                }
                //

            case MSG_CANCELJOB :
                cdbg2("copyapp received canceljob\n");
#if 0
#ifdef HAVE_ID_COPY
                if (idCopyEnable == TRUE)
                {
                    //Clear the ID copy var. ID copy is one-shot
                    idCopyEnable = FALSE;
                }
#endif
#endif
                g_page_info.nup_image_params.ID_nup = IDNUP_INACTIVE;
                if (job_resources != NULL)
                    copy_cancel(msg.param3, job_resources);
                    
                job_resources = NULL;
                if (sv != NULL)
                {
                    scanvar_delete(&sv);
                }
                break;
                
			case MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START :
				cdbg2("copyapp received MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START\n");
				if (g_legal_merge)
				{
					cdbg2("FB scan for Legal scan\n");
					copyapp_log_event(COPYAPP_EVENT_WAITING_FOR_USER_INPUT_MERGE);
					WaitingForInput = TRUE;
					g_legal_merge_wait = TRUE;
				}
				break;
				
            default:
                /* ignore */
                cdbg2("copyapp ignore unknown message type=%#x param1=%#x\n",
                      msg.msgType, msg.param1);
                break;
        }
    }
    return 0;
}

/**
 * \brief Initialize the Copy Application.
 *
 * Called from main() before threading runs to do pre-OS setup of the Copy
 * Application.
 *
 * \author David Poole
 * \date 19-Aug-2005
 *
 */

void CopyAppCmdInit( void );
void CopyAppInit( void )
{
    UINT pxretcode;

    cdbg2( "CopyAppInit...\n");

    /* create the queue */
    posix_create_message_queue( &copytask_msgq, "/copyapp",  
                                            COPYTASK_QUEUE_SIZE, sizeof(MESSAGE));    

    router_register_queue(COPYAPPID, copytask_msgq);  // Register a message queue.

    //
    // Register the resource
    rm_register(e_CopyApp, COPYAPPID);

    copyapp_observer_constructor();

    // start the scan task
    pxretcode = posix_create_thread( &copytask_id, &copytask, 0, "Copy_App",  copytask_stack,
                     COPYTASK_STACK_SIZE, POSIX_THR_PRI_NORMAL );
    XASSERT(pxretcode==0, pxretcode);

    /* davep 16-Aug-2011 ; create the continuous copy thread */
    copy_continuous_onetime_init();
 
    CopyAppCmdInit();
}
 
