/*
 * ============================================================================
 * Copyright (c) 2010,2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file jbig_decode_config.h
 * 
 * \brief JBIG device info
 * 
 **/
 
#ifndef JBIG_DECODE_CONFIG_H
#define JBIG_DECODE_CONFIG_H
 
#include <stdint.h>
#include "asic.h"

#define NUM_JBIG_BLOCKS 1

typedef struct jbig_block_config_s
{
    JBIG_REGS_t* jbig_regs;
    IDMA_SLI_REGS_t* jbig_idma_regs;
    ODMA_SLI_REGS_t* jbig_odma_regs;
    uint32_t intnum_jbig;
    uint32_t intnum_idma;
    uint32_t intnum_odma;
}jbig_decode_block_config_t;

const jbig_decode_block_config_t* jbig_decode_get_block_config( void );

#endif
