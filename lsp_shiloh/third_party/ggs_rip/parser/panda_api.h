/* Copyright (C) 2011-2013 Global Graphics Software Ltd. All rights reserved.
*
* This example is provided on an "as is" basis and without
* warranty of any kind. Global Graphics Software Ltd. does not
* warrant or make any representations regarding the use or results
* of use of this example.
*
* Portions Copyright (c) 2011-2013 Marvell International, Ltd. All Rights Reserved
*/

#define FREETYPE	0

#ifndef _PANDA_API_H_
#define _PANDA_API_H_

#ifdef HAVE_MARVELL_SDK
#define PAPERLESSPATH
#ifdef MSVC
typedef void IOReg;
#else
#include "agConnectMgr.h"
#endif
#else
typedef void IOReg;
#endif

enum
{
  EMS_PCL5_EXE = 1,             /**< Executive paper size */
  EMS_PCL5_LETTER = 2,          /**< Letter paper size */
  EMS_PCL5_LEGAL = 3,           /**< Legal paper size */
  EMS_PCL5_LEDGER = 6,          /**< Ledger paper size */
  EMS_PCL5_A6 = 24,             /**< A6 paper size */
  EMS_PCL5_A5 = 25,             /**< A5 paper size */
  EMS_PCL5_A4 = 26,             /**< A4 paper size */
  EMS_PCL5_A3 = 27,             /**< A3 paper size */
  EMS_PCL5_JISB5 = 45 ,         /**< JISB5  paper size */
  EMS_PCL5_JISB4 = 46 ,         /**< JISB4  paper size */
  EMS_PCL5_OFUKU = 72,          /**< Ofuku paper size */
  EMS_PCL5_MONARCH_ENV = 80,    /**< Monarch Envelope paper size */  
  EMS_PCL5_COM10_ENV,           /**< COM10 Envelope paper size */
  EMS_PCL5_DL_ENV = 90,         /**< DL Envelope paper size */
  EMS_PCL5_C5_ENV = 91,         /**< C5 Envelope paper size */
  EMS_PCL5_CUSTOM = 101,        /**< Custom Envelope paper size */
  EMS_PCL5_US_FOLIO = 2007,     /**< Foolscap paper size */
  EMS_PCL5_STATEMENT,           /**< Statement paper size */
  EMS_PCL5_8X13 = 2011,         /**< Folio/Government legal paper size */       
  EMS_PCL5_825X13,              /**< Folio paper size */
  EMS_PCL5_C6_ENV = 2022,       /**< C6 Envelope paper size */
  EMS_PCL5_8K = 2030,           /**< 8 gak paper size */
  EMS_PCL5_G_LG = 2031,         /**< Government legal paper size */
//  EMS_PCL5_G_LT = 2032,         /**< Government letter paper size */
  EMS_PCL5_TABLOID              /**< Unknown value for PCL5???*/
};
typedef int EMS_ePCL5size;

enum
{
  EMS_PCLX_CUSTOM = -1,         /* not used in PCLXL */
  EMS_PCLX_LETTER = 0,          /**< Letter paper size */
  EMS_PCLX_LEGAL = 1,           /**< Legal paper size */
  EMS_PCLX_A4 = 2,              /**< A4 paper size */
  EMS_PCLX_EXE = 3,             /**< Executive paper size */
  EMS_PCLX_LEDGER = 4,          /**< Ledger paper size */
  EMS_PCLX_A3 = 5,              /**< A3 paper size */
  EMS_PCLX_COM10_ENV = 6,       /**< COM10 Envelope paper size */
  EMS_PCLX_MONARCH_ENV = 7,     /**< Monarch Envelope paper size */
  EMS_PCLX_C5_ENV = 8,          /**< C5 Envelope paper size */
  EMS_PCLX_DL_ENV = 9,          /**< DL Envelope paper size */
  EMS_PCLX_JISB4 = 10,          /**< JISB4  paper size */
  EMS_PCLX_JISB5 = 11,          /**< JISB5  paper size */
  EMS_PCLX_A5 = 16,             /**< A5 paper size */
  EMS_PCLX_A6 = 17,             /**< A6 paper size */
  EMS_PCLX_JISB6,               /**< JIS B6 paper size */
  EMS_PCLX_8K,                  /**< 8 gak paper size */
  EMS_PCLX_16K,                 /**< 16 gak paper size */
  EMS_PCLX_DEFAULT = 96,        /**< Default paper size */
  EMS_PCLX_STATEMENT = 99,      /**< Statement paper size */
  EMS_PCLX_8X13,                /**< Folio/Government legal paper size */
  EMS_PCLX_US_FOLIO,            /**< Foolscap paper size */
  EMS_PCLX_825X13,              /**< Folio paper size */
  EMS_PCLX_G_LG,                /**< Government legal paper size */
//  EMS_PCLX_G_LT,                /**< Government letter paper size */
  EMS_PCLX_C6_ENV,              /**< C6 Envelope paper size */
  EMS_PCLX_OFUKU,               /**< Unknown value for PCLXL???*/
  EMS_PCLX_TABLOID              /**< Unknown value for PCLXL???*/
  
};
typedef int EMS_ePCLXLsize;

/// engine_media_sizes_t will match this structure
typedef struct panda_mediasize_s
{
 unsigned int mediaSize;     ///< mediasize_t
 unsigned int width;         ///< width in mm/100 
 unsigned int height;        ///< height in mm/100  
} panda_mediasize_t;

typedef struct panda_paper_pdl_name_table_s
{
 int nMediaSizeId;         ///< mediasize_t marvell papersize key.
 int nPCL5PaperSizeID;     /* PCL 5 Paper size ID.  */
 int nPCLXLPaperSizeID;    /* PCL XL Paper size ID. */
 char szPSName[32];        /* Postscript name.    */
 char szXLName[32];        /* XL Paper size name. */ 
 
} panda_paper_pdl_name_table_t;

typedef struct panda_paper_pdl_logical_table_s
{
 int             nMediaSizeId;        ///< mediasize_t marvell papersize key.
 int             nTopUnprintable;     /**< Unprintable distance at top (microinches). */
 int             nBottomUnprintable;  /**< Unprintable distance at bottom (microinches). */
 int             nLeftUnprintable;    /**< Unprintable distance on left (microinches). */
 int             nRightUnprintable;   /**< Unprintable distance on right (microinches). */
 int             nTopLogicalPage;     /**< Physical to logical page distance at top (microinches). */
 int             nBottomLogicalPage;  /**< Physical to logical page distance at bottom (microinches). */
 int             nLeftLogicalPage;    /**< Physical to logical page distance on left (microinches). */
 int             nRightLogicalPage;   /**< Physical to logical page distance on right (microinches). */

} panda_paper_pdl_logical_table_t;

typedef struct TPandaAPI_PaperInfo {
  float fPageWidthPts;      /* Paper width in points  */ 
  float fPageHeightPts;     /* Paper height in points */
  const panda_paper_pdl_logical_table_t  *pPDLLogical; /* Unprintable, and margins (trim and blank) */
  const panda_paper_pdl_name_table_t     *pPDLNames;   /* PDL ID's and strings */
} TPandaAPI_PaperInfo;

typedef struct TPandaAPI_PageInfo {
  int nJobNum;           // Job number
  int nSubJobNum;        // Sub job number
  int nPageNum;          // Page number

  //. Expect the following values when the raster callback function is called.
  int nImageWidthPix;    // Width of page... not width of raster data. 
  int nDataWidthBytes;   // Width of raster data... not width of page
  int nImageHeightLines; // Height of page and raster data
  int nColorants;        // Number of colors
  int nBPP;              // Bits Per Pixel Per Color
} TPandaAPI_PageInfo;

typedef struct TPandaAPI_Band {
  int nJobNum;           // Job number
  int nSubJobNum;        // Sub job number
  int nPageNum;          // Page number
  int nBandNum;          // Band number
  int nImageWidthPix;    // Width of page... not width of raster data
  int nDataWidthBytes;   // Width of raster data... not width of page
  int nImageHeightLines; // Height of page and raster data
  int nBandHeightLines;  // Height of the valid lines in this band 
                         //   The last band is often shorter than previous bands. However,
                         //   the memory block is usually the same size as previous bands.
  int nColorants;        // Number of colors
  int nBPP;              // Bits Per Pixel Per Color
  int nValidColorants;   // Bit fields... 0RGB CMYK.  e.g. 0x70 band data contains RGB. 0x04 band data is just Magenta.
  int bPleaseFreeThisBand; // If zero then it is not safe to delete the pBandData. If not zero, then pBandData needs freeing
  char *pBandData;       // Raster Data
} TPandaAPI_Band;

#define PandaAPI_PARSEREAD_SOME 1 /* PARSEREAD_SOME */ 
#define PandaAPI_PARSEREAD_ANY  2 /* PARSEREAD_ANY */ 
     
/* The details of the selected media size that is to be used */
typedef struct {
  TPandaAPI_PaperInfo tPaperInfo;
  int   nTrayInput;         /* Engine is using this input tray */
  int   bPaperRotated;      /* Set to 1 to rotate the page for LEF <--> SEF fed paper */
} TPandaAPI_Media_FromEngine;

/* Given these parameters, find the most suitable media size to use */
typedef struct {
  float fPageWidthPts;      /* Job requested page size */
  float fPageHeightPts;
  int   nTrayInput;         /* Job requested input tray */
  char  szMediaType[32];    /* Job requested media type */
} TPandaAPI_Media_FromJob;
    
typedef struct {
  int nResolution;          /* e.g. PJL resolution (only one res in pjl) */
  int nDuplex;              /* e.g. PJL duplex */
  int nCollate;             /* e.g. PJL collate */
  int nTumble;              /* e.g. PJL tumble */
  int nCopies;              /* e.g. PJL copies */
  int nTrayInput;           /* e.g. PJL media source/input tray */
  int nMediaType;           /* media type array ID */
  int nPaper;               /* paper config array ID */
  TPandaAPI_Media_FromEngine tDefaultMedia; /* PJL sets this */
  int nInputMethod;         /* 0 for stream on pipe, 1 for job on local disk */
  char szJobPathnameOnDisk[256];  /* Pathname of job on local disk. Plus null. */ 
  int nTestPageJob;         /* 0 for stream on pipe, 1 for embedded config job, 2 for embedded PS font list job, 3 for embedded PCL font list job. */
} TPandaAPI_JobSettings_FromEngine;

typedef struct {
  int nResolutionX;
  int nResolutionY;
  int nDuplex;
  int nCollate;
  int nTumble;
  int nCopies;
  int nTrayInput;           /* e.g. PJL media source/input tray */
  int nMediaType;
  int nPaper;               /* paper config array ID */
  TPandaAPI_Media_FromJob *pMediaFromJob;          /* see PandaAPI_YourChanceToSelectMedia_Callback */
  TPandaAPI_Media_FromEngine *pMediaFromEngine;    /* see PandaAPI_YourChanceToSelectMedia_Callback */
} TPandaAPI_JobSettings_FromJob;


void PandaAPI_NewJob_Callback(IOReg *pipe, /* in */
                              int jobNum,  /* in */
                              TPandaAPI_JobSettings_FromEngine **ppEngineSettings  /* out */);
  void PandaAPI_NewSubJob_Callback(IOReg *pipe, int jobNum, int subjobNum);
    void PandaAPI_NewPage_Callback(IOReg *pipe /* in */, 
                                   TPandaAPI_PageInfo *pPageInfo /* in */, 
                                   TPandaAPI_JobSettings_FromJob *pPDLJobSettings /* in */,
                                   TPandaAPI_JobSettings_FromEngine **ppEngineSettings /* out */);
      void PandaAPI_Raster_Callback(IOReg *pipe, TPandaAPI_Band *pBand);
    void PandaAPI_EndPage_Callback(IOReg *pipe, int jobNum, int subjobNum, int pageNum);
  void PandaAPI_EndSubJob_Callback(IOReg *pipe, int jobNum, int subjobNum, int err);
void PandaAPI_EndJob_Callback(IOReg *pipe, int jobNum, int err);

void PandaAPI_CancelJobRequest_Callback();
int PandaAPI_YourChanceToAbort_Callback();

#ifdef FREETYPE
int PandaAPI_YourChanceToChangeStride_Callback(int nWidthBytes,
                                               int *nLineStrideBytes);
#else
int PandaAPI_YourChanceToChangeStride_Callback(int nMode, int nBPP, int nWidth,
                                               int *nLineBytes, int *nLineStrideBytes, int *nColorantStride);
#endif

int PandaAPI_YourChanceToSelectMedia_Callback(IOReg *pipe, 
                                              TPandaAPI_Media_FromJob *pMediaFromJob       /* in */,
                                              TPandaAPI_Media_FromEngine *pMediaFromEngine /* out */); /* Used with "-p 2" command line argument only */

int PandaAPI_GetPaperInfo_Callback(int nPaperIndex, TPandaAPI_PaperInfo *pPaperInfo );
int PandaAPI_GetCustomPaperInfo_Callback(IOReg *pipe, TPandaAPI_PaperInfo *pPaperInfo );
int PandaAPI_GetDefaultPaperInfo_Callback(IOReg *pipe, TPandaAPI_PaperInfo *pPaperInfo );

void *PandaAPI_malloc(int cbSize);
void PandaAPI_free(void *ptr);

void *PandaAPI_PipeHandle();
int PandaAPI_PipeReadConnect(void *pipeHandle, char *pBuffer, int cbBuffer, int nReadMode);
int PandaAPI_PipeRewindData(void *pipeHandle, char *pBuffer, int nBytes);
int PandaAPI_PipeWriteConnect(void *pipeHandle, char *pSendBuf, int nBytesToWrite);

void PandaAPI_DebugStats();
void PandaAPI_Printf(char *str,...);

const panda_paper_pdl_logical_table_t * paper_map_mediaSize_to_panda_pdl_logical( int mediaSize );
const panda_paper_pdl_logical_table_t * paper_get_panda_pdl_logical( int mediaSize );

const panda_paper_pdl_name_table_t * paper_map_mediaSize_to_panda_pdl_name( int mediaSize );
const panda_paper_pdl_name_table_t * paper_get_panda_pdl_name( int mediaSize );
EMS_ePCL5size paper_map_mediaSize_to_EMS_ePCL5size( int mediaSize );
EMS_ePCL5size paper_get_EMS_ePCL5size( int mediaSize);
EMS_ePCLXLsize paper_map_mediaSize_to_EMS_ePCLXLsize( int mediaSize );
EMS_ePCLXLsize paper_get_EMS_ePCLXLsize( int mediaSize);
const char * paper_map_mediaSize_to_szPSName( int mediaSize );
const char * paper_get_szPSName( int mediaSize );
const char * paper_map_mediaSize_to_szXLName( int mediaSize );
const char * paper_get_szXLName( int mediaSize );


#endif

