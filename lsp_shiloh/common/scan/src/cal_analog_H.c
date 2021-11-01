/*
 * ============================================================================
 * Copyright (c) 2006-2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/*
 * \brief
 *
 * Here is how pwm calibration is carried out:
 *
 * 1) Set a white target at about 80% of full range
 *
 * 2) Set up a top and bottom PWM value limit, set current pwm to the top limit as start test value.
 *
 * 3) Use current pwm to perform a scan and find out whether the curve top exceeds the target.
 *    If it does, change top to current pwm value. If not, change  bottom to the test value.
 *
 * 4) If curve top is close enough to the target, we are done with test pwm as calibrated value.
 *    Otherwise, change test value to be the average of top and bottom limit and go back to step 3.
 *
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "scos.h" 

#include "list.h"           
#include "lassert.h"
#include "interrupt_api.h"
#include "intnums.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "regAddrs.h"
#include "io.h"

#include "scancore.h"
#include "scantypes.h"
#include "scanif.h"
#include "scantools.h"
#include "scandbg.h"
#include "scanhwerr.h"
#include "scan.h"
#include "scanlib.h"
#include "scanvars.h"
#include "scanpipe.h"
#include "scanmech.h"
#include "safetylock.h"
#include "icedma.h"
#include "pichw.h"
#include "pic.h"
#include "piehw.h"
#include "cal.h"
#include "scancmdq.h"
#include "calstats.h"
#include "scanimg.h"
#include "cisx.h"
#include "scansen.h"
#include "scantask.h"
#include "afe.h"
#include "scands.h"
#include "scancap.h"
#include "cal_common.h"
#include "calcaplist.h"

/**
 * Function name: findAnalogOffset
 *
 * \brief Calculates and applies the analog offset.
 *
 * Description: Analyzes the data acquired during a scan of the calibration
 * label and computes & applies the analog offsets.
 * 
 * The offset DAC in AFE  is applied on reversed signal, i.e. the dark signal
 * has maximum analog value, that means if we want to boost the final digital
 * signal up, we need to apply more NEGATIVE offset on the analog signal.  The
 * function assumes that WM8152 is working under unit gain!!!!!!. After the
 * analog offset is applied, if there is a change in the analog gain, like that
 * being changed in the analog gain correction performed after, the offset in
 * the AFE  needs to be divided by the gain value to matain same absolute offset
 * level in output signal.
 *  
 *
 * \author Eric Huang
 * \author David Poole
 *
 **/


static cal_err_t findAnalogOffset( int scanType, bool positive_going_video, 
                                    uint16_t **datas, uint32_t numPixels, int *offset)
{
    int offset_adjust;
    uint16_t min_value, curve_floor;
    uint16_t target;  
 
    /* offsets all tied together (3-sensors, 3 channels) so only use the red
     * value 
     */
    target = scanvar_get()->cal_black_analog_target[SV_COLOR_RED];
//    target = cal_get_config()->BLACK_TARGET + 250;//scanvar_get()->cal_black_analog_target;

    if( scanType == SCAN_CMODE_MONO )
    {
        min_value = findCurveFloor(datas[0] + (2*numPixels), numPixels, 16, 128);
        /*
            If the floor is too low, report the failure
        */
    }
    else
    {
        XASSERT( scanType==SCAN_CMODE_COLOR, scanType );

        // For each color plane, find the pixel having the lowest value. That pixel will
        // determine how much analog offset must be applied to bring the pixel's value down to
        // the target value.
        min_value = findCurveFloor(datas[0] + (2*numPixels), numPixels, 16, 128);
        curve_floor= findCurveFloor(datas[1] + (2*numPixels), numPixels, 16, 128);
        if(curve_floor < min_value)
            min_value=curve_floor;

        curve_floor= findCurveFloor(datas[2] + (2*numPixels), numPixels, 16, 128);
        if(curve_floor < min_value)
            min_value= curve_floor;

    }

    dbg1("%s: min_value=%d\n", __FUNCTION__, min_value);

    if(min_value < 1)
        return CALERR_ANALOG_OFFSET_BAD_FLOOR;

    offset_adjust = (int)target - (int)min_value;

   /* davep 17-Jun-2010 ; by subtracting a positive black level adjustment
    * here, we're handling negative going video (this is the behavior on
    * sensors with black voltage above white voltage)
    */

   if(positive_going_video ) {
        //Harris fixed start
        /* white pixel > black pixel */
        //if( offset_adjust < 0 ) {
        //    *offset = (*offset)-offset_adjust;
        //}
        //else {
        //    *offset = (*offset)+offset_adjust;
        //}
        *offset += offset_adjust;
        //Harris fixed end
    }
    else {
        //Harris fixed start
        /* white pixel < black pixel */
        //if( offset_adjust > 0 ) {
        //    *offset -= offset_adjust;
        //}
        //else {
        //    *offset += offset_adjust;
        //}
        *offset -= offset_adjust;
        //Harris fixed end
   }
   return 0;
}

/*
  The is calibration routine handle sensor that have single offset for all color channels.
*/ 

cal_err_t cal_calc_analog_offset( CalibrationCase *pCCase )
{
    scan_err_t scerr;
    cal_err_t calerr, close_calerr;
    uint16_t *datas[3];
    int offset;
    bool analog_offset_success;
    int floor_failure_count;
    uint32_t num_rows, pixels_per_row;
    uint32_t analog_offset_retry_max;
    SENSOR_CONF sensor_conf; 
    float default_gain;
    struct scan_afe_info_t *scan_afe_info;
    int num_colors, i, num_cis;
    uint8_t channel_array[3];

    dbg2( "%s\n", __FUNCTION__ );

    scansen_get_sensor_conf( pCCase->sensor_num,pCCase->cmode, pCCase->horiz_res, &sensor_conf );

    /* davep 12-Jun-2008 ; XXX temp debug ; removing some globals */
    ASSERT( pCCase->pixels_to_scan > 0 );

    /* try this many times to calculate an analog offset before we give up and
     * claim a hardware fault
     */
    scerr = scands_get_integer_with_default( "cal_analog_offset_retry", 
                                &analog_offset_retry_max, 5 );
    XASSERT( scerr==0, scerr );

    /*  Set no-offset. Notice both positive and negative signal use zero as no offset 
     */
    offset = 0;
   
    /* count how many times we loop calculating the offset; too many times, we'll give
     * up and call it a hardware failure (we only loop when the min output is computed as zero)
     */
    floor_failure_count = 0;

    analog_offset_success = false;

    /* davep 19-Dec-2012 ; move hardwired DEFAULT_GAIN to afe_info structure */
    default_gain = 1.0;
    afe_get_afe_info( &scan_afe_info );

    if (scan_afe_info->version > 0)
    {
        default_gain = scan_afe_info->gain_value_for_offset_cal;
    }
    else if( scan_afe_info->analog_cal_default_gain_not_null ) {
        default_gain = scan_afe_info->analog_cal_default_gain;
    }

    /* davep 16-Jul-2013 ; check for up-to-date structure */
    ASSERT( FLOAT_TO_DECIMAL(default_gain) > 0 );

    while( !analog_offset_success ) {
        calerr = cal_scan_lines_open(SCAN_TARGET_TYPE_STATIONARY_CAL); 
        if( calerr != 0 ) { 
            dbg2( "%s scan open failed calerr=%d\n", __FUNCTION__, calerr );
            return calerr; 
        }

        scanimg_set_analog_offset( pCCase->sensor_num, offset, offset, offset);

        cal_send_uint32_metadata( 4,
                CAL_METADATA_CAL_CAP, CAL_CAP_ANALOG_OFFSET,
                CAL_METADATA_RED_ANALOG_OFFSET, offset,
                CAL_METADATA_GREEN_ANALOG_OFFSET, offset,
                CAL_METADATA_BLUE_ANALOG_OFFSET, offset );

        pCCase->red_gain = default_gain;
        pCCase->green_gain = default_gain;
        pCCase->blue_gain = default_gain;
        scanimg_set_analog_gain(pCCase->sensor_num, FLOAT_TO_DECIMAL(pCCase->red_gain), 
                                FLOAT_TO_DECIMAL(pCCase->green_gain), 
                                FLOAT_TO_DECIMAL(pCCase->blue_gain));

        cal_send_uint32_metadata( 4,
                CAL_METADATA_CAL_CAP, CAL_CAP_ANALOG_GAIN,
                CAL_METADATA_RED_ANALOG_GAIN, FLOAT_TO_DECIMAL(pCCase->red_gain),
                CAL_METADATA_GREEN_ANALOG_GAIN, FLOAT_TO_DECIMAL(pCCase->green_gain),
                CAL_METADATA_BLUE_ANALOG_GAIN, FLOAT_TO_DECIMAL(pCCase->blue_gain));

        /* Turn off illumination - don't want any light from our led or another sensor's leaking over */
        turn_off_all_leds();

        calerr = cal_scan_lines_run(); 
        
        if( calerr != 0 ) {
            /* on error, cal_scan_lines_run() will clean up */
            dbg2( "%s scan run failed calerr=%d\n", __FUNCTION__, calerr );
            return calerr;
        }

        num_colors = 3;
        if(pCCase->cmode == SCAN_CMODE_MONO) {
            num_colors = 1;
        }
        for (i=0;i<num_colors;i++)
        {
            // code to calculate array for staggered sensors would go here
            num_cis = PIC_WDMA_NUM_CHANNELS/3;
            // sensor 0 gets channels 0, 2, 4.  sensor 1 gets 1, 3, 5
            channel_array[i] = num_cis*i + pCCase->sensor_num;
        }
        // get all the data for all sensors - only pull out the sensor data we want
        ice_getScanData(pCCase->cmode, datas, &num_rows, &pixels_per_row, channel_array);

        //Find offsets
        calerr = findAnalogOffset(pCCase->cmode, 
                                  sensor_conf.positive_going_video, 
                                  datas, 
                                  pCCase->pixels_to_scan, 
                                  &offset);

        /* close now in case the findAnalogOffset() failed */
        close_calerr = cal_scan_lines_close();
        if( close_calerr != 0 ) {
            /* cal_scan_lines_close() logs error */
            return close_calerr;
        }

        if( calerr==CALERR_ANALOG_OFFSET_BAD_FLOOR ) {
            /* signal is too low so we will keep iterating until the calculated offset
                raises us up off the floor */

            floor_failure_count += 1;

            if(sensor_conf.positive_going_video)
            {
                //Force to raise up the offset to get away from dark
                 offset+=1000;
            }else
            {
                //Force to decrease offset to get away from dark
                 offset-=1000;
            }

            if( floor_failure_count >= analog_offset_retry_max ) {
                dbg1("%s have analog offset floor failures=%d! analog offset failed\n", 
                        __FUNCTION__, floor_failure_count );
				scanlib_send_sc(SMSG_SC_AFE_OFFSET_FAIL);   
                return CALERR_ANALOG_OFFSET_BAD_FLOOR;
            }
        }
        else { 
            /* success! */
            analog_offset_success = true;

            pCCase->red_offset = offset;
            pCCase->green_offset = offset;
            pCCase->blue_offset = offset;

            dbg2("%s Found offset: %d \n", __FUNCTION__, offset);

            cal_send_uint32_metadata( 3,
                    CAL_METADATA_RED_ANALOG_OFFSET, pCCase->red_offset,
                    CAL_METADATA_GREEN_ANALOG_OFFSET, pCCase->green_offset,
                    CAL_METADATA_BLUE_ANALOG_OFFSET, pCCase->blue_offset );

            /* davep 26-Oct-2006 ; capture another dark scan to see the results
             * of the analog offset
             */
            /* davep 25-Sep-2012 ; adding the calcaplist to control debug scans
             * at run-time
             */
            if( cal_cap_list_contains( CAL_CAP_AFTER_ANALOG_OFFSET ) ) {
                cal_send_scan_id( CAL_CAP_AFTER_ANALOG_OFFSET );
                calerr = cal_debug_scan( pCCase );
                if( calerr != 0 ) {
                    scanlog(0,  "%s cal_debug_scan failed calerr=%d\n", __FUNCTION__, calerr );
                    return calerr;
                }
            }

            /* turn on the LEDs for subsequent debug scans */
            pCCase->red_enable = pCCase->green_enable = pCCase->blue_enable = true;
        }
    }

    return 0;
}


/**
 * \brief  Determine if the sample pixels are close enough to our target pixels. 
 *
 * Used to decide if we can stop searching for an LED calibration value.
 *
 * \author David Poole
 * \date 22-May-2008
 *
 */
static bool led_adjust_close_enough( int num_colors, uint16_t target[3], uint16_t curve_max[3] )
{
    scan_err_t scerr;
    int i;
    bool close_enough;
    uint16_t delta;
    uint32_t led_adjust_pixel_target_threshold;

    close_enough = true;

    /* If the curve ceiling is within this value, we'll consider our LED adjustment
     * calculation good enough and quit.
     *
     * 3000 is roughly 5% of 65536, the full scale pixel range. This is a bit
     * of a guess at the moment and could probably stand to be tweaked.
     */
    scerr = scands_get_integer_with_default( "cal_led_adjust_pixel_target", 
                                &led_adjust_pixel_target_threshold, 3000 );
    XASSERT( scerr==0, scerr );

    /* look for curves out of range */
    for( i=0 ; i<num_colors ; i++ )  {

        /* using unsigned ints so be careful about wrapping */
        if( target[i] > curve_max[i] ) {
            delta = target[i] - curve_max[i];
        }
        else {
            delta = curve_max[i] - target[i];
        }

        /* davep 22-May-2008 ; XXX temp debug */
//        dbg2( "%s color=%d max=%d target=%d delta=%d threshold=%d\n", 
//                    __FUNCTION__, i, curve_max[i], target[i], delta,
//                    LED_EXP_PIXEL_TARGET_THRESHOLD );

        if( delta > led_adjust_pixel_target_threshold ) {
            close_enough = false;
            /* too bad, so sad, thanks for playing */
            break;
        }
    }

    return close_enough;
}

static float convert_to_float(uint32_t fixed_int_val)
{
    // pass in a number like 1345, and it will be converted to
    // a float value of 13.45, without floating point division
    int  whole, rem, tenths, hundredths;

    whole = fixed_int_val / 100;
    rem = fixed_int_val - (whole*100);
    tenths = rem/10;
    hundredths = rem - (tenths*10);
    return ((float) whole + (float)(tenths)*.1 + (float)(hundredths)*.01);

}

/*
  This function calibrates both gain and led adjustment (was only pwm formerly,
  hence the function name but because the pwm can cause noise problems,
  it is useful to calibrate exposure instead). Note that for certain weird
  sensors, the 3 gain/offset control are actually not for different colors,
  rather they are for the 3 segment of a single video channel. So we have to
  use the same gain/offset for all three settings, and the only color-dependent
  analog control is the LED adjustment (pwm or exposure/gating)
*/
#define MAX_VALID_SCAN_DATA 65530
cal_err_t cal_calc_pwm_gain(CalibrationCase *pCCase )
{
    cal_err_t calerr, close_calerr;
    const struct scanvars *sv;
    uint16_t *datas[3];
    uint16_t curve_max[3], target[3];
    uint32_t test_adj[3], top_adj[3], bottom_adj[3];
    int i, num_colors, loop_count;
    uint32_t num_rows, pixels_per_row;
    float current_gain;
    uint32_t decimal_gain;
    uint32_t new_gain_factor;
    struct scan_afe_info_t *scan_afe_info;
    bool first_pass = true;
    uint32_t curve_sum, average_val, average_target;
    int num_cis;
    uint8_t channel_array[3];
    const CAL_CONFIG *cal_config = cal_get_config();
    int percentage_change;
    int32_t signed_adj;
    bool sensor_maxed_out = false; // sensor pegged
    
    dbg2("%s\n", __FUNCTION__ );

    sv = scanvar_get();

    for( i=0 ; i<3 ; i++ ) {
        target[i] = sv->cal_white_analog_target[i];
    }

    //Set the adj for all channels to a default value for 1st time analog gain calc
    top_adj[0] = 100; /* 100.00% */
    top_adj[1] = 100;
    top_adj[2] = 100;

    for( i=0 ; i<3 ; i++ ) {
//        test_adj[i] = top_adj[i] - 20; // give ourselves 20% headroom (80%)
        test_adj[i] = top_adj[i] - 30; // give ourselves 30% headroom (70%)
        bottom_adj[i] = 1;  /* 1.00% */
    }

    num_colors = 3;
    if(pCCase->cmode == SCAN_CMODE_MONO) {
        num_colors = 1;
    }

    loop_count = 0;

    /* davep 19-Dec-2012 ; move hardwired DEFAULT_GAIN to afe_info structure */
    current_gain = 1.0;
    afe_get_afe_info( &scan_afe_info );
    if(scan_afe_info->analog_cal_default_gain_not_null ) {
        current_gain = scan_afe_info->analog_cal_default_gain;
    }
    decimal_gain = FLOAT_TO_DECIMAL(current_gain);
    
    while( 1 ) {

        dbg2("%s try red_adj=%d green_adj=%d blue_adj=%d, s#=%d, gain=%d\n", 
                    __FUNCTION__, 
                    test_adj[0], 
                    test_adj[1], 
                    test_adj[2], pCCase->sensor_num,decimal_gain);

        calerr = cal_scan_lines_open(SCAN_TARGET_TYPE_STATIONARY_CAL); 
        if( calerr != 0 ) { 
            dbg2( "%s scan open failed calerr=%d\n", __FUNCTION__, calerr );
            return calerr; 
        }

        // make sure the lights are on to scan
        /* Turn off illumination - don't want any light from another sensor's leaking over */
        turn_off_all_leds();
        
        scanimg_set_led_enable(pCCase->sensor_num, true, true, true);
        scanimg_set_analog_offset(pCCase->sensor_num, pCCase->red_offset, pCCase->green_offset, pCCase->blue_offset);
        scanimg_set_analog_gain(pCCase->sensor_num, decimal_gain, decimal_gain, decimal_gain);
#ifndef HAVE_SCANLEDEXP_CAL
        scanimg_set_led_duty(pCCase->sensor_num, test_adj[0], test_adj[1], test_adj[2] );
        scanimg_set_led_exposure(pCCase->sensor_num, 100, 100, 100);   // fix at 100%
        cal_send_uint32_metadata( 4,
                CAL_METADATA_CAL_CAP, CAL_CAP_LEDS,
                CAL_METADATA_RED_PWM, test_adj[0],
                CAL_METADATA_GREEN_PWM, test_adj[1],
                CAL_METADATA_BLUE_PWM, test_adj[2] );
#else        
        scanimg_set_led_exposure(pCCase->sensor_num, test_adj[0], test_adj[1], test_adj[2]);
        scanimg_set_led_duty(pCCase->sensor_num, 100, 100, 100);   // set to 100% to reduce noise
        cal_send_uint32_metadata( 4,
                CAL_METADATA_CAL_CAP, CAL_CAP_LEDS,
                CAL_METADATA_RED_EXP, test_adj[0],
                CAL_METADATA_GREEN_EXP, test_adj[1],
                CAL_METADATA_BLUE_EXP, test_adj[2] );
#endif        

        cal_send_uint32_metadata( 4,
                CAL_METADATA_CAL_CAP, CAL_CAP_ANALOG_GAIN,
                CAL_METADATA_RED_ANALOG_GAIN, decimal_gain,
                CAL_METADATA_GREEN_ANALOG_GAIN, decimal_gain,
                CAL_METADATA_BLUE_ANALOG_GAIN, decimal_gain);

        calerr = cal_scan_lines_run();
        
        if( calerr != 0 ) {
            /* on error, cal_scan_lines_run() will clean up */
            dbg2( "%s scan run failed calerr=%d\n", __FUNCTION__, calerr );
            return calerr; 
        }

        for (i=0;i<num_colors;i++)
        {
            // code to calculate array for staggered sensors would go here
            num_cis = PIC_WDMA_NUM_CHANNELS/3;
            // sensor 0 gets channels 0, 2, 4.  sensor 1 gets 1, 3, 5
            channel_array[i] = num_cis*i + pCCase->sensor_num;
        }

        // get all the data for the sensor
        ice_getScanData(pCCase->cmode, datas, &num_rows, &pixels_per_row,channel_array );
        average_target = curve_sum = 0;
        for( i=0 ; i<num_colors ; i++ ) {
            //4, 64 are determined experimentally, maybe not the best
            curve_max[i] = findCurveCeiling( datas[i], pCCase->pixels_to_scan, 4, 64); 

            // first scan is a special case - see lower down in the function
            if (first_pass)
            {
                curve_sum += curve_max[i];
                average_target += target[i];
                if (curve_max[i] > MAX_VALID_SCAN_DATA)
                    sensor_maxed_out = true;
            }
            else
            {
                dbg2("%s top=%d bottom=%d max=%d target=%d adj=%d gain=%d\n", __FUNCTION__, 
                     top_adj[i], bottom_adj[i], 
                     curve_max[i], target[i], 
                     test_adj[i],decimal_gain );
                
                if( target[i] < curve_max[i] ) {
                    top_adj[i] = test_adj[i];
                }
                else {
                    bottom_adj[i] = test_adj[i];
                }
            }
        }

        close_calerr = cal_scan_lines_close();
        if( close_calerr != 0 ) {
            /* cal_scan_lines_close() logs error */
            return close_calerr;
        }

        /* davep 22-May-2008 ; stop when we're close enough,          */
        if( led_adjust_close_enough( num_colors, target, curve_max ) ) {
            dbg2( "%s sensor_num=%d, led adjust close enough so leaving search with loop_count=%d\n", 
                  __FUNCTION__, pCCase->sensor_num,loop_count );
            break;
        }

        // first scan, calculate what the gain should be, set that value. After, adjust LEDs to fine tune.
        if (first_pass)
        {
            // average the curve maxes and calculate what the gain should be
            average_val = curve_sum/num_colors;
            average_target = average_target/num_colors;
            if (sensor_maxed_out)
            {
                sensor_maxed_out = false;
                if (decimal_gain > 100)
                {
                    // the sensor is overwhelmed, reduce the gain, and repeat the first pass
                    // reduce by 25% or to 1.0, whichever is the larger gain value
                    decimal_gain = decimal_gain - (decimal_gain * 25)/100;
                    if (decimal_gain < 100)
                        decimal_gain = 100;
                    dbg2("Sensor %d pegged, dropping gain down to %d\n",pCCase->sensor_num,decimal_gain);
                }
                else
                {
                    // gain is already low enough, let's reduce the led adjustment
                    for (i=0;i<num_colors;i++)
                    {
                        if (curve_max[i] > MAX_VALID_SCAN_DATA)
                        {
                            dbg2("Sensor %d pegged, curve_max[%d]=%d, subtracting 10\n",pCCase->sensor_num,i,curve_max[i]);
                            test_adj[i] -=10;
                            if (test_adj[i] == 0){
        												scanlib_send_sc(SMSG_SC_AFE_GAIN_FAIL); 
                                ASSERT("LED %d just turned completely off - something is horribly wrong\n");
        										}
                            // since gain won't change anymore, update our top and bottom limits
                            if( target[i] < curve_max[i] ) {
                                top_adj[i] = test_adj[i];
                            }
                            else {
                                bottom_adj[i] = test_adj[i];
                            }                            
                        }
                    }
                }
                // now we loop back up and do another scan with new gain or led test_adj value
            }
            else
            {
                // did a first scan, now compute an average gain before tuning led adjustment
                first_pass = false;
                new_gain_factor = (average_target * 100)/average_val; // multiple by 100 for 2 digit fixed point
                decimal_gain = (new_gain_factor * decimal_gain)/ 100; // removing the extra 2 digit fixed point
                // quick clamp - don't allow a gain > gain cap (usually around 8,
                // something's probably wrong in that case....)
                if (decimal_gain > cal_config->ANALOG_GAIN_CAP_TIMES_10*10)
                    decimal_gain = cal_config->ANALOG_GAIN_CAP_TIMES_10*10;
            }
            loop_count++;
        }
        else
        {
            /* emergency escape hatch */
            if( loop_count++ >= 12 ) {
                dbg1("loop_count emergency escape hatch loop_count=%d\n",loop_count);
                break;
            }
            
            
            //Need another round
            for( i=0 ; i<num_colors ; i++ ) {
                percentage_change = ((target[i] - curve_max[i]) * 100)/curve_max[i];
                if (percentage_change > 0)
                    percentage_change += 1; //round up
                else
                    percentage_change -= 1; // round down
                
                signed_adj = (int32_t) test_adj[i];
                test_adj[i] = (uint32_t) (signed_adj + ((percentage_change * signed_adj)/ 100));
                if (test_adj[i] <= bottom_adj[i])
                    test_adj[i] = bottom_adj[i];
                else if (test_adj[i] >= top_adj[i])
                    test_adj[i] = top_adj[i];
                dbg2( "%s %d top=%d bottom=%d test=%d, loop=%d, s#=%d, percent_chg=%d\n", __FUNCTION__, __LINE__,
                      top_adj[i], bottom_adj[i], test_adj[i],loop_count, pCCase->sensor_num, percentage_change );
            }
        }
        if(pCCase->cmode == SCAN_CMODE_MONO ) {
            test_adj[1] = test_adj[0]; 
            test_adj[2] = test_adj[0]; 
        }
    }

/**********************	Strip Dump	**********************/
	for( i=0 ; i<num_colors ; i++ )
	{
	    struct file *filp, *filp1;
	    cal_pixel_t *pData;
	    int row;
	    int pixel;
	    	    
	    /* Kernel memory access setting */
	    mm_segment_t old_fs = get_fs();
	    
	    set_fs(get_ds());
	 
	 	/* Check Dump or not */
	 	filp = filp_open("/data/strip_dump", O_RDWR, 0);
	 	
	 	/* Skip Dump */
	 	if (IS_ERR(filp)) {
			;
	    }
	    /* Do Dump */
	    else{
	    	filp_close(filp, NULL);
	    	/* Open Dump File */
		    if(pCCase->sensor_num == 0)
		    {
			  	if(num_rows == 30){					// 300dpi
					if( num_colors==1 ) {	// M
		            	filp1 = filp_open("/data/gain_f_m_300(2592x30).raw", O_CREAT|O_RDWR, 0);
		        	}
			        else {
						if(i == 0)			// R
				    		filp1 = filp_open("/data/gain_f_r_300(2592x30).raw", O_CREAT|O_RDWR, 0);
						else if(i == 1)		// G
				    		filp1 = filp_open("/data/gain_f_g_300(2592x30).raw", O_CREAT|O_RDWR, 0);
				    	else				// B
				    		filp1 = filp_open("/data/gain_f_b_300(2592x30).raw", O_CREAT|O_RDWR, 0);
			        }
			    }
			    else{								// 600dpi
					if( num_colors==1 ) {	// M
		            	filp1 = filp_open("/data/gain_f_m_600(5168x60).raw", O_CREAT|O_RDWR, 0);
		        	}
			        else {
						if(i == 0)			// R
				    		filp1 = filp_open("/data/gain_f_r_600(5168x60).raw", O_CREAT|O_RDWR, 0);
						else if(i == 1)		// G
				    		filp1 = filp_open("/data/gain_f_g_600(5168x60).raw", O_CREAT|O_RDWR, 0);
				    	else				// B
				    		filp1 = filp_open("/data/gain_f_b_600(5168x60).raw", O_CREAT|O_RDWR, 0);
			        }
				}
			}
			else
			{
		    	if(num_rows == 30){					// 300dpi
					if( num_colors==1 ) {	// M
		            	filp1 = filp_open("/data/gain_b_m_300(2592x30).raw", O_CREAT|O_RDWR, 0);
		        	}
			        else {
						if(i == 0)			// R
				    		filp1 = filp_open("/data/gain_b_r_300(2592x30).raw", O_CREAT|O_RDWR, 0);
						else if(i == 1)		// G
				    		filp1 = filp_open("/data/gain_b_g_300(2592x30).raw", O_CREAT|O_RDWR, 0);
				    	else				// B
				    		filp1 = filp_open("/data/gain_b_b_300(2592x30).raw", O_CREAT|O_RDWR, 0);
			        }
			    }
			    else{								// 600dpi
					if( num_colors==1 ) {	// M
		            	filp1 = filp_open("/data/gain_b_m_600(5168x60).raw", O_CREAT|O_RDWR, 0);
		        	}
			        else {
						if(i == 0)			// R
				    		filp1 = filp_open("/data/gain_b_r_600(5168x60).raw", O_CREAT|O_RDWR, 0);
						else if(i == 1)		// G
				    		filp1 = filp_open("/data/gain_b_g_600(5168x60).raw", O_CREAT|O_RDWR, 0);
				    	else				// B
				    		filp1 = filp_open("/data/gain_b_b_600(5168x60).raw", O_CREAT|O_RDWR, 0);
			        }
				}
			}
			/* Dump Strip Image */
		    if (IS_ERR(filp1)) {
		        ;
		    }
		    else {
		    	dbg1( "[CAL_DUMP] Gain/PWM Cal Dump Start(cis:%d, cmode:%d, ch:%d, ppr:%d, bytes:%d)\n", pCCase->sensor_num, num_colors, i,
		    								pixels_per_row, sizeof(cal_pixel_t) * pixels_per_row * num_rows);
		    	pData = datas[i];

				vfs_write(filp1, pData, sizeof(cal_pixel_t) * pixels_per_row * num_rows, &filp1->f_pos);
				
				// 2nd line dump (1st line : sorted)
				for(pixel=pixels_per_row ; pixel<2*pixels_per_row ;pixel++){
					if(pixel == pixels_per_row)
						dbg1( "[CAL_DUMP] 1st Line Dump\n");
					if(pixel%20 == 0)	// display at every 20 pixel
						dbg1( "%ld\n", pData[pixel] >> 8);	// only MSB
				}
					
				filp_close(filp1, NULL);
		    }
	    }
		/* Restore Kernel Memory Setting */
		set_fs(old_fs);
	}
/*********************************************************/

    // we may have changed the decimal_gain, so update the float current_gain
    current_gain = convert_to_float(decimal_gain);
    pCCase->red_gain = current_gain;
    pCCase->green_gain = current_gain;
    pCCase->blue_gain = current_gain;
    dbg2("current gain of (converted) %d just stored into calcalse\n",FLOAT_TO_DECIMAL(current_gain));

#ifndef HAVE_SCANLEDEXP_CAL            
    pCCase->red_pwm =  test_adj[0];
    pCCase->green_pwm = test_adj[1];
    pCCase->blue_pwm =  test_adj[2];
        
    pCCase->red_exp = 100;
    pCCase->green_exp = 100;
    pCCase->blue_exp = 100;
    dbg1("%s EXIT: final pwm red=%d green=%d blue=%d, s#=%d\n", __FUNCTION__, 
         pCCase->red_pwm, pCCase->green_pwm, pCCase->blue_pwm, pCCase->sensor_num);    
#else    
    pCCase->red_pwm =  100;
    pCCase->green_pwm = 100;
    pCCase->blue_pwm =  100;
        
    pCCase->red_exp = test_adj[0];
    pCCase->green_exp = test_adj[1];
    pCCase->blue_exp = test_adj[2];
    dbg1("%s EXIT: final exp red=%d green=%d blue=%d, s#=%d\n", __FUNCTION__, 
         pCCase->red_exp, pCCase->green_exp, pCCase->blue_exp, pCCase->sensor_num);
#endif
    
    return 0;
}

