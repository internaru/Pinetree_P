/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "memAPI.h"
#include "cmd_sys_api.h"
#include "cmd_proc_api.h"
#include "cmd_cmd.h"
#include "cmd_var.h"
#include "cmd_cntxt.h"

/* -------------------------------------------------------------------------- */
void* cmd_malloc(unsigned int size)
{
  return MEM_MALLOC(size);
}

/* -------------------------------------------------------------------------- */
void  cmd_free(void * ptr)
{
  if (ptr) MEM_FREE_AND_NULL(ptr);
}

/* -------------------------------------------------------------------------- */
static int __echo(int argc, char **argv, int add_cr)
{
  int i;
  for (i=1; i<argc; i++)
  {
    if (i+1 != argc) cmd_printf("%s ",  argv[i]);
    else if (add_cr) cmd_printf("%s\n", argv[i]);
    else             cmd_printf("%s",   argv[i]);
  }
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
static int _puts(int argc, char **argv) { return __echo(argc, argv, 1); }
static int _echo(int argc, char **argv) { return __echo(argc, argv, 0); }

/* -------------------------------------------------------------------------- */
void cmd_init(void)
{
  cmd_cmd_init();
#ifdef HAVE_CMD_VARS
  cmd_var_init();
#endif
  cmd_cntxt_init();
  cmd_register("echo", "print arguments as output (no implicit CR)",
               "...", "Prints all arguments", _echo);
  cmd_register("puts", "print arguments as output (with implicit CR)",
               "...", "Prints all arguments followed by a CRLF", _puts);
  {
    extern void cmd_wrapper_init(void);
    cmd_wrapper_init();
  }
}
