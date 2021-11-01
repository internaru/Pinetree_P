/* 
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/*
 * Some kernel functions can only be called from GPL'd modules. Split the
 * scantask into three modules: 
 *      kscanman (GPL; kernel/userspace interface)
 *      kscantask (Proprietary; the bulk of the scan code)
 *      scansb (GPL; kernel functions that require GPL module)
 *
 * davep 25-Sep-2013 
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <asm/gpio.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/mv61_cdma.h>
#include <linux/interrupt.h>

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scanplat.h"
#include "scanmech.h"
#include "adfsensor_pinetree.h"
#include "scanmech_pinetree.h" 
#include "scanplat_sb.h"

/**
 * \brief  Linux GPL code 
 *
 * \author David Poole
 * \date 20-Sep-2013
 */

/* davep 20-Sep-2013 ; moving ADF GPIO init to GPL code */
#define GPIO_C(n)   (64+n)
#define GPIO_D(n)   (96+n)
#define GPIO_F(n)   (160+n)
#define GPIO_G(n)   (192+n)

static bool (*pip_isr)(void) = NULL;
static irqreturn_t pip_irq_handler(int irq, void *dev_id)
{
    if (pip_isr != NULL)
    {
        if (pip_isr())
        {
            return IRQ_HANDLED;
        }
    }
    return IRQ_NONE;
}


int adf_sensor_kernel_onetime_init( struct pt_adf *adf, bool (*pip_handler)(void) )
{
    int err;
    int pip_irq = 0;

#if GPIO_ISR
    adf->adf_pip_sensor_gpio = GPIO_F(23);
#else    
    adf->adf_pip_sensor_gpio = GPIO_F(1);  //Mod.kks 
#endif    
    err = gpio_request(adf->adf_pip_sensor_gpio, "ADF Paper In Path Sensor");
    XASSERT(err==0, err);
    err = gpio_direction_input(adf->adf_pip_sensor_gpio);
    XASSERT(err==0, err);
    
    // Per programmer's guide, deglitch setting 0x08 will deglitch between 3-4us
    gpio_set_deglitch(adf->adf_pip_sensor_gpio, 0x08);

    pip_isr = pip_handler;
    pip_irq = gpio_to_irq(adf->adf_pip_sensor_gpio);
    XASSERT(pip_irq >= 0, pip_irq);
    err = request_irq(pip_irq, pip_irq_handler, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "PIP intr", "PIP intr");
    XASSERT(err==0, err);

    adf->adf_pp_sensor_gpio = GPIO_F(5);
    err = gpio_request(adf->adf_pp_sensor_gpio, "ADF Paper Present Sensor");
    XASSERT(err==0, err);
    err = gpio_direction_input(adf->adf_pp_sensor_gpio);
    XASSERT(err==0, err);

    adf->adf_cio_sensor_gpio = GPIO_F(8);
    err = gpio_request(adf->adf_cio_sensor_gpio, "ADF Cover Is Open Sensor");
    XASSERT(err==0, err);
    err = gpio_direction_input(adf->adf_cio_sensor_gpio);
    XASSERT(err==0, err);

    adf->adf_int_sensor_gpio = GPIO_F(6);
    err = gpio_request(adf->adf_int_sensor_gpio, "ADF Interval Sensor");
    XASSERT(err==0, err);
    err = gpio_direction_input(adf->adf_int_sensor_gpio);
    XASSERT(err==0, err);

    return 0;
}

int pinetree_cis_select_kernel_init( struct pt_cis *cis )
{  // CIS Select									by JJW (for new CIS)
    int err;
    
    cis->cis_select_gpio = GPIO_G(19);
    err = gpio_request(cis->cis_select_gpio, "CIS Select");
    XASSERT(err==0, err);
#if TEST_PORT_G19
    err = gpio_direction_output(cis->cis_select_gpio,0);
#else
    err = gpio_direction_input(cis->cis_select_gpio);
#endif

    XASSERT(err==0, err);
	
	return 0;
}


/**
 * \brief  
 *
 * \author David Poole
 * \date 20-Sep-2013
 */

#define GPIO_C(n)   (64+n)
#define GPIO_D(n)   (96+n)
#define GPIO_F(n)   (160+n)

//static unsigned int       scan_home_sensor_gpio;
//static unsigned int       scan_nmotor_driver_stby;
//static unsigned int       scan_nmotor_driver_rst;
//
//static unsigned int       phase_mode_select1;
//static unsigned int       phase_mode_select2;
//static unsigned int       current_control;

int smech_pinetree_kernel_init( struct pt_mech *pt )
{
    int err;

    /*phase Select*/
    pt->phase_mode_select1 = GPIO_D(0);
    err = gpio_request(pt->phase_mode_select1, "Phase mode Select1");
    XASSERT(err==0, err);
    err = gpio_direction_output(pt->phase_mode_select1,0);
    XASSERT(err==0, err);

    pt->phase_mode_select2 = GPIO_D(1);
    err = gpio_request(pt->phase_mode_select2, "Phase mode Select2");
    XASSERT(err==0, err);
    err = gpio_direction_output(pt->phase_mode_select2,1);
    XASSERT(err==0, err);
   
    /*    
     *     Phase    mode1     mode2             
     *          2Phase        0             0
     *          1-2Phase     1             0
     *       W1-2Phase     0            1
     *     2W1-2Phase      1           1         
     *                                                      
     */
       
    gpio_set_value(pt->phase_mode_select1, 0);
    gpio_set_value(pt->phase_mode_select2, 1);
    
    pt->current_control = GPIO_D(9);
    err = gpio_request(pt->current_control, "Current Control");
    XASSERT(err==0, err);
    err = gpio_direction_output(pt->current_control,0);
    XASSERT(err==0, err);
    gpio_set_value(pt->current_control, 0);     //High Current 0.7A
    
    pt->a3_clutch = GPIO_D(2);
    err = gpio_request(pt->a3_clutch, "A3_clutch");
    XASSERT(err==0, err);
    err = gpio_direction_output(pt->a3_clutch,0);
    XASSERT(err==0, err);
    gpio_set_value(pt->a3_clutch, 0); 
    
    /*pt->FB_enable = GPIO_D(26);
    err = gpio_request(pt->FB_enable, "FB_Enable");
    XASSERT(err==0, err);
    err = gpio_direction_output(pt->FB_enable,0);   //LOW Enable.  High Disable.
    XASSERT(err==0, err);
    gpio_set_value(pt->FB_enable, 0);    */ 
      
    pt->scan_home_sensor_gpio = GPIO_F(9);
    err = gpio_request(pt->scan_home_sensor_gpio, "Scanner Home Sensor");
    XASSERT(err==0, err);
    err = gpio_direction_input(pt->scan_home_sensor_gpio);
    XASSERT(err==0, err);

    /* davep 30-Aug-2012 ; switch to GPIOD-6 */
    pt->scan_nmotor_driver_stby = GPIO_D(6);
//    scan_nmotor_driver_stby = GPIO_D(21);
    err = gpio_request(pt->scan_nmotor_driver_stby, "nMotor Driver STBY");
    XASSERT(err==0, err);
    err = gpio_direction_output(pt->scan_nmotor_driver_stby,1);
    XASSERT(err==0, err);
#ifdef HAVE_PINETREE_FORM_FACTOR
    gpio_set_value(pt->scan_nmotor_driver_stby, 1);
#endif

    pt->scan_nmotor_driver_rst = GPIO_D(7);
    err = gpio_request(pt->scan_nmotor_driver_rst, "nMotor Driver RST");
    XASSERT(err==0, err);
    err = gpio_direction_output(pt->scan_nmotor_driver_rst,1);
    XASSERT(err==0, err);
#ifdef HAVE_PINETREE_FORM_FACTOR
    gpio_set_value(pt->scan_nmotor_driver_rst, 1);
#endif

    return 0;
}

void scanplat_kernel_gpio_set_value( unsigned int gpio, int value )
{
    gpio_set_value(gpio,value);
}

int scanplat_kernel_gpio_get_value( unsigned int gpio )
{
    return gpio_get_value(gpio);
}

int scanplat_kernel_get_stepper_dma( struct dma_chan **stmotor_dma_chan,
                                     struct mv61_dma_slave *tx_dma_slave ) 
{
    dma_cap_mask_t mask;

    dma_cap_zero(mask);
    dma_cap_set(DMA_SLAVE,mask);

    *stmotor_dma_chan = dma_request_channel(mask,filter,tx_dma_slave);

    return 0;
}

void scanplat_kernel_release_stepper_dma( struct dma_chan **stmotor_dma_chan )
{
    dma_release_channel(*stmotor_dma_chan);
    *stmotor_dma_chan = NULL;
}

static int __init scanplat_sb_init_module(void)
{
    /* 
     * A non 0 return means init_module failed; module can't be loaded. 
     */
    return 0;
}

static void __exit scanplat_sb_cleanup_module(void)
{
}

module_init( scanplat_sb_init_module );
module_exit( scanplat_sb_cleanup_module );

EXPORT_SYMBOL(adf_sensor_kernel_onetime_init);
EXPORT_SYMBOL(pinetree_cis_select_kernel_init); //by JJW (for new CIS)
EXPORT_SYMBOL(smech_pinetree_kernel_init);

EXPORT_SYMBOL(scanplat_kernel_gpio_set_value);
EXPORT_SYMBOL(scanplat_kernel_gpio_get_value);

EXPORT_SYMBOL(scanplat_kernel_get_stepper_dma);
EXPORT_SYMBOL(scanplat_kernel_release_stepper_dma);

MODULE_AUTHOR("Kernel level code for low level scan hardware interfaces.");
MODULE_DESCRIPTION("scanplat_sb");

MODULE_LICENSE("GPL");
MODULE_VERSION("2013_SEP_24");


