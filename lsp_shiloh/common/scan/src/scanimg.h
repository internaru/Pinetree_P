/** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 * Header file for the ScanImaging class (scanimg.c), the class around the
 * Sensor and AFE.
 *
 * davep 26-Mar-2008
 *
 **/

#ifndef SCANIMG_H
#define SCANIMG_H

scan_err_t scanimg_onetime_init( void );

scan_err_t scanimg_open( scan_cmode_t cmode, uint32_t dpi, uint32_t sensor_bitmask );
scan_err_t scanimg_setup( void );
scan_err_t scanimg_run( void );
scan_err_t scanimg_stop( void );
void scanimg_close( void );

void scanimg_set_led_enable( uint32_t sensor_num, bool red, bool green, bool blue );
void scanimg_get_led_enable( uint32_t sensor_num, bool *p_red, bool *p_green, bool *p_blue );

void scanimg_set_led_exposure( uint32_t sensor_num, uint32_t expR, uint32_t gateG, uint32_t gateB );
void scanimg_get_led_exposure( uint32_t sensor_num, uint32_t *p_expR, uint32_t *p_expG, uint32_t *p_expB );

void scanimg_set_led_duty( uint32_t sensor_num, uint32_t dutyR, uint32_t dutyG, uint32_t dutyB);
void scanimg_get_led_duty( uint32_t sensor_num, uint32_t *p_dutyR, uint32_t *p_dutyG, uint32_t *p_dutyB);

void scanimg_set_analog_gain( uint32_t sensor_num, uint32_t red_gain, uint32_t green_gain, uint32_t blue_gain );
void scanimg_get_analog_gain( uint32_t sensor_num, uint32_t *p_red_gain, uint32_t *p_green_gain, uint32_t *p_blue_gain );

void scanimg_set_analog_offset( uint32_t sensor_num, int red_offset, int green_offset, int blue_offset );
void scanimg_get_analog_offset( uint32_t sensor_num, int *p_red_offset, int *p_green_offset, int *p_blue_offset );

void scanimg_set_scanx( uint32_t new_scanx );
void scanimg_get_scanx( uint32_t *scanx );

void scanimg_get_pixper( uint32_t *pixper );

void scanimg_get_cmode( scan_cmode_t *cmode );
void scanimg_get_dpi( uint32_t *dpi );
void scanimg_get_sensor_bitmask( uint32_t *sensor_bitmask );
void scanimg_set_sensor_bitmask( uint32_t sensor_bitmask );

uint32_t scanimg_calc_sclkper_from_line_time( uint32_t scan_line_time_usec );
uint32_t scanimg_calc_line_time_from_sclkper( uint32_t sclkper );
uint32_t scanimg_get_line_time_usec( void );

typedef enum {
    SCANX_ALIGN_PAD=1,
    SCANX_ALIGN_TRUNCATE,
} scanx_align_t;

scan_err_t scanimg_align_scanx( uint32_t *p_scanx, uint32_t align_to, scanx_align_t align_how );

scan_err_t scanimg_scanning_pause( void );
scan_err_t scanimg_scanning_resume( void );

#define SCANIMG_SINGLE_SENSOR(sens_bitmask) ((sens_bitmask & ~(0x1)) == 0)

#endif

