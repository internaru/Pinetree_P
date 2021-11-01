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
 * \file utf8.c
 *
 * \brief This file contains utility functions used for converting 
 * characters to and from UTF-8.
 *
 */

  
#include <string.h>
#include "utf8.h"
#include "logger.h"
#include "lassert.h"

/* private function prototypes */
int   IsValidCombinedChar( UINT32 ch );

  /* 
  UTF-8 encoding/decoding functions
  
  Code point          1st byte    2nd byte    3rd byte    4th byte
  ----------          --------    --------    --------    --------
  U+0000..U+007F      00..7F
  U+0080..U+07FF      C2..DF      80..BF
  U+0800..U+0FFF      E0          A0..BF      80..BF
  U+1000..U+FFFF      E1..EF      80..BF      80..BF
  U+10000..U+3FFFF    F0          90..BF      80..BF      80..BF
  U+40000..U+FFFFF    F1..F3      80..BF      80..BF      80..BF
  U+100000..U+10FFFF  F4          80..8F      80..BF      80..BF
  
  */
 
 #define NumUTF8Sequences        7
 #define MaxUTF8Bytes            4
 
 #define UTF8ByteSwapNotAChar    0xFFFE
 #define UTF8NotAChar            0xFFFF
 
 #define MaxUTF8FromUCS4         0x10FFFF
 
 #define UTF16SurrogatesBegin    0x10000
 #define MaxUTF16FromUCS4        0x10FFFF
 
 /* UTF-16 surrogate pair areas */
/*
Surrogates and Supplementary Characters
The Unicode standard has established 16 additional planes of characters, each the same size as the BMP. Naturally, most code points beyond the BMP do not yet have characters assigned to them, but this gives Unicode the potential to define 1,114,112 characters (that is, 216 * 17 characters) within the code point ranges U+0000 to U+10FFFF. In order for UTF-16 to represent this larger repertoire of characters  that is, those characters beyond the BMP  the Unicode Standard defines surrogates, surrogate pairs, and supplementary characters.

A supplementary character is a character which is located beyond the BMP; in UTF-16, surrogate pairs are needed to represent supplementary characters. A surrogate pair is a pair of UTF-16 code values that represent a single supplementary character; each of the two code values in the pair is called a surrogate. The first (high) surrogate is a 16-bit code value in the range U+D800 to U+DBFF. The second (low) surrogate is a 16-bit code value in the range U+DC00 to U+DFFF. Using the surrogate mechanism, UTF-16 can support all 1,114,112 potential Unicode characters. For more details about supplementary characters, surrogate pairs and surrogates, refer to The Unicode Standard  . 


*/
 #define UTF16LowSurrogateBegin  0xD800
 #define UTF16LowSurrogateEnd    0xDBFF
 #define UTF16HighSurrogateBegin 0xDC00
 #define UTF16HighSurrogateEnd   0xDFFF


 /* offsets into validUTF8 table below */
 static const int offsetUTF8Sequences[MaxUTF8Bytes + 1] =
 {
     0, /* 1 byte */
     1, /* 2 bytes */
     2, /* 3 bytes */
     4, /* 4 bytes */
     NumUTF8Sequences /* must be last */
 };

#define UTF8_VALID_BYTES_MAX_SIZE 8

 static const struct validUTF8Sequence
 {
      unsigned int lowChar;
      unsigned int highChar;
      int  numBytes;
      unsigned char validBytes[ UTF8_VALID_BYTES_MAX_SIZE ];
 } validUTF8[NumUTF8Sequences] =
 {
 /*   low       high   #bytes  byte 1      byte 2      byte 3      byte 4 */
     {0x0000,   0x007F,   1, {0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {0x0080,   0x07FF,   2, {0xC2, 0xDF, 0x80, 0xBF, 0x00, 0x00, 0x00, 0x00}},
     {0x0800,   0x0FFF,   3, {0xE0, 0xE0, 0xA0, 0xBF, 0x80, 0xBF, 0x00, 0x00}},
     {0x1000,   0xFFFF,   3, {0xE1, 0xEF, 0x80, 0xBF, 0x80, 0xBF, 0x00, 0x00}},
     {0x10000,  0x3FFFF,  4, {0xF0, 0xF0, 0x90, 0xBF, 0x80, 0xBF, 0x80, 0xBF}},
     {0x40000,  0xFFFFF,  4, {0xF1, 0xF3, 0x80, 0xBF, 0x80, 0xBF, 0x80, 0xBF}},
     {0x100000, 0x10FFFF, 4, {0xF4, 0xF4, 0x80, 0x8F, 0x80, 0xBF, 0x80, 0xBF}} 
 };
 
static int DecodeUTF8BytesToChar( UINT32 *c, UINT8 firstByte, 
                            UINT8 *successorBytes, int* count )
 {
     UINT8 tempbuf[10];
     UINT8 *buf = &tempbuf[0];
     UINT32 ch = 0, n = 0;
     int i, bytes = 0;
     int hasError = 0;
     
     if ( successorBytes )
         buf = (UINT8 *) successorBytes;
         
     /* special check if we have been passed an EOF char */
     if ( firstByte == 0xFF )
     {
         /* at present */
         *c = firstByte;
         *count = 1;
         return 0;
     }
 
     ch = firstByte; /* first byte is passed in separately */
     
     if (ch <= 0x7F) /* 0XXX XXXX one byte */
     {
         n = ch;
         bytes = 1;
     }
     else if ((ch & 0xE0) == 0xC0)  /* 110X XXXX  two bytes */
     {
         n = ch & 31;
         bytes = 2;
     }
     else if ((ch & 0xF0) == 0xE0)  /* 1110 XXXX  three bytes */
     {
         n = ch & 15;
         bytes = 3;
     }
     else if ((ch & 0xF8) == 0xF0)  /* 1111 0XXX  four bytes */
     {
         n = ch & 7;
         bytes = 4;
     }
     else if ((ch & 0xFC) == 0xF8)  /* 1111 10XX  five bytes */
     {
         n = ch & 3;
         bytes = 5;
         hasError = 1;
     }
     else if ((ch & 0xFE) == 0xFC)  /* 1111 110X  six bytes */
     {
         n = ch & 1;
         bytes = 6;
         hasError =1;
     }
     else
     {
         /* not a valid first byte of a UTF-8 sequence */
         n = ch;
         bytes = 1;
         hasError =1;
     }
 
     /* successor bytes should have the form 10XX XXXX */
 
     for ( i=0; i < bytes-1; ++i )
     {
         if ( !buf[i] || (buf[i] & 0xC0) != 0x80 )
         {
             hasError =1;
             bytes = i;
             break;
         }
         n = (n << 6) | (buf[i] & 0x3F);
     }

     
     if (!hasError && ((n == UTF8ByteSwapNotAChar) || (n == UTF8NotAChar)))
         hasError = 1;
         
     if (!hasError && (n > MaxUTF8FromUCS4))
         hasError = 1;
 
 #if 0 /* Breaks Big5 D8 - DF */
     if (!hasError && (n >= kUTF16LowSurrogateBegin) && (n <= kUTF16HighSurrogateEnd))
         /* unpaired surrogates not allowed */
         hasError = yes;
 #endif
 
     if (!hasError)
     {
         int lo, hi;

         /* prevent buffer overflow */
         ASSERT( bytes > 0 );
		 ASSERT( bytes < ( MaxUTF8Bytes + 1 ) );
         lo = offsetUTF8Sequences[bytes - 1];
         hi = offsetUTF8Sequences[bytes] - 1;

         /* check for overlong sequences */
         if ((n < validUTF8[lo].lowChar) || (n > validUTF8[hi].highChar))
             hasError = 1;
         else
         {
             hasError = 1; /* assume error until proven otherwise */

             for (i = lo; i <= hi; i++)
             {
                 int tempCount;
                 UINT8 theByte;

                 for (tempCount = 0; tempCount < bytes; tempCount++)
                 {
                     if (!tempCount)
                         theByte = firstByte;
                     else
                         theByte = buf[tempCount - 1];

                     /* prevent buffer overflow */
                     ASSERT( ( tempCount * 2 ) + 1 < UTF8_VALID_BYTES_MAX_SIZE );

                     if ( theByte >= validUTF8[i].validBytes[(tempCount * 2)] &&
                          theByte <= validUTF8[i].validBytes[(tempCount * 2) + 1] )
                         hasError =0;
                     if (hasError)
                         break;
                 }
             }
         }
     }
 
 #if DEBUG
     if ( hasError )
     {
        /* debug */
        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("UTF-8 decoding error of %d bytes : ", bytes) );
        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("0x%02x ", firstByte ));
        for (i = 1; i < bytes; i++)
            DPRINTF( (DBG_LOUD|DBG_OUTPUT), ("0x%02x ", buf[i - 1]));
        DPRINTF( (DBG_LOUD|DBG_OUTPUT), (" = U+%04ulx\n", n ));
     }
 #endif
 
     *count = bytes;
     *c = n;
     if ( hasError )
         return -1;
     return 0;
 }
/*
   count  
*/
static int EncodeCharToUTF8Bytes( UINT32 c, UINT8 *buf, int *count )
 {
     int bytes = 0;
     int hasError = 0;
     
     if (c <= 0x7F)  /* 0XXX XXXX one byte */
     {
         buf[0] = (UINT8) c;
         bytes = 1;
     }
     else if (c <= 0x7FF)  /* 110X XXXX  two bytes */
     {
         buf[0] = (UINT8) ( 0xC0 | (c >> 6) );
         buf[1] = (UINT8) ( 0x80 | (c & 0x3F) );
         bytes = 2;
     }
     else if (c <= 0xFFFF)  /* 1110 XXXX  three bytes */
     {
         buf[0] = (UINT8) (0xE0 | (c >> 12));
         buf[1] = (UINT8) (0x80 | ((c >> 6) & 0x3F));
         buf[2] = (UINT8) (0x80 | (c & 0x3F));
         bytes = 3;
         if ( c == UTF8ByteSwapNotAChar || c == UTF8NotAChar )
             hasError = 1;
 #if 0 /* Breaks Big5 D8 - DF */
         else if ( c >= kUTF16LowSurrogateBegin && c <= kUTF16HighSurrogateEnd )
             /* unpaired surrogates not allowed */
             hasError = yes;
 #endif
     }
     else if (c <= 0x1FFFFF)  /* 1111 0XXX  four bytes */
     {
         buf[0] = (UINT8) (0xF0 | (c >> 18));
         buf[1] = (UINT8) (0x80 | ((c >> 12) & 0x3F));
         buf[2] = (UINT8) (0x80 | ((c >> 6) & 0x3F));
         buf[3] = (UINT8) (0x80 | (c & 0x3F));
         bytes = 4;
         if (c > MaxUTF8FromUCS4)
             hasError = 1;
     }
     else if (c <= 0x3FFFFFF)  /* 1111 10XX  five bytes */
     {
         buf[0] = (UINT8) (0xF8 | (c >> 24));
         buf[1] = (UINT8) (0x80 | (c >> 18));
         buf[2] = (UINT8) (0x80 | ((c >> 12) & 0x3F));
         buf[3] = (UINT8) (0x80 | ((c >> 6) & 0x3F));
         buf[4] = (UINT8) (0x80 | (c & 0x3F));
         bytes = 5;
         hasError = 1;
     }
     else if (c <= 0x7FFFFFFF)  /* 1111 110X  six bytes */
     {
         buf[0] = (UINT8) (0xFC | (c >> 30));
         buf[1] = (UINT8) (0x80 | ((c >> 24) & 0x3F));
         buf[2] = (UINT8) (0x80 | ((c >> 18) & 0x3F));
         buf[3] = (UINT8) (0x80 | ((c >> 12) & 0x3F));
         buf[4] = (UINT8) (0x80 | ((c >> 6) & 0x3F));
         buf[5] = (UINT8) (0x80 | (c & 0x3F));
         bytes = 6;
         hasError = 1;
     }
     else
         hasError = 1;
         
     /* don't output invalid UTF-8 byte sequence to a stream */
     
 
 #if DEBUG
     if ( hasError )
     {
         int i;
         DPRINTF((DBG_LOUD|DBG_OUTPUT), ( "UTF-8 encoding error for U+%x : ", c ));
         for (i = 0; i < bytes; i++)
             DPRINTF((DBG_LOUD|DBG_OUTPUT), ("0x%02x ", buf[i]) );
         DPRINTF((DBG_LOUD|DBG_OUTPUT), ("\n") );
     }
 #endif
     
     *count = bytes;
     if (hasError)
         return -1;
     return 0;
 }
 
 
/** \brief Routine for converting UTF8 string to 32-bit Unicode string
 * 
 * \param[in] str_U32  buffer to hold returned 32-bit unicode string
 * \param[in] str_UTF8 pointer to UTF8 string to be converted
 * 
 *\return length of the unicode string obtained
 *
 */
int UTF8ToU32(UINT32 *str_U32, UINT8 *str_UTF8)
{
    int i = 0;
    int j = 0;
    int c = 0;
    int err;

    while( *(str_UTF8 + i))
    {
        err = DecodeUTF8BytesToChar( str_U32 + j, *(str_UTF8 + i), str_UTF8 + i + 1, &c );
        ASSERT(!err);
        i = i + c;
        j++;
    }
    //Mark the end
    str_U32[j]=0;
    return j;
}

/** \brief Routine for converting a maximum number of characters in a UTF8 string
 *         to 32-bit Unicode string.  Note the this function ALWAYS inserts a
 *         null terminator so the output buffer should accomodate maxChars+1
 *         characters.
 * 
 * \param[in]  str_U32  buffer to hold returned 32-bit unicode string
 * \param[in]  str_UTF8 pointer to UTF8 string to be converted
 * \param[in]  maxChars maximum number of characters to copy (not including the
 *             null terminator)
 * 
 *\return length of the unicode string obtained, not including the null terminator
 *
 */
int UTF8NToU32(UINT32 *str_U32, const UINT8 *str_UTF8, int maxChars)
{
    int i = 0;
    int j = 0;
    int c = 0;
    int err;

    while ((*(str_UTF8 + i)) && maxChars)
    {
        err = DecodeUTF8BytesToChar( str_U32 + j, *(str_UTF8 + i), ( UINT8* )( str_UTF8 + i + 1 ), &c );
        ASSERT(!err);

        if(c > 0) 
           i = i + c;
        else
           i++; 

        if(!err)
        {
           j++;
           maxChars--;
        }
    }
    //Mark the end
    str_U32[j]=0;
    return j;
}

//------------------------------------------------------------------------------
// FUNCTION NAME: UTF8StrNCpy
/**
 * \brief Routine for copying a maximum number of characters in a UTF8 string to
 *        another UTF8 string.  Note the this function ALWAYS inserts a null
 *        terminator so the output buffer should accomodate maxChars+1 characters.
 *
 * \param str_UTF8_out (UINT8*) Pointer to the output string.
 * \param str_UTF8_in  (UINT8*) Pointer to the input string.
 * \param maxChars (int) The maximum number of characters to copy, not including
 *                       the null terminator.
 * 
 * \return int
 * \retval The number of non-null characters copied to the output string.
 *
 **/
int UTF8StrNCpy( UINT8 *str_UTF8_out, const UINT8 *str_UTF8_in, int maxChars )
{
    int i = 0;
    int j = 0;
    int c = 0;
    int err;
    UINT32 ch32;

    while ( *(str_UTF8_in + i) && maxChars )
    {
        // convert current UTF8 character to Unicode just to determine the UTF8 byte length
        err = DecodeUTF8BytesToChar(&ch32, *(str_UTF8_in + i), ( UINT8* )( str_UTF8_in + i + 1 ), &c );
        ASSERT(!err);

        // copy current UTF8 character to the output string if no error
        if ( !err )
        {
            memcpy( str_UTF8_out + i, str_UTF8_in + i, c );

            j++;
            maxChars--;
        }

        // slide our input string offset
        if ( c > 0 )
        {
            i = i + c;
        }
        else
        {
            i++; 
        }
    }

    //null terminate the output string & return the number of non-null characters copied
    str_UTF8_out[ i ]=0;
    return j;
}

/**\brief Get the number of characters in a UTF8 format string
 */
int UTF8StrLen( UINT8 *str_UTF8)
{
    int i = 0;
    int j = 0;
    int c = 0;
    int err;
    UINT32 ch32;
    while( *(str_UTF8 + i))
    {
        err = DecodeUTF8BytesToChar(&ch32, *(str_UTF8 + i), str_UTF8 + i + 1, &c );
        ASSERT(!err);
        i = i + c;
        j++;
    }
    return j;
}

/**\brief Route for converting 32-bit Unicode string to UTF8 string.
 *
 *\param[in] str_UTF8 buffer to hold resulting UTF8 string  
 *\param[in] str_U32  buffer to hold 32bit unicode string to be converted
 *\return length of UTF8 string obtained
 */

int U32ToUTF8(UINT8 *str_UTF8, UINT32 *str_U32) 
{
    int i = 0;
    int j = 0;
    int c = 0;
    int err;

    while( *(str_U32 + j))
    {
         err= EncodeCharToUTF8Bytes( *( str_U32 + j ),  str_UTF8 + i , &c);
         ASSERT(!err);
         i = i + c;
         j++;
    }
    //Mark the end
    str_UTF8[i] = 0;
    return i;

}


/**\brief Route for converting 32-bit Unicode string to UTF8 string.
 *        Note the this function ALWAYS inserts a null terminator so
 *        the output buffer should accomodate maxChars+1 characters.
 *\param[in] str_UTF8 buffer to hold resulting UTF8 string  
 *\param[in] str_U32  buffer to hold 32bit unicode string to be converted
 * \param[in] maxChars maximum number of characters to copy (not including the
 *             null terminator)
 *\return length of UTF8 string obtained
 */

int U32NToUTF8(UINT8 *str_UTF8, UINT32 *str_U32, int maxChars) 
{
    int i = 0;
    int j = 0;
    int c = 0;
    int err;

	while(( *(str_U32 + j)) && maxChars)
    {
         err= EncodeCharToUTF8Bytes( *( str_U32 + j ),  str_UTF8 + i , &c);
         ASSERT(!err);
         i = i + c;
         j++;
		 maxChars--;
    }
    //Mark the end
    str_UTF8[i] = 0;
    return i;

}

/**\brief Scans through a UTF-8 string and replaces any invalid characters
 *        with a '?'.
 *
 *\param[in] str_UTF8 string to sanitize
 *
 *\return true if data was sanitized, false otherwise
 */
bool SanitizeUTF8String(UINT8 *str_UTF8)
{
    bool    chars_replaced = false;
    UINT32  decoded_char;
    UINT32  i = 0;
    int     bytes_in_char = 0;
    int     err;

    while( *(str_UTF8 + i))
    {
        err = DecodeUTF8BytesToChar( &decoded_char, *(str_UTF8 + i), str_UTF8 + i + 1, &bytes_in_char );
        if (!err)
        {
            i = i + bytes_in_char;
        }
        else
        {
            // There was a byte that doesn't decode correctly.  Replace that byte
            // and try again with the next one.
            *(str_UTF8 + i) = '?';
            i++;
            chars_replaced = true;
        }
    }

    return chars_replaced;
}


int  IsValidUTF16FromUCS4( UINT32 ucs4 )
{
   return ( ucs4 <= MaxUTF16FromUCS4 );
}
 
int   IsHighSurrogate( UINT16 ch )
{
     return ( ch >= UTF16HighSurrogateBegin && ch <= UTF16HighSurrogateEnd );
}

int  IsLowSurrogate( UINT16 ch )
{
     return ( ch >= UTF16LowSurrogateBegin && ch <= UTF16LowSurrogateEnd );
}

unsigned long CombineSurrogatePair( UINT16 high, UINT16 low )
 {
     ASSERT(IsHighSurrogate(high));
     ASSERT(IsLowSurrogate(low));
     return ( ((low - UTF16LowSurrogateBegin) * 0x400) + 
              high - UTF16HighSurrogateBegin + 0x10000 );
 }
 
int   SplitSurrogatePair( UINT16 utf16, UINT16* low, UINT16* high )
 {
     int status = ( IsValidCombinedChar( utf16 ) && high && low );
     if ( status )
     {
         *low  = (utf16 - UTF16SurrogatesBegin) / 0x400 + UTF16LowSurrogateBegin;
         *high = (utf16 - UTF16SurrogatesBegin) % 0x400 + UTF16HighSurrogateBegin;
     }
     return status;
 }
 
int   IsValidCombinedChar( UINT32 ch )
 {
     return ( ch >= UTF16SurrogatesBegin &&
              (ch & 0x0000FFFE) != 0x0000FFFE &&
              (ch & 0x0000FFFF) != 0x0000FFFF );
 }
 
int    IsCombinedChar( UINT32 ch )
 {
     return ( ch >= UTF16SurrogatesBegin );
 }


 

