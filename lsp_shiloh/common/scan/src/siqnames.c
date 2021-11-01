/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Avago Technologies, Inc. All Rights Reserved
 *                      
 *                         Avago Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#include <stdlib.h>

#include "lassert.h"

#include "scantypes.h"
#include "siqdata.h"


/* This stuff doesn't live in siqdata.c so I can build the parser as a
 * standalone host-based executable. siqdata.c has too many dependencies on
 * scanvars.c
 */
 
/** convert human names to internal numbers 
 * \todo TODO switch to hash table because this list is going to get HUGE
 */
const struct siqdata_name g_siq_names[] = {
    { "name",      4, SIQ_NAME },

    /* davep 01-Nov-2012 ; adding "user" settings so can use siqfile as a
     * pickle/unpickle for all scan settings. These settings are ignored in a
     * siqfile and are only used in Linux userspace/kernelspace communication.
     * 
     */
    { "dpi", 3, SIQ_USER_DPI },
    { "scale", 5, SIQ_USER_SCALE },
    { "area", 4, SIQ_USER_AREA },
    { "capture_cal", 11, SIQ_CAPTURE_CAL },
    { "pieout", 6, SIQ_PIEOUT },
    { "bpp", 3, SIQ_BPP },
    { "docsrc", 6, SIQ_DOC_SRC },

    /* calibration tuning */
    { "cal_white_digital", 17, SIQ_CAL_WHITE_DIGITAL },
    { "cal_black_digital", 17, SIQ_CAL_BLACK_DIGITAL },
    { "cal_white_analog", 16, SIQ_CAL_WHITE_ANALOG },
    { "cal_black_analog", 16, SIQ_CAL_BLACK_ANALOG },
    { "cal_analog_offset", 17, SIQ_CAL_ANALOG_OFFSET },
    { "cal_analog_gain", 15, SIQ_CAL_ANALOG_GAIN },
    { "cal_led_pwm", 11, SIQ_CAL_LED_PWM },
    { "cal_led_exp", 11, SIQ_CAL_LED_EXP },

    /* IQ tuning */
    { "white_clip",      10, SIQ_WHITE_CLIP_POINT },
    { "black_clip",      10, SIQ_BLACK_CLIP_POINT },
    { "gamma",            5, SIQ_GAMMA },
    { "contrast",         8, SIQ_CONTRAST },
    { "brightness",      10, SIQ_BRIGHTNESS },
    { "copy_brightness", 15, SIQ_COPY_BRIGHTNESS },
    { "sharpness",        9, SIQ_SHARPNESS },
    { "rgb_tint",         8, SIQ_RGB_TINT },
	// add.lsh.for background removal
	{ "background",       10, SIQ_BACKGROUND },
	
    /* 5x5 filter implemented in firmware (only for emergencies) */
    { "sw_filter",        9, SIQ_SW_FILTER },

    /* firmware margins; support strange sensors PIC can't handle. Also
     * overfloaded to support firmware chipgap correction.
     */
    { "fw_margins",     10, SIQ_FW_MARGINS },

    /* PIC Bit Depth Reduction (BDR) */
    { "pic_bdr_enable", 14, SIQ_PIC_BDR_ENABLE },
    { "pic_bdr_lut", 11, SIQ_PIC_BDR_LUT },

    /* PIE Color Space Converter (CSC) */
    { "csc_enable",  10, SIQ_CSC_ENABLE },
    { "csc_rgb_lut", 11, SIQ_CSC_RGB_LUT },
    { "csc_rgb_matrix", 14, SIQ_CSC_RGB_MATRIX },
    { "csc_rgbycc_matrix", 17, SIQ_CSC_RGBYCC_MATRIX },
    { "csc_yccrgb_matrix", 17, SIQ_CSC_YCCRGB_MATRIX },
    { "csc_rgbsrgb_bypass", 18, SIQ_CSC_RGBSRGB_BYPASS },
    { "csc_rgbycc_bypass", 17, SIQ_CSC_RGBYCC_BYPASS },
    { "csc_yccrgb_bypass", 17, SIQ_CSC_YCCRGB_BYPASS },
    { "csc_r_lut", 9, SIQ_CSC_R_LUT },
    { "csc_g_lut", 9, SIQ_CSC_G_LUT },
    { "csc_b_lut", 9, SIQ_CSC_B_LUT },

    /* PIE Tone Contract and Neutral Sensitivity (TCNS) */
    { "tcns_enable",  11, SIQ_TCNS_ENABLE },
    { "tcns_lut_y",   10, SIQ_TCNS_LUT_Y },
    { "tcns_lut_cb",  11, SIQ_TCNS_LUT_CB },
    { "tcns_lut_cr",  11, SIQ_TCNS_LUT_CR },

    /* PIE color shift, CIS color fringe removal (CS) */
    { "cis_colorshift_enable", 21, SIQ_CIS_COLORSHIFT_ENABLE },
    { "cis_colorshift_round_mode", 25, SIQ_CIS_COLORSHIFT_ROUND_MODE },
    { "cis_colorshift_mmode", 20, SIQ_CIS_COLORSHIFT_MMODE},
    { "cis_colorshift_mult", 19, SIQ_CIS_COLORSHIFT_MULT },

    /* PIE MultiFilter (MF) */
    { "mf_enable", 9, SIQ_MF_ENABLE  },
    { "mf_ai_lut_y", 11, SIQ_MF_AI_LUT_Y  },
    { "mf_ai_lut_cbcr", 14, SIQ_MF_AI_LUT_CBCR  },
    { "mf_y_radius", 11, SIQ_MF_Y_RADIUS },
    { "mf_cbcr_radius", 14, SIQ_MF_CBCR_RADIUS },
    { "mf_scale_10", 11, SIQ_MF_SCALE_10 },
    { "mf_scale_15", 11, SIQ_MF_SCALE_15 },
    { "mf_scale_20", 11, SIQ_MF_SCALE_20 },
    { "mf_output_scale", 15, SIQ_MF_OUTPUT_SCALE },
    { "mf_offset", 9, SIQ_MF_OFFSET },
    
    /* davep 20-Dec-2011 ; adding variable filter coefficients, erosion, dilation */
    { "mf_enable_erosion", 17, SIQ_MF_ENABLE_EROSION },
    { "mf_enable_dilation", 18, SIQ_MF_ENABLE_DILATION },
    { "mf_enable_var_coeff", 19, SIQ_MF_ENABLE_VAR_COEFF },
    { "mf_var_coeff_y", 14, SIQ_MF_VAR_COEFF_Y },
    { "mf_var_coeff_y_sf", 17, SIQ_MF_VAR_COEFF_Y_SF },
    { "mf_var_coeff_y_shift", 20, SIQ_MF_VAR_COEFF_Y_SHIFT },
    { "mf_var_coeff_chroma", 19, SIQ_MF_VAR_COEFF_CHROMA },
    { "mf_var_coeff_chroma_sf", 22, SIQ_MF_VAR_COEFF_CHROMA_SF },
    { "mf_var_coeff_chroma_shift", 25, SIQ_MF_VAR_COEFF_CHROMA_SHIFT },

    { "mf_te_enable", 12, SIQ_MF_TE  },
    { "te_count_black_min", 18, SIQ_TE_COUNT_BLACK_MIN },
    { "te_count_white_min", 18, SIQ_TE_COUNT_WHITE_MIN },
    { "te_count_white_plus_black_min", 29, SIQ_TE_COUNT_WHITE_PLUS_BLACK_MIN },
    { "te_center_color_max", 19, SIQ_TE_CENTER_COLOR_MAX },
    { "te_intense_color_max", 20, SIQ_TE_INTENSE_COLOR_MAX },
    { "te_black_y_max", 14, SIQ_TE_BLACK_Y_MAX },
    { "te_black_CbCr_max", 17, SIQ_TE_BLACK_CbCr_MAX },
    { "te_white_y_min", 14, SIQ_TE_WHITE_Y_MIN },
    { "te_white_CbCr_max", 17, SIQ_TE_WHITE_CbCr_MAX },
    { "te_use_5x5_matrix", 17, SIQ_TE_USE_5X5_MATRIX },

    /* HalfPack, the HalfToner/BitPacker. Deprecated--moved to another
     * hardware block. Only the firmware based ordered dither emulator remains.
     */
    { "hp_bpp",          6,  SIQ_HP_BPP },
    { "hp_horiz_rep",   12,  SIQ_HP_HORIZ_REP },
    { "hp_size_lut",    11,  SIQ_HP_SIZE_LUT },
    { "hp_sbias_lut",   12,  SIQ_HP_SBIAS_LUT },
    { "hp_density_lut", 14,  SIQ_HP_DENSITY_LUT },
    { "hp_diffus_lut",  13,  SIQ_HP_DIFFUS_LUT },
    { "hp_alg_invert_data", 18, SIQ_HP_ALG_INVERT_DATA },
    { "hp_alg_accum_err",16,  SIQ_HP_ALG_ACCERR  },
    { "hp_alg_sbias_idx",16,  SIQ_HP_ALG_SBIAS_I },
    { "hp_alg_sbias_dot",16,  SIQ_HP_ALG_SBIAS_D },
    { "hp_alg_doadot",   13,  SIQ_HP_ALG_DOADOT },
    { "hp_alg_shift",    12,  SIQ_HP_ALG_SHIFT },
    { "hp_alg_errweight1",17,  SIQ_HP_ALG_ERRW1 },
    { "hp_alg_errweight2",17,  SIQ_HP_ALG_ERRW2 },
    { "hp_alg_errweight3",17,  SIQ_HP_ALG_ERRW3 },
    { "hp_bias_sram_cfg", 16,  SIQ_HP_BIAS_SRAM_CFG },
    { "hp_bias_reg0",    12,   SIQ_HP_BIAS_REG0 },
    { "hp_bias_reg1",    12,   SIQ_HP_BIAS_REG1 },

    /* must have an empty entry to indicate end of list */
    { 0, 0, 0 },
};

/**
 * \brief return human name for internal siqfile key 
 *
 * Used to convert a siqfile data key into a nice readable string. 
 *
 * \author David Poole
 * \date 29-Dec-2005
 *
 */

const char *
siqdata_find_name_for_key( int key )
{
    int i;

    /* linear search, blah blah blah */
    for( i=0 ; ; i++ ) {
        if( g_siq_names[i].key == key ) {
            return g_siq_names[i].str;
        }
    }
    return "(unknown key!)";
}

/**
 * \brief get first name in list of siq data names; initialize the iterator
 *
 * See also siqdata_get_next_name() 
 *
 *
 * \author David Poole
 * \date 25-Jun-2006
 *
 */

const struct siqdata_name * 
siqdata_get_first_name( void )
{
    return &g_siq_names[0];
}

/**
 * \brief  A rough iterator on g_siq_names[].
 *
 * See also siqdata_get_first_name()
 *
 *
 * \author David Poole
 * \date 25-Jun-2006
 *
 */

const struct siqdata_name * 
siqdata_get_next_name( const struct siqdata_name *name )
{
    /* stupid human checks */
    if( name==NULL ) {
        return NULL;
    }

    XASSERT( name >= g_siq_names, (uint32_t)name );    
    XASSERT( name < g_siq_names+sizeof(g_siq_names), (uint32_t)name );

    /* somehow already on last element!? */
    if( name->len == 0 ) {
        return NULL;
    }

    /* move to next element in our array */
    name++;

    /* if on end-of-list marker, return NULL */
    if( name->len==0 ) {
        return NULL;
    }
            
    return name;
}

