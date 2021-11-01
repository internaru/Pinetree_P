/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanmarg.c
 *
 * \brief convenience functions to read/write flatbed/adf margin tables
 *
 * Actual margins are stored in the mech driver. 
 *
 * If you're trying to retrieve a margin, use the scanmech margin functions.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lassert.h"

#include "scancore.h"
#include "scantypes.h"
#include "scanlog.h"
#include "scandbg.h"
#include "scanmargin.h"

//#define SCANMARGIN_DEBUG

#ifdef SCANMARGIN_DEBUG
#define margdbg2 dbg2
#else
#define margdbg2(...)
#endif

/**
 * \brief Get the flatbed x/y margins.
 *
 *  Platform margin adjustments used to hide parts of the mech.
 */

scan_err_t scanmargin_get_flatbed_margins( struct scan_flatbed_margins fb_margin_table[],
                                    int dpi, scan_cmode_t cmode,
                                    struct scan_flatbed_margins *fb_margins)
{
    int i;

    margdbg2("%s: searching for dpi=%d cmode=%d\n", __FUNCTION__, dpi, cmode);
    
    /* Search the table */
    for (i=0 ; ; i++)
    {
        /* sanity check against bad lists */
        XASSERT( i<42, i );

        if (fb_margin_table[i].dpi==0) {
            /* end of list entry */
            return SCANERR_NO_ENTRY;
        }

        margdbg2("fb_margin_table[%d] dpi=%d cmode=%d\n", i, dpi, cmode);
       
        if (fb_margin_table[i].dpi==dpi && fb_margin_table[i].cmode==cmode) {
            margdbg2( "%s found!\n", __FUNCTION__ );
            memcpy(fb_margins, &fb_margin_table[i], sizeof(struct scan_flatbed_margins));
            break;
        }
    }

    return SCANERR_NONE;
}

/**
 * \brief  Test/debug function for tuning the flatbed margins
 *
 */

scan_err_t scanmargin_debug_set_fb_margins( struct scan_flatbed_margins fb_margin_table[],
                                          struct scan_flatbed_margins *new_fb_margins )
{
    int i;

    margdbg2("%s: searching for dpi=%d cmode=%d\n", __FUNCTION__, 
                new_fb_margins->dpi, new_fb_margins->cmode);

    for (i=0 ; ; i++)
    {
        /* sanity check against bad lists */
        XASSERT( i<42, i );

        if (fb_margin_table[i].dpi==0) {
            return SCANERR_NO_ENTRY;
        }
        
        margdbg2("fb_margin_table[%d] dpi=%d cmode=%d\n", 
                    i, fb_margin_table[i].dpi, fb_margin_table[i].cmode);

        if(fb_margin_table[i].dpi  ==new_fb_margins->dpi &&
           fb_margin_table[i].cmode==new_fb_margins->cmode)
        {
            /* note no range checking. enjoy! */
            memcpy(&fb_margin_table[i], new_fb_margins, sizeof(struct scan_flatbed_margins));
            break;
        }
    }

    return SCANERR_NONE;
}

/**
 * \brief  Get the ADF tof/bof margins.
 *
 */

scan_err_t scanmargin_get_adf_margins(struct scan_adf_margins adf_margin_table[],
                                      int dpi, scan_cmode_t cmode,
                                      struct scan_adf_margins *adf_margins)
{
    int i;
    
    margdbg2("%s: searching for dpi=%d cmode=%d\n", __FUNCTION__, dpi, cmode);

    /* Search the table */
    for( i=0 ; ; i++ ) {
        /* sanity check against bad lists */
        XASSERT( i<42, i );

        if( adf_margin_table[i].dpi==0 ) {
            return SCANERR_NO_ENTRY;
        }

        margdbg2("adf_margin_table[%d] dpi=%d cmode=%d\n", 
                    i, adf_margin_table[i].dpi, adf_margin_table[i].cmode);

        if( adf_margin_table[i].dpi==dpi && adf_margin_table[i].cmode==cmode ) {
            margdbg2( "%s found!\n", __FUNCTION__ );
            memcpy( adf_margins, &adf_margin_table[i], sizeof(struct scan_adf_margins) );
            break;
        }
    }

    return SCANERR_NONE;
}

/**
 * \brief  Set the ADF tof/bof margins.
 *
 */

scan_err_t scanmargin_debug_set_adf_margins(struct scan_adf_margins adf_margin_table[],
                                            struct scan_adf_margins *new_adf_margins)
{
    int i;

    margdbg2("%s: searching for dpi=%d cmode=%d\n", __FUNCTION__, 
                new_adf_margins->dpi, new_adf_margins->cmode);

    for( i=0 ; ; i++ ) {
        /* sanity check against bad lists */
        XASSERT( i<42, i );

        if( adf_margin_table[i].dpi==0 ) {
            return SCANERR_NO_ENTRY;
        }

        if( adf_margin_table[i].dpi==new_adf_margins->dpi &&
            adf_margin_table[i].cmode==new_adf_margins->cmode ) {

            /* note no range checking. enjoy! */
            memcpy( &adf_margin_table[i], new_adf_margins, sizeof(struct scan_adf_margins) );
            break;
        }
    }

    return SCANERR_NONE;
}

