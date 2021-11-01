#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include "spdf_types.h"
#include "scantypes.h"
#include "scanapp_uiscan.h"
#include "scandbg.h"
#include "logger.h"
#include "make2pdf.h"
#include "faxapp_make2raw.h"

#define DBG_PRFX "SCN PDF: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)

static SPDF_UINT8 SPDF_WriteHeader(SPDF_DOC *gtSPDF_Doc);
static SPDF_UINT8 SPDF_WriteTrailer(SPDF_DOC *gtSPDF_Doc);
static SPDF_UINT32 SPDF_Write_Stream(SPDF_DOC *gtSPDF_Doc, SPDF_BYTE *buf, SPDF_UINT32 length, SPDF_UINT32 nObjects;);
static SPDF_UINT32 SPDF_Write_Binary(SPDF_DOC *gtSPDF_Doc, SPDF_BYTE *buf, SPDF_UINT32 length);

SPDF_UINT8	SPDF_SetColorSpace(SPDF_DOC *gtSPDF_Doc, SPDF_SOURCE_TYPE SourceType)
{
	if ((gtSPDF_Doc == NULL) || (gtSPDF_Doc->gtSPDF_PageData == NULL))
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_PAGE_DATA_CHECK_ERROR\n", __FUNCTION__);
		return SPDF_PAGE_DATA_CHECK_ERROR;
	}

	switch(SourceType)
	{
		case SPDF_SOURCE_BW_1BPP :
			gtSPDF_Doc->gtSPDF_PageData->ucBitDepth = 1;
			gtSPDF_Doc->gtSPDF_PageData->nTotalPlanes = 1;
			break;
		case SPDF_SOURCE_GRAY_8BPP :
			gtSPDF_Doc->gtSPDF_PageData->ucBitDepth = 8;
			gtSPDF_Doc->gtSPDF_PageData->nTotalPlanes = 1;
			break;
		case SPDF_SOURCE_COLOR_24BPP :
			gtSPDF_Doc->gtSPDF_PageData->ucBitDepth = 8;
			gtSPDF_Doc->gtSPDF_PageData->nTotalPlanes = 3;
			break;
		default:
			break;
	}
	return SPDF_NO_ERROR;
}

SPDF_UINT8 SPDF_SetResolution(SPDF_DOC *gtSPDF_Doc, SPDF_UINT32 nResolution)
{
	if ((gtSPDF_Doc == NULL) || (gtSPDF_Doc->gtSPDF_PageData == NULL))
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_PAGE_DATA_CHECK_ERROR\n", __FUNCTION__);
		return SPDF_PAGE_DATA_CHECK_ERROR;
	}
	
	gtSPDF_Doc->gtSPDF_PageData->nXResolution = nResolution;
	gtSPDF_Doc->gtSPDF_PageData->nYResolution = nResolution;
	
	return SPDF_NO_ERROR;
}

void hex_print( char *buf, int len)
{
	int nLoop;
	
	printf ("===============================\n");
	for (nLoop=0; nLoop<len; nLoop++)
	{
		printf ("%02X ", buf[nLoop]);
	}
	printf ("\n");
}

SPDF_UINT8 SPDF_SetPassword(SPDF_DOC *gtSPDF_Doc, const char *owner_passwd, const char *user_passwd)
{
	if (gtSPDF_Doc==NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_DOC_DATA_CHECK_ERROR\n", __FUNCTION__);
		return SPDF_DOC_DATA_CHECK_ERROR;
	}
	
	if (gtSPDF_Doc->bEncryptOn == FALSE)
	{
		gtSPDF_Doc->bEncryptOn = TRUE;
		gtSPDF_Doc->stEncryptDict = malloc(sizeof(SPDF_Encrypt_Rec));
		
		SPDF_Encrypt_Init(gtSPDF_Doc->stEncryptDict);
		
		if (SPDF_EncryptDict_SetPassword (gtSPDF_Doc->stEncryptDict, owner_passwd, user_passwd) != SPDF_NO_ERROR)
		{
			DBG_PRINTF_SDH_DEBUG("[RYU] SPDF_EncryptDict_SetPassword Error\n");
			return SPDF_COMMON_ERROR;
		}
		SPDF_EncryptDict_Prepare(gtSPDF_Doc->stEncryptDict);
	}
		
	//hex_print((char*)gtSPDF_Doc->stEncryptDict->owner_passwd, 32);
	//hex_print((char*)gtSPDF_Doc->stEncryptDict->user_passwd, 32);
	
	//hex_print((char*)gtSPDF_Doc->stEncryptDict->owner_key, 32);
	//hex_print((char*)gtSPDF_Doc->stEncryptDict->user_key, 32);
	
	//DBG_PRINTF_SDH_DEBUG("[RYU] permission : %d, %d\n", gtSPDF_Doc->stEncryptDict->permission, (SPDF_ENABLE_PRINT | SPDF_ENABLE_EDIT_ALL | SPDF_ENABLE_COPY | SPDF_ENABLE_EDIT | SPDF_PERMISSION_PAD));
	//DBG_PRINTF_SDH_DEBUG("[RYU] permission : %d\n", SPDF_ENABLE_PRINT + SPDF_ENABLE_EDIT_ALL + SPDF_ENABLE_COPY + SPDF_ENABLE_EDIT + SPDF_PERMISSION_PAD);
	return SPDF_NO_ERROR;
}

SPDF_UINT8 SPDF_SetPermission(SPDF_DOC *gtSPDF_Doc, SPDF_INT32 permission)
{			
	if (gtSPDF_Doc==NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_DOC_DATA_CHECK_ERROR\n", __FUNCTION__);
		return SPDF_DOC_DATA_CHECK_ERROR;
	}
	else if ((gtSPDF_Doc->bEncryptOn == FALSE) || (gtSPDF_Doc->stEncryptDict == NULL))
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_DOC_ENCRYPTION_ERROR\n", __FUNCTION__);
		return SPDF_DOC_ENCRYPTION_ERROR;
	}
	
	gtSPDF_Doc->stEncryptDict->permission = permission;
	SPDF_EncryptDict_Prepare(gtSPDF_Doc->stEncryptDict);
	DBG_PRINTF_SDH_DEBUG("%s : SPDF_SetPermission(%d)\n", __FUNCTION__, permission);
	return SPDF_NO_ERROR;
}

SPDF_UINT8	SPDF_Page_SetWidth(SPDF_DOC *gtSPDF_Doc, SPDF_UINT32 nWidth)
{
	if ((gtSPDF_Doc == NULL) || (gtSPDF_Doc->gtSPDF_PageData == NULL))
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_PAGE_DATA_CHECK_ERROR\n", __FUNCTION__);
		return SPDF_PAGE_DATA_CHECK_ERROR;
	}
	
	gtSPDF_Doc->gtSPDF_PageData->nWidth = nWidth;
	
	return SPDF_NO_ERROR;
}

SPDF_UINT8	SPDF_Page_SetHeight(SPDF_DOC *gtSPDF_Doc, SPDF_UINT32 nHeight)
{
	if ((gtSPDF_Doc == NULL) || (gtSPDF_Doc->gtSPDF_PageData == NULL))
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_PAGE_DATA_CHECK_ERROR\n", __FUNCTION__);
		return SPDF_PAGE_DATA_CHECK_ERROR;
	}
	
	gtSPDF_Doc->gtSPDF_PageData->nHeight = nHeight;
	
	return SPDF_NO_ERROR;
}

SPDF_UINT8 SPDF_Page_SetRotate(SPDF_DOC *gtSPDF_Doc, SPDF_UINT16 nRotate)
{
	DBG_PRINTF_SDH_DEBUG("%s : %p\n", __FUNCTION__, gtSPDF_Doc);
	
	if ((gtSPDF_Doc == NULL) || (gtSPDF_Doc->gtSPDF_PageData == NULL))
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_PAGE_DATA_CHECK_ERROR\n", __FUNCTION__);
		return SPDF_PAGE_DATA_CHECK_ERROR;
	}
	
	gtSPDF_Doc->gtSPDF_PageData->nRotate = nRotate;
	
	return SPDF_NO_ERROR;
}

SPDF_UINT32	SPDF_GetFileSize(SPDF_DOC *gtSPDF_Doc)
{
	if (gtSPDF_Doc!=NULL)
		return gtSPDF_Doc->nFilePosition;
	else
		return 0;
}

SPDF_UINT32	SPDF_GetPages(SPDF_DOC *gtSPDF_Doc)
{
	if (gtSPDF_Doc!=NULL)
		return gtSPDF_Doc->nPageNo;
	else
		return 0;
}

SPDF_VOID SPDF_ClearSPDFResource(SPDF_DOC *gtSPDF_Doc)
{
	DBG_PRINTF_SDH_DEBUG("%s Start\n", __FUNCTION__);
	
	if (gtSPDF_Doc!=NULL)
	{
		if (gtSPDF_Doc->fSaveFile != NULL)
		{
			fflush(gtSPDF_Doc->fSaveFile);
			fsync(fileno(gtSPDF_Doc->fSaveFile));
			fclose(gtSPDF_Doc->fSaveFile);
			gtSPDF_Doc->fSaveFile = NULL;
		}
		
		if (gtSPDF_Doc->fkids != NULL)
		{
			fclose(gtSPDF_Doc->fkids);
			if (gtSPDF_Doc->KidsFileName)
			{
				remove(gtSPDF_Doc->KidsFileName);
				free(gtSPDF_Doc->KidsFileName);
			}
			gtSPDF_Doc->fkids = NULL;
		}
		
		if (gtSPDF_Doc->foffsets != NULL)
		{
			fclose(gtSPDF_Doc->foffsets);
			if (gtSPDF_Doc->OffsetFileName)
			{
				remove(gtSPDF_Doc->OffsetFileName);
				free(gtSPDF_Doc->OffsetFileName);
			}
			gtSPDF_Doc->foffsets = NULL;
		}
		
		if (gtSPDF_Doc->gtSPDF_PageData != NULL)
		{
			free(gtSPDF_Doc->gtSPDF_PageData);
			gtSPDF_Doc->gtSPDF_PageData = NULL;
		}
		
		if ((gtSPDF_Doc->bEncryptOn) && (gtSPDF_Doc->stEncryptDict))
		{
			free(gtSPDF_Doc->stEncryptDict);
			gtSPDF_Doc->stEncryptDict = NULL;
		}
		
		free(gtSPDF_Doc);
		gtSPDF_Doc = NULL;
	}
}

PSPDF_DOC SPDF_Open(const char *FileName, const char *KidsFileName, const char *OffsetFileName)
{
	PSPDF_DOC gtSPDF_Doc;
	UINT32 length;

	DBG_PRINTF_SDH_DEBUG("%s Start\n", __FUNCTION__);
	
	if ((strlen(FileName) == 0) || (strlen(KidsFileName) == 0) || (strlen(OffsetFileName) == 0))
	{
		DBG_PRINTF_SDH_DEBUG("%s ERROR\n", __FUNCTION__);
		return NULL;
	}
	
	gtSPDF_Doc = malloc(sizeof(SPDF_DOC));
	memset(gtSPDF_Doc, 0x00, sizeof(SPDF_DOC));
	gtSPDF_Doc->gtSPDF_PageData = malloc(sizeof(SPDF_PAGE_DATA));
	memset(gtSPDF_Doc->gtSPDF_PageData, 0x00, sizeof(SPDF_PAGE_DATA));
	
	gtSPDF_Doc->nPageNo = 0 ;
	gtSPDF_Doc->nObjects = 2;
	gtSPDF_Doc->nFilePosition = 0;
	gtSPDF_Doc->bEncryptOn = FALSE;
	
	gtSPDF_Doc->KidsFileName = malloc(strlen(KidsFileName)+1);
	strcpy(gtSPDF_Doc->KidsFileName, KidsFileName);
	gtSPDF_Doc->fkids = fopen(KidsFileName, "w+");
	
	if ( gtSPDF_Doc->fkids == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_OPEN_FKID_ERROR\n", __FUNCTION__);
		SPDF_ClearSPDFResource(gtSPDF_Doc);
		return NULL;
	}
	
	gtSPDF_Doc->OffsetFileName = malloc(strlen(OffsetFileName)+1);
	strcpy(gtSPDF_Doc->OffsetFileName, OffsetFileName);
	gtSPDF_Doc->foffsets = fopen(OffsetFileName, "w+");
	if (gtSPDF_Doc->foffsets == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_OPEN_FOFFSET_ERROR\n", __FUNCTION__);
		SPDF_ClearSPDFResource(gtSPDF_Doc);
		return 	NULL;
	}
	
	gtSPDF_Doc->fSaveFile = fopen(FileName, "wb");
	if (gtSPDF_Doc->fSaveFile == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_OPEN_SAVEFILE_ERROR\n", __FUNCTION__);
		SPDF_ClearSPDFResource(gtSPDF_Doc);
		return 	NULL;
	}
	
	length = fprintf(gtSPDF_Doc->fSaveFile, "%%PDF-1.4\012%%\267\276\255\252\012");
	
	gtSPDF_Doc->nFilePosition += length ;
	
	gtSPDF_Doc->gtSPDF_PageData->nRotate = 0;
	
	return gtSPDF_Doc;
}

SPDF_UINT8 SPDF_Close(SPDF_DOC *gtSPDF_Doc)
{
	SPDF_UINT32 length;
	SPDF_UINT32 i;
	SPDF_UINT32 nkids, noffset;
	SPDF_UINT32 offsets[3];		/**< Location of PDF objects */
		
	DBG_PRINTF_SDH_DEBUG("%s start\n", __FUNCTION__);
	
	if (gtSPDF_Doc == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_DOC_DATA_CHECK_ERROR\n", __FUNCTION__);
		return SPDF_DOC_DATA_CHECK_ERROR;
	}

	if (gtSPDF_Doc->fSaveFile == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_CHECK_SAVEFILE_ERROR\n", __FUNCTION__);
		SPDF_ClearSPDFResource(gtSPDF_Doc);
		return SPDF_FILE_CHECK_SAVEFILE_ERROR;
	}

	if ( gtSPDF_Doc->fkids == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_CHECK_FKID_ERROR\n", __FUNCTION__);
		SPDF_ClearSPDFResource(gtSPDF_Doc);
		return SPDF_FILE_CHECK_FKID_ERROR;
	}
	else
	{
		fflush(gtSPDF_Doc->fkids);
		fsync(fileno(gtSPDF_Doc->fkids));
		
		rewind(gtSPDF_Doc->fkids);
	}
	
	if ( gtSPDF_Doc->foffsets == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_CHECK_FOFFSET_ERROR\n", __FUNCTION__);
		SPDF_ClearSPDFResource(gtSPDF_Doc);
		return SPDF_FILE_CHECK_FOFFSET_ERROR;
	}
	else
	{
		fflush(gtSPDF_Doc->foffsets);
		fsync(fileno(gtSPDF_Doc->foffsets));
		
		rewind(gtSPDF_Doc->foffsets);
	}

	offsets[0] = gtSPDF_Doc->nFilePosition ;
	gtSPDF_Doc->nObjects++;
	
/* pages */		
	length = fprintf(gtSPDF_Doc->fSaveFile, "1 0 obj\012"
					"<<\012"
					"/Type /Pages\012"
					"/Kids [ ");

	gtSPDF_Doc->nFilePosition += length ;
					
	for ( i = 0 ; i < gtSPDF_Doc->nPageNo ; ++i )
	{
		fscanf(gtSPDF_Doc->fkids, "%d", &nkids);
		length = fprintf(gtSPDF_Doc->fSaveFile, "%u 0 R\015\012", nkids);
		gtSPDF_Doc->nFilePosition += length ;
	}

	length = fprintf(gtSPDF_Doc->fSaveFile, " ]\012"
					"/Count %d\012"
					">>\012"
					"endobj\012", gtSPDF_Doc->nPageNo);

	gtSPDF_Doc->nFilePosition += length;
	offsets[1] = gtSPDF_Doc->nFilePosition;
	gtSPDF_Doc->nObjects++;

	length = fprintf(gtSPDF_Doc->fSaveFile, "2 0 obj\012" /* Info */
					"<<\012"
					"/Type /Catalog\012"
					"/Pages 1 0 R\012"
					">>\012"
					"endobj\012");

	gtSPDF_Doc->nFilePosition += length ;

	if (gtSPDF_Doc->bEncryptOn == TRUE)
	{
		offsets[2] = gtSPDF_Doc->nFilePosition;
		gtSPDF_Doc->nObjects ++;
		length = fprintf(gtSPDF_Doc->fSaveFile,"%d 0 obj\012"
				"<<\012"
				"/O ", gtSPDF_Doc->nObjects-2);
		gtSPDF_Doc->nFilePosition += length ;
		
		length = SPDF_Write_Binary(gtSPDF_Doc, gtSPDF_Doc->stEncryptDict->owner_key, SPDF_PASSWD_LEN);
		gtSPDF_Doc->nFilePosition += length ;
		
		length = fprintf(gtSPDF_Doc->fSaveFile,"\012"
				"/U ");
		gtSPDF_Doc->nFilePosition += length ;
		
		length = SPDF_Write_Binary(gtSPDF_Doc, gtSPDF_Doc->stEncryptDict->user_key, SPDF_PASSWD_LEN);
		gtSPDF_Doc->nFilePosition += length ;
		
		length = fprintf(gtSPDF_Doc->fSaveFile,"\012"
				"/Filter /Standard\012"
				"/V 2\012"
				"/R 3\012"
				"/Length 128\012"
				"/P %d\012"
				">>\012"
				"endobj\012", gtSPDF_Doc->stEncryptDict->permission);
		gtSPDF_Doc->nFilePosition += length ;
	}
	
	length = fprintf(gtSPDF_Doc->fSaveFile, "xref\012"
					"0 %d\012"
					"0000000000 65535 f\015\012",
					gtSPDF_Doc->nObjects - 1);

	length = fprintf(gtSPDF_Doc->fSaveFile, "%010d 00000 n\015\012", offsets[0]);
	
	length = fprintf(gtSPDF_Doc->fSaveFile, "%010d 00000 n\015\012", offsets[1]);

	fscanf(gtSPDF_Doc->foffsets, "%d", &noffset);
	while(!feof(gtSPDF_Doc->foffsets))
	{
		length = fprintf(gtSPDF_Doc->fSaveFile, "%010d 00000 n\015\012", noffset);
		fscanf(gtSPDF_Doc->foffsets, "%d", &noffset);
	}
	
	if (gtSPDF_Doc->bEncryptOn == TRUE)
	{	
		length = fprintf(gtSPDF_Doc->fSaveFile, "%010d 00000 n\015\012", offsets[2]);
	}
	
	length = fprintf(gtSPDF_Doc->fSaveFile,
					"trailer\012"
					"<<\012"
					"/Size %d\012"
					"/Root 2 0 R\012", gtSPDF_Doc->nObjects-1);
					
	if (gtSPDF_Doc->bEncryptOn == TRUE)
	{
		length = fprintf(gtSPDF_Doc->fSaveFile,
				"/Encrypt %d 0 R\012"
				"/ID [ ", gtSPDF_Doc->nObjects-2);
		
		length = SPDF_Write_Binary(gtSPDF_Doc, gtSPDF_Doc->stEncryptDict->encrypt_id, SPDF_ID_LEN);
		length = fprintf(gtSPDF_Doc->fSaveFile," ");
		length = SPDF_Write_Binary(gtSPDF_Doc, gtSPDF_Doc->stEncryptDict->encrypt_id, SPDF_ID_LEN);
		
		length = fprintf(gtSPDF_Doc->fSaveFile," ]\012");
	}
	
	length = fprintf(gtSPDF_Doc->fSaveFile,
					">>\012"
					"startxref\012"
					"%u\012"
					"%%%%EOF\012", gtSPDF_Doc->nFilePosition);
	
	DBG_PRINTF_SDH_DEBUG("FILE_CLOSE: %d, %d, %u\n",gtSPDF_Doc->nPageNo, gtSPDF_Doc->nObjects, gtSPDF_Doc->nFilePosition);
	
	fflush( gtSPDF_Doc->fSaveFile );
	fsync(fileno( gtSPDF_Doc->fSaveFile ));
	SPDF_ClearSPDFResource(gtSPDF_Doc);
	return SPDF_NO_ERROR;
}

SPDF_UINT8 SPDF_SetSizeFromJpeg (SPDF_DOC *gtSPDF_Doc, SPDF_BYTE  *cBuf)
{
	SPDF_UINT16 tag;
	SPDF_UINT32 height;
	SPDF_UINT32 width;
	SPDF_BYTE precision;
	SPDF_BYTE num_components;
	SPDF_UINT16 nLocation = 0;

	DBG_PRINTF_SDH_DEBUG ("%s start\n", __FUNCTION__);

	tag = (cBuf[nLocation]<<8) + cBuf[nLocation+1]; nLocation += 2;
//	DBG_PRINTF_SDH_DEBUG ("tag=%04X\n", tag);
	if (tag != 0xFFD8)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_IMAGE_JPEGFILE_ERROR\n", __FUNCTION__);
		return SPDF_IMAGE_JPEGFILE_ERROR;
	}

	/* find SOF record */
	for (;;)
	{
		SPDF_UINT16 size;
		
		tag = (cBuf[nLocation]<<8) + cBuf[nLocation+1]; nLocation += 2;
		size = (cBuf[nLocation]<<8) + cBuf[nLocation+1]; nLocation += 2;
		
//		DBG_PRINTF_SDH_DEBUG ("tag=%04X size=%u\n", tag, size);
		
		if (tag == 0xFFC0 || tag == 0xFFC1 || tag == 0xFFC2 || tag == 0xFFC9)
		{
			precision = cBuf[nLocation++];
			height = (cBuf[nLocation]<<8) + cBuf[nLocation+1]; nLocation += 2;
			width = (cBuf[nLocation]<<8) + cBuf[nLocation+1]; nLocation += 2;
			num_components = cBuf[nLocation++];
			DBG_PRINTF_SDH_DEBUG ("width=%u height=%u\n", width, height);
			break;
		}
		else if ((tag | 0x00FF) != 0xFFFF)
		{
			DBG_PRINTF_SDH_DEBUG("%s : SPDF_IMAGE_JPEGFILE_ERROR\n", __FUNCTION__);
			return SPDF_IMAGE_JPEGFILE_ERROR;
		}
		
		nLocation += (size-2);
	}
	
	SPDF_Page_SetWidth(gtSPDF_Doc, width);
	SPDF_Page_SetHeight(gtSPDF_Doc, height);
	
	return SPDF_NO_ERROR;
}

SPDF_UINT8 SPDF_LoadImageFromFile(SPDF_DOC *gtSPDF_Doc, const char *fFileName)
{
	SPDF_UINT8	nReturnValue;
	SPDF_UINT32 nImageSize;
	SPDF_UINT32 length;
	SPDF_UINT32 retval;
	FILE *fJpgFile;
	SPDF_BYTE *cBuf;
	
	fJpgFile = fopen (fFileName, "rb" );

	if (fJpgFile==NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_OPEN_JPEGFILE_ERROR\n", __FUNCTION__);
		return SPDF_FILE_OPEN_JPEGFILE_ERROR;
	}
	
	// obtain file size  Target : iSize
	fseek (fJpgFile , 0 , SEEK_END);
	nImageSize = ftell (fJpgFile);
	rewind (fJpgFile);

	// allocate memory to contain the whole file:
	cBuf = (unsigned char*)malloc(sizeof(char)*(nImageSize));
	if (cBuf == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_MEMORY_FULL_ERROR\n", __FUNCTION__);
		fclose (fJpgFile);
		return SPDF_MEMORY_FULL_ERROR;
	}

	// copy the file into the buffer:
	retval = fread (cBuf,1,nImageSize,fJpgFile);
	if (retval != nImageSize)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_READ_JPEGFILE_ERROR\n", __FUNCTION__);
		free(cBuf);
		fclose (fJpgFile);
		return SPDF_FILE_READ_JPEGFILE_ERROR;
	}
	
	fclose(fJpgFile);

	if (SPDF_SetSizeFromJpeg(gtSPDF_Doc, cBuf))
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_READ_JPEGFILE_ERROR\n", __FUNCTION__);
		free(cBuf);
		return SPDF_FILE_READ_JPEGFILE_ERROR;
	}
	
	nReturnValue = SPDF_WriteHeader(gtSPDF_Doc);
	if (nReturnValue)
	{
		free(cBuf);
		return nReturnValue;
	}
	
	retval = SPDF_Write_Stream(gtSPDF_Doc, cBuf, nImageSize, gtSPDF_Doc->nObjects);

	DBG_PRINTF_SDH_DEBUG("[RYU] file write : %d(%d)\n", retval, nImageSize);

	if(retval <= 0)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_WRITE_SAVEFILE_ERROR\n", __FUNCTION__);
		free(cBuf);
		return SPDF_FILE_WRITE_SAVEFILE_ERROR;
	}
	
	gtSPDF_Doc->nFilePosition += nImageSize;
	gtSPDF_Doc->gtSPDF_PageData->nImageSize = nImageSize;
	
	length = fprintf(gtSPDF_Doc->fSaveFile, "\012endstream\012"
					"endobj\012");
	gtSPDF_Doc->nFilePosition += length;
	
	free(cBuf);
	
	nReturnValue = SPDF_WriteTrailer(gtSPDF_Doc);
	
	if (nReturnValue)
		return nReturnValue;
	else
		return SPDF_NO_ERROR;
}

// for 1bit data(Black & White)
SPDF_UINT8 SPDF_LoadImageFromMem(SPDF_DOC *gtSPDF_Doc, SPDF_BYTE *cBuf, SPDF_UINT32 nImageSize)
{
	SPDF_UINT8	nReturnValue;
	SPDF_UINT32 length ;
	SPDF_UINT32 retval, isEncodeOK;
	Fax3CodecState sp;
	
	nReturnValue = SPDF_WriteHeader(gtSPDF_Doc);
	if (nReturnValue)
		return nReturnValue;
	
	if (nImageSize == 0)
	{	
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_IMAGE_BUF_DATA_ERROR\n", __FUNCTION__);
		return SPDF_IMAGE_BUF_DATA_ERROR;
	}
	
	DBG_PRINTF_SDH_DEBUG("%s : pos(%u), pageno(%d), nObjects(%d)\n",__FUNCTION__, gtSPDF_Doc->nFilePosition, gtSPDF_Doc->nPageNo, gtSPDF_Doc->nObjects);
	
	if (gtSPDF_Doc->gtSPDF_PageData->ucBitDepth == 1)
	{
		sp.rowpixels = gtSPDF_Doc->gtSPDF_PageData->nWidth;
		sp.imagesize = nImageSize;
		
		Fax3SetupState(&sp);
		Fax3PreEncode(&sp);

		isEncodeOK = Fax3Encode_UIScan(&sp, cBuf);
		
		DBG_PRINTF_SDH_DEBUG("Info : %d, %d, %d\n",sp.rowpixels, sp.imagesize,isEncodeOK);
		if (isEncodeOK)
		{
			length = fprintf( gtSPDF_Doc->fSaveFile,"/Filter [/CCITTFaxDecode]\012"
							"/DecodeParms[<</Columns %d /Rows %d>>]\012"
							">> stream\015\012", gtSPDF_Doc->gtSPDF_PageData->nWidth, gtSPDF_Doc->gtSPDF_PageData->nHeight);
			gtSPDF_Doc->nFilePosition += length ;
			retval = SPDF_Write_Stream(gtSPDF_Doc, sp.tif_rawbase, sp.tif_rawcc, gtSPDF_Doc->nObjects);
			if(retval <= 0)
			{
				DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_WRITE_SAVEFILE_ERROR\n", __FUNCTION__);
				return SPDF_FILE_WRITE_SAVEFILE_ERROR;
			}
			
			gtSPDF_Doc->nFilePosition += sp.tif_rawcc;
			gtSPDF_Doc->gtSPDF_PageData->nImageSize = sp.tif_rawcc;
		}
		else
		{
			length = fprintf( gtSPDF_Doc->fSaveFile,">> stream\015\012");
			gtSPDF_Doc->nFilePosition += length ;
			retval = SPDF_Write_Stream(gtSPDF_Doc, cBuf, nImageSize, gtSPDF_Doc->nObjects);
			if(retval <= 0)
			{
				DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_WRITE_SAVEFILE_ERROR\n", __FUNCTION__);
				return SPDF_FILE_WRITE_SAVEFILE_ERROR;
			}
			gtSPDF_Doc->nFilePosition += nImageSize;
			gtSPDF_Doc->gtSPDF_PageData->nImageSize = nImageSize;
		}
		Fax3PostEncode(&sp);
	}
	else
	{
		retval = SPDF_Write_Stream(gtSPDF_Doc, cBuf, nImageSize, gtSPDF_Doc->nObjects);
		
		if(retval <= 0)
		{
			DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_WRITE_SAVEFILE_ERROR\n", __FUNCTION__);
			return SPDF_FILE_WRITE_SAVEFILE_ERROR;
		}
		
		gtSPDF_Doc->nFilePosition += nImageSize;
		gtSPDF_Doc->gtSPDF_PageData->nImageSize = nImageSize;
	}
	
	length = fprintf(gtSPDF_Doc->fSaveFile, "\012endstream\012"
					"endobj\012");
	gtSPDF_Doc->nFilePosition += length;
	
	nReturnValue = SPDF_WriteTrailer(gtSPDF_Doc);
	if (nReturnValue)
		return nReturnValue;
	else
		return SPDF_NO_ERROR;
}

static SPDF_UINT8 SPDF_WriteHeader(SPDF_DOC *gtSPDF_Doc)
{
	SPDF_UINT32 length ;
	
	/* Write the PDF header, then the whole compressed raster, then
	* the trailer. Use PDF 1.5 as we are now supporting 16 bit.
	*/
	
	DBG_PRINTF_SDH_DEBUG("%s start\n",__FUNCTION__);
	
	if (gtSPDF_Doc->fSaveFile == NULL)
	{
		DBG_PRINTF_SDH_DEBUG("%s : SPDF_FILE_CHECK_SAVEFILE_ERROR\n", __FUNCTION__);
		SPDF_ClearSPDFResource(gtSPDF_Doc);
		return SPDF_FILE_CHECK_SAVEFILE_ERROR;
	}

	fprintf(gtSPDF_Doc->foffsets,"%u\012", gtSPDF_Doc->nFilePosition);
	gtSPDF_Doc->nObjects++;

	length = fprintf(gtSPDF_Doc->fSaveFile,
					"%d 0 obj\012"
					"<<\012"
					"/Type /XObject\012"
					"/Subtype /Image\012"
					"/Width %d\012"
					"/Height %d\012"
					"/BitsPerComponent %d\012"
					"/Length %d 0 R\012"
					"/ColorSpace ",
					gtSPDF_Doc->nObjects, gtSPDF_Doc->gtSPDF_PageData->nWidth, gtSPDF_Doc->gtSPDF_PageData->nHeight, gtSPDF_Doc->gtSPDF_PageData->ucBitDepth, gtSPDF_Doc->nObjects+1 );
					
	gtSPDF_Doc->nFilePosition += length ;

	/* Write the color space */
	if (gtSPDF_Doc->gtSPDF_PageData->nTotalPlanes == 1 )
		length = fprintf(gtSPDF_Doc->fSaveFile, "/DeviceGray\012");
	else if (gtSPDF_Doc->gtSPDF_PageData->nTotalPlanes == 4 )
		length = fprintf(gtSPDF_Doc->fSaveFile, "/DeviceCMYK\012");
	else if (gtSPDF_Doc->gtSPDF_PageData->nTotalPlanes == 3 )
		length = fprintf(gtSPDF_Doc->fSaveFile, "/DeviceRGB\012");

	gtSPDF_Doc->nFilePosition += length ;
	
	if (gtSPDF_Doc->gtSPDF_PageData->ucBitDepth != 1)
	{
		length = fprintf(gtSPDF_Doc->fSaveFile, "/Filter /DCTDecode\012"
							">> stream\015\012");		
		gtSPDF_Doc->nFilePosition += length ;
	}
	
	return SPDF_NO_ERROR;
}

static SPDF_UINT8 SPDF_WriteTrailer(SPDF_DOC *gtSPDF_Doc)
{
	SPDF_UINT32 length;
	SPDF_UINT32 streamlength;
	SPDF_BYTE	stream_data[SPDF_STREAM_BUF_SIZE];
	
	fprintf(gtSPDF_Doc->foffsets,"%u\012", gtSPDF_Doc->nFilePosition);
	gtSPDF_Doc->nObjects++;
	
	length = fprintf(gtSPDF_Doc->fSaveFile,"%d 0 obj\012" /* Length of image */
				"%d\012"
				"endobj\012", gtSPDF_Doc->nObjects, gtSPDF_Doc->gtSPDF_PageData->nImageSize);
	
	gtSPDF_Doc->nFilePosition += length ;
	fprintf(gtSPDF_Doc->foffsets,"%u\012", gtSPDF_Doc->nFilePosition);
	gtSPDF_Doc->nObjects++;

	length = fprintf(gtSPDF_Doc->fSaveFile,  "%d 0 obj\012" /* Page resources */
					"<<\012"
					"/XObject << /Im%d %d 0 R >>\012"
					"/ProcSet [/PDF /ImageB /ImageC]\012"
					">>\012"
					"endobj\012", gtSPDF_Doc->nObjects, gtSPDF_Doc->nPageNo+1 ,gtSPDF_Doc->nObjects-2);

	gtSPDF_Doc->nFilePosition += length ;
	fprintf(gtSPDF_Doc->foffsets,"%u\012", gtSPDF_Doc->nFilePosition);
	gtSPDF_Doc->nObjects++;

	length = fprintf(gtSPDF_Doc->fSaveFile,  "%d 0 obj\012" /* Page contents stream */
					"<< /Length %d 0 R >>\012"
					"stream\015\012", gtSPDF_Doc->nObjects, gtSPDF_Doc->nObjects+1);

	gtSPDF_Doc->nFilePosition += length ;

	streamlength = sprintf((char*)stream_data, "q\012%d 0 0 %d 0 0 cm\012/Im%d Do\012Q",
					(int)((gtSPDF_Doc->gtSPDF_PageData->nWidth * 72.0) / gtSPDF_Doc->gtSPDF_PageData->nXResolution),
					(int)((gtSPDF_Doc->gtSPDF_PageData->nHeight * 72.0) / gtSPDF_Doc->gtSPDF_PageData->nYResolution), gtSPDF_Doc->nPageNo+1);

	length = SPDF_Write_Stream(gtSPDF_Doc, stream_data, streamlength, gtSPDF_Doc->nObjects);
	gtSPDF_Doc->nFilePosition += length ;
	
	length = fprintf(gtSPDF_Doc->fSaveFile, "\012endstream\012"
					"endobj\012");

	gtSPDF_Doc->nFilePosition += length ;
	fprintf(gtSPDF_Doc->foffsets,"%u\012", gtSPDF_Doc->nFilePosition);
	gtSPDF_Doc->nObjects++;

	length = fprintf(gtSPDF_Doc->fSaveFile, "%d 0 obj\012" /* Length of page contents stream */
					"%d\012"
					"endobj\012", gtSPDF_Doc->nObjects, streamlength);

	gtSPDF_Doc->nFilePosition += length ;
	fprintf(gtSPDF_Doc->foffsets,"%u\012", gtSPDF_Doc->nFilePosition);
	gtSPDF_Doc->nObjects++;

	fprintf(gtSPDF_Doc->fkids, "%d\012", gtSPDF_Doc->nObjects);
	gtSPDF_Doc->nPageNo++;

	length = fprintf(gtSPDF_Doc->fSaveFile, "%d 0 obj\012" /* Page object */
					"<<\012"
					"/Type /Page\012"
					"/Resources %d 0 R\012"
					"/Contents %d 0 R\012"
					"/Parent 1 0 R\012"
					"/MediaBox [0 0 %d %d]\012"
					"/Rotate %d\012"
					">>\012"
					"endobj\012",gtSPDF_Doc->nObjects, gtSPDF_Doc->nObjects-3, gtSPDF_Doc->nObjects-2,
					(int)((gtSPDF_Doc->gtSPDF_PageData->nWidth * 72.0) / gtSPDF_Doc->gtSPDF_PageData->nXResolution),
					(int)((gtSPDF_Doc->gtSPDF_PageData->nHeight * 72.0) / gtSPDF_Doc->gtSPDF_PageData->nYResolution), gtSPDF_Doc->gtSPDF_PageData->nRotate);
					
	gtSPDF_Doc->nFilePosition += length ;
	
	gtSPDF_Doc->gtSPDF_PageData->nImageSize = 0;
	DBG_PRINTF_SDH_DEBUG("PDF_WFT END %d, %d, %u\n",gtSPDF_Doc->nPageNo, gtSPDF_Doc->nObjects, gtSPDF_Doc->nFilePosition);
	return SPDF_NO_ERROR;
}

static SPDF_UINT32 SPDF_Write_Stream(SPDF_DOC *gtSPDF_Doc, SPDF_BYTE *buf, SPDF_UINT32 length, SPDF_UINT32 nObject)
{
	SPDF_UINT32 retval;
	SPDF_UINT32 write_length = 0;
#if 0
	SPDF_BYTE	ebuf[SPDF_STREAM_BUF_SIZE];
	SPDF_BOOL	flg=FALSE;
	SPDF_UINT	size=0;
#else
#endif
	
	if (gtSPDF_Doc->bEncryptOn == TRUE)
	{
		SPDF_Encrypt_InitKey(gtSPDF_Doc->stEncryptDict, nObject, 0);
		SPDF_Encrypt_Reset(gtSPDF_Doc->stEncryptDict);
#if 0
		for (;;) {
			if ((write_length+SPDF_STREAM_BUF_SIZE) < length)
			{
				size = SPDF_STREAM_BUF_SIZE;
			}
			else
			{
				size = length - write_length;
				flg = TRUE;
			}
				
			SPDF_Encrypt_CryptBuf(gtSPDF_Doc->stEncryptDict, buf+write_length, ebuf, size);
			retval = fwrite(ebuf, 1, size, gtSPDF_Doc->fSaveFile);
			
			if (retval != size)
				return SPDF_COMMON_ERROR;
		
			write_length += size;
		
			if (flg)
				break;
		}
#else
		SPDF_Encrypt_CryptBuf(gtSPDF_Doc->stEncryptDict, buf, buf, length);
		retval = fwrite(buf, 1, length, gtSPDF_Doc->fSaveFile);
		
		if (retval != length)
			return SPDF_COMMON_ERROR;
	
		write_length += length;
#endif
	}
	else
	{
		write_length = fwrite(buf, 1, length, gtSPDF_Doc->fSaveFile);
	}
	
	return write_length;
}

static SPDF_UINT32 SPDF_Write_Binary(SPDF_DOC *gtSPDF_Doc, SPDF_BYTE *buf, SPDF_UINT32 length)
{
	SPDF_CHAR wbuf[SPDF_TEXT_DEFAULT_LEN];
	SPDF_UINT nLoop;
	SPDF_UINT idx = 0;
	SPDF_UINT32 write_length;
	const SPDF_BYTE* p;
	
	p = buf;
	
	for (nLoop = 0; nLoop < length; nLoop++, p++) {
		char c = (char)(*p >> 4);

		if (c <= 9)
			c += 0x30;
		else
			c += 0x41 - 10;
		wbuf[idx++] = c;

		c = (char)(*p & 0x0f);
		if (c <= 9)
			c += 0x30;
		else
			c += 0x41 - 10;
		wbuf[idx++] = c;
	}
    
    wbuf[idx++] = 0;

	write_length = fprintf(gtSPDF_Doc->fSaveFile, "<%s>", wbuf);

    return write_length;
}
