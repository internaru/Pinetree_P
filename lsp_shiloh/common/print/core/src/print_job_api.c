/*
 *
 * =====================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * =====================================================================
 *
 */

/** 
 * \file print_job_api.c
 * 
 * \brief Interface for creating print jobs and sending pages.
 * 
 **/

#include <string.h>  // for memset

#include "print_job_api.h"
#include "print_job_types.h"
#include "PrintJobInfo.h"
#include "lassert.h"
#include "agMessage.h"
#include "memAPI.h"
#include "PrintTools.h"
#include "printvars_api.h"
#include "print_mode_api.h"
#include "engPaperHandlingAPI.h" // have duplexer ?
#include "dma_buffer.h"

#include "print_logger.h"
#include "debug.h"
/** local definitions **/

#define DBG_PRFX "PM(API): "
// TODO: deal with creation of global job ID and ability to access it.

//////////////////////////////////////////////////////////////////////////////
// 
// JOB specific portion of the API
// 
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//
// print_job_construct
// 
//////////////////////////////////////////////////////////////////////////////
print_job_t* print_job_construct(mlimiter_t* limiter, IOReg* job_pipe)
{
    print_job_t* print_job = NULL;
    JOB_DATA* job_data = NULL;

    ASSERT(job_pipe != NULL);
    ASSERT(limiter != NULL);

    /*  Create new job object */
    print_job = (print_job_t*) MEM_MALLOC_LIMITED(limiter, sizeof(print_job_t));
    job_data = (JOB_DATA*) MEM_MALLOC_LIMITED(limiter, sizeof(JOB_DATA));
    memset(print_job, 0, sizeof(print_job_t));

    print_job->job_data = job_data;
    print_job->limiter = limiter;
    print_job->job_dest = INVALID_MODULE_ID;
    print_job->DefaultInputSource = TRAY_ANY;

    memset(job_data, 0, sizeof(JOB_DATA));
    SET_JOB_DATA_COOKIE(job_data);

    ATInitList(&job_data->sheet_list);

    job_data->JobMode = e_Normal;
    job_data->ChannelID = job_pipe;

    // create new job info from pipe's template 
    // observer's register with the pipe's template
    // the print_jobmgr gets a copy, the settings are now job scoped
    // 
    job_data->pjob_info = PrintJobInfo_copy_constructor(job_pipe->pjob_info);
    ASSERT(job_data->pjob_info); // constructor must wait for memory.

    print_job_set_duplex_and_binding( print_job, 
				      PrintJobInfo_int_field_get( job_data->pjob_info, DUPLEX ),
				      PrintJobInfo_int_field_get( job_data->pjob_info, BINDING ) );

    // QTY is collated copies. COPIES is uncollated copies
    print_job->collate = (job_data->COLLATE = PrintJobInfo_int_field_get( job_data->pjob_info, QTY ) );
    DPRINTF((DBG_SOFT|DBG_OUTPUT),("%s: print QTY is %d \n", __func__, job_data->COLLATE) );	// 2013.09.13		by Juny

    if ( print_job->collate )
	print_job->num_copies = job_data->COPIES = job_data->COLLATE;
    else
	print_job->num_copies = job_data->COPIES = PrintJobInfo_int_field_get( job_data->pjob_info, COPIES );
    DBG_ASSERT( job_data->COPIES > 0 );
    // lsptodo: add pjl paper 
    job_data->PAPER = MEDIASIZE_ANY;
    if ( printvar_get_default_papertype( &job_data->mediaType ) != OK )
    {
        job_data->mediaType =  MEDIATYPE_ANY;
    }
    job_data->DEFAULTSOURCE = TRAY_ANY;
    job_data->JobMode = e_Normal;
    job_data->SendingModule = INVALID_MODULE_ID;
    print_job->doc_type = job_data->DocType = e_DOCUMENT;
    print_job->job_id = 0;

    return print_job;
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_start
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_start(print_job_t* p_job, MODULE_ID job_dest)
{
    MESSAGE msg;

    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    p_job->job_started = true;
    p_job->job_dest = job_dest;
    p_job->job_data->oem_data = p_job;

    msg.msgType = MSG_JOBSTART;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = p_job->job_data;

    // job_data is now owned by the job manager.  once job_start occurs job_data can no longer
    // be altered through the API so detach.
    p_job->job_data = NULL;

    SYMsgSend (job_dest, &msg);                     // send the message to the job manager.
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_end
// 
//////////////////////////////////////////////////////////////////////////////
print_job_t* print_job_end(print_job_t* p_job)
{
    MESSAGE msg;
    ASSERT(p_job != NULL);

    // TODO: assert that page is not in progress

    memset(&msg, 0, sizeof(MESSAGE));

    if(p_job->job_started)
    {
        ASSERT(p_job->job_dest != INVALID_MODULE_ID);
        
        DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"[%s] job_id=%d\n", __FUNCTION__, p_job->job_id);

        msg.msgType = MSG_JOBEND;
        msg.param2 = p_job->job_id;
        SYMsgSend(p_job->job_dest, &msg);

    }

    if(p_job->job_data != NULL)
    {
        MEM_FREE_AND_NULL(p_job->job_data);
    }
    MEM_FREE_AND_NULL(p_job);
    return p_job;
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_abort
// 
//////////////////////////////////////////////////////////////////////////////
print_job_t* print_job_abort(print_job_t* p_job)
{
    MESSAGE msg;
    ASSERT(p_job != NULL);

    memset(&msg, 0, sizeof(MESSAGE));

    if(p_job->job_started)
    {
        ASSERT(p_job->job_dest != INVALID_MODULE_ID);

        msg.msgType = MSG_JOBABORT;
        SYMsgSend(p_job->job_dest, &msg);
    }

    if(p_job->job_data != NULL)
    {
        MEM_FREE_AND_NULL(p_job->job_data);
    }
    MEM_FREE_AND_NULL(p_job);
    return p_job;
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_cancel
// 
//////////////////////////////////////////////////////////////////////////////
print_job_t* print_job_cancel(print_job_t* p_job)
{
    if(p_job != NULL)
    {
        if(p_job->job_data != NULL)
        {
            MEM_FREE_AND_NULL(p_job->job_data);
        }
        MEM_FREE_AND_NULL(p_job);
    }
    return p_job;
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_job_mode
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_job_mode(print_job_t* p_job, JOB_MODE job_mode)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    // TODO: find out if e_KillAll is ever used by anything and consider discarding if not...
    // if we keep it maybe rename to print_job_set_page_cancel_mode and rename all associated
    // enums, etc.

    if(p_job->job_data != NULL)
    {
        p_job->job_data->JobMode = job_mode;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_job_data_type
// 
//////////////////////////////////////////////////////////////////////////////

/* obsolete 
void print_job_set_job_data_type(print_job_t* p_job, JOB_DATA_TYPE job_data_type)
{
    // TODO: should be able to combine this and doctype when merged to unity

    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->jobDataType = job_data_type;
    }
} 
*/ 

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_job_doc_type
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_doc_type(print_job_t* p_job, DOCTYPE doc_type)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->DocType = doc_type;
        p_job->doc_type = doc_type;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_source_module
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_source_module(print_job_t* p_job, MODULE_ID src_mod_id)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    // TODO: kill this off when we do away with e_KillAll

    if(p_job->job_data != NULL)
    {
        p_job->job_data->SendingModule = src_mod_id;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_collation
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_collation(print_job_t* p_job, bool collate)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->COLLATE = (uint16_t) collate;
        p_job->collate = collate;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_duplex
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_duplex(print_job_t* p_job, uint16_t duplex)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->DUPLEX = duplex;
        p_job->duplex = duplex;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_duplex_rotate_backside
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_duplex_rotate_backside(print_job_t* p_job, bool rotate_backside)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job != NULL)
    {
        p_job->duplex_rotate_backside = rotate_backside;
        p_job->job_data->duplex_rotate_backside = p_job->duplex_rotate_backside;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_num_copies
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_num_copies(print_job_t* p_job, uint32_t num_copies)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->COPIES = num_copies;
        p_job->num_copies = num_copies;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_request_media_detect
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_request_media_detect(print_job_t* p_job, bool request_media_detect)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    p_job->job_data->request_media_detect = request_media_detect;
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_media_type
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_media_type(print_job_t* p_job, mediatype_t mediatype)
{
    // obsolete? YES.  KILL OFF IN UNITY

    // TODO: if we keep this function we need to persist so that it can
    // be inherited

    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->mediaType = mediatype;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_media_size
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_media_size(print_job_t* p_job, mediasize_t mediasize)
{
    // obsolete?  YES KILL IT

    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    // TODO: if we keep this function we need to persist so that it can
    // be inherited

    if(p_job->job_data != NULL)
    {
        p_job->job_data->PAPER = mediasize;
   } 
}

void print_job_set_copy_tray_setting_info(print_job_t* p_job, mediasize_t tray_0, mediasize_t tray_1, mediasize_t tray_2, mediasize_t tray_3)
{
    // obsolete?  YES KILL IT

    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    // TODO: if we keep this function we need to persist so that it can
    // be inherited

    if(p_job->job_data != NULL)
    {
        p_job->job_data->CopyTraySettingSize[0] = tray_0;
        p_job->job_data->CopyTraySettingSize[1] = tray_1;
        p_job->job_data->CopyTraySettingSize[2] = tray_2;
        p_job->job_data->CopyTraySettingSize[3] = tray_3;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_default_source
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_default_source(print_job_t* p_job, trayenum_t source_tray)
{
    // obsolete?  YES KILL OFF

    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    // TODO: if we keep this function we need to persist so that it can
    // be inherited

    if(p_job->job_data != NULL)
    {
        p_job->job_data->DEFAULTSOURCE = source_tray;
    }

    // save this in the job information so that we can assign it later to each page
    p_job->DefaultInputSource = source_tray;
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_darkness_level
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_darkness_level(print_job_t* p_job, uint32_t darknesslevel)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->ImageOption.darknessLevel = darknesslevel;
   } 
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_toner_save
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_toner_save(print_job_t* p_job, uint32_t tonersave)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->ImageOption.tonerSave = tonersave;
   } 
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_image_resolution
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_image_resolution(print_job_t* p_job, uint32_t imageresolution)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->ImageOption.imageResolution = imageresolution;
   } 
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_parser_info
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_parser_info(print_job_t* p_job, uint8_t parser)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->parser_info = parser;
   } 
}

//////////////////////////////////////////////////////////////////////////////
//
// print_job_set_job_info
// 
//////////////////////////////////////////////////////////////////////////////
void print_job_set_job_info(print_job_t* p_job, JOBMODE mode)
{
    ASSERT(p_job != NULL);
    ASSERT(p_job->job_data != NULL);
    ASSERT(!p_job->job_started);

    if(p_job->job_data != NULL)
    {
        p_job->job_data->JobInfo = mode;
   } 
}


//////////////////////////////////////////////////////////////////////////////
// 
// PAGE specific portion of the API
// 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// print_page_construct
// 
//////////////////////////////////////////////////////////////////////////////
print_page_t* print_page_construct(print_job_t* p_job)
{
    print_page_t* page;

    ASSERT(p_job != NULL);

    page = (print_page_t*) MEM_MALLOC_LIMITED(p_job->limiter, sizeof(print_page_t));
    page->page_data = GetPageHeader(p_job->limiter);
    page->my_job = p_job;

    // increment current page number
    p_job->current_page_number++;

    // set page defaults

    // inherit these from the job
    page->page_data->COPIES         = p_job->num_copies;
    page->page_data->DUPLEX         = (uint16_t)p_job->duplex;
    page->page_data->COLLATE        = (uint16_t)p_job->collate;
    page->page_data->DocType        = p_job->doc_type;

    // set the remaining to default values

    // If the user has specified a default source for the job we will use the same
    // source as the default for the page. The user can change this later if they desire.
    page->page_data->DEFAULTSOURCE    = p_job->DefaultInputSource;

    if ( printvar_get_default_papertype( &page->page_data->mediaType ) != OK )
    {
        page->page_data->mediaType =  MEDIATYPE_ANY;
    }
    page->page_data->PAPER            = MEDIASIZE_ANY;
    page->page_data->requestedOutTray = TRAY_ANY;
    page->page_data->page_output_color_mode    = e_Mono;
    page->page_data->num_input_colors = 1;
    page->page_data->PrintQuality     = PRINTQUALITY_NORMAL;
    page->page_data->video_justification_mode = VIDEO_JUSTIFY_AUTO;

    page->page_data->image_enhancement.brightness = 100;
    page->page_data->image_enhancement.contrast = 100;
    page->page_data->fLastPage = 0;
    page->page_data->force_default_media_size = 0;

    return page;
}

/** 
 * \brief Handles the backside/frontside auto-duplex flag settings.  
 * 
 * \param page - pointer to the page 
 *  
 * \param current_page_number - the current page count
 * 
 * \return bool - true if this is the first page on the sheet; else false.
 *  
 * This is the implementation of the auto-duplex handling code.  It is exported 
 * as an api routine for use by the generators.  Some generators (such as the 
 * n-up generator) need to discard and create new page.  Thus they must handle 
 * resetting the duplex values for the pages they are handling.  To avoid duplication 
 * of the duplex code this api was exported.  However, it should NEVER be used by 
 * parsers or applications.  It should only be used by generators and internally 
 * by the print_job_api itself. 
 *
 **/
bool print_page_handle_autoduplex_restricted(PAGE_DATA* page_data, uint32_t current_page_number, bool duplex_rotate_backside)
{
    if (!EngGetHaveAutoDuplexer())
    {
        // if we don't have autoduplex support always return true to indicate
        // that this is the first page on the sheet.  thus on a printer that
        // doesn't support autoduplex we will always have one page per sheet.
        return true;
    }
    bool is_first_page_on_sheet;
    bool pageOrder_1_2_3_4 = true; // FALSE means 2-1-4-3 page order, the usual.   PARSER ORDER.

    // clear all current duplex settings except for the DMDuplex value.
    // This is done to handle cases where we are re-setting pages given to us
    // by generators.
    page_data->DUPLEX = GetDMDuplex(page_data->DUPLEX);

    if ((GetDMDuplex(page_data->DUPLEX) == DMDUP_VERTICAL)
        || (GetDMDuplex(page_data->DUPLEX) == DMDUP_HORIZONTAL))
    {
        // long vs. short edge binding
        if (GetDMDuplex(page_data->DUPLEX) == DMDUP_HORIZONTAL)
        {
            SetShortEdgeBindingOn(page_data->DUPLEX);
        }

        // enable auto duplex
        SetAutoDuplexOn(page_data->DUPLEX);
        is_first_page_on_sheet = current_page_number & 1;


        // backside rotation
        if (true == duplex_rotate_backside && !is_first_page_on_sheet)
        {
            SetRotation(page_data->DUPLEX, e_Rotate180); // rotate backside.
        }
        else
        {
            SetRotation(page_data->DUPLEX, e_NoRotate);
        }

        // page order from the parser
        if (pageOrder_1_2_3_4) // = front, back, front, back, ... .
        {
            if (!is_first_page_on_sheet)
            {
                SetBackSideOn(page_data->DUPLEX);
            }
        }
        else // Page order 2, 1, 4, 3, ... = back, front, back, front, ... .
        {
            if (is_first_page_on_sheet)
            {
                SetBackSideOn(page_data->DUPLEX);
            }

            SetParserSendsBackSideFirstOn(page_data->DUPLEX);
        }

        // page order to the engine
        if (EngDuplexerFrontSideFirst())
        {
            SetSendFrontSideFirstToEngineOff(page_data->DUPLEX);
        }
        else
        {
            SetSendFrontSideFirstToEngineOn(page_data->DUPLEX);
        }
    } // end if ( DMDUP_VERTICAL || DMDUP_HORIZONTAL )
    else
    {
        // Simplex mode was selected for this page.
        SetAutoDuplexOff(page_data->DUPLEX);
        is_first_page_on_sheet = true;
    }

    return is_first_page_on_sheet;
}

/** 
 * \brief Handles the backside/frontside auto-duplex flag settings.
 * 
 * \param page - pointer to the page
 * 
 * \return bool - true if this is the first page on the sheet; else false.
 * 
 * This is a utility routine to handle the complex auto-duplex bitflag settings.
 * It is not part of the exported API.
 *
 **/
static bool handle_page_autoduplex(print_page_t* page)
{
    return print_page_handle_autoduplex_restricted(page->page_data, page->my_job->current_page_number, page->my_job->duplex_rotate_backside);
}

//////////////////////////////////////////////////////////////////////////////
//
// image_get_media_size_enum
// 
//////////////////////////////////////////////////////////////////////////////
error_type_t image_get_media_size_enum( image_info_t * input_info, mediasize_t * media_size )
{
    float width_in_inches = (float)input_info->image_width_in_pixels/input_info->image_dpi.x;
    float height_in_inches = (float)input_info->image_height_in_lines/input_info->image_dpi.y;

    return paper_get_media_size_enum( width_in_inches, height_in_inches, PAPER_UNITS_INCH, media_size );
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_start
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_start(print_page_t* page)
{
    MESSAGE msg;
    static SHEET_DATA *sheet = 0; // remember sheet from front to back.
    bool is_first_page_on_sheet;

    ASSERT(page != NULL);
    ASSERT(page->my_job != NULL);
    ASSERT(page->page_data != NULL);

    // verify that required page parameters have been set
    ASSERT(page->page_data->image_output_info.image_dpi.x != 0);
    ASSERT(page->page_data->image_output_info.image_dpi.y != 0);
    ASSERT(page->page_data->image_output_info.image_height_in_lines != 0);
    ASSERT(page->page_data->image_output_info.image_bpp != 0);
    ASSERT(page->page_data->image_output_info.image_width_in_pixels != 0);
    ASSERT(page->page_data->image_output_info.image_data_width_in_bytes != 0);

    // some software drivers set num copies incorrectly; guard against this
    ASSERT(page->page_data->COPIES > 0);
    if(page->page_data->COPIES == 0)
    {
        page->page_data->COPIES = 1;
    }
/*
    if(page->page_data->image_output_info.image_width_in_pixels == 0 )
    {
        if(page->page_data->PAPER == MEDIASIZE_ANY)
        {
            page->page_data->image_output_info.image_width_in_pixels = page->page_data->VIDEO_X;
        }
        else
        {
            uint32_t width, height;

            error_type_t result = paper_get_media_size(page->page_data->PAPER, &width, &height);

            if( result == OK)
            {
                width = (width / 2540) * page->page_data->RESOLUTION_X; // media size is currently stores ast mm x 100???
                page->page_data->image_output_info.image_width_in_pixels = width;
            }
        }
    }
*/ 
    if( page->page_data->PAPER == MEDIASIZE_ANY )
    {
        image_get_media_size_enum( &page->page_data->image_output_info, &page->page_data->PAPER );
    }
    // handle the autoduplex settings; this determines whether or not we need
    // to allocate a sheet
    is_first_page_on_sheet = handle_page_autoduplex(page);


    extern sem_t *page_token_semaphore;  

#ifdef HAVE_SCAN_SUPPORT
    extern int scanmech_adf_client_request_pause( void );
    extern int scanmech_adf_client_clear_pause( void );
#else
#define scanmech_adf_client_request_pause()
#define scanmech_adf_client_clear_pause()
#endif

    if(is_first_page_on_sheet)
    {
        sheet = (SHEET_DATA *) MEM_MALLOC_LIMITED(page->my_job->limiter, sizeof(SHEET_DATA));
        memset(sheet, 0, sizeof(SHEET_DATA));
        msg.msgType = MSG_JOBSHEETSTART;
        msg.param1 = 0;
        msg.param2 = 0;
        msg.param3 = sheet;

	//DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"[S_CANCEL] step1. pause!! print_page_start 1 (%d)\n", is_first_page_on_sheet);
	//scanmech_adf_client_request_pause();
	sem_wait( page_token_semaphore );
	//DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"[S_CANCEL] step6. resume!! print_page_start 1 (%d)\n", is_first_page_on_sheet);
	//scanmech_adf_client_clear_pause();

        SYMsgSend (page->my_job->job_dest, &msg);         // get a sheet started in the Job manager.
		DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"print_page_start : Send SheetStart!!!\n");
    } 
    else
    {
    //DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"[S_CANCEL] step1. pause!! print_page_start 2 (%d)\n", is_first_page_on_sheet);
	//scanmech_adf_client_request_pause();
	sem_wait( page_token_semaphore );
	//DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"[S_CANCEL] step6. resume!! print_page_start 1 (%d)\n", is_first_page_on_sheet);
	//scanmech_adf_client_clear_pause();
    }

    ASSERT(sheet);
    page->page_data->sheet = sheet;
    print_mode_get(page->page_data);

    // Send the page to the job manager
    msg.msgType = MSG_JOBPAGESTART;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = page->page_data;

    // the page_data is now owned by the jobmgr.  Detach.
    page->page_data = NULL;

    SYMsgSend (page->my_job->job_dest, &msg);
    DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"print_page_start : Send PageStart!!!\n");
    if (!is_first_page_on_sheet)
       sheet = 0;
}

int print_page_start_would_block()
{
	extern sem_t *page_token_semaphore;
	int cnt;

	sem_getvalue( page_token_semaphore, &cnt );
	return cnt;
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_end
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_end(print_page_t* page, PIXELCNT* pix_counts)
{
    MESSAGE msg;

    ASSERT(page != NULL);
    ASSERT(page->my_job != NULL);

    // when the page_end is sent we detach page_data and send it to
    // jobmgr.  Thus this assert checks to make sure that you are
    // not calling print_page_end without first calling print_page_start.
    ASSERT(page->page_data == NULL);
    
    DBG_PRINTF(LOG_CRIT|DBG_PRINTMGR,"[%s] job_id=%d\n", __FUNCTION__, page->my_job->job_id);

    // Send the page to the job manager
    msg.msgType = MSG_JOBPAGEEND;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = (void *)page->my_job->job_id;
    //msg.param3 = pix_counts;
    SYMsgSend (page->my_job->job_dest, &msg);
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_free
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_free(print_page_t* page)
{
    ASSERT(page != NULL);

    if(page->page_data != NULL)
    {
        // Don't free the print_mode object here, wait until the page has 
        // printed, so it's deleted by the job manager.
        MEM_FREE_AND_NULL(page->page_data);
    }
    page->my_job = NULL;

    MEM_FREE_AND_NULL(page);
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_collation
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_collation(print_page_t* page, bool collate)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->COLLATE = collate;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_duplex
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_duplex(print_page_t* page, uint16_t duplex)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    // REVISIT: does this routine make sense or should we just inherit from job always?


    if(page->page_data != NULL)
    {
        page->page_data->DUPLEX = duplex;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_manual_duplex_second_side
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_manual_duplex_second_side(print_page_t* page)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);
    ASSERT(page->my_job != NULL);

    if(page->page_data != NULL)
    {
        // set indicator that this page is a second side page of
        // manual duplex job
        page->page_data->ManDup2ndPg = 1;

        // if this is the first second side page in this job set
        // the flag to stall to allow the user to load the input tray
        if(!page->my_job->manual_duplex_started)
        {
            page->my_job->manual_duplex_started = true;
            page->page_data->ManualDuplex = 1;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_input_source
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_input_source(print_page_t* page, trayenum_t in_tray)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->DEFAULTSOURCE = in_tray;
        page->page_data->temp_InputTray = in_tray;  	/* @@ tray value req. by LYJ 2013.06.26		by Juny */
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_output_dest
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_output_dest(print_page_t* page, trayenum_t out_tray)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->requestedOutTray = out_tray;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_media_size
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_media_size(print_page_t* page, mediasize_t mediasize)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->PAPER = mediasize;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_media_default_size
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_media_default_size(print_page_t* page, uint8_t value)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->force_default_media_size = value;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_media_type
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_media_type(print_page_t* page, mediatype_t mediatype)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->mediaType = mediatype;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_num_copies
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_num_copies(print_page_t* page, uint32_t num_copies)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    // TODO: this goes away.  always inherit from job in the page structure

    if(page->page_data != NULL)
    {
        page->page_data->COPIES = num_copies;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_color_mode
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_color_mode(print_page_t* page, page_color_mode_t page_color)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->page_output_color_mode = page_color;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_colorspace
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_colorspace(print_page_t* page, input_color_space_t colorspace)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->input_color_space = colorspace;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_num_input_colors
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_num_input_colors(print_page_t* page, uint16_t num_input_colors)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->num_input_colors = num_input_colors;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_print_quality
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_print_quality(print_page_t* page, PRINT_QUALITY print_quality)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->PrintQuality = print_quality;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_dpi_x
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_dpi_x(print_page_t* page, uint32_t dpi_x)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->image_output_info.image_dpi.x = dpi_x;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_dpi_y
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_dpi_y(print_page_t* page, uint32_t dpi_y)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->image_output_info.image_dpi.y = dpi_y;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_raster_x
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_raster_x(print_page_t* page, uint32_t x_bits)
{
    // REVIST: give this function a more meaningful name?
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        ASSERT(x_bits % 8 == 0);
        page->page_data->image_output_info.image_data_width_in_bytes = x_bits / 8;
    }                                                                         
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_raster_x_stride
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_raster_x_stride(print_page_t* page, uint32_t x_stride_bits)
{
    // REVIST OBSOLETE?  RASTER_X now appears to include the stride and stride does
    // not appear to be set by either the mono or color software drivers.  remove
    // if not used by ink.

    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
    //    page->page_data->STRIDE = x_stride_bits;
    }                                                                                 
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_output_bpp
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_output_bpp(print_page_t* page, uint32_t output_bpp)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->image_output_info.image_bpp = output_bpp;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_image_width_in_pixels
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_image_width_in_pixels(print_page_t* page, uint32_t x_pixels)
{
    // REVIST: give this function a more meaningful name?
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->image_output_info.image_width_in_pixels = x_pixels;
    }   
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_video_x
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_video_x(print_page_t* page, uint32_t x_pixels)
{
    // REVIST: give this function a more meaningful name?
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->image_output_info.image_width_in_pixels = x_pixels;
    }   
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_video_y
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_image_height_lines(print_page_t* page, uint32_t num_lines)
{
    // REVIST: this is the same as RASTER_Y.  rename and combine?
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->image_output_info.image_height_in_lines = num_lines;
    }   
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_contrast
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_contrast(print_page_t* page, uint32_t contrast)
{
    // used to adjust contrast
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->image_enhancement.contrast = contrast;
    }   
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_brightness
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_brightness(print_page_t* page, uint32_t brightness)
{
    // used to adjust brightness
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->image_enhancement.brightness = brightness;
    }   
}


//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_app
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_app(print_page_t* page, uint8_t kindofapp)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);	

	if(page->page_data != NULL)
		page->page_data->image_output_info.sourceAPP = kindofapp;
}
	
//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_custom_media_size_x
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_custom_media_size_x(print_page_t* page, uint32_t xdim)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->MEDIA_SIZE_X = xdim;
    }
}


//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_custom_media_size_y
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_custom_media_size_y(print_page_t* page, uint32_t ydim)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->MEDIA_SIZE_Y = ydim;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_custom_media_units
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_custom_media_units(print_page_t* page, paper_size_units_t media_units)
{
   /*
    * TODO: Rename the SIZE_RETURN to something more clear (such as MEDIA_SIZE_UNITS)
    * and improve the enums.
    */

    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->MEDIA_SIZE_UNITS = media_units;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_oneup_rotation_params
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_oneup_rotation_params(print_page_t* page, ROTATE_DIR rotation)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->fRotate_oneup_image = rotation;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_sindoh_rotation_params
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_sindoh_rotation_params(print_page_t* page, ROTATE_DIR rotation, uint32_t h, uint32_t w )
{
	int aligned_h = 0;
	
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);
    
    if ( h % 128 )
		aligned_h = ((h /128)+1) * 128;
		
	DPRINTF((DBG_SOFT|DBG_OUTPUT), ("height(%d) was changed to aligned width(%d)", h, aligned_h));

    if(page->page_data != NULL)
    {
        page->page_data->fRotate_oneup_image = rotation;
        page->page_data->nup_image_params.num_images = 1;
        page->page_data->nup_image_params.image_width = aligned_h;
        page->page_data->nup_image_params.image_height = w;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_nup_params
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_nup_params(print_page_t* page, nup_image_params_t *nup_params)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);
    ASSERT(nup_params != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->nup_image_params = *nup_params;
    }
}

void print_page_get_print_mode(print_page_t *page, print_mode_t **print_mode)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);
    *print_mode = page->page_data->print_mode;
}

void print_page_add_zx_color_size(print_page_t* page, uint32_t color_size)

{
    page->page_data->zx_params.color_size_in_bytes = color_size;
}

void print_page_add_zx_raster_x(print_page_t* page, uint32_t raster_x)

{
    page->page_data->zx_params.width_in_pixels = raster_x;
}
void print_page_add_zx_raster_y(print_page_t* page, uint32_t raster_y)

{
    page->page_data->zx_params.height_in_lines = raster_y;
}
void print_page_add_zx_res_x(print_page_t* page, uint32_t res_x)

{
    page->page_data->zx_params.res.x = res_x;
}
void print_page_add_zx_res_y(print_page_t* page, uint32_t res_y)

{
    page->page_data->zx_params.res.y = res_y;
}

void print_page_add_imaging_bbox_int(print_page_t* page, uint32_t imaging_bbox[4])
{
    memcpy(page->page_data->cups_r_params.imaging_bbox_int,
           imaging_bbox,
           4*sizeof(uint32_t));
}
void print_page_add_imaging_bbox_float(print_page_t* page, float imaging_bbox[4])
{
    memcpy(page->page_data->cups_r_params.imaging_bbox_float,
           imaging_bbox,
           4*sizeof(float));
}
void print_page_add_page_size_int(print_page_t* page, uint32_t page_size[2])
{
    memcpy(page->page_data->cups_r_params.page_size_int,
           page_size,
           2*sizeof(uint32_t));
}
void print_page_add_page_size_float(print_page_t* page, float page_size[2])
{
    memcpy(page->page_data->cups_r_params.page_size_float,
           page_size,
           2*sizeof(float));
}
void print_page_add_color_space(print_page_t* page, uint32_t color_space)
{
    page->page_data->cups_r_params.color_space = color_space;
}
void print_page_add_compression(print_page_t* page, uint32_t compression)
{
    page->page_data->cups_r_params.compression = compression;
}
void print_page_add_marker_type(print_page_t* page, char marker_type[64])
{
    memcpy(page->page_data->cups_r_params.marker_type,
           marker_type,
           64*sizeof(char));
}
void print_page_add_rendering_intent(print_page_t* page, char rendering_intent[64])
{
    memcpy(page->page_data->cups_r_params.rendering_intent,
           rendering_intent,
           64*sizeof(char));
}
void print_page_add_page_size_name(print_page_t* page, char page_size_name[64])
{
    memcpy(page->page_data->cups_r_params.page_size_name,
           page_size_name,
           64*sizeof(char));
}
void print_page_add_anti_bleed_control(print_page_t* page, uint32_t anti_bleed_control)
{
    page->page_data->cups_r_params.anti_bleed_control = anti_bleed_control;
}
void print_page_add_source_image_lightness(print_page_t* page, uint32_t source_image_lightness)
{
    page->page_data->cups_r_params.source_image_lightness = source_image_lightness;
}
void print_page_add_advance_dot_placement(print_page_t* page, uint32_t advance_dot_placement)
{
    page->page_data->cups_r_params.advance_dot_placement = advance_dot_placement;
}
void print_page_add_duplex_dry_time(print_page_t* page, uint32_t duplex_dry_time)
{
    page->page_data->cups_r_params.duplex_dry_time = duplex_dry_time;
}
void print_page_add_top_of_form_detect_disable(print_page_t* page, uint32_t top_of_form_detect_disable)
{
    page->page_data->cups_r_params.top_of_form_detect_disable = top_of_form_detect_disable;
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_borderless
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_borderless(print_page_t* page, uint32_t borderless)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->BORDERLESS = borderless;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// print_page_set_borderless
// 
//////////////////////////////////////////////////////////////////////////////
void print_page_set_print_gray(print_page_t* page, uint32_t gray)
{
    ASSERT(page != NULL);
    ASSERT(page->page_data != NULL);

    if(page->page_data != NULL)
    {
        page->page_data->PRINTGRAY = gray;
    }
}

//////////////////////////////////////////////////////////////////////////////
// 
// PLANE specific portion of the API
// 
//////////////////////////////////////////////////////////////////////////////

void print_plane_start(print_page_t* page, COLOR plane_color)
{
    MESSAGE msg;

    msg.msgType = MSG_JOBPLANESTART;
    msg.param1 = plane_color;
    msg.param2 = 0;
    msg.param3 = 0;

    SYMsgSend(page->my_job->job_dest, &msg);
}

void print_plane_end(print_page_t* page, COLOR plane_color)
{
    MESSAGE msg;

    msg.msgType = MSG_JOBPLANEEND;
    msg.param1 = plane_color;
    msg.param2 = 0;
    msg.param3 = 0;

    SYMsgSend(page->my_job->job_dest, &msg);
}


void print_plane_add_data( print_page_t* page, 
                           COLOR plane_color, 
                           BigBuffer_t* data, 
                           uint32_t data_len_in_bytes, 
                           PLANE_TYPE strip_type,
                           bool last_strip, 
                           uint32_t resolution_x, 
                           uint32_t resolution_y, 
                           uint32_t strip_width_in_pixels, 
                           uint32_t strip_height,
                           uint32_t bits_per_pixel,
                           bool left_to_right,
                           compression_info_t* comp_info)
{
    PLANED* plane;
    MESSAGE msg;

    plane = print_construct_plane( page->my_job->limiter, plane_color, data, data_len_in_bytes,
                                  strip_type, last_strip, resolution_x, resolution_y,
                                  strip_width_in_pixels, strip_height, bits_per_pixel, left_to_right, comp_info );

    msg.msgType = MSG_JOBPLANEDATA;
    msg.param1 = plane_color;
    msg.param2 = 0;
    msg.param3 = plane;

    SYMsgSend(page->my_job->job_dest, &msg);
}

PLANED *print_construct_plane( mlimiter_t *limiter, // print_page_t* page, 
                                    COLOR plane_color, 
                                    BigBuffer_t* data, 
                                    uint32_t data_len_in_bytes, 
                                    PLANE_TYPE strip_type,
                                    bool last_strip, 
                                    uint32_t resolution_x, 
                                    uint32_t resolution_y, 
                                    uint32_t strip_width_in_pixels, 
                                    uint32_t strip_height,
                                    uint32_t bits_per_pixel,
                                    bool left_to_right,
                                    compression_info_t* comp_info)
{
   /*
    * last_strip - MUST set to true if this is the last strip of the plane;
    * else set to false.
    * REVISIT: do we really need print_plane_end since we have this?
    * 
    * resolution_x - x resolution of the data in dpi
    * REVISIT: is this really needed or inherit from page?
    * 
    * 
    * resolution_y - y resolution of the data in dpi
    * REVISIT: is this really needed or inherit from page?
    * 
    * strip_width - full width of the strip in pixels including the alignment padding if any
    * REVISIT: is this really needed or inherit from page?
    * This may be needed for ink.  Need to check when merge to unity.  Discuss w/ Todd & Danny
    */

    PLANED* plane;

    plane = (PLANED*)MEM_MALLOC_LIMITED(limiter, (sizeof(PLANED)));
    memset(plane, 0, sizeof(PLANED));

    SET_PLANED_COOKIE(plane);

    // convert the data buffer to a dma buffer; note that this
    // may already have been done by a parser or generator, but if that
    // is the case, no harm no foul to do it here also.
    plane->big_buffer    = BigBuffer_convert_to_dma_buffer(data);
    plane->DataLength    = data_len_in_bytes;
    plane->sysPlaneColor = plane_color;
    plane->LastData      = last_strip;
    plane->image_info.image_dpi.x = resolution_x;
    plane->image_info.image_dpi.y = resolution_y;
    plane->image_info.image_width_in_pixels = strip_width_in_pixels;
    plane->image_info.image_data_width_in_bytes = (strip_width_in_pixels * bits_per_pixel) / 8;
    plane->image_info.image_height_in_lines = strip_height;
    plane->image_info.image_bpp = bits_per_pixel;
    plane->image_info.left_to_right = left_to_right;

    // If no compression was specified, set it to none.
    if( comp_info == 0 )
    {
        plane->compression_info.comp_type = COMPTYPE_NONE;
    }
    else
    {
         plane->compression_info = *comp_info;
    }

    plane->BlockDataType = strip_type;

    return plane;
}


/**
 * \brief Translate driver INPUTTYPE input tray value to its corresponding internal
 *        trayenum_t value.
 *
 * \param input (INPUTTYPE) Requested input tray/bin value.
 *
 * \return trayenum_t Corresponding internal specifier.
 *
 */
trayenum_t print_translate_input_tray(INPUTTYPE input)
{
    trayenum_t tray;
    printer_setup_t pSetup;
    trayenum_t internalTray = TRAY_ANY;

    //map the incoming tray selection to our internal enum
    printvar_get_printersetup(&pSetup);

    for(tray = TRAY_1; tray < NUM_INPUTS; tray++)
    {
        if(pSetup.inputTrays[tray].inputPresent && pSetup.inputTrays[tray].inputType == input)
        {
            internalTray = tray;
            break;
        }
    }

    DPRINTF((DBG_SOFT|DBG_OUTPUT),("%s: Translated driver tray req %d, to internal tray %d\n", __func__, input, internalTray+1));
    return internalTray;
}

/**
 * \brief Translate driver argOUTPUTTYPE output tray value to its corresponding internal
 *        trayenum_t value.
 *
 * \param output_tray (OUTPUTTYPE) Requested output tray/bin value.
 *
 * \return trayenum_t Corresponding internal specifier.
 *
 */
trayenum_t print_translate_output_tray(OUTPUTTYPE output_tray)
{
    printer_setup_t pSetup;
    trayenum_t retValTRAYENUM = TRAY_ANY;
    trayenum_t tray;

    printvar_get_printersetup(&pSetup);

    for (tray = TRAY_1; tray < NUM_OUTPUTS; tray++)
    {
        if (pSetup.outputTrays[tray].outputPresent && pSetup.outputTrays[tray].outputType == output_tray)
        {
            retValTRAYENUM = tray;
            break;
        }
    }

    DPRINTF((DBG_SOFT|DBG_OUTPUT), ("%s: Translated driver argOUTPUTTYPE=%d to raw trayenum_t=%d\n", __func__, output_tray, retValTRAYENUM));
    return retValTRAYENUM;
}

void print_job_set_duplex_and_binding( print_job_t *print_job, uint32_t duplex, uint32_t binding ) 
{  
    if ( duplex && EngGetHaveAutoDuplexer() ) {
	    // convert to enum DUPLEX_MODE
	    if (!binding) { 
		    print_job->duplex =  DMDUP_VERTICAL;
		    print_job->duplex_rotate_backside = true;
	    } else {
		    print_job->duplex =  DMDUP_HORIZONTAL;
		    print_job->duplex_rotate_backside = false;
	    }
	    print_job->job_data->DUPLEX = print_job->duplex; 
	    print_job->job_data->duplex_rotate_backside = print_job->duplex_rotate_backside;
	    // todo design: duplex rotate is too hands on.

    } else {
	    print_job->job_data->DUPLEX = print_job->duplex = DMDUP_SIMPLEX;  // enum value 1 
    }
}
