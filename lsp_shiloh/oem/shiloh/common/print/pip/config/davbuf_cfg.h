/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the DAVBUF block config.
  * 
 **/

#ifndef __DB_CFG_HEADER
  #define __DB_CFG_HEADER

#include <stdint.h>

#include "ahs_cfg.h"
#include "cr_cfg.h"
#include "csc_cfg.h"
#include "fhs_cfg.h"
#include "ued_cfg.h"
#include "kmod_cfg.h"
#include "nga_cfg.h"
#include "pipe_access_cfg.h"
#include "serp_cfg.h"
#include "sccsc_cfg.h"
#include "tp_cfg.h"
#include "yipi_cfg.h"

#include "asic.h"


//#define DAVBUF_PIXWIDTH_SERP 5120   // Min widths to invoke DAVBUF
#define DAVBUF_PIXWIDTH_SERP 7400   // Min widths to invoke DAVBUF
#define DAVBUF_PIXWIDTH_UED 10240 
#define DAVBUF_PIXWIDTH_KMOD 10240 
#define DAVBUF_PIXBURST 16
#define DAVBUF_BUSWIDTH 32 
#define DAVBUF_NORMAL 0
#define DAVBUF_BITS_PER_PIXEL 64


typedef struct DAVBUFF_REGS_s DB_REGS_t;

typedef struct db_cfg_struct 
{
    uint32_t imagewidth;
    uint32_t imageheight;
    uint32_t bpp;
    uint32_t mode;
    uint32_t buffer;
    uint32_t buffer_end;
    uint32_t pixbursts;
    uint32_t beatsperpixburst;
    uint32_t sram_start;
    uint32_t sram_end;
} db_cfg_t;

extern db_cfg_t db_ued_generic_cfg;
extern db_cfg_t db_serp_generic_cfg;
extern db_cfg_t db_kmod_generic_cfg;

#endif

