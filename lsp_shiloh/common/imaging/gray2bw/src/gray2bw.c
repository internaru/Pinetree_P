/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include <stdint.h>
#include "gray2bw.h"

static int _do_pixel(unsigned char* c, short* e, int x)
{
  int v = e[x] + (255-c[x]);
  if (v > 127) c[x] = 255;
  else         c[x] = 0;
  return v - c[x];
}

static void _dither(short* p,         /* Prev error */
                    unsigned char* c, /* Current data */
                    short* n,         /* Next error */
                    int w, int dir)
{
#if 0 /* This is thresholding which does not work well for images */
  int i;
  for (i=0;i<w;i++)
    if (c[i] > 127) c[i] = 0;
    else            c[i] = 255;
#else /* What follows is serpentine floyd stienberg */
  int i, e;

  memset(n, 0, w*2);
  if (dir)
  {
    e = _do_pixel(c, p, i=0);
    p[i+1] +=    e /2;
    n[i+1] += (1*e)/8;
    n[i]   += (3*e)/8;
    for (i=1;i<(w-1);i++)
    {
      e = _do_pixel(c, p, i);
      p[i+1] += (7*e)/16;
      n[i+1] += (1*e)/16;
      n[i]   += (5*e)/16;
      n[i-1] += (3*e)/16;
    }
    e = _do_pixel(c, p, i=w-1);
    n[i]   += (5*e)/8;
    n[i-1] += (3*e)/8;
  }
  else
  {
    e = _do_pixel(c, p, i=w-1);
    p[i-1] +=    e /2;
    n[i-1] += (1*e)/8;
    n[i]   += (3*e)/8;
    for (i=w-2;i>0;i--)
    {
      e = _do_pixel(c, p, i);
      p[i-1] += (7*e)/16;
      n[i-1] += (1*e)/16;
      n[i]   += (5*e)/16;
      n[i+1] += (3*e)/16;
    }
    e = _do_pixel(c, p, i=0);
    n[i]   += (5*e)/8;
    n[i+1] += (3*e)/8;
  }
#endif
}

static void _pack(unsigned char* out, unsigned char* in, int w)
{
  unsigned char byte = 0;
  while (w)
  {
    if (*in++) byte |= 0x80; if (!--w) break;
    if (*in++) byte |= 0x40; if (!--w) break;
    if (*in++) byte |= 0x20; if (!--w) break;
    if (*in++) byte |= 0x10; if (!--w) break;
    if (*in++) byte |= 0x08; if (!--w) break;
    if (*in++) byte |= 0x04; if (!--w) break;
    if (*in++) byte |= 0x02; if (!--w) break;
    if (*in++) byte |= 0x01; if (!--w) break;
    *out++ = byte;
    byte = 0;
  }
  *out++ = byte;
}

typedef struct
{
  int width;
  short* rows[2];
  int which;
} _cntxt_t;

uint32_t gray2bw_memreq(int width)
{
  return sizeof(_cntxt_t) + 4*width;
}

void* gray2bw_create(int width, uint32_t* mem, int mem_bytes)
{
  _cntxt_t* hdl;
  if (mem_bytes < gray2bw_memreq(width)) return NULL;
  memset(mem, 0, gray2bw_memreq(width));
  hdl = (_cntxt_t*)mem;
  mem += sizeof(_cntxt_t)/4;
  hdl->rows[0] = (short*)mem;
  hdl->rows[1] = hdl->rows[0] + width;
  hdl->width = width;
  return hdl;
}

/* This is destructive to the contents pointed to at data */
int gray2bw_convert(void* hdl, uint8_t* data, int numrows)
{
  _cntxt_t* c = (_cntxt_t*)hdl;
  uint8_t* in = data;
  uint8_t* out = data;
  while (numrows--)
  {
    _dither(c->rows[c->which], in, c->rows[c->which^1], c->width, c->which);
    _pack(out, in, c->width);
    in += c->width;
    out += (c->width+7)/8;
    c->which ^= 1;
  }
  return 0;
}
