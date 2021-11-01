/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the AHS block.
  * 
 **/

#ifdef DONT

#include "ahs_api.h"

#define DBG_PRFX "AHS: "

//********************************
// ahs_cfg_driver - setup the AHS block
//********************************
void ahs_cfg_driver(AHS_cfg_t *ahs_cfg, AHS_REGS_t *ahs_reg)
{
    uint32_t reg_val;

    DBG_PRINTF(LOG_NOTICE|DBG_PIP, DBG_PRFX "Called ahs_cfg_driver() \n\r");

    // AHS Configuration Register (ACR)
    reg_val      = ahs_reg->ACR;
    reg_val      = AHS_ACR_IMAGE_WIDTH_M1_REPLACE_VAL(reg_val, (ahs_cfg->up0_width-1));
    reg_val      = AHS_ACR_CPU_ACCESS_REPLACE_VAL(reg_val, 0);
    reg_val      = AHS_ACR_SS_REPLACE_VAL(reg_val, ahs_cfg->serp_start_dir);
    reg_val      = AHS_ACR_SD_REPLACE_VAL(reg_val, ahs_cfg->serpentine_data);
    reg_val      = AHS_ACR_STATS_ENABLE_REPLACE_VAL(reg_val, ahs_cfg->en_stats0); // en_stats0 and 1 should always be same
    reg_val      = AHS_ACR_AH1_ENABLE_REPLACE_VAL(reg_val, ahs_cfg->en_ah1);  
    reg_val      = AHS_ACR_AH0_ENABLE_REPLACE_VAL(reg_val, ahs_cfg->en_ah0);
    reg_val      = AHS_ACR_BYPASS_ENABLE_REPLACE_VAL(reg_val, ahs_cfg->en_bypass);
    ahs_reg->ACR = reg_val;

    // AH Configuration Register (AAHCR)
    reg_val        = ahs_reg->AAHCR;
    reg_val        = AHS_AAHCR_AH1_MODE_SELECT_REPLACE_VAL(reg_val, ahs_cfg->ah0_mode_select);
    reg_val        = AHS_AAHCR_AH0_MODE_SELECT_REPLACE_VAL(reg_val, ahs_cfg->ah0_mode_select);
    ahs_reg->AAHCR = reg_val;

    //********* FIXME - C register files need updated - fix needed for stats *********
}

//********************************
// ahs_setup_luts - program the LUTs
//********************************
void ahs_setup_luts(uint32_t* lut0_data,
                    uint32_t* lut1_data,
                    AHS_REGS_t *ahs_reg)
{
    // load 'em
    ahs_lut_load(ahs_reg->LUT0, lut0_data);
    ahs_lut_load(ahs_reg->LUT1, lut1_data);

    // check 'em
    ahs_lut_check(ahs_reg->LUT0, lut0_data);
    ahs_lut_check(ahs_reg->LUT1, lut1_data);
}

//********************************
// AHS_lut_load
// Load the specified LUT with data at data_ptr
//********************************
void ahs_lut_load(volatile uint32_t *ahs_lut_ptr, 
                  uint32_t* data_ptr)
{
    uint32_t reg_val;
    uint32_t i;

    // first set the LUT access bits
    reg_val      = ahs_reg->ACR;
    reg_val      = AHS_ACR_CPU_ACCESS_REPLACE_VAL(reg_val, 1);
    ahs_reg->ACR = reg_val;

    // blast the data into the LUT
    for(i=0; i<AHS_LUT_DEPTH; i++)
    {
        ahs_lut_ptr[i] = data_ptr[i];
    }

    // disable LUT access
    reg_val      = ahs_reg->ACR;
    reg_val      = AHS_ACR_CPU_ACCESS_REPLACE_VAL(reg_val, 0);
    ahs_reg->ACR = reg_val;
}

//********************************
// ahs_lut_check
// Compare the specified LUT with data at data_ptr
//********************************
void ahs_lut_check(volatile uint32_t *ahs_lut_ptr, 
                   uint32_t* data_ptr)
{
    uint32_t reg_val;
    uint32_t i;

    // first set the LUT access bits
    reg_val      = ahs_reg->ACR;
    reg_val      = AHS_ACR_CPU_ACCESS_REPLACE_VAL(reg_val, 1);
    ahs_reg->ACR = reg_val;

    DBG_PRINTF(LOG_NOTICE|DBG_PIP, DBG_PRFX "%s : Before LUT read config = 0x%08x\n\r",
        __FUNCTION__, ahs_reg->ACR);

    // read and compare the LUT data
    for(i=0; i<AHS_LUT_DEPTH; i++)
    {
        reg_val = ahs_lut_ptr[i];
        if(reg_val != data_ptr[i])
        {
            DBG_PRINTF(LOG_NOTICE|DBG_PIP, DBG_PRFX "ERROR: %s : Incorrect LUT Data LUT Addr=0x%08x, Act=0x%08x, Exp=0x%08x\n\r",
                __FUNCTION__, &ahs_lut_ptr[i], reg_val, data_ptr[i]);
        }
    }

    // disable LUT access
    reg_val      = ahs_reg->ACR;
    reg_val      = AHS_ACR_CPU_ACCESS_REPLACE_VAL(reg_val, 0);
    ahs_reg->ACR = reg_val;
}
#endif

