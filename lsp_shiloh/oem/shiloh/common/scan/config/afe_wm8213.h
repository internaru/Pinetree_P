/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef WM8213_H
#define WM8213_H

/* Wolfson AFE WM8213 data and functions */
#define WM8213_REG_SETUP1   0x01
#define WM8213_REG_SETUP2   0x02
#define WM8213_REG_SETUP3   0x03
#define WM8213_REG_SWRESET  0x04
#define WM8213_REG_AUTO_RESET  0x05
#define WM8213_REG_SETUP4   0x06
#define WM8213_REG_SETUP5   0x07
#define WM8213_REG_SETUP6   0x08
#define WM8213_REG_DAC_RED  0x20
#define WM8213_REG_DAC_GRN  0x21
#define WM8213_REG_DAC_BLU  0x22
#define WM8213_REG_DAC_RGB  0x23
#define WM8213_REG_PGA_GAIN_LSB_RED  0x24
#define WM8213_REG_PGA_GAIN_LSB_GRN  0x25
#define WM8213_REG_PGA_GAIN_LSB_BLU  0x26
#define WM8213_REG_PGA_GAIN_LSB_RGB  0x27
#define WM8213_REG_PGA_GAIN_MSB_RED  0x28
#define WM8213_REG_PGA_GAIN_MSB_GRN  0x29
#define WM8213_REG_PGA_GAIN_MSB_BLU  0x2a
#define WM8213_REG_PGA_GAIN_MSB_RGB  0x2b

/* davep 07-Jul-2012 ; being lazy and only creating register fields we're
 * currently using 
 */
#define WM8213_REG_SETUP1_EN     (1<<0)
#define WM8213_REG_SETUP1_LEGACY (1<<7)

#define WM8213_REG_SETUP2_OPFORM_4bit   (0x3)

#define WM8213_REG_SETUP3_RLCDAC(n)  (n&0x0f)

#define WM8213_REG_SETUP6_VSMPDET    (1<<0)
#define WM8213_REG_SETUP6_VDEL(n)    ((n&0x7)<<1)

#define WM8213_REG_SETUP6_POSNEG_NEG (0<<4)
#define WM8213_REG_SETUP6_POSNEG_POS (1<<4)


#endif /* WM8213_H */

