/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the PIP config.
  * 
 **/

#ifndef __PIP_CFG_HEADER
  #define __PIP_CFG_HEADER

#include <stdint.h>

#include "asic.h"
#include "ahs_cfg.h"
#include "cp_cfg.h"
#include "cr_cfg.h"
#include "csc_cfg.h"
#include "fhs_cfg.h"
#include "ued_cfg.h"
#include "th_cfg.h"
#include "kmod_cfg.h"
#include "nga_cfg.h"
#include "raster_odma_cfg.h"
#include "pipe_access_cfg.h"
#include "pa_idma_cfg.h"
#include "serp_cfg.h"
#include "sccsc_cfg.h"
#include "tp_cfg.h"
#include "yipi_cfg.h"
#include "davbuf_cfg.h"
#include "mip_cfg.h"
#include "stat_cfg.h"
#if ASIC_REV >= ASIC_REV_B0  && ASIC_gemstone
#include "sharpen_cfg.h"
#include "ctwin_cfg.h"
#include "tems_cfg.h"

#endif
#include "color_reg_enhance_cfg.h"

#ifdef ASIC_gemstone
#define PIP_MAX_OUTPUT_CHANS 6
#else // granite
#define PIP_MAX_OUTPUT_CHANS 8
#endif
typedef struct pip_cfg_struct 
{
    uint32_t          cfg_mode;
    const char        *cfg_name;
    bool               mono_nColor; //< 8bit gray input NOT 24bit RGB input
    uint32_t           imagew;
    uint32_t           imageh;
    uint32_t           bpp;
    ahs_cfg_t         *ahs_cfg;
    cr_cfg_t          *cr_cfg;
    csc_cfg_t         *csc_cfg;
    fhs_cfg_t         *fhs_cfg;
    ued_cfg_t         *ued_cfg;
    th_cfg_t          *th_cfg[4];
    kmod_cfg_t        *kmod_cfg;
    nga_cfg_t         *nga_cfg;
    raster_odma_cfg_t *raster_odma_cfg[PIP_MAX_OUTPUT_CHANS];
    pipe_access_cfg_t *pipe_access_cfg;
    pa_idma_cfg_t     *pa_idma_cfg;
    raster_odma_cfg_t *pa_odma_cfg;
    serp_cfg_t        *serp_cfg;
    sccsc_cfg_t       *sccsc_cfg;
    tp_cfg_t          *tp_cfg;
    cp_cfg_t          *cp_cfg;
    mip_cfg_t         *mip_cfg;
    yipi_cfg_t        *yipi_cfg;
    stat_cfg_t        *stat_cfg;
#if ASIC_REV >= ASIC_REV_B0  && ASIC_gemstone
    sharpen_cfg_t     *sharpen_cfg;
    ctwin_cfg_t       *ctwin_cfg;
    tems_cfg_t        *tems_cfg;

#endif
    color_reg_enhance_cfg_t *cre_cfg;
    db_cfg_t          *db_ued_cfg;
    db_cfg_t          *db_kmod_cfg;
    db_cfg_t          *db_serp_cfg;
    uint8_t           *sccsc_table;
    uint8_t           *onedlut0;
    uint8_t           *onedlut1;
    uint8_t           *onedlut2;
    uint8_t           *onedlut3;
    uint8_t           *onedlut4;
    uint8_t           *onedlut5;
} pip_cfg_t;

bool pip_cfg_is_mode_supported(uint32_t mode);
error_type_t pip_cfg_copy(uint32_t mode, pip_cfg_t *cfg);
error_type_t pip_cfg_reference_get(uint32_t mode, pip_cfg_t **cfg);
error_type_t pip_cfg_mode_bpp_get(uint32_t mode, uint32_t *bpp);
const char *pip_cfg_mode_name_get(uint32_t mode);
//-jrserror_type_t pip_cfg_out_chan_map_get(uint32_t mode,
//-jrs                                      pip_odma_chan_map_entry_t out_chan_map[]);
#endif

