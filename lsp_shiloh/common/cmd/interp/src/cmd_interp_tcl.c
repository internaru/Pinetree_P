/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "cmd_interp_api.h"
#include "tcl.h"

/* -------------------------------------------------------------------------- */
typedef struct cmd_interp_hdl_t
{
  Tcl_Interp *interp;
  Tcl_CmdBuf  cmdbuf;
} _hdl_t;

/* -------------------------------------------------------------------------- */
int  cmd_interp_hdl_size(void)
{
  return sizeof(_hdl_t);
}

/* -------------------------------------------------------------------------- */
extern char cmd_interp_tcl_procs[];
int cmd_interp_hdl_create(struct cmd_interp_hdl_t* hdl)
{
  hdl->interp = Tcl_CreateInterp();
  if (!hdl->interp)
  {
    return -1;
  }
  hdl->cmdbuf = Tcl_CreateCmdBuf();
  if (!hdl->cmdbuf)
  {
    Tcl_DeleteInterp(hdl->interp);
    return -2;
  }
  Tcl_Eval(hdl->interp, cmd_interp_tcl_procs, 0, NULL);
  return 0;
}

/* -------------------------------------------------------------------------- */
void cmd_interp_hdl_delete(struct cmd_interp_hdl_t* hdl)
{
  Tcl_DeleteInterp(hdl->interp);
  Tcl_DeleteCmdBuf(hdl->cmdbuf);
}

/* -------------------------------------------------------------------------- */
void cmd_interp_hdl_add_result(struct cmd_interp_hdl_t* hdl, const char* str)
{
  Tcl_AppendResult(hdl->interp, str, NULL);
}

/* -------------------------------------------------------------------------- */
static cmd_interp_cmd_hdlr_t _hdlr = NULL;
void cmd_interp_register_cmd_hdlr(cmd_interp_cmd_hdlr_t hdlr) { _hdlr = hdlr; }
static int _hook(ClientData d, Tcl_Interp* interp, int argc, char* argv[])
{
  return _hdlr?(_hdlr(d,argc,argv)?TCL_ERROR:TCL_OK):TCL_ERROR;
}

/* -------------------------------------------------------------------------- */
void cmd_interp_hdl_add_cmd(struct cmd_interp_hdl_t* hdl,
                            const char* name, void* data)
{
  Tcl_CreateCommand(hdl->interp, (char*)name, _hook, data, NULL);
}

/* -------------------------------------------------------------------------- */
int cmd_interp_hdl_parse_cmd(struct cmd_interp_hdl_t* hdl,
                             const char* data, int datalen, const char** cmd)
{
  int i;
  *cmd = NULL;
  for (i=0;i<datalen;i++)
  {
    char dat[2];
    dat[0] = *data++;
    dat[1] = 0;
    if ((*cmd = Tcl_AssembleCmd(hdl->cmdbuf, dat))) return i+1;
  }
  return i;
}

/* -------------------------------------------------------------------------- */
int cmd_interp_hdl_run(struct cmd_interp_hdl_t* hdl,
                       const char*  cmd,
                       const char** result)
{
  int r = (TCL_OK == Tcl_Eval(hdl->interp, (char*)cmd, 0, NULL))?0:-1;
  if (result)
  {
    if (hdl->interp->result && *(hdl->interp->result))
      *result = hdl->interp->result;
    else
      *result = NULL;
  }
  return r;
}
