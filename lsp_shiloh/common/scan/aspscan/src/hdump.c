/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "log.h"

/**
 *  Very nice hex dump function.
 *
 */

void hex_dump( unsigned char *ptr, int size )
{
    static char hex_ascii[] =
       { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    int i;
    unsigned char line[80];
    unsigned char *ascii, *hex;
    unsigned char *endptr;
    unsigned long offset=0;

   endptr = ptr + size;
   memset( line, ' ', 80 );
   line[69] = 0;
   while( ptr != endptr ) {
      hex = &line[2];
      ascii = &line[52];
      for( i=0 ; i<16 ; i++ ) {
         if( isprint(*ptr) )
            *ascii++ = *ptr;
         else
            *ascii++ = '.';
         *hex++ = hex_ascii[ *ptr>>4 ];
         *hex++ = hex_ascii[ *ptr&0x0f ];
         *hex++ = ' ';
         ptr++;
         if( ptr == endptr ) {
            /* clean out whatever is left from the last line */
            memset( hex, ' ', (15-i)*3 );
            memset( ascii, ' ', 15-i );
            break;
         }
      }
      dbg1( "0x%08lx %s\n", offset, line );
      offset += 16;
   }
}

