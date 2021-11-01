/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/


#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

#include "utils.h"
#include "memAPI.h"
#include "lassert.h"

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
    tokens[cnt] = 0;

    return cnt;
}

int tokenize_escaped_str( char *src, char *tokens[], int tokenslen )
{
    int   token_cnt = 0;
    bool  in_token  = false;
    char *read_pos  = src;
    char *write_pos = src;
    char  delimiter = ' ';

    while (*read_pos != '\0')
    {
        if (*read_pos == delimiter)
        {
            if (in_token)
            {
                in_token = false;
        
                *(write_pos++) = 0;
        
                if ((token_cnt + 1) >= tokenslen)
                {
                    // Don't accept any more tokens after this one -- just stop parsing
                    break;
                }

                // Go back to the default delimiter
                delimiter = ' ';
            }
        }
        else
        {
            if (!in_token)
            {
                in_token = true;
                tokens[token_cnt++] = write_pos;

                if (*read_pos == '"' || *read_pos == '\'')
                {
                    delimiter = *(read_pos++);
                    continue;
                }
                else
                {
                    delimiter = ' ';
                }
            }

            if ((*read_pos == '\\') && (delimiter != ' '))
            {
                read_pos++;
                if (*read_pos == '\0')
                {
                    *(write_pos++) = '\\';
                    break;
                }
                else
                {
                    *(write_pos++) = *read_pos;
                }
            }
            else
            {
                *(write_pos++) = *read_pos;
            }
        }

        read_pos++;
    }
    *write_pos = 0;
    tokens[token_cnt] = 0;

    return token_cnt;
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
    char *errptr;
    int len;

    if( str == 0 ) {
        goto fail;
    }

    len = strlen(str);
    /* 32-bit number, strlen("4294967295")==10 */
    if( len==0 || len > 10 ) {
        goto fail;
    }

    if( len>2 && str[0] == '0' && str[1] == 'x' ) {
        /* assume hex */
        *num32 = strtoul( str, &errptr, 16 );
        if( *errptr != 0 ) {
            /* bad integer */
            *num32 = 0;
            goto fail;
        }
    }
    else {
        /* assume decimal */
        *num32 = strtoul( str, &errptr, 10 );
        if( *errptr != 0 ) {
            /* bad integer */
            *num32 = 0;
            goto fail;
        }
    }

    return 0;

fail:
    return -1;
}

/**************************************************
 * Function name   : str_mksint
 *    returns      : 0 - success
 *                   -1 - bad integer
 *    str          : NULL terminated string containing decimal or hex integer
 *                      to convert to uint32_t 
 *    num32        : destination of signed string->integer converstion 
 * Created by      : Bryan Allen
 * Date created    : June 2011
 * Description     : 
 *
 *  This function is identical to the above function, but it handles
 *  signed integer parsing.
 *
 *  Convert a NULL terminated string to a int32_t. If the first chars of the
 *  string are '0x', tries to convert as hex. Otherwise, tries to convert as
 *  decimal.
 *
 *  Essentially this is a convenient wrapper around strtol().
 *
 * Notes           : 
 **************************************************/

int str_mksint( const char *str, int32_t *num32 ) 
{
    char *errptr;
    int len;

    if( str == 0 ) {
        goto fail;
    }

    len = strlen(str);
    /* 32-bit number, strlen("4294967295")==10 */
    if( len==0 || len > 10 ) {
        goto fail;
    }

    if( len>2 && str[0] == '0' && str[1] == 'x' ) {
        /* assume hex */
        *num32 = strtoul( str, &errptr, 16 );
        if( *errptr != 0 ) {
            /* bad integer */
            *num32 = 0;
            goto fail;
        }
    }
    else {
        /* assume decimal */
        *num32 = strtol( str, &errptr, 10 );
        if( *errptr != 0 ) {
            /* bad integer */
            *num32 = 0;
            goto fail;
        }
    }

    return 0;

fail:
    return -1;
}

/**
 * \brief convert a hex character [0-9a-f] to integer 
 *
 * \retval integer value of character ; 0 (zero) if invalid char
 *
 * Using 0 as return for invalid as a "least damage" principle and to act a bit
 * like atoi().
 *
 * \author David Poole
 * \date 02-May-2007
 *
 */

unsigned char char_to_num( unsigned char c )
{
    static const char chars[] = "0123456789abcdef";
    int i;

    c = tolower( c );
    for( i=0 ; i<16 ; i++ ) {
        if( c==chars[i] ) {
            return i;
        }
    }
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
    if( s1==0 || s2==0 ) {
        if( s1==0 && s2==0 && chars_to_match==0 ) {
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

/** 
 * \brief Convert a hex string written in ASCII to a raw data
 * buffer
 * 
 * \param str - string to decode
 * \param raw - buffer to hold converted bytes
 * \param max_len - max length of the raw buffer
 * 
 * \return int: number of bytes in raw buffer
 * 
 * Converts a hex string to raw bytes.  For example, the string
 * "DEADBEEF" would become {0xDE, 0xAD, 0xBE, 0xEF} and the
 * return value would be 4.  Non-hex characters are ignored
 * ("DE:AD:BE:EF" would produce the same result.)
 * 
 **/
int hex_str_to_raw( char *str, char *raw, int max_len )
{
    int len   = 0;
    int shift = 0x01;

    do {
        if (isxdigit(*str))
        {
            if (shift)
            {
                *raw = char_to_num(*str) << 4;
            }
            else
            {
                *raw |= char_to_num(*str);
                ++raw;
                ++len;
            }
            shift ^= 0x01;
        }
    } while ((*++str != '\0') && (len < max_len));

    return len;
}

/** 
 * \brief Check to see if all bytes in the buffer are null
 * (0x00) bytes
 * 
 * \param data - buffer to check
 * \param len - length of buffer
 * 
 * \return bool: true if all bytes were 0x00
 * 
 **/
bool buff_is_all_zero( uint8_t *data, int len )
{
    while(len > 0)
    {
        if (*data != 0)
        {
            return false;
        }

        data++;
        len--;
    }

    return true;
}

/** 
 * \brief Return duplicate null terminated string allocated with
 *        MEM_MALLOC.
 * 
 * \param s - string to duplicate
 *
 * \return char* - duplicate
 * 
 **/
char * str_dup(const char *s)
{
    char *result; 
    result = (char*)MEM_MALLOC(strlen(s) + 1);
    if (result == (char*)0)
        return (char*)0;
    strcpy(result, s);
    return result;
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
char * trim(char * string)
{
    return  rtrim(ltrim(string));
}

char * ltrim(char * string)
{
    int     len;
    int     i;          //general loop variable

    if(NULL != string)
    {
        for(i = 0, len = strlen(string); i < len; i++)
        {
            if(0 == isblank(string[i]))
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
            if(0 == isblank(string[i]))
            {
                break;
            }
        }

        string[i + 1] = '\0';
    }
    return  string;
}


/** 
 * \brief Remove the '\r' and '\n' from the end of a string 
 * 
 * \param string 
 * 
 * \return
 * 
 **/
char * chop_enter_chars(char * string)
{
    int     i;      //general loop variable

    if(NULL != string)
    {
        i = strlen(string);
        if(string[i - 1] == '\r' || string[i - 1] == '\n')
        {
            string[i - 1] = '\0';
        }
        if(i > 1 && (string[i - 2] == '\r' || string[i - 2] == '\n'))
        {
            string[i - 2] = '\0';
        }
    }
    return  string;
}

/** 
 * \brief Case insensitive string compare
 *
 * \param val1 - first string
 * 
 * \param val2 - second string
 *
 * \return success/failure
 *
 * Compares two simple ASCII chars and determines if they are
 * equal. Case insensitive comparison.
 * 
 **/
bool charsEquali( char val1, char val2 )
{
    if ( val1 == val2 ) return true;
    if ( val1 >= 'a' && val1 <= 'z' ) val1 = ( val1 - 'a' ) + 'A';
    if ( val2 >= 'a' && val2 <= 'z' ) val2 = ( val2 - 'a' ) + 'A';
    return val1 == val2;
}


/** 
 * \brief Case insensitive substring search
 *
 * \param haystack - string to search
 * 
 * \param needle - substring to search for
 *
 * \return - pointer to start of substring
 *
 * Find one string inside another.  Case insensitive for lower
 * ASCII.
 * 
 **/
char *strstri( char *haystack, char *needle )
{
    char *cmpHaystack, *cmpNeedle;

    if ( haystack == NULL || needle == NULL )
    {
        return NULL;
    }

    while ( *haystack != '\0' )
    {
        if ( charsEquali( *haystack, *needle ) )
        {
            // Initial match -- see if this is the entire string
            cmpHaystack = haystack;
            cmpNeedle = needle;
            while ( ( *cmpHaystack != '\0' ) && ( *cmpNeedle != '\0' ) && charsEquali( *cmpHaystack, *cmpNeedle ) )
            {
                cmpHaystack++;
                cmpNeedle++;
            }
            if ( *cmpNeedle == '\0' )
            {
                // Match!
                return haystack;
            }
        }

        haystack++;
    }

    // Didn't find it
    return NULL;
}

/** 
 * \brief Case insensitive substring search
 *
 * \param haystack - string to search, must NOT be NULL
 * 
 * \param needle - substring to search for, must NOT be NULL
 *
 * \param str_size_in_bytes - the number of characters to look at in the
 * haystack, must not be 0
 *
 * \return - pointer to start of substring, or NULL if the
 * needle was not found in the haystack
 *
 * Find one string inside another.  Case insensitive for lower
 * ASCII.
 * This function looks at the first str_size_in_bytes characters but never
 * beyond haystack + str_size_in_bytes.  Also will not look beyond the
 * NULL-terminating character in either input string
 *
 **/
char *strnstri( char *haystack, char *needle, uint32_t str_size_in_bytes )
{
    char *cmpHaystack;
    char *cmpNeedle;
    bool continue_searching;
    uint32_t counter;

    ASSERT( NULL != haystack );
    ASSERT( NULL != needle );
    ASSERT( NULL != needle );

    counter = 0;

    if ( ( ( counter + strlen( needle ) ) > str_size_in_bytes ) ||
         ( *haystack == '\0' ) )
    {
        continue_searching = false;
    }
    else
    {
        continue_searching = true;
    }

    while ( true == continue_searching )
    {
        if ( charsEquali( *haystack, *needle ) )
        {
            // Initial match -- see if this is the entire string
            cmpHaystack = haystack;
            cmpNeedle = needle;
            while ( ( *cmpHaystack != '\0' ) && ( *cmpNeedle != '\0' ) && charsEquali( *cmpHaystack, *cmpNeedle ) )
            {
                cmpHaystack++;
                cmpNeedle++;
            }
            if ( *cmpNeedle == '\0' )
            {
                // Match!
                return haystack;
            }
        }

        counter++;
        haystack++;

        if ( ( ( counter + strlen( needle ) ) > str_size_in_bytes ) ||
             ( *haystack == '\0' ) )
        {
            continue_searching = false;
        }

    }

    /* Didn't find it */
    return NULL;
}

/**
 * \brief Kill end-of-line characters.
 *
 * Work-a-like of Perl's chomp(). 
 *
 * Warning! Modifies the buffer! Overwrites trailing 0x0d and 0x0a characters
 * with 0 (null).
 *
 * \in buflen is the string length of the buffer. 
 * \out buflen is the new string length after killing end-of-line chars.
 *
 * \author DP
 * \date 06-Sep-2012
 */
void str_chomp( char *buf, int *buflen )
{
    ASSERT( buf != NULL );
    ASSERT( buflen != NULL );

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

