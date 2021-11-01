/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_UAP_H
#define NET_UAP_H

#include <stdint.h>
#include <stdbool.h>

#ifdef HAVE_POWER_MGR

/* Target average power
 * 
 * The system will attempt to maintain the moving average at this power level by
 * dynamically adjusting the target duty cycle each sample period. The closer
 * NET_LP_TARGET_AVG_PWR_MW is to NET_LP_IOWAKE_PWR_MW the more maintainable the power
 * level will be.  Conversely, the closer NET_LP_TARGET_AVG_PWR_MW is to
 * NET_LP_SLEEP_PWR_MW the more likely we are to exceed the power budget during times of
 * increased activity.
 *
 */
#define NET_LP_TARGET_AVG_PWR_MW    750

/* Power measured while in the sleep state.
 */
#define NET_LP_SLEEP_PWR_MW         550

/* Power measured while in the I/O wake state.
 */
#define NET_LP_IOWAKE_PWR_MW        1100

#if (NET_LP_TARGET_AVG_PWR_MW <= NET_LP_SLEEP_PWR_MW)
    #error "A NET_LP_TARGET_AVG_PWR_MW value less than NET_LP_SLEEP_PWR_MW cannot be achieved!"
#endif 

/* Average power period
 * 
 * Time over which power will be averaged by the moving average function. The selected
 * value should large enough to prevent overreaction to temporary spikes in power
 * consumption, while not exceeding the time period over which low power conformance
 * authorities (e.g. Blue Angel or Energy Star) will measure average power.
 * 
 * An overreaction can cause an I/O wake processing blackout, resulting in lost packets or
 * suttering responsiveness in user experience.   
 */
#define NET_LP_AVG_POWER_PERIOD_MS      30000

/* Sample period for the smoothed average power function.
 * 
 * Determines the frequency at which discrete samples will be taken for the exponential
 * moving average function. For an accurate moving average, this value should be at least
 * 2-3 times larger than NET_LP_MAX_WAKE_LATENCY_MS but significanty smaller than
 * NET_LP_AVG_POWER_PERIOD_MS.  Setting the value too small relative to
 * NET_LP_MAX_WAKE_LATENCY_MS can skew average power due to large variance in the actual
 * sample period (samples can only be taken on sleep cycle boundaries).
 */
#define NET_LP_SAMPLE_PERIOD_MS     1000

/* Max wake latency
 * 
 * Max sleep time before forcing a wake. Assuming no other wake triggers, max latency is
 * chosen such that sleep time + computed wake time = sample period, which aligns with the
 * sample check we do before each sleep request from pwr mgr.
 */ 
#define NET_LP_MAX_WAKE_LATENCY_MS  200

#define NET_LP_MAX_WAKE_TIME_MS     5000

// > 100 = increase; < 100 = decrease
#define NET_LP_DEVIATION_WEIGHT     500

/* Minumum duty cycle
 * 
 * Forces a minimum wake time.  Purpose is to provide a catch-all for network protocols
 * that generate tx traffic in the absence of any corresponding rx traffic, such as lease
 * renewal for client apps like DHCP or service announcements for discovery protocols like
 * WS-Discovery or mDNS.
 * 
 * The logic will attempt to maximize up-time within bounds of NET_LP_TARGET_DUTY_CYCLE,
 * but the NET_LP_MIN_DUTY_CYCLE is guaranteed even if we are forced to exceed power
 * budget over the average power period defined by NET_LP_AVG_POWER_PERIOD_MS.
 */
#define NET_LP_MIN_DUTY_CYCLE       50 // (x1000, e.g. 50 => 0.05)

/* "Alpha" smoothing factor for exponential moving average function
 * 
 * Calculated as a function of the number of sample periods over which the moving average
 * is to be applied.  The average over this time period will be only an approximation
 * because in an exponential moving average the samples never actually expire. However,
 * from a practical point of view the weightings become insignificant after N samples.
 * 
 * * a = 2/(N+1)
 * * where:
 * *    'a' = Alpha smoothing factor used to compute smoothed_average_power.
 * *    'N' = Number of sample periods over which the moving average is taken. 
 */
#define NET_LP_SMOOTHING_FACTOR \
    ((2 * 1000) / ((NET_LP_AVG_POWER_PERIOD_MS / NET_LP_SAMPLE_PERIOD_MS) + 1))

/* Target wake time
 * 
 * I/O wake time per sample_period required to achieve target_average_power: 
 * 
 * * d = (A - s) / (q - s)
 * * where:
 * *    'A' = average power
 * *    's' = power consumed while in the sleep state,
 * *    'q' = power consumed while in the I/O quick wake state, 
 * *    'd' = the I/O quick wake duty cycle
 * 
 * * d = tq/tt
 * * where:
 * *    'tq' = quick wake time
 * *    'tt' = total time (quick wake time + sleep time)
 * 
 * Substituting for 'd' and solving for 'tq' yields:
 * 
 * * tq = tt(A - s) / (q - s)
 */
#define NET_LP_TARGET_DUTY_CYCLE \
    (((NET_LP_TARGET_AVG_PWR_MW - NET_LP_SLEEP_PWR_MW) * 1000) / (NET_LP_IOWAKE_PWR_MW - NET_LP_SLEEP_PWR_MW))

#endif // HAVE_POWER_MGR

typedef struct
{
    bool msg_queue_initialized;

    // singleton MAC dev instance
    uint8_t dev_index;
    eth_mac_link_status_t prev_link_status;
    uint32_t tx_frames_complete_pending;

    net_link_event_notify_func_t link_event_notify_func;
    void *link_event_user_data;

    ttUserInterface treck_interface;
    OS_MUTEX mtx;

#ifdef HAVE_POWER_MGR

    eth_phy_handle_t phy_hndl; // TODO TEMP for pwr mgr
    pwr_mgr_level_t pwr_level; // power level at which Ethernet block is currently operating
    uint32_t pwr_mgr_uid;
    uint32_t pwr_mgr_event_flag;

// TEMP move to a platform-level API

    /* Exponential moving average of power over period NET_LP_AVG_POWER_PERIOD_MS
     * 
     * * for t=1, SA(1) = A(1)
     * * for t>1, SA(t+1) = a*A(t) + (1-a)*SA(t)
     * * where:
     * *    'SA' = smoothed average power
     * *    'A' = average power over the sample period
     * *    'a' = "alpha" smoothing factor, where 0 < a < 1. The higher the value,
     *      the more sensitive SA is to changes in A
     * 
     * 'A' is computed each sample_period NET_LP_SAMPLE_PERIOD_MS as follows:
     * 
     * * A = qd + s(1-d)
     * * where:
     * *    'A' = average power
     * *    's' = power consumed while in the Sleep state
     * *    'q' = power consumed while in the quick I/O wake state
     * *    'd' = the quick wake duty cycle
     */
    uint32_t    smoothed_average_power_mw;

    /* Target duty cycle for the current sample period.  This value will vary from
     * NET_LP_TARGET_DUTY_CYCLE by an amount proportionate to the deviation of
     * smoothed_average_power_mw from NET_LP_TARGET_AVG_PWR_MW
     */
    int32_t     target_duty_cycle;

    uint32_t    wake_initial_ticks; // threadx ticks at trans from FULL power into sleep/iowake mode
    uint32_t    sleep_initial_ticks; // sleep ticks at trans from FULL power into sleep/iowake mode
    uint32_t    wake_periodic_ticks; // threadx ticks at start of current time period 
    uint32_t    sleep_periodic_ticks; // sleep ticks at start of current time period 

    /* Number of average power samples taken since trans from FULL power into sleep/iowake
     * mode. Incremented on average once per NET_LP_SAMPLE_PERIOD_MS.
     */
    uint32_t    sample_cnt;

    uint32_t    max_sleep_latency; // TODO TEMP DEBUG
    uint32_t    prev_sleep_iterations; // TODO TEMP DEBUG

#endif // HAVE_POWER_MGR

} net_uap_context_t;

net_uap_context_t *net_uap_get_ctxt(void);

//=========================================================================================================================
// rdj TEMP - raw socket stuff - move to eth_mac module after completing net_eth and eth_mac module refactoring
//=========================================================================================================================

/**
 * \brief Callback functions take this form.
 *
 * \param ether_type The ethernet packet type (eg. LLTD = 0x88D9)
 */
typedef void (*net_uap_notify_callback_t)( uint16_t ether_type );


/**
 * \brief Register to be called when a packet of the type \a
 * protocol is received.
 * 
 * \param ether_type The ethernet packet type (eg. LLTD = 0x88D9)
 *
 * \return 0 on success
 * \return -1 on error
 */
error_type_t net_uap_notify_callback_register( uint16_t ether_type, net_uap_notify_callback_t callback );

/**
 * \brief send raw packets to the network driver, bypassing the
 * network stack.
 *
 * \param data_ptr
 * \param data_len
 *
 * \return 0 on success
 * \return -1 on error
 */
int uap_link_raw_send( char *data_ptr, int data_len );


#endif // NET_ETH_H

//eof
