/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the KMOD block config.
  * 
 **/

#ifndef __KMOD_CFG_HEADER
  #define __KMOD_CFG_HEADER

#include <stdint.h>

typedef struct kmod_cfg_struct 
{
    uint32_t pad;
    uint32_t kthresh;
    uint32_t cthresh;
    uint32_t mthresh;
    uint32_t window;
    uint32_t kdbypass;
    uint32_t kfbypass;
    uint32_t trap;
    uint32_t bypass;
    uint32_t serp;
    uint32_t startdir;
    uint32_t mmode;
    uint32_t cmode;
    uint32_t dmcs;
    uint32_t dmrs;
    uint32_t writedv;
    uint32_t writehex;
} kmod_cfg_t;

extern kmod_cfg_t kmod_cfg;
extern kmod_cfg_t kmod_testpip2_cfg;
extern kmod_cfg_t kmod_testpip2_a_cfg;
extern kmod_cfg_t kmod_deplete_cfg;
extern kmod_cfg_t kmod_fortify_cfg;
extern kmod_cfg_t kmod_fortdel_cfg;
extern kmod_cfg_t kmod_trap_cfg;

#endif
