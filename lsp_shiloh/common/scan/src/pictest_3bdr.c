/**
 * \file picdma_descrip.c
 *
 * \brief PIC Write DMA driver for chained descriptor based DMA hardware
 *
 * davep 02-Apr-2010 ; adding 3-lut BDR test code
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>

#include "scos.h"

#include "list.h"
#include "regAddrs.h"
#include "lassert.h"
#include "debug.h"
#include "memAPI.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "pichw.h"
#include "pic.h"
#include "pictest.h"

static scan_err_t bdr_test_luts( void )
{
    uint32_t *lut; 
    volatile uint32_t *ptr32;
    int lut_numbytes;
    int i;

    lut_numbytes = sizeof(uint32_t)*PIC_BDR_LUT_SIZE;

    lut = MEM_MALLOC( lut_numbytes );
    if( lut==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    dbg2( "%s load color0 LUT\n", __FUNCTION__ );
    memset( lut, 0xff, lut_numbytes );
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR0MONO );
    pic_bdr_enable_fw_access( true );
    pic_bdr_set_lut( lut );
    pic_bdr_enable_fw_access( false );

//    dbg2( "%s dump color0\n", __FUNCTION__ );
//    pic_bdr_dump(true);

    dbg2( "%s load color1 LUT\n", __FUNCTION__ );
    memset( lut, 0xee, lut_numbytes );
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR1 );
    pic_bdr_enable_fw_access( true );
    pic_bdr_set_lut( lut );
    pic_bdr_enable_fw_access( false );
//    dbg2( "%s dump color1\n", __FUNCTION__ );
//    pic_bdr_dump(true);

//    dbg2( "%s dump color0\n", __FUNCTION__ );
//    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR0MONO );
//    pic_bdr_dump(true);

    dbg2( "%s load color2 LUT\n", __FUNCTION__ );
    memset( lut, 0xdd, lut_numbytes );
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR2 );
    pic_bdr_enable_fw_access( true );
    pic_bdr_set_lut( lut );
    pic_bdr_enable_fw_access( false );
//    dbg2( "%s dump color2\n", __FUNCTION__ );
//    pic_bdr_dump(true);

    /* verify! */
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR0MONO );
    pic_bdr_enable_fw_access( true );
    ptr32 = PIC_BDR_LUT;
    for( i=0 ; i<PIC_BDR_LUT_SIZE ; i++ ) {
        XASSERT( *ptr32==0xff, *ptr32 );
    }
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR1 );
    ptr32 = PIC_BDR_LUT;
    for( i=0 ; i<PIC_BDR_LUT_SIZE ; i++ ) {
        XASSERT( *ptr32==0xee, *ptr32 );
    }
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR2 );
    ptr32 = PIC_BDR_LUT;
    for( i=0 ; i<PIC_BDR_LUT_SIZE ; i++ ) {
        XASSERT( *ptr32==0xdd, *ptr32 );
    }
    pic_bdr_enable_fw_access( false );

    PTR_FREE( lut );

    return 0;
}

void bdr_3lut_test( void )
{
    dbg2( "%s\n", __FUNCTION__ );

    pic_bdr_set_bypass( true );
    pic_bdr_dump(false);
    pic_bdr_set_bypass( false);
    pic_bdr_dump(false);

    pic_bdr_enable_fw_access( true );
    pic_bdr_dump(false);
    pic_bdr_enable_fw_access( false );
    pic_bdr_dump(false);

    pic_bdr_enable_fw_access( true );
    pic_bdr_set_num_luts( 3 );
    pic_bdr_dump(false);
    XASSERT( *PIC_BDR_BDRCR & PIC_BDR_BDRCR_NUMLUTS(PIC_BDR_BDRCR_NUMLUTS_THREE), *PIC_BDR_BDRCR );

    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR0MONO );
    pic_bdr_dump(false);
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR1 );
    pic_bdr_dump(false);
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR2 );
    pic_bdr_dump(false);

//    pic_bdr_dump(true);

    bdr_test_luts();
}

