/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include "tx_api.h"
#include "tcl.h"
#include "tclHash.h"
#include "strfmt.h"
#include "cmd_proc_api.h"
#include "cmd_var_api.h"
#include "cmd_private.h"

typedef struct __var_t
{
  struct __var_t *next;
  char           *name;
  char           *value;
} _var_t;

static _var_t *_vars = NULL;

/* -------------------------------------------------------------------------- */
static void _access(int get_nRel)
{
  static TX_SEMAPHORE _sem;
  static int _initted = 0;
  if (!_initted)
  {
    tx_semaphore_create(&_sem, "cmd_var", 1);
    _initted = 1;
  }
  if (get_nRel) tx_semaphore_get(&_sem, TX_WAIT_FOREVER);
  else          tx_semaphore_put(&_sem); 
}

/* -------------------------------------------------------------------------- */
static _var_t* _findvar(const char* name)
{
  _var_t* v;
  if (!name || !name[0]) return NULL;
  for (v=_vars; v; v=v->next)
    if (!strcmp(v->name, name))
      return v;
  return NULL;
}

/* -------------------------------------------------------------------------- */
int cmd_defvar(const char* name, const char* val)
{
  _var_t* var;
  if (!name || !*name || !val) return CMD_VAR_BAD_PARAM;

  _access(1);
  if (!(var = _findvar(name)))
  {
    if (!(var        = (_var_t*)cmd_malloc(sizeof(_var_t))) ||
        !(var->name  = (char*)cmd_malloc(strlen(name)+1)) ||
        !(var->value = (char*)cmd_malloc(strlen(val)+1)))
    {
      _access(0);
      if (var && var->name) cmd_free(var->name);
      if (var)              cmd_free(var);
      return CMD_VAR_MEMORY; /* Out of memory of some sort */
    }
    strcpy(var->name, name);
    strcpy(var->value, val);
    var->next = _vars;
    _vars = var;
  }
  _access(0);
  return CMD_VAR_OK;
}

/* -------------------------------------------------------------------------- */
int cmd_setvar(const char* name, const char* val)
{
  _var_t* var;
  if (!name || !*name || !val) return CMD_VAR_BAD_PARAM;

  _access(1);
  if (!(var = _findvar(name)))
  {
    _access(0);
    return cmd_defvar(name, val);
  }

  if (strlen(var->value) < strlen(val))
  {
    char* n = (char*)cmd_malloc(strlen(val)+1);
    if (!n) { _access(0); return CMD_VAR_MEMORY; }
    cmd_free(var->value);
    var->value = n;
  }
  strcpy(var->value, val);
  _access(0);
  return CMD_VAR_OK;
}

/* -------------------------------------------------------------------------- */
int cmd_getvar(const char* name, char* curval, int maxlen)
{
  _var_t* var;
  if (!name || !*name || !curval || !maxlen)
    return CMD_VAR_BAD_PARAM;

  _access(1);
  if (!(var = _findvar(name))) { _access(0); return CMD_VAR_NOTFOUND; }
  if ((strlen(var->value)+1) > maxlen) { _access(0); return CMD_VAR_MEMORY; }
  strcpy(curval, var->value);
  _access(0);
  return CMD_VAR_OK;
}

/* -------------------------------------------------------------------------- */
int cmd_incvar(const char* name, int val, int* newval)
{
  _var_t* var;

  if (!name || !*name) return CMD_VAR_BAD_PARAM;

  _access(1);
  if (!(var = _findvar(name)))
  {
    _access(0);
    return CMD_VAR_NOTFOUND;
  }

  if (val)
  {
    char valstr[15]; /* More than enough for any 32bit number and NULL */
    val += cmd_atoi(var->value);
    strfmt(valstr, "%d", val);

    if (strlen(var->value) < strlen(valstr))
    {
      char* n = (char*)cmd_malloc(strlen(valstr)+1);
      if (!n) { _access(0); return CMD_VAR_MEMORY; }
      cmd_free(var->value);
      var->value = n;
    }
    strcpy(var->value, valstr);
  }
  else
    val = cmd_atoi(var->value);
  _access(0);
  if (newval) *newval = val;
  return CMD_VAR_OK;
}

/* -------------------------------------------------------------------------- */
/* JSA - it is peculiar that this API has no way to report errors */
int cmd_getint(const char* name)
{
  int val;
  if (CMD_VAR_OK != cmd_incvar(name, 0, &val)) return 0;
  return val;
}

/* -------------------------------------------------------------------------- */
static int _cmd_defvar(int argc, char* argv[])
{
  if ((argc < 2) || (argc > 3))                 return CMD_USAGE_ERROR;
  if (cmd_defvar(argv[1], (argc>2)?argv[2]:"")) return CMD_ERROR;
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
static int _cmd_getvar(int argc, char* argv[])
{
  _var_t* var;
  if (argc != 2) return CMD_USAGE_ERROR;

  _access(1);
  if ((var = _findvar(argv[1])))
  {
    cmd_append_result("%s", var->value);
    _access(0);
    return CMD_OK;
  }
  else
  {
    _access(0);
    cmd_append_result("I cannot find a variable named '%s'", argv[1]);
    return CMD_ERROR;
  }
}

/* -------------------------------------------------------------------------- */
static int _cmd_setvar(int argc, char* argv[])
{
  if (argc != 3)                    return CMD_USAGE_ERROR;
  if (cmd_setvar(argv[1], argv[2])) return CMD_ERROR;
  else                              cmd_append_result("%s", argv[2]);
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
static int _cmd_incvar(int argc, char* argv[])
{
  int r, val = 1, new;
  if      (argc  < 2) return CMD_USAGE_ERROR;
  else if (argc == 3) val = cmd_atoi(argv[2]);
  else if (argc >  3) return CMD_USAGE_ERROR;
  if (*(argv[0]) == 'd') val *= -1; /* This was a decvar and not incvar */
  if ((r = cmd_incvar(argv[1], val, &new)))
  {
    cmd_append_result("Error adjusting variable by %d (%d)", val, r);
    return CMD_ERROR;
  }
  cmd_append_result("%d", new);
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
void cmd_var_init(void)
{
  _access(1);
  _access(0);

  cmd_register("defvar", "Define a shared variable",
               "<varname> [<initval>=\"\"]",
               "Shared variables are available to every CMD interpreter in "
               "the system as well as to C programs. A variable can be "
               "defined with a default value.", _cmd_defvar);
  cmd_register("getvar", "Get the value of a shared variable",
               "<varname>", NULL, _cmd_getvar);
  cmd_register("setvar", "Set a new value for a shared variable",
               "<varname> <value>", NULL, _cmd_setvar);
  cmd_register("incvar", "Increment a shared variable by 1 or more",
               "<varname> [<value>=1]", NULL, _cmd_incvar);
  cmd_register("decvar", "Decrement a shared variable by 1 or more",
               "<varname> [<value>=1]", NULL, _cmd_incvar);
}
