/*
 * ============================================================================
 * Copyright (c) 2010, 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/// \file dec_laser_priv.h

#ifndef DEC_LASERPRIV_H
#define DEC_LASERPRIV_H

#include "posix_ostools.h"
#include "delay_api.h"
#include "gpio_api.h"
#include "key_table.h"
#include "dec_laser.h"
#include "logger.h"
#include "devices_logger.h"

#define DBG_PRFX "LASER: "                      // simple string prepend to the front of all strings.
//#define LOGGER_SUBMOD 0x800              // (0x1 << 0x11) submodule/channel is this bit, if set then log.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | DEVICES_LOGGER_SUBMOD_DEC_LASER

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)          // LOG_ERR is the level 
#define DBG_DEBUG(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 
#define DBG_LOG_ERR(...) DBG_MEMLOG(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_DEBUG(...) DBG_MEMLOG(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 


void dec_laser_local_init();


typedef enum dec_laser_state_e 
{   
    IDLE ,
    SPINNING_MIRROR,
    CONTINUE_SPINNING_MIRROR,
    ENABLE_LASER,
#if 1
    ENABLE_LASER2,
    ENABLE_LASER3,
    ENABLE_LASER4,
#endif
    SYNCING_HSYNC,
    SYNCING_FILTER,
    DO_ALL, //< Used for testing
    READY_FOR_TOP
}dec_laser_state_t;


typedef struct dec_wavegen_s
{
    // dec_laser_t *laser;
    uint32_t wavegen_num;
    WAVEGEN_REGS_t *wavegen;
    uint16_t lut_value_on;
} dec_wavegen_t;

typedef struct dec_mirror_s
{
    // dec_laser_t *laser;
    uint32_t mirror_num;
    MIRROR0_REGS_t *lscan_mirror;
    uint32_t min_spin_time;         ///< expected spinup ramp time in ms
    uint32_t wd_spin_time;          ///< spinup watchdog timeout in ms
} dec_mirror_t ;

typedef struct dec_wfs_s
{
    // dec_laser_t * laser;
    // dec_mirror_t * mirror;
    uint32_t wfs_num;
    WFG0_REGS_t *lscan_wfg; 
} dec_wfs_t;



struct dec_laser_s 
{
    dec_laser_t *next_laser;  ///< Multiple Lasers may be linked
    uint32_t laser_num;      
    dec_mirror_t *mirror;     
    dec_wfs_t *wfs;
    dec_wavegen_t *wavegen;
    WFS_REGS_t *lscan_wfs;    //singular
    HSYNC_REGS_t *lscan_hsync; //singular
    uint32_t lperiod;
    uint32_t start_lperiod;
    //gpio_handle_t *lockdet0_pin; ///< LS_lockdet[0] input 
    delay_msec_timer_t *  laser_timer;
    delay_msec_timer_t *  laser_WD_timer;
    dec_laser_state_t spin_state;
    dec_laser_state_t next_phase;
    sem_t complete_sem;
    bool lock_det;

    uint32_t enable_force_laser;    ///< set to 1 if engine uses force laser feature to enable in HW; else 0
    uint32_t enable_video_gating;   ///< set to 1 if video is gated by dec HW VDOEnable registers to enable HW; else 0
};

void init_lscan_hsync_hcfg(HSYNC_REGS_t *lscan_hsync, uint32_t en, uint32_t wfg_enable, uint32_t pulse_type, uint32_t pulse_qual,
                           uint32_t hsync_qual, uint32_t in_pol, uint32_t out_pol, uint32_t enable);

void init_lscan_hsync_filter(HSYNC_REGS_t *lscan_hsync,
                             uint32_t fall_hi, uint32_t fall_lo, 
                             uint32_t rise_hi, uint32_t rise_lo, 
                             uint32_t hfilt_thres);

void init_lscan_hsync_timers(HSYNC_REGS_t *lscan_hsync,
                             uint32_t tcfg1, uint32_t tcfg2,
                             uint32_t event01, uint32_t event02, uint32_t event03, uint32_t event04,
                             uint32_t event05, uint32_t event06, uint32_t event07, uint32_t event08,
                             uint32_t event09, uint32_t event10, uint32_t event11, uint32_t event12,
                             uint32_t event13, uint32_t event14, uint32_t event15, uint32_t event16
                             );

void init_mirror_mcfg1(MIRROR0_REGS_t *lscan_mirror, 
                       uint32_t mode, uint32_t hsync_sel, uint32_t facets, uint32_t control);

void init_mirror_mcfg2(MIRROR0_REGS_t *lscan_mirror,
                       uint32_t galvor_dis, uint32_t galvon_dis, uint32_t galvop_dis, uint32_t galvor_idle,
                       uint32_t galvon_idle, uint32_t galvop_idle, uint32_t lock_pol, uint32_t recov,
                       uint32_t max_adj, uint32_t min_adj);

void init_mirror_galv_sync(MIRROR0_REGS_t *lscan_mirror,
                           uint32_t enable0, uint32_t set0,
                           uint32_t enable1, uint32_t set1);

void init_mirror_galv_wfg(MIRROR0_REGS_t *lscan_mirror,
                           uint32_t enable0, uint32_t set0,
                           uint32_t enable1, uint32_t set1);

void init_mirror_galvo_rtol(MIRROR0_REGS_t *lscan_mirror,
                            uint32_t rise0, uint32_t fall0,
                            uint32_t rise1, uint32_t fall1);

void init_mirror_galvop(MIRROR0_REGS_t *lscan_mirror,
                        uint32_t rise0_enable, uint32_t rise0_set, uint32_t fall0_enable, uint32_t fall0_set,
                        uint32_t rise1_enable, uint32_t rise1_set, uint32_t fall1_enable, uint32_t fall1_set);

void init_mirror_galvon(MIRROR0_REGS_t *lscan_mirror,
                        uint32_t rise0_enable, uint32_t rise0_set, uint32_t fall0_enable, uint32_t fall0_set,
                        uint32_t rise1_enable, uint32_t rise1_set, uint32_t fall1_enable, uint32_t fall1_set);

void init_mirror_hsync_en0(MIRROR0_REGS_t *lscan_mirror,
                           uint32_t rise0_enable, uint32_t rise0_set, uint32_t fall0_enable, uint32_t fall0_set,
                           uint32_t rise1_enable, uint32_t rise1_set, uint32_t fall1_enable, uint32_t fall1_set);

void init_mirror_hsync_en1(MIRROR0_REGS_t *lscan_mirror,
                           uint32_t rise0_enable, uint32_t rise0_set, uint32_t fall0_enable, uint32_t fall0_set,
                           uint32_t rise1_enable, uint32_t rise1_set, uint32_t fall1_enable, uint32_t fall1_set);

void init_wfs_scfg(WFG0_REGS_t *lscan_wfg,
                   uint32_t mode, uint32_t recov, 
                   uint32_t sync_sel, uint32_t galvo_sel, uint32_t hsync_sel, 
                   uint32_t count);

void set_wfs_period(volatile uint32_t * period_reg, uint32_t idle, uint32_t period);

void set_wfs_action(volatile uint32_t *action_reg, uint32_t action, uint32_t count);

void dec_laser_internal_phase_complete( void *at_phase_complete_data, uint32_t next_phase );

void dec_laser_spin_complete( dec_laser_t *laser, dec_laser_state_t state  );

void set_wavegen_DAC (dec_laser_t * laser, uint16_t value);

void dec_laser_timer_complete_callback_func(void *data);

bool dec_laser_spin_phase_exec(dec_laser_t *laser, uint32_t phase);

void init_Lscan (dec_laser_t * laser);

/// int key - string - object mapping, from dec_key_table
/// returned object is not owned so don't delete it.
/// int key - string - object mapping, from key_table.h
/// object_by_index
/// object_by_name
/// name_by_index
/// index_by_name  
//KEY_TABLE_DECLARE_PUBLIC_2( dec_mirror, dec_mirror_t );

/// int key - string - object mapping, from dec_key_table
/// returned object is not owned so don't delete it.
/// int key - string - object mapping, from key_table.h
/// object_by_index
/// object_by_name
/// name_by_index
/// index_by_name  
//KEY_TABLE_DECLARE_PUBLIC_2( dec_wfs, dec_wfs_t );

/// int key - string - object mapping, from dec_key_table
/// returned object is not owned so don't delete it.
/// int key - string - object mapping, from key_table.h
/// object_by_index
/// object_by_name
/// name_by_index
/// index_by_name  
//KEY_TABLE_DECLARE_PUBLIC_2( dec_wavegen, dec_wavegen_t );




#define WAVEFORM_ACTION_DISABLED    0x0
#define WAVEFORM_ACTION_HIGH        0x1
#define WAVEFORM_ACTION_LOW         0x2
#define WAVEFORM_ACTION_TOGGLE      0x3

#define hsync_timer_restart 0
#define hsync_timer_pulse0 0x1
#define hsync_timer_pulse1 0x2
#define hsync_timer_pulse2 0x3
#define hsync_timer_pulse3 0x4

#endif
