/*
 * ============================================================================
 * Copyright (c) 2011-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *\file json.c
 *\brief JSON parser.
 */

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "dprintf.h"
#include "logger.h"
#include "lassert.h"
#include "memAPI.h"
#include "SysInit.h"
#include "platform_api.h"

#include "json_api.h"



// General debug trace commands
#define DBG_PRFX "JSON: "
#define DBG_MSG(...) DPRINTF( (DBG_SOFT|DBG_OUTPUT), (DBG_PRFX __VA_ARGS__) )


char *json_parseValue(JSON *node, char *value);
char *json_formatOutput(JSON *node);


uint32_t json_strnum(char *number)
{
    char *ptr = number;
    uint32_t value=0;
    uint32_t sign=1;
    uint32_t power=0;
    
    if (ptr)
    {
        // check for sign
        if (*ptr == '-')
        {
            sign = -1; // set negative
            ptr++;
        }

        // bypass any leading zeros
        if (*ptr == '0')
        {
            while (*ptr == '0')
            {
                // just skip past
                ptr++;
            }
        }

        // capture the numeric value
        if (*ptr >= '1' && *ptr <= '9')
        {
            while (*ptr >= '0' && *ptr <= '9')
            {
                // convert to number value
                value = (*ptr - '0') + (value * 10);
                ptr++;
            }
        }
        
        // check for any fractional part of the value
        if (*ptr == '.')
        {
            ptr++;
            
            while (*ptr >= '0' && *ptr <= '9')
            {
                // convert to number value
                value = (*ptr - '0') + (value * 10);
                power--;
                ptr++;
            }
        }
        
        // update value to final value 
        value = value * pow(10, power) * sign;

    }
    
    return value;
}

char* json_strdup(char* string)
{
      size_t length = strlen(string) + 1;
      char *newStr = (char*)MEM_MALLOC(length);

      if (newStr)
      {
        strcpy(newStr, string);
      }
      
      return newStr;
}

char *json_skip_comment(char *in)
{
    char *ptr = in;
    
    // check for valid comment start
    if (*ptr == '/')
    {
        // for next character either slash or star expected
        ptr++;
        if (*ptr == '/') 
        {
            // skip until end of line
            ptr++;
            while (ptr && *ptr) 
            {
                if (*ptr == '\n')
                {
                    ptr++; 
                    return ptr;
                }
                
                ptr++; 
            }
        } 
        else if (*ptr == '*') 
        {
            // skip until end of comment
            ptr++;
            while (ptr && *ptr) 
            {
                if (*ptr == '*')
                {
                    ptr++;
                    if (*ptr == '/')
                    {
                        ptr++; 
                        return ptr;
                    }
                    else
                    {
                        // decrement to previous character
                        ptr--;
                    }
                }
                
                ptr++; 
            }
        } 
        else
        {
            // decrement to previous character
            ptr--;
        }
    }
    
    return ptr;
}

char *json_skip_whitespace(char *in) 
{
    char *ptr = in;
    
    while (ptr && *ptr) 
    {
        if ((unsigned char)*ptr > 32)
        {
            // done
            break;
        }
        
        ptr++; 
    }
        
    return ptr;
}


JSON *json_new()
{
    JSON* node = (JSON*)MEM_MALLOC(sizeof(JSON));
    
    if (node) 
    {
        memset(node, 0, sizeof(JSON));
    }
        
    return node;
}

JSON *json_newObject(char *string)
{
    JSON *node = json_new();
    
    if (node)
    {
        node->valueType = JSON_T_OBJECT;
        node->value = json_strdup(string);
    }
    
    return node;
}

JSON *json_newArray()
{
    JSON *node = json_new();
    
    if (node)
    {
        node->valueType = JSON_T_ARRAY;
    }
    
    return node;
}

JSON *json_newString(char *string)
{
    JSON *node = json_new();
    
    if (node)
    {
        node->valueType = JSON_T_STRING;
        node->value = json_strdup(string);
    }
    
    return node;
}

JSON *json_newNumber(uint32_t number)
{
    JSON *node = json_new();
    
    if (node)
    {
        node->valueType = JSON_T_NUMBER;
        
        char *str =  (char*)MEM_MALLOC(64);
        if (str)
        {
            minSprintf(str, "%u", number);
        }
        
        node->value = str;
    }
    
    return node;
}

JSON *json_newBool(bool value)
{
    JSON *node = json_new();
    
    if (node)
    {
        node->valueType = JSON_T_BOOLEAN;
        if (value)
            node->value = json_strdup("true");
        else
            node->value = json_strdup("false");
    }
    
    return node;
}

JSON *json_newNull()
{
    JSON *node = json_new();
    
    if (node)
    {
        node->valueType = JSON_T_NULL;
    }
    
    return node;
}

void json_free(JSON *node)
{
    JSON *next = 0;
    
    while (node)
    {
        next = node->next;
        
        // delete any children
        if (node->child) 
        {
            json_free(node->child);
        }

        // free the key
        if (node->key) 
        {
            memFree(node->key);
        }

        // free the value
        if (node->value) 
        {
            memFree(node->value);
        }
        
        // free the node itself
        memFree(node);
        
        node = next;
    }
}


char *json_parseOutString(JSON *node, char *string)
{
    char *ptr = string;
    char *value, *str;
    int length = 0;
    
    // make sure node is valid
    if (!node)
    {
        // invalid node
        DBG_MSG("json_parseOutString: invalid node\n");
        return NULL;
    }
    
    // check for valid string start
    if (*ptr != '\"')
    {
        // invalid string
        DBG_MSG("json_parseOutString: invalid string\n");
        return NULL;
    }
    
    // advance past quote
    ptr++;

    // determine length of string
    while (*ptr != '\"' && *ptr)
    {
        length++;  // increment length counter
        
        // make sure to skip any escaped characters
        if (*ptr == '\\')
            ptr++;
            
        ptr++;
    }

    value = (char *)MEM_MALLOC(length+1);
    if (!value)
    {
        // unable to allocate string value
        DBG_MSG("json_parseOutString: unable to allocate string\n");
        return NULL;
    }

    ptr = string+1; // set pointer to beginning of string
    str = value; // set pointer to string value
    
    // loop while not quotes or null
    while (*ptr != '\"' && *ptr)
    {
        
        // special case any escaped characters 
        if (*ptr == '\\')
        {
            ptr++; // advance past the escape character
            
            // handle supported escaped characters
            switch (*ptr)
            {
                case 'n': // new line feed
                    *str = '\n';    
                    str++;
                    break;
                    
                case 'r': // carriage return
                    *str = '\r';    
                    str++;
                    break;
                    
                case 't': // horizontal tab
                    *str = '\t';    
                    str++;
                    break;
                    
                case 'f': // form feed
                    *str = '\f';    
                    str++;
                    break;
                    
                case 'b': // backspace
                    *str = '\b';    
                    str++;
                    break;
                    
                case 'u': // transcode utf16 to utf8
                    {
                        int64_t value;
                        char temp[5];

                        ptr++; // advance past the 'u' character
                        
                        temp[0] = *ptr++;
                        temp[1] = *ptr++;
                        temp[2] = *ptr++;
                        temp[3] = *ptr;
                        temp[4] = 0;

                        value = strtol (temp, 0, 16);

                        if (value < 0x80)
                        {
                            // just map to utf8
                            *str = (char) value;
                            str++;
                        }
                        else if (value < 0x800)
                        {
                            // two byte encoding
                            char ch1 = 0xC0;
                            char ch2 = 0x80;

                            ch2 += (value & 0x3F);
                            value >>= 6;
                            ch1 += (value & 0x1F);

                            *str = ch1;
                            str++;
                            
                            *str = ch2;
                            str++;
                        }
                        else if (value < 0x10000)
                        {
                            if (value < 0xD800 || 0xDBFF < value)
                            {
                                // three byte encoding
                                char ch1 = 0xE0;
                                char ch2 = 0x80;
                                char ch3 = 0x80;

                                ch3 += (value & 0x3F);
                                value >>= 6;
                                ch2 += (value & 0x3F);
                                value >>= 6;
                                ch1 += (value & 0xF);

                                *str =ch1;
                                str++;
                            
                                *str = ch2;
                                str++;

                                *str = ch3;
                                str++;
                            }
                            else
                            {
                                // do we need to support surrogate pairs?
                                DBG_MSG("json_parseOutString: surrogate pairs not supported!\n");
                            }
                        }
                        else
                        {
                            DBG_MSG("json_parseOutString: unsupported utf16 value: 0x%x\n", value);
                        }
                    }
                    break;
                    
                default:
                    // unsupported, just set it to the character value
                    *str=*ptr;
                    str++;
                    break;
            }
            
            ptr++;
        }
        else
        {
            // otherwise just make a copy of the character
            *str=*ptr;
            str++;
            ptr++;
        }
    }
    
    *str = 0; // terminate the string
    
    node->valueType = JSON_T_STRING;
    node->value = value;

    // check for valid string termination
    if (*ptr == '\"') 
    {
        ptr++;
        return ptr;
    }

    // invalid string termination
    DBG_MSG("json_parseOutString: invalid string termination\n");
    return NULL;
}

char *json_parseOutNumber(JSON *node, char *number)
{
    char *ptr = number;
    char *value, *str;
    int length = 0;
    
    // make sure node is valid
    if (!node)
    {
        // invalid node
        DBG_MSG("json_parseOutNumber: invalid node\n");
        return NULL;
    }
    
    if (ptr)
    {
        // skip any whitespace
        ptr = json_skip_whitespace(ptr);
        
        // determine length of string
        while (*ptr && (*ptr == '-' || *ptr == '.' || (*ptr >= '0' && *ptr <= '9')))
        {
            length++;  // increment length counter
            ptr++;
        }

        value = (char *)MEM_MALLOC(length+1);
        if (!value)
        {
            // unable to allocate string value
            DBG_MSG("json_parseOutNumber: unable to allocate string\n");
            return NULL;
        }
        
        ptr = number;
        str = value; // set pointer to string value

        // skip any whitespace
        ptr = json_skip_whitespace(ptr);
        
        while (*ptr && (*ptr == '-' || *ptr == '.' || (*ptr >= '0' && *ptr <= '9')))
        {
            *str=*ptr;
            str++;
            ptr++;
        }
            
        node->valueType = JSON_T_NUMBER;
        node->value = value;
    }
    
    return ptr;
}

char *json_parseOutObject(JSON *node, char *ptr)
{
    JSON *child;
    
    // make sure node is valid
    if (!node)
    {
        // invalid node
        DBG_MSG("json_parseOutObject: invalid node\n");
        return NULL;
    }
    
    if (*ptr != '{')
    {
        // not an object
        DBG_MSG("json_parseOutObject: not an object\n");
        return NULL;
    }    
    
    ptr++; // advance past object indicator

    node->valueType = JSON_T_OBJECT;

    // skip any whitespace
    ptr = json_skip_whitespace(ptr);    

    // check for an empty object
    if (*ptr == '}')
    {
        ptr++; // advance past object terminator
        return ptr;
    }

    child = json_new();
    if (!child)
    {
        // unable to allocate child
        DBG_MSG("json_parseOutObject: unable to allocate child\n");
        return NULL;
    }
    
    node->child = child;
    
    // get string for the child
    ptr = json_parseOutString(child, json_skip_whitespace(ptr));
    if (!ptr)
    {
        return NULL;
    }
    
    // skip any whitespace
    ptr = json_skip_whitespace(ptr);    

    // move string to string
    child->key = child->value;
    child->value = 0;
    
    if (*ptr != ':')
    {
        //invalid format
        DBG_MSG("json_parseOutObject: invalid format\n");
        return NULL;
    }
    
    ptr++;

    // get value for the child
    ptr = json_parseValue(child, ptr);
    if (!ptr)
    {
        return NULL;
    }
    
    // skip any whitespace
    ptr = json_skip_whitespace(ptr);    
    
    // loop while adding new items
    while (*ptr == ',')
    {
        JSON *newNode = json_new();
        if (!newNode)
        {
            // failed to allocate new node
            DBG_MSG("json_parseOutObject: failed to allocate new node\n");
            return NULL;
        }

        child->next = newNode;
        newNode->prev = child;
        child = newNode;
        
        
        // get string for the child
        ptr = json_parseOutString(child, json_skip_whitespace(ptr+1));
        if (!ptr)
        {
            return NULL;
        }
        
        // skip any whitespace
        ptr = json_skip_whitespace(ptr);    

        // move string to string
        child->key = child->value;
        child->value = 0;
        
        if (*ptr != ':')
        {
            //invalid format
            DBG_MSG("json_parseOutObject: invalid format\n");
            return NULL;
        }
        
        ptr++;

        // get value for the child
        ptr = json_parseValue(child, ptr);
        if (!ptr)
        {
            return NULL;
        }
        
        // skip any whitespace
        ptr = json_skip_whitespace(ptr);    
    }

    // check for valid object termination
    if (*ptr == '}') 
    {
        ptr++;
        return ptr;
    }

    // invalid object termination
    DBG_MSG("json_parseOutObject: invalid object termination\n");
    return NULL;
}

char *json_formatOutputString(char *string)
{
    char *ptr, *ptr2, *output;
    unsigned char temp;
    int length = 0;
    
    if (!string) 
    {
        // just return empty string
        return json_strdup("");
    }

    ptr = string;
    while (*ptr)
    {
        // get character
        temp = *ptr;
        
        // add one for output char
        length++;
        
        // determine if it is an escaped character
        if (strchr("\"\\\b\f\n\r\t",temp))
        {
            // add 1 for escaped prefix char
            length++; 
        } 
        // otherwise is it an unprintable character
        else if (temp<32) 
        {
            // add 5 to print out hex of char
            length += 5;
        }
            
        ptr++;
    }
    
    length += 3; // add for quotes and null char
    output = (char*)MEM_MALLOC(length);
    
    if (!output)
    {
        // failed to allocate memory for output
        DBG_MSG("json_formatOutputString: failed to allocate memory for output\n");
        return NULL;
    }

    ptr = string;
    ptr2 = output;
    
    *ptr2 = '\"';
    ptr2++;
    
    while (*ptr)
    {
        // determine if character is printable, not a quote and not an escaped prefix char
        temp = *ptr;
        if (temp > 31 && temp != '\"' && temp != '\\') 
        {
            *ptr2=*ptr;
            
            ptr++;
            ptr2++;
        }
        else
        {
            *ptr2 = '\\';
            ptr2++;
            
            temp = *ptr;
            ptr++;
            
            switch (temp)
            {
                case '\\':    
                    *ptr2 = '\\';    
                    ptr2++;
                    break;
                    
                case '\"':    
                    *ptr2 = '\"';    
                    ptr2++;
                    break;
                    
                case '\b':    
                    *ptr2 = 'b';    
                    ptr2++;
                    break;
                    
                case '\f':    
                    *ptr2 = 'f';    
                    ptr2++;
                    break;
                    
                case '\n':    
                    *ptr2 = 'n';    
                    ptr2++;
                    break;
                    
                case '\r':    
                    *ptr2 = 'r';    
                    ptr2++;
                    break;
                    
                case '\t':    
                    *ptr2 = 't';    
                    ptr2++;
                    break;
                    
                default: // print escaped hex value
                    minSprintf(ptr2, "u%04x", temp);
                    ptr2 += 5;    
                    break;    
            }
        }
    }
    
    *ptr2 = '\"';
    ptr2++;
    
    *ptr2 = 0;
    ptr2++;
    
    return output;
}

char *json_formatString(JSON *node)
{
    if (!node) 
    {
        DBG_MSG("json_formatString: invalid node\n");
        return NULL;
    }

    return json_formatOutputString(node->value);
}

char *json_formatNumber(JSON *node)
{
    if (!node) 
    {
        DBG_MSG("json_formatNumber: invalid node\n");
        return NULL;
    }

    return (json_strdup(node->value));
}

char *json_formatObject(JSON *node)
{
    int numArray = 0, arraySize = 0, size = 0, i;
    char **array, **strings;
    JSON *child;
    char *ptr, *formattedOutput = 0;

    
    if (!node)
    {
        DBG_MSG("json_formatObject: invalid node\n");
        return NULL;
    }
    
    child=node->child;
    
    // determine the number of items needed in array
    while (child)
    {
        numArray++;
        child = child->next;
    }
    
    arraySize = numArray * sizeof(char*);
    
    // allocate array
    array=(char**)MEM_MALLOC(arraySize);
    if (!array) 
    {
        // unable to allocate array
        DBG_MSG("json_formatObject: unable to allocate memory for array\n");
        return NULL;
    }
    
    // allocate strings
    strings=(char**)MEM_MALLOC(arraySize);
    if (!strings) 
    {
        memFree(array);
        
        // unable to allocate strings
        DBG_MSG("json_formatObject: unable to allocate memory for strings\n");
        return NULL;
    }
    
    // init array items
    memset(array, 0, arraySize);
    memset(strings, 0, arraySize);
    
    size += 7;
    
    // setup all array items
    child=node->child;
    i = 0;
    while (child)
    {
        strings[i] = json_formatOutputString(child->key);
        array[i] = json_formatOutput(child);
        
        if (strings[i] && array[i]) 
        {
            // increment size of output string
            size += strlen(strings[i]) + strlen(array[i]) + 2; 
        }
        else 
        {
            // failure
            size = 0;
            break;
        }
        
        i++;
        child=child->next;
    }
    
    // if size is valid, then attempt to allocate memory for formatted output
    if (size) 
    {
        formattedOutput=(char*)MEM_MALLOC(size);
    }
    
    // assume failure if formatted output not allocated
    if (!formattedOutput)
    {
        for (i=0; i<numArray; i++)
        {
            if (strings[i]) 
            {
                memFree(strings[i]);
            }
            
            if (array[i]) 
            {
                memFree(array[i]);
            }
        }
        
        memFree(strings);
        memFree(array);
        
        DBG_MSG("json_formatObject: formatted output failure\n");
        return NULL;
    }
    
    // generate formatted output for JSON array
    ptr=formattedOutput;
    *ptr='{';
    ptr++;
    
    *ptr=0;
    
    for (i=0; i<numArray; i++)
    {
        strcpy(ptr, strings[i]);
        ptr += strlen(strings[i]);
        
        *ptr = ':';
        ptr++;
        
        strcpy(ptr, array[i]);
        ptr += strlen(array[i]);
        
        if (i < numArray-1)
        {
            *ptr =',';
            ptr++;
        }
            
        *ptr=0;
        
        memFree(strings[i]);
        memFree(array[i]);
    }
    
    memFree(strings);
    memFree(array);

    *ptr = '}';
    ptr++;
    *ptr = 0;
    
    return formattedOutput;    
}

char *json_parseOutArray(JSON *node, char *ptr)
{
    JSON *child;
    
    // make sure node is valid
    if (!node)
    {
        // invalid node
        DBG_MSG("json_parseOutArray: invalid node\n");
        return NULL;
    }
    
    if (*ptr != '[')
    {
        // not an array
        DBG_MSG("json_parseOutArray: not an array\n");
        return NULL;
    }    
    
    ptr++; // advance past array indicator

    node->valueType = JSON_T_ARRAY;

    // skip any whitespace
    ptr = json_skip_whitespace(ptr);    

    // check for an empty array
    if (*ptr == ']')
    {
        ptr++; // advance past array terminator
        return ptr;
    }

    child = json_new();
    if (!child)
    {
        // unable to allocate child node
        DBG_MSG("json_parseOutArray: unable to allocate child node\n");
        return NULL;
    }
    
    node->child = child;
    
    // get value for the child
    ptr = json_parseValue(child, ptr);
    if (!ptr)
    {
        return NULL;
    }
    
    // skip any whitespace
    ptr = json_skip_whitespace(ptr);    
    
    // loop while adding new items
    while (*ptr == ',')
    {
        JSON *newNode = json_new();
        if (!newNode)
        {
            // failed to allocate new node
            DBG_MSG("json_parseOutArray: failed to allocate new node\n");
            return NULL;
        }

        child->next = newNode;
        newNode->prev = child;
        child = newNode;
        
        // get value for new child
        ptr = json_parseValue(child, ptr+1);
        if (!ptr)
        {
            return NULL;
        }
        
        // skip any whitespace
        ptr = json_skip_whitespace(ptr);    
    }

    // check for valid array termination
    if (*ptr == ']') 
    {
        ptr++;
        return ptr;
    }

    // invalid array termination
    DBG_MSG("json_parseOutArray: invalid array termination\n");
    return NULL;
}

char *json_formatArray(JSON *node)
{
    int numArray = 0, arraySize = 0, size = 0, i;
    char **array;
    JSON *child;
    char *ptr, *formattedOutput = 0;

    
    if (!node)
    {
        DBG_MSG("json_formatArray: invalid node\n");
        return NULL;
    }
    
    child=node->child;
    
    // determine the number of items needed in array
    while (child)
    {
        numArray++;
        child = child->next;
    }
    
    arraySize = numArray * sizeof(char*);
    
    // allocate array
    array=(char**)MEM_MALLOC(arraySize);
    if (!array) 
    {
        // unable to allocate array
        DBG_MSG("json_formatArray: inable to allocate array\n");
        return NULL;
    }
    
    // init array items
    memset(array, 0, arraySize);
    
    size += 5;
    
    // setup all array items
    child=node->child;
    i = 0;
    while (child)
    {
        array[i] = json_formatOutput(child);
        
        if (array[i]) 
        {
            // increment size of output string
            size += strlen(array[i]) + 2; 
        }
        else 
        {
            // failure
            size = 0;
            break;
        }
        
        i++;
        child=child->next;
    }
    
    // if size is valid, then attempt to allocate memory for formatted output
    if (size) 
    {
        formattedOutput=(char*)MEM_MALLOC(size);
    }
    
    // assume failure if formatted output not allocated
    if (!formattedOutput)
    {
        for (i=0; i<numArray; i++)
        {
            if (array[i]) 
            {
                memFree(array[i]);
            }
        }
        
        memFree(array);
        
        DBG_MSG("json_formatArray: formatted output failure\n");
        return NULL;
    }
    
    // generate formatted output for JSON array
    ptr=formattedOutput;
    *ptr='[';
    ptr++;
    *ptr=0;
    
    for (i=0; i<numArray; i++)
    {
        strcpy(ptr, array[i]);
        ptr += strlen(array[i]);
        
        if (i < numArray-1)
        {
            *ptr =',';
            ptr++;
        }
            
        *ptr=0;
        
        memFree(array[i]);
    }
    
    memFree(array);

    *ptr = ']';
    ptr++;
    *ptr = 0;
    
    return formattedOutput;    
}

char *json_parseValue(JSON *node, char *value)
{
    char *ptr = 0;
    
    if (node && value)
    {
        ptr = json_skip_whitespace(value);
        
        switch (*ptr)
        {
            case '{':
                ptr = json_parseOutObject(node, ptr);
                break;
                
            case '[':
                ptr = json_parseOutArray(node, ptr);
                break;
                
            case '\"':
                ptr = json_parseOutString(node, ptr);
                break;
                
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-':
                ptr = json_parseOutNumber(node, ptr);
                break;
            
            default:
            {
                if (strncasecmp(ptr, "null", 4) == 0)
                { 
                    node->valueType = JSON_T_NULL;  
                    ptr += 4;
                }
                else if (strncasecmp(ptr, "true", 4) == 0)
                { 
                    node->valueType = JSON_T_BOOLEAN;
                    node->value = json_strdup("true");
                    ptr += 4;
                }
                else if (strncasecmp(ptr, "false", 5) == 0)
                { 
                    node->valueType = JSON_T_BOOLEAN;
                    node->value = json_strdup("false");
                    ptr += 5;
                }
                else if ( (strncasecmp(ptr, "//", 2) == 0) || (strncasecmp(ptr, "/*", 2) == 0) )
                { 
                    // skip any comments
                    ptr = json_skip_comment(ptr);
                }
                else
                {
                    // no match
                    DBG_MSG("json_parseValue: no match\n");
                    ptr = 0;
                }
            }
        }
        
    }
    
    return ptr;
}

JSON *json_parseInput(char *string)
{
    JSON *node = 0;
    
    if (!string)
    {
        DBG_MSG("json_parseInput: invalid string\n");
        return NULL;
    }
    
    node = json_new();
    if (node)
    {
        if (!json_parseValue(node, string))
        {
            json_free(node);
            node = 0;
        }
    }
    
    return node;
}

char *json_formatOutput(JSON *node)
{
    char *value=0;
    
    if (!node)
    {
        DBG_MSG("json_formatOutput: invalid node\n");
        return NULL;
    }
    
    switch (node->valueType)
    {
        case JSON_T_OBJECT:    
            value = json_formatObject(node);
            break;
            
        case JSON_T_ARRAY:    
            value = json_formatArray(node);
            break;
            
        case JSON_T_STRING:    
            value = json_formatString(node);
            break;
            
        case JSON_T_NUMBER:    
            value = json_formatNumber(node);
            break;
            
        case JSON_T_BOOLEAN:    
            value = json_strdup(node->value); 
            break;
            
        case JSON_T_NULL:    
            value = json_strdup("null");    
            break;
        
        default:
            DBG_MSG("json_formatOutput: unsupported valueType\n");
            return NULL;
    }
    
    return value;
}

int  json_getArrayCount(JSON *node)
{
    JSON *child = (node)?node->child:0;
    int count=0;
    
    while(child)
    {
        count++;
        child = child->next;
    }
    
    return count;
}

JSON *json_getArray(JSON *node, int index)
{
    JSON *child = (node)?node->child:0;
    
    while(child && index > 0)
    {
        index--;
        child = child->next;
    }
    
    if (index > 0)
        child = NULL;
    
    return child;
}

error_type_t json_getArrayString(JSON *node, int index, char **string)
{
    error_type_t e_res = FAIL;

    *string = NULL;

    JSON *child = json_getArray(node, index);
    if (child)
    {
        *string = json_strdup(child->value);
        e_res = OK;
    }
    
    return e_res;
}

error_type_t json_getArrayNumber(JSON *node, int index, uint32_t *value)
{
    error_type_t e_res = FAIL;
    JSON *child = json_getArray(node, index);
    if (child)
    {
        DBG_MSG("json_getArrayNumber getting number %s\n", child->value);
        *value = json_strnum(child->value);
        e_res = OK;
    }
    
    return e_res;
}

error_type_t json_getArrayBool(JSON *node, int index, bool *value)
{
    error_type_t e_res = FAIL;
    JSON *child = json_getArray(node, index);
    if (child)
    {
        if (strncasecmp(child->value, "true", 4) == 0)
        { 
            *value = true;
            e_res = OK;
        }
        else if (strncasecmp(child->value, "false", 5) == 0)
        { 
            *value = false;
            e_res = OK;
        }
    }
    
    return e_res;
}

error_type_t json_insertStringIntoArray(JSON *node, int index, char *string)
{
    error_type_t e_res = FAIL;
    JSON *child = json_newString(string);
    if (child)
    {
        e_res = json_insertChildIntoArray(node, index, child);
    }
    
    return e_res;
}

error_type_t json_insertNumberIntoArray(JSON *node, int index, uint32_t number)
{
    error_type_t e_res = FAIL;
    JSON *child = json_newNumber(number);
    if (child)
    {
        DBG_MSG("json_insertNumberIntoArray setting number %u, %s\n", number, child->value);
        e_res = json_insertChildIntoArray(node, index, child);
    }
    
    return e_res;
}

error_type_t json_insertBoolIntoArray(JSON *node, int index, bool value)
{
    error_type_t e_res = FAIL;
    JSON *child = json_newBool(value);
    if (child)
    {
        e_res = json_insertChildIntoArray(node, index, child);
    }
    
    return e_res;
}

JSON *json_getObject(JSON *node, char *key)
{
    JSON *child = 0;

    if (key)
    {
        child = (node)?node->child:0;
        while(child && strcasecmp(child->key, key))
        {
            child = child->next;
        }
    }
    else
    {
        child = node;
    }
    
    return child;
}

error_type_t json_getObjectString(JSON *node, char *key, char **string)
{
    error_type_t e_res = FAIL;
    
    *string = NULL;
    
    JSON *child = json_getObject(node, key);
    if (child)
    {
        *string = json_strdup(child->value);
        e_res = OK;
    }
    
    return e_res;
}

error_type_t json_setObjectString(JSON *node, char *key, char *string)
{
    error_type_t e_res = FAIL;
    JSON *child = json_getObject(node, key);
    if (child)
    {
        if (child->valueType == JSON_T_STRING)
        {
            if (child->value)
                memFree(child->value);
                
            child->value = json_strdup(string);
            e_res = OK;
        }
    }
    else
    {
        e_res = json_insertChildObject(node, key, json_newString(string));
    }
    
    return e_res;
}

error_type_t json_getObjectNumber(JSON *node, char *key, uint32_t *value)
{
    error_type_t e_res = FAIL;
    JSON *child = json_getObject(node, key);
    if (child)
    {
        *value = json_strnum(child->value);
        e_res = OK;
    }
    
    return e_res;
}

error_type_t json_setObjectNumber(JSON *node, char *key, uint32_t value)
{
    error_type_t e_res = FAIL;
    JSON *child = json_getObject(node, key);
    if (child)
    {
        if (child->valueType == JSON_T_NUMBER)
        {
            if (child->value)
                memFree(child->value);
                
            char *str =  (char*)MEM_MALLOC(64);
            if (str)
            {
                minSprintf(str, "%u", value);
                child->value = str;
                e_res = OK;
            }
            else
            {
                child->value = NULL;
            }
            
        }
    }
    else
    {
        json_insertChildObject(node, key, json_newNumber(value));
    }
    
    return e_res;
}

error_type_t json_getObjectBool(JSON *node, char *key, bool *value)
{
    error_type_t e_res = FAIL;
    JSON *child = json_getObject(node, key);
    if (child)
    {
        if (strncasecmp(child->value, "true", 4) == 0)
        { 
            *value = true;
            e_res = OK;
        }
        else if (strncasecmp(child->value, "false", 5) == 0)
        { 
            *value = false;
            e_res = OK;
        }
    }
    
    return e_res;
}

error_type_t json_setObjectBool(JSON *node, char *key, bool value)
{
    error_type_t e_res = FAIL;
    JSON *child = json_getObject(node, key);
    if (child)
    {
        if (child->valueType == JSON_T_BOOLEAN)
        {
            if (child->value)
                memFree(child->value);
                
            if (value)
                child->value = json_strdup("true");
            else
                child->value = json_strdup("false");
                
            e_res = OK;
        }
    }
    else
    {
        e_res = json_insertChildObject(node, key, json_newBool(value));
    }
    
    return e_res;
}

JSON *json_extractArray(JSON *node, int index)
{
    JSON *child = (node)?node->child:0;
    
    while(child && index > 0)
    {
        index--;
        child = child->next;
    }
    
    if (child)
    {
        if (child->prev)
        {
            child->prev->next = child->next;
        }
            
        if (child->next)
        {
            child->next->prev = child->prev;
        }
            
        if (child == node->child)
        {
            node->child = child->next;
        }
            
        child->prev = 0;
        child->next = 0;
    }
    
    return child;
}

error_type_t json_removeArray(JSON *node, int index)
{
    error_type_t e_res = FAIL;
    
    JSON *child = json_extractArray(node, index);
    if (child)
    {
        json_free(child);
        e_res = OK;
    }
    
    return e_res;
}

JSON *json_extractObject(JSON *node, char *key)
{
    JSON *child = 0;
    
    if (key)
    {
        child = (node)?node->child:0;
        
        while(child && strcasecmp(child->key, key))
        {
            child = child->next;
        }
    }
    else
    {
        child = node;
    }
    
    if (child)
    {
        if (child->prev)
        {
            child->prev->next = child->next;
        }
            
        if (child->next)
        {
            child->next->prev = child->prev;
        }
            
        if (child == node->child)
        {
            node->child = child->next;
        }
            
        child->prev = 0;
        child->next = 0;
    }
    
    return child;
}

error_type_t json_removeObject(JSON *node, char *key)
{
    error_type_t e_res = FAIL;
    
    JSON *child = json_extractObject(node, key);
    if (child)
    {
        json_free(child);
        e_res = OK;
    }
    
    return e_res;
}

error_type_t json_insertChild(JSON *node, JSON *insert)
{
    error_type_t e_res = FAIL;
    if (insert)
    {
        JSON *child = (node)?node->child:0;
        
        while (child && child->next) 
        {
            child = child->next; 
        }
        
        if (child) 
        {
            child->next = insert;
            insert->prev=child;
        } 
        else 
        {
            node->child=insert;
        }
        
        e_res = OK;
    }
    
    return e_res;
}

error_type_t json_insertChildObject(JSON *node, char *key, JSON *insert)
{
    error_type_t e_res = FAIL;
    if (insert)
    {
        if (insert->key)
        {
            memFree(insert->key);
        }
        
        insert->key = json_strdup(key);
        
        e_res = json_insertChild(node, insert);
    }
    
    return e_res;
}

error_type_t json_insertChildIntoArray(JSON *node, int index, JSON *insert)
{
    error_type_t e_res = FAIL;
    if (insert)
    {
        JSON *child = (node)?node->child:0;
        
        while (child && child->next && index > 0) 
        {
            index--;
            child = child->next; 
        }
        
        if (index > 0)
        {
            if (child)
                index--;
                
            while (index > 0)
            {
                JSON *placeHolder = json_newNull();
                                
                DBG_MSG("json_insertChildIntoArray adding null child\n");
                if (child) 
                {
                    child->next = placeHolder;
                    placeHolder->prev = child;
                } 
                else 
                {
                    node->child = placeHolder;
                }
                
                child = placeHolder;
                index--;
            }
            
            if (child) 
            {
                DBG_MSG("json_insertChildIntoArray adding child to tail\n");
                if (child->next)
                {
                    child->next->prev = insert;
                }
                
                insert->next = child->next;
                child->next = insert;
                insert->prev = child;
            } 
            else 
            {
                DBG_MSG("json_insertChildIntoArray adding child to beginning\n");
                node->child = insert;
            }
        }
        else
        {
            if (child) 
            {
                DBG_MSG("json_insertChildIntoArray adding child ahead\n");
                if (child->prev)
                {
                    child->prev->next = insert;
                }
                
                insert->prev = child->prev;
                child->prev = insert;
                insert->next = child;

                if (child == node->child)
                {
                    node->child = insert;
                }
            } 
            else 
            {
                DBG_MSG("json_insertChildIntoArray adding child to beginning\n");
                node->child = insert;
            }
        }
        
        e_res = OK;
    }
    
    return e_res;
}
