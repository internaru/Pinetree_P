#ifndef SCANAPP_IMAGELOG_H
#define SCANAPP_IMAGELOG_H

#ifdef EXTERN
#undef EXTERN
#endif

#include <stdint.h>
#include "print_job_types.h"
#include "hpdf.h"
#include "hpdf_doc.h"

#define SCAN_IMAGELOG_FOLDER_PATH_MAX	255
#define IMAGELOG_FILENAME_LENGTH 300

bool SCAN_IL_IS_enable(void);
HPDF_STATUS SCAN_IL_PDF_Start(uint8_t *dataBuff, unsigned long bufferSize, unsigned int data_pixel_wid, unsigned int data_pixel_height, unsigned int pdf_width, unsigned int pdf_height, int pageNum, int source_type, int application_type);
HPDF_STATUS SCAN_IL_PDF_NewDoc();
HPDF_STATUS SCAN_IL_PDF_NewPage(unsigned int pdf_width, unsigned int pdf_height);
HPDF_STATUS SCAN_IL_PDF_Draw_Raw_Image(uint8_t* dataBuffer, unsigned long dataBufferSize, unsigned int data_pixel_wid, unsigned int data_pixel_height, unsigned int pdf_draw_width, unsigned int pdf_draw_height, int source_type, int application_type);
HPDF_STATUS SCAN_IL_PDF_SaveDoc(int pdf_file_number, int application_type);
int SCAN_IL_PDF_Set_JPEG_compress(int color_space, unsigned int wid_pixel, unsigned int height_pixel);
int SCAN_IL_PDF_Start_JPEG_file(char *jpeg_file_name, unsigned int pdf_width, unsigned int pdf_height, int pageNum);
int SCAN_IL_PDF_Start_PDF_file(char *pdf_file_name, unsigned int pdf_width, unsigned int pdf_height, int pageNum);
int SCAN_IL_PDF_Write_JPEG_compress(uint8_t *buf, uint16_t byte_per_row, uint16_t rows_count);
int SCAN_IL_PDF_JPEG_Finish();

typedef enum {
	IMAGELOG_ORIGNAL_SOUCE_NONE = 0,
	IMAGELOG_ORIGNAL_SOUCE_PDF = 1,
	IMAGELOG_ORIGNAL_SOUCE_TIFF = 2,
	IMAGELOG_ORIGNAL_SOUCE_JPEG = 3,
} IMAGELOG_ORIGNAL_SOUCE_FILE_FORMAT;

typedef enum {
	IMAGELOG_ORIGNAL_SOUCE_RAW_1BPP_MEM = 0,
	IMAGELOG_ORIGNAL_SOUCE_RAW_8BPP_MEM = 1,
	IMAGELOG_ORIGNAL_SOUCE_RAW_24BPP_MEM = 2,
} IMAGELOG_ORIGNAL_SOUCE_MEMORY_FORMAT;

typedef enum {
	IMAGELOG_APPLICATION_TYPE_OF_SCAN_NONE = 0,
	IMAGELOG_APPLICATION_TYPE_OF_SCAN_UI_SCAN = 1,
	IMAGELOG_APPLICATION_TYPE_OF_SCAN_PC_SCAN = 2,
	IMAGELOG_APPLICATION_TYPE_OF_SCAN_FAX_SCAN = 3,
} IMAGELOG_APPLICATION_TYPE_OF_SCAN;


typedef struct scan_il_struct
{
	int 		pdf_width;
	int 		pdf_height;
	int 		source_type;
	bool		isEndData;
	uint8_t	*data;
}SCAN_IL_STRUCT;

#endif // SCANAPP_IMAGELOG_H
