/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <math.h>
#include "lab2rgbi.h"

static unsigned int _rgbtbl[256];
static void _init_rgb_table(void)
{
  /* What we need to do is pre-compute the answers for the following set of
     equations. Note that R, G, and B are all treated the same. Note that the
     range of inputs is going to be 28 bits of precision (i.e. 0xfffffff = 1.0)

     if ( v > 0.0031308 ) v = 1.055 * ( v ^ ( 1 / 2.4 ) ) - 0.055
     else                 v = 12.92 * v
     RGB = v * 255

     if ( v > 0.0031308 ) RGB = 269.025 * v^(1/2.4) - 14.025
     else                 RGB = 3294.60 * v

     if (RGB < 10.3) v = RGB/3294.60
     else            v = ((RGB + 14.025)/269.025)^2.4


     .0031308 * 12.92 = 10.31473368
     The first 10 answers (0.5 - 9.5) will use the first form
  */ 
  int i;
  for (i=0;i<256;i++)
  {
    double v = i;
           v += 0.5;
    if (i<10) v = (v/3294.6);
    else      v = pow((v+14.025)/269.025,2.4);
    _rgbtbl[i] = v * 268435455.0 /*0xfffffff.0 */ + 0.5;
  }
}

static inline int _rgblookup(signed int x)
{
  int ans = 127, div = 64;

  /* x comes in as 0 - 2^30 (+/- some overflow). The code below finds the
     correct rgb value by binary searching the pre-computed table */
  if (x < 0)         return   0;
  if (x > 0xfffffff) return 255;

  while (div)
  {
    if      (_rgbtbl[ans] > x) ans -= div;
    else if (_rgbtbl[ans] < x) ans += div;
    else                       break;
    div >>= 1;
  }
  if (_rgbtbl[ans] >= x) return ans;
  else                   return ans+1;
}

static inline void _doit(const unsigned char *src, unsigned char *dst,
                         const int rgb_map[3])
{
  unsigned int l, ll, lll, a, aa, b, bb, x, y, z;

  l = src[0];
  a = src[1];
  b = src[2];

  /* The first thing we need to do is to convert these fax LAB values back
     up to the full range of L*, a*, b* values. Consider the formulae:

     These are the equations that convert x, y, and z (scaled 0-1023)
     int L, a and b (scaled and offset for fax to 0-255 range)

     L = 255/100 * (116 * (y/1024) - 16)
     a = 3/2*(500*(x-y)/1024)+255/2
     b = 1275/1000*(200*(y-z)/1024)+765/8

     We need to reverse these to get back to 10 bit x, y, and z. Then we need
     to decide if we need to cube or linearly scale before we matrix multiply.
     These vars, at this form, are sometimes known as X_var, Y_var, and Z_var
     because they are not yet X, Y, and Z

     L = 255/100 * (116 * (y/1024) - 16)
     L = 255*116*y/102400 - 255*16/100
     L = 29580*y/102400 - 4080/100
     100L = 29580*y/1024 - 4080
     1024*(100L + 4080)/29580 = y
     Y = 512*(10L + 408)/1479
     Y = 5120/1479*L + 512*408/1479
     Y = 5120/1479*L + 12288/87
     Y = 5120L/1479 + 17*12288/(87*17)
     Y = (5120L + 17*12288)/1479
     Y = (5120L + 208896)/1479
     Y = 1024*(5L+204)/1479

     a = 3/2*(500*(x-y)/1024)+255/2
     2a = 1500*(x-y)/1024+255
     1024*(2a-255)/1500 = x-y
     X = Y + 1024*(2a-255)/1500
     X = Y + 2048A/1500-1024*255/1500
     X = Y + 2048A/1500-1024*255/1500
     X = Y + 512A/375-256*17/25
     X = Y + 512A/375-4352/25
     X = Y + 512A/375-15*4352/(15*25)
     X = Y + (512A+15*4352)/375
     X = Y + (512A+65280)/375
     X = (5120L + 208896)/1479 + (512A+65280)/375
     X = 379*(5120L + 208896)/(375*1479) + 1479*(512A+65280)/(375*1479)
     X = (1940480L + 79171584 + 757248A + 96549120)/(375*1479)
     X = (1940480L + 757248A + 175720704)/554625
     X = 256*(7580L+2958A+686409)/554625

     b = 1275/1000*(200*(y-z)/1024)+765/8
     b = 1275/5*((y-z)/1024)+765/8
     8b = 1275/5*((y-z)/128)+765
     8b-765 = 1275/5*((y-z)/128)
     128*(8b-765) = 1275/5*(y-z)
     5*128*(8b-765)/1275 = y-z
     z = y - 5*128*(8b-765)/1275
     z = y - 5120b/1275+489600/1275
     z = y - 5120b/1275+489600/1275
     Z = Y - 1024b/255+384
     Z = Y - 1024b/255+255*384/255
     Z = Y - (1024b - 255*384)/255
     Z = Y - (1024b - 97920)/255
     Z = (5120L + 208896)/1479 - (1024b - 97920)/255
     Z = 5*(5120L + 208896)/(5*1479) - 29*(1024b - 97920)/(29*255)
     Z = (25600L + 1044480 - 29696B + 2839680)/7395
     Z = (25600L - 29696B + 3884160)/7395
     Z = 128*(200L-232B+30345)/7395

     So we have the final three equations shown here:

     Y = 1024*(   5L      +   204)/  1479
     X =  256*(7580L+2958A+686409)/554625
     Z =  128*( 200L- 232B+ 30345)/  7395

     (with L, A, and B all in the range of 0-255 the sub-terms of these
      equations will all fit inside of an unsigned 32 bit number. Also, Z has
      the potential to be negative if B > 25/29L + 130.8. I am pretty sure that
      this is outside of the gamut of the input colorspace(s) that we expect to
      process but we may need to handle this case).

     Once we have X, Y, Z as described above we need to decide if we will get
     back to true X, Y, and Z by either cubing the results or by using the
     linear formula.

     if (Y < 6/29) we use the linear equations, otherwise we cube.
     Y < 6/29 in the range 0-1 is the same as Y < 211.66 in the 0-1023 range.

     The forward linear equation is:
       yvar = (24389/27 * y + 16)/116
     We need to reverse this since we are going from yvar to y
       y = 27*(yvar * 116 - 16)/24389
       y = (27*yvar*116 - 27*16)/24389
       y = (3132*yvar - 432)/24389

     Plug this equation into the above equations to get the 3 linear conversions
      Y =       1024*(5L+204)/1479
      Y = (3132*1024*(5L+204)/1479 - 432)/24389
      Y = (36*1024*(5L+204)/17 - 17*432/17)/24389
      Y = ((184320L+7520256)/17 - 7344/17)/24389
      Y = (184320L+7520256-7344)/(17*24389)
      Y = (184320L+7512912)/(17*24389)
      Y = (184320L+7512912)/414613

      X =        256*(7580L+2958A+686409)/554625
      X = (3132*(256*(7580L+2958A+686409)/554625)-432)/24389
      X = (12*(256*(7580L+2958A+686409)/2125)-432)/24389
      X = (12*(256*(7580L+2958A+686409)/2125)-432)/24389
      X = (3072*(7580L+2958A+686409)/2125)-432)/24389
      X = ((2328570/2125L+9086976/2125A+2108648448/2125)-432)/24389
      X = ((465714/425L+534528/125A+124038144/125)-432)/24389
      X = (5*465714L/(5*425)+17*534528A/(17*125)+17*123984144/(17*125))/24389
      X = (2328570L/2125+9086976A/2125+2107730448/2125)/24389
      X = (2328570L+9086976A+2107730448)/2125*24389
      X = (2328570L+9086976A+2107730448)/51826625  (everything fits in 2^32)

      Z =        128*(200L-232B+30345)/7395
      Z = (3132*(128*(200L-232B+30345)/7395)-432)/24389
      Z = (36*(128*(200L-232B+30345)/85)-432)/24389
      Z = (4608*(200L-232B+30345)/85-432)/24389
      Z = (921600L-1069056B+139829760)/85-(85*432)/85)/24389
      Z = (921600L-1069056B+139793040)/(85*24389)
      Z = (921600L-1069056B+139793040)/2073065  (everything fits in 2^32)
     

    So we use these equations when the corresponding 'var' is less than 212.

      Y = (184320L+7512912)/414613
      X = (2328570L+9086976A+2107730448)/51826625
      Z = (921600L-1069056B+139793040)/2073065
 
    And we use the cube equations (see below) in the other condition.
       
    The cube of the above Y = 5120L/1479 + 12288/87 is shown below:
        Y = LLL134217728000/3235225239
            +LL107374182400/21145263
              +L85899345920/414613
               +68719476736/24389

     a = 3/2*(500*(x-y)/1024)+255/2
     2a = 1500*(x-y)/1024+255
     1024*(2a-255)/1500 = x-y
     x = y + 1024*(2a-255)/1500
     x = 5120/1479*L + 12288/87 + 1024*(2a-255)/1500
     x = 5120/1479*L + 12288/87 + 2048a/1500-1024*255/1500
     x = 5120L/1479 + 12288/87 + 512A/375 - 4352/25
     x = 5120L/1479 - 71424/87*25 + 512A/375
     x = 5120L/1479 + 512A/375 - 23808/725
     x^3 = AAA134217728/52734375
          +AAL268435456/13865625
          -AA2080374784/11328125
          +ALL536870912/10937205
          -AL8321499136/8935625
          +A290212282368/65703125
          +LLL134217728000/3235225239
          -LL8321499136/7048421
          +L580424564736/51826625
          -13494871130112/381078125

     b = 1275/1000*(200*(y-z)/1024)+765/8
     b = 1275/5*((y-z)/1024)+765/8
     8b = 1275/5*((y-z)/128)+765
     8b-765 = 1275/5*((y-z)/128)
     128*(8b-765) = 1275/5*(y-z)
     5*128*(8b-765)/1275 = y-z
     z = y - 5*128*(8b-765)/1275
     z = y - 5120b/1275+489600/1275
     z = y - 5120b/1275+489600/1275
     z = y - 1024b/255+384
     z = 5120/1479*L + 12288/87 - 1024b/255 + 384
     z = 5120L/1479 - 1024B/255 + 15232/29
     z^3 = -BBB1073741824/16581375
           +BBL1073741824/6411465
           +BB939524096/36975
           -BLL5368709120/37186497
           -BL1879048192/42891
           -B13975420928/4205
           +LLL134217728000/3235225239
           +LL23488102400/1243839
           +L69877104640/24389
           +3534034567168/24389
  */

  ll  = l*l;
  lll = l*l*l;
  aa  = a*a;
  bb  = b*b;

  y = (1024*(5*l+204))/1479;
  if (y < 212)
    y = (184320*l+7512912+414613/2)/414613;
  else
    y = (83*lll>>1)-(223*lll>>14)-(221*lll>23)    /* 134217728000/3235225239 */
      + (40623*ll>>3)+(58517*ll>>20)+(807*ll>>31) /* 107374182400/21145263 */
      + (13259493*l>>6)-(2616371*l>>31)           /* 85899345920/414613 */
      + 2817642;                                  /* 68719476736/24389 */

  x = (256*(7580*l+2958*a+686409))/554625;
  if (x < 212)
    x = (2328570*l+9086976*a+2107730448)/51826625;
  else
    x = (83*lll>>1)-(223* lll>>14)-(221*lll>23)                          /* 134217728000/3235225239 */
      + (163*aa*a>>6)-(7*aa*a>>12)-(113*aa*a>>29)                        /* 134217728/52734375 */
      + (155*aa*l>>3)-(249*aa*l>>14)-(183*aa*l>>23)+(9*aa*l>>28)         /* 268435456/13865625 */
      + (49*a*ll)+(177*a*ll>>11)+(125*a*ll>>19)+(141*a*ll>29)+(a*ll>>30) /* 536870912/10937205 */
      - (23507*aa>>7)+(52345*aa>>25)+(29*aa>>31)                         /*-2080374784/11328125 */
      - (59601*a*l>>6)-(13785*a*l>>21)+(5*a*l>>30)                       /*-8321499136/8935625*/
      - (9445*ll>>3)+(51065*ll>>23)+(125*ll>>31)                         /*-8321499136/7048421*/
      + (9046065*a>>11)+(236781*a>>31)                                   /* 290212282368/65703125 */
      + (11468135*l>>10)+(898127*l>>31)                                  /* 580424564736/51826625 */
      - 35412l;                                                          /*-13494871130112/381078125 */

  z = (128*(200*l-232*b+30345))/7395;
  if (z < 212)
    z = (921600*l-1069056*b+139793040)/2073065;
  else
    z = (83*lll>>1)-(223* lll>>14)-(221*lll>23)                           /* 134217728000/3235225239 */
      - (65*bb*b)+(125*bb*b>>9)-(161*bb*b>>22)+(29*bb*b>>31)              /*-1073741824/16581375 */
      + (167*bb*l)+(121*bb*l>>8)-(133*bb*l>>18)+(241*bb*l>>29)+(bb*l>>30) /* 1073741824/6411465 */
      - (144*b*ll)-(191*b*ll>>9)+(33*b*ll>>16)+(107*b*ll>27)-(b*ll>>30)   /*-5368709120/37186497 */
      + (50819*bb>>1)+(27769*bb>>17)-(2775*bb>>31)                        /* 939524096/36975 */
      - (43810*b*l)+(39837*b*l>>18)+(1113*b*l>>31)                        /*-1879048192/42891 */
      + (37767*ll>>1)+(14467*ll>>18)-(903*ll>>31)                         /* 23488102400/1243839 */
      - (6647049*b>>1)-(6471497*b>>26)+(b>>29)                            /*-13975420928/4205 */
      + (5739215*l>>1)-(5991617*l>>26)-(3*l>>30)                          /* 69877104640/24389 */
      + 144902807;                                                        /* 3534034567168/24389 */

  /* JSA - it is here where we would apply any change in white point */

  /* Now, to get to RGB we multiply by the inverse matrix used for RGB to XYZ.
     Namely, the following matrix is used:

     http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
     r, g, b = M * x, y, z
     M = 3.1338561 -1.6168667 -0.4906146
        -0.9787684  1.9161415  0.0334540
         0.0719453 -0.2289914  1.4052427

     Our problem, however, is that our numbers are now two big for some
     of these coefficients. To solve this problem we are going to scale
     the maxtrix by 1/4 (2 bits) to get the new matrix shown here:

     M =  0.783464025 -0.404216675 -0.122653650  xr yr zr
         -0.244692100  0.479035375  0.008363500  xg yg zg
          0.017986325 -0.057247850  0.351310675  xb yb zb

     And we compute RGB as linear combinations of the above. Note,
     however, that we must be aware of potential negative numbers
     due to the equations of the previous step. */

#define _T(V, M, S) ((signed int)((V*M)>>S))

  signed int rev, xr, yr, zr, xg, yg, zg, xb, yb, zb;

  /* We know that y will always be in the range from 0 - 0x3fffffff */
  yr =-_T(y,3,3) -_T(y,1,5) +_T(y,1, 9) +_T(y,3,15) -_T(y,3,18) +_T(y,3,25) +_T(y,1,29);
  yg = _T(y,1,1) -_T(y,3,7) +_T(y,3,10) -_T(y,1,11) +_T(y,1,15) +_T(y,1,20) -_T(y,3,30) +_T(y,1,31);
  yb =-_T(y,1,4) +_T(y,3,9) -_T(y,1,11) -_T(y,1,13) +_T(y,3,20) +_T(y,1,22) +_T(y,1,25) -_T(y,3,30);

  /* X can be negative. We need to detect this and handle it */
  rev = 0;
  if (x > 0x7fffffff)
  {
    x = 0xffffffff - x + 1;
    rev = 1;
  }
  xr= _T(x,3,2) +_T(x,1,5) +_T(x,1,9)  +_T(x,1,12) +_T(x,1,16) +_T(x,3,21) +_T(x,1,24) +_T(x,3,28) -_T(x,1,30);
  xg=-_T(x,1,2) +_T(x,3,9) -_T(x,1,11) -_T(x,1,14) -_T(x,1,19) -_T(x,1,22) -_T(x,3,28) -_T(x,1,29);
  xb= _T(x,1,6) +_T(x,1,9) +_T(x,3,13) +_T(x,3,16) -_T(x,1,18) +_T(x,1,25) -_T(x,3,30) +_T(x,1,31);
  if (rev)
  {
    xr = -xr;
    xg = -xg;
    xb = -xb;
  }

  /* Z can be negative. We need to detect this and handle it */
  rev = 0;
  if (z > 0x7fffffff)
  {
    z = 0xffffffff - z + 1;
    rev = 1;
  }
  zr=-_T(z,1,3) +_T(z,1,9)  +_T(z,3,13) +_T(z,1,15) -_T(z,1,18) +_T(z,3,23) -_T(z,3,26) -_T(z,1,29);
  zg= _T(z,1,7) +_T(z,1,11) +_T(z,1,14) +_T(z,1,19) -_T(z,1,22) +_T(z,1,26) -_T(z,1,31);
  zb= _T(z,3,3) -_T(z,3, 7) -_T(z,1,12) -_T(z,1,17) -_T(z,1,24) +_T(z,1,28) +_T(z,1,30);
  if (rev)
  {
    zr = -zr;
    zg = -zg;
    zb = -zb;
  }

#undef _T

  dst[rgb_map[0]] = _rgblookup(xr+yr+zr);
  dst[rgb_map[1]] = _rgblookup(xg+yg+zg);
  dst[rgb_map[2]] = _rgblookup(xb+yb+zb);
}

void lab2rgbi(const unsigned char *src, /* Ptr to lab data */
                    unsigned char *dst, /* Ptr to rgb results */
              int len,                  /* Should be number of pixels */
              int dst_stride,           /* Bytes per pixel (e.g. 3 or 4) */
              const int rgb_map[3])     /* Default is {0,1,2} which works for
                                           RGB or RGBX output. For XBGR input
                                           you should use {3,2,1} */
{
  static int _init = 0;
  if (!_init)
  {
    _init_rgb_table();
    _init = 1;
  }
  while (len--)
  {
    _doit(src, dst, rgb_map);
    src += 3;
    dst += dst_stride;
  }
}
