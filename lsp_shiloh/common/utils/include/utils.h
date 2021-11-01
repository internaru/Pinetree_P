/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file utils.h 
 *
 * \brief This file contains public utils functions.
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int str_split( char *src, int srclen, char *tokens[], int tokenslen, const char *delims );
int tokenize_escaped_str( char *src, char *tokens[], int tokenslen );
void str_tr( char *str, const char *from, const char *to, int len );
int str_mkint( const char *str, uint32_t *num32 );
int str_mksint( const char *str, int32_t *num32 );
unsigned char char_to_num( unsigned char c );
bool str_match( const char *s1, const char *s2, int chars_to_match );
void str_chomp( char *buf, int *buflen );
int hex_str_to_raw( char *str, char *raw, int max_len );
bool buff_is_all_zero( uint8_t *data, int len );
char * str_dup(const char *s);
char * trim(char * string);
char * ltrim(char * string);
char * rtrim(char * string);
char * chop_enter_chars(char * string);
char *strstri( char *haystack, char *needle );
char *strnstri( char *haystack, char *needle, uint32_t str_size_in_bytes );


#define MICROSEC	1
#define MILLISEC	1000
#define KILO    1024
#define NUM_CRC_ENTRIES 8

/** \brief util to determine Array Size. */
#define ARRAY_SIZE( arrayName ) ( sizeof( arrayName ) / sizeof( arrayName[0] ) )

/*
 * Trick for getting line and file into a string from sources on the web.
 * By referencing this way it get the file and string into a string.
 */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define WHERE_AT __FILE__ ":" TOSTRING(__LINE__)

#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#endif


/**
 * 
 * \brief Initialize any utils internals.
 *
 */
void utils_init( void );


#ifdef __cplusplus
}
#endif

#endif

