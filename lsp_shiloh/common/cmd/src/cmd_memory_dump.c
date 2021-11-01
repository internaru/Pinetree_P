/******************************************************************************
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \brief This file implements a simple hex dump of a memory location.  The
 * difference between this hex dump and others is that this hex dump outputs 
 * directly to the "cmd" context so it is useful to call in a command callback
 * or proc function.
 */

#include <stdint.h>
#include <ctype.h>
#include "cmd_proc_api.h"
#include "error_types.h"

error_type_t cmd_memory_dump( void* address, uint32_t numwords, cmd_dump_wordsize_t wordsize, cmd_dump_format_t dump )
{
  union
  {
    unsigned char  *addr8;
    unsigned short *addr16;
    unsigned int   *addr32;
    unsigned int    addr;
  } addr;
  char charfield[17];
  int w;
  uint32_t wordsize_int;

  addr.addr = (unsigned int)(uintptr_t)address;

  if ( ( dump != CMD_DUMP_FORMAT_ALL ) && ( dump != CMD_DUMP_FORMAT_HEX ) && ( dump != CMD_DUMP_FORMAT_ASCII ) )
  {
      return (error_type_t)CMD_USAGE_ERROR;
  }

  if ((wordsize != CMD_DUMP_WORDSIZE_1BYTE) && (wordsize != CMD_DUMP_WORDSIZE_2BYTES) && (wordsize != CMD_DUMP_WORDSIZE_4BYTES))
  {
      return (error_type_t)CMD_USAGE_ERROR;
  }
  if ((CMD_DUMP_WORDSIZE_2BYTES == 2) && (addr.addr & 1))
  {
      return (error_type_t)CMD_USAGE_ERROR;
  }
  if ((CMD_DUMP_WORDSIZE_4BYTES == 4) && (addr.addr & 3))
  {
      return (error_type_t)CMD_USAGE_ERROR;
  }

  if ( CMD_DUMP_WORDSIZE_1BYTE == wordsize )
  {
      wordsize_int = 1;
  }
  if ( CMD_DUMP_WORDSIZE_2BYTES == wordsize )
  {
      wordsize_int = 2;
  }
  if ( CMD_DUMP_WORDSIZE_4BYTES == wordsize )
  {
      wordsize_int = 4;
  }

  while (numwords)
  {
    cmd_printf("%8.8x: ", addr.addr);
    for (w=0;(w<(16/wordsize_int)) && numwords;w++)
    {
      if (wordsize == CMD_DUMP_WORDSIZE_1BYTE)
      {
        unsigned char c = *addr.addr8;
        if ( ( dump == CMD_DUMP_FORMAT_ALL ) || ( dump == CMD_DUMP_FORMAT_HEX ) )
        {
            cmd_printf("%2.2x ", c);
            if (!isprint(c))
            {
                c = '.';
            }
            else if (isspace(c) && (c != ' '))
            {
                c = '.';
            }
        }
        charfield[w] = c;
      }
      else if (wordsize == CMD_DUMP_WORDSIZE_2BYTES)
      {
        if ( ( dump == CMD_DUMP_FORMAT_ALL ) || ( dump == CMD_DUMP_FORMAT_HEX ) )
        {
            cmd_printf("%4.4x ", *addr.addr16);
        }
      }
      else if (wordsize == CMD_DUMP_WORDSIZE_4BYTES)
      {
          if ( ( dump == CMD_DUMP_FORMAT_ALL ) || ( dump == CMD_DUMP_FORMAT_HEX ) )
          {
              cmd_printf("%8.8x ", *addr.addr32);
          }
      }
      addr.addr += wordsize_int;
      numwords -= 1;
    }
    if ( ( wordsize == CMD_DUMP_WORDSIZE_1BYTE ) &&
         ( ( dump == CMD_DUMP_FORMAT_ALL ) || ( dump == CMD_DUMP_FORMAT_ASCII ) ) )
    {
      charfield[w] = 0;
      while (w++ < 16)
      {
          cmd_printf("   ");
      }
      cmd_printf("%s", charfield);
    }
    cmd_printf("\n");
  }

  return (error_type_t)CMD_OK;

}
