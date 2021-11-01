/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file asp.h
 *
 * \brief header file for ASP (Asp Scan Protocol)
 *
 */

#ifndef ASP_H
#define ASP_H

#define ASP_COOKIE 0x41535001 //"ASP1"

typedef enum
{
    e_LockScanResource = 0,
    e_ReleaseScanResource = 1,
    e_StartScanJob = 2,
    e_CancelScanJob = 3, // from host to us
    e_AbortScanJob = 4,  // from us to host
    e_ScanImageData = 5,
    e_GetScanJobSettings = 6,
    e_SetScanJobSettings = 7,
    e_SetDefaultScanJobSettings = 8,
    e_StartJob = 9,
    e_StartSheet = 10,
    e_StartPage = 11,
    e_EndJob = 12,
    e_EndSheet = 13,
    e_EndPage = 14,
    e_AdfIsPaperPresent = 15,
    e_AdfUnused1 = 16,          // Not implemented
    e_AdfEjectSheet = 17,       // Not implemented
    e_AdfPickNextSheet = 18,    // Not implemented
    // For power management.
    e_EnterStandby = 19,    // Not implemented
    e_EnterReadyState = 20, // Not implemented
    // For protocol management / recovery
    e_ResetXmitBuffers = 21,
    e_ResetTimeoutCounter = 22,
    e_LampControl = 23,     // not implemented 
    e_ScanNewPage = 24,     // obsolete; no longer used 
    e_ScanDataBlob = 25,    // opaque data ; originally created for cal-to-host metadata
    e_GetPlatformString = 26, // (davep 07-Sep-2010) get scan hardware/firmware capabilities
	e_AdfInformation = 27,

    /* davep 28-Mar-2007 ; these aren't documented and aren't used; removing
     * while I'm here
     */
//    e_MoveScanBarRelative,
//    e_MoveScanBarAbsolute,

    /* upper word of the message field is reserved for our own nefarious
     * purposes 
     */
    e_InternalBase=65536,

} SCAN_MESSAGE_TYPE;

/* 
 * davep 12-Nov-2008 ; see enum scan_data_type-scantypes.h for scan data type numbers 
 */

typedef enum
{
	e_adf_info_NO_ADF,
	e_adf_info_Single_ADF,
	e_adf_info_Duplex_ADF_1pass,
	e_adf_info_Duplex_ADF_2pass,
} SCAN_ADF_INFORMATION;

typedef enum
{
    e_Success,
    e_Fail,
    e_Busy,
    e_CmdNotRecognized,
    e_InvalidArg,
    e_AdfEmpty,
    e_AdfMisPick,
    e_AdfJam
} SCAN_RESPONSE_TYPE;

typedef struct
{
    uint32_t Cookie;
    uint32_t Message;
    int32_t Param1;
    int32_t Param2;
    uint32_t Status; /* from SCAN_RESPONSE_TYPE */
    uint32_t DataLength;
    uint32_t Reserved1;
    uint32_t Reserved2;
} SCAN_HEADER;

typedef struct
{
    int32_t top;
    int32_t left;
    int32_t bottom;
    int32_t right;
} SCAN_DIMENSIONS;

typedef struct
{
    uint32_t x_numerator;
    uint32_t x_denominator;
    uint32_t y_numerator;
    uint32_t y_denominator;
} XY_SCALE_TYPE;

#define HG_SCAN_TYPE_START 128
#define RU_SCAN_TYPE_START 64

typedef enum
{
    e_MonochromeScan,
    e_ColorScan,
    e_ScanThroughCopy, ///< scan to host through copy path (good for testing) */
    e_BilevelScan,	///SINDOH Added for Bi-Level Scan	
} SCAN_TYPE;


typedef struct
{
    int32_t Gamma;
    int32_t Brightness;
    int32_t Contrast;
    int32_t Resolution;
    XY_SCALE_TYPE XYScale;
    uint32_t Sharp;
    /* davep 21-Nov-2011 ; deprecated 'Smooth' because redundant with 'Sharp' */
    uint32_t deprecated1;
    uint32_t BitsPerPixel;
    int32_t reserved1;
    int32_t reserved2;
    int32_t reserved3;
    uint32_t flags; ///< see SCAN_JOB_FLAGS_xxx
    uint32_t DataType;     ///< see enum scan_data_type
    SCAN_DIMENSIONS scan_window;
    SCAN_DIMENSIONS ScannableArea;
    uint32_t ScanType; ///< see enum SCAN_TYPE
} SCAN_JOB_SETTINGS;

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
#define SCAN_JOB_FLAGS_GET_ORIGINAL_CONTENT(flags) (((flags)>>24)&0xff)

/* Values for the SCAN_JOB_FLAGS_DOCUMENT_SOURCE field */ 
#define SCAN_JOB_SETTINGS_DOC_SRC_AUTO    0  /* firmware decides */
#define SCAN_JOB_SETTINGS_DOC_SRC_FLATBED 1 
#define SCAN_JOB_SETTINGS_DOC_SRC_ADF     2
#define SCAN_JOB_SETTINGS_DOC_SRC_ADF_DUPLEX  3

/* Values for the SCAN_JOB_FLAGS_ORIGINAL_CONTENT field */
#define SCAN_JOB_FLAGS_ORIGINAL_CONTENT_AUTO  0 /* firmware decides */
#define SCAN_JOB_FLAGS_ORIGINAL_CONTENT_TEXT  1 
#define SCAN_JOB_FLAGS_ORIGINAL_CONTENT_MIXED 2 
#define SCAN_JOB_FLAGS_ORIGINAL_CONTENT_PHOTO 3 

/* davep 21-Apr-2011 ; copy-to-host stuff the copy quality value into
 * SCAN_JOB_SETTINGS.Reserved1. The values from e_CopyQuality are used except
 * for this one value. If the host sends this value, use the default copy
 * quality (don't change the copy quality before running copy-to-host)
 */
#define SCANAPP_COPY_QUALITY_DEFAULT 0xffff

/* davep 15-Apr-2013 ; adding page_side to START_PAGE, IMAGE_DATA, and END_PAGE
 * messages.  Mechanisms with two (or more?) sensors can scan both sides of the
 * paper simultaneously. The page_side is in the SCAN_HEADER.Param1 field.
 */
#define SCAN_MSG_START_PAGE_GET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_START_PAGE_SET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_IMAGE_DATA_GET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_IMAGE_DATA_SET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_END_PAGE_GET_PAGE_SIDE(p) ((p)&0xff)
#define SCAN_MSG_END_PAGE_SET_PAGE_SIDE(p) ((p)&0xff)

typedef struct
{
    uint32_t DataType;  ///< see enum scan_data_type
    uint32_t RowNumber;
    uint32_t NumberOfRows;
    uint32_t BytesPerPixel;
    uint32_t PixelsPerRow;
    uint32_t PixelsPerRowPadded;
} SCAN_DATA_HEADER;

typedef struct
{
    SCAN_HEADER header;
    SCAN_DATA_HEADER dataheader;
} SCAN_HEADER_SEND_DATA;

#endif

