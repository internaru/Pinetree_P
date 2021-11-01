/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/* #include <stdlib.h> */
#include <stdio.h>
/* #include <string.h> */
/* #include <sys/types.h> */
/* #include <sys/stat.h> */
/* #include <fcntl.h> */

#include "cmd_proc_api.h"
//#include "timer_api.h"
#include "ppm.h"
#include "memAPI.h"
#include "xyscale.h"

static int _xyscale(int argc, char* argv[])
{
  uint8_t *mem, *inptr, *outptr;
  uint32_t memsize;
  int speed, r, outrow, inrow;
  xyscale_hdl_t hdl;
  ppm_img_t *in, *out;
  char *infilename, *outfilename;

  if (argc < 5) return CMD_USAGE_ERROR;
  infilename = argv[1];
  outfilename = argv[2];
  if (argc > 5) speed  = cmd_atoi(argv[5]);
  else          speed  = 0;

  if ((r = ppm_readfile(infilename, &in)))
  {
    cmd_printf("Failed to read %s (rc=%d).\n", infilename, r);
    return CMD_ERROR;
  }

  if (!(out = (ppm_img_t*)MEM_MALLOC(sizeof(ppm_img_t))))
  {
    ppm_free(in);
    cmd_printf("Failed to allocate output ppm memory\n");
    return CMD_ERROR;
  }
  out->x = cmd_atoi(argv[3]);
  out->y = cmd_atoi(argv[4]);
  if (!(out->d = (uint8_t*)MEM_MALLOC(3*out->x*out->y)))
  {
    ppm_free(in);
    MEM_FREE_AND_NULL(out);
    cmd_printf("Failed to allocate output ppm data memory\n");
    return CMD_ERROR;
  }
  memsize = xyscale_mem_size(3, in->x, in->y, out->x, out->y, speed);
  cmd_printf("XYScale requests %lu bytes of memory\n", (long unsigned int)memsize);
  if (!(mem = MEM_MALLOC(memsize)))
  {
    ppm_free(in);
    ppm_free(out);
    cmd_printf("Failed to allocate xyscale memory\n");
    return CMD_ERROR;
  }
  if ((r = xyscale_init(&hdl, 3, in->x, in->y, out->x, out->y,
                        mem, memsize, speed)))
  {
    ppm_free(in);
    ppm_free(out);
    MEM_FREE_AND_NULL(mem);
    cmd_printf("Scale library complains (%d)\n", r);
    return CMD_ERROR;
  }

  inptr = in->d;
  outptr = out->d;
  inrow = outrow = 0;
//  uint32_t t = timer_get_time_usec();
  while (1)
  {
    /* Read as much output as we can */
    if (XYSCALE_OKAY == (r = xyscale_outdata(hdl, outptr)))
    {
      outptr += 3*out->x;
      if (++outrow == out->y) break;
    }
    else if (r != XYSCALE_DATA_EMPTY)
    {
      cmd_printf("Error (%d) on xyscale_outdata\n", r);
      break;
    }

    /* Provide an input row if possible */
    if (XYSCALE_OKAY == (r = xyscale_indata(hdl, inptr)))
    {
      /* We want to replicate the last row */
      if (++inrow < in->y)
        inptr += 3*in->x;
    }
    else if (r != XYSCALE_DATA_FULL)
    {
      cmd_printf("Error (%d) on xyscale_indata\n", r);
      break;
    }
  }
//  t = timer_get_time_usec() - t;
  ppm_writefile(outfilename, out);
  ppm_free(in);
  ppm_free(out);
  MEM_FREE_AND_NULL(mem);
//  cmd_printf("XYSCale took %lu milliseconds\n", t/1000);
  return CMD_OK;
}

void xyscale_cmd_init(void)
{
  cmd_register("xyscale", "Scale a file and produce a new file",
           "<infile> <outfile> <outcols> <outrows> [<speed>=0]",
           "This utility works on ppm files. Its "
           "purpose, for now, is to measure the performance of the xyscale "
           "routine. Because of this it allocates full memory buffers for the "
           "operation. XYScale, of course, does not require this but it "
           "removes the file IO overhead from the measurement.",
           _xyscale);
}
