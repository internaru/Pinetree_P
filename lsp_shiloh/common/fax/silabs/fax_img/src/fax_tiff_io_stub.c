/******************************************************************************
 * Copyright (c) 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <string.h>
#include <stdio.h>

#include "agRouter.h"
#include "ATypes.h"
#include "fax_tiff_io.h"

/**
 *\brief Check the validity of a TIFF file
 *
 *\param[in] fname   Full path TIFF file name
 *
 *\return Non-zero The line count in the 1st page of the file. Only line count greater than 30 is returned\n
 *           Zero: Invalid TIFF file
 */
uint32_t fax_tiff_check_file_validity(char *fname)
{
  return 0;
}


/**
 * \brief Initialize a TIFF file reader to read content of a TIFF file
 *
 *\param[in] fname            Full path TIFF file name
 *\param[in] out_resolution   The resolution for output bitmap buffer
 *\param[in] page_size        Output paper size
 *\param[in] scale_factor     Scale factor
 *\param[in] page_break       True to break at page boundary during read
 *
 * \return void * A handle to the reader
 *
 * Only 300 and 600 output resolution are supported.
 *
 * Scale factor use 100 as a base factor. A value of 100 indicates 100% scaling, while 50 indicating 50%% scaling.
 * A scale factor of zero also turns of the scaling.
 *
 */
void *fax_tiff_reader_init(char *fname, uint16_t out_resolution, FAX_PAGE_SIZE page_size, uint16_t scale_factor, BOOL page_break)
{
  return NULL;
}


/*
 **\brief Read and decode TIuFF content to bitmap data
 *
 *\param[in]     handle          TIFF reader handle
 *\param[in]     data            Buffer to hold bmp image data
 *\param[out]    line_width      Width of buffer line in bytes
 *\param[in,out] lines_cnt       Number of lines to read, and actual count of lines read out
 *\param[out]    page_status     Buffer holds current page status\n
 *                                0 More to come\n
 *                               1 End of a page, more page to come
 *                               2 End of document
 *
 * \return Completion code   0 Success, others error code
 *
 * Read a buffer of image data. The number of lines to be read is
 * given by line_cnt. Use a line_cnt with very large value will
 * cause a whole page data get read out.

 */
uint32_t fax_tiff_reader_read(void *handle, uint8_t *data, uint16_t line_width, uint16_t *line_cnt, uint8_t *page_status)
{
  return 0;
}

/*
 **\brief TIFF reader cleaning up.
 *
 * \param[in] handle TIFF reader handler
 *
 * \return Completion code  0 Success, others error code
 *
 * User must call this to release at end of usage to release all resource used by the reader
 *
 */
uint32_t fax_tiff_reader_close(void *handle)
{
  return 0;
}

/*
 **\brief Initialize a new TIFF writer
 *
 *\param[in] fname       Full path file name of the TIFF file to write to
 *\param[in] resolution  Resolution to be used
 *\param[in] page_size   Size of page for write

 * \return   A handle to the writer, NULL if failure
 *
 * User uses this to initialize a TIFF file writer.
 *
 */
void *fax_tiff_writer_init(char *fname, FAX_TYPES resolution, FAX_PAGE_SIZE page_size)
{
  return NULL;}

/*
 **\brief Obtain current TIFF file dimension information used with TIFF writer
 *
 *\param[in] handle TIFF writer handler
 *\param[out] ResX  Horizontal resolution (width) in 1/00 inch
 *\param[out] ResY Vertical resolution (height) in 1/00 inch
 *
 * \return Completion code  0 Success, others error code
 */
uint32_t fax_tiff_writer_get_dimensions(void *handle, int32_t *ResX, int32_t *ResY)
{
  return 0;
}


/*
 **\brief Start a new page for write
 *
 *\param[in] handle TIFF writer handler
 *
 *\return completion code  0 Success, others error code
 */
uint32_t fax_tiff_writer_start_new_page(void *handle)
{
  return 0;
}


/*
 **\brief Write more data to current page
 *
 *\param[in] handle      Writer handle
 *\param[in] buf         Data buffer
 *\param[in] line_cnt    Number of lines in the buffer
 *\param[in] line_size   Width of line
 *
 *\return Completion code  0 Success, others error code
 */
uint32_t fax_tiff_writer_write(void *handle, uint8_t *buf, uint16_t line_cnt, uint16_t line_size)
{
  return 0;
}

/*
 **\brief Mark the end of a page
 *
 *\param[in] handle   Writer handle
 *
 *\return Completion code  0 Success, others error code
 */
uint32_t fax_tiff_writer_end_page(void *handle)
{
  return 0;
}


/*
 **\brief Close a writer document
 *
 *\param[in] handle Writer handle
 *
 *\return Completion code  0 Success, others error code
 */
uint32_t fax_tiff_writer_close(void *handle)
{
  return 0;
}


/**
 * \brief Create a sample TIFF file
 *
 * \param[in] fname Full path name for the file to be enerated
 * \param[in] pages Number of pages to be included in the file
 *
 * \return  Completion code 0 Success, others error code
 *
 * This is a only for debug purpose
 */
uint32_t fax_generate_sample_fax(char *fname, uint16_t pages)
{
  return 0;
}

