/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef STR_H
#define STR_H

int str_split( char *string, char *pattern, char *ptrList[], int ptrListLen );
void str_chomp( char *buf, int *buflen );
int str_mkint( const char *str, int len, unsigned long int *value );
bool str_match( const char *s1, const char *s2, int chars_to_match );

#ifndef min
#define min(x,y)  ((x)<(y)?(x):(y))
#endif

#ifndef max
#define max(x,y)  ((x)>(y)?(x):(y))
#endif

#endif

