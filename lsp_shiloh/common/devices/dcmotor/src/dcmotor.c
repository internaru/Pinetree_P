/*
 * ======================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

/// \file dcmotor.c

#include "posix_ostools.h"
#include "uio_lib_api.h"
#include "ATypes.h"
#include "logger.h"
#include "lassert.h"
#include "delay_api.h"
#include "intnums.h"
#include "debug.h"
#include "stdlib.h"
#include "memAPI.h"
#include "hwconfig_api.h"
#include "dcmothw.h"
#include "dec_adc_api.h"
#include "dcmotor.h"
#ifdef HAVE_DEC_LASER
#include "dec_laser.h"
#endif
#include "dcmotor_priv.h"
#include "mtr6pin.h"        /* Macros/declares to support DCMOTOR_BASE2 motor block */

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
static int dcmotor_dbcallback( int argc, char *argv[] );
#endif
/* The following globals are used for serial debugging commands, but leave them in
 * even in release builds (don't want ifdefs all through the servo code).
 */
static BOOL       pid_debug = FALSE;
static UINT32     debug_kp;
static UINT32     debug_ki;
static UINT32     debug_kd;

static BOOL       debug_disable_safety = FALSE;
static BOOL       debug_disable_servo  = FALSE;
//static UINT32     debug_mtr_duty = 0;
static BOOL       debug_disable_pwm_wdog = FALSE;

static dec_adc_t  *adc_24v;
static uint32_t   basic_line_24v;

/* The ISR duration logs become meaningless with so many entries, we are usually
 * looking for just the longer durations.  The following define will set a threshold
 * for when the ISR duration gets logged (in usec).
 */
#define ISR_DUR_LOG_THRESH    5

/**
 *  \brief (Helper) Set encoder input debounce
 *
 *  Helper function to set the encoder input debounce for a specific move speed
 *  (each move speed has a unique encoder frequency).
 *
 *  Note that the encoder debounce is applied to the raw EncA and EncB input signals,
 *  before they are converted into quad counts. Also note that we have to decounce 
 *  each raw edge transition (twice per period).
 *
 *  The following picture shows the time period (dt) that needs to be calculated in
 *  order to get a valid debounce time:
 *
 *            | dt  |
 *            .     . 
 *            +-----+     +-----+
 *  EncA      |     |     |     +
 *         ---+     +-----+     +-----
 *            .     .
 *            .  +-----+     +-----+
 *  EncB      .  |  .  |     |     +
 *          -----+  .  +-----+     +--
 *            .     .
 *            +--+  +--+  +--+  +--+
 *  Quads     |  |  |  |  |  |  |  |
 *          --+  +--+  +--+  +--+  +--
 *            1  2  3  4  5  6  7  8
 *
 *  The quad count increments on each quad edge!
 *
 *  \param[in] motor_id  Valid Motor ID
 *
 **/
static void mot_set_enc_debounce(dcmotor_t * dcmotor)
{
    uint32_t block_freq = hwGetBusSpeed() * ONE_MHZ;
    uint32_t dbCount;
    uint32_t quadsPerSecond;
    uint32_t quadFreq;
    float  encoderPulseWidth;
    float  debounceTime;

    /* Motor speeds are given in scaled quads per irq, so the first step is to get
     * that into quads per second (not exactly encoder frequency, see comments below).
     */
    quadsPerSecond = dcmotor->move_params->speed * (1000 /dcmotor->mtr_connects->sample_period);
    quadsPerSecond = quadsPerSecond >> dcmotor->fbits;

    /* Quads per second is almost a frequency, but quads get counted on every edge
     * of EncA xor EncB.  So two quad counts works out to one quad 'period'. Look
     * at the picture above ...
     */
    quadFreq = quadsPerSecond >> 1;     // divide by 2

    /* One quadrature period works out to be half of one raw encoder period (dt in
     * the picture above), which is the time we are after.
     */
    encoderPulseWidth = 1.0 / quadFreq; 

    /* Calculate the desired debounce time */
    debounceTime = encoderPulseWidth * (ENC_DB_PERCENT / 100.0);

    /* The encoder debounce is clocked by the scan system clock, so figure out
     * how many clock ticks we will need to program.
     *   dbCount = debounceTime / clock period;
     */
    dbCount = (uint32_t)(debounceTime * block_freq);

//-jrs    if ( !IN_INTERRUPT() )
    {
      dbg2(("%s: scaled speed=%d fbits=%d qps=%d qfreq=%d dbcount=%d blockf=%d\n", 
           __FUNCTION__, 
           dcmotor->move_params->speed, 
           dcmotor->fbits,
           quadsPerSecond, quadFreq,
           dbCount, block_freq ));
    }

    /* Set the debounce values in hardware (not using zero pos). Note: the mtr6pin
     * layer will protect against out of range debounce counts.
     */
    set_mtr6pin_debounce_zdb( dcmotor->motor_regs, 0 );
    set_mtr6pin_debounce_edb( dcmotor->motor_regs, dbCount );
}

/**
 *  \brief (Helper) Validate motor ptr
 *
 *  Helper function to validate the passed in motor id.
 *
 *  \param[in] dcmotor  Motor Ptr
 *
 *  \return BOOL
 *  \retval TRUE   Motor ID is valid
 *  \retval FALSE  Motor ID is not valid
 **/
static BOOL valid_dcmotor_ptr(dcmotor_t * dcmotor)
{
    /* WARNING: this routine can be called from interrupt context. I am doing the
     *          serial debug only because it will be followed by an assert.
     */

    /* Check for a valid motor number */
    if (dcmotor == NULL)
    {
        dbg1(("Invalid motor ID - out of range\n"));
        ASSERT(0);
        return FALSE;
    }

    if (dcmotor->cookie != DCMOTOR_COOKIE)
    {
        dbg1(("Invalid motor cookie - out of range\n"));
        ASSERT(0);
        return FALSE;
    }
    /* Saninty check: null register pointer? */
    if (dcmotor->motor_regs == NULL)
    {
        dbg1(("ERROR: motor registers are not set\n"));
        ASSERT(0);
    }

    /* If we made it here ... the id is OK */
    return TRUE;
}
void smot_add_trigger(dcmotor_t *          dcmotor,
                      motor_trigger_cond_t type,
                      uint32_t               param1,
                      TriggerCbFuncPtr     function_ptr)
{
    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    /* At least make sure we have a valid function pointer */
    if ( (type != TRIG_NULL) && (function_ptr == NULL) )
    {
        dbg1(( "%s: Invalid trigger callback function\n", __FUNCTION__));
        ASSERT(0);
    }

    /* Right now we only support ONE trigger at a time, and that will be a one shot */
    dcmotor->mtr_trigger.trigger_type   = type;
    dcmotor->mtr_trigger.trigger_param1 = param1;
    dcmotor->mtr_trigger.trigger_function_ptr = function_ptr;
}
/**
 *  \brief (Helper) Set a new motor state
 *
 *  Helper function to set a new motor state.  Checks to see if a trigger
 *  is registered for this state transition, if so the call the registered
 *  callback function.
 *
 *  \param[in]  dcmotor  Motor Ptr
 *  \param[in]  new_state New motor state
 **/
static void mot_set_motor_state(dcmotor_t * dcmotor, MotorState_t new_state)
{
    dcmotor->motor_state = new_state;

    if (dcmotor->mtr_trigger.trigger_type == TRIG_STATE)
    {
        if (dcmotor->mtr_trigger.trigger_param1 == new_state)
        {
            /* Call the function (always one shot, so clear it first) */
            dcmotor->mtr_trigger.trigger_type = TRIG_NULL;
            dcmotor->mtr_trigger.trigger_function_ptr(dcmotor);
        }
        else if (new_state == MOT_STATE_STOP)
        {
            /* Always clean up state striggers when we stop a move */
            dcmotor->mtr_trigger.trigger_type = TRIG_NULL;
        }
    }
}

/**
 *  \brief (Helper) Calculate max allowed PWM
 *
 *  Helper function to calculate the max motor pwm according to current velocity
 *  and pwm parameters for the motor
 *
 *  \param[in] dcmotor  Motor Ptr
 *  \param[in] curVel    Current motor velocity
 */
static int32_t calc_maxPwm(dcmotor_t * dcmotor, int32_t curVel)
{
    int32_t maxPwm;
    const motor_param_t *mp = dcmotor->move_params;

    if ( curVel >= mp->velTreshHigh ) {
        /* Velocity is above high threshold: allow max PWM for this motor mode to
         * maximize torque and avoid trouble
         */
        maxPwm = mp->maxPwmHigh;
    }
    else if ( curVel < mp->velTreshLow ) {
        /* Velocity is below low threshold (so we are either ramping or about to
         * stall): so use min PWM for this motor mode to prevent over current
         */
        maxPwm = mp->maxPwmLow;
    }
    else {
        /* We are in between the velocity thresholds: use linear equation to set PWM
         * in between high/low limits based on velocity
         */
        maxPwm = mp->maxPwmLow + ((mp->maxPwmHigh - mp->maxPwmLow) * 
                 (curVel - mp->velTreshLow) / (mp->velTreshHigh - mp->velTreshLow));
    }

    /* Make sure the PWM limit never drops below zero */
    if (maxPwm < 0) {
        maxPwm = 0;
    }

    return maxPwm;
}

/**
 *  \brief (Helper) Convert duty cycle to phase based cycle
 *
 *  The 'normal' algorithm uses the motor controller as follows:
 *   - pwm0 signal into PWM to control speed (0 to pwm_period)
 *   - direction bit into PHASE to control direction
 *
 *  The phase approach uses:
 *   - PWM is a 0/1 on/off switch
 *   - pmw0 signal is fed into the PHASE input to control both speed and
 *     direction. 50% duty cycle is stop, 100% is full in one direction, 0
 *     is full in the opposite direction.
 *
 *  \notes
 *  This routine requires a duty value between 0 and the maximum PWM period. 
 *
 *  \param[in]  motor_id     Motor ID
 *  \param[in]  reg_duty_val Duty value (must be 0 to PWM period)
 **/
static uint32_t mot_duty_to_phase( dcmotor_t * dcmotor, uint32_t reg_duty_val )
{
    uint32_t pwm_period = dcmotor->pwm_period;
    int32_t phase_pwm;

    /* Period must be set, non-zero and in range. */
    ASSERT( (0 < pwm_period) & (pwm_period <= get_mtr6pin_pwm_period_max()));

    /* Do the conversion.  Note that if the motor direction was supposed to
     * be swapped, the invert bit in the motor control register should take
     * care of business as we apply the pwm0 signal.
     */
    if ( dcmotor->move_direction == MOT_FORWARD )
    {
        phase_pwm = (reg_duty_val + pwm_period) / 2;
        
        if (phase_pwm < (pwm_period / 2))
        {
            phase_pwm = pwm_period / 2;
        }
        else if (phase_pwm > pwm_period)
        {
            phase_pwm = pwm_period;
        }
    }
    else
    {
        phase_pwm = (pwm_period - reg_duty_val) / 2;

        if (phase_pwm > (pwm_period / 2))
        {
            phase_pwm = pwm_period / 2;
        }
        else if (phase_pwm < 0)
        {
            phase_pwm = 0;
        }
    }

    return (uint32_t)phase_pwm;
}


/**
 *  \brief (Helper) Apply Servo PWM to motor
 *
 *  Used to apply servo control to the motor. This routine may leave a holding
 *  current on the motor when 0 PWM is commanded, so don't use this routine to
 *  stop the motor after a move (see mot_no_pwm()).  0 PWM may be called for 
 *  under normal servo conditions,  but this is not the 'stopped' use case.
 *
 *  \notes
 *  This routine requires a duty value between 0 and the maximum PWM period. 
 *
 *  \param[in]  dcmotor     Motor ptr
 *  \param[in]  reg_duty_val Duty value (must be 0 to PWM period)
 **/
static void mot_pwm_control( dcmotor_t * dcmotor, uint32_t reg_duty_val,bool update_duty_val )
{
    uint32_t period;
    uint32_t prev_period;
    int32_t delta_period;
    int32_t delta_v;
    int32_t diff_error = 0;

    /* Period must be set, non-zero and in range. */
    period = dcmotor->pwm_period;
    prev_period = dcmotor->prev_pwm_period;
#ifdef HAVE_DEC_LASER
    if (adc_24v && period > 0) 
    {
        uint32_t v_read = dec_adc_raw_read_channel(adc_24v);
               
        //DBG_LOG_ERR("V = %x %d\n",v_read,(dcmotor->pwm_period * v_read) / 0x3ff);
        //if (!dcmotor->laser) 
        //{
            //DBG_LOG_ERR("v = %d v_ref %d\n",v_read, basic_line_24v);
            if (v_read > basic_line_24v) 
            {
                basic_line_24v = v_read;
            }
            dcmotor->voltage_sum -= dcmotor->voltage[dcmotor->voltage_index];
            dcmotor->voltage_sum += v_read;
            dcmotor->voltage[dcmotor->voltage_index] = v_read;

            dcmotor->voltage_error_sum -= dcmotor->voltage_error[dcmotor->voltage_index];
            delta_v = dcmotor->voltage_error[dcmotor->voltage_index] = v_read - (dcmotor->voltage_sum/NUM_VOLTAGE_SUMS);
            diff_error = delta_v - dcmotor->voltage_error[dcmotor->voltage_index == 0 ? NUM_VOLTAGE_SUMS - 1: dcmotor->voltage_index - 1];
            dcmotor->voltage_error_sum += dcmotor->voltage_error[dcmotor->voltage_index];
            //DBG_LOG_ERR("V error sum = %d delta_v = %d diff_error = %d\n",dcmotor->voltage_error_sum,delta_v,diff_error);
            dcmotor->voltage_index++;
            if (dcmotor->voltage_index == NUM_VOLTAGE_SUMS) 
            {
                dcmotor->voltage_index = 0;
            }
            period =  (v_read >= basic_line_24v)? period :((period * (v_read )) /(basic_line_24v)) ;
            //if (period != 6600)
            if (dcmotor->motor_use == laser_motor)
            {
                DBG_LOG_DEBUG("p = %d\n",period);
            }
            DBG_LOG_DEBUG("p = %d v = %d v_ref %d\n",period, v_read, basic_line_24v);
            
        //}
        
    }
#endif
    ASSERT( (0 < period) & (period<=get_mtr6pin_pwm_period_max()));

    /* Keep the duty value in range, i.e. duty must be < period */
    delta_period = period -prev_period;
    /*update_duty_val = false;*/
#define MAX_DIFF_PERIOD 800
    if (update_duty_val && delta_period != 0  && dcmotor->pwm_period != 0) 
    {
        int32_t diff_period;
        //uint32_t oldRDV = reg_duty_val;
        //DBG_LOG_ERR("delta = %d\n",((delta_period * (int32_t)reg_duty_val) / ((int32_t)dcmotor->pwm_period)) );

        if (dcmotor->motor_use == laser_drive_motor) 
        {
             
            
            diff_period =  (0 *delta_v +/*(int32_t)(((delta_period * 10) * (int32_t)reg_duty_val) / ((int32_t)dcmotor->pwm_period))*/ + 0 * dcmotor->voltage_error_sum / 2 + 30 * diff_error) ;//compilers are stupid sometimes
            //DBG_LOG_ERR("V error sum = %d delta_v = %d diff_error = %d diff_period = %d\n",dcmotor->voltage_error_sum,delta_v,diff_error,diff_period);
            if (diff_period > 0 && diff_period > MAX_DIFF_PERIOD * 3) 
            {
                diff_period = MAX_DIFF_PERIOD * 3;
            }
            else if(diff_period < 0 && diff_period < -MAX_DIFF_PERIOD * 3)
            {
                diff_period = -MAX_DIFF_PERIOD* 3;
            }
            if (diff_period < 0 && reg_duty_val < -diff_period) 
            {
                //do nothing rolling sucks
            }
            else
            {
                reg_duty_val += diff_period;
            }
            //DBG_LOG_ERR("delta_v = %d diff_error = %d diff_period = %d PWM = %d\n",delta_v,diff_error,diff_period,reg_duty_val);
            //DBG_LOG_ERR("dcmotor->sum_pwm_period %d dcmotor->sum_pwm_period %d\n",dcmotor->sum_pwm_period,dcmotor->sum_pwm_period);
            //reg_duty_val += 6 * dcmotor->sum_pwm_period;
            
        }
        else
        {

            diff_period = (int32_t)(((delta_period ) * (int32_t)reg_duty_val) / ((int32_t)dcmotor->pwm_period))/4  ;//compilers are stupid sometimes
            if (diff_period > 0 && diff_period > MAX_DIFF_PERIOD/16) 
            {
                diff_period = MAX_DIFF_PERIOD/16;
            }
            else if(diff_period < 0 && diff_period < -MAX_DIFF_PERIOD/16)
            {
                diff_period = -MAX_DIFF_PERIOD/16;
            }
            if (diff_period > 0 && reg_duty_val < diff_period) 
            {
                //do nothing rolling sucks
            }
            else
            {
                reg_duty_val -= diff_period;
            }
        }
        //reg_duty_val += (int32_t)(((delta_period * 8) * (int32_t)reg_duty_val) / ((int32_t)dcmotor->pwm_period))  ;//compilers are stupid sometimes
        //if (dcmotor->laser)
        //{
            //DBG_LOG_ERR("old RDV %d new RDV %d delta %d\n",oldRDV,reg_duty_val, reg_duty_val - oldRDV);
        //}
            
    }
    if (reg_duty_val >= period)
    {
        reg_duty_val = period - 1;
    }
    ASSERT(reg_duty_val < period);

    /* If we are controlling the PWM with phase, then re-map the PWM value */
    
    if (dcmotor->pwm_method == DC_PWM_PHASE)
    {
       reg_duty_val = mot_duty_to_phase( dcmotor, reg_duty_val ); 
    }
    // DBG_LOG_ERR("%s duty = %d, period = %d\n", __func__, reg_duty_val, period);
    /* Set duty cycle */
    set_mtr6pin_pwm_duty( dcmotor->motor_regs, reg_duty_val );
    /* Set period */
    set_mtr6pin_pwm_period( dcmotor->motor_regs, period );
    dcmotor->prev_pwm_period = period;
}

/**
 *  \brief (Helper) Apply 0 PWM to motor
 *
 * This routine is used to make sure that no PWM is applied to the motor. Some servo
 * methods will actually apply a holding current at 0 PWM, this routine will disable
 * that to achieve no PWM.
 *
 *  \notes
 *   - This routine should not be used for doing servo control, rather to make sure
 *     we have no PWM
 *   - this routine does not disable the motor block
 *
 *  \param[in]  dcmotor     Motor Ptr
 **/
static void mot_no_pwm( dcmotor_t * dcmotor )
{
    DBG_INFO("%s: motor %d\n", __func__, dcmotor->motor_id);
    /* The first step is to apply 0 PWM as usual */
    mot_pwm_control( dcmotor, 0 ,false);

    /* Clear the motor enable.  Note that not all PWM methods use motor enable
     * (it will not be mapped in the connect table); the 6-pin driver will simply
     * ignore it if not mapped.
     */
    set_mtr6pin_en_state( dcmotor->motor_regs, 
                          dcmotor->mtr_connects,
                          0 );
}
/**
 *  \brief (Helper) Calculate the motor load rolling average
 *
 *  Helper function to calculate our rolling average. We consider the data set
 *  to become valid once it has a full compliment of data points loaded; after
 *  that the average rolls.
 *
 *  \param[in]  dcmotor  Motor Ptr
 *  \param[out] AvgLoad   Pointer to returned load value
 *
 *  \return int32_t
 *  \retval 0          Average calculated and
 *  returned in AvgLoad
 *  \retval -1  Average not calculated, -1
 *  returned in AvgLoad
 **/
#ifdef DO_OCC
static int32_t avg_ld_calc(dcmotor_t * dcmotor, int32_t *AvgLoad)
{
    /* Only return the average when we have a full deck */
    if (dcmotor->mtr_load.LdAvgValid == FALSE)
    {
        *AvgLoad = MTR_LD_BAD_AVE;
        return -1;
    }

    *AvgLoad = (dcmotor->mtr_load.LdTotal / LD_AVG_SIZE);

    return 0;
}
#endif

/**
 *  \brief (Helper) Add sample to motor load rolling average
 *
 *  Helper function to add a new data sample to our rolling average. We
 *  consider the data set to become valid once it has a full compliment
 *  of data points loaded; after that the average rolls.
 *
 *  \param[in] dcmotor  Motor Ptr
 *  \param[in] new_val   New data value
 **/
#ifdef DO_OCC
static void avg_ld_add_val(dcmotor_t * dcmotor, SINT16 new_val)
{
    uint32_t curIdx = dcmotor->mtr_load.LdAvgIdx;

    /* Ignore the first few data points; may have over/under shoot from
     * motor ramp up
     */
    if (dcmotor->mtr_load.LdAvgSettleCount < dcmotor->mtr_load.LdAvgSettleLimit)
    {
        dcmotor->mtr_load.LdAvgSettleCount++;
        return;
    }

    /* Quick sanity check (don't want to stomp off end of our array) */
    if (curIdx >= LD_AVG_SIZE)
    {
        dbg1(("ERROR: LdAvgIdx out of bounds (%d)\n", curIdx));
        ASSERT(FALSE);
    }

    /* curIdx is positioned at the point to insert the next sample, which also
     * happens to point at:
     *  - 0 if the load array has not wrapped around yet
     *  - an old load value that is about to be replaced with a new one
     * Subtract out the old value from our running total before we insert the new value.
     */
    dcmotor->mtr_load.LdTotal -= dcmotor->mtr_load.LdRollAvg[curIdx];

    /* Save new data value and move current index */
    dcmotor->mtr_load.LdValidSamples++;
    dcmotor->mtr_load.LdRollAvg[curIdx] = new_val;
    curIdx++;

    /* Update the running total with the new value */
    dcmotor->mtr_load.LdTotal += new_val;

    /* Handle index roll over */
    if (curIdx >= LD_AVG_SIZE)
    {
        /* Reset our index, and all data slots are now full (valid) */
        curIdx = 0;
        dcmotor->mtr_load.LdAvgValid = TRUE;
    }

    /* Save current index */
    dcmotor->mtr_load.LdAvgIdx = curIdx;
}
#endif
/**
 *  \brief (Helper) Calculate motor current
 *
 *  Helper function to calculate the approximate motor current based using:
 *   I = (V - E) / R
 *
 * Where:
 *   V = Voltage applied (by PWM)
 *   E = Back EMF (Motor RPM * Voltage Constant)
 *   R = Motor terminal resistance
 *
 *  \param[in] dcmotor  Motor Ptr
 *
 *  \return SINT16
 *  \retval Motor current in mA
 **/
#ifdef DO_OCC
static SINT16 calc_mtr_current(dcmotor_t * dcmotor)
{
    float  duty_val;
    float  pwm_per;
    float  pwm_duty_pct;
    float  V;
    float  RPM;
    float  Bemf;
    float  I;

    XASSERT(dcmotor->mtr_consts != NULL, (uint32_t)dcmotor->mtr_consts);

    /* Start with the applied voltage V (PWM duty percent * supply voltage) */
    duty_val = dcmotor->prev_CtrlPWM >> dcmotor->fbits;
    pwm_per  = dcmotor->pwm_period;
    pwm_duty_pct = duty_val / pwm_per;

    V = pwm_duty_pct * dcmotor->mtr_consts->motor_voltage;

    /* Figure out the motor RPM.  We know our speed in quads per ISR period, and
     * the number of quads per motor revolution.
     */
    RPM = (dcmotor->curr_velocity / dcmotor->mtr_consts->quads_rev) *
            (60.0 / (dcmotor->mtr_connects->sample_period * 0.001));
    RPM = RPM / (1<<dcmotor->fbits);

    /* Get the back EMF */
    Bemf = (RPM / 1000) * dcmotor->mtr_consts->Kv;

    /* Now calculate the current */
    I = (V - Bemf) / dcmotor->mtr_consts->term_resist;

    /* Return the integer value as mA */
    return((SINT16)(I * 1000));
}
#endif
/**
 *  \brief (Helper) Call OCC callback for specified event
 *
 *  Call the OCC callback for the specified event, but:
 *    - must have a registered OCC callback function
 *    - the event must be enabled
 *    - must be first call attempt (events are one shot)
 *
 *  \param[in] dcmotor  Motor Ptr
 *  \param[in] OCCevent  OCC event type
 **/
#ifdef DO_OCC
static void make_occ_call(dcmotor_t * dcmotor, OCCErrorCodes_t OCCevent)
{
    /* See if an OCC function is registered, enabled, and not yet called */
    if ( (dcmotor->MechOCCFunc != NULL) &&
         (dcmotor->OCCEnable[OCCevent] == TRUE) &&
         (dcmotor->OCCSent[OCCevent] == FALSE) )
    {
        /* OCC events are one shot: mark it as called and make the call */
        dcmotor->OCCSent[OCCevent] = TRUE;
        (dcmotor->MechOCCFunc)(OCCevent);
    }
}
#endif
/**
 *  \brief (Helper) See if motor load OCC event has occurred
 *
 *  Called after the control loop has calculated velocity, errors, and new PWM
 *  command to check for potential problems with the motor load (current). Note
 *  that we only check the load when the motor is running at steady state; ramps
 *  just get too wild.
 *
 *  \param[in] dcmotor  Motor Ptr
 **/
#ifdef DO_OCC
static void do_load_occ_processing(dcmotor_t * dcmotor)
{
    int32_t     AvgLoad;           /* Average load value this time around  */
    SINT16     motorI;            /* Motor current (in mA)                */

    /* Sudden changes in our steady state load can be interesting to the mech code
     * in some cases.  Look at motor load when motor is in steady state ... 
     */
    if ( dcmotor->motor_state == MOT_STATE_STEADY )
    {
        /* Calculate motor current (in mA), this will be the basis for motor load */
        motorI = calc_mtr_current(dcmotor);

        /* Add our most recent motor current to the data set */
        avg_ld_add_val(dcmotor, motorI);

        /* Attempt to calculate the average load (only works is data set is full) */
        if (avg_ld_calc(dcmotor, &AvgLoad) == 0)
        {
            /* We have a valid average load, log it */

            /* Only run no load logic if enabled for this move and a valid target is set */
            if ( (debug_disable_safety == FALSE) &&
                 (dcmotor->OCCEnable[OCC_MTR_NO_LOAD] == TRUE) &&
                 (dcmotor->NoLdTarget != MTR_LD_BAD_AVE) )
            {
                /* Complain if no threshold set (should have bailed above) */
                XASSERT(dcmotor->NoLdTarget != MTR_LD_BAD_AVE, dcmotor->NoLdTarget);

                /* Verify no load limit is still OK */
                if (AvgLoad < dcmotor->NoLdTarget)
                {
                    /* Time to make OOC call if registered and not already sent */
                    make_occ_call(dcmotor, OCC_MTR_NO_LOAD);
                }
            } /* end if No Load OCC enabled */

        } /* end if valid load average */
    } /* end if steady state */
}
#endif

/**
 *  \brief (Helper) Reset our OCC state
 *
 *  Helper function to reset our OCC state.  Because OCC events are one-shot, we
 *  will do this at the end of every move.
 *
 *  \param[in] dcmotor motor ptr
 **/
static void reset_occ_state(dcmotor_t * dcmotor)
{
    uint32_t i;

    for (i = 0; i < OCC_NUM_ERRS; i++)
    {
        dcmotor->OCCSent[i]   = FALSE;
        dcmotor->OCCEnable[i] = FALSE;
    }
}
/**
 *  \brief (Helper) Enable motor interrupts
 *
 *  Turn on the motor block IRQ: Note that all motor IRQ's will be acked
 *  first to prevent spurious interrupts.
 *
 *  \param[in]  motor_id    Motor ID
 **/
static void mot_int_enable(dcmotor_t * dcmotor)
{
    uint32_t intAckMask;

    /* Ack interrupts so any outstanding interrupts don't fire. */
    intAckMask =
        (   DCMOTOR_BASE_IACK_ENCPOS_MASK |
            DCMOTOR_BASE_IACK_IENC_MASK   |
            DCMOTOR_BASE_IACK_IPER_MASK   |
            DCMOTOR_BASE_IACK_FILR_MASK   |
            DCMOTOR_BASE_IACK_OUTR_MASK   |
            DCMOTOR_BASE_IACK_WATCH_MASK  |
            DCMOTOR_BASE_IACK_ZPOS_MASK   |
            DCMOTOR_BASE_IACK_ENC_MASK    |
            DCMOTOR_BASE_IACK_TBASE_MASK
        );
    dcmotor->motor_regs->IACK = intAckMask;

    /* Enable the motor block IRQ */
    uio_int_enable(dcmotor->uio_dev);
}


/**
 *  \brief (Helper) Disable all motor interrupts
 *
 *  Will mask out all the individual motor interrupts as well as disable the
 *  motor block IRQ.
 *
 *  \param[in]  dcmotor dcmotor ptr
 **/
static void mot_int_disable(dcmotor_t * dcmotor)
{
    /* Shut down the block IRQ */
    uio_int_disable(dcmotor->uio_dev);

    /* Turn off all the individial motor interrupts (forces a clean setup for 
     * every move) 
     */
    dcmotor->motor_regs->IEN = 0;
}

/**
 *  \brief (Helper) Scan motor halt (non-emergency)
 *
 *  \param[in] dcmotor  Motor Ptr
 **/
static void mot_dc_halt(dcmotor_t * dcmotor)
{
    

    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    DBG_INFO("%s: motor %d\n", __func__, dcmotor->motor_id);

    /* Disable all motor interrupts */
    mot_int_disable(dcmotor);

    /* Set pwm duty cycle to 0 */
    mot_no_pwm(dcmotor);

    /* Disable all PWM outputs to prevent accidental PWM application */
    /* Turn off motor by reseting MCFG EN bit */
    set_mtr6pin_motorconfig_enable( dcmotor->motor_regs, MTR6PIN_MCFG_DISABLE );

    /* Now that the motor block is disabled, turn off the PWM watchdog */
    mtr6pin_pwm_wdog_disable( dcmotor->motor_regs );

    /* Move done: reset all OCC state (not enabled, not sent) */
    reset_occ_state(dcmotor);

    /* Now in stopped state (warning: motor could still be moving) */
    mot_set_motor_state(dcmotor, MOT_STATE_STOP);
}
/**
 *  \brief (Helper) Reset motor load rolling average
 *
 *  Helper function to reset OCC rolling average state.
 *
 *  \param[in] dcmotor motor ptr
 **/
static void avg_ld_reset(dcmotor_t * dcmotor)
{
    uint32_t i;

    /* Reset the current index and data valid flag */
    dcmotor->mtr_load.LdTotal          = 0;
    dcmotor->mtr_load.LdAvgIdx         = 0;
    dcmotor->mtr_load.LdAvgValid       = FALSE;
    dcmotor->mtr_load.LdAvgSettleCount = 0;
    dcmotor->mtr_load.LdValidSamples   = 0;

    /* Zero out the data points just to be safe */
    for (i = 0; i < LD_AVG_SIZE; i++)
    {
        dcmotor->mtr_load.LdRollAvg[i] = 0;
    }
}
/**
 *  \brief (Helper) Read the current encoder value
 *
 *  Helper function to read Read the current encoder value from the
 *  supplied register address
 *
 *  \warning
 *  We must return a scaled encoder value here (see motors[motor_id].fbits) as
 *  current clients expect it.  As we get more valid position bits in the
 *  encoder register our scaled values will overflow.  Need to find a
 *  better way to cork up this scaling business.
 *
 *  \param[in] dcmotor  Client motor Ptr
 *
 *  \return uint32_t
 *  \retval Scaled encoder value
 **/
static uint32_t mot_enc_position_read( dcmotor_t * dcmotor )
{
    return (get_mtr6pin_encoder_pos( dcmotor->motor_regs ) << dcmotor->fbits);
}

static uint32_t mot_enc_period_read( dcmotor_t * dcmotor,uint32_t *rising )
{
    return (get_mtr6pin_encoder_period( dcmotor->motor_regs, dcmotor->mtr_connects->enc_inputs, rising ));
}
/**
 *  \brief (API) Motor Sleep
 *
 *  API function: please see documentation in scanmot_dc.h
 **/

static void smot_dc_run(dcmotor_t * dcmotor)
{
    uint32_t   startPos;
    uint32_t v_read;
    int i;

    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    motor_dbg1(( "%s motor_id=%d\n", __FUNCTION__, dcmotor->motor_id ));
        /* Bail out if the motor has been disabled! */
    if (dcmotor->motor_state == MOT_STATE_DISABLED)
    {
        dbg1(("%s: Motor id (%d) is DISABLED!\n", __FUNCTION__, dcmotor->motor_id));
        return;
    }

    /* At this point we MUST have the motor move params configured */
    XASSERT(dcmotor->move_params != NULL, (uint32_t)dcmotor->move_params);

    /* Trying to start another move while one is already in progress? Very Naughty. */
    //XASSERT(dcmotor->motor_state == MOT_STATE_STOP, dcmotor->motor_state);

    /* All of the motor irqs should be disabled here */
    
    XASSERT(dcmotor->motor_regs->IEN == 0, dcmotor->motor_regs->IEN); 

    /* Make sure the motor has no PWM applied */
    //dbg1(( "Exisiting PWM Control 0x%08lX\n", motors[motor_id].motor_regs->pwm_control ));
    if (dcmotor->motor_state == MOT_STATE_STOP)
    {
        mot_no_pwm(dcmotor);
    
    

        /* Read the current encoder position and use it as our starting point */
        startPos = mot_enc_position_read(dcmotor);

        /* Dump out some debug info, but make sure we are not in IRQ context (can happen during
        * pause/resume).
        */
//-jrs        if ( !IN_INTERRUPT() )
        {
            dbg1(("%s: id=%d speed=%ld accel=%ld pwmlow=%ld maxperr=%ld\n",
                  __FUNCTION__, dcmotor->motor_id,
                  dcmotor->move_params->speed,
                  dcmotor->move_params->acceleration,
                  dcmotor->move_params->maxPwmLow,
                  dcmotor->move_params->pos_error_limit));

            dbg2(( "%s: current pos: %d\n", __FUNCTION__, (startPos >> dcmotor->fbits) ));
        }

        dcmotor->MeasuredPosTime0      = startPos;
        dcmotor->calcPosTime0          = startPos;
        dcmotor->MeasuredPosTimeMinus1 = startPos;
        dcmotor->RefPos                = startPos;

        dcmotor->RefVel         = 0;
        dcmotor->RefBias         = 0;
        dcmotor->MoveDistance   = 0;
        dcmotor->MoveCount      = 0;
        dcmotor->move_direction = MOT_FORWARD;
        dcmotor->RunForever     = true;
        dcmotor->curr_velocity  = 0;
        dcmotor->prev_velocity  = 0;
        dcmotor->prev_vel_error = 0;
        dcmotor->sum_error      = 0;
        dcmotor->CtrlPWM        = 0;
        dcmotor->prev_CtrlPWM   = 0;
        dcmotor->rampCounts     = 0;
        dcmotor->decelCounts    = 0;
        dcmotor->coastCounts    = 0;
        dcmotor->stopCounts     = 0;
        dcmotor->ctl_loop_count = 0;
        dcmotor->sum_pwm_period = 0;
        if (adc_24v) 
        {
            v_read = dec_adc_raw_read_channel(adc_24v);
        }
        else
        {
            v_read = 0;
        }
        
        dcmotor->voltage_sum = v_read * NUM_VOLTAGE_SUMS;
        dcmotor->voltage_error_sum = 0;
        for (i = 0; i < NUM_VOLTAGE_SUMS; i++) 
        { 
            dcmotor->voltage[i] = v_read;
            dcmotor->voltage_error[i] = 0;
        }
        dcmotor->voltage_index = 0;
        dcmotor->tick = 0;
        dcmotor->error_filter_pos = 0;
        /* Reset our rolling average load */
        avg_ld_reset(dcmotor);

        /* Set up the encoder debounce values for this move */
        mot_set_enc_debounce(dcmotor);
    }
    else
    {
        dcmotor->RefVel         = dcmotor->curr_velocity;
    }

    if (dcmotor->laser && (dcmotor->current_speed->index == 2)) 
    {
        dcmotor->period_ref = (240 * ONE_MHZ * dcmotor->mtr_connects->sample_period) / (1000 / 2 );
    }
    else
    {
        dcmotor->period_ref = (hwGetBusSpeed() * ONE_MHZ * dcmotor->mtr_connects->sample_period) / (1000 / 2 );

    }

        
    DBG_INFO("period_ref %d\n",dcmotor->period_ref);
    /* We will be accelerating next ... */
    mot_set_motor_state(dcmotor, MOT_STATE_ACCEL );

    /* Set the motor direction.  Note that not all PWM methods use motor direction
     * (it will not be mapped in the connect table); the 6-pin driver will simply
     * ignore it if not mapped.
     */
    set_mtr6pin_dir_state( dcmotor->motor_regs, 
                           dcmotor->mtr_connects,
                           MOT_FORWARD );

    /* Set the motor enable.  Note that not all PWM methods use motor enable
     * (it will not be mapped in the connect table); the 6-pin driver will simply
     * ignore it if not mapped.
     */
    set_mtr6pin_en_state( dcmotor->motor_regs, 
                          dcmotor->mtr_connects,
                          1 );

    /* Debug dump before motor motion starts */
    //smot_dc_dump(motor_id);

    /* Turn on the PWM watchdog, give it 10x the servo period */
    if(!debug_disable_pwm_wdog)
    {
        mtr6pin_pwm_wdog_enable( dcmotor->motor_regs, 10 * dcmotor->mtr_connects->sample_period );
    }

    /* Enable the motor block (enables the PWM signal) */
    set_mtr6pin_motorconfig_enable(dcmotor->motor_regs, MTR6PIN_MCFG_ENABLE);

    /* Enable our base motor interrupts (others may have been enabled before this point),
     * but note that the big block interrupt is still disabled for now.
     */

    dcmotor->motor_regs->IEN |= 
                       (
                         DCMOTOR_BASE_IEN_TBASE_MASK |   /* Timebase                   */
                         DCMOTOR_BASE_IEN_IENC_MASK      /* Illegal encoder transition */
                       );

    if (dcmotor->use_encoder_isr) 
    {
        dcmotor->motor_regs->IEN |= 
                       (
                         DCMOTOR_BASE_IEN_ENC_MASK
                       );
    }

    /* Ack all the individual motor interrupts and turn on the big motor block irq */
    mot_int_enable(dcmotor);
}
/**
 * \brief just set a pwm value.  Used for external mtr control.
 * 

 * 
 * @param dcmotor The motor to update
 * @param pwm  The pwm value to set.
 */
void smot_start_motor(dcmotor_t *dcmotor, uint32_t pwm)
{
  //  dbg1(("pwm input value %x\n",pwm));
    mot_pwm_control(dcmotor, abs(pwm), true);
    mot_set_motor_state (dcmotor, MOT_STATE_STEADY );
    /* Set the motor enable.  Note that not all PWM methods use motor enable
     * (it will not be mapped in the connect table); the 6-pin driver will simply
     * ignore it if not mapped.
     */
    set_mtr6pin_en_state( dcmotor->motor_regs, 
                          dcmotor->mtr_connects,
                          1 );
    /* Enable the motor block (enables the PWM signal) */
    set_mtr6pin_motorconfig_enable(dcmotor->motor_regs, MTR6PIN_MCFG_ENABLE);
}



static void smot_dc_enter_sleep(dcmotor_t * dcmotor)
{
    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    ASSERT(dcmotor->motor_state == MOT_STATE_STOP);

    /* Disable all motor interrupts */
    mot_int_disable(dcmotor);

    /* Make sure we don't have any PWM applied */
    mot_no_pwm(dcmotor);

    /* Disable the motor block */
    set_mtr6pin_motorconfig_enable( dcmotor->motor_regs, MTR6PIN_MCFG_DISABLE );

    /* Now that the motor block is disabled, turn off the PWM watchdog */
    mtr6pin_pwm_wdog_disable( dcmotor->motor_regs );
    set_mtr6pin_slp_state( dcmotor->motor_regs, // order IS important or you will hit the brake
                           dcmotor->mtr_connects,
                           1 );
    set_mtr6pin_mode_state( dcmotor->motor_regs, 
                           dcmotor->mtr_connects,
                           1 );
}

/**
 *  \brief (API) Motor Exit Sleep
 *
 *  API function: please see documentation in scanmot_dc.h
 **/
static void smot_dc_exit_sleep(dcmotor_t * dcmotor)
{
    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    motor_dbg1(( "%s motor_id=%d\n", __FUNCTION__, dcmotor->motor_id ));

    ASSERT(dcmotor->motor_state == MOT_STATE_STOP);
    set_mtr6pin_mode_state( dcmotor->motor_regs, // order IS important or you will hit the brake
                           dcmotor->mtr_connects,
                           0 );
    set_mtr6pin_slp_state( dcmotor->motor_regs, 
                           dcmotor->mtr_connects,
                           0 );
    //mot_blk->mot_config = (mot_blk->mot_config & ~(DCMOTOR_MOT_CFG_EN(1) | DCMOTOR_MOT_CFG_SLEEP(1)));

    /* Exit sleep:
     *  TBD, for now leave the motor block disabled.  We will turn it on before
     *  doing a move.
     */

    /* Provide a (min) 1 ms interval before applying PWM signals to allow the
     * charge pump to stabilize
     */
    posix_sleep_ms(20);
}

void smot_dc_enter_sleep_4931(dcmotor_t * dcmotor)
{
    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    motor_dbg1(( "%s motor_id=%d\n", __FUNCTION__, dcmotor->motor_id ));

    ASSERT(dcmotor->motor_state == MOT_STATE_STOP);

    /* Disable all motor interrupts */
    mot_int_disable(dcmotor);

    /* Make sure we don't have any PWM applied */
    mot_no_pwm(dcmotor);

    /* Disable the motor block */
    set_mtr6pin_motorconfig_enable( dcmotor->motor_regs, MTR6PIN_MCFG_DISABLE );

    /* Now that the motor block is disabled, turn off the PWM watchdog */
    mtr6pin_pwm_wdog_disable( dcmotor->motor_regs );

    set_mtr6pin_slp_state( dcmotor->motor_regs, // order IS important or you will hit the brake
                           dcmotor->mtr_connects,
                           1 );
#if 0 //Not needed
    set_mtr6pin_mode_state( dcmotor->motor_regs, 
                           dcmotor->mtr_connects,
                           1 );
#endif
}
void smot_dc_enter_sleep_enable(dcmotor_t * dcmotor)
{
    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    motor_dbg1(( "%s motor_id=%d\n", __FUNCTION__, dcmotor->motor_id ));

    ASSERT(dcmotor->motor_state == MOT_STATE_STOP);

    /* Disable all motor interrupts */
    mot_int_disable(dcmotor);

    /* Make sure we don't have any PWM applied */
    mot_no_pwm(dcmotor);

    /* Disable the motor block */
    set_mtr6pin_motorconfig_enable( dcmotor->motor_regs, MTR6PIN_MCFG_DISABLE );

    /* Now that the motor block is disabled, turn off the PWM watchdog */
    mtr6pin_pwm_wdog_disable( dcmotor->motor_regs );

    set_mtr6pin_slp_state( dcmotor->motor_regs, // order IS important or you will hit the brake
                           dcmotor->mtr_connects,
                           0 );
#if 0 //Not needed
    set_mtr6pin_mode_state( dcmotor->motor_regs, 
                           dcmotor->mtr_connects,
                           1 );
#endif
}
/**
 *  \brief (API) Motor Exit Sleep
 *
 *  API function: please see documentation in scanmot_dc.h
 **/

void smot_dc_exit_sleep_4931(dcmotor_t * dcmotor)
{
    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    motor_dbg1(( "%s motor_id=%d\n", __FUNCTION__, dcmotor->motor_id ));
    if (dcmotor->motor_state != MOT_STATE_STOP) 
    {
        return;
    }
    ASSERT(dcmotor->motor_state == MOT_STATE_STOP);
#if 0 //don't do this on this chip
    set_mtr6pin_mode_state( dcmotor->motor_regs, // order IS important or you will hit the brake
                           dcmotor->mtr_connects,
                           0 );

    set_mtr6pin_slp_state( dcmotor->motor_regs, 
                           dcmotor->mtr_connects,
                           0 );
#endif 
    //mot_blk->mot_config = (mot_blk->mot_config & ~(DCMOTOR_MOT_CFG_EN(1) | DCMOTOR_MOT_CFG_SLEEP(1)));

    /* Exit sleep:
     *  TBD, for now leave the motor block disabled.  We will turn it on before
     *  doing a move.
     */

    /* Provide a (min) 1 ms interval before applying PWM signals to allow the
     * charge pump to stabilize
     */
    posix_sleep_ms(20);
}

/**
 *  \brief (API) Motor Exit Sleep
 *
 *  API function: please see documentation in scanmot_dc.h
 **/

void smot_dc_exit_sleep_enable(dcmotor_t * dcmotor)
{
    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    motor_dbg1(( "%s motor_id=%d\n", __FUNCTION__, dcmotor->motor_id ));
    if (dcmotor->motor_state != MOT_STATE_STOP) 
    {
        return;
    }
    ASSERT(dcmotor->motor_state == MOT_STATE_STOP);
#if 0 //don't do this on this chip
    set_mtr6pin_mode_state( dcmotor->motor_regs, // order IS important or you will hit the brake
                           dcmotor->mtr_connects,
                           0 );
#endif
    set_mtr6pin_slp_state( dcmotor->motor_regs, 
                           dcmotor->mtr_connects,
                           1 );
 
    //mot_blk->mot_config = (mot_blk->mot_config & ~(DCMOTOR_MOT_CFG_EN(1) | DCMOTOR_MOT_CFG_SLEEP(1)));

    /* Exit sleep:
     *  TBD, for now leave the motor block disabled.  We will turn it on before
     *  doing a move.
     */

    /* Provide a (min) 1 ms interval before applying PWM signals to allow the
     * charge pump to stabilize
     */
    posix_sleep_ms(20);
}
/**
 *  \brief (API) Set the encoder value to middle of its range
 *
 *  API function: please see documentation in scanmot_dc.h
 **/
static void mot_dc_set_mid_position(dcmotor_t * dcmotor)
{
    uint32_t mid_pos;

    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    mid_pos = get_mtr6pin_encoder_pos_max_value() / 2;
    set_mtr6pin_encoder_pos( dcmotor->motor_regs, mid_pos >> dcmotor->fbits );

    motor_dbg1(("%s: pos=%ld\n", __FUNCTION__, mot_enc_position_read(dcmotor) ));
}

static void smot_dc_set_motor_move_params (dcmotor_t * dcmotor, const motor_param_t *move_params)
{
    /* WARNING: this routine can be called from interrupt context (usually a scan trigger)!! */

    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        dbg1(("%s: Invalid motor id (%x)\n", __FUNCTION__, dcmotor));
        return;
    }

    /* Null motor move params would be rude */
    XASSERT(move_params != NULL, (uint32_t)move_params);

    /* Set the motor move parameters pointer */
    dcmotor->move_params = move_params;

    /* DEBUG: in cases where we disable the jam logic it is sometimes helpful to
     *   limit how much the position error can wind up (it results in running over
     *   target speed to bring the integral term back to zero).  With jam detect
     *   enabled this will not change anything ... its way over the jam limit.
     */
    dcmotor->PosErrCap = move_params->pos_error_limit + move_params->pos_error_limit / 4;//move_params->pos_error_limit * 1.25
//-jrs    if( !IN_INTERRUPT() )
    {
       dbg2(("%s: PosErrLimit=%d PosErrCap=%d\n", __FUNCTION__,
                  dcmotor->move_params->pos_error_limit, 
                  dcmotor->PosErrCap));
    }
}


/**
 *  \brief (Helper) Setup default values for the encoder period logic
 *
 **/
static void mot_set_default_enc_periods(dcmotor_t * dcmotor)
{
    /* Set power on defaults for the encoder period logic */
    set_mtr6pin_encoderconfig_iscale( dcmotor->motor_regs, 0 );
    set_mtr6pin_pm_oscale( dcmotor->motor_regs, 0 );
    set_mtr6pin_pm_mscale( dcmotor->motor_regs, 0 );
    set_mtr6pin_pm_mult( dcmotor->motor_regs,   0 );

    set_mtr6pin_encoderconfig_rsyncsel( dcmotor->motor_regs, MTR6PIN_RS_PULSE );
    set_mtr6pin_encoderconfig_rmode( dcmotor->motor_regs, MTR6PIN_RMODE_NOFILTER_CYCLE );
}

/**
 *  \brief (Helper) Setup the row syncs
 *
 **/
static void mot_set_row_sync(dcmotor_t * dcmotor,
                             MOTOR_SYNC  rowsync_type,
                             uint32_t      rowsync_pos)
{
    uint32_t start_pos;
    uint32_t current_position;
    uint32_t verify_position;

    /* rowsync_pos is passed in as a scaled encoder value, we must shift it down before
     * programming it into hardware.
     */
    ASSERT((rowsync_pos >> dcmotor->fbits) <= get_mtr6pin_encoder_pos_max_value());
    start_pos = rowsync_pos >> dcmotor->fbits;

    /* Save the current encoder position */
    current_position = get_mtr6pin_encoder_pos(dcmotor->motor_regs);

    /* Note: setting the encoder position to zero has a secondary function of setting a
     *       hardware state variable that disables row sync outputs (until the next start
     *       pos is hit).
     */
    set_mtr6pin_encoder_pos( dcmotor->motor_regs, 0 );

    /* Restore the encoder position (and validate our restored register setting) */
    set_mtr6pin_encoder_pos( dcmotor->motor_regs, current_position );
    verify_position = get_mtr6pin_encoder_pos(dcmotor->motor_regs);
    if (verify_position != current_position)
    {
        dbg1(("%s: curr_pos=%ld ver_pos=%ld\n", 
                  __FUNCTION__, current_position, verify_position));
        XASSERT( verify_position == current_position, verify_position );
    }

    /* The current implementation never stops row syncs, so set that to ignore here */
    set_mtr6pin_rstop_ignore( dcmotor->motor_regs, 1 );
    set_mtr6pin_rstop( dcmotor->motor_regs, 0 );

    /* Always start out with the encoder period defaults, customize below as needed */
    mot_set_default_enc_periods(dcmotor);

    /* Setup desired row synch start behavior */
    switch (rowsync_type)
    {
        case SYNC_DISABLE:
            /* Do not generate any row sync pulses */
            /* Set the start position out in the weeds: we should never hit this
             * under normal operating conditions.
             */
            set_mtr6pin_rstart( dcmotor->motor_regs, UNREACHABLE_RS_START );
            break;

        case SYNC_FIRST:
            /* Just one row sync at the target encoder position */
            set_mtr6pin_rstart( dcmotor->motor_regs, start_pos );
            break;

        case SYNC_EVERY:
            /* Motor control sends all row syncs based on encoder period */

            /* Sync every will need additional encoder period irq's enabled. Note the motor
             * block main irq is still disabled at this point.
             */
            dcmotor->motor_regs->IEN |= 
                     (
                         DCMOTOR_BASE_IEN_IPER_MASK |   /* Illegal input period       */
                         DCMOTOR_BASE_IEN_FILR_MASK |   /* Filter out of range        */
                         DCMOTOR_BASE_IEN_OUTR_MASK     /* Output period out of range */
                     );

            set_mtr6pin_encoderconfig_iscale( dcmotor->motor_regs, dcmotor->move_params->enc_iscale );
    
            set_mtr6pin_pm_oscale( dcmotor->motor_regs, dcmotor->move_params->rs_oscale );
            set_mtr6pin_pm_mscale( dcmotor->motor_regs, dcmotor->move_params->rs_mscale );
            set_mtr6pin_pm_mult( dcmotor->motor_regs, dcmotor->move_params->rs_pm );

            set_mtr6pin_encoderconfig_rsyncsel( dcmotor->motor_regs, MTR6PIN_RS_PULSE );
            set_mtr6pin_encoderconfig_rmode( dcmotor->motor_regs, MTR6PIN_RMODE_FILTER_CYCLE );

            set_mtr6pin_rstart( dcmotor->motor_regs, start_pos );
            break;

        default:
            dbg1(("%s: Invalid rowsync type=%d\n", __FUNCTION__, rowsync_type));
            XASSERT(FALSE, rowsync_type);
            break;
    }
}

/**
 *  \brief (Helper) Set default encoder input debounce
 *
 *  Helper function to set the default encoder input debounce for a specific 
 *  input pulse width (or reset time).
 *
 *  \param[in] dcmotor  Valid Motor ptr
 *  \param[in] time_usec Time of pulse width to debounce (usec)
 *
 **/
static void mot_set_default_enc_debounce(dcmotor_t * dcmotor, uint32_t time_usec)
{
    uint32_t block_freq = hwGetBusSpeed();
    uint32_t dbCount;

    /* The encoder debounce is clocked by the scan system clock, so figure out
     * how many clock ticks we will need to program.
     *   dbCount = time_usec / clock period;
     * NOTE: The supplied time and block_freq have a lot of zeros in common, so
     *   we can ignore them here.
     */
    dbCount = (uint32_t)(time_usec * block_freq);

    /* Use the same DB percentage as other calculations */
    dbCount = (uint32_t)(dbCount * (ENC_DB_PERCENT / 100.0));

//-jrs    if ( !IN_INTERRUPT() )
    {
      dbg2(("%s: time_usec=%d blockf=%d dbCount=%d\n", 
              __FUNCTION__, time_usec, block_freq, dbCount ));
    }

    /* A zero count would defeat the point: zero debounce can cause unexpected
     * encoder edges when stopped.
     */
    XASSERT(dbCount != 0, dbCount);

    /* Set the debounce value in hardware. Note: the mtr6pin layer will protect
     * against out of range debounce counts.
     */
    set_mtr6pin_debounce_zdb( dcmotor->motor_regs, 0 );
    set_mtr6pin_debounce_edb( dcmotor->motor_regs, dbCount/8 );
}


/**
 *  \brief (Helper) Reset motor duty cycle data
 *
 *  Helper function to reset motor duty cycle state.
 *
 *  \param[in] dcmotor  Motor Ptr
 **/
#ifdef TRACK_DCMOTOR_DUTYCYCLE
static void duty_reset(dcmotor_t * dcmotor)
{
    uint32_t i;

    dcmotor->mtr_duty.curDutyIdx  = 0;
    dcmotor->mtr_duty.curDutyTime = 0;

    /* Reset all of the minute usage buckets */
    for (i = 0; i < DUTY_BUCKETS; i++)
    {
        dcmotor->mtr_duty.usage[i].tv_sec = 0;
        dcmotor->mtr_duty.usage[i].tv_nsec = 0;
    }
}
#endif
/**
 *  \brief (Helper) Validate servo ISR rate
 *
 *  The servo ISR must be called at regular intervals or the algorithm just
 *  falls apart.  If configured:
 *   - assert on rate failure
 *
 *  \param[in] dcmotor  Motor Ptr
 **/
static void isr_rate_check(dcmotor_t * dcmotor)
{
    static uint32_t thisTime = 0;   /* CPU tick this time                  */
    static uint32_t lastTime = 0;   /* CPU tick last time                  */

    uint32_t     isr_time_usecs;    /* Time delta since last ISR           */

    /* Beware of counter roll-over and invalid last tick on first pass through */
    lastTime = thisTime;
    thisTime = timer_get_time_usec();
    if ((dcmotor->ctl_loop_count > 1) && (thisTime > lastTime))
    {
        isr_time_usecs = thisTime - lastTime;

#ifdef ISR_STARVE_ASSERT
        if (isr_time_usecs > ((dcmotor->mtr_connects->sample_period * 1000) + SERVO_ISR_SLOP) )
        {
            dbg1(("\nMotor ISR starved! (isr_time=%d, motors[motor_id].ctl_loop_count=%d)\n",
                     isr_time_usecs, dcmotor->ctl_loop_count ));
            ASSERT(0);
        }
        else if (isr_time_usecs < ((dcmotor->mtr_connects->sample_period * 1000) - SERVO_ISR_SLOP) )
        {
            dbg1(("\nMotor ISR too fast! (isr_time=%d, dcmotor->ctl_loop_count=%d)\n",
                     isr_time_usecs, dcmotor->ctl_loop_count ));
            ASSERT(0);
        }
#endif
    }
}


/**
 *  \brief (Helper) Calculate PWM period and freq divider
 *
 *  This routine calculates the values that need to be written into the pwm 
 *  period and pwm freq divider registers to obtain the desired pwm frequency.
 *  Note the the PWM freq is generally a fixed value set once when the motor is
 *  created.
 *
 *  \param[in]  pwm_freq     Desired PWM frequency
 *  \param[out] pwm_period   Period needed to achieve pwm frequency
 *  \param[out] pwm_divisor  Divisor needed to achieve pwm frequency
 **/
static void calc_mot_pwm_period( uint32_t pwm_freq, uint32_t *pwm_period, uint32_t *pwm_divisor )
{
    uint32_t block_freq;
    uint32_t pwmper;
    uint32_t pwmdiv;

    /* Note: The Oscope verifies our block uses the bus clock freq, not
     *       the system frequency (Mhz is unit of measure)
     */
    block_freq = hwGetBusSpeed() * ONE_MHZ;

    /* Calculate the period before clock divisor is applied */
    pwmper = block_freq / pwm_freq;

    /* From the Gemstone docs:
     *   PWM Freq = Bus Clock / ((PWM_DIV + 1) * PWM_PER)
     * So calculate the divisor and adjust the pwmper.
     */
    pwmdiv  = pwmper / get_mtr6pin_pwm_period_max();
    pwmper /= (pwmdiv + 1);

    dbg1(("%s: Block Freq=%d, PWM Freq=%d, period=%d, divisor=%d\n",
               __FUNCTION__, block_freq, pwm_freq, pwmper, pwmdiv ));

    /* Check calculated value to make sure it fits in the available bitfield */
    XASSERT(pwmper <= get_mtr6pin_pwm_period_max(), pwmper);

    *pwm_period  = pwmper;
    *pwm_divisor = pwmdiv;
}

/**
 * \brief Get dec sensor registers memory mapped to user space using dec_sensor UIO kernel driver.
 * 
 */
MOTOR_REGS* dcmotor_get_regs_base(uio_dev_t* dcmotor_uio_dev)
{
    MOTOR_REGS* motor_regs = NULL;
    uio_map_t *map = uio_get_map(dcmotor_uio_dev, 0);

    if (map != NULL)
    {
        motor_regs = map->virt_addr;
    }
    
    DBG_DEBUG("%s = %#x\n", __func__, motor_regs);

    return motor_regs;
}

/**
 *  \brief (Helper) Set block address and IRQ number
 *
 *  \param[in]  dcmotor  Motor Ptr
 **/
void mot_set_blk_addr_irq(dcmotor_t * dcmotor)
{
    motor_block_t block_num = dcmotor->mtr_connects->block_num;

    switch (block_num)
    {
        case DC_MTR_BLK_0:
            dcmotor->uio_dev = uio_open_dev("dcmotor_6pin_block0");
            XASSERT(dcmotor->uio_dev != NULL, errno);

            dcmotor->motor_regs = dcmotor_get_regs_base(dcmotor->uio_dev);
            ASSERT(dcmotor->motor_regs != NULL);

            dcmotor->motor_irq_num = INTNUM_MOTORS_0;
            break;
        case DC_MTR_BLK_1:
            dcmotor->uio_dev = uio_open_dev("dcmotor_6pin_block1");
            XASSERT(dcmotor->uio_dev != NULL, errno);

            dcmotor->motor_regs = dcmotor_get_regs_base(dcmotor->uio_dev);
            ASSERT(dcmotor->motor_regs != NULL);

            dcmotor->motor_irq_num = INTNUM_MOTORS_1;
            break;
        case DC_MTR_BLK_2:
            dcmotor->uio_dev = uio_open_dev("dcmotor_6pin_block2");
            XASSERT(dcmotor->uio_dev != NULL, errno);

            dcmotor->motor_regs = dcmotor_get_regs_base(dcmotor->uio_dev);
            ASSERT(dcmotor->motor_regs != NULL);

            dcmotor->motor_irq_num = INTNUM_MOTORS_2;
            break;
        case DC_MTR_BLK_3:
            dcmotor->uio_dev = uio_open_dev("dcmotor_6pin_block3");
            XASSERT(dcmotor->uio_dev != NULL, errno);

            dcmotor->motor_regs = dcmotor_get_regs_base(dcmotor->uio_dev);
            ASSERT(dcmotor->motor_regs != NULL);

            dcmotor->motor_irq_num = INTNUM_MOTORS_3;
            break;
        case DC_MTR_BLK_4:
            dcmotor->uio_dev = uio_open_dev("dcmotor_4pin_block0");
            XASSERT(dcmotor->uio_dev != NULL, errno);

            dcmotor->motor_regs = dcmotor_get_regs_base(dcmotor->uio_dev);
            ASSERT(dcmotor->motor_regs != NULL);

            dcmotor->motor_irq_num = INTNUM_MOTORS_4;
            break;

        default:
            dbg1(("%s: bad motor block (%d)\n", __FUNCTION__, block_num));
            XASSERT(0, block_num);
            break;
    }
}

/**
 *  \brief (Helper) Check for stale duty cycle data
 *
 *  If we have been idle too long, then we may have to reset our history
 *
 *  \param[in] dcmotor  Motor Ptr
 *  \param[in] now_time  Current time
 **/
#ifdef TRACK_DCMOTOR_DUTYCYCLE
static void duty_check_stale(dcmotor_t * dcmotor, struct timespec now_time)
{
    /* If we have been idle too long, then we may have to reset our history */
    if (now_time.tv_sec - dcmotor->mtr_duty.curDutyTime >= DUTY_BUCKET_TOTAL)
    {
        /* Wipe the slate clean */
        duty_reset(dcmotor);

        /* Mark the first bucket with our now time */
        dcmotor->mtr_duty.curDutyTime = now_time.tv_sec;
    }
}
#endif
/**
 *  \brief (Helper) Find duty cycle data bucket
 *
 *  Find the bucket that matches up with the current time
 *
 *  \param[in] dcmotor  Motor Ptr
 *  \param[in] now_time  Current time
 **/
#ifdef TRACK_DCMOTOR_DUTYCYCLE
static UINT8 duty_find_bucket(dcmotor_t * dcmotor, struct timespec now_time)
{
    uint32_t i;
    UINT8  search_idx;

    /* Quick sanity check (don't want to stomp off end of our array) */
    if (dcmotor->mtr_duty.curDutyIdx >= DUTY_BUCKETS)
    {
        dbg1(("ERROR: curDutyIdx out of bounds (%d)\n",
               dcmotor->mtr_duty.curDutyIdx));
        ASSERT(FALSE);
    }

    /* Make sure the bucket list data isn't stale */
    duty_check_stale(dcmotor, now_time);

    /* Test the current bucket first, we may already be in the right spot. Each
     * bucket holds a minutes worth of motor usage data.
     */
    if (now_time.tv_sec - dcmotor->mtr_duty.curDutyTime < DUTY_BUCKET_SECS)
    {
        /* We have arrived */
        return dcmotor->mtr_duty.curDutyIdx;
    }

    /* Now we have to dig for a new bucket.  Note that we must set each bucket we
     * pass over to zero.
     */
     search_idx = dcmotor->mtr_duty.curDutyIdx;

     for (i = 1; i < DUTY_BUCKETS; i++)
     {
        /* Advance the search index and handle wrap around as needed */
        search_idx++;
        if (search_idx >= DUTY_BUCKETS)
        {
            /* Wrap around in the usage array */
            search_idx = 0;
        }

        /* Clear the new bucket (may contain stale data) */
        dcmotor->mtr_duty.usage[search_idx].tv_sec = 0;
        dcmotor->mtr_duty.usage[search_idx].tv_nsec = 0;

        /* Is this the right spot in time? */
        if (now_time.tv_sec - (dcmotor->mtr_duty.curDutyTime + (i * DUTY_BUCKET_SECS)) < DUTY_BUCKET_SECS)
        {
            /* We have arrived */
            dcmotor->mtr_duty.curDutyTime = now_time.tv_sec;
            dcmotor->mtr_duty.curDutyIdx  = search_idx;
            return dcmotor->mtr_duty.curDutyIdx;
        }
     }

     /* We should have returned above, bad mojo if we get here */
     ASSERT(FALSE);
     return dcmotor->mtr_duty.curDutyIdx;
}
#endif
/**
 *  \brief (Helper) Add data to a duty cycle data bucket
 *
 *  Add usage data to the specified bucket
 *
 *  \param[in] dcmotor     Motor Ptr
 *  \param[in] usage_bucket Bucket index
 *  \param[in] nsec         Nanoseconds to add to bucket
 **/
#ifdef TRACK_DCMOTOR_DUTYCYCLE
static void duty_increment_bucket(dcmotor_t * dcmotor, UINT8 usage_bucket, uint32_t nsec)
{
    /* Quick sanity check (don't want to stomp off end of our array) */
    if (usage_bucket >= DUTY_BUCKETS)
    {
        dbg1(("ERROR: usage_bucket out of bounds (%d)\n", usage_bucket));
        ASSERT(FALSE);
    }

    /* Just add the new time right in, don't worry about blow-outs */
    dcmotor->mtr_duty.usage[usage_bucket].tv_nsec += nsec;

    /* Now sort out any nsec blow outs */
    while( dcmotor->mtr_duty.usage[usage_bucket].tv_nsec >= NANOSECS_PER_SECOND )
    {
        dcmotor->mtr_duty.usage[usage_bucket].tv_nsec -= NANOSECS_PER_SECOND;
        dcmotor->mtr_duty.usage[usage_bucket].tv_sec  += 1;
    }

    /* Simple sanity check bucket limits */
    XASSERT(dcmotor->mtr_duty.usage[usage_bucket].tv_sec <= DUTY_BUCKET_SECS,
            dcmotor->mtr_duty.usage[usage_bucket].tv_sec);
}
#endif
/**
 *  \brief (Helper) Add motor duty cycle usage
 *
 *  Helper function to reset motor duty cycle state.
 *
 *  \param[in] dcmotor  Motor Ptr
 *  \param[in] nsec      Nanoseconds of usage to add
 **/
#ifdef TRACK_DCMOTOR_DUTYCYCLE
static void duty_add_usage_nsec(dcmotor_t * dcmotor, uint32_t nsec)
{
    int             ret_code;
    UINT8           usage_bucket;
    struct timespec now_time;

    /* Grab the current time (seconds since boot) */
    ret_code = clock_gettime( CLOCK_REALTIME, &now_time );
    XASSERT(ret_code == 0, ret_code);

    /* Hunt down the proper bucket to stash our motor usage */
    usage_bucket = duty_find_bucket(dcmotor, now_time);

    /* Add the motor run time data to the bucket */
    duty_increment_bucket(dcmotor, usage_bucket, nsec);

}
#endif
/**
 *  \brief (Helper) Scan motor servo control loop
 *
 *  \param[in] dcmotor  Motor Ptr
 **/
static void mot_dc_update_control_output(dcmotor_t * dcmotor)
{
    int32_t     etCurVelocity;     /* Encoder tick based velocity         */
    int32_t     etCurrAccel;
    int32_t     RawEtCurVelocity;  /* Encoder tick based velocity - RAW   */
    int32_t     RefCurrVelocity = 0;
    int32_t     VelError;          /* error in velocity                   */
    int32_t     deltaVelError;
    int32_t     PosError;          /* error in position                   */
    int32_t     CtrlPWM;           /* controller output                   */    
    int32_t     p_term = 0;        /* for the pi calculations             */
    int32_t     i_term = 0;
    int32_t     d_term = 0;
    int32_t     r_term;            /*repeditive term*/
    int32_t     PWMLimit;          /* PWM limit for this motor mode       */
    uint32_t     enc_period;
    uint32_t     enc_period_dev = 1;
    uint32_t     period_error;
    int32_t     PrevSum;
    int32_t     ErrorTerm = 0;

    /* Service the PWM wdog unless:
     *  - debug command to pop wdog is set
     *  - watchdog already expired (don't want to accidentally reset PWM)
     */
    if (debug_disable_pwm_wdog == FALSE)
    {
        mtr6pin_pwm_wdog_service(dcmotor->motor_regs, WDOG_NO_PWM_RESET);
    }

    /* Nothing to do here if we are stopped, bail out now */
    if (dcmotor->motor_state == MOT_STATE_STOP || dcmotor->motor_state == MOT_STATE_DISABLED)
    {
        /* This state is a bit unexpected: shut down the PWM to be safe */
        mot_no_pwm(dcmotor);
        return;
    }

    /* At this point we MUST have the motor move params configured */
    XASSERT(dcmotor->move_params != NULL, (uint32_t)dcmotor->move_params);

    /* Bump control loop counter (gets reset to zero before each move) */
    dcmotor->ctl_loop_count++;

    /* System ERROR Checking:
     *   Check our ISR rate to make sure all is well (not too fast or too slow).
     *   This routine will assert (if configured) on error.
     */
    isr_rate_check(dcmotor);

    /* Save the previous encoder position and read the current position (scaled values) */
    period_error = 0;
    if (debug_disable_servo == FALSE)
    {
        /* Under normal servo control, use the encoder */
        dcmotor->MeasuredPosTimeMinus1 = dcmotor->MeasuredPosTime0;
        dcmotor->MeasuredPosTime0      = mot_enc_position_read( dcmotor );
#ifdef HAVE_DEC_LASER
        if (dcmotor->laser && dcmotor->current_speed->index == 2)
        {
            enc_period = (dcmotor->current_speed->index == 2)?dec_laser_period(dcmotor->laser, &period_error)   : 0;
            enc_period_dev = 18;
            //period_error /= 18;

             //DBG_LOG_ERR(" pwm %d enc_period %x period_error %x diff %d\n",dcmotor->CtrlPWM >> dcmotor->fbits,enc_period,period_error, period_error - enc_period  );
        }
        else if (dcmotor->laser) 
        {
            uint32_t edge;
            RefCurrVelocity = dcmotor->RefVel;
            enc_period = mot_enc_period_read(dcmotor, &edge);
            DBG_LOG_DEBUG(" pwm %d pp enc_period %x dcmotor->period_ref %x diff %d\n",dcmotor->CtrlPWM >> dcmotor->fbits, enc_period,(dcmotor->period_ref << dcmotor->fbits) / RefCurrVelocity, enc_period - (dcmotor->period_ref << dcmotor->fbits) / RefCurrVelocity );
        }
        else 
        {
#endif
            uint32_t edge;
            RefCurrVelocity = dcmotor->RefVel;

           
            enc_period = mot_enc_period_read(dcmotor, &edge);
            DBG_LOG_DEBUG("edge %d\n",edge);
            //enc_period = 0;
#ifdef HAVE_DEC_LASER
        }
#endif
        
        //if (dcmotor->motor_state == MOT_STATE_STEADY)
        {
            DBG_LOG_DEBUG("Encoder period is %d\n",enc_period);
        }
        if (dcmotor->motor_state == MOT_STATE_STEADY  && dcmotor->MeasuredPosTimeMinus1 == dcmotor->MeasuredPosTime0) 
        {
            DBG_LOG_DEBUG("enc didnt move %d\n", dcmotor->MeasuredPosTime0);
        }
    }
    else
    {
        /* Under debug no servo control, fake the encoder (be careful ...) */
        dcmotor->MeasuredPosTimeMinus1 = dcmotor->MeasuredPosTime0;
        dcmotor->MeasuredPosTime0      = dcmotor->RefPos;
    }

    /* Make the old 'current' state values the previous ones: time marches on */
    dcmotor->prev_velocity = dcmotor->curr_velocity;
    d_term = (dcmotor->CtrlPWM - dcmotor->prev_CtrlPWM );
    //d_term = dcmotor->CtrlPWM;
    dcmotor->prev_CtrlPWM  = dcmotor->CtrlPWM;

    /* Check for a position trigger
     *   REVISIT: this may be a problem if the encoder rolls over.  At this point only the ADF
     *   should cause a rollover (no stoppy copy) ... and that doesn't use position triggers.
     */
    if (dcmotor->mtr_trigger.trigger_type == TRIG_POSITION)
    {
        if ( ((dcmotor->move_direction == MOT_FORWARD) &&
             (dcmotor->MeasuredPosTime0 >= dcmotor->mtr_trigger.trigger_param1)) ||
             ((dcmotor->move_direction == MOT_REVERSE) &&
             (dcmotor->MeasuredPosTime0 <= dcmotor->mtr_trigger.trigger_param1)) )
        {
            /* Call the trigger function (always one shot, so clear it first) */
            dcmotor->mtr_trigger.trigger_type = TRIG_NULL;
            dcmotor->mtr_trigger.trigger_function_ptr(dcmotor);
        }
    }

    /* Calculate velocity and position error based on encoder ticks.
     *   Position error: + is too far behind, - is too far ahead of reference
     */
    if (dcmotor->move_direction == MOT_REVERSE)
    {
        /* Motor in reverse (decreasing encoder values). NOT supporting encoder rollover! */
        etCurVelocity = dcmotor->MeasuredPosTimeMinus1 -
                        dcmotor->MeasuredPosTime0;
        PosError = dcmotor->MeasuredPosTime0 - dcmotor->RefPos;
    }
    else
    {
        /* Motor in forward (increasing encoder values). NOT supporting encoder rollover! */
        etCurVelocity = dcmotor->MeasuredPosTime0 -
                        dcmotor->MeasuredPosTimeMinus1;
        if (enc_period > 0xff && etCurVelocity > 0) 
        {
            uint64_t tmp64int;
            tmp64int = ((((uint64_t)(dcmotor->period_ref* enc_period_dev))  << (dcmotor->fbits + 20))   / enc_period) >> 20;
            etCurVelocity = (uint32_t) tmp64int;
            if (period_error != 0) 
            {
                tmp64int = ((((uint64_t)(dcmotor->period_ref* enc_period_dev)) << (dcmotor->fbits+ 20))  / period_error) >> 20;
                RefCurrVelocity = (uint32_t) tmp64int;
                //DBG_LOG_ERR(" enc_period %x period_error %x diff %d\n",enc_period,period_error, enc_period - period_error );
                DBG_LOG_DEBUG("pwm %d etCurVelocity %x RefCurrVelocity %x diff %d\n",dcmotor->CtrlPWM >> dcmotor->fbits,etCurVelocity,RefCurrVelocity, RefCurrVelocity - etCurVelocity  );
            }
            else
            {
                if (RefCurrVelocity == 0) 
                {
                    RefCurrVelocity = etCurVelocity;
                }
                tmp64int = ((((uint64_t)(dcmotor->period_ref* enc_period_dev)) << (dcmotor->fbits+ 20))  / RefCurrVelocity) >> 20;
                DBG_LOG_DEBUG(" pwm %d pp enc_period %x dcmotor->period_ref %x diff %d\n",dcmotor->CtrlPWM >> dcmotor->fbits, enc_period,(uint32_t)tmp64int, enc_period - (uint32_t) tmp64int);
                DBG_LOG_DEBUG(" pwm %d etCurVelocity %x RefCurrVelocity %x diff %d\n",dcmotor->CtrlPWM >> dcmotor->fbits,etCurVelocity,RefCurrVelocity, RefCurrVelocity - etCurVelocity  );
            }
            //if (dcmotor->motor_state == MOT_STATE_STEADY) 
            {
                DBG_LOG_DEBUG(" etCurVelocity %d ref %d diff %d\n",etCurVelocity, (dcmotor->period_ref << dcmotor->fbits) / enc_period,etCurVelocity - (dcmotor->period_ref << dcmotor->fbits) / enc_period);
                DBG_LOG_DEBUG(" etCurVelocity %d ref %d diff %d\n",etCurVelocity, dcmotor->MeasuredPosTime0 - dcmotor->MeasuredPosTimeMinus1, etCurVelocity - (dcmotor->MeasuredPosTime0 - dcmotor->MeasuredPosTimeMinus1));
            }
        }
        else
        {
            if (etCurVelocity == 0)
                etCurVelocity = dcmotor->curr_velocity;
            period_error = 0;
            RefCurrVelocity = dcmotor->RefVel;
            DBG_LOG_DEBUG("  slow pwm %d etCurVelocity %x RefCurrVelocity %x diff %d\n",dcmotor->CtrlPWM >> dcmotor->fbits,etCurVelocity,RefCurrVelocity, RefCurrVelocity - etCurVelocity  );
        }
        dcmotor->calcPosTime0 += etCurVelocity;
        
        if (dcmotor->motor_state == MOT_STATE_STEADY) 
        {
            //DBG_LOG_ERR("etCurVelocity %d\n", etCurVelocity);
        }
        
        PosError = (dcmotor->RefPos + RefCurrVelocity) - dcmotor->calcPosTime0;
    }

    /* DEBUG ONLY: if we have motor safety turned off then limit the accumulated position
     * error, we don't want the integral winding up on us.
     */
    if ( (debug_disable_safety == TRUE) && (PosError > dcmotor->PosErrCap) )
    {
        PosError = dcmotor->PosErrCap;
    }

    /* A negative motor velocity is BAD, but it can happen as we are ramping or coming to a stop.
     * Clamp it to zero if it happens.
     */
    RawEtCurVelocity = etCurVelocity;
    if (etCurVelocity < 0)
    {
        motor_dbg2(( "WARN: Negative motor velocity\n"));
        etCurVelocity = 0;
    }

    /* Move count represents the total distance moved; add in the number of encoder ticks
     * that have passed this sample period (same as the encoder tick based velocity value)
     */
    dcmotor->MoveCount += etCurVelocity;

    /* Save the current velocity in the motor state */
    dcmotor->curr_velocity = etCurVelocity;
    etCurrAccel =  dcmotor->curr_velocity - dcmotor->prev_velocity;

    /* Calculate velocity error (may go negative)
     *   Velocity error: + is too slow, - is to fast compared to reference
     */
    VelError = RefCurrVelocity - etCurVelocity;
    //if ((VelError > 0 && period_error > 0) || (VelError < 0 && period_error < 0)) 
    //{
    //    dbg_printf("dcmotor->RefVel %d VelError %d\n",dcmotor->RefVel, VelError>>2);
    //    dcmotor->RefVel += VelError >> 2;
    //}

#define VEL_CLOSE_ENOUGH1 5000
#define VEL_CLOSE_ENOUGH2 1000
    if (dcmotor->motor_use == laser_drive_motor) 
    {
        if (VelError > 0 ) 
        {
            if (VelError < VEL_CLOSE_ENOUGH2) 
            {
                VelError *= 4;
            }
            else if (VelError < VEL_CLOSE_ENOUGH1) 
            {
                VelError *= 4;
            }
            
            //VelError >>= 1;
        }
        else if (VelError < 0) 
        {
            if (VelError > -VEL_CLOSE_ENOUGH2) 
            {
                VelError *= 4;
            }
            else if (VelError > -VEL_CLOSE_ENOUGH1) 
            {
                VelError *= 4;
            }
        }
    }

    deltaVelError = VelError - dcmotor->prev_vel_error;
    DBG_LOG_DEBUG("deltaVelError %d \n",deltaVelError);
#define VEL_SMALL_AMOUNT 200
#if 0
    if (dcmotor->motor_state == MOT_STATE_STEADY)
    {
        if ((dcmotor->motor_use == laser_drive_motor) && ((VelError > dcmotor->prev_vel_error && (dcmotor->prev_vel_error < VEL_SMALL_AMOUNT)) || (VelError < dcmotor->prev_vel_error && (dcmotor->prev_vel_error > -VEL_SMALL_AMOUNT))))
        {
            
            if (deltaVelError > 0) 
            {
                deltaVelError *= -48/2;
            }
            else
            {
                deltaVelError *= -48/2;
            }
        }
    }
#endif
    dcmotor->prev_vel_error = VelError; 
    //dbg_printf("etCurVelocity %d VelError %d\n",etCurVelocity, VelError);
    /* If our position error is positive then we are behind, which probably means we are
     * on the way to being stuck or up against the wall.
     */
    if ( (debug_disable_safety == FALSE) &&
         (dcmotor->motor_state != MOT_STATE_COAST) &&
         (PosError > dcmotor->move_params->pos_error_limit) )
    {
        DBG_ERR("Setting to Coast\n");
        /* Too much error -- we're ramming into a wall or got stuck, so stop. */
        motor_dbg1(( "WARNING %ld > %ld\n", PosError, dcmotor->move_params->pos_error_limit ));

        /* Get PWM off the motor as soon as possible */
        mot_no_pwm(dcmotor);

        /* Transition to coast state to let motor come to a stop (skip any decel) */
        mot_set_motor_state(dcmotor, MOT_STATE_COAST);

        /* If stall OCC function is enabled, call it now */
#ifdef DO_OCC
        make_occ_call(dcmotor, OCC_MTR_STALL);
#endif

        motor_dbg1(( "%8s=0x%08lX ", "etCurVelocity", etCurVelocity));
        motor_dbg1(( "PosError:%d MeasuredPosTime0:%ld MeasuredPosTimeMinus1:%ld\n",
                        PosError, dcmotor->MeasuredPosTime0, dcmotor->MeasuredPosTimeMinus1));
        motor_dbg1(( "RefVel:%d \n", dcmotor->RefVel));
        motor_dbg1(( "RefPos:%d \n", dcmotor->RefPos));
    }

    switch (dcmotor->motor_state)
    {
        case MOT_STATE_ACCEL:
             /* Accelerate until our reference velocity is equal to the target velocity. */
             
             if (dcmotor->RefVel == dcmotor->move_params->speed)
             {
                 /* Reference velocity reached the target, so stop increasing it.
                  * Now we must wait until the actual velocity reaches the target.
                  */
                 //if (dcmotor->laser && (dcmotor->current_speed->index == 2))
                 //{
                 //    dcmotor->RefVel = (dcmotor->period_ref << dcmotor->fbits) / (dec_laser_target_period(dcmotor->laser) / 18);
                 //}
                 DBG_ERR("TO MOT_STATE_WAIT_RAMP\n");
                 mot_set_motor_state (dcmotor, MOT_STATE_WAIT_RAMP );
             }
             else
             {
                 /* Still accelerating. Calculate new reference velocity based on acceleration,
                  * don't overshoot our target.
                  */
                 dcmotor->RefVel += dcmotor->move_params->acceleration;
                 dcmotor->RefVel = MIN(dcmotor->RefVel,dcmotor->move_params->speed);
                 dcmotor->RefBias += dcmotor->move_params->bias_accel;
                 dcmotor->RefBias = MIN(dcmotor->RefBias,dcmotor->move_params->bias);
                 DBG_LOG_DEBUG("RefBias %x bias_accel %x fbias %d\n", dcmotor->RefBias, dcmotor->move_params->bias_accel,dcmotor->RefBias >> dcmotor->fbits);
                 /* Keep track of how long it's taking to ramp up. */
                 //motors[motor_id].rampCounts += motors[motor_id].RefVel;
                 dcmotor->rampCounts += RawEtCurVelocity;
             }
            break;

        case MOT_STATE_WAIT_RAMP:
            /* At this point, our relative velocity is equal to the final velocity but our
             * actual velocity probably isn't. Stay in this state until the actual velocity
             * reaches the target velocity.
             */

            /* Continue keeping track of how long it's taking to ramp up */
            //motors[motor_id].rampCounts += motors[motor_id].RefVel;
            dcmotor->rampCounts += RawEtCurVelocity;
            
            if (etCurVelocity >= dcmotor->RefVel)
            {
                /* Motor has reached the target velocity */
                DBG_ERR("TO MOT_STATE_STEADY\n");
                
                mot_set_motor_state (dcmotor, MOT_STATE_STEADY );
            }
            break;

        case MOT_STATE_STEADY:
            /* In this state, we've reached our final speed and are moving at what
             * is ideally a constant velocity. There's not much to do in this state
             * except wait for our deceleration point.
             */

            /* Notes on ramp-down:
             * We are having problems with accurate positioning of the motor. The
             * problem is with slow moves: the ramp up takes longer than the ramp
             * down so we cannot just use the ramp up distance for a ramp down. This
             * will cause us to stop too soon.  The current implementation sets a
             * trigger point before the stop point and transitions to a slower move
             * speed: the motor can then stop without a ramp down.
             * BUT: the sudden drop of PWM to zero can cause RFI problems so we will
             *   still go through a simple decel.
             */

            /* Have we reached our target position (simple decel version)? */
            if ( !dcmotor->RunForever && dcmotor->MoveCount >= dcmotor->MoveDistance )
            {
                /* Go straight to coast (removes PWM right away) */
                mot_set_motor_state(dcmotor, MOT_STATE_COAST);
            }

#if 0
            /* 'Classic' ramp down ... disabled for now */
            /* Determine when to start decelerating based on how long it took to ramp up.
             * We're doing this here rather than in the WAIT_RAMP state because the target
             * position can change if we have to stop mid-scan (due to running out of buffers).
             */
            if ( (dcmotor->MoveDistance <= dcmotor->rampCounts) ||
                 (dcmotor->MoveCount >= (dcmotor->MoveDistance - dcmotor->rampCounts)))
            {
                motor_dbg1(( "Reached stop pos.\n" ));
                mot_set_motor_state(dcmotor, MOT_STATE_DECEL );
            }
#endif
            break;

        case MOT_STATE_DECEL:
            /* In this state we wait for the reference velocity to reach 0 */
            if (dcmotor->RefVel == 0)
            {
                /* Reference velocity is 0. Now we must wait for the actual motor
                 * speed to reach 0.
                 */
                mot_set_motor_state(dcmotor, MOT_STATE_COAST );
            }
            else
            {
                /* Still decelerating, don't undershoot */
                dcmotor->RefVel -= dcmotor->move_params->acceleration;
                dcmotor->RefVel = MAX(dcmotor->RefVel,0);

                /* Keep track of distance travelled while decelerating */
                dcmotor->decelCounts += RawEtCurVelocity;
            }
            break;

        case MOT_STATE_COAST:
            /* At this point the reference velocity has reached 0, and we're waiting
             * for the actual motor velocity to reach 0. Note that it is not uncommon
             * for the system to 'rebound' a bit after stopping, so lets get more than
             * one sample at zero velocity.
             */
            dcmotor->coastCounts += RawEtCurVelocity;

            /* Note we are using the raw velocity here: want to wait for any negative velocity
             * to settle as well.
             */
            if (RawEtCurVelocity == 0)
            {
                if (++dcmotor->stopCounts >= STOP_COUNTS)
                {
                    /* Motor has really stopped moving, so we're done */
                    mot_dc_halt(dcmotor);
                    return;
                }
            }
            else
            {
                /* We moved, reset stop counter */
                dcmotor->stopCounts = 0;
            }
            break;

        default:
            break;
    }

    /* Calculate our PWM output */
    if (dcmotor->motor_state == MOT_STATE_COAST)
    {
        /* Don't want PWM applied while coasting */
        dcmotor->CtrlPWM = 0;
        CtrlPWM = 0;
    }
    else
    {
        /* Calculate the PI terms.  Note that I kicked the derivative term off
         * the island (it caused more trouble than it was worth).
         * WARNING: the error terms can and will go negative, but they may also get
         *   huge and rollover our signed 32 bit integers.  We must remove some of
         *   the scaling to reduce rollover ... but we also lose precision.
         */
        if (pid_debug == FALSE)
        {
            const uint32_t halfbits1 = dcmotor->fbits - 4;
            const uint32_t halfbits2 = dcmotor->fbits - halfbits1;
            
            p_term = (dcmotor->move_params->kp >> dcmotor->fbits) * VelError;
            
            while ((VelError > 0 && p_term < 0) ||  (VelError < 0 && p_term > 0)) 
            {
                VelError /= 2;
                p_term = (dcmotor->move_params->kp >> dcmotor->fbits) * VelError;
            }
            PrevSum = dcmotor->sum_error;
#define MAX_ERROR ((int32_t)(1<< dcmotor->fbits ))
            if (abs(VelError) < (MAX_ERROR)) 
            {
                if (VelError > 0) 
                {
                    //dcmotor->sum_error += (VelError << 0) /** dcmotor->mtr_connects->sample_period*/;
                }
                dcmotor->sum_error += (VelError ) /** dcmotor->mtr_connects->sample_period*/;
                ErrorTerm = VelError;
                    
            }
            else if (VelError < 0) 
            {
                dcmotor->sum_error -= MAX_ERROR;
                ErrorTerm = -MAX_ERROR;
            }
            else
            {
                dcmotor->sum_error += MAX_ERROR;
                ErrorTerm = MAX_ERROR;
            }
            ASSERT(dcmotor->move_params->ki >= 0);
            i_term = (dcmotor->move_params->ki >> halfbits1) * (dcmotor->sum_error >> halfbits2 ); 
            //i_term = (dcmotor->move_params->ki >> dcmotor->fbits) * PosError ;

            //d_term = (dcmotor->move_params->kd >> dcmotor->fbits)  * (etCurVelocity - etCurrAccel);
            
            if (dcmotor->motor_state == MOT_STATE_STEADY) 
            {
                uint64_t tmp64int;
                ASSERT(dcmotor->move_params->kd >= 0);
                
                if (deltaVelError >= 0) 
                {
                    tmp64int =((uint64_t)(dcmotor->move_params->kd >> halfbits1) * (uint64_t)deltaVelError)  >> halfbits2 ;
                    d_term = tmp64int;
                }
                else
                {
                    tmp64int =((uint64_t)(dcmotor->move_params->kd >> halfbits1) * (uint64_t)(-deltaVelError))  >> halfbits2 ;
                    d_term = tmp64int;
                    d_term = -d_term;
                }
                
                //d_term = ((dcmotor->move_params->kd >> halfbits1)  * (deltaVelError >> halfbits2)/*/ dcmotor->mtr_connects->sample_period*/) ;
                if (d_term > 0) 
                {
                    //d_term *=3;
                    //d_term >>= 1;
                }
                //d_term = (dcmotor->move_params->kd >> dcmotor->fbits)  * (( 3 * etCurVelocity  + dcmotor->prev_velocity - (etCurrAccel )) >> 2);
                //d_term  = -((dcmotor->move_params->kd >> dcmotor->fbits) * etCurrAccel); 
                DBG_LOG_DEBUG("p_term %d i_term %d d_term %d\n", p_term, i_term, d_term);
                DBG_LOG_DEBUG("deltaVelError %d HB1 %d, HB2 %d, kd %d, dterm %d\n",deltaVelError,halfbits1,halfbits2,dcmotor->move_params->kd,d_term);
            }
            else
            {
                //d_term = (dcmotor->move_params->kd >> dcmotor->fbits)  * (etCurVelocity);
                d_term = ((dcmotor->move_params->kd >> halfbits1)  * (deltaVelError >> halfbits2 /*/ dcmotor->mtr_connects->sample_period*/));
                //d_term = 0;
                DBG_LOG_DEBUG("p_term %d i_term %d d_term %d VelError %d etCurrAccel %d\n", p_term, i_term, d_term, VelError,etCurrAccel);
            }
            //DBG_LOG_ERR("p_term %d i_term %d d_term %d VelError %d etCurrAccel %d\n", p_term, i_term, d_term, VelError,etCurrAccel);
            //d_term = dcmotor->CtrlPWM;
        }
        else
        {
            /* If pid values have been changed by the serial command line, then use those
             * values. This is only for testing and motor tuning.
             */
            p_term = debug_kp * (VelError >> dcmotor->fbits);
            i_term = debug_ki * (PosError >> dcmotor->fbits);
            d_term = debug_kd;
        }
        if (dcmotor->motor_state == MOT_STATE_STEADY && /*dcmotor->motor_use == laser_drive_motor*/ false)
        {
            int32_t num,denom;
            int curr_index = dcmotor->error_filter_pos;
            num = ((VelError + (2<<dcmotor->fbits) + dcmotor->error_filter[(curr_index) % ERROR_FILTER_SIZE])/4) * dcmotor->error_filter[(curr_index + 9) % ERROR_FILTER_SIZE];
            num *= ((1 << dcmotor->fbits)  
                    -(16  * dcmotor->error_filter[(curr_index    ) % ERROR_FILTER_SIZE]/10) 
                    + (78 * dcmotor->error_filter[(curr_index + 1) % ERROR_FILTER_SIZE]/100)
                    );
            num *= 32;
            denom = (1<<dcmotor->fbits) - ((VelError + (2<<dcmotor->fbits) + dcmotor->error_filter[(curr_index) % ERROR_FILTER_SIZE])/4) * dcmotor->error_filter[(curr_index + 9) % ERROR_FILTER_SIZE];
            denom *= (  
                      -(97  * dcmotor->error_filter[(curr_index + 2) % ERROR_FILTER_SIZE]/1000) 
                      - (85 * dcmotor->error_filter[(curr_index + 3) % ERROR_FILTER_SIZE]/1000)
                    );
            DBG_LOG_DEBUG("num %d, denom %d\n",num,denom);
            r_term = 4 *((num/denom)<<dcmotor->fbits);
            dcmotor->error_filter_pos = dcmotor->error_filter_pos == 0 ? ERROR_FILTER_SIZE - 1: dcmotor->error_filter_pos - 1;
            dcmotor->error_filter[dcmotor->error_filter_pos] = VelError;

        }
        else
        {
            dcmotor->error_filter_pos = dcmotor->error_filter_pos == 0 ? ERROR_FILTER_SIZE - 1: dcmotor->error_filter_pos - 1;
            dcmotor->error_filter[dcmotor->error_filter_pos] = VelError;
            r_term = 0;
        }
         
        if (debug_disable_servo == FALSE)
        {
            dcmotor->CtrlPWM = (dcmotor->RefBias + p_term + i_term + d_term + r_term);
            if (dcmotor->motor_use == laser_drive_motor) 
            {
                DBG_LOG_DEBUG("ET %d RB %d pt %d it %d dt %d\n",VelError,dcmotor->RefBias>> dcmotor->fbits, p_term>> dcmotor->fbits, i_term>> dcmotor->fbits, d_term>> dcmotor->fbits/*, r_term >>dcmotor->fbits*/);
            }
            if (dcmotor->CtrlPWM < 0 && p_term > 0) //int math rolled.  fix it.
            {
               dcmotor->CtrlPWM = 0xeffffff; 
            }
        }
        else
        {
            /* In no servo debug mode the error terms will go to zero, which will drive
             * the PWM to zero.  Just hold the last known value.
             */
            dcmotor->CtrlPWM = dcmotor->prev_CtrlPWM;
        }

        /* Account for the scaling we do to the coefficient values (velocity/error coefficients
         * are taken out above)
         */
        CtrlPWM = dcmotor->CtrlPWM >> dcmotor->fbits;

        /* Not coasting, so update our reference position
         * DEBUG ONLY: if we have motor safety turned off then limit the accumulated
         * reference position
         */
        if ( (debug_disable_safety == FALSE) ||
             ((debug_disable_safety == TRUE) && (PosError < dcmotor->PosErrCap)) )
        {
            if (dcmotor->move_direction == MOT_REVERSE)
            {
                dcmotor->RefPos -= dcmotor->RefVel;
            }
            else
            {
                dcmotor->RefPos += RefCurrVelocity;
            }
        }
    }


    /* Clamp the PWM to the range specified for this motor mode.  Note that we have
     * two limits (we must choose the smaller of):
     *  - a maximum determined by the pwm frequency (100% duty cycle)
     *  - one dynamically calculated based on the our velocity and specific motor mode
     *    constants.
     */
    PWMLimit = MIN(dcmotor->pwm_period,
                   calc_maxPwm(dcmotor, etCurVelocity) );
    //dbg_printf("CtrlPWM %d PWMLimit %d\n",CtrlPWM,PWMLimit);
    if (CtrlPWM > PWMLimit)
    {
        CtrlPWM = PWMLimit;
        dcmotor->CtrlPWM = CtrlPWM << dcmotor->fbits;
        dcmotor->sum_error -= ErrorTerm; //saturated don't increase error term
    }
    else if (CtrlPWM < 0)
    {
        DBG_LOG_ERR("p_term %d i_term %d d_term %d VelError %d \n", p_term, i_term, d_term, VelError);
        DBG_LOG_ERR("CtrlPWM < 0 %d\n", CtrlPWM);
        dcmotor->sum_error -= ErrorTerm;//saturated don't increase error term
        dcmotor->CtrlPWM = CtrlPWM = dcmotor->prev_CtrlPWM / 4;
    }
    //dbg_printf("CtrlPWM %d\n",CtrlPWM);
    /* Set the new PWM */
    //if (dcmotor->motor_state == MOT_STATE_STEADY) 
            //{
                DBG_LOG_DEBUG("CtrlPWM %d\n", CtrlPWM);
            //}
    mot_pwm_control(dcmotor, abs(CtrlPWM), true);

    /* Now that we have our PWM applied, look at the motor load and see if any
     * OCC events need to be triggered.
     */
#ifdef DO_OCC //TURNED OFF FOR NOW
    do_load_occ_processing(dcmotor);
#endif

    /* Log some more motor run time */
#ifdef TRACK_DCMOTOR_DUTYCYCLE
    duty_add_usage_nsec(dcmotor, (dcmotor->mtr_connects->sample_period * NANOSECS_PER_MILLISEC));
#endif

}

/**
 *  \brief (Helper) Scan motor ISR
 *
 *  \param[in] intRegMask  Motor ID (set when we did intAttach)
 **/
static void dcmotor_interrupt(int32_t interrupt_count, void *context)
{
    uint32_t     isr_pending;
    uint32_t     isr_enabled;
    //static int     enc_edge = 0;
    uint32_t     begin_isr_usec;
    uint32_t     end_isr_usec;
    uint32_t     isr_dur_usec;
    dcmotor_t * dcmotor = (dcmotor_t *) context;

    /* Sanity check on the motor id */
    if (!valid_dcmotor_ptr(dcmotor))
    {
        motor_dbg1(("%s: Invalid motor ptr (%x)\n", __FUNCTION__, dcmotor));
        ASSERT(0);
        return;
    }

    /* At this point we MUST have the motor registers configured */
    XASSERT(dcmotor->motor_regs != NULL, (uint32_t)dcmotor->motor_regs);

    /* Get ISR start time (for debug) */
    begin_isr_usec = timer_get_time_usec();

    /* Check to see what we have enabled and what is pending */
    isr_pending = dcmotor->motor_regs->IPEND;
    isr_enabled = dcmotor->motor_regs->IEN;

    if ((isr_enabled & IEN_TBASE_MASK) && (isr_pending & IPEND_TBASE_MASK))
    {
        dcmotor->motor_regs->IACK |= IACK_TBASE_MASK;

        /* Call the control output function every 2 ms */
        ++dcmotor->tick;
        
        if (dcmotor->tick >= dcmotor->mtr_connects->sample_period  /** 10*/  )
        {
            dcmotor->tick = 0;
            if (dcmotor->motor_state != MOT_STATE_STEADY) 
            {
                mot_dc_update_control_output(dcmotor);
            }
            else if ((dcmotor->motor_regs->IEN & DCMOTOR_BASE_IEN_ENC_MASK) == 0)
            {
                mot_dc_update_control_output(dcmotor);
            }
            else
            {
                if (adc_24v /*&& dcmotor->tick % 2*/)
                {
                    if (dcmotor->pwm_period > 0 && dcmotor->CtrlPWM > 0)
                    {
                        //mot_pwm_control(dcmotor,dcmotor->CtrlPWM >> dcmotor->fbits, true);
                    }
                }
            }
            
        }
        else if (adc_24v /*&& ((dcmotor->tick % 2) == 1)*/)
        {
            if (dcmotor->pwm_period > 0 && dcmotor->CtrlPWM > 0 && (dcmotor->motor_regs->IEN & DCMOTOR_BASE_IEN_ENC_MASK) == 0)
            {
                mot_pwm_control(dcmotor,dcmotor->CtrlPWM >> dcmotor->fbits, true);
            }
        }

#ifdef VERIFY_TIMEBASE
        /* Wiggle a gpio so we can verify the timebase with a scope */
        gpio_write( tb_gpio_pin, tick[dcmotor->motor_id] );
#endif
    }

    if ((isr_enabled & IEN_ENC_MASK) && (isr_pending & IPEND_ENC_MASK))
    {
        dcmotor->tick = 0;
        dcmotor->motor_regs->IACK = IACK_ENC_MASK;
        mot_dc_update_control_output(dcmotor);
        /* Not using encoder edge IRQ at this point */
        //++enc_edge;
    }

    if ((isr_enabled & IEN_IENC_MASK) && (isr_pending & IPEND_IENC_MASK))
    {
        dcmotor->motor_regs->IACK = IACK_IENC_MASK;
        motor_dbg1(( "ILLEGAL_ENCODER_STATE isr\n"));

        /* Only assert on an illegal encoder state if the motor is running at
         * steady state: it is possible for the motor to back up a bit during
         * ramp up and coast/stops.
         */
        XASSERT(dcmotor->motor_state != MOT_STATE_STEADY, isr_pending);
    }

    if ((isr_enabled & IEN_IPER_MASK) && (isr_pending & IPEND_IPER_MASK))
    {
        dcmotor->motor_regs->IACK = IACK_IPER_MASK;
        motor_dbg1(("INPUT_PERIOD_RANGE isr\n"));
        ASSERT(0);
    }

    if ((isr_enabled & IEN_FILR_MASK) && (isr_pending & IPEND_FILR_MASK))
    {
        dcmotor->motor_regs->IACK = IACK_FILR_MASK;
        motor_dbg1(( "FILTER_RANGE isr\n"));
        ASSERT(0);
    }

    if ((isr_enabled & IEN_OUTR_MASK) && (isr_pending & IPEND_OUTR_MASK))
    {
        dcmotor->motor_regs->IACK = IACK_OUTR_MASK;
        motor_dbg1(("OUTPUT_PERIOD_RANGE isr\n"));
        ASSERT(0);
    }

    if ((isr_enabled & IEN_ZPOS_MASK) && (isr_pending & IPEND_ZPOS_MASK))
    {
        dcmotor->motor_regs->IACK = IACK_ZPOS_MASK;
        motor_dbg1(("ZERO_POSITION isr\n"));
        ASSERT(0);
    }

    /* Get ISR end time (for debug).  Log the ISR time, ignoring the cases where
     * the timer rolls over.
     */
    end_isr_usec = timer_get_time_usec();
    if (end_isr_usec > begin_isr_usec)
    {
        isr_dur_usec = end_isr_usec - begin_isr_usec;

        /* Only log the big ones for now ... */
        if (isr_dur_usec > ISR_DUR_LOG_THRESH)
        {
//            LOG(LOG_SMOT_ISR_DUR, isr_dur_usec);
        }
    }
}
void dcmotor_create_motor(dcmotor_t * dcmotor,
                          const motor_connect_t *mtr_connects,
                          const motor_const_t   *mtr_constants,
                          UINT8                  enc_scale_fbits)
{
    volatile MOTOR_REGS *motorRegs;

    /* Brief sanity checks */
    XASSERT( mtr_constants != NULL, (uint32_t)mtr_constants );
    XASSERT( mtr_connects != NULL,  (uint32_t)mtr_connects );

    /* Save pointers to the provided motor parameters */
    dcmotor->mtr_connects = mtr_connects;
    dcmotor->mtr_consts = mtr_constants;
    dcmotor->cookie = DCMOTOR_COOKIE;

    /* add slp cmds if NULL*/
    if (dcmotor->smot_dc_enter_sleep_func == NULL) 
    {
        dbg1(("%s: new motor_id=%d using default enter sleep func\n", 
             __FUNCTION__, dcmotor->motor_id));
        dcmotor->smot_dc_enter_sleep_func = smot_dc_enter_sleep;
    }
    if (dcmotor->smot_dc_exit_sleep_func == NULL) 
    {
        dbg1(("%s: new motor_id=%d using default exit sleep func\n", 
             __FUNCTION__, dcmotor->motor_id));
        dcmotor->smot_dc_exit_sleep_func = smot_dc_exit_sleep;
    }
    
    /* Set the motor base address and IRQ number based on the block number in
     * the motor connections.
     */
    mot_set_blk_addr_irq(dcmotor);

    dbg1(("%s: new motor_id=%d at base_addr=0x%08lx\n", 
             __FUNCTION__, dcmotor->motor_id, dcmotor->motor_regs));

    /* Set some default motor state */
    dcmotor->motor_state    = MOT_STATE_STOP;
    dcmotor->move_direction = MOT_FORWARD;
    dcmotor->fbits          = enc_scale_fbits;

    /* Set up the scan motor interrupt handler (but leave IRQ disabled) */
    uio_int_disable(dcmotor->uio_dev);
    uio_int_attach(dcmotor->uio_dev, 0, dcmotor_interrupt, (void*)dcmotor);

    /* Reset all OCC errors (not enabled, not sent) */
    reset_occ_state(dcmotor);

    /* Reset our rolling average load and target value */
    avg_ld_reset(dcmotor);
    dcmotor->NoLdTarget = MTR_LD_BAD_AVE;
    dcmotor->mtr_load.LdAvgSettleLimit = 0;

    /* Reset our motor duty cycle data */
#ifdef TRACK_DCMOTOR_DUTYCYCLE
    duty_reset(dcmotor);
#endif
    /* No callback registered yet */
    dcmotor->MechOCCFunc = NULL;

    /* Make an easy pointer to the registers for use below */
    motorRegs = dcmotor->motor_regs;

    /* Disable all interrupts */
    mot_int_disable(dcmotor);

    /*
     * Set default values for h/w encoder debounce.  We will set these later for
     * each specific move, but for now we need good (non-zero) defaults.
     */
    mot_set_default_enc_debounce(dcmotor, ENC_RESET_TIME_USEC);

    /* Reset the encoder block */
    set_mtr6pin_soft_reset( motorRegs, 1 );
    set_mtr6pin_soft_reset( motorRegs, 0 );

    /* A very short delay to make sure the encoder block debounce is stable. */
    posix_sleep_us(ENC_RESET_TIME_USEC);

    /* Set up PWM defaults */
    /* calculate pwm period and divisor for the target frequency */
    calc_mot_pwm_period( DC_PWM_FREQ,
                         &dcmotor->pwm_period,
                         &dcmotor->pwm_divisor);
    dcmotor->prev_pwm_period = dcmotor->pwm_period;
    
    dbg1(("%s: pwm_period (max pwm): %d\n", __FUNCTION__, dcmotor->pwm_period));

    /* Set motor block pwm period, divisor and delay to their correct values */
    set_mtr6pin_pwm_divisor( motorRegs, dcmotor->pwm_divisor );
    set_mtr6pin_pwm_period( motorRegs,  dcmotor->pwm_period );
    set_mtr6pin_pwm_delay( motorRegs,   0 );

    dcmotor->PosErrCap = 0;

    /* Configure the PWM output method (quick copy of mtr conect value) */
    dcmotor->pwm_method = dcmotor->mtr_connects->pwm_type;

    /* The mech code sets up the coordinate system such that MOT_FORWARD moves the
     * motor away from the origin and MOT_REVERSE moves the motor towards the origin.
     * Sometimes we need to swap the motor directions to make this happen. The motor
     * connect table will handle this.
     */

    /* Set the base motor config register: this call will leave the motor disabled */
    set_mtr6pin_motorconfig_initialize( motorRegs, dcmotor->mtr_connects );

    /* The mech code sets up the coordinate system such that MOT_FORWARD
     * results in increasing encoder counts, and MOT_REVERSE results in
     * decreasing counts.  Sometimes we need to swap the encoder signals to
     * make this happen.
     */

    /* Configure the encoder register. Note that the motor code is crafted around a 
     * couple of key assumptions:
     *  - the servo control loop runs every 2ms (two ticks of a 1ms motor IRQ)
     *  - encoders are configured to give quad counts
     */
    set_mtr6pin_encoderconfig_initialize( motorRegs,
                                          dcmotor->mtr_connects,
                                          /*MTR6PIN_TBSEL_100_US*/ MTR6PIN_TBSEL_1_MS );

    /* Disable row sync pulses */
    mot_set_row_sync( dcmotor, SYNC_DISABLE, 0 );

    /* Set a default encoder position in the middle of our range, we have no idea which
     * way the mech code will go right out of the gate.
     */
    mot_dc_set_mid_position(dcmotor);
}

static void dcmotor_irq_post_semaphore_function_status( dcmotor_t * dcmotor ) 
{
    dcmotor->current_speed = dcmotor->target_speed;
    sem_post(&dcmotor->semaphore);
}

uint32_t dcmotor_wait(dcmotor_t *dcmotor, uint32_t timeout_msec)
{
    if (dcmotor)
    {
        return posix_sem_timedwait_ms(&dcmotor->semaphore, timeout_msec);
    }
    return -1;
}

void dcmotor_run( dcmotor_t *dcmotor, dcmotor_speed_t *speed )
{
    DBG_ASSERT(dcmotor && speed);

    if (speed->index == 0) // off case.
    {
        mot_dc_halt(dcmotor);
        (*dcmotor->smot_dc_enter_sleep_func)(dcmotor);
        ASSERT(speed->index == 0 );
        dcmotor->target_speed = dcmotor->current_speed = speed;
        return;
    }
    if (dcmotor->current_speed == speed)
    {
        return; // nothing to do: 
    }

    if (speed->index != 0) // not stopped
    {
        if (dcmotor->motor_state != MOT_STATE_STOP)
        {
            mot_int_disable(dcmotor);
        }
        else
        {
            (*dcmotor->smot_dc_exit_sleep_func)(dcmotor);
        }
        dcmotor->target_speed = speed;
        smot_dc_set_motor_move_params (dcmotor, speed->move_params[dcmotor->motor_id]);
        smot_add_trigger(dcmotor, TRIG_STATE, MOT_STATE_STEADY, dcmotor_irq_post_semaphore_function_status);
        smot_dc_run(dcmotor);
    }
    else // off case.
    {
        mot_dc_halt(dcmotor);
        (*dcmotor->smot_dc_enter_sleep_func)(dcmotor);
        ASSERT(speed->index == 0 );
        dcmotor->target_speed = dcmotor->current_speed = speed;
    }
}


uint32_t dcmotor_speed_msecs_to_ramp(dcmotor_t * dcmotor, dcmotor_speed_t * speed)
{
    return speed->eta[dcmotor->motor_id];
}


void dcmotor_init()
{   
    int err;
    dcmotor_t * dcmotor;

    dcmotor_local_init();
    dcmotor = dcmotor_by_index(0);
    
    dcmotor->target_speed = dcmotor->current_speed = dcmotor_speed_by_name("speed_off");
    err = sem_init(&dcmotor->semaphore, 0, 0);
    XASSERT(err == 0, errno);

    dcmotor = dcmotor_by_index(1);
    if (dcmotor) 
    {   
        dcmotor->target_speed = dcmotor->current_speed = dcmotor_speed_by_name("speed_off");
        err = sem_init(&dcmotor->semaphore, 0, 0);
        XASSERT(err == 0, errno);

        adc_24v = dec_adc_by_name("24v power");
        if(adc_24v != NULL)
        {
            basic_line_24v = dec_adc_raw_read_channel(adc_24v);
        }
    }
    else
    {
        adc_24v = NULL;
        basic_line_24v = 0;
    }
    
    
#ifdef HAVE_CMD
    REL_ASSERT( CMD_OK == cmd_register( "dcmotor",
                                        NULL,
                                        NULL,
                                        NULL,
                                        dcmotor_dbcallback ) );
#endif
}


#ifdef HAVE_CMD

static int dcmotor_dbcallback( int argc, char *argv[] )
{
    int dcmotor_id; 
    uint32_t speed_id;
    dcmotor_t * dcmotor;
    dcmotor_speed_t * speed;

    int i;
    const char *str;


    if (argc < 3 )
    {
        dbg_printf("debug:dcmotor dcmotor_number_0->n 0 (stop) 2 (full speed) \n");

        
        for (i = 0, str = dcmotor_name_by_index(i); 
             str ; 
             i++, str = dcmotor_name_by_index(i) )
        {
            dbg_printf(" dcmotor [%d] %s\n", i, str);
        }
        
        /* not enough parameters */
        return 0;
    }

    dcmotor_id = atoi(argv[1]);
    speed_id = atoi(argv[2]);
    
    dcmotor = dcmotor_by_index(dcmotor_id);
    speed = dcmotor_speed_by_index(speed_id);

    if (( dcmotor == NULL) ||( speed == NULL)) 
    {
        dbg_printf("dcmotor dcmotor_number_0->n 0 (stop) 2 (full speed) \n");
        return 0;
    }
    if (speed->index == 0) 
    {
        dcmotor_run(dcmotor, speed);
        dbg_printf("dcmotor stopped");
        return 0;
    }
    else
    {
        uint32_t timeout = dcmotor_speed_msecs_to_ramp(dcmotor, speed) + 100; // msecs
        DBG_LOG_ERR("dbg dcmotor start\n");
        dcmotor_run(dcmotor, speed);
        dbg_printf("dcmotor started waiting %d mSec\n", timeout);
        if (dcmotor_wait(dcmotor, timeout ) != 0)
        {
            DBG_LOG_ERR("dbg dcmotor timed out %d \n", timeout );
            dbg_printf("dcmotor timed out\n");
        }
        else
        {
            DBG_LOG_ERR("dbg dcmotor done\n");
                
            dbg_printf("dcmotor at speed \n");
        }

    }
            
    return 0;
}
#endif // HAVE_CMD
