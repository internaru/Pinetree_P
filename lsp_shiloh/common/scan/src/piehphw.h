/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef PIEHPHW_H
#define PIEHPHW_H

//-*******************************
// Mono HalfPack block
// HalfPack_MA.doc
//-*******************************

#define PIE_HP_RDMA_DCR_OFFSET    0x00 /* read DMA Configuration Register */
#define PIE_HP_RDMA_DAR_OFFSET    0x04 /* read DMA Address Register */
#define PIE_HP_RDMA_LWR_OFFSET    0x08 /* read DMA Line Width Register */
#define PIE_HP_RDMA_SCR_OFFSET    0x0c /* read DMA Strip Configuration Register */
#define PIE_HP_RDMA_TCR_OFFSET    0x10 /* read DMA Transfer Count Register */
#define PIE_HP_RDMA_STR_OFFSET    0x14 /* read DMA Start Register */
#define PIE_HP_RDMA_ASR_OFFSET    0x18 /* read DMA Address Status Register */
#define PIE_HP_RDMA_TCRR_OFFSET   0x1c /* read DMA Transfer Count Remaining register */
#define PIE_HP_RDMA_DSR_OFFSET    0x20 /* read DMA Status Register */
#define PIE_HP_RDMA_DRR_OFFSET    0x24 /* read DMA Results Register */
#define PIE_HP_RDMA_DIR_OFFSET    0x28 /* read DMA Interrupt Register */
#define PIE_HP_CFG_OFFSET       0x4000 /* Configuration register */
#define PIE_HP_ALGO_OFFSET      0x4004 /* Algorithm register */
#define PIE_HP_BIAS_OFFSET      0x4008 /* Bias register */
#define PIE_HP_SIZE_OFFSET      0x5000 /* size SRAM */
#ifdef HAVE_HALFPACK_SZDD
#define PIE_HP_SZDD_OFFSET      0x5000 /* unified Size, Diffusion, Density SRAM */
#endif
#define PIE_HP_SBIAS_OFFSET     0x6000 /* signed bias SRAM */
#define PIE_HP_DENSITY_OFFSET   0x7000 /* density SRAM */
#define PIE_HP_ERROR_DIFF_OFFSET 0x7800 /* error diffusion SRAM */
#define PIE_HP_ERROR_OFFSET     0x8000 /* error SRAM */
#define PIE_HP_WDMA_DCR_OFFSET  0xc000 /* write DMA Configuration Register */
#define PIE_HP_WDMA_DAR_OFFSET  0xc004 /* write DMA Address Register */
#define PIE_HP_WDMA_LWR_OFFSET  0xc008 /* write DMA Line Width Register */
#define PIE_HP_WDMA_SCR_OFFSET  0xc00c /* write DMA Strip Configuration Register */
#define PIE_HP_WDMA_TCR_OFFSET  0xc010 /* write DMA Transfer Count Register */
#define PIE_HP_WDMA_STR_OFFSET  0xc014 /* write DMA Start Register */
#define PIE_HP_WDMA_ASR_OFFSET  0xc018 /* write DMA Address Status Register */
#define PIE_HP_WDMA_TCRR_OFFSET 0xc01c /* write DMA Transfer Count Remaining register */
#define PIE_HP_WDMA_DSR_OFFSET  0xc020 /* write DMA Status Register */
#define PIE_HP_WDMA_DRR_OFFSET  0xc024 /* write DMA Results Register */
#define PIE_HP_WDMA_DIR_OFFSET  0xc028 /* write DMA Interrupt Register */

#define PIE_HP_RDMA_DCR   ((volatile UINT32 *)(PIE_HP_RDMA_DCR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_DAR   ((volatile UINT32 *)(PIE_HP_RDMA_DAR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_LWR   ((volatile UINT32 *)(PIE_HP_RDMA_LWR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_SCR   ((volatile UINT32 *)(PIE_HP_RDMA_SCR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_TCR   ((volatile UINT32 *)(PIE_HP_RDMA_TCR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_STR   ((volatile UINT32 *)(PIE_HP_RDMA_STR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_ASR   ((volatile UINT32 *)(PIE_HP_RDMA_ASR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_TCRR  ((volatile UINT32 *)(PIE_HP_RDMA_TCRR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_DSR   ((volatile UINT32 *)(PIE_HP_RDMA_DSR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_DRR   ((volatile UINT32 *)(PIE_HP_RDMA_DRR_OFFSET + PIE_HP_BASE))
#define PIE_HP_RDMA_DIR   ((volatile UINT32 *)(PIE_HP_RDMA_DIR_OFFSET + PIE_HP_BASE))
#define PIE_HP_CFG        ((volatile UINT32 *)(PIE_HP_CFG_OFFSET + PIE_HP_BASE))
#define PIE_HP_ALGO       ((volatile UINT32 *)(PIE_HP_ALGO_OFFSET + PIE_HP_BASE))
#define PIE_HP_BIAS       ((volatile UINT32 *)(PIE_HP_BIAS_OFFSET + PIE_HP_BASE))
#define PIE_HP_SIZE       ((volatile UINT32 *)(PIE_HP_SIZE_OFFSET + PIE_HP_BASE))
#define PIE_HP_SBIAS      ((volatile UINT32 *)(PIE_HP_SBIAS_OFFSET + PIE_HP_BASE))
#define PIE_HP_DENSITY    ((volatile UINT32 *)(PIE_HP_DENSITY_OFFSET + PIE_HP_BASE))
#define PIE_HP_ERROR_DIFF ((volatile UINT32 *)(PIE_HP_ERROR_DIFF_OFFSET + PIE_HP_BASE))
#define PIE_HP_ERROR      ((volatile UINT32 *)(PIE_HP_ERROR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_DCR   ((volatile UINT32 *)(PIE_HP_WDMA_DCR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_DAR   ((volatile UINT32 *)(PIE_HP_WDMA_DAR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_LWR   ((volatile UINT32 *)(PIE_HP_WDMA_LWR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_SCR   ((volatile UINT32 *)(PIE_HP_WDMA_SCR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_TCR   ((volatile UINT32 *)(PIE_HP_WDMA_TCR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_STR   ((volatile UINT32 *)(PIE_HP_WDMA_STR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_ASR   ((volatile UINT32 *)(PIE_HP_WDMA_ASR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_TCRR  ((volatile UINT32 *)(PIE_HP_WDMA_TCRR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_DSR   ((volatile UINT32 *)(PIE_HP_WDMA_DSR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_DRR   ((volatile UINT32 *)(PIE_HP_WDMA_DRR_OFFSET + PIE_HP_BASE))
#define PIE_HP_WDMA_DIR   ((volatile UINT32 *)(PIE_HP_WDMA_DIR_OFFSET + PIE_HP_BASE))

/* davep 29-Dec-2005 ; HalfPack's LUTs are the same size but let's enumerate
 * them anyway 
 */
#define PIE_HP_SIZE_LUT_SIZE  256   /**< number of 32-bit entries in HalfPack Size LUT */
#define PIE_HP_SBIAS_LUT_SIZE 256   /**< number of 32-bit entries in HalfPack Signed Bias LUT */
#define PIE_HP_DENSITY_LUT_SIZE 256 /**< number of 32-bit entries in HalfPack Density LUT */
#define PIE_HP_DIFFUS_LUT_SIZE 256  /**< number of 32-bit entries in HalfPack Diffusion LUT */

#ifdef PIE_HP_SZDD_OFFSET
/* unified Size, Density, and Diffusion SRAM */
#define PIE_HP_SZDD_LUT  ((volatile UINT32 *)(PIE_HP_SZDD_OFFSET + PIE_HP_BASE))
#define PIE_HP_SZDD_LUT_SIZE 256  /**< number of 32-bit entries in HalfPack SZDD LUT */

#define PIE_HP_SZDD_DENSITY_FIELD(x)  ( (x) & 0x000000ff )
#define PIE_HP_SZDD_DIFFUS_FIELD(x)  ( ((x) & 0x000000ff) << 8 )
#define PIE_HP_SZDD_SIZE_FIELD(x)  ( ((x) & 0x0000000f) << 16 )
#endif

/* Note: a lot of these DMA registers' fields are the same as PIE's read/write
 * DMA fields. However, Mono HalfPack is its own module inside PIE so they have
 * their own register definitions just in case somebody somewhere decides to
 * start deviating HalfPack and PIE.
 */
/* HalfPack Configuration register fields */
#define PIE_HP_CFG_RESET          (1<<0) /* 1=soft reset active */
#define PIE_HP_CFG_CPU_ACCESS     (1<<1)
#define PIE_HP_CFG_BITDEPTH_1BIT  (0<<2) /* 1-bit output data */
#define PIE_HP_CFG_BITDEPTH_2BIT  (1<<2) /* 2-bit output data */
#define PIE_HP_CFG_BITDEPTH_4BIT  (2<<2) /* 4-bit output data */
#define PIE_HP_CFG_HORIZ_REPL     (1<<4) /* horizontal replicate 2x */
#define PIE_HP_CFG_BYPASS         (1<<5) /* 1=bypass block */
#ifdef HAVE_HALFPACK_FULL_PIXEL_DIFFUSION
#define PIE_HP_CFG_PAIRED_ERRDIFF (0<<6) /* pixel pair error diffusion */
#define PIE_HP_CFG_NORMAL_ERRDIFF (1<<6) /* normal (non-paired) error diffusion */
#endif

/* HalfPack Algorithm register fields */
#define PIE_HP_ALGO_INVERT      (1<<0)
#define PIE_HP_ALGO_ACCUM_ERR   (1<<1)
#define PIE_HP_ALGO_BIAS_IDX    (1<<2)
#define PIE_HP_ALGO_BIAS_DOT    (1<<3)
#define PIE_HP_ALGO_DO_A_DOT    (1<<4)
#define PIE_HP_SHIFT_VALUE(x)   ((x&3)<<6)
#define PIE_HP_ALGO_ERR_WT_1(x) ((x&0xf)<<8)
#define PIE_HP_ALGO_ERR_WT_2(x) ((x&0xf)<<16)
#define PIE_HP_ALGO_ERR_WT_3(x) ((x&0xf)<<24)

/* HalfPack Bias register fields */
#define PIE_HP_BIAS_SRAM_CFG (1<<0)
#define PIE_HP_BIAS_REG0(x)  ((x&0xff)<<8)
#define PIE_HP_BIAS_REG1(x)  ((x&0xff)<<16)

/* HalfPack DMA Configuration registers fields (same for both read and write DMA) */
#define PIE_HP_DMA_DCR_EN       (1<<0)  /* 1=DMA enabled, 0=DMA disabled */
#define PIE_HP_DMA_DCR_RESET    (1<<1)  /* 1=soft reset, 0=no action */
#define PIE_HP_DMA_DCR_CIE      (1<<31) /* DMA complete interrupt enable */

/* HalfPack DMA Strip Config registers fields (same for both read and write DMA) */
/* 0=normal (left to right, 0,1,2,3),  1=reverse (right to left, 3,2,1,0) */
#define PIE_HP_DMA_SCR_DIR      (1<<0)  /* DMA address direction */
#define PIE_HP_DMA_SCR_SOI      (1<<1)  /* DMA start-of-image */
#define PIE_HP_DMA_SCR_REPL     (1<<2)  /* vertical replicate 2x */ 

/* HalfPack DMA Status registers fields (same for both read and write DMA) */
#define PIE_HP_DMA_DSR_ACTIVE   (1<<0)  /* 1=DMA active, 0=DMA idle */
#define PIE_HP_DMA_DSR_READY    (1<<1)  /* 1=ready to accept params, 0=busy */
#define PIE_HP_DMA_DSR_CMPL     (1<<31) /* DMA complete interrupt pending */

/* HalfPack DMA Results registers fields (same for both read and write DMA) */
#define PIE_HP_DMA_DRR_XCNT     0xfffff   /* bits for transfer count */
#define PIE_HP_DMA_DRR_ERROR    (1<<22)   /* 1=DMA error, 0=no DMA error */
#define PIE_HP_DMA_DRR_TERM     (1<<23)   /* 1=DMA terminated by hw or fw */

/* HalfPack DMA Interrupt ACK registers fields (same for both read and write DMA) */
#define PIE_HP_DMA_DIR_CLEAR    (1<<0)    /* write bit to 0 to clear PIE read DMA interrupt */

#endif

