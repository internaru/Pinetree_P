/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_LAB2RGBI_H__
#define __INC_LAB2RGBI_H__

void lab2rgbi(const unsigned char *src, /* Ptr to lab data */
                    unsigned char *dst, /* Ptr to rgb results */
              int len,                  /* Should be number of pixels */
              int dst_stride,           /* Bytes per pixel (e.g. 3 or 4) */
              const int rgb_map[3]);    /* Default is {0,1,2} which works for
                                           RGB or RGBX output. For XBGR output
                                           you should use {3,2,1} */

#endif
