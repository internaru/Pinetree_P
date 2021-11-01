/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>

#include <linux/module.h> 
#include <linux/kernel.h> 

#include "utils.h"

/**************************************************
 * Function name   : str_split
 *    returns      : int - number of tokens found in string
 *    src          : (in) 
 *    srclen       : (in)
 *    tokens[]     : (out)
 *    tokenslen    : (in) number of elements available in tokens[] array
 *    delims       : (in) null terminated list of delimiters to use in parsing
 *                      src string
 * Created by      : David Poole
 * Date created    : 9-Feb-05
 * Description     : 
 *
 *  Splits a null terminated string into tokens based on characters in delims.
 *
 *  For example, 
 *      ptr = strdup( "This is a test");
 *      str_split( ptr, strlen(ptr), tokens, 10, " \t" );
 *  will return
 *      tokens[0] = "This"
 *      tokens[1] = "is"
 *      tokens[2] = "a"
 *      tokens[3] = "test"
 *      tokens[4] = NULL
 *
 *  Another example,
 *      ptr = strdup( "10,20,   30, 40, 50");
 *      str_split( ptr, strlen(ptr), tokens, 10, " ," );
 *  or
 *      ptr = strdup( "10:20:30:40:50");
 *      str_split( ptr, strlen(ptr), tokens, 10, " :" );
 *  will return
 *      tokens[0] = "10"
 *      tokens[1] = "20"
 *      tokens[2] = "30"
 *      tokens[3] = "40"
 *      tokens[4] = "50"
 *      tokens[5] = NULL
 *
 *
 *  Any tokens past tokenslen-1 (always leaves an extra space for NULL) will be
 *  silently ignored.
 *
 *
 * BUGS:
 *  Doesn't handle any sort of character escapes.
 *  "This\ is a test" comes out as tokens[0]="This\" NOT tokens[0]="This is"
 *
 *
 * Notes           : 
 *
 *  >>DESTRUCTIVE FUNCTION<< Pokes NULLs into src and fills tokens[] with
 *  pointers into src. Don't send a static string (e.g., 
 *      str_split( "this is a test", ... )
 *
 *  Beware of freeing strings once you have the tokens[] array. tokens[] points
 *  into the source string. 
 *
 *  Beware of passing strings on the stack then returning from the function and
 *  continuing to use tokens[] which now point onto a dead stack frame.
 *
 **************************************************/

int str_split( char *src, int srclen, char *tokens[], int tokenslen, const char *delims )
{   
    char *ptr, *endptr;
    int state;
    int cnt;

    ptr = src;
    endptr = &src[srclen];
    cnt = 0;

#define IN_DELIM 1
#define IN_TOKEN 2

    state = IN_DELIM;

    while( ptr < endptr ) {

        /* is this character one of the separators? */
        if( strchr( delims, *ptr ) ) {
            if( state==IN_TOKEN ) {
                /* end of current token; drop a NULL termination */
                *ptr = 0;
                state = IN_DELIM;
            }
        }
        else {
            if( state==IN_DELIM ) {
                /* start of new token; save a pointer and start searching for
                 * the next delimiter
                 * -1 to save space for a final NULL entry after the final
                 * token (like argv[])
                 */
                if( cnt < tokenslen-1 ) {
                    tokens[cnt] = ptr;
                    cnt++;
                }
                state = IN_TOKEN;
            }
        }

        ptr++;

    } /* end while(ptr<endptr) */

#undef IN_DELIM
#undef IN_TOKEN

    /* final NULL entry */
    tokens[cnt] = NULL;

    return cnt;
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

/**************************************************
 * Function name   : str_mkint
 *    returns      : 0 - success
 *                   -1 - bad integer
 *    str          : NULL terminated string containing decimal or hex integer
 *                      to convert to uint32_t 
 *    num32        : destination of string->integer converstion 
 * Created by      : David Poole
 * Date created    : Feb 2005
 * Description     : 
 *
 *  Convert a NULL terminated string to a uint32_t. If the first chars of the
 *  string are '0x', tries to convert as hex. Otherwise, tries to convert as
 *  decimal.
 *
 *  Essentially this is a convenient wrapper around strtoul().
 *
 * Notes           : 
 **************************************************/

int str_mkint( const char *str, uint32_t *num32 ) 
{
    int retcode;
    int len;
    unsigned long tmp;

    if( str == NULL ) {
        goto fail;
    }

    len = strlen(str);
    /* 32-bit number, strlen("4294967295")==10 */
    if( len==0 || len > 10 ) {
        goto fail;
    }

    if( len>2 && str[0] == '0' && str[1] == 'x' ) {
        /* assume hex */
        retcode = kstrtoul( str, 16, &tmp );
        if( retcode != 0 ) {
            /* bad integer */
            *num32 = 0;
            goto fail;
        }
        *num32 = (uint32_t)tmp;
    }
    else {
        /* assume decimal */
        /* davep 01-Apr-2013 ; handle leading '-' (siqfiles) */
        if( str[0]=='-') {
            retcode = kstrtol( str, 10, &tmp );
        }
        else {
            retcode = kstrtoul( str, 10, &tmp );
        }
        if( retcode != 0 ) {
            /* bad integer */
            *num32 = 0;
            goto fail;
        }
        *num32 = (uint32_t)tmp;
    }

    return 0;

fail:
    return -1;
}

/**************************************************
 * Function name   : str_tr
 *    returns      : none
 *    str          : string to operate upon
 *    from         : chars to translate
 *    to           :  
 *    len          : length of source str
 * Created by      : 
 * Date created    : 
 * Description     : 
 *
 *  Like UNIX 'tr', str_tr() "translates" a set of source characters to a set
 *  of other characters.
 *
 *  from[0] -> to[0]
 *  from[1] -> to[1]
 *  ...
 *  from[n] -> to[n]
 *
 *  It's a one-to-one mapping from the chars in 'from' to the same location in
 *  'to'.
 *
 *  For example:
 *      str = strdup( "hello, world" );
 *      str_tr( str, "aeiou", "AEIOU", strlen(str) );
 *      // str now "hEllO, wOrld"
 *
 *  Why is this useful you ask? I needed to create a PGM header in the Scan
 *  module. PGM wants its fields separated by linefeeds. Our sprintf() will
 *  only do \r\n (0x0d0a).
 *
 *      minSprintf( buf, "P5.%d.%d.32768.", pixels, lines );
 *      str_tr( buf, ".", "\012", strlen(buf) );
 *
 * Notes           : 
 *
 *  This function could probably be implemented much, much faster.
 *
 **************************************************/

void str_tr( char *str, const char *from, const char *to, int len )
{
    int i, j;
    int trlen;

    /* only translate as many chars from the smaller of the two */
    trlen = MIN( strlen(from), strlen(to) );

    /* yes, this is an O(N^2) algorithm */
    for( i=0 ; i<len ; i++ ) {
        for( j=0 ; j<trlen ; j++ ) {
            if( str[i] == from[j] ) {
                str[i] = to[j];
                break;
            }
        }
    }
}

/** 
 * \brief Remove the blank character from the beginning and end 
 * of a string 
 * 
 * \param string 
 * 
 * \return The string has been trimmed
 * 
 **/

/* davep 18-Oct-2012 ; no ctypes.h in linux kernel */
#define xxx_isblank(c) ((c)==' '||(c)=='\t')

char * ltrim(char * string)
{
    int     len;
    int     i;          //general loop variable

    if(NULL != string)
    {
        for(i = 0, len = strlen(string); i < len; i++)
        {
            if(0 == xxx_isblank(string[i]))
            {
                break;
            }
        }

        if(i == 0)
        {
            //
            //There is no space character at the beginning of the string
            //
            //do nothing
            //
        }
        else if(i == len)
        {
            //
            //There is no non-space character in the string
            //
            string[0] = '\0';
        }
        else
        {
            memmove(string, string + i, len - i + 1);       // + 1 for include NULL
        }
    }
    return  string;
}

char * rtrim(char * string)
{
    int     i;          //general loop variable

    if(NULL != string)
    {
        for(i = strlen(string) - 1; i >= 0; i--)
        {
            if(0 == xxx_isblank(string[i]))
            {
                break;
            }
        }

        string[i + 1] = '\0';
    }
    return  string;
}

char * trim(char * string)
{
    return  rtrim(ltrim(string));
}


