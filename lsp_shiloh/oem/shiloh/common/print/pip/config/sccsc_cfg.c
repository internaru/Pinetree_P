/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the ScCSC block config.
  * 
 **/

#include "sccsc_cfg.h"

sccsc_cfg_t sccsc_bypass_cfg = 
{
	.outchans = 6,
	.outbpp = 10,
	.chanmap = {0, 1, 2, 3, 4, 5},
	.bypassall = 1,
	.ialg = 0,         // tetrahedral
	.nge = 0,
	.wme = 0,
	.kme = 0,
	.lutsel = 0,
	.sband_en = 0,
	.sband_mask = 0,
	.sband_value = 0,
	.lutsize = 0,
	.wt = {0, 0, 0},
	.wr = {0, 0, 0, 0, 0, 0},
	.kr = {0, 0, 0, 0, 0, 0},
	.imgheight = 587,
	.imgwidth = 880
};

sccsc_cfg_t sccsc_photo_cfg = 
{
	.outchans = 6,
	.outbpp = 10,
	.chanmap = {0, 1, 2, 3, 4, 5},
	.bypassall = 0,
	.ialg = 3,         // tetrahedral
	.nge = 0,
	.wme = 1,
	.kme = 0,
	.lutsel = 0,
	.sband_en = 0,
	.sband_mask = 0,
	.sband_value = 0,
	.lutsize = 0,
	.wt = {1016, 1016, 1016},
	// If using p-ink, the last '0' in .wr should be 850. Otherwise, want all to be 0
	.wr = {0, 0, 0, 0, 0, 0},
	.kr = {731, 731, 731, 922, 0, 0},
	.imgheight = 587,
	.imgwidth = 880
};

sccsc_cfg_t sccsc_plain_cfg = 
{
	.outchans = 6,
	.outbpp = 10,
	.chanmap = {0, 1, 2, 3, 4, 5},
	.bypassall = 0,
	.ialg = 3,         // tetrahedral
	.nge = 0,
	.wme = 1,
	.kme = 0,
	.lutsel = 0,
	.sband_en = 0,
	.sband_mask = 0,
	.sband_value = 0,
	.lutsize = 0,
	.wt = {1016, 1016, 1016},
	.wr = {0, 0, 0, 0, 0, 0},
	.kr = {0, 0, 0, 0, 1023, 0},
	.imgheight = 587,
	.imgwidth = 880
};

sccsc_cfg_t sccsc_mono8_cfg = 
{
	.outchans = 6,
	.outbpp = 10,
	.chanmap = {0, 1, 2, 3, 4, 5},
	.bypassall = 0,
	.ialg = 2,         // radial
	.nge = 0,
	.wme = 0,
	.kme = 0,
	.lutsel = 0,
	.sband_en = 0,
	.sband_mask = 0,
	.sband_value = 0,
	.lutsize = 0,
	.wt = {0, 0, 0},
	.wr = {0, 0, 0, 0, 0, 0},
	.kr = {0, 0, 0, 0, 0, 0},
	.imgheight = 587,
	.imgwidth = 880
};

