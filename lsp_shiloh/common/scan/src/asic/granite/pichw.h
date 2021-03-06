/**
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

#ifndef PICHW_H
#define PICHW_H

// auto generated by rereg.py @ 08:36:04 29-Jun-2012

#define PIC_CI_IENR               ((volatile uint32_t *)(ICE_BASE+0x80000))
#define PIC_CI_WDMA2_BIT     (0x1 << 11)
#define PIC_CI_WDMA1_BIT     (0x1 << 10)
#define PIC_CI_WDMA0_BIT     (0x1 << 9)
#define PIC_CI_WDMA_COMMON_BIT (0x1 << 8)
#define PIC_CI_IDMA_2D_Ch0   (0x1 << 1)
#define PIC_CI_COMBUSOI_BIT  (0x1 << 0)
#define PIC_CI_WDMA5_BIT     (0x1 << 14)
#define PIC_CI_WDMA4_BIT     (0x1 << 13)
#define PIC_CI_WDMA3_BIT     (0x1 << 12)
#define PIC_CI_WDMA2_BIT     (0x1 << 11)
#define PIC_CI_WDMA1_BIT     (0x1 << 10)
#define PIC_CI_WDMA0_BIT     (0x1 << 9)
#define PIC_CI_WDMA_COMMON_BIT (0x1 << 8)
#define PIC_CI_IDMA_2D_Ch0   (0x1 << 1)
#define PIC_CI_IDMA_2D_Ch1   (0x1 << 2)
#define PIC_CI_WDMA_BIT(x)   (1<< (x) )
/* ASIC dropped support for staggered sensors but need these symbols to
 * compile (for now).
 */
//#define PIC_CI_WDMA3_BIT     (0)  /* write DMA 3 interrupt */
//#define PIC_CI_WDMA4_BIT     (0)  /* write DMA 4 interrupt */
//#define PIC_CI_WDMA5_BIT     (0)  /* write DMA 5 interrupt */

#define PIC_CI_IPR                ((volatile uint32_t *)(ICE_BASE+0x80004))
#define PIC_CI_IPR_WDMA2_BIT      (0x1 << 11)
#define PIC_CI_IPR_WDMA1_BIT      (0x1 << 10)
#define PIC_CI_IPR_WDMA0_BIT      (0x1 << 9)
#define PIC_CI_IPR_WDMA_COMMON_BIT (0x1 << 8)
#define PIC_CI_IPR_IDMA_2D_Ch0    (0x1 << 1)
#define PIC_CI_IPR_COMBUSOI_BIT   (0x1 << 0)
#define PIC_CI_IPR_WDMA5_BIT      (0x1 << 14)
#define PIC_CI_IPR_WDMA4_BIT      (0x1 << 13)
#define PIC_CI_IPR_WDMA3_BIT      (0x1 << 12)
#define PIC_CI_IPR_WDMA2_BIT      (0x1 << 11)
#define PIC_CI_IPR_WDMA1_BIT      (0x1 << 10)
#define PIC_CI_IPR_WDMA0_BIT      (0x1 << 9)
#define PIC_CI_IPR_WDMA_COMMON_BIT (0x1 << 8)
#define PIC_CI_IPR_IDMA_2D_Ch0    (0x1 << 1)
#define PIC_CI_IPR_IDMA_2D_Ch1    (0x1 << 2)
#define PIC_CI_WDMA_BIT(x)   (1<< (x) )
/* ASIC dropped support for staggered sensors but need these symbols to
 * compile (for now).
 */
//#define PIC_CI_WDMA3_BIT     (0)  /* write DMA 3 interrupt */
//#define PIC_CI_WDMA4_BIT     (0)  /* write DMA 4 interrupt */
//#define PIC_CI_WDMA5_BIT     (0)  /* write DMA 5 interrupt */

#define PIC_CI_ITR                ((volatile uint32_t *)(ICE_BASE+0x80008))
#define PIC_CI_ITR_WDMA2_BIT      (0x1 << 11)
#define PIC_CI_ITR_WDMA1_BIT      (0x1 << 10)
#define PIC_CI_ITR_WDMA0_BIT      (0x1 << 9)
#define PIC_CI_ITR_WDMA_COMMON_BIT (0x1 << 8)
#define PIC_CI_ITR_IDMA_2D_Ch0    (0x1 << 1)
#define PIC_CI_ITR_COMBUSOI_BIT   (0x1 << 0)
#define PIC_CI_ITR_WDMA5_BIT      (0x1 << 14)
#define PIC_CI_ITR_WDMA4_BIT      (0x1 << 13)
#define PIC_CI_ITR_WDMA3_BIT      (0x1 << 12)
#define PIC_CI_ITR_WDMA2_BIT      (0x1 << 11)
#define PIC_CI_ITR_WDMA1_BIT      (0x1 << 10)
#define PIC_CI_ITR_WDMA0_BIT      (0x1 << 9)
#define PIC_CI_ITR_WDMA_COMMON_BIT (0x1 << 8)
#define PIC_CI_ITR_IDMA_2D_Ch0    (0x1 << 1)
#define PIC_CI_ITR_IDMA_2D_Ch1    (0x1 << 2)
#define PIC_CI_WDMA_BIT(x)   (1<< (x) )
/* ASIC dropped support for staggered sensors but need these symbols to
 * compile (for now).
 */
//#define PIC_CI_WDMA3_BIT     (0)  /* write DMA 3 interrupt */
//#define PIC_CI_WDMA4_BIT     (0)  /* write DMA 4 interrupt */
//#define PIC_CI_WDMA5_BIT     (0)  /* write DMA 5 interrupt */

#define PIC_CI_CBIR               ((volatile uint32_t *)(ICE_BASE+0x8000c))
#define PIC_CI_CBIR_SOFTRESET     (0x1 << 0)
#define PIC_CI_CBIR_DUAL_CHANNEL  (0x1 << 1)

#define PIC_ADCN_ANR              ((volatile uint32_t *)(ICE_BASE+0x81000))
#define PIC_ADCN_ANR_TEST         (0x1 << 9)
#define PIC_ADCN_ANR_BYPASS       (0x1 << 8)
#define PIC_ADCN_ANR_INV          (0x1 << 7)
#define PIC_ADCN_ANR_JUSTIFY      (0x1 << 6)
#define PIC_ADCN_ANR_PADFILL(x)   (((x) & 0x3) << 4)
#define PIC_ADCN_ANR_ABITS(x)     (((x) & 0xf) << 0)
/* PIC_ADCN_ANR_PADFILL values */
#define PIC_ADCN_ANR_PADFILL_ZEROES  0  /* pad with all 0's */
#define PIC_ADCN_ANR_PADFILL_ONES    1  /* pad with all 1's */
#define PIC_ADCN_ANR_PADFILL_MSB     3  /* replicate MSB bits */

#define PIC_ADCN_ANI              ((volatile uint32_t *)(ICE_BASE+0x81004))
#define PIC_ADCN_ANI_OVERRUN      (0x1 << 0)

#define PIC_ADCN_ANIP             ((volatile uint32_t *)(ICE_BASE+0x81008))
#define PIC_ADCN_ANIP_OVERRUN     (0x1 << 0)

#define PIC_ADCN_ANA              ((volatile uint32_t *)(ICE_BASE+0x8100c))
#define PIC_ADCN_ANA_OVERRUN      (0x1 << 0)

#define PIC_ADCN_ANIF             ((volatile uint32_t *)(ICE_BASE+0x81010))
#define PIC_ADCN_ANIF_OVERRUN     (0x1 << 0)

#define PIC_ADCN_ANT              ((volatile uint32_t *)(ICE_BASE+0x81014))
#define PIC_ADCN_ANT_PIC_TAGOUT(x) (((x) & 3) << 22)
#define PIC_ADCN_ANT_EXP          (0x1 << 21)
#define PIC_ADCN_ANT_COLOR(x)     (((x) & 0x7) << 18)
#define PIC_ADCN_ANT_DATATYPE(x)  (((x) & 0x3) << 16)
#define PIC_ADCN_ANT_DATA(x)      (((x) & 0xffff) << 0)
/* ADC Normalization Test Mode register PIC_ADCN_TEST_COLOR field values */
#define PIC_ADCN_TEST_CEVEN_0   PIC_CBI_CEVEN_0    /* Color 0 (even or non-staggered) */
#define PIC_ADCN_TEST_CEVEN_1   PIC_CBI_CEVEN_1    /* Color 1 (even or non-staggered) */
#define PIC_ADCN_TEST_CEVEN_2   PIC_CBI_CEVEN_2    /* Color 2 (even or non-staggered) */
#define PIC_ADCN_TEST_MEVEN     PIC_CBI_MEVEN      /* Monochrome (even or non-staggereed) */
#define PIC_ADCN_TEST_CODD_0    PIC_CBI_CODD_0     /* Color 0 (odd) */
#define PIC_ADCN_TEST_CODD_1    PIC_CBI_CODD_1     /* Color 1 (odd) */
#define PIC_ADCN_TEST_CODD_2    PIC_CBI_CODD_2     /* Color 2 (odd) */
#define PIC_ADCN_TEST_MODD      PIC_CBI_MODD       /* Monochrome (odd) */
/* ADC Normalization Test Mode register PIC_ADCN_TEST_DATATYPE field values */
#define PIC_ADCN_TEST_DT_NORMP   PIC_CBI_NORMP    /* normal data pixel */
#define PIC_ADCN_TEST_DT_FIRP    PIC_CBI_FIRP     /* first data pixel in line */
#define PIC_ADCN_TEST_DT_LASTP   PIC_CBI_LASTP    /* last data pixel in line */
#define PIC_ADCN_TEST_DT_FIRPLL  PIC_CBI_FIRPLL   /* first data pixel in last line */


#define PIC_BM_BMCR               ((volatile uint32_t *)(ICE_BASE+0x82000))
#define PIC_BM_BMCR_BYPASS        (0x1 << 31)
#define PIC_BM_BMCR_CC            (0x1 << 30)
#define PIC_BM_BMCR_NUML(x)       (((x) & 0x7) << 24)
#define PIC_BM_BMCR_WIN(x)        (((x) & 0xff) << 16)
#define PIC_BM_BMCR_P(x)          (((x) & 0xffff) << 0)

#define PIC_BM_LRMR0              ((volatile uint32_t *)(ICE_BASE+0x82004))
#define PIC_BM_LRMR_RIGHT(x)     (((x) & 0xffff) << 16)
#define PIC_BM_LRMR_LEFT(x)      (((x) & 0xffff) << 0)

#define PIC_BM_LRMR1              ((volatile uint32_t *)(ICE_BASE+0x82008))

#define PIC_BM_LRMR2              ((volatile uint32_t *)(ICE_BASE+0x8200c))

#define PIC_BM_BMR0               ((volatile uint32_t *)(ICE_BASE+0x82010))
#define PIC_BM_BMR0_AVERAGEC0     (0xffffff << 0)

#define PIC_BM_BMR1               ((volatile uint32_t *)(ICE_BASE+0x82014))
#define PIC_BM_BMR1_AVERAGEC1     (0xffffff << 0)

#define PIC_BM_BMR2               ((volatile uint32_t *)(ICE_BASE+0x82018))
#define PIC_BM_BMR2_AVERAGEC2     (0xffffff << 0)

#define PIC_BM_BMR3               ((volatile uint32_t *)(ICE_BASE+0x8201c))
#define PIC_BM_BMR3_ACCEXP        (0xfffff << 0)

#define PIC_HS_HSCR               ((volatile uint32_t *)(ICE_BASE+0x85000))
#define PIC_HS_HSCR_BYPASS        (0x1 << 0)
#define PIC_HS_HSCR_BYPASS1       (0x1 << 1)
#define PIC_HS_HSCR_BYPASS        (0x1 << 0)

#define PIC_HS_HSSR               ((volatile uint32_t *)(ICE_BASE+0x85004))
#define PIC_HS_HSSR_SF(x)         (((x) & 0x1ffff) << 0)

#define PIC_BDR_LUT               ((volatile uint32_t *)(ICE_BASE+0x86000))
#define PIC_BDR_LUT_VAL           (0xff << 0)
/* Number of 32-bit fields in PIC BDR LUT (1025 is NOT a typo -- the LUT
 * covers values from 0 to 1024 since part of its algorithm involves fetching
 * entry x+1).
 * LUT contains this many 8-bit values stored as 32-bit registers.
 */
#define PIC_BDR_LUT_SIZE   1025

#define PIC_BDR_BDRLUTR           ((volatile uint32_t *)(ICE_BASE+0x87000))
#define PIC_BDR_BDRLUTR_VAL       (0xff << 0)

#define PIC_BDR_BDRCR             ((volatile uint32_t *)(ICE_BASE+0x87004))
#define PIC_BDR_BDRCR_NUMLUTS(x)  (((x) & 0x1) << 6)
#define PIC_BDR_BDRCR_LUTSEL(x)   (((x) & 0x3) << 4)
#define PIC_BDR_BDRCR_CPU         (0x1 << 1)
#define PIC_BDR_BDRCR_BYPASS      (0x1 << 0)
#define PIC_BDR_BDRCR_NUMLUTS_ONE   0
#define PIC_BDR_BDRCR_NUMLUTS_THREE 1
#define PIC_BDR_BDRCR_LUTSEL_COLOR0MONO 0
#define PIC_BDR_BDRCR_LUTSEL_COLOR1 1
#define PIC_BDR_BDRCR_LUTSEL_COLOR2 2



#define PIC_PC_BSRLSGCR           ((volatile uint32_t *)(ICE_BASE+0x88000))
#define PIC_PC_BSRLSGCR_L_MAX_GAIN_COLOR2_INT(x) (((x) & 0x1) << 23)
#define PIC_PC_BSRLSGCR_L_MAX_GAIN_COLOR2_DEC(x) (((x) & 0x7) << 20)
#define PIC_PC_BSRLSGCR_R_MIN_GAIN_COLOR2_INT(x) (((x) & 0x1) << 19)
#define PIC_PC_BSRLSGCR_R_MIN_GAIN_COLOR2_DEC(x) (((x) & 0x7) << 16)
#define PIC_PC_BSRLSGCR_L_MAX_GAIN_COLOR1_OR_ODD_INT(x) (((x) & 0x1) << 15)
#define PIC_PC_BSRLSGCR_L_MAX_GAIN_COLOR1_OR_ODD_DEC(x) (((x) & 0x7) << 12)
#define PIC_PC_BSRLSGCR_R_MIN_GAIN_COLOR1_OR_ODD_INT(x) (((x) & 0x1) << 11)
#define PIC_PC_BSRLSGCR_R_MIN_GAIN_COLOR1_OR_ODD_DEC(x) (((x) & 0x7) << 8)
#define PIC_PC_BSRLSGCR_L_MAX_GAIN_COLOR0_OR_EVEN_INT(x) (((x) & 0x1) << 7)
#define PIC_PC_BSRLSGCR_L_MAX_GAIN_COLOR0_OR_EVEN_DEC(x) (((x) & 0x7) << 4)
#define PIC_PC_BSRLSGCR_R_MIN_GAIN_COLOR0_OR_EVEN_INT(x) (((x) & 0x1) << 3)
#define PIC_PC_BSRLSGCR_R_MIN_GAIN_COLOR0_OR_EVEN_DEC(x) (((x) & 0x7) << 0)

#define PIC_PC_BSLSVCR            ((volatile uint32_t *)(ICE_BASE+0x88004))
#define PIC_PC_BSLSVCR_L_SLOPE_VAL_COLOR0_OR_EVEN(x) (((x) & 0xff) << 16)
#define PIC_PC_BSLSVCR_L_SLOPE_VAL_COLOR1_OR_ODD(x) (((x) & 0xff) << 8)
#define PIC_PC_BSLSVCR_L_SLOPE_VAL_COLOR2(x) (((x) & 0xff) << 0)

#define PIC_PC_BSLMPCR0           ((volatile uint32_t *)(ICE_BASE+0x88008))
#define PIC_PC_BSLMPCR0_L_MAX_PIX_VAL_COLOR0_OR_EVEN(x) (((x) & 0xffff) << 0)

#define PIC_PC_BSLMPCR1           ((volatile uint32_t *)(ICE_BASE+0x8800c))
#define PIC_PC_BSLMPCR1_L_MAX_PIX_VAL_COLOR1_OR_ODD(x) (((x) & 0xffff) << 0)

#define PIC_PC_BSLMPCR2           ((volatile uint32_t *)(ICE_BASE+0x88010))
#define PIC_PC_BSLMPCR2_L_MAX_PIX_VAL_COLOR2(x) (((x) & 0xffff) << 0)

#define PIC_PC_BSRSVCR            ((volatile uint32_t *)(ICE_BASE+0x88014))
#define PIC_PC_BSRSVCR_R_SLOPE_VAL_COLOR0_OR_EVEN(x) (((x) & 0xff) << 16)
#define PIC_PC_BSRSVCR_R_SLOPE_VAL_COLOR1_OR_ODD(x) (((x) & 0xff) << 8)
#define PIC_PC_BSRSVCR_R_SLOPE_VAL_COLOR2(x) (((x) & 0xff) << 0)

#define PIC_PC_BSRSPCR0           ((volatile uint32_t *)(ICE_BASE+0x88018))
#define PIC_PC_BSRSPCR0_R_START_PIX_VAL_COLOR0_OR_EVEN(x) (((x) & 0xffff) << 0)

#define PIC_PC_BSRSPCR1           ((volatile uint32_t *)(ICE_BASE+0x8801c))
#define PIC_PC_BSRSPCR1_R_START_PIX_VAL_COLOR1_OR_ODD(x) (((x) & 0xffff) << 0)

#define PIC_PC_BSRSPCR2           ((volatile uint32_t *)(ICE_BASE+0x88020))
#define PIC_PC_BSRSPCR2_R_START_PIX_VAL_COLOR2(x) (((x) & 0xffff) << 0)

#define PIC_PC_PDCR1              ((volatile uint32_t *)(ICE_BASE+0x88024))
#define PIC_PC_PDCR1_PRNU_COLOR2(x) (((x) & 0x3) << 30)
#define PIC_PC_PDCR1_PRNU_COLOR1(x) (((x) & 0x3) << 28)
#define PIC_PC_PDCR1_PRNU_COLOR0(x) (((x) & 0x3) << 26)
#define PIC_PC_PDCR1_TOTAL_BITS(x) (((x) & 0x7) << 23)
#define PIC_PC_PDCR1_DSNU_COLOR2(x) (((x) & 0xf) << 19)
#define PIC_PC_PDCR1_TOTAL_COLOR2(x) (((x) & 0x7) << 16)
#define PIC_PC_PDCR1_DSNU_COLOR1(x) (((x) & 0xf) << 11)
#define PIC_PC_PDCR1_TOTAL_COLOR1(x) (((x) & 0x7) << 8)
#define PIC_PC_PDCR1_DSNU_COLOR0(x) (((x) & 0xf) << 3)
#define PIC_PC_PDCR1_TOTAL_COLOR0(x) (((x) & 0x7) << 0)

// granite doesn't have Additional Gain regs - scapri 11-July-2013
//#define PIC_PC_AGCR0              ((volatile uint32_t *)(ICE_BASE+0x88028))
#define PIC_PC_AGCR0_ADDGAINDEC   (0x1 << 14)
#define PIC_PC_AGCR0_ADDGAINFRAC  (0x3fff << 0)
/* PIC Additional Gain register bits */
    /* PIC Additional Gain config register bits for color 0 */
#define PIC_PC_AGCR_MAX   (1<<14)


#define PIC_PC_AGCR1              ((volatile uint32_t *)(ICE_BASE+0x8802c))
#define PIC_PC_AGCR1_ADDGAINDEC   (0x1 << 14)
#define PIC_PC_AGCR1_ADDGAINFRAC  (0x3fff << 0)

#define PIC_PC_AGCR2              ((volatile uint32_t *)(ICE_BASE+0x88030))
#define PIC_PC_AGCR2_ADDGAINDEC   (0x1 << 14)
#define PIC_PC_AGCR2_ADDGAINFRAC  (0x3fff << 0)

#define PIC_PC_PDCR2              ((volatile uint32_t *)(ICE_BASE+0x88034))
#define PIC_PC_PDCR2_CC           (0x1 << 16)
#define PIC_PC_PDCR2_IGSTAG       (0x1 << 9)
#define PIC_PC_PDCR2_BYALL        (0x1 << 8)
#define PIC_PC_PDCR2_BYEXP        (0x1 << 3)
#define PIC_PC_PDCR2_BYPRNU       (0x1 << 2)
#define PIC_PC_PDCR2_BYDSNU       (0x1 << 1)
#define PIC_PC_PDCR2_BYBAD        (0x1 << 0)
#define PIC_PC_PDCR2_BYEXP        (0x1 << 3)
#define PIC_PC_PDCR2_BYPRNU       (0x1 << 2)
#define PIC_PC_PDCR2_BYDSNU       (0x1 << 1)
#define PIC_PC_PDCR2_BYBAD        (0x1 << 0)

#define PIC_PC_PONCR              ((volatile uint32_t *)(ICE_BASE+0x88038))
#define PIC_PC_PONCR_PON_N(x)     (((x) & 0x7) << 13)
#define PIC_PC_PONCR_PON_NORM(x)  (((x) & 0x1fff) << 0)

#define PIC_WDMA_DWOCR            ((volatile uint32_t *)(ICE_BASE+0x90000))
#define PIC_WDMA_DWOCR_DISABLEMODE(x) (((x) & 0x3) << 16)
#define PIC_WDMA_DWOCR_DM_NO_ACCEPT 0 ///< Don't accept data from the Scan 
#define PIC_WDMA_DWOCR_DM_DISCARD   1 ///< Accept and discard data from Scan 
#define PIC_WDMA_DWOCR_DM_ACCEPT    2 ///< Accept and process data from Scan 


#define PIC_WDMA_DWRTBL           ((volatile uint32_t *)(ICE_BASE+0x90004))
#define PIC_WDMA_DWRTBL_MODD(x)   (((x) & 0xf) << 28)
#define PIC_WDMA_DWRTBL_CODD_2(x) (((x) & 0xf) << 24)
#define PIC_WDMA_DWRTBL_CODD_1(x) (((x) & 0xf) << 20)
#define PIC_WDMA_DWRTBL_CODD_0(x) (((x) & 0xf) << 16)
#define PIC_WDMA_DWRTBL_MEVEN(x)  (((x) & 0xf) << 12)
#define PIC_WDMA_DWRTBL_CEVEN_2(x) (((x) & 0xf) << 8)
#define PIC_WDMA_DWRTBL_CEVEN_1(x) (((x) & 0xf) << 4)
#define PIC_WDMA_DWRTBL_CEVEN_0(x) (((x) & 0xf) << 0)

#ifdef HAVE_NSENSOR_SUPPORT
#define PIC_DMA_CHANNEL_CEVEN_0  0 
#define PIC_DMA_CHANNEL_CEVEN_1  2 
#define PIC_DMA_CHANNEL_CEVEN_2  4 
#else
#define PIC_DMA_CHANNEL_CEVEN_0  0 
#define PIC_DMA_CHANNEL_CEVEN_1  1 
#define PIC_DMA_CHANNEL_CEVEN_2  2 
#endif
#define PIC_DMA_CHANNEL_MEVEN   0 
#define PIC_DMA_CHANNEL_CODD_0  1 
#define PIC_DMA_CHANNEL_CODD_1  3 
#define PIC_DMA_CHANNEL_CODD_2  5 
#define PIC_DMA_CHANNEL_MODD    1 


#define PIC_WDMA_DWCIS            ((volatile uint32_t *)(ICE_BASE+0x90008))
#define PIC_WDMA_DWCIS_SELECT     (0x3f << 0)

#define PIC_WDMA_CFG              ((volatile uint32_t *)(ICE_BASE+0x91000))
#define PIC_WDMA_CFG_SERPENTINE   (0x1 << 21)
#define PIC_WDMA_CFG_LINE_REV     (0x1 << 20)
#define PIC_WDMA_CFG_TRAN_REV     (0x1 << 19)
#define PIC_WDMA_CFG_UPPER_HALF   (0x1 << 18)
#define PIC_WDMA_CFG_MSB_IN       (0x1 << 17)
#define PIC_WDMA_CFG_ENABLE       (0x1 << 16)
#define PIC_WDMA_CFG_IN_WIDTH(x)  (((x) & 0x7) << 8)
#define PIC_WDMA_CFG_IN_WIDTH_1_BIT 0
#define PIC_WDMA_CFG_IN_WIDTH_2_BIT 1
#define PIC_WDMA_CFG_IN_WIDTH_4_BIT 2
#define PIC_WDMA_CFG_IN_WIDTH_8_BIT 3
#define PIC_WDMA_CFG_IN_WIDTH_16_BIT 4
#define PIC_WDMA_CFG_IN_WIDTH_32_BIT 5
#define PIC_WDMA_CFG_IN_WIDTH_24_BIT 6
#define PIC_WDMA_CFG_GET_IN_WIDTH(x) (((x) & (0x7<<8)) >> 8)
#define PIC_WDMA_CFG_BURST_LEN(x) (((x) & 0x3) << 0)
#define PIC_WDMA_CFG_BURST_LEN_4_WORD 0
#define PIC_WDMA_CFG_BURST_LEN_8_WORD 1
#define PIC_WDMA_CFG_BURST_LEN_16_WORD 2
/* reset values for a few registers (used to sanity test hardware addresses) */
#define PIC_WDMA_CFG_R      0x00060000

/* how far in bytes each DMA channels registers are offset from each other */
#define PIC_WDMA_CHANNEL_REGS_OFFSET  0x1000

#define PIC_WDMA_STATUS           ((volatile uint32_t *)(ICE_BASE+0x91004))
#define PIC_WDMA_STATUS_DESC_OWN  (0x1 << 10)
#define PIC_WDMA_STATUS_DESC_EOI  (0x1 << 9)
#define PIC_WDMA_STATUS_DESC_SOI  (0x1 << 8)
#define PIC_WDMA_STATUS_REVERSE   (0x1 << 7)
#define PIC_WDMA_STATUS_SOFTRESET (0x1 << 6)
#define PIC_WDMA_STATUS_FULL_CBUF (0x1 << 5)
#define PIC_WDMA_STATUS_EMPTY_CBUF (0x1 << 4)
#define PIC_WDMA_STATUS_FULL_DBUF (0x1 << 3)
#define PIC_WDMA_STATUS_EMPTY_DBUF (0x1 << 2)
#define PIC_WDMA_STATUS_PACKER_EMPTY (0x1 << 1)
#define PIC_WDMA_STATUS_DMA_BUSY  (0x1 << 0)
/* reset values for a few registers (used to sanity test hardware addresses) */
#define PIC_WDMA_STATUS_R   0x00000016


#define PIC_WDMA_LINE_SIZE        ((volatile uint32_t *)(ICE_BASE+0x91008))
#define PIC_WDMA_LINE_SIZE_LINE_SIZE (0x3ffff << 0)

#define PIC_WDMA_INT_EN           ((volatile uint32_t *)(ICE_BASE+0x9100c))
#define PIC_WDMA_INT_EN_RRESP     (0x1 << 10)
#define PIC_WDMA_INT_EN_BRESP     (0x1 << 9)
#define PIC_WDMA_INT_EN_RST       (0x1 << 8)
#define PIC_WDMA_INT_EN_RST       (0x1 << 8)
#define PIC_WDMA_INT_EN_EOI       (0x1 << 7)
#define PIC_WDMA_INT_EN_DIR       (0x1 << 6)
#define PIC_WDMA_INT_EN_CL_ALI    (0x1 << 5)
#define PIC_WDMA_INT_EN_EOL_ALI   (0x1 << 4)
#define PIC_WDMA_INT_EN_EOI_ALI   (0x1 << 3)
#define PIC_WDMA_INT_EN_EOI_ERR   (0x1 << 2)
#define PIC_WDMA_INT_EN_FIN       (0x1 << 1)
#define PIC_WDMA_INT_EN_WHO       (0x1 << 0)

#define PIC_WDMA_INT_PEND         ((volatile uint32_t *)(ICE_BASE+0x91010))
#define PIC_WDMA_INT_PEND_RRESP   (0x1 << 10)
#define PIC_WDMA_INT_PEND_BRESP   (0x1 << 9)
#define PIC_WDMA_INT_PEND_RST     (0x1 << 8)
#define PIC_WDMA_INT_PEND_RST     (0x1 << 8)
#define PIC_WDMA_INT_PEND_EOI     (0x1 << 7)
#define PIC_WDMA_INT_PEND_DIR     (0x1 << 6)
#define PIC_WDMA_INT_PEND_CL_ALI  (0x1 << 5)
#define PIC_WDMA_INT_PEND_EOL_ALI (0x1 << 4)
#define PIC_WDMA_INT_PEND_EOI_ALI (0x1 << 3)
#define PIC_WDMA_INT_PEND_EOI_ERR (0x1 << 2)
#define PIC_WDMA_INT_PEND_FIN     (0x1 << 1)
#define PIC_WDMA_INT_PEND_WHO     (0x1 << 0)

#define PIC_WDMA_INT_ACK          ((volatile uint32_t *)(ICE_BASE+0x91014))
#define PIC_WDMA_INT_ACK_RRESP    (0x1 << 10)
#define PIC_WDMA_INT_ACK_BRESP    (0x1 << 9)
#define PIC_WDMA_INT_ACK_RST      (0x1 << 8)
#define PIC_WDMA_INT_ACK_RST      (0x1 << 8)
#define PIC_WDMA_INT_ACK_EOI      (0x1 << 7)
#define PIC_WDMA_INT_ACK_DIR      (0x1 << 6)
#define PIC_WDMA_INT_ACK_CL_ALI   (0x1 << 5)
#define PIC_WDMA_INT_ACK_EOL_ALI  (0x1 << 4)
#define PIC_WDMA_INT_ACK_EOI_ALI  (0x1 << 3)
#define PIC_WDMA_INT_ACK_EOI_ERR  (0x1 << 2)
#define PIC_WDMA_INT_ACK_FIN      (0x1 << 1)
#define PIC_WDMA_INT_ACK_WHO      (0x1 << 0)

#define PIC_WDMA_INT_FO           ((volatile uint32_t *)(ICE_BASE+0x91018))
#define PIC_WDMA_INT_FO_RRESP     (0x1 << 10)
#define PIC_WDMA_INT_FO_BRESP     (0x1 << 9)
#define PIC_WDMA_INT_FO_RST       (0x1 << 8)
#define PIC_WDMA_INT_FO_RST       (0x1 << 8)
#define PIC_WDMA_INT_FO_EOI       (0x1 << 7)
#define PIC_WDMA_INT_FO_DIR       (0x1 << 6)
#define PIC_WDMA_INT_FO_CL_ALI    (0x1 << 5)
#define PIC_WDMA_INT_FO_EOL_ALI   (0x1 << 4)
#define PIC_WDMA_INT_FO_EOI_ALI   (0x1 << 3)
#define PIC_WDMA_INT_FO_EOI_ERR   (0x1 << 2)
#define PIC_WDMA_INT_FO_FIN       (0x1 << 1)
#define PIC_WDMA_INT_FO_WHO       (0x1 << 0)

#define PIC_WDMA_DESC_WRITE       ((volatile uint32_t *)(ICE_BASE+0x9101c))
#define PIC_WDMA_DESC_WRITE_DESC  (0x3fffffff << 2)

#define PIC_WDMA_DESC_READ        ((volatile uint32_t *)(ICE_BASE+0x91020))
#define PIC_WDMA_DESC_READ_DESC   (0xffffffff << 0)

#define PIC_WDMA_XFER_LENGTH      ((volatile uint32_t *)(ICE_BASE+0x91024))
#define PIC_WDMA_XFER_LENGTH_LEN  (0xffffffff << 0)

#define PIC_WDMA_XFER_ADDR        ((volatile uint32_t *)(ICE_BASE+0x91028))
#define PIC_WDMA_XFER_ADDR_ADDR   (0xffffffff << 0)

#define PIC_WDMA_XFER_BURST       ((volatile uint32_t *)(ICE_BASE+0x9102c))
#define PIC_WDMA_XFER_BURST_BURST (0x1f << 0)

#define PIC_WDMA_DATA_VAL         ((volatile uint32_t *)(ICE_BASE+0x91030))
#define PIC_WDMA_DATA_VAL_DATA_VALID (0xffffffff << 0)

#define PIC_WDMA_RESET            ((volatile uint32_t *)(ICE_BASE+0x91034))
#define PIC_WDMA_RESET_SOFT_RESET (0x1 << 0)
/* davep 24-Mar-2010 ; temp hack to be able to build until I can rewrite PIC
 * WDMA with the new chained stuff.
 */
#define PIC_WDMA_OFFSET 0
#define PIC_WDMA_START_OFFSET 0


#endif // PICHW_H
