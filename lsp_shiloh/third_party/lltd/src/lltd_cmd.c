/*
 # ============================================================================
 # Copyright (c) 2014 Marvell Semiconductor, Inc. All Rights Reserved
 #                      
 # ============================================================================
 */

#ifdef USE_LLTD_CMD
#include <string.h>

#include "cmd_proc_api.h"
#include "globals.h"
#include "net_link_api.h" // net_link_lookup_by_name()

#define LLTD_CMD_MAIN "lltd"
#define LLTD_SUBCMD_DUMP "dump"
#define LLTD_SUBCMD_TRIGGER "trigger"
#define LLTD_SUBCMD_TRACE "trace"

static int lltd_cmd_cb_dump(int argc, char* argv[])
{
    IF_DEBUG
        int i;
        DBG_ERR("Responder Interfaces\n");
        for (i = 0; i < g_osl->responder_if_num; i++)
            DBG_ERR(" [%2d][%5s][%3d][%8s][" ETHERADDR_FMT "]\n",
                            IF_INDEX(i), IF_NAME(i), IF_SOCK(i),
                            IF_TYPE(i) == 0 ? "wired" : 
                            IF_TYPE(i) == 1 ? "wireless" : "--- ",
                            ETHERADDR_PRINT(&IF_ADDR(i)));
    END_DEBUG
    return CMD_OK;
}

static int lltd_cmd_cb_trigger(int argc, char* argv[])
{
    int next = 1;
    char* link_name = argv[next++];

    while (link_name)
    {
        net_link_t* link = net_link_lookup_by_name(link_name);
        if (link)
        {
            DBG_ERR("%s() %s -> %#x\n", __func__, link_name, link);
#if DEBUG
            osl_interface_handle_state_change(NET_IFACE_EVENT_LINK_STATUS_CHANGE, (void*)link);
            lltd_cmd_cb_dump(0, NULL);
#endif 
        }
        link_name = argv[next++];
    }
    return CMD_OK;
}

static int lltd_cmd_cb_trace(int argc, char* argv[])
{
    if (argc != 2)
        return CMD_USAGE_ERROR;

    if (strcmp(argv[1], "on") == 0)
        g_trace_flags = TRC_SELECT;
    else if (strcmp(argv[1], "off") == 0)
        g_trace_flags = 0;
    else
        return CMD_USAGE_ERROR;

    IF_TRACED(TRC_SELECT)
        DBG_ERR("trace activated\n");
    END_TRACE
    return CMD_OK;
}

void lltd_cmd_register(void)
{
    if (cmd_register_cmd(LLTD_CMD_MAIN, NULL,
                            NULL, NULL, NULL, NULL) != CMD_OK)
        DBG_ERR("\"%s\" command registration failed\n", LLTD_CMD_MAIN);

    if (cmd_register_subcmd(LLTD_CMD_MAIN, LLTD_SUBCMD_DUMP,
                            NULL, NULL, NULL, lltd_cmd_cb_dump) != CMD_OK)
        DBG_ERR("\"%s\" subcommand registration failed\n", LLTD_SUBCMD_DUMP);

    if (cmd_register_subcmd(LLTD_CMD_MAIN, LLTD_SUBCMD_TRIGGER,
                            NULL, NULL, NULL, lltd_cmd_cb_trigger) != CMD_OK)
        DBG_ERR("\"%s\" subcommand registration failed\n", LLTD_SUBCMD_TRIGGER);

    if (cmd_register_subcmd(LLTD_CMD_MAIN, LLTD_SUBCMD_TRACE,
                            NULL, NULL, NULL, lltd_cmd_cb_trace) != CMD_OK)
        DBG_ERR("\"%s\" subcommand registration failed\n", LLTD_SUBCMD_TRACE);
}

#endif // USE_LLTD_CMD

// eof lltd_cmd.c
