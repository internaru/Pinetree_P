/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *  @file eth_mac_api.h
 *  
 *  @brief Ethernet MAC device driver API
 * 
 *  This API presents an abstracted top-down view of an Ethernet MAC device driver. The API hides
 *  details of a particular Ethernet MAC hardware device. Ethernet MAC device drivers that
 *  conform to this API can be integrated into the system with minimal impact on overlying
 *  network layers.
 */

#ifndef ETH_MAC_API_H
#define ETH_MAC_API_H

#include <stdint.h>
#include <stdbool.h>
#include "error_types.h"

/**
 * @name Ethernet MAC return codes
 * 
 * @note error code values 0 through -10 are reserved in error_types.h
 */
typedef int32_t eth_mac_rcode_t;
#define ETH_MAC_OK              OK
#define ETH_MAC_FAIL            FAIL
#define ETH_MAC_HW_ERR          -11 ///< Hardware error or device unresponsive.
#define ETH_MAC_BAD_ADDR        -12 ///< Invalid MAC address.
#define ETH_MAC_NO_BUFS         -13 ///< Unable to acquire user buffer.
#define ETH_MAC_BAD_BUF         -14 ///< Invalid user buffer.
#define ETH_MAC_BAD_PARAM       -15 ///< Invalid function parameter value.
#define ETH_MAC_BAD_STATE       -16 ///< Operation cannot be performed in current context state
#define ETH_MAC_INTERNAL_ERR    -17 ///< Internal logic error detected.
#define ETH_MAC_NOT_PERMITTED   -18 ///< Operation not permitted at this time or under existing circumstances.
#define ETH_MAC_TIMEOUT         -19 ///< Operation timed out
//@}


/**
 * @brief Receive buffer flag bitmap
 *
 * Use the ETH_MAC_RX_FLAG_X flags to access the bit fields of this uint32_t.  The device
 * driver uses these flags to further qualify the receive buffers passed to the user via
 * eth_mac_accept_rx_buffer().
 */
typedef uint32_t eth_mac_rx_flags_t;

/**
 * @name Receive buffer flags
 *
 * Use these ETH_MAC_RX_FLAG_X symbolic constants to access the bit fields of 
 * eth_mac_rx_buffer_flags_t. 
 */

/**
 * @brief Indicates that this buffer completes the current Ethernet frame. 
 *  
 * If the ETH_MAC_RX_FLAG_IS_PARTIAL flag is not set also, then the buffer contains the 
 * entire Ethernet frame. 
 */
#define ETH_MAC_RX_FLAG_IS_LAST                     0x0001

/**
 * @brief Indicates that the buffer comprises a partial Ethernet frame. 
 *  
 * It is normal to receive a partial frame anytime an incoming frame is larger than the 
 * available receive buffer. To avoid partial frames, make sure the buffers provided via 
 * the eth_mac_get_user_buffer_func_t callback are at least as large as the maximum frame 
 * size (e.g. 1518 bytes for standard Ethernet frames). 
 *  
 * The user can assume that partial frames will arrive in order, and that the
 * ETH_MAC_RX_FLAG_IS_LAST flag will be set on the final buffer of the frame. 
 *  
 * It is also possible to receive a partial frame indicator if the frame EOF is somehow 
 * damaged or lost in transit over the Ethernet medium. 
 */
#define ETH_MAC_RX_FLAG_IS_PARTIAL                  0x0002

//@}

/**
 * @brief Transmit buffer flag bitmap
 *
 * Use the ETH_MAC_TX_FLAG_X flags to access the bit fields of this uint32_t.
 */
typedef uint32_t eth_mac_tx_flags_t;

/**
 * @name Transmit buffer flags
 *
 * Use these ETH_MAC_TX_FLAG_X symbolic constants to access the bit fields of 
 * eth_mac_tx_buffer_flags_t. 
 */

/**
 * @brief Indicates that this buffer completes the current Ethernet frame. 
 */
#define ETH_MAC_TX_FLAG_IS_LAST                     0x0001

//@}

/**
 * @brief Ethernet link configuration method
 *
 * The method by which the Ethernet link will attempt to configure itself and acquire link status
 * (e.g. auto-negotiation vs. forced speed and duplex).
 */
typedef enum 
{
    ETH_MAC_LINK_CONFIG_AUTO, ///< Auto-negotiate speed/duplex.
    ETH_MAC_LINK_CONFIG_10_HALF, ///< Force 10 Mbps half duplex.
    ETH_MAC_LINK_CONFIG_10_FULL, ///< Force 10 Mbps full duplex.
    ETH_MAC_LINK_CONFIG_100_HALF, ///< Force 100 Mbps half duplex.
    ETH_MAC_LINK_CONFIG_100_FULL, ///< Force 100 Mbps full duplex.
    ETH_MAC_LINK_CONFIG_1G_HALF, ///< Force 1 Gbps half duplex.
    ETH_MAC_LINK_CONFIG_1G_FULL ///< Force 1 Gbps full duplex.
} eth_mac_link_config_t;


/**
 * @brief Ethernet link status
 * 
 * Current Ethernet link status including link up/down state and *resolved* PHY speed and duplex. A
 * link status other than ETH_MAC_LINK_STATUS_DOWN implies that the link is "up" and can communicate
 * over the attached network at the indicated speed and duplex mode.
 */
typedef enum
{
    ETH_MAC_LINK_STATUS_DOWN, ///< Device has not acquired link status.
    ETH_MAC_LINK_STATUS_10_HALF, ///< 10 Mbps half duplex link is established.
    ETH_MAC_LINK_STATUS_10_FULL, ///< 10 Mbps half duplex link is established.
    ETH_MAC_LINK_STATUS_100_HALF, ///< 100 Mbps half duplex link is established.
    ETH_MAC_LINK_STATUS_100_FULL, ///< 100 Mbps half duplex link is established.
    ETH_MAC_LINK_STATUS_1G_HALF, ///< 1 Gbps half duplex link is established.
    ETH_MAC_LINK_STATUS_1G_FULL ///< 1 Gbps half duplex link is established.
} eth_mac_link_status_t;

// rx error counters
// total of all counters represents number of invalid rx frames that have been received
// exactly one counter is incremented for every bad rx frame.
typedef struct
{
    uint32_t late_collision;
    uint32_t giant_frame;
    uint32_t crc_err; // Ethernet frame CRC err
    uint32_t chksum_offload_err; // TCP, UDP, or ICMP6 checksum error
    uint32_t ip_chksum_offload_err; // IP checksum error
    uint32_t internal_err;
} eth_mac_rx_errors_t;

// rx statistics
// "good_frames" plus "bad_frames" equals the total number of rx frames received by the device
// "good_frames" minus "ucast_frames" minus "bcast_frames" equals total good multicast frames received
typedef struct
{
    uint32_t                good_frames;
    uint32_t                bad_frames;
    uint32_t                ucast_frames; // good unicast frames
    uint32_t                bcast_frames; // good broadcast frames
    eth_mac_rx_errors_t     errors; // sum of all "errors" counters equals "bad_frames"
    uint32_t                bytes; // bytes received in good frames not including Ethernet CRC bytes
} eth_mac_rx_stats_t;

// tx error counters
typedef struct
{
    /** 
     *  @brief Late Tx Collision Counter
     * 
     *  Indicates that frame transmission was aborted due to a collision occurring after the
     *  collision window (later than one slotTime from the start of the packet transmission).
     *  A late collision is counted twice, i.e., both as a collision and as a lateCollision.
     *  Valid only in Half-Duplex mode, always zero otherwise. Corresponds to 802.3-2008
     *  Section 2 Clause 30.3.1.1.10 aLateCollisions attribute.
     */
    uint32_t late_collision;

    /**
     *  @brief Excessive Collision Counter
     * 
     *  Indicates that frame transmission was aborted after too many successive collisions.
     *  The "collision_cnt" member of eth_mac_tx_stats_t struct indicates number of
     *  collisions.  Valid only in Half-Duplex mode, always zero otherwise. Corresponds to
     *  802.3-2008 Section 2 Clause 30.3.1.1.11 aFramesAbortedDueToXSColls attribute.
     */
    uint32_t excess_collision;

    /**
     *  @brief Internal MAC Tx Error 
     * 
     *  Indicates that frame transmission was aborted due to an internal MAC sublayer error.
     *  The most common internal error is transmit underflow, for example when the GMAC DMA
     *  encounters an empty Tx buffer while transmitting the frame. If this flag is set then
     *  none of the other Tx error flags will be set. Corresponds to 802.3-2008 Section 2
     *  Clause 30.3.1.1.12 aFramesLostDueToIntMACXmitError attribute.
     */
    uint32_t internal_err;

    /**
     *  @brief Bad Carrier Sense
     * 
     *  Indicates that the carrier sense signal from the PHY was either not asserted or was
     *  deasserted for one or more tx clock periods during the transmission of a frame. Valid
     *  only when in Half-Duplex mode and the frame is transmitted without collision, always
     *  zero otherwise. Corresponds to 802.3-2008 Section 2 Clause 30.3.1.1.13
     *  aCarrierSenseErrors attribute.
     */
    uint32_t bad_carrier;

    /** 
     *  @brief Excessive Deferral
     * 
     *  Indicates that frame transmission was aborted due to deferral timeout. Note that
     *  deferral begins when the transmitter is ready to transmit, but is prevented because of
     *  an active CRS (carrier sense) signal on the GMII/MII. Defer time is not cumulative. If
     *  the transmitter defers, then transmits, collides, backs off, and then has to defer
     *  again after completion of back-off, the deferral timer resets to 0 and restarts. Valid
     *  only in Half-Duplex mode, always zero otherwise. Corresponds to 802.3-2008 Section 2
     *  Clause 30.3.1.1.20 aFramesWithExcessiveDeferral attribute.
     */
    uint32_t excess_deferral;

    /** 
     *  @brief Tx Jabber Timeout
     * 
     *  Indicates that frame transmission was aborted due to a jabber timeout, meaning the
     *  transmitter remained active for too long while attempting to transmit the frame. Valid
     *  only in Half-Duplex mode. Corresponds to 802.3-2008 Section 2 Clause 30.5.1.1.6
     *  aJabber attribute.
     */
    uint32_t jabber_timeout;

    /** 
     *  @brief Tx Checksum Offload Error Counter
     * 
     *  Indicates that frame transmission was either aborted entirely or that the frame may
     *  have been sent with an invalid IP, TCP, UDP, or ICMP checksum. This flag is set
     *  following detection of a possible error or other anomoly during checksum calculation
     *  by the MAC hardware layer, such that a checksum either could not be generated or
     *  may be invalid. The flag is valid only when Tx checksum offload is supported and
     *  enabled, always zero otherwise.
     */
    uint32_t chksum_offload_err;

} eth_mac_tx_errors_t;

// tx statistics
typedef struct
{
    /** 
     *  @brief Good frames counter
     *  
     *  Represents the number of tx frames that are known to have been transmitted
     *  successfully.  "good_frames" plus "bad_frames" equals the total number of tx frames
     *  the device has attempted to transmit.
     */
    uint32_t            good_frames;

    /** 
     *  @brief Bad frames counter
     *  
     *  Represents the number of tx frames that have failed or may have failed. A summary of
     *  tx error frames, i.e. the sum of all tx error counters.
     */
    uint32_t            bad_frames;

    /** 
     *  @brief Unicast frames counter
     */
    uint32_t            ucast_frames;

    /** 
     *  @brief Broadcast frames counter
     */
    uint32_t            bcast_frames;

    /** 
     *  @brief Error counters
     *  
     *  Exactly one counter is incremented for every tx frame that has failed or may have
     *  failed.
     */
    eth_mac_tx_errors_t errors;

    /** 
     *  @brief Bytes successfully transmitted 
     */
    uint32_t            bytes;

    /** 
     *  @brief Deferred Counter
     * 
     *  Indicates the number of frames for which transmission was delayed on first attempt
     *  because the medium was busy, i.e. an external carrier was detected on the medium.
     *  Valid only in Half-Duplex mode. Corresponds to 802.3-2008 Section 2 Clause
     *  30.3.1.1.9 aFramesWithDeferredXmissions attribute.
     */
    uint32_t            deferred;

    /** 
     *  @brief Collision Counter
     *  
     *  Number of collisions that occurred while attempting to transmit frames. The
     *  Collision Counter is incremented once for every collision that occurs prior to
     *  successful frame transmission.  Note however that only excessive collisions or a
     *  late collision will cause frame transmission to be aborted. Whenever the Late
     *  Collision Counter is incremented, the Collision Counter is also incremented. Whenever
     *  the Excessive Collisions Counter is incremented, the Collision Counter is incremented
     *  by the number of collisions that occurred before frame transmission was aborted. Valid
     *  only in Half-Duplex mode.
     */
    uint32_t            collision;

} eth_mac_tx_stats_t;


/*
 * convenience macros for aligning buffer sizes to dcache line size
 */

#define ETH_MAC_ROUND_UP(size, align) \
        ((((size) + (align - 1)) / align) * align)

#define ETH_MAC_ROUND_DOWN(size, align) \
        (((size) / align) * align)

#define ETH_MAC_ROUND_UP_PTR(ptr, align) \
        (((((uint32_t)ptr) + (align - 1)) / align) * align)

#define ETH_MAC_ROUND_DOWN_PTR(ptr, align) \
        ((((uint32_t)ptr) / align) * align)


/* 
 *  eth_mac_get_user_buffer_func_t
 * 
 *  This user-defined callback function is registered by the user via eth_mac_init() and
 *  is called by the MAC driver to acquire buffers for storing incoming Ethernet frames.
 * 
 *  Buffer memory acquired via this callback is owned by the MAC driver until it is
 *  released back to the user.  After receiving notification from the driver that incoming
 *  Ethernet frames are pending, the user calls eth_mac_get_rx_buf() to reclaim ownership
 *  of one or more buffers. The driver may also relinquish buffers that are either empty
 *  or that contain discarded data via the user's registered
 *  eth_mac_free_user_buf_func_t() callback.
 * 
 *  The user may associate user-defined data with the buffer that will be returned upon
 *  transfering ownership back to the user.
 * 
 *  notes:
 * 
 *      Buffer sizes less than the max Rx frame size (e.g. 1518 bytes) may result in
 *      incoming Ethernet frames that are divided across multiple buffers.
 * 
 *      IMPORTANT: The driver will automatically flush corresponding system cache lines
 *      prior to DMAing any data into the buffer. It is therefore critical that the buffer
 *      provided by this callback owns whole cache lines that are aligned on cache line
 *      boundaries. The buffer itself need not be aligned. However, in this case the user
 *      must add pad bytes around the buffer as necessary to guarantee ownership of an
 *      integer multiple of cache lines aligned on cache line boundaries.
 * 
 *  params:
 * 
 *      len (out) - buffer size in bytes.  Must be aligned to dcache line size.
 * 
 *      source_id (out) - User-defined source ID for tracking buffer origin, or for
 *      further qualifing the user_data param.
 * 
 *      user_data (out) - Opaque user-defined data that the user wishes to
 *          associate with this buffer.
 * 
 *  returns:
 * 
 *      Pointer to user buffer, or NULL if no buffers are available. A return of NULL may
 *      result in dropped Rx packets. The driver will automatically flush corresponding
 *      system cache lines prior to DMAing any data into the buffer
 */
typedef char *(*eth_mac_get_user_buffer_func_t)(uint32_t *len, uint32_t *source_id, void **user_data);

/* 
 *  eth_mac_free_user_buffer_func_t
 * 
 *  This user-defined callback function is registered by the user via eth_mac_init() and
 *  is called by the MAC driver to relinquish ownership of empty buffers or buffers
 *  containing discarded or otherwise invalid data.
 * 
 *  params:
 * 
 *      buf (in) - Pointer to the empty user buffer being released back to the user.
 * 
 *      source_id (in) - User-defined source ID for tracking packet origin (e.g. network
 *      stack vs. layer-2 raw socket), or for further qualifing the user_data param.
 * 
 *      user_data (in) - Opaque user-defined data associated with this buffer.
 * 
 *  returns:
 * 
 *      none
 */
typedef void (*eth_mac_free_user_buffer_func_t)(char *buf, uint32_t source_id, void *user_data);

/**
 * @brief Ethernet MAC link handle
 *
 * Device handle generated by eth_mac_open() used to reference the physical MAC device in 
 * successive API calls. 
 */
typedef struct eth_mac_context_s *eth_mac_handle_t;

/* 
 *  optional callback - If not populated then a polling mechanism must be used to detect a
 *  change in link status change, e.g. via repeated calls to eth_mac_get_link_status() on
 *  a timer trigger. All user does in this callback is break context e.g. via a msg queue
 *  so that a call to eth_mac_get_link_status() can be made.
 *  
 *  Note: This callback may be called from the context of an ISR. The user MUST break
 *  context prior to blocking execution or making any calls that could indirectly bock
 *  execution. Processing within the callback prior to breaking context must be kept to a
 *  minimum.
 */ 
typedef void (*eth_mac_isr_notify_link_func_t)(void *user_data);

/* 
 *  Note: This callback may be called from the context of an ISR. The user MUST break
 *  context prior to blocking execution or making any calls that could indirectly bock
 *  execution. Processing within the callback prior to breaking context must be kept to a
 *  minimum.
 */ 
typedef void (*eth_mac_isr_notify_frame_func_t)(uint16_t rx_frames, uint16_t tx_frames,
                                                uint32_t rx_bytes, uint32_t tx_bytes,
                                                uint32_t flags, void *user_data);

typedef struct 
{
    eth_mac_get_user_buffer_func_t      get_user_buf; ///< Populate with a user buffer allocation routine (required).
    eth_mac_free_user_buffer_func_t     free_user_buf; ///< Populate with a user buffer relinquish routine (required).
    eth_mac_isr_notify_frame_func_t     isr_notify_frame; ///< Populate with a frame notification callback (required).
    eth_mac_isr_notify_link_func_t      isr_notify_link; ///< Populate with a link status notification callback (optional), or NULL if none.
} eth_mac_func_table_t;


/*
 *  eth_mac_init()
 * 
 *  Performs a software reset of the underlying Ethernet hardware and initializes device
 *  registers and driver context to a known-good state.
 * 
 *  Blocks until device reset and driver initialization have completed or until reset
 *  timeout or other initializtion failure.
 * 
 *  params:
 * 
 *  phys_addr - Pointer to physical address (e.g. 6-byte MAC address).  Bytes are copied
 *  so user can free mem upon return.
 * 
 *  rx_buf_pool_max_bytes - Size of the receive buffer pool in bytes.
 * 
 *  tx_buf_queue_max_slots - Number of slots to allocate in driver's transmit queue.
 * 
 * 
 * 
 *  returns:
 * 
 *      'true' on success, or 'false' on failure.
 */
eth_mac_rcode_t eth_mac_init(uint8_t dev_index, uint8_t *phys_addr, uint8_t len,
                             eth_mac_link_config_t link_config, 
                             eth_mac_func_table_t *func_table, void *user_data,
                             uint32_t rx_buf_pool_max_bytes, uint32_t tx_buf_queue_max_slots);

/* 
 *  eth_mac_open()
 * 
 *  Enables MAC interrupts and places the MAC driver in a running state. While in this
 *  state the driver will process Rx and Tx Ethernet frames when the PHY has link up
 *  status.
 * 
 *  During this call the MAC driver will attempt to allocate an Rx buffer pool through a
 *  series of calls to the user's registered eth_mac_get_user_buf_func_t callback. This Rx
 *  buffer pool helps accomodate sudden bursts of Rx traffic and delays in user calls to
 *  eth_mac_accept_rx_buffer(). The driver will continue to acquire user buffers until the
 *  collective size of the Rx buffer pool is at least as large as the value indicated by
 *  parameter 'rx_buf_pool_max_bytes'.
 * 
 *  If an Rx frame arrives while the buffer pool is exhausted, the MAC driver will discard
 *  the frame and increment the missed frame counter. A large number of missed frames may
 *  indicate a need for the user to increase the value of the 'rx_buf_pool_max_bytes'
 *  parameter.
 * 
 *  The MAC driver will also create a Tx buffer queue initialized with parameter
 *  'tx_buf_queue_max_slots' number of slots. While no buffers are pre-allocated for the
 *  Tx buffer queue, there is a small amount of overhead required per slot (e.g. ~5 bytes,
 *  however the exact amount may vary across MAC driver implemenations). The Tx buffer
 *  queue helps to accomodate sudden bursts of Tx traffic and reduces the number of
 *  time-consuming Tx stalls that occur in device hardware following Tx buffer underruns.
 * 
 *  The number of slots allocated to the Tx queue determines the maximum number of
 *  outstanding Tx buffers that can exist. A Tx buffer is considered outstanding while it
 *  is in possession of the the MAC driver, after the user calls
 *  eth_mac_insert_tx_buffer() but before the user calls eth_mac_complete_tx_buffer() for
 *  the corresponding buffer.
 * 
 *  When the driver's Tx buffer queue becomes full, any further calls to
 *  eth_mac_insert_tx_buffer() will fail and the Tx buffer referenced by that call will
 *  remain the responsibility of the user. The user can assume another Tx queue slot has
 *  been freed following a subsequent user call to eth_mac_complete_tx_buffer().
 * 
 *  params:
 * 
 * 
 * 
 *  returns:
 * 
 *      'true' on success, or 'false' on failure.
 */
eth_mac_rcode_t eth_mac_open(uint8_t dev_index);

/* 
 *  eth_mac_close()
 * 
 *  Disables MAC interrupts and places the MAC driver in a stopped state. 
 * 
 *  The driver will repeatedly call the user-defined eth_mac_free_user_buffer_func_t
 *  callback function to relenquish back to the user all Tx buffers pending in the
 *  driver's Tx buffer queue, any empty buffers in the Rx buffer pool, and any Rx buffers
 *  with payload awaiting processing by the ISR or pending in the post-ISR receive queue.
 * 
 *  params:
 * 
 *      none
 * 
 *  returns:
 * 
 *      reference count (a non-zero value means the device has remained open for other
 *      clients/users sharing this device)
 */
uint8_t eth_mac_close(uint8_t dev_index);

/*
 *  eth_mac_accept_rx_buffer()
 *  
 *  Accept ownership of an incoming Rx buffer from the driver.
 *  
 *  If the user's registered eth_mac_isr_notify_frame_func_t callback is called with an
 *  "rx_frames" parameter value greater than zero, the user must call this routine to
 *  accept ownership of an Rx buffer that has been populated with received Ethernet
 *  payload data. Ownership of the Rx buffer memory was originally transfered from the
 *  user to the driver via the user's registered eth_mac_get_user_buffer_func_t callback.
 *  
 *  notes:
 * 
 *      The user MUST NOT call this routine from the eth_mac_isr_notify_frame_func_t callback as
 *      that routine may have been called from the context of an ISR.
 * 
 *  params:
 * 
 *      len (out) - Pointer to storage to be populated with the number of bytes of payload
 *      data stored in the rx buffer.  This value does not refer to the size of the buffer
 *      itself.
 * 
 *      flags (out) - Pointer to storage to be populated with one or more flags which
 *      further qualify the rx buffer.
 * 
 *      stats (out) - Pointer to storage allocated by caller used to pass back rx frame
 *      statistics including any errors that occurred during frame reception. To  
 *      facilitate cumulative statistics gathering, any existing counts in the object are 
 *      incremented without resetting them to zero. If reset is desired, it is the user's
 *      responsibility to set the counters to zero prior to the call. The statistics
 *      counters are updated only when ETH_MAC_RX_FLAG_IS_LAST is set in the "flags"
 *      parameter passed back to the caller.
 * 
 *      source_id (out) - Pointer to storage to be populated with the user-defined source
 *      ID for tracking buffer origin (e.g. network stack vs. layer-2 raw socket), or
 *      for further qualifing the user_data param. Use NULL if don't care about this
 *      value.
 * 
 *      user_data (out) - Pointer to storage to be populated with the opaque user-defined
 *      data value associated with this buffer. Use NULL if don't care about this value.
 * 
 *  returns:
 * 
 *      Pointer to a user buffer containing a received Ethernet frame (or part of a frame
 *      if ETH_MAC_RX_FLAG_IS_PARTIAL is set in the "flags" parameter).  NULL if no more
 *      Rx buffers are pending.
 */
char *eth_mac_accept_rx_buffer(uint8_t dev_index, uint32_t *len, eth_mac_rx_flags_t *flags,
                               eth_mac_rx_stats_t *stats, uint32_t *source_id, void **user_data);

/*
 *  eth_mac_insert_tx_buffer()
 * 
 *  Inserts a user-allocated Tx buffer into the MAC driver's send queue.
 *  
 *  On successful return, buffer memory is owned by the driver until transmission is
 *  either successfully completed or aborted.  The user will be notified of Tx completion
 *  via the registered eth_mac_isr_notify_frame_func_t callback with a "tx_frames"
 *  parameter value greater than zero.  The user must then break context from the callback
 *  (possibly called from an ISR) and reclaim ownership of Tx buffer memory by calling
 *  eth_mac_complete_tx_buffer() once for each "tx_frames" count.  If transmission is
 *  aborted by the MAC driver, buffer ownerwhip is transferred back to the user via the
 *  user's registered eth_mac_free_user_buf_func_t() callback.
 * 
 *  The user may associate an opaque user-defined data object with the buffer.  The user
 *  data will be returned by the driver when relinquishing ownership of the buffer back to
 *  the user.
 * 
 *  The user must delimit Ethernet frames by setting the 'is_last' flag to 'true' on the
 *  last buffer of a frame. Scatter send is supported by setting 'is_last' to 'false'
 *  until the last buffer of frame.
 * 
 *  Any alignment of buffer data required by the MAC hardware is handled by the driver
 *  itself.
 * 
 *  params:
 * 
 *      data - Pointer to buffer containing the data to be transmitted.  IMPORTANT: buffer
 *      size must be aligned to dcache line size, and buffer memory must be aligned to
 *      cache line boundary.
 * 
 *      len - Number of bytes of data contained in the buffer.  
 * 
 *      is_last - Indicates whether or not this is the last buffer of the frame.
 * 
 *      source_id - User-defined source ID for tracking buffer origin (e.g. network stack
 *      vs. layer-2 raw socket), or for further qualifing the user_data param.
 * 
 *      user_data - Opaque user-defined data to associate with this buffer.
 *      
 *  returns:
 * 
 *      ETH_MAC_OK on success.
 */
eth_mac_rcode_t eth_mac_insert_tx_buffer(uint8_t dev_index, char *data, uint32_t len, bool is_last,
                                         uint32_t source_id, void *user_data);

/*
 *  eth_mac_complete_tx_buffer()                                                        
 *  
 *  Reclaim ownership of Tx buffer memory following transmission of an Ethernet frame.
 *  
 *  If the user's registered eth_mac_isr_notify_frame_func_t callback is called with a
 *  "tx_frames" parameter value greater than zero, the user must call this routine once
 *  for every "tx_frames" count in order to reclaim ownership of Tx buffer memory
 *  containing the transmitted Ethernet payload data, thereby completing packet
 *  transmission. Ownership of the Tx buffer memory was originally transfered from the
 *  user to the driver via a call to eth_mac_insert_tx_buffer().
 *                                                                                        
 *  notes:                                                                                
 * 
 *      The user MUST NOT call this routine from the eth_mac_isr_notify_frame_func_t
 *      callback as that routine may have been called from the context of an ISR.
 *                                                                                        
 *  params:                                                                               
 *                                                                                        
 *      len (out) - Populated with the number of bytes of payload data contained in the Tx
 *      buffer.  This value does not refer to the size of the buffer itself.
 *                                                                                        
 *      flags (out) - Pointer to storage to be populated with one or more flags which
 *      further qualify the tx buffer.
 * 
 *      stats (out) - Pointer to storage allocated by caller used to pass back tx frame
 *      statistics including any errors that occurred while attempting to transmit
 *      the frame. To facilitate cumulative statistics gathering, any existing counts in
 *      the object are incremented without resetting them to zero. If reset is desired, it
 *      is the user's responsibility to set the counters to zero prior to the call. The
 *      statistics counters are updated only when ETH_MAC_TX_FLAG_IS_LAST is set in the
 *      "flags" parameter passed back to the caller.
 * 
 *      source_id (out) - Pointer to storage to be populated with the user-defined source
 *      ID for tracking buffer origin (e.g. network stack vs. layer-2 raw socket), or
 *      for further qualifing the user_data param. Use NULL if don't care about this
 *      value.
 * 
 *      user_data (out) - Pointer to storage to be populated with the opaque user-defined
 *      data value associated with this buffer. Use NULL if don't care about this value.
 *                                                                                        
 *  returns:                                                                              
 *                                                                                        
 *      Pointer to a user buffer containing a transmitted Ethernet frame, or part of a    
 *      frame depending on the value of the 'is_last' flag. NULL if the Tx buffer        
 *      completion queue is empty or driver is not open.                                 
 */
char *eth_mac_complete_tx_buffer(uint8_t dev_index, uint32_t *len, eth_mac_tx_flags_t *flags,
                                 eth_mac_tx_stats_t *stats, uint32_t *source_id, void **user_data);
/*
 *  Poll for current link status.
 *  
 *  The user should call this routine following each callback of the user's registered
 *  eth_mac_isr_notify_link_func_t callback function. If the
 *  eth_mac_isr_notify_link_func_t callback is not used then the user should call this
 *  routine at regular intervals to poll for changes in link status.
 *  
 *  The user should call this routine at least once following registration for link status
 *  notification in order to determine initial link status.
 *  
 *  Note:  The user MUST NOT call this routine from the eth_mac_isr_notify_link_func_t
 *  callback as that routine may have been called from the context of an ISR.
 */ 
eth_mac_link_status_t eth_mac_get_link_status(uint8_t dev_index);


eth_mac_rcode_t eth_mac_get_phys_addr(uint8_t dev_index, uint8_t *phys_addr, uint8_t len);

// must be called by user whenever link config options change
eth_mac_rcode_t eth_mac_apply_config_options(uint8_t dev_index, eth_mac_link_config_t link_config);


// add mac addr to filter list
// TODO use perfect filter registers first then use hash - currently non-masked addrs are always added via hash
eth_mac_rcode_t eth_mac_filter_add(uint8_t dev_index, uint8_t *phys_addr, uint8_t len, uint32_t flags);

// remove from perfect filter list
// removing a single address from hash bitmask is not allowed due to possibility of overlapped addrs 
eth_mac_rcode_t eth_mac_filter_remove(uint8_t dev_index, uint8_t *phys_addr, uint8_t len);

// currently only clears hash table
// WARNING: filters can originate from multiple sources, e.g. Treck, layer-2 raw sockets, etc.
eth_mac_rcode_t eth_mac_filter_remove_all(uint8_t dev_index);

// masked addr filters must use perfect filter mechanism
// locked - filter can't be removed by eth_mac_filter_remove_all() -- removed only via eth_mac_filter_remove()
eth_mac_rcode_t eth_mac_filter_add_masked(uint8_t dev_index, uint8_t *phys_addr, uint8_t len,
                                          uint8_t byte_mask, bool locked);



typedef enum
{
    ETH_MAC_PWR_MODE_D0, // Fully On
    ETH_MAC_PWR_MODE_D1, // Intermediate1 - reduced power (full network stack but LEDs off, etc.)
    ETH_MAC_PWR_MODE_D2, // Intermediate2 - active off network proxy (micro-network stack)
    ETH_MAC_PWR_MODE_D3, // Off - device powered off and unresponsive to its bus
} eth_mac_pwr_mode_t;

eth_mac_rcode_t eth_mac_set_power_mode(eth_mac_pwr_mode_t pwr_mode, uint32_t flags, void *data);

#endif // ETH_MAC_API_H

//eof
