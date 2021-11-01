#ifndef SCANAPP_SAVEFILE_H
#define SCANAPP_SAVEFILE_H

#ifdef EXTERN
#undef EXTERN
#endif

#include <stdint.h>
#include "print_job_types.h"
#include "hpdf.h"
#include "hpdf_doc.h"
#include "../../../../../../../common_header/SysSharedMemory.h"

#define 	MAX_SCAN_FILE_FULL_PATH		100

HPDF_STATUS SCAN_PDF_Start(uint8_t *dataBuff, unsigned long bufferSize, 
								unsigned int data_pixel_wid, unsigned int data_pixel_height, 
								unsigned int pdf_width, unsigned int pdf_height, 
								int pageNum, 
								int source_type,
								int application_type);
HPDF_STATUS SCAN_PDF_NewDoc();
HPDF_STATUS SCAN_PDF_NewPage(unsigned int pdf_width, unsigned int pdf_height);
HPDF_STATUS SCAN_PDF_Draw_Raw_Image( uint8_t* dataBuffer, unsigned long dataBufferSize, 
											unsigned int data_pixel_wid, unsigned int data_pixel_height, 
											unsigned int pdf_draw_width, unsigned int pdf_draw_height,
											int source_type, 
											int application_type );
											
HPDF_STATUS SCAN_PDF_SaveDoc(char *pdf_file_name);

typedef enum {
	SAVEFILE_SEPERATED_TYPE_NONE = 0,
	SAVEFILE_SEPERATED_TYPE_SEPERATE = 1,
	SAVEFILE_SEPERATED_TYPE_NOT_SEPERATE = 2,	
} SAVE_FILE_SEPERATED_TYPE;

typedef enum {
	SCAN_ORIGNAL_SOUCE_BPP_NONE = 0,
	SCAN_ORIGNAL_SOUCE_RAW_8BPP_MEM = 1,
	SCAN_ORIGNAL_SOUCE_RAW_24BPP_MEM = 2,
} SCAN_ORIGNAL_SOUCE_MEMORY_FORMAT;

typedef enum {
	SAVEFILE_APPLICATION_TYPE_OF_SCAN_NONE = 0,
	SAVEFILE_APPLICATION_TYPE_OF_SCAN_UI_SCAN = 1,
	SAVEFILE_APPLICATION_TYPE_OF_SCAN_PC_SCAN = 2,
	SAVEFILE_APPLICATION_TYPE_OF_SCAN_FAX_SCAN = 3,
} SAVE_FILE_APPLICATION_TYPE_OF_SCAN;

#endif // SCANAPP_SAVEFILE_H
