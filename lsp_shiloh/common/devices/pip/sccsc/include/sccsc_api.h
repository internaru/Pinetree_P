/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the ScCSC block.
  * 
 **/

#ifndef __SCCSC_FUNC_H_INCLUDED
#define __SCCSC_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "regAddrs.h"
#include "logger.h"
#include "sccsc_cfg.h"

void sccsc_setup(SCCSC_REGS_t *sc_csc_reg, 
                 sccsc_cfg_t *sccsc_cfg,
                 uint64_t *ilut,
                 uint64_t *cachelut,
                 uint32_t cpuaccess, 
                 uint32_t hc, 
                 uint32_t base_address );

void sccsc_wr_ram(SCCSC_REGS_t *sc_csc_reg, 
                  uint32_t addr, 
                  uint32_t autoinc, 
		          uint32_t count, 
                  uint64_t* data);

void sccsc_loadilut(SCCSC_REGS_t *sc_csc_reg, 
                    uint32_t autoinc, 
                    uint64_t* lut);

void sccsc_loadcachelut(SCCSC_REGS_t *sc_csc_reg, 
                        uint32_t lutsize, 
                        uint64_t* lut);

void sccsc_gen_img(uint32_t *img,
		           uint32_t size);

void sccsc_gen_lut(uint64_t *lut,
		           uint32_t cubesize);

void sccsc_load_exp(SCCSC_REGS_t *sc_csc_reg,
                    uint32_t *img,
                    uint32_t pcnt,
                    uint32_t *addr );

#endif
