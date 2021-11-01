/** @file wps_l2.c
 *  @brief This file contains functions handling layer 2 socket read/write.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
//#include <sys/ioctl.h>
#include <errno.h>
/*
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <arpa/inet.h>
*/
#include "common.h"
#include "raw_socket_api.h"

#include "wps_def.h"
#include "wps_util.h"
#include "wps_l2.h"
#include "wps_os.h"
#include "logger.h"
#include "wlanInterface.h"

#define WPS_L2_BUF_SIZE	2300

int tfGetSocketError(int socketDescriptor);

/********************************************************
        Local Variables
********************************************************/

/********************************************************
        Global Variables
********************************************************/

/********************************************************
        Local Functions
********************************************************/
/** 
 *  @brief Process Layer 2 socket receive function
 *  
 *  @param sock         Socket number for receiving
 *  @param l2_ctx       A pointer to user private information 
 *  @return             None
 */
static void
wps_l2_receive(int sock, void *l2_ctx)
{
    WPS_L2_INFO *l2 = l2_ctx;
    u8 *buf = NULL;
    int res;

    ENTER();
	
	buf = (u8 *) os_malloc(WPS_L2_BUF_SIZE);
	if (buf)
	{
        struct l2_ethhdr *ethhdr;
#if 0
	    struct sockaddr_ll ll;
	    socklen_t fromlen;

	    memset(&ll, 0, sizeof(ll));
	    fromlen = sizeof(ll);
	    res = 
            recvfrom(sock, buf, WPS_L2_BUF_SIZE, 0, (struct sockaddr *) &ll, &fromlen); 
#endif    
	    //res = recv(sock, (char *)buf, WPS_L2_BUF_SIZE, 0); 
	    res = raw_recv(sock, (char *)buf, WPS_L2_BUF_SIZE, 0);
        if (res < 0) {
	        DPRINTF(DBG_SOFT|DBG_TERMINAL, ("***l2PacketRECV:-- FAILED.....\n"));
            LEAVE();
	        return;
	    }
   
	    DPRINTF(DBG_SOFT|DBG_TERMINAL, ("***l2PacketRECV:-- Inside the l2packet receive ethernet\n"));

	    ethhdr = (struct l2_ethhdr *) buf;

	    l2->rx_callback(ethhdr->h_source, (unsigned char *) (ethhdr + 1),
	            res - sizeof(*ethhdr));
		os_free(buf);
	} else {
		DPRINTF(DBG_LOUD|DBG_OUTPUT, ("***l2PacketRECV:-- MALLOC FAILURE!\n"));	
	}
    LEAVE();
}

/********************************************************
        Global Functions
********************************************************/
/** 
 *  @brief Get Layer 2 MAC Address
 *  
 *  @param l2           A pointer to structure of layer 2 information
 *  @param addr         A pointer to returned buffer
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_l2_get_mac(WPS_L2_INFO * l2, u8 * addr)
{
    ENTER();
    memcpy(addr, l2->my_mac_addr, ETH_ALEN);
    LEAVE();
    return WPS_STATUS_SUCCESS;
}

/** 
 *  @brief Process Layer 2 socket send function
 *  
 *  @param l2           A pointer to structure of layer 2 information
 *  @param dst_addr     Destination address to send
 *  @param proto        Protocol number for layer 2 packet
 *  @param buf          A pointer to sending packet buffer
 *  @param len          Packet length
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_l2_send(WPS_L2_INFO * l2, const u8 * dst_addr, u16 proto,
            const u8 * buf, size_t len)
{
    int ret = WPS_STATUS_FAIL;
    struct l2_ethhdr *dataWithHdr

    ENTER();

    if (l2 == NULL) {
        LEAVE();
        return WPS_STATUS_FAIL;
    }

	dataWithHdr = os_malloc(len + sizeof(struct l2_ethhdr));
	if (dataWithHdr)
    {
        os_memcpy(((char *)dataWithHdr) + sizeof(struct l2_ethhdr), buf, len);
        os_memcpy(dataWithHdr->h_dest, dst_addr, ETH_ALEN);
        os_memcpy(dataWithHdr->h_source, l2->my_mac_addr, ETH_ALEN);
        dataWithHdr->h_proto = htons(proto);
    
//        wps_hexdump(MSG_INFO, "Outgoing data", dataWithHdr, len + sizeof(struct l2_ethhdr));
        ret = raw_send(l2->fd, (char *) dataWithHdr, len + sizeof(struct l2_ethhdr), 0);
        if(ret == 0)
            ret = len;

        os_free(dataWithHdr);
    } else {
        DPRINTF(DBG_LOUD|DBG_OUTPUT, ("***wps_l2_send:-- MALLOC FAILURE!\n"));	
	}

    LEAVE();
    return ret;
}

/** 
 *  @brief Process Layer 2 socket initialization
 *  
 *  @param ifname       Interface name
 *  @param role         WPS_REGISTRAR or WPS_ENROLLEE
 *  @param protocol     Ethernet protocol number in host byte order
 *  @param rx_callback  Callback function that will be called for each received packet
 *  @param l2_hdr       1 = include layer 2 header, 0 = do not include header
 *  @return             A pointer to l2 structure
 */
WPS_L2_INFO *
wps_l2_init(const char *ifname,
            int role,
            unsigned short protocol,
            void (*rx_callback) (const u8 * src_addr,
                                 const u8 * buf, size_t len), int l2_hdr)
{
    WPS_L2_INFO *l2;
    // struct ifreq ifr;
    //struct sockaddr_ll ll;

    ENTER();

    l2 = os_malloc(sizeof(WPS_L2_INFO));
    if (l2 == NULL) {
        LEAVE();
        return NULL;
    }

    memset(l2, 0, sizeof(*l2));
    strncpy(l2->ifname, ifname, sizeof(l2->ifname));
    l2->rx_callback = rx_callback;
    l2->l2_hdr = l2_hdr;

    l2->fd = socket(AF_INET, SOCK_RAW, htons(protocol));   
    if(l2->fd == -1)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("The socket error code [%d]\n", tfGetSocketError(l2->fd)));
        os_free(l2);
        return NULL;
    }

    if (strcmp(l2->ifname, "wfd0") != 0)
    {
        if (setsockopt(l2->fd, SOL_SOCKET, SO_BINDTODEVICE, l2->ifname, strlen(l2->ifname)) < 0)
        {
            DPRINTF(DBG_SOFT|DBG_OUTPUT, ("Failed to bind to interface: %s\n", l2->ifname));
            os_free(l2);
            return NULL;
        }
        WlanGetMacAddr(WIFI_IFACE_NAME_WFD, (char *)l2->my_mac_addr);
    }
    else
    {
        if (role == WPS_ENROLLEE) {
            // Temporary override - use mlan0 raw handle
            if (setsockopt(l2->fd, SOL_SOCKET, SO_BINDTODEVICE, "mlan0", strlen("mlan0")) < 0)
            {
                DPRINTF(DBG_SOFT|DBG_OUTPUT, ("Failed to bind to interface: %s\n", l2->ifname));
                os_free(l2);
                return NULL;
            }
            WlanGetMacAddr(WIFI_IFACE_NAME_STATION, (char *)l2->my_mac_addr);
        } else {
            // Temporary override - use uap0 raw handle
            if (setsockopt(l2->fd, SOL_SOCKET, SO_BINDTODEVICE, "uap0", strlen("uap0")) < 0)
            {
                DPRINTF(DBG_SOFT|DBG_OUTPUT, ("Failed to bind to interface: %s\n", l2->ifname));
                os_free(l2);
                return NULL;
            }
            WlanGetMacAddr(WIFI_IFACE_NAME_UAP, (char *)l2->my_mac_addr);
        }
    }
    
    /* register the read socket handler for l2 packet */
    wps_register_rdsock_handler(l2->fd, wps_l2_receive, l2);

    LEAVE();
    return l2;
}


/** 
 *  @brief Process Layer 2 socket free
 *  
 *  @param l2       A pointer to user private information 
 *  @return         None
 */
void
wps_l2_deinit(WPS_L2_INFO * l2)
{
    ENTER();

    if (l2 == NULL) {
        LEAVE();
        return;
    }

    if (l2->fd >= 0) {
        wps_unregister_rdsock_handler(l2->fd);
        raw_close_socket(l2->fd);
    }

    os_free(l2);

    LEAVE();
}
