/***********************************************************
* (c) Copyright 2008-2011 Marvell International Ltd. 
 *
 *                         Marvell Confidential
* ==========================================================
*/
 
/**
 * \file print_job_utils.c
 *
 * \brief Utilities for creating general and "special" jobs: calibration jobs,
 *      cleaning page jobs, etc. A properly formed job will consist of calls
 *      to StartJob(), StartPage(), EndPage() and EndJob(), or their variants.
 *
 ******************************************************************************
 */

/*** include files ***/

#include "agRouter.h"

#include "ATypes.h"
#include "logger.h"
#include "print_job_types.h"
#include "agMessage.h"
#include "agprint.h"
#include "agJobUtils.h"
#include "debug.h"
#include "lassert.h"
#include "agResourceMgr.h"
#include "PrintTools.h"
#include "print_mode_api.h"
#include <string.h> // for memset

/*** local definitions ***/

/*** default settings ***/

/*** external functions ***/

/*** external data ***/

/*** internal functions ***/
static void SetPrintConfig_PAGE_DATA(PAGE_DATA* pPage, PAGEINFO* pageInfo);
static void SetPrintConfig_JOB_DATA(JOB_DATA * pJob, PAGEINFO* pageInfo);
static void SetPrintConfig_PLANED(PLANED * pPlane, PAGEINFO* pageInfo, STRIPINFO* stripInfo);

#ifdef HAVE_SDHFAX   
extern PrintJobInfo_t * ///< return null or new object
PrintJobInfo_copy_constructor(PrintJobInfo_t *parent);
extern bool print_page_handle_autoduplex_restricted(PAGE_DATA* page_data, uint32_t current_page_number, bool duplex_rotate_backside);
extern error_type_t image_get_media_size_enum( image_info_t * input_info, mediasize_t * media_size );
extern int getDuplexPrintOn(void);

extern int second_page_on_sheet;
#endif 

/** 
 * \brief Send a start job message to the MODULE_ID given in Dest (usually JobMgr).
 * 
 * \param jobInfo Information on the job
 * 
 * \param pageInfo Information on the pages.
 * 
 * \retval None
 * 
 * 
 **/
void StartJob(JOBINFO* jobInfo, PAGEINFO* pageInfo, MODULE_ID Dest, mlimiter_t *lim)
{
    JOB_DATA *JobHeader;
    MESSAGE     Message;

    JobHeader = (JOB_DATA *) MEM_MALLOC_LIMITED(lim, sizeof(JOB_DATA));
    ASSERT(NULL != JobHeader);
    
    // Initialize the header
    memset(JobHeader, 0, sizeof(JOB_DATA));
    SET_JOB_DATA_COOKIE(JobHeader);

    //   clearConfig(JobHeader);         // clear the options in the job header.
    JobHeader->DEFAULTSOURCE = TRAY_2;
    JobHeader->COPIES = 1;
    JobHeader->PAPER = MEDIASIZE_ANY;
    JobHeader->mediaType = MEDIATYPE_ANY;

    ATInitList(&JobHeader->sheet_list);	//need to check
#ifdef HAVE_SDHFAX
    JobHeader->ChannelID = jobInfo->job_pipe;
#else
	JobHeader->ChannelID = 0;
#endif
    JobHeader->JobMode = e_Normal;      // normal job mode.
   // JobHeader->JobMode = jobInfo->JobMode;
    JobHeader->SendingModule = jobInfo->SendingModule;
    JobHeader->ChannelID = jobInfo->job_pipe;
//    JobHeader->jobDataType = jobInfo->JobData;
    JobHeader->DocType = jobInfo->DocType;
#ifdef HAVE_SDHFAX    
    JobHeader->pjob_info = PrintJobInfo_copy_constructor(jobInfo->job_pipe->pjob_info);
	ASSERT(JobHeader->pjob_info); // constructor must wait for memory.
	
	JobHeader->COLLATE = 0;
	
	if(getDuplexPrintOn())
	{
		JobHeader->DUPLEX = DMDUP_VERTICAL;//DMDUP_VERTICAL;
		JobHeader->duplex_rotate_backside = true;
	}
	else
	{
		JobHeader->DUPLEX = DMDUP_SIMPLEX;
	}
	
#endif
    if(pageInfo)
    {
       SetPrintConfig_JOB_DATA(JobHeader, pageInfo);
    }

    Message.msgType = MSG_JOBSTART;
    Message.param3 = JobHeader;

    SYMsgSend(Dest, &Message);
}


/* FUNCTION NAME: StartPage */
 
/** 
 * \brief Format and send a start page to the print engine.
 * This routine sends a start sheet to the engine and then starts 
 * a send page message.  If we get into duplexers we will have to
 * break this out into a start sheet and start page.
 * 
 * \param pageInfo Info on this page.
 * 
 * \retval None
 * 
 **/
void StartPage(PAGEINFO *pageInfo, MODULE_ID Dest, mlimiter_t *lim)
{
   MESSAGE Message;
   static SHEET_DATA *Sheet = 0;
   PAGE_DATA *Page;

   Page = GetPageHeader(lim);

   SetPrintConfig_PAGE_DATA(Page, pageInfo);
#ifdef HAVE_SDHFAX    
    if( Page->PAPER == MEDIASIZE_ANY )
    {
        //image_get_media_size_enum( &Page->image_output_info, &Page->PAPER );
        Page->PAPER = MEDIASIZE_A4;
    }
    printf("\n(YHCHO) %s() - PAPER = %d\n", __FUNCTION__,Page->PAPER);

	print_page_handle_autoduplex_restricted(Page,!second_page_on_sheet,true);

	if(pageInfo->duplexInfo == 1/*simplex*/ || !second_page_on_sheet)
#endif
	{
		printf("\n(YHCHO) %s() - MSG_JOBSHEETSTART\n", __FUNCTION__);
        Sheet = (SHEET_DATA *) MEM_MALLOC_LIMITED(lim, sizeof(SHEET_DATA));
        ASSERT(NULL != Sheet);
        memset(Sheet, 0, sizeof(SHEET_DATA));

        //Sheet->BackPage = NULL;
        //Sheet->FrontPage = NULL;
        //Sheet->DocType = pageInfo->DocType; 
        Message.msgType = MSG_JOBSHEETSTART;
        Message.param3 = Sheet;
        SYMsgSend(Dest, &Message);
	}

   //Page->PageSent = 0;
   //Page->DataDeleted = 1;

   Page->manual_swaths = pageInfo->ManualSwathList;

   if (NULL != Page->manual_swaths)
   {
       Page->manual_swath_mode_enabled = 1;
   }
#ifdef HAVE_SDHFAX  
   ASSERT(Sheet);
   Page->sheet = Sheet; 
#endif
   print_mode_get( Page );
   Message.msgType = MSG_JOBPAGESTART;
   Message.param3 = Page;
   SYMsgSend(Dest, &Message);
   
   Page = NULL;
#ifdef HAVE_SDHFAX
   if(second_page_on_sheet)
	Sheet = 0;
#endif
}

 

/* FUNCTION NAME: EndPage */
 
/** 
 * \brief Format and send an end page message. Used ONLY for
 *      internal pages, send pixel count values if any for
 *      internally generated pages.  If no pixel count vals send 0s for counts.
 * 
 * \retval None
 * 
 **/
void EndPageEx( MODULE_ID Dest, PIXELCNT *page_pix_counts, mlimiter_t *lim)
{
    MESSAGE Message;

    Message.param1 = Message.param2 = 0;
    Message.param3 = NULL;

    Message.msgType = MSG_JOBPAGEEND;

    SYMsgSend (Dest, &Message);
}

void page_plane_start(MODULE_ID Dest, COLOR plane_color)
{
    MESSAGE msg;

    msg.msgType = MSG_JOBPLANESTART;
    msg.param1 = plane_color;
    msg.param2 = 0;
    msg.param3 = 0;

    SYMsgSend(Dest, &msg);	
}

void page_plane_end(MODULE_ID Dest, COLOR plane_color)
{
    MESSAGE msg;

    msg.msgType = MSG_JOBPLANEEND;
    msg.param1 = plane_color;
    msg.param2 = 0;
    msg.param3 = 0;

    SYMsgSend(Dest, &msg);	
}


/**
 * \brief Format and send an end page message. Used ONLY for
 *      internal pages.  This is the old (obsolete) form
 *      of EndPage.  It always passes pixel counts as NULL.
 *
 * \retval None
 *
 **/
void EndPage( MODULE_ID Dest, mlimiter_t *lim )
{
	page_plane_end(Dest,COLOR_BLACK);
    EndPageEx(Dest, NULL, lim);
}
 
/* FUNCTION NAME: EndJob */
 
/** 
 * \brief Format and send an end job message to the engine.
 * 
 * \retval None
 * 
 **/
void EndJob( MODULE_ID Dest )
{
   MESSAGE Message;
 
   Message.msgType = MSG_JOBEND;
   Message.param3 = NULL;
   SYMsgSend(Dest, &Message);
}

 
 
/* FUNCTION NAME: SendStrip */
 
/** 
 * \brief Format and send data to the ending.
 * 
 * \param pageInfo Information on the page.
 * 
 * \param stripInfo Information on the data and the data.
 * 
 * \retval None
 * 
 **/
void SendStrip(PAGEINFO *pageInfo, STRIPINFO* stripInfo, MODULE_ID Dest, mlimiter_t *lim)
{
   MESSAGE     Message;

   PLANED *plane;

   memset( &Message, 0, sizeof(MESSAGE));

   plane = (PLANED *) MEM_MALLOC_LIMITED(lim, sizeof(PLANED));
   ASSERT(NULL != plane);
   memset(plane, 0, sizeof(PLANED));

   Message.msgType = MSG_JOBPLANEDATA;
   SET_PLANED_COOKIE(plane);

   plane->big_buffer = stripInfo->big_buffer;
   plane->BlockDataType = stripInfo->stripDataType;

   Message.param3 = plane;
   Message.param1 = stripInfo->color;

   SetPrintConfig_PLANED(plane, pageInfo, stripInfo);
   plane->LastData = (stripInfo->lastStrip) ? 1 : 0;

   plane->sysPlaneColor = stripInfo->color;
       
   // for compression
   if (PLANEDATA_COMPTYPE_JPEG == stripInfo->compType)
   {
       plane->DataLength = stripInfo->bufSize;
       plane->compression_info.comp_type = COMPTYPE_JPEG;
   }
   else if (PLANEDATA_COMPTYPE_JBIG == stripInfo->compType)
   {
       plane->DataLength = stripInfo->bufSize;
       plane->compression_info.comp_type = COMPTYPE_JBIG;
   }
   else
   {
       plane->DataLength = (pageInfo->PixelsPerLine * pageInfo->BitsPerPixel) / CHARSIZE;   
       plane->compression_info.comp_type = COMPTYPE_NONE;
   }

   plane->compression_info.comp_details.jbig.L0 =  stripInfo->linesPerStripe;

   SYMsgSend(Dest, &Message);
}

 
 
/* FUNCTION NAME: SetPrintConfig_XXX_DATA */
 
/** 
 * \brief Given a page header, unpack the pageInfo into the page header.
 * 
 * \param pPage The page header to fill
 * 
 * \param pageInfo The page info where to obtain info to fill the page header.
 * 
 * \retval None
 * 
 **/

static void SetPrintConfig_PAGE_DATA(PAGE_DATA* pPage, PAGEINFO* pageInfo)
{
    // Alphabetical order, with a couple "intuitive" exceptions.
    pPage->COPIES           = pageInfo->copies;
    pPage->temp_InputTray   = pageInfo->InputTray;
    pPage->DEFAULTSOURCE    = pageInfo->InputTray;
    pPage->DocType          = pageInfo->DocType;
    pPage->DUPLEX           = pageInfo->duplexInfo;
    pPage->ManDup2ndPg      = pageInfo->ManDup2ndPg;
    pPage->ManualDuplex     = pageInfo->ManualDuplex;
    pPage->mediaType        = pageInfo->PageType;
    pPage->PAPER            = pageInfo->PageSize;
    pPage->PrintQuality     = pageInfo->printQuality;
    pPage->requestedOutTray = pageInfo->requestedOutTray;
    pPage->TONER_SAVE       = pageInfo->TonerSaveMode;
    pPage->COLLATE          = pageInfo->collate;
    pPage->num_active_planes = pageInfo->num_active_planes;
    pPage->video_justification_mode = VIDEO_JUSTIFY_AUTO;

    pPage->nup_image_params.page_orientation = pageInfo->nup_image_params.page_orientation;
    pPage->nup_image_params.num_images = pageInfo->nup_image_params.num_images;
    pPage->nup_image_params.image_width = pageInfo->nup_image_params.image_width;
    pPage->nup_image_params.image_height = pageInfo->nup_image_params.image_height;

    pPage->page_output_color_mode         = pageInfo->monoOrColor;

    // Set the input_info params for the page.
    pPage->image_output_info.image_width_in_pixels = pageInfo->PixelsPerLine;
    pPage->image_output_info.image_height_in_lines = pageInfo->Video_Y;
    pPage->image_output_info.image_bpp = pageInfo->BitsPerPixel;
    pPage->image_output_info.image_data_width_in_bytes = (pageInfo->PixelsPerLine * pageInfo->BitsPerPixel) / 8;
    pPage->image_output_info.image_dpi.x = pageInfo->X_Resolution;
    pPage->image_output_info.image_dpi.y = pageInfo->Y_Resolution;
    
    pPage->fLastPage = 0;
    
//-  pPage->Xd               = pageInfo->PixelsPerLine * pageInfo->BitsPerPixel;
} // end SetPrintConfig_PAGE_DATA()

static void SetPrintConfig_PLANED(PLANED * pPlane, PAGEINFO* pageInfo, STRIPINFO* stripInfo)
{
    // Alphabetical order.
    pPlane->DataLength      = (pageInfo->PixelsPerLine * pageInfo->BitsPerPixel) / CHARSIZE;
    pPlane->DUPLEX          = pageInfo->duplexInfo;

    // Set the image_info params for the plane.
    pPlane->image_info.image_dpi.x = stripInfo->X_Resolution;
    pPlane->image_info.image_dpi.y = stripInfo->Y_Resolution;
    pPlane->image_info.image_width_in_pixels = pageInfo->PixelsPerLine;
    pPlane->image_info.image_height_in_lines = stripInfo->numLines;
    pPlane->image_info.image_bpp = pageInfo->BitsPerPixel;
    pPlane->image_info.image_data_width_in_bytes = (pageInfo->PixelsPerLine * pageInfo->BitsPerPixel) / 8;
    pPlane->image_info.left_to_right = true;
    
    pPlane->compression_info.comp_details.jbig.Xd = pageInfo->PixelsPerLine * pageInfo->BitsPerPixel;
}

static void SetPrintConfig_JOB_DATA(JOB_DATA * pJob, PAGEINFO* pageInfo)
{
    // Alphabetical order.
    pJob->COPIES            = pageInfo->copies;
    pJob->DUPLEX            = pageInfo->duplexInfo;
    pJob->DEFAULTSOURCE     = pageInfo->InputTray;
    pJob->mediaType         = pageInfo->PageType;
    pJob->PAPER             = pageInfo->PageSize;
    pJob->requestedOutTray  = pageInfo->requestedOutTray;
}

