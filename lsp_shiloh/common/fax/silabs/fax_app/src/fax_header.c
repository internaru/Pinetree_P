/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dprintf.h>

#include "memAPI.h"
#include "strfmt.h"
#include "freetype_api.h"
#include "fax_header.h"
#include "string_mgr_api.h"

#include "ATypes.h"
#include "date_time_api.h"
#include "platform_api.h"

typedef struct
{
  int stride; /* bytes */
  int rows;
  char date_string[30];
  char page_string[30];
  uint8_t *buf;
} _header_t;
static _header_t _header;

/*----------------------------------------------*/
static void _draw_bitmap(unsigned char* cdata, int cw, int ch, int pitch, fax_header_type_t type, unsigned char* idata,
    int iw, int ih, int ixpos /* in pixels */, int iypos)
{
  int p, q, j, i;
  int x_max;
  int y_max;

  x_max = ixpos + cw;
  y_max = iypos + ch;

  if (type == FAX_HEADER_TYPE_MONO)
  {
    int dest_bit;
    int src_bit;
    /* width of char is in bits */
    for (dest_bit = ixpos, src_bit = 0; src_bit < cw; dest_bit++, src_bit++)
    {
      for (j = iypos, q = 0; j < y_max; j++, q++)
      {
        if ((dest_bit + 7) / 8 >= iw || j >= ih)
          continue;
        idata[j * iw + (dest_bit / 8)] |= (((cdata[(q * pitch) + (src_bit / 8)] << (src_bit % 8)) & 0x80) >> (dest_bit
            % 8));
      }
    }
  }
  else if (type == FAX_HEADER_TYPE_GRAYSCALE)
  {
    for (i = ixpos, p = 0; i < x_max; i++, p++)
    {
      for (j = iypos, q = 0; j < y_max; j++, q++)
      {
        if (i >= iw || j >= ih)
          continue;
        idata[j * iw + i] &= 0xff - cdata[q * pitch + p];
      }
    }
  }
  else if (type == FAX_HEADER_TYPE_RGBX)
  {
    for (i = ixpos, p = 0; i < x_max; i++, p++)
    {
      for (j = iypos, q = 0; j < y_max; j++, q++)
      {
        uint8_t val = 0xff - cdata[q * pitch + p];

        if (i >= iw / 4 || j >= ih)
          continue;
        idata[j * iw + (i * 4)] &= val;
        idata[j * iw + (i * 4) + 1] &= val;
        idata[j * iw + (i * 4) + 2] &= val;
        idata[j * iw + (i * 4) + 3] &= val;
      }
    }
  }
  else if (type == FAX_HEADER_TYPE_RGB)
  {
    for (i = ixpos, p = 0; i < x_max; i++, p++)
    {
      for (j = iypos, q = 0; j < y_max; j++, q++)
      {
        uint8_t val = 0xff - cdata[q * pitch + p];

        if (i >= iw / 3 || j >= ih)
          continue;
        idata[j * iw + (i * 3)] &= val;
        idata[j * iw + (i * 3) + 1] &= val;
        idata[j * iw + (i * 3) + 2] &= val;
      }
    }
  }

}

/*----------------------------------------------*/
static void _write_string(char *string, void *font, fax_header_type_t type, int x_loc)
{
  freetype_glyph_data_t glyph;
  bool mono;
  int i;
  int y = (_header.rows * 2 + 1) / 3;
  mono = (type == FAX_HEADER_TYPE_MONO);
  for (i = 0; i < strlen(string); i++)
  {
    if (freetype_render_glyph(font, string[i], mono, &glyph))
      continue;

    _draw_bitmap(glyph.data, glyph.width, glyph.rows, glyph.pitch, type, _header.buf, _header.stride, _header.rows,
        x_loc + glyph.left, y - glyph.top);

    x_loc += glyph.advance_x;
  }
}

/*----------------------------------------------*/
void *fax_header_build(int stride, int x_res, int y_res, fax_header_type_t type, int page, 
                       char *name, char *number, struct tm *the_time, 
                       fax_header_date_format_t format)
{
  int x_loc = 0;
  uint8_t value = 0;
  void *font;
  char *fontfile = NULL;
  struct tm rtc_time;
  e_TIME_FORMAT time_format;
  e_DATE_FORMAT date_format; 

  memset(&_header, 0, sizeof(_header));

  //plat_get_date_time_format(&time_format, &date_format);
  time_format = e_12HOUR;
  date_format = e_MONTH_FIRST;
  
 
  if(the_time)
  {
    memcpy(&rtc_time, the_time, sizeof(struct tm));
  }
  else
  {  
     time_t ltime;
     gmtime_r(&ltime, &rtc_time);
  }
  
  if(format == FAX_HEADER_FORMAT_DATE_AND_TIME)
  {
      char date_string[20];

      memset(date_string, 0, sizeof(date_string));

      formatDate( _header.date_string, &rtc_time, date_format, false ); 
      strncpy( _header.date_string, date_string, sizeof(_header.date_string) );
      formatTime( _header.date_string + strlen(date_string), &rtc_time, time_format, false, true ); 
  }
  else
  {
      formatDate( _header.date_string, &rtc_time, date_format, false ); 
  }

//  minNSprintf(_header.page_string, sizeof(_header.page_string), 
//              string_mgr_get_string(STRING_RPT_PAGE, string_mgr_loc_full), page);

  _header.rows = y_res / 5;
  _header.stride = stride;
  _header.buf = MEM_MALLOC(stride * _header.rows);

  if (type == FAX_HEADER_TYPE_MONO)
    value = 0;
  else if (type == FAX_HEADER_TYPE_GRAYSCALE)
    value = 0xff;
  else if (type == FAX_HEADER_TYPE_RGBX)
    value = 0xff;
  else if (type == FAX_HEADER_TYPE_RGB)
    value = 0xff;
  memset(_header.buf, value, (stride * _header.rows));

  /* line to draw the text on */

  font = freetype_open_font(fontfile, 10 /* point size */, x_res, y_res);

  x_loc = x_res / 5;
  _write_string(_header.date_string, font, type, x_loc);

  x_loc = x_res * 3;
  if(name)
  {
    _write_string(name, font, type, x_loc);
  }

  x_loc = x_res * 5;
  if(number)
  {
    _write_string(number, font, type, x_loc);
  }

  x_loc = (x_res * 75) / 10;
  _write_string(_header.page_string, font, type, x_loc);

  freetype_close_font(font);

  return (&_header);
}

/*----------------------------------------------*/
void fax_header_get_data(void *handle, uint8_t ** data, int *rows)
{
  _header_t *header = (_header_t*)handle;

  *data = header->buf;
  *rows = header->rows;
}

/*----------------------------------------------*/
void fax_header_destroy(void *handle)
{
  _header_t *header = (_header_t*)handle;

  MEM_FREE_AND_NULL(header->buf);
  memset(&_header, 0, sizeof(_header));
}

