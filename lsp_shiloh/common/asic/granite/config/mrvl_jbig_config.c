/*
 * ============================================================================
 * Copyright (c) 2010, 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file mrvl_jbig_config.c
 * 
 * \brief JBIG device info.
 * 
 **/
#include <stdint.h>
#include <stdio.h>
#include "jbig_api.h"
#include "mrvl_jbig_config.h"
#include "asic.h"
#include "uio_lib_api.h"
#include "lassert.h"


static bool jbig_config_initialized = false;
jbig_block_config_t jbig_block_config_array[NUM_JBIG_BLOCKS];

/** 
 * \brief Get the platform configuration.
 * 
 * \param config Return configuration to the address of this
 * location.
 * 
 * \return OK if successful, FAIL if not.
 **/
const jbig_block_config_t* jbig_platform_get_config()
{
    uio_map_t *map = NULL; 

    if(!jbig_config_initialized)
    {
        jbig_block_config_array[0].jbig_block_type = JBIG_CODEC_BLOCK;
        jbig_block_config_array[0].dev_jbig = uio_open_dev("jbig_codec");
        map = uio_get_map(jbig_block_config_array[0].dev_jbig, 0);
        ASSERT(map != NULL);
        jbig_block_config_array[0].jbig_regs = map->virt_addr;

        jbig_block_config_array[0].dev_idma = uio_open_dev("jbig_codec_idma");
        map = uio_get_map(jbig_block_config_array[0].dev_idma, 0);
        ASSERT(map != NULL);
        jbig_block_config_array[0].jbig_idma_regs = map->virt_addr;

        jbig_block_config_array[0].dev_odma = uio_open_dev("jbig_codec_odma");
        map = uio_get_map(jbig_block_config_array[0].dev_odma, 0);
        ASSERT(map != NULL);
        jbig_block_config_array[0].jbig_odma_regs = map->virt_addr;

        jbig_config_initialized = true;
    }

    return jbig_block_config_array;
}

