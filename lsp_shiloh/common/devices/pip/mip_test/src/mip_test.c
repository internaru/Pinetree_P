/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the MIP block.
  * 
 **/

#include "mip_test_api.h"
#include "logger.h"

#define DBG_PRFX "MIP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

//********************************
// mip_cfg_driver - setup the MIP block
//********************************
void mip_cfg_driver(MIP_REGSET_REGS_t *mip_reg, mip_cfg_t *mip_cfg)
{
    uint32_t reg_val;

    DBG_PRINTF_NOTICE( "Called mip_cfg_driver() \n\r");

    // MIP configuration register (MCR)
    reg_val        = mip_reg->MCR;
    reg_val        = MIP_REGSET_MCR_TONERRESOLUTION_REPLACE_VAL(reg_val, mip_cfg->tonerresolution);
    reg_val        = MIP_REGSET_MCR_ADDCOLRIGHT_REPLACE_VAL(reg_val, mip_cfg->addrcol);
    reg_val        = MIP_REGSET_MCR_ADDCOLLEFT_REPLACE_VAL(reg_val, mip_cfg->addlcol);
    reg_val        = MIP_REGSET_MCR_ADDOUTLINESBOTTOM_REPLACE_VAL(reg_val, mip_cfg->addbottline);
    reg_val        = MIP_REGSET_MCR_ADDOUTLINESTOP_REPLACE_VAL(reg_val, mip_cfg->addtopline);
    reg_val        = MIP_REGSET_MCR_TONERMODE_REPLACE_VAL(reg_val, mip_cfg->tonermode);
    reg_val        = MIP_REGSET_MCR_BUFFERLIMIT_REPLACE_VAL(reg_val, mip_cfg->bufferlimit);
    reg_val        = MIP_REGSET_MCR_MODE_REPLACE_VAL(reg_val, mip_cfg->mode);
    reg_val        = MIP_REGSET_MCR_WINDOWSIZE_REPLACE_VAL(reg_val, mip_cfg->winsize);
    reg_val        = MIP_REGSET_MCR_OUTDEPTH_REPLACE_VAL(reg_val, mip_cfg->outdepth);
    reg_val        = MIP_REGSET_MCR_INWIDTH_REPLACE_VAL(reg_val, mip_cfg->inwidth);
    reg_val        = MIP_REGSET_MCR_INDEPTH_REPLACE_VAL(reg_val, mip_cfg->indepth);
    reg_val        = MIP_REGSET_MCR_CPUACCESS_REPLACE_VAL(reg_val, mip_cfg->cpuaccess);
    reg_val        = MIP_REGSET_MCR_BYPASS_REPLACE_VAL(reg_val, mip_cfg->bypass);
    mip_reg->MCR = reg_val;

    // MIP Horizontal Move register (MHMR)
    reg_val        = mip_reg->MHMR;
    reg_val        = MIP_REGSET_MHMR_HMOVELAST_REPLACE_VAL(reg_val, mip_cfg->hmovelast);
    reg_val        = MIP_REGSET_MHMR_HMOVE7_REPLACE_VAL(reg_val, mip_cfg->hmove[7]);
    reg_val        = MIP_REGSET_MHMR_HMOVE6_REPLACE_VAL(reg_val, mip_cfg->hmove[6]);
    reg_val        = MIP_REGSET_MHMR_HMOVE5_REPLACE_VAL(reg_val, mip_cfg->hmove[5]);
    reg_val        = MIP_REGSET_MHMR_HMOVE4_REPLACE_VAL(reg_val, mip_cfg->hmove[4]);
    reg_val        = MIP_REGSET_MHMR_HMOVE3_REPLACE_VAL(reg_val, mip_cfg->hmove[3]);
    reg_val        = MIP_REGSET_MHMR_HMOVE2_REPLACE_VAL(reg_val, mip_cfg->hmove[2]);
    reg_val        = MIP_REGSET_MHMR_HMOVE1_REPLACE_VAL(reg_val, mip_cfg->hmove[1]);
    reg_val        = MIP_REGSET_MHMR_HMOVE0_REPLACE_VAL(reg_val, mip_cfg->hmove[0]);
    mip_reg->MHMR = reg_val;

    // MIP Vertical Move register (MVMR)
    reg_val        = mip_reg->MVMR;
    reg_val        = MIP_REGSET_MVMR_VMOVELAST_REPLACE_VAL(reg_val, mip_cfg->vmovelast);
    reg_val        = MIP_REGSET_MVMR_VMOVE7_REPLACE_VAL(reg_val, mip_cfg->vmove[7]);
    reg_val        = MIP_REGSET_MVMR_VMOVE6_REPLACE_VAL(reg_val, mip_cfg->vmove[6]);
    reg_val        = MIP_REGSET_MVMR_VMOVE5_REPLACE_VAL(reg_val, mip_cfg->vmove[5]);
    reg_val        = MIP_REGSET_MVMR_VMOVE4_REPLACE_VAL(reg_val, mip_cfg->vmove[4]);
    reg_val        = MIP_REGSET_MVMR_VMOVE3_REPLACE_VAL(reg_val, mip_cfg->vmove[3]);
    reg_val        = MIP_REGSET_MVMR_VMOVE2_REPLACE_VAL(reg_val, mip_cfg->vmove[2]);
    reg_val        = MIP_REGSET_MVMR_VMOVE1_REPLACE_VAL(reg_val, mip_cfg->vmove[1]);
    reg_val        = MIP_REGSET_MVMR_VMOVE0_REPLACE_VAL(reg_val, mip_cfg->vmove[0]);
    mip_reg->MVMR = reg_val;

    //MIP Horizontal Count register (MHCR)
    reg_val         = mip_reg->MHCR;
    reg_val         = MIP_REGSET_MHCR_HCOUNTSTEP_REPLACE_VAL(reg_val, mip_cfg->hcountstep);
    reg_val         = MIP_REGSET_MHCR_HCOUNTOFFSET_REPLACE_VAL(reg_val, mip_cfg->hcountoffset);
    mip_reg->MHCR = reg_val;

    //MIP Vertical Count register (MVCR)
    reg_val         = mip_reg->MVCR;
    reg_val         = MIP_REGSET_MVCR_VCOUNTSTEP_REPLACE_VAL(reg_val, mip_cfg->vcountstep);
    reg_val         = MIP_REGSET_MVCR_VCOUNTOFFSET_REPLACE_VAL(reg_val, mip_cfg->vcountoffset);
    mip_reg->MVCR = reg_val;

    //MIP Window Stability register (MWSR)
    reg_val         = mip_reg->MWSR;
    reg_val         = MIP_REGSET_MWSR_VSTABLECOLS_REPLACE_VAL(reg_val, mip_cfg->vstablecol);
    reg_val         = MIP_REGSET_MWSR_VSTABLEDEPTH_REPLACE_VAL(reg_val, mip_cfg->vstabledepth);
    reg_val         = MIP_REGSET_MWSR_VSTABLESIZE_REPLACE_VAL(reg_val, mip_cfg->vstablesize);
    reg_val         = MIP_REGSET_MWSR_HSTABLEROWS_REPLACE_VAL(reg_val, mip_cfg->hstablerow);
    reg_val         = MIP_REGSET_MWSR_HSTABLEDEPTH_REPLACE_VAL(reg_val, mip_cfg->hstabledepth);
    reg_val         = MIP_REGSET_MWSR_HSTABLESIZE_REPLACE_VAL(reg_val, mip_cfg->hstablesize);
    mip_reg->MWSR = reg_val;

    //MIP MUX register 0 (MMR0)
    reg_val         = mip_reg->MMR0;
    reg_val         = MIP_REGSET_MMR0_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[0]);
    reg_val         = MIP_REGSET_MMR0_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[0]);
    reg_val         = MIP_REGSET_MMR0_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[0]);
    mip_reg->MMR0 = reg_val;

    //MIP MUX register 1 (MMR1)
    reg_val         = mip_reg->MMR1;
    reg_val         = MIP_REGSET_MMR1_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[1]);
    reg_val         = MIP_REGSET_MMR1_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[1]);
    reg_val         = MIP_REGSET_MMR1_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[1]);
    mip_reg->MMR1 = reg_val;

    //MIP MUX register 2 (MMR2)
    reg_val         = mip_reg->MMR2;
    reg_val         = MIP_REGSET_MMR2_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[2]);
    reg_val         = MIP_REGSET_MMR2_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[2]);
    reg_val         = MIP_REGSET_MMR2_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[2]);
    mip_reg->MMR2 = reg_val;

    //MIP MUX register 3 (MMR3)
    reg_val         = mip_reg->MMR3;
    reg_val         = MIP_REGSET_MMR3_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[3]);
    reg_val         = MIP_REGSET_MMR3_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[3]);
    reg_val         = MIP_REGSET_MMR3_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[3]);
    mip_reg->MMR3 = reg_val;

    //MIP MUX register 4 (MMR4)
    reg_val         = mip_reg->MMR4;
    reg_val         = MIP_REGSET_MMR4_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[4]);
    reg_val         = MIP_REGSET_MMR4_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[4]);
    reg_val         = MIP_REGSET_MMR4_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[4]);
    mip_reg->MMR4 = reg_val;

    //MIP MUX register 5 (MMR5)
    reg_val         = mip_reg->MMR5;
    reg_val         = MIP_REGSET_MMR5_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[5]);
    reg_val         = MIP_REGSET_MMR5_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[5]);
    reg_val         = MIP_REGSET_MMR5_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[5]);
    mip_reg->MMR5 = reg_val;

    //MIP MUX register 6 (MMR6)
    reg_val         = mip_reg->MMR6;
    reg_val         = MIP_REGSET_MMR6_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[6]);
    reg_val         = MIP_REGSET_MMR6_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[6]);
    reg_val         = MIP_REGSET_MMR6_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[6]);
    mip_reg->MMR6 = reg_val;

    //MIP MUX register 7 (MMR7)
    reg_val         = mip_reg->MMR7;
    reg_val         = MIP_REGSET_MMR7_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[7]);
    reg_val         = MIP_REGSET_MMR7_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[7]);
    reg_val         = MIP_REGSET_MMR7_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[7]);
    mip_reg->MMR7 = reg_val;

    //MIP MUX register 8 (MMR8)
    reg_val         = mip_reg->MMR8;
    reg_val         = MIP_REGSET_MMR8_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[8]);
    reg_val         = MIP_REGSET_MMR8_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[8]);
    reg_val         = MIP_REGSET_MMR8_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[8]);
    mip_reg->MMR8 = reg_val;

    //MIP MUX register 9 (MMR9)
    reg_val         = mip_reg->MMR9;
    reg_val         = MIP_REGSET_MMR9_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[9]);
    reg_val         = MIP_REGSET_MMR9_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[9]);
    reg_val         = MIP_REGSET_MMR9_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[9]);
    mip_reg->MMR9 = reg_val;

    //MIP MUX register 10 (MMR10)
    reg_val         = mip_reg->MMR10;
    reg_val         = MIP_REGSET_MMR10_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[10]);
    reg_val         = MIP_REGSET_MMR10_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[10]);
    reg_val         = MIP_REGSET_MMR10_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[10]);
    mip_reg->MMR10 = reg_val;

    //MIP MUX register 11 (MMR11)
    reg_val         = mip_reg->MMR11;
    reg_val         = MIP_REGSET_MMR11_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[11]);
    reg_val         = MIP_REGSET_MMR11_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[11]);
    reg_val         = MIP_REGSET_MMR11_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[11]);
    mip_reg->MMR11 = reg_val;

    //MIP MUX register 12 (MMR12)
    reg_val         = mip_reg->MMR12;
    reg_val         = MIP_REGSET_MMR12_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[12]);
    reg_val         = MIP_REGSET_MMR12_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[12]);
    reg_val         = MIP_REGSET_MMR12_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[12]);
    mip_reg->MMR12 = reg_val;

    //MIP MUX register 13 (MMR13)
    reg_val         = mip_reg->MMR13;
    reg_val         = MIP_REGSET_MMR13_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[13]);
    reg_val         = MIP_REGSET_MMR13_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[13]);
    reg_val         = MIP_REGSET_MMR13_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[13]);
    mip_reg->MMR13 = reg_val;

    //MIP MUX register 14 (MMR14)
    reg_val         = mip_reg->MMR14;
    reg_val         = MIP_REGSET_MMR14_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[14]);
    reg_val         = MIP_REGSET_MMR14_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[14]);
    reg_val         = MIP_REGSET_MMR14_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[14]);
    mip_reg->MMR14 = reg_val;

    //MIP MUX register 15 (MMR15)
    reg_val         = mip_reg->MMR15;
    reg_val         = MIP_REGSET_MMR15_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[15]);
    reg_val         = MIP_REGSET_MMR15_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[15]);
    reg_val         = MIP_REGSET_MMR15_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[15]);
    mip_reg->MMR15 = reg_val;

    //MIP MUX register 16 (MMR16)
    reg_val         = mip_reg->MMR16;
    reg_val         = MIP_REGSET_MMR16_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[16]);
    reg_val         = MIP_REGSET_MMR16_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[16]);
    reg_val         = MIP_REGSET_MMR16_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[16]);
    mip_reg->MMR16 = reg_val;

    //MIP MUX register 17 (MMR17)
    reg_val         = mip_reg->MMR17;
    reg_val         = MIP_REGSET_MMR17_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[17]);
    reg_val         = MIP_REGSET_MMR17_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[17]);
    reg_val         = MIP_REGSET_MMR17_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[17]);
    mip_reg->MMR17 = reg_val;

    //MIP MUX register 18 (MMR18)
    reg_val         = mip_reg->MMR18;
    reg_val         = MIP_REGSET_MMR18_ADDRVSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxvstable[18]);
    reg_val         = MIP_REGSET_MMR18_ADDRHSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxhstable[18]);
    reg_val         = MIP_REGSET_MMR18_ADDRUNSTABLEMUX_REPLACE_VAL(reg_val, mip_cfg->addmuxunstable[18]);
    mip_reg->MMR18 = reg_val;

    //MIP Galvo setup register 0 (MGSR)
    reg_val         = mip_reg->MGSR;
    reg_val         = MIP_REGSET_MGSR_HGALVOSTART_REPLACE_VAL(reg_val, mip_cfg->hgalvostart);
    mip_reg->MGSR = reg_val;
 }

// Note that these routines are set up for when the ink mode is enabled and
// the TIELUT is 1536x64.  Other modes have wider LUTs and these would need
// to be modified to support them.

//********************************
// mip_load_tielut - Load the data LUT for text enahance.
//********************************
void mip_load_tielut(uint32_t *TIELUT, uint32_t locations, MIP_REGSET_REGS_t *mip_reg)
{
    // locations variable holds the number of 64-bit entries to load.
    // TIELUT should have 2*locations valid entries.

    uint32_t i;
    uint32_t reg_val;

    // Enable CPU Access
    reg_val        = mip_reg->MCR;
    reg_val        = MIP_REGSET_MCR_CPUACCESS_REPLACE_VAL(reg_val, 1);
    mip_reg->MCR = reg_val;

    // Now load the LUT values as 2, 32-bit pairs
    for(i=0; i<locations; i++) 
    {
        mip_reg->TIEWR0 = TIELUT[2*i];
        mip_reg->TIEWR1 = TIELUT[(2*i)+1];
        reg_val = 0;
        reg_val = MIP_REGSET_TIEAR_ADDRESS_REPLACE_VAL(reg_val, i);
        reg_val = MIP_REGSET_TIEAR_MODE_REPLACE_VAL(reg_val, 0);
        mip_reg->TIEAR = reg_val;
    }

    // Disable CPU Access
    reg_val        = mip_reg->MCR;
    reg_val        = MIP_REGSET_MCR_CPUACCESS_REPLACE_VAL(reg_val, 0);
    mip_reg->MCR = reg_val;
}

//********************************
// mip_check_tielut - Verify the data LUT for text enahance.
//********************************
void mip_check_tielut(uint32_t *TIELUT, uint32_t locations, MIP_REGSET_REGS_t *mip_reg)
{
    // locations variable holds the number of 64-bit entries to check.
    // TIELUT should have 2*locations valid entries.

    uint32_t i;
    uint32_t reg_val;

    // Enable CPU Access
    reg_val        = mip_reg->MCR;
    reg_val        = MIP_REGSET_MCR_CPUACCESS_REPLACE_VAL(reg_val, 1);
    mip_reg->MCR = reg_val;

    // Now read the LUT values as 2, 32-bit pairs and compare to TIELUT
    for(i=0; i<locations; i++) 
    {
        reg_val = 0;
        reg_val = MIP_REGSET_TIEAR_ADDRESS_REPLACE_VAL(reg_val, i);
        reg_val = MIP_REGSET_TIEAR_MODE_REPLACE_VAL(reg_val, 1);
        mip_reg->TIEAR = reg_val;

        if(mip_reg->TIEDRR0 != TIELUT[2*i]) 
        {
            DBG_PRINTF_NOTICE( "ERROR: %s : Incorrect tielut data0 at Addr=0x%8x, Act=0x%8x, Exp=0x%8x\n\r", __FUNCTION__, i, mip_reg->TIEDRR0, TIELUT[2*i]);
        }

        if(mip_reg->TIEDRR1 != TIELUT[(2*i)+1]) 
        {
            DBG_PRINTF_NOTICE( "ERROR: %s : Incorrect tielut data1 at Addr=0x%8x, Act=0x%8x, Exp=0x%8x\n\r", __FUNCTION__, i, mip_reg->TIEDRR1, TIELUT[(2*i)+1]);
        }
    }

    // Disable CPU Access
    reg_val        = mip_reg->MCR;
    reg_val        = MIP_REGSET_MCR_CPUACCESS_REPLACE_VAL(reg_val, 0);
    mip_reg->MCR = reg_val;
}
