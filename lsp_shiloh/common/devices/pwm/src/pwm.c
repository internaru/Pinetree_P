/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/// \file pwm.c


#include <stdlib.h> /* for atoi */
#include <stdint.h>
#include <stdbool.h>

#include "lassert.h"
#include "logger.h"
#include "debug.h"
#include "utils.h"

#include "regAddrs.h"
#include "DEC_CONFIG_regmasks.h"
#include "DEC_CONFIG_regstructs.h"

#include "pwm_api.h"
#include "pwm_priv.h"

#include "memAPI.h"

#if defined( HAVE_CMD ) && defined ( DEBUG )
#include "cmd_proc_api.h"
#endif

/// private structure for pwm control
///
///
struct pwm_s
{
    uint16_t delay_cnt;        ///< number to delay before repeating 1&2
    uint16_t first_semicycle;  ///< 
    uint16_t second_semicycle;                  
    bool first_cycle_high;     ///< output high/low for first cycle
    uint32_t pwm_number;       ///< hw block number 0-(n-1) pwm blocks.
    uint32_t timebase; 
    uint32_t off_state;

    uint32_t period_ticks;      ///< total period of the pwm in ticks of the current timebase;
                                ///< may be used to calculate first and second semicycle by percent

    uint32_t current_duty_cycle;

    DPWM_TOP_REGS_REGS_t *pwmtop;
    DPWM_REGS0_REGS_t *pwm_regs;
    DEC_CONFIG_REGS_REGS_t *dec_config;
};

static void pwm_run_current_cycle(const pwm_t *pwm);

/// Using this constructor allows single on off control
/// with the dec_dpwm_run call.
///
///
pwm_t *
pwm_config_construct(
    uint32_t pwm_block_num, 
    bool first_cycle_high, 
    uint16_t delay_cnt, 
    uint16_t first_semicycle,
    uint16_t second_semicycle,   
    bool off_is_low,
    dpwm_timebase_e timebase)
{
     pwm_t *pwm = (pwm_t *)MEM_MALLOC(sizeof(pwm_t));
     if (pwm) 
     {
         pwm->pwm_number = pwm_block_num;
         pwm->first_cycle_high = first_cycle_high;
         pwm->delay_cnt = delay_cnt;
         pwm->first_semicycle = first_semicycle;
         pwm->second_semicycle = second_semicycle;   
         pwm->timebase = timebase;
         pwm->pwmtop = (DPWM_TOP_REGS_REGS_t *)dec_pwm_get_regs_base(); 
         pwm->dec_config = (DEC_CONFIG_REGS_REGS_t *)dec_config_get_regs_base();
         pwm->pwm_regs = (DPWM_REGS0_REGS_t *)
             (dec_pwm_get_regs_base() + ((pwm_block_num+1)*0x100));
         pwm->off_state = off_is_low ? 0 : 1;

         pwm->pwm_regs->cfg = DPWM_REGS0_CFG_OFF_STATE_REPLACE_VAL(pwm->pwm_regs->cfg, pwm->off_state);

         pwm->period_ticks = first_semicycle + second_semicycle;
         pwm->current_duty_cycle = (first_semicycle == 0) ? 0 : (pwm->period_ticks*100/first_semicycle);

         // sanity check on register block sizes not changing!
         //DBG_ASSERT( DEC_DEC_PWM_DPWM_REGS4_BASE == ((dec_pwm_get_regs_base()) + ((4+1)*0x100)));
     }     
     return pwm;
}

/// Using this constructor allows the duty cycle to be specified at runtime as a
/// percent (0 - 100) of the period with the pwm_run_dutycycle_percent() call.
///
pwm_t * pwm_period_construct(
    uint32_t pwm_block_num, 
    bool first_cycle_high, 
    uint16_t delay_cnt, 
    bool off_is_low,
    dpwm_timebase_e timebase,
    uint32_t period,                ///< total period of the pwm in ticks of the current timebase
    uint32_t default_duty_cycle     ///< Default duty cycle to be specified at runtime as a percent (0 - 100) of the period
    )
{
     pwm_t *pwm = (pwm_t *)MEM_MALLOC(sizeof(pwm_t));
     DBG_ASSERT( pwm_block_num < 32 );

     if (pwm) 
     {
         ASSERT(default_duty_cycle <= 100);

         pwm->pwm_number = pwm_block_num;
         pwm->first_cycle_high = first_cycle_high;
         pwm->delay_cnt = delay_cnt;
         pwm->timebase = timebase;
         pwm->pwmtop = (DPWM_TOP_REGS_REGS_t *)dec_pwm_get_regs_base(); 
         pwm->dec_config = (DEC_CONFIG_REGS_REGS_t *)dec_config_get_regs_base();
         pwm->pwm_regs = (DPWM_REGS0_REGS_t *)
             (dec_pwm_get_regs_base() + ((pwm_block_num+1)*0x100));
         pwm->off_state = off_is_low ? 0 : 1;

         pwm->pwm_regs->cfg = DPWM_REGS0_CFG_OFF_STATE_REPLACE_VAL(pwm->pwm_regs->cfg, pwm->off_state);

         pwm->period_ticks = period;
         pwm->current_duty_cycle = default_duty_cycle;
         pwm->first_semicycle  = period * default_duty_cycle / 100;
         pwm->second_semicycle = period - pwm->first_semicycle;

         // sanity check on register block sizes not changing!
         //DBG_ASSERT( DEC_DEC_PWM_DPWM_REGS4_BASE == ((dec_pwm_get_regs_base()) + ((4+1)*0x100)));
     }     
     return pwm;
}

/// Most pwm's use a simple duty cycle based on the bit width.
///
///
pwm_t *
pwm_simple_construct(
    uint32_t pwm_block_num, 
    uint32_t width, ///< 2**width cycles to vary the duty cycle.
    dpwm_timebase_e timebase)
{
    DBG_ASSERT( pwm_block_num < 32 );

    // construct with first cycle low, no delay, off high, and period 2**width
    return pwm_period_construct(pwm_block_num, false, 0, false, timebase, (1<<width), 0);
}

/// Turn a PWM on at it's last configured duty cycle
void pwm_on(const pwm_t *pwm)
{
    pwm_run_current_cycle(pwm);
}

/// Turn a PWM off
void pwm_off(const pwm_t *pwm)   ///< which pwm to turn off
{
    ASSERT(pwm != NULL);
    if (pwm == 0 )
        return;

    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("pwm_off %d\n", pwm->pwm_number));

    pwm->pwmtop->en &= ~(1 << pwm->pwm_number);

    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_OFF_STATE_REPLACE_VAL(
       pwm->pwm_regs->cfg, 
       pwm->off_state);

    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_SOFT_DISABLE_REPLACE_VAL(
        pwm->pwm_regs->cfg, 
        0);

    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_ENABLE_REPLACE_VAL(
        pwm->pwm_regs->cfg, 0);

    pwm->pwmtop->en |= (1 << pwm->pwm_number);

#if ASIC_REV < ASIC_REV_Z3
    pwm->dec_config->oe_dpwm |= (1 << pwm->pwm_number);
#endif
}


uint16_t pwm_get_first_semicycle(const pwm_t *pwm)
{
    ASSERT(pwm);
    return pwm->first_semicycle;
}

uint16_t pwm_get_second_semicycle(const pwm_t *pwm)
{
    ASSERT(pwm);
    return pwm->second_semicycle;
}

static void
pwm_run_current_cycle(const pwm_t *pwm) 
{
    uint32_t enable = 1;
    uint32_t off_state;
    ASSERT(pwm != NULL);
    if (pwm == 0 )
        return;

    // validate dec register addresses.
    //DBG_ASSERT( DEC_DEC_PWM_DPWM_REGS4_BASE == ((dec_pwm_get_regs_base()) + ((4+1)*0x100)));

    pwm->pwmtop->en &= ~(1 << pwm->pwm_number);
    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_ENABLE_REPLACE_VAL(
        pwm->pwm_regs->cfg, 0);

    // pwm of 100% is a special case for the hardware.  If HW drives either cycle to 100% then
    // the other cycle is 0.  In this case HW drives as if disabled.  So at recommendation of
    // HW designer when trying to drive 100% we instead set HW to disable and drive the inverse
    // of the configured off state.  Note that we assume that if either cycle matches the period
    // then the caller is trying to drive 100%
    if(pwm->current_duty_cycle == 100 || 
       pwm->first_semicycle == pwm->period_ticks || 
       pwm->second_semicycle == pwm->period_ticks)
    {
        pwm->pwm_regs->count0 = 0;
        pwm->pwm_regs->count1 = 0;
        pwm->pwm_regs->count2 = 0;
        off_state = !pwm->off_state;
        enable = 0;
    }
    else
    {
        pwm->pwm_regs->count0 = pwm->delay_cnt;
        pwm->pwm_regs->count1 = pwm->first_semicycle;
        pwm->pwm_regs->count2 = pwm->second_semicycle;
        off_state = pwm->off_state;
    }

    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_HIGH_LOW_REPLACE_VAL(
        pwm->pwm_regs->cfg, 
        pwm->first_cycle_high ? 1 : 0
        );       // first semi-cycle is high or low

    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_TIMEBASE_SEL_REPLACE_VAL(
        pwm->pwm_regs->cfg, 
        pwm->timebase); 

    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_OFF_STATE_REPLACE_VAL(
       pwm->pwm_regs->cfg, 
       off_state);

    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_SOFT_DISABLE_REPLACE_VAL(
        pwm->pwm_regs->cfg, 
        0);   // soft disable when enable=0 

    pwm->pwm_regs->cfg = DPWM_REGS0_CFG_ENABLE_REPLACE_VAL(
        pwm->pwm_regs->cfg, 
        enable);

    pwm->pwmtop->en |= (1 << pwm->pwm_number);

#if ASIC_REV < ASIC_REV_Z3
    pwm->dec_config->oe_dpwm |= (1 << pwm->pwm_number);
#endif
}

void
pwm_run_dutycycle_percent(pwm_t *pwm, uint32_t dutycycle_percent)
{
    pwm->current_duty_cycle = dutycycle_percent;

    pwm->first_semicycle  = pwm->period_ticks * pwm->current_duty_cycle / 100;
    pwm->second_semicycle = pwm->period_ticks - pwm->first_semicycle;

    if (pwm->current_duty_cycle == 0)
    {
        pwm_off(pwm);
    }
    else
    {
        // apply new settings
        pwm_run_current_cycle(pwm); 
    }
}

void
pwm_run_full(pwm_t *pwm, 
	     bool first_cycle_high, 
	     uint16_t delay_cnt, 
	     uint16_t first_semicycle,
	     uint16_t second_semicycle,
	     int timebase)
{
    if (pwm == 0 )
        return;

    // validate dec register addresses.
    //DBG_ASSERT( DEC_DEC_PWM_DPWM_REGS4_BASE == ((dec_pwm_get_regs_base()) + ((4+1)*0x100)));

    ASSERT( delay_cnt + first_semicycle + second_semicycle < 0x0000ffff );

    // reconfigure pwm
    pwm->delay_cnt = delay_cnt;
    pwm->first_semicycle = first_semicycle;
    pwm->second_semicycle = second_semicycle;
    pwm->first_cycle_high = first_cycle_high;
    XASSERT(timebase >= -1 && timebase <= 6, timebase);
    if(timebase != -1)  // test for caller specifying timebase to not change.
    {
        pwm->timebase = timebase;        
    }
    pwm->current_duty_cycle = (first_semicycle == 0) ? 0 : (pwm->period_ticks*100/first_semicycle);

    // apply new settings
    pwm_run_current_cycle(pwm); 

}

void
pwm_change_duty_cycle(pwm_t *pwm, 
              uint16_t first_semicycle,
              uint16_t second_semicycle)
{
    if (pwm == 0 )
        return;

    // validate dec register addresses.
    //DBG_ASSERT( DEC_DEC_PWM_DPWM_REGS4_BASE == ((dec_pwm_get_regs_base()) + ((4+1)*0x100)));

    // reconfigure pwm
    pwm->first_semicycle = first_semicycle;
    pwm->second_semicycle = second_semicycle;
    pwm->current_duty_cycle = (first_semicycle == 0) ? 0 : (pwm->period_ticks*100/first_semicycle);

    // apply new settings
    pwm_run_current_cycle(pwm); 
}

void
pwm_change_timebase(pwm_t *pwm, 
					uint8_t mode)
{
	if (pwm == 0 )
        return;
    
    pwm->timebase = (mode==1)?eDPWM_Timebase_1us:eDPWM_Timebase_Clks;
	return;
}

void
pwm_run(pwm_t *pwm, 
        uint16_t speed
    )
{
    uint16_t inverse_speed; 
    
    ASSERT(pwm != NULL);
    ASSERT(speed <= pwm->period_ticks);

    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("pwm_run %d speed = %d \n", pwm->pwm_number, speed));

    if(speed == 0)
    {
        pwm_off(pwm);
    }
    else
    {
        // reconfigure pwm
        inverse_speed = pwm->period_ticks - speed;
    
        pwm->first_semicycle = inverse_speed;   // first semicycle 
        pwm->second_semicycle = speed;           // second semicycle   
        pwm->current_duty_cycle = (pwm->first_semicycle == 0) ? 0 : (pwm->period_ticks*100/pwm->first_semicycle);

        // apply new settings
        pwm_run_current_cycle(pwm); 
    }
}

#if defined( HAVE_CMD ) && defined ( DEBUG )

void pwm_debug_usage()
{
    dbg_printf("pwm command options:\n");
    dbg_printf("    display - show currently configured pwms\n");
    dbg_printf("    speed [pwm_id] [val] - set pwm speed explictly\n");
    dbg_printf("    pct [pwm_id] [val] - set pwm duty cycle as a percent of the current period\n");
}

void pwm_debug_cmd_err()
{
    dbg_printf("PWM command error or invalid command.\n");
    pwm_debug_usage();
}

int pwm_debug_callback( int argc, char *argv[] )
{
    int pos = 1;
    int i;
    const char* str;
    int pwm_num;
    uint32_t arg_val;

    if( argc < 2 || str_match( argv[pos], "help", 4 )) 
    {
        pwm_debug_usage();
        /* not enough parameters */
        return 0;
    }

    if( str_match(argv[pos], "display", 7 ) ) 
    {
        for (i = 0, str = pwm_name_by_index(i); 
             str ; 
             i++, str = pwm_name_by_index(i) )
        {
            dbg_printf(" pwm [%d] %s\n", i, str);
        }
        goto DONE;
    }

    // all subsequent commands assume arg count of 4
    if(argc < 4)
    {
        pwm_debug_cmd_err();
        goto DONE;
    }
    pwm_num = atoi(argv[pos+1]);
    arg_val = atoi(argv[pos+2]);

    if( str_match(argv[pos], "speed", 5 ) ) 
    {
        pwm_run( pwm_by_index(pwm_num), arg_val );
    }
    else if( str_match(argv[pos], "pct", 3 ) ) 
    {
        pwm_run_dutycycle_percent(pwm_by_index(pwm_num), arg_val);
    }
    else
    {
        dbg_printf("Failed to match command %s", argv[pos]);
        pwm_debug_cmd_err();
    }
    
DONE:
    return 0;
}
#endif

void pwm_init()
{
    pwm_local_init();  
#if defined( HAVE_CMD ) && defined ( DEBUG )
    DBG_ASSERT( CMD_OK == cmd_register( "pwm",
                                        NULL,
                                        NULL,
                                        NULL,
                                        pwm_debug_callback ) );
#endif
}
