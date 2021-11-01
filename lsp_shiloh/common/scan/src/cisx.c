/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 *  CISX management code.
 *  davep 19-Nov-2010
 *
 *  Adding dual scanner support (two CISX blocks)
 *  davep 30-Jan-2013
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "regAddrs.h"
#include "interrupt_api.h"
#include "intnums.h"
#include "list.h"
#include "memAPI.h"
#include "cpu_api.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "safetylock.h"
#include "icedma.h"
#include "ddma.h"
#include "cisx.h"
#include "scantools.h"
#include "scanif.h"
#include "scancmdq.h"
#include "scantask.h"
#include "CISX_int_regstructs.h"
#include "CISX_int_regmasks.h"
#include "CISX_IDMA_AXI_regstructs.h"
#include "CISX_IDMA_AXI_regmasks.h"
#include "CISX_ODMA_AXI_regstructs.h"
#include "CISX_ODMA_AXI_regmasks.h"
#include "scands.h"
#include "scanmem.h"
#include "scanimg.h"

/* Define to turn on more verbose debugging */
//#define CISX_DEBUG  

#ifdef CISX_DEBUG
  #define cisx_dbg2 dbg2 
#else
  #define cisx_dbg2(...)
#endif

/* davep 16-Sep-2011 ; delta between a couple asic revs is creating problems.
 * Kcah around the issue.
 */
#ifndef CISX_ODMA_INT_EN_RST_INT_EN_MASK
#define CISX_ODMA_INT_EN_RST_INT_EN_MASK 0
#endif
#ifndef CISX_IDMA_INT_EN_RST_INT_EN_MASK
#define CISX_IDMA_INT_EN_RST_INT_EN_MASK 0
#endif

/* channel numbers; indexes into the global register block arrays */
#define CISX0_ODMA0 0
#define CISX0_ODMA1 1 
#define CISX0_ODMA2 2
#define CISX1_ODMA0 3
#define CISX1_ODMA1 4 
#define CISX1_ODMA2 5

#define CISX0_IDMA0 0
#define CISX1_IDMA0 1

static volatile CISX_INT_REGS_t * const cisx_regs_list[] = {
#ifdef HAVE_NSENSOR_SUPPORT
    (volatile CISX_INT_REGS_t *)(ICE_CISX_CISX0_CISX_INT_BASE),
    (volatile CISX_INT_REGS_t *)(ICE_CISX_CISX1_CISX_INT_BASE),
#else
    (volatile CISX_INT_REGS_t *)(ICE_CISX_CISX_INT_BASE),
#endif
    0 /* end of list */
};

static volatile CISX_ODMA_REGS_t * const odma_registers_list[] = {
#ifdef HAVE_NSENSOR_SUPPORT
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_CISX0_ODMA0_BASE,
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_CISX0_ODMA1_BASE,
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_CISX0_ODMA2_BASE,
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_CISX1_ODMA0_BASE,
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_CISX1_ODMA1_BASE,
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_CISX1_ODMA2_BASE,
#else
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_ODMA0_BASE,
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_ODMA1_BASE,
    (volatile CISX_ODMA_REGS_t *)ICE_CISX_ODMA2_BASE,
#endif
    0 /* end of list */
};

static volatile CISX_IDMA_REGS_t * const idma_registers_list[] = {
#ifdef HAVE_NSENSOR_SUPPORT
    (volatile CISX_IDMA_REGS_t *)(ICE_CISX_CISX0_IDMA0_BASE),
    (volatile CISX_IDMA_REGS_t *)(ICE_CISX_CISX1_IDMA0_BASE),
#else
    (volatile CISX_IDMA_REGS_t *)(ICE_CISX_IDMA_BASE),
#endif
    0 /* end of list */
};

#define MAX_CISX_DMA_DESCRIPTORS 2
//#define MAX_CISX_DMA_DESCRIPTORS 8
//#define MAX_CISX_DMA_DESCRIPTORS 32

struct cisx_mm {
    /** icebufs ready and waiting to be loaded into DMA controller */
    ATLISTENTRY readies;

    /** icebufs currently loaded into DMA controller */
    ATLISTENTRY running;

    int num_readies;
    int num_running;

    /* davep 13-Mar-2013 ; check for memory leaks, double pointers, etc. */
    int num_descriptors_allocated;

    /* davep 03-Apr-2013 ; adding run-time enabling of dual scan */
    uint32_t sensor_bitmask;

    /* davep 22-Apr-2013 ; attempt to allocate from fast memory (e.g. SRAM). If
     * the allocation fails, we'll alloc from DRAM instead
     */
    void *cisx_buf;
    uint32_t cisx_buf_num_bytes;
    bool cisx_buf_is_fast_memory;
};

#define MAX_CISX_CHANNEL_NAME 31

struct cisx_channel {
    /* the channel number this data structure assigned to */
    uint8_t channel;

    char name[MAX_CISX_CHANNEL_NAME];

    /* 27-Apr-05 ; needed a way to unambiguously tell if a channel is in-use */
    bool is_open;

    uint32_t bytes_per_row;

    struct ddma_descriptor_list desc_list;
};

static struct cisx_channel cisx_odma_channel[CISX_ODMA_NUM_CHANNELS];
static struct cisx_channel cisx_idma_channel[CISX_IDMA_NUM_CHANNELS];

static struct cisx_mm cisx_mm;

/* davep 29-Jan-2013 ; adding dual scan */
static int num_odma_channels;
static uint8_t odma_channels[CISX_ODMA_NUM_CHANNELS];
static int num_idma_channels;
static uint8_t idma_channels[CISX_IDMA_NUM_CHANNELS];

struct cisx_block {
    char name[8];
    uint8_t odma_channels[3];
    uint8_t idma_channel;
};

#ifdef __KERNEL__
static irqreturn_t cisx_interrupt( int irq, void *dev_id ) 
#else
static void cisx_interrupt( uint32_t param )
#endif
{
    int i;
    uint32_t ipend;
    uint32_t num32;
    volatile CISX_INT_REGS_t * cisxregs;

    /* 
     * BIT FAT NOTE: THIS IS AN INTERRUPT HANDLER.
     *
     */

    cisx_dump();

    /* As of this writing (05-Apr-2011) the only interrupts I'm using are those
     * indicating an error. CISX runs autonomously once started so no
     * interrupts are necessary for normal behavior.
     *
     * If you are here, something has gone wrong. Horribly, horribly wrong.
     */

    ipend = 0;

    /* find first non-zero CISX int_pend */
    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];
        num32 = cisxregs->IntPend;

        /* davep 11-Feb-2013 ; ack everything so we don't get stuck in here */
        cisxregs->IntAck = num32;

        if( num32 ) {
            ipend = num32;
            dbg1( "%s cisx=%d ipend=%x\n", __FUNCTION__, i, ipend );
        }
    }

    /* davep 08-Jul-2011 ; stomp on the scan block and cisx immediately so we
     * can get an accurate snapshot of where we went pear shaped
     */
    scif_clock(0);
    scif_control(0,0);
    cisx_odma_channel_disable(0);
    cisx_odma_channel_disable(1);
    cisx_odma_channel_disable(2);
    cisx_idma_channel_disable(0);
    scan_cmdq_emergency_halt();

    dbg1( "ERROR! %s ipend=%#x\n", __FUNCTION__, ipend );

//    scif_dump();
//    icetest_dump();
//    icetest_ddma_dump();
//    pic_dump();
//    cisx_ddma_data_peek();

    XASSERT( 0, ipend );

#ifdef __KERNEL__
    /* CISX interrupt only enabled for errors */
    BUG();
    return IRQ_HANDLED;
#endif
}

scan_err_t cisx_duplex_setup( void )
{
    scan_err_t scerr = SCANERR_NOT_IMPLEMENTED;
#ifdef HAVE_NSENSOR_SUPPORT
    volatile CISX_INT_REGS_t * cisxregs;

    /* davep 29-Jan-2013 ; tinkering with dual scan
     *  CISX0 cisx_tag = tag value from Scan for CISX0
     *  CISX1 cisx_tag = tag value from Scan for CISX1
     *
     *  numsensors==1 in CISX0
     *
     *  CISX0 force_odd=0
     *  CISX1 force_odd=1
     *
     */

    cisxregs = cisx_regs_list[0];
    cisxregs->CisxCfg = CISX_INT_CISXCFG_FORCE_ODD_REPLACE_VAL( cisxregs->CisxCfg, 0 );
    cisxregs->CisxCfg = CISX_INT_CISXCFG_CISX_TAG_REPLACE_VAL( cisxregs->CisxCfg, 0 );
    cisxregs->CisxCfg = CISX_INT_CISXCFG_NUMSENSORS_REPLACE_VAL( cisxregs->CisxCfg, 1 );

    cisxregs = cisx_regs_list[1];
    cisxregs->CisxCfg = CISX_INT_CISXCFG_FORCE_ODD_REPLACE_VAL( cisxregs->CisxCfg, 1 );
    cisxregs->CisxCfg = CISX_INT_CISXCFG_CISX_TAG_REPLACE_VAL( cisxregs->CisxCfg, 1 );
    cisxregs->CisxCfg = CISX_INT_CISXCFG_NUMSENSORS_REPLACE_VAL( cisxregs->CisxCfg, 1 );

    scerr = SCANERR_NONE;
#endif
    return scerr;
}

uint32_t cisx_interrupt_disable( void )
{
    int i;
    uint32_t was_enabled;
    volatile CISX_INT_REGS_t * cisxregs;

    was_enabled = 0;

    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];

        was_enabled |= cisxregs->IntEn != 0;

        /* turn all interrupts off */
        cisxregs->IntEn = 0;
    }

    /* return non-zero if anything was enabled */
    return was_enabled != 0;
}

void cisx_interrupt_enable( void )
{
    int i;
    volatile CISX_INT_REGS_t * cisxregs;

    /* davep 08-Jul-2011 ; turn 'em all on; gotta catch 'em all!
     * The non-error DMA interrupts (e.g., fin_int_en) are disabled at each DMA
     * channel. Firmware doesn't need to handle CISX DMA interrupts for CISX to
     * function.
     */
    for( i=0 ; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];

        cisxregs->IntEn = ~0;
    }
}

static void cisx_desc_list_sanity( struct ddma_descriptor_list *desc_list )
{
    int sanity_count; 
    struct ddma_descriptor *head;
    struct ddma_descriptor *curr;

//    dbg2( "%s head=%p num=%d\n", __FUNCTION__, head, num_descriptors ));

    head = desc_list->list;

    ASSERT( head );
    ASSERT( desc_list->num_descriptors );

    curr = head;
    sanity_count = 0;
    do {
        /* CISX never adds buffers to the list so every descriptor must have a
         * pointer attached
         */
        XASSERT( curr->src_addr, (uint32_t )curr );
        XASSERT( curr->transfer_len_bytes, (uint32_t )curr );

        /* all buffers must be identical in size */
        XASSERT( curr->transfer_len_bytes==head->transfer_len_bytes, curr->transfer_len_bytes );

        curr = (struct ddma_descriptor *)curr->fw_next_descriptor_addr;

        /* davep 07-Jul-2012 ; make sure we have our convenience pointers
         * correctly set
         */
        XASSERT( curr->fw_src_addr, (uint32_t)curr );

        /* avoid infinite loops on corrupted lists */
        sanity_count += 1;
        XASSERT( sanity_count<=desc_list->num_descriptors, sanity_count );

    } while( curr != head );

    XASSERT( sanity_count==desc_list->num_descriptors, sanity_count );
}


/*
 *  CISX ODMA Functions
 *
 */

static void odma_channel_dump( volatile CISX_ODMA_REGS_t *oregs )
{
    dbg2( "     cfg=0x%08x    status=0x%08x line_size=0x%08x\n", 
                oregs->cfg, oregs->status, oregs->line_size );
    dbg2( "  int_en=0x%08x  int_pend=0x%08x desc_read=0x%08x\n", 
                oregs->int_en, oregs->int_pend, oregs->desc_read );
    dbg2( "xfer_len=0x%08x  xferaddr=0x%08x  xferburs=0x%08x\n", 
                oregs->xfer_length, oregs->xfer_addr, oregs->xfer_burst );
}

static volatile CISX_ODMA_REGS_t * get_odma_regs( uint8_t channel )
{
    XASSERT( channel < CISX_ODMA_NUM_CHANNELS, channel ); 
    return odma_registers_list[channel];
}

static volatile CISX_IDMA_REGS_t * get_idma_regs( uint8_t channel )
{
    XASSERT( channel < CISX_IDMA_NUM_CHANNELS, channel ); 
    return idma_registers_list[channel];
}

void cisx_odma_channel_disable( uint8_t channel )
{
    volatile CISX_ODMA_REGS_t *oregs;

    oregs = get_odma_regs( channel );

    oregs->cfg &= ~CISX_ODMA_CFG_ENABLE_MASK;
}

void cisx_odma_channel_enable( uint8_t channel )
{
    volatile CISX_ODMA_REGS_t *oregs;
    uint32_t num32;

    oregs = get_odma_regs( channel );

    oregs->cfg |= CISX_ODMA_CFG_ENABLE_MASK;

    num32 = oregs->cfg;

    /* set to 16-bits; XXX magic number! */
    num32 = CISX_ODMA_CFG_IN_WIDTH_REPLACE_VAL( num32, 4 );

//    /* set to 32-bits; XXX magic number! */
//    num32 = CISX_ODMA_CFG_IN_WIDTH_REPLACE_VAL( num32, 5 );

    /* set burst to ? words; XXX magic number! */
    num32 = CISX_ODMA_CFG_BURST_LEN_REPLACE_VAL( num32, 0 );
//    num32 = CISX_ODMA_CFG_BURST_LEN_REPLACE_VAL( num32, 1 );

    /* davep 25-May-2011 ; copy/paste from asic team's unit test */
    num32 = CISX_ODMA_CFG_SERPENTINE_REPLACE_VAL(num32,0);
    num32 = CISX_ODMA_CFG_LINE_REV_REPLACE_VAL(num32,0);
    num32 = CISX_ODMA_CFG_TRAN_REV_REPLACE_VAL(num32,0);
    num32 = CISX_ODMA_CFG_UPPER_HALF_REPLACE_VAL(num32,0);
    num32 = CISX_ODMA_CFG_MSB_IN_REPLACE_VAL(num32,0);

    oregs->cfg = num32;

    /* davep 22-Jul-2011 ; catch everything except reset (anything else is an
     * error) ; hopefully will future proof us as well
     */
    oregs->int_en = ~(CISX_ODMA_INT_EN_RST_INT_EN_MASK);
}

void cisx_odma_channel_reset( uint8_t channel )
{
    volatile CISX_ODMA_REGS_t *oregs;

    oregs = get_odma_regs( channel );

    oregs->reset |= CISX_ODMA_RESET_SOFT_RESET_MASK;
    cpu_spin_delay( 10 );  /* wait a little while to allow hw to reset */
    oregs->reset &= ~CISX_ODMA_RESET_SOFT_RESET_MASK;

    oregs->int_en = 0;

    /* ack any pending interrupts */
    oregs->int_ack = ~0;

    /* write back to documented power-on defaults */
    oregs->cfg = CISX_ODMA_CFG_UPPER_HALF_MASK 
               | CISX_ODMA_CFG_MSB_IN_MASK;
    oregs->line_size = 0;
}

static void cisx_odma_channel_launch( struct cisx_channel *cch )
{
    int i;
    volatile CISX_ODMA_REGS_t *oregs;
    struct ddma_descriptor_list *desc_list;

    XASSERT( cch->is_open, cch->channel );

    oregs = get_odma_regs( cch->channel );

    /* use a shortcut to keep expression size down */
    desc_list = &cch->desc_list;
     
    /* once we start CISX, this list never changes. So flush *everything* */
    for( i=0 ; i<desc_list->num_descriptors ; i++ ) {
        ddma_desc_flush( &desc_list->list[i] );
    }

    /* fire in the hole! */
    oregs->line_size = cch->bytes_per_row;
    oregs->desc_write = (uint32_t)desc_list->list[0].dma_ptr_self;
}

/*
 * CISX IDMA Functions
 *
 */

static void idma_channel_dump( volatile CISX_IDMA_REGS_t *iregs )
{
    dbg2( "     cfg=0x%08x   status=0x%08x line_width=0x%08x\n", 
                iregs->cfg, iregs->status, iregs->line_width );
    dbg2( "  int_en=0x%08x   int_st=0x%08x   descread=0x%08x\n", 
                iregs->int_en, iregs->int_st, iregs->desc_read );
    dbg2( "xfer_len=0x%08x xferaddr=0x%08x ctrl_word=0x%08x\n", 
                iregs->xfer_length, iregs->xfer_addr, iregs->ctrl_word );
}

void cisx_idma_channel_disable( uint8_t channel )
{
    volatile CISX_IDMA_REGS_t *idma_regs;

    idma_regs = get_idma_regs( channel );
    idma_regs->cfg &= ~CISX_IDMA_CFG_ENABLE_MASK; 
    idma_regs->int_en = 0;
}


void cisx_idma_channel_enable( uint8_t channel )
{
    volatile CISX_IDMA_REGS_t *idma_regs;
    uint32_t num32;

    idma_regs = get_idma_regs( channel );
    idma_regs->cfg |= CISX_IDMA_CFG_ENABLE_MASK; 

    num32 = idma_regs->cfg;

    /* set to ?-bits; XXX magic number! */
    num32 = CISX_IDMA_CFG_OUT_WIDTH_REPLACE_VAL( num32, 4 );
//    num32 = CISX_IDMA_CFG_OUT_WIDTH_REPLACE_VAL( num32, 5 );

    /* davep 25-May-2011 ; copy/paste from asic team's unit test */
    num32 = CISX_IDMA_CFG_REPLICATE_REPLACE_VAL( num32, 0 );
    num32 = CISX_IDMA_CFG_BURST_LEN_REPLACE_VAL( num32, 0 );

    idma_regs->cfg = num32;

    /* davep 22-Jul-2011 ; turn on everything except reset (should enable
     * all the error conditions and trap on anything weird we don't yet know
     * about)
     */
    idma_regs->int_en = ~(CISX_IDMA_INT_EN_RST_INT_EN_MASK);
}

void cisx_idma_channel_reset( uint8_t channel )
{
    volatile CISX_IDMA_REGS_t *idma_regs;

    idma_regs = get_idma_regs( channel );

    idma_regs->int_en = 0;

    /* ack all pending interrupts */
    idma_regs->int_cl = ~0;

    /* no reset bit so just write back to documented power-on defaults.
     * Note the CISX Rev 1.0 docs says cfg.replicate==1 at power-on but the
     * notes then say, "Not used. Set to 0." so let's just set it all to zero.
     */
    idma_regs->cfg = 0;

    idma_regs->line_width = 0;

    /* sanity check on my reset code and on the register access locations */
    XASSERT( idma_regs->status ==
            (CISX_IDMA_STATUS_SOFTRESET_MASK | CISX_IDMA_STATUS_EMPTY_CBUF_MASK | 
             CISX_IDMA_STATUS_EMPTY_DBUF_MASK |  CISX_IDMA_STATUS_PACKER_EMPTY_MASK),
            idma_regs->status );

}

static void cisx_idma_channel_launch( struct cisx_channel *cch )
{
    int i;
    volatile CISX_IDMA_REGS_t *idma_regs;
    struct ddma_descriptor_list *desc_list;

    XASSERT( cch->is_open, cch->channel );

    /* use a shortcut to keep expression size down */
    desc_list = &cch->desc_list;

    idma_regs = get_idma_regs( cch->channel );

    /* once we start CISX, this list never changes. So flush *everything* */
    for( i=0 ; i<desc_list->num_descriptors ; i++ ) {
        ddma_desc_flush( &desc_list->list[i] );
    }

    /* fire in the hole! */
    idma_regs->line_width = cch->bytes_per_row;
    idma_regs->desc_write = (uint32_t)desc_list->list[0].dma_ptr_self;
}

/*
 *  CISX DMA (both O & I) Function
 *  
 */

static void cisx_channel_sanity( struct cisx_channel *cch )
{
    struct ddma_descriptor_list *desc_list;

    cisx_dbg2( "%s %s %d\n", __FUNCTION__, cch->name, cch->channel );

    /* use a shortcut to reduce ->'s */
    desc_list = &cch->desc_list;

    if( !cch->is_open ) {
        XASSERT( desc_list->num_descriptors==0, desc_list->num_descriptors );
        XASSERT( desc_list->list==NULL, (uint32_t)desc_list->list );
        return;
    }

    if( desc_list->num_descriptors ) {
        cisx_desc_list_sanity( desc_list );
    }
    else {
        XASSERT( desc_list==NULL, (uint32_t)desc_list );
    }
}

static void cisx_channel_init( struct cisx_channel *cch, const char *name, uint8_t channel )
{
    memset( cch, 0, sizeof(struct cisx_channel) );

    strncpy( cch->name, name, MAX_CISX_CHANNEL_NAME );
    cch->channel = channel;
}

static scan_err_t cisx_channel_open( struct cisx_channel *cch, const char *name, 
                                    uint8_t channel, uint32_t bytes_per_row )
{
    scan_err_t scerr;
    int i;
    struct ddma_descriptor *desc;

    cisx_dbg2( "%s name=\"%s\" ch=%d bpr=%d\n", __FUNCTION__, name, channel, bytes_per_row );

    /* beware of memory leaks */
    XASSERT( cch->desc_list.list==NULL, (uint32_t)cch->desc_list.list );
    XASSERT( cch->desc_list.num_descriptors==0, cch->desc_list.num_descriptors );

    cisx_channel_init( cch, name, channel );

    cch->bytes_per_row = bytes_per_row;

    /* allocate a block of descriptors */
    scerr = ddma_descriptor_list_alloc( &cch->desc_list, name, MAX_CISX_DMA_DESCRIPTORS );
    if( scerr != SCANERR_NONE ) {
        /* ddma logs error */
        return scerr;
    }

    /* clear all the flags (we never stop, want no interrupts */
    for( i=0 ; i<cch->desc_list.num_descriptors ; i++ ) {
        desc = &cch->desc_list.list[i];
        desc->config_flags = 0;
        ddma_desc_flush( desc );
    }

    /* open for business! */
    cch->is_open = true;

    return SCANERR_NONE;
}

static void cisx_channel_close( struct cisx_channel *cch )
{
    cisx_dbg2( "%s %d\n", __FUNCTION__, cch->channel );

    XASSERT( cch->is_open, cch->channel );

    ddma_descriptor_list_free( &cch->desc_list );
    cch->is_open = false;
}

static void cisx_channel_load( struct cisx_channel *cch, 
                                uint8_t *cpu_addr, dma_addr_t dma_addr,
                               uint32_t num_rows, uint32_t bytes_per_row )
{
    int i;
    struct ddma_descriptor_list *desc_list;
    struct ddma_descriptor *desc;

//    dbg2( "%s ch=%d name=\"%s\" dest=%x nr=%d bpr=%d\n", __FUNCTION__, 
//            cch->channel, cch->name, dma_dest, num_rows, bytes_per_row  );

    /* stupid human checks */
    XASSERT( bytes_per_row, cch->channel );
    XASSERT( num_rows, cch->channel );
    XASSERT( cpu_addr, cch->channel );
    XASSERT( dma_addr, cch->channel );

    desc_list = &cch->desc_list;

    XASSERT( desc_list->num_descriptors, cch->channel );
    XASSERT( desc_list->list != NULL, cch->channel );

    /* CISX is a circular list. All memory allocated before we start. We never
     * add new buffers once we're running. 
     *
     * I want simple. Don't bother with head/tail pointers because CISX is
     * pretty much autonomous once we're running. Fire/forget. 
     *
     * Thus we're doing a stupid linear search to add buffers to our desc_list.
     */
    for( i=0 ; i<desc_list->num_descriptors ; i++ ) {
        if( desc_list->list[i].src_addr==0 ) {
            /* found an empty slot! claim it */
            desc = &desc_list->list[i];
            desc->fw_src_addr = (void *)cpu_addr;

            dbg2( "%s cpu_addr=%x dma_addr=%x len=%d\n", __FUNCTION__, 
                    cpu_addr, dma_addr, num_rows*bytes_per_row );
            desc->src_addr = (uint32_t)dma_addr;
            desc->transfer_len_bytes = num_rows * bytes_per_row;
            break;
        }
    }

    /* if this assert fails, trying to add more buffers than we have
     * descriptors allocated. Shouldn't happen because of the way we're
     * allocating buffers.
     */
    XASSERT( i!=desc_list->num_descriptors, i );
}

/* 
 * CISX Control Functions
 *
 */

void cisx_dump( void )
{
    int i;
    volatile CISX_INT_REGS_t * cisxregs;

    dbg2( "%s\n", __FUNCTION__ );

    for( i=0 ; cisx_regs_list[i] ; i++ ) {
        cisxregs = cisx_regs_list[i];

        dbg2( "     cfg=0x%08x     stat=0x%08x    inten=0x%08x intpend=0x%08x\n", 
                    cisxregs->CisxCfg, cisxregs->CisxStat, cisxregs->IntEn, cisxregs->IntPend );
        dbg2( "chan0pix=0x%08x chan1pix=0x%08x chan2pix=0x%08x \n", 
                    cisxregs->Chan0Pix, cisxregs->Chan1Pix, cisxregs->Chan2Pix );
    }

    cisx_dma_dump();
}

void cisx_set_bypass( bool bypass )
{
    int i;
    volatile CISX_INT_REGS_t * cisxregs;

    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];
        if( bypass ) {
            cisxregs->CisxCfg = CISX_INT_CISXCFG_BYPASS_REPLACE_VAL( cisxregs->CisxCfg, 1 );
        }
        else {
            cisxregs->CisxCfg = CISX_INT_CISXCFG_BYPASS_REPLACE_VAL( cisxregs->CisxCfg, 0 );
        }

        /* davep 30-Jan-2013 ; only the first sensor is enabled if we're doing single sided scan */
        if (!bypass && SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask)) {
            break;
        }
    }
}

void cisx_set_cmode( scan_cmode_t cmode )
{
#ifdef CISX_INT_CISXCFG_COLORSCAN_REPLACE_VAL
    int i;
    uint32_t num32;
    volatile CISX_INT_REGS_t * cisxregs;

    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];
        num32 = cisxregs->CisxCfg;

        if( cmode==SCAN_CMODE_MONO ) {
            num32 = CISX_INT_CISXCFG_COLORSCAN_REPLACE_VAL( num32, 0 );
        }
        else { 
            num32 = CISX_INT_CISXCFG_COLORSCAN_REPLACE_VAL( num32, 1 );
        }

        cisxregs->CisxCfg = num32;

    }
#endif
}

void cisx_set_3channel_order( uint8_t cisx_3chan_order )
{
    int i;
    volatile CISX_INT_REGS_t * cisxregs;

    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];
        cisxregs->CisxCfg = CISX_INT_CISXCFG_CFG_3CH_ORDER_REPLACE_VAL( cisxregs->CisxCfg, cisx_3chan_order );
    }
}

void cisx_set_pixels( uint8_t odma_channel, uint32_t num_dummy_pixels, uint32_t num_data_pixels, int append_flag )
{
    uint32_t num32;
    volatile CISX_INT_REGS_t * cisxregs;

    dbg2( "%s ch=%d dummy=%d data=%d append=%d\n", __FUNCTION__, 
            odma_channel, num_dummy_pixels, num_data_pixels, append_flag );

    num32 = (CISX_INT_CHAN0PIX_CHAN0DUM_MASK & (num_dummy_pixels<<CISX_INT_CHAN0PIX_CHAN0DUM_SHIFT))
          | (CISX_INT_CHAN0PIX_CHAN0DAT_MASK & (num_data_pixels<<CISX_INT_CHAN0PIX_CHAN0DAT_SHIFT))
          ;

    switch( odma_channel ) {
        case CISX0_ODMA0 :
        case CISX0_ODMA1 : 
        case CISX0_ODMA2 : 
            cisxregs = cisx_regs_list[0];
            break;
        case CISX1_ODMA0 :
        case CISX1_ODMA1 : 
        case CISX1_ODMA2 : 
            cisxregs = cisx_regs_list[1];
            break;

        default : 
            XASSERT( 0, odma_channel );
            return;
    }

    switch( odma_channel ) {
        case CISX0_ODMA0 :
        case CISX1_ODMA0 :
            cisxregs->Chan0Pix = num32;
            if(append_flag) {
                cisxregs->CisxCfg |= CISX_INT_CISXCFG_APPCHAN0_MASK;
            }
            break;

        case CISX0_ODMA1 : 
        case CISX1_ODMA1 : 
            cisxregs->Chan1Pix = num32;
            if(append_flag) {
                cisxregs->CisxCfg |= CISX_INT_CISXCFG_APPCHAN1_MASK;
            }
            break;

        case CISX0_ODMA2 : 
        case CISX1_ODMA2 : 
            cisxregs->Chan2Pix = num32;
            break;

        default : 
            XASSERT( 0, odma_channel );
            break;
    }
}

scan_err_t cisx_open( scan_cmode_t cmode, 
                      const struct cisx_sensor_conf *cisxsensor,
                       uint32_t sensor_bitmask)
{
    struct cisx_mm *mm;
    scan_err_t scerr;
    int i, channel_idx, num_descriptors;
    uint32_t bytes_per_row, bytes_per_row_padded, bytes_per_segment, num_active_pixels;
    struct cisx_channel *cch;
    struct ice_dma_buffer *icebuf;
    char name[DDMA_NAME_LEN+1];
    uint8_t *ptr;

    cisx_dbg2( "%s cmode=%d num_sensors=%d\n", __FUNCTION__, cmode, num_sensors );

    mm = &cisx_mm;

    /* davep 03-Apr-2013 ; adding run-time enabling of dual scan */
    mm->sensor_bitmask = sensor_bitmask;

    /* how many pixels are we going to get out of CISX? */
    num_active_pixels = 0;
    for( i=0 ; i<cisxsensor->num_segments ; i++ ) {
        num_active_pixels += cisxsensor->segments[i].num_data;
    }
        
    /* the sizeof(uint16_t) is because cisx works with 16-bit pixels */
    bytes_per_row = num_active_pixels * sizeof(uint16_t);

    /* only tested 3 segment sensors */
    XASSERT( cisxsensor->num_segments==3, cisxsensor->num_segments );

    /* davep 29-Jan-2013 ; adding dual scan */
    if( SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask) ) {
        num_odma_channels = 3;
        odma_channels[0] = CISX0_ODMA0;
        odma_channels[1] = CISX0_ODMA1;
        odma_channels[2] = CISX0_ODMA2;
    }
    else {
        num_odma_channels = 6;
        odma_channels[0] = CISX0_ODMA0;
        odma_channels[1] = CISX0_ODMA1;
        odma_channels[2] = CISX0_ODMA2;
        odma_channels[3] = CISX1_ODMA0;
        odma_channels[4] = CISX1_ODMA1;
        odma_channels[5] = CISX1_ODMA2;
    }

    /* davep 25-Mar-2013 ; adding cbiout_pace value to cisx config. Older code
     * might not have this value set so use zero as an 'uninitialized' value.
     * This will probably case problems later when we *DO* want to use zero.
     */
    if( cisxsensor->cbiout_pace ) {
        cisx_set_cbiout_pace( cisxsensor->cbiout_pace );
    }

    channel_idx = 0;
    while( channel_idx < num_odma_channels ) {
        for( i=0 ; i<cisxsensor->num_segments ; i++ ) {
            cch = &cisx_odma_channel[ odma_channels[channel_idx] ];

            cisx_set_pixels( odma_channels[channel_idx], 
                             cisxsensor->segments[i].num_dummy, 
                             cisxsensor->segments[i].num_data,  
                             cisxsensor->segments[i].append_flag  );

            /* the sizeof(uint16_t) is because cisx works with 16-bit pixels */
            bytes_per_segment = cisxsensor->segments[i].num_data * sizeof(uint16_t);

            strncpy( name, "cisxodma ", DDMA_NAME_LEN );
            name[8] = '0' + cch->channel;
            scerr = cisx_channel_open( cch, name, odma_channels[channel_idx], bytes_per_segment );
            if( scerr != SCANERR_NONE ) {
                cisx_close();
                return scerr;
            }

            cisx_odma_channel_enable( cch->channel );
            channel_idx += 1;
        }
    }

    /* davep 29-Jan-2013 ; adding dual scan */
    if( SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask) ) {
        num_idma_channels = 1;
        idma_channels[0] = CISX0_IDMA0;
    }
    else {
        num_idma_channels = 2;
        idma_channels[0] = CISX0_IDMA0;
        idma_channels[1] = CISX1_IDMA0;
    }

    for( i=0 ; i<num_idma_channels ; i++ ) {
        cch = &cisx_idma_channel[ idma_channels[i] ];
        strncpy( name, "cisxidma ", DDMA_NAME_LEN );
        name[8] = '0' + cch->channel;
        scerr = cisx_channel_open( cch, name, idma_channels[i], bytes_per_row );
        if( scerr != SCANERR_NONE ) {
            cisx_close();
            return scerr;
        }
        cisx_idma_channel_enable( cch->channel );
    }

    /* 
     * At this point, we'll start allocating memory for the CISX buffers.
     */

    /* be paranoid about memory leaks */
    XASSERT( mm->num_readies==0, mm->num_readies );
    XASSERT( mm->num_running==0, mm->num_running );
    ASSERT( ATIsListEmpty( &mm->readies ) );
    ASSERT( ATIsListEmpty( &mm->running ) );

    /* davep 21-Jun-2011 ; pad up to dma */
    bytes_per_row_padded = ICE_DMA_ALIGN_ME( bytes_per_row );

    num_descriptors = MAX_CISX_DMA_DESCRIPTORS;
    /* if we're doing dual scan, we have two CISX mouths to feed */
    if( !SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask) ) {
        num_descriptors *= 2;
    }
    mm->num_descriptors_allocated = 0;

    /* davep 22-Apr-2013 ; allocate an aggregate buffer (instead of multiple
     * smaller buffers). Want to get fast memory (e.g., SRAM) if possible and
     * the fastmem interface is simpler if we use one single buffer.
     */
    mm->cisx_buf_num_bytes = bytes_per_row_padded * num_descriptors;
    mm->cisx_buf_is_fast_memory = false;

    /* beware memory leaks */
    XASSERT( mm->cisx_buf==NULL, (uint32_t)mm->cisx_buf );

    mm->cisx_buf = scanmem_get_fast_memory(SCANMEM_TAG_CISX,mm->cisx_buf_num_bytes, 0);
    if( mm->cisx_buf!=NULL ) {
        /* we got fast memory! hurray! */
        dbg2( "%s using numbytes=%d of fast memory\n", __FUNCTION__, mm->cisx_buf_num_bytes );
        mm->cisx_buf_is_fast_memory = true;
    }
    else {
        mm->cisx_buf = ice_dma_alloc_buffer(mm->cisx_buf_num_bytes);
        if( mm->cisx_buf==NULL ) {
            dbg2( "%s unable to allocate numbytes=%d\n", __FUNCTION__, mm->cisx_buf_num_bytes );
            mm->cisx_buf_num_bytes = 0;
            return SCANERR_OUT_OF_MEMORY;
        }
        dbg2( "%s using numbytes=%d of regular memory\n", __FUNCTION__, mm->cisx_buf_num_bytes );
    }
    ptr = mm->cisx_buf;

    /* one icebuf per odma/idma descriptor */
    for( i=0 ; i<num_descriptors ; i++ ) {
        icebuf = icebuf_new_blank( ICEBUF_TAG_CISX );
        if( icebuf==NULL ) {
            return SCANERR_OUT_OF_MEMORY;
        }
        icebuf->data = ptr;
        /* Don't use icebuf->dma_handle or dma_map_single here. We'll do all
         * that in the launch function below.
         */
        icebuf->num_rows = 1;
        icebuf->max_rows = 1;
        icebuf->bytes_per_row = bytes_per_row_padded;
        icebuf->datalen = icebuf->max_rows * icebuf->bytes_per_row;
        ptr += icebuf->datalen;

        /* put it on the readies list */
        ATInsertTailList( &mm->readies, &icebuf->listnode );
        mm->num_readies++;
        mm->num_descriptors_allocated++;
    }

    cisx_set_cmode( cmode );

    if( !SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask) ) {
        scerr = cisx_duplex_setup();
        XASSERT( scerr==SCANERR_NONE, scerr );
    }

    cisx_set_3channel_order( cisxsensor->chan_order );

    return SCANERR_NONE;
}

static scan_err_t cisx_launch_single( struct ice_dma_buffer *icebuf, 
                                      struct cisx_block *block )
{
    int i;
    uint8_t *cpu_addr[3];
    dma_addr_t dma_addr[3];
    struct cisx_channel *cch;

//    dbg2( "%s name=%s\n", __FUNCTION__, block->name );

    /* XXX note I've hardwired to three segment sensor */

    cpu_addr[0] = icebuf->data;
    cpu_addr[1] = cpu_addr[0] + cisx_odma_channel[0].bytes_per_row;
    cpu_addr[2] = cpu_addr[1] + cisx_odma_channel[1].bytes_per_row;

#ifdef __KERNEL__
    for( i=0 ; i<3 ; i++ ) {
        /* davep 23-Apr-2013 ; if we're using memory not CPU dcache mappable
         * (e.g., sram), don't dma map it.
         */
        if( cisx_mm.cisx_buf_is_fast_memory ) {
            dma_addr[i] = (dma_addr_t)cpu_addr[i];
        }
        else {
            dma_addr[i] = dma_map_single( NULL, (void *)cpu_addr[i],
                                          icebuf->num_rows * icebuf->bytes_per_row, 
                                          DMA_BIDIRECTIONAL );
            XASSERT( dma_addr[i], (uint32_t)cpu_addr[i] );
        }
    }
#else 
    for( i=0 ; i<3 ; i++ ) {
        dma_addr[i] = (dma_addr_t)cpu_addr[i];
    }
#endif

//    dbg2( "%s %p %x %x %x\n", __FUNCTION__, icebuf->data, cpu_addr[0], cpu_addr[1], cpu_addr[2] );

    for( i=0 ; i<3 ; i++ ) {
        cch = &cisx_odma_channel[ block->odma_channels[i] ];
        cisx_channel_load( cch, cpu_addr[i], dma_addr[i], icebuf->num_rows, cch->bytes_per_row );
    }

    cch = &cisx_idma_channel[ block->idma_channel ];
    cisx_channel_load( cch, cpu_addr[0], dma_addr[0], icebuf->num_rows, cch->bytes_per_row );

    return SCANERR_NONE;
}

void cisx_launch( void )
{
    scan_err_t scerr;
    struct cisx_mm *mm;
    int i;
    struct ice_dma_buffer *icebuf;
    int num_cisx, cisx_block_idx;
    struct cisx_block cisx_block_list[2];

    cisx_dbg2( "%s\n", __FUNCTION__ );

    mm = &cisx_mm;

    memset( cisx_block_list, 0, sizeof(cisx_block_list) );
    num_cisx = 1;
    strcpy( cisx_block_list[0].name, "CISX0" );
    for( i=0 ; i<3 ; i++ ) {
        cisx_block_list[0].odma_channels[i] = odma_channels[i];
    }
    cisx_block_list[0].idma_channel = idma_channels[0];

    if( !SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask) ) {
        XASSERT( num_odma_channels==6, num_odma_channels );
        XASSERT( num_idma_channels==2, num_idma_channels );
        num_cisx++;
        strcpy( cisx_block_list[1].name, "CISX1" );
        for( i=0 ; i<3 ; i++ ) {
            cisx_block_list[1].odma_channels[i] = odma_channels[i+3];
        }
        cisx_block_list[1].idma_channel = idma_channels[1];
    }

    cisx_set_bypass( false );

    /* pull everything from the ready list, push into hardware, add to
     * descriptor DMA 
     */
    cisx_block_idx = 0;
    while( 1 ) {
        icebuf = icebuf_pop( &mm->readies );
        if( icebuf==NULL ) {
            break;
        }
        icebuf_sanity( icebuf );

        mm->num_readies--;
        XASSERT( mm->num_readies>=0, mm->num_readies );

        scerr = cisx_launch_single( icebuf, &cisx_block_list[cisx_block_idx] );
        XASSERT( scerr==SCANERR_NONE, scerr ); 

        /* ping pong back and forth between the CISX blocks */
        cisx_block_idx = (cisx_block_idx+1) % num_cisx;

        /* attach to the end of our running queue */
        ATInsertTailList( &mm->running, &icebuf->listnode );
        mm->num_running++;
    }

    for( i=0 ; i<num_odma_channels ; i++ ) {
        cisx_odma_channel_launch( &cisx_odma_channel[ odma_channels[i] ] );
    }
    for( i=0 ; i<num_idma_channels ; i++ ) {
        cisx_idma_channel_launch( &cisx_idma_channel[ idma_channels[i] ] );
    }

    cisx_interrupt_enable();

//    /* davep 29-Jun-2011 ; XXX temp debug */
//    cisx_dump();
//    cisx_ddma_dump();
}

void cisx_sanity( void )
{
    struct cisx_mm *mm;
    int i;
    int cnt;

    mm = &cisx_mm;

    for( i=0 ; i<CISX_ODMA_NUM_CHANNELS ; i++ ) {
        cisx_channel_sanity( &cisx_odma_channel[i] );
    }
    for( i=0 ; i<CISX_IDMA_NUM_CHANNELS ; i++ ) {
        cisx_channel_sanity( &cisx_idma_channel[i] );
    }

    dbg2( "%s running=%d readies=%d\n", __FUNCTION__, mm->num_running,
            mm->num_readies );

    cnt = mm->num_running + mm->num_readies;
    XASSERT( cnt==mm->num_descriptors_allocated, cnt );

    cnt = count_list( &mm->readies );
    XASSERT( cnt==mm->num_readies, cnt );

    cnt = count_list( &mm->running );
    XASSERT( cnt==mm->num_running, cnt );
}

void cisx_close( void )
{
    struct cisx_mm *mm;
    int i;
    struct ice_dma_buffer *icebuf;

    cisx_dbg2( "%s\n", __FUNCTION__ );

    mm = &cisx_mm;

    XASSERT( mm->num_readies==0, mm->num_readies );

    /* turn off CISX */
    cisx_interrupt_disable();
    cisx_set_bypass( true );

    /* pull all icebufs from running list, put back on ready list */
    while( 1 ) {
        icebuf = icebuf_pop( &mm->running );
        if( icebuf==NULL ) {
            break;
        }
        mm->num_running--;
        XASSERT( mm->num_running >=0, mm->num_running );

        /* attach to the end of our running queue */
        ATInsertTailList( &mm->readies, &icebuf->listnode );
        mm->num_readies++;
        XASSERT( mm->num_readies <= mm->num_descriptors_allocated, mm->num_readies ); 

        /* The memory was allocated in one aggregate chunk then dividied
         * between the icebufs. Clear out the data pointers. We'll free the
         * aggregate buffer later.
         */
        icebuf->data = NULL;
        icebuf->dma_handle = (dma_addr_t)NULL;
        icebuf->num_rows = 0;
        icebuf->max_rows = 0;
        icebuf->datalen = 0;
        icebuf->bytes_per_row = 0;
    }

    /* free the memory we allocated for this channel */
    ice_free_counted_list( &mm->readies, mm->num_readies );
    mm->num_readies = 0;
    
    XASSERT( mm->num_running==0, mm->num_running );
    ASSERT( ATIsListEmpty( &mm->running ) );

    for( i=0 ; i<num_odma_channels; i++ ) {
        cisx_odma_channel_disable( odma_channels[i] );
        cisx_channel_close( &cisx_odma_channel[ odma_channels[i] ] );
    }
    for( i=0 ; i<num_idma_channels; i++ ) {
        cisx_idma_channel_disable( idma_channels[i] );
        cisx_channel_close( &cisx_idma_channel[ idma_channels[i] ] );
    }

    /* davep 22-Apr-2013 ; release our aggregate cisx buffer */
    if( mm->cisx_buf_is_fast_memory ) {
        scanmem_free_fast_memory( SCANMEM_TAG_CISX, mm->cisx_buf, mm->cisx_buf_num_bytes, 0 );
        mm->cisx_buf = NULL;
        mm->cisx_buf_num_bytes = 0;
    }
    else {
        PTR_FREE( mm->cisx_buf );
        mm->cisx_buf_num_bytes = 0;
    }

    /* clean up the hardware */
    cisx_reset();
}

void cisx_dma_dump( void )
{
    int i;

    for( i=0 ; odma_registers_list[i]!=0 ; i++ ) {
        odma_channel_dump( odma_registers_list[i] );
    }
    for( i=0 ; idma_registers_list[i]!=0 ; i++ ) {
        idma_channel_dump( idma_registers_list[i] );
    }

    cisx_ddma_dump();
}

void cisx_ddma_dump( void )
{
    int i;

    for( i=0 ; i<CISX_ODMA_NUM_CHANNELS ; i++ ) {
        if( cisx_odma_channel[i].desc_list.list ) {
            ddma_descriptor_list_dump( &cisx_odma_channel[i].desc_list);
        }
    }
    for( i=0 ; i<CISX_IDMA_NUM_CHANNELS ; i++ ) {
        if( cisx_idma_channel[i].desc_list.list ) {
            ddma_descriptor_list_dump( &cisx_idma_channel[i].desc_list);
        }
    }

    cisx_ddma_data_peek();
}

void cisx_ddma_data_peek( void )
{
    struct cisx_mm *mm;
    struct ice_dma_buffer *icebuf;

    dbg2( "%s\n", __FUNCTION__ );

    mm = &cisx_mm;

//    XASSERT( mm->num_running==MAX_CISX_DMA_DESCRIPTORS, mm->num_running );
    icebuf = CONTAINING_RECORD( mm->running.nextEntry, struct ice_dma_buffer, listnode );
    ASSERT( icebuf );

    icebuf_sanity( icebuf );
    
//    cpu_dcache_invalidate_region( icebuf->data, 64 );
//    scanlog_hex_dump( icebuf->data, 64 );
    
#if 0
    /* The following little snippet of code will peek at the first buffer of
     * the ODMA and the IDMA channels. The buffer at the head of each
     * descriptor list is hexdump'd.
     */
    int i;
    struct cisx_channel *cch;
    volatile CISX_ODMA_REGS_t *odma_regs;

    for( i=0 ; i<CISX_ODMA_NUM_CHANNELS ; i++ ) {
        cch = &cisx_odma_channel[i];
        dbg2( "%s %d i=%d ch=%d\n", __FUNCTION__, __LINE__, i, cch->channel );
        odma_regs = getregs( cch->channel );
        ddma_data_peek( &cch->desc_list, odma_regs->line_size );
    }
    ddma_data_peek( &cisx_idma_channel[0].desc_list, idma_regs->line_width );
#endif

#if 0
    /* this big snippet of code will peek at every single buffer in the IDMA
     * descriptor chain
     */
    uint8_t *ptr;
    struct ddma_descriptor *head_desc;
    struct ddma_descriptor *desc;
    int idma_idx;
    struct cisx_channel *idma;

    for( idma_idx=0 ; idma_idx<CISX_IDMA_NUM_CHANNELS ; idma_idx++ ) {
        dbg2( "%s idma_idx=%d\n", __FUNCTION__, idma_idx );

        idma = &cisx_idma_channel[idma_idx];
        if( !idma || !idma->desc_list.list ) {

            dbg2( "%s %d should not see this!\n", __FUNCTION__, __LINE__ );

            continue;
        }

        head_desc = &cisx_idma_channel[idma_idx].desc_list.list[0];
        desc = head_desc;
        do { 
            ddma_desc_dump( desc );

            XASSERT( desc->fw_src_addr, (uint32_t)desc );
            ptr = (uint8_t *)desc->fw_src_addr;
            if( ptr ) {
                if( 1 ) {
                    /* dump a small chunk */
                    cpu_dcache_invalidate_region( ptr, 32 );
                    scanlog_hex_dump( ptr, 32 );
                }
                else {
                    /* dump entire buffer */
                    dbg2( "%s ptr=%p len=%d %d\n", __FUNCTION__, 
                                ptr, desc->transfer_len_bytes, cpu_get_dcache_line_size() );
                    cpu_dcache_invalidate_region( ptr, desc->transfer_len_bytes & ICE_DMA_BYTE_ALIGN_MASK );
    //                cpu_dcache_invalidate_region( ptr, desc->transfer_len_bytes );
                    scanlog_hex_dump( ptr, desc->transfer_len_bytes );
                }
            }
            desc = (struct ddma_descriptor *)desc->fw_next_descriptor_addr;
        } while( desc != head_desc );
    }
#endif
}

void cisx_dma_reset( void )
{
    int i;

    for( i=0 ; i<CISX_ODMA_NUM_CHANNELS ; i++ ) {
        cisx_odma_channel_disable( i );
        cisx_odma_channel_reset( i );
    }
    for( i=0 ; i<CISX_IDMA_NUM_CHANNELS ; i++ ) {
        cisx_idma_channel_disable( i );
        cisx_idma_channel_reset( i );
    }
}

void cisx_reset( void )
{
    int i;
    volatile CISX_INT_REGS_t * cisxregs;

    /* shut it all off before we start poking at it */
    cisx_interrupt_disable();
    cisx_set_bypass( true );

    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];

        /* restore CISX to power-on defaults */
        cisxregs->CisxCfg = CISX_INT_CISXCFG_BYPASS_MASK 
                          | CISX_INT_CISXCFG_NUMCHANS_MASK 
                          ;

        /* sanity check to make sure my code is pointed at the right addresses */
        XASSERT( cisxregs->CisxCfg==(CISX_INT_CISXCFG_BYPASS_MASK | CISX_INT_CISXCFG_NUMCHANS_MASK), 
                (uint32_t)cisxregs );

        cisxregs->Chan0Pix = 0;
        cisxregs->Chan1Pix = 0;
        cisxregs->Chan2Pix = 0;

        cisx_interrupt_disable();

        /* ack all pending interrupts */
        cisxregs->IntAck = ~0;
    }

    cisx_dma_reset();

    num_odma_channels = 0;
    num_idma_channels = 0;
    memset( odma_channels, 0, sizeof(odma_channels) );
    memset( idma_channels, 0, sizeof(idma_channels) );
}

scan_err_t cisx_soft_setup( void )
{
//    uint32_t num32;
    int i;
    volatile CISX_INT_REGS_t * cisxregs;
    uint32_t num32;

    cisx_dbg2( "%s\n", __FUNCTION__ );

    cisx_reset();

    cisx_interrupt_enable();

    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];

        /* davep 25-Mar-2013 ; The cbiout_pace can be overwritten by cisxsensor
         * structure passed to cisx_open(). The cisxsensor structure is
         * typically read from platform specific code scansen_get_cisx_conf().
         */

        if( !SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask) ) {
            scands_get_integer_with_default( "cbiout_pace", &num32, 2 );
        }
        else {
            scands_get_integer_with_default( "cbiout_pace", &num32, 10 );
        }

        /* davep 07-Apr-2011 ; XXX shotgun debug ; slow down the output */
//        cisxregs->CisxCfg = CISX_INT_CISXCFG_CBIOUT_PACE_REPLACE_VAL( cisxregs->CisxCfg, 10 );
//        cisxregs->CisxCfg = CISX_INT_CISXCFG_CBIOUT_PACE_REPLACE_VAL( cisxregs->CisxCfg, 2 );
//        cisxregs->CisxCfg = CISX_INT_CISXCFG_CBIOUT_PACE_REPLACE_VAL( cisxregs->CisxCfg, 15 );
        cisxregs->CisxCfg = CISX_INT_CISXCFG_CBIOUT_PACE_REPLACE_VAL( cisxregs->CisxCfg, num32 );

        cisxregs->CisxCfg = CISX_INT_CISXCFG_NUMCHANS_REPLACE_VAL( cisxregs->CisxCfg, 3 );
    }

    /* davep 28-Jun-2011 ; default to 3 which isn't 0,1,2 but compensates for
     * the two pixels the scan block eats (which throws off the state machines)
     */
    cisx_set_3channel_order( 3 );

    cisx_set_bypass( true );

    return SCANERR_NONE;
}

#ifdef __KERNEL__
static scan_err_t cisx_capture_interrupt( void )
{
    int retcode;

    /* wait until explicitly enabled */
    cisx_interrupt_disable(); 

    retcode = request_irq( INTNUM_CISX, cisx_interrupt, 0, "icecisx", NULL );
    dbg2( "%s request_irq retcode=%d\n", __FUNCTION__, retcode );
    XASSERT(retcode==0, retcode );

    return SCANERR_NONE;
}

static void cisx_release_interrupt( void )
{
    cisx_interrupt_disable();
    /* disable at the OS level as well */
    disable_irq( INTNUM_CISX );
    free_irq( INTNUM_CISX, NULL );
}
#else
static scan_err_t cisx_capture_interrupt( void )
{
    intAttach( INTNUM_CISX, 0, cisx_interrupt, 0 );
    intEnable( INTNUM_CISX );
    return SCANERR_NONE;
}

static void cisx_release_interrupt( void )
{
    cisx_interrupt_disable();
    /* disable at the OS level as well */
    intDisable( INTNUM_CISX );
    intDetach( INTNUM_CISX );
}
#endif

scan_err_t cisx_onetime_init( void )
{
    struct cisx_mm *mm;
    int i;
    volatile CISX_INT_REGS_t * cisxregs;

    dbg2( "%s\n", __FUNCTION__ );

    mm = &cisx_mm;

    cisx_interrupt_disable();
    cisx_capture_interrupt();

    cisx_soft_setup();

    /* sanity check to make sure my code is pointed at the right addresses */
    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];
        XASSERT( cisxregs->CisxCfg & (CISX_INT_CISXCFG_BYPASS_MASK | CISX_INT_CISXCFG_NUMCHANS_MASK), 
                (uint32_t)cisxregs );
    }

    /* our dma descriptors must be padded to a cache line size so we can flush
     * them from cache without breaking neighboring cache lines
     */
    XASSERT( sizeof(struct ddma_descriptor)==cpu_get_dcache_line_size(), 
            sizeof(struct ddma_descriptor));

    /* initialize global data structures */
    for( i=0 ; i<CISX_ODMA_NUM_CHANNELS ; i++ ) {
        cisx_channel_init( &cisx_odma_channel[i], "odma", i );
    }
    for( i=0 ; i<CISX_IDMA_NUM_CHANNELS ; i++ ) {
        cisx_channel_init( &cisx_idma_channel[i], "idma", i );
    }

    ATInitList( &mm->readies );
    ATInitList( &mm->running );
    mm->num_readies = 0;
    mm->num_running = 0;

//    /* davep 25-Apr-2011 ; XXX temp debug */
//    cisx_dump();

    return SCANERR_NONE;
}

scan_err_t cisx_cleanup_module( void )
{
    cisx_release_interrupt();

    return SCANERR_NONE;
}

void cisx_get_pixels_per_row( uint32_t *ppr ) 
{
    uint32_t total;
    volatile CISX_INT_REGS_t * cisxregs;

    /* test/debug function ; get total pixel count from chanNpix registers */
    total = 0;

    /* Note only reading the first CISX. */
    cisxregs = cisx_regs_list[0];

    /* only capture one channel ; all channels should be identical */
    total += CISX_INT_CHAN0PIX_CHAN0DUM_MASK_SHIFT(cisxregs->Chan0Pix); 
    total += CISX_INT_CHAN0PIX_CHAN0DAT_MASK_SHIFT(cisxregs->Chan0Pix); 

    total += CISX_INT_CHAN1PIX_CHAN1DUM_MASK_SHIFT(cisxregs->Chan1Pix); 
    total += CISX_INT_CHAN1PIX_CHAN1DAT_MASK_SHIFT(cisxregs->Chan1Pix); 

    total += CISX_INT_CHAN2PIX_CHAN2DUM_MASK_SHIFT(cisxregs->Chan2Pix); 
    total += CISX_INT_CHAN2PIX_CHAN2DAT_MASK_SHIFT(cisxregs->Chan2Pix); 

    *ppr = total;
}

uint32_t cisx_get_int_pending( void )
{
    volatile CISX_INT_REGS_t * cisxregs;

    /* This function used during CISX regression testing. Rather than
     * triggering the interrupt then trying to do huge logging dumps from
     * interrupt context, we can poll on the CISX interrupt status.
     */

    /* davep 08-Mar-2013 ; XXX for now, hardwire to 0'th CISX */
    cisxregs = cisx_regs_list[0];
    return cisxregs->IntPend;
}

/**
 * \brief  allow Cfg.cbiout_pace to be platform specific. 
 *
 * "CBI Output Pacing"
 *
 * VallidOut frequency = BusClkFreq/(cbiout_pace + 2) 
 * ValidOut frequency (max) = BusClkFreq/2
 * 
 * \author David Poole
 * \date 25-Mar-2013
 */

void cisx_set_cbiout_pace( uint32_t cbiout_pace )
{
    int i;
    volatile CISX_INT_REGS_t * cisxregs;
    uint32_t num32;

    if( !SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask) ) {
        scands_get_integer_with_default( "cbiout_pace", &num32, 2 );
    }
    else {
        scands_get_integer_with_default( "cbiout_pace", &num32, cbiout_pace );
    }

    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];
        cisxregs->CisxCfg = CISX_INT_CISXCFG_CBIOUT_PACE_REPLACE_VAL(
                cisxregs->CisxCfg, num32 );
    }
}

/**
 * \brief  Get CISX Dual Channel enable/disable state
 *
 * Created for sanity checking. All of Scan block, CISX, and PIC must be in
 * agreement for dual channel mode.
 *
 * \author David Poole
 * \date 03-Apr-2013
 */

bool cisx_get_dual_channel_enabled( void )
{
    bool enabled;
    int i;
    volatile CISX_INT_REGS_t * cisxregs;

#if !defined HAVE_NSENSOR_SUPPORT
    /* no chance in chocolate syrup you'll ever have dual scan enabled */
    return false;
#endif

    /* all CISX blocks enabled => dual scan */
    enabled = true;
    for( i=0; cisx_regs_list[i] ; i++ )  {
        cisxregs = cisx_regs_list[i];

        if( cisxregs->CisxCfg & CISX_INT_CISXCFG_BYPASS_MASK ) {
            /* bit set => this channel in bypass */

            enabled = false;
        }
    }

    /* sneak in some sanity checking */
    if( enabled ) {
        XASSERT( num_odma_channels==6, num_odma_channels );
        XASSERT( num_idma_channels==2, num_idma_channels );
    }

    /* make sure hardware agrees with our global flag */
    XASSERT( enabled==(!SCANIMG_SINGLE_SENSOR(cisx_mm.sensor_bitmask)), cisx_mm.sensor_bitmask);

    return enabled;
}

