/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file model_info.c
 *
 *  \brief Model information utility functions
 *
 *  Please see model_info.h for interface/usage documentation.
 *
 **/
 
#include "error_types.h"
#include "string.h"
#include "platform_api.h"
#include "model_info.h"

static char model_name[MODEL_INFO_STR_SIZE];
static char model_number[MODEL_INFO_STR_SIZE];

/**
 * @brief Get the platform model name
 *
 * This routine gets the platform model name from platvars_get_prtnamestr()
 *
 * @param  String	return pointer to model name string
 *
 * @return OK or FAIL
 */
error_type_t get_model_name(char **String)
{
    int i, len;
    char * prt_name_str;

    // Pull model name from PrinterNameStr
    platvars_get_prtnamestr(&prt_name_str);
    len = strlen(prt_name_str);

    if (len > MODEL_INFO_STR_SIZE)
        len = MODEL_INFO_STR_SIZE;

    model_name[0] = 0;
    for (i=0; i<len; i++) {
        // Search for last space
        if (*(prt_name_str + len - 1 - i) == ' ') {
            strncpy(model_name, prt_name_str, len - 1 - i);
            model_name[len - 1 - i] = 0;
            break;
        }
    }

    *String = model_name;
    return OK;
}


/**
 * @brief Get the platform model number
 *
 * This routine gets the platform model number from platvars_get_prtnamestr()
 *
 * @param  String	return pointer to model number string
 *
 * @return OK or FAIL
 */
error_type_t get_model_number(char **String)
{
    int i, len;
    char * prt_name_str;

    // Pull model number from PrinterNameStr
    platvars_get_prtnamestr(&prt_name_str);
    len = strlen(prt_name_str);

    if (len > MODEL_INFO_STR_SIZE)
        len = MODEL_INFO_STR_SIZE;

    model_number[0] = 0;
    for (i=0; i<len; i++) {
        // Search for last space
        if (*(prt_name_str + len - 1 - i) == ' ') {
            strncpy(model_number, prt_name_str + len - i, i);
            model_number[i] = 0;
            break;
        }
    }

    *String = model_number;
    return OK;
}


