/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Net-SNMP Sub-Agent for Printer MIBs
 * Main Source Module
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <signal.h>

// #include "../include/sdhEngineTable_old.h"
#include "printer_mib_translation.h"
#include "prtGeneralTable.h"
#include "prtInputTable.h"
#include "prtMarkerTable.h"
#include "prtMarkerSuppliesTable.h"
#include "prtMarkerColorantTable.h"
#include "prtMediaPathTable.h"
#include "prtInterpreterTable.h"
#include "systemSystem.h"
#include "sdhEngGeneral.h"
#include "sdhEngStatus.h"
// #include "sdhEngineTable_old.h"
#include "sdhEngOld.h"
#include "sdhEngineTable.h"
#include "hrDeviceTable.h"
#include "hrPrinterTable.h"
#include "portMonitorTable.h"

/* printer-mib agent aka prtmib-subagent */
void spawn_snmp_subagent_listener(void);

static int keep_running;

// stop-handler for signals SIGINT SIGTERM
RETSIGTYPE stop_server(int a) {
    keep_running = 0;
    exit(a); // forward this signal
}


int32_t net_snmp_subagent_init(void)
{
    snmp_enable_stderrlog();

    /* make us a agentx client */
    netsnmp_ds_set_boolean(
        NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);

    /* initialize tcpip, if necessary */
    SOCK_STARTUP;

    /* initialize the agent library */
    init_agent("prtmib-subagent");

    /* initialize mib code here */
	init_systemSystem();
	init_hrDeviceTable();
	init_hrPrinterTable();
    init_prtGeneralTable();  
    init_prtInputTable();  
    init_prtMarkerTable();  
    init_prtMarkerSuppliesTable();  
    init_prtMarkerColorantTable();  
    init_prtMediaPathTable();  
    init_prtInterpreterTable(); 
	init_sdhEngGeneralTable();
	init_sdhEngStatusTable();
	// init_sdhEngineTable_old();
	init_sdhEngOld();
	init_sdhEngineTable();
	init_portMonitorTable();
    /* subagent will be used to read subagent.conf files */
    init_snmp("prtmib-subagent");


    /* In case we recevie a request to stop (kill -TERM or kill -INT) */
    keep_running = 1;
#if 0
    // what's the long term strategy for signal handling?
#else
    signal(SIGTERM, stop_server);
    signal(SIGINT, stop_server);
#endif


    /* populate the mib tables */
 	populate_hrDeviceTable(
                get_prtMIB_OIDMap(HOSTMIBTABLE_ID_HRDEVICE));
    populate_hrPrinterTable(
                get_prtMIB_OIDMap(HOSTMIBTABLE_ID_HRPRINTER));
    populate_prtGeneralTable(
                get_prtMIB_OIDMap(PRTMIBTABLE_ID_GENERAL));
    populate_prtInputTable(
                get_prtMIB_OIDMap(PRTMIBTABLE_ID_INPUT));
    populate_prtMarkerTable(
                get_prtMIB_OIDMap(PRTMIBTABLE_ID_MARKER));
    populate_prtMarkerSuppliesTable(
                get_prtMIB_OIDMap(PRTMIBTABLE_ID_MARKERSUPPLIES));
    populate_prtMarkerColorantTable(
                get_prtMIB_OIDMap(PRTMIBTABLE_ID_MARKERCOLORANT));
    populate_prtMediaPathTable(
                get_prtMIB_OIDMap(PRTMIBTABLE_ID_MEDIAPATH));
    populate_prtInterpreterTable(
                get_prtMIB_OIDMap(PRTMIBTABLE_ID_INTERPRETER));
    populate_sdhEngGeneralTable(
                get_prtMIB_OIDMap(SDH_ID_SDHENGGENERAL));
    // populate_sdhEngineTable_old(
                // get_prtMIB_OIDMap(SDH_ID_SDHENGINE_OLD));
    populate_sdhEngineTable(
                get_prtMIB_OIDMap(SDH_ID_SDHENGINE));
    // spawn printer's snmp subagent ..
    spawn_snmp_subagent_listener();

    return 0;
}

// app-interface -- transfer control from general snmp-startup -> agentx
void net_snmp_init(void)
{
    (void)net_snmp_subagent_init();
}

// create and spawn the snmp agentx() thread
#include <posix_ostools.h>

#define SNMP_LISTENER_STACK_SIZE POSIX_MIN_STACK_SIZE
static __attribute__ ((aligned (8))) char agentx_stack[SNMP_LISTENER_STACK_SIZE]; /* stack size */
pthread_t subagentx;

#include "oid_api.h"
#include "oid_producer_api.h"
#include "net_snmp_subagent_api.h" // printer mib/oid registration required

/**************************************************/
/** \brief Process snmp subagent requests:        */
void* snmp_subagent_task(void* unused)
{

    //oid_register_module_printermib();

    //snmp_log(LOG_INFO, "Net-SNMP Printer MIB Subagent is up and running.\n");
    snmp_log(LOG_INFO, "Printer-MIB SubagentX is running.\n");

    /* listen forever unless otherwise terminated .. */
    while (keep_running) {
        agent_check_and_process(1); /* 0 == don't block */
    }

    /* at shutdown time */
    snmp_shutdown("prtmib-subagent");
    SOCK_CLEANUP;

    return 0;
}

/**************************************************/
/** \brief Initialize the snmp_subagent() thread  *
 *                                                *
 */
void spawn_snmp_subagent_listener(void)
{
  /* create thread for fax context management */
  posix_create_thread(&subagentx,
                       snmp_subagent_task,
                       (void *) 1,
                       "snmp_subagentX",
                      &agentx_stack,
	              sizeof(agentx_stack),
                      POSIX_THR_PRI_NORMAL);
}

// eof
