/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 *  CISX management code.
 *
 *  davep 19-Nov-2010
 */

#ifndef CISX_H
#define CISX_H

/* davep 29-Jan-2013 ; adding dual sensor support */
#ifdef HAVE_NSENSOR_SUPPORT
#define CISX_ODMA_NUM_CHANNELS 6
#define CISX_IDMA_NUM_CHANNELS 2
#else
#define CISX_ODMA_NUM_CHANNELS 3
#define CISX_IDMA_NUM_CHANNELS 1
#endif

/* interrupt name changes depending on your ASIC */
#ifndef INTNUM_CISX
#define INTNUM_CISX INTNUM_ICE_CISX
#endif

struct sensor_segment {
    const int num_dummy;
    const int num_data;
    const int append_flag;
};

/* two or three will be normal */
#define CISX_SENSOR_MAX_SEGMENTS 12

struct cisx_sensor_conf {
    int num_segments;
    struct sensor_segment segments[CISX_SENSOR_MAX_SEGMENTS];
    uint8_t chan_order;  /* one of CISX_3CHAN_ORDER_nnn */

    /* davep 25-Mar-2013 ; allow Cfg.cbiout_pace to be platform specific. 
     *
     * "CBI Output Pacing"
     *
     * VallidOut frequency = BusClkFreq/(cbiout_pace + 2) 
     * ValidOut frequency (max) = BusClkFreq/2
     */
    uint32_t cbiout_pace;
};

#define CISX_3CHAN_ORDER_012 0
#define CISX_3CHAN_ORDER_021 1
#define CISX_3CHAN_ORDER_102 2
#define CISX_3CHAN_ORDER_120 3
#define CISX_3CHAN_ORDER_201 4
#define CISX_3CHAN_ORDER_210 5

void cisx_dump( void );
void cisx_dma_dump( void );
void cisx_ddma_dump( void );
scan_err_t cisx_onetime_init( void );
scan_err_t cisx_cleanup_module( void );

uint32_t cisx_interrupt_disable( void );
void cisx_interrupt_enable( void );

void cisx_set_cmode( scan_cmode_t cmode );
void cisx_set_pixels( uint8_t channel, uint32_t num_dummy_pixels, uint32_t num_data_pixels, int append_flag );
void cisx_reset( void );
scan_err_t cisx_soft_setup( void );
void cisx_set_bypass( bool bypass );
void cisx_set_cbiout_pace( uint32_t cbiout_pace );
void cisx_sanity( void );
scan_err_t cisx_open( scan_cmode_t cmode, 
                      const struct cisx_sensor_conf *cisxsensor,
                      uint32_t sensor_bitmask);
void cisx_launch( void );
void cisx_close( void );
void cisx_ddma_data_peek( void );
bool cisx_get_dual_channel_enabled( void );

void cisx_odma_channel_reset( uint8_t channel );
void cisx_odma_channel_disable( uint8_t channel );
void cisx_odma_channel_enable( uint8_t channel );
void cisx_odma_channel_start( uint8_t channel );

void cisx_idma_channel_reset( uint8_t channel );
void cisx_idma_channel_disable( uint8_t channel );
void cisx_idma_channel_enable( uint8_t channel );
void cisx_idma_channel_start( uint8_t channel );

void cisx_get_pixels_per_row( uint32_t *ppr );
uint32_t cisx_get_int_pending( void );

#endif

