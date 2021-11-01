/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \file cmd_proc_api.h
 *
 * \brief This is the cmd procedure implementer's API
 *
 * Cmd procedures are implemented in many modules of the SDK. Each implementor
 * will use the functions described herein to register those procedures with
 * the cmd system and to provide their services to the system.
 *
 **/

#ifndef CMD_PROC_API_H
#define CMD_PROC_API_H

#include <stdlib.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Return values for cmd_c_proc
 *
 * CMD_OK and CMD_ERROR are pretty self explanatory.
 * CMD_USAGE_ERROR is a special case of CMD_ERROR that will cause the cmd
 *   system to provide whatever usage information is available to the user
 *   as output (not as results). All other handling of CMD_USAGE_ERROR is
 *   exactly as CMD_ERROR.
 */
#define CMD_OK           (0)
#define CMD_ERROR       (-1)
#define CMD_USAGE_ERROR (-2)

/**
 * \brief This is the type of registered C procedures for the cmd system
 *
 * \param[in] argc  The number of elements in the argv array
 * \param[in] argv  The arguments, including the cmd specification, that were
 *                  used to cause this invocation of a command.
 * \retval          One of CMD_OK, CMD_ERROR, or CMD_USAGE_ERROR.
 *
 * Commands are "executed" by functions of these types. All command line
 * parameters are parsed and provided by way of argc and argv. The command
 * name(s) are provided in the initial argv values with additional values
 * allocated for each command parameter. For example, a command executed as
 * 'mycmd arg1 "arg2 is this" arg3' (without the enclosing tick-marks) would
 * be parsed and executed with argv[0]="mycmd", argv[1]="arg1",
 * argv[2]="arg2 is this" and argv[3]="arg3".
 *
 * Note that in the case of commands which employ hierarchical levels of
 * subprocs the "path" to the cmd_c_proc is parsed and stored in argv[0].
 * Inside of argv[0] is a space separated list of cmd, subproc names. For this
 * reason it is always ensured that the first "real" parameter to a procedure
 * will always be in argv[1]. Argv[0] will vary depending upon where/how the
 * command is registered.
 *
 * A return value of CMD_USAGE_ERROR should be returned as a cue to the cmd
 * system to provide an indication to the user that the command was invoked
 * with the wrong number or type of arguments. An appropriate error message
 * will be produced along with the usage string for the command (if provided).
 *
 * Command procedures can (and should) produce output with the cmd_printf()
 * function. Cmd_printf() may only be called from such a context.
 *
 * Command procedures should provide results, if any, to the user by using the
 * cmd_append_result() function.
 */
typedef int (*cmd_c_proc)(int argc, char* argv[]);

/**
 * \brief Clients should register their command handlers via this function.
 * (DEPRECATED see cmd_register() below)
 *
 * \param[in] name       The NULL terminated name by which this procedure will
 *                       be known. Pointed to memory should be owned by the
 *                       caller and should be non-volatile (i.e. permanently
 *                       allocated). Note that names that start with an
 *                       underscore (_) will be somewhat hidden by the help
 *                       system. Commands thus named can still be executed but
 *                       unless they are explicitly searched for, verbatim,
 *                       they will not be visible to the help system. This is
 *                       a means whereby commands intended only for internal
 *                       use can be left "unadvertised". Acceptable characters
 *                       for name are a-z, 0-9 and _. (no upper case!)
 * \param[in] shortalias <deprecated - pass it whatever you want - for now>
 * \param[in] desc       This is a one sentence, short description of the
 *                       command procedure. It should not contain any sort of
 *                       usage guidelines. It will be provided to the user in
 *                       the case of a non-verbose help request.
 *                       Pointed to memory should be owned by the caller and
 *                       should be non-volatile (i.e. permanently allocated).
 * \param[in] usage      This is a usage string for the command. Convention
 *                       calls for the following "syntax".
 *                       -Variable names are given in <>'s (and described in
 *                          detail in the "notes" section).
 *                       -A parameter in []'s is optional
 *                       -Optional parameters may have default values specified
 *                       The following is an example usage string:
 *                       <name> <value> 0|1 [<mode>=normal] [ABC|PDQ|none]
 *                       This is interpreted to mean that this command takes
 *                       at least 3 parameters and potentially up to 5.
 *                       The first parameter is called "name" and should be
 *                         described in more detail in the notes data.
 *                       The second parameter is called "value" (see notes).
 *                       The third parameter should be either 0 or 1
 *                       The fourth parameter is called <mode>. It is optional.
 *                         If not provided it will take on the value of "normal"
 *                         Other valid values would likely be described in notes
 *                       The fifth parameter has no name and should, if
 *                         provided, be either ABC, PDQ or none.
 *                       Pointed to memory should be owned by the caller and
 *                       should be non-volatile (i.e. permanently allocated).
 *                       usage for cmds which have subcmds (i.e. proc==NULL)
 *                         will never be used by the help system. Each subcmd
 *                         will have its own usage string.
 * \param[in] notes      A description of the command's behavior, assumptions,
 *                       parameters and anything else. This should be a single
 *                       string with no embedded carriage returns, line feeds
 *                       or tab characters. The cmd system will flow this to an
 *                       appropriate number of columns when it provides this
 *                       data in response to a verbose help request.
 *                       Pointed to memory should be owned by the caller and
 *                       should be non-volatile (i.e. permanently allocated).
 * \param[in] proc       This is the command handler (see cmd_c_proc)
 *                       If subcommands will be registered then "proc" should
 *                       be NULL (in fact it must be NULL).
 * \retval               0 indicates success. !0 indicates failure.
 **/
#ifdef HAVE_CMD
    int cmd_register_cmd(const char* name, const char* shortalias,
			 const char* desc, const char* usage, const char* notes,
			 cmd_c_proc  proc);
#else
    static inline int cmd_register_cmd(const char* name, const char* shortalias,
				       const char* desc, const char* usage, const char* notes,
				       cmd_c_proc  proc) 
    { return 0; }
#endif

/**
 * \brief Clients may register sub-commands of cmds (or other subcmds)
 * (DEPRECATED see cmd_register() below)
 *
 * \param[in] cmdname  The name of the command under which this subcommand will
 *                     be found. cmdname may be a sequence of command names
 *                     separated by a single space if the new sub-command is to
 *                     be found more than one level deep. For example, a cmdname
 *                     of "top next" would be used to hand this new command
 *                     under the "next" sub-command and the top level command
 *                     named "top". Note that all "nodes" of the cmdname must
 *                     have been previously registered and must have NULL proc
 *                     handlers. You cannot attach sub commands beneath commands
 *                     which have non-NULL proc handlers.
 * \param[in] subname  This is the subcommand name. Subcommands are invoked
 *                     as <cmdname> <subname> [... parameters ...].
 *                     Note that subnames that start with an underscore (_)
 *                     will be somewhat hidden by the help system. Commands
 *                     thus named can still be executed as normal but they will
 *                     not be normally searchable. To get the help for such a
 *                     subcommand the help search string must match, verbatim,
 *                     this subname.
 *                     Pointed to memory should be owned by the caller and
 *                     should be non-volatile (i.e. permanently allocated)
 *                     Acceptable characters for subname are a-z, 0-9 and _.
 * \param[in] desc     This is a one sentence, short description of the
 *                     command procedure. It should not contain any sort of
 *                     usage guidelines. It will be provided to the user in
 *                     the case of a non-verbose help request.
 *                     Pointed to memory should be owned by the caller and
 *                     should be non-volatile (i.e. permanently allocated).
 * \param[in] usage    This is a usage string for the command. Convention
 *                     calls for the following "syntax".
 *                     -Variable names are given in <>'s (and described in
 *                        detail in the "notes" section).
 *                     -A parameter in []'s is optional
 *                     -Optional parameters may have default values specified
 *                     The following is an example usage string:
 *                     <name> <value> 0|1 [<mode>=normal] [ABC|PDQ|none]
 *                     This is interpreted to mean that this command takes
 *                     at least 3 parameters and potentially up to 5.
 *                     The first parameter is called "name" and should be
 *                       described in more detail in the notes data.
 *                     The second parameter is called "value" (see notes).
 *                     The third parameter should be either 0 or 1
 *                     The fourth parameter is called <mode>. It is optional.
 *                       If not provided it will take on the value of "normal"
 *                       Other valid values would likely be described in notes
 *                     The fifth parameter has no name and should, if
 *                       provided, be either ABC, PDQ or none.
 *                     Pointed to memory should be owned by the caller and
 *                     should be non-volatile (i.e. permanently allocated).
 *                     usage for cmds which have subcmds (i.e. proc==NULL)
 *                       will never be used by the help system. Each subcmd
 *                       will have its own usage string.
 * \param[in] notes    A description of the command's behavior, assumptions,
 *                     parameters and anything else. This should be a single
 *                     string with no embedded carriage returns, line feeds
 *                     or tab characters. The cmd system will flow this to an
 *                     appropriate number of columns when it provides this
 *                     data in response to a verbose help request.
 *                     Pointed to memory should be owned by the caller and
 *                     should be non-volatile (i.e. permanently allocated).
 * \param[in] proc     This is the command handler (see cmd_c_proc)
 *                     If subcommands will be registered then "proc" should
 *                     be NULL (in fact it must be NULL).
 * \retval             0 indicates success. !0 indicates failure.
 **/
#ifdef HAVE_CMD
    int cmd_register_subcmd(const char* cmdname, const char* subname,
			    const char* desc, const char* usage, const char* notes,
			    cmd_c_proc  proc);
#else
    static inline int cmd_register_subcmd(const char* cmdname, const char* subname,
					  const char* desc, const char* usage, const char* notes,
					  cmd_c_proc  proc)
    { return 0; }
#endif

/**
 * \brief Register a cmd, subcmd, or cmd hierarchy level
 *
 * \param[in] cmdname  The full path for the cmd, subcmd, or hierarchy level
 *                     being registered. Nodes in the hierarchy are separated
 *                     by a single space character. Memory pointed to may be
 *                     freed upon return from this call as an internal copy is
 *                     made upon successful execution. Note that names that
 *                     start with an underscore (_) will be somewhat hidden by
 *                     the help system. Commands thus named can still be
 *                     executed but unless they are explicitly searched for,
 *                     verbatim, they will not be visible from the help system.
 *                     This is a means whereby commands intended only for
 *                     internal use can be left "unadvertised". Acceptable
 *                     characters for names are a-z, 0-9 and _. (no upper case!)
 * \param[in] desc     This is a one sentence, short description of the
 *                     cmd set of cmd's under this level. It should not contain
 *                     any sort of usage guidelines or range of parameters.
 *                     Pointed to memory should be owned by the caller and
 *                     should be non-volatile (i.e. permanently allocated).
 * \param[in] usage    This is a usage string for the cmd. usage should be NULL
 *                     for non-cmd invocations (i.e. when proc is NULL).
 *                     Convention calls for the following "syntax".
 *                     -Variable names are given in <>'s (and described in
 *                        detail in the "notes" section).
 *                     -A parameter in []'s is optional
 *                     -Optional parameters may have default values specified
 *                     The following is an example usage string:
 *                     <name> <value> 0|1 [<mode>=normal] [ABC|PDQ|none]
 *                     This is interpreted to mean that this command takes
 *                     at least 3 parameters and potentially up to 5.
 *                     The first parameter is called "name" and should be
 *                       described in more detail in the notes data.
 *                     The second parameter is called "value" (see notes).
 *                     The third parameter should be either 0 or 1
 *                     The fourth parameter is called <mode>. It is optional.
 *                       If not provided it will take on the value of "normal"
 *                       Other valid values would likely be described in notes
 *                     The fifth parameter has no name and should, if
 *                       provided, be either ABC, PDQ or none.
 *                     Pointed to memory should be owned by the caller and
 *                     should be non-volatile (i.e. permanently allocated).
 * \param[in] notes    A description of the command's behavior, assumptions,
 *                     parameters and anything else. This should be a single
 *                     string with no embedded carriage returns, line feeds
 *                     or tab characters. The cmd system will flow this to an
 *                     appropriate number of columns when it provides this
 *                     data in response to a verbose help request.
 *                     Pointed to memory should be owned by the caller and
 *                     should be non-volatile (i.e. permanently allocated).
 * \param[in] proc     This is the command handler (see cmd_c_proc)
 *                     If subcommands will be registered then "proc" should
 *                     be NULL (in fact it must be NULL).
 * \retval             0 indicates success. !0 indicates failure.
 **/
#ifdef HAVE_CMD
    int cmd_register(const char* cmdname,
		     const char* desc, const char* usage, const char* notes,
		     cmd_c_proc  proc);
#else
    static inline int cmd_register(const char* cmdname,
				   const char* desc, const char* usage, const char* notes,
				   cmd_c_proc  proc)
    { return 0; }
#endif



/**
 * \brief Used by cmd procedures to produce output
 *
 * \param[in] fmt  A printf style format string. See "strfmt" module for a 
 *                 description of what sorts of formatting are supported.
 * \param[in] ...  Additional paramters to satisfy the fmt string.
 *
 * Command output cannot be used as results. In a non-scripted environment
 * (i.e. no TCL or other scripting language) the distinction is irrelevant.
 *
 * If cmd_printf is called outside of a valid cmd_c_proc context then it will
 * be silently ignored and no output will be produced.
 **/
#ifdef HAVE_CMD
    int cmd_printf(const char* f,...)        __attribute__ ((format (printf,1,2)));
#else
    static inline int cmd_printf(const char* f,...)        __attribute__ ((format (printf,1,2)))
    { return 0; }
#endif
/**
 * \brief Used by cmd procedures to produce results
 *
 * \param[in] fmt  A printf style format string. See "strfmt" module for a 
 *                 description of what sorts of formatting are supported.
 * \param[in] ...  Additional paramters to satisfy the fmt string.
 *
 * Command results can be used as results. In a non-scripted environment
 * (i.e. no TCL or other scripting language) the distinction is irrelevant.
 *
 * Often the best "results" are a single word, value or other easily usable
 * piece of information. Think of it as something that could be a paramater
 * to another command. In other cases however longer bodies of results may
 * be just as useful (e.g. the list of all files within a directory).
 *
 * If cmd_printf is called outside of a valid cmd_c_proc context then it will
 * be silently ignored and no output will be produced.
 **/
#ifdef HAVE_CMD
    int cmd_append_result(const char* f,...) __attribute__ ((format (printf,1,2)));
#else
    static inline int cmd_append_result(const char* f,...) __attribute__ ((format (printf,1,2)))
    { return 0; }
#endif
/**
 * \brief Used by cmd procedures to synchronize execution with output stream
 *
 * The cmd system will buffer any output from the command stream in an effort to
 * facilitate the best performance. At times this buffering may not be desired.
 * For example, commands that provide status feedback while executing may want
 * that status to be flushed from the cmd system in a "realtime" fashion before
 * execution continues. This function allows the procedure to flush any pending
 * output to the wrapper pipeline before execution is allowed to continue.
 **/
#ifdef HAVE_CMD
    void cmd_output_sync(void);
#else
    static inline void cmd_output_sync(void)
    { return 0; }
#endif
/**
 * \brief Convert a string, with any necessary base conversion, into a signed
 *        representation of the resulting value.
 *
 * \param[in] str  A NULL-terminated character string. The first space
 *                 delimited number will be converted and returned. If
 *                 non-numeric digits precede valid numeric digits than the
 *                 value of 0 is returned (i.e. no errors are detected in the
 *                 normal sense).
 *
 * As you can see this is a simple macro invocation of strtoul(). You can find
 * more details about how this function works by seeing the man pages of that
 * function. The point in using cmd_atoi is to allow cmd's to use this as a
 * reliable way of making sure that this sort of string to integer service can
 * be done even on systems that may not provide strtoul.
 **/
#ifdef HAVE_CMD
#define cmd_atoi(str) ((int)(strtoul(str, NULL, 0)))
#else
    static inline void cmd_atoi(str)
    { }
#endif
/** \brief An enum used to support the cmd_memory_dump() function to indicate
 * the wordsize of the memory dump */
typedef enum cmd_dump_wordsize_e
{
    CMD_DUMP_WORDSIZE_1BYTE, /*!< dump in 1 byte chunks */
    CMD_DUMP_WORDSIZE_2BYTES, /*!< dump in 2 byte chunks */
    CMD_DUMP_WORDSIZE_4BYTES, /*!< dump in 4 byte chunks */
} cmd_dump_wordsize_t;


/** \brief An enum used to support the cmd_memory_dump() function to indicate
 * which format to use when dumping memory. */
typedef enum cmd_dump_format_e
{
    CMD_DUMP_FORMAT_ALL, /*!< Dump both HEX and ASCII, the most common option */
    CMD_DUMP_FORMAT_HEX, /*!< Dump only HEX, NO ASCII */
    CMD_DUMP_FORMAT_ASCII, /*!< Dump only ASCII, NO HEX */
} cmd_dump_format_t;


/**
 * \brief dump values at a specified memory address using the cmd_printf() function.
 *
 * \param address a memory address where the memory dump will begin reading
 * values from
 *
 * \param numwords The number of words to dump, the word size is specified
 * using the wordsize parameter
 *
 * \param wordsize A valid cmd_dump_wordsize_t enum value, indicating the word size
 * to use when displaying memory
 *
 * \param format A valid cmd_dump_format_t enum value, used to indicate the
 * format of the output
 *
 * \return CMD_OK upon success, CMD_USAGE_ERROR otherwise
 *
 */
#ifdef HAVE_CMD
    error_type_t cmd_memory_dump( void *address,
				  uint32_t numwords,
				  cmd_dump_wordsize_t wordsize,
				  cmd_dump_format_t format );
#else
    static inline error_type_t cmd_memory_dump( void *address,
						uint32_t numwords,
						cmd_dump_wordsize_t wordsize,
						cmd_dump_format_t format )
    { return 0; }
#endif
#ifdef __cplusplus
}
#endif

#endif /* CMD_PROC_API_H */
