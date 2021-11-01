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
#include "scanapp_save_file.h"
#ifdef HAVE_IMAGELOG
#include "imagelog.h"
#endif

#define DBG_PRFX "SCN SAVE: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
#define SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#define SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

static HPDF_Doc pdf_pointer = NULL;

/**
 * \brief SCAN_PDF_Start
 *
 */
HPDF_STATUS SCAN_PDF_Start(uint8_t *dataBuff, 
								unsigned long bufferSize, 
								unsigned int data_pixel_wid, 
								unsigned int data_pixel_height, 
								unsigned int pdf_width, 
								unsigned int pdf_height, 
								int pageNum, 
								int source_type, 
								int application_type)
{
	//step1 : make pdf pointer (documnet)
	if( pdf_pointer == NULL ) {
		SCAN_PDF_NewDoc();
	}
	
	if( pdf_pointer != NULL ) {
		//step2 : make new page
		SCAN_PDF_NewPage(pdf_width, pdf_height);
		
		//step3 : write scan data to pdf page
		SCAN_PDF_Draw_Raw_Image(dataBuff, 
									bufferSize, 
									data_pixel_wid, 
									data_pixel_height, 
									pdf_width,
									pdf_height, 
									source_type,
									application_type);
	}

	return HPDF_OK;
}

/**
 * \brief SCAN_PDF_NewDoc
 *
 */
HPDF_STATUS SCAN_PDF_NewDoc()
{
	HPDF_Error_Handler error_handler = NULL;

	/* Make a new pdf object. */
	pdf_pointer = HPDF_New(error_handler, NULL);

	if( !pdf_pointer ){
		SDH_ERR("error: cannot create pdf handle");
		return -1;
	}
	else {
		SDH_DEBUG("HPDF New Documnet Created\n");
	}
	
	HPDF_SetCompressionMode(pdf_pointer, HPDF_COMP_IMAGE);

	return HPDF_OK;
}

/**
 * \brief scanner request image log start 
 *
 */
HPDF_STATUS SCAN_PDF_NewPage(unsigned int pdf_width, unsigned int pdf_height)
{
    HPDF_Page page;
    HPDF_Destination dst;

	if( pdf_pointer != NULL ) {
		/* Add a new page object. */
		page = HPDF_AddPage( pdf_pointer );
		dst = HPDF_Page_CreateDestination (page);
		ASSERT(page != NULL);
		HPDF_Page_SetWidth(page, pdf_width);
		HPDF_Page_SetHeight(page, pdf_height);
	}

	return HPDF_OK;
}

/**
 * \brief SCAN_PDF_Draw_Raw_Image
 *
 */
HPDF_STATUS SCAN_PDF_Draw_Raw_Image(	uint8_t* dataBuffer, 
											unsigned long dataBufferSize, 
											unsigned int data_pixel_wid, 
											unsigned int data_pixel_height, 
											unsigned int pdf_draw_width, 
											unsigned int pdf_draw_height,
											int source_type, 
											int application_type 
										)
{
	HPDF_Image image = NULL;
	HPDF_Page page = HPDF_GetCurrentPage( pdf_pointer );

	switch(source_type) {
		case SCAN_ORIGNAL_SOUCE_RAW_8BPP_MEM:
			/* Load the raw data (8bpp) To (1bpp) */
			if(application_type == SAVEFILE_APPLICATION_TYPE_OF_SCAN_FAX_SCAN) {
				scanData_gray2bw(dataBuffer, 
										data_pixel_wid, 
										data_pixel_height, 
										data_pixel_height, 
										SCAN_DITHERING_TEXT, 
										SCAN_DATA_MINIMUM_VAL_IS_BLACK);
										
				image = HPDF_LoadRawImageFromMem(
												pdf_pointer, 
												dataBuffer,
												data_pixel_wid,
												data_pixel_height,
												HPDF_CS_DEVICE_GRAY,
												1,
												TRUE);
			}
			break;
		default:
			break;
	}

	if(image != NULL) {
		// Draw Image into PDF File
		if(HPDF_OK == HPDF_Page_DrawImage(page, image, 0, 0, pdf_draw_width, pdf_draw_height)) {
			SDH_DEBUG("HPDF_Page_DrawImage : %d, %d\n", pdf_draw_width, pdf_draw_width);
		}
		else {
			SDH_ERR("HPDF_Page_DrawImage : Write fail\n");
		}
	}
	else {
		SDH_ERR("PDF_DrawImage : No raw image data\n");	
	}
	
	return HPDF_OK;
}

/**
 * \brief SCAN_PDF_SaveDoc
 *
 */
HPDF_STATUS SCAN_PDF_SaveDoc(char *pdf_file_name)
{
	if( pdf_pointer != NULL ) {
		HPDF_SaveToFile( pdf_pointer, pdf_file_name );
		HPDF_Free( pdf_pointer );
		pdf_pointer = NULL;
		system("sync");

		SDH_DEBUG("HPDF PDF Save Doc : %s\n", pdf_file_name);
	}
	else {
		SDH_DEBUG("HPDF pdf_pointer is NULL\n");
	}

	return HPDF_OK;
}

