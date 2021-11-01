/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** Similar to scan except it handles RAW data provided externaly.
*/
#include <stdint.h>
#include "dprintf.h"
#include "fax_app_raw.h"
#include "fax_app_prv.h"
#include "fax_tiff_io.h"
#include "fax_app_jpeg.h"
#include "jpeglib.h"
#include "gray2bw.h"
#include "memAPI.h"

static void *tiff_writer_handle;
static fax_store_handle_t store;
static fax_app_resolution_t resolution;
static bool jpeg;

/*----------------------------------------------------------*/
static uint32_t _write_tiff( uint8_t *buf, int rows, int row_size )
{ 
  int res;
  
  res = fax_app_faxer_free_storage(rows*row_size); 
  if(res)
  {
    dbg_printf("FAXAPP: out of storage memory, sending cancel.\n");
  }
  else if((res = fax_tiff_writer_write(tiff_writer_handle, buf, rows, row_size)))
  {
    dbg_printf("FAXAPP: call to faxer_tiff_writer_write() failed, sending cancel.\n");
  }
  return(res);
}

/*----------------------------------------------------------*/
uint32_t fax_app_raw_jpeg_start( fax_store_handle_t store_param, fax_app_resolution_t resolution_param )
{
  if(store == NULL && tiff_writer_handle == NULL)
  {
    store = store_param;
    resolution = resolution_param;
    jpeg = 1;
    return( 0 );
  }
  return(1);
}

/*----------------------------------------------------------*/
uint32_t fax_app_raw_mono_start( char * filename, fax_app_resolution_t resolution_param )
{
  if(store == NULL && tiff_writer_handle == NULL)
  {
    tiff_writer_handle = fax_tiff_writer_init(filename, /*resolution*/FAX_TYPE_PHOTO, FAX_PGSIZE_LETTER);
    resolution = resolution_param;
    jpeg = 0;
    return(0);
  } 
  return(1);
}

/*----------------------------------------------------------*/
uint32_t fax_app_raw_cancel( )
{
  if(tiff_writer_handle)
  {
    fax_tiff_writer_close(tiff_writer_handle);
    tiff_writer_handle = NULL;
  }

  /* do not destroy the store, it was passed to us */
  if(store)
  {
    store = NULL;
  }

  return(0);
}

/*----------------------------------------------------------*/
uint32_t fax_app_raw_add_page( uint8_t *buf, fax_app_input_data_format_t src_format, 
                               uint32_t pix_width, uint32_t pix_height,
                               bool complete )
{
  uint32_t res = 0;
  uint32_t bytes_per_row;

  if( buf )
  {
    /* determine bytes per pix */
    if(jpeg)
    {
      switch(src_format)
      {
        case FAX_INPUT_FORMAT_GRAY:
          bytes_per_row = pix_width * 1;
          break;
        case FAX_INPUT_FORMAT_RGB:
          bytes_per_row = pix_width * 3;
          break;
        case FAX_INPUT_FORMAT_XBGR:
          bytes_per_row = pix_width * 4;
          break;
        default:
          ASSERT(0);
          return(1);
      }
    }
    else
    {
      bytes_per_row = pix_width/8;
      if(pix_width%8)
      {
        /* pad */
        bytes_per_row++;
      }
    }

    if(jpeg)
    {
      uint32_t dpi;
      struct jpeg_compress_struct jpeg_comp_handle;

      ASSERT(store);

      if(resolution == FAX_TYPE_PHOTO)
      {
        dpi = 300;
      } 
      else
      {  
        dpi = 200;
      }

      dbg_printf("Starting jpeg compress\n");
      fax_app_jpeg_start_compress(&jpeg_comp_handle, src_format,
                                pix_width, pix_height,
                                dpi, 
                                store);

      dbg_printf("Starting jpeg write\n");
      fax_app_jpeg_write(&jpeg_comp_handle, buf, bytes_per_row);

      dbg_printf("Finishing compress\n");
      if(fax_app_jpeg_finish_compress(&jpeg_comp_handle) == 0)
      {
        dbg_printf("Failed finishing compress\n");
        res = FAIL;
      }
    }
    else
    {
      uint32_t mem_req;
      uint8_t *mem;
      void *handle;
    
      mem_req = gray2bw_memreq(pix_width);
      mem = MEM_MALLOC(mem_req);
      handle = gray2bw_create(pix_width, (uint32_t*)mem, mem_req);
    
      fax_tiff_writer_start_new_page(tiff_writer_handle);
  
      /* convert to 1 bit */
      gray2bw_convert(handle, buf, pix_height); 

      /* write data to tiff */
      res = _write_tiff(buf, pix_height, bytes_per_row);

      fax_tiff_writer_end_page(tiff_writer_handle);

      MEM_FREE_AND_NULL(mem);
    }
  }

  if(complete)
  {
    fax_app_raw_cancel();
  }
  return(res);
}
