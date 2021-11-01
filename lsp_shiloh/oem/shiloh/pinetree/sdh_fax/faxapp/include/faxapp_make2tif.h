#ifndef __FAXAPP_MAKE_TIF_FILE__
#define __FAXAPP_MAKE_TIF_FILE__


#include <stdio.h>

#include "dc_type.h"


#define TRUE		1
#define FALSE		0
#define NO_OF_TAGS	18
#define MAX_STREAM_SIZE (10*1024*1024)


#define TIFF_INIT       1
#define TIFF_IFH		2
#define TIFF_IFD		4
#define TIFF_CLOSE		8


typedef struct tagTIFout_globals
{
    unsigned int	pageno;
    unsigned int	filepos;			/**< Current file position */
    FILE 			*SaveFile;
    unsigned char	*stream;
} TTIFOUT_GLOBALS;


typedef struct {
    unsigned char	ucRotate;
    unsigned int	dXResolution;
    unsigned int	dYResolution;
    unsigned int	nHeight;
    unsigned int	nWidth;
    unsigned int	nImageSize;
} TIF_CLIENT;


typedef struct _TiffHeader
{
    unsigned short	Identifier;  	/* Byte-order Identifier */
    unsigned short	Version;     	/* TIFF version number (always 2Ah) */
    unsigned int 	IFDOffset;   	/* Offset of the first Image File Directory*/
} TIFHEAD;


typedef struct _TifTag
{
    unsigned short	TagId;       	/* The tag identifier  */
    unsigned short	DataType;    	/* The scalar type of the data items  */
    unsigned int	DataCount;   	/* The number of items in the tag data  */
    unsigned int	DataOffset;  	/* The byte offset to the data items  */
} TIFTAG;


#pragma pack(push,1)
typedef struct _TifIfd
{
    unsigned short	NumDirEntries;			/* Number of Tags in IFD  */
    TIFTAG  		TagList[NO_OF_TAGS];	/* Array of Tags  */
    unsigned int	NextIFDOffset;			/* Offset to next IFD  */
    unsigned char	Manufacture[7];
    unsigned int	XDpi[2];
    unsigned int	YDpi[2];
} TIFIFD;
#pragma pack(pop)

#endif // #define __FAXAPP_MAKE_TIF_FILE__
