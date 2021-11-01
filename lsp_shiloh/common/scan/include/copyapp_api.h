/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef COPYAPP_API_H
#define COPYAPP_API_H

#include "print_job_types.h"
#include "response_subject.h"
#include "SysUI_Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/* davep 20-Sep-2010 ; RIP Status Manager. Replace with something new. */
typedef enum {
    COPYAPP_EVENT_NULL=0,  /* invalid/uninitialized */
    COPYAPP_EVENT_IDLE=1,
    COPYAPP_EVENT_COPY_START=2,
    COPYAPP_EVENT_COPY_PAGE_START=3,
    COPYAPP_EVENT_COPY_RUNNING=4,
    COPYAPP_EVENT_COPY_COMPLETE=5,
    COPYAPP_EVENT_CANCEL=6,
    COPYAPP_EVENT_OUT_OF_MEMORY=7,
    COPYAPP_EVENT_WAITING_FOR_USER_INPUT=8,
    COPYAPP_EVENT_COPY_OFFLINE=9,
    COPYAPP_EVENT_COPY_FROM_ADF=10,
    COPYAPP_EVENT_WAITING_FOR_USER_INPUT_MERGE=11,
    COPYAPP_EVENT_PARAM_FAULT=12,
    /* more later */
} copyapp_event_t;

typedef struct copyapp_observer_data_s
{
    Response_Subject_T_BaseClassMembers;
    copyapp_event_t event;
    uint32_t number_of_scans;
} copyapp_observer_data_t;

typedef enum
{
	e_COPY_FUNCTION_OFF = 0,
	e_COPY_FUNCTION_ON,
	e_COPY_FUNCTION_ON_OFF_MAX
}e_COPY_FUNCTION_ON_OFF;

typedef enum
{
    e_PORTRAIT = 0,
    e_LANDSCAPE
}e_ORIENTATION; ///< Orientation 

typedef enum
{
//    e_ORG_100PCNT = 0,
    e_ONE_NORMAL = 0,
    e_TWO_PAGES_PER_SHEET, 
    e_FOUR_PAGES_PER_SHEET,
	e_ID_COPY,					// IPC used up to 3
    e_A3_MERGE,					// A3 merge
    e_DLT_MERGE,				// DLT merge
	e_A3_SEPARATE,				// A3 Sepatare
    e_DLT_SEPARATE,				// DLT merge
	e_CUSTOM_PCNT,
	e_CUSTOM_REGION,
	e_ORG_100PCNT,
    e_A4_2_LTR_94PCNT,
    e_LTR_2_A4_97PCNT,
    e_FULL_PAGE_91PCNT,
    e_LEGAL_2_LTR_78PCNT,
    e_LEGAL_2_A4_83PCNT			// copy app cmd used up to this point
}e_COPYRESIZE; ///< Copy Resize 

/* DO NOT CHANGE THESE NUMBERS!!!
 *
 * These values are used in a our scanning protocol for copy-to-host.
 *
 * Add to the end if you need to add another copy quality mode.
 */
typedef enum{
    e_TEXT = 0,
    e_DRAFT = 1,
    e_MIXED = 2,
    e_FILM_PHOTO = 3,
    e_PICTURE = 4,

    /* davep 01-Oct-2007 ; adding the fax modes for copy-to-host; only for
     * testing/debugging so users shouldn't see these in the control panel
     */
    e_FAX_TEXT = 7, 
    e_FAX_MIXED = 8,

    /* MUST BE LAST! (Used for range checking) */
    e_COPYQUALITY_MAX
}e_COPYQUALITY; ///< Copy Quality

typedef enum{
    e_SCANQUALITY_NORMAL = 0,
    e_SCANQUALITY_HIGH = 1,

    /* MUST BE LAST! (Used for range checking) */
    e_SCANQUALITY_MAX
}e_SCANQUALITY; ///< Scan Quality of Copy mode

/*  replace with more generic e_COPY
typedef enum
{
    e_IDCOPY_NEXT = 0,
    e_IDCOPY_FINISH = 1,
    e_IDCOPY_CANCEL = 2
}e_IDCOPY; ///< CopyID 
*/

typedef enum
{
    e_COPY_NEXT = 0,
    e_COPY_FINISH = 1,
    e_COPY_CANCEL = 2
}e_COPY; 

typedef enum
{
    e_COPY_INTRAY_MPT = 0,
    e_COPY_INTRAY_TRAY1,
    e_COPY_INTRAY_TRAY2,
    e_COPY_INTRAY_TRAY3,
    e_COPY_INTRAY_MAX
}e_COPYOUTTRAY; 

typedef enum
{
	e_COPY_FIND_TRAY_SCOPE_A4 = 0,
	e_COPY_FIND_TRAY_SCOPE_A4_LT,
	e_COPY_FIND_TRAY_SCOPE_A4_LT_LG,
	e_COPY_FIND_TRAY_SCOPE_LT,
	e_COPY_FIND_TRAY_SCOPE_LT_A4,
	e_COPY_FIND_TRAY_SCOPE_LT_LG_A4,
	e_COPY_FIND_TRAY_SCOPE_LG_LT_A4,
	e_COPY_FIND_TRAY_SCOPE_MAX
}e_COPY_FIND_TRAY_SCOPE;

#define e_COPY_INTRAY_AUTO	8

/** maximum and min values for CopyConfig.background settings */
#define COPY_BACKGROUND_MAX  9
#define COPY_BACKGROUND_MIN  0

/** maximum and min values for CopyConfig.coloradj settings */
#define COPY_COLORADJUST_MAX  11
#define COPY_COLORADJUST_MIN   1

/** maximum and min values for CopyConfig.copyBrightness
 *  settings */
#define COPY_BRIGHTNESS_MAX  9
#define COPY_BRIGHTNESS_MIN  1

/** maximum and min values for CopyConfig.copyContrast settings */
#define COPY_CONTRAST_MAX  5
#define COPY_CONTRAST_MIN  1

/** maximum and min values for CopyConfig.sharp settings */
#define COPY_SHARP_MAX   11
#define COPY_SHARP_MIN    1

#define COPY_SCALE_AUTO	  0
#define COPY_SCALE_MAX	200
#define COPY_SCALE_MIN	 50
#define COPY_SCALE_ERROR 255

#define COPY_BORDERING_OFF	  0
#define COPY_BORDERING_MAX	100
#define COPY_BORDERING_MIN	  1

#define COPY_MAX_NUM_OF_COPY	99

typedef struct ScanRegionTag
{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
} ScanRegion;

/* davep 02-Dec-2011 ; finally adding mono vs color copy */
typedef enum {
    COPY_CMODE_MONO=1,  ///< copy output mono
    COPY_CMODE_COLOR    ///< copy output color (not available on all models; ask your doctor)
} copy_cmode_t;

typedef struct 
{
    int copyBrightness; ///< 1-11      TODO: Support in copyapp.c
    int copyContrast;   ///< 1-11
    int copySharp;      ///< 0-4      TODO: Support in copyapp.c  
    e_COPYQUALITY copyQuality;
    copy_cmode_t ccmode;
    PRINT_QUALITY printQuality;
    int copyNumber;     ///< 1-99
    e_COPYRESIZE copyResize;      
    e_ORIENTATION Orientation2p;  
    e_ORIENTATION Orientation4p;  
    int ResizePercentage;///<  25- 400
    mediatype_t printPaperType;
    mediasize_t printPaperSize;
    mediasize_t printTrayPaperInfo[4];
    mediasize_t scanPaperSize;
    BOOL copyCollation;
    trayenum_t inputTray;
    UINT16 printDuplex;
    BOOL scanDuplex;
    BOOL printBorderless;
    ScanRegion region;
    int redAdjust;
    int blueAdjust;
    int greenAdjust;
    int copyBackground;
    int copy_resolution_x;
    int copy_resolution_y;
    int scan_resolution;
    int darkness;
} CopyConf;  

UINT8 copyapp_check_param_valid(stCopy_GoCopy stGoCopy);
UINT8 copyapp_check_nUP_valid(UINT16 scanPaperSize, UINT16 printPaperSize);
BOOL copyapp_check_adf_paper(void);

void copyapp_start_job( void );
void copyapp_cancel_job( void );
void copyapp_continue_job( void );
void copyapp_copy_input( uint32_t action );

UINT8 copyapp_find_tray(UINT16 doc_code, UINT16 *tray_code, UINT8 scale, ROTATE_DIR *rot, UINT16 *paper_code);
UINT8 copyapp_find_tray_A4_LT_LG(UINT16 *tray_code, UINT8 scope);
UINT8 copyapp_find_scale(UINT16 scanPaperSize, UINT16 printPaperSize, ROTATE_DIR *rot);

void copyapp_set_fp_rotation( ROTATE_DIR copy_rotation );
void copyapp_set_fp_quality( e_COPYQUALITY new_quality, copy_cmode_t ccmode );
void copyapp_set_fp_colormode( copy_cmode_t ccmode );
void copyapp_set_fp_qualitymode( e_COPYQUALITY new_quality );
void copyapp_set_fp_resize( e_COPYRESIZE resize_var, uint32_t custom_resize );
void copyapp_set_fp_num_copies( int num_copies );
void copyapp_set_fp_brightness( int brightness );
void copyapp_set_fp_contrast( int contrast );
void copyapp_set_fp_sharpness( int sharpness );
void copyapp_set_fp_print_quality( PRINT_QUALITY print_quality );
void copyapp_set_fp_input_tray( trayenum_t input_tray );
void copyapp_set_fp_print_paper_size_info( UINT16 *tray_code );
void copyapp_set_fp_collation( BOOL enable );
void copyapp_set_fp_print_duplex( BOOL enable );
void copyapp_set_fp_scan_duplex( BOOL enable );
void copyapp_set_fp_color_balance( int r, int g, int b );
void copyapp_set_fp_background( int background );
void copyapp_set_fp_paper_size( mediasize_t paper_size );
void copyapp_set_fp_resolution( int dpi );
void copyapp_get_copyjob_config( CopyConf *conf ); // get perjob values
void copyapp_get_copy_config( CopyConf *conf ); // get nvram values
void copyapp_set_copyjob_config( CopyConf *conf ); // set perjob values
void copyapp_set_copy_config( CopyConf *conf );  // set nvram values
void copyapp_set_copy_config_defaults( void );  // set default nvram values
void copyapp_set_fp_print_paper_size( mediasize_t papersize );
void copyapp_set_fp_scan_paper_size( mediasize_t papersize );
void copyapp_set_fp_print_density( int density );
void copyapp_set_id_copy(bool val);

#ifdef HAVE_ID_COPY
void copyapp_id_copy_start_job( void );
void copyapp_id_copy_input( uint32_t action );
#endif
void copy_continuous_start( void );
void copy_continuous_stop( void );

void copyapp_set_fp_orientation( e_ORIENTATION orientation );
bool is_scanner_alive();

/* observer APIs */
#define COPYAPP_OBSERVER_ID 0x636f7079

void copyapp_observer_attach( Observer_t *o );

void copyapp_observer_detach( Observer_t *o );

copyapp_event_t copyapp_observer_get_event( Subject_t *s );

#ifdef __cplusplus
}
#endif

#endif

