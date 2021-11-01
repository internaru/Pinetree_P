#ifndef DCMOTOR_PRIV_H
#define DCMOTOR_PRIV_H
/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/// \file dcmotor_priv.h

#include "dcmotor.h"
#include "logger.h"
#include "devices_logger.h"
#include "posix_ostools.h"
#include "uio_lib_api.h"

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
    UINT32 speed;                 /**< Motor speed (quads per irq)        */
    UINT32 acceleration;          /**< Acceleration (quads per irq)       */
    UINT32 bias;
    UINT32 bias_accel;
    SINT32 kp;                    /**< PID: proportional constant         */
    SINT32 ki;                    /**< PID: integral constant             */
    SINT32 kd;                    /**< PID: derivative constant (unused)  */
    UINT8  rs_oscale;             /**< Sync every: output period scale    */
    UINT8  rs_mscale;             /**< Sync every: multiplier scale       */
    UINT16 rs_pm;                 /**< Sync every: out period multiplier  */
    UINT8  enc_iscale;            /**< Encoder period iScale              */
    SINT32 pos_error_limit;       /**< Max position error                 */
    SINT32 maxPwmHigh;            /**< Dyn PWM: Max allowed this mode     */
    SINT32 maxPwmLow;             /**< Dyn PWM: Min allowed this mode     */
    SINT32 velTreshHigh;          /**< Dyn PWM: vel thresh to high pwm    */
    SINT32 velTreshLow;           /**< Dyn PWM: vel thresh to low pwm     */
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
 *  \brief Scan motor constants
 *
 *  Parameters for the physical motor (typically found in the motor specs),
 *  which includes the attached encoder.  Note: How the motor hooks to the
 *  mechanism isn't a concern here (things like gear ratio or quads per inch).
 **/
typedef struct motor_const_s
{
    float  Kt;                    /**< Motor torque const (N*cm/amp)      */
    float  Kv;                    /**< Voltage generation const (V/Krpm)  */
    float  Km;                    /**< Motor constant (N*cm/sqrt(watt))   */
    UINT32 term_resist;           /**< Motor terminal resistance (ohms)   */
    UINT32 encoder_cpr;           /**< Encoder counts per revolution      */
    UINT32 quads_rev;             /**< Quad counts per motor revolution   */
    UINT32 motor_voltage;         /**< Motor supply voltage               */
} motor_const_t;


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
typedef void (*TriggerCbFuncPtr)(dcmotor_t * dcmotor);


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


/**
 *  \brief Possible OCC Error Conditions
 *
 *  Conditions that the motor code can detect and provide to a registered OCC routine.
 *  A little background on how each of the supported conditions can arise:
 *   - OCC_MOTOR_STALL:  
 *           As the motor is being controlled we keep track of our position error.  If
 *           the error gets too large the motor code will halt the motor.  This can 
 *           happen when ramming the wall (either on purpose during home find or not), 
 *           or when the scanbar becomes stuck.  The mech code knows if this is expected
 *           or not, but needs to know or strange hangs can result.
 *   - OCC_MTR_NO_LOAD:   
 *           Trigger event when motor load drops below a set point.  Note that you
 *           must set the trigger level (see smot_dc_set_load_trigger) before 
 *           registering for a no load OCC. You should also specify a settle time for
 *           you specific SSA (see smot_dc_set_load_settle_time).
 **/
typedef enum
{
    OCC_MTR_STALL = 0,           /**< Motor stall (normal when ramming wall)  */
    OCC_MTR_NO_LOAD,             /**< Below no motor load threshold           */
    OCC_NUM_ERRS                 /**< This must always be LAST!               */
} OCCErrorCodes_t;

/* Define an 'unreachable' row sync start position.  We use this when disabling
 * row sync outputs; we should never hit this unless the encoder rolls over.
 * NOTE: don't pick a value that can be hit if the encoder rolls over by one
 *   tick, could hit that during a soft reset if the encoder block debounce
 *   twitches (can be +/- one count) ... I picked -4 counts
 */
#define UNREACHABLE_RS_START  (DCMOTOR_BASE_EPOS_RD_POS_MASK - 4)
/**
 *  \brief OCC function pointer
 *
 *  Function pointer to an OCC (error callback) routine.
 **/
typedef void (*OCCFuncPtr)(OCCErrorCodes_t ErrorCode);
/* Servo control constants */
//#define SERVO_MS              2          /* Every 2ms                               */
#define SERVO_ISR_SLOP        200        /* Scaled by 1000, so 0.20ms               */
#define ONE_MHZ               1000000
#define NANOSECS_PER_SECOND   1000000000
#define NANOSECS_PER_MILLISEC 1000000
#define MICROSECS_PER_SECOND  1000000

#define DC_PWM_FREQ           23000      /* Does this need to be configurable?      */

#define STOP_COUNTS           12         /* How many 0 counts before really stopped */

#define ENC_DB_PERCENT        25         /* Encoder debounce percentage             */

#define ENC_RESET_TIME_USEC   1          /* Enc reset time (may be in irq context)  */


/*
 * Motor Load:
 *   We track the average motor load (motor current) over a fixed rolling time period 
 *   to allow the mech code to request OCC events if the motor load unexpectedly changes.
 *
 *     Rolling window length = SERVO_MS (2ms) * LD_AVG_SIZE (375) = 0.75 second
 *
 * Note that we don't start taking the current average until we finish the motor ramp
 * and hit steady state. The configured load settle time must also pass before load 
 * data is collected.
 */
#define LD_AVG_SIZE          375       /* Data points for load rolling average    */

typedef struct {
    SINT16  LdRollAvg[LD_AVG_SIZE];    /* Data for load rolling average           */
    SINT32  LdTotal;                   /* Total of all stored load values         */
    UINT32  LdAvgIdx;                  /* Current index into average array        */
    BOOL    LdAvgValid;                /* Is array valid (fully populated)        */
    UINT32  LdAvgSettleCount;          /* Motor load settle counter               */
    UINT32  LdAvgSettleLimit;          /* Motor load settle limit                 */
    UINT32  LdValidSamples;            /* Debug: number of valid load samples     */
} motor_load_t;

/*
 * Motor Duty Cycle:
 *   We track motor run time (duty cycle) over a fixed period of time to enable
 *   the mech code to implement features like motor overheat.  This implementation
 *   uses an array of data buckets, each bucket will keep track of DUTY_BUCKET_SECS
 *   seconds worth of motor run data.  curDutyIdx will point to the current bucket,
 *   the start of which is at time curDutyTime.  Be aware that the motor code is
 *   not always running, so we must take care to make sure the bucket data hasn't
 *   gone stale.
 *
 *   Duty cycle time window = 15 buckets * 60 seconds = 15 minutes
 */

#ifdef TRACK_DCMOTOR_DUTYCYCLE
#define DUTY_BUCKETS      15               /* Number of buckets                 */
#define DUTY_BUCKET_SECS  60               /* How many seconds in each bucket?  */
#define DUTY_BUCKET_TOTAL (DUTY_BUCKET_SECS * DUTY_BUCKETS)

typedef struct {
    UINT8             curDutyIdx;           /* Current usage bucket index        */
    unsigned long int curDutyTime;          /* Starting time of current bucket   */
#if 1
    struct timespec   usage[DUTY_BUCKETS];  /* The bucket list (not the movie)   */
#endif
} motor_duty_t;
#endif
/*
 * Motor Trigger: data structure to store client registered motor triggers
 */
typedef struct {
    motor_trigger_cond_t trigger_type;
    UINT32               trigger_param1;
    TriggerCbFuncPtr     trigger_function_ptr;
} motor_trigger_t;
#define DCMOTOR_COOKIE  0x1341552e
struct dcmotor_s
{
    int cookie;

    dcmotor_speed_t *current_speed; ///< ramping if not same as target
    dcmotor_speed_t *target_speed;  ///< ramping if not same as current
    sem_t semaphore;
    uint32_t motor_id;
    uio_dev_t* uio_dev;
    volatile MOTOR_REGS *motor_regs;   /* Motor block register base address       */
    uint32_t  motor_irq_num;             /* Motor block IRQ number                  */
    void (*smot_dc_enter_sleep_func)(dcmotor_t * dcmotor);
    void (*smot_dc_exit_sleep_func)(dcmotor_t * dcmotor);
    motor_use_t motor_use;
    bool use_encoder_isr;               ///< If true use the encoder ISR to update motor control values;
                                        /// If false motor control values are updated base on the timer ISR.                                      
    void * laser;
#define ERROR_FILTER_SIZE 10
    int32_t error_filter[ERROR_FILTER_SIZE];
    int32_t error_filter_size;
    int32_t error_filter_pos;
    int32_t * error_filter_terms_B_minus;
    int32_t * error_filter_terms_B_plus;
    const motor_connect_t *mtr_connects; /* Motor pin connections                 */
    const motor_const_t *mtr_consts;   /* Motor constants structure pointer       */
    const motor_param_t *move_params;  /* Motor params for current move           */

    motor_pwm_out_t  pwm_method;       /* Motor PWM method                        */
    uint32_t  tick;                      /* Number of mSec between encoder measurements*/
    MotorState_t  motor_state;         /* Current motor state                     */
    uint8_t   move_direction;            /* Motor direction                         */
    int32_t  curr_velocity;             /* Velocity from this ISR sample time      */
    int32_t  prev_velocity;             /* Velocity from last ISR sample time      */
    int32_t  prev_vel_error;
    int32_t  sum_error;
    uint32_t  MeasuredPosTime0;          /* Current encoder position                */
    uint32_t  calcPosTime0;              /* Current encoder position                */
    uint32_t  MeasuredPosTimeMinus1;     /* Position last ISR sample time           */
    uint32_t  period_ref;                /* Number of ticks in a int*/
    uint32_t  fbits;                     /* Encoder value scaling bits              */
    int32_t  PosErrCap;                 /* Debug: limit on accumulated pos error   */
    int32_t  RefVel;                    /* Reference velocity (proportional ref)   */
    int32_t  RefBias;
    int32_t  RefPos;                    /* Reference position (integral ref)       */
    uint32_t  pwm_period;                /* Max PWM period (based on freq, divisor) */
    uint32_t  prev_pwm_period;
    int32_t  sum_pwm_period;
#define NUM_VOLTAGE_SUMS 32
    uint32_t  voltage[NUM_VOLTAGE_SUMS];
    int32_t  voltage_error[NUM_VOLTAGE_SUMS];
    uint32_t  voltage_index;
    uint32_t  voltage_sum;
    int32_t  voltage_error_sum;
    uint32_t  pwm_divisor;               /* PWM divisor                             */
    bool    RunForever;                /* Don't go to set point, keep going       */
    uint32_t  MoveDistance;              /* Target move distance                    */
    uint32_t  MoveCount;                 /* Actual move distance                    */
    int32_t  CtrlPWM;                   /* Controller output                       */  
    int32_t  prev_CtrlPWM;              /* Previous controller output              */
    int32_t  rampCounts;                /* Logged ramp up counts                   */
    int32_t  decelCounts;               /* Logged deceleration counts              */
    int32_t  coastCounts;               /* Logged coast counts                     */
    uint32_t  stopCounts;                /* Velocity 0 counts (at end of coast)     */
    uint32_t  ctl_loop_count;            /* Passes through the control loop         */

    OCCFuncPtr MechOCCFunc;            /* Mech error callback function            */
    bool    OCCSent[OCC_NUM_ERRS];     /* OCC sent for this event?                */
    bool    OCCEnable[OCC_NUM_ERRS];   /* OCC enabled for this event type?        */
    int32_t  NoLdTarget;                /* OCC no load target (mA)                 */
#if 1

    motor_load_t    mtr_load;          /* Motor load (current) data               */
#ifdef POSIX_AVAIL
    motor_duty_t    mtr_duty;          /* Motor duty cycle data                   */
#endif
    motor_trigger_t mtr_trigger;       /* Our one trigger event                   */
#endif
};
void dcmotor_create_motor(dcmotor_t * dcmotor,
                          const motor_connect_t *mtr_connects,
                          const motor_const_t   *mtr_constants,
                          UINT8                  enc_scale_fbits);
/// customized startup initialization.
///
void dcmotor_local_init(void);

#ifdef __cplusplus
}
#endif
#endif
