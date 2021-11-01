/*
 * ============================================================================
 * Copyright (c) 2011   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *
 * \brief This file contains platform-specific configuration details for the ZJ
 * Stream parser.
 *
 */

#include <stdint.h>
#include "ATypes.h"
#include "lassert.h"
#include "zj_parser_config.h"


static zj_parser_config_t zj_parser_platform_config =
{
    .data_format = ZJ_DATA_FORMAT_JBIG,
};

/**
 *
 * See zj_parser_config.h for details.
 *
**/
error_type_t zj_parser_get_config( const zj_parser_config_t **config )
{
    ASSERT( NULL != config );

    *config = &zj_parser_platform_config;

    return OK;
}

