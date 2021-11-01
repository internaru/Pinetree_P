/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* 
 * Scan block LED control.
 *
 * davep 03-Jul-2012 ; adding 6170 multiple sensor support.
 */

#include <stdint.h>
#include <stdbool.h>

#include "lassert.h"
#include "regAddrs.h"
#include "SCAN_regstructs.h"
#include "SCAN_regmasks.h"

#include "scantypes.h"
#include "scancore.h"
#include "scanif.h"
#include "scandbg.h"
#include "scansen.h"

static volatile SCAN_REGS_t * const scif_regs = (volatile SCAN_REGS_t*)(ICE_SCAN_BASE);


// scif_led_duty - compute time on in pixel for each LED
// based on the requested duty cycle for each LED, compute the amount of time
// that the LEDs should be on during the lpwmperiod.  Turn on at 0, off at the calculated time.
// Note that we don't set the LEDPOL - 0 is the reset value we write in scif_led_reset, and
// 0 is pwm normally low - so we are already set to the proper value.
void scif_led_duty(uint32_t sensor_num, unsigned int lpwmperiod, uint32_t dutyR, uint32_t dutyG, uint32_t dutyB)
{
    uint32_t lpwmmin, lpwmmax, ledRoff, ledGoff, ledBoff;

    dbg2( "%s s#=%d dutyR=%d dutyG=%d dutyB=%d (these are duty cycle percentages!) pwmperiod=%d\n", __FUNCTION__, sensor_num, dutyR, dutyG, dutyB,lpwmperiod );


    lpwmmax = lpwmperiod - 1;

    // lowest value 0 - in case caller wants 100%, starting at 0 allows that
    lpwmmin = 0;

    // The duty cycle is the amount of time an LED is on during a pixel time.  If the pixel period
    // is 100, and the duty cycle is 20%, you can turn on the LED at time 79, and off at time 99.
    // On at 1, off at 21 would also be equally mathematically valid
    ledRoff = (lpwmperiod * dutyR) / 100 + lpwmmin;
    if (ledRoff > lpwmmax)
        ledRoff = lpwmmax;
    ledGoff = (lpwmperiod * dutyG) / 100 + lpwmmin;
    if (ledGoff > lpwmmax)
        ledGoff = lpwmmax;
    ledBoff = (lpwmperiod * dutyB) / 100 + lpwmmin;
    if (ledBoff > lpwmmax)
        ledBoff = lpwmmax;

    // special case - 100% means don't allow the LEDs to turn off at all for noise reduction.  so
    // for this case, we set the led*off to be *higher* than the lpwmperiod, and the ASIC will never
    // turn off the LED

    if (dutyR == 100)
        ledRoff = lpwmperiod + 1;
    if (dutyG == 100)
        ledGoff = lpwmperiod + 1;
    if (dutyB == 100)
        ledBoff = lpwmperiod + 1;

    if (sensor_num == 0)
    {
        scif_regs->LPWMConfig0 = SCAN_LPWMCONFIG0_LPWMPER_REPLACE_VAL(scif_regs->LPWMConfig0, lpwmperiod);
        scif_regs->LPWM00 = SCAN_LPWM00_LED0ON_REPLACE_VAL(scif_regs->LPWM00, lpwmmin );
        scif_regs->LPWM00 = SCAN_LPWM00_LED0OFF_REPLACE_VAL(scif_regs->LPWM00, ledRoff );
        scif_regs->LPWM10 = SCAN_LPWM10_LED1ON_REPLACE_VAL(scif_regs->LPWM10, lpwmmin );
        scif_regs->LPWM10 = SCAN_LPWM10_LED1OFF_REPLACE_VAL(scif_regs->LPWM10, ledGoff );
        scif_regs->LPWM20 = SCAN_LPWM20_LED2ON_REPLACE_VAL(scif_regs->LPWM20, lpwmmin );
        scif_regs->LPWM20 = SCAN_LPWM20_LED2OFF_REPLACE_VAL(scif_regs->LPWM20, ledBoff );
    }
    else 
    {
        scif_regs->LPWMConfig1 = SCAN_LPWMCONFIG1_LPWMPER_REPLACE_VAL(scif_regs->LPWMConfig1, lpwmperiod);
        scif_regs->LPWM01 = SCAN_LPWM01_LED0ON_REPLACE_VAL(scif_regs->LPWM01, lpwmmin );
        scif_regs->LPWM01 = SCAN_LPWM01_LED0OFF_REPLACE_VAL(scif_regs->LPWM01, ledRoff );
        scif_regs->LPWM11 = SCAN_LPWM11_LED1ON_REPLACE_VAL(scif_regs->LPWM11, lpwmmin );
        scif_regs->LPWM11 = SCAN_LPWM11_LED1OFF_REPLACE_VAL(scif_regs->LPWM11, ledGoff );
        scif_regs->LPWM21 = SCAN_LPWM21_LED2ON_REPLACE_VAL(scif_regs->LPWM21, lpwmmin );
        scif_regs->LPWM21 = SCAN_LPWM21_LED2OFF_REPLACE_VAL(scif_regs->LPWM21, ledBoff );
    }
}

void scif_ledR_gate(uint32_t sensor_num, unsigned int pos, unsigned int neg)
{
    if (sensor_num == 0)
    {
        scif_regs->LED00 = SCAN_LPWM00_LED0ON_REPLACE_VAL( scif_regs->LED00, pos );
        scif_regs->LED00 = SCAN_LPWM00_LED0OFF_REPLACE_VAL( scif_regs->LED00, neg );
    }
    else
    {
        scif_regs->LED01 = SCAN_LPWM01_LED0ON_REPLACE_VAL( scif_regs->LED01, pos );
        scif_regs->LED01 = SCAN_LPWM01_LED0OFF_REPLACE_VAL( scif_regs->LED01, neg );
    }
}

void scif_ledG_gate(uint32_t sensor_num, unsigned int pos, unsigned int neg)
{

    if (sensor_num == 0)
    {
        scif_regs->LED10 = SCAN_LPWM10_LED1ON_REPLACE_VAL( scif_regs->LED10, pos );
        scif_regs->LED10 = SCAN_LPWM10_LED1OFF_REPLACE_VAL( scif_regs->LED10, neg );
    }
    else
    {
        scif_regs->LED11 = SCAN_LPWM11_LED1ON_REPLACE_VAL( scif_regs->LED11, pos );
        scif_regs->LED11 = SCAN_LPWM11_LED1OFF_REPLACE_VAL( scif_regs->LED11, neg );
    }
}

void scif_ledB_gate(uint32_t sensor_num, unsigned int pos, unsigned int neg)
{

    if (sensor_num == 0)
    {
        
        scif_regs->LED20 = SCAN_LPWM20_LED2ON_REPLACE_VAL( scif_regs->LED20, pos );
        scif_regs->LED20 = SCAN_LPWM20_LED2OFF_REPLACE_VAL( scif_regs->LED20, neg );
    }
    else
    {
        scif_regs->LED21 = SCAN_LPWM21_LED2ON_REPLACE_VAL( scif_regs->LED21, pos );
        scif_regs->LED21 = SCAN_LPWM21_LED2OFF_REPLACE_VAL( scif_regs->LED21, neg );
    }
}

void scif_led_enable(uint32_t sensor_num, int onR, int onG, int onB)
{
    uint32_t num32;

//    dbg2( "%s onR=%d onG=%d onB=%d, s#=%d\n", __FUNCTION__, onR, onG, onB,sensor_num );

    //num32 = (onR<<2) | (onG<<1) | onB;
    num32 = (onB<<2) | (onG<<1) | onR;
    if (sensor_num == 0)
    {
        scif_regs->SCFG2 = SCAN_SCFG2_LEDEN0_REPLACE_VAL( scif_regs->SCFG2, num32 );
    }
    else
    {
        scif_regs->SCFG2 = SCAN_SCFG2_LEDEN1_REPLACE_VAL( scif_regs->SCFG2, num32 );
    }

    /* XXX temp debug */
//    dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
//    scif_dump();
//    afe_dump();
}

void scif_led_reset( void )
{
    uint32_t sensor_num;
    /* looks like zero is a safe reset value for all these */
    scif_regs->LPWMConfig0 = 0;
    scif_regs->LPWM00 = 0;
    scif_regs->LPWM00 = 0;
    scif_regs->LPWM10 = 0;
    scif_regs->LPWM10 = 0;
    scif_regs->LPWM20 = 0;
    scif_regs->LPWM20 = 0;

    scif_regs->LPWMConfig1 = 0;
    scif_regs->LPWM01 = 0;
    scif_regs->LPWM01 = 0;
    scif_regs->LPWM11 = 0;
    scif_regs->LPWM11 = 0;
    scif_regs->LPWM21 = 0;
    scif_regs->LPWM21 = 0;

    for (sensor_num=0;sensor_num < scansen_get_num_sensors();sensor_num++)
    {
        scif_ledR_gate(sensor_num,0,0);
        scif_ledG_gate(sensor_num,0,0);
        scif_ledB_gate(sensor_num,0,0);
    }
}

void scif_led_dump( void )
{
    dbg1( "%8s=0x%08x %8s=0x%08x %8s=0x%08x\n", 
            "LED00", scif_regs->LED00,
            "LED10", scif_regs->LED10,
            "LED20", scif_regs->LED20
        );
    dbg1( "%8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x\n", 
            "LPWMCfg0", scif_regs->LPWMConfig0,
            "LPWM00", scif_regs->LPWM00,
            "LPWM10", scif_regs->LPWM10,
            "LPWM20", scif_regs->LPWM20
        );
    dbg1( "%8s=0x%08x %8s=0x%08x %8s=0x%08x\n", 
            "LED01", scif_regs->LED01,
            "LED11", scif_regs->LED11,
            "LED21", scif_regs->LED21
        );
    dbg1( "%8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x\n", 
            "LPWMCfg1", scif_regs->LPWMConfig1,
            "LPWM01", scif_regs->LPWM01,
            "LPWM11", scif_regs->LPWM11,
            "LPWM21", scif_regs->LPWM21
        );
}

