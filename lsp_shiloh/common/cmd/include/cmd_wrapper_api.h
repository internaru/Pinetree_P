/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \file cmd_wrapper_api.h
 *
 * \brief This is the cmd wrapper API to the cmd system.
 *
 * There are a few places where commands get executed by the system. This
 * includes parsers and interpreters of various sorts. This API describes
 * the interface functions available to create and destroy cmd wrappers and
 * to execute commands within those wrappers.
 *
 **/

#ifndef CMD_WRAPPER_H
#define CMD_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief This is a command output handler function type.
 *
 * \param[in] client_data This is the pointer provided by the client
 * \param[in] data        A pointer to the data being provided as output
 * \param[in] data_len    An indication of how much data is being provided.
 *
 * The client provides this function to the cmd wrapper service in order to
 * provide a means whereby command output data can be sent back to the client.
 *
 * Note that data is NOT \0-terminated!
 *
 **/
typedef void (*cmd_wrapper_output_hdlr_fn_t)(void* client_data,
                                             char*        data,
                                             unsigned int data_len);

/**
 * \brief Create a wrapper context for the execution of commands.
 *
 * \param[in] name  The name of the wrapper. This will be used to construct
 *                  the thread name for this wrapper as well used in any
 *                  services that require an name for the wrapper (none exist
 *                  at the time of this writing). This value is copied into
 *                  allocated memory and need not be permanently allocated from
 *                  the caller's point of view.
 * \retval  An opaque handle that must be used when invoking the other services
 *          within this API including wrapper deletion.
 *          A retval of NULL indicates that wrapper creation has failed.
 **/
void* cmd_wrapper_create(const char* name);

/**
 * \brief Delete a wrapper context and free associated memory
 *
 * \param[in] hdl  The handle returned by a call to cmd_wrapper_create()
 * \retval      0  Indicates success
 *
 * \warning Wrapper deletion will block until any current command execution
 * completes. There is no other safe way to ensure that all associated memory
 * can be freed. This includes associated stack space, any script context, etc.
 * For this reason, an attempt to delete a "hung" command context will result
 * in the context attempting the deletion to be "hung" as well. In the future
 * it may be a good idea to allow the specification of a timeout option but,
 * for now, care must be taken by the caller to be aware of this condition.
 **/
int   cmd_wrapper_delete(void* hdl);

/**
 * \brief Run a single command on a specific cmd wrapper
 *
 * \param[in] hdl         The handle returned by a call to cmd_wrapper_create()
 * \param[in] cmd         The command string to execute
 * \param[in] out         A handler to recieve any command output
 * \param[in] client_data An argument provided to out
 * \retval    0           Indicates success
 *
 * This is a blocking call that causes the cmd to be executed by a cmd wrapper.
 * The command will return once the command execution finishes. The out
 * handler will be called under the wrapper's context (i.e. not from the caller)
 * with any output data.
 *
 * Note that results of the top level command become outputs of the entire
 * command. For example: "add [add 3 4] 5" will produces an output of 12
 * ("add 3+4" produces a result of 7, this result is used in "add 7 5" to
 * produce a result of 12, since this outermost command produced results those
 * results are "upgraded" to be the output of the entire command.
 *
 * Only complete commands are executed this way. If cmd does not contain at
 * least one complete script command then nothing is done. If cmd contains more
 * then one command but the last command is not complete than that incomplete
 * command is not run. Incomplete commands are discarded with no indication.
 *
 * You may experience unexpected behavior by mixing calls to
 * cmd_wrapper_pipeline with cmd_wrapper_run or cmd_wrapper_runcmd with the
 * same cmd wrapper handle. This use is not recommended or supported.
 **/
int   cmd_wrapper_runcmd(void* hdl, const char* cmd,
                         cmd_wrapper_output_hdlr_fn_t out, void* client_data);

/**
 * \brief Run a single command on a specific cmd wrapper
 *
 * \param[in] hdl         The handle returned by a call to cmd_wrapper_create()
 * \param[in] cmd         The command string to execute
 * \param[in] outstr      A location to store any command output (may be NULL)
 * \param[in] outstr_len  The size of the memory pointed to be outstr
 * \retval             0  Indicates success
 *
 * This is a blocking call that causes the cmd to be executed by a cmd wrapper.
 * The command will return once the command execution finishes. The outstr will
 * contain the entire command output or as much of the results as can be stored
 * given the outstr_len. If the command output is truncated then the most
 * recent output is stored in outstr (i.e. the output is left truncated as
 *
 * Note that results of the top level command become outputs of the entire
 * command. For example: "add [add 3 4] 5" will produces an output of 12
 * ("add 3+4" produces a result of 7, this result is used in "add 7 5" to
 * produce a result of 12, since this outermost command produced results those
 * results are "upgraded" to be the output of the entire command.
 *
 * Only complete commands are executed this way. If cmd does not contain at
 * least one complete script command then nothing is done. If cmd contains more
 * then one command but the last command is not complete than that incomplete
 * command is not run. Incomplete commands are discarded with no indication.
 *
 * You may experience unexpected behavior by mixing calls to
 * cmd_wrapper_pipeline with cmd_wrapper_run or cmd_wrapper_runcmd with the
 * same cmd wrapper handle. This use is not recommended or supported.
 **/
int   cmd_wrapper_run(void* hdl, const char* cmd, char* outstr, int outstr_len);

/**
 * \brief Run a pipeline of command input on a specific cmd wrapper
 *
 * \param[in] hdl         The handle returned by a call to cmd_wrapper_create()
 * \param[in] cmd         The input data to feed to the wrapped script engine.
 * \param[in] out         A handler to recieve any command output
 * \param[in] client_data An argument provided to out
 * \retval    0           Indicates success
 *
 * This is a blocking call that causes the cmd data to be fed into the wrapped
 * cmd pipeline. Any executed commands cause this function to block until those
 * commands complete.
 * 
 * The out handler will be called under the wrapper's context (i.e. not from
 * the caller) with any output data.
 *
 * Note that results of the top level command become outputs of the entire
 * command. For example: "add [add 3 4] 5" will produces an output of 12
 * ("add 3+4" produces a result of 7, this result is used in "add 7 5" to
 * produce a result of 12, since this outermost command produced results those
 * results are "upgraded" to be the output of the entire command.
 *
 * Any cmd data that does not complete a command is saved until the next data
 * is provided to this cmd wrapper. In this way data is pipelined across
 * function calls into the script engine. This is useful in handling streams
 * of command data without the caller needing to delineate command boundaries.
 *
 * You may experience unexpected behavior by mixing calls to
 * cmd_wrapper_pipeline with cmd_wrapper_run or cmd_wrapper_runcmd with the
 * same cmd wrapper handle. This use is not recommended or supported.
 **/

int   cmd_wrapper_pipeline(void* hdl, const char* input, int inlen,
                           cmd_wrapper_output_hdlr_fn_t out, void* client_data);

#ifdef __cplusplus
}
#endif

#endif /* CMD_WRAPPER_API_H */
