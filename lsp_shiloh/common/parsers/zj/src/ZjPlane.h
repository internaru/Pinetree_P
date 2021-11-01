/******************************************************************************
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/* ZjPlane.h

   SuperRIP low-level driver for ZjRCA
   (C) Zenographics, Inc. 2004-2005.  All rights reserved.

  [10-Aug-99,PMD] Split this out of Zj.h, prior to merge w/ZjRCA.h
 */

#ifndef __ZJPLANE_H__
#define __ZJPLANE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
/*  MONOCHROME = 0, */
	RED = 1, GREEN = 2, BLUE = 3, INTERLACE = 4,
	CYAN = 1, MAGENTA = 2, YELLOW = 3, BLACK = 4,
	CYAN2 = 5, MAGENTA2 = 6, YELLOW2 = 7, BLACK2 = 8 }
	PLANE;

#ifdef __cplusplus
}
#endif
#endif /* __ZJPLANE_H__*/
