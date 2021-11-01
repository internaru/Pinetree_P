/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the KMOD block.
  * 
 **/

#ifndef __KMOD_FUNC_H_INCLUDED
#define __KMOD_FUNC_H_INCLUDED

#include <stdint.h>
#include "kmod_cfg.h"
#include "asic.h"
#include "regAddrs.h"
#include "logger.h"

void kmod_cfg_driver(KMOD_REGS_t *kmod_reg, kmod_cfg_t *kmod_cfg);
void kmod_load_pattern(KMOD_REGS_t *kmod_reg, char *pat0, char *pat1, char* pat2, char *pat3);
void kmod_load_dither(KMOD_REGS_t *kmod_reg, char *c_dither, char *m_dither, char *k_dither);
void kmod_enable(KMOD_REGS_t *kmod_reg);
void kmod_disable(KMOD_REGS_t *kmod_reg);

#endif
