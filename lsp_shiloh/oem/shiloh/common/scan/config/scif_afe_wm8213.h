/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2006   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan Interface for WM8213
 * Author: Harris Huang
 *
 *****************************************************************************/

#ifndef SCANIF_WM8213_H
#define SCANIF_WM8213_H


#define SCIF_AFEPD                ((volatile UINT32 *)(ICE_BASE+0x400c0))
#define SCIF_AFEPD_RDWR(x)        (((x) & 0x1) << 31)
#define SCIF_AFEPD_CCODE(x)       (((x) & 0x3) << 28)
#define SCIF_AFEPD_CLOC(x)        (((x) & 0xf) << 24)
#define SCIF_AFEPD_DATA(x)        (((x) & 0xffffff) << 0)
#define SCIF_AFEPD_DATA_WIDTH     24 //field width of serial data


void scif_afe_wm8213_setup(SCIF_AFE_CONF ac);


unsigned int scif_afe_wm8213_read(unsigned int reg);


void scif_afe_wm8213_write(unsigned int reg, unsigned int data);


#endif //SCANIF_WM8213_H

