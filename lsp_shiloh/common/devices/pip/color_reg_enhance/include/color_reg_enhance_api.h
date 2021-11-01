#ifndef COLOR_REG_ENHANCE_API_H
#define COLOR_REG_ENHANCE_API_H
/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
*/

/**
 *  \file color_reg_enhance_api.h
 *  \brief Function prototypes and defines for accessing the contone pad pogo
 *         hardware.
 * 
 **/
#include "color_reg_enhance_cfg.h"


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */

 
/* -------------------------------------------------------------------------- */
/*                   F U N C T I O N  P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */


/**
 * \brief Initialize the color registration enhancement hardware driver.
 */
void color_reg_enhance_init( void );

error_type_t color_reg_enhance_set_cfg(
    const color_reg_enhance_cfg_t *cfg, 
    int *internal_buffering_lines_needed ///< returns the number of lines witheld from output for area OPS
    );

void color_reg_enhance_dump_regs( void );


#endif
