/***********************************************************
* (c) Copyright 2008-2010 Marvell International Ltd.
*
*               Marvell Confidential
* ==========================================================
*/

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "logger.h"


/** \brief Last valid DRAM memory address. Used in pointer validity checks. */
static const uintptr_t dram_last_addr;

void debug_set_dram_end( uintptr_t addr )
{
    uintptr_t num;
    uintptr_t *ptr;

    num = (uintptr_t)&dram_last_addr;
    ptr = (uintptr_t *)num;
    *ptr = addr;
}

uintptr_t debug_get_dram_last_addr( void )
{
    return dram_last_addr;
}

void hex_dump_named(void *start_addr, int size, char *region)
{

    int i;
    unsigned char line[80];
    unsigned char *ascii, *hex;
    unsigned char *endptr, *ptr = (unsigned char *)start_addr;
    uint32_t offset = (uint32_t)ptr;
    static char hex_ascii[] =
    {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    endptr = ptr + size;
    memset(line, ' ', 80);
    line[69] = 0;

    if (region)
    {
        dbg_printf("\n%s:\n", region);
    }

    while(ptr != endptr) {
        hex = &line[2];
        ascii = &line[52];

        for(i = 0 ; i < 16 ; i++) {
            if(isprint(*ptr))
                *ascii++ = *ptr;
            else
                *ascii++ = '.';

            *hex++ = hex_ascii[*ptr >> 4];
            *hex++ = hex_ascii[*ptr & 0x0F];
            *hex++ = ' ';

            // clean out whatever is left from the last line...
            if(++ptr == endptr) {
                memset(hex, ' ', (15 - i) * 3);
                memset(ascii, ' ', 15 - i);
                break;
            }
        }

        dbg_printf("0x%08x %s\n", offset, line);
        offset += 16;
    }

    dbg_printf("\n");
}

