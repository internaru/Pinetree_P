/*
 * WPA Supplicant - Layer2 packet handling with Linux packet sockets
 * Copyright (c) 2003-2005, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include "logger.h"
#include "includes.h"

#include "common.h"
#include "eloop.h"
#include "l2_packet.h"

#include "raw_socket_api.h"

#include "wireless_copy.h"
#include "lassert.h"

#include "wlanInterface.h"

#define L2_PACKET_RECV_SIZE 2300

int tfGetSocketError(int socketDescriptor);


struct l2_packet_data {
	int fd; /* packet socket for EAPOL frames */
	char ifname[IFNAMSIZ + 1];
	int ifindex;
	u8 own_addr[ETH_ALEN];
	void (*rx_callback)(void *ctx, const u8 *src_addr,
			    const u8 *buf, size_t len);
	void *rx_callback_ctx;
	int l2_hdr; /* whether to include layer 2 (Ethernet) header data
		     * buffers */
};


int l2_packet_get_own_addr(struct l2_packet_data *l2, u8 *addr)
{
	os_memcpy(addr, l2->own_addr, ETH_ALEN);
	return 0;
}


int l2_packet_send(struct l2_packet_data *l2, const u8 *dst_addr, u16 proto,
		   const u8 *buf, size_t len)
{
    int ret;

    if (l2 == NULL)
        return -1;
    if (l2->l2_hdr) {
        ret = raw_send(l2->fd, (char *) buf, len, 0);
    } else {
        struct l2_ethhdr *dataWithHdr = os_malloc(len + sizeof(struct l2_ethhdr));

        os_memcpy(((char *)dataWithHdr) + sizeof(struct l2_ethhdr), buf, len);
        os_memcpy(dataWithHdr->h_dest, dst_addr, ETH_ALEN);
        l2_packet_get_own_addr(l2, dataWithHdr->h_source);
        dataWithHdr->h_proto = htons(proto);

        ret = raw_send(l2->fd, (char *)dataWithHdr, len + sizeof(struct l2_ethhdr), 0);

        os_free(dataWithHdr);
    }

    // ca: retVal = WirelessDrvrSendRaw(buf, len);
    if(ret == 0)
    {
        ret = len;
    }

    return ret;
}


static void l2_packet_receive(int sock, void *eloop_ctx, void *sock_ctx)
{
    struct l2_packet_data *l2 = eloop_ctx;
    char * buf;
    int res;
    struct l2_ethhdr *ethhdr;

    buf = (char *)os_malloc(L2_PACKET_RECV_SIZE);
    if (buf == NULL)
        return;

    res = recv(sock, buf, L2_PACKET_RECV_SIZE, 0); 
    if (res < 0) {
        DPRINTF(DBG_SOFT|DBG_TERMINAL, ("***l2PacketRECV:-- FAILED.....\n"));
        os_free(buf);
        return;
    }
    if (res < sizeof(*ethhdr)) {
        wpa_printf(MSG_DEBUG, "l2_packet_receive: Dropped too short "
               "%d packet", res);
        os_free(buf);
        return;
    }

    //DPRINTF(DBG_SOFT|DBG_TERMINAL, ("***l2PacketRECV:-- Inside the l2packet receive ethernet\n"));

    ethhdr = (struct l2_ethhdr *) buf;

    l2->rx_callback(l2->rx_callback_ctx, ethhdr->h_source,
            (unsigned char *) (ethhdr + 1),
            res - sizeof(*ethhdr));

    os_free(buf);
}

struct l2_packet_data * l2_packet_init(
	char *ifname, const u8 *own_addr, unsigned short protocol,
	void (*rx_callback)(void *ctx, const u8 *src_addr,
			    const u8 *buf, size_t len),
	void *rx_callback_ctx, int l2_hdr)
{
    //IpAddr ipAddr;
    struct l2_packet_data *l2;
    //struct ifreq ifr;
    //struct sockaddr_ll ll;      
    //int on = 1;  //Used as parameter to setsockopt.

    l2 = (struct l2_packet_data *) os_malloc(sizeof(struct l2_packet_data));
    if (l2 == NULL)
        return NULL;
    memset(l2, 0, sizeof(*l2));
    strncpy(l2->ifname, ifname, sizeof(l2->ifname));
    l2->rx_callback = rx_callback;
    l2->rx_callback_ctx = rx_callback_ctx;

    #if 0   
    l2->fd = socket(PF_PACKET, SOCK_RAW, htons(protocol)); 
    #else
    l2->fd = socket(AF_INET, SOCK_RAW, htons(protocol));   
    //l2->fd = tfRawSocket(0, htons(protocol));
    if(l2->fd == -1)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT, ("The socket error code [%d]\n", tfGetSocketError(l2->fd)));
        return NULL;
    }

//-jrs    setsockopt(l2->fd,IPPROTO_IP,IPO_HDRINCL, (const char *)&on, sizeof(on));  
    #endif

    if (l2->fd < 0) {
        #if 0   
//-jrs         perror("socket(PF_PACKET, SOCK_RAW)"); 
        #endif
        DPRINTF(DBG_SOFT|DBG_TERMINAL, ("socket(PF_PACKET, SOCK_RAW)"));
        os_free(l2);
        return NULL;
    }

    if (strcmp(l2->ifname, "wfd0") != 0)
    {
        setsockopt(l2->fd, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname));
    } else {
        // Assume the supplicant only runs with STA 
        // Temporary override - use mlan0 raw handle
        if (setsockopt(l2->fd, SOL_SOCKET, SO_BINDTODEVICE, "mlan0", strlen("mlan0")) < 0)
        {
            DPRINTF(DBG_SOFT|DBG_OUTPUT, ("Failed to bind to interface: %s\n", l2->ifname));
        }
    }

#if 0 
    strncpy(ifr.ifr_name, l2->ifname, sizeof(ifr.ifr_name));
    if (ioctl(l2->fd, SIOCGIFINDEX, &ifr) < 0) {

        #if 0   
//-jrs         perror("ioctl[SIOCGIFINDEX]"); 
        #endif
        
        closesocket(l2->fd);
        os_free(l2);
        return NULL;
    }

    memset(&ll, 0, sizeof(ll));
    ll.sll_family = AF_INET;
    ll.sll_ifindex = 0;//ifr.ifr_ifindex;  // TODO:--- We always have single interface. (Assumed 0 based indexing)
    ll.sll_protocol = htons(protocol);

    if (bind(l2->fd, (struct sockaddr *) &ll, sizeof(ll)) < 0) {  
        #if 0   
//-jrs         perror("bind[PF_PACKET]"); 
        #endif
        closesocket(l2->fd);
        os_free(l2);
        return NULL;
    }

    if (ioctl(l2->fd, SIOCGIFHWADDR, &ifr) < 0) {
        #if 0   
//-jrs         perror("ioctl[SIOCGIFHWADDR]"); 
        #endif
        closesocket(l2->fd);  
        os_free(l2);
        return NULL;
    }   
    memcpy(l2->own_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);    
#else
    //wlan_get_mac_address(wlan_get_net_dev(), l2->own_addr);
    WlanGetMacAddr(l2->ifname, (char *) l2->own_addr);
#endif

    eloop_register_read_sock(l2->fd, l2_packet_receive, l2, NULL);

    return l2;
}


void l2_packet_deinit(struct l2_packet_data *l2)
{
	if (l2 == NULL)
		return;

	if (l2->fd >= 0) {
		eloop_unregister_read_sock(l2->fd);
		//closesocket(l2->fd);
        raw_close_socket(l2->fd);
	}
		
	os_free(l2);
}


int l2_packet_get_ip_addr(struct l2_packet_data *l2, char *buf, size_t len)
{
/*
	int s;
	struct ifreq ifr;
	struct sockaddr_in *saddr;

	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		perror("socket");
		return -1;
	}
	os_memset(&ifr, 0, sizeof(ifr));
	os_strncpy(ifr.ifr_name, l2->ifname, sizeof(ifr.ifr_name));
	if (ioctl(s, SIOCGIFADDR, &ifr) < 0) {
		perror("ioctl[SIOCGIFADDR]");
		close(s);
		return -1;
	}
	close(s);
	saddr = (struct sockaddr_in *) &ifr.ifr_addr;
	if (saddr->sin_family != AF_INET)
		return -1;
	os_snprintf(buf, len, "%s", inet_ntoa(saddr->sin_addr));
*/
    ASSERT("TODO: Implement l2_packet_get_ip_addr" == 0);
	return 0;
}


void l2_packet_notify_auth_start(struct l2_packet_data *l2)
{
}
