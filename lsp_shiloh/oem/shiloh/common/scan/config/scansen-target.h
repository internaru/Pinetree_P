/*

 * ============================================================================

 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved

 *                      

 *                         Marvell Confidential

 * ============================================================================

 */

/* 

 * This file is a h/a/c/k to support building different executables with

 * different sensors.

 * 

 * dave 25-May-2011

 */



#ifndef SCANSEN_TARGET_H

#define SCANSEN_TARGET_H



#define SCAN_SENSOR_TYPE1     87

#define SCAN_SENSOR_TYPE2     88 





/** 

 * \brief LED gate timing 

 *

 */

typedef struct {

    scan_cmode_t   cmode;

    uint32_t         dpi;

    uint32_t         edgeR;

    uint32_t         edgeF;

} scan_led_timing_t;





/** 

 * \brief SCANX gate timing 

 *

 */

typedef struct {

    scan_cmode_t   cmode;

    uint32_t         dpi;

    uint32_t         scanx;

} scan_scanx_conf_t;





/**

 *  \brief CISX configuration

 *

 **/

struct scan_cisx_conf {

    uint32_t                     dpi;           

    struct cisx_sensor_conf    cisx_conf;

};





int scansen_get_sensor_id( void );



#endif



