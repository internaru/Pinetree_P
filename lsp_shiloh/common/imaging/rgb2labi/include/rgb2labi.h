/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_RGB2LABI_H__
#define __INC_RGB2LABI_H__

void rgb2labi(const unsigned char *src, /* Ptr to either rgb or rgbx data */
                    unsigned char *dst, /* Ptr to lab data results */
              int len,                  /* Should be number of pixels */
              int src_stride,           /* Bytes per pixel (e.g. 3 or 4) */
              const int rgb_map[3]);    /* Default is {0,1,2} which works for
                                           RGB or RGBX input. For XBGR input
                                           you should use {3,2,1} */

#endif
