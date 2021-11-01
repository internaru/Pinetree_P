/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for Text Pad (TP). 
  * 
 **/

#ifndef __TP_FUNC_C_INCLUDED
#define __TP_FUNC_C_INCLUDED

#include <stdint.h>
#include "tp_api.h"
#include "logger.h"

#define DBG_PRFX "TP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

void tp_setup(TP_TEXT_REGS_t *tp_reg, tp_cfg_t *tp_cfg) 
{
    uint32_t reg_value;

    // Write the Configuration Register
    reg_value = tp_reg->Cfg;
    reg_value = TP_TEXT_CFG_BPP_REPLACE_VAL(reg_value,tp_cfg->bpp);
    reg_value = TP_TEXT_CFG_OUTSEL_REPLACE_VAL(reg_value,tp_cfg->outsel);
    reg_value = TP_TEXT_CFG_PIXORDER_REPLACE_VAL(reg_value,tp_cfg->pixorder);
    tp_reg->Cfg = reg_value; 

    // Write the Left Pad Configuration Register
    reg_value = tp_reg->LPad;
    reg_value = TP_TEXT_LPAD_LEFTEDGEFILL_REPLACE_VAL(reg_value,tp_cfg->leftedgefill);
    reg_value = TP_TEXT_LPAD_LEFTLEN_REPLACE_VAL(reg_value,tp_cfg->leftlen);
    tp_reg->LPad = reg_value;

    // Write the Left Pad Value Register
    reg_value = tp_reg->LVal;
    reg_value = TP_TEXT_LVAL_LEFTVAL_REPLACE_VAL(reg_value,tp_cfg->leftval);
    tp_reg->LVal = reg_value;

    // Write the Right Pad Configuration Register
    reg_value = tp_reg->RPad;
    reg_value = TP_TEXT_RPAD_RIGHTEDGEFILL_REPLACE_VAL(reg_value,tp_cfg->rightedgefill);
    reg_value = TP_TEXT_RPAD_RIGHTLEN_REPLACE_VAL(reg_value,tp_cfg->rightlen);
    tp_reg->RPad = reg_value;

    // Write the Right Pad Value Register
    reg_value = tp_reg->RVal;
    reg_value = TP_TEXT_RVAL_RIGHTVAL_REPLACE_VAL(reg_value,tp_cfg->rightval);
    tp_reg->RVal = reg_value;
}

void tp_print(TP_TEXT_REGS_t *tp_reg) 
{
    DBG_PRINTF_NOTICE("TP Register Base Addr:0x%08x\n", tp_reg);
    DBG_PRINTF_NOTICE("Current TP Register Vals:\n\r");
    DBG_PRINTF_NOTICE("   TP.Cfg  BPP           : (%08x)\n\r", TP_TEXT_CFG_BPP_MASK_SHIFT(tp_reg->Cfg));
    DBG_PRINTF_NOTICE("   TP.Cfg  OutSel        : (%08x)\n\r", TP_TEXT_CFG_OUTSEL_MASK_SHIFT(tp_reg->Cfg));
    DBG_PRINTF_NOTICE("   TP.Cfg  PixOrder      : (%08x)\n\r", TP_TEXT_CFG_PIXORDER_MASK_SHIFT(tp_reg->Cfg));
    DBG_PRINTF_NOTICE("\n\r");
    DBG_PRINTF_NOTICE("   TP.LPad LeftEdgeFill  : (%08x)\n\r", TP_TEXT_LPAD_LEFTEDGEFILL_MASK_SHIFT(tp_reg->LPad));
    DBG_PRINTF_NOTICE("   TP.LPad LeftLen       : (%08x)\n\r", TP_TEXT_LPAD_LEFTLEN_MASK_SHIFT(tp_reg->LPad));
    DBG_PRINTF_NOTICE("\n\r");
    DBG_PRINTF_NOTICE("   TP.LVal               : (%08x)\n\r", TP_TEXT_LVAL_LEFTVAL_MASK_SHIFT(tp_reg->LVal));
    DBG_PRINTF_NOTICE("\n\r");
    DBG_PRINTF_NOTICE("   TP.RPad RightEdgeFill : (%08x)\n\r", TP_TEXT_RPAD_RIGHTEDGEFILL_MASK_SHIFT(tp_reg->RPad));
    DBG_PRINTF_NOTICE("   TP.RPad RightLen      : (%08x)\n\r", TP_TEXT_RPAD_RIGHTLEN_MASK_SHIFT(tp_reg->RPad));
    DBG_PRINTF_NOTICE("\n\r");
    DBG_PRINTF_NOTICE("   TP.RVal               : (%08x)\n\r", TP_TEXT_RVAL_RIGHTVAL_MASK_SHIFT(tp_reg->RVal));
}

#endif
