/*
 *
 * ============================================================================
 * Copyright (c) 2008-2013   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/**
 *
 * \file print_job_types.h
 *
 * \brief Declarations of print job attributes.
 *
 * The job structure is maintained as a series of linked lists.  The list heads are stored in
 * each section.  There are job nodes.  There is only a single job node per job.  Then a
 * sheet node.  There are as many sheet nodes as there are pages in the job.  And there are
 * page nodes.  There are 1 page per sheet single sided or 2 pages/sheet duplex.  Data planes
 * are stored for each plane.
 *
 */


#ifndef PRINT_JOB_TYPES_H
#define PRINT_JOB_TYPES_H

#include <stdbool.h>

#include "list.h"
#include "agConnectMgr.h"
#include "agprint.h"
#include "agRouter.h"
#include "BigBuffer.h"
#include "paper_size_config.h"
#include "paper_types_config.h"
#include "print_mode_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PLANED_COOKIE 0x504C4144
#define PAGE_DATA_COOKIE 0x50414745
#define JOB_DATA_COOKIE 0x4A4F4244

#define IS_PLANED(x) ((x)->planed_cookie == PLANED_COOKIE)
#define IS_PAGE_DATA(x) ((x)->page_data_cookie == PAGE_DATA_COOKIE)
#define IS_JOB_DATA(x) ((x)->job_data_cookie == JOB_DATA_COOKIE)

#define SET_PLANED_COOKIE(x) ((x)->planed_cookie = PLANED_COOKIE)
#define SET_PAGE_DATA_COOKIE(x) ((x)->page_data_cookie = PAGE_DATA_COOKIE)
#define SET_JOB_DATA_COOKIE(x) ((x)->job_data_cookie = JOB_DATA_COOKIE)


/**
 * Job status enum
 */
typedef enum
{
    JOBOK, ///< job printed successfully
    PARSEERROR,///< job errored out in the parser and was cancelled.
    CANCELLED, ///< job was cancelled by the user, button pressed.
    PAPER_OUT_CANCEL_RESEND, ///< job was cancelled by the printer, memory low while out of paper and no paper sensor.
    UNSUPPORTED_PDL, ///< pjl enter language for unsupported language, not called when auto language switching.
} JOB_STATUS;

/**
 * Job mode.  Used to decide what to do when cancelled.
 */
typedef enum
{
    e_Normal,
    e_KillAll       ///< On a cancel kill all pages and send notification to the calling program.
} JOB_MODE;


/** This typedef is used for a call to ScanPlanes. */
typedef enum
{
    CLEAR_IF_USECOUNT_IS_ZERO,
    YES_CLEAR,
    BY_1,
    TO_1,
    TO_1_IF_UNUSED_CLEAR_IF_USED
} FORCE_CLEAR;

typedef enum
{
    ZERO_IS_ALLOWED,
    ZERO_IS_NOT_ALLOWED
} ALLOW_ZERO;


typedef enum
{
    PACKED_RGB,
    PACKED_XRGB,
    PACKED_RGBX,
    GRAYSCALE,              ///< ADDITIVE 
    GRAYSCALE_SUBTRACTIVE,  ///< WAS GRAYSCALE_INVERT 
    PLANAR_C,
    PLANAR_M,
    PLANAR_Y,
    PLANAR_K,
    // DELTA_HYBRID,    ///< hybrid/delta format (strip)
    DITHER,
    DITHERPARAMS,
    PHOTO_LAYOUT,
    // Always add new values above this line! Also be sure to << ATTENTION! <<
    // update the switch statements in ToString_PLANE_TYPE()! << ATTENTION! <<

    // TO BE DEPRECATED SOON!!!
    ZX_STREAM,
} PLANE_TYPE;

typedef enum
{
    VIDEO_JUSTIFY_CENTERED,
    VIDEO_JUSTIFY_AUTO,
    VIDEO_JUSTIFY_PAIRED,
} justification_mode_t;


typedef enum
{
    DPI_200 = 200,
    DPI_300 = 300,
    DPI_400 = 400,
    DPI_600 = 600,
    DPI_1200 = 1200
} resolution_t;


/////////////////////////////////////////////////////////////////////////////////////////////////
// The following set of structures are used to describe the compression type and specify the
// compression parameters for the strips of data that make up a print plane.

/// enum representing the supported types of compression.  Currently only JBIG, but will be expanded.
typedef enum compression_types_s
{
    COMPTYPE_JBIG,
    COMPTYPE_RGB, // REVISIT DAB this should be deprecated and replaced with pixel packing info as this is not really compression type
    COMPTYPE_JPEG,
    COMPTYPE_ZX_STREAM,
    COMPTYPE_NONE
} compression_types_t;

typedef enum application_types_s
{
    APPTYPE_PRINT,
    APPTYPE_COPY,
    APPTYPE_FAX
} application_types_t;

/// JBIG specific compression parameters.
typedef struct jbig_compression_info_s
{
    uint32_t Xd; ///< Xd represents the padded data width, in bits, of each line in the video buffer.
                 //  Xd must be padded to a multiple of the video burst length to allow video hardware DMA to work correctly.
    uint32_t Yd; ///< JBIG total image length in lines
    uint32_t L0; ///< JBIG lines per stripe.
    uint32_t option_flags;
} jbig_compression_info_t;


typedef struct rgb_compression_info_s
{
    uint32_t bits_per_pixel;
    uint32_t Xd; ///< Xd represents the padded data width, in bits, of each line in the video buffer.
                 //  Xd must be padded to a multiple of the video burst length to allow video hardware DMA to work correctly.
    uint32_t Yd; ///< total image length in lines
} rgb_compression_info_t;

typedef struct raw_compression_info_s
{
    uint32_t bits_per_pixel;
    uint32_t Xd;
    uint32_t Yd;
} raw_compression_info_t;
 
/// compression information wrapper struct
typedef struct compression_info_s
{
    compression_types_t comp_type;
    union
    {
        jbig_compression_info_t jbig;
        rgb_compression_info_t rgb;
        raw_compression_info_t raw;
    } comp_details;
} compression_info_t;

typedef struct image_resolution_s
{
    uint32_t x;
    uint32_t y;
} image_resolution_t;


typedef struct image_info_s
{
    uint32_t image_width_in_pixels;     // This is the width of the image, used for calculating side margins
    uint32_t image_height_in_lines;
    uint32_t image_bpp;
    uint32_t image_data_width_in_bytes; // This is the data width of the white space padded image.
    uint32_t image_scale_right_padding;	// This is the padding size of scan scale. it will use rotation case in genup task.
    uint32_t image_top_padding;			// This is the padding size of top of image. it will use rotation case in genup task.
    
	int32_t image_engshift;
	uint8_t sourceAPP;
    image_resolution_t image_dpi;       // x and y resolution of the image
    bool left_to_right;                 // Direction of the data in the image
} image_info_t;




/***********************************************************
 * Macros for use with AutoDuplex and the DUPLEX member of
 * PRINTCONFIG. The other bits have meaning *ONLY*
 * if (GetAutoDuplex(PAGE_DATA.DUPLEX) == TRUE).
 *
 * REVISIT TODO: Recode all attributes related to DUPLEX as
 * members of a struct, and let the compiler handle the
 * details of accessing each field!
 */
// Bit order, left to right = most significant bit to least significant bit.
#define DUPLEX_ReversePlaneOrderMask        0x8000
#define DUPLEX_SendFrontSideFirstToEngineMask 0x4000
#define DUPLEX_ConvertLastOddPageToSimplexMask 0x0800
#define DUPLEX_RotationMask                 0x0700  // Type: rotate_dir_t; 4 directions + a don't-care value.
#define DUPLEX_RotationShiftBits            8
#define DUPLEX_ParserSendsBackSideFirstMask 0x0080
#define DUPLEX_BackSideMask                 0x0040
#define DUPLEX_ShortEdgeBindingMask         0x0020
#define DUPLEX_AutoDuplexMask               0x0010
#define DUPLEX_DMDuplexMask                 0x000F  // Type: uint; Copied from Duplex tag from host based parser.
#define DUPLEX_DMDuplexShiftBits            0
// Alphabetical order by field name.
#define GetAutoDuplex(x)                    ( ((x) & DUPLEX_AutoDuplexMask) ? TRUE : FALSE )
#define SetAutoDuplexOff(x)                 ( x &= ~DUPLEX_AutoDuplexMask )
#define SetAutoDuplexOn(x)                  ( x |= DUPLEX_AutoDuplexMask )
#define GetBackSide(x)                      ( ((x) & DUPLEX_BackSideMask) ? TRUE : FALSE )
#define SetBackSideOff(x)                   ( x &= ~DUPLEX_BackSideMask )
#define SetBackSideOn(x)                    ( x |= DUPLEX_BackSideMask )
#define GetDMDuplex(dup)                    ( (dup & DUPLEX_DMDuplexMask) >> DUPLEX_DMDuplexShiftBits )
#define SetDMDuplex(dup, val)               ( dup = (dup & ~DUPLEX_DMDuplexMask) | (((val) << DUPLEX_DMDuplexShiftBits) & DUPLEX_DMDuplexMask) )
#define GetParserSendsBackSideFirst(x)      ( ((x) & DUPLEX_ParserSendsBackSideFirstMask) ? TRUE : FALSE )
#define SetParserSendsBackSideFirstOff(x)   ( x &= ~DUPLEX_ParserSendsBackSideFirstMask )
#define SetParserSendsBackSideFirstOn(x)    ( x |= DUPLEX_ParserSendsBackSideFirstMask )
#define GetRotation(dup)                    ( (dup & DUPLEX_RotationMask) >> DUPLEX_RotationShiftBits )
#define SetRotation(dup, rot)               ( dup = (dup & ~DUPLEX_RotationMask) | (((rot) << DUPLEX_RotationShiftBits) & DUPLEX_RotationMask) )
#define GetShortEdgeBinding(x)              ( ((x) & DUPLEX_ShortEdgeBindingMask) ? TRUE : FALSE )
#define SetShortEdgeBindingOff(x)           ( x &= ~DUPLEX_ShortEdgeBindingMask )
#define SetShortEdgeBindingOn(x)            ( x |= DUPLEX_ShortEdgeBindingMask )
#define GetSendFrontSideFirstToEngine(x)    ( ((x) & DUPLEX_SendFrontSideFirstToEngineMask) ? TRUE : FALSE )
#define SetSendFrontSideFirstToEngineOff(x) ( x &= ~DUPLEX_SendFrontSideFirstToEngineMask )
#define SetSendFrontSideFirstToEngineOn(x)  ( x |= DUPLEX_SendFrontSideFirstToEngineMask )
#define GetReversePlaneOrder(x)             ( ((x) & DUPLEX_ReversePlaneOrderMask) ? TRUE : FALSE )
#define SetReversePlaneOrderOff(x)          ( x &= ~DUPLEX_ReversePlaneOrderMask )
#define SetReversePlaneOrderOn(x)           ( x |= DUPLEX_ReversePlaneOrderMask )
#define GetConvertLastOddPageToSimplex(x)    ( ((x) & DUPLEX_ConvertLastOddPageToSimplexMask) ? TRUE : FALSE )
#define SetConvertLastOddPageToSimplexOff(x) ( x &= ~DUPLEX_ConvertLastOddPageToSimplexMask )
#define SetConvertLastOddPageToSimplexOn(x)  ( x |= DUPLEX_ConvertLastOddPageToSimplexMask )

/**
 * \brief Symbolic names and values as defined in Microsoft's DEVMODE interface.
 */
typedef enum {
    DMDUP_SIMPLEX = 1,
    DMDUP_VERTICAL = 2,
    DMDUP_HORIZONTAL = 3
} DUPLEX_MODE;
# if defined( HAVE_DBG_PRINTF )
const char* ToString_DUPLEX_MODE( DUPLEX_MODE duplexModeEnum );
# endif // defined( HAVE_DBG_PRINTF )

/*
   Define the JBIG Block Header
*/
typedef struct
{
   unsigned Xd, Yd, L0;
   unsigned char optionFlags;
} tJBIG_BIH;

/** End of data flag */
typedef enum
{
    NORMAL_BLOCK,
    LAST_BLOCK
} END_DATA;

/** Print quality */
typedef enum
{
    PRINTQUALITY_NORMAL = 0,
    PRINTQUALITY_BETTER,
    PRINTQUALITY_BEST,
    PRINTQUALITY_DRAFT
} PRINT_QUALITY;

/** ID nup type */
typedef enum
{
    IDNUP_INACTIVE = 0,			// SDH use INACTIVE and SDH_ACTIVE
    IDNUP_ACTIVE,				// marvell merge several images in ID copy case.
    IDNUP_FINISHED,				// But, SDH use 2UP format(A5LEF+A5LEF => A4SEF and NORotation)
    IDNUP_CANCELED,				// (Common case, 2UP format has rotation)
    IDNUP_SDH_ACTIVE,
// For special mode
    IDNUP_A3MERGE_ROTATE,
    IDNUP_A3MERGE_NOROTATE
} ID_nup_t;

/**
 * Zx layer parameters struct
 */
typedef struct
{
    uint32_t width_in_pixels;
    uint32_t height_in_lines;
    uint32_t frac_scale_height_in_lines;
    image_resolution_t res;
    uint32_t color_size_in_bytes;
} zx_params_t;

/**
 * CUPS raster header fields of interest
 */
typedef struct
{
    uint32_t imaging_bbox_int[4];
    float imaging_bbox_float[4];
    uint32_t page_size_int[2];
    float page_size_float[2];
    uint32_t color_space;
    uint32_t compression;
    char marker_type[64];
    char rendering_intent[64];
    char page_size_name[64];
    uint32_t anti_bleed_control;
    uint32_t source_image_lightness;
    uint32_t advance_dot_placement;
    uint32_t duplex_dry_time;                ///< Duplex dry time field from header
    uint32_t top_of_form_detect_disable;
} cups_r_params_t;

typedef struct
{
    uint32_t opaque_data_len;
    BigBuffer_t *opaque_data;
} Opaque_Data_Params_t;

typedef struct
{
    page_orientation_t page_orientation;   ///< Orientation of the images on the page.
    uint16_t     num_images;   ///< Number of images to place on the page.
    uint32_t     image_width;  ///< Image width, shared by all images on this page.
    uint32_t     image_height; ///< Image height, shared by all images on this page.
    bool       adf_scan;     ///< Image was scanned from ADF
    ID_nup_t   ID_nup;       ///< All images are ORed together into this page.
} nup_image_params_t;

typedef enum {
    //e_linear_RGB, //may need to support
    e_sRGB,
    e_adobe_RGB,
    e_additive_gray,
    e_subtractive_gray
} input_color_space_t;

// cleaning page types
typedef enum cleaning_page_type_e
{
    CLEANING_PAGE_TYPE_ALL,
    CLEANING_PAGE_TYPE_COLOR,
    CLEANING_PAGE_TYPE_KT
} cleaning_page_type_t;

/** Plane Data Config */
typedef struct
{
    uint32_t    planed_cookie;
    ATLISTENTRY listNode;   ///< Used to link all of the planes together to define an entire pages' data.
    COLOR       sysPlaneColor;  ///< The system representation of the plane color ; was PLANE_COLOR
    uint32_t DataLength;          ///< length of the attached data buffer; note that for compressed data this is not the same
                                ///  as image strip dimensions described by image_info
    bool LastData;
    uint16_t UseCount;
    bool released_but_not_freed;
    uint16_t      DUPLEX;
    PLANE_TYPE BlockDataType;
    BigBuffer_t *big_buffer;

    compression_info_t compression_info;
    image_info_t image_info;
    input_color_space_t color_space;

} PLANED;

typedef enum swath_type_e
{
    SWATH_TYPE_NORMAL,
    SWATH_TYPE_SENSOR
} swath_type_t;

typedef enum swath_direction_e
{
    SWATH_DIRECTION_AUTO,    //determined automatically by engine
    SWATH_DIRECTION_FORWARD,
    SWATH_DIRECTION_REVERSE
} swath_direction_t;

typedef enum page_state_e
{
    PAGE_STATE_NEW,    
    PAGE_STATE_JOB_MGR,
    PAGE_STATE_PR_MGR,
    PAGE_STATE_CANCELING,
    PAGE_STATE_JAMMED,
    PAGE_STATE_RETRY,
    PAGE_STATE_ENG_PRESTART,
    PAGE_STATE_ENG_START,
    PAGE_STATE_VIDEO_DONE,
    PAGE_STATE_FREE,
} page_state_t;

typedef struct manual_swath_s
{
    swath_type_t type;
    uint32_t vertical_offset;
    uint32_t height;
    uint32_t resolution_x;
    uint32_t resolution_y;
    uint32_t left_margin;
    uint32_t width_in_pixels;
    uint32_t row_advance;
    bool rows_released;
    swath_direction_t direction;
    uint32_t speed;
    int32_t paper_advance;
    struct manual_swath_s *next_swath;
} manual_swath_t;

typedef struct image_enhancement_s
{
    uint32_t brightness; // range [0,200] neutral = 100
    uint32_t contrast;   // range [0,200] neutral = 100
} image_enhancement_t;

typedef struct image_option_s
{
	uint32_t darknessLevel;			// range 1~10 / default = 6
	uint32_t tonerSave;			// 1 = On, 0 = Off
	uint32_t imageResolution;
}image_option_t;

// forward declaration for use in PAGE_DATA structure
struct _JOB_DATA;
typedef struct _SHEET_DATA  SHEET_DATA;

typedef struct _PAGE_DATA
{
    uint32_t    page_data_cookie;
    ATLISTENTRY listNode;
    mediasize_t PAPER;              ///< Media Size requested by job. May be wildcard or invalid value.    uint16_t      VIDEO_BPP;          ///< VIDEO_BPP represents the bits per pixel of the image as it is received by the video block.
    uint16_t      COPIES;
    uint16_t    num_input_colors;   ///< Number of input colors in the source data
    uint16_t      BORDERLESS;
    uint8_t       PRINTGRAY;
    DOCTYPE     DocType;
    PRINT_QUALITY PrintQuality;
    trayenum_t	  temp_InputTray;
    trayenum_t    DEFAULTSOURCE;      ///< Input tray requested by job. May be wildcard or invalid value.
    trayenum_t    requestedOutTray;   ///< Output tray requested by job. May be wildcard or invalid value.
    mediatype_t   mediaType;          ///< Media Type requested by job. May be wildcard or invalid value.
	image_option_t	ImageOption;
	uint8_t	  	MediaDescription;
	
    uint16_t      TONER_SAVE;

    uint16_t      COLLATE;            ///< Filled in by HBParser, but never used...
    uint16_t      DUPLEX;

    uint16_t      RET;
    uint32_t      MEDIA_SIZE_X;       ///< Width or XFeed dimension in (paper_size_units_t)MEDIA_SIZE_UNITS.
    uint32_t      MEDIA_SIZE_Y;       ///< Height or Feed dimension in (paper_size_units_t)MEDIA_SIZE_UNITS.
    paper_size_units_t      MEDIA_SIZE_UNITS;   ///< Units for MEDIA_SIZE_X and MEDIA_SIZE_Y.

    uint16_t      PageSent;           ///< Number of copies of this page that have been sent to the engine.
    uint16_t      CanDeleteData;      ///< Flag set by engine indicating that the data has started to be put on paper.  This means that it can be deleted if JamRecovery is off.
    uint16_t      DataDeleted;        ///< set when data has been deleted from this plane.
    uint16_t      PagesPrinted;       ///< The number of pages that have been printed, used for # copies.
    uint16_t      CurrentUseCount;    ///< the use count to use when pages cancel.
    uint16_t    numPlaneBuffers[NUM_MARKING_COLOR_CHANNELS]; ///< Keep track of how many data buffers for each plane.
    MLISTENTRY planeDataPendingPrintList[NUM_MARKING_COLOR_CHANNELS]; ///< Array of lists of plane data buffers not yet printed
    uint16_t       Racing;            ///< Set when racing the engine
    struct _JOB_DATA* pMyJob;       ///< pointer to job that this page is part of
    uint8_t       JamRecovery;        ///< is Jam recovery on for this page?
    uint8_t       ManualDuplex;       ///< is this page the first duplex (backside) page of manual duplex job
    uint8_t       ManDup2ndPg;        ///< if this is 0 it is a front page.  if 1 it is 2nd page
    uint8_t       PageReadyToSend;    ///< this job is ready to send to pm.
    uint16_t      PageCount;          ///< One-relative sequential count of the page number within the job.
    trayenum_t  engTray;            ///< Input tray engine should use (determined by fw not user settings)
    trayenum_t	engEmptyTray;
    mediasize_t engMediaSize;       ///< size engine should use (determined by fw not user settings)
    mediatype_t engMediaType;       ///< type engine should use (determined by fw not user settings)
    trayenum_t  engOutTray;         ///< Output tray engine should use (determined by fw not user settings)
    page_color_mode_t page_output_color_mode; ///< Color mode of the printed output (mono or color)
    uint8_t       engPrintMode;       ///< Remember which engine Print Mode / Fuser Mode setting we selected.
#if defined(PIXELCNT_BYHOST) || defined(PIXELCNT_BYFW)
    PIXELCNT*   swPixCountPtr;      ///< Pointer to pixel counts sent by the host; NULL if no counts sent by host.
    PIXELCNT    hwPixCount;         ///< Hardware pixel counts; on a host based pixel count system these are only
                                    ///< used in cases where we don't have host counts such as true internal pages.
                                    ///< On a FW pix count system they are always used.
#endif

#if defined(PCL_PARSER)
    void*       foreignPageHandle;
#endif

   SHEET_DATA *sheet; ///< not owned reference to the sheet this page belongs to.

   uint32_t       MediaWidth;
   uint32_t       MediaHeight;
#ifdef HAVE_RTOS
   // LSPTODO: convert to using messages for data communication
   TX_EVENT_FLAGS_GROUP *EventFlags;         ///< Event flag pointer for data communication
#endif
   zx_params_t  zx_params;                   ///< Zx layer parameters
   cups_r_params_t  cups_r_params;           ///< CUPS raster header fields of intrest
   nup_image_params_t   nup_image_params;    ///< N-up image parameters
   uint8_t        manual_swath_mode_enabled;   ///< 1 if manual swath mode is enabled, 0 otherwise.
   manual_swath_t *manual_swaths;            ///< Dynamically allocated list of manual swaths; used if not NULL
   uint32_t       num_active_planes;           ///< Number of active planes on the page (0 to NUM_MARKING_COLOR_CHANNELS)

   image_info_t image_output_info;           ///< Dimensions of the output print data (to be printed)
   input_color_space_t input_color_space;    ///< used to configure PIP color conversion from input to output color
   print_mode_t  *print_mode;                ///< The print pipe mode and other data selected at a high level for this page, usable downstream
   justification_mode_t  video_justification_mode;  ///< video just, e.g. auto, centered, paired
   image_enhancement_t image_enhancement;   ///< brightness and contrast image adjustments
   page_state_t page_state;
   UINT8		fLastPage;
   UINT8		fSourceTrayChg;
   uint8_t		parser_info;
   ROTATE_DIR	fRotate_oneup_image;
   uint8_t		force_default_media_size;
} PAGE_DATA;

/**
 * Define the Sheet header
*/
struct _SHEET_DATA 
{
    ATLISTENTRY node;       ///< node use to link sheets into a list
    DOCTYPE     DocType;    ///< todo: document why doctype on page, sheet and job, can they vary ? 
    PAGE_DATA  *FrontPage;  ///< Front all simplex and manual duplex only have front images
    PAGE_DATA  *BackPage;   ///< Back side image, 
    bool        sheetReady; ///< Only release the page for print when its ready, used to hold for both sides present.	
};

/**
 * Define the Job Header
*/
typedef struct _JOB_DATA
{
    uint32_t      job_data_cookie;
    ATLISTENTRY   node;               ///< Node used for linking jobs into lists.
    uint16_t      PAGECOUNT;          ///< Filled in by JobManager, used to stall at end of job in certain conditions.
    uint16_t      COLLATE;            ///< if copies > 1 then collate or not.
    mediasize_t   PAPER;              ///< Media Size requested by job. May be wildcard or invalid value.
    uint16_t      COPIES;             ///< Number of copies to print.
    trayenum_t    DEFAULTSOURCE;      ///< Input tray requested by job. May be wildcard or invalid value.
    trayenum_t    requestedOutTray;   ///< Output tray requested by job. May be wildcard or invalid value.
    bool          request_media_detect; ///< this job would like the engine to detect the media type before pages are produced
    mediatype_t   mediaType;          ///< Media Type requested by job. May be wildcard or invalid value.
    uint16_t      DUPLEX;
    bool          duplex_rotate_backside;
	image_option_t	ImageOption;
	bool		mismatch_continue;
	
    IOReg      *ChannelID;          ///< Information on the I/O being used.
    void       *Resources;          ///< Resource info for the job.
    PrintJobInfo_t *pjob_info;      ///< observer lists for page/job events.
    DOCTYPE     DocType;            ///< cal vs regular print job
    JOB_MODE    JobMode;            ///< what mode to use when cancelling this job.
    MODULE_ID   SendingModule;      ///< the pointer to the sending module.
    uint8_t     JobReceived;        ///< set when the entire job has been received.
    uint8_t     JobStartSent;       ///< the job has already been sent to the page mgr.  //dgb info? unused cull.
    uint8_t     JobReceiveActive;   ///< while in the process of receiving this job, set this.
    ATLISTENTRY sheet_list;         ///< Linked list of sheets.
//    JOB_DATA_TYPE jobDataType;      ///< Copy, print, etc.
    uint32_t    JobNum;             ///< A job identifier; just an incrementing global counter.
    bool        paper_out_all;      ///< Track if paper out occured on this job.
    bool        job_data_aborted;   ///< Track if data receive was aborted for this job.
    bool        job_cancelling;     ///< Track if this job is in the process of being cancelled.
    bool        job_paused;         ///< Track if this job is paused
    bool        job_sent;           ///< Track if we've sent this job

	uint8_t		parser_info;
	uint8_t		JobInfo;			/// 0 : print, 1 : copy, 2 : fax
	mediasize_t CopyTraySettingSize[NUM_INPUTS];
    void    *oem_data;         ///< Used to specify vendor specific data.
    mediasize_t	PrintedPaperSize;
} JOB_DATA;

#ifdef HAVE_DBG_PRINTF
const char* to_string_plane_type( PLANE_TYPE planeType );
const char* to_string_document_type( DOCTYPE doc_type );
const char* to_string_print_quality( PRINT_QUALITY print_quality );
#else 
static inline const char* to_string_plane_type( PLANE_TYPE planeType ) { return 0; }
static inline const char* to_string_document_type( DOCTYPE doc_type ) { return 0; }
static inline const char* to_string_print_quality( PRINT_QUALITY print_quality ) { return 0;}
#endif 

#ifdef __cplusplus
}
#endif

#endif

