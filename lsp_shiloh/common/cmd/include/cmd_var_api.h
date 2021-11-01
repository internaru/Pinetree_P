/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \file cmd_var_api.h
 *
 * \brief This is the API for C/C++ modules to interact with cmd variables
 *
 * Cmd variables are a method whereby data may be shared, globally, amongst
 * all cmd wrappers and with the C/C++ modules. Cmd variables can be thought
 * as a global string:string mapping. Each variable has a unique variable
 * name and a string value at any time. Even though the value of a variable
 * is an arbitrary string there are functions for manipulating variables as
 * if they were integers (which is a strict subset of all possible string
 * values).
 *
 **/

#ifndef CMD_VAR_API_H
#define CMD_VAR_API_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief These are the possible return values for each service in this API
 *
 * Explicit details about what values are used for what conditions are included
 * for each function as part of the function header block.
 **/
#define CMD_VAR_OK         (0)
#define CMD_VAR_MEMORY    (-1) /* Out of memory or maxlen too short */
#define CMD_VAR_NOTFOUND  (-2) /* Varname not found */
#define CMD_VAR_BAD_PARAM (-3) /* Bad parameter used in invocation */


/**
 * \brief Define a command variable with an initial value.
 *
 * \param[in] varname  The name of the variable.
 * \param[in] initval  An initial value to assign to the variable.
 *                     Initval may be "" (the empty string) but not NULL.
 * \retval  CMD_VAR_OK        - Variable is defined
 *                              (initval may or may not have been used)
 *          CMD_VAR_MEMORY    - Memory space was not available.
 *          CMD_VAR_BAD_PARAM - Either varname or initval is NULL
 *          CMD_VAR_BAD_PARAM - Varname is ""
 *
 * Note that a variable may be defined more than once but initial values on
 * subsequent definitions will be discarded. In other words if a variable is
 * defined in one place and then later the same variable (i.e. with the same
 * varname) is defined again then that second definition will have no affect
 * on the current value assigned to the variable.
 **/
int cmd_defvar(const char* varname, const char* initval);

/**
 * \brief Update the value of a variable
 *
 * \param[in] varname  The name of the variable.
 * \param[in] newval   An value to assign to the variable.
 *                     Newval may be "" (the empty string) but not NULL.
 * \retval  CMD_VAR_OK        - Variable is set to the newval without error
 *          CMD_VAR_MEMORY    - Memory space to store the newval is not avail.
 *          CMD_VAR_BAD_PARAM - Either varname or newval is NULL
 *          CMD_VAR_BAD_PARAM - Varname is ""
 *
 * Note that variables need not be defined before they are set. In such as case
 * a new variable is allocated as if cmd_defvar had been called instead.
 **/
int cmd_setvar(const char* varname, const char* newval);

/**
 * \brief Retrieve the current value assigned to a variable. Makes a copy of
 *        the value in caller provided memory.
 *
 * \param[in] varname  The name of the variable.
 * \param[in] curval   A pointer to a location into which the variable's
 *                     value may be written.
 * \param[in] maxlen   The maximum number of bytes that can be written to
 *                     curval (note that this must include space for a \0 byte).
 * \retval  CMD_VAR_OK        - A null-terminated copy of varname's value has
 *                              been copied into curval.
 *          CMD_VAR_NOTFOUND  - A variable named <varname> is not yet known.
 *          CMD_VAR_MEMORY    - curval's maxlen is not large enough to allow
 *                              varname's value to be safely copied.
 *          CMD_VAR_BAD_PARAM - Either varname or curval is NULL
 *          CMD_VAR_BAD_PARAM - Varname is ""
 *          CMD_VAR_BAD_PARAM - Maxlen is 0
 **/
int cmd_getvar(const char* varname, char* curval, int maxlen);

/**
 * \brief Retrieve the current numeric value of a specified variable
 *
 * \param[in] varname  The name of the variable.
 * \retval  0 - If any error is encountered in the call to cmd_incvar().
 * \retval  -0x8000000 - 0x7fffffff - In any other case
 *
 * Note that this is a convenient shortcut method of reading the current
 * integer value of a variable. The cmd_incvar procedure should be used when
 * error detection is necessary. "int val = cmd_getint(name)" is somewhat
 * equivalent to "cmd_incvar(name, 0, &val)". When cmd_incvar encounters an
 * error the result of cmd_getint will be zero and no error indication is
 * provided.
 **/
int cmd_getint(const char* varname);

/**
 * \brief Perform an integer addition on a variable and return the result.
 *
 * \param[in] varname  The name of the variable.
 * \param[in] inc      What value to add (can be positive, 0, or negative)
 * \param[in] newval   A pointer to a location to store the converted result.
 *                     Newval may be null if the results are not wanted.
 * \retval  CMD_VAR_OK        - Variable has been updated (in the case of non-0
 *                              inc value) and the resulting value has been
 *                              stored in newval (unless newval is NULL).
 *          CMD_VAR_NOTFOUND  - A variable named <varname> is not yet known.
 *          CMD_VAR_MEMORY    - The resulting value has grown in size such that
 *                              memory sufficient to store the result cannot be
 *                              allocated (this would be VERY rare indeed).
 *          CMD_VAR_BAD_PARAM - Varname is NULL
 *          CMD_VAR_BAD_PARAM - Varname is ""
 *
 * Note that this is a safer way than cmd_getint() to retrieve the value of
 * a variable. Inc may be 0 which allows this to be a "readonly" operation.
 **/
int cmd_incvar(const char* varname, int inc, int* newval);

#ifdef __cplusplus
}
#endif

#endif /* CMD_VAR_API_H */
