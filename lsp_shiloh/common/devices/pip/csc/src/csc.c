/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the 3x3 CSC block.
  * 
 **/

#include <stdint.h>
#include "csc_api.h"
#include "logger.h"

#define DBG_PRFX "CSC: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

const int ILUTAWIDTH = 8;
const int ILUTDWIDTH = 12;
const int ILUTDEPTH = 768;
const int OLUTAWIDTH = 8;
const int OLUTDWIDTH = 8;
const int OLUTDEPTH = 768;

// initialize the CSC registers from the passed configuration struct
void csc_setup(CSC_DAVINCI_REGS_t *csc_reg, csc_cfg_t *csc_cfg) 
{
    uint32_t reg_val;

    // CSC config 
    reg_val = csc_reg->CCR;
    reg_val = CSC_DAVINCI_CCR_BYPASSALL_REPLACE_VAL(reg_val, csc_cfg->bypass);
    reg_val = CSC_DAVINCI_CCR_OFFSETBYPASS_REPLACE_VAL(reg_val, csc_cfg->offset_bypass);
    reg_val = CSC_DAVINCI_CCR_PREOFFBYPASS_REPLACE_VAL(reg_val, csc_cfg->preoffset_bypass);
    reg_val = CSC_DAVINCI_CCR_POSTOFFBYPASS_REPLACE_VAL(reg_val, csc_cfg->postoffset_bypass);
    reg_val = CSC_DAVINCI_CCR_ILUTBYPASS_REPLACE_VAL(reg_val, csc_cfg->inlut_bypass);
    reg_val = CSC_DAVINCI_CCR_OLUTBYPASS_REPLACE_VAL(reg_val, csc_cfg->outlut_bypass);
    csc_reg->CCR = reg_val;

    // Coefficient registers
    csc_loadcoeff(csc_reg, csc_cfg->coeff);

    if (csc_cfg->loadinlut) 
    {
        csc_loadinlut(csc_reg, csc_cfg->inlutptr, csc_cfg->ilor);
    }

    if (csc_cfg->loadoutlut) 
    {
        csc_loadoutlut(csc_reg, csc_cfg->outlutptr, csc_cfg->olor);
    }
}

void csc_loadinlut(CSC_DAVINCI_REGS_t *csc_reg, uint32_t *ptr, uint32_t overflow [3]) 
{
    uint32_t reg_val;
    uint32_t i;

    reg_val = csc_reg->CCR;
    reg_val = CSC_DAVINCI_CCR_CPUACCESS_REPLACE_VAL(reg_val, 1);
    csc_reg->CCR = reg_val;
    for (i = 0; i < ILUTDEPTH; i++) 
    {
        csc_reg->ILIW[i] = *ptr++;
    }
    csc_reg->ILOR0 = overflow[0];
    csc_reg->ILOR1 = overflow[1];
    csc_reg->ILOR2 = overflow[2];
    reg_val = CSC_DAVINCI_CCR_CPUACCESS_REPLACE_VAL(reg_val, 0);
    csc_reg->CCR = reg_val;
}

void csc_loadoutlut(CSC_DAVINCI_REGS_t *csc_reg, uint32_t *ptr, uint32_t overflow [3]) 
{
    uint32_t reg_val;
    uint32_t i;

    reg_val = csc_reg->CCR;
    reg_val = CSC_DAVINCI_CCR_CPUACCESS_REPLACE_VAL(reg_val, 1);
    csc_reg->CCR = reg_val;
    for (i = 0; i < OLUTDEPTH; i++) 
    {
        csc_reg->OLIW[i] = *ptr++;
    }
    csc_reg->OLOR0 = overflow[0];
    csc_reg->OLOR1 = overflow[1];
    csc_reg->OLOR2 = overflow[2];
    reg_val = CSC_DAVINCI_CCR_CPUACCESS_REPLACE_VAL(reg_val, 0);
    csc_reg->CCR = reg_val;
}

void csc_loadcoeff(CSC_DAVINCI_REGS_t *csc_reg, uint32_t coeff[9]) 
{
    csc_reg->MCR0 = coeff[0];
    csc_reg->MCR1 = coeff[1];
    csc_reg->MCR2 = coeff[2];
    csc_reg->MCR3 = coeff[3];
    csc_reg->MCR4 = coeff[4];
    csc_reg->MCR5 = coeff[5];
    csc_reg->MCR6 = coeff[6];
    csc_reg->MCR7 = coeff[7];
    csc_reg->MCR8 = coeff[8];
}

void csc_print(CSC_DAVINCI_REGS_t *csc_reg) 
{
    uint32_t reg_val;

    DBG_PRINTF_NOTICE( "Current CSC Register Values:\n\r");
    DBG_PRINTF_NOTICE( "\n\r");
    reg_val = csc_reg->CCR;
    DBG_PRINTF_NOTICE( "  CSC CCR (config)       : (%08x)\n\r", reg_val);
    DBG_PRINTF_NOTICE( "      Bypass             : (%08x)\n\r", CSC_DAVINCI_CCR_BYPASSALL_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      offset Bypass      : (%08x)\n\r", CSC_DAVINCI_CCR_OFFSETBYPASS_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      pre-offset Bypass  : (%08x)\n\r", CSC_DAVINCI_CCR_PREOFFBYPASS_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      post-offset Bypass : (%08x)\n\r", CSC_DAVINCI_CCR_POSTOFFBYPASS_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      input LUT Bypass   : (%08x)\n\r", CSC_DAVINCI_CCR_ILUTBYPASS_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      output LUT Bypass  : (%08x)\n\r", CSC_DAVINCI_CCR_OLUTBYPASS_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      CPU Access         : (%08x)\n\r", CSC_DAVINCI_CCR_CPUACCESS_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "\n\r");

    DBG_PRINTF_NOTICE( "  CSC coefficient0       : (%08x)\n\r", csc_reg->MCR0);
    DBG_PRINTF_NOTICE( "  CSC coefficient1       : (%08x)\n\r", csc_reg->MCR1);
    DBG_PRINTF_NOTICE( "  CSC coefficient2       : (%08x)\n\r", csc_reg->MCR2);
    DBG_PRINTF_NOTICE( "  CSC coefficient3       : (%08x)\n\r", csc_reg->MCR3);
    DBG_PRINTF_NOTICE( "  CSC coefficient4       : (%08x)\n\r", csc_reg->MCR4);
    DBG_PRINTF_NOTICE( "  CSC coefficient5       : (%08x)\n\r", csc_reg->MCR5);
    DBG_PRINTF_NOTICE( "  CSC coefficient6       : (%08x)\n\r", csc_reg->MCR6);
    DBG_PRINTF_NOTICE( "  CSC coefficient7       : (%08x)\n\r", csc_reg->MCR7);
    DBG_PRINTF_NOTICE( "  CSC coefficient8       : (%08x)\n\r", csc_reg->MCR8);
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "  CSC ILOR1              : (%08x)\n\r", csc_reg->ILOR0);
    DBG_PRINTF_NOTICE( "  CSC ILOR2              : (%08x)\n\r", csc_reg->ILOR1);
    DBG_PRINTF_NOTICE( "  CSC ILOR3              : (%08x)\n\r", csc_reg->ILOR2);
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "  CSC OLOR1              : (%08x)\n\r", csc_reg->OLOR0);
    DBG_PRINTF_NOTICE( "  CSC OLOR2              : (%08x)\n\r", csc_reg->OLOR1);
    DBG_PRINTF_NOTICE( "  CSC OLOR3              : (%08x)\n\r", csc_reg->OLOR2);
}
