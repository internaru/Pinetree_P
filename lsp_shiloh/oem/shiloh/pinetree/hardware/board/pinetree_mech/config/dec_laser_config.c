/*
 * ======================================================================
 * Copyright (c) 2010, 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

/// \file dec_laser_config.c

#include "regAddrs.h"
#include "asic.h"
#include "dec_laser_regs_config.h"
#include "dec_laser.h"
#include "dec_laser_priv.h"

#include <stdint.h>
#include <string.h>
#include "lassert.h"

#define DEC_CLOCK_FREQ_MHZ 240
#define LASER_PERIOD_MARGIN (100)


static unsigned int lperiod_in_ns = 289012;
static unsigned int lmotor_period_in_ns = 288696;
static unsigned int sh1_high_period_in_ns = 271180;
static unsigned int sh2_clock_high = 8000;
static unsigned int sh2_clock_low = 4000;

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
#if 0
#define MAX_WAVEGEN 1
static dec_wavegen_t my_wavegens[MAX_WAVEGEN];
static dec_wavegen_t *mywavegens[] = 
{
    &my_wavegens[0],
    0
};  // last must be null.
#endif

static gpio_handle_t *laser_enable;

void init_Lscan (dec_laser_t * laser) 
{
   DEC_CONFIG_REGS_REGS_t *dec_config = dec_laser_get_dec_config_regs_base();
   MIRROR0_REGS_t *lscan_mirror = laser->mirror->lscan_mirror;
   WFS_REGS_t *lscan_wfs = laser->lscan_wfs;
   HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;
   WFG0_REGS_t *lscan_wfg = laser->wfs->lscan_wfg;
   lscan_mirror->mcfg1 = MIRROR0_MCFG1_MODE_REPLACE_VAL(lscan_mirror->mcfg1, 0x0 );//reset mirror counts
   lscan_mirror->mcfg1 = MIRROR0_MCFG1_MODE_REPLACE_VAL(lscan_mirror->mcfg1, 0x1 );//restore open loop mode

   DBG_ERR("%s\n", __func__);  

   ///mirror config
   init_mirror_mcfg2(lscan_mirror,
                          0x3, // disable GalvoRtoL
                          0x1, // disable nStart
                          0x1, // disable Clk
                          0x0, // idle GalvoRtoL=0
                          0x1, // Start=1 (active low)
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
                         0x0, // active low HSyncIns
                         0x0, // active low HSyncOuts
                         0xF);// enable all HSyncOuts


   /// HSync filter configuration
   init_lscan_hsync_filter(lscan_hsync,
                           0x1,0x1, //fall hi & lo
                           0x1,0x1, //rise hi & lo
                           0xf);   //hfilt threshold

   lscan_wfg->scfg = WFG0_SCFG_RECOV_REPLACE_VAL(lscan_wfg->scfg, 0x0 );                   ///< don't insert missing HSYNC

   /// waveform select
   lscan_wfs->out_sel = WFS_OUT_SEL_ENABLE_REPLACE_VAL(lscan_wfs->out_sel, 0x00 );      // waveform
   lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel, 0x00 );    // waveform
   lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel, 0x03 );     // fixed output until motor reports lock

   /// data override
   lscan_wfs->data_sel = WFS_DATA_SEL_ENABLE_REPLACE_VAL(lscan_wfs->data_sel, 0x00 );  // noop
   lscan_wfs->data_sel = WFS_DATA_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->data_sel, 0x00 );// noop
   lscan_wfs->data_sel = WFS_DATA_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->data_sel, 0x003 );// noop

   /// normal polarity
   lscan_wfs->wave_pol = WFS_WAVE_POL_ENABLE_INV_REPLACE_VAL(lscan_wfs->wave_pol, 0x00 );
   lscan_wfs->wave_pol = WFS_WAVE_POL_FORCE_INV_REPLACE_VAL(lscan_wfs->wave_pol, 0x00 );
   lscan_wfs->wave_pol = WFS_WAVE_POL_LD_CTRL_INV_REPLACE_VAL(lscan_wfs->wave_pol,0x00 );

   /**
    * LD_CTRL[11:0] are up to 12 pin output of the lscan block.
    * They are either constant or some OR function of the waveform
    * generator blocks.  It is one of the main outputs of the lscan
    * blocks. For this engine, we only need to use LD_CTRL[1:0]
    */

   /// LD_CTRL[0] = SH1 WFG0[0]
   lscan_wfs->wave_ld_sel0 = WFS_WAVE_LD_SEL0_WFG0_REPLACE_VAL(0x0, 0x1 );
   /// LD_CTRL[1] = SH2 WFG0[1]
   lscan_wfs->wave_ld_sel1 = WFS_WAVE_LD_SEL1_WFG0_REPLACE_VAL(0x0, 0x2 );

   dec_config->oe_misc = DEC_CONFIG_REGS_OE_MISC_LD_CTRL_OE_REPLACE_VAL(dec_config->oe_misc, 0x3 ); // enable LD_CTRL[1:0]

   /// LD_CTRL[11:2] unused
   lscan_wfs->wave_ld_sel2 = 0;
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
   lscan_wfs->wave_frc_sel0 = 0; 
   // debug: hack a stripe | in WFG[7]
   // lscan_wfs->wave_frc_sel0 = WFS_WAVE_FRC_SEL0_WFG0_REPLACE_VAL(0x0, 0x8C );
   lscan_wfs->wave_frc_sel1 = 0;
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

static void spinning_mirror_return_func ( dec_laser_t * dec_laser )
{ 
    MIRROR0_REGS_t *lscan_mirror = dec_laser->mirror->lscan_mirror;
    static uint32_t lock_count = 0;
#if 0
    dbg_printf("mirror timer fired, status_reg = %x lock_det = %d locked = %d\n",lscan_mirror->status,
               MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status),
               MIRROR0_STATUS_LOCKED_MASK_SHIFT(lscan_mirror->status)
               );
#endif   
    
//    dbg_printf("%s: mirror status = %#x cfg1 = %#x, cfg2 = %#x\n", __func__, lscan_mirror->status, lscan_mirror->mcfg1, lscan_mirror->mcfg2);

    ASSERT(dec_laser->spin_state == SPINNING_MIRROR);
    /**
     * Turn enable laser after lock det
     * Only do it once
     * Then continue waiting for both lock det & locked
     * locked can only occur after laser has been enabled.
     */
    if ((dec_laser->lock_det == false) && (MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status) == 1))
    {
        DBG_ERR("Laser motor locked status_reg = %x\n", lscan_mirror->status);
        dec_laser->lock_det = true;
        lock_count = 0;
        dec_laser_internal_phase_complete(dec_laser, ENABLE_LASER );
        return;
    }

    // If this succeeds then we are finished with the phase and can go to the next one.      
    // Otherwise, it waits for the next ISR to fire
    if ((MIRROR0_STATUS_LOCK_DET_MASK_SHIFT(lscan_mirror->status) == 1)
        && (MIRROR0_STATUS_LOCKED_MASK_SHIFT(lscan_mirror->status) == 1))
    {
        if(lock_count >= 5)
        {
            DBG_ERR("Laser beam locked status_reg = %x\n", lscan_mirror->status);
            lock_count = 0;
            dec_laser_internal_phase_complete(dec_laser, SYNCING_HSYNC );
        }
        else
        {
            lock_count++;
            dec_laser_internal_phase_complete(dec_laser, CONTINUE_SPINNING_MIRROR );
        }
    }
    else //Wait and try again
    {
//        dbg_printf("Wait and try again\n");
        lock_count = 0;
        dec_laser_internal_phase_complete(dec_laser, CONTINUE_SPINNING_MIRROR );
    }
}

void dec_laser_timer_complete_callback_func(void *data)  

{
    dec_laser_t * dec_laser = (dec_laser_t *) data;

    dec_laser->laser_timer = NULL;
#if 0
    MIRROR0_REGS_t *lscan_mirror = dec_laser->mirror->lscan_mirror;
    dbg_printf("Timer cb fn, %d %x %d %d\n",dec_laser->spin_state,
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
        spinning_mirror_return_func ( dec_laser );
        break;
    case ENABLE_LASER:
        dec_laser_internal_phase_complete(dec_laser, CONTINUE_SPINNING_MIRROR );
        break;
    case SYNCING_HSYNC:
        dec_laser_internal_phase_complete(dec_laser, SYNCING_FILTER );
        break;
    case SYNCING_FILTER:
        dec_laser->lscan_hsync->hcfg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(dec_laser->lscan_hsync->hcfg, 0x0 );      ///NO filter/qual with WFG HSync_en
        dec_laser_spin_complete(dec_laser, READY_FOR_TOP );
        break;
    default:
        DBG_ASSERT(0 == "unknown laser state");
        dec_laser_spin_complete(dec_laser, IDLE );
    }
}
static bool dec_start_laser_mirror(dec_laser_t * laser)
{
   MIRROR0_REGS_t *lscan_mirror;

   DBG_ERR("%s\n", __func__);   
   
   DBG_ASSERT(laser->spin_state == IDLE); 

   /// make sure timer is NULL'ed out
   laser->laser_timer = NULL;
   laser->lock_det = false;

   init_Lscan(laser);

   lscan_mirror = laser->mirror->lscan_mirror;

   /// enable polygon mirror rotation
   lscan_mirror->mcfg2 = MIRROR0_MCFG2_GALVOP_DIS_REPLACE_VAL(lscan_mirror->mcfg2, 0x0 ); ///< enable Clk

   //TODO: need to make delay configurable?
   posix_sleep_ms(100); // delay for 0.1s after starting clock before we toggle start line

   lscan_mirror->mcfg2 = MIRROR0_MCFG2_GALVON_IDLE_REPLACE_VAL(lscan_mirror->mcfg2, 0x0 ); ///< Start=0 (active low)

   laser->spin_state = SPINNING_MIRROR;// always change state before ISR.

//   dbg_printf("%s: mirror status = %#x cfg1 = %#x, cfg2 = %#x\n", __func__, lscan_mirror->status, lscan_mirror->mcfg1, lscan_mirror->mcfg2);
   
   laser->laser_timer = delay_msec_timer_non_blocking(200, dec_laser_timer_complete_callback_func, laser, false);
   return ( true );
}

static bool dec_enable_laser_mirror(dec_laser_t * laser)
{ 
    WFG0_REGS_t *lscan_wfg = laser->wfs->lscan_wfg;
    WFS_REGS_t *lscan_wfs = laser->lscan_wfs;
    HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;
    error_type_t err;

    DBG_ASSERT (laser->spin_state == SPINNING_MIRROR); 
   
    DBG_ERR("%s\n", __func__);

    err = gpio_write( laser_enable, 1); // enable laser power
    ASSERT(err == OK);

   /// LD_CTRL[0] = SH (WFG0[0])
   lscan_wfs->wave_ld_sel0 = WFS_WAVE_LD_SEL0_WFG0_REPLACE_VAL(0x0, 0x1 );

   /// LD_CTRL[1] = LDON (data) WFG0[1]
   lscan_wfs->wave_ld_sel1 = WFS_WAVE_LD_SEL1_WFG0_REPLACE_VAL(0x0, 0x2 );   


   lscan_wfg->scfg = WFG0_SCFG_COUNT_REPLACE_VAL(lscan_wfg->scfg, laser->lperiod + LASER_PERIOD_MARGIN );

   lscan_hsync->hcfg = HSYNC_HCFG_WFG_ENABLE_REPLACE_VAL(lscan_hsync->hcfg, 0x3 );      // enable the wave form generators
   lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel, 0x00 );     // output data based on waveform 

   lscan_wfg->scfg = WFG0_SCFG_MODE_REPLACE_VAL(lscan_wfg->scfg, 0x0 );                ///< disabled (reset)
   lscan_wfg->scfg = WFG0_SCFG_MODE_REPLACE_VAL(lscan_wfg->scfg, 0x2 );                ///< sync to HSyncIn (enable)

    laser->spin_state = ENABLE_LASER; // always change state before enabling timer.
   
    laser->laser_timer = delay_msec_timer_non_blocking(1, dec_laser_timer_complete_callback_func, laser, false); 
    return ( true );
}

static bool dec_continue_spinning_mirror(dec_laser_t * laser)
{
    laser->spin_state = SPINNING_MIRROR;
    laser->laser_timer = delay_msec_timer_non_blocking(10, dec_laser_timer_complete_callback_func, laser, false);
    ASSERT(laser->laser_timer != NULL);
    //dbg_printf("Laser Timer %x \n", laser->laser_timer);
    return(true);
}

static bool dec_hsync_laser_mirror(dec_laser_t * laser)
{
   HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;
   WFG0_REGS_t *lscan_wfg = laser->wfs->lscan_wfg;
   WFS_REGS_t *lscan_wfs = laser->lscan_wfs;

   DBG_ASSERT (laser->spin_state == SPINNING_MIRROR); 

    DBG_ERR("%s\n", __func__);
   
   lscan_wfg->scfg = WFG0_SCFG_COUNT_REPLACE_VAL(lscan_wfg->scfg, laser->lperiod + LASER_PERIOD_MARGIN );

   /// LD_CTRL[0] = SH1 (WFG0[0])
   lscan_wfs->wave_ld_sel0 = WFS_WAVE_LD_SEL0_WFG0_REPLACE_VAL(0x0, 0x1 );

   /// LD_CTRL[1] = SH2 WFG0[1]
   lscan_wfs->wave_ld_sel1 = WFS_WAVE_LD_SEL1_WFG0_REPLACE_VAL(0x0, 0x2 );   

   lscan_hsync->hcfg = HSYNC_HCFG_WFG_ENABLE_REPLACE_VAL(lscan_hsync->hcfg, 0x3 );  // enable the wave form generators


   lscan_wfg->scfg = WFG0_SCFG_MODE_REPLACE_VAL(lscan_wfg->scfg, 0x0 );                ///< disabled (reset)
   lscan_wfg->scfg = WFG0_SCFG_MODE_REPLACE_VAL(lscan_wfg->scfg, 0x2 );                ///< sync to HSyncIn (enable)
   
   laser->spin_state = SYNCING_HSYNC; //always change state before enabling timer.
 
   laser->laser_timer = delay_msec_timer_non_blocking(100, dec_laser_timer_complete_callback_func, laser, false);
   return( true );
}

bool dec_filter_laser_mirror(dec_laser_t * laser)
{
    WFS_REGS_t *lscan_wfs = laser->lscan_wfs;
    HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;
    WFG0_REGS_t *lscan_wfg = laser->wfs->lscan_wfg;

    DBG_ERR("%s\n", __func__);

    DBG_ASSERT (laser->spin_state == SYNCING_HSYNC); 

    lscan_wfs->out_sel = WFS_OUT_SEL_FORCE_ON_REPLACE_VAL(lscan_wfs->out_sel, 0x00 );       ///< enable ForceLaserOn wave

    lscan_hsync->hcfg = HSYNC_HCFG_PULSE_QUAL_REPLACE_VAL(lscan_hsync->hcfg, 0x1 );         ///< filter/qual with WFG HSync_en
    lscan_wfg->scfg = WFG0_SCFG_RECOV_REPLACE_VAL(lscan_wfg->scfg, 0x1 );                   ///< insert missing HSYNC
    lscan_wfg->scfg = WFG0_SCFG_COUNT_REPLACE_VAL(lscan_wfg->scfg, laser->lperiod + LASER_PERIOD_MARGIN );  ///< HSYNC missing period
 
    laser->spin_state = SYNCING_FILTER;
    laser->laser_timer = delay_msec_timer_non_blocking(1, dec_laser_timer_complete_callback_func, laser, false);

    return( true );
}

bool dec_laser_spin_phase_exec(dec_laser_t *laser, uint32_t phase) 
{
    DBG_ASSERT(laser != NULL);

    if ( phase == SPINNING_MIRROR ) 
    {
        return ( dec_start_laser_mirror(laser) ); ///<  Starts the mirror spinning 
    }
    else if (phase == CONTINUE_SPINNING_MIRROR) 
    {
        return(dec_continue_spinning_mirror(laser)); ///<  Continue to wait for the mirror to spin up to speed
    }
    else if (phase == ENABLE_LASER ) 
    {
        return ( dec_enable_laser_mirror(laser) ); //< Inits the laser
    }
    else if ( phase == SYNCING_HSYNC ) 
    {
        return ( dec_hsync_laser_mirror(laser) ); ///< Syncs the mirror with hsync
    }
    else if ( phase == SYNCING_FILTER ) 
    {
        return ( dec_filter_laser_mirror(laser) ); ///< Fills up the hsync filter
    }
    else 
    {
        DBG_ERR("laser %x, phase %d \n",laser, phase);
        DBG_ASSERT(0);///< Bad phase
        dec_stop_laser_mirror(laser);
        return(false);
    }
}

void dec_enable_vdo(dec_laser_t *laser, bool on_notoff)
{ 
    WFS_REGS_t *lscan_wfs = laser->lscan_wfs;
    HSYNC_REGS_t *lscan_hsync = laser->lscan_hsync;

    if (on_notoff)
    {
      lscan_wfs->wave_en_sel0 = 
            WFS_WAVE_EN_SEL0_WFG0_REPLACE_VAL(0x0, 0x10);   ///< enables the vdo enable output bit. 
      lscan_wfs->wave_en_sel1 = 
            WFS_WAVE_EN_SEL1_WFG0_REPLACE_VAL(0x0, 0x10);   ///< enables the vdo enable output bit.            
      // lscan_hsync->hcfg =  HSYNC_HCFG_ENABLE_REPLACE_VAL(lscan_hsync->hcfg, 0xf);                                                 
    }
    else
    {
        lscan_wfs->wave_en_sel0 = 0;                        ///< disables the vdo enable output bit
        lscan_wfs->wave_en_sel1 = 0;                        ///< disables the vdo enable output bit
	lscan_hsync->hcfg =  HSYNC_HCFG_ENABLE_REPLACE_VAL(lscan_hsync->hcfg, 0x0);
    }
        
}

void dec_stop_laser_mirror(dec_laser_t *laser)
{ 
    WFS_REGS_t *lscan_wfs;
    MIRROR0_REGS_t* lscan_mirror = laser->mirror->lscan_mirror;
    error_type_t err;

    lscan_wfs = laser->lscan_wfs;
    
    DBG_ERR("%s\n", __func__);
        
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
   lscan_wfs->data_sel = WFS_DATA_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->data_sel, 0x003 );  // SH1 on; SH2 on
   lscan_wfs->out_sel = WFS_OUT_SEL_LD_CTRL_REPLACE_VAL(lscan_wfs->out_sel,    0x003 );  // latch SH lines.

   err = gpio_write( laser_enable, 0); ///< disable laser
   ASSERT(err == OK);

   // turn off mirror motor start signal
   lscan_mirror->mcfg2 = MIRROR0_MCFG2_GALVON_IDLE_REPLACE_VAL(lscan_mirror->mcfg2, 0x1 ); ///< Start=1 (active low)

   //TODO: need to make delay configurable?
   posix_sleep_ms(100); // delay for 0.1s before stopping clock after we toggle start line

   // turn off mirror motor clock
   lscan_mirror->mcfg2 = MIRROR0_MCFG2_GALVOP_DIS_REPLACE_VAL(lscan_mirror->mcfg2, 0x1 ); ///< disable Clk

   sem_post(&laser->complete_sem); // Do this last to avoid task switching
}

void dec_laser_local_init(void)
{
   APB_CONFIG_REGS_t *apbconfig = NULL;
   uint32_t lperiod; 
   uint32_t lmotor_period;
   uint32_t sh1_high, sh1_low; 
   uint32_t sh2_high, sh2_low;
   error_type_t err;

   DBG_ERR("%s\n", __func__);  

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

#define NS_TO_DEC_CLOCK_CYCLES(ns_val) ((ns_val)*DEC_CLOCK_FREQ_MHZ/1000)
#define NUM_MIRROR_FACETS 4

#define LPERIOD_IN_NS          	289012//512845//288612      // HSYNC Period.  Determined through measurement w/ oscilliscope.
#define LMOTOR_PERIOD_IN_NS     288696//512345//288296      // for output frequency of 3.46864717kHz
#define SH1_HIGH_PERIOD_IN_NS   271180//492345//270180      // Documentation specifies 284400, but that value causes hsync to lose lock
                                            // This value is determined through experimentation and is 95% of documented value.

   lperiod = NS_TO_DEC_CLOCK_CYCLES(lperiod_in_ns);   ///< period of a mirror face in clock pulses
   lmotor_period = NS_TO_DEC_CLOCK_CYCLES(lmotor_period_in_ns);

   // SH1 transitions from low to high at the start of nHSYNC and remains high for 284.4us
   sh1_high = 0;
   sh1_low = NS_TO_DEC_CLOCK_CYCLES(sh1_high_period_in_ns); 

   // SH2 transitions from high to low 4us after nHSYNC and remains low for 4us
   sh2_low = NS_TO_DEC_CLOCK_CYCLES(sh2_clock_low); //4000 //14000
   sh2_high = NS_TO_DEC_CLOCK_CYCLES(sh2_clock_high); //8000 //28000

   mylasers[0] = &my_lasers[0];

   mywfss[0] = &my_wfss[0];
   mywfss[0]->wfs_num = 0;
   mywfss[0]->lscan_wfg = dec_laser_get_lscan_wfg_regs(0);

   mymirrors[0] = &my_mirrors[0];
   mymirrors[0]->mirror_num = 0;
   mymirrors[0]->lscan_mirror = dec_laser_get_lscan_mirror_regs(0);
   mymirrors[0]->min_spin_time = 4100;
   mymirrors[0]->wd_spin_time = mymirrors[0]->min_spin_time*2;

   mylasers[0]->next_laser = NULL;
   mylasers[0]->laser_num = 0;
   mylasers[0]->mirror = &my_mirrors[0];
   mylasers[0]->wfs = &my_wfss[0];

   mylasers[0]->lscan_wfs = dec_laser_get_lscan_wfs_regs();
   mylasers[0]->lscan_hsync = dec_laser_get_lscan_hsync_regs();
   mylasers[0]->lperiod = lperiod;
   mylasers[0]->start_lperiod = 2000 * DEC_CLOCK_FREQ_MHZ; ///<2 ms 
   mylasers[0]->spin_state = IDLE;

   mylasers[0]->enable_force_laser = 0;
   mylasers[0]->enable_video_gating = 0;

   err = gpio_open( &laser_enable,
                        GPIO_BANK_E,
                        15,
                        GPIO_DIRECTION_OUTPUT,
                        GPIO_LOGIC_SETTING_ACTIVE_LOW,
                        0 );
   ASSERT(err == OK); //This should never fail.

   // HSYNC configuration
   init_lscan_hsync_hcfg(mylasers[0]->lscan_hsync,
                         0,   ///< use internal GalvoRtoL
                         0x0, ///<  disable WFG[3:0]
                         0x2, ///<  pulse on falling HSync
                         0x0, ///<  no qual (for now)
                         0x0, ///<  no qual (for now)
                         0x0, ///<  active low HSyncIns
                         0x0, ///<  active low HSyncOuts
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
                     0x4,                 ///<  4 facets
                     mylasers[0]->lperiod
                     );

   init_mirror_mcfg2(mylasers[0]->mirror->lscan_mirror,
                          0x3, ///<  disable GalvoRtoL
                          0x1, ///<  disable nStart
                          0x1, ///<  disable Clk
                          0x0, ///<  idle GalvoRtoL=0
                          0x1, ///<  idle Start=1
                          0x0, ///<  idle Clk = 0
                          0x0, ///<  nReady_LSU
                          0x0, ///<  ignored open-loop
                          0x0, ///<  ignored open-loop
                          0x0  ///<  ignored open-loop
                          );
   mylasers[0]->mirror->lscan_mirror->lockmax = lperiod * 4 + 77;  ///<  max period (4 facets) to consider locked
   mylasers[0]->mirror->lscan_mirror->lockmin = lperiod * 4 - 77;  ///<  min period (4 facets) to consider locked
                                                                   ///<  77 = 80ns*240/1000 * 4
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
                      lmotor_period/2 + 10,
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
                 0x0,   ///<  wait for sync
                 0x0,   ///<  unused
                 0x0,   ///<  ignore GalvoRtoL
                 0x0,   ///<  use HSyncIn[0]
                 mylasers[0]->lperiod + LASER_PERIOD_MARGIN     ///<  overflow count
                 );
   /**
    * Each wfs has 8 general use wavelets (0 to 7) and 1
    * hsync wavelet(8) and one calibration wavelet.  Each wavelet
    * can fire every N (nSync) cycles.  For some engines, they fire
    * every nSync.  Each wavelet can have an initial state of high
    * or low.  Each wavelet can have up to two state changes, at
    * action1x.period and action2x.period (where x is the wavelet
    * number) clock ticks from nsync.  For more complicated waves,
    * multiple wavelets, can be or'd together to get the desired
    * wave.
    */
   /// WFG0[0] = SH1 (LD_Ctrl[0])
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period0, 
                  0x0, 0x1 ///<  Return to idle low every cycle 
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action10, 
                  WAVEFORM_ACTION_HIGH, sh1_high ///<  set high @ 0us 
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action20,
                  WAVEFORM_ACTION_LOW, sh1_low   ///<  set low @ 284.4us
                  );

   /// WFG0[1] = SH2 (LD_Ctrl[1])
   set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period1, 
                  0x1, 0x1 ///<  Return to idle high every cycle 
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action11, 
                  WAVEFORM_ACTION_LOW, sh2_low ///<  set low @ 4us 
                  );
   set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action21,
                  WAVEFORM_ACTION_HIGH, sh2_high   ///<  set high @ 8us
                  );

   /// Cal request (unused)
   mylasers[0]->wfs->lscan_wfg->period9 = 0;
   mylasers[0]->wfs->lscan_wfg->action19 = 0;
   mylasers[0]->wfs->lscan_wfg->action29 = 0;

}

void dec_laser_config_change(unsigned char mode)
{
	uint32_t lperiod; 
	uint32_t lmotor_period;
	uint32_t sh1_high, sh1_low; 
	uint32_t sh2_high, sh2_low;
   
#define SPEED_25PPM 0
#define SPEED_45PPM 1
#define SPEED_40PPM 2
#define SPEED_36PPM 3
#define SPEED_32PPM 4
#define NS_TO_DEC_CLOCK_CYCLES(ns_val) ((ns_val)*DEC_CLOCK_FREQ_MHZ/1000)
#define NUM_MIRROR_FACETS 4

	if(mode == SPEED_25PPM)
	{
		lperiod_in_ns = 501316;
		lmotor_period_in_ns = 501316;
		sh1_high_period_in_ns = 472645;
		sh2_clock_high = 28000;
		sh2_clock_low = 14000;
	}
	else if(mode == SPEED_32PPM)
	{
		lperiod_in_ns = 419181;
		lmotor_period_in_ns = 419181;
		sh1_high_period_in_ns = 379181;
		sh2_clock_high = 8000;
		sh2_clock_low = 4000;
	}
	else if(mode == SPEED_36PPM)
	{
		lperiod_in_ns = 364081;
		lmotor_period_in_ns = 364081;
		sh1_high_period_in_ns = 324081;
		sh2_clock_high = 8000;
		sh2_clock_low = 4000;
	}
	else if(mode == SPEED_40PPM)
	{
		lperiod_in_ns = 318581;
		lmotor_period_in_ns = 317581;
		sh1_high_period_in_ns = 278581;
		sh2_clock_high = 8000;
		sh2_clock_low = 4000;
	}
	else
	{
		lperiod_in_ns = 289012;
		lmotor_period_in_ns = 288696;
		sh1_high_period_in_ns = 249012;
		sh2_clock_high = 8000;
		sh2_clock_low = 4000;
	}
	
	lperiod = NS_TO_DEC_CLOCK_CYCLES(lperiod_in_ns);   ///< period of a mirror face in clock pulses
	lmotor_period = NS_TO_DEC_CLOCK_CYCLES(lmotor_period_in_ns);
	
	// SH1 transitions from low to high at the start of nHSYNC and remains high for 284.4us
	sh1_high = 0;
	sh1_low = NS_TO_DEC_CLOCK_CYCLES(sh1_high_period_in_ns); 
	
	// SH2 transitions from high to low 4us after nHSYNC and remains low for 4us
	sh2_low = NS_TO_DEC_CLOCK_CYCLES(sh2_clock_low); //4000 //14000
	sh2_high = NS_TO_DEC_CLOCK_CYCLES(sh2_clock_high); //8000 //28000

	mylasers[0]->lperiod = lperiod;	
	
	mylasers[0]->mirror->lscan_mirror->mcfg1 = MIRROR0_MCFG1_CONTROL_REPLACE_VAL(mylasers[0]->mirror->lscan_mirror->mcfg1, mylasers[0]->lperiod );
	
	mylasers[0]->mirror->lscan_mirror->lockmax = lperiod * 4 + 77;  ///<  max period (4 facets) to consider locked
	mylasers[0]->mirror->lscan_mirror->lockmin = lperiod * 4 - 77;  ///<  min period (4 facets) to consider locked
																   ///<  77 = 80ns*240/1000 * 4
									
	mylasers[0]->mirror->lscan_mirror->galvop_fall0 = MIRROR0_GALVOP_FALL0_SET_REPLACE_VAL(mylasers[0]->mirror->lscan_mirror->galvop_fall0, lmotor_period/2 + 10);
	
	mylasers[0]->wfs->lscan_wfg->scfg = WFG0_SCFG_COUNT_REPLACE_VAL(mylasers[0]->wfs->lscan_wfg->scfg, mylasers[0]->lperiod + LASER_PERIOD_MARGIN);

	/**
	* Each wfs has 8 general use wavelets (0 to 7) and 1
	* hsync wavelet(8) and one calibration wavelet.  Each wavelet
	* can fire every N (nSync) cycles.  For some engines, they fire
	* every nSync.  Each wavelet can have an initial state of high
	* or low.  Each wavelet can have up to two state changes, at
	* action1x.period and action2x.period (where x is the wavelet
	* number) clock ticks from nsync.  For more complicated waves,
	* multiple wavelets, can be or'd together to get the desired
	* wave.
	*/
	/// WFG0[0] = SH1 (LD_Ctrl[0])
	set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period0, 
				  0x0, 0x1 ///<  Return to idle low every cycle 
				  );
	set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action10, 
				  WAVEFORM_ACTION_HIGH, sh1_high ///<  set high @ 0us 
				  );
	set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action20,
				  WAVEFORM_ACTION_LOW, sh1_low   ///<  set low @ 284.4us
				  );
	
	/// WFG0[1] = SH2 (LD_Ctrl[1])
	set_wfs_period(&mylasers[0]->wfs->lscan_wfg->period1, 
				  0x1, 0x1 ///<  Return to idle high every cycle 
				  );
	set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action11, 
				  WAVEFORM_ACTION_LOW, sh2_low ///<  set low @ 4us 
				  );
	set_wfs_action(&mylasers[0]->wfs->lscan_wfg->action21,
				  WAVEFORM_ACTION_HIGH, sh2_high   ///<  set high @ 8us
				  );
}
