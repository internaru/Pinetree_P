/* 
 * ============================================================================
 * Copyright (c) 2007-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/*
 * scantypes.h
 *
 * Basic global types used throughout the scan code.
 *
 * This file should be able to be included in any code anywhere without having
 * to depend on other files other than ATypes.h
 */

#ifndef SCANTYPES_H
#define SCANTYPES_H

#include <stdint.h>
#include <stdbool.h>

/** Whether we're scanning mono or color */
typedef enum {
    SCAN_CMODE_MONO=1,  ///< scanning mono
    SCAN_CMODE_COLOR    ///< scanning color
} scan_cmode_t;

/** 
 * \brief Sensor hardware margins 
 *
 * Contains the left and right inactive pixels and number of active pixels.
 */
// NOTE that the field "left" is to be set to 0 for a sensor that requires CISX
// support (multi-segment sensor combined by CISX)
typedef struct {
    uint32_t dpi;    ///< sensor dpi; used as key into table of this structure
    uint32_t left;   ///< number of dark/inactive pixels at the left
    uint32_t active; ///< number of active pixels 
    uint32_t right;  ///< number of inactive pixels at the right side of the sensor
} scan_sensor_margin_t;

/** 
 * Error codes returned from any/all/most of the scan subsystem functions.
 *
 * Please try to keep the errors general enough to be used in more than one
 * situation but specific enough to be useful. See errno for inspiration.
 *
 * All these numbers should be negative so we'll behave like errno which most
 * people are already familiar.
 */
typedef enum {
    SCANERR_NONE          =   0, ///< no error. Huzzah!
    SCANERR_OUT_OF_MEMORY =  -1, ///< ran out of memory (ENOMEM)
    SCANERR_SCAN_CANCELLED=  -2, ///< someone cancelled the scan (EINTR)
    SCANERR_NO_ENTRY      =  -3, ///< a table lookup failed (ENOENT)
    SCANERR_PAPER_JAM     =  -4, ///< ADF paper jam  (EIO)
    SCANERR_CAL_FAILED    =  -5, ///< cal failed so can't scan
    SCANERR_INVALID_PARAM =  -6, ///< invalid parameters to function (EINVAL)
    SCANERR_HARDWARE_FAIL =  -7, ///< hardware failure detected
    SCANERR_MSG_FAIL      =  -8, ///< rtos message failed
    SCANERR_MISSED_NOTCH  =  -9, ///< notchfind failed - looked but couldn't find
    SCANERR_BUSY          = -10, ///< scanner is busy (EBUSY)
    SCANERR_NO_PAPER_EDGE = -11, ///< expected TOF/BOF edge, didn't get it
    SCANERR_NOT_IMPLEMENTED=-13, ///< called a function with no implementation
    SCANERR_NOT_PERMITTED = -14, ///< operation not permitted (EPERM)
    SCANERR_ALREADY       = -15, ///< operation already in progress (EALREADY)
    SCANERR_NO_PAPER_LOADED = -16, ///< no paper in the input tray
    SCANERR_TIMEDOUT      = -17, ///< operation timed out (ETIMEDOUT)

    SCANERR_GENERIC_FAIL  = -99, ///< something unexpected went wrong
} scan_err_t;

/** \brief Message structure passed around the scan subsystem */
typedef struct {
    uint32_t msgType; ///< One of SMSG_xxx 
    uint32_t param1;  ///< depends on message
    uint32_t param2;  ///< depends on message
    void *param3;   ///< depends on message (sometimes used as uint32_t)
} scan_msg_t;

/** \brief PIE pixel format
 *
 */

typedef enum {
    PIE_PIXEL_XRGB=1,
    PIE_PIXEL_RGBX,
    PIE_PIXEL_MONO,
    PIE_PIXEL_3_PLANES,
    PIE_PIXEL_RGB, 
} pie_pixel_t;

/* Scan message callback function. When non-null, scantask will send messages
 * here while scanning.
 */
typedef scan_err_t (*scan_callback_fn_t)( scan_msg_t *scan_msg );

/** Public Scan Subsystem messages.
 *
 * There are a number of other messages passed around the scan subsystem. For
 * example, messages from interrupts will also use the scan_msg_t structure. 
 *
 * The public messages start at this base.  Think of it like WM_USER. Any
 * messages [0, SMSB_PUB_BASE-1] are reserved by Scan.
 *
 * Can't be an enum because these numbers are shared with another internal
 * header file.
 *
 * davep 24-Jun-2009 ; large update to messages to better support ADF scanning.
 * Eliminated SMSG_NEW_PAGE, added SMSG_PAGE_START and SMSG_SCAN_END.
 *
 * When scanning from an ADF, the sequence from Scantask will be:
 *
 *  SMSG_SCAN_SIZE 
 *  SMSG_PAGE_START
 *  SMSG_PAGE_DATA
 *  SMSG_PAGE_DATA
 *  ...
 *  SMSG_PAGE_DATA
 *  SMSG_PAGE_END  (another page in the adf)
 *  SMSG_PAGE_START
 *  SMSG_PAGE_DATA
 *  SMSG_PAGE_DATA
 *  ...
 *  SMSG_PAGE_DATA
 *  SMSG_PAGE_END  (no more pages in the adf)
 *  SMSG_SCAN_END
 *  SMSG_SCAN_READY
 *
 *  When scanning from the flatbed, the sequence is the same except there will
 *  only be one PAGE_START/PAGE_DATA{1,}/PAGE_END sequence.
 */

#define SMSG_PUB_BASE      (1000)

#define SMSG_SCAN_INIT_DONE (SMSG_PUB_BASE + 1)
#define SMSG_SCAN_READY    (SMSG_PUB_BASE + 2)
#define SMSG_SCAN_FAILED   (SMSG_PUB_BASE + 3)
#define SMSG_SCAN_START    (SMSG_PUB_BASE + 4)
#define SMSG_SCAN_END      (SMSG_PUB_BASE + 5)
#define SMSG_SCAN_SIZE     (SMSG_PUB_BASE + 6) 
#define SMSG_PAGE_START    (SMSG_PUB_BASE + 7)
#define SMSG_PAGE_DATA     (SMSG_PUB_BASE + 8)
#define SMSG_PAGE_END      (SMSG_PUB_BASE + 9)
#define SMSG_SCAN_CANCEL   (SMSG_PUB_BASE + 10)
#define SMSG_SMECH_POLL    (SMSG_PUB_BASE + 11)
#define SMSG_DATA_BLOB     (SMSG_PUB_BASE + 12)

/** \def SMSG_SCAN_INIT_DONE
 *
 * Sent by Scantask when initialization is complete.  
 *
 * scantask_init() starts the Scantask thread, which first initializes the scan
 * hardware then runs some simple self-tests.  If the initialization and
 * self-tests are successful, param1 will be zero. Failure is defined by 
 * param1 != 0.
 *
 * See the error codes in scanhwerr.h
 *
 * \li msgType: SMSG_SCAN_INIT_DONE
 * \li param1: zero: scan hardware/firmware initialized successfully
 *           : non-zero: value from scanhwerr.h
 * \li param2: reserved
 * \li param3: reserved
 *
 */

/** \def SMSG_SCAN_READY
 *
 * Sent by Scantask after a scan is complete and he is ready and able to
 * perform another scan.  Scantask sends this message right before returning to
 * his idle state. 
 *
 * Do not message Scantask between the SMSG_SCAN_END and SMSG_SCAN_READY. See
 * also SMSG_SCAN_END. 
 *
 * \li msgType: SMSG_SCAN_READY
 * \li param1: reserved
 * \li param2: reserved
 * \li param3: reserved
 *
 */

/** \def SMSG_SCAN_START
 *
 * Sent to Scantask to begin a scan.  Will not be transmitted by Scantask.
 *
 * \li msgType: SMSG_SCAN_START
 * \li param1: reserved
 * \li param2: reserved
 * \li param3: reserved
 *
 */

/** \def SMSG_SCAN_END
 *
 * Sent by Scantask to indicate a scan is complete. 
 *
 * Between the SMSG_SCAN_END and SMSG_SCAN_READY is the "Scantask Blackout
 * Period".  Scantask is busy cleaning up and should not be messaged.
 *
 * Yes, I'm perfectly aware there is a race condition.  
 *
 * \li msgType: SMSG_SCAN_END
 * \li param1: reserved
 * \li param2: reserved
 * \li param3: reserved
 *
 */

/** \def SMSG_SCAN_SIZE 
 *
 *  Sent by Scantask, this message contains the size of the scanned area.
 *  Derived by Scantask by taking the requested scan size and adjusting it up,
 *  down, left, right, and sideways for DMA. Necessary to tell a higher layer
 *  the description of the actual data.
 *
 *  With an ADF scan, the total_rows will probably be meaningless.
 *
 * param1: pixels per row padded
 * param2: pixels per row
 *
 * param3:
 *  [fedcba9876543210fedcba9876543210]
 *  [.3.........2.........1.........0]
 *  [10987654321098765432109876543210]
 *  [........SSSSSSSSNNNNNNNNNNNNNNNN]
 *
 *  '.' - reserved
 *  'N' - total_rows (16-bits)
 *  'S' - pixel size in bits (e.g., 1,2,4,8,16,32 etc)  (8-bits)
 *
 */

/** Gets the bits per pixel value from the SMSG_SCAN_SIZE message.  Note this is
 * BITS per pixel, not BYTES.
 */
#define SMSG_SIZE_GET_PIXEL_SIZE(n) (((n)>>16)&0xff)   ///< bits per pixel

/** Sets the bits per pixel size in the SMSG_SCAN_SIZE message */
#define SMSG_SIZE_SET_PIXEL_SIZE(n) (((n)&0xff)<<16)  ///< bits per pixel

#define SMSG_SIZE_GET_NUM_ROWS(n) ((n)&0xffff)
#define SMSG_SIZE_SET_NUM_ROWS(n) ((n)&0xffff)


/** \def SMSG_PAGE_START
 * 
 * Sent by Scantask. Indicates Scantask is starting a new page.
 *
 * msgType: SMSG_PAGE_START
 *
 * param1: 
 *  [.3.........2.........1.........0]
 *  [10987654321098765432109876543210]
 *  [........................SSSSSSSS]
 *
 *  '.' - reserved
 *  'S' - page side (usually 0 or 1; for dual sidded scanning)
 *
 * param2: reserved
 * param3: reserved
 */

#define SMSG_PAGE_START_GET_PAGE_SIDE(p) ((p)&0xff)
#define SMSG_PAGE_START_SET_PAGE_SIDE(p) ((p)&0xff)

/** \def SMSG_PAGE_DATA
 *
 *  Here comes data!
 *
 *  Sent by Scantask, this message will contain a pointer to the data (param3)
 *  and the number of rows in the buffer.  The number of rows in any particular
 *  SMSG_PAGE_DATA message can change. 
 *
 *  The data width will be the pixels_per_row_padded as sent in the
 *  SMSG_SCAN_SIZE message.  The data width will be constant during the entire
 *  scan.
 *
 *  Bytes in the buffer will be:
 *      pixels_per_row_padded * (8/bits_per_pixel) * num_rows
 *
 *  bits_per_pixel and pixels_per_row_added come from SMSG_SCAN_SIZE
 *
 *  param2 contains a flags field and the type of data.  Use the
 *  SMSG_DATA_GET_xxx macros to decode param2.
 *
 *  param1:
 *  [.3.........2.........1.........0]
 *  [10987654321098765432109876543210]
 *  [MMMMMMMMMMMMMMMMNNNNNNNNNNNNNNNN]
 *
 *  'M' - maximum number of rows in this buffer (allocated data space)
 *  'N' - number of rows in this buffer (actual image data) (numrows<=maxrows)
 *
 *  param2:
 *  [fedcba9876543210fedcba9876543210]
 *  [.3.........2.........1.........0]
 *  [10987654321098765432109876543210]
 *  [TTTTTTTT......................SL]
 *
 *  '.' - reserved
 *  'L' - last buffer; set to indicate this is the last buffer in the scan
 *  'S' - page side (0 or 1); side 1 will be seen on mechs that have a sensor
 *          on both sides of the paper.
 *  'T' - data type (8-bits)
 *
 *  The param2 data type will be one of scan_data_type.  
 *
 *  \li msgType: SMSG_PAGE_DATA
 *  \li param1: number of rows in this buffer (see description above)
 *  \li param2: flags and data type (see description above)
 *  \li param3: pointer to the data 
 */

/** Gets the number of rows from the SMSG_PAGE_DATA message */
#define SMSG_DATA_GET_NUM_ROWS(n)           ((n)&0xffff)

/** Gets the scan_data_type from the SMSG_PAGE_DATA message */
#define SMSG_DATA_GET_DATATYPE(n)           (((n)>>24)&0xff)

/** Gets the page side field. Mechs with 2x sensors can scan both sides
 * simultaneously. Buffers containing data from either side may arrive
 * interleaved.
 * davep 04-Apr-2013 
 */
#define SMSG_DATA_GET_PAGE_SIDE(n)   (((n)&0x02)>>1)

/** Gets the last buffer flag from the SMSG_PAGE_DATA message. When this flag
 * is set, this is the last SMSG_PAGE_DATA buffer in this scan.
 */
#define SMSG_DATA_GET_LAST_BUFFER_FLAG(n)   ((n)&0x01)

/** Sets the number of rows in the SMSG_PAGE_DATA message. */
#define SMSG_DATA_SET_NUM_ROWS(n)          ((n)&0xffff)

/** Sets the data type in the SMSG_PAGE_DATA message. */
#define SMSG_DATA_SET_DATATYPE(n)           (((n)&0xff)<<24)

/** Sets the page side field. Mechs with 2x sensors can scan both sides
 * simultaneously. Buffers containing data from either side may arrive
 * interleaved.
 * davep 04-Apr-2013 
 */
#define SMSG_DATA_SET_PAGE_SIDE(n)          (((n)&0x01)<<1) 

/** Sets the last buffer flag in the SMSG_PAGE_DATA message */
#define SMSG_DATA_LAST_BUFFER_FLAG      (1<<0) 

/* davep 15-May-2013 ; adding icebuf's max_rows to the message */
#define SMSG_DATA_GET_MAX_ROWS(n)           (((n)>>16)&0xffff)
#define SMSG_DATA_SET_MAX_ROWS(n)           (((n)&0xffff)<<16)

/** \def SMSG_PAGE_END
 *
 * Sent by Scantask. Indicates Scantask is done sending data for the current
 * page.
 *
 * msgType: SMSG_PAGE_END
 *
 * param1: 
 *  [.3.........2.........1.........0]
 *  [10987654321098765432109876543210]
 *  [........................SSSSSSSS]
 *
 *  '.' - reserved
 *  'S' - page side (usually 0 or 1; for dual sidded scanning)
 *
 * param2: reserved
 * param3: reserved
 */

#define SMSG_PAGE_END_GET_PAGE_SIDE(p) ((p)&0xff)
#define SMSG_PAGE_END_SET_PAGE_SIDE(p) ((p)&0xff)

/** \def SMSG_SCAN_CANCEL
 *
 * Sent to Scantask, will cause Scantask to cancel.
 *
 * Scantask will never send a SMSG_SCAN_CANCEL. 
 *
 * \li msgType: SMSG_SCAN_CANCEL
 * \li param1: reserved
 * \li param2: reserved
 * \li param3: reserved
 */


/** \def SMSG_SCAN_FAILED
 *
 * Sent by Scantask in response to a SMSG_SCAN_START, the SMSG_SCAN_FAILED
 * message indicates the scanner was unable to complete the scan.  The reason
 * for the scan will be in the 'param1' field.
 *
 * \li msgType: SMSG_SCAN_FAILED
 * \li param1: one of the scan_err_t types
 * \li param2: reserved
 * \li param3: reserved
 */

/** \def SMSG_SMECH_POLL
 *
 * Sent to Scantask, asking to call into the mech driver's periodic polling
 * function.  How the mech driver implements that polling function is not
 * Scantask's concern.
 *
 * Originally created to handle scan mechs needing to use the sensor to poll on
 * ADF Paper Present sensors. The mech driver sets up a periodic timer that
 * sends this message to Scantask.
 *
 * \li msgType: SMSG_SMEC_POLL
 * \li param1: reserved
 * \li param2: reserved
 * \li param3: reserved
 */

/** \def SMSG_DATA_BLOB
 *
 * Sent by Scantask.  An unstructured, opaque blob of data.  Used to pass
 * untyped data.
 *
 * Originally created to send calibration meta data (e.g., exposure, pwms) to
 * the host during cal-to-host.  Will also be used for calgraphs (eventually).
 *
 * \li msgType: SMSG_SMEC_BLOB
 * \li param1: bytesize 
 * \li param2: reserved
 * \li param3: pointer to data blob
 */

/** Values for the data type field in the SMSG_SCAN_DATA message. */
typedef enum {
    SCAN_DATA_NULL_TYPE   = 0,  ///< uninitialized/none/invalid
    SCAN_DATA_TYPE_XRGB   = 1,  ///< 4-byte pixel packed as '0RGB' (ZeroRedGreenBlue)
    SCAN_DATA_TYPE_PLANE  = 2,  
    SCAN_DATA_TYPE_RED    = 3,  ///< Red
    SCAN_DATA_TYPE_GREEN  = 4,  ///< Green
    SCAN_DATA_TYPE_BLUE   = 5,  ///< Blue
    SCAN_DATA_TYPE_MONO   = 6,  ///< Grayscale
    SCAN_DATA_TYPE_HP1BPP = 7,  ///< Mono HalfPack 1bpp
    SCAN_DATA_TYPE_HP2BPP = 8,  ///< Mono HalfPack 2bpp
    SCAN_DATA_TYPE_HP4BPP = 9,  ///< Mono HalfPack 4bpp
    SCAN_DATA_TYPE_BRG_INTERLACED = 10, ///< IceLite interlaced data (BRG not a typo)
    SCAN_DATA_TYPE_RGBX   = 11,  ///< 4-byte pixel packed as 'RGBX' (RedGreenBlueZero)

    /* davep 07-May-2009 ; mirror types are a kludge to handle adf mech where
     * the paper is pulled across the sensor backwards. 
     */
    SCAN_DATA_TYPE_BRG_INTERLACED_MIRRORED = 12,
    SCAN_DATA_TYPE_MONO_MIRRORED = 13, 
    SCAN_DATA_TYPE_RGB    = 14,  ///< 3-byte pixel packed as 'RGB' 

    /* davep 21-Nov-2011 ; adding for customer; not yet used anywhere */
    SCAN_DATA_TYPE_JPEG_COMPRESSED = 15,

    /* davep 20-Jul-2013 ; adding bilevel scanning (monochrome scan, dithered
     *                      in the device)
     */
    SCAN_DATA_TYPE_BILEVEL = 16,	///Bi-Level Scan of Threshold
    SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION = 17,	///SINDOH Added, for Bi-Level Scan of Error Diffusion


    /* used for range checking */
    SCAN_DATA_TYPE_LAST,
} scan_data_type;


/** The target type we are scanning. Document scan, calibration scan,
 * etc. Used by the mech code when positioning the sensor's start position.
 */
typedef enum {
    SCAN_TARGET_TYPE_NONE                 = 0,

    /* davep 23-Oct-2012; DEPRECATED as of Oct-2012. Use STATIONARY_CAL and
     * MOVING_CAL below for new designs.
     */
    SCAN_TARGET_TYPE_CAL                  = 1, ///< calibration 

    SCAN_TARGET_TYPE_DOC                  = 2, ///< document so move to start of doc
    SCAN_TARGET_TYPE_NOTCHFIND_REVERSE    = 3, ///< notchfind, direction is reverse
    SCAN_TARGET_TYPE_SOFT_PP              = 4, ///< software based paper present (soft pp)
    SCAN_TARGET_TYPE_NOTCHFIND_REVERSE_MO = 5, ///< notchfind with moveout, direction is reverse
    SCAN_TARGET_TYPE_NOTCHFIND_FORWARD    = 6, ///< notchfind, direction is forward

    SCAN_TARGET_TYPE_STATIONARY_CAL       = 7, ///< stationary in middle of cal strip (analog cal)
    SCAN_TARGET_TYPE_MOVING_CAL           = 8, ///< captures the full cal strip and surrounding area (digital cal)
} scan_target_type_t;

/** The document source. Used on systems with multiple document inputs, e.g.,
 * ADF or flatbed.
 */
typedef enum {
    /* force scan from flatbed (ignored on adf only systems) */
    SCAN_DOCUMENT_SOURCE_FLATBED = 0,   

    /* force scan from adf (ignored on flatbed only systems) */
    SCAN_DOCUMENT_SOURCE_ADF     = 1,   

    /* force scan from adf+duplexing (ignored on flatbed only systems; falls
     * back to ADF on simplex only systems) 
     */
    SCAN_DOCUMENT_SOURCE_ADF_DUPLEX = 2,

    /* davep 03-Apr-2013 ; scan mech supports 2x sensors, one on each side of
     * paper. A docsrc of adf_duplex will be changed to adf_nsensor if scancap
     * supports nsensor.
     */
    SCAN_DOCUMENT_SOURCE_ADF_NSENSOR = 3,
} scan_document_source_t;

typedef enum {
    SCANVAR_CAL_AUTO = 0,  ///< let the scan subsystem decide when to cal (default)
    SCANVAR_CAL_DISABLED = 1, ///< run the scan uncalibrated
    SCANVAR_CAL_ALWAYS = 2, ///< always cal for this scan 
} scanvar_cal_t;

/* davep 15-Feb-2011 ; throwing this in to support app drivers requiring
 * micromanage detail of our behavior. These values are advisory only. 
 * Don't trust them. 
 */
typedef enum {
    SCANNER_IS_NOTHING = 0, /* initial state; NULL, nothing, invalid */
    SCANNER_IS_OFFLINE = 1,
    SCANNER_IS_IDLE = 2,
    SCANNER_IS_RUNNING = 3,
    SCANNER_IS_CALIBRATING = 4,
    SCANNER_IS_ASLEEP = 5,  /* we're in power-save mode */
} scanner_is_what_t;

/* davep 12-Oct-2011 ; operations on enire pages (e.g., rotate180 (adf)) can
 * operate selectively on certain pages.
 *
 * Originally created to handle even/odd page flipping for duplex scanners.
 * Future development will enclude fun full image stuff like histogram
 * equaliziation.
 */
typedef enum {
    SCAN_PAGE_RANGE_NULL=0,  /* uninitialized/null/none */
    SCAN_PAGE_RANGE_ALL=1,   /* each and every page */
    SCAN_PAGE_RANGE_EVEN=2,  /* only even pages */
    SCAN_PAGE_RANGE_ODD=3,   /* only odd pages */
} scan_page_range;

/**
 * \brief Flatbed margin information for the current scan hardware platform.
 *
 * These are static values based on the hardware platform and should not 
 * change at run-time (unless using the serial command line for tuning). 
 */
struct scan_flatbed_margins {
    /** dpi and cmode are keys into a table of the x/y margins */
    int dpi;
    scan_cmode_t cmode;

    /** the tuned upper left hand corner, in hundredths of an inch */
    int left_margin_hinches_x;
    int top_margin_hinches_y;

    /** the tuned bottom right corner, in hundredths of an inch */
    int right_margin_hinches_x;
    int bottom_margin_hinches_y;
};

/**
 * \brief ADF margin information for the current scan hardware platform.
 *
 * These are static values based on the hardware platform and should not 
 * change at run-time (unless using the serial command line for tuning). 
 */

struct scan_adf_margins {
    /** dpi and cmode are keys into a table of the tof/bof distances */
    int dpi;
    scan_cmode_t cmode;

    /** the number of rows from the staged first page until to top of the page
     * (depends on how the page is staged); see also
     * ScanMech.scan_page_prepare_fcn() below.
     */
    int rows_to_first_tof;

    /** the number of rows from flag up on current page gap until the TOF of
     * next page  
     */
    int rows_to_tof;

    /** the number of rows from flag down until BOF of current page */
    int rows_to_bof;
 
    /** the tuned left side edge, in hundredths of an inch.  The center of the ADF
     *  mech does not always line up with the center of the scan bar, tune that out
     *  here 
     */    
    int left_margin_hinches;

    /* The next set of fields are for duplex. If you're not using a Duplex ADF,
     * set to zero.
     */
    /* davep 13-Oct-2011 ; adding duplex backside margins */
    int rows_to_tof_backside;
    int rows_to_bof_backside;
};

/**
 * \brief notch position information for the current scan 
 *        hardware platform.
 */
struct scan_notch_pos {
    int x_pos;
    int y_pos;
};

/* add.lsh.for time stamp  */
#if 0
	#define TIMESTAMP(fmt, ...)
#else
	#define TIMESTAMP(fmt, ...) do { \
		struct timeval t; \
		struct tm broken; \
		do_gettimeofday(&t); \
		time_to_tm(t.tv_sec, 0, &broken); \
		dbg1("%d:%d:%ld " fmt, broken.tm_min, broken.tm_sec, t.tv_usec, ##__VA_ARGS__); \
	} while(0)
#endif

#endif

