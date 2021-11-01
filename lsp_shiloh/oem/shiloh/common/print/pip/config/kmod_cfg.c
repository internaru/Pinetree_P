/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the KMOD block config.
  * 
 **/

#include "kmod_cfg.h"

kmod_cfg_t kmod_cfg = 
{
    .pad = 0,
    .kthresh = 3,
    .cthresh = 3,
    .mthresh = 1,
    .window = 0,
    .kdbypass = 0x0,
    .kfbypass = 0x0,
    .trap = 0x0,
    .bypass = 0x1,
    .serp = 0x0,
    .startdir = 0x0,
    .mmode = 0x0,
    .cmode = 0x0,
    .dmcs = 0x0,
    .dmrs = 0x0,
    .writedv = 0x1,
    .writehex = 0x1
};

kmod_cfg_t kmod_testpip2_cfg = 
{
    .pad = 0,
    .kthresh = 15,
    .cthresh = 15,
    .mthresh = 15,
    .window = 0,
    .kdbypass = 0x0,
    .kfbypass = 0x0,
    .trap = 0x0,
    .bypass = 0x1,
    .serp = 0x1,
    .startdir = 0x0,
    .mmode = 0x0,
    .cmode = 0x0,
    .dmcs = 0x0,
    .dmrs = 0x0,
    .writedv = 0x1,
    .writehex = 0x1
};

kmod_cfg_t kmod_testpip2_a_cfg = 
{
    .pad = 0,
    .kthresh = 15,
    .cthresh = 15,
    .mthresh = 15,
    .window = 0,
    .kdbypass = 0x0,
    .kfbypass = 0x0,
    .trap = 0x0,
    .bypass = 0x1,
    .serp = 0x1,
    .startdir = 0x0,
    .mmode = 0x0,
    .cmode = 0x0,
    .dmcs = 0x0,
    .dmrs = 0x0,
    .writedv = 0x1,
    .writehex = 0x1
};

kmod_cfg_t kmod_deplete_cfg = 
{
    .pad = 0,
    .kthresh = 1,
    .cthresh = 0,
    .mthresh = 0,
    .window = 0,
    .kdbypass = 0x0,
    .kfbypass = 0x1,
    .trap = 0x0,
    .bypass = 0x0,
    .serp = 0x1,
    .startdir = 0x0,
    .mmode = 0x0,
    .cmode = 0x0,
    .dmcs = 0x0,
    .dmrs = 0x0,
    .writedv = 0x1,
    .writehex = 0x1
};

kmod_cfg_t kmod_fortify_cfg = 
{
    .pad = 0,
    .kthresh = 1,
    .cthresh = 0,
    .mthresh = 0,
    .window = 0,
    .kdbypass = 0x1,
    .kfbypass = 0x0,
    .trap = 0x0,
    .bypass = 0x0,
    .serp = 0x1,
    .startdir = 0x0,
    .mmode = 0x0,
    .cmode = 0x0,
    .dmcs = 0x0,
    .dmrs = 0x0,
    .writedv = 0x1,
    .writehex = 0x1
};

kmod_cfg_t kmod_fortdepl_cfg = 
{
    .pad = 0,
    .kthresh = 1,
    .cthresh = 0,
    .mthresh = 0,
    .window = 0,
    .kdbypass = 0x0,
    .kfbypass = 0x0,
    .trap = 0x0,
    .bypass = 0x0,
    .serp = 0x1,
    .startdir = 0x0,
    .mmode = 0x0,
    .cmode = 0x0,
    .dmcs = 0x0,
    .dmrs = 0x0,
    .writedv = 0x1,
    .writehex = 0x1
};

kmod_cfg_t kmod_trap_cfg = 
{
    .pad = 0,
    .kthresh = 1,
    .cthresh = 1,
    .mthresh = 1,
    .window = 0,
    .kdbypass = 0x0,
    .kfbypass = 0x0,
    .trap = 0x1,
    .bypass = 0x0,
    .serp = 0x1,
    .startdir = 0x0,
    .mmode = 0x0,
    .cmode = 0x0,
    .dmcs = 0x0,
    .dmrs = 0x0,
    .writedv = 0x1,
    .writehex = 0x1
};

