/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// Supply general os/network interface

#ifndef _OS_NETWORK_INTERFACE_
#define _OS_NETWORK_INTERFACE_

#ifndef __linux__

// treck library interface
#include "trsocket.h"

#else // __linux__ variation

#include <stdint.h>
#include <netinet/in.h>
#include "posix_ostools.h" // posix shim layer

// local extern (revisit cast) (see 'man inet_ntop' (not <arpa/inet.h>))
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
// local extern (revisit cast) (see 'man inet_pton' (not <arpa/inet.h>))
int inet_pton(int af, const char *src, void *dst);
// local extern (revisit cast) (see 'man inet_addr' (not <arpa/inet.h>))
in_addr_t inet_addr(const char *cp);

#define MAX_IP_ADDR_STR_SIZE INET_ADDRSTRLEN /* (16) */

void init_network_vars(void); // register NETWORK vars debug (part of common?)

#define THR_PRI_NORMAL POSIX_THR_PRI_NORMAL

// threadx replacements;  modelled after threadx TX_ settings:

#define OS_4_ULONG         16 /* 4 ulong byte-size */
#define OS_2_ULONG         8 /* 2 ulong byte-size */

#define OS_SUCCESS         0
#define OS_INHERIT        -2
#define OS_QUEUE_FULL     -1 
#define OS_NO_WAIT         0 /*POSIX_NO_WAIT*/
#define OS_WAIT_FOREVER    POSIX_WAIT_FOREVER
#define OS_AUTO_START      0

#define DEFAULT_TIME_SLICE 100

// generic os structures (mapped to posix)
#define OS_THREAD pthread_t
#define OS_MUTEX  pthread_mutex_t
#define OS_QUEUE  mqd_t
#define OS_TIMER  timer_t

#define os_time_get posix_gettime_ticks
#define tx_time_get os_time_get

#define os_mutex_get(lock, ignored) pthread_mutex_lock(lock)
#define os_mutex_put(lock) pthread_mutex_unlock(lock)
#define os_mutex_delete(ignored)
#define os_queue_create posix_create_message_queue
#define os_queue_receive posix_wait_for_message
#define os_queue_send(q,buf,wait_opt) posix_message_send((mqd_t)q,(char*)buf,sizeof(buf),MQ_DEFAULT_PRIORITY,wait_opt)

// stubs for treck network library interface
#include "os_treck_stubs.h"

#endif // __linux__

#endif // _OS_NETWORK_INTERFACE_

//eof
