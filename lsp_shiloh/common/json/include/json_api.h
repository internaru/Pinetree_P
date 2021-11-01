/*
 * ============================================================================
 * Copyright (c) 2011-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *\file json_api.h
 *\brief JSON parser API header.
 */

#ifndef JSON_API_H
#define JSON_API_H

#ifdef __cplusplus
extern "C"
{
#endif

/* JSON Value types */
typedef enum
{
    JSON_T_OBJECT=0,
    JSON_T_ARRAY,
    JSON_T_STRING,
    JSON_T_NUMBER,
    JSON_T_BOOLEAN,
    JSON_T_NULL
} JSON_VALUE_TYPES;

/* JSON linked list structure */
typedef struct _json 
{
    // next and previous in the linked list
    struct _json *next, *prev;

    // if this is a root Object or Array type (the parent), 
    // then it will have a pointer to a child JSON struct
    struct _json *child;
    
    // key string
    char *key;
    
    // type of object
    JSON_VALUE_TYPES valueType; 
    
    // object value, based on above valueType
    char *value;
    
} JSON;


extern JSON *json_newObject(char *value);
extern JSON *json_newArray();
extern JSON *json_newString(char *string);
extern JSON *json_newNumber(uint32_t number);
extern JSON *json_newBool(bool value);
extern JSON *json_newNull();

extern void json_free(JSON *node);

extern JSON *json_parseInput(char *string);
extern char *json_formatOutput(JSON *node);

extern int   json_getArrayCount(JSON *node);
extern JSON *json_getArray(JSON *node, int index);
extern JSON *json_getObject(JSON *node, char *key);

extern error_type_t json_getArrayString(JSON *node, int index, char **string);
extern error_type_t json_getArrayNumber(JSON *node, int index, uint32_t *value);
extern error_type_t json_getArrayBool(JSON *node, int index, bool *value);

extern error_type_t json_insertStringIntoArray(JSON *node, int index, char *string);
extern error_type_t json_insertNumberIntoArray(JSON *node, int index, uint32_t number);
extern error_type_t json_insertBoolIntoArray(JSON *node, int index, bool value);

extern error_type_t json_getObjectString(JSON *node, char *key, char **string);
extern error_type_t json_getObjectNumber(JSON *node, char *key, uint32_t *value);
extern error_type_t json_getObjectBool(JSON *node, char *key, bool *value);

extern error_type_t json_setObjectString(JSON *node, char *key, char *string);
extern error_type_t json_setObjectNumber(JSON *node, char *key, uint32_t number);
extern error_type_t json_setObjectBool(JSON *node, char *key, bool value);

extern JSON *json_extractArray(JSON *node, int index);
extern JSON *json_extractObject(JSON *node, char *key);

extern error_type_t json_removeArray(JSON *node, int index);
extern error_type_t json_removeObject(JSON *node, char *key);

extern error_type_t json_insertChild(JSON *node, JSON *insert);
extern error_type_t json_insertChildIntoArray(JSON *node, int index, JSON *insert);
extern error_type_t json_insertChildObject(JSON *node, char *key, JSON *insert);

#ifdef __cplusplus
}
#endif

#endif //JSON_API_H
