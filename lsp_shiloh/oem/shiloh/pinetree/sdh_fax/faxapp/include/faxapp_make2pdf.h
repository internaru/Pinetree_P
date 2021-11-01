
#ifndef __FAXAPP_MAKE_PDF_FILE__
#define __FAXAPP_MAKE_PDF_FILE__

#include <stdio.h>

#include "dc_type.h"


#define TRUE 1
#define FALSE 0
#define MAX_STREAM_SIZE (10*1024*1024)


#define PDF_INIT		1
#define PDF_HEADER		2
#define PDF_TRAILER		4
#define PDF_CLOSE		8


typedef struct tagPDFout_globals
{
    UINT32	pageno;			/**< page no...		*/
    UINT32	nobjects;		/**< Number of objects in PDF file */
    UINT32	filepos;		/**< Current file position */
    UINT32	offsets[2];		/**< Location of PDF objects */
    FILE *SaveFile;
    FILE *fkids, *foffsets;
    unsigned char	*stream;
} TPDFOUT_GLOBALS;


typedef struct
{
    unsigned char	ucBitDepth;
    unsigned char	uTotalPlanes;
    unsigned char	ucRotate;
    unsigned int	dXResolution;
    unsigned int	dYResolution;
    unsigned int	nHeight;
    unsigned int	nWidth;
    unsigned int	nImageSize;
} PDF_CLIENT;
#endif // #ifndef __FAXAPP_MAKE_PDF_FILE__

