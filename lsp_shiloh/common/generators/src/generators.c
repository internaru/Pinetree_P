/*
 * ============================================================================
 * Copyright (c) 2011  Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include "generator_inits.h"
#include "dprintf.h"
#include "lassert.h"

/**
 * \file generators.c 
 * \brief Common code for implementing generators. 
 */

/** 
 * \brief Initialize generators.
 * 
 * \param None.
 */
void generators_init(void)
{
    dbg_printf("%s\n", __func__);
#ifdef HAVE_GENERATOR_SKELETON
    genskel_init();
#endif


#if defined HAVE_GENERATOR_PIP || defined HAVE_GENERATOR_PIP_ADAPTOR
    genpip_init();
#else
#warning no pip
#endif

#ifdef HAVE_GENERATOR_PIPP_PIPR
    gen_pipp_pipr_init();
#endif

#ifdef HAVE_GENERATOR_COMPRESS
    gen_compress_init();
#endif

#ifdef HAVE_GENERATOR_DECOMPRESS
    gen_decompress_init();
#endif

#ifdef HAVE_GENERATOR_NUP
    gen_nup_init();
#endif

#ifdef HAVE_GENERATOR_OEM
// TODO: insert call to init oem generators here
#endif
}
