/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "lassert.h"

#include "scancore.h"
#include "scantypes.h"
#include "stripdma.h"
#include "scanlog.h"
#include "piehw.h"
#include "pie.h"
#include "scandbg.h"

#define MAX_QUEUE_DEPTH 6

static void planar_loop_test( struct stripdma_list *stripdma )
{
    scan_err_t scerr;
    int i;

    dbg2( "%s\n", __FUNCTION__ );

    /* successive reads/writes */
    for( i=0 ; i<100 ; i++ ) {
        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_RED );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_GREEN );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_BLUE );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_read( stripdma, PIE_RDMA_RED_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_push_read( stripdma, PIE_RDMA_GREEN_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_push_read( stripdma, PIE_RDMA_BLUE_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }

    stripdma_list_dump( stripdma );
}

static void planar_fill_test( struct stripdma_list *stripdma )
{
    scan_err_t scerr;
    int i;

    dbg2( "%s\n", __FUNCTION__ );

    XASSERT( stripdma->num_pending==0, stripdma->num_pending );

    /* fill it with writes */
    for( i=0 ; i<MAX_QUEUE_DEPTH ; i++ ) {
        dbg2( "%s %d\n", __FUNCTION__, i );
        XASSERT( stripdma->num_pending==i, stripdma->num_pending );

        scerr = stripdma_try_push_write( stripdma, PIE_WDMA_CHANNEL_RED );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_try_push_read( stripdma, PIE_RDMA_RED_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_RED );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_GREEN );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_BLUE );
        XASSERT(scerr==SCANERR_NONE,scerr);

        stripdma_list_dump( stripdma );
    }

    scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_RED );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);

    scerr = stripdma_try_push_write( stripdma, PIE_WDMA_CHANNEL_RED );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);

    /* empty it */
    for( i=0 ; i<MAX_QUEUE_DEPTH ; i++ ) {
        dbg2( "%s %d\n", __FUNCTION__, i );
        XASSERT( stripdma->num_pending==MAX_QUEUE_DEPTH-i, stripdma->num_pending );

        scerr = stripdma_try_push_read( stripdma, PIE_RDMA_RED_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_try_push_read( stripdma, PIE_RDMA_GREEN_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_try_push_read( stripdma, PIE_RDMA_BLUE_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_read( stripdma, PIE_RDMA_RED_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_push_read( stripdma, PIE_RDMA_GREEN_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_push_read( stripdma, PIE_RDMA_BLUE_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }

    /* fill it with reads */
    for( i=0 ; i<MAX_QUEUE_DEPTH ; i++ ) {
        dbg2( "%s %d\n", __FUNCTION__, i );
        XASSERT( stripdma->num_pending==i, stripdma->num_pending );

        scerr = stripdma_try_push_read( stripdma, PIE_RDMA_RED_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_try_push_read( stripdma, PIE_RDMA_RED_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_read( stripdma, PIE_RDMA_RED_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_read( stripdma, PIE_RDMA_GREEN_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_read( stripdma, PIE_RDMA_BLUE_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);

        stripdma_list_dump( stripdma );
    }

    scerr = stripdma_push_read( stripdma, PIE_RDMA_RED_EVEN );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);

    scerr = stripdma_try_push_read( stripdma, PIE_RDMA_RED_EVEN );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);

    /* empty it */
    for( i=0 ; i<MAX_QUEUE_DEPTH ; i++ ) {
        dbg2( "%s %d\n", __FUNCTION__, i );
        XASSERT( stripdma->num_pending==MAX_QUEUE_DEPTH-i, stripdma->num_pending );

        scerr = stripdma_try_push_write( stripdma, PIE_WDMA_CHANNEL_RED );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_try_push_write( stripdma, PIE_WDMA_CHANNEL_GREEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_try_push_write( stripdma, PIE_WDMA_CHANNEL_BLUE );
        XASSERT(scerr==SCANERR_NONE,scerr);

        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_RED );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_GREEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
        scerr = stripdma_push_write( stripdma, PIE_WDMA_CHANNEL_BLUE );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }

    XASSERT( stripdma->num_pending==0, stripdma->num_pending );
}

static void planar_test( void ) 
{
    scan_err_t scerr;
    struct stripdma_list stripdma;
    struct stripdma_strip strip_list[MAX_QUEUE_DEPTH+1];

    dbg2( "%s\n", __FUNCTION__ );

    stripdma_list_init( &stripdma, "stripdma0", MAX_QUEUE_DEPTH+1, strip_list );

    /* test three channels in, three channels out */
    stripdma_set_mask( &stripdma, 
            CHANNEL_TO_MASK(PIE_RDMA_RED_EVEN)  |
            CHANNEL_TO_MASK(PIE_RDMA_GREEN_EVEN)|
            CHANNEL_TO_MASK(PIE_RDMA_BLUE_EVEN),

            CHANNEL_TO_MASK(PIE_WDMA_CHANNEL_RED)  |
            CHANNEL_TO_MASK(PIE_WDMA_CHANNEL_GREEN)|
            CHANNEL_TO_MASK(PIE_WDMA_CHANNEL_BLUE) );
            

    stripdma_list_dump( &stripdma );

    /* push in three channels of write */
    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_RED );
    XASSERT(scerr==SCANERR_NONE,scerr);

    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_GREEN );
    XASSERT(scerr==SCANERR_NONE,scerr);

    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_BLUE );
    XASSERT(scerr==SCANERR_NONE,scerr);

    stripdma_list_dump( &stripdma );

    /* push in three more channels of write */
    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_RED );
    XASSERT(scerr==SCANERR_NONE,scerr);
    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_GREEN );
    XASSERT(scerr==SCANERR_NONE,scerr);
    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_BLUE );
    XASSERT(scerr==SCANERR_NONE,scerr);

    stripdma_list_dump( &stripdma );

    /* we should have three writes queued and no reads */
    XASSERT( stripdma.bottom->mask==7, stripdma.bottom->mask );

    scerr = stripdma_push_read( &stripdma, PIE_RDMA_RED_EVEN );
    XASSERT(scerr==SCANERR_NONE,scerr);
    scerr = stripdma_push_read( &stripdma, PIE_RDMA_GREEN_EVEN );
    XASSERT(scerr==SCANERR_NONE,scerr);
    scerr = stripdma_push_read( &stripdma, PIE_RDMA_BLUE_EVEN );
    XASSERT(scerr==SCANERR_NONE,scerr);

    stripdma_list_dump( &stripdma );
    
    planar_loop_test( &stripdma );

    scerr = stripdma_push_read( &stripdma, PIE_RDMA_RED_EVEN );
    XASSERT(scerr==SCANERR_NONE,scerr);
    scerr = stripdma_push_read( &stripdma, PIE_RDMA_GREEN_EVEN );
    XASSERT(scerr==SCANERR_NONE,scerr);
    scerr = stripdma_push_read( &stripdma, PIE_RDMA_BLUE_EVEN );
    XASSERT(scerr==SCANERR_NONE,scerr);

    stripdma_list_dump( &stripdma );

    /* we should now be empty */
    XASSERT( stripdma.bottom->mask==0, stripdma.bottom->mask );

    planar_fill_test( &stripdma );
}

static void stall_restart_test( void ) 
{
    scan_err_t scerr;
    int i;
    struct stripdma_list stripdma;
    struct stripdma_strip strip_list[MAX_QUEUE_DEPTH+1];

    dbg2( "%s\n", __FUNCTION__ );

    stripdma_list_init( &stripdma, "stripdma0", MAX_QUEUE_DEPTH+1, strip_list );

    /* test using mono channels in/out */
    stripdma_set_mask( &stripdma, 
            CHANNEL_TO_MASK(PIE_RDMA_MONO_EVEN), 
            CHANNEL_TO_MASK(PIE_WDMA_CHANNEL_MONO) );

    XASSERT( stripdma.num_pending==0, stripdma.num_pending );

    /* push two writes */
    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_MONO );
    XASSERT(scerr==SCANERR_NONE,scerr);

    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_MONO );
    XASSERT(scerr==SCANERR_NONE,scerr);

    XASSERT( stripdma.num_pending==2, stripdma.num_pending );
    stripdma_list_dump( &stripdma );

    stripdma_stall( &stripdma );

    stripdma_list_dump( &stripdma );

    /* try to push more writes -- should fail */
    scerr = stripdma_try_push_write( &stripdma, PIE_WDMA_CHANNEL_MONO );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);
    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_MONO );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);

    stripdma_list_dump( &stripdma );

    /* two reads should be allowed then further reads blocked */
    scerr = stripdma_push_read( &stripdma, PIE_RDMA_MONO_EVEN );
    XASSERT(scerr==SCANERR_NONE,scerr);
    XASSERT( stripdma.num_pending==1, stripdma.num_pending );
    scerr = stripdma_push_read( &stripdma, PIE_RDMA_MONO_EVEN );
    XASSERT(scerr==SCANERR_NONE,scerr);
    XASSERT( stripdma.num_pending==0, stripdma.num_pending );

    stripdma_list_dump( &stripdma );

    /* no more reads or writes allowed even though the list is empty */
    scerr = stripdma_push_read( &stripdma, PIE_RDMA_MONO_EVEN );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);
    scerr = stripdma_try_push_read( &stripdma, PIE_RDMA_MONO_EVEN );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);
    scerr = stripdma_try_push_write( &stripdma, PIE_WDMA_CHANNEL_MONO );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);
    scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_MONO );
    XASSERT(scerr==SCANERR_NO_ENTRY,scerr);

    stripdma_list_dump( &stripdma );

    /* allow the DMAs to refill */
    stripdma_reload( &stripdma );
    stripdma_list_dump( &stripdma );

    for( i=0 ; i<MAX_QUEUE_DEPTH ; i++) {
        XASSERT( stripdma.num_pending==i, stripdma.num_pending );
        scerr = stripdma_push_write( &stripdma, PIE_WDMA_CHANNEL_MONO );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }
    stripdma_list_dump( &stripdma );

    for( i=0 ; i<MAX_QUEUE_DEPTH ; i++) {
        XASSERT( stripdma.num_pending==MAX_QUEUE_DEPTH-i, stripdma.num_pending );
        scerr = stripdma_push_read( &stripdma, PIE_RDMA_MONO_EVEN );
        XASSERT(scerr==SCANERR_NONE,scerr);
    }
}

void stripdma_run_tests( void )
{
    planar_test();

    stall_restart_test();
}

#if defined __APPLE__ || defined __linux__ && !defined __KERNEL__
int main( void )
{
    scanlog_set_level(2);
    
    stripdma_run_tests();

    return EXIT_SUCCESS;
}
#endif

