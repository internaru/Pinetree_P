/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/* This file contains all setup/interaction with the jpeg library.  It also contains a destination 
   implementation. */
#include <dprintf.h>
#include "debug.h"

#include "fax_app_jpeg.h"
#include "lassert.h"
#include "memAPI.h"
#include "rgb2labi.h"

#include "jpeglib.h"
#include "jerror.h"

#include "xyscale.h"
static xyscale_hdl_t _xy;

#include "fax_app_prv.h"


#define COLOR_OUTPUT_COMPONENTS (3)

#define RAW_DECODE_BUF_SIZE (10000)
#define ENCODE_BUF_SIZE     (1000000) /* most files are 400 k or less, we should
                                         also probably just have a small buffer and dump out to the 
                                         store more often...but for now */


typedef struct
{
  struct jpeg_error_mgr jerr;
  struct jpeg_destination_mgr cdest;
  uint8_t *dest_buf;
  int dest_buf_size;
  int input_width;
  fax_store_handle_t store;
  fax_app_input_data_format_t src_format;
} _encode_t;


typedef struct
{
  struct jpeg_source_mgr pub;   /* public fields */
  fax_store_pg_handle_t page;
  uint16_t resolution;
  JOCTET *raw_buffer;
  uint32_t stride;
  bool complete;
} _src_mgr_t;

typedef struct
{
  struct jpeg_error_mgr jerr;
  _src_mgr_t dsrc;
  JOCTET *row_buffer;
  uint8_t *scale_buffer;
  uint8_t *scale_in_row;
  uint32_t scale_width;
  uint32_t scale_height;
  uint32_t scale_read;
} _decode_t;

/* one encode */
static _encode_t _encode_data;
/* one decode */
static _decode_t _decode_data;


/*-------------------------------------------------*/
/* Compress 
*--------------------------------------------------*/

/**************************************************
* Destination object 
***************************************************/
/**
 * called by the jpeg to initialize a buffer to used in compression
 */
static void init_dest(j_compress_ptr cinfo)
{
  /* pre funded and setup...nothing to do here */
}
/**
 * This is called by the sw jpeg compressor when the compression is complete.
 * So it should send the data, and shut down everything else.
 */
static void term_destination(j_compress_ptr cinfo)
{
}
/**
 * This is called by the sw jpeg compressor when it has filled one of 
 * our buffers.  Send the buffer to the host and call the init to get the next one.
 */
static boolean empty_output_buffer(j_compress_ptr cinfo)
{
    /* should never get here for now due to size of buffer and single page
       For now, if you see this the jpeg dest buffer should increase in size 
       TODO: add overflow handling */
    ASSERT(0);
    return(0);
}


/*************************************************
* Lib functions 
*************************************************/

/*------------------------------------------------*/
void fax_app_jpeg_start_compress(struct jpeg_compress_struct *cinfo, 
                                 fax_app_input_data_format_t src_format,
                                 int pix_width, int pix_height, int res,
                                 fax_store_handle_t store )
{
   /* See T.4 spec for more details about this header.  It is required for faxes. */
   JOCTET g3fax_data[] = {/* G3FAX */   0x47,0x33,0x46,0x41,0x58,0x00, 
                          /* version */ 0x07,0xCA, 
                          /* res = 200 */ 0x00,0xC8};
   ASSERT(cinfo);
   cinfo->err = jpeg_std_error(&_encode_data.jerr); 

   ASSERT(store);
   _encode_data.store = store;
  
   jpeg_create_compress(cinfo); 
   //jpeg_xt_dest(cinfo, (fat_file_handle_t*)_scan_info.fhandle);

   _encode_data.src_format = src_format;
   if(src_format == FAX_INPUT_FORMAT_GRAY)
   {
     cinfo->input_components = 1;
   }
   else 
   {
     cinfo->input_components = COLOR_OUTPUT_COMPONENTS;
   }

   if(res == 200) 
   {
     dbg_printf("Start JPEG with resolution of 200\n");
     cinfo->image_width  = FAX_ENCODED_WIDTH_200;  /* Pixel width in 200 dpi res, spec width for A4/Letter, not 
                                                 input width.  Some machines do not print the data */ 
     /* default g3fax header */
   }
   else if(res == 300)
   {
     dbg_printf("Start JPEG with resolution of 300\n");
     cinfo->image_width  = FAX_ENCODED_WIDTH_300;  /* Pixel width in 300 dpi res, spec width for A4/Letter, not 
                                                 input width.  Some machines do not print the data */ 
     g3fax_data[8] = 0x01;
     g3fax_data[9] = 0x2c;
   }
   else
   {
       ASSERT(0); /* unsupported resolution */
   }
   _encode_data.input_width = pix_width;
   cinfo->image_height = pix_height; /* pixel height */
   if(src_format == FAX_INPUT_FORMAT_GRAY)
   {
     cinfo->in_color_space = JCS_GRAYSCALE; 
   } 
   else
   {
     cinfo->in_color_space = JCS_YCbCr; /* actualy it is LAB, but this is what works becuase it disables
                                           any color conversion. */
   }

   dbg_printf("Input Color Space = %d, Input Components = %d\n", cinfo->in_color_space, cinfo->input_components);

   /* may be optional */
   /* set to our own custom destination, keeping this simple for now */
   cinfo->dest = &_encode_data.cdest; 
   cinfo->dest->next_output_byte = _encode_data.dest_buf = MEM_MALLOC_ALIGN(ENCODE_BUF_SIZE,  cpu_get_dcache_line_size());
   ASSERT(cinfo->dest->next_output_byte);
   cinfo->dest->free_in_buffer = _encode_data.dest_buf_size = ENCODE_BUF_SIZE;

   cinfo->dest->init_destination = init_dest;
   cinfo->dest->empty_output_buffer = empty_output_buffer;
   cinfo->dest->term_destination = term_destination;

   jpeg_set_defaults(cinfo);

   // quality is a number from 0 to 100 that specifies how much 
   // to compress.  
   // 0 = highest compress, lowest quality, 
   // 100=lowest compress, highest quality.
   jpeg_set_quality( cinfo, 50, TRUE );  /* not sure what quality we should use, most machines appear
                                            to use ~50 */
  
   /* do not include JFIF header */
   cinfo->write_JFIF_header = 0;

   /* restart markers on */
   cinfo->restart_in_rows = 5;

   jpeg_start_compress(cinfo, TRUE);  
   /* write special marker for G3FAX */
   jpeg_write_marker(cinfo, 0xE1, g3fax_data, sizeof(g3fax_data)); 
   dbg_printf("FAXAPP JPEG: compress started, width = %d height = %d\n",cinfo->image_width, cinfo->image_height );
}

/*------------------------------------------------*/
/* assumes input is RGBX */
void fax_app_jpeg_write(struct jpeg_compress_struct *cinfo, unsigned char *data, int rows)
{
   int i;
   unsigned char * input_line_buff = data;
   /* color */
   if(cinfo->input_components > 1)
   {
     unsigned char * lab_line_buff = MEM_MALLOC(cinfo->image_width * COLOR_OUTPUT_COMPONENTS /* L+A+B */);
     static const int xbgr_map[3] = {3, 2, 1};
     static const int rgb_map[3] = {1, 2, 3};
     const int *map;
     int bytes_per_pix;

     ASSERT(lab_line_buff); 
     memset(lab_line_buff, 0, sizeof(lab_line_buff));

     if(_encode_data.src_format == FAX_INPUT_FORMAT_XBGR)
     {
       map = xbgr_map;
       bytes_per_pix = 4;
     }
     else
     {
       map = rgb_map;
       bytes_per_pix = 3;
     }

     /* write one row at a time...jpeg requires setting up an array of ptrs, so this is easiest */
     for(i=0; i<rows;i++, input_line_buff+=_encode_data.input_width*bytes_per_pix )
     {
       JSAMPROW row_ptr[1];
       
       uint32_t width = MIN(cinfo->image_width, _encode_data.input_width);

       rgb2labi(input_line_buff, lab_line_buff, width, bytes_per_pix, map);
       /* check if we need to fill */
       if(cinfo->image_width > width)
       {
         unsigned char *fill_buff;
         int fill_width = cinfo->image_width-width;
         /* fill with white */
         fill_buff = MEM_MALLOC(fill_width*bytes_per_pix ); 
         ASSERT(fill_buff);
         memset(fill_buff, 0xff, fill_width*bytes_per_pix ); 
         rgb2labi(fill_buff, lab_line_buff+(width*COLOR_OUTPUT_COMPONENTS), fill_width, bytes_per_pix, map);
         MEM_FREE_AND_NULL(fill_buff);
       }
       row_ptr[0] = lab_line_buff; 
       jpeg_write_scanlines(cinfo, row_ptr, 1);
     }
     MEM_FREE_AND_NULL(lab_line_buff); 
   }
   /* grayscale */
   else
   {
     unsigned char *line_buff = MEM_MALLOC(cinfo->image_width);
     uint32_t width = MIN(cinfo->image_width, _encode_data.input_width);
     ASSERT(line_buff);
     memset(line_buff, 0, sizeof(line_buff));
 
     /* write one row at a time...jpeg requires setting up an array of ptrs, so this is easiest */
     for(i=0; i<rows;i++, input_line_buff+=_encode_data.input_width)
     {
       JSAMPROW row_ptr[1];
       memcpy(line_buff, input_line_buff, width);
       row_ptr[0] = line_buff; 
       jpeg_write_scanlines(cinfo, row_ptr, 1);
     }
     MEM_FREE_AND_NULL(line_buff); 
   }
}

/*------------------------------------------------*/
static void _modify_jpeg_height(uint8_t *buf, int size, uint16_t height)
{
  int i;
  /* search and find S0F0 tag, replace height in the tag */
  for(i=0; i<size; i++)
  {
    if(i>0 && buf[i-1] == 0xff && buf[i] == 0xc0)
    {
      int height_offset = i+4; 
      ASSERT(height_offset+1 <= size);
      buf[height_offset] = (height>>8)&0xff; 
      buf[height_offset+1] = height&0xff; 
      break;
    } 
  }
  ASSERT(i<size);
} 

/*------------------------------------------------*/
/* returns size */
int fax_app_jpeg_finish_compress(struct jpeg_compress_struct *cinfo)
{
  int size = 0;
  if(cinfo)
  {
    fax_store_pg_handle_t page;
    bool changed_height = 0; 
    int res;
 
    /* the cleint may not have written enough data (eg. ADF scan short doc), so we will modify the height */
    if(cinfo->next_scanline < cinfo->image_height)
    {
      dbg_printf("FAXAPP: too little data sent to jpeg, changing jpeg height\n");
      cinfo->image_height = cinfo->next_scanline;
      changed_height = 1;
    } 
    jpeg_finish_compress(cinfo);   
    size = _encode_data.dest_buf_size - cinfo->dest->free_in_buffer;
    ASSERT(_encode_data.store);
    if(changed_height)
    {
      _modify_jpeg_height(_encode_data.dest_buf, size, cinfo->image_height);
    }
    /* check that we have storage space for this JPEG */
  
    res = fax_app_faxer_free_storage(size + 1000); 
    if(res)
    {
      size = 0;
      dbg_printf("FAXAPP: out of storage memory, did not store jpeg.\n");
    }
    else 
    {
      page = fax_store_page_open_new(_encode_data.store, FAX_STORE_MODE_WRITE); 
      fax_store_page_write_data(page, _encode_data.dest_buf, size, true);
      fax_store_page_close(page);
    }
  }
  return(size);
}

/*------------------------------------------------*/
int fax_app_jpeg_destroy_compress(struct jpeg_compress_struct *cinfo)
{
  int size = 0;
  if(cinfo)
  {
    jpeg_destroy_compress(cinfo);
  }
  if(_encode_data.dest_buf) MEM_FREE_AND_NULL(_encode_data.dest_buf);
  memset(&_encode_data, 0, sizeof(_encode_data));
   
  return(size);
}

/*-------------------------------------------------*/
/* Decompress 
*--------------------------------------------------*/
static void init_source (j_decompress_ptr cinfo)
{
  /* nothing to do */
}

static boolean fill_input_buffer (j_decompress_ptr cinfo)
{
  _src_mgr_t  *src = (_src_mgr_t*) cinfo->src;
  uint32_t nbytes = 0;

  ASSERT(src->page);
 
  if(!src->complete) 
  {
    nbytes = fax_store_page_read_data( src->page, src->raw_buffer, RAW_DECODE_BUF_SIZE, &src->complete);
  }
  if (nbytes == 0) 
  {
    WARNMS(cinfo, JWRN_JPEG_EOF);
    /* Insert a fake EOI marker */
    src->raw_buffer[0] = (JOCTET) 0xFF;
    src->raw_buffer[1] = (JOCTET) JPEG_EOI;
    nbytes = 2;
  }

  src->pub.next_input_byte = src->raw_buffer;
  src->pub.bytes_in_buffer = nbytes;

  return TRUE;
}

static void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  /* function copied from another source implementation for now, will
     look at possible improvements later if needed */
  struct jpeg_source_mgr * src = cinfo->src;

  if (num_bytes > 0) 
  {
    while (num_bytes > (long) src->bytes_in_buffer) 
    {
      num_bytes -= (long) src->bytes_in_buffer;
      (*src->fill_input_buffer) (cinfo);
    }
    src->next_input_byte += (size_t) num_bytes;
    src->bytes_in_buffer -= (size_t) num_bytes;
  }
}

static void term_source (j_decompress_ptr cinfo)
{
  /* nothing to do */
}


/* We are giong to overide the standard error handler to ensure that the failures cause 
   our code to return to us rather than calling exit.  Unfortionatly, it appears the only
   way to do this is with setjump/longjump!! ahh! Both epeg and the example file do this 
   same thing so I am going to stick with it for no.   
*/
typedef struct  
{
  struct jpeg_error_mgr pub;	
  //jmp_buf setjmp_buffer;
} _decode_error;

/*------------------------------------------------*/
void _error_exit (j_common_ptr cinfo)
{
  /* display the message */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  //longjmp(err->setjmp_buffer, 1);
}

/*------------------------------------------------*/
static uint16_t _get_res_from_g3_header(fax_store_pg_handle_t page)
{
  uint8_t buf[100];
  bool comp = 0;
  uint16_t res = 0;
  bool ff_found = 0;
  bool soi_found = 0;
  int offset = 0;
  int nbytes;

  while(!comp)
  {
    int i;
    nbytes = fax_store_page_read_data(page, buf, sizeof(buf), &comp);

    for(i=0; i<nbytes; i++)
    {
      offset++;
      dbg_printf("0x%2x\n", buf[i]);
      /* find SOI marker */
      if(!ff_found && buf[i] == 0xff) 
      {
        ff_found = 1;
      }
      else if(!soi_found && buf[i] == 0xd8)
      {
        soi_found = 1; 
        break;
      }
      else  
      {
        ff_found = 0;
      }
    }
    if(soi_found) break;
  }

  /* G3 fax header should directly follow the SOI marker */ 
  if(soi_found)
  {
    fax_store_page_set_read_pos(page, offset);
    nbytes = fax_store_page_read_data(page, buf, 14, &comp);
    dbg_printf("JPEG bytes = %d, buf[0]= 0x%2x buf[1] = 0x%2x string = |%s|\n", nbytes, buf[0], buf[1], &buf[4]);
    /* APP1 marker */
    if(nbytes == 14 && buf[0] == 0xff && buf[1] == 0xe1 && !strncmp((char*)&buf[4], "G3FAX", 5)) 
    {
      res = (buf[12]<<8) | (buf[13]&0xff);  
    }
  }
  else
  {
    dbg_printf("FAXAPP: JPEG ERROR invalid image, no SOI tag found.\n");
    /* invalid image */
    res = 0; 
  }
  dbg_printf("FAXAPP: JPEG resolution was read in as %d\n", res);
  if(res != 200 && res != 300) res = 200;
  dbg_printf("FAXAPP: JPEG resolution was set to %d\n", res);
  /* rest for jpeg decompress */
  fax_store_page_set_read_pos(page, 0);
  return(res);
}

/*------------------------------------------------*/
/* returns row stride                             */
int fax_app_jpeg_start_decompress(struct jpeg_decompress_struct *cinfo, fax_store_pg_handle_t page, uint32_t *stride, 
                                  mediasize_t media_size )
{
  _src_mgr_t *src;
  uint16_t resolution;
  uint32_t max_height; 
  uint32_t max_width; 
  uint32_t scale_buf_size;

  ASSERT(cinfo);
  ASSERT(page);

  /* first, determine the resolution */
  resolution = _get_res_from_g3_header(page);
  if(!resolution)
  {
    dbg_printf("FAXAPP: ERROR, JPEG header invalid.\n");
    return(-1);
  }

  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo->err = jpeg_std_error(&_decode_data.jerr);
  cinfo->err->error_exit = _error_exit;
  /* We will exstablish the setjmp return in the read function that may cause the error */

  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(cinfo);

  /* setup the src to our own custom src   */
  cinfo->src = &_decode_data.dsrc.pub;
  cinfo->src->init_source = init_source; 
  cinfo->src->fill_input_buffer = fill_input_buffer;
  cinfo->src->skip_input_data = skip_input_data;
  cinfo->src->resync_to_restart = jpeg_resync_to_restart;
  cinfo->src->term_source = term_source;
  cinfo->src->next_input_byte = NULL;
  cinfo->src->bytes_in_buffer = 0;

  src = (_src_mgr_t*)cinfo->src;
  src->resolution = resolution;
  /* buffer for reading the raw data */
  src->raw_buffer = (*cinfo->mem->alloc_small)
		((j_common_ptr) cinfo, JPOOL_IMAGE, RAW_DECODE_BUF_SIZE);

  src->page = page;
  src->complete = 0;

  if(jpeg_read_header(cinfo, FALSE) != JPEG_HEADER_OK)
  {
    dbg_printf("FAXAPP: ERROR, JPEG header invalid.\n");
    return(-1);
  }

  /* we don't want colorspace conversion */
  cinfo->out_color_space = cinfo->jpeg_color_space; 
  /* this will fill in the output width, etc */
  jpeg_start_decompress(cinfo);

  /* calculate scale.  We will use our external scaler since the jpeg lib only supports
     limited 1/8 scale ratios */
  switch(media_size)
  {
    case MEDIASIZE_A4:
      max_height = (resolution*117)/10;
      max_width  = (resolution*83)/10;
      break;
    case MEDIASIZE_LEGAL:
      max_height = resolution*14;
      max_width  = (resolution*85)/10;
      break;
    case MEDIASIZE_LETTER:
    default:
      max_height = resolution*11;
      max_width  = (resolution*85)/10;
      break;
  }
  max_height -= (resolution*4)/10; /* .4 inch margin */
  max_width -= (resolution*1)/20;  /* .05 inch margin to ensure no loss of data */

  scale_buf_size = 0; /* Hack - use this var as signal that we need or do not need scaling */
  if(cinfo->image_height > max_height)
  {
     uint32_t scale = (max_height*1000)/cinfo->image_height;
     _decode_data.scale_height = max_height;
     _decode_data.scale_width  = (cinfo->output_width*scale)/1000;
      
     dbg_printf("SCALE DUE TO HEIGHT: scale = %d scale height = %d scale width = %d\n", scale, _decode_data.scale_height, _decode_data.scale_width);
     scale_buf_size = 1; /* We need scaling */
  }
  else if(cinfo->image_width > max_width)
  { 
     uint32_t scale = (max_width*1000)/cinfo->image_width;
     _decode_data.scale_height = (cinfo->output_height*scale)/1000;
     _decode_data.scale_width  = max_width;
     dbg_printf("SCALE DUE TO WIDTH: scale = %d scale height = %d scale width = %d\n", scale,  
                _decode_data.scale_height, _decode_data.scale_width);
     scale_buf_size = 1; /* We need scaling */
  }

  if (scale_buf_size)
  {
    dbg_printf(">>>>>>>SCALE\n");

    /* XYScale tells us, for real, how much memory it needs */
    scale_buf_size = xyscale_mem_size(cinfo->output_components,
                      /*  input x,y */ cinfo->output_width, cinfo->output_height,
                      /* output x,y */ _decode_data.scale_width, _decode_data.scale_height,
                                       XYSCALE_SPEED_NORMAL);
    _decode_data.scale_buffer = MEM_MALLOC_ALIGN(scale_buf_size, 4);

    /* init scaler if needed */
    if(xyscale_init(&_xy, cinfo->output_components,
                    /*  input x,y */ cinfo->output_width, cinfo->output_height,
                    /* output x,y */ _decode_data.scale_width, _decode_data.scale_height,
                    _decode_data.scale_buffer, scale_buf_size, XYSCALE_SPEED_NORMAL))
    {
      fax_app_jpeg_end_decompress(cinfo); 
      return(-2);
    }
    *stride = _decode_data.scale_width * cinfo->output_components;
    /* input row for sending to scaler */
    _decode_data.scale_in_row = MEM_MALLOC(cinfo->output_width * cinfo->output_components);
  }
  else
  {
    *stride = cinfo->output_width * cinfo->output_components;
  }

  dbg_printf("\n\nOutput width = %d Output components = %d\n\n\n", cinfo->output_width, cinfo->output_components);
  /* bytes per row in output buffer */
  /* Make a one-row-high sample array that will go away when done with image */
  _decode_data.row_buffer = MEM_MALLOC(*stride);
  src->page = page;

  return(0);
};

void fax_app_jpeg_get_output_dimensions(struct jpeg_decompress_struct *cinfo, uint32_t *pix_width, uint32_t *pix_height)
{
  if(_decode_data.scale_buffer)
  {
    *pix_width = _decode_data.scale_width;
    *pix_height = _decode_data.scale_height;
  }
  else
  {
    *pix_width = cinfo->output_width;
    *pix_height = cinfo->output_height;
  }
}

void fax_app_jpeg_get_output_components(struct jpeg_decompress_struct *cinfo, int *comps)
{
  *comps = cinfo->output_components;
}

void fax_app_jpeg_get_output_resolution(struct jpeg_decompress_struct *cinfo, uint16_t *res)
{
  *res = ((_src_mgr_t*)cinfo->src)->resolution;
}

void fax_app_jpeg_end_decompress(struct jpeg_decompress_struct *cinfo)
{
  MEM_FREE_AND_NULL(_decode_data.row_buffer);
  if(_decode_data.scale_buffer) MEM_FREE_AND_NULL(_decode_data.scale_buffer);  
  if(_decode_data.scale_in_row) MEM_FREE_AND_NULL(_decode_data.scale_in_row);  
  jpeg_destroy_decompress(cinfo);
  memset(&_decode_data, 0, sizeof(_decode_data));
}


/*------------------------------------------------*/
/* returns pointer to row data */
int fax_app_jpeg_read_data_scanline(struct jpeg_decompress_struct *cinfo, uint8_t **buff, bool *completed)
{
#if 0
  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(_decode_data.jerr.setjmp_buffer)) 
  {
    /* If we get here, the JPEG code has signaled an error.
     * We need to clean up the JPEG object, close the input file, and return.
     */
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }
#endif
  /* if we have to scale */
  if(_decode_data.scale_buffer)
  {
     if(_decode_data.scale_read < _decode_data.scale_height)
     {
       while(xyscale_outdata(_xy, _decode_data.row_buffer))
       {
         JSAMPROW row_ptr[1];
         row_ptr[0] = _decode_data.scale_in_row;   
         jpeg_read_scanlines(cinfo, row_ptr, 1);
         /* now pass into scaler */
         xyscale_indata(_xy, _decode_data.scale_in_row); 
       }
       _decode_data.scale_read++;  
       *buff = _decode_data.row_buffer;

       if(_decode_data.scale_read >= _decode_data.scale_height)
         *completed = 1;
       else
         *completed = 0;
       return(0);
     }
  }
  else if(cinfo->output_scanline < cinfo->output_height)
  {
      JSAMPROW row_ptr[1];
      row_ptr[0] = _decode_data.row_buffer;   
      jpeg_read_scanlines(cinfo, row_ptr, 1);
      *buff = row_ptr[0];
      if(cinfo->output_scanline >= cinfo->output_height)
        *completed = 1;
      else
        *completed = 0;
      return(0);
  } 

  return(-1);
}

