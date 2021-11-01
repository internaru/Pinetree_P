/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "ppm.h"

/* Standard memory is not yet available - we map malloc/free to memAPI funcs */
#include "memAPI.h"
#define malloc(size) MEM_MALLOC(size)
#define free(ptr)    MEM_FREE_AND_NULL(ptr)

static void* _read_ppm_line(char* dst, uint32_t size, FILE* file)
{
  while (1)
  {
    if (!fgets(dst, size, file)) return NULL;
    if ((dst[0] == '#') || (dst[1] == '#')) continue;
    return dst;
  }
}

int ppm_readfile(const char* infile, ppm_img_t** img)
{
  char data[1024];
  int n, x, y;

  FILE* f;
  ppm_img_t* i;

  f = fopen(infile, "rb");
  if (!f) return -1;

  i = (ppm_img_t*)malloc(sizeof(ppm_img_t));
  if (!i)
    { fclose(f); return -2; }

  if (!_read_ppm_line(data, sizeof(data), f))
    { free(i); fclose(f); return -3; }

  if (strncmp(data, "P6", 2))
    { free(i); fclose(f); return -4; }

  if (!_read_ppm_line(data, sizeof(data), f))
    { free(i); fclose(f); return -5; }

  if (2 != sscanf(data, "%d %d", &x, &y))
    { free(i); fclose(f); return -6; }
  i->x = x;
  i->y = y;

  if (!_read_ppm_line(data, sizeof(data), f))
    { free(i); fclose(f); return -7; }

  if (1 != sscanf(data, "%d", &x))
    { free(i); fclose(f); return -8; }

  if (255 != x)
    { free(i); fclose(f); return -9; }
  n = 3 * i->x * i->y;

  if (!(i->d = (uint8_t*)malloc(n)))
    { free(i); fclose(f); return -10; }

  if (1 != fread(i->d, n, 1, f))
    { free(i->d); free(i); fclose(f); return -11; }

  fclose(f);
  *img = i;
  return 0;
}

int ppm_writefile(const char* filename, ppm_img_t* img)
{
  FILE* outfile = fopen(filename, "wb");
  if (!outfile) return -1;
  fprintf(outfile, "P6\n%lu %lu\n255\n", (long unsigned int)img->x, (long unsigned int)img->y);
  fwrite(img->d, 3 * img->x, img->y, outfile);
  fclose(outfile);
  return 0;
}

void ppm_free(ppm_img_t* i)
{
  if (!i) return;
  free(i->d);
  free(i);
}
