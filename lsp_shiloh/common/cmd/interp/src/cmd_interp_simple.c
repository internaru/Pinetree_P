/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include "memAPI.h"
#include "utils.h"
#include "cmd_interp_api.h"

/* -------------------------------------------------------------------------- */
typedef struct __cmd_t
{
  char* name;
  void* data;
  struct __cmd_t* next;
} _cmd_t;

/* -------------------------------------------------------------------------- */
typedef struct cmd_interp_hdl_t
{
  struct { char* str; int len; } cmd, res;
  int fullcmd;
  _cmd_t* cmds;
} _hdl_t;

/* -------------------------------------------------------------------------- */
int  cmd_interp_hdl_size(void)
{
  return sizeof(_hdl_t);
}

/* -------------------------------------------------------------------------- */
int cmd_interp_hdl_create(struct cmd_interp_hdl_t* hdl)
{
  memset(hdl, 0, sizeof(*hdl));
  return 0;
}

/* -------------------------------------------------------------------------- */
void cmd_interp_hdl_delete(struct cmd_interp_hdl_t* hdl)
{
  while (hdl->cmds)
  {
    _cmd_t* n = hdl->cmds->next;
    MEM_FREE_AND_NULL(hdl->cmds->name);
    MEM_FREE_AND_NULL(hdl->cmds);
    hdl->cmds = n;
  }
  if (hdl->cmd.str) MEM_FREE_AND_NULL(hdl->cmd.str);
  if (hdl->res.str) MEM_FREE_AND_NULL(hdl->res.str);
}

/* -------------------------------------------------------------------------- */
void cmd_interp_hdl_add_result(struct cmd_interp_hdl_t* hdl, const char* str)
{
  if (!str || !*str) return;

  int add_len    = strlen(str);
  int alloc_len  = hdl->res.len;
  int cur_len    = alloc_len?strlen(hdl->res.str):0;
  int needed_len = (cur_len + add_len + 1);

  if (alloc_len < needed_len)
  {
    if (alloc_len)
    {
      hdl->res.str = MEM_REALLOC(hdl->res.str, needed_len);
    }
    else
    {
        hdl->res.str = MEM_MALLOC(needed_len); /// BUG: leaks on command not found.
      *(hdl->res.str) = 0;
    }
    hdl->res.len = needed_len;
  }
  strcat(hdl->res.str, str);
}

/* -------------------------------------------------------------------------- */
static cmd_interp_cmd_hdlr_t _hdlr = NULL;
void cmd_interp_register_cmd_hdlr(cmd_interp_cmd_hdlr_t hdlr) { _hdlr = hdlr; }

/* -------------------------------------------------------------------------- */
void cmd_interp_hdl_add_cmd(struct cmd_interp_hdl_t* hdl,
                            const char* name, void* data)
{
  _cmd_t* c = MEM_MALLOC(sizeof(*c));
  strcpy((c->name = MEM_MALLOC(strlen(name)+1)), name);
  c->data = data;
  c->next = hdl->cmds;
  hdl->cmds = c;
}

/* -------------------------------------------------------------------------- */
int cmd_interp_hdl_parse_cmd(struct cmd_interp_hdl_t* hdl,
                             const char* data, int datalen, const char** cmd)
{
  /* The idea is to add characters to our growing string until we see one of
     the command terminating characters (which are ;\n\r right now). Of course
     we should probably handle quoted and escaped versions of those chars but
     for demonstration only I am not going to go to that complexity. */
  int i, l;

  *cmd = NULL; /* Make sure we clear out any passed in garbage */

  /* Free any previously detected cmd */
  if (hdl->fullcmd)
  {
    hdl->fullcmd = 0;
    if (hdl->cmd.str) MEM_FREE_AND_NULL(hdl->cmd.str);
    hdl->cmd.len = 0;
  }

  /* Start or continue to build up a new command */
  l = hdl->cmd.str?strlen(hdl->cmd.str):0;
  for (i=0;i<datalen;i++)
  {
    /* If this is a termination point then handle it */
    if ((data[i] == ';') || (data[i] == '\n') || (data[i] == '\r'))
    {
      if (hdl->cmd.str)
      {
        *cmd = hdl->cmd.str;
        hdl->fullcmd = 1;
      }
      return i+1;
    }

    /* We must not have terminated so we continue to build this character into
       the string */
    if ((l == 0) || (l+1 >= hdl->cmd.len))
    {
      hdl->cmd.len += 256;
      if (hdl->cmd.str) hdl->cmd.str = MEM_REALLOC(hdl->cmd.str, hdl->cmd.len);
      else              hdl->cmd.str = MEM_MALLOC(               hdl->cmd.len);
      ASSERT(hdl->cmd.str);
    }
    hdl->cmd.str[l++] = data[i];
    hdl->cmd.str[l]   = 0;
  }
  return i;
}

/* -------------------------------------------------------------------------- */
static _cmd_t* _findcmd(_cmd_t* cmds, char* srch)
{
  while (cmds)
  {
    if (!strcmp(cmds->name, srch))
      return cmds;
    cmds = cmds->next;
  }
  return NULL;
}

/* -------------------------------------------------------------------------- */
int cmd_interp_hdl_run(struct cmd_interp_hdl_t* hdl,
                       const char*  cmd,
                       const char** result)
{
  int argc, r=-1;
  char *copy, *argv[20];
  _cmd_t* c;

  if (result) *result = NULL;
 
  
  if (hdl->res.str) MEM_FREE_AND_NULL(hdl->res.str);
  hdl->res.len = 0;

  if (_hdlr)
  {
    copy = MEM_MALLOC(strlen(cmd)+1);
    strcpy(copy, cmd);
    argc = tokenize_escaped_str(copy, argv, 20);
    if (argc > 0)
    {
      if ((c = _findcmd(hdl->cmds, argv[0])))
        r = _hdlr(c->data, argc, argv);
      else if (argv[0] && *argv[0])
      {
        cmd_interp_hdl_add_result(hdl, "Command not found: ");
        cmd_interp_hdl_add_result(hdl, argv[0]);
      }
    }
    MEM_FREE_AND_NULL(copy);
  }

  if (result && hdl->res.len) *result = hdl->res.str;

  return r;
}
