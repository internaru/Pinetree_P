/*
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file pjl_enum_objs.c
 *
 * \brief
 *
 */

#include <stdint.h>
#include <string.h>
#include "pjl_api.h"
#include "memAPI.h"
#include "zPJL.h"
#include "zPJL_sym.h"
#include "zPJL_test.h"
#include "list.h"
#include "lassert.h"


extern WORD_MATCH *pwm_OnOff, *pwm_OnOffAuto;
struct pjl_object_s
{
//    ATLISTENTRY list_val;
    int32_t pjl_value;
    char *pjl_string_Value;
    WORD_MATCH *match_item;
    int32_t     max;
    int32_t     min;
    int32_t     increment;

};

// static ATLISTENTRY pjl_objects;

int32_t pjl_get_val(pjl_object_t *obj_addr)
{
    pjl_object_t *CurObject;

    CurObject = (pjl_object_t *) obj_addr;
    return CurObject->pjl_value;
}

char *pjl_get_string_val(pjl_object_t *obj_addr)
{
    pjl_object_t *CurObject;
    char *tempstr;

    CurObject = (pjl_object_t *) obj_addr;
    if(CurObject->pjl_string_Value == NULL)
        return NULL;
    tempstr = MEM_MALLOC(strlen(CurObject->pjl_string_Value));
    if(tempstr != NULL)
        strcpy(tempstr, CurObject->pjl_string_Value);
    return tempstr;
}

error_type_t pjl_set_val(pjl_object_t *obj_addr, int32_t Value)
{
    pjl_object_t *CurObject;
    uint32_t i;

    CurObject = (pjl_object_t *) obj_addr;
    if((Value < CurObject->min) ||
       (Value > CurObject->max))
        return FAIL;
    i = 0;
    while((Value != (CurObject->min + i * CurObject->increment)) &&
          ((CurObject->min + i * CurObject->increment) <= CurObject->max)) i++;
    if(CurObject->max < (CurObject->min + i * CurObject->increment))
       return FAIL;     // invalid value
    CurObject->pjl_value = Value;
    CurObject->pjl_string_Value = SYM(CurObject->match_item[i].key);
    return OK;

}
error_type_t pjl_set_string_val(pjl_object_t *obj_addr, char *value)
{
    uint32_t i;
    int32_t n;
    pjl_object_t *CurObject;

    CurObject = (pjl_object_t *) obj_addr;

    i = 0;
    while((CurObject->match_item[i].key != 0xffff) &&
          (strcasecmp(value, SYM(CurObject->match_item[i].key)) != 0)) i++;
    if(CurObject->match_item[i].key == 0xffff)
        return FAIL;
    CurObject->pjl_string_Value = SYM(CurObject->match_item[i].key);
    n = CurObject->min;
    for(;i > 0; i--)
        n += CurObject->increment;
    CurObject->pjl_value = n;
    return OK;
}

pjl_object_t *pjl_enum_open(WORD_MATCH *match_item, int32_t min, int32_t increment)
{
    pjl_object_t *Object;
    int32_t i, n;
  
    Object = MEM_CALLOC(1,sizeof(pjl_object_t));
    if(Object == NULL)
        return NULL;
    Object->match_item = match_item;
    Object->min = min;
    Object->increment = increment;
    i=0;
    n = min;
    while(match_item[++i].key != 0xffff) 
        n = n + increment;
    Object->max = n;
    return Object;
}

void pjl_enum_close(pjl_object_t *enum_object)
{
    pjl_object_t *CurObject;

    CurObject = (pjl_object_t *) enum_object;
    memFree(CurObject);
}


#ifdef HAVE_UNIT_TEST
int pjlunittests(void)
{
    int32_t val;
    char *TempString;
    pjl_object_t *TempObject, *tempObj;
    error_type_t status;

    TempObject = pjl_enum_open(pwm_OnOff, 0,1);
    if(TempObject == NULL)
        return __LINE__;
    pjl_set_val(TempObject, 1);      // should be on
    TempString = pjl_get_string_val(TempObject);
    if(TempString == NULL)
        return __LINE__;
    if(strcasecmp("ON", TempString) != 0)
        return __LINE__;
    memFree(TempString);
    val = pjl_get_val(TempObject);
    if(val != 1)
        return __LINE__;
    pjl_set_string_val(TempObject, "OFf");
    val = pjl_get_val(TempObject);
    if(val != 0)
        return __LINE__;
    pjl_set_string_val(TempObject, "on");
    val = pjl_get_val(TempObject);
    if(val != 1)
        return __LINE__;

    tempObj = pjl_enum_open(pwm_OnOffAuto, 0,1);
    if(tempObj == NULL)
        return __LINE__;
    pjl_set_val(tempObj,1);      // should be on
    TempString = pjl_get_string_val(tempObj);
    if(TempString == NULL)
        return __LINE__;
    if(strcasecmp("ON", TempString) != 0)
        return __LINE__;
    memFree(TempString);
    val = pjl_get_val(tempObj);
    if(val != 1)
        return __LINE__;
    pjl_set_string_val(tempObj, "OFf");
    val = pjl_get_val(tempObj);
    if(val != 0)
        return __LINE__;
    pjl_set_string_val(tempObj, "on");
    val = pjl_get_val(tempObj);
    if(val != 1)
        return __LINE__;
    status = pjl_set_string_val(tempObj, "auto");
    if(status != OK)
        return __LINE__;
    val = pjl_get_val(tempObj);
    if(val != 2)
        return __LINE__;
    status = pjl_set_string_val(tempObj, "foo");
    if(status != FAIL)
        return __LINE__;
    val = pjl_get_val(tempObj);
    if(val != 2)
        return __LINE__;
    status = pjl_set_val(tempObj, 5);
    if(status != FAIL)
        return __LINE__;
    TempString = pjl_get_string_val(tempObj);
    if(strcasecmp("AUTO", TempString) != 0)
        return __LINE__;
    memFree(TempString);

    

    pjl_enum_close(TempObject);
    pjl_enum_close(tempObj);
    TempObject = pjl_enum_open(pwm_OnOffAuto, 5, 2);
    if(TempObject == NULL)
        return __LINE__;
    status = pjl_set_string_val(TempObject, "auto");
    if(status != OK)
        return __LINE__;
    val = pjl_get_val(TempObject);
    if(val != 9)
        return __LINE__;
    status = pjl_set_val(TempObject, 6);
    if(status != FAIL)
        return __LINE__;
    status = pjl_set_val(TempObject, 7);
    if(status != OK)
        return __LINE__;
    TempString = pjl_get_string_val(TempObject);
    if(strcasecmp("ON", TempString) != 0)
        return __LINE__;
    memFree(TempString);

    pjl_enum_close(TempObject);
    
    return OK;
}

#endif
