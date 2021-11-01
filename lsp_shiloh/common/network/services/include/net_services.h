/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef NET_SERVICES_H
#define NET_SERVICES_H

#include <stdint.h>
#include <stdbool.h>

#include "net_api.h" // api collection
#include "os_network.h"
#include "error_types.h"

#define NET_SERVICE_MQ_NAME "/net-services-msgq"
#define NET_SERVICE_MQ_NUM_MSG_BUFS 8
#define NET_SERVICE_MQ_MSG_NUM_BUFS 8
#define NET_SERVICE_MSGQ_BUF_BYTES OS_4_ULONG

// services
typedef enum {
    NET_SERVICE_UNAVAILABLE = 0,
    NET_SERVICE_SNMP = 1,
    NET_SERVICE_DHCP = 2,
    NET_SERVICE_MDNS = 3,
    NET_SERVICE_STA  = 4, // station aka mlan/wlan
    NET_SERVICE_WLAN = NET_SERVICE_STA, // aka wifi
    NET_SERVICE_MLAN = NET_SERVICE_STA, // aka wifi
    NET_SERVICE_UAP,
    NET_SERVICE_IFCONFIG,
    NET_SERVICE_UDHCP,
    NET_SERVICES_ALL,
    NET_SERVICES_NUMBER = NET_SERVICES_ALL // last entry
} net_service_t;

// service request
typedef enum {
    NET_SERVICES_IDLE  = 0,
    NET_SERVICE_CONFIG = 1, // NET_SERVICES_CONFIG,
    NET_SERVICE_STOP   = 2, // NET_SERVICES_STOP,
    NET_SERVICE_START  = 3, // NET_SERVICES_START,
    NET_SERVICE_RENEW  = 4, // stop regardless, (re)start if enabled
    NET_SERVICE_STATUS,
    NET_SERVICE_NUM_MSGS,
    NUM_SERVICE_ACTIONS_ALL = NET_SERVICE_NUM_MSGS, // last entry
} net_service_request_t;

typedef enum { // bit field
    NET_SERVICES_INIT   = 0,
    NET_SERVICES_BUSY   = 1,
    NET_SERVICES_CONFIG = 1<<NET_SERVICE_CONFIG, // 1
    NET_SERVICES_START  = 1<<NET_SERVICE_START,  // 2
    NET_SERVICES_STOP   = 1<<NET_SERVICE_STOP,   // 3
    NET_SERVICES_BOUND  = 1<<NET_SERVICE_RENEW,  // 4
    NET_SERVICES_STATES_NUMBER // last entry
} net_services_state_t;



#if 1 // unguarded

#define add_service_state(service, req) g_net_services_ctxt.state |=  (service<<16 | req)
#define clr_service_state(service, req) g_net_services_ctxt.state &= ~(service<<16 | req)

#else // guarded
#define add_service_state(service, req) \
    NET_SERVICE_CONTEXT_LOCK(); \
    g_net_services_ctxt.state |= (service<<8|req); \
    NET_SERVICE_CONTEXT_UNLOCK();

#define clr_service_state(service, req) \
    NET_SERVICE_CONTEXT_LOCK(); \
    g_net_services_ctxt.state &= ~(service<<8|req); \
    NET_SERVICE_CONTEXT_UNLOCK();
#endif

// net services current state
typedef struct net_services_ctxt_s
{
    bool enabled;
    net_services_state_t state;
} net_services_ctxt_t;


// net service msg request
typedef struct net_service_msg_s
{
    net_service_request_t request;
    net_service_t         service;
    void*                 udata;
    void*                 reply;
} net_service_msg_t;

// bundle a msg with it's start/stop times
typedef struct ns_packet_req_s
{
    net_service_msg_t req;
    long start_time;
    long finish_time;
} ns_packet_req_t;

typedef union {
    net_service_msg_t msg;
    char buf[sizeof(net_service_msg_t)];
} net_service_msg_buf_t;


// misc traffic counters
extern long g_net_service_all_req_counter;
extern long g_net_service_folded_req_counter;
extern long g_all_workq_req_counter;
extern long g_folded_workq_req_counter;
extern int  g_current_req;
extern      ns_packet_req_t g_log_req[];

#endif // NET_SERVICES_H

//eof
