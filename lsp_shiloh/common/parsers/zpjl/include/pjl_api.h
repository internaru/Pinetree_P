/*
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file pjl_api.h
 *
 * \brief API for pjl to register variables to support and grammar to support.
 *
 * The grammar registration provides a hook to allow access to function
 * through pcl.
 * For example you could define a new grammar word "FOO" and a function that is 
 * executed when that word happens.  Then when pjl receives:
 * \@PJL FOO BAR Bar, the program will be called with the string "BAR Bar"
 * passed in.
 */

#ifndef _PJL_API_H
#define _PJL_API_H

#include <stdbool.h>
#include <stdint.h>
#include "agConnectMgr.h"
#include "pjl_enum_objs_api.h"
#include "PrintJobInfo.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    e_Set,      
    e_Get,
    e_GetSet,
} PJL_VARIABLE_TYPE;

typedef enum
{
	e_job_scope,    ///< job scope means set is different from default.
	e_global_scope  ///< global scope means set == default 
} pjl_variable_scope_t;

/// singleton: pjl parser table of get/set/default variables: PJL_VARS[num_variables]
struct pjl_vars
{
    PJL_VARIABLE_PROCESS    VarUse; ///< How this variable is to be used.
    pjl_variable_scope_t     scope; ///< job or global scope variable.
    uint32_t                   oid; ///< the oid value
    uint32_t             pji_index; ///< index into class table: pjl_local_variable_table[ pji_index ]
    int               DefaultValue; ///< Factory default ???
    int                   MinValue; ///< The min value of this variable
    int                   MaxValue; ///< The max value of this variable
    UINT16                  VarKey; ///< The pjl variable key.
    pjl_object_t *enum_object;
};



/**
 * \brief Structure used when registering a get/set/default variable with the pjl system
 */
typedef struct
{
    char *var_name;                 ///< Variable name
    PJL_VARIABLE_PROCESS VarUse;    ///< How this variable is to be used: get/set
    PJL_VARIABLE_TYPE var_type;     ///< The type of variable: int enum string
    pjl_variable_scope_t scope;     ///< job or global scope variable.   
    WORD_MATCH **NextMatch;         ///< the strings for use in the enum grammar ON/OFF 
    int32_t enum_start;
    int32_t enum_increment;
    uint32_t var_min_value;
    uint32_t var_max_value;
    uint32_t oid;                   ///< the oid value for this variable.
    uint32_t object_offset;
} pjl_register_variable_t;

/// called from pji_local_copy_init to populate the PJL_VARS[] table with common and oem variables.
///
void pjl_register_variable( pjl_register_variable_t *var, uint32_t index );

/**
 * \brief typedef for callback to process new grammar word.
 * When pjl finds the grammer_entry word after the \@PJL string it calls this 
 * with the rest of the pjl string.
 * \param[in] Pipe The i/o pipe on which to respond.
 * \param[in] input_string A pointer to the rest of the input pjl string.
 * \returns void Nothing is returned, any errors are ignored by the pjl parser.
 */
typedef void *(pjl_process_t)(IOReg *Pipe, char *input_string);
/**
 * \brief Structure used for registering new grammar word.
 */
typedef struct
{
    char *grammar_entry;            ///< The word after \@PJL that will cause this to be called.
    pjl_process_t *process_function;///< The function called to process this.
} pjl_reg_grammar_t;

/**
 * \brief Register a new grammar word with the pjl parser.
 *
 * This word is placed at the command level.  For example if I registered the
 * word "HELLO" then when I send the command \@PJL HELLO blah blah ...
 * The process function will be called with the string blah blah ...
 * Then the function is responsible for doing all the processing and sending
 * the responses back to the host.
 *
 * \param[in] new_grammar specifying the specifics of the word.
 * \returns void
 */
void pjl_register_new_grammar(pjl_reg_grammar_t *new_grammar);

/**
 * \brief Initialize the pjl system and get ready to run.
 */
void pjl_init(void);


#ifdef __cplusplus
}
#endif

#endif

