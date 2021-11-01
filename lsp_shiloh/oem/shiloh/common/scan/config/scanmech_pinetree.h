/** 
 * ============================================================================
 * Copyright (c) 2007-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *   mech driver definitions.
 *
 **/
 
#ifndef INC_SCANMECH_PINETREE_H
#define INC_SCANMECH_PINETREE_H


//MRVL-Harris-20120215 for ADF {
//supported mech types
typedef enum {
    SCAN_MECH_FLATBED,
    SCAN_MECH_ADF
} t_ScanMechType;
//MRVL-Harris-20120215 for ADF }

void smech_pinetree_init( t_ScanMech *scan_mech);


int smech_get_adf_position(void);

#ifdef __KERNEL__
/* davep 24-Sep-2013 ; Pinetree mech/motor GPIO data structure for GPIO
 * initialization (requires calls to GPL functions so in scansb module).
 */
struct pt_mech {
     unsigned int scan_home_sensor_gpio;
     unsigned int scan_nmotor_driver_stby;
     unsigned int scan_nmotor_driver_rst;

     unsigned int phase_mode_select1;
     unsigned int phase_mode_select2;
     unsigned int current_control;
     
	 unsigned int a3_clutch;
	 //unsigned int FB_enable;
};
int smech_pinetree_kernel_init( struct pt_mech *pt );
#endif

#endif /* INC_SCANMECH_PINETREE_H */
