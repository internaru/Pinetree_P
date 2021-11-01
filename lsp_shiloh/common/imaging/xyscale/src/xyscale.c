/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <string.h>
#include <stdint.h>
#include "xyscale.h"

#define _MAGIC1    (0x12ab54ef)
#define _MAGIC2    (0x0945de23)

typedef struct _hdl_st
{
  uint32_t    magic1;
  union
  {
    uint32_t *cur32;
    uint8_t  *cur8;
  };
  union
  {
    uint32_t *sum;
    uint32_t *prv32;
    uint8_t  *prv8;
  };
  int       (*fn_i)(struct _hdl_st* h, uint8_t* data);
  int       (*fn_o)(struct _hdl_st* h, uint8_t* data);
  uint32_t    in_y;
  uint32_t    out_y;
  uint32_t    in_x;
  uint32_t    out_x;
  uint32_t    out_x_dim;
  uint32_t    inbytes;
  uint32_t    outbytes;
  uint32_t    inrows;
  uint32_t    y_replicate; /* A flag indicating that we need to shift output */
  union
  {
    uint32_t  outpos;
    uint32_t  inpos;
  };
  uint32_t    planes;
  uint32_t    sum0;
  uint32_t    magic2;
} _hdl_t;

/*----------------------------------------------------------------------------*/
static void _scale8(uint32_t* dst, uint8_t* src, uint32_t num, uint32_t scale)
{
  while (num--)
    *dst++ = scale * (*src++);
}

/*----------------------------------------------------------------------------*/
static void _sum8(uint32_t* dst, uint8_t* src, uint32_t num, uint32_t scale)
{
  while (num--)
  {
    *dst = ((*dst) + (scale * (*src++)));
     dst++;
  }
}

/*----------------------------------------------------------------------------*/
static void _scale32(uint32_t* dst, uint32_t* src, uint32_t num, uint32_t scale)
{
  while (num--)
    *dst++ = scale * (*src++);
}

/*----------------------------------------------------------------------------*/
static void _sum32(uint32_t* dst, uint32_t* src, uint32_t num, uint32_t scale)
{
  while (num--)
  {
    *dst = ((*dst) + (scale * (*src++)));
     dst++;
  }
}

/*----------------------------------------------------------------------------*/
static void _write32(uint8_t* dst, uint32_t* src, uint32_t num, uint32_t div)
{
  while (num--) *dst++ = (*src++ + div/2) / div;
}

/*----------------------------------------------------------------------------*/
static void _interp8y(uint8_t* dst, uint8_t* prv, uint8_t* cur,
                      uint32_t num, uint32_t w2, uint32_t total)
{
  uint32_t w1 = total-w2;
  while (num--)
    *dst++ = (w1 * *prv++ + w2 * *cur++ + total/2)/total;
}

/*----------------------------------------------------------------------------*/
static void _interp32y(uint8_t* dst, uint32_t* prv, uint32_t* cur,
                       uint32_t num, uint32_t w2, uint32_t total, uint32_t div)
{
  uint32_t w1 = total-w2;
  total *= div;
  while (num--)
    *dst++ = (w1 * *prv++ + w2 * *cur++ + total/2)/total;
}

/*----------------------------------------------------------------------------*/
static void _interp8x(uint8_t* dst, uint8_t* src,
                      uint32_t inxs, uint32_t outxs, uint32_t outx, uint32_t planes)
{
  uint32_t x, p, pos;
  uint8_t  *d, *s, prv, cur;
  uint32_t inpix = (inxs * outx)/outxs; /* We know outx/outxs is an integer */
  uint32_t ipc; /* In pix count - don't want to use more pixels then we have */
  for (p=0;p<planes;p++)
  {
    pos = outxs/2; /* Start at a half position to maintain X centering */
    s   = src + p;
    prv = cur = *s;
    d   = dst + p;
    ipc = inpix;
    for (x=0;x<outx;x++)
    {
      if (pos) *d = ((outxs - pos) * prv + pos * cur + outxs/2) / outxs;
      else     *d = prv;
      d += planes;
      if ((pos += inxs) > outxs)
      {
        pos -= outxs;
        prv  = cur;
        if (--ipc) s += planes;
        cur  = *s;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
static void _interp32x(uint32_t* dst, uint8_t* src,
                       uint32_t inxs, uint32_t outxs, uint32_t outx, uint32_t planes)
{
  uint32_t *d, x, p, pos;
  uint8_t  *s, prv, cur;
  uint32_t inpix = (inxs * outx)/outxs; /* We know outx/outxs is an integer */
  uint32_t ipc; /* In pix count - don't want to use more pixels then we have */
  for (p=0;p<planes;p++)
  {
    pos = outxs/2; /* Start at a half position to maintain X centering */
    s   = src + p;
    prv = cur = *s;
    d   = dst + p;
    ipc = inpix;
    for (x=0;x<outx;x++)
    {
      if (pos) *d = (outxs - pos) * prv + pos * cur;
      else     *d =  outxs        * prv;
      d += planes;
      if ((pos += inxs) > outxs)
      {
        pos -= outxs;
        prv  = cur;
        if (--ipc) s += planes;
        cur  = *s;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
static void _scale32x(uint32_t* dst, uint8_t* src,
                      uint32_t inxs, uint32_t outxs, uint32_t outx,
                      uint32_t planes)
{
  uint32_t *d, x, sum, pos, p;
  uint8_t  *s, c=0;
  for (p=0;p<planes;p++)
  {
    s = src + p;
    d = dst + p;
    sum = 0;
    pos = 0;
    for (x=0;x<outx;x++)
    {
      while (pos < inxs)
      {
        int r = inxs - pos;
        c = *s; s += planes;
        if (r >= outxs) sum += c * outxs;
        else            sum += c * r;
        pos += outxs;
      }
      *d = sum; d += planes;
      pos -= inxs;
      if (pos) sum = c * pos;
      else     sum = 0;
    }
  }
}

/*----------------------------------------------------------------------------*/
static void _scale8x(uint8_t* dst, uint8_t* src,
                     uint32_t inxs, uint32_t outxs, uint32_t outx,
                     uint32_t planes)
{
  uint32_t x, sum, pos, p;
  uint8_t  *d, *s, c=0;
  for (p=0;p<planes;p++)
  {
    d = dst + p;
    s = src + p;
    sum = 0;
    pos = 0;
    for (x=0;x<outx;x++)
    {
      while (pos < inxs)
      {
        int r = inxs - pos;
        c = *s; s += planes;
        if (r >= outxs) sum += c * outxs;
        else            sum += c * r;
        pos += outxs;
      }
      *d = (sum + inxs/2)/inxs; d += planes;
      pos -= inxs;
      if (pos) sum = c * pos;
      else     sum = 0;
    }
  }
}

/*----------------------------------------------------------------------------*/
static void _choose8up(uint8_t* dst, uint8_t* src,
                       uint32_t inxs, uint32_t outxs, uint32_t outx,
                       uint32_t planes)
{
  uint32_t x, p, pos;
  uint8_t  *d, *s;
  for (p=0;p<planes;p++)
  {
    d = dst+p;
    s = src+p;
    pos = 0;
    for (x=0;x<outx;x++)
    {
      *d = *s; d += planes;
      if ((pos += inxs) > outxs)
      {
        pos -= outxs;
        s += planes;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
static void _choose8dwn(uint8_t* dst, uint8_t* src,
                        uint32_t inxs, uint32_t outxs, uint32_t outx,
                        uint32_t planes)
{
  uint32_t x, p, pos;
  uint8_t  *d, *s;
  for (p=0;p<planes;p++)
  {
    d = dst+p;
    s = src+p;
    pos = 0;
    for (x=0;x<outx;x++)
    {
      *d = *s; d += planes;
      pos += inxs;
      while (pos > outxs)
      {
        pos -= outxs;
        s += planes;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* X interpolate input (8->32), Y interpolate output (32->8) */
static int _up_up_i(_hdl_t* h, uint8_t* data)
{
  uint32_t* tmp;
  if (h->inrows >= 2) return XYSCALE_DATA_FULL;
  tmp = h->cur32;
  h->cur32 = h->prv32;
  h->prv32 = tmp;
  _interp32x(h->cur32, data, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->inrows += 1;
  if (h->y_replicate)
  {
    h->y_replicate = 0;
    h->outpos = h->out_y / 2;
    return XYSCALE_DATA_FULL; /* Let's get this first row once again */
  }
  return XYSCALE_OKAY;
}
static int _up_up_o(_hdl_t* h, uint8_t* data)
{
  if (h->inrows != 2) return XYSCALE_DATA_EMPTY;
  if (h->outpos) _interp32y(data, h->prv32, h->cur32, h->outbytes, h->outpos, h->out_y, h->out_x);
  else             _write32(data, h->prv32,           h->outbytes,                      h->out_x);
  if ((h->outpos += h->in_y) >= h->out_y)
  {
    h->outpos -= h->out_y;
    h->inrows -= 1;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Keep nearest row and choose nearest column on output */
/* Input is _keep_1row */
static int _up_up_o_fast(_hdl_t* h, uint8_t* data)
{
  if (!h->inrows) return XYSCALE_DATA_EMPTY;
  _choose8up(data, h->cur8, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->outpos += h->in_y;
  if (h->outpos > h->out_y)
  {
    h->outpos -= h->out_y;
    h->inrows = 0;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Copy input (8->8), X interpolate output (8->8) */
static int _up_eq_i(_hdl_t* h, uint8_t* data)
{
  if (h->inrows) return XYSCALE_DATA_FULL;
  memcpy(h->cur8, data, h->inbytes);
  h->inrows = 1;
  return XYSCALE_OKAY;
}
static int _up_eq_o(_hdl_t* h, uint8_t* data)
{
  if (h->inrows != 1) return XYSCALE_DATA_EMPTY;
  _interp8x(data, h->cur8, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->inrows = 0;
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Copy input (8->8), Choose nearest neighbor output */
/* Input is _keep_1row */
static int _up_eq_o_fast(_hdl_t* h, uint8_t* data)
{
  if (h->inrows != 1) return XYSCALE_DATA_EMPTY;
  _choose8up(data, h->cur8, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->inrows = 0;
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* X Interpolate input (8->32), Y scale output */
static int _up_dwn_i(_hdl_t* h, uint8_t* data)
{
  int r = h->in_y - h->inrows; /* How much room do we have? */
  if (r <= 0) return XYSCALE_DATA_FULL;
  _interp32x(h->cur32, data, h->in_x, h->out_x, h->out_x_dim, h->planes);
  if (r >= h->out_y)
  {
    if (h->sum0) { _scale32(h->sum, h->cur32, h->outbytes, h->out_y); h->sum0 = 0; }
    else             _sum32(h->sum, h->cur32, h->outbytes, h->out_y);
  }
  else
  {
    if (h->sum0) { _scale32(h->sum, h->cur32, h->outbytes, r); h->sum0 = 0; }
    else             _sum32(h->sum, h->cur32, h->outbytes, r);
  }
  h->inrows += h->out_y;
  return XYSCALE_OKAY;
}
static int _up_dwn_o(_hdl_t* h, uint8_t* data)
{
  if (h->inrows < h->in_y) return XYSCALE_DATA_EMPTY;
  if (h->inrows -= h->in_y)
  {
    _write32(data, h->sum, h->outbytes, h->in_y * h->out_x);
    _scale32(h->sum, h->cur32, h->outbytes, h->inrows);
    h->sum0 = 0;
  }
  else
  {
    _write32(data, h->sum, h->outbytes, h->in_y * h->out_x);
    h->sum0 = 1;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* inx<outx ; iny>outy : Keep useful rows and choose nearest column on output */
static int _keep_useful_rows(_hdl_t* h, uint8_t* data)
{
  if (h->inrows > h->in_y) return XYSCALE_DATA_FULL;
  h->inrows += h->out_y;
  memcpy(h->cur8, data, h->inbytes);
  return XYSCALE_OKAY;
}
static int _up_dwn_o_fast(_hdl_t* h, uint8_t* data)
{
  if (h->inrows < h->in_y) return XYSCALE_DATA_EMPTY;
  _choose8up(data, h->cur8, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->inrows -= h->in_y;
  return XYSCALE_OKAY;
}


/*----------------------------------------------------------------------------*/
/* Keep 2 input rows and y interpolate on output */
static int _eq_up_i(_hdl_t* h, uint8_t* data)
{
  uint8_t* tmp;
  if (h->inrows >= 2) return XYSCALE_DATA_FULL;
  tmp = h->cur8;
  h->cur8 = h->prv8;
  h->prv8 = tmp;
  memcpy(h->cur8, data, h->inbytes);
  h->inrows += 1;
  if (h->y_replicate)
  {
    h->y_replicate = 0;
    h->outpos = h->out_y / 2;
    return XYSCALE_DATA_FULL; /* Let's get this first row once again */
  }
  return XYSCALE_OKAY;
}
static int _eq_up_o(_hdl_t* h, uint8_t* data)
{
  if (h->inrows != 2) return XYSCALE_DATA_EMPTY;
  if (h->outpos) _interp8y(data, h->prv8, h->cur8, h->outbytes, h->outpos, h->out_y);
  else              memcpy(data, h->prv8,          h->outbytes);
  if ((h->outpos += h->in_y) >= h->out_y)
  {
    h->outpos -= h->out_y;
    h->inrows -= 1;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Keep nearest row  */
/* Input is _keep_1row */
static int _eq_up_o_fast(_hdl_t* h, uint8_t* data)
{
  if (!h->inrows) return XYSCALE_DATA_EMPTY;
  memcpy(data, h->cur8, h->outbytes);
  h->outpos += h->in_y;
  if (h->outpos > h->out_y)
  {
    h->outpos -= h->out_y;
    h->inrows = 0;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Copy a row in, copy a row out */
static int _eq_eq_i(_hdl_t* h, uint8_t* data)
{
  if (h->inrows) return XYSCALE_DATA_FULL;
  memcpy(h->cur8, data, h->inbytes);
  h->inrows = 1;
  return XYSCALE_OKAY;
}
static int _eq_eq_o(_hdl_t* h, uint8_t* data)
{
  if (!h->inrows) return XYSCALE_DATA_EMPTY;
  memcpy(data, h->cur8, h->outbytes);
  h->inrows = 0;
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Sum weighted versions of the input into a sum and divide on output */
static int _eq_dwn_i(_hdl_t* h, uint8_t* data)
{
  int r = h->in_y - h->inrows; /* How much room do we have? */
  if (r <= 0) return XYSCALE_DATA_FULL;
  if (r >= h->out_y)
  {
    if (h->sum0) { _scale8(h->sum, data, h->inbytes, h->out_y); h->sum0 = 0; }
    else           _sum8(h->sum, data, h->inbytes, h->out_y);
  }
  else /* (r < h->out_y) */
  {
    if (h->sum0) { _scale8(h->sum, data, h->inbytes, r); h->sum0 = 0; }
    else             _sum8(h->sum, data, h->inbytes, r);
    memcpy(h->cur8, data, h->inbytes);
  }
  h->inrows += h->out_y;
  return XYSCALE_OKAY;
}
static int _eq_dwn_o(_hdl_t* h, uint8_t* data)
{
  if (h->inrows < h->in_y) return XYSCALE_DATA_EMPTY;
  if (h->inrows -= h->in_y)
  {
    _write32(data, h->sum, h->outbytes, h->in_y);
    _scale8(h->sum, h->cur8, h->outbytes, h->inrows);
    h->sum0 = 0;
  }
  else
  {
    _write32(data, h->sum, h->outbytes, h->in_y);
    h->sum0 = 1;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* inx==outx ; iny>outy : Keep useful rows */
static int _eq_dwn_i_fast(_hdl_t* h, uint8_t* data)
{
  if (h->inrows > h->in_y) return XYSCALE_DATA_FULL;
  h->inrows += h->out_y;
  memcpy(h->cur8, data, h->inbytes);
  return XYSCALE_OKAY;
}
static int _eq_dwn_o_fast(_hdl_t* h, uint8_t* data)
{
  if (h->inrows < h->in_y) return XYSCALE_DATA_EMPTY;
  memcpy(data, h->cur8, h->outbytes);
  h->inrows -= h->in_y;
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* X scale input (8->32), Y interpolate output (32->8) */
static int _dwn_up_i(_hdl_t* h, uint8_t* data)
{
  uint32_t* tmp;
  if (h->inrows >= 2) return XYSCALE_DATA_FULL;
  tmp = h->cur32;
  h->cur32 = h->prv32;
  h->prv32 = tmp;
  _scale32x(h->cur32, data, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->inrows += 1;
  if (h->y_replicate)
  {
    h->y_replicate = 0;
    h->outpos = h->out_y / 2;
    return XYSCALE_DATA_FULL; /* Let's get this first row once again */
  }
  return XYSCALE_OKAY;
}
static int _dwn_up_o(_hdl_t* h, uint8_t* data)
{
  if (h->inrows != 2) return XYSCALE_DATA_EMPTY;
  if (h->outpos) _interp32y(data, h->prv32, h->cur32, h->outbytes, h->outpos, h->out_y, h->in_x);
  else             _write32(data, h->prv32,           h->outbytes,                      h->in_x);
  if ((h->outpos += h->in_y) >= h->out_y)
  {
    h->outpos -= h->out_y;
    h->inrows -= 1;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
static int _keep_1row(_hdl_t* h, uint8_t* data)
{
  if (h->inrows) return XYSCALE_DATA_FULL;
  memcpy(h->cur8, data, h->inbytes);
  h->inrows = 1;
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Keep nearest row and choose nearest column on output */
static int _dwn_up_i_fast(_hdl_t* h, uint8_t* data)
{
  if (h->inrows) return XYSCALE_DATA_FULL;
  _choose8dwn(h->cur8, data, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->inrows = 1;
  return XYSCALE_OKAY;
}
static int _dwn_up_o_fast(_hdl_t* h, uint8_t* data)
{
  if (!h->inrows) return XYSCALE_DATA_EMPTY;
  memcpy(data, h->cur8, h->outbytes);
  h->outpos += h->in_y;
  if (h->outpos > h->out_y)
  {
    h->outpos -= h->out_y;
    h->inrows = 0;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Xscale scale input (8->8), copy output */
static int _dwn_eq_i(_hdl_t* h, uint8_t* data)
{
  if (h->inrows) return XYSCALE_DATA_FULL;
  _scale8x(h->cur8, data, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->inrows = 1;
  return XYSCALE_OKAY;
}
static int _dwn_eq_o(_hdl_t* h, uint8_t* data)
{
  if (!h->inrows) return XYSCALE_DATA_EMPTY;
  memcpy(data, h->cur8, h->outbytes);
  h->inrows = 0;
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
static int _dwn_eq_i_fast(_hdl_t* h, uint8_t* data)
{
  if (h->inrows) return XYSCALE_DATA_FULL;
  _choose8dwn(h->cur8, data, h->in_x, h->out_x, h->out_x_dim, h->planes);
  h->inrows = 1;
  return XYSCALE_OKAY;
}
static int _dwn_eq_o_fast(_hdl_t* h, uint8_t* data)
{
  if (!h->inrows) return XYSCALE_DATA_EMPTY;
  memcpy(data, h->cur8, h->outbytes);
  h->inrows = 0;
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* X scale input (8->32), Y scale output (32->8) */
static int _dwn_dwn_i(_hdl_t* h, uint8_t* data)
{
  int r = h->in_y - h->inrows; /* How much room do we have? */
  if (r <= 0) return XYSCALE_DATA_FULL;
  _scale32x(h->cur32, data, h->in_x, h->out_x, h->out_x_dim, h->planes);
  if (r >= h->out_y)
  {
    if (h->sum0) { _scale32(h->sum, h->cur32, h->outbytes, h->out_y); h->sum0 = 0; }
    else             _sum32(h->sum, h->cur32, h->outbytes, h->out_y);
  }
  else
  {
    if (h->sum0) { _scale32(h->sum, h->cur32, h->outbytes, r); h->sum0 = 0; }
    else             _sum32(h->sum, h->cur32, h->outbytes, r);
  }
  h->inrows += h->out_y;
  return XYSCALE_OKAY;
}
static int _dwn_dwn_o(_hdl_t* h, uint8_t* data)
{
  if (h->inrows < h->in_y) return XYSCALE_DATA_EMPTY;
  if (h->inrows -= h->in_y)
  {
    _write32(data, h->sum, h->outbytes, h->in_y * h->in_x);
    _scale32(h->sum, h->cur32, h->outbytes, h->inrows);
    h->sum0 = 0;
  }
  else
  {
    _write32(data, h->sum, h->outbytes, h->in_y * h->in_x);
    h->sum0 = 1;
  }
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
/* Special case (for contrast overlay) scaling grayscale data down by 8x8 */
static int _dwn_dwn_i_by8(_hdl_t* h, uint8_t* data)
{
  int n, x;
  uint32_t *sptr;
  
  if (h->inrows == 8) return XYSCALE_DATA_FULL;

  sptr = h->sum;
  if (h->inrows)
  {
    for (x=0;x<h->out_x_dim;x++)
    {
      for (n=0;n<8;n++) *sptr += *data++;
                         sptr += 1;
    }
  }
  else
  {
    for (x=0;x<h->out_x_dim;x++)
    {
                        *sptr  = *data++;
      for (n=0;n<7;n++) *sptr += *data++;
                         sptr += 1;
    }
  }
  h->inrows += 1;
  return XYSCALE_OKAY;
}
static int _dwn_dwn_o_by8(_hdl_t* h, uint8_t* data)
{
  int x;
  uint32_t *sptr;

  if (h->inrows < 8) return XYSCALE_DATA_EMPTY;
  sptr = h->sum;
  for (x=0;x<h->out_x_dim;x++) *data++ = ((*sptr+++32)>>6);
  h->inrows = 0;
  return XYSCALE_OKAY;
  
}

/*----------------------------------------------------------------------------*/
/* inx<outx ; iny>outy : Choose nearest columns on input of useful rows */
static int _dwn_dwn_i_fast(_hdl_t* h, uint8_t* data)
{
  if (h->inrows > h->in_y) return XYSCALE_DATA_FULL;
  h->inrows += h->out_y;
  _choose8dwn(h->cur8, data, h->in_x, h->out_x, h->out_x_dim, h->planes);
  return XYSCALE_OKAY;
}
static int _dwn_dwn_o_fast(_hdl_t* h, uint8_t* data)
{
  if (h->inrows < h->in_y) return XYSCALE_DATA_EMPTY;
  memcpy(data, h->cur8, h->outbytes);
  h->inrows -= h->in_y;
  return XYSCALE_OKAY;
}

/*----------------------------------------------------------------------------*/
static void* _malloc(uint8_t** mem, uint32_t* memsize,
                     uint32_t size, uint32_t align)
{
  void* m;
  align -= 1;
  while ((uint32_t)*mem & align) { *mem += 1; *memsize -= 1; }
  if (*memsize < size) return NULL;
  m = *mem;
  *memsize -= size;
  *mem += size;
  return m;
}

/*----------------------------------------------------------------------------*/
static int _configure(_hdl_t* h, xyscale_speed_t speed,
                       uint8_t** mem, uint32_t* memsize)
{
  if        (h->in_x == h->out_x)
  {
    if      (h->in_y == h->out_y)
    {
      /* inx=outx ; iny==outy */
      /* copy input, no scaling */
      h->fn_i = _eq_eq_i;
      h->fn_o = _eq_eq_o;
      if (!(h->cur8 =_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
    }
    else if (h->in_y >  h->out_y)
    {
      /* inx=outx ; iny>outy */
      if (speed == XYSCALE_SPEED_FAST)
      {
        /* keep nearest rows */
        h->fn_i = _eq_dwn_i_fast;
        h->fn_o = _eq_dwn_o_fast;
        if (!(h->cur8 =_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
      }
      else
      {
        /* keep an current copy,
           yscale down (averaging) keep a 32bit sum */
        h->fn_i = _eq_dwn_i;
        h->fn_o = _eq_dwn_o;
        if (!(h->cur8 =_malloc(mem,memsize,  h->planes*h->in_x,1))) return -1;
        if (!(h->sum  =_malloc(mem,memsize,4*h->planes*h->in_x,4))) return -1;
      }
    }
    else /* (h->in_y <  h->out_y) */
    {
      /* inx=outx ; iny<outy */
      if (speed == XYSCALE_SPEED_FAST)
      {
        /* keep nearest rows */
        h->fn_i = _keep_1row;
        h->fn_o = _eq_up_o_fast;
        if (!(h->cur8 =_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
      }
      else
      {
        /* keep two input rows and bilinear scale output */
        h->fn_i = _eq_up_i;
        h->fn_o = _eq_up_o;
        if (!(h->cur8 =_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
        if (!(h->prv8 =_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
        h->y_replicate = 1; /* Shift output down 1/2 input row */
      }
    }
  }
  else if   (h->in_x > h->out_x)
  {
    /* inx>outx ; iny==outy */
    if      (h->in_y == h->out_y)
    {
      if (speed == XYSCALE_SPEED_FAST)
      {
        /* choose nearest column on input, copy to output */
        h->fn_i = _dwn_eq_i_fast;
        h->fn_o = _dwn_eq_o_fast;
        if (!(h->cur8 =_malloc(mem,memsize,h->planes*h->out_x,1))) return -1;
      }
      else
      {
        /* xscale down input (averaging), keep output */
        h->fn_i = _dwn_eq_i;
        h->fn_o = _dwn_eq_o;
        if (!(h->cur8 =_malloc(mem,memsize,h->planes*h->out_x,1))) return -1;
      }
    }
    else if (h->in_y >  h->out_y)
    {
      /* inx>outx ; iny>outy */
      if (speed == XYSCALE_SPEED_FAST)
      {
        /* choose nearest columns on input of useful rows */
        h->fn_i = _dwn_dwn_i_fast;
        h->fn_o = _dwn_dwn_o_fast;
        if (!(h->cur8 =_malloc(mem,memsize,h->planes*h->out_x,1))) return -1;
      }
      else if ((h->planes == 1) &&
               ((h->out_x * 8) == h->in_x) &&
               ((h->out_y * 8) == h->in_y))
      {
        /* Optimized case for contrast overlay */
        /* Sum input for 8 columns and 8 rows into sum and then divide by 64 */
        h->fn_i = _dwn_dwn_i_by8;
        h->fn_o = _dwn_dwn_o_by8;
        if (!(h->sum  =_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
      }
      else
      {
        /* xscale down input (averaging - keep in 32bit)
           yscale down input (averaging - keep 32bit sum) */
        h->fn_i = _dwn_dwn_i;
        h->fn_o = _dwn_dwn_o;
        if (!(h->cur32=_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
        if (!(h->sum  =_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
      }
    }
    else /* (h->in_y <  h->out_y) */
    {
      /* inx>outx ; iny<outy */
      if (speed == XYSCALE_SPEED_FAST)
      {
        /* choose nearest columns on input of useful rows */
        h->fn_i = _dwn_up_i_fast;
        h->fn_o = _dwn_up_o_fast;
        if (!(h->cur8=_malloc(mem,memsize,h->planes*h->out_x,1))) return -1;
      }
      else
      {
        /* xscale down input (averaging - keep in 32bit)
           yscale up output (bilinear - keep prev row) */
        h->fn_i = _dwn_up_i;
        h->fn_o = _dwn_up_o;
        if (!(h->cur32=_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
        if (!(h->prv32=_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
        h->y_replicate = 1; /* Shift output down 1/2 input row */
      }
    }
  }
  else   /* (h->in_x <  h->out_x) */
  {
    if      (h->in_y == h->out_y)
    {
      /* inx<outx ; iny==outy */
      if (speed == XYSCALE_SPEED_FAST)
      {
        /* copy input, choose nearest column on output */
        h->fn_i = _keep_1row;
        h->fn_o = _up_eq_o_fast;
        if (!(h->cur8=_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
      }
      else
      {
        /* copy input, bilinear scale output (8 bit inx) */
        h->fn_i = _up_eq_i;
        h->fn_o = _up_eq_o;
        if (!(h->cur8=_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
      }
    }
    else if (h->in_y >  h->out_y)
    {
      /* inx<outx ; iny>outy */
      if (speed == XYSCALE_SPEED_FAST)
      {
         /* keep useful rows choose nearest column on output */
        h->fn_i  = _keep_useful_rows;
        h->fn_o  = _up_dwn_o_fast;
        if (!(h->cur8=_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
      }
      else
      {
         /* xscale up input (keep 32bit bilinear row)
            yscale down output (keep average sum) */
        h->fn_i  = _up_dwn_i;
        h->fn_o  = _up_dwn_o;
        if (!(h->cur32=_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
        if (!(h->sum  =_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
      }
    }
    else /* (h->in_y <  h->out_y) */
    {
      /* inx<outx ; iny<outy */
      if (speed == XYSCALE_SPEED_FAST)
      {
         /* keep nearest row choose nearest column on output */
        h->fn_i  = _keep_1row;
        h->fn_o  = _up_up_o_fast;
        if (!(h->cur8=_malloc(mem,memsize,h->planes*h->in_x,1))) return -1;
      }
      else
      {
        /* xscale up input (keep 32bit bilinear row)
           yscale up output (keep a previous row) */
        h->fn_i  = _up_up_i;
        h->fn_o  = _up_up_o;
        if (!(h->cur32=_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
        if (!(h->prv32=_malloc(mem,memsize,4*h->planes*h->out_x,4))) return -1;
        h->y_replicate = 1; /* Shift output down 1/2 input row */
      }
    }
  }
  return 0;
}

static void _reduce(uint32_t *v1, uint32_t *v2)
{
  uint32_t p, x1 = *v1, x2 = *v2;

  /* Note that we probably do not need this many primes. 101*101 > 10,000 */
  static const unsigned _p[]={2,3,5,7,11,13,17,19,21,23,29,31,37,41,43,47,53,
                              59,61,67,71,73,79,83,89,97,101};
  for (p=0;p<(sizeof(_p)/sizeof(_p[0]));p++)
  {
    uint32_t sqr = _p[p] * _p[p];
    if ((sqr > x1) || (sqr > x2)) break;

    while (!(x1%_p[p]) && !(x2%_p[p]))
    {
      x1 /= _p[p];
      x2 /= _p[p];
    }
  }
  *v1 = x1;
  *v2 = x2;
}

int xyscale_outdata(xyscale_hdl_t hdl, uint8_t* data)
{
  _hdl_t* h = (_hdl_t*)hdl;
  if (!h||(h->magic1!=_MAGIC1)||(h->magic2!=_MAGIC2))
    return XYSCALE_HANDLE_ERR;
  return h->fn_o(h, data);
}

int xyscale_indata(xyscale_hdl_t hdl, uint8_t* data)
{
  _hdl_t* h = (_hdl_t*)hdl;
  if (!h||(h->magic1!=_MAGIC1)||(h->magic2!=_MAGIC2))
    return XYSCALE_HANDLE_ERR;
  return h->fn_i(h, data);
}

uint32_t xyscale_mem_size(int planes,
                          int in_x, int in_y, int out_x, int out_y,
                          xyscale_speed_t speed)
{
  _hdl_t h;
  uint32_t memsize = 0xfffffffc;
  uint8_t* mem = (uint8_t*)4;

  memset(&h, 0, sizeof(_hdl_t));
  h.planes = planes;
  h.in_x = in_x;
  h.in_y = in_y;
  h.out_x = out_x;
  h.out_y = out_y;

  _malloc(&mem, &memsize, sizeof(_hdl_t), sizeof(_hdl_t*));
  _configure(&h, speed, &mem, &memsize);
  _malloc(&mem, &memsize, sizeof(void*), sizeof(void*)); /* For rounding */
  return memsize = 0xfffffffc - memsize;
}

int xyscale_init(xyscale_hdl_t* hdl,
                 int planes, int in_x, int in_y, int out_x, int out_y,
                 uint8_t* mem, uint32_t memsize,
                 xyscale_speed_t speed)
{
  /* Check and allocate memory */
  _hdl_t* h;
  if (!(h = _malloc(&mem, &memsize, sizeof(_hdl_t), sizeof(_hdl_t*))))
    return XYSCALE_MEMORY_ERR;
  memset(h, 0, sizeof(_hdl_t));
  h->planes = planes;
  h->in_x = in_x;
  h->in_y = in_y;
  h->out_x = out_x;
  h->out_y = out_y;
  if (_configure(h, speed, &mem, &memsize)) return XYSCALE_MEMORY_ERR;

  /* Set up shrotcuts and whatnot */
  h->out_x_dim = out_x;
  h->outbytes = out_x * planes;
  h->inbytes  = in_x  * planes;
  _reduce(&h->in_y, &h->out_y);
  _reduce(&h->in_x, &h->out_x);
  h->sum0 = 1;

  h->magic1   = _MAGIC1;
  h->magic2   = _MAGIC2;
  *hdl = h;

  return XYSCALE_OKAY;
}

