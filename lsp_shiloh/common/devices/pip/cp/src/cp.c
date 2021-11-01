/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for Contone Pad (CP). 
  * 
 **/

#ifndef __CP_FUNC_C_INCLUDED
#define __CP_FUNC_C_INCLUDED

#include <stdint.h>
#include "cp_api.h"
#include "logger.h"

#define DBG_PRFX "CP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

void cp_setup(TP_CONTONE_REGS_t *cp_reg, cp_cfg_t *cp_cfg) 
{
    uint32_t reg_value;

    // Write the Configuration Register
    reg_value = cp_reg->Cfg;
    reg_value = TP_CONTONE_CFG_BPP_REPLACE_VAL(reg_value,cp_cfg->bpp);
    reg_value = TP_CONTONE_CFG_OUTSEL_REPLACE_VAL(reg_value,cp_cfg->outsel);
    reg_value = TP_CONTONE_CFG_PIXORDER_REPLACE_VAL(reg_value,cp_cfg->pixorder);
    cp_reg->Cfg = reg_value; 

    // Write the Left Pad Configuration Register
    reg_value = cp_reg->LPad;
    reg_value = TP_CONTONE_LPAD_LEFTEDGEFILL_REPLACE_VAL(reg_value,cp_cfg->leftedgefill);
    reg_value = TP_CONTONE_LPAD_LEFTLEN_REPLACE_VAL(reg_value,cp_cfg->leftlen);
    cp_reg->LPad = reg_value;

    // Write the Left Pad Value Register
    reg_value = cp_reg->LVal;
    reg_value = TP_CONTONE_LVAL_LEFTVAL_REPLACE_VAL(reg_value,cp_cfg->leftval);
    cp_reg->LVal = reg_value;

    // Write the Right Pad Configuration Register
    reg_value = cp_reg->RPad;
    reg_value = TP_CONTONE_RPAD_RIGHTEDGEFILL_REPLACE_VAL(reg_value,cp_cfg->rightedgefill);
    reg_value = TP_CONTONE_RPAD_RIGHTLEN_REPLACE_VAL(reg_value,cp_cfg->rightlen);
    cp_reg->RPad = reg_value;

    // Write the Right Pad Value Register
    reg_value = cp_reg->RVal;
    reg_value = TP_CONTONE_RVAL_RIGHTVAL_REPLACE_VAL(reg_value,cp_cfg->rightval);
    cp_reg->RVal = reg_value;
}

void cp_print(TP_CONTONE_REGS_t *cp_reg)
{
    DBG_PRINTF_NOTICE( "CP Register Base Addr:0x%08x\n", cp_reg);
    DBG_PRINTF_NOTICE( "Current CP Register Vals:\n\r");
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "   CP.LPad LeftEdgeFill  : (%08x)\n\r", TP_CONTONE_LPAD_LEFTEDGEFILL_MASK_SHIFT(cp_reg->LPad));
    DBG_PRINTF_NOTICE( "   CP.LPad LeftLen       : (%08x)\n\r", TP_CONTONE_LPAD_LEFTLEN_MASK_SHIFT(cp_reg->LPad));
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "   CP.LVal               : (%08x)\n\r", TP_CONTONE_LVAL_LEFTVAL_MASK_SHIFT(cp_reg->LVal));
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "   CP.RPad RightEdgeFill : (%08x)\n\r", TP_CONTONE_RPAD_RIGHTEDGEFILL_MASK_SHIFT(cp_reg->RPad));
    DBG_PRINTF_NOTICE( "   CP.RPad RightLen      : (%08x)\n\r", TP_CONTONE_RPAD_RIGHTLEN_MASK_SHIFT(cp_reg->RPad));
    DBG_PRINTF_NOTICE( "\n\r");
    DBG_PRINTF_NOTICE( "   CP.RVal               : (%08x)\n\r", TP_CONTONE_RVAL_RIGHTVAL_MASK_SHIFT(cp_reg->RVal));
}

#endif
