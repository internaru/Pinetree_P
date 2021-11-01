/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief This is the header file containing platform-specific configuration
 * details for the ZJ Stream parser.
 *
 */

#ifndef ZJ_PARSER_CONFIG_H
#define ZJ_PARSER_CONFIG_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief Enumeration of potential ZJ Stream data formats.
 * slave 
 */
typedef enum
{
    ZJ_DATA_FORMAT_JBIG = 0,
    ZJ_DATA_FORMAT_ZX
} zj_data_format_t;

/**
 * \brief ZJ Parser configuration structure.
 * slave 
 */
typedef struct zj_parser_config_s
{
    zj_data_format_t data_format;
} zj_parser_config_t;


/*
 * \brief Returns the ZJ Stream Parser configuration for this platform.
 *
 * \param config A pointer to the zj_parser_config_t pointer owned by the
 * caller.
 *
 * \retval error_type_t OK if successful, FAIL otherwise.
 *
 * \note The zj_parser_config_t struct returned by this function has been
 * statically allocated, and does not need to be freed.
 *
 */
error_type_t zj_parser_get_config( const zj_parser_config_t **config );


#ifdef __cplusplus
}
#endif

#endif  // ZJ_PARSER_CONFIG_H

