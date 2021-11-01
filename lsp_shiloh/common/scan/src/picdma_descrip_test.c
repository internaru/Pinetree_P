/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file picdma_descrip_test.c
 *
 * \brief Test the Descriptor based PIC Write DMA Driver.
 *
 * davep 28-Mar-2010 ; Added as for new descriptor DMA
 *
 * davep 30-Apr-2010 ; Separated from picdma_descrip.c
 *
 */

#include <stdint.h>

#include "scos.h"

#include "regAddrs.h"
#include "lassert.h"
#include "interrupt_api.h"
#include "intnums.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "list.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "ddma.h"
#include "pichw.h"
#include "pic.h"
#include "piccbi.h"
#include "pictest.h"
#include "picdma_descrip.h"
#include "picdma_descrip_test.h"
#include "scanmem.h"

#define __DEL_XASSERT__  (1)
extern void scanlib_send_sc(uint8_t sc );

static struct ddma_channel test_desc_channel_list[PIC_WDMA_NUM_CHANNELS];

static struct pic_interrupt_stats test_pic_istats;

static scan_err_t setup_descriptors_for_transfer( struct ddma_channel *dch, 
                                            uint8_t *buf, int buflen_bytes,
                                            int bytes_per_transfer )
{
    scan_err_t scerr;
    uint8_t *ptr8;
    int bytes_remaining;
    struct ddma_descriptor *desc;
    int i;
    int num_descriptors;

    /* 
     * This function only used in test code. 
     *
     * Called from picdma_descrip_test()->dma_test().
     */

    dbg2( "%s buf=%p buflen=%d bytes_per_transfer=%d\n", 
                __FUNCTION__, buf, buflen_bytes, bytes_per_transfer );

    ptr8 = buf;
    bytes_remaining = buflen_bytes;

    num_descriptors = buflen_bytes / bytes_per_transfer;
    if( buflen_bytes%bytes_per_transfer != 0 ) {
        num_descriptors += 1;
    }
    XASSERT( num_descriptors*bytes_per_transfer >= buflen_bytes, num_descriptors );

    dbg2( "%s alloc num_desc=%d bytes=%d\n", __FUNCTION__, num_descriptors, 
                num_descriptors*sizeof(struct ddma_descriptor) );

    /* davep 26-Apr-2012 ; change to the ddma.c functions so I can test the
     * ddma.c functions, too
     */
    scerr = ddma_channel_alloc_descriptors( dch, num_descriptors );
    if( scerr != SCANERR_NONE ) {
        /* ddma function logs error */
        return scerr;
    }

    /* initialize the descriptors to point to our data */
    for( i=0 ; i<dch->desc_list.num_descriptors ; i++ ) {
        if( bytes_remaining <= 0 ) {
            break;
        }
        desc = &dch->desc_list.list[i];

        /* don't memset the descriptor; the ddma alloc function should have
         * initialized parts of it
         */

        desc->transfer_len_bytes = MIN( bytes_remaining, bytes_per_transfer );

        desc->src_addr = (uint32_t)ptr8;

        bytes_remaining -= desc->transfer_len_bytes;
        ptr8 += desc->transfer_len_bytes;

        /* we want slightly different flags for our test */
        desc->config_flags = DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK
//                           |  DDMA_DESCRIPTOR_CONFIG_SOI
//                           |  DDMA_DESCRIPTOR_CONFIG_INT
                           ;

        /* davep 11-May-2012 ; sanity test my new ddma descriptor function;
         * make sure new dma pointer initialized correctly
         */
        XASSERT( desc->dma_ptr_self, (uint32_t)desc );

        if( bytes_remaining > 0 ) {
            ASSERT( desc->fw_next_descriptor_addr );
            ASSERT( desc->dma_next_descriptor_addr );
        }
        else {
            /* last link in the chain, want an interrupt here */
            desc->config_flags |= DDMA_DESCRIPTOR_CONFIG_INT;
            /* davep 23-Apr-2012 ; yeah, we should probably stop here, too */
            desc->config_flags |= DDMA_DESCRIPTOR_CONFIG_STOP;
        }

        /* flush the descriptor from the cache to main mem where dma can find it */
        ddma_desc_flush( desc );

        ddma_desc_dump( desc );
    }

    /* did we have enough descriptors in our array to handle this request? */
    XASSERT( bytes_remaining==0, bytes_remaining );

    /* success! */
    return SCANERR_NONE;
}

static void validate_8bit( uint8_t *buf, int buflen, uint8_t expected_value )
{
    /* TODO */
}

static void validate_16bit( uint8_t *buf, int buflen, uint16_t expected_value )
{
    uint16_t *ptr16, *endptr16;

    /* better have an even number of bytes, eh? */
    XASSERT( buflen%2==0, buflen );
    ptr16 = (uint16_t *)buf;
    endptr16 = (uint16_t *)(buf + buflen );

    while( ptr16 < endptr16 ) {
        if( *ptr16 != expected_value ) {
            dbg1( "%s invalid value %d found at %p offset %d\n", __FUNCTION__,
                        *ptr16, ptr16, (uint8_t*)ptr16 - buf );
            XASSERT( 0, (uint32_t)*ptr16 );
        }
        ptr16++;
    }
}

static void channel_test_cleanup( struct ddma_channel *dch )
{
    dbg2( "%s channel=%d\n", __FUNCTION__, dch->channel );

    XASSERT( dch->is_open, dch->channel );

    ddma_descriptor_list_free( &dch->desc_list );
    ddma_channel_close( dch );
}

static scan_err_t dma_test( scan_cmode_t cmode, int pixels_per_row, int bits_per_pixel, int total_rows )
{
    scan_err_t scerr, final_scerr;
    int i, bytes_per_row, buflen_bytes;
    int bytes_per_transfer;
    struct ddma_channel *dch;
    uint8_t channels[PIC_WDMA_NUM_CHANNELS];
    uint8_t num_channels;
    struct dma_ptr buf_list[PIC_WDMA_NUM_CHANNELS];
    uint16_t pixel;
    int color_list[3], color_idx;
    int row_idx;
    uint32_t pixel_count;
    char name[DDMA_NAME_LEN+1];

    dbg2( "%s cmode=%d ppr=%d bpr=%d total_rows=%d\n", __FUNCTION__, 
                cmode, pixels_per_row, bits_per_pixel, total_rows );

    memset( buf_list, 0, sizeof(buf_list) );
    memset( channels, 0, sizeof(channels) );
    memset( test_desc_channel_list, 0, sizeof(test_desc_channel_list) );

    pic_setup_for_test();
    pic_hs_set_bypass( true );
    pic_bm_set_bypass( true );
    pic_prnudsnu_set_bypass_all( true );
    pic_prnudsnu_set_bypass_prnu( true );
    pic_prnudsnu_set_bypass_dsnu( true );
    pic_prnudsnu_set_bypass_bad_pix_replace( true );
    pic_prnudsnu_set_bypass_exp_comp( true );

    pic_wdma_init_routing();

    switch( bits_per_pixel ) {
        case 8 :
            pic_wdma_set_bitpack_mode( PIC_BITPACK_8BIT );
            break;

        case 16 :
            pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );
            break;

        default :
            XASSERT(0,bits_per_pixel);
            return SCANERR_INVALID_PARAM;
    }

    pic_cwdma_set_config(PIC_WDMA_DWOCR_DM_ACCEPT);

//    /* XXX shotgun debug */
//    pic_adcnorm_set_bypass( true );

//    pic_dump();

    if( cmode==SCAN_CMODE_MONO ) {
        num_channels = 1;
    }
    else {
        num_channels = 3;
    }

    /* FIXME change rhs to use #def instead of magic numbers */
    memset( channels, 0, sizeof(channels) );
    if( num_channels==3 ) {
        channels[0] = 0;
        channels[1] = 1;
        channels[2] = 2;
    }
    else {
        channels[0] = 0;
    }

//    pixels_per_row = 1024;
//    bits_per_pixel = 8;
//    bits_per_pixel = 16;
//    total_rows = 32;
    bytes_per_transfer = 4096;

    bytes_per_row = pixels_per_row * (bits_per_pixel/8);
    buflen_bytes = bytes_per_row * total_rows;

    /* allocate memory for destination */
    for( i=0 ; i<num_channels ; i++ ) {
        buf_list[i].size = buflen_bytes;
#ifdef __KERNEL__
        buf_list[i].ptr = dma_alloc_coherent( NULL, buflen_bytes, &buf_list[i].dma_handle, GFP_DMA );
#else
        buf_list[i].ptr = ice_dma_alloc_buffer( buflen_bytes );
        buf_list[i].dma_handle = buf_list[i].ptr;
#endif
        if( buf_list[i].ptr==NULL ) {
            /* failure! clean up and leave */
            dbg2( "%s out of memory at buffer %d of %d\n", __FUNCTION__, i, num_channels );
            final_scerr = SCANERR_OUT_OF_MEMORY;
            goto fail_free_buf_list;
        }

        dbg2( "%s buf[%d]=%p handle=0x%x\n", __FUNCTION__, i, buf_list[i].ptr, buf_list[i].dma_handle );

        /* init to a value so we can see changes */
        memset( buf_list[i].ptr, 0xee, buflen_bytes );
        cpu_dcache_writeback_region( buf_list[i].ptr, buflen_bytes );
        cpu_dcache_invalidate_region( buf_list[i].ptr, buflen_bytes );

        scanlog_hex_dump( buf_list[i].ptr, 64 );
    }

    /* turn off all our channels, set up for our transfer */
    for( i=0 ; i<num_channels ; i++ ) {
        pic_wdma_channel_disable( channels[i] );
        pic_wdma_channel_dump( channels[i] );
    }

    /* break up destination memory into a series of descriptors */
    for( i=0 ; i<num_channels ; i++ ) {
        dch = &test_desc_channel_list[i];
        strncpy( name, "picwdma ", DDMA_NAME_LEN );
        name[7] = '0' + i;
        scerr = ddma_channel_open( dch, name, i );
        if( scerr != SCANERR_NONE ) {
            final_scerr = scerr;
            goto fail_cleanup_channels; 
        }

        scerr = setup_descriptors_for_transfer( dch, (uint8_t*)buf_list[i].dma_handle,
                                                 buflen_bytes, bytes_per_transfer ); 
        if( scerr != SCANERR_NONE ) {
            dbg2( "%s setup_descriptors failed scerr=%d\n", __FUNCTION__, scerr );
            final_scerr = scerr;
            goto fail_cleanup_channels; 
        }
    }

    for( i=0 ; i<num_channels ; i++ ) {

        pic_wdma_channel_enable( channels[i] );

        /* write the first descriptor to the dma channel, starting the transaction */
        dch = &test_desc_channel_list[ channels[i] ];
        XASSERT( dch->is_open, dch->channel );

        pic_ddma_desc_write( channels[i], &dch->desc_list.list[0], bytes_per_row );

        pic_wdma_channel_dump( channels[i] );
    }

    /* we depend on the interrupt counts to match our setup so start from zero
     * every time
     */
    memset( &test_pic_istats, 0, sizeof(test_pic_istats) );

    /* fire in the hole! */
    dbg1( "%s enabling PIC interupt\n", __FUNCTION__ );
    pic_interrupt_enable();

    dbg2( "%s write to ant\n", __FUNCTION__ );

    pixel = 0x1234;
    if( num_channels==3 ) {
        color_list[0] = PIC_CBI_CEVEN_0;
        color_list[1] = PIC_CBI_CEVEN_1;
        color_list[2] = PIC_CBI_CEVEN_2;
    }
    else {
        color_list[0] = PIC_CBI_MEVEN;
    }

    pixel_count = 0;

    /* send all our data */
    for( row_idx=0 ; row_idx<total_rows ; row_idx++ ) {

        dbg2( "%s row=%d\n", __FUNCTION__, row_idx );

        /* send the first pixel */
        for( color_idx=0 ; color_idx<num_channels ; color_idx++ ) {
            /* first pixel in non-last line */
            pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRP, color_list[color_idx], pixel );
            pixel_count++;
        }

        /* the -2 is for the first and last pixels */
        for( i=0 ; i<pixels_per_row-2 ; i++ ) {

//            dbg2( "%s i=%d\n", __FUNCTION__, i );

            for( color_idx=0 ; color_idx<num_channels ; color_idx++ ) {

//                dbg2( "%s color_idx=%d\n", __FUNCTION__, color_idx );

                pic_adcnorm_ant( PIC_ADCN_TEST_DT_NORMP, color_list[color_idx], pixel );
                pixel_count++;

//                /* XXX temp debug */
//                dma_regs = pic_cwdma_get_regs( channels[color_idx] );
//                dbg2( "%d %#08x %#08x %#08x\n", color_list[color_idx],
//                            *PIC_ADCN_ANIP, 
//                            dma_regs->status, 
//                            dma_regs->xfer_length ); 
            }
        }

        /* send the last pixel */
        for( color_idx=0 ; color_idx<num_channels ; color_idx++ ) {
            pic_adcnorm_ant( PIC_ADCN_TEST_DT_LASTP, color_list[color_idx], pixel );
            pixel_count++;
        }
    }

    dbg2( "%s pixel_count=%d expected=%d\n", __FUNCTION__, pixel_count, pixels_per_row*total_rows*num_channels );

    /* we're now done; prepare for validation and cleanup */
    pic_interrupt_disable();

    /* we should have gotten three interrupts, one for each channel */
    for( i=0 ; i<num_channels ; i++ ) {
        XASSERT( test_pic_istats.dma[i]==1, test_pic_istats.dma[i] );
    }

    /* shut everything down */
    for( i=0 ; i<num_channels ; i++ ) {
        pic_wdma_channel_disable( i );
        pic_wdma_channel_dump( channels[i] );
    }

    /* validation */
    for( i=0 ; i<num_channels ; i++ ) {

        cpu_dcache_invalidate_region( buf_list[i].ptr, buflen_bytes );

        if( bits_per_pixel==8 ) {
            validate_8bit( buf_list[i].ptr, buflen_bytes, pixel );
        }
        else {
            validate_16bit( buf_list[i].ptr, buflen_bytes, pixel );
        }
        
        /* peek at a chunk at either end of buffer */
        scanlog_hex_dump( buf_list[i].ptr, 64 );
        scanlog_hex_dump( buf_list[i].ptr+buflen_bytes-64, 64 );
    }

//    pic_dump();

    dbg2( "%s success!\n", __FUNCTION__ );
    final_scerr = SCANERR_NONE;

fail_cleanup_channels : 
    for( i=0 ; i<num_channels ; i++ ) {
        dch = &test_desc_channel_list[ channels[i] ];
        if( dch->is_open ) {
            channel_test_cleanup( dch );
        }
    }

fail_free_buf_list : 
    /* free our allocated memory */
    for( i=0 ; i<num_channels ; i++ ) {
        if( buf_list[i].ptr ) {
#ifdef __KERNEL__
            dma_free_coherent( NULL, buf_list[i].size, 
                                    buf_list[i].ptr, buf_list[i].dma_handle );
#else
            PTR_FREE( buf_list[i].ptr );
#endif
        }
    }

    pic_cleanup_after_test();

    return final_scerr;
}

static scan_err_t run_dma_tests( void )
{
    scan_err_t scerr;
    scan_cmode_t cmode;
    int pixels_per_row, total_rows;
    
    dbg2( "%s\n", __FUNCTION__ );

//    cmode = SCAN_CMODE_MONO;
    cmode = SCAN_CMODE_COLOR;
    pixels_per_row = 1024;
//    bits_per_pixel = 8;
//    total_rows = 200;
    total_rows = 32;

    scerr = dma_test( cmode, pixels_per_row, 8, total_rows );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s %d dma_test failed scerr=%d\n", __FUNCTION__, __LINE__, scerr  );
        return scerr;
    }

    scerr = dma_test( cmode, pixels_per_row, 16, total_rows );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s %d dma_test failed scerr=%d\n", __FUNCTION__, __LINE__, scerr  );
        return scerr;
    }

    /* Switch to mono, run the same tests. Glee! */
    cmode = SCAN_CMODE_MONO;
    scerr = dma_test( cmode, pixels_per_row, 8, total_rows );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s %d dma_test failed scerr=%d\n", __FUNCTION__, __LINE__, scerr  );
        return scerr;
    }

    scerr = dma_test( cmode, pixels_per_row, 16, total_rows );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s %d dma_test failed scerr=%d\n", __FUNCTION__, __LINE__, scerr  );
        return scerr;
    }

    return 0;
}

#ifdef __KERNEL__
static irqreturn_t tst_pic_interrupt( int irq, void *dev_id ) 
#else
static void tst_pic_interrupt( uint32_t ignore )
#endif
{
    uint32_t pic_ipend, adc_ipend;
    int i;
    struct pic_cwdma_regs *dma_regs;

    /* 
     * BIG FAT NOTE!  This is an interrupt handler!!
     */

    /* read PIC's pending interrupts */
    pic_ipend = *PIC_CI_IPR;

    /* read the ADC's pending interrupts */
    adc_ipend = *PIC_ADCN_ANIP;

    test_pic_istats.count++;

//    dbg2( "%s %#x %#x\n", __FUNCTION__, pic_ipend, adc_ipend );

    for( i=0 ; i<PIC_WDMA_NUM_CHANNELS ; i++ ) {
        dma_regs = pic_cwdma_get_regs(i);
//        dbg2( "%s %d %#x %#x\n", __FUNCTION__, i, dma_regs->int_en, dma_regs->int_pend );

        /* if we have a pending "Transfer End" interrupt, clear it */
        if( dma_regs->int_pend & PIC_WDMA_INT_PEND_FIN) {
            dma_regs->int_ack |= PIC_WDMA_INT_ACK_FIN; 

            test_pic_istats.dma[i]++;
        }

        /* davep 25-Jul-2011 ; ack a reset */
        if( dma_regs->int_pend & PIC_WDMA_INT_PEND_RST ) {
            dma_regs->int_ack |= PIC_WDMA_INT_ACK_RST; 
        }

        /* davep 25-Jul-2011 ; die hard on anything unexpected */
#if __DEL_XASSERT__
    if( !(dma_regs->int_pend & ~(PIC_WDMA_INT_PEND_FIN|PIC_WDMA_INT_PEND_RST)) ) {
        // no action
    }
    else {
        dbg1("XASSERT(%d,%d)\n", __LINE__, dma_regs->int_pend);
        scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
    }
#else /* __DEL_XASSERT__ */
        XASSERT( !(dma_regs->int_pend & ~(PIC_WDMA_INT_PEND_FIN|PIC_WDMA_INT_PEND_RST)), dma_regs->int_pend );
#endif /* __DEL_XASSERT__ */
    }

    if( pic_ipend & PIC_CI_COMBUSOI_BIT ) {
        /* fail hard on overflow */
        dbg1( "ERROR! %s OVERFLOW pic_ipend=%#x adc_ipend=%#x\n", 
                    __FUNCTION__, pic_ipend, adc_ipend );

        pic_dump();
#if __DEL_XASSERT__
		scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
#else /* __DEL_XASSERT__ */
        XASSERT( 0, pic_ipend );
#endif /* __DEL_XASSERT__ */
    }

    if( adc_ipend & PIC_ADCN_ANI_OVERRUN) {
        /* fail hard on overflow */
#if __DEL_XASSERT__
		dbg1( "ERROR! %s OVERFLOW pic_ipend=%#x adc_ipend=%#x\n", 
                    __FUNCTION__, pic_ipend, adc_ipend );
		scanlib_send_sc(SMSG_SC_SCAN_ASSERT_FAIL);
#else /* __DEL_XASSERT__ */
        XASSERT( 0, adc_ipend );
#endif /* __DEL_XASSERT__ */
    }

#ifdef __KERNEL__
    return IRQ_HANDLED;
#endif
}

#ifdef __KERNEL__
static scan_err_t picdma_descrip_test_capture_interrupt( void )
{
    int retcode;

    /* wait until explicitly enabled */
    pic_interrupt_disable();

    retcode = request_irq( INTNUM_PIC, tst_pic_interrupt, 0, "icepic", NULL );
    dbg2( "%s request_irq retcode=%d\n", __FUNCTION__, retcode );

    return SCANERR_NONE;
}
#else
static scan_err_t picdma_descrip_test_capture_interrupt( void )
{
    intAttach( INTNUM_PIC, 0, tst_pic_interrupt, 0 );
    return SCANERR_NONE;
}
#endif

scan_err_t picdma_descrip_test( void )
{
    scan_err_t scerr, final_scerr;

    dbg2( "%s\n", __FUNCTION__);

    /* sanity check for swapping pointers / integers back and forth */
    XASSERT( sizeof(uint32_t)==sizeof(struct ddma_descriptor *), 
                sizeof(struct pic_cwmda_descriptor *) );

    dbg2( "%s steal the PIC interrupt\n", __FUNCTION__ );
    pic_release_interrupt();

    picdma_descrip_test_capture_interrupt();

    pic_wdma_reset();

    scerr = run_dma_tests();
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s run_dma_tests failed scerr=%d\n", __FUNCTION__, scerr );
        /* ignore error and continue to clean up */
    }
    final_scerr = scerr;

    dbg2( "%s restore PIC interrupt\n", __FUNCTION__ );
    pic_release_interrupt();

    /* restore PIC interrupt to proper handler */
    pic_capture_interrupt();

    pic_wdma_reset();

    if( final_scerr==SCANERR_NONE ) {
        dbg1( "%s all tests completed successfully\n", __FUNCTION__ );
    }
    else {
        dbg1( "%s FAILED with scerr=%d\n", __FUNCTION__, final_scerr );
    }

    dbg2( "%s done\n", __FUNCTION__ );

    return SCANERR_NONE;
}

