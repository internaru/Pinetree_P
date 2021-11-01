/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include "SDHFaxCommonHeader.h"
#include "../../fax_memmgr/include/fax_memAPI.h"
#include <string.h>
#include <stdio.h>
#include <dprintf.h>
#include <assert.h>
#include "agRouter.h"
#include "ATypes.h"
#include "lassert.h"
#include "agConnectMgr.h"
#include "agResourceMgr.h"
#include "SJMgr.h"

#include "paper_size_api.h"

#include "scantypes.h"
#include "scanvars.h"
#include "scanplat.h"
#include "scancap.h"
#include "scands.h"
#include "scanmsg.h"
#include "utils.h"
#include "../../../../common/scan/apps/a3mergeInterface.h"
#include "../../../../common/scan/apps/scanapp_uiscan.h"
#include "../../../../../../common/scan/include/scanmech.h"
#include "faxapp_job.h"
#include "faxapp_param.h"
#include "faxapp_scan.h"
#include "faxapp_notify.h"
#include "posix_ostools.h"
#include "../../CommonHeader/SDHFaxCommonHeader.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
#include "../../../../common/scan/apps/scanapp_save_file.h"
#include "fsu.h"
#endif
#ifdef HAVE_IMAGELOG
#include "../../../../common/scan/apps/scanapp_imagelog.h"
#include "imagelog.h"
#endif

#define FAX_SCAN_RESOLUTION 300  // when we scan for mono we always do this resolution.
#if 0 /* scan currently is always passing back 300 dpi data */
#define FAX_COLOR_SCAN_RESOLUTION 200  
#else
#define FAX_JPEG_SCAN_RESOLUTION 300  
#endif
#define FAX_COLOR_SCAN_COMPONENTS 4 /* RGBX */

#define FAX_SCAN_WIDTH_A4 825//825//827//849
#define FAX_SCAN_HEIGHT_A4 1170//1170//1170//1138

#define FAX_SCAN_WIDTH_LETTER 850//864//864//850
#define FAX_SCAN_HEIGHT_LETTER 1100//1100

#define FAX_SCAN_WIDTH_LEGAL 850//864
#define FAX_SCAN_HEIGHT_LEGAL 1400//1400//1400

#define FAX_SCAN_WIDTH_A5_ROTATED 825//826//825//849
#define FAX_SCAN_HEIGHT_A5_ROTATED 585//585//1138

#define FAX_SCAN_WIDTH_A5 585//849
#define FAX_SCAN_HEIGHT_A5 825//825//826//1138

#define FAX_SCAN_WIDTH_A6 413
#define FAX_SCAN_HEIGHT_A6 585

#define FAX_SCAN_WIDTH_B5 714
#define FAX_SCAN_HEIGHT_B5 1011

#define FAX_SCAN_SRC_SCALE 100 /* CURRENTLY SCAN HANGS IF WE CHANGE THIS...
                                  percent to scale source to leave room for header */

/* set to 1 to see all scan events on serial */
#define FAX_DEBUG_SCAN_EVENTS (0)

#define CONVERT_300DPI (HPDF_REAL)((HPDF_REAL)300.0/(HPDF_REAL)72.0)

static unsigned char FaxAppScanStack[FAXAPP_SCAN_STACK_SIZE] ALIGN8;
static mqd_t faxapp_scan_msgq;
static pthread_t faxapp_scan_thd_id;
static void *faxapp_scan_mgr (void *unused);


//taeyoukim add.
int fax_scan_page_count = 0;
int text_photo_mode = 0; // 0 : text, 1 : photo
BOOLEAN isScanComplete = FALSE;
BOOLEAN A3_merge = FALSE;
BOOLEAN fb_legal = FALSE;
BOOLEAN scan_cancel_flag = FALSE;
int image_rotation = 0; //0: PASS, 1: A5 SEF, 2: B5 SEF
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
static char pdfFileName[MAX_LEN_FILE_PATH_NAME] = {0, };

#endif
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];

typedef struct _faxapp_page_s
{
    uint8_t * data_strips;
    uint32_t current_lines;
    BOOL print_page_active;
}faxapp_page_t;

static faxapp_page_t fax_duplex_pages[2];

extern int gFaxBgRemove;
extern UINT8 gFaxScanSource;
extern UINT16 fsu_job_num_scan;
extern UINT16 fsu_job_num_scan_image_log;
extern UINT8 FSUCurrentFBScanPageNo;

extern int gFaxAbnormalFlag;
extern UINT8 isDKSolution;

static BOOL adf_paper_is_present( void );
extern BOOL adf_sensor_paper_present( void );
extern void MakeRawToPbm (int CurrentPage, uint8_t *buffer, int wid, int hig, UINT16 jnum, int fax_mode);
extern void de_activate_scan_to_mem(void);
extern UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
extern void fsu_update_total_pages (UINT16 jnum, UINT8 Page);
extern UINT32 faxapp_get_Density(void);
extern void fax_rotate(uint8_t *pS, uint8_t *pD, int type_width, int strip_width, int strip_height, int margin);
extern UINT8 faxapp_get_ImageType(void);
extern scan_err_t scanmech_get_fb_margins( int dpi, 
                                    scan_cmode_t cmode,
                                    struct scan_flatbed_margins *fb_margins );
extern scan_err_t scanmech_get_adf_margins( int dpi, 
                                     scan_cmode_t cmode,
                                     struct scan_adf_margins *adf_margins );                                    
extern int scanner_service_sync_marginTable(void);
extern UINT32 faxapp_get_AutoDarkness(void);

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern void fsu_request_make_dot_txt_file (const char *pPath, UINT16 jnum);
extern void fsu_request_make_dot_end_file (const char *pPath, UINT16 jnum, UINT8 job_type);
extern void fsu_request_get_pdf_file_name(const char *pPath, UINT16 jnum, UINT8 job_type, char *pdf_file_name);
extern void fsu_update_unique_job_id (UINT16 jnum, UINT32 jobId);
extern UINT8 fsu_get_broadcast (UINT16 jnum);
#endif
extern void fsu_update_faxScanInfo (UINT16 jnum, void *scanInfo);
extern void scanman_set_scanpapersize(uint16_t scanPaperSize);

#if __TX_REPORT_IMAGE_PRINT__
extern void tx_report_image_setting(UINT16 jnum, int type);
extern int getgFaxTxResultRptImage(void);
#endif

//typedef unsigned char fax_app_brightness_t;
typedef FAX_VR_t fax_app_resolution_t;

typedef struct
{
  void *context;
//  fax_app_subjob_callback_t update_callback; 
  char *fname;
//  fax_store_handle_t store;
  void *tiff_writer_handle;
  bool color;
//  struct jpeg_compress_struct jpeg_comp_handle;
  int brightness;
  int background;
  fax_app_resolution_t resolution;
  uint16_t active_page;
  uint16_t pages_completed;
} _scan_job_info_t;
/* only one at this time */
static _scan_job_info_t _scan_info = {.pages_completed=0};

/*----------------------------------------------------------*/
#if FAX_DEBUG_SCAN_EVENTS
#define _TS(_a) {_a, #_a}
static const char *msg_to_string(uint32_t msg)
{
  int i;
  typedef struct
  {
    AGMSG msg;
    char *str; 
  } msg_to_str_t;
  static const msg_to_str_t string_tbl[] = 
  {
    _TS(MSG_RESOURCES),
    _TS(MSG_FAXER_EVT),
    _TS(MSG_ACKRECIPE),
    _TS(MSG_NAKRECIPE),
    _TS(MSG_SCAN_SIZE),
    _TS(MSG_SCAN_PAGESTART),
    _TS(MSG_SCAN_PLANEDATA),
    _TS(MSG_SCAN_PAGEEND),
    _TS(MSG_SCAN_JOBSTART),
    _TS(MSG_SCAN_JOBEND),
  };

  for(i=0; i<ARRAY_SIZE(string_tbl); i++) 
  {
    if(string_tbl[i].msg == msg)
     break;
  }
  if(i<ARRAY_SIZE(string_tbl))
    return(string_tbl[i].str);
  else
    return("UNKNOWN_MSG");
}
#endif


int is_adf_paper_present (void)
{
    int ret_val = 0;

    if (adf_paper_is_present() == TRUE)
    {
        ret_val = 1;
    }
    else
    {
        ret_val = 0;
    }

    return ret_val;
}


static BOOL adf_paper_is_present( void )
{
    struct scan_platform_capabilities scan_capabilities;

    scancap_get_capabilities( &scan_capabilities );
    if( !scan_capabilities.has_adf ) {
        return FALSE;
    }

    /* ballen 10/4 -- make this read directly from the sensor */
    return adf_sensor_paper_present();
}


void FaxScanJobMgrInit(void)
{
    int retcode = 0;

    //
    // Create the queue
    //
    posix_create_message_queue(&faxapp_scan_msgq, "/FaxAppScanQueue", FAXSCANTASK_QUEUE_SIZE, sizeof(MESSAGE));

    //
    // Register a message queue.
    //
    router_register_queue(SCANTOFAXID, faxapp_scan_msgq);

    //
    // Register the resource
    //
    rm_register(e_ScanToFax, SCANTOFAXID);

    //
    // start the fax app job mgr task
    //
    retcode = posix_create_thread(&faxapp_scan_thd_id, faxapp_scan_mgr, 0, "FaxAppScanMgr", FaxAppScanStack, FAXAPP_SCAN_STACK_SIZE, POSIX_THR_PRI_NORMAL);

    XASSERT( retcode==0, retcode );
}


void faxapp_fax_scan_start_job( void )
{
    error_type_t err = 0;
    STARTRECIPE *StartIt = (STARTRECIPE *)NULL;
    MESSAGE faxapp_mq_buf;

    printf("\n(YHCHO) %s()\n", __FUNCTION__);

    StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
    XASSERT(StartIt != NULL,0);

    memset( &faxapp_mq_buf, 0, sizeof(faxapp_mq_buf) );

    faxapp_mq_buf.msgType = MSG_STARTRECIPE;

    StartIt->Job = ejob_ScanFax;
    StartIt->AdditionalResource = 0;//e_ScanToFax;
    StartIt->Pipe = 0;
    StartIt->Wait = e_NoWait;
    StartIt->SendingModule = FAXER_SCAN_JOB_AGENT_ID;

    faxapp_mq_buf.param3 = StartIt;

    err = SYMsgSend(SJMID, &faxapp_mq_buf);
    XASSERT( err==OK, err );
}


void clearFaxScanPageCount(void)
{
	_scan_info.pages_completed = 0;
}

/*----------------------------------------------------------*/
/* setup the scan job */
void _setup_scan(struct scanvars *sv, _scan_job_info_t *scan_info)
{
	//int32_t             ResX=300, ResY = 300;
	uint32_t scan_width = 0;
	uint32_t scan_height = 0;
	//struct scanvars     *sv = NULL;
	uint32_t            scan_resolution;
	UINT32			  page_size;
	int width_offset = 0;
	int height_offset = 0;
	int scale_size_x = 100;
	int scale_size_y = 0;//100;
	scan_err_t scerr = SCANERR_NONE;
    struct scan_flatbed_margins fb_margins;
    struct scan_adf_margins adf_margins;
    int legal_sendmsg_check = 0;

	if(sv == NULL)
	{
		printf("Fail to get new scan var, Abort!\n");
		return;
	}

	//Sync Margin Table with DB
	scanner_service_sync_marginTable();

	page_size = faxapp_get_originalSizeCode();
	image_rotation = 0;
    printf("\n(YHCHO) %s() page_size = %ld\n", __FUNCTION__,page_size);
	
	switch(page_size)
	{
		case MEDIASIZE_LETTER :
			scan_width  = FAX_SCAN_WIDTH_LETTER;
			scan_height = FAX_SCAN_HEIGHT_LETTER;
			scale_size_x = 101;
			scale_size_y = 98;
			break;
		case MEDIASIZE_A4 :
			scan_width  = FAX_SCAN_WIDTH_A4;
			scan_height = FAX_SCAN_HEIGHT_A4;
			scale_size_x = 102;
			scale_size_y = 98;
			break;
		case MEDIASIZE_LEGAL :
			scan_width  = FAX_SCAN_WIDTH_LEGAL;
			scan_height = FAX_SCAN_HEIGHT_LEGAL;
			scale_size_x = 101;
			scale_size_y = 98;
			break;
		case MEDIASIZE_A5_ROTATED :
			scan_width  = FAX_SCAN_WIDTH_A5_ROTATED;
			scan_height = FAX_SCAN_HEIGHT_A5_ROTATED;
			scale_size_x = 102;
			scale_size_y = 100;
			break;
		case MEDIASIZE_A3:
			scan_width  = FAX_SCAN_WIDTH_LETTER;
			scan_height = FAX_SCAN_HEIGHT_A4;
			scale_size_y = 102;
			break;
		case MEDIASIZE_11X17:
			scan_width  = FAX_SCAN_WIDTH_LETTER;
			scan_height = FAX_SCAN_HEIGHT_LETTER;
			scale_size_y = 105;
			break;			
		case MEDIASIZE_A5:
			scan_width  = FAX_SCAN_WIDTH_A5;
			scan_height = FAX_SCAN_HEIGHT_A5;
			scale_size_y = 102;
			image_rotation = 1; //A5
			break;		
		case MEDIASIZE_B5:
			scan_width  = FAX_SCAN_WIDTH_B5;
			scan_height = FAX_SCAN_HEIGHT_B5;
			scale_size_y = 100;
			image_rotation = 2;	//B5
			break;
		case MEDIASIZE_A6:
			scan_width  = FAX_SCAN_WIDTH_A6;
			scan_height = FAX_SCAN_HEIGHT_A6;
			scale_size_x = 102;
			scale_size_y = 98;
			break;
		default:
			scan_width  = FAX_SCAN_WIDTH_A4;
			scan_height = FAX_SCAN_HEIGHT_A4;
			scale_size_x = 102;
			scale_size_y = 98;
			break;
	}

	if(page_size == MEDIASIZE_A3 || page_size == MEDIASIZE_11X17)
	{
		A3_merge = TRUE;
	}
	else
	{
		A3_merge = FALSE;
	}
    /* setup tiff */
    scan_resolution = FAX_SCAN_RESOLUTION;

	if(fb_legal == TRUE)
	{
		scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_FLATBED);
		printf("FAX SCAN: Fax from flatbed : fb legal\n");
		legal_sendmsg_check = 1;
		scale_size_x = 100;
		scale_size_y = 100;

		scan_width = 850;
		scan_height = 725; // temp
		height_offset = 445;
	}
	else if(gFaxScanSource == FAX_SCAN_SOURCE_FB)//if(!adf_paper_is_present())
	{
		scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_FLATBED);
		printf("FAX SCAN: Fax from flatbed\n");
				
		if(page_size == MEDIASIZE_LEGAL)
		{
			fb_legal = TRUE;
			
			scale_size_x = 100;
			scale_size_y = 100;
			
			scan_width = 850;
			scan_height = 725;
		}

		//MAX_FB_HEIGHT_HINCHES 1156
		if(scan_height > 1156)
		{
			scan_height = 1156;
		}
	}
	else
	{
		if(A3_merge == TRUE) //A3 Scan
		{
			scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_FLATBED);

			//MAX_FB_HEIGHT_HINCHES 1156
			if(scan_height > 1156)
			{
				scan_height = 1156;
			}
		}
		else // ADF Scan
		{
			width_offset = (850 - scan_width)/2 ;

			if(faxapp_get_DualScan())
			{
			  scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_ADF_DUPLEX);
			  printf("FAX SCAN: Dual Scan Fax from ADF\n");		    
			}
			else
			{
			  scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_ADF);
			  printf("FAX SCAN: Simplex Scan Fax from ADF\n");
			}
		}
	}
	
	if(sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED)
	{
		scerr = scanmech_get_fb_margins( sv->dpi, sv->cmode, &fb_margins );
        if( scerr == SCANERR_NONE ) 
        {
			printf("%s: Flatbed LEFT/TOP_Margin_: x=%d, y=%d\n", __FUNCTION__,
                 fb_margins.left_margin_hinches_x, fb_margins.top_margin_hinches_y);

            width_offset += fb_margins.left_margin_hinches_x;
            height_offset += fb_margins.top_margin_hinches_y;
		}
        else 
        {
			printf("Can't get FB Margin. ERR: %d\n", scerr);
		}		
	}
	else
	{
        scerr = scanmech_get_adf_margins( sv->dpi, sv->cmode, &adf_margins );		
        if( scerr==SCANERR_NONE ) 
        {
            printf("%s: ADF left_mar=%d\n", __FUNCTION__, adf_margins.left_margin_hinches);
            width_offset += adf_margins.left_margin_hinches;
		}
        else 
        {
			printf("Can't get ADF Margin. ERR: %d\n", scerr);
		}
	}

	printf("\n(YHCHO) %s() background : %d\n", __FUNCTION__, scan_info->background);
	scanvar_set_copy_brightness(sv, scan_info->brightness);
	scanvar_set_background(sv, scan_info->background );
	scanvar_set_dpi(sv, scan_resolution);
	scerr = scanvar_set_xyscale(sv, scale_size_x, 100, scale_size_y,100);
	XASSERT( scerr==SCANERR_NONE, scerr );

	scanvar_set_area(sv, width_offset, height_offset, scan_width, scan_height);

 	if ( page_size == MEDIASIZE_A3 || page_size == MEDIASIZE_11X17 || (fb_legal == TRUE && legal_sendmsg_check == 0))
 	{
		MESSAGE send_msg;
		error_type_t err;
		memset( &send_msg, 0x00, sizeof(send_msg) );

		send_msg.msgType = MSG_A3MERGE_SET_START;
		send_msg.param1 = A3MERGE_SCAN;
		
		if(fb_legal == TRUE)
		{
			send_msg.param2 = A3MERGE_LG;
		}
		else if(page_size == MEDIASIZE_A3)
		{
			send_msg.param2 = A3MERGE_A3;
		}
		else
		{
			send_msg.param2 = A3MERGE_DLT;
		}
		
		err = SYMsgSend(SCANMANID, &send_msg );
		XASSERT( err==OK, err );
	}
}

void faxapp_fb_legal_scan_second_page_start(void)
{
    MESSAGE msg;
    error_type_t err;
    
    memset( &msg, 0, sizeof(msg));
	msg.msgType = MSG_USER_INPUT;
	msg.param1 = 1;//PARAM_USER_OK;
	err = SYMsgSend( SCANTOFAXID, &msg );
    XASSERT( err==OK, err ); 
}

void faxapp_fb_multiscan_request(int value)
{
    MESSAGE msg;
    error_type_t err;
    
    memset( &msg, 0, sizeof(msg));
	msg.msgType = MSG_USER_INPUT;
	msg.param1 = value;
	err = SYMsgSend( SCANTOFAXID, &msg );
    XASSERT( err==OK, err ); 
}

void faxapp_scan_cancel(void)
{
    MESSAGE msg;
    error_type_t err;
    scan_err_t scerr = SCANERR_NONE;
    
    if(gFaxScanSource != FAX_SCAN_SOURCE_FB && A3_merge != TRUE)
    {
		scerr = scanmech_adf_client_request_pause();
		if( scerr == SCANERR_NONE ) 
		{
			printf("SCANMECH_IOCTL_ADF_CLIENT_PAUSE : Success\n");
		}
		else 
		{
			printf("SCANMECH_IOCTL_ADF_CLIENT_PAUSE : FAIL : %d\n", scerr);						
		}
	}
	
	scan_cancel_flag = TRUE;
	
	memset( &msg, 0, sizeof(msg) );	
	
	msg.msgType = MSG_CANCELJOB;
	msg.param1 = SYS_REQUEST;
	msg.param3 = (void*) e_ScanToFax;
	err = SYMsgSend( SJMID, &msg );
	XASSERT( err==OK, err );
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern int scanner_erase_image_folder(char *folderPath);
void faxapp_scan_solution(int scanResult)
{
	if( faxapp_get_job_type() == FAX_APP_JOB_TYPE_FAX_SOLUTION ) {
		char folder_path[100] = {}; //MAX_SCAN_FILE_FULL_PATH

		memset(pdfFileName, 0, sizeof(pdfFileName));

		scanner_job_get_current_working_folder(folder_path);

		if (fsu_get_broadcast (fsu_job_num_scan) == 0)
		{
			fsu_request_make_dot_txt_file(folder_path, fsu_job_num_scan);
			fsu_request_make_dot_end_file(folder_path, fsu_job_num_scan, SCAN_TO_MEM);
			fsu_request_get_pdf_file_name(folder_path, fsu_job_num_scan, SCAN_TO_MEM, pdfFileName);
		}
		else
		{
			fsu_request_make_dot_txt_file(folder_path, gFaxBroadcastJobInfo[fsu_get_original_doc_id(fsu_job_num_scan)][0]);
			fsu_request_make_dot_end_file(folder_path, gFaxBroadcastJobInfo[fsu_get_original_doc_id(fsu_job_num_scan)][0], SCAN_TO_MEM);
			fsu_request_get_pdf_file_name(folder_path, gFaxBroadcastJobInfo[fsu_get_original_doc_id(fsu_job_num_scan)][0], SCAN_TO_MEM, pdfFileName);
		}
		
		//Request Save PDF File
		if( strlen(pdfFileName) != 0 ) {
			SCAN_PDF_SaveDoc(pdfFileName);
			printf("PDF Save File Request. Path is : %s\n", pdfFileName);
		}
		else {
			printf("pdfFileName is NULL : FAIL\n");
		}

		if(scanResult)
		{
			//Request send (.pdf, .end, .txt ) file
			scanner_send_to_ftp_request_from_fax(faxapp_get_ftpID_fax_solution(),
													faxapp_get_ftpPassword_fax_solution(),
													faxapp_get_ftpAddress_fax_solution(),
													faxapp_get_ftpPort_fax_solution(),
													folder_path,
													fsu_job_num_scan);
		}
		else
		{
			scanner_erase_image_folder(folder_path);
		}
	}
}
#endif

#include <time.h>
extern void add_binary_image_count(void);
extern void sub_binary_image_count(void);
extern int get_binary_image_count(void);
//extern int fax_scan_job_complete;
/*----------------------------------------------------------*/
/*
 *  Fax application code for scanning.
 *
 */
static void *faxapp_scan_mgr (void *unused)
{
    MESSAGE             job_msg;    // Warning -- this message is used by the job macros
    MESSAGE             msg;
    CURRENT_RESOURCE    *OurResource = NULL;
    struct scanvars     *sv = NULL;
    int                 scan_bytes_per_row_padded = 1;
    int                 first_pagestart_seen = 0;
    uint32_t pixels_per_row_padded=0, pixels_per_row=0, total_rows=0, bits_per_pixel;
    uint16_t num_rows = 0;
    uint8_t *planedata = NULL;
#ifdef HAVE_SCAN_SUPPORT
    scan_data_type dtype;
#endif
    bool last_buffer;
    int temp_count = 0;    
    int m_300DPI_width = 2592;
    int m_300DPI_height = 3432;
    int a3_300DPI_width = 3648;
    int i = 0;
    int margin_wid = 0;
    int line_count[2] = {0};
    uint8_t page_side = 0;
    int pxretcode = 0;
    
    printf("FAX SCAN: faxapp_scan_mgr start\n");
    for( i = 0; i < 2; i++ ) {
        fax_duplex_pages[i].data_strips = NULL;
        fax_duplex_pages[i].current_lines = 0;
        fax_duplex_pages[i].print_page_active = false;
    }

    while(1)
    {
        pxretcode = posix_wait_for_message(faxapp_scan_msgq, (char *)&msg, sizeof(MESSAGE), POSIX_WAIT_FOREVER);

        XASSERT(pxretcode == 0, pxretcode);

#if FAX_DEBUG_SCAN_EVENTS
        dbg_printf("FAX SCAN: received event %s\n", msg_to_string(msg.msgType));
#endif
            
        switch(msg.msgType)
        {
            case MSG_FAXER_EVT:
            {
                break;
            }
            
            case MSG_RESOURCES:
            {
                //printf("FAXSCANAPP: MSG_RESOURCES\n");
                OurResource = (CURRENT_RESOURCE *) msg.param3;
                break;
            }
                
            case MSG_ACKRECIPE:
            {
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
				JOB_RESOURCES *tempJobResourcePtr = ((CURRENT_RESOURCE *)OurResource)->JobResources;
				printf("\n(YHCHO) %s() Uinique Job Id = %d\n", __FUNCTION__, tempJobResourcePtr->JobId);
				if(fsu_get_broadcast(fsu_job_num_scan) == 1)
				{
					UINT16 OriginalDocId = fsu_get_original_doc_id(fsu_job_num_scan);
					
					for(i = 0; i< gFax_BC_Num[OriginalDocId].BC_Total_Num; i++)
					{
						UINT16 broadcast_jnum = gFaxBroadcastJobInfo[OriginalDocId][i];
						fsu_update_unique_job_id(broadcast_jnum, tempJobResourcePtr->JobId);
					}
				}
				else
				{
					fsu_update_unique_job_id(fsu_job_num_scan, tempJobResourcePtr->JobId);
				}
#endif
				
                scan_bytes_per_row_padded = 1;
                first_pagestart_seen = 0;
                	
				fb_legal = FALSE;
					
				_scan_info.fname = NULL;
				// _scan_info.store = 0;
                _scan_info.color = 0;
				_scan_info.brightness = faxapp_get_Density();
				if(!faxapp_get_AutoDarkness())
				{
					_scan_info.background = 0;
				}
				else
				{
					_scan_info.background = gFaxBgRemove;
				}
				_scan_info.resolution = FAX_VR_300;
				_scan_info.active_page = 0;
			  
				isScanComplete = FALSE;
				//START_JOB(ejob_ScanFax, FAXER_SCAN_JOB_AGENT_ID, 0);         

				ASSERT( sv==NULL );

				text_photo_mode = faxapp_get_ImageType();
				if(text_photo_mode == 0)
				{ 
					printf("\n(YHCHO) %s() TEXT MODE\n", __FUNCTION__);
					sv = scanvar_text_fax_new();
				}
				else if(text_photo_mode == 1)
				{
					printf("\n(YHCHO) %s() PHOTO MODE\n", __FUNCTION__);
					sv = scanvar_mixed_fax_new();
				}

                _setup_scan(sv, &_scan_info); 
                if(sv == NULL)
                {
					faxapp_scan_cancel();
					break;
                }
                
                if(OurResource == NULL)
                {
					faxapp_scan_cancel();
					break;
				}
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
				if( faxapp_get_job_type() == FAX_APP_JOB_TYPE_FAX_SOLUTION )
				{
					scanner_job_setupJobManage( SCAN_SAVE_FILESYSTEM_TYPE_USE_RAMDISK );
				}
#endif
				scanman_set_scanpapersize(faxapp_get_originalSizeCode());

                // Send a message to the scanner to start the job.
                msg.msgType = MSG_SCAN_JOBSTART;
                msg.param2 = 0;
                msg.param3  = sv;
                SYMsgSend( OurResource->Source, &msg );

                break;
            }
                
            case MSG_NAKRECIPE:
            {
                dbg_printf("Got NAKRECEIPE! Abort scanning!\n");
                break;
            }
                
            case MSG_SCAN_SIZE:
            {
				if(scan_cancel_flag == TRUE)
				{
					printf("\n(YHCHO) %s() scan_cancel_flag: MSG_SCAN_SIZE\n", __FUNCTION__);
					break;
				}
                /* davep 22-Apr-2011 ; change to new scan size message */
                msg_scan_size_decode( &msg, &pixels_per_row_padded, &pixels_per_row, &total_rows, &bits_per_pixel );
                //printf("\n(YHCHO) %s() MSG_SCAN_SIZE\n", __FUNCTION__);
                printf("\n(YHCHO) %s() pixels_per_row_padded = %d, pixels_per_row = %d, total_rows = %d, bits_per_pixel = %d\n",
                 __FUNCTION__,pixels_per_row_padded,pixels_per_row,total_rows,bits_per_pixel);
                 
                m_300DPI_height = total_rows;
                if(A3_merge == TRUE)
                {
					margin_wid = (a3_300DPI_width - pixels_per_row_padded)/2;
				}
				else if(image_rotation) // if A5 SEF or B5 SEF
				{
					line_count[0] = line_count[1] = 0;
					m_300DPI_width = pixels_per_row_padded;
					if(image_rotation == 1) //A5 SEF
					{
						margin_wid = (2592 - total_rows)/2;
						m_300DPI_height = 2592;
					}
					else if(image_rotation == 2) //B5 SEF
					{
						margin_wid = (3072 - total_rows)/2;
						m_300DPI_height = 3072;
					}
				}
				else
				{
					m_300DPI_width = 2592;
					margin_wid = (m_300DPI_width - pixels_per_row_padded)/2;
				}

                /* poison the structure to catch someone using old fields */
                memset( &msg, 0xaa, sizeof(MESSAGE));
                scan_bytes_per_row_padded = pixels_per_row_padded>>3;
                
                break;
            }

            case MSG_SCAN_PLANEDATA:
            {
				if(scan_cancel_flag == TRUE)
				{
					printf("\n(YHCHO) %s() scan_cancel_flag: MSG_SCAN_PLANEDATA\n", __FUNCTION__);
					MEM_FREE_AND_NULL(planedata);
                    if( fax_duplex_pages[page_side].data_strips ) {
                        MEM_FREE_AND_NULL(fax_duplex_pages[page_side].data_strips);
                    }
					break;
				}

				//printf("\n(YHCHO) %s() MSG_SCAN_PLANEDATA\n", __FUNCTION__);
                msg_scan_planedata_decode( &msg, &num_rows, &dtype, &planedata, &last_buffer );                
                msg_scan_planedata_get_page_side( &msg, &page_side );
                
                //printf("\n(YHCHO) %s() page_side = %d, num_rows = %d\n", __FUNCTION__,page_side, num_rows);

                /* poison the structure to catch someone using old fields */
                memset( &msg, 0xaa, sizeof(MESSAGE));
                if(A3_merge == TRUE)
                {
					for(i=0;i<num_rows;i++)
					{
						memcpy(&fax_duplex_pages[page_side].data_strips[fax_duplex_pages[page_side].current_lines+margin_wid],&planedata[i*pixels_per_row_padded],pixels_per_row_padded);
						fax_duplex_pages[page_side].current_lines += a3_300DPI_width;
					}
				}
				else if(fb_legal == TRUE)
				{
					for(i=0;i<num_rows;i++)
					{
						memcpy(&fax_duplex_pages[page_side].data_strips[fax_duplex_pages[page_side].current_lines+margin_wid],&planedata[i*pixels_per_row_padded],pixels_per_row_padded);
						fax_duplex_pages[page_side].current_lines += m_300DPI_width;
					}					
				}
				else if(image_rotation) //if A5 SEF or B5 SEF. Need Rotation
				{
					//m_300DPI_height, m_300DPI_width, margin_wid,num_rows, fax_duplex_pages[page_side].data_strips, planedata, line_count
					//fax_rotate(uint8_t *pS, uint8_t *pD, int type_width, int strip_width, int strip_height, int margin)
					if(line_count[page_side] == 0)
					{
						line_count[page_side] = margin_wid;
					}
					
					if((line_count[page_side] - margin_wid)+ num_rows > total_rows)
					{
						num_rows = total_rows - (line_count[page_side] - margin_wid);
						if(num_rows == 0)
						{
							MEM_FREE_AND_NULL(planedata);
							break;
						}
					}
					//printf("\n(YHCHO) %s() page_side = %d, line_count = %d\n", __FUNCTION__,page_side, line_count[page_side] - margin_wid);
					fax_rotate(&planedata[0], &fax_duplex_pages[page_side].data_strips[0], m_300DPI_height, m_300DPI_width, num_rows, line_count[page_side]);
					line_count[page_side] += num_rows;
				}
				else
				{
					if((fax_duplex_pages[page_side].current_lines + num_rows*m_300DPI_width) <= m_300DPI_height*m_300DPI_width)
					{
						//memcpy(&data[total_count_rows],&planedata[0],num_rows*pixels_per_row);
						
						for(i=0;i<num_rows;i++)
						{
							memcpy(&fax_duplex_pages[page_side].data_strips[fax_duplex_pages[page_side].current_lines+margin_wid],&planedata[i*pixels_per_row_padded],pixels_per_row_padded);
							fax_duplex_pages[page_side].current_lines += m_300DPI_width;
						}
						
						//total_count_rows += num_rows*m_300DPI_width;		
						//printf("\n(YHCHO) %s() margin_wid = %d, pixels_per_row_padded = %d\n", __FUNCTION__,margin_wid, pixels_per_row_padded);
					}
					else if((fax_duplex_pages[page_side].current_lines+ num_rows*m_300DPI_width) > m_300DPI_height*m_300DPI_width)
					{
						temp_count = (m_300DPI_height*m_300DPI_width - fax_duplex_pages[page_side].current_lines)/m_300DPI_width;
						//memcpy(&data[total_count_rows],&planedata[0],temp_count*pixels_per_row);
						
						for(i=0;i<temp_count;i++)
						{
							memcpy(&fax_duplex_pages[page_side].data_strips[fax_duplex_pages[page_side].current_lines+margin_wid],&planedata[i*pixels_per_row_padded],pixels_per_row_padded);
							fax_duplex_pages[page_side].current_lines += m_300DPI_width;
						}					
						
						//total_count_rows += temp_count*pixels_per_row;		
						//printf("\n(YHCHO) %s() total_count_rows = %d\n", __FUNCTION__,fax_duplex_pages[page_side].current_lines/m_300DPI_width);
					}
				}
				
				/*
				 * Scan code allocated the buffer for us.  Now that we've dealt with it, we
				 * need to free it for them (they have no way to know we're done with it).
				 */
				MEM_FREE_AND_NULL(planedata);
                
                break;
            }

            case MSG_CANCELJOB:
            {
                printf("\n(YHCHO) %s() MSG_CANCELJOB\n", __FUNCTION__);
                if( fax_duplex_pages[page_side].data_strips ) {
                    MEM_FREE_AND_NULL(fax_duplex_pages[page_side].data_strips);
                }
				
				scan_cancel_flag = FALSE;

                ACK_CANCEL(msg.param3, FAXER_SCAN_JOB_AGENT_ID);
                
                isScanComplete = FALSE;

                fax_scan_page_count = _scan_info.pages_completed;

                de_activate_scan_to_mem();

                // Tell the scanner that we are all done.
                // Remember to set the cookie, i.e. the scanvars variable.
                msg.msgType = MSG_SCAN_JOBEND;
                msg.param3  = sv;
                if(OurResource == NULL)
                {
                    XASSERT(0,0);
                }
                SYMsgSend( OurResource->Source, &msg );

                END_JOB(OurResource, FAXER_SCAN_JOB_AGENT_ID);

                if(sv)
                {
                   scanvar_delete( &sv );
                   sv = NULL;
                }
                
                if(_scan_info.tiff_writer_handle)
                {
//                    fax_tiff_writer_close(_scan_info.tiff_writer_handle);
                    _scan_info.tiff_writer_handle = NULL;
                }

                first_pagestart_seen = 0;

                break;
            }

            case MSG_SCAN_PAGESTART:
            {
                if(scan_cancel_flag == TRUE)
                {
                    printf("\n(YHCHO) %s() scan_cancel_flag: MSG_SCAN_PAGESTART\n", __FUNCTION__);
                    break;
                }

                //printf("\n(YHCHO) %s() MSG_SCAN_PAGESTART - _scan_info.pages_completed: %d\n", __FUNCTION__, _scan_info.pages_completed);
                if (_scan_info.pages_completed >= 1)
                {
                    if (fsu_get_memory_full_status_nand() == 1)
                    {
                        printf("\n(YHCHO) %s() - NAND Full...\n", __FUNCTION__);
                        faxapp_scan_cancel();

                        break;
                    }

                    if (fsu_get_memory_full_status_ram() == 1)
                    {
                        printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);
                        faxapp_scan_cancel();

                        break;
                    }
                }
				
                isScanComplete = FALSE;

                msg_scan_page_start_decode( &msg, &page_side );
                //printf("\n(YHCHO) %s() MSG_SCAN_PAGESTART page_side = %d\n", __FUNCTION__,page_side);

                line_count[page_side] = 0;

                if( fax_duplex_pages[page_side].data_strips ) {
                    printf("FAX: Not freed(%p)\n", fax_duplex_pages[page_side].data_strips);
                    MEM_FREE_AND_NULL(fax_duplex_pages[page_side].data_strips);
                }
                if(A3_merge == TRUE)
                {
                    fax_duplex_pages[page_side].data_strips = (uint8_t *)MEM_MALLOC(a3_300DPI_width*total_rows);
                    if(fax_duplex_pages[page_side].data_strips == NULL)
                    {
                        XASSERT(0,0);
                    }
                    else
                    {
                        memset(fax_duplex_pages[page_side].data_strips,255,a3_300DPI_width*total_rows);
                        fax_duplex_pages[page_side].current_lines = 0;
                    }
                }
                else
                {
                    fax_duplex_pages[page_side].data_strips = (uint8_t *)MEM_MALLOC(m_300DPI_width*m_300DPI_height);
                    if(fax_duplex_pages[page_side].data_strips == NULL)
                    {
                        XASSERT(0,0);
                    }
                    else
                    {
                        //printf("\n(YHCHO) %s() data malloc & set 0\n", __FUNCTION__);
                        memset(fax_duplex_pages[page_side].data_strips,255,m_300DPI_width*m_300DPI_height);
                        fax_duplex_pages[page_side].current_lines = 0;
                    }
                }
                _scan_info.active_page++;

                break;
            }

            case MSG_SCAN_PAGEEND:
            {
                int real_width = 0;
                int real_height = 0;
				
                if(scan_cancel_flag == TRUE)
                {
                    printf("\n(YHCHO) %s() scan_cancel_flag: MSG_SCAN_PAGEEND\n", __FUNCTION__);
                    if( fax_duplex_pages[page_side].data_strips ) {
                        MEM_FREE_AND_NULL(fax_duplex_pages[page_side].data_strips);
                    }
                    break;
                }

                UINT16 OriginalDocId = fsu_get_original_doc_id(fsu_job_num_scan);

                msg_scan_page_end_decode( &msg, &page_side );
                XASSERT(page_side < 2, page_side);
                
                //printf("\n(YHCHO) %s() MSG_SCAN_PAGEEND page_side = %d\n", __FUNCTION__,page_side);
            
                _scan_info.pages_completed++;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
#if __HAVE_FAXSOLUTION_DK__
#endif
				if(!isDKSolution)
				{
					if( faxapp_get_job_type() == FAX_APP_JOB_TYPE_FAX_SOLUTION ) {
						uint8_t *scan_page_data;
						unsigned long malloced_pageBuf_size = 0;
						unsigned int malloced_pageBuf_WidPixel = 0;
						unsigned int malloced_pageBuf_HeiPixel = 0;			
						unsigned int pdf_width = 0;
						unsigned int pdf_height = 0;
							
						if(A3_merge == TRUE) {
							malloced_pageBuf_size = a3_300DPI_width * total_rows;
							malloced_pageBuf_WidPixel = a3_300DPI_width;
							malloced_pageBuf_HeiPixel = total_rows;
						}
						else if(image_rotation)
						{
							malloced_pageBuf_size = m_300DPI_width * m_300DPI_height;
							malloced_pageBuf_WidPixel = m_300DPI_height;
							malloced_pageBuf_HeiPixel = m_300DPI_width;
						}
						else {
							malloced_pageBuf_size = m_300DPI_width * m_300DPI_height;	//bpp:8 (if depended user setting, have to change)
							malloced_pageBuf_WidPixel = m_300DPI_width;
							malloced_pageBuf_HeiPixel = m_300DPI_height;
						}
								
						scan_page_data = (uint8_t *)MEM_MALLOC(malloced_pageBuf_size);
						memset(scan_page_data, 0xFF, malloced_pageBuf_size);
						memcpy(scan_page_data, fax_duplex_pages[page_side].data_strips, malloced_pageBuf_size);
											
						pdf_width = malloced_pageBuf_WidPixel/CONVERT_300DPI;
						pdf_height = malloced_pageBuf_HeiPixel/CONVERT_300DPI;

						if(scan_page_data != NULL) {
							SCAN_PDF_Start( 	scan_page_data, 
												malloced_pageBuf_size,
												malloced_pageBuf_WidPixel, 
												malloced_pageBuf_HeiPixel, 
												pdf_width, 
												pdf_height, 
												(int)_scan_info.pages_completed, 
												1/*IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM*/, 
												3/*IMAGELOG_APPLICATION_TYPE_OF_SCAN_FAX_SCAN*/
											);
						}
						else {
							printf("\n(Fax Scan) scan_page_data PageMem is NULL\n");
						}
						
						MEM_FREE_AND_NULL(scan_page_data);
					}
				}
#endif

#ifdef HAVE_IMAGELOG
                //if image log is turn-on we save each page memory into PDF file
                printf("\n(YHCHO) %s() Image Log !!\n", __FUNCTION__);
                if(IsImageLogEnabled('F', IMAGELOG_FAX_SUBJOB_TYPE_TX) == true) {
                    uint8_t *imagelog_page_data;
                    unsigned long malloced_pageBuf_size = 0;
                    unsigned int malloced_pageBuf_WidPixel = 0;
                    unsigned int malloced_pageBuf_HeiPixel = 0;
                    unsigned int pdf_width = 0;
                    unsigned int pdf_height = 0;

                    if(A3_merge == TRUE) {
                        malloced_pageBuf_size = a3_300DPI_width * total_rows;
                        malloced_pageBuf_WidPixel = a3_300DPI_width;
                        malloced_pageBuf_HeiPixel = total_rows;
                    }
                    else if(image_rotation)
                    {
                        malloced_pageBuf_size = m_300DPI_width * m_300DPI_height;
                        malloced_pageBuf_WidPixel = m_300DPI_height;
                        malloced_pageBuf_HeiPixel = m_300DPI_width;
					  }
                    else {
                        malloced_pageBuf_size = m_300DPI_width * m_300DPI_height;	//bpp:8 (if depended user setting, have to change)
                        malloced_pageBuf_WidPixel = m_300DPI_width;
                        malloced_pageBuf_HeiPixel = m_300DPI_height;
                    }

                    imagelog_page_data = (uint8_t *)MEM_MALLOC(malloced_pageBuf_size);
                    memset(imagelog_page_data, 0xFF, malloced_pageBuf_size);
                    memcpy(imagelog_page_data, fax_duplex_pages[page_side].data_strips, malloced_pageBuf_size);

                    pdf_width = malloced_pageBuf_WidPixel/CONVERT_300DPI;
                    pdf_height = malloced_pageBuf_HeiPixel/CONVERT_300DPI;
                    
                    fsu_job_num_scan_image_log = fsu_job_num_scan;

                    if(imagelog_page_data != NULL) {
                        SCAN_IL_PDF_Start( imagelog_page_data, malloced_pageBuf_size,
                                                malloced_pageBuf_WidPixel, malloced_pageBuf_HeiPixel,
                                                pdf_width, pdf_height,
                                                (int)_scan_info.pages_completed,
                                                IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM,
                                                IMAGELOG_APPLICATION_TYPE_OF_SCAN_FAX_SCAN
                                            );
                    }
                    else {
                        printf("\n(Fax Scan) ImageLog Turn-On. But PageMem is NULL\n");
                    }

                    MEM_FREE_AND_NULL(imagelog_page_data);
                }
#endif

                if (fsu_get_flag_manual_tx(fsu_job_num_scan) == 1)
                {
                    fsu_update_total_pages(fsu_job_num_scan, _scan_info.pages_completed);
                }

                //Finish current page                

                if (_scan_info.pages_completed >= 2)
                {
                    if (gFaxScanSource == FAX_SCAN_SOURCE_ADF)
                    {
						 if (fsu_get_broadcast(fsu_job_num_scan) == 1)
						{
							faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_FROM_ADF, _scan_info.pages_completed, 0, 0, 0, gFaxBroadcastJobInfo[OriginalDocId][0]);
						}
						else
						{
							faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_FROM_ADF, _scan_info.pages_completed, 0, 0, 0, fsu_job_num_scan);
						}
                    }
                }

                if(A3_merge == TRUE)
                {
                    real_width = a3_300DPI_width;
                    real_height = total_rows;
                }
                else if(image_rotation)
                {
                    real_width = m_300DPI_height;
                    real_height = m_300DPI_width;
                }
                else
                {
                    real_width = m_300DPI_width;
                    real_height = m_300DPI_height;
                }

                if (fsu_get_broadcast(fsu_job_num_scan) == 1)
                {
                    MakeRawToPbm(_scan_info.pages_completed, fax_duplex_pages[page_side].data_strips, real_width, real_height, gFaxBroadcastJobInfo[OriginalDocId][0], REQUEST_SOURCE_FAX);
                }
                else
                {
                    MakeRawToPbm(_scan_info.pages_completed, fax_duplex_pages[page_side].data_strips, real_width, real_height, fsu_job_num_scan, REQUEST_SOURCE_FAX);
                }

                fax_duplex_pages[page_side].current_lines = 0;

                isScanComplete = TRUE;

              if( fax_duplex_pages[page_side].data_strips ) {
                MEM_FREE_AND_NULL(fax_duplex_pages[page_side].data_strips);
              }
                //printf("\n(YHCHO) %s() MSG_SCAN_PAGEEND ok\n",__FUNCTION__);

                break;
            }

            case MSG_SCAN_JOBEND:
            {
				if(scan_cancel_flag == TRUE)
				{
					printf("\n(YHCHO) %s() scan_cancel_flag: MSG_SCAN_JOBEND\n", __FUNCTION__);
					if( fax_duplex_pages[page_side].data_strips ) {
                        MEM_FREE_AND_NULL(fax_duplex_pages[page_side].data_strips);
                    }
					break;
				}

				scan_cancel_flag = FALSE;

                //printf("\n(YHCHO) %s() MSG_SCAN_JOBEND\n", __FUNCTION__);

                fax_scan_page_count = _scan_info.pages_completed;

                if (gFaxScanSource == FAX_SCAN_SOURCE_FB && fb_legal != TRUE)
                {
                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_NEXT_DOCUMENT_WAIT, 0, 0, 0, 0, fsu_job_num_scan);
                }
                else
                {
                    de_activate_scan_to_mem();

                    msg.param3  = NULL;

#ifdef HAVE_SCAN_SUPPORT
                    // Tell the scanner that we are all done.
                    // Remember to set the cookie, i.e. the scanvars variable.
                    msg.param2  = FAXER_SCAN_JOB_AGENT_ID;
                    msg.param3  = OurResource;

                    if(OurResource == NULL)
                    {
                        XASSERT(0,0);
                    }

                    SYMsgSend( OurResource->Source, &msg );
#endif

                    //End of scan
                    END_JOB(OurResource, FAXER_SCAN_JOB_AGENT_ID);

#ifdef HAVE_SCAN_SUPPORT
                    scanvar_delete( &sv );
#endif
                    sv = NULL;
                }

                break;
            }

			case MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START :
			{
				if (fb_legal)
				{
					printf("\n(YHCHO) %s() FB scan for Legal scan\n",__FUNCTION__);
					faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_REQUEST_FOR_LEGAL_SIZE_MERGE, 0, 0, 0, 0, fsu_job_num_scan);
				}
				break;            
			}
			
			case MSG_USER_INPUT:
			{
				int is_scan_done = 0;
				
				printf("\n(YHCHO) %s() MSG_USER_INPUT: %ld\n",__FUNCTION__, msg.param1);
				
				if (fb_legal)
				{
					printf("\n(YHCHO) %s() FB scan for Legal scan Second page\n",__FUNCTION__);
					scan_bytes_per_row_padded = 1;
					first_pagestart_seen = 0;

					_scan_info.fname = NULL;
					_scan_info.color = 0;
					_scan_info.brightness = faxapp_get_Density();
					if(faxapp_get_AutoDarkness())
					{
						_scan_info.background = 0;
					}
					else
					{
						_scan_info.background = gFaxBgRemove;
					}
					_scan_info.resolution = FAX_VR_300;
					_scan_info.active_page = 0;
				  
					isScanComplete = FALSE;   

					_setup_scan(sv, &_scan_info); 
					if(sv == NULL)
					{
					  faxapp_scan_cancel();
					  break;
					}
					
					if(OurResource == NULL)
					{
						faxapp_scan_cancel();
						break;
					}
					
					faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_FROM_FB, FSUCurrentFBScanPageNo, 0, 0, 0, fsu_job_num_scan);

					msg.msgType = MSG_SCAN_JOBSTART;
					msg.param2 = 0;
					msg.param3  = sv;
					SYMsgSend( OurResource->Source, &msg );					
				}
				else if(msg.param1 == FB_MULTI_PAGE_SCAN_DONE)
				{
					is_scan_done = 1;
				}
				else if(msg.param1 == FB_MULTI_PAGE_SCAN_GO)
				{
					faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_FROM_FB, fax_scan_page_count, 0, 0, 0, fsu_job_num_scan);
					
					// Send a message to the scanner to start the job.
					msg.msgType = MSG_SCAN_JOBSTART;
					msg.param2 = 0;
					msg.param3  = sv;
					SYMsgSend( OurResource->Source, &msg );
				}
				else if(msg.param1 == FB_MULTI_PAGE_SCAN_CANCELED)
				{
					is_scan_done = 1;
					gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_SCAN_CANCEL;
					isScanComplete = FALSE;
				}
				
				if(is_scan_done == 1)
				{
					clearFaxScanPageCount(); // set fax scan completed page count 0.
					
					de_activate_scan_to_mem();

#ifdef HAVE_SCAN_SUPPORT
					// Tell the scanner that we are all done.
					// Remember to set the cookie, i.e. the scanvars variable.
					msg.msgType = MSG_SCAN_JOBEND;
					msg.param3  = sv;

					if(OurResource == NULL)
					{
						XASSERT(0,0);
					}

					SYMsgSend( OurResource->Source, &msg );
#endif

					//End of scan
					END_JOB(OurResource, FAXER_SCAN_JOB_AGENT_ID);

#ifdef HAVE_SCAN_SUPPORT
					scanvar_delete( &sv );
#endif
					sv = NULL;
				}
				
				break;  				
			}
			
			case FAXAPP_MSG_SCAN_JOB_INFO:
			{
				printf("\n(YHCHO) %s() FAXAPP_MSG_SCAN_JOB_INFO\n", __FUNCTION__);
				stIC_JobDoneInfo stFaxScanInfo;
				memcpy(&stFaxScanInfo, msg.param3, sizeof(stIC_JobDoneInfo));
				printf("JobID:%ld UserID=%s DocType:%d PagesPrinted:%d,paperSize:%d,duplex:%d\n",
				stFaxScanInfo.jobNum, stFaxScanInfo.userID, stFaxScanInfo.jobDocType, stFaxScanInfo.totalPages
				,stFaxScanInfo.paperSize,stFaxScanInfo.duplex);

				fsu_update_faxScanInfo(fsu_job_num_scan, &stFaxScanInfo);

				MEM_FREE_AND_NULL(msg.param3);
				break;
			}
			
            default:
            {
                dbg_printf("Unknown msg! type=%d\n", msg.msgType);

                break;
            }
        } // switch(msg.msgType)
    } // while(1)

    return 0;
}

