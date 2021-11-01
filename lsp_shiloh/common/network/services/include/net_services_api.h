/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef NET_SERVICES_API_H
#define NET_SERVICES_API_H

#include "net_services.h"

extern net_services_ctxt_t   g_net_services_ctxt;

void net_services_init(void);
void launch_net_services_backend(void);

error_type_t net_service_request(net_service_request_t, net_service_t, void* user_data, void* reply);
error_type_t net_service_workq(net_service_request_t, net_service_t, void* user_data, void* reply);

int initiate_net_services_timer(long delta);

// checkup service state vs enabled/disabled and hw-availability
//    where service is: mdns, wifi (wlan/mlan), uap, 'all'
error_type_t net_services_checkup(net_service_t service);

bool snmp_running(void); // true/false running/not-running -- os-query

// cmd 'net services'
#define NUM_TRACKED_REQUESTS 19 
void show_service_requests();
void show_net_services_activity();
bool track_request(int action, int service);

// misc debug
char* str_request(int action);
char* str_service(int service);
long timer_active(timer_t*); // returns ms remaining -- 0 means not running

#endif // NET_SERVICES_API_H

//eof
