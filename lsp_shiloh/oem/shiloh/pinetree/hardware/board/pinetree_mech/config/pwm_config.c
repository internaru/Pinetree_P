/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "lassert.h"
#include "logger.h"
#include "debug.h"

#include "regAddrs.h"
#include "uio_lib_api.h"
#include "key_table.h"
#include "pwm_api.h"
#include "pwm_priv.h"
#include "logger.h"

//// FANS
#define PWM_MAIN_FAN 0 
#define PWM_SUB_FAN 1
//// DEC high voltage PWMS
#define DEC_PWM_TRANSFER    2
#define DEC_PWM_CHARGE      3
#define DEC_PWM_DEVELOPER   4
//// MAIN MOTOR
#define PWM_MAIN_MOTOR   5
//// +FANS
#define PWM_DEV_FAN 6


const const char * dec_pwm_key_table[] = 
{
    "main_fan", 
    "sub_fan",
    "pwm_transfer",
    "pwm_charge",
    "pwm_developer",
    "main_motor",
    "dev_fan",
    0
};

static pwm_t *mypwms[8] = {0, };  // last must be null

KEY_TABLE_DEFINE_PRIVATE_2( pwm, pwm_t, dec_pwm_key_table, mypwms );


/**
 * \brief Get dec pwm registers memory mapped to user space using dec_pwm UIO kernel driver.
 * 
 */
void *dec_pwm_get_regs_base(void)
{
    static void *pwm_dev = NULL;
    static void *pwm_regs = NULL;

    if(pwm_dev == NULL)
    {
        pwm_dev = uio_open_dev("dec_pwm");
        XASSERT(pwm_dev != NULL, errno);
    }

    if(pwm_regs == NULL)
    {
        // get dec_pwm map
        uio_map_t *map = uio_get_map(pwm_dev, 0);

        if (map != NULL)
        {
            pwm_regs = map->virt_addr;
        }
    }

    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s = %#x\n", __func__, pwm_regs));

    return pwm_regs;
}

/**
 * \brief Get dec config registers memory mapped to user space using dec_pwm UIO kernel driver.
 * 
 */
void *dec_config_get_regs_base(void)
{
    static void *config_dev = NULL;
    static void *config_regs = NULL;

    if(config_dev == NULL)
    {
        config_dev = uio_open_dev("dec_config");
        XASSERT(config_dev != NULL, errno);
    }

    if(config_regs == NULL)
    {
        // get dec_config map
        uio_map_t *map = uio_get_map(config_dev, 0);

        if (map != NULL)
        {
            config_regs = map->virt_addr;
        }
    }

    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s = %#x\n", __func__, config_regs));

    return config_regs;
}

void pwm_local_init()
{
    /// map / enable the pin bypass to connect the board line to the asic block.


    // construct 2 variable speed fans using pwm control.
    // pwm block number, bit width of pwm, timebase
//    mypwms[PWM_MAIN_FAN] = pwm_simple_construct(13, 8, eDPWM_Timebase_1us );
//    mypwms[PWM_SUB_FAN] = pwm_simple_construct(14, 8, eDPWM_Timebase_1us );

    #define MAIN_FAN_PWM_PERIOD 333
    #define MAIN_FAN_DUTY_CYCLE 0.50
//    #define MAIN_FAN_DUTY_CYCLE 1
    #define MAIN_FAN_ACTIVE_PULSE (MAIN_FAN_PWM_PERIOD * MAIN_FAN_DUTY_CYCLE)
    #define MAIN_FAN_INACTIVE_PULSE (MAIN_FAN_PWM_PERIOD - MAIN_FAN_ACTIVE_PULSE)

    mypwms[PWM_MAIN_FAN] = pwm_config_construct( 13,    // block number.
                                                true, // first cycle high
                                                0,     // no delay 
                                                MAIN_FAN_ACTIVE_PULSE,    // first cycle
                                                MAIN_FAN_INACTIVE_PULSE,  // second cycle
                                                true, // off is high
                                                eDPWM_Timebase_1us // clock
                                                 );

    #define SUB_FAN_PWM_PERIOD 333
    #define SUB_FAN_DUTY_CYCLE 0.50
    #define SUB_FAN_ACTIVE_PULSE (SUB_FAN_PWM_PERIOD * SUB_FAN_DUTY_CYCLE)
    #define SUB_FAN_INACTIVE_PULSE (SUB_FAN_PWM_PERIOD - SUB_FAN_ACTIVE_PULSE)

    mypwms[PWM_SUB_FAN] = pwm_config_construct( 14,    // block number.
                                                true, // first cycle high
                                                0,     // no delay 
                                                SUB_FAN_ACTIVE_PULSE,    // first cycle
                                                SUB_FAN_INACTIVE_PULSE,  // second cycle
                                                true, // off is high
                                                eDPWM_Timebase_1us // clock
                                                );

    #define DEV_FAN_PWM_PERIOD 333
    #define DEV_FAN_DUTY_CYCLE 0.50
    #define DEV_FAN_ACTIVE_PULSE (DEV_FAN_PWM_PERIOD * DEV_FAN_DUTY_CYCLE)
    #define DEV_FAN_INACTIVE_PULSE (DEV_FAN_PWM_PERIOD - DEV_FAN_ACTIVE_PULSE)

    mypwms[PWM_DEV_FAN] = pwm_config_construct( 7,    // block number.
                                                true, // first cycle high
                                                0,     // no delay 
                                                DEV_FAN_ACTIVE_PULSE,    // first cycle
                                                DEV_FAN_INACTIVE_PULSE,  // second cycle
                                                true, // off is high
                                                eDPWM_Timebase_1us // clock
                                                );

    // All high voltage pwms run at 24.41Khz.  This is a period of ~41us.
    // They are active high and have a 2us delay between falling edges.
    // The Transfer PWM has a Print duty cycle of 70.1% (180 of 255).
    // The Transfer PWM has a IP duty cycle of 24.1% (69 of 255).
    // The Transfer PWM has a clean duty cycle of 0% (0 of 255).
    // The Charge PWM has a duty cycle of 81.6% (208 of 255).
    // The Developer Print PWM has a duty cycle of 62.4% (159 of 255).
    // The Developer No-Print PWM has a duty cycle of 7.8% (20 of 255)
    // The MainMotor PWM has a duty cycle of 50% (127 of 255)

    #define HV_PWM_PERIOD 8160   	// 200MHz/8160 = 24.5KHz			//41        // period in us
    #define HV_PWM_PERIOD_MTR 73449//74710   	// 200MHz/74710 = 24.5KHz			//41        // period in us
	
    #define TRANSFER_PRINT_DUTY_CYCLE 0.701
    #define TRANSFER_IP_DUTY_CYCLE 0.241
    #define TRANFER_CLEAN_DUTY_CYCLE 0
    #define TRANSFER_DUTY_CYCLE TRANFER_CLEAN_DUTY_CYCLE  // default to clean duty cycle

    #define CHARGE_DUTY_CYCLE 0.816

    #define DEVELOPER_PRINT_DUTY_CYCLE 0.624
    #define DEVELOPER_NO_PRINT_DUTY_CYCLE 0.078  
    #define DEVELOPER_DUTY_CYCLE DEVELOPER_NO_PRINT_DUTY_CYCLE  // default to no-print duty cycle
	
	#define MAINMOTOR_DUTY_CYCLE 0.5
	
    #define TRANSFER_ACTIVE_PULSE (HV_PWM_PERIOD * TRANSFER_DUTY_CYCLE)
    #define TRANSFER_INACTIVE_PULSE (HV_PWM_PERIOD - TRANSFER_ACTIVE_PULSE)
    #define CHARGE_ACTIVE_PULSE (HV_PWM_PERIOD * CHARGE_DUTY_CYCLE)
    #define CHARGE_INACTIVE_PULSE (HV_PWM_PERIOD - CHARGE_ACTIVE_PULSE)
    #define DEVELOPER_ACTIVE_PULSE (HV_PWM_PERIOD * DEVELOPER_DUTY_CYCLE)
    #define DEVELOPER_INACTIVE_PULSE (HV_PWM_PERIOD - DEVELOPER_ACTIVE_PULSE)
    
    #define MAINMOTOR_ACTIVE_PULSE (HV_PWM_PERIOD_MTR * MAINMOTOR_DUTY_CYCLE)
    #define MAINMOTOR_INACTIVE_PULSE (HV_PWM_PERIOD_MTR - MAINMOTOR_ACTIVE_PULSE)

	mypwms[PWM_MAIN_MOTOR] = pwm_config_construct( 5,    // block number.
                                                true, // first cycle high
                                                0,     // no delay 
                                                MAINMOTOR_ACTIVE_PULSE,    // first cycle
                                                MAINMOTOR_INACTIVE_PULSE,  // second cycle
                                                true, // off is low
                                                eDPWM_Timebase_Clks		//eDPWM_Timebase_1us // clock

        );
        
    mypwms[DEC_PWM_TRANSFER] = pwm_config_construct( 22,    // block number.
                                                true, // first cycle high
                                                0,     // no delay 
                                                TRANSFER_ACTIVE_PULSE,    // first cycle
                                                TRANSFER_INACTIVE_PULSE,  // second cycle
                                                true, // off is low
                                                eDPWM_Timebase_Clks		//eDPWM_Timebase_1us // clock

        );

    mypwms[DEC_PWM_CHARGE] = pwm_config_construct( 23,    // block number.
                                               true, // first cycle high
                                               2,     // delay us
                                               CHARGE_ACTIVE_PULSE,    // first cycle
                                               CHARGE_INACTIVE_PULSE,  // second cycle
                                               true, // off is low
                                               eDPWM_Timebase_Clks		//eDPWM_Timebase_1us // clock
        );

    mypwms[DEC_PWM_DEVELOPER] = pwm_config_construct( 24,    // block number.
                                                     true, // first cycle high
                                                     4,     // delay us
                                                     DEVELOPER_ACTIVE_PULSE,    // first cycle
                                                     DEVELOPER_INACTIVE_PULSE,  // second cycle
                                                     true, // off is low
                                                     eDPWM_Timebase_Clks		//eDPWM_Timebase_1us // clock
        );

    DBG_ASSERT( mypwms[PWM_DEV_FAN+1] == 0 );     // must be null after last entry.
    DBG_ASSERT( 0 == PWM_MAIN_FAN );  // validate enum order
}



