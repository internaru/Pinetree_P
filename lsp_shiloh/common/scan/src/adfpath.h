/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef ADFPATH_H
#define ADFPATH_H

/* Structure shared between scanpipe.c and adfgapper.c. Holds useful state of
 * the adf paper path
 */

struct adf_paper_path {
    bool in_page;

    int row_counter;

    /* Catch paper jam, mispick, and end of input. Save the row number of the
     * most recent TOF and BOF seen.  
     *
     * If our current row is too far from our most recent BOF, we're at the end
     * of input.
     *
     * If our current row is too far from our most recent TOF, we've jammed.
     *
     * If our tof_marker is zero (no TOF) and our row count gets to a
     * certain point, we have a mispick (no paper).
     *
     */
    int tof_marker;
    int bof_marker;

    /* davep 18-Jul-2009 ; add scaling to the pipe cutter; rather than having to
     * do scanvar_get() each time, grab the scale factors and store here for
     * later use by the pipe cutter
     */
    struct scan_scale scale;

    /* davep 13-Oct-2011 ; copy entire adf margin structure for this dpi/cmode
     * here
     */
    struct scan_adf_margins adf_margins;

    /* davep 08-Sep-2011 ; adding duplex support */
    bool use_duplex;
    int duplex_page_side; /* sides in [1,2] (0==uninitialized) */
};

void adf_paper_path_init( const struct scanvars *sv, struct adf_paper_path *adfpath );
void adf_paper_path_debug_log( struct adf_paper_path *adfpath );

#endif

