/*
 * ============================================================================
 * Copyright (c) 2010-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file mrvl_text_pad.c
 * 
 * \brief
 * 
 **/
#include <stdint.h>
#include <stdbool.h>

#include "ATypes.h"
#include "color_reg_enhance_cfg.h"
#include "lassert.h"
#include "error_types.h"
#include "logger.h"

#include "asic.h"
//#include "CRE_regmasks.h"
//#include "CRE_regstructs.h"

#include "regAddrs.h"



/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */

#define DBG_PRFX "CRE: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )
#define CRE_DBG_LVL (LOG_INFO | DBG_PIP)
#define CRE_LOUD (DBG_LOUD | DBG_OUTPUT)


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */




/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */
#include "asic.h"

extern  CRE_RASTER_REGS_t *cre_reg;



/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */





/* -------------------------------------------------------------------------- */
/*                     E X P O R T E D  F U N C T I O N S                     */
/* -------------------------------------------------------------------------- */

void color_reg_enhance_init()
{
    //  cre_reg = (CRE_RASTER_REGS_t *) PIP_CRE_BASE;

   DBG_PRINTF_DEBUG( "%s ENTRY reg base address: 0x%x\n", __func__, cre_reg);

}


void color_reg_enhance_dump_regs( void )
{
    uint32_t reg_val;
    uint32_t i;

    DBG_PRINTF_ERR("\n");
    DBG_PRINTF_ERR("Color Reg Enhance control       0x%x\n", cre_reg->CNTRL);
    DBG_PRINTF_ERR("Color Reg Enhance core control  0x%x\n", cre_reg->CORE_CNTRL);
    DBG_PRINTF_ERR("Color Reg Enhance edge control  0x%x\n", cre_reg->EDGE_CNTRL);
    DBG_PRINTF_ERR("Color Reg Enhance pile control  0x%x\n", cre_reg->PILE_CNTRL);
    DBG_PRINTF_ERR("Color Reg Enhance sharp control 0x%x\n", cre_reg->SHARP_CNTRL);
    DBG_PRINTF_ERR("Color Reg Enhance trap control  0x%x\n", cre_reg->TRAP_CNTRL);
   // DBG_PRINTF_ERR, ("Color Reg Enhance 0x%x\n", cre_reg->);

    DBG_PRINTF_ERR("Color Reg Enhance core scale C  %d\n", cre_reg->CORE_SCALE_C);
    DBG_PRINTF_ERR("Color Reg Enhance core scale M  %d\n", cre_reg->CORE_SCALE_M);
    DBG_PRINTF_ERR("Color Reg Enhance core scale Y  %d\n", cre_reg->CORE_SCALE_Y);
    DBG_PRINTF_ERR("Color Reg Enhance core scale K  %d\n", cre_reg->CORE_SCALE_K);

    DBG_PRINTF_ERR("Color Reg Enhance core threshold C  %d\n", cre_reg->CORE_T_C);   
    DBG_PRINTF_ERR("Color Reg Enhance core threshold M  %d\n", cre_reg->CORE_T_M);   
    DBG_PRINTF_ERR("Color Reg Enhance core threshold Y  %d\n", cre_reg->CORE_T_Y);   
    DBG_PRINTF_ERR("Color Reg Enhance core threshold K  %d\n", cre_reg->CORE_T_K);   

    DBG_PRINTF_ERR("\n");
    DBG_PRINTF_ERR("CRE Status 0x%x\n", cre_reg->STATUS);  ///< 0x0 [R]: Status Register
    // DBG_PRINTF_ERR,("CRE  0x%x\n", cre_reg->CLR_STATUS);  ///< 0x4 [W]: CLEAR STATUS REG

    DBG_PRINTF_ERR("CRE Control 0x%x\n", cre_reg->CNTRL);  ///< 0x8 [R/W]: CRE  0x%xCONTROL REG

    DBG_PRINTF_ERR("CRE Trap control 0x%x\n", cre_reg->TRAP_CNTRL);  ///< 0xc [R/W]: TRAPPING CONTROL REG

    DBG_PRINTF_ERR("\n");
    DBG_PRINTF_ERR("CRE trap cyan threshold    %d\n", cre_reg->TRAP_T_C);  ///< 0x10 [R/W]: TRAP CYAN THRESHOLD
    DBG_PRINTF_ERR("CRE trap magenta threshold %d\n", cre_reg->TRAP_T_M);  ///< 0x14 [R/W]: TRAP MAGENTA THRESHOLD
    DBG_PRINTF_ERR("CRE trap yellow threshold  %d\n", cre_reg->TRAP_T_Y);  ///< 0x18 [R/W]: TRAP YELLOW THRESHOLD
    DBG_PRINTF_ERR("CRE trap black threshold   %d\n", cre_reg->TRAP_T_K);  ///< 0x1c [R/W]: TRAP BLACK TRESHOLD

    DBG_PRINTF_ERR("CRE trap intensity percent limit %d\n", cre_reg->TRAP_IPLIM);  ///< 0x20 [R/W]: TRAP INTENSITY PERCENTAGE LIMIT
    DBG_PRINTF_ERR("CRE trap intensity threshold %d\n", cre_reg->TRAP_I_T);  ///< 0x24 [R/W]: TRAP INTENSITY THRESHOLD
    DBG_PRINTF_ERR("CRE trap black line threshold %d\n", cre_reg->TRAP_KLINE_T);  ///< 0x28 [R/W]: TRAP BLACK LINE THRESHOLD
    DBG_PRINTF_ERR("CRE adjacent black pixels per line %d\n", cre_reg->TRAP_KPLINE);  ///< 0x2c [R/W]: ADJACENT BLACK PIXELS PER LINE
    DBG_PRINTF_ERR("CRE trap maximum %d\n", cre_reg->TRAP_MAX);  ///< 0x30 [R/W]: TRAP MAXIMUM

    //  DBG_PRINTF_ERR,("CRE  0x%x\n", cre_reg->reserved0[3]);


    // *** Enable CPU to access LUTs. ***
    reg_val = cre_reg->CNTRL;
    reg_val = REPLACE_VAL( CRE, CNTRL, CPUACCESS, reg_val, 1);
    cre_reg->CNTRL = reg_val;

    // TRAP Cyan Intensity LUT
    DBG_PRINTF_ERR("\n");
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTCI)/sizeof(cre_reg->TRAP_LUTCI[0])); i++)
    {
        DBG_PRINTF_ERR("Cyan Intensity LUT entry %d = %d\n", i, cre_reg->TRAP_LUTCI[i]);
    }

    // TRAP Magenta Intensity LUT
    DBG_PRINTF_ERR("\n");
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTMI)/sizeof(cre_reg->TRAP_LUTMI[0])); i++)
    {
        DBG_PRINTF_ERR("Magenta Intensity LUT entry %d = %d\n", i, cre_reg->TRAP_LUTMI[i]);
    }

    // TRAP Yellow Intensity LUT
    DBG_PRINTF_ERR("\n");
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTYI)/sizeof(cre_reg->TRAP_LUTYI[0])); i++)
    {
        DBG_PRINTF_ERR("Yellow Intensity LUT entry %d = %d\n", i, cre_reg->TRAP_LUTYI[i]);
    }

    // TRAP Black Intensity LUT
    DBG_PRINTF_ERR("\n");
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTKI)/sizeof(cre_reg->TRAP_LUTKI[0])); i++)
    {
        DBG_PRINTF_ERR("Black Intensity LUT entry %d = %d\n", i, cre_reg->TRAP_LUTKI[i]);
    }

    // TRAP Cyan Fade LUT
    DBG_PRINTF_ERR("\n");
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTCF)/sizeof(cre_reg->TRAP_LUTCF[0])); i++)
    {
        DBG_PRINTF_ERR("Cyan Fade LUT entry %d = %d\n", i, cre_reg->TRAP_LUTCF[i]);
    }

    // TRAP Magenta Fade LUT
    DBG_PRINTF_ERR("\n");
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTMF)/sizeof(cre_reg->TRAP_LUTMF[0])); i++)
    {
        DBG_PRINTF_ERR("Magenta Fade LUT entry %d = %d\n", i, cre_reg->TRAP_LUTMF[i]);
    }

    // TRAP Yellow Fade LUT
    DBG_PRINTF_ERR("\n");
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTYF)/sizeof(cre_reg->TRAP_LUTYF[0])); i++)
    {
        DBG_PRINTF_ERR("Yellow Fade LUT entry %d = %d\n", i, cre_reg->TRAP_LUTYF[i]);
    }

    // TRAP Black Fade LUT
    DBG_PRINTF_ERR("\n");
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTKF)/sizeof(cre_reg->TRAP_LUTKF[0])); i++)
    {
        DBG_PRINTF_ERR("Black Fade LUT entry %d = %d\n", i, cre_reg->TRAP_LUTKF[i]);
    }

    // *** disable CPU access to LUTs. ***
    reg_val = cre_reg->CNTRL;
    reg_val = REPLACE_VAL( CRE, CNTRL, CPUACCESS, reg_val, 0);
    cre_reg->CNTRL = reg_val;


    DBG_PRINTF_ERR("\n");
    DBG_PRINTF_ERR("CRE Sharp control 0x%x\n", cre_reg->SHARP_CNTRL);  ///< 0x400 [R/W]: SHARPEN CONTROL REG

    DBG_PRINTF_ERR("CRE Sharp scale cyan    %d\n", cre_reg->SHARP_SCALE_C);  ///< 0x404 [R/W]: SHARPEN SCALE CYAN
    DBG_PRINTF_ERR("CRE Sharp scale magenta %d\n", cre_reg->SHARP_SCALE_M);  ///< 0x408 [R/W]: SHARPEN SCALE MAGENTA
    DBG_PRINTF_ERR("CRE Sharp scale yellow  %d\n", cre_reg->SHARP_SCALE_Y);  ///< 0x40c [R/W]: SHARPEN SCALE YELLOW
    DBG_PRINTF_ERR("CRE Sharp scale black   %d\n", cre_reg->SHARP_SCALE_K);  ///< 0x410 [R/W]: SHARPEN SCALE BLACK

    DBG_PRINTF_ERR ("\n");
    DBG_PRINTF_ERR("CRE Sharp offset cyan    %d\n", cre_reg->SHARP_OFFSET_C);  ///< 0x414 [R/W]: SHARPEN OFFSET CYAN
    DBG_PRINTF_ERR("CRE Sharp offset magenta %d\n", cre_reg->SHARP_OFFSET_M);  ///< 0x418 [R/W]: SHARPEN OFFSET MAGENTA
    DBG_PRINTF_ERR("CRE Sharp offset yellow  %d\n", cre_reg->SHARP_OFFSET_Y);  ///< 0x41c [R/W]: SHARPEN OFFSET YELLOW
    DBG_PRINTF_ERR("CRE Sharp offset black   %d\n", cre_reg->SHARP_OFFSET_K);  ///< 0x420 [R/W]: SHARPEN OFFSET BLACK

    DBG_PRINTF_ERR ("\n");
    DBG_PRINTF_ERR("CRE Edge strength control 0x%x\n", cre_reg->EDGE_CNTRL);  ///< 0x430 [R/W]: EDGE STRENGTH CONTROL REG

    DBG_PRINTF_ERR("CRE Edge strength threshold cyan    %d\n", cre_reg->EDGE_T_C);  ///< 0x434 [R/W]: CYAN EDGE STRENGTH THRESHOLD
    DBG_PRINTF_ERR("CRE Edge strength threshold magenta %d\n", cre_reg->EDGE_T_M);  ///< 0x438 [R/W]: MAGENTA EDGE STRENGTH THRESHOLD
    DBG_PRINTF_ERR("CRE Edge strength threshold yellow  %d\n", cre_reg->EDGE_T_Y);  ///< 0x43c [R/W]: YELLOW EDGE STRENGTH THRESHOLD
    DBG_PRINTF_ERR("CRE Edge strength threshold black   %d\n", cre_reg->EDGE_T_K);  ///< 0x440 [R/W]: BLACK EDGE STRENGTH THRESHOLD

    DBG_PRINTF_ERR("CRE Edge strength scale factor cyan    %d\n", cre_reg->EDGE_SCALE_C);  ///< 0x444 [R/W]: CYAN EDGE STRENGTH SCALE FACTOR
    DBG_PRINTF_ERR("CRE Edge strength scale factor magenta %d\n", cre_reg->EDGE_SCALE_M);  ///< 0x448 [R/W]: MAGENTA EDGE STRENGTH SCALE FACTOR
    DBG_PRINTF_ERR("CRE Edge strength scale factor yellow  %d\n", cre_reg->EDGE_SCALE_Y);  ///< 0x44c [R/W]: YELLOW EDGE STRENGTH SCALE FACTOR
    DBG_PRINTF_ERR("CRE Edge strength scale factor black   %d\n", cre_reg->EDGE_SCALE_K);  ///< 0x450 [R/W]: BLACK EDGE STRENGTH SCALE FACTOR

    DBG_PRINTF_ERR("CRE Edge strength offset cyan    %d\n", cre_reg->EDGE_OFFSET_C);  ///< 0x454 [R/W]: CYAN EDGE STRENGTH OFFSET
    DBG_PRINTF_ERR("CRE Edge strength offset magenta %d\n", cre_reg->EDGE_OFFSET_M);  ///< 0x458 [R/W]: MAGENTA EDGE STRENGTH OFFSET
    DBG_PRINTF_ERR("CRE Edge strength offset yellow  %d\n", cre_reg->EDGE_OFFSET_Y);  ///< 0x45c [R/W]: YELLOW EDGE STRENGTH OFFSET
    DBG_PRINTF_ERR("CRE Edge strength offset black   %d\n", cre_reg->EDGE_OFFSET_K);  ///< 0x460 [R/W]: BLACK EDGE STRENGTH OFFSET

    DBG_PRINTF_ERR ("\n");
    DBG_PRINTF_ERR("CRE Coring Control 0x%x\n", cre_reg->CORE_CNTRL);  ///< 0x470 [R/W]: CORING CONTROL REG

    DBG_PRINTF_ERR("CRE Coring threshold cyan    %d\n", cre_reg->CORE_T_C);  ///< 0x474 [R/W]: CORE CYAN THRESHOLD
    DBG_PRINTF_ERR("CRE Coring threshold magenta %d\n", cre_reg->CORE_T_M);  ///< 0x478 [R/W]: CORE MAGENTA THRESHOLD
    DBG_PRINTF_ERR("CRE Coring threshold yellow  %d\n", cre_reg->CORE_T_Y);  ///< 0x47c [R/W]: CORE YELLOW THRESHOLD
    DBG_PRINTF_ERR("CRE Coring threshold black   %d\n", cre_reg->CORE_T_K);  ///< 0x480 [R/W]: CORE BLACK THRESHOLD

    DBG_PRINTF_ERR("CRE Coring threshold CMYK 0x%x\n", cre_reg->CORE_T_CMYK);  ///< 0x484 [R/W]: CORING CMYK THRESHOLD

    DBG_PRINTF_ERR("CRE Coring scale factor cyan    %d\n", cre_reg->CORE_SCALE_C);  ///< 0x488 [R/W]: CORE CYAN SCALE FACTOR
    DBG_PRINTF_ERR("CRE Coring scale factor magenta %d\n", cre_reg->CORE_SCALE_M);  ///< 0x48c [R/W]: CORE MAGENTA SCALE FACTOR
    DBG_PRINTF_ERR("CRE Coring scale factor yellow  %d\n", cre_reg->CORE_SCALE_Y);  ///< 0x490 [R/W]: CORE YELLOW SCALE FACTOR
    DBG_PRINTF_ERR("CRE Coring scale factor black   %d\n", cre_reg->CORE_SCALE_K);  ///< 0x494 [R/W]: CORE BLACK SCALE FACTOR

    DBG_PRINTF_ERR ("\n");
    DBG_PRINTF_ERR("CRE Piling Control 0x%x\n", cre_reg->PILE_CNTRL);  ///< 0x4a0 [R/W]: PILING CONTROL REG
    DBG_PRINTF_ERR("CRE Pile edge threshold 0x%x\n", cre_reg->PILE_ET);  ///< 0x4a4 [R/W]: PILE EDGE THRESHOLD

    DBG_PRINTF_ERR("CRE Pile threshold cyan    %d\n", cre_reg->PILE_T_C);  ///< 0x4a8 [R/W]: PILE CYAN THRESHOLD
    DBG_PRINTF_ERR("CRE Pile threshold magenta %d\n", cre_reg->PILE_T_M);  ///< 0x4ac [R/W]: PILE MAGENTA THRESHOLD
    DBG_PRINTF_ERR("CRE Pile threshold yellow  %d\n", cre_reg->PILE_T_Y);  ///< 0x4b0 [R/W]: PILE YELLOW THRESHOLD
    DBG_PRINTF_ERR("CRE Pile threshold black   %d\n", cre_reg->PILE_T_K);  ///< 0x4b4 [R/W]: PILE BLACK THRESHOLD

    DBG_PRINTF_ERR("CRE Leading edge pile scale cyan    %d\n", cre_reg->PILE_SCALE_LE_C);  ///< 0x4b8 [R/W]: CYAN LEADING EDGE PILE SCALE
    DBG_PRINTF_ERR("CRE Leading edge pile scale magenta %d\n", cre_reg->PILE_SCALE_LE_M);  ///< 0x4bc [R/W]: MAGENTA LEADING EDGE PILE SCALE 
    DBG_PRINTF_ERR("CRE Leading edge pile scale yellow  %d\n", cre_reg->PILE_SCALE_LE_Y);  ///< 0x4c0 [R/W]: YELLOW LEADING EDGE PILE SCALE
    DBG_PRINTF_ERR("CRE Leading edge pile scale black   %d\n", cre_reg->PILE_SCALE_LE_K);  ///< 0x4c4 [R/W]: BLACK LEADING EDGE PILE SCALE
                                                             
    DBG_PRINTF_ERR("CRE Trailing edge pile scale cyan    %d\n", cre_reg->PILE_SCALE_TE_C);  ///< 0x4c8 [R/W]: CYAN TRAILING EDGE PILE SCALE
    DBG_PRINTF_ERR("CRE Trailing edge pile scale magenta %d\n", cre_reg->PILE_SCALE_TE_M);  ///< 0x4cc [R/W]: MAGENTA TRAILING EDGE PILE SCALE 
    DBG_PRINTF_ERR("CRE Trailing edge pile scale yellow  %d\n", cre_reg->PILE_SCALE_TE_Y);  ///< 0x4d0 [R/W]: YELLOW TRAILING EDGE PILE SCALE
    DBG_PRINTF_ERR("CRE Trailing edge pile scale black   %d\n", cre_reg->PILE_SCALE_TE_K);  ///< 0x4d4 [R/W]: BLACK TRAILING EDGE PILE SCALE

}


/*
error_type_t color_reg_enhance_set_cfg(color_reg_enhance_cfg_t *cfg)
{
    uint32_t reg_val = 0;

    reg_val = REPLACE_VAL( CRE, CNTRL, BYPASSALL, reg_val, cfg->bypass);
    reg_val = REPLACE_VAL( CRE, CNTRL, CPUACCESS, reg_val, cfg->enable_cpu_access_to_luts);
    reg_val = REPLACE_VAL( CRE, CNTRL, COE, reg_val, cfg->cyan_channel_enable);
    reg_val = REPLACE_VAL( CRE, CNTRL, MOE, reg_val, cfg->magenta_channel_enable);
    reg_val = REPLACE_VAL( CRE, CNTRL, YOE, reg_val, cfg->yellow_channel_enable);
    reg_val = REPLACE_VAL( CRE, CNTRL, KOE, reg_val, cfg->black_channel_enable);


    // Now that the cfg register shadow is setup, write it to hardware.
    cre_reg->CNTRL = reg_val;

    DBG_PRINTF_ERR("%s set cfg register 0x%x to 0x%x\n", __func__, &cre_reg->CNTRL, cre_reg->CNTRL);

    return OK;
} 
*/ 

error_type_t color_reg_enhance_set_cfg(color_reg_enhance_cfg_t *cfg, int *internal_buffering_lines_needed)
{
    uint32_t reg_val;
    uint32_t i;
    error_type_t ret_val = OK;

    DBG_PRINTF_DEBUG( "%s ENTRY\n", __func__);

    if ( !cfg->control.bypass_all
	 && ( cfg->trap_control.enable 
	      || cfg->sharpen_control.enable 
	      || cfg->edge_strength_detect_enable 
	      || cfg->coring_control.enable 
	      || cfg->pile_control.enable )
	 && ( *internal_buffering_lines_needed < 3 ) ) { 
	*internal_buffering_lines_needed = 3; // minimum input lines retained for area filter  
    }
    //-----------------------
    // TOP CONFIG
    //----------------------
    // load the top config registers
    reg_val = cre_reg->CNTRL;
    reg_val = REPLACE_VAL( CRE, CNTRL, BYPASSALL, reg_val, cfg->control.bypass_all);
    reg_val = REPLACE_VAL( CRE, CNTRL, HX2, reg_val, cfg->control.hx2);
    reg_val = REPLACE_VAL( CRE, CNTRL, VX2, reg_val, cfg->control.vx2);
    reg_val = REPLACE_VAL( CRE, CNTRL, COE, reg_val, cfg->control.cyan_channel_enable);
    reg_val = REPLACE_VAL( CRE, CNTRL, MOE, reg_val, cfg->control.magenta_channel_enable);
    reg_val = REPLACE_VAL( CRE, CNTRL, YOE, reg_val, cfg->control.yellow_channel_enable);
    reg_val = REPLACE_VAL( CRE, CNTRL, KOE, reg_val, cfg->control.black_channel_enable);
    cre_reg->CNTRL = reg_val;

    DBG_PRINTF_DEBUG("%s set cfg register 0x%x to 0x%x\n", __func__, &cre_reg->CNTRL, cre_reg->CNTRL);

    //-----------------------
    // TRAP CONFIG
    //----------------------
    // Load trap control register
    reg_val = cre_reg->TRAP_CNTRL;
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_ENABLE, reg_val, cfg->trap_control.enable);
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_OTMODE, reg_val, cfg->trap_control.trap_object_type_mode);
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_ENPIXSIDE, reg_val, cfg->trap_control.enable_trapping_under_both_sides_of_an_edge);
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_ENKLINE, reg_val, cfg->trap_control.enable_non_vert_horiz_k_line_detection); 
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_EN1PIXHV, reg_val, cfg->trap_control.enable_one_pixel_wide_k_line_detection);
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_EN2PIXHV, reg_val, cfg->trap_control.enable_two_pixel_wide_k_line_detection);
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_EN2PIX, reg_val, cfg->trap_control.enable_detection_of_edges_two_pixels_away);
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_EN1PIX, reg_val, cfg->trap_control.enable_detection_of_edges_one_pixel_away);
    reg_val = REPLACE_VAL( CRE, TRAP, CNTRL_KONLY, reg_val, cfg->trap_control.enable_black_trapping_only);    
    cre_reg->TRAP_CNTRL = reg_val;

    // load the trap C,M,Y,K threshold registers
    cre_reg->TRAP_T_C = cfg->cyan_trap_threshold;
    cre_reg->TRAP_T_M = cfg->magenta_trap_threshold;
    cre_reg->TRAP_T_Y = cfg->yellow_trap_threshold;
    cre_reg->TRAP_T_K = cfg->black_trap_threshold;

    // TRAP INTENSITY PERCENTAGE LIMIT
    cre_reg->TRAP_IPLIM = cfg->trap_intensity_percent_limit;

    // TRAP INTENSITY THRESHOLD
    cre_reg->TRAP_I_T = cfg->trap_intensity_threshold;

    // TRAP BLACK LINE THRESHOLD
    cre_reg->TRAP_KLINE_T = cfg->trap_k_line_threshold;

    // TRAP ADJACENT BLACK PIXELS PER LINE
    cre_reg->TRAP_KPLINE = cfg->adjacent_k_pixels_per_line;

    // TRAP MAXIMUM
    cre_reg->TRAP_MAX = cfg->max_trapping_limit;

    // *** Enable CPU to access LUTs. ***
    reg_val = cre_reg->CNTRL;
    reg_val = REPLACE_VAL( CRE, CNTRL, CPUACCESS, reg_val, 1);
    cre_reg->CNTRL = reg_val;

    // TRAP Cyan Intensity LUT
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTCI)/sizeof(cre_reg->TRAP_LUTCI[0])); i++)
    {
        cre_reg->TRAP_LUTCI[i] = cfg->trap_cyan_intensity_lut[i];
        reg_val = cre_reg->TRAP_LUTCI[i];
        if ( reg_val != cfg->trap_cyan_intensity_lut[i] )
        {
            DBG_PRINTF_ERR("Cyan Intensity LUT read failed: Entry # %d, Exp=0x%X, Act=0x%X", i, cfg->trap_cyan_intensity_lut[i], reg_val);
        }
    }

    // TRAP Magenta Intensity LUT
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTMI)/sizeof(cre_reg->TRAP_LUTMI[0])); i++)
    {
        cre_reg->TRAP_LUTMI[i] = cfg->trap_magenta_intensity_lut[i];
        reg_val = cre_reg->TRAP_LUTMI[i];
        if (reg_val != cfg->trap_magenta_intensity_lut[i])
        {
            DBG_PRINTF_ERR("Magenta Intensity LUT read failed: Entry # %d, Exp=0x%X, Act=0x%X", i, cfg->trap_magenta_intensity_lut[i], reg_val);
        }
    }

    // TRAP Yellow Intensity LUT
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTYI)/sizeof(cre_reg->TRAP_LUTYI[0])); i++)
    {
        cre_reg->TRAP_LUTYI[i] = cfg->trap_yellow_intensity_lut[i];
        reg_val = cre_reg->TRAP_LUTYI[i];
        if (reg_val != cfg->trap_yellow_intensity_lut[i])
        {
            DBG_PRINTF_ERR("Yellow Intensity LUT read failed: Entry # %d, Exp=0x%X, Act=0x%X", i, cfg->trap_yellow_intensity_lut[i], reg_val);
        }
    }

    // TRAP Black Intensity LUT
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTKI)/sizeof(cre_reg->TRAP_LUTKI[0])); i++)
    {
        cre_reg->TRAP_LUTKI[i] = cfg->trap_black_intensity_lut[i];
        reg_val = cre_reg->TRAP_LUTKI[i];
        if (reg_val != cfg->trap_black_intensity_lut[i])
        {
            DBG_PRINTF_ERR("Black Intensity LUT read failed: Entry # %d, Exp=0x%X, Act=0x%X", i, cfg->trap_black_intensity_lut[i], reg_val);
        }
    }

    //TRAP Cyan Fade LUT
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTCF)/sizeof(cre_reg->TRAP_LUTCF[0])); i++)
    {
        cre_reg->TRAP_LUTCF[i] = cfg->trap_cyan_fade_lut[i];
        reg_val = cre_reg->TRAP_LUTCF[i];
        if (reg_val != cfg->trap_cyan_fade_lut[i])
        {
            DBG_PRINTF_ERR("Cyan Fade LUT read failed: Entry # %d, Exp=0x%X, Act=0x%X", i, cfg->trap_cyan_fade_lut[i], reg_val);
        }
    }

    // TRAP Magenta Fade LUT
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTMF)/sizeof(cre_reg->TRAP_LUTMF[0])); i++)
    {
        cre_reg->TRAP_LUTMF[i] = cfg->trap_magenta_fade_lut[i];
        reg_val = cfg->trap_magenta_fade_lut[i];
        if (reg_val != cfg->trap_magenta_fade_lut[i])
        {
            DBG_PRINTF_ERR("Magenta Fade LUT read failed: Entry # %d, Exp=0x%X, Act=0x%X", i, cfg->trap_magenta_fade_lut[i], reg_val);
        }
    }

    // TRAP Yellow Fade LUT
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTYF)/sizeof(cre_reg->TRAP_LUTYF[0])); i++)
    {
        cre_reg->TRAP_LUTYF[i] = cfg->trap_yellow_fade_lut[i];
        reg_val = cre_reg->TRAP_LUTYF[i];
        if (reg_val != cfg->trap_yellow_fade_lut[i])
        {
            DBG_PRINTF_ERR("Yellow Fade LUT read failed: Entry # %d, Exp=0x%X, Act=0x%X", i, cfg->trap_yellow_fade_lut[i], reg_val);
        }
    }

    // TRAP Black Fade LUT
    for (i=0; i<(sizeof(cre_reg->TRAP_LUTKF)/sizeof(cre_reg->TRAP_LUTKF[0])); i++)
    {
        cre_reg->TRAP_LUTKF[i] = cfg->trap_black_fade_lut[i];
        reg_val = cre_reg->TRAP_LUTKF[i];
        if (reg_val != cfg->trap_black_fade_lut[i])
        {
            DBG_PRINTF_ERR("Black Fade LUT read failed: Entry # %d, Exp=0x%X, Act=0x%X", i, cfg->trap_black_fade_lut[i], reg_val);
        }
    }

    // *** disable CPU access to LUTs. ***
    reg_val = cre_reg->CNTRL;
    reg_val = REPLACE_VAL( CRE, CNTRL, CPUACCESS, reg_val, 0);
    cre_reg->CNTRL = reg_val;

    //-----------------------
    //SHARP CONFIG
    //----------------------
    // Sharp control reg
    reg_val = cre_reg->SHARP_CNTRL;
    reg_val = REPLACE_VAL( CRE, SHARP, CNTRL_ENABLE, reg_val, cfg->sharpen_control.enable);
    reg_val = REPLACE_VAL( CRE, SHARP, CNTRL_OTMODE, reg_val, cfg->sharpen_control.object_type_mode);
    cre_reg->SHARP_CNTRL = reg_val;

    // Sharp C,M,Y,K scale values   
    cre_reg->SHARP_SCALE_C = cfg->cyan_sharpen_scale_factor;
    cre_reg->SHARP_SCALE_M = cfg->magenta_sharpen_scale_factor;
    cre_reg->SHARP_SCALE_Y = cfg->yellow_sharpen_scale_factor;
    cre_reg->SHARP_SCALE_K = cfg->black_sharpen_scale_factor ;

    //Sharp C,M,Y,K offset values   
    cre_reg->SHARP_OFFSET_C = cfg->cyan_sharpen_offset;
    cre_reg->SHARP_OFFSET_M = cfg->magenta_sharpen_offset;
    cre_reg->SHARP_OFFSET_Y = cfg->yellow_sharpen_offset;
    cre_reg->SHARP_OFFSET_K = cfg->black_sharpen_offset;

    //-----------------------
    //EDGE STRENGTH CONFIG
    //----------------------
    // Edge control reg
    reg_val = cre_reg->EDGE_CNTRL;
    reg_val = REPLACE_VAL( CRE, EDGE, CNTRL_ENABLE, reg_val, cfg->edge_strength_detect_enable);
    cre_reg->EDGE_CNTRL = reg_val;

    // Edge C,M,Y,K threshold regs
    cre_reg->EDGE_T_C = cfg->cyan_edge_strength_threshold;
    cre_reg->EDGE_T_M = cfg->magenta_edge_strength_threshold;
    cre_reg->EDGE_T_Y = cfg->yellow_edge_strength_threshold;
    cre_reg->EDGE_T_K = cfg->black_edge_strength_threshold;

    // Edge C,M,Y,K scale regs
    cre_reg->EDGE_SCALE_C = cfg->cyan_edge_strength_scale_factor;
    cre_reg->EDGE_SCALE_M = cfg->magenta_edge_strength_scale_factor;
    cre_reg->EDGE_SCALE_Y = cfg->yellow_edge_strength_scale_factor;
    cre_reg->EDGE_SCALE_K = cfg->black_edge_strength_scale_factor;

    // Edge C,M,Y,K offset regs
    cre_reg->EDGE_OFFSET_C = cfg->cyan_edge_strength_offset;
    cre_reg->EDGE_OFFSET_M = cfg->magenta_edge_strength_offset;
    cre_reg->EDGE_OFFSET_Y = cfg->yellow_edge_strength_offset;
    cre_reg->EDGE_OFFSET_K = cfg->black_edge_strength_offset;

    //-----------------------
    // CORE CONFIG
    //----------------------
    //Core control reg
    reg_val = cre_reg->CORE_CNTRL;
    reg_val = REPLACE_VAL( CRE, CORE, CNTRL_ENABLE, reg_val, cfg->coring_control.enable);
    reg_val = REPLACE_VAL( CRE, CORE, CNTRL_OTMODE, reg_val, cfg->coring_control.object_type_mode);
    reg_val = REPLACE_VAL( CRE, CORE, CNTRL_ENCHKBRD, reg_val, cfg->coring_control.enable_checkerboard);
    reg_val = REPLACE_VAL( CRE, CORE, CNTRL_MODE, reg_val, cfg->coring_control.mode);
    cre_reg->CORE_CNTRL = reg_val;

    // Core C,M,Y,K threshold regs
    cre_reg->CORE_T_C = cfg->cyan_coring_threshold;
    cre_reg->CORE_T_M = cfg->magenta_coring_threshold;
    cre_reg->CORE_T_Y = cfg->yellow_coring_threshold;
    cre_reg->CORE_T_K = cfg->black_coring_threshold;

    // Core Total CMYK threshold reg
    cre_reg->CORE_T_CMYK = cfg->cmyk_max_coring_threshold;

    // Core C,M,Y,K scale regs
    cre_reg->CORE_SCALE_C = cfg->cyan_coring_scale_factor;
    cre_reg->CORE_SCALE_M = cfg->magenta_coring_scale_factor;
    cre_reg->CORE_SCALE_Y = cfg->yellow_coring_scale_factor;
    cre_reg->CORE_SCALE_K = cfg->black_coring_scale_factor;

    //-----------------------
    // PILE CONFIG
    //-----------------------
    // Pile control reg
    reg_val = cre_reg->PILE_CNTRL;
    reg_val = REPLACE_VAL( CRE, PILE, CNTRL_ENABLE, reg_val, cfg->pile_control.enable);
    reg_val = REPLACE_VAL( CRE, PILE, CNTRL_OTMODE, reg_val, cfg->pile_control.object_type_mode);
    reg_val = REPLACE_VAL( CRE, PILE, CNTRL_TEMODE, reg_val, cfg->pile_control.trailing_edge_pile_mode);
    reg_val = REPLACE_VAL( CRE, PILE, CNTRL_LEMODE, reg_val, cfg->pile_control.leading_edge_pile_mode);
    cre_reg->PILE_CNTRL = reg_val;

    // Pile edge threshold
    cre_reg->PILE_ET = cfg->pile_edge_threshold;

    // Pile C,M,Y,K threshold regs
    cre_reg->PILE_T_C = cfg->cyan_pile_threshold;
    cre_reg->PILE_T_M = cfg->magenta_pile_threshold;
    cre_reg->PILE_T_Y = cfg->yellow_pile_threshold;
    cre_reg->PILE_T_K = cfg->black_pile_threshold;

    // Pile C,M,Y,K Leading edge scale regs
    cre_reg->PILE_SCALE_LE_C = cfg->cyan_leading_edge_pile_scale;
    cre_reg->PILE_SCALE_LE_M = cfg->magenta_leading_edge_pile_scale;
    cre_reg->PILE_SCALE_LE_Y = cfg->yellow_leading_edge_pile_scale;
    cre_reg->PILE_SCALE_LE_K = cfg->black_leading_edge_pile_scale;

    // Pile C,M,Y,K Trailing edge scale regs
    cre_reg->PILE_SCALE_TE_C = cfg->cyan_trailing_edge_pile_scale;
    cre_reg->PILE_SCALE_TE_M = cfg->magenta_trailing_edge_pile_scale;
    cre_reg->PILE_SCALE_TE_Y = cfg->yellow_trailing_edge_pile_scale;
    cre_reg->PILE_SCALE_TE_K = cfg->black_trailing_edge_pile_scale;

    return ret_val;
} //CRE_RASTER_cfg







/* -------------------------------------------------------------------------- */
/*                      P R I V A T E  F U N C T I O N S                      */
/* -------------------------------------------------------------------------- */

