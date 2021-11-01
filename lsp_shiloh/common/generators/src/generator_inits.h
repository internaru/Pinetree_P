/**
 *
 * ============================================================================
 * Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file generator_inits.c 
 * \brief Init routine declarations for all common generators.
 */
#ifndef _GENERATOR_INITS_H
#define _GENERATOR_INITS_H

#include <stdint.h>

uint32_t genskel_init( void );
uint32_t genpip_init( void );
uint32_t gen_pipp_pipr_init( void );
uint32_t gen_compress_init( void );
uint32_t gen_decompress_init( void );
uint32_t gen_nup_init( void );

#endif // _GENERATOR_INITS_H

