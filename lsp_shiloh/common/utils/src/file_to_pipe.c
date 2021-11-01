/******************************************************************************
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \brief Allows a connection manager pipe to be created with a file as a
 * backend This registers as another io. The parser reads data from the pipe
 * and handles it accordingly.
 *
 */

#include "error_types.h"
#include "file_to_pipe_api.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "memAPI.h"
#include "io_device.h"
#include "agConnectMgr.h"
#include "cmd_proc_api.h"
#define SYS_SHARED_MEMORY
#include "SysSharedMemory.h"
#undef SYS_SHARED_MEMORY
#ifdef HAVE_PRINT_SUPPORT
#include "PrintJobInfo.h"
#endif

static int32_t file_to_pipe_read( io_device_impl_t * fh,
                                  void * buffer,
                                  uint32_t length,
                                  uint32_t timeout );

static int32_t file_to_pipe_write( io_device_impl_t * fh,
                                   void * buffer,
                                   uint32_t length,
                                   uint32_t timeout );

static int32_t file_to_pipe_io_cmd( io_device_impl_t * fh,
                                    ioctl_cmd_t cmd,
                                    void * details );
static int file_to_pipe_cmd( int argc,
                             char* argv[] );

static void keep_file_print_setting( stShmGoPrint *pPrintInfo, int doc_type );

typedef struct io_device_impl_s
{
    FILE* filehdl; /*!< A file handle */
    void *cm_pipe; /*!< A connection manager pipe */
    char path[FILE_TO_PIPE_MAX_PATH];
    void *context; /*!< user's file_to_pipe() context */
    file_to_pipe_done_callback_t done_callback;
} file_to_pipe_file_and_pipe_t;

///< 3 pipes for files at max.
///< 
static file_to_pipe_file_and_pipe_t one_file_pipe[3] = { {0,}, {0,}, {0,} }; 

/* File-print Function Utils */
static int fp_file_print_flag = 0;
static int fp_tray_no = 0;
static int fp_darkness = 0;
static int fp_collate = 0;
static int fp_duplex_bind = 0;
static int fp_duplex = 0;
static int fp_quantity = 0;
static int fp_doc_type = 0;
static int fp_paper_size_height = 0;
static int fp_paper_size_width = 0;

void file_to_pipe_init( void )
{
    cmd_register_cmd( "file_to_pipe",
              "file_to_pipe",
              "Connects a file as a pipe to the connection manager <dirpath>",
              "<dirpath>",
              "Connects a file as a pipe to the connection manager <dirpath>",
              file_to_pipe_cmd );
}

static file_to_pipe_file_and_pipe_t * get_pipe()
{
	int i;
	for (i=0; i < 3; i++)
	{
		if (one_file_pipe[i].filehdl == 0)
			return &one_file_pipe[i];
	}
	return 0;
}
/** 
 * \brief connect a file to a pipe via the connection manager
 * The file will be attached as a pipe to the connection
 * manager and then handled with the appropriate parser
 * 
 * \retval OK upon successful start of the connection, FAIL otherwise
 * \revision: 20131029 LALIT added doc_type. Get document type and set to print job info.
 */
error_type_t file_to_pipe( char *path,
                           file_to_pipe_done_callback_t done_callback,
                           void *context,
                            int doc_type)
{
    PIPE_INFO file_to_pipe_pipe_info;
    file_to_pipe_file_and_pipe_t *file_and_pipe;
    error_type_t e_res;
    void *pipe = 0;
	stShmGoPrint *pPrintInfo = context;

    ASSERT(NULL != path);

    e_res = OK;

    file_and_pipe = get_pipe();
    if (NULL == file_and_pipe)
    {
        e_res = FAIL;
    }
    else
    {
        pipe = file_and_pipe->cm_pipe;
    }

    if (OK == e_res)
    {
		if(doc_type == e_USBDRIVEPDFPAGE)
			keep_file_print_setting(pPrintInfo, e_USBDRIVEPDFPAGE);
		else if(doc_type == e_USBDRIVEJPGPAGE)
			keep_file_print_setting(pPrintInfo, e_USBDRIVEJPGPAGE);


        /* save user-supplied context */
        file_and_pipe->context = context;
        /* save user-supplied path */
        strncpy(file_and_pipe->path, path, FILE_TO_PIPE_MAX_PATH);
        /* save user-supplied callback */
        ASSERT(NULL != done_callback);
        file_and_pipe->done_callback = done_callback;

        /* attemp to open file */
        file_and_pipe->filehdl = fopen((const char *)path, "r");
        if (!file_and_pipe->filehdl)
        {
            e_res = FAIL;
        }
    }

    if (OK == e_res)
    {
        memset(&file_to_pipe_pipe_info, 0, sizeof(PIPE_INFO));
        file_to_pipe_pipe_info.io.data_read = file_to_pipe_read;
        file_to_pipe_pipe_info.io.data_write = file_to_pipe_write;
        file_to_pipe_pipe_info.io.ioctl = file_to_pipe_io_cmd;
        file_to_pipe_pipe_info.channel_type = e_Print;
        file_to_pipe_pipe_info.DocType = (DOCTYPE)doc_type;	//LALIT: Set Document Type 
        file_to_pipe_pipe_info.io.data_read_handle = file_and_pipe;

        /* register our pipe */
        if (!pipe)
        {
            pipe = PipeRegister(&file_to_pipe_pipe_info);
        }
        else
        {
            if(((IOReg*)pipe)->pjob_info)
            {
                ((IOReg*)pipe)->pjob_info->jobDocType =  file_to_pipe_pipe_info.DocType;
            }
        }

        file_and_pipe->cm_pipe = pipe;
        if (NULL == file_and_pipe->cm_pipe)
        {
            fclose(file_and_pipe->filehdl);
            file_and_pipe->filehdl = 0;
            e_res = FAIL;
        }
    }

    if (OK == e_res)
    {
        /* got all resources (memory, pipe, etc.) -- connect our pipe and
         * begin parsing/printing */
        PipeConnect(file_and_pipe->cm_pipe);
    }

	if ( FAIL == e_res )
		reset_file_print_setting();

    return e_res;
}


/** 
 * \brief Supply a read function for the connection manager.
 * The file looks like an io to the connection manager.  The read
 * function is supplied by this routine.  We send the file data back
 * to the connection manager using this routine.
 * 
 */
static int32_t file_to_pipe_read( io_device_impl_t *fh,
                                  void *buffer,
                                  uint32_t length,
                                  uint32_t timeout )
{
    file_to_pipe_file_and_pipe_t *file_and_pipe;
    int err;

    ASSERT( NULL != fh );
    ASSERT( NULL != buffer );
    ASSERT( 0 != length );


    /* this fh is really a file_to_pipe_file_and_pipe_t, make it so number one */
    file_and_pipe = (file_to_pipe_file_and_pipe_t *)fh;

    /* WARNING, no way to timeout or not block... so we are
     * ignoring timeout value,
     * convert 0 return to eof 
     */
    err = fread(buffer, 1, length, file_and_pipe->filehdl);
    return err ? err : -1; 
} 



/** 
 * \brief Simulate the write, chuck the data
 * because this is a read only pipe
 * 
 */
static int32_t file_to_pipe_write( io_device_impl_t * fh,
                                   void * buffer,
                                   uint32_t length,
                                   uint32_t timeout )
{
    ASSERT( NULL != buffer );
    // chuck the data.
    MEM_FREE_AND_NULL( buffer );
    return length;
}



/** 
 * \brief handle io cmds from connection manager
 * This routine is called by the connection manager after the file has
 * been parsed).
 * 
 */
static int32_t file_to_pipe_io_cmd( io_device_impl_t *fh,
                                    ioctl_cmd_t cmd,
                                    void *details )
{
    /* we are done, release the data and return. */
    if ( cmd == e_close )
    {
        file_to_pipe_file_and_pipe_t *file_and_pipe;

        ASSERT( NULL != fh );


        /* this fh is really a file_to_pipe_file_and_pipe_t, make it so number one */
        file_and_pipe = (file_to_pipe_file_and_pipe_t *)fh;
	if ( file_and_pipe->filehdl )
	{
		//if ( !file_and_pipe->cm_pipe->parent ) 
		//	return 0;
		fclose( file_and_pipe->filehdl );
		file_and_pipe->filehdl = 0;
		ASSERT( NULL != file_and_pipe->done_callback );
		file_and_pipe->done_callback( file_and_pipe->path, file_and_pipe->context );
	}
    }
    //  e_close_write is ignored since we don't write.
    return 0;
}

static void _file_to_pipe_cmd_done( char *path, void *context )
{
    /* nothing to do */
    return;
}
static int file_to_pipe_cmd( int argc, char* argv[] )
{
    int i_res;

    i_res = CMD_OK;

    error_type_t e_res;
    if (argc != 2)
    {
        i_res = CMD_USAGE_ERROR;
    }

    if ( CMD_OK == i_res )
    {
        e_res = file_to_pipe( argv[1], _file_to_pipe_cmd_done, NULL, e_DOCUMENT);
        if ( OK != e_res )
        {
            i_res = CMD_ERROR;
        }
    }

    return i_res;

}

#define FILE_PRINT_SIMPLEX 0
#define FILE_PRINT_DUPLEX_SE 1
#define FILE_PRINT_DUPLEX_LE 2

void keep_file_print_setting( stShmGoPrint *pPrintInfo, int doc_type )
{
	fp_file_print_flag = 1;
	fp_tray_no = pPrintInfo->inTray;
	fp_darkness = pPrintInfo->darkness;
	fp_collate = pPrintInfo->sort;
	if(pPrintInfo->duplex == FILE_PRINT_SIMPLEX)
	{
		fp_duplex = 0;
		fp_duplex_bind = 0;
	}
	else if(pPrintInfo->duplex == FILE_PRINT_DUPLEX_SE)
	{
		fp_duplex = 1;
		fp_duplex_bind = 1;
	}
	else if(pPrintInfo->duplex == FILE_PRINT_DUPLEX_LE)
	{
		fp_duplex = 1;
		fp_duplex_bind = 0;
	}
	fp_quantity = pPrintInfo->repeat;
	fp_doc_type = doc_type;
	fp_paper_size_height = pPrintInfo->image_height;
	fp_paper_size_width = pPrintInfo->image_width;
	printf("----------> FP:pPrint Info %d %d %d %d %d %d %d\n", pPrintInfo->inTray, pPrintInfo->sort, pPrintInfo->duplex, pPrintInfo->repeat, doc_type, pPrintInfo->image_height, pPrintInfo->image_width);
	printf("----------> FP:GET %d %d %d %d %d %d %d %d %d %d\n", fp_file_print_flag, fp_tray_no, fp_darkness, fp_collate, fp_duplex_bind, fp_duplex, fp_quantity, fp_doc_type, fp_paper_size_height, fp_paper_size_width);
	return;
}

void set_file_print_setting( int* _tray_no, int* _darkness, int* _collate, int* _duplex_bind, int* _duplex, int* _quantity, int* _doc_type, int* _image_height, int* _image_width )
{
	if(fp_file_print_flag)
	{
		(*_tray_no) = fp_tray_no;
		(*_darkness) = fp_darkness;
		(*_collate) = fp_collate;
		(*_duplex_bind) = fp_duplex_bind;
		(*_duplex) = fp_duplex;
		(*_quantity) = fp_quantity;
		(*_doc_type) = fp_doc_type;
		(*_image_height) = fp_paper_size_height;
		(*_image_width) = fp_paper_size_width;
		printf("----------> FP:SET %d %d %d %d %d %d %d %d %d %d\n", fp_file_print_flag, fp_tray_no, fp_darkness, fp_collate, fp_duplex_bind, fp_duplex, fp_quantity, fp_doc_type, fp_paper_size_height, fp_paper_size_width);
	}
	else
	{
		printf("----------> FP:It isn't File Print Job.\n");
	}
	return;
}

void reset_file_print_setting()
{
	fp_file_print_flag = 0;
	fp_tray_no = 0;
	fp_darkness = 0;
	fp_collate = 0;
	fp_duplex_bind = 0;
	fp_duplex = 0;
	fp_quantity = 0;
	fp_doc_type = 0;
	fp_paper_size_height = -1;
	fp_paper_size_width = -1;

	printf("----------> FP:RESET %d %d %d %d %d %d %d %d %d %d\n", fp_file_print_flag, fp_tray_no, fp_darkness, fp_collate, fp_duplex_bind, fp_duplex, fp_quantity, fp_doc_type, fp_paper_size_height, fp_paper_size_width);
	return;
}
