/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Agilent Technologies, Inc. All Rights Reserved
 *                      
 *                         Agilent Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef SCANRESET_H
#define SCANRESET_H

/* converted by \bin\vh_to_ch.py from scanreset.vh on 07-Dec-2004 */
/* Register Reset Values 
 *
 * Verified as correct as I can as of 10-Dec-04.
 *      -DaveP
 */
/* 11-Feb-05 ; verified against "Scan Block HLD 1.6" */
/* davep 30-Sep-2005 ; verified against "Scan Block HLD 1.8" */
#define SCAN_CFG1_R         0x0000f000 
#define SCAN_CFG2_R         0x00000000 
#define SCAN_CFG3_R         0x00000000 
#define SCAN_CTRL_R         0x00000000 
#define SCAN_STAT_R         0x00000020 
#define SCAN_X_R            0x00000000 
#define SCAN_CYCLE_R        0x00000000 
#define SCAN_START_CFG1_R   0x00ff00ff 
#define SCAN_START_CFG2_R   0x00ff00ff 
#define SCAN_START_CFG3_R   0xffffffff 
#define SCAN_START_CFG4_R   0xffffffff 
#define SCAN_CLMP_CFG_R     0x00ffffff 
#define SCAN_SENS_CLK1_R    0xffffffff 
#define SCAN_SENS_CLK2_R    0xffffffff 
#define SCAN_SENS_CLK3_R    0xffffffff 
#define SCAN_SENS_CLK4_R    0xffffffff 
#define SCAN_SENS_CLK5_R    0xffffffff 
#define SCAN_SENS_CLK6_R    0xffffffff 
#define SCAN_AFE_CLK1_R     0xffffffff 
#define SCAN_AFE_CLK2_R     0xffffffff 
#define SCAN_AFE_CLK3_R     0xffffffff 
#define SCAN_AFE_CLK4_R     0xffffffff 
#define SCAN_AFE_CLK5_R     0xffffffff 
#define SCAN_AFE_LDATA1_R   0xff00ff00 
#define SCAN_AFE_LDATA2_R   0xff00ff00 
#define SCAN_AFE_LDATA3_R   0xff00ff00 
#define SCAN_AFE_LDATA4_R   0xff00ff00 
#define SCAN_AFE_LDATA5_R   0xff00ff00 
#define SCAN_AFE_LDATA6_R   0xff00ff00 
#define SCAN_AFE_PSEQ1_R    0x00000000 
#define SCAN_AFE_PSEQ2_R    0x00000000 
#define SCAN_BULB_PWM_R     0x0fff0000 
#define SCAN_LED_PWM_R      0x007f7f7f 
#define SCAN_LED0_GATE_R    0x00000000 
#define SCAN_LED1_GATE_R    0x00000000 
#define SCAN_LED2_GATE_R    0x00000000 
#define SCAN_MCFG_R         0x00000000 
#define SCAN_L6219_MCTRL_R  0x00ef00ef 
#define SCAN_A3967_MCTRL_R  0x00000000 
#define SCAN_T62209_MCTRL_R 0x00000000 
#define SCAN_M_PWMCFG_R     0x00000000 
#define SCAN_PWM_CTRLA_R    0xffffffff 
#define SCAN_PWM_CTRLB_R    0xffffffff 
#define SCAN_PWM_CTRLC_R    0xffffffff 
#define SCAN_PWM_CTRLD_R    0xffffffff 
#define SCAN_AFE_PROGC_R    0x00000d57 
#define SCAN_LPWM0_R        0x00000000 
#define SCAN_LPWM1_R        0x00000000 

/* davep 28-Jan-2013 ; adding CFGARB reset; verified against 6170 programmer's
 * guide rev3.1 
 */
#define SCAN_CFGARB_R   0x00000001

#endif // SCANRESET_H

