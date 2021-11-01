/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <string.h>
#include <pthread.h>
#include <posix_ostools.h>
#include "strfmt.h"
#include "cmd_proc_api.h"
#include "cmd_cmd.h"
#include "cmd_private.h"
#include "lassert.h"

static cmd_cmd_t *_cmds = NULL;
static cmd_cmd_t *_end  = NULL;
static cmd_cmd_t *_sorted = NULL;
static int _cmdcnt = 0;/* Bump this up when command list changes to let
                          interpreters know that they need to register again */

static pthread_mutex_t _sem;

/* -------------------------------------------------------------------------- */
static void _access(int Lock_nUnlock)
{
  if (Lock_nUnlock) pthread_mutex_lock(&_sem);
  else              pthread_mutex_unlock(&_sem);
}

/* -------------------------------------------------------------------------- */
int cmd_cmd_get_new_cmds(void (*cb)(cmd_cmd_t*, struct cmd_interp_hdl_t*),
                         void* arg, int cmdlevel)
{
  int c;
  cmd_cmd_t* cmd;

  _access(1);
  for (cmd=_cmds,c=0; c<_cmdcnt; c++,cmd=cmd->next)
    if (c >= cmdlevel)
      cb(cmd, arg);
  cmdlevel = _cmdcnt;
  _access(0);
  return cmdlevel;

}

/* -------------------------------------------------------------------------- */
static cmd_cmd_t* _findproc(cmd_cmd_t* cmds, const char *name)
{
  if (!name || !*name) return NULL;

  for(; cmds; cmds=cmds->next)
    if (!strcmp(cmds->name, name))
      return cmds;
  return NULL;
}

/* -------------------------------------------------------------------------- */
cmd_cmd_t* cmd_cmd_find_cmd(char* names[], int num)
{
  int n = 0;
  cmd_cmd_t* cmd = NULL;

  _access(1);
  while (num--)
    if (!(cmd = _findproc(cmd?(cmd->subcmds):_cmds,names[n++])))
      break;
  _access(0);
  return cmd;
}

/* -------------------------------------------------------------------------- */
static cmd_cmd_t* _find_cmdpath(const char* cmdpath)
{
  char** names;
  int l, n, num;
  const char* p1;
  const char* p2 = NULL;
  cmd_cmd_t* cmd = NULL;

  if (!cmdpath || !*cmdpath) return NULL;

  /* The idea here is to parse the special cmdpath into a list of subcmd
     nodes. The rules are that there must be exactly one space in between
     node names and no special characters are handled (i.e. no backslash or
     any other form of escaping).
     For example a cmdpath of "toplevel sub1 subcmd2 specificcmd" would be
     broken up into names[] = {"toplevel", "sub1", "subcmd2", "specificcmd"}
     and num would be set to 4.
     With these two pieces of information cmd_cmd_find_cmd can be invoked. */
  num = 1;
  p1 = cmdpath;
  while (*p1)
  {
    p2 = strchr(p1, ' ');
    if (!p2) break;
    num += 1;
    p1 = p2+1;
  }
  if (num == 1) return cmd_cmd_find_cmd((char**)&cmdpath, 1);
  if (!(names = (char**)cmd_malloc(num * sizeof(char*)))) return NULL;
  memset(names, 0, num*sizeof(char*));

  for (n=0, p1=cmdpath; n<num; n++)
  {
    if (n+1 < num)
    {
      p2 = strchr(p1, ' ');
      ASSERT(p2);
      l = (unsigned int)(uintptr_t)p2 - (unsigned int)(uintptr_t)p1;
    }
    else
    {
      l = strlen(p1);
    }
    if (!(names[n] = (char*)cmd_malloc(l+1))) break;
    memcpy(names[n], p1, l);
    (names[n])[l] = 0;
    p1 = p2+1;
  }

  if (n == num) cmd = cmd_cmd_find_cmd(names, num);

  for (n=0; n<num; n++)
    if (names[n])
      cmd_free(names[n]);
  cmd_free(names);

  return cmd;
}

/* -------------------------------------------------------------------------- */
static cmd_cmd_t* _new_cmd(cmd_cmd_t* list,
                         const char* name,
                         const char* desc, const char* usage, const char* notes,
                         cmd_c_proc  proc)
{
  cmd_cmd_t *cmd;
  static const char *_accept = "abcdefghijklmnopqrstuvwxyz"
                               "0123456789"
                               "_";

  /* Check that name is specified and non-blank and [_a-z0-9]+ */
  if (! name) return NULL;
  if (!*name) return NULL;
  if (strspn(name, _accept) != strlen(name)) return NULL;

  /* Make sure that we can not find a command with this same name */
  if (name && (cmd = _findproc(list, name)))  return NULL;

  /* Allocate and populate a new argument struct */
  if (!(cmd = (cmd_cmd_t *)cmd_malloc(sizeof(cmd_cmd_t)))) return NULL;

  cmd->name    = name;
  cmd->desc    = desc;
  cmd->usage   = usage;
  cmd->notes   = notes;
  cmd->proc    = proc;
  cmd->subcmds = NULL; /* There can be none yet */
  cmd->next    = NULL;
  cmd->sort    = NULL;
  return cmd;
}

/* -------------------------------------------------------------------------- */
int cmd_register_subcmd(const char* cmdname, const char* subname,
                const char* desc, const char* usage, const char* notes,
                cmd_c_proc  proc)
{
  cmd_cmd_t *subcmd, *cmd = _find_cmdpath(cmdname);

  if (!cmd || cmd->proc) return -1;
  _access(1);
  if ((subcmd = _new_cmd(cmd->subcmds, subname, desc, usage, notes, proc)))
  {
    /* We add to the end of the list to keep the subcommands sorted in the
       order that they arrived */
    cmd_cmd_t* p = cmd->subcmds;
    if (p) while (p->next) p = p->next;
    if (p) p->next = subcmd;
    else   cmd->subcmds = subcmd;
  }
  _access(0);
  return subcmd?0:-1;
}

/* -------------------------------------------------------------------------- */
int cmd_register_cmd(const char* name, const char* ignore, const char* desc,
                     const char* usage, const char* notes,
                     cmd_c_proc  proc)
{
  cmd_cmd_t *cmd;

  _access(1);
  if ((cmd = _new_cmd(_cmds, name, desc, usage, notes, proc)))
  {
    /* Link the new command to the end of the list */
    if (_end)
    {
      _end->next = cmd;
      _end->sort = cmd;
      _end = cmd;
    }
    else
    {
      _cmds = _end = _sorted = cmd;
    }
    _cmdcnt++;
  }
  _access(0);
  return cmd?0:-1;
}

/* -------------------------------------------------------------------------- */
int cmd_register(const char* name,
                 const char* desc, const char* usage, const char* notes,
                 cmd_c_proc  proc)
{
  char *cmdstr, *subcmdstr;
  int i;

  if (!name || !*name) return -1;             /* Bad parameter */
  i = strlen(name);
  if (!(cmdstr = cmd_malloc(i+1))) return -2; /* Out of memory ? */
  memcpy(cmdstr, name, i+1);
  cmdstr[i] = 0;
  subcmdstr = strrchr(cmdstr, ' ');
  if (subcmdstr)
  {
    *subcmdstr = 0;
    subcmdstr += 1;
    i = cmd_register_subcmd(cmdstr, subcmdstr, desc, usage, notes, proc);
  }
  else
  {
    i = cmd_register_cmd(cmdstr, NULL, desc, usage, notes, proc);
  }
  if (i) cmd_free(cmdstr);
  return i;
}

/* -------------------------------------------------------------------------- */
static void __flow(int cols, const char* prefix, const char* str)
{
  if (!cols) cols = 80;
  while (str && *str)
  {
    int cpl = cols - strlen(prefix);
    int len = strlen(str);
    while (len)
    {
      char c = *str;
      if ((c != ' ') || (c != '\n') || (c != '\t')) break;
      len -= 1;
      str += 1;
    }
    if (cpl > len) cpl = len;
    while (cpl && (len > cpl))
    {
      char c = str[cpl];
      if ((c == ' ') || (c == '\n') || (c == '\t')) break;
      cpl--;
    }
    if (!cpl)
    {
      char*   q = strchr(str, ' ');
      if (!q) q = strchr(str, '\n');
      if (!q) q = strchr(str, '\t');
      if (!q) cpl = strlen(str);
      else    cpl = (int)(uintptr_t)q - (int)(uintptr_t)str;
    }
    cmd_printf("%s%.*s\n", prefix, cpl, str);
    str += cpl;
    if (*str) str++;
  }
}

/* -------------------------------------------------------------------------- */
static const char* _flow_usage = "<columns> <prefix> <string>";
static const char* _flow_notes = "Reflow the provided <string> to the "
  "specified number of <columns>. Each new line will be prefixed by <prefix>.";
static int _flow(int argc, char* argv[])
{
  if (argc != 4) return CMD_USAGE_ERROR;
  __flow(cmd_atoi(argv[1]), argv[2], argv[3]);
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
static cmd_cmd_t* _find_next(cmd_cmd_t* list, cmd_cmd_t* ref)
{
  cmd_cmd_t* min = NULL;
  while (list)
  {
    /* if (name < min (or no min)) && (name > ref (or no ref)) */
    if ((!min || (strcmp(list->name, min->name) < 0)) &&
        (!ref || (strcmp(list->name, ref->name) > 0)))
      min = list;
    list = list->next;
  }
  return min;
}

/* -------------------------------------------------------------------------- */
static void _sort(void)
{
  cmd_cmd_t *next, *ref = _find_next(_cmds, NULL);
  _sorted = ref;
  while ((next = _find_next(_cmds, ref)))
  {
    ref->sort = next;
    ref = next;
  }
  if (ref != NULL)
  {
      ref->sort = NULL;
  }
}

/* -------------------------------------------------------------------------- */
static void __cmdlist(int width)
{
  cmd_cmd_t *cmd;
  int l, c, cols, maxw = 1;
  if (width <= 0) width = 80;
  _access(1);
  _sort(); /* I know it is lazy but this hack way to get the list sorted */
  for (cmd=_sorted; cmd; cmd=cmd->sort)
  {
    if (*cmd->name == '_') continue;
    if ((l = strlen(cmd->name)) > maxw)
      maxw = l;
  }
  if (maxw > width) width = maxw;
  cols = width/maxw;
  for (c=0, cmd=_sorted; cmd; cmd=cmd->sort)
  {
    if (*cmd->name == '_') continue;
    cmd_printf("%*s", maxw, cmd->name);
    if (++c >= cols) { cmd_printf("\n"); c = 0; }
    else             { cmd_printf(" ");         }
  }
  if (c) cmd_printf("\n");
  _access(0);
}

/* -------------------------------------------------------------------------- */
static const char* _cmdlist_usage = "[<columns> = 80]";
static int _cmdlist(int argc, char* argv[])
{
  int width = 80;
  if (argc>1) width = cmd_atoi(argv[1]);
  if (argc>2) return CMD_USAGE_ERROR;
  __cmdlist(width);
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
static const char* _cmdsrch_usage = "[-v] [-c <columns>] "
                                    "[<string 1> [<string 2>] ...]";
static const char* _cmdsrch_notes = "All commands whose name contains at least "
  "one of the provided strings are displayed. You can use this to search for "
  "commands when you do not know the exact name but only a partial name.";
static void __srch(char* pre, cmd_cmd_t* cmd, char* names[], int numnames,
                   int verbose, int width)
{
  int n;

  /* Prune this search (i.e. hide all commands under this level) if this cmd
     starts with _ and does not match any of the provided names */
  if (*cmd->name == '_')
  {
    for (n=0;n<numnames;n++)
      if (!strcmp(cmd->name, names[n]))
        break;
    if (n == numnames)
      return;
  }

  for (n=0;n<numnames;n++)
  {
    if (strstr(cmd->name, names[n]))
    {
      if (pre && *pre)                           cmd_printf("%s ",pre);
                                                 cmd_printf("%s",cmd->name);
      if (cmd->proc && cmd->usage && *cmd->usage)cmd_printf(" %s",cmd->usage);
      if (cmd->desc && *cmd->desc)               cmd_printf(" -- %s",cmd->desc);
                                                 cmd_printf("\n");
      if (verbose && cmd->notes && *cmd->notes)  __flow(width, "  ",cmd->notes);
      break;
    }
  }

  if (cmd->subcmds)
  {
    int nl;
    char* newpre;

    if (pre) nl = strlen(pre)+strlen(cmd->name)+2;
    else     nl =             strlen(cmd->name)+1;

    if ((newpre = (char*)cmd_malloc(nl)))
    {
      cmd_cmd_t* sub = cmd->subcmds;

      if (pre) strfmt(newpre, "%s %s", pre, cmd->name);
      else     strfmt(newpre,    "%s",      cmd->name);
      while (sub)
      {
        __srch(newpre, sub, names, numnames, verbose, width);
        sub = sub->next;
      }
      cmd_free(newpre);
    }
  }
}

/* -------------------------------------------------------------------------- */
static int _cmdsrch(int argc, char* argv[])
{
  cmd_cmd_t *cmd;
  int arg, verbose=0, width=80;
  for (arg=1;arg<argc;arg++)
  {
    if      (!strcmp(argv[arg], "-v"))          verbose = 1;
    else if (!strcmp(argv[arg], "--verbose=1")) verbose = 1;
    else if (!strcmp(argv[arg], "--verbose=0")) verbose = 0;
    else if (!strcmp(argv[arg], "-c"))
    {
      if (arg+1 == argc) return CMD_USAGE_ERROR;
      width = cmd_atoi(argv[++arg]);
      if (width <= 0) width = 80; /* When <= 0 is requested then use default */
    }
    else
      break;
  }
  if (arg == argc) return CMD_USAGE_ERROR;
  _access(1);
  _sort(); /* I know it is lazy but this hack way to get the list sorted */
  for (cmd=_sorted; cmd; cmd=cmd->sort)
    __srch(NULL, cmd, argv+arg, argc-arg, verbose, width);
  _access(0);
  return CMD_OK;
}

/* -------------------------------------------------------------------------- */
static char* _make_precmds(int argc, char* argv[])
{
  int a, l = 0;
  char* s;
  for (a=0;a<argc;a++)
    l += (strlen(argv[a])+1);
  if (!l) return NULL;
  if ((s = cmd_malloc(l)))
  {
    l = 0;
    for (a=0;a<argc;a++)
    {
      strcpy(s+l, argv[a]);
      l += strlen(argv[a]);
      s[l++] = ' ';
    }
    s[--l] = 0;
  }
  return s;
}

/* -------------------------------------------------------------------------- */
static const char* _cmdhelp_usage = "[-v] [--verbose=0|1] [-c <columns>] <cmd>";
static const char* _cmdhelp_notes = "Gives help for a command. When verbose "
  "help is requested the notes for a specific command are provided formatted "
  "to the specified number of columns.";
static int _cmdhelp(int argc, char* argv[])
{
  cmd_cmd_t *cmd;
  int verbose = -1;
  int width = 80;
  int arg;
  char* pre;
  for (arg=1;arg<argc;arg++)
  {
    if      (!strcmp(argv[arg], "-v"))          verbose = 1;
    else if (!strcmp(argv[arg], "--verbose=1")) verbose = 1;
    else if (!strcmp(argv[arg], "--verbose=0")) verbose = 0;
    else if (!strcmp(argv[arg], "-c"))
    {
      if (arg+1 == argc) return CMD_USAGE_ERROR;
      width = cmd_atoi(argv[++arg]);
      if (width <= 0) width = 80; /* When <= 0 is requested then use default */
    }
    else
      break;
  }
  if (arg == argc)
  {
    cmd_printf("You should request help for one of the commands:\n");
    __cmdlist(width);
    return CMD_OK;
  }

  pre = _make_precmds(argc-arg-1, argv+arg);

  _access(1);

  if (!(cmd = cmd_cmd_find_cmd(argv+arg, argc-arg)))
  {
             cmd_printf("Command is not valid: \"");
    if (pre) cmd_printf("%s ", pre);
             cmd_printf("%s\"\n", argv[argc-1]);
    _access(0);
    cmd_free(pre);
    return CMD_ERROR;
  }

  if (verbose == -1)
  {
    if (cmd->proc) verbose = 1;
    else           verbose = 0;
  }

  const char* names[2];
  names[0] = "";        /* Match all subcommands */
  names[1] = cmd->name; /* If this is a hidden cmd we need to be explicit */
  __srch(pre, cmd, (char**)names, 2, verbose, width);
  _access(0);
  cmd_free(pre);
  return CMD_OK;
}

#define _TEST_CMDS (0)
#if _TEST_CMDS
/* -------------------------------------------------------------------------- */
static int _test(int argc, char* argv[])
{
  int a;
  for (a=0;a<argc;a++)
    cmd_printf("argv[%d] = %s\n", a, argv[a]);
  cmd_printf("I am returning CMD_USAGE_ERROR\n");
  return CMD_USAGE_ERROR;
}

/* -------------------------------------------------------------------------- */
static void _register_test_commands(void)
{
  cmd_register("tests",               "Test Command",       NULL, NULL, _test);
  cmd_register("_hiddentest",         "Test Command",       NULL, NULL, _test);
  cmd_register("testcmds",            "Test Commands",      NULL, NULL, NULL);
  cmd_register("testcmds one",        "Test Commands",      NULL, NULL, _test);
  cmd_register("testcmds two",        "More test Commands", NULL, NULL, NULL);
  cmd_register("testcmds _three",     "Test Commands",      NULL, NULL, _test);
  cmd_register("testcmds four",       "Test Commands",      NULL, NULL, _test);
  cmd_register("testcmds _five",      "More test Commands", NULL, NULL, NULL);
  cmd_register("testcmds six",        "More test Commands", NULL, NULL, NULL);
  cmd_register("testcmds two _acmd",  "a test command",     NULL, NULL, _test);
  cmd_register("testcmds two bcmd",   "b test command",     NULL, NULL, _test);
  cmd_register("testcmds two ccmd",   "c test command",     NULL, NULL, _test);
  cmd_register("testcmds _five acmd", "a test command",     NULL, NULL, _test);
  cmd_register("testcmds _five _bcmd","b test command",     NULL, NULL, _test);
  cmd_register("testcmds _five _ccmd","c test command",     NULL, NULL, _test);
  cmd_register("testcmds six _acmd",  "a test command",     NULL, NULL, _test);
  cmd_register("testcmds six _bcmd",  "b test command",     NULL, NULL, _test);
  cmd_register("testcmds six _ccmd",  "c test command",     NULL, NULL, _test);
  cmd_register("_hiddencmds",         "Test Commands",      NULL, NULL, NULL);
  cmd_register("_hiddencmds one",     "Test Commands",      NULL, NULL, _test);
}
#endif

/* -------------------------------------------------------------------------- */
void cmd_cmd_init(void)
{
  posix_mutex_init(&_sem);
  cmd_register("cmd", "Cmd system commands", NULL, NULL, NULL);
  cmd_register("flowtext", "Flow a text string for line breaks",
               _flow_usage, _flow_notes, _flow);
  cmd_register("cmd list", "List all top level commands",
               _cmdlist_usage, NULL, _cmdlist);
  cmd_register("list", "List all top level commands",
               _cmdlist_usage, NULL, _cmdlist);
  cmd_register("cmd search",
               "Search for commands that contain certain words",
               _cmdsrch_usage, _cmdsrch_notes, _cmdsrch);
  cmd_register("search",
               "Search for commands that contain certain words",
               _cmdsrch_usage, _cmdsrch_notes, _cmdsrch);
  cmd_register("cmd help", "Show help for commands",
               _cmdhelp_usage, _cmdhelp_notes, _cmdhelp);
  cmd_register("help", "Show help for commands",
               _cmdhelp_usage, _cmdhelp_notes, _cmdhelp);
#if _TEST_CMDS
  _register_test_commands();
#endif
}
