/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *\file json_config_api.h
 *\brief JSON config file interace header.
 */

#ifndef JSON_CONFIG_API_H
#define JSON_CONFIG_API_H

#include "json_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* JSON Config open types */
typedef enum
{
    JSON_CONFIG_T_ALL=0,
    JSON_CONFIG_T_READONLY,
    JSON_CONFIG_T_NOFLUSH,
} JSON_CONFIG_OPEN_TYPES;

typedef struct _json_config
{
    char *filePath;
    JSON_CONFIG_OPEN_TYPES openType;
    
    JSON *json;
    
} JSON_CONFIG;


extern JSON_CONFIG* json_config_new(char *pathToFile);
JSON_CONFIG* json_config_open(char *pathToFile, JSON_CONFIG_OPEN_TYPES openType);

extern void json_config_flush(JSON_CONFIG *config);
extern void json_config_close(JSON_CONFIG *config);

extern error_type_t json_config_getString(JSON_CONFIG *config, char *directive, char *key, char *value, uint32_t max_len_in_bytes);
extern error_type_t json_config_getNumber(JSON_CONFIG *config, char *directive, char *key, uint32_t *value);
extern error_type_t json_config_getBool(JSON_CONFIG *config, char *directive, char *key, bool *value);

extern error_type_t json_config_setString(JSON_CONFIG *config, char *directive, char *key, char *value);
extern error_type_t json_config_setNumber(JSON_CONFIG *config, char *directive, char *key, uint32_t value);
extern error_type_t json_config_setBool(JSON_CONFIG *config, char *directive, char *key, bool value);

extern error_type_t json_config_delete(JSON_CONFIG *config, char *directive, char *key);

extern int json_config_getArrayCount(JSON_CONFIG *config, char *directive, char *key);

extern error_type_t json_config_setStringToArray(JSON_CONFIG *config, char *directive, char *key, int index, char *value);
extern error_type_t json_config_setNumberToArray(JSON_CONFIG *config, char *directive, char *key, int index, uint32_t value);
extern error_type_t json_config_setBoolToArray(JSON_CONFIG *config, char *directive, char *key, int index, bool value);

extern error_type_t json_config_getStringFromArray(JSON_CONFIG *config, char *directive, char *key, int index, char *value, uint32_t max_len_in_bytes);
extern error_type_t json_config_getNumberFromArray(JSON_CONFIG *config, char *directive, char *key, int index, uint32_t *value);
extern error_type_t json_config_getBoolFromArray(JSON_CONFIG *config, char *directive, char *key, int index, bool *value);

extern error_type_t json_config_deleteFromArray(JSON_CONFIG *config, char *directive, char *key, int index);

#ifdef __cplusplus
}
#endif

#endif //JSON_CONFIG_API_H
