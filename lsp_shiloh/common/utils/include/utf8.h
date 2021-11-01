/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

#ifndef _UTF8_H_
#define _UTF8_H_
#include <stdbool.h>
#include "ATypes.h"


#ifdef __cplusplus
extern "C" {
#endif
//typedef unsigned short UINT16;
//typedef unsigned long UINT32;
//typedef unsigned char UINT8;

#define MAX_UTF8_BYTES_PER_CHAR 4  // Number of storage bytes to reserve for each UTF8 character.

int UTF8ToU32(UINT32 *str_U32, UINT8 *str_UTF8);
int UTF8NToU32(UINT32 *str_U32, const UINT8 *str_UTF8, int maxChars);
int U32ToUTF8(UINT8 *str_UTF8, UINT32 *str_U32); 
int U32NToUTF8(UINT8 *str_UTF8, UINT32 *str_U32, int maxChars); 
int UTF8StrLen( UINT8 *str_UTF8);
int UTF8StrNCpy( UINT8 *str_UTF8_out, const UINT8 *str_UTF8_in, int maxChars );
bool SanitizeUTF8String(UINT8 *str_UTF8);

#ifdef __cplusplus
}
#endif

#endif
