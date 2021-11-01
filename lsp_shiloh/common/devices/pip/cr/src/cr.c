/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the CR block.
  * 
 **/

#include <stdint.h>
#include "cr_api.h"
#include "logger.h"
#include "lassert.h"

#define DBG_PRFX "CR: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

// initialize the CR registers from the passed configuration
void cr_setup(CR_REGS_t *cr_reg, cr_cfg_t *cr_cfg) 
{
    uint32_t reg_val;

    // CR_REP0-3 registers
    reg_val = cr_reg->REP0;
    reg_val = CR_REP0_REP0CH0_REPLACE_VAL (reg_val, (cr_cfg->rep[0] >> 16) & 0xff);
    reg_val = CR_REP0_REP0CH1_REPLACE_VAL (reg_val, (cr_cfg->rep[0] >> 8) & 0xff);
    reg_val = CR_REP0_REP0CH2_REPLACE_VAL (reg_val, (cr_cfg->rep[0]) & 0xff);
    cr_reg->REP0 = reg_val;

    reg_val = cr_reg->REP1;
    reg_val = CR_REP1_REP1CH0_REPLACE_VAL (reg_val, (cr_cfg->rep[1] >> 16) & 0xff);
    reg_val = CR_REP1_REP1CH1_REPLACE_VAL (reg_val, (cr_cfg->rep[1] >> 8) & 0xff);
    reg_val = CR_REP1_REP1CH2_REPLACE_VAL (reg_val, (cr_cfg->rep[1]) & 0xff);
    cr_reg->REP1 = reg_val;
#if 0 // ! granite
    reg_val = cr_reg->REP2;
    reg_val = CR_REP2_REP2CH0_REPLACE_VAL (reg_val, (cr_cfg->rep[2] >> 16) & 0xff);
    reg_val = CR_REP2_REP2CH1_REPLACE_VAL (reg_val, (cr_cfg->rep[2] >> 8) & 0xff);
    reg_val = CR_REP2_REP2CH2_REPLACE_VAL (reg_val, (cr_cfg->rep[2]) & 0xff);
    cr_reg->REP2 = reg_val;

    reg_val = cr_reg->REP3;
    reg_val = CR_REP3_REP3CH0_REPLACE_VAL (reg_val, (cr_cfg->rep[3] >> 16) & 0xff);
    reg_val = CR_REP3_REP3CH1_REPLACE_VAL (reg_val, (cr_cfg->rep[3] >> 8) & 0xff);
    reg_val = CR_REP3_REP3CH2_REPLACE_VAL (reg_val, (cr_cfg->rep[3]) & 0xff);
    cr_reg->REP3 = reg_val;
#endif
    // CR_UPPER0-3 registers
    reg_val = cr_reg->UPPER0;
    reg_val = CR_UPPER0_UP0CH0_REPLACE_VAL (reg_val, (cr_cfg->upper[0] >> 16) & 0xff);
    reg_val = CR_UPPER0_UP0CH1_REPLACE_VAL (reg_val, (cr_cfg->upper[0] >> 8) & 0xff);
    reg_val = CR_UPPER0_UP0CH2_REPLACE_VAL (reg_val, (cr_cfg->upper[0]) & 0xff);
    cr_reg->UPPER0 = reg_val;

    reg_val = cr_reg->UPPER1;
    reg_val = CR_UPPER1_UP1CH0_REPLACE_VAL (reg_val, (cr_cfg->upper[1] >> 16) & 0xff);
    reg_val = CR_UPPER1_UP1CH1_REPLACE_VAL (reg_val, (cr_cfg->upper[1] >> 8) & 0xff);
    reg_val = CR_UPPER1_UP1CH2_REPLACE_VAL (reg_val, (cr_cfg->upper[1]) & 0xff);
    cr_reg->UPPER1 = reg_val;
#if 0
    reg_val = cr_reg->UPPER2;
    reg_val = CR_UPPER2_UP2CH0_REPLACE_VAL (reg_val, (cr_cfg->upper[2] >> 16) & 0xff);
    reg_val = CR_UPPER2_UP2CH1_REPLACE_VAL (reg_val, (cr_cfg->upper[2] >> 8) & 0xff);
    reg_val = CR_UPPER2_UP2CH2_REPLACE_VAL (reg_val, (cr_cfg->upper[2]) & 0xff);
    cr_reg->UPPER2 = reg_val;

    reg_val = cr_reg->UPPER3;
    reg_val = CR_UPPER3_UP3CH0_REPLACE_VAL (reg_val, (cr_cfg->upper[3] >> 16) & 0xff);
    reg_val = CR_UPPER3_UP3CH1_REPLACE_VAL (reg_val, (cr_cfg->upper[3] >> 8) & 0xff);
    reg_val = CR_UPPER3_UP3CH2_REPLACE_VAL (reg_val, (cr_cfg->upper[3]) & 0xff);
    cr_reg->UPPER3 = reg_val;
#endif 
    // CR_LOWER0-3 registers
    reg_val = cr_reg->LOWER0;
    reg_val = CR_LOWER0_LO0CH0_REPLACE_VAL (reg_val, (cr_cfg->lower[0] >> 16) & 0xff);
    reg_val = CR_LOWER0_LO0CH1_REPLACE_VAL (reg_val, (cr_cfg->lower[0] >> 8) & 0xff);
    reg_val = CR_LOWER0_LO0CH2_REPLACE_VAL (reg_val, (cr_cfg->lower[0]) & 0xff);
    cr_reg->LOWER0 = reg_val;

    reg_val = cr_reg->LOWER1;
    reg_val = CR_LOWER1_LO1CH0_REPLACE_VAL (reg_val, (cr_cfg->lower[1] >> 16) & 0xff);
    reg_val = CR_LOWER1_LO1CH1_REPLACE_VAL (reg_val, (cr_cfg->lower[1] >> 8) & 0xff);
    reg_val = CR_LOWER1_LO1CH2_REPLACE_VAL (reg_val, (cr_cfg->lower[1]) & 0xff);
    cr_reg->LOWER1 = reg_val;
#if 0 
    reg_val = cr_reg->LOWER2;
    reg_val = CR_LOWER2_LO2CH0_REPLACE_VAL (reg_val, (cr_cfg->lower[2] >> 16) & 0xff);
    reg_val = CR_LOWER2_LO2CH1_REPLACE_VAL (reg_val, (cr_cfg->lower[2] >> 8) & 0xff);
    reg_val = CR_LOWER2_LO2CH2_REPLACE_VAL (reg_val, (cr_cfg->lower[2]) & 0xff);
    cr_reg->LOWER2 = reg_val;

    reg_val = cr_reg->LOWER3;
    reg_val = CR_LOWER3_LO3CH0_REPLACE_VAL (reg_val, (cr_cfg->lower[3] >> 16) & 0xff);
    reg_val = CR_LOWER3_LO3CH1_REPLACE_VAL (reg_val, (cr_cfg->lower[3] >> 8) & 0xff);
    reg_val = CR_LOWER3_LO3CH2_REPLACE_VAL (reg_val, (cr_cfg->lower[3]) & 0xff);
    cr_reg->LOWER3 = reg_val;

    // CR_PIX_EN registers
    reg_val = cr_reg->PIX_EN;
    reg_val = CR_PIX_EN_SBANDEN_REPLACE_VAL   (reg_val, cr_cfg->sbanden);
    reg_val = CR_PIX_EN_SBANDPOL_REPLACE_VAL  (reg_val, cr_cfg->sbandpol);
    reg_val = CR_PIX_EN_SBANDVAL_REPLACE_VAL  (reg_val, cr_cfg->sbandval);
    reg_val = CR_PIX_EN_SBANDMASK_REPLACE_VAL (reg_val, cr_cfg->sbandmask);
    cr_reg->PIX_EN = reg_val;
#endif
    // cr_cfg registers
    reg_val = cr_reg->CFG;
    ASSERT(0 == 1); // not ported yet granite 
    //    reg_val = CR_CFG_ZXEN_REPLACE_VAL     (reg_val, cr_cfg->zxen);
    //    reg_val = CR_CFG_JBIGEN_REPLACE_VAL   (reg_val, cr_cfg->jbigen);
    //    reg_val = CR_CFG_SBANDMAP_REPLACE_VAL (reg_val, cr_cfg->sbandmap);
    //    reg_val = CR_CFG_BYPASS_REPLACE_VAL   (reg_val, cr_cfg->bypass);
    ASSERT(0 == 1); // not ported yet
    cr_reg->CFG = reg_val;
}

void cr_clear(CR_REGS_t *cr_reg) 
{
    cr_reg->REP0   = 0;
    cr_reg->REP1   = 0;
    //cr_reg->REP2   = 0;
    //cr_reg->REP3   = 0;
    cr_reg->UPPER0 = 0;
    cr_reg->UPPER1 = 0;
    //cr_reg->UPPER2 = 0;
    //cr_reg->UPPER3 = 0;
    cr_reg->LOWER0 = 0;
    cr_reg->LOWER1 = 0;
    //cr_reg->LOWER2 = 0;
    //cr_reg->LOWER3 = 0;
    //cr_reg->PIX_EN = 0;
    cr_reg->CFG    = 0;
}

void CR_print(CR_REGS_t *cr_reg) 
{
    DBG_PRINTF_NOTICE( "Current CR Register Vals:\n\r");
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "   CR CFG        : (%08x)\n\r", cr_reg->CFG);
    //DBG_PRINTF_NOTICE( "       ZXEN      : (%08x)\n\r", CR_CFG_ZXEN_MASK_SHIFT(cr_reg->CFG));
    //DBG_PRINTF_NOTICE( "       JBIGEN    : (%08x)\n\r", CR_CFG_JBIGEN_MASK_SHIFT(cr_reg->CFG));
    //DBG_PRINTF_NOTICE( "       SBANDMAP  : (%08x)\n\r", CR_CFG_SBANDMAP_MASK_SHIFT(cr_reg->CFG));
    //DBG_PRINTF_NOTICE( "       BYPASS    : (%08x)\n\r", CR_CFG_BYPASS_MASK_SHIFT(cr_reg->CFG));
    //DBG_PRINTF_NOTICE( "\n\r");
    //DBG_PRINTF_NOTICE( "   CR PIX_EN     : (%08x)\n\r", cr_reg->PIX_EN);
    //DBG_PRINTF_NOTICE( "       SBANDEN   : (%08x)\n\r", CR_PIX_EN_SBANDEN_MASK_SHIFT(cr_reg->PIX_EN));
    //DBG_PRINTF_NOTICE( "       SBANDPOL  : (%08x)\n\r", CR_PIX_EN_SBANDPOL_MASK_SHIFT(cr_reg->PIX_EN));
    //DBG_PRINTF_NOTICE( "       SBANDVAL  : (%08x)\n\r", CR_PIX_EN_SBANDVAL_MASK_SHIFT(cr_reg->PIX_EN));
    //DBG_PRINTF_NOTICE( "       SBANDMASK : (%08x)\n\r", CR_PIX_EN_SBANDMASK_MASK_SHIFT(cr_reg->PIX_EN));
    //DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "   CR LOWER0     : (%08x)\n\r", cr_reg->LOWER0);
    DBG_PRINTF_NOTICE( "   CR UPPER0     : (%08x)\n\r", cr_reg->UPPER0);
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "   CR LOWER1     : (%08x)\n\r", cr_reg->LOWER1);
    DBG_PRINTF_NOTICE( "   CR UPPER1     : (%08x)\n\r", cr_reg->UPPER1);
    DBG_PRINTF_NOTICE( "\n\r");
    //DBG_PRINTF_NOTICE( "   CR LOWER2     : (%08x)\n\r", cr_reg->LOWER2);
    //DBG_PRINTF_NOTICE( "   CR UPPER2     : (%08x)\n\r", cr_reg->UPPER2);
    //DBG_PRINTF_NOTICE( "\n\r");
    //DBG_PRINTF_NOTICE( "   CR LOWER3     : (%08x)\n\r", cr_reg->LOWER3);
    //DBG_PRINTF_NOTICE( "   CR UPPER3     : (%08x)\n\r", cr_reg->UPPER3);
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "   CR REP0       : (%08x)\n\r", cr_reg->REP0);
    DBG_PRINTF_NOTICE( "   CR REP1       : (%08x)\n\r", cr_reg->REP1);
    //DBG_PRINTF_NOTICE( "   CR REP2       : (%08x)\n\r", cr_reg->REP2);
    //DBG_PRINTF_NOTICE( "   CR REP3       : (%08x)\n\r", cr_reg->REP3);
}

void CR_start(CR_REGS_t *cr_reg, cr_cfg_t *cr_cfg) 
{
    // DO NOTHING!!
}
