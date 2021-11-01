/*
 * ============================================================================
 * Copyright (c) 2008-2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/**\file fax_dbg_net.c
 * The file contians the fax debug network status API functions.
 */
#include <string.h>
#ifdef HAVE_CMD

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "cmd_proc_api.h"
#include "fax_dbg_net.h"

static int dbg_net_active = 0;
static int dbg_net_socket = -1;
static int dbg_net_port = FAX_TEST_STATUS_UDP_PORT;
static struct sockaddr_in dbg_net_sa;

static int fax_dbg_net_write_msg(uint32_t msg);

/**
 * This function opens the UDP socket with port FAX_TEST_STATUS_UDP_PORT.
 *
 * The fax test client app will trigger a call to this function with the
 * "fax netopen <ip_address>" command.  This IP address is currently limited
 * to IPv4.
 *
 * @param ipaddr
 *   The IP address of the host system running the fax test client.
 * @return
 *   Returns zero on success or -1 on socket open failure.
 */
int fax_dbg_net_open(char *ipaddr)
{
    memset(&dbg_net_sa, 0, sizeof(dbg_net_sa));

    dbg_net_sa.sin_family = AF_INET;
    dbg_net_sa.sin_addr.s_addr = inet_addr(ipaddr);
    dbg_net_sa.sin_port = htons(dbg_net_port);
    
    dbg_net_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (-1 == dbg_net_socket)
    {
        dbg_net_active = 0;
        cmd_printf("FAXDBGNET: ERROR - Failed to create UDP status socket\n");
        return -1;
    }
    else
    {
        dbg_net_active = 1;
        cmd_printf("FAXDBGNET: Created UDP status socket (port %d)\n", 
            dbg_net_port);
        return 0;
    }
}


/**
 * This function closes the UDP socket with port FAX_TEST_STATUS_UDP_PORT.
 *
 * The fax test client app will trigger a call to this function with the
 * "fax netclose" command.
 *
 * @return
 *   Returns zero on success. 
 */
int fax_dbg_net_close(void)
{
    cmd_printf("FAXDBGNET: Closing UDP status socket\n");
    dbg_net_active = 0;
    close(dbg_net_socket);
    dbg_net_socket = -1;

    return 0;
}



int fax_dbg_net_log_fax_app_state(fax_app_state_t state)
{
    uint32_t msg;

    msg = (uint32_t) state + FAX_APP_STATE_MIN_RANGE;

    return fax_dbg_net_write_msg(msg);
}


int fax_dbg_net_log_fax_app_fax_sub_state(fax_app_fax_sub_state_t state)
{
    uint32_t msg;

    msg = (uint32_t) state + FAX_APP_FAX_STATE_MIN_RANGE;

    return fax_dbg_net_write_msg(msg);
}


int fax_dbg_net_log_fax_host_status(fax_host_status_t status)
{
    uint32_t msg;

    msg = (uint32_t) status + HOST_STATUS_MIN_RANGE; 

    return fax_dbg_net_write_msg(msg);


}

/**
 * This function sends out a msg on the UDP socket.
 *
 * This function will ensure the entire message is written out in 
 * network long format.  It is called by the fax_dbg_net_log_*() 
 * functions.
 *
 * @return
 *   Returns zero on success, -1 on socket not active, or -2 on socket write
 *   failure.
 */

static int fax_dbg_net_write_msg(uint32_t msg)
{
    int total = 0;
    int remaining = FAX_DBG_NET_MSG_LEN;
    int n;
    uint32_t tmp;
    char *msgptr;

    if (dbg_net_active)
    {
        tmp = htonl(msg);
        msgptr = (char *)&tmp;
        
        while (total < FAX_DBG_NET_MSG_LEN)
        {
            n = sendto(dbg_net_socket, msgptr + total, remaining, 0, 
                (struct sockaddr *)&dbg_net_sa, sizeof(dbg_net_sa));
            if (n < 0)
            {
                cmd_printf("FAXDBGNET: Error sending packet: %d\n",  errno);
                return -2;
            }
            total += n;
            remaining -= n;
        }
        
        return 0;
    }
    else
    {
        return -1;
    }
}

#endif
