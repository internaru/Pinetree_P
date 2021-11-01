/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Agilent Technologies, Inc. All Rights Reserved
 *                      
 *                         Agilent Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef PICRESET_H
#define PICRESET_H

// PIC common block
#define PIC_CI_CBIR_R 0
#define PIC_CI_IENR_R 0
#define PIC_CI_IPR_R  0
#define PIC_CI_ITR_R  0

/*
 * PIC Bulb Shadow
 * based on Version 0.0 (3/21/05) of the PIC MA document
 */
#define PIC_BS_RLSGC_R  0
#define PIC_BS_LSVC_R   0
#define PIC_BS_LMPCR0_R 0x00000200
#define PIC_BS_LMPCR1_R 0x00000200
#define PIC_BS_LMPCR2_R 0x00000200
#define PIC_BS_RSVC_R   0
#define PIC_BS_RSPCR0_R 0x00000200
#define PIC_BS_RSPCR1_R 0x00000200
#define PIC_BS_RSPCR2_R 0x00000200

/*
 * PIC Additional Gain & PONCR
 * based on the PIC HLD version 0.0 (3/21/05)
 */
#define PIC_AGCR0_R 0x00004000
#define PIC_AGCR1_R 0x00004000
#define PIC_AGCR2_R 0x00004000

#define PIC_PONC_R  0x00000038
 
/*
 * PIC ADC
 * based on MA_ADCNorm.doc "Version 1.7"
 */
#define PIC_ADCN_ANR_R  0x0000010d
#define PIC_ADCN_ANA_R  0
#define PIC_ADCN_ANT_R  0

/* 
 * PIC BM 
 * based on MA_BulbMonLRMargin.doc "Issue Date: 04-07-2004"
 */
#define PIC_BM_BMCR_R (1<<31)
#define PIC_BM_LRMR0_R 0  
#define PIC_BM_LRMR1_R 0  
#define PIC_BM_LRMR2_R 0
#define PIC_BM_BMR0_R  0
#define PIC_BM_BMR1_R  0
#define PIC_BM_BMR2_R  0
#define PIC_BM_BMR3_R  0

/*
 * PIC Pixel Correction
 * based on PixelCorrection.doc 0.3
 */
#define PIC_PC_PDCR1_R 0
#define PIC_PC_PDCR2_R 0x00000100

/* PIC PRNU/DSNU DMA
 * based on PIC HLD version 0.0 (3/21/05)
 */
#define PIC_PC_RDMA_PDCR_R   0
#define PIC_PC_RDMA_PDAR_R   0
#define PIC_PC_RDMA_PDTCR_R  0
#define PIC_PC_RDMA_PDSTR_R  0
#define PIC_PC_RDMA_PDCAR_R  0
#define PIC_PC_RDMA_PDCTCR_R 0
#define PIC_PC_RDMA_PDSR_R   2
#define PIC_PC_RDMA_DRR_R    0
#define PIC_PC_PDIR_R        0

/*
 * PIC HS
 * based on MA_Hscale.doc version 0.3 
 */
#define PIC_HS_HSCR_R   1
#define PIC_HS_HSSR_R   0x00010000

/* PIC Bit Depth Reduction
 * based on PIC HLD version 0.0 (3/21/05)
 */
#define PIC_BDR_BDRC_R 1

/* 
 * PIC WDMA
 * based on PICWDMA_MA.doc version 0.91
 */
#define PIC_WDMA_DWOCR_R    0x00020000
#define PIC_WDMA_DWRTBL_R   0x15310420
#define PIC_WDMA_DWCIS_R    0

#endif

