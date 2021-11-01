/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// public includes
#include <string.h>
#include <stdio.h>
#include "os_network.h"

// common includes
#include "error_types.h"
#include "net_conf_api.h" // file_purpose_t, net_config_file_t
#include "os_system.h" // os_system

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "net.conf.cmd: "
#define DBG_ON
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
//#define DBG_RELEASE_ON

#include "net_debug.h" // DBG_ family defaults


#ifdef DBG_VERBOSE_ON 

#define CMD_BUCKET "conf"
// return number of configuration files
int list_conf_files_cb(int argc, char* argv[])
{
    net_config_file_t* file = NULL;
    char qualified_file[256];
    int of_interest = 0; // first conf file ..

#if 0 // list individual files?
    if (argc > 1) // show requested file(s)
    {
        int next = 1;
        while (argv[next])
        {
            sscanf(argv[next],"%d", &of_interest); // by number
            if (of_interest < 0 || of_interest > last_conf_file)
            {
                DBG_ERR("%s() bad file# %s\n", __func__, argv[next]);
            }
            else
            {
                file = &net_conf_files[of_interest];
                int file_type = file->purp;
                lookup_fs_name(of_interest, qualified_file, 256);
                DBG_VERBOSE("%2d: %16s: %s\n", file_type, get_purpose_str(file_type), qualified_file);
            }
            next++;
        }
    }
    else
#endif
    {
        while (of_interest <= last_conf_file)
        {
            file = &net_conf_files[of_interest];
            int file_type = file->purp;
            UNUSED_VAR(file_type);
            lookup_fs_name(of_interest, qualified_file, 256);
            DBG_VERBOSE("%2d: %16s: %s\n", file_type, get_purpose_str(file_type), qualified_file);
        
            of_interest++;
        }
    }
    return of_interest;
}

static char heap_str[32]; // local working string buffer

#define returnAsLiteral(intf) { case intf: return #intf; }
char* get_purpose_str(int index)
{
    switch (index)
    {
        returnAsLiteral(Interface_e);
        returnAsLiteral(Bonjour_e);
        returnAsLiteral(DHCP_client_e);
        returnAsLiteral(SNMP_e);
        returnAsLiteral(Station_e);
        returnAsLiteral(uAP_e);
        returnAsLiteral(uAP_udhcp_e);
        returnAsLiteral(WIFI_drivers_e);
    }
    sprintf(heap_str, "%s(%d) undefined?\n", __func__, index);
return heap_str;
}

int read_conf_file_cb(int argc, char* argv[])
{
    net_config_file_t* file = NULL;
    int of_interest = 0; // first_conf_file
    char qualified_file[256];

    if (argc > 1) // show particular file(s)
    {
        int next = 1;
        while (argv[next])
        {
            sscanf(argv[next],"%d", &of_interest); // by number
            if (of_interest < 0 || of_interest > last_conf_file)
            {
                DBG_ERR("%s() file '%s' err\n", __func__, argv[next]);
            }
            else
            {
                file = &net_conf_files[of_interest];
                int file_type = file->purp;
                UNUSED_VAR(file_type);
                // memset(qualified_file, 0, 256);
                lookup_fs_name(of_interest, qualified_file, 256);
                DBG_VERBOSE("%s() %16s : %s\n", __func__, get_purpose_str(file_type), qualified_file);

                char* file_contents = slurp_file(qualified_file); // free buffer
                if (file_contents)
                {
                    dbg_printf("\n%s\n", file_contents);
                    free(file_contents);
                }
                else
                {
                    DBG_ERR("%s() file %s EMPTY ..\n", __func__, qualified_file);
                }
            }

            next++;
        }
    }
    else // show all files
    {
      while (of_interest < last_conf_file)
      {
          file = &net_conf_files[of_interest];
          int file_type = file->purp;
          UNUSED_VAR(file_type);
          // memset(qualified_file, 0, 256);
          lookup_fs_name(of_interest, qualified_file, 256);
          DBG_VERBOSE("%s() %16s : %s\n", __func__, get_purpose_str(file_type), qualified_file);

          char* file_contents = slurp_file(qualified_file); // free buffer
          if (file_contents)
          {
              dbg_printf("\n%s\n", file_contents);
              free(file_contents);
          }
          else
          {
              DBG_ERR("%s() file %s EMPTY ..\n", __func__, qualified_file);
          }

          of_interest++;
      }
    }
    return of_interest;
}

#if 0
int popen_direct_cb(int argc, char* argv[])
{
    char cmd_req[128];
    error_type_t ret = OK;

    if (argc > 1) // show requested file(s)
    {
        int next = 1;
        while (argv[next])
        {
            strcat(cmd_req, argv[next]);
            strcat(cmd_req, " ");
            next++;
        }
        // char os_req[128 + 32];
        // sprintf(os_req, "popen %s ;", cmd_req);
        // ret = os_system(os_req);
        ret = os_system(cmd_req);
    }
    return ret;
}

net_config_file_t* lookup_conf_by_index(int index)
{
    if (index < last_conf_file && index >= 0)
      return &net_conf_files[index];
    return &net_conf_files[Interface_e]; // otherwise default
}

int find_conf_file_cb(int argc, char* argv[])
{
    net_config_file_t* file = NULL;
    char* find_intf(char* file); // revisit cast
    int of_interest = (int) Interface_e; // default


    // setup search file
    if (argc > 2)
    {
        int i;
        sscanf(argv[argc-1],"%d",&i);
        file= lookup_conf_by_index(i);
        dbg_printf("%s() find in file %d?\n", __func__, file->basename);
    }
    file = lookup_conf_by_index(Interface_e);
    char qualified_file[256];
    sprintf(qualified_file, "%s/%s", file->dirname, file->basename);

    char* found = NULL;
    if (argc > 1) // lookup particular string(s)
    {
        int next = 1;
        while (argv[next])
        {
            found = find_token(argv[next], qualified_file);
            if (found != NULL)
            {
                dbg_printf("%s() found '%s' in '%s'\n", __func__, *found, qualified_file);
            }
            else
            {
                dbg_printf("%s() '%s' NOT found in '%s'\n", __func__, argv[next], qualified_file);
            }
            next++;
        }
    }
    else // used canned lookups
    {
        found = find_intf(qualified_file);
        if (found != NULL)
        {
            dbg_printf("%s() found 'iface' in '%s'\n", __func__, *found);
        }
        found = find_auto(qualified_file);
        if (found != NULL)
        {
            dbg_printf("%s() found 'auto' in '%s'\n", __func__, *found);
        }
        found = find_manual(qualified_file);
        if (found != NULL)
        {
            dbg_printf("%s() found 'manual' in '%s'\n", __func__, *found);
        }
    }
    return of_interest;
}
#endif

static void register_cmd_plugin(void)
{
    int cmd_res =
        cmd_register_cmd(CMD_BUCKET, NULL, NULL, NULL, NULL, NULL);

    // don't XASSERT(cmd_res==0, cmd_res);
    if (cmd_res != CMD_OK) // don't fatal, simply note:
    {
        DBG_ERR("%s() failed registration -- '%s' (err %d)\n", __FUNCTION__, CMD_BUCKET, cmd_res);
    }

    cmd_res = cmd_register_subcmd(CMD_BUCKET,
                                   "list",
                                   NULL,
                                   NULL,
                                   NULL,
                                   list_conf_files_cb );

    // unsuccessful,  don't XASSERT( ( CMD_OK == cmd_res ), cmd_res );
    if (cmd_res != CMD_OK) // don't fatal, simply note:
    {
        DBG_ERR("%s() failed registration -- '%s/%s'\n", __FUNCTION__, CMD_BUCKET, "list");
    }

    cmd_res = cmd_register_subcmd(CMD_BUCKET,
                                   "read",
                                   NULL,
                                   NULL,
                                   NULL,
                                   read_conf_file_cb );

    // unsuccessful,  don't XASSERT( ( CMD_OK == cmd_res ), cmd_res );
    if (cmd_res != CMD_OK) // don't fatal, simply note:
    {
        DBG_ERR("%s() failed registration -- '%s/%s'\n", __FUNCTION__, CMD_BUCKET, "list");
    }


#if 0 // tbd
    // pipe-direct commands
    cmd_res =
        cmd_register_cmd("popen", NULL, NULL, NULL, NULL, NULL);

    cmd_res = cmd_register_subcmd("popen",
                                   "sh",
                                   NULL,
                                   NULL,
                                   NULL,
                                   popen_direct_cb );

    // unsuccessful,  don't XASSERT( ( CMD_OK == cmd_res ), cmd_res );
    if (cmd_res != CMD_OK) // don't fatal, simply note:
    {
        DBG_ERR("%s() failed registration -- '%s/%s'\n", __FUNCTION__, "popen", "sh");
    }
#endif

return;
}

#else
#define register_cmd_plugin(...) { (void)0; }
#endif

void net_conf_init(void)
{
    register_cmd_plugin();
}

#ifdef __cplusplus
}
#endif

// eof
