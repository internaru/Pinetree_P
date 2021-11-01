// img2pdfDlg.h
//
#ifndef _img2pdf_h_
#define _img2pdf_h_

#include <stdio.h>
#include "spdf_types.h"
#include "spdf_encrypt.h"
#include "spdf_encryptdict.h"

typedef enum
{
	SPDF_SOURCE_BW_1BPP = 0,
	SPDF_SOURCE_GRAY_8BPP,
	SPDF_SOURCE_COLOR_24BPP,
}SPDF_SOURCE_TYPE;

typedef struct {
	SPDF_BYTE		ucBitDepth;
	SPDF_BYTE		nTotalPlanes;
	SPDF_UINT16		nRotate;
	SPDF_UINT32		nXResolution;
	SPDF_UINT32		nYResolution;
	SPDF_UINT32		nHeight;
	SPDF_UINT32		nWidth;
	SPDF_UINT32		nImageSize;
} SPDF_PAGE_DATA;

typedef struct tagSPDF_DOC
{
	SPDF_UINT32		nPageNo;			/**< page no...		*/
	SPDF_UINT32		nObjects;			/**< Number of objects in PDF file */
	SPDF_UINT32		nFilePosition;		/**< Current file position */
	char			*KidsFileName;
	char			*OffsetFileName;
	FILE			*fSaveFile;
	FILE			*fkids, *foffsets;
	SPDF_PAGE_DATA	*gtSPDF_PageData;
	SPDF_BOOL		bEncryptOn;
	SPDF_Encrypt	stEncryptDict;
}SPDF_DOC, *PSPDF_DOC;

PSPDF_DOC	SPDF_Open(const char *FileName, const char *KidsFileName, const char *OffsetFileName);
SPDF_UINT8	SPDF_Close(SPDF_DOC *gtSPDF_Doc);
SPDF_UINT8	SPDF_SetColorSpace(SPDF_DOC *gtSPDF_Doc, SPDF_SOURCE_TYPE SourceType);
SPDF_UINT8	SPDF_SetResolution(SPDF_DOC *gtSPDF_Doc, SPDF_UINT32 nResolution);
SPDF_UINT8	SPDF_Page_SetWidth(SPDF_DOC *gtSPDF_Doc, SPDF_UINT32 nWidth);
SPDF_UINT8	SPDF_Page_SetHeight(SPDF_DOC *gtSPDF_Doc, SPDF_UINT32 nHeight);
SPDF_UINT8	SPDF_Page_SetRotate(SPDF_DOC *gtSPDF_Doc, SPDF_UINT16 nRotate);
SPDF_UINT8	SPDF_LoadImageFromFile(SPDF_DOC *gtSPDF_Doc, const char *fFileName);						// for jpeg
SPDF_UINT8	SPDF_LoadImageFromMem(SPDF_DOC *gtSPDF_Doc, SPDF_BYTE *cBuf, SPDF_UINT32 nImageSize);		// for 1bit data(Black & White)
SPDF_UINT32	SPDF_GetFileSize(SPDF_DOC *gtSPDF_Doc);
SPDF_UINT32	SPDF_GetPages(SPDF_DOC *gtSPDF_Doc);
SPDF_VOID	SPDF_ClearSPDFResource(SPDF_DOC *gtSPDF_Doc);

SPDF_UINT8	SPDF_SetPassword(SPDF_DOC *gtSPDF_Doc, const char *owner_passwd, const char *user_passwd);
SPDF_UINT8	SPDF_SetPermission(SPDF_DOC *gtSPDF_Doc, SPDF_INT32 permission);
#endif /*  _img2pdf_h_ */
