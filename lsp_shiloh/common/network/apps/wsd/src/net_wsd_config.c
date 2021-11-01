/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_wsd_config.c
 *
 * \brief This file implements the public configuration API for
 * the WSD.
 * 
 **/

//--------------------------------------
// Included files
//--------------------------------------
#include "net_wsd_config.h"
#include "lassert.h"
#include "net_iface_vars.h"

//--------------------------------------
// Local Definitions and Types
//--------------------------------------

#define SYS_RESTORE_ENABLED_VAL true
#define PORT                    5357 /* for http connection */ //4499
#define DEFAULT_EVENT_RATE      1
#define TRANSACTIONS_PER_SOCKET 10
#define NUM_DISPATCHER_THREADS  15
#define JOB_HISTORY_ELEMENTS    5
#define MANUFACTURER_URL        "http://www.marvell.com"
#define SUPPORT_URL             "http://www.marvell.com/support/"

wsd_custom_func_table_t custom_func_table =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

//--------------------------------------
// Global Definitions
//--------------------------------------

//--------------------------------------
// Local Function Declarations
//--------------------------------------


//--------------------------------------
// Local Function Definitions
//--------------------------------------

void net_wsd_config( net_wsd_config_t *config )
{
    config->sys_restore_wsd_enabled     = SYS_RESTORE_ENABLED_VAL;
    config->port                        = PORT;
    config->default_event_rate          = DEFAULT_EVENT_RATE;
    config->transactions_per_socket     = TRANSACTIONS_PER_SOCKET;
    config->num_dispatcher_threads      = NUM_DISPATCHER_THREADS;
    config->num_job_history_elements    = JOB_HISTORY_ELEMENTS;
    config->manufacturer_url            = MANUFACTURER_URL;
    config->support_url                 = SUPPORT_URL;
    config->wsd_custom_func_table       = &custom_func_table;
}


