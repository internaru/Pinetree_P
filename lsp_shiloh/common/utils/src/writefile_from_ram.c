/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "memAPI.h"
#include "writefile_from_ram.h"

error_type_t writefile_from_ram(const char* filepath,
                                void* source, uint64_t source_size)
{
  if (!filepath || !source_size)
    return WRITEFILE_FROM_RAM_USAGE_ERROR;

  int fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND);

  if (fd < 0)
  {
    return WRITEFILE_FROM_RAM_FAIL_TO_OPEN;
  }

  if (source_size != write(fd, source, source_size))
  {
    close(fd);
    return WRITEFILE_FROM_RAM_FAIL_TO_WRITE;
  }

  close(fd);
  return OK;
}
