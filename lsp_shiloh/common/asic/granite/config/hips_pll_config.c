/* 
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

 #include <stdint.h>
 #include "hips_pll_config.h"


uint8_t hips_pll_num_chans(void)
{
    return 8;
}

uint32_t hips_get_cr0(void)
{
    return 0x06ab2c1a;      // the pll1_cr0 register value
}

uint32_t hips_get_cr1(void)
{
    return 0x00c7348a;      // cr1 register value
}

uint32_t hips_get_cr2(void)
{
    return 0x051a2c38;
}
