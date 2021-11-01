/*
 * ============================================================================
 * Copyright (c) 2006-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file mtr6pin.c
 *
 * \brief Interface routines between a DC motor user and the DCMOTOR_BASE2
 *        motor block
 *
 * \warning
 *    Note that this file tries to make no assumptions on how the motor hardware is
 *    connected ... the provided motor connection table should tell us what we need
 *    to know.
 **/

#include "ATypes.h"         /* UINT32 and its ilk */
#include "regAddrs.h"
#include "lassert.h"        /* ASSERT, XASSERT    */
#include "debug.h"
#include "logger.h"

#include "dcmotor.h"
#include "dcmotor_priv.h"
#include "dcmothw.h"        /* Motor HW defines   */

#include "mtr6pin.h"        /* DCMOTOR_BASE2 motor interface */


#define INVALID_PIN_NUM  0xff


/******************************************************************************
 * Private Helper Functions
 *****************************************************************************/

/**
 *  \brief (Helper) Get the invert value for the specified DC motor pin
 *
 *  Grab the invert value for the specified DC pin from the supplied connection
 *  table.  Note that this routine translates from connect table values to actual
 *  register values.
 *
 *  \param[in]  motor_connect_t*  Pointer to motor connections table 
 *  \param[in]  UINT32            DC motor block pin number (0 - 5)
 *  \param[out] UINT32            Returned invert value (register value)
 **/
static UINT32 connect_get_inv_for_pin(const motor_connect_t *mtr_connects, UINT32 pin_num)
{
    UINT32 ret_val = MTR6PIN_MCFG_NO_INVERT;

    /* Simple parameter sanity checking */
    XASSERT(mtr_connects != NULL, (UINT32)mtr_connects);
    XASSERT(pin_num < 6, pin_num);

    if (mtr_connects->pin_cfg[pin_num].invert == DC_PIN_INVERT)
    {
        ret_val = MTR6PIN_MCFG_INVERT;
    }

    return ret_val;
}

/**
 *  \brief (Helper) Get the pin select signal value for the specified DC motor pin
 *
 *  Grab the signal value for the specified DC pin from the supplied connection table.
 *  Note that this routine translates from connect table values to actual register values.
 *
 *  \param[in]  motor_connect_t* Pointer to motor connections table 
 *  \param[in]  UINT32           DC motor block pin number (0 - 5)
 *  \param[out] UINT32           Returned signal value (register value)
 **/
static UINT32 connect_get_sig_for_pin(const motor_connect_t *mtr_connects, UINT32 pin_num)
{
    UINT32 ret_val = MTR6PIN_MCFG_PINSEL_OUT_DEFAULT;

    /* Simple parameter sanity checking */
    XASSERT(mtr_connects != NULL, (UINT32)mtr_connects);
    XASSERT(pin_num < 6, pin_num);

    /* If this pin is marked as no-connect, just return the default signal (set above).
     * Otherwise translate signal between connect table value and our register value.
     */
    if (mtr_connects->pin_cfg[pin_num].func != DC_PIN_FUNC_NC)
    {
        switch (mtr_connects->pin_cfg[pin_num].signal)
        {
            case DC_PIN_NO_SIGNAL:
                ret_val = MTR6PIN_MCFG_PINSEL_OUT_DEFAULT;
                break;
            case DC_PIN_ZERO:
                ret_val = MTR6PIN_MCFG_PINSEL_OUT_0;
                break;
            case DC_PIN_ONE:
                ret_val = MTR6PIN_MCFG_PINSEL_OUT_1;
                break;
            case DC_PIN_PWM0:
                ret_val = MTR6PIN_MCFG_PINSEL_OUT_PWM0;
                break;
            case DC_PIN_PWM1:
                ret_val = MTR6PIN_MCFG_PINSEL_OUT_PWM1;
                break;
            case DC_PIN_DBG_INT_ROW_SYNC:
                ret_val = MTR6PIN_MCFG_PINSEL_OUT_INT_RS;
                break;
            case DC_PIN_DBG_EXT_ROW_SYNC:
                ret_val = MTR6PIN_MCFG_PINSEL_OUT_EXT_RS;
                break;
            default:
                dbg1(("%s: unknown pin signal (%d)\n", 
                       __FUNCTION__, mtr_connects->pin_cfg[pin_num].signal));
                XASSERT(0, mtr_connects->pin_cfg[pin_num].signal);
                break;
        }
    }

    return ret_val;
}

/**
 *  \brief (Helper) Get the pin number of the supplied function
 *
 *  Grab the signal value for the specified DC pin from the supplied connection table.
 *
 *  \param[in]  motor_connect_t* Pointer to motor connections table 
 *  \param[in]  motor_pin_func_t Motor function to find
 *  \param[out] UINT32           Returned pin number (0-5 if found, otherwise INVALID_PIN_NUM)
 **/
static UINT32 connect_get_pin_for_func(const motor_connect_t *mtr_connects, 
                                       motor_pin_func_t       mtr_pin_func)
{
    UINT32 a_pin;
    UINT32 ret_val = INVALID_PIN_NUM;

    /* Simple parameter sanity checking */
    XASSERT(mtr_connects != NULL, (UINT32)mtr_connects);

    /* Looking for an unconnected pin makes no sense here (could be more than one) */
    if (mtr_pin_func == DC_PIN_FUNC_NC)
    {
        return INVALID_PIN_NUM;
    }

    /* Look for the desired pin function, it may be OK for it not to exist */
    for (a_pin = 0; a_pin < 6; a_pin++)
    {
        if (mtr_connects->pin_cfg[a_pin].func == mtr_pin_func)
        {
            ret_val = a_pin;
            break;
        }
    }

    return ret_val;
}

/**
 *  \brief (Helper) Set on/off pin state for specified pin
 *
 *  Set the state of the specified pin.
 *
 *  \param[in]  motor_regs   Pointer to the motor registers
 *  \param[in]  pin          Pin number (0 - 5)
 *  \param[in]  state        Desired on/off state (0 or 1)
 **/
static void set_state_for_pin(volatile MOTOR_REGS *motor_regs, UINT32 pin, UINT32 state)
{
    UINT32 mcfg_val  = motor_regs->MCFG;
    UINT32 sig_state = MTR6PIN_MCFG_PINSEL_OUT_0;

    /* Validate and map the state */
    XASSERT( (state == 0) || (state == 1), state );
    if (state == 1)
    {
        sig_state = MTR6PIN_MCFG_PINSEL_OUT_1;
    }

    switch (pin)
    {
        case 0:
            mcfg_val = DCMOTOR_BASE_MCFG_PINSEL0_REPLACE_VAL(mcfg_val, sig_state);
            break;
        case 1:
            mcfg_val = DCMOTOR_BASE_MCFG_PINSEL1_REPLACE_VAL(mcfg_val, sig_state);
            break;
        case 2:
            mcfg_val = DCMOTOR_BASE_MCFG_PINSEL2_REPLACE_VAL(mcfg_val, sig_state);
            break;
        case 3:
            mcfg_val = DCMOTOR_BASE_MCFG_PINSEL3_REPLACE_VAL(mcfg_val, sig_state);
            break;
        case 4:
            mcfg_val = DCMOTOR_BASE_MCFG_PINSEL4_REPLACE_VAL(mcfg_val, sig_state);
            break;
        case 5:
            mcfg_val = DCMOTOR_BASE_MCFG_PINSEL5_REPLACE_VAL(mcfg_val, sig_state);
            break;
        default:
            XASSERT(0, pin);
            break;
    }

    /* Set the new register value */
    motor_regs->MCFG = mcfg_val;
}



/******************************************************************************
 * Public 6-pin Functions
 *****************************************************************************/

/* Return maximum valid period value */
UINT32 get_mtr6pin_pwm_period_max( void )
{
    return DCMOTOR_BASE_PPER0_PWMPER_MASK_SHIFT( DCMOTOR_BASE_PPER0_PWMPER_MASK );
}

/* Replace PWM period field in PPER0 register with a new value. */ 
void set_mtr6pin_pwm_period( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->PPER0 = DCMOTOR_BASE_PPER0_PWMPER_REPLACE_VAL( motor_regs->PPER0, value );

    /* If PPER0 is non-zero it must be larger than PDTY0 */
    ASSERT( (motor_regs->PPER0 == 0) || (motor_regs->PPER0 > motor_regs->PDTY0) );
}

/* Return maximum valid period value */
UINT32 get_mtr6pin_pwm_duty_max( void )
{
    return DCMOTOR_BASE_PDTY0_PWMDTY_MASK_SHIFT( DCMOTOR_BASE_PDTY0_PWMDTY_MASK );
}

/* Replace PWM duty field in PDTY0 register with a new value. */ 
void set_mtr6pin_pwm_duty( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->PDTY0 = DCMOTOR_BASE_PDTY0_PWMDTY_REPLACE_VAL( motor_regs->PDTY0, value );

    /* If PDTY0 must be 0 or in the range 0..(PPER0-1) */
    if (!((0 == motor_regs->PDTY0) || (motor_regs->PDTY0 < motor_regs->PPER0))) 
    {
        //dbg1(("%s: motor_regs->PDTY0 %d motor_regs->PPER0 %d\n", 
        //               __FUNCTION__, motor_regs->PDTY0,motor_regs->PPER0));
    }
    //ASSERT( (0 == motor_regs->PDTY0) || (motor_regs->PDTY0 < motor_regs->PPER0) );
}

/* Read current value of the encoder position register */
UINT32 get_mtr6pin_encoder_pos( volatile MOTOR_REGS * motor_regs )
{
    return (DCMOTOR_BASE_EPOS_RD_POS_MASK_SHIFT( motor_regs->EPOS_RD ) );
}


/* Read current value of the encoder period register */
UINT32 get_mtr6pin_encoder_period( volatile MOTOR_REGS * motor_regs, motor_enc_t enc_inputs, uint32_t * rising )
{
#if 0
    if (enc_inputs == AB_NORMAL || enc_inputs == SINGLE_A) 
    {
        return DCMOTOR_BASE_APER_APER_MASK_SHIFT(motor_regs->APER);
    }
    else
    {
        return DCMOTOR_BASE_BPER_BPER_MASK_SHIFT(motor_regs->BPER);
    }
#else
    motor_regs->LATCH_EN = 1;//Whack a mole

    *rising = DCMOTOR_BASE_TS_ENC_1_TS_ENCA_MASK_SHIFT(motor_regs->LATCH_TS_ENC_0);
    //dbg1(("4 %x 3 %x 2 %x 1 %x 0 %x\n",motor_regs->LATCH_TS_ENC_4, motor_regs->LATCH_TS_ENC_3,motor_regs->LATCH_TS_ENC_2,motor_regs->LATCH_TS_ENC_1,motor_regs->LATCH_TS_ENC_0));
    if (DCMOTOR_BASE_TS_ENC_0_TS_ENC_VAL_MASK_SHIFT(motor_regs->LATCH_TS_ENC_0) == 0) //no readings
    {
        return(0);
    }
    else if (DCMOTOR_BASE_TS_ENC_1_TS_ENC_VAL_MASK_SHIFT(motor_regs->LATCH_TS_ENC_1) == 0)//one reading
    {
        if (DCMOTOR_BASE_TS_ENC_0_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_0) < DCMOTOR_BASE_TS_ENC_0_TS_ENC_MASK) 
        {
            return (DCMOTOR_BASE_TS_ENC_0_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_0) + DCMOTOR_BASE_TS_ENC_0_TS_ENC_MASK);
        }
        else 
        {
            return(0);
        }
    }    
    else if (DCMOTOR_BASE_TS_ENC_2_TS_ENC_VAL_MASK_SHIFT(motor_regs->LATCH_TS_ENC_2) == 0) //two readings
    {
        return ( DCMOTOR_BASE_TS_ENC_1_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_1) +
                 DCMOTOR_BASE_TS_ENC_0_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_0));
        
    }
    else if (DCMOTOR_BASE_TS_ENC_3_TS_ENC_VAL_MASK_SHIFT(motor_regs->LATCH_TS_ENC_3) == 0) //three readings
    {
        return ( DCMOTOR_BASE_TS_ENC_1_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_1) +
                 DCMOTOR_BASE_TS_ENC_0_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_0));
        
    }
    else //its moving now
    {
#if 0
        return (/*(DCMOTOR_BASE_TS_ENC_4_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_4) +
                 DCMOTOR_BASE_TS_ENC_3_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_3) +
                 2 * DCMOTOR_BASE_TS_ENC_2_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_2) +
                 4 * */DCMOTOR_BASE_TS_ENC_0_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_0)/*) / 8*/);
#endif
        //return ((DCMOTOR_BASE_TS_ENC_2_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_2) +
        //         DCMOTOR_BASE_TS_ENC_1_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_1)) / 2);
        //return DCMOTOR_BASE_TS_ENC_1_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_1);
        //return ((DCMOTOR_BASE_TS_ENC_3_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_3) +
        //         DCMOTOR_BASE_TS_ENC_2_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_2) +
        //         DCMOTOR_BASE_TS_ENC_1_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_1)) / 3);
        return (   DCMOTOR_BASE_TS_ENC_1_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_1) +
                   DCMOTOR_BASE_TS_ENC_0_TS_ENC_MASK_SHIFT(motor_regs->LATCH_TS_ENC_0)
               );
        //return (DCMOTOR_BASE_FILTOUT_FILTOUT_MASK_SHIFT(motor_regs->FILTOUT<< 5));
        
    }
        
#endif
}
/* Write position to the encoder write register */
void set_mtr6pin_encoder_pos( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->EPOS_WR = DCMOTOR_BASE_EPOS_WR_POS_REPLACE_VAL( motor_regs->EPOS_WR, value );
}

/* Fetch the maximum value the encoder can hold */
UINT32 get_mtr6pin_encoder_pos_max_value()
{
    return (DCMOTOR_BASE_EPOS_RD_POS_MASK >> DCMOTOR_BASE_EPOS_RD_POS_SHIFT);
}

/* Set new value in the PWM delay register */
void set_mtr6pin_pwm_delay( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->PDLY0 = DCMOTOR_BASE_PDLY0_PWMDLY_REPLACE_VAL(motor_regs->PDLY0,value);
}

/* Set new value in the PWM divisor register */
void set_mtr6pin_pwm_divisor( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->PDIV0 = DCMOTOR_BASE_PDIV0_PWMDIV_REPLACE_VAL(motor_regs->PDIV0,value);
}


/**
 *  \brief Set the default motor configuration
 *
 *  Function to set the default motor configuration (MCFG).  The motor 'connection' map 
 *  was provided by the mech code, our job here is to get things mapped correctly.
 *  This is done just once when a motor is created, we must be sure to leave the motor
 *  block disabled.
 *
 *  \param[in] motor_regs    Pointer to the motor registers
 *  \param[in] connect_table Pointer to a motor connection table 
 **/
void set_mtr6pin_motorconfig_initialize( volatile MOTOR_REGS   *motor_regs, 
                                         const motor_connect_t *connect_table )
{
    UINT32 mcfg_val = 0;
    UINT32 invert   = 0;

    /* Make sure to start out with the motor block disabled */
    mcfg_val = DCMOTOR_BASE_MCFG_EN_REPLACE_VAL(mcfg_val, MTR6PIN_MCFG_DISABLE);

    /* Build the invert mask based on the settings in the connections table. Note
     * that I didn't put this into a loop so I could use the OBA macros (in case 
     * things move around in later revs).
     * BTW: this code will only work when 'not inverted' = 0.
     */
    XASSERT(MTR6PIN_MCFG_NO_INVERT == 0, MTR6PIN_MCFG_NO_INVERT);
    invert |= (connect_get_inv_for_pin(connect_table, 0) << MTR6PIN_MCFG_INV_PIN0);
    invert |= (connect_get_inv_for_pin(connect_table, 1) << MTR6PIN_MCFG_INV_PIN1);
    invert |= (connect_get_inv_for_pin(connect_table, 2) << MTR6PIN_MCFG_INV_PIN2);
    invert |= (connect_get_inv_for_pin(connect_table, 3) << MTR6PIN_MCFG_INV_PIN3);
    invert |= (connect_get_inv_for_pin(connect_table, 4) << MTR6PIN_MCFG_INV_PIN4);
    invert |= (connect_get_inv_for_pin(connect_table, 5) << MTR6PIN_MCFG_INV_PIN5);

    ASSERT(invert == (invert & (DCMOTOR_BASE_MCFG_INV_MASK>>DCMOTOR_BASE_MCFG_INV_SHIFT)));
    mcfg_val = DCMOTOR_BASE_MCFG_INV_REPLACE_VAL(mcfg_val, invert);


    /* Roll through all of the pin selects.  Note that I didn't put this into a loop
     * so I could use the OBA macros (in case things move around in later revs).
     */
    mcfg_val = DCMOTOR_BASE_MCFG_PINSEL0_REPLACE_VAL(mcfg_val, connect_get_sig_for_pin(connect_table,0));
    mcfg_val = DCMOTOR_BASE_MCFG_PINSEL1_REPLACE_VAL(mcfg_val, connect_get_sig_for_pin(connect_table,1));
    mcfg_val = DCMOTOR_BASE_MCFG_PINSEL2_REPLACE_VAL(mcfg_val, connect_get_sig_for_pin(connect_table,2));
    mcfg_val = DCMOTOR_BASE_MCFG_PINSEL3_REPLACE_VAL(mcfg_val, connect_get_sig_for_pin(connect_table,3));
    mcfg_val = DCMOTOR_BASE_MCFG_PINSEL4_REPLACE_VAL(mcfg_val, connect_get_sig_for_pin(connect_table,4));
    mcfg_val = DCMOTOR_BASE_MCFG_PINSEL5_REPLACE_VAL(mcfg_val, connect_get_sig_for_pin(connect_table,5));

    /* Make sure only valid bits are being set */
    ASSERT( mcfg_val == (mcfg_val & ( DCMOTOR_BASE_MCFG_EN_MASK       |
                                      DCMOTOR_BASE_MCFG_INV_MASK      |
                                      DCMOTOR_BASE_MCFG_PINSEL0_MASK  |
                                      DCMOTOR_BASE_MCFG_PINSEL1_MASK  |
                                      DCMOTOR_BASE_MCFG_PINSEL2_MASK  |
                                      DCMOTOR_BASE_MCFG_PINSEL3_MASK  |
                                      DCMOTOR_BASE_MCFG_PINSEL4_MASK  |
                                      DCMOTOR_BASE_MCFG_PINSEL5_MASK )) );

    /* Set the actual HW MCFG register */
    motor_regs->MCFG = mcfg_val;
}

/**
 *  \brief Set/reset the enable bit in the motor config register
 *
 *  Sets the MCFG enable bit to the desired state.  Note that the enable bit
 *  is the overall enable bit for any generated PWM drives.
 *
 *  \param[in] motor_regs    Pointer to the motor registers
 *  \param[in] enable        Desired enable setting 
 **/
void set_mtr6pin_motorconfig_enable ( volatile MOTOR_REGS   *motor_regs, 
                                      UINT32                 enable )
{
    UINT32 reg;

    /* Enable range check (prevent bit stomping) */
    XASSERT ((enable & (~DCMOTOR_BASE_MCFG_EN_MASK >> DCMOTOR_BASE_MCFG_EN_SHIFT)) == 0, enable);

    reg = motor_regs->MCFG;
    reg = DCMOTOR_BASE_MCFG_EN_REPLACE_VAL( reg, enable );
    motor_regs->MCFG = reg;
    motor_regs->TS_CTRL = DCMOTOR_BASE_TS_CTRL_TS_ENABLE_REPLACE_VAL(0,enable) |      //enable TS
                      DCMOTOR_BASE_TS_CTRL_TS_ABSOLUTE_REPLACE_VAL(0,0) |    //relative TS
                      DCMOTOR_BASE_TS_CTRL_TS_TBSEL_REPLACE_VAL(0,6);        //system clock
    // removed to avoid debug prints in at_assert callback
//-jrs    dbg1(("motor_regs->TS_CTRL %x\n", motor_regs->TS_CTRL));
}

/**
 *  \brief Set the state of the direction pin
 *
 *  Uses the appropriate MCFG pinsel to set the state of the direction pin.  We 
 *  need the connection table to figure out what the proper bit is ... note that 
 *  it will not be mapped for some PWM methods.
 *
 *  \param[in] motor_regs    Pointer to the motor registers
 *  \param[in] connect_table Pointer to a motor connection table
 *  \param[in] dir           Desired motor direction
 **/
void set_mtr6pin_dir_state ( volatile MOTOR_REGS   *motor_regs, 
                             const motor_connect_t *connect_table,
                             motor_dir_t            dir )
{
    UINT32 dir_pin = INVALID_PIN_NUM;
    UINT32 dir_val = MTR6PIN_MCFG_DIR_FWD;

    /* Make sure direction is valid, map it to a register value */
    XASSERT( (dir == MOT_FORWARD) || (dir == MOT_REVERSE), dir );
    if (dir == MOT_REVERSE)
    {
        dir_val = MTR6PIN_MCFG_DIR_REV;
    }

    /* Find the direction pin (typically only used for NORMAL PWM) */
    dir_pin = connect_get_pin_for_func(connect_table, DC_PIN_FUNC_DIR);
    if (dir_pin == INVALID_PIN_NUM)
    {
        /* Dir not mapped: this is required for NORMAL PWM */
        /* NOTE: would normally assert here, but certain motor controllers take away
         *       our direction control on purpose.
         */
        //XASSERT(connect_table->pwm_type != DC_PWM_NORMAL, connect_table->pwm_type);
        return;
    }

    /* Make the needed MCFG changes */
    set_state_for_pin(motor_regs, dir_pin, dir_val);
}

/**
 *  \brief Set the state of the enable pin
 *
 *  Uses the appropriate MCFG pinsel to set the state of the enable pin.  We 
 *  need the connection table to figure out what the proper bit is ... note that 
 *  it will not be mapped for some PWM methods.
 *
 *  \param[in] motor_regs    Pointer to the motor registers
 *  \param[in] connect_table Pointer to a motor connection table
 *  \param[in] enable        Desired enable state
 **/
void set_mtr6pin_en_state ( volatile MOTOR_REGS   *motor_regs, 
                            const motor_connect_t *connect_table,
                            UINT32                 enable )
{
    UINT32 en_pin = INVALID_PIN_NUM;
    UINT32 en_val = MTR6PIN_MCFG_PINSEL_OUT_0;

    /* Make sure enable is valid, map it to a register value */
    XASSERT( (enable == 0) || (enable == 1), enable );
    if (enable == 1)
    {
        en_val = MTR6PIN_MCFG_PINSEL_OUT_1;
    }

    /* Find the enable pin (typically only used for PHASE PWM) */
    en_pin = connect_get_pin_for_func(connect_table, DC_PIN_FUNC_EN);
    if (en_pin == INVALID_PIN_NUM)
    {
        /* Enable not mapped: this is required for PHASE PWM */
        XASSERT(connect_table->pwm_type != DC_PWM_PHASE, connect_table->pwm_type);
        return;
    }

    /* Make the needed MCFG changes */
    set_state_for_pin(motor_regs, en_pin, en_val);
}
/**
 *  \brief Set the state of the sleep pin
 *
 *  Uses the appropriate MCFG pinsel to set the state of the
 *  sleep pin.  We need the connection table to figure out what
 *  the proper bit is ... note that it will not be mapped for
 *  some PWM methods.
 *
 *  \param[in] motor_regs    Pointer to the motor registers
 *  \param[in] connect_table Pointer to a motor connection table
 *  \param[in] sleep        Desired sleep state
 **/
void set_mtr6pin_slp_state ( volatile MOTOR_REGS   *motor_regs, 
                            const motor_connect_t *connect_table,
                            UINT32                 sleep )
{
    UINT32 slp_pin = INVALID_PIN_NUM;
    UINT32 slp_val = MTR6PIN_MCFG_PINSEL_OUT_0;

    /* Make sure enable is valid, map it to a register value */
    XASSERT( (sleep == 0) || (sleep == 1), sleep );
    if (sleep == 1)
    {
        slp_val = MTR6PIN_MCFG_PINSEL_OUT_1;
    }

    /* Find the sleep pin (typically only used for PHASE PWM) */
    slp_pin = connect_get_pin_for_func(connect_table, DC_PIN_FUNC_SLP);
    if (slp_pin == INVALID_PIN_NUM)
    {
        /* Sleep not mapped: this is required for PHASE PWM */
        //XASSERT(connect_table->pwm_type == INVALID_PIN_NUM, connect_table->pwm_type);
        return;
    }

    /* Make the needed MCFG changes */
    set_state_for_pin(motor_regs, slp_pin, slp_val);
}
/**
 *  \brief Set the state of the sleep pin
 *
 *  Uses the appropriate MCFG pinsel to set the state of the
 *  sleep pin.  We need the connection table to figure out what
 *  the proper bit is ... note that it will not be mapped for
 *  some PWM methods.
 *
 *  \param[in] motor_regs    Pointer to the motor registers
 *  \param[in] connect_table Pointer to a motor connection table
 *  \param[in] sleep        Desired sleep state
 **/
void set_mtr6pin_mode_state ( volatile MOTOR_REGS   *motor_regs, 
                              const motor_connect_t *connect_table,
                              UINT32                 mode )
{
    UINT32 mode_pin = INVALID_PIN_NUM;
    UINT32 mode_val = MTR6PIN_MCFG_PINSEL_OUT_0;

    /* Make sure enable is valid, map it to a register value */
    XASSERT( (mode == 0) || (mode == 1), mode );
    if (mode == 1)
    {
        mode_val = MTR6PIN_MCFG_PINSEL_OUT_1;
    }

    /* Find the mode pin (typically only used for PHASE PWM) */
    mode_pin = connect_get_pin_for_func(connect_table, DC_PIN_FUNC_MODE);
    if (mode_pin == INVALID_PIN_NUM)
    {
        /* mode not mapped: this is required for PHASE PWM */
        XASSERT(connect_table->pwm_type == INVALID_PIN_NUM, connect_table->pwm_type);
        return;
    }

    /* Make the needed MCFG changes */
    set_state_for_pin(motor_regs, mode_pin, mode_val);
}

/**
 *  \brief Set the default encoder configuration
 *
 *  Function to set the default encoder configuration (ECFG).  The motor 'connection' map 
 *  was provided by the mech code, our job here is to get things mapped correctly.
 *  This is done just once when a motor is created. Note that the current motor code only
 *  works with quad counts, so an AB encoder mode is always selected.
 *
 *  \param[in] motor_regs      Pointer to the motor registers
 *  \param[in] connect_table   Pointer to a motor connection table 
 *  \param[in] timebase_select Desired motor IRQ timebase
 **/
void set_mtr6pin_encoderconfig_initialize( volatile MOTOR_REGS   *motor_regs,
                                           const motor_connect_t *connect_table,
                                           UINT32                 timebase_select ) 
{
    UINT32 ecfg_val = 0;

    UINT32 pin;
    UINT32 inpin_cfg = motor_regs->IN_PIN_CFG;

    /* ENCA */
    pin = connect_get_pin_for_func(connect_table, DC_PIN_FUNC_ENCA);
    if (pin == INVALID_PIN_NUM)
    {
        dbg1(("%s: Must specify ENCA pin in motor connect table\n", __FUNCTION__));
        XASSERT(0, DC_PIN_FUNC_ENCA);
    }
    else
    {
        inpin_cfg = DCMOTOR_BASE_IN_PIN_CFG_ENCA_SEL_REPLACE_VAL(inpin_cfg, pin);
    }

    /* ENCB */
    pin = connect_get_pin_for_func(connect_table, DC_PIN_FUNC_ENCB);
    if (pin == INVALID_PIN_NUM)
    {
        dbg1(("%s: Did you forget to specify ENCB pin in motor connect table\n", __FUNCTION__));
        //XASSERT(0, DC_PIN_FUNC_ENCB);
    }
    else
    {
        inpin_cfg = DCMOTOR_BASE_IN_PIN_CFG_ENCB_SEL_REPLACE_VAL(inpin_cfg, pin);
    }

    /* Write the HW register */
    motor_regs->IN_PIN_CFG = inpin_cfg;

    /* Now we have our encoder pins configured, get down to setting our desired
     * encoder behavior (note that we started with the ECFG register zeroed).
     */
    ecfg_val = DCMOTOR_BASE_ECFG_ENCEN_REPLACE_VAL(ecfg_val,    1);
    ecfg_val = DCMOTOR_BASE_ECFG_RSYNCSEL_REPLACE_VAL(ecfg_val, MTR6PIN_RS_PULSE);
    ecfg_val = DCMOTOR_BASE_ECFG_TBASEEN_REPLACE_VAL(ecfg_val,  1);
    ecfg_val = DCMOTOR_BASE_ECFG_TBASESEL_REPLACE_VAL(ecfg_val, timebase_select);
    ecfg_val = DCMOTOR_BASE_ECFG_ZPMODE_REPLACE_VAL(ecfg_val,   MTR6PIN_ZPMODE_IGNORE);
    ecfg_val = DCMOTOR_BASE_ECFG_FSHIFT_REPLACE_VAL(ecfg_val,   5);
    //ecfg_val = DCMOTOR_BASE_ECFG_ISCALE_REPLACE_VAL(ecfg_val,   5);
    
    //ecfg_val = DCMOTOR_BASE_ECFG_PMODE_REPLACE_VAL(ecfg_val,    MTR6PIN_PMODE_AB);
    ecfg_val = DCMOTOR_BASE_ECFG_RMODE_REPLACE_VAL(ecfg_val,    MTR6PIN_RMODE_FILTER_EDGE);

    /* See if we need to swap enca and encb */
    
    
    if (connect_table->enc_inputs == AB_SWAPPED)
    {
        ecfg_val = DCMOTOR_BASE_ECFG_PMODE_REPLACE_VAL(ecfg_val,    MTR6PIN_PMODE_AB);
        ecfg_val = DCMOTOR_BASE_ECFG_IMODE_REPLACE_VAL(ecfg_val, MTR6PIN_IMODE_AB_SWAP);
    }
    else if (connect_table->enc_inputs == AB_NORMAL) 
    {
        ecfg_val = DCMOTOR_BASE_ECFG_PMODE_REPLACE_VAL(ecfg_val,    MTR6PIN_PMODE_AB);
        ecfg_val = DCMOTOR_BASE_ECFG_IMODE_REPLACE_VAL(ecfg_val, MTR6PIN_IMODE_AB);
    }
    else if (connect_table->enc_inputs == SINGLE_A) 
    {
        ecfg_val = DCMOTOR_BASE_ECFG_PMODE_REPLACE_VAL(ecfg_val,    MTR6PIN_PMODE_A);
        ecfg_val = DCMOTOR_BASE_ECFG_IMODE_REPLACE_VAL(ecfg_val, MTR6PIN_IMODE_A);
    }
    else if (connect_table->enc_inputs == SINGLE_B) 
    {
        ecfg_val = DCMOTOR_BASE_ECFG_PMODE_REPLACE_VAL(ecfg_val,    MTR6PIN_PMODE_B);
        ecfg_val = DCMOTOR_BASE_ECFG_IMODE_REPLACE_VAL(ecfg_val, MTR6PIN_IMODE_B);
    }
    else  
    {
        dbg1(("%s: Bad Encoder type specified\n", __FUNCTION__));
        XASSERT(0, connect_table->enc_inputs);
    }

    /* Verify that only valid bits are set */
    ASSERT( ecfg_val == (ecfg_val & ( DCMOTOR_BASE_ECFG_TEST_MASK     |
                                      DCMOTOR_BASE_ECFG_ENCEN_MASK    |
                                      DCMOTOR_BASE_ECFG_RSYNCSEL_MASK |
                                      DCMOTOR_BASE_ECFG_ISCALE_MASK   |
                                      DCMOTOR_BASE_ECFG_TBASEEN_MASK  |
                                      DCMOTOR_BASE_ECFG_TBASESEL_MASK |
                                      DCMOTOR_BASE_ECFG_ZPMODE_MASK   |
                                      DCMOTOR_BASE_ECFG_IMODE_MASK    |
                                      DCMOTOR_BASE_ECFG_PMODE_MASK    |
                                      DCMOTOR_BASE_ECFG_FSHIFT_MASK |
                                      DCMOTOR_BASE_ECFG_ISCALE_MASK |
                                      DCMOTOR_BASE_ECFG_RMODE_MASK) ) );

    /* Now write the actual HW ECFG register */
    motor_regs->ECFG = ecfg_val;
    motor_regs->ENC_INT_CFG = DCMOTOR_BASE_ENC_INT_CFG_ENCEDGESEL_REPLACE_VAL(motor_regs->ENC_INT_CFG,0);
}


void set_mtr6pin_encoderconfig_iscale( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->ECFG = DCMOTOR_BASE_ECFG_ISCALE_REPLACE_VAL( motor_regs->ECFG, value );
}


void set_mtr6pin_encoderconfig_enable( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->ECFG = DCMOTOR_BASE_ECFG_ENCEN_REPLACE_VAL( motor_regs->ECFG, value );
}

void set_mtr6pin_encoderconfig_rsyncsel( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->ECFG = DCMOTOR_BASE_ECFG_RSYNCSEL_REPLACE_VAL( motor_regs->ECFG, value );
}

void set_mtr6pin_encoderconfig_rmode( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->ECFG = DCMOTOR_BASE_ECFG_RMODE_REPLACE_VAL( motor_regs->ECFG, value );
}

/* Set rowsync start position */
void set_mtr6pin_rstart( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->RSTART = DCMOTOR_BASE_RSTART_RSTART_REPLACE_VAL(motor_regs->RSTART,value);
}

/* Set rowsync ignore bit */
void set_mtr6pin_rstop_ignore( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->RSTOP = DCMOTOR_BASE_RSTOP_IGNORE_REPLACE_VAL(motor_regs->RSTOP,value);
}

/* Set rstop position */
void set_mtr6pin_rstop( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    motor_regs->RSTOP = DCMOTOR_BASE_RSTOP_RSTOP_REPLACE_VAL(motor_regs->RSTOP,value);
}

/* Set debounce register zdb field */
void set_mtr6pin_debounce_zdb( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    UINT32 zdb_val = MIN(value, (DCMOTOR_BASE_DB_ZDB_MASK >> DCMOTOR_BASE_DB_ZDB_SHIFT));

    motor_regs->DB = DCMOTOR_BASE_DB_ZDB_REPLACE_VAL(motor_regs->DB, zdb_val);
}

/* Set debounce register edb field */
void set_mtr6pin_debounce_edb( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    UINT32 edb_val = MIN(value, (DCMOTOR_BASE_DB_EDB_MASK >> DCMOTOR_BASE_DB_EDB_SHIFT));

    motor_regs->DB = DCMOTOR_BASE_DB_EDB_REPLACE_VAL(motor_regs->DB, edb_val);
}

/* Get debounce register edb field */
UINT32 get_mtr6pin_debounce_edb( volatile MOTOR_REGS * motor_regs )
{
    return (DCMOTOR_BASE_DB_EDB_MASK_SHIFT( motor_regs->DB ) );
}

/* set oscale field in rowsync PM register */
void set_mtr6pin_pm_oscale( volatile MOTOR_REGS * motor_regs, UINT32 oscale )
{
    motor_regs->RS_PM = DCMOTOR_BASE_RS_PM_OSCALE_REPLACE_VAL( motor_regs->RS_PM, oscale );
}

/* set mscale field in rowsync PM register */
void set_mtr6pin_pm_mscale( volatile MOTOR_REGS * motor_regs, UINT32 mscale )
{
    motor_regs->RS_PM = DCMOTOR_BASE_RS_PM_MSCALE_REPLACE_VAL( motor_regs->RS_PM, mscale );
}

/* set mult field in rowsync PM register */
void set_mtr6pin_pm_mult( volatile MOTOR_REGS * motor_regs, UINT32 mult )
{
    motor_regs->RS_PM = DCMOTOR_BASE_RS_PM_MULT_REPLACE_VAL( motor_regs->RS_PM, mult );
}

/* Set/Reset the Soft Reset bit for the motor block */
void set_mtr6pin_soft_reset( volatile MOTOR_REGS * motor_regs, UINT32 value )
{
    /* verify that the new SRESET value is in range */
    ASSERT( value == (value & DCMOTOR_BASE_SRESET_SRESET_MASK_SHIFT(DCMOTOR_BASE_SRESET_SRESET_MASK)) );

    motor_regs->SReset = DCMOTOR_BASE_SRESET_SRESET_REPLACE_VAL( motor_regs->SReset, value );
}

/* Service the PWM Watchdog */
void mtr6pin_pwm_wdog_service(volatile MOTOR_REGS * motor_regs, BOOL allow_pwm_reset)
{
    UINT32 lfsr_val = 0;

#ifdef DEBUG
    static bool oneshot = true;  // only output once to prevent debug output spam on error
    // register access is volatile so optimzier won't remove even if DBG macro compiled out
    // so wrap with ifdef DEBUG
    if(DCMOTOR_BASE_PWATCH_STAT_EXPIRED_MASK_SHIFT(motor_regs->PWATCH_STAT) != 0)
    {
	if (oneshot)
	{
  	    oneshot = false;
            DBG_ERR("ERROR: %s WATCHDOG FIRED\n", __func__);
	}
    }
#endif

    /* Writing the service register can reset an expired watchdog condition and re-enable
     * the PWM: so only do the service if:
     *   - the caller allows a reset
     *   - expired condition is not pending
     */
    if ( allow_pwm_reset == WDOG_ALLOW_PWM_RESET ||
         DCMOTOR_BASE_PWATCH_STAT_EXPIRED_MASK_SHIFT(motor_regs->PWATCH_STAT) == 0 )
    {
        /* Grab the current LFSR value */
        lfsr_val = DCMOTOR_BASE_PWATCH_STAT_LFSR_MASK_SHIFT(motor_regs->PWATCH_STAT);

        /* Invert the LFSR and write it back to the service register */
        lfsr_val = (~lfsr_val) & DCMOTOR_BASE_PWATCH_STAT_LFSR_MASK;
    
        motor_regs->PWATCH_SERVICE = DCMOTOR_BASE_PWATCH_SERVICE_SERVICE_REPLACE_VAL(
            motor_regs->PWATCH_SERVICE,
            lfsr_val);
    }
}

/* Enable the PWM Watchdog */
void mtr6pin_pwm_wdog_enable( volatile MOTOR_REGS * motor_regs, UINT32 wdog_ms )
{
    UINT32 ctrl_val = 0;

    /* Getting here with the pwm wdog already running could mean:
     *  - logic error in scan/motor code
     *  - the system restarted with a motor runing and the wdog fired
     *
     * Assume the latter and disable the wdog here, which will also reset the
     * PWM.
     */
    if (DCMOTOR_BASE_PWATCH_CTRL_EN_MASK_SHIFT(motor_regs->PWATCH_CTRL) == 1)
    {
        mtr6pin_pwm_wdog_disable( motor_regs );
        XASSERT(DCMOTOR_BASE_PWATCH_CTRL_EN_MASK_SHIFT(motor_regs->PWATCH_CTRL) == 0,  
              motor_regs->PWATCH_CTRL);
    }

    /* If the watchdog popped during the last move, we may need to re-enable the pwm
     * before proceeding.
     */
    if (DCMOTOR_BASE_PWATCH_STAT_EXPIRED_MASK_SHIFT(motor_regs->PWATCH_STAT) == 1)
    {
        mtr6pin_pwm_wdog_service(motor_regs, WDOG_ALLOW_PWM_RESET);

        /* Get upset if the expired bit didn't clear ... that would imply that the
         * PWM remains disabled.
         */
        XASSERT(DCMOTOR_BASE_PWATCH_STAT_EXPIRED_MASK_SHIFT(motor_regs->PWATCH_STAT) == 0,
                  motor_regs->PWATCH_STAT);
    }

    /* Our wdog terminal count is given in ms and we will run the watchdog timebase
     * at 1ms, so no conversion is needed. Create our control register value.
     */
    ctrl_val = DCMOTOR_BASE_PWATCH_CTRL_EN_REPLACE_VAL(ctrl_val, 1);
    ctrl_val = DCMOTOR_BASE_PWATCH_CTRL_TBASESEL_REPLACE_VAL(ctrl_val, MTR6PIN_TBSEL_1_MS);
    ctrl_val = DCMOTOR_BASE_PWATCH_CTRL_TERMCNT_REPLACE_VAL(ctrl_val, wdog_ms);

    /* Now write the actual HW PWATCH CTRL register ... starts the wdog! */
    motor_regs->PWATCH_CTRL = ctrl_val;
}

/* Disable the PWM Watchdog */
void mtr6pin_pwm_wdog_disable( volatile MOTOR_REGS * motor_regs )
{
    /* Shut it down. */
    motor_regs->PWATCH_CTRL = DCMOTOR_BASE_PWATCH_CTRL_EN_REPLACE_VAL(motor_regs->PWATCH_CTRL, 0);
}

/* Get PWM watchdog status register */
UINT32 get_mtr6pin_pwm_wdog_status( volatile MOTOR_REGS * motor_regs )
{
    return (motor_regs->PWATCH_STAT);
}

/* Dump the motor registers */
void mtr6pin_reg_dump(volatile MOTOR_REGS *motor_regs)
{
#if SDBGLEVEL > 0
    dbg1(( "\n"));
    dbg1(( "%12s=0x%08lx\n",   "&motor_regs",  motor_regs));
    dbg1(( "%12s=0x%08lx\n",   "MCFG",         motor_regs->MCFG));
    dbg1(( "%12s=0x%08lx\n",   "PDIV0",        motor_regs->PDIV0));
    dbg1(( "%12s=0x%08lx\n",   "PPER0",        motor_regs->PPER0));
    dbg1(( "%12s=0x%08lx\n",   "PDTY0",        motor_regs->PDTY0));
    dbg1(( "%12s=0x%08lx\n",   "PDLY0",        motor_regs->PDLY0));
    dbg1(( "%12s=0x%08lx\n",   "PDIV1",        motor_regs->PDIV1));
    dbg1(( "%12s=0x%08lx\n",   "PPER1",        motor_regs->PPER1));
    dbg1(( "%12s=0x%08lx\n",   "PDTY1",        motor_regs->PDTY1));
    dbg1(( "%12s=0x%08lx\n",   "PDLY1",        motor_regs->PDLY1));
    dbg1(( "%12s=0x%08lx\n",   "ECFG",         motor_regs->ECFG));
    dbg1(( "%12s=0x%08lx\n",   "SReset",       motor_regs->SReset));
    dbg1(( "%12s=0x%08lx\n",   "DB",           motor_regs->DB));
    dbg1(( "%12s=0x%08lx\n",   "EPOS_RD",      motor_regs->EPOS_RD));
    dbg1(( "%12s=0x%08lx\n",   "EPOS_WR",      motor_regs->EPOS_WR));
    dbg1(( "%12s=0x%08lx\n",   "LPOS",         motor_regs->LPOS));
    dbg1(( "%12s=0x%08lx\n",   "APER",         motor_regs->APER));
    dbg1(( "%12s=0x%08lx\n",   "BPER",         motor_regs->BPER));
    dbg1(( "%12s=0x%08lx\n",   "TPER",         motor_regs->TPER));
    dbg1(( "%12s=0x%08lx\n",   "FILK1",        motor_regs->FILK1));
    //dbg1(( "%12s=0x%08lx\n",   "FILB1",        motor_regs->FILB1));
    //dbg1(( "%12s=0x%08lx\n",   "FILB2",        motor_regs->FILB2));
    //dbg1(( "%12s=0x%08lx\n",   "FILB3",        motor_regs->FILB3));
    dbg1(( "%12s=0x%08lx\n",   "FILA2",        motor_regs->FILA2));
    dbg1(( "%12s=0x%08lx\n",   "FILA3",        motor_regs->FILA3));
    //dbg1(( "%12s=0x%08lx\n",   "FILK2",        motor_regs->FILK2));
    dbg1(( "%12s=0x%08lx\n",   "FILSTAT",      motor_regs->FILSTAT));
    dbg1(( "%12s=0x%08lx\n",   "FILD1",        motor_regs->FILD1));
    dbg1(( "%12s=0x%08lx\n",   "FILD2",        motor_regs->FILD2));
    dbg1(( "%12s=0x%08lx\n",   "FILTOUT",      motor_regs->FILTOUT));
    dbg1(( "%12s=0x%08lx\n",   "RS_PM",        motor_regs->RS_PM));
    dbg1(( "%12s=0x%08lx\n",   "RS_DB",        motor_regs->RS_DB));
    dbg1(( "%12s=0x%08lx\n",   "RS_DLY",       motor_regs->RS_DLY));
    dbg1(( "%12s=0x%08lx\n",   "RS_OUT",       motor_regs->RS_OUT));
    dbg1(( "%12s=0x%08lx\n",   "RSTART",       motor_regs->RSTART));
    dbg1(( "%12s=0x%08lx\n",   "RSTOP",        motor_regs->RSTOP));
    dbg1(( "%12s=0x%08lx\n",   "IEN",          motor_regs->IEN));
    dbg1(( "%12s=0x%08lx\n",   "IPEND",        motor_regs->IPEND));
    dbg1(( "%12s=0x%08lx\n",   "IACK",         motor_regs->IACK));
    dbg1(( "%12s=0x%08lx\n",   "TS_CTRL",      motor_regs->TS_CTRL));
    dbg1(( "%12s=0x%08lx\n",   "TS_CNT",       motor_regs->TS_CNT));
    //dbg1(( "%12s=0x%08lx\n",   "TS_ENCA_0",    motor_regs->TS_ENCA_0));
    //dbg1(( "%12s=0x%08lx\n",   "TS_ENCA_1",    motor_regs->TS_ENCA_1));
    //dbg1(( "%12s=0x%08lx\n",   "TS_ENCA_2",    motor_regs->TS_ENCA_2));
    //dbg1(( "%12s=0x%08lx\n",   "TS_ENCB_0",    motor_regs->TS_ENCB_0));
    //dbg1(( "%12s=0x%08lx\n",   "TS_ENCB_1",    motor_regs->TS_ENCB_1));
    //dbg1(( "%12s=0x%08lx\n",   "TS_ENCB_2",    motor_regs->TS_ENCB_2));
    //dbg1(( "%12s=0x%08lx\n",   "TB_ENC_TS",    motor_regs->TB_ENC_TS));
    //dbg1(( "%12s=0x%08lx\n",   "TB_TS",        motor_regs->TB_TS));
    //dbg1(( "%12s=0x%08lx\n",   "TB_ENC_VALUE", motor_regs->TB_ENC_VALUE));
    dbg1(( "%12s=0x%08lx\n",   "PWATCH_CTRL",  motor_regs->PWATCH_CTRL));
    dbg1(( "%12s=0x%08lx\n",   "PWATCH_STAT",  motor_regs->PWATCH_STAT));
    dbg1(( "%12s=0x%08lx\n",   "WATCH_CTRL",   motor_regs->WATCH_CTRL));
    dbg1(( "%12s=0x%08lx\n",   "WATCH_VALUE",  motor_regs->WATCH_VALUE));
    dbg1(( "%12s=0x%08lx\n",   "WATCH_CNT",    motor_regs->WATCH_CNT));
    dbg1(( "%12s=0x%08lx\n",   "ENC_INT_CFG",  motor_regs->ENC_INT_CFG));
    dbg1(( "%12s=0x%08lx\n",   "TRIG_EPOS",    motor_regs->TRIG_EPOS));
    dbg1(( "%12s=0x%08lx\n",   "TRIG_CTRL",    motor_regs->TRIG_CTRL));
    //dbg1(( "%12s=0x%08lx\n",   "TRIG_DELAY",   motor_regs->TRIG_DELAY));
    //dbg1(( "%12s=0x%08lx\n",   "TRIG_CNT",     motor_regs->TRIG_CNT));
    dbg1(("\n"));
#endif
}

