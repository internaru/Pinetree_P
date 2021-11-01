/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /*  net_link_enum.h
  * 
  *  This file contains configuration info for the net_link module.
  * 
  *  Any values specified in this file will override the default values in the common
  *  net_link_config_dflt.c configuration file.
  */


#ifndef _NET_LINK_ENUM_HEADER
#define _NET_LINK_ENUM_HEADER

#define LINK_DOWN_NOTIFY_DELAY_MSEC         2200
/*  When a link-down event occurs on a bound and configured link, the netdrvr state machine will
 *  delay closing the interface for LINK_DOWN_NOTIFY_DELAY_MSEC milliseconds.  If the link
 *  comes back up before the timeout then the interface will remain in the configured
 *  state. Otherwise the interface will be closed.  The delay helps to filter out link
 *  up/down glitches that would otherwise trigger a lengthy reconfiguration sequence.
 */

#define LINK_UP_NOTIFY_DELAY_SEC            2
/*  When a lower priority link-up event occurs and no links are currently bound to Treck interface,
 *  the link event handler will delay LINK_UP_NOTIFY_DELAY_SEC seconds before generating a link-up
 *  message. The delay helps avoid race conditions that would otherwise result in link up/down
 *  glitches when multiple links are enabled simultaneously (e.g. during boot up), and a lower
 *  priority link acquires link status faster than a higher priority link.
 */

#define EXCLUSIVE_LINK_ENABLE_DELAY_SEC     (LINK_UP_NOTIFY_DELAY_SEC+2)
/*  When a higher priority link is up, lower priority links that are registered as "exclusive" are
 *  disabled, e.g. in an effort to conserve power.  When a higher prioriy link goes down, the
 *  netdrvr state machine will delay EXCLUSIVE_LINK_ENABLE_DELAY_SEC seconds before sending an
 *  "Enable" notification to underlying link drivers.  The delay helps avoid enable/disable glitches
 *  caused when a higher priority link goes down and back up in short period of time.
 */


// IMPORTANT: link state logic relies on the order in which states are defined below
// IMPORTANT: after modifying this table, must also modify g_net_link_state_strs below
typedef enum 
{
    NET_LINK_STATE_DOWN_DELAY,
    NET_LINK_STATE_DOWN_PENDING,
    NET_LINK_STATE_DOWN,
    NET_LINK_STATE_UP_DELAY,
    NET_LINK_STATE_UP,

    // add new states above this point
    NET_LINK_NUM_STATES
} link_state_t;

typedef enum
{
    NET_LINK_MSG_UP_EVENT,
    NET_LINK_MSG_UP_POLL,       // poll for link-up status independent of any event
    NET_LINK_MSG_UP,            // a 'link-up' event was received from link driver
    NET_LINK_MSG_DOWN_EVENT,
    NET_LINK_MSG_DOWN_PENDING,
    NET_LINK_MSG_DOWN,
    NET_LINK_MSG_RESET_EVENT,
    NET_LINK_MSG_ENABLE_LINKS,
    NET_LINK_MSG_CHECK_LINKS, // periodic

    // add new msgs above this point
    NET_LINK_NUM_MSGS
} net_link_msg_type_t;

#endif // _NET_LINK_ENUM_HEADER
