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
#include "SDHFaxCommonHeader.h"
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
#include "dma_buffer.h"
#include "faxapp_param.h"
#include "faxapp_print.h"
#include "posix_ostools.h"
#include "faxapp_job.h"
#include "agConnectMgr.h"
#include "faxapp_notify.h"
#include "print_job_api.h"
#include "printvars_api.h"
#ifdef HAVE_PRINT_SUPPORT
#include "PrintJobInfo.h"
#endif
//
// Static Function Declaration
//
static unsigned char FaxAppPrintStack[FAXAPP_PRINT_STACK_SIZE] ALIGN8;
static mqd_t faxapp_print_msgq;
static pthread_t faxapp_print_thd_id;
static void *faxapp_print_mgr(void *unused);


//
// Static Variables
//
/* only one at this time */
static _print_job_info_t _print_info;
static print_job_t *fax_print_job;
static PAGEINFO fax_print_pageinfo;
static print_page_t* fax_print_page;
static uint8_t fax_old_jam_recovery;
//static pthread_mutex_t _mutex; /* to protect portions of print_info */

static int print_width = 0;
static int print_height = 0;
uint8_t *faxapp_print_buffer = (uint8_t *)NULL;

int second_page_on_sheet = 0;
int duplex_print_page_count = 0;
int user_input_print_cancel = 0;

mediasize_t paper_enum = 0;
extern int input_tray;
extern int need_image_rotation;
trayenum_t  _print_input_tray = TRAY_2;

typedef enum {
    AUTO_REDUCTION,
    NO_REDUCTION_1PAGE,
    NO_REDUCTION_2PAGE
} PRINT_PAGE_STATE;

typedef struct {
    uint32_t width;
    uint32_t height;
} ENGMEDIASIZES;

//
// Global Variables
//
/* print media size setting */
mediasize_t _print_media_size = MEDIASIZE_A4;//MEDIASIZE_ANY;//MEDIASIZE_LETTER;//MEDIASIZE_A4;

/* Print a stamp (footer) on faxes (1) or not (0) */
bool _stamp_faxes = 1; // on by default
extern int DiscardLength;
static int Auto_Reduction_state = AUTO_REDUCTION;
static int faxapp_print_is_on = 0;


//
// External Variables
//
extern int gFaxPrintCurrentPage;
extern int change_width;
extern int change_height;
extern uint32_t paper_width_pixel;
extern uint32_t paper_height_pixel;
extern int paper_pixel_per_mm;

extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern UINT16 fsu_job_num_prn;

extern void abort_mem_to_prn(void);
extern void fsu_update_rx_docman(int result_prn_job, int prn_page);
extern int fsu_print_page_setting(void);
extern int fsu_print_image_setting(void);
extern int fsu_total_page_print_complete_check(void);
extern void fsu_fax_print_job_done(void);
extern int fsu_get_rcv_done (void);
extern int getAutoReductionOn(void);
extern int getDuplexPrintOn(void);
extern int getgFaxPrintMode(void);
extern int fsu_get_memory_full_status_ram (void);
extern UINT8 fsu_get_isDuplex (UINT16 jnum);

extern PrintJobInfo_t * ///< return null or new object
PrintJobInfo_construct(IOReg *pipe);



/**
 * Function name: FaxPrintJobMgrInit
 *
 * \brief Initialize the fax print job manager
 *
 * Initialize data structures, create the queue, register the mainbox and start the task for the fax print job manager
 **/
void FaxPrintJobMgrInit(void)
{
    int retcode = 0;

    // Create the queue
    posix_create_message_queue(&faxapp_print_msgq, "/FaxAppPrintQueue", FAXPRINTTASK_QUEUE_SIZE, sizeof(MESSAGE));

    // Register a message queue.
    router_register_queue(FAXPRINTID, faxapp_print_msgq);

    // Register the resource
    rm_register(e_FaxPrintApp, FAXPRINTID);

    // start the fax app job mgr task
    retcode = posix_create_thread(&faxapp_print_thd_id, faxapp_print_mgr, 0, "FaxAppPrintMgr", FaxAppPrintStack, FAXAPP_PRINT_STACK_SIZE, POSIX_THR_PRI_NORMAL);

    XASSERT( retcode==0, retcode );
}

bool FaxPrintIsLongPage()
{
    if(print_height - (DiscardLength * paper_pixel_per_mm) > paper_height_pixel)
    {
        if(getAutoReductionOn() == DISABLE)
        {
            //Auto_Reduction_state = NO_REDUCTION_1PAGE;
            printf("%s(): long page\n", __FUNCTION__);
            return true;
        }
    }
    printf("%s(): short/autoReduction page\n", __FUNCTION__);
    return false;
}

void FaxPrintJobResult(int result_fax_print, int printed_pages)
{
    printf("FaxPrintJobResult: %d, %d\n", result_fax_print, printed_pages);

    fsu_update_rx_docman(result_fax_print, printed_pages);
}

void faxapp_page_end( void )
{
   MESSAGE Message;

   Message.msgType = FAXAPP_MSG_PAGE_END;
   Message.param3 = NULL;
   SYMsgSend(FAXPRINTID, &Message);
}

void faxapp_next_page( void )
{
   MESSAGE Message;

   Message.msgType = FAXAPP_MSG_NEXT_PAGE;
   Message.param3 = NULL;
   SYMsgSend(FAXPRINTID, &Message);
}

static void
faxapp_print_page_info( const PAGEINFO *p )
{
    printf(  "%s pagetype=%d pagesize=%d bpp=%ld ppl=%ld xres=%ld yres=%ld vidx=%ld vidy=%ld duplex=%#x collate=%d\n",
           __FUNCTION__, p->PageType, p->PageSize, p->BitsPerPixel,
           p->PixelsPerLine, p->X_Resolution, p->Y_Resolution, p->Video_X,
           p->Video_Y, p->duplexInfo, p->collate
         );
}

/**
 *
 * \brief send a STARTRECIPE to the System Job Manager for fax print to get resources for this job.
 *        faxapp_fax_print_start_job() fuction is called in Rx Phase C (after the FLAG_PRINT_START is set in fipp_mem_to_prn() function).
 *
 * \author tycho03@sindoh.com
 * \date
 *
 **/
void faxapp_fax_print_start_job( void )
{
    error_type_t err = 0;
    STARTRECIPE *StartIt = (STARTRECIPE *)NULL;
    MESSAGE faxapp_mq_buf;

    printf("\n(YHCHO) %s()\n", __FUNCTION__);

    second_page_on_sheet = duplex_print_page_count % 2; // 0 : front, 1 : back

    if((fsu_get_isDuplex(fsu_job_num_prn) != ENABLE) || !second_page_on_sheet)
    {
        printf("\n(YHCHO) %s() MSG_STARTRECIPE\n", __FUNCTION__);

        StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
        XASSERT(StartIt != NULL,0);

        memset( &faxapp_mq_buf, 0, sizeof(faxapp_mq_buf) );

        faxapp_mq_buf.msgType = MSG_STARTRECIPE;

        StartIt->Job = ejob_FaxPrint;
        StartIt->AdditionalResource = e_FaxPrintApp;
        StartIt->Pipe = 0;
        StartIt->Wait = e_WaitForever;

        faxapp_mq_buf.param3 = StartIt;

        /* send a message to the System Job Manager telling it to kick off a fax print */
        err = SYMsgSend(SJMID, &faxapp_mq_buf);
        XASSERT( err==OK, err );
    }
    else
    {
        printf("\n(YHCHO) %s()  msg : faxapp_next_page\n", __FUNCTION__);
        faxapp_next_page();
    }
}

// NullPipe stubs
static int32_t NullPipeRead(IOReg *Pipe, void *DataBuffer, uint32_t Length)
{
      return 0;
}

static int32_t NullPipeWrite(IOReg *Pipe ,
                   void *DataBuffer, uint32_t Length)
{
    MEM_FREE_AND_NULL(DataBuffer);    // chuck the data.
    return(Length);
}

static int32_t NullPipeIOCmd(IOReg *Pipe, ioctl_cmd_t Cmd, void *Details)
{
    return 0;
}
static IOReg NullPipeMEMORY;
static IOReg *NullPipe;

/*Start a new fax print job
 *
 * \return 0 Failure
 *         Otherwise Handle to the print job
 * */
void faxapp_print_start(CURRENT_RESOURCE *resource, PAGEINFO *page_info)
{
    unsigned char res = 0;
    uint8_t jam_recovery;

    /* get media height in mm */
    res = fsu_get_resolution(fsu_job_num_prn);

    memset( page_info, 0, sizeof( PAGEINFO ) );

    //
    page_info->PageSize = 9;
    page_info->PageType = 1;
    page_info->TonerSaveMode = FALSE;

    page_info->Y_Resolution = MONO_PRINT_RES;
    page_info->X_Resolution = MONO_PRINT_RES;
    page_info->copies = 1;  // set the number of copies.
    page_info->collate = 1;  // set collate on off.
    page_info->InputTray = _print_input_tray;
    page_info->printQuality = PRINTQUALITY_NORMAL;
    page_info->duplexInfo = 1;
    page_info->darkness = 6;

    page_info->monoOrColor = e_Mono;

    page_info->nup_image_params.num_images = 1;
    page_info->nup_image_params.page_orientation = 0;

    //
    page_info->printQuality = PRINTQUALITY_NORMAL;

    page_info->BitsPerPixel = 8;
    page_info->DocType = e_FAX;

    page_info->RawData = TRUE;

    mlimiter_start(mlimiter_by_name("print"));

    if (NullPipe == 0)
    {
        NullPipe = &NullPipeMEMORY;
        memset(NullPipe, 0, sizeof(IOReg));
        NullPipe->pipe_cmd_read  = NullPipeRead;
        NullPipe->pipe_cmd_write = NullPipeWrite;
        NullPipe->pipe_ioctl     = NullPipeIOCmd;
        NullPipe->PipeType       = e_Print;
        NullPipe->pjob_info = PrintJobInfo_construct(NullPipe);
        NullPipe->pjob_info->jobDocType = e_FAX;
    }
    printvar_get_jamrecovery(&fax_old_jam_recovery);
    jam_recovery = 0;
    printvar_set_jamrecovery(&jam_recovery);

    fax_print_job = print_job_construct(mlimiter_by_name("print"), NullPipe);

    print_job_set_doc_type(fax_print_job, page_info->DocType);
    print_job_set_collation(fax_print_job, page_info->collate);
    print_job_set_darkness_level(fax_print_job, page_info->darkness);
    print_job_set_num_copies(fax_print_job, page_info->copies);
    print_job_set_job_info(fax_print_job, PRINTJOB);
    //print_job_set_copy_tray_setting_info(pjob, scan_job_conf.printTrayPaperInfo[TRAY_MPT], scan_job_conf.printTrayPaperInfo[TRAY1],
    //									scan_job_conf.printTrayPaperInfo[TRAY2],scan_job_conf.printTrayPaperInfo[TRAY3]);

    if(fsu_get_isDuplex(fsu_job_num_prn) == ENABLE)
    {
        print_job_set_duplex(fax_print_job, DMDUP_VERTICAL);
        print_job_set_duplex_rotate_backside(fax_print_job, true);
        page_info->duplexInfo = DMDUP_VERTICAL;
    }

    print_job_start(fax_print_job, resource->Destination);
}


void faxapp_setup_page_info(PAGEINFO *page_info, uint32_t pix_width, uint32_t pix_height, bool color, uint16_t resolution)
{
    ENGMEDIASIZES FaxMediaSizePixels;
    uint32_t media_width_mmx100, media_height_mmx100;

    /* Set page info parameters */
    memset(page_info, 0, sizeof(PAGEINFO));

    page_info->BitsPerPixel = 8;
    page_info->X_Resolution = MONO_PRINT_RES;
    page_info->Y_Resolution = MONO_PRINT_RES;

    page_info->DocType = e_FAX;
    page_info->monoOrColor = e_Mono;
    page_info->InputTray = _print_input_tray;
    page_info->PageSize = _print_media_size;
    page_info->PageType = MEDIATYPE_PLAIN_PAPER;
    page_info->Video_Y = pix_height;
    page_info->PixelsPerLine = pix_width;
    page_info->Video_X = pix_width;
    page_info->RawData = TRUE;
    page_info->copies = 1;

    memset(&FaxMediaSizePixels, 0, sizeof(ENGMEDIASIZES));
    paper_get_media_size(page_info->PageSize,  &media_width_mmx100, &media_height_mmx100);
    FaxMediaSizePixels.width = MMx100_TO_DOTS(media_width_mmx100, page_info->X_Resolution);
    FaxMediaSizePixels.height = MMx100_TO_DOTS(media_height_mmx100, page_info->Y_Resolution);

    if (FaxMediaSizePixels.width & 0xf)
    {
        FaxMediaSizePixels.width = (FaxMediaSizePixels.width&0xfffffff0)+ 16;
    }

    page_info->nup_image_params.page_orientation = 0;
    page_info->nup_image_params.num_images = 1;
    page_info->nup_image_params.image_width = FaxMediaSizePixels.width;
    page_info->nup_image_params.image_height = FaxMediaSizePixels.height;

    if(fsu_get_isDuplex(fsu_job_num_prn))
    {
        page_info->duplexInfo = 3;// 1: simplex  	 2,3: duplex
    }
    else
    {
        page_info->duplexInfo = 1;
    }
    printf("\n(YHCHO) %s() - duplexInfo : %d\n", __FUNCTION__,page_info->duplexInfo);
}


static void _send_strip(print_page_t *print_page, void *data, uint32_t buf_size, bool last_strip, const PAGEINFO *page_info, int num_rows)
{
    COLOR color=COLOR_BLACK;
    BigBuffer_t *big_buffer;
    PLANE_TYPE planetype = GRAYSCALE;
    bool print_right_to_left = TRUE;
    compression_info_t compression_info;

    compression_info.comp_type = COMPTYPE_NONE;
    compression_info.comp_details.rgb.bits_per_pixel = 8;

    big_buffer = dma_buffer_adopt(data, buf_size);

    print_plane_add_data( print_page,
                          color,
                          big_buffer,
                          buf_size,
                          planetype,
                          last_strip, // only pass along last_strip if we're not padding; otherwise done below.
                          page_info->X_Resolution, page_info->Y_Resolution,
                          page_info->PixelsPerLine,
                          num_rows,
                          page_info->BitsPerPixel,
                          (bool)print_right_to_left, &compression_info);
}

print_page_t* faxapp_printer_create_page(PAGEINFO *page_info)
{
    print_page_t *print_page;

    //
    page_info->Video_Y = print_height;

    page_info->PixelsPerLine = print_width;
    page_info->Video_X = print_width;

    page_info->nup_image_params.image_height = print_height;
    page_info->nup_image_params.image_width = print_width;

    page_info->PageSize = paper_enum;
    page_info->InputTray = _print_input_tray;
    //

    print_page = print_page_construct(fax_print_job);
    ASSERT(print_page != NULL);

    print_page_set_brightness( print_page, 80 );
    print_page_set_contrast( print_page, 80 );

    print_page_set_media_size( print_page, page_info->PageSize );
    print_page_set_media_type( print_page, page_info->PageType );
    print_page_set_input_source(print_page, page_info->InputTray );
    print_page_set_duplex(print_page, page_info->duplexInfo );

    print_page_set_print_quality( print_page, page_info->printQuality );

    print_page_set_dpi_x(print_page, page_info->X_Resolution);
    print_page_set_dpi_y(print_page, page_info->Y_Resolution);
    print_page_set_raster_x(print_page, page_info->Video_X *
                page_info->BitsPerPixel);

    print_page_set_video_x(print_page, page_info->Video_X);
    printf("\n(YHCHO) %s() Video_X : %ld\n", __FUNCTION__, page_info->Video_X);
    print_page_set_output_bpp(print_page, page_info->BitsPerPixel); //PRINTER_PIXEL_DEPTH);

    print_page_set_num_copies(print_page, page_info->copies);
    print_page_set_color_mode(print_page, page_info->monoOrColor);
    print_page_set_colorspace(print_page, (page_info->monoOrColor == e_Mono) ? e_additive_gray : e_sRGB);

    print_page_set_oneup_rotation_params(print_page, 0);
    print_page_set_nup_params(print_page, &(page_info->nup_image_params));

    print_page_set_app(print_page, (uint8_t)0xFA);

    return print_page;
}

void faxapp_printer_end_page( print_page_t *print_page, const PAGEINFO *page_info)
{
    printf("\n(YHCHO) %s()\n", __FUNCTION__);

    print_plane_end(print_page, COLOR_BLACK);
    print_page_end(print_page, NULL);
    print_page_free(print_page);
}

/*Move on to print next page
 *
 * \return 0   Success
 *         1   Doc end reached
 *         < 0 Error
 * */
int faxapp_print_next_page(void)
{
    uint32_t page_height;
    uint8_t *strip_buf = (uint8_t *)NULL;;
    int ret;
    unsigned char strip_index = 0;
    uint16_t lines_in_strip = LINES_IN_STRIP;
    active_page_t active_page;
    bool data_complete;
    int pages_to_print;
    uint32_t job_id;
    uint16_t resolution;
    int size = 0;
    int compare_height = 0;
    int current_pos = 0;

    memset(&active_page, 0, sizeof(active_page));

    print_width = change_width;
    print_height = change_height;
    printf("\n(YHCHO) %s() - print_width = %d print_height = %d\n", __FUNCTION__,print_width,print_height);

    compare_height = print_height;

    if(Auto_Reduction_state == AUTO_REDUCTION)
    {
        if(print_height - (DiscardLength * paper_pixel_per_mm) > paper_height_pixel)
        {
            if(getAutoReductionOn() == DISABLE)
            {
                Auto_Reduction_state = NO_REDUCTION_1PAGE;
                compare_height = paper_height_pixel;
            }
        }
    }
    else if(Auto_Reduction_state == NO_REDUCTION_2PAGE)
    {
        //compare_height = print_height - A4_HEIGHT;
        compare_height = paper_height_pixel;
        if(need_image_rotation == 1)
        {
            current_pos = print_width*paper_height_pixel;
            //if(lseek(fhandle, (print_width*paper_height_pixel), SEEK_CUR) == -1)
        }
        else
        {
            current_pos = print_width*(paper_height_pixel-DiscardLength*paper_pixel_per_mm);
            //if(lseek(fhandle, (print_width*(paper_height_pixel-DiscardLength*paper_pixel_per_mm)), SEEK_CUR) == -1)
        }
    }

    /* protect access to _print_info while we determine the state of the pages BEFORE
     we do anything with them...this will be our known state until we return later
     if needed */
    //_access(1);
    /* store these varaibles */
    data_complete = _print_info.data_complete = 1;
    pages_to_print = _print_info.pages_to_print;
    job_id = _print_info.job_id;
    //_access(0);

    resolution = MONO_PRINT_RES;
    active_page.color = 0;
    active_page.type = FAX_HEADER_TYPE_MONO;
    /* pix width and height is just the max (media size) */

    active_page.width = print_width;
    active_page.height = compare_height;

    printf("FAXAPP: height:%d resolution:%d\n", active_page.height, resolution);
    // the print line width in byte.
    active_page.output_byte_width = active_page.width;

    /* set page info structure */
    //faxapp_setup_page_info(&page_info, active_page->width, active_page->height, active_page->color, resolution);

    fax_print_page = faxapp_printer_create_page(&fax_print_pageinfo);
    print_page_set_image_height_lines(fax_print_page, active_page.height);

    ASSERT(fax_print_page);

    //fax_print_page->page_data->image_output_info.image_scale_right_padding = (uint32_t)0;
    //fax_print_page->page_data->image_output_info.image_top_padding = (uint32_t)0;

    print_page_start(fax_print_page);
    print_plane_start(fax_print_page, COLOR_BLACK);

    page_height = active_page.height; // height of printable area
    /* submit strips for print. */
    while (!active_page.done)
    {
        if (strip_index > MAX_NUM_STRIPS)
        {
            printf("FAXAPP Ran out of strip storage: strip_index:%d(MAX:%d)\n", strip_index, MAX_NUM_STRIPS);
            MEM_FREE_AND_NULL(faxapp_print_buffer);
            return (NP_ERROR);
        }

        strip_buf = (uint8_t *)MEM_MALLOC(active_page.output_byte_width * LINES_IN_STRIP);
        if (strip_buf == NULL)
        {
            printf("FAXAPP: Fail to allocate strip buffer memory.  We will return soon to try again!\n");
            MEM_FREE_AND_NULL(faxapp_print_buffer);
            posix_sleep_ticks(200);
            return (NP_MORE_DATA);
        }
        else
        {
            lines_in_strip = LINES_IN_STRIP;
            ret = SUCCESS;

            if (active_page.pageLineCnt + lines_in_strip >= page_height)
            {
                lines_in_strip = page_height - active_page.pageLineCnt + 15;
            }

            memset(strip_buf, 0, active_page.output_byte_width * LINES_IN_STRIP);

            if(Auto_Reduction_state == NO_REDUCTION_2PAGE && active_page.pageLineCnt >= print_height - paper_height_pixel)
            {
                ret = 0;
            }
            else if((active_page.page_status != PAGE_BREAK) && (active_page.page_status != DOC_END))
            {
                size = active_page.output_byte_width * LINES_IN_STRIP;
                if(current_pos + size > print_width * print_height)
                {
                    size =  print_width * print_height - current_pos;
                }

                if (size == 0)
                {
                    MEM_FREE_AND_NULL(strip_buf);
                    MEM_FREE_AND_NULL(faxapp_print_buffer);

                    return (NP_ERROR);
                }

                memcpy(strip_buf, &faxapp_print_buffer[current_pos], size);
                current_pos += size;
                lines_in_strip = size/(active_page.output_byte_width);
            }

            active_page.pageLineCnt += lines_in_strip;
            //printf("\n(YHCHO) %s() pageLineCnt = %d\n", __FUNCTION__,active_page->pageLineCnt);

            page_height = active_page.height; // height of printable area

            active_page.done = (active_page.pageLineCnt >= page_height) || (strip_index == MAX_NUM_STRIPS);

            if (active_page.pageLineCnt >= page_height)
            {
                lines_in_strip -= (active_page.pageLineCnt - page_height);
            }

            // The memory will be released by the print service. Remove our reference.
            _send_strip(fax_print_page, strip_buf, size, active_page.done, &fax_print_pageinfo, lines_in_strip);
            //printf("\n(YHCHO) %s() _send_strip OK, %d\n", __FUNCTION__,active_page.pageLineCnt);

            strip_index++;
        }
    } // DONE with page

    faxapp_printer_end_page(fax_print_page, &fax_print_pageinfo);

    if(Auto_Reduction_state == AUTO_REDUCTION)
    {
        MEM_FREE_AND_NULL(faxapp_print_buffer);
        active_page.page_status = DOC_END;
        second_page_on_sheet = duplex_print_page_count % 2;
    }
    else if(Auto_Reduction_state == NO_REDUCTION_1PAGE)
    {
        Auto_Reduction_state = NO_REDUCTION_2PAGE;
        active_page.page_status = SUCCESS;
        second_page_on_sheet = duplex_print_page_count % 2;
        duplex_print_page_count++;
    }
    else if(Auto_Reduction_state == NO_REDUCTION_2PAGE)
    {
        MEM_FREE_AND_NULL(faxapp_print_buffer);
        active_page.page_status = DOC_END;
        Auto_Reduction_state = AUTO_REDUCTION;
        second_page_on_sheet = duplex_print_page_count % 2;
    }

    //pCtrl->pages_printed++;

    if (active_page.page_status == DOC_END && data_complete)
    {
        printf("\n(YHCHO) %s() NP_END_OF_DOC\n", __FUNCTION__);
        memset(&active_page, 0, sizeof(active_page_t));
        return (NP_END_OF_DOC); /* end of doc */
    }
    else
    {
        printf("\n(YHCHO) %s() NP_MORE_DATA\n", __FUNCTION__);
        memset(&active_page, 0, sizeof(active_page_t));
        return (NP_MORE_DATA);
    }
}


/**
 * Complete a print job
 *
 * \return 0 Success
 *         Otherwise error
 */
int faxapp_print_end(CURRENT_RESOURCE *job_resources)
{
    MESSAGE msg;
    error_type_t err;

    if(fax_print_job != NULL)
    {
        print_job_end(fax_print_job);
        mlimiter_stop(mlimiter_by_name("print"));
        fax_print_job = NULL;
    }

    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_FREERECIPE;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = job_resources;
    err = SYMsgSend( SJMID, &msg );
    XASSERT( err==OK, err );

    printvar_set_jamrecovery(&fax_old_jam_recovery);

    return 0;
}


/*
 *  Agent thread function for print
 *
 */
static void *faxapp_print_mgr(void *unused)
{
    MESSAGE job_msg;
    MESSAGE msg;
    CURRENT_RESOURCE *OurResource = NULL;
    int res = NP_END_OF_DOC;
    int pxretcode = 0;
    int setValue = 0;

    printf("FAX PRINT: faxapp_print_mgr start\n");

    while (1)
    {
        pxretcode = posix_wait_for_message(faxapp_print_msgq, (char*)&msg, sizeof(MESSAGE), POSIX_WAIT_FOREVER);
        XASSERT(pxretcode == 0, pxretcode);
        switch (msg.msgType)
        {
            case FAXAPP_MSG_NEXT_PAGE:
                if(res != NP_MORE_DATA)
                {
                    if (fsu_get_memory_full_status_ram() == 1)
                    {
                        printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);
                        faxapp_page_end();
                        break;
                    }

                    if(!fsu_print_page_setting())
                    {
                        printf("\n(YHCHO) %s() - print page setting fail\n", __FUNCTION__);

                        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_PRINT_PAGE_SETTING);
                    }

                    if(fsu_print_image_setting())
                    {
                        printf("\n(YHCHO) %s() - print image setting fail\n", __FUNCTION__);

                        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_PRINT_IMAGE_SETTING);
                    }

                    if(faxapp_print_buffer == NULL)
                    {
                        printf("\n(YHCHO) %s() - faxapp_print_buffer must be not NULL !!\n", __FUNCTION__);
                        ASSERT(0);
                    }

					if(setValue == 0)
					{
						setValue = 1;
						faxapp_print_start(OurResource, &fax_print_pageinfo);
						faxapp_print_page_info(&fax_print_pageinfo);
					}

                    _print_media_size = paper_enum;
                    switch(input_tray)
                    {
                        case 0:
                            _print_input_tray = TRAY_1;
                            break;
                        case 1:
                            _print_input_tray = TRAY_2;
                            break;
                        case 2:
                            _print_input_tray = TRAY_3;
                            break;
                        case 3:
                            _print_input_tray = TRAY_4;
                            break;
                        case 8:
                            _print_input_tray = TRAY_ANY;
                            break;
                        default:
                            _print_input_tray = TRAY_2;
                            break;
                    }
                    printf("\n(YHCHO) %s() _print_input_tray : %d\n", __FUNCTION__,_print_input_tray);
                }

                res = faxapp_print_next_page();

                if(user_input_print_cancel == 1)
                {
                    if(getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_EVERY_PAGE && !fsu_get_rcv_done())
                    {
                        ;
                    }
                    else
                    {
                        printf("\n(YHCHO) %s() faxapp_print_cancel!\n", __FUNCTION__);
                        faxapp_print_cancel();
                        break;
                    }
                }
                else if(res == NP_MORE_DATA) //Auto Reduction OFF & Long paper print
                {
                    printf("\n(YHCHO) %s() NP_MORE_DATA\n", __FUNCTION__);
                    msg.msgType = FAXAPP_MSG_NEXT_PAGE;
                    msg.param1 = 0;
                    msg.param2 = 0;
                    msg.param3 = NULL;
                    SYMsgSend(FAXPRINTID, &msg);
                    break;
                }
                else if(!fsu_get_isDuplex(fsu_job_num_prn) || !second_page_on_sheet || getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_ALL)
                {
                    if (res == NP_END_OF_DOC)
                    {
                        printf("\n(YHCHO) %s() NP_END_OF_DOC\n", __FUNCTION__);
                        if(getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_ALL)
                        {
                            if(!fsu_total_page_print_complete_check())
                            {
                                msg.msgType = FAXAPP_MSG_NEXT_PAGE;
                                msg.param1 = 0;
                                msg.param2 = 0;
                                msg.param3 = NULL;
                                SYMsgSend(FAXPRINTID, &msg);
                                break;
                            }
                        }

                        faxapp_print_end(OurResource);
                        de_activate_mem_to_prn();
                        faxapp_print_is_on = 0;

                        break;
                    }

                    if(res == NP_ERROR)
                    {
                        ;
                    }
                }
                else
                {
                     printf("\n(YHCHO) %s() Wait Back Page\n", __FUNCTION__);
                     de_activate_mem_to_prn();
                }

                break;

            case FAXAPP_MSG_PAGE_END:
                printf("\n(YHCHO) %s() FAXAPP_MSG_PAGE_END\n", __FUNCTION__);

                faxapp_print_end(OurResource);
                faxapp_print_is_on = 0;

                break;

            case FAXAPP_MSG_CANCEL_DONE:
                /* this event allows us to finish up all events that came between cancel and actualy canceling the job */
                ACK_CANCEL(msg.param3, FAXPRINTID);

                de_activate_mem_to_prn();
                break;

            case MSG_RESOURCES:
                OurResource = (CURRENT_RESOURCE *) msg.param3;
                break;

            case MSG_ACKRECIPE:
//                _update_status(FAX_SUBJOB_ACTIVE);
                faxapp_print_is_on = 1;
                setValue = 0;
                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PRINTING, 0, 0, 0, 0, fsu_job_num_prn);

                user_input_print_cancel = 0;

                _print_info.fname = NULL;//fname;
                _print_info.job_id = ejob_FaxPrint;
                _print_info.pjob_handle = NULL;
                _print_info.data_complete = 0;//complete;
                _print_info.pages_to_print = 1;//pages;

                if (OurResource == NULL)
                {
                    printf("OurResource == NULL");
                    faxapp_print_cancel();
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
                printf("Got NAKRECEIPE! Abort printing!");
                faxapp_print_is_on = 0;
                break;

            case MSG_CANCELJOB:
                printf("FAX PRINT CANCEL!");

                faxapp_print_end(OurResource);

                faxapp_print_subsystem_cancel();

                //_print_info.pCtrl.canceling = 1;
                msg.msgType = FAXAPP_MSG_CANCEL_DONE;
                msg.param1 = 0;
                msg.param2 = 0;
                //msg.param3 = msg.param3; // pass back the resource
                SYMsgSend(FAXPRINTID, &msg);

                faxapp_print_is_on = 0;
                break;

            case MSG_USER_INPUT:
                user_input_print_cancel = 1;
                printf("\n(YHCHO) %s() user_input_print_cancel : %d\n", __FUNCTION__,user_input_print_cancel);
                break;

            default:
                printf("FAXAPP: print got unknown msg! type = [%d]\n", msg.msgType);
                break;
        }
    }

    return 0;
}

int faxapp_print_cancel(void)
{
    MESSAGE msg;
    memset(&msg, 0, sizeof(msg));
    msg.msgType = MSG_CANCELJOB;
    msg.param1 = SYS_REQUEST;
    msg.param2 = 0;
    msg.param3 = (void*) e_FaxPrintApp;

    printf("FAXAPP: Canceling fax print job\n");
    SYMsgSend(SJMID, &msg);
    return (0);
}

int faxapp_print_subsystem_cancel(void)
{
    MESSAGE msg;
    memset(&msg, 0, sizeof(msg));
    msg.msgType = MSG_CANCELJOB;
    msg.param1 = SYS_REQUEST;
    msg.param2 = 0;
    msg.param3 = (void*) e_Printer;

    printf("FAXAPP: Canceling fax print subsystem job\n");
    SYMsgSend(SJMID, &msg);
    return (0);
}

void faxapp_print_user_input(void)
{
    MESSAGE msg;
    error_type_t err;

    if(faxapp_print_is_on == 0)
    {
        faxapp_print_subsystem_cancel();
    }
    else
    {
        memset( &msg, 0, sizeof(msg));
        msg.msgType = MSG_USER_INPUT;
        msg.param1 = 1;//PARAM_USER_OK;
        err = SYMsgSend( FAXPRINTID, &msg );
        XASSERT( err==OK, err );
    }
}

