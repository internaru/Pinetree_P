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
#include "../hpdflib/include/hpdf.h"
#include "../hpdflib/include/hpdf_doc.h"
#include "jpeglib.h"
#include "db_api.h"
#include "a3mergeInterface.h"
#include "solution.h"
#include "SysUI_Interface.h"
#include "scanapp_margins.h"
#include "scanapp_imagelog.h"
#include "imagelog.h"

#define DBG_PRFX "SCN IL: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
#define SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#define SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

#ifdef HAVE_IMAGELOG

#ifdef HAVE_SDHFAX
extern UINT32 fsu_get_unique_job_id(UINT16 jnum);
extern UINT16 fsu_job_num_scan_image_log;
#endif

static HPDF_Doc pdf;
static unsigned char	*jpeg_mem_for_pdf = NULL;
static unsigned long	sizeof_jpeg_mem_for_pdf = 0;
static struct jpeg_compress_struct cinfo;
static struct jpeg_error_mgr jerr;

HPDF_STATUS SCAN_IL_PDF_Start(uint8_t *dataBuff, unsigned long bufferSize, unsigned int data_pixel_wid, unsigned int data_pixel_height, unsigned int pdf_width, unsigned int pdf_height, int pageNum, int source_type, int application_type)
{
	SCAN_IL_PDF_NewDoc();
	
	SCAN_IL_PDF_NewPage(pdf_width, pdf_height);
	
	SCAN_IL_PDF_Draw_Raw_Image(dataBuff, 
								bufferSize, 
								data_pixel_wid, 
								data_pixel_height, 
								pdf_width, 
								pdf_height, 
								source_type, 
								application_type);
								
	SCAN_IL_PDF_SaveDoc(pageNum, application_type);

	return HPDF_OK;
}

int SCAN_IL_PDF_Start_JPEG_file(char *jpeg_file_name, unsigned int pdf_width, unsigned int pdf_height, int pageNum)
{	
	SCAN_IL_PDF_NewDoc();
	
	SCAN_IL_PDF_NewPage(pdf_width, pdf_height);

	HPDF_Image image = NULL;
	HPDF_Page page = HPDF_GetCurrentPage(pdf);
	
	SDH_DEBUG("IL_HPDF_Page JPEG File path:%s\n", jpeg_file_name);
    image = HPDF_LoadJpegImageFromFile (pdf, jpeg_file_name);	
	
	if(image != NULL) {
		if(HPDF_OK == HPDF_Page_DrawImage(page, image, 0, 0, pdf_width, pdf_height)) {
			SDH_DEBUG("IL_HPDF_Page_DrawJPEG_File : %d, %d\n", pdf_width, pdf_height);
		}
		else {
			SDH_ERR("IL_HPDF_Page_DrawJPEG_File : Write fail: W(%d),H(%d)\n", pdf_width, pdf_height);
		}
	}
	else {
		SDH_ERR("IL_PDF_DrawImage : No JPEG File data\n");	
	}
								
	SCAN_IL_PDF_SaveDoc(pageNum, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN);

	return 0;
}

int SCAN_IL_PDF_Start_PDF_file(char *pdf_file_name, unsigned int pdf_width, unsigned int pdf_height, int pageNum)
{
	char dest_fileName[IMAGELOG_FILENAME_LENGTH];
	char *imglogFilename = get_il_filename(scanner_service_get_job_id());
	memset(dest_fileName, 0, sizeof(dest_fileName));
	snprintf(dest_fileName, IMAGELOG_FILENAME_LENGTH, "%s/%s/%s_%04d.pdf", IMAGE_LOG_PATH, imglogFilename, imglogFilename, pageNum);

	char sysCmd[MAX_SYSTEM_CMD_LENGTH] = {0x00,};
	sprintf(sysCmd, "cp %s %s", pdf_file_name, dest_fileName);
	system(sysCmd);
#if 0
	system("sync");
	
	SCAN_IL_PDF_SaveDoc(pageNum, IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN);
#else
//Authorize the file use Aria API
	ARIA_encode(dest_fileName, 0);
	system("sync");
#endif	
	return 0;
}

HPDF_STATUS SCAN_IL_PDF_NewDoc()
{
	/* Make a new pdf object. */	
	HPDF_Error_Handler error_handler = NULL;

	pdf = HPDF_New(error_handler, NULL);
	if(!pdf){
		SDH_ERR("error: cannot create pdf handle");
		return -1;
	}
	HPDF_SetCompressionMode(pdf, HPDF_COMP_IMAGE);

	return HPDF_OK;
}

HPDF_STATUS SCAN_IL_PDF_NewPage(unsigned int pdf_width, unsigned int pdf_height)
{
    HPDF_Page page;
    HPDF_Destination dst;

	if( pdf != NULL ) {
		/* Add a new page object. */
		page = HPDF_AddPage( pdf );
		dst = HPDF_Page_CreateDestination (page);
		ASSERT(page != NULL);
	}
	else
	{
		SDH_ERR("error: pdf is NULL");
		return -1;
	}
	
	HPDF_Page_SetWidth(page, pdf_width);
	HPDF_Page_SetHeight(page, pdf_height);

	return HPDF_OK;
}

HPDF_STATUS SCAN_IL_PDF_Draw_Raw_Image(	uint8_t* dataBuffer, 
												unsigned long dataBufferSize, 
												unsigned int data_pixel_wid, 
												unsigned int data_pixel_height, 
												unsigned int pdf_draw_width, 
												unsigned int pdf_draw_height, 
												int source_type, 
												int application_type)
{
	HPDF_Image image = NULL;
	HPDF_Page page = HPDF_GetCurrentPage(pdf);

	SDH_DEBUG("IL PDF Draw : Source is(%d), pixelWid(%d),PixelHeight(%d)\n", source_type, data_pixel_wid, data_pixel_height);
			
	switch(source_type) {
		case IMAGELOG_ORIGNAL_SOUCE_RAW_1BPP_MEM:
			/* Load the raw data (1bpp) */
				image = HPDF_LoadRawImageFromMem(pdf, dataBuffer,
												data_pixel_wid,
												data_pixel_height,
												HPDF_CS_DEVICE_GRAY,
												1,
												TRUE);
			break;
		case IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM:
			/* Load the raw data (8bpp) & 'JPEG compress' & Save */
			SCAN_IL_PDF_Set_JPEG_compress(e_MonochromeScan, data_pixel_wid, data_pixel_height);
			SCAN_IL_PDF_Write_JPEG_compress(dataBuffer, data_pixel_wid, data_pixel_height);
			SCAN_IL_PDF_JPEG_Finish();
			SDH_ERR("sizeof_jpeg_mem_for_pdf : %d\n", sizeof_jpeg_mem_for_pdf);
			image = HPDF_LoadJpegImageFromMem(pdf, jpeg_mem_for_pdf, sizeof_jpeg_mem_for_pdf);
			break;
		case IMAGELOG_ORIGNAL_SOUCE_RAW_24BPP_MEM:
			/* Load the raw data (24bpp) & 'JPEG compress' & Save */
			SCAN_IL_PDF_Set_JPEG_compress(e_ColorScan, data_pixel_wid, data_pixel_height);
			SCAN_IL_PDF_Write_JPEG_compress(dataBuffer, data_pixel_wid*3, data_pixel_height);
			SCAN_IL_PDF_JPEG_Finish();
			SDH_ERR("sizeof_jpeg_mem_for_pdf : %d\n", sizeof_jpeg_mem_for_pdf);
			image = HPDF_LoadJpegImageFromMem(pdf, jpeg_mem_for_pdf, sizeof_jpeg_mem_for_pdf);
			break;
		default:
			break;
	}

	if(image != NULL) {
		// Draw Image into PDF File
		if(HPDF_OK == HPDF_Page_DrawImage(page, image, 0, 0, pdf_draw_width, pdf_draw_height)) {
			SDH_DEBUG("IL_HPDF_Page_DrawImage : %d, %d\n", pdf_draw_width, pdf_draw_width);
		}
		else {
			SDH_ERR("IL_HPDF_Page_DrawImage : Write fail\n");
		}
	}
	else {
		SDH_ERR("IL_PDF_DrawImage : No raw image data\n");	
	}
	
	return HPDF_OK;
}

HPDF_STATUS SCAN_IL_PDF_SaveDoc(int pdf_file_number, int application_type)
{
	char fileName[IMAGELOG_FILENAME_LENGTH];
	char *imglogFilename;
	
	switch( application_type ) {
		case IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN:
		case IMAGELOG_APPLICATION_TYPE_OF_SCAN_PC_SCAN:
			imglogFilename = get_il_filename(scanner_service_get_job_id());
			break;
		case IMAGELOG_APPLICATION_TYPE_OF_SCAN_FAX_SCAN:
			imglogFilename = get_il_filename(fsu_get_unique_job_id(fsu_job_num_scan_image_log));
			break;			
		default:
			imglogFilename = get_il_filename(scanner_service_get_job_id());
			break;
	}
	memset(fileName, 0, sizeof(fileName));
	snprintf(fileName, IMAGELOG_FILENAME_LENGTH, "%s/%s/%s_%04d.pdf", IMAGE_LOG_PATH, imglogFilename, imglogFilename, pdf_file_number);
	HPDF_SaveToFile(pdf, fileName);
	SDH_ERR("PDF SAVE [%s]\n", fileName);
	HPDF_Free(pdf);

	//Authorize the file use Aria API
	ARIA_encode(fileName, 0);

	system("sync");
	SDH_ERR("PDF END\n");

	return HPDF_OK;
}

bool SCAN_IL_IS_enable(void)
{
	/* check image log is enable */
	bool isEnableImagelog_scan;
	isEnableImagelog_scan = IsImageLogEnabled('S', 0);
	
	return isEnableImagelog_scan;
}

int SCAN_IL_PDF_Set_JPEG_compress(int color_space, unsigned int wid_pixel, unsigned int height_pixel)
{
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_mem_dest(&cinfo, &jpeg_mem_for_pdf, &sizeof_jpeg_mem_for_pdf);

	cinfo.image_width = wid_pixel;
	cinfo.image_height = height_pixel;

	if( color_space == e_MonochromeScan ) {
		cinfo.input_components = 1;
		cinfo.in_color_space = JCS_GRAYSCALE;
	}
	else if (color_space == e_ColorScan) {
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
	}
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, JPEG_LIB_QUALITY_SET_VAL_MID, TRUE);
	cinfo.dct_method=JDCT_FASTEST;
	jpeg_start_compress(&cinfo, TRUE);
	
	return 0;
}

int SCAN_IL_PDF_Write_JPEG_compress(uint8_t *buf, uint16_t byte_per_row, uint16_t rows_count)
{
	uint16_t row_idx = 0;
	for(row_idx=0; row_idx<rows_count; row_idx++)
	{
		JSAMPROW row_pointer[1];
		row_pointer[0] = &buf[row_idx * byte_per_row];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	return 0;
}

int SCAN_IL_PDF_JPEG_Finish()
{
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	
	return 0;
}

#endif //HAVE_IMAGELOG

