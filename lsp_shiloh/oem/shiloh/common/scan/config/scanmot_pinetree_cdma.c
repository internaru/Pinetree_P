/*
 * ============================================================================
 * Copyright (c) 2006-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanmot_pinetree.c
 *
 * \brief Stepper Servo Scan Motor Driver
 *
 */

#ifndef __KERNEL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <posix_ostools.h>
#include "scos.h"
#include "ATypes.h"
#include "memAPI.h"
#include "interrupt_api.h"
#include "regAddrs.h"
#include "intnums.h"
#include "lassert.h"
#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanalyzer.h"
#include "utils.h"
#include "hwconfig_api.h"
#include "scandbg.h"
#include "scanhw.h"
#include "scan.h"
#include "scanif.h"
#include "cpu_api.h"
#include "debug.h"
#include "ostools.h"
#include "STMotor_regstructs.h"
#include "STMotor_regmasks.h"
#include "mtr6pin.h"
#include "scanplat.h"
#include "scanmot_pinetree.h"
#include "cdma_api.h" //MRVL-Harris for stepper cdma
#else // __KERNEL__
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/kfifo.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/kernel.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/scatterlist.h>
#include <linux/mv61_cdma.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach/irq.h>
#include <mach/irqs.h>
#include "scanmot_pinetree.h"
#include "asic/STMotor_regstructs.h"
#include "asic/STMotor_regmasks.h"
#include "utils.h"
#include "scancore.h"
#include "scantypes.h"
#include "scanplat_sb.h"
#include "scandbg.h"

#include "scanif.h"


// Turn off for builds in full Scan driver
//#define SYSFS_CMD_DEBUG

#ifdef SYSFS_CMD_DEBUG
#include "scanmech_step_config.h"

#define DRIVER_NAME "gogin-stepper"

#define STEPPER_NUM_MOTORS  3
static stmotor_id_t   motors[STEPPER_NUM_MOTORS] = {INVALID_STEP_MOTOR_ID,INVALID_STEP_MOTOR_ID};
static stmotor_id_t   test_motor;
#endif

#endif // __KERNEL__

/* SC 219 Motor Backward /  HomeSensor ON/ Home Sensor OFF marking*/
#if 0
#define TIMESTAMP
#else
#define TIMESTAMP { struct timeval my_time; \
    do_gettimeofday(&my_time); \
    printk("K %5ld:%6ld ", my_time.tv_sec, my_time.tv_usec);}
#endif

//#ifdef DEBUG
//#define dbg1(args...) printk(args)
////#define dbg1(args...)
//#define dbg2(args...) printk(args)
////#define dbg2(args...)
//#else
//#error foo
//#define dbg1(args...)
//#define dbg2(args...)
//#endif

#define STMOTOR_SMC_CFG_OFFSET                                   0x0000
    #define STMOTOR_SMC_CFG_CDMA_MODE(x)                         (((x) & 0x0001) <<  1)
    #define STMOTOR_SMC_CFG_STEP_EN(x)                           (((x) & 0x0001) <<  0)

#define STMOTOR_SMC_CTRL_OFFSET                                  0x0004
    #define STMOTOR_SMC_CTRL_SRESET(x)                           (((x) & 0x0001) <<  8)
    #define STMOTOR_SMC_CTRL_STCNT_RST(x)                        (((x) & 0x0001) <<  4)
    #define STMOTOR_SMC_CTRL_CMD_ABRT(x)                         (((x) & 0x0001) <<  0)

#define STMOTOR_LS_INCR_OFFSET                                   0x0008
    #define STMOTOR_LS_INCR_LS_SRC(x)                            (((x) & 0x0001) << 20)
    #define STMOTOR_LS_INCR_LS_INCR(x)                           (((x) & 0xFFFF) <<  0)

#define STMOTOR_REG_01_OFFSET                                    0x000C

#define STMOTOR_NUM_USTEPS_OFFSET                                0x0010

#define STMOTOR_STEP_EN_IDLE_OFFSET                              0x0014
    #define STMOTOR_STEP_EN_IDLE_SMC_5(x)                        (((x) & 0x0001) <<  5)
    #define STMOTOR_STEP_EN_IDLE_SMC_4(x)                        (((x) & 0x0001) <<  4)
    #define STMOTOR_STEP_EN_IDLE_SMC_3(x)                        (((x) & 0x0001) <<  3)
    #define STMOTOR_STEP_EN_IDLE_SMC_2(x)                        (((x) & 0x0001) <<  2)
    #define STMOTOR_STEP_EN_IDLE_SMC_1(x)                        (((x) & 0x0001) <<  1)
    #define STMOTOR_STEP_EN_IDLE_SMC_0(x)                        (((x) & 0x0001) <<  0)

#define STMOTOR_TVT_01_OFFSET                                    0x0018
#define STMOTOR_TVT_23_OFFSET                                    0x001C
#define STMOTOR_TVT_45_OFFSET                                    0x0020
#define STMOTOR_TVT_67_OFFSET                                    0x0024
#define STMOTOR_TVT_8_OFFSET                                     0x0028

#define STMOTOR_SEQ_0_OFFSET                                     0x002C
#define STMOTOR_SEQ_1_OFFSET                                     0x0030
#define STMOTOR_SEQ_2_OFFSET                                     0x0034
#define STMOTOR_SEQ_3_OFFSET                                     0x0038
#define STMOTOR_SEQ_4_OFFSET                                     0x003C
#define STMOTOR_SEQ_5_OFFSET                                     0x0040

#define STMOTOR_OUT_MODE_OFFSET                                  0x0044
    #define STMOTOR_OUT_MODE_O_MODE5(x)                          (((x) & 0x0007) << 20)
    #define STMOTOR_OUT_MODE_O_MODE4(x)                          (((x) & 0x0007) << 16)
    #define STMOTOR_OUT_MODE_O_MODE3(x)                          (((x) & 0x0007) << 12)
    #define STMOTOR_OUT_MODE_O_MODE2(x)                          (((x) & 0x0007) <<  8)
    #define STMOTOR_OUT_MODE_O_MODE1(x)                          (((x) & 0x0007) <<  4)
    #define STMOTOR_OUT_MODE_O_MODE0(x)                          (((x) & 0x0007) <<  0)

#define STMOTOR_STEP_INT_OFFSET                                  0x0048

#define STMOTOR_PWM_T_OFFSET                                     0x004C

#define STMOTOR_PWM_P_OFFSET                                     0x0050

#define STMOTOR_ACC_INCR_OFFSET                                  0x0054

#define STMOTOR_PWM_M_OFFSET                                     0x0058

#define STMOTOR_CMD_OFFSET                                       0x005C
    #define STMOTOR_CMD_ST_TGT(x)                                (((x) & 0xFFFF) << 16)
    #define STMOTOR_CMD_INF(x)                                   (((x) & 0x0001) << 15)
    #define STMOTOR_CMD_LS_EN(x)                                 (((x) & 0x0001) << 12)
    #define STMOTOR_CMD_DIR(x)                                   (((x) & 0x0001) <<  5)
    #define STMOTOR_CMD_ACC_DIR(x)                               (((x) & 0x0001) <<  4)
    #define STMOTOR_CMD_CMD(x)                                   (((x) & 0x0007) <<  0)


#define STMOTOR_CMD_ISSUE_OFFSET                                 0x0060

#define STMOTOR_COM_STAT_OFFSET                                  0x0064

#define STMOTOR_COM_PWM_P_OFFSET                                 0x0068

#define STMOTOR_COM_ACC_INCR_OFFSET                              0x006C

#define STMOTOR_COM_PWM_M_OFFSET                                 0x0070

#define STMOTOR_COM_CMD_OFFSET                                   0x0074

#define STMOTOR_COM_STP_OFFSET                                   0x0078

#define STMOTOR_COM_TVT_OFFSET                                   0x007C

#define STMOTOR_COM_SEQ_OFFSET                                   0x0080

#define STMOTOR_I_EN_OFFSET                                      0x0084

#define STMOTOR_I_PEND_OFFSET                                    0x0088

#define STMOTOR_I_ACK_OFFSET                                     0x008C

#define STMOTOR_I_FORCE_OFFSET                                   0x0090

#define STMOTOR_SMC_TRIG                                         0x0094
    #define STMOTOR_SMC_TRIG_SMC5(x)                             (((x) & 0x0007) << 20)
    #define STMOTOR_SMC_TRIG_SMC4(x)                             (((x) & 0x0007) << 16)
    #define STMOTOR_SMC_TRIG_SMC3(x)                             (((x) & 0x0007) << 12)
    #define STMOTOR_SMC_TRIG_SMC2(x)                             (((x) & 0x0007) <<  8)
    #define STMOTOR_SMC_TRIG_SMC1(x)                             (((x) & 0x0007) <<  4)
    #define STMOTOR_SMC_TRIG_SMC0(x)                             (((x) & 0x0007) <<  0)


#define STMOTOR_CMD_HOLD                                         0
#define STMOTOR_CMD_STEP                                         1
#define STMOTOR_CMD_ACCEL_PWM_M                                  2
#define STMOTOR_CMD_HWLS                                         3
#define STMOTOR_CMD_ACCEL_PWM_P                                  4

//#define Phase_12

#ifdef Phase_12
struct pt_mech {
     unsigned int scan_home_sensor_gpio;
     unsigned int scan_nmotor_driver_stby;
     unsigned int scan_nmotor_driver_rst;

     unsigned int phase_mode_select1;
     unsigned int phase_mode_select2;
     unsigned int current_control;
     
	 unsigned int a3_clutch;
	// unsigned int FB_enable;
};

extern struct pt_mech pinetree_mech;
#endif

typedef struct stmotor_params_s
{
    volatile STMOTOR_REG_REGS_t   *stmotor_regs;               //stepper motor control register
    uint32_t                       stmotor_irq_num;            //IRQ number
    const stmotor_connect_t       *stmotor_connects;           //stepper motor connection
    dpi_profile_rec               *all_dpi_profile;            //all supportted dpi profile
    dpi_profile_rec               *cur_dpi_profile;            //current dpi profile
    const uint32_t                *stmotor_sequence;           //stepper motor output sequence
    STMotorState_t                 stmotor_cur_state;          //current motor state
    STMotorState_t                 stmotor_pre_state;          //previous motor state
    uint32_t                       target_steps;               //target steps
    uint32_t                       pending_steps;              //pending steps
    uint32_t                       max_command_steps;          //max command steps //MRVL-Harris-20111223
    uint8_t                        direction;                  //direction
    uint32_t                       ramp_step_limit;            //provide a step limit for ramp-up and ramp-down
    uint32_t                       ramp_up_steps;              //before ramp_up_steps, motor should be ramp-up
    uint32_t                       ramp_down_steps;            //after ramp_down_steps, motor should be ramp-down
    stepper_cmd_packet_t           cmd_packet;                 //command packet
    bool                           ls_en;  
    int                            ls_incr;             
    uint32_t                       ls_start_pos;
    uint16_t                       preholding_steps;
    uint16_t                       postholding_steps;
    STMotorMoveType_t              move_type;                  //????
    int                            location;
    int                            location_access;
    bool                           stop_requested;
    //MRVL-Harris for stepper cdma start
    bool                           stmotor_cdma_enable;
    uint32_t                       stmotor_cdma_num;
#ifndef __KERNEL__
    cdma_handle_t                 *stmotor_cdma_handle;
    cdma_chain_cmds_t             *stmotor_cdma_chain;
#else
    struct dma_chan               *stmotor_dma_chan;
    struct sg_table               *stmotor_sg_table;
    struct mv61_dma_slave          tx_dma_slave;
    struct semaphore               sg_sem;
    uint8_t                       *cmd_queue;
#endif
    uint32_t                       cur_stepcnt;
    uint32_t                       pre_stepcnt;
    //MRVL-Harris for stepper cdma end
    bool                           (*sensor_check_fcn)(void);  //function pointer to a function that checks the sensor for a wall hit (FB) or PIP (ADF)
} stmotor_params_t;

static stmotor_params_t *stmotors    =  NULL;

static int8_t  st_max_motors         = -1;
static int8_t  stmotors_created      =  0;

bool  reduce_control  = 0;

extern bool SC_halt_flg;  //don't move the motor after SC hale.  mod.kks 2014.06.02

#ifdef Phase_12
//#define	FB_DECEL_CONTROL_STEP   94 //  2mm
	#define	FB_DECEL_CONTROL_STEP   142// 3mm
#else
	#define	FB_DECEL_CONTROL_STEP   377// 4mm
#endif

#define	ADF_DECEL_CONTROL_STEP   944// 10mm

#ifndef __KERNEL__
TX_SEMAPHORE                    stmotor_cdma_semaphore; //MRVL-Harris for stepper cdma
#endif

static char *motor_irq_name[6] = {"stmotor0","stmotor1","stmotor2","stmotor3","stmotor4","stmotor5"};

#define step_func_enter()              
#define step_func_leave()              dbg2("STEP@%s: leave\n", __FUNCTION__)
#define step_func_infor(msg, args...) 
#define step_func_error(msg, args...)  dbg2("STEP@%s.%d: " msg, __FUNCTION__, __LINE__, ##args)
#define step_func_debug(msg, args...)  
#define step_func_temp1(msg, args...)  

//#define step_func_enter()              dbg2("STEP@%s: enter\n", __FUNCTION__)
//#define step_func_leave()              dbg2("STEP@%s: leave\n", __FUNCTION__)
//#define step_func_leave()
#define step_func_infor(msg, args...)  //dbg1("STEP@%s: " msg, __FUNCTION__, ##args)
#define step_func_error(msg, args...)   //dbg1("STEP@%s.%d: " msg, __FUNCTION__, __LINE__, ##args)
#define step_func_debug(msg, args...)  //dbg2("STEP@%s.%d: " msg, __FUNCTION__, __LINE__, ##args)
#define step_func_temp1(msg, args...)  //dbg1(msg, ##args)

static bool smot_step_debug_in_interrupt_enable = false; //only enable this when you want to see some debug message in interrupt
//static bool smot_step_debug_in_interrupt_enable =true; //kks test

/* davep 31-Aug-2012 ; copy/paste my linux-kernel lassert.h */
#define ASSERT(cond)  BUG_ON(!(cond))
//#define XASSERT(cond,value) BUG_ON(!(cond))

#define XASSERT(cond,value) \
    do { \
        if( !(cond) ) { \
            printk( KERN_CRIT "XASSERT failed at %s %d value=0x%x\n", \
                    __FILE__, __LINE__, value );\
            panic("BUG!"); \
        } \
    } while(0); 

#ifdef HAVE_DBG_CALLBACK_CMDS
#include "scos_cmd_proc.h"
static int smot_step_debug_callback(int argc, char *argv[]);
#endif


static void smot_step_update_location(stmotor_id_t motor_id);

static void smot_step_dump(stmotor_id_t motor_id);

static bool smot_step_motor_id_check(stmotor_id_t motor_id)
{
    /* WARNING: this routine can be called from interrupt context.
     *          the debug message will be followed by an assert.
     */

    if (st_max_motors == -1)
    {
        ASSERT(0);
        step_func_error("st_max_motors == -1, please call smot_step_init first\n");
        ASSERT(0);
        return false;
    }

    if (stmotors_created == 0)
    {
        ASSERT(0);
        step_func_error("stmotors_created == 0, please call smot_step_create_motor first\n");
        ASSERT(0);
        return false;
    }

    if (motor_id >= stmotors_created)
    {
        ASSERT(0);
        step_func_error("motor_id >= stmotors_created, motor_id = %d, stmotors_created = %d\n", motor_id, stmotors_created);
        ASSERT(0);
        return false;
    }

    if (stmotors == NULL)
    {
        ASSERT(0);
        step_func_error("stmotors == NULL\n");
        ASSERT(0);
        return false;
    }

    if (stmotors[motor_id].stmotor_regs == NULL)
    {
        ASSERT(0);
        step_func_error("stmotors[%d].stmotor_regs == NULL\n", motor_id);
        ASSERT(0);
        return false;
    }

    return true;
}


static void smot_step_register_sensor_check_fcn(stmotor_id_t motor_id, bool (*sensor_check_func)(void))
{
    if (!smot_step_motor_id_check(motor_id))
    {
        step_func_error("invalid motor id [%d]\n", motor_id);
        ASSERT(0);
        return;
    }
    stmotors[motor_id].sensor_check_fcn = sensor_check_func;
}


static void smot_step_set_torque_vector_table(stmotor_id_t motor_id)
{
    uint16_t tvt[9];
    uint16_t i;

    for (i=0; i<9; i++)
    {
        tvt[i] = stmotors[motor_id].cur_dpi_profile->tvt_table[i];
    }

    stmotors[motor_id].stmotor_regs->TVT_01 = STMOTOR_REG_TVT_01_TVT_0_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_01, tvt[0]);
    stmotors[motor_id].stmotor_regs->TVT_01 = STMOTOR_REG_TVT_01_TVT_1_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_01, tvt[1]);
    stmotors[motor_id].stmotor_regs->TVT_23 = STMOTOR_REG_TVT_23_TVT_2_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_23, tvt[2]);
    stmotors[motor_id].stmotor_regs->TVT_23 = STMOTOR_REG_TVT_23_TVT_3_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_23, tvt[3]);
    stmotors[motor_id].stmotor_regs->TVT_45 = STMOTOR_REG_TVT_45_TVT_4_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_45, tvt[4]);
    stmotors[motor_id].stmotor_regs->TVT_45 = STMOTOR_REG_TVT_45_TVT_5_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_45, tvt[5]);
    stmotors[motor_id].stmotor_regs->TVT_67 = STMOTOR_REG_TVT_67_TVT_6_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_67, tvt[6]);
    stmotors[motor_id].stmotor_regs->TVT_67 = STMOTOR_REG_TVT_67_TVT_7_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_67, tvt[7]);

    stmotors[motor_id].stmotor_regs->TVT_8  = STMOTOR_REG_TVT_8_TVT_8_REPLACE_VAL(stmotors[motor_id].stmotor_regs->TVT_8, tvt[8]);
}


static void smot_step_set_sequence(stmotor_id_t motor_id)
{
    uint32_t seq[6];
    uint16_t i;

    for (i=0; i<6; i++)
    {
        seq[i] = stmotors[motor_id].stmotor_sequence[i];
    }


    stmotors[motor_id].stmotor_regs->SEQ_0 = seq[0];
    stmotors[motor_id].stmotor_regs->SEQ_1 = seq[1];
    stmotors[motor_id].stmotor_regs->SEQ_2 = seq[2];
    stmotors[motor_id].stmotor_regs->SEQ_3 = seq[3];
    stmotors[motor_id].stmotor_regs->SEQ_4 = seq[4];
    stmotors[motor_id].stmotor_regs->SEQ_5 = seq[5];
}


void smot_step_block_enable(stmotor_id_t motor_id, bool value)
{
    step_func_enter();
    stmotors[motor_id].stmotor_regs->SMC_CFG = STMOTOR_REG_SMC_CFG_STEP_EN_REPLACE_VAL(stmotors[motor_id].stmotor_regs->SMC_CFG, value);
}


//MRVL-Harris for stepper cdma start
static void smot_step_cdma_mode_enable(stmotor_id_t motor_id, bool value)
{
    step_func_enter();
    stmotors[motor_id].stmotor_regs->SMC_CFG = STMOTOR_REG_SMC_CFG_CDMA_MODE_REPLACE_VAL(stmotors[motor_id].stmotor_regs->SMC_CFG, value);
}
//MRVL-Harris for stepper cdma end


static void smot_step_soft_reset(stmotor_id_t motor_id, bool value)
{
    step_func_enter();
    stmotors[motor_id].stmotor_regs->SMC_CTRL = STMOTOR_REG_SMC_CTRL_SRESET_REPLACE_VAL(stmotors[motor_id].stmotor_regs->SMC_CTRL, value);
}


static void smot_step_count_reset(stmotor_id_t motor_id, bool value)
{
    step_func_enter();
    stmotors[motor_id].stmotor_regs->SMC_CTRL = STMOTOR_REG_SMC_CTRL_STCNT_RST_REPLACE_VAL(stmotors[motor_id].stmotor_regs->SMC_CTRL, value);
    //MRVL-Harris for stepper cdma start
    if (value)
    {
        stmotors[motor_id].cur_stepcnt = 0;
        stmotors[motor_id].pre_stepcnt = 0;
    }
    //MRVL-Harris for stepper cdma end
}


static void smot_step_command_abort(stmotor_id_t motor_id, bool value)
{
    step_func_enter();
    stmotors[motor_id].stmotor_regs->SMC_CTRL = STMOTOR_REG_SMC_CTRL_CMD_ABRT_REPLACE_VAL(stmotors[motor_id].stmotor_regs->SMC_CTRL, value);
}


static uint32_t smot_step_get_out_mode(const stmotor_connect_t *stmotor_connects, uint32_t pin_num)
{
    uint32_t ret_val = 0;

    switch (stmotor_connects->smc_output_mode[pin_num].signal)
    {
        case STEP_OUTPUT_MODE_PWM1:
            ret_val = SMC_OUTPUT_PWM1;
            break;
        case STEP_OUTPUT_MODE_PWM2:
            ret_val = SMC_OUTPUT_PWM2;
            break;
        case STEP_OUTPUT_MODE_REG0:
            ret_val = SMC_OUTPUT_REG0;
            break;
        case STEP_OUTPUT_MODE_REG1:
            ret_val = SMC_OUTPUT_REG1;
            break;
        case STEP_OUTPUT_MODE_SEQ:
            ret_val = SMC_OUTPUT_SEQ;
            break;
        case STEP_OUTPUT_MODE_LS:
            ret_val = SMC_OUTPUT_LS;
            break;
        default:
            step_func_error("%d is unknown pin signal\n", stmotor_connects->smc_output_mode[pin_num].signal);
            XASSERT(0, stmotor_connects->smc_output_mode[pin_num].signal);
            break;
    }

    return ret_val;
}


//MRVL-Harris for stepper cdma start
static uint32_t smot_step_get_step_count(stmotor_id_t motor_id)
{
    uint32_t stepcnt = 0;

    stepcnt = stmotors[motor_id].stmotor_regs->COM_STP;
    stepcnt = stepcnt & 0xFFFFFF;
    if (stmotors[motor_id].direction == MOVE_REVERSE)
    {
        if (stepcnt >= 0x800000)
        {
            stepcnt = 0x1000000 - stepcnt;
        }
    }
    stmotors[motor_id].pre_stepcnt = stmotors[motor_id].cur_stepcnt;
    stmotors[motor_id].cur_stepcnt = stepcnt;
    stmotors[motor_id].location_access ++;
    if (stmotors[motor_id].direction == MOVE_REVERSE)
    {
        stmotors[motor_id].location -= (stmotors[motor_id].cur_stepcnt - stmotors[motor_id].pre_stepcnt); 
    }
    else
    {
        stmotors[motor_id].location += (stmotors[motor_id].cur_stepcnt - stmotors[motor_id].pre_stepcnt); 
    }
    return (stepcnt);
}
//MRVL-Harris for stepper cdma end


static void smot_step_set_out_mode(stmotor_id_t motor_id)
{

    uint32_t out_mode_val = 0;

    out_mode_val = STMOTOR_REG_OUT_MODE_O_MODE0_REPLACE_VAL(out_mode_val, smot_step_get_out_mode(stmotors[motor_id].stmotor_connects, 0));
    out_mode_val = STMOTOR_REG_OUT_MODE_O_MODE1_REPLACE_VAL(out_mode_val, smot_step_get_out_mode(stmotors[motor_id].stmotor_connects, 1));
    out_mode_val = STMOTOR_REG_OUT_MODE_O_MODE2_REPLACE_VAL(out_mode_val, smot_step_get_out_mode(stmotors[motor_id].stmotor_connects, 2));
    out_mode_val = STMOTOR_REG_OUT_MODE_O_MODE3_REPLACE_VAL(out_mode_val, smot_step_get_out_mode(stmotors[motor_id].stmotor_connects, 3));
    out_mode_val = STMOTOR_REG_OUT_MODE_O_MODE4_REPLACE_VAL(out_mode_val, smot_step_get_out_mode(stmotors[motor_id].stmotor_connects, 4));
    out_mode_val = STMOTOR_REG_OUT_MODE_O_MODE5_REPLACE_VAL(out_mode_val, smot_step_get_out_mode(stmotors[motor_id].stmotor_connects, 5));

    stmotors[motor_id].stmotor_regs->OUT_MODE = out_mode_val;
}


static void smot_step_set_blk_addr_irq(stmotor_id_t motor_id)
{
    stmotor_block_t block_num;
    int pid_table[] = {35,36,37,38,39,40};

    block_num = stmotors[motor_id].stmotor_connects->block_num;

    switch (block_num)
    {
        case STEP_MTR_BLK_0:
            stmotors[motor_id].stmotor_regs = (STMOTOR_REG_REGS_t *)APB_STMOTOR6PIN0_BASE;
            stmotors[motor_id].stmotor_irq_num = IRQ_MOTOR_0;
            stmotors[motor_id].stmotor_cdma_num = 35; //MRVL-Harris for stepper cdma
            break;
        case STEP_MTR_BLK_1:
            stmotors[motor_id].stmotor_regs = (STMOTOR_REG_REGS_t *)APB_STMOTOR6PIN1_BASE;
            stmotors[motor_id].stmotor_irq_num = IRQ_MOTOR_1;
            stmotors[motor_id].stmotor_cdma_num = 36; //MRVL-Harris for stepper cdma
            break;
        case STEP_MTR_BLK_2:
            stmotors[motor_id].stmotor_regs = (STMOTOR_REG_REGS_t *)APB_STMOTOR6PIN2_BASE;
            stmotors[motor_id].stmotor_irq_num = IRQ_MOTOR_2;
            stmotors[motor_id].stmotor_cdma_num = 37; //MRVL-Harris for stepper cdma
            break;
        case STEP_MTR_BLK_3:
            stmotors[motor_id].stmotor_regs = (STMOTOR_REG_REGS_t *)APB_STMOTOR6PIN3_BASE;
            stmotors[motor_id].stmotor_irq_num = IRQ_MOTOR_3;
            stmotors[motor_id].stmotor_cdma_num = 38; //MRVL-Harris for stepper cdma
            break;
        case STEP_MTR_BLK_4:
            stmotors[motor_id].stmotor_regs = (STMOTOR_REG_REGS_t *)APB_STMOTOR4PIN0_BASE;
            stmotors[motor_id].stmotor_irq_num = IRQ_MOTOR_4;
            stmotors[motor_id].stmotor_cdma_num = 39; //MRVL-Harris for stepper cdma
            break;
        case STEP_MTR_BLK_5:
            stmotors[motor_id].stmotor_regs = (STMOTOR_REG_REGS_t *)APB_STMOTOR4PIN1_BASE;
            stmotors[motor_id].stmotor_irq_num = IRQ_MOTOR_5;
            stmotors[motor_id].stmotor_cdma_num = 40; //MRVL-Harris for stepper cdma
            break;
        default:
            step_func_error("%d is invalid stepper motor block number\n", block_num);
            XASSERT(0, block_num);
            break;
    }
#ifdef __KERNEL__
    
    stmotors[motor_id].tx_dma_slave.vtype = MV61_VDMA_OWNED;
    stmotors[motor_id].tx_dma_slave.wr_delay = 0;
    stmotors[motor_id].tx_dma_slave.destendian = MV61_DMA_LITTLE_ENDIAN;
    stmotors[motor_id].tx_dma_slave.srcendian = MV61_DMA_LITTLE_ENDIAN;
    stmotors[motor_id].tx_dma_slave.flowctrl = MV61_DMA_MEMORY_TO_PERIPHERAL;
    stmotors[motor_id].tx_dma_slave.dest_pid = pid_table[block_num];
    stmotors[motor_id].tx_dma_slave.dest_addr_inc = true;
    stmotors[motor_id].tx_dma_slave.src_addr_inc = true;
    stmotors[motor_id].tx_dma_slave.dest_width = MV61_DMA_XFER_WIDTH_32BIT;
    stmotors[motor_id].tx_dma_slave.src_width = MV61_DMA_XFER_WIDTH_32BIT;
    stmotors[motor_id].tx_dma_slave.data_unit_size = MV61_DMA_UNIT_SIZE_32BIT;
    stmotors[motor_id].tx_dma_slave.dest_burst = MV61_DMA_BURST1;
    stmotors[motor_id].tx_dma_slave.src_burst = MV61_DMA_BURST1;
    stmotors[motor_id].tx_dma_slave.dest_reg = (dma_addr_t)&(stmotors[motor_id].stmotor_regs->PWM_T);
    stmotors[motor_id].tx_dma_slave.timebase = MV61_TIMEBASE_1MS;
    stmotors[motor_id].tx_dma_slave.timer = 0;
    stmotors[motor_id].tx_dma_slave.wrap = 24;

#endif
}


static void smot_step_set_pwm_m(stmotor_id_t motor_id, uint32_t pwm_m)
{
    ASSERT( stmotors[motor_id].stmotor_regs );
    stmotors[motor_id].stmotor_regs->PWM_M = STMOTOR_REG_PWM_M_PWM_M_REPLACE_VAL(stmotors[motor_id].stmotor_regs->PWM_M, pwm_m);
}


static void smot_step_set_pwm_p(stmotor_id_t motor_id, uint32_t pwm_p)
{
    ASSERT( stmotors[motor_id].stmotor_regs );
    stmotors[motor_id].stmotor_regs->PWM_P = STMOTOR_REG_PWM_P_PWM_P_REPLACE_VAL(stmotors[motor_id].stmotor_regs->PWM_P, pwm_p);
}


static void smot_step_set_reg_bit0(stmotor_id_t motor_id, bool bit0)
{
    ASSERT( stmotors[motor_id].stmotor_regs );
    stmotors[motor_id].stmotor_regs->REG_01 = STMOTOR_REG_REG_01_REG_BIT0_REPLACE_VAL(stmotors[motor_id].stmotor_regs->REG_01, bit0);
}


static void smot_step_set_reg_bit1(stmotor_id_t motor_id, bool bit1)
{
    ASSERT( stmotors[motor_id].stmotor_regs );
    stmotors[motor_id].stmotor_regs->REG_01 = STMOTOR_REG_REG_01_REG_BIT1_REPLACE_VAL(stmotors[motor_id].stmotor_regs->REG_01, bit1);
}


static void smot_step_set_acc_incr(stmotor_id_t motor_id)
{
    uint8_t acc_incr;

    acc_incr = stmotors[motor_id].cur_dpi_profile->acc_incr;

    ASSERT( stmotors[motor_id].stmotor_regs );
    stmotors[motor_id].stmotor_regs->ACC_INCR = STMOTOR_REG_COM_ACC_INCR_ACC_INCR_REPLACE_VAL(stmotors[motor_id].stmotor_regs->ACC_INCR, acc_incr);
}


void smot_step_int_enable(stmotor_id_t motor_id)
{
    uint32_t intAckMask;

    intAckMask =
        (   STMOTOR_REG_I_ACK_CMD_MASK |
            STMOTOR_REG_I_ACK_STEP_INT_MASK |
            STMOTOR_REG_I_ACK_FIFO_OVERFLOW_MASK |
            STMOTOR_REG_I_ACK_I_OMODE_MASK |
            STMOTOR_REG_I_ACK_I_USTEPS_MASK |
            STMOTOR_REG_I_ACK_STCNT_UO_MASK |
            STMOTOR_REG_I_ACK_I_INST_MASK |
            STMOTOR_REG_I_ACK_PCNT_UO_MASK |
            STMOTOR_REG_I_ACK_MCNT_UO_MASK
        );
    stmotors[motor_id].stmotor_regs->I_ACK = intAckMask;
    stmotors[motor_id].stmotor_regs->I_ACK = 0;
}


static void smot_step_int_disable(stmotor_id_t motor_id)
{
    stmotors[motor_id].stmotor_regs->I_EN = 0;
}


void smot_step_set_line_start(stmotor_id_t   motor_id,
                              uint8_t        ls_src,
                              uint16_t       ls_incr)
{
    if (ls_incr == 0)
    {
        step_func_error("ls_incr should not be zero\n");
        ASSERT(0);
    }
    stmotors[motor_id].stmotor_regs->LS_INCR = (STMOTOR_LS_INCR_LS_SRC(ls_src) |
                                                STMOTOR_LS_INCR_LS_INCR(ls_incr-1));
}


void smot_step_enable_line_start(stmotor_id_t motor_id,
                                 bool enable)
{
    stmotors[motor_id].stmotor_regs->CMD = STMOTOR_REG_CMD_LS_EN_REPLACE_VAL(stmotors[motor_id].stmotor_regs->CMD, enable);
    stmotors[motor_id].ls_en = enable;
}


//MRVL-Harris for stepper cdma start
void smot_step_motor_cdma_monitor(stmotor_id_t motor_id)
{
    #ifndef __KERNEL__
    uint32_t          status;
    #endif
    uint32_t          step_moved;
    STMotorState_t    smmotor_pre_state;
    STMotorState_t    smmotor_cur_state;

    step_func_enter();
    step_moved = smot_step_get_step_count(motor_id);
    if (step_moved >= stmotors[motor_id].target_steps)
    {
        stmotors[motor_id].pending_steps = 0;      
    }
    else
    {
        stmotors[motor_id].pending_steps = stmotors[motor_id].target_steps - step_moved;
    }

    smmotor_cur_state = stmotors[motor_id].stmotor_cur_state;
    smmotor_pre_state = smmotor_cur_state;

    if (stmotors[motor_id].stop_requested) //need to ramp down the motor immediately
    {
        if (smmotor_cur_state == STMOTOR_AT_SPEED)
        {
            //cmd abort was done in smot_step_request_motor_stop
            smmotor_cur_state = STMOTOR_RAMPING_DOWN;
            stmotors[motor_id].stop_requested = false;
            //actually, this interrupt was issued at the end of the 1st cmd of ramping_down.
            //so one cmd was used.
            stmotors[motor_id].target_steps = step_moved + stmotors[motor_id].ramp_up_steps - stmotors[motor_id].max_command_steps;
            stmotors[motor_id].pending_steps = stmotors[motor_id].target_steps - step_moved;
            if (smot_step_debug_in_interrupt_enable)
            {
                step_func_infor("xxx1: motor_id=%d, step_moved=%d, new target_steps=%d\n", 
                    motor_id,
                    step_moved,
                    stmotors[motor_id].target_steps);
            }
        }
        else if ((smmotor_cur_state == STMOTOR_STOP) || (smmotor_cur_state == STMOTOR_RAMPING_DOWN))
        {
            //do nothing
            stmotors[motor_id].stop_requested = false;
        }
        else if (smmotor_cur_state == STMOTOR_RAMPING_UP)
        {
            if ((step_moved + stmotors[motor_id].max_command_steps) >= stmotors[motor_id].ramp_up_steps)
            {
                smot_step_command_abort(motor_id, true); //abort the last cmd of ramping_up
                smot_step_command_abort(motor_id, true); //abort the cmd of at_speed
                if (stmotors[motor_id].ls_en)
                {
                    smot_step_command_abort(motor_id, true); //drop one more cmd for ls
                }
                smmotor_cur_state = STMOTOR_RAMPING_DOWN;
                stmotors[motor_id].stop_requested = false;
                stmotors[motor_id].target_steps = step_moved + stmotors[motor_id].ramp_up_steps;
                stmotors[motor_id].pending_steps = stmotors[motor_id].target_steps - step_moved;
                if (smot_step_debug_in_interrupt_enable)
                {
                    step_func_infor("xxx2: motor_id=%d, step_moved=%d, new target_steps=%d\n", 
                        motor_id, 
                        step_moved,
                        stmotors[motor_id].target_steps);
                }
            }
        }          
    } 

    if (stmotors[motor_id].stmotor_cdma_enable == false)
    {
        smmotor_cur_state = STMOTOR_STOP;
        smot_step_int_disable(motor_id);
        smot_step_update_location(motor_id);
        smot_step_cdma_mode_enable(motor_id, false);
        #ifdef HAVE_PINETREE_FORM_FACTOR
        smot_step_set_idle(motor_id, false, 0x3F);
        #else
        smot_step_set_idle(motor_id, false, 0);
        #endif
        smot_step_block_enable(motor_id, false);
        #ifndef __KERNEL__
        status = CdmaClose(stmotors[motor_id].stmotor_cdma_handle);
        if (status != 0)
        {
            step_func_error("CdmaClose error\n");
            ASSERT(0);
        }
        tx_semaphore_delete(&stmotor_cdma_semaphore);
        #else
        //dmaengine_terminate_all(stmotors[motor_id].stmotor_dma_chan);
        #endif
    }
    else if (smmotor_cur_state == STMOTOR_STOP)
    {
        if (stmotors[motor_id].pending_steps == 0)
        {
			TIMESTAMP;
            smmotor_cur_state = STMOTOR_STOP;
            smot_step_int_disable(motor_id);
            smot_step_update_location(motor_id);
            smot_step_cdma_mode_enable(motor_id, false);
            #ifdef HAVE_PINETREE_FORM_FACTOR
            smot_step_set_idle(motor_id, false, 0x3F);
            #else
            smot_step_set_idle(motor_id, false, 0);
            #endif
            smot_step_block_enable(motor_id, false);
            #ifndef __KERNEL__
            status = CdmaClose(stmotors[motor_id].stmotor_cdma_handle);
            if (status != 0)
            {
                step_func_error("CdmaClose error\n");
                ASSERT(0);
            }
            tx_semaphore_delete(&stmotor_cdma_semaphore);
            #else
            //dmaengine_terminate_all(stmotors[motor_id].stmotor_dma_chan);
            #endif 
        }
        else
        {
            if (stmotors[motor_id].ramp_up_steps == 0) //ramp-up was disabled
            {
                smmotor_cur_state = STMOTOR_STOP;
            }
            else //ramp-up was enabled
            {
                smmotor_cur_state = STMOTOR_RAMPING_UP;
            }
        }
    }
    else if (smmotor_cur_state == STMOTOR_RAMPING_UP)
    {
        if ((step_moved + stmotors[motor_id].max_command_steps) >= stmotors[motor_id].ramp_up_steps)
        {
            if (smot_step_debug_in_interrupt_enable)
            {
                step_func_infor("xxx3: motor_id=%d, step_moved=%d, ramp_up_steps=%d\n", 
                    motor_id,
                    step_moved, 
                    stmotors[motor_id].ramp_up_steps);
            }
            smmotor_cur_state = STMOTOR_AT_SPEED;
        }
        else
        {
            smmotor_cur_state = STMOTOR_RAMPING_UP;
        }
    }
    else if (smmotor_cur_state == STMOTOR_AT_SPEED)
    {
		if ((step_moved + stmotors[motor_id].max_command_steps) >= stmotors[motor_id].ramp_down_steps)
		{
			//actually, there is no cmd interrupt during at_speed.
			//this interrupt was issued at ramping_down, not at_speed.
			if (smot_step_debug_in_interrupt_enable)
			{
				step_func_infor("xxx4: motor_id=%d, step_moved=%d, ramp_down_steps=%d\n", 
					motor_id,
					step_moved, 
					stmotors[motor_id].ramp_down_steps);
			}
			smmotor_cur_state = STMOTOR_RAMPING_DOWN;
		}
		else
		{
			smmotor_cur_state = STMOTOR_AT_SPEED;
		}
    }
    else if (smmotor_cur_state == STMOTOR_RAMPING_DOWN)
    {
        if ((step_moved + stmotors[motor_id].max_command_steps) >= stmotors[motor_id].target_steps)
        {
            if (smot_step_debug_in_interrupt_enable)
            {
                step_func_infor("xxx5: motor_id=%d, step_moved=%d, target_steps=%d\n", 
                    motor_id,
                    step_moved, 
                    stmotors[motor_id].target_steps);
            }
            smmotor_cur_state = STMOTOR_STOP;
        }
        else
        {
            smmotor_cur_state = STMOTOR_RAMPING_DOWN;
        }
    }
    /* Mod.kks add the motor state(STMOTOR_PREHOLD/ STMOTOR_POSTHOLD/STMOTOR_INVALID).*/
    else if (smmotor_cur_state == STMOTOR_PREHOLD)
    {
		if (stmotors[motor_id].preholding_steps > 0) 
			{
				smmotor_cur_state = STMOTOR_PREHOLD;
			}
			else
			{
				smmotor_cur_state = STMOTOR_RAMPING_UP;
			}
    }
    else if (smmotor_cur_state == STMOTOR_POSTHOLD)
    {
		if (stmotors[motor_id].postholding_steps > 0) 
			{
				smmotor_cur_state = STMOTOR_POSTHOLD;
			}
			else
			{
				smmotor_cur_state = STMOTOR_STOP;
			}
    }
    else if (smmotor_cur_state == STMOTOR_INVALID)
    {
		smmotor_cur_state = STMOTOR_STOP;
		dbg1(" [Mech] Motor pre_state : %d   cur_state : %d \n", smmotor_pre_state, smmotor_cur_state);
    }
    else
    {
        step_func_error("invalid motor state [%d]\n", smmotor_cur_state);
        ASSERT(0);    
    }

    if (smot_step_debug_in_interrupt_enable)
    {
        if (smmotor_pre_state != smmotor_cur_state)
        {
            step_func_infor("motor_id=%d, smmotor_pre_state=%d, smmotor_cur_state=%d\n", 
                motor_id,
                smmotor_pre_state, 
                smmotor_cur_state);
        }
    }

    stmotors[motor_id].stmotor_pre_state = smmotor_pre_state;
    stmotors[motor_id].stmotor_cur_state = smmotor_cur_state;
}


#ifndef __KERNEL__
void smot_step_motor_cdma_callback(void *cb_data, uint32_t isr_pending_bits) 
#else
void smot_step_motor_cdma_callback(void *cb_data) 
#endif
{
    stmotor_id_t  motor_id;
    uint32_t      *temp;
    
    temp = cb_data;
    for (motor_id=0; motor_id<stmotors_created; motor_id++) 
    {
        if (stmotors[motor_id].stmotor_cdma_num == *temp) 
        {
            break;
        }
    }
    if (!smot_step_motor_id_check(motor_id)) 
    {
        step_func_error("invalid motor id [%d]\n", motor_id);
        ASSERT(0);
        return;
    }
    #ifdef __KERNEL__
    if (stmotors[motor_id].stmotor_sg_table != NULL) {
        sg_free_table(stmotors[motor_id].stmotor_sg_table);
        kfree(stmotors[motor_id].stmotor_sg_table);
    }

    if (stmotors[motor_id].cmd_queue != NULL) 
        kfree(stmotors[motor_id].cmd_queue);

    dmaengine_terminate_all(stmotors[motor_id].stmotor_dma_chan);
    scanplat_kernel_release_stepper_dma(&stmotors[motor_id].stmotor_dma_chan);
    #endif
    if (smot_step_debug_in_interrupt_enable)
    {
        step_func_infor("stmotors[%d]'s cdma transfer was completed\n", motor_id);
        smot_step_dump(motor_id);
    }
    stmotors[motor_id].stmotor_cdma_enable = false;
    #ifndef __KERNEL__
    tx_semaphore_put(&stmotor_cdma_semaphore);
    #else
    //MRVL-Harris-20121030 {
    stmotors[motor_id].stmotor_cur_state = STMOTOR_STOP;
    smot_step_int_disable(motor_id);
    smot_step_update_location(motor_id);
    smot_step_cdma_mode_enable(motor_id, false);
    #ifdef HAVE_PINETREE_FORM_FACTOR
    smot_step_set_idle(motor_id, false, 0x3F);
    #else
    smot_step_set_idle(motor_id, false, 0);
    #endif
    smot_step_block_enable(motor_id, false);
    //MRVL-Harris-20121030 }
    up(&stmotors[motor_id].sg_sem);
    #endif
}
//MRVL-Harris for stepper cdma end


#if 0 //MRVL-Harris for stepper cdma
static void smot_step_set_command_packet(stmotor_id_t motor_id, stepper_cmd_packet_t stepper_cmd_packet)
{
    uint32_t temp;

    stmotors[motor_id].stmotor_regs->PWM_M = STMOTOR_REG_PWM_M_PWM_M_REPLACE_VAL(stmotors[motor_id].stmotor_regs->PWM_M, stepper_cmd_packet.pwm_m);
    stmotors[motor_id].stmotor_regs->PWM_P = STMOTOR_REG_PWM_P_PWM_P_REPLACE_VAL(stmotors[motor_id].stmotor_regs->PWM_P, stepper_cmd_packet.pwm_p);
    stmotors[motor_id].stmotor_regs->ACC_INCR = STMOTOR_REG_ACC_INCR_ACC_INCR_REPLACE_VAL(stmotors[motor_id].stmotor_regs->ACC_INCR, stepper_cmd_packet.acc_incr);
    temp = (STMOTOR_CMD_ST_TGT(stepper_cmd_packet.st_tgt) |
            STMOTOR_CMD_INF(stepper_cmd_packet.inf) |
            STMOTOR_CMD_LS_EN(stepper_cmd_packet.ls_en) |
            STMOTOR_CMD_DIR(stepper_cmd_packet.dir) |
            STMOTOR_CMD_ACC_DIR(stepper_cmd_packet.acc_dir) |
            STMOTOR_CMD_CMD(stepper_cmd_packet.cmd));
    stmotors[motor_id].stmotor_regs->CMD = temp;
    stmotors[motor_id].stmotor_regs->CMD_ISSUE = stepper_cmd_packet.issue;
}


static stepper_cmd_packet_t smot_step_get_command_packet(stmotor_id_t motor_id)
{
    stepper_cmd_packet_t stepper_cmd_packet;
    dpi_profile_rec     *pp;
    STMotorState_t       smmotor_pre_state;
    STMotorState_t       smmotor_cur_state;
    uint32_t  step_moved;
    uint32_t  rampup_index, rampdown_index, ramp_limit;
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

    pp = stmotors[motor_id].cur_dpi_profile;
    step_moved = stmotors[motor_id].target_steps - stmotors[motor_id].pending_steps;
    smmotor_cur_state = stmotors[motor_id].stmotor_cur_state;
    smmotor_pre_state = smmotor_cur_state;

    //init for build error
    pwm_m = pp->pwm_m_start;
    st_tgt = 0;
    cmd = STMOTOR_CMD_HOLD;
    issue = false;

    if (stmotors[motor_id].stop_requested) //need to ramp down the motor immediately
    {
        if (smmotor_cur_state != STMOTOR_STOP)
        {
            stmotors[motor_id].pending_steps = stmotors[motor_id].target_steps - stmotors[motor_id].ramp_down_steps;
            step_moved = stmotors[motor_id].ramp_down_steps;
            smmotor_cur_state = STMOTOR_RAMPING_DOWN;
        }
        stmotors[motor_id].stop_requested = false;
    }

    if (smmotor_cur_state == STMOTOR_STOP)
    {
        //the next state of STMOTOR_STOP can be STMOTOR_STOP, STMOTOR_PREHOLD, STMOTOR_RAMPING_UP or STMOTOR_AT_SPEED
        if (stmotors[motor_id].pending_steps == 0)
        {
            pwm_m = pp->pwm_m_start;
            smmotor_cur_state = STMOTOR_STOP;
            st_tgt = 0;
            cmd = STMOTOR_CMD_HOLD;
            issue = false;
            smot_step_update_location(motor_id);
            #ifdef HAVE_PINETREE_FORM_FACTOR
            smot_step_set_idle(motor_id, false, 0x3F); //MRVL-Harris-20120329 for step loss
            #else
            smot_step_set_idle(motor_id, false, 0); //MRVL-Harris-20120329 for step loss
            #endif
            smot_step_block_enable(motor_id, false); //MRVL-Harris
        }
        else
        {
            if (stmotors[motor_id].preholding_steps > 0) //preholding function was enabled
            {
                pwm_m = pp->pwm_m_start;
                smmotor_cur_state = STMOTOR_PREHOLD;
                st_tgt = stmotors[motor_id].preholding_steps;
                cmd = STMOTOR_CMD_HOLD;
                issue = true;
                stmotors[motor_id].preholding_steps = 0;
            }
            else //preholding function was disabled
            {
                if (stmotors[motor_id].ramp_up_steps == 0) //ramp-up was disabled
                {
                    pwm_m = pp->pwm_m_target;
                    smmotor_cur_state = STMOTOR_AT_SPEED;
                }
                else //ramp-up was enabled
                {
                    rampup_index = step_moved / stmotors[motor_id].max_command_steps;
                    ramp_limit = stmotors[motor_id].ramp_step_limit / stmotors[motor_id].max_command_steps;
                    if (rampup_index >= ramp_limit)
                    {
                        rampup_index = ramp_limit - 1;
                    }
                    pwm_m = pp->accel_table[rampup_index];
                    smmotor_cur_state = STMOTOR_RAMPING_UP;
                }
                if (stmotors[motor_id].pending_steps >= stmotors[motor_id].max_command_steps)
                {
                    st_tgt = stmotors[motor_id].max_command_steps;
                }
                else
                {
                    st_tgt = stmotors[motor_id].pending_steps;
                }
                cmd = STMOTOR_CMD_STEP;
                issue = true;
            }
        }
    }
    else if (smmotor_cur_state == STMOTOR_PREHOLD)
    {
        //the next state of STMOTOR_PREHOLD can be STMOTOR_RAMPING_UP or STMOTOR_AT_SPEED
        if (stmotors[motor_id].ramp_up_steps == 0) //ramp-up was disabled
        {
            pwm_m = pp->pwm_m_target;
            smmotor_cur_state = STMOTOR_AT_SPEED;
        }
        else //ramp-up was enabled
        {
            rampup_index = step_moved / stmotors[motor_id].max_command_steps;
            ramp_limit = stmotors[motor_id].ramp_step_limit / stmotors[motor_id].max_command_steps;
            if (rampup_index >= ramp_limit)
            {
                rampup_index = ramp_limit - 1;
            }
            pwm_m = pp->accel_table[rampup_index];
            smmotor_cur_state = STMOTOR_RAMPING_UP;
        }
        if (stmotors[motor_id].pending_steps >= stmotors[motor_id].max_command_steps)
        {
            st_tgt = stmotors[motor_id].max_command_steps;
        }
        else
        {
            st_tgt = stmotors[motor_id].pending_steps;
        }
        cmd = STMOTOR_CMD_STEP;
        issue = true;
    }
    else if (smmotor_cur_state == STMOTOR_RAMPING_UP)
    {
        //the next state of STMOTOR_RAMPING_UP can be STMOTOR_RAMPING_UP, STMOTOR_AT_SPEED or STMOTOR_RAMPING_DOWN
        if (stmotors[motor_id].ramp_up_steps == 0) //ramp-up was disabled
        {
            pwm_m = pp->pwm_m_target;
            smmotor_cur_state = STMOTOR_AT_SPEED;
        }
        else //ramp-up was enabled
        {
            if (step_moved <= stmotors[motor_id].ramp_up_steps)
            {
                rampup_index = step_moved / stmotors[motor_id].max_command_steps;
                ramp_limit = stmotors[motor_id].ramp_step_limit / stmotors[motor_id].max_command_steps;
                if (rampup_index >= ramp_limit)
                {
                   rampup_index = ramp_limit - 1;
                }
                pwm_m = pp->accel_table[rampup_index];
                smmotor_cur_state = STMOTOR_RAMPING_UP;
            }
            else if (step_moved <= stmotors[motor_id].ramp_down_steps)
            {
                pwm_m = pp->pwm_m_target;
                smmotor_cur_state = STMOTOR_AT_SPEED;
            }
            else
            {
                rampdown_index = (step_moved - stmotors[motor_id].ramp_down_steps) / stmotors[motor_id].max_command_steps;
                if (stmotors[motor_id].ramp_down_steps == stmotors[motor_id].ramp_up_steps)
                {
                    ramp_limit = stmotors[motor_id].ramp_down_steps / stmotors[motor_id].max_command_steps;
                }
                else
                {
                    ramp_limit = stmotors[motor_id].ramp_step_limit / stmotors[motor_id].max_command_steps;
                }
                if (rampdown_index > ramp_limit)
                {
                    rampdown_index = ramp_limit;
                }
                rampdown_index = ramp_limit - rampdown_index;
                if (rampdown_index > 0)
                {
                    rampdown_index--;
                }
                if (rampdown_index >= ramp_limit)
                {
                    rampdown_index = ramp_limit - 1;
                }
                pwm_m = pp->accel_table[rampdown_index];
                smmotor_cur_state = STMOTOR_RAMPING_DOWN;
            }
        }
        if (stmotors[motor_id].pending_steps >= stmotors[motor_id].max_command_steps)
        {
            st_tgt = stmotors[motor_id].max_command_steps;
        }
        else
        {
            st_tgt = stmotors[motor_id].pending_steps;
        }
        cmd = STMOTOR_CMD_STEP;
        issue = true;
    }
    else if (smmotor_cur_state == STMOTOR_AT_SPEED)
    {
        //the next state of STMOTOR_AT_SPEED can be STMOTOR_AT_SPEED, STMOTOR_RAMPING_DOWN, STMOTOR_POSTHOLD or STMOTOR_STOP
        if (stmotors[motor_id].target_steps == 0xFFFFFFFF)
        {
            pwm_m = pp->pwm_m_target;
            smmotor_cur_state = STMOTOR_AT_SPEED;
            st_tgt = stmotors[motor_id].max_command_steps;
            if (step_moved >= 2 * (stmotors[motor_id].ls_start_pos))
            {
                stmotors[motor_id].pending_steps += st_tgt;
            }
            cmd = STMOTOR_CMD_STEP;
            issue = true;

        }else
        if (stmotors[motor_id].ramp_down_steps == stmotors[motor_id].target_steps) //ramp-down was disabled
        {
            if (stmotors[motor_id].pending_steps > 0) //target was not reached
            {
                //constant speed
                pwm_m = pp->pwm_m_target;
                smmotor_cur_state = STMOTOR_AT_SPEED;
                if (stmotors[motor_id].pending_steps >= stmotors[motor_id].max_command_steps)
                {
                    st_tgt = stmotors[motor_id].max_command_steps;
                }
                else
                {
                    st_tgt = stmotors[motor_id].pending_steps;
                }
                cmd = STMOTOR_CMD_STEP;
                issue = true;
            }
            else //target was reached
            {
                if (stmotors[motor_id].postholding_steps > 0) //postholding function was enabled
                {
                    //posthold
                    pwm_m = pp->pwm_m_start;
                    smmotor_cur_state = STMOTOR_POSTHOLD;
                    st_tgt = stmotors[motor_id].postholding_steps;
                    cmd = STMOTOR_CMD_HOLD;
                    issue = true;
                    stmotors[motor_id].postholding_steps = 0;
                }
                else //postholding function was disabled
                {
                    //stop
                    pwm_m = pp->pwm_m_start;
                    smmotor_cur_state = STMOTOR_STOP;
                    st_tgt = stmotors[motor_id].max_command_steps;
                    cmd = STMOTOR_CMD_HOLD;
                    issue = true;
                }
            }
        }
        else //ramp-down was enabled
        {
            if (step_moved <= stmotors[motor_id].ramp_down_steps)
            {
                //constant speed
                pwm_m = pp->pwm_m_target;
                smmotor_cur_state = STMOTOR_AT_SPEED;
            }
            else
            {
                //ramp-down
                rampdown_index = (step_moved - stmotors[motor_id].ramp_down_steps) / stmotors[motor_id].max_command_steps;
                if (stmotors[motor_id].ramp_down_steps == stmotors[motor_id].ramp_up_steps)
                {
                    ramp_limit = stmotors[motor_id].ramp_down_steps / stmotors[motor_id].max_command_steps;
                }
                else
                {
                    ramp_limit = stmotors[motor_id].ramp_step_limit / stmotors[motor_id].max_command_steps;
                }
                if (rampdown_index > ramp_limit)
                {
                    rampdown_index = ramp_limit;
                }
                rampdown_index = ramp_limit - rampdown_index;
                if (rampdown_index > 0)
                {
                    rampdown_index--;
                }
                if (rampdown_index >= ramp_limit)
                {
                    rampdown_index = ramp_limit - 1;
                }
                pwm_m = pp->accel_table[rampdown_index];
                smmotor_cur_state = STMOTOR_RAMPING_DOWN;
            }
            if (stmotors[motor_id].pending_steps >= stmotors[motor_id].max_command_steps)
            {
                st_tgt = stmotors[motor_id].max_command_steps;
            }
            else
            {
                st_tgt = stmotors[motor_id].pending_steps;
            }
            cmd = STMOTOR_CMD_STEP;
            issue = true;
        }
    }
    else if (smmotor_cur_state == STMOTOR_RAMPING_DOWN)
    {
        //the next state of STMOTOR_RAMPING_DOWN can be STMOTOR_RAMPING_DOWN, STMOTOR_POSTHOLD or STMOTOR_STOP
        if (stmotors[motor_id].pending_steps > 0) //target was not reached
        {
            //ramp-down
            rampdown_index = (step_moved - stmotors[motor_id].ramp_down_steps) / stmotors[motor_id].max_command_steps;
            if (stmotors[motor_id].ramp_down_steps == stmotors[motor_id].ramp_up_steps)
            {
                ramp_limit = stmotors[motor_id].ramp_down_steps / stmotors[motor_id].max_command_steps;
            }
            else
            {
                ramp_limit = stmotors[motor_id].ramp_step_limit / stmotors[motor_id].max_command_steps;
            }
            if (rampdown_index > ramp_limit)
            {
                rampdown_index = ramp_limit;
            }
            rampdown_index = ramp_limit - rampdown_index;
            if (rampdown_index > 0)
            {
                rampdown_index--;
            }
            if (rampdown_index >= ramp_limit)
            {
                rampdown_index = ramp_limit - 1;
            }
            pwm_m = pp->accel_table[rampdown_index];
            smmotor_cur_state = STMOTOR_RAMPING_DOWN;
            if (stmotors[motor_id].pending_steps >= stmotors[motor_id].max_command_steps)
            {
                st_tgt = stmotors[motor_id].max_command_steps;
            }
            else
            {
                st_tgt = stmotors[motor_id].pending_steps;
            }
            cmd = STMOTOR_CMD_STEP;
            issue = true;
        }
        else //target was reached
        {
            if (stmotors[motor_id].postholding_steps > 0) //postholding function was enabled
            {
                //posthold
                pwm_m = pp->pwm_m_start;
                smmotor_cur_state = STMOTOR_POSTHOLD;
                st_tgt = stmotors[motor_id].postholding_steps;
                cmd = STMOTOR_CMD_HOLD;
                issue = true;
                stmotors[motor_id].postholding_steps = 0;
            }
            else //postholding function was disabled
            {
                //stop
                pwm_m = pp->pwm_m_start;
                smmotor_cur_state = STMOTOR_STOP;
                st_tgt = stmotors[motor_id].max_command_steps;
                cmd = STMOTOR_CMD_HOLD;
                issue = true;
            }
        }
    }
    else if (smmotor_cur_state == STMOTOR_POSTHOLD)
    {
        //the next state of STMOTOR_POSTHOLD can be STMOTOR_STOP
        pwm_m = pp->pwm_m_start;
        smmotor_cur_state = STMOTOR_STOP;
        st_tgt = stmotors[motor_id].max_command_steps;
        cmd = STMOTOR_CMD_HOLD;
        issue = true;
    }
    else
    {
        step_func_error("invalid motor state [%d]\n", smmotor_cur_state);
        ASSERT(0);
        memset(&stepper_cmd_packet, 0, sizeof(stepper_cmd_packet));
        return stepper_cmd_packet;
    }
    if ( (cmd != STMOTOR_STOP) && (cmd != STMOTOR_CMD_HOLD) )
    {
        if ( stmotors[motor_id].pending_steps > st_tgt)
        {
            stmotors[motor_id].pending_steps -= st_tgt;
        }
        else
        {
            stmotors[motor_id].pending_steps = 0;
        }
    }

    stmotors[motor_id].stmotor_pre_state = smmotor_pre_state;
    stmotors[motor_id].stmotor_cur_state = smmotor_cur_state;

    pwm_p = pp->pwm_p; //pwm_p was fixed, only pwm_m was updated
    //pwm_m should be updated in the loop
    acc_incr = pp->acc_incr;
    //st_tgt should be updated in the loop
    inf = 0;
    ls_en = stmotors[motor_id].ls_en;
    dir = stmotors[motor_id].direction;
    acc_dir = 0;
    //cmd should be updated in the loop
    //issue should be updated in the loop

    stepper_cmd_packet.pwm_m = pwm_m;
    stepper_cmd_packet.pwm_p = pwm_p;
    stepper_cmd_packet.acc_incr = acc_incr;
    stepper_cmd_packet.st_tgt = st_tgt;;
    stepper_cmd_packet.inf = inf;

    stepper_cmd_packet.ls_en = 0;
    if (ls_en != 0)
    {
        //if (smmotor_cur_state != STMOTOR_STOP)
        if (smmotor_cur_state == STMOTOR_AT_SPEED)
        {
            if ( step_moved > stmotors[motor_id].ls_start_pos)
            {
                stepper_cmd_packet.ls_en = 1;
            }
        }
    }

    stepper_cmd_packet.dir = dir;
    stepper_cmd_packet.acc_dir = acc_dir;
    stepper_cmd_packet.cmd = cmd;
    stepper_cmd_packet.issue = issue;
    stmotors[motor_id].cmd_packet = stepper_cmd_packet;

    return (stepper_cmd_packet);
}
#endif //MRVL-Harris for stepper cdma


//MRVL-Harris for stepper cdma start
static void smot_step_run(stmotor_id_t motor_id)
{
    typedef enum 
    {
        pwm_t_pos,
        pwm_p_pos,
        acc_incr_pos,
        pwm_m_pos,
        cmd_pos,
        cmd_issue_pos,
    }step_reg_pos_t;

    uint32_t cmd[10];
    uint32_t cnt = 0;
    uint32_t temp, num_cmds;
    uint32_t regtemp;
    dpi_profile_rec     *pp;
    #ifdef __KERNEL__
    struct scatterlist *sgl = NULL;
    int                 nexts = 0;
    #endif

    step_func_infor("motor_id =%d\n", motor_id);

    stmotors[motor_id].stop_requested = false;

    //prepare cdma chain
    pp = stmotors[motor_id].cur_dpi_profile;

    #ifndef __KERNEL__
    stmotors[motor_id].stmotor_cdma_chain = cdma_chain_array_free(stmotors[motor_id].stmotor_cdma_chain);
    #endif

    if (stmotors[motor_id].max_command_steps == 0)
    {
        step_func_error("stmotors[%d].max_command_steps == 0\n", motor_id);
        ASSERT(0);
    }

    num_cmds = 0;
    
    if (stmotors[motor_id].preholding_steps != 0)
    {
        num_cmds ++; //prehold
    }
   
    if (stmotors[motor_id].ramp_up_steps != 0)
    {
        num_cmds += stmotors[motor_id].ramp_up_steps / stmotors[motor_id].max_command_steps; //ramping_up
    }

    num_cmds++; //at_speed without ls

    if ((stmotors[motor_id].ramp_up_steps != 0) && (stmotors[motor_id].ls_en == true))
    {
        num_cmds++; //at_speed with ls
    }

    if (stmotors[motor_id].ramp_up_steps != 0)
    {
        num_cmds += stmotors[motor_id].ramp_up_steps / stmotors[motor_id].max_command_steps; //ramping_down
        /* KKS Start 95step=2mm*/
		if (reduce_control == 1){
			if(motor_id == 0)
				num_cmds += FB_DECEL_CONTROL_STEP; 
			else
				num_cmds += ADF_DECEL_CONTROL_STEP; 
		}
		else{
			num_cmds += stmotors[motor_id].ramp_up_steps / stmotors[motor_id].max_command_steps; //ramping_down
		}
		/* KKS End */
    }
    /*
    if (stmotors[motor_id].postholding_steps != 0)
    {
        num_cmds++; //posthold
    }
    */
    #ifndef __KERNEL__
    num_cmds += 4; //cdma
    stmotors[motor_id].stmotor_cdma_chain = cdma_chain_array_allocate_coherent(num_cmds, cmd_issue_pos + 1);
    #else
    step_func_debug("stmotors[%d] num_cmds:%d\n", motor_id, num_cmds);
    if(down_interruptible(&stmotors[motor_id].sg_sem)) {
        return;
    }
    stmotors[motor_id].cmd_queue = kzalloc(num_cmds * (cmd_issue_pos+1) * sizeof(uint32_t), GFP_DMA);
    stmotors[motor_id].stmotor_sg_table = kmalloc(sizeof(struct sg_table), GFP_DMA);
    sg_alloc_table(stmotors[motor_id].stmotor_sg_table, num_cmds, GFP_DMA);
    sgl = stmotors[motor_id].stmotor_sg_table->sgl;
    #endif

    //prehold
    if (stmotors[motor_id].preholding_steps != 0)    
    {
        cmd[pwm_t_pos] = 0;
        cmd[pwm_p_pos] = pp->pwm_p;
        cmd[acc_incr_pos] = pp->acc_incr;
        cmd[pwm_m_pos] = pp->pwm_m_start;
        regtemp = (STMOTOR_CMD_ST_TGT(stmotors[motor_id].preholding_steps) |
                   STMOTOR_CMD_INF(0) |
                   STMOTOR_CMD_LS_EN(0) |
                   STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                   STMOTOR_CMD_ACC_DIR(0) |
                   STMOTOR_CMD_CMD(STMOTOR_CMD_HOLD));
        cmd[cmd_pos] = regtemp;
        cmd[cmd_issue_pos] = true;
        #ifndef __KERNEL__
        cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                &stmotors[motor_id].stmotor_regs->PWM_P,
                                &cmd[pwm_p_pos],
                                1 + (cmd_issue_pos - pwm_p_pos));
        #else
        #define CMD_SZ  (6 * 4)
        #define SGL  (stmotors[motor_id].stmotor_sg_table->sgl)
        #define ADD_SG(name_parm) { \
        memcpy(&stmotors[motor_id].cmd_queue[nexts*CMD_SZ], cmd, CMD_SZ); \
        sg_set_buf(sgl, &stmotors[motor_id].cmd_queue[nexts*CMD_SZ], CMD_SZ); \
        step_func_debug("%s Added cmd buff %d @:%p size:%d\n", name_parm, nexts+1,&stmotors[motor_id].cmd_queue[nexts*CMD_SZ], CMD_SZ); \
        nexts++; \
        sgl = sg_next(sgl); }
        ADD_SG("1st");
        #endif
    }

    //ramping_up
    if (stmotors[motor_id].ramp_up_steps != 0)
    {
        temp = stmotors[motor_id].ramp_up_steps / stmotors[motor_id].max_command_steps;
        for (cnt=0; (cnt<temp); cnt++) 
        {
            cmd[pwm_t_pos] = 0;
            cmd[pwm_p_pos] = pp->pwm_p;
            cmd[acc_incr_pos] = pp->acc_incr;
            cmd[pwm_m_pos] = pp->accel_table[cnt];
            regtemp = (STMOTOR_CMD_ST_TGT(stmotors[motor_id].max_command_steps) |
                       STMOTOR_CMD_INF(0) |
                       STMOTOR_CMD_LS_EN(0) |
                       STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                       STMOTOR_CMD_ACC_DIR(0) |
                       STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
            cmd[cmd_pos] = regtemp;
            cmd[cmd_issue_pos] = true;
            #ifndef __KERNEL__
            cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                    &stmotors[motor_id].stmotor_regs->PWM_P,
                                    &cmd[pwm_p_pos],
                                    1 + (cmd_issue_pos - pwm_p_pos));
            #else
            ADD_SG("2nd");
            #endif
        }
    }

    //at_speed
    if (stmotors[motor_id].ramp_up_steps != 0)    
    {
        if (stmotors[motor_id].ramp_down_steps <= stmotors[motor_id].ramp_up_steps)
        {
            step_func_error("stmotors[%d].ramp_down_steps(%d) <= stmotors[%d].ramp_up_steps(%d)\n",
                                motor_id,stmotors[motor_id].ramp_down_steps,
                                motor_id,stmotors[motor_id].ramp_up_steps);                                
            ASSERT(0);
        }
    
        if (stmotors[motor_id].ls_en == true)
        {
            if (stmotors[motor_id].ls_start_pos <= stmotors[motor_id].ramp_up_steps)
            {
                step_func_error("stmotors[%d].ls_start_pos(%d) <= stmotors[%d].ramp_up_steps(%d)\n",
                                motor_id,stmotors[motor_id].ls_start_pos,
                                motor_id,stmotors[motor_id].ramp_up_steps);                                

                ASSERT(0);
            }
            if (stmotors[motor_id].ramp_down_steps <= stmotors[motor_id].ls_start_pos)
            {
                step_func_error("stmotors[%d].ramp_down_steps(%d) <= stmotors[%d].ls_start_pos(%d)\n",
                                motor_id,stmotors[motor_id].ramp_down_steps,
                                motor_id,stmotors[motor_id].ls_start_pos);
                ASSERT(0);
            }
        
            cmd[pwm_t_pos] = 0;
            cmd[pwm_p_pos] = pp->pwm_p;
            cmd[acc_incr_pos] = pp->acc_incr;
            cmd[pwm_m_pos] = pp->accel_table[cnt-1];

            temp = stmotors[motor_id].ramp_down_steps - stmotors[motor_id].ramp_up_steps;
            
            if (temp > 0xFFFF)
            {
                regtemp = (STMOTOR_CMD_ST_TGT(stmotors[motor_id].ls_start_pos - stmotors[motor_id].ramp_up_steps) |
                           STMOTOR_CMD_INF(0) |
                           STMOTOR_CMD_LS_EN(0) |
                           STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                           STMOTOR_CMD_ACC_DIR(0) |
                           STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
                cmd[cmd_pos] = regtemp;
                cmd[cmd_issue_pos] = true;
                #ifndef __KERNEL__
                cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                        &stmotors[motor_id].stmotor_regs->PWM_P,
                                        &cmd[pwm_p_pos],
                                        1 + (cmd_issue_pos - pwm_p_pos));
                #else
                ADD_SG("3rd");
                #endif

                regtemp = (STMOTOR_CMD_ST_TGT(0xFFFF) |
                           STMOTOR_CMD_INF(1) |
                           STMOTOR_CMD_LS_EN(1) |
                           STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                           STMOTOR_CMD_ACC_DIR(0) |
                           STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
                cmd[cmd_pos] = regtemp;
                cmd[cmd_issue_pos] = true;
                #ifndef __KERNEL__
                cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                        &stmotors[motor_id].stmotor_regs->PWM_P,
                                        &cmd[pwm_p_pos],
                                        1 + (cmd_issue_pos - pwm_p_pos));
                #else
                ADD_SG("4th");
                #endif
                
            }
            else
            {
                regtemp = (STMOTOR_CMD_ST_TGT(stmotors[motor_id].ls_start_pos - stmotors[motor_id].ramp_up_steps) |
                           STMOTOR_CMD_INF(0) |
                           STMOTOR_CMD_LS_EN(0) |
                           STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                           STMOTOR_CMD_ACC_DIR(0) |
                           STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
                cmd[cmd_pos] = regtemp;
                cmd[cmd_issue_pos] = true;
                #ifndef __KERNEL__
                cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                        &stmotors[motor_id].stmotor_regs->PWM_P,
                                        &cmd[pwm_p_pos],
                                        1 + (cmd_issue_pos - pwm_p_pos));
                #else
                ADD_SG("5th");
                #endif

                regtemp = (STMOTOR_CMD_ST_TGT(stmotors[motor_id].ramp_down_steps - stmotors[motor_id].ls_start_pos) |
                           STMOTOR_CMD_INF(0) |
                           STMOTOR_CMD_LS_EN(1) |
                           STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                           STMOTOR_CMD_ACC_DIR(0) |
                           STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
                cmd[cmd_pos] = regtemp;
                cmd[cmd_issue_pos] = true;
                #ifndef __KERNEL__
                cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                        &stmotors[motor_id].stmotor_regs->PWM_P,
                                        &cmd[pwm_p_pos],
                                        1 + (cmd_issue_pos - pwm_p_pos));
                #else
                ADD_SG("6th");
                #endif
                
            }
        }            
        else
        {
            cmd[pwm_t_pos] = 0;
            cmd[pwm_p_pos] = pp->pwm_p;
            cmd[acc_incr_pos] = pp->acc_incr;
            cmd[pwm_m_pos] = pp->accel_table[cnt-1];
            temp = stmotors[motor_id].ramp_down_steps - stmotors[motor_id].ramp_up_steps;
            if (temp > 0xFFFF)
            {
                regtemp = (STMOTOR_CMD_ST_TGT(0xFFFF) |
                           STMOTOR_CMD_INF(1) |
                           STMOTOR_CMD_LS_EN(0) |
                           STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                           STMOTOR_CMD_ACC_DIR(0) |
                           STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
            }
            else
            {
                regtemp = (STMOTOR_CMD_ST_TGT(temp) |
                           STMOTOR_CMD_INF(0) |
                           STMOTOR_CMD_LS_EN(0) |
                           STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                           STMOTOR_CMD_ACC_DIR(0) |
                           STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
            }
            cmd[cmd_pos] = regtemp;
            cmd[cmd_issue_pos] = true;
            #ifndef __KERNEL__
            cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                    &stmotors[motor_id].stmotor_regs->PWM_P,
                                    &cmd[pwm_p_pos],
                                    1 + (cmd_issue_pos - pwm_p_pos));
            #else
            ADD_SG("7th");
            #endif
        }
    }
    else
    {
        cmd[pwm_t_pos] = 0;
        cmd[pwm_p_pos] = pp->pwm_p;
        cmd[acc_incr_pos] = pp->acc_incr;
        cmd[pwm_m_pos] = pp->pwm_m_start;
        regtemp = (STMOTOR_CMD_ST_TGT(stmotors[motor_id].target_steps) |
                   STMOTOR_CMD_INF(0) |
                   STMOTOR_CMD_LS_EN(0) |
                   STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                   STMOTOR_CMD_ACC_DIR(0) |
                   STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
        cmd[cmd_pos] = regtemp;
        cmd[cmd_issue_pos] = true;
        #ifndef __KERNEL__
        cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                &stmotors[motor_id].stmotor_regs->PWM_P,
                                &cmd[pwm_p_pos],
                                1 + (cmd_issue_pos - pwm_p_pos));
        #else
        ADD_SG("8th");
        #endif
    }

    //ramping_down
    if (stmotors[motor_id].ramp_up_steps != 0)
    {
        //temp = stmotors[motor_id].ramp_up_steps / stmotors[motor_id].max_command_steps;
        
        /* KKS Start */
       if (reduce_control == 1){
		   if(motor_id == 0)
				temp = FB_DECEL_CONTROL_STEP ; //kks 377step = 4mm
			else
				temp = ADF_DECEL_CONTROL_STEP ; //kks 944step = 10mm
		}
		else{
			temp = stmotors[motor_id].ramp_up_steps / stmotors[motor_id].max_command_steps;
		}
		/* KKS End */
		
        for (cnt=0; (cnt<temp); cnt++) 
        {
            cmd[pwm_t_pos] = 0;
            cmd[pwm_p_pos] = pp->pwm_p;
            cmd[acc_incr_pos] = pp->acc_incr;
            //cmd[pwm_m_pos] = pp->accel_table[temp-1-cnt];
            
           /*KKS Start */
           if (reduce_control == 1){// && motor_id == 0){
				cmd[pwm_m_pos] = pp->accel_table[stmotors[motor_id].ramp_up_steps -1-2*cnt];  
			}
			else{
				cmd[pwm_m_pos] = pp->accel_table[temp -1-cnt];
		   }
		   /*KKS end*/
		   
            regtemp = (STMOTOR_CMD_ST_TGT(stmotors[motor_id].max_command_steps) |
                       STMOTOR_CMD_INF(0) |
                       STMOTOR_CMD_LS_EN(0) |
                       STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                       STMOTOR_CMD_ACC_DIR(0) |
                       STMOTOR_CMD_CMD(STMOTOR_CMD_STEP));
            cmd[cmd_pos] = regtemp;
            cmd[cmd_issue_pos] = true;
            #ifndef __KERNEL__
            cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                    &stmotors[motor_id].stmotor_regs->PWM_P,
                                    &cmd[pwm_p_pos],
                                    1 + (cmd_issue_pos - pwm_p_pos));
            #else
            ADD_SG("9th");
            #endif
         }
    }
	
	/*
    //posthold
    if (stmotors[motor_id].postholding_steps != 0)    
    {
        cmd[pwm_t_pos] = 0;
        cmd[pwm_p_pos] = pp->pwm_p;
        cmd[acc_incr_pos] = pp->acc_incr;
        cmd[pwm_m_pos] = pp->pwm_m_start;
        regtemp = (STMOTOR_CMD_ST_TGT(stmotors[motor_id].postholding_steps) |
                   STMOTOR_CMD_INF(0) |
                   STMOTOR_CMD_LS_EN(0) |
                   STMOTOR_CMD_DIR(stmotors[motor_id].direction) |
                   STMOTOR_CMD_ACC_DIR(0) |
                   STMOTOR_CMD_CMD(STMOTOR_CMD_HOLD));
        cmd[cmd_pos] = regtemp;
        cmd[cmd_issue_pos] = true;
        #ifndef __KERNEL__
        cdma_chain_add_cmd_copy(stmotors[motor_id].stmotor_cdma_chain, 
                                &stmotors[motor_id].stmotor_regs->PWM_P,
                                &cmd[pwm_p_pos],
                                1 + (cmd_issue_pos - pwm_p_pos));
        #else
        ADD_SG("10th");
        #endif
    }
   */
    #ifndef __KERNEL__
    tx_semaphore_create(&stmotor_cdma_semaphore,
                        "stmotor_cdma_semaphore",
                        0);

    //open cdma
    temp = CdmaOpen(-1, &stmotors[motor_id].stmotor_cdma_handle);
    if (temp != 0)
    {
        step_func_error("CdmaOpen error\n");
        ASSERT(0);
    }
    stmotors[motor_id].stmotor_cdma_enable = true;
    temp = cdma_chain_transfer(stmotors[motor_id].stmotor_cdma_handle,
                               stmotors[motor_id].stmotor_cdma_chain,
                               stmotors[motor_id].stmotor_cdma_num, 
                               smot_step_motor_cdma_callback,
                               &stmotors[motor_id].stmotor_cdma_num);
    if (temp != 0)
    {
        step_func_error("cdma_chain_transfer error\n");
        ASSERT(0);
    }
    #else
    {
        dma_cap_mask_t mask;
        struct dma_async_tx_descriptor *tx_desc;
        dma_cookie_t cookie;
        int len;
    
        dma_cap_zero(mask);
        dma_cap_set(DMA_SLAVE,mask);
        scanplat_kernel_get_stepper_dma( &stmotors[motor_id].stmotor_dma_chan,&(stmotors[motor_id].tx_dma_slave));
//        stmotors[motor_id].stmotor_dma_chan = dma_request_channel(mask,filter,&(stmotors[motor_id].tx_dma_slave));
        if (stmotors[motor_id].stmotor_dma_chan == NULL)
        {
            step_func_error("dma_request_channel error\n");
            ASSERT(0);
        }
        stmotors[motor_id].stmotor_cdma_enable = true;
        step_func_debug("stmotors[%d].stmotor_sg_table:%p,dma_chan:%p sgl:%p nexts:%d\n", motor_id,
                         stmotors[motor_id].stmotor_sg_table, 
                         stmotors[motor_id].stmotor_dma_chan, SGL, nexts);

        #if 0
        {
            int i;
            struct scatterlist *sg;
            for_each_sg(SGL,sg,nexts,i) {
                step_func_debug("sg[%d].dma_addr:%p, dma_len:%u, offset:%xh\n", 
                                 i,
                                 (void*)sg_dma_address(sg),
                                 sg_dma_len(sg),
                                 sg->offset);
            }
        }
        #endif
        
        XASSERT( stmotors[motor_id].stmotor_dma_chan, motor_id );
        XASSERT( stmotors[motor_id].stmotor_dma_chan->device, motor_id );
        XASSERT( stmotors[motor_id].stmotor_dma_chan->device->dev, motor_id );
        len = dma_map_sg(stmotors[motor_id].stmotor_dma_chan->device->dev,
                         SGL,
                         nexts,
                         DMA_TO_DEVICE);
        if(len <= 0) step_func_error("dma_map_sg error\n");

        tx_desc = stmotors[motor_id].stmotor_dma_chan->device->device_prep_slave_sg(
                                            stmotors[motor_id].stmotor_dma_chan,
                                            SGL,
                                            len,
                                            DMA_TO_DEVICE, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
        ASSERT( tx_desc );
        if(!tx_desc) step_func_error("dma_prep_slave_sg error\n");

        tx_desc->callback = smot_step_motor_cdma_callback;
        tx_desc->callback_param = &stmotors[motor_id].stmotor_cdma_num;

        cookie = dmaengine_submit(tx_desc);
        stmotors[motor_id].stmotor_dma_chan->device->device_issue_pending(stmotors[motor_id].stmotor_dma_chan);

        //smot_step_dump(motor_id);
    }
    #endif

    smot_step_int_disable(motor_id);

    smot_step_cdma_mode_enable(motor_id, false);
    smot_step_block_enable(motor_id, false);
    smot_step_command_abort(motor_id, true);
    smot_step_count_reset(motor_id, true);
    smot_step_command_abort(motor_id, false);
    smot_step_count_reset(motor_id, false);
    
    regtemp = stmotors[motor_id].stmotor_regs->I_EN;
    regtemp = STMOTOR_REG_I_EN_CMD_REPLACE_VAL(regtemp, 1);
    stmotors[motor_id].stmotor_regs->I_ACK = stmotors[motor_id].stmotor_regs->I_PEND;
    stmotors[motor_id].stmotor_regs->I_EN = regtemp;

    while(1)
    {
        smot_step_command_abort(motor_id, true);
        regtemp = stmotors[motor_id].stmotor_regs->COM_STAT;
        if (STMOTOR_REG_COM_STAT_EMPTY_MASK_SHIFT(regtemp) == true)
        {
            break;
        }
    }
    step_func_debug("empty cmd fifo\n");

    smot_step_int_disable(motor_id);

    smot_step_cdma_mode_enable(motor_id, true);
    smot_step_block_enable(motor_id, true);

    regtemp = stmotors[motor_id].stmotor_regs->I_EN;
    regtemp = STMOTOR_REG_I_EN_CMD_REPLACE_VAL(regtemp, 1);
    stmotors[motor_id].stmotor_regs->I_ACK = stmotors[motor_id].stmotor_regs->I_PEND;
    stmotors[motor_id].stmotor_regs->I_EN = regtemp;
}
//MRVL-Harris for stepper cdma end


irqreturn_t smot_step_motor_interrupt(int irq, void *intRegMask)
{
    uint32_t              isr_pending;
    uint32_t              isr_enabled;
    stmotor_id_t          motor_id = (stmotor_id_t) intRegMask;

    step_func_debug("smot_step_motor_interrupt motor_id:%d\n", motor_id);

    if (!smot_step_motor_id_check(motor_id))
    {
        step_func_error("invalid motor id [%d]\n", motor_id);
        ASSERT(0);
        return IRQ_NONE;
    }
    if (stmotors[motor_id].stmotor_regs == NULL)
    {
        step_func_error("motor registers are null\n");
        ASSERT(0);
        return IRQ_NONE;
    }

    isr_pending = stmotors[motor_id].stmotor_regs->I_PEND;
    isr_enabled = stmotors[motor_id].stmotor_regs->I_EN;

    if (isr_pending & (STMOTOR_REG_I_PEND_I_OMODE_MASK  | 
                       STMOTOR_REG_I_PEND_I_USTEPS_MASK |
                       STMOTOR_REG_I_PEND_STCNT_UO_MASK |
                       STMOTOR_REG_I_PEND_I_INST_MASK   |
                       STMOTOR_REG_I_PEND_PCNT_UO_MASK  |
                       STMOTOR_REG_I_PEND_MCNT_UO_MASK)) 
    {
        if ((isr_pending & STMOTOR_REG_I_PEND_I_OMODE_MASK) && (isr_enabled & STMOTOR_REG_I_EN_I_OMODE_MASK))
        {
            step_func_error("illegal output mode\n");
            ASSERT(0);
        }
        else if ((isr_pending & STMOTOR_REG_I_PEND_I_USTEPS_MASK) && (isr_enabled & STMOTOR_REG_I_EN_I_USTEPS_MASK))
        {
            step_func_error("illegal number micro-steps\n");
            ASSERT(0);
        }
        else if ((isr_pending & STMOTOR_REG_I_PEND_STCNT_UO_MASK) && (isr_enabled & STMOTOR_REG_I_EN_STCNT_UO_MASK))
        {
            step_func_error("step counter under/overflow\n");
            ASSERT(0);
        }
        else if ((isr_pending & STMOTOR_REG_I_PEND_I_INST_MASK) && (isr_enabled & STMOTOR_REG_I_EN_I_INST_MASK))
        {
            step_func_error("illegal instruction\n");
            ASSERT(0);
        }
        else if ((isr_pending & STMOTOR_REG_I_PEND_PCNT_UO_MASK) && (isr_enabled & STMOTOR_REG_I_EN_PCNT_UO_MASK))
        {
            step_func_error("pwm_p under/overflow\n");
            ASSERT(0);
        }
        else if ((isr_pending & STMOTOR_REG_I_PEND_MCNT_UO_MASK) && (isr_enabled & STMOTOR_REG_I_EN_MCNT_UO_MASK))
        {
            step_func_error("pwm_m under/overflow\n");
            ASSERT(0);
        }
    }

    if ((isr_pending & STMOTOR_REG_I_PEND_FIFO_OVERFLOW_MASK) && (isr_enabled & STMOTOR_REG_I_EN_FIFO_OVERFLOW_MASK))
    {
        stmotors[motor_id].stmotor_regs->I_ACK |= STMOTOR_REG_I_ACK_FIFO_OVERFLOW_MASK;
        step_func_debug("fifo overflow\n");
    }
    if ((isr_pending & STMOTOR_REG_I_PEND_CMD_MASK) && (isr_enabled & STMOTOR_REG_I_EN_CMD_MASK))
    {
        stmotors[motor_id].stmotor_regs->I_ACK |= STMOTOR_REG_I_ACK_CMD_MASK;
        smot_step_motor_cdma_monitor(motor_id);
    }
    if ((isr_pending & STMOTOR_REG_I_PEND_STEP_INT_MASK) && (isr_enabled & STMOTOR_REG_I_EN_STEP_INT_MASK))
    {
        stmotors[motor_id].stmotor_regs->I_ACK |= STMOTOR_REG_I_ACK_STEP_INT_MASK;
    }

    return IRQ_HANDLED;
}


static void smot_step_dump(stmotor_id_t motor_id)
{
    dbg1("%16s=0x%08X\n", "SMC_CFG", stmotors[motor_id].stmotor_regs->SMC_CFG);
    dbg1("%16s=0x%08X\n", "SMC_CTRL", stmotors[motor_id].stmotor_regs->SMC_CTRL);
    dbg1("%16s=0x%08X\n", "LS_INCR", stmotors[motor_id].stmotor_regs->LS_INCR);
    dbg1("%16s=0x%08X\n", "REG_01", stmotors[motor_id].stmotor_regs->REG_01);
    dbg1("%16s=0x%08X\n", "NUM_USTEPS", stmotors[motor_id].stmotor_regs->NUM_USTEPS);
    dbg1("%16s=0x%08X\n", "STEP_EN_IDLE", stmotors[motor_id].stmotor_regs->STEP_EN_IDLE);
    dbg1("%16s=0x%08X\n", "TVT_01", stmotors[motor_id].stmotor_regs->TVT_01);
    dbg1("%16s=0x%08X\n", "TVT_23", stmotors[motor_id].stmotor_regs->TVT_23);
    dbg1("%16s=0x%08X\n", "TVT_45", stmotors[motor_id].stmotor_regs->TVT_45);
    dbg1("%16s=0x%08X\n", "TVT_67", stmotors[motor_id].stmotor_regs->TVT_67);
    dbg1("%16s=0x%08X\n", "TVT_8", stmotors[motor_id].stmotor_regs->TVT_8);
    dbg1("%16s=0x%08X\n", "SEQ_0", stmotors[motor_id].stmotor_regs->SEQ_0);
    dbg1("%16s=0x%08X\n", "SEQ_1", stmotors[motor_id].stmotor_regs->SEQ_1);
    dbg1("%16s=0x%08X\n", "SEQ_2", stmotors[motor_id].stmotor_regs->SEQ_2);
    dbg1("%16s=0x%08X\n", "SEQ_3", stmotors[motor_id].stmotor_regs->SEQ_3);
    dbg1("%16s=0x%08X\n", "SEQ_4", stmotors[motor_id].stmotor_regs->SEQ_4);
    dbg1("%16s=0x%08X\n", "SEQ_5", stmotors[motor_id].stmotor_regs->SEQ_5);
    dbg1("%16s=0x%08X\n", "OUT_MODE", stmotors[motor_id].stmotor_regs->OUT_MODE);
    dbg1("%16s=0x%08X\n", "STEP_INT", stmotors[motor_id].stmotor_regs->STEP_INT);
    dbg1("%16s=0x%08X\n", "PWM_T", stmotors[motor_id].stmotor_regs->PWM_T);
    dbg1("%16s=0x%08X\n", "PWM_P", stmotors[motor_id].stmotor_regs->PWM_P);
    dbg1("%16s=0x%08X\n", "ACC_INCR", stmotors[motor_id].stmotor_regs->ACC_INCR);
    dbg1("%16s=0x%08X\n", "PWM_M", stmotors[motor_id].stmotor_regs->PWM_M);
    dbg1("%16s=0x%08X\n", "CMD", stmotors[motor_id].stmotor_regs->CMD);
    dbg1("%16s=0x%08X\n", "CMD_ISSUE", stmotors[motor_id].stmotor_regs->CMD_ISSUE);
    dbg1("%16s=0x%08X\n", "COM_STAT", stmotors[motor_id].stmotor_regs->COM_STAT);
    dbg1("%16s=0x%08X\n", "COM_PWM_P", stmotors[motor_id].stmotor_regs->COM_PWM_P);
    dbg1("%16s=0x%08X\n", "COM_ACC_INCR", stmotors[motor_id].stmotor_regs->COM_ACC_INCR);
    dbg1("%16s=0x%08X\n", "COM_PWM_M", stmotors[motor_id].stmotor_regs->COM_PWM_M);
    dbg1("%16s=0x%08X\n", "COM_CMD", stmotors[motor_id].stmotor_regs->COM_CMD);
    dbg1("%16s=0x%08X\n", "COM_STP", stmotors[motor_id].stmotor_regs->COM_STP);
    dbg1("%16s=0x%08X\n", "COM_TVT", stmotors[motor_id].stmotor_regs->COM_TVT);
    dbg1("%16s=0x%08X\n", "COM_SEQ", stmotors[motor_id].stmotor_regs->COM_SEQ);
    dbg1("%16s=0x%08X\n", "I_EN", stmotors[motor_id].stmotor_regs->I_EN);
    dbg1("%16s=0x%08X\n", "I_PEND", stmotors[motor_id].stmotor_regs->I_PEND);
    dbg1("%16s=0x%08X\n", "I_ACK", stmotors[motor_id].stmotor_regs->I_ACK);
    dbg1("%16s=0x%08X\n", "I_FORCE", stmotors[motor_id].stmotor_regs->I_FORCE);
    dbg1("%16s=0x%08X\n", "SMC_TRIG", stmotors[motor_id].stmotor_regs->SMC_TRIG);
}


/**
 * \brief Set Stepper Motor location
 *
 **/
//MRVL-Harris for stepper cdma start
void smot_step_set_location(stmotor_id_t motor_id, int location)
{
    if (!smot_step_motor_id_check(motor_id))
    {
        step_func_error("invalid motor id [%d]\n", motor_id);
        ASSERT(0);
        return;
    }

    stmotors[motor_id].location_access++;
    stmotors[motor_id].location = location;
    step_func_infor("stmotors[%d].location was set to %d\n", motor_id, stmotors[motor_id].location);
}
//MRVL-Harris for stepper cdma end


/**
 * \brief Get Stepper Motor location
 *
 **/
//MRVL-Harris for stepper cdma start
int smot_step_get_location(stmotor_id_t motor_id)
{
    uint32_t stepcnt;

    if (!smot_step_motor_id_check(motor_id))
    {
        step_func_error("invalid motor id [%d]\n", motor_id);
        ASSERT(0);
        return 0;
    }

    stepcnt = smot_step_get_step_count(motor_id);
    //step_func_infor("stmotors[%d].location was %d\n", motor_id, stmotors[motor_id].location);
    return (stmotors[motor_id].location);
}
//MRVL-Harris for stepper cdma end


/**
 * \brief Update Stepper Motor location
 *
 **/
//MRVL-Harris for stepper cdma start
void smot_step_update_location(stmotor_id_t motor_id)
{
    uint32_t stepcnt;

    if (!smot_step_motor_id_check(motor_id))
    {
        step_func_error("invalid motor id [%d]\n", motor_id);
        ASSERT(0);
        return;
    }
    
    stepcnt = smot_step_get_step_count(motor_id);
    if (smot_step_debug_in_interrupt_enable)
    {
        step_func_infor("stmotors[%d].location was update to %d\n", motor_id, stmotors[motor_id].location);
    }
    smot_step_count_reset(motor_id, true);
    smot_step_count_reset(motor_id, false);
}
//MRVL-Harris for stepper cdma end


//MRVL-Harris-20120329 for step loss {
/**
 *  \brief Motor set to idle or leave from idle
 *
 *  \param[in] stmotor_id    Client Motor ID
 *  \param[in] state         true  means copy seq to idle
 *                           false means write seq to idle
 *  \parma[in] seq           for state is false
 **/
void smot_step_set_idle(stmotor_id_t motor_id, bool state, uint32_t seq)
{
    uint32_t   ptr_seq;
    uint32_t   idle_smc;
    uint32_t   mask;

    if (state == true)
    {
        ptr_seq = stmotors[motor_id].stmotor_regs->COM_SEQ & 0x1F;
        mask = 1 << ptr_seq;
        idle_smc = 0;
        if (stmotors[motor_id].stmotor_regs->SEQ_0 & mask)
        {
            idle_smc += STMOTOR_STEP_EN_IDLE_SMC_0(true);
        }
        if (stmotors[motor_id].stmotor_regs->SEQ_1 & mask)
        {
            idle_smc += STMOTOR_STEP_EN_IDLE_SMC_1(true);
        }
        if (stmotors[motor_id].stmotor_regs->SEQ_2 & mask)
        {
            idle_smc += STMOTOR_STEP_EN_IDLE_SMC_2(true);
        }
        if (stmotors[motor_id].stmotor_regs->SEQ_3 & mask)
        {
            idle_smc += STMOTOR_STEP_EN_IDLE_SMC_3(true);
        }
        if (stmotors[motor_id].stmotor_regs->SEQ_4 & mask)
        {
            idle_smc += STMOTOR_STEP_EN_IDLE_SMC_4(true);
        }
        if (stmotors[motor_id].stmotor_regs->SEQ_5 & mask)
        {
            idle_smc += STMOTOR_STEP_EN_IDLE_SMC_5(true);
        }
        stmotors[motor_id].stmotor_regs->STEP_EN_IDLE = idle_smc & 0x3F;
    }
    else
    {
        stmotors[motor_id].stmotor_regs->STEP_EN_IDLE = seq & 0x3F;
    }
}
//MRVL-Harris-20120329 for step loss }


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
 *  \return bool
 *  \retval true     Motor is in the idle (stopped) state
 *  \retval false    Motor is not idle
 **/
//MRVL-Harris for stepper cdma start
bool smot_step_motor_is_idle(stmotor_id_t motor_id)
{
    if ((stmotors[motor_id].stmotor_cur_state == STMOTOR_STOP) && (STMOTOR_REG_SMC_CFG_STEP_EN_MASK_SHIFT(stmotors[motor_id].stmotor_regs->SMC_CFG)!=true))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//MRVL-Harris for stepper cdma end


/**
 *  \brief (API) Initialize Stepper motor code
 *
 *  API function to initialize the Stepper motor code, must be called once at system
 *  startup before any other motor routines can be used.
 *
 *  \param[in] num_motors  Number of motors to support
 *
 **/
bool smot_step_init(uint8_t num_motors)
{
#ifdef HAVE_DBG_CALLBACK_CMDS
    int retcode;
#endif

    if (stmotors != NULL)
    {
        step_func_error("motor has already been initialized\n");
        ASSERT(0);
        return true;
    }

    stmotors = (stmotor_params_t *)kmalloc(sizeof(stmotor_params_t) * num_motors, GFP_KERNEL);
    if (stmotors == NULL)
    {
        step_func_error("memory allocation failed\n");
        ASSERT(0);
        return true;
    }
    memset(stmotors, 0, (sizeof(stmotor_params_t)*num_motors));
    st_max_motors = num_motors;

#ifdef HAVE_DBG_CALLBACK_CMDS
    retcode = cmd_register_cmd("step",NULL, NULL, NULL, NULL, smot_step_debug_callback);
    XASSERT(retcode==0, retcode);
#endif
    return false;
}


/**
 *  \brief (API) Set up a new motor
 *
 *  API function to create a new motor instance, usually called by the mech code
 *  during system startup.  This routine must only be called once for each
 *  physical motor to be supported by the motor driver.
 *
 **/
stmotor_id_t smot_step_create_motor(dpi_profile_rec*         all_dpi_profile,
                                    const stmotor_connect_t *stmotor_connects,
                                    uint32_t*                stmotor_sequence,
                                    uint32_t                 ramp_table_length,
                                    uint32_t                 stmotor_max_command_steps,
                                    bool                     (*sensor_check_func)(void))
{
    stmotor_id_t motor_id = INVALID_STEP_MOTOR_ID;
    dpi_profile_rec *pp;
    bool enable_dump_step_create_motor = false;
    uint8_t i;
    int retcode;

    if (st_max_motors == -1)
    {
        step_func_error("please call smot_step_init first\n");
        ASSERT(0);
        return INVALID_STEP_MOTOR_ID;
    }

    if (stmotors_created >= st_max_motors)
    {
        step_func_error("no more available motor ID\n");
        ASSERT(0);
        return INVALID_STEP_MOTOR_ID;
    }

    motor_id = stmotors_created;
    stmotors_created++;

    step_func_debug("motor[%d] all_dpi_profile=%p\n", motor_id, (void*)all_dpi_profile);
    step_func_debug("motor[%d] stmotor_connects=%p\n", motor_id, (void*)stmotor_connects);
    step_func_debug("motor[%d] stmotor_sequence=%p\n", motor_id, (void*)stmotor_sequence);
    step_func_debug("motor[%d] ramp_table_length=%d\n", motor_id, ramp_table_length);
    step_func_debug("motor[%d] stmotor_max_command_steps=%d\n", motor_id, stmotor_max_command_steps);
    step_func_debug("motor[%d] sensor_check_func=%p\n", motor_id, (void*)sensor_check_func);

    stmotors[motor_id].stmotor_connects = stmotor_connects;
    smot_step_set_blk_addr_irq(motor_id);
    step_func_infor("ID=%d ADDR=0x%p IRQ=%d CDMA=%d\n", motor_id, 
                                                             stmotors[motor_id].stmotor_regs,
                                                             stmotors[motor_id].stmotor_irq_num,
                                                             stmotors[motor_id].stmotor_cdma_num);
#ifndef __KERNEL__
    stmotors[motor_id].stmotor_dma_chan = 0;
#endif

    //block reset
    smot_step_block_enable(motor_id, false); //stepping disable
    smot_step_soft_reset(motor_id, true); //soft reset
    smot_step_count_reset(motor_id, true); //count reset
    smot_step_command_abort(motor_id, true); //command abort
    smot_step_command_abort(motor_id, false);
    smot_step_soft_reset(motor_id, false);
    smot_step_count_reset(motor_id, false);
    //smot_step_block_enable(motor_id, true); //MRVL-Harris-20120329 for step loss
    //smot_step_set_idle(motor_id, true, 0); //MRVL-Harris-20120329 for step loss
    //smot_step_block_enable(motor_id, false); //MRVL-Harris-20120329 for step loss

    //interrupt config
    retcode = request_irq(stmotors[motor_id].stmotor_irq_num,
                          smot_step_motor_interrupt,
                          0,
                          motor_irq_name[motor_id],
                          (void*)motor_id);
    step_func_infor("request_irq retcode=%d\n", retcode);
    smot_step_int_disable(motor_id);

    //output mode config
    smot_step_set_out_mode(motor_id);

    //internal location config
    smot_step_set_location(motor_id, -1);
    stmotors[motor_id].location_access = 0;
    stmotors[motor_id].cur_stepcnt = 0;
    stmotors[motor_id].pre_stepcnt = 0;
    stmotors[motor_id].stop_requested = false;

    //load 300dpi mono as default profile
    stmotors[motor_id].all_dpi_profile = all_dpi_profile;
    stmotors[motor_id].stmotor_sequence = stmotor_sequence;
    pp = &(stmotors[motor_id].all_dpi_profile[DPI300M]);
    stmotors[motor_id].cur_dpi_profile = pp;
    if (enable_dump_step_create_motor)
    {
        dbg1("%s=%08d\n", "pwm_p", pp->pwm_p);
        dbg1("%s=%08d\n", "pwm_m_start", pp->pwm_m_start);
        dbg1("%s=%08d\n", "pwm_m_target", pp->pwm_m_target);
        dbg1("%s=%08d\n", "preholding_steps", pp->preholding_steps);
        dbg1("%s=%08d\n", "postholding_steps", pp->postholding_steps);
        dbg1("%s=%08d\n", "acc_incr", pp->acc_incr);
        dbg1("%s=\n", "tvt_table");
        for (i=0; i<9; i++)
        {
            dbg1("%08d, ", pp->tvt_table[i]);
            if ( (i%4) == 3) dbg1("\n");
        }
        dbg1("\n");
        dbg1("%s=\n", "accel_table");
        for (i=0; i<ramp_table_length; i++)
        {
            dbg1("%08d, ", pp->accel_table[i]);
            if ( (i%4) == 3) dbg1("\n");
        }
        dbg1("\n");
        dbg1("%s=\n", "seq_table");
        for (i=0; i<6; i++)
        {
            dbg1("0x%08x\n", stmotors[motor_id].stmotor_sequence[i]);
        }
    }

    smot_step_set_torque_vector_table(motor_id);
    smot_step_set_sequence(motor_id);
    smot_step_set_pwm_m(motor_id, pp->pwm_m_start);
    smot_step_set_pwm_p(motor_id, pp->pwm_p);
    smot_step_set_acc_incr(motor_id);
    stmotors[motor_id].stmotor_regs->NUM_USTEPS = STMOTOR_REG_NUM_USTEPS_NUM_REPLACE_VAL(stmotors[motor_id].stmotor_regs->NUM_USTEPS, 8);
    smot_step_set_line_start(motor_id, 1, 1175); //300DPI

    //set default variables
    stmotors[motor_id].stmotor_cur_state = STMOTOR_STOP;
    stmotors[motor_id].stmotor_pre_state = STMOTOR_STOP;
    stmotors[motor_id].target_steps = 0;
    stmotors[motor_id].pending_steps = 0;
    stmotors[motor_id].max_command_steps = stmotor_max_command_steps; //MRVL-Harris-20111223
    stmotors[motor_id].direction = MOVE_FORWARD;
    stmotors[motor_id].ramp_step_limit = ramp_table_length * stmotors[motor_id].max_command_steps; //MRVL-Harris-20111223
    stmotors[motor_id].ramp_up_steps = 0;
    stmotors[motor_id].ramp_down_steps = 0;
    stmotors[motor_id].ls_en = false;
    stmotors[motor_id].preholding_steps = pp->preholding_steps;
    stmotors[motor_id].postholding_steps = pp->postholding_steps;

    smot_step_block_enable(motor_id, true); //MRVL-Harris-20120329 for step loss
    #ifdef HAVE_PINETREE_FORM_FACTOR
    smot_step_set_idle(motor_id, false, 0x3F); //MRVL-Harris-20120329 for step loss
    #else
    smot_step_set_idle(motor_id, false, 0); //MRVL-Harris-20120329 for step loss
    #endif
    smot_step_block_enable(motor_id, false); //MRVL-Harris-20120329 for step loss
    smot_step_register_sensor_check_fcn(motor_id, sensor_check_func);
#ifdef __KERNEL__
    sema_init(&stmotors[motor_id].sg_sem, 1);
#endif

    if (enable_dump_step_create_motor)
    {
        smot_step_dump(motor_id);
    }

    return motor_id;
}


/**
 *  \brief (API) Initiate 'gentle' motor stop
 *
 **/
void smot_step_request_motor_stop(stmotor_id_t  motor_id)
//MRVL-Harris for stepper cdma start
{
    uint32_t  regtemp;
    stmotors[motor_id].stop_requested = true;
    regtemp = stmotors[motor_id].stmotor_regs->COM_CMD;
    if (stmotors[motor_id].stmotor_cur_state == STMOTOR_AT_SPEED)
    {
        smot_step_command_abort(motor_id, true);
        if ((stmotors[motor_id].ls_en) && !(regtemp & STMOTOR_REG_COM_CMD_LS_EN_MASK))
        {
            smot_step_command_abort(motor_id, true);
        }
    }
}
//MRVL-Harris for stepper cdma end


/**
 *  \brief (API) Send an emergency halt command to a stepper motor
 *
 **/
void smot_step_emergency_halt(stmotor_id_t  motor_id)
{
    smot_step_block_enable(motor_id, true);
    stmotors[motor_id].stmotor_cur_state = STMOTOR_INVALID;
    stmotors[motor_id].stmotor_pre_state = STMOTOR_INVALID;
}


/**
 *  \brief (API) Move the stepper motor relative to the current position
 *
 **/
void smot_step_move_rel(stmotor_id_t  motor_id,
                        uint32_t      motor_dpi,
                        uint32_t      move_steps,
                        uint8_t       move_direction,
                        bool          enable_sp,                  
                        uint32_t      enable_sp_at_stepnum)                         
{
	if(SC_halt_flg)
		return;  //don't move the motor after SC hale.  mod.kks 2014.06.02

    dpi_profile_rec *pp;
    uint32_t         temp;

    step_func_enter();
		
    if (!smot_step_motor_id_check(motor_id))
    {
        /* davep 05-Dec-2012 ;  */
        ASSERT(0);
        step_func_error("invalid motor id [%d]\n", motor_id);
        ASSERT(0);
        return;
    }

#ifdef Phase_12    
    if(motor_id == 0){  //1-2 Phase
		scanplat_kernel_gpio_set_value(pinetree_mech.phase_mode_select1, 1);
		scanplat_kernel_gpio_set_value(pinetree_mech.phase_mode_select2, 0);
	}
	else{ // W1-2 Phase
		scanplat_kernel_gpio_set_value(pinetree_mech.phase_mode_select1, 0);
		scanplat_kernel_gpio_set_value(pinetree_mech.phase_mode_select2, 1);
	}
#endif
    if (0 == move_steps) {

        /* davep 24-Sep-2013 ; XXX temp debug */
        ASSERT(0);

        return;
    }

    ASSERT(stmotors);
    ASSERT(&stmotors[motor_id]);
//    ASSERT(stmotors[motor_id].all_dpi_profile);
//    ASSERT(stmotors[motor_id].all_dpi_profile[motor_dpi]);
    pp = &(stmotors[motor_id].all_dpi_profile[motor_dpi]);
    XASSERT(pp,motor_id);

    stmotors[motor_id].cur_dpi_profile = pp;
    stmotors[motor_id].stmotor_cur_state = STMOTOR_STOP;
    stmotors[motor_id].stmotor_pre_state = STMOTOR_STOP;
    stmotors[motor_id].target_steps = move_steps;
    stmotors[motor_id].pending_steps = move_steps;
    stmotors[motor_id].direction = move_direction;
    {
        if (move_steps < (2*stmotors[motor_id].ramp_step_limit))  //Short distance Case.
        {
            if (move_steps <= (4*2*stmotors[motor_id].max_command_steps))
            {
                stmotors[motor_id].ramp_up_steps = 0;
                stmotors[motor_id].ramp_down_steps = move_steps;
            }
            else
            {
                if (stmotors[motor_id].max_command_steps == 0)
                {
                    step_func_error("stmotors[%d].max_command_steps == 0\n", motor_id);
                    ASSERT(0);
                }
                //ramping_up needs 2 cmds at least
                //ramping_down needs 2 cmds at least
                //min steps = 4 x 2 = 8 max_command_steps
                temp = (move_steps / 4) / stmotors[motor_id].max_command_steps;
                temp = temp * stmotors[motor_id].max_command_steps;
                stmotors[motor_id].ramp_up_steps = temp;                
				stmotors[motor_id].ramp_down_steps = move_steps - stmotors[motor_id].ramp_up_steps;
				reduce_control  = 0;
            }
        }
        else //Normal Case.
        {
            stmotors[motor_id].ramp_up_steps = stmotors[motor_id].ramp_step_limit;
            reduce_control  = 1;
            //stmotors[motor_id].ramp_down_steps = move_steps - stmotors[motor_id].ramp_step_limit;
                if(motor_id != 0)  {// ADF 
					stmotors[motor_id].ramp_down_steps = move_steps - ADF_DECEL_CONTROL_STEP;
				}
				else{ //FB
					stmotors[motor_id].ramp_down_steps = move_steps - FB_DECEL_CONTROL_STEP ;  //Mod.kks
					stmotors[motor_id].pending_steps  = 0;
				}
        }
    }
    if (enable_sp)
    {
        stmotors[motor_id].ls_start_pos = enable_sp_at_stepnum;
        stmotors[motor_id].ls_en = true;
    }
    else
    {
        stmotors[motor_id].ls_start_pos = -1; 
        stmotors[motor_id].ls_en = false;
    }

#if 0
    dbg1("stmotors[%d].ramp_step_limit=%d\n", motor_id, stmotors[motor_id].ramp_step_limit);
    dbg1("stmotors[%d].ramp_up_steps=%d\n", motor_id, stmotors[motor_id].ramp_up_steps);
    dbg1("stmotors[%d].ramp_down_steps=%d\n", motor_id, stmotors[motor_id].ramp_down_steps);
    dbg1("stmotors[%d].ls_start_pos=%d\n", motor_id, stmotors[motor_id].ls_start_pos);
    dbg1("stmotors[%d].ls_en=%d\n", motor_id, stmotors[motor_id].ls_en);
    dbg1("Start motor %d  Current POS %d\n", motor_id, smot_step_get_location(motor_id));
    dbg1("motors[%d].target_steps=%d\n", motor_id, stmotors[motor_id].target_steps);

#else  // Motor debug for Test  Mod.kks
    step_func_error("stmotors[%d].target_steps=%d\n", motor_id, stmotors[motor_id].target_steps);
    step_func_error("stmotors[%d].ramp_up_steps=%d\n", motor_id, stmotors[motor_id].ramp_up_steps);
    step_func_error("stmotors[%d].ramp_down_steps=%d\n", motor_id, stmotors[motor_id].ramp_down_steps);
    step_func_error("stmotors[%d].kks_down_steps =%d\n",  motor_id, stmotors[motor_id].target_steps-stmotors[motor_id].ramp_down_steps);
#endif
    
    stmotors[motor_id].preholding_steps = pp->preholding_steps;
    stmotors[motor_id].postholding_steps = 0; //pp->postholding_steps;
    if (move_direction == MOVE_FORWARD)
    {
        #ifdef HAVE_PINETREE_FORM_FACTOR
        smot_step_set_reg_bit0(motor_id, true);
        smot_step_set_reg_bit1(motor_id, false);
        #else
        smot_step_set_reg_bit0(motor_id, false);
        smot_step_set_reg_bit1(motor_id, true);
        #endif
    }
    else //Backward 
    {
        #ifdef HAVE_PINETREE_FORM_FACTOR
        smot_step_set_reg_bit0(motor_id, false);
        smot_step_set_reg_bit1(motor_id, true);
        #else
        smot_step_set_reg_bit0(motor_id, true);
        smot_step_set_reg_bit1(motor_id, false);
        #endif
    }
    smot_step_run(motor_id);

}


/**
 *  \brief (API) Move the stepper motor to an absolute position
 *
 **/
void smot_step_move_abs(stmotor_id_t  motor_id,
                        uint32_t      motor_dpi,
                        int           tar_pos_steps,
                        bool          enable_sp,
                        uint32_t      enable_sp_at_stepnum)
{
    int       cur_pos_steps;
    uint32_t  move_steps;
    uint32_t  move_direction;

    if (!smot_step_motor_id_check(motor_id))
    {
        step_func_error("invalid motor id [%d]\n", motor_id);
        ASSERT(0);
        return;
    }

    cur_pos_steps = smot_step_get_location(motor_id);

    /* Using the current absolute position and the desired destination position,
     * calculate a relative move
     */
    if (cur_pos_steps > tar_pos_steps)
    {
        move_direction = MOVE_REVERSE;
        move_steps = cur_pos_steps - tar_pos_steps;
    }
    else
    {
        move_direction = MOVE_FORWARD;
        move_steps = tar_pos_steps - cur_pos_steps;
    }

    smot_step_move_rel(motor_id,
                       motor_dpi,
                       move_steps,
                       move_direction,
                       enable_sp,
                       enable_sp_at_stepnum);
}



#ifdef HAVE_DBG_CALLBACK_CMDS
static void smot_step_show_help_move(uint32_t level)
{
    step_func_temp1("move: send a move command to stepper\n");    
    if (level != 0)
    {
        step_func_temp1(" Usage: step move <id> <mode> <dpi> <steps> <dir>\n");   
        step_func_temp1("  id: motor id\n");
        step_func_temp1("  mode: color or mono\n");
        step_func_temp1("  dpi: 150, 300, 600 or 12000\n");
        step_func_temp1("  steps: step number for movement\n");
        step_func_temp1("  dir: 1 = forward, 0 = reverse\n");
    }
}


static void smot_step_show_help_movestop(uint32_t level)
{
    step_func_temp1("movestop: send a movestop command to stepper\n");    
    if (level != 0)
    {
        step_func_temp1(" Usage: step movestop <id> <mode> <dpi> <steps> <dir> <lspos> <stop>\n");   
        step_func_temp1("  id: motor id\n");
        step_func_temp1("  mode: color or mono\n");
        step_func_temp1("  dpi: 150, 300, 600 or 12000\n");
        step_func_temp1("  steps: step number for movement\n");
        step_func_temp1("  dir: 1 = forward, 0 = reverse\n");
        step_func_temp1("  lspos: line start position, 0 means no ls\n");
        step_func_temp1("  stop: stop step number\n");
    }
}


static void smot_step_show_help_movestop2(uint32_t level)
{
    step_func_temp1("movestop2: send a movestop command to 2 steppers\n");    
    if (level != 0)
    {
        step_func_temp1(" Usage: step movestop <id1> <id2> <mode> <dpi> <steps> <dir> <stop1> (stop2>\n");   
        step_func_temp1("  id1: motor 1 id\n");
        step_func_temp1("  id2: motor 2 id\n");
        step_func_temp1("  mode: color or mono\n");
        step_func_temp1("  dpi: 150, 300, 600 or 12000\n");
        step_func_temp1("  steps: step number for movement\n");
        step_func_temp1("  dir: 1 = forward, 0 = reverse\n");
        step_func_temp1("  stop1: stop step number for motor 1\n");
        step_func_temp1("  stop2: stop step number for motor 2\n");

    }
}



static void smot_step_show_help_absmove(uint32_t level)
{
    step_func_temp1("absmove: move stepper to an absolute position\n");    
    if (level != 0)
    {
        step_func_temp1(" Usage: step absmove <id> <mode> <dpi> <pos>\n");   
        step_func_temp1("  id: motor id\n");
        step_func_temp1("  mode: color or mono\n");
        step_func_temp1("  dpi: 150, 300, 600 or 12000\n");
        step_func_temp1("  pos: target absolute position\n");
    }
}

static void smot_step_show_help_dump(uint32_t level)
{
    step_func_temp1("dump: dump step registers\n");    
    if (level != 0)
    {
        step_func_temp1(" Usage: step dump\n");   
        step_func_temp1("  id: motor id\n");
    }
}


static void smot_step_show_help_getpos(uint32_t level)
{
    step_func_temp1("getpos: get current position\n");    
    if (level != 0)
    {
        step_func_temp1(" Usage: step getpos <id>\n");   
        step_func_temp1("  id: motor id\n");
    }
}


static void smot_step_show_help(void)
{
    step_func_temp1("\nstep debug command list\n");
    smot_step_show_help_move(0);
    smot_step_show_help_movestop(0);
    smot_step_show_help_movestop2(0);
    smot_step_show_help_absmove(0);
    smot_step_show_help_dump(0);
    smot_step_show_help_getpos(0);
}


int smot_step_debug_callback(int argc, char *argv[])
{
    int i, pos;

    for (i=0 ; i<argc ; i++)
    {
        step_func_temp1("%s %d %s\n", __FUNCTION__, i, argv[i]);
    }

    if (argc < 2)
    {
        smot_step_show_help();
        return 0;
    }

    pos = 1;

    if (strncmp(argv[pos], "move", 5) == 0)
    {
        uint32_t     temp;
        stmotor_id_t motor_id;
        uint32_t     move_steps;
        uint8_t      move_dir;
        uint8_t      move_dpi;
        uint8_t      count = 0;
        uint32_t     stepcnt;

        if(argc != 7)
        {
            smot_step_show_help_move(1);
            return 0;
        }
        else
        {
            //get motor id
            str_mkint(argv[pos+1], &temp);
            motor_id = temp;

            //get scan mode and dpi
            if (strncmp(argv[pos+2], "color", 6) == 0)
            {
                str_mkint(argv[pos+3], &temp);
                if (temp == 150) {
                    move_dpi = DPI150C;
                } else if (temp == 300) {
                    move_dpi = DPI300C;
                } else if (temp == 600) {
                    move_dpi = DPI600C;
                } else if (temp == 1200) {
                    move_dpi = DPI1200C;
                } else {
                    smot_step_show_help_move(1);
                    return 0;
                }
            }
            else if (strncmp(argv[pos+2], "mono", 5) == 0)
            {
                str_mkint(argv[pos+3], &temp);
                if (temp == 150) {
                    move_dpi = DPI150M;
                } else if (temp == 300) {
                    move_dpi = DPI300M;
                } else if (temp == 600) {
                    move_dpi = DPI600M;
                } else if (temp == 1200) {
                    move_dpi = DPI1200M;
                } else {
                    smot_step_show_help_move(1);
                    return 0;
                }
            }
            else
            {
                smot_step_show_help_move(1);
                return 0;
            }

            //get move steps
            str_mkint(argv[pos+4], &temp);
            move_steps = temp;

            //get move direction
            str_mkint(argv[pos+5], &temp);
            move_dir = temp;

            //move
            smot_step_move_rel(motor_id, move_dpi, move_steps, move_dir, false, 1);
            while(1) 
            {
                stepcnt = smot_step_get_step_count(motor_id);
                step_func_temp1("stepcnt=%d\n", stepcnt);
                count++;
                if (count > 20)
                {
                    count = 0;
                    if (smot_step_motor_is_idle(motor_id))
                    {
                        break;
                    }
                }
            }
        }
    }
    else if (strncmp(argv[pos], "movestop", 9) == 0)
    {
        uint32_t     temp;
        stmotor_id_t motor_id;
        uint8_t      move_dir;
        uint8_t      move_dpi;
        uint32_t     move_steps;
        uint32_t     stop_steps;
        uint8_t      count = 0;
        uint32_t     stepcnt;
        uint32_t     lspos;
        
        bool         stop_enable = true;
        
        if(argc != 9)
        {
            smot_step_show_help_movestop(1);   
            return 0;
        }
        else
        {
            //get motor id
            str_mkint(argv[pos+1], &temp);
            motor_id = temp; 

            //get scan mode and dpi
            if (strncmp(argv[pos+2], "color", 6) == 0)
            {
                str_mkint(argv[pos+3], &temp);
                if (temp == 150) {
                    move_dpi = DPI150C;  
                } else if (temp == 300) {
                    move_dpi = DPI300C;
                } else if (temp == 600) {
                    move_dpi = DPI600C;
                } else if (temp == 1200) {
                    move_dpi = DPI1200C;
                } else {
                    smot_step_show_help_movestop(1);  
                    return 0;
                }
            }  
            else if (strncmp(argv[pos+2], "mono", 5) == 0)
            {
                str_mkint(argv[pos+3], &temp);
                if (temp == 150) {
                    move_dpi = DPI150M;  
                } else if (temp == 300) {
                    move_dpi = DPI300M;
                } else if (temp == 600) {
                    move_dpi = DPI600M;
                } else if (temp == 1200) {
                    move_dpi = DPI1200M;
                } else {
                    smot_step_show_help_movestop(1);  
                    return 0;
                }
            }  
            else
            {
                smot_step_show_help_movestop(1);  
                return 0;
            }

            //get move steps
            str_mkint(argv[pos+4], &temp);
            move_steps = temp;

            //get move direction
            str_mkint(argv[pos+5], &temp);
            move_dir = temp;

            //get stop steps
            str_mkint(argv[pos+6], &temp);
            lspos = temp;            

            //get stop steps
            str_mkint(argv[pos+7], &temp);
            stop_steps = temp;            

            //move
            if (lspos == 0)
            {
                smot_step_move_rel(motor_id, move_dpi, move_steps, move_dir, false, -1);
            }
            else
            {
                smot_step_move_rel(motor_id, move_dpi, move_steps, move_dir, true, lspos);
            }
            while(1) 
            {
                stepcnt = smot_step_get_step_count(motor_id);
                step_func_temp1("step=%d\n", stepcnt);
                if (stop_enable)
                {
                    if (stepcnt >= stop_steps)
                    {
                        smot_step_request_motor_stop(motor_id);
                        stop_enable = false;
                    }
                }
                
                count++;
                if (count > 20)
                {
                    count = 0;
                    if (smot_step_motor_is_idle(motor_id))
                    {
                        break;
                    }
                }
            }
        }
    }    

   else if (strncmp(argv[pos], "movestop2", 10) == 0)
    {
        uint32_t     temp;
        stmotor_id_t motor_id1;
        stmotor_id_t motor_id2;
        uint8_t      move_dir;
        uint8_t      move_dpi;
        uint32_t     move_steps;
        uint8_t      count = 0;
        uint32_t     stepcnt1;
        uint32_t     stepcnt2;
        uint32_t     stop_steps1;
        uint32_t     stop_steps2;
        bool         stop_enable1 = true;
        bool         stop_enable2 = true;
        
        if(argc != 10)
        {
            smot_step_show_help_movestop2(1);   
            return 0;
        }
        else
        {
            //get motor id
            str_mkint(argv[pos+1], &temp);
            motor_id1 = temp; 

            str_mkint(argv[pos+2], &temp);
            motor_id2 = temp; 

            //get scan mode and dpi
            if (strncmp(argv[pos+3], "color", 6) == 0)
            {
                str_mkint(argv[pos+4], &temp);
                if (temp == 150) {
                    move_dpi = DPI150C;  
                } else if (temp == 300) {
                    move_dpi = DPI300C;
                } else if (temp == 600) {
                    move_dpi = DPI600C;
                } else if (temp == 1200) {
                    move_dpi = DPI1200C;
                } else {
                    smot_step_show_help_movestop2(1);  
                    return 0;
                }
            }  
            else if (strncmp(argv[pos+3], "mono", 5) == 0)
            {
                str_mkint(argv[pos+4], &temp);
                if (temp == 150) {
                    move_dpi = DPI150M;  
                } else if (temp == 300) {
                    move_dpi = DPI300M;
                } else if (temp == 600) {
                    move_dpi = DPI600M;
                } else if (temp == 1200) {
                    move_dpi = DPI1200M;
                } else {
                    smot_step_show_help_movestop2(1);  
                    return 0;
                }
            }  
            else
            {
                smot_step_show_help_movestop2(1);  
                return 0;
            }

            //get move steps
            str_mkint(argv[pos+5], &temp);
            move_steps = temp;

            //get move direction
            str_mkint(argv[pos+6], &temp);
            move_dir = temp;

            //get stop steps
            str_mkint(argv[pos+7], &temp);
            stop_steps1 = temp;            
            str_mkint(argv[pos+8], &temp);
            stop_steps2 = temp;             

            //move
            smot_step_move_rel(motor_id1, move_dpi, move_steps, move_dir, false, 1);
            smot_step_move_rel(motor_id2, move_dpi, move_steps, move_dir, false, 1);
            while(1) 
            {
                stepcnt1 = smot_step_get_step_count(motor_id1);
                stepcnt2 = smot_step_get_step_count(motor_id2);
                step_func_temp1("step[1]=%d, step[2]=%d\n", stepcnt1, stepcnt2);
                if (stop_enable1)
                {
                    if (stepcnt1 >= stop_steps1)
                    {
                        smot_step_request_motor_stop(motor_id1);
                        stop_enable1 = false;
                    }
                }
                if (stop_enable2)
                {
                    if (stepcnt2 >= stop_steps2)
                    {
                        smot_step_request_motor_stop(motor_id2);
                        stop_enable2 = false;
                    }
                }
                
                count++;
                if (count > 20)
                {
                    count = 0;
                    if (smot_step_motor_is_idle(motor_id1)&&smot_step_motor_is_idle(motor_id2))
                    {
                        break;
                    }
                }
            }
        }
    }       
    else if (strncmp(argv[pos], "getpos", 7) == 0)
    {
        uint32_t             temp;
        stmotor_id_t         motor_id;
        int                  location;
        if(argc != 3)
        {
            smot_step_show_help_getpos(1);   
            return 0;
        }
        else
        {
            //get motor id
            str_mkint(argv[pos+1], &temp);
            motor_id = temp; 
        
            location = smot_step_get_location(motor_id);
            dbg1("location of motor %d = %d\n", motor_id, location);
        }
    }
    else if (strncmp(argv[pos], "dump", 5) == 0)
    {
        uint32_t             temp;
        stmotor_id_t         motor_id;
        
        if(argc != 3)
        {
            smot_step_show_help_dump(1);   
            return 0;
        }
        else
        {
            //get motor id
            str_mkint(argv[pos+1], &temp);
            motor_id = temp; 
        
            smot_step_dump(motor_id);
        }
    }
    else if (strncmp(argv[pos], "absmove", 8) == 0)
    {
        uint32_t            temp;
        stmotor_id_t         motor_id;
        uint8_t              move_dpi; 
        uint32_t             target;
        uint8_t              count = 0;

        if(argc != 6)
        {
            smot_step_show_help_absmove(1);   
            return 0;
        }
        else
        {
            //get motor id
            str_mkint(argv[pos+1], &temp);
            motor_id = temp; 

            //get scan mode and dpi
            if (strncmp(argv[pos+2], "color", 6) == 0)
            {
                str_mkint(argv[pos+3], &temp);
                if (temp == 150) {
                    move_dpi = DPI150C;  
                } else if (temp == 300) {
                    move_dpi = DPI300C;
                } else if (temp == 600) {
                    move_dpi = DPI600C;
                } else if (temp == 1200) {
                    move_dpi = DPI1200C;
                } else {
                    smot_step_show_help_absmove(1);  
                    return 0;
                }
            }  
            else if (strncmp(argv[pos+2], "mono", 5) == 0)
            {
                str_mkint(argv[pos+3], &temp);
                if (temp == 150) {
                    move_dpi = DPI150M;  
                } else if (temp == 300) {
                    move_dpi = DPI300M;
                } else if (temp == 600) {
                    move_dpi = DPI600M;
                } else if (temp == 1200) {
                    move_dpi = DPI1200M;
                } else {
                    smot_step_show_help_absmove(1);  
                    return 0;
                }
            }  
            else
            {
                smot_step_show_help_absmove(1);  
                return 0;
            }

            //get move steps
            str_mkint(argv[pos+4], &temp);
            target = temp;

            //move
            smot_step_move_abs(motor_id,
                               move_dpi,
                               target,
                               true, 
                               1);
            while(1) 
            {
                step_func_temp1("current location = %d\n", smot_step_get_location(motor_id));
                count++;
                if (count > 20)
                {
                    count = 0;
                    if (smot_step_motor_is_idle(motor_id))
                    {
                        break;
                    }
                }
            }
        }
    }
    else
    {
        smot_step_show_help();
    }
    return 0;
}
#endif

#if defined(__KERNEL__) && defined(SYSFS_CMD_DEBUG)
///////////////////////////////////////////////////////////
static ssize_t move_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    //struct net_device *netdev = dev_get_drvdata(dev);
    smot_step_dump(motors[test_motor]);

    return snprintf(buf, PAGE_SIZE, "%x\n", 0xdeadbeef);
}


static ssize_t move_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    int         motor_num = -1;
    int         distance = 0;
    int         direction = 0;
    motor_dir_t motor_dir;

    sscanf(buf,"%d %d %d",&motor_num,&distance,&direction);

    motor_num = ((motor_num < 0) || (motor_num >= stmotors_created)) ? -1 : motor_num;
    distance = ((distance < 0) || (distance > ((motor_num==0)?200:500))) ? 0 : distance;
    motor_dir = (direction == 0)?MOT_REVERSE:MOT_FORWARD;

    printk(KERN_ERR "\n%s: got motor: %d distance: %d direction: %s\n",
            __func__,(int)motor_num,distance,(motor_dir==MOT_REVERSE)?"REV":"FWD");

    switch(motor_num) {
        case 0:
            smot_step_move_rel(motor_num, DPI600M, distance*600, motor_dir, false, 0);
            break;
        case 1:
        case 2:
            smot_step_move_rel(motor_num, DPI600M, distance*192, motor_dir, false, 0);
            break;
        default:
            printk(KERN_ERR "Invalid motor number: %d\n", motor_num);
            break;
    }

    test_motor = motor_num;

    return count;
}

static ssize_t stmotor_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    return snprintf(buf, PAGE_SIZE, "%x\n", test_motor);
}


static ssize_t stmotor_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    int motor_num = (int)simple_strtol(buf, NULL, 0);

    motor_num = ((motor_num < 0) || (motor_num > 2)) ? -1 : motor_num;

    printk(KERN_ERR "\n%s: got motor num: %d to test\n", __func__,motor_num);

    switch(motor_num) {
        case 0:
            if (motors[motor_num] == INVALID_STEP_MOTOR_ID)
            {
                //create ADF motor
                motors[motor_num] = smot_step_create_motor(stmotor_adf_dpi_profiles,
                        &stmotor_adf_pin_connects,
                        stmotor_adf_sequence,
                        AD_NRAMP_VALS,
                        1024,
                        NULL);
            }

            printk(KERN_ERR "%x\n", motors[motor_num]);
            break;
        case 1:
            if (motors[motor_num] == INVALID_STEP_MOTOR_ID)
            {
                //create Flatbed motor
                motors[motor_num] = smot_step_create_motor(stmotor_fb_dpi_profiles,
                        &stmotor_fb_pin_connects,
                        stmotor_fb_sequence,
                        FB_NRAMP_VALS,
                        1024,
                        NULL);
            }
        case 2:
            if (motors[motor_num] == INVALID_STEP_MOTOR_ID)
            {
                //create Feed motor
                motors[motor_num] = smot_step_create_motor(stmotor_fd_dpi_profiles,
                        &stmotor_fd_pin_connects,
                        stmotor_fd_sequence,
                        FD_NRAMP_VALS,
                        1024,
                        NULL);
            }

            printk(KERN_ERR "%x\n", motors[motor_num]);
            break;

        default:
            printk(KERN_ERR "Invalid motor number: %d\n", motor_num);
            break;
    }

    test_motor = motor_num;

    return count;
}

DEVICE_ATTR(stmotor, (S_IRUGO|S_IWUSR), stmotor_show, stmotor_store);
DEVICE_ATTR(move, (S_IRUGO|S_IWUSR), move_show, move_store);
///////////////////////////////////////////////////////////

static int stepper_platform_probe(struct platform_device *pdev)
{
    int              retval = 0;
    struct resource	*res;
    int              irq;

    printk(KERN_ERR "\n\n\n%s: driver registration begun\n\n\n", __func__);

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res)
    {
        dev_err(&pdev->dev, "platform_get_resource failed\n");
        return -ENXIO;
    }

    irq = platform_get_irq(pdev, 0);
    if (irq < 0)
    {
        dev_err(&pdev->dev, "platform_get_irq failed\n");
        return -ENXIO;
    }

    if(( device_create_file(&pdev->dev,&dev_attr_move)) ||
       ( device_create_file(&pdev->dev,&dev_attr_stmotor)))
        printk(KERN_ERR DRIVER_NAME"%s:%d device_create_file failed.\n", __func__, __LINE__);

    return retval;
}


static int stepper_platform_remove(struct platform_device *pdev)
{
    printk(KERN_ERR "%s: driver platform remove begun\n", __func__);
	return (0);
}

static int stepper_platform_suspend(struct platform_device *pdev, pm_message_t state)
{
    printk(KERN_ERR "%s: driver platform suspend begun\n", __func__);
	return (0);
}

static int stepper_platform_resume(struct platform_device *pdev)
{
    printk(KERN_ERR "%s: driver platform resume begun\n", __func__);
    return(0);
}

static struct platform_driver stepper_platform_driver =
{
    .probe   = stepper_platform_probe,
    .remove  = stepper_platform_remove,
    .suspend = stepper_platform_suspend,
    .resume  = stepper_platform_resume,
    .driver  = {
        .name  = DRIVER_NAME,
        .owner = THIS_MODULE
    }
};

/**
 * stepper_platform_init: module init
 *
 * Driver load
 */
static int __init stepper_platform_init(void)
{
    int retval;

    printk(KERN_ERR "%s: driver platform init begun\n", __func__);

    retval = smot_step_init(STEPPER_NUM_MOTORS);
    if (retval)
    {
        printk(KERN_ERR "%s: error initializing driver\n", __func__);
    }
    else
    {
        retval = platform_driver_register(&stepper_platform_driver);
        if (retval)
        {
            //stepper_onetime_cleanup();
            printk(KERN_ERR "%s: error registering platform driver\n", __func__);
        }
        else
        {
            printk(KERN_ERR "stepper:  platform registration complete\n");
        }
    }
    return retval;
}
module_init(stepper_platform_init);

/**
 * stepper_platform_exit: module exit
 *
 * Driver unload
 */
static void __exit stepper_platform_exit(void)
{
    printk(KERN_ERR "%s: driver platform init begun\n", __func__);

    platform_driver_unregister(&stepper_platform_driver);
    //stepper_onetime_cleanup();
    printk(KERN_ERR "stepper:  platform remove complete\n");
}
module_exit(stepper_platform_exit);
EXPORT_SYMBOL(smot_step_create_motor);
EXPORT_SYMBOL(smot_step_move_rel);
EXPORT_SYMBOL(smot_step_move_abs);
EXPORT_SYMBOL(smot_step_request_motor_stop);
EXPORT_SYMBOL(smot_step_emergency_halt);
EXPORT_SYMBOL(smot_step_motor_is_idle);
EXPORT_SYMBOL(smot_step_set_idle);
EXPORT_SYMBOL(smot_step_block_enable);
EXPORT_SYMBOL(smot_step_set_location);
EXPORT_SYMBOL(smot_step_get_location);
EXPORT_SYMBOL(smot_step_set_line_start);

MODULE_AUTHOR("Copyright (c) 2012 Marvell International Ltd. All Rights Reserved");
MODULE_DESCRIPTION("Marvell Gogin series (mv[36]1x0) Stepper Driver");

MODULE_LICENSE("GPL");
MODULE_VERSION("2012_Jun_7");
#endif
