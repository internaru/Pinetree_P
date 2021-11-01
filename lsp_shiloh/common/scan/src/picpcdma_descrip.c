/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file picpcdma_descrip.c
 *
 * \brief  Descriptor based PIC PC (Pixel Correction, aka PRNU/DSNU) LUT DMA
 *
 * Also known as "IDMA 2D DMA for PRNU/DSNU"
 *
 * davep 06-Aug-2010
 *
 **/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "hwconfig_api.h"
#include "cpu_api.h"
#include "lassert.h"
#include "regAddrs.h"
#include "debug.h"
#include "list.h"
#include "IDMA_2D_regmasks.h"
#include "IDMA_2D_regstructs.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "ddma.h"
#include "pichw.h"
#include "pic.h"
#include "picdma_descrip.h"
#include "scansen.h"

/* fix bletcherousnous in header files */
#ifdef ICE_PIC_PRNUDSNU_IDMA_2D0_BASE
#define ICE_PIC_PRNUDSNU_IDMA_2D_BASE ICE_PIC_PRNUDSNU_IDMA_2D0_BASE
#endif
/* davep 31-Jan-2013 ; they changed the names again */
#ifdef ICE_PIC_PRNUDSNU_S_IDMA_2D_BASE 
#define ICE_PIC_PRNUDSNU_IDMA_2D_BASE ICE_PIC_PRNUDSNU_S_IDMA_2D_BASE 
#endif

/* davep 15-Jan-2013 ; adding support for >1 prnudsnu idma */
static IDMA_2D_REGS_t * const idma2d_regs_array[] = {
#ifdef HAVE_NSENSOR_SUPPORT
    (IDMA_2D_REGS_t *)ICE_PIC_PRNUDSNU_IDMA_2D0_BASE,
    (IDMA_2D_REGS_t *)ICE_PIC_PRNUDSNU_IDMA_2D1_BASE,
#else
    (IDMA_2D_REGS_t *)ICE_PIC_PRNUDSNU_IDMA_2D_BASE,
#endif
    0 /* end of list */
};

/* davep 15-Jan-2013 ; TODO add 2nd descriptor for 2nd sensor PRNU/DSNU DMA */
static IDMA_2D_REGS_t * idma2d_regs = (IDMA_2D_REGS_t *)ICE_PIC_PRNUDSNU_IDMA_2D_BASE;


/* PIC WDMA and PIC PC LUT DMA use the same descriptor format so borrow that
 * data struture. We only need one descriptor because the PC LUT DMA simply
 * repeats
 */
// created descriptor array for nsensor
static struct ddma_descriptor *idma2d_desc_array[SCANSEN_MAX_SENSORS];
static struct ddma_descriptor *idma2d_desc = NULL; // cause error if block not properly set
static int current_block_num=0; // icky, but we need to let callers restore the block after resets

void pic_prnudsnu_dma_dump( void )
{
    dbg2( "      cfg=%08x   status=%08x line_width=%08x\n", 
                idma2d_regs->cfg, idma2d_regs->status, 
                idma2d_regs->line_width );
    dbg2( "   int_en=%08x   int_st=%08x\n", 
                idma2d_regs->int_en, idma2d_regs->int_st );
    dbg2( "desc_read=%08x xfer_len=%08x   xferaddr=%08x\n", 
                idma2d_regs->desc_read, idma2d_regs->xfer_length, 
                idma2d_regs->xfer_addr );

    ddma_desc_dump( idma2d_desc );
}

void pic_prnudsnu_enable_lut_dma( bool enable )
{
    if( enable ) {
        idma2d_regs->cfg = IDMA_2D_CFG_ENABLE_REPLACE_VAL( idma2d_regs->cfg, 1 );
    }
    else {
        idma2d_regs->cfg = IDMA_2D_CFG_ENABLE_REPLACE_VAL( idma2d_regs->cfg, 0 );
    }
}

void pic_prnudsnu_reset_lut_dma( void )
{
    uint32_t num32;

    /* disable */
    pic_prnudsnu_enable_lut_dma( false );    

    /* reset the block */
    idma2d_regs->reset |= IDMA_2D_RESET_SOFT_RESET_MASK; 
    cpu_spin_delay( 1 ); /* hold it for "a little while" (XXX-necessary?) */
    idma2d_regs->reset &= ~IDMA_2D_RESET_SOFT_RESET_MASK; 

    /* ack all outstanding interrupts */
    idma2d_regs->int_cl = ~0;

    /* write registers back to power-on defaults */
    idma2d_regs->int_en = 0;
    idma2d_regs->line_width = 0;

    /* address location verification */
    num32 = IDMA_2D_STATUS_SOFTRESET_MASK 
        | IDMA_2D_STATUS_EMPTY_CBUF_MASK 
        | IDMA_2D_STATUS_EMPTY_DBUF_MASK 
        | IDMA_2D_STATUS_PACKER_EMPTY_MASK 
        ;
    XASSERT( idma2d_regs->status==num32, idma2d_regs->status );


    /* davep 04-Jan-2013 ; shotgun debug; hardwire 2nd channel disabled */
//    idma2d_regs_1->cfg = IDMA_2D_CFG_ENABLE_REPLACE_VAL( idma2d_regs_1->cfg, 0 );

//    pic_prnudsnu_dma_dump();
}

void pic_prnudsnu_set_lut_addr( dma_addr_t lut_addr )
{
    /* make sure pointer is correctly longword (32-bit) aligned */
//    XASSERT( ((uint32_t)lut_addr & ~ICE_DMA_BYTE_ALIGN_MASK) == 0, (uint32_t)lut_addr );
	
    idma2d_desc->src_addr = (uint32_t)lut_addr;
    idma2d_desc->fw_next_descriptor_addr = idma2d_desc;
    idma2d_desc->config_flags |= DDMA_DESCRIPTOR_CONFIG_OWNER_BLOCK;
}

void pic_prnudsnu_set_lut_xfer_count( unsigned long byte_count )
{
    idma2d_desc->transfer_len_bytes = byte_count;
    /* set to repeat forever */
    idma2d_regs->line_width = IDMA_2D_LINE_WIDTH_REPEAT_FOREVER_REPLACE_VAL( idma2d_regs->line_width, 1 );

    idma2d_regs->line_width = IDMA_2D_LINE_WIDTH_LINE_WIDTH_REPLACE_VAL( idma2d_regs->line_width, byte_count );
}

void pic_prnudsnu_start_lut_dma( void )
{
//    pic_prnudsnu_dump();
//    pic_prnudsnu_dma_dump();
//    pic_prnudsnu_desc_dump();

    /* davep 08-Jan-2013 ; turn on error interrupt(s) */
    idma2d_regs->int_en = IDMA_2D_INT_EN_WHO_INT_EN_MASK;
    ddma_desc_flush( idma2d_desc );

    /* fire in the hole! */
    idma2d_regs->desc_write = (uint32_t)idma2d_desc->dma_ptr_self;

//    pic_prnudsnu_dump();
//    pic_prnudsnu_dma_dump();
//    pic_prnudsnu_desc_dump();
}

unsigned long pic_prnudsnu_get_lut_xfer_count( void )
{
    return idma2d_desc->transfer_len_bytes;
}

uint32_t pic_prnudsnu_dma_get_block(void)
{
    return     current_block_num;
}

/**
 * \brief  Set which prnudsnu dma block we're talking to
 *  And, if we haven't allocated a descriptor yet, allocate it
 *
 * See comments in pic_prnudsnu_set_block() for details on this function.
 *
 * \author David Poole
 * \date 15-Jan-2013
 */

void pic_prnudsnu_dma_set_block( uint32_t block )
{
        scan_err_t scerr;

#ifdef HAVE_NSENSOR_SUPPORT
    /* as of this writing (Jan-2013), we support a max of two sensors */
    XASSERT(block < scansen_get_num_sensors(), block );
#else
    XASSERT(block==0, block );
#endif
    /* davep 04-Jan-2013 ; use a dynamic descriptor (allocate once then re-use) */
    if( idma2d_desc_array[block]==NULL ) {
        scerr = ddma_descriptor_alloc( &idma2d_desc_array[block] );
        if( scerr!=SCANERR_NONE ) {
            dbg2( "%s unable to alloc prnudsnu descriptor scerr=%d\n", 
                  __FUNCTION__, scerr );
            XASSERT(0,scerr);
            return;
        }
    }

    idma2d_regs = idma2d_regs_array[block];
    idma2d_desc = idma2d_desc_array[block];
    dbg2("%s switched to sensor %d\n",__FUNCTION__,block);
    current_block_num = block;
}

