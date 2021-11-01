/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef ASP_H
#define ASP_H

#define ASP_COOKIE 0x41535001  /* "ASP\01" */

/* Scan types */
#define SCAN_TYPE_MONO  0
#define SCAN_TYPE_COLOR 1
#define SCAN_TYPE_SCAN_THRU_COPY  2  /* capture copy data via scan-to-host (test/debug feature) */

/* Scan message types */
#define SCAN_MSG_LOCK_SCAN_RESOURCE      0
#define SCAN_MSG_RELEASE_SCAN_RESOURCE   1
#define SCAN_MSG_START_SCAN_JOB          2
#define SCAN_MSG_CANCEL_SCAN_JOB         3
#define SCAN_MSG_ABORT_SCAN_JOB          4
#define SCAN_MSG_SCAN_IMAGE_DATA         5
#define SCAN_MSG_GET_SCAN_JOB_SETTINGS   6
#define SCAN_MSG_SET_SCAN_JOB_SETTINGS   7
#define SCAN_MSG_SET_DEFAULT_SCAN_JOB_SETTINGS 8
#define SCAN_MSG_START_JOB               9
#define SCAN_MSG_START_SHEET             10
#define SCAN_MSG_START_PAGE              11
#define SCAN_MSG_END_JOB                 12
#define SCAN_MSG_END_SHEET               13
#define SCAN_MSG_END_PAGE                14
#define SCAN_MSG_ADF_IS_PAPER_PRESENT    15
#define SCAN_MSG_ADF_UNUSED1             16 /* not implemented in FW */
#define SCAN_MSG_ADF_EJECT_SHEET         17 /* not implemented in FW */
#define SCAN_MSG_ADF_PICK_NEXT_SHEET     18 /* not implemented in FW */
#define SCAN_MSG_ENTER_STANDBY           19 /* not implemented in FW */
#define SCAN_MSG_ENTER_READY_STATE       20 /* not implemented in FW */
/* 21 not implemented in aspscan */
#define SCAN_MSG_RESET_TIMEOUT           22
/* 23 not implemented in aspscan */
#define SCAN_MSG_NEW_PAGE                24 /* obsolete; remove in future */
#define SCAN_MSG_DATA_BLOB               25
#define SCAN_MSG_GET_PLATFORM_STRING     26

/* internal scan message types */
#define SCAN_MSG_INTERNAL_BASE  65536

/* Status response codes */
#define SCAN_STAT_SUCCESS    0
#define SCAN_STAT_FAIL       1
#define SCAN_STAT_BUSY       2   
#define SCAN_STAT_CMD_NOT_RECOGNIZED 3
#define SCAN_STAT_INVALID_ARG    4
#define SCAN_STAT_ADF_EMPTY      5
#define SCAN_STAT_ADF_MISPICK    6
#define SCAN_STAT_ADF_JAM        7

/* Scan data types */
#define SCAN_DATA_NULL_TYPE      0 /* uninitialized/none/invalid */
#define SCAN_DATA_XRGB_PACKED    1
#define SCAN_DATA_PLANAR         2  
#define SCAN_DATA_RED            3
#define SCAN_DATA_GREEN          4
#define SCAN_DATA_BLUE           5
#define SCAN_DATA_MONO           6
#define SCAN_DATA_MONO_HALFPACK_1BPP 7
#define SCAN_DATA_MONO_HALFPACK_2BPP 8
#define SCAN_DATA_MONO_HALFPACK_4BPP 9
#define SCAN_DATA_BRG_INTERLACED 10 /* ICE Lite interlaced in Blue, Red, Green order */
#define SCAN_DATA_RGBX_PACKED    11 /* RGB0; like XRGB (0RGB) but with the 0 as LSB */
#define SCAN_DATA_BRG_INTERLACED_MIRRORED 12
#define SCAN_DATA_MONO_MIRRORED 13
#define SCAN_DATA_RGB_PACKED    14
#define SCAN_DATA_JPEG_COMPRESSED 15 /* not yet used */
#define SCAN_DATA_BILEVEL       16  /* mono image dithered to 0,255 (but still 8-bpp) */

/* Scan job settings 'flags' field values */
/* 
 * 32      24      16      8       0
 * |       |       |       |       |
 *  10987654321098765432109876543210
 *  oooooooo........dddddddd.....pnc
 *
 * c : cal-to-host (1-bit)
 * n : no cal (disable cal for this scan)
 * p : no pie (disable all image enhancements for this scan)
 *
 * d : document source (8-bits); see SCAN_DOCUMENT_SOURCE_xxx
 * o : original content (8-bits)
 *
 * davep 15-Jul-2009 ; adding 'document source' field.
 * davep 15-Jun-2011 ; adding 'original content' field
 * davep 26-Jul-2011 ; adding nocal and nopie flags 
 */
#define SCAN_JOB_FLAGS_CAL_TO_HOST  (1<<0) /* send calibration data instead of scan data */
#define SCAN_JOB_FLAGS_NO_CAL       (1<<1) /* disable calibration for this scan */
#define SCAN_JOB_FLAGS_NO_PIE       (1<<2) /* no image enhancements for this scan */

#define SCAN_JOB_FLAGS_GET_DOCUMENT_SOURCE(flags) (((flags)>>8)&0xff)
#define SCAN_JOB_FLAGS_SET_DOCUMENT_SOURCE(value) (((value)&0xff)<<8)

#define SCAN_JOB_FLAGS_GET_ORIGINAL_CONTENT(flags) (((flags)>>24)&0xff)
#define SCAN_JOB_FLAGS_SET_ORIGINAL_CONTENT(value) (((value)&0xff)<<24)

/* Values for the SCAN_JOB_FLAGS_DOCUMENT_SOURCE field */
#define SCAN_JOB_SETTINGS_DOC_SRC_AUTO			0  /* firmware decides */
#define SCAN_JOB_SETTINGS_DOC_SRC_FLATBED		1 
#define SCAN_JOB_SETTINGS_DOC_SRC_ADF			2
#define SCAN_JOB_SETTINGS_DOC_SRC_ADF_DUPLEX	3
#define SCAN_JOB_SETTINGS_DOC_SRC_FLATBED_A3	4

/* Values for the SCAN_JOB_FLAGS_ORIGINAL_CONTENT field */
#define SCAN_JOB_FLAGS_ORIGINAL_CONTENT_AUTO  0 /* firmware decides */
#define SCAN_JOB_FLAGS_ORIGINAL_CONTENT_TEXT  1 
#define SCAN_JOB_FLAGS_ORIGINAL_CONTENT_MIXED 2 
#define SCAN_JOB_FLAGS_ORIGINAL_CONTENT_PHOTO 3 

/* 24-Sep-06 davep ; settings for copy quality when doing copy-to-host. These
 * mirror (almost) the e_COPYQUALITY enum in agprint.h but I'm adding 1 so that
 * 0 becomes "leave as default quality"
 *
 * The copy quality is stuffed into the 'reserved1' field of struct scan_job.
 *
 * davep 01-Oct-2007 ;  adding fax text/mixed so can test fax IQ 
 */
#define COPY_QUALITY_TEXT    0
#define COPY_QUALITY_DRAFT   1
#define COPY_QUALITY_MIXED   2
#define COPY_QUALITY_FILM_PHOTO 3
#define COPY_QUALITY_PICTURE    4
#define COPY_QUALITY_COLORCOPY  5
#define COPY_QUALITY_TIJ_MONOCOPY 6
#define COPY_QUALITY_FAX_TEXT   7
#define COPY_QUALITY_FAX_MIXED  8
#define COPY_QUALITY_LAST COPY_QUALITY_FAX_MIXED /* keep this updated! */
#define COPY_QUALITY_DEFAULT 0xffff

/* davep 11-Apr-2007 ; responses for adf_status for aspscan_query_adf() */
#define SCAN_ADF_STATUS_UNKNOWN          0 /* dunno */
#define SCAN_ADF_STATUS_NOT_ADF          1 /* device does not have an ADF */
#define SCAN_ADF_STATUS_PAPER_PRESENT    2 /* ADF thinks it has paper */
#define SCAN_ADF_STATUS_NO_PAPER_PRESENT 3 /* ADF think there is no paper */


// this is a duplication of cal.h in the scan src code.  We currently
// (as of Aug 15, 2013) only need the NOW_CALIBRATING_SENSOR_NUM, but copied all
// of them here for completeness.  It is probably very wise to check in
// cal.h to make sure this is current.
#define CAL_METADATA_BASE 100
#define CAL_METADATA_VERT_RES           (CAL_METADATA_BASE+1)
#define CAL_METADATA_HORIZ_RES          (CAL_METADATA_BASE+2)
#define CAL_METADATA_CMODE              (CAL_METADATA_BASE+3)
#define CAL_METADATA_RED_ENABLED        (CAL_METADATA_BASE+4)
#define CAL_METADATA_GREEN_ENABLED      (CAL_METADATA_BASE+5)
#define CAL_METADATA_BLUE_ENABLED       (CAL_METADATA_BASE+6)
#define CAL_METADATA_RED_PWM            (CAL_METADATA_BASE+7)
#define CAL_METADATA_GREEN_PWM          (CAL_METADATA_BASE+8)
#define CAL_METADATA_BLUE_PWM           (CAL_METADATA_BASE+9)
#define CAL_METADATA_RED_EXP            (CAL_METADATA_BASE+10)
#define CAL_METADATA_GREEN_EXP          (CAL_METADATA_BASE+11)
#define CAL_METADATA_BLUE_EXP           (CAL_METADATA_BASE+12)
#define CAL_METADATA_RED_ANALOG_GAIN    (CAL_METADATA_BASE+13)
#define CAL_METADATA_GREEN_ANALOG_GAIN  (CAL_METADATA_BASE+14)
#define CAL_METADATA_BLUE_ANALOG_GAIN   (CAL_METADATA_BASE+15)
#define CAL_METADATA_RED_ANALOG_OFFSET  (CAL_METADATA_BASE+16)
#define CAL_METADATA_GREEN_ANALOG_OFFSET (CAL_METADATA_BASE+17)
#define CAL_METADATA_BLUE_ANALOG_OFFSET (CAL_METADATA_BASE+18)
#define CAL_METADATA_BITS_PER_PIXEL     (CAL_METADATA_BASE+19)
#define CAL_METADATA_ROWS_PER_STRIP     (CAL_METADATA_BASE+20)
#define CAL_METADATA_PIXELS_PER_ROW     (CAL_METADATA_BASE+21)
#define CAL_METADATA_CAL_CAP            (CAL_METADATA_BASE+22) 
#define CAL_METADATA_PIXELBAND_MONO     (CAL_METADATA_BASE+23)
#define CAL_METADATA_PIXELBAND_RED      (CAL_METADATA_BASE+24)
#define CAL_METADATA_PIXELBAND_GREEN    (CAL_METADATA_BASE+25)
#define CAL_METADATA_PIXELBAND_BLUE     (CAL_METADATA_BASE+26)
#define CAL_METADATA_PRNU_WHITE_AVG     (CAL_METADATA_BASE+27)
#define CAL_METADATA_PRNU_BLACK_AVG     (CAL_METADATA_BASE+28)
#define CAL_METADATA_DEVICE_ENDIAN      (CAL_METADATA_BASE+29)
#define CAL_METADATA_CAL_STRIP          (CAL_METADATA_BASE+30)
// *NOW* is sent down when we start calibrating a new side
#define CAL_METADATA_NOW_CALIBRATING_SENSOR_NUM (CAL_METADATA_BASE+31)

/* davep 12-Apr-2013 ; adding page_side to START_PAGE, IMAGE_DATA, and END_PAGE
 * messages.  Mechanisms with two (or more?) sensors can scan both sides of the
 * paper simultaneously. The page_side is in the scan_header.param1 field for
 * all three messages.
 */
#define SCAN_MSG_START_PAGE_GET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_START_PAGE_SET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_IMAGE_DATA_GET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_IMAGE_DATA_SET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_END_PAGE_GET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_END_PAGE_SET_PAGE_SIDE(p) ((p)&0xff)

/* SCAN_DATA_PLANAR is sent in messages from the host to the scanner. In
 * response, the scanner will send SCAN_DATA_RED, SCAN_DATA_GREEN, and
 * SCAN_DATA_BLUE buffers. The scanner will never send the host
 * SCAN_DATA_PLANAR in a data message.
 */

struct xyscale {
    uint32_t x_numerator;
    uint32_t x_denominator;
    uint32_t y_numerator;
    uint32_t y_denominator;
};

struct scan_dimensions {
    int32_t top;
    int32_t left;
    int32_t bottom;
    int32_t right;
};

struct scan_job {
    int32_t gamma;
    int32_t brightness;
    int32_t contrast;
    int32_t resolution;
    struct xyscale xyscale;
    uint32_t sharp;
    /* davep 21-Nov-2011 ; removed smooth since was redundant with sharpen */
    uint32_t deprecated1;
    uint32_t bpp; 
    /* reserved value could be used for IQT Auto Registration with SINDOH's IQT Program */
    int32_t reserved1; ///0: Normal Scan Mode, 1: FB, 2: ADF front, 3: ADF back
    int32_t reserved2; ///Margin Value of X(pixel)
    int32_t reserved3; ///Margin Value of Y(pixel)
    int32_t flags;       /* see SCAN_JOB_FLAGS_xxx */
    uint32_t data_type; /* one of SCAN_DATA_xxx */
    struct scan_dimensions window;
    struct scan_dimensions area;
    uint32_t scan_type;    /* SCAN_TYPE_MONO or SCAN_TYPE_COLOR */
};

struct scan_header {
    uint32_t cookie; /* SCAN_COOKIE */
    uint32_t msg;
    uint32_t param1;
    uint32_t param2;
    uint32_t status; /* SCAN_STAT_xxx */
    uint32_t datalen;
    uint32_t reserved1;
    uint32_t reserved2;
};

struct scan_data_header {
    uint32_t data_type;  /* one of SCAN_DATA_xxx */
    uint32_t rownum;
    uint32_t numrows;
    uint32_t bytes_per_pixel;
    uint32_t pixels_per_row;
    uint32_t pixels_per_row_padded;
};

/* used over the interrupt endpoint */
struct scan_status {
    uint32_t cookie;
    uint32_t msg;
    uint32_t param1;
    uint32_t param2;
};

/* useful utility functions */
void asp_swap_dimension_fields( struct scan_dimensions *sd );
void asp_swap_xyscale_fields( struct xyscale *xyscale );
void asp_swap_job_fields( struct scan_job *job );
void asp_swap_header_fields( struct scan_header *hdr );
void asp_swap_data_header_fields( struct scan_data_header *dhdr );
int asp_scan_job_to_str( char *str, int len, struct scan_job *job );
int asp_scan_header_to_str( char *str, int len, struct scan_header *hdr );
int asp_data_header_to_str( char *str, int len, struct scan_data_header *dhdr );
const char *asp_header_status_to_string( int scan_stat );

void scan_msg_page_start_decode( struct scan_header *hdr, uint8_t *page_side );
void scan_msg_image_data_decode( struct scan_header *hdr, uint8_t *page_side );
void scan_msg_page_end_decode( struct scan_header *hdr, uint8_t *page_side );

#endif

