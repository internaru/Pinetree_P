/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2006   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#include <string.h>
#include <math.h>

#include "scos.h"

#include "lassert.h"
#include "interrupt_api.h"
#include "memAPI.h"
#include "intnums.h"
#include "cpu_api.h"
#include "regAddrs.h"

#include "MFilter_regmasks.h"
#include "MFilter_regstructs.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scan.h"
#include "piehw.h"
#include "pie.h"
#include "piereset.h"
#include "scanvars.h"
#include "piecs.h"
#include "safeint.h"

/* Define to turn on more verbose debugging */
//#define PIE_DEBUG  

#ifdef PIE_DEBUG
  #define pie_dbg2 dbg2 
#else
  #define pie_dbg2(...) 
#endif

static struct pie_interrupt_stats pie_interrupt_stats;
static bool pie_enable_lock = false;
/* small steps to the RegBuild headers */
static volatile SCULPTOR_REGS_t* const sculptor_regs = (volatile SCULPTOR_REGS_t *)(ICE_PIE_MFILTER_SCULPTOR_BASE);

/**
 * \def PIE_DMA_INTERRUPT_BITS
 * Shorthand for all the PIE read/write DMA interrupt bits.
 */
#define PIE_DMA_INTERRUPT_BITS  (~0)

#define xPIE_DMA_INTERRUPT_BITS  \
       (    PIE_CI_RDMA0_BIT    \
          | PIE_CI_RDMA1_BIT    \
          | PIE_CI_RDMA2_BIT    \
          | PIE_CI_RDMA3_BIT    \
          | PIE_CI_RDMA4_BIT    \
          | PIE_CI_RDMA5_BIT    \
          | PIE_CI_WDMA0_BIT    \
          | PIE_CI_WDMA1_BIT    \
          | PIE_CI_WDMA2_BIT )

/**
 * \def PIEHP_DMA_INTERRUPT_BITS
 * Shorthand for all the PIE HalfPack read/write DMA interrupt bits.
 */
#define PIEHP_DMA_INTERRUPT_BITS \
    (   PIE_CI_MONOHP_WDMA_BIT   \
      | PIE_CI_MONOHP_RDMA_BIT   )


/**
 * \brief Main PIE interrupt handler.
 *
 *  PIE interrupt handler. Receives interrupts from the PIE read/write DMA
 *  interrupts and the Mono HalfPack read/write DMA.
 *
 * \author David Poole
 * \date 6-Mar-05
 *
 **/
extern int dbg_cnt_pie_interrupt;	//add.lsh.for pipe debug

#ifdef __KERNEL__
static irqreturn_t pie_interrupt( int irq, void *dev_id ) 
#else
static void pie_interrupt( uint32_t param )
#endif
{
    uint32_t ipend;

//    dbg1("%s 0x%x\n", __FUNCTION__, *PIE_CI_IPR );
	dbg_cnt_pie_interrupt++;
    pie_interrupt_stats.count++;

    ipend = *PIE_CI_IPR;

    /* if we have a read or write DMA interrupt, hand it off */
    /* 9-Mar-05 ; as of this writing only the PIE input/output read/write DMA
     * interrupts are used. Eventually we'll have the Mono HalfPack read/write
     * DMA interrupts enabled.
     */
    if( ipend & PIE_DMA_INTERRUPT_BITS ) {
        pie_dma_interrupt( ipend );
    }

#ifdef __KERNEL__
    return IRQ_HANDLED;
#endif
}

uint32_t pie_interrupt_disable( void )
{
    uint32_t was_enabled;

    was_enabled = *PIE_CI_IENR && 1;

    /* disable everything */
    *PIE_CI_IENR = 0;

    return was_enabled;
}

void pie_interrupt_enable( void )
{
    /* enable everything we can */
    if(pie_enable_lock == false)	// mod.lsh for background removal
    	*PIE_CI_IENR = ~0; 
    else
    	dbg2( "[BG] bypass pie interrupt!!\n");
}

void pie_interrupt_enable_lock( void )		// add.lsh for background removal
{
	dbg2( "[BG] PIE Interrupt lock!!\n");
	pie_enable_lock = true;
}

void pie_interrupt_enable_unlock( void )	// add.lsh for background removal
{
	dbg2( "[BG] PIE Interrupt unlock!!\n");
	pie_enable_lock = false;
}

bool pie_interrupt_lock_status_get( void )	// add.lsh for background removal
{
	return pie_enable_lock;
}
/**
 * \brief Restores a block of 32-bit registers to a value.
 *
 *  Originally written to restore PIE subblock lookup tables (such as the Color
 *  Space Conversion lookup tables) and other register groups (such as the
 *  Color Space Conversion 3x3 matrices) to zero.
 *
 * \param  ptr32 base pointer of registers to set to value
 * \param  count how many 32-bit longwords to set
 * \param  value new value to write to location(s)
 *
 * \author David Poole
 * \date 3-May-05
 *
 **/
static void
regblockset( volatile uint32_t *ptr32, uint32_t count, uint32_t value )
{
    while( count ) {
        *ptr32++ = value;
        count--;
    }
}

/**
 * \brief  convenience function to return the number of bits in a specific PIE
 *         pixel type
 *
 * \author David Poole
 * \date 22-Apr-2011
 *
 */

int pie_get_bits_per_pixel( pie_pixel_t ppt )
{
    int bits_per_pixel;

    bits_per_pixel = 0;

    switch( ppt ) {
        case PIE_PIXEL_MONO :
        case PIE_PIXEL_3_PLANES :
            bits_per_pixel = 8;
            break;

        case PIE_PIXEL_XRGB :
        case PIE_PIXEL_RGBX :
            bits_per_pixel = 32;
            break;

        case PIE_PIXEL_RGB :
            bits_per_pixel = 24;
            break;

        default:
            /* wtf? */
            XASSERT( 0, ppt );
            break;
    }

    return bits_per_pixel;
}

int pie_get_bytes_per_pixel( pie_pixel_t ppt )
{
    return pie_get_bits_per_pixel(ppt) / 8;
}

void pie_pixel_type_to_scan_data_type( pie_pixel_t ppt, scan_data_type *sdt )
{
    *sdt = SCAN_DATA_NULL_TYPE;

    switch( ppt ) {
        case PIE_PIXEL_XRGB :
            *sdt = SCAN_DATA_TYPE_XRGB;
            break;

        case PIE_PIXEL_RGBX :
            *sdt = SCAN_DATA_TYPE_RGBX;
            break;

        case PIE_PIXEL_3_PLANES :
            *sdt = SCAN_DATA_TYPE_PLANE;
            break;

        case PIE_PIXEL_MONO :
            *sdt = SCAN_DATA_TYPE_MONO;
            break;

        case PIE_PIXEL_RGB :
            *sdt = SCAN_DATA_TYPE_RGB;
            break;

        default : 
            /* unknown/unsupported PIE output format */
            XASSERT( 0, ppt );
            break;
    }
}

/**********************************************************
 *
 * BDE Functions
 *
 *********************************************************/

/**
 * \brief Reset Bit Depth Enchance (BDE)
 *
 *  Resets BDE (Bit Depth Enhance) lookup table and config register(s) back to
 *  power-on default values.
 * 
 * \author David Poole
 * \date 4-May-05
 *
 **/
void 
pie_bde_reset( void )
{
    pie_dbg2( "%s\n", __FUNCTION__ );

    /* set Bypass mode and allow CPU access */
    *PIE_BDE_CFG = PIE_BDE_CFG_R | PIE_BDE_CFG_CPU;

    /* restore LUT to power-on values */
    regblockset( PIE_BDE_LUT, PIE_BDE_LUT_SIZE, 0 );

    /* set cfg to power-on value (disables CPU access, enable Bypass) */
    *PIE_BDE_CFG = PIE_BDE_CFG_R;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 19-Feb-2010
 *
 */

void pie_bde_set_bypass( bool bypass )
{
    if( bypass ) {
        /* set the bypass bit */
        *PIE_BDE_CFG |= PIE_BDE_CFG_BYPASS;
    }
    else {
        /* clear the bypass bit */
        *PIE_BDE_CFG &= ~PIE_BDE_CFG_BYPASS;
    }
}

/**
 * \brief Enable/disable CPU access to PIE BDE LUT
 *
 * \author David Poole
 * \date 10-Feb-2010
 *
 */

void pie_bde_enable_fw_access( bool access ) 
{
    if( access ) {
        *PIE_BDE_CFG |= PIE_BDE_CFG_CPU;
    }
    else {
        *PIE_BDE_CFG &= ~PIE_BDE_CFG_CPU;
    }
}

/**
 * \brief Setup Bit Depth Enhance block
 *
 * Initialize the BDE config register and lookup table.  Prior to running
 * pixels through the block, firmware should program the BDE Configuration
 * register (BDECR) and BDE LUT using this function.
 *
 * \param[in] mode color or mono
 * \param[in] lut array of length \link #PIE_BDE_LUT_SIZE PIE_BDE_LUT_SIZE \endlink 
 *            of entries for the bit depth enhance lookup table
 *
 * \author David Poole
 * \date 26-July-05
 *
 **/
void
pie_bde_setup( pie_bde_mode mode, uint32_t lut[] )
{
    int i;
    volatile uint32_t *ptr32;

    /* The caller should have called pie_reset() before calling us so make sure
     * all our our LUT's are zero to test pie_reset().
     *
     * If the assertion marked "@@@" below fails, we're not resetting the LUT
     * to zeroes in pie_reset() or we're not calling pie_reset() before calling
     * this function.
     */

    /* from "MA_BitDepthEnhance.pdf":
     *
     * Prior to running pixels through the block, firmware should program the
     * BDE Configuration register (BDECR) and BDE LUT as follows.
     *
     * 1. Enable CPU write access to the LUT by writing a 1 to the
     *    BDECR.C bit (CPU Access bit).
     * 2. Write the LUT values to the entries of the LUT SRAM.
     * 3. Disable CPU write access to the LUT by writing a 0 to BDECR.C bit.
     * 4. Select the Bypass and Mono options by writing to the BDECR.B and
     *    BDECR.M bits. Note that steps 3 and 4 can be combined into one write
     *    access to the BDECR.
     */

    pie_dbg2( "%s\n", __FUNCTION__ );

    /* Must set to bypass while we program the LUT. */
    if( mode==PIE_BDE_MODE_COLOR ) {
        *PIE_BDE_CFG = PIE_BDE_CFG_BYPASS | PIE_BDE_CFG_CPU | PIE_BDE_CFG_COLOR;
    }
    else {
        *PIE_BDE_CFG = PIE_BDE_CFG_BYPASS | PIE_BDE_CFG_CPU | PIE_BDE_CFG_MONO;
    }

    /* init the LUT user's values */
    ptr32 = PIE_BDE_LUT;
    for( i=0 ; i<PIE_BDE_LUT_SIZE ; i++ ) {
        /* @@@ - see comments about this assertion above */
        XASSERT( *ptr32==0, *ptr32 );

        /* identity ; same number out as number in */
        *ptr32++ = PIE_BDE_LUT_ENTRY(lut[i]);
    }

    /* clear bypass so block is now programmed and enabled;
     * clear CPU access bit so we can keep our code from messing with it
     */
    *PIE_BDE_CFG &= ~(PIE_BDE_CFG_BYPASS|PIE_BDE_CFG_CPU);

//    dbg2("BDE_CFG=0x%x\n", *PIE_BDE_CFG );
}

void pie_bde_dump( void )
{
    dbg2( "BDE_CFG=0x%x\n", *PIE_BDE_CFG );
}

/**********************************************************
 *
 * CSC Functions
 *
 *********************************************************/

/**
 * \brief Resets the three Color Space Conversion blocks.
 *
 *  Resets:
 *      \li RGB2esRGB 
 *      \li esRGB2YCrCb 
 *      \li YCrCb2esRGB 
 *
 *  Restores all 3x3 matrices to zeroes. Restores the RGB2esRGB LUT to zeroes. 
 *  (Zero is power-on default for both.)
 *
 * \author David Poole
 * \date 3-May-05
 *
 **/
void pie_csc_reset( void )
{
    pie_dbg2( "%s\n", __FUNCTION__ );

    /* 
     * setup RGB2esRGB; 
     */

    /* set to defaults (bypass), enable LUT CPU access */
    *PIE_CSC_RGB_CCR = PIE_CSC_RGB_CCR_R | PIE_CSC_RGB_CCR_CPU;

    /* clear coefficient 3x3 matrix */
    regblockset( PIE_CSC_RGB_MUL, 9, 0 );

    /* clear CSC RGB2esRGB LUT(s) */
    if( pie_csc_has_unified_rgb_luts() ) {
        /* running on something with one LUT for all of CSC RGB */
        regblockset( PIE_CSC_RGB_LUT, PIE_CSC_RGB_LUT_SIZE, 0 );
    }
    else {
        /* running on something with one LUT for each of R,G,B */
        regblockset( PIE_CSC_RED_LUT, PIE_CSC_RGB_LUT_SIZE, 0 );
        regblockset( PIE_CSC_GREEN_LUT, PIE_CSC_RGB_LUT_SIZE, 0 );
        regblockset( PIE_CSC_BLUE_LUT, PIE_CSC_RGB_LUT_SIZE, 0 );
    }

    /* set back to defaults (bypass); disables LUT CPU access */
    *PIE_CSC_RGB_CCR = PIE_CSC_RGB_CCR_R;


    /* 
     * setup esRGB2YCrCb 
     */

    /* set to defaults (bypass), no LUT so don't bother with CPU access */
    *PIE_CSC_ESR_CCR = PIE_CSC_ESR_CCR_R;

    /* clear coefficient 3x3 matrix */
    regblockset( PIE_CSC_ESR_MUL, 9, 0 );

    /* 
     * setup YCrCb2esRGB 
     */
    /* set to defaults (bypass), no LUT so don't bother with CPU access */
    *PIE_CSC_YCC_CCR = PIE_CSC_YCC_CCR_R;

    /* clear coefficient 3x3 matrix */
    regblockset( PIE_CSC_YCC_MUL, 9, 0 );
}


/**
 * \brief  Probe the current ASIC to determine if we have one CSC LUT or three
 *
 * Introduced three separate CSC RGB2esRGB LUTS, one for each of R,G,B.
 * Previous platforms used one for all three channels.
 *
 *
 * \author David Poole
 * \date 29-Oct-2007
 *
 */

bool pie_csc_has_unified_rgb_luts( void )
{
#ifdef HAVE_THREE_RGB2ESRGB_LUT 
    /* davep 15-Apr-2010 ; so far, everything newer than ~2005 */
    return false;
#else
    /* asic family circa 2005 */
    return true;
#endif
}

/**
 * \brief enable/disable firmware access to CSC LUT 
 *
 *
 * \author David Poole
 * \date 03-Jul-2006
 *
 */

void
pie_csc_rgb_enable_fw_access( bool access ) 
{
    if( access ) {
        *PIE_CSC_RGB_CCR |= PIE_CSC_RGB_CCR_CPU;
    }
    else {
        *PIE_CSC_RGB_CCR &= ~PIE_CSC_RGB_CCR_CPU;
    }
}

/**
 * \brief  enable/disable CSC bypass
 *
 * Note! CSC is three blocks with interlocking functionality. Bypass-all will
 * bypass all three sub-blocks but disabling bypass without loading all the
 * proper luts, etc, would be plain nutty.
 *
 * \author David Poole
 * \date 19-Feb-2010
 *
 */

void pie_csc_set_bypass_all( bool bypass )
{
    if( bypass ) {
        /* set to block to bypass */
        *PIE_CSC_RGB_CCR |= (PIE_CSC_RGB_CCR_BYPASS|PIE_CSC_RGB_CCR_BYLUT);
        *PIE_CSC_ESR_CCR |= (PIE_CSC_ESR_CCR_BYPASS|PIE_CSC_ESR_CCR_OFFSETBYPASS|PIE_CSC_ESR_CCR_POSTOFFBYPASS);
        *PIE_CSC_YCC_CCR |= (PIE_CSC_YCC_CCR_BYPASS|PIE_CSC_YCC_CCR_OFFSETBYPASS|PIE_CSC_YCC_CCR_PREOFFBYPASS);
    }
    else {
        /* clear bypass bits */
        *PIE_CSC_RGB_CCR &= ~(PIE_CSC_RGB_CCR_BYPASS|PIE_CSC_RGB_CCR_BYLUT);
        *PIE_CSC_ESR_CCR &= ~(PIE_CSC_ESR_CCR_BYPASS|PIE_CSC_ESR_CCR_OFFSETBYPASS|PIE_CSC_ESR_CCR_POSTOFFBYPASS);
        *PIE_CSC_YCC_CCR &= ~(PIE_CSC_YCC_CCR_BYPASS|PIE_CSC_YCC_CCR_OFFSETBYPASS|PIE_CSC_YCC_CCR_PREOFFBYPASS);
    }
}

/**
 * \brief debug print all of CSC 
 *
 *
 * \author David Poole
 * \date 03-Jul-2006
 *
 */

void
pie_csc_dump( void )
{
    dbg2( "CSC_RGB_CCR=0x%x CSC_ESR_CCR=0x%x CSC_YCC_CCR=0x%x\n", 
                *PIE_CSC_RGB_CCR, *PIE_CSC_ESR_CCR, *PIE_CSC_YCC_CCR );

    pie_csc_rgb_enable_fw_access( true );

    if( pie_csc_has_unified_rgb_luts() ) {
        /* CSC RGB LUT; overflow register was memory contiguous */
        scanlog_hex_dump( (unsigned char *)PIE_CSC_RGB_LUT, PIE_CSC_RGB_LUT_SIZE*sizeof(uint32_t) );
    }
    else {
        /* davep 29-Oct-2007 ; In the separated CSC RGB LUTs, the overflow
         * registers moved to after the blue LUT. Want to hide the movement from
         * the outside world as much as possible.   See also pie_csc_rgb_set_lut()
         *
         * The (PIE_CSC_RGB_LUT_SIZE-1) below is because the internal srams are
         * only 256 4-byte values but the logical table must be 257. 
         */

        /* CSC red LUT */
        scanlog_hex_dump( (unsigned char *)PIE_CSC_RED_LUT, (PIE_CSC_RGB_LUT_SIZE-1)*sizeof(uint32_t) );
        scanlog_hex_dump( (unsigned char *)PIE_CSC_RED_OVERFLOW, sizeof(uint32_t) );

        /* CSC green LUT */
        scanlog_hex_dump( (unsigned char *)PIE_CSC_GREEN_LUT, (PIE_CSC_RGB_LUT_SIZE-1)*sizeof(uint32_t) );
        scanlog_hex_dump( (unsigned char *)PIE_CSC_GREEN_OVERFLOW, sizeof(uint32_t) );

        /* CSC blue LUT */
        scanlog_hex_dump( (unsigned char *)PIE_CSC_BLUE_LUT, (PIE_CSC_RGB_LUT_SIZE-1)*sizeof(uint32_t) );
        scanlog_hex_dump( (unsigned char *)PIE_CSC_BLUE_OVERFLOW, sizeof(uint32_t) );
    }
    pie_csc_rgb_enable_fw_access( false );

    scanlog_hex_dump( (unsigned char *)PIE_CSC_RGB_MUL, 9*sizeof(uint32_t) );
    scanlog_hex_dump( (unsigned char *)PIE_CSC_ESR_MUL, 9*sizeof(uint32_t) );
    scanlog_hex_dump( (unsigned char *)PIE_CSC_YCC_MUL, 9*sizeof(uint32_t) );
}

void
pie_csc_rgb_set_lut( uint32_t *data, volatile uint32_t *lut )
{
    volatile uint32_t *ptr32;
    int i;
    uint32_t num32;

    /* note: assumes caller enabled CPU access! see 
     * pie_csc_rgb_enable_fw_access() 
     */

    /* stupid human check */
    XASSERT( lut==PIE_CSC_RGB_LUT   ||
             lut==PIE_CSC_RED_LUT   || 
             lut==PIE_CSC_GREEN_LUT ||
             lut==PIE_CSC_BLUE_LUT, (uint32_t)lut );

    ptr32 = lut;

//    dbg2( "%s lut=%p\n", __FUNCTION__, ptr32 );

    /* initialize LUT */
    if( pie_csc_has_unified_rgb_luts() ) {
        for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE ; i++ ) {
            *ptr32++ = *data++;
        }
    }
    else {
        /* davep 29-Oct-2007 ; the overflow register moved; the lut is
         * now 256 4-byte elements and the overflow sits above all three of the
         * new CSC LUTs
         */
        /* davep 09-Apr-2010 ; adding a verification step because we have luts
         * that can move around between ASICs
         */
        for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE-1 ; i++ ) {
            uint32_t foo32;

            num32 = PIE_CSC_RGB_LUT_LUTVAL( *data );
            *ptr32 = num32;

            /* davep 18-Oct-2011 ; (asicbz 3687) dare not disturb foo32 until
             * 18-Oct-2013 
             */
            foo32 = *ptr32;

            XASSERT( *ptr32==num32, *ptr32 );
            ptr32++;
            data++;

            /* quiet a warning */
            foo32 = foo32;
        }

        /* This if/else is a bit of a kludge ; want to hide the detail of the
         * overflow register from the caller so we have to know what LUT they're
         * writing to.  (Started with a switch; gcc 4.1 complained.)
         *
         * From the PIE CSC MA:
         * "This means that when NUM_LUTS = 3 that the overflow registers for
         * the Color1 and Color2 tables will not be contiguous with the main LUT
         * data, but will follow the Color3 overflow data in the address map." 
         */
        if( lut==PIE_CSC_RED_LUT ) {
            ptr32 = PIE_CSC_RED_OVERFLOW;
        }
        else if( lut==PIE_CSC_GREEN_LUT ) {
            ptr32 = PIE_CSC_GREEN_OVERFLOW;
        }
        else if( lut== PIE_CSC_BLUE_LUT ) {
            ptr32 = PIE_CSC_BLUE_OVERFLOW;
        }
        else {
            XASSERT( 0, (uint32_t)lut); 
        }

//        dbg2( "%s overflow=%p\n", __FUNCTION__, ptr32 );

        /* write the last value */
        *ptr32++ = *data++;
    }

}

/**
 * \brief Setup the Color Space Conversion blocks
 *
 * This function is a starting point for future development. As of this writing
 * (26-July-05), it does the simplest thing necessary to push data through
 * MultiFilter. 
 *
 * Enables all three of the color conversion sub-blocks (RGB2esRGB, esRGB2YCC,
 * YCC2esRGB). Sets the RGB2esRGB LUT to a 1:1 lookup and its 3x3 matrix to an
 * identity.
 *
 * Sets the esRGB-YCC and YCC-esRGB conversion matrices as follows:
 *
 * sRGB to YCC Coefficients:
 * 
\htmlonly
    <table border="0" cellspacing="5">
    <tr><td>0.2990</td>
        <td>0.5870</td>
        <td>0.1140</td>
    </tr>
    <tr><td>-0.1687</td>
        <td>-0.3313</td>
        <td> 0.5000</td>
    </tr>
    <tr><td> 0.5000</td>
        <td>-0.4187</td>
        <td>-0.0813</td>
    </tr>
    </table>
\endhtmlonly
 * 
 * 
 * YCC to sRGB Coefficients:
 *
\htmlonly
    <table border="0" cellspacing="5">
    <tr><td>1.0000</td>
        <td>0.0000</td>
        <td>1.4020</td>
    </tr>
    <tr><td>1.0000</td>
        <td>-0.3441</td>
        <td>-0.7141</td>
    </tr>
    <tr><td>1.0000</td>
        <td>1.7720</td>
        <td>-0.0001</td>
    </tr>
    </table>
\endhtmlonly
 *
 * \author David Poole
 * \date 26-July-05
 *
 **/

void
pie_csc_setup( void )
{
    int i;
    volatile uint32_t *ptr32;
    const uint32_t *coeff;
    const struct scanvars *sv;

    /* The caller should have called pie_reset() before calling us so make sure
     * all our our LUT's are zero to test pie_reset().
     *
     * If the [1] assertion fails, we're not resetting the LUT/matrix to zeroes
     * in pie_reset() or we're not calling pie_reset() before calling this
     * function.
     */

    /* 
     * Setup RGB2esRGB. 
     *
     * Has both 3x3 and LUT. 
     */

    sv = scanvar_get();
    /* set to block to defaults (bypass), enable CPU access to LUT */
    *PIE_CSC_RGB_CCR = PIE_CSC_RGB_CCR_R | PIE_CSC_RGB_CCR_CPU;

    /* copy coefficient matrix;
     * note: RGB 3x3 has 16-bit fields
     * RGB normalization factor is 2**14, so "1" = 16384
     * Negative values are expressed as a 2's complement hex number.
     */
    ptr32 = PIE_CSC_RGB_MUL;
    coeff = sv->csc_rgb_matrix;
    for( i=0 ; i<9 ; i++ ) {
        /* see above [1] */
        XASSERT( *ptr32==0, *ptr32 );
        *ptr32++ = *coeff++;
    }

    /* davep 29-Oct-2007 ; adding new LUT for each plane; note that we'll
     * link all three to the same LUT if we're sent a scanvar with all three in
     * one LUT
     *
     * Note also that enabling any single individual LUTs will override the RGB
     * combined LUT behavior.  (Newer features trump older features but older
     * features will behave as expected.)
     */
    if( sv->use_csc_rgb_lut && 
            !( sv->use_csc_r_lut || sv->use_csc_g_lut || sv->use_csc_b_lut) ) {
        /* davep 29-Oct-2007 ; use same RGB lut in all three tables */
        pie_dbg2( "%s using combined CSC RGB LUT\n", __FUNCTION__ );

        if( pie_csc_has_unified_rgb_luts() ) {
            /* running on something with unified CSC RGB LUT */
            pie_csc_rgb_set_lut( sv->csc_rgb_lut.data, PIE_CSC_RGB_LUT );
        }
        else {
            /* running on something with individual CSC RGB LUTs; set all three
             * of the LUTs to the same values
             */
            pie_csc_rgb_set_lut( sv->csc_rgb_lut.data, PIE_CSC_RED_LUT );
            pie_csc_rgb_set_lut( sv->csc_rgb_lut.data, PIE_CSC_GREEN_LUT );
            pie_csc_rgb_set_lut( sv->csc_rgb_lut.data, PIE_CSC_BLUE_LUT );
        }

    }
    else {
        pie_dbg2( "%s using individual CSC RGB LUTs\n", __FUNCTION__ );
        if( sv->use_csc_r_lut ) {
            pie_csc_rgb_set_lut( sv->csc_r_lut.data, PIE_CSC_RED_LUT );
        }
        if( sv->use_csc_g_lut ) {
            pie_csc_rgb_set_lut( sv->csc_g_lut.data, PIE_CSC_GREEN_LUT );
        }
        if( sv->use_csc_b_lut ) {
            pie_csc_rgb_set_lut( sv->csc_b_lut.data, PIE_CSC_BLUE_LUT );
        }
    }

    /*  clear bypass, disable CPU access */
    /* davep 20-Jul-2006 ; adding ability to bypass individual CSC blocks for
     * testing/debugging
     */
    if( !sv->csc_rgb_srgb_bypass ) {
        *PIE_CSC_RGB_CCR = 0;
    }

    /* 
     * Setup esRGB2YCrCb. 
     *
     * 3x3 matrix, no LUT.
     */

    /* set to defaults (bypass), no LUT so don't bother with CPU access */
    *PIE_CSC_ESR_CCR = PIE_CSC_ESR_CCR_R;

    /* copy coefficient matrix */
    ptr32 = PIE_CSC_ESR_MUL;
//    coeff = esrgb_to_ycc;
    coeff = sv->csc_rgbycc_matrix;
    for( i=0 ; i<9 ; i++ ) {
        /* see above [1] */
        XASSERT( *ptr32==0, *ptr32 );
        *ptr32++ = *coeff++;
    }

    /* clear bypass, clear CPU access and we're good to go */
    /* davep 20-Jul-2006 ; adding ability to bypass individual CSC blocks for
     * testing/debugging
     */
    if( !sv->csc_srgb_ycc_bypass ) {
        *PIE_CSC_ESR_CCR = 0;
    }

    /* 
     * Setup YCrCb2esRGB.
     *
     * 3x3 matrix, no LUT.
     */

    /* set to defaults (bypass), no LUT so don't bother with CPU access */
    *PIE_CSC_YCC_CCR = PIE_CSC_YCC_CCR_R;

    /* copy coefficient matrix */
    ptr32 = PIE_CSC_YCC_MUL;
//    coeff = ycc_to_esrgb;
    coeff = sv->csc_yccrgb_matrix;
    for( i=0 ; i<9 ; i++ ) {
        /* see above [1] */
        XASSERT( *ptr32==0, *ptr32 );
        *ptr32++ = *coeff++;
    }

    /* clear bypass, clear CPU access, clear the bypasses */
    /* davep 20-Jul-2006 ; adding ability to bypass individual CSC blocks for
     * testing/debugging
     */
    if( !sv->csc_ycc_rgb_bypass ) {
        *PIE_CSC_YCC_CCR = 0;
    }

//    dbg2("RGB_CCR=0x%x ESR_CCR=0x%x YCC_CCR=0x%x\n", 
//               *PIE_CSC_RGB_CCR, *PIE_CSC_ESR_CCR, *PIE_CSC_YCC_CCR );

    /* davep 03-Jul-2006 ; for debugging */
//    pie_csc_dump();
}

/**********************************************************
 *
 * CPA Functions
 *
 *********************************************************/

/**
 * \brief Reset Color Plane Adjust (CPA).
 *
 *  Resets CPA (Color Plane Adjust) lookup table and config register(s) back to
 *  power-on default values.
 *
 * \author David Poole
 * \date 25-July-05
 *
 **/
void 
pie_cpa_reset( void )
{
#ifdef HAVE_PIE_CPA
    pie_dbg2( "%s\n", __FUNCTION__ );

    /* set to Bypass mode and allow CPU access to LUT */
    *PIE_CPA_CR = PIE_CPA_CR_R | PIE_CPA_CR_CPU;

    /* restore LUT to power-on values */
    regblockset( PIE_CPA_LUT, PIE_CPA_LUT_SIZE, 0 );

    /* set cfg to power-on value (disables CPU access, enable Bypass) */
    *PIE_CPA_CR = PIE_CPA_CR_R;
#endif
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 19-Feb-2010
 *
 */

void pie_cpa_set_bypass( bool bypass )
{
#ifdef HAVE_PIE_CPA
    if( bypass ) {
        *PIE_CPA_CR |= PIE_CPA_CR_BYPASS; 
    }
    else {
        *PIE_CPA_CR &= ~PIE_CPA_CR_BYPASS; 
    }
#endif
}

/**
 * \brief Setup Color Plane Adjust block
 *
 * Initialize the CPA config register and lookup table. Prior to running pixels
 * through the block, firmware should program the CPA Configuration register
 * and CPA LUT using this function.
 *
 * \param[in] mode_bit shift or pull
 * \param[in] lut array of length \link #PIE_CPA_LUT_SIZE PIE_CPA_LUT_SIZE \endlink 
 *            of entries for the bit depth enhance lookup table
 *
 * \author David Poole
 * \date 26-July-05
 *
 **/
void
pie_cpa_setup( uint32_t mode_bit, uint32_t lut[] ) 
{
#ifdef HAVE_PIE_CPA
    int i;
    volatile uint32_t *ptr32;

    /* The caller should have called pie_reset() before calling us so make sure
     * all our our LUT's are zero to test pie_reset().
     *
     * If the [1] assertion fails, we're not resetting the LUT to zeroes in
     * pie_reset() or we're not calling pie_reset() before calling this
     * function.
     */

    pie_dbg2( "%s mode=%ld\n", __FUNCTION__, mode_bit );

    /* setup Color Plane Adjust block */

    /* put into bypass mode, enable CPU access */
    *PIE_CPA_CR = PIE_CPA_CR_R | PIE_CPA_CR_CPU;

    /* initialize LUT to same value (brute force and ignorance) */
    ptr32 = PIE_CPA_LUT;
    for( i=0 ; i<PIE_CPA_LUT_SIZE ; i++ ) {
        /* see above [1] */
        XASSERT( *ptr32==0, *ptr32 );

        *ptr32++ = lut[i];
    }

    XASSERT( mode_bit==PIE_CPA_CR_SHIFT || mode_bit==PIE_CPA_CR_PULL, mode_bit);

    /* enable (clear bypass), disable CPU access, set shift/pull method */
    *PIE_CPA_CR = mode_bit;

    pie_dbg2( "%s end PIE_CPA_CR=0x%x\n", __FUNCTION__, *PIE_CPA_CR );
#endif
}

/**********************************************************
 *
 * TCNS Functions
 *
 *********************************************************/

/**
 * \brief Reset Tone Control & Neutral Sensitivity block
 *
 *  Resets TCNS (Tone Contrast/Neutral Sensitivity) lookup table and config
 *  register(s) back to power-on default values.
 *
 * \author David Poole
 * \date 4-May-05
 *
 **/
void 
pie_tcns_reset( void )
{
    pie_dbg2( "%s\n", __FUNCTION__ );

    /* set to Bypass mode and allow CPU access to LUT */
    *PIE_TCNS_CR = PIE_TCNS_CR_R | PIE_TCNS_CR_CPU;

    /* restore LUT to power-on values */
    regblockset( PIE_TCNS_LUT, PIE_TCNS_LUT_SIZE, 0 );

    /* set cfg to power-on value (disables CPU access, enable Bypass) */
    *PIE_TCNS_CR = PIE_TCNS_CR_R;
}

void
pie_tcns_dump( void )
{
    int i;
    volatile uint32_t *ptr32;
    uint32_t num32;

    dbg2( "TCNS_CR=0x%x\n", *PIE_TCNS_CR );

    dbg2("PIE TCNS printed MSB to LSB\n" );
    pie_tcns_enable_fw_access( true );

    ptr32 = PIE_TCNS_LUT;
    for( i=0 ; i<PIE_TCNS_LUT_SIZE ; i++ ) {
        num32 = *ptr32++;
        dbg2( "%5d: %-3d %-3d %-3d", i, (num32>>22)&0xff,
                    (num32>>11)&0xff, (num32&0xff) );
        if( (i+1)%4==0 ) {
            dbg2("\n");
        }
    }

    pie_tcns_enable_fw_access( false );
}

void
pie_tcns_enable_fw_access( bool access ) 
{
    if( access ) {
        *PIE_TCNS_CR |= PIE_TCNS_CR_CPU;
    }
    else {
        *PIE_TCNS_CR &= ~PIE_TCNS_CR_CPU;
    }
}

void
pie_tcns_set_bypass( bool bypass ) 
{
    if( bypass ) {
        *PIE_TCNS_CR |= PIE_TCNS_CR_BYPASS;
    }
    else {
        *PIE_TCNS_CR &= ~PIE_TCNS_CR_BYPASS;
    }
}

void
pie_tcns_set_lut( uint32_t *lut )
{
    volatile uint32_t *ptr32;
    int i;

    ptr32 = PIE_TCNS_LUT;

    /* initialize LUT */
    for( i=0 ; i<PIE_TCNS_LUT_SIZE ; i++ ) {
        *ptr32++ = *lut++;
    }
}

/**********************************************************
 *
 * MultiFilter Functions
 *
 *********************************************************/

/**
 * \brief Reset the MultiFilter/Sculptor block
 *
 *  Restores the MultiFilter Sculptor subblock to power-on values including the
 *  Activity Index lookup table.
 *
 * \author David Poole
 * \date 9-May-05
 *
 **/

void pie_filter_reset( void )
{
    pie_dbg2( "%s\n", __FUNCTION__ );

#if 0
    /* set both modes to Bypass and allow CPU access to LUT */
    *PIE_FILTER_SCR2 = PIE_FILTER_SCR2_R | PIE_FILTER_SCR2_CPU;

    /* restore LUT to power-on values */
    regblockset( PIE_FILTER_LUT, PIE_FILTER_LUT_SIZE, 0 );

    /* set cfg to power-on values (disables CPU access, enable Bypass all) */
    *PIE_FILTER_SCR2 = PIE_FILTER_SCR2_R;
    *PIE_FILTER_SCR1 = PIE_FILTER_SCR1_R;

    /* restore text enhance registers to power-on values */
    *PIE_FILTER_TEL1R = PIE_FILTER_TEL1R_R;
    *PIE_FILTER_TEL2R = PIE_FILTER_TEL2R_R;
    *PIE_FILTER_TEL3R = PIE_FILTER_TEL3R_R;
#else
    sculptor_regs->SCR1 = PIE_FILTER_SCR1_R;
    sculptor_regs->SCR2 = PIE_FILTER_SCR2_R;
    sculptor_regs->TEL1R = PIE_FILTER_TEL1R_R;
    sculptor_regs->TEL2R = PIE_FILTER_TEL2R_R;
    sculptor_regs->TEL3R = PIE_FILTER_TEL3R_R;
    sculptor_regs->EC = 0;
    sculptor_regs->SF = 0;
#ifdef HAVE_PIE_SCULPTOR_5X5
    sculptor_regs->YVC00_01 = 0;
    sculptor_regs->YVC02_10 = 0;
    sculptor_regs->YVC11_12 = 0;
    sculptor_regs->YVC20_21 = 0;
#endif
#ifdef HAVE_PIE_SCULPTOR_5X9
    sculptor_regs->ECI = 0;
    sculptor_regs->YVC00_01 = 0;
    sculptor_regs->YVC02_03 = 0;
    sculptor_regs->YVC04_10 = 0;
    sculptor_regs->YVC11_12 = 0;
    sculptor_regs->YVC13_14 = 0;
    sculptor_regs->YVC20_21 = 0;
    sculptor_regs->YVC22_23 = 0;
    sculptor_regs->YVC24 = 0;

    sculptor_regs->CVC00_01 = 0;
    sculptor_regs->CVC02_03 = 0;
    sculptor_regs->CVC04_10 = 0;
    sculptor_regs->CVC11_12 = 0;
    sculptor_regs->CVC13_14 = 0;
    sculptor_regs->CVC20_21 = 0;
    sculptor_regs->CVC22_23 = 0;
    sculptor_regs->CVC24 = 0;
#endif

    /* restore AI LUT to power-on values */
    pie_filter_enable_fw_access( true );
    regblockset( sculptor_regs->AILR, PIE_FILTER_LUT_SIZE, 
                    sizeof(sculptor_regs->AILR)/sizeof(sculptor_regs->AILR[0]) );
    pie_filter_enable_fw_access( false );

#endif
}

/**
 * \brief Dump the MultiFilter control registers 
 *
 *
 * \author David Poole
 * \date 14-Oct-2005
 *
 */

void
pie_filter_dump( void )
{
    dbg2( "PIE_FILTER_SCR1=0x%x PIE_FILTER_SCR2=0x%x\n", 
                *PIE_FILTER_SCR1, *PIE_FILTER_SCR2 );

    dbg2( "PIE_FILTER_TEL1R=0x%x PIE_FILTER_TEL2R=0x%x PIE_FILTER_TEL3R=0x%x\n",
                *PIE_FILTER_TEL1R, *PIE_FILTER_TEL2R, *PIE_FILTER_TEL3R );
}

/**
 * \brief Dump the MultiFilter Activity Index LUT 
 *
 *
 * \author David Poole
 * \date 14-Oct-2005
 * 
 * Unconditionally enables then disables CPU access to the LUT. CPU access
 * will be <em>disabled</em> after calling this function.
 *
 * Do not EVER call this function while DMA is active!  
 *
 */

void
pie_filter_dump_lut( void )
{
    int i;
    volatile uint32_t *ptr32;

    /* enable CPU access so we can see it (don't do this while DMA is running! */
    pie_filter_enable_fw_access( true );

    ptr32 = PIE_FILTER_LUT;
    for( i=0 ; i<PIE_FILTER_LUT_SIZE ; i+=4 ) {
        dbg2( "0x%x 0x%x 0x%x 0x%x\n", ptr32[i], ptr32[i+1], 
                    ptr32[i+2], ptr32[i+3] );
    }

    /* now shut it off again so we can't damage anything */
    pie_filter_enable_fw_access( false );
}

/**
 * \brief Enable/Disable bypass for all of the MultiFilter block 
 *
 *
 * \author David Poole
 * \date 14-Oct-2005
 *
 */

void
pie_filter_set_bypass( bool bypass )
{
    if( bypass ) {
        *PIE_FILTER_SCR2 |= PIE_FILTER_SCR2_BYPASS;
    }
    else {
        *PIE_FILTER_SCR2 &= ~PIE_FILTER_SCR2_BYPASS;
    }
}

/**
 * \brief Enable/Disable MultiFilter's Text Enhance bypass mode  
 *
 *
 * \author David Poole
 * \date 14-Oct-2005
 *
 */

void
pie_filter_set_bypass_te( bool bypass )
{
    if( bypass ) {
        *PIE_FILTER_SCR2 |= PIE_FILTER_SCR2_TE_BYPASS;
    }
    else {
        *PIE_FILTER_SCR2 &= ~PIE_FILTER_SCR2_TE_BYPASS;
    }

}

/**
 * \brief Enable/Disable access to the MultiFilter Activity Index LUT 
 *
 *
 * \author David Poole
 * \date 14-Oct-2005
 *
 */

void 
pie_filter_enable_fw_access( bool enable )
{
    if( enable ) {
        *PIE_FILTER_SCR2 |= PIE_FILTER_SCR2_CPU;
    }
    else {
        *PIE_FILTER_SCR2 &= ~PIE_FILTER_SCR2_CPU;
    }
}

/**
 * \brief Put MultiFilter into either Mono or Color mode 
 *
 *
 * \author David Poole
 * \date 14-Oct-2005
 *
 */

void
pie_filter_set_monocolor( pie_mf_color monocolor )
{
    if( monocolor==PIE_MF_MONO ) {
        *PIE_FILTER_SCR2 |= PIE_FILTER_SCR2_MONO; 
    }
    else {
        /* 0 == color mode */
        *PIE_FILTER_SCR2 &= ~PIE_FILTER_SCR2_MONO; 
    }
}

/**
 * \brief Set the MultiFilter Text Enhance search radius matrix 
 *
 *
 * \author David Poole
 * \date 14-Oct-2005
 *
 */

void 
pie_filter_set_te_matrix( pie_mf_te_matrix te_matrix )
{
    if( te_matrix==PIE_MF_TE_MATRIX_5x5 ) {
        /* 2 == use 5x5 matrix, so set the bit */
        *PIE_FILTER_SCR2 |= PIE_FILTER_SCR2_TE_MATRIX; 
    }
    else {
        /* 1 == use 3x3 matrix, so clear the bit */
        *PIE_FILTER_SCR2 &= ~PIE_FILTER_SCR2_TE_MATRIX; 
    }
}

/**
 * \brief Fill the MultiFilter Activity Index LUT 
 *
 * \author David Poole
 * \date 14-Oct-2005
 *
 */

void
pie_filter_set_lut( uint32_t *lut )
{
    int i;
    volatile uint32_t *ptr32;

    ptr32 = PIE_FILTER_LUT;
    for( i=0 ; i<PIE_FILTER_LUT_SIZE ; i++ ) {
        *ptr32++ = *lut++;
    }

}

/**
 * \brief setup PIE MultiFilter 
 *
 *
 * \author David Poole
 * \date 19-Sep-2005
 *
 */

void pie_te_setup(void)
{
    const struct scanvars *sv = scanvar_get();

    *PIE_FILTER_TEL1R =
        PIE_FILTER_TEL1R_CNTB(sv->te_count_black_min) | // count black min. 0-31
        PIE_FILTER_TEL1R_CNTW(sv->te_count_white_min) | // count white min. 0-31
        PIE_FILTER_TEL1R_CNTBW(sv->te_count_white_plus_black_min) | // count black+white min. 0-63
        PIE_FILTER_TEL1R_CNTC(sv->te_center_color_max); // center color max. 0-127

    *PIE_FILTER_TEL2R =
        PIE_FILTER_TEL2R_CMAX(sv->te_intense_color_max); // intense color max. 0-127

    *PIE_FILTER_TEL3R =
        PIE_FILTER_TEL3R_BYMAX(sv->te_black_y_max) | // black Y max. 0-127
        PIE_FILTER_TEL3R_BCrCbMAX(sv->te_black_CbCr_max) | // black CbCr max. 0-127
        PIE_FILTER_TEL3R_WYMIN(sv->te_white_y_min) | // white Y min. 0-127
        PIE_FILTER_TEL3R_WCrCbMAX(sv->te_white_CbCr_max); // white CbCr max. 0-127

    if( sv->te_use_5x5_matrix ) {
        /* 1 == use 5x5 */
        *PIE_FILTER_SCR2 |= PIE_FILTER_SCR2_TE_MATRIX;  
    }
    else {
        /* 0 == use 3x3 */
        *PIE_FILTER_SCR2 &= ~PIE_FILTER_SCR2_TE_MATRIX;  
    }


    /* enable text enhance */
    pie_filter_set_bypass_te( false );
}

/* davep 01-Apr-2013 ; sanity check the coefficients. Each should be 17
 * bits max
 */
#define SCULPTOR_SANITY \
    do{\
        for( i=0 ; i<MF_COEF_MAX_COEFFICIENTS ; i++ ) {\
            XASSERT( !(sv->mf_varcoef_y[i] & ~PIE_SCULPTOR_VARCOEF_MASK), sv->mf_varcoef_y[i] ); \
            XASSERT( !(sv->mf_varcoef_chroma[i] & ~PIE_SCULPTOR_VARCOEF_MASK), sv->mf_varcoef_chroma[i] ); \
        }\
    } while(0);

static void pie_varcoef_setup(void)
{
#ifdef HAVE_PIE_SCULPTOR_5X5
    const struct scanvars *sv = scanvar_get();
    uint32_t reg_val;
    int i;

    SCULPTOR_SANITY;

    reg_val = sculptor_regs->YVC00_01;
    reg_val = SCULPTOR_YVC00_01_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[2]);
    reg_val = SCULPTOR_YVC00_01_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[3]);
    sculptor_regs->YVC00_01 = reg_val;
    reg_val = sculptor_regs->YVC02_10;
    reg_val = SCULPTOR_YVC02_10_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[4]);
    reg_val = SCULPTOR_YVC02_10_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[7]);
    sculptor_regs->YVC02_10 = reg_val;
    reg_val = sculptor_regs->YVC11_12;
    reg_val = SCULPTOR_YVC11_12_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[8]);
    reg_val = SCULPTOR_YVC11_12_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[9]);
    sculptor_regs->YVC11_12 = reg_val;
    reg_val = sculptor_regs->YVC20_21;
    reg_val = SCULPTOR_YVC20_21_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[12]);
    reg_val = SCULPTOR_YVC20_21_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[13]);
    sculptor_regs->YVC20_21 = reg_val;
    reg_val = sculptor_regs->YVC22;
    reg_val = SCULPTOR_YVC22_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[14]);
    reg_val = SCULPTOR_YVC22_S_REPLACE_VAL(reg_val, sv->mf_varcoef_y_shift);
    sculptor_regs->YVC22 = reg_val;

    /* shift factor */
    reg_val = sculptor_regs->SF;
    reg_val = SCULPTOR_SF_Y_REPLACE_VAL(reg_val, sv->mf_varcoef_y_sf);
    reg_val = SCULPTOR_SF_C_REPLACE_VAL(reg_val, sv->mf_varcoef_chroma_sf);
    sculptor_regs->SF= reg_val;

    reg_val = sculptor_regs->CVC00_01;
    reg_val = SCULPTOR_CVC00_01_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[2]);
    reg_val = SCULPTOR_CVC00_01_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[3]);
    sculptor_regs->CVC00_01 = reg_val;
    reg_val = sculptor_regs->CVC02_10;
    reg_val = SCULPTOR_CVC02_10_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[4]);
    reg_val = SCULPTOR_CVC02_10_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[7]);
    sculptor_regs->CVC02_10 = reg_val;
    reg_val = sculptor_regs->CVC11_12;
    reg_val = SCULPTOR_CVC11_12_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[8]);
    reg_val = SCULPTOR_CVC11_12_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[9]);
    sculptor_regs->CVC11_12 = reg_val;
    reg_val = sculptor_regs->CVC20_21;
    reg_val = SCULPTOR_CVC20_21_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[12]);
    reg_val = SCULPTOR_CVC20_21_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[13]);
    sculptor_regs->CVC20_21 = reg_val;
    reg_val = sculptor_regs->CVC22;
    reg_val = SCULPTOR_CVC22_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[14]);
    reg_val = SCULPTOR_CVC22_S_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma_shift);
    sculptor_regs->CVC22 = reg_val;
#endif
#ifdef HAVE_PIE_SCULPTOR_5X9
    const struct scanvars *sv = scanvar_get();
    uint32_t reg_val;
    int i;

    SCULPTOR_SANITY;

    reg_val = sculptor_regs->YVC00_01;
    reg_val = SCULPTOR_YVC00_01_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[0]);
    reg_val = SCULPTOR_YVC00_01_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[1]);
    sculptor_regs->YVC00_01 = reg_val;
    reg_val = sculptor_regs->YVC02_03;
    reg_val = SCULPTOR_YVC02_03_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[2]);
    reg_val = SCULPTOR_YVC02_03_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[3]);
    sculptor_regs->YVC02_03 = reg_val;
    reg_val = sculptor_regs->YVC04_10;
    reg_val = SCULPTOR_YVC04_10_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[4]);
    reg_val = SCULPTOR_YVC04_10_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[5]);
    sculptor_regs->YVC04_10 = reg_val;
    reg_val = sculptor_regs->YVC11_12;
    reg_val = SCULPTOR_YVC11_12_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[6]);
    reg_val = SCULPTOR_YVC11_12_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[7]);
    sculptor_regs->YVC11_12 = reg_val;
    reg_val = sculptor_regs->YVC13_14;
    reg_val = SCULPTOR_YVC13_14_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[8]);
    reg_val = SCULPTOR_YVC13_14_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[9]);
    sculptor_regs->YVC13_14 = reg_val;
    reg_val = sculptor_regs->YVC20_21;
    reg_val = SCULPTOR_YVC20_21_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[10]);
    reg_val = SCULPTOR_YVC20_21_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[11]);
    sculptor_regs->YVC20_21 = reg_val;
    reg_val = sculptor_regs->YVC22_23;
    reg_val = SCULPTOR_YVC22_23_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[12]);
    reg_val = SCULPTOR_YVC22_23_B_REPLACE_VAL(reg_val, sv->mf_varcoef_y[13]);
    sculptor_regs->YVC22_23 = reg_val;
    reg_val = sculptor_regs->YVC24;
    reg_val = SCULPTOR_YVC24_A_REPLACE_VAL(reg_val, sv->mf_varcoef_y[14]);
    reg_val = SCULPTOR_YVC24_S_REPLACE_VAL(reg_val, sv->mf_varcoef_y_shift);
    sculptor_regs->YVC24 = reg_val;

    /* shift factor */
    reg_val = sculptor_regs->SF;
    reg_val = SCULPTOR_SF_Y_REPLACE_VAL(reg_val, sv->mf_varcoef_y_sf);
    reg_val = SCULPTOR_SF_C_REPLACE_VAL(reg_val, sv->mf_varcoef_chroma_sf);
    sculptor_regs->SF= reg_val;

    reg_val = sculptor_regs->CVC00_01;
    reg_val = SCULPTOR_CVC00_01_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[0]);
    reg_val = SCULPTOR_CVC00_01_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[1]);
    sculptor_regs->CVC00_01 = reg_val;
    reg_val = sculptor_regs->CVC02_03;
    reg_val = SCULPTOR_CVC02_03_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[2]);
    reg_val = SCULPTOR_CVC02_03_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[3]);
    sculptor_regs->CVC02_03 = reg_val;
    reg_val = sculptor_regs->CVC04_10;
    reg_val = SCULPTOR_CVC04_10_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[4]);
    reg_val = SCULPTOR_CVC04_10_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[5]);
    sculptor_regs->CVC04_10 = reg_val;
    reg_val = sculptor_regs->CVC11_12;
    reg_val = SCULPTOR_CVC11_12_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[6]);
    reg_val = SCULPTOR_CVC11_12_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[7]);
    sculptor_regs->CVC11_12 = reg_val;
    reg_val = sculptor_regs->CVC13_14;
    reg_val = SCULPTOR_CVC13_14_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[8]);
    reg_val = SCULPTOR_CVC13_14_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[9]);
    sculptor_regs->CVC13_14 = reg_val;
    reg_val = sculptor_regs->CVC20_21;
    reg_val = SCULPTOR_CVC20_21_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[10]);
    reg_val = SCULPTOR_CVC20_21_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[11]);
    sculptor_regs->CVC20_21 = reg_val;
    reg_val = sculptor_regs->CVC22_23;
    reg_val = SCULPTOR_CVC22_23_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[12]);
    reg_val = SCULPTOR_CVC22_23_B_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[13]);
    sculptor_regs->CVC22_23 = reg_val;
    reg_val = sculptor_regs->CVC24;
    reg_val = SCULPTOR_CVC24_A_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma[14]);
    reg_val = SCULPTOR_CVC24_S_REPLACE_VAL(reg_val,sv->mf_varcoef_chroma_shift);
    sculptor_regs->CVC24 = reg_val;
#endif
}

void pie_filter_setup( int stripsize, int pixels_per_row )
{
    const struct scanvars *sv;
    uint32_t reg_val;
    
    sv = scanvar_get();

    pie_dbg2( "%s ss=%d ppr=%d\n", __FUNCTION__, stripsize, pixels_per_row );

    pie_filter_set_pogo( stripsize, pixels_per_row );

    /* put into bypass and enable CPU access so we can program the Activity
     * lookup table
     */
    pie_filter_set_bypass( true );

    if( sv->use_mf_ai_lut ) {
        pie_filter_enable_fw_access( true );

        /* davep 03-Jul-2006 ; take activity lut from scanvars */
        pie_filter_set_lut( sv->mf_ai_lut.data );
    }

    /* disable CPU access but leave in bypass while we program the rest of the
     * registers
     */
    pie_filter_enable_fw_access( false );


    /* davep 05-Jul-2006 ; integrate multifilter into scanvars;  
     * assume scanvar's values are exactly the register settings 
     *
     * daj 19-Dec-2011: TODO: this should be done using piehw.h.
     *                  Use PIP-style setting for the multifilter settings, and
     *                  use new variable coefficients.  Important note is that
     *                  the mf_ values don't need to be pre-shifted an ASIC-
     *                  specific amount by setup code.
     *
     */
    reg_val = sculptor_regs->SCR1;
    reg_val = SCULPTOR_SCR1_T_REPLACE_VAL(reg_val, sv->mf_offset);
    reg_val = SCULPTOR_SCR1_N_REPLACE_VAL(reg_val, sv->mf_lut_output_scale);
    reg_val = SCULPTOR_SCR1_P_REPLACE_VAL(reg_val, sv->mf_scale_20);
    reg_val = SCULPTOR_SCR1_M_REPLACE_VAL(reg_val, sv->mf_scale_15);
    reg_val = SCULPTOR_SCR1_S_REPLACE_VAL(reg_val, sv->mf_scale_10);
    reg_val = SCULPTOR_SCR1_CRCBR_REPLACE_VAL(reg_val, sv->mf_cbcr_radius);
    reg_val = SCULPTOR_SCR1_Y_REPLACE_VAL(reg_val, sv->mf_y_radius);
    sculptor_regs->SCR1 = reg_val;

    reg_val = sculptor_regs->SCR2;
    reg_val = SCULPTOR_SCR2_R_REPLACE_VAL(reg_val, sv->use_mf_erosion);
    reg_val = SCULPTOR_SCR2_D_REPLACE_VAL(reg_val, sv->use_mf_dilation);
    reg_val = SCULPTOR_SCR2_V_REPLACE_VAL(reg_val, sv->use_mf_varcoefficients);
    sculptor_regs->SCR2 = reg_val;

    if( sv->use_mf_varcoefficients ) {
        pie_varcoef_setup();
    }

    if( sv->use_mf_te ) {
        pie_te_setup();
    }
    else {
        pie_filter_set_bypass_te(true);
    }
   
    /* pick a color; we have two choices */
    if( sv->cmode == SCAN_CMODE_MONO ) {
        pie_filter_set_monocolor( PIE_MF_MONO );
    }
    else {
        pie_filter_set_monocolor( PIE_MF_COLOR );
    }

    /* take out of bypass all */
    pie_filter_set_bypass( false );

//    pie_filter_dump();
//    pie_filter_dump_lut();
}

/**
 * \brief Set MultiFilter pogo align config register
 *
 * Set up the configuration register in the PIE MultiFilter Pogo Align config
 * register (PACR).
 *
 * \param   num_rows line count field of PACR
 * \param   pixels_per_row pixel count field of PACR
 *
 * \author David Poole
 * \date 23-May-05
 *
 **/

void pie_filter_set_pogo( int num_rows, int pixels_per_row )
{
    *PIE_ALIGN_PACR = PIE_ALIGN_PACR_LINES(num_rows)
                    | PIE_ALIGN_PACR_PIXELS(pixels_per_row)
                    ;
}

/**********************************************************
 *
 * XYScale Functions
 *
 *********************************************************/

/**
 * \brief Restore PIE XYScale block to power-on defaults. 
 *
 *
 * \author David Poole
 * \date 18-Apr-05
 *
 */

void
pie_xy_reset( void )
{
    /* 18-Apr-05 ; set registers back to power-on defaults */
    *PIE_SCALE_CFG = PIE_SCALE_CFG_R;
    *PIE_SCALE_INXY = PIE_SCALE_INXY_R;
    *PIE_SCALE_OUTX = PIE_SCALE_OUTX_R;
    *PIE_SCALE_FILLC1C2 = PIE_SCALE_FILLC1C2_R;
    *PIE_SCALE_FILLC0 = PIE_SCALE_FILLC0_R;
    *PIE_SCALE_SCALEX = PIE_SCALE_SCALEX_R;
    *PIE_SCALE_SCALEY = PIE_SCALE_SCALEY_R;
    *PIE_SCALE_SXMN = PIE_SCALE_SXMN_R;
    *PIE_SCALE_SYMN = PIE_SCALE_SYMN_R;
    *PIE_SCALE_TX = PIE_SCALE_TX_R;
    *PIE_SCALE_TY = PIE_SCALE_TY_R;
    *PIE_SCALE_INITYPER = PIE_SCALE_INITYPER_R;
    
    /* 22-Apr-05 ; force to Bypass mode */
    *PIE_SCALE_CFG |= PIE_SCALE_CFG_BYPASS;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 19-Feb-2010
 *
 */

void pie_xy_set_bypass( bool bypass )
{
    if( bypass ) {
        *PIE_SCALE_CFG |= PIE_SCALE_CFG_BYPASS;
    }
    else {
        *PIE_SCALE_CFG &= ~PIE_SCALE_CFG_BYPASS;
    }
}

/**
 * \brief Dump all XYScale registers.
 *
 * Debug printfs all PIE XYScale's registers.
 *
 * \author David Poole
 * \date 21-Apr-05
 *
 *
 **/
void pie_xy_dump( void )
{
    dbg2( "CFG=0x%x ", *PIE_SCALE_CFG );
    dbg2( "INXY=0x%x ", *PIE_SCALE_INXY );
    dbg2( "OUTX=0x%x ", *PIE_SCALE_OUTX );
    dbg2( "FILLC1C2=0x%x ", *PIE_SCALE_FILLC1C2 );
    dbg2( "FILLC0=0x%x ", *PIE_SCALE_FILLC0 );
    dbg2( "SCALEX=0x%x ", *PIE_SCALE_SCALEX );
    dbg2( "SCALEY=0x%x ", *PIE_SCALE_SCALEY );
    dbg2( "SXMN=0x%x ", *PIE_SCALE_SXMN );
    dbg2( "SYMN=0x%x ", *PIE_SCALE_SYMN );
    dbg2( "TX=0x%x ", *PIE_SCALE_TX );
    dbg2( "TY=0x%x ", *PIE_SCALE_TY );
    dbg2( "CURRDX=0x%x ", *PIE_SCALE_CURRDX );
    dbg2( "CURRDY=0x%x ", *PIE_SCALE_CURRDY );
    dbg2( "INITYPER=0x%x ", *PIE_SCALE_INITYPER );
    dbg2( "CURRXPER=0x%x ", *PIE_SCALE_CURRXPER );
    dbg2( "CURRYPER=0x%x ", *PIE_SCALE_CURRYPER );
    dbg2( "OBS=0x%x\n", *PIE_SCALE_OBS );
}


/**
 * \brief  ceil() without float
 *
 *  ceil(3) for a 16-bit value that is <<16 into a 32-bit value in order to
 *  make accurate division with only integers. 
 *
 *      uint32_t a,b,c;
 *
 *      # quotient = int(ceil(float(a)/float(b)b));
 *      a = numer<<16;
 *      b = a / denom;
 *      c = f16_ceil();
 *      quotient = c;
 *
 * \author David Poole
 * \date 02-Aug-2013
 */

static uint32_t f16_ceil( uint32_t n )
{
    /* ceil() without float.
     *
     *
     */
    if( n & 0xffff ) {
        //# if there is a fractional part
        //# add 1 to base; clear fractional part
        return (n+(1<<16)) & (~0xffff);
    }
    return n;
}

/**
 * \brief  ceil() without float
 *
 * see f16_ceil() for more detail
 *
 * \author David Poole
 * \date 02-Aug-2013
 */

static uint64_t f32_ceil( uint64_t n) 
{
    uint64_t a,b;

    if( n & (uint64_t)0xffffffff ) {
        //# there is a fractional part
        //# add 1 to base; clear fractional part
        a = (n+(1ull<<32));
        b = a & (~0xffffffffull);
        return (n+(1ull<<32)) & (~0xffffffffull);
    }
    return n;
}


/**
 * \brief  Calculate edge pixels for linear mode scaling.
 *
 * Die floating point, die!
 *
 * \author David Poole
 * \date 01-Aug-2013
 */

uint32_t pie_xy_calc_linear_mode_edge_pixels( uint32_t numerator, 
                                              uint32_t denominator )
{
    uint32_t edge_pixels;
    uint32_t a,b,c,d;

//    dbg2( "%s numer=%d denom=%d\n", __FUNCTION__, numerator, denominator );

    XASSERT(numerator>=denominator,numerator);

    /* from the MA:
     *      edge_pixels = ceiling( (scale-1)/2 ) 
     *
     * former floating point code 
     *    edge_pixels = (int)ceil( ((float)numerator/(float)denominator - 1) / 2.0);
     *
     * Numerator and denominator are a fraction used to determine scale. The
     * numer, denom are limited to 15-bit numbers so we can use 32-bit number
     * for our integer division.
     */

    a = (numerator<<16) / (denominator);
    b = a - (1<<16);
    c = b / (2);
    d = f16_ceil(c);
    edge_pixels = d >> 16;

    XASSERT( edge_pixels>=0 && edge_pixels<=15, edge_pixels );

    return edge_pixels;
}

/**
 * \brief Calculate Linear mode strip size
 *
 *  The PIE XYScale documentation uses floating point division for some of
 *  these calculations. However, Linux kernel has no float. Using a 64-bit
 *  integer to do the division.
 *
 * \author David Poole
 * \date 02-Aug-2013
 */

static uint32_t pie_xy_calc_linear_mode_strip_size( uint32_t strip_size_in, uint32_t y_scale )
{
    uint64_t a,b,c,d,e;
    uint64_t numer64, denom64;
    uint32_t strip_size_out;

    /* original floating point math 
     *  strip_size_out = (int)ceil( (strip_size_in * (1<<16)) / (float)fracY);
     */

    /* strip_size_in will almost always be in [1,22]
     * y_scale is a 16-bit number [1,65535]
     */

    /* create a 64-bit integer so get the most accurate result from the integer
     * division
     */
    a = (uint64_t)strip_size_in << 32;
    b = a * (1<<16);
    numer64 = b;
    denom64 = (uint64_t)y_scale;
    c = safeint_divide_uint64( numer64, denom64 );
    d = f32_ceil(c);
    e = d >> 32;
    strip_size_out = (uint32_t)e;

    return strip_size_out;
}

/**
 * \brief  Initialize the XY Scaler 
 *
 * \param[in] scale     
 *      the x,y scale factors as a fraction
 *
 * \param[in] pixels_in 
 *      how many pixels per row 
 *
 * \param[in] strip_size_in
 *      the number of rows acted upon in a single pass; programmed into the
 *      hardware
 *
 * \param[in] total_rows_in
 *      the complete size of the input image
 *
 * \param[in] extra_pad
 *      how many extra pixels are added to the end of each row
 *
 * \param[in] scaler_mode PIE_XY_SCALER_LINEAR or PIE_XY_SCALER_RAPR
 *
 * \param[out] fracY
 *      the calculated Y scaler value, normalized to 16-bit
 *  
 * \param[out] pixels_out
 *      the number of pixels per row output, DMA aligned.
 *
 * \param[out] strip_size_out
 *      the Maximum strip size the hardware will output; <strong>Note
 *      actual strip size can change across strips!</strong>
 *
 * \param[out] total_rows_out
 *      The total number of rows output from the scaler. The actual output from
 *      the hardware will be padded to an exact strip size because of the DMA
 *      requirements but the driver will remove the extra padding on the
 *      output.
 *
 * \author David Poole
 * \date 29-Aug-2005
 *
 */

void
pie_xy_scale(  struct pie_scale *scale,
               int pixels_in,
               int strip_size_in,
               int total_rows_in,
               int extra_pad,
               int scaler_mode,

               int *pixels_out,
               int strip_size_out,
               int *total_rows_out )
{
    int x_edge_pixels, y_edge_pixels; 
    uint32_t scale_tx, scale_ty;

    /* Set up XYScale.  */
    dbg2( "%s ssi=%d sso=%d x=%d/%d y=%d/%d mode=%s\n", __FUNCTION__, 
                strip_size_in, strip_size_out,
                scale->x_numerator, scale->x_denominator, 
                scale->y_numerator, scale->y_denominator,
                scaler_mode==PIE_XY_SCALER_LINEAR?"linear":"rapr" );

    /* davep 07-Oct-2013 ; Linear mode HW only works down to 1/2. If we're
     * scaling down, we force RAPR. If we're scaling up, we usually want Linear
     * but RAPR also works. Verify to avoid confusing math errors deeper in
     * the PIE setup code.
     */
    if( scale->x_numerator < scale->x_denominator ) {
        XASSERT( scaler_mode==PIE_XY_SCALER_RAPR, scaler_mode );
    }
    if( scale->y_numerator < scale->y_denominator ) {
        XASSERT( scaler_mode==PIE_XY_SCALER_RAPR, scaler_mode );
    }

    /* davep 13-Sep-2005 ; require the input pixels to be DMA aligned;
     * XXX I may want to remove this later
     */
    /* davep 03-Mar-2008 ; removing it since incoming pixels != incoming bytes */
//    XASSERT( pixels_in%16 == 0, pixels_in );

    /* First, put into Bypass mode before fiddling the registers (the data
     * sheet says so).
     */ 
    *PIE_SCALE_CFG |= PIE_SCALE_CFG_BYPASS; 

    /* set the number of rows per buffer and the number of pixels per row */
    *PIE_SCALE_INXY = PIE_SCALE_INXY_CC( pixels_in )
                    | PIE_SCALE_INXY_LC( strip_size_in )
                    ;

//    /* arbitrary scale */
//    if( scaler_mode==PIE_XY_SCALER_LINEAR ) {
//        x_scale = ((1<<16) * scale->x_denominator) / scale->x_numerator;
//        y_scale = ((1<<16) * scale->y_denominator) / scale->y_numerator;
//    }
//    else {
//        XASSERT( scaler_mode==PIE_XY_SCALER_RAPR, scaler_mode );
//        x_scale = ((1<<16) * scale->x_numerator) / scale->x_denominator;
//        y_scale = ((1<<16) * scale->y_numerator) / scale->y_denominator;
//    }

    XASSERT( scale->x_scale && PIE_SCALE_SCALEX_SCALE(scale->x_scale)==scale->x_scale, scale->x_scale );
    XASSERT( scale->y_scale && PIE_SCALE_SCALEY_SCALE(scale->y_scale)==scale->y_scale, scale->y_scale );

    *PIE_SCALE_SCALEX = PIE_SCALE_SCALEX_SCALE(scale->x_scale);
    *PIE_SCALE_SCALEY = PIE_SCALE_SCALEY_SCALE(scale->y_scale);

    /* how many pixels are we going to get out? */
    pie_xy_calc_expected_pixels( pixels_in, scale->x_scale, scaler_mode, pixels_out );
    
    /* add in any extra pad bytes we might need (e.g., for HalfPack) */
    *pixels_out += extra_pad;

    /* make sure each output row is quadword DMA aligned */
//    *pixels_out = ((*pixels_out+15) / 16) * 16;
    *pixels_out = ICE_DMA_ALIGN_ME(*pixels_out);
    
    *PIE_SCALE_OUTX = PIE_SCALE_OUTX_CC( *pixels_out );

    /* make sure each output row is quadword DMA aligned */
//    XASSERT( *PIE_SCALE_OUTX % 16 == 0, *PIE_SCALE_OUTX );

    /* 21-June-05 ; note total_rows won't be used in the MultiStrip driver
     * because the output row count depends on XYScale settings.
     */
    pie_xy_calc_expected_rows( total_rows_in, scale->y_scale, scaler_mode, total_rows_out );

    /* 16-June-05 ; have to adjust the write strip size even though PIE write
     * DMA will get the rows from the XYScale block. We need this number to
     * correctly allocate memory for write buffers. This number needs to be the
     * largest the write strip size will ever be.
     */
    /* davep 14-Oct-2005 ; I'm changing the rules. Originally I wanted to have
     * as many of these silly numbers calculated in one place (here, in the
     * xyscale setup function) but found I needed too many of them in order to
     * set up HalfPack's numbers. Then I wound up calculating them in two
     * places--before xyscale was set up and here in xyscale setup. I wound up
     * ignoring the second set of numbers. Now, however, the two sets of
     * numbers are divering because of MultiFilter. I don't want this function
     * to know about MultiFilter so I'm no longer going to allow it to change
     * any of the settings. From now on we'll have to calculate all this crap
     * before we call xyscale setup.
     *
     * I'm validating everything to within an inch of its life so don't get too
     * cocky.
     */
//    *strip_size_out = (strip_size_in * y_scale) / (1<<16);
    if( scaler_mode==PIE_XY_SCALER_LINEAR ) {
         XASSERT( strip_size_out==pie_xy_calc_linear_mode_strip_size(strip_size_in,scale->y_scale), 
                     strip_size_out );
    }
    else {
        XASSERT( scaler_mode==PIE_XY_SCALER_RAPR, scaler_mode );
        XASSERT( strip_size_out==(strip_size_in * scale->y_scale) / (1<<16), strip_size_out );
    }

    /* 6-May-05 ; set fill data to something other than zero so it shows up */
#if 0
    *PIE_SCALE_FILLC0 = PIE_SCALE_FILLC0_F0(0xbb);
    *PIE_SCALE_FILLC1C2 = PIE_SCALE_FILLC1C2_F1(0xbb)
                        | PIE_SCALE_FILLC1C2_F2(0xbb)
                        ;
//    *PIE_SCALE_FILLC0 = PIE_SCALE_FILLC0_F0(0xbb);
//    *PIE_SCALE_FILLC1C2 = PIE_SCALE_FILLC1C2_F1(0xcc)
//                        | PIE_SCALE_FILLC1C2_F2(0xdd)
//                        ;
#else
    /* 0xff is black for laser, white for RGB color scans, and (I think) white for
     * ink copies
     */
    *PIE_SCALE_FILLC0 = PIE_SCALE_FILLC0_F0(0xff);
    *PIE_SCALE_FILLC1C2 = PIE_SCALE_FILLC1C2_F1(0xff)
                        | PIE_SCALE_FILLC1C2_F2(0xff)
                        ;

//    /* black */
//    *PIE_SCALE_FILLC0 = PIE_SCALE_FILLC0_F0(0x00);
//    *PIE_SCALE_FILLC1C2 = PIE_SCALE_FILLC1C2_F1(0x00)
//                        | PIE_SCALE_FILLC1C2_F2(0x00)
//                        ;
#endif

    /* 26-May-05 davep ; set to full roundup */
    /* davep 23-Jun-2006 ; XXX I did this back when I was shotgun debugging
     * scaler problems. Will this break Linear?
     */
    *PIE_SCALE_CFG |= PIE_SCALE_CFG_ROUNDUP( PIE_SCALE_RUP_ALL );

    /* davep 15-Jun-2006 ; turn on linear mode and hope for the best... */
    if( scaler_mode==PIE_XY_SCALER_LINEAR ) {
//        dbg1("%s enabling XYScale linear mode\n", __FUNCTION__ );
        *PIE_SCALE_CFG |= PIE_SCALE_CFG_LM_MODE;

        /* four other registers need configuration in Linear mode plus we need to
         * set up the Edge Pixels field in the SCALEX register
         */

        *PIE_SCALE_SXMN = PIE_SCALE_SCALEX_SCALE( scale->x_scale ) - 65536;
        *PIE_SCALE_SYMN = PIE_SCALE_SCALEY_SCALE( scale->y_scale ) - 65536;
//        dbg2("SXMN=0x%x SYMN=0x%x\n", *PIE_SCALE_SXMN, *PIE_SCALE_SYMN );

        /* edge = ceiling( (scale-1)/2 ) */
        x_edge_pixels = pie_xy_calc_linear_mode_edge_pixels( scale->x_numerator,
                                                      scale->x_denominator );
        XASSERT( x_edge_pixels>=0 && x_edge_pixels<=15, x_edge_pixels );

        *PIE_SCALE_SCALEX |= PIE_SCALE_SCALEX_EDGE(x_edge_pixels);
//        dbg2( "x_edge_pixels=%d x_scale=%d\n", x_edge_pixels, x_scale );

        y_edge_pixels = pie_xy_calc_linear_mode_edge_pixels( scale->y_numerator,
                                                      scale->y_denominator );        
//        dbg2( "y_edge_pixels=%d y_scale=%d\n", y_edge_pixels, y_scale );

        /* Do I know what this formula means? No, I do not. It's straight from the
         * XYScale MA.
         *
         * Use temporaries so I can debug print the results because these two
         * registers are write only.
         */
        scale_tx = x_edge_pixels * scale->x_scale - 32768 + scale->x_scale/2;
        scale_ty = y_edge_pixels * scale->y_scale - 32768 + scale->y_scale/2;
//        dbg1("scale_tx=0x%x scale_ty=0x%x\n", scale_tx, scale_ty );
        *PIE_SCALE_TX = scale_tx;
        *PIE_SCALE_TY = scale_ty;
    }
    else {

        /* Disable LM mode so we're now set to RAPR (Running Average Pixel
         * Replication) mode 
         */
        *PIE_SCALE_CFG &= ~PIE_SCALE_CFG_LM_MODE;

        /* so we don't have a warning about unused function variables */
        x_edge_pixels = 0;
        y_edge_pixels = 0;
        scale_tx = 0;
        scale_ty = 0;
    }

    /* Now that we're all set up, disable bypass so XYScale is now running */
    *PIE_SCALE_CFG &= ~PIE_SCALE_CFG_BYPASS; 

    /* davep 06-Jul-2006 ; XXX bug in XYScale hardware. Have to program TX/TY
     * registers *outside* bypass mode.
     */
    if( scaler_mode==PIE_XY_SCALER_LINEAR ) {
        *PIE_SCALE_TX = scale_tx;
        *PIE_SCALE_TY = scale_ty;
    }
    
    /* before we run any data through, let's see what all the registers are */
    pie_xy_dump();
}

/**
 * \brief Get number of rows out of scaler based on settings.
 *
 *  Because of XYScale's padding requirements and the scaling factor, the exact
 *  number of rows coming out of XYScale cannot be predicted ahead of time.
 *  However, using the scale factor, we can tell how many actual rows of real
 *  data to expect. 
 *
 *  We need to pad the bottom of the image on the read to match the stripsize
 *  but that extra data must be removed from the output image when we're
 *  complete. 
 *
 * \param[in] total_rows_in number of rows fed into top of XYScale block
 * \param[in] fracY XYScale's Y direction scale factor
 * \param[in] scaler_mode PIE_XY_SCALER_LINEAR or PIE_XY_SCALER_RAPR
 * \param[out] expected_rows_out based on the total number of input rows and the
 *          scale factor, how many rows would a normal human expect
 *
 * \author David Poole
 * \date 26-June-05
 *
 * \remarks This function will eventually be freakishly complicated because of
 * the possibility of numeric overflow with total_rows_in * fracY > 2^32.
 * DougK sent me an email containing a formula which should prevent overflows.
 *
 * \remarks 26-July-05 I'm now finally putting DougK's email formula into this
 * function. Hasn't been excessively tested, though. See 26-July-05 notes for
 * original email.
 *
 *
 *davep 13-Jun-2011 ; DO NOT CALL THIS FUNCTION TO GET HORIZONTAL VALUES!
 *    Only call for rows!  The -1 in Linear mode is something bizarre for rows.
 *    Not pixels. See DougK for explanation.  Or maybe I'm just not that smart.
 *
 **/

void pie_xy_calc_expected_rows( int rows_in, int scale_factor, int scaler_mode, int *rows_out )
{
    XASSERT( rows_in>0, rows_in );

    if( scaler_mode==PIE_XY_SCALER_LINEAR ) {
        /* davep 10-Jul-2006 ; DougK suggested using total_rows_in-1 for Linear */
        *rows_out = ( (rows_in-1) * (1<<16)) / scale_factor;
    }
    else if( scaler_mode==PIE_XY_SCALER_RAPR ) {
        XASSERT( scaler_mode==PIE_XY_SCALER_RAPR, scaler_mode );
        /* 26-July-05 davep ; original simple formula which would overflow pretty
         * quickly
         *
         *    *expected_rows_out = (total_rows_in * xyscale) / (1<<16);
         */
        /* DougK's new formula which I admit I don't follow */
        *rows_out = ( ((rows_in>>12) * scale_factor) + (((rows_in % (1<<12) ) * scale_factor) >> 12)) >> 4;
    }
}

/**
 * \brief calculate expected pixel width
 *
 *  why is this function different than the 'expected_rows' version above?
 *  Because there is a huge amount of code that relies on the expected rows
 *  version and changing to the same formula below breaks that code. 
 *
 *  The -1 in linear above is because linear eats a line. I need to go through
 *  a lot of code to fix the +1/-1 compensation. But first, need to get this
 *  code done.
 *
 * \author David Poole
 * \date 13-Jun-2011
 *
 */

void pie_xy_calc_expected_pixels( int pixels_in, int scale_factor, int scaler_mode, int *pixels_out )
{
    unsigned long long llpin, llpout, llscale;

    XASSERT( pixels_in>0, pixels_in );

    llscale = (unsigned long long)scale_factor;
    llpin = (unsigned long long)pixels_in;

    if( scaler_mode==PIE_XY_SCALER_LINEAR ) {
        llpout = (llpin * (1<<16));
        llpout = safeint_divide_uint64( llpout, llscale );
    }
    else {
        XASSERT( scaler_mode==PIE_XY_SCALER_RAPR, scaler_mode );
        llpout = (llpin * llscale) / (1<<16);
    }

    dbg2( "%s scale=%llu pin=%llu pout=%llu\n", __FUNCTION__, llscale, llpin, llpout );

    XASSERT( (unsigned long)(llpout>>32)==0, (uint32_t)(llpout>>32) );

    *pixels_out = llpout;
}

/**
 * \brief Utility function to calculate XYScale's scale factors 
 *
 * \author David Poole
 * \date 10-Jun-2011
 *
 */

void pie_xy_calc_xyscale( struct pie_scale *scale, int scaler_mode )
{
    if( scaler_mode==PIE_XY_SCALER_LINEAR ) {
        scale->x_scale = ((1<<16) * scale->x_denominator) / scale->x_numerator;
        scale->y_scale = ((1<<16) * scale->y_denominator) / scale->y_numerator;
    }
    else {
        XASSERT( scaler_mode==PIE_XY_SCALER_RAPR, scaler_mode );
        scale->x_scale = ((1<<16) * scale->x_numerator) / scale->x_denominator;
        scale->y_scale = ((1<<16) * scale->y_numerator) / scale->y_denominator;
    }

    XASSERT( PIE_SCALE_SCALEX_SCALE(scale->x_scale)==scale->x_scale, scale->x_scale );
    XASSERT( PIE_SCALE_SCALEY_SCALE(scale->y_scale)==scale->y_scale, scale->y_scale );
}
    
/**
 * \brief  Return max write DMA rows out based on our current platform.
 *
 * \author David Poole
 * \date 17-Nov-2006
 *
 */

static int
pie_get_wdma_max_rows( void )
{
    return PIE_WDMA_MAX_ROWS;
}

/**
 * \brief  Calculate the optimal stripsize taking into account the scaling
 * factor.
 *
 * \param[in,out] read_stripsize
 * \param[in] scaler_mode PIE_XY_SCALER_LINEAR or PIE_XY_SCALER_RAPR
 * \param[in] fracY
 * \param[out] write_stripsize
 *
 * \author David Poole
 * \date 20-Sep-2005
 *
 *
 */

scan_err_t
pie_xy_calc_stripsize( int *read_stripsize, 
                       int fracY, 
                       int scaler_mode, 
                       int *write_stripsize )
{
    int strip_size_in;
    int strip_size_out;
    int max_rows_out;

    strip_size_in = *read_stripsize;
    XASSERT( strip_size_in>0 && strip_size_in<=PIE_RDMA_MAX_ROWS, strip_size_in );

    max_rows_out = pie_get_wdma_max_rows();

    while( 1 ) {
        if( scaler_mode==PIE_XY_SCALER_LINEAR ) {
            strip_size_out = pie_xy_calc_linear_mode_strip_size( strip_size_in, fracY );
        }
        else {
            XASSERT( scaler_mode==PIE_XY_SCALER_RAPR, scaler_mode );
            strip_size_out = (strip_size_in * fracY) / (1<<16);
        }

        if( strip_size_out <= max_rows_out ) {
            break;
        }

        strip_size_in--;
        if( strip_size_in <= 0 ) {
            /* most likely cause of this failure is a scaling factor that is
             * too large 
             */
            XASSERT( 0, *read_stripsize );
            *write_stripsize = 0;
            return SCANERR_INVALID_PARAM;
        }
    } 

    *read_stripsize = strip_size_in;
    *write_stripsize = strip_size_out;

    /* check my math */
    XASSERT( *read_stripsize>0 && *read_stripsize<=PIE_RDMA_MAX_ROWS, *read_stripsize );
    XASSERT( *write_stripsize>0 && *write_stripsize<=PIE_WDMA_MAX_ROWS, *write_stripsize );

    return 0;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 09-Jun-2011
 *
 */

scan_err_t pie_calc_stripsize( struct pie_strip *strip, int y_scale, int scaler_mode )
{
    scan_err_t scerr;
    int extra_rows_for_cs;
    int extra_rows_for_mf;

    extra_rows_for_cs = 0;
    if( strip->use_cs ) {
        extra_rows_for_cs = 2;
    }
    extra_rows_for_mf = 0;
    if( strip->use_mf ) {
        /* TODO add support for 3x3 (eats two rows) */
        extra_rows_for_mf = 4;
    }
    strip->total_extra_rows = extra_rows_for_cs + extra_rows_for_mf;
        
    strip->rdma_in = PIE_RDMA_MAX_ROWS;

    strip->cs_in = strip->rdma_in;
    strip->cs_out = strip->cs_in - extra_rows_for_cs;

    strip->mf_in = strip->cs_out;
    strip->mf_out = strip->mf_in - extra_rows_for_mf;

    strip->xy_in = strip->mf_out;

    if( strip->use_xy ) {
        scerr = pie_xy_calc_stripsize( &strip->xy_in, y_scale, scaler_mode, &strip->xy_out );
        if( scerr != SCANERR_NONE ) {
            memset( strip, 0, sizeof(struct pie_strip) );
            return scerr;
        }
        strip->wdma_out = strip->xy_out;

        strip->mf_out = strip->xy_in;
        strip->mf_in = strip->mf_out + extra_rows_for_mf;

        strip->cs_out = strip->mf_in;
        strip->cs_in = strip->cs_out + extra_rows_for_cs;

        strip->rdma_in = strip->cs_in;
    }

    /* check for math mistakes */
    XASSERT( strip->rdma_in <= PIE_RDMA_MAX_ROWS, strip->rdma_in );
    XASSERT( strip->wdma_out <= PIE_WDMA_MAX_ROWS, strip->wdma_out );

    return SCANERR_NONE;
}

void pie_dbg2_scale( struct pie_scale *scale ) 
{
    pie_dbg2( "pie scale x=%d/%d y=%d/%d xs=%d ys=%d\n", 
        scale->x_numerator, scale->x_denominator, 
        scale->y_numerator, scale->y_denominator,
        scale->x_scale, scale->y_scale );
}

void pie_dbg2_strip( struct pie_strip *strip )
{
    pie_dbg2( "pie strip in=%d cs=%d,%d mf=%d,%d xy=%d,%d out=%d\n", 
                strip->rdma_in,
                strip->cs_in, strip->cs_out,
                strip->mf_in, strip->mf_out,
                strip->xy_in, strip->xy_out,
                strip->wdma_out );
}

void pie_dbg2_row( struct pie_dma_row *row )
{
    pie_dbg2( "pie row pi=%d po=%d bi=%d bo=%d\n", 
                row->pixels_in, row->pixels_out, row->bytes_in, row->bytes_out );
}

/**
 * \brief Get/Set the PIE context.
 *
 * Works like a CPU context switch. Get current registers, restore other
 * registers.
 *
 * As of this writing (20-Jul-2013), only does the XYScale registers necessary
 * for multiplexing multiple images with same scale factor.
 *
 * \author David Poole
 * \date 20-Jul-2013
 */

void pie_get_context( struct pie_context *ctx )
{
    /* save PIE hardware context */
    ctx->cyr = *PIE_SCALE_CURRDY;
    ctx->cxr = *PIE_SCALE_CURRDX;
    ctx->yper = *PIE_SCALE_CURRYPER;

//    dbg2( "%s %x %x %x\n", __FUNCTION__, ctx->cyr, ctx->cxr, ctx->yper );
}

void pie_set_context( struct pie_context *ctx )
{
//    dbg2( "%s currdy=0x%x write=0x%x\n", __FUNCTION__, 
//            *PIE_SCALE_CURRDY, ctx->cyr);

    /* load PIE hardware context */
    *PIE_SCALE_TY = ctx->cyr;
    *PIE_SCALE_TX = ctx->cxr;

    *PIE_SCALE_INITYPER = ctx->yper;
}

/**
 * \brief calculate bytes per row for DMA channel read and write 
 *
 * Takes into account the 16-byte DMA alignment requirement.
 *
 * \param[in] pixels_per_row
 * \param[in] pixel_size
 * \param[in] scan_type
 * \param[in] pie_output_type
 * \param[in] scale
 * \param[in] extra_pad     Optional extra padding which will be added/removed
 *                          from the row by the XYScale block via the OUTX register.
 *                          Final DMA align will occur after extra_pad is added
 *                          to pixels_out.
 * \param[in] scaler_mode PIE_XY_SCALER_LINEAR or PIE_XY_SCALER_RAPR
 * \param[out] row
 *
 * \author David Poole
 * \date 30-Aug-2005
 *
 */

void
pie_xy_calc_row( int pixels_per_row, 
                 uint32_t pixel_size, 
                 pie_pixel_t pie_output_type,
                 struct pie_scale *scale,
                 int extra_pad,
                 int scaler_mode,
                 struct pie_dma_row *row )
{
    dbg2( "%s ppr=%d psize=%d pout=%d\n", __FUNCTION__,
                pixels_per_row, pixel_size, pie_output_type );

    /* stupid human check */
    XASSERT( scale->x_scale && PIE_SCALE_SCALEX_SCALE(scale->x_scale)==scale->x_scale, scale->x_scale );

    memset( row, 0, sizeof(struct pie_dma_row ) );
    row->pixels_in = pixels_per_row;
    pie_xy_calc_expected_pixels( row->pixels_in, scale->x_scale, scaler_mode, &row->pixels_out ); 

    if( pixel_size == 8 ) {
        row->bytes_in = row->pixels_in;
    }
    else {
        XASSERT( pixel_size==16, pixel_size );
        row->bytes_in = 2 * row->pixels_in;
    }

    /* add in the pad pixels */
    row->pixels_out += extra_pad;

    /* pad up to make sure we're DMA aligned (note we're mixing pixels and
     * bytes; the math still works)
     */
    row->pixels_out = ICE_DMA_ALIGN_ME(row->pixels_out);

    row->bytes_out = pie_get_bytes_per_pixel(pie_output_type) * row->pixels_out;

    pie_dbg2_row( row );
}

/**********************************************************
 *
 * Overall PIE Functions
 *
 *********************************************************/

/**
 * \brief Get pointer to the PIE interrupt statistics
 *
 *  Want to keep pie_interrupt_stats local to pie.c (no externs). Use this
 *  accessor function if we need to fiddle with the structure fields.
 *  
 *  <b>
 *  Please don't change the fields unless you're pie_dma_interrupt() or
 *  pie_interrupt()!
 *  </b>
 *
 * \retval pie_interrupt_stats structure containing interrupt statistics
 * \author David Poole
 * \date 9-Mar-05
 *
 **/
struct pie_interrupt_stats * get_pie_interrupt_stats( void )
{
    return &pie_interrupt_stats;
}

/**
 * \brief clear the PIE interrupt stats 
 *
 * It's nice to have a clean slate before running a bunch of stuff through PIE.
 * 
 * \author David Poole
 * \date 20-Oct-2005
 *
 * Remember. Only you can prevent stupid side effects. Don't call this function
 * while PIE is active.
 */

void pie_clear_interrupt_stats( void )
{
    memset( &pie_interrupt_stats, 0, sizeof(pie_interrupt_stats) );
}

void pie_interrupt_stats_dump( void )
{
    dbg1( "pie ints count=%d rdma=%d %d %d %d %d %d wdma=%d %d %d %d %d %d\n", 
        pie_interrupt_stats.count,
        pie_interrupt_stats.rdma[0],
        pie_interrupt_stats.rdma[1],
        pie_interrupt_stats.rdma[2],
        pie_interrupt_stats.rdma[3],
        pie_interrupt_stats.rdma[4],
        pie_interrupt_stats.rdma[5],
        pie_interrupt_stats.wdma[0],
        pie_interrupt_stats.wdma[1],
        pie_interrupt_stats.wdma[2],
        pie_interrupt_stats.wdma[3],
        pie_interrupt_stats.wdma[4],
        pie_interrupt_stats.wdma[5] );
}

/**
 * \brief Toggle the PIE block reset bit.
 *
 *  Performs a reset on all of the PIE hardware block by setting then clearing
 *  the reset bit.
 *
 *  This is its own function because the only way to completely reset certain
 *  parts of PIE (such as the DMA, perhaps more) is by resetting all of PIE.
 *
 * \author David Poole
 * \date 11-May-05
 *
 **/
void
pie_block_reset( void )
{
    pie_dbg2( "%s\n", __FUNCTION__);

    /* reset PIE by holding the reset bit for a few ticks 
     * XXX is 1 microsecond long enough? Too long?
     */
    /* 24-Mar-05 ; RandyS and JohnM tell me 1us is puh-lenty. */
    *PIE_CI_CBIR |= PIE_CI_CBIR_RESET;
    cpu_spin_delay( 1 );
    *PIE_CI_CBIR &= ~PIE_CI_CBIR_RESET;
}

/**
 * \brief Restore all of PIE to its power-on default values.
 *
 * This function resets all the PIE registers to their documented power-on
 * values. Calls the sub-block reset functions.
 *
 * \author David Poole
 * \date 6-May-05
 *
 **/
void
pie_reset( void )
{
    pie_dbg2("%s\n", __FUNCTION__);

    pie_block_reset();

    /* 
     * Reset common registers
     */

    /* disable all interrupts */
    pie_interrupt_disable();

    /*
     * Reset Read DMA
     */

    /* reset the register values to power-on defaults (paranoia) */
    pie_rdma_reset();
    

    /*
     * Reset bit depth enhance
     * MA_BitDepthEnhance.doc
     */

    /* 4-May-05 ; reset BDE and LUT to power-on values */
    pie_bde_reset();

    /* davep 07-Mar-2013 ; adding CStats */
    pie_cstats_reset();

    /* 
     * Reset PIE CIS ColorShift
     *
     * new as of ~Feb-2011
     */
    pie_cs_reset();

    /* 
     * Reset RGB2esRGB
     * CSC_MA.doc
     */

    /* reset all of the three CSC blocks; multiply coefficients and LUT are
     * reset to power-on values (zero)
     */
    pie_csc_reset();


    /*
     * Reset esRGB2YCrCb
     * CSC_MA.doc
     */
    /* 3-May-05 ; now done in pie_csc_reset() */


    /* 
     * Reset color plane adjust
     * MA_ColorPlaneAdjust.doc
     */
    pie_cpa_reset();


    /* 
     * Reset tone/contrast & neutral sensitivity
     * MA_ToneContrastNeutral.doc
     */
    pie_tcns_reset();


    /* 
     * Reset pogo aligner
     * PogoAlignerMA.doc
     */

    /* 8-Mar-05 ; I can't find a bypass bit for this */
    *PIE_ALIGN_PACR = PIE_ALIGN_PACR_R;


    /* 
     * Reset multifilter Y/CrCb & Text Enhance
     * MultiFilterrMA.doc, SculptorMA.doc
     */

    pie_filter_reset();


    /*
     * Reset YCrCb2esRGB
     * CSC_MA.doc
     */
    /* 3-May-05 ; now done in pie_csc_reset() */


    /* 
     * Reset X-Y Scale
     * xyscale.doc
     */

    pie_xy_reset();


    /*
     * Reset Mono HalfPack
     * HalfPack_MA.doc
     */

    /* davep 07-Oct-2007 ; some platforms don't have HalfPack */
#ifdef HAVE_ICE_HALFPACK
    piehp_reset();
#endif

    /* 7-Mar-05 ; set to Bypass; as of this writing, it doesn't come up in
     * bypass 
     */
//    *PIE_HP_CFG |= PIE_HP_CFG_BYPASS;


    /* 
     * Reset Write DMA
     * PIEWriteDMAMA.doc
     */
    pie_wdma_reset();
}

/**
 * \brief PIE hardware setup.
 *
 *  Restores PIE to as close to power-on values as possible. Doesn't interfere
 *  with operating system interface (interrupts, extra debugging, etc).
 *
 *  Created to separate first-time intialization of PIE (turn on the hardware
 *  as well as hook up operating system interrupts, add debug commands,
 *  one-time data structure set-up) from things that needed to be done to reset
 *  PIE completely to a predictable, clean, runnable state.
 *
 * \retval 0 success
 * \retval !0 detectable hardware failure
 *
 * \author David Poole
 * \date 10-Mar-05
 *
 *
 **/
scan_err_t pie_soft_setup( void )
{
//    dbg1("%s\n", __FUNCTION__);

    /* disable PIE while configuring */
    pie_interrupt_disable();

    /* reset all of PIE to their power-on values */
//    pie_block_reset();
    pie_reset();

    /* 
     * Setup PIE Common Block
     */

    /* Turn on PIE read/write DMA interrupts. Leave off Mono HalfPack
     * read/write DMA interrupts until we actually need them.
     */
    /* 11-Mar-05 ; enable Mono HalfPack interrupts */
//    num32 = (uint32_t)(PIE_CI_RDMA0_BIT     
//          | PIE_CI_RDMA1_BIT     
//          | PIE_CI_RDMA2_BIT     
//          | PIE_CI_RDMA3_BIT     
//          | PIE_CI_RDMA4_BIT     
//          | PIE_CI_RDMA5_BIT     
//          | PIE_CI_RDMACMN_BIT   
//          | PIE_CI_WDMA0_BIT     
//          | PIE_CI_WDMA1_BIT     
//          | PIE_CI_WDMA2_BIT     
//          | PIE_CI_WDMACMN_BIT   
//          | PIE_CI_MONOHP_WDMA_BIT 
//          | PIE_CI_MONOHP_RDMA_BIT )
//          ;
    /* davep 14-Mar-2011 ; turn on everything and hope for the best */
    pie_interrupt_enable();

    return SCANERR_NONE;
}

#ifdef __KERNEL__
scan_err_t pie_capture_interrupt( void )
{
    int retcode;

    /* wait until explicitly enabled */
    pie_interrupt_disable();

    retcode = request_irq( INTNUM_PIE, pie_interrupt, 0, "icepie", NULL );
    dbg2( "%s request_irq retcode=%d\n", __FUNCTION__, retcode );
    XASSERT(retcode==0, retcode );

    return SCANERR_NONE;
}

void pie_release_interrupt( void )
{
    pie_interrupt_disable();
    /* disable at the OS level as well */
    disable_irq( INTNUM_PIE );
    free_irq( INTNUM_PIE, NULL );
}
#else
scan_err_t pie_capture_interrupt( void )
{
    intAttach( INTNUM_PIE, 1, pie_interrupt, INTNUM_PIE );
    intEnable( INTNUM_PIE );
    return SCANERR_NONE;
}

void pie_release_interrupt( void )
{
    pie_interrupt_disable();
    /* disable at the OS level as well */
    intDisable( INTNUM_PIE );
    intDetach( INTNUM_PIE );
}
#endif


/**
 * \brief PIE hardware and OS setup.
 *
 *  Does one-time initial PIE hardware and OS level setup. Should be called
 *  once in life of system.
 *  
 * \bug Not yet returning hardware failure! Hardwired to return 0.
 *
 * \retval 0 success
 * \retval !0 detectable hardware failure
 *
 * \author David Poole
 * \date 6-Mar-05
 *
 **/

scan_err_t pie_onetime_init( void )
{
    scan_err_t scerr;

    pie_dbg2("%s\n", __FUNCTION__);

    pie_reset();

    /* davep 15-Mar-2010 ; Sanity check some addresses, at least one register
     * in each sub-block.  These registers default to non-zero.
     */
//    XASSERT( *PIE_RDMA_CCR==PIE_RDMA_CCR_R, (uint32_t)*PIE_RDMA_CCR );
    XASSERT( *PIE_BDE_CFG==PIE_BDE_CFG_R, (uint32_t)*PIE_BDE_CFG );
    XASSERT( *PIE_CSC_RGB_CCR==PIE_CSC_RGB_CCR_R, (uint32_t)*PIE_CSC_RGB_CCR );
    XASSERT( *PIE_CSC_ESR_CCR==PIE_CSC_ESR_CCR_R, (uint32_t)*PIE_CSC_ESR_CCR );
    XASSERT( *PIE_CSC_YCC_CCR==PIE_CSC_YCC_CCR_R, (uint32_t)*PIE_CSC_YCC_CCR );
#ifdef HAVE_PIE_CPA
    XASSERT( *PIE_CPA_CR==PIE_CPA_CR_R, (uint32_t)*PIE_CPA_CR );
#endif
    XASSERT( *PIE_TCNS_CR==PIE_TCNS_CR_R, (uint32_t)*PIE_TCNS_CR );
    XASSERT( *PIE_ALIGN_PACR==PIE_ALIGN_PACR_R, (uint32_t)*PIE_ALIGN_PACR );
    XASSERT( *PIE_FILTER_SCR2==PIE_FILTER_SCR2_R, (uint32_t)*PIE_FILTER_SCR2 );
    XASSERT( *PIE_SCALE_CFG==PIE_SCALE_CFG_R, (uint32_t)*PIE_SCALE_CFG );
//    XASSERT( *PIE_WDMA_DGCR==PIE_WDMA_DGCR_R, (uint32_t)*PIE_WDMA_DGCR );

    /* add PIE interrupt into OS but don't enable until everything is set up */
    pie_interrupt_disable();
    pie_capture_interrupt();

    scerr = pie_soft_setup();
    if( scerr != 0 ) {
        /* pie_soft_setup() hardwire to return 0 at this time but just in
         * case... 
         */
        return scerr;
    }

    /* all PIE data structures set up so we now can allow "things" to happen */
    pie_interrupt_enable();

    return SCANERR_NONE;
}

/**
 * \brief  Release all PIE resources. 
 *
 * Originally created for Linux kernel module __exit
 *
 * \author David Poole
 * \date 31-May-2012
 */

scan_err_t pie_cleanup_module( void )
{
    pie_release_interrupt();

    return SCANERR_NONE;
}

/**
 * \brief  Dump a few parts of PIE
 *
 * Doesn't dump all of PIE (too huge) but just the top level interrupt interface stuff.
 *
 * \author David Poole
 * \date 21-Jun-2013
 */

void pie_dump( void )
{
    /* can't use the regbuild registers because the header files are different
     * names
     */
    dbg1( "PIECI IENR=0x%08x IPR=0x%08x CBIR=0x%08x PipeConfig=??\n", 
            *PIE_CI_IENR, *PIE_CI_IPR, *PIE_CI_CBIR );
}

