/*
 * ======================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */
#define DITHERED_CLOCK
/// \file dcmotor_ol_config.c

#include "ATypes.h"
#include "lassert.h"
#include <stdint.h>
#include "asic.h"

#include "dcmotor_ol.h"
#include "dcmotor_ol_priv.h"
#include "mtr6pin.h"        /* Macros/declares to support DCMOTOR6PIN2 motor block */

#ifdef DITHERED_CLOCK
#define CLOCK_TICKS_PER_SEC 198000000
#else
#define CLOCK_TICKS_PER_SEC 200000000
#endif

#define PP_INT_MS 1//10


static dcmotor_ol_t my_dcmotors[1];
static dcmotor_ol_t *mydcmotors[] = { 
    &my_dcmotors[0],
//    &my_dcmotors[1],
    0
};
const const char * dcmotor_key_table[] = 
{
    "dcmotor_paper_motor",
    0
};
KEY_TABLE_DEFINE_PRIVATE_2( dcmotor, dcmotor_ol_t, dcmotor_key_table, mydcmotors );

static dcmotor_speed_t my_dcmotor_speeds[3];
static dcmotor_speed_t *mydcmotor_speeds[] = {
    &my_dcmotor_speeds[0],
    &my_dcmotor_speeds[1],
    &my_dcmotor_speeds[2],
    0
};
const const char * dcmotor_speed_key_table[] = 
{
    "speed_off",    ///< Not moving no power enabled 
    "speed_half",   ///< not used mapped to full
    "speed_full",   ///< ramp from 0 to full speed
    0
};
KEY_TABLE_DEFINE_PRIVATE_2( dcmotor_speed, dcmotor_speed_t, dcmotor_speed_key_table, mydcmotor_speeds );


 /*
   * Paper Path motor pin connections
   *   Mech configs:
   *    - Normal PWM mode
   *    - connected to motor block 2 
   *    - LD speed lock detect is mapped to Encoder A.
   *
   *   HW configured as:
   *      DC0 - PWM
   *      DC1 - ENCA LD - lock detect mapped to encoder A
   *      DC2 - ST - start/enable
   *      DC3 - DIR
   *      DC4 - FG - not currently connected.
   *      DC5 - ENCB
   */
  static const motor_connect_t motorPP_pin_connects = {
      DC_PWM_NORMAL,                        /* Normal PWM connections */
      DC_MTR_BLK_2,                         /* On ASIC motor block 0  */
      SINGLE_A,                             /* Single ended use A encoder */
      PP_INT_MS,                            /* mSec / sample period */
      {
//  		{DC_PIN_FUNC_PWM,  DC_PIN_NO_INVERT, DC_PIN_PWM0},      /* DC0 - BLDC_CLK*/
//        {DC_PIN_FUNC_ENCA, DC_PIN_INVERT,    DC_PIN_NO_SIGNAL}, /* DC1 - BLDC_LD Speed lock detect; mapped to ENCA - driven with GPIO*/
//        {DC_PIN_FUNC_EN,   DC_PIN_INVERT,    DC_PIN_ZERO},      /* DC2 - BLDC_ST */
//        {DC_PIN_FUNC_DIR,  DC_PIN_NO_INVERT, DC_PIN_ONE},       /* DC3 - BLDC_DIR High == forward, low == backward*/
//        {DC_PIN_FUNC_NC, DC_PIN_NO_INVERT, DC_PIN_NO_SIGNAL},   /* DC4 BLDC_FG */
//        {DC_PIN_FUNC_NC, DC_PIN_NO_INVERT, DC_PIN_NO_SIGNAL}    /* DC5 */
  		{DC_PIN_FUNC_PWM,  DC_PIN_NO_INVERT, DC_PIN_PWM0},      /* DC0 - BLDC_CLK*/
        {DC_PIN_FUNC_NC, DC_PIN_NO_INVERT, DC_PIN_NO_SIGNAL}, /* DC1 - BLDC_LD Speed lock detect; mapped to ENCA - driven with GPIO*/
        {DC_PIN_FUNC_NC, DC_PIN_NO_INVERT, DC_PIN_NO_SIGNAL},      /* DC2 - BLDC_ST */
        {DC_PIN_FUNC_NC, DC_PIN_NO_INVERT, DC_PIN_NO_SIGNAL},       /* DC3 - BLDC_DIR High == forward, low == backward*/
        {DC_PIN_FUNC_ENCA, DC_PIN_NO_INVERT,    DC_PIN_NO_SIGNAL},   /* DC4 BLDC_FG */
        {DC_PIN_FUNC_NC, DC_PIN_NO_INVERT, DC_PIN_NO_SIGNAL}    /* DC5 */
      }
  };

static const motor_param_t motorPP_move_params = { 30 };

void dcmotor_local_init(void)
{
    extern void smot_dc_enter_sleep_4931(dcmotor_ol_t * dcmotor);
    extern void smot_dc_exit_sleep_4931(dcmotor_ol_t * dcmotor);
    mydcmotor_speeds[0]->index = 0;
    mydcmotor_speeds[0]->eta[0] = 100; /// 100msec to stop


    mydcmotor_speeds[1]->index = 1;
    mydcmotor_speeds[1]->move_params[0] = &motorPP_move_params;
    mydcmotor_speeds[1]->eta[0] = 4000; /// msec to get to speed

    mydcmotor_speeds[2]->index = 2;
    mydcmotor_speeds[2]->move_params[0] = &motorPP_move_params;
    mydcmotor_speeds[2]->eta[0] = 4000; /// msec to get to speed


    mydcmotors[0]->motor_id = 0;
    mydcmotors[0]->smot_dc_enter_sleep_func = smot_dc_enter_sleep_4931;
    mydcmotors[0]->smot_dc_exit_sleep_func = smot_dc_exit_sleep_4931;
    mydcmotors[0]->motor_use = laser_drive_motor;
    mydcmotors[0]->pwm_freq = 31000;//31kHz //2677; // 2.677 kHz

#ifdef __linux__
    mydcmotors[0]->use_encoder_isr = false;  // on the LSP we don't currently use the encoder ISR due to latencies, but on TX we do
#else
    mydcmotors[0]->use_encoder_isr = true;  // on the LSP we don't currently use the encoder ISR due to latencies, but on TX we do
#endif
    dcmotor_create_motor(mydcmotors[0], &motorPP_pin_connects);

}



