
#ifndef _SPDF_TYPES_H
#define _SPDF_TYPES_H

#define TRUE 1
#define FALSE 0
#define MAX_STREAM_SIZE (12*1024*1024)

typedef void				SPDF_VOID;

typedef signed char			SPDF_CHAR;
typedef signed char			SPDF_INT8;
typedef signed short		SPDF_INT16;
typedef signed int			SPDF_INT32;

typedef unsigned char		SPDF_BYTE;
typedef unsigned char		SPDF_UINT8;
typedef unsigned short		SPDF_UINT16;
typedef unsigned int		SPDF_UINT32;
typedef unsigned int		SPDF_UINT;

typedef	signed int			SPDF_BOOL;
typedef signed int			SPDF_INT;

typedef enum
{
	SPDF_NO_ERROR = 0x00,
	SPDF_FILE_OPEN_SAVEFILE_ERROR,
	SPDF_FILE_OPEN_FKID_ERROR,
	SPDF_FILE_OPEN_FOFFSET_ERROR,
	SPDF_FILE_OPEN_JPEGFILE_ERROR,
	SPDF_FILE_READ_JPEGFILE_ERROR,
	SPDF_FILE_WRITE_SAVEFILE_ERROR,
	SPDF_FILE_CHECK_SAVEFILE_ERROR,
	SPDF_FILE_CHECK_FKID_ERROR,
	SPDF_FILE_CHECK_FOFFSET_ERROR,
	SPDF_IMAGE_BUF_DATA_ERROR,
	SPDF_IMAGE_JPEGFILE_ERROR,
	SPDF_MEMORY_FULL_ERROR,
	SPDF_DOC_DATA_CHECK_ERROR,
	SPDF_PAGE_DATA_CHECK_ERROR,
	SPDF_DOC_ENCRYPTION_ERROR,
	SPDF_COMMON_ERROR = 0xFF,
}SPDF_ERROR;

#define SPDF_PTRACE(ARGS)

/*----- permission flags (only Revision 2 is supported)-----------------------*/

#define SPDF_ENABLE_READ         0
#define SPDF_ENABLE_PRINT        4
#define SPDF_ENABLE_EDIT_ALL     8
#define SPDF_ENABLE_COPY         16
#define SPDF_ENABLE_EDIT         32

#define SPDF_STREAM_BUF_SIZE	4096
#define SPDF_TEXT_DEFAULT_LEN	128
#endif /* _SPDF_TYPES_H */

