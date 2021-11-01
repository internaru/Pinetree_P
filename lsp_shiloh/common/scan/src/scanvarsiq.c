/******************************************************************************
 * Copyright (c) 2006-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * ============================================================================
 * Copyright (c) 2004, 2005 Avago Technologies, Inc. All Rights Reserved
 *                      
 *                         Avago Confidential
 * ============================================================================
 **/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "scos.h"

#include "lassert.h"

/* need the siqfile_print_fn capabilities in scanvars.h */
#define USE_SIQFILE_PRINT_FN 1

/* need vscanlog2() */
#define USE_VSCANLOG 1

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "piehw.h"
#include "piehphw.h"
#include "scanvars.h"
#include "siqdata.h"
#include "siqparse.h"
#include "pichw.h"

static siqfile_print_fn siqfile_print = NULL;

/**
 * \brief ; a whacky mechanism to capture siqfile to a string through a global
 * registered siqfile_print function pointer.
 *
 * Originally created to "pickle" a scanvar to send to linux kernel kscantask.
 *
 * \author David Poole
 * \date 31-Oct-2012
 */

siqfile_print_fn set_siqfile_print_fn( siqfile_print_fn new_siqfile_print_fn )
{
    siqfile_print_fn old_fn;

    /* note: no semaphore protection! */

    old_fn = siqfile_print;
    siqfile_print = new_siqfile_print_fn;
    return old_fn;
}

/**
 * \brief merge a siqfile array into scanvars
 *
 * If the scanvar array doesn't exist, it will be created. Data is copied, not
 * pointed to. Scanvars unconditionally releases its data plus we might want
 * someone further down the code path to have the freedom to modify the
 * scanvars data without worrying about damaging later scans. (siqfile data
 * persists across scanvar instantiations.)
 *
 * \author David Poole
 * \date 29-Dec-2005
 *
 */

static scan_err_t
siq_merge_uint32_array( const struct siqdata *sd, 
                        struct uint32_array *array,
                        int expected_num_values )
{
    scan_err_t scerr;

    /* 'array' may or may not actually have any data already attached; take the
     * opportunity to do some sanity checking
     */
    if( array->num_entries ) {
        ASSERT( array->data!=NULL );
    }
    else {
        XASSERT( array->data==NULL, (uint32_t)array->data );
    }

    /* Make sure the siqfile had exactly the number of values we expected. We
     * can't test this when we're parsing. We can only test it right when we
     * want to use it.
     */
    if( sd->num_values != expected_num_values ) {
        dbg1( "ignoring bad %s; want %d entries, found %d entries\n", 
                    siqdata_find_name_for_key(sd->key), 
                    expected_num_values, 
                    sd->num_values );
        return SCANERR_INVALID_PARAM;
    }

    scerr = uint32_array_copy( array, 
                                 sd->num32_values,
                                 sd->num_values );
    return scerr;
}

/**
 * \brief merge a siqfile boolean into scanvars
 *
 * \author David Poole
 * \date 30-Dec-2005
 *
 */

static void
siq_merge_bool( const struct siqdata *sd, bool *flag )
{
    if( sd->num32 ) {
        *flag = true;
    }
    else { 
        *flag = false;
    }
}

/**
 * \brief range check then accept a siqfile value into an 8-bit field 
 *
 * Some register fields take 2-8 bits. This function range checks the source
 * uint32_t by the number of allowable bits. If src is value, copy to the dest.
 *
 *
 * \author David Poole
 * \date 02-Jan-2006
 *
 */

static scan_err_t siq_merge_uint8( const struct siqdata *sd, uint8_t *num, uint8_t num_valid_bits )
{
    uint32_t mask;

    /* make a temporary so we don't ever have to worry about sign extension;
     * make a mask to check for any values outside the required range
     * 
     * for example, 4-bit value: 0xffffffff >> (32-4) == 0x0000000f
     */
    mask = ~( 0xffffffff >> (32-num_valid_bits) );

//    dbg1( "%s mask=%#lx num32=%#lx value=%#lx\n", __FUNCTION__, mask,
//                sd->num32, sd->num32 & mask);

    /* range checking */
    if( sd->num32 & mask ) {
        dbg1( "ignoring bad %s; found value of %ld for %d-bit value\n", 
                    siqdata_find_name_for_key(sd->key), 
                    sd->num32, num_valid_bits );
        return SCANERR_INVALID_PARAM;
    }

    *num = sd->num32;
    return 0;
}

/**
 * \brief  merge in data stored as 8-bit values in a larger field
 *
 * Originally written for the MultiFilter AI LUT which is the two 8-bit Y and
 * CbCr fields together as a 16-bit value.
 *
 *
 * \author David Poole
 * \date 03-Nov-2006
 *
 */

static scan_err_t 
siq_merge_8bit_fields( const struct siqdata *sd, 
                       struct uint32_array *lut, 
                       int lut_size,
                       uint32_t shift )
{
    int i;
    uint32_t num32;
    scan_err_t scerr;

    /* the scanvars' lut may or may not actually have any data already
     * attached; take the opportunity to do some sanity checking
     */
    if( lut->num_entries ) {
        ASSERT( lut->data!=NULL );
    }
    else {
        XASSERT( lut->data==NULL, (uint32_t)lut->data );
    }

    /* Make sure the siqfile had exactly the number of values we expected. We
     * can't test this when we're parsing. We can only test it right when we
     * want to use it.
     */
    if( sd->num_values != lut_size ) {
        dbg1( "ignoring bad %s; want %d entries, found %d entries\n", 
                    siqdata_find_name_for_key(sd->key), 
                    lut_size, 
                    sd->num_values );
        return SCANERR_INVALID_PARAM;
    }

    /* if we don't have data to begin with, allocate it */
    if( lut->data == NULL ) {
        scerr = uint32_array_new( lut, lut_size );
        if( scerr!=0 ) {
            return scerr;
        }
    }

    /* be very careful about overwriting memory */
    XASSERT( lut->num_entries==lut_size, lut->num_entries );

    /* put the new values together into the exist lut */
    for( i=0 ; i<lut->num_entries ; i++ ) {
        num32 = lut->data[i];

        /* clear the bits of the value we want to override */
        num32 &= ~(0xff<<shift);

        /* set the bits of the new value */
        num32 |= (sd->num32_values[i] & (uint32_t)0x00ff ) << shift;

        /* replace the lut entry with the new value */
        lut->data[i] = num32;
    }

    return 0;
}

/**
 * \brief merge a siqfile TCNS table into scanvars
 *
 * The TCNS LUT is a pain in the neck. It's a 32-bit LUT made up of three 8-bit
 * values but those 8-bit values are broken across byte boundries.
 *
 * xxyyyyyy yyxxxrrr rrrrrxxx bbbbbbbb
 *
 * y=Y r=Cr b=Cb
 *
 * We decided to make the tcns siqfile lut into three luts so we can at least
 * have a prayer of reading the siqfile.
 *
 * This function merges a siqfile TCNS lut into scanvar by clearing the
 * existing 8-bit entry at the uint32_t lut entry's shift value then setting the
 * new value.
 *
 * If the scanvar array doesn't exist, it will be allocated.
 *
 * \author David Poole
 * \date 30-Dec-2005
 *
 */

static scan_err_t
siq_merge_tcns( const struct siqdata *sd, 
                struct uint32_array *tcns_lut, 
                uint32_t shift )
{
    int i;
    uint32_t num32;
    scan_err_t scerr;

    /* the scanvars' tcns lut may or may not actually have any data already
     * attached; take the opportunity to do some sanity checking
     */
    if( tcns_lut->num_entries ) {
        ASSERT( tcns_lut->data!=NULL );
    }
    else {
        XASSERT( tcns_lut->data==NULL, (uint32_t)tcns_lut->data );
    }

    /* Make sure the siqfile had exactly the number of values we expected. We
     * can't test this when we're parsing. We can only test it right when we
     * want to use it.
     */
    if( sd->num_values != PIE_TCNS_LUT_SIZE ) {
        dbg1( "ignoring bad %s; want %d entries, found %d entries\n", 
                    siqdata_find_name_for_key(sd->key), 
                    PIE_TCNS_LUT_SIZE, 
                    sd->num_values );
        return SCANERR_INVALID_PARAM;
    }

    /* if we don't have data to begin with, allocate it */
    if( tcns_lut->data == NULL ) {
        scerr = uint32_array_new( tcns_lut, PIE_TCNS_LUT_SIZE  );
        if( scerr!=0 ) {
            return scerr;
        }
    }

    /* be very careful about overwriting memory */
    XASSERT( tcns_lut->num_entries==PIE_TCNS_LUT_SIZE, tcns_lut->num_entries );

    for( i=0 ; i<PIE_TCNS_LUT_SIZE ; i++ ) {
        num32 = tcns_lut->data[i];

        /* clear the bits of the value we want to override */
        num32 &= ~(0xff<<shift);

        /* set the bits of the new value */
        num32 |= (sd->num32_values[i] & (uint32_t)0x00ff ) << shift;

        /* replace the tcns lut entry with the new value */
        tcns_lut->data[i] = num32;
    }

    return 0;
}

/**
 * \brief copy siqfile RGB/Mono color data into scanvar with validation
 *
 * Cal targets and white/black points are arrays of 16-bit integers. Need to
 * copy from siqfile to scanvar with validation.
 *
 * If copying mono, set unused array entires to zero.
 *
 * As of this writing, the targets and white/black points are statically
 * declared in scanvar so no memory allocation necessary.
 * 
 * \author David Poole
 * \date 02-Jan-2006
 *
 * davep 01-Feb-2007 ;  now also using this function to handle the analog
 * gain/offset and led pwm/exposure fields
 *
 */

static scan_err_t
merge_rgbmono_array( const struct siqdata *sd, uint16_t target[] )
{
    /* sanity check the source data */

    if( sd->num_values==1 ) {
        target[0] = sd->num32_values[0];
        target[1] = 0;
        target[2] = 0;
    }
    else if( sd->num_values==3 ) {
        target[0] = sd->num32_values[0];
        target[1] = sd->num32_values[1];
        target[2] = sd->num32_values[2];
    }
    else {
        /* invalid data; we need either 1 (mono) or 3 (color) entries in the
         * target array
         */
        dbg1( "ignoring bad %s; want 1 or 3 entries, found %d entries\n", 
                    siqdata_find_name_for_key(sd->key), 
                    sd->num_values );
        return SCANERR_INVALID_PARAM;
    }
    return 0;        
}

static scan_err_t
merge_sint32_tuple( const struct siqdata *sd, int target[] )
{
    /* sanity check the source data */

    if( sd->num_values==1 ) {
        target[0] = sd->num32_values[0];
        target[1] = 0;
        target[2] = 0;
    }
    else if( sd->num_values==3 ) {
        target[0] = sd->num32_values[0];
        target[1] = sd->num32_values[1];
        target[2] = sd->num32_values[2];
    }
    else {
        /* invalid data; we need either 1 (mono) or 3 (color) entries in the
         * target array
         */
        dbg1( "ignoring bad %s; want 1 or 3 entries, found %d entries\n", 
                    siqdata_find_name_for_key(sd->key), 
                    sd->num_values );
        return SCANERR_INVALID_PARAM;
    }
    return 0;        
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 01-Nov-2012
 */

scan_err_t scanvar_siq_merge_user_setting( const struct siqdata *sd, struct scanvars *sv )
{
    scan_err_t scerr;

    switch( sd->key ) {
        case SIQ_USER_DPI :
            sv->dpi = sd->num32;
            break;

        case SIQ_USER_SCALE :
            /* if present, should be four numbers */
            if( sd->num_values != 4 ) {
                dbg1( "%s ignoring bad user scale; want 4 entries, found %d entries\n", 
                        __FUNCTION__, sd->num_values );
                return SCANERR_INVALID_PARAM;
            }
            scerr = scanvar_set_xyscale( sv, 
                                        sd->num32_values[0], /* X numerator */
                                        sd->num32_values[1], /* X denominator */
                                        sd->num32_values[2], /* Y numerator */
                                        sd->num32_values[3]  /* Y denominator */ 
            );
            if( scerr != SCANERR_NONE ) {
                return SCANERR_INVALID_PARAM;
            }
            break;

        case SIQ_USER_AREA :
            /* if present, should be four numbers */
            if( sd->num_values != 4 ) {
                dbg1( "%s ignoring bad user area ; want 4 entries, found %d entries\n", 
                        __FUNCTION__, sd->num_values );
                return SCANERR_INVALID_PARAM;
            }
            sv->user_area_hinch.x = sd->num32_values[0];
            sv->user_area_hinch.y = sd->num32_values[1];
            sv->user_area_hinch.width = sd->num32_values[2];
            sv->user_area_hinch.height = sd->num32_values[3];
            break;

        case SIQ_CAPTURE_CAL :
            /* boolean 1,0 */
            switch( sd->num32 ) {
                case 0 : 
                    sv->capture_cal = false;
                    break;
                case 1 :
                    sv->capture_cal = true;
                    break;
                default:
                    dbg1( "%s ignoring bad integer for capture_cal (want 0,1)\n", 
                                __FUNCTION__, sd->num32 );
                    break;
            }
            break;

        case SIQ_PIEOUT : 
            if( sd->num32 < PIE_PIXEL_XRGB || sd->num32 > PIE_PIXEL_RGB ) {
                dbg1( "%s ignoring bad integer for pieout\n", 
                            __FUNCTION__, sd->num32 );
            }
            else {
                dbg2( "%s overriding pieout=%d\n", __FUNCTION__, sd->num32 );
                scanvar_set_pieout( sv, (pie_pixel_t)sd->num32 );
            }
            break;

        case SIQ_BPP: 
            scerr = scanvar_set_bpp( sv, sd->num32 );
            if( scerr != SCANERR_NONE ) {
                dbg2( "%s ignoring invalid bpp=%d\n", __FUNCTION__, sd->num32 );
            }
            else {
                dbg2( "%s overriding bpp=%d\n", __FUNCTION__, sd->num32 );
            }
            break;

        case SIQ_DOC_SRC :
            scerr = scanvar_set_document_source( sv, (scan_document_source_t)sd->num32 );
            if( scerr != SCANERR_NONE ) {
                dbg2( "%s ignoring invalid docsrc=%d\n", __FUNCTION__, sd->num32 );
            }
            else {
                dbg2( "%s overriding docsrc=%d\n", __FUNCTION__, sd->num32 );
            }
            break;

        default :
            /* ignore unknown/unwanted data */
            break;
    }

    return SCANERR_NONE;
}

scan_err_t scanvar_siq_merge_all_user_settings( const struct siqdata_table *siqdata, struct scanvars *sv )
{
    scan_err_t scerr, final_scerr;
    int i;
    const struct siqdata *sd;

    final_scerr = SCANERR_NONE;

    /* user sent down some data for this particular scan; go through each of
     * the siqfile variables and override the scanvar data with it
     */
    for( i=0 ; i<siqdata->num_entries ; i++ ) {
        sd = (const struct siqdata *)&siqdata->data[i];

        scerr = scanvar_siq_merge_user_setting( sd, sv );
        if( scerr != SCANERR_NONE ) {
            /* scanvar_siq_merge_user_setting will log error */
            final_scerr = scerr;
            break;
        }
    }

    return final_scerr;
}

/**
 * \brief combine a single siqfile data element into scanvar
 *
 * \author David Poole
 * \date 01-Jan-2006
 *
 */

void scanvar_siq_merge_data( const struct siqdata *sd, struct scanvars *sv )
{
    scan_err_t scerr;

    switch( sd->key ) {

        /*
         * calibration targets
         */
        case SIQ_CAL_WHITE_DIGITAL :
            scerr = merge_rgbmono_array( sd, sv->cal_white_digital_target );
            if( scerr==0 ) {
                dbg2( "overriding cal white digital\n" );
                sv->use_cal_white_digital_target = true;
            }
            break;

        case SIQ_CAL_BLACK_DIGITAL:
            scerr = merge_rgbmono_array( sd, sv->cal_black_digital_target );
            if( scerr==0 ) {
                dbg2( "overriding cal black digital\n" );
                sv->use_cal_black_digital_target = true;
            }
            break;

        case SIQ_CAL_WHITE_ANALOG :
            scerr = merge_rgbmono_array( sd, sv->cal_white_analog_target );
            if( scerr==0 ) {
                dbg2( "overriding cal white analog\n" );
                sv->use_cal_white_analog_target = true;
            }
            break;

        case SIQ_CAL_BLACK_ANALOG :
            scerr = merge_rgbmono_array( sd, sv->cal_black_analog_target );
            if( scerr==0 ) {
                dbg2( "overriding cal black analog\n" );
                sv->use_cal_black_analog_target = true;
            }
            break;

        case SIQ_CAL_ANALOG_OFFSET :
            scerr = merge_sint32_tuple( sd, sv->cal_analog_offset );
            if( scerr==0 ) {
                dbg2( "overriding cal analog offset\n" );
                sv->use_cal_analog_offset = true;
            }
            break;

        case SIQ_CAL_ANALOG_GAIN :
            scerr = merge_rgbmono_array( sd, sv->cal_analog_gain );
            if( scerr==0 ) {
                dbg2( "overriding cal analog gain\n" );
                sv->use_cal_analog_gain = true;
            }
            break;

        case SIQ_CAL_LED_PWM :
            scerr = merge_rgbmono_array( sd, sv->cal_led_pwm );
            if( scerr==0 ) {
                dbg2( "overriding cal led pwm\n" );
                sv->use_cal_led_pwm = true;
            }
            break;

        case SIQ_CAL_LED_EXP :
            scerr = merge_rgbmono_array( sd, sv->cal_led_exp );
            if( scerr==0 ) {
                dbg2( "overriding cal led exp\n" );
                sv->use_cal_led_exp = true;
            }
            break;

        case SIQ_CAL_USE_GRAY_ALG :
            sv->use_gray_cal = sd->num32;
            dbg2( "overriding gray calibration\n" );
            break;

        /* 
         * IQ/CQ Tuning
         */
        case SIQ_WHITE_CLIP_POINT  :
            scerr = merge_rgbmono_array( sd, sv->white_clip_point );
            if( scerr==0 ) {
                dbg2( "overriding white point\n" );
                sv->use_white_clip_point = true;
            }
            break;

        case SIQ_BLACK_CLIP_POINT  :
            scerr = merge_rgbmono_array( sd, sv->black_clip_point );
            if( scerr==0 ) {
                dbg2( "overriding black point\n" );
                sv->use_black_clip_point = true;
            }
            break;
        
        case SIQ_GAMMA :
            /* pretty much any number is valid here. Feel free to shoot
             * yourself in the foot.
             */
            sv->gamma_times_10 = sd->num32;
            dbg2( "overriding gamma\n" );
            sv->use_gamma = true;
            break;

        case SIQ_CONTRAST :
            sv->contrast = (uint8_t)sd->num32;
            dbg2( "overriding contrast\n" );
            break;

        case SIQ_BRIGHTNESS :   
            sv->brightness = (uint8_t)sd->num32;
            dbg2( "overriding brightness\n" );
            break;

        case SIQ_COPY_BRIGHTNESS :
            if( (int)(sd->num32) < 1 || (int)(sd->num32) > 11 )
            {
                dbg1( "ignoring invalid copy brightness value %#lx (%d)\n",
                            sd->num32, (int)sd->num32 );
            }
            else
            {
                sv->copy_brightness = sd->num32;
                dbg2( "overriding COPY brightness\n" );
            }
            break;

        case SIQ_SHARPNESS :
            sv->sharpness = (uint8_t)sd->num32;
            dbg2( "overriding sharpness\n" );
            break;

        case SIQ_RGB_TINT : 
            scerr = merge_sint32_tuple( sd, sv->rgb_tint );
            if( scerr != 0 ) {
                dbg2( "ignoring invalid rgb_tint\n" );
                sv->rgb_tint[0] = sv->rgb_tint[1] = sv->rgb_tint[2] = 0;
            }
            else {
                dbg2( "overriding rgb_tint\n" );
            }
            break;
		
		case SIQ_BACKGROUND :
            sv->bgr_sensitivity = (uint8_t)sd->num32;
            dbg1( "[BG] BGR scanvar_siq_merge_data!!(%d)\n", sv->bgr_sensitivity);
            break;
            
        case SIQ_SW_FILTER :
            /* possible values are 0, 1, 2
             * 0 - disabled
             * 1 - filter after PIC (not yet implemented)
             * 2 - filter after PIE
             */
            if( sd->num32 > 2 ) {
                dbg1( "ignoring invalid filter value\n" );
            }
            else {
                sv->sw_filter_position = sd->num32;
                dbg2( "overriding software filter\n" );
            }
            break;

        case SIQ_FW_MARGINS : 
            siq_merge_bool( sd, &sv->use_fw_margins );
            dbg2( "overriding FW margins bool\n" );
            break;

        /*
         * PIC BDR (Bit Depth Reduction) 
         */  
        case SIQ_PIC_BDR_ENABLE :
            siq_merge_bool( sd, &sv->use_pic_bdr );
            dbg2( "overriding PIC BDR bool\n" );
            break;

        case SIQ_PIC_BDR_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->pic_bdr_lut,
                            PIC_BDR_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding PIC BDR LUT\n" );
                sv->use_pic_bdr_lut = true;
            }
            break;

        /* 
         * CSC (Color Space Conversion)
         */
        case SIQ_CSC_ENABLE :
            siq_merge_bool( sd, &sv->use_csc );
            dbg2( "overriding CSC bool\n" );
            break;

        case SIQ_CSC_RGB_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->csc_rgb_lut,
                            PIE_CSC_RGB_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding csc rgb lut\n" );
                sv->use_csc_rgb_lut = true;
            }
            break;

        case SIQ_CSC_RGB_MATRIX :
            memcpy(sv->csc_rgb_matrix,sd->num32_values,sd->num_values*4);
            break;

        case SIQ_CSC_RGBYCC_MATRIX :
            memcpy(sv->csc_rgbycc_matrix, sd->num32_values, sd->num_values*4);
            break;

        case SIQ_CSC_YCCRGB_MATRIX :
            memcpy(sv->csc_yccrgb_matrix, sd->num32_values, sd->num_values*4);
            break;

        case SIQ_CSC_RGBSRGB_BYPASS :
            siq_merge_bool( sd, &sv->csc_rgb_srgb_bypass );
            dbg2( "overriding CSC RGB-sRGB bypass\n" );
            break;
            
        case SIQ_CSC_RGBYCC_BYPASS :
            siq_merge_bool( sd, &sv->csc_srgb_ycc_bypass );
            dbg2( "overriding CSC sRGB-YCC bypass\n" );
            break;
            
        case SIQ_CSC_YCCRGB_BYPASS :
            siq_merge_bool( sd, &sv->csc_ycc_rgb_bypass );
            dbg2( "overriding CSC YCC-RGB bypass\n" );
            break;

        case SIQ_CSC_R_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->csc_r_lut,
                            PIE_CSC_RGB_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding csc red lut\n" );
                sv->use_csc_r_lut = true;
            }
            break;

        case SIQ_CSC_G_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->csc_g_lut,
                            PIE_CSC_RGB_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding csc green lut\n" );
                sv->use_csc_g_lut = true;
            }
            break;
            
        case SIQ_CSC_B_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->csc_b_lut,
                            PIE_CSC_RGB_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding csc blue lut\n" );
                sv->use_csc_b_lut = true;
            }
            break;

        /*
         * TCNS (Tone Contrast and Neutral Sensitivity)
         */
        case SIQ_TCNS_ENABLE :
            siq_merge_bool( sd, &sv->use_tcns );
            dbg2( "overriding TCNS bool\n" );
            break;

        case SIQ_TCNS_LUT_Y :
            scerr = siq_merge_tcns( sd, &sv->tcns_lut,
                                        PIE_TCNS_LUT_Y_SHIFT );
            if( scerr==0 ) {
                dbg2( "overriding TCNS Y LUT\n" );
                sv->use_tcns_lut = true;
            }
            break;

        case SIQ_TCNS_LUT_CB :
            scerr = siq_merge_tcns( sd, &sv->tcns_lut,
                                        PIE_TCNS_LUT_CB_SHIFT );
            if( scerr==0 ) {
                dbg2( "overriding TCNS CB LUT\n" );
                sv->use_tcns_lut = true;
            }
            break;

        case SIQ_TCNS_LUT_CR :
            scerr = siq_merge_tcns( sd, &sv->tcns_lut,
                                        PIE_TCNS_LUT_CR_SHIFT );
            if( scerr==0 ) {
                dbg2( "overriding TCNS CR LUT\n" );
                sv->use_tcns_lut = true;
            }
            break;

        /* davep 05-Nov-2012 ; adding PIE ColorShift */
        case SIQ_CIS_COLORSHIFT_ENABLE :
            siq_merge_bool( sd, &sv->use_colorshift );
            dbg2( "overriding cis colorshift bool\n" );
            break;

        case SIQ_CIS_COLORSHIFT_ROUND_MODE :
            sv->cis_colorshift_round_mode = sd->num32;
            dbg2( "overriding cis_colorshift_round_mode\n" );
            break;

        case SIQ_CIS_COLORSHIFT_MMODE :
            if( sd->num_values != 3 ) {
                dbg2( "ignoring invalid CIS Colorshift Multiplier Mode\n" );
            }
            else {
                memcpy( sv->cis_colorshift_mmode, sd->num32_values, 3*sizeof(uint32_t) );
                dbg2( "overriding cis_colorshift_mmode\n" );
            }
            break;

        case SIQ_CIS_COLORSHIFT_MULT :
            if( sd->num_values != 3 ) {
                dbg2( "ignoring invalid CIS Colorshift Multiplier Array\n" );
            }
            else {
                memcpy( sv->cis_colorshift_mult, sd->num32_values, 3*sizeof(uint32_t) );
                dbg2( "overriding cis_colorshift_mult\n" );
            }
            break;

        /*
         * MultiFilter
         */
        case SIQ_MF_ENABLE :
            siq_merge_bool( sd, &sv->use_mf );
            dbg2( "overriding MF bool\n" );
            break;

        case SIQ_MF_AI_LUT_Y :
            scerr = siq_merge_8bit_fields( sd, &sv->mf_ai_lut,
                            PIE_FILTER_LUT_SIZE, 8 );
            if( scerr == 0 ) {
                dbg2( "overriding MF AI LUT Y\n" );
                sv->use_mf_ai_lut = true;
            }
            break;

        case SIQ_MF_AI_LUT_CBCR :
            scerr = siq_merge_8bit_fields( sd, &sv->mf_ai_lut,
                            PIE_FILTER_LUT_SIZE, 0 );
            if( scerr == 0 ) {
                dbg2( "overriding MF AI LUT CbCr\n" );
                sv->use_mf_ai_lut = true;
            }
            break;

        case SIQ_MF_Y_RADIUS :
            /* valid values are 10, 15, 20 for radii of 1.0, 1.5 and 2.0;
             * convert into register setting
             */
            scerr = 0;
            switch( sd->num32 ) {
                case 10 :
                    sv->mf_y_radius = PIE_FILTER_SCR1_Y_1;
                    break;
                case 15 :
                    sv->mf_y_radius = PIE_FILTER_SCR1_Y_1_5;
                    break;
                case 20 :
                    sv->mf_y_radius = PIE_FILTER_SCR1_Y_2;
                    break;
                default :
                    dbg1( "ignoring invalid MF Y radius value %ld\n", sd->num32 );
                    scerr = SCANERR_INVALID_PARAM;
                    break;
            }
            if( scerr==0 ) {
                dbg2( "overriding MF Y radius\n" );
            }
            break;

        case SIQ_MF_CBCR_RADIUS :
            /* valid values are 10, 15, 20 for radii of 1.0, 1.5 and 2.0;
             * convert into register setting
             */
            scerr = 0;
            switch( sd->num32 ) {
                case 10 :
                    sv->mf_cbcr_radius = PIE_FILTER_SCR1_R_1;
                    break;
                case 15 :
                    sv->mf_cbcr_radius = PIE_FILTER_SCR1_R_1_5;
                    break;
                case 20 :
                    sv->mf_cbcr_radius = PIE_FILTER_SCR1_R_2;
                    break;
                default :
                    dbg1( "ignoring invalid MF CbCr radius value %ld\n", sd->num32 );
                    scerr = SCANERR_INVALID_PARAM;
                    break;
            }
            if( scerr==0 ) {
                dbg2( "overriding MF CbCr radius\n" );
            }
            break;

        case SIQ_MF_SCALE_10 :
            /* Valid values are: 1000, 500, 250, 125 to represent the
             * fractional multipliers used by MultiFilter (multiply by 1000)
             */
            scerr = 0;
            switch( sd->num32 ) {
                case 1000 :
                    sv->mf_scale_10 = PIE_FILTER_SCR1_S_1;
                    break;
                case 500 :
                    sv->mf_scale_10 = PIE_FILTER_SCR1_S_0_5;
                    break;
                case 250 :
                    sv->mf_scale_10 = PIE_FILTER_SCR1_S_0_25;
                    break;
                case 125 :
                    sv->mf_scale_10 = PIE_FILTER_SCR1_S_0_125;
                    break;
                default :
                    dbg1( "ignoring invalid MF Scale 1.0 value %ld\n", sd->num32 );
                    scerr = SCANERR_INVALID_PARAM;
                    break;
            }
            if( scerr==0 ) {
                dbg2( "overriding MF scale 1.0\n" );
            }
            break;
            
        case SIQ_MF_SCALE_15 :
            /* Valid values are: 1000, 500, 250, 0 to represent the
             * fractional multipliers used by MultiFilter (multiply by 1000)
             */
            scerr = 0;
            switch( sd->num32 ) {
                case 1000 :
                    sv->mf_scale_15 = PIE_FILTER_SCR1_M_1;
                    break;
                case 500 :
                    sv->mf_scale_15 = PIE_FILTER_SCR1_M_0_5;
                    break;
                case 250 :
                    sv->mf_scale_15 = PIE_FILTER_SCR1_M_0_25;
                    break;
                case 0 :
                    sv->mf_scale_15 = PIE_FILTER_SCR1_M_0;
                    break;
                default :
                    dbg1( "ignoring invalid MF Scale 1.5 value %ld\n", sd->num32 );
                    scerr = SCANERR_INVALID_PARAM;
                    break;
            }
            if( scerr==0 ) {
                dbg2( "overriding MF scale 1.5\n" );
            }
            break;

        case SIQ_MF_SCALE_20 :
            /* Valid values are: 500, 250, 125, 0 to represent the
             * fractional multipliers used by MultiFilter (multiply by 1000)
             */
            scerr = 0;
            switch( sd->num32 ) {
                case 500 :
                    sv->mf_scale_20 = PIE_FILTER_SCR1_P_0_5;
                    break;
                case 250 :
                    sv->mf_scale_20 = PIE_FILTER_SCR1_P_0_25;
                    break;
                case 125 :
                    sv->mf_scale_20 = PIE_FILTER_SCR1_P_0_125;
                    break;
                case 0 :
                    sv->mf_scale_20 = PIE_FILTER_SCR1_P_0;
                    break;
                default :
                    dbg1( "ignoring invalid MF Scale 2.0 value %ld\n", sd->num32 );
                    scerr = SCANERR_INVALID_PARAM;
                    break;
            }
            if( scerr==0 ) {
                dbg2( "overriding MF scale 2.0\n" );
            }
            break;

        case SIQ_MF_OUTPUT_SCALE :
            /* valid values are: 16, 32, 64, 128 */
            scerr = 0;
            switch( sd->num32 ) {
                case 16 :
                    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_16;
                    break;
                case 32 :
                    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_32;
                    break;
                case 64 :
                    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_64;
                    break;
                case 128 :
                    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_128;
                    break;
                default :
                    dbg1( "ignoring invalid MF LUT output scale %ld\n", sd->num32 );
                    scerr = SCANERR_INVALID_PARAM;
                    break;
            }
            if( scerr==0 ) {
                dbg2( "overriding MF output scale\n" );
            }
            break;

        case SIQ_MF_OFFSET :
            /* value values are [0..511] */
            if( sd->num32 < 0 || sd->num32 > 511 ) {
                dbg1( "ignoring invalid MF offset\n" );
            }
            else {
                sv->mf_offset = sd->num32;
                dbg2( "overriding MF offset\n" );
            }
            break;
            
        case SIQ_MF_ENABLE_EROSION : 
            siq_merge_bool( sd, &sv->use_mf_erosion );
            dbg2("overriding MF erosion\n" );
            break;

        case SIQ_MF_ENABLE_DILATION : 
            siq_merge_bool( sd, &sv->use_mf_dilation );
            dbg2("overriding MF dilation\n" );
            break;

        case SIQ_MF_ENABLE_VAR_COEFF : 
            siq_merge_bool( sd, &sv->use_mf_varcoefficients );
            dbg2("overriding enable MF variable coefficients\n" );
            break;

        case SIQ_MF_VAR_COEFF_Y : 
            if( sd->num_values != MF_COEF_MAX_COEFFICIENTS ) {
                dbg1( "invalid MF Y LUT: num entries should be %d but is %d\n", 
                            MF_COEF_MAX_COEFFICIENTS, sd->num_values );
            }
            else {
                memcpy( sv->mf_varcoef_y, sd->num32_values, sd->num_values*sizeof(uint32_t) );
                dbg2( "overriding MF variable coefficient Y\n" );
            }
            break;

        case SIQ_MF_VAR_COEFF_Y_SF : 
            sv->mf_varcoef_y_sf = sd->num32;
            dbg2( "overriding mf coefficient Y SF\n" );
            break;

        case SIQ_MF_VAR_COEFF_Y_SHIFT : 
            sv->mf_varcoef_y_shift = sd->num32;
            dbg2( "overriding mf coefficient Y shift\n" );
            break;

        case SIQ_MF_VAR_COEFF_CHROMA : 
            if( sd->num_values != MF_COEF_MAX_COEFFICIENTS ) {
                dbg1( "invalid MF chroma LUT: num entries should be %d but is %d\n", 
                            MF_COEF_MAX_COEFFICIENTS, sd->num_values );
            }
            else {
                memcpy( sv->mf_varcoef_chroma, sd->num32_values, sd->num_values*sizeof(uint32_t) );
                dbg2( "overriding MF variable coefficient chroma\n" );
            }
            break;

        case SIQ_MF_VAR_COEFF_CHROMA_SF : 
            sv->mf_varcoef_chroma_sf = sd->num32;
            dbg2( "overriding mf coefficient chroma SF\n" );
            break;

        case SIQ_MF_VAR_COEFF_CHROMA_SHIFT : 
            sv->mf_varcoef_chroma_shift = sd->num32;
            dbg2( "overriding mf coefficient chroma shift\n" );
            break;

        case SIQ_MF_TE :
            siq_merge_bool( sd, &sv->use_mf_te );
            dbg2( "overriding MF TE bool\n" );
            break;
        case SIQ_TE_COUNT_BLACK_MIN:
            sv->te_count_black_min = sd->num32;
            dbg2( "overriding MF TE count black min\n" );
            break;
        case SIQ_TE_COUNT_WHITE_MIN:
            sv->te_count_white_min = sd->num32;
            dbg2( "overriding MF TE count white min\n" );
            break;
        case SIQ_TE_COUNT_WHITE_PLUS_BLACK_MIN:
            sv->te_count_white_plus_black_min = sd->num32;
            dbg2( "overriding MF TE count white plus black min\n" );
            break;
        case SIQ_TE_CENTER_COLOR_MAX:
            sv->te_center_color_max = sd->num32;
            dbg2( "overriding MF TE center_color_max\n" );
            break;
        case SIQ_TE_INTENSE_COLOR_MAX:
            sv->te_intense_color_max = sd->num32;
            dbg2( "overriding MF TE intense_color_max\n" );
            break;
        case SIQ_TE_BLACK_Y_MAX:
            sv->te_black_y_max = sd->num32;
            dbg2( "overriding MF TE black_y_max\n" );
            break;
        case SIQ_TE_BLACK_CbCr_MAX:
            sv->te_black_CbCr_max = sd->num32;
            dbg2( "overriding MF TE black_CbCr_max\n" );
            break;
        case SIQ_TE_WHITE_Y_MIN:
            sv->te_white_y_min = sd->num32;
            dbg2( "overriding MF TE black_y_min\n" );
            break;
        case SIQ_TE_WHITE_CbCr_MAX:
            sv->te_white_CbCr_max = sd->num32;
            dbg2( "overriding MF TE white_CbCr_max\n" );
            break;
        case SIQ_TE_USE_5X5_MATRIX :
            sv->te_use_5x5_matrix = sd->num32;
            dbg2( "overriding MF TE use 5x5 matrix\n" );
            break;

        /*
         * Half Pack
         */
        case SIQ_HP_BPP :
            /* valid values are 4,2,1 */
            if( sd->num32!=4 && sd->num32!=2 && sd->num32!=1 ) {
                dbg1( "ignoring invalid HalfPack BPP value %ld\n", sd->num32 );
            }
            else {
                sv->hp_bpp = sd->num32;
                dbg2( "overriding hp bpp\n" );
            }
            break;

        case SIQ_HP_HORIZ_REP :
            siq_merge_bool( sd, &sv->use_hp_horiz_rep );
            dbg2( "overriding HalfPack horiz rep bool\n" );
            break;

        case SIQ_HP_SIZE_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->hp_size_lut, 
                                              PIE_HP_SIZE_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding HalfPack Size LUT\n" );
                sv->use_hp_size_lut = true;
            }
            break;

        case SIQ_HP_SBIAS_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->hp_sbias_lut, 
                                              PIE_HP_SBIAS_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding HalfPack Signed Bias LUT\n" );
                sv->use_hp_sbias_lut = true;
            }
            break;

        case SIQ_HP_DENSITY_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->hp_density_lut, 
                                              PIE_HP_DENSITY_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding HalfPack Density LUT\n" );
                sv->use_hp_density_lut = true;
            }
            break;

        case SIQ_HP_DIFFUS_LUT :
            scerr = siq_merge_uint32_array( sd, &sv->hp_diffus_lut, 
                                              PIE_HP_DIFFUS_LUT_SIZE );
            if( scerr==0 ) {
                dbg2( "overriding HalfPack Error Diffusion LUT\n" );
                sv->use_hp_diffus_lut = true;
            }
            break;

        case SIQ_HP_ALG_INVERT_DATA :
            siq_merge_bool( sd, &sv->hp_alg_invert_data );
            dbg2( "overriding HalfPack Invert Data\n" );
            break;

        case SIQ_HP_ALG_ACCERR  :
            siq_merge_bool( sd, &sv->hp_alg_accum_err );
            dbg2( "overriding HalfPack Accum Error\n" );
            break;

        case SIQ_HP_ALG_SBIAS_I :
            siq_merge_bool( sd, &sv->hp_alg_sbias_idx );
            dbg2( "overriding HalfPack SBias Index\n" );
            break;

        case SIQ_HP_ALG_SBIAS_D :
            siq_merge_bool( sd, &sv->hp_alg_sbias_dot );
            dbg2( "overriding HalfPack SBias DoADot\n" );
            break;

        case SIQ_HP_ALG_DOADOT :
            siq_merge_bool( sd, &sv->hp_alg_doadot );
            dbg2( "overriding HalfPack DoADot\n" );
            break;

        case SIQ_HP_ALG_SHIFT :
            /* check for valid shift values; ignore bad values */
            if( sd->num32 < 4 || sd->num32 > 7 ) {
                dbg1( "ignoring invalid shift value %ld; want value in [4-7]\n", 
                            sd->num32);
            }
            else {
                sv->hp_alg_shift = sd->num32;
                sv->use_hp_alg_shift = true;
                dbg2( "overriding HalfPack shift\n" );
            }
            break;

        case SIQ_HP_ALG_ERRW1 :
            /* require 4-bit value */
            if( siq_merge_uint8( sd, &sv->hp_alg_errweight1, 4 ) == 0 ) {
                dbg1( "overriding HalfPack error weight 1\n" );
            }
            break;

        case SIQ_HP_ALG_ERRW2 :
            /* require 4-bit value */
            if( siq_merge_uint8( sd, &sv->hp_alg_errweight2, 4 ) == 0 ) {
                dbg1( "overriding HalfPack error weight 2\n" );
            }
            break;

        case SIQ_HP_ALG_ERRW3 :
            /* require 4-bit value */
            if( siq_merge_uint8( sd, &sv->hp_alg_errweight3, 4 ) == 0 ) {
                dbg1( "overriding HalfPack error weight 3\n" );
            }
            break;

        case SIQ_HP_BIAS_SRAM_CFG :
            /* valid values are 0 and 1 */
            if( siq_merge_uint8( sd, &sv->hp_bias_sram_cfg, 1 ) == 0 ) {
                dbg1( "overriding HalfPack sram cfg\n" );
            }
            break;

        case SIQ_HP_BIAS_REG0 :
            /* full 8-bit value */
            if( siq_merge_uint8( sd, &sv->hp_bias_reg0, 8 ) == 0 ) {
                dbg1( "overriding HalfPack bias reg0\n" );
            }
            break;

        case SIQ_HP_BIAS_REG1 :
            /* 8-bit value */
            if( siq_merge_uint8( sd, &sv->hp_bias_reg1, 8 ) == 0 ) {
                dbg1( "overriding HalfPack bias reg1\n" );
            }
            break;


        default :
            /* ignore unknown/unwanted data */
            break;
    }
}

/**
 * \brief  Merge siqfile data into a scanvar
 *
 * After a scanvar is instantiated, the scanvar constructor should call this
 * function to add in or replace any siqfile data.
 *
 * \author David Poole
 * \date 26-Dec-2005
 *
 */

void
scanvar_siq_merge( struct scanvars *sv )
{
    int i;
    const struct siqdata_table *siqdata;
    const struct siqdata *sd;

    dbg2( "%s %d\n", __FUNCTION__, sv->id );

    /* does this scanvar even have any siqfile data available for it? If not,
     * leave now
     */
    siqdata = siqdata_lookup( sv->id );
    if( siqdata==NULL ) {
        dbg2( "no host scaniq data for %d\n", sv->id );
        return;
    }

    /* user sent down some data for this particular scan; go through each of
     * the siqfile variables and override the scanvar data with it
     */
    for( i=0 ; i<siqdata->num_entries ; i++ ) {
        sd = (const struct siqdata *)&siqdata->data[i];
        scanvar_siq_merge_data( sd, sv );
    }
}

/**
 * \brief  look up a scanvar siqname by its key (e.g., SIQ_GAMMA)
 *
 * \author David Poole
 * \date 22-May-2007
 *
 */

bool 
scanvar_siq_has_key( struct scanvars *sv, int key )
{
    int idx;
    const struct siqdata_table *siqdata;

    /* does this scanvar even have any siqfile data available for it? If not,
     * leave now
     */
    siqdata = siqdata_lookup( sv->id );
    if( siqdata==NULL ) {
        return false;
    }

    /* look up the name and convert to a scanvar class id */
    idx = siqdata_find_key( siqdata, key );
    if( idx == -1 ) {
        return false;
    }

    return true;
}

/**
 * \brief functions to print data in scanvars 
 *
 *
 * \author David Poole
 * \date 25-Jun-2006
 *
 * davep 31-Oct-2012 ; add whacky mechanism to capture siqfile to a string
 * through a global registered siqfile_print function pointer.
 */

static void p( const char *fmt, ... )
{
    va_list ap;

    va_start( ap, fmt );
    if( siqfile_print ) {
        siqfile_print( fmt, ap );
    }
    else {
        vscanlog2( (char *)fmt, ap );
    }
    va_end( ap );
}

static void
print_uint32( const char *name, const uint32_t num32 )
{
    p( "%s = %ld\n", name, num32 );
}

static void
print_bool( const char *name, const bool flag )
{
    p( "%s = %d\n", name, flag );
}

static void
xprint_uint32_array( const char *name, 
                     const struct uint32_array *array, 
                     char *num_fmt )
{
    int i;

    if( array->num_entries > 8 ) {
        p( "%s = {\n    ", name );
    }
    else {
        p( "%s = { ", name );
    }

    for( i=0 ; i<array->num_entries ; i++ ) {
        p( num_fmt, array->data[i] );
        if( (i+1)%16 == 0 ) {
            p( "\n    " );
        }
    }
    p( "}\n" );
}

static void
print_uint32_array( const char *name, const struct uint32_array *array )
{
    xprint_uint32_array( name, array, "%ld, " );
}

/* davep 25-Jan-2007 ; don't need this yet (kept getting a warning) */
#if 0
static void
printhex_uint32_array( const char *name, const struct uint32_array *array )
{
    xprint_uint32_array( name, array, "%#lx, " );
}
#endif

/* Needed to do this hack so I could use the same "%d" format with arrays of
 * uint16_t[] and arrays of int[]; otherwise, I get a warning when passing an
 * int[] to a uint16_t[] function and vice versa.
 *
 * Some days, I long for C++ and templates.
 */
#define PRINT_DECIMAL_ARRAY(name,array,len)  \
    do {                                \
        int i;                          \
        p( "%s = { ", name );      \
        for( i=0 ; i<len ; i++ ) {      \
            p( "%d, ", array[i] ); \
            if( (i+1)%16 == 0 ) {       \
                p( "\n    " );     \
            }                           \
        }                               \
        p( "}\n" );                \
    }                                   \
    while(0);

static void
print_raw_uint16_array( const char *name, const uint16_t array[], int len )
{
    PRINT_DECIMAL_ARRAY(name,array,len);
}

static void
print_raw_int_array( const char *name, const int array[], int len )
{
    PRINT_DECIMAL_ARRAY(name,array,len);
}

static void
xprint_raw_uint32_array( const char *name, const uint32_t array[], 
                         int len, char *num_fmt )
{
    int i;

    p( "%s = { ", name );
    for( i=0 ; i<len ; i++ ) {
        p( num_fmt, array[i] );
        if( (i+1)%16 == 0 ) {
            p( "\n    " );
        }
    }
    p( "}\n" );
}

/* davep 25-Jan-2007 ; don't need this yet (kept getting a warning) */
#if 0
static void
printhex_raw_uint32_array( const char *name, const uint32_t array[], int len )
{
    xprint_raw_uint32_array( name, array, len, "%#lx, " );
}
#endif

static void
print_raw_uint32_array( const char *name, const uint32_t array[], int len )
{
    xprint_raw_uint32_array( name, array, len, "%ld, " );
}

static void
print_tcns( const char *name, const struct uint32_array * array, int shift )
{
    int i;

    /* tcns is actually 3 8-bit values stored in a 32-bit value across byte
     * boundries. In order to be able to read the silly thing, I split it
     * across three different tables
     */
    p( "%s = { ", name );
    for( i=0 ; i<array->num_entries; i++ ) {
        /* only print the bits of the field we currently want */
        p( "%ld, ", (array->data[i] & (0xff<<shift)) >> shift );
        if( (i+1)%16 == 0 ) {
            p( "\n    " );
        }
    }
    p( "}\n" );
}

static void
print_mf_ai_lut( const char *name, const struct uint32_array * array, int shift ) 
{
    int i;
    signed char num8;

    /* MF Activity Index lut is two signed 8-bit fields stuck together in a
     * 16-bit field
     */
    p( "%s = { ", name );
    for( i=0 ; i<array->num_entries; i++ ) {
        /* only print the bits of the field we currently want; fields are
         * signed 8-bit integers
         *
         * BIG FAT NOTE! I'm trusting the compiler and host hardware a LOT. I'm
         * assuming a char is 8-bits. I'm assuming the native hardware uses
         * two's compliment (same as the MF AI LUT). I'm assuming the printf()
         * we use will correctly dump an 8-bit value passed through %d as
         * negative.
         */
        num8 = (array->data[i] & (0xff<<shift)) >> shift;
        p( "%d, ", num8 );
//        p( "%ld, ", (array->data[i] & (0xff<<shift)) >> shift );
        if( (i+1)%16 == 0 ) {
            p( "\n    " );
        }
    }
    p( "}\n" );
}


static void 
print_var( const struct scanvars *sv, const struct siqdata_name *name )
{
    uint32_t num32;

    switch( name->key ) {
        case SIQ_NAME :
            p( "%s = \"%s\"\n", name->str, scanvar_name_lookup( sv->id ) );
            break;

        /* davep 01-Nov-2012 ; user settings (these aren't stored in the global
         * siqdata cache but are used during scanvar pickle/unpickle
         * operations)
         */
        case SIQ_USER_DPI : 
            print_uint32( name->str, sv->dpi );
            break;

        case SIQ_USER_SCALE : 
            p( "%s = { %d, %d, %d, %d }\n", name->str,
                    sv->scale.x_numer, sv->scale.x_denom,  
                    sv->scale.y_numer, sv->scale.y_denom );
            break;

        case SIQ_USER_AREA : 
            p( "%s = { %d, %d, %d, %d }\n", name->str,
                    sv->user_area_hinch.x, sv->user_area_hinch.y,  
                    sv->user_area_hinch.width, sv->user_area_hinch.height );
            break;

        case SIQ_CAPTURE_CAL :
            print_bool( name->str, sv->capture_cal );
            break;

        case SIQ_PIEOUT : 
            p( "%s = %d\n", name->str, sv->pieout );
            break;

        case SIQ_BPP: 
            p( "%s = %d\n", name->str, scanvar_get_bpp(sv) );
            break;

        case SIQ_DOC_SRC :
            p( "%s = %d\n", name->str, sv->doc_src );
            break;

        /*
         * calibration targets
         */
        case SIQ_CAL_WHITE_DIGITAL :
            if( sv->use_cal_white_digital_target ) {
                print_raw_uint16_array( name->str, sv->cal_white_digital_target,
                        SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_CAL_BLACK_DIGITAL:
            if( sv->use_cal_black_digital_target ) {
                print_raw_uint16_array( name->str, sv->cal_black_digital_target,
                        SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_CAL_WHITE_ANALOG :
            if( sv->use_cal_white_analog_target ) {
                print_raw_uint16_array( name->str, sv->cal_white_analog_target,
                        SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_CAL_BLACK_ANALOG :
            if( sv->use_cal_black_analog_target ) {
                print_raw_uint16_array( name->str, sv->cal_black_analog_target,
                        SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_CAL_ANALOG_OFFSET :
            if( sv->use_cal_analog_offset ) {
                print_raw_int_array( name->str, sv->cal_analog_offset,
                        SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_CAL_ANALOG_GAIN :
            if( sv->use_cal_analog_gain ) {
                print_raw_uint16_array( name->str, sv->cal_analog_gain,
                        SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_CAL_LED_PWM :
            if( sv->use_cal_led_pwm ) {
                print_raw_uint16_array( name->str, sv->cal_led_pwm,
                        SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_CAL_LED_EXP :
            if( sv->use_cal_led_exp ) {
                print_raw_uint16_array( name->str, sv->cal_led_exp,
                        SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_CAL_USE_GRAY_ALG :
            print_bool( name->str, sv->use_gray_cal );
            break;

        /* 
         * Sorta calibration but not really. 
         */
        case SIQ_WHITE_CLIP_POINT  :
            if( sv->use_white_clip_point ) {
                print_raw_uint16_array( name->str, sv->white_clip_point, SV_COLOR_NUM_ENTRIES );
            }
            break;

        case SIQ_BLACK_CLIP_POINT  :
            if( sv->use_black_clip_point ) {
                print_raw_uint16_array( name->str, sv->black_clip_point, SV_COLOR_NUM_ENTRIES );
            }
            break;
        
        case SIQ_GAMMA :
            /* pretty much any number is valid here. Feel free to shoot
             * yourself in the foot.
             */
            print_uint32( name->str, sv->gamma_times_10 );
            break;

        case SIQ_CONTRAST :
            p( "%s = %d\n", name->str, sv->contrast );
            break;

        case SIQ_BRIGHTNESS :
            print_uint32( name->str, sv->brightness );
            break;

        case SIQ_COPY_BRIGHTNESS :
            p( "%s = %d\n", name->str, sv->copy_brightness );
            break;

        case SIQ_SHARPNESS :
            p( "%s = %d\n", name->str, sv->sharpness );
            break;

        case SIQ_RGB_TINT : 
            print_raw_int_array( name->str, sv->rgb_tint , SV_COLOR_NUM_ENTRIES );
            break;

		case SIQ_BACKGROUND :
			dbg1( "[BG] BGR print_var!!\n");
            p( "%s = %d\n", name->str, sv->bgr_sensitivity );
            break;
            
        case SIQ_SW_FILTER :
            /* possible values are 0, 1, 2
             * 0 - disabled
             * 1 - filter after PIC (not yet implemented)
             * 2 - filter after PIE
             */
            p( "%s = %d\n", name->str, sv->sw_filter_position );
            break;

        case SIQ_FW_MARGINS : 
            print_bool( name->str, sv->use_fw_margins );
            break;

        /*
         * PIC BDR (Bit Depth Reduction) 
         */  
        case SIQ_PIC_BDR_ENABLE :
            print_bool( name->str, sv->use_pic_bdr );
            break;

        case SIQ_PIC_BDR_LUT :
            if( sv->use_pic_bdr && sv->use_pic_bdr_lut ) {
                print_uint32_array( name->str, &sv->pic_bdr_lut );
            }
            break;

        /* 
         * CSC (Color Space Conversion)
         */
        case SIQ_CSC_ENABLE :
            print_bool( name->str, sv->use_csc );
            break;

        case SIQ_CSC_RGB_LUT :
            if( sv->use_csc && sv->use_csc_rgb_lut ) {
                print_uint32_array( name->str, &sv->csc_rgb_lut );
            }
            break;

        case SIQ_CSC_RGB_MATRIX :
            if( sv->use_csc ) {
                /* csc_rgb_matrix is a 3x3 matrix */
                print_raw_uint32_array( name->str, sv->csc_rgb_matrix, 9 );
            }
            break;

        case SIQ_CSC_RGBYCC_MATRIX :
            if( sv->use_csc ) {
                /* another 3x3 matrix */
                print_raw_uint32_array( name->str, sv->csc_rgbycc_matrix, 9 );
            }
            break;

        case SIQ_CSC_YCCRGB_MATRIX :
            if( sv->use_csc ) {
                /* another 3x3 matrix */
                print_raw_uint32_array( name->str, sv->csc_yccrgb_matrix, 9 );
            }
            break;

        case SIQ_CSC_RGBSRGB_BYPASS :
            if( sv->use_csc ) {
                print_bool( name->str, sv->csc_rgb_srgb_bypass );
            }
            break;
            
        case SIQ_CSC_RGBYCC_BYPASS :
            if( sv->use_csc ) {
                print_bool( name->str, sv->csc_srgb_ycc_bypass );
            }
            break;
            
        case SIQ_CSC_YCCRGB_BYPASS :
            if( sv->use_csc ) {
                print_bool( name->str, sv->csc_ycc_rgb_bypass );
            }
            break;

        case SIQ_CSC_R_LUT :
            if( sv->use_csc && sv->use_csc_r_lut ) {
                print_uint32_array( name->str, &sv->csc_r_lut );
            }
            break;

        case SIQ_CSC_G_LUT :
            if( sv->use_csc && sv->use_csc_g_lut ) {
                print_uint32_array( name->str, &sv->csc_g_lut );
            }
            break;

        case SIQ_CSC_B_LUT :
            if( sv->use_csc && sv->use_csc_b_lut ) {
                print_uint32_array( name->str, &sv->csc_b_lut );
            }
            break;
            

        /*
         * TCNS (Tone Contrast and Neutral Sensitivity)
         */
        case SIQ_TCNS_ENABLE :
            print_bool( name->str, sv->use_tcns );
            break;

        case SIQ_TCNS_LUT_Y :
            if( sv->use_tcns && sv->use_tcns_lut ) {
                print_tcns( name->str, &sv->tcns_lut, PIE_TCNS_LUT_Y_SHIFT );
            }
            break;

        case SIQ_TCNS_LUT_CB :
            if( sv->use_tcns && sv->use_tcns_lut ) {
                print_tcns( name->str, &sv->tcns_lut, PIE_TCNS_LUT_CB_SHIFT );
            }
            break;

        case SIQ_TCNS_LUT_CR :
            if( sv->use_tcns && sv->use_tcns_lut ) {
                print_tcns( name->str, &sv->tcns_lut, PIE_TCNS_LUT_CR_SHIFT );
            }
            break;

        /* davep 05-Nov-2012 ; adding PIE ColorShift */
        case SIQ_CIS_COLORSHIFT_ENABLE :
            print_bool( name->str, sv->use_colorshift );
            break;

        case SIQ_CIS_COLORSHIFT_ROUND_MODE :
            if( sv->use_colorshift ) {
                print_uint32( name->str, sv->cis_colorshift_round_mode  );
            }
            break;

        case SIQ_CIS_COLORSHIFT_MMODE :
            if( sv->use_colorshift ) {
                print_raw_uint32_array( name->str, sv->cis_colorshift_mmode, 3 );
            }
            break;

        case SIQ_CIS_COLORSHIFT_MULT :
            if( sv->use_colorshift ) {
                print_raw_uint32_array( name->str, sv->cis_colorshift_mult, 3 );
            }
            break;

        /*
         * MultiFilter
         */
        case SIQ_MF_ENABLE :
            print_bool( name->str, sv->use_mf );
            break;

        case SIQ_MF_AI_LUT_Y :
            if( sv->use_mf && sv->use_mf_ai_lut ) {
                print_mf_ai_lut( name->str, &sv->mf_ai_lut, 8 );
            }
            break;

        case SIQ_MF_AI_LUT_CBCR :
            if( sv->use_mf && sv->use_mf_ai_lut ) {
                print_mf_ai_lut( name->str, &sv->mf_ai_lut, 0 );
            }
            break;

        case SIQ_MF_Y_RADIUS :
            if( !sv->use_mf ) {
                break;
            }
            switch( sv->mf_y_radius ) {
                case PIE_FILTER_SCR1_Y_1 :
                    num32 = 10;
                    break;
                case PIE_FILTER_SCR1_Y_1_5 :
                    num32 = 15;
                    break;
                case PIE_FILTER_SCR1_Y_2 :
                    num32 = 20;
                    break;
                default :
                    XASSERT( 0, sv->mf_y_radius );
                    num32 = -1;
                    break;
            }
            print_uint32( name->str, num32 );
            break;

        case SIQ_MF_CBCR_RADIUS :
            if( !sv->use_mf ) {
                break;
            }
            switch( sv->mf_cbcr_radius ) {
                case PIE_FILTER_SCR1_R_1 :
                    num32 = 10;
                    break;
                case PIE_FILTER_SCR1_R_1_5 :
                    num32 = 15;
                    break;
                case PIE_FILTER_SCR1_R_2 :
                    num32 = 20;
                    break;
                default :
                    XASSERT( 0, sv->mf_cbcr_radius );
                    num32 = -1;
                    break;
            }
            print_uint32( name->str, num32 );
            break;

        case SIQ_MF_SCALE_10 :
            if( !sv->use_mf ) {
                break;
            }
            switch( sv->mf_scale_10 ) {
                case PIE_FILTER_SCR1_S_1 :
                    num32 = 1000;
                    break;
                case PIE_FILTER_SCR1_S_0_5 :
                    num32 = 500;
                    break;
                case PIE_FILTER_SCR1_S_0_25 :
                    num32 = 250;
                    break;
                case PIE_FILTER_SCR1_S_0_125 :
                    num32 = 125;
                    break;
                default:
                    XASSERT( 0, sv->mf_scale_10 );
                    num32 = -1;
                    break;
            }
            print_uint32( name->str, num32 );
            break;

        case SIQ_MF_SCALE_15 :
            if( !sv->use_mf ) {
                break;
            }
            switch( sv->mf_scale_15 ) {
                case PIE_FILTER_SCR1_M_1 :
                    num32 = 1000;
                    break;
                case PIE_FILTER_SCR1_M_0_5 :
                    num32 = 500;
                    break;
                case PIE_FILTER_SCR1_M_0_25 :
                    num32 = 250;
                    break;
                case PIE_FILTER_SCR1_M_0 :
                    num32 = 0;
                    break;
                default:
                    XASSERT( 0, sv->mf_scale_15 );
                    num32 = -1;
                    break;
            }
            print_uint32( name->str, num32 );
            break;

        case SIQ_MF_SCALE_20 :
            if( !sv->use_mf ) {
                break;
            }
            switch( sv->mf_scale_20 ) {
                case PIE_FILTER_SCR1_P_0_5 :
                    num32 = 500;
                    break;
                case PIE_FILTER_SCR1_P_0_25 :
                    num32 = 250;
                    break;
                case PIE_FILTER_SCR1_P_0_125 :
                    num32 = 125;
                    break;
                case PIE_FILTER_SCR1_P_0 :
                    num32 = 0;
                    break;
                default:
                    XASSERT( 0, sv->mf_scale_20 );
                    num32 = -1;
                    break;
            }
            print_uint32( name->str, num32 );
            break;

        case SIQ_MF_OUTPUT_SCALE :
            if( !sv->use_mf ) {
                break;
            }
            switch( sv->mf_lut_output_scale ) {
                case PIE_FILTER_SCR1_N_16 :
                    num32 = 16;
                    break;
                case PIE_FILTER_SCR1_N_32 :
                    num32 = 32;
                    break;
                case PIE_FILTER_SCR1_N_64:
                    num32 = 64;
                    break;
                case PIE_FILTER_SCR1_N_128 :
                    num32 = 128;
                    break;
                default:
                    XASSERT( 0, sv->mf_lut_output_scale );
                    num32 = -1;
                    break;
            }
            print_uint32( name->str, num32 );
            break;

        case SIQ_MF_OFFSET :
            if( !sv->use_mf ) {
                break;
            }
            num32 = sv->mf_offset >> PIE_FILTER_SCR1_OFFSET_SHIFT;
            print_uint32( name->str, num32 );
            break;

        case SIQ_MF_ENABLE_EROSION : 
            print_bool( name->str, sv->use_mf_erosion );
            break;

        case SIQ_MF_ENABLE_DILATION : 
            print_bool( name->str, sv->use_mf_dilation );
            break;

        case SIQ_MF_ENABLE_VAR_COEFF : 
            print_bool( name->str, sv->use_mf_varcoefficients );
            break;

        case SIQ_MF_VAR_COEFF_Y : 
            if( sv->use_mf_varcoefficients ) {
                print_raw_uint32_array( name->str, sv->mf_varcoef_y, MF_COEF_MAX_COEFFICIENTS );
            }
            break;

        case SIQ_MF_VAR_COEFF_Y_SF : 
            if( sv->use_mf_varcoefficients ) {
                print_uint32( name->str, sv->mf_varcoef_y_sf );
            }
            break;

        case SIQ_MF_VAR_COEFF_Y_SHIFT : 
            if( sv->use_mf_varcoefficients ) {
                print_uint32( name->str, sv->mf_varcoef_y_shift );
            }
            break;

        case SIQ_MF_VAR_COEFF_CHROMA : 
            if( sv->use_mf_varcoefficients ) {
                print_raw_uint32_array( name->str, sv->mf_varcoef_chroma, MF_COEF_MAX_COEFFICIENTS );
            }
            break;

        case SIQ_MF_VAR_COEFF_CHROMA_SF : 
            if( sv->use_mf_varcoefficients ) {
                print_uint32( name->str, sv->mf_varcoef_chroma_sf );
            }
            break;

        case SIQ_MF_VAR_COEFF_CHROMA_SHIFT : 
            if( sv->use_mf_varcoefficients ) {
                print_uint32( name->str, sv->mf_varcoef_chroma_shift );
            }
            break;

        case SIQ_MF_TE :
            print_bool( name->str, sv->use_mf_te );
            break;
        case SIQ_TE_COUNT_BLACK_MIN:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_count_black_min );
            }
            break;
        case SIQ_TE_COUNT_WHITE_MIN:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_count_white_min );
            }
            break;
        case SIQ_TE_COUNT_WHITE_PLUS_BLACK_MIN:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_count_white_plus_black_min );
            }
            break;
        case SIQ_TE_CENTER_COLOR_MAX:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_center_color_max );
            }
            break;
        case SIQ_TE_INTENSE_COLOR_MAX:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_intense_color_max );
            }
            break;
        case SIQ_TE_BLACK_Y_MAX:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_black_y_max );
            }
            break;
        case SIQ_TE_BLACK_CbCr_MAX:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_black_CbCr_max );
            }
            break;
        case SIQ_TE_WHITE_Y_MIN:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_white_y_min );
            }
            break;
        case SIQ_TE_WHITE_CbCr_MAX:
            if( sv->use_mf_te ) {
                print_uint32( name->str, sv->te_white_CbCr_max );
            }
            break;
        case SIQ_TE_USE_5X5_MATRIX :
            if( sv->use_mf_te ) {
                print_bool( name->str, sv->te_use_5x5_matrix ); 
            }
            break;

        /*
         * Half Pack
         */
        case SIQ_HP_BPP :
            if( sv->use_hp ) {
                print_uint32( name->str, sv->hp_bpp );
            }
            break;

        case SIQ_HP_HORIZ_REP :
            if( sv->use_hp ) {
                print_bool( name->str, sv->use_hp_horiz_rep );
            }
            break;

        case SIQ_HP_SIZE_LUT :
            if( sv->use_hp_size_lut ) {
                print_uint32_array( name->str, &sv->hp_size_lut );
            }
            break;

        case SIQ_HP_SBIAS_LUT :
            if( sv->use_hp_sbias_lut ) {
                print_uint32_array( name->str, &sv->hp_sbias_lut );
            }
            break;

        case SIQ_HP_DENSITY_LUT :
            if( sv->use_hp_density_lut ) {
                print_uint32_array( name->str, &sv->hp_density_lut );
            }
            break;

        case SIQ_HP_DIFFUS_LUT :
            if( sv->use_hp_diffus_lut ) {
                print_uint32_array( name->str, &sv->hp_diffus_lut );
            }
            break;

        case SIQ_HP_ALG_INVERT_DATA :
            if( sv->use_hp ) {
                print_bool( name->str, sv->hp_alg_invert_data );
            }
            break;

        case SIQ_HP_ALG_ACCERR  :
            if( sv->use_hp ) {
                print_bool( name->str, sv->hp_alg_accum_err );
            }
            break;

        case SIQ_HP_ALG_SBIAS_I :
            if( sv->use_hp ) {
                print_bool( name->str, sv->hp_alg_sbias_idx );
            }
            break;

        case SIQ_HP_ALG_SBIAS_D :
            if( sv->use_hp ) {
                print_bool( name->str, sv->hp_alg_sbias_dot );
            }
            break;

        case SIQ_HP_ALG_DOADOT :
            if( sv->use_hp ) {
                print_bool( name->str, sv->hp_alg_doadot );
            }
            break;

        case SIQ_HP_ALG_SHIFT :
            if( sv->use_hp_alg_shift ) {
                print_uint32( name->str, sv->hp_alg_shift );
            }
            break;

        case SIQ_HP_ALG_ERRW1 :
            if( sv->use_hp ) {
                p( "%s = %d\n", name->str, sv->hp_alg_errweight1 );
            }
            break;

        case SIQ_HP_ALG_ERRW2 :
            if( sv->use_hp ) {
                p( "%s = %d\n", name->str, sv->hp_alg_errweight2 );
            }
            break;

        case SIQ_HP_ALG_ERRW3 :
            if( sv->use_hp ) {
                p( "%s = %d\n", name->str, sv->hp_alg_errweight3 );
            }
            break;

        case SIQ_HP_BIAS_SRAM_CFG :
            if( sv->use_hp ) {
                p( "%s = %d\n", name->str, sv->hp_bias_sram_cfg );
            }
            break;

        case SIQ_HP_BIAS_REG0 :
            if( sv->use_hp ) {
                p( "%s = %d\n", name->str, sv->hp_bias_reg0 );
            }
            break;

        case SIQ_HP_BIAS_REG1 :
            if( sv->use_hp ) {
                p( "%s = %d\n", name->str, sv->hp_bias_reg1 );
            }
            break;

        default :
            /* unknown/unwanted data */
            dbg2( "%s unknown key=%d\n", __FUNCTION__, name->key );
            XASSERT( 0, name->key );
            break;
    }
}

/**
 * \brief  Print a scanvar in the siqfile format. 
 *
 * Useful for exporting scanvar settings.
 *
 *
 * \author David Poole
 * \date 25-Jun-2006
 *
 */

void scanvar_siq_print( const struct scanvars *sv )
{
    const struct siqdata_name *name;

    name = siqdata_get_first_name();

    while( name != NULL ) {

        print_var( sv, name );

        name = siqdata_get_next_name( name );
    }

    dbg1("%s done\n", __FUNCTION__ );
}

