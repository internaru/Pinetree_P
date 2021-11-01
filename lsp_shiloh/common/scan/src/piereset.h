/*
 * ============================================================================
 * Copyright (c) 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 *
 * Description:
 *
 * Documented power-on reset values for various PIE registers
 *
 *****************************************************************************/

#ifndef PIERESET_H
#define PIERESET_H

/* 
 *  Common registers have nothing needing reset.
 */

/*
 *  Read DMA
 */
#define PIE_RDMA_CCR_R   0x00000001
#define PIE_RDMA_CSR_R   0
#define PIE_RDMA_DCR_R   0
#define PIE_RDMA_DAR1_R  0
#define PIE_RDMA_DAR2_R  0
#define PIE_RDMA_DRCR1_R 0
#define PIE_RDMA_DRCR2_R 0
#define PIE_RDMA_DBCR_R  0
#define PIE_RDMA_DTCR_R  0

/*
 *  Bit depth enhance
 */
#define PIE_BDE_CFG_R 0x00000001


/* 
 *  RGB2esRGB
 */
#define PIE_CSC_RGB_CCR_R 0x00000001


/*
 *  esRGB2YCrCb
 */
#define PIE_CSC_ESR_CCR_R 0x00000001

/*
 *  YCrCb2esRGB
 */
#define PIE_CSC_YCC_CCR_R 0x00000001

/* 
 *  Color plane adjust
 */
#define PIE_CPA_CR_R 0x00000001


/* 
 *  Tone/contrast & neutral sensitivity
 */
#define PIE_TCNS_CR_R 0x000000001


/* 
 *  Pogo aligner
 */

#define PIE_ALIGN_PACR_R  0x00000306


/* 
 *  Multifilter Y/CrCb & Text Enhance
 */

#define PIE_FILTER_SCR1_R  0
#define PIE_FILTER_SCR2_R  0x00000100
#define PIE_FILTER_TEL1R_R 0
#define PIE_FILTER_TEL2R_R 0
#define PIE_FILTER_TEL3R_R 0


/* 
 *  X-Y Scale
 */

#define PIE_SCALE_CFG_R     0x00000101
#define PIE_SCALE_INXY_R             0
#define PIE_SCALE_OUTX_R             0
#define PIE_SCALE_FILLC1C2_R         0
#define PIE_SCALE_FILLC0_R           0
#define PIE_SCALE_SCALEX_R           0
#define PIE_SCALE_SCALEY_R           0
#define PIE_SCALE_SXMN_R             0
#define PIE_SCALE_SYMN_R             0
#define PIE_SCALE_TX_R               0
#define PIE_SCALE_TY_R               0
#define PIE_SCALE_INITYPER_R 0x00010000

/*
 *  Mono HalfPack
 */

#define PIE_HP_CFG_R   0x00000020
#define PIE_HP_ALGO_R  0
#define PIE_HP_BIAS_R  0
/* XXX should we reset the SRAMs as well? */

/* HalfPack read/write channel reset values */
#define PIE_HP_DMA_DAR_R 0
#define PIE_HP_DMA_LWR_R 0
#define PIE_HP_DMA_SCR_R 0
#define PIE_HP_DMA_TCR_R 0


/* 
 *  Write DMA
 */
/* 22-Mar-05 ; do NOT set rows to zero at any time. Causes serious problems
 * with the Write DMA state machines if PIE is reset while rows is zero. As of
 * this writing, the MA is incorrect and a new version is forthcoming.
 */
//#define PIE_WDMA_DGCR_R 0
#define PIE_WDMA_DGCR_R 0x0000000f

#define PIE_WDMA_DCR_R  0
#define PIE_WDMA_DAR_R  0
#define PIE_WDMA_DRCR_R 0 
#define PIE_WDMA_DBCR_R 0 
#define PIE_WDMA_DTCR_R 0 

#endif

