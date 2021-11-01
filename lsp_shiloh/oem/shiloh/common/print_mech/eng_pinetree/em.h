/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **								    								
 **	 File Name :	em.h					    				
 **	 Revision  :	ver 1.4								    			
 **	 Date	   : 	Dec 21, 2009										
 **	 Author    :	Youngjin Lee		
 +-----------------------------------------------------------------+*/
 
 
#include "gpio_api.h"
#include "dcmotor_ol.h" // dcmotor_openloop.h
#include "pwm_api.h"
#include "dec_sensor.h"
#include "dec_fuser.h"
#include "dec_laser.h"
#include "dec_adc_api.h"
#include "video_laser_api.h"

#include <stdbool.h>
#include "memAPI.h"
#include "delay_api.h"
#include "logger.h"
#include "debug.h"
#include "utils.h"
#include "print_status_codes.h"
#include "engine_config.h"

#include "sys_init_api.h"
#include "lassert.h"
#include "agMessage.h"

#include "print_job_types.h"
#include "delay_api.h"
#include "spec.h"

#ifndef __EM_H
#define __EM_H

#if MAC_TYPE_M
#define SW_ENGINE_VERSION 		"9.3.9"
#else
#define SW_ENGINE_VERSION 		"7.3.9"
#endif
#define SW_DESCRIPTION 			"Official Release Version"

#define	MARGIN_ADJUST 	1	/* 0->NO USE/1->USE : @@2013.01.08. By CSPark */
#define	SMART_IC_TONER	1	/* 0->NO USE/1->USE : @@2013.01.10. By CSPark */

#define FULLSTACK_CHECK 0
#define IPG_ENABLE 1
#define TEST_PIN 0	//Test GPIO_E17  R1505
#define TEST_PIN2 0	//Test GPIO_E18  R1506
#define PWS_TO_PRINT 1
#define MOTOR_SPEED_MODE 1
#define NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE 0
#define SW_DESC_ICJIG	"Engine Version For IC Jig"
#define SMART_IC_LOCK_ENABLE 1
#define MPT_SENSOR_PIN_CHG 1
#define FANDELAY_IN_STANDBY 0
#define TONER_LOCKUP_13750 0
#define TONER_LOCKUP_14250 1

#define DEBUG_EXIT_MOTOR_SPEED 1
#define EXIT_MOTOR_SPEED 1
#define HARDWARETEST 1

#define PAGE_COMPLETE	1
#define REJECT_PRINT	1
#define EC1_MICR	1	
#define MPF_CORRECT	1
#define	NEARFULL_COUNT	1
#define FULL_JAM	1
#define FULL_DUPLEX	1
#define REGAL_UPDATE	1
#define FULL_EMPTY	1
#define MODE_OF_TEMP_10_20	0
#define MODE_OF_TEMP_10_15	1
#define ROUGH_PAPER_SAME_LABEL	1
#define OVER_SENSOR	0
#define STAIR_FUSER	1
#define POOR_FUSER	1
#define LSU_932		1
#define HOME		1

#define SLOWDOWN_THRESHOLD	10
#define	SPEEDUP_THRESHOLD	0
#define FASTEST		2
#define FAST		1
#define SLOW		0

#define BOOST_CHARGE_TRANSFER	0	
#define HOLD_DUPLEX	0
#define FUSER_220	1

#define MPF_JAM		1
#define POR_CHECK_FULL	1
#define FULL_OPERATION	1
#define DEBUG_DARKNESS	0
#define DARKNESS	1

#define DEBUG_STRING	0
#define LAST_PAGESUPERVISOR 1
#define MOD_0409	1
#define FUSER_TEST	0
#define DEBUG_LENGTH	0

#define FUSER_PER	0
#define DEBUG_EARLY	1
#define DEBUG_FIRST	0
#define OPEN_THERMISTER	1
#define OPEN_THERMISTER_VOLTAGE	0xE1
#define LeadingEdge	0
#define DEBUG 1//0
#define	DEBUG_JOG	0
#define DEBUG_FUSER 0
#define DEBUG_EP    0
#define DEBUG_PAGE  0
	
#define	DEV_TEST_TAP1	0
#define TOOL_DEV_TAP	0	/* Development Tool tap 3 */
#define EP          1
#define RECEIVED 	1
#define NOT_RECEIVED 0
#define SEND		1
#define NOT_SEND	0
#define SET			1
#define CLEAR		0

#define ENCRYPTION_PRE_READ		0
#define ENCRYPTION_MAIN_READ	1

//#define TRUE		1
//#define FALSE		0
// #define NULL		0

#define	RESERVED	0

#define	ERROR		1
#define NO_ERROR	0
#define	USED		1
#define NOT_USED	0
#define	NONE		0
#define	ONE			1
#define ON			1
#define	OFF			0
#define LOCK		1
#define UNLOCK		0
#define	RUNNING		1
#define	STOP		0
#define HIGH		1
#define LOW		0
#define	NOT_ACTIVE	0
#define	ACTIVE		1
#define	NOTIFY		1

#define	FULL	1

#define MOTOR_OLD 0
#define MOTOR_NEW 1

#define	BACK	1
#define	FRONT	0

#define	OPEN	1
#define	CLOSE	0

#define	JAM	1
#define NOJAM	0

#define	PRESENT	1
#define	NOT_PRESENT	0

#define	NO_EMPTY	0
#define	EMPTY		1

#define MOTOR_RUN	1
#define MOTOR_STOP 	0

#define SC_OK		1
#define	SC_FAIL		0
 
#define FullStackClear	0
#define FullStackSet	1
 
#define	WINDOW	0
#define	NOWINDOW	1
#define	TONER_LEVEL_INDEX		5
#define Dev_Tool	0

#define ForwardExitMt 1
#define BackwardExitMt 2
#define DoubleBackwardExitMt 3
#define NotUsedExitMt 0
#define DoubleForwardExitMt 4

#define CMD_ENG_WAKE_TIMER				300
#define CMD_WATCHDOG_INIT				301
#define	CMD_WATCHDOG_CLEAR				302
#define	CMD_WATCHDOG_STOP				303
#define	CMD_CHK_MAIN_CLK				304
 
#define ENG_TOGGLE_TEST 0

// #define KARA_S_OLD_ENGINE 1
#define KARA_S 1
/*Oak L/LF ���� �ӽ÷� Sensor power ���� ���� (0�� ���) - ����� ���� �ҹ� ������ 1�� �ؾ� ��*/
// #define SENSOR_POWER_OFF 1
/*�Ա� �������� ������ ���� ��� Jam ó�� (1�� ���)*/
//#define INPUT_IDG_GATE 0
/*ABBA L������ Oak ������ DB instance �̸��� Ʋ���� Engine�� ���̸� �־��� (Abba L�� �ƴϸ� 0)*/
#define ABBA_L	0
/*�ű� EP Test Parameter ��� ���� : EP Test�� ������ �ڵ�� �ݵ�� 0����*/
#define FUSER_PARAM_TEST 0
#define EP_PARAM_TEST 0
/*�ű� Fuser Test Parameter ��� ���� H������ ���Ͽ� Test�ÿ��� 1, Release code�� �ݵ�� 0����*/ 
#define NEW_FUSER_TEST_PARAM 0
/*�ű� Engine Test Parameter ��� ���� Release code�� �ݵ�� 0����*/ 
#define ENGINE_PARAM_TEST 0
/*�ű� Full stack code ���� ���� H������ 1, L������ 0*/ 
// #define FULL_STACK 1
/*�ű� Fuser Device Driver ��� ���� H������ 1, L������ 0*/ 
// #define NEW_FUSER 1
/*�ű� Duplex cooling ��� ���� H�������� Duplex cooling ������ �� 1, Release code�� �ݵ�� 0����*/ 
// #define DUPLEX_COOLING 1
/*SmartIC ���� ������ ������� ��� ���� (0�� ���), Release code�� ���� �ҹ� �ݵ�� 1��*/
#define SECURITY_IC_ATTACHED 1 /* [KSF052901] */
/*SmartIC ������ ��ġ���� Smart IC�� �ƹ��͵� write ���� ���� (0�� ���), Release code�� ���� �ҹ� �ݵ�� 1��*/
#define SMART_IC_WRITE 1 /* [KSR052901] */
/*Pilot#1 ���� ���� SmartIC�� �����ǰ� �� (1�� ���), Release code�� ���� �ҹ� �ݵ�� 0����*/
// #define OLD_SM_IC_SUPPORT 0
/*L������ MH ���� ���� (1�� ��� MH)*/
#define MAC_TYPE_MH 1
/*MH ���� ���� (1�� ��� M)*/
//#define MAC_TYPE_M 1 // 1->Pinetree(40ppm), 0->Pinetree-S,Kara-S(47ppm) // go to spec.h
//#define MAC_TYPE_L 0 // go to spec.h
/*Engine ��E?trace on/off*/
#define ENG_TRACE 1
#define KSF_DBG_ONE		0
#define KSF_DBG_TWO		0
#define KSF_DBG_THREE	0 /* [KSF050809] */
#define KSF_DBG_TIMESTAMP 0 /* [KSF052201] */
#define	KSF_TEMP_DISPLAY	0	/* [KSF052205] */
/*SmartIC ��E?tracee on/off*/
#define SMART_IC_TRACE 0
/*���� TLI�� ��E��?�������� SmartIC�� �޷� ������E�����ǰ� �� (0�� �濁E, ����EL�� �ݵ�� 1��, H�� ��а� 0����*/
#define MACHINE_TLI_READ 1

#define KSR_DBG_ONE		1 /* [KSR032902] for SMC dbg */
#define KSR_DBG_TWO		0 /* [KSR040201] for Drum IC dbg */
#define USE_TONER_IC		1 /* [KSR040202] */
#define USE_DRUM_IC		0 /* [KSR040202] */
#define KSR_DBG_THREE	0 /* [KSR050810] */
#define KSR_DBG_ERR		1 /* [KSR070201] */
#define NEW_SC_LOG		0 /* [KSF061301] (SC305) */
#define NEW_SC_LOG2		0 /* [KSF072301] (SC302) */
#define STRESS_TEST_TONER_IC	0 /* [KSR062501] */
#define STRESS_TEST_TONER_IC_AUTH	0 /* [KSR062601] */
#define OVER_HEAT_DBG	0 /* [KSF070901] */
#define KSF_DBG_DUTY		0 /* [KSF072302] */
#define KSF_DBG_THERM	0 /* [KSF080901] */
#define KSF_DBG_PST		0 /* [KSF081301] (warmup heating state table) */
#define KSF_DBG_PST_PRT	0 /* [KSF081401] (Printing state table) */
#define KSF_DBG_SET_DUTY	0 /* [KSF081403] */
#define KSF_DBG_SET_DUTY_WH	0 /* [KSF090601] */
#define FAN_MGR			1
//#define IGNORE_HEATER_ERROR

#define DB_REGULAR_FILENAME 	"/home/ABBA_OAK.db"

#define PICK_THD 1
#define UART_TX_THD 0
#define DARKNESS_TEST 0
#define SPEED_25PPM	0
#define SPEED_45PPM	1
#define SPEED_40PPM	2
#define SPEED_36PPM	3
#define SPEED_32PPM	4
#define DUPLEX_SLOW_MODE 0
#define DUPLEX_SLOW_MODE2 1

	/* 16-bit Free Run Timmer */
#define	UpCounter_1ms	4000
#define	UpCounter_Max	0xffff
		
#define IOCTL_MAGIC		0xE000
/* For Memory */
#define	ComBufSize		20
	/* Size of buffer for communication */

typedef	struct	_combuf	{
				Uint8	dt[ComBufSize] ;
				Uint8	wp ;
				Uint8	rp ;
} COMBUF ;

typedef struct st_Flag {
	union {
		unsigned char BYTE;
		struct {
			unsigned char	bit7:	1;
			unsigned char	bit6:	1;
			unsigned char	bit5:	1;
			unsigned char	bit4:	1;
			unsigned char	bit3:	1;
			unsigned char	bit2:	1;
			unsigned char	bit1:	1;
			unsigned char	bit0:	1;
		} BIT;
	} DR;
} ST_FLAG;

typedef	union	_bitstatus	{
	Uint8	byte ;
	struct	{
		Uint8	b0:1 ;
		Uint8	b1:1 ;
		Uint8	b2:1 ;
		Uint8	b3:1 ;
		Uint8	b4:1 ;
		Uint8	b5:1 ;
		Uint8	b6:1 ;
		Uint8	b7:1 ;
	}	bit ;
} BITSTATUS ;

typedef struct STRUCT_PRINT_PAGE_MSG
{
	unsigned int	temp_PageID;
	unsigned int	temp_Source;
	unsigned int	temp_Narrow;
	unsigned int	temp_MediaDescription;
	unsigned int	temp_Length;
	unsigned int	temp_Darkness;
	unsigned int	temp_TonerSaver;
	unsigned int	temp_OutputDestination;
	unsigned int 	temp_cheat_gap;
} Struct_Temp_PageMsg;

 //typedef struct STRUCT_FUSERCONTROL
 //{
	 //unsigned char			RequestState;
	 //unsigned char			State;
	 //unsigned char			Target_Temperature;
	 //unsigned char 			Current_Temperature;
	 //unsigned char			PaperJam;
	 //unsigned char			TempPaperJam;
	 //unsigned char			EM_State;
	 //unsigned char			fPowerSaveMode;
	 //unsigned char			fMakeMotorRun;
	 //unsigned char			fFatalError;
	 //unsigned char			fPOR_PageSupervisor_Active;
	 //unsigned char			fMainMotorState;
	 //unsigned char			WakeUp_Temperature;
	 //unsigned char			fFuserFault;
	 //unsigned int			FuserFaultNumber;
	 //unsigned char			NarrowMediaCount;
	 //unsigned char			fDuplexJob;
 //}__attribute__((packed))	Struct_Fuser_Control;
#define MAX_NUM_PAGE_TOKENS 20 

#define NUM_TONER_CHECKPOINT		13
#define TONER_CHECK_AREA_OFFSET		200			

typedef struct
{
	Uint8 	sn_buf[4];
	
	Uint32  addr_TonerCheckPoint[NUM_TONER_CHECKPOINT];
	Uint32  addr_TonerCheckWrite;
	Uint32	f_TonerCheck;
	Uint32	TonerUsedCheckData;
}TONERCHECK;

#define fTonerUsed_1K	0x0001
#define fTonerUsed_2K	0x0002
#define fTonerUsed_3K	0x0004
#define fTonerUsed_4K	0x0008
#define fTonerUsed_5K	0x0010
#define fTonerUsed_6K	0x0020
#define fTonerUsed_7K	0x0040
#define fTonerUsed_8K	0x0080
#define fTonerUsed_9K	0x0100
#define fTonerUsed_10K	0x0200
#define fTonerUsed_11K	0x0400
#define fTonerUsed_12K	0x0800
#define fTonerUsed_13K	0x1000

enum
{
	TonerUsed_1K,
	TonerUsed_2K,
	TonerUsed_3K,
	TonerUsed_4K,
	TonerUsed_5K,
	TonerUsed_6K,
	TonerUsed_7K,
	TonerUsed_8K,
	TonerUsed_9K,
	TonerUsed_10K,
	TonerUsed_11K,
	TonerUsed_12K,
	TonerUsed_13K
};

#ifdef DEBUG
#define DEBUG_PAGE_VALIDATE
     extern void dbg_validate_page(PAGE_DATA *pPage);   
#endif

// debug logging tool 
#if 1
#define slogit(e) e,  logStringImpl("%s\n", #e )
// #define slogit(e) e,  dbg_printf("%s\n", #e )
#else
#define slogit(e) e
#endif
 
typedef struct page_handle_s page_handle_t;
 
typedef struct engine_state_s
{
    uint32_t cookie;
    bool paper_out_tray1;
    bool paper_out_tray2;		// tray2
    bool paper_out_tray3;		// tray3
    bool paper_out_tray4;		// mpt
    bool in_job;
    page_handle_t *pick_page;      ///< next page to be picked.
    page_handle_t *picked_page;      ///< page just picked.
    page_handle_t *active_page;    ///< not null from page_recv till bottom
    page_handle_t *page_exiting;   ///< not null from bottom till safe in output bin

    page_handle_t *send_msg_page;  ///< temp incoming page
    page_handle_t *video_done;  ///< video is done page 
    uint32_t page_queue[MAX_NUM_PAGE_TOKENS]; ///< page msg queue 
    uint32_t pm_statusrequest; ///< print_status_codes.h 
    uint32_t top_to_bottom_delay_msecs;
    uint32_t bottom_to_safe_delay_msecs;
    delay_msec_timer_t *page_safe_timer;
    bool input_jam;
    bool exit_jam;
    bool misspick_tray1_jam;
    bool misspick_tray2_jam;
    bool misspick_tray3_jam;
    bool misspick_tray4_jam;
    bool exit_not_detacted_jam;
    bool input_not_released_jam;
    bool exit_not_released_jam;
    bool duplex_jam;
    bool full_stack;
    bool front_door_open;
    bool cartridge_lock;
    bool cartridge_rec_fail;
    int engine_fatal_error;
    Uint8 fcancelrequest;

	uint32_t mirror_motor_state;
} engine_state_t;

typedef struct engine_hardware_s
{
    dec_sensor_t *paper_avail;  
    dec_sensor_t *paper_avail_mpt;
    dec_sensor_t *paper_entry;
    dec_sensor_t *paper_exit;
	//dec_sensor_t *door_open;  // access to jam clear 
	//dec_sensor_t *paper_outbin_full;
	gpio_handle_t *wake_key;
	gpio_handle_t *sensor_pwr;
	gpio_handle_t *ui_nreset;
    gpio_handle_t *pick_pin;        // paper pick clutch
    gpio_handle_t *pick_mpt;
    gpio_handle_t *duplex_sol;
    gpio_handle_t *tx_sensing;
    gpio_handle_t *paper_motor_lock;
    gpio_handle_t *mainfan_lock;
    gpio_handle_t *subfan_lock;
    gpio_handle_t *devfan_lock;
    gpio_handle_t *paper_outbin_full;
    gpio_handle_t *door_open;
    
#if KARA_S
	gpio_handle_t *paper_low;
	gpio_handle_t *paper_high;
	gpio_handle_t *papersize_sw3;
	gpio_handle_t *papersize_sw2;
	gpio_handle_t *papersize_sw1;
	gpio_handle_t *mainmotor_start;
	gpio_handle_t *mainmotor_dir;
	pwm_t *mainmotor_voltage;    ///< (main motor)
#endif

    dec_fuser_t *fuser;
    dec_adc_t *thermistor;
    dcmotor_ol_t* paper_motor;
    dcmotor_speed_t *speed_off;
    dcmotor_speed_t *speed_full;

    pwm_t *fuser_heater; /* [KSF050201] */
    pwm_t *main_fan;
    pwm_t *sub_fan;
    pwm_t *dev_fan;

    gpio_handle_t *thv_en;      ///< transfer voltage enable
    pwm_t *transfer_voltage;    ///< (toner to paper)
    pwm_t *developer_voltage;   ///< (toner to drum)
    pwm_t *charging_voltage;    ///< (drum charge)
    dec_laser_t *laser;

    gpio_handle_t *pin_24v_power;
    gpio_handle_t *pre_heat_ctrl;
#if MPT_SENSOR_PIN_CHG
    gpio_handle_t *paper_avail_mpt_forScanIRQ;
#endif
#if TEST_PIN //Test GPIO-E17  R1505
    gpio_handle_t *GPIO_E17; 
#endif
#if TEST_PIN2 //Test GPIO-E18  R1506
    gpio_handle_t *GPIO_E18; 
#endif
}engine_hardware_t;

typedef struct sp_mode_hardware_test_s
{
	Uint8 test_off;
	Uint8 sensor_check;
	Uint8 solenoid_check;
	Uint8 dcmotor_t1_check;
	Uint8 dcmotor_t2_check;
	Uint8 dcmotor_t3_check;
	Uint8 tray_status_t1_check;
	Uint8 tray_status_t2_check;
	Uint8 tray_status_t3_check;
}sp_mode_hardware_test_t;

enum
{
	LSU_MIRROR_RUN,
	LSU_MIRROR_STOP,
	LSU_MIRROR_RUN_AND_SYNC,
};
#if PICK_THD
enum
{
	DC_MOTOR_RUN,
	DC_MOTOR_STOP,
};
#endif
//engine_hardware_t *eng_hardware;

#define HEATER_PWM_PERIOD 526 //66849 /* [KSF050201] - start */
#define HEATER_DUTY_CYCLE 0.50
#define HEATER_ACTIVE_PULSE (HEATER_PWM_PERIOD * HEATER_DUTY_CYCLE)
#define HEATER_INACTIVE_PULSE (HEATER_PWM_PERIOD - HEATER_ACTIVE_PULSE) /* [KSF050201] - end */

/** High voltage defines; note that these are duplicated from pwm_config.c
  * This is BAD.  Need to develop common header file and improve API. */
#define MAIN_FAN_PWM_PERIOD 333
#define MAIN_FAN_DUTY_CYCLE 0.50
#define MAIN_FAN_ACTIVE_PULSE (MAIN_FAN_PWM_PERIOD * MAIN_FAN_DUTY_CYCLE)
#define MAIN_FAN_INACTIVE_PULSE (MAIN_FAN_PWM_PERIOD - MAIN_FAN_ACTIVE_PULSE)

#define SUB_FAN_PWM_PERIOD 333
#define SUB_FAN_DUTY_CYCLE 0.04
#define SUB_FAN_ACTIVE_PULSE (SUB_FAN_PWM_PERIOD * SUB_FAN_DUTY_CYCLE)
#define SUB_FAN_INACTIVE_PULSE (SUB_FAN_PWM_PERIOD - SUB_FAN_ACTIVE_PULSE)

#define DEV_FAN_PWM_PERIOD 333
#define DEV_FAN_DUTY_CYCLE 0.04
#define DEV_FAN_ACTIVE_PULSE (DEV_FAN_PWM_PERIOD * DEV_FAN_DUTY_CYCLE)
#define DEV_FAN_INACTIVE_PULSE (DEV_FAN_PWM_PERIOD - DEV_FAN_ACTIVE_PULSE)
  
#define HV_PWM_PERIOD 	8160   	// 200MHz/8160 = 24.5KHz			//41     // period in us
#define HV_PWM_PER_CONV		(HV_PWM_PERIOD/255)

#if ABBA2
#if MAC_TYPE_L
/// no use
#define HV_PWM_PERIOD_MTR		73286		// 198MHz/73286 = 2.702kHz		3242.07rpm * 5 / 6 = 2.702kHz
#define HV_PWM_PER_CONV_MTR		(HV_PWM_PERIOD_MTR/100)
#elif MAC_TYPE_M
///32ppm
#define HV_PWM_PERIOD_MTR		106337		// 198MHz/106337 = 1.862kHz		2242.8rpm * 5 / 6 = 1.862kHz
#define HV_PWM_PER_CONV_MTR		(HV_PWM_PERIOD_MTR/100)
#else
///36ppm
#define HV_PWM_PERIOD_MTR		92351		// 198MHz/92351 = 2.144kHz		2572.8rpm * 5 / 6 = 2.144kHz
#define HV_PWM_PER_CONV_MTR		(HV_PWM_PERIOD_MTR/100)
#endif
#else
#if MAC_TYPE_L
/// no use
#define HV_PWM_PERIOD_MTR		73286		// 198MHz/73286 = 2.702kHz		3242.07rpm * 5 / 6 = 2.702kHz	// gap ring
#define HV_PWM_PERIOD_MTR_NEW		81429		// 198MHz/81429 = 2.4315525kHz		3242.07rpm * 4.5 / 6 = 2.4315525kHz	// gap ring
#define HV_PWM_PER_CONV_MTR		(HV_PWM_PERIOD_MTR/100)
#elif MAC_TYPE_M
///40ppm
#define HV_PWM_PERIOD_MTR		80915		// 198MHz/80915 = 2.447kHz		2936.4rpm * 5 / 6 = 2.702kHz	// gap ring
#define HV_PWM_PERIOD_MTR_NEW		89906		// 198MHz/89906 = 2.2023kHz		2936.4rpm * 4.5 / 6 = 2.2023kHz	// gap ring
#define HV_PWM_PER_CONV_MTR		(HV_PWM_PERIOD_MTR/100)
#else
///45ppm
//#define HV_PWM_PERIOD_MTR 		74046   	// 198MHz/74046 = 2.674kHz										// no gap ring
#define HV_PWM_PERIOD_MTR		73286		// 198MHz/73286 = 2.702kHz		3242.07rpm * 5 / 6 = 2.702kHz	// gap ring
#define HV_PWM_PERIOD_MTR_NEW		81429		// 198MHz/81429 = 2.4315525kHz		3242.07rpm * 4.5 / 6 = 2.4315525kHz	// gap ring
#define HV_PWM_PER_CONV_MTR		(HV_PWM_PERIOD_MTR/100)
#endif
#endif

///25ppm
#define HV_PWM_PERIOD_MTR_L 	127388   	// 198MHz/127388 = 1.554kHz		1864.8rpm * 5 / 6 = 1.554kHz
// #define HV_PWM_PERIOD_MTR_L 	141570   	// Error, 198MHz/141570 = 1.3986kHz		1864.8rpm * 4.5 / 6 = 1.3986kHz
#define HV_PWM_PERIOD_MTR_L_NEW		715		// 1MHz/715 = 1398.6Hz		1864.8rpm * 4.5 / 6 = 1.3986kHz
#define HV_PWM_PER_CONV_MTR_L		(HV_PWM_PERIOD_MTR_L/100)

///laminate 5ppm
#define HV_PWM_PERIOD_MTR_LAMINATE		3405		// 1MHz/3405 = 293.7Hz		352.4rpm * 5 / 6 = 293.7Hz
#define HV_PWM_PERIOD_MTR_LAMINATE_NEW		3783		// 1MHz/3783 = 264.3Hz		352.4rpm * 4.5 / 6 = 264.3Hz
#define HV_PWM_PER_CONV_MTR_LAMINATE		(HV_PWM_PERIOD_MTR_LAMINATE/100)

#define TRANSFER_PRINT_DUTY_CYCLE 0.701
#define TRANSFER_IP_DUTY_CYCLE 0.241
#define TRANSFER_CLEAN_DUTY_CYCLE 0

#define DEVELOPER_PRINT_DUTY_CYCLE 0.624
#define DEVELOPER_NO_PRINT_DUTY_CYCLE 0.078  

#define TRANSFER_PRINT_ACTIVE_PULSE (HV_PWM_PERIOD * TRANSFER_PRINT_DUTY_CYCLE)
#define TRANSFER_PRINT_INACTIVE_PULSE (HV_PWM_PERIOD - TRANSFER_PRINT_ACTIVE_PULSE)
#define TRANSFER_IP_ACTIVE_PULSE (HV_PWM_PERIOD * TRANSFER_IP_DUTY_CYCLE)
#define TRANSFER_IP_INACTIVE_PULSE (HV_PWM_PERIOD - TRANSFER_IP_ACTIVE_PULSE)
#define TRANSFER_CLEAN_ACTIVE_PULSE (HV_PWM_PERIOD * TRANSFER_CLEAN_DUTY_CYCLE)
#define TRANSFER_CLEAN_INACTIVE_PULSE (HV_PWM_PERIOD - TRANSFER_CLEAN_ACTIVE_PULSE)

#define DEVELOPER_PRINT_ACTIVE_PULSE (HV_PWM_PERIOD * DEVELOPER_PRINT_DUTY_CYCLE)
#define DEVELOPER_PRINT_INACTIVE_PULSE (HV_PWM_PERIOD - DEVELOPER_PRINT_ACTIVE_PULSE)
#define DEVELOPER_NO_PRINT_ACTIVE_PULSE (HV_PWM_PERIOD * DEVELOPER_NO_PRINT_DUTY_CYCLE)
#define DEVELOPER_NO_PRINT_INACTIVE_PULSE (HV_PWM_PERIOD - DEVELOPER_NO_PRINT_ACTIVE_PULSE)


#define	fEngineRestartCmd			CommandReceiveFlag.DR.BIT.bit0
#define fStartWarmupCmd				CommandReceiveFlag.DR.BIT.bit1
#define fPendingPrintPageCmd		CommandReceiveFlag.DR.BIT.bit2
#define	fClearErrorsCmdRcv			CommandReceiveFlag.DR.BIT.bit3


#define	fEngine_Restart_Cmd						IpcFlag1.DR.BIT.bit0
#define	fPrint_Page_Cmd							IpcFlag1.DR.BIT.bit1
#define fClear_Intervention_Required_Errors_Cmd	IpcFlag1.DR.BIT.bit2
#define fQuery_Jammed_Paper_Locations_Cmd		IpcFlag1.DR.BIT.bit3
#define fEarly_Start_Print_Cmd					IpcFlag1.DR.BIT.bit4
#define fQuery_Device_Information_Cmd			IpcFlag1.DR.BIT.bit5
#define fQuery_Device_Inventory_Cmd				IpcFlag1.DR.BIT.bit6
#define fPower_Saver_mode_Cmd					IpcFlag1.DR.BIT.bit7

#define fQuery_Machine_Check_Information_Cmd	IpcFlag2.DR.BIT.bit0
#define	fPower_Saver_mode_Wake_Up_Cmd			IpcFlag2.DR.BIT.bit1
#define fHSync_FAIL_Cmd							IpcFlag2.DR.BIT.bit2
#define fHSync_OKAY_Cmd							IpcFlag2.DR.BIT.bit3
#define fToner_Count_Cmd						IpcFlag2.DR.BIT.bit4
#define fTotal_Print_Count_Cmd					IpcFlag2.DR.BIT.bit5
#define fQuery_Engine_Software_Info_Cmd			IpcFlag2.DR.BIT.bit6
#define fMagnification_Chg_Cmd					IpcFlag2.DR.BIT.bit7

#define fSensor_Test_Start_Cmd					IpcFlag3.DR.BIT.bit0
#define	fSensor_Test_End_Cmd					IpcFlag3.DR.BIT.bit1
#define fSolenoid_Test_Start_Cmd				IpcFlag3.DR.BIT.bit2
#define fSolenoid_Test_End_Cmd					IpcFlag3.DR.BIT.bit3
#define fQuery_Attached_Devices_Cmd				IpcFlag3.DR.BIT.bit4
#define fCold_Wake_Up_Cmd						IpcFlag3.DR.BIT.bit5

/* Bit Definition for Fatal Error Status */
#define	fPaperFeedMotorFailure		EngineFatalError1.DR.BIT.bit0
#define	fMirrorMotorFailure			EngineFatalError1.DR.BIT.bit1
#define	fLaserDiodeFailure			EngineFatalError1.DR.BIT.bit2
#define	fNoFirstHsyncSignal			EngineFatalError1.DR.BIT.bit3
#define	fPrintheadLostHsync			EngineFatalError1.DR.BIT.bit4
#define	fVideoDoneFail				EngineFatalError1.DR.BIT.bit5
#define	fSoftwareFailure			EngineFatalError1.DR.BIT.bit7

/* Machine Check Code Definition (Fatal Error of engine) */

// Engine > HostPrint : 0xx, HostPrint > SysMgr : 3xx 
#define PRINTHEAD_LOST_HSYNC			10	//310
#define SOFTWARE_ERROR					90	//390	
#define SOFTWARE_ERROR_OPTION			80	//380	
#define NO_MAIN_MOTOR_LOCK				20	//320	
#define NO_MIRROR_MOTOR_LOCK			11	//311	
#define NO_FAN_MOTOR_LOCK				30	//330
#define OPTION_LOST_LINK				40	//340
#define VIDEO_DONE_FAIL					12	//312

#define FUSERFATAL300					200//300	
#define FUSERFATAL301					201//301	
#define FUSERFATAL302					202//302
#define FUSERFATAL303					203//303
#define FUSERFATAL304					204//304
#define FUSERFATAL305					205//305
#define MAINFANLOCKTIMEOUT					30//130
#define SUBFANLOCKTIMEOUT					31//131
#define DEVFANLOCKTIMEOUT					32//132

#define UNDER_TEMP_WHILE_PRINTING		200	//300
#define UNDER_TEMP_WHILE_STANDBY		201	//301
#define FAILED_TO_REACH_STANDBY_TEMP	202	//302
#define	OVER_TEMP_ERROR					203	//303
#define THERMISTER_ERROR				204	//304

#define JAM_LE_INPUT_DETACT_MPT			100
#define JAM_LE_INPUT_DETACT_TRAY1		101
#define JAM_LE_INPUT_DETACT_OPTION		102
#define JAM_LE_INPUT_DETACT_OPTION2		103
#define JAM_LE_EXIT_DETACT				104
#define JAM_TE_INPUT_RELEASE			105
#define JAM_TE_EXIT_RELEASE				106
#define JAM_DUPLEX_PATH					107
#define JAM_IDLE_STATE					105	//108

#define JAM_CLEAR_LAMINATING			0
#define JAM_INPUT_DETECT_LAMINATING		100
#define JAM_INPUT_RELEASE_LAMINATING	105
		
#define CARTRIDGE_CMD_CERT_FAILULE		110
#define CARTRIDGE_CMD_CLOSE_FAILULE		111
#define CARTRIDGE_CMD_TONER_LOW_END		112
#define CARTRIDGE_CMD_WRONG_DATA		113
#define CARTRIDGE_CMD_NO_ISSUE			0
		
#define PARAM_TONER_LOW					1
#define PARAM_TONER_END					2
		
#define	CARTRIDGE_CHECK_ERROR			0xFF
#define	CARTRIDGE_CHECK_OK				0x01
#define	CARTRIDGE_CHECK_TONER_ISSUE		0x55		

#define JAM_MARGIN					34//68		//46 (*100msec)
	
#define WATCHDOG_DEFAULT			7000
#define WATCHDOG_RESET				1
#define SW_RESET_DETACT				300

#define DRUM 		1
#define TONER		0

typedef struct STRUCT_FUSER_CONTROL{
	unsigned char	RequestState;
	unsigned char	State;
	unsigned char	Target_Temperature;
	unsigned char	Current_Temperature;
	unsigned char	PaperJam;
	unsigned char	TempPaperJam;
	unsigned char	EM_State;
	unsigned char	fPowerSaveMode;
	unsigned char	fMakeMotorRun;
	unsigned char	fFatalError;
	unsigned char	fPOR_PageSupervisor_Active;
	unsigned char	fMainMotorState;
	unsigned char	StandbyModeType;
	unsigned char 	fFuserFault;	
	unsigned int	FuserFaultNumber;
	int				NarrowMediaCount;
	unsigned char	fDuplexJob ;		// 1: duplex , 0 : simplex
	unsigned char	fLowTemperature;
	unsigned char	fCoverOpen; /* [KSF050303] */	
	unsigned char	fDevFanOff_ForLowServo;
	unsigned char	FanStopLongDelayInStandby;
#if BLI_TEST	
	unsigned char	LowSpeedFan_Wakeup;
#endif
} __attribute__((packed)) Struct_Fuser_Control ;

/* RequestState Variable*/
enum 
{
	CMD_FUSER_OFF = 0,
	CMD_FUSER_COLD_WAKE = 3,
	CMD_FUSER_STANDBY = 5,
	CMD_FUSER_PRINT = 7,
	CMD_FUSER_POWERSAVE = 8,
	CMD_FUSER_POWERLOW = 10 /* [KSF032702] add power low *//* [KSF050201] */
};

/* State Variable*/
enum
{
	ST_OFF = 0,
	ST_ERROR,
	ST_WAIT_RUN,
	ST_COLD_WAKEUP,
	ST_WARMUP,
	ST_STANDBY,
	ST_HEATING,
	ST_PRINT,
	ST_POWER_SAVE,
	ST_COOL_STANDBY,
	ST_POWER_LOW	/* [KSF050201] */
};

#define	NORMAL_PAPER	5

#define	Temp_000C	0 
#define	Temp_001C   1 
#define	Temp_002C   2  
#define Temp_003C	3  
#define	Temp_004C	4  
#define	Temp_005C	5   
#define Temp_006C   6  
#define	Temp_007C   7  
#define	Temp_008C   8   
#define Temp_009C   9  
#define	Temp_010C   10  
#define	Temp_011C	11   
#define Temp_012C	12  
#define	Temp_013C   13  
#define	Temp_014C	14   
#define Temp_015C   15  
#define	Temp_016C	16  
#define	Temp_017C	17   
#define Temp_018C	18  
#define	Temp_019C	19  
#define	Temp_020C	20   
#define Temp_021C	21  
#define	Temp_022C	22  
#define	Temp_023C	23   
#define Temp_024C	24  
#define	Temp_025C	25  
#define	Temp_026C	26   
#define Temp_027C	27  
#define	Temp_028C	28  
#define	Temp_029C	29   
#define Temp_030C	30  
#define	Temp_031C	31  
#define	Temp_032C	32   
#define Temp_033C	33  
#define	Temp_034C	34  
#define	Temp_035C	35   
#define Temp_036C	36  
#define	Temp_037C	37  
#define	Temp_038C	38   
#define Temp_039C	39  
#define	Temp_040C	40  
               
#define Temp_041C 	41
#define	Temp_042C   42
#define	Temp_043C	43
#define Temp_044C   44
#define	Temp_045C	45
#define	Temp_046C   46
#define	Temp_047C   47 
#define Temp_048C	48 
#define	Temp_049C	49 
#define	Temp_050C	50  
#define Temp_051C   51 
#define	Temp_052C   52 
#define	Temp_053C   53  
#define Temp_054C   54 
#define	Temp_055C   55  
#define	Temp_056C	56   
#define Temp_057C	57  
#define	Temp_058C   58  
#define	Temp_059C	59   
#define Temp_060C   60  
#define	Temp_061C	61  
#define	Temp_062C	62   
#define Temp_063C	63  
#define	Temp_064C	64  
#define	Temp_065C	65   
#define Temp_066C	66  
#define	Temp_067C	67  
#define	Temp_068C	68   
#define Temp_069C	69  
#define	Temp_070C	70  
#define	Temp_071C	71   
#define Temp_072C	72  
#define	Temp_073C	73  
#define	Temp_074C	74   
#define Temp_075C	75  
#define	Temp_076C	76  
#define	Temp_077C	77   
#define Temp_078C	78  
#define	Temp_079C	79  
#define	Temp_080C	80   
               
#define	Temp_081C	81   
#define	Temp_082C	82    
#define Temp_083C	83   
#define	Temp_084C	84   
#define	Temp_085C	85    
#define Temp_086C	86  
#define	Temp_087C	87  
#define	Temp_088C	88 
#define Temp_089C   89 
#define Temp_090C   90 
#define	Temp_091C	91 
#define	Temp_092C   92 
#define	Temp_093C   93  
#define Temp_094C	94  
#define	Temp_095C	95  
#define	Temp_096C	96   
#define Temp_097C   97  
#define	Temp_098C   98  
#define	Temp_099C   99   
#define Temp_100C   100 
#define	Temp_101C   101  
#define	Temp_102C	102   
#define Temp_103C	103  
#define	Temp_104C   104  
#define	Temp_105C	105   
#define Temp_106C   106  
#define	Temp_107C	107  
#define	Temp_108C	108   
#define Temp_109C	109  
#define	Temp_110C	110  
#define	Temp_111C	111   
#define Temp_112C	112  
#define	Temp_113C	113  
#define	Temp_114C	114   
#define Temp_115C	115  
#define	Temp_116C	116  
#define	Temp_117C	117   
#define Temp_118C	118  
#define	Temp_119C	119  
#define	Temp_120C	120   
#define Temp_121C	121  
#define	Temp_122C	122  
#define	Temp_123C	123   
#define Temp_124C	124  
#define	Temp_125C	125  
#define	Temp_126C	126   
#define Temp_127C	127  
#define	Temp_128C	128  
#define	Temp_129C	129   
#define Temp_130C	130  

#define	Temp_131C	131   
#define Temp_132C	132 
#define	Temp_133C	133 
#define	Temp_134C	134
#define Temp_135C   135
#define Temp_136C   136
#define	Temp_137C	137
#define	Temp_138C   138
#define	Temp_139C   139 
#define Temp_140C	140 
#define	Temp_141C	141 
#define	Temp_142C	142  
#define Temp_143C   143 
#define	Temp_144C   144 
#define	Temp_145C   145  
#define Temp_146C   146 
#define	Temp_147C   147  
#define	Temp_148C	148   
#define Temp_149C	149  
#define	Temp_150C   150  
#define	Temp_151C	151   
#define Temp_152C   152  
#define	Temp_153C	153  
#define	Temp_154C	154   
#define Temp_155C	155  
#define	Temp_156C	156  
#define	Temp_157C	157   
#define Temp_158C	158  
#define	Temp_159C	159  
#define	Temp_160C	160   
#define Temp_161C	161  
#define	Temp_162C	162  
#define	Temp_163C	163   
#define Temp_164C	164  
#define	Temp_165C	165  
#define	Temp_166C	166   
#define Temp_167C	167  
#define	Temp_168C	168  
#define	Temp_169C	169   
#define Temp_170C	170  
#define	Temp_171C	171  
#define	Temp_172C	172   
#define Temp_173C	173  
#define	Temp_174C	174  
#define	Temp_175C	175   
#define Temp_176C	176  
#define	Temp_177C	177  
#define	Temp_178C	178   
#define Temp_179C	179 
#define	Temp_180C	180 

#define Temp_181C	181
#define Temp_182C   182
#define	Temp_183C	183
#define	Temp_184C   184
#define	Temp_185C   185 
#define Temp_186C	186 
#define	Temp_187C	187 
#define	Temp_188C	188  
#define Temp_189C   189 
#define	Temp_190C   190 
#define	Temp_191C   191  
#define Temp_192C   192 
#define	Temp_193C   193  
#define	Temp_194C	194   
#define Temp_195C	195  
#define	Temp_196C   196  
#define	Temp_197C	197   
#define Temp_198C   198  
#define	Temp_199C	199  
#define	Temp_200C	200   
#define Temp_201C	201  
#define	Temp_202C	202  
#define	Temp_203C	203   
#define Temp_204C	204  
#define	Temp_205C	205  
#define	Temp_206C	206   
#define Temp_207C	207  
#define	Temp_208C	208  
#define	Temp_209C	209   
#define Temp_210C	210  
#define	Temp_211C	211  
#define	Temp_212C	212   
#define Temp_213C	213  
#define	Temp_214C	214  
#define	Temp_215C	215   
#define Temp_216C	216  
#define	Temp_217C	217  
#define	Temp_218C	218   
#define Temp_219C	219  
#define	Temp_220C	220  
#define	Temp_221C	221   
#define Temp_222C	222  
#define	Temp_223C	223  
#define	Temp_224C	224   
#define Temp_225C	225 
#define	Temp_226C	226 
#define	Temp_227C	227
#define Temp_228C   228
#define Temp_229C   229
#define	Temp_230C	230

#define	Temp_231C   231 
#define Temp_232C	232 
#define	Temp_233C	233 
#define	Temp_234C	234  
#define Temp_235C   235 
#define	Temp_236C   236 
#define	Temp_237C   237  
#define Temp_238C   238 
#define	Temp_239C   239  
#define	Temp_240C	240   
#define Temp_241C	241  
#define	Temp_242C   242  
#define	Temp_243C	243   
#define Temp_244C   244  
#define	Temp_245C	245  
#define	Temp_246C	246   
#define Temp_247C	247  
#define	Temp_248C	248  
#define	Temp_249C	249   
#define Temp_250C	250
#define Temp_251C	251  
#define	Temp_252C   252  
#define	Temp_23C	253   
#define Temp_254C   254  
#define	Temp_255C	255  
#define	Temp_256C	256   
#define Temp_257C	257  
#define	Temp_258C	258  
#define	Temp_259C	259   
#define Temp_260C	260
#define	Temp_MAXC   260

#define PID_Kp 0.5
#define PID_Ki 0.05//0.02
#define PID_Kd 0.1//0.02
#define PID_dt 1
#if MAC_TYPE_L
#define PID_Target_Normal 2111
#elif MAC_TYPE_M
#define PID_Target_Normal 1817
#else
#define PID_Target_Normal 2111
#endif
#define PID_Target_Quiet 1300
#define PID_Target_Recycled 350

#define STATIC_MOTOR_RUN_TIME_UNDER_TEMP	600
#define STATIC_MOTOR_RUN_TIME_OVER_TEMP		400

#if ABBA2 // Abba2
#define FUSER_PARAM_TARGET_TMP				180
#define FUSER_PARAM_STANDBY_TMP				160
#define FUSER_PARAM_STANDBY_TIME		6000	/* [KSF032502] siyo add 60[sec] */
#define FUSER_PARAM_PICKUP_TMP				25
#define FUSER_PARAM_PICKUP_TMP_FOR_PWS			5
#define FUSER_PARAM_MT_RUN_TMP1				120
#define FUSER_PARAM_MT_RUN_TMP2		70
#define FUSER_PARAM_MT_RUN_TMP3		60
#define FUSER_PARAM_MT_RUN_TMP4		45
#define FUSER_PARAM_MT_RUN_TMP5		45
#define FUSER_PARAM_MT_RUN_TIME1		600	// 6sec
#define FUSER_PARAM_MT_RUN_TIME2		300
#define FUSER_PARAM_MT_RUN_TIME3		400
#define FUSER_PARAM_MT_RUN_TIME4		0
#define FUSER_PARAM_Ts_NM				-1500
#define FUSER_PARAM_Ts_NS				-500
#define FUSER_PARAM_Ts_PS				500
#define FUSER_PARAM_Ts_PM				1500
#define FUSER_PARAM_Ts_Off				4	
#define	FUSER_PARAM_T_On_Pre			1
#define	FUSER_PARAM_T_On_Cur			8
#define	FUSER_PARAM_Temp_On				0
#define	FUSER_PARAM_Temp_Off			500 // 5C
#define	FUSER_PARAM_Ts_On_NM			10
#define	FUSER_PARAM_Ts_On_NS			6
#define	FUSER_PARAM_Ts_On_ZR			4
#define	FUSER_PARAM_Ts_On_PS			2
#define FUSER_PARAM_Tt_NB				-1200
#define FUSER_PARAM_Tt_NM				-600
#define FUSER_PARAM_Tt_NS				-200
#define FUSER_PARAM_Tt_PS				200
#define FUSER_PARAM_Tt_PM				400
#define FUSER_PARAM_dTt_NB				-150
#define FUSER_PARAM_dTt_NM				-100
#define	FUSER_PARAM_dTt_NS				-50
#define FUSER_PARAM_dTt_PS				100
#define FUSER_PARAM_dTt_PM				200
#define	FUSER_PARAM_dTt_PB				300
#define FUSER_PARAM_FAN_HIGH_SPEED_DUTY			50
#define FUSER_PARAM_FAN_LOW_SPEED_DUTY			1
#define FUSER_PARAM_DUP_TARGET_TMP				170
#define FUSER_PARAM_FAN_TIME_BEFORE_STANDBY 	12
#define	FUSER_PARAM_FAN_TIME_PWR_SAVE 			240
#define FUSER_PARAM_MAIN_FAN_LOW 				5
#define FUSER_PARAM_MAIN_FAN_MIN 				2
#define FUSER_PARAM_SUB_FAN_LOW 				4
#define FUSER_PARAM_COLD_WAKEUP_HOLDING_TEMP	120
#define FUSER_PARAM_COLD_WAKEUP_HOLDING_TIME	2
#define FUSER_PARAM_COLD_WAKEUP_TAR_TEMP		130
#else // Kara/-S, Pinetree/-S
#define FUSER_PARAM_TARGET_TMP			180
#define FUSER_PARAM_STANDBY_TMP		175 	/* [KSF032502] siyo chg 160 -> 175 *//* [KSF050201] - start */
#define FUSER_PARAM_STANDBY_TIME		6000	/* [KSF032502] siyo add 60[sec] */
#define FUSER_PARAM_PICKUP_TMP			15 	/* [KSF032502] siyo chg 25 -> 15 */
#if BLI_TEST
#define FUSER_PARAM_PICKUP_TMP_FOR_PWS			0
#define FUSER_PARAM_PICKUP_TMP_FOR_PWS_2nd		10
#else
#define FUSER_PARAM_PICKUP_TMP_FOR_PWS			5
#endif
#define FUSER_PARAM_MT_RUN_TMP1		125	/* [KSF032502] siyo chg 120 -> 125 *//* [KSF050201] - end */
#define FUSER_PARAM_MT_RUN_TMP2		70
#define FUSER_PARAM_MT_RUN_TMP3		60
#define FUSER_PARAM_MT_RUN_TMP4		45
#define FUSER_PARAM_MT_RUN_TMP5		45
#define FUSER_PARAM_MT_RUN_TIME1		600	//1200 100->1[sec] /* [KSF050201] */
#define FUSER_PARAM_MT_RUN_TIME2		300	/* [KSF032502] siyo chg 0 -> 300 *//* [KSF050201] */
#define FUSER_PARAM_MT_RUN_TIME3		400
#define FUSER_PARAM_MT_RUN_TIME4		0
#define FUSER_PARAM_Ts_NM				-1500
#define FUSER_PARAM_Ts_NS				-500
#define FUSER_PARAM_Ts_PS				500
#define FUSER_PARAM_Ts_PM				1500
#define FUSER_PARAM_Ts_Off				4	
#define	FUSER_PARAM_T_On_Pre			1
#define	FUSER_PARAM_T_On_Cur			8
#define	FUSER_PARAM_Temp_On				0
#define	FUSER_PARAM_Temp_Off			500		//1000
#define	FUSER_PARAM_Ts_On_NM			10
#define	FUSER_PARAM_Ts_On_NS			6
#define	FUSER_PARAM_Ts_On_ZR			4
#define	FUSER_PARAM_Ts_On_PS			2
#define FUSER_PARAM_Tt_NB				-1200
#define FUSER_PARAM_Tt_NM				-600
#define FUSER_PARAM_Tt_NS				-300
#define FUSER_PARAM_Tt_PS				200
#define FUSER_PARAM_Tt_PM				400
#define FUSER_PARAM_dTt_NB				-150
#define FUSER_PARAM_dTt_NM				-100
#define	FUSER_PARAM_dTt_NS				-50
#define FUSER_PARAM_dTt_PS				100
#define FUSER_PARAM_dTt_PM				200
#define	FUSER_PARAM_dTt_PB				300
#define FUSER_PARAM_FAN_HIGH_SPEED_DUTY			50
#define FUSER_PARAM_FAN_LOW_SPEED_DUTY			1
#define FUSER_PARAM_DUP_TARGET_TMP				170
#define FUSER_PARAM_FAN_TIME_BEFORE_STANDBY 	12
#define	FUSER_PARAM_FAN_TIME_PWR_SAVE 			240
#define FUSER_PARAM_MAIN_FAN_LOW 				5
#define FUSER_PARAM_MAIN_FAN_MIN 				2
#define FUSER_PARAM_SUB_FAN_LOW 				4
#define FUSER_PARAM_COLD_WAKEUP_HOLDING_TEMP	120
#define FUSER_PARAM_COLD_WAKEUP_HOLDING_TIME	2
#define FUSER_PARAM_COLD_WAKEUP_TAR_TEMP		130
#endif

#define FUSER_PARAM_TMP_LAMINATE	160
#define FUSER_PARAM_TMP_LAMINATE_PREHEAT	190
#define FUSER_PARAM_PICKUP_TMP_LAMINATE		10
// #define ENG_PARAM_INPUT_IDG_LIMIT			7	//10

#define ENG_PARAM_TEMP_SERVO_H				62
#define ENG_PARAM_TEMP_SERVO_HM				70
#define ENG_PARAM_TEMP_SERVO_M				95
#define ENG_PARAM_TEMP_SERVO_ML				110
#define ENG_PARAM_TEMP_SERVO_L				180
#define ENG_PARAM_1ST_NOR_PAGE_H			30
#define ENG_PARAM_1ST_NOR_PAGE_HM			30
#define ENG_PARAM_1ST_NOR_PAGE_M			50
#define ENG_PARAM_1ST_NOR_PAGE_L			100
#define ENG_PARAM_1ST_NOR_PAGE_H_MAX		100
#define ENG_PARAM_1ST_NOR_PAGE_M_MAX		150	
#define ENG_PARAM_2ND_NOR_PAGE_H			3
#define ENG_PARAM_2ND_NOR_PAGE_HM			3
#define ENG_PARAM_2ND_NOR_PAGE_M			10
#define ENG_PARAM_2ND_NOR_PAGE_L			100
#define ENG_PARAM_SLOW_COOL_TIME_H			5000
#define ENG_PARAM_SLOW_COOL_TIME_HM			4000
#define ENG_PARAM_SLOW_COOL_TIME_M			6000
#define ENG_PARAM_SLOW_COOL_TIME_L			12000
#define ENG_PARAM_COOLING_TIME_OFFSET		10
	
/* Smart Chip Offset Address */	
#define SC_TLI_ADDR_H						0
#define SC_TLI_ADDR_M						1
#define SC_TLI_ADDR_L						2
#define SC_PRODUCTION_SN_ADDR				3
#define SC_MEMORY_MAP_VER_ADDR				4 // start
#define SC_BRAND_ID_ADDR					5	
#define SC_REGION_ID_ADDR					6
#define SC_CART_DIST_ADDR					7
#define SC_NEW_INFO_ADDR					8
#define SC_RECYCLE_INFO_ADDR				9
#define SC_TONER_NEAR_END_ADDR				10
#define SC_TONER_END_ADDR					11
#define SC_TONER_BLOCK_ADDR					12
#define SC_TONER_HISTORY_COUNT_ADDR			13
#define SC_PAGE_COUNT_ADDR					14
#define SC_PAGE_COUNT_CONVERSION_ADDR	15
#define SC_TONER_FREE_MOTOR_ON_ADDR		16/* [KSR032912]-start */
#define SC_TONER_HIS_FACTOR_PERCENT_1_ADDR				17 //0~1.24%		// 0 ~ 1.25%
#define SC_TONER_HIS_FACTOR_PERCENT_2_ADDR				18 //1.25~2.49%		// 1.25 ~ 2.5%
#define SC_TONER_HIS_FACTOR_PERCENT_3_ADDR				19 //2.5~4.24%		// 2.5 ~ 4.25%
#define SC_TONER_HIS_FACTOR_PERCENT_4_ADDR				20 //4.25~9.99%		// 4.25 ~ 7.45%
#define SC_TONER_HIS_FACTOR_PERCENT_5_ADDR				21 //10~49.99%		// 7.45 ~ 13.13%
#define SC_TONER_HIS_FACTOR_PERCENT_6_ADDR				22 //50~ %			// 13.13 ~ 25.74%
//#define SC_TONER_HIS_FACTOR_PERCENT_7_ADDR		23 // 25.74 ~ 50%
//############## DO NOT ACCESS 24 ADDRESS #####################
//#define SC_TONER_HIS_FACTOR_PERCENT_8_ADDR		24 // 50 ~ 100% // DO NOT ACCESS 24 ADDRESS
//############## DO NOT ACCESS 24 ADDRESS #####################
#define SC_TONER_HIS_MODE_JOB_1_ADDR		25 // 1p
#define SC_TONER_HIS_MODE_JOB_2_ADDR		26 // 2p
#define SC_TONER_HIS_MODE_JOB_3_ADDR		27 // 3p
#define SC_TONER_HIS_MODE_JOB_4_ADDR		28 // 4p
#define SC_TONER_HIS_MODE_JOB_5_ADDR		29 // 5p
#define SC_TONER_HIS_MODE_JOB_6_ADDR		30 // 6 ~ 10p
#define SC_TONER_HIS_MODE_JOB_7_ADDR		31 // 11 ~ 20p
#define SC_TONER_HIS_MODE_JOB_8_ADDR		32 // 21 ~ 50p		//21 ~ 30p
#define SC_TONER_HIS_MODE_JOB_9_ADDR		33 // 51p ~			//31 ~ 50p
#define SC_TONER_HIS_MODE_JOB_10_ADDR		34 // Total			//51 ~ 74p
//#define SC_TONER_HIS_MODE_JOB_11_ADDR	35 // 74 ~ 100p
//#define SC_TONER_HIS_MODE_JOB_12_ADDR	36 // 101 ~
//#define SC_TONER_HIS_MODE_JOB_13_ADDR	37 // total
#define SC_DRUM_CART_USAGE_ADDR			38 // <- Drum Only
#define SC_DRUM_REST_ADDR					39 // <- Drum Only /* [KSR032912]-start */
#define SC_DRUM_MOTOR_RUN_TIME_ADDR			40

#define SC_TONER_USED_CHECK_1K_ADDR			70
#define SC_TONER_USED_CHECK_2K_ADDR			71
#define SC_TONER_USED_CHECK_3K_ADDR			72
#define SC_TONER_USED_CHECK_4K_ADDR			73
#define SC_TONER_USED_CHECK_5K_ADDR			74
#define SC_TONER_USED_CHECK_6K_ADDR			75
#define SC_TONER_USED_CHECK_7K_ADDR			76
#define SC_TONER_USED_CHECK_8K_ADDR			77
#define SC_TONER_USED_CHECK_9K_ADDR			78
#define SC_TONER_USED_CHECK_10K_ADDR		79
#define SC_TONER_USED_CHECK_11K_ADDR		80
#define SC_TONER_USED_CHECK_12K_ADDR		81
#define SC_TONER_USED_CHECK_13K_ADDR		82
#define SC_WRONG_TONER_COUNT_CHK_ADDR		98
#define SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR	99
#define SC_DUMMY_ADDR						100

#define SC_V3_TONER_NEAR_END_ADDR			150
#define SC_V3_TONER_END_ADDR				151
#define SC_V3_TONER_BLOCK_ADDR				152

#define SC_CHIP_VERSION_ADDR				483
#define SC_MEMORY_LOCK_AREA_FIRST_ADDR		484
#define SC_DATA_ENCRYPTION_AREA_START_ADDR	150
#define SC_DATA_ENCRYPTION_AREA_END_ADDR	459

#define SC_TEST_ADDR_MOTOR_RUN_TIME			17
	
#define SC_NEW_TONER_NUM					100
#define SC_USED_TONER_NUM					1
#define SC_WRITE_WAIT_TIME					7	// (50msec)
	
#define ST_TONER_NEAR_END					1
#define	ST_TONER_END						2
#define	ST_TONER_BLOCK						3
#define	ST_NO_ISSUE							4
#define	ST_CERT_FAILULE						5
#define	ST_TONER_WRONG						6

//#if MAC_TYPE_MH
#define	TRANS_COEFFICIENT_5PERCENT			137044			//136067	// Number of 5% chart dots
#define	TRANS_COEFFICIENT_100PERCENT		2740880			//2721340	// Number of 100% chart dots /* [KSR032912] */
#define NIP_TONER_CONSUMPTION				6852
#define JOB_TONER_CORRECTION_VALUE			38372
#define MAX_VALUE_IN_CHIP 					0x7FFFFFFF   
#define DOTCOUNT_OF_5PERCHART_15K			(15000*TRANS_COEFFICIENT_5PERCENT)
#define DOTCOUNT_OF_5PERCHART_13750			(13750*TRANS_COEFFICIENT_5PERCENT) // 19K
#define DOTCOUNT_OF_5PERCHART_14250			(14250*TRANS_COEFFICIENT_5PERCENT) // 18K

#define NUM_DOTS_1P25PERCENT				34017		// Number of 1.25% chart dots
#define NUM_DOTS_2P5PERCENT					68034		// Number of 2.5% chart dots 
#define NUM_DOTS_4P25PERCENT				115657		// Number of 4.25% chart dots 
#define NUM_DOTS_10PERCENT					272134		// Number of 10% chart dots 
#define NUM_DOTS_50PERCENT					1360670		// Number of 50% chart dots 
//#else
//#define	TRANS_COEFFICIENT_5PERCENT			130595	// Number of 5% chart dots	
//#endif
#define NUM_TONER_FACTOR						6 /* [KSR032912] */
#define NUM_TONER_JOB						10 /* [KSR032912] */

#define DEFAULT_TONER_CART_30				3000
#define DEFAULT_TONER_CART_60				6000
#define DEFAULT_TONER_CART_90				9000
#define DEFAULT_TONER_CART_130				13000

#if ABBA2
#define	TONER_NEAR_END_3K_CART_VALUE		300
#define	TONER_NEAR_END_6K_CART_VALUE		300
#define	TONER_NEAR_END_9K_CART_VALUE		900
#define	TONER_BLOCK_VALUE_3K				1500
#define	TONER_BLOCK_VALUE_6K				2500
#define	TONER_BLOCK_VALUE_9K				3000
#else
#define	TONER_NEAR_END_3K_CART_VALUE		500
#define	TONER_NEAR_END_6K_CART_VALUE		500
#define	TONER_NEAR_END_9K_CART_VALUE		900
#define	TONER_NEAR_END_13K_CART_VALUE		1300
#define	TONER_BLOCK_VALUE_3K				1500
#define	TONER_BLOCK_VALUE_6K				2500
#define	TONER_BLOCK_VALUE_9K				3500
#define	TONER_BLOCK_VALUE_13K				2500
#endif
#define	DRUM_BLOCK_VALUE					2000

#define JOG_WAIT_TIME_1MIN			(Uint32)(1*60*100)
#define JOG_WAIT_TIME_5MIN			(Uint32)(5*60*100)
#define JOG_WAIT_TIME_10MIN			(Uint32)(10*60*100)
#define JOG_WAIT_TIME_30MIN			(Uint32)(30*60*100)
#define JOG_WAIT_TIME_60MIN			(Uint32)(60*60*100)
#define JOG_WAIT_TIME_120MIN		(Uint32)(120*60*100)
	
#define JOG_RUN_PERIOD_10SEC		(Uint32)(10*100)
#define JOG_RUN_PERIOD_20SEC		(Uint32)(20*100)
#define JOG_RUN_PERIOD_30SEC		(Uint32)(30*100)
#define JOG_RUN_PERIOD_60SEC		(Uint32)(60*100)
#define JOG_RUN_PERIOD_300SEC		(Uint32)(300*100)
#define JOG_RUN_PERIOD_600SEC		(Uint32)(600*100)
#define JOG_RUN_PERIOD_30MIN		(Uint32)(30*60*100)
#define JOG_RUN_PERIOD_60MIN		(Uint32)(60*60*100)
#define JOG_RUN_PERIOD_120MIN		(Uint32)(120*60*100)
	
/* Paper Jam Location */
#define	fPaperJamOnPins				PaperJam.DR.BIT.bit7
#define	fPaperJamBetweenPinsAndPexs	PaperJam.DR.BIT.bit6
#define	fPaperJamOnPexs				PaperJam.DR.BIT.bit5
#define	fPaperJamOnDis				PaperJam.DR.BIT.bit4
#define	fPaperJamAtMPF				PaperJam.DR.BIT.bit3
#define	RESERVED2					PaperJam.DR.BIT.bit2
#define	RESERVED3					PaperJam.DR.BIT.bit1
#define	RESERVED4					PaperJam.DR.BIT.bit0
	
/* Bit Definition for PageSupervisor Warning Status */
#define	PaperJamOnPins					TempPaperJam.DR.BIT.bit7
#define	PaperJamBetweenPinsAndPexs		TempPaperJam.DR.BIT.bit6
#define	PaperJamOnPexs					TempPaperJam.DR.BIT.bit5
#define PaperJamInDuplex				TempPaperJam.DR.BIT.bit4
#define	PaperJamBetweenPickupAndPins	TempPaperJam.DR.BIT.bit3
#define RESERVED12						TempPaperJam.DR.BIT.bit2
#define RESERVED5						TempPaperJam.DR.BIT.bit1
#define	RESERVED6						TempPaperJam.DR.BIT.bit0

#define	fPaperEmptyForMPF		PaperEmpty.DR.BIT.bit0
#define	fPaperEmptyForT1		PaperEmpty.DR.BIT.bit1
#define fPaperEmptyForT2		PaperEmpty.DR.BIT.bit2
#define fPaperEmptyForT3		PaperEmpty.DR.BIT.bit3
#define RESERVED8				PaperEmpty.DR.BIT.bit4
#define RESERVED9				PaperEmpty.DR.BIT.bit5
#define RESERVED10				PaperEmpty.DR.BIT.bit6
#define RESERVED11				PaperEmpty.DR.BIT.bit7

#define RESERVED17				TempPaperEmpty.DR.BIT.bit0
#define	EmptyForMPF				TempPaperEmpty.DR.BIT.bit1
#define	EmptyForT1				TempPaperEmpty.DR.BIT.bit2
#define	EmptyForT2				TempPaperEmpty.DR.BIT.bit3
#define EmptyForT3				TempPaperEmpty.DR.BIT.bit4
#define RESERVED14				TempPaperEmpty.DR.BIT.bit5
#define RESERVED15				TempPaperEmpty.DR.BIT.bit6
#define RESERVED16				TempPaperEmpty.DR.BIT.bit7

/* Error */
#define	fDoorCoverError			CoverError.DR.BIT.bit0

/* Door State */
#define	fFrontCoverState		CoverState.DR.BIT.bit0
#define	fRearCoverState			CoverState.DR.BIT.bit1

#define	fEngineApplication		Flags.DR.BIT.bit0
#define	fADC_Data				Flags.DR.BIT.bit1
#define	fFuserInit				Flags.DR.BIT.bit2
#define	fADC_First				Flags.DR.BIT.bit3
#define fFirstFuserData			Flags.DR.BIT.bit4
#define fFirstFuserTemp			Flags.DR.BIT.bit5
#define	fFuserEarlyStart		Flags.DR.BIT.bit6

#define fRetryPickUp			TrayFlags.DR.BIT.bit0
#define fStatusOfTray1Remove	TrayFlags.DR.BIT.bit1
#define fStatusOfTray1Insert	TrayFlags.DR.BIT.bit2
#define fLastPickUp				TrayFlags.DR.BIT.bit3

#define fMPFAttached			AttachedDevice.DR.BIT.bit0
#define fTray1Attached			AttachedDevice.DR.BIT.bit1
#define fTray2Attached			AttachedDevice.DR.BIT.bit2
#define fDuplexerAttached		AttachedDevice.DR.BIT.bit6
#define fManualFeedAttached		AttachedDevice.DR.BIT.bit7

#define VALUEOFPRINTEDPAGECOUNTER 25

enum {
	MTR_MODE_NORMAL = 0,
	MTR_MODE_QUIET,
	MTR_MODE_R1200,
	MTR_MODE_LAMINATE,
	MTR_MODE_SLOWToFAST = 0x1000,
};

enum {
	CON_MTR_STOP_LAMI1,
	CON_MTR_STOP_LAMI2,
};

#define MAX_JOGMODEENV 		3
#define MAX_JOGMODETIME 	4
enum {
	JOG_MODE_H = 0,
	JOG_MODE_M,
	JOG_MODE_L,
};

enum {
	FAN_CTRL_H = 0,
	FAN_CTRL_L,
};

enum {
	EnvTemp_Clear,
	EnvTemp_Slow,
	EnvTemp_Fast
};
enum {
	EnvTemp_HH,
	EnvTemp_H,
	EnvTemp_HM,
	EnvTemp_M,
	EnvTemp_ML,
	EnvTemp_L,
	EnvTemp_DL
};
		
enum {
	SP_InputSensor = 1,
	SP_ExitSensor,
	SP_MPTSensor,
	SP_Tray1Sensor,
	SP_Tray2Sensor,
	SP_CoverSwitch
};

/* For Engine Manager Sequence */
enum {
	/* For POR and Warmup */
	EMS_RIP_SYNC,
	EMS_StartWarmup,
	EMS_StartPOR,
	EMS_PowerOnResetState,
	EMS_POR_Standby,
	
	/* For Standby */
	EMS_PrintReady,/*Sequence 5*/
	EMS_Standby,
	EMS_PowerSave,
	EMS_JogMode,
	/* For Print Sequence */
	EMS_Printing,
	
	/* For Laminating Sequence */
	EMS_CheckExitSensorForLaminatingStart,
	EMS_CheckExitSensorForLaminatingEnd,
	EMS_Laminating_Servo,
	EMS_PreHeating,
	EMS_Laminating,
	EMS_Laminating_Standby,
	EMS_Laminating_End,
	EMS_Laminating_Error,
	EMS_Laminating_Recovery,
	EMS_Laminating_CloseCounter,
	EMS_Laminating_CloseDoorCover,
	
	/* For EP Cleaning Process */
	EMS_ExitLastPage,/*Sequence 10*/
	EMS_EPCleaning,
	EMS_PrinterStop,
	
	/* For Open Front Cover Condition */
	EMS_OpenDoorCover,
	EMS_CloseCounter,
	EMS_CloseDoorCover,/*Sequence 15*/
	
	EMS_RecoverPaperJam,
	
	EMS_RecoverPaperEmpty,
	
	EMS_RecoverFullStack,
	
	EMS_RecoverNoStopError, 

	EMS_RecoverError,
	EMS_OptionResync,
	EMS_FatalError,/* For Unrecoverable Error Condition *//*Sequence 20*/
	/* For Open Near Cover Condition */
	EMS_SCError,
	EMS_Diagonostic,
	EMS_NonRecoveryError,
	/* For Maximum State Sign */
	EMS
};

#define MAX_PAGE_SUPERVISOR 6
	/* The number of page puservisor structures */
	/* This number is required 4 or 8 for capacity of memory and (Get_PgSupParamAddress (Uint8 n) function */

// stefan add page ptrs #define	RIPDATA 9 
#define	RIPDATA 14

/* For Paper */
// typedef union	MediaDiscript  {
	// Uint8 BYTE;
	// struct {
		// Uint8	Type:		4;
		// Uint8	Weight:		2;
		// Uint8	Texture:	2;
	// } BIT;
// } MEDIA_DISCRPT;
	
typedef union	MediaDiscript  {
	Uint8 BYTE;
	struct {
		Uint8	Texture:	2;
		Uint8	Weight:		2;
		Uint8	Type:		4;
	} BIT;
} MEDIA_DISCRPT;

/****************************************************
 TRAY Status
****************************************************/



struct _TRAY_DATA
{
	Uint8 oldTrayStatus;
	Uint8 newTrayStatus;
	Uint8 emptyTrayStatus;
};
typedef struct _TRAY_DATA 	TRAY_DATA;

typedef struct _STRUCT_TRAY_STATUS
{
	TRAY_DATA Tray1; //tray1
	TRAY_DATA Tray2; //tray2
	TRAY_DATA Tray3; //tray3
	TRAY_DATA Tray4; //mpt
}STRUCT_TRAY_STATUS;

#define TRAY1CHATTERING 100
#define TRAY1HIGHLOWCHATTERING 2
#define TRAY1SIZECHATTERING 2
#define TRAY2CHATTERING 2
#define TRAY3CHATTERING 2
#define MPTCHATTERING 200 // 2sec

#define STAT_TRAY_ATTACHED			0x80
#define STAT_TRAY_MEDIA_SIZE		0x70
#define STAT_TRAY_MEDIA_LEVEL		0x03

#define TRAY_ATTACHED			0x80

#define TRAY_MEDIA_UNKNOWN		0x10
#define TRAY_MEDIA_LT			0x20
#define TRAY_MEDIA_EXEC			0x30
#define TRAY_MEDIA_A4			0x40
#define TRAY_MEDIA_B5			0x50
#define TRAY_MEDIA_LG			0x60
#define TRAY_MEDIA_A5			0x70

#define TRAY_MEDIA_LOW			0x02
#define TRAY_MEDIA_NORMAL		0x03
#define TRAY_MEDIA_EMPTY		0x01

#define GetTrayAttachInfo(x)			( ((x) & STAT_TRAY_ATTACHED) ? TRUE : FALSE )
#define SetTrayAttachOff(x)				( x &= ~STAT_TRAY_ATTACHED )
#define SetTrayAttachOn(x)				( x |= STAT_TRAY_ATTACHED )

#define GetTrayMediaInfo(x)				( (x) & STAT_TRAY_MEDIA_SIZE )
#define SetTrayMediaSize(x,val)			( x = (x & ~STAT_TRAY_MEDIA_SIZE) | val )

#define GetTrayMediaLevel(x)			( (x) & STAT_TRAY_MEDIA_LEVEL )
#define SetTrayMediaLevel(x, y)			( x = (x & ~STAT_TRAY_MEDIA_LEVEL) | ((y) & STAT_TRAY_MEDIA_LEVEL))
#define SetTrayMediaNormal(x)			( x = (x & ~STAT_TRAY_MEDIA_LEVEL) | TRAY_MEDIA_NORMAL )
#define SetTrayMediaLow(x)				( x = (x & ~STAT_TRAY_MEDIA_LEVEL) | TRAY_MEDIA_LOW )
#define SetTrayMediaEmpty(x)			( x = (x & ~STAT_TRAY_MEDIA_LEVEL) | TRAY_MEDIA_EMPTY )

	/* Media Source CD2 */
#define MS_MultiPurposeTray		0x10
#define MS_Tray1				0x11
#define MS_Tray2				0x12
#define MS_Tray3				0x13
#define	MS_Duplex				0x80

#define DB_DUPLEX_UPDATED		0x01
#define DB_TOPMARGIN_UPDATED	0x02
#define DB_LEFTMARGIN_UPDATED	0x03
#define DB_TRAYNO_UPDATED		0x04
#define DB_BACKTOPMARGIN_UPDATED		0x05
#define DB_BACKLEFTMARGIN_UPDATED		0x06
#define DB_BEAMADJUST_UPDATED   0x07
#define DB_VERTICAL_MAGNIFICATION_UPDATED   0x08
#define DB_HORIZONTAL_MAGNIFICATION_UPDATED   0x09
#define DB_A4_LT_GAP_MARGIN   0x0A
#define DB_COPY_LEFTMARGIN_UPDATED   0x0B
#define DB_COPY_BACKLEFTMARGIN_UPDATED   0x0C
#define DB_COPY_TOPMARGIN_UPDATED   0x0D
#define DB_COPY_BACKTOPMARGIN_UPDATED   0x0E
#define DB_SERVOADJUST_UPDATED 0x0F


/* 2001.11.22 Added Full Stack */
#define	Output_Bin				0x90

#define	DEFAULT_PAGE			0x05

	/* Narrow Media CD3 */
enum {
        NarrowWidthMedia = 0,
	NormalWidthMedia

};
	/* Media Type CD4 */
enum {
	MTYPE_Paper = 0,
	MTYPE_Label,
	MTYPE_Envelope,
	MTYPE_CardStock,
	MTYPE_Transparency,
	MTYPE_VinylLabel,
	MTYPE_PostCard,
	MTYPE_Recycled,
	MTYPE_RESERVED_MIN,
	MTYPE_DuplexPaper,
	MTYPE_RoughPaper, /* added for special transfer settings */
	MTYPE_DuplexRoughPaper, /* added for special transfer settings */
	MTYPE_SmoothPaper, /* added for special transfer settings */
	MTYPE_DuplexSmoothPaper, /* added for special transfer settings */
	MTYPE_RESERVED,
	MTYPE_Unknown = 15
};

	/* Media Weight CD4 */
enum {
	MW_Light = 0,
	MW_Normal,
	MW_Heavy,
	MW_RESERVED
};

	/* Media Texture CD4 */
enum {
	MTX_Smooth = 0,
	MTX_Normal,
	MTX_Rough,
	MTX_RESERVED
};

enum {
	Within10PagesDX = 0,
	Within10PagesSX,
	Over10PagesDX,
	Over10PagesSX
};

	/* Media Size CD5 */
// enum {
	// MS_NotDefine = 0,
	// MS_Letter, 				/* 8.5" * 11" */
	// MS_Legal,				/* 8.5" * 14" */
	// MS_B5,					/* 7.2" * 10.1" */
	// MS_A4, 					/* 8.3" * 11.7" */
	// MS_Exec,				/* 7.25" * 10.5" */
	// MS_A5,					/* 5.83" * 8.27" */
	// MS_Custom,
	// MS_Unknown,
	// MS_Envelope_75,			/* 7.5" */
	// MS_Envelope_9,			/* 8.9" */
	// MS_Envelope_10,			/* 9.5" */
	// MS_Envelope_DL,			/* 8.66" */
	// MS_Envelope_C5,			/* 9.02" */
	// MS_Envelope_B5,			/* 9.84" */
	// MS_Envelope_Other,
	// MS_Statement = 19,		/* 8.5" */ 
	// MS_Folio = 20			/* 13" */
// };
		
enum {
	MS_NotDefine = 0,
	MS_A4, 
	MS_A5,
	MS_A5_LEF,
	MS_A6,
	MS_B5, 			
	MS_B6,
	MS_Letter,
	MS_Legal,
	MS_Exec,			
	MS_Statement,
	MS_Folio,			
	MS_Quatro,
	MS_G_Letter,
	MS_G_Legal,
	MS_Card_100by148,
	MS_Card_148by200,
	MS_Card_200by148,
	MS_Envelope_Monarch,
	MS_Envelope_9,
	MS_Envelope_10,
	MS_Envelope_DL,	
	MS_Envelope_C5,	
	MS_Envelope_C6,	
	MS_Custom,
	MS_Unknown,
};

	/* For Darkness Level CD6 */
enum {
	DARKNA = 0,
	DARKLightest,
	DARKLight,
	DARKNormal,
	DARKDark,
	DARKDarkest
};

	/* For Toner Saver CD7 */
enum {
	TONERSaverOFF = 0,
	TONERSaverON
};

	/* For Output Destination CD8 */
enum {
	OutputStandardOrRear = 0,
	OutputDuplexor
};

	/* For Simplex or Duplex */
enum {
	Front = 0,
	Back
};

enum {
	RETURN_TYPE = 0,
	NON_PREBATE_TYPE
};
#if 0 /* [KSF061701] */
enum {
	TLI_ERROR = 0,
	TLI_A40025,
	TLI_A40524,
	TLI_A405T24,
	TLI_A405G24,
	TLI_A400X25,
	TLI_A400S25,
	TLI_A40025_C,
	TLI_A40132_C,
	TLI_A40236_C,
	TLI_C1A40236_C,
	TLI_A40132,
	TLI_A1A401DN32,
	TLI_A40632,
	TLI_A2A406DN32,
	TLI_A402DN36,
	TLI_A407DN36,
	TLI_M40025,
	TLI_M40525,
	TLI_M400X25,
	TLI_M405G25,
	TLI_M40125,
	TLI_M40625,
	TLI_M401X25,
	TLI_M406G25,
	TLI_M400S25,
	TLI_M401S25,
	TLI_M40230,
	TLI_M40335,
	TLI_M40730,
	TLI_M40835,
	TLI_M402X30,
	TLI_M403X35,
	TLI_M407G30,
	TLI_M408G35,
	TLI_M402S30,
	TLI_M403S35,
	TLI_F40025,
	TLI_F40525,
	TLI_F405G25,
	TLI_F400X25,
	TLI_F400S25,
	TLI_C1M40230_C,
	TLI_C2M40335_C,
	TLI_C3M40230_C,
	TLI_C4M40335_C,
	TLI_A400N10,
	TLI_A401N21,
	TLI_A402N21,
	TLI_M401N10,
	TLI_M402N21,
	TLI_M403N21,
	TLI_C5M40125_C,
	TLI_KA40DVT2, /* [KSR040101] */	
	TLI_DEFAULT
};
#endif

#if 1 /* [KSF061701] */
enum {
	TLI_ERROR = 0,
	TLI_A40025,
	TLI_A40524,
	TLI_A405T24,
	TLI_A405G24,
	TLI_A400X25,
	TLI_A400S25,
	TLI_A40025_C,
	TLI_A40132_C,
	TLI_A40236_C,
	TLI_C1A40236_C, // 10
	TLI_A40132,
	TLI_A1A401DN32,
	TLI_A40632,
	TLI_A2A406DN32,
	TLI_A402DN36,
	TLI_A407DN36,
	TLI_M40025,
	TLI_M40525,
	TLI_M400X25,
	TLI_M405G25, // 20
	TLI_M40125,
	TLI_M40625,
	TLI_M401X25,
	TLI_M406G25,
	TLI_M400S25,
	TLI_M401S25,
	TLI_M40230,
	TLI_M40335,
	TLI_M40730,
	TLI_M40835, // 30
	TLI_M402X30,
	TLI_M403X35,
	TLI_M407G30,
	TLI_M408G35,
	TLI_M402S30,
	TLI_M403S35,
	TLI_F40025,
	TLI_F40525,
	TLI_F405G25,
	TLI_F400X25, // 40
	TLI_F400S25,
	TLI_C1M40230_C,
	TLI_C2M40335_C,
	TLI_C3M40230_C,
	TLI_C4M40335_C,
	TLI_A400N10,
	TLI_A401N21,
	TLI_A402N21,
	TLI_M401N10,
	TLI_M402N21, // 50
	TLI_M403N21,
	TLI_C5M40125_C,
	TLI_KA40DVT2, // for kara DVT
	TLI_A610DN40, // for kara
	TLI_A610DN40_C, // for kara
	TLI_A615DN40, // for kara
	TLI_A611DN45,
	TLI_A611DN45_C,
	TLI_A616DN45,
	TLI_A616DNS45, // 60
	TLI_M61040, // for pinetree 
	TLI_M61540, // for pinetree
	TLI_M61140, 
	TLI_M61640,
	TLI_M61245,
	TLI_M61745,
	TLI_M61345,
	TLI_M61845,
	TLI_M611DN40_C,
	TLI_M612DN45_C, // 70
	TLI_A610NA40,
	TLI_A615NA40,
	TLI_A611NA47,
	TLI_A616NA47,
	TLI_A610BR40_L,
	TLI_A611BR47_L,
	TLI_A610BR40_H,
	TLI_A611BR47_H,
	TLI_A610TR40,
	TLI_A611TR47, // 80
	TLI_A610ZA40,
	TLI_A611ZA47,
	TLI_A610TN40,
	TLI_A611TN47,
	TLI_A610AE40,
	TLI_A611AE47,
	TLI_A610IR40,
	TLI_A611IR47,
	TLI_A610RU40,
	TLI_A611RU47, // 90
	TLI_A610IN40_HD,
	TLI_A611IN47_HD,
	TLI_A610IN40,
	TLI_A611IN47,
	TLI_A610MY40,
	TLI_A611MY47,
	TLI_A610SA40,
	TLI_A611SA47,
	TLI_A610AU40,
	TLI_A611AU47, // 100
	TLI_A610AR40,
	TLI_A611AR47,
	TLI_A610ES40,
	TLI_A611ES47,
	TLI_A610PY40,
	TLI_A611PY47,
	TLI_M611NA40,
	TLI_M612NA47,
	TLI_M611BR40_L,
	TLI_M612BR47_L, // 110
	TLI_M611BR40_H,
	TLI_M612BR47_H,
	TLI_M611TR40,
	TLI_M612TR47,
	TLI_M611ZA40,
	TLI_M612ZA47,
	TLI_M611TN40,
	TLI_M612TN47,
	TLI_M611AE40,
	TLI_M612AE47, // 120
	TLI_M611IR40,
	TLI_M612IR47,
	TLI_M611RU40,
	TLI_M612RU47,
	TLI_M611IN40_HD,
	TLI_M612IN47_HD,
	TLI_M611IN40,
	TLI_M612IN47,
	TLI_M611MY40,
	TLI_M612MY47, // 130
	TLI_M611SA40,
	TLI_M612SA47,
	TLI_M611AU40,
	TLI_M612AU47,
	TLI_M611AR40,
	TLI_M612AR47,
	TLI_M611CO40,
	TLI_M612CO47,
	TLI_M611ES40,
	TLI_M612ES47, // 140
	TLI_M611PY40,
	TLI_M612PY47,
	TLI_A413DN36,
	TLI_M610NA40,
	TLI_M610BR40_L,
	TLI_M610IR40,
	TLI_M610RU40,
	TLI_M610IN40_HD,
	TLI_M610IN40,
	TLI_M610MY40,
	TLI_M610SA40,
	TLI_M610AU40,
	TLI_M610AR40,
	TLI_M610CO40,
	TLI_M610ES40,
	TLI_M610PY40,
	TLI_A610MX40,
	TLI_A611MX47,
	TLI_M611MX40,
	TLI_M612MX47,
	TLI_A610PE40,
	TLI_A611PE47,
	TLI_M611PE40,
	TLI_M612PE47,
	TLI_A610CL40,
	TLI_A611CL47,
	TLI_M611CL40,
	TLI_M612CL47,
	
	TLI_A610DN40H,	// kara
	TLI_A615DN40H,
	TLI_A610LV40H,
	TLI_A610HV40H,
	TLI_A610DN40_CH,
	
	TLI_A611DN45H,	// kara-s
	TLI_A616DN45H,
	TLI_A616DNS45H,
	TLI_A611LV47H,
	TLI_A611HV47H,
	TLI_A611DN45HHU,
	TLI_A611DN45HH,
	TLI_A611DN45_CH,
	TLI_P247B01,
	
	TLI_M61140H,	// pinetree
	TLI_M61640H,
	TLI_M611LV40H,
	TLI_M611HV40H,
	TLI_M611DN40_CH,
	
	TLI_M61245H,	// pinetree-s
	TLI_M61745H,
	TLI_M612LV47H,
	TLI_M612HV47H,
	TLI_M61245HH,
	TLI_M612DN45_CH,
	TLI_M247B01,
	
	TLI_M61040H,	// pinetree-l
	TLI_M61540H,

	TLI_DEFAULT
};

enum {
	DR_WRONGTLI = 0,
	DR_TEMPORARILY = 1,
	DR_A610R40K =2,
};

enum {
	TN_WRONGTLI = 0,
	TN_RNDMASTER = 1,
	TN_S400T2HK = 2,
	TN_S400T5K = 3,
	TN_S400T8K = 4,
	TN_S400ST2HK = 5,
	TN_S400ST5K = 6,
	TN_LA25T1HKA = 7,
	TN_LA25T1HKB = 8,
	TN_LA25T2HK = 9,
	TN_LA25T8K = 10,		//10
	TN_LA30T2HK = 11,
	TN_LA35T5K = 12,
	TN_LA35T8K = 13,
	TN_LA25T1HK_C = 14,
	TN_LA30T2HK_C = 15,
	TN_LA35T5K_C = 16,
	TN_S400T2HK_C = 17,
	TN_S400T5K_C = 18,
	TN_S400T8K_C = 19,
	TN_M400T2HK = 20,	//20
	TN_M400T5K = 21,
	TN_M400T8K = 22,
	TN_M400ST2HK = 23,
	TN_M400ST5K = 24,
	TN_M400ST8K = 25,
	TN_M012T1HKA = 26,
	TN_M012T2HK = 27,
	TN_M012T8K = 28,
	TN_M032T1HK = 29,
	TN_M032T2HK = 30,	//30
	TN_M012T1HKB = 31,
	TN_M012T2HKB = 32,
	TN_M032T2HK_C = 33,
	TN_M400T2HK_C = 34,
	TN_M400T5K_C = 35,
	TN_M400T8K_C = 36,
	TN_F400T2HK = 37,
	TN_F400T5K = 38,
	TN_F400ST2HK = 39,
	TN_F400ST5K = 40,	//40
	TN_M015T1HKA = 41,
	TN_M015T1HKB = 42,
	TN_NA400T2HKR = 43,
	TN_NA400T5KR = 44,
	TN_NA400T8KR = 45,
	TN_NA400T2HKN = 46,
	TN_NA400T5KN = 47,
	TN_NA400T8KN = 48,
	TN_LA30T2HK_N = 49,
	TN_NM400T2HKR = 50,	//50
	TN_NM400T5KR = 51,
	TN_NM400T8KR = 52,
	TN_NM400T2HKN = 53,
	TN_NM400T5KN = 54,
	TN_NM400T8KN = 55,
	TN_M032T2HK_N = 56,
	TN_LA40T12K = 57,
	TN_AMTONERCART = 58,
	TN_A610T3K = 59,
	TN_A610T6K = 60,		//60
	TN_A610T12K = 61,
	TN_LA40T6K = 62,
	TN_LA40T9K = 63,
	TN_M610T3K = 64,
	TN_M610T6K = 65, 
	TN_M610T12K = 66,
	TN_M013T6K = 67,
	TN_M013T9K = 68,
	TN_A610T13K = 69,
	TN_M610T13K = 70,		//70
	TN_LA40T6K_C = 71,
	TN_A610T3K_C = 72,
	TN_A610T6K_C = 73,
	TN_A610T13K_C = 74,
	TN_M013T3K = 75,
	TN_A610T3K_W = 76,
	TN_A610T6K_W = 77,
	TN_A610T13K_W = 78,
	TN_LA40T6K_W = 79,
	TN_ABBA2T9K = 80,
	TN_M013T3K_C = 81,
	TN_M610T3K_C = 82,
	TN_M610T6K_C = 83,
	TN_M610T13K_C = 84,
	TN_M013T3K_W = 85,
	TN_LA40T9K_W = 86,
	TN_L236T3K = 87,
	TN_L236T6K = 88,
	TN_A410T3K = 89,
	TN_A410T6K = 90,
	TN_A410T9K = 91,
	TN_LA40T3K_W = 92,
	TN_A616DNST6K = 93,
	TN_A616DNST13K = 94,
	TN_THM247B01 = 95,
	TN_P247T3K_WV = 96,
	TN_M247T3K_WV = 97,
	TN_THM247A01 = 98,
};

#define NumOfKeyData	16
#endif

enum {
	JogType1 = 1,
	JogType2,
	JogType3,
	JogType4,
	JogType5,
	JogType6,
	JogType7,
	JogType8,
	JogType9
};

enum {
	ABBA_OAK_L,
	ABBA_OAK_M,
	ABBA_OAK_H
};

enum {
	LOCAT_KOREA,
	LOCAT_CHINA,
	LOCAT_NA_LT,
	LOCAT_NA_A4,
	LOCAT_UNKNOWN = 100,
};

enum {
	MAIN_CLOCK_OLD,
	MAIN_CLOCK_123MHZ,
	MAIN_CLOCK_196MHZ
};

#define MICR_TONER	0x01

// typedef struct STRUCT_STARTFROMPOWERSAVE
// {
	// Boolean			bePrintFromPowerSave;	// engine
	// Boolean			fMainMotorState;			// both
	// Boolean			EarlyStartCmd;			// engine
	// Boolean 		PrintPageCmd;			// engine
	// Boolean			AlreadyGetTargetTemp;	// fuser
	// Uint8			WakeUp_Temperature;		// both
// }Struct_StartFromPowerSave;

/* For Page Supervisor */
typedef struct PAGESUPERVISOR {
	Uint8 			Active;
	
	Uint32 			PageID;
	Uint32 			Source;
	Uint32 			Narrow;
	MEDIA_DISCRPT	MediaDescription;
	Uint32			Darkness;
	Uint32			TonerSaver;
	Uint32			OutputDestination;
	Uint32			cheat_gap;
	
	Uint32			Length;
	
	Uint8			FuserTemp;
	Uint8			Flags;
	
	Uint8			T1_State;
	Uint32			T1_Value;
	Uint32			T1_Comp;
	Uint8			T2_State;
	Uint32			T2_Value;
	Uint32			T1Delay;
	Uint32			T2Delay;
	Uint8			SequenceID;
	Uint8			fFullStackCheckSize;
	Uint8			fNoPaperOnTray;
	Uint8			PowersaveToPrint;
	Uint8			OptionPrePick;
	Uint8			EmptySourceTray;
	struct			PAGESUPERVISOR *PageAhead;
	page_handle_t	*page_info;
	page_handle_t	*page_done_info;
	Uint8			fvideodone;
	Uint8 			fvideoOn;
	Uint8			CleaningTime_NoVideo;
	Uint8			RecycledPaperEnable;
	
	int				InputDetectTime;
	int				InputReleaseTime;
	int				ExitDetectTime;
	int				ExitReleaseTime;
} PageSupervisor ;

/* For Page Supervisor Sequence */
enum {
	PSS_NONE,	/* 0 sequence*/
/* POR EP Initialize Sequence */
	PSL_EP_InitializeFor_PreHeating,
	PSL_Dev_On_BeforServoSearch,
	PSL_xfr_On_BeforServoSearch,
	PSL_ServoSearchStart_PreHeating,
	PSL_ServoSearchDone_PreHeating,
	PSL_Dev_On_PreHeating,
	PSL_Dev_Ready_PreHeating,
	PSL_PreHeating_Done,
	
	PSL_StartPaperPickupSequence_Laminating,
	PSL_FinishPaperPickup_Laminating,
	PSL_DetectStartToPins_Laminating,
	PSL_CheckPinsToPins_Laminating,
	PSL_Finish_Laminating,
	PSL_StartEndProcess_Laminating,
	PSL_End_Laminating,
	
	PSS_EP_InitializeFor_POR,
	/* Core Search for Transfer Bias */
	/* Start EP Timing */
	PSS_ChargeToAPC,
	PSS_ContinueAPC,
	PSS_Dev_On,
	PSS_EraseOn,	/* 5sequence*/
	PSS_Xfr_On,
	
	PSS_MotorRun_Start,
	PSS_MotorRun_ChargeToAPC,
	PSS_MotorRun_ContinueAPC,
	PSS_MotorRun_Dev_On,	/* 10sequence */
	PSS_MotorRun_Xfr_On,

	PSS_StartJogMode,

	/* Servo Search for Transfer Bias */
	PSS_StartServoSequence,
	PSS_XfrServoStepUp,
	PSS_XfrServoStepDown,			/* 15sequence */
	PSS_XfrServoStep1Up,	
	PSS_XfrEndServo,
	
	PSS_PaperCleaning,
	PSS_PaperCleaningOnPins,
	PSS_PaperCleaningOnXfr,			/* 20sequence */
	
	/* Syncronization for EP and Motor */
	PSS_MotorWarmup,
	PSS_HSync_Detact,
	PSS_Developer_Off,
	PSS_Developer_On,
	PSS_Transfer_On,				/* 25sequence */
	/* Cleaning Sequence of POR Sequence */
	/* End of Initiaize Sequence */	
	PSS_FinishInitPOR,			
	PSS_FuserWarmupForPOR,	
	
/* 1st Print Sequence */
	/* Print Start Sequence */
	PSPS_StartPrintSequence,	
	PSPS_StartPrinter,
	

/* 05.07 */
/* Cleaning after 10min continuous printing */
#if 1	/* Add '99/June/17 For Interpage Cleaning for charge roll */
	PSPS_StartEPCleaning,			/* 30sequence */
		/* Wait for Starting EP cleaning process */
	PSPS_ChargeCleaning,				
		/* Wait for Turning off a Charge bias */
	PSPS_Dev_Off,			
		/* Wait for Turning off a Developer bias */
	PSPS_PickAftEPCleaning,			
		/* Continue 2 cycles of Chaging roll */
#endif	

	PSPS_ChargeToAPC,
	PSPS_ContinueAPC,			/* 35sequence */
	PSPS_Dev_On,				
	PSPS_EraseOn,		
	PSPS_Xfr_On,	
	
	/* Servo Search for Transfer Bias */
	PSPS_StartServoSequence,
	PSPS_XfrServoStepUp,			/* 40sequence */
	PSPS_XfrServoStepDown,			
	PSPS_XfrServoStep1Up,	
	PSPS_XfrEndServo,	
	
	PSPS_PaperCleaning,
	PSPS_PaperCleaningOnPins,		/* 45sequence */
	PSPS_PaperCleaningOnXfr,		
	/* EP Initialize for Print Sequence */
	/* Servo Search for Transfer Bias */
	/* Syncronization for EP and Motor */
	PSPS_MotorWarmup,
	PSPS_MotorWarmupForPwsToPrint,
	PSPS_HSync_Detact,
	PSPS_Developer_Off,				/* 50sequence */
	PSPS_Developer_On,
	PSPS_Transfer_On,
	PSPS_FinishInit,			
	
	/* Entry Point For Continuous Printing */
	PSPS_PickupForContinue,	
			/* Wait for Starting Continuous Printing Process */


	/* Paper Pick Up */
	PSPS_StartPaperPickupSequence,		/* 55sequence */
	PSPS_OptionResync,
	PSPS_ContinueTray2Pickup,	
	PSPS_FinishPaperPickup,
	
	PSPS_BoostCharge,

	/* Paper Transfer befor Paper Input */
	PSPS_DetectStartToPins,		/* 60sequence */
	
	PSPS_ToNextPrintReady,				
	PSPS_WaitNextPickup,
	
	/* Timing for Print Process */
	PSPS_DetectNarrowMedia,   /* 07.24 To detect Narrow Media, Added by Kyungbal */
	PSPS_Dev_OnPrinting,
	PSPS_EraseOnPrinting,			/* 65sequence */
	PSPS_Xfr_OnPrinting,				
	
	PSPS_CheckPinsToPexs,
	/* Paper Jam Sensing for Paper Length at Paper Input Sensor */	
	PSPS_CheckPinsToPins,
        PSPS_FeedJamOutFuser,     /* VerADS */
	PSPS_WaitPinsOff,			/* 70sequence */

#if IPG_ENABLE
	PSPS_SendOptionTrayNotifyBeforeVideoStart,	
	PSPS_VideoOff,
	PSPS_VideoOn, // 130312, namja
	PSPS_NextVideoPage,
	PSPS_EndToXfr,				/* 75sequence */
#else
	PSPS_EndToXfr,
#endif
	PSPS_EndToErase,							
	PSPS_SelectExitMotorSpeed,
	
	/* Paper Jam Sensing for Paper Length at Paper Exit Sensor */
	PSPS_CheckPexsToPexs,
	/* PSPS_EndOfPrintPage, */	/* Simplex End Of Print */
	
/* Duplex Paper Sequence */
	PSPS_StopDuplexBackSide,	
	PSPS_StartDuplexor,				/* 80sequence */
	PSPS_CheckDis,							
	/* Added 2001.05.09 For Duplex Hold Point */
	PSPS_ToDuplexHoldPoint,	
	PSPS_DuplexFrontPrintReady,

// #if DUPLEX_COOLING
//#if MAC_TYPE_MH
	PSPS_DuplexCooling,
	PSPS_StartCooling,					/* 85sequence */
	PSPS_WaitCooling,						
	PSPS_WaitFuserTemp,
//#endif	
	PSPS_EndOfPrintPage,	/* Duplex End Of Print */
	PSPS_FullStackCheckPoint,
	PSPS_FullStackOn,
	PSPS_FullStackOff,				/* 90sequence */
	
/* Paper Exit Sequence */
	PSPS_StartExtendedFeed,					
	PSPS_EmptyCheckWaiting,
	PSPS_ToHopper,	
	
/* End of Process Sequence */
	/* EP Cleaning Sequence */
	PSS_StartEPCleaning,
	PSS_ChargeCleaning,					/* 95sequence */
	PSS_EPCleaning,							
	PSS_Dev_Off,
	PSS_StopMainMotor,
	PSS_RunMainMotorForJobFinish,
	PSS_FinishEPCleaning,				/* 100sequence */
	
	/* Engine Idle State */
	/* Main Motor Off, Mirror Motor On, Fuser On */
	PSS_Idle,
	PSS_StopJogMode,						
	
	/* Stop Printer Sequence */
	PSS_StopPrinterSequence,
	PSS_StopSequence,
/* Print Sequence Error */
	PSS_PrinterError, 	//PS_PrintSequenceError		/* 105sequence */
	PSS_ExitError,
	PS_Error								

};

#define DUPLEXPRINT300TIME 40*6000
#define DUPLEXPRINTSLOW 15*6000
#define DUPLEXPRINTSLOWPAGES 300

// #define BLDC_DEFAULT_SPEED_L_OLD		43927
// #define BLDC_DEFAULT_SPEED_M_OLD		33574	//29337//30330(35ppm)	//29337 (36ppm)
// #define BLDC_DEFAULT_SPEED_H_OLD		29337	//30330(35ppm)	//29337 (36ppm)

// Main Clock 123.219308MHz (1250) // 196.068750 MHz (2350)
// rpm =>1683 rpm
// Hz => rpm*5/6 = 1683*5/6 = 1402.5
// PWM Clock = 123219308 / ( (43927+1) * 2 ) =  1402.5Hz		// 43927 = 0xAB97	 // 1250 Case
// PWM Clock = 196068750 / ( (34949+1) * 4 ) =  1402.5Hz		// 21963 = 0x55CB    // 2350 Case

#define BLDC_DEFAULT_SPEED_L_123MHZ		43927		//21963
// #define BLDC_DEFAULT_SPEED_M_123MHZ		33574		//16786	
#define BLDC_DEFAULT_SPEED_M_123MHZ		33004		//16786	
// #define BLDC_DEFAULT_SPEED_H_123MHZ		29337		//14668	
#define BLDC_DEFAULT_SPEED_H_123MHZ		28992		//14668	

#define BLDC_DEFAULT_SPEED_L_196MHZ		34949
// #define BLDC_DEFAULT_SPEED_M_196MHZ		26631
#define BLDC_DEFAULT_SPEED_M_196MHZ		26225	//26315
// #define BLDC_DEFAULT_SPEED_H_196MHZ		23451
// Kara S 46ppm : rpm -> 3242 , paperspeed -> 296.7mm/s, Hz -> 2701Hz
#define BLDC_DEFAULT_SPEED_H_196MHZ		18142//22860	//23066

#define	MIR_MTR_DEFAULT_SPEED_2B_L_123MHZ		33651
#define	MIR_MTR_DEFAULT_SPEED_PRAM_L_123MHZ		14200
// #define	MIR_MTR_DEFAULT_SPEED_PRAM_M_123MHZ		26018
#define	MIR_MTR_DEFAULT_SPEED_PRAM_M_123MHZ		25576
// #define	MIR_MTR_DEFAULT_SPEED_PRAM_H_123MHZ		22754
#define	MIR_MTR_DEFAULT_SPEED_PRAM_H_123MHZ		22230

#define	MIR_MTR_DEFAULT_SPEED_2B_L_196MHZ		53813
#define	MIR_MTR_DEFAULT_SPEED_PRAM_L_196MHZ		22595
// #define	MIR_MTR_DEFAULT_SPEED_PRAM_M_196MHZ		41400
#define	MIR_MTR_DEFAULT_SPEED_PRAM_M_196MHZ		40696
// #define	MIR_MTR_DEFAULT_SPEED_PRAM_H_196MHZ		36387	
#define	MIR_MTR_DEFAULT_SPEED_PRAM_H_196MHZ		35548

#define	NumberOfBoundary	20
#define	Modifier_HeartBeat  1	

#define	MAX_8BIT_NUMBER		255
#define	DarknessNumber		6
#define	TonerSaverNumber	2
#define	TransferSpecNumber	160
#define	MediaNumber		14
#define	TransferPWMRange	4
#define	DarknessLevel		11
#define	NumberOfTempState	4
#define NumberOfFuserParameter	47
#define NumberOfEngineParameter	20
#define TempStatusNumber	6
#define PageRange			3	//4

/* PWM Value */
#define	PWM_Minimum	0
	/* Transfer PWM Value */

/* EVT1.02  By Song 
#define	LIMTE_FOR_TRANSFER_SERVO_SEARCH_PWM	210
*/
#define LIMTE_FOR_TRANSFER_SERVO_SEARCH_PWM 	255//200	//164 
#define	TRANSFER_INTERPAGE_SLOPE	5		/*816*/
#define	TRANSFER_INTERPAGE_OFFSET	0

#if MAC_TYPE_MH
#define	DefaultCharge_PWM			200
#else
#define	DefaultCharge_PWM			213		// ABBA Default Charge PWM < 213(Duty) = -1650(OutputVoltage) >
#endif
#define	DefaultCharge_PWM_Laminating		138

#define Num_ChargerTable 4
#define Num_DeveloperTable 5
#define Num_TransferTable 3
	
//#define	DefaultDeveloper_PWM		228 /* 296		 -610V */ 
#define	DefaultTransfer_PWM			114 /* 148		 1200V */ 
//#define	TransferCORE_PWM			53 		/* -200v Drum Core */
// ABBA Transfer PWM < Duty = (OutputVoltage+1350) / 53.5 >
#define	TransferCORE_PWM			55	// ABBA NEW!!!  -200v
#define	AdderForTransferServoSearch 7	
#define	TransferBiasPWMForPOR		1 // 0
#define	TransferBiasPWMForCleaning	1 // 0
	/* Developer PWM Value */
/* 2001.10.10 ver1.02 */
//#define	DeveloperBiasPWMForNoPrint	86	/* 112 -30v */ 
//#define	DeveloperBiasPWMForCleaning	129 /* 167 -199v */ 
// ABBA Developer PWM < Duty = (OutputVoltage-52.65) / -10.53 >
#define	DefaultDeveloper_PWM		161	// ABBA NEW!!! -610v
#define	DeveloperBiasPWMForNoPrint	20	// ABBA NEW!!!  -30v
#define	DeveloperBiasPWMForCleaning	20	// 11Aug09 Cleaning -30v //61  // ABBA NEW!!!  -200v
#define	DeveloperBiasPWMForStop		0
#define	DeveloperBiasPWMForLaminating		50
#define	DeveloperBiasPWMForLaminatingReady		159

#if ABBA2
#define TransferBiasPWMForInterpageGap	79
#else
#define TransferBiasPWMForInterpageGap	69
#endif
#define TransferBiasPWMForInterpageGap_Laminating 79
/* Developer Darkness Value */

/* EVT1-1 */	/* 2001.10.11 EP Condition */
#define	TonerSaverDevPWM_Lightest	181 /* 	235  -414V */  
#define	TonerSaverDevPWM_Light		188 /*	244  -442V */  
#define	TonerSaverDevPWM_Normal	   	194 /*	252  -467V */  
#define	TonerSaverDevPWM_Dark	    	201 /* 	261  -495V */ 
#define	TonerSaverDevPWM_Darkness	208 /*	270  -524V */ 
#define	DevPWM_Lightest			214 /*  278  -549V */
#define	DevPWM_Light			222 /*  288  -581V */
#define	DevPWM_Normal			231 /*  300  -618V */
/*#define	DevPWM_Normal		281 */
#define	DevPWM_Dark			241 /*  312  -656V */ 
#define	DevPWM_Darkness			248 /*  322  -688V */ 
	/* Developer Darkness Value With Toner Saver */

	/* Physical Paper Length (mm) */
#define	PL_Letter			279
#define	PL_Legal			356
#define	PL_B5				250
#define	PL_A4				297
#define	PL_Exec				267
#define	PL_A5				210
#define	PL_Custom			PL_Legal
#define	PL_Unknown			PL_Legal
#define	PL_Envelope_75		190
#define	PL_Envelope_9		225
#define	PL_Envelope_10		241
#define	PL_Envelope_DL		220
#define	PL_Envelope_C5		229
#define	PL_Envelope_B5		250
#define	PL_Envelope_Other	PL_Legal
#define	PL_Statement		216
#define	PL_Folio			330
#define	PL_Reserved			PL_Legal
#define PL_A6				148
#define PL_Card_100by148	148
#define PL_Card_200by148	148
#define PL_A5_LEF			148
#define PL_Envelope_C6		162
#define PL_B6				182
#define PL_Envelope_Monarch	190
#define PL_Card_148by200	200
#define PL_Quatro			254
#define PL_G_Letter			267
#define PL_G_Legal			356

#define	SERVO_ONE_UP_STEP	1
#define	SERVO_UP_STEP	5
#define	SERVO_DOWN_STEP	10

#if MAC_TYPE_L
#define	MOTORSPEED				154.03
#define PAPERSPEED				154.03
#define FEEDMOTORSPEED			154.03
#elif MAC_TYPE_M
#if ABBA2
#define	MOTORSPEED				205
#define PAPERSPEED				205
#define FEEDMOTORSPEED			205
#else
#define	MOTORSPEED				269.6
#define PAPERSPEED				269.6
#define FEEDMOTORSPEED			269.6
#endif
#else
#if ABBA2
#define	MOTORSPEED				233.37		
#define PAPERSPEED				233.37
#define FEEDMOTORSPEED			233.37
#else
#define	MOTORSPEED				296.7		
#define PAPERSPEED				296.7		
#define FEEDMOTORSPEED			296.7	
#endif
#endif
#define MOTORSPEED_L			165
#define PAPERSPEED_L			165	
#define FEEDMOTORSPEED_L		165	
#define MOTORSPEED_R1200			78.15

#define MOTORSPEED_LAMINATE			32.3
#define PAPERSPEED_LAMINATE			32.3
#define FEEDMOTORSPEED_LAMINATE		32.3

#define NarrowMediaBoundary		(Uint32)(NarrowLengthBoundary / MOTORSPEED)
#define NarrowMediaBoundary_QUIET		(Uint32)(NarrowLengthBoundary / MOTORSPEED_L)
#define NarrowMediaBoundary_R1200		(Uint32)(NarrowLengthBoundary / MOTORSPEED_R1200)
	
//~ #if MAC_TYPE_MH
#define NarrowLengthBoundary	27300
//~ #else
//~ #define NarrowLengthBoundary	24500
//~ #endif

/* Second Pick Parameter */
// #define FROM_MPF_TO_PIN_TIME				77
// #define FROM_TRAY1_TO_PIN_TIME				125
// #define FROM_TRAY2_TO_PIN_TIMtemp_media_sizeE				195

#define FROM_MPF_TO_PIN_TIME		(Uint32)(FROM_MPF_TO_PIN / MOTORSPEED)
#define FROM_TRAY1_TO_PIN_TIME		(Uint32)(FROM_TRAY1_TO_PIN / MOTORSPEED)
#define FROM_TRAY2_TO_PIN_TIME		(Uint32)(FROM_TRAY2_TO_PIN / MOTORSPEED)
#define FROM_TRAY3_TO_PIN_TIME		(Uint32)(FROM_TRAY3_TO_PIN / MOTORSPEED)

#define FROM_MPF_TO_PIN_TIME_L		(Uint32)(FROM_MPF_TO_PIN / MOTORSPEED_L)
#define FROM_TRAY1_TO_PIN_TIME_L		(Uint32)(FROM_TRAY1_TO_PIN / MOTORSPEED_L)
#define FROM_TRAY2_TO_PIN_TIME_L		(Uint32)(FROM_TRAY2_TO_PIN / MOTORSPEED_L)
#define FROM_TRAY3_TO_PIN_TIME_L		(Uint32)(FROM_TRAY3_TO_PIN / MOTORSPEED_L)

#define FROM_MPF_TO_PIN				10700
#define FROM_TRAY1_TO_PIN			20703//26703//16950
#define FROM_TRAY2_TO_PIN			32097//27400
#define FROM_TRAY3_TO_PIN			38097//48800//54800
	
#define LIMIT_TRAY1_TO_PIN_TIME		(Uint32)(LIMIT_TRAY1_TO_PIN / MOTORSPEED)
#define LIMIT_TRAY2_TO_PIN_TIME		(Uint32)(LIMIT_TRAY2_TO_PIN / MOTORSPEED)
#define LIMIT_TRAY3_TO_PIN_TIME		(Uint32)(LIMIT_TRAY3_TO_PIN / MOTORSPEED)

#define LIMIT_TRAY1_TO_PIN_TIME_L		(Uint32)(LIMIT_TRAY1_TO_PIN / MOTORSPEED_L)
#define LIMIT_TRAY2_TO_PIN_TIME_L		(Uint32)(LIMIT_TRAY2_TO_PIN / MOTORSPEED_L)
#define LIMIT_TRAY3_TO_PIN_TIME_L		(Uint32)(LIMIT_TRAY3_TO_PIN / MOTORSPEED_L)

#if MAC_TYPE_L
#define LIMIT_TRAY1_TO_PIN			20780
#define LIMIT_TRAY2_TO_PIN			32330
#define LIMIT_TRAY3_TO_PIN			60000
#elif MAC_TYPE_M
#define LIMIT_TRAY1_TO_PIN			26203
#define LIMIT_TRAY2_TO_PIN			38097//32880
#define LIMIT_TRAY3_TO_PIN			60000
#else
#define LIMIT_TRAY1_TO_PIN			26203//26703//25578//21950
#define LIMIT_TRAY2_TO_PIN			38097//36197//32880
#define LIMIT_TRAY3_TO_PIN			60000
#endif
	
/* Motor Run Parameter after Jam */
#define RUNNING_TIME_AFTER_JAM				85
#define STOP_TIME_AFTER_JAM					86
#define STOP_WAINTING_TIME					97
#define BWD_START_TIME						98
#define BWD_STOP_TIME						102
	
#define RUNNING_TIME_AFTER_JAM_L			85
#define RUNNING_TIME_AFTER_JAM_M			42//61
#define RUNNING_TIME_AFTER_JAM_H			38//54
	
/* Physical Layout */ /* mm * TenTimes */
	/* Roll */
#define	Charge2Roll	9299	/* 1782 */	/* PI14.8 * 2 = 92.99mm */
#define	Xfr1Roll	5316	/* 1782 */	/* PI16.92 = 53.16mm */
#define	Fuser1Roll	7850	/* PI25, 78.5mm */
#define	Drum1Roll	9420	/* PI30, 94.2mm */
	/* Paper Feed */
#define	MPFToPaper	0		/* 0mm */
#define	MPFToPins	11982	//15500	//13038	/* 156.0572mm */
#define Pad1Inch        2540    /* 25.4 mm */

#define	T1ToPaper	0	
/*#define T1ToPins	19502*/
/*#define T1ToPins	21202*/	/* 187mm */
#define T1ToPins	19828	/* 202.13mm */
#define	T2ToPaper	0
/*#define T2ToPins	(27400 + 2540)*/ /* 274mm measured + 1 inch padding until DVT trays characterized BKB */			/* 690L */
#define T2ToPins	25724 /* 235mm */

#define	T3ToPaper	0
#define T3ToPins	25724 /* 235mm */

/* Paper Input Sensor To Process */
#define PinsToXfr		4632					/* 46.324 */
// #define PinsActuate	700   						/* 7mm  */
#if ABBA2
#if MAC_TYPE_M
#define PinsActuate		1600 						/* 7mm  */
#else
#define PinsActuate		1700//1000 						/* 7mm  */
#endif
#else
#define PinsActuate		1800  //1200 						/* 7mm  */
#endif
#define DividerActuate	1600						/* 16mm */
#define PinsToDev	(PinsToXfr - DevToXfr) 		/* 43mm - 24.347mm */   
	/* Process Layout */
#define ChargeToLD			916					/* 9.164mm */
#define ContinueAPC			500					/* 5mm */
#define ChargeToDev			2407		/* 24.078mm */
#define ChargeToDevOn		3000		// 25.853mm	//ABBA NEW!!! 07May09 yjlee
#if ABBA2
#define ChargeToDevOnForPrint		5200		// 25.853mm	//ABBA NEW!!! 07May09 yjlee
#else
#define ChargeToDevOnForPrint		4500		// 25.853mm	//ABBA NEW!!! 07May09 yjlee
#endif
#define DevOnToXfrServo		7802		// 78.027mm	//ABBA NEW!!! 07May09 yjlee
#define ChargeToCleaner			6000/*7797*/		/* 77.972mm */
#define APC_EndToDev		(ChargeToDev - ChargeToLD - ContinueAPC)	/* 22.9mm - 5mm */
#define LDtoDev				(ChargeToDev - ChargeToLD)  /* 22.9 - 6.67*/
#define DevToXfr			2434				/* 24.347mm */
#define XfrToErase			785					/* 7.85mm */
#define	XfrToCleaner		3095				/* 30.95mm */
#define XfrToCharge			4577				/* 45.775mm */
#define	XfrToFuser			12000				/* 120mm */
//#define XfrToPexs			15400				/* 154mm */
#define XfrToPexs			15260			//20000				/* 154mm */
#define	EraseToPexs			(XfrToPexs - XfrToErase)
	/* Paper Exit Sensor */
#define PexsToDivider		2700
#define PexsToExitRoller	3400				/* 34mm */
#define	Exit1RollToExit2Roll	9000			/* 90mm */
#define PexsToHopper		(PexsToExitRoller + Exit1RollToExit2Roll)     /* 34mm + 90mm */
	/* Duplex */
#define	Exit1RollToDis		17780				/* 132mm + 45.8mm */
#define	DisToHoldPoint		/*17000*//*12700*/14909				/* 54.2mm + 61.2mm */
#define	HoldPointToPins		12860
	/* EP Cleaning Layout */
#define WaitCleaning		11300
/*#define CleaningTime		(Charge2Roll + ChargeToDev/2 + DevToXfr + XfrToCleaner)*/
#define CleaningTime		(Charge2Roll + ChargeToCleaner)

/* VerADS */
#define FuserLabelJamFeedDist   48300   /* 19 inches or 483mm */

#define Length_5mm			500
#define Length_7mm			700
#define Length_10mm			1000

#define Length_84mm                     8400
#define Length_25mm                     2500

#define	Gap22mm				2200				/* 20mm */
#define Gap25mm				2540				/* 25.4mm */
#define Gap28mm				2800				/* 28mm */
#define PhysGapCorrection		-100
#define Gap32mm				(3200 + PhysGapCorrection)	/* 32mm */
#define Gap53mm				(5300 + PhysGapCorrection)	/* 53mm */	// 14Aug09 : CHG 1 (new)
#define Gap68mm				(6800 + PhysGapCorrection)
#define Gap71mm				(7100 + PhysGapCorrection)
#define Gap75mm				(7500 + PhysGapCorrection)
#define Gap75mm_cheat				(7500 + PhysGapCorrection + 500)
#define Gap100mm			(10000 + PhysGapCorrection)
#define Gap105mm			(10500 + PhysGapCorrection)
#define Gap110mm			(11000 + PhysGapCorrection)
#define Gap128mm			(12850 + PhysGapCorrection)
#define Gap140mm			(14000 + PhysGapCorrection)
#define Gap156mm			(15600 + PhysGapCorrection)
// #define Gap208mm			(20800 + PhysGapCorrection)	
#define Gap166mm			(16600 + PhysGapCorrection)
#define Gap218mm			(21800 + PhysGapCorrection)
#define Gap300mm			(30000 + PhysGapCorrection)
#define Gap400mm			(40000 + PhysGapCorrection)

#define	Gap35mm				3500				/* 35mm */
#define Gap80mm				(8000 + PhysGapCorrection)	
#define Gap80mm_cheat				(8000 + PhysGapCorrection + 600)
#define Gap76mm				(7600 + PhysGapCorrection)
#define Gap90mm				(9000 + PhysGapCorrection)
#define Gap90mm_cheat2				(9000 + PhysGapCorrection + 800)
#define Gap90mm_cheat				(9000 + PhysGapCorrection - 900)
#define Gap160mm				(16000 + PhysGapCorrection)
#define Gap219mm			(21900 + PhysGapCorrection)
	/* Roller Nip */
#define EraseNip			500					/* 5mm */


/* Physical Layout Timing */
/* For HeartBeat */
#define	MIRROR_MOTOR_RPM	26456.69
#define	LINES_PER_SEC	MIRROR_MOTOR_RPM / 6 / 60 
#define	Modifier_HeartBeatToLines	1000 / LINES_PER_SEC * Modifier_HeartBeatToMlSec
#define	Modifier_HeartBeatToMlSec	1	/* 1000/ 1000 */

#define	MsecTime				1000
#define	BaseTimer				10				/* 10msec Base Timer */
#define	HTimes					100
#define	Modifier_BaseTimer		MsecTime / BaseTimer
#define	Modifer_timer			MsecTime / BaseTimer / HTimes

#define SolenoidOnDelay			500				/* 50msec = 5mm */
#define	SolenoidOnTime			1500 
#define	Modifer_timerPerMotor	Modifer_timer / MOTORSPEED
#define	Modifer_timerPerMotor_L	Modifer_timer / MOTORSPEED_L
#define	MMTimerPerMotorSpeed	Modifier_BaseTimer / MOTORSPEED
#define	MMTimerPerMotorSpeed_L	Modifier_BaseTimer / MOTORSPEED_L
#define	MMTimerPerFeedSpeed		Modifier_BaseTimer / FEEDMOTORSPEED
#define	MMTimerPerFeedSpeed_L		Modifier_BaseTimer / FEEDMOTORSPEED_L
#define	MMTimerPerPaperSpeed	Modifier_BaseTimer / PAPERSPEED
#define	MMTimerPerPaperSpeed_L	Modifier_BaseTimer / PAPERSPEED_L

/* Engine Manager Standart Time */
#define EMTicks_50msec (Uint32)(50L * Modifier_HeartBeat) / BaseTimer
#define EMTicks_200msec (Uint32)(200L * Modifier_HeartBeat) / BaseTimer
#define EMTicks_500msec (Uint32)(500L * Modifier_HeartBeat) / BaseTimer
#define EMTicks_5sec 	(Uint32)(5000L * Modifier_HeartBeat) / BaseTimer
#define	EMTicks_6sec	(Uint32)(6000L * Modifier_HeartBeat) / BaseTimer
#define	EMTicks_1min	(Uint32)(60000L * Modifier_HeartBeat) / BaseTimer
#define EMTicks_10min 	(Uint32)(600000L * Modifier_HeartBeat) / BaseTimer
#define EMTicks_5min 	(Uint32)(300000 * Modifier_HeartBeat) / BaseTimer

/* PageSupervisor Standard Time */
#define	PSTicks_Minimum	(Uint32)(1L)
#define	PSTicks_10ms	(Uint32)(10L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_20ms	(Uint32)(20L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_30ms	(Uint32)(30L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_40ms	(Uint32)(40L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_50ms	(Uint32)(50L	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_100ms	(Uint32)(100L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_125ms	(Uint32)(125L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_180ms	(Uint32)(180L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_200ms	(Uint32)(200L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_300ms	(Uint32)(300L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_500ms	(Uint32)(500L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_600ms	(Uint32)(600L 	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_1s		(Uint32)(1000L	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_2s		(Uint32)(2000L	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_3s		(Uint32)(3000L	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_4s		(Uint32)(4000L	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_5s		(Uint32)(5000L	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_6Hs		(Uint32)(6500L	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_10s		(Uint32)(10000L	* Modifier_HeartBeat) / BaseTimer
#define	PSTicks_15s		(Uint32)(15000L * Modifier_HeartBeat) / BaseTimer
#define	PSTicks_20s		(Uint32)(20000L * Modifier_HeartBeat) / BaseTimer
#define	PSTicks_30s		(Uint32)(30000L * Modifier_HeartBeat) / BaseTimer
#define	PSTicks_1min	(Uint32)(60000L * Modifier_HeartBeat) / BaseTimer
#define	PSTicks_Maximum	(Uint32)(0xffffffff)

	/* Motor Timing */
#define PSTicks_MotorLock		PSTicks_5s
#define PSTicks_Tray2Stepping	PSTicks_200ms
/*#define PSTicks_Tray2Stepping	PSTicks_50ms*/ /* Tested by Kyungbal 08.10 */
	/* Paper Feed Timing */
#define	PSTicks_T1ToPaper		(Uint32)(T1ToPaper * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_T1ToPins		(Uint32)((T1ToPins - T1ToPaper) * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_T1ToPins_L		(Uint32)((T1ToPins - T1ToPaper) * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define	PSTicks_T2ToPaper		(Uint32)(T2ToPaper * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_T2ToPins		(Uint32)((T2ToPins - T2ToPaper) * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_T2ToPins_L		(Uint32)((T2ToPins - T2ToPaper) * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define	PSTicks_T3ToPaper		(Uint32)(T3ToPaper * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_T3ToPins		(Uint32)((T3ToPins - T3ToPaper) * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_T3ToPins_L		(Uint32)((T3ToPins - T3ToPaper) * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define PSTicks_MPFToPins		(Uint32)((MPFToPins - MPFToPaper) * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_MPFToPins_L		(Uint32)((MPFToPins - MPFToPaper) * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define PSTicks_SolenoidOnDelay	(Uint32)(SolenoidOnDelay * Modifier_HeartBeat * Modifer_timerPerMotor)
/*#define PSTicks_SolenoidOnTime	(Uint32)(SolenoidOnTime * Modifier_HeartBeat * Modifer_timerPerMotor)*/
#define PSLimit_TrayToPins_LAMINATE		1000	//10000msec (9920+80 sol on time)
#define PSLimit_TrayToPins		130//250//143	//1510msec (1430+80 sol on time)	//114	//150	/* 1500msec */
#define PSLimit_TrayToPins_QUIET		200//300//143	//1510msec (1430+80 sol on time)	//114	//150	/* 1500msec */
#define PSLimit_Tray2ToPins		180//400//231	/* 2310msec */	//350	/* 3500msec */
#define PSLimit_Tray2ToPins_QUIET		260//500//231	/* 2310msec */	//350	/* 3500msec */f
#define PSLimit_Tray3ToPins		210//600//231	/* 2310msec */	//350	/* 3500msec */
#define PSLimit_Tray3ToPins_QUIET		310//700//231	/* 2310msec */	//350	/* 3500msec */

#define	PSTicks_SolenoidOnTime	60//8//40	//25	//8	//20	/* 250msec */
#define	PSTicks_SolenoidOnTime_Quiet	91//8//40	//25	//8	//20	/* 250msec */
#define	PSTicks_SolenoidOnTime_PINTREEL	47
#define	PSTicks_SolenoidOnTime_PINTREEL_Quiet	60

#define	PSTicks_SolenoidOnTime_MPT	15	/* 250msec */
#define	PSTicks_SolenoidOnTime_TRAY2	(40+40)	//20	/* 250msec */
#define	PSTicks_SolenoidOnTime_TRAY3	40	//20	/* 250msec */
#define PSTicks_ChargeBeforePins			8  // namja

enum {
	SOLENOID_TRAY1 = 1,
	SOLENOID_TRAY2,
	SOLENOID_TRAY3,
	SOLENOID_MPT,
	SOLENOID_DUP,
};

#define PSTicks_PickupToBoostChargeMPF		103
#define PSTicks_PickupToBoostChargeT1		156
#define PSTicks_PickupToBoostChargeT2		224
#define PSTicks_PickupToBoostChargeT3		448
#define PSTicks_PickupToBoostChargeDuplex	91
    /* Paper Input Sensor To Process Timing */
#define PSTicks_PinsToXfr		(Uint32)(PinsToXfr * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_XfrMargine		1
#define PSTicks_PinsActuate		(Uint32)(PinsActuate * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_PinsActuate_L		(Uint32)(PinsActuate * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define PSTicks_DividerActuate		(Uint32)(DividerActuate * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_PinsToDev		(Uint32)(PinsToDev * Modifier_HeartBeat * Modifer_timerPerMotor)
		
#if MAC_TYPE_MH
#define	PSTicks_FeelerOn		2/*4.491mm*/
#else
#define	PSTicks_FeelerOn		4/*4.491mm*/
#endif

#define	PSTicks_PexsFeelerOn		4/*4.491mm*/
#define	PSTicks_FeelerOff		2/*2.588mm*/
#define	PSTicks_PexsFeelerOff		2/*2.588mm*/
#define	PSTicks_PinsOffToXfrMargine	8
#define PSTicks_DevMargine		6

	/* Process Layout */
#define PSTicks_ChargeToLD		(Uint32)(ChargeToLD * Modifier_HeartBeat * Modifer_timerPerMotor)		// ChargeToLD = 9.164mm
#define PSTicks_ContinueAPC		(Uint32)(ContinueAPC * Modifier_HeartBeat * Modifer_timerPerMotor)		// ContinueAPC = 5mm
#define PSTicks_APC_EndToDev	(Uint32)(APC_EndToDev * Modifier_HeartBeat * Modifer_timerPerMotor)		// APC_EndToDev = 24.078mm - 9.164mm - 5mm
#define	PSTicks_ChargeToDevOn	(Uint32)(ChargeToDevOn * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! ChargeToDevOn = 25.853mm
#define	PSTicks_ChargeToDevOn_L	(Uint32)(ChargeToDevOn * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define	PSTicks_ChargeToDevOnForPrint	(Uint32)(ChargeToDevOnForPrint * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! ChargeToDevOn = 25.853mm
#define	PSTicks_ChargeToDevOnForPrint_L	(Uint32)(ChargeToDevOnForPrint * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define	PSTicks_DevOnToXfrServo	(Uint32)(DevOnToXfrServo * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! DevOnToXfrServo = 78.027mm
#define	PSTicks_DevOnToXfrServo_L	(Uint32)(DevOnToXfrServo * Modifier_HeartBeat * Modifer_timerPerMotor_L) 	// ABBA NEW!!! DevOnToXfrServo = 78.027mm
#define PSTicks_ChargeToDev		(Uint32)(ChargeToDev * Modifier_HeartBeat * Modifer_timerPerMotor)		// ChargeToDev = 24.07mm
#define PSTicks_LDtoDev			(Uint32)(LDtoDev * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_MinLDToDev		7	/* 8mm */
#define	PSTicks_VideoWait		3	/* 38.4msec */
#define PSTicks_LDEnToDev		(Uint32)(LDEnToDev * Modifier_HeartBeat * Modifer_timerPerMotor)	// ABBA NEW!!! 144msec	//08May09		
#define LDEnToDev				1829	// ABBA NEW!!! 20.525mm	//08May09
#define PSTicks_HSync			3	/* 20msec */
#define PSTicks_DevToXfr		(Uint32)(DevToXfr * Modifier_HeartBeat * Modifer_timerPerMotor)			// DevToXfr = 24.34mm
#define PSTicks_XfrToErase		(Uint32)(XfrToErase * Modifier_HeartBeat * Modifer_timerPerMotor)		// XfrToErase = 7.85mm
#define PSTicks_XfrToCharge		(Uint32)(XfrToCharge * Modifier_HeartBeat * Modifer_timerPerMotor)		// XfrToCharge = 45.77mm
#define PSTicks_NipAndRising	10
#define PSTicks_XfrOnToExitDet	(Uint32)((XfrToExitJam - PinsOnToXfr) * Modifier_HeartBeat * Modifer_timerPerMotor)		
#define PSTicks_XfrOnToExitDet_L	(Uint32)((XfrToExitJam - PinsOnToXfr) * Modifier_HeartBeat * Modifer_timerPerMotor_L)		
#define PSTicks_XfrOffToExitRel	(Uint32)((XfrToExitJam - PinsOffToXfr) * Modifier_HeartBeat * Modifer_timerPerMotor)		
#define PSTicks_XfrOffToExitRel_L	(Uint32)((XfrToExitJam - PinsOffToXfr) * Modifier_HeartBeat * Modifer_timerPerMotor_L)		
#define XfrToExitJam			23500	//22500
#define PSTicks_XfrToFuser		(Uint32)(XfrToFuser * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_XfrToPexs		(Uint32)(XfrToPexs * Modifier_HeartBeat * Modifer_timerPerMotor)
#define	PSTicks_EraseToPexs		(Uint32)(EraseToPexs * Modifier_HeartBeat * Modifer_timerPerMotor)
		
#define	PSTicks_PinsToDevPrint	(Uint32)(PinsToDevPrint * Modifier_HeartBeat * Modifer_timerPerMotor)	// ABBA NEW!!! //08May09
#define	PinsToDevPrint			2052	// ABBA NEW!!! 20.525mm	//08May09	// 140msec
#define	PSTicks_DevOnToXfrPrint	(Uint32)(DevOnToXfrPrint * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! //08May09
//#define DevOnToXfrPrint			2716	// ABBA NEW!!! 27.066mm	//08May09	// 190msec
#define DevOnToXfrPrint			2280	// ABBA NEW!!! 22.8mm	//06Aug09	// 160msec
//#define	PSTicks_DevOnToXfrPrint	1	
		
#define PSTicks_PinsOnToXfr			(Uint32)(PinsOnToXfr * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_PinsOnToXfr_L			(Uint32)(PinsOnToXfr_L * Modifier_HeartBeat * Modifer_timerPerMotor_L)

#define	PSTicks_Dev_NoPrint_PreHeating			139
#define	PSTicks_Txf_Clean_PreHeating			(200 - PSTicks_Dev_NoPrint_PreHeating)
#define	PSTicks_CleanTxf_Laminating				170

#if IPG_ENABLE
#define PSTicks_PinsOffToVideoOff (Uint32)(PinsOffToVideoOff * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_PinsOffToVideoOff_L (Uint32)(PinsOffToVideoOff * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define PSTicks_VideoOffToVideoOn (Uint32)(VideoOffToVideoOn * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_VideoOffToVideoOn_L (Uint32)(VideoOffToVideoOn * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define PinsOffToVideoOff 1000
#define VideoOffToVideoOn 1500
#define	PSTicks_PinsOffToXfr		(Uint32)((PinsOffToXfr/*-PinsOffToVideoOff-VideoOffToVideoOn*/) * Modifier_HeartBeat * Modifer_timerPerMotor)
#define	PSTicks_PinsOffToXfr_L		(Uint32)((PinsOffToXfr/*-PinsOffToVideoOff-VideoOffToVideoOn*/) * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#else
#define	PSTicks_PinsOffToXfr		(Uint32)(PinsOffToXfr * Modifier_HeartBeat * Modifer_timerPerMotor)
#endif

#if MAC_TYPE_L
#define PinsOnToXfr				5000
#define PinsOffToXfr			5440	// ABBA NEW!!! 50mm+4.4mm	
#elif MAC_TYPE_M
#define PinsOnToXfr				4200	//4800	//4600
#define PinsOnToXfr_L			4900	//4600		
#define PinsOffToXfr			5340	//5040	// ABBA NEW!!! 46mm+4.4mm
#else
#if ABBA2
#define PinsOnToXfr				4800	//4600		
#define PinsOnToXfr_L			4200	//4600		
#define PinsOffToXfr			5540	//5040	// ABBA NEW!!! 46mm+4.4mm
#else
#define PinsOnToXfr				4200	//4600		
#define PinsOnToXfr_L			4900	//4600		
#define PinsOffToXfr			5370	//5040	// ABBA NEW!!! 46mm+4.4mm
#endif
#endif

#define	PSTicks_DevOnToServoPOR	(Uint32)(DevOnToServoPOR * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! 303msec	//08May09
#define	PSTicks_DevOnToServoPOR_L	(Uint32)(DevOnToServoPOR * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define DevOnToServoPOR			13118	// ABBA NEW!!! (24.871 + 53.156*2)mm	//08May09
#define	PSTicks_DevOnToXfrCleaning	(Uint32)(DevOnToXfrCleaning * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! 990msec	//08May09
#define	PSTicks_DevOnToXfrCleaning_L	(Uint32)(DevOnToXfrCleaning * Modifier_HeartBeat * Modifer_timerPerMotor_L) 
#define DevOnToXfrCleaning		(15864-DeveloperNoPrintingLength)//14364	// ABBA NEW!!! ((50.13+2.2+53.156*2)-15)mm
#define	PSTicks_XfrToChargeOff	(Uint32)(XfrToChargeOff * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! 351msec	//08May09
#define XfrToChargeOff			5000	// ABBA NEW!!! 50mm	//08May09
#define	PSTicks_ChargeToMotorOff	(Uint32)(ChargeToMotorOff * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! 1074msec	//08May09
// #define ChargeToMotorOff		15299	// ABBA NEW!!! (92.991+60)mm	//08May09
#define ChargeToMotorOff		23299	// ABBA NEW!!! (92.991+60+80)mm	//22Dec10 : �ּһ����� ���� �̹��� ����
#define	PSTicks_XfrToMotorOff	(Uint32)(XfrToMotorOff * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! 1074msec	//08May09	
#define	PSTicks_XfrToMotorOff_L	(Uint32)(XfrToMotorOff * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#if ABBA2
#define XfrToMotorOff			23599	// ABBA NEW!!! (92.991+60+80)mm	//22Dec10 : �ּһ����� ���� �̹��� ����		
#else
#define XfrToMotorOff			23299	// ABBA NEW!!! (92.991+60+80)mm	//22Dec10 : �ּһ����� ���� �̹��� ����		
#endif
	
#define	PSTicks_Exit1RollToRegRol	(Uint32)(Exit1RollToRegRol * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! //13May09
#define Exit1RollToRegRol		15000	// ABBA NEW!!! temp 320mm	//13May09	
#define	PSTicks_DupHoldToIns	(Uint32)(DupHoldToIns * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! //18May09
#define DupHoldToIns			30000	// ABBA NEW!!! //18May09		
		
#define	PSTicks_FullStackOnToOff	(Uint32)(FullStackOnToOff * Modifier_HeartBeat * Modifer_timerPerMotor) 	// ABBA NEW!!! //18May09
#define	PSTicks_FullStackOnToOff_L	(Uint32)(FullStackOnToOff * Modifier_HeartBeat * Modifer_timerPerMotor_L) 	// ABBA NEW!!! //18May09
#define FullStackOnToOff			35000		

		
#define PSTicks_WaitCooling		(Uint32)(WaitCooling * Modifier_HeartBeat * Modifer_timerPerMotor)
#define WaitCooling				(32650 + 29700 + 3000)		// (Pin~Exit roll) + A4 Length + Margin
/* Paper Exit Sensor Timing */
#define PSTicks_PexsToDivider		(Uint32)(PexsToDivider * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_PexsToExitSolOn		(Uint32)((PexsToExitSolOn) * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_PexsToExitSolOn_L		(Uint32)((PexsToExitSolOn) * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define PexsToExitSolOn			4042//4392

#define	PSTicks_Exit1RollToExit2Roll	(Uint32)(Exit1RollToExit2Roll * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_PexsToHopper	(Uint32)(PexsToHopper * Modifier_HeartBeat * Modifer_timerPerMotor)
	/* Duplex */
	/* 1 Times */
#define	PSTicks_Exit1RollToDis_1	(Uint32)(Exit1RollToDis * Modifier_HeartBeat * Modifer_timerPerMotor)
#define	PSTicks_DisToHoldPoint_1	(Uint32)(DisToHoldPoint * Modifier_HeartBeat * Modifer_timerPerMotor)
#define	PSTicks_HoldPointToPins_1	(Uint32)(HoldPointToPins * Modifier_HeartBeat * Modifer_timerPerMotor)
	/* 2 Times */
#define	PSTicks_Exit1RollToDis_2	(Uint32)(Exit1RollToDis * Modifier_HeartBeat * Modifer_timerPerMotor / 2 /** 1/2*/)
#define	PSTicks_DisToHoldPoint_2	(Uint32)(DisToHoldPoint * Modifier_HeartBeat * Modifer_timerPerMotor / 2 /** 1/2*/)
#define	PSTicks_HoldPointToPins_2	(Uint32)(HoldPointToPins * Modifier_HeartBeat * Modifer_timerPerMotor / 2)

	/* EP Cleaning Timing */
#define PSTicks_WaitCleaning	(Uint32)(WaitCleaning * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_WaitCleaning_L	(Uint32)(WaitCleaning * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define PSTicks_CleaningTime	(Uint32)(CleaningTime * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_Charge2Roll	(Uint32)(Charge2Roll * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_XfrCleaningTime	(Uint32)(Xfr1Roll * Modifier_HeartBeat * Modifer_timerPerMotor)		// Xfr1Roll = 5316	53.16mm
#define PSTicks_XfrCleaningTime_L	(Uint32)(Xfr1Roll * Modifier_HeartBeat * Modifer_timerPerMotor_L)		// Xfr1Roll = 5316	53.16mm
#define	PSTicks_XfrCleaningPrint	26		//ABBA NEW!!! 26(PSTicks_XfrCleaningPrint) + 11(PSTicks_DeveloperNoPrintingLength)
#define PSTicks_TxfIPTime	(Uint32)(TxfIPTime * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_TxfIPTime_L	(Uint32)(TxfIPTime * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#if ABBA2
#define TxfIPTime		6100
#else
#define TxfIPTime		6000
#endif
/* Added for Main Motor Lock up Time - Sungho Kim 21July2003 */
#define	PSTicks_XMotorLockUp	10

#if ABBA2
#define	PSTicks_StopMotorTime	40		//ABBA NEW!!!
#else
#define	PSTicks_StopMotorTime	50		//ABBA NEW!!!
#endif
#define	PSTicks_StopDuplexMargine	4
	
#define	PSTicks_DupSolOnToOff	(Uint32)((DupSolOnToOff) * Modifier_HeartBeat * Modifer_timerPerMotor)	//180	//150
#define	PSTicks_DupSolOnToOff_L	(Uint32)((DupSolOnToOff) * Modifier_HeartBeat * Modifer_timerPerMotor_L)	//180	//150
#define	DupSolOnToOff			34590
	
#define PSTicks_FuserLabelJamFeedDist  (Uint32)(FuserLabelJamFeedDist * Modifier_HeartBeat * Modifer_timerPerMotor)

#define PSTicks_5mm			(Uint32)(Length_5mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_7mm			(Uint32)(Length_7mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_10mm			(Uint32)(Length_10mm * Modifier_HeartBeat * Modifer_timerPerMotor)

#define Default_PSTicks_Gap22mm			(Uint32)(Gap22mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define Default_PSTicks_Gap22mm_L			(Uint32)(Gap22mm * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define Default_PSTicks_Gap25mm			(Uint32)(Gap25mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define Default_PSTicks_Gap28mm			(Uint32)(Gap28mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define Default_PSTicks_Gap32mm			(Uint32)(Gap32mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define Default_PSTicks_Gap53mm			(Uint32)(Gap53mm * Modifier_HeartBeat * Modifer_timerPerMotor)		// 14Aug09 : CHG 1 (new)
#define Default_PSTicks_Gap68mm			(Uint32)(Gap68mm * Modifier_HeartBeat * Modifer_timerPerMotor)	
#define Default_PSTicks_Gap80mm			(Uint32)(Gap80mm * Modifier_HeartBeat * Modifer_timerPerMotor)
	
#define	PSTicks_Gap35mm			(Uint32)(Gap35mm * Modifier_HeartBeat * Modifer_timerPerMotor)

/* MPF Gap */
#define PSTicks_84mm                    (Uint32)(Length_84mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_25mm                    (Uint32)(Length_25mm * Modifier_HeartBeat * Modifer_timerPerMotor)
/* Tray Gap adder for labels */
#define PSTicks_25mm                    (Uint32)(Length_25mm * Modifier_HeartBeat * Modifer_timerPerMotor)

/* Gaps for variable throughput table */
#define PSTicks_Gap47mm                 (Uint32)(4700 * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_Gap63mm                 (Uint32)(6300 * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_Gap82mm                 (Uint32)(8200 * Modifier_HeartBeat * Modifer_timerPerMotor)

/* Define entries for the selectable throughput tables.  RIP sends down one of four 
   throughput rates to run.  Throughput rate variation achieved with variable gaps */
#define NUM_THROUGHPUTS                 4

/* Nominal Throughput 0 = 22ppm */
#define NominalThroughput_0             22
                                        /* Set MPF to 65mm + 19mm = 84mm for all media */
#define MPF_Gap_0                       (Uint32)(Default_PSTicks_Gap80mm)
#define Tray_LabelGap_0                 (Uint32)(Default_PSTicks_Gap32mm + PSTicks_25mm)
//#define Tray_NormalGap_0                (Uint32)(Default_PSTicks_Gap32mm)			// 14Aug09 : CHG 1 (del)
#if MAC_TYPE_MH
#define Tray_NormalGap_0                (Uint32)(Default_PSTicks_Gap80mm)
#else
#define Tray_NormalGap_0                (Uint32)(Default_PSTicks_Gap68mm)			// 14Aug09 : CHG 1 (new)									
#endif
											
#define InterpageGap_L					(Uint32)(Gap68mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_L_QUIET					(Uint32)(Gap68mm * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define InterpageGap_Export_L			(Uint32)(Gap100mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#if IPG_ENABLE
#if ABBA2
#define InterpageGap_M					(Uint32)(Gap75mm_cheat/*Gap75mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor)
#else
#define InterpageGap_M					(Uint32)(Gap90mm_cheat2/*Gap80mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor)
#endif
#define InterpageGap_M_Plus70			(Uint32)(Gap160mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_M_QUIET			(Uint32)(Gap90mm_cheat/*Gap80mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define InterpageGap_M_Plus70_QUIET		(Uint32)(Gap160mm/*Gap80mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#if ABBA2
#define InterpageGap_H					(Uint32)(Gap80mm_cheat/*Gap80mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor)
#else
#define InterpageGap_H					(Uint32)(Gap90mm_cheat/*Gap80mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor)
#endif
#define InterpageGap_H_Plus70			(Uint32)(Gap160mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_H_QUIET			(Uint32)(Gap90mm_cheat/*Gap80mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define InterpageGap_H_Plus70_QUIET		(Uint32)(Gap160mm/*Gap80mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#else
#define InterpageGap_M					(Uint32)(Gap75mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_H					(Uint32)(Gap218mm/*Gap80mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor)
#endif
#define InterpageGap_M_SAGI				(Uint32)(Gap219mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_M_SAGI_QUIET				(Uint32)(Gap219mm * Modifier_HeartBeat * Modifer_timerPerMotor_L)
//#define InterpageGap_M					(Uint32)(Gap71mm * Modifier_HeartBeat * Modifer_timerPerMotor)
//#define InterpageGap_H					(Uint32)(Gap76mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_Legal_MH			(Uint32)(Gap300mm/*Gap218mm*//*Gap100mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_Legal_MH_QUIET			(Uint32)(Gap300mm/*Gap218mm*//*Gap100mm*/ * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define InterpageGap_Narrow_L			(Uint32)(Gap166mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_Narrow_L_QUIET			(Uint32)(Gap166mm * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define InterpageGap_Narrow_M			(Uint32)(Gap218mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_Narrow_M_QUIET		(Uint32)(Gap218mm * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define InterpageGap_Narrow_H			(Uint32)(Gap218mm * Modifier_HeartBeat * Modifer_timerPerMotor)								
#define InterpageGap_Narrow_H_QUIET			(Uint32)(Gap218mm * Modifier_HeartBeat * Modifer_timerPerMotor_L)								
#define InterpageGap_M_Duplex			(Uint32)(Gap218mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_M_Duplex_QUIET			(Uint32)(Gap218mm * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define InterpageGap_H_Duplex			(Uint32)(Gap218mm * Modifier_HeartBeat * Modifer_timerPerMotor)
#define InterpageGap_H_Duplex_QUIET			(Uint32)(Gap218mm * Modifier_HeartBeat * Modifer_timerPerMotor_L)
/* Nominal Throughput 1 = 21ppm */
#define NominalThroughput_1             21
#define MPF_Gap_1                       (Uint32)(PSTicks_84mm)
#define Tray_LabelGap_1                 (Uint32)(PSTicks_Gap47mm)
#define Tray_NormalGap_1                (Uint32)(PSTicks_Gap47mm)

/* Nominal Throughput 2 = 20ppm */
#define NominalThroughput_2             20
#define MPF_Gap_2                       (Uint32)(PSTicks_84mm)
#define Tray_LabelGap_2                 (Uint32)(PSTicks_Gap63mm)
#define Tray_NormalGap_2                (Uint32)(PSTicks_Gap63mm)

/* Nominal Throughput 3 = 19ppm */
#define NominalThroughput_3             19
#define MPF_Gap_3                       (Uint32)(PSTicks_84mm)
#define Tray_LabelGap_3                 (Uint32)(PSTicks_Gap82mm)
#define Tray_NormalGap_3                (Uint32)(PSTicks_Gap82mm)

	/* Roller Nip */
#define PSTicks_EraseNip		(Uint32)(EraseNip * Modifier_HeartBeat * Modifer_timerPerMotor)
	/* Chattering Time */
#define	CHATTERING_TIME			6
#define	CHATTERING_TIME_ADJ		0
#define CHATTERING_TIME_PINS	1
//08May09 yjlee
//#define PSTicks_DeveloperNoPrintingLength		13
// #define PSTicks_DeveloperNoPrintingLength		11	// ABBA NEW!!! 105mm
#define PSTicks_DeveloperNoPrintingLength		(Uint32)(DeveloperNoPrintingLength * Modifier_HeartBeat * Modifer_timerPerMotor) 
#define PSTicks_DeveloperNoPrintingLength_L		(Uint32)(DeveloperNoPrintingLength * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define DeveloperNoPrintingLength			1330//1500	// ABBA NEW!!! //18May09		
#define PSTicks_DeveloperNoPrintingLengthForPrint		(Uint32)(DeveloperNoPrintingLengthForPrint * Modifier_HeartBeat * Modifer_timerPerMotor) 
#define PSTicks_DeveloperNoPrintingLengthForPrint_L		(Uint32)(DeveloperNoPrintingLengthForPrint * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define DeveloperNoPrintingLengthForPrint			1500	// ABBA NEW!!! //18May09		

	/* Servo Timing */
#define PSTicks_ServoStep200ms			PSTicks_200ms
#define PSTicks_ServoStep50ms			PSTicks_50ms
#define PSTicks_ServoStep20ms			PSTicks_20ms

// #define	PSTicks_PaperLengthMinimum	(Uint32)(16002L * Modifier_HeartBeat * Modifer_timerPerMotor) /* 6.3" = 160.02mm*/
#define	PSTicks_PaperLengthMinimum	(Uint32)(14000L * Modifier_HeartBeat * Modifer_timerPerMotor) /* 6.3" = 160.02mm*/

#define	Slip_10			11 / 10			/* 10% */
#define	Slip_20			12 / 10			/* 20% */
#define	Slip_30				13 / 10			/* 30% */

#define	PSEmpty_MPFToPins	(Uint32)(MPFToPins * Modifier_HeartBeat * Slip_30 * Modifer_timerPerMotor)
#define	PSEmpty_T1ToPins	(Uint32)(T1ToPins * Modifier_HeartBeat * Slip_30 * Modifer_timerPerMotor)
#define	PSEmpty_T2ToPins	(Uint32)(T2ToPins * Modifier_HeartBeat * Slip_30 * Modifer_timerPerMotor)
#define	PSEmpty_T3ToPins	(Uint32)(T3ToPins * Modifier_HeartBeat * Slip_30 * Modifer_timerPerMotor)

#define	PSEmpty_MPFToPins_L	(Uint32)(MPFToPins * Modifier_HeartBeat * Slip_30 * Modifer_timerPerMotor_L)
#define	PSEmpty_T1ToPins_L	(Uint32)(T1ToPins * Modifier_HeartBeat * Slip_30 * Modifer_timerPerMotor_L)
#define	PSEmpty_T2ToPins_L	(Uint32)(T2ToPins * Modifier_HeartBeat * Slip_30 * Modifer_timerPerMotor_L)
#define	PSEmpty_T3ToPins_L	(Uint32)(T3ToPins * Modifier_HeartBeat * Slip_30 * Modifer_timerPerMotor_L)
/****** TEMPORARY WORKAROUND FOR LEGAL SIZE PAPER PROBLEM WITH LAWRIN ******/
#if (BIOS_ASIC_SET == BIOS_ASIC_SET_LAWRIN)
#define EXTRA_LEGAL_TICKS  50
#else
#define EXTRA_LEGAL_TICKS  0
#endif
/****************************************************************************/
#define PSTicks_Pad1Inch                (Uint32)(Pad1Inch * Modifier_HeartBeat * Modifer_timerPerMotor)
#define PSTicks_Pad1Inch_L                (Uint32)(Pad1Inch * Modifier_HeartBeat * Modifer_timerPerMotor_L)
#define	PSTicks_Legal	(Uint32)(PL_Legal * Modifier_HeartBeat * MMTimerPerMotorSpeed)
#define	PSTicks_Legal_L	(Uint32)(PL_Legal * Modifier_HeartBeat * MMTimerPerMotorSpeed_L)
#define	PSLimit_Legal	        (Uint32)(PSTicks_Legal + PSTicks_Pad1Inch + PSTicks_PinsActuate)
#define	PSLimit_Legal_L	        (Uint32)(PSTicks_Legal_L + PSTicks_Pad1Inch_L + PSTicks_PinsActuate_L)
#define	PSLimit_PinsToPexs	(Uint32)((PinsToXfr + XfrToPexs + Pad1Inch) * Modifier_HeartBeat * Modifer_timerPerMotor)


#define	Slip_100				2				/* 100% */
#define	PSLimit_MPFToPins	(Uint32)(MPFToPins * Modifier_HeartBeat * Slip_100 * Modifer_timerPerMotor)
#define	PSLimit_T1ToPins	(Uint32)(T1ToPins * Modifier_HeartBeat * Slip_100 * Modifer_timerPerMotor)
#define	PSLimit_T2ToPins	(Uint32)(T2ToPins * Modifier_HeartBeat * Slip_100 * Modifer_timerPerMotor)

#define	PSLimit100_Legal	(Uint32)(PL_Legal * Modifier_HeartBeat * Slip_100 * MMTimerPerMotorSpeed)

#define	PSTicks_3TimesLegal	(Uint32)(3 * (PL_Legal + 32) * Modifier_HeartBeat * MMTimerPerMotorSpeed)
/* For Duplex */
	/* 1 * Speed */
#define	PSTicks_DuplexHoldingToPick1	10
#define PSLimit_DuplexHoldPointToPins	(Uint32)(HoldPointToPins * Modifier_HeartBeat * Slip_100 * Modifer_timerPerMotor)
	/* 1.5 * Speed */
#define	PSTicks_DuplexToPick1			10
#define PSLimit_DuplexToPins			10

#define	IntervalNarrowMedia	(PSTicks_15s / 2)
#define	Interval_22ppm	(PSTicks_1min / 22)
#define	IntervalShorterThanLetter	(PSTicks_1min / 17)
#define	Interval_12ppm	(PSTicks_1min / 12)


#define PSTicks_JogMode_RunTime_L	15
#define PSTicks_JogMode_RunTime_H	5//11
#define PSTicks_StopJogMode 28

#define JOGMODE_PAGE_MAX 15
#define JOGMODE_PRINT_TIME_MINUTE 60
#define JOGMODE_PRINT_TIME (JOGMODE_PRINT_TIME_MINUTE * 60 * 100)

#define FANCTRL_PAGE_MAX 301
#define FANCTRL_PRINT_TIME_MINUTE 30
#define FANCTRL_PRINT_TIME (FANCTRL_PRINT_TIME_MINUTE * 60 * 100)

#define UNKNOWN_PAPER_LENGTH \
	PL_Legal / 10 * Modifier_HeartBeat * 11 * MMTimerPerFeedSpeed
#define UNKNOWN_PAPER_LENGTH_L \
	PL_Legal / 10 * Modifier_HeartBeat * 11 * MMTimerPerFeedSpeed_L
#define LETTER_LENGTH \
	(PL_A4 + PL_Letter) * Modifier_HeartBeat * MMTimerPerFeedSpeed / 2
/*
#ifdef	8PPM
 8PPM */
/*#define	NarrowMedeaInterval	(PSTicks_15s / 2)

#elif	6PPM
 6PPM */
/*#define	NarrowMedeaInterval	PSTicks_10s

#else
 4PPM
#define	NarrowMedeaInterval	PSTicks_15s
#endif */

#ifdef SMART_IC_TONER	/* [KSR032101] for SMC */
extern void 	SecurityIC_Check(void);
extern Uint8	fSmartIC_Authentication;
extern void	SmartICDevDrvInit(void);
#endif /* [KSR032101] for SMC */
Uint8 Get_TrayInfo(Uint8 Tray_num);

#endif /* __EM_H */
/* $Id: em.h,v 1.7 2003/08/05 17:05:56 bfbrown (null) $ */
