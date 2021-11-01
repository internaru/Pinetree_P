/* 
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANPLAT_SB_H
#define SCANPLAT_SB_H 

struct pt_adf;
struct pt_cis;
struct pt_mech;

int adf_sensor_kernel_onetime_init( struct pt_adf *adf, bool (*pip_isr)(void) );
int pinetree_cis_select_kernel_init( struct pt_cis *cis ); //by JJW (for new CIS)
int smech_pinetree_kernel_init( struct pt_mech *pt );

void scanplat_kernel_gpio_set_value( unsigned int gpio, int value );
void scanplat_kernel_gpio_set_value( unsigned int gpio, int value );
int scanplat_kernel_gpio_get_value( unsigned int gpio );

struct dma_chan;
struct mv61_dma_slave;

int scanplat_kernel_get_stepper_dma( struct dma_chan **stmotor_dma_chan,
                                     struct mv61_dma_slave *tx_dma_slave );

void scanplat_kernel_release_stepper_dma( struct dma_chan **stmotor_dma_chan );

#endif

