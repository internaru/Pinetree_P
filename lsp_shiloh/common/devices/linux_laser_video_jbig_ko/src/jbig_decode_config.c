/*
 * ============================================================================
 * Copyright (c) 2010,2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file jbig_decode_config.c
 * 
 * \brief JBIG device info.
 * 
 **/
#include <stdint.h>
#include "jbig_decode_config.h"

static const jbig_decode_block_config_t jbig_block_config =
{
    .jbig_regs = (JBIG_REGS_t*)JBIG_JBIG_DECODE_BASE,
    .jbig_idma_regs = (IDMA_SLI_REGS_t*)JBIG_JBIG_DECODE_IDMA_BASE,
    .jbig_odma_regs = (ODMA_SLI_REGS_t*)JBIG_JBIG_DECODE_ODMA_BASE,
    .intnum_jbig = INTNUM_JBIG_DECODER_OVER,
    .intnum_idma = INTNUM_JBIG_DECODER_JBD,
    .intnum_odma = INTNUM_JBIG_DECODER_JPX
};

/** 
 * \brief Get the platform configuration.
 * 
 * \param config Return configuration to the address of this
 * location.
 * 
 * \return OK if successful, FAIL if not.
 **/
const jbig_decode_block_config_t* jbig_decode_get_block_config( void )
{
    return &jbig_block_config;
}

