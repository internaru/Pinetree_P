/*
 *
 * ============================================================================
 * Copyright (c) 2006-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file agJobUtils.h
 *
 * \brief Job-related utility declarations.
 *
 */
 
#ifndef _AGJOBUTILS_H
#define _AGJOBUTILS_H

#include "agprint.h"
#include "agRouter.h"
#include "print_job_types.h"
#include "agConnectMgr.h"
#include "BigBuffer.h"
#include "mlimiter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    char    *JobName; 
    DOCTYPE DocType;
    IOReg *job_pipe;
    JOB_MODE JobMode;
 //   JOB_DATA_TYPE JobData;
    MODULE_ID SendingModule;
}JOBINFO;

typedef struct
{
    DOCTYPE DocType;
    mediatype_t PageType;       ///< page type of this page.
    mediasize_t PageSize;       ///< size of paper
    trayenum_t  InputTray;      ///< tray to pull from
    trayenum_t  requestedOutTray; ///< Output tray/bin to send page to.
    PRINT_QUALITY printQuality; ///< Requested print quality of this page.
    nup_image_params_t nup_image_params; ///< N-up image parameters.
    UINT32    BitsPerPixel;   ///< bit depth per pixel per color plane.
    UINT32    PixelsPerLine;
    UINT32    X_Resolution;   
    UINT32    Y_Resolution;
    UINT32    Video_X;
    UINT32    Video_Y;
    SINT32    offset_x;
    SINT32    offset_y;
    UINT32    copies;
    UINT16    duplexInfo;
    UINT8     ManualDuplex;    // is this page the first duplex page of manual duplex job
    UINT8     ManDup2ndPg;     // if this is 0 it is a front page.  if 1 it is 2nd page
    BOOL      RawData;
    BOOL      TonerSaveMode;    ///< Turn on toner save mode.
    page_color_mode_t monoOrColor;
    BOOL      collate; ///< collate duplex copies only meaningfull if copies > 1
    UINT32    num_active_planes;
    manual_swath_t * ManualSwathList;
    UINT32    darkness;
} PAGEINFO;

typedef struct
{
   BigBuffer_t *big_buffer;
   PLANE_TYPE   stripDataType;
   BOOL   lastStrip;
   COLOR  color;
   UINT32 numLines;
   UINT32 compType;
   UINT32 bufSize;
   UINT32 X_Resolution;   
   UINT32 Y_Resolution;
   UINT32 linesPerStripe;
}STRIPINFO;

typedef struct
{
    UINT32  BlockType;
    UINT8   LastBlock;
    UINT32  xCap;
    UINT32  BlockWidth;
} PLANEINFO;

#define CHARSIZE 8 

void StartJob(JOBINFO* jobInfo, PAGEINFO* pageInfo, MODULE_ID Dest, mlimiter_t *lim);
void AttachManualSwathInfo( manual_swath_t *manual_swath_info );
void StartPage(PAGEINFO *pageInfo, MODULE_ID Dest, mlimiter_t *lim);
void StartPlane(PLANEINFO * Data, MODULE_ID Dest);
void SendStrip(PAGEINFO *pageInfo, STRIPINFO* stripInfo, MODULE_ID Dest, mlimiter_t *lim);
void EndPageEx(MODULE_ID Dest, PIXELCNT *page_pix_counts, mlimiter_t *lim);
void EndPage(MODULE_ID Dest, mlimiter_t *lim);
void EndJob(MODULE_ID Dest);
void page_plane_start(MODULE_ID Dest, COLOR plane_color);
void page_plane_end(MODULE_ID Dest, COLOR plane_color);


#ifdef __cplusplus
}
#endif

#endif  // _AGJOBUTILS_H
