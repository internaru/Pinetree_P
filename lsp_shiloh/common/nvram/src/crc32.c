/* 
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *               Marvell Confidential
 * ============================================================================
 */


/*  The generator polynomial  is x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+
    x^8+x^7+x^5+x^4+x^2+x^1+x^0
  The representation used is that the coefficient of x^0 is stored in the 
  LSB of the 32-bit 
*/

#include <stdint.h>

#define POLY 0x04c11db7L

/**
 * \brief Generate the crc table for the calcuations
 */
static void crc_gen_table(uint32_t *crc_table_input)
{
    register int i, j;  register uint32_t acc_crc;
    for ( i = 0;  i < 256;  i++ )
    {
        acc_crc = ( (unsigned long) i << 24 );
        for ( j = 0;  j < 8;  j++ )
        {
            if ( acc_crc & 0x80000000L )
                acc_crc =
                ( acc_crc << 1 ) ^ POLY;
            else
                acc_crc =
                ( acc_crc << 1 );
        }
        crc_table_input[i] = acc_crc;
    }
    return; 
}

uint32_t short_crc32(char *data_blk_ptr, int block_size)
{
    register int i, j;
    register uint32_t acc_crc=0;
    uint32_t param_table[256];
    crc_gen_table(param_table);
    for ( j = 0;  j < block_size;  j++ )
    {
        i = ( (int) ( acc_crc >> 24) ^ *data_blk_ptr++ ) & 0xff;
        acc_crc = ( acc_crc << 8 ) ^ param_table[i];
    }
    return acc_crc; 
}


