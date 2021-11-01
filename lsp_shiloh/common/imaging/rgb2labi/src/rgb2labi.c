/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <math.h>
#include "rgb2labi.h" /* To make sure proto matches */

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)>(b))?(b):(a))


/* This pre-computed table contains a conversion from an sRGB pixel value of
   0-255 to an RGB value scaled 0-32767. */
static unsigned short _rgbtbl[256];
static void _init_rgb_table(void)
{
  int x;
  for (x=0;x<256;x++)
  {
    double p = x;
    p /= 255.0;
    if (p <= 0.04045) p /= 12.92;
    else              p = pow((p+0.055)/1.055,2.4);
    p = p * 32767.0 + 0.5;
    _rgbtbl[x] = (int)p;
  }
}

/* This pre-computed table contains the the breakpoints for finding cube roots
   of the first 1024 cubes. If asked the question, what is the cube root of
   54409, you would find, in this table, that _cubes[37] < 54409 < _cubes[38]
   or that 52734 < 54409 < 57066. This indicates that the correct answer to the
   question, properly rounded, is 38. The function _cuberoot(x) finds the
   properly rounded cube root of x for 0 <= x <= 1024^3 by binary searching
   this table. */
static unsigned int _cubes[1024];
static void _compute_cubes(void)
{
  int x;
  for (x=0;x<1024;x++)
  {
    double p = x;
           p += .5;
           p = p*p*p;
    _cubes[x] = p;
  }
}
static inline int _cuberoot(int x)
{
  int ans = 511, div = 256;
  /* X comes in as 0 - 2^30. The cube root will be between 0 and 1024 and
     we can use the pre-computed table to return the answer */
  while (div)
  {
    if      (_cubes[ans] > x) ans -= div;
    else if (_cubes[ans] < x) ans += div;
    else                      break;
    div >>= 1;
  }
  if (_cubes[ans] >= x) return ans;
  else                  return ans+1;
}

static int _var(int r, int g, int b, int row)
{
  unsigned int x;

  /* RGB to XYZ in D65 space
    http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
    0.4124564  0.3575761  0.1804375
    0.2126729  0.7151522  0.0721750
    0.0193339  0.1191920  0.9503041

    RGB to XYZ with Bradford conversion to D50 space built in
    http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
    0.4360747  0.3850649  0.1430804
    0.2225045  0.7168786  0.0606169
    0.0139322  0.0971045  0.7141733 */

  if (row == 0)
  {
    /* Compute x = X = .4360747 * r + .3850649 * g + .1430804 * b */
    x =  (28579*r>>16)-(1673*r>>28)
        +( 6309*g>>14)-(  99*g>>24)
        +( 9377*b>>16)-(2717*b>>31);
  }
  else if (row == 1)
  {
    /* Compute x = Y = .2225045 * r + .7168786 * g + .0606169 * b */
    x =  ( 7291*r>>15)+( 1799*r>>31)
        +(23491*g>>15)-(21105*g>>31)
        +(31781*b>>19)-(  587*b>>30);
  }
  else /* if (row == 2) */
  {
    /* Compute x = Z = .0139322 * r + .0971045 * g + .7141733 * b */
    x =  (14609*r>>20)-(  15*r>>29)
        +(25455*g>>18)+(1483*g>>30)
        +(11701*b>>14)+( 503*b>>29);
  }


  /* Now we need to compute one of two things.
     if (x < 216/24389) we compute (24389/27 * x + 16) / 116
     otherwise we compute the cube root of x.
     x starts as a 2^15 number (interpreted as 0->1)
     the result should be a 10 bit number.
     The 2^15 threshold is, therefore, 2^15 * 216/24389 = 290
     (24389x/27 + 16)/116 --> x(29^2)/(27*2^7) + (2^12)/29
                              (x(29^3)+2^19*27)/(29*27*2^7)
                            = (24389x + 14155776)/100224 */

  if (x < 291) return (24389*x + 14155776 + 100224/2)/100224;

  /* Now we want to compute "cube root of x"
     x is initially in the range 0-2^15
     we want the answer in the range of 0-2^10.
           cuberoot(32768)       = 32
         cuberoot(32768*32768) = 1024 */
  return _cuberoot(x << 15);
}

static inline void _srgb2lab(const unsigned char *src, unsigned char *dst,
                             const int rgb_map[3])
{
  int r, g, b, x, y, z;

  /* JSA - TODO - Consider special casing white and black pixels */

  /* convert linear RGB to sRGB (gamma corrected) */
  /* JSA - Speed this up later by doing endian checking and doing a single
           aligned read of a 32bit and unpack and all sorts of other stuff.
           For now this works well enough */
  r = _rgbtbl[src[rgb_map[0]]];
  g = _rgbtbl[src[rgb_map[1]]];
  b = _rgbtbl[src[rgb_map[2]]];

  x = _var(r, g, b, 0);
  y = _var(r, g, b, 1);
  z = _var(r, g, b, 2);

  /* Now we need to compute L*, a*, and b* from x, y, and z. We'll reuse the
     r,g,b variables to save stack space. r will hold L*, g will hold a*,
     b will hold b*. The floating point equations are:

            L* = 116 * y - 16  (we are interested in results   0 to 100)
            a* = 500 * (x - y) (we are interested in results -85 to  85)
            b* = 200 * (y - z) (we are interested in results -75 to 125)

     Now, the T.42 spec says that we want to use a unique RANGE and OFFSET for
     limitting the gamut range of each of L, a, and b as well. In fact these
     are specified in the T.32 spec as L:0 to 100; a:-85 to 85; and b:-75 to 125

     The 8 bit integral values for L, a, and b will therefore derive from
     the following equations which have shifts and scales built in:
            L = 255/100 * (116 * y - 16)           (range is 0 to 255)
            a = 255/170 * (500 * (x - y)) + 127.5  (range is 0 to 255)
            b = 255/200 * (200 * (y - z)) + 95.625 (range is 0 to 255)

     Now our input is not scaled from 0-1 as these equations suppose but,
     rather, from 0-1024. We need to include that in our calculations.

            L = 2.55 * (116 * (y/1024) - 16)
            L = (13056*(116*y/1024-16))/5120  (multiply and divide by 5120)
            L = (1479*y-208896)/5120          (distribute)
            L = (1479*y-208896+2560)/5120     (prepare for roundoff)
            L = (1479*y-206336)/5120          (final equation for L)

            a = 1.5*(500*(x-y)/1024)+127.5
            a = (768*(500*(x-y)/1024)+65280)/512 (multiply and divide by 512)
            a = (375*(x-y)+65280)/512            (distribute)
            a = (375*(x-y)+65280+256)/512        (prepare for roundoff)
            a = (375*(x-y)+65536)/512            (final equation for a)

            b = 1.275*(200*(y-z)/1024)+95.625
            b = 255*(y-z)/1024+95.625             (distribute)
            b = 1024*(255*(y-z)/1024+95.625)/1024 (multiply and divide by 1024)
            b = (255*(y-z)+97920)/1024            (distribute)
            b = (255*(y-z)+97920+512)/1024        (prepare for division)
            b = (255*(y-z)+98432)/1024            (final equation for b) */
  r = (1479*  y   - 206336) /5120; /* This is really l but we reuse the var */
  g = ( 375*(x-y) +  65536) >>  9; /* This is really a*; we reuse the var */
  b = ( 255*(y-z) +  98432) >> 10; /* This is b*; variable name is good */
  *dst++ = MIN(255, MAX(0, r));
  *dst++ = MIN(255, MAX(0, g));
  *dst++ = MIN(255, MAX(0, b));
}

void rgb2labi(const unsigned char *src, /* Ptr to either rgb or rgbx data */
                    unsigned char *dst, /* Ptr to lab data results */
              int len,                  /* Should be number of pixels */
              int src_stride,           /* Bytes per pixel (e.g. 3 or 4) */
              const int rgb_map[3])     /* Default is {0,1,2} which works for
                                           RGB or RGBX input. For XBGR input
                                           you should use {3,2,1} */
{
  static int _init = 0;
  if (!_init)
  {
    _init_rgb_table();
    _compute_cubes();
    _init = 1;
  }
  while (len--)
  {
    _srgb2lab(src, dst, rgb_map);
    src += src_stride;
    dst += 3;
  }
}
