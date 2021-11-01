/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the MIP block config.
  * 
 **/

#ifndef __MIP_CFG_HEADER
  #define __MIP_CFG_HEADER

#include <stdint.h>

typedef struct mip_cfg_struct 
{
    uint32_t bypass;
    uint32_t cpuaccess;
    uint32_t bufferlimit; // this is the encoded value for number of lines
    uint32_t winsize;     // this is the encoded window size as documented in the ers
    uint32_t addlcol;
    uint32_t addrcol;
    uint32_t addtopline;
    uint32_t addbottline;
    uint32_t tonermode;
    uint32_t tonerresolution;
    uint32_t mode;
    uint32_t outdepth;   // the encoded output bit depth
    uint32_t indepth;    // the encoded input bit depth
    uint32_t inwidth;
    uint32_t hmove[8];
    uint32_t vmove[8];
    uint32_t hmovelast;
    uint32_t vmovelast;
    uint32_t addmuxunstable[19];
    uint32_t addmuxhstable[19];
    uint32_t addmuxvstable[19];
    uint32_t vstabledepth;
    uint32_t vstablecol;
    uint32_t vstablesize;
    uint32_t hstabledepth;
    uint32_t hstablerow;
    uint32_t hstablesize;
    uint32_t hgalvostart;
    uint32_t hcountstep;
    uint32_t hcountoffset;
    uint32_t vcountstep;
    uint32_t vcountoffset;
} mip_cfg_t;

extern mip_cfg_t mip_bypass_cfg;
extern mip_cfg_t mip_test_cfg;

#endif
