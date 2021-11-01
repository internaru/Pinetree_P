/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved.
 *
 *                      Marvell Confidential
 * ============================================================================
 */

#include "freetype_api.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "lassert.h"

#ifdef __linux__
#define _DEFAULT_FONT "/usr/share/fonts"
#else
#include "datafs_api.h"
#define _DEFAULT_FONT "/"DATAFS_ROOT"/"FREETYPE_DEFAULT_FONTFILE
#endif

static FT_Library _ft_lib;

void freetype_init(void)
{
  FT_Init_FreeType(&_ft_lib); /* initialize library */
  /* TODO: Add error handling */

  extern void freetype_cmd_init(void);
  freetype_cmd_init();
}

void* freetype_open_font(const char* fontfile,
                         unsigned ptsize,
                         unsigned x_resolution,
                         unsigned y_resolution)
{
  FT_Face face;
  FT_Error error;
  if (!fontfile)
  {
    ASSERT(0);  // Need to build freetype fonts into the linux system
    fontfile = _DEFAULT_FONT;
  }
  if ((error = FT_New_Face(_ft_lib, fontfile, 0, &face))) return NULL;
  if ((error = FT_Set_Char_Size(face, ptsize * 64, 0, x_resolution, y_resolution)))
  {
    FT_Done_Face(face);
    return NULL;
  }
  return face;
}

void freetype_close_font(void* font_hdl)
{
  if (!font_hdl) return;
  FT_Done_Face((FT_Face)font_hdl);
}

int freetype_render_glyph(void* font_hdl, unsigned long charcode,
                          int mono, freetype_glyph_data_t* data)
{
  if (!font_hdl) return -1;
  if (!data)     return -1;
  FT_Int32     load_flags;
  FT_Error     error;
  FT_Face      face = (FT_Face)font_hdl;
  FT_GlyphSlot slot = face->glyph;
  if (!data) return -1;
  if (mono) load_flags = FT_LOAD_RENDER | FT_LOAD_MONOCHROME;
  else      load_flags = FT_LOAD_RENDER;
  if ((error = FT_Load_Char((FT_Face)font_hdl, charcode, load_flags)))
    return -2;
  data->rows      = slot->bitmap.rows;
  data->width     = slot->bitmap.width;
  data->pitch     = slot->bitmap.pitch;
  data->data      = slot->bitmap.buffer;
  data->left      = slot->bitmap_left;
  data->top       = slot->bitmap_top;
  data->advance_x = slot->advance.x >> 6;
  data->advance_y = slot->advance.y >> 6;
  return 0;
}
