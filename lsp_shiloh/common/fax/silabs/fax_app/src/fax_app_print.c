/*
 *
 * ============================================================================
 * Copyright (c) 2008-2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <string.h>
#include <stdio.h>

#include "agRouter.h"
#include "ATypes.h"
#include "lassert.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "agprint.h"
#include "agJobUtils.h"

#include "logger.h"
#include "paper_size_api.h"
#include "utils.h"
#include "sjm_observers_api.h"

#include "faxer_api.h"
#include "fax_tiff_io.h"
#include "fax_app_prv.h"
#include "fax_app_jpeg.h" 
#include "fax_header.h"

#include "lab2rgbi.h"
#include "posix_ostools.h"

//#define _DBG(fmt, ...) dbg_printf(fmt, ## _VA_ARGS_ )
#define _DBG(fmt, ...) dbg_msg("FAXAPP", fmt, ## _VA_ARGS_ )

#define LINES_IN_STRIP      300 ///< use 300 lines per strip for printing.
#define MONO_PRINT_RES      300 
#define JPEG_PRINT_MAX_RES  300 /* can change to 200 for some images */ 
#define STRIP_LINES_FOR_FAX_RX_HDR_200 40  ///< The number of lines for the fax receive header 200 DPI
#define STRIP_LINES_FOR_FAX_RX_HDR_300 60  ///< The number of lines for the fax receive header 300 DPI
#define HEADER_NAME_LEN      ((FAXER_ALPHA_NUMERIC_MAX_STRING_LENGTH * FAXER_UTF8_BYTES_PER_CHAR) + 1)
#define CNV_RESOLUTION FAX_RES_300x300  ///< This is the resolution we want from rdr
#define MAX_NUM_STRIPS      ((300*15)/LINES_IN_STRIP) 
#define COLOR_PRINT_COMPS (3) /* RGB */
#define GRAY_PRINT_COMPS  (1) 

/* Page status...defined here since tiff lib doesn't have return types defined.  */
#define SUCCESS     0
#define PAGE_BREAK  1
#define DOC_END     2
#define PAGE_ERROR  4

/* Errors returned when retrieving data */
#define NP_ERROR      (-1)
#define NP_END_OF_DOC (1)
#define NP_MORE_DATA  (0)

/* internal messages */
#define FAXAPP_MSG_NEXT_PAGE   (MSG_LAST_MSG +1)
#define FAXAPP_MSG_CANCEL_DONE (MSG_LAST_MSG +2)

#define PRINT_TASK_CONTROL_TOKEN 0x456723AB

typedef struct
{
    fax_store_pg_handle_t cpage;
    bool incomplete;
    bool done;
    uint8_t page_status;
    uint32_t pageLineCnt;
    uint32_t output_byte_width;
    uint32_t width;
    uint32_t height;
    bool color;
    fax_header_type_t type;
} active_page_t;

typedef struct
{
    unsigned int token;
    uint16_t scale_page;
    void *reader;
    int reader_pages;
    int max_pixels_per_row;
    int max_lines_per_page;
    int strip_index;
    JOBINFO job_info;
    mediasize_t DefMediaSize;
    CURRENT_RESOURCE *resource;
    unsigned int cur_page_num;
    bool stamp_faxes;
    struct jpeg_decompress_struct jpeg_info;
    mlimiter_t *limiter;
    /* stored here to handle active pages that may have not completed due to 
     lack of memory */
    active_page_t active_page;
    bool canceling;
    int pages_printed; /* used by reader task (print) */
} PRINT_TASK_CONTROL;

/* filled out and passed to print thread to initiate a print job */
typedef struct
{
    bool in_use;
    void *context;
    uint32_t job_id;
    fax_app_subjob_callback_t update_callback;
    void *pjob_handle;
    char *fname;
    fax_store_handle_t store; /* for jpeg */
    PRINT_TASK_CONTROL pCtrl;
    int pages_to_print; /* number of pages sent */
    bool data_complete; /* all pages have been sent, only set by client */
} _print_job_info_t;

/* only one at this time */
static _print_job_info_t _print_info;
static pthread_mutex_t _mutex; /* to protect portions of print_info */

/* print media size setting */
mediasize_t _print_media_size = MEDIASIZE_LETTER;

/* Print a stamp (footer) on faxes (1) or not (0) */
bool _stamp_faxes = 1; // on by default

/*----------------------------------------------------------------------------*/
static void _access(int lock_nLock)
{
  if (lock_nLock) pthread_mutex_lock( &_mutex );
  else            pthread_mutex_unlock( &_mutex );
}

/*----------------------------------------------------------------------------*/
static void _update_status(fax_subjob_state_t state)
{
    fax_subjob_update_t update;
    /* clear it out */
    memset(&update, 0, sizeof(update));
    /* only context id and state are needed */
    update.context = _print_info.context;
    update.state = state;
    if (_print_info.update_callback)
    {
        (*_print_info.update_callback)(&update);
    }
    else
    {
        dbg_printf("FAXAPP: print subjob callback is NULL!!\n");
        ASSERT(0);
    }
}

/*Start a new fax print job
 * 
 * \return 0 Failure
 *         Otherwise Handle to the print job
 * */
void *faxer_app_print_start(CURRENT_RESOURCE *resource, _print_job_info_t *info)
{
    PRINT_TASK_CONTROL *pCtrl = &_print_info.pCtrl;
    uint32_t media_width, media_height;

    if (resource == NULL)
    {
        dbg_printf("FAXPP: print empty resource passed in. Abort!\n");
        return NULL;
    }
    if (info->fname)
    {
        if (!info->fname || strlen(info->fname) < 1)
        {
            dbg_printf("FAXAPP: print empty tiff_name passed in. Abort!\n");
            return NULL;
        }

        if (!fax_tiff_check_file_validity(info->fname))
        {
            dbg_printf("FAXAPP: print invalid TIFF.  Abort!\n");
            return NULL;
        }
    }
    else if (!info->store)
    {
        dbg_printf("FAXAPP: print empty store object.  Abort!\n");
        return NULL;
    }

#if 0  /* small structure, only one job at a time...staticly defined */ 
    pCtrl = MEM_MALLOC(sizeof(PRINT_TASK_CONTROL));
    if(!pCtrl)
    {
        dbg_printf("Fail to allocate memory. Abort!\n");
        return NULL;
    }
#endif

    pCtrl->token = PRINT_TASK_CONTROL_TOKEN;
    pCtrl->resource = resource;
    pCtrl->scale_page = 0;
    pCtrl->DefMediaSize = _print_media_size;

    /* get media height in mm */
    paper_get_media_size(pCtrl->DefMediaSize, &media_width, &media_height);

    /* determine max pix width and height base on media size in mm, align width to 128 
     (I think 16 would work for mono, but we will just stick with 128 for all cases for now) */
    if (info->store)
    {
        pCtrl->max_pixels_per_row = ((media_width * JPEG_PRINT_MAX_RES + 1270) / 2540 + 127) & 0xffffff80;
        pCtrl->max_lines_per_page = (media_height * JPEG_PRINT_MAX_RES) / 2540;
    }
    else
    {
        FAX_PAGE_SIZE page_size;

        pCtrl->max_pixels_per_row = ((media_width * MONO_PRINT_RES + 1270) / 2540 + 127) & 0xffffff80;
        pCtrl->max_lines_per_page = (media_height * MONO_PRINT_RES) / 2540;

        switch (pCtrl->DefMediaSize)
        {
            case MEDIASIZE_LETTER:
                page_size = FAX_PGSIZE_LETTER;
                break;
            case MEDIASIZE_LEGAL:
                page_size = FAX_PGSIZE_LEGAL;
                break;
            case MEDIASIZE_A4:
                page_size = FAX_PGSIZE_A4;
                break;
            default:
                page_size = FAX_PGSIZE_LETTER;
                break;
        }
        pCtrl->reader = fax_tiff_reader_init(_print_info.fname, MONO_PRINT_RES, page_size, 1/* forces fit to page */,
                true/*page breaks*/);
        if (!pCtrl->reader)
        {
            dbg_printf("Fail to obtain tiff reader!\n");
            return (NULL);
        }
    }

    pCtrl->stamp_faxes = _stamp_faxes;

    memset(&(pCtrl->job_info), 0, sizeof(JOBINFO));
    pCtrl->job_info.DocType = e_FAX;
    pCtrl->job_info.JobName = NULL;
    pCtrl->job_info.JobMode = e_Normal;
    pCtrl->job_info.SendingModule = FAXPRINTID;
    pCtrl->job_info.job_pipe = NULL;

    pCtrl->cur_page_num = 1;
    /* get the fax limiter */
    pCtrl->limiter = mlimiter_by_name("fax");
    if (!pCtrl->limiter)
    {
        dbg_printf("FAXAPP: Fail to get memory limiter for fax print resource! Abort!\n");
        ASSERT(0);
        return (NULL);
    }
    mlimiter_start(pCtrl->limiter);

    StartJob(&(pCtrl->job_info), NULL, pCtrl->resource->Destination, pCtrl->limiter);

    return pCtrl;

}

void _setup_page_info(PAGEINFO *page_info, uint32_t pix_width, uint32_t pix_height, bool color, uint16_t resolution)
{
    /* Set page info parameters */
    memset(page_info, 0, sizeof(PAGEINFO));
    if (_print_info.store)
    {
        page_info->BitsPerPixel = color ? (COLOR_PRINT_COMPS * 8) : (GRAY_PRINT_COMPS * 8);
        /* supprted resolutions */
        ASSERT(resolution == 200 || resolution == 300);
        page_info->X_Resolution = resolution;
        page_info->Y_Resolution = resolution;
    }
    else
    {
        page_info->BitsPerPixel = 1;
        page_info->X_Resolution = MONO_PRINT_RES;
        page_info->Y_Resolution = MONO_PRINT_RES;
    }
    page_info->DocType = e_FAX;
    page_info->monoOrColor = color ? e_Color : e_Mono;
    page_info->InputTray = TRAY_ANY;
    page_info->PageSize = _print_info.pCtrl.DefMediaSize;
    page_info->PageType = MEDIATYPE_PLAIN_PAPER;
    page_info->Video_Y = pix_height;
    page_info->PixelsPerLine = pix_width;
    page_info->Video_X = pix_width;
    page_info->RawData = TRUE;
    dbg_printf("\nprint res = %d x %d \n", page_info->X_Resolution, page_info->Y_Resolution);
    page_info->copies = 1;
}

static int _get_jpeg_data(PRINT_TASK_CONTROL *pCtrl, uint8_t *out_buf, uint32_t output_buf_width, bool color,
        uint16_t *lines_in_strip, uint8_t *pg_status)
{
    int ret = 0;
    uint16_t lines_to_read = *lines_in_strip;
    uint8_t *lab_data = NULL;
    bool completed = 0;
    uint32_t jpeg_width, jpeg_height;
    uint16_t jpeg_res;
    uint32_t center_offset = 0;
    uint32_t media_width;
    uint32_t media_width_pix;
    uint32_t media_height;

    dbg_printf("FAXAPP: jpeg print, getting data\n");

    fax_app_jpeg_get_output_dimensions(&pCtrl->jpeg_info, &jpeg_width, &jpeg_height);
    fax_app_jpeg_get_output_resolution(&pCtrl->jpeg_info, &jpeg_res);

    paper_get_media_size(pCtrl->DefMediaSize, &media_width, &media_height);
    media_width_pix = (media_width * jpeg_res) / 2540;

    dbg_printf("JPEG_WIDTH = %d MEDIA_WIDTH = %d\n", jpeg_width, media_width_pix);
    /* place image in center of the page, determine center offset if needed */
    if (jpeg_width < media_width_pix)
    {
        if (color)
            center_offset = (media_width_pix - jpeg_width) / 2 * COLOR_PRINT_COMPS;
        else
            center_offset = (media_width_pix - jpeg_width) / 2 * GRAY_PRINT_COMPS;
    }
    else
    {
        center_offset = 0;
    }
    dbg_printf("Center offset = %d Width = %d JPEG Width = %d\n", center_offset, pCtrl->active_page.width, jpeg_width);

    *pg_status = SUCCESS;
    while (lines_to_read && *pg_status != DOC_END && *pg_status != PAGE_BREAK)
    {
        /* jpeg wrapper returns a pointer to its row buffer */
        ret = fax_app_jpeg_read_data_scanline(&pCtrl->jpeg_info, &lab_data, &completed);
        if (ret == 0 && lab_data)
        {
            uint8_t *write_buf;
            if (color)
            {
                //static const int _map_rgbx[3] = {3,2,1};
                static const int _map_rgb[3] = {2, 1, 0};
                /* center data */
                write_buf = out_buf + center_offset;
                /* convert from lab to rgbx and place into the strip buffer, be sure to convert
                 no more than the jpeg size */
                lab2rgbi(lab_data, write_buf, MIN((jpeg_width*COLOR_PRINT_COMPS), output_buf_width-center_offset)
                        /COLOR_PRINT_COMPS, COLOR_PRINT_COMPS, _map_rgb);
            }
            else /* assumed 1 component grayscale */
            {
                write_buf = out_buf + center_offset;
                memcpy(write_buf, lab_data, MIN(jpeg_width, output_buf_width - center_offset));
            }
            out_buf += output_buf_width;
            lines_to_read--;
            if (completed)
            {
                dbg_printf("FAXAPP: print fax store num pages = %d\n", fax_store_num_pages(_print_info.store));
                if (fax_store_num_pages(_print_info.store) == 1)
                {
                    *pg_status = DOC_END;
                }
                else
                {
                    *pg_status = PAGE_BREAK;
                }
                break;
            }
        }
        else
        {
            dbg_printf("Error reading jpeg file\n");
            *pg_status = PAGE_ERROR;
            ret = -1;
            break;
        }
    }
    *lines_in_strip -= lines_to_read;
    return (ret);
}

static void _send_strip(PRINT_TASK_CONTROL *pCtrl, PAGEINFO *page_info, BigBuffer_t *strip, bool done,
        uint32_t num_lines, bool color)
{
    STRIPINFO strip_info;
    memset(&strip_info, 0, sizeof(strip_info));

    strip_info.big_buffer = strip;
    if ((COLOR_PRINT_COMPS * 8) == page_info->BitsPerPixel)
    {
        strip_info.stripDataType = PACKED_RGB;
    }
    else if ((GRAY_PRINT_COMPS * 8) == page_info->BitsPerPixel)
    {
        strip_info.stripDataType = GRAYSCALE;
    }
    else
    {
        strip_info.stripDataType = PLANAR_K;
    }
    strip_info.compType = PLANEDATA_COMPTYPE_NONE;

    strip_info.color = COLOR_BLACK;

    if (done)
    {
        strip_info.lastStrip = 1;
    }
    strip_info.numLines = num_lines;

    SendStrip(page_info, &strip_info, pCtrl->resource->Destination, pCtrl->limiter);
}

/* returns 0 if ok, 1 if could not allocate memory */
static void _stamp_fax(PRINT_TASK_CONTROL *pCtrl, active_page_t *active_page, uint32_t job_id, PAGEINFO *page_info,
                       BigBuffer_t *strip_buf)
{
    if (pCtrl->stamp_faxes)
    {
        FAXER_ACTIVITY_ENTRY act;
        uint32_t count;
        uint32_t *index_set;
        int bad_fax_log = 0;
        char fax_time[30 + 1];
        void *header;
        uint8_t *header_buf;
        int header_rows;

        memset(&act, 0, sizeof(act));
        memset(&fax_time, 0, sizeof(fax_time));

        count = faxer_find_fax(NULL, FAXER_ACT_TYPE_NULL, 0xffff, 0, 0); // find out the number of logs
        if (count > 0)
        {
            index_set = MEM_MALLOC(sizeof(uint32_t) * count);
            if (job_id == 0)
            {
                // use the last one if color, since there is only one.
                count = faxer_find_fax(NULL, FAXER_ACT_TYPE_NULL, 0xffff, index_set, count);
                bad_fax_log = faxer_get_log(NULL, index_set[(uint32_t) count - 1], &act);
            }
            else
            {
                bad_fax_log = faxer_get_log(NULL, _print_info.job_id, &act); // 0 if OK
            }
            // get fax log info and add to page
            MEM_FREE_AND_NULL(index_set);
        }

        if (bad_fax_log)
        {
            // log not found or error (color=no file, etc.)
            header = fax_header_build(active_page->output_byte_width, page_info->X_Resolution, page_info->Y_Resolution,
                    active_page->type, (pCtrl->pages_printed + 1), NULL, 
                    NULL, NULL, FAX_HEADER_FORMAT_DATE_AND_TIME);
        }
        else
        {
            // OK, so use time from log
            header = fax_header_build(active_page->output_byte_width, page_info->X_Resolution, page_info->Y_Resolution,
                    active_page->type, (pCtrl->pages_printed + 1), act.remoteName, act.remoteId,
                    &act.time, FAX_HEADER_FORMAT_DATE_AND_TIME);
        }
        fax_header_get_data(header, &header_buf, &header_rows);
        memcpy(strip_buf->data, (char *) header_buf, active_page->output_byte_width * header_rows);
        _send_strip(pCtrl, page_info, strip_buf, active_page->done, header_rows, _print_info.store/*active_page->color*/);
        fax_header_destroy(header);
        active_page->pageLineCnt += header_rows;
    }
}


/*Move on to print next page 
 * 
 * \return 0   Success
 *         1   Doc end reached
 *         < 0 Error 
 * */
int faxer_app_print_next_page(void *handle)
{
    uint32_t page_height;

    BigBuffer_t *strip_buf[MAX_NUM_STRIPS];
    int ret;
    int strip_index = 0;
    uint16_t lines_in_strip = LINES_IN_STRIP;
    PRINT_TASK_CONTROL *pCtrl = (PRINT_TASK_CONTROL *) handle;

    uint32_t media_height; // size of the paper
    uint32_t media_width;

    PAGEINFO page_info;
    uint32_t src_stride;
    active_page_t *active_page;
    bool data_complete;
    int pages_to_print;
    bool page_done = 0;
    uint32_t job_id;
    uint16_t resolution;

    if (pCtrl->token != PRINT_TASK_CONTROL_TOKEN)
    {
        dbg_printf("Invalid task control!\n");
        return -1;
    }

    /* protect access to _print_info while we determine the state of the pages BEFORE
     we do anything with them...this will be our known state until we return later
     if needed */
    _access(1);
    /* store these varaibles */
    data_complete = _print_info.data_complete;
    pages_to_print = _print_info.pages_to_print;
    job_id = _print_info.job_id;
    _access(0);
    dbg_printf("FAXAPP: next_page() print data complete = %d pages to print = %d pages_printed = %d\n", data_complete,
            pages_to_print, pCtrl->pages_printed);

    /* active page stores all info in case we have to come back due to lack of memory */
    active_page = &pCtrl->active_page;
    active_page->done = 0;

    /* setup for the page if we don't already have an incomplete page  */
    if (!active_page->incomplete)
    {
        bool printed_all_known = 0;

        /* if data is complete, just ignore page counts and continue as normal */
        if (!data_complete)
        {
            /* first check if there are more pages to print */
            if (_print_info.store)
            {
                if (pages_to_print == pCtrl->pages_printed)
                    printed_all_known = 1;
            }
            else
            {
                /* image converter requires us to keep one complete page between that being
                 read and that being written */
                if (pages_to_print <= (pCtrl->pages_printed + 1))
                    printed_all_known = 1;
            }

            if (printed_all_known)
            {
                /* delay, then return...this gives chance for more pages to be added */
                posix_sleep_ticks(200);
                return (NP_MORE_DATA);
            }
        }
        else if (pages_to_print != 0 && pages_to_print == pCtrl->pages_printed)
        {
            /* we are done, we must have hit a small timing hole that caused us to return here...just
             return that we are done */
            dbg_printf("FAXAPP: TIMING EXIT pages_to_print:%d pages_printed:%d data_complete:%d \n",
                    pages_to_print, pCtrl->pages_printed, data_complete);
            return (NP_END_OF_DOC);
        }

        if (_print_info.store)
        {
            int comps;
            uint32_t jpeg_width, jpeg_height;
            active_page->cpage = fax_store_page_open_first(_print_info.store, FAX_STORE_MODE_READ);
            if (!active_page->cpage)
            {
                dbg_printf("FAXAPP: print could not open jpeg page from store\n");
                memset(active_page, 0, sizeof(active_page));
                return (NP_ERROR);
            }
            /* fix file if needed */
            fax_store_make_jpeg_readable_by_ijg(active_page->cpage);
            /* start jpeg decompress for this page */
            if (fax_app_jpeg_start_decompress(&pCtrl->jpeg_info, active_page->cpage, &src_stride, pCtrl->DefMediaSize))
            {
                fax_store_page_destroy(active_page->cpage);
                dbg_printf("FAXAPP: print could not open jpeg page from store\n");
                memset(active_page, 0, sizeof(active_page));
                return (NP_ERROR);
            }
            /* get jpeg info to determine the output width */
            fax_app_jpeg_get_output_dimensions(&pCtrl->jpeg_info, &jpeg_width, &jpeg_height);
            fax_app_jpeg_get_output_resolution(&pCtrl->jpeg_info, &resolution);
            dbg_printf("FAXAPP: jpeg size, width = %d height = %d\n", jpeg_width, jpeg_height);
            /* ensure width is 128 byte aligned, if not the image pipe will do additional padding and
             we have seen issues with this logic in print */
            /* we will center the image in the page */
            active_page->width = ((pCtrl->max_pixels_per_row * resolution) / JPEG_PRINT_MAX_RES + 127) & 0xffffff80;
            paper_get_media_size(pCtrl->DefMediaSize, &media_width, &media_height);
            pCtrl->max_lines_per_page = (media_height * resolution) / 2540;
            dbg_printf("FAXAPP: pCtrl->max_lines_per_page:%d resolution:%d\n", pCtrl->max_lines_per_page, resolution);
            active_page->height = pCtrl->max_lines_per_page;
            // the print line width in byte.
            fax_app_jpeg_get_output_components(&pCtrl->jpeg_info, &comps);
            if (comps == 1)
            {
                active_page->output_byte_width = active_page->width * GRAY_PRINT_COMPS;
                active_page->color = 0;
                active_page->type = FAX_HEADER_TYPE_GRAYSCALE;
            }
            else
            {
                active_page->output_byte_width = active_page->width * COLOR_PRINT_COMPS;
                active_page->color = 1;
                active_page->type = FAX_HEADER_TYPE_RGB;
            }
        }
        else
        {
            resolution = MONO_PRINT_RES;
            active_page->color = 0;
            active_page->type = FAX_HEADER_TYPE_MONO;
            /* pix width and height is just the max (media size) */
            active_page->width = pCtrl->max_pixels_per_row;
            active_page->height = pCtrl->max_lines_per_page;
            dbg_printf("FAXAPP: pCtrl->max_lines_per_page:%d resolution:%d\n", pCtrl->max_lines_per_page, resolution);
            // the print line width in byte.
            active_page->output_byte_width = active_page->width / 8;
        }
        /* set page info structure */
        _setup_page_info(&page_info, active_page->width, active_page->height, active_page->color, resolution);

        StartPage(&page_info, pCtrl->resource->Destination, pCtrl->limiter);
    }

    page_height = active_page->height; // height of printable area
    /* submit strips for print. */
    while (!active_page->done)
    {
        strip_buf[strip_index] = BigBuffer_Malloc(pCtrl->limiter, active_page->output_byte_width * LINES_IN_STRIP);
        if (strip_buf[strip_index] == NULL)
        {
            dbg_printf("FAXAPP: Fail to allocate strip buffer memory.  We will return soon to try again!\n");
            active_page->incomplete = 1;
            posix_sleep_ticks(200);
            return (NP_MORE_DATA);
        }
        else
        {
            lines_in_strip = LINES_IN_STRIP;
            ret = SUCCESS;

            if (active_page->pageLineCnt + lines_in_strip >= page_height)
            {
                lines_in_strip = page_height - active_page->pageLineCnt + 15;
            }

            if (_print_info.store)
            {
                // for color or gray scale
                memset(strip_buf[strip_index]->data, 0xff, active_page->output_byte_width * LINES_IN_STRIP);
                if ((active_page->page_status != PAGE_BREAK) && (active_page->page_status != DOC_END))
                {
                    dbg_printf("FAXAPP: READ color / jpeg data\n");
                    ret = _get_jpeg_data(pCtrl, (uint8_t*) strip_buf[strip_index]->data,
                            active_page->output_byte_width, active_page->color, &lines_in_strip,
                            &active_page->page_status);
                }
            }
            else
            {
                // for BW
                memset(strip_buf[strip_index]->data, 0, active_page->output_byte_width * LINES_IN_STRIP);
                if ((active_page->page_status != PAGE_BREAK) && (active_page->page_status != DOC_END))
                {
                    ret = fax_tiff_reader_read(pCtrl->reader, (unsigned char *) (strip_buf[strip_index]->data),
                            active_page->output_byte_width, &lines_in_strip, &active_page->page_status);
                }
            }
            if (ret != 0)
            {
                if (strip_buf[strip_index])
                {
                    BigBuffer_Free(strip_buf[strip_index]);
                }
                return (NP_ERROR);
            }
            active_page->pageLineCnt += lines_in_strip;

            page_height = active_page->height; // height of printable area
            if (pCtrl->stamp_faxes)
            {
                if (resolution == 200)
                    page_height -= STRIP_LINES_FOR_FAX_RX_HDR_200; // reduce printable area to leave space for stamp
                else
                    page_height -= STRIP_LINES_FOR_FAX_RX_HDR_300; // reduce printable area to leave space for stamp
            }
            if (strip_index >= MAX_NUM_STRIPS)
            {
                dbg_printf("FAXAPP Ran out of strip storage: strip_index:%d(MAX:%d)\n", strip_index, MAX_NUM_STRIPS);
            }
            active_page->done = (active_page->pageLineCnt >= page_height) || (strip_index >= MAX_NUM_STRIPS);

            if (active_page->pageLineCnt >= page_height)
            {
                lines_in_strip -= (active_page->pageLineCnt - page_height);
            }

            if (!pCtrl->stamp_faxes)
            {
                page_done = active_page->done;
            }
            // The memory will be released by the print service. Remove our reference.
            _send_strip(pCtrl, &page_info, strip_buf[strip_index], page_done, lines_in_strip, _print_info.store/*active_page->color*/);

            strip_index++;
        }
    } // DONE with page

   if(pCtrl->stamp_faxes)
   {
     BigBuffer_t *stamp_strip_buf;
     stamp_strip_buf = BigBuffer_Malloc(pCtrl->limiter, active_page->output_byte_width * LINES_IN_STRIP);
     if (strip_buf == NULL)
     {
        dbg_printf("FAXAPP: Fail to allocate strip buffer memory.  We will return soon to try again!\n");
        active_page->incomplete = 1;
        posix_sleep_ticks(200);
        return (NP_MORE_DATA);
     }
     else
     {
       memset(stamp_strip_buf->data, 0, active_page->output_byte_width * LINES_IN_STRIP);
       _stamp_fax(pCtrl, active_page, job_id, &page_info, stamp_strip_buf);
     }
   }

    /* we may need to throw away data if tiff */
    if (!_print_info.store && active_page->page_status != DOC_END && active_page->page_status != PAGE_BREAK)
    {
        unsigned char* buf = MEM_MALLOC(active_page->output_byte_width);
        uint16_t lines_in_buf = 1;
        uint16_t trash_count = 0;

        /* if we can't allocate mem, just use the last strip row */
        if (!buf)
            buf = (unsigned char *) strip_buf[strip_index]->data;

        /* throw away any remaining data...REVISIT...is it ok to throw away data from a fax? */
        while (active_page->page_status != DOC_END && active_page->page_status != PAGE_BREAK)
        {
            fax_tiff_reader_read(pCtrl->reader, buf, active_page->output_byte_width, &lines_in_buf,
                    &active_page->page_status);
            trash_count++;
        }
        dbg_printf("FAXAPP: print threw away %d rows of received image due to overrun\n", trash_count);
        MEM_FREE_AND_NULL(buf);
    }
    dbg_printf("FAXAPP: print End of page, active_page->page_status=%d active_page->pageLineCnt=%d\n",
            active_page->page_status, active_page->pageLineCnt);

    EndPage(pCtrl->resource->Destination, pCtrl->limiter);

    if (active_page->cpage)
    {
        error_type_t err;
        fax_app_jpeg_end_decompress(&pCtrl->jpeg_info);
        err = fax_store_page_destroy(active_page->cpage);
        ASSERT(err == OK);
    }

    pCtrl->pages_printed++;
    if (active_page->page_status == DOC_END && data_complete)
    {
        memset(active_page, 0, sizeof(pCtrl->active_page));
        return (NP_END_OF_DOC); /* end of doc */
    }
    else
    {
        memset(active_page, 0, sizeof(pCtrl->active_page));
        return (NP_MORE_DATA);
    }
}

/**
 * Complete a print job
 * 
 * \return 0 Success
 *         Otherwise error
 */
int faxer_app_print_end(void **handle)
{
    PRINT_TASK_CONTROL *pCtrl = NULL;

    if ((*handle) != NULL)
    {
        pCtrl = (PRINT_TASK_CONTROL *) (*handle);

        if (pCtrl->token != PRINT_TASK_CONTROL_TOKEN)
        {
            dbg_printf("Invalid task control!\n");
            return -1;
        }
        if (pCtrl->reader)
        {
            if (fax_tiff_reader_close(pCtrl->reader))
            {
                dbg_printf("FAXAPP: print failed to close tiff reader\n");
            }
            pCtrl->reader = 0;
        }

        if (pCtrl->limiter)
            mlimiter_stop(pCtrl->limiter);
        EndJob(pCtrl->resource->Destination);
    }
    return 0;
}

static void _job_update(Observer_t *observer, Subject_t *s)
{
    ASSERT( VALIDATE_OBJECT_KEY( observer,
                    Observer_t,
                    subject_observer_oid_key,
                    SJM_JobStatus_ID ) );
    dbg_printf("\n!!!FAX JOB UPDATE, job type = %d job status = %d\n", SJM_Get_JobType(s), SJM_Get_JobStatus(s));
    /* this takes place on the caller's (notifier's) thread, so be careful not to take
     too many cycles */
    if ((SJM_Get_JobType(s) == ejob_FaxPrint) && (SJM_Get_JobStatus(s) == esjm_JobDone))
    {
        if (_print_info.pCtrl.canceling)
            _update_status(FAX_SUBJOB_CANCELED);
        else
            _update_status(FAX_SUBJOB_COMPLETE);
        memset(&_print_info, 0, sizeof(_print_info));
        SJM_DetachObserver_JobStatus(observer);
    }
}

/*
 *  Agent thread function for print
 * 
 */
void * fax_app_agent_print(void* Input)
{
    MESSAGE job_msg;
    MESSAGE msg;
    CURRENT_RESOURCE *OurResource = NULL;
    mqd_t *pQueue = (mqd_t *) Input;
    Observer_t *job_observer;
    int res;

    job_observer = Observer_Constructor(SJM_JobStatus_ID, _job_update);
    posix_mutex_init(&_mutex);

    while (1)
    {
        if(0 != posix_wait_for_message(*pQueue, (char*)&msg,  sizeof(MESSAGE), 
                                       POSIX_WAIT_FOREVER)) 
            continue;

        switch (msg.msgType)
        {
            case FAXAPP_MSG_NEXT_PAGE:
                if (!_print_info.pCtrl.canceling)
                {
                    if (_print_info.pjob_handle)
                        res = faxer_app_print_next_page(_print_info.pjob_handle);
                    else
                        res = NP_ERROR;

                    /* success, but more pages remain */
                    if (res == NP_MORE_DATA)
                    {
                        msg.msgType = FAXAPP_MSG_NEXT_PAGE;
                        msg.param1 = 0;
                        msg.param2 = 0;
                        msg.param3 = NULL;
                        SYMsgSend(FAXPRINTID, &msg);
                    }
                    /* done with pages */
                    else if (res == NP_END_OF_DOC)
                    {
                        /* register for job status observer so we know when the job actualy completes...
                         print is a subjob of our job */
                        SJM_AttachObserver_JobStatus(job_observer);
                        END_JOB(OurResource, FAXPRINTID);
                        faxer_app_print_end(&_print_info.pjob_handle);
                    }
                    else if (!_print_info.pCtrl.canceling)
                    {
                        dbg_printf("Print next page failed, canceling job.\n");
                        _update_status(FAX_SUBJOB_CANCELING);
                        fax_app_print_cancel(_print_info.context);
                    }
                }
                break;
            case FAXAPP_MSG_CANCEL_DONE:
                /* this event allows us to finish up all events that came between cancel and actualy canceling the job */
                ACK_CANCEL(msg.param3, FAXPRINTID);
                break;
            case MSG_RESOURCES:
                OurResource = (CURRENT_RESOURCE *) msg.param3;
                break;

            case MSG_ACKRECIPE:
                _update_status(FAX_SUBJOB_ACTIVE);

                _print_info.pjob_handle = faxer_app_print_start(OurResource, &_print_info);

                if (!_print_info.pjob_handle)
                {
                    dbg_printf("Fail to start fax print job");
                    _update_status(FAX_SUBJOB_CANCELING);
                    fax_app_print_cancel(_print_info.context);
                    break;
                }

                /* send a message to print each page...this is so cancel and other messages can
                 get serviced as needed */
                msg.msgType = FAXAPP_MSG_NEXT_PAGE;
                msg.param1 = 0;
                msg.param2 = 0;
                msg.param3 = NULL;
                SYMsgSend(FAXPRINTID, &msg);
                break;

            case MSG_NAKRECIPE:
                _update_status(FAX_SUBJOB_CANCELED);
                dbg_printf("Got NAKRECEIPE! Abort printing!");
                break;

            case MSG_CANCELJOB:
                dbg_printf("FAX PRINT CANCEL!");
                /* register for job status observer */
                SJM_AttachObserver_JobStatus(job_observer);
                END_JOB(OurResource, FAXPRINTID);
                faxer_app_print_end(&_print_info.pjob_handle);
                _print_info.pCtrl.canceling = 1;
                msg.msgType = FAXAPP_MSG_CANCEL_DONE;
                msg.param1 = 0;
                msg.param2 = 0;
                msg.param3 = msg.param3; // pass back the resource
                SYMsgSend(FAXPRINTID, &msg);
                break;

            default:
                dbg_printf("FAXAPP: print got unknown msg! type = [%d]\n", msg.msgType);
                break;
        }
    }
    return 0;
}

/*---------------------------------------------------------*/
int fax_app_print_notify_page_added(void)
{
    _access(1);
    /* page has been added, is the document complete or not? */
    _print_info.pages_to_print++;
    _access(0);
    return (0);
}

/*---------------------------------------------------------*/
int fax_app_print_notify_document_completed(void)
{
    _access(1);
    _print_info.data_complete = 1;
    _access(0);
    return (0);
}

/*---------------------------------------------------------*/
int fax_app_print_start(char *fname, void *context, uint32_t job_id, fax_app_subjob_callback_t callback, int pages,
        bool complete)
{
    dbg_printf("FAXAPP: start mono print called\n");
    //Ignore if in printing
    if (_print_info.in_use)
    {
        dbg_printf("FAXAPP: start mono print failed, job active!\n");
        return (-1);
    }
    else
    {
        _print_info.in_use = 1;
    }

    if (1/*fax_tiff_check_file_validity(fname)*/)
    {
        STARTRECIPE *StartIt;
        MESSAGE job_msg;
        _print_info.fname = fname;
        _print_info.context = context;
        _print_info.job_id = job_id;
        _print_info.update_callback = callback;
        _print_info.pjob_handle = NULL;
        _print_info.store = 0;
        _print_info.data_complete = complete;
        _print_info.pages_to_print = pages;
        START_JOB(ejob_FaxPrint, FAXPRINTID, e_FaxPrintApp);
    }
    else
    {
        dbg_printf("FAXAPP: tiff file is unprintable (failed validity check)! Mark it printed.\n");
        return (-2);
    }
    return (0);
}

/*---------------------------------------------------------*/

int fax_app_print_start_jpeg(fax_store_handle_t store, void *context, fax_app_subjob_callback_t callback, int pages,
        bool complete)
{
    dbg_printf("FAXAPP: start jpeg print\n");
    //Ignore if in printing
    if (_print_info.in_use)
    {
        dbg_printf("FAXAPP: start jpeg print failed, job active!\n");
        return (-1);
    }
    else
    {
        _print_info.in_use = 1;
    }

    if (!store)
    {
        dbg_printf("FAXAPP: start jpeg print failed, no store!\n");
        return (-2);
    }

    STARTRECIPE *StartIt;
    MESSAGE job_msg;
    _print_info.fname = NULL;
    _print_info.context = context;
    _print_info.update_callback = callback;
    _print_info.pjob_handle = NULL;
    _print_info.store = store;
    _print_info.data_complete = complete;
    _print_info.pages_to_print = pages;
    START_JOB(ejob_FaxPrint, FAXPRINTID, e_FaxPrintApp);
    return (0);
}

int fax_app_print_cancel(void *context)
{
    MESSAGE msg;
    memset(&msg, 0, sizeof(msg));
    msg.msgType = MSG_CANCELJOB;
    msg.param1 = SYS_REQUEST;
    msg.param2 = 0;
    msg.param3 = (void*) e_FaxPrintApp;

    dbg_printf("FAXAPP: Canceling fax print job\n");
    SYMsgSend(SJMID, &msg);
    return (0);
}

fax_app_res_t fax_app_print_set_media_size(mediasize_t media_size)
{
    fax_app_res_t res = FAX_APP_RES_SUCCESS;
    if ((media_size == MEDIASIZE_LETTER) || (media_size == MEDIASIZE_LEGAL) || (media_size == MEDIASIZE_A4))
    {
        _print_media_size = media_size;
    }
    else
    {
        res = FAX_APP_RES_ERROR;
        ASSERT(0);
    }
    return (res);
}

fax_app_res_t fax_app_print_set_stamp_faxes(bool on)
{
    _stamp_faxes = on;

    return (FAX_APP_RES_SUCCESS);
}

bool fax_app_print_get_stamp_faxes(void)
{
    return (_stamp_faxes);
}

