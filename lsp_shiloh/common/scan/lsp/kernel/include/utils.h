/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 * Clone-ish of common OBA code utils.h
 */

#ifndef UTILS_H
#define UTILS_H

#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#endif

int str_mkint( const char *str, uint32_t *num32 );
bool str_match( const char *s1, const char *s2, int chars_to_match ) ;
int str_split( char *src, int srclen, char *tokens[], int tokenslen, const char *delims );
void str_tr( char *str, const char *from, const char *to, int len );
char * trim(char * string);

#endif

