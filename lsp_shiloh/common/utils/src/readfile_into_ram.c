/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
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
#include "readfile_into_ram.h"

error_type_t readfile_into_ram(const char* filepath,
                               void** results, uint64_t* results_size)
{
  struct stat st;

  if (!filepath || !results)
    return READFILE_INTO_RAM_USAGE_ERROR;

  int fd = open(filepath, O_RDONLY);

  if (fd < 0)
  {
    return READFILE_INTO_RAM_FAIL_TO_OPEN;
  }
  if (fstat(fd, &st))
  {
    close(fd);
    return READFILE_INTO_RAM_FAIL_TO_STAT;
  }

  if (results_size) *results_size = st.st_size;

  *results = MEM_MALLOC(st.st_size+1); /* We'll null terminate */
  if (!*results)
  {
    close(fd);
    return READFILE_INTO_RAM_FAIL_TO_ALLOC;
  }

  if (st.st_size != read(fd, *results, st.st_size))
  {
    MEM_FREE_AND_NULL(*results);
    close(fd);
    return READFILE_INTO_RAM_FAIL_TO_READ;
  }
  close(fd);
  ((char*)*results)[st.st_size] = 0; /* NULL terminate for convenience */
  return OK;
}
