/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *\file json_config.c
 *\brief JSON config file interface.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include "dprintf.h"
#include "logger.h"
#include "lassert.h"
#include "memAPI.h"
#include "SysInit.h"
#include "platform_api.h"

#include "json_config_api.h"



// General debug trace commands
#define DBG_PRFX "JSON_CONFIG: "
#define DBG_MSG(...) DPRINTF( (DBG_LOUD|DBG_OUTPUT), (DBG_PRFX __VA_ARGS__) )



char* json_config_strdup(char* string)
{
      size_t length = strlen(string) + 1;
      char *newStr = (char*)MEM_MALLOC(length);

      if (newStr)
      {
        strcpy(newStr, string);
      }
      
      return newStr;
}

JSON_CONFIG* json_config_new(char *pathToFile)
{
    FILE *file;
    JSON_CONFIG *config = NULL;

    // file should not already exist
    file = fopen(pathToFile, "r+");
    if (file)
    {
        fclose(file);
        return NULL;
    }
    
    config = (JSON_CONFIG *)MEM_MALLOC(sizeof(JSON_CONFIG));
    if (config != NULL)
    {
        memset(config, 0, sizeof(JSON_CONFIG));
        
        config->filePath = json_config_strdup(pathToFile);
        config->json = json_newNull();

    }
    
    return config;
}

JSON_CONFIG* json_config_open(char *pathToFile, JSON_CONFIG_OPEN_TYPES openType)
{
    FILE *file;
    JSON_CONFIG *config;

    config = (JSON_CONFIG *)MEM_MALLOC(sizeof(JSON_CONFIG));
    if (config != NULL)
    {
        memset(config, 0, sizeof(JSON_CONFIG));
        
        config->filePath = json_config_strdup(pathToFile);
        config->openType = openType;
        
        file = fopen(pathToFile, "r+");
        if (file)
        {
            struct stat st;
            uint32_t size = 0;
            int fd = fileno(file);
            
            fstat(fd, &st);
            size = st.st_size;            
            
            if (size)
            {
                char *data = MEM_MALLOC(size);
                
                if (data)
                {
                    size = fread(data, 1, size, file);
                    
                    config->json = json_parseInput(data);
                    memFree(data);
                }
            }
            
            fclose(file);
        }
        else
        {
            config->json = json_newNull();
        }
    }
    
    return config;
}

void json_config_flush(JSON_CONFIG *config)
{
    FILE *file;

    ASSERT(config != NULL);
    
    if (config->openType == JSON_CONFIG_T_ALL)
    {
        file = fopen(config->filePath, "w+");
        if (file)
        {
            char *data = json_formatOutput(config->json);
                
            if (data)
            {
                uint32_t size = strlen(data);
                fwrite(data, 1, size, file);
                
                memFree(data);
            }
            
            fclose(file);
        }
    }
}

void json_config_close(JSON_CONFIG *config)
{
    ASSERT(config != NULL);

    json_config_flush(config);
    
    if (config->json)
        json_free(config->json);

    if (config->filePath)
        memFree(config->filePath);
        
    memFree(config);
}

JSON* json_config_getParentNode(JSON_CONFIG *config, char *directive)
{
    JSON *node;

    ASSERT(config != NULL);

    node = config->json;

    if (directive)
    {
        char *ptr = json_config_strdup(directive);
        char *token, *saveptr;
        char *delim = "\\";
        
        token = strtok_r(ptr, delim, &saveptr);
        while( token && node ) 
        {
            node = json_getObject(node, token);
            token = strtok_r(NULL, delim, &saveptr);
        } 
        
        memFree(ptr);
    }
    
    return node;
}

JSON* json_config_setChildNode(JSON *node, char *key)
{
    JSON *child = json_getObject(node, key);
    
    if (child == NULL)
    {
        JSON *newObject = json_newObject(key);
        
        if (newObject)
            json_insertChildObject(node, key, newObject);
            
        child = json_getObject(node, key);
    }
    
    return child;
}

JSON* json_config_setParentNode(JSON_CONFIG *config, char *directive)
{
    JSON *node;

    ASSERT(config != NULL);

    node = config->json;

    if (directive)
    {
        char *ptr = json_config_strdup(directive);
        char *token, *saveptr;
        char *delim = "\\";
        
        token = strtok_r(ptr, delim, &saveptr);
        while( token && node ) 
        {
            node = json_config_setChildNode(node, token);
            token = strtok_r(NULL, delim, &saveptr);
        } 
        
        memFree(ptr);
    }
    
    return node;
}

error_type_t json_config_getString(JSON_CONFIG *config, char *directive, char *key, char *value, uint32_t max_len_in_bytes)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    ASSERT(value != NULL);

    JSON *node = json_config_getParentNode(config, directive);
    if (node)
    {
        char *ptr; 
        
        e_res = json_getObjectString(node, key, &ptr);
        if (e_res == OK && ptr)
        {
            strncpy(value, ptr, max_len_in_bytes);
            memFree(ptr);
        }
        else
        {
            e_res = FAIL;
        }
    }
    else
    {
        e_res = FAIL;
    }
    
    return e_res;
}

error_type_t json_config_setString(JSON_CONFIG *config, char *directive, char *key, char *value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    ASSERT(value != NULL);

    JSON *node = json_config_setParentNode(config, directive);
    if (node)
    {
        e_res = json_setObjectString(node, key, value);
    }
    else
    {
        e_res = FAIL;
        DBG_MSG("json_config_setString failed!");
    }
 
    return e_res;
}


error_type_t json_config_getNumber(JSON_CONFIG *config, char *directive, char *key, uint32_t *value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    ASSERT(value != NULL);

    JSON *node = json_config_getParentNode(config, directive);
    if (node)
    {
        e_res = json_getObjectNumber(node, key, value);
    }
    else
    {
        e_res = FAIL;
    }
    
    return e_res;
}

error_type_t json_config_setNumber(JSON_CONFIG *config, char *directive, char *key, uint32_t value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);

    JSON *node = json_config_setParentNode(config, directive);
    if (node)
    {
        json_setObjectNumber(node, key, value);
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}


error_type_t json_config_getBool(JSON_CONFIG *config, char *directive, char *key, bool *value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    ASSERT(value != NULL);

    JSON *node = json_config_getParentNode(config, directive);
    if (node)
    {
        e_res = json_getObjectBool(node, key, value);
    }
    else
    {
        e_res = FAIL;
    }
    
    return e_res;
}

error_type_t json_config_setBool(JSON_CONFIG *config, char *directive, char *key, bool value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);

    JSON *node = json_config_setParentNode(config, directive);
    if (node)
    {
        json_setObjectBool(node, key, value);
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}


error_type_t json_config_delete(JSON_CONFIG *config, char *directive, char *key)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    
    if (config->openType == JSON_CONFIG_T_READONLY)
        return FAIL;

    JSON *node = json_config_getParentNode(config, directive);
    if (node)
    {
        json_removeObject(node, key);
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}

int json_config_getArrayCount(JSON_CONFIG *config, char *directive, char *key)
{
    ASSERT(config != NULL);
    
    JSON *node = json_config_getParentNode(config, directive);
    if (node && key)
    {
        node = json_getObject(node, key);
    }
    
    return json_getArrayCount(node);
}

error_type_t json_config_setStringToArray(JSON_CONFIG *config, char *directive, char *key, int index, char *value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    
    if (config->openType == JSON_CONFIG_T_READONLY)
    {
        DBG_MSG("json_config_setStringToArray Failed, set to read only\n");
        return FAIL;
    }

    JSON *node = json_config_setParentNode(config, directive);
    if (node && key)
    {
        node = json_config_setChildNode(node, key);
    }
    
    if (node)
    {
        json_removeArray(node, index);
        json_insertStringIntoArray(node, index, value);
    }
    else
    {
        DBG_MSG("json_config_setStringToArray Failed, NULL node\n");
        e_res = FAIL;
    }
 
    return e_res;
}

error_type_t json_config_setNumberToArray(JSON_CONFIG *config, char *directive, char *key, int index, uint32_t value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    
    if (config->openType == JSON_CONFIG_T_READONLY)
        return FAIL;

    JSON *node = json_config_setParentNode(config, directive);
    if (node && key)
    {
        node = json_config_setChildNode(node, key);
    }
    
    if (node)
    {
        json_removeArray(node, index);
        json_insertNumberIntoArray(node, index, value);
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}

error_type_t json_config_setBoolToArray(JSON_CONFIG *config, char *directive, char *key, int index, bool value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    
    if (config->openType == JSON_CONFIG_T_READONLY)
        return FAIL;

    JSON *node = json_config_setParentNode(config, directive);
    if (node && key)
    {
        node = json_config_setChildNode(node, key);
    }
    
    if (node)
    {
        json_removeArray(node, index);
        json_insertBoolIntoArray(node, index, value);
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}

error_type_t json_config_getStringFromArray(JSON_CONFIG *config, char *directive, char *key, int index, char *value, uint32_t max_len_in_bytes)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    
    JSON *node = json_config_getParentNode(config, directive);
    if (node && key)
    {
        node = json_getObject(node, key);
    }
    
    if (node)
    {
        char *ptr;
        e_res = json_getArrayString(node, index, &ptr);
        if (e_res == OK && ptr)
        {
            strncpy(value, ptr, max_len_in_bytes);
            memFree(ptr);
        }
        else
        {
            e_res = FAIL;
        }
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}

error_type_t json_config_getNumberFromArray(JSON_CONFIG *config, char *directive, char *key, int index, uint32_t *value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    ASSERT(value != NULL);
    
    JSON *node = json_config_getParentNode(config, directive);
    if (node && key)
    {
        node = json_getObject(node, key);
    }
    
    if (node)
    {
        e_res = json_getArrayNumber(node, index, value);    
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}

error_type_t json_config_getBoolFromArray(JSON_CONFIG *config, char *directive, char *key, int index, bool *value)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    ASSERT(value != NULL);
    
    JSON *node = json_config_getParentNode(config, directive);
    if (node && key)
    {
        node = json_getObject(node, key);
    }
    
    if (node)
    {
        e_res = json_getArrayBool(node, index, value);    
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}

error_type_t json_config_deleteFromArray(JSON_CONFIG *config, char *directive, char *key, int index)
{
    error_type_t e_res = OK;
    
    ASSERT(config != NULL);
    
    JSON *node = json_config_getParentNode(config, directive);
    if (node && key)
    {
        node = json_getObject(node, key);
    }
    
    if (node)
    {
        json_removeArray(node, index);    
    }
    else
    {
        e_res = FAIL;
    }
 
    return e_res;
}
