/*
 * ======================================================================
 * Copyright (c) 2010, 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

/// \file dec_laser_4729.c
#include "asic.h"
#include "regAddrs.h"
#include "uio_lib_api.h"
#include "dec_laser.h"
#include "dec_laser_priv.h"
#include "dcmotor.h"
#include "gpio_api.h"

#include <stdint.h>
#include <string.h>
#include "lassert.h"
static dcmotor_t * laser_motor;
static dcmotor_speed_t * off_speed;
static dcmotor_speed_t * half_speed;
static dcmotor_speed_t * full_speed;

static dec_laser_t my_lasers[1];
const const char * dec_laser_key_table[] = 
{
    "laser0", 
    0
};
static dec_laser_t *mylasers[] = 
{
    &my_lasers[0],
    0
};  // last must be null, size is one more than number of objects.

KEY_TABLE_DEFINE_PRIVATE_2( dec_laser, dec_laser_t, dec_laser_key_table, mylasers );


// no need for key table interface since these aren't public.
static dec_mirror_t my_mirrors[1];
const const char * dec_mirror_key_table[] = 
{
    "mirror0", 
    0
};
static dec_mirror_t *mymirrors[] = {
    &my_mirrors[0],
    0
};  // last must be null.

//yes there are 4 wfs, but we are only using 1
#define MAX_WFS 1
static dec_wfs_t my_wfss[MAX_WFS];
const const char * dec_wfs_key_table[] = 
{
    "wfs0", 
    0
};

static dec_wfs_t *mywfss[] = 
{
    &my_wfss[0],
    0
};  // last must be null.


//yes there are 4 wavegen, but we are only using 1
#define MAX_WAVEGEN 1
static dec_wavegen_t my_wavegens[MAX_WAVEGEN];
static dec_wavegen_t *mywavegens[] = 
{
    &my_wavegens[0],
    0
};  // last must be null.

static gpio_handle_t *laser_pwr_enable;
static gpio_handle_t *laser_enable;


/**
 * 
 */

void init_Lscan (dec_laser_t * laser) 
{
   DEC_CONFIG_REGS_REGS_t *dec_config = dec_laser_get_dec_config_regs_base();
   MIRROR0_REGS_t *lscan_mirror = laser->mirror->lscan_mirror;
   WFS_REGS_t *lscan_wfs = laser->lscan_wfs;
   HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;
   WFG0_REGS_t *lscan_wfg = laser->wfs->lscan_wfg;

   lscan_mirror->mcfg1 = MIRROR0_MCFG1_MODE_REPLACE_VAL(lscan_mirror->mcfg1, 0x0 );//reset mirror counts
   lscan_mirror->mcfg1 = MIRROR0_MCFG1_MODE_REPLACE_VAL(lscan_mirror->mcfg1, 0x1 );//restore open loop mode

   ///disable the mirror
   init_mirror_mcfg2(lscan_mirror,
                          0x3, // disable GalvoRtoL
                          0x1, // disable nStart
                          0x1, // disable Clk
                          0x0, // idle GalvoRtoL=0
                          0x0, // Start=0
                          0x0, // idle Clk = 0
                          0x0, // nReady_LSU
                          0x0, // ignored open-loop
                          0x0, // ignored open-loop
                          0x0  // ignored open-loop
                          );                                                                                           
   /// HSYNC configuration
   init_lscan_hsync_hcfg(lscan_hsync,
                         0,   // use internal GalvoRtoL
                         0x0, // disable WFG[3:0]
                         0x2, // pulse on falling HSync
                         0x0, // no qual (for now)
                         0x0, // no qual (for now)
                         0x0, // active high HSyncIns
                         0x0, // active high HSyncOuts
                         0xF);// enable all HSyncOuts

    set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action28,
                  WAVEFORM_ACTION_HIGH, laser->lperiod /2
                  );

   /// HSync filter configuration
   init_lscan_hsync_filter(lscan_hsync,
                           0x1,0x1, //fall hi & lo
                           0x1,0x1, //rise hi & lo
                           0xf);   //hfilt threshold
   

   lscan_wfg->scfg = WFG0_SCFG_RECOV_REPLACE_VAL(lscan_wfg->scfg, 0x0 );                   ///< allow missing HSYNC

   /// data override
   lscan_wfs->data_sel = WFS_DATA_SEL_ENABLE_REPLACE_VAL(lscan_wfs->data_sel, 0x00 );  // noop
   lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel, 0x00 );//FORCE_LASER off. 
   lscan_wfs->data_sel = WFS_DATA_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->data_sel, 0x006 );//LD_ON forced off.

   /// invert ForceLaserOn & LD 1 & 2
   lscan_wfs->wave_pol = WFS_WAVE_POL_ENABLE_INV_REPLACE_VAL(lscan_wfs->wave_pol, 0x00 );
   lscan_wfs->wave_pol = WFS_WAVE_POL_FORCE_INV_REPLACE_VAL(lscan_wfs->wave_pol, 0x00 );
   lscan_wfs->wave_pol = WFS_WAVE_POL_LD_CTRL_INV_REPLACE_VAL(lscan_wfs->wave_pol,0x000 );
   /// waveform select (LD_CTRl[1] is fixed output)
   lscan_wfs->out_sel = WFS_OUT_SEL_ENABLE_REPLACE_VAL(lscan_wfs->out_sel, 0x00 );   // waveform
   lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel, 0x03 ); // override until ready
   lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel, 0x002 ); // manual on ld_ON wave on SH
   /**
    * LD_CTRL[11:0] are up to 12 pin output of the lscan block.
    * They are either constant or some OR function of the waveform
    * generator blocks.  It is one of the main outputs of the lscan
    * blocks.
    */
   // old wfg 
   // LD_CTRL[0] = SH (WFG0[0])
   // lscan_wfs->wave_ld_sel0 = WFS_WAVE_LD_SEL0_WFG0_REPLACE_VAL(0x0, 0x1 );
   //
   // LD_CTRL[1] = LDON (data)
   // lscan_wfs->wave_ld_sel1 = WFS_WAVE_LD_SEL1_WFG0_REPLACE_VAL(0x0, 0x2 );


   /// LD_CTRL[0] = SH (WFG0[06] | WFG0[05])
   //lscan_wfs->wave_ld_sel0 = WFS_WAVE_LD_SEL0_WFG0_REPLACE_VAL(0x0, 0x60 );  // wfg0[6] | wfg0[5] --> SH
   /// LD_CTRL[1] = LDON (data)
   lscan_wfs->wave_ld_sel1 = WFS_WAVE_LD_SEL1_WFG0_REPLACE_VAL(0x0, 0x1 );  // wfg0[5] --> LDON
   lscan_wfs->wave_ld_sel2 = WFS_WAVE_LD_SEL2_WFG0_REPLACE_VAL(0x0, 0x2 );  // wfg0[5] --> LDON
   dec_config->oe_misc = DEC_CONFIG_REGS_OE_MISC_LD_CTRL_OE_REPLACE_VAL(dec_config->oe_misc, 0x6 ); // enable LD_CTRL[1:0]

   /// LD_CTRL[11:3] unused
   lscan_wfs->wave_ld_sel3 = 0;
   lscan_wfs->wave_ld_sel4 = 0;
   lscan_wfs->wave_ld_sel5 = 0;
   lscan_wfs->wave_ld_sel6 = 0;
   lscan_wfs->wave_ld_sel7 = 0;
   lscan_wfs->wave_ld_sel8 = 0;
   lscan_wfs->wave_ld_sel9 = 0;
   lscan_wfs->wave_ld_sel10 = 0;
   lscan_wfs->wave_ld_sel11 = 0;

   /**
    * ForceLaserOn[8:0] are up to 8 pin output of the lscan block.
    * They are either constant or some OR function of the waveform
    * generator blocks.  It is one of the main outputs of the lscan
    * blocks.
    */
   /// ForceLaserOn = !(WFG0[3] | WFG[2])
   //lscan_wfs->wave_frc_sel0 = WFS_WAVE_FRC_SEL0_WFG0_REPLACE_VAL(0x0, 0x84 );
   lscan_wfs->wave_frc_sel0 = WFS_WAVE_FRC_SEL0_WFG0_REPLACE_VAL(0x0, 0x24 );
   lscan_wfs->wave_frc_sel1 = WFS_WAVE_FRC_SEL1_WFG0_REPLACE_VAL(0x0, 0x08 );
   // stefan foo hack a stripe | in WFG[7]
   // lscan_wfs->wave_frc_sel0 = WFS_WAVE_FRC_SEL0_WFG0_REPLACE_VAL(0x0, 0x8C );
   // lscan_wfs->wave_frc_sel1 = 0;
   lscan_wfs->wave_frc_sel2 = 0;
   lscan_wfs->wave_frc_sel3 = 0;
   lscan_wfs->wave_frc_sel4 = 0;
   lscan_wfs->wave_frc_sel5 = 0;
   lscan_wfs->wave_frc_sel6 = 0;
   lscan_wfs->wave_frc_sel7 = 0;

   /**
    * EnableVDO[8:0] are up to 8 pin output of the lscan block.
    * They are either constant or some OR function of the waveform
    * generator blocks.  It is one of the main outputs of the lscan
    * blocks.
    */

   /// EnableVDO = WFG0[4]
   // lscan_wfs->wave_en_sel0 = WFS_WAVE_EN_SEL0_WFG0_REPLACE_VAL(0x0, 0x10);
   lscan_wfs->wave_en_sel0 = 0;  ///< initially disabled
   lscan_wfs->wave_en_sel1 = 0;
   lscan_wfs->wave_en_sel2 = 0;
   lscan_wfs->wave_en_sel3 = 0;
   lscan_wfs->wave_en_sel4 = 0;
   lscan_wfs->wave_en_sel5 = 0;
   lscan_wfs->wave_en_sel6 = 0;
   lscan_wfs->wave_en_sel7 = 0;
}
static void spinning_mirror_return_func_4729 ( dec_laser_t * dec_laser )
{ 
    MIRROR0_REGS_t *lscan_mirror = dec_laser->mirror->lscan_mirror;
    static uint32_t lock_count = 0;
#if 0
    dbg_printf("mirror isr fired, %x %d %d\n",lscan_mirror->status,
               MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status),
               MIRROR0_STATUS_LOCKED_MASK_SHIFT(lscan_mirror->status)
               );
#endif   
    
    ASSERT(dec_laser->spin_state == SPINNING_MIRROR);
    /**
     * Turn enable laser after lock det
     * Only do it once
     * Then continue waiting for both lock det & locked
     * locked can only occur after laser has been enabled.
     */
    if (dec_laser->lock_det == false && OK != dcmotor_wait(laser_motor, 0)   ) 
    {
        dec_laser_internal_phase_complete(dec_laser, CONTINUE_SPINNING_MIRROR );
        return;
    }
#if 0
    dbg_printf("mirror isr fired, %x %d %d\n",lscan_mirror->status,
               MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status),
               MIRROR0_STATUS_LOCKED_MASK_SHIFT(lscan_mirror->status)
               );
#endif
    if ((dec_laser->lock_det == false) /*&& (MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status) == 1)*/)
    {
        dec_laser->lock_det = true;
        lock_count=0;
        dec_laser_internal_phase_complete(dec_laser, ENABLE_LASER );
        return;
    }

    
    if (
        /*(MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status) == 1)*/
#if ASIC_REV >= ASIC_REV_Z3
        /*&& */(MIRROR0_STATUS_LOCKED_MASK_SHIFT(lscan_mirror->status) == 1)
#endif
        ) // If this succeeds then we are finished with the phase and can go to the next one.  
              // Otherwise, it waits for the next ISR to fire
    {
        //dbg_printf("Laser spun up\n");
        if (lock_count == 5)
        {
            dec_laser_internal_phase_complete(dec_laser, SYNCING_HSYNC );
            lock_count=0;
        }
        else
        {
            lock_count++;
            dec_laser_internal_phase_complete(dec_laser, CONTINUE_SPINNING_MIRROR );
        }
    }
    else //Wait and try again
    {
        //dbg_printf("Wait and try again\n");
        lock_count=0;
        dec_laser_internal_phase_complete(dec_laser, CONTINUE_SPINNING_MIRROR );
    }
}
void dec_laser_timer_complete_callback_func(void *data)  

{
    dec_laser_t * dec_laser = (dec_laser_t *) data;
    HSYNC_REGS_t *lscan_hsync = dec_laser->lscan_hsync;
    //WFS_REGS_t *lscan_wfs = dec_laser->lscan_wfs;

    dec_laser->laser_timer = NULL;
#if 0
    MIRROR0_REGS_t *lscan_mirror = dec_laser->mirror->lscan_mirror;
    DBG_LOG_ERR("Timer cb fn, %d %x %d %d\n",dec_laser->spin_state,
               lscan_mirror->status,
               MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status),
               MIRROR0_STATUS_LOCKED_MASK_SHIFT(lscan_mirror->status)
               );
#endif
    switch (dec_laser->spin_state) 
    {
    case IDLE:
        dec_laser_spin_complete(dec_laser, IDLE );
        break;
    case SPINNING_MIRROR:
        spinning_mirror_return_func_4729 ( dec_laser );
        break;
    case ENABLE_LASER:
        lscan_hsync->hcfg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(lscan_hsync->hcfg, 0x1 );         ///< filter/qual with WFG HSync_en
        dec_laser_internal_phase_complete(dec_laser, ENABLE_LASER2 );
        break;
    case ENABLE_LASER2:
        dec_laser_internal_phase_complete(dec_laser, CONTINUE_SPINNING_MIRROR );
        break;
    case SYNCING_HSYNC:
        dec_laser_internal_phase_complete(dec_laser, SYNCING_FILTER );
        break;
    case SYNCING_FILTER:
        //dec_laser->lscan_hsync->hcfg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(dec_laser->lscan_hsync->hcfg, 0x0 );      ///NO filter/qual with WFG HSync_en
        //lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel, 0x00 );       ///< enable ForceLaserOn wave
        dec_laser_spin_complete(dec_laser, READY_FOR_TOP );
        break;
    default:
        DBG_ASSERT(0 == "unknown laser state");
        dec_laser_spin_complete(dec_laser, IDLE );
    }
}

static bool dec_start_laser_mirror_4729(dec_laser_t * laser)
{
    MIRROR0_REGS_t *lscan_mirror;

    DBG_ASSERT(laser->spin_state == IDLE); 

    /// make sure timer is NULL'ed out
    laser->laser_timer = NULL;
    laser->lock_det = false;

    init_Lscan(laser);

    /// set laser diode power
   
    set_wavegen_DAC(laser, laser->wavegen->lut_value_on);
    if ( gpio_write( laser_pwr_enable, 0)) 
    {
        ASSERT(false);//if this happens, gpio is completely screwed up
    }
    if ( gpio_write( laser_enable, 0)) 
    {
        ASSERT(false);//if this happens, gpio is completely screwed up
    }

    lscan_mirror = laser->mirror->lscan_mirror;

    /// enable polygon mirror rotation
    lscan_mirror->mcfg2 = MIRROR0_MCFG2_GALVOR_DIS_REPLACE_VAL(
        lscan_mirror->mcfg2, 0x0 ); ///< enable GalvoRtoL
    lscan_mirror->mcfg2 = MIRROR0_MCFG2_GALVOP_DIS_REPLACE_VAL(
        lscan_mirror->mcfg2, 0x0 ); ///< enable Clk
    lscan_mirror->mcfg2 = MIRROR0_MCFG2_GALVON_IDLE_REPLACE_VAL(
        lscan_mirror->mcfg2, 0x1 ); ///< Start=1

    dcmotor_run(laser_motor, half_speed);

    laser->spin_state = SPINNING_MIRROR;// always change state before ISR.
   
    laser->laser_timer = delay_msec_timer_non_blocking(500, dec_laser_timer_complete_callback_func, laser, false);
    return ( true );
}
static bool dec_enable_laser_mirror_4729(dec_laser_t * laser)
{
#if 1
    WFG0_REGS_t *lscan_wfg = laser->wfs->lscan_wfg;
    WFS_REGS_t *lscan_wfs = laser->lscan_wfs;
    //HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;
#endif
    DBG_ASSERT (laser->spin_state == SPINNING_MIRROR); 

    //dbg_printf("Laser Enabled\n");
    DBG_LOG_ERR("enabling laser\n");
#if 1
    //lscan_hsync->hcfg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(lscan_hsync->hcfg, 0x1 );         ///< filter/qual with WFG HSync_en
    lscan_wfg->scfg = WFG0_SCFG_COUNT_REPLACE_VAL(lscan_wfg->scfg, laser->lperiod + laser->lperiod / 2 );
    lscan_wfs->data_sel = WFS_DATA_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->data_sel, 0x004 );  ///< LDStartup (enable laser)
    lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel, 0x006);
    lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel, 0x01 );  ///< uses FRC LASER ON 
    lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel, 0x03);
#endif
    laser->spin_state = ENABLE_LASER; // always change state before enabling timer.
    DBG_LOG_DEBUG("done enabling laser\n");
    laser->laser_timer = delay_msec_timer_non_blocking(5, dec_laser_timer_complete_callback_func, laser, false);
    
    return ( true );
}
static bool dec_enable2_laser_mirror_4729(dec_laser_t * laser)
{
    WFS_REGS_t *lscan_wfs = laser->lscan_wfs;
    HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;

    DBG_ASSERT (laser->spin_state == ENABLE_LASER); 
    DBG_LOG_ERR("starting laser motor\n");
    lscan_hsync->hcfg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(lscan_hsync->hcfg, 0x1 );         ///< filter/qual with WFG HSync_en
    lscan_hsync->hcfg = HSYNC_HCFG_HSYNC_QUAL_REPLACE_VAL(lscan_hsync->hcfg, 0x1 );         ///< filter/qual with WFG HSync_en
    lscan_hsync->hcfg = HSYNC_HCFG_WFG_ENABLE_REPLACE_VAL(lscan_hsync->hcfg, 0xf );
    lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel, 0x00 );  ///< uses FRC LASER ON 
    lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel, 0x02);
    dcmotor_run(laser_motor, full_speed);
    
 
    laser->spin_state = ENABLE_LASER2; // always change state before enabling timer.
    DBG_LOG_DEBUG("done starting motor\n");
    laser->laser_timer = delay_msec_timer_non_blocking(2, dec_laser_timer_complete_callback_func, laser, false); 
    return ( true );
}
static bool dec_continue_spinning_mirror_4729(dec_laser_t * laser)
{
    laser->spin_state = SPINNING_MIRROR;
    laser->laser_timer = delay_msec_timer_non_blocking(10, dec_laser_timer_complete_callback_func, laser, false);
    ASSERT(laser->laser_timer != NULL);
    //dbg_printf("Laser Timer %x \n", laser->laser_timer);
    return(true);
}
static bool dec_hsync_laser_mirror_4729(dec_laser_t * laser)
{
   //HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;
   WFG0_REGS_t *lscan_wfg = laser->wfs->lscan_wfg;
   WFS_REGS_t *lscan_wfs = laser->lscan_wfs;

   DBG_ASSERT (laser->spin_state == SPINNING_MIRROR); 

  
   lscan_wfg->scfg = WFG0_SCFG_COUNT_REPLACE_VAL(lscan_wfg->scfg, laser->lperiod + 100 );

   /// LD_CTRL[0] = SH (WFG0[0])
   //lscan_wfs->wave_ld_sel0 = WFS_WAVE_LD_SEL0_WFG0_REPLACE_VAL(0x0, 0x1 );

   /// LD_CTRL[2] = nSH_A
   /// LD_CTRL[3] = nSH_B
   lscan_wfs->wave_ld_sel1 = WFS_WAVE_LD_SEL1_WFG0_REPLACE_VAL(0x0, 0x1 );
   lscan_wfs->wave_ld_sel2 = WFS_WAVE_LD_SEL2_WFG0_REPLACE_VAL(0x0, 0x2 );

/// stefan foo
#if 0
   // force laser on manual set.
   lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel,0xFF);  // Force laser 100% on   
   lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel,   0xFF);  // latch 

   cpu_spin_delay(1);
   // ld_cntl0 and ld_cntl 1 are set
   lscan_wfs->data_sel = WFS_DATA_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->data_sel, 0x003 );  // SH=1 && LD=1
   lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel,    0x003 );  // latches the data
   cpu_spin_delay(1);
/// end foo
#endif 
   // waveform contol of everything but LD_CTRL1, so laser still forced on.

   //lscan_wfs->data_sel = WFS_DATA_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->data_sel, 0x002 );  // LDON forced to 1,  sh is waveform
   //lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel,    0x002 );  // latch ldon.
   lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel, 0x000);
   // 0 disables qualification, or wave form qualify.
   //lscan_hsync->hcfg = HSYNC_HCFG_WFG_ENABLE_REPLACE_VAL(lscan_hsync->hcfg, 0xf ); 


   //lscan_wfg->scfg = WFG0_SCFG_MODE_REPLACE_VAL(lscan_wfg->scfg, 0x0 );                ///< disabled (reset)
   //lscan_wfg->scfg = WFG0_SCFG_MODE_REPLACE_VAL(lscan_wfg->scfg, 0x2 );                ///< sync to HSyncIn (enable)
   
   laser->spin_state = SYNCING_HSYNC; //always change state before enabling timer.
 
   laser->laser_timer = delay_msec_timer_non_blocking(100, dec_laser_timer_complete_callback_func, laser, false);
   return( true );
}

bool dec_filter_laser_mirror_4729(dec_laser_t * laser)
{
    WFS_REGS_t *lscan_wfs = laser->lscan_wfs;

    DBG_ASSERT (laser->spin_state == SYNCING_HSYNC);
    //gpio_isr_disable( nVideo_gpio );
    

    /* stefan foo   moved for comment out block debugging  */
    HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;
    WFG0_REGS_t *lscan_wfg = laser->wfs->lscan_wfg;
    DBG_LOG_DEBUG("changing hsync %x\n",WFG0_ACTION10_COUNT_MASK_SHIFT(lscan_wfg->action27));
    lscan_hsync->hcfg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(lscan_hsync->hcfg, 0x0 );         ///< filter/qual with WFG HSync_en disable
    set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action28,
                  WAVEFORM_ACTION_HIGH, WFG0_ACTION10_COUNT_MASK_SHIFT(lscan_wfg->action27)  
                  );
    lscan_hsync->hcfg = HSYNC_HCFG_WFG_ENABLE_REPLACE_VAL(lscan_hsync->hcfg, 0xf );
    lscan_hsync->hcfg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(lscan_hsync->hcfg, 0x1 );         ///< filter/qual with WFG HSync_en
    lscan_wfg->scfg = WFG0_SCFG_RECOV_REPLACE_VAL(lscan_wfg->scfg, 0x1 );                   ///< allow missing HSYNC
    lscan_wfg->scfg = WFG0_SCFG_COUNT_REPLACE_VAL(lscan_wfg->scfg, laser->lperiod + 100 );  ///< HSYNC missing period
    //lscan_wfs->wave_pol = WFS_WAVE_POL_FORCE_INV_REPLACE_VAL(lscan_wfs->wave_pol, 0x03 );
    //lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel, 0x00 );    ///< clears FRC LASER ON
    lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel, 0x01 );    ///< clears FRC LASER ON
    lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel, 0x03 );       ///< enable ForceLaserOn wave
    lscan_wfs->wave_frc_sel0 = WFS_WAVE_FRC_SEL0_WFG0_REPLACE_VAL(0x0, 0x84 );
    lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel, 0x00 );    ///< clears FRC LASER ON
    lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel, 0x00 );       ///< enable ForceLaserOn wave
    DBG_LOG_DEBUG("finished changing hsync\n");
    laser->spin_state = SYNCING_FILTER;
    laser->laser_timer = delay_msec_timer_non_blocking(1000, dec_laser_timer_complete_callback_func, laser, false);

    return( true );
}

void dec_enable_vdo(dec_laser_t *laser, bool on_notoff)
{ 
    WFS_REGS_t *lscan_wfs = laser->lscan_wfs;

    if (on_notoff)
    {
        lscan_wfs->wave_en_sel0 = 
            WFS_WAVE_EN_SEL0_WFG0_REPLACE_VAL(0x0, 0x10);   ///< enables the vdo enable output bit. 
        lscan_wfs->wave_en_sel1 = 
            WFS_WAVE_EN_SEL1_WFG0_REPLACE_VAL(0x0, 0x10);   ///< enables the vdo enable output bit. 
                                                            
    }
    else
    {
        lscan_wfs->wave_en_sel0 = 0;                        ///< disables the vdo enable output bit
        lscan_wfs->wave_en_sel1 = 0;                        ///< disables the vdo enable output bit
    }
        
}
void dec_stop_laser_mirror(dec_laser_t *laser)
{ 
    WFS_REGS_t *lscan_wfs;

    lscan_wfs = laser->lscan_wfs;
    
    
    if (laser->laser_timer) 
    {
        laser->laser_timer = delay_msec_timer_cancel(laser->laser_timer);
    }
    laser->spin_state = IDLE;
    if (laser->laser_WD_timer) 
    {
        laser->laser_WD_timer = delay_msec_timer_cancel(laser->laser_WD_timer);
    }
    
    /// turn off laser
   lscan_wfs->data_sel = WFS_DATA_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->data_sel, 0x006 );  // LDON forced to 0
   lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel,    0x006 );  // latch ldon.


   lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel,0x00);  // Force laser 0% on   
   lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel,   0x03);  // latch 

    
    if ( gpio_write( laser_enable, 1)) 
    {
        ASSERT(false);//if this happens, gpio is completely screwed up
    }
    if ( gpio_write( laser_pwr_enable, 1)) 
    {
        ASSERT(false);//if this happens, gpio is completely screwed up
    }
    
    set_wavegen_DAC(laser, 0); ///<turns off the power to the laser
    // turn off polygon motor and restore defaults   

    init_Lscan(laser);  ///< resets the mirror which stops it spinning
    dcmotor_run(laser_motor, off_speed);
    sem_post(&laser->complete_sem); // Do this last to avoid task switching
}
bool dec_laser_spin_phase_exec(dec_laser_t *laser, uint32_t phase) 
{
    DBG_ASSERT(laser != NULL);
#if 0
    MIRROR0_REGS_t *lscan_mirror = laser->mirror->lscan_mirror;
    DBG_LOG_ERR("Timer exec fn, %d %x %d %d\n",laser->spin_state,
               lscan_mirror->status,
               MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status),
               MIRROR0_STATUS_LOCKED_MASK_SHIFT(lscan_mirror->status)
               );
#endif
    if ( phase == SPINNING_MIRROR ) 
    {
        return ( dec_start_laser_mirror_4729(laser) ); ///<  Starts the mirror spinning 
    }
    else if (phase == CONTINUE_SPINNING_MIRROR) 
    {
        return(dec_continue_spinning_mirror_4729(laser)); ///<  Continue to wait for the mirror to spin up to speed
    }
    else if (phase == ENABLE_LASER ) 
    {
        return ( dec_enable_laser_mirror_4729(laser) ); //< Inits the laser
    }
    else if (phase == ENABLE_LASER2 ) 
    {
        return ( dec_enable2_laser_mirror_4729(laser) ); //< Inits the starts motor
    }
    else if ( phase == SYNCING_HSYNC ) 
    {
        return ( dec_hsync_laser_mirror_4729(laser) ); ///< Syncs the mirror with hsync
    }
    else if ( phase == SYNCING_FILTER ) 
    {
        return ( dec_filter_laser_mirror_4729(laser) ); ///< Fills up the hsync filter
    }
    else 
    {
        DBG_ERR("laser %x, phase %d \n",laser, phase);
        DBG_ASSERT(0);///< Bad phase
        dec_stop_laser_mirror(laser);
        return(false);
    }
}
/**
 * Init code
 */
void dec_laser_local_init(void)
{
   APB_CONFIG_REGS_t *apbconfig = NULL;
   uint32_t lperiod; 
   uint32_t sh_a_high, sh_a_low;
   uint32_t sh_b_high, sh_b_low;
   uint32_t fla_high, fla_low; 
   uint32_t flb_high, flb_low; 
   uint32_t envdo_high, envdo_low; 
   uint32_t enhsync_low, enhsync_high, enhsync_high1;
   uint32_t start_sync_high, start_sync_low; 
   uint32_t start_sync_sh_high, start_sync_sh_low;
   uint32_t db_force_on, db_force_off;

   apbconfig = dec_laser_open_apb_config_regs();

#if defined(ASIC_gemstone)
   apbconfig->PINCR = APB_CONFIG_PINCR_LVDO_OE_REPLACE_VAL( apbconfig->PINCR, 0xF );
   apbconfig->PINCR = APB_CONFIG_PINCR_PENLVDSSEL_REPLACE_VAL( apbconfig->PINCR, 0x0 );
   //apbconfig->PLL0_CR1 = APB_CONFIG_PLL0_CR1_SEL_VCO_CLK_SE_REPLACE_VAL( apbconfig->PLL0_CR1, 0x1 );
   apbconfig->PDCR1 = APB_CONFIG_PDCR1_LVDSSR_REPLACE_VAL(apbconfig->PDCR1, 0x3c);      // fast slew for VDO outputs
   apbconfig->PWCR = APB_CONFIG_PWCR_LVDS_PAD_PDWN_REPLACE_VAL(apbconfig->PWCR, 0x0 );  // power up VDO outputs
#elif defined(ASIC_granite)
   apbconfig->PINCR = APB_CONFIG_PINCR_LVDO_OE_REPLACE_VAL( apbconfig->PINCR, 0xFF );
   apbconfig->PINCR = APB_CONFIG_PINCR_LVDS_PENNVID_SEL_REPLACE_VAL( apbconfig->PINCR, 0x0 );
   //apbconfig->PLL0_CR1 = APB_CONFIG_PLL0_CR1_SEL_VCO_CLK_SE_REPLACE_VAL( apbconfig->PLL0_CR1, 0x1 );
   apbconfig->PDCR1 = APB_CONFIG_PDCR1_OLVDSSR_REPLACE_VAL(apbconfig->PDCR1, 0x1FF); // fast slew for VDO outputs
   apbconfig->PDCR2 = APB_CONFIG_PDCR2_ILVDS_PD_REPLACE_VAL(apbconfig->PDCR2, 0x0);  // power up VDO outputs
#else
#error "Unknown ASIC"
#endif

   dec_laser_close_apb_config_regs();   // no need to keep apb config regs open since only used once at init
   apbconfig = NULL;

     /**
      * ddd.dd uSec converted to ppp MHzclock pulses is ddddd * ppp /
      * 100
      * 
      * I'm wondering if it should be ((ddddd * ppp) + 50) / 100)
      * that will give consistant rounding.
      */
#define num_mirror_facets 4
#define lperiod_in_100_msec (53860 )
#define hysync_window_100ms 1560
#define hysync_post_trigger_100ms 50
   lperiod = lperiod_in_100_msec*240/100 /*71182*/;   // 240MHz  ///< period of a mirror face in clock pulses
   
   sh_a_low =  (lperiod_in_100_msec - 11300)*240/100;
   sh_a_high = (lperiod_in_100_msec - (11300 )+ 580)*240/100;
   sh_b_low =  (lperiod_in_100_msec - 9800)*240/100;
   sh_b_high = (lperiod_in_100_msec - (9800 )+ 580)*240/100;
   fla_low = (lperiod_in_100_msec - (11500))*240/100;
   fla_high = (lperiod_in_100_msec - (11500 )+ 1000)*240/100;
   flb_low = (lperiod_in_100_msec - 10000)*240/100;
   flb_high = (lperiod_in_100_msec - (10000 )+ 1000)*240/100;
   envdo_high = 5600*240/100;
   envdo_low = (38000)*240/100;
   enhsync_low = /*hysync_post_trigger_100ms *240/100*/ 0;
   enhsync_high = (lperiod_in_100_msec - hysync_window_100ms + hysync_post_trigger_100ms)*240/100;
   enhsync_high1 = (lperiod_in_100_msec - (lperiod_in_100_msec  /3))*240/100;
   start_sync_high = 50000 * 240 /100;
   start_sync_low  = 1000 * 240;
   start_sync_sh_high = start_sync_low + sh_a_high;
   start_sync_sh_low = start_sync_low + sh_a_low;
   db_force_on = 10000*240/100;
   db_force_off = 19000*240/100;


   mylasers[0] = &my_lasers[0];

   mywavegens[0] = &my_wavegens[0];
   mywavegens[0]->wavegen_num = 0;
   mywavegens[0]->wavegen = dec_laser_get_wavegen_regs(0);
   mywavegens[0]->lut_value_on = 434;///< LD_power = 1.4v = 434*3.3/1024

   mywfss[0] = &my_wfss[0];
   mywfss[0]->wfs_num = 0;
   mywfss[0]->lscan_wfg = dec_laser_get_lscan_wfg_regs(0);

   mymirrors[0] = &my_mirrors[0];
   mymirrors[0]->mirror_num = 0;
   mymirrors[0]->lscan_mirror = dec_laser_get_lscan_mirror_regs(0);
   mymirrors[0]->min_spin_time = 6200;
   mymirrors[0]->wd_spin_time = 6200; // 3100;

   mylasers[0]->next_laser = NULL;
   mylasers[0]->laser_num = 0;
   mylasers[0]->mirror = &my_mirrors[0];
   mylasers[0]->wfs = &my_wfss[0];
   mylasers[0]->wavegen = &my_wavegens[0];
   mylasers[0]->lscan_wfs = dec_laser_get_lscan_wfs_regs();
   mylasers[0]->lscan_hsync = dec_laser_get_lscan_hsync_regs();
   mylasers[0]->lperiod = lperiod;
   mylasers[0]->start_lperiod = 2000 * 240; ///<2 ms 
   mylasers[0]->spin_state = IDLE;
   if ( gpio_open( &laser_pwr_enable,
                        GPIO_BANK_E,
                        23,
                        GPIO_DIRECTION_OUTPUT,
                        GPIO_LOGIC_SETTING_ACTIVE_HIGH,
                        1 ) != OK )

   {
       ASSERT(false); //This should never fail.
   }
   if ( gpio_open( &laser_enable,
                        GPIO_BANK_E,
                        13,
                        GPIO_DIRECTION_OUTPUT,
                        GPIO_LOGIC_SETTING_ACTIVE_HIGH,
                        1 ) != OK )

   {
       ASSERT(false); //This should never fail.
   }
   // HSYNC configuration
   init_lscan_hsync_hcfg(mylasers[0]->lscan_hsync,
                         0,   ///< use internal GalvoRtoL
                         0x0, ///<  disable WFG[3:0]
                         0x2, ///<  pulse on falling HSync
                         0x0, ///<  no qual (for now)
                         0x0, ///<  no qual (for now)
                         0x0, ///<  active high HSyncIns
                         0x0, ///<  active high HSyncOuts
                         0x0); ///< not yet 0xF);///<  enable all HSyncOuts

   // HSync filter configuration
   init_lscan_hsync_filter(mylasers[0]->lscan_hsync,
                           0x1,0x1, ///< fall hi & lo
                           0x1,0x1, ///< rise hi & lo
                           0xf);   ///< hfilt threshold
                                   
   // HSync timers (set all timers to measure HSync[0] only)
   init_lscan_hsync_timers(mylasers[0]->lscan_hsync,                            
                           0,  ///<  disable all timers
                           0,  ///<  disable all timers
                           hsync_timer_pulse0,  ///<  event01 = hsync[0]
                           hsync_timer_pulse0,  ///<  event02 = hsync[0]
                           hsync_timer_pulse0,  ///<  event03 = hsync[0]
                           hsync_timer_pulse0,  ///<  event04 = hsync[0]
                           hsync_timer_pulse0,  ///<  event05 = hsync[0]
                           hsync_timer_pulse0,  ///<  event06 = hsync[0]
                           hsync_timer_pulse0,  ///<  event07 = hsync[0]
                           hsync_timer_pulse0,  ///<  event08 = hsync[0]
                           hsync_timer_pulse0,  ///<  event09 = hsync[0]
                           hsync_timer_pulse0,  ///<  event10 = hsync[0]
                           hsync_timer_pulse0,  ///<  event11 = hsync[0]
                           hsync_timer_pulse0,  ///<  event12 = hsync[0]
                           hsync_timer_pulse0,  ///<  event13 = hsync[0]
                           hsync_timer_pulse0,  ///<  event14 = hsync[0]
                           hsync_timer_pulse0,  ///<  event15 = hsync[0]
                           hsync_timer_pulse0   ///<  event16 = hsync[0]
                           );
   init_mirror_mcfg1(mylasers[0]->mirror->lscan_mirror, 
                     0x1,                 ///<  open-loop BLDC
                     0x0,                 ///<  use HSyncIn[0]
                     num_mirror_facets,   ///<  4 facets
                     mylasers[0]->lperiod ///<  296.59us
                     );

   init_mirror_mcfg2(mylasers[0]->mirror->lscan_mirror,
                          0x3, ///<  disable GalvoRtoL
                          0x1, ///<  disable nStart
                          0x1, ///<  disable Clk
                          0x0, ///<  idle GalvoRtoL=0
                          0x0, ///<  Start=0
                          0x0, ///<  idle Clk = 0
                          0x0, ///<  nReady_LSU
                          0x0, ///<  ignored open-loop
                          0x0, ///<  ignored open-loop
                          0x0  ///<  ignored open-loop
                          );
   mylasers[0]->mirror->lscan_mirror->lockmax = lperiod * num_mirror_facets  + 16;  ///<  max period (4 facets) to consider locked
   mylasers[0]->mirror->lscan_mirror->lockmin = lperiod * num_mirror_facets  -16;  ///<  min period (4 facets) to consider locked
   DBG_DEBUG("Lock period is %x \n",lperiod * num_mirror_facets);
   mylasers[0]->mirror->lscan_mirror->hsync_int = 20;          ///<  Interrupt every 20 HSync pulses
   mylasers[0]->mirror->lscan_mirror->int_en = 0;              ///<  int enable

// int status (pending)
/*
MIRROR0_INT_ST_HSYNC_INT_ST_MASK_SHIFT(lscan_mirror0->int_st)
MIRROR0_INT_ST_MEASR_INT_ST_MASK_SHIFT(lscan_mirror0->int_st)
MIRROR0_INT_ST_ERROR_INT_ST_MASK_SHIFT(lscan_mirror0->int_st)
MIRROR0_INT_ST_LATE_INT_ST_MASK_SHIFT(lscan_mirror0->int_st)
MIRROR0_INT_ST_EARLY_INT_ST_MASK_SHIFT(lscan_mirror0->int_st)
*/

   mylasers[0]->mirror->lscan_mirror->int_cl = 0;  ///<  int ack
   mylasers[0]->mirror->lscan_mirror->int_fo = 0;  ///<  int force
                                                   
   init_mirror_galv_sync(mylasers[0]->mirror->lscan_mirror,
                         0x0, 0x0, ///<  sync_galvo0 enable & set
                         0x0, 0x0);///<  sync_galvo1 enable & set
                                     
   init_mirror_galv_wfg(mylasers[0]->mirror->lscan_mirror,
                        0x0, 0x0, ///<  sync_wfg0 enable & set
                        0x0, 0x0);///<  sync_wfg1 enable & set

   /// Unused
   init_mirror_galvo_rtol(mylasers[0]->mirror->lscan_mirror,
                          0x0, 0x0, ///< galvo_rtol rise0 & fall0
                          0x0, 0x0);///< galvo_rtol rise1 & fall1
                                    
   init_mirror_galvop(mylasers[0]->mirror->lscan_mirror,
                      1,
                      10, ///<  Clock
                      1,
                      lperiod/2 + 10,
                      0,0,0,0 ///< galvop 1 unused
                      );
   
   init_mirror_galvon(mylasers[0]->mirror->lscan_mirror,
                      0,0,0,0, ///< galvon 0 unused
                      0,0,0,0 ///< galvon 1 unused
                      );  

   /// unused in open-loop mode
   init_mirror_hsync_en0(mylasers[0]->mirror->lscan_mirror,
                           0,0,0,0, 
                           0,0,0,0 
                           );

   /// unused in open-loop mode
   init_mirror_hsync_en1(mylasers[0]->mirror->lscan_mirror,
                           0,0,0,0, 
                           0,0,0,0 
                           );
   
   init_wfs_scfg(mylasers[0]->wfs->lscan_wfg,
                 0x2,   ///<  sync to HSyncIn
                 0x1,   ///<  wait for sync
                 0x0,   ///<  unused
                 0x0,   ///<  ignore GalvoRtoL
                 0x0,   ///<  use HSyncIn[0]
                 mylasers[0]->lperiod + 100     ///<  overflow count
                 );
   /**
    * Each wfs has 8 general use wavelets (0 to 7) and 1
    * hsync wavelet(8) and one calibration wavelet.  Each wavelet
    * can fire every N (nSync) cycles. For some engines they fire
    * every nSync.  Each wavelet can have an initial state of high
    * or low.  Each wavelet can have up to two state changes, at
    * action1x.period and action2x.period (where x is the wavelet
    * number) clock ticks from nsync.  For more complicated waves,
    * multiple wavelets, can be or'd together to get the desired
    * wave.
    */
   /// WFG0[0] = SH_A // 
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period0, 
                  0x1, 0x1 ///<  RTZ every cycle 
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action10, 
                  WAVEFORM_ACTION_LOW, sh_a_low ///<  set high @ 252.43us 
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action20,
                  WAVEFORM_ACTION_HIGH, sh_a_high   ///<  set low @ 274.25us
                  );

   /// WFG0[1] = SH_B
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period1,
                  0x1,  0x1  
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action11,
                  WAVEFORM_ACTION_LOW, sh_b_low  ///<  none
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action21,
                  WAVEFORM_ACTION_HIGH, sh_b_high  ///<  none
                  );

   /// WFG0[2] = fla
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period2,
                  0x0, 0x1       ///<  RTZ every cycle
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action12,
                  WAVEFORM_ACTION_HIGH, fla_low  
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action22,
                  WAVEFORM_ACTION_LOW, fla_high 
                  ); 

   /// WFG0[3] = flb
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period3,
                  0x0, 0x1);       ///<  RTZ every cycle
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action13,
                  WAVEFORM_ACTION_HIGH, flb_low  ///<  set high @ 272.31us
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action23,
                  WAVEFORM_ACTION_LOW, flb_high);  ///<  set low @ 277.29us

   /// WFG[4] = EnableVDO   
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period4,
                  0x0, 0x1       ///<  RTZ every cycle
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action14,
                  WAVEFORM_ACTION_HIGH, envdo_high);  
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action24,
                  WAVEFORM_ACTION_LOW, envdo_low  
                  );


   /// WFG[5] hsync
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period5,
                  0x1, 0x1       ///<  RTZ every cycle
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action15,
                  WAVEFORM_ACTION_LOW,0   
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action25,
                  WAVEFORM_ACTION_HIGH, /*enhsync_high1*/enhsync_high1  
                  );

   /// WFG[6] hsync
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period6,
                  0x0, 0x1       // RTZ every cycle
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action16,
                  WAVEFORM_ACTION_LOW, 0  
                  ); 
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action26,
                  WAVEFORM_ACTION_DISABLED, enhsync_high/*enhsync_high +1  */
                  );

   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period7,
                  0x0, 0x1       // RTZ every cycle
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action17,
                  WAVEFORM_ACTION_LOW, enhsync_low  
                  ); 
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action27,
                  WAVEFORM_ACTION_HIGH, enhsync_high  
                  );
   /// unused WFG0[7]
   //mylasers[0]->wfs->lscan_wfg->period7 = 0;
   //mylasers[0]->wfs->lscan_wfg->action17 = 0;
   //mylasers[0]->wfs->lscan_wfg->action27 = 0;

   // stefan foo
   // hack a stripe in video output polarity and 
   // WFG0[7] = !ForceLaserOn_B
   //set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period7,
   //               0x1, 0x1);       ///<  RTZ every cycle
   //set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action17,
   //               WAVEFORM_ACTION_LOW, db_force_on  
   //               );
   //set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action27,
   //               WAVEFORM_ACTION_HIGH, db_force_off);



/* 
   The connection is that the output of wfg element 8 is dedicated to the HSync_enable_wfg signal.  
   So, the first code snippet you quote programs wfg0[8], and the second snippet enables filtering with 
   HSync_enable_wfg (referred to as "WFG HSync_en" in the comment), and the link is that those are the same thing.
   That connection doesn't seem to be clearly described in the DRS documentation.  
*/


   /// HSync_enable
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period8,
                  0x1, 0x1       ///<  RTO every cycle
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action18,
                  WAVEFORM_ACTION_LOW, 0 
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action28,
                  WAVEFORM_ACTION_HIGH, enhsync_high1  
                  );

   /// Cal request (unused)
   mylasers[0]->wfs->lscan_wfg->period9 = 0;
   mylasers[0]->wfs->lscan_wfg->action19 = 0;
   mylasers[0]->wfs->lscan_wfg->action29 = 0;
   laser_motor = dcmotor_by_name("dcmotor_mirror_motor");
   off_speed = dcmotor_speed_by_name("speed_off");
   half_speed = dcmotor_speed_by_name("speed_half");
   full_speed = dcmotor_speed_by_name("speed_full");
}
