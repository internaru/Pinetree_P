/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the 3x3 NGA block.
  * 
 **/

#include <stdint.h>
#include "logger.h"
#include "nga_api.h"
#include "logger.h"

#define DBG_PRFX "NGA: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

// lsptodo: nga can be 8 or 10 bit indexed so 256 or 1024 indexes.
//  is a single block shared between scan and print ?
//
const int LUTDEPTH = 256;

// initialize the NGA registers from the passed configuration struct
void nga_setup(NGADJUST_DAVINCI_REGS_t *nga_reg, nga_cfg_t *nga_cfg) 
{
    uint32_t reg_val;

    // NGA config 
    reg_val = nga_reg->Config;
    reg_val = NGADJUST_DAVINCI_CONFIG_BYPASS_REPLACE_VAL(reg_val, nga_cfg->bypass);
    reg_val = NGADJUST_DAVINCI_CONFIG_METHOD_REPLACE_VAL(reg_val, nga_cfg->pull_nshift);
    nga_reg->Config = reg_val;

    // NGA perpixenable 
    reg_val = nga_reg->perPixEn;
    reg_val = NGADJUST_DAVINCI_PERPIXEN_SBANDEN_REPLACE_VAL(reg_val, nga_cfg->perpixenable);
    reg_val = NGADJUST_DAVINCI_PERPIXEN_SBANDPOL_REPLACE_VAL(reg_val, nga_cfg->perpixenpolarity);
    reg_val = NGADJUST_DAVINCI_PERPIXEN_SBANDMASK_REPLACE_VAL(reg_val, nga_cfg->perpixenmask);
    reg_val = NGADJUST_DAVINCI_PERPIXEN_SBANDVAL_REPLACE_VAL(reg_val, nga_cfg->perpixenvalue);
    nga_reg->perPixEn = reg_val;
    if (nga_cfg->loadlut) 
    {
        nga_loadlut(nga_reg, nga_cfg->c1lutptr, nga_cfg->c2lutptr);
    }
}

void nga_loadlut(NGADJUST_DAVINCI_REGS_t *nga_reg, uint8_t *c1Ptr, uint8_t *c2Ptr) 
{
    uint32_t reg_val;
    uint32_t lut_val;
    uint32_t i;

    reg_val = nga_reg->Config;
    reg_val = NGADJUST_DAVINCI_CONFIG_CPU_ACCESS_REPLACE_VAL(reg_val, 1);
    nga_reg->Config = reg_val;
    for (i = 0; i < LUTDEPTH; i++) 
    {
        lut_val = NGADJUST_DAVINCI_LUTACCESS_C1_REPLACE_VAL(0, *c1Ptr++);
        lut_val = NGADJUST_DAVINCI_LUTACCESS_C2_REPLACE_VAL(lut_val, *c2Ptr++);
        nga_reg->LUTAccess[i] = lut_val;
    }
    reg_val = NGADJUST_DAVINCI_CONFIG_CPU_ACCESS_REPLACE_VAL(reg_val, 0);
    nga_reg->Config = reg_val;
}

void nga_print(NGADJUST_DAVINCI_REGS_t *nga_reg) 
{
    uint32_t reg_val;

    DBG_PRINTF_NOTICE( "Current NGA Register Values:\n\r");
    DBG_PRINTF_NOTICE( "\n\r");
    reg_val = nga_reg->Config;
    DBG_PRINTF_NOTICE( "  NGA Config                : (%08x)\n\r", reg_val);
    DBG_PRINTF_NOTICE( "      Bypass              : (%01x)\n\r", NGADJUST_DAVINCI_CONFIG_BYPASS_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      Method (pull_nshift): (%01x)\n\r", NGADJUST_DAVINCI_CONFIG_METHOD_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      CPU Access          : (%01x)\n\r", NGADJUST_DAVINCI_CONFIG_CPU_ACCESS_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "\n\r");

    reg_val = nga_reg->perPixEn;
    DBG_PRINTF_NOTICE( "  NGA Per Pixel Enable Reg  : (%08x)\n\r", reg_val);
    DBG_PRINTF_NOTICE( "      Enable              : (%01x)\n\r", NGADJUST_DAVINCI_PERPIXEN_SBANDEN_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      Polarity            : (%01x)\n\r", NGADJUST_DAVINCI_PERPIXEN_SBANDPOL_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      Mask                : (%02x)\n\r", NGADJUST_DAVINCI_PERPIXEN_SBANDMASK_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "      Value               : (%02x)\n\r", NGADJUST_DAVINCI_PERPIXEN_SBANDVAL_MASK_SHIFT(reg_val));
    DBG_PRINTF_NOTICE( "\n\r");
}
