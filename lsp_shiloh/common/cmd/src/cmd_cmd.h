/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __INC_CMD_CMD_H__
#define __INC_CMD_CMD_H__

#include "cmd_proc_api.h"
#include "cmd_interp_api.h"

typedef struct __cmd_t
{
  struct __cmd_t *next;  /* Pointer to next in list */
  struct __cmd_t *sort;  /* Pointer to next in list (sorted order) */
  const char     *name;  /* Name */
  const char     *desc;  /* The short description */
  const char     *usage; /* A detailed description */
  const char     *notes; /* A detailed description */
  cmd_c_proc      proc;
  struct __cmd_t *subcmds;
} cmd_cmd_t;

void cmd_cmd_init(void);
int  cmd_cmd_get_new_cmds(void (*cb)(cmd_cmd_t*, struct cmd_interp_hdl_t*),
                          void* arg, int level);
cmd_cmd_t* cmd_cmd_find_cmd(char* names[], int num);

#endif
