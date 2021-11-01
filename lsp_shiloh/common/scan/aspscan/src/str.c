/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "err.h"
#include "str.h"

int str_split( char *string, char *pattern, char *ptrList[], int ptrListLen )
{
    int cnt;
    int state;

//    assert( string != NULL );
//    assert( pattern != NULL );
    if( string==NULL || pattern==NULL ) {
        return ERR_FAIL;
    }

#define IS_VALID 0
#define IS_SPLIT 1

    if( strchr( pattern, *string ) ) {
        // string starts with a split character 
        state = IS_SPLIT;
        cnt = 0;
    }
    else {
        // string starts with a valid character 
        state = IS_VALID;
        ptrList[0] = string;
        cnt = 1;
    }

    while( *string ) {
        if( strchr( (char *)pattern, *string ) ) {
            // split character 

            if( state == IS_VALID ) {
                // state transition from reading a string to split state
                // terminate last string
                *string = 0;
                state = IS_SPLIT;
            }
        }
        else {
            // valid character
            if( state == IS_SPLIT ) {
                // state transition from a split state to reading a valid string
                // start a new string
                state = IS_VALID;
                
                // not enough space given to us to finish the job 
                // so just return what we can and be happy
                if( cnt == ptrListLen ) {
                    return cnt;
                }

                ptrList[cnt] = string;
                cnt++;
            }
        }
                
        string++;
    }

#undef IS_VALUE
#undef IS_SPLIT
    
    return cnt;
}

void str_chomp( char *buf, int *buflen )
{
    assert( buf != NULL );
    assert( buflen != NULL );

    /* kill end of line character(s) */

    /* kills \r\n (0x0d0x0a) or \n (0xa) */

    /* assumes buflen-1 is last byte of buf */
    /* (ie, buffer goes from 0..buflen-1) */

    /* stupid human check */
    if( *buflen == 0 ) {
        return;
    }

    /* convert from string length to index */
    *buflen -= 1;

    while( buf[*buflen] == 0x0a || buf[*buflen] == 0x0d ) {
        buf[*buflen] = 0;
        if( *buflen==0 ) {
            return;
        }
        *buflen -= 1;
    }
    /* back to strlen() instead of index */
    *buflen += 1;
}

int
str_mkint( const char *str, int len, unsigned long int *value )
{
    char *endptr;
    unsigned long int num32;
    
    /* max length strlen("4294967295")==10, strlen("0xffffffff")==10 */
    *value=0;
    if( len==0 || len>10 || str==NULL ) {
        return -1;
    }

    /* if we have a '0x', try hex */
    if( len > 2 && str[0]=='0' && (str[1]=='x'||str[1]=='X') ) {
        num32 = strtoul( str, &endptr, 16 );
        if( *endptr != 0 ) {
            return -1;
        }
    }
    else { 
        /* otherwise, assume base 10 */
        num32 = strtoul( str, &endptr, 10 );
        if( *endptr != 0 ) {
            return -1;
        }
    }

    *value = num32;
    return 0;
}

/**
 * \brief Match two strings.
 *
 *  Exactly match two strings. Create because strncmp() can match substrings,
 *  e.g., strncmp( "hell", "hello", 4 ) matches.
 *
 * Test/example code:
 *  assert( str_match( "hello", "hello", 5 ) );
 *  assert( !str_match( "hello", "hello", 4 ) );
 *  assert( !str_match( "hello", "hello", 6 ) );
 *  assert( !str_match( "hello", "hello", 7 ) );
 *  assert( !str_match( "hell", "hello", 4 ) );
 *  assert( !str_match( "hello", "hell", 5 ) );
 *  assert( str_match( NULL, NULL, 0 ) );
 *  assert( !str_match( NULL, NULL, 10 ) );
 *  assert( !str_match( "foo", NULL, 3 ) );
 *  assert( !str_match( "foo", NULL, 0 ) );
 *  assert( !str_match( NULL, "FOOBAR", 6 ) );
 *  assert( !str_match( NULL, "FOOBAR", 0 ) );
 *  char *s = "world";
 *  assert( str_match( s, s, 5 ) );
 *
 * If either (but not both) of the incoming strings are NULL -> no match
 *
 * If both NULL,
 *  (NULL, NULL,  0) -> match
 *  (NULL, NULL, !0) -> no match
 *
 * Two empty, zero length strings should intuitively match.
 * Two empty strings required to match a number of chars should intitively
 * not match.
 * 
 * \author David Poole
 * \date 22-Sep-2010
 *
 */

bool str_match( const char *s1, const char *s2, int chars_to_match ) 
{
    if( s1==NULL || s2==NULL ) {
        if( s1==NULL && s2==NULL && chars_to_match==0 ) {
            return true;
        }
        else {
            return false;
        }
    }

    while( chars_to_match && *s1 && *s2 ) {
        if( *s1 != *s2 ) {
            return false;
        }

        chars_to_match--;
        s1 += 1;
        s2 += 1;
    }

    /* the strings, in their entirety, have to match */
    if( ! (*s1==0 && *s2==0 && chars_to_match==0) ) {
        return false;
    }

    return true;
}

