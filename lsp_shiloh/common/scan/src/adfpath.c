/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file adfpath.c
 *
 * \brief Functions used by scanpipe.c for monitoring paper through the ADF
 * path.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanvars.h"
#include "scanlog.h"
#include "scanplat.h"
#include "adfsensor.h"
#include "adfpath.h"
#include "scanmech.h"

void adf_paper_path_debug_log( struct adf_paper_path *adfpath )
{
    dbg1( "%s in_page=%d row_counter=%d tof=%d bof=%d use_duplex=%d duplex_page_side=%d\n", __FUNCTION__, 
        adfpath->in_page, adfpath->row_counter, 
        adfpath->tof_marker, adfpath->bof_marker, 
        adfpath->use_duplex, adfpath->duplex_page_side 
    );
}

/**
 * \brief  Initialize the ADF paper watcher 
 *
 * Need to track the adf paper state so we can cut the pages at tof/bof.
 *
 * (Rolled a bunch of code into this function.)
 *
 * \author David Poole
 * \date 2*-Jul-2009
 *
 */

void adf_paper_path_init( const struct scanvars *sv, struct adf_paper_path *adfpath )
{
    scan_err_t scerr;
    uint32_t dpi_horiz, dpi_vert;

    memset( adfpath, 0, sizeof(struct adf_paper_path) );
    if( adf_sensor_paper_in_path() ) {
        adfpath->in_page = true;
    }

    scanvar_get_hw_dpi( sv, &dpi_horiz, &dpi_vert );

    dbg2("%s in_page=%d srcdpi=%d hw_dpi=%d\n", __FUNCTION__, 
            adfpath->in_page,sv->dpi, dpi_horiz);
    
    /* page_side==0 means "uninitialized". Set to side 1. */
    adfpath->duplex_page_side = 1;

    scanvar_get_pie_scale( sv, &adfpath->scale.x_numer, 
                               &adfpath->scale.x_denom, 
                               &adfpath->scale.y_numer, 
                               &adfpath->scale.y_denom ); 


    if( sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF_DUPLEX ) {
        adfpath->use_duplex = true;
    }

    /* get a full copy of the margin settings for this dpi/cmode */
    if( adfpath->use_duplex ) {
        scerr = scanmech_get_adf_duplex_margins( dpi_horiz, sv->cmode, &adfpath->adf_margins );
    }
    else {
        scerr = scanmech_get_adf_margins( dpi_horiz, sv->cmode, &adfpath->adf_margins );
    }
    XASSERT( scerr==0, scerr );
}

