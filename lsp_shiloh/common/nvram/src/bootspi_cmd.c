/*
 * ============================================================================
 * (C) Copyright 2009   Marvell International Ltd.
 *                           All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <string.h>
#include <stdio.h>
#include "cmd_proc_api.h"
#include "dbg_api.h"
#include "memAPI.h"
#include "code_table.h"
#include "memdevice.h"
#include "nvram_fwupdate.h"

/*----------------------------------------------------------------------------*/
static int _erase_sector(int num)
{
  MEMDEV_HANDLE*   hdl;
  if (OK != devOpen(MEM_SPI0, &hdl))
  {
    return -1;
  }
  if (OK != devErase(hdl, num))
  {
    devClose(hdl);
    return -2;
  }
  devClose(hdl);
  return 0;
}

/*----------------------------------------------------------------------------*/
static int _write_sector(int num, void* data)
{
  MEMDEV_HANDLE*   hdl;
  DRIVER_CONFIG_t  info;

  if (OK != devOpen(MEM_SPI0, &hdl))
  {
    return -1;
  }
  if (OK != devReport(hdl, &info))
  {
    devClose(hdl);
    return -2;
  }
  if (OK != devErase(hdl, num))
  {
    devClose(hdl);
    return -3;
  }
  if (info.BlockSize != devWrite(hdl, data, info.BlockSize*num, info.BlockSize))
  {
    devClose(hdl);
    return -4;
  }
  devClose(hdl);
  return 0;
}

/*----------------------------------------------------------------------------*/
static int _read_sector(int num, void* data)
{
  MEMDEV_HANDLE*   hdl;
  DRIVER_CONFIG_t  info;

  if (OK != devOpen(MEM_SPI0, &hdl))
  {
    return -1;
  }
  if (OK != devReport(hdl, &info))
  {
    devClose(hdl);
    return -2;
  }
  if (info.BlockSize != devRead(hdl, data, info.BlockSize*num, info.BlockSize))
  {
    devClose(hdl);
    return -3;
  }
  devClose(hdl);
  return 0;
}

/*----------------------------------------------------------------------------*/
static int _dev_report(DRIVER_CONFIG_t *info)
{
  MEMDEV_HANDLE*   hdl;

  if (OK != devOpen(MEM_SPI0, &hdl))
  {
    return -1;
  }
  if (OK != devReport(hdl, info))
  {
    devClose(hdl);
    return -2;
  }
  devClose(hdl);
  return 0;
}

/*------------------------------------------------------------------------------
   This code assumes that flash is layed out as follows (YYYYY specific):
    N   = number of flash blocks
    S   = size of flash blocks
    C   = block number of the code table (always 0 or 1)
    c   = block number of the config table
    u   = first block of usable code (follows zcode)
    U   = last block of usable code
    fw  = location of a valid FW image (code)
    fwe = last block used by this fw file
    fi  = location of a valid "files" image
    fie = location of a valid "files" image
   Blocks 0::u-1  : boot block, code table and zboot code
   Blocks u::U    : usable (possibly contains fw and/or fi)
   Blocks U+1:N-2 : flash storage area
   Block  N-1     : config table
    Variables u and U can be determined from the code table
    Variable N is known from the bspi_api
   The function returns -1 if any error occurs
   NULL pointers are acceptable (in which case the answer is not reported)
------------------------------------------------------------------------------*/
static int _flash_map(int *N, int *S,
                      int *C, int *c, int *u, int *U,
                      int *fw, int *fwe,
                      int *fi, int *fie)
{
  DRIVER_CONFIG_t info;
  uint32_t *data, entry;
  uint64_t offset, length;
  int a, localC, localu, localU;

  if (_dev_report(&info)) return -1;

  if (N) *N = info.BlockCount;
  if (S) *S = info.BlockSize;

  if (!(data = (uint32_t*)MEM_MALLOC(info.BlockSize))) return -2;

  /* Find codetable in one of two possible locations (one of firsttwo blocks) */
  for (localC=0;localC<2;localC++)
  {
    if (_read_sector(localC, data))
    {
      MEM_FREE_AND_NULL(data);
      return -3;
    }
    if (ct_verify_table((const code_table_header_t *)data)) break;
  }
  if (localC > 1)
  {
    MEM_FREE_AND_NULL(data);
    return -4;
  }
  if (C) *C = localC;

  /* If no config table is found then you've got to wonder how things work
     well enough to be even running this code */
  if (c) *c = -1;
  if ((entry = ct_find_entry(data, CT_CONFIG_DATA, 1)))
  {
    offset = ct_get_entry_offset(data, entry);
    length = ct_get_entry_length(data, entry);
    a = offset / info.BlockSize; /* Assume offset is a block boundary */
    if (c) *c = a+localC;
  }

  /* If no code is found than we are probably not set up as described above */
  localu = -1;
  if ((entry = ct_find_entry(data, CT_CODE, 1)))
  {
    offset = ct_get_entry_offset(data, entry);
    length = ct_get_entry_length(data, entry);
    a = (offset + length + info.BlockSize - 1) / info.BlockSize;
    localu = a+localC;
  }
  if (u) *u = localu;

  /* Find the CT_FILE_SYS for NVRAM storage to compute u */
  localU = -1;
  if ((entry = ct_find_entry(data, CT_FILE_SYS, 1)))
  {
    offset = ct_get_entry_offset(data, entry);
    length = ct_get_entry_length(data, entry);
    a = offset / info.BlockSize;
    localU = a-1+localC;
  }
  if (U) *U = localU;

  MEM_FREE_AND_NULL(data);

  if (fw || fwe)
  {
    if (fw)  *fw = -1;
    if (fwe) *fwe = -1;
    if ((localu >= 0) && (localU >= 0))
    {
      MEMDEV_HANDLE* hdl;
      unsigned long header[4], footer[4];
      devOpen(MEM_SPI0, &hdl);
      for (a=localu;a<=localU;a++)
      {
        devRead(hdl,(void*)header,a*info.BlockSize,16);
        if (header[0] != 0xe1ffc0de) continue;
        devRead(hdl,(void*)footer,a*info.BlockSize+16+((header[1]+3)&~3), 16);
        if (header[0] != footer[0]) continue;
        if (header[1] != footer[1]) continue;
        if (header[2] != footer[2]) continue;
        if (header[3] != footer[3]) continue;
        if (fw)  *fw  = a /* already has code table offset built in */;
        if (fwe) *fwe = a + (32+((header[1]+3)&~3))/info.BlockSize;
        break;
      }
      devClose(hdl);
    }
  }

  if (fi)  *fi  = -1; /* Not supported yet */
  if (fie) *fie = -1; /* Not supported yet */
    
  return 0;
}

/*----------------------------------------------------------------------------*/
error_type_t nvram_fwupdate(const char* filename)
{
  /* Note the calls to cmd_printf() will be no-op if this API is used outside
     of a cmd context. This is just fine. */
  int n, num;
  FILE* f;
  uint32_t filesize;
  unsigned char *data;

  int blocksize, fwcodeblock, lastusable;

  if (_flash_map(NULL, &blocksize, NULL, NULL, NULL, &lastusable,
                 &fwcodeblock, NULL,
                 NULL, NULL))
  {
    cmd_printf("Failed to map firmware image\n");
    return NVRAM_FWUPDATE_ERROR_FLASH_RD;
  }

  if (fwcodeblock == -1)
  {
    cmd_printf("No code found in flash\n");
    return NVRAM_FWUPDATE_ERROR_NO_FLASH_FW;
  }

  if (!(f = fopen(filename, "r")))
  {
    cmd_printf("Failed to open %s\n", filename);
    return NVRAM_FWUPDATE_ERROR_FILE_RD;
  }

  if (fseek(f, 0, SEEK_END))
  {
    fclose(f);
    cmd_printf("Cannot seek to end of file %s\n", filename);
    return NVRAM_FWUPDATE_ERROR_FILE_RD;
  }

  filesize = ftell(f);

  if (fseek(f, 0, SEEK_SET))
  {
    fclose(f);
    cmd_printf("Cannot seek to beginning of file %s\n", filename);
    return NVRAM_FWUPDATE_ERROR_FILE_RD;
  }

  num = (filesize+blocksize-1)/blocksize;
  if ((fwcodeblock+num-1) > lastusable)
  {
    fclose(f);
    cmd_printf("File is too large to fit into flash space\n");
    return NVRAM_FWUPDATE_ERROR_FILE_SIZE;
  }
  
  if (!(data = (unsigned char*)MEM_MALLOC(blocksize)))
  {
    fclose(f);
    cmd_printf("Failed to alloc %d bytes of storage\n", blocksize);
    return NVRAM_FWUPDATE_ERROR_MEMORY;
  }

  if (1 != fread(data, 16, 1, f))
  {
    fclose(f);
    MEM_FREE_AND_NULL(data);
    cmd_printf("Failed to read header\n");
    return NVRAM_FWUPDATE_ERROR_FILE_RD;
  }

  fseek(f, filesize-16, SEEK_SET);
  if (1 != fread(data+16, 16, 1, f))
  {
    fclose(f);
    MEM_FREE_AND_NULL(data);
    cmd_printf("Failed to read footer\n");
    return NVRAM_FWUPDATE_ERROR_FILE_RD;
  }
  fseek(f, 0, SEEK_SET);

  if (memcmp(data, data+16, 16) ||
      (data[1] != 0xc0) || (data[0] != 0xde) ||
      (data[3] != 0xe1) || (data[2] != 0xff))
  {
    fclose(f);
    MEM_FREE_AND_NULL(data);
    cmd_printf("Specified file is not a valid firmware file\n");
    return NVRAM_FWUPDATE_ERROR_BADFILE;
  }
 
  for (n=0;n<num;n++)
  {
    int l = fread(data, 1, blocksize, f);
    if (l > 0)
    {
      cmd_printf("Writing sector: %d (%d bytes; %d/%d blocks)\n",
                 fwcodeblock, l, n+1, num);
      cmd_output_sync();
      dbg_sync();
      _write_sector(fwcodeblock++, data);
    }
    if (l <= 0) break;
  }
  MEM_FREE_AND_NULL(data);
  fclose(f);
  return OK;
}

/*----------------------------------------------------------------------------*/
static const char* _burnfw_desc  = "Program new firmware into flash";
static const char* _burnfw_usage = "<path to fw>";
static const char* _burnfw_notes = "This command will update the firmware of "
 "the device. The <path to fw> file should contain an image produced by the "
 "makeimage tool.";
static int _burnfw(int argc, char* argv[])
{
  if (argc != 2) return CMD_USAGE_ERROR;
  if (OK != nvram_fwupdate(argv[1])) return CMD_ERROR;
  cmd_printf("Complete\n");
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static const char * _map_desc = "Map the flash image. What is where";
static int _map(int argc, char* argv[])
{
  int r, N, S, C, c, u, U, fw, fwe, fi, fie;
  if ((r = _flash_map(&N, &S, &C, &c, &u, &U, &fw, &fwe, &fi, &fie)))
  {
    cmd_printf("Could not map flash device (%d)\n", r);
    return CMD_ERROR;
  }
  cmd_printf("Flash Blocks:     %d\n", N);
  cmd_printf("Flash Block size: %d\n", S);
  cmd_printf("Code table:       %d\n", C);
  cmd_printf("Config table:     %d\n", c);
  cmd_printf("'Usable' blocks:  %d - %d\n", u, U);
  cmd_printf("Firmware:         %d - %d\n", fw, fwe);
  cmd_printf("Files:            %d - %d\n", fi, fie);
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static const char* _erase_desc = "Erase a single sector";
static const char* _erase_usage = "<sector number>";
static int _erase(int argc, char* argv[])
{
  if (argc != 2) return CMD_USAGE_ERROR;
  int sector = cmd_atoi(argv[1]);
  int r = _erase_sector(sector);
  if (r)
  {
    cmd_printf("Error erasing sector %d: %d\n", sector, r);
    return CMD_ERROR;
  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
void bootspi_cmd_init(void)
{
  cmd_register_cmd("bootspi", NULL, "BootSPI commands", NULL, NULL, NULL);
  cmd_register_subcmd("bootspi", "erase_sector",
                      _erase_desc, _erase_usage, NULL, _erase);
  cmd_register_cmd(   "burnfw", NULL,
                      _burnfw_desc, _burnfw_usage, _burnfw_notes, _burnfw);
  cmd_register_subcmd("bootspi", "fwupdate",
                      _burnfw_desc, _burnfw_usage, _burnfw_notes, _burnfw);
  cmd_register_subcmd("bootspi", "map", _map_desc, NULL, NULL, _map);
}
