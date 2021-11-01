/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

//#include "regAddrs.h" 
#include "memAPI.h"
#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanplat.h"
#include "scanmech.h"
#include "scanmech_null.h"
#include "scancap.h"
#include "scanvars.h"
#include "piehw.h"
#include "pie.h"
#include "siqdata.h"
#include "scanvarsiq.h"
#include "scanmech_pinetree.h" 
#include "piecs.h"
#include "scands.h"
#ifdef SCAN_MECH_LINUX_APP
    #include "scanmech_app.h"
#endif

#define SDH_FAX_TUNING	1

static const struct scan_platform_capabilities g_caps = {
    .has_adf = true,
    .has_flatbed = true,
    .has_duplex = true,
    .has_nsensor = true,

    /* davep 24-May-2013 ; adding num_sensors */
    .num_sensors = 2,

    /* zero terminate these lists */
    .fb_supported_horiz_dpi = { 300, 600, 0 },
    .fb_supported_vert_dpi = { 300, 600, 0 },
    .adf_supported_horiz_dpi = { 300, 600, 0 },
    .adf_supported_vert_dpi =  { 300, 600, 0 },

    /* null terminate this list, too */
    .scan_data_type_list = {SCAN_DATA_TYPE_XRGB, 
                            SCAN_DATA_TYPE_MONO, SCAN_DATA_TYPE_RGBX, 
                            SCAN_DATA_TYPE_RGB, SCAN_DATA_TYPE_PLANE, 
                            SCAN_DATA_TYPE_BILEVEL, SCAN_DATA_TYPE_BILEVEL_ERRORDIFFUSION,
                            SCAN_DATA_NULL_TYPE }
};

static t_ScanMech scan_mech; 

void scanplat_init_mech(void)
{
    memset( &scan_mech, 0, sizeof( t_ScanMech ) );
#ifdef NULL_MECH_DRIVER
    smech_null_init( &scan_mech );
#elif defined (SCAN_MECH_SHILOH)
    smech_pinetree_init( &scan_mech );
#elif defined SCAN_MECH_LINUX_APP
    smech_app_init( &scan_mech );
#else
    #error Unknown/unsupported scan mech
#endif

    /* davep 25-Jun-2013 ; bump way up the number of buffers because this is
     * such a fast platform.
     *
     */
    scands_set_integer( "pipe_pic_pie_buffers", 40 );
}

void scanplat_get_capabilities( struct scan_platform_capabilities *caps )
{
    memcpy( caps, &g_caps, sizeof(struct scan_platform_capabilities));
}

uint32_t scanplat_get_max_scan_length( scan_document_source_t docsrc  )
{
    uint32_t max_scan_length;

    max_scan_length = 0;

    switch( docsrc ) {
        case SCAN_DOCUMENT_SOURCE_FLATBED :
            max_scan_length = 1169; /* ~11" */
            break;

        case SCAN_DOCUMENT_SOURCE_ADF:
        case SCAN_DOCUMENT_SOURCE_ADF_DUPLEX:
        case SCAN_DOCUMENT_SOURCE_ADF_NSENSOR:
            max_scan_length = 1500;
            break;

        default :
            /* say what? */
            XASSERT(0,docsrc);
            break;
    }
    
    return max_scan_length;
}

static const uint8_t mono_scan_mf_ai_lut_y[] = {  
    -16, -16, -15, -15, -15, -14, -14, -13, -13, -13, -12,
    -12, -12, -11, -11, -10, -10, -10,  -9,  -9,  -9,  -8,  -8,  -7,  -7,  -7,
    -6,  -6,  -6,  -5,  -5,  -4,  -4,  -4,  -3,  -3,  -3,  -2,  -2,  -1,  -1,
    -1,   0,   0,   1,   1,   1,   2,   2,   2,   3,   3,   4,   4,   4,   5,
    5,   5,   6,   6,   7,   7,   7,   8, };
    
static const uint8_t color_scan_mf_ai_lut_y[] = {  
    -16, -16, -15, -15, -15, -14, -14, -13, -13, -13, -12,
    -12, -12, -11, -11, -10, -10, -10,  -9,  -9,  -9,  -8,  -8,  -7,  -7,  -7,
    -6,  -6,  -6,  -5,  -5,  -4,  -4,  -4,  -3,  -3,  -3,  -2,  -2,  -1,  -1,
    -1,   0,   0,   1,   1,   1,   2,   2,   2,   3,   3,   4,   4,   4,   5,
    5,   5,   6,   6,   7,   7,   7,   8, };

static const uint8_t mf_ai_lut_cbcr[]  = {  
    -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16,
    -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16,
    -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16,
    -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16, -16,
    -16, -16, -16, -16, -16, -16, -16, -16, };

static const uint32_t mf_var_coeff_y[] = { 
                     0, 0, 0,  0,  0, 
                     0, 0, 3, 17, 29, 
                     0, 0, 8, 29, 44, };
static const uint32_t mf_var_coeff_chroma[] = { 
                     0, 0, 0,  0,  0, 
                     0, 0, 3, 17, 29, 
                     0, 0, 8, 29, 44, };
                     
struct scanvars *scanplat_sv_mono_scan_new( void )
{
	scan_err_t scerr;
    struct scanvars *sv;

    sv = scanvar_mono_scan_new();
//    sv = scanvar_new();
    if( sv==NULL ) {
        dbg1( "no memory for scanvar!\n" );
        return NULL;
    }

//    sv->use_pie = false;
    sv->use_pie = true;
    sv->pieout = PIE_PIXEL_MONO;
//    scanvar_disable_image_enhancements( sv );

/***************** Improve Text Sharpness (SDH LSH) ******************/
    /* turn on MultiFilter */
    sv->use_mf = true;

    /* populate the MuliFilter Activity Index LUT (content sensitive
     * smooth/sharpen) 
     */
    scerr = scanvar_set_mf_ai_lut( sv, mono_scan_mf_ai_lut_y, NULL );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* we want the MultiFilter block's variable coefficients */
    scerr = scanvar_set_mf_coef_lut( sv, mf_var_coeff_y, mf_var_coeff_chroma);
    XASSERT( scerr==SCANERR_NONE, scerr );
    sv->mf_varcoef_y_shift      = 8;
    sv->mf_varcoef_y_sf         = 2048;
    sv->mf_varcoef_chroma_shift = 8;
    sv->mf_varcoef_chroma_sf    = 2048;

    sv->mf_y_radius    = PIE_FILTER_SCR1_Y_1;//PIE_FILTER_SCR1_Y_1, PIE_FILTER_SCR1_Y_1_5, PIE_FILTER_SCR1_Y_2
    sv->mf_cbcr_radius = PIE_FILTER_SCR1_R_1;//PIE_FILTER_SCR1_R_1, PIE_FILTER_SCR1_R_1_5, PIE_FILTER_SCR1_R_2
    sv->mf_scale_10    = PIE_FILTER_SCR1_S_1;//PIE_FILTER_SCR1_S_1, PIE_FILTER_SCR1_S_0_5, PIE_FILTER_SCR1_S_0_25;
    sv->mf_scale_15    = PIE_FILTER_SCR1_M_0_5;//PIE_FILTER_SCR1_M_1, PIE_FILTER_SCR1_M_0_5, PIE_FILTER_SCR1_M_0_25, PIE_FILTER_SCR1_M_0;
    sv->mf_scale_20    = PIE_FILTER_SCR1_P_0_25;//PIE_FILTER_SCR1_P_0_5, PIE_FILTER_SCR1_P_0_25, PIE_FILTER_SCR1_P_0_125, PIE_FILTER_SCR1_P_0;
    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_32;//PIE_FILTER_SCR1_N_16, PIE_FILTER_SCR1_N_32, PIE_FILTER_SCR1_N_64, PIE_FILTER_SCR1_N_128;
    sv->mf_offset      = 0;
    sv->use_mf_varcoefficients = true;
/***************************************************************/    
    /* davep 12-Jul-2012 ; sensor defaults to 600, we don't have rez select
     * working  
     */
//    scanvar_set_dpi( sv, 600 );

    scanvar_set_pic_bpp( sv, 8 );
//    scanvar_set_pic_bpp( sv, 16 );
//    scanvar_set_area( sv, 0, 0, 850, 400 ); //MRVL-Harris-20121030

//    scanvar_set_document_source( sv, SCAN_DOCUMENT_SOURCE_ADF );

//    scanvar_set_sharpness( sv, 200 );

    /* davep 14-Jun-2013 ; bump up the PIE rows per buffer in attempt to avoid
     * PIC/Scan overflows
     */
    sv->pie_min_rows_per_buffer = 64;

    /* davep 18-Jun-2013 ; fixing firmware margins */
    sv->use_fw_margins = true;
    
    return sv;
}

struct scanvars *scanplat_sv_color_scan_new( void )
{
    scan_err_t scerr;
    struct scanvars *sv;

    sv = scanvar_color_scan_new();
    if( sv==NULL ) {
        return NULL;
    }

    sv->use_pie = true;
    sv->pieout = PIE_PIXEL_RGB;

    scanvar_set_pic_bpp( sv, 8 );

    /* davep 01-Apr-2013 ; adding siqfile settings */

    /* turn on MultiFilter */
    sv->use_mf = true;

    /* populate the MuliFilter Activity Index LUT (content sensitive
     * smooth/sharpen) 
     */
    scerr = scanvar_set_mf_ai_lut( sv, color_scan_mf_ai_lut_y, mf_ai_lut_cbcr );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* we want the MultiFilter block's variable coefficients */
    scerr = scanvar_set_mf_coef_lut( sv, mf_var_coeff_y, mf_var_coeff_chroma);
    XASSERT( scerr==SCANERR_NONE, scerr );
    sv->mf_varcoef_y_shift      = 8;
    sv->mf_varcoef_y_sf         = 2048;
    sv->mf_varcoef_chroma_shift = 8;
    sv->mf_varcoef_chroma_sf    = 2048;
/***************** Improve Text Sharpness (SDH LSH) ******************/
    sv->mf_y_radius    = PIE_FILTER_SCR1_Y_1;//PIE_FILTER_SCR1_Y_1, PIE_FILTER_SCR1_Y_1_5, PIE_FILTER_SCR1_Y_2
    sv->mf_cbcr_radius = PIE_FILTER_SCR1_R_1;//PIE_FILTER_SCR1_R_1, PIE_FILTER_SCR1_R_1_5, PIE_FILTER_SCR1_R_2
    sv->mf_scale_10    = PIE_FILTER_SCR1_S_1;//PIE_FILTER_SCR1_S_1, PIE_FILTER_SCR1_S_0_5, PIE_FILTER_SCR1_S_0_25;
    sv->mf_scale_15    = PIE_FILTER_SCR1_M_0_5;//PIE_FILTER_SCR1_M_1, PIE_FILTER_SCR1_M_0_5, PIE_FILTER_SCR1_M_0_25, PIE_FILTER_SCR1_M_0;
    sv->mf_scale_20    = PIE_FILTER_SCR1_P_0_25;//PIE_FILTER_SCR1_P_0_5, PIE_FILTER_SCR1_P_0_25, PIE_FILTER_SCR1_P_0_125, PIE_FILTER_SCR1_P_0;
    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_32;//PIE_FILTER_SCR1_N_16, PIE_FILTER_SCR1_N_32, PIE_FILTER_SCR1_N_64, PIE_FILTER_SCR1_N_128;
    sv->mf_offset      = 0;
    sv->use_mf_varcoefficients = true;
/***************************************************************/    
    sv->use_mf_te = 1;
    sv->te_use_5x5_matrix = 1;
    sv->te_count_black_min = 4;
    sv->te_count_white_min = 4;
    sv->te_count_white_plus_black_min = 10;
    sv->te_center_color_max = 25;
    sv->te_intense_color_max = 35;
    sv->te_black_y_max = 75;
    sv->te_black_CbCr_max = 63;
    sv->te_white_y_min = 100;
    sv->te_white_CbCr_max = 20;

    /* davep 11-Apr-2013 ; turn on colorshift; use known good settings from
     * another product
     */
    sv->use_colorshift = true;
    sv->cis_colorshift_round_mode = PIE_COLORSHIFT_CONFIG_ROUND_MODE_FIXED;
    sv->cis_colorshift_mmode[0] = PIE_COLORSHIFT_MMODE_LINE_BELOW;  /* red */
    sv->cis_colorshift_mmode[1] = PIE_COLORSHIFT_MMODE_LINE_BOTH;   /* green */
    sv->cis_colorshift_mmode[2] = PIE_COLORSHIFT_MMODE_LINE_ABOVE;  /* blue */
    sv->cis_colorshift_mult[0] = 11; /* red */
    sv->cis_colorshift_mult[1] = 11; /* green */
    sv->cis_colorshift_mult[2] = 11; /* blue */

    /* davep 23-Jun-2013 ; bump up the PIE rows per buffer in attempt to avoid
     * PIC/Scan overflows
     */
//    sv->pie_min_rows_per_buffer = 32;

    /* Merge again to pick up any changes from a siqfile */
    scanvar_siq_merge( sv );

    return sv;
}


uint32_t scanplat_calc_sclkper( scan_cmode_t cmode, uint32_t dpi, uint32_t pixeltimes_per_line )
{
	//struct scanvars *temp_sv;
    //MRVL-Harris notes here
    //for stepper
    //200MHz/pwm_m/ls_incr ~ 250MHz/pixer/SCAN_SCYCLE_DEFAULT/sclkper
    //sclkper ~ 250MHz/pixer/SCAN_SCYCLE_DEFAULT*ls_incr*pwm_m/200MHz
    //don't forget 1.1 safe factor in scan_cmdq_setup
    //sclkper ~ 250MHz/pixer/SCAN_SCYCLE_DEFAULT*ls_incr*pwm_m/200MHz/1.1
    
    if (cmode == SCAN_CMODE_MONO) {
        if (dpi == 300) {
            return (39); //=250MHz/60/32*955*66/198MHz/1.1
        } else if (dpi == 600) {
            return (59); //=250MHz/60/32*500*200/200MHz/1.1
        } else if (dpi == 1200) {
            XASSERT(0, dpi); //not support "now"
            return (-1);
        } else {
            XASSERT(0, dpi);
            return (-1);
        }
    } else if (cmode == SCAN_CMODE_COLOR) {
        if (dpi == 300) {
            return  (29); //=250MHz/60/32*125*400/200MHz/1.1
        } else if (dpi == 600) { 
            return (59); //=250MHz/60/32*200*500/200MHz/1.1
        } else if (dpi == 1200) {
            XASSERT(0, dpi); //not support "now"
            return (-1);
        } else {
            XASSERT(0, dpi);
            return (-1);
        }
    } else {
        XASSERT(0, cmode);
        return (-1);
    }
}

static void init_copy_settings( struct scanvars *sv )
{
    /* common settings for all copy scanvars 
     *
     * TODO add more as we tune copy
     */
    scanvar_set_pic_bpp( sv, 8 );
    sv->pie_min_rows_per_buffer = 32;
    sv->pie_max_rows_per_buffer = 64;
}

struct scanvars *scanplat_sv_draft_copy_new( void )
{
    struct scanvars *sv;
    sv = scanvar_draft_copy_new();
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_text_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_text_copy_new(); 
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_mixed_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_mixed_copy_new(); 
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_filmphoto_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_filmphoto_copy_new(); 
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_picture_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_picture_copy_new(); 
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_draft_color_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_draft_color_copy_new(); 
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_text_color_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_text_color_copy_new(); 
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_mixed_color_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_mixed_color_copy_new(); 
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_filmphoto_color_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_filmphoto_color_copy_new(); 
    init_copy_settings( sv );
    return sv;
}

struct scanvars *scanplat_sv_picture_color_copy_new( void )
{ 
    struct scanvars *sv;
    sv = scanvar_picture_color_copy_new(); 
    init_copy_settings( sv );
    return sv;
}


struct scanvars *scanplat_sv_text_fax_new( void )
{ 
    return scanvar_text_fax_new(); 
}

struct scanvars *scanplat_sv_mixed_fax_new( void )
{ 
    return scanvar_mixed_fax_new(); 
}

struct scanvars *scanplat_sv_mono_cal_new( void )
{ 
    return scanvar_mono_cal_new(); 
}

struct scanvars *scanplat_sv_color_cal_new( void )
{ 
    return scanvar_color_cal_new(); 
}

