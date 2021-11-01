/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 
/** 
 * \file sigma_interface.c
 * 
 * \brief Code to tie the WFA Sigma code into our system
 * 
 * Responsible for creating the wfa threads, communicating with
 * other parts of the system, etc.
 * 
 **/

#include "os_headers.h"
#include "agMessage.h"
#include "wfa_portall.h"
#include "wfa_sock.h"
#include "wfa_debug.h"
#include "wfa_main.h"
#include "wifi_intf_event_api.h"
#include "wifi_intf_vars.h"

#include "tx_api.h"
//#include "agRouter.h"
//#include "sys_init_api.h"

// *********************************************************************
#define WFA_SIGMA_CA_STACK_SIZE  6 * 1024
			
TX_THREAD wfa_sigma_ca_thread;
ALIGN8 char wfa_sigma_ca_stack[WFA_SIGMA_CA_STACK_SIZE];

int wfa_sigma_ca_main(unsigned int unused);

#define END_OF_REQUEST " \r\n"

// *********************************************************************
#define WFA_SIGMA_DUT_STACK_SIZE  6 * 1024

TX_THREAD wfa_sigma_dut_thread;
ALIGN8 char wfa_sigma_dut_stack[WFA_SIGMA_DUT_STACK_SIZE];

int wfa_sigma_dut_main(unsigned int unused);

// *********************************************************************

static sig_t alarm_sig_func = NULL;

static TX_TIMER sigAlrmTimer;
static int alarmTimerCreated = FALSE;

// *********************************************************************

/** 
 * \brief Create the thread that runs sigma's control agent and
 * start running it.
 * 
 **/
static void startSigmaDUT()
{
    tx_thread_create( &wfa_sigma_dut_thread, 
                       "Sigma DUT",
                       (void(*)(ULONG)) wfa_sigma_dut_main,
                       0, 
                       wfa_sigma_dut_stack,
                       WFA_SIGMA_DUT_STACK_SIZE, 
                       THR_PRI_NORMAL, 
                       THR_PRI_NORMAL,
                       1, 
                       TX_AUTO_START );
}

/** 
 * \brief Create the thread that runs sigma's control agent and
 * start running it.
 * 
 **/
static void startSigmaCA()
{
    tx_thread_create( &wfa_sigma_ca_thread, 
                       "Sigma Control Agent",
                       (void(*)(ULONG)) wfa_sigma_ca_main,
                       0, 
                       wfa_sigma_ca_stack,
                       WFA_SIGMA_CA_STACK_SIZE, 
                       THR_PRI_NORMAL, 
                       THR_PRI_NORMAL,
                       1, 
                       TX_AUTO_START );
}


/** 
 * \brief Start Sigma's control agent and DUT
 **/
void startSigma()
{
    // Wait for the rest of the system (especially network)
    //SysWaitForInit(); 

    // Let the DUT get up and running (the CA will connect to it)
    //wSLEEP(10);

    startSigmaDUT();

    //wSLEEP(10);

    startSigmaCA(); 
}

/** 
 * \brief Pass down commands to the Sigma CA.
 * 
 **/
void sigma_cmd_handler(char *request_data, int request_len, char *response_buffer, int *response_len)
{
    struct sockaddr_in ca_addr;
    int                ca_socket;
    int                response_complete = FALSE; 
    int                num_read_calls = 0;

    wMEMCPY(request_data + request_len, END_OF_REQUEST, strlen(END_OF_REQUEST));
    request_len += strlen(END_OF_REQUEST);

    // Establish an internal connection to the CA
    if ((ca_socket = wSOCKET(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        DPRINT_ERR(WFA_ERR, "socket() failed: %s\n", wSOCKET_ERR_TEXT(ca_socket));
        wEXIT(1);
    }

    wMEMSET(&ca_addr, 0, sizeof(ca_addr)); 
    ca_addr.sin_family      = AF_INET;
    ca_addr.sin_addr.s_addr = inet_addr(NET_LOOPBACK_ADDR);
    ca_addr.sin_port        = htons(WFA_SIGMA_CA_PORT);

    if (wCONNECT(ca_socket, (struct sockaddr *) &ca_addr, sizeof(ca_addr)) < 0)
    {
        DPRINT_ERR(WFA_ERR, "Failed to connect to CA: %s\n", wSOCKET_ERR_TEXT(ca_socket));
        wEXIT(1);
    }

    // Send the request (skip the cmd= that comes in the post if it is there
    if (wSTRNCMP(request_data, "cmd=", 4) == 0)
    {
        wSEND(ca_socket, request_data + 4, request_len - 4, 0);
    }
    else
    {
        wSEND(ca_socket, request_data, request_len, 0);
    }
     
    ASSERT(response_buffer != NULL);
    *response_len = 0;

    do
    {
        int   bytes_read;
        char *latest_data;

        latest_data = response_buffer + (*response_len);
        bytes_read = wRECV(ca_socket, latest_data, WFA_BUFF_1K - (*response_len), 0);
        num_read_calls++;
        if (bytes_read > 0)
        {
            *response_len += bytes_read;

            if (wSTRNCMP("status,COMPLETE", latest_data, 15) == 0 || 
                    wSTRNCMP("status,INVALID", (char *)latest_data, 14) == 0 ||
                    num_read_calls >= 3)
            {
                response_complete = TRUE;
            }
            else
            {
                ASSERT((*response_len) < WFA_BUFF_1K);
            }
        }
        else
        {
            DPRINT_ERR(WFA_ERR, "Error on read: %s\n", wSOCKET_ERR_TEXT(ca_socket));
            wEXIT(1);
        }
    } while(!response_complete);
    
    wCLOSE(ca_socket); 
}

void wfa_signal(int sigNum, sig_t sigFunc)
{
    if (sigNum == SIGALRM)
    {
        if (sigFunc != SIG_IGN)
        {
            alarm_sig_func = sigFunc;
        }
        else
        {
            alarm_sig_func = NULL;
        }
    }
}

void wfa_sigalrm_timer_func(unsigned long unused)
{
    wPRINTF("Sigma: Timer expired\n");
    tx_timer_deactivate(&sigAlrmTimer);
    if (alarm_sig_func != NULL)
    {
        alarm_sig_func(SIGALRM);
        alarm_sig_func = NULL;
    }
}

void wfa_alarm(int sec)
{
    if (!alarmTimerCreated)
    {
        //setting callback time to something so that the timer create function does not bomb
        tx_timer_create(&sigAlrmTimer, "wfa_sigAlrmTimer", 
                wfa_sigalrm_timer_func, 0, 100 * 10, 0, TX_NO_ACTIVATE); 
        
        alarmTimerCreated = TRUE;            
    }

    tx_timer_deactivate(&sigAlrmTimer);
    if (sec > 0)
    {
        wPRINTF("Setting up alarm to fire in %d seconds\n", sec);

        tx_timer_change(&sigAlrmTimer, 100 * sec, 0);
        tx_timer_activate(&sigAlrmTimer);            
    }
}

