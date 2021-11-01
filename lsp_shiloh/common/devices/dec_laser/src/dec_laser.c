/*
 * ======================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */
/// \file dec_laser.c
/// 
/// WARNING this block isn't general purpose yet but rather
/// highly coupled for now, revisit...
///

#include "lassert.h"
#include "asic.h"
#include "sys_init_api.h"

#include <stdint.h>
#include "logger.h"
#include "debug.h"
#include <string.h>
#include <stdlib.h>
#include "delay_api.h"
#include "dec_laser.h"
#include "dec_laser_priv.h"
#include "dec_laser_regs_config.h"
#include "hips_pll_api.h"
#include "ATypes.h"


// BUGBUG: FIX THIS: THIS NEEDS SPLIT OUT
// THIS FILE IS NOT SHIPPABLE THE WAY IT IS



#ifndef ABS
#define ABS( x ) (x) > 0 ? (x) : -(x)
#endif
#define QUEUE_SIZE        4  * 2      // max laser * 2 so we won't fill queue. 
typedef struct dec_laser_msg_s 
{
    dec_laser_t *laser;
} dec_laser_msg_t;

static mqd_t         dec_laser_mq;
static pthread_t     dec_laser_mgr_thread;
ALIGN8 static unsigned char dec_laser_thread_stack[POSIX_MIN_STACK_SIZE];

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
int dec_laser_dbcallback( int argc, char *argv[] );
#endif

void dec_laser_spin_complete( dec_laser_t *laser, dec_laser_state_t state);

//static bool dec_laser_spin_phase_exec(dec_laser_t *laser, uint32_t phase) ;

/**
 * Following functions are used to init various registers
 */

/**
 * inits the hcfg register of the lscan_hsync
 * 
 */

void init_lscan_hsync_hcfg(HSYNC_REGS_t *lscan_hsync, uint32_t en, uint32_t wfg_enable, uint32_t pulse_type, uint32_t pulse_qual,
                                  uint32_t hsync_qual, uint32_t in_pol, uint32_t out_pol, uint32_t enable)
{
    uint32_t temp_reg;

    temp_reg = HSYNC_HCFG_GALVO_EN_REPLACE_VAL(lscan_hsync->hcfg,   en );
    temp_reg = HSYNC_HCFG_WFG_ENABLE_REPLACE_VAL(temp_reg, wfg_enable ); 
    temp_reg = HSYNC_HCFG_PULSE_TYPE_REPLACE_VAL(temp_reg, pulse_type ); 
    temp_reg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(temp_reg, pulse_qual ); 
    temp_reg = HSYNC_HCFG_HSYNC_QUAL_REPLACE_VAL(temp_reg, hsync_qual );
    temp_reg = HSYNC_HCFG_IN_POL_REPLACE_VAL(temp_reg, in_pol );
    temp_reg = HSYNC_HCFG_OUT_POL_REPLACE_VAL(temp_reg, out_pol );
    temp_reg = HSYNC_HCFG_SOS_SEL_REPLACE_VAL(temp_reg, 0 );
    lscan_hsync->hcfg = HSYNC_HCFG_ENABLE_REPLACE_VAL(temp_reg, enable );
}

/**
 * Inits the filter register of the lscan_hsync
 * 
 */

void init_lscan_hsync_filter(HSYNC_REGS_t *lscan_hsync,
                             uint32_t fall_hi, uint32_t fall_lo, 
                             uint32_t rise_hi, uint32_t rise_lo, 
                             uint32_t hfilt_thres)
{
    uint32_t temp_reg;

    temp_reg = HSYNC_HFILT_FALL_HI_REPLACE_VAL(lscan_hsync->hfilt,fall_hi);
    temp_reg = HSYNC_HFILT_FALL_LO_REPLACE_VAL(temp_reg,fall_lo);
    temp_reg = HSYNC_HFILT_RISE_LO_REPLACE_VAL(temp_reg,rise_lo);
    temp_reg = HSYNC_HFILT_RISE_HI_REPLACE_VAL(temp_reg,rise_hi);
    lscan_hsync->hfilt = HSYNC_HFILT_THRESHOLD_REPLACE_VAL(temp_reg,hfilt_thres);
}

/**
 * Inits the timers of the lscan_hsync
*/
void init_lscan_hsync_timers(HSYNC_REGS_t *lscan_hsync,
                             uint32_t tcfg1, uint32_t tcfg2,
                             uint32_t event01, uint32_t event02, uint32_t event03, uint32_t event04,
                             uint32_t event05, uint32_t event06, uint32_t event07, uint32_t event08,
                             uint32_t event09, uint32_t event10, uint32_t event11, uint32_t event12,
                             uint32_t event13, uint32_t event14, uint32_t event15, uint32_t event16
                             )
{
   uint32_t temp_reg;

   lscan_hsync->tcfg1 = tcfg1;  
   lscan_hsync->tcfg2 = tcfg2;  
   temp_reg = HSYNC_TCFG1_EVENT01_REPLACE_VAL(lscan_hsync->tcfg1, event01);  
   temp_reg = HSYNC_TCFG1_EVENT02_REPLACE_VAL(temp_reg, event02);  
   temp_reg = HSYNC_TCFG1_EVENT03_REPLACE_VAL(temp_reg, event03);  
   temp_reg = HSYNC_TCFG1_EVENT04_REPLACE_VAL(temp_reg, event04);  
   temp_reg = HSYNC_TCFG1_EVENT05_REPLACE_VAL(temp_reg, event05);
   temp_reg = HSYNC_TCFG1_EVENT06_REPLACE_VAL(temp_reg, event06);
   temp_reg = HSYNC_TCFG1_EVENT07_REPLACE_VAL(temp_reg, event07);
   lscan_hsync->tcfg1 = HSYNC_TCFG1_EVENT08_REPLACE_VAL(temp_reg, event08);

   temp_reg = HSYNC_TCFG2_EVENT09_REPLACE_VAL(lscan_hsync->tcfg2, event09);
   temp_reg = HSYNC_TCFG2_EVENT10_REPLACE_VAL(temp_reg, event10);
   temp_reg = HSYNC_TCFG2_EVENT11_REPLACE_VAL(temp_reg, event11);
   temp_reg = HSYNC_TCFG2_EVENT12_REPLACE_VAL(temp_reg, event12);
   temp_reg = HSYNC_TCFG2_EVENT13_REPLACE_VAL(temp_reg, event13);
   temp_reg = HSYNC_TCFG2_EVENT14_REPLACE_VAL(temp_reg, event14);
   temp_reg = HSYNC_TCFG2_EVENT15_REPLACE_VAL(temp_reg, event15);
   lscan_hsync->tcfg2 = HSYNC_TCFG2_EVENT16_REPLACE_VAL(temp_reg, event16);
}

/**
 * Inits the mirror[n] mcfg1 reg
 * 
 */

void init_mirror_mcfg1(MIRROR0_REGS_t *lscan_mirror, 
                       uint32_t mode, uint32_t hsync_sel, uint32_t facets, uint32_t control)
{
   uint32_t temp_reg;

   temp_reg = MIRROR0_MCFG1_MODE_REPLACE_VAL(lscan_mirror->mcfg1, mode );       
   temp_reg = MIRROR0_MCFG1_HSYNC_SEL_REPLACE_VAL(temp_reg, hsync_sel );  
   temp_reg = MIRROR0_MCFG1_FACETS_REPLACE_VAL(temp_reg, facets );     
   lscan_mirror->mcfg1 = MIRROR0_MCFG1_CONTROL_REPLACE_VAL(temp_reg, control );
}

/**
 * Inits the mirror[n] mcfg2 reg
 * 
 */

void init_mirror_mcfg2(MIRROR0_REGS_t *lscan_mirror,
                       uint32_t galvor_dis, uint32_t galvon_dis, uint32_t galvop_dis, uint32_t galvor_idle,
                       uint32_t galvon_idle, uint32_t galvop_idle, uint32_t lock_pol, uint32_t recov,
                       uint32_t max_adj, uint32_t min_adj)
{
   uint32_t temp_reg = 0;

   temp_reg = MIRROR0_MCFG2_GALVOR_DIS_REPLACE_VAL(lscan_mirror->mcfg2, galvor_dis); 
   temp_reg = MIRROR0_MCFG2_GALVON_DIS_REPLACE_VAL(temp_reg, galvon_dis); 
   temp_reg = MIRROR0_MCFG2_GALVOP_DIS_REPLACE_VAL(temp_reg, galvop_dis); 
   temp_reg = MIRROR0_MCFG2_GALVOR_IDLE_REPLACE_VAL(temp_reg, galvor_idle);
   temp_reg = MIRROR0_MCFG2_GALVON_IDLE_REPLACE_VAL(temp_reg, galvon_idle);
   temp_reg = MIRROR0_MCFG2_GALVOP_IDLE_REPLACE_VAL(temp_reg, galvop_idle);
   temp_reg = MIRROR0_MCFG2_DEC_POL_REPLACE_VAL(temp_reg, 0);
   temp_reg = MIRROR0_MCFG2_ACC_POL_REPLACE_VAL(temp_reg, 0);
   temp_reg = MIRROR0_MCFG2_LOCK_POL_REPLACE_VAL(temp_reg, lock_pol);  
   temp_reg = MIRROR0_MCFG2_RECOV_REPLACE_VAL(temp_reg, recov);     
   temp_reg = MIRROR0_MCFG2_MAX_ADJ_REPLACE_VAL(temp_reg, max_adj);
   lscan_mirror->mcfg2 = MIRROR0_MCFG2_MIN_ADJ_REPLACE_VAL(temp_reg, min_adj);   
}

/**
 * Inits the mirror[n] galv_sync reg
 * 
 */

void init_mirror_galv_sync(MIRROR0_REGS_t *lscan_mirror,
                           uint32_t enable0, uint32_t set0,
                           uint32_t enable1, uint32_t set1)
{
   uint32_t temp_reg;

   temp_reg = MIRROR0_SYNC_GALVO0_ENABLE_REPLACE_VAL(lscan_mirror->sync_galvo0, enable0);
   lscan_mirror->sync_galvo0 = MIRROR0_SYNC_GALVO0_SET_REPLACE_VAL(temp_reg, set0);

   temp_reg = MIRROR0_SYNC_GALVO1_ENABLE_REPLACE_VAL(lscan_mirror->sync_galvo1, enable1);
   lscan_mirror->sync_galvo1 = MIRROR0_SYNC_GALVO1_SET_REPLACE_VAL(temp_reg, set1);
}

/**
 * Inits the mirror[n] galv_wfg reg
 * 
 */

void init_mirror_galv_wfg(MIRROR0_REGS_t *lscan_mirror,
                           uint32_t enable0, uint32_t set0,
                           uint32_t enable1, uint32_t set1)
{
   uint32_t temp_reg;

   temp_reg = MIRROR0_SYNC_WFG0_ENABLE_REPLACE_VAL(lscan_mirror->sync_wfg0, enable0);
   lscan_mirror->sync_wfg0 = MIRROR0_SYNC_WFG0_SET_REPLACE_VAL(temp_reg, set0);

   temp_reg = MIRROR0_SYNC_WFG1_ENABLE_REPLACE_VAL(lscan_mirror->sync_wfg1, enable1);
   lscan_mirror->sync_wfg1 = MIRROR0_SYNC_WFG1_SET_REPLACE_VAL(temp_reg, set1);
}

/**
 * Inits the mirror[n] galvo_rtol reg
 * 
 */

void init_mirror_galvo_rtol(MIRROR0_REGS_t *lscan_mirror,
                            uint32_t rise0, uint32_t fall0,
                            uint32_t rise1, uint32_t fall1)
{
   lscan_mirror->galvo_rtol_rise0 = rise0; 
   lscan_mirror->galvo_rtol_rise1 = rise1;
   lscan_mirror->galvo_rtol_fall0 = fall0;
   lscan_mirror->galvo_rtol_fall1 = fall1;
}

/**
 * Inits the mirror[n] galvop reg
 * 
 */

void init_mirror_galvop(MIRROR0_REGS_t *lscan_mirror,
                        uint32_t rise0_enable, uint32_t rise0_set, uint32_t fall0_enable, uint32_t fall0_set,
                        uint32_t rise1_enable, uint32_t rise1_set, uint32_t fall1_enable, uint32_t fall1_set)
{
   uint32_t temp_reg;

   temp_reg = MIRROR0_GALVOP_RISE0_SET_REPLACE_VAL(lscan_mirror->galvop_rise0, rise0_set); // Clock
   lscan_mirror->galvop_rise0 = MIRROR0_GALVOP_RISE0_ENABLE_REPLACE_VAL(temp_reg, rise0_enable);

   temp_reg = MIRROR0_GALVOP_FALL0_SET_REPLACE_VAL(lscan_mirror->galvop_fall0, fall0_set);
   lscan_mirror->galvop_fall0 = MIRROR0_GALVOP_FALL0_ENABLE_REPLACE_VAL(temp_reg, fall0_enable);

   temp_reg = MIRROR0_GALVOP_RISE1_SET_REPLACE_VAL(lscan_mirror->galvop_rise1, rise1_set); // Clock
   lscan_mirror->galvop_rise1 = MIRROR0_GALVOP_RISE1_ENABLE_REPLACE_VAL(temp_reg, rise1_enable);

   temp_reg = MIRROR0_GALVOP_FALL1_SET_REPLACE_VAL(lscan_mirror->galvop_fall1, fall1_set);
   lscan_mirror->galvop_fall1 = MIRROR0_GALVOP_FALL1_ENABLE_REPLACE_VAL(temp_reg, fall1_enable);
}

/**
 * Inits the mirror[n] galvon reg
 * 
 */

void init_mirror_galvon(MIRROR0_REGS_t *lscan_mirror,
                        uint32_t rise0_enable, uint32_t rise0_set, uint32_t fall0_enable, uint32_t fall0_set,
                        uint32_t rise1_enable, uint32_t rise1_set, uint32_t fall1_enable, uint32_t fall1_set)
{
   uint32_t temp_reg;

   temp_reg = MIRROR0_GALVON_RISE0_SET_REPLACE_VAL(lscan_mirror->galvon_rise0, rise0_set); // Clock
   lscan_mirror->galvon_rise0 = MIRROR0_GALVON_RISE0_ENABLE_REPLACE_VAL(temp_reg, rise0_enable);

   temp_reg = MIRROR0_GALVON_FALL0_SET_REPLACE_VAL(lscan_mirror->galvon_fall0, fall0_set);
   lscan_mirror->galvon_fall0 = MIRROR0_GALVON_FALL0_ENABLE_REPLACE_VAL(temp_reg, fall0_enable);

   temp_reg = MIRROR0_GALVON_RISE1_SET_REPLACE_VAL(lscan_mirror->galvon_rise1, rise1_set); // Clock
   lscan_mirror->galvon_rise1 = MIRROR0_GALVON_RISE1_ENABLE_REPLACE_VAL(temp_reg, rise1_enable);

   temp_reg = MIRROR0_GALVON_FALL1_SET_REPLACE_VAL(lscan_mirror->galvon_fall1, fall1_set);
   lscan_mirror->galvon_fall1 = MIRROR0_GALVON_FALL1_ENABLE_REPLACE_VAL(temp_reg, fall1_enable);
}

/**
 * Inits the mirror[n] hsync_en0 reg
 * 
 */

void init_mirror_hsync_en0(MIRROR0_REGS_t *lscan_mirror,
                           uint32_t rise0_enable, uint32_t rise0_set, uint32_t fall0_enable, uint32_t fall0_set,
                           uint32_t rise1_enable, uint32_t rise1_set, uint32_t fall1_enable, uint32_t fall1_set)
{
   uint32_t temp_reg;

   temp_reg = MIRROR0_HSYNC_EN0_RISE0_SET_REPLACE_VAL(lscan_mirror->hsync_en0_rise0, rise0_set); // Clock
   lscan_mirror->hsync_en0_rise0 = MIRROR0_HSYNC_EN0_RISE0_ENABLE_REPLACE_VAL(temp_reg, rise0_enable);

   temp_reg = MIRROR0_HSYNC_EN0_FALL0_SET_REPLACE_VAL(lscan_mirror->hsync_en0_fall0, fall0_set);
   lscan_mirror->hsync_en0_fall0 = MIRROR0_HSYNC_EN0_FALL0_ENABLE_REPLACE_VAL(temp_reg, fall0_enable);

   temp_reg = MIRROR0_HSYNC_EN0_RISE1_SET_REPLACE_VAL(lscan_mirror->hsync_en0_rise1, rise1_set); // Clock
   lscan_mirror->hsync_en0_rise1 = MIRROR0_HSYNC_EN0_RISE1_ENABLE_REPLACE_VAL(temp_reg, rise1_enable);

   temp_reg = MIRROR0_HSYNC_EN0_FALL1_SET_REPLACE_VAL(lscan_mirror->hsync_en0_fall1, fall1_set);
   lscan_mirror->hsync_en0_fall1 = MIRROR0_HSYNC_EN0_FALL1_ENABLE_REPLACE_VAL(temp_reg, fall1_enable);
}

/**
 * Inits the mirror[n] hsync_en1 reg
 * 
 */

void init_mirror_hsync_en1(MIRROR0_REGS_t *lscan_mirror,
                           uint32_t rise0_enable, uint32_t rise0_set, uint32_t fall0_enable, uint32_t fall0_set,
                           uint32_t rise1_enable, uint32_t rise1_set, uint32_t fall1_enable, uint32_t fall1_set)
{
   uint32_t temp_reg;

   temp_reg = MIRROR0_HSYNC_EN1_RISE0_SET_REPLACE_VAL(lscan_mirror->hsync_en1_rise0, rise0_set); // Clock
   lscan_mirror->hsync_en1_rise0 = MIRROR0_HSYNC_EN1_RISE0_ENABLE_REPLACE_VAL(temp_reg, rise0_enable);

   temp_reg = MIRROR0_HSYNC_EN1_FALL0_SET_REPLACE_VAL(lscan_mirror->hsync_en1_fall0, fall0_set);
   lscan_mirror->hsync_en1_fall0 = MIRROR0_HSYNC_EN1_FALL0_ENABLE_REPLACE_VAL(temp_reg, fall0_enable);

   temp_reg = MIRROR0_HSYNC_EN1_RISE1_SET_REPLACE_VAL(lscan_mirror->hsync_en1_rise1, rise1_set); // Clock
   lscan_mirror->hsync_en1_rise1 = MIRROR0_HSYNC_EN1_RISE1_ENABLE_REPLACE_VAL(temp_reg, rise1_enable);

   temp_reg = MIRROR0_HSYNC_EN1_FALL1_SET_REPLACE_VAL(lscan_mirror->hsync_en1_fall1, fall1_set);
   lscan_mirror->hsync_en1_fall1 = MIRROR0_HSYNC_EN1_FALL1_ENABLE_REPLACE_VAL(temp_reg, fall1_enable);
}

/**
 * Inits the wfs[n] scfg reg
 * 
 */

void init_wfs_scfg(WFG0_REGS_t *lscan_wfg,
                   uint32_t mode, uint32_t recov, 
                   uint32_t sync_sel, uint32_t galvo_sel, uint32_t hsync_sel, 
                   uint32_t count)
{
   uint32_t temp_reg;

   temp_reg = WFG0_SCFG_MODE_REPLACE_VAL(lscan_wfg->scfg, mode);
   temp_reg = WFG0_SCFG_RECOV_REPLACE_VAL(temp_reg, recov);
   temp_reg = WFG0_SCFG_SYNC_SEL_REPLACE_VAL(temp_reg, sync_sel);
   temp_reg = WFG0_SCFG_GALVO_SEL_REPLACE_VAL(temp_reg, galvo_sel);
   temp_reg = WFG0_SCFG_HSYNC_SEL_REPLACE_VAL(temp_reg, hsync_sel);
   lscan_wfg->scfg = WFG0_SCFG_COUNT_REPLACE_VAL(temp_reg, count);
}

/**
 * Inits the wfs[n] period reg
 * 
 */

void set_wfs_period(volatile uint32_t * period_reg, uint32_t idle, uint32_t period)
{
   uint32_t temp_reg = *period_reg;

   temp_reg = WFG0_PERIOD0_IDLE_REPLACE_VAL(temp_reg, idle);       // RTZ
   temp_reg = WFG0_PERIOD0_PERIOD_REPLACE_VAL(temp_reg, period);     // every cycle
   temp_reg = WFG0_PERIOD0_ACTIVE_REPLACE_VAL(temp_reg, 0);     // Set to default, engineering overkill
   
   *period_reg = temp_reg;
}

/**
 * Inits the wfs[n] action reg
 * 
 */

void set_wfs_action(volatile uint32_t *action_reg, uint32_t action, uint32_t count)
{
   *action_reg = WFG0_ACTION10_ACTION_REPLACE_VAL((*action_reg), action);  
   *action_reg = WFG0_ACTION10_COUNT_REPLACE_VAL((*action_reg), count); 
}


/**
 * following functions are used to prepare laser & mirror for
 * printing
 */

/**
 * Turns on and off Voltage for laser
 * 
 */
#ifndef A2140

void set_wavegen_DAC (dec_laser_t * laser, uint16_t value) 
{
   WAVEGEN_REGS_t *wavegen = laser->wavegen->wavegen;
   DEC_CONFIG_REGS_t *dec_config = dec_laser_get_dec_config_regs_base();
   
   dec_config->oe_misc = DEC_CONFIG_REGS_OE_MISC_DAC_OE_REPLACE_VAL(dec_config->oe_misc, 0x0 );  // disable DAC[x]
   wavegen->WCR = WAVEGEN_WCR_HSYNCSEL_REPLACE_VAL(wavegen->WCR, 0x0 );   // use HSync[0] 
   wavegen->WCR = WAVEGEN_WCR_ENABLE_REPLACE_VAL(wavegen->WCR, 1 ); 
   wavegen->WTR = WAVEGEN_WTR_TIMESTEP_REPLACE_VAL(wavegen->WTR, 100 );
   wavegen->WSLR = WAVEGEN_WSLR_SEGLENGTH_REPLACE_VAL(wavegen->WSLR, 0xFFFF );

   if (value > 1023)
     dbg_printf("Illegal value for initWavegen: %04x",value);
   value &= 0x3FF;

   int i;
   volatile uint32_t *ptr;
   for (i=0, ptr = &wavegen->WGLUT0; i < 32; i++, ptr++)
       *ptr = WAVEGEN_WGLUT0_DUTYCYCLE_REPLACE_VAL(*ptr, value );
   // enable DAC[n]
   dec_config->oe_misc = DEC_CONFIG_REGS_OE_MISC_DAC_OE_REPLACE_VAL(dec_config->oe_misc,( 1 << laser->wavegen->wavegen_num ));  
}
#endif

/**
 * Called when a phase is complete and there is still more
 * phases to be completed before the laser is ready.
 * 
 */
void dec_laser_internal_phase_complete( void *at_phase_complete_data, uint32_t next_phase )
{
    int status;
    dec_laser_t * dec_laser = (dec_laser_t *)  at_phase_complete_data;
    static dec_laser_msg_t    msg;

    DBG_LOG_DEBUG("dec_laser_internal_phase_complete %d, next phase %d \n", dec_laser->spin_state, next_phase);
    dec_laser->next_phase = next_phase; 
    msg.laser  =  dec_laser;
    status = posix_message_send(dec_laser_mq, (char*)&msg, sizeof(dec_laser_msg_t), 0, 0);
    XASSERT(status == 0, status);
}


uint32_t dec_laser_target_period(dec_laser_t *laser)
 {
     MIRROR0_REGS_t *lscan_mirror = laser->mirror->lscan_mirror;
     
     return (lscan_mirror->lockmax + lscan_mirror->lockmin) / 2;;
 }
uint32_t dec_laser_period( dec_laser_t *laser, uint32_t *baseline )

 {
     
     MIRROR0_REGS_t *lscan_mirror = laser->mirror->lscan_mirror;
#if 1
     const uint32_t lock_min = lscan_mirror->lockmin;
     const uint32_t lock_max = lscan_mirror->lockmax;
     const uint32_t ave_base_line = (lock_max + lock_min) >> 1;
     const uint32_t baseline_high = (lock_max + ave_base_line) >> 1;
     const uint32_t baseline_low = (lock_min + ave_base_line) >> 1;

     
#endif
     uint32_t period = MIRROR0_STATUS_PERIOD_MASK_SHIFT(lscan_mirror->status);
     DBG_LOG_DEBUG("Period is %x scfg %x hcfg %x\n",period,laser->wfs->lscan_wfg->scfg,laser->lscan_hsync->hcfg);
#if 1
     if (period > ave_base_line/*/*lscan_mirror->lockmin*/ )
     {
         //*baseline = baseline_low /*lscan_mirror->lockmax*/;
         if (period > baseline_high) 
         {
             *baseline = baseline_low /*lock_min*/;
         }
         else 
         {
             *baseline = ave_base_line;
         }
     }
     else //if (period < baseline_low/*lscan_mirror->lockmin */)
     {
         
         if (period  < baseline_low) 
         {
             *baseline = baseline_high /*lock_max*/;
             
         }
         else
         {
             *baseline = ave_base_line;
         }
     }
     //else
     //{
     //    *baseline = 0;
     //}
#else
     *baseline = ave_base_line;
#endif
     
     return period;
 }

/**
 * This thread manages the phase transisions while starting up
 * the laser.  It can handle multiple lasers at a time.
 * 
 */

static void* DecLaserThreadLoop( void* unused )
{
    dec_laser_t * laser; 
    int err;
    dec_laser_msg_t     incomingMsg;

    // Wait for the system initialization to finish.

    SysWaitForInit();
    while(true)
    {   
        err = posix_wait_for_message(dec_laser_mq, (char*)&incomingMsg, sizeof(dec_laser_msg_t), POSIX_WAIT_FOREVER);
        XASSERT(err == 0, err);
        laser = incomingMsg.laser; 
        //dbg_printf("Got message %d %x %d \n", err, incomingMsg.laser, laser->next_phase); 
        if (!dec_laser_spin_phase_exec(laser, laser->next_phase))
        {
            laser->spin_state = IDLE; 
        }         
    }
    return 0;
} 


/**
 * Continue to wait for mirror to be synced.
 */
bool dec_continue_spinning_mirror(dec_laser_t * laser)
{
    laser->spin_state = SPINNING_MIRROR;
    laser->laser_timer = delay_msec_timer_non_blocking(10, dec_laser_timer_complete_callback_func, laser, false);
    ASSERT(laser->laser_timer != NULL);
    //dbg_printf("Laser Timer %x \n", laser->laser_timer);
    return(true);
}

#ifdef HAVE_DEC_SENSOR
void dec_enable_vdo_top_sensor(dec_laser_t * laser, dec_sensor_t *sensor, bool on_notoff)
{
    DEC_CONFIG_REGS_REGS_t *p_dec_config_reg = dec_laser_get_dec_config_regs_base();
    HSYNC_REGS_t *p_dec_lscan_hsync_reg = laser->lscan_hsync;

    if (on_notoff)
    {
        p_dec_config_reg->top_sync0 = 1 << dec_sensor_pin_number(sensor);
        p_dec_config_reg->top_sync1 = 1 << dec_sensor_pin_number(sensor);                               
        p_dec_lscan_hsync_reg->hcfg =  HSYNC_HCFG_ENABLE_REPLACE_VAL(p_dec_lscan_hsync_reg->hcfg, 0xf);  
    }
    else
    {
        p_dec_config_reg->top_sync0 = 0;
        p_dec_config_reg->top_sync1 = 0;
        p_dec_lscan_hsync_reg->hcfg =  HSYNC_HCFG_ENABLE_REPLACE_VAL(p_dec_lscan_hsync_reg->hcfg, 0); 
    }
}
#endif

/** Manually fire top sync if needed. */
void dec_laser_set_top_sync( dec_laser_t *laser, bool set )
{
    DEC_CONFIG_REGS_REGS_t *p_dec_config_reg = dec_laser_get_dec_config_regs_base();
    HSYNC_REGS_t *p_dec_lscan_hsync_reg = laser->lscan_hsync;
    //HVSYNC_REGS_t *hvsync_reg = (HVSYNC_REGS_t *) MVDO_HVSYNC_BASE;
    if ( set ) 
    {
        
        //DBG_LOG_DEBUG("top laser hvsync HFSR0 %x HLOR0 %x\n", hvsync_reg->HFSR0, hvsync_reg->HLOR0);
        //if (hvsync_reg->HFSR0 == 0x602) 
        //{
        //    DBG_LOG_ERR("DANGER DANGER reversing HvSyncs\n");
        //    hvsync_reg->OUT_SEL0 = HVSYNC_OUT_SEL1_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL0, 4);
        //    hvsync_reg->OUT_SEL1 = HVSYNC_OUT_SEL1_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL1, 0);
        //}
#if defined (ASIC_granite) || (defined(ASIC_gemstone) && (ASIC_REV < ASIC_REV_B0))
        p_dec_config_reg->top_sync0 = DEC_CONFIG_REGS_TOP_SYNC0_M_REPLACE_VAL(p_dec_config_reg->top_sync0, 1 );
        p_dec_config_reg->top_sync1 = DEC_CONFIG_REGS_TOP_SYNC1_M_REPLACE_VAL(p_dec_config_reg->top_sync1, 1 );
        p_dec_config_reg->top_sync0 = DEC_CONFIG_REGS_TOP_SYNC0_M_REPLACE_VAL(p_dec_config_reg->top_sync0, 0 );
        p_dec_config_reg->top_sync1 = DEC_CONFIG_REGS_TOP_SYNC1_M_REPLACE_VAL(p_dec_config_reg->top_sync1, 0 );
#else 
        p_dec_config_reg->man_tb_sync = DEC_CONFIG_REGS_MAN_TB_SYNC_TOP_REPLACE_VAL(p_dec_config_reg->man_tb_sync, 0x3);
#endif
        p_dec_lscan_hsync_reg->hcfg =  HSYNC_HCFG_ENABLE_REPLACE_VAL(p_dec_lscan_hsync_reg->hcfg, 0xf); 
        DBG_LOG_ERR("laser Top Sync fired \n");

#if 0
/// stefan foo:  force laser all on.
        laser->lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(laser->lscan_wfs->data_sel,0xFF);  // Force laser 100% on   
        laser->lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(laser->lscan_wfs->out_sel,   0xFF);  // latch 
#endif

    }
    else 
    {
        p_dec_lscan_hsync_reg->hcfg =  HSYNC_HCFG_ENABLE_REPLACE_VAL(p_dec_lscan_hsync_reg->hcfg, 0);
#if 0
/// stefan foo:  force laser all on.
        laser->lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(laser->lscan_wfs->data_sel,0x0);  // Force laser 0% OFF
        laser->lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(laser->lscan_wfs->out_sel,   0xFF);  // latch 
#endif
    }
}
/**
 * called when either the laser has spun up to speed or an error
 * has occurred and it is aborting.
 * 
 * It will clean up and unblock the caller.
 * 
 */
void dec_laser_spin_complete( dec_laser_t *laser, dec_laser_state_t state  )
{
    DBG_LOG_DEBUG("dec_laser_spin_complete %d, next phase %d \n", laser->spin_state, state);

    laser->spin_state = state;
    if (laser->laser_WD_timer) 
    {
        laser->laser_WD_timer = delay_msec_timer_cancel(laser->laser_WD_timer);
    }

    sem_post(&laser->complete_sem);

}

/**
 * WD callback for the the start laser call.
 * 
 * It cleans up before it unblocks.
 */
static void start_laser_wd(void * callbackData)
{
    dec_laser_t *laser = (dec_laser_t *) callbackData;

    laser->laser_WD_timer = NULL;
    laser->spin_state = IDLE;
    sem_post(&laser->complete_sem);
    DBG_LOG_ERR("start_laser_WD: postsem\n");

}

/**
 * External interfaces
 * 
 */

/**
 * Estimated time it will take to spin up laser
 * 
 */

uint32_t dec_laser_eta(dec_laser_t * laser)
{
    return laser->mirror->min_spin_time;  
}

/**
 * Starts the spinnig process
 * 
 */

error_type_t dec_laser_start(dec_laser_t *laser)
{
    DBG_ASSERT(laser != NULL);

    if (laser->spin_state != IDLE) // Someone tries to start laser while it is running
    {
        return DEC_LASER_START_ERROR_START_WHILE_STARTED;
    }
    while (sem_trywait(&laser->complete_sem) == 0) //cleans out any extra sem puts
    {
        DBG_LOG_ERR("dec_laser_start: sem_trywait dec\n");
    }
     
    if (!dec_laser_spin_phase_exec(laser, SPINNING_MIRROR))
    {
        return DEC_LASER_START_ERROR_STARTING_UNKNOWN_MOTOR_ERROR;  
    }
    
    laser->laser_WD_timer = delay_msec_timer_non_blocking(laser->mirror->wd_spin_time, start_laser_wd, laser, false);
    return OK;
}

/**
 * Blocks calling thread until either the WD timer pops or the
 * spin up completes
 * 
 */
error_type_t dec_laser_wait(dec_laser_t *laser)
{
    DBG_ASSERT(laser != NULL);

    sem_wait(&laser->complete_sem);
    DBG_LOG_ERR("dec_laser_wait: sem_wait dec\n");

    DBG_ASSERT (laser->laser_WD_timer == NULL); // timer must self clear.
    
    // timer and wd could fire at the same time, so clear the extra if needed.
    while (sem_trywait(&laser->complete_sem) == 0)
    {
        DBG_LOG_ERR("dec_laser_wait: sem_trywait dec\n");
    }

    if (laser->spin_state ==  READY_FOR_TOP) //< Laser's ready
    {
        return (OK);
    }
    else 
    {
        // WD popped, either WD too short or HW issue. .... extern doc why did i die....
        return ( DEC_LASER_WAIT_ERROR_WATCHDOG_EXPIRED );
    }

}


/**
 * device init routine
 */
hips_pll_device_t* hips_handles[NUM_VIDEO_CHAN] = {0};
void dec_laser_init(void)
{   
    uint32_t chan;
    error_type_t status;
    dec_laser_t * dec_laser;

    HIPWM0_REGS_t *svdo0_hipwm = NULL;
    HIPWM0_REGS_t *svdo1_hipwm = NULL; //REVISIT Single beam vs dual beam
    dec_laser_local_init();

    // Enable all the HIPS channels; NOTE that we currently never close/disable the channels.
    for (chan=0; chan<NUM_VIDEO_CHAN; chan++)
    {
        status = hips_pll_open(&hips_handles[chan], chan);
        ASSERT(status == OK);
    }

    dec_laser = dec_laser_by_index( 0 );

    svdo0_hipwm = dec_laser_open_hipwm_regs(0);
    svdo1_hipwm = dec_laser_open_hipwm_regs(1);

    // Setting to 1 allow force laser on waveform to drive video.  Enable based on configuration of the engine.
    svdo0_hipwm->HCR = HIPWM0_HCR_CFGDECFORCELASERONEN_REPLACE_VAL(svdo0_hipwm->HCR, dec_laser->enable_force_laser);
    svdo1_hipwm->HCR = HIPWM0_HCR_CFGDECFORCELASERONEN_REPLACE_VAL(svdo1_hipwm->HCR, dec_laser->enable_force_laser);

    // Setting these bits on enables gating of the video signal with the VDOEnable waveform.
    // Enable based on configuration of the engine.
    svdo0_hipwm->HCR = HIPWM0_HCR_CFGDECVIDEODATAEN_REPLACE_VAL(svdo0_hipwm->HCR, dec_laser->enable_video_gating);
    svdo1_hipwm->HCR = HIPWM0_HCR_CFGDECVIDEODATAEN_REPLACE_VAL(svdo1_hipwm->HCR, dec_laser->enable_video_gating);

    // Enable is done by video during video init; shouldn't need to do here -jrs
//-jrs    svdo0_hipwm->HCR = HIPWM0_HCR_ENABLE_REPLACE_VAL(svdo0_hipwm->HCR, 1);
    dec_laser_close_hipwm_regs(0);
    dec_laser_close_hipwm_regs(1);

#if defined (ASIC_granite) || (defined(ASIC_gemstone) && (ASIC_REV < ASIC_REV_B0))
    {
        // on ASSP's without auto-reset manual top signal make sure the manual top signal is not active
        DEC_CONFIG_REGS_REGS_t *p_dec_config_reg = dec_laser_get_dec_config_regs_base();
        p_dec_config_reg->top_sync0 = DEC_CONFIG_REGS_TOP_SYNC0_M_REPLACE_VAL(p_dec_config_reg->top_sync0, 0 );
        p_dec_config_reg->top_sync1 = DEC_CONFIG_REGS_TOP_SYNC1_M_REPLACE_VAL(p_dec_config_reg->top_sync1, 0 );
        p_dec_config_reg->top_sync2 = DEC_CONFIG_REGS_TOP_SYNC1_M_REPLACE_VAL(p_dec_config_reg->top_sync2, 0 );
        p_dec_config_reg->top_sync3 = DEC_CONFIG_REGS_TOP_SYNC1_M_REPLACE_VAL(p_dec_config_reg->top_sync3, 0 );
    }
#endif


    posix_create_message_queue(&dec_laser_mq, "/dec_laser_mq", QUEUE_SIZE, sizeof(dec_laser_msg_t));
    
    sem_init(&dec_laser->complete_sem, 0, 0);    

#ifdef HAVE_CMD
    REL_ASSERT( CMD_OK == cmd_register( "laser",
                                        NULL,
                                        NULL,
                                        NULL,
                                        dec_laser_dbcallback ) );
#endif
    
    
    posix_create_thread(&dec_laser_mgr_thread, DecLaserThreadLoop, dec_laser, "tDecLaserMgrRcvMsg", 
                        dec_laser_thread_stack, POSIX_MIN_STACK_SIZE, POSIX_THR_PRI_DRIVER);
}

/**
 * testing dec_laser
 * 
 * 
 */



#ifdef HAVE_CMD
/// serial debug testing 
///

#define MAX_DEC_LASER_TESTS 2
int dec_laser_dbcallback( int argc, char *argv[] )
{
    int num; 
    const char *str;
    int i;
    int test;
    uint32_t time_ms;
    error_type_t success;
    uint32_t begin_time, end_time;
    dec_laser_t * laser;
    MIRROR0_REGS_t *lscan_mirror;

    
    if (argc < 3 )         /* not enough parameters */
    {
        dbg_printf("debug:dec_laser laser_number_0->n (0) stop /  (1) automagic init (2) cancel test msec\n");

        for (i = 0, str = dec_laser_name_by_index(i); 
             str ; 
             i++, str = dec_laser_name_by_index(i) )
        {
            laser = dec_laser_by_index(i);
            lscan_mirror = laser->mirror->lscan_mirror;
            dbg_printf(" dec_laser [%d] %s:state %d, lock det %d, locked %d status %x\n", i, str, 
                                                                 laser->spin_state,
                                                                (MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status) == 1)?1:0,
                                                                (MIRROR0_STATUS_LOCKED_MASK_SHIFT(lscan_mirror->status) == 1)?1:0,
                                                                lscan_mirror->status);

        }
        return 0;
    }
    

    num = atoi(argv[1]);
    test = atoi(argv[2]);
     
    if (test > MAX_DEC_LASER_TESTS) 
    {
        dbg_printf("debug:dec_laser laser_number_0->n (0) stop /  (1) automagic init (2) cancel test msec\n");
    }
    else 
    {
        // sanity check.

        test = test > 0 && test <= MAX_DEC_LASER_TESTS ? test : 0;

        laser = dec_laser_by_index(num);

        if (!laser) 
        {
            dbg_printf("debug:dec_laser %d, %x \n", num, laser);
            return 0;
        }
        if (test == 0) 
        {
            dec_stop_laser_mirror(laser);
            dbg_printf("debug:dec_laser dec_laser stopped \n");
            return 0;
        }

        if (test == 1 ) 
        {
            begin_time = timer_get_time_usec()/100;
            time_ms = dec_laser_eta(laser);
            success = dec_laser_start(laser);
            if (success)
            {
                dbg_printf("dec_laser_start failed \n");
            }
            else
                success = dec_laser_wait(laser);
            end_time = timer_get_time_usec()/100;
            dbg_printf("debug:dec_laser Mirror init %s, time %d 100 uSec, expected time is %d \n",
                       (!success)?"succeeded":"failed",
                       end_time - begin_time,
                       time_ms * 10);
        }
        if (test == 2 ) 
        {
            if (argc < 4) 
            {
                dbg_printf("debug:dec_laser laser_number_0->n  (2) cancel test msec\n");
                return 0;
            }
            int cancel_time  = atoi(argv[3]);
            dbg_printf("cancel time %d \n",cancel_time);
            begin_time = timer_get_time_usec()/100;
            time_ms = dec_laser_eta(laser);
            success = dec_laser_start(laser);
            if (success)
            {
                dbg_printf("dec_laser_start failed \n");
            }
            else
            {
                delay_msec_timer(cancel_time);
                dec_stop_laser_mirror(laser);
                success = dec_laser_wait(laser);
            }
                
            end_time = timer_get_time_usec()/100;
            dbg_printf("debug:dec_laser Mirror init %s, time %d 100 uSec, expected time is %d \n",
                       (!success)?"succeeded":"failed",
                       end_time - begin_time,
                       time_ms * 10);
        }
        else 
        {
        
            /**
             * doesn't work anymore
             */
        }
    }
    return 0;
}
#endif

bool spinning_mirror_lock ( dec_laser_t * dec_laser )
{
	return dec_laser->lock_det;
}
void mirror_lock_clear ( dec_laser_t * dec_laser )
{
	dec_laser->lock_det = false;
}
