/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file mrvl_jbig_config.h
 * 
 * \brief JBIG device info
 * 
 **/
 
#ifndef MRVL_JBIG_CONFIG_H
#define MRVL_JBIG_CONFIG_H
 
#include <stdint.h>
#include "error_types.h"
#include "jbig_api.h"
#include "asic.h"
//#include "jbig_compatibility.h"
#include "uio_lib_api.h"

#define NUM_JBIG_BLOCKS 1      ///< number of JBIG blocks supported for this ASIC

typedef struct jbig_block_config_s
{
    JBIG_BLOCK jbig_block_type; // encode or decode
    JBIG_REGS_t* jbig_regs;
    IDMA_SLI_REGS_t* jbig_idma_regs;
    ODMA_SLI_REGS_t* jbig_odma_regs;
    uio_dev_t* dev_jbig;
    uio_dev_t* dev_idma;
    uio_dev_t* dev_odma;
}jbig_block_config_t;

const jbig_block_config_t* jbig_platform_get_config();

#endif
