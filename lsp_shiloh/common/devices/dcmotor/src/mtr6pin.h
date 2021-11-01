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
 * \brief declarations and macros to support interface to motor 
 *        block DCMOTOR6PIN
 *
 **/

#ifndef MTR6PIN_H
#define MTR6PIN_H




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


/*
 ** Declare accessor functions for motor block DCMOTOR6PIN2
 */

/* Return maximum valid period value */
UINT32 get_mtr6pin_pwm_period_max( void );

/* Replace PWM period field in PPER0 register with a new value. */ 
void set_mtr6pin_pwm_period( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Return maximum valid period value */
UINT32 get_mtr6pin_pwm_duty_max( void );

/* Replace PWM duty field in PDTY0 register with a new value. */ 
void set_mtr6pin_pwm_duty( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Read current value of the encoder position register */
UINT32 get_mtr6pin_encoder_pos( volatile MOTOR_REGS * motor_regs );

/* Read current value of the encoder period register */
UINT32 get_mtr6pin_encoder_period( volatile MOTOR_REGS * motor_regs, motor_enc_t enc_inputs,uint32_t * rising );

/* Write position to the encoder write register */
void set_mtr6pin_encoder_pos( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Fetch the maximum value the encoder can hold */
UINT32 get_mtr6pin_encoder_pos_max_value();

/* Set new value in the PWM delay register */
void set_mtr6pin_pwm_delay( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Set new value in the PWM divisor register */
void set_mtr6pin_pwm_divisor( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Initialize control bits in the MCFG register, ASIC specific */
void set_mtr6pin_motorconfig_initialize( volatile MOTOR_REGS   *motor_regs, 
                                         const motor_connect_t *connect_table );

/* Set/Reset the enable bit in the motor config register */
void set_mtr6pin_motorconfig_enable( volatile MOTOR_REGS * motor_regs, UINT32 enable );

/* Set the direction pin state */
void set_mtr6pin_dir_state ( volatile MOTOR_REGS   *motor_regs, 
                             const motor_connect_t *connect_table,
                             motor_dir_t            dir );

/* Set the enable pin state */
void set_mtr6pin_en_state ( volatile MOTOR_REGS   *motor_regs, 
                            const motor_connect_t *connect_table,
                            UINT32                 enable );

/* Set the sleep pin state */
void set_mtr6pin_slp_state ( volatile MOTOR_REGS   *motor_regs, 
                            const motor_connect_t *connect_table,
                            UINT32                 sleep );

/* Set the mode pin state */
void set_mtr6pin_mode_state ( volatile MOTOR_REGS   *motor_regs, 
                            const motor_connect_t *connect_table,
                            UINT32                 mode );

void set_mtr6pin_encoderconfig_initialize( volatile MOTOR_REGS   *motor_regs,
                                           const motor_connect_t *connect_table,
                                           UINT32                 timebase_select );

void set_mtr6pin_encoderconfig_iscale( volatile MOTOR_REGS * motor_regs, UINT32 value );

void set_mtr6pin_encoderconfig_enable( volatile MOTOR_REGS * motor_regs, UINT32 value );

void set_mtr6pin_encoderconfig_rsyncsel( volatile MOTOR_REGS * motor_regs, UINT32 value );

void set_mtr6pin_encoderconfig_rmode( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Set rowsync start position */
void set_mtr6pin_rstart( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Set rowsync ignore bit */
void set_mtr6pin_rstop_ignore( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Set rstop position */
void set_mtr6pin_rstop( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Set debounce register zdb field */
void set_mtr6pin_debounce_zdb( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Set debounce register edb field */
void set_mtr6pin_debounce_edb( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Get debounce register edb field */
UINT32 get_mtr6pin_debounce_edb( volatile MOTOR_REGS * motor_regs );

/* set oscale field in rowsync PM register */
void set_mtr6pin_pm_oscale( volatile MOTOR_REGS * motor_regs, UINT32 oscale );

/* set mscale field in rowsync PM register */
void set_mtr6pin_pm_mscale( volatile MOTOR_REGS * motor_regs, UINT32 mscale );

/* set mult field in rowsync PM register */
void set_mtr6pin_pm_mult( volatile MOTOR_REGS * motor_regs, UINT32 mult );

/* Set/Reset the Soft Reset bit for the motor block */
void set_mtr6pin_soft_reset( volatile MOTOR_REGS * motor_regs, UINT32 value );

/* Service the PWM Watchdog */
void mtr6pin_pwm_wdog_service(volatile MOTOR_REGS * motor_regs, BOOL allow_pwm_reset);

/* Enable the PWM Watchdog */
void mtr6pin_pwm_wdog_enable( volatile MOTOR_REGS * motor_regs, UINT32 wdog_ms );

/* Disable the PWM Watchdog */
void mtr6pin_pwm_wdog_disable( volatile MOTOR_REGS * motor_regs );

/* Get PWM watchdog status register */
UINT32 get_mtr6pin_pwm_wdog_status( volatile MOTOR_REGS * motor_regs );

/* Dump the motor registers */
void mtr6pin_reg_dump(volatile MOTOR_REGS *motor_regs);

#endif
