/*
 * ============================================================================
 * Copyright (c) 2008-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file main_threadx.c
 * 
 * \brief Main task controlling the supplicant.
 * 
 * Primary link between system and the supplicant code. Creates 
 * the main execution thread and controls startup/shutdown of 
 * the supplicant. 
 * 
 **/

#include "tx_api.h"
#include "logger.h"   //SS added to resolve the minVsprintf

#include "includes.h"
#include "common.h"
#include "wpa_supplicant_i.h"
#include "eloop.h"
#include "stddef.h"
#include "lassert.h"
#include "platform.h"
#include "os.h"
#include "wlanInterface.h"
#include "wifi_intf_api.h"
#include "raw_socket_api.h"

// TODO: Figure out how to get this synched with the header files that define it
typedef struct {
    UINT8 SSID[MAX_SSID_NAME_LEN];
    int WPAEncryptionMode;
    UINT8 PassPhrase[MAX_PASS_PHRASE];
    int WPA_ON;
    char *ifname;
}wpaSupConfig;

#define SUPPLICANT_STACK_SIZE 8192
ALIGN8 char supplicantStack[SUPPLICANT_STACK_SIZE];
SUPPLICANT_THREAD_STATUS supplicantThreadStatus = SUP_TERMINATED;
void supplicantThreadFunc(unsigned long arg);
TX_THREAD supplicantThread;

struct wpa_global *gWPAGlobalParams = NULL;
wpaSupConfig* pSupConfig = NULL;

int SupConfig(wpaSupConfig sConfig)
{
    wpa_printf(1, "==> SupConfig (v5)\n");

    if(sConfig.WPA_ON && ((supplicantThreadStatus == SUP_TERMINATED)||(supplicantThreadStatus == SUP_RUNNING)))
    {
        if(supplicantThreadStatus < SUP_TERMINATED) 
        {
            //DPRINTF((DBG_SOFT | DBG_OUTPUT), ("\n\nSupConfig: terminate supplicantThread 1\n\n"));
            eloop_terminate();

            // signal any blocking socket layer selects to return
            raw_signal();

            // wait for supplicant to die
            while(supplicantThreadStatus < SUP_TERMINATED)
            {
                tx_thread_sleep(10);
            }
            tx_thread_delete(&supplicantThread);
            supplicantThreadStatus = SUP_TERMINATED;

        }
        // start the supplicant thread
        memset(&supplicantThread, 0, sizeof(supplicantThread));
        //pSupConfig = getMemory(sizeof(wpaSupConfig), SCRATCHALLOC);
        if (pSupConfig == NULL) {
            pSupConfig = os_malloc(sizeof(wpaSupConfig));
            ASSERT(pSupConfig);
        }
        memcpy(pSupConfig, &sConfig, sizeof(wpaSupConfig));

//        wpa_driver_wext_purge_event_queue();        // toss any stale events in the queue
        supplicantThreadStatus = SUP_INITIALIZING;

        wpa_printf(1, "Creating supplicant thread");

        // start the supplicant thread
        // NOTE that this thread runs at a priority equal to that of the highest other normal thread in the system.
        // This is done because testing has determined that it is critical that the supplicant respond quickly to
        // EAPOL events.  An example is key-refresh in the middle of a job.  If the key refresh takes too long,
        // the AP will disassociate, the job will be killed, and it will be ~1 minute before we re-associate. -jrs
        tx_thread_create(&supplicantThread, "WPA Supplicant", supplicantThreadFunc, (ULONG) pSupConfig, supplicantStack, SUPPLICANT_STACK_SIZE, 
                         THR_PRI_IMAGE, THR_PRI_IMAGE, DEFAULT_TIME_SLICE*2, TX_AUTO_START);

        // make sure it is really running and hasn't died before returning status
        wpa_printf(1, "waiting for supplicant thread to run");
        while(supplicantThreadStatus == SUP_INITIALIZING)
        {
//            wpa_printf(1, "Checking supplicant thread status\n");
            tx_thread_sleep(10);
        }

        if(supplicantThreadStatus == SUP_RUNNING)
        {
            wpa_printf(1, "<== SupConfig\n");

            return 0;
        }
        else
        {
            tx_thread_delete(&supplicantThread);
            supplicantThreadStatus = SUP_TERMINATED;
            wpa_printf(1, "<== SupConfig FAILED\n");
            return -1;
        }

    }
    else if(!(sConfig.WPA_ON) && (supplicantThreadStatus == SUP_RUNNING))
    {
        //DPRINTF((DBG_SOFT | DBG_OUTPUT), ("\n\nSupConfig: terminate supplicantThread 2\n\n"));
        // stop the supplicant thread
        eloop_terminate();

        // signal any blocking socket layer selects to return
        raw_signal();

        // wait for supplicant to die
        while(supplicantThreadStatus < SUP_TERMINATED)
        {
            tx_thread_sleep(10);
        }
        tx_thread_delete(&supplicantThread);
        supplicantThreadStatus = SUP_TERMINATED;
        wpa_printf(1, "<== SupConfig\n");
        return 0;
    }
    else
    {
        // invalid state for call
        wpa_printf(1, "<== SupConfig FAILED\n");
        return -1;
    }
}


/** 
 * \brief ThreadX entry point for supplicant thread.  Exectues
 * main supplicant event loop.
 * 
 * \param arg Not used
 * 
 * <more detailed description of function/typedef/struct/etc.>
 * 
 **/
void supplicantThreadFunc(unsigned long arg)
{
    struct wpa_interface iface;
    int exitcode = 0;
    struct wpa_params params;
    wpaSupConfig* sConfig = (wpaSupConfig*) arg;

    memset(&params, 0, sizeof(params));
    params.wpa_debug_level = MSG_ERROR;
    params.daemonize = FALSE;
    params.wpa_debug_use_file = FALSE;

    supplicantThreadStatus = SUP_INITIALIZING;

    gWPAGlobalParams = wpa_supplicant_init(&params);
    if (gWPAGlobalParams == NULL)
        ASSERT("Couldn't wpa_supplicant_init" == 0); //return -1;

    memset(&iface, 0, sizeof(iface));

    /* TODO: set interface parameters */
    //iface.ifname         = WIFI_IFACE_NAME_STATION;
    iface.ifname         = sConfig->ifname;
    iface.driver         = "marvell";
    //iface.confname       = "datastore";
    iface.confname       = sConfig->ifname;
    iface.bridge_ifname  = NULL;
    iface.driver_param   = NULL;
    iface.ctrl_interface = NULL;

    if (wpa_supplicant_add_iface(gWPAGlobalParams, &iface) == NULL)
        exitcode = -1;

    if (exitcode == 0)
    {
        supplicantThreadStatus = SUP_RUNNING;
        exitcode = wpa_supplicant_run(gWPAGlobalParams);
    }

    //DPRINTF((DBG_SOFT | DBG_OUTPUT), ("\n\n!!!supplicantThreadFunc: Terminating Supplicant!!!\n\n"));

    supplicantThreadStatus = SUP_TERMINATING;
    wpa_supplicant_deinit(gWPAGlobalParams);
    gWPAGlobalParams = NULL;
    supplicantThreadStatus = SUP_TERMINATED;
}

