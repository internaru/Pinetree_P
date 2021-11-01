/******************************************************************************
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  *
  *This file contains definitions for PIP
  *
 **/

#ifndef __PIP_FUNC_H_INCLUDED
#define __PIP_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "cr_api.h"
#include "cr_cfg.h"
#include "pipe_access_api.h"
#include "pipe_access_cfg.h"
#include "sccsc_api.h"
#include "serp_api.h"
#include "serp_cfg.h"
#include "tp_api.h"
#include "tp_cfg.h"
#include "mip_test_api.h"
#include "mip_cfg.h"
#include "yipi_api.h"
#include "yipi_cfg.h"
#include "regAddrs.h"
#include "logger.h"
#include "idma_api.h"
#include "ahs_api.h"
#include "fhs_api.h"
#include "csc_api.h"
#include "nga_api.h"
#include "fhs_cfg.h"

extern ZX_REGS_t                *zx_reg;
extern JBIG_CODEC_REGS_t        *jbig_codec_reg;
extern JBIG_DECODE_REGS_t       *jbig_decode_reg;
extern JBIG_CODEC_ODMA_REGS_t   *jbig_codec_odma_reg;
extern JBIG_CODEC_IDMA_REGS_t   *jbig_codec_idma_reg;
extern JBIG_DECODE_ODMA_REGS_t  *jbig_decode_odma_reg;
extern JBIG_DECODE_IDMA_REGS_t  *jbig_decode_idma_reg;
extern PIP_REGS_t               *pip_reg_u;
extern PIP_REGS_t               *pip_reg_l;
extern TP_TEXT_REGS_t           *tp_reg;
extern TP_CONTONE_REGS_t        *cp_reg;
extern CR_REGS_t                *cr_reg;
extern CSC_DAVINCI_REGS_t       *csc_dav_reg;
extern NGADJUST_DAVINCI_REGS_t  *nga_reg;
extern SCCSC_REGS_t             *sc_csc_reg;
extern SERP_REGS_t              *serp_reg;
#ifdef ASIC_gemstone
extern UED_BASIC_REGS_t         *ued_reg;
extern CRE_RASTER_REGS_t        *cre_reg;
#else
extern UED_ADVANCED_REGS_t      *ued_reg;
extern CRE_REGS_t               *cre_reg;
extern IDMA_REGS_t              *pip_lpc_idma0_reg;
extern IDMA_REGS_t              *pip_lpc_idma1_reg;
extern IDMA_REGS_t              *pip_lpc_idma2_reg;
extern IDMA_REGS_t              *pip_lpc_idma3_reg;
#endif
extern MIP_REGSET_REGS_t        *mip_reg;
extern YIPI_REGS_t              *yipi_reg;
extern PIP_STATS_REGS_t         *stat_reg;
extern KMOD_REGS_t              *kmod_reg;
extern PIPE_ACCESS_REGS_t       *pa_reg;
extern CBI2CDMA_REGS_t          *pa_cdma_reg;
extern PA_ODMA_REGS_t           *pa_odma_reg;
extern PA_IDMA_REGS_t           *pa_idma_reg;
extern DAVBUFF_REGS_t           *db_ued_reg;
extern DAVBUFF_REGS_t           *db_kmod_reg;
extern DAVBUFF_REGS_t           *db_serp_reg;
extern FHS_REGS_t               *fhs_reg;
extern ODMA_SLI_REGS_t          *odma_dav_reg[8];
extern CBI2CDMA_REGS_t          *stat_cdma0;
extern CBI2CDMA_REGS_t          *stat_cdma1;
extern CBI2CDMA_REGS_t          *stat_cdma2;
#if (defined(ASIC_gemstone) && ASIC_REV >= ASIC_REV_B0)
extern SHARPEN_REGS_t           *sharpen_reg;
extern CTWIN_REGS_t             *ctwin_reg;
extern TEMS_REGS_t              *tems_reg;
#endif

extern CORE_REGS_t              *th0_reg;
extern SRAM_REGS_t              *th0_sram_reg;
extern CORE_REGS_t              *th1_reg;
extern SRAM_REGS_t              *th1_sram_reg;
extern CORE_REGS_t              *th2_reg;
extern SRAM_REGS_t              *th2_sram_reg;
extern CORE_REGS_t              *th3_reg;
extern SRAM_REGS_t              *th3_sram_reg;

// include images
extern uint32_t image[];
extern const uint64_t colorTable [4913];

void pip_setupregs(void);
void pip_setbypass(void);

#endif
