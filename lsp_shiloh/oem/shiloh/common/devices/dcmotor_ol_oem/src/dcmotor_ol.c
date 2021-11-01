/*
 * ======================================================================
 * Copyright (c) 2011-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

/// \file dcmotor.c

#include <stdint.h>
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
#include "asic.h"
#include "dcmotor_ol.h"
#include "dcmotor_ol_priv.h"
#include "mtr6pin.h"        /* Macros/declares to support DCMOTOR_BASE2 motor block */
#include "spec.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
static int dcmotor_dbcallback( int argc, char *argv[] );
#endif
/* The following globals are used for serial debugging commands, but leave them in
 * even in release builds (don't want ifdefs all through the servo code).
 */
static BOOL       debug_disable_pwm_wdog = FALSE;

/* The ISR duration logs become meaningless with so many entries, we are usually
 * looking for just the longer durations.  The following define will set a threshold
 * for when the ISR duration gets logged (in usec).
 */
#define ISR_DUR_LOG_THRESH    5

enum{
	NORMAL_PAPER_MODE,
	RECYCLED_PAPER_MODE
};

int     current_duty=700;
int     controlcount=0;
int     accelcount=0;
uint32_t     current_pps=0;
int     pps_error=0;
int     before_pps_error=0;
int     diff_pps_error=0;
float		I_err=0;
float		D_err=0;
int     fdc_pid=0;
int     fpidcontrol=0;
float     Kp=0;
float     Ki=0;
float     Kd=0;
float     dt=0;
int     target_pps=0;
int     target_pps_Start=0;
int     target_pps_final=0;
int     accel_pps=0;
uint32_t paper_enc;
int count = 0;

char lowspeedstart=0;
char RecycledPaperMode=0;
int MotorStallWarningCnt=0;

//gpio_handle_t *GPIO_E17;  //test pin
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
static BOOL valid_dcmotor_ptr(dcmotor_ol_t * dcmotor)
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
void smot_add_trigger(dcmotor_ol_t *          dcmotor,
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
static void mot_set_motor_state(dcmotor_ol_t * dcmotor, MotorState_t new_state)
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
static uint32_t mot_duty_to_phase( dcmotor_ol_t * dcmotor, uint32_t reg_duty_val )
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
static void mot_pwm_control( dcmotor_ol_t * dcmotor, uint32_t reg_duty_val,bool update_duty_val )
{
    uint32_t period;

    /* Period must be set, non-zero and in range. */
    period = dcmotor->pwm_period;

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
static void mot_no_pwm( dcmotor_ol_t * dcmotor )
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
    DBG_ERR("(eng_dev)%s\n", __func__);
}

/**
 *  \brief (Helper) Enable motor interrupts
 *
 *  Turn on the motor block IRQ: Note that all motor IRQ's will be acked
 *  first to prevent spurious interrupts.
 *
 *  \param[in]  motor_id    Motor ID
 **/
static void mot_int_enable(dcmotor_ol_t * dcmotor)
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
    DBG_ERR("(eng_dev)%s\n", __func__);
}


/**
 *  \brief (Helper) Disable all motor interrupts
 *
 *  Will mask out all the individual motor interrupts as well as disable the
 *  motor block IRQ.
 *
 *  \param[in]  dcmotor dcmotor ptr
 **/
static void mot_int_disable(dcmotor_ol_t * dcmotor)
{
    /* Shut down the block IRQ */
    uio_int_disable(dcmotor->uio_dev);

    /* Turn off all the individial motor interrupts (forces a clean setup for 
     * every move) 
     */
    dcmotor->motor_regs->IEN = 0;
    DBG_ERR("(eng_dev)%s\n", __func__);
}

/**
 *  \brief (Helper) Scan motor halt (non-emergency)
 *
 *  \param[in] dcmotor  Motor Ptr
 **/
static void mot_dc_halt(dcmotor_ol_t * dcmotor)
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

    /* Now in stopped state (warning: motor could still be moving) */
    mot_set_motor_state(dcmotor, MOT_STATE_STOP);
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
uint32_t mot_enc_position_read( dcmotor_ol_t * dcmotor )
{
    return (get_mtr6pin_encoder_pos( dcmotor->motor_regs ) );
}

uint32_t mot_enc_period_read( dcmotor_ol_t * dcmotor,uint32_t *rising )
{
    return (get_mtr6pin_encoder_period( dcmotor->motor_regs, dcmotor->mtr_connects->enc_inputs, rising ));
}
/**
 *  \brief (API) Motor Sleep
 *
 *  API function: please see documentation in scanmot_dc.h
 **/

static void smot_dc_run(dcmotor_ol_t * dcmotor)
{
    uint32_t pwm;

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
        
        dcmotor->move_direction = MOT_FORWARD;
        dcmotor->CtrlPWM        = 0;
        dcmotor->rampCounts     = 0;
        dcmotor->decelCounts    = 0;
        dcmotor->coastCounts    = 0;
        dcmotor->stopCounts     = 0;
        dcmotor->ctl_loop_count = 0;
        dcmotor->tick = 0;
    }

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
    //-jrs set_mtr6pin_en_state( dcmotor->motor_regs, dcmotor->mtr_connects, 1 );

    /* Debug dump before motor motion starts */
    //smot_dc_dump(motor_id);

    /* Turn on the PWM watchdog, give it 10x the servo period */
    // stefan: trying 100 instead of 10 x for a longer watchdog
//del by namja    mtr6pin_pwm_wdog_enable( dcmotor->motor_regs, 100 * dcmotor->mtr_connects->sample_period );

    /* Enable the motor block (enables the PWM signal) */
    set_mtr6pin_motorconfig_enable(dcmotor->motor_regs, MTR6PIN_MCFG_ENABLE);

    /* set the PWM value */
    pwm = (dcmotor->target_speed->move_params[0]->pwm_duty_cycle * dcmotor->pwm_period)/100;  //calculate percent of period for duty cycle
    DBG_ERR("Setting PWM = %u, period = %u\n", pwm, dcmotor->pwm_period);
    
	if(RecycledPaperMode == RECYCLED_PAPER_MODE)
	{
		mot_pwm_control(dcmotor, 0, false);
	}
	else
	{
	    mot_pwm_control(dcmotor, pwm, true);
	}

    // delay for 10ms while running the clock before setting the enable pin
    posix_sleep_ms(10);

    /* Set the motor enable.  Note that not all PWM methods use motor enable
     * (it will not be mapped in the connect table); the 6-pin driver will simply
     * ignore it if not mapped.
     */
    set_mtr6pin_en_state( dcmotor->motor_regs, 
                          dcmotor->mtr_connects,
                          1 );

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
void smot_start_motor(dcmotor_ol_t *dcmotor, uint32_t pwm)
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



void smot_dc_enter_sleep(dcmotor_ol_t * dcmotor)
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
void smot_dc_exit_sleep(dcmotor_ol_t * dcmotor)
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

void smot_dc_enter_sleep_4931(dcmotor_ol_t * dcmotor)
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
void smot_dc_enter_sleep_enable(dcmotor_ol_t * dcmotor)
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

void smot_dc_exit_sleep_4931(dcmotor_ol_t * dcmotor)
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

void smot_dc_exit_sleep_enable(dcmotor_ol_t * dcmotor)
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

void smot_dc_set_motor_move_params (dcmotor_ol_t * dcmotor, const motor_param_t *move_params)
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

}


/**
 *  \brief (Helper) Setup default values for the encoder period logic
 *
 **/
void mot_set_default_enc_periods(dcmotor_ol_t * dcmotor)
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
void mot_set_row_sync(dcmotor_ol_t * dcmotor,
                             MOTOR_SYNC  rowsync_type,
                             uint32_t      rowsync_pos)
{
    uint32_t start_pos;
    uint32_t current_position;
    uint32_t verify_position;

    /* rowsync_pos is passed in as a scaled encoder value, we must shift it down before
     * programming it into hardware.
     */
//-jrs    ASSERT((rowsync_pos >> dcmotor->fbits) <= get_mtr6pin_encoder_pos_max_value());
//-jrs    start_pos = rowsync_pos >> dcmotor->fbits;
    ASSERT((rowsync_pos) <= get_mtr6pin_encoder_pos_max_value());
    start_pos = rowsync_pos;

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
#if 0
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
#else
	    UNUSED_VAR( start_pos );
#endif
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
void mot_set_default_enc_debounce(dcmotor_ol_t * dcmotor, uint32_t time_usec)
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

    dbg2(("%s: time_usec=%d blockf=%d dbCount=%d\n", 
              __FUNCTION__, time_usec, block_freq, dbCount ));

    /* A zero count would defeat the point: zero debounce can cause unexpected
     * encoder edges when stopped.
     */
    XASSERT(dbCount != 0, dbCount);

    /* Set the debounce value in hardware. Note: the mtr6pin layer will protect
     * against out of range debounce counts.
     */
    set_mtr6pin_debounce_zdb( dcmotor->motor_regs, 0 );
//del by namja    set_mtr6pin_debounce_edb( dcmotor->motor_regs, dbCount/8 );
    set_mtr6pin_debounce_edb( dcmotor->motor_regs, 989 ); //Edb = 989 => input filtering = (989+1)/SysClk = 5us
}

/**
 *  \brief (Helper) Validate servo ISR rate
 *
 *  The servo ISR must be called at regular intervals or the algorithm just
 *  falls apart.  If configured:
 *   - assert on rate failure
 *
 *  \param[in] dcmotor  Motor Ptr
 **/
void isr_rate_check(dcmotor_ol_t * dcmotor)
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
#else
	UNUSED_VAR( isr_time_usecs );
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
void calc_mot_pwm_period( uint32_t pwm_freq, uint32_t *pwm_period, uint32_t *pwm_divisor )
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
void mot_set_blk_addr_irq(dcmotor_ol_t * dcmotor)
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
 *  \brief (Helper) Scan motor servo control loop
 *
 *  \param[in] dcmotor  Motor Ptr
 **/
void mot_dc_update_control_output(dcmotor_ol_t * dcmotor)
{
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
    /* System ERROR Checking:
     *   Check our ISR rate to make sure all is well (not too fast or too slow).
     *   This routine will assert (if configured) on error.
     */
    isr_rate_check(dcmotor);

    switch (dcmotor->motor_state)
    {
        case MOT_STATE_ACCEL:
             /* Accelerate until our reference velocity is equal to the target velocity. */
            //TODO: determine if we need to do anything to progress from this state.
            // currently we just move on after the first tick
//            DBG_ERR("TO MOT_STATE_WAIT_RAMP\n");
            mot_set_motor_state (dcmotor, MOT_STATE_WAIT_RAMP );
            break;

        case MOT_STATE_WAIT_RAMP:
            {
                uint32_t speed_locked;
                /* At this point, our relative velocity is equal to the final velocity but our
                 * actual velocity probably isn't. Stay in this state until the actual velocity
                 * reaches the target velocity.
                 */
    
                /* read lock input to determine if target velocity reached. */
                speed_locked = get_mtr6pin_encoder_current_value(dcmotor->motor_regs, DC_PIN_FUNC_ENCA);

                if(0/*speed_locked*/)
                {
                    /* Motor has reached the target velocity */
//                    DBG_ERR("TO MOT_STATE_STEADY\n");
                    mot_set_motor_state (dcmotor, MOT_STATE_STEADY );
                }
    
            }
            break;

        case MOT_STATE_STEADY:
            /* In this state, we've reached our final speed and are moving at what
             * is ideally a constant velocity. There's not much to do in this state
             * except wait for our deceleration point.
             */

            break;

        case MOT_STATE_DECEL:
            mot_set_motor_state(dcmotor, MOT_STATE_COAST );
            break;

        case MOT_STATE_COAST:
            /* At this point the reference velocity has reached 0, and we're waiting
             * for the actual motor velocity to reach 0. Note that it is not uncommon
             * for the system to 'rebound' a bit after stopping, so lets get more than
             * one sample at zero velocity.
             */

            /* Note we are using the raw velocity here: want to wait for any negative velocity
             * to settle as well.
             */
                if (++dcmotor->stopCounts >= STOP_COUNTS)
                {
                    /* Motor has really stopped moving, so we're done */
                    mot_dc_halt(dcmotor);
                    return;
                }
            break;

        default:
            break;
    }

}

/**
 *  \brief (Helper) Scan motor ISR
 *
 *  \param[in] intRegMask  Motor ID (set when we did intAttach)
 **/
void dcmotor_interrupt(int32_t interrupt_count, void *context)
{
//	gpio_write(GPIO_E17, 1);
    uint32_t     isr_pending;
    uint32_t     isr_enabled;
    //static int     enc_edge = 0;
    uint32_t     begin_isr_usec;
    uint32_t     end_isr_usec;
    uint32_t     isr_dur_usec;
    dcmotor_ol_t * dcmotor = (dcmotor_ol_t *) context;

	uint32_t block_freq = hwGetBusSpeed()*1000*1000; // 198 * 1000 * 1000 Hz 
	uint32_t edge;
	uint32_t enc_period;

	int pwm;
	float p_result;
	float i_result;	
	float d_result;	

	controlcount++;

	if(fpidcontrol == 0)
	{
		enc_period = mot_enc_period_read(dcmotor, &edge);
		if(enc_period != 0)
			current_pps = block_freq / enc_period;
		else
			current_pps = 0;
		
		if(get_mtr6pin_encoder_pos(dcmotor->motor_regs) >= paper_enc)
		{
			mot_dc_halt(dcmotor);
			(*dcmotor->smot_dc_enter_sleep_func)(dcmotor);
			dcmotor->target_speed = dcmotor->current_speed = dcmotor_speed_by_name("speed_off");
			
			DBG_ERR("(eng_dev)dcmotor_off : CurrentPos = %u (%d)\n", get_mtr6pin_encoder_pos(dcmotor->motor_regs), controlcount);
			fpidcontrol = 1;
		    set_mtr6pin_encoder_pos(dcmotor->motor_regs, 0);
		}	
		
		if(RecycledPaperMode == RECYCLED_PAPER_MODE)
		{
			if((controlcount >= 17) && (lowspeedstart == 1))
			{
				lowspeedstart = 0;
				target_pps = target_pps_final;
				DBG_ERR("(eng_dev)Target PPS Change %d => %d  (%d)\n", target_pps_Start, target_pps, controlcount);
				fdc_pid = 0;
			}
		}
#if 1
		if((controlcount >= MotorStallWarningCnt) && (current_pps < 600))
		{
			mot_dc_halt(dcmotor);
			(*dcmotor->smot_dc_enter_sleep_func)(dcmotor);
			dcmotor->target_speed = dcmotor->current_speed = dcmotor_speed_by_name("speed_off");
			
			DBG_ERR("(eng_dev)dcmotor_off(Warning) : CurrentPos = %u (%d)\n", get_mtr6pin_encoder_pos(dcmotor->motor_regs), controlcount);
			fpidcontrol = 1;
		    set_mtr6pin_encoder_pos(dcmotor->motor_regs, 0);		
		}
#endif
		
		if((fdc_pid	== 0) && (current_pps <= accel_pps)) //1950 = targetpps(2167) * 90%
		{	
			if(RecycledPaperMode == RECYCLED_PAPER_MODE)
			{
				current_duty = 0;
			}
			accelcount++;
		}
		else
		{ 
			fdc_pid = 1;
			
			pps_error = target_pps - current_pps;
			p_result = pps_error * Kp;
			
			I_err += pps_error * dt;
			i_result = I_err * Ki;
			
			D_err = (pps_error - before_pps_error) / dt;
			d_result = D_err * Kd;
			
			before_pps_error = pps_error;
	
			current_duty = current_duty - p_result - i_result - d_result;
		}
		
		if(current_duty > 1000)
		{
			current_duty = 990;
		}
		else if(current_duty < 10)
		{
			current_duty = 10;
		}
		else
		{
		
		}
			
		pwm = ((1000-current_duty) * dcmotor->pwm_period)/1000;
		mot_pwm_control(dcmotor, pwm, true);
		fdc_pid = 1;
	}
	
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
/*	
	if ((isr_enabled & IEN_ENC_MASK) && (isr_pending & IPEND_ENC_MASK)) // when use_encoder_isr == true
	{
	}
*/
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
//	gpio_write(GPIO_E17, 0);
}
void dcmotor_create_motor(dcmotor_ol_t * dcmotor,
                          const motor_connect_t *mtr_connects)
{
    volatile MOTOR_REGS *motorRegs;

    /* Brief sanity checks */
    XASSERT( mtr_connects != NULL,  (uint32_t)mtr_connects );

    /* Save pointers to the provided motor parameters */
    dcmotor->mtr_connects = mtr_connects;
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

    /* Set up the scan motor interrupt handler (but leave IRQ disabled) */
    uio_int_disable(dcmotor->uio_dev);
    uio_int_attach(dcmotor->uio_dev, 0, dcmotor_interrupt, (void*)dcmotor);

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
    calc_mot_pwm_period( dcmotor->pwm_freq,
                         &dcmotor->pwm_period,
                         &dcmotor->pwm_divisor);
    
    dbg1(("%s: pwm_period (max pwm): %d\n", __FUNCTION__, dcmotor->pwm_period));

    /* Set motor block pwm period, divisor and delay to their correct values */
    set_mtr6pin_pwm_divisor( motorRegs, dcmotor->pwm_divisor );
    set_mtr6pin_pwm_period( motorRegs,  dcmotor->pwm_period );
    set_mtr6pin_pwm_delay( motorRegs,   0 );

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
                                          /*MTR6PIN_TBSEL_100_US*/ /*MTR6PIN_TBSEL_1_MS*/MTR6PIN_TBSEL_10_MS ); //MTR6PIN_TBSEL_1_MS //MTR6PIN_TBSEL_10_MS
    /* Disable row sync pulses */
    mot_set_row_sync( dcmotor, SYNC_DISABLE, 0 );

}

void dcmotor_irq_post_semaphore_function_status( dcmotor_ol_t * dcmotor ) 
{
    dcmotor->current_speed = dcmotor->target_speed;
    sem_post(&dcmotor->semaphore);
}

uint32_t dcmotor_wait(dcmotor_ol_t *dcmotor, uint32_t timeout_msec)
{
    if (dcmotor)
    {
        return posix_sem_timedwait_ms(&dcmotor->semaphore, timeout_msec);
    }
    return -1;
}

void dcmotor_run( dcmotor_ol_t *dcmotor, dcmotor_speed_t *speed )
{
    DBG_ASSERT(dcmotor && speed);

	DBG_ERR("(eng_dev)dcmotor_run : SpeedIndex = %d, CurrentPos = %u, paper_enc = %u, controlcount = %d\n", speed->index, get_mtr6pin_encoder_pos(dcmotor->motor_regs), paper_enc, controlcount);
	set_mtr6pin_encoder_pos(dcmotor->motor_regs, 0);
	fpidcontrol = 0;
	
	if(RecycledPaperMode == RECYCLED_PAPER_MODE)
	{
		lowspeedstart = 1;
		fdc_pid = 1;
		current_duty = 1000;
		MotorStallWarningCnt = 40;
	}
	else
	{
		fdc_pid = 0;
		current_duty = 700;
		MotorStallWarningCnt = 20;
	}
	
	controlcount = 0;
	accelcount = 0;
	current_pps = 0;
	
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
		DBG_ERR("(eng_dev)dcmotor : nothing to do (%d, %d)\n", dcmotor->current_speed->index, speed->index);
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


uint32_t dcmotor_speed_msecs_to_ramp(dcmotor_ol_t * dcmotor, dcmotor_speed_t * speed)
{
    return speed->eta[dcmotor->motor_id];
}


void dcmotor_init()
{   
    int err;
    dcmotor_ol_t * dcmotor;
    
//	gpio_open( &GPIO_E17, GPIO_BANK_E, 17, GPIO_DIRECTION_OUTPUT, GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0 );
	
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

int dcmotor_dbcallback( int argc, char *argv[] )
{
    int dcmotor_id; 
    uint32_t speed_id;
    dcmotor_ol_t * dcmotor;
    dcmotor_speed_t * speed;

    int i;
    const char *str;


    if (argc < 2 )
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
    dcmotor = dcmotor_by_index(dcmotor_id);
    if(argc < 3)
    {
        mtr6pin_reg_dump(dcmotor->motor_regs);
        return 0;
    }

    speed_id = atoi(argv[2]);    
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

void set_encoders_of_paper(dcmotor_ol_t * dcmotor, uint32_t papersize)
{
	uint32_t psize = papersize;

	if(psize == 1) paper_enc = 1600*2;//1800*2; //LT
	else if(psize == 2) paper_enc = 1300*2;//1500*2; //EXEC
	else if(psize == 3) paper_enc = 1600*2;//1900*2; //A4
	else if(psize == 4) paper_enc = 1200*2; //B5
	else if(psize == 5) paper_enc = 1988*2;//2200*2; //LG
	else if(psize == 6) paper_enc = 1200*2;//850*2; //A5
	else if(psize == 7) paper_enc = 1200*2;//1900*2; //cover open
	else {paper_enc = 1200*2;} //unknown size

	DBG_ERR("(eng_dev)psize (%d, %d)\n", psize, paper_enc);
}

int get_dcmotor_current_duty(dcmotor_ol_t * dcmotor)
{
	return current_duty;
}

int get_dcmotor_accel_count(dcmotor_ol_t * dcmotor)
{
	return accelcount;
}

int get_dcmotor_control_count(dcmotor_ol_t * dcmotor)
{
	return controlcount;
}

uint32_t get_dcmotor_current_pps(dcmotor_ol_t * dcmotor)
{
	return current_pps;
}

void set_pidcontrol_param(dcmotor_ol_t * dcmotor, float K_p, float K_i, float K_d, float d_t, int first_target, int second_target)
{
	Kp = K_p;
	Ki = K_i;
	Kd = K_d;
	dt = d_t;
	target_pps_Start = first_target;
	target_pps_final = second_target;
	target_pps = target_pps_Start;
	accel_pps = target_pps * 90 / 100;
	
	if(target_pps_Start != target_pps_final)
	{
		RecycledPaperMode = RECYCLED_PAPER_MODE;
	}
	else
	{
		RecycledPaperMode = NORMAL_PAPER_MODE;
	}
	
	DBG_ERR("(pid)Kp = %f, Ki = %f, Kd = %f, dt = %f, taget_pps = %d,  taget_pps_final = %d, accel_pps = %d, Recycle = %d\n", Kp, Ki, Kd, dt, target_pps, target_pps_final, accel_pps, RecycledPaperMode);
	return;
}
	
