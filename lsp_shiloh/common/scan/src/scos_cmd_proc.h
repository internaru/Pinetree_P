/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/* \file cmd_proc_api.h
 *
 * \brief Clone(ish) of firmware command line cmd_proc functions.
 *
 * davep 12-Sep-2012
 */

#ifndef CMD_PROC_API
#define CMD_PROC_API

#define CMD_OK 0
#define CMD_ERROR -1
#define CMD_USAGE_ERROR -2

typedef int (*cmd_c_proc)(int argc, char* argv[]);

int cmd_register_cmd(const char* name, const char* shortalias,
                     const char* desc, const char* usage, const char* notes,
                     cmd_c_proc  proc);

int cmd_register_subcmd(const char* cmdname, const char* subname,
                const char* desc, const char* usage, const char* notes,
                cmd_c_proc  proc);

int cmd_printf(const char* f,...)        __attribute__ ((format (printf,1,2)));

int cmd_append_result(const char* f,...) __attribute__ ((format (printf,1,2)));

/* the following are not part of the OBA API */
void cmd_debug_dump( void );
int cmd_line_run( const char *cmdline );

#endif

