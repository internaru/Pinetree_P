/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the MIP block config.
  * 
 **/

#include "mip_cfg.h"

mip_cfg_t mip_bypass_cfg = 
{
    .bypass = 1,
    .cpuaccess = 0,
    .bufferlimit = 0,
    .winsize = 0,
    .addlcol = 0,
    .addrcol = 0,
    .addtopline = 0,
    .addbottline = 0,
    .tonermode = 0,
    .tonerresolution = 0,
    .mode = 0,
    .outdepth = 0,
    .indepth = 0,
    .inwidth = 0,
    .hmove = {0, 0, 0, 0, 0, 0, 0, 0},
    .vmove = {0, 0, 0, 0, 0, 0, 0, 0},
    .hmovelast = 0,
    .vmovelast = 0,
    .addmuxunstable = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .addmuxhstable = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .addmuxvstable = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .vstabledepth = 0,
    .vstablecol = 0,
    .vstablesize = 0,
    .hstabledepth = 0,
    .hstablerow = 0,
    .hstablesize = 0,
    .hgalvostart = 0,
    .hcountstep = 0,
    .hcountoffset = 0,
    .vcountstep = 0,
    .vcountoffset = 0
};

mip_cfg_t mip_test_cfg = 
{
    .bypass = 0,
    .cpuaccess = 0,
    .bufferlimit = 7,
    .winsize = 0,  
    .addlcol = 2,
    .addrcol = 5,
    .addtopline = 2,
    .addbottline = 5,
    .tonermode = 0,
    .tonerresolution = 0,
    .mode = 0,
    .outdepth = 0,
    .indepth = 0,
    .inwidth = 0,
    .hmove = {0, 1, 0, 0, 0, 0, 0, 0},
    .vmove = {0, 1, 0, 0, 0, 0, 0, 0},
    .hmovelast = 1,
    .vmovelast = 1,   
    .addmuxunstable = {42,46,35,27,31,32,10,14,22,24, 3, 5, 6, 2,18,40,40,40,40},
    .addmuxhstable = {42,46,35,27,31,32,10,14,22,24, 3, 5, 6, 2,18,40,40,40,40},
    .addmuxvstable = {42,46,35,27,31,32,10,14,22,24, 3, 5, 6, 2,18,40,40,40,40},
    .vstabledepth = 0,
    .vstablecol = 16,
    .vstablesize = 4,
    .hstabledepth = 0,
    .hstablerow = 16,
    .hstablesize = 4,   
    .hgalvostart = 0,
    .hcountstep = 0,
    .hcountoffset = 0,
    .vcountstep = 0,
    .vcountoffset = 0
};
