/*
 * ============================================================================
 * Copyright (c) 2006-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file mtr6pin.h
 *
 * \brief declarations and macros to support interface to motor block DCMOTOR6PIN
 *
 **/

#ifndef MTR6PIN_H
#define MTR6PIN_H

#define MOTOR_REGS DCMOTOR_BASE_REGS_t


/* MCFG pin select values */
#define MTR6PIN_MCFG_PINSEL_OUT_DEFAULT 0
#define MTR6PIN_MCFG_PINSEL_OUT_0       0
#define MTR6PIN_MCFG_PINSEL_OUT_1       1
#define MTR6PIN_MCFG_PINSEL_OUT_PWM0    2
#define MTR6PIN_MCFG_PINSEL_OUT_PWM1    3
#define MTR6PIN_MCFG_PINSEL_OUT_INT_RS  4
#define MTR6PIN_MCFG_PINSEL_OUT_EXT_RS  5

/* MCFG Enable values */
#define MTR6PIN_MCFG_DISABLE            0
#define MTR6PIN_MCFG_ENABLE             1

/* MCFG Direction values */
#define MTR6PIN_MCFG_DIR_REV            0
#define MTR6PIN_MCFG_DIR_FWD            1

/* MCFG Sleep values */
#define MTR6PIN_MCFG_WAKE               0
#define MTR6PIN_MCFG_SLEEP              1

/* MCFG Invert Bit Positions */
#define MTR6PIN_MCFG_INVERT             1    /* Bit setting for inverted      */
#define MTR6PIN_MCFG_NO_INVERT          0    /* Bit setting for not inverted  */

#define MTR6PIN_MCFG_INV_PIN5         5    /* Bit position 5, ex (1<<5)     */
#define MTR6PIN_MCFG_INV_PIN4         4    /* Bit position 4, ex (1<<4)     */
#define MTR6PIN_MCFG_INV_PIN3         3    /* Bit position 3, ex (1<<3)     */
#define MTR6PIN_MCFG_INV_PIN2         2    /* Bit position 2, ex (1<<2)     */
#define MTR6PIN_MCFG_INV_PIN1         1    /* Bit position 1, ex (1<<1)     */
#define MTR6PIN_MCFG_INV_PIN0         0    /* Bit position 0, ex (1<<0)     */

/* ECFG Encoder configuration Zpmode zero position modes */
#define MTR6PIN_ZPMODE_ZERO_TO_ONE  0
#define MTR6PIN_ZPMODE_ONE_TO_ZERO  1
#define MTR6PIN_ZPMODE_IGNORE       2
#define MTR6PIN_ZPMODE_IGNORE2      3

/* ECFG Encoder configuration RowSync modes */
#define MTR6PIN_RS_TOGGLE   0
#define MTR6PIN_RS_PULSE    1
#define MTR6PIN_RS_ROWSYNC  2
#define MTR6PIN_RS_ROWSYNC2 3

/* ECFG Encoder configuration TbaseSel timebase values */
#define MTR6PIN_TBSEL_100_MS    0
#define MTR6PIN_TBSEL_10_MS     1
#define MTR6PIN_TBSEL_1_MS      2
#define MTR6PIN_TBSEL_100_US    3
#define MTR6PIN_TBSEL_10_US     4
#define MTR6PIN_TBSEL_1_US      5

/* ECFG Encoder configuration Imode input modes */
#define MTR6PIN_IMODE_AB        0
#define MTR6PIN_IMODE_A         1
#define MTR6PIN_IMODE_B         2
#define MTR6PIN_IMODE_AB_SWAP   3

/* ECFG Encoder configuration Pmode values */
#define MTR6PIN_PMODE_A         0
#define MTR6PIN_PMODE_B         1
#define MTR6PIN_PMODE_AB        2

/* ECFG Encoder configuration Rmode values */
#define MTR6PIN_RMODE_NOFILTER_EDGE   0
#define MTR6PIN_RMODE_NOFILTER_CYCLE  1
#define MTR6PIN_RMODE_FILTER_EDGE     2
#define MTR6PIN_RMODE_FILTER_CYCLE    3

/* Map Platform dependent bit masks to their platform independent counterparts */
/* Interrupt enable bits */
#define IEN_ENCPOS_MASK DCMOTOR_BASE_IEN_ENCPOS_MASK
#define IEN_IENC_MASK   DCMOTOR_BASE_IEN_IENC_MASK
#define IEN_IPER_MASK   DCMOTOR_BASE_IEN_IPER_MASK
#define IEN_FILR_MASK   DCMOTOR_BASE_IEN_FILR_MASK
#define IEN_OUTR_MASK   DCMOTOR_BASE_IEN_OUTR_MASK
#define IEN_WATCH_MASK  DCMOTOR_BASE_IEN_WATCH_MASK
#define IEN_ZPOS_MASK   DCMOTOR_BASE_IEN_ZPOS_MASK
#define IEN_ENC_MASK    DCMOTOR_BASE_IEN_ENC_MASK
#define IEN_TBASE_MASK  DCMOTOR_BASE_IEN_TBASE_MASK

/* Interrupt pending bits */
#define IPEND_ENCPOS_MASK DCMOTOR_BASE_IPEND_ENCPOS_MASK
#define IPEND_IENC_MASK   DCMOTOR_BASE_IPEND_IENC_MASK
#define IPEND_IPER_MASK   DCMOTOR_BASE_IPEND_IPER_MASK
#define IPEND_FILR_MASK   DCMOTOR_BASE_IPEND_FILR_MASK
#define IPEND_OUTR_MASK   DCMOTOR_BASE_IPEND_OUTR_MASK
#define IPEND_WATCH_MASK  DCMOTOR_BASE_IPEND_WATCH_MASK
#define IPEND_ZPOS_MASK   DCMOTOR_BASE_IPEND_ZPOS_MASK
#define IPEND_ENC_MASK    DCMOTOR_BASE_IPEND_ENC_MASK
#define IPEND_TBASE_MASK  DCMOTOR_BASE_IPEND_TBASE_MASK

/* Interrupt pending bits */
#define IACK_ENCPOS_MASK DCMOTOR_BASE_IACK_ENCPOS_MASK
#define IACK_IENC_MASK   DCMOTOR_BASE_IACK_IENC_MASK
#define IACK_IPER_MASK   DCMOTOR_BASE_IACK_IPER_MASK
#define IACK_FILR_MASK   DCMOTOR_BASE_IACK_FILR_MASK
#define IACK_OUTR_MASK   DCMOTOR_BASE_IACK_OUTR_MASK
#define IACK_WATCH_MASK  DCMOTOR_BASE_IACK_WATCH_MASK
#define IACK_ZPOS_MASK   DCMOTOR_BASE_IACK_ZPOS_MASK
#define IACK_ENC_MASK    DCMOTOR_BASE_IACK_ENC_MASK
#define IACK_TBASE_MASK  DCMOTOR_BASE_IACK_TBASE_MASK

/* PWM WDOG allow reset defines */
#define WDOG_ALLOW_PWM_RESET   TRUE
#define WDOG_NO_PWM_RESET      FALSE

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
    bool            swap_enc_inputs; /**< Swap encoder A/B inputs            */
    motor_pin_cfg_t pin_cfg[6];      /**< DC0 - DC5 pin configuration        */
} motor_connect_t;

typedef struct mtr6pin_s
{
    MOTOR_REGS * motor_regs;                /* Pointer to the DC Motor block registers */
    uint32_t  motor_irq_num;                  /* Motor block IRQ number                */
    const motor_connect_t * mtr_connects;   /* Motor pin connections                   */
    uint32_t pwm_period;                    /* The pwm period                          */
    uint32_t pwm_divisor;                   /* The pwm divisor                         */
} mtr6pin_t;

/*
 ** Declare accessor functions for motor block DCMOTOR6PIN2
 */

/* Read current value of the encoder position register */
uint32_t mtr6pin_get_encoder_pos( mtr6pin_t * mot );

/* Write position to the encoder write register */
void mtr6pin_set_encoder_pos( mtr6pin_t * mot, uint32_t value );

/* Fetch the maximum value the encoder can hold */
uint32_t mtr6pin_get_encoder_pos_max_value();

/* Return the current pwm period */
uint32_t mtr6pin_get_pwm_period( mtr6pin_t * mot);

/* Initialize control bits in the MCFG register, ASIC specific */
void mtr6pin_set_motorconfig_initialize( mtr6pin_t * mot );

/* Set/Reset the enable bit in the motor config register */
void mtr6pin_set_motorconfig_enable( mtr6pin_t * mot, uint32_t enable );

/* Set the direction pin state */
void mtr6pin_set_dir_state ( mtr6pin_t * mot, motor_dir_t dir );

/* Set the value of the sleep pin */
void mtr6pin_set_sleep_state ( mtr6pin_t * mot, bool sleep );

/* Set the enable pin state */
void mtr6pin_set_en_state ( mtr6pin_t * mot, uint32_t enable );

void mtr6pin_set_encoderconfig_initialize( mtr6pin_t * mot, uint32_t timebase_select );

/* Set/Reset the Soft Reset bit for the motor block */
void mtr6pin_set_soft_reset( mtr6pin_t * mot, uint32_t value );

/* Service the PWM Watchdog */
void mtr6pin_set_pwm_wdog_service(mtr6pin_t * mot, bool allow_pwm_reset);

/* Enable the PWM Watchdog */
void mtr6pin_set_pwm_wdog_enable( mtr6pin_t * mot, uint32_t wdog_ms );

/* Disable the PWM Watchdog */
void mtr6pin_set_pwm_wdog_disable( mtr6pin_t * mot );

/* Get PWM watchdog status register */
uint32_t mtr6pin_get_pwm_wdog_status( mtr6pin_t * mot );

/* Dump the motor registers */
void mtr6pin_reg_dump(mtr6pin_t * mot );

/**
 *  \brief Set encoder input debounce
 *
 *  Function to set the encoder input debounce for a specific move speed
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
 *  \param[in] motor_regs Pointer to the motor registers
 *  \param[in] unscaled_quads_per_second Speed for this move (in the given units)
 *
 **/
void mtr6pin_set_enc_debounce(mtr6pin_t * mot, uint32_t unscaled_quads_per_second);

/**
 *  \brief Set default encoder input debounce
 *
 *  Function to set the default encoder input debounce for a specific 
 *  input pulse width (or reset time).
 *
 *  \param[in] motor_regs Pointer to the motor registers
 *  \param[in] time_usec Time of pulse width to debounce (usec)
 *
 **/
void mtr6pin_set_default_enc_debounce(mtr6pin_t * mot, 
                                      uint32_t time_usec);

/**
 *  \brief Setup the row syncs
 *
 *  If rowsync_type is _not_ SYNC_EVERY, rs_oscale, rs_mscale, rs_pm and
 *  enc_iscale are ignored!
 *
 *  \param[in] motor_regs Pointer to the motor registers
 *  \param[in] rowsync_type Which row syncing should be used to generate start pulses
 *  \param[in] unscaled_rowsync_pos Where the first start pulse should occur, in unscaled quads
 *  \param[in] rs_oscale output period scale -- SYNC_EVERY only
 *  \param[in] rs_mscale multiplier scale -- SYNC_EVERY only
 *  \param[in] rs_pm out period multiplier -- SYNC_EVERY only
 *  \param[in] enc_iscale Encoder period iScale -- SYNC_EVERY only
 **/
void mtr6pin_set_row_sync(mtr6pin_t * mot,
                          MOTOR_SYNC  rowsync_type,
                          uint32_t      unscaled_rowsync_pos,
                          uint8_t       rs_oscale,
                          uint8_t       rs_mscale,
                          uint16_t      rs_pm,
                          uint8_t       enc_iscale);
/**
 *  \brief (API) Motor block soft reset
 *
 *  Does a soft reset on the specified motor block:
 *   - row syncs are disabled
 *   - row sync start position set to hardware default
 *   - encoder value is preserved
 *
 *  \param[in] motor_regs Pointer to the motor registers
 **/
void mtr6pin_dc_soft_reset(mtr6pin_t * mot);

/**
 *  \brief Apply Servo PWM to motor
 *
 *  Used to apply servo control to the motor. This routine may leave a holding
 *  current on the motor when 0 PWM is commanded, so don't use this routine to
 *  stop the motor after a move (see mtr6pin_no_pwm()).  0 PWM may be called for 
 *  under normal servo conditions,  but this is not the 'stopped' use case.
 *
 *  \notes
 *  This routine requires a duty value between 0 and the maximum PWM period. 
 *
 *  \param[in]  motor_regs Pointer to the motor registers
 *  \param[in]  reg_duty_val Duty cycle value
 *  \param[in]  pwm_period The pwm period for this motor
 *  \param[in]  pwm_method Specifies if this motor is a phase or normal motor
 *  \param[in]  dir MOT_FORWARD or MOT_REVERSE
 **/
void mtr6pin_pwm_control( mtr6pin_t * mot, uint32_t reg_duty_val, motor_dir_t dir );

/**
 *  \brief Apply 0 PWM to motor
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
 *  \param[in]  motor_regs Pointer to the motor registers
 *  \param[in]  pwm_period The pwm period for this motor
 *  \param[in]  pwm_method Specifies if this motor is a phase or normal motor
 *  \param[in]  mtr_connects The motor connection table
 **/
void mtr6pin_no_pwm( mtr6pin_t * mot );

/**
 *  \brief (Helper) Enable motor interrupts
 *
 *  Turn on the motor block IRQ: Note that all motor IRQ's will be acked
 *  first to prevent spurious interrupts.
 *
 *  \param[in]  mot The motor
 **/
void mtr6pin_int_enable( mtr6pin_t * mot );


/**
 *  \brief (Helper) Disable all motor interrupts
 *
 *  Will mask out all the individual motor interrupts as well as disable the
 *  motor block IRQ.
 *
 *  \param[in]  mot The motor
 **/
void mtr6pin_int_disable( mtr6pin_t * mot );

/*
 * \brief Initialize a motor
 *
 * This method will do all necessary initializations to get a motor ready
 *
 *  \param[in]  mot The motor to initialize
 *  \param[in]  mtr_connects The motor connections
 *  \param[in]  interrupt_handler Interrupt service routine
 *  \param[in]  handler_arg An argument to be passed to the isr
 */
scan_err_t mtr6pin_init(mtr6pin_t * mot, const motor_connect_t * mtr_connects,
                        PFN_ISR interrupt_handler, uint32_t handler_arg );

#endif


