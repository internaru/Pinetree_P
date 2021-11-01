/* 
 * ============================================================================
 * Copyright (c) 2009-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file adfsensor_pinetree.c
 *
 * \brief Shiloh ADF sensor functions
 *
 *  Implementations for the Shiloh ADF sensors. 
 *    Paper In Path (PIP)
 *    Paper Present (PP)
 *    Cover Is Open (CIO)
 *    Interval Present (INP)
 *
 **/

#include "stdint.h"
#include "stdbool.h"
//#include "ATypes.h"
#include "lassert.h"
#include "scos.h"                               //MRVL-Harris-20120215 for ADF
//#include "delay_api.h"
#include "scancore.h"
#include "scantypes.h"
#include "scanmech.h"
#include "scandbg.h"
#include "scantask.h"                           //MRVL-Harris-20120215 for ADF
#include "adfsensor.h"
#include "adfsensor_pinetree.h"
#include "scanmech_pinetree.h"
#include "gpio_api.h"
#include "gpio_config_api.h"
//#include "error_types.h"
#include "scanplat_sb.h"
#include "picdma_descrip.h"

#ifdef __KERNEL__
#include <asm/gpio.h>
#include <linux/timer.h>
#endif


#define adfsen_func_enter()              dbg2("ADFSEN@%s: enter\n", __FUNCTION__)
#define adfsen_func_leave()              dbg2("ADFSEN@%s: leave\n", __FUNCTION__)
#define adfsen_func_infor(msg, args...)  dbg1("ADFSEN@%s: " msg, __FUNCTION__, ##args)
#define adfsen_func_debug(msg, args...)  dbg2("ADFSEN@%s: " msg, __FUNCTION__, ##args)
#define adfsen_func_error(msg, args...)  dbg1("ADFSEN@%s.%d: " msg, __FUNCTION__, __LINE__, ##args)
#define adfsen_func_helpu(msg, args...)  dbg1(msg, ##args)

/* add the define mod.kks 2014.06.12*/
#define	OFF 0
#define  ON 1

#ifdef __KERNEL__
static struct pt_adf pinetree_adf;
static struct pt_cis pinetree_cis; //by JJW (for new CIS)

#else
static gpio_handle_t      *adf_pip_sensor_gpio;
static gpio_handle_t      *adf_pp_sensor_gpio;
static gpio_handle_t      *adf_cio_sensor_gpio;
static gpio_handle_t      *adf_int_sensor_gpio;
static gpio_handle_t      *cis_select_gpio; //by JJW (for new CIS)
#endif
/*
 * Internal globals
 */
static bool            pip_callback_enable;

static bool            pp_callback_enable;
static bool            pp_debounced_state;
static uint32_t        pp_debounce_count;

//cis select
int	   cis_select_state; //by JJW (for new CIS)

//MRVL-Harris-20120215 for ADF {
static bool            cio_callback_enable;
bool            cio_debounced_state; // To use the debounce state with smech mod.kks 2014.06.13
static uint32_t        cio_debounce_count; 
bool 			ChangeTopCoverState=OFF;  // Check the Top Cover change state. mod.kks 2014.06.13
//MRVL-Harris-20120215 for ADF }

/*SP input Check Mode mod.kks 2014.06.12*/
extern	bool 	SP_IN_check_flg;
extern	bool 	SP_OUT_check_flg;
/*end SP Input Check*/

#if 0 //MRVL-Harris 20121030
#ifndef __KERNEL__
static delay_msec_timer_t *adf_timer;
#else
static struct timer_list adf_timer;
#define MILLISEC(n) ((n * HZ)/1000)
#endif
#endif //MRVL-Harris 20121030

/* Number of samples required for a valid pip state transition.  The sensor should be
 * sampled at 1ms intervals.
 */
#define PP_DEBOUNCE_COUNT           20 //50  Because of 1msec -> 10msec mod.kks
#define CIO_DEBOUNCE_COUNT          20 //50  Because of 1msec -> 10msec  mod.kks        //MRVL-Harris-20120215 for ADF
#define PIP_STEPS_DEBOUNCE_COUNT    1600 //40mm  

void adf_sensor_pip_callback_disable(void)
{
    /* Stop callback to core scan code */
    pip_callback_enable = false;
}

void adf_sensor_pip_callback_enable(void)
{
    /* Enable callback to core scan code */
    pip_callback_enable = true;
}

bool pip_isr(void)
{
    static struct timespec prev_isr_time = {0, 0};
    struct timespec cur_isr_time;
    bool   bouncing = true;

    getrawmonotonic(&cur_isr_time);
    
    if (cur_isr_time.tv_sec - prev_isr_time.tv_sec >= 2)
    {
        bouncing = false;
    }
    else
    {
        long   ms_diff;
        ms_diff  = (cur_isr_time.tv_sec - prev_isr_time.tv_sec) * 1000;
        ms_diff += (cur_isr_time.tv_nsec  / 1000000);
        ms_diff -= (prev_isr_time.tv_nsec / 1000000);

        if (ms_diff > 30)
        {
            bouncing = false;
        }
    }

    if (pip_callback_enable && (!bouncing))
    {
        struct pic_cwdma_regs* regs = pic_cwdma_get_regs(0);
        if(SP_OUT_check_flg ==OFF ) // Only use in Normal case and SP Input Sensor Check mode. mod.kks 2014.06.12
		{
			if(SP_IN_check_flg == OFF) //NOT SP Mode mod.kks 2014.06.12
				smech_adf_paper_event_callback(SCAN_ADF_PAPER_TOF, adf_sensor_paper_in_path() ? 1 : 0, regs->xfer_length);
			else//SP Input Check Mode mod.kks 2014.06.12
				smech_adf_paper_event_callback(SCAN_SP_PAPER_1STSCAN, adf_sensor_paper_in_path() ? 1 : 0, 0);
		}
    }

    prev_isr_time = cur_isr_time;
    return true;
}

/**
 * \brief Initialize PP debounce logic 
 *
 */
void adf_sensor_pp_debounce_init( void )
{
    /* Sync up with the hardware state and reset the debounce counter */
    pp_debounced_state = adf_sensor_paper_present();
    pp_debounce_count  = 0;
}


/**
 * \brief Debounce the PP sensor 
 *
 * This routine should be called at 1ms intervals to sample the state of the PP
 * sensor.  If the debounce logic detects a state change it will call the mech's
 * paper event callback (if enabled). 
 *
 * \warning
 * This routine can be called from interrupt context!
 *
 */
void adf_sensor_pp_debounce_sample( void )
{
    /* 
     * BIG FAT NOTE! 
     *
     * This can be called in interrupt context!
     */

    bool current_pp_state;

    /* Read the current state of our PP sensor */
    current_pp_state = adf_sensor_paper_present();

    
    /* If the debounced state doesn't match the current sample, we need to get busy
     * with some firmware debounce action.
     */
    if (current_pp_state != pp_debounced_state)
    {
        /* Looks like we may have a transition, increment our debounce counter */
        pp_debounce_count++;

        /* For a valid transition, we need to see PP_DEBOUNCE_COUNT samples in a row */
        if (pp_debounce_count >= PP_DEBOUNCE_COUNT)
        {
            /* We have a valid edge: set the state and reset the debounce counter */
            pp_debounced_state = current_pp_state;
            pp_debounce_count  = 0;
            
            /* Should we tell someone about this event? */
            if (pp_callback_enable)
            {
                //adfsen_func_debug("pp=%d\n", current_pp_state);
                if(SP_OUT_check_flg ==OFF)  // Only use in Normal case and SP Input Sensor Check mode. mod.kks 2014.06.12
				{	
					if(SP_IN_check_flg == OFF) //NOT SP Mode mod.kks 2014.06.12
						smech_adf_paper_event_callback(SCAN_ADF_PAPER_PRESENT, pp_debounced_state, 0);
					else //SP Input Check Mode mod.kks 2014.06.12
						smech_adf_paper_event_callback(SCAN_SP_PAPER_PRESENT, pp_debounced_state, 0);
				}
            }
        }
    }
    else
    {
        /* The states match for now (but could be bouncing), reset the debounce counter */
        pp_debounce_count = 0;    
    }
}


//MRVL-Harris-20120215 for ADF {
/**
 * \brief Initialize CIO debounce logic 
 *
 */
void adf_sensor_cio_debounce_init(void)
{
    /* Sync up with the hardware state and reset the debounce counter */
    cio_debounced_state = adf_sensor_cover_is_open();
    cio_debounce_count  = 0;
}


/**
 * \brief Debounce the CIO sensor 
 *
 */
void adf_sensor_cio_debounce_sample(void)
{
    /* 
     * BIG FAT NOTE! 
     *
     * This can be called in interrupt context!
     */

    bool current_cio_state;
    scan_msg_t msg;
    scan_err_t scerr;
    
    /* Read the current state of our CIO sensor */
    current_cio_state = adf_sensor_cover_is_open();
    
    
    /* If the debounced state doesn't match the current sample, we need to get busy
     * with some firmware debounce action.
     */
    if (current_cio_state != cio_debounced_state)
    {
        /* Looks like we may have a transition, increment our debounce counter */
        cio_debounce_count++;

        /* For a valid transition, we need to see CIO_DEBOUNCE_COUNT samples in a row */
        if (cio_debounce_count >= CIO_DEBOUNCE_COUNT)
        {
            /* We have a valid edge: set the state and reset the debounce counter */
            cio_debounced_state = current_cio_state;
            cio_debounce_count  = 0;
            ChangeTopCoverState = ON;

            /* Cover Open Jam (Scanning) */
            //All case (OFF -> ON or ON-> OFF) sensor state.  If the sensor state chage, awalys enter below code in Original Marvell Code.[mod.kks memo]
			if(cio_callback_enable){ 
				dbg1( "[jam] Cover Open Jam !! (scanning)\n");
				adfsen_func_debug("cio=%d\n", current_cio_state);
				cio_callback_enable = false;
				msg.msgType = SMSG_SCAN_MECH_FAULT;
				msg.param1  = SMECH_ADF_ERROR_CIO;
				msg.param2  = SCAN_MECH_ADF;
				msg.param3  = 0;
				scerr = scantask_msg_send_nowait(&msg);
				XASSERT(scerr == 0, scerr);
			}
            /* Cover Open Info (Scanning, Ready) */
#if 1	// add.lsh.to display cover status in ready state
			if(SP_OUT_check_flg == OFF)  // Only use in Normal case and SP Input Sensor Check mode. mod.kks 2014.06.12
			{	
				if(SP_IN_check_flg == OFF) //NOT SP Mode mod.kks 2014.06.12
					smech_adf_paper_event_callback(SCAN_ADF_TOP_COVER_OPEN, current_cio_state, 0);
				else //SP Input Check Mode mod.kks 2014.06.12
					smech_adf_paper_event_callback(SCAN_SP_PAPER_TOPCOVER, current_cio_state, 0);
			}
#endif
        }
    }
    else
    {
        /* The states match for now (but could be bouncing), reset the debounce counter */
        cio_debounce_count = 0;    
    }
}


void adf_sensor_cio_callback_disable(void)
{
    /* Stop callback to core scan code */
    cio_callback_enable = false;
}


void adf_sensor_cio_callback_enable(void)
{
    /* Enable callback to core scan code */
    cio_callback_enable = true;
    adf_sensor_cio_debounce_init(); //MRVL-Harris-20120302 for ADF
}
//MRVL-Harris-20120215 for ADF }


#if 0 //MRVL-Harris 20121030
#ifdef __KERNEL__
static void adf_timer_callback(unsigned long cb_data)
#else
static void adf_timer_callback(void *cb_data)
#endif
{
    /* Treat this function like IRQ context! */

    /* Sample the PIP sensor */
    //adf_sensor_pip_debounce_sample(); //MRVL-Harris-20111223

    /* Sample the PP sensor */
    //adf_sensor_pp_debounce_sample();

    //MRVL-Harris-20120215 for ADF {
    /* Sample the CIO sensor */
    //adf_sensor_cio_debounce_sample(); //currently, move to stepper interrupt 
    //MRVL-Harris-20120215 for ADF }   

#ifdef __KERNEL__
    adf_timer.expires = jiffies + MILLISEC(1);
    add_timer(&adf_timer);
#endif	
}
#endif //MRVL-Harris 20121030

/**
 * \brief Initialize the ADF sensors
 *
 *  See documentation in adfsensor.h
 */
void adf_sensor_onetime_init( void )
{
#ifndef __KERNEL__
    error_type_t err;

#if GPIO_ISR
    err = gpio_open(&adf_pip_sensor_gpio, GPIO_BANK_F, 23,
#else    
    err = gpio_open(&adf_pip_sensor_gpio, GPIO_BANK_F, 1,  //Mod.kks
#endif    
                    GPIO_DIRECTION_INPUT, 
                    GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0);
    XASSERT(err==OK, err);

    err = gpio_open(&adf_pp_sensor_gpio, GPIO_BANK_F, 5,
                    GPIO_DIRECTION_INPUT, 
                    GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0);
    XASSERT(err==OK, err);

    err = gpio_open(&adf_cio_sensor_gpio, GPIO_BANK_F, 8,
                    GPIO_DIRECTION_INPUT, 
                    GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0);
    XASSERT(err==OK, err);    

    err = gpio_open(&adf_int_sensor_gpio, GPIO_BANK_F, 6,
                    GPIO_DIRECTION_INPUT, 
                    GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0);
    XASSERT(err==OK, err);
    
   // by JJW (for new CIS) 
   err = gpio_open(&cis_select_gpio, GPIO_BANK_G, 19,
#if TEST_PORT_G19
                    GPIO_DIRECTION_OUTPUT, 
#else
                    GPIO_DIRECTION_INPUT,
#endif

                    GPIO_LOGIC_SETTING_ACTIVE_HIGH, 0);
    XASSERT(err==OK, err);
       
#else
    int err;

    /* davep 24-Sep-2013 ; move GPIO function calls into a GPL module */
    memset( &pinetree_adf, 0, sizeof(struct pt_adf) );
    err = adf_sensor_kernel_onetime_init(&pinetree_adf, pip_isr);
    XASSERT(err==0,err);
    
    // by JJW (for new CIS) 
    memset( &pinetree_cis, 0, sizeof(struct pt_cis) );
    err = pinetree_cis_select_kernel_init(&pinetree_cis);
    XASSERT(err==0,err);
#endif

    /* Our PIP callback is disabled by default */
    pip_callback_enable = true;

    /* Our PP callback is enabled by default */
    pp_callback_enable = true;
    adf_sensor_pp_debounce_init();

    //MRVL-Harris-20120215 for ADF {
    /* Our CIO callback is disabled by default */
    cio_callback_enable = false;
    adf_sensor_cio_debounce_init();
    //MRVL-Harris-20120215 for ADF }
    
#if TEST_PORT_G19    
    cis_select_state = 0; //by JJW (for new CIS)
#else
    cis_select_state = cis_select(); //by JJW (for new CIS)
#endif

    dbg1("[JJW] cis select state : %d\n",cis_select_state);

    /* davep 04-Feb-2011 ; send out initial state */
    smech_adf_paper_event_callback(SCAN_ADF_PAPER_PRESENT, pp_debounced_state, 0);

#if 0 //MRVL-Harris 20121030
    /* Start the timer to poll the adf sensors.
     * REVISIT:  How will leaving this timer running all the time work with
     *           powersave?  Badly would be my guess.
     */
#ifndef __KERNEL__
    adf_timer = delay_msec_timer_non_blocking(1,                          /* 1ms timer interval */
                                              adf_timer_callback,         /* Callback function  */
                                              NULL,                       /* Callback data      */
                                              true);                      /* Repeating          */

    XASSERT(adf_timer != NULL, (uint32_t)adf_timer);
#else
    init_timer(&adf_timer);
    adf_timer.expires = jiffies + MILLISEC(1);
    adf_timer.data = 0;
    adf_timer.function = &adf_timer_callback;
    add_timer(&adf_timer);
#endif
#endif //MRVL-Harris 20121030
}


bool adf_sensor_cover_is_open(void)
{
    bool flag = false;
    uint32_t logic_level = 0xFFFF;
#ifndef __KERNEL__
    error_type_t err;
    err = gpio_read(adf_cio_sensor_gpio, &logic_level);
    if (err != OK)
    {
        adfsen_func_error("gpio_read failed\n");
        return (false);
    }
#else
    logic_level = scanplat_kernel_gpio_get_value(pinetree_adf.adf_cio_sensor_gpio);
#endif
    if (logic_level != 0x00)
    {
        flag = false;
    }
    else    
    {
        flag = true;
    }
    return (flag);
}


bool adf_sensor_paper_present(void)
{
    bool flag = false;
    uint32_t logic_level = 0xFFFF;
#ifndef __KERNEL__
    error_type_t err;
    err = gpio_read(adf_pp_sensor_gpio, &logic_level);
    if (err != OK)
    {
        adfsen_func_error("gpio_read failed\n");
        return (false);
    }
#else
    logic_level = scanplat_kernel_gpio_get_value(pinetree_adf.adf_pp_sensor_gpio);
#endif
    if (logic_level != 0x00)
    {
        flag = false;
    }
    else    
    {
        flag = true;
    }
    return (flag);
}


bool adf_sensor_paper_in_path(void)
{
    bool flag = false;
    uint32_t logic_level = 0xFFFF;
#ifndef __KERNEL__
    error_type_t err;
    err = gpio_read(adf_pip_sensor_gpio, &logic_level);
    if (err != OK)
    {
        adfsen_func_error("gpio_read failed\n");
        return (false);
    }
#else
    logic_level = scanplat_kernel_gpio_get_value(pinetree_adf.adf_pip_sensor_gpio);
#endif
    if (logic_level != 0x00)
    {
        flag = false;
    }
    else    
    {
        flag = true;
    }
    return (flag);
}


bool adf_sensor_int_present(void)
{
    bool flag = false;
    uint32_t logic_level = 0xFFFF;
#ifndef __KERNEL__
    error_type_t err;
    err = gpio_read(adf_int_sensor_gpio, &logic_level);
    if (err != OK)
    {
        adfsen_func_error("gpio_read failed\n");
        return (false);
    }
#else
    logic_level = scanplat_kernel_gpio_get_value(pinetree_adf.adf_int_sensor_gpio);
#endif
    if (logic_level != 0x00)
    {
        flag = true;
    }
    else    
    {
        flag = false;
    }
    return (flag);
}


// by JJW (for new CIS)
bool cis_select(void)
{
    bool flag = false;
    uint32_t logic_level = 0xFFFF;

#ifndef __KERNEL__
    error_type_t err;

    err = gpio_read(cis_select_gpio, &logic_level);
    if (err != OK)
    {
        smech_func_error("gpio_read failed\n");
        return (false);
    }
#else
    logic_level = scanplat_kernel_gpio_get_value(pinetree_cis.cis_select_gpio);
    //dbg1("[JJW] cis_select , logic_level : %d\n", logic_level);
#endif
    if (logic_level != 0x00)
    {
        flag = true; 
        dbg1("[JJW] new CIS\n");
    }
    else
    {
        flag = false; 
        dbg1("[JJW] old CIS\n");
    }
    return (flag);

}

void gpio_high(void)
{
#ifndef __KERNEL__
    gpio_write(cis_select_gpio, 1);
#else
	scanplat_kernel_gpio_set_value(pinetree_cis.cis_select_gpio, 1);
#endif
}

void gpio_low(void)
{
#ifndef __KERNEL__
    gpio_write(cis_select_gpio, 0);
#else
	scanplat_kernel_gpio_set_value(pinetree_cis.cis_select_gpio, 0);
#endif	
}

void gpio_value(int val)
{
#ifndef __KERNEL__
    gpio_write(cis_select_gpio, val);
#else
	scanplat_kernel_gpio_set_value(pinetree_cis.cis_select_gpio, val);
#endif	
}
