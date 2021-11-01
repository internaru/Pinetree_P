/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved.
 *
 *                      Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include "cmd_proc_api.h"
#include "memAPI.h"
#include "strfmt.h"
#include "freetype_api.h"

static void _draw_bitmap(unsigned char* cdata,
                         int cw, int ch, int pitch, int mono,
                         unsigned char* idata,
                         int iw, int ih, int ixpos, int iypos)
{
  int  i, j, p, q;
  int  x_max = ixpos + cw / (mono?8:1);
  int  y_max = iypos + ch;

  for (i=ixpos, p=0; i<x_max; i++, p++ )
  {
    for (j=iypos, q=0; j<y_max; j++, q++)
    {
      if (i >= iw || j>= ih)
        continue;

      idata[j*iw+i] |= cdata[q*pitch+p];
    }
  }
}

static int _freetype(int argc, char* argv[], int mono)
{
  char* fontfile;
  char* text;
  char* outfile;

  int   n, num_chars;

  int   w=320, h=200;

  unsigned char *image;
  FILE* f;
  void* font;
  int x, y;
  freetype_glyph_data_t glyph;

  if (argc < 4) return CMD_USAGE_ERROR;

  fontfile      = argv[1];                           /* first argument     */
  text          = argv[2];                           /* second argument    */
  num_chars     = strlen( text );
  outfile       = argv[3];
  if (argc > 4) w = cmd_atoi(argv[4]);
  if (argc > 5) h = cmd_atoi(argv[5]);

  if (mono) w /= 8;

  image = (unsigned char*)MEM_MALLOC(w*h);
  if (!image)
  {
    cmd_printf("Memory allocation error\n");
    return CMD_ERROR;
  }
  memset(image, 0, w*h);

  if (!strcmp(fontfile, "-")) fontfile = NULL; /* Use built in font */
  font = freetype_open_font(fontfile, 50, 100, 100);

  /* start pt (relative to the upper left corner)  */
  x = w / num_chars;
  y = (h*2 + 1)/3;

  for ( n = 0; n < num_chars; n++ )
  {
    /* load glyph image into the slot (erase previous one) */
    if (freetype_render_glyph(font, text[n], mono, &glyph)) continue;

    /* now, draw to our target surface (convert position) */
    _draw_bitmap(glyph.data, glyph.width, glyph.rows, glyph.pitch, mono,
                 image, w, h, (x + glyph.left)/(mono?8:1), y - glyph.top);

    /* increment pen position */
    x += glyph.advance_x;
    y += glyph.advance_y;
  }

  freetype_close_font(font);

  /* Now write results */
  if ((f = fopen(outfile, "w")))
  {
    char header[100];
    if (mono) n = strfmt(header, "P4\n%d %d\n", w*8, h);
    else      n = strfmt(header, "P5\n%d %d\n255\n", w, h);
    fwrite(header, 1, n, f);
    fwrite(image, w, h, f);
    fclose(f);
  }

  MEM_FREE_AND_NULL(image);
  return CMD_OK;
}

static int _gray(int argc, char* argv[]) { return _freetype(argc, argv, 0); }
static int _mono(int argc, char* argv[]) { return _freetype(argc, argv, 1); }

static int _glyph(int argc, char* argv[])
{
  unsigned int unicode;
  char* fontfile;
  char* outfile;
  int ptsize=12;
  int resolution=300;

  void* font;
  freetype_glyph_data_t glyph;
  FILE* f;
  char header[40];
  int n;

  if (argc < 4) return CMD_USAGE_ERROR;

                unicode    = cmd_atoi(argv[1]);
                fontfile   =          argv[2];
                outfile    =          argv[3];
  if (argc > 4) ptsize     = cmd_atoi(argv[4]);
  if (argc > 5) resolution = cmd_atoi(argv[5]);

  /* Open font at the specified ptsize and resolution */
  if (!strcmp(fontfile, "-")) fontfile = NULL; /* Use built in font */
  font = freetype_open_font(fontfile, ptsize, resolution, resolution);
  if (!font)
  {
    cmd_printf("Failed to open font\n");
    return CMD_ERROR;
  }

  /* load glyph image into the slot (erase previous one) */
  if (freetype_render_glyph(font, unicode, 1, &glyph))
  {
    cmd_printf("Failed to render glyph\n");
    return CMD_ERROR;
  }

  cmd_printf("Glyph Rendered (%d): width=%d, rows=%d, pitch=%d, L=%d, T=%d\n",
             unicode, glyph.width, glyph.rows,glyph.pitch,glyph.left,glyph.top);

  /* Create file */
  if (!(f = fopen(outfile, "w")))
  {
    freetype_close_font(font);
    cmd_printf("Failed to create output file (%s)\n", outfile);
    return CMD_ERROR;
  }

  n = strfmt(header, "P4\n%d %d\n", glyph.pitch*8, glyph.rows);
  if (n != fwrite(header, 1, n, f))
  {
    freetype_close_font(font);
    fclose(f);
    cmd_printf("Failed to write image header\n");
    return CMD_ERROR;
  }

  for (n=0;n<glyph.rows;n++)
  {
    if (glyph.pitch != fwrite(glyph.data, 1, glyph.pitch, f))
    {
      freetype_close_font(font);
      fclose(f);
      cmd_printf("Failed to write image row %d\n", n+1);
      return CMD_ERROR;
    }
    glyph.data += glyph.pitch;
  }
  freetype_close_font(font);
  fclose(f);
  return CMD_OK;
}

void freetype_cmd_init(void)
{
  cmd_register("freetype", "Freetype commands", NULL, NULL, NULL);
  cmd_register("freetype greytext", "Render a string into a greymap",
               "<fontfile> <text> <outfile> [<width>=320] [<height>=200]",
               NULL, _gray);
  cmd_register("freetype monotext", "Render a string into a bitmap",
               "<fontfile> <text> <outfile> [<width>=320] [<height>=200]",
               NULL, _mono);
  cmd_register("freetype glyph", "Extract a glyph's bitmap",
              "<unicode> <fontfile> <outfile> [<ptsize>=12] [<resolution>=300]",
              NULL, _glyph);
}
