/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include "fax_phbook.h"
#include "cmd_proc_api.h"

#ifdef HAVE_CMD

/*----------------------------------------------------------------------------*/
static int _set(int argc, char* argv[])
{
  char *number, *name = "";
  int id, blacklist = 0;

  /* usage: <id> <number> [<name> = ""] [<blacklist> = 0] */
  if (argc < 3) return CMD_USAGE_ERROR;
                id        = cmd_atoi(argv[1]);
                number    = argv[2];
  if (argc > 3) name      = argv[3];
  if (argc > 4) blacklist = cmd_atoi(argv[4]);
  if (argc > 5) return CMD_USAGE_ERROR;

  if (fax_phbook_set(id, name, number, blacklist))
  {
    cmd_append_result("Error setting phbook %d to %s, %s, %d\n",
                      id, number, name, blacklist);
    return CMD_ERROR;
  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _grpadd(int argc, char* argv[])
{
  int gid, pid, arg;

  /* usage: <grpid> <id> [<id2>] [<id3>] ... */
  if (argc < 3) return CMD_USAGE_ERROR;
  gid = cmd_atoi(argv[1]);
  arg = 2;
  while (arg < argc)
  {
    pid = cmd_atoi(argv[arg++]);
    if (fax_phbook_group_add_entry(gid, pid))
    {
      cmd_append_result("Error adding phone book entry %d to group %d\n",
                        pid, gid);
      return CMD_ERROR;
    }
  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _grpname(int argc, char* argv[])
{
  int id;

  /* usage: <id> <name> */
  if (argc != 3) return CMD_USAGE_ERROR;
  id = cmd_atoi(argv[1]);

  if (fax_phbook_group_set_name(id, argv[2]))
  {
    cmd_append_result("Error setting group name %d to %s\n", id, argv[2]);
    return CMD_ERROR;
  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _clr(int argc, char* argv[])
{
  /* usage: <id> [<id>] ... */
  if (argc < 2) return CMD_USAGE_ERROR;
  int arg = 1;
  while (arg < argc)
  {
    int id = cmd_atoi(argv[arg++]);
    if (fax_phbook_erase(id))
    {
      cmd_append_result("Error clearing phbook %d\n", id);
      return CMD_ERROR;
    }
  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _grpclr(int argc, char* argv[])
{
  /* usage: <id> [<id>] ... */
  if (argc < 2) return CMD_USAGE_ERROR;
  int arg = 1;
  while (arg < argc)
  {
    int id = cmd_atoi(argv[arg++]);
    if (fax_phbook_group_erase(id))
    {
      cmd_append_result("Error clearing group %d\n", id);
      return CMD_ERROR;
    }
  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _dump(int argc, char* argv[])
{
  char name[FAX_PHBOOK_NAME_MAX_LEN];
  char number[FAX_PHBOOK_NUMBER_MAX_LEN];

  int blacklist;
  unsigned int pid, gid, c, count;

  if (argc != 1) return CMD_USAGE_ERROR;

  cmd_printf("--- Phone book entries ---\n");
  for (pid=0; pid<FAX_PHBOOK_NUM_ENTRIES; pid++)
  {
    if (!fax_phbook_get(pid, name, number, &blacklist) &&
        (*name || *number))
    {
      cmd_printf("%3d. %s; %s%s\n", pid, number, name,blacklist?" {BLKLST}":"");
    }
  }
  cmd_printf("--- Speed Dial Groups ---\n");
  for (gid=0; gid<FAX_PHBOOK_NUM_GROUPS; gid++)
  {
    if (!fax_phbook_group_get_entry_count(gid, &count) && count)
    {
      fax_phbook_group_get_name(gid, name);
      cmd_printf("%3d. %s\n", gid, name);
      for (c=0;c<count;c++)
      {
        int r;
        cmd_printf("  %3d:", c);
        if (!(r = fax_phbook_group_get_phbook_idx(gid, c, &pid)))
        {
          cmd_printf(" %3d.", pid);
          if (!fax_phbook_get(pid, name, number, &blacklist))
            cmd_printf(" %s; %s%s", number, name, blacklist?" {BLKLST}":"");
        }
        else
          cmd_printf(" Failed to get pid for gid %d which %d (%d)\n",gid,c,r);
        cmd_printf("\n");
      }
    }
  }
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static int _flush(int argc, char* argv[])
{
  if (argc != 1) return CMD_USAGE_ERROR;
  fax_phbook_commit();
  return CMD_OK;
}

/*----------------------------------------------------------------------------*/
void fax_phbook_cmd_init(void)
{
  cmd_register("fax", "Fax commands", NULL, NULL, NULL);
  cmd_register("fax phbook", "Fax phonebook commands",
               NULL, NULL, NULL);
  cmd_register("fax phbook set", "Set a phone book entry",
           "<id> <number> [<name> = ""] [<blacklist> = 0]",
           "This command sets the number, name, and blacklist status of a "
           "single, specified phone book entry. The only required inputs are "
           "the phone book id, <id>, and the phone number, <number>. If <name> "
           "is not provided then the name will be blank in the phone book "
           "entry. If <blacklist> is not set then the blacklist setting is set "
           "to 0.", _set);
  cmd_register("fax phbook group_add",
           "Add a set of phone book entries to a group",
           "<grpid> <id> [<id2>] [<id3>] ...",
           "This command adds one or more phone book entries to a single "
           "phone book group.", _grpadd);
  cmd_register("fax phbook group_name",
           "Set the name of a group list",
           "<grpid> <name>",
           "This command can be used to set the group name of a single phone "
           "book group.", _grpname);
  cmd_register("fax phbook clear",
           "Clear one or more phone book entries",
           "<id> [<id>] ...",
           "This clears all data from a set of phone book entries and removes "
           "those same entries from any groups to which they belonged.",
           _clr);
  cmd_register("fax phbook group_clear", "Clear phone book groups",
           "<id> [<id>] ...", "This command removes all entries from a set of "
           "phone book groups. It also sets the name of those groups to be "
           "blank.", _grpclr);
  cmd_register("fax phbook dump",
           "Dump the contents of the phone book", NULL, "Displays all of the "
           "details about the current phone book including all entries "
           "(numbers, names, and blacklist) and all group lists (namea) and "
           "the members of those group lists.", _dump);
  cmd_register("fax phbook commit", "Commit changes to the "
           "underlying NV storage device", NULL, NULL, _flush);
}

#else /* HAVE_CMD */

void fax_phbook_cmd_init(void) { ; }

#endif /* HAVE_CMD */
