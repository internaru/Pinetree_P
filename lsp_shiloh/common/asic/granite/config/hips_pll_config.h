
/* 
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifndef HIPS_PLL_CONFIG_H
#define HIPS_PLL_CONFIG_H
#include <stdint.h>

uint8_t hips_pll_num_chans(void);


uint32_t hips_get_cr0(void);
uint32_t hips_get_cr1(void);
uint32_t hips_get_cr2(void);

#endif
