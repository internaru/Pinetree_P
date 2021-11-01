/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __INC_PPM_H__
#define __INC_PPM_H__

#include <stdint.h>

typedef struct
{
  uint32_t x, y; /* x dim, y dim */
  uint8_t* d;
} ppm_img_t;

int  ppm_readfile(const char* infile, ppm_img_t** img);
int  ppm_writefile(const char* filename, ppm_img_t* img);
void ppm_free(ppm_img_t* i);

#endif /* __INC_PPM_H__ */
