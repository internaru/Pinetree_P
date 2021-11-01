//
// Include Header Files
//
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/vfs.h>
#include <sys/sysinfo.h>
#include "posix_ostools.h"
#include "agRouter.h"
#include "ATypes.h"
#include "list.h"
#include "memAPI.h"
#include "lassert.h"
#include "agMessage.h"
#include "agConnectMgr.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "regAddrs.h"
#include "agJobUtils.h"
#include "ResourceMap.h"
#include "utils.h"
#include "agprint.h"
#include "hwconfig_api.h"
#include "debug.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "data_access_api.h"
#include "error_types.h"
#include "BigBuffer.h"
#include "event_observer.h"
#include "scanapp_observers_api.h"
#include "response_subject.h"
#include "cmd_proc_api.h"
#include "scantypes.h"
#include "scanvars.h"
#include "scands.h"
#include "scanhwerr.h"
#include "scanman.h"
#include "scanplat.h"
#include "scancap.h"
#include "scanmsg.h"
#include "print_job_api.h"
#include "PrintJobInfo.h"
#include "printvars_api.h"
#include "data_access_api.h"
#include "dma_buffer.h"
#include "assert.h"
#include "scanapp.h"
#include "scanapp_uiscan.h"
#include "scanapp_dataprocess.h"
#include "platform.h"
#include "scancore.h"
#include "scos.h"
#include "scandbg.h"
#include "scantools.h"
#include "piehw.h"
#include "scanhwerr.h"
#include "scanmech.h"
#include "scanmech_app.h"
#include "asp.h"
#include "pic.h"
#include "fraction.h"
#include "adfsensor.h"
#include "outfile.h"
#include "logger.h"
#include "../../../pinetree/internal_page/libs/tiffio.h"
#include "db_api.h"
#include "a3mergeInterface.h"
#include "solution.h"
#include "SysUI_Interface.h"
#include "scanapp_margins.h"
#include "scanapp_imagelog.h"
#include "scanapp_save_file.h"
#include "SysCardReaderMsg.h"
#include "jpeglib.h"
#include "make2pdf.h"

#define USE_SCAN_DUMP	0

#if USE_SCAN_DUMP
int  dump_no = 0;
#endif

/**
 * \brief Regarding Scan Margin
 * 		- g_adf_margins
 * 		- g_flatbed_margins
 * 
 * 		1inch == 25.4mm, 0.1inch == 2.54mm, 0.01inch == 0.254mm
 */

/**
 * \brief UI Scan Application Flow 
 * 
 * 		1.MSG_SETTING_CHANGED
 * 		2.MSG_STARTRECIPE 
 * 		3.MSG_RESOURCES 
 * 		4.MSG_ACKRECIPE
 * 		5.MSG_SCAN_SIZE 
 *		6.MSG_SCAN_PAGESTART
 * 		7.MSG_SCAN_PAGEEND
 */


/**
 * \brief Making The Scan Result File : JPEG, TIF, PDF
 * 
 * -----------------------------------------------------------------------------------
 * #1. Making JPEG (using libjpeg) described in <libjpeg.doc>
 * -----------------------------------------------------------------------------------
 *  - Step1. Allocate and initialize a JPEG compression object.
 *      		struct jpeg_compress_struct cinfo;
				struct jpeg_error_mgr jerr;
				...
				cinfo.err = jpeg_std_error(&jerr);
				jpeg_create_compress(&cinfo);
 *  - Step2. Specify the destination for the compressed data (eg, a file).
				FILE * outfile;
				...
				if ((outfile = fopen(filename, "wb")) == NULL) {
                fprintf(stderr, "can't open %s\n", filename);
                exit(1);
				}
            jpeg_stdio_dest(&cinfo, outfile);
 *  - 3. Set parameters for compression, including image size & colorspace.
            image_width		Width of image, in pixels
            image_height		Height of image, in pixels
            input_components	Number of color channels (samples per pixel)
            in_color_space		Color space of source image

            Typical code for a 24-bit RGB source image is

                cinfo.image_width = Width;      // image width and height, in pixels
                cinfo.image_height = Height;
                cinfo.input_components = 3;     // # of color components per pixel
                cinfo.in_color_space = JCS_RGB; // colorspace of input image

                jpeg_set_defaults(&cinfo);
			
			-Now you can set any non-default parameters you wish to
			 Here we just illustrate the use of quality (quantization table) scaling:
				jpeg_set_quality(&cinfo, quality, TRUE); //limit to baseline-JPEG values
 *  - 4. jpeg_start_compress(...);
            jpeg_start_compress(&cinfo, TRUE);
 *  - 5. while (scan lines remain to be written)
            jpeg_write_scanlines(...);
 *  - 6. Finish compression 
			jpeg_finish_compress(...);
 *	- 7. After finish_compress, we can close the output file.
			fclose(outfile);
 *  - 8. Release the JPEG compression object.
            jpeg_destroy_compress(&cinfo);

 * ---------------------------------------------------------------
 * #2. Making TIFF (using libtiff)
 * ---------------------------------------------------------------
    JSAMPLE         -  1채널.            RGB??각각?�소. 기본값�? 8
    JSAMPROW        -  JSAMPLE*         RGB?�선 ?�나???�소.
                                        GRAY?�서???�나�??�당?�고 RGB?� 비슷?�게 ?�동?�듯 ?��?�?
                                        ?�제 ?�인?� ?�해�?--;
    JSAMPARRAY      - JSAMPROW*         ?�소??배열. ?��?지 ?�줄. scanline
    JSAMPIMAGE      - JSAMPARRAY*       ?��?지 ?�체. ?��???�?채널???�누?�져 ?�으??3차배??

 * ---------------------------------------------------------------
 * #3. Making PDF (using haru-pdf lib)
 * ---------------------------------------------------------------
	* http://libharu.sourceforge.net/document_handling.html
	* 	 
	* Basic Concept : Marvell SDK support only raw image data (not support jpeg process)
	* So, we use 'LIB_JPEG_TURBO' for making jpeg image.
	* For PDF making, we use below processing.
	* 	1. Although user setup the PDF file type, scan application work like scan to JPEG.
	* 	2. After making JPEG file finished, transfer that JPEG file into PDF documnet resource by using haru-pdf lib.
	* 	3. All page scan process finished, save that PDF documnet into PDF file.
	* 	4. Until now, PDF making PDF file regarded just One PDF file (multi-page).
	* 	5. For single-page PDF File we have to insert another mode.
*/


/**
 * \brief Send Function (API with Solution App)
 * 
 * [ Scan App -> Solution App ]
 * 
 * 		typedef struct stSolution_Response_ {
 * 			UINT32			sendRequestAppMsgQueueId;
 * 			UINT32			sendJobId;
 * 			void*			reserved;
 * 		}stSolution_Request;
 * 
 * [ Scan App <- Solution App ] 
 * 		MESSAGE.msgType = MSG_SENDJOB_RESULT;
 * 		MESSAGE.param1 = jobId;
 * 		MESSAGE.param2 = errorCode;
 * 
 * 
 * 1) ?�메??
 * 		int sendMailWrapper(const char *username, const char *password, const char *sender,
 *                  const char *mailhost, int port, const char *attachmentPath, const char *toAddress,
 *                  const char *subject, const char *content, int secureType, int authType, int isDebugMode,
 *                  stSolution_Request *email_mqueue_data)
 * 			- ?�용 ?�시 
 * 				sendMailWrapper(SMTP?�버ID, SMTP?�호, 보내?�사?�주?? SMTP?�버주소, SMTP?�버?�트, 첨�??�일경로, 
 * 										받는?�람주소, 메일?�목, 메일본문, 1, 1, 0, email_mqueue_data);
 * 
 * 2) FTP
 * 		int ftpUploadWrapper(const char *username, const char *password, const char *destination, int port, 
 * 						const char *filePath, int serverType, const char *proxyserver, int isDebugMode, stSolution_Request ftp_mqueue_data)
 * 			- ?�용 ?�시  
 * 				ftpUploadWrapper(FTP?�버ID, FTP?�호, FTP?�버주소, FTP?�버?�트, 첨�??�일경로, 0, NULL, 0, ftp_mqueue_data);
 * 
 * 3) SMB
 * 		int smbUpload(const char *username, const char *password, const char *smbshare, const char *remotePath, 
 * 					const char *localPath, int isDebug, stSolution_Request *smb_mqueue_data);
 * 			- ?�용 ?�시 
 * 				smbUpload(SMB?�버ID, SMB?�호, 보내?�사?�주?? SMB?�버주소, SMB?�버 ?�일경로, 첨�??�일경로, 0, smb_mqueue_data);
 * 
 */

int scanner_tiff_write_scanline(TIFF *tifFilePoint, uint8_t *buf, uint16_t writeStart_rowNumber, uint16_t rows_count, uint8_t page_side);
int scanner_jpeg_write_scanline(uint8_t page_side, uint8_t *buf, uint16_t rows_count);

/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "SCN UI: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
///Using Method.
// DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
// DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

///For testing the RAM_DISK, use this : "mkdir ram_disk" -> "mount -t tmpfs -o size=100m tmpfs ./ram_disk" (100M)
#define RAMDISK_PATH	"/tmp" ///Do not include '/' at last, this path is meaning of RAM_DISK (setup automatically)
#define TEMP_FOLDER_PATH	"/tmp/ui_scan/tmp_jpg"
static char gUI_usbThumbdriveRootPath[MAX_SCAN_FILE_FULL_PATH] = "/mnt/sda1";
static char gUI_ramDiskScanBasicPath[MAX_SCAN_FILE_FULL_PATH] = "/tmp/ui_scan";	//created when initialize
static char gUI_sdCardRootPath[MAX_SCAN_FILE_FULL_PATH] = "/mnt/sd";
static char gUI_Image_Folder[SDH_FILEPATH_MAX];	//current scanning 'scanned image folder'

#define STATE_TRANSITION(new_state)  (state_transition( &State, (new_state), __LINE__ ))
static void state_transition( UI_SCAN_APP_STATE *curr_state, 
                              UI_SCAN_APP_STATE new_state, 
                              int line_number )
{
    DBG_PRINTF_SDH_DEBUG( "scanapp state from %d to %d at %d\n", *curr_state, new_state,line_number );
    *curr_state = new_state;
}

ALIGN(8)
static uint8_t UiScanAppStack[SCANAPP_UISCAN_STACK_SIZE];
static mqd_t uiscanapp_msgq;
static pthread_t uiscanapp_thd_id;

static UI_SCAN_APP_STATE State;				///< State variable.
static struct uiscan_members uiscan_self;
static struct uiscan_stats_t uiscan_stats; 
static UISCAN_JOB_SETTINGS uiJobSettings;		///< The ui scan job settings.
static UISCAN_JOB_MANAGER currentUiScanJobManage;
static scanOnePageBuf_t	scanOnePageImageBuf[UI_SCAN_DUAL_PAGE_NUM];

#ifdef HAVE_IMAGELOG
static bool g_nIsEnableScanImageLog = 0;		 ///<Image Log 0: Off 1: On
static scanOnePageBuf_t	scanOnePageMemImageLog;
#endif

static struct scanvars *g_scanvars;
CURRENT_RESOURCE *job_resources;
static bool g_scan_started_from_adf;
static struct scan_platform_capabilities scan_capabilities;
struct output_file_list outfile_list;

/* Adding page sides for dual scan */
static int page_number = 0;

/*
 **********************************************************************************************
 * ///Regarding Making TIFF File
 **********************************************************************************************
 */
/* we set the two tifFile pointer for dual scan
 * When dual scan processing, front page and back page job is mixed
 * So, we have to set up two tif file descripter.
*/
static TIFF *tifFile = NULL;
static TIFF *tifFile_backside = NULL;
static uint16_t tifWriteScanLineRowIdx = 0;
static uint16_t tifWriteScanLineRowIdx_backside = 0;
//static uint16_t tifMultiPageWritePageNum = 0;


/*
 **********************************************************************************************
 * ///Regarding Making JPEG File
 **********************************************************************************************
 */
/* we set the two jpeg compressure for dual scan
 * When dual scan processing, front page and back page job is mixed
 * So, we have to set up two jpeg compressure and open 2 file.
*/
FILE *jpegFile;
static struct jpeg_compress_struct cinfo;
static struct jpeg_compress_struct cinfo_backside;
static struct jpeg_error_mgr jerr;
static struct jpeg_error_mgr jerr_backside;
static bool bjpeg_write_scanline[2];

#define PDF_MIN_MEMFREE_SIZE	(100*1024*1024)
#define PDF_MAX_FILE_SIZE		(1900*1024*1024)
int get_free_heap_mem(void);

/*
 **********************************************************************************************
 * ///Regarding Making PDF File
 **********************************************************************************************
 */
//static PSPDF_DOC gtSPDF_Doc_Front = NULL;
//static PSPDF_DOC gtSPDF_Doc_Back = NULL;
#define PAGE_SIDE_FRONT	0
#define PAGE_SIDE_BACK	1
static PSPDF_DOC gtSPDF_Doc[2] = {0,};
static bool pdf_documnet_started = false;
static char pdfFileNameForFrontSide[FILENAME_MAX];
static char pdfFileNameForBackSide[FILENAME_MAX];
static	char jpgFileNameForFrontSide[FILENAME_MAX];
static	char jpgFileNameForBackSide[FILENAME_MAX];

#ifdef HAVE_IMAGELOG
/**
 * \brief scanner request image log start 
 *
 */
int scanner_imagelog_start(uint8_t page_side)
{
//	if(uiJobSettings.fileFormat == UI_FILE_TIFF ) {
		//Not Support (have to add)
//		return 1;
//	}
	UINT32 pdfWidth = (uiJobSettings.scan_window.right * 72) / 100;
	UINT32 pdfHeight = (uiJobSettings.scan_window.bottom  * 72) / 100;

	UINT32	pageBuffer_PixelWidth	= uiscan_self.pixels_per_row_padded;
	UINT32	pageBuffer_PixelHeight	= (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100;
	
	UINT32 scan_data_type = 0;
	
	switch(uiJobSettings.fileFormat) {
		case UI_FILE_TIFF:
		case UI_FILE_TIFF_MULTI_PAGE:
			{
				if( uiJobSettings.ScanType == e_ColorScan ) {
					//Color Mode
					scan_data_type = IMAGELOG_ORIGNAL_SOUCE_RAW_24BPP_MEM;
					SCAN_IL_PDF_Start( scanOnePageImageBuf[page_side].buf, scanOnePageImageBuf[page_side].bufsize_bytes,
										pageBuffer_PixelWidth, pageBuffer_PixelHeight, pdfWidth, pdfHeight, page_number, scan_data_type, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN );	
				}
				else if( uiJobSettings.ScanType == e_MonochromeScan ) {
					//Gray Mode (8bit)
					scan_data_type = IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM;
				
					if (( uiJobSettings.ScanType == e_MonochromeScan) && ((uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL) || (uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION)))
					{
						SCAN_IL_PDF_Start( scanOnePageMemImageLog.buf, scanOnePageMemImageLog.bufsize_bytes,
										pageBuffer_PixelWidth, pageBuffer_PixelHeight, pdfWidth, pdfHeight, page_number, scan_data_type, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN );
					}
					else
					{
						SCAN_IL_PDF_Start( scanOnePageImageBuf[page_side].buf, scanOnePageImageBuf[page_side].bufsize_bytes,
										pageBuffer_PixelWidth, pageBuffer_PixelHeight, pdfWidth, pdfHeight, page_number, scan_data_type, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN );
					}
				}
			}
			break;
		case UI_FILE_JPEG:
			{
				if(page_side == 1) {				
					SCAN_IL_PDF_Start_JPEG_file(jpgFileNameForBackSide, pdfWidth, pdfHeight, page_number);				
				}
				else {
					SCAN_IL_PDF_Start_JPEG_file(jpgFileNameForFrontSide, pdfWidth, pdfHeight, page_number);
				}
			}
			break;
		case UI_FILE_PDF_MULTI_PAGE:
			{
				if(page_side == 1) {			
					DBG_PRINTF_SDH_DEBUG("[RYU] IL Back %d, %s\n", page_number, jpgFileNameForBackSide);
					if (( uiJobSettings.ScanType == e_MonochromeScan) && ((uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL) || (uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION)))
					{
						scan_data_type = IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM;
						SCAN_IL_PDF_Start( scanOnePageMemImageLog.buf, scanOnePageMemImageLog.bufsize_bytes,
										pageBuffer_PixelWidth, pageBuffer_PixelHeight, pdfWidth, pdfHeight, page_number, scan_data_type, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN );
					}
					else
					{
						SCAN_IL_PDF_Start_JPEG_file(jpgFileNameForBackSide, pdfWidth, pdfHeight, page_number);
						scanner_erase_image_file(jpgFileNameForBackSide);					
					}
				}
				else {
					DBG_PRINTF_SDH_DEBUG("[RYU] IL Front %d, %s\n", page_number, jpgFileNameForFrontSide);
					if (( uiJobSettings.ScanType == e_MonochromeScan) && ((uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL) || (uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION)))
					{
						scan_data_type = IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM;
						SCAN_IL_PDF_Start( scanOnePageMemImageLog.buf, scanOnePageMemImageLog.bufsize_bytes,
										pageBuffer_PixelWidth, pageBuffer_PixelHeight, pdfWidth, pdfHeight, page_number, scan_data_type, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN );
					}
					else
					{
						SCAN_IL_PDF_Start_JPEG_file(jpgFileNameForFrontSide, pdfWidth, pdfHeight, page_number);
						scanner_erase_image_file(jpgFileNameForFrontSide);
					}
				}
			}
			break;
		case UI_FILE_PDF:
			{
				if(page_side == 1) {
					if (( uiJobSettings.ScanType == e_MonochromeScan) && ((uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL) || (uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION)))
					{
						scan_data_type = IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM;
						SCAN_IL_PDF_Start( scanOnePageMemImageLog.buf, scanOnePageMemImageLog.bufsize_bytes,
										pageBuffer_PixelWidth, pageBuffer_PixelHeight, pdfWidth, pdfHeight, page_number, scan_data_type, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN );
					}
					else
					{
						SCAN_IL_PDF_Start_PDF_file(pdfFileNameForBackSide, pdfWidth, pdfHeight, page_number);		
					}
				}
				else {
					if (( uiJobSettings.ScanType == e_MonochromeScan) && ((uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL) || (uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION)))
					{
						scan_data_type = IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM;
						SCAN_IL_PDF_Start( scanOnePageMemImageLog.buf, scanOnePageMemImageLog.bufsize_bytes,
										pageBuffer_PixelWidth, pageBuffer_PixelHeight, pdfWidth, pdfHeight, page_number, scan_data_type, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN );
					}
					else
					{
						SCAN_IL_PDF_Start_PDF_file(pdfFileNameForFrontSide, pdfWidth, pdfHeight, page_number);										
					}
				}
			}
			break;
		default:
			break;
	}

	if( (uiJobSettings.ScanType == e_MonochromeScan) && 
		(uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL || uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION) ) 
	{
		if(scanOnePageMemImageLog.buf != NULL)
		{
			MEM_FREE_AND_NULL(scanOnePageMemImageLog.buf);
			scanOnePageMemImageLog.write_index = 0;
			scanOnePageMemImageLog.bufsize_bytes = 0;
		}
	}
	return 0;
}
#endif

/**
 * \brief Request Scanner Pause 
 *
 */
static scan_err_t scanner_adf_pause_request(void)
{
	scan_err_t scerr = SCANERR_NONE;

	DBG_PRINTF_SDH_DEBUG("[S_CANCEL] step1. pause!! UI ScanApp\n");
	scerr = scanmech_adf_client_request_pause();

	if( scerr == SCANERR_NONE ) {
		DBG_PRINTF_SDH_DEBUG("SCANMECH_IOCTL_ADF_CLIENT_PAUSE : Success\n");
	}
	else {
		DBG_PRINTF_SDH_DEBUG("SCANMECH_IOCTL_ADF_CLIENT_PAUSE : FAIL : %d\n", scerr);						
	}
	
	return scerr;
}


/**
 * \brief Request Job cancel 
 *
 */
static void scanner_job_cancel_request(void)
{
    MESSAGE msg;
    error_type_t err;
	
	//Request pause the ADF
	if( g_scan_started_from_adf ) {
		scanner_adf_pause_request();
	}
	
	msg.msgType = MSG_CANCELJOB;
	msg.param1 = SYS_REQUEST;
	msg.param3 = (void*) e_ScanToEmailApp;
	err = SYMsgSend( SJMID, &msg );
	XASSERT( err==OK, err );
	
	STATE_TRANSITION( UI_SCAN_APP_CANCEL );
}

/**
 * \brief Setup a start page for the downstream (scanner) resource.
 *
 */
static void scanner_job_start( struct scanvars *sv, CURRENT_RESOURCE *job_resources )
{
    MESSAGE msg;
    error_type_t err;

    DBG_PRINTF_SDH_DEBUG(  "%s job_resources=%p src=%d\n", __FUNCTION__, job_resources, job_resources->Source );

    /* send the startjob to my upstream; we pass control of the scanvar to the
     * scanner itself
     */
	scanman_set_scanpapersize(uiJobSettings.sizecode);
	
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_SCAN_JOBSTART;
    msg.param1 = SCANTOEMAILAPPID;
    msg.param2 = 0;
    msg.param3 = sv;
    err = SYMsgSend( job_resources->Source, &msg );
    XASSERT( err==OK, err );
}


/**
\brief scanner_job_get_current_working_folder
 * Return copy working folder path to pointer
 */
int scanner_job_get_current_working_folder(char *working_folder_path)
{
	if( strlen(gUI_Image_Folder) > 0 ) {
		strncpy( working_folder_path, gUI_Image_Folder, strlen(gUI_Image_Folder) );
		DBG_PRINTF_SDH_DEBUG(  "%s path is =%s\n", __FUNCTION__,  working_folder_path );
		return 0;
	}
	else {
		DBG_PRINTF_SDH_DEBUG(  "%s path is = NULL\n", __FUNCTION__ );
		return -1;
	}
}

/**
\brief Setup scanner_job_setupJobManage
 * In Here we setup the folder path & file name
 * 		Only USB Thumbdrive scan support change folder path & file name
 * 			- If user didn't setup the file name, file name fixed into time name
 * 			- If USB Thumbdrive path length is 0, folder path fixed into /mnt/sda1
 * 				
 * 		Scan To X doesn't support change folder path & file name
 * 			- folder path is fixed into /tmp/ui_scan/(#JobNum)
 * 			- file name is fixed into time name
 */
int scanner_job_setupJobManage(int save_filesystem_type)
{
	int result = 0;
	
	memset(gUI_Image_Folder, 0x00, sizeof(gUI_Image_Folder));
	
	/* increade job number */
	if(currentUiScanJobManage.uiScanJobNum++ > MAX_UISCAN_JOB_COUNT) {
		currentUiScanJobManage.uiScanJobNum = 0;
	}

	/* setup the default file name by time */
	scanner_service_set_variable_with_current_time(currentUiScanJobManage.uiScanJobTempFolderName);

	/* Setup Scan temp foler basename */
	if( save_filesystem_type == SCAN_SAVE_FILESYSTEM_TYPE_USE_THUMBDRIVE ) {
#if SDH_USE_SET_FILE_INFO
		memset( gUI_Image_Folder, 0x00, sizeof(gUI_Image_Folder) );
		
		//USB Folder Path
		if(strlen(uiJobSettings.saveToFileInfo.mountedPath) != 0) {
			strncpy(gUI_Image_Folder, uiJobSettings.saveToFileInfo.mountedPath, strlen(uiJobSettings.saveToFileInfo.mountedPath));	//user selected path
			DBG_PRINTF_SDH_DEBUG("USB Mount Path(1) : %s\n", gUI_Image_Folder);
		}
		else {
			strncpy(gUI_Image_Folder, gUI_usbThumbdriveRootPath, strlen(gUI_usbThumbdriveRootPath));	//usb thumbdrive root path
			DBG_PRINTF_SDH_DEBUG("USB Mount Path(2) : %s\n", gUI_Image_Folder);
		}		
		//USB Scan File Name
		if(strlen(uiJobSettings.saveToFileInfo.fileName) != 0) { 
			//user set name
			memset(currentUiScanJobManage.uiScanJobTempFolderName, 0x00, sizeof(currentUiScanJobManage.uiScanJobTempFolderName));
			strncpy(currentUiScanJobManage.uiScanJobTempFolderName, uiJobSettings.saveToFileInfo.fileName, strlen(uiJobSettings.saveToFileInfo.fileName));
			DBG_PRINTF_SDH_DEBUG("USB Folder Name(1) : %s\n", currentUiScanJobManage.uiScanJobTempFolderName);
		}
		//Setup the full path of folder which scan file will save into
//		sprintf(gUI_Image_Folder, "%s/%s", usbMountPath, currentUiScanJobManage.uiScanJobTempFolderName);
		DBG_PRINTF_SDH_DEBUG("USB Total Path : %s\n", gUI_Image_Folder);
		scanner_make_image_folder_using_system_cmd(gUI_Image_Folder);
		
		switch (uiJobSettings.fileFormat)
		{
		    case UI_FILE_PDF:
		    case UI_FILE_PDF_MULTI_PAGE:
				if( (uiJobSettings.ScanType == e_ColorScan) || 
					(uiJobSettings.ScanType == e_MonochromeScan && uiJobSettings.DataType == SCAN_DATA_TYPE_MONO) ) 
				{
					result = scanner_make_image_folder(TEMP_FOLDER_PATH, FALSE);
				}
				break;
			default :
				break;
		}
#else
		//Setup the full path of folder which scan file will save into
		sprintf(gUI_Image_Folder, "%s/%s", gUI_usbThumbdriveRootPath, currentUiScanJobManage.uiScanJobTempFolderName);
#endif
	}
	else {
		/* 
		 * Make the image folder for save scanned file which distingushed into individual folder
		 * example   "/tmp/ui_scan/1/" is the first ui_scan job
		 *           "/tmp/ui_scan/2/" is the 2nd ui_scan job
		 * deleting foler is occured when we received the result from 'sending app'
		 */
		//Setup the full path of folder which scan file will save into
		sprintf(gUI_Image_Folder, "%s/%d", gUI_ramDiskScanBasicPath, currentUiScanJobManage.uiScanJobNum);
		
		if(uiJobSettings.scanToDestination == UI_FILE_DEST_EMAIL)
		{
			//E-mail File Name
			if(strlen(uiJobSettings.sendToEmailInfo.fileName) != 0) { 
				//user set name
				memset(currentUiScanJobManage.uiScanJobTempFolderName, 0x00, sizeof(currentUiScanJobManage.uiScanJobTempFolderName));
				strncpy(currentUiScanJobManage.uiScanJobTempFolderName, uiJobSettings.sendToEmailInfo.fileName, strlen(uiJobSettings.sendToEmailInfo.fileName));
				DBG_PRINTF_SDH_DEBUG("E-mail Folder Name(2) : %s\n", currentUiScanJobManage.uiScanJobTempFolderName);
			}
		}
		else
		{
			//FTP/SMB/webDAV File Name
			if(strlen(uiJobSettings.sendToServerInfo.fileName) != 0) { 
				//user set name
				memset(currentUiScanJobManage.uiScanJobTempFolderName, 0x00, sizeof(currentUiScanJobManage.uiScanJobTempFolderName));
				strncpy(currentUiScanJobManage.uiScanJobTempFolderName, uiJobSettings.sendToServerInfo.fileName, strlen(uiJobSettings.sendToServerInfo.fileName));
				DBG_PRINTF_SDH_DEBUG("SendToServer Folder Name(3) : %s\n", currentUiScanJobManage.uiScanJobTempFolderName);
			}
		}
			
		/* Make folder for scaned file */
		result = scanner_make_image_folder(gUI_Image_Folder, TRUE);
	}

	return result;
}

int scanner_job_setupMergeJob_scanArea(int jobType)
{
	if(jobType == UI_SCAN_MERGEJOB_A3) {
		uiJobSettings.scan_window.right = 850;		//Width of 'Letter' for A3 Merge
		uiJobSettings.scan_window.bottom = 1170; 	//Height of 'A4' for A3 Merge 
		uiJobSettings.ScannableArea.right = 850;
		uiJobSettings.scanActionType = UI_SCAN_MERGEJOB_A3;
	}
	else if(jobType == UI_SCAN_MERGEJOB_11X17) {
		uiJobSettings.scan_window.right = 850;		//Width of 'Letter' for 11x17 Merge
		uiJobSettings.scan_window.bottom = 1100; 	//Height of 'Letter' for 11x17 Merge 
		uiJobSettings.ScannableArea.right = 850;
		uiJobSettings.scanActionType = UI_SCAN_MERGEJOB_11X17;		
	}
	else if(jobType == UI_SCAN_MERGEJOB_LEGAL) {
		uiJobSettings.scan_window.top = 0;
		uiJobSettings.scan_window.right = 850;
		uiJobSettings.scan_window.left = 0;
		uiJobSettings.scan_window.bottom = 725;
		uiJobSettings.ScannableArea.right = 850;
		
		//In Legal FB Scan, maximum resolution is 300, color mode is not supported.
		if(uiJobSettings.Resolution > MAX_LEGAL_MERGE_RESOLUTION) {
			uiJobSettings.Resolution = MAX_LEGAL_MERGE_RESOLUTION;
		}
		if(uiJobSettings.ScanType == e_ColorScan) {
			uiJobSettings.ScanType = e_MonochromeScan;
			uiJobSettings.DataType = SCAN_DATA_TYPE_MONO;
		}
		uiJobSettings.scanActionType = UI_SCAN_MERGEJOB_LEGAL;
	}
	else if(jobType == UI_SCAN_MERGEJOB_LEGAL_2ND_PAGE) {
		uiJobSettings.scan_window.left = 0;
		uiJobSettings.scan_window.right = 850;
		uiJobSettings.scan_window.top = 445;
		uiJobSettings.scan_window.bottom = 1170;
		uiJobSettings.ScannableArea.right = 850;
		uiJobSettings.scanActionType = UI_SCAN_MERGEJOB_LEGAL_2ND_PAGE;
	}
	else {
		uiJobSettings.scanActionType = UI_SCAN_MERGEJOB_NONE;
	}
	
	return 0;
}

int scanner_job_setupMergeJob_forStart(int jobType)
{
	if(jobType == UI_SCAN_MERGEJOB_A3) {				
		DBG_PRINTF_SDH_DEBUG(  "send_msg -> SCANMANID: MSG_A3MERGE_SET_START. A3 Merge\n" );
		MESSAGE send_msg;
		error_type_t err;
		memset( &send_msg, 0x00, sizeof(send_msg) );
		send_msg.msgType = MSG_A3MERGE_SET_START;
		send_msg.param1 = A3MERGE_SCAN;	 
		send_msg.param2 = A3MERGE_A3;
		err = SYMsgSend(SCANMANID, &send_msg );
		XASSERT( err==OK, err );
	}
	else if(jobType == UI_SCAN_MERGEJOB_11X17) {		
		DBG_PRINTF_SDH_DEBUG(  "send_msg -> SCANMANID: MSG_A3MERGE_SET_START. 11x17 Merge\n" );
		MESSAGE send_msg;
		error_type_t err;
		memset( &send_msg, 0x00, sizeof(send_msg) );
		send_msg.msgType = MSG_A3MERGE_SET_START;
		send_msg.param1 = A3MERGE_SCAN;	 
		send_msg.param2 = A3MERGE_DLT;
		err = SYMsgSend(SCANMANID, &send_msg );
		XASSERT( err==OK, err );
	}
	else if(jobType == UI_SCAN_MERGEJOB_LEGAL) {
		DBG_PRINTF_SDH_DEBUG(  "send_msg -> SCANMANID: MSG_A3MERGE_SET_START. Legal Merge\n" );
		MESSAGE send_msg;
		error_type_t err;
		memset( &send_msg, 0x00, sizeof(send_msg) );
		send_msg.msgType = MSG_A3MERGE_SET_START;
		send_msg.param1 = A3MERGE_SCAN;
		send_msg.param2 = A3MERGE_LG;
		err = SYMsgSend(SCANMANID, &send_msg );
		XASSERT( err==OK, err );		
	}
	return 0;
}

int scanner_clear_state_variable()
{
	uiscan_stats.sizes = 0;
	uiscan_stats.scan_starts = 0;
	uiscan_stats.scan_ends = 0;
	uiscan_stats.page_starts = 0;
	uiscan_stats.page_ends = 0;
	uiscan_stats.page_datas = 0;
	uiscan_stats.readies = 0;
	uiscan_stats.current_rows_index[0] = 0;
	uiscan_stats.current_rows_index[1] = 0;
	uiscan_stats.file_write_error = 0;
	
	page_number = 0;

	return 0;
}


/**
 * \brief scanner_malloc_page_memory
 *
 * malloc memory for save 1 page scanned image
 *
 */
static void scanner_malloc_page_memory(uint8_t page_side)
{
	if(scanOnePageImageBuf[page_side].buf != NULL) {
		if (scanOnePageImageBuf[page_side].bufsize_bytes == (uiscan_self.bytes_per_row * ((uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100)))
		{
			memset(scanOnePageImageBuf[page_side].buf, 0xFF, scanOnePageImageBuf[page_side].bufsize_bytes);
			scanOnePageImageBuf[page_side].write_index = 0;
			DBG_PRINTF_SDH_DEBUG(" scanOnePageImageBuf reuse[%ld] - bufsize_bytes : %ld\n",page_side, scanOnePageImageBuf[page_side].bufsize_bytes);
		}
		else
		{
			MEM_FREE_AND_NULL(scanOnePageImageBuf[page_side].buf);
			scanOnePageImageBuf[page_side].write_index = 0;
			scanOnePageImageBuf[page_side].bufsize_bytes = 0;
			DBG_PRINTF_SDH_DEBUG(" scanOnePageImageBuf[%ld] MEM_FREE_AND_NULL - bufsize_bytes : %ld\n",page_side, scanOnePageImageBuf[page_side].bufsize_bytes);
		}
	}
	if(scanOnePageImageBuf[page_side].buf == NULL) {
		/// Prepare Image Buffer of One Page scan, more 1 page dummy memory for incomming data when job cancel.
		scanOnePageImageBuf[page_side].bufsize_bytes = (uiscan_self.bytes_per_row * ((uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100));
		scanOnePageImageBuf[page_side].buf = MEM_MALLOC(scanOnePageImageBuf[page_side].bufsize_bytes);
		memset(scanOnePageImageBuf[page_side].buf, 0xFF, scanOnePageImageBuf[page_side].bufsize_bytes);
		scanOnePageImageBuf[page_side].write_index = 0;
		DBG_PRINTF_SDH_DEBUG(" scanOnePageImageBuf[%ld] Malloced --> W(%ld)H(%ld),bufsize_bytes(%ld)\n",
									page_side,
									uiscan_self.bytes_per_row, 
									(uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100,
									page_side, scanOnePageImageBuf[page_side].bufsize_bytes);
	}
}

/**
 * \brief scanner_malloc_page_memory for image log
 *
 * malloc memory for save 1 page scanned image for ImageLog
 *
 */
#ifdef HAVE_IMAGELOG
static void scanner_copy_page_memory_for_imagelog(uint8_t page_side)
{
	if(scanOnePageMemImageLog.buf != NULL) {
		MEM_FREE_AND_NULL(scanOnePageMemImageLog.buf);
		scanOnePageMemImageLog.write_index = 0;
		scanOnePageMemImageLog.bufsize_bytes = 0;
		DBG_PRINTF_SDH_DEBUG(" scanOnePageImageLogBuf[%d] MEM_FREE_AND_NULL - bufsize_bytes : %ld\n",page_side, scanOnePageMemImageLog.bufsize_bytes);
	}
	if(scanOnePageMemImageLog.buf == NULL && scanOnePageImageBuf[page_side].buf != NULL) {
		
		scanOnePageMemImageLog.bufsize_bytes = scanOnePageImageBuf[page_side].bufsize_bytes;
//		scanOnePageMemImageLog.buf = MEM_MALLOC(scanOnePageMemImageLog.bufsize_bytes);
		scanOnePageMemImageLog.buf = malloc(scanOnePageMemImageLog.bufsize_bytes);
		DBG_PRINTF_SDH_DEBUG("[RYU] addr:%p, size:%d\n",scanOnePageMemImageLog.buf, scanOnePageMemImageLog.bufsize_bytes);
		memset(scanOnePageMemImageLog.buf, 0xFF, scanOnePageMemImageLog.bufsize_bytes);
		memcpy(scanOnePageMemImageLog.buf, scanOnePageImageBuf[page_side].buf, scanOnePageImageBuf[page_side].bufsize_bytes);
		DBG_PRINTF_SDH_DEBUG(" scanOnePageImageLogBuf[%d] Malloced - bufsize_bytes : %ld\n",page_side, scanOnePageMemImageLog.bufsize_bytes);
	}
}
#endif

/**
 * \brief scanner_free_page_memory
 *
 * free memory for save 1 page scanned image
 *
 */
static void scanner_free_page_memory(uint8_t page_side)
{
	/*
	 ********************************************************************************
	 * Free Scaned Image page memory
	 ********************************************************************************
	 */
	if(scanOnePageImageBuf[page_side].buf != NULL) {
		MEM_FREE_AND_NULL(scanOnePageImageBuf[page_side].buf);
		scanOnePageImageBuf[page_side].write_index = 0;
		scanOnePageImageBuf[page_side].bufsize_bytes = 0;
	}
}

/**
 * \brief Handle start of a page.
 *
 * Does all the necessary page_start in a scan.
 *
 */
static void scanner_page_start(uint8_t page_side)
{
    char outfilename[FILENAME_MAX];
    struct output_file *outfile;
    
    /* TIFF Multi-Page */
	if(uiJobSettings.fileFormat == UI_FILE_TIFF_MULTI_PAGE) {				
		/// Prepare TIFF File Pointer
		//Only use front page pointer because this is Multi-Page tiff
		if( tifFile == NULL ) {
			make_output_filename( outfilename, UI_FILE_TIFF, uiscan_stats.page_starts, uiJobSettings.DataType, page_side );
			if((tifFile = TIFFOpen( outfilename, "w")) == NULL) {
				DBG_PRINTF_SDH_ERR("Could not open TIF file for writing\n");
				scanner_set_file_write_error(0);
				if(State == UI_SCAN_APP_SCANNING) {
					scanner_job_cancel_request();
				}
			}
			else {
				DBG_PRINTF_SDH_DEBUG(" UI_FILE_TIFF_MULTI_PAGE's FILE OPEN : %s\n", outfilename);
				/// set TIFF File parameter
				scanner_tiff_set_field(page_side);
			}
		}
	}
    /* TIFF Single-Page */
	else if(uiJobSettings.fileFormat == UI_FILE_TIFF) {
		if( page_side ) {
			if( tifFile_backside == NULL ) {
				// Set file name
				DBG_PRINTF_SDH_DEBUG( "outfile is NULL \n" );
				make_output_filename( outfilename, UI_FILE_TIFF, uiscan_stats.page_starts, uiJobSettings.DataType, page_side );
				// Open the TIFF file
				if((tifFile_backside = TIFFOpen( outfilename, "w")) == NULL) {
					DBG_PRINTF_SDH_ERR("Could not open TIF file for writing\n");
					scanner_set_file_write_error(0);
					if(State == UI_SCAN_APP_SCANNING) {
						scanner_job_cancel_request();
					}
				}
				else {
					DBG_PRINTF_SDH_DEBUG("  FILE OPEN : %s\n", outfilename);
					scanner_tiff_set_field(page_side);
				}
			}
		}
		else {
			if( tifFile == NULL ) {
				// Set file name
				DBG_PRINTF_SDH_DEBUG( "outfile is NULL \n" );
				make_output_filename( outfilename, UI_FILE_TIFF, uiscan_stats.page_starts, uiJobSettings.DataType, page_side );
				// Open the TIFF file
				if((tifFile = TIFFOpen( outfilename, "w")) == NULL) {
					DBG_PRINTF_SDH_ERR("Could not open TIF file for writing\n");
					scanner_set_file_write_error(0);
					if(State == UI_SCAN_APP_SCANNING) {
						scanner_job_cancel_request();
					}
				}
				else {
					DBG_PRINTF_SDH_DEBUG("  FILE OPEN : %s\n", outfilename);
					scanner_tiff_set_field(page_side);
				}
			}
		}
	}
    /* PDF */
	else if(uiJobSettings.fileFormat == UI_FILE_PDF || uiJobSettings.fileFormat == UI_FILE_PDF_MULTI_PAGE) {
		if( (uiJobSettings.ScanType == e_ColorScan) || 
			(uiJobSettings.ScanType == e_MonochromeScan && uiJobSettings.DataType == SCAN_DATA_TYPE_MONO) ) 
		{
			/* PDF Scan, if color, gray mode --> we use jpeg compress
			 * 				if B/W mode --> we use original raw data
			 */
			outfile = output_file_list_lookup( &outfile_list, uiJobSettings.DataType, page_side );
			if( outfile == NULL ) {
				///Although PDF file, we make JPEG file at first
				make_output_filename( outfilename, UI_FILE_JPEG, uiscan_stats.page_starts, uiJobSettings.DataType, page_side );
				outfile = output_file_list_fopen( &outfile_list, outfilename, uiJobSettings.DataType, page_side );
				
				DBG_PRINTF_SDH_DEBUG( "outfile is NULL : make_output_filename : %s\n" , outfilename);

				if( !outfile ) {
					DBG_PRINTF_SDH_ERR( "failed to open \"%s\" for scanned image!\n", outfilename );
					scanner_set_file_write_error(0);
					if(State == UI_SCAN_APP_SCANNING) {
						scanner_job_cancel_request();
					}
				}
				else {
					///Set JPEG File parameter
					scanner_jpeg_set_field(page_side, outfile->filep);

					///Save Current JPEG File Name for saving.
					scanner_backup_jpegFilename(page_side, outfilename);
				}
			}
			else {
				DBG_PRINTF_SDH_DEBUG(" Error: scanner_page_started, But outfile is Aleady Exi\n");
			}
		}
		else {
			DBG_PRINTF_SDH_DEBUG("PDF Scan mode is B/W\n");
		}
		///At Page Start, Make PDF Document (Just Rearded as One File)
		
		scanner_pdf_open_spdf(page_side);
	}
    /* JPEG File */	
	else if(uiJobSettings.fileFormat == UI_FILE_JPEG) {
		outfile = output_file_list_lookup( &outfile_list, uiJobSettings.DataType, page_side );
		if( outfile == NULL ) {
			//we make JPEG file at first
			make_output_filename( outfilename, UI_FILE_JPEG, uiscan_stats.page_starts, uiJobSettings.DataType, page_side );
			outfile = output_file_list_fopen( &outfile_list, outfilename, uiJobSettings.DataType, page_side );
			
			DBG_PRINTF_SDH_DEBUG( "outfile is NULL : make_output_filename : %s\n" , outfilename);

			if( !outfile ) {
				DBG_PRINTF_SDH_ERR( "failed to open \"%s\" for scanned image!\n", outfilename );
				scanner_set_file_write_error(0);
				if(State == UI_SCAN_APP_SCANNING) {
					scanner_job_cancel_request();
				}
			}
			else {
				///Set JPEG File parameter
				scanner_jpeg_set_field(page_side, outfile->filep);

				///Save Current JPEG File Name for saving.
				scanner_backup_jpegFilename(page_side, outfilename);
			}
		}
		else {
			DBG_PRINTF_SDH_DEBUG(" Error: scanner_page_started, But outfile is Aleady Exi\n");
		}
	}
	
	///malloc page memory
	scanner_malloc_page_memory(page_side);	
}


/**
 * \brief Handle end of a scanned page.
 *
 * Does all the necessary pageend tasks in a scan. Increments all the scan page counts.
 *
 */
static void scanner_page_end( uint8_t page_side )
{
	uint32_t totalScanWritableRows = 0;

#if USE_SCAN_DUMP
		DBG_PRINTF_SDH_DEBUG("Dump #1 START\n");
		FILE *fs;
		char path[200];
		
		sprintf(path, "/mnt/sda1/dump_file_%d_W%d_H%d.raw",  ++dump_no, uiscan_self.bytes_per_row, ((uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100));
		fs = fopen( path, "wb" );
		fwrite(scanOnePageImageBuf[page_side].buf, 1, scanOnePageImageBuf[page_side].bufsize_bytes, fs);
		fclose(fs);
		system("sync");
		DBG_PRINTF_SDH_DEBUG("Dump #1 END\n");
#endif
		
	/**
	 * \brief about incomming scan data (SINDOH Yhlee Wrote.)
	 * 		Incomming Scan line number couldn't exactly same what we set with scanvar_area
	 * 		For example, 
	 * 			if we set A4 size (8.25inch * 11.70inch, it means 2475(W)*3510(H)
	 * 			But, incomming scan data could be like 3321, 3237, 3321, or other value.
	 * 			So, we have to check the number of rows which incomming from /dev/scanman
	 * 			If, incomming number of rows is over what we want, we select the value we set.
	 * 			If, incomming number of rows is smaller than we want, we select the value incomming data's number of rows
	 */
	/*
	 ********************************************************************************
	 * Check Scan image buffer rows number before writing into File.
	 ********************************************************************************
	 */
	DBG_PRINTF_SDH_DEBUG(  "%s : received totalScanRows=%d, page_side:%d\n", __FUNCTION__, uiscan_stats.current_rows_index[page_side], page_side);
	if( uiscan_stats.current_rows_index[page_side] > (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100 ) {
		totalScanWritableRows = (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100;
	}
	else {
		//totalScanWritableRows = uiscan_stats.current_rows_front_side;
		/* Although scan image is less than user's setting area, we make whole image for user */
		totalScanWritableRows = (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100;
	}

	DBG_PRINTF_SDH_DEBUG(  "%s : total ScanWritable Rows=%d, page_side:%d\n", __FUNCTION__, totalScanWritableRows, page_side);
	
#ifdef HAVE_IMAGELOG
	if (g_nIsEnableScanImageLog)
	{
		if( (uiJobSettings.ScanType == e_MonochromeScan) && 
			(uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL || uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION) ) 
		{
			scanner_copy_page_memory_for_imagelog(page_side);
		}
	}
#endif
	/*
	 ********************************************************************************
	 * Write Scaned Image into File (TIFF Single-Page)
	 ********************************************************************************
	 */
	if(uiJobSettings.fileFormat == UI_FILE_TIFF) {
		if(uiscan_stats.file_write_error == 0) {			
			/// Write Scan Line (TIFF)
			if(page_side) {
				if( tifFile_backside != NULL ) {
					scanner_tiff_write_scanline(tifFile_backside, scanOnePageImageBuf[page_side].buf, 0, totalScanWritableRows, FALSE);				
					TIFFWriteDirectory(tifFile_backside);
				}
			}
			else {
				if( tifFile != NULL ) {
					scanner_tiff_write_scanline(tifFile, scanOnePageImageBuf[page_side].buf, 0, totalScanWritableRows, FALSE);
					TIFFWriteDirectory(tifFile);
				}
			}
		}
		else {
			DBG_PRINTF_SDH_DEBUG("File Error, can't write TIFF Lib\n");
		}
		
		// Close the TIFF file
		if(uiscan_stats.file_write_error == 0) {
			scanner_tiff_close(page_side);
		}
	}
	/*
	 ********************************************************************************
	 * Write Scaned Image into File (TIFF Multi-Page)
	 ********************************************************************************
	 */
	else if(uiJobSettings.fileFormat == UI_FILE_TIFF_MULTI_PAGE) {
		if(uiscan_stats.file_write_error == 0) {
			/// Set the TIFF multi-page page number
			//uint32_t tifMultiPageTotalpageNum = tifMultiPageWritePageNum + 1;
			scanner_tiff_set_field(0);
			//TIFFSetField(tifFile, TIFFTAG_PAGENUMBER, tifMultiPageWritePageNum++, tifMultiPageTotalpageNum);
			
			/// Write Scan Line (TIFF)
			scanner_tiff_write_scanline(tifFile, scanOnePageImageBuf[page_side].buf, 0, totalScanWritableRows, FALSE);
			TIFFWriteDirectory(tifFile);
		}
		else {
			DBG_PRINTF_SDH_DEBUG("File Error, can't write TIFF Lib\n");
		}
	}
	/*
	 ********************************************************************************
	 * Write Scaned Image into File (PDF, PDF-Multi)
	 ********************************************************************************
	 */
	else if(uiJobSettings.fileFormat == UI_FILE_PDF || uiJobSettings.fileFormat == UI_FILE_PDF_MULTI_PAGE) {
		if( (uiJobSettings.ScanType == e_ColorScan) || 
			(uiJobSettings.ScanType == e_MonochromeScan && uiJobSettings.DataType == SCAN_DATA_TYPE_MONO) ) 
		{
			/* Color, Gray Mode */
			if(uiscan_stats.file_write_error == 0) {
				///Write Scan Line (JPEG)
				scanner_jpeg_write_scanline(page_side, scanOnePageImageBuf[page_side].buf, totalScanWritableRows);
				
				///Close JPEG File
				scanner_jpeg_close(page_side);
			}
			else {
				DBG_PRINTF_SDH_DEBUG("File Error, can't write/close JPEG Lib\n");
			}
			DBG_PRINTF_SDH_DEBUG("PDF Scan is Color/Gray Mode\n");
		}
		else {
			DBG_PRINTF_SDH_DEBUG("PDF Scan is B/W Mode\n");
		}
		
		if(uiscan_stats.file_write_error == 0) {
			/// Write Page Image (PDF)
			scanner_pdf_write_pageImage_spdf( page_side );
		}
		else {
			DBG_PRINTF_SDH_DEBUG("File Error, can't write/close PDF Multi\n");
		}	
		if(uiJobSettings.fileFormat == UI_FILE_PDF) {				
			///Close pdf
			scanner_pdf_close_spdf(page_side);
		}
	}
	/*
	 ********************************************************************************
	 * Write Scaned Image into File (JPEG)
	 ********************************************************************************
	 */
	else if(uiJobSettings.fileFormat == UI_FILE_JPEG) {
		if(uiscan_stats.file_write_error == 0) {
			///Write Scan Line (JPEG)
			scanner_jpeg_write_scanline(page_side, scanOnePageImageBuf[page_side].buf, totalScanWritableRows);
			
			///Close JPEG File
			scanner_jpeg_close(page_side);
		}
		else {
			DBG_PRINTF_SDH_DEBUG("File Error, can't write/close JPEG Lib\n");
		}
	}
	
#ifdef HAVE_IMAGELOG
		/* If machine use image log function, save into SD or fixed location for send to server */
		//check using of image log function set in SP(DB value)
	if(g_nIsEnableScanImageLog ) {
			scanner_imagelog_start(page_side);
		}
#endif

	/*
	 ********************************************************************************
	 * Free Scaned Image page memory
	 ********************************************************************************
	 */
	/// Free Page Memory
	//scanner_free_page_memory(page_side);

	/* only update the page_number for a single side */
//	if( page_side == 0 ) {
		page_number += 1;
//	}
	
	/// Clear Count Value
	uiscan_self.gtotal = 0;
	uiscan_stats.current_rows_index[page_side] = 0;
}

/**
 * \brief send_cancel_ack_msg
 *
 * Sends an send_cancel_ack_msg to system job manager
 * to the system job manager to release myself as a resource.
 */
static void send_cancel_ack_msg( void *param3 )
{
	MESSAGE msg;
	error_type_t err;

    /* sending the MSG_CANCELJOB(SYS_ACK) */
	memset( &msg, 0, sizeof(msg) );
	msg.msgType = MSG_CANCELJOB;
	msg.param1 = SYS_ACK;
	msg.param2 = SCANTOEMAILAPPID;
	msg.param3 = param3;
	err = SYMsgSend( SJMID, &msg);
	XASSERT( err==OK, err );
}

static void send_free_msg( void )
{
	MESSAGE msg;
	error_type_t err;
	
   /* sending the FREERECIPE */
	memset( &msg, 0, sizeof(msg) );
	msg.msgType = MSG_FREERECIPE;
	msg.param1 = 0;
	msg.param2 = 0;
	msg.param3 = job_resources;
	err = SYMsgSend( SJMID, &msg );
	XASSERT( err==OK, err );
}

/**
 * \brief scanner_job_clear_app_resource
 *
 * When scan job finished or canceled, clear resource of 'scan application'
 */
int scanner_job_clear_app_resource(void)
{
    uint8_t idx = 0;
    
	/// Clear Memory
	for(idx=0; idx<UI_SCAN_DUAL_PAGE_NUM; idx++) {
		scanner_free_page_memory(idx);
	}
		   
	/// File Close
	switch(uiJobSettings.fileFormat) {
	case UI_FILE_TIFF:
	case UI_FILE_TIFF_MULTI_PAGE:
		scanner_tiff_close(FALSE);	//TIFF front page
		scanner_tiff_close(TRUE);	//TIFF back page
		break;
	case UI_FILE_JPEG:
		scanner_jpeg_close(FALSE);	//JPEG front page
		scanner_jpeg_close(TRUE);	//JPEG back page
		break;
	case UI_FILE_PDF_MULTI_PAGE:
		scanner_jpeg_close(FALSE);	//JPEG front page
		scanner_jpeg_close(TRUE);	//JPEG back page
		scanner_pdf_close_spdf(FALSE);
		break;
	case UI_FILE_PDF:	//PDF Single-page
		scanner_jpeg_close(FALSE);	//JPEG front page
		scanner_jpeg_close(TRUE);	//JPEG back page
		scanner_pdf_close_spdf(FALSE);	//PDF front page
		scanner_pdf_close_spdf(TRUE);	//PDF back page
		break;
	default:
		break;
	}

	/// remove temp files (just for PDF scan)
	if ((uiJobSettings.fileFormat == UI_FILE_PDF_MULTI_PAGE || uiJobSettings.fileFormat == UI_FILE_PDF))
	{
		if( (uiJobSettings.ScanType == e_ColorScan) || 
			(uiJobSettings.ScanType == e_MonochromeScan && uiJobSettings.DataType == SCAN_DATA_TYPE_MONO) ) 
		{
			char sysCmd[MAX_SYSTEM_CMD_LENGTH] = {0x00,};
			sprintf(sysCmd, "rm -rf \"%s\"", TEMP_FOLDER_PATH);
			system(sysCmd);
			system("sync");
		}
	}
	return 0;
}

/**
 * \brief End a scan job.
 *
 * Sends an end-job message to the resource. Sends the necessary message
 * to the system job manager to release myself as a resource.
 */
static void scanner_job_end( struct scanvars *sv, CURRENT_RESOURCE *job_resources )
{
    MESSAGE msg;
    error_type_t err;

    DBG_PRINTF_SDH_DEBUG(  "%s job_resources=%p src=%d\n", __FUNCTION__, job_resources, job_resources->Source );

    /* Tell the source that we are done talking to him. */
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_SCAN_JOBEND;
    msg.param2 = SCANTOEMAILAPPID;
    msg.param3 = job_resources;
    err = SYMsgSend( job_resources->Source, &msg ); // tell the scanner to finish.
    XASSERT( err==OK, err );

    /* clear resource of scan application */
	scanner_job_clear_app_resource();

    /* sending the FREERECIPE */
	send_free_msg();

	if(uiJobSettings.scanToDestination == UI_FILE_DEST_EMAIL || uiJobSettings.scanToDestination == UI_FILE_DEST_FTP 
		|| uiJobSettings.scanToDestination == UI_FILE_DEST_SMB || uiJobSettings.scanToDestination == UI_FILE_DEST_WEBDAV) {
		scanner_send_image_files(uiJobSettings.scanToDestination);
	}

	STATE_TRANSITION( UI_SCAN_APP_IDLE );
}


/**
 * \brief Handle User Input of a pc_scan job.
 *
 * User Input the User Panel for next scan job
 *
 */
void scanner_pc_scan_input( uint32_t action )
{
	MESSAGE msg;
	error_type_t err;
	memset( &msg, 0, sizeof(msg));
	switch(action)
	{
		case SCAN_ACTION_RESUME:
			DBG_PRINTF_SDH_DEBUG("%s -> SCAN_ACTION_RESUME\n", __FUNCTION__ );
			msg.msgType = MSG_USER_INPUT;
			msg.param1 = PARAM_USER_OK;
			break;
#if SDH_USE_PC_SCAN_CANCEL_BY_UI
		case SCAN_ACTION_CANCEL:			//PC Scan job cancel after job start
			DBG_PRINTF_SDH_DEBUG("%s -> SCAN_ACTION_CANCEL\n", __FUNCTION__ );
			msg.msgType = MSG_USER_INPUT;
			msg.param1 = PARAM_USER_CANCEL_JOB;
			break;
#endif
		default:
			return; // ignore, dont send msg
			break;
	}
	
	err = SYMsgSend( SCANPARSEID, &msg );
	XASSERT( err==OK, err ); 
}

/**
 * \brief Handle User Input of a scan job.
 *
 * User Input the User Panel for scan job stop/cancel/pause
 *
 */
void scanner_ui_scan_input( uint32_t action )
{
    MESSAGE msg;
    error_type_t err;
    
    memset( &msg, 0, sizeof(msg));
    
    switch(action)
    {
		case SCAN_ACTION_RESUME:
			DBG_PRINTF_SDH_DEBUG("%s -> SCAN_ACTION_RESUME\n", __FUNCTION__ );
			msg.msgType = MSG_USER_INPUT;
			msg.param1 = PARAM_USER_OK;
			//transfer message to PC Scan App
			scanner_pc_scan_input(SCAN_ACTION_RESUME);
			break;	
		case SCAN_ACTION_FINISH:
			DBG_PRINTF_SDH_DEBUG("%s -> SCAN_ACTION_FINISH\n", __FUNCTION__ );
			msg.msgType = MSG_USER_INPUT;
			msg.param1 = PARAM_USER_FINISH_JOB;
			break;
		case SCAN_ACTION_CANCEL:
			DBG_PRINTF_SDH_DEBUG("%s -> SCAN_ACTION_CANCEL\n", __FUNCTION__ );
			/*
			 ******************************************************************************************************
			 * Regarding Job Cancel 
			 * 		- ADF Pause command works fine : scanmech_adf_client_request_pause()
			 * 		- ADF Resume command works not fine : scanmech_adf_client_clear_pause()
			 * 	SINDOH Spec : If user push 'cancel' button in UI, App pause the ADF and cancel job
			 ******************************************************************************************************
			 */
			DBG_PRINTF_SDH_DEBUG("%s -> SCAN_ACTION_CANCEL : Current State is %d\n", __FUNCTION__ , State);
			
 			if( State == UI_SCAN_APP_SCANNING ) {
				//Request Job Cancel
				scanner_job_cancel_request();
			}
#if 0
			else {
#if 1
				scerr = scanmech_adf_client_clear_pause();
#else
				uint32_t req_id;
				scerr = scanman_dev_request(SCANMECH_IOCTL_ADF_CLIENT_PAUSE, 0, NULL, &req_id);	//Resume
#endif
				STATE_TRANSITION(UI_SCAN_APP_SCANNING);
				
				if( scerr == SCANERR_NONE ) {
					DBG_PRINTF_SDH_DEBUG("SCANMECH_IOCTL_ADF_CLIENT_RESUME : Success\n");
				}
				else {
					DBG_PRINTF_SDH_DEBUG("SCANMECH_IOCTL_ADF_CLIENT_RESUME : FAIL : %d\n", scerr);						
				}
				
				DBG_PRINTF_SDH_DEBUG("%s -> Not Work. State is %d\n", __FUNCTION__ , State);
			}
#endif
			return;
			break;
		default:
			return; // ignore, dont send msg
			break;
    }

    err = SYMsgSend( SCANTOEMAILAPPID, &msg );
    XASSERT( err==OK, err ); 
}

/**
 * \brief Erase the tmeporary image file located in RAMDISK
 *
 * For deleting the image located in RAMDISK
 *
 */
int scanner_erase_image_folder(char *folderPath)
{
	DBG_PRINTF_SDH_DEBUG(" [%s]: scanner_erase_image_folder: START!\n", __FUNCTION__);
	char sysCmd[MAX_SYSTEM_CMD_LENGTH] = {0x00,};
	sprintf(sysCmd, "rm -rf %s", folderPath);
	system(sysCmd);
	system("sync");
	DBG_PRINTF_SDH_DEBUG(" [%s]: scanner_erase_image_folder: END!\n", __FUNCTION__);

	return 0;
}

/**
 * \brief Erase the tmeporary image file located in RAMDISK
 *
 * For deleting the image located in RAMDISK
 *
 */
int scanner_erase_image_file(char *filePath)
{
	DBG_PRINTF_SDH_DEBUG(" [%s]: scanner_erase_image_file: START!\n", __FUNCTION__);
	char sysCmd[MAX_SYSTEM_CMD_LENGTH] = {0x00,};
	sprintf(sysCmd, "rm %s", filePath);
	system(sysCmd);
	system("sync");
	DBG_PRINTF_SDH_DEBUG(" [%s]: scanner_erase_image_file: END!\n", __FUNCTION__);
	
	return 0;
}

/**
 * \brief scanner_service_get_filesystem_usage
 * 
 * 		We have to check Ram Disk Avariable Size when using Ram Disk(/tmp) for Send (Email, FTP, SMB)
 * 		when using USB Thumbdrive we have to check USB Thumbrive(/mnt/sda1)
 * 
 * 	<Other Method Example>
 * Assuming a modern POSIX (Linux, OS X, etc.), 
 * we can also use statvfs: struct statvfs st; 
 * 		$ df -h .
 * 		Filesystem     Size   Used  Avail Capacity  Mounted on
 * 		/dev/disk0s2  233Gi   82Gi  150Gi    36%    /
 * 		To attribute usage, use du -h /path (disk usage).
 * statvfs("path", &st);
 * unsigned long free_space = st.f_bfree * st.f_frsize;
 * Use df -h /path/to/directory (mnemonically, "disk free"). 
 * The -h flag causes it to use human-readable, base-2 sizes (based on 1024 bytes to the kilobyte) 
 * rather than a raw count of 512-byte blocks. You can get base-10 sizes (based on 1000 bytes to the kilobyte 
 */
 
int scanner_service_get_filesystem_usage(char *path)
{
	int ret_val = 0;
	int CurrentTotalSize = 0;
	struct statfs lstatfs;

	statfs(path, &lstatfs);

	CurrentTotalSize = lstatfs.f_bavail * (lstatfs.f_bsize >> 10);
	DBG_PRINTF_SDH_DEBUG("DISK USABLE SIZE : %d, PATH:%s\n", CurrentTotalSize, path);

	if (CurrentTotalSize >= UISCAN_MINIMUM_MEMSIZE) {
		ret_val = 0; // not full(Ok)
	}
	else {
		ret_val = -1; // full
	}

	return ret_val;
}

int scanner_service_set_variable_with_current_time(char *variable)
{
	memset(variable, 0x00, sizeof(variable));

	if( uiJobSettings.isPersonalScanJob == TRUE ) {
		//Get Time until usec
		struct timeval now;
		struct tm *tm;
		gettimeofday(&now, NULL);

		tm = localtime(&now.tv_sec);
		sprintf(variable, "%04d%02d%02d%02d%02d%02d%03d%03d",	tm->tm_year + 1900, 
																		tm->tm_mon + 1, 
																		tm->tm_mday, 
																		tm->tm_hour, 
																		tm->tm_min, 
																		tm->tm_sec, 
																		(int)(now.tv_usec/1000), 
																		(int)(now.tv_usec%1000));
	}
	else {
		//Get Time until sec
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(variable, "%04d%02d%02d_%02d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec );
	}
	
	return 0;
}

/**
 * \brief scanner_make_end_file
 *
 *
 */
int scanner_make_end_file(char *fileFullPath, char *file_name)
{
	FILE *scan_end_file;
	char end_file_full_name[100] = {0x00, };
	char authUserId[AUTH_BASIC_LENGTH] = {0x00,};
	snprintf( authUserId, sizeof(authUserId), "%s", GetCurerntAuthUser() );
	
	if ((( uiJobSettings.scanToDestination == UI_FILE_DEST_USBTHUMB) && (strlen(uiJobSettings.saveToFileInfo.fileName) == 0 ))
		|| (( uiJobSettings.scanToDestination == UI_FILE_DEST_EMAIL )&& (strlen(uiJobSettings.sendToEmailInfo.fileName) == 0))
		|| ((( uiJobSettings.scanToDestination == UI_FILE_DEST_FTP ) || ( uiJobSettings.scanToDestination == UI_FILE_DEST_SMB )
			|| ( uiJobSettings.scanToDestination == UI_FILE_DEST_WEBDAV )) && (strlen(uiJobSettings.sendToServerInfo.fileName) == 0)))
		sprintf( end_file_full_name, "%s/S%s_%s_box.end", fileFullPath, file_name, authUserId );
	else
		sprintf( end_file_full_name, "%s/%s.end", fileFullPath, file_name);
	
	DBG_PRINTF_SDH_DEBUG("endfile : %s\n", end_file_full_name);
	scan_end_file = fopen( end_file_full_name, "wr");
	
	if (scan_end_file)
		fclose( scan_end_file );
	
	return 1;
}

/**
 * \brief Sending result of Email, FTP, SMB etc...,
 *
 *
 */
int scanner_send_result(unsigned int sendResultJobId, unsigned int errorCode) 
{
	DBG_PRINTF_SDH_DEBUG("%s Result Job ID:(%d), ErrorCode:(%d)\n", __FUNCTION__ , sendResultJobId, errorCode);
	
	unsigned int sendResultId = 0;
	char tmpPath[MAX_OUTPUT_FILENAME];
	memset(tmpPath, 0x00, sizeof(tmpPath));
	
	sendResultId = sendResultJobId;
	sprintf(tmpPath, "%s/%d", gUI_ramDiskScanBasicPath, sendResultId);
	scanner_erase_image_folder(tmpPath);
	
	return 0;
}

/**
 * \brief Sending scanned image to Email, FTP, etc...,
 *
 * For Sending use solution API
 *
 */
int scanner_send_image_files(int destination)
{
	int sendResult = 0;
	
	/***************************************************************
	* Request Email Send
	***************************************************************/
	switch(destination) {
	case UI_FILE_DEST_EMAIL:	
		sendResult = scanner_send_to_email();
		break;
	case UI_FILE_DEST_FTP:
		sendResult = scanner_send_to_ftp(SEND_REQUEST_NORMAL);
		break;
	case UI_FILE_DEST_SMB:
		sendResult = scanner_send_to_smb(SEND_REQUEST_NORMAL);
		break;
	case UI_FILE_DEST_WEBDAV:
		sendResult = scanner_send_to_webdav(SEND_REQUEST_NORMAL);
		break;
	default:
		break;
	}
	
	return sendResult;
}

/**
 * \brief Sending scanned image to Email
 */
#if 0
int scanner_send_to_email()
{
	int sendResult = 0;
	stSolution_Request	sendRequestMsg;
	char emailSenderAddr[SDH_EMAIL_ADDRESS_STR_MAX] = {0,};
	char emailReceiverAddrList[SDH_EMAIL_RECEIVER_MAX*SDH_EMAIL_ADDRESS_STR_MAX] = {0,};
	char emailTitleStr[SDH_EMAIL_SUBJECT_STR_MAX] = {0,};
	char emailBodyStr[SDH_EMAIL_TEXT_STR_MAX] = {0,};
		
	/***************************************************************
	* Get Setting Value of SMTP
	***************************************************************/
	char serverAddr[200]={0,};
	char senderID[200]={0,};
	char senderPW[200]={0,};
	int serverPort = 0;
	int secureType = 1;
	int authType = 1;
	int isDebugMode = 0;
	
	error_type_t db_retval = FAIL;
	char* tempValue;
	
	/* Mail Server */	
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailServer", &db_retval);
	if(db_retval == OK) {
		if(tempValue != NULL) {
			snprintf (serverAddr,  sizeof(serverAddr), "%s",tempValue);
			MEM_FREE_AND_NULL(tempValue);
		}
	}
	
	/* Mail ID */	
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailID", &db_retval);
	if(db_retval == OK) {
		if(tempValue != NULL) {
			snprintf (senderID,  sizeof(senderID), "%s",tempValue);
			MEM_FREE_AND_NULL(tempValue);
		}
	}

	/* Mail Password */	
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailPW", &db_retval);
	if(db_retval == OK) {
		if(tempValue != NULL) {
			snprintf (senderPW,  sizeof(senderPW), "%s",tempValue);
			MEM_FREE_AND_NULL(tempValue);
		}
	}

	/* Mail Port */
	serverPort = db_get_var_int("CONF_SYSTEM", "DefaultMailPort", &db_retval);
	if(db_retval == FAIL) {
		serverPort = 0;
	}
	db_retval = FAIL;
#if 0
	/* Security type */
	secureType = db_get_var_int("CONF_SYSTEM", "DefaultMailSecureType", &db_retval);
	if(db_retval == FAIL) {
		secureType = 0;
	}
	db_retval = FAIL;

	/* Auth type */
	authType = db_get_var_int("CONF_SYSTEM", "DefaultMailAuthType", &db_retval);
	if(db_retval == FAIL) {
		authType = 0;
	}
	db_retval = FAIL;
#endif

	/***********************************************************************************
	* Get Setting Value of Email Sender, Receiver, Subject, Text
	************************************************************************************/
	/* Sender */
	if(strlen(uiJobSettings.sendToEmailInfo.emailSenderAddress) != 0) {
		DBG_PRINTF_SDH_DEBUG("SENDER set to User Setting\n");
		strncpy(emailSenderAddr, uiJobSettings.sendToEmailInfo.emailSenderAddress, sizeof(emailSenderAddr));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("SENDER set to SINODH\n");
		strncpy(emailSenderAddr, "SINDOH", sizeof(emailSenderAddr));		
	}

	/* Receiver */
	if(strlen(uiJobSettings.sendToEmailInfo.emailReceiverAddress) != 0) {
		strncpy(emailReceiverAddrList, uiJobSettings.sendToEmailInfo.emailReceiverAddress, sizeof(emailReceiverAddrList));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("uiJobSettings.sendToEmailInfo.emailReceiverAddress : NULL\n");
	}
	
	/* Subject (Title) */
	if(strlen(uiJobSettings.sendToEmailInfo.emailSubject) != 0) {
		strncpy(emailTitleStr, uiJobSettings.sendToEmailInfo.emailSubject, sizeof(emailTitleStr));
	}
	else {
		strncpy(emailTitleStr, "Scanned Image", sizeof(emailTitleStr));
	}
	
	/* Text */
	if(strlen(uiJobSettings.sendToEmailInfo.emailText) != 0) {
		strncpy(emailBodyStr, uiJobSettings.sendToEmailInfo.emailText, sizeof(emailBodyStr));
	}
	else {
		strncpy(emailBodyStr, "Attached files is scanned image", sizeof(emailBodyStr));
	}
	
	/* Request Message Queue */
	memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));
	sendRequestMsg.sendRequestAppMsgQueueId = SCANTOEMAILAPPID;
	sendRequestMsg.sendJobId = currentUiScanJobManage.uiScanJobNum;
	sendRequestMsg.systemJobID = currentUiScanJobManage.systemJobID;
	
	DBG_PRINTF_SDH_DEBUG(" Email ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" Email  Scan App -- [ Email Send REQUEST ]\n");
	DBG_PRINTF_SDH_DEBUG(" Email ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" Email  M.Q AppMessageId        : %d\n", sendRequestMsg.sendRequestAppMsgQueueId);
	DBG_PRINTF_SDH_DEBUG(" Email  M.Q systemJobID         : %d\n", sendRequestMsg.systemJobID);
	DBG_PRINTF_SDH_DEBUG(" Email  M.Q sendJobId           : %d\n", sendRequestMsg.sendJobId);	
	DBG_PRINTF_SDH_DEBUG(" Email  sender ID               : %s\n", senderID);
	DBG_PRINTF_SDH_DEBUG(" Email  sender PW               : %s\n", senderPW);
	DBG_PRINTF_SDH_DEBUG(" Email  Sender Addr             : %s\n", emailSenderAddr);
	DBG_PRINTF_SDH_DEBUG(" Email  server Addr             : %s\n", serverAddr);
	DBG_PRINTF_SDH_DEBUG(" Email  server Port             : %d\n", serverPort);
	DBG_PRINTF_SDH_DEBUG(" Email  gUI_Image_Folder        : %s\n", gUI_Image_Folder);
	DBG_PRINTF_SDH_DEBUG(" Email  Receiver AddrList       : %s\n", emailReceiverAddrList);
	DBG_PRINTF_SDH_DEBUG(" Email  TitleStr                : %s\n", emailTitleStr);
	DBG_PRINTF_SDH_DEBUG(" Email  BodyStr                 : %s\n", emailBodyStr);
	DBG_PRINTF_SDH_DEBUG(" Email  secureType              : %d\n", secureType);
	DBG_PRINTF_SDH_DEBUG(" Email  authType                : %d\n", authType);
	DBG_PRINTF_SDH_DEBUG(" Email  isDebugMode             : %d\n", isDebugMode);

	/*
	 ********************************************************************************************************************************************************
	 * <<SCAN TO EMAIL>>
	 * [API] Header File - common/network/apps/ports/include/solution.h
	 * 			int sendMailWrapper(const char *username, const char *password, const char *sender,
	 * 					const char *mailhost, int port, const char *attachmentPath, const char *toAddress,
	 * 					const char *subject, const char *content, int secureType, int authType, int isDebugMode, stSolution_Request *email_mqueue);
	 * 			Example - sendMailWrapper(SMTP?�버ID, SMTP?�버PW, 보내?�사?�주?? 
	 * 						SMTP?�버주소, SMTP?�버?�트, 첨�??�일경로, 받는?�람주소, 메일?�목, 메일본문, secureType, authType, isDebugMode, messageQueue);
	 * 
	 * 					- srsmtp.sindoh.com, 25
	 * 					- smtp.gmail.com, 25
	 * 					- receiver : "younghoolee@sindoh.com;younghoo82@gmail.com;akhileshnema@sindoh.com;nexus87@sindoh.com"
	 ********************************************************************************************************************************************************
	 */

#if SDH_USE_SEND_EMAIL_FUNCTION_TEST
	sendResult = sendMailWrapper(senderID, senderPW, emailSenderAddr, serverAddr, serverPort, gUI_Image_Folder, 
						"younghoolee@sindoh.com;younghoo82@gmail.com", emailTitleStr, emailBodyStr, secureType, authType, isDebugMode, &sendRequestMsg);
#else
	sendResult = sendMailWrapper(senderID, senderPW, emailSenderAddr, serverAddr, serverPort, gUI_Image_Folder, 
						emailReceiverAddrList, emailTitleStr, emailBodyStr, secureType, authType, isDebugMode, &sendRequestMsg);
#endif
														
	if(sendResult == -1) {
		DBG_PRINTF_SDH_DEBUG(" [%s]: Email Send[#1] Fail!\n", __FUNCTION__);
	}
	else {
		DBG_PRINTF_SDH_DEBUG(" [%s]: Email Send[#1] Success!\n",__FUNCTION__);
	}
	
	return sendResult;
}
#else
int scanner_send_to_email()
{
	email_info_message_t *email_info=malloc(sizeof(email_info_message_t));
	int sendResult = 0;

	/***************************************************************
	* Get Setting Value of SMTP
	***************************************************************/
	error_type_t db_retval = FAIL;
	char* tempValue;

	/* Mail ID, username */
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailID", &db_retval);
	if ((db_retval == OK) && (tempValue != NULL)) {
		email_info->username = (char *)malloc(strlen(tempValue)+1);
		strcpy(email_info->username, tempValue);
	}
	else {
		email_info->username = NULL;
		DBG_PRINTF_SDH_DEBUG("username : NULL\n");
	}

	/* Mail Password, password */	
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailPW", &db_retval);
	if ((db_retval == OK) && (tempValue != NULL)) {
		email_info->password = (char *)malloc(strlen(tempValue)+1);
		strcpy(email_info->password, tempValue);
	}
	else {
		email_info->password = NULL;
		DBG_PRINTF_SDH_DEBUG("password : NULL\n");
	}
	
	/* senderEmail, DB:DefaultMailAddress */
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailAddress", &db_retval);
	if ((db_retval == OK) && (tempValue != NULL)) {
		email_info->senderEmail = (char *)malloc(strlen(tempValue)+1);
		strcpy(email_info->senderEmail, tempValue);
	}
	else {
		email_info->senderEmail = NULL;
		DBG_PRINTF_SDH_DEBUG("senderEmail : NULL\n");
	}
	
	/* senderName, DB:DefaultMailUserName */
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailUserName", &db_retval);
	if ((db_retval == OK) && (tempValue != NULL)) {
		email_info->senderName = (char *)malloc(strlen(tempValue)+1);
		strcpy(email_info->senderName, tempValue);
	}
	else {
		email_info->senderName = NULL;
		DBG_PRINTF_SDH_DEBUG("senderName : NULL\n");
	}
	
	/* mailhost, DB:DefaultMailServer */
	tempValue = db_get_var("CONF_SYSTEM", "DefaultMailServer", &db_retval);
	if ((db_retval == OK) && (tempValue != NULL)) {
		email_info->mailhost = (char *)malloc(strlen(tempValue)+1);
		strcpy(email_info->mailhost, tempValue);
	}
	else {
		email_info->mailhost = NULL;
		DBG_PRINTF_SDH_DEBUG("mailhost : NULL\n");
	}
	
	/* port, Mail Port, DB:DefaultMailPort*/
	email_info->port = db_get_var_int("CONF_SYSTEM", "DefaultMailPort", &db_retval);
	if (db_retval == FAIL) {
		email_info->port = 0;
	}

	/***********************************************************************************
	* Get Setting Value of attachmentPath, Receiver, Subject, Content
	************************************************************************************/
	/* attachmentPath */
	if(strlen(gUI_Image_Folder) != 0) {
		email_info->attachmentPath = (char *)malloc(strlen(gUI_Image_Folder)+1);
		strcpy(email_info->attachmentPath, gUI_Image_Folder);
	}
	else
	{
		email_info->attachmentPath = NULL;
		DBG_PRINTF_SDH_DEBUG("attachmentPath : NULL\n");
	}

	/* Receiver */
	if(strlen(uiJobSettings.sendToEmailInfo.emailReceiverAddress) != 0) {
		email_info->toAddress = (char *)malloc(strlen(uiJobSettings.sendToEmailInfo.emailReceiverAddress)+1);
		strcpy(email_info->toAddress, uiJobSettings.sendToEmailInfo.emailReceiverAddress);
	}
	else {
		email_info->toAddress = NULL;
		DBG_PRINTF_SDH_DEBUG("uiJobSettings.sendToEmailInfo.emailReceiverAddress : NULL\n");
	}
	
	/* Subject (Title) */
	email_info->subject = (char *)malloc(SDH_EMAIL_SUBJECT_STR_MAX);
	memset(email_info->subject,0x00, SDH_EMAIL_SUBJECT_STR_MAX);
	if(strlen(uiJobSettings.sendToEmailInfo.emailSubject) != 0) {
		strncpy(email_info->subject, uiJobSettings.sendToEmailInfo.emailSubject, SDH_EMAIL_SUBJECT_STR_MAX-1);
	}
	else {
		strcpy(email_info->subject, "Scanned Image");
	}

	/* Text */
	email_info->content = (char *)malloc(SDH_EMAIL_TEXT_STR_MAX);
	memset(email_info->content, 0x00, SDH_EMAIL_TEXT_STR_MAX);
	if(strlen(uiJobSettings.sendToEmailInfo.emailText) != 0) {
		strncpy(email_info->content, uiJobSettings.sendToEmailInfo.emailText, SDH_EMAIL_TEXT_STR_MAX-1);
	}
	else {
		if ((uiscan_stats.page_starts == 1) || (uiJobSettings.fileFormat == UI_FILE_PDF_MULTI_PAGE) || (uiJobSettings.fileFormat == UI_FILE_TIFF_MULTI_PAGE))
		{
			strcpy(email_info->content, "Attached file is scanned image.");
		}
		else
		{
			strcpy(email_info->content, "Attached files are scanned image.");
		}
	}

	/* secureType, DB:DefaultMailSecurity */
	email_info->secureType = db_get_var_int("CONF_SYSTEM", "DefaultMailSecurity", &db_retval);
	if (db_retval == FAIL) {
		email_info->secureType = 0;
	}

	/* authType, DB:DefaultMailAuth */
	email_info->authType = db_get_var_int("CONF_SYSTEM", "DefaultMailAuth", &db_retval);
	if (db_retval == FAIL) {
		email_info->authType = 0;
	}
	
	/* email_msg */
	email_info->email_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
	memset(email_info->email_msg, 0x00, sizeof(stSolution_Request));
	email_info->email_msg->sendRequestAppMsgQueueId = SCANTOEMAILAPPID;
	email_info->email_msg->sendJobId = currentUiScanJobManage.uiScanJobNum;
	email_info->email_msg->systemJobID = currentUiScanJobManage.systemJobID;
	
	/* etc */
	email_info->isDebugMode = 0;

	DBG_PRINTF_SDH_DEBUG(" Email ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" Email  Scan App -- [ Email Send REQUEST ]\n");
	DBG_PRINTF_SDH_DEBUG(" Email ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" Email  M.Q AppMessageId        : %d\n", email_info->email_msg->sendRequestAppMsgQueueId);
	DBG_PRINTF_SDH_DEBUG(" Email  M.Q systemJobID         : %d\n", email_info->email_msg->systemJobID);
	DBG_PRINTF_SDH_DEBUG(" Email  M.Q sendJobId           : %d\n", email_info->email_msg->sendJobId);	
	DBG_PRINTF_SDH_DEBUG(" Email  sender ID               : %s\n", email_info->username);
	DBG_PRINTF_SDH_DEBUG(" Email  sender PW               : %s\n", email_info->password);
	DBG_PRINTF_SDH_DEBUG(" Email  Sender Addr             : %s\n", email_info->senderEmail);
	DBG_PRINTF_SDH_DEBUG(" Email  Sender Name             : %s\n", email_info->senderName);
	DBG_PRINTF_SDH_DEBUG(" Email  server Addr             : %s\n", email_info->mailhost);
	DBG_PRINTF_SDH_DEBUG(" Email  server Port             : %d\n", email_info->port);
	DBG_PRINTF_SDH_DEBUG(" Email  attachment Path         : %s\n", email_info->attachmentPath);
	DBG_PRINTF_SDH_DEBUG(" Email  Receiver AddrList       : %s\n", email_info->toAddress);
	DBG_PRINTF_SDH_DEBUG(" Email  TitleStr                : %s\n", email_info->subject);
	DBG_PRINTF_SDH_DEBUG(" Email  BodyStr                 : %s\n", email_info->content);
	DBG_PRINTF_SDH_DEBUG(" Email  secureType              : %d\n", email_info->secureType);
	DBG_PRINTF_SDH_DEBUG(" Email  authType                : %d\n", email_info->authType);

	/*
	 ********************************************************************************************************************************************************
	 * <<SCAN TO EMAIL>>
	 * [API] Header File - common/network/apps/ports/include/solution.h
	 * 			int sendMailWrapper(email_info_message_t *email_info_message)
	 ********************************************************************************************************************************************************
	 */
	sendResult = sendMail(email_info);
	
	if(sendResult != 0) {
		DBG_PRINTF_SDH_DEBUG(" [%s]: Email Send[#1] Fail(%d)!\n", __FUNCTION__, sendResult);
	}
	else {
		DBG_PRINTF_SDH_DEBUG(" [%s]: Email Send[#1] Success!\n",__FUNCTION__);
	}
	
	deleteEmailInfo(email_info);
	
	return sendResult;
}
#endif

int scanner_send_to_ftp_request_from_fax( char * ftpID, char* ftpPassword, char* ftpAddress, int ftpPort, char *foler_path, int jobNum )
{
	int sendResult = 0;
	stSolution_Request	sendRequestMsg;

	char ftpServerId[SDH_AUTH_CONF_BASIC_MAX]={0,};
	char ftpServerPW[SDH_AUTH_CONF_BASIC_MAX]={0,};
	char ftpServerFullPath[SDH_AUTH_CONF_FTP_ADDR]={0,};
	int ftpServerPort = 0;
	
	/* Server ID */
	if(strlen(ftpID) != 0) {
        if(strlen(ftpID) > SDH_AUTH_CONF_BASIC_MAX) {
            DBG_PRINTF_SDH_ERR("serverId : Length exceed!! %s\n", ftpID);
            strncpy(ftpServerId, ftpID, sizeof(ftpServerId) - 1);
        }
        else
			strncpy(ftpServerId, ftpID, strlen(ftpID));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverId : NULL\n");
	}

	/* Server Password */
	if(strlen(ftpPassword) != 0) {
        if(strlen(ftpPassword) > SDH_AUTH_CONF_BASIC_MAX) {
            DBG_PRINTF_SDH_ERR("serverPassword : Length exceed!! %s\n", ftpPassword);
            strncpy(ftpServerPW, ftpPassword, sizeof(ftpServerPW) - 1);
        }
        else		
			strncpy(ftpServerPW, ftpPassword, strlen(ftpPassword));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverPassword : NULL\n");
	}
	
	/* Server Full path (Address) */
	if( strlen(ftpAddress) != 0 ) {
        if(strlen(ftpAddress) > SDH_AUTH_CONF_FTP_ADDR)
        {
            DBG_PRINTF_SDH_DEBUG("ftpAddress : Length exceed!! %s\n", ftpAddress);
            strncpy(ftpServerFullPath, ftpAddress, sizeof(ftpServerFullPath) - 1);
        }
        else
			strncpy(ftpServerFullPath, ftpAddress, strlen(ftpAddress));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("ftpAddress : NULL\n");
	}
	
	/* port */
	ftpServerPort = ftpPort;
	
	/* Request Message Queue */
	memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));
	sendRequestMsg.sendRequestAppMsgQueueId = SCANTOEMAILAPPID;
	sendRequestMsg.sendJobId = FAX_SEND_FLAG + SET_FAX_JOB_NO(jobNum) + currentUiScanJobManage.uiScanJobNum;
	
	/* Debug Display */
	DBG_PRINTF_SDH_DEBUG(" FTP ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" FTP  Scan App -- [ FTP Send REQUEST ]\n");
	DBG_PRINTF_SDH_DEBUG(" FTP ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" FTP  M.Q AppMessageId        : %d\n", sendRequestMsg.sendRequestAppMsgQueueId);
	DBG_PRINTF_SDH_DEBUG(" FTP  M.Q sendJobId           : %d\n", sendRequestMsg.sendJobId);
	DBG_PRINTF_SDH_DEBUG(" FTP  ftp ServerId            : %s\n", ftpServerId);
	DBG_PRINTF_SDH_DEBUG(" FTP  ftp ServerPW            : %s\n", ftpServerPW);
	DBG_PRINTF_SDH_DEBUG(" FTP  ftp ServerFullPath      : %s\n", ftpServerFullPath);
	DBG_PRINTF_SDH_DEBUG(" FTP  ftp ServerPort          : %d\n", ftpServerPort);
	
	sendResult = ftpUploadWrapper(ftpServerId, ftpServerPW, ftpServerFullPath, ftpServerPort, foler_path, 0, NULL, 0, &sendRequestMsg,TYPESYSMGR);

	if(sendResult == -1) {
		DBG_PRINTF_SDH_DEBUG(" [%s]: FTP Send[#1] Fail!\n", __FUNCTION__);
	}
	else{
		DBG_PRINTF_SDH_DEBUG(" [%s]: FTP Send[#1] Success!\n",__FUNCTION__);
	}

	//setup the send job type (FAX Auth job)
	currentUiScanJobManage.uiScanSendJobType = SEND_REQUEST_FAX_AUTH_JOB;

	return sendResult;
}

/**
 * \brief Sending scanned image to FTP
 */
int scanner_send_to_ftp(int requestType)
{
	int sendResult = 0;
	stSolution_Request	sendRequestMsg;
	char ftpServerId[SDH_SERVER_ID_MAX]={0,};
	char ftpServerPW[SDH_SERVER_PASSWORD_MAX]={0,};
	char ftpServerFullPath[SDH_SERVER_PATH_MAX]={0,};
	int ftpServerPort = 0;
	
	/* Server ID */
	if(strlen(uiJobSettings.sendToServerInfo.serverId) != 0) {
		strncpy(ftpServerId, uiJobSettings.sendToServerInfo.serverId, sizeof(ftpServerId));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverId : NULL\n");
	}

	/* Server Password */
	if(strlen(uiJobSettings.sendToServerInfo.serverPassword) != 0) {
		strncpy(ftpServerPW, uiJobSettings.sendToServerInfo.serverPassword, sizeof(ftpServerPW));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverPassword : NULL\n");
	}
	
	/* Server Full path (Address + Path) */
	DBG_PRINTF_SDH_DEBUG("FTP ServerAddress : %s\n", uiJobSettings.sendToServerInfo.serverAddress);
	DBG_PRINTF_SDH_DEBUG("FTP ServerPath    : %s\n", uiJobSettings.sendToServerInfo.serverPath);
	
	if( strlen(uiJobSettings.sendToServerInfo.serverAddress) != 0 ) {
		sprintf(ftpServerFullPath, "%s/", uiJobSettings.sendToServerInfo.serverAddress);
	}
	if(strlen(uiJobSettings.sendToServerInfo.serverPath) != 0) {
		strcat(ftpServerFullPath, uiJobSettings.sendToServerInfo.serverPath);
	}
	
	/* port */
	ftpServerPort = uiJobSettings.sendToServerInfo.serverPort;
	
	/* make .end file which name is same as original scanned file, and send together */
	if( uiJobSettings.isPersonalScanJob == TRUE ) {
		scanner_make_end_file( gUI_Image_Folder, currentUiScanJobManage.uiScanJobTempFolderName );
	}
	
	/* Request Message Queue */
	memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));
	sendRequestMsg.sendRequestAppMsgQueueId = SCANTOEMAILAPPID;
	sendRequestMsg.sendJobId = currentUiScanJobManage.uiScanJobNum;
	sendRequestMsg.systemJobID = currentUiScanJobManage.systemJobID;
	
	/* Debug Display */
	DBG_PRINTF_SDH_DEBUG(" FTP ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" FTP  Scan App -- [ FTP Send REQUEST ]\n");
	DBG_PRINTF_SDH_DEBUG(" FTP ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" FTP  M.Q AppMessageId        : %d\n", sendRequestMsg.sendRequestAppMsgQueueId);
	DBG_PRINTF_SDH_DEBUG(" FTP  M.Q systemJobID         : %d\n", sendRequestMsg.systemJobID);
	DBG_PRINTF_SDH_DEBUG(" FTP  M.Q sendJobId           : %d\n", sendRequestMsg.sendJobId);
	DBG_PRINTF_SDH_DEBUG(" FTP  ftp ServerId            : %s\n", ftpServerId);
	DBG_PRINTF_SDH_DEBUG(" FTP  ftp ServerPW            : %s\n", ftpServerPW);
	DBG_PRINTF_SDH_DEBUG(" FTP  ftp ServerFullPath      : %s\n", ftpServerFullPath);
	DBG_PRINTF_SDH_DEBUG(" FTP  image folder            : %s\n", gUI_Image_Folder);	
	DBG_PRINTF_SDH_DEBUG(" FTP  ftp ServerPort          : %d\n", ftpServerPort);
	
	/*
	 ********************************************************************************************************************************************************
	 * <<SCAN TO FTP>>
	 * [API] Header File - common/network/apps/ports/include/solution.h
	 * 					int ftpUploadWrapper(const char *username, const char *password, const char *destination, int port, 
	 * 							const char *filePath, int serverType, const char *proxyserver, int isDebugMode, stSolution_Request ftp_mqueue_data)
	 * 
	 * 			Example - ftpUploadWrapper(FTP?�버ID, FTP?�호, FTP?�버주소, FTP?�버?�트, 첨�??�일경로, 0, NULL, 0, ftp_mqueue_data);
	 ********************************************************************************************************************************************************
	 */ 
#if SDH_USE_SEND_FTP_FUNCTION_TEST
	sendResult = ftpUploadWrapper("ad", "adad", "ftp://10.15.20.83/", 21, gUI_Image_Folder, 0, NULL, 1, &sendRequestMsg,TYPESYSMGR);
#else
	if( requestType == SEND_REQUEST_NORMAL ) {
		sendResult = ftpUploadWrapper(ftpServerId, ftpServerPW, ftpServerFullPath, ftpServerPort, gUI_Image_Folder, 0, NULL, 0, &sendRequestMsg,TYPESYSMGR);
	}
	else if( requestType == SEND_REQUEST_CONNECT_TEST ) {
		sendRequestMsg.sendRequestAppMsgQueueId = SEND_REQ_APP_CONNTEST_UI;
		sendResult = ftpUploadWrapper(ftpServerId, ftpServerPW, ftpServerFullPath, ftpServerPort, gUI_Image_Folder, 0, NULL, 0, &sendRequestMsg,TYPECONNTEST_UI);		
	}
#endif

	if(sendResult == -1) {
		DBG_PRINTF_SDH_DEBUG(" [%s]: FTP Send[#1] Fail!\n", __FUNCTION__);
	}
	else{
		DBG_PRINTF_SDH_DEBUG(" [%s]: FTP Send[#1] Success!\n",__FUNCTION__);
	}
	
	return sendResult;
}

/**
 * \brief Sending scanned image to SMB
 */
int scanner_send_to_smb(int requestType)
{
	int sendResult = 0;
	stSolution_Request	sendRequestMsg;	
	char smbServerId[SDH_SERVER_ID_MAX]={0,};
	char smbServerPW[SDH_SERVER_PASSWORD_MAX]={0,};
	char smbServerAddress[SDH_SERVER_PATH_MAX]={0,};
	char smbServerPath[SDH_SERVER_PATH_MAX]={0,};
	int smbPort = 0;
	int isDebugMode = 0;
	
	/* Server ID */
	if(strlen(uiJobSettings.sendToServerInfo.serverId) != 0) {
		strncpy(smbServerId, uiJobSettings.sendToServerInfo.serverId, sizeof(smbServerId));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverId : NULL\n");
	}

	/* Server Password */
	if(strlen(uiJobSettings.sendToServerInfo.serverPassword) != 0) {
		strncpy(smbServerPW, uiJobSettings.sendToServerInfo.serverPassword, sizeof(smbServerPW));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverPassword : NULL\n");
	}
	
	/* Server Address */
	if(strlen(uiJobSettings.sendToServerInfo.serverAddress) != 0) {
		strncpy(smbServerAddress, uiJobSettings.sendToServerInfo.serverAddress, sizeof(smbServerAddress));	
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverAddress : NULL\n");
	}
	
	/* Server Path */
	if(strlen(uiJobSettings.sendToServerInfo.serverPath) != 0) {
		strncpy(smbServerPath, uiJobSettings.sendToServerInfo.serverPath, sizeof(smbServerPath));	
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverPath : NULL\n");
	}
	
	/* SMB Port */
	smbPort = uiJobSettings.sendToServerInfo.serverPort;
	
	/* Request Message Queue */
	memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));
	sendRequestMsg.sendRequestAppMsgQueueId = SCANTOEMAILAPPID;
	sendRequestMsg.sendJobId = currentUiScanJobManage.uiScanJobNum;
	sendRequestMsg.systemJobID = currentUiScanJobManage.systemJobID;
	
	/* Debug Display */
	DBG_PRINTF_SDH_DEBUG(" SMB ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" SMB  Scan App -- [ SMB Send REQUEST ]\n");
	DBG_PRINTF_SDH_DEBUG(" SMB ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" SMB  M.Q AppMessageId   : %d\n", sendRequestMsg.sendRequestAppMsgQueueId);
	DBG_PRINTF_SDH_DEBUG(" SMB  M.Q systemJobID    : %d\n", sendRequestMsg.systemJobID);
	DBG_PRINTF_SDH_DEBUG(" SMB  M.Q sendJobId      : %d\n", sendRequestMsg.sendJobId);
	DBG_PRINTF_SDH_DEBUG(" SMB  smbServerId        : %s\n", smbServerId);
	DBG_PRINTF_SDH_DEBUG(" SMB  smbServerPW        : %s\n", smbServerPW);
	DBG_PRINTF_SDH_DEBUG(" SMB  smbServerAddress   : %s\n", smbServerAddress);
	DBG_PRINTF_SDH_DEBUG(" SMB  smbPort            : %d\n", smbPort);	
	DBG_PRINTF_SDH_DEBUG(" SMB  smbServerPath      : %s\n", smbServerPath);
	DBG_PRINTF_SDH_DEBUG(" SMB  gUI_Image_Folder   : %s\n", gUI_Image_Folder);

	/*
	 ********************************************************************************************************************************************************
	 * <<SCAN TO SMB>>
	 * [API] Header File - common/network/apps/ports/include/solution.h
	 ********************************************************************************************************************************************************
	 */
#if SDH_USE_SEND_SMB_FUNCTION_TEST
	isDebugMode = 1;
	sendResult = smbUploadWrapper("nexus87", "1234", "smb://10.15.102.28/Public", 445, "", gUI_Image_Folder, isDebugMode, &sendRequestMsg,TYPESYSMGR);
#else
	if( requestType == SEND_REQUEST_NORMAL ) {
		sendResult = smbUploadWrapper(smbServerId, smbServerPW, smbServerAddress, smbPort, smbServerPath, gUI_Image_Folder, isDebugMode, &sendRequestMsg,TYPESYSMGR);
	}
	else if( requestType == SEND_REQUEST_CONNECT_TEST ) {	
		sendRequestMsg.sendRequestAppMsgQueueId = SEND_REQ_APP_CONNTEST_UI;
		sendResult = smbUploadWrapper(smbServerId, smbServerPW, smbServerAddress, smbPort, smbServerPath, gUI_Image_Folder, isDebugMode, &sendRequestMsg,TYPECONNTEST_UI);
	}
#endif

	if(sendResult == -1) {
		DBG_PRINTF_SDH_DEBUG(" [%s]: SMB Send[#1] Fail!\n", __FUNCTION__);
	}
	else {
		DBG_PRINTF_SDH_DEBUG(" [%s]: SMB Send[#1] Success!\n",__FUNCTION__);
	}
	
	return sendResult;
}

/**
 * \brief Sending scanned image to WebDav
 */
int scanner_send_to_webdav(int requestType)
{
	int sendResult = 0;
	stSolution_Request	sendRequestMsg;
	char webDavServerId[SDH_SERVER_ID_MAX]={0,};
	char webDavServerPW[SDH_SERVER_PASSWORD_MAX]={0,};
	char webDavServerFullPath[SDH_SERVER_PATH_MAX]={0,};
	int webDavServerPort = 0;
	
	/* Server ID */
	if(strlen(uiJobSettings.sendToServerInfo.serverId) != 0) {
		strncpy(webDavServerId, uiJobSettings.sendToServerInfo.serverId, sizeof(webDavServerId));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverId : NULL\n");
	}

	/* Server Password */
	if(strlen(uiJobSettings.sendToServerInfo.serverPassword) != 0) {
		strncpy(webDavServerPW, uiJobSettings.sendToServerInfo.serverPassword, sizeof(webDavServerPW));
	}
	else {
		DBG_PRINTF_SDH_DEBUG("serverPassword : NULL\n");
	}
	
	/* Server Full path (Address + Path) */
	DBG_PRINTF_SDH_DEBUG("webDav ServerAddress : %s\n", uiJobSettings.sendToServerInfo.serverAddress);
	DBG_PRINTF_SDH_DEBUG("webDav ServerPath    : %s\n", uiJobSettings.sendToServerInfo.serverPath);
	
	if( strlen(uiJobSettings.sendToServerInfo.serverAddress) != 0 ) {
		sprintf(webDavServerFullPath, "%s/", uiJobSettings.sendToServerInfo.serverAddress);
	}
	if(strlen(uiJobSettings.sendToServerInfo.serverPath) != 0) {
		strcat(webDavServerFullPath, uiJobSettings.sendToServerInfo.serverPath);
	}
	
	/* port */
	webDavServerPort = uiJobSettings.sendToServerInfo.serverPort;
	
	/* Request Message Queue */
	memset(&sendRequestMsg, 0x00, sizeof(stSolution_Request));
	sendRequestMsg.sendRequestAppMsgQueueId = SCANTOEMAILAPPID;
	sendRequestMsg.sendJobId = currentUiScanJobManage.uiScanJobNum;
	sendRequestMsg.systemJobID = currentUiScanJobManage.systemJobID;
	
	/* Debug Display */
	DBG_PRINTF_SDH_DEBUG(" webDav ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" webDav  Scan App -- [ WebDav Send REQUEST ]\n");
	DBG_PRINTF_SDH_DEBUG(" webDav ========================================================\n");
	DBG_PRINTF_SDH_DEBUG(" webDav  M.Q AppMessageId        : %d\n", sendRequestMsg.sendRequestAppMsgQueueId);
	DBG_PRINTF_SDH_DEBUG(" webDav  M.Q systemJobID         : %d\n", sendRequestMsg.systemJobID);
	DBG_PRINTF_SDH_DEBUG(" webDav  M.Q sendJobId           : %d\n", sendRequestMsg.sendJobId);
	DBG_PRINTF_SDH_DEBUG(" webDav  webDav ServerId            : %s\n", webDavServerId);
	DBG_PRINTF_SDH_DEBUG(" webDav  webDav ServerPW            : %s\n", webDavServerPW);
	DBG_PRINTF_SDH_DEBUG(" webDav  webDav ServerFullPath      : %s\n", webDavServerFullPath);
	DBG_PRINTF_SDH_DEBUG(" webDav  webDav ServerPort          : %d\n", webDavServerPort);
	
	/*
	 ********************************************************************************************************************************************************
	 * <<SCAN TO WebDav>>
	 * [API] Header File - common/network/apps/ports/include/solution.h
	 * 					int webDAVUploadWrapper(webDAVServerId, webDAVServerPW, webDAVServerFullPath, webDAVServerPort, gUI_Image_Folder, 0, NULL, 0, &sendRequestMsg,TYPESYSMGR); 
	 * 			Example - webDAVUploadWrapper(?�버ID, ?�호, ?�버주소, ?�버?�트, 첨�??�일경로, 0, NULL, 0, mqueue_data);
	 ********************************************************************************************************************************************************
	 */ 
#if SDH_USE_SEND_WEBDAV_FUNCTION_TEST
	sendResult = webDAVUploadWrapper("ad", "adad", "webdav address/", 21, gUI_Image_Folder, 0, NULL, 1, &sendRequestMsg,TYPESYSMGR);
#else
	if( requestType == SEND_REQUEST_NORMAL ) {
		sendResult = webDAVUploadWrapper(webDavServerId, webDavServerPW, webDavServerFullPath, webDavServerPort, gUI_Image_Folder, 0, NULL, 0, &sendRequestMsg,TYPESYSMGR);
	}
	else if( requestType == SEND_REQUEST_CONNECT_TEST ) {
		sendRequestMsg.sendRequestAppMsgQueueId = SEND_REQ_APP_CONNTEST_UI;
		sendResult = webDAVUploadWrapper(webDavServerId, webDavServerPW, webDavServerFullPath, webDavServerPort, gUI_Image_Folder, 0, NULL, 0, &sendRequestMsg,TYPECONNTEST_UI);		
	}
#endif

	if(sendResult == -1) {
		DBG_PRINTF_SDH_DEBUG(" [%s]: webDav Send[#1] Fail!\n", __FUNCTION__);
	}
	else{
		DBG_PRINTF_SDH_DEBUG(" [%s]: webDav Send[#1] Success!\n",__FUNCTION__);
	}
	
	return sendResult;
}


/**
 * \brief scanner_make_scan_basic_foler
 * 		- Make folder in 'RamDisk' for saving scan image files
 */
int scanner_make_scan_basic_foler()
{
	scanner_make_image_folder(gUI_ramDiskScanBasicPath, FALSE);
	
	return 0;
}

/**
 * \brief scanner_make_image_folder
 */
int scanner_make_image_folder(char *folderPath, bool removemode)
{
	if( 0 != chdir(folderPath) ) {
		if( 0 != mkdir(folderPath, 0777) ) {
			DBG_PRINTF_SDH_DEBUG("%s : FAIL! path:%s\n", __FUNCTION__, folderPath);
			return -1;
		}
		else {
			DBG_PRINTF_SDH_DEBUG("%s : folderPath is %s\n", __FUNCTION__, folderPath);
			return 0;
		}
	}
	else{
		if (removemode)
		{
			char sysCmd[MAX_SYSTEM_CMD_LENGTH] = {0x00,};
			sprintf(sysCmd, "rm \"%s\"/*", folderPath);
			DBG_PRINTF_SDH_DEBUG("%s : folder is exist : %s\n", __FUNCTION__, sysCmd);
			system(sysCmd);
		}
	}
	
	return 0;
}

void scanner_make_image_folder_using_system_cmd(char *folderPath)
{
	if( 0 != chdir(folderPath) ) {
		char *sysCmd = malloc(SDH_FILEPATH_MAX);
		memset(sysCmd, 0x00, SDH_FILEPATH_MAX);
		sprintf(sysCmd, "mkdir -p \"%s\"", folderPath);
		DBG_PRINTF_SDH_DEBUG("%s : folderPath is : %s\n", __FUNCTION__, folderPath);
		system(sysCmd);
		free(sysCmd);
	}
}
/**
 * \brief Setup a scan erase
 *
 * Setup a scan erase
 *
 */
#if SDH_USE_ERASE_BORDER
static scan_err_t setup_scan_eraseborder( UISCAN_JOB_SETTINGS *settings )
{
    scan_err_t scerr = SCANERR_NONE;

	uint32_t eraseLeftPixel = 0;
	uint32_t eraseRightPixel = 0;
	uint32_t eraseTopPixel = 0;
	uint32_t eraseBottomPixel = 0;
	
	uint32_t user_width_hinchs = 0;
	uint32_t user_height_hinchs = 0;
	uint32_t user_width_pixel = 0;
	uint32_t user_height_pixel = 0;

	user_width_hinchs = settings->scan_window.right - settings->scan_window.left;
	user_height_hinchs = settings->scan_window.bottom - settings->scan_window.top;
	user_width_pixel = (user_width_hinchs * settings->Resolution)/100;
	user_height_pixel = (user_height_hinchs * settings->Resolution)/100;
	
	eraseLeftPixel = ((settings->Resolution*(DEFAULT_BORDER_LEFT_MM + settings->eraseBorderParam.userInputLeft)*100)) / (2540);
	eraseRightPixel = ((settings->Resolution*(DEFAULT_BORDER_LEFT_MM + settings->eraseBorderParam.userInputRight)*100)) / (2540);
	eraseTopPixel = ((settings->Resolution*(DEFAULT_BORDER_LEFT_MM + settings->eraseBorderParam.userInputTop)*100)) / (2540);
	eraseBottomPixel = ((settings->Resolution*(DEFAULT_BORDER_LEFT_MM + settings->eraseBorderParam.userInputBottom)*100)) / (2540);

	DBG_PRINTF_SDH_DEBUG("%s L Pixel (%d)\n", __FUNCTION__, eraseLeftPixel);
	DBG_PRINTF_SDH_DEBUG("%s R Pixel (%d)\n", __FUNCTION__, eraseRightPixel);
	DBG_PRINTF_SDH_DEBUG("%s T Pixel (%d)\n", __FUNCTION__, eraseTopPixel);
	DBG_PRINTF_SDH_DEBUG("%s B Pixel (%d)\n", __FUNCTION__, eraseBottomPixel);
	
	settings->eraseBorderParam.eraseLeftPixel = eraseLeftPixel;
	settings->eraseBorderParam.eraseRightPixel = eraseRightPixel;
	settings->eraseBorderParam.eraseTopPixel = eraseTopPixel;
	settings->eraseBorderParam.eraseBottomPixel = eraseBottomPixel;
	
	return scerr;
}
#endif //SDH_USE_ERASE_BORDER

/**
 * \brief Setup a scan area
 *
 * Setup a scan area.
 *
 */
static scan_err_t setup_scan_area( struct scanvars *sv, UISCAN_JOB_SETTINGS *settings )
{
    int32_t user_x, user_y;
    int32_t user_width, user_height;
    scan_err_t scerr = SCANERR_NONE;
    struct scan_flatbed_margins fb_margins;
    struct scan_adf_margins adf_margins;
    uint32_t dpi_horiz, dpi_vert;

    /* all these numbers are in 1/100" */

	//Sync Margin Table with DB
	scanner_service_sync_marginTable();

    user_x = settings->scan_window.left;
    user_y = settings->scan_window.top;

    user_width = settings->scan_window.right - settings->scan_window.left;
    user_height = settings->scan_window.bottom - settings->scan_window.top;

    DBG_PRINTF_SDH_DEBUG( "%s scan_window : left=%d top=%d right=%d bottom=%d\n", __FUNCTION__,
          settings->scan_window.left,
          settings->scan_window.top,
          settings->scan_window.right,
          settings->scan_window.bottom );
    DBG_PRINTF_SDH_DEBUG( "%s user_val : x=%d y=%d width=%d height=%d\n",
          __FUNCTION__, user_x, user_y, user_width, user_height );

    scanvar_get_hw_dpi( sv, &dpi_horiz, &dpi_vert );

    /* We need to get the platform margin data based on the document source, dpi,
     * and color mode.
     * Adjust platform margins (need to clip off bits of the mech)
     */
    if (sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED) {
        scerr = scanmech_get_fb_margins( sv->dpi,
                                         sv->cmode,
                                         &fb_margins );
                                         
        if( scerr == SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG("%s: Flatbed LEFT/TOP_Margin_: x=%d, y=%d\n", __FUNCTION__,
                 fb_margins.left_margin_hinches_x, fb_margins.top_margin_hinches_y);

            user_x += fb_margins.left_margin_hinches_x;
            user_y += fb_margins.top_margin_hinches_y;
		}
        else {
			DBG_PRINTF_SDH_DEBUG("Can't get FB Margin. ERR: %d\n", scerr);
		}
		
		if(user_height > MAX_SCAN_FB_HEIGHT_HINCHES) {
			user_height = MAX_SCAN_FB_HEIGHT_HINCHES;
			DBG_PRINTF_SDH_DEBUG("%s: Flatbed height is limited to : %d\n", __FUNCTION__, user_height);
		}
		DBG_PRINTF_SDH_DEBUG( "%s user x=%d y=%d width=%d height=%d\n", __FUNCTION__, user_x, user_y, user_width, user_height );
    }
    else if (sv->doc_src == SCAN_DOCUMENT_SOURCE_ADF || sv->doc_src  == SCAN_DOCUMENT_SOURCE_ADF_DUPLEX) {
        scerr = scanmech_get_adf_margins( sv->dpi,
                                          sv->cmode,
                                          &adf_margins );

        if( scerr==SCANERR_NONE ) {

            DBG_PRINTF_SDH_DEBUG("%s: ADF left_mar=%d\n", __FUNCTION__, adf_margins.left_margin_hinches);

            user_x += adf_margins.left_margin_hinches;
			/* SINDOH */
			//For ADF top margin, we use margin table located in Kernel
			/*
			 *************************************************************************************************************
			 * user_y : 'user_y' value is no effection in ADF, 
			 * If we want to adjust margin of ADF we have to setup the 'margin table' located in kernel
			 *************************************************************************************************************
			 */
		}
        else {
			DBG_PRINTF_SDH_DEBUG("Can't get ADF Margin. ERR: %d\n", scerr);
		}
		
		/// SINDOH Added for ADF Offset for smaller than Letter of Hori size.
		/// ADF Offset value is set in <SysServiceMgr - ScanAppService>
		user_x += settings->input_horizontal_adfOffset;
    }

    DBG_PRINTF_SDH_DEBUG( "scanvar_set_area : x=%d y=%d width=%d height=%d\n", user_x, user_y, user_width, user_height );
    scerr = scanvar_set_area( sv, user_x, user_y, user_width, user_height );
    
    //Update Scan Area into Job Setting
	settings->scan_window.left		= user_x;	 	
	settings->scan_window.top		= user_y;    
	settings->scan_window.right	= user_width;
	settings->scan_window.bottom	= user_height;
	settings->ScannableArea.right	= user_width;

	if(settings->sizecode == MEDIASIZE_A3) {
		settings->scan_window.right	= 1169;
		settings->scan_window.bottom = 1650;
	}
	else if(settings->sizecode == MEDIASIZE_11X17) {
		settings->scan_window.right	= 1100;
		settings->scan_window.bottom = 1700;
	}
	else if((settings->sizecode == MEDIASIZE_LEGAL) && (g_scan_started_from_adf == FALSE)) {
		settings->scan_window.right	= 850;
		settings->scan_window.bottom = 1400;
	}

#if SDH_USE_ERASE_BORDER
	//Setup scan erase border
	if(uiJobSettings.scanActionType == UI_SCAN_MERGEJOB_NONE) {
		setup_scan_eraseborder(settings);
	}
	else {
		DBG_PRINTF_SDH_DEBUG("This is merge scan job, so we don't Setup erase border\n");
	}
#endif

    return scerr;
}

/**
 * \brief choose_document_source
 *
 * check document source
 *
 */
static scan_err_t choose_document_source( struct scanvars *sv,
                                          UISCAN_JOB_SETTINGS *job_settings,
                                          bool adf_paper_present )
{
    ASSERT( sv!=NULL );

    /* davep 27-Jul-2009 ; if we don't have an ADF, hardwire to flatbed no
     * matter what the host says (short, sweet, easy fix that I'll probably get
     * yelled at for in a couple years)
     */
    if( !scan_capabilities.has_adf ) {
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
        DBG_PRINTF_SDH_DEBUG("  scan doesn't have ADF\n");
        return 0;
    }
	
    switch( job_settings->scanSource ) {
    case SCAN_JOB_SETTINGS_DOC_SRC_AUTO :
		DBG_PRINTF_SDH_DEBUG( "  scan doc source : AUTO\n" );
        if( adf_paper_present ) {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF);
			DBG_PRINTF_SDH_DEBUG( "  scan doc source : ADF_1side : by AUTO\n" );
        }
        else {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
			DBG_PRINTF_SDH_DEBUG( "  scan doc source : FB : by AUTO\n" );
        }
        break;

    case SCAN_JOB_SETTINGS_DOC_SRC_FLATBED :
        scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
		DBG_PRINTF_SDH_DEBUG( "  scan doc source : FB\n" );
        break;

    case SCAN_JOB_SETTINGS_DOC_SRC_ADF :
        if( adf_paper_present ) {
			scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF);
			DBG_PRINTF_SDH_DEBUG( "  scan doc source : ADF_1side\n" );
		}
		else {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
			DBG_PRINTF_SDH_DEBUG( "  scan doc source : changed ADF -> FB\n" );
		}
        break;

    case SCAN_JOB_SETTINGS_DOC_SRC_ADF_DUPLEX:
		if( adf_paper_present ) {
			scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF_DUPLEX);
			DBG_PRINTF_SDH_DEBUG( "  scan doc source : ADF_DUPLEX\n" );
		}
		else {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
			DBG_PRINTF_SDH_DEBUG( "  scan doc source : changed ADF_DUPLEX -> FB\n" );
		}
        break;

    default :
        DBG_PRINTF_SDH_DEBUG( "%s ignoring unknown docsrc=%d from host\n",
              __FUNCTION__, job_settings->scanSource );

        if( adf_paper_present ) {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF);
        }
        else {
            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
        }
        break;
    }

    return 0;
}

static bool adf_paper_is_present( void )
{
    /* REVISIT: the OBA design used to store the ADF sensor state in the data
     *   store, which still may make sense.  Problem is the ds has changed and
     *   we haven't had a chance to hook things back up.  Go straight to the ADF
     *   sensor for now (a stub routine will take care of things if a physical sensor
     *   isn't present).
     */

#if 1
    struct scan_platform_capabilities scan_capabilities;

    scancap_get_capabilities( &scan_capabilities );
    if( !scan_capabilities.has_adf ) {
        return false;
    }

    return adf_sensor_paper_present();
#else
    struct scan_platform_capabilities scan_capabilities;
    uint8_t paperval=0;

    scancap_get_capabilities( &scan_capabilities );
    if( !scan_capabilities.has_adf ) {
        return false;
    }
//    scands_get_paper_present(&paperval);
    if (paperval == 1)
        return true;
    else
        return false;
#endif
}

bool scanner_check_adf_paper(void)
{
	return adf_paper_is_present();
}

scan_err_t allocate_global_scanvar_for_uiscan( UISCAN_JOB_SETTINGS *job_settings )
{
    /* be paranoid about memory leaks; if user sends down multiple set config
     * requests, quietly throw away the previous one
     */
    if( g_scanvars != NULL ) {
        /* free the previous; */
        DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars\n" );
        scanvar_delete( &g_scanvars );
    }

    if( job_settings->ScanType == e_ColorScan) {
        g_scanvars = scanplat_sv_color_scan_new();
    }
    else if( job_settings->ScanType == e_MonochromeScan ) {
        g_scanvars = scanplat_sv_mono_scan_new();
    }
    else {
        return SCANERR_INVALID_PARAM;
    }

    if( g_scanvars==NULL ) {
        DBG_PRINTF_SDH_DEBUG( "out of memory for scanap scanvar\n" );
        return SCANERR_OUT_OF_MEMORY;
    }

    return SCANERR_NONE;
}

/**
 * \brief Construct a scanvar for an upcoming scan.
 *
 * If there is already an existing scanvar, delete it. Allows for recovery from
 * weird protocol behavior (usually the host should send only one
 * SetScanJobSettings message).
 *
 * \param scan_type The type of scan we are doing.
 * \author David Poole
 * \date 30-Dec-2005
 *
 * davep 14-Jun-2012 ; roll a bunch of code from message loop into this
 *                     function so we have better error checking
 */
static scan_err_t setup_scan_params( struct scanvars *sv, UISCAN_JOB_SETTINGS *job_settings )
{
    scan_err_t scerr;

    ASSERT( sv!=NULL );

	DBG_PRINTF_SDH_DEBUG("  Resolution    = %ld\n", job_settings->Resolution);
	DBG_PRINTF_SDH_DEBUG("  Orig Size     = %ld\n", job_settings->sizecode);
	DBG_PRINTF_SDH_DEBUG("  Destination   = %ld\n", job_settings->scanToDestination);
	DBG_PRINTF_SDH_DEBUG("  fileFormat    = %ld\n", job_settings->fileFormat);
	DBG_PRINTF_SDH_DEBUG("  jpegQuality   = %ld\n", job_settings->jpegQuality);
	DBG_PRINTF_SDH_DEBUG("  flags         = %ld\n", job_settings->flags);	

    DBG_PRINTF_SDH_DEBUG("  Gamma = %ld\n",job_settings->Gamma);
    if (job_settings->Gamma >= 10 && job_settings->Gamma <= 100) {
        scerr = scanvar_set_gamma(sv, job_settings->Gamma);
        if( scerr != SCANERR_NONE ) {
            return scerr;
        }
    }

    DBG_PRINTF_SDH_DEBUG("  Brightness = %ld\n",job_settings->Brightness);
    scerr = scanvar_set_brightness(sv, job_settings->Brightness);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    DBG_PRINTF_SDH_DEBUG("  Contrast = %ld\n",job_settings->Contrast);
    scerr = scanvar_set_contrast( sv, job_settings->Contrast);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
#if 0	// del.lsh not to clear LUT set before
    DBG_PRINTF_SDH_DEBUG("  Sharpness = %ld\n",job_settings->Sharp);
    scerr = scanvar_set_sharpness( sv, job_settings->Sharp);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
#endif
    DBG_PRINTF_SDH_DEBUG("  Requested Resolution = %ld\n",job_settings->Resolution);
    scerr = scanvar_set_dpi( sv, job_settings->Resolution );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    DBG_PRINTF_SDH_DEBUG("  XYScale = %ld %ld %ld %ld\n",
         job_settings->XYScale.x_numerator,job_settings->XYScale.x_denominator,
         job_settings->XYScale.y_numerator,job_settings->XYScale.y_denominator);
    scerr = scanvar_set_xyscale( sv,
                                 job_settings->XYScale.x_numerator,job_settings->XYScale.x_denominator,
                                 job_settings->XYScale.y_numerator,job_settings->XYScale.y_denominator );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
#if 0	// del.lsh not to clear LUT set before
    DBG_PRINTF_SDH_DEBUG("  Sharp = %ld\n",job_settings->Sharp);
    scerr = scanvar_set_sharpness( sv, job_settings->Sharp );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
#endif
    DBG_PRINTF_SDH_DEBUG("  BitsPerPixel = %ld\n",job_settings->BitsPerPixel);
    scerr = scanvar_set_bpp( sv, job_settings->BitsPerPixel);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    g_scan_started_from_adf = adf_paper_is_present();
    DBG_PRINTF_SDH_DEBUG("  %s: g_scan_started_from_adf=%d\n", __FUNCTION__, g_scan_started_from_adf);

    scerr = choose_document_source( sv, job_settings, g_scan_started_from_adf );
    if( scerr != 0 ) {
        return scerr;
    }

    DBG_PRINTF_SDH_DEBUG("  scan_window: top: %ld, left: %ld, bottom: %ld, right: %ld\n",
         job_settings->scan_window.top,
         job_settings->scan_window.left,
         job_settings->scan_window.bottom-job_settings->scan_window.top,
         job_settings->scan_window.right-job_settings->scan_window.left);

    scerr = setup_scan_area( sv, job_settings);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    DBG_PRINTF_SDH_DEBUG("  ScanType = %ld\n",job_settings->ScanType);
    if (job_settings->ScanType == e_MonochromeScan) {
        scanvar_set_pieout( sv, PIE_PIXEL_MONO);
    }
    else {
        switch((scan_data_type)job_settings->DataType)
        {
        case SCAN_DATA_TYPE_PLANE:
            scanvar_set_pieout( sv, PIE_PIXEL_3_PLANES);
            break;
        case SCAN_DATA_TYPE_XRGB:
            scanvar_set_pieout(sv, PIE_PIXEL_XRGB);
            break;
        case SCAN_DATA_TYPE_MONO:
            scanvar_set_pieout(sv, PIE_PIXEL_MONO);
            break;
        case SCAN_DATA_TYPE_RGBX:
            scanvar_set_pieout(sv, PIE_PIXEL_RGBX);
            break;
        case SCAN_DATA_TYPE_RGB:
            scanvar_set_pieout(sv, PIE_PIXEL_RGB);
            break;
        default:
            DBG_PRINTF_SDH_DEBUG("SCAN: Invalid Scan data type value %lx\n",
                 job_settings->DataType);
            return SCANERR_INVALID_PARAM;
            break;
        }
    }

    return SCANERR_NONE;
}


static scan_err_t buffer_write( FILE *f, uint8_t* buf, uint32_t buflen_bytes )
{
    int retcode;
    uint8_t *ptr;
    uint32_t bytes_remain;

    ASSERT( buflen_bytes );
    ASSERT(f);

    ptr = buf;
    bytes_remain = buflen_bytes;

    while( bytes_remain > 0 ) {
        retcode = fwrite( ptr, 1, bytes_remain, f );
        if( retcode <= 0 ) {
            DBG_PRINTF_SDH_DEBUG( "%s write failed errno=%d\n", __FUNCTION__, errno );
            return SCANERR_MSG_FAIL;
        }

        ptr += retcode;
        bytes_remain -= retcode;
    }

    return SCANERR_NONE;
}

static void htons_data( unsigned char *buf, uint32_t buf_num_bytes )
{
    uint16_t *ptr16, *endptr16;

    ptr16 = (uint16_t*)buf;
    endptr16 = (uint16_t *)(buf + buf_num_bytes);

    while( ptr16 < endptr16 ) {
        *ptr16 = htons(*ptr16);
        ptr16++;
    }
}

int scanner_set_file_write_error(int errorType)
{
	uiscan_stats.file_write_error = 1;
	scanapp_log_event(SCANAPP_EVENT_SCAN_FAILED_FILE_WRITE);
	return 0;
}

extern void ScanSetJobFileName(char *FileName);
void make_output_filename( char outfilename[], int fileformat, int page_number, scan_data_type dtype, uint8_t page_side  )
{	
    int retcode;
    char *extension;
	char outfilename_base[FILENAME_MAX];

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	DBG_PRINTF_SDH_DEBUG("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	/* Setup Scan Path */
	if (( uiJobSettings.scanToDestination == UI_FILE_DEST_USBTHUMB ) &&  (fileformat == UI_FILE_JPEG) )
	{
		switch (uiJobSettings.fileFormat)
		{
		    case UI_FILE_PDF:
		    case UI_FILE_PDF_MULTI_PAGE:
				if( uiJobSettings.isPersonalScanJob == TRUE )
					sprintf( outfilename_base, "%s/S%s", TEMP_FOLDER_PATH, currentUiScanJobManage.uiScanJobTempFolderName);
				else
					sprintf( outfilename_base, "%s/%s", TEMP_FOLDER_PATH, currentUiScanJobManage.uiScanJobTempFolderName);
				break;
			default :
				if( uiJobSettings.isPersonalScanJob == TRUE )
					sprintf( outfilename_base, "%s/S%s", gUI_Image_Folder, currentUiScanJobManage.uiScanJobTempFolderName );
				else
					sprintf( outfilename_base, "%s/%s", gUI_Image_Folder, currentUiScanJobManage.uiScanJobTempFolderName );
				break;
		}
	}
	else
	{
		if ((uiJobSettings.isPersonalScanJob == TRUE ) &&
			((( uiJobSettings.scanToDestination == UI_FILE_DEST_USBTHUMB) && (strlen(uiJobSettings.saveToFileInfo.fileName) == 0 ))
			|| (( uiJobSettings.scanToDestination == UI_FILE_DEST_EMAIL )&& (strlen(uiJobSettings.sendToEmailInfo.fileName) == 0))
			|| ((( uiJobSettings.scanToDestination == UI_FILE_DEST_FTP ) || ( uiJobSettings.scanToDestination == UI_FILE_DEST_SMB )
				|| ( uiJobSettings.scanToDestination == UI_FILE_DEST_WEBDAV )) && (strlen(uiJobSettings.sendToServerInfo.fileName) == 0))))
			sprintf( outfilename_base, "%s/S%s", gUI_Image_Folder, currentUiScanJobManage.uiScanJobTempFolderName );
		else
			sprintf( outfilename_base, "%s/%s", gUI_Image_Folder, currentUiScanJobManage.uiScanJobTempFolderName );
	}

    DBG_PRINTF_SDH_DEBUG("  fileFormat = %ld\n",fileformat );
    switch( fileformat )
    {
	case UI_FILE_TIFF_MULTI_PAGE:
    case UI_FILE_TIFF:
        extension = "tif";
        break;
    case UI_FILE_PDF:
    case UI_FILE_PDF_MULTI_PAGE:
        extension = "pdf";
        break;
    case UI_FILE_JPEG:
        extension = "jpg";
        break;
    case UI_FILE_PGM:
        if( dtype == SCAN_DATA_TYPE_RGB ) {
            extension = "ppm";
        }
        else {
            extension ="pgm";
        }
        break;
    default:
        extension = NULL;
        break;
    }

	if ((uiJobSettings.isPersonalScanJob == TRUE ) &&
		((( uiJobSettings.scanToDestination == UI_FILE_DEST_USBTHUMB) && (strlen(uiJobSettings.saveToFileInfo.fileName) == 0 ))
		|| (( uiJobSettings.scanToDestination == UI_FILE_DEST_EMAIL )&& (strlen(uiJobSettings.sendToEmailInfo.fileName) == 0))
		|| ((( uiJobSettings.scanToDestination == UI_FILE_DEST_FTP ) || ( uiJobSettings.scanToDestination == UI_FILE_DEST_SMB )
			|| ( uiJobSettings.scanToDestination == UI_FILE_DEST_WEBDAV )) && (strlen(uiJobSettings.sendToServerInfo.fileName) == 0))))
	{
		char  authUserId[AUTH_BASIC_LENGTH] = {0x00,};
		char  ScanJobFileName[SDH_FILENAME_MAX + 1] = {0};

		memset(ScanJobFileName, 0x00, SDH_FILENAME_MAX+1);
		snprintf( authUserId, sizeof(authUserId), "%s", GetCurerntAuthUser() );

		//sprintf( outfilename_base, "%s/S%s", gUI_Image_Folder, currentUiScanJobManage.uiScanJobTempFolderName);
			
		if( (uiJobSettings.pageType == UI_FILE_PAGE_MULTI ) && ( fileformat != UI_FILE_JPEG )) {
			retcode = snprintf( outfilename, FILENAME_MAX, "%s_%s_box.%s", 
							outfilename_base,
							authUserId,
							extension );
		}
		else {
			retcode = snprintf( outfilename, FILENAME_MAX, "%s_%s_box_%04d.%s", 
							outfilename_base, authUserId,
							page_number,
							extension );
		}
	}
	else {
		//sprintf( outfilename_base, "%s/%s", gUI_Image_Folder, currentUiScanJobManage.uiScanJobTempFolderName);
		if((uiJobSettings.pageType == UI_FILE_PAGE_SINGLE ) || ( fileformat == UI_FILE_JPEG )
			|| (uiJobSettings.isPersonalScanJob == FALSE )) {
			retcode = snprintf( outfilename, FILENAME_MAX, "%s_%04d.%s", 
							outfilename_base, page_number, 
							extension );
		}
		else if (uiJobSettings.isPersonalScanJob == TRUE )
		{
			retcode = snprintf( outfilename, FILENAME_MAX, "%s.%s", 
							outfilename_base,
							extension );
		}
	}
	
	if (page_number == 0)
	{
		char  ScanJobFileName[SDH_FILENAME_MAX + 1] = {0};
		memset(ScanJobFileName, 0x00, SDH_FILENAME_MAX+1);

		if ((uiJobSettings.isPersonalScanJob == TRUE ) &&
			((( uiJobSettings.scanToDestination == UI_FILE_DEST_USBTHUMB) && (strlen(uiJobSettings.saveToFileInfo.fileName) == 0 ))
			|| (( uiJobSettings.scanToDestination == UI_FILE_DEST_EMAIL )&& (strlen(uiJobSettings.sendToEmailInfo.fileName) == 0))
			|| ((( uiJobSettings.scanToDestination == UI_FILE_DEST_FTP ) || ( uiJobSettings.scanToDestination == UI_FILE_DEST_SMB )
				|| ( uiJobSettings.scanToDestination == UI_FILE_DEST_WEBDAV )) && (strlen(uiJobSettings.sendToServerInfo.fileName) == 0))))
		{
			sprintf( ScanJobFileName, "S%s_%s_box.%s", currentUiScanJobManage.uiScanJobTempFolderName, GetCurerntAuthUser(), extension );
		}
		else
		{
			sprintf( ScanJobFileName, "%s.%s", currentUiScanJobManage.uiScanJobTempFolderName, extension );
		}
		ScanSetJobFileName(ScanJobFileName);
	}

    XASSERT( retcode<FILENAME_MAX, retcode );    
}

int scanner_service_get_job_id(void)
{
	return currentUiScanJobManage.systemJobID;
}

int scanner_service_get_fb_wait_time(void)
{
	int scan_fbSCan_waitTime = 0;	
	error_type_t db_retval = FAIL;
	
	scan_fbSCan_waitTime = db_get_var_int("CONF_SCANSET", "TimeoutFlatbed", &db_retval);

	if( db_retval == FAIL ) {
		DBG_PRINTF_SDH_DEBUG("db_get_var_int FAIL! : CONF_SCANSET,TimeoutFlatbed\n");
		return -1;	
	}
	DBG_PRINTF_SDH_DEBUG("TimeoutFlatbed is %d\n", scan_fbSCan_waitTime);
	return scan_fbSCan_waitTime;
}

#if SDH_USE_ERASE_BORDER
static void scanner_erase_border(uint16_t num_rows, uint8_t *scanData, uint8_t page_side, scan_data_type dataType)
{
	uint32_t num_bytes = num_rows * uiscan_self.bytes_per_row;
	
	//White padding (Erase Border) : left and rigth
	if( (scanData) && (uiJobSettings.eraseBorderParam.eraseLeftPixel || uiJobSettings.eraseBorderParam.eraseRightPixel) ) {
		uint32_t rowIdx;
		uint32_t bytes_per_pixel = uiscan_self.bits_per_pixel/8;
		uint32_t eraseLeftDataSize = (uiJobSettings.eraseBorderParam.eraseLeftPixel * bytes_per_pixel);

		uint32_t nTotalImageWidthPixel = (uiJobSettings.Resolution * uiJobSettings.scan_window.right)/100;
		uint32_t eraseRightPixelPoint = nTotalImageWidthPixel - uiJobSettings.eraseBorderParam.eraseRightPixel;
		uint32_t eraseRightDataPoint = eraseRightPixelPoint * bytes_per_pixel;
		uint32_t eraseRightDataSize = uiscan_self.bytes_per_row - eraseRightDataPoint;
		
		//DBG_PRINTF_SDH_DEBUG("Padding Data Size:BytePerRow(%d), L(%d), R(%d), RPoint(%d)\n", uiscan_self.bytes_per_row, eraseLeftDataSize, eraseRightDataSize,eraseRightDataPoint);
		
		for(rowIdx=0; rowIdx<num_rows; rowIdx++) {
			memset( scanData + (rowIdx * uiscan_self.bytes_per_row), 0xFF, eraseLeftDataSize );
			memset( scanData + (rowIdx * uiscan_self.bytes_per_row) + eraseRightDataPoint, 0xFF, eraseRightDataSize);
		}
	}
	
	//White padding (Erase Border) : top and bottom
	if( (scanData) && (uiJobSettings.eraseBorderParam.eraseTopPixel || uiJobSettings.eraseBorderParam.eraseBottomPixel) ) {
		uint32_t buf_height = num_rows;
		uint32_t nPaddingHeight = 0;
		uint32_t nImageHeight = uiscan_stats.current_rows_index[page_side];
		
		uint32_t nTotalImageHeightPixel = (uiJobSettings.Resolution * uiJobSettings.scan_window.bottom)/100;
		uint32_t nEraseTop = uiJobSettings.eraseBorderParam.eraseTopPixel;
		uint32_t nEraseBottom = nTotalImageHeightPixel - uiJobSettings.eraseBorderParam.eraseBottomPixel;
		
		if( nImageHeight < nEraseTop )
		{
			if( nImageHeight + buf_height < nEraseTop )
			{
				memset( scanData, 0xFF, num_bytes);
			}
			else
			{
				nPaddingHeight = nEraseTop - nImageHeight;
				memset( scanData, 0xFF, uiscan_self.bytes_per_row * nPaddingHeight );
			}
			//DBG_PRINTF_SDH_DEBUG("padding top %p, %d\n", scanData, uiscan_self.bytes_per_row * nPaddingHeight );
		}
		else if( nEraseBottom < nImageHeight + buf_height)
		{
			if( nEraseBottom < nImageHeight )
			{
				memset( scanData, 0xFF, num_bytes);
			}
			else
			{
				nPaddingHeight = (nImageHeight + buf_height) - nEraseBottom;
				//DBG_PRINTF_SDH_DEBUG("padding bottom 2 %p, %d\n", scanData+( (nEraseBottom-nImageHeight) * uiscan_self.bytes_per_row), uiscan_self.bytes_per_row * nPaddingHeight);
				memset( scanData + ((nEraseBottom-nImageHeight) * uiscan_self.bytes_per_row), 0xFF, uiscan_self.bytes_per_row * nPaddingHeight);
			}
		}
	}	
}
#endif


/**
 * Function name: handle_data_message
 *
 * \brief Main routine for data transfer and save into file
 *
 **/
static scan_err_t handle_data_message( int page_number, MESSAGE *msg )
{
	scan_err_t scerr = SCANERR_NONE;
    uint16_t num_rows;
    scan_data_type dtype;
    uint8_t *scanData;
    bool last_buffer;
    uint32_t num_bytes;
    uint8_t page_side;
    struct output_file *outfile; 
    char outfilename[FILENAME_MAX];
    char pbm_header[64];
    
    /*
     ********************************************************************************************************************
	 * decode the message because we need to know the image band buffer size
	 * 
	 * 		< num_rows >
	 * 			-> It means the number of rows incomming image band data (in normal : 64 rows)
	 * 
	 * 		< num_bytes >
	 * 			-> It means data size of incomming image band data
	 * 			-> Calculated by 'uiscan_self' value which set in 'MSG_SCAN_SIZE'
	 * 
	 * 		< scanData >
	 * 			-> It is the buffer contains incomming image band data (planedata)
	 * 		
	 ********************************************************************************************************************
	 */
    msg_scan_planedata_decode( msg, &num_rows, &dtype, &scanData, &last_buffer );
    ASSERT( scanData != NULL );
    msg_scan_planedata_get_page_side( msg, &page_side );
    
#if 0
		DBG_PRINTF_SDH_DEBUG( "%s num_rows=%d current_rows_index[%d]=%d dtype=%d scanData=%p side=%d last=%d\n", __FUNCTION__,
            num_rows, page_side, uiscan_stats.current_rows_index[page_side], dtype, scanData, page_side, last_buffer );
#endif

    num_bytes = num_rows * uiscan_self.bytes_per_row;

#if SDH_USE_ERASE_BORDER
	if(uiJobSettings.scanActionType == UI_SCAN_MERGEJOB_NONE) {
		scanner_erase_border(num_rows, scanData, page_side, dtype);
	}
	else {
		//DBG_PRINTF_SDH_DEBUG("This is merge scan job, so we don't erase border\n");
	}
#endif

	switch( uiJobSettings.fileFormat )
	{
    /**************************************************************************************
	 * Data Parsing : TIFF, TIFF Multi-Page, JPEG, PDF, PDF Multi-Page
	 **************************************************************************************/
	case UI_FILE_TIFF:
	case UI_FILE_TIFF_MULTI_PAGE:
	case UI_FILE_JPEG:
	case UI_FILE_PDF:	
	case UI_FILE_PDF_MULTI_PAGE:
		/// Copy Image Data into 'One Page Memory'
		if(scanOnePageImageBuf[page_side].write_index < scanOnePageImageBuf[page_side].bufsize_bytes) {
			if(scanOnePageImageBuf[page_side].write_index + num_bytes > scanOnePageImageBuf[page_side].bufsize_bytes ) {
				DBG_PRINTF_SDH_DEBUG("scanOnePageImageBuf[%d].buf WRITE data num change from [%d]", page_side, num_bytes);
				num_bytes = scanOnePageImageBuf[page_side].bufsize_bytes - scanOnePageImageBuf[page_side].write_index;
				DBG_PRINTF_SDH_DEBUG("to [%d]\n", num_bytes);
			}			
			if(scanOnePageImageBuf[page_side].buf != NULL && scanData != NULL) {
				//DBG_PRINTF_SDH_DEBUG(" Copy image buffer into scanOnePageImageBuf[%d].buf - num_byte : %ld\n", page_side, num_bytes);
				memcpy( &scanOnePageImageBuf[page_side].buf[ scanOnePageImageBuf[page_side].write_index ], scanData, num_bytes);
				scanOnePageImageBuf[page_side].write_index += num_bytes;
			}
			else {
				DBG_PRINTF_SDH_DEBUG("scanOnePageImageBuf[page_side].buf is NULL, or scanData is NULL\n"); 
			}
		}
		else {
			DBG_PRINTF_SDH_DEBUG("Can't Write Buffer into scanOnePageImageBuf[%d].buf, Oversize of Index\n", page_side);
		}
		break;
    /**************************************************************************************
	 * Data Parsing : PGM (raw) <Just for debug, not included in Function Spec.
	 **************************************************************************************/
	case UI_FILE_PGM:
		outfile = output_file_list_lookup( &outfile_list, dtype, page_side );
		if( outfile == NULL ) {
			DBG_PRINTF_SDH_DEBUG( "outfile is NULL \n" );
			make_output_filename( outfilename, UI_FILE_PGM, uiscan_stats.page_starts, dtype, page_side );
			//make_output_filename( outfilename, page_number, dtype, page_side );
			outfile = output_file_list_fopen( &outfile_list, outfilename, dtype, page_side );

			if( !outfile ) {
				DBG_PRINTF_SDH_ERR( "failed to open \"%s\" for scanned image!\n", outfilename );
				scanner_set_file_write_error(0);
				if(State == UI_SCAN_APP_SCANNING) {
					scanner_job_cancel_request();
				}
			}
			else {
				/* write a PBM header */
				if( dtype == SCAN_DATA_TYPE_RGB ) {
					fwrite( "P6\n", 1, 3, outfile->filep );
				}
				else {
					fwrite( "P5\n", 1, 3, outfile->filep );
				}
				memset( pbm_header, 0, sizeof(pbm_header) );
				sprintf( pbm_header, "%d %d\n", uiscan_self.pixels_per_row_padded, uiscan_self.total_rows );
				fwrite( pbm_header, 1, strlen(pbm_header), outfile->filep );
				if( uiscan_self.bits_per_pixel==16 ) {
					fwrite( "65535\n", 1, 6, outfile->filep );
				}
				else {
					fwrite( "255\n", 1, 4, outfile->filep );
				}
				break;
			}
		}
		
		if( outfile ) {
			if( uiscan_self.bits_per_pixel==16 ) {
				htons_data( scanData, num_bytes );
			}
			scerr = buffer_write( outfile->filep, scanData, num_bytes );
			if( scerr != SCANERR_NONE ) {
				/* buffer_write() logs error */
				scanner_set_file_write_error(0);
				if(State == UI_SCAN_APP_SCANNING) {
					scanner_job_cancel_request();
				}
				return scerr;
			}
			fflush( outfile->filep );
		}
		break;
	}
    
    ///Count up Scanned Image , Cleared @ 'scanner_page_end()'
	uiscan_self.gtotal += num_bytes;
	
	///Increase received row number in each mode (front, back)
	uiscan_stats.current_rows_index[page_side] += num_rows;
	
	///Clear Memory pointer
    memFree( scanData );
	
	/*
	 **************************************************************************************************************************
	 * LAST Buffer Catched !
	 **************************************************************************************************************************
	 */
    if( last_buffer ) {
        DBG_PRINTF_SDH_DEBUG( "Scan Last Buffer CATCHED!. File Type:%d \n",  uiJobSettings.fileFormat );
        switch( uiJobSettings.fileFormat )
        {
		/*********************************************************************************
		 *  File Closing :
		 * 		- 1) TIFF					: executed in "scanner_page_end()'
		 * 		- 2) TIFF(Multi Page)  : executed in 'scanner_job_end()'
		 * 		- 3) JPEG              : executed in 'scanner_page_end()'
		 * 		- 4) PDF(Multi Page)   : executed in 'scanner_job_end()'
		 * 		- 5) PDF(Single Page)  : executed in 'scanner_page_end()'
		 *********************************************************************************/
		case UI_FILE_TIFF:
		case UI_FILE_TIFF_MULTI_PAGE:
		case UI_FILE_JPEG:
		case UI_FILE_PDF:
		case UI_FILE_PDF_MULTI_PAGE:
			break;
		/*********************************************************************************
		 * File Closing : PGM (raw)
		 *********************************************************************************/
        case UI_FILE_PGM:
			scerr = output_file_list_fclose( &outfile_list, dtype, page_side );
			if( scerr != SCANERR_NONE ) {
				DBG_PRINTF_SDH_DEBUG( "%s failed to close file for dtype=%d page_side=%d\n", __FUNCTION__, dtype, page_side );
				scanner_set_file_write_error(0);
				if(State == UI_SCAN_APP_SCANNING) {
					scanner_job_cancel_request();
				}				
			}
			XASSERT( scerr==SCANERR_NONE, scerr );						
            break;
		}
    }
    
    return scerr;
}

/**
 * Function name: uiscanapp_mgr
 *
 * \brief Main thread of the scan app job manager
 *
 * This thread handles the upper level of the ui scan app
 **/
void *uiscanapp_mgr (void* unused)
{
    MESSAGE msg;
//    error_type_t err;
    scan_err_t scerr;
    UINT pxretcode;
    job_resources = NULL;
    BOOL WaitingForInput = FALSE;
    
//    static int page_number=0;
    uint8_t page_side;
	int diskUsableCheck = 0;

    //wait for system init to finish
    SysWaitForInit();

    while (1)
    {
        //
        // Get the message from the queue.
        //
        pxretcode = posix_wait_for_sys_message( uiscanapp_msgq, (char *)&msg, POSIX_WAIT_FOREVER);

        //Do nothing if no scanner
        if(!scanner_is_alive()){
            DBG_PRINTF_SDH_DEBUG( "%s Scanner is NOT alive\n", __FUNCTION__);
            continue;
        }

        switch (msg.msgType)
        {
		/*
		 * *****************************************************************************************
		 * MSG_CONNECT_TEST
		 * *****************************************************************************************
		 */			
		case MSG_CONNECT_TEST:		
			DBG_PRINTF_SDH_DEBUG( "===============> MSG RCV: MSG_CONNECT_TEST\n");
            if( State == UI_SCAN_APP_IDLE ) {
				memset( &uiJobSettings, 0x00, sizeof(UISCAN_JOB_SETTINGS) );
				memcpy( &uiJobSettings, msg.param3, sizeof(UISCAN_JOB_SETTINGS));
				
				if(uiJobSettings.scanToDestination == UI_FILE_DEST_FTP) {
					scanner_send_to_ftp(SEND_REQUEST_CONNECT_TEST);
				}
				else if(uiJobSettings.scanToDestination == UI_FILE_DEST_SMB) {
					scanner_send_to_smb(SEND_REQUEST_CONNECT_TEST);
				}
				else if(uiJobSettings.scanToDestination == UI_FILE_DEST_WEBDAV) {
					scanner_send_to_webdav(SEND_REQUEST_CONNECT_TEST);					
				}
			}
			MEM_FREE_AND_NULL( msg.param3 ); //free here. (malloc is occured in "SysServiceMgr.c -> ScanAppService")
			break;
			
		/*
		 * *****************************************************************************************
		 * MSG_SETTING_CHANGED
		 * *****************************************************************************************
		 */
        case MSG_SETTING_CHANGED:
			DBG_PRINTF_SDH_DEBUG( "===============> MSG RCV: MSG_SETTING_CHANGED\n");
			DBG_PRINTF_SDH_DEBUG( " State : %d\n", State);
            if( State == UI_SCAN_APP_IDLE ) {
				DBG_PRINTF_SDH_DEBUG( "SCAN uiJobSettings Requested.\n");
				memset( &uiJobSettings, 0x00, sizeof(UISCAN_JOB_SETTINGS) );
				memcpy( &uiJobSettings, msg.param3, sizeof(UISCAN_JOB_SETTINGS));
				
				//For Merge Job, change the scan area
				g_scan_started_from_adf = adf_paper_is_present();
				
				if(	uiJobSettings.sizecode == MEDIASIZE_A3 ) {
					scanner_job_setupMergeJob_scanArea(UI_SCAN_MERGEJOB_A3);
				}
				else if( uiJobSettings.sizecode == MEDIASIZE_11X17 ) {
					scanner_job_setupMergeJob_scanArea(UI_SCAN_MERGEJOB_11X17);
				}
				else if((uiJobSettings.sizecode == MEDIASIZE_LEGAL) && (g_scan_started_from_adf == FALSE)) {
					scanner_job_setupMergeJob_scanArea(UI_SCAN_MERGEJOB_LEGAL);
				}
				else {
					scanner_job_setupMergeJob_scanArea(UI_SCAN_MERGEJOB_NONE);					
				}
				
				scerr = allocate_global_scanvar_for_uiscan( &uiJobSettings );				
				scerr = setup_scan_params( g_scanvars, &uiJobSettings );
				
				if( scerr != SCANERR_NONE ) {
					/* we didn't like something about the job request so reject it */
					if( g_scanvars ) {
						scanvar_delete( &g_scanvars );
					}
					break;
				}
			}
			else {
				DBG_PRINTF_SDH_DEBUG("State is not UI_SCAN_APP_IDLE : state=%d\n", State);
			}
            MEM_FREE_AND_NULL( msg.param3 ); //free here. (malloc is occured in "SysServiceMgr.c -> ScanAppService")
            break;
            
		/*
		 * *****************************************************************************************
		 * MSG_STARTRECIPE
		 * *****************************************************************************************
		 */
        case MSG_STARTRECIPE:
            if( State == UI_SCAN_APP_IDLE ) {
				DBG_PRINTF_SDH_DEBUG( "===============> MSG RCV: MSG_STARTRECIPE \n");
				//
				// Get a MSG_STARTRECIPE message from the System Service Manager to kick off a UI Scan
				//
				// Update the status to scan pending and forward the message to the system job mgr.
				msg.param2 = SCANTOEMAILAPPID;
				SYMsgSend(SJMID, &msg);

#ifdef HAVE_IMAGELOG				
				if (SCAN_IL_IS_enable())
				{
					DBG_PRINTF_SDH_DEBUG("Scan IMAGELOG TURN-ON\n");
					g_nIsEnableScanImageLog = TRUE;
				}
				else
				{
					g_nIsEnableScanImageLog = FALSE;
				}
#endif
			}
			else {
				MEM_FREE_AND_NULL( msg.param3 ); //free here. (malloc is occured in "SysServiceMgr.c -> ScanAppService")
			}
			break;

		/*
		 * *****************************************************************************************
		 * MSG_RESOURCES
		 * *****************************************************************************************
		 */
        case MSG_RESOURCES:
            DBG_PRINTF_SDH_DEBUG( "===============> MSG RCV: MSG_RESOURCES \n");
            /* be wary of memory leaks */
            job_resources = (CURRENT_RESOURCE *) msg.param3;
            DBG_PRINTF_SDH_DEBUG( "scanapp received resources src=%d dst=%d\n",
                        job_resources->Source,
                        job_resources->Destination );
            currentUiScanJobManage.systemJobID = job_resources->JobResources->JobId;
            /* We have the resources, wait for the job manager to tell us to go */
            break;

		/*
		 * *****************************************************************************************
		 * MSG_ACKRECIPE
		 * *****************************************************************************************
		 */
        case MSG_ACKRECIPE:
            DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_ACKRECIPE\n" );
			/* 
			 * tell the scanner to start working; scanner will tell me how
			 * big the scan data will be in an internal message
			 */

#if USE_SDH_A3_MERGE_ALGORITHM
			/* If A3 Job Requested from UI, Application setup size into A4, and send special MSG to SCAN_MAN
			 * Then, SCAN_MAN Setup the parameter for use A3 Merge Alghorithm
			 */
			if(uiJobSettings.sizecode == MEDIASIZE_A3) {
				scanner_job_setupMergeJob_forStart( UI_SCAN_MERGEJOB_A3 );
			}
			else if(uiJobSettings.sizecode == MEDIASIZE_11X17) {
				scanner_job_setupMergeJob_forStart( UI_SCAN_MERGEJOB_11X17 );
			}
			else if((uiJobSettings.sizecode == MEDIASIZE_LEGAL) && (g_scan_started_from_adf == FALSE)) {
				scanner_job_setupMergeJob_forStart( UI_SCAN_MERGEJOB_LEGAL );
			}
#endif
			scanner_job_start( g_scanvars, job_resources );

			/* Setup UI Scan Job Num and temp folder name */
			if(uiJobSettings.scanToDestination == UI_FILE_DEST_USBTHUMB) {
				scanner_job_setupJobManage( SCAN_SAVE_FILESYSTEM_TYPE_USE_THUMBDRIVE );
			}
			else {
				scanner_job_setupJobManage( SCAN_SAVE_FILESYSTEM_TYPE_USE_RAMDISK );
			}
			
			/* Clear variable regarding scan state */
			scanner_clear_state_variable();

			/* Send Event to Observer regarding SCAN State */                        
			scanapp_log_event( SCANAPP_EVENT_SCAN_START );    
			STATE_TRANSITION( UI_SCAN_APP_SCANNING );
            break;
            
		/*
		 * *****************************************************************************************
		 * MSG_USER_INPUT
		 * *****************************************************************************************
		 */
		case MSG_USER_INPUT:
			DBG_PRINTF_SDH_DEBUG( "===============> MSG RCV: MSG_USER_INPUT\n" );
			if (!WaitingForInput) {
				DBG_PRINTF_SDH_DEBUG("WaitingForInput : is FALSE\n");
				break;          // we should not be here, get out
			}
			WaitingForInput = FALSE;
			//
			// Change status to scanning.
			//
			if (msg.param1 == PARAM_USER_OK) {
				//For Merge Job, change the scan area for next scan
				if((uiJobSettings.sizecode == MEDIASIZE_LEGAL) && (g_scan_started_from_adf == FALSE)) {
					DBG_PRINTF_SDH_DEBUG( "==> PARAM_USER_OK: MEDIASIZE_LEGAL\n" );
					scanner_job_setupMergeJob_scanArea(UI_SCAN_MERGEJOB_LEGAL_2ND_PAGE);					
					scerr = setup_scan_params( g_scanvars, &uiJobSettings );
				}
				
				// We are ready to go with the next scan.
				scanner_job_start( g_scanvars, job_resources );

				/* Send Event to Observer regarding SCAN State */                        
				scanapp_log_event( SCANAPP_EVENT_SCAN_START );
				break;
			}
			else if (msg.param1 == PARAM_USER_FINISH_JOB)
			{
				scanner_job_end( g_scanvars, job_resources );         
				scanapp_log_event( SCANAPP_EVENT_SCAN_COMPLETE );
				system("echo 3 > /proc/sys/vm/drop_caches");
				break;
			}
			//
			// See if this user input was correct.  If not handle it.
			//
			else
			{
				break;
			}

		/*
		 * *****************************************************************************************
		 * MSG_SCAN_PAGESTART
		 * *****************************************************************************************
		 */
        case MSG_SCAN_PAGESTART:
            DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_SCAN_PAGESTART : %d\n", uiscan_stats.page_starts);
            msg_scan_page_start_decode( &msg, &page_side );
            DBG_PRINTF_SDH_DEBUG( "uiscan received page_start page_side=%d\n", page_side );
            
            /* check Ram Disk usable or not */
			if(uiJobSettings.scanToDestination == UI_FILE_DEST_USBTHUMB) { 
				if ( strncmp( gUI_Image_Folder, "/mnt/sd/", 8 ) == 0 )
				{
					diskUsableCheck = scanner_service_get_filesystem_usage(gUI_sdCardRootPath);
					if ( diskUsableCheck != -1 ){
						diskUsableCheck = scanner_service_get_filesystem_usage( RAMDISK_PATH );
				}
				}
				else
				{
					diskUsableCheck = scanner_service_get_filesystem_usage(gUI_usbThumbdriveRootPath);
					if ( diskUsableCheck != -1 ){
						diskUsableCheck = scanner_service_get_filesystem_usage( RAMDISK_PATH );
				}
				}
			}
			else {
				diskUsableCheck = scanner_service_get_filesystem_usage(RAMDISK_PATH);
			}
			
			if( diskUsableCheck == -1) {
				DBG_PRINTF_SDH_DEBUG("scanner_job_cancel_request. due to DISK OVER\n");
				scanapp_log_event(SCANAPP_EVENT_SCAN_FAILED_MEMORY_FULL);
				scanner_set_file_write_error(0);
				if(State == UI_SCAN_APP_SCANNING) {
					scanner_job_cancel_request();
				}
			}
			else {
				scanner_page_start(page_side);
				uiscan_stats.page_starts++;
			}
            break;

		/*
		 * *****************************************************************************************
		 * MSG_SCAN_SIZE
		 * *****************************************************************************************
		 */
        case MSG_SCAN_SIZE:
            DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_SCAN_SIZE \n");
            msg_scan_size_decode(
                        &msg,
                        &uiscan_self.pixels_per_row_padded,
                        &uiscan_self.pixels_per_row,
                        &uiscan_self.total_rows,
                        &uiscan_self.bits_per_pixel
                );
            DBG_PRINTF_SDH_DEBUG( "%s ppr=%d pprp=%d tr=%d bpp=%d\n", __FUNCTION__,
                    uiscan_self.pixels_per_row,
                    uiscan_self.pixels_per_row_padded,
                    uiscan_self.total_rows,
                    uiscan_self.bits_per_pixel
                );

            /* pre-calculate the bytes_per_row (will be constant across this scan) */
            uiscan_self.bytes_per_row = uiscan_self.pixels_per_row_padded * (uiscan_self.bits_per_pixel/8);
            uiscan_stats.sizes++;
			break;

		/*
		 * *****************************************************************************************
		 * MSG_SCAN_PLANEDATA
		 * *****************************************************************************************
		 */
        case MSG_SCAN_PLANEDATA:
//            DBG_PRINTF_SDH_DEBUG( "uiscan received MSG_SCAN_PLANEDATA \n");
            uiscan_stats.page_datas++;
            handle_data_message( page_number, &msg );    
            break;

		/*
		 * *****************************************************************************************
		 * MSG_SCAN_PAGEEND
		 * *****************************************************************************************
		 */
        case MSG_SCAN_PAGEEND:
            DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_SCAN_PAGEEND : %d \n", uiscan_stats.page_starts);
            msg_scan_page_end_decode( &msg, &page_side );
            DBG_PRINTF_SDH_DEBUG( "uiscan received pageend Page_side:%d, P.Number:%d\n", page_side, page_number );
            
			if (get_free_heap_mem() < PDF_MIN_MEMFREE_SIZE)
			{
				system("echo 3 > /proc/sys/vm/drop_caches");
            }
            
            scanner_page_end( page_side );
            
            /* Send Event to Observer regarding SCAN State */
			scanapp_log_event( SCANAPP_EVENT_SCAN_RUNNING );		
            break;

		/*
		 * *****************************************************************************************
		 * MSG_SCAN_JOBEND
		 * *****************************************************************************************
		 */
        case MSG_SCAN_JOBEND:
            DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_SCAN_JOBEND \n");
			if (scanman_get_skipmsgfail() == true)
			{
				uiscan_stats.scan_ends++;
				scanner_job_end( g_scanvars, job_resources );
				scanapp_log_event( SCANAPP_EVENT_SCAN_COMPLETE );
				system("echo 3 > /proc/sys/vm/drop_caches");
			}
			else if( (g_scan_started_from_adf == false) && 
				(uiJobSettings.scanActionType == UI_SCAN_MERGEJOB_NONE) &&
				(scanner_service_get_fb_wait_time() > 0)
			) {
				/* If current job is FB and wait configure value is over than 0, request next FB Scan */
				WaitingForInput = TRUE;
				scanapp_log_event(SCANAPP_EVENT_WAITING_FOR_USER_INPUT);
				DBG_PRINTF_SDH_DEBUG("Request : SCANAPP_EVENT_WAITING_FOR_USER_INPUT\n");
			}
			else {
				uiscan_stats.scan_ends++;
				scanner_job_end( g_scanvars, job_resources );

				/* Send Event to Observer regarding SCAN State */            
				scanapp_log_event( SCANAPP_EVENT_SCAN_COMPLETE );
				system("echo 3 > /proc/sys/vm/drop_caches");
			}
			break;

		/*
		 * *****************************************************************************************
		 * MSG_WAIT_NEXTPAGE_JOB_START_FROM_USER (Received from 'ScanMan')
		 * *****************************************************************************************
		 */
		case MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START:
            DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START\n");
			WaitingForInput = TRUE;
			scanapp_log_event(SCANAPP_EVENT_WAITING_FOR_NEXT_LEGAL_UISCAN);
			break;
		
		/*
		 * *****************************************************************************************
		 * MSG_A3MERGE_NOTIFY_MERGE_END (Received from 'ScanMan')
		 * *****************************************************************************************
		 */
		case MSG_A3MERGE_NOTIFY_MERGE_END:
			DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_A3MERGE_NOTIFY_MERGE_END\n");
			if(msg.param1 == A3MERGE_STATUS_RollerDetected) {
				scanapp_log_event(SCANAPP_EVENT_SCAN_FAILED_FB_EMPTY_FOR_MERGE);
			}
			break;
		
		/*
		 * *****************************************************************************************
		 * MSG_CANCELJOB (Received from 'SystemJobMgr')
		 * *****************************************************************************************
		 */
		case MSG_CANCELJOB:
			DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_CANCELJOB\n");
			DBG_PRINTF_SDH_DEBUG( "%s acking job cancel in state : %d\n", __FUNCTION__ , State);
			/* Send an ack to the system jobmgr we've successfully cancelled */
			send_cancel_ack_msg( msg.param3 );
				
			/* free the configuration memory */
			if( g_scanvars != NULL) {
				DBG_PRINTF_SDH_DEBUG( "deleting user's previous scanvars. MSG_CANCELJOB\n" );
				scanvar_delete( &g_scanvars );
			}
			
			/* clear resource of scan application */
			scanner_job_clear_app_resource();
			send_free_msg();
			scanapp_log_event( SCANAPP_EVENT_SCAN_COMPLETE );
			STATE_TRANSITION( UI_SCAN_APP_IDLE );
			system("echo 3 > /proc/sys/vm/drop_caches");
			break;
			
		/*
		 * *****************************************************************************************
		 * MSG_SENDJOB_RESULT (Receive when Send job finished from 'Sending App')
		 * *****************************************************************************************
		 */
		case MSG_SENDJOB_RESULT:
			DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_SENDJOB_RESULT\n");
			
			//If the sending process is for 'FAX Auth job', transfer the result to FAX App.
			if ( msg.param1 & FAX_SEND_FLAG )
			{
				//If the sending process is for 'FAX Auth job', transfer the result to FAX App.
				scanner_send_result(GET_SCAN_JOB_NO(msg.param1), msg.param2);
				msg.param1 = GET_FAX_JOB_NO(msg.param1);
				msg.msgType = MSG_SENDJOB_FOR_AUTH_FAX_RESULT;
				SYMsgSend( FAXAPPSOLUTIONID, &msg );
			}
			else
			{
				scanner_send_result(msg.param1, msg.param2);
			}
			break;
			
		/*
		 * *****************************************************************************************
		 * MSG_NITIFY_USBTHUMB_DISCONNECT (Receive when USB Thumbdrive disconnected)
		 * *****************************************************************************************
		 */
 		case MSG_NITIFY_USBTHUMB_DISCONNECT:
			DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: MSG_NITIFY_USBTHUMB_DISCONNECT\n");
			if(State == UI_SCAN_APP_SCANNING && uiJobSettings.scanToDestination == UI_FILE_DEST_USBTHUMB) {
				
				if ( strncmp( gUI_Image_Folder, "/mnt/sd/", 8 ) != 0 )
				{
					DBG_PRINTF_SDH_DEBUG("scanner_job_cancel_request. due to DISK DISCONNECT\n");
					scanner_job_cancel_request();
					scanner_set_file_write_error(0);
				}
				else
				{
					DBG_PRINTF_SDH_DEBUG("Now, scan to SD card\n");
				}
			}
			break;

		default:
			DBG_PRINTF_SDH_DEBUG(  "===============> MSG RCV: Unknown Message type=%#x param1=%#x\n", msg.msgType, msg.param1);
			break;
        }
    } //while (1)

    return 0;

}
/**
 * Function name: ScanAppUiScanInit
 *
 * \brief Initialize the ui scan app
 *
 * Initialize data structures, create the queue, register the mainbox and start the task for the ui scan app
 **/
void ScanAppUiScanInit(void)
{
    int retcode = 0;

    DBG_PRINTF_SDH_DEBUG("ScanAppUiScanInit\n");

    output_file_list_init( &outfile_list );

    /* get the hardware capabilities of the platform (we need to know if we have an ADF or not) */
    scancap_get_capabilities( &scan_capabilities );

    //
    // Create the queue
    //
    posix_create_message_queue(&uiscanapp_msgq, "/ScanAppUiScanQueue", SCANAPP_UISCAN_QUEUE_SIZE, sizeof(MESSAGE));

    //
    // Register a message queue.
    //
    router_register_queue(SCANTOEMAILAPPID, uiscanapp_msgq);

    //
    // Register the resource
    //
    rm_register(e_ScanToEmailApp, SCANTOEMAILAPPID);
    //
    // start the scan app job mgr task
    //
    retcode = posix_create_thread(&uiscanapp_thd_id, uiscanapp_mgr, 0, "UiScanAppMgr", UiScanAppStack,
                                  SCANAPP_UISCAN_STACK_SIZE, POSIX_THR_PRI_NORMAL);
	
	//Make temp folder in 'RamDisk' for UI Scan
	scanner_make_scan_basic_foler();
		                                  
	//
	// Change State into Idle
	//
	STATE_TRANSITION( UI_SCAN_APP_IDLE );
                                 
    XASSERT( retcode==0, retcode );

#if USE_DEBUG_FUNC	
	//debug_make_jpg_test();
	//debug_make_tif_test();
    //debug_make_pdf_test();
#endif
}

void debug_make_jpg_test()
{
	cinfo.err = jpeg_std_error(&jerr);
	
	jpeg_create_compress(&cinfo);

	// Open the JPEG file
	if (( jpegFile = fopen( "outputTEST.jpg", "wb" )) == NULL ) {
		DBG_PRINTF_SDH_ERR("Could not open JPEG file for writing\n");
		assert(0);
	}

	jpeg_stdio_dest(&cinfo, jpegFile);

	cinfo.image_width = 2560;   					// image width and height, in pixels
	cinfo.image_height = 3525;  					//3312;

	if( uiJobSettings.ScanType == e_MonochromeScan ) {
		cinfo.input_components = 1;             	// # of color components per pixel
		cinfo.in_color_space = JCS_GRAYSCALE;  	//colorspace of input image
	}
	else if ( uiJobSettings.ScanType == e_ColorScan) {
		cinfo.input_components = 3;         		// # of color components per pixel
		cinfo.in_color_space = JCS_RGB;     	// colorspace of input image
	}
	jpeg_set_defaults(&cinfo);
	
	//jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	jpeg_set_quality(&cinfo, 50, TRUE /* limit to baseline-JPEG values */);
	
	jpeg_start_compress(&cinfo, TRUE);

	/* Finish compression, Save file and release memory */
	jpeg_finish_compress(&cinfo);
	fclose( jpegFile );
	jpeg_destroy_compress( &cinfo );				
}


/**
 * Function name: sdh_dbg_make_tif_test
 *
 * \brief Sindoh Debug Routine for SCAN
 *
 **/
void debug_make_tif_test()
{
    // Define an tifFile
    char buffer[25 * 144] = { 0x00, /* boring hex omitted */ };
    TIFF *image;

    // Open the TIFF file
    if((image = TIFFOpen("output.tif", "w")) == NULL){
      printf("Could not open output.tif for writing\n");
      exit(42);
    }

    // We need to set some values for basic tags before we can add any data
    TIFFSetField(image, TIFFTAG_IMAGEWIDTH, 25 * 8);    // set the width of the image
    TIFFSetField(image, TIFFTAG_IMAGELENGTH, 144);
    TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 1);      // set the size of the channels
    TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);    // set number of channels per pixel
    TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, 144);

    TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
    TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE /*PLANARCONFIG_CONTIG*/);

    TIFFSetField(image, TIFFTAG_XRESOLUTION, 150.0);
    TIFFSetField(image, TIFFTAG_YRESOLUTION, 150.0);
    TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

    // Write the information to the file
    TIFFWriteEncodedStrip(image, 0, buffer, 25 * 144);

    // Close the file
    TIFFClose(image);
}

int get_free_heap_mem(void)
{
	struct sysinfo info;
	
	do{
		sysinfo(&info);
		DBG_PRINTF_SDH_DEBUG("[meminfo] T%10d, F%10d, U%10d\n", info.totalram, info.freeram, info.totalram-info.freeram);
		if (info.freeram != 0)
			return info.freeram;
		usleep(100000);
	}while(1);
}

void scanner_backup_jpegFilename(uint8_t page_side, char *outfilename)
{
	if( page_side ) {
		memset(jpgFileNameForBackSide, 0x00, sizeof(jpgFileNameForBackSide));
		strcpy( jpgFileNameForBackSide, outfilename );
	}
	else {
		memset(jpgFileNameForFrontSide, 0x00, sizeof(jpgFileNameForFrontSide));
		strcpy( jpgFileNameForFrontSide, outfilename );	
	}
}

#if SDH_USE_SCAN_DOCUMENT_PASSWD
int scanner_pdf_get_password_string(char *passwdStr)
{	
    switch( uiJobSettings.scanToDestination ) {
		case UI_FILE_DEST_USBTHUMB:	//Scan To USB
			/* Password String */
			if(strlen(uiJobSettings.saveToFileInfo.documnetPasswd) != 0) {
				strncpy(passwdStr, uiJobSettings.saveToFileInfo.documnetPasswd, sizeof(uiJobSettings.saveToFileInfo.documnetPasswd));
			}		
			break;
		case UI_FILE_DEST_EMAIL:		//Scan To Email
			/* Password String */
			if(strlen(uiJobSettings.sendToEmailInfo.documnetPasswd) != 0) {
				strncpy(passwdStr, uiJobSettings.sendToEmailInfo.documnetPasswd, sizeof(uiJobSettings.sendToEmailInfo.documnetPasswd));
			}
			break;
		case UI_FILE_DEST_FTP:		//Scan To FTP
		case UI_FILE_DEST_SMB:		//SCan To SMB
		case UI_FILE_DEST_WEBDAV:	//SCan To SebDav
			/* Password String */
			if(strlen(uiJobSettings.sendToServerInfo.documnetPasswd) != 0) {
				strncpy(passwdStr, uiJobSettings.sendToServerInfo.documnetPasswd, sizeof(uiJobSettings.sendToServerInfo.documnetPasswd));
			}
			break;
		default:
			break;
	}
	
	return 0;
}
#endif

void scanner_pdf_make_error(SPDF_ERROR errval)
{
	DBG_PRINTF_SDH_DEBUG("scanner_pdf_make_error\n", errval);
	scanner_set_file_write_error(0);
	scanapp_log_event(SCANAPP_EVENT_SCAN_FAILED_FILE_WRITE);
	scanner_job_cancel_request();
}

void scanner_pdf_write_pageImage_spdf(uint8_t page_side)
{
	PSPDF_DOC gtSPDF = NULL;
	SPDF_ERROR rtn_val;
	char jpgFileName[FILENAME_MAX] = {0,};
	uint32_t scan_height_Pixel = 0;
	UINT32	pageBuffer_PixelWidth	= uiscan_self.pixels_per_row_padded;

	if (uiscan_stats.file_write_error != 0)
		return;

	if ((page_side) && (uiJobSettings.fileFormat == UI_FILE_PDF))
		gtSPDF = gtSPDF_Doc[PAGE_SIDE_BACK];
	else
		gtSPDF = gtSPDF_Doc[PAGE_SIDE_FRONT];
	
	if (page_side)
		memcpy( jpgFileName, jpgFileNameForBackSide, sizeof(jpgFileNameForBackSide));
	else
		memcpy( jpgFileName, jpgFileNameForFrontSide, sizeof(jpgFileNameForFrontSide));		
				
	scan_height_Pixel = (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100;

	if ((rtn_val=SPDF_SetResolution(gtSPDF, uiJobSettings.Resolution)) != SPDF_NO_ERROR )
	{
		scanner_pdf_make_error(rtn_val);
		return;
	}
	
	if ((rtn_val=SPDF_Page_SetWidth(gtSPDF, pageBuffer_PixelWidth)) != SPDF_NO_ERROR )
	{
		scanner_pdf_make_error(rtn_val);
		return;
	}

	if ((rtn_val=SPDF_Page_SetHeight(gtSPDF, scan_height_Pixel)) != SPDF_NO_ERROR )
	{
		scanner_pdf_make_error(rtn_val);
		return;
	}	
	
	if( (uiJobSettings.ScanType == e_MonochromeScan) && 
		(uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL || uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION) ) 
	{
		int dither_type;
		
		if ((rtn_val=SPDF_SetColorSpace(gtSPDF, SPDF_SOURCE_BW_1BPP)) != SPDF_NO_ERROR )
		{
			scanner_pdf_make_error(rtn_val);
			return;
		}	
		
		if( uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL ) {
			dither_type = SCAN_DITHERING_TEXT;
			DBG_PRINTF_SDH_DEBUG("B/W(Text) Mode\n");
		}
		else if( uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION ) {
			dither_type = SCAN_DITHERING_PHOTH;
			DBG_PRINTF_SDH_DEBUG("B/W(Photo) Mode\n");
		}
		
		scanData_gray2bw_fast(scanOnePageImageBuf[page_side].buf, pageBuffer_PixelWidth, scan_height_Pixel, 
							scan_height_Pixel, dither_type, SCAN_DATA_MINIMUM_VAL_IS_BLACK);
		
		if ((rtn_val=SPDF_LoadImageFromMem(gtSPDF, scanOnePageImageBuf[page_side].buf, (scanOnePageImageBuf[page_side].bufsize_bytes/8))) != SPDF_NO_ERROR )
		{
			scanner_pdf_make_error(rtn_val);
			return;
		}						
	}
	else 
	{
		if ( uiJobSettings.ScanType == e_ColorScan)
			rtn_val = SPDF_SetColorSpace(gtSPDF, SPDF_SOURCE_COLOR_24BPP);
		else
			rtn_val = SPDF_SetColorSpace(gtSPDF, SPDF_SOURCE_GRAY_8BPP);
		
		if (rtn_val != SPDF_NO_ERROR )
		{
			scanner_pdf_make_error(rtn_val);
			return;
		}	
		
		if ((rtn_val=SPDF_LoadImageFromFile(gtSPDF, jpgFileName)) != SPDF_NO_ERROR )
		{
			scanner_pdf_make_error(rtn_val);
			return;
		}	
		
#ifdef HAVE_IMAGELOG
		if (!g_nIsEnableScanImageLog)
		{
			scanner_erase_image_file(jpgFileName);
		}
#else
		scanner_erase_image_file(jpgFileName);
#endif
	}

	DBG_PRINTF_SDH_DEBUG("[RYU] %d pages, %d bytes(%d MB)\n", SPDF_GetPages(gtSPDF), SPDF_GetFileSize(gtSPDF), (SPDF_GetFileSize(gtSPDF)/(1024*1024)));
	if (SPDF_GetFileSize(gtSPDF) > PDF_MAX_FILE_SIZE)
	{
    	if (scanman_get_skipmsgfail() == false)
    	{
        	scan_err_t scerr;
        	scerr = scanmech_adf_client_request_pause();
        	scanapp_log_event(SCANAPP_EVENT_SCAN_STOP_MEMORY_FULL);
			scanman_set_skipmsgfail(true);
		}
    }
}

static void scanner_pdf_clear_doc_spdf(uint8_t page_side)
{
	if (gtSPDF_Doc[page_side] != NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : Doc(side%d) is not NULL\n", __FUNCTION__, page_side);
		SPDF_ClearSPDFResource(gtSPDF_Doc[page_side]);
		gtSPDF_Doc[page_side] = NULL;
	}
}

void scanner_pdf_set_password_spdf(uint8_t page_side)
{
	/* set password */
	SPDF_ERROR rtn_val;
	char ownerPasswd[SDH_SCAN_DOCUMENT_PASSWD_MAX] = {0x00,};
	char userPasswd[SDH_SCAN_DOCUMENT_PASSWD_MAX] = {0x00,};
	scanner_pdf_get_password_string( ownerPasswd );

	if (strlen(ownerPasswd) != 0)
	{
		memcpy(userPasswd, ownerPasswd, sizeof(ownerPasswd));
		userPasswd[0]++;
		if ((rtn_val=SPDF_SetPassword (gtSPDF_Doc[page_side], ownerPasswd, userPasswd)) != SPDF_NO_ERROR)
		{
			scanner_pdf_make_error(rtn_val);
		}
		else
		{	
			DBG_PRINTF_SDH_DEBUG("PDF security PW : %s\n", ownerPasswd);
		}
		
		//SPDF_SetPermission(gtSPDF_Doc[page_side], SPDF_ENABLE_READ | SPDF_ENABLE_PRINT | /*SPDF_ENABLE_EDIT_ALL |*/ SPDF_ENABLE_COPY | SPDF_ENABLE_EDIT);
	}
	else
	{
		DBG_PRINTF_SDH_DEBUG("secure PDF owner PW is empty\n");
	}	
}

void scanner_pdf_open_spdf(uint8_t page_side)
{
	if (uiscan_stats.file_write_error != 0 )
		return;

	if(uiJobSettings.fileFormat == UI_FILE_PDF_MULTI_PAGE && pdf_documnet_started == false) {
		pdf_documnet_started = true;
		memset(pdfFileNameForFrontSide, 0x00, sizeof(pdfFileNameForFrontSide));
		make_output_filename( pdfFileNameForFrontSide, UI_FILE_PDF_MULTI_PAGE, uiscan_stats.page_starts, uiJobSettings.DataType, page_side );
		scanner_pdf_clear_doc_spdf(PAGE_SIDE_FRONT);
		gtSPDF_Doc[PAGE_SIDE_FRONT] = SPDF_Open(pdfFileNameForFrontSide, "/tmp/ui_scan/fkids.sh", "/tmp/ui_scan/foffset.sh");
		if (gtSPDF_Doc[PAGE_SIDE_FRONT] == NULL)
		{
			DBG_PRINTF_SDH_DEBUG("%s gtSPDF_Doc side0 is NULL\n",__FUNCTION__);
			scanner_pdf_make_error(SPDF_FILE_OPEN_SAVEFILE_ERROR);
			return;
		}
		else
		{
			DBG_PRINTF_SDH_DEBUG("scanner_pdf_open_spdf:UI_FILE_PDF_MULTI_PAGE(Page:%d)\n", uiscan_stats.page_starts);
			if( uiJobSettings.isSecurityDocument == true ) {
				scanner_pdf_set_password_spdf(PAGE_SIDE_FRONT);
			}
		}
	}
	else if(uiJobSettings.fileFormat == UI_FILE_PDF) {

		scanner_pdf_clear_doc_spdf(page_side);
		
		if (page_side)
		{
			memset(pdfFileNameForBackSide, 0x00, sizeof(pdfFileNameForBackSide));
			make_output_filename( pdfFileNameForBackSide, UI_FILE_PDF_MULTI_PAGE, uiscan_stats.page_starts, uiJobSettings.DataType, page_side );
			gtSPDF_Doc[page_side] = SPDF_Open(pdfFileNameForBackSide, "/tmp/ui_scan/fkids_b.sh", "/tmp/ui_scan/foffset_b.sh");
		}
		else
		{
			memset(pdfFileNameForFrontSide, 0x00, sizeof(pdfFileNameForFrontSide));
			make_output_filename( pdfFileNameForFrontSide, UI_FILE_PDF_MULTI_PAGE, uiscan_stats.page_starts, uiJobSettings.DataType, page_side );
			gtSPDF_Doc[page_side] = SPDF_Open(pdfFileNameForFrontSide, "/tmp/ui_scan/fkids_f.sh", "/tmp/ui_scan/foffset_f.sh");				
		}
		
		if (gtSPDF_Doc[page_side] == NULL)
		{
			DBG_PRINTF_SDH_DEBUG("%s gtSPDF_Doc side%d is NULL\n",__FUNCTION__, page_side);
			scanner_pdf_make_error(SPDF_FILE_OPEN_SAVEFILE_ERROR);
			return;
		}
		else
		{
			DBG_PRINTF_SDH_DEBUG("scanner_pdf_open_spdf:UI_FILE_PDF_MULTI_PAGE(Page:%d)\n", uiscan_stats.page_starts);
			if( uiJobSettings.isSecurityDocument == true ) {
				scanner_pdf_set_password_spdf(page_side);
			}
		}
		DBG_PRINTF_SDH_DEBUG("scanner_pdf_open_spdf:UI_FILE_PDF(Page:%d)\n", uiscan_stats.page_starts);
	}
}
void scanner_pdf_close_spdf(uint8_t page_side)
{
	SPDF_ERROR rtn_val;
	
	if (gtSPDF_Doc[page_side] != NULL)
	{
		if ((rtn_val = SPDF_Close(gtSPDF_Doc[page_side])) != SPDF_NO_ERROR)
		{
			scanner_pdf_make_error(rtn_val);
		}
	}
	gtSPDF_Doc[page_side] = NULL;

	pdf_documnet_started = false;
}

void scanner_tiff_set_field(uint8_t page_side)
{
	if(page_side) {
		/// We need to set some values for basic tags before we can add any data
		TIFFSetField(tifFile_backside, TIFFTAG_IMAGEWIDTH, (uiJobSettings.scan_window.right * uiJobSettings.Resolution)/100 );	// set the width of the image
		TIFFSetField(tifFile_backside, TIFFTAG_IMAGELENGTH, (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100 );	// set the height of the image
		
		if( uiJobSettings.ScanType == e_MonochromeScan ) {
			if( uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL || uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION) {
				TIFFSetField(tifFile_backside, TIFFTAG_BITSPERSAMPLE, 1); //Black & White
				/* setup compress */
				TIFFSetField(tifFile_backside, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
				TIFFSetField(tifFile, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
			}
			else {
				TIFFSetField(tifFile_backside, TIFFTAG_BITSPERSAMPLE, 8); //Gray
				/* setup compress */
				TIFFSetField(tifFile_backside, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
				TIFFSetField(tifFile_backside, TIFFTAG_JPEGPROC, JPEGPROC_BASELINE);
				TIFFSetField(tifFile_backside, TIFFTAG_JPEGQUALITY, 50);
				TIFFSetField(tifFile_backside, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RAW);
				TIFFSetField(tifFile, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
			}
			TIFFSetField(tifFile_backside, TIFFTAG_SAMPLESPERPIXEL, 1); //Black & White, Gray
		}
		else if( uiJobSettings.ScanType == e_ColorScan ) {
			/* setup compress */
			TIFFSetField(tifFile_backside, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
			TIFFSetField(tifFile_backside, TIFFTAG_JPEGPROC, JPEGPROC_BASELINE);
			TIFFSetField(tifFile_backside, TIFFTAG_JPEGQUALITY, 50);
			TIFFSetField(tifFile_backside, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
						
			TIFFSetField(tifFile_backside, TIFFTAG_BITSPERSAMPLE, 8);
			TIFFSetField(tifFile_backside, TIFFTAG_SAMPLESPERPIXEL, 3); //RGB
			TIFFSetField(tifFile_backside, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		}
		TIFFSetField(tifFile_backside, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);		
		TIFFSetField(tifFile_backside, TIFFTAG_ROWSPERSTRIP, (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100);
		TIFFSetField(tifFile_backside, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
		TIFFSetField(tifFile_backside, TIFFTAG_XRESOLUTION, (float)uiJobSettings.Resolution);
		TIFFSetField(tifFile_backside, TIFFTAG_YRESOLUTION, (float)uiJobSettings.Resolution);
		TIFFSetField(tifFile_backside, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
				
		if(uiJobSettings.fileFormat == UI_FILE_TIFF_MULTI_PAGE) {
			/* We are writing single page of the multipage file */
			TIFFSetField(tifFile_backside, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
		}
	}
	else {
		/// We need to set some values for basic tags before we can add any data
		TIFFSetField(tifFile, TIFFTAG_IMAGEWIDTH, (uiJobSettings.scan_window.right * uiJobSettings.Resolution)/100 );	// set the width of the image
		TIFFSetField(tifFile, TIFFTAG_IMAGELENGTH, (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100 );	// set the height of the image
		
		///A bit of terminology
		/* Images are made up of pixels. In black and white imaging, 
		 * the pixel has one of two values, 0 or 1. This can be represented in a single bit. 
		 * For grayscale and color images however, the pixel needs to store a much greater range of values; 
		 * if a pixel was to have 255 levels of gray, we would need 8 bits to store that pixel. 
		 * Each of these values is called a sample. 
		 * TIFF expresses the size of the value in a tag called TIFFTAG_BITSPERSAMPLE. 
		 * This will be 1 for black and white, and some larger number for grayscale.
		 * For color images, we need to store even more information. 
		 * For each pixel we will need to store a red, green, and blue value. 
		 * Each of these values is stored in a separate sample. 
		 * Therefore, we will need to define TIFFTAG_SAMPLESPERPIXEL. 
		 * This will be 1 for black and white, or grayscale, but will normally be 3 for color images. 
		 * We also need to define the size of each sample, so we'll still need to set a value for TIFFTAG_BITSPERSAMPLE.
		*/
		
		///A bit of terminology about <TIFFTAG_PLANARCONFIG>
		/* The other interesting thing to discuss here is the planar configuration of the image. 
		 * Here I've specified PLANARCONFIG_CONTIG, 
		 * which means that the red green and blue information 
		 * for a given pixel is grouped together in the strips of image data. 
		 * The other option is PLANARCONFIG_SEPARATE, 
		 * where the red samples for the image are stored together, then the blue samples, 
		 * and finally the green samples.
		 */
		if( uiJobSettings.ScanType == e_MonochromeScan ) {
			if( uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL || uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION) {
				TIFFSetField(tifFile, TIFFTAG_BITSPERSAMPLE, 1);	//Black & White
				/* setup compress */
				TIFFSetField(tifFile, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
				TIFFSetField(tifFile, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);	
			}
			else {
				TIFFSetField(tifFile, TIFFTAG_BITSPERSAMPLE, 8); //Gray
				/* setup compress */
				TIFFSetField(tifFile, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
				TIFFSetField(tifFile, TIFFTAG_JPEGPROC, JPEGPROC_BASELINE);
				TIFFSetField(tifFile, TIFFTAG_JPEGQUALITY, 50);
				TIFFSetField(tifFile, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RAW);
				//TIFFSetField(tifFile, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
				TIFFSetField(tifFile, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
			}
			TIFFSetField(tifFile, TIFFTAG_SAMPLESPERPIXEL, 1); //Black & White, Gray
		}
		else if( uiJobSettings.ScanType == e_ColorScan ) {
			/* setup compress */
			TIFFSetField(tifFile, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
			TIFFSetField(tifFile, TIFFTAG_JPEGPROC, JPEGPROC_BASELINE);
			TIFFSetField(tifFile, TIFFTAG_JPEGQUALITY, 50);
			TIFFSetField(tifFile, TIFFTAG_JPEGCOLORMODE, JPEGCOLORMODE_RGB);
			
			TIFFSetField(tifFile, TIFFTAG_BITSPERSAMPLE, 8);
			TIFFSetField(tifFile, TIFFTAG_SAMPLESPERPIXEL, 3); //RGB
			TIFFSetField(tifFile, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		}
		TIFFSetField(tifFile, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);		
		TIFFSetField(tifFile, TIFFTAG_ROWSPERSTRIP, (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100);
		TIFFSetField(tifFile, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
		TIFFSetField(tifFile, TIFFTAG_XRESOLUTION, (float)uiJobSettings.Resolution);
		TIFFSetField(tifFile, TIFFTAG_YRESOLUTION, (float)uiJobSettings.Resolution);
		TIFFSetField(tifFile, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
				
		if(uiJobSettings.fileFormat == UI_FILE_TIFF_MULTI_PAGE) {
			/* We are writing single page of the multipage file */
			TIFFSetField(tifFile, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
		}
	}
}

int scanner_tiff_write_scanline(TIFF *tifFilePoint, uint8_t *buf, uint16_t writeStart_rowNumber, uint16_t rows_count, uint8_t page_side)
{				
	/// Write image data into the TIFF file packet by packet (line by line)
	//DBG_PRINTF_SDH_DEBUG(" : TIF_write_scanlines -> %d \n", rows_count);

	uint16_t row_idx = 0;
	UINT32	pageBuffer_PixelWidth	= uiscan_self.pixels_per_row_padded;
	UINT32	pageBuffer_PixelHeight	= (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100;
	
	DBG_PRINTF_SDH_DEBUG("TIFF Write Pixel(W:%d,H:%d), StartRow:%d, RowCount:%d\n", pageBuffer_PixelWidth, pageBuffer_PixelHeight, writeStart_rowNumber, rows_count);
	
	/*
	 **************************************************************************************************************
	 *	For Writing scaned image buffer into tiff file Pointer.
	 *		#1.	SCAN_DATA_TYPE_BILEVEL(B/W Text), SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION(B/W Photo)
	 * 			1) data process for Bi-Level data.
	 * 		#2.	SCAN_DATA_TYPE_RGB | SCAN_DATA_TYPE_MONO
	 * 			1) Write image buffer data into tiff file pointer.
	 **************************************************************************************************************
	 */	
	if( uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL || uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION) {
		if( uiJobSettings.DataType == SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION) {
			scanData_gray2bw(buf, pageBuffer_PixelWidth, pageBuffer_PixelHeight, pageBuffer_PixelHeight, SCAN_DITHERING_PHOTH, SCAN_DATA_MINIMUM_VAL_IS_WHITE);
		}
		else {
			scanData_gray2bw(buf, pageBuffer_PixelWidth, pageBuffer_PixelHeight, pageBuffer_PixelHeight, SCAN_DITHERING_TEXT, SCAN_DATA_MINIMUM_VAL_IS_WHITE);
		}
		for(row_idx=0; row_idx < rows_count; row_idx++) {
			TIFFWriteScanline( tifFilePoint, &buf[row_idx * (uiscan_self.bytes_per_row/8)], row_idx + writeStart_rowNumber, 0 );
		}
	}
	else {
		for(row_idx=0; row_idx < rows_count; row_idx++) {
			TIFFWriteScanline( tifFilePoint, &buf[row_idx * uiscan_self.bytes_per_row], row_idx + writeStart_rowNumber, 0 );
		}
	}

	//control the index of tiff scanwrite number
	if(page_side) {
		tifWriteScanLineRowIdx_backside += rows_count;
		//DBG_PRINTF_SDH_DEBUG(" : TIF_write_scanlines END-> %d, Total:%d \n", rows_count, tifWriteScanLineRowIdx_backside);
	}
	else {
		tifWriteScanLineRowIdx += rows_count;
		//DBG_PRINTF_SDH_DEBUG(" : TIF_write_scanlines END-> %d, Total:%d \n", rows_count, tifWriteScanLineRowIdx);
	}
	
	return 0;
}

void scanner_tiff_close(uint8_t page_side)
{
	if( page_side ) {
		if( tifFile_backside != NULL ) {
			TIFFClose( tifFile_backside );
			tifFile_backside = NULL;
			tifWriteScanLineRowIdx_backside = 0; //initialize tif scanline of Backside write row index
			DBG_PRINTF_SDH_DEBUG("tifFile_backside CLOSED[%d]\n", page_side);
		}
		else {
			DBG_PRINTF_SDH_DEBUG("tifFile_backside is NULL[%d]\n", page_side);
		}
	}
	else {
		if( tifFile != NULL ) {			
			TIFFClose( tifFile );
			tifFile = NULL;
			tifWriteScanLineRowIdx = 0; //initialize tif scanline write row index
			DBG_PRINTF_SDH_DEBUG("tifFile CLOSED[%d]\n", page_side);
		}
		else {
			DBG_PRINTF_SDH_DEBUG("tifFile is NULL[%d]\n", page_side);
		}
	}
	system("sync");
}

void scanner_jpeg_set_field(uint8_t page_side, FILE *outfilePoint)
{
	if( page_side ) {
		cinfo_backside.err = jpeg_std_error(&jerr_backside);				
		jpeg_create_compress(&cinfo_backside);
		jpeg_stdio_dest( &cinfo_backside, outfilePoint);

		///Set JPEG File Width, Height (Pixel) : width length(inch) * dpi --> [image width and height to pixels]
		/// e.g : Letter : 850*1100(8.5*11 inch) -> 300 DPI Mode -> [8.5*300 / 11*300] = [2550, 3300]
		cinfo_backside.image_width = (uiJobSettings.scan_window.right * uiJobSettings.Resolution)/100;
		cinfo_backside.image_height = (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100;

		if( uiJobSettings.ScanType == e_MonochromeScan ) {
			cinfo_backside.input_components = 1;             	// # of color components per pixel
			cinfo_backside.in_color_space = JCS_GRAYSCALE;  	//colorspace of input image
		}
		else if ( uiJobSettings.ScanType == e_ColorScan) {
			cinfo_backside.input_components = 3;         		// # of color components per pixel
			cinfo_backside.in_color_space = JCS_RGB;     	// colorspace of input image
		}
		jpeg_set_defaults(&cinfo_backside);	//Located in "jcparam.c"
		
		switch( uiJobSettings.jpegQuality ) {
			case UI_SCAN_JPEG_QUALITY_LOW:
				jpeg_set_quality(&cinfo_backside, JPEG_LIB_QUALITY_SET_VAL_LOW, TRUE /* limit to baseline-JPEG values */);
				break;
			case UI_SCAN_JPEG_QUALITY_MID:
				jpeg_set_quality(&cinfo_backside, JPEG_LIB_QUALITY_SET_VAL_MID, TRUE /* limit to baseline-JPEG values */);
				break;
			case UI_SCAN_JPEG_QUALITY_HIGH:
				jpeg_set_quality(&cinfo_backside, JPEG_LIB_QUALITY_SET_VAL_HIGH, TRUE /* limit to baseline-JPEG values */);
				break;
			default:
				jpeg_set_quality(&cinfo_backside, JPEG_LIB_QUALITY_SET_VAL_MID, TRUE /* limit to baseline-JPEG values */);
				break;
		}
		cinfo_backside.dct_method=JDCT_FASTEST;
		cinfo_backside.write_JFIF_header = TRUE;
		cinfo_backside.density_unit= 1;
		cinfo_backside.X_density = uiJobSettings.Resolution;
		cinfo_backside.Y_density = uiJobSettings.Resolution;
		jpeg_start_compress(&cinfo_backside, TRUE);			
	}
	else {
		cinfo.err = jpeg_std_error(&jerr);				
		jpeg_create_compress(&cinfo);
		jpeg_stdio_dest( &cinfo, outfilePoint );

		///Set JPEG File Width, Height (Pixel) : width length(inch) * dpi --> [image width and height to pixels]
		/// e.g : Letter : 850*1100(8.5*11 inch) -> 300 DPI Mode -> [8.5*300 / 11*300] = [2550, 3300]
		cinfo.image_width = (uiJobSettings.scan_window.right * uiJobSettings.Resolution)/100;
		cinfo.image_height = (uiJobSettings.scan_window.bottom * uiJobSettings.Resolution)/100;

		if( uiJobSettings.ScanType == e_MonochromeScan ) {
			cinfo.input_components = 1;             	// # of color components per pixel
			cinfo.in_color_space = JCS_GRAYSCALE;  	//colorspace of input image
		}
		else if ( uiJobSettings.ScanType == e_ColorScan) {
			cinfo.input_components = 3;         		// # of color components per pixel
			cinfo.in_color_space = JCS_RGB;     	// colorspace of input image
		}
		jpeg_set_defaults(&cinfo);	//Located in "jcparam.c"
				
		switch( uiJobSettings.jpegQuality ) {
			case UI_SCAN_JPEG_QUALITY_LOW:
				jpeg_set_quality(&cinfo, JPEG_LIB_QUALITY_SET_VAL_LOW, TRUE /* limit to baseline-JPEG values */);
				break;
			case UI_SCAN_JPEG_QUALITY_MID:
				jpeg_set_quality(&cinfo, JPEG_LIB_QUALITY_SET_VAL_MID, TRUE /* limit to baseline-JPEG values */);
				break;
			case UI_SCAN_JPEG_QUALITY_HIGH:
				jpeg_set_quality(&cinfo, JPEG_LIB_QUALITY_SET_VAL_HIGH, TRUE /* limit to baseline-JPEG values */);
				break;
			default:
				jpeg_set_quality(&cinfo, JPEG_LIB_QUALITY_SET_VAL_MID, TRUE /* limit to baseline-JPEG values */);
				break;
		}
		cinfo.dct_method=JDCT_FASTEST;
		cinfo.write_JFIF_header = TRUE;
		cinfo.density_unit= 1;
		cinfo.X_density = uiJobSettings.Resolution;
		cinfo.Y_density = uiJobSettings.Resolution;
		jpeg_start_compress(&cinfo, TRUE);	
	}
	bjpeg_write_scanline[page_side] = false;
}

int scanner_jpeg_write_scanline(uint8_t page_side, uint8_t *buf, uint16_t rows_count)
{
	/// Write Raw Scanned Image Data into the <JPEG memory destination> or <JPEG file pointer>
	//DBG_PRINTF_SDH_DEBUG(" : jpeg_write_scanlines -> %d \n", num_rows);

	uint16_t row_idx = 0;
	for(row_idx=0; row_idx<rows_count; row_idx++)
	{
		JSAMPROW row_pointer[1];
		row_pointer[0] = &buf[row_idx * uiscan_self.bytes_per_row];
		if(page_side) {
			jpeg_write_scanlines(&cinfo_backside, row_pointer, 1); //located in 'jcapistd.c'
		}
		else {	
			jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
	}
	bjpeg_write_scanline[page_side] = true;
	return 0;
}

void scanner_jpeg_close(uint8_t page_side)
{
	scan_err_t scerr;
	if( output_file_list_lookup( &outfile_list, uiJobSettings.DataType, page_side ) != NULL ) {
		DBG_PRINTF_SDH_DEBUG( "%s (finish compress), (file close), (release compress). dtype=%d page_side=%d\n", __FUNCTION__, uiJobSettings.DataType, page_side );

		/// Finish JPEG compression
		if (bjpeg_write_scanline[page_side] == true)
		{
			if( page_side ) {
				jpeg_finish_compress(&cinfo_backside); //located in 'jcapimin.c'
			}
			else {
				jpeg_finish_compress(&cinfo);
			}
		}
		
		/// Close JPEG File
		scerr = output_file_list_fclose( &outfile_list, uiJobSettings.DataType, page_side );
		if( scerr != SCANERR_NONE ) {
			DBG_PRINTF_SDH_DEBUG( "%s failed to close file for dtype=%d page_side=%d\n", __FUNCTION__, uiJobSettings.DataType, page_side );
			scanner_set_file_write_error(0);
			if(State == UI_SCAN_APP_SCANNING) {
				scanner_job_cancel_request();
			}
		}
		XASSERT( scerr==SCANERR_NONE, scerr );

		/// Relese JPEG compression
		if( page_side ) {
			jpeg_destroy_compress( &cinfo_backside);
		}
		else {
			jpeg_destroy_compress( &cinfo );
		}
	}
	else {
		DBG_PRINTF_SDH_DEBUG( "%s there is no list lookup dtype=%d page_side=%d\n", __FUNCTION__, uiJobSettings.DataType, page_side );
	}
}

void scanner_jpeg_close_for_pdf(uint8_t page_side)
{

	if( page_side ) {
		/// Finish & Release JPEG compression
		jpeg_finish_compress(&cinfo_backside); //located in 'jcapimin.c'
		jpeg_destroy_compress(&cinfo_backside);
		DBG_PRINTF_SDH_DEBUG( "JPEG Source Finish & Released for PDF(side:%d)\n", page_side);		
	}
	else {
		/// Finish & Release JPEG compression
		jpeg_finish_compress(&cinfo);
		jpeg_destroy_compress(&cinfo);
		DBG_PRINTF_SDH_DEBUG( "JPEG Source Finish & Released for PDF(side:%d)\n", page_side);
	}
}
