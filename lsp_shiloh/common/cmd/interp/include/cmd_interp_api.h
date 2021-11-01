/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 * \file cmd_interp_api.h
 *
 * \brief This API must be implemented/provided by a valid cmd_interp backend.
 *
 * The cmd system needs exactly one cmd_interp to execute commands. This API
 * describes the services and how they are used by cmd module. There are a few
 * example cmd_interps provided including a basic "line at a time" cmd_interp
 * called "simple" and a more complex example that relies upon the opensource
 * TCL 6.7 being available.
 *
 * Theory of operations:
 *
 * cmd registers a cmd_hdlr with the cmd_interp (cmd_interp_register_cmd_hdlr)
 * cmd creates one or more cmd_interp_hdl's (cmd_interp_hdl_create)
 * cmd registers one or more commands with each cmd_interp_hdl (cmd_interp_hdl_add_cmd)
 * cmd uses cmd_interp to parse commands (cmd_interp_hdl_parse_cmd)
 * cmd uses cmd_interp to execute commands (cmd_interp_hdl_run)
 * cmd_interp, in turn, calls back into cmd (cmd_hdlr) as part of command execution.
 * cmd will, during command execution, provide results back to cmd_interp (cmd_interp_hdl_add_result)
 * cmd will delete cmd_interp_hdl's that are no longer needed (cmd_interp_hdl_delete)
 *
 **/

#ifndef __CMD_INTERP_H__
#define __CMD_INTERP_H__

/**
 * \brief The type for a cmd_interp command handler
 *
 * \param[in] data  The data pointer, provided initially, to
                    cmd_interp_hdl_add_cmd, for the executed command
 * \param[in] argc  The number of elements in the argv array
 * \param[in] argv  The arguments of the command
 * \retval          One of CMD_OK, CMD_USAGE_ERROR or CMD_ERROR
 *
 * Functions of this type will be called to execute commands. The command will
 * be parsed into argc and argv and the associated cmd, according to argv[0],
 * will be identified prior to invocation. The data provided as the first
 * parameter will be the same data provided when argv[0] was registered with
 * the cmd_interp_hdl_add_cmd API.
**/
typedef int (*cmd_interp_cmd_hdlr_t)(void* data, int argc, char* argv[]);

/**
 * \brief Provide a command handling hook to the cmd_interp
 *
 * \param[in] hdlr  A function pointer to be invoked to process commands
 *
 * The cmd_interp should expect hdlr to be provided by cmd and should invoke
 * hdlr for each command that is parsed and executed (by cmd_interp_hdl_run)
**/
void cmd_interp_register_cmd_hdlr(cmd_interp_cmd_hdlr_t hdlr);

/**
 * \brief The "opaque" type of a cmd_interp_hdl
**/
struct cmd_interp_hdl_t;

/**
 * \brief  Provides the required size of a cmd_interp_hdl
 *
 * \retval The required size of a cmd_interp_hdl
**/
int  cmd_interp_hdl_size(void);

/**
 * \brief Initialize the provided cmd_interp_hdl storage area
 *
 * \param[in] hdl  A pointer to memory of size indicated by
 *                 cmd_interp_hdl_size().
 * \retval         0 upon success
**/
int  cmd_interp_hdl_create(struct cmd_interp_hdl_t* hdl);

/**
 * \brief Free memory and associated resource for a specified cmd_interp_hdl
 *
 * \param[in] hdl  A pointer to memory that was previously initialized by
 *                 cmd_interp_hdl_create().
**/
void cmd_interp_hdl_delete(struct cmd_interp_hdl_t* hdl);

/**
 * \brief Define a command name and associated command data for a specific
 *        instance of the cmd_interp
 *
 * \param[in] hdl  A pointer to memory that was previously initialized by
 *                 cmd_interp_hdl_create().
 * \param[in] name A NULL terminated name of a command. This will be provided
 *                 as argv[0] in any future invocations of cmd_hdlr
 * \param[in] data A data pointer that will be provided in all future
 *                 invoactions of cmd_hdlr when argv[0] == name.
**/
void cmd_interp_hdl_add_cmd(struct cmd_interp_hdl_t* hdl,
                            const char* name, void* data);

/**
 * \brief Parse a single command from the input stream according to the "syntax"
 *        of the cmd language for the interpreter.
 *
 * \param[in]  hdl     A pointer to memory that was previously initialized by
 *                     cmd_interp_hdl_create().
 * \param[in]  data    A stream of input data from which commands should be
 *                     parsed. Note that neither data nor *data is modified by
 *                     this call.
 * \param[in]  datalen How much data is provided (number of bytes)
 * \param[out] cmd     *cmd will be set to either NULL or to a copy of the input
 *                     data that can be treated as a complete command. *cmd will
 *                     be NULL if either the command is not yet complete or if
 *                     no valid command has yet been identified. When *cmd is
 *                     non-NULL it is assumed to be a correctly formed command
 *                     to be executed.
 * \retval             An indication of how many bytes of data have been
 *                     consumed. When *cmd is NULL this will always equal
 *                     datalen. When *cmd is not NULL this may be less than the
 *                     full length of datalen.
 *
 * Note that this does NOT execute any command it simply serves to allow the
 * caller to identify "bites" of data to consume from the input stream and to
 * later pass into cmd_interp_hdl_run() for execution. The data is copied into
 * and held within an internal buffer until a complete command is provided to
 * the caller through the cmd pointer.
**/
int  cmd_interp_hdl_parse_cmd(struct cmd_interp_hdl_t* hdl,
                              const char* data, int datalen, const char** cmd);

/**
 * \brief Execute a command string
 *
 * \param[in]  hdl    A pointer to memory that was previously initialized by
 *                    cmd_interp_hdl_create().
 * \param[in]  cmd    A NULL-terminated command to be executed.
 * \param[out] result If any results have been generated then *result will
 *                    contain a NULL terminated string indicating those results.
 *                    Cmd will display these results to the user in the correct
 *                    way.
 * \retval            0 indicates the command execution was successful.
 *
 * The implementation of this command will invoke the registered cmd_hdlr to do
 * the actual work. The implementation needs to be able to tokenize the cmd
 * into argc and argv and the identify registered commands and provide the
 * associated data to the cmd_hdlr function.
**/
int  cmd_interp_hdl_run(struct cmd_interp_hdl_t* hdl,
                        const char* cmd,
                        const char** result);

/**
 * \brief Provide execution results to the cmd_interp
 *
 * \param[in]  hdl    A pointer to memory that was previously initialized by
 *                    cmd_interp_hdl_create().
 * \param[in]  str    A NULL-terminated string of results
 *
 * The str should be concatenated to any previous results (for a single
 * command execution) and saved for provision back to the cmd module as part
 * of the call to cmd_interp_hdl_parse_cmd.
**/
void cmd_interp_hdl_add_result(struct cmd_interp_hdl_t* hdl, const char* str);

#endif /* __CMD_INTERP_H__ */
