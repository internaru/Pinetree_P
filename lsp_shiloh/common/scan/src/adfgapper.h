/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef ADFGAPPER_H
#define ADFGAPPER_H

struct adf_page_gapper 
{
    bool stop_at_next_page_gap;
    bool resume_on_mech_ready;

    /* Record the distance we have traveled in one big move. If we exceed a
     * certain distance, we must pause to prevent motor controller rollover.
     */
    uint32_t distance_traveled_rows;

    /* If distance_traveled_rows exceeds this value, we pause at a page gap.
     * We'll do a new move to restart the encoders.
     */
    uint32_t max_distance_traveled_rows;

    /* if free memory drops below this value, we will pause at a page gap. We
     * won't resume until free memory is again above this level (we can't pause
     * the adf so if memory is running low, we pause between pages to hopefully
     * recover memory).
     */
    uint32_t required_memory_for_next_page;
};

void adf_page_gapper_init( struct adf_page_gapper *adfgapper, 
                           uint32_t low_memory_trigger, uint32_t max_rows_trigger );
scan_err_t adf_page_gapper_resume( struct adf_page_gapper *adfgapper, 
                                   struct adf_paper_path *adfpath );
void adf_page_gapper_update_distance( struct adf_page_gapper *adfgapper, 
                                      struct adf_paper_path *adfpath, 
                                      uint32_t num_rows );
scan_err_t adf_page_gapper_test_for_pause( struct adf_page_gapper *adfgapper, 
                                           struct adf_paper_path *adfpath );
scan_err_t adf_page_gapper_poll( struct adf_page_gapper *adfgapper, 
                                 struct adf_paper_path *adfpath );

scan_err_t adf_page_gapper_client_pause( void );
scan_err_t adf_page_gapper_client_resume( void );

#endif

