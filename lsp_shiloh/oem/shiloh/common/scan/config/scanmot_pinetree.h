/*
 * ============================================================================
 * Copyright (c) 2004-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanmot_pinetree.h
 *
 * \brief Header file for scan stepper motor driver
 *
 *  Data types and API functions for the stepper motor driver.
 *
 *      +-----------------------------------+
 *      +   Device Independent Scan Code    +
 *      +-----------------------------------+
 *      +   Device Specific Mechanism Code  +
 *      +-----------------------------------+
 *      +   stepper Motor Code              +
 *      +-----------------------------------+
 *
 *  The stepper motor code is at the bottom of the architectural heap, so we try to
 *  limit the motor codes view to just the motor, encoder, and ASIC. For example,
 *  motor code has no idea how the motor is connected to the scan mechanism (gear
 *  ratios, single motor used for dual purposes, etc). The mechanism code should
 *  encapsulate all the SSA details ... the motor code simply does as it is commanded.
 *  In the event of a failure, the OCC mechanism can be used to send signals up the
 *  architectural tree in real time.
 */

#ifndef INC_SCANMOT_PINETREE_H
#define INC_SCANMOT_PINETREE_H

#define STMOTOR_REGS struct STMOTOR_REG_REGS_s

#define INVALID_STEP_MOTOR_ID        -1
#define MOVE_FORWARD                  1
#define MOVE_REVERSE                  0

#define SMC_OUTPUT_PWM1               0
#define SMC_OUTPUT_PWM2               1
#define SMC_OUTPUT_REG0               2
#define SMC_OUTPUT_REG1               3
#define SMC_OUTPUT_SEQ                4
#define SMC_OUTPUT_LS                 5


//stepper motor ID
typedef int stmotor_id_t;


//stepper motor block select
typedef enum {
    STEP_MTR_BLK_0 = 0,              //ASIC stepper motor block 0
    STEP_MTR_BLK_1,                  //ASIC stepper motor block 1
    STEP_MTR_BLK_2,                  //ASIC stepper motor block 2
    STEP_MTR_BLK_3,                  //ASIC stepper motor block 3
    STEP_MTR_BLK_4,                  //ASIC stepper motor block 4
    STEP_MTR_BLK_5,                  //ASIC stepper motor block 5
} stmotor_block_t;


//stepper motor pin output mode selection
typedef enum {
    STEP_OUTPUT_MODE_PWM1 = 0,
    STEP_OUTPUT_MODE_PWM2,
    STEP_OUTPUT_MODE_REG0,
    STEP_OUTPUT_MODE_REG1,
    STEP_OUTPUT_MODE_SEQ,
    STEP_OUTPUT_MODE_LS
} stmotor_pin_signal_t;


//stepper motor pin output mode configuration
typedef struct stmotor_output_mode_s
{
    stmotor_pin_signal_t signal;
} stmotor_output_mode_t;


//stepper motor pin configuration
typedef struct stmotor_connect_s
{
    stmotor_block_t        block_num;           //ASIC stepper motor block number
    stmotor_output_mode_t  smc_output_mode[6];  //SMC0~SMC5 pin configuration
} stmotor_connect_t;


//stepper motor state
typedef enum
{
    STMOTOR_STOP = 0,
    STMOTOR_PREHOLD,
    STMOTOR_RAMPING_UP,
    STMOTOR_AT_SPEED,
    STMOTOR_RAMPING_DOWN,
    STMOTOR_POSTHOLD,
    STMOTOR_INVALID,
} STMotorState_t;


//stepper motor index of dpi proglie
enum DPI_PROFILE
{
    DPI150C  = 0,
    DPI150M  = 1,  
    DPI300C  = 2,
    DPI300M  = 3,
    DPI600C  = 4,
    DPI600M  = 5,
    DPI1200C = 6,
    DPI1200M = 7
};


typedef enum DPI_PROFILE dpi_index;


//stepper motor dpi profile
typedef struct {
    uint32_t  pwm_p;              //pwm_p, 24 bits
    uint32_t  pwm_m_start;        //pwm_m, 24 bits
    uint32_t  pwm_m_target;       //pwm_m, 24 bits
    uint16_t  preholding_steps;
    uint16_t  postholding_steps;
    uint8_t   acc_incr;           //acc_incr, 8 bits, set to 0 if using firmware profile
    uint16_t *tvt_table;          //torque vector table, 16 bits, useless if using sequence control
    uint32_t *accel_table;
}dpi_profile_rec;


//stepper motor cmd packet
typedef struct {
    uint32_t  pwm_m;
    uint32_t  pwm_p;
    uint8_t   acc_incr;
    uint16_t  st_tgt;
    uint8_t   inf;
    uint8_t   ls_en;
    uint8_t   dir;
    uint8_t   acc_dir;
    uint8_t   cmd;
    uint8_t   issue;
}stepper_cmd_packet_t;


//stepper motor move type
typedef enum {
    MOTOR_MOVE_POSITION,
    MOTOR_MOVE_CONTINUOUS
} STMotorMoveType_t;


/**
 *  \brief (API) Initialize Stepper motor code
 *
 *  API function to initialize the Stepper motor code, must be called once at system
 *  startup before any other motor routines can be used.
 *
 *  \param[in] num_motors  Number of motors to support
 *
 **/
bool smot_step_init(uint8_t num_motors);


/**
 *  \brief (API) Set up a new motor
 *
 *  API function to create a new motor instance, usually called by the mech code
 *  during system startup.  This routine must only be called once for each
 *  physical motor to be supported by the motor driver.
 *
 **/
stmotor_id_t smot_step_create_motor(dpi_profile_rec*           all_dpi_profile,
                                    const stmotor_connect_t   *stmotor_connects,
                                    uint32_t*                  stmotor_sequence,
                                    uint32_t                   ramp_table_length,
                                    uint32_t                   stmotor_max_command_steps,
                                    bool                       (*sensor_check_func)(void));

/**
 *  \brief (API) Move the stepper motor relative to the current position
 *
 **/
void smot_step_move_rel(stmotor_id_t    motor_id,
                        uint32_t        motor_dpi,
                        uint32_t        move_steps,
                        uint8_t         move_direction,
                        bool            enable_sp,
                        uint32_t        enable_sp_at_stepnum);


/**
 *  \brief (API) Move the stepper motor to an absolute position
 *
 **/
void smot_step_move_abs(stmotor_id_t  motor_id,
                        uint32_t      motor_dpi,
                        int           tar_pos_steps,
                        bool          enable_sp, 
                        uint32_t      enable_sp_at_stepnum);


/**
 *  \brief (API) Initiate 'gentle' motor stop
 *
 **/
void smot_step_request_motor_stop(stmotor_id_t  motor_id);


/**
 *  \brief (API) Send an emergency halt command to a stepper motor
 *
 **/
void smot_step_emergency_halt(stmotor_id_t  motor_id);


/**
 *  \brief (API) Check for motor idle (stopped)
 *
 *  Motor code clients should use this routine to wait for the motor to
 *  stop after requesting a 'gentle' stop (the client will have to poll
 *  this function in some sort of thread friendly delay).
 *
 *  \warning
 *  This routine cannot currently detect the motor state after a 'hard' stop,
 *  in that case the motor may still be moving after the stop state has been
 *  set.
 *
 *  \param[in] stmotor_id    Client Motor ID
 *
 *  \return BOOL
 *  \retval TRUE     Motor is in the idle (stopped) state
 *  \retval FALSE    Motor is not idle
 **/
bool smot_step_motor_is_idle(stmotor_id_t motor_id);


//MRVL-Harris-20120329 for step loss {
/**
 *  \brief Motor set to idle or leave from idle
 *
 *  \param[in] stmotor_id    Client Motor ID
 *  \param[in] state         TRUE  means copy seq to idle
 *                           FALSE means write seq to idle
 *  \parma[in] seq           for state is false
 **/
void smot_step_set_idle(stmotor_id_t motor_id, bool state, uint32_t seq);
//MRVL-Harris-20120329 for step }


/**
 * \brief TBD
 *  
 *  
 *
 **/
void smot_step_block_enable(stmotor_id_t motor_id, bool value);


/**
 * \brief Set Stepper Motor location
 *  
 **/
void smot_step_set_location(stmotor_id_t motor_id, int location);


/**
 * \brief Get Stepper Motor location
 *  
 **/
int smot_step_get_location(stmotor_id_t motor_id);


/**
 * \brief Set Stepper Motor line start
 *  
 **/
void smot_step_set_line_start(stmotor_id_t motor_id,
                              uint8_t      ls_src,
                              uint16_t     ls_incr);

#endif //INC_SCANMOT_PINETREE_H
