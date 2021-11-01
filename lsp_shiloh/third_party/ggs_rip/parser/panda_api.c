/* Copyright (C) 2011-2013 Global Graphics Software Ltd. All rights reserved.
*
* This example is provided on an "as is" basis and without
* warranty of any kind. Global Graphics Software Ltd. does not
* warrant or make any representations regarding the use or results
* of use of this example.
*
* Portions Copyright (c) 2011-2013 Marvell International, Ltd. All Rights Reserved
*/

#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#undef threadx

#ifdef threadx
#include "tx_api.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "logger.h"

#else
#include <stdlib.h>
#endif

#include "agConnectMgr.h"	// req.  jcshin for e_doc type		2013.12.18	by Juny

#include "panda_api.h"
#ifdef HAVE_MARVELL_SDK
#include "lassert.h"
#include "print_job_api.h"
#include "dma_buffer.h"
#include "PrintJobInfo.h"
#include "debug.h"
#include "file_to_pipe_api.h"
#endif

#ifdef HAVE_IMAGELOG
//#include "imagelog.h"
#endif

#ifdef HAVE_MARVELL_SDK
extern IOReg *gGGSPipe;
#ifdef PAPERLESSPATH
print_job_t *job = NULL; // \todo Tidy this global

#define PJL_TRAY	0

#define APIDEBUG
#ifdef APIDEBUG
int gcbSize;
#endif

#define DBG_PRFX "ggs: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 22 ) 
#include "logger.h"

#endif
#endif

extern char _File_To_Save[256]; /*file paht to save into disk*/
extern int PRNDUMPENABLE; /*defined in PrinMgr.c*/

int IsPCImagePrint =0; /*special case when image file is received from pc but not a PJL*/

extern int ggs_global_testpage; /* see ggs_parser.c */
extern int ggs_resolution;
static void Handle_Raster_MONO1(TPandaAPI_Band *pBand);
static void Handle_Raster_MONO8(TPandaAPI_Band *pBand);
static void Handle_Raster_RGB24(TPandaAPI_Band *pBand);
static void Handle_Raster_CMYK1(TPandaAPI_Band *pBand);
void (*l_pfnRasterHandler)(TPandaAPI_Band *pBand);

void PandaAPI_Printf(char *str,...)
{
#ifdef HAVE_MARVELL_SDK
  if ( DBG_WOULD_PRINTF( LOG_DEBUG ) )
  {
    char buf[8000]; 
    int len;
    va_list args;
    va_start(args, str);
    len = vsnprintf(buf,sizeof(buf)-1,str,args);
    buf[sizeof(buf)-1]='\0';
    if(len >= sizeof(buf)) {
      my_dbg_printf("PandaAPI_Printf: vsprintf would have overrun, required %d, buffer %d. Message truncated.\n", len, sizeof(buf));
    }
    my_dbg_printf("%s", buf);
    va_end(args);
  }
#else
  char buf[8000]; 
  int len;
  va_list args;
  va_start(args, str);
  len = vsnprintf(buf,sizeof(buf)-1,str,args);
  buf[sizeof(buf)-1]='\0';
  if(len >= sizeof(buf)) {
    printf("PandaAPI_Printf: vsprintf would have overrun, required %d, buffer %d. Message truncated.\n", len, sizeof(buf));
  }
  printf("%s", buf);
  va_end(args);
#endif
}

void PandaAPI_DebugStats() {
#if 0
  void PMS_GGS_DisplayStdLibCounts();
  void PMS_GGS_DisplayLibUnresolvedCounts();
  PMS_GGS_DisplayStdLibCounts();
  PMS_GGS_DisplayLibUnresolvedCounts();
#endif
}

void Do_A_Simple_Test() {
long long a;
float f=1234567.0;
float d=0.00001;
#ifdef threadx
DBG_PRINTF_ALERT("%f\n", f);
#else
printf("%f\n", f);
#endif
for (a=0; a<1000000000LL; a++) {
 f=f*d;
}

#ifdef threadx
DBG_PRINTF_ALERT("%f\n", f);
#else
printf("%f\n", f);
#endif
}

int oneshot = 0;
extern int force_copies;
extern bool force_collate;
extern bool force_duplex;
int pjl_tray = 0;

#ifdef HAVE_MARVELL_SDK
extern MODULE_ID gDestination; // For callback \todo Tidy this up         
#endif


static TPandaAPI_JobSettings_FromEngine my_EngineSettings;
static int SindohGGSPaperSize = -1;			// 2014.01.10		by Juny
static uint32_t PAGE_COUNT=1;
static int isDuplexJob=0;

void SetMediaFromEngine(TPandaAPI_Media_FromEngine *pMediaFromEngine, int nPaper) {
  uint32_t width, height;
  float fwidth, fheight;
  error_type_t enum_match_found;
  if ( nPaper == MEDIASIZE_ANY ) {
    //nPaper = MEDIASIZE_LETTER; // language/country default A4/LETTER lsptodo:
    nPaper = MEDIASIZE_A4; // language/country default A4/LETTER lsptodo:
  }
  
  enum_match_found = paper_get_media_size( nPaper, &width, &height );
  SindohGGSPaperSize = nPaper;			// 2014.01.10		by Juny
  DBG_PRINTF_ERR( "%s: media id %d = %d\n", __FUNCTION__, nPaper, SindohGGSPaperSize);
// Workaround to get whole suite output even if it's not quite perfect yet.  
#if 0
  DBG_ASSERT( enum_match_found == OK );
#else
  if(enum_match_found != OK) {
    // \todo HELP... user intervention required or something else
    // e.g. job specified A3 
    DBG_PRINTF_ERR( " %s HELP... No suitable media size found. Using Letter instead.\n", __FUNCTION__, 
                        nPaper, width, height );
    nPaper = MEDIASIZE_LETTER;
    enum_match_found = paper_get_media_size( nPaper, &width, &height );
  }
#endif
  
  DBG_PRINTF_INFO( " %s Media %d, %d x %d 100's mm\n", __FUNCTION__, 
                      nPaper, width, height );
  
  fwidth = width / 2540.0f * 72.0f; // 100xmm -> 72dpi
  fheight = height / 2540.0f * 72.0f; // 100xmm -> 72dpi
  
  pMediaFromEngine->tPaperInfo.fPageWidthPts = fwidth;      /* Engine is using this page size */
  pMediaFromEngine->tPaperInfo.fPageHeightPts = fheight;
  pMediaFromEngine->nTrayInput = 1;         /* Engine is using this input tray */
 
  pMediaFromEngine->tPaperInfo.pPDLLogical = paper_get_panda_pdl_logical( nPaper );; 
  pMediaFromEngine->tPaperInfo.pPDLNames = paper_get_panda_pdl_name( nPaper );; 

// Workaround to get whole suite output even if it's not quite perfect yet.  
#if 0
  DBG_ASSERT( pMediaFromEngine->tPaperInfo.pPDLLogical );
  DBG_ASSERT( pMediaFromEngine->tPaperInfo.pPDLNames );
#else
  if(pMediaFromEngine->tPaperInfo.pPDLLogical == NULL) {
    DBG_PRINTF_ERR( " %s Media %d, has no pdl logical definition. Using Letter logical definitions instead.\n", __FUNCTION__, 
                        nPaper, width, height );
    pMediaFromEngine->tPaperInfo.pPDLLogical = paper_get_panda_pdl_logical( MEDIASIZE_LETTER );
    pMediaFromEngine->tPaperInfo.pPDLNames = paper_get_panda_pdl_name( MEDIASIZE_LETTER );
  }
#endif

  DBG_PRINTF_INFO( " %s Media %d, %d x %d 100's mm, \"%s\"\n", __FUNCTION__, 
                      nPaper, width, height, 
                      pMediaFromEngine->tPaperInfo.pPDLNames->szXLName );
  
  
  pMediaFromEngine->bPaperRotated = 0;      /* Set to 1 to rotate the page for LEF <--> SEF fed paper */
  
}

extern int l_nArg_x;
static bool normal_halftone = true; //false;

void GetEngineSettings(IOReg *pipe, TPandaAPI_JobSettings_FromEngine *pEngineSettings) {
  extern char gszFileOnDisk[];
  extern int ggs_global_testpage; /* see ggs_parser.c */
  ASSERT( pEngineSettings );

  DBG_PRINTF_INFO( " %s (0x%p)\n", __FUNCTION__, pEngineSettings);

  pEngineSettings->nResolution = ggs_resolution; 
  pEngineSettings->nDuplex = PrintJobInfo_int_field_get( pipe->pjob_info, DUPLEX ); 
  isDuplexJob=pEngineSettings->nDuplex ;// static variable to have duplex job information
  pEngineSettings->nCollate = PrintJobInfo_int_field_get( pipe->pjob_info, QTY ); 
  DBG_PRINTF_ERR("PandaAPI QTY %d\n", pEngineSettings->nCollate);		// 2013.09.13		by Juny
  pEngineSettings->nTumble = PrintJobInfo_int_field_get( pipe->pjob_info, BINDING ); 
  if (pEngineSettings->nCollate) 
      pEngineSettings->nCopies = pEngineSettings->nCollate;
  else
      pEngineSettings->nCopies = PrintJobInfo_int_field_get( pipe->pjob_info, COPIES ); 
  pEngineSettings->nCopies = pEngineSettings->nCopies > 0 ? pEngineSettings->nCopies : 1;
  
  if ( pEngineSettings->nCollate == 1 )
    pEngineSettings->nCollate = 0;
  
  // \todo Check and test media type and source/input tray.
  pEngineSettings->nMediaType = PrintJobInfo_int_field_get( pipe->pjob_info, MEDIATYPE ); 
  
  /// 2015 inhwan5
  pjl_tray = PrintJobInfo_int_field_get( pipe->pjob_info, MEDIASOURCE ); 
  if ( pjl_tray == 4 )
	pjl_tray = 0;
  else
	pjl_tray += 1;

  DBG_PRINTF_ERR( " %s [%d]: pjl_tray = %d\n", __FUNCTION__, __LINE__, pjl_tray);
  
  pEngineSettings->nTrayInput = TRAY_ANY;
  // 1 bit or 2 bit halftone based on PJL resolution 1200 implies: 600x2bit
  normal_halftone = (PrintJobInfo_int_field_get( pipe->pjob_info, RESOLUTION ) == 600 );
  pEngineSettings->nPaper = PrintJobInfo_int_field_get( pipe->pjob_info, PAPER ); 
  DBG_PRINTF_INFO( " %s pEngineSettings->nPaper = %d\n", __FUNCTION__, pEngineSettings->nPaper);
  SetMediaFromEngine(&pEngineSettings->tDefaultMedia, pEngineSettings->nPaper);

  if(strlen(gszFileOnDisk) > 0) {
    pEngineSettings->nInputMethod = 1;  /* 0 for stream on pipe, 1 for job on local disk */
    strcpy(pEngineSettings->szJobPathnameOnDisk, gszFileOnDisk);  /* Pathname of job on local disk */ 
  } else {
    pEngineSettings->nInputMethod = 0;
    strcpy(pEngineSettings->szJobPathnameOnDisk, "");  
  }


  // nTestPageJob...
  //    0 = no font list (typical, do job on pipe), 
  //    1 = built-in config page job (pipe stream is flushed?) 
  //    2 = built-in PS font list job (pipe stream is flushed?) 
  //    3 = built-in PCL font list job (pipe stream is flushed?) 
  //  pEngineSettings->nTestPageJob = 0; /* Ignore test page setting */
  //  pEngineSettings->nTestPageJob = 1; /* Force Config page */
  //  pEngineSettings->nTestPageJob = 2; /* Force PS Font List */
  //  pEngineSettings->nTestPageJob = 3; /* Force PCL Font List */
  pEngineSettings->nTestPageJob = ggs_global_testpage;
  
  DBG_PRINTF_INFO( " %s pEngineSettings->nTestPageJob = %d", __FUNCTION__, pEngineSettings->nTestPageJob);
}


void SetJobFromPDL(print_job_t *job, TPandaAPI_JobSettings_FromJob *pPDLJobSettings) {
  int resolutionyyj = 0;
  ASSERT( pPDLJobSettings );

  DBG_PRINTF_INFO( " %s GetEngineSettings(0x%p)\n", __FUNCTION__, pPDLJobSettings);

  /* /todo Apply any job overrides
  */

/* new job log 	2013.12.27	by Juny */
  DBG_PRINTF_ERR( "@@ Pandajob : %d (%d %d) %d %d %d\n", pPDLJobSettings->nCopies, pPDLJobSettings->nDuplex, pPDLJobSettings->nTumble,
      pPDLJobSettings->nCollate, my_EngineSettings.nMediaType, pPDLJobSettings->pMediaFromJob->nTrayInput );
  print_job_set_num_copies( job, pPDLJobSettings->nCopies );
  print_job_set_duplex_and_binding( job, pPDLJobSettings->nDuplex, pPDLJobSettings->nTumble );
  print_job_set_collation( job, pPDLJobSettings->nCollate );

//  DBG_PRINTF_ERR( "%s: job media id %d = %d\n", __FUNCTION__, my_EngineSettings.nPaper , pPDLJobSettings->nPaper );
// remove here to there before call this	@@ 2013.11.19		by Juny
//  print_job_set_media_size( job, my_EngineSettings.nPaper );
  print_job_set_media_type( job, my_EngineSettings.nMediaType );
  print_job_set_default_source( job, pPDLJobSettings->pMediaFromJob->nTrayInput );
  resolutionyyj = ( normal_halftone == 1 ? 600 : normal_halftone == 0 ? 1200 : 600 );
  DBG_PRINTF_DEBUG("halftone %d is %d\n", normal_halftone, resolutionyyj);
  print_job_set_image_resolution( job, resolutionyyj );	// 2013.10.31		by Juny

  // pPDLJobSettings->pMediaFromJob
// tray 
// media
//
}


void PandaAPI_NewJob_Callback(IOReg *pipe, /* in */
                              int jobNum,  /* in */
                              TPandaAPI_JobSettings_FromEngine **ppEngineSettings  /* out */) 
{
  DBG_PRINTF_ERR( "@@ Panda Job start \n");		// @@ 2013.10.23		by Juny
  PandaAPI_Printf("PandaAPI_NewJob_Callback(0x%p, %d)\n", pipe, jobNum);
  *ppEngineSettings = &my_EngineSettings;  
  GetEngineSettings( pipe, *ppEngineSettings );
}

void PandaAPI_NewSubJob_Callback(IOReg *pipe, int jobNum, int subjobNum) {
  DBG_PRINTF_ERR( "@@ Panda subJob start \n");		// @@ 2013.10.23		by Juny
  PandaAPI_Printf("PandaAPI_NewSubJob_Callback(0x%p, %d, %d)\n", pipe, jobNum, subjobNum);
}

void PandaAPI_CancelJobRequest_Callback() {
  // dbg_printf("PandaAPI_CancelJobRequest_Callback()\n");
  // \todo Not yet called.

//  reset_file_print_setting();		/* maybe not needed 	@@ for UI print		2013.11.06		by Juny */
#ifdef PAPERLESSPATH
	// mp_parser_send_cancelmsg(); 
#endif
}

int PandaAPI_YourChanceToAbort_Callback() {
    extern bool ggs_global_cancel_request;
    
    if (ggs_global_cancel_request) {
	ggs_global_cancel_request = 0;

    	DBG_PRINTF_INFO("PandaAPI_Chance_To_Abort_Callback()... aborting\n");
	return 1;
    }

    return 0;
}

#ifdef HAVE_MARVELL_SDK
// yeah old global ...

print_page_t *p_page = 0;
extern bool force_duplex;
extern int force_copies;
extern bool force_collate;
int nDataWidthBytes_backup;
int nImageHeightLines_backup;
int nBPP_backup;
int tempTrayValue_backup;
int nMediaType_backup;
//int tempOutbin_backup;
int tmpPaperSize;
#endif

/* imageWidthPix is the page as specified by the user's document
   dataWidthBytes is the width of the raster data
 */
void PandaAPI_NewPage_Callback(IOReg *pipe /* in */, 
                               TPandaAPI_PageInfo *pPageInfo /* in */, 
                               TPandaAPI_JobSettings_FromJob *pPDLJobSettings /* in */,
                               TPandaAPI_JobSettings_FromEngine **ppEngineSettings /* out */)
{
 
  int TempDarkness = 6;
  int tempTrayValue = 8;
  uint32_t economode = 0, darkness = 6;
  int PDF_JPG_type = 0;		// 2013.11.19		by Juny
  int fileprint_height = -1;
  int fileprint_width = -1;

  DBG_PRINTF_ERR( "@@ Panda Page start \n");		// @@ 2013.10.23		by Juny
  PandaAPI_Printf("PandaAPI_NewPage_Callback(0x%p, %d, %d, %d, %d, %d, %d, %d)\n", 
    pipe, 
    pPageInfo->nJobNum, 
    pPageInfo->nPageNum, 
    pPageInfo->nImageWidthPix, 
    pPageInfo->nDataWidthBytes, 
    pPageInfo->nImageHeightLines, 
    pPageInfo->nColorants, 
    pPageInfo->nBPP);

  /* @@ trans GG tray setting to Marvell tray setting */
  switch (pPDLJobSettings->pMediaFromJob->nTrayInput)
  {
	  case 0:
			tempTrayValue = TRAY_ANY;	//1;
		break;
	  case 2:
			tempTrayValue = TRAY_1;
		break;
	  case 3:
			tempTrayValue = TRAY_2;
	    break;
	  case 4:
			tempTrayValue = TRAY_3;
	    break;
	  case 5:
			tempTrayValue = TRAY_4;
		break;
  }

  if(IsPCImagePrint)
  {
	tempTrayValue = TRAY_1;
	DBG_PRINTF_ERR("use MPT for pc image print\n");
  }
  if ( oneshot == 0 ) {	

	  job = print_job_construct( mlimiter_by_name("print"), pipe ); 
	  ASSERT( job );

/* Job informations 	@@ 2013.11.08	 	by Juny */
	  economode = PrintJobInfo_int_field_get( pipe->pjob_info, ECONOMODE );
	  darkness = PrintJobInfo_int_field_get( pipe->pjob_info, TONERDARKNESS );
	  TempDarkness = (int)darkness;
//	  DBG_PRINTF_DEBUG("get ECONOMODE %d DARKNESS %d \n", economode, TempDarkness);

/* Job informations if UI print	 	@@ 2013.11.08	 	by Juny */
     set_file_print_setting( &(tempTrayValue) , &TempDarkness , &(pPDLJobSettings->nCollate) , 
            &(pPDLJobSettings->nTumble) , &(pPDLJobSettings->nDuplex) , &(pPDLJobSettings->nCopies) , &(PDF_JPG_type), &(fileprint_height), &(fileprint_width) );

/* set paper as A4 when  @@2013.11.19	by Juny  */ 
// remove here to there before call this
//  print_job_set_media_size( job, my_EngineSettings.nPaper );
// if PDF & size is 258 (US Folio), then 
// if JPG, then 
#if 0		// 2014.01.10		by Juny
      if ( PDF_JPG_type == e_USBDRIVEJPGPAGE 
			|| ( PDF_JPG_type == e_USBDRIVEPDFPAGE && my_EngineSettings.nPaper == 0 ) )
      {
        DBG_PRINTF_ERR( "media size is %d <- %d \n", my_EngineSettings.nPaper, 9 );
		  pPDLJobSettings->nPaper = my_EngineSettings.nPaper = 9;
      }

		  print_job_set_media_size( job, my_EngineSettings.nPaper /* as A4 */ );
#else		// 2014.01.10		by Juny
      if ( PDF_JPG_type == e_USBDRIVEJPGPAGE 
			|| ( PDF_JPG_type == e_USBDRIVEPDFPAGE && SindohGGSPaperSize == 0 ) )
      {
        DBG_PRINTF_ERR( "media size is %d <- %d \n", SindohGGSPaperSize, 9 );
		  pPDLJobSettings->nPaper = SindohGGSPaperSize = 9;
      }
	  print_job_set_media_size( job, SindohGGSPaperSize /* as A4 */ );
#endif

      DBG_PRINTF_ERR( "file2print job %d %d %d %d %d %d %d (%d) (%d , %d)\n",
			  tempTrayValue , TempDarkness , pPDLJobSettings->nCollate , 
            pPDLJobSettings->nTumble , pPDLJobSettings->nDuplex , pPDLJobSettings->nCopies , PDF_JPG_type ,
			  economode, pPDLJobSettings->nPaper , pPDLJobSettings->pMediaFromJob->nTrayInput );

/* set print mgr. with Job informations	 	@@ 2013.11.08	 	by Juny */
     DBG_PRINTF_ERR("GG parser  diff margin = not set always\n");
     print_job_set_parser_info(job, 0);
     print_job_set_job_info(job, PRINTJOB);
     print_job_set_toner_save(job, economode);
     print_job_set_darkness_level(job, TempDarkness);
     
	if ( (SindohGGSPaperSize!=1) && (SindohGGSPaperSize!=9) && (SindohGGSPaperSize!=5) && (SindohGGSPaperSize!=14) && (SindohGGSPaperSize!=258) )	//A4, Letter, Legal, US Folio(8.5*13)
	{
		pPDLJobSettings->nDuplex = 0;
		DBG_PRINTF_ERR("SindohGGSPaperSize = %d, nDuplex setting was deleted\n", SindohGGSPaperSize);
	}
		
	  SetJobFromPDL( job, pPDLJobSettings ); /* @@ nTrayInput dosen't effect to real printer world */

	  // GetEngineSettings(pEngineSettings);
	  *ppEngineSettings = &my_EngineSettings;  

	  print_job_start( job, gDestination );
	  ++oneshot;
  }

  l_pfnRasterHandler = NULL;
  switch(pPageInfo->nColorants) {
    case 1: 
      {
        switch(pPageInfo->nBPP) {
          case 1:
            PandaAPI_Printf("PandaAPI_NewPage_Callback set MONO1 raster handler\n");
            l_pfnRasterHandler = Handle_Raster_MONO1;
            break;
          case 8:
            PandaAPI_Printf("PandaAPI_NewPage_Callback set MONO8 raster handler\n");
            l_pfnRasterHandler = Handle_Raster_MONO8;
            break;
          default:
            break;
        }
      }
      break;
    case 3:
      PandaAPI_Printf("PandaAPI_NewPage_Callback set RGB24 raster handler\n");
      l_pfnRasterHandler = Handle_Raster_RGB24;
      break;
    case 4:
      PandaAPI_Printf("PandaAPI_NewPage_Callback set CMYK1 raster handler\n");
      l_pfnRasterHandler = Handle_Raster_CMYK1;
      break;
    default:
      break;
  }
#ifdef HAVE_MARVELL_SDK
#ifdef PAPERLESSPATH
  ASSERT( l_pfnRasterHandler );
#endif
#endif


#ifdef HAVE_MARVELL_SDK
#if 1
//  ASSERT( p_page == 0 );

  if (p_page) {
      DBG_PRINTF_ERR("p_page not zero\n");
      return;  // job cancel close incomplete page first.
  }
  p_page = print_page_construct( job ); // global job ...
  ASSERT( p_page );

  /// will need more color knobs, brighness, contrast, vivid etc...
  print_page_set_color_mode( p_page, pPageInfo->nColorants == 1 ? e_Mono : e_Color ); 
  print_page_set_colorspace( p_page, pPageInfo->nColorants == 1 ? e_subtractive_gray : e_sRGB );
  print_page_set_num_input_colors( p_page, pPageInfo->nColorants );
  print_page_set_dpi_x( p_page, ggs_resolution );
  print_page_set_dpi_y( p_page, ggs_resolution );
  print_page_set_video_x( p_page, pPageInfo->nDataWidthBytes ); // skfoo pitch nImageWidthPix );
  nDataWidthBytes_backup = pPageInfo->nDataWidthBytes;

  // bits 
  print_page_set_raster_x( p_page, pPageInfo->nDataWidthBytes * 8 ); // * pPageInfo->nColorants );

  print_page_set_image_height_lines( p_page, pPageInfo->nImageHeightLines );
  nImageHeightLines_backup = pPageInfo->nImageHeightLines;

  // really the input bpp expect the print image pipe to change this.  
  print_page_set_output_bpp( p_page, pPageInfo->nBPP ); 
  nBPP_backup = pPageInfo->nBPP;
  
  print_page_set_print_quality( p_page, normal_halftone ? PRINTQUALITY_NORMAL : PRINTQUALITY_BEST );
  //
//  DBG_PRINTF_ERR( "%s: page media id %d = %d\n", __FUNCTION__, my_EngineSettings.nPaper, pPDLJobSettings->nPaper);
#if 0
  print_page_set_media_size( p_page, my_EngineSettings.nPaper );
#else		// 2014.01.10		by Juny
  print_page_set_media_size( p_page, (mediasize_t)SindohGGSPaperSize );
  tmpPaperSize = SindohGGSPaperSize;
#endif
  print_page_set_media_type( p_page, my_EngineSettings.nMediaType );
  nMediaType_backup = my_EngineSettings.nMediaType;

#if 1		// PDL print 		2013.11.06	by Juny
  if ( pPDLJobSettings && pPDLJobSettings->pMediaFromJob ) {
//      DBG_PRINTF_INFO( "tray from pdl %d -> %d\n", pPDLJobSettings->pMediaFromJob->nTrayInput , tempTrayValue );
      //print_page_set_input_source( p_page, pPDLJobSettings->pMediaFromJob->nTrayInput );


     set_file_print_setting( &(tempTrayValue) , &TempDarkness , &(pPDLJobSettings->nCollate) , 
            &(pPDLJobSettings->nTumble) , &(pPDLJobSettings->nDuplex) , &(pPDLJobSettings->nCopies) , &(PDF_JPG_type), &(fileprint_height), &(fileprint_width)  );

#if 0
      if ( PDF_JPG_type == e_USBDRIVEJPGPAGE 
			|| ( PDF_JPG_type == e_USBDRIVEPDFPAGE && my_EngineSettings.nPaper == 0 ) )
      {
        DBG_PRINTF_ERR( "media size is %d <- %d \n", my_EngineSettings.nPaper, 9 );
		  pPDLJobSettings->nPaper = my_EngineSettings.nPaper = 9;
      }
#else		// 2014.01.10		by Juny
      if ( PDF_JPG_type == e_USBDRIVEJPGPAGE 
			|| ( PDF_JPG_type == e_USBDRIVEPDFPAGE && SindohGGSPaperSize == 0 ) )
      {
        DBG_PRINTF_ERR( "media size is %d <- %d \n", SindohGGSPaperSize, 9 );
		  pPDLJobSettings->nPaper = SindohGGSPaperSize = 9;
      }
#endif


      DBG_PRINTF_ERR( "file2print page %d %d %d %d %d %d %d (%d) (%d -> %d, %d) h=%d w=%d\n",
			  tempTrayValue , TempDarkness , pPDLJobSettings->nCollate , 
            pPDLJobSettings->nTumble , pPDLJobSettings->nDuplex , pPDLJobSettings->nCopies , PDF_JPG_type ,
			  economode, pPDLJobSettings->nPaper , SindohGGSPaperSize /* my_EngineSettings.nPaper */, pPDLJobSettings->pMediaFromJob->nTrayInput, fileprint_height, fileprint_width );

#if PJL_TRAY
      DBG_PRINTF_ERR("input source tray (%d) from PJL !\n", pjl_tray );
      print_page_set_input_source( p_page, pjl_tray );
      tempTrayValue_backup = pjl_tray;
#else
      DBG_PRINTF_ERR("input source tray (%d) from PCL !\n", tempTrayValue );
      print_page_set_input_source( p_page, tempTrayValue );
      tempTrayValue_backup = tempTrayValue;
#endif

  }
#else
  if ( pPDLJobSettings && pPDLJobSettings->pMediaFromJob ) {
      DBG_PRINTF_INFO( "tray from pdl %d\n", pPDLJobSettings->pMediaFromJob->nTrayInput );
      //print_page_set_input_source( p_page, pPDLJobSettings->pMediaFromJob->nTrayInput );
      
  int tempTrayValue = 8;
  switch (pPDLJobSettings->pMediaFromJob->nTrayInput)
  {
	  case 0:
			tempTrayValue = TRAY_ANY;	//1;
		break;
	  case 2:
			tempTrayValue = TRAY_1;
		break;
	  case 3:
			tempTrayValue = TRAY_2;
	    break;
	  case 4:
			tempTrayValue = TRAY_3;
	    break;
	  case 5:
			tempTrayValue = TRAY_4;
		break;
  }
  
#if PJL_TRAY
      DBG_PRINTF_ERR("input source tray (%d) from PJL !\n", pjl_tray );
      print_page_set_input_source( p_page, pjl_tray );
      tempTrayValue_backup = pjl_tray;
#else
      DBG_PRINTF_ERR("input source tray (%d) from PCL !\n", tempTrayValue );
      print_page_set_input_source( p_page, tempTrayValue );
      tempTrayValue_backup = tempTrayValue;
#endif

  }
#endif
  if ( (ggs_global_testpage==2) || (ggs_global_testpage==3) )
  {
	DBG_PRINTF_ERR("This is Font List (%d) !\n", ggs_global_testpage );
    print_page_set_media_default_size(p_page, 1);
  }
  
  fileprint_height *= 600;	fileprint_height /= 72;
  fileprint_width *= 600;		fileprint_width /= 72;
  
  if ( SindohGGSPaperSize == MEDIASIZE_A4_ROTATED )
  {
	  fileprint_height = 4961;
	  fileprint_width = 7040;
  }
  
  if ( (fileprint_height>0) && (fileprint_width>0) )
  {
	  DBG_PRINTF_ERR("Check rotating status! h=%d w=%d\n", fileprint_height, fileprint_width );
	  
	  if ( (fileprint_width>5200) && (fileprint_width<8500) )
	  {
		  if ( fileprint_height<5200 )
		  {
			  print_page_set_sindoh_rotation_params(p_page, 1, fileprint_height, fileprint_width );
		  }
	  }
  }
  
  print_page_start( p_page );
  DBG_PRINTF_ERR("p_page start OK\n");
  PAGE_COUNT++;
#endif

#ifdef APIDEBUG
  gcbSize=0;// checking amount of raster data recv for this page
#endif
#endif
}

void Insert_LastBlankPage( void )
{
  if (p_page) {
      DBG_PRINTF_ERR("p_page not zero for Insert_LastBlankPage\n");
      return;  // job cancel close incomplete page first.
  }

  p_page = print_page_construct( job ); // global job ...
  ASSERT( p_page );

  /// will need more color knobs, brighness, contrast, vivid etc...
  print_page_set_color_mode( p_page, e_Mono ); 
  print_page_set_colorspace( p_page, e_subtractive_gray );
  print_page_set_num_input_colors( p_page, 1 );
  print_page_set_dpi_x( p_page, ggs_resolution );
  print_page_set_dpi_y( p_page, ggs_resolution );
  print_page_set_video_x( p_page, nDataWidthBytes_backup ); // skfoo pitch nImageWidthPix );
  print_page_set_raster_x( p_page, nDataWidthBytes_backup * 8 ); // * pPageInfo->nColorants );
  print_page_set_image_height_lines( p_page, nImageHeightLines_backup );
  print_page_set_output_bpp( p_page, nBPP_backup ); 
  print_page_set_print_quality( p_page, PRINTQUALITY_NORMAL );
  print_page_set_input_source( p_page, tempTrayValue_backup );
  print_page_set_media_size( p_page, tmpPaperSize );
  print_page_set_media_type( p_page, nMediaType_backup );

  //print_page_set_output_dest(p_page, tempOutbin_backup);
  print_page_start( p_page );
  PAGE_COUNT++;
  
  compression_info_t comp_info; 
  comp_info.comp_type = COMPTYPE_NONE ;
  comp_info.comp_details.raw.Xd = nDataWidthBytes_backup;

  char * dumy_data = MEM_MALLOC(nDataWidthBytes_backup*64);
  //memset(dumy_data, 0xFF, nDataWidthBytes_backup*64);
  memset(dumy_data, 0x0, nDataWidthBytes_backup*64);
  int nLinesReceived_tmp = 0;
  bool eof;
  
  //for ( nLinesReceived_tmp=0; nLinesReceived_tmp<=64; nLinesReceived_tmp+=64 )
  //{
	  //DBG_PRINTF_ERR("nLinesReceived_tmp=%d ImageHeight=%d\n", nLinesReceived_tmp, nImageHeightLines_backup);
	  //if ( nLinesReceived_tmp < 64 )
	  //{
		//comp_info.comp_details.raw.Yd = 64; 
		//eof = false;
	  //}
	  //else
	  //{
	    //comp_info.comp_details.raw.Yd = 64;//nImageHeightLines_backup - nLinesReceived + 64; 
	    //eof = true;
	  //}
  /// Juniper와 다르게 nLines 만큼 dummy_data 넣어줘야 video error를 면할 수 있다
  for ( nLinesReceived_tmp=0; nLinesReceived_tmp<=nImageHeightLines_backup; nLinesReceived_tmp+=64 )
  {
	  //DBG_PRINTF_ERR("nLinesReceived_tmp=%d ImageHeight=%d\n", nLinesReceived_tmp, nImageHeightLines_backup);
	  if ( nLinesReceived_tmp < nImageHeightLines_backup )
	  {
		comp_info.comp_details.raw.Yd = 64; 
		eof = false;
	  }
	  else
	  {
	    comp_info.comp_details.raw.Yd = nImageHeightLines_backup - nLinesReceived_tmp + 64; 
	    eof = true;
	  }
 
	  //BigBuffer_t *bb = dma_buffer_adopt( dumy_data, nDataWidthBytes_backup * comp_info.comp_details.raw.Yd );
	  BigBuffer_t *bb = dma_buffer_copy_from( dumy_data, nDataWidthBytes_backup * comp_info.comp_details.raw.Yd );
	  ASSERT( bb );

	  print_plane_add_data( p_page, COLOR_BLACK, bb, bb->datalen, PLANAR_K, 
							eof, // not last strip 
							ggs_resolution, ggs_resolution, 
							nDataWidthBytes_backup, // pBand->nImageWidthPix, 
							comp_info.comp_details.raw.Yd, 
							8,  // 8 bbp 
							true,  // left to right not reversed for duplex backside
							&comp_info ); 
  }
  MEM_FREE_AND_NULL( dumy_data );
  print_page_end( p_page, NULL );
  print_page_free( p_page );
  p_page = 0;
}

static void Handle_Raster_MONO1(TPandaAPI_Band *pBand) {
  static int nLinesReceived;
#ifdef HAVE_MARVELL_SDK
  PandaAPI_Printf("Handle_Raster_MONO1(0x%p)\n  job %d, page %d, band %d, wpix %d, wbytes %d, hpage %d,  hband %d, colors %d, bpp %d, 0x%p\n", 
    pBand,
    pBand->nJobNum, 
    pBand->nPageNum, 
    pBand->nBandNum, 
    pBand->nImageWidthPix, 
    pBand->nDataWidthBytes, 
    pBand->nImageHeightLines, 
    pBand->nBandHeightLines, 
    pBand->nColorants,
    pBand->nBPP, 
    pBand->pBandData);

  //assert job
  //assert band dimensions
  bool eoi;
  compression_info_t comp_info; 
  comp_info.comp_type = COMPTYPE_NONE ;
  comp_info.comp_details.raw.Xd = pBand->nDataWidthBytes * 8; // stride in bits.
  comp_info.comp_details.raw.Yd = pBand->nBandHeightLines; 

  BigBuffer_t *bb = dma_buffer_adopt( pBand->pBandData, pBand->nDataWidthBytes * pBand->nBandHeightLines );
  ASSERT( bb );

  // \todo... If dma_buffer_adopt is doing a memcpy, then can we try to get rid it?
  //  PandaAPI_free(pBand->pBandData); // freeing the data that we thought the engine was freeing
   
  if(pBand->nBandNum==0) {
    nLinesReceived = 0;
  }
  nLinesReceived += pBand->nBandHeightLines;
  eoi = (nLinesReceived >= pBand->nImageHeightLines - 1) ? true : false;  // last strip?

  print_plane_add_data( p_page, COLOR_BLACK, bb, bb->datalen, PLANAR_K, eoi,
                        ggs_resolution, ggs_resolution, pBand->nImageWidthPix, pBand->nBandHeightLines, 
                        1,  // 1 bbp 
                        true,  // left to right not reversed for duplex backside
                        &comp_info ); 
                        
  PandaAPI_Printf("Handle_Raster_MONO1(0x%p) returning\n", pBand);
  
#endif
}

static void Handle_Raster_CMYK1(TPandaAPI_Band *pBand) {
    static int nLinesReceived;
  DBG_PRINTF_INFO("Handle_Raster_CMYK1(0x%p)\n  job %d, page %d, band %d, wpix %d, wbytes %d, hpage %d,  hband %d, colors %d, bpp %d, 0x%p\n", 
    pBand,
    pBand->nJobNum, 
    pBand->nPageNum, 
    pBand->nBandNum, 
    pBand->nImageWidthPix, 
    pBand->nDataWidthBytes, 
    pBand->nImageHeightLines, 
    pBand->nBandHeightLines, 
    pBand->nColorants,
    pBand->nBPP, 
    pBand->pBandData);

#ifdef HAVE_MARVELL_SDK
  //assert job
  //assert band dimensions
  compression_info_t comp_info; 
  comp_info.comp_type = COMPTYPE_NONE ;
  comp_info.comp_details.raw.Xd = pBand->nDataWidthBytes * 8; // stride in bits.
  comp_info.comp_details.raw.Yd = pBand->nBandHeightLines; 




  BigBuffer_t *bb = 0; 
  char * plane_data;
  bool eoi;
  ASSERT( bb );

  if(pBand->nBandNum==0) {
    nLinesReceived = 0;
  }
  nLinesReceived += pBand->nBandHeightLines;
  eoi = (nLinesReceived >= pBand->nImageHeightLines - 1) ? true : false;  // last strip?

  plane_data = pBand->pBandData + ( pBand->nDataWidthBytes * pBand->nBandHeightLines );
  bb = dma_buffer_copy_from( plane_data, pBand->nDataWidthBytes * pBand->nBandHeightLines );
  print_plane_add_data( p_page, COLOR_MAGENTA, bb, bb->datalen, PLANAR_K, 
                        eoi, // last strip?
                        ggs_resolution, ggs_resolution, pBand->nImageWidthPix, pBand->nBandHeightLines, 
                        1,  // 1 bbp 
                        true,  // left to right not reversed for duplex backside
                        &comp_info ); 
  plane_data = pBand->pBandData + 2 * ( pBand->nDataWidthBytes * pBand->nBandHeightLines );
  bb = dma_buffer_copy_from( plane_data, pBand->nDataWidthBytes * pBand->nBandHeightLines );
  print_plane_add_data( p_page, COLOR_YELLOW, bb, bb->datalen, PLANAR_K, 
                        eoi, // last strip?
                        ggs_resolution, ggs_resolution, pBand->nImageWidthPix, pBand->nBandHeightLines, 
                        1,  // 1 bbp 
                        true,  // left to right not reversed for duplex backside
                        &comp_info ); 
  plane_data = pBand->pBandData + 3 * ( pBand->nDataWidthBytes * pBand->nBandHeightLines );
  bb = dma_buffer_copy_from( plane_data, pBand->nDataWidthBytes * pBand->nBandHeightLines );
  print_plane_add_data( p_page, COLOR_BLACK, bb, bb->datalen, PLANAR_K, 
                        eoi, // last strip?
                        ggs_resolution, ggs_resolution, pBand->nImageWidthPix, pBand->nBandHeightLines, 
                        1,  // 1 bbp 
                        true,  // left to right not reversed for duplex backside
                        &comp_info ); 

  bb = dma_buffer_adopt( pBand->pBandData, pBand->nDataWidthBytes * pBand->nBandHeightLines );
  print_plane_add_data( p_page, COLOR_CYAN, bb, bb->datalen, PLANAR_K, 
                        eoi, // last strip?
                        ggs_resolution, ggs_resolution, pBand->nImageWidthPix, pBand->nBandHeightLines, 
                        1,  // 1 bbp 
                        true,  // left to right not reversed for duplex backside
                        &comp_info ); 

#endif
}

static void Handle_Raster_MONO8(TPandaAPI_Band *pBand) {
#ifdef HAVE_MARVELL_SDK
  static int nLinesReceived;
  bool eoi;
    
  PandaAPI_Printf("Handle_Raster_MONO8(TPandaAPI_Band 0x%p = {job %d, page %d, band %d, wpix %d, wbytes %d, himage %d, hband %d, colors %d, bpp %d, valid colors %d, free Band Data %d, data 0x%p}), pipe 0x%p\n", 
    pBand,
    pBand->nJobNum, 
    pBand->nPageNum, 
    pBand->nBandNum, 
    pBand->nImageWidthPix, 
    pBand->nDataWidthBytes, 
    pBand->nImageHeightLines, 
    pBand->nBandHeightLines, 
    pBand->nColorants,
    pBand->nBPP, 
    pBand->nValidColorants,
    pBand->bPleaseFreeThisBand,
    pBand->pBandData,
    pipe);

  //assert job
  //assert band dimensions
  compression_info_t comp_info; 
  comp_info.comp_type = COMPTYPE_NONE ;
  comp_info.comp_details.raw.Xd = pBand->nDataWidthBytes; // stride in bits.
  comp_info.comp_details.raw.Yd = pBand->nBandHeightLines; 

#if 0
  if (1) { // test for white at end of every line.
      char * p = pBand->pBandData;
      int i,r;
      int width = pBand->nDataWidthBytes;
      for (i=0; i < pBand->nBandHeightLines; i++) 
	  for (r=1; r <= 8; r++ ) 
	      if ( p[width - r ] != 0 ) 
		  dbg_printf( "ggs %x at row %d  %d\n", p[width-r], i, width-r ); 
  }
#endif


  BigBuffer_t *bb = dma_buffer_adopt( pBand->pBandData, pBand->nDataWidthBytes * pBand->nBandHeightLines );
  ASSERT( bb );

  if(pBand->nBandNum==0) {
    nLinesReceived = 0;
  }
  nLinesReceived += pBand->nBandHeightLines;
  eoi = (nLinesReceived >= pBand->nImageHeightLines - 1) ? true : false;  // last strip?

  print_plane_add_data( p_page, COLOR_BLACK, bb, bb->datalen, PLANAR_K, 
                        eoi, // not last strip 
                        ggs_resolution, ggs_resolution, 
                        pBand->nDataWidthBytes, // pBand->nImageWidthPix, 
                        pBand->nBandHeightLines, 
                        8,  // 8 bbp 
                        true,  // left to right not reversed for duplex backside
                        &comp_info ); 

#endif
}

static void Handle_Raster_RGB24(TPandaAPI_Band *pBand) {
  DBG_PRINTF_INFO("Handle_Raster_RGB24(0x%p)\n  job %d, page %d, band %d, wpix %d, wbytes %d, hpage %d,  hband %d, colors %d, bpp %d, 0x%p\n", 
    pBand,
    pBand->nJobNum, 
    pBand->nPageNum, 
    pBand->nBandNum, 
    pBand->nImageWidthPix, 
    pBand->nDataWidthBytes, 
    pBand->nImageHeightLines, 
    pBand->nBandHeightLines, 
    pBand->nColorants,
    pBand->nBPP, 
    pBand->pBandData);


#ifdef HAVE_MARVELL_SDK
  //assert job
  //assert band dimensions
    static int nLinesReceived;

    compression_info_t comp_info; 
    bool eoi;

    comp_info.comp_type = COMPTYPE_NONE ;
    comp_info.comp_details.raw.Xd = pBand->nDataWidthBytes; // stride in bits.
    comp_info.comp_details.raw.Yd = pBand->nBandHeightLines; 
    
    BigBuffer_t *bb = dma_buffer_adopt( pBand->pBandData, pBand->nDataWidthBytes * pBand->nBandHeightLines * pBand->nColorants );
    ASSERT( bb );
    
    if(pBand->nBandNum==0) {
	  nLinesReceived = 0;
    }
    nLinesReceived += pBand->nBandHeightLines;
    eoi = (nLinesReceived >= pBand->nImageHeightLines - 1) ? true : false;  // last strip?

    print_plane_add_data( p_page, COLOR_BLACK, bb, bb->datalen, PACKED_RGB, 
			  eoi, // not last strip 
			  ggs_resolution, ggs_resolution, 
			  pBand->nDataWidthBytes, // pBand->nImageWidthPix, 
			  pBand->nBandHeightLines, 
			  24,  // 8 bbp and 3 components seems wrong.
			  true,  // left to right not reversed for duplex backside
			  &comp_info ); 
    
#endif

}

void PandaAPI_Raster_Callback(IOReg *pipe, TPandaAPI_Band *pBand) {
/*
  PandaAPI_Printf("PandaAPI_Raster_Callback(0x%p, job %d, page %d, band %d, wpix %d, wbytes %d, himage %d, hband %d, colors %d, bpp %d, 0x%p)\n", 
    pipe, 
    pBand->nJobNum, 
    pBand->nPageNum, 
    pBand->nBandNum, 
    pBand->nImageWidthPix, 
    pBand->nDataWidthBytes, 
    pBand->nImagedHeightLines, 
    pBand->nBandHeightLines, 
    pBand->nColorants,
    pBand->nValidColorants,
    pBand->bPleaseFreeThisBand,
    pBand->nBPP, 
    pBand->pBandData);
*/

#ifdef HAVE_MARVELL_SDK
#ifdef APIDEBUG
  gcbSize+=(pBand->nDataWidthBytes*pBand->nBandHeightLines);// double checking amount of data recv
#endif

  ASSERT( l_pfnRasterHandler );
  (*l_pfnRasterHandler)(pBand);

#ifndef PAPERLESSPATH
  if(pBand->bPleaseFreeThisBand) {
    PandaAPI_free(pBand->pBandData);
  }
#endif
#else
  (*l_pfnRasterHandler)(pBand);
  if(pBand->bPleaseFreeThisBand) {
    PandaAPI_free(pBand->pBandData);
  }
#endif
}


void PandaAPI_EndPage_Callback(IOReg *pipe, int jobNum, int subjobNum, int pageNum) 
{
#ifdef HAVE_MARVELL_SDK
#ifdef PAPERLESSPATH
  print_page_end( p_page, NULL );
  print_page_free( p_page );
  p_page = 0;
#endif
    DBG_PRINTF_ERR( "@@ Panda Page end \n");		// @@ 2013.10.14		by Juny
#endif
}

void PandaAPI_EndSubJob_Callback(IOReg *pipe, int jobNum, int subjobNum, int err) {
  DBG_PRINTF_ERR( "@@ Panda subJob end \n");		// @@ 2013.10.23		by Juny
  PandaAPI_Printf("PandaAPI_EndSubJob_Callback(0x%p, %d, %d, %d)\n", pipe, jobNum, subjobNum, err);
  if (p_page) {
      // close incomplete page on cancel.
      PandaAPI_EndPage_Callback(pipe, jobNum, subjobNum, 666); 
  }
  DBG_PRINTF_ERR("DuplexJob=%d MediaType=%d PAGE_COUNT=%d\n", isDuplexJob, my_EngineSettings.nMediaType, PAGE_COUNT);
  if ( isDuplexJob && my_EngineSettings.nMediaType==MEDIATYPE_LETTERHEAD && !(PAGE_COUNT%2) )
	Insert_LastBlankPage();
  
  reset_file_print_setting();		/* @@ for UI print		2013.11.06		by Juny */
}

/* Called when we are not going to use the pipe anymore.
   jobNum incremented every NewJob callback.
   err indicates that an issue was detected during processing.
*/
void PandaAPI_EndJob_Callback(IOReg *pipe, int jobNum, int err) {
  DBG_PRINTF_ERR( "@@ Panda Job end (PAGE_COUNT=%d)\n", PAGE_COUNT-1);
  // dbg_printf("PandaAPI_EndJob_Callback(0x%p)\n", pipe);
  IsPCImagePrint =0; /*reset pc image print value*/
  PAGE_COUNT=1;
#ifdef HAVE_MARVELL_SDK
#ifdef PAPERLESSPATH
    extern bool ggs_global_cancel;
  if ( job ) {
      job = ggs_global_cancel ? print_job_cancel(job) : print_job_end( job );
      DBG_PRINTF_NOTICE( " %s 0x%p %d %s\n", __FUNCTION__, job, jobNum, ggs_global_cancel ? "cancel" : "job"  );
  } else if (!oneshot) {
	  job = print_job_construct( mlimiter_by_name("print"), pipe ); 
	  print_job_start( job, gDestination );
	  job = print_job_end( job );
	  DBG_PRINTF_ERR( " %s job nesting error %p %d\n", __FUNCTION__, job, jobNum );
  }

  {
    MESSAGE msg;
    msg.msgType = MSG_END_PARSE;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = 0;
    oneshot = 0;
    SYMsgSend(GGS_RIP_ID, &msg); 
  }

  // err needs to bubble up to the return from parser
#endif

//  ASSERT(0); // force kill at end of job while ssh not working and neither is Ctrl-C in serial console (see Google)
#endif
}

#ifdef FREETYPE
int PandaAPI_YourChanceToChangeStride_Callback(int nWidth,
                                               int *nLineStrideBytes)
{
  uint32_t nBytes;

  if(nWidth < 600) { /* Ignore the quirkiness of setpagedevice requests by filtering out requests for less than 1 inch wide */
    return 0;
  }
  
  nBytes = (nWidth+127)&~127;

  *nLineStrideBytes = nBytes;
  PandaAPI_Printf( "ggs: nWidth %d, *nLineStrideBytes %d\n", __FUNCTION__, nWidth, *nLineStrideBytes );

  return 1;
}
#else
int PandaAPI_YourChanceToChangeStride_Callback(int nMode, int nBPP, int nWidth,
                                               int *nLineBytes, int *nLineStrideBytes, int *nColorantStride) 
{
  uint32_t nBytes;

  if(nWidth < 600) { /* Ignore the quirkiness of setpagedevice requests by filtering out requests for less than 1 inch wide */
    return 0;
  }
  
#if 1 // experiment with pip pad stefan foo
  nBytes = nWidth; /// 
  // nBytes = ((nWidth+15)&~15)/8;
// #else
  switch(nBPP) {
    case 1:
        nBytes = ((nWidth+127)&~127)/8;
      break;
    case 2:
        nBytes = ((nWidth+63)&~63)/8;
      break;
    case 4:
      nBytes = ((nWidth+31)&~31)/8;
      break;
    default:
    case 8:
      nBytes = nWidth;
      break;
    case 16:
      nBytes = nWidth*2;
      break;
  }

  // nBytes = (nBytes+31)&~31; 
  //nBytes = (nBytes+15)&~15; // cache aligned.
  // 128 byte aligned... 4992 is the target for A4 RGB24-> 1bpp
  //nBytes -= 16; 
   nBytes = (nBytes+127)&~127;
#endif

  if(nMode == 6 /* -r 6 is PMS_RGB_PixelInterleaved */) {
    *nLineStrideBytes = *nLineBytes = (nBytes*3); 
  } else {
    *nLineStrideBytes = *nLineBytes = nBytes;
  }
  *nColorantStride = nBytes;

  PandaAPI_Printf( "ggs: %s b %d w %d bytes %d stridebytes %d colorantstride %d\n", __FUNCTION__, nBytes, nWidth, *nLineBytes, *nLineStrideBytes, *nColorantStride );
  return 1;
}
#endif

int PandaAPI_GetCustomPaperInfo_Callback(IOReg *pipe, TPandaAPI_PaperInfo *pPaperInfo ) {
  DBG_PRINTF_INFO( "%s %p %p\n", __FUNCTION__, pipe, pPaperInfo);
  if(pipe) {
    my_EngineSettings.nPaper = PrintJobInfo_int_field_get( pipe->pjob_info, PAPER ); 
  } else {
    my_EngineSettings.nPaper = 9; /* Select A4 at start, before first job */
  }
  /* my_EngineSettings.nPaper = PrintJobInfo_int_field_get( pipe->pjob_info, CUSTOM ); 
  SetMediaFromEngine(&my_EngineSettings.tCustomMedia, my_EngineSettings.nPaper);
  memcpy(pPaperInfo, &my_EngineSettings.tCustomMedia.tPaperInfo, sizeof(TPandaAPI_PaperInfo)); */

  SetMediaFromEngine(&my_EngineSettings.tDefaultMedia, my_EngineSettings.nPaper);
  memcpy(pPaperInfo, &my_EngineSettings.tDefaultMedia.tPaperInfo, sizeof(TPandaAPI_PaperInfo));
  DBG_PRINTF_INFO( "pPaperInfo %f x %f src %d %s\n", __FUNCTION__, 
		     pPaperInfo->fPageWidthPts, pPaperInfo->fPageHeightPts,
		     my_EngineSettings.nInputMethod, my_EngineSettings.szJobPathnameOnDisk);
  return 0;
}

int PandaAPI_GetDefaultPaperInfo_Callback(IOReg *pipe, TPandaAPI_PaperInfo *pPaperInfo ) {
  DBG_PRINTF_INFO( "%s %p %p\n", __FUNCTION__, pipe, pPaperInfo);
  if(pipe) {
    my_EngineSettings.nPaper = PrintJobInfo_int_field_get( pipe->pjob_info, PAPER ); 
  } else {
    my_EngineSettings.nPaper = 9; /* Select A4 at start, before first job */
  }
  SetMediaFromEngine(&my_EngineSettings.tDefaultMedia, my_EngineSettings.nPaper);
  memcpy(pPaperInfo, &my_EngineSettings.tDefaultMedia.tPaperInfo, sizeof(TPandaAPI_PaperInfo));
  DBG_PRINTF_INFO( "pPaperInfo %f x %f\n", __FUNCTION__, pPaperInfo->fPageWidthPts, pPaperInfo->fPageHeightPts);
  return 0;
}

int PandaAPI_GetPaperInfo_Callback(int nPaperIndex, TPandaAPI_PaperInfo *pPaperInfo ) {
  int nNumPaperSizes = paper_get_num_media_sizes();
  mediasize_t tmp_size;
  uint32_t uWidth;
  uint32_t uHeight;
  int bSupported;
  
  if(!pPaperInfo) {
    return nNumPaperSizes;
  }

//  PandaAPI_Printf( "ggs: %s %d, %p \n", __FUNCTION__, nPaperIndex, pPaperInfo );
  
  DBG_ASSERT( nNumPaperSizes > nPaperIndex );
  
#if 0 /* info */
  int i;
  for(i=0; i<nNumPaperSizes; i++) {
    tmp_size = paper_get_entry( i );

    paper_get_media_size(tmp_size, &uWidth, &uHeight);
    bSupported = paper_engine_supports_mediasize( tmp_size );
    DBG_PRINTF_INFO( "%s: paper_get_entry(%d), %d = %d x %d, supported=%d\n", 
      __FUNCTION__, i, tmp_size, uWidth, uHeight, bSupported );
  }
#endif

  tmp_size = paper_get_entry( nPaperIndex );
  bSupported = paper_engine_supports_mediasize( tmp_size );
  if(!bSupported) {
    return -1;
  }
  
  paper_get_media_size(tmp_size, &uWidth, &uHeight);
  DBG_PRINTF_ERR( "%s: media id %d = %d\n", __FUNCTION__, nPaperIndex, tmp_size);
  pPaperInfo->pPDLLogical = paper_get_panda_pdl_logical( tmp_size );
  pPaperInfo->pPDLNames = paper_get_panda_pdl_name( tmp_size );
     
  if( !pPaperInfo->pPDLLogical ) {
    DBG_PRINTF_INFO( "%s: WARNING no pdl data for paper_get_entry(index %d) is media id %d = %s %d x %d, pdl_logical=0x%p, pdl_names=0x%p. Not adding this media to PDL paper info list\n", 
      __FUNCTION__, nPaperIndex, tmp_size, pPaperInfo->pPDLNames? pPaperInfo->pPDLNames->szPSName:"", uWidth, uHeight, pPaperInfo->pPDLLogical, pPaperInfo->pPDLNames );
    return -1;
  }

  DBG_PRINTF_INFO( "%s: paper_get_entry(index %d) is media id %d = %s %d x %d.\n", 
    __FUNCTION__, nPaperIndex, tmp_size, pPaperInfo->pPDLNames? pPaperInfo->pPDLNames->szPSName:"", uWidth, uHeight, pPaperInfo->pPDLLogical, pPaperInfo->pPDLNames );
   
  DBG_ASSERT( pPaperInfo->pPDLLogical );
  DBG_ASSERT( pPaperInfo->pPDLNames );

  pPaperInfo->fPageWidthPts = uWidth / 2540.0f * 72.0f; 
  pPaperInfo->fPageHeightPts = uHeight / 2540.0f * 72.0f; 
  
  return 0  ;
}

int PandaAPI_YourChanceToSelectMedia_Callback(IOReg *pipe, 
                                              TPandaAPI_Media_FromJob *pMediaFromJob       /* in */,
                                              TPandaAPI_Media_FromEngine *pMediaFromEngine /* out */) {
                                              
  int nRetVal = 1;
  mediasize_t mediaSize=-1;
  
  int nPageWidth_HundredthsOfAMillimeter =  (int)((pMediaFromJob->fPageWidthPts * 2540.0f)/72.0f);
  int nPageHeight_HundredthsOfAMillimeter = (int)((pMediaFromJob->fPageHeightPts * 2540.0f)/72.0f);
  UNUSED_VAR(nPageWidth_HundredthsOfAMillimeter);   // -jrs avoid compiler warnings in release build
  UNUSED_VAR(nPageHeight_HundredthsOfAMillimeter);

  DBG_PRINTF_INFO( "*************** %s in %p out %p\n", __FUNCTION__, pMediaFromJob, pMediaFromEngine );
  DBG_PRINTF_INFO( "*************** %s %f x %f points\n", __FUNCTION__, pMediaFromJob->fPageWidthPts, pMediaFromJob->fPageHeightPts );
  DBG_PRINTF_INFO( " %s Media from job %d x %d 100'sMM, Tray %d\n", __FUNCTION__, 
                      nPageWidth_HundredthsOfAMillimeter, nPageHeight_HundredthsOfAMillimeter,
                      pMediaFromJob->nTrayInput );
  
  // PDL's are rounding page sizes up, we compensate by subtracting a millimeter then the  
  // media size matching is based on mm/100 and not exceeding the papersize, rather than comparing a delta.
  // 
  pMediaFromJob->fPageWidthPts -= 2.83;  // - 1.0 mm
  pMediaFromJob->fPageHeightPts -= 2.83;

  /* Convert pdl choice to supported size in engine, using algorithm in function
     get_media_size_enum() in lsp_shiloh\common\paper\src\paper_size.c 
   */
  paper_get_media_size_enum( pMediaFromJob->fPageWidthPts, 
                             pMediaFromJob->fPageHeightPts,
                             PAPER_UNITS_72DPI,
                             &mediaSize );
                             
  DBG_PRINTF_INFO( " %s paper_get_media_size_enum says use mediaSize %d\n", __FUNCTION__, mediaSize );
                      
  /* Fill the API structure using the mediaSize obtained from paper_get_media_size_enum */
  SetMediaFromEngine(pMediaFromEngine, mediaSize);

/* effect for PS N-up 	@@ 2013.12.13		by Juny */
	  DBG_PRINTF_ERR("test tray input %d\n", pMediaFromJob->nTrayInput );
	  pMediaFromEngine->nTrayInput = (pMediaFromJob->nTrayInput);  
  
  /* \todo Loop all trays to select closest match */
  /*
    int nTolerance = 176; / * 5.0 pts (1.76 mm) * /

    if( pMediaFromJob->uInputTray ) { / * job specified input tray * /
        if (pMediaFromJob->uInputTray == 7) 
          i = A3TrayPosition;
        else
          i = (int)(pMediaFromJob->uInputTray -2);

        g_pstTrayInfo[i].ePaperSize = GetTrayInfo(i);
        GetPaperInfo(g_pstTrayInfo[i].ePaperSize, &pPaperInfo);

        if((fabs(nPageWidth_HundredthsOfAMillimeter - pPaperInfo->width) < nTolerance) &&
           (fabs(nPageHeight_HundredthsOfAMillimeter - pPaperInfo->height) < nTolerance )) {
          bTraySelected = 1;
          *ppTrayInfo = &g_pstTrayInfo[i];
          PMS_TRAY_TRACE("PMS_MediaSelect_D, Tray(index %d) selected based on size within tolerance\n", i);
        }
        / * if no match to a tray, then see if a rotated match can be found * /
        else if((fabs(nPageWidth_HundredthsOfAMillimeter - pPaperInfo->dHeight) < nTolerance) &&
           (fabs(nPageHeight_HundredthsOfAMillimeter - pPaperInfo->dWidth) < nTolerance )) {
          bTraySelected = 1;
          *rotate = 1;
          *ppTrayInfo = &g_pstTrayInfo[i];
          PMS_TRAY_TRACE("PMS_MediaSelect_D_r, Tray (index %d) selected based on rotated size within tolerance\n", i);
        }
     } else {
        LOOP FOR ALL TRAYS
        IS MATCH?
        IS ROTATED MATCH?
     }     
  */

  return nRetVal;
}
 
#define USE_STATIC_RIPMEMPOOL 0
#if USE_STATIC_RIPMEMPOOL
static void *ggs_rip_mem_pool = 0;
static int ggs_rip_mem_pool_size = 0;
#endif
void *PandaAPI_malloc(int cbSize) {
  void *p;
#if 0  // ifdef HAVE_MARVELL_SDK
  p = (void*)MEM_MALLOC_LIMITED( mlimiter_by_name("printer"), cbSize );
#else 
#if USE_STATIC_RIPMEMPOOL
  if (ggs_rip_mem_pool && cbSize == ggs_rip_mem_pool_size) {
	  p = ggs_rip_mem_pool;
  } else if (!ggs_rip_mem_pool && cbSize > 1024*1024) {
	  ggs_rip_mem_pool = malloc(cbSize);
	  ggs_rip_mem_pool_size = cbSize;
	  p = ggs_rip_mem_pool;
  }
#endif
  p = (void*)malloc(cbSize);
#endif
/*  PandaAPI_Printf("PandaAPI_malloc(%d) returning %p\n", cbSize, p); */
  return p;
}

void PandaAPI_free(void *ptr) {
#if 0 // ifdef HAVE_MARVELL_SDK
  MEM_FREE_AND_NULL(ptr);
#else
#if USE_STATIC_RIPMEMPOOL 
  if ( ptr != ggs_rip_mem_pool )
	  free(ptr);
#else
  free(ptr);
#endif
#endif
}

void *PandaAPI_PipeHandle() {
#ifdef HAVE_MARVELL_SDK
  return (void*)gGGSPipe;
#else
  return NULL;
#endif
}

int PandaAPI_PipeReadConnect(void *pipeHandle, char *pBuffer, int cbBuffer, int nReadMode) {
#ifdef HAVE_MARVELL_SDK
//  DBG_PRINTF_ALERT("PandaAPI_PipeReadConnect(), gGGSPipe=%p, pipeHandle=%p\n", gGGSPipe, pipeHandle);
#if 0		// for log 		2013.12.03		by Juny
  return ((IOReg *)pipeHandle)->ReadConnect((IOReg *)pipeHandle, pBuffer, cbBuffer, nReadMode);
#else
//  DBG_PRINTF_ERR("PandaAPI_PipeReadConnect(), gGGSPipe=%p, pipeHandle=%p\n", gGGSPipe, pipeHandle);
  int32_t return_agread = 0;

  return_agread = ((IOReg *)pipeHandle)->ReadConnect((IOReg *)pipeHandle, pBuffer, cbBuffer, nReadMode);
  if (return_agread == 0)
    DBG_PRINTF_ERR("agread =%d\n", return_agread);
 
  if(PRNDUMPENABLE) 
  {  
	  FILE * fp=NULL;
	  fp =fopen(_File_To_Save,"ab");
	  if(fp )
	  {
		  if(return_agread>0)
		  {
			  fwrite(pBuffer,1,return_agread,fp);
		  }
		  fclose(fp);
	  }
  }
  return return_agread;
#endif
#else
  PandaAPI_Printf("*** TODO  %s simulate %s\n", __FILE__, __FUNCTION__);
  return -1;
#endif
}

int PandaAPI_PipeRewindData(void *pipeHandle, char *pBuffer, int nBytes) {
#ifdef HAVE_MARVELL_SDK
//  DBG_PRINTF_ALERT("PandaAPI_PipeRewindData(), gGGSPipe=%p, pipeHandle=%p\n", gGGSPipe, pipeHandle);
  return ((IOReg *)pipeHandle)->RewindData((IOReg *)pipeHandle, pBuffer, nBytes);
#else
  PandaAPI_Printf("*** TODO  %s simulate %s\n", __FILE__, __FUNCTION__);
  return 0;
#endif
}

int PandaAPI_PipeWriteConnect(void *pipeHandle, char *pSendBuf, int nBytesToWrite) {
#ifdef HAVE_MARVELL_SDK
//  DBG_PRINTF_ALERT("PandaAPI_PipeWriteConnect(), gGGSPipe=%p, pipeHandle=%p\n", gGGSPipe, pipeHandle);
  return ((IOReg *)pipeHandle)->WriteConnect((IOReg *)pipeHandle, pSendBuf, nBytesToWrite);
#else
  PandaAPI_Printf("*** TODO  %s simulate %s\n", __FILE__, __FUNCTION__);
  return 0;
#endif
}


#ifndef HAVE_MARVELL_SDK

/* Very very very quick implementation of Marvell SDK for linux lib testing */
int main(int argc, char *argv[]) {
  extern int GGSmain(char *);
  extern int gnPDL;
  int i;
  char szCmd[1024]= "ebdwrapper -m 48 -v on -e bypass -r 1 -d 8 -j 3 -o NONE -t 2 -s 9100";
  char *p;
  
//  Do_A_Simple_Test();
  
  if(argc < 2) {
  } else {
    p = &szCmd[0];
    for(i=0;i<argc;i++) {
      p+=sprintf(p, "%s ", argv[i]);
    }
    *--p='\0';
  }
  
  PandaAPI_Printf("Calling GGSmain(\"%s\")\n", szCmd);
  gnPDL = 4; // PCL5c
  GGSmain(szCmd);

  return 0;
}

#endif

