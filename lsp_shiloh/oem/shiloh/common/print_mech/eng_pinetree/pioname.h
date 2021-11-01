/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **								    								
 **	 File Name :	pioname.h					    					
 +-----------------------------------------------------------------+*/

#ifndef __PIONAME_H
#define __PIONAME_H

//#define XPORT_LOCK			dec_sensor_poll(eng_hw->paper_motor_lock)
#define MM_LOCK				0		//GP_Read(NREADY)

#define MPF_SEN				dec_sensor_poll(eng_hw->paper_avail_mpt)
#define TRAY1_SEN			dec_sensor_poll(eng_hw->paper_avail)
#define DOOR_OPEN			dec_sensor_poll(eng_hw->door_open) 

#define PAPER_IN			dec_sensor_poll(eng_hw->paper_entry)
#define PAPER_OUT			dec_sensor_poll(eng_hw->paper_exit)
#define FULL_STACK_SEN		dec_sensor_poll(eng_hw->paper_outbin_full)
//#define WAKE_KEY_S			GP_Read(WAKE_KEY)
 
#endif

