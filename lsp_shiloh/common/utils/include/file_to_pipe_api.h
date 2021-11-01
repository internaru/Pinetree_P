/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __INC_FILE_TO_PIPE_API_H__
#define __INC_FILE_TO_PIPE_API_H__

#include "error_types.h"
#include "SysUI_Interface.h"

#ifdef __cplusplus
extern "C" {
#endif


/** \brief The file to pipe module's maximum path length
 * do not pass path names larger than this to file_to_pip() */
#define FILE_TO_PIPE_MAX_PATH 512


/**
 *
 * \brief A callback function used when calling file_to_pipe.  This
 * callback function will be called when the parser is done reading data
 * from your file/pipe.
 *
 * \param path is the path used when calling file_to_pipe
 *
 * \param context is a user-supplied opaque data pointer
 *
 */
typedef void (*file_to_pipe_done_callback_t)( char *path, void *context );


/**
 * \brief Initialize the file to pipe API
 */
void file_to_pipe_init( void );


/**
 * \brief Creates a pipe from the file and connects it with the connection manager
 * This will result in a parser reading the data from the file and typically
 * results in a print
 *
 * \param path A valid null-terminated string representing the
 * filesystem's path to the file that will be converted to a pipe
 * NOTE: that path length cannot be longer than FILE_TO_PIPE_MAX_PATH
 * including the NULL terminating character
 *
 * \param done_callback This function will be called when the parser
 * is done reading data from the pipe.  Users of this API can then
 * clean up anything necessary to be cleaned up
 *
 * \param context is a user-supplied opaque data pointer
 *
 * NOTE: the file must be in a format that a parser will understand
 */
error_type_t file_to_pipe( char *path,
                           file_to_pipe_done_callback_t done_callback,
                           void *context,
                            int doc_type);

void set_file_print_setting( int* _tray_no, int* _darkness, int* _collate, int* _collate_bind, int* _duplex, int* _quantity, int* _doc_type, int* _image_height, int* _image_width );
void reset_file_print_setting();

#ifdef __cplusplus
}
#endif

#endif /* __INC_FILE_TO_PIPE_API_H__ */
