/**
 * \file cal.c
 *
 * \brief Scanner calibration ASIC-dependent  routines.
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2006-2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "scos.h" 

#include "list.h"           
#include "lassert.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scanvars.h"
#include "scanmech.h"
#include "safetylock.h"
#include "icedma.h"
#include "pic.h"
#include "cal.h"
#include "scands.h"
#include "cal_common.h"
#include "scanlib.h"

/* forward declarations */

int cal_retry_flag = 0;

extern uint32_t sensor_num_cal;

static int cal_hw_setup(const CAL_CONFIG *pconfig);

static cal_err_t processCalibrationCase(CalibrationCase *pCCase);


static const CAL_CONFIG calconf_32BITPRNUDSNU_NOQUAD = { 
                                              USE_QUAD:false,
                                              PRNUDSNU_LUT_ENTRY_SIZE:4,
                                              PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU:true,
                                              DSNU_SHIFT:0,
                                              MAX_DIGITAL_CORRECTION:0xFFFF,
                                              DIGITAL_GAIN_PRECISION:14,
                                              MAX_DIGITAL_GAIN_TIMES_10:50,
                                              ANALOG_GAIN_CAP_TIMES_10:79,
                                              COLOR_SAT_SLOPE_THRESHOLD:95,
                                              MONO_SAT_SLOPE_THRESHOLD:95,
                                              REPLACE_BAD_PIXEL:0xFFFF,
                                              BYPASS_REPLACE_BAD_PIXEL: false,
                                              hw_setup: cal_hw_setup,
                                              build_case: create_calibration_case,
                                              process_case: processCalibrationCase };

static const CAL_CONFIG calconf_24BITPRNUDSNU_NOQUAD = { 
                                              USE_QUAD:false,
                                              PRNUDSNU_LUT_ENTRY_SIZE:3,
                                              PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU:false,
                                              DSNU_SHIFT:4,
                                              MAX_DIGITAL_CORRECTION:0xFFF,
                                              DIGITAL_GAIN_PRECISION:10,
                                              MAX_DIGITAL_GAIN_TIMES_10:50,
                                              ANALOG_GAIN_CAP_TIMES_10:79,
                                              COLOR_SAT_SLOPE_THRESHOLD:95,
                                              MONO_SAT_SLOPE_THRESHOLD:95,
                                              REPLACE_BAD_PIXEL:0xFFF,
                                              BYPASS_REPLACE_BAD_PIXEL: false,
                                              hw_setup: cal_hw_setup,
                                              build_case: create_calibration_case,
                                              process_case: processCalibrationCase };

static const CAL_CONFIG calconf_24BITPRNUDSNU_QUAD = {  
                                              USE_QUAD:true,
                                              PRNUDSNU_LUT_ENTRY_SIZE:4,
                                              PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU:false,
                                              DSNU_SHIFT:4,
                                              MAX_DIGITAL_CORRECTION:0xFFF,
                                              DIGITAL_GAIN_PRECISION:10,
                                              MAX_DIGITAL_GAIN_TIMES_10:50,
                                              ANALOG_GAIN_CAP_TIMES_10:79,
                                              COLOR_SAT_SLOPE_THRESHOLD:95,
                                              MONO_SAT_SLOPE_THRESHOLD:95,
                                              REPLACE_BAD_PIXEL:0xFFF,
                                              BYPASS_REPLACE_BAD_PIXEL: false,
                                              hw_setup: cal_hw_setup,
                                              build_case: create_calibration_case,
                                              process_case: processCalibrationCase };

const CAL_CONFIG *cal_get_config()
{
    /* davep 22-Oct-2012 ; disable quad until cal strip overscan is stable */
    return &calconf_24BITPRNUDSNU_NOQUAD;
//    return &calconf_24BITPRNUDSNU_QUAD;
}

/*
    One time configuration of the calibration
*/
static int cal_hw_setup(const CAL_CONFIG *pconfig)
{
   if((pconfig->PRNUDSNU_LUT_ENTRY_SIZE != 4) &&  (pconfig->PRNUDSNU_LUT_ENTRY_SIZE != 3))
        return 1;

    /* Check everything before set hardware*/
    if(pconfig->USE_QUAD)
    { 
#ifndef HAVE_PIC_PC_QUAD
        return 1;           
#endif
        /* can not do 32-bit PRNU/DSNU when QUAD is enabled*/
        if (pconfig->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU)
            return 2;

        if(pconfig->PRNUDSNU_LUT_ENTRY_SIZE != 4)
            return 3;
    }else
    {
        if(pconfig->PRNUDSNU_LUT_ENTRY_SIZE ==4 && !pconfig->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU)
            return 4;

        if(pconfig->PRNUDSNU_LUT_ENTRY_SIZE !=4 && pconfig->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU)
            return 5;
    }

    /* TBD: Do more thorough check*/
    if(pconfig->DIGITAL_GAIN_PRECISION >=12 && !pconfig->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU)
        return 6;

    // make sure that the saturation slope thresholds are reasonable
    if((pconfig->COLOR_SAT_SLOPE_THRESHOLD > 100) || (pconfig->MONO_SAT_SLOPE_THRESHOLD > 100))
        return 7;
   
    if(pconfig->USE_QUAD || !pconfig->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU)
    {
        pic_prnudsnu_set_num_correction_bits(24);
    }
    else
    {
        pic_prnudsnu_set_num_correction_bits(32);
    }

    if(pconfig->USE_QUAD)
    {
        /* hardwire the thing to 12/12/8 (quad/prnu/dsnu) */
        pic_prnudsnu_set_coeffwidth(32);
    }
    else
    {
        if(!pconfig->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU) {
            pic_prnudsnu_set_coeffwidth(24);
        }
        else {
            pic_prnudsnu_set_coeffwidth(32);
        }
    }

    pic_prnudsnu_set_num_extra_prnu_bits_col0_mono_even(0);
    pic_prnudsnu_set_num_extra_prnu_bits_col1_odd(0);
    pic_prnudsnu_set_num_extra_prnu_bits_col2(0);

    if(pconfig->MAX_DIGITAL_GAIN_TIMES_10!=50)
        return 7;

    if(pconfig->MAX_DIGITAL_CORRECTION==0xFFF)
    {
          /* 1010/2011 Eric: we Only support MAX_DIGITAL_GAIN=5.0 
       Here is how I get the multiple factor (checked with Paul), note we don't use 
       scale factor
       1) Start with a 12 bit fractional 0.12, which has a range of [0,0.9999..]
       2) Since we want to PRNU to have a max= 4.99999, which means we want
          to use the 0.12 to represent (max-1)=3.99999, that requires a multi factor
           f0=4
       3) Considering we want to use 0x8000 as offset, which means we want 0x8000 
          to be 1.0, that means we want a x.15 as a final PRNU value.

       4) To get 0.12 to x.15, we need another multi factor f1=8 (3 bit shift)
         
        5) Final multi factor is f=f0*f1=32
        */
        pic_prnudsnu_set_prnu_muloff(32, 0, 0x8000);
    }
    else if(pconfig->MAX_DIGITAL_CORRECTION == 0xFFFF)
    {
        /* For 16-bit PRNU, here is how the  factor 2 is calcualted:
          Starting from 0.16, we need f0=4, to convert a 0.16 to a 0.15, we need f1=1/2. Final f=f0*f1=2
        */
        pic_prnudsnu_set_prnu_muloff(2, 0, 0x8000); 
    }else
    {
        return 8; //Error
    }

    pic_prnudsnu_set_dsnu_muloff(1<<(pconfig->DSNU_SHIFT), 0, 0);

    return 0;
} 

/**\brief Routine to start a calibration process.
 * 
 * \param[in] pCCase Calibration case to be processed.
 * 
 * \author Eric Huang
 * \author Brad Simith
 * \author David Poole
 * 
 **/

static cal_err_t
processCalibrationCase(CalibrationCase *pCCase)
{
    scan_err_t scerr;
    cal_cache_t *cached_cal;
    cal_err_t calerr;
    uint32_t num32;

    calerr = 0;

    cal_send_all_calcase_metadata( pCCase );

    cal_send_uint32_metadata( 2, 
            CAL_METADATA_BITS_PER_PIXEL, 16,
            CAL_METADATA_ROWS_PER_STRIP, get_needed_rows_per_buffer( pCCase->horiz_res) );

    // Disable motor motion for the first few steps.
    scan_enable_motor_motion(false);

    calerr = cal_setup( pCCase );
    if( calerr != 0 ) {
        scanlog(0,  "%s cal_setup failed, err=%d\n", __FUNCTION__ ,calerr);
        return calerr;
    }

    /* Eric H.: To avoid saturated sensor output show up in calibration, do a summy scan
                before calibrating.
    */
    calerr= cal_dummy_scan( pCCase );
    if( calerr != 0 ) {
        scanlog(0,  "%s cal_dummy_scan failed, err=%d\n", __FUNCTION__ ,calerr);
        return calerr;
    }

    calerr = cal_calc_analog_offset(pCCase);
    if( calerr != 0 ) {
        scanlog(0, "%s calc_analog_offset() failed with calerr=%d\n", 
                    __FUNCTION__, calerr );
        return calerr;
    }
 
    calerr = cal_calc_pwm_gain(pCCase);
    if( calerr != 0 ) {
        scanlog(0, "%s cal_calc_led_pwm() failed with calerr=%d\n", 
                    __FUNCTION__, calerr ); 
        return calerr;
    }

	int retry_cnt = 0;   

retry:
    // Enable motor motion for the remaining steps
    scan_enable_motor_motion(true);
    calerr = cal_calc_digital_correction(pCCase);
    
    if( calerr != 0 ) {
//        scanlog(0, "%s cal_calc_digital_correction() failed with calerr=%d\n", 
        dbg1("%s cal_calc_digital_correction() failed with calerr=%d\n",         
                   __FUNCTION__, calerr ); 
		cal_retry_flag=1;
		if(retry_cnt < 5){	  
			retry_cnt++;
			dbg1("[CAL_Retry] cal error retry count = %d\n",retry_cnt);
			goto retry;
		}
//		cal_retry_flag=0;			
		
		if(sensor_num_cal == 0)
		{
			scanlib_send_sc(SMSG_SC_BAD_PIXEL_FAIL);
        }
		else
       	{
        	scanlib_send_sc(SMSG_SC_BAD_PIXEL_FAIL_B);        		
       	}
        return calerr;
    }
    scan_enable_motor_motion(true);

    /* store everything (except LUTs!) to cache */
    cached_cal = getCalCache(pCCase);
    ASSERT(cached_cal);
    store_to_cal_cache( cached_cal, pCCase );

    scerr = scands_get_integer_with_default( "cal_debug_scans", &num32, 0 );
    XASSERT( scerr==SCANERR_NONE, scerr );
    
    /* To enable debug scan at end of cal, set the scan_defines to a nonzero value*/
    cal_run_debug_scans( pCCase, cached_cal, num32 );

    /* davep 18-Sep-2012 ; capture a bunch of scans  */
    if( num32 ) {
        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        cal_send_scan_id( CAL_CAP_DEBUG );
        calerr = cal_debug_scan( pCCase );

        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        cal_send_scan_id( CAL_CAP_DEBUG );
        calerr = cal_debug_scan( pCCase );

        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        cal_send_scan_id( CAL_CAP_DEBUG );
        calerr = cal_debug_scan( pCCase );

        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        cal_send_scan_id( CAL_CAP_DEBUG );
        calerr = cal_debug_scan( pCCase );
    }

    /* davep 17-Mar-2008 ; make sure motor enabled before we leave (?) */
    scan_enable_motor_motion(true);

    dbg1("\n====== calibration complete ======\n");

    return 0;
}

