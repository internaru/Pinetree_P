/******************************************************************************
 * Copyright (c) 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

//#define HAVE_ICE_HALFPACK 1

/* 2007 moved HalfPack to his own address space (out from underneath
 * PIE) and gave him interrupts of his own. 
 *
 * Define HAVE_INTEGRATED_HALFPACK if HalfPack still lives with PIE
 *
 * Define HAVE_INDEPENDENT_HALFPACK if HalfPack lives on its own 
 *
 * (Hopefully) has no effect unless HAVE_ICE_HALFPACK is also defined.
 * comment both out for ASICs with no HalfPack, which has no halfpack
 */
// #define HAVE_INDEPENDENT_HALFPACK 1
//#define HAVE_INTEGRATED_HALFPACK 1

/* 2007 introduced a PRNU/DSNU max gain of shift left three by
 * stealing the config bits from the shift right 1.  MUL8 gives us a max 8x PRNU
 * pixel correction where our previous was 4x.
 */
#define HAVE_ICE_PRNUDSNU_MUL8 1

/* 2007 HalfPack added an integrated Size, Error Diffusion, and
 * Density table. If this is undefined, the firmware will assume the the tables
 * are in three independent memory locations.
 */
//#define HAVE_HALFPACK_SZDD 1

/* 2007 added the option of doing error diffusion on full pixels
 * instead of pixel pairs. No more "pixels come in pears" jokes, I guess. Sad...
 */
//#define HAVE_HALFPACK_FULL_PIXEL_DIFFUSION 1

/* 2007 added the feature to be able to select DMA burst sizes
 * between 16 and 32 bytes.  PIC, PIE, and HalfPack DMAs all support this
 * feature.  2005 doesn't have this feature.
 */
#define HAVE_ICE_DMA_SET_BURST_SIZE 1

/* On ASICs with selectable burst size (HAVE_ICE_DMA_SET_BURST_SIZE), the burst
 * size can be either 16 or 32 bytes.
 *
 * On ASICs without selectable burst size, 16 is the only valid option.
 */
//#define ICE_DMA_BYTE_ALIGN 16
#define ICE_DMA_BYTE_ALIGN 32

/* Some ASICs have a PIE which can pack output pixels into RGBX and RGB
 * (24-bpp) pixels.
 */
#define HAVE_PIE_OUTPUT_RGBX 1
#define HAVE_PIE_OUTPUT_RGB  1

//#define HAVE_CACHE_BYPASS 1

/* davep 24-Mar-2010 ; ASIC dropped support for staggered sensors so
 * adding new symbol to support staggered hardware 
 */
//#define HAVE_STAGGERED_SENSOR_SUPPORT 1

/* This ASIC has three BDR LUTs in PIC */
#define HAVE_THREE_BDR_LUT 1

/* davep 26-Mar-2010 ; we dropped the 2005 style PIC WDMA and moved to
 * a chained descriptor model
 */
#define HAVE_PIC_DESCRIPTOR_DMA 1

/*
 * Z1 added the ACLK6 register to accomodate 4 additional MCLK edges (for a total
 * of 16, up from 12).  This was needed to support the Internal AFE OSR=8 mode.
 */
#define HAVE_SCIF_ACLK6 1

/*
 * Z1 added Unexpected Motor Sync to the SCIF status register.  It was
 * intended for debug use, but has a habit of wagging around (which means we
 * can't just ignore it).
 */
#define HAVE_SCIF_UNEXPSYNC 1

/* davep 22-Oct-2010 ; moving a run-time ASIC ID check to a compile time flag
 * so we can avoid hardwiring asic id numbers.
 *
 * If this is defined, PIE's CSC RGB->esRGB block has three LUTS, one each for
 * R,G,B.  If not defined, we have a single LUT for all three colors.
 */
#define HAVE_THREE_RGB2ESRGB_LUT 1

/* davep 19-Nov-2010 ; adding support for CISX. CISX does pixel reordering of
 * parallel sensors.
 */
#define HAVE_CISX 1

/* davep 21-Jan-2011 ; adding PIC PRNU/DSNU Quadratic Correction */
#define HAVE_PIC_PC_QUAD 1

/* davep 14-Jun-2011 ; adding ICE Test, a block to feed data into Scan and/or
 * PIC via DMA 
 */
#define HAVE_ICETEST 1

/* davep 20-Dec-2011 ; adding 5x5 and 5x9 MultiFilter variable windows */
//#define HAVE_PIE_SCULPTOR_5X5
#define HAVE_PIE_SCULPTOR_5X9

/* davep 16-Jul-2012 ; ASIC support for >1 sensors (currently only two) */
#define HAVE_NSENSOR_SUPPORT

/* davep 07-Mar-2013 ; swap R/B planes on ASICS that don't have the proper PIE
 * DEPOGO capability
 */
#define HAVE_PIE_POGO_BGR 

/* davep 11-Apr-2013 ; enabling PIE ColorShift, a block to merge CIS color
 * fringed lines 
 */
#define HAVE_PIE_COLORSHIFT 1

