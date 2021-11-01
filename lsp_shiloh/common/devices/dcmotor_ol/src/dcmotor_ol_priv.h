#ifndef DCMOTOR_PRIV_H
#define DCMOTOR_PRIV_H
/*
 * ============================================================================
 * Copyright (c) 2011-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/// \file dcmotor_priv.h

#include "dcmotor_ol.h"
#include "logger.h"
#include "devices_logger.h"
#include "posix_ostools.h"
#include "uio_lib_api.h"
#include "gpio_api.h"


#ifdef __cplusplus
extern "C" {
#endif

#define DBG_PRFX "dcmot: "                        // simple string prepend to the front of all strings.
//#define LOGGER_SUBMOD 0x8000              // (0x1 << 0x15) submodule/channel is this bit, if set then log.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | DEVICES_LOGGER_SUBMOD_DCMOTOR

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)          // LOG_ERR is the level
#define DBG_INFO(...) DBG_PRINTF(LOG_INFO, DBG_PRFX __VA_ARGS__)  
#define DBG_DEBUG(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)  
#define DBG_LOG_ERR(...) DBG_MEMLOG(LOG_ERR, DBG_PRFX __VA_ARGS__)                                                                        // 
#define DBG_LOG_NOTICE(...) DBG_MEMLOG(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_DEBUG(...) DBG_MEMLOG(LOG_DEBUG, DBG_PRFX __VA_ARGS__)                                                                        // 

#define LOGLEVEL 3

#if LOGLEVEL >= 1  
#define dbg1(x)  DPRINTF((DBG_LOUD | DBG_OUTPUT),x)
#define motor_dbg1(x) DPRINTF((DBG_LOUD | DBG_OUTPUT),x)
#else
#define dbg1(x)
#define motor_dbg1(x)

#endif

#if LOGLEVEL >= 2 
#define dbg2(x) DPRINTF((DBG_LOUD | DBG_OUTPUT), x) 
#define motor_dbg2(x) DPRINTF((DBG_LOUD | DBG_OUTPUT), x)
#else
#define dbg2(x)
#define motor_dbg2(x)
#endif

#if LOGLEVEL >= 3 
#define dbg3(x) DPRINTF(3, x) 
#else
#define dbg3(x)
#endif


#define NUM_DCMOTORS 2

typedef struct motor_param_s
{
    uint32_t pwm_duty_cycle;      /**< Duty cycle for this speed (percent 0 - 100) */
} motor_param_t;

struct dcmotor_speed_s
{
    uint32_t index; ///< if 0 then speed is zero
    const motor_param_t * move_params[NUM_DCMOTORS];
    uint32_t eta[NUM_DCMOTORS]; ///< milliseconds estimated to ramp to speed.
};
/**
 * \brief motor_use_t
 * 
 * Enum of use of motor
 * Different uses of motor require different control methods.
 */

typedef enum motor_use_e {
    scan_motor = 0,
    laser_motor,
    ink_drive_motor,
    laser_drive_motor
} motor_use_t;


/**
 *  \brief Motor ID type
 *
 *  Motor ID returned by create_motor, then used for all subsequent API calls.
 **/
typedef SINT8 motor_id_t;

typedef enum {SYNC_FIRST, SYNC_EVERY, SYNC_DISABLE} MOTOR_SYNC; // Line Sync Mode
/**
 *  \brief Motor direction type
 *
 *  Used to specify the direction of a motor move.  Note that the mech code
 *  configures the coordinate system and motor/encoder polarities, so the
 *  docs below are simply guidelines.
 **/
typedef enum {
    MOT_REVERSE = 0,           /**< Move causes decreasing encoder counts */
    MOT_FORWARD                /**< Move causes increasing encoder counts */
} motor_dir_t;


/**
 *  \brief DC Motor PWM Algorithm select
 *
 *  Selects the method used to apply the PWM (and direction) signals to the motor 
 *  controller.  In the normal mode, PWM is applied to PWM (enable) and Direction
 *  is applied to phase.  In the Phase mode, the PWM signal is applied to the phase
 *  input. Phase mode may be useful for motor controllers that do not support any 
 *  automatic current recirculation modes.
 **/
typedef enum {
    DC_PWM_NORMAL = 0,               /**< PWM signal applied to PWM      */
    DC_PWM_PHASE                     /**< PWM signal applied to PHASE    */
} motor_pwm_out_t;


/**
 *  \brief DC Motor block select
 *
 *  Selects the ASIC motor block to be used.
 **/
typedef enum {
    DC_MTR_BLK_0 = 0,                /**< ASIC motor block 0             */
    DC_MTR_BLK_1,                    /**< ASIC motor block 1             */
    DC_MTR_BLK_2,                    /**< ASIC motor block 2             */
    DC_MTR_BLK_3,                    /**< ASIC motor block 3             */
    DC_MTR_BLK_4,
} motor_block_t;


/**
 *  \brief DC Motor pin function select
 *
 *  Selects the function for an ASIC motor block pin.
 **/
typedef enum {
    DC_PIN_FUNC_NC = 0,            /**< Pin not connected                */
    DC_PIN_FUNC_PWM,               /**< Pin used for PWM                 */
    DC_PIN_FUNC_DIR,               /**< Pin used for DIR (Normal only)   */
    DC_PIN_FUNC_SLP,               /**< Pin used for sleep (Normal only) */
    DC_PIN_FUNC_MODE,              /**< Pin used for mode (Normal only)  */
    DC_PIN_FUNC_EN,                /**< Pin used for enable (Phase only) */
    DC_PIN_FUNC_ENCA,              /**< Pin used for encoder A           */
    DC_PIN_FUNC_ENCB,              /**< Pin used for encoder B           */
    DC_PIN_FUNC_DBG                /**< Pin used for debug signal        */
} motor_pin_func_t;


/**
 *  \brief DC Motor pin invert
 *
 *  Selects the polarity of ASIC motor pin functions.
 **/
typedef enum {
    DC_PIN_NO_INVERT = 0,            /**< Do not invert pin polarity     */
    DC_PIN_INVERT                    /**< Invert pin polarity            */
} motor_pin_invert_t;


/**
 *  \brief DC Motor pin signal
 *
 *  Selects the inital state or signal source for ASIC motor output pins.
 **/
typedef enum {
    DC_PIN_NO_SIGNAL = 0,            /**< No output signal required      */
    DC_PIN_ZERO,                     /**< Output a zero signal           */
    DC_PIN_ONE,                      /**< Output a one signal            */
    DC_PIN_PWM0,                     /**< Use the PWM signal as source   */
    DC_PIN_PWM1,                     /**< Use the PWM signal as source   */
    DC_PIN_DBG_INT_ROW_SYNC,         /**< Use Debug internal row sync    */
    DC_PIN_DBG_EXT_ROW_SYNC          /**< Use Debug external row sync    */
} motor_pin_signal_t;


/**
 *  \brief Motor pin configuration
 *
 *  Defines the configuration for a single motor pin, only used as part of the
 *  motor_connect_t structure.
 **/
typedef struct motor_pin_cfg_s
{
    motor_pin_func_t   func;      /**< Pin function                   */
    motor_pin_invert_t invert;    /**< Pin invert                     */
    motor_pin_signal_t signal;    /**< Output pin signal select       */
} motor_pin_cfg_t;
/**
 *  \brief Scan encoder types
 * 
 *  typical scan encoder configurations
 */
typedef enum {
    AB_NORMAL,                      /**< Normal AB encoder */
    AB_SWAPPED,                     /**< Swapped AB encoder */
    SINGLE_A,                       /**< Single-ended encoded on A */
    SINGLE_B                        /**< Single-ended encoded on B */
} motor_enc_t;
/**
 *  \brief Scan motor connections
 *
 *  Table used to specify how the 6 pins of the motor block are actually connected
 *  to the hardware.  A PWM_NORMAL connection will usually require DIR, PWM, SLP, 
 *  and MODE; a PWM_PHASE connection only requires PWM and EN. Problem is these
 *  signals may be connected to the motor block in a variety of ways, the mech
 *  code needs to tell us.
 **/
typedef struct motor_connect_s
{
    motor_pwm_out_t pwm_type;        /**< Type of PWM connection             */
    motor_block_t   block_num;       /**< ASIC motor block number            */
    motor_enc_t     enc_inputs;      /**< encoder A/B inputs                 */
    UINT32          sample_period;    /**< sample period in mSec             */
    motor_pin_cfg_t pin_cfg[6];      /**< DC0 - DC5 pin configuration        */
} motor_connect_t;


/**
 *  \brief Motor trigger conditions
 *
 *  Conditions/events that can be used to generate triggers.
 **/
typedef enum {
    TRIG_NULL,                    /**< No trigger set (clear trigger)     */
    TRIG_POSITION,                /**< Trigger on encoder value           */
    TRIG_VELOCITY,                /**< Trigger on speed                   */
    TRIG_MOTOR_STOP,              /**< Trigger after complete motor stop  */
    TRIG_MOTOR_START,             /**< Trigger on motor start             */
    TRIG_STATE                    /**< Trigger on specific motor state    */
} motor_trigger_cond_t;


#define INVALID_DC_MOTOR_ID   -1      /**< Invalid motor_id_t value            */
#define MTR_LD_BAD_AVE        -1      /**< Bad average motor load value        */


/**
 *  \brief Trigger callback function pointer
 *
 *  Function pointer to a motor trigger callback routine.
 **/
typedef void (*TriggerCbFuncPtr)(dcmotor_ol_t * dcmotor);


/**
 *  \brief Internal DC motor state
 *
 *  Internal state of a motor; type is exposed to support setting of TRIG_STATE
 *  triggers.
 **/
typedef enum
{
    MOT_STATE_STOP,             /**< Motor is stopped/idle                 */
    MOT_STATE_ACCEL,            /**< Accelerating to target speed          */
    MOT_STATE_WAIT_RAMP,        /**< Waiting for steady state to settle    */
    MOT_STATE_STEADY,           /**< Running at target velocity            */
    MOT_STATE_DECEL,            /**< Ramp down (NOT USED)                  */
    MOT_STATE_COAST,            /**< Coasting to stop, no PWM applied      */
    MOT_STATE_DISABLED          /**< Motor disabled, no moves until reboot */
} MotorState_t;

/* Define an 'unreachable' row sync start position.  We use this when disabling
 * row sync outputs; we should never hit this unless the encoder rolls over.
 * NOTE: don't pick a value that can be hit if the encoder rolls over by one
 *   tick, could hit that during a soft reset if the encoder block debounce
 *   twitches (can be +/- one count) ... I picked -4 counts
 */
#define UNREACHABLE_RS_START  (DCMOTOR_BASE_EPOS_RD_POS_MASK - 4)

/* Servo control constants */
//#define SERVO_MS              2          /* Every 2ms                               */
#define SERVO_ISR_SLOP        200        /* Scaled by 1000, so 0.20ms               */
#define ONE_MHZ               1000000
#define NANOSECS_PER_SECOND   1000000000
#define NANOSECS_PER_MILLISEC 1000000
#define MICROSECS_PER_SECOND  1000000

#define STOP_COUNTS           12         /* How many 0 counts before really stopped */

#define ENC_DB_PERCENT        25         /* Encoder debounce percentage             */

#define ENC_RESET_TIME_USEC   1          /* Enc reset time (may be in irq context)  */


/*
 * Motor Trigger: data structure to store client registered motor triggers
 */
typedef struct {
    motor_trigger_cond_t trigger_type;
    UINT32               trigger_param1;
    TriggerCbFuncPtr     trigger_function_ptr;
} motor_trigger_t;

typedef struct DCMOTOR_BASE_REGS_s MOTOR_REGS;
#define DCMOTOR_COOKIE  0x1341552e
struct dcmotor_ol_s
{
    int cookie;

    dcmotor_speed_t *current_speed; ///< ramping if not same as target
    dcmotor_speed_t *target_speed;  ///< ramping if not same as current
    sem_t semaphore;
    uint32_t motor_id;
    uio_dev_t* uio_dev;
    volatile MOTOR_REGS *motor_regs;   /* Motor block register base address       */
    uint32_t  motor_irq_num;             /* Motor block IRQ number                  */
    void (*smot_dc_enter_sleep_func)(dcmotor_ol_t * dcmotor);
    void (*smot_dc_exit_sleep_func)(dcmotor_ol_t * dcmotor);
    motor_use_t motor_use;
    bool use_encoder_isr;               ///< If true use the encoder ISR to update motor control values;
                                        /// If false motor control values are updated base on the timer ISR.    

    const motor_connect_t *mtr_connects; /* Motor pin connections                 */
    const motor_param_t *move_params;  /* Motor params for current move           */

    motor_pwm_out_t  pwm_method;       /* Motor PWM method                        */
    uint32_t  tick;                      /* Number of mSec between encoder measurements*/
    MotorState_t  motor_state;         /* Current motor state                     */
    uint8_t   move_direction;            /* Motor direction                         */
    uint32_t pwm_freq;                  /* PWM frequency     */

    uint32_t  pwm_period;                /* Max PWM period (based on freq, divisor) */
    uint32_t  pwm_divisor;               /* PWM divisor                             */
    int32_t  CtrlPWM;                   /* Controller output                       */  
    int32_t  rampCounts;                /* Logged ramp up counts                   */
    int32_t  decelCounts;               /* Logged deceleration counts              */
    int32_t  coastCounts;               /* Logged coast counts                     */
    uint32_t  stopCounts;                /* Velocity 0 counts (at end of coast)     */
    uint32_t  ctl_loop_count;            /* Passes through the control loop         */

    motor_trigger_t mtr_trigger;       /* Our one trigger event                   */
};
void dcmotor_create_motor(dcmotor_ol_t * dcmotor,
                          const motor_connect_t *mtr_connects);
/// customized startup initialization.
///
void dcmotor_local_init(void);

#ifdef __cplusplus
}
#endif
#endif
