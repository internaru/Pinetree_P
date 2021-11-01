/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for PIP.
  * 
 **/

#include <stdint.h>
#include "pip_utils.h"
#include "asic.h"
#include "logger.h"
#include "lassert.h"
#include "ued_api.h"
#include "map_mem.h"

#include "posix_ostools.h"

#define DBG_PRFX "PIP_UTILS: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

// PIP Register Handles
ZX_REGS_t               *zx_reg;
//-jrsJBIG_CODEC_REGS_t       *jbig_codec_reg;
//-jrsJBIG_DECODE_REGS_t      *jbig_decode_reg;
//-jrsJBIG_CODEC_ODMA_REGS_t  *jbig_codec_odma_reg;
//-jrsJBIG_CODEC_IDMA_REGS_t  *jbig_codec_idma_reg;
//-jrsJBIG_DECODE_ODMA_REGS_t *jbig_decode_odma_reg;
//-jrsJBIG_DECODE_IDMA_REGS_t *jbig_decode_idma_reg;
PIP_REGS_t              *pip_reg_u;
PIP_REGS_t              *pip_reg_l;
TP_TEXT_REGS_t          *tp_reg;
TP_CONTONE_REGS_t       *cp_reg;
CR_REGS_t               *cr_reg;
CSC_DAVINCI_REGS_t      *csc_dav_reg;
NGADJUST_DAVINCI_REGS_t *nga_reg;
SCCSC_REGS_t            *sc_csc_reg;
SERP_REGS_t             *serp_reg;
#ifdef ASIC_gemstone
UED_BASIC_REGS_t        *ued_reg;
#else
UED_ADVANCED_REGS_t        *ued_reg;
#endif
MIP_REGSET_REGS_t       *mip_reg;
YIPI_REGS_t             *yipi_reg;
PIP_STATS_REGS_t        *stat_reg;
AUTO_HT_REGS_t          *ah_reg;
KMOD_REGS_t             *kmod_reg;
ODMA_SLI_REGS_t         *odma_dav_reg[8];
PIPE_ACCESS_REGS_t      *pa_reg;
CBI2CDMA_REGS_t         *pa_cdma_reg;
PA_ODMA_REGS_t          *pa_odma_reg;   
PA_IDMA_REGS_t          *pa_idma_reg;
#ifdef ASIC_granite
IDMA_REGS_t             *pip_lpc_idma0_reg;
IDMA_REGS_t             *pip_lpc_idma1_reg;
IDMA_REGS_t             *pip_lpc_idma2_reg;
IDMA_REGS_t             *pip_lpc_idma3_reg;
#endif
DAVBUFF_REGS_t          *db_ued_reg;
DAVBUFF_REGS_t          *db_kmod_reg;
DAVBUFF_REGS_t          *db_serp_reg;
FHS_REGS_t              *fhs_reg;
CBI2CDMA_REGS_t         *stat_cdma0;
CBI2CDMA_REGS_t         *stat_cdma1;
CBI2CDMA_REGS_t         *stat_cdma2;
#if ASIC_REV >= ASIC_REV_B0 && ASIC_gemstone
SHARPEN_REGS_t          *sharpen_reg;
CTWIN_REGS_t            *ctwin_reg;
TEMS_REGS_t             *tems_reg;


CRE_RASTER_REGS_t       *cre_reg;
#else
//#define CRE_RASTER_REGS_t CRE_REGS_t
CRE_REGS_t       *cre_reg;
#endif

CORE_REGS_t             *th0_reg;
SRAM_REGS_t             *th0_sram_reg;
CORE_REGS_t             *th1_reg;
SRAM_REGS_t             *th1_sram_reg;
CORE_REGS_t             *th2_reg;
SRAM_REGS_t             *th2_sram_reg;
CORE_REGS_t             *th3_reg;
SRAM_REGS_t             *th3_sram_reg;



/// memory map hardware registers using addresses from regAddrs.h instead of the "linux" uio why since the number of blocks exceeds what a single uio driver can have.
///
///
void pip_setupregs(void)
{
    int fd;
    void *mapBase; // page and structure addresses may be different, we don't unmap which would require keeping the base or computing it.

    if((fd = open("/dev/mem", (O_RDWR | O_SYNC | O_CLOEXEC))) < 0) {
        DBG_PRINTF_CRIT("pip_init()  open('dev/mem') failed ");
        ASSERT(fd == 0);
    }

    zx_reg = (ZX_REGS_t *) mapMem( ZX_BASE, sizeof( ZX_REGS_t ), &mapBase, fd );
    ASSERT( zx_reg );
//-jrs    jbig_codec_reg = (JBIG_CODEC_REGS_t *) mapMem( JBIG_JBIG_CODEC_BASE, sizeof(JBIG_CODEC_REGS_t), &mapBase, fd );
//-jrs    jbig_decode_reg = (JBIG_DECODE_REGS_t *) mapMem( JBIG_JBIG_DECODE_BASE, sizeof(JBIG_DECODE_REGS_t), &mapBase, fd );
//-jrs    jbig_codec_odma_reg = (JBIG_CODEC_ODMA_REGS_t *) mapMem( JBIG_JBIG_CODEC_ODMA_BASE, sizeof(JBIG_CODEC_ODMA_REGS_t), &mapBase, fd );
//-jrs    jbig_codec_idma_reg = (JBIG_CODEC_IDMA_REGS_t *) mapMem( JBIG_JBIG_CODEC_IDMA_BASE, sizeof(JBIG_CODEC_IDMA_REGS_t), &mapBase, fd );
//-jrs    jbig_decode_odma_reg = (JBIG_DECODE_ODMA_REGS_t *) mapMem( JBIG_JBIG_DECODE_ODMA_BASE, sizeof(JBIG_DECODE_ODMA_REGS_t), &mapBase, fd );
//-jrs    jbig_decode_idma_reg = (JBIG_DECODE_IDMA_REGS_t *) mapMem( JBIG_JBIG_DECODE_IDMA_BASE, sizeof(JBIG_DECODE_IDMA_REGS_t), &mapBase, fd );
    pip_reg_u = (PIP_REGS_t *) mapMem( PIP_PIP_BASE, sizeof(PIP_REGS_t), &mapBase, fd );
// lsptodo is this for serenity?
//    pip_reg_l = (PIP_REGS_t *) mapMem( (PIP_BASE + 0x40000), sizeof(PIP_REGS_t), &mapBase, fd );
    tp_reg = (TP_TEXT_REGS_t *) mapMem( PIP_TP_BASE, sizeof(TP_TEXT_REGS_t), &mapBase, fd );
    cp_reg = (TP_CONTONE_REGS_t *) mapMem( PIP_CP_BASE, sizeof(TP_CONTONE_REGS_t), &mapBase, fd );
    cr_reg = (CR_REGS_t *) mapMem( PIP_CR_BASE, sizeof(CR_REGS_t), &mapBase, fd );
    csc_dav_reg = (CSC_DAVINCI_REGS_t *) mapMem( PIP_CSC_BASE, sizeof(CSC_DAVINCI_REGS_t), &mapBase, fd );
    nga_reg = (NGADJUST_DAVINCI_REGS_t *) mapMem( PIP_NGA_BASE, sizeof(NGADJUST_DAVINCI_REGS_t), &mapBase, fd );
    sc_csc_reg = (SCCSC_REGS_t *) mapMem( PIP_SC_CSC_BASE, sizeof(SCCSC_REGS_t), &mapBase, fd );
    serp_reg = (SERP_REGS_t *) mapMem( PIP_SERP_BASE, sizeof(SERP_REGS_t), &mapBase, fd );
    ued_reg = (UED_REGS_t *) mapMem( PIP_UED_BASE, sizeof(UED_REGS_t), &mapBase, fd );
    mip_reg = (MIP_REGSET_REGS_t *) mapMem( PIP_MIP_BASE, sizeof(MIP_REGSET_REGS_t), &mapBase, fd );
    stat_reg = (PIP_STATS_REGS_t *) mapMem( PIP_STATS_BASE, sizeof(PIP_STATS_REGS_t), &mapBase, fd );
    yipi_reg = (YIPI_REGS_t *) mapMem( PIP_YIPI_BASE, sizeof(YIPI_REGS_t), &mapBase, fd );
    //   ah_reg = (AUTO_HT_REGS_t *)PIP_AH_BASE;
    kmod_reg = (KMOD_REGS_t *) mapMem( PIP_KMOD_BASE, sizeof(KMOD_REGS_t), &mapBase, fd );
    odma_dav_reg[0] = (ODMA_SLI_REGS_t *) mapMem( PIP_ODMA0_BASE, sizeof(ODMA_SLI_REGS_t), &mapBase, fd );
    odma_dav_reg[1] = (ODMA_SLI_REGS_t *) mapMem( PIP_ODMA1_BASE, sizeof(ODMA_SLI_REGS_t), &mapBase, fd );
    odma_dav_reg[2] = (ODMA_SLI_REGS_t *) mapMem( PIP_ODMA2_BASE, sizeof(ODMA_SLI_REGS_t), &mapBase, fd );
    odma_dav_reg[3] = (ODMA_SLI_REGS_t *) mapMem( PIP_ODMA3_BASE, sizeof(ODMA_SLI_REGS_t), &mapBase, fd );
    odma_dav_reg[4] = (ODMA_SLI_REGS_t *) mapMem( PIP_ODMA4_BASE, sizeof(ODMA_SLI_REGS_t), &mapBase, fd );
    odma_dav_reg[5] = (ODMA_SLI_REGS_t *) mapMem( PIP_ODMA5_BASE, sizeof(ODMA_SLI_REGS_t), &mapBase, fd );
    odma_dav_reg[6] = (ODMA_SLI_REGS_t *) mapMem( PIP_ODMA6_BASE, sizeof(ODMA_SLI_REGS_t), &mapBase, fd );
    odma_dav_reg[7] = (ODMA_SLI_REGS_t *) mapMem( PIP_ODMA7_BASE, sizeof(ODMA_SLI_REGS_t), &mapBase, fd );
    pa_reg = (PIPE_ACCESS_REGS_t *) mapMem( PIP_PA_BASE, sizeof(PIPE_ACCESS_REGS_t), &mapBase, fd );
    pa_cdma_reg = (CBI2CDMA_REGS_t *) mapMem( PIP_PA_CDMA_BASE, sizeof(CBI2CDMA_REGS_t), &mapBase, fd );
    pa_odma_reg = (PA_ODMA_REGS_t *) mapMem( PIP_PA_PA_ODMA_BASE, sizeof(PA_ODMA_REGS_t), &mapBase, fd );      // not using PA_ODMA_REGS_t so I can call ODMA tasks
    pa_idma_reg = (PA_IDMA_REGS_t *) mapMem( PIP_PA_PA_IDMA_BASE, sizeof(PA_IDMA_REGS_t), &mapBase, fd );
    db_ued_reg = (DAVBUFF_REGS_t *) mapMem( PIP_DB_HT_BASE, sizeof(DAVBUFF_REGS_t), &mapBase, fd );
    db_kmod_reg = (DAVBUFF_REGS_t *) mapMem( PIP_DB_KMOD_BASE, sizeof(DAVBUFF_REGS_t), &mapBase, fd );
    db_serp_reg = (DAVBUFF_REGS_t *) mapMem( PIP_DB_SERP_BASE, sizeof(DAVBUFF_REGS_t), &mapBase, fd );
    fhs_reg = (FHS_REGS_t *) mapMem( PIP_FHS_BASE, sizeof(FHS_REGS_t), &mapBase, fd );
    stat_cdma0 = (CBI2CDMA_REGS_t *) mapMem( PIP_STAT_CDMA0_BASE, sizeof(CBI2CDMA_REGS_t), &mapBase, fd );
    stat_cdma1 = (CBI2CDMA_REGS_t *) mapMem( PIP_STAT_CDMA1_BASE, sizeof(CBI2CDMA_REGS_t), &mapBase, fd );
    stat_cdma2 = (CBI2CDMA_REGS_t *) mapMem( PIP_STAT_CDMA2_BASE, sizeof(CBI2CDMA_REGS_t), &mapBase, fd );
#if ASIC_REV >= ASIC_REV_B0  && ASIC_gemstone
    sharpen_reg = (SHARPEN_REGS_t *)PIP_SHRPN_BASE;
    ctwin_reg = (CTWIN_REGS_t *)PIP_CTWIN_BASE;
    tems_reg = (TEMS_REGS_t *)PIP_TEMS_BASE;
    cre_reg = (CRE_RASTER_REGS_t *)PIP_CRE_BASE;
#else
    cre_reg = (CRE_REGS_t *) mapMem( PIP_CRE_BASE, sizeof(CRE_REGS_t *), &mapBase, fd);
    pip_lpc_idma0_reg = (IDMA_REGS_t *)mapMem( PIP_LPC_IDMA0_BASE, sizeof(IDMA_REGS_t), &mapBase, fd);
    pip_lpc_idma1_reg = (IDMA_REGS_t *)mapMem( PIP_LPC_IDMA1_BASE, sizeof(IDMA_REGS_t), &mapBase, fd);
    pip_lpc_idma2_reg = (IDMA_REGS_t *)mapMem( PIP_LPC_IDMA2_BASE, sizeof(IDMA_REGS_t), &mapBase, fd);
    pip_lpc_idma3_reg = (IDMA_REGS_t *)mapMem( PIP_LPC_IDMA3_BASE, sizeof(IDMA_REGS_t), &mapBase, fd);
#endif
#if ASIC_granite || (ASIC_REV >= ASIC_REV_B0  && ASIC_gemstone)
    th0_reg      = (CORE_REGS_t *)mapMem( PIP_TH0_CORE_BASE, sizeof(CORE_REGS_t), &mapBase, fd );
    th0_sram_reg = (SRAM_REGS_t *)mapMem( PIP_TH0_SRAM_BASE, sizeof(SRAM_REGS_t), &mapBase, fd );
    th1_reg      = (CORE_REGS_t *)mapMem( PIP_TH1_CORE_BASE, sizeof(CORE_REGS_t), &mapBase, fd );
    th1_sram_reg = (SRAM_REGS_t *)mapMem( PIP_TH1_SRAM_BASE, sizeof(SRAM_REGS_t), &mapBase, fd );
    th2_reg      = (CORE_REGS_t *)mapMem( PIP_TH2_CORE_BASE, sizeof(CORE_REGS_t), &mapBase, fd );
    th2_sram_reg = (SRAM_REGS_t *)mapMem( PIP_TH2_SRAM_BASE, sizeof(SRAM_REGS_t), &mapBase, fd );
    th3_reg      = (CORE_REGS_t *)mapMem( PIP_TH3_CORE_BASE, sizeof(CORE_REGS_t), &mapBase, fd );
    th3_sram_reg = (SRAM_REGS_t *)mapMem( PIP_TH3_SRAM_BASE, sizeof(SRAM_REGS_t), &mapBase, fd );
#else
    th3_reg      = (CORE_REGS_t *)mapMem( PIP_TH3_CORE_BASE, sizeof(CORE_REGS_t), &mapBase, fd );
    th3_sram_reg = (SRAM_REGS_t *)mapMem( PIP_TH3_SRAM_BASE, sizeof(SRAM_REGS_t), &mapBase, fd );

//    th0_reg->TCR |= CORE_TCR_BYPASS_MASK;
//    th1_reg->TCR |= CORE_TCR_BYPASS_MASK;
//    th2_reg->TCR |= CORE_TCR_BYPASS_MASK
//    th3_reg->TCR |= CORE_TCR_BYPASS_MASK;
#endif


    close(fd);
}

