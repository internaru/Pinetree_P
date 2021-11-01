/**
 * ============================================================================
 * Copyright (c) 2006-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *   Pinetree mech driver.  This driver has rough support for two physical
 *   motors, but:
 *     - only one motor may run at a time
 *     - the mechType global must always indicate the running mech/motor
 *     - pause/resume is only supported on the flatbed
 *
 **/

#ifndef __KERNEL__
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "ATypes.h"
#include "memAPI.h"                    //MRVL-Harris-20120105 for notchfind
#include "tx_api.h"
#else
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/slab.h>
#endif

#include <linux/delay.h>
//#include "error_types.h"
#include "lassert.h"
#ifdef HAVE_DBG_CALLBACK_CMDS
    #include "scos_cmd_proc.h"
#endif
#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scanhwerr.h"
#include "scan.h"
#include "scanhw.h"
#include "scanmech.h"
#include "scancmdq.h"
#include "scanalyzer.h"
#include "scanif.h"
#include "scanvars.h"
#include "scanlib.h"
#include "debug.h"
#include "utils.h"
#include "scanmargin.h"
#include "scands.h"
#include "scanimg.h"                   //MRVL-Harris-20120105 for notchfind
#include "scancore.h"                  //MRVL-Harris-20120105 for notchfind
#include "scanman.h"                   //MRVL-Harris-20120105 for notchfind
#include "cal.h"                       //MRVL-Harris-20120119 for scan power-on calibration
#include "cal_common.h"                //MRVL-Harris-20120119 for scan power-on calibration
#include "adfpath.h"
#include "adfgapper.h"


#include "scanmot_pinetree.h"
#include "scanmech_pinetree.h"
#include "adfsensor.h"
#include "adfsensor_pinetree.h"

#ifndef __KERNEL__
#include "gpio_api.h"
#include "gpio_config_api.h"
#include "regAddrs.h"
#include "apb_config_regstructs.h"
#include "apb_config_regmasks.h"
#include "delay_api.h"
#else
#include <asm/gpio.h>
#include <linux/timer.h>
#include "asic/regAddrs.h"
#include "asic/apb_config_regstructs.h"
#include "asic/apb_config_regmasks.h"
#include "scanplat_sb.h"
#endif

#include "scos.h"
#include "scantask.h"
#include "ostools.h"
#include "scanmech_step_config.h"
#include "../../../../../common/debug/include/logger.h"

static stmotor_id_t FB_MOTOR  = INVALID_STEP_MOTOR_ID;
static stmotor_id_t ADF_MOTOR = INVALID_STEP_MOTOR_ID;
static stmotor_id_t FD_MOTOR  = INVALID_STEP_MOTOR_ID;

#define smech_func_enter()              //dbg1("SMECH@%s: enter\n", __FUNCTION__)
#define smech_func_leave()             //dbg1("SMECH@%s: leave\n", __FUNCTION__)
#define smech_func_infor(msg, args...)  //dbg1("SMECH@%s: " msg, __FUNCTION__, ##args)
#define smech_func_debug(msg, args...)  //dbg2("SMECH@%s: " msg, __FUNCTION__, ##args)
#define smech_func_error(msg, args...)  dbg1("SMECH@%s.%d: " msg, __FUNCTION__, __LINE__, ##args)
#define smech_func_helpu(msg, args...)  //dbg1(msg, ##args)

#define	ON			1
#define OFF			0
#define	HIGH		1
#define LOW			0
#define	HP_SUCCESS	1
#define	HP_FAIL		0

/*A3, DLT merge */
enum{A3_2ndSCAN_READY = 0,
			A3_EJECT,                       //1
			DLT_1stSCAN_READY,  //2
			DLT_2ndSCAN_READY, //3
			DLT_EJECT                       //4
			}; 

/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "SCN MECH_PT: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(7)
///Using Method.
#define DBG_PRINTF_SDH_ERR(...)		DBG_PRINTF(LOG_SDH_ERR, 		DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_SDH_DEBUG(...)	DBG_PRINTF(LOG_SDH_DEBUG,	DBG_PRFX __VA_ARGS__)


#define HAVE_DBG_CALLBACK_CMDS

/* SC 219 Motor Backward /  HomeSensor ON/ Home Sensor OFF marking*/
#if 0
#define TIMESTAMP
#else
#define TIMESTAMP { struct timeval my_time; \
    do_gettimeofday(&my_time); \
    printk("K %5ld:%6ld ", my_time.tv_sec, my_time.tv_usec);}
#endif

/*
 * platform margins
 *   (Tuned for 1.5mm top and left, using notch find home location)
 */
static struct scan_flatbed_margins g_flatbed_margins[] = {
    /* dpi, cmode, left_margin_hinches_x, top_margin_hinches_y, right_margin_hinches_x */  //1/100 inch mod.kks
    {  75,  SCAN_CMODE_MONO,   7,  54, 864, 1170},
    {  75,  SCAN_CMODE_COLOR,  7,  54, 864, 1170},    
    { 100,  SCAN_CMODE_MONO,   7,  54, 864, 1170},
    { 100,  SCAN_CMODE_COLOR,  7,  54, 864, 1170},    
    { 150,  SCAN_CMODE_MONO,   7,  54, 864, 1170},
    { 150,  SCAN_CMODE_COLOR,  7,  54, 864, 1170},
    { 200,  SCAN_CMODE_MONO,   7,  54, 864, 1170},
    { 200,  SCAN_CMODE_COLOR,  7,  54, 864, 1170},
    { 300,  SCAN_CMODE_MONO,   7,  54, 864, 1170},
    { 300,  SCAN_CMODE_COLOR,  7,  54, 864, 1170},
    { 600,  SCAN_CMODE_MONO,   7,  54, 864, 1170},
    { 600,  SCAN_CMODE_COLOR,  7,  54, 864, 1170},
    { 1200, SCAN_CMODE_MONO,   7,  54, 864, 1170},
    { 1200, SCAN_CMODE_COLOR,  7,  54, 864, 1170},
    { 2400, SCAN_CMODE_MONO,   7,  54, 864, 1170},
    { 2400, SCAN_CMODE_COLOR,  7,  54, 864, 1170},

    /* end of table marker */
    { 0, SCAN_CMODE_MONO, 0, 0 },
};


static struct scan_adf_margins g_adf_margins[] = {
    /* dpi, cmode,            first_tof, tof, bof, left_margin_hinches, rows_to_tof_backside, rows_to_bof_backside */
    {  .dpi=300,
       .cmode=SCAN_CMODE_MONO,
       .rows_to_first_tof=280,
       .rows_to_tof=247,		// 20 -> 247
//       .rows_to_bof=190,		// 30 -> 190 -> 210
       .rows_to_bof=210,		// 30 -> 190 -> 210
       .left_margin_hinches=0,
//       .rows_to_tof_backside=850,
       .rows_to_tof_backside=870,
    },
    {  .dpi=300,
       .cmode=SCAN_CMODE_COLOR,
       .rows_to_first_tof=280,
       .rows_to_tof=247,
//       .rows_to_bof=190,
       .rows_to_bof=210,
       .left_margin_hinches=0,
//       .rows_to_tof_backside=850,
       .rows_to_tof_backside=870,
    },
    {  .dpi=600,
       .cmode=SCAN_CMODE_MONO,
       .rows_to_first_tof=280,
       .rows_to_tof=495,		// 70 -> 495
//       .rows_to_bof=420,		// 300 -> 420
       .rows_to_bof=460,		// 300 -> 420
       .left_margin_hinches=0,
//       .rows_to_tof_backside=1700,  /* no, really */ 	// 1800 -> 1700
       .rows_to_tof_backside=1740,  /* no, really */ 	// 1800 -> 1700
    },
    {  .dpi=600,
       .cmode=SCAN_CMODE_COLOR,
       .rows_to_first_tof=280,
       .rows_to_tof=495,
//       .rows_to_bof=420,
       .rows_to_bof=460,       
       .left_margin_hinches=0,
//       .rows_to_tof_backside=1700,
       .rows_to_tof_backside=1740,
    },

    /* end of table marker */
    { .dpi=0, 
      .cmode=SCAN_CMODE_MONO
    },
};


#define FB_HOME_POSITION                0 // 500       //scanbar home position, unit: 0.001 inches
#define FB_TOP_HARDSTOP_POSITION         0         //hardstop position, unit: 0.001 inches
#define FB_BOT_HARDSTOP_POSITION         14000     //hardstop position, unit: 0.001 inches
#define FB_TOP_BED_POSITION              FB_CAL_POSITION //1482//1488      //start of flatbed glass, unit: 0.001 inches
#define FB_BOT_BED_POSITION              14000     //end of flatbed glass, unit: 0.001 inches
//#define FB_BOT_BED_POSITION              13250     //end of flatbed glass, unit: 0.001 inches
#define FB_CAL_POSITION                  940       //cal strip position
#define FB_STAGE_MIN_DELTA               2000      //2 inches (must exceed this to stage)
#define FB_STAGE_DIST                    500       //0.50 inch (actual staging distance)
#define FB_MAX_STEPS                     100000    //unit: steps
//#define FB_STEPS_PER_INCH               1198     //unit: 0.001 inches   1step : 0.02115425    // 25.4/0.021207294*2 =2395.408700....
#define FB_STEPS_PER_INCH               2395     //unit: 0.001 inches   1step : 0.02115425    // 25.4/0.021207294*2 =2395.408700....
#define FB_STEP_INCH_FACTOR              1000

#define ADF_HOME_POSITION                900       //unit: 0.001 inches
#define ADF_STAGE_DIST                   8000      //unit: 0.001 inches
//#define ADF_EMPTY_DIST                20000     //unit: 0.001 inches
#define ADF_EMPTY_DIST                  7874     //unit: 0.001 inchesr
#define ADF_SCAN_DIST                    250000    //unit: 0.001 inches
#define ADF_STEPS_PER_INCH               2400      //unit: 0.001 inches
#define ADF_STEP_INCH_FACTOR             1000

//#define SCANMECH_IOCTL_NOTCHFIND         (SCANMECH_IOCTL_USER_BASE-1) //MRVL-Harris-20120105 for notchfind
//#define SCANMECH_IOCTL_HARDSTOP          (SCANMECH_IOCTL_USER_BASE-2)

// Set these as appropriate for the customer mech
#define NOTCH_START_POSITION             2340      //start of notch //0.001 inches
#define NOTCHFIND_SCAN_RESOLUTION        600       //DPI
#define NOTCHFIND_SCAN_WIDTH             860       //unit: 0.01 inches
#define NOTCHFIND_SCAN_STARTPOS          150       //unit: 0.01 inches
#define NOTCHFIND_SCAN_LEN               1100       //unit: 0.01 inches
#define NOTCHFIND_SCANVAR_WHITE_CLIP     190
#define NOTCHFIND_SCANVAR_BLACK_CLIP     40
#define NOTCHFIND_SCANVAR_GAMMA          20

#define FB_MAX_CMD_STEPS                 1 //MRVL-Harris-20111223
#define AD_MAX_CMD_STEPS                 1 //MRVL-Harris-20111223
#define FD_MAX_CMD_STEPS                 1

#define FB_RAMP_STEPS                    FB_NRAMP_VALS * FB_MAX_CMD_STEPS
#define FB_REWIND_LS_START_POSITION      FB_RAMP_STEPS + 300
#define FB_REWIND_REVERSE_STEPS          FB_REWIND_LS_START_POSITION + FB_RAMP_STEPS

#define AD_LS_START_POSITION             AD_NRAMP_VALS * AD_MAX_CMD_STEPS + 100

static t_ScanMechType	mechType;
#ifndef __KERNEL__
static gpio_handle_t	*scan_home_sensor_gpio;
static gpio_handle_t	*scan_nmotor_driver_stby;
static gpio_handle_t	*scan_nmotor_driver_rst;
//static gpio_handle_t	*fb_enable;
#else
static struct pt_mech pinetree_mech;
#endif

static scan_target_type_t	current_scan_target_type;
static MOTOR_SYNC       smech_motor_sync;
static uint32_t         smech_start_pos;
static uint32_t         smech_profile_index;
static bool             smech_pause_rewind_enable;
static bool             smech_pagedone_homing_enable;
static bool             smech_scan_move_cal_enable; //MRVL-Harris-20120302 for scan moving calibration
static bool             smech_adf_clearpage_simplify;
static bool             smech_adf_inputpage_simplify;

bool  					ADF_mech_running=OFF;
static bool 			needToStopper=OFF;
static bool				feedmotor_forbid = OFF;
static bool				duplexflg=OFF;
static bool				PaperJAMFlg=OFF;
static bool				cancel_request_flg=OFF;
bool				SC_halt_flg = OFF;   //don't move the motor after SC halt.  mod.kks 2014.06.02

/* Home Position Detection */
static bool				hp_req = false;
static bool				hp_req_level = LOW;
static bool				hp_req_result = false;
static struct scos_sem 	hp_sem;	// add.lsh.for SC219
static bool				sleep_mode_state = OFF;  //need to the stable sensor state. mod.kks 2014.08.22


#ifndef __KERNEL__
static delay_msec_timer_t *smech_timer;
#else
static struct timer_list smech_timer;
#define MILLISEC(n) ((n * HZ)/1000)
#endif

static uint32_t			stmotor_profile_index;

static uint32_t scan_pixels_per_row = 0;
static uint32_t scan_expected_num_rows = 0;
static scan_err_t notch_find_results;

int toggle_val = 0;

extern int	cal_retry_flag;
extern bool cio_debounced_state; // Check the Top Cover debounce state. mod.kks 2014.06.13
extern bool ChangeTopCoverState;  // Check the Top Cover change state. mod.kks 2014.06.13

/*SP value in Scanner Service mode mod.kks 2014.06.12*/
bool 	SP_IN_check_flg = OFF;
bool 	SP_OUT_check_flg = OFF;
bool	SP_AdfFreeRunModeFlg=OFF;
static	bool SPdirection;
static	uint32_t    SPprofile_index;
static	uint32_t	SPsteps;
/*END */

/*
 * Internal function prototypes
 */
static bool isDocSourceADF(void);
static bool flatbed_sensor_home_is_found(void);
static stmotor_id_t get_motor_id_from_mech(t_ScanMechType mech_type);
static uint32_t get_stmotor_profile_index(scan_cmode_t cmode, uint32_t dpi);
static uint32_t inches_to_steps(t_ScanMechType mech_type, uint32_t inches);
static int steps_to_inches(t_ScanMechType mech_type, int steps);
static MOTOR_SEL get_stmotor_scif(t_ScanMechType mech_mtr);
static scan_err_t smech_wait_for_stop(stmotor_id_t motor_id);
static scan_err_t smech_move_to_position(int target, bool wait);
static	scan_err_t smech_stopper_arrange(void); //mod. kks
static scan_err_t smech_move_to_A3scanpoint(int direction, uint32_t distance); //mod. kks
static scan_err_t smech_move_to_calibration(void);
static scan_err_t smech_move_to_home(void);
static scan_err_t smech_move_to_HS_ON(void); //To return the Home Sensor ON state  mod.kks 2014.06.10
static scan_err_t smech_find_hardstop(void);
static scan_err_t smech_adf_input_paper(void);
static scan_err_t smech_adf_test_func(void);
static scan_err_t smech_adf_clear_paper(void);
static void smech_trig_scan_stopped(stmotor_id_t motor_id);
static void smech_trig_pause_rewind_stopped(stmotor_id_t motor_id);
static void smech_pause_rewind_stopped (stmotor_id_t motor_id);
static void smech_trig_paused_stopped(stmotor_id_t motor_id);
static void smech_paused_stopped(stmotor_id_t motor_id);
static void smech_fb_selftest(void);
static	scan_err_t smech_eject_paper(void); //mod. kks

static scan_err_t smech_notch_find_msg_data(scan_msg_t *msg);

//MRVL-Harris-20120119 for scan power-on calibration {
static scan_err_t smech_poweron_cal_run(struct scanvars **psv);
static scan_err_t smech_poweron_cal(scan_cmode_t cmode, int dpi);
//MRVL-Harris-20120119 for scan power-on calibration }

/*SP Function in Scanner Service mode mod.kks 2014.06.12*/
static void smech_feed_motor_run(int target);
static void smech_trans_motor_run(int target);
static void smech_value_To_speed(int target);
/*END */

bool bScanEC = false;

#ifdef HAVE_DBG_CALLBACK_CMDS
static int smech_pinetree_debug_callback(int argc, char *argv[]);
static t_ScanMechType debugMechType = SCAN_MECH_FLATBED;
#endif


/**
 *  \brief Ask if document source is ADF
 *
 *  The scan client chooses the document source and provides it in the scan vars, the
 *  mech code should honor this request whenever possible (situations like paper not
 *  present may make it impossible). This helper routine checks to see if the source
 *  is the ADF.
 *
 *  \return bool
 *  \retval true  Document source is ADF
 *  \retval false Document source is not ADF
 **/
bool isDocSourceADF(void)
{
    bool flag = false;
    const struct scanvars *sv = scanvar_get();

    XASSERT(sv != NULL, (uint32_t)sv);

    if (sv != NULL)
    {
        switch (sv->doc_src)
        {
			dbg1("Scan Source %d \n", sv->doc_src);
            case SCAN_DOCUMENT_SOURCE_FLATBED:
                flag = false; //from flatbed
                break;
            case SCAN_DOCUMENT_SOURCE_ADF:
				flag = true; //from ADF
			    duplexflg = OFF;
                break;
            case SCAN_DOCUMENT_SOURCE_ADF_NSENSOR:
                flag = true; //from ADF
                duplexflg = ON;
                break;
             default:
                smech_func_error("unknown document source %d\n", sv->doc_src);
                XASSERT(0, sv->doc_src);
                break;
        }
    }
    DBG_PRINTF_SDH_DEBUG("return %d[0=FB,1=ADF]\n", flag);
    return (flag);
}


/**
 *  \brief Is flatbed home found?
 *
 *  \return bool
 *  \retval true  Home is found
 *  \retval false Home is not found
 **/
bool flatbed_sensor_home_is_found(void)
{
    bool flag = false;
    uint32_t logic_level = 0xFFFF;

#ifndef __KERNEL__
    error_type_t err;

    err = gpio_read(scan_home_sensor_gpio, &logic_level);
    if (err != OK)
    {
        smech_func_error("gpio_read failed\n");
        return (false);
    }
#else
    logic_level = scanplat_kernel_gpio_get_value(pinetree_mech.scan_home_sensor_gpio);
#endif
    if (logic_level != 0x00)
    {
        flag = true; //home is found
    }
    else
    {
        flag = false; //home is not found
    }
    return (flag);

}


/**
 *  \brief Get the motor id from the mech type
 *
 *  Given the scan mech type, get the associated motor id.  This is
 *  the 'normal' way lookups should be done.
 **/
stmotor_id_t get_motor_id_from_mech(t_ScanMechType mech_type)
{
    stmotor_id_t motor_id = INVALID_STEP_MOTOR_ID;

    switch(mech_type)
    {
        case SCAN_MECH_FLATBED:
            motor_id = FB_MOTOR;
            break;
        case SCAN_MECH_ADF:
            motor_id = ADF_MOTOR;
            break;
        default:
            XASSERT(0, mech_type);
            break;
    }
    XASSERT(motor_id != INVALID_STEP_MOTOR_ID, motor_id);
//    DBG_PRINTF_SDH_DEBUG("return %d[%d=FB_MOTOR,%d=ADF_MOTOR]\n", motor_id, FB_MOTOR, ADF_MOTOR);
    return motor_id;
}


/**
 *  \brief Get a profile ID for stepper motor
 *
 **/
uint32_t get_stmotor_profile_index(scan_cmode_t cmode, uint32_t dpi)
{
    uint32_t index = DPI600C;

    smech_func_infor("cmode=%d[%d=MONO,%d=COLOR], dpi=%d\n", cmode, SCAN_CMODE_MONO, SCAN_CMODE_COLOR, dpi);

    if (cmode == SCAN_CMODE_COLOR)
    {
        if (dpi == 150) {
            index = DPI150C;
        } else if (dpi == 300) {
            index = DPI300C;
        } else if (dpi == 600) {
            index = DPI600C;
        } else if (dpi == 1200) {
            index = DPI1200C;
        } else {
            smech_func_error("unknown DPI in color mode\n");
            XASSERT(0, dpi);
        }
    }
    else if (cmode == SCAN_CMODE_MONO)
    {
        if (dpi == 150) {
            index = DPI150M;
        } else if (dpi == 300) {
            index = DPI300M;
        } else if (dpi == 600) {
            index = DPI600M;
        } else if (dpi == 1200) {
            index = DPI1200M;
        } else {
            smech_func_error("unknown DPI in mono mode\n");
            XASSERT(0, dpi);
        }
    }
    else
    {
        smech_func_error("unknown scan cmode\n");
        XASSERT(0, cmode);
    }
    stmotor_profile_index = index;
    return (index);
}


/**
 *  \brief Convert inches to steps
 *
 **/
uint32_t inches_to_steps(t_ScanMechType mech_type, uint32_t inches)
{
    uint32_t steps = 0;

    if (mech_type == SCAN_MECH_FLATBED)
    {
        steps = ((inches * FB_STEPS_PER_INCH) + (FB_STEP_INCH_FACTOR >> 1)) / FB_STEP_INCH_FACTOR;
    }
    else if (mech_type == SCAN_MECH_ADF)
    {
        steps = ((inches * ADF_STEPS_PER_INCH) + (ADF_STEP_INCH_FACTOR >> 1)) / ADF_STEP_INCH_FACTOR;
    }
    else
    {
        smech_func_error("unknown ScanMechType\n");
        XASSERT(0, mech_type);
    }
    return (steps);
}


/**
 *  \brief Convert steps to inches
 *
 **/
int steps_to_inches(t_ScanMechType mech_type, int steps)
{
    int inches = 0;

    if (mech_type == SCAN_MECH_FLATBED)
    {
        inches = ((steps * FB_STEP_INCH_FACTOR) + (FB_STEPS_PER_INCH >> 1)) / FB_STEPS_PER_INCH;        
    }
    else if (mech_type == SCAN_MECH_ADF)
    {
        inches = ((steps * ADF_STEP_INCH_FACTOR) + (ADF_STEPS_PER_INCH >> 1)) / ADF_STEPS_PER_INCH;
    }
    else
    {
        smech_func_error("unknown ScanMechType\n");
        XASSERT(0, mech_type);
    }
    return (inches);
}


/**
 *  \brief Set LS of stepper motor
 **/
void set_stmotor_ls_source(t_ScanMechType mech_mtr)
{
    stmotor_block_t block_num;
    volatile APB_CONFIG_REGS_t *apbconfig = (APB_CONFIG_REGS_t *)APB_CONFIG_BASE;

    switch(mech_mtr)
    {
        case SCAN_MECH_FLATBED:
            block_num = stmotor_fb_pin_connects.block_num;
            break;
        case SCAN_MECH_ADF:
            block_num = stmotor_adf_pin_connects.block_num;
            break;
        default:
            block_num = stmotor_fb_pin_connects.block_num;
            smech_func_error("unknown ScanMechType\n");
            XASSERT(0, mech_mtr);
            break;
    }

#ifdef CONFIG_ARCH_MV31X0
    switch(block_num)
    {
        case STEP_MTR_BLK_0:
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL1_REPLACE_VAL(apbconfig->PINCR, 0);
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL2_REPLACE_VAL(apbconfig->PINCR, 0);
            break;

        case STEP_MTR_BLK_1:
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL1_REPLACE_VAL(apbconfig->PINCR, 1);
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL2_REPLACE_VAL(apbconfig->PINCR, 0);
            break;


        case STEP_MTR_BLK_2:
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL1_REPLACE_VAL(apbconfig->PINCR, 0);
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL2_REPLACE_VAL(apbconfig->PINCR, 0);
            break;

        case STEP_MTR_BLK_3:
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL1_REPLACE_VAL(apbconfig->PINCR, 0);
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL2_REPLACE_VAL(apbconfig->PINCR, 1);
            break;

        default:
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL1_REPLACE_VAL(apbconfig->PINCR, 0);
            apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL2_REPLACE_VAL(apbconfig->PINCR, 0);
            smech_func_error("unknown motor block number\n");
            XASSERT(0, stmotor_fb_pin_connects.block_num);
            break;
    }
#elif CONFIG_ARCH_MV61X0
    apbconfig->PINCR = APB_CONFIG_PINCR_SCANRSYNCSEL_REPLACE_VAL(apbconfig->PINCR, block_num + 8); //MRVL-Harris-20121030
#else
#error No implementation of Scanner Sync Sel register code for this ASIC
#endif
}


/**
 *  \brief Get the SCIF motor number
 *
 *  Figure out the mapping between our motor block and the scif motor
 *  input: we need to do this to properly route the row sync signal
 *  between the blocks.
 **/
MOTOR_SEL get_stmotor_scif(t_ScanMechType mech_mtr)
{
    MOTOR_SEL       scif_mtr;
    stmotor_block_t block_num;

    switch(mech_mtr)
    {
        case SCAN_MECH_FLATBED:
            block_num = stmotor_fb_pin_connects.block_num;
            break;
        case SCAN_MECH_ADF:
            block_num = stmotor_adf_pin_connects.block_num;
            break;
        default:
            block_num = stmotor_fb_pin_connects.block_num;
            smech_func_error("unknown ScanMechType\n");
            XASSERT(0, mech_mtr);
            break;
    }

    switch(block_num)
    {
        case STEP_MTR_BLK_0:
        case STEP_MTR_BLK_1:
            scif_mtr = MOTOR1;
            break;
        case STEP_MTR_BLK_2:
        case STEP_MTR_BLK_3:
        /* davep 30-Aug-2012 ; add STEP_MTR_BLK_4 */
        case STEP_MTR_BLK_4:
            scif_mtr = MOTOR2;
            break;
        default:
            scif_mtr = MOTOR1;
            smech_func_error("unknown motor block number\n");
            XASSERT(0, stmotor_fb_pin_connects.block_num);
            break;
    }
    return scif_mtr;
}


/**
 *  \brief Block until motor move completes
 *
 *  In some cases the mech code will completely handle a motor move (no calls
 *  back out to scanlib).  In these cases the mech code must use this routine
 *  to block until the move is done.
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE   Move complete, no error
 *  \retval SCANERR_*      Move failure, failure specific error code
 **/
scan_err_t smech_wait_for_stop(stmotor_id_t motor_id)
{
    scan_err_t scerr = SCANERR_NONE;

    do
    {
	
#ifdef __KERNEL__
		mdelay(5);	// add.lsh.to apply 5ms sleep for while()		
#else
        {
            Uint       txretcode;
            txretcode = tx_thread_sleep(20);
            XASSERT(txretcode == TX_SUCCESS, txretcode);
        }
#endif
    } while (!smot_step_motor_is_idle(motor_id));

//    smech_func_infor("motor_id = %d\n", motor_id);
    return scerr;
}


/**
 * \brief  Move the scan bar to an absolute position
 *
 *  A flatbed move operation to position the scan bar at a specific position:
 *  the mech code will be in control of blocking until the move is complete.
 *
 *  \param[in] target     Absolute target position (in hundreds of an inch)
 *  \param[in] wait       Wait for move to complete
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE   Move complete, no error
 *  \retval SCANERR_*      Move failure,  specific error code
 **/
scan_err_t smech_move_to_position(int target, bool wait) 
{
    scan_err_t   scerr = SCANERR_NONE;
    int          previous;
    uint32_t     move_inches;
    uint8_t      move_direction;
    uint32_t     profile_index;

    smech_func_enter();


    //flatbed check
    if (mechType != SCAN_MECH_FLATBED)
    {
        smech_func_error("MechType is not flatbed\n");
        scerr = SCANERR_INVALID_PARAM;
        goto smech_move_to_position_return;
    }

    //end of previous movement
    scerr = smech_wait_for_stop(FB_MOTOR);
    if (scerr != SCANERR_NONE)
    {
        goto smech_move_to_position_return;
    }

    previous = steps_to_inches(SCAN_MECH_FLATBED, smot_step_get_location(FB_MOTOR));

    if (previous > target)
    {
        move_inches = (uint32_t)(previous - target);
        move_direction = MOVE_REVERSE;
    }
    else if (previous < target)
    {
        move_inches = (uint32_t)(target - previous);
        move_direction = MOVE_FORWARD;
    }
    else
    {
        return SCANERR_NONE;
    }

    smech_func_infor("previous=%d, target=%d\n", previous, target);
    profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600); //using middle speed
    smot_step_move_rel(FB_MOTOR,
                       profile_index,
                       inches_to_steps(SCAN_MECH_FLATBED, move_inches),
                       move_direction,
                       false,
                       -1);

    if (wait == true)
    {
        scerr = smech_wait_for_stop(FB_MOTOR);
    }
	dbg2("CISPOS( %d )\n", smot_step_get_location(FB_MOTOR));
	 
smech_move_to_position_return:
    if (scerr != SCANERR_NONE)
    {
        smech_func_error("ErrCode = %d\n", scerr);
    }
    return scerr;
}

scan_err_t smech_move_to_A3scanpoint(int direction, uint32_t distance)  
{
    scan_err_t  scerr = SCANERR_NONE;
    uint32_t    profile_index;
    uint32_t    steps;
        
    smech_func_enter();
       
	scanplat_kernel_gpio_set_value(pinetree_mech.a3_clutch, 1); 

	scerr = smech_wait_for_stop(FD_MOTOR);
	
	steps = distance*(10000/191)*2;
		
	if((direction == A3_EJECT) || (direction == DLT_EJECT)  ){ //move to eject
		profile_index = get_stmotor_profile_index(SCAN_CMODE_MONO,150);
		direction = MOVE_FORWARD;
	}
	else{ // move to scan ready.
		profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR,150);
		direction = MOVE_REVERSE;
	}
	
	smot_step_move_rel(FD_MOTOR,
					   profile_index,
					   steps,
					   direction,
					   false,
					   -1);
    
	scerr = smech_wait_for_stop(FD_MOTOR);    		
	scanplat_kernel_gpio_set_value(pinetree_mech.a3_clutch, 0); 
	
	/* after eject paper stopper arrange */
	if(direction == MOVE_FORWARD)
		smech_stopper_arrange();
	
	return scerr;
}

/* When the SC halt occur all of the mech STOP  mod.kks 2014. 06. 02 */
static void  smech_SC_halt(void)
{
	SC_halt_flg = ON;  // Don't move the motor.
	dbg1("[MECH] SC Halt Error \n");

/* Motor Sleep Mode(HW Logic OFF*/	
 #ifndef __KERNEL__
    gpio_write(scan_nmotor_driver_stby, 0);
#else
    scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 0); // Low : Sleep, High:Active. 
#endif

    smot_step_request_motor_stop(FB_MOTOR); // Scanner motor OFF.
    smot_step_request_motor_stop(ADF_MOTOR); // ADF motor OFF.
    smot_step_request_motor_stop(FD_MOTOR); // Feed motor OFF.
    scanplat_kernel_gpio_set_value(pinetree_mech.a3_clutch, 0);  // A3 clutch OFF.
}

/*add the stopper action , mod.kks*/
scan_err_t smech_stopper_arrange(void)
{
    scan_err_t  scerr = SCANERR_NONE;
    uint32_t    profile_index;
    uint32_t    steps;
        
    smech_func_enter();

	//scerr = smech_wait_for_stop(FD_MOTOR);	// del.lsh not to conflict with engine timer
	
	profile_index = get_stmotor_profile_index(SCAN_CMODE_MONO,1200);
		
	smot_step_move_rel(FD_MOTOR,
					   profile_index,
					   2500*2,
					   MOVE_REVERSE,
					   false,
					   -1);
    						
	//scerr = smech_wait_for_stop(FD_MOTOR);   // del.lsh not to conflict with engine timer
	
    smot_step_set_location(FD_MOTOR,  inches_to_steps(SCAN_MECH_ADF, 0)); 
        		
	return scerr;
}

/*add the eject paper action in Cancel Mode , mod.kks*/
scan_err_t smech_eject_paper(void)
{
    scan_err_t  scerr = SCANERR_NONE;
    uint32_t    profile_index;
    uint32_t    steps;
        
    smech_func_enter();
	
	if(adf_sensor_paper_in_path() || adf_sensor_int_present()){
		smot_step_move_rel(ADF_MOTOR,
							   smech_profile_index,
							   inches_to_steps(SCAN_MECH_ADF, 20000),
							   MOVE_FORWARD,
							   false,
							   -1);
					 
		smot_step_move_rel(FD_MOTOR,
							   stmotor_profile_index,
							   inches_to_steps(SCAN_MECH_ADF, 3000),
							   MOVE_FORWARD,
							   false,
							   -1);
	}
	else
	{
		smot_step_move_rel(ADF_MOTOR,
						   smech_profile_index,
						   inches_to_steps(SCAN_MECH_ADF, 3716),
						   MOVE_FORWARD,
						   false,
						   -1);
	}
								
	scerr = smech_wait_for_stop(ADF_MOTOR);
    
    if(adf_sensor_paper_in_path() || adf_sensor_int_present()){
		PaperJAMFlg = ON;
		smech_adf_paper_event_callback(SCAN_ADF_PAPER_JAM, true, 0);  //Remainder JAM in cancel. Mod.kks.
	}
	smech_adf_clearpage_simplify = false;
	
    if (scerr != SCANERR_NONE)
    {
        goto smech_adf_eject_paper_return;
    }
    			
smech_adf_eject_paper_return:

    if (scerr != SCANERR_NONE)
    {
        smech_func_error("ErrCode = %d\n", scerr);
    }
    return scerr;
}

/**
 * \brief  Move the scan bar to the Cal Strip position
 *
 *  A flatbed move operation to position the scan bar for a calibration scan: the mech
 *  code will be in control of blocking until the move is complete.
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE   Move complete, no error
 *  \retval SCANERR_*      Move failure,  specific error code
 **/
scan_err_t smech_move_to_calibration(void)
{
    scan_err_t scerr = SCANERR_NONE;

    smech_func_enter();

    //flatbed check
    if (mechType != SCAN_MECH_FLATBED)
    {
        smech_func_error("MechType is not flatbed\n");
        scerr = SCANERR_INVALID_PARAM;
        return scerr;
    }

    scerr = smech_move_to_position(FB_CAL_POSITION, true);
    if (scerr != SCANERR_NONE)
    {
        smech_func_error("ErrCode = %d\n", scerr);
    }
    return scerr;
}
/** 
 *    To return the Home Sensor ON state.  mod.kks 2014.06.10 
 *    Always the mech(CIS) return the home Sensor OFF to ON state.
 *    After move_to_HS_ON() the CIS current postion Home Sensor   -4.8mm (453step) 
 **/
scan_err_t smech_move_to_HS_ON(void)
{
	scan_err_t  scerr = SCANERR_NONE;
	uint32_t    profile_index=0;
	bool        FBHOME0 = false;
	uint32_t    ErrorCode = 0x0000;
	int32_t curr_location_steps;
	int32_t lowSpeed_steps = 2828; // 30mm
	int32_t highSpeed_steps = 0;

	smech_func_enter();
			
	curr_location_steps = smot_step_get_location(FB_MOTOR);
		
    //flatbed check
    if (mechType != SCAN_MECH_FLATBED)
    {
        ErrorCode = 0xE100;
        scerr = SCANERR_INVALID_PARAM;
        goto smech_find_hardstop_exit;
    }

    scerr = smech_wait_for_stop(FB_MOTOR);
    if (scerr != SCANERR_NONE)
    {
        ErrorCode = 0xE101;
        goto smech_find_hardstop_exit;
    }
	
	FBHOME0 = flatbed_sensor_home_is_found();
	
	dbg2(" MoveToHome Start (home:%d)\n", FBHOME0);
	 	 
	/***********/
	/* Home On */
	/***********/
    if (FBHOME0)
    {
		/* Don't move because Home Sensor ON state  mod.kks 2014.06.10*/
		scerr = SCANERR_NONE;
		return scerr;
	}
	/************/
	/* Home Off */
	/************/
	else
	{
		/* Home Off -> On */
		if(curr_location_steps > lowSpeed_steps) //High Speed moving
		{
			highSpeed_steps = 	curr_location_steps - lowSpeed_steps;
			profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 300);

			smot_step_move_rel(get_motor_id_from_mech(mechType),
							   profile_index,
							   highSpeed_steps,
							   MOVE_REVERSE,
							   false,
							   -1);
			smech_wait_for_stop(FB_MOTOR);							   
		}
		
		/*Low Speed moving */
		profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600);

		smot_step_move_rel(get_motor_id_from_mech(mechType),
						   profile_index,
						   lowSpeed_steps*2,
						   MOVE_REVERSE,
						   false,
						   -1);
		
		/* Waiting Sensor Event */
		{
			int sem_getvalue;

			scos_sem_getvalue(&hp_sem, &sem_getvalue);
			dbg2("[HP] Clear HP Sema (num:%d)\n", sem_getvalue);
			
			/* Clear Semaphore */
			if (sem_getvalue)
			{
				while(sem_getvalue)
				{
					scos_sem_wait(&hp_sem);
					scos_sem_getvalue(&hp_sem, &sem_getvalue);
				}
			}
			/* Waiting Semaphore */
			hp_req = true;
			hp_req_level = HIGH;
			hp_req_result = HP_FAIL;
			
			dbg1("[HP] Waiting HP Sema (%d)\n", hp_req_level);
			scos_sem_wait(&hp_sem);
			dbg1("[HP] Complete HP Sema (%d)\n", hp_req_level);
		}

        /* Stop Motor */
        if (!smot_step_motor_is_idle(FB_MOTOR))
        {
            smech_trig_scan_stopped(FB_MOTOR);
            smot_step_request_motor_stop(FB_MOTOR);
        }
        scerr = smech_wait_for_stop(FB_MOTOR);
        
#ifndef __KERNEL__
		gpio_write(scan_nmotor_driver_stby, 1);
#else
		scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 1);
#endif
		/* Error Handle */
        if (hp_req_result == HP_FAIL)
        {
        	smech_func_error("ERROR(0xE400) MoveToHome(step1 : Off -> On)\n");
            ErrorCode = 0xE400;
            scerr = SCANERR_HARDWARE_FAIL;
            scanlib_send_sc(SMSG_SC_CIS_HOME_ON_FAIL);
            goto smech_find_hardstop_exit;
        }
        
        dbg2("MoveToHome Done(Step1 : Off -> On)\n");
    }
    	
smech_find_hardstop_exit:    
    if (ErrorCode != 0x0000)
    {
        smech_func_error("ERROR ErrCode=0x%X, scerr=%d\n", ErrorCode, scerr);
    }
    return scerr;      
}

/**
 * \brief  Move the scan bar to the home position
 *
 *  A flatbed move operation to position the scan bar at the home position: the mech
 *  code will be in control of blocking until the move is complete.
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE   Move complete, no error
 *  \retval SCANERR_*      Move failure,  specific error code
 **/
scan_err_t smech_move_to_home(void)
{
	scan_err_t  scerr = SCANERR_NONE;
	uint32_t    profile_index=0;
	bool        FBHOME0 = false;
	uint32_t    ErrorCode = 0x0000;
	int32_t curr_location_steps;
	int32_t lowSpeed_steps = 2828; // 30mm
	int32_t highSpeed_steps = 0;

	smech_func_enter();
			
	curr_location_steps = smot_step_get_location(FB_MOTOR);
    
    /* Remove current CIS postion check mod.kks 2014.06.10 
	if(curr_location_steps == FB_HOME_POSITION){
		dbg2("MoveToHome Done (alread home)\n");
		return scerr;
	}
	*/
		
    //flatbed check
    if (mechType != SCAN_MECH_FLATBED)
    {
        ErrorCode = 0xE100;
        scerr = SCANERR_INVALID_PARAM;
        goto smech_find_hardstop_exit;
    }

    scerr = smech_wait_for_stop(FB_MOTOR);
    if (scerr != SCANERR_NONE)
    {
        ErrorCode = 0xE101;
        goto smech_find_hardstop_exit;
    }
	
	FBHOME0 = flatbed_sensor_home_is_found();
	
	dbg2(" MoveToHome Start (home:%d)\n", FBHOME0);
	 	 
	/***********/
	/* Home On */
	/***********/
    if (FBHOME0)
    {
		/* Home On -> Off */
		
        profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600);
        
        smot_step_move_rel(get_motor_id_from_mech(mechType),
                           profile_index,
                           inches_to_steps(SCAN_MECH_FLATBED, 790), 
                           MOVE_FORWARD,
                           false,
                           -1);

		/* Waiting Sensor Event */
		{
			int sem_getvalue;

			scos_sem_getvalue(&hp_sem, &sem_getvalue);
			dbg2("[HP] Clear HP Sema (num:%d)\n", sem_getvalue);
			
			/* Clear Semaphore */
			if (sem_getvalue)
			{
				while(sem_getvalue)
				{
					scos_sem_wait(&hp_sem);
					scos_sem_getvalue(&hp_sem, &sem_getvalue);
				}
			}
			/* Waiting Semaphore */
			hp_req = true;
			hp_req_level = LOW;
			hp_req_result = HP_FAIL;
			
			dbg1("[HP] Waiting HP Sema (%d)\n", hp_req_level);
			scos_sem_wait(&hp_sem);
			dbg1("[HP] Complete HP Sema (%d)\n", hp_req_level);
		}

		/* Stop Motor */
        if (!smot_step_motor_is_idle(FB_MOTOR))
        {
            smech_trig_scan_stopped(FB_MOTOR);
            smot_step_request_motor_stop(FB_MOTOR);
        }
        scerr = smech_wait_for_stop(FB_MOTOR);
        
#ifndef __KERNEL__
		gpio_write(scan_nmotor_driver_stby, 1);
#else
		scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 1);
#endif
		/* Error Handle */
        if (hp_req_result == HP_FAIL)
        {
        	smech_func_error("ERROR(0xE200) MoveToHome(step1 : On -> Off)\n");
            ErrorCode = 0xE200;
            scerr = SCANERR_HARDWARE_FAIL;
            scanlib_send_sc(SMSG_SC_CIS_HOME_OFF_FAIL);
            goto smech_find_hardstop_exit;
        }
        
		smot_step_set_location(FB_MOTOR, inches_to_steps(SCAN_MECH_FLATBED, FB_HOME_POSITION));
        dbg2(" MoveToHome Done(On -> Off)\n");
	}
	/************/
	/* Home Off */
	/************/
	else
	{
		/* Home Off -> On */
		if(curr_location_steps > lowSpeed_steps) //High Speed moving
		{
			highSpeed_steps = 	curr_location_steps - lowSpeed_steps;
			profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 300);

			smot_step_move_rel(get_motor_id_from_mech(mechType),
							   profile_index,
							   highSpeed_steps,
							   MOVE_REVERSE,
							   false,
							   -1);
			smech_wait_for_stop(FB_MOTOR);							   
		}
		
		/*Low Speed moving */
		profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600);

		smot_step_move_rel(get_motor_id_from_mech(mechType),
						   profile_index,
						   lowSpeed_steps*2,
						   MOVE_REVERSE,
						   false,
						   -1);
		
		/* Waiting Sensor Event */
		{
			int sem_getvalue;

			scos_sem_getvalue(&hp_sem, &sem_getvalue);
			dbg2("[HP] Clear HP Sema (num:%d)\n", sem_getvalue);
			
			/* Clear Semaphore */
			if (sem_getvalue)
			{
				while(sem_getvalue)
				{
					scos_sem_wait(&hp_sem);
					scos_sem_getvalue(&hp_sem, &sem_getvalue);
				}
			}
			/* Waiting Semaphore */
			hp_req = true;
			hp_req_level = HIGH;
			hp_req_result = HP_FAIL;
			
			dbg1("[HP] Waiting HP Sema (%d)\n", hp_req_level);
			scos_sem_wait(&hp_sem);
			dbg1("[HP] Complete HP Sema (%d)\n", hp_req_level);
		}

        /* Stop Motor */
        if (!smot_step_motor_is_idle(FB_MOTOR))
        {
            smech_trig_scan_stopped(FB_MOTOR);
            smot_step_request_motor_stop(FB_MOTOR);
        }
        scerr = smech_wait_for_stop(FB_MOTOR);
        
#ifndef __KERNEL__
		gpio_write(scan_nmotor_driver_stby, 1);
#else
		scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 1);
#endif
		/* Error Handle */
        if (hp_req_result == HP_FAIL)
        {
        	smech_func_error("ERROR(0xE400) MoveToHome(step1 : Off -> On)\n");
            ErrorCode = 0xE400;
            scerr = SCANERR_HARDWARE_FAIL;
            scanlib_send_sc(SMSG_SC_CIS_HOME_ON_FAIL);
            goto smech_find_hardstop_exit;
        }
        
        dbg2("MoveToHome Done(Step1 : Off -> On)\n");
        
		/* Home On -> Off */
		
        profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600);
        
        smot_step_move_rel(get_motor_id_from_mech(mechType),
                           profile_index,
                           inches_to_steps(SCAN_MECH_FLATBED, 790), 
                           MOVE_FORWARD,
                           false,
                           -1);

		/* Waiting Sensor Event */
		{
			int sem_getvalue;

			scos_sem_getvalue(&hp_sem, &sem_getvalue);
			dbg2("[HP] Clear HP Sema (num:%d)\n", sem_getvalue);
			
			/* Clear Semaphore */
			if (sem_getvalue)
			{
				while(sem_getvalue)
				{
					scos_sem_wait(&hp_sem);
					scos_sem_getvalue(&hp_sem, &sem_getvalue);
				}
			}
			/* Waiting Semaphore */
			hp_req = true;
			hp_req_level = LOW;
			hp_req_result = HP_FAIL;
			
			dbg1("[HP] Waiting HP Sema (%d)\n", hp_req_level);
			scos_sem_wait(&hp_sem);
			dbg1("[HP] Complete HP Sema (%d)\n", hp_req_level);
		}

		/* Stop Motor */
        if (!smot_step_motor_is_idle(FB_MOTOR))
        {
            smech_trig_scan_stopped(FB_MOTOR);
            smot_step_request_motor_stop(FB_MOTOR);
        }
        scerr = smech_wait_for_stop(FB_MOTOR);
        
#ifndef __KERNEL__
		gpio_write(scan_nmotor_driver_stby, 1);
#else
		scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 1);
#endif
		/* Error Handle */
        if (hp_req_result == HP_FAIL)
        {
        	smech_func_error("ERROR(0xE600) MoveToHome(step2 : On -> Off)\n");
            ErrorCode = 0xE600;
            scerr = SCANERR_HARDWARE_FAIL;
            scanlib_send_sc(SMSG_SC_CIS_HOME_OFF_FAIL);
            goto smech_find_hardstop_exit;
        }
        smot_step_set_location(FB_MOTOR, inches_to_steps(SCAN_MECH_FLATBED, FB_HOME_POSITION));
        dbg2("MoveToHome Done(Step2 : On -> Off)\n");
    }
	
smech_find_hardstop_exit:    
    if (ErrorCode != 0x0000)
    {
        smech_func_error("ERROR ErrCode=0x%X, scerr=%d\n", ErrorCode, scerr);
    }
    return scerr;  
}


/**
 * \brief  Find the hardstop position by ramming the wall
 *
 *  A flatbed move operation to position the scan bar at the home position: the mech
 *  code will be in control of blocking until the move is complete.  This routine is
 *  used when we don't know exactly where we are (thus the intentional wall ram):
 *    - First we need to make a long move towards the hard stop (wall)
 *    - Then move a known distance back to our home position
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE   Move complete, no error
 *  \retval SCANERR_*      Move failure,  specific error code
 **/
scan_err_t smech_find_hardstop(void)
{
    scan_err_t  scerr = SCANERR_NONE;
    uint32_t    profile_index=0;
    bool        FBHOME0 = false;
    uint32_t    ErrorCode = 0x0000;

    smech_func_enter();

    //flatbed check
    if (mechType != SCAN_MECH_FLATBED)
    {
        ErrorCode = 0xE100;
        scerr = SCANERR_INVALID_PARAM;
        goto smech_find_hardstop_exit;
    }

    scerr = smech_wait_for_stop(FB_MOTOR);
    if (scerr != SCANERR_NONE)
    {
        ErrorCode = 0xE101;
        goto smech_find_hardstop_exit;
    }
	
	FBHOME0 = flatbed_sensor_home_is_found();
	
	dbg2("POR MoveToHome Start (home:%d)\n", FBHOME0);
	 	 
	/***********/
	/* Home On */
	/***********/
    if (FBHOME0)
    {
	/* Home On -> Off */
		
        profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600);
        
        smot_step_move_rel(get_motor_id_from_mech(mechType),
                           profile_index,
                           inches_to_steps(SCAN_MECH_FLATBED, 790), 
                           MOVE_FORWARD,
                           false,
                           -1);

		/* Waiting Sensor Event */
		{
			int sem_getvalue;

			scos_sem_getvalue(&hp_sem, &sem_getvalue);
			dbg1("[HP] POR Clear HP Sema (num:%d)\n", sem_getvalue);
			
			/* Clear Semaphore */
			if (sem_getvalue)
			{
				while(sem_getvalue)
				{
					scos_sem_wait(&hp_sem);
					scos_sem_getvalue(&hp_sem, &sem_getvalue);
				}
			}
			/* Waiting Semaphore */
			hp_req = true;
			hp_req_level = LOW;
			hp_req_result = HP_FAIL;
			
			dbg1("[HP] POR Waiting HP Sema (%d)\n", hp_req_level);
			scos_sem_wait(&hp_sem);
			dbg1("[HP] POR Complete HP Sema (%d)\n", hp_req_level);
		}
			
		/* Stop Motor */
        if (!smot_step_motor_is_idle(FB_MOTOR))
        {
            smech_trig_scan_stopped(FB_MOTOR);
            smot_step_request_motor_stop(FB_MOTOR);
        }
        scerr = smech_wait_for_stop(FB_MOTOR);
        
#ifndef __KERNEL__
		gpio_write(scan_nmotor_driver_stby, 1);
#else
		scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby,1);
#endif
		/* Error Handle*/
        if (hp_req_result == HP_FAIL)
        {
        	smech_func_error(" ERROR(0xE200) POR MoveToHome(step1 : On -> Off)\n");
            ErrorCode = 0xE200;
            scerr = SCANERR_HARDWARE_FAIL;
            scanlib_send_sc(SMSG_SC_CIS_HOME_OFF_FAIL);
            goto smech_find_hardstop_exit;
        }
        
		smot_step_set_location(FB_MOTOR, inches_to_steps(SCAN_MECH_FLATBED, FB_TOP_HARDSTOP_POSITION));
        dbg2(" POR MoveToHome Done(On -> Off)\n");
	}
	/************/
	/* Home Off */
	/************/
	else
	{
		/* Home Off -> On */
		
		profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 300);

		smot_step_move_rel(get_motor_id_from_mech(mechType),
						   profile_index,
						   inches_to_steps(SCAN_MECH_FLATBED, FB_BOT_HARDSTOP_POSITION),
						   MOVE_REVERSE,
						   false,
						   -1);

		/* Waiting Sensor Event */		
		{
			int sem_getvalue;

			scos_sem_getvalue(&hp_sem, &sem_getvalue);
			dbg1("[HP] POR Clear HP Sema (num:%d)\n", sem_getvalue);
			
			/* Clear Semaphore */
			if (sem_getvalue)
			{
				while(sem_getvalue)
				{
					scos_sem_wait(&hp_sem);
					scos_sem_getvalue(&hp_sem, &sem_getvalue);
				}
			}
			/* Waiting Semaphore */
			hp_req = true;
			hp_req_level = HIGH;
			hp_req_result = HP_FAIL;
			
			dbg1("[HP] POR Waiting HP Sema (%d)\n", hp_req_level);
			scos_sem_wait(&hp_sem);
			dbg1("[HP] POR Complete HP Sema (%d)\n", hp_req_level);
		}
        		
        /* Stop Motor */     
        if (!smot_step_motor_is_idle(FB_MOTOR))
        {
            smech_trig_scan_stopped(FB_MOTOR);
            smot_step_request_motor_stop(FB_MOTOR);
        }
        scerr = smech_wait_for_stop(FB_MOTOR);
		
#ifndef __KERNEL__
		gpio_write(scan_nmotor_driver_stby, 1);
#else
		scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 1);
#endif
		/* Error Handle */
        if (hp_req_result == HP_FAIL)
        {
        	smech_func_error(" ERROR(0xE400) POR MoveToHome(step1 : Off -> On)\n");
            ErrorCode = 0xE400;
            scerr = SCANERR_HARDWARE_FAIL;
            scanlib_send_sc(SMSG_SC_CIS_HOME_ON_FAIL);
            goto smech_find_hardstop_exit;
        }
        
        dbg2(" POR MoveToHome Done(Step1 : Off -> On)\n");
#if 0       
		/* Home On -> Off */
		
        profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600);
        
        smot_step_move_rel(get_motor_id_from_mech(mechType),
                           profile_index,
                           inches_to_steps(SCAN_MECH_FLATBED, 790), 
                           MOVE_FORWARD,
                           false,
                           -1);
		
		/* Waiting Sensor Event */
		{
			int sem_getvalue;

			scos_sem_getvalue(&hp_sem, &sem_getvalue);
			dbg1("[HP] POR Clear HP Sema (num:%d)\n", sem_getvalue);
			
			/* Clear Semaphore */
			if (sem_getvalue)
			{
				while(sem_getvalue)
				{
					scos_sem_wait(&hp_sem);
					scos_sem_getvalue(&hp_sem, &sem_getvalue);
				}
			}
			/* Waiting Semaphore */
			hp_req = true;
			hp_req_level = LOW;
			hp_req_result = HP_FAIL;
			
			dbg1("[HP] POR Waiting HP Sema (%d)\n", hp_req_level);
			scos_sem_wait(&hp_sem);
			dbg1("[HP] POR Complete HP Sema (%d)\n", hp_req_level);
		}
				
		/* Stop Motor */
        if (!smot_step_motor_is_idle(FB_MOTOR))
        {
            smech_trig_scan_stopped(FB_MOTOR);
            smot_step_request_motor_stop(FB_MOTOR);
        }
        scerr = smech_wait_for_stop(FB_MOTOR);
		
#ifndef __KERNEL__
		gpio_write(scan_nmotor_driver_stby, 1);
#else
		scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 1);
#endif
		
		/* Error Handle */
        if (hp_req_result == HP_FAIL)
        {
        	smech_func_error(" ERROR(0xE600) POR MoveToHome(step2 : On -> Off)\n");
            ErrorCode = 0xE600;
            scerr = SCANERR_HARDWARE_FAIL;
            scanlib_send_sc(SMSG_SC_CIS_HOME_OFF_FAIL);
            goto smech_find_hardstop_exit;
        }
        smot_step_set_location(FB_MOTOR, inches_to_steps(SCAN_MECH_FLATBED, FB_TOP_HARDSTOP_POSITION));
        dbg2(" POR MoveToHome Done(Step2 : On -> Off)\n");
#endif
		/* Temporary the CIS Position Setting. mod.kks  2014.06.10
		 *  To avoid abnormal case. If the move_to_home() is called the CIS position not define fail.
		 */
        smot_step_set_location(FB_MOTOR, inches_to_steps(SCAN_MECH_FLATBED, FB_TOP_HARDSTOP_POSITION)); 
    }
	
smech_find_hardstop_exit:    
    if (ErrorCode != 0x0000)
    {
        smech_func_error(" ERROR ErrCode=0x%X, scerr=%d\n", ErrorCode, scerr);
    }
    return scerr;
}



/**
 *  \brief Stage ADF Sheet
 *
 *  Helper function to stage the first sheet in the ADF at the TOF sensor.  Note
 *  it is still possible to encounter ADF paper jams or door open events while
 *  running the stage operation.
 *
 *  \warning
 *  Calling this routine with paper already in the path will cause a new paper jam
 *  event.
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE          ADF is staged
 *  \retval SCANERR_NO_PAPER_EDGE Didn't find a leading edge, not staged
 *  \retval SCANERR_PAPER_JAM     Paper jam (either existing or new)
 *  \retval SCANERR_*             Motor move failure, failure specific error code
 **/
scan_err_t smech_adf_input_paper(void)
{
    scan_err_t   scerr = SCANERR_NONE;
    uint32_t       profile_index;
    bool		PP0;
    bool		PIPD0;
    
    PP0= adf_sensor_paper_present();
    PIPD0 = adf_sensor_paper_in_path();

    smech_func_enter();

    //ADF check
    if (mechType != SCAN_MECH_ADF)
    {
        smech_func_error("MechType is not ADF\n");
        scerr = SCANERR_INVALID_PARAM;
        goto smech_adf_input_paper_return;
    }
    
    //paper jam check
    if (adf_sensor_paper_in_path())
    {
        smech_func_error("paper was already in path\n");
        scerr = SCANERR_PAPER_JAM;
        goto smech_adf_input_paper_return;
    }
    
    /*mod.kks*/
    if(PP0 == false){
		smot_step_request_motor_stop(ADF_MOTOR);
		smot_step_request_motor_stop(FD_MOTOR);

	}
	
    //end of previous movement
    scerr = smech_wait_for_stop(ADF_MOTOR);
    if (scerr != SCANERR_NONE)
    {
        goto smech_adf_input_paper_return;
    }
    
    profile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 300);
    smot_step_move_rel(ADF_MOTOR,
                       profile_index,
                       inches_to_steps(SCAN_MECH_ADF, ADF_STAGE_DIST),
                       MOVE_FORWARD,
                       false,
                       -1);

    do
    {
        if (adf_sensor_paper_in_path())
        {
            smech_func_infor("paper in path\n");
            smot_step_request_motor_stop(ADF_MOTOR);
            smot_step_request_motor_stop(FD_MOTOR);
            break;
        }

#ifdef __KERNEL__
        schedule();
#else
        {
            Uint         txretcode;
            txretcode = tx_thread_sleep(1);
            XASSERT(txretcode == TX_SUCCESS, txretcode);
        }
#endif
    } while (!smot_step_motor_is_idle(ADF_MOTOR));
    scerr = smech_wait_for_stop(ADF_MOTOR);
    smot_step_set_location(ADF_MOTOR, inches_to_steps(SCAN_MECH_ADF, ADF_HOME_POSITION));

    if (scerr == SCANERR_NONE)
    {
        if (!adf_sensor_paper_in_path())
        {
            smech_func_infor("paper not in path\n");
            scerr = SCANERR_NO_PAPER_EDGE;
        }
    }

smech_adf_input_paper_return:

    if (scerr != SCANERR_NONE)
    {
        smech_func_error("ErrCode = %d\n", scerr);
    }
    return scerr;
}


/**
 *  \brief ADF test function
 **/
/* ADF Free Run of SP Scanner service Mode. mod.kks 2014.06.12*/
scan_err_t smech_adf_test_func(void)
{
     scan_err_t   scerr = SCANERR_NONE;
    uint32_t     profile_index;
    int          testpoint1;
    int          testpoint2;
    int          testpoint;
    uint32_t     state;

    smech_func_enter();
    
    scerr = smech_wait_for_stop(ADF_MOTOR);
    scerr = smech_wait_for_stop(FD_MOTOR);
    
    SP_AdfFreeRunModeFlg = ON;
    
    if(!adf_sensor_paper_present()){
		dbg1("No paper. Please set the paper on the ADF \n");
		return;
	}

    profile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 300);
    smot_step_move_rel(ADF_MOTOR,
                       profile_index,
                       inches_to_steps(SCAN_MECH_ADF, 200000),
                       MOVE_FORWARD,
                       false,
                       -1);
	         
	smot_step_move_rel(FD_MOTOR,
					   stmotor_profile_index,
					   inches_to_steps(SCAN_MECH_ADF, 200000),
					   MOVE_FORWARD,
					   false,
					   -1);

	ADF_mech_running = ON;

    testpoint1 = smot_step_get_location(ADF_MOTOR) + inches_to_steps(SCAN_MECH_ADF, 10000);
    testpoint2 = testpoint1;
    state = 0;
    do
    {
        if (state == 0)
        {
            testpoint = smot_step_get_location(ADF_MOTOR);
            if (testpoint >= testpoint1)
            {
                state = 1;
            }
        }
        else if (state == 1)
        {
            if (!adf_sensor_paper_present())
            {
                state = 2;
                testpoint2 = smot_step_get_location(ADF_MOTOR) + inches_to_steps(SCAN_MECH_ADF, 10000);
            }
        }
        else if (state == 2)
        {
            testpoint = smot_step_get_location(ADF_MOTOR);
            if (testpoint >= testpoint2)
            {
                break;
            }
        }
    } while (!smot_step_motor_is_idle(ADF_MOTOR));
    smot_step_request_motor_stop(ADF_MOTOR);
    smot_step_request_motor_stop(FD_MOTOR);
    scerr = smech_wait_for_stop(ADF_MOTOR);
    SP_AdfFreeRunModeFlg = OFF;
    
    smot_step_set_location(ADF_MOTOR, inches_to_steps(SCAN_MECH_ADF, ADF_HOME_POSITION));
	if(!cio_debounced_state && !adf_sensor_paper_in_path() && !adf_sensor_int_present())
		smech_stopper_arrange();  //mod.kks

smech_adf_test_func_return:

    if (scerr != SCANERR_NONE)
    {
        smech_func_error("ErrCode = %d\n", scerr);
    }
    return scerr;
}
/* ADF Free Run Mode End */

uint32_t       FDMOTOR_STATE = 0;
bool         PIPD0;
bool         PIPD1;
bool         INPD0;
bool         INPD1;
bool		PP0;
bool		TOP;

int			FB_STEPS;
uint32_t			GAP_STEPS = 0;

/* SP INPUT Mode Sensor State mod.kks 2014.06.12*/
bool SP_IntervalSensor0;
bool SP_IntervalSensor1;
bool SP_HomeSensor0;
bool SP_HomeSensor1; 
/*SP INPUT Mode END*/
	
#ifdef __KERNEL__
static void smech_adf_timer_callback(unsigned long cb_data)
#else
static void smech_adf_timer_callback(void *cb_data)
#endif
{
	const struct scanvars *scanvar;
		
	/* Sensor Debounce */
    adf_sensor_pp_debounce_sample();
    adf_sensor_cio_debounce_sample();
	
	/* Only the routine be used  in SP INPTU Check Mode for no effect in normal case mod.kks 2014.06.12*/
	if(SP_IN_check_flg){
		SP_IntervalSensor0 = adf_sensor_int_present();
		SP_HomeSensor0 = flatbed_sensor_home_is_found();
			
		/*Interval Sensor Check */
		if(SP_IntervalSensor0 != SP_IntervalSensor1){
			SP_IntervalSensor1 = SP_IntervalSensor0;
			smech_adf_paper_event_callback(SCAN_SP_PAPER_INTERVAL, adf_sensor_int_present() ? 1 : 0, 0);
		}
			
		/*Home Sensor Check */
		if(SP_HomeSensor0 != SP_HomeSensor1){
			SP_HomeSensor1 = SP_HomeSensor0;
			smech_adf_paper_event_callback(SCAN_SP_PAPER_HOME, flatbed_sensor_home_is_found() ? 1 : 0, 0);
		}
	}
	/* SP INPUT Chcec END */
	 
	/* Home Position Detection */
	if(hp_req == true){
		bool FBHOME0 = false;
		
		FBHOME0 = flatbed_sensor_home_is_found();

		/* Post Semaphore */
		if (FBHOME0 == hp_req_level || smot_step_motor_is_idle(FB_MOTOR))
		{
			dbg2("[HP] Post HP Sema (Home:%d)\n", hp_req_level);
			scos_sem_post(&hp_sem);
			hp_req = false;
			
			if(FBHOME0 == hp_req_level)
				hp_req_result = HP_SUCCESS;
			else
				hp_req_result = HP_FAIL;
		
			/* Stop Motor */
#ifndef __KERNEL__
			gpio_write(scan_nmotor_driver_stby, 0);
#else
			scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 0);
#endif
		}
	}
	
	 
	if(!sleep_mode_state){ // Wait for the stable Sensor state
		/* If the Top Cover State cange, it will check below code mod.kks 2014.06.13*/
		if(ChangeTopCoverState){  
			ChangeTopCoverState = OFF;
			if(cio_debounced_state)  //Check the Top Cover debounce State. mod.kks 2014.06.13
			{
				if(ADF_mech_running == ON){
					if(SP_AdfFreeRunModeFlg){ //Check ADF Free Run Mode case. If the normal ADF running It does not used. mod.kks 2014.06.23
						dbg1( "Top Cover Open in ADF FREE RUN MODE!!\n");
						smot_step_request_motor_stop(FD_MOTOR);
						smot_step_request_motor_stop(ADF_MOTOR);
					}
					FDMOTOR_STATE = 0x00;
					ADF_mech_running = OFF;
					feedmotor_forbid = OFF;
				}
				else{ // IDLE STATE.  mod.kks 2014.06.13
					/* Check the motor Stop in Normal case mod.kks 2014.06.12 */
					if(!smot_step_motor_is_idle(FD_MOTOR) && SP_OUT_check_flg == OFF){ 
						smot_step_request_motor_stop(FD_MOTOR);
					}
					
					/* Check the motor Stop in Normal case mod.kks 2014.06.12 */
					if(!smot_step_motor_is_idle(ADF_MOTOR) && SP_OUT_check_flg == OFF){ 
						smot_step_request_motor_stop(ADF_MOTOR);
					}
				}
							
				needToStopper = ON;
			}
			else //TOP Cover Close
			{
				if(!adf_sensor_paper_in_path() &&  !adf_sensor_int_present() )   // No Paper on adf path. mod.kks 2014.06.13
				{
					/*Stopper arrange mod.kks */
					if(needToStopper)  
					{
						needToStopper = OFF;
						if(smot_step_motor_is_idle(FD_MOTOR)){	// mod.lsh not to conflict with engine timer
							smech_stopper_arrange();
						}
					}

					/* Jam Clear */
					if(PaperJAMFlg){
	#if 1	// mod.lsh.to display cover status in ready state
						PaperJAMFlg = OFF;
						if(!SP_AdfFreeRunModeFlg){ //TBD check list(app level) with lee young hoo . mod.kks
							smech_adf_paper_event_callback(SCAN_ADF_PAPER_JAM, false, 0);  //Mod.kks JAM Clear
							smech_adf_paper_event_callback(SCAN_ADF_PAPER_NOPICK, false, 0);  
							dbg1( "[jam] Clear Jam!!\n");
						}
	#endif
					} 
				}
				else // When Top Cover close, the paper is located on adf path. mod.kks 2014.06.13  /
				{
					if(!SP_AdfFreeRunModeFlg) //TBD check list(app level) with lee young hoo . mod.kks
					{
						PaperJAMFlg = ON;
						smech_adf_paper_event_callback(SCAN_ADF_PAPER_JAM, true, 0);  //Remainder JAM Mod.kks   
						dbg1( "[jam] Remainder Jam!!\n");
					}
				}
			}		
		}
	}
		
	/* ADF Scan Mecha Control */
	if(PaperJAMFlg){
		FDMOTOR_STATE = 0x00;
		ADF_mech_running = OFF;
	}
	
	/* IDLE State */
	if(FDMOTOR_STATE == 0x00)
	{
		if(ADF_mech_running == ON)
		{
			FDMOTOR_STATE = 0x01;
			smech_func_infor("0 -> 1 \n");
		}
	}
	/*feed motor (PAPER PICKUP) off timing check*/
	else if (FDMOTOR_STATE == 0x01) 
	{
		PP0= adf_sensor_paper_present();  //Last Page Solution kks.
		if(PP0==false)
		{
			FDMOTOR_STATE = 0x00;
			smech_func_infor("1 -> 0 \n");
			ADF_mech_running = OFF;
		}
		else{
			INPD0 = adf_sensor_int_present();
			if (INPD0 == true)
			{
				FB_STEPS = smot_step_get_location(FD_MOTOR) + inches_to_steps(SCAN_MECH_ADF, 2362);   //60mm
				FDMOTOR_STATE = 0x02; 
				smech_func_infor("1 -> 2 \n");
			}
		}
	}
	
	/*Feed Motor OFF */
	else if (FDMOTOR_STATE == 0x02)
	{
		if (smot_step_get_location(FD_MOTOR) > FB_STEPS)
		{
			smot_step_request_motor_stop(FD_MOTOR);
			feedmotor_forbid = ON;
			FDMOTOR_STATE = 0x03;
			smech_func_infor("2 -> 3 \n");
		}
	}
	/*Before job end check*/
	else if (FDMOTOR_STATE == 0x03)
	{
		PIPD0 = adf_sensor_paper_in_path();
		if(PIPD0 == false)
		{
			FDMOTOR_STATE = 0x04 ;
			smech_func_infor("3 -> 4 \n");
		}
	}
	/* Position Sensor ON check*/
	else if (FDMOTOR_STATE == 0x04)
	{
		PIPD0 = adf_sensor_paper_in_path();
		if(PIPD0 == true)
		{
			FDMOTOR_STATE = 0x05 ;
			smech_func_infor("4 -> 5 \n");
		}
	}
	/*Paper GAP setting */
	else if(FDMOTOR_STATE == 0x05)
	{
		INPD0 = adf_sensor_int_present(); /* Page gap check */
		if(INPD0 == false)
		{	
			feedmotor_forbid = OFF;
			PP0= adf_sensor_paper_present();  /* Next Page check */
			if(PP0 ==true && !cancel_request_flg){  //Cancel Mode Check mod.kks 2014.07.23
				if(duplexflg){  // To keep the  2nd side image in duplex Mode.
					//GAP_STEPS = smot_step_get_location(ADF_MOTOR) + inches_to_steps(SCAN_MECH_ADF, 3740);   //95mm
					GAP_STEPS = smot_step_get_location(ADF_MOTOR) + inches_to_steps(SCAN_MECH_ADF, 3740+2000);   //175mm
					FDMOTOR_STATE = 0x06;
					smech_func_infor("5 -> 6 \n");
				}
				else{  //SImplex 
					if(SP_OUT_check_flg == OFF){  //Normal Mode mod.kks 2014.06.12
						scanvar = scanvar_get();
						if (scanvar->cmode == SCAN_CMODE_MONO && scanvar->hw_dpi_vert == 300 ) {  // Only Mono 300
							FDMOTOR_STATE = 0x07;
							smech_func_infor("5- > 7 \n");
						}
						else{ // Not Mono 300.
							GAP_STEPS = smot_step_get_location(ADF_MOTOR) +inches_to_steps(SCAN_MECH_ADF, 3149);  //80mm 
							FDMOTOR_STATE = 0x06;
							smech_func_infor("5 -> 6 \n");
						}
					}
					else{ //ADF_Free Run in SP MODE. mod.kks 2014.06.12 (Always 300 Mono Simplex)
						FDMOTOR_STATE = 0x07;
						smech_func_infor("5- > 7 \n");
					}
				}	
			}
			else{ //No Paper
				ADF_mech_running = OFF;
				FDMOTOR_STATE = 0x16;
				smech_func_infor("5 -> 16 \n");
			}
		}
	}
    /* Paper Gap Check */
	else if (FDMOTOR_STATE == 0x06)
	{
		if (smot_step_get_location(ADF_MOTOR) > GAP_STEPS)
		{
			FDMOTOR_STATE = 0x07;
			smech_func_infor("6 -> 7 \n");
		}
	}
	/* Next Page Feeding */
	else if (FDMOTOR_STATE == 0x07)
	{
		if (smot_step_motor_is_idle(FD_MOTOR) && !smot_step_motor_is_idle(ADF_MOTOR)) 
		{
			smot_step_set_location(FD_MOTOR,  inches_to_steps(SCAN_MECH_ADF, 0)); 
			smot_step_move_rel(FD_MOTOR,
								   stmotor_profile_index,
								   inches_to_steps(SCAN_MECH_ADF, 200000),
								   MOVE_FORWARD,
								   false,
								   -1);
			FDMOTOR_STATE = 0x01;
			smech_func_infor("7 -> 1 \n");
		}
	}
	/* Mech Control End */
	else if (FDMOTOR_STATE == 0x16) 
	{
		if (smot_step_motor_is_idle(ADF_MOTOR)) 
		{
			FDMOTOR_STATE = 0x00; 
			smech_func_infor("16 -> 0 \n");  
		}
		else
		{
			FDMOTOR_STATE = 0x16;  // wait the ADF Motor Stop.
		}
	}
#ifdef __KERNEL__
smech_timer.expires = jiffies + MILLISEC(1);
add_timer(&smech_timer);
#endif
}


/**
 *  \brief get ADF position
 *
 *  \return ADF position
 **/
int smech_get_adf_position(void)
{
    return (smot_step_get_location(ADF_MOTOR));
}


/**
 *  \brief ADF Clear paper path
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE   Empty input tray operation successful
 *  \retval SCANERR_*      Move failure, failure specific error code
 **/
scan_err_t smech_adf_clear_paper(void)
{
    int        firstEmptyPos;
    int        currEmptyPos;
    int        deltaPos;
    scan_err_t scerr = SCANERR_NONE;
    uint32_t     profile_index;
    bool       ADFPIP0;
    bool       ADFPIP1;

    smech_func_enter();

    //ADF check
    if (mechType != SCAN_MECH_ADF)
    {
        smech_func_error("MechType is not ADF\n");
        scerr = SCANERR_INVALID_PARAM;
        goto smech_adf_clear_paper_return;
    }

    firstEmptyPos = 0;
    currEmptyPos  = 0;

    ADFPIP0 = adf_sensor_paper_in_path();
    ADFPIP1 = ADFPIP0;
    if (smech_adf_clearpage_simplify)
    {
        if (ADFPIP0)
        {
            smech_func_infor("paper in path\n");
            //end of previous movement
            scerr = smech_wait_for_stop(ADF_MOTOR);
            if (scerr != SCANERR_NONE)
            {
                goto smech_adf_clear_paper_return;
            }
            /*smot_step_set_location(ADF_MOTOR, inches_to_steps(SCAN_MECH_ADF, ADF_HOME_POSITION));
            profile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 300);
            smot_step_move_rel(ADF_MOTOR,
                               profile_index,
                               inches_to_steps(SCAN_MECH_ADF, ADF_EMPTY_DIST),
                               MOVE_FORWARD,
                               false,
                               -1);*/
        }
        else
        {
            smech_func_infor("paper not in path \n");
        }
    }
    else
    {
        //end of previous movement
        scerr = smech_wait_for_stop(ADF_MOTOR);
        if (scerr != SCANERR_NONE)
        {
            goto smech_adf_clear_paper_return;
        }
        /*smot_step_set_location(ADF_MOTOR, inches_to_steps(SCAN_MECH_ADF, ADF_HOME_POSITION));
        profile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 300);
        smot_step_move_rel(ADF_MOTOR,
                           profile_index,
                           inches_to_steps(SCAN_MECH_ADF, ADF_EMPTY_DIST),
                           MOVE_FORWARD,
                           false,
                           -1);*/
    }

    do
    {
        ADFPIP1 = ADFPIP0;
        ADFPIP0 = adf_sensor_paper_in_path();
        if (ADFPIP1 != ADFPIP0)
        {
            if (ADFPIP0)
            {
                smech_func_infor("paper in path\n");
            }
            else
            {
                smech_func_infor("paper not in path\n");
            }
        }
		
        if (!ADFPIP0)
        {/*  mod.kks
            currEmptyPos = smot_step_get_location(ADF_MOTOR);
            if (firstEmptyPos == 0)
            {
                firstEmptyPos = currEmptyPos;
            }
            if (currEmptyPos >= firstEmptyPos)
            {
                deltaPos = currEmptyPos - firstEmptyPos;
            }
            else
            {
                deltaPos = 0;
            }
			
            //if (deltaPos >= inches_to_steps(SCAN_MECH_ADF, 8000))
            if (deltaPos >= inches_to_steps(SCAN_MECH_ADF, 1000))
            {*/
                smot_step_request_motor_stop(ADF_MOTOR);
                smot_step_request_motor_stop(FD_MOTOR);
                break;
            //}
        }

#ifdef __KERNEL__
//        schedule();
#else
        {
            Uint       txretcode;
            txretcode = tx_thread_sleep(1);
            XASSERT(txretcode == TX_SUCCESS, txretcode);
        }
#endif
    } while (!smot_step_motor_is_idle(ADF_MOTOR));
    scerr = smech_wait_for_stop(ADF_MOTOR);
    if (scerr != SCANERR_NONE)
    {
        goto smech_adf_clear_paper_return;
    }
    smot_step_set_location(ADF_MOTOR, inches_to_steps(SCAN_MECH_ADF, ADF_HOME_POSITION));

smech_adf_clear_paper_return:

    smech_adf_clearpage_simplify = false;
    if (scerr != SCANERR_NONE)
    {
        smech_func_error("ErrCode = %d\n", scerr);
    }
    return scerr;
}


void smech_trig_scan_stopped(stmotor_id_t motor_id)
{
    /*
     * BIG FAT NOTE! This function called from interrupt context!
     */
    smech_set_status(SCAN_MECH_READY);
}


void smech_trig_pause_rewind_stopped(stmotor_id_t motor_id)
{
    /* The motor code coasts to a complete stop, so no additional wait is needed */
    smech_pause_rewind_stopped(motor_id);
}

void smech_pause_rewind_stopped (stmotor_id_t motor_id)
{
    /* WARNING: we could be in IRQ context here! */

    /* We are done, set mech to ready */
    smech_set_status(SCAN_MECH_READY);
}


void smech_trig_paused_stopped(stmotor_id_t motor_id)
{
    /* The motor code coasts to a complete stop, so no additional wait is needed */
    smech_paused_stopped(motor_id);
}


void smech_paused_stopped(stmotor_id_t motor_id)
{
    if (mechType == SCAN_MECH_ADF)
    {
        smech_trig_pause_rewind_stopped(ADF_MOTOR);
    }
    else
    {
        smech_trig_pause_rewind_stopped(FB_MOTOR);
    }
}

// customer-specific pattern match on the notch.
// Scan buffer is passed in by caller, we index into the proper line and 
// then look for a pattern match.
//
// This simple demo version here just looks for the first 16 pixels to be dark
//
bool match_notch_pattern_on_scanline(uint8_t *scan_data,int y_count,uint32_t scan_pixels_per_row)
{
#define DARK_NOTCH_PIXEL 0x60

    int x;
    bool notch_found=true;
    uint8_t *scanline;

    scanline = &scan_data[y_count*scan_pixels_per_row];

    for (x=0;x<16;x++)
    {
        if (scanline[x] > DARK_NOTCH_PIXEL)
        {
            notch_found = false;
            break;
        }
    }
    return(notch_found);
}

#define INCH_MOVE FB_STEPS_PER_INCH
/**
 *  \brief: main function of notch find
 **/
static scan_err_t smech_pinetree_notchfind(t_ScanMech* scan_motor)
{
    scan_err_t        scerr = SCANERR_NONE;
    scan_err_t        final_scerr = SCANERR_NONE;
    uint32_t          ErrorCode = 0x0000;
    struct scanvars *sv;
    uint32_t          red_gain, green_gain, blue_gain;
    uint32_t          red_duty, green_duty, blue_duty;
    uint32_t          red_offset, green_offset, blue_offset;    

    smech_func_enter();
    if (mechType != SCAN_MECH_FLATBED)
    {
        final_scerr = SCANERR_INVALID_PARAM;
        ErrorCode = 0xE100;
        goto smech_notch_find_exit2;
    }
    scan_enable_motor_motion(true);

    smot_step_set_location(FB_MOTOR, 0); // reset our current position, since we'll be resetting anyway

    // move the bar 1 inch away from the notch in case we are past it
    smot_step_move_rel(FB_MOTOR,
                       get_stmotor_profile_index(SCAN_CMODE_MONO, 300),
                       INCH_MOVE,
                       MOVE_FORWARD,
                       false,
                       0);
    scerr = smech_wait_for_stop(get_motor_id_from_mech(mechType));

    if (scerr != SCANERR_NONE)
    {
        final_scerr = scerr;
        ErrorCode = 0xE101;
        goto smech_notch_find_exit2;
    }

    notch_find_results = SCANERR_MISSED_NOTCH;
    sv = scanplat_sv_mono_scan_new();
    if (sv == NULL)
    {
        final_scerr = SCANERR_OUT_OF_MEMORY;
        ErrorCode = 0xE200;
        goto smech_notch_find_exit2;
    }

    scanvar_push(sv);

    scanvar_set_use_pie(sv, false);
    sv->white_clip_point[0] = NOTCHFIND_SCANVAR_WHITE_CLIP;
    sv->black_clip_point[0] = NOTCHFIND_SCANVAR_BLACK_CLIP;
    scanvar_set_gamma(sv, NOTCHFIND_SCANVAR_GAMMA);
    scanvar_set_area(sv, 0, NOTCHFIND_SCAN_STARTPOS, NOTCHFIND_SCAN_WIDTH, NOTCHFIND_SCAN_LEN); 
    scanvar_set_dpi(sv, NOTCHFIND_SCAN_RESOLUTION);
    scanvar_set_pic_bpp(sv, 8);
    scanvar_set_scan_callback(sv, smech_notch_find_msg_data);

    scan_enable_motor_motion(true);
    scerr = scanmech_prepare(SCAN_TARGET_TYPE_NOTCHFIND_REVERSE);
    if (scerr != SCANERR_NONE)
    {
        final_scerr = SCANERR_MISSED_NOTCH;
        ErrorCode = 0xE201;
        goto smech_notch_find_exit1;
    }

    scerr = scanlib2_setup();
    if (scerr != SCANERR_NONE)
    {
        final_scerr = SCANERR_MISSED_NOTCH;
        ErrorCode = 0xE202;
        goto smech_notch_find_exit1;
    }

    scanimg_get_analog_gain(0, &red_gain, &green_gain, &blue_gain);
    scanimg_get_led_duty(0, &red_duty, &green_duty, &blue_duty);
    red_duty   = 100;
    green_duty = 100;
    blue_duty  = 100;
    red_gain = 450;
    green_gain = 450;    
    blue_gain = 450;
    red_offset = -28000;
    green_offset = -28000;
    blue_offset = -28000;    

    scanimg_set_analog_offset(0, red_offset, green_offset, blue_offset);
    scanimg_set_analog_gain(0, red_gain, green_gain, blue_gain);
    scanimg_set_led_duty(0, red_duty, green_duty, blue_duty);

    scerr = scanlib2_run(0);
    if (scerr != SCANERR_NONE)
    {
        if (scerr == SCANERR_HARDWARE_FAIL)
        {
            final_scerr = scerr;
            ErrorCode = 0xE203;
        }
        if (scerr == SCANERR_SCAN_CANCELLED)
        {
            // if we found the notch, all is well, since when we find the notch we cancel the scan
            if (notch_find_results == SCANERR_NONE)
                final_scerr = SCANERR_NONE;
            else
            {
                final_scerr = scerr;
                ErrorCode = 0xE204;
            }
        }
        else
        {
            final_scerr = SCANERR_MISSED_NOTCH;
            ErrorCode = 0xE205;
        }
    }

    scanlib2_done();
    scanmech_done(SMECH_DONE_FLAG_SUCCESS);

    if (notch_find_results == SCANERR_NONE)
    {
        int current_pos_steps;
        // Need to set the stepper motor location now that the notch is found.
        // Functions like the next 2 commented out lines can be used to compute
        // current location.
        // current_pos_inches = steps_to_inches(SCAN_MECH_FLATBED, smot_step_get_location(FB_MOTOR));
        // current_pos_inches = current_pos_inches + NOTCH_START_POSITION;
        // current_pos_steps = inches_to_steps(SCAN_MECH_FLATBED, current_pos_inches);

        current_pos_steps = 0; // replace with the proper calculations above
        smot_step_set_location(FB_MOTOR, current_pos_steps);

        // now move to "home" location
        scerr = smech_move_to_home();  // Notchfind home dectect in marvell code. It does not use in PINETREE.
        if (scerr != SCANERR_NONE)
        {
            ErrorCode = 0xE301;
        }
    }
    else
    {
        final_scerr = notch_find_results;
        ErrorCode = 0xE302;
    }

smech_notch_find_exit1:
    scerr = smech_wait_for_stop(get_motor_id_from_mech(mechType));
    if (scerr != SCANERR_NONE)
    {
        ErrorCode = 0xE300;
    }
    scanvar_pop();
    scanvar_delete(&sv);

smech_notch_find_exit2:
    if (final_scerr != SCANERR_NONE)
    {
        smech_func_error("NG, ErrCode=0x%X, final_scerr=%d\n", ErrorCode, final_scerr);
    }
    return (final_scerr);
}


/**
 *  \brief: callback function of notch find
 **/
scan_err_t smech_notch_find_msg_data(scan_msg_t *msg)
{
    uint32_t   scan_rows = 0;
    uint8_t   *scan_data;
    int y_count;
    scan_err_t scerr;

    if (msg->msgType == SMSG_SCAN_SIZE)
    {
        scan_pixels_per_row = (uint32_t)msg->param1;
        scan_expected_num_rows = (uint32_t) msg->param3;
        DBG_PRINTF_SDH_DEBUG("notch pixels %d rows %d\n", msg->param1, (int) msg->param3);
    }
    else if (msg->msgType == SMSG_PAGE_DATA)
    {
        if (notch_find_results == SCANERR_NONE)
            return SCANERR_NONE; // we've already found the notch, just ignore the buffered buffers
        
        XASSERT(msg->param3 != 0, (uint32_t) msg->param3);

        scan_rows = SMSG_DATA_GET_NUM_ROWS(msg->param1);

        // make sure we really scanned mono data
        XASSERT(SMSG_DATA_GET_DATATYPE(msg->param2) == SCAN_DATA_TYPE_MONO,msg->param2); 

        // the actual page data from the scan is in param3
        scan_data  = (uint8_t*) msg->param3;

        DBG_PRINTF_SDH_DEBUG("data buffer rows %d\n", scan_rows);

        for (y_count = 0; y_count < scan_rows; y_count++)
        {
            if (match_notch_pattern_on_scanline(scan_data,y_count,scan_pixels_per_row))
            {
                // found the notch pattern!
                notch_find_results = SCANERR_NONE;
                /* Send a message to scantask asking to cancel the scan. */
                scerr = scantask_cancel_msg();
                XASSERT( scerr==0, scerr );

                if (!smot_step_motor_is_idle(FB_MOTOR))
                {
                    smech_trig_scan_stopped(FB_MOTOR);
                    smot_step_request_motor_stop(FB_MOTOR);
                }
                scerr = smech_wait_for_stop(FB_MOTOR);
                
                break;
            }
        }
        kfree(scan_data);
    }
    else if (msg->msgType == SMSG_PAGE_START)
    {
        //nothing to do here for notch find
    }
    else if (msg->msgType == SMSG_PAGE_END)
    {
        //nothing to do here for notch find
    }
    else
    {
        XASSERT(0, msg->msgType);
    }

    return (SCANERR_NONE);
}


//MRVL-Harris-20120119 for scan power-on calibration {
scan_err_t smech_poweron_cal_run(struct scanvars **psv)
{
    scan_err_t cal_scerr, final_scerr;
    struct scanvars *sv;
    struct scanvars *cal_sv;
    struct scanvars *tmp_sv;

    PTR_ASSIGN(sv, *psv);

    scanvar_push(sv);

    sv = scanvar_get();

    scanvar_calc_pixel_area(sv);

    cal_sv = scanvar_cal_newcopy(sv);

    scanvar_push(cal_sv);


    // hardcode sensor 0 (sensor_bitmap) to do poweron cal
    cal_scerr = calibrate(0x01, cal_sv->hw_dpi_horiz, cal_sv->cmode, true);

    tmp_sv = scanvar_pop();
    XASSERT( tmp_sv==cal_sv, tmp_sv->id );
    tmp_sv = NULL;

    scanvar_delete(&cal_sv);

    if (cal_scerr != SCANERR_NONE)
    {
        final_scerr = cal_scerr;
        goto smech_poweron_cal_run_exit;
    }

    final_scerr = SCANERR_NONE;

smech_poweron_cal_run_exit:

    if (scanvar_peek())
    {
        tmp_sv = scanvar_pop();
        XASSERT( tmp_sv==sv, tmp_sv->id );
        tmp_sv = NULL;
        PTR_ASSIGN( *psv, sv );
    }

    XASSERT( scanvar_peek()==NULL, scanvar_peek()->id );
    return final_scerr;
}


scan_err_t smech_poweron_cal(scan_cmode_t cmode, int dpi)
{
    struct scanvars *sv;
    scan_err_t scerr;

    if (cmode==SCAN_CMODE_MONO)
    {
        sv = scanplat_sv_mono_scan_new();
    }
    else
    {
        sv = scanplat_sv_color_scan_new();
    }
    if (sv==NULL)
    {
        scerr = SCANERR_OUT_OF_MEMORY;
        goto smech_poweron_cal_exit;
    }

    scerr = scanvar_set_dpi(sv, dpi);
    if (scerr != SCANERR_NONE)
    {
        goto smech_poweron_cal_exit;
    }

    scerr = smech_poweron_cal_run(&sv);

    ASSERT(sv!=NULL);

smech_poweron_cal_exit:

    scanvar_delete(&sv);
    return scerr;
}
//MRVL-Harris-20120119 for scan power-on calibration }


/**
 *  \brief Page setup (Mech API)
 *
 *  Mechanism API function to setup for scanning. At this point the page prepare
 *  operation has completed and we need to set the motor parameters and row sync
 *  start position for the actual scan.
 *
 *  \param[in] scan_mech         Pointer to mech data structure
 *
 **/
static void smech_pinetree_scan_setup(t_ScanMech* scan_motor)
{
    const struct scanvars *scanvar;
    long int x, y, width, height;
    int currentstep;
    int targetstep;
    stmotor_id_t motor_id;

    smech_func_enter();

    XASSERT(current_scan_target_type!=SCAN_TARGET_TYPE_NONE,
            current_scan_target_type);

    smech_profile_index = 0;
    smech_pagedone_homing_enable = false;

    scanvar = scanvar_get();

    switch(current_scan_target_type)
    {
        case SCAN_TARGET_TYPE_CAL :
            //MRVL-Harris-20120302 for scan moving calibration {
            if (!smech_scan_move_cal_enable)
            {
                smech_motor_sync = SYNC_DISABLE;
                smech_start_pos = 0;
                smech_profile_index = get_stmotor_profile_index(scanvar->cmode, scanvar->hw_dpi_vert);
                scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_STEPPER, MOT_STEPPER);
            }
            else
            {
                smech_motor_sync = SYNC_FIRST;
                smech_start_pos = FB_REWIND_REVERSE_STEPS;
                smech_profile_index = get_stmotor_profile_index(scanvar->cmode, scanvar->hw_dpi_vert);
              //MRVL-Harris-20121030 {
              #ifdef CONFIG_ARCH_MV31X0
                scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_STEPPER, MOT_DC);
              #elif CONFIG_ARCH_MV61X0
                scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_DC, MOT_DC);
              #else
              #error No implementation for this ASIC
              #endif
              //MRVL-Harris-20121030 }
                set_stmotor_ls_source(mechType);
                if (scanvar->cmode == SCAN_CMODE_COLOR) {
                    motor_id = get_motor_id_from_mech(mechType);
                    if (scanvar->hw_dpi_vert == 300) {
                        smot_step_set_line_start(motor_id, 1, FB_LSINCR_COLO_0300);
                    } else if (scanvar->hw_dpi_vert == 600) {
                        smot_step_set_line_start(motor_id, 1, FB_LSINCR_COLO_0600);
                    } else if (scanvar->hw_dpi_vert == 1200) {
                        smot_step_set_line_start(motor_id, 1, FB_LSINCR_COLO_1200);
                    } else {
                        XASSERT(0, scanvar->hw_dpi_vert);
                    }
                } else if (scanvar->cmode == SCAN_CMODE_MONO) {
                    motor_id = get_motor_id_from_mech(mechType);
                    if (scanvar->hw_dpi_vert == 300) {
                        smot_step_set_line_start(motor_id, 1, FB_LSINCR_MONO_0300);
                    } else if (scanvar->hw_dpi_vert == 600) {
                        smot_step_set_line_start(motor_id, 1, FB_LSINCR_MONO_0600);
                    } else if (scanvar->hw_dpi_vert == 1200) {
                        smot_step_set_line_start(motor_id, 1, FB_LSINCR_MONO_1200);
                    } else {
                        XASSERT(0, scanvar->hw_dpi_vert);
                    }
                }
                else {
                    XASSERT(0, scanvar->cmode);
                }
            }
            //MRVL-Harris-20120302 for scan moving calibration }
            break;


        case SCAN_TARGET_TYPE_DOC :
            if (mechType == SCAN_MECH_ADF)
            {
                smech_motor_sync = SYNC_EVERY;

                scanvar_set_flip_horizontal(scanvar_get(), true);

                smot_step_set_location(get_motor_id_from_mech(mechType),
                                       inches_to_steps(mechType, ADF_HOME_POSITION));

                smech_start_pos = AD_LS_START_POSITION;

                smech_profile_index = get_stmotor_profile_index(scanvar->cmode, scanvar->hw_dpi_vert);
				
                if (smech_motor_sync == SYNC_DISABLE)
                {
                    scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_STEPPER, MOT_STEPPER);
                }
                else
                {
                  //MRVL-Harris-20121030 {
                  #ifdef CONFIG_ARCH_MV31X0
                    scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_STEPPER, MOT_DC);
                  #elif CONFIG_ARCH_MV61X0
                    scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_DC, MOT_DC);
                  #else
                  #error No implementation for this ASIC
                  #endif
                  //MRVL-Harris-20121030 }
                    set_stmotor_ls_source(mechType);
                    if (smech_motor_sync == SYNC_EVERY)
                    {
                        if (scanvar->cmode == SCAN_CMODE_COLOR) {
                            motor_id = get_motor_id_from_mech(mechType);
                            if (scanvar->hw_dpi_vert == 300) {
                                smot_step_set_line_start(motor_id, 1, AD_LSINCR_COLO_0300);
                            } else if (scanvar->hw_dpi_vert == 600) {
                                smot_step_set_line_start(motor_id, 1, AD_LSINCR_COLO_0600);
								scanplat_kernel_gpio_set_value(pinetree_mech.current_control, 1);  //LOW Current  0.5A    mod.kks                           
                            } else if (scanvar->hw_dpi_vert == 1200) {
                                smot_step_set_line_start(motor_id, 1, AD_LSINCR_COLO_1200);
                            } else {
                                XASSERT(0, scanvar->hw_dpi_vert);
                            }
                        } else if (scanvar->cmode == SCAN_CMODE_MONO) {
                            motor_id = get_motor_id_from_mech(mechType);
                            if (scanvar->hw_dpi_vert == 300) {
                                smot_step_set_line_start(motor_id, 1, AD_LSINCR_MONO_0300);
                            } else if (scanvar->hw_dpi_vert == 600) {
                                smot_step_set_line_start(motor_id, 1, AD_LSINCR_MONO_0600);
                            } else if (scanvar->hw_dpi_vert == 1200) {
                                smot_step_set_line_start(motor_id, 1, AD_LSINCR_MONO_1200);
                            } else {
                                XASSERT(0, scanvar->hw_dpi_vert);
                            }
                        }
                        else {
                            XASSERT(0, scanvar->cmode);
                        }
                    }
                }
            }
            else //FB_MOTOR
            {
                smech_motor_sync = SYNC_EVERY;

                scanvar_set_flip_horizontal(scanvar_get(), false);

                scanvar_get_area(scanvar, &x, &y, &width, &height);

                currentstep = smot_step_get_location(FB_MOTOR);
                if ( scan_get_config() & SCAN_CONFIG_SCAN_FROM_HOME )
                {
                    targetstep = inches_to_steps(mechType, FB_HOME_POSITION) + 
                                 inches_to_steps(mechType, y * 10);
                }
                else
                {
                    targetstep = inches_to_steps(mechType, FB_TOP_BED_POSITION) +
                                 inches_to_steps(mechType, y * 10);
                }
                if (currentstep > targetstep) {
                    smech_start_pos = currentstep - targetstep;
                } else {
                    smech_start_pos = targetstep - currentstep;
                }

                smech_profile_index = get_stmotor_profile_index(scanvar->cmode, scanvar->hw_dpi_vert);
//??                if ( scan_get_config() & SCAN_CONFIG_SCAN_FROM_HOME )
//??                {
//??                    smech_start_pos = 0;
//??                    smech_motor_sync = SYNC_DISABLE;
//??                }

                if (smech_motor_sync == SYNC_DISABLE)
                {
                    scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_STEPPER, MOT_STEPPER);
                }
                else
                {
                  //MRVL-Harris-20121030 {
                  #ifdef CONFIG_ARCH_MV31X0
                    scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_STEPPER, MOT_DC);
                  #elif CONFIG_ARCH_MV61X0
                    scif_motor_setup(get_stmotor_scif(mechType), smech_motor_sync, MOT_DC, MOT_DC);
                  #else
                  #error No implementation for this ASIC
                  #endif
                  //MRVL-Harris-20121030 }
                    set_stmotor_ls_source(mechType);
                    {
                        if (scanvar->cmode == SCAN_CMODE_COLOR) {
                            motor_id = get_motor_id_from_mech(mechType);
                            if (scanvar->hw_dpi_vert == 300) {
                                smot_step_set_line_start(motor_id, 1, FB_LSINCR_COLO_0300);
                            } else if (scanvar->hw_dpi_vert == 600) {
                                smot_step_set_line_start(motor_id, 1, FB_LSINCR_COLO_0600);
                            } else if (scanvar->hw_dpi_vert == 1200) {
                                smot_step_set_line_start(motor_id, 1, FB_LSINCR_COLO_1200);
                            } else {
                                XASSERT(0, scanvar->hw_dpi_vert);
                            }
                        } else if (scanvar->cmode == SCAN_CMODE_MONO) {
                            motor_id = get_motor_id_from_mech(mechType);
                            if (scanvar->hw_dpi_vert == 300) {
                                smot_step_set_line_start(motor_id, 1, FB_LSINCR_MONO_0300);
                            } else if (scanvar->hw_dpi_vert == 600) {
                                smot_step_set_line_start(motor_id, 1, FB_LSINCR_MONO_0600);
                            } else if (scanvar->hw_dpi_vert == 1200) {
                                smot_step_set_line_start(motor_id, 1, FB_LSINCR_MONO_1200);
                            } else {
                                XASSERT(0, scanvar->hw_dpi_vert);
                            }
                        }
                        else {
                            XASSERT(0, scanvar->cmode);
                        }
                    }
                }
            }
            break;

        //MRVL-Harris-20120109 for notchfind {
        case SCAN_TARGET_TYPE_NOTCHFIND_FORWARD:
        case SCAN_TARGET_TYPE_NOTCHFIND_REVERSE:
            smech_motor_sync = SYNC_FIRST;
            scanvar_set_flip_horizontal(scanvar_get(), true);
            scanvar_get_area(scanvar, &x, &y, &width, &height);
            currentstep = smot_step_get_location(FB_MOTOR);
            targetstep = inches_to_steps(SCAN_MECH_FLATBED, y * 10);
            if (currentstep > targetstep) {
                smech_start_pos = currentstep - targetstep;
            } else {
                smech_start_pos = targetstep - currentstep;
            }
            smech_profile_index = get_stmotor_profile_index(scanvar->cmode, scanvar->hw_dpi_vert);
          //MRVL-Harris-20121030 {
          #ifdef CONFIG_ARCH_MV31X0
            scif_motor_setup(get_stmotor_scif(SCAN_MECH_FLATBED), smech_motor_sync, MOT_STEPPER, MOT_DC);
          #elif CONFIG_ARCH_MV61X0
            scif_motor_setup(get_stmotor_scif(SCAN_MECH_FLATBED), smech_motor_sync, MOT_DC, MOT_DC);
          #else
          #error No implementation for this ASIC
          #endif
          //MRVL-Harris-20121030 }
            set_stmotor_ls_source(SCAN_MECH_FLATBED);
            break;
        //MRVL-Harris-20120109 for notchfind }

        default :
            XASSERT(0, current_scan_target_type);
            break;

    }

    smech_func_infor(" ## smech_motor_sync = %d, smech_start_pos = %d\n", smech_motor_sync, smech_start_pos);
}


/**
 *  \brief Page prepare (Mech API)
 *
 *  Mechanism API function to prepare a page for scanning.  This step must
 *  put the mech in a state to do the requested scan.  For example:
 *   - a calibration scan needs to be positioned under the cal target
 *   - a flatbed scan needs the scanbar to be staged in the proper position
 *   - an ADF scan needs the paper to be positioned at Top Of Form
 *
 *  If we can't get the mech ready an error must be returned so we can unwind.
 *
 *  \warning
 *  Preparing for one type of scan may require us to do another type of scan. For
 *  example: a TOF scan will call back out to scanlib, which will effectively cause
 *  this routine to be re-entered.
 *
 *  \param[in] scan_mech         Pointer to mech data structure
 *  \param[in] scan_target_type  Type of scan to prepare for
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE          Page prepare passed
 *  \retval SCANERR_HARDWARE_FAIL Motor move failed, scanner is stuck/dead
 **/
static scan_err_t smech_pinetree_page_prepare(t_ScanMech* scan_motor, scan_target_type_t scan_target_type)
{
    scan_err_t scerr = SCANERR_NONE;
    const struct scanvars *sv;
    long int x, y, width, height;
    int32_t curr_location_steps;
    int32_t scan_location_steps;
    uint32_t delta_steps;
    uint32_t stage_pos;

    smech_func_enter();

    current_scan_target_type = scan_target_type;
    smech_pause_rewind_enable = false;

    switch(scan_target_type)
    {
        case SCAN_TARGET_TYPE_CAL:
            mechType = SCAN_MECH_FLATBED;
            scerr = smech_move_to_calibration();
            break;

        case SCAN_TARGET_TYPE_DOC:
            if (isDocSourceADF())
            {
                mechType = SCAN_MECH_FLATBED;
                //scerr = smech_move_to_position(FB_HOME_POSITION, true);
                /* CIS Home Position detecting mod.kks 2014.06.10 */
                scerr = smech_move_to_home();  //CIS move Homesensor ON to OFF.
                if (scerr != SCANERR_NONE)
                {
                    smech_func_error("FB move home failt\n");
                    scerr = SCANERR_SCAN_CANCELLED;
                }
                
                /* ADF Scan position move mod.kks 2014.06.10 */
                scerr = smech_move_to_position(100, true); // 2.54mm moving 
                if (scerr != SCANERR_NONE)
                {
                    smech_func_error("FB move home failt\n");
                    scerr = SCANERR_SCAN_CANCELLED;
                }
                mechType = SCAN_MECH_ADF;
								
                if (!adf_sensor_paper_present())
                {
                    smech_func_error("please load paper first\n");
                    scerr = SCANERR_SCAN_CANCELLED;
                }
                
                if (smech_adf_inputpage_simplify == false)
                {
                    if (scerr == SCANERR_NONE)
                    {
                        scerr = smech_adf_input_paper();
                    }
                }
                //MRVL-Harris-20120215 for ADF {
                else
                {
                    if (adf_sensor_paper_in_path() || adf_sensor_int_present())
                    {
						smech_func_error("Jam!! (before Scan)\n");
						smech_adf_paper_event_callback(SCAN_ADF_PAPER_JAM, true, 0);
						
						PaperJAMFlg = ON;
                        scerr = SCANERR_PAPER_JAM;
                    }
                    else if(cio_debounced_state)  //Check the Top Cover debounce State. mod.kks 2014.06.13
                    {
                        smech_func_error("Cover Open Jam!!  (before Scan)\n");
                        smech_adf_paper_event_callback(SCAN_ADF_TOP_COVER_OPEN, true, 0);

                        PaperJAMFlg = ON;
                        scerr = SCAN_ADF_TOP_COVER_OPEN;
                    }
                }
                //MRVL-Harris-20120215 for ADF }
            }
            else //FB Scan prepare.
            {
                mechType = SCAN_MECH_FLATBED;
                if ( scan_get_config() & SCAN_CONFIG_SCAN_FROM_HOME )
                {
                    scerr = smech_move_to_position(FB_TOP_HARDSTOP_POSITION, true);
                }
                else
                {
                    scerr = smech_move_to_home();  // Home ON to OFF at the FB Scan before.
                }
                sv = scanvar_get();
                if ( !(scan_get_config() & SCAN_CONFIG_SCAN_FROM_HOME) &&
                     (sv->hw_dpi_vert > 300) )
                {
                    scanvar_get_area(sv, &x, &y, &width, &height);
                    curr_location_steps = smot_step_get_location(FB_MOTOR);
                    scan_location_steps = inches_to_steps(mechType, FB_TOP_BED_POSITION) + inches_to_steps(mechType, y*10);
                    if (curr_location_steps >= scan_location_steps)
                    {
                        delta_steps = curr_location_steps - scan_location_steps;
                    }
                    else
                    {
                        delta_steps = scan_location_steps - curr_location_steps;
                    }


                    if (delta_steps > inches_to_steps(mechType, FB_STAGE_MIN_DELTA))
                    {
                        if (scan_location_steps > inches_to_steps(mechType, FB_STAGE_DIST))
                        {
                            stage_pos = steps_to_inches(mechType, (scan_location_steps - inches_to_steps(mechType, FB_STAGE_DIST)));

                            if (FB_TOP_BED_POSITION < stage_pos)
                            {
                                scerr = smech_move_to_position(stage_pos, true);
                            }
                        }
                    }
                }
            }
            break;

        case SCAN_TARGET_TYPE_NOTCHFIND_FORWARD:
        case SCAN_TARGET_TYPE_NOTCHFIND_REVERSE:
            mechType = SCAN_MECH_FLATBED;
            break;

        default:
            XASSERT(0, scan_target_type);
            break;

    }

    if (scerr != SCANERR_NONE)
    {
        smech_func_error("ErrCode = %d\n", scerr);
        current_scan_target_type = SCAN_TARGET_TYPE_NONE;
    }

    return scerr;
}


/**
 *  \brief Page run (Mech API)
 *
 *  Mechanism API function to run a scan. At this point the page prepare
 *  and setup operations have completed, now we need to start the motors.
 *
 *  \param[in] scan_mech         Pointer to mech data structure
 *
 **/

static void smech_pinetree_scan_start(t_ScanMech* scan_motor)
{
    const struct scanvars *scanvar;
    scan_err_t scerr = SCANERR_NONE;
    int    currentstep;
    int    targetstep;
    uint32_t deltastep;
    scan_cmdq_row_status_t smech_scan_cmdq_row_status;
    int                    smech_completed_rows;

    smech_func_enter();

    scanvar = scanvar_get();
    smech_set_status(SCAN_MECH_RUNNING);
	
    if (mechType == SCAN_MECH_ADF)
    {
        switch (current_scan_target_type)
        {
            case SCAN_TARGET_TYPE_DOC:
                smech_wait_for_stop(FD_MOTOR);
                smech_wait_for_stop(ADF_MOTOR);
                if (smech_pause_rewind_enable)
                {
                    smech_pause_rewind_enable = false;
                }
                adf_sensor_cio_callback_enable(); //MRVL-Harris-20120215 for ADF
                smot_step_move_rel(ADF_MOTOR,
									   smech_profile_index,
									   0xFFFFFFF,
									   MOVE_FORWARD,
									   true,
									   smech_start_pos);
                 
                 if(feedmotor_forbid == OFF && adf_sensor_paper_present() ){                 
					smot_step_move_rel(FD_MOTOR,
										   stmotor_profile_index,
										   inches_to_steps(SCAN_MECH_ADF, 200000),
										   MOVE_FORWARD,
										   false,
										   -1);
    			 }
				
				ADF_mech_running = ON;
				smech_func_infor("smech_pinetree_scan_start !!! \n");
                break;

				default:
					smech_func_error("unexpected ADF scan target %d\n", current_scan_target_type);
					XASSERT(0, current_scan_target_type);
					break;
        }
    }
    else if (mechType == SCAN_MECH_FLATBED)
    {
        switch (current_scan_target_type)
        {
            //MRVL-Harris-20120109 for notchfind {
            case SCAN_TARGET_TYPE_NOTCHFIND_FORWARD:
                smech_wait_for_stop(FB_MOTOR);
                smot_step_move_rel(FB_MOTOR,
                                   smech_profile_index,
                                   inches_to_steps(SCAN_MECH_FLATBED, (NOTCHFIND_SCAN_LEN*4*10)),
                                   MOVE_FORWARD,
                                   true,
                                   smech_start_pos);
                break;

            case SCAN_TARGET_TYPE_NOTCHFIND_REVERSE:
                smech_wait_for_stop(FB_MOTOR);
                smot_step_move_rel(FB_MOTOR,
                                   smech_profile_index,
                                   inches_to_steps(SCAN_MECH_FLATBED, (NOTCHFIND_SCAN_LEN*4*10)),
                                   MOVE_REVERSE,
                                   true,
                                   smech_start_pos);
                break;
            //MRVL-Harris-20120109 for notchfind }

            case SCAN_TARGET_TYPE_DOC:
                smech_wait_for_stop(FB_MOTOR);
                if (smech_pause_rewind_enable)
                {
                    smech_scan_cmdq_row_status = scan_cmdq_get_row_status();
                    smech_completed_rows = smech_scan_cmdq_row_status.rs_completed_rows;
                    currentstep = smot_step_get_location(FB_MOTOR) - FB_RAMP_STEPS;
                    targetstep  = inches_to_steps(SCAN_MECH_FLATBED, FB_TOP_BED_POSITION) +
                                  inches_to_steps(SCAN_MECH_FLATBED, (scanvar->user_area_hinch.y * 10)) +
                                  inches_to_steps(SCAN_MECH_FLATBED, ((smech_completed_rows * 1000) / scanvar->hw_dpi_vert));

                    DBG_PRINTF_SDH_DEBUG("smech_completed_rows=%d\n", smech_completed_rows);
                    DBG_PRINTF_SDH_DEBUG("currentstep=%d\n", currentstep);
                    DBG_PRINTF_SDH_DEBUG("targetstep=%d\n", targetstep);

                    if (currentstep >= targetstep)
                    {
                        deltastep = currentstep - targetstep;
                        smech_start_pos = smech_start_pos - deltastep;
                    }
                    else
                    {
                        deltastep = targetstep - currentstep;
                        smech_start_pos = smech_start_pos + deltastep;
                    }

                    smot_step_move_rel(FB_MOTOR,
                                       smech_profile_index,
                                       FB_REWIND_REVERSE_STEPS,
                                       MOVE_REVERSE,
                                       false,
                                       -1);

                    smech_wait_for_stop(FB_MOTOR);
                    smech_pause_rewind_enable = false;
                }
#if 0  //KKS Test
                smot_step_move_abs(FB_MOTOR,
                                   smech_profile_index,
                                   inches_to_steps(SCAN_MECH_FLATBED, FB_BOT_BED_POSITION),
                                   true,
                                   smech_start_pos);
#else
				if(smech_profile_index == 3){	   //Mono 300
					smot_step_move_rel(FB_MOTOR,
												   smech_profile_index,
												   smech_start_pos+28020+377+5,         //Until  ~ 11.62 inch    1step = 0.021207294 (2- clk)
												   //smech_start_pos+14055+141+5,         //Until  ~ 11.62 inch    1step = 0.021207294 (2- clk)
												   //smech_start_pos+28000+377+5,        //I tax Show
												   MOVE_FORWARD,
												   true,
												   smech_start_pos);  
				}
				else{
				smot_step_move_rel(FB_MOTOR,
											   smech_profile_index,
											   smech_start_pos+28100+377+5,         //Until  ~ 11.62 inch
											   //smech_start_pos+14055+141+5,         //Until  ~ 11.62 inch
											  //smech_start_pos+28050+377+5,         //I tax Show
											   MOVE_FORWARD,
											   true,
											   smech_start_pos);  
				}						  
#endif
                break;

            case SCAN_TARGET_TYPE_CAL:
                //MRVL-Harris-20120302 for scan moving calibration {
                if (smech_scan_move_cal_enable)
                {
                   if(cal_retry_flag){ //for SC217
						cal_retry_flag = 0;
						scerr = smech_move_to_home(); 
						if (scerr != SCANERR_NONE)     
						{
							smech_func_error("smech_find_home error\n");
						}    								
  						smech_move_to_calibration();
						dbg1("[RETRY_TEST] cal_retry_flag = %d \n", cal_retry_flag);
  					}

	                smech_wait_for_stop(FB_MOTOR);
                    smot_step_move_rel(FB_MOTOR,
                                       get_stmotor_profile_index(SCAN_CMODE_COLOR, 600),
                                       FB_REWIND_REVERSE_STEPS,
                                       MOVE_REVERSE,
                                       false,
                                       -1);
                    smech_wait_for_stop(FB_MOTOR);

 					mdelay(100);
                    smot_step_move_abs(FB_MOTOR,
                                       smech_profile_index,
                                       inches_to_steps(SCAN_MECH_FLATBED, FB_BOT_BED_POSITION),
                                       true,
                                       smech_start_pos);
                }
                //MRVL-Harris-20120302 for scan moving calibration }
                break;

            default:
                smech_func_error("unexpected flatbed scan target %d\n", current_scan_target_type);
                smot_step_move_abs(FB_MOTOR,
                                   smech_profile_index,
                                   inches_to_steps(SCAN_MECH_FLATBED, FB_BOT_BED_POSITION),
                                   true,
                                   smech_start_pos);
                break;
        }
    }
    else
    {
        smech_func_error("unknown MechType %d\n", mechType);
        XASSERT(0, mechType);
    }
}


/**
 *  \brief Page done (Mech API)
 *
 *  Mechanism API function to do page done processing.  In general this
 *  routine should leave the scanbar where it started before the scan, which
 *  is usually the PP position.  Multi-page ADF scans are an execption as we
 *  need to scan additional pages.
 *
 *  \param[in] scan_mech         Pointer to mech data structure
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE           ADF page prepare passed
 *  \retval SCANERR_HARDWARE_FAIL  Motor move failed, scanner is stuck/dead
 *  \retval SCANERR_SCAN_CANCELLED Jam/mispick, cancel
 **/
static scan_err_t smech_pinetree_page_done(t_ScanMech* scan_motor, smech_done_flags_t flags)
{
    bool                   AreWeCancelling;
    scan_err_t             scerr = SCANERR_NONE;

    smech_func_enter();

    AreWeCancelling = false;
    if(flags == SMECH_DONE_FLAG_CANCEL)
    {
        smech_func_infor("flags == SMECH_DONE_FLAG_CANCEL\n");
        AreWeCancelling = true;
    }

    if (AreWeCancelling && (current_scan_target_type == SCAN_TARGET_TYPE_NONE))
    {
        return scerr;
    }

    switch(current_scan_target_type)
    {
        case SCAN_TARGET_TYPE_CAL:
            if (mechType == SCAN_MECH_ADF)
            {
                smot_step_set_location(ADF_MOTOR, inches_to_steps(SCAN_MECH_ADF, ADF_HOME_POSITION));
            }
            else
            {
//                scerr = smech_move_to_calibration(); //test.kmg
            }
            break;

        case SCAN_TARGET_TYPE_DOC:
            if (mechType == SCAN_MECH_ADF)
            {
                //MRVL-Harris-20120215 for ADF {
                if (AreWeCancelling) // mod.kks in cancel mode.
                {
					AreWeCancelling = false;
					FDMOTOR_STATE = 0x00;
					ADF_mech_running = OFF;
					feedmotor_forbid = OFF;
					cancel_request_flg = OFF;
					if(!PaperJAMFlg)
						smech_eject_paper(); //mod.kks
                }
                else if(cio_debounced_state)  //Check the Top Cover debounce State. mod.kks 2014.06.13else if (adf_sensor_cover_is_open())
                {
                    smech_func_error("cover is open\n");
                    scerr = SCAN_ADF_TOP_COVER_OPEN;
                }
                else
                {
					scerr = smech_adf_clear_paper();
                }
                smot_step_set_location(ADF_MOTOR, inches_to_steps(SCAN_MECH_ADF, ADF_HOME_POSITION));
                smot_step_set_location(FD_MOTOR, inches_to_steps(SCAN_MECH_ADF, 0));
                
                scanplat_kernel_gpio_set_value(pinetree_mech.current_control, 0); //High Current 0.7A   mod.kks
                adf_sensor_cio_callback_disable();
				
				/* Return the CIS to Home Sensor ON state. mod.kks 2014.06.10*/
				mechType = SCAN_MECH_FLATBED; 
				scerr = smech_move_to_HS_ON();  
				if (scerr != SCANERR_NONE)
				{
					smech_func_error("smech_move_to_home error\n");
				}
				
				if(!adf_sensor_paper_in_path() &&  !adf_sensor_int_present() && !cio_debounced_state){
					if(smot_step_motor_is_idle(FD_MOTOR)){  // when the top cover open JAM to avoid the duplicate moving cmd mod.kks 2014.06.17
						smech_stopper_arrange();  //mod.kks
					}
				}
				cancel_request_flg = OFF;  //to support last page cancel cmd (when no paper state in path the app send cancel)  mode mode.kks 2014.07.23
                //MRVL-Harris-20120215 for ADF }
            }
            else // FB 
            {
                scerr = SCANERR_NONE;
                scerr = smech_wait_for_stop(FB_MOTOR);
                if (scerr != SCANERR_NONE)
                {
                    smech_func_error("smech_wait_for_stop error\n");
                }
				
				/* CIS Postion adjust in marvell. it does not use the function in PINETREE */
                if ((smech_pagedone_homing_enable == true) && (scerr == SCANERR_NONE))
                {
                    scerr = smech_find_hardstop();
                    if (scerr != SCANERR_NONE)
                    {
                        smech_func_error("smech_find_hardstop error\n");
                    }
                }
				/*End */
				
                if (scerr == SCANERR_NONE)
                {
                    scerr = smech_move_to_HS_ON();
                    if (scerr != SCANERR_NONE)
                    {
                        smech_func_error("smech_move_to_home error\n");
                    }
                }
                smech_pagedone_homing_enable = false;
                cancel_request_flg = OFF;  //Fix the timing in cancel mode mode.kks 2014.07.23
            }
            break;

        case SCAN_TARGET_TYPE_NOTCHFIND_FORWARD:
        case SCAN_TARGET_TYPE_NOTCHFIND_REVERSE:
            break;

        default:
            smech_func_error("unexpected scan target %d\n", current_scan_target_type);
            XASSERT(0, current_scan_target_type);
            break;
    }

    current_scan_target_type = SCAN_TARGET_TYPE_NONE;

    if (scerr != SCANERR_NONE)
    {
        smech_func_error("ErrCode = %d\n", scerr);
    }

    return scerr;
}


/**
 *  \brief Start a blocking motor stop (Mech API)
 *
 *  Mechanism API function to do start a motor stop, the caller will be blocked
 *  until the motor comes to a complete stop. Must not be called from IRQ
 *  context!
 *
 *  \param[in] scan_mech         Pointer to mech data structure
 **/
static void smech_pinetree_blocking_stop(t_ScanMech* scan_motor)
{
    stmotor_id_t motor_id = get_motor_id_from_mech(mechType);

    //smech_func_enter();
	
    if (mechType == SCAN_MECH_ADF)
    {
        smech_adf_clearpage_simplify = true;
        smech_set_status(SCAN_MECH_READY);
        if (smech_adf_clearpage_simplify)
        {
            //MRVL-Harris-20120215 for ADF {
            if (adf_sensor_paper_in_path())
            {
                smech_func_error("paper is in path\n");
                //smot_step_request_motor_stop(get_motor_id_from_mech(mechType));
                smot_step_request_motor_stop(ADF_MOTOR);
                smot_step_request_motor_stop(FD_MOTOR);
            }
           else if(cio_debounced_state)  //Check the Top Cover debounce State. mod.kks 2014.06.13
            {
                smech_func_error("cover is open\n");
                //smot_step_request_motor_stop(get_motor_id_from_mech(mechType));
                smot_step_request_motor_stop(ADF_MOTOR);
                smot_step_request_motor_stop(FD_MOTOR);
            }
			/*Cancel Mode Check mod.kks 2014.07.23*/
			else if(cancel_request_flg == ON){
				smech_func_error("Cancel case\n");
			    smot_step_request_motor_stop(ADF_MOTOR);
                smot_step_request_motor_stop(FD_MOTOR);
			}
            else
            {
                smech_func_error("unknown case\n");
                smot_step_request_motor_stop(get_motor_id_from_mech(mechType));
                smot_step_request_motor_stop(ADF_MOTOR);
                smot_step_request_motor_stop(FD_MOTOR);
            }
            //MRVL-Harris-20120215 for ADF }
        }
        else
        {
            //smot_step_request_motor_stop(get_motor_id_from_mech(mechType));
            smot_step_request_motor_stop(ADF_MOTOR);
            smot_step_request_motor_stop(FD_MOTOR);
        }
    }
    else
    {
        smot_step_request_motor_stop(motor_id);
        smech_wait_for_stop(motor_id);
        smech_set_status(SCAN_MECH_READY);
    }
}


/**
 *  \brief Start an async motor stop (Mech API)
 *
 *  Mechanism API function to do start a motor stop, the caller will not hang
 *  around and wait for the stop to complete (they will watch for the mech
 *  status to go to ready).
 *
 *  \param[in] scan_mech         Pointer to mech data structure
 **/
static void smech_pinetree_async_stop(t_ScanMech* scan_motor)
{
//    smech_func_enter();

    if (mechType == SCAN_MECH_ADF)
    {
        smech_adf_clearpage_simplify = true;
        smech_trig_scan_stopped(get_motor_id_from_mech(mechType));
        if (smech_adf_clearpage_simplify)
        {
            if (adf_sensor_paper_in_path())
            {
                //smot_step_request_motor_stop(get_motor_id_from_mech(mechType));
                smot_step_request_motor_stop(ADF_MOTOR);
                smot_step_request_motor_stop(FD_MOTOR);                
            }
            else
            {
                //do nothing here
            }

        }
        else
        {
            //smot_step_request_motor_stop(get_motor_id_from_mech(mechType));
            smot_step_request_motor_stop(ADF_MOTOR);
            smot_step_request_motor_stop(FD_MOTOR);
        }
    }
    else
    {
        smech_trig_scan_stopped(get_motor_id_from_mech(mechType));
        if (current_scan_target_type == SCAN_TARGET_TYPE_CAL)
        {
            //MRVL-Harris-20120302 for scan moving calibration {
            if (smech_scan_move_cal_enable)
            {
                smot_step_request_motor_stop(get_motor_id_from_mech(mechType));
            }
            //MRVL-Harris-20120302 for scan moving calibration }
        }
        else
        {
            smot_step_request_motor_stop(get_motor_id_from_mech(mechType));
        }
    }
}


/**
 *  \brief Emergency motor stop (Mech API)
 *
 *  Mechanism API function to do an emergency stop.  The motor driver will NOT
 *  allow any motor moves after this call, the system must restart.
 *
 *  \param[in] scan_mech         Pointer to mech data structure
 **/
static void smech_pinetree_halt(t_ScanMech* scan_motor)
{
    //smech_func_enter();
    
  #ifndef __KERNEL__
    gpio_write(scan_nmotor_driver_stby, 0);
  #else
    scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 0);
  #endif
    smot_step_emergency_halt(FB_MOTOR);
    smot_step_emergency_halt(ADF_MOTOR);
    smot_step_emergency_halt(FD_MOTOR);
}


/**
 *  \brief Start a motor pause (Mech API)
 *
 *  Mechanism API function to do start a motor pause.  Note that while pause/resume
 *  is only supported on the flatbed, this routine is also used to stop the motor
 *  for our ADF rest stops (done to prevent encoder rollover or motor overheat).
 *
 *  \param[in] scan_mech         Pointer to mech data structure
 **/
static void smech_pinetree_start_pause(t_ScanMech* scan_motor)
{
    const struct scanvars *scanvar;
    stmotor_id_t motor_id = INVALID_STEP_MOTOR_ID;

//    smech_func_enter();

    motor_id = get_motor_id_from_mech(mechType);
    scanvar = scanvar_get();

    smech_pause_rewind_enable = true;
    if (mechType == SCAN_MECH_ADF)
    {
        smech_start_pos = AD_LS_START_POSITION;
    }
    else
    {
        smech_start_pos = FB_REWIND_LS_START_POSITION;
    }
    smech_trig_paused_stopped(motor_id);
    if (mechType == SCAN_MECH_ADF)
    {
        smot_step_request_motor_stop(ADF_MOTOR);
        smot_step_request_motor_stop(FD_MOTOR);
    }
    else
    {
        smot_step_request_motor_stop(FB_MOTOR);
    }
}


/**
 *  \brief Scan mech self test
 *
 *  Do a quick test of the mech motors and sensors.
 **/
#define PIPECUT_MOVE_DISTANCE_ROWS     0x7fffffff
#define REQUIRED_MEMORY_FOR_NEXT_PAGE  (32*1024*1024) //MRVL-Harris-20111223
static scan_err_t smech_pinetree_selftest(t_ScanMech* scan_motor)
{
    scan_err_t scerr = SCANERR_NONE;
    bool       scan_poweron_cal_enable = false; //MRVL-Harris-20120119 for scan power-on calibration
    bool       notch_find_enable = false;
    uint32_t    profile_index;
    uint32_t    steps;

    smech_func_enter();

    smot_step_set_location(ADF_MOTOR, inches_to_steps(SCAN_MECH_ADF,0)); // ADF_HOME_POSITION));

	if(!adf_sensor_cover_is_open() && !adf_sensor_paper_in_path() && !adf_sensor_int_present())
	{
		smech_stopper_arrange();  //mod.kks
	}
	
	if(adf_sensor_cover_is_open()){
		smech_adf_paper_event_callback(SCAN_ADF_TOP_COVER_OPEN, true, 0);
		dbg1("Top Cover Open(POR)\n");
		needToStopper = ON;
		
	}
	else if(adf_sensor_paper_in_path() || adf_sensor_int_present()){
		dbg1("Jam!! (Remainder)\n");
		smech_adf_paper_event_callback(SCAN_ADF_PAPER_JAM, true, 0); //Reminder JAM
		PaperJAMFlg = ON;
	}
	else
		;
	            
//    scan_set_config_nocal(true);

    scerr = scands_set_integer("pipecut_move_distance_rows", PIPECUT_MOVE_DISTANCE_ROWS);
    XASSERT( scerr==SCANERR_NONE, scerr );

//TODO    scerr = scands_set_integer("required_memory_for_next_page", REQUIRED_MEMORY_FOR_NEXT_PAGE);
//TODO    XASSERT( scerr==SCANERR_NONE, scerr );


    //MRVL-Harris-20120110 for notchfind {
    if (notch_find_enable)
    {
        scerr = scanmech_notchfind();
        if (scerr != SCANERR_NONE)
        {
            smech_func_error("ErrCode = %d\n", scerr);
            return scerr;
        }
    }
    else
    {
		 profile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 300);
        
        smot_step_move_rel(get_motor_id_from_mech(mechType),
                           profile_index,
                           inches_to_steps(SCAN_MECH_FLATBED, 1000), 
                           MOVE_FORWARD,
                           false,
                           -1);
                           
		smech_wait_for_stop(FB_MOTOR);                           
		
        scerr = smech_find_hardstop();
        if (scerr != SCANERR_NONE)
        {
            smech_func_error("ErrCode = %d\n", scerr);
            return scerr;
        }
    }
    //MRVL-Harris-20120110 for notchfind }

    //MRVL-Harris-20120119 for scan power-on calibration {
    //scan_poweron_cal_enable = true;
    if (scan_poweron_cal_enable)
    {
        scerr = smech_poweron_cal(SCAN_CMODE_COLOR, 300);
        if (scerr != SCANERR_NONE)
        {
            return scerr;
        }

        scerr = smech_poweron_cal(SCAN_CMODE_MONO, 300);
        if (scerr != SCANERR_NONE)
        {
            return scerr;
        }

        scerr = smech_move_to_home();  //It does not use in PINETREE 
        if (scerr != SCANERR_NONE)
        {
            return scerr;
        }
    }
    //MRVL-Harris-20120119 for scan power-on calibration }

    return SCANERR_NONE;
}


/**
 * \brief TBD
 *
 *
 *
 **/
static scan_err_t smech_pinetree_poll(t_ScanMech* scan_motor)
{
    return SCANERR_NONE;
}


/**
 * \brief TBD
 *
 *
 *
 **/
//MRVL-Harris-20120215 for ADF {
static scan_err_t smech_pinetree_fault_msg(t_ScanMech *scan_mech, scan_msg_t *msg)
{
    scan_err_t      scerr = SCANERR_NONE;
    t_ScanMechType  MechType;
    uint16_t          ErrorCode;

    smech_func_enter();

    XASSERT(scan_mech != NULL, (uint32_t)scan_mech);
    XASSERT(msg != NULL,       (uint32_t)msg);

    ErrorCode = msg->param1;
    MechType = msg->param2;

    smech_func_infor("ErrorCode=%d, MechType=%d\n", ErrorCode, MechType);

    if (MechType == SCAN_MECH_ADF)
    {
        if (ErrorCode == SMECH_ADF_ERROR_CIO)
        {
            scerr = SCAN_ADF_TOP_COVER_OPEN;
        }
        else if (ErrorCode == SMECH_ADF_ERROR_JAM)
        {
            scerr = SCANERR_PAPER_JAM;
        }
        else
        {
            scerr = SCANERR_HARDWARE_FAIL;
        }
    }
    else
    {
        scerr = SCANERR_HARDWARE_FAIL;
    }
    return scerr;
}
//MRVL-Harris-20120215 for ADF }


/**
 * \brief TBD
 *
 *
 *
 **/
//MRVL-Harris-20120215 for ADF {  //Send JAM infomation to APP  mod.kks  
static scan_err_t smech_pinetree_force_failure(t_ScanMech *scan_mech, smech_failure_t fail_code)
{
    smech_func_enter();
    PaperJAMFlg = ON; 
    
    if(fail_code==SMECH_FAILURE_ADF_NO_PICKUP_JAM)  //J201
    {
		smech_adf_paper_event_callback(SCAN_ADF_PAPER_NOPICK, true, 0);  //TBD Not Ready system.
	}
    else if (fail_code==SMECH_FAILURE_ADF_PAPER_JAM) //J202
    {
        smech_adf_paper_event_callback(SCAN_ADF_PAPER_JAM, true, 0);
    }
    else if (fail_code==SMECH_FAILURE_SHORT_PAPER)
    {
        smech_adf_paper_event_callback(SCAN_ADF_PAPER_SHORT, true, 0);
    }
    else if(fail_code==SMECH_FAILURE_ADF_TOPCOVER_OPEN_JAM)   //Top Cover Open Error mod.kks 2014.06.13
	{
		smech_adf_paper_event_callback(SCAN_ADF_TOP_COVER_OPEN, true, 0);
	}
    return SCANERR_NONE;
}
//MRVL-Harris-20120215 for ADF }


/**
 *  \brief Set mech sleep mode
 *
 *  Scan power may be going away or coming back on: make sure the mech is
 *  ready.
 *
 *  \param sleep_mode - true: enter sleep mode, false: exit sleep mode
 *
 *  \return scan_err_t
 *  \retval SCANERR_NONE   Sleep mode set, no errors
 **/
static scan_err_t smech_pinetree_sleep(t_ScanMech* scan_motor, bool sleep_mode)
{
    return SCANERR_NONE;
}


/**
 * \brief TBD
 *
 *
 *
 **/
static void smech_pinetree_get_min_line_time(uint32_t scan_line_time_usec, uint32_t *mech_line_time_usec)
{
    const struct scanvars *scanvar;

    smech_func_enter();

    scanvar = scanvar_get();

    *mech_line_time_usec = 0;
#if 0 //MRVL-Harris 20121031
    if (mechType == SCAN_MECH_FLATBED)
    {
        if (scanvar->hw_dpi_vert == 300)
        {
            if (scanvar->cmode == SCAN_CMODE_COLOR)
            {
                *mech_line_time_usec = 576;
            }
            else
            {
                *mech_line_time_usec = 1728;
            }
        }
    }
#endif //MRVL-Harris 20121031
}


/**
 * \brief TBD
 *
 *
 *
 **/
static bool smech_pinetree_is_motor_overheating(t_ScanMech *scan_motor)
{
    return false;
}


static scan_err_t smech_pinetree_ioctl(t_ScanMech *scan_mech, uint32_t request, int arg, void *ptr)
{
    scan_err_t scerr;
    struct scan_adf_margins *adf_margins = NULL;
    struct scan_flatbed_margins *fb_margins;

    /*
     *   Note! Note! Note! 
     *
     *   This function can be called from any thread context!  Do not assume
     *   the caller is running in scantask thread context.
     *
     *   Dangerous or long running operations (such as motor moves) will be
     *   triggered via a SMIRB (see scanman_smirb.c). Other operations (such as
     *   getting/setting margins) can be called from any thread context.
     *
     */

//    smech_func_enter();

    switch( request ) {
        case SCANMECH_IOCTL_MOVE_TO_CAL :
            break;

        case SCANMECH_IOCTL_MOVE_TO_HOME :
            scerr = smech_move_to_home();
            return (scerr);
            break;
            
        case SCANMECH_IOCTL_MOVE_TO_HS_ON :
            scerr = smech_move_to_HS_ON();
            return (scerr);
            break;
            
        case SCANMECH_IOCTL_NOTCHFIND :
            scerr = scanmech_notchfind();
            return (scerr);
            break;
		/* A3 DLT Merge Mech */
		case SCANMECH_IOCTL_MOVE_TO_A3POSITION :
            if( arg == A3_EJECT ) {
				scerr = smech_move_to_A3scanpoint(A3_EJECT, 216);
	         }
            else if( arg == A3_2ndSCAN_READY ) {
				scerr = smech_move_to_A3scanpoint(A3_2ndSCAN_READY, 216);				
            }
            else if( arg == DLT_1stSCAN_READY ) {
				scerr = smech_move_to_A3scanpoint(DLT_1stSCAN_READY, 11);	// By mechnical team request	mod.kks 2014.08.22
            }
            else if( arg == DLT_2ndSCAN_READY ) {
				scerr = smech_move_to_A3scanpoint(DLT_2ndSCAN_READY, 216);				
            }
            else if( arg == DLT_EJECT ) {
				scerr = smech_move_to_A3scanpoint(DLT_EJECT, 222);				
            }
            else { 
				DBG_PRINTF_SDH_DEBUG("smech_pinetree_ioctl : SCANERR_INVALID_PARAM \n");
                scerr = SCANERR_INVALID_PARAM;
            }
            break;

        case SCANMECH_IOCTL_HARDSTOP :
            scerr = smech_find_hardstop();
            return (scerr);
            break;

        case SCANMECH_IOCTL_GET_FLATBED_MARGINS :
            scerr = SCANERR_NONE;

            if( arg != sizeof(struct scan_flatbed_margins) ) {
                return SCANERR_INVALID_PARAM;
            }

            fb_margins = (struct scan_flatbed_margins *)ptr;
            scerr = scanmargin_get_flatbed_margins( g_flatbed_margins, fb_margins->dpi, fb_margins->cmode, fb_margins );
            return scerr;
            break;

        case SCANMECH_IOCTL_GET_ADF_MARGINS :
            scerr = SCANERR_NONE;

            if( arg != sizeof(struct scan_adf_margins) ) {
                return SCANERR_INVALID_PARAM;
            }

            adf_margins = (struct scan_adf_margins *)ptr;

            scerr = scanmargin_get_adf_margins( g_adf_margins, adf_margins->dpi, adf_margins->cmode, adf_margins );
            return scerr;
            break;

        case SCANMECH_IOCTL_DEBUG_SET_ADF_MARGINS :
            scerr = SCANERR_NONE;

            if( arg != sizeof(struct scan_adf_margins) ) {
                return SCANERR_INVALID_PARAM;
            }

            adf_margins = (struct scan_adf_margins *)ptr;

            scerr = scanmargin_debug_set_adf_margins( g_adf_margins, adf_margins );
            return scerr;
            break;

        case SCANMECH_IOCTL_DEBUG_SET_FLATBED_MARGINS :
            scerr = SCANERR_NONE;

            if( arg != sizeof(struct scan_flatbed_margins) ) {
                return SCANERR_INVALID_PARAM;
            }

            fb_margins = (struct scan_flatbed_margins *)ptr;

            scerr = scanmargin_debug_set_fb_margins( g_flatbed_margins, fb_margins );
            return scerr;
            break;

        case SCANMECH_IOCTL_PAPER_JAM_RECOVER:
            //under building
            scerr = SCANERR_NONE;
            return scerr;
            break;

        case SCANMECH_IOCTL_ADF_CLIENT_PAUSE :
            /* davep 24-Jan-2013 ; adding client pause/resume, a feature where
             * downstream code can request the ADF stop at the next page gap.
             * The client code (e.g., copyapp or scanapp) is responsible for
             * clearing the pause condition so the ADF scan can resume.
             */
            /* TODO check if we're running an ADF scan. If we're not running an
             * ADF scan, return SCANERR_INVALID
             */
            if( arg==1 ) {
                scerr = adf_page_gapper_client_pause();
				cancel_request_flg=ON;
            }
            else if( arg==0 ) {
                scerr = adf_page_gapper_client_resume();
            }
            else { 
                scerr = SCANERR_INVALID_PARAM;
            }
            return scerr;
            break;
		/*SP Mode mode.kks 2014.06.13*/
		case SCANMECH_IOCTL_SP_FB_FREE_RUN :
            SP_OUT_check_flg = ON;
            smech_fb_selftest(); // Move to A4 Scan end porint and then move to home ON.
            SP_OUT_check_flg = OFF;
            break;
            
        case SCANMECH_IOCTL_SP_ADF_FREE_RUN :    
			SP_OUT_check_flg = ON;
            scerr = smech_adf_test_func(); //Mono 300 ADF Free Run until No paper.
            SP_OUT_check_flg = OFF;
            return scerr;
            break;
            
        case SCANMECH_IOCTL_SP_FEED_MOTOR :    				
			SP_OUT_check_flg = ON;
            smech_feed_motor_run(arg); //arg => 1:Forward Modo 300  2:Mono 600 3: Color 300 4: Color 600 5:Reverse Mono 300 6:Mono 600 7:Color 300 8:Color 600
            SP_OUT_check_flg = OFF;
            break;
            
         case SCANMECH_IOCTL_SP_TRANS_MOTOR :   
            SP_OUT_check_flg = ON;
            smech_trans_motor_run(arg); //arg => 1:Forward Modo 300  2:Mono 600 3: Color 300 4: Color 600 5:Reverse Mono 300 6:Mono 600 7:Color 300 8:Color 600
            SP_OUT_check_flg = OFF;
            break;
        
        /* Sensor Check Input Mode mod.kks 2014.06.13 */						
        case SCANMECH_IOCTL_SP_PRESENT_SENSOR :   
        case SCANMECH_IOCTL_SP_INTERVAL_SENSOR :   
        case SCANMECH_IOCTL_SP_1STSCAN_SENSOR :   
        case SCANMECH_IOCTL_SP_TOPCOVER_SENSOR :   
        case SCANMECH_IOCTL_SP_HOMESENSOR:
			if(arg == 1) // To no effect in normal mode.   arg => 1: SP INPUT Mode 0: Normal Mode.  mod.kks
				SP_IN_check_flg = ON;
			else
				SP_IN_check_flg = OFF;
			break;
		/*Sensor Check END */
		
		/* SP Mode END*/				
		
		/* Sleep Mode */
		case SCANMECH_IOCTL_SLEEP_MODE:
#ifndef __KERNEL__
			gpio_write(scan_nmotor_driver_stby, 0);
#else
			scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 0); // Motor Driver Sleep Mode to save power. mod.kks 2014.08.07
#endif
			sleep_mode_state = ON;  //need to the stable seneor state mod.kks 2014.08.22
			dbg1("MECH Sleep Mode enter \n");
			smech_adf_paper_event_callback(SCAN_ADF_POWERSAVE_DONE, true, 0);  //Mech Power Save Mode Done mod.kks 2014.09.15
			break;
		
		/* Wakeup Mode */
		case SCANMECH_IOCTL_WAKE_UP_MODE:
#ifndef __KERNEL__
			gpio_write(scan_nmotor_driver_stby, 1);
#else
			scanplat_kernel_gpio_set_value(pinetree_mech.scan_nmotor_driver_stby, 1); // Motor Driver wake up. mod.kks 2014.08.07
#endif
			mdelay(1); //To motor driver spec 1ms. mod.kks
			sleep_mode_state = OFF; //need to the stable seneor state mod.kks 2014.08.22
			dbg1("MECH Wake-up  \n");
			smech_adf_paper_event_callback(SCAN_ADF_WAKEUP_DONE, true, 0);  //Mech Power Save Mode Done mod.kks 2014.09.24
			break;	
			
        default :
            return SCANERR_NOT_IMPLEMENTED;
    }

    return SCANERR_NONE;
}

/**
 * \brief TBD
 *
 *
 *
 **/
//static scan_err_t smech_pinetree_adf_duplex_flip_page(t_ScanMech *scan_mech)
//{
//    return SCANERR_NONE;
//}


/**
 * \brief TBD
 *
 *
 *
 **/
//static scan_err_t smech_pinetree_adf_duplex_eject(t_ScanMech *scan_mech)
//{
//    return SCANERR_NONE;
//}


/**
 * \brief TBD
 *
 *
 *
 **/
//static scan_err_t smech_pinetree_adf_duplex_stage_input_sheet(t_ScanMech *scan_mech)
//{
//    return SCANERR_NONE;
//}


/**
 * \brief TBD
 *
 *
 *
 **/
//static scan_err_t smech_pinetree_ioctl(t_ScanMech *scan_mech, uint32_t request, int arg, void *ptr)
//{
//    return SCANERR_NONE;
//}


/**
 *  \brief mech driver power on init
 *
 *  must be called once at startup before using any other mech driver interfaces.
 *
 *  \param[in] scan_mech         pointer to mech data structure
 **/
void smech_pinetree_init(t_ScanMech *scan_mech)
{
    int err;
    int retcode;

    //register a scan mech debug command line
    retcode = 0;
#ifdef HAVE_DBG_CALLBACK_CMDS
    retcode = cmd_register_cmd("smech", NULL, NULL, NULL, NULL, smech_pinetree_debug_callback);
    XASSERT( retcode==0, retcode );
#endif

    scan_mech->pending_high_water = 800;
    scan_mech->pending_low_water = 240;

    scan_mech->mech_status = SCAN_MECH_ERROR;

    scan_mech->scan_setup_fcn = smech_pinetree_scan_setup;
    scan_mech->scan_page_prepare_fcn = smech_pinetree_page_prepare;
    scan_mech->smot_scan_start_fcn = smech_pinetree_scan_start;
    scan_mech->scan_page_done_fcn = smech_pinetree_page_done;
    scan_mech->smot_scan_blocking_stop_fcn = smech_pinetree_blocking_stop;
    scan_mech->smot_scan_async_stop_fcn = smech_pinetree_async_stop;
    scan_mech->smot_scan_halt_fcn = smech_pinetree_halt;
    scan_mech->smot_scan_start_pause_fcn = smech_pinetree_start_pause;
    scan_mech->smech_selftest_fcn = smech_pinetree_selftest;
    scan_mech->smech_notchfind_fcn = smech_pinetree_notchfind;
    scan_mech->smech_poll_fcn = smech_pinetree_poll;
    scan_mech->smech_fault_msg_fcn = smech_pinetree_fault_msg;
    scan_mech->smech_force_failure_fcn = smech_pinetree_force_failure;
    scan_mech->smech_sleep_fcn = smech_pinetree_sleep;
    scan_mech->smech_get_min_line_time_fcn = smech_pinetree_get_min_line_time;
    scan_mech->smech_is_motor_overheating_fcn = smech_pinetree_is_motor_overheating;
    //scan_mech->smech_adf_duplex_flip_page_fcn = smech_pinetree_adf_duplex_flip_page;
    //scan_mech->smech_adf_duplex_eject_fcn = smech_pinetree_adf_duplex_eject;
    //scan_mech->smech_adf_duplex_stage_input_sheet_fcn = smech_pinetree_adf_duplex_stage_input_sheet;
    scan_mech->smech_ioctl_fcn = smech_pinetree_ioctl;
    scan_mech->smech_adf_duplex_flip_page_fcn = NULL;
    scan_mech->smech_adf_duplex_eject_fcn = NULL;
    scan_mech->smech_adf_duplex_stage_input_sheet_fcn = NULL;
    scan_mech->smot_scan_SC_halt_fcn = smech_SC_halt;   //To stop the motor and clutch in Soc SC Error   mod.kks.  2014. 06.02

    scan_set_scan_mech(scan_mech);

//#ifndef __KERNEL__
    //init 3 motor drivers
    smot_step_init(3);
//#endif

    //create flatbed motor
    FB_MOTOR = smot_step_create_motor(stmotor_fb_dpi_profiles,
                                      &stmotor_fb_pin_connects,
                                      stmotor_fb_sequence,
                                      FB_NRAMP_VALS,
                                      FB_MAX_CMD_STEPS,
                                      NULL);

    XASSERT(FB_MOTOR != INVALID_STEP_MOTOR_ID, FB_MOTOR);
    smech_func_infor("FB_MOTOR ID = %d\n", FB_MOTOR);

    //create adf motor
    ADF_MOTOR = smot_step_create_motor(stmotor_adf_dpi_profiles,
                                       &stmotor_adf_pin_connects,
                                       stmotor_adf_sequence,
                                       AD_NRAMP_VALS,
                                       AD_MAX_CMD_STEPS,
                                       NULL);

    XASSERT(ADF_MOTOR != INVALID_STEP_MOTOR_ID, ADF_MOTOR);
    smech_func_infor("ADF_MOTOR ID = %d\n", ADF_MOTOR);

    //create feed motor
    FD_MOTOR = smot_step_create_motor(stmotor_fd_dpi_profiles,
                                      &stmotor_fd_pin_connects,
                                      stmotor_fd_sequence,
                                      FD_NRAMP_VALS,
                                      FD_MAX_CMD_STEPS,
                                      NULL);

    XASSERT(FD_MOTOR != INVALID_STEP_MOTOR_ID, FD_MOTOR);
    smech_func_infor("FD_MOTOR ID = %d\n", FD_MOTOR);

#ifndef __KERNEL__
    smech_timer = delay_msec_timer_non_blocking(1,  /* 1ms timer interval */
                                                smech_adf_timer_callback, /* Callback function  */
                                                NULL,                     /* Callback data      */
                                                true);                    /* Repeating          */

    XASSERT(smech_timer != NULL, (uint32_t)smech_timer);
#else
    init_timer(&smech_timer);
    smech_timer.expires = jiffies + MILLISEC(1);
    smech_timer.data = 0;
    smech_timer.function = &smech_adf_timer_callback;
    add_timer(&smech_timer);
#endif

    //create scan home sensor GPIO
#ifndef __KERNEL__
    err = gpio_open(&scan_home_sensor_gpio, GPIO_BANK_F, 9,
                    GPIO_DIRECTION_INPUT,
                    GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0);
    XASSERT(err==OK, err);

    err = gpio_open(&scan_nmotor_driver_stby, GPIO_BANK_D, 21,
                    GPIO_DIRECTION_OUTPUT,
                    GPIO_LOGIC_SETTING_ACTIVE_HIGH, 1);
    XASSERT(err==OK, err);
  #ifdef HAVE_PINETREE_FORM_FACTOR
    gpio_write(scan_nmotor_driver_stby, 1);
  #endif

    err = gpio_open(&scan_nmotor_driver_rst,  GPIO_BANK_C, 17,
                    GPIO_DIRECTION_OUTPUT,
                    GPIO_LOGIC_SETTING_ACTIVE_HIGH, 1);
    XASSERT(err==OK, err);
  #ifdef HAVE_PINETREE_FORM_FACTOR
    gpio_write(scan_nmotor_driver_rst, 1);
  #endif
  
/*	err = gpio_open(&fb_enable, GPIO_BANK_D, 26,
				GPIO_DIRECTION_OUTPUT,
				GPIO_LOGIC_SETTING_ACTIVE_HIGH, 1);
	XASSERT(err==OK, err);*/
#else
    /* davep 24-Sep-2013 ; move GPIO and DMA function calls into a GPL module */
    memset( &pinetree_mech, 0, sizeof(struct pt_mech) );
    err = smech_pinetree_kernel_init(&pinetree_mech);
    XASSERT(err==0,err);
#endif

    adf_sensor_onetime_init();

    smech_pause_rewind_enable = false;
    smech_pagedone_homing_enable = false;
    smech_adf_clearpage_simplify = false;
    smech_adf_inputpage_simplify = true;
    smech_scan_move_cal_enable = true; //MRVL-Harris-20120302 for scan moving calibration
    smech_set_status(SCAN_MECH_READY);

	/* Home Position Detection */
	err = scos_sem_init(&hp_sem,"hp_detection",0);	// add.lsh.for SC219
	XASSERT( err==SCANERR_NONE, err ); 
    
    bScanEC = false;
    
	DBG_PRINTF_SDH_DEBUG( "smech_pinetree_init : Done \n");
    
}


#ifdef HAVE_DBG_CALLBACK_CMDS
static void smech_show_help_mech(uint32_t level)
{
    smech_func_helpu("mech: set debugMechType\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech mech <type>\n");
        smech_func_helpu("  type: 0 = FLATBED, 1 = ADF\n");
    }
}


static void smech_show_help_adfinput(uint32_t level)
{
    smech_func_helpu("adfinput: adf input paper\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech adfinput\n");
    }
}


static void smech_show_help_adftest(uint32_t level)
{
    smech_func_helpu("adftest: adf test function\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech adftest\n");
    }
}


static void smech_show_help_adfclear(uint32_t level)
{
    smech_func_helpu("adfclear: adf clear paper\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech adfclear\n");
    }
}


static void smech_show_help_sensor(uint32_t level)
{
    smech_func_helpu("sensor: read smech sensor\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech sensor <type>\n");
        smech_func_helpu("  type: 0 = FLATBED sensor for home is found (HIF)\n");
        smech_func_helpu("  type: 1 = ADF sensor for cover is open (CIO)\n");
        smech_func_helpu("  type: 2 = ADF sensor for paper present (PP)\n");
        smech_func_helpu("  type: 3 = ADF sensor for paper in path (PIP)\n");
        smech_func_helpu("  type: 4 = ADF sensor for interval present (INP) \n");
        smech_func_helpu("  type: 5 = all ADF sensors\n");
        smech_func_helpu("  type: 6 = all sensors\n");
    }
}


static void smech_show_help_moveto(uint32_t level)
{
    smech_func_helpu("moveto: move to position\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech moveto <pos> <wait>\n");
        smech_func_helpu("   pos: move to position, unit is 0.001 inch\n");
        smech_func_helpu("  wait: 0 = do not wait for stop\n");
        smech_func_helpu("        1 = wait for stop\n");
    }
}


static void smech_show_help_move(uint32_t level)
{
    smech_func_helpu("move: send a move command to scanmot\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech move <mode> <dpi> <dist> <dir> <wait>\n");
        smech_func_helpu("  mode: mono = 1, color = 2\n");
        smech_func_helpu("  dpi:  150, 300, 600 or 12000\n");
        smech_func_helpu("  dist: move distance, unit is 0.001 inch\n");
        smech_func_helpu("  dir:  1 = forward, 0 = reverse\n");
        smech_func_helpu("  wait: 0 = do not wait for stop\n");
        smech_func_helpu("        1 = wait for stop\n");
    }
}


static void smech_show_help_stop(uint32_t level)
{
    smech_func_helpu("stop: send a stop command to scanmot\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech stop\n");
    }
}


static void smech_show_help_hardstop(uint32_t level)
{
    smech_func_helpu("hardstop: send a hardstop command to scanmech flatbed\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech hardstop\n");
    }
}


//MRVL-Harris-20120105 for notchfind start
static void smech_show_help_notchfind(uint32_t level)
{
    smech_func_helpu("hardstop: send a notchfind command to scanmech flatbed\n");
    if (level != 0)
    {
        smech_func_helpu(" Usage: smech notchfind\n");
    }
}
//MRVL-Harris-20120105 for notchfind end

/* Add the Scanner Free Run Mode of SP Function mod.kks 2014.06.12*/
void smech_fb_selftest(void)
{
	uint32_t    profile_index;
	scan_err_t scerr = SCANERR_NONE;
	
    scerr = smech_move_to_home();  
    if (scerr != SCANERR_NONE)
    {
        dbg1("[SP Mode] move to home fail.  Please check FB motor and harness. \n");
    }
     
    smech_start_pos = inches_to_steps(SCAN_MECH_FLATBED, FB_TOP_BED_POSITION);
    profile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 300); //using middle speed
	smot_step_move_rel(FB_MOTOR,
							   smech_profile_index,
							   smech_start_pos+28009,         //Until  297 mm moving    1step = 0.021207294 (2- clk)
							   MOVE_FORWARD,
							   false,
							   -1);  

	smech_wait_for_stop(FB_MOTOR);	
	
	scerr = smech_move_to_HS_ON();					   
	if (scerr != SCANERR_NONE)
    {
        dbg1("[SP Mode] move to HS ON fail.  Please check FB motor and harness. \n");
    }
}
/* Scanner Free Run Mode END */

/*Feed Motor in SP Mode */
static void smech_feed_motor_run(int target)
{
	/*previous motor state check*/
	smech_wait_for_stop(FD_MOTOR);	

	/*SP Mode Setting */
	SP_OUT_check_flg= ON;
	
	/*Speed, Step, direction setting*/ 
	smech_value_To_speed(target);
	
	/*FEED Motor run*/
	smot_step_move_rel(FD_MOTOR,
						   SPprofile_index,
						   SPsteps,         
						   SPdirection,
						   false,
						   -1);  					   
						   
    /*wait for stop*/						
	smech_wait_for_stop(FD_MOTOR);	
	
	SP_OUT_check_flg = OFF;						   
}
/* Feed Motor End */

/*Transport Motor in SP Mode mod.kks 2014.06.12 */
static void smech_trans_motor_run(int target)
{
	/*previous motor state check*/
	smech_wait_for_stop(ADF_MOTOR);	

	/*SP Mode Setting */
	SP_OUT_check_flg= ON;
	
	/*Speed, Step, direction setting*/ 
	smech_value_To_speed(target);
	
	/*Trans port Motor run*/
	smot_step_move_rel(ADF_MOTOR,
						   SPprofile_index,
						   SPsteps,         
						   SPdirection,
						   false,
						   -1);  					   
						   
    /*wait for stop*/						
	smech_wait_for_stop(ADF_MOTOR);	
	
	SP_OUT_check_flg = OFF;						
}
/* Transport Motor End */

/* Change the value to speed mod.kks 2014.06.12 */
static void smech_value_To_speed(int target)
{
	switch(target){
		case 1:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 300); 
			SPdirection = MOVE_FORWARD;
			SPsteps = 10000;
		break;
		
		case 2:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 600); 
			SPdirection = MOVE_FORWARD;
			SPsteps = 5000;
		break;
		
		case 3:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 300); 
			SPdirection = MOVE_FORWARD;
			SPsteps = 10000;
		break;			
		
		case 4:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600); 
			SPdirection = MOVE_FORWARD;
			SPsteps = 5000;
		break;
		
		case 5:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 300); 
			SPdirection = MOVE_REVERSE;
			SPsteps = 10000;
		break;
		
		case 6:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_MONO, 600); 
			SPdirection = MOVE_REVERSE;
			SPsteps = 5000;
		break;
		
		case 7:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 300); 
			SPdirection = MOVE_REVERSE;
			SPsteps = 10000;
		break;
		
		case 8:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600); 
			SPdirection = MOVE_REVERSE;
			SPsteps = 5000;
		break;			
		
		default:
			SPprofile_index = get_stmotor_profile_index(SCAN_CMODE_COLOR, 600); 
			SPdirection = MOVE_REVERSE;
			dbg1("Invalid value Check mode value \n");
        break;
	}
}
/*Trans the value to speed */

static void smech_show_help(void)
{
    smech_func_helpu("\nstepper debug command list\n");
    smech_show_help_mech(0);
    smech_show_help_sensor(0);
    smech_show_help_move(0);
    smech_show_help_stop(0);
    smech_show_help_hardstop(0);
    smech_show_help_adfinput(0);
    smech_show_help_adftest(0);
    smech_show_help_adfclear(0);
    smech_show_help_moveto(0);
    smech_show_help_notchfind(0); //MRVL-Harris-20120105 for notchfind
}


//scan mech debug command line
static int smech_pinetree_debug_callback( int argc, char *argv[] )
{
    int    i, pos;
    int    retcode;
    uint32_t num32;
    t_ScanMechType currMechType;

    smech_func_enter();
    for (i=0; i<argc; i++)
    {
        smech_func_infor("argv[%d] = %s\n", i, argv[i]);
    }

    if ( argc < 2 )
    {
        smech_show_help();
        return 0;
    }

    currMechType = mechType;
    mechType = debugMechType;

    if (mechType == SCAN_MECH_ADF)
    {
        smech_func_infor("debugMechType = ADF\n");
    }
    else
    {
        smech_func_infor("debugMechType = FLATBED\n");
    }

    pos = 1;

    if (strncmp(argv[pos], "moveto", 7) == 0)
    {
        uint32_t tarpos;
        uint32_t wait;
        if (argc != 4)
        {
            smech_show_help_moveto(1);
            goto fail;
        }

        pos++;
        str_mkint(argv[pos++], &wait);
        str_mkint(argv[pos++], &tarpos);
#if 0        
        mechType = SCAN_MECH_ADF;
        smech_move_to_A3scanpoint(wait, tarpos);
        dbg1("direction %d , distance_mm %d \n", wait, tarpos);
 #else
		mechType = SCAN_MECH_FLATBED;
        if (wait == 0)
        {
            smech_move_to_position((int)tarpos, false);
        }
        else
        {
            smech_move_to_position((int)tarpos, true);
        }
#endif        
    }
     else if(strncmp(argv[pos], "fbselftest", 10) == 0)
    {
        if (argc != 2)
        {
            smech_show_help_hardstop(1);
            goto fail;
        }
        smech_fb_selftest();
    }
    else if (strncmp(argv[pos], "adfclear", 9) == 0)
    {
        if (argc != 2)
        {
            smech_show_help_adfclear(1);
            goto fail;
        }
        else
        {
            mechType = SCAN_MECH_ADF;
            smech_adf_clear_paper();
        }
    }
    else if (strncmp(argv[pos], "adftest", 8) == 0)
    {
        if (argc != 2)
        {
            smech_show_help_adftest(1);
            goto fail;
        }
        else
        {
            mechType = SCAN_MECH_ADF;
            smech_adf_test_func();
        }
    }
    else if (strncmp(argv[pos], "adfinput", 9) == 0)
    {
        if (argc != 2)
        {
            smech_show_help_adfinput(1);
            goto fail;
        }
        else
        {
            mechType = SCAN_MECH_ADF;
            smech_adf_input_paper();
        }
    }
    else if (strncmp(argv[pos], "mech", 5) == 0)
    {
        if (argc != 3)
        {
            smech_show_help_mech(1);
            goto fail;
        }

        pos++;
        retcode = str_mkint(argv[pos++], &num32);
        if( retcode != 0 ) {
            goto fail;
        }

        if (num32 == 0)
        {
            debugMechType = SCAN_MECH_FLATBED;
            smech_func_infor("debugMechType was set to FLATBED\n");
        }
        else if (num32 == 1)
        {
            debugMechType = SCAN_MECH_ADF;
            smech_func_infor("debugMechType was set to ADF\n");
        }
        else
        {
            smech_func_error("unknown debugMechType\n");
        }
    }
    else if(strncmp(argv[pos], "hardstop", 9) == 0)
    {
        if (argc != 2)
        {
            smech_show_help_hardstop(1);
            goto fail;
        }
        mechType = SCAN_MECH_FLATBED;
        smech_find_hardstop();
    }
    //MRVL-Harris-20120105 for notchfind start
    else if(strncmp(argv[pos], "notchfind", 10) == 0)
    {
        //uint32_t  req_id;

        if (argc != 2)
        {
            smech_show_help_notchfind(1);
            goto fail;
        }
        // mechType = SCAN_MECH_FLATBED;
        // scanman_dev_request(SCANMECH_IOCTL_NOTCHFIND, 0, 0, &req_id);
        if (scanmech_notchfind() != SCANERR_NONE)
            smech_func_infor("NOTCH NOT FOUND\n");
        else
            smech_func_infor("NOTCH FOUND!\n");
    }
    //MRVL-Harris-20120105 for notchfind end
    else if(strncmp(argv[pos], "stop", 5) == 0)
    {
        if (argc != 2)
        {
            smech_show_help_stop(1);
            goto fail;
        }
        if (mechType == SCAN_MECH_ADF)
        {
            smot_step_request_motor_stop(ADF_MOTOR);
        }
        else
        {
            smot_step_request_motor_stop(FB_MOTOR);
        }
    }
    else if(strncmp(argv[pos], "move", 5) == 0)
    {
        uint32_t cmode;
        uint32_t dpi;
        uint32_t dist;
        uint32_t dir;
        uint32_t profile_index;
        uint8_t  count = 0;
        uint32_t wait = 0;

        if (argc != 7)
        {
            smech_show_help_move(1);
            goto fail;
        }
        pos++;
        str_mkint(argv[pos++], &cmode);
        str_mkint(argv[pos++], &dpi);
        str_mkint(argv[pos++], &dist);
        str_mkint(argv[pos++], &dir);
        str_mkint(argv[pos++], &wait);

        if ((cmode!=SCAN_CMODE_MONO)&&(cmode!=SCAN_CMODE_COLOR))
        {
            smech_func_error("cmode must be either SCAN_CMODE_MONO (%d) or SCAN_CMODE_COLOR (%d)\n", SCAN_CMODE_MONO, SCAN_CMODE_COLOR);
            goto fail;
        }
        if ((dir!=MOVE_FORWARD)&&(dir!=MOVE_REVERSE))
        {
            smech_func_error("dir must be either MOVE_FORWARD (%d) or MOVE_REVERSE (%d)\n", MOVE_FORWARD, MOVE_REVERSE);
            goto fail;
        }
        profile_index = get_stmotor_profile_index(cmode, dpi);
        smot_step_move_rel(get_motor_id_from_mech(mechType), profile_index, inches_to_steps(mechType, dist), dir, true, 1);
        if (wait != 0)
        {
            while(1)
            {
                count++;
                if (count > 20)
                {
                    count = 0;
                    if (smot_step_motor_is_idle(get_motor_id_from_mech(mechType)))
                    {
                        break;
                    }
                }
            }
        }
    }
    else if (strncmp(argv[pos], "sensor", 7) == 0)
    {
        if (argc != 3)
        {
            smech_show_help_sensor(1);
            goto fail;
        }
        pos++;
        retcode = str_mkint(argv[pos++], &num32);
        if( retcode != 0 ) {
            goto fail;
        }
        if (num32 == 0) {
            smech_func_infor("HIF=%d\n", flatbed_sensor_home_is_found());
        }
        else if (num32 == 1) {
            smech_func_infor("CIO=%d\n", adf_sensor_cover_is_open());
        }
        else if (num32 == 2) {
            smech_func_infor("PP=%d\n",  adf_sensor_paper_present());
        }
        else if (num32 == 3) {
            smech_func_infor("PIP=%d\n", adf_sensor_paper_in_path());
        }
        else if (num32 == 4) {
            smech_func_infor("INT=%d\n", adf_sensor_int_present());
        }
        else if (num32 == 5)
        {
            smech_func_infor("CIO=%d PP=%d PIP=%d INP=%d\n", adf_sensor_cover_is_open(),
                                                             adf_sensor_paper_present(),
                                                             adf_sensor_paper_in_path(),
                                                             adf_sensor_int_present());
        }
        else if (num32 == 6)
        {
            smech_func_infor("HIF=%d CIO=%d PP=%d PIP=%d INP=%d\n", flatbed_sensor_home_is_found(),
                                                                    adf_sensor_cover_is_open(),
                                                                    adf_sensor_paper_present(),
                                                                    adf_sensor_paper_in_path(),
                                                                    adf_sensor_int_present());
        }
        else
        {
            smech_func_error("unknown sensor type\n");
        }
    }
    else
    {
        smech_show_help();
    }

fail:
    mechType = currMechType;

    return 0;
}
#endif //HAVE_DBG_CALLBACK_CMDS


