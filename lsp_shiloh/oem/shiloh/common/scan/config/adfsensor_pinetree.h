/* 
 * ============================================================================
 * Copyright (c) 2009-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file adfsensor_pinetree.h
 *
 * \brief internal Shiloh pinetree ADF sensor function definitions
 *
 *  internal scan mech Shiloh pinetree ADF sensor function definitions.  
 *
 **/

#ifndef ADFSENSOR_PINETREE_H
#define ADFSENSOR_PINETREE_H

//MRVL-Harris-20120215 for ADF {
#define SMECH_ADF_ERROR_CIO          0xE0
#define SMECH_ADF_ERROR_JAM          0xE1
//MRVL-Harris-20120215 for ADF }

/*
 *********************************************************************************************
 * Regarding ADF, Distinguish method between Rework Board and original Board.
 *	0 : for Original Board
 *	1 : for Rework Board 
 *********************************************************************************************
 */
#define GPIO_ISR 0

#define TEST_PORT_G19 0

bool adf_sensor_cover_is_open(void);
bool adf_sensor_int_present(void);

bool cis_select(void); //by JJW
void gpio_high(void);
void gpio_low(void);
void gpio_value(int val);//val:1 -> high, val:0 -> low


void adf_sensor_pip_debounce_sample(void); //MRVL-Harris-20111223

//MRVL-Harris-20120215 for ADF {
void adf_sensor_cio_debounce_sample(void);
void adf_sensor_cio_callback_enable(void);
void adf_sensor_cio_callback_disable(void);
//MRVL-Harris-20120215 for ADF }

void adf_sensor_pp_debounce_sample(void); //MRVL-Harris 20121030

#ifdef __KERNEL__
/* davep 24-Sep-2013 ; Pinetree ADF GPIO data structure for GPIO initialization
 * (requires calls to GPL functions so in scansb module).
 */
struct pt_adf {
    unsigned int adf_pip_sensor_gpio;
    unsigned int adf_pp_sensor_gpio;
    unsigned int adf_cio_sensor_gpio;
    unsigned int adf_int_sensor_gpio;
};

struct pt_cis { //by JJW (for new CIS)
	unsigned int cis_select_gpio; 
};

int adf_sensor_kernel_onetime_init( struct pt_adf *adf, bool (*pip_isr)(void) );
#endif

#endif //ADFSENSOR_PINETREE_H

