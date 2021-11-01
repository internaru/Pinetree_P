/****************************************************************************** 
 *  Description:
 *      wlan network device structure and interface
 *
 *  The wlan drivers use a net_device structure to maintain context.
 *  This structure was defined in some linux header files, but since we're
 *  not on linux, we really don't need anything that complex, so we'll
 *  just create our own analogue structure here.
 *
 *  This structure will maintain the context for the drivers and will serve
 *  as the function pointer interface from the upper level protocols into
 *  the wlan component.
 *
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2012
 *
 *****************************************************************************/

#ifndef _OS_NET_DEVICE_H
#define _OS_NET_DEVICE_H

#include "trsocket.h"
#include "os_defs.h"

typedef struct sk_buff{
	struct sk_buff	*prev;
	struct sk_buff	*next;
	UINT32		len;
	UCHAR		*data;
	UCHAR		*tail;
	UCHAR       *head; // bqiu
	void 		*bufDesc;
	UINT8       priority;
	struct timeval stamp;
    UCHAR       *txdata;
    void        *handle;
}sk_buff;

#define ARPHRD_ETHER    1               /* Ethernet 10Mbps              */
#define ETH_HLEN	14
#define ENOTSUPP	523

#define	IFF_BROADCAST	0x2		/* broadcast address valid	*/
#define	IFF_PROMISC	0x100		/* receive all packets		*/
#define	IFF_ALLMULTI	0x200		/* receive all multicast packets*/
#define IFF_MULTICAST	0x1000		/* Supports multicast		*/
#define MAX_SSID_NAME_LEN 33 // 32 plus null
#define MAX_PASS_PHRASE 64

#ifdef IFNAMSIZ
#undef IFNAMSIZ
#define IFNAMSIZ 64
#endif

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

struct SCAN_SSID_IE{
	UINT8 SSID[33];
	UINT32 ssid_len;
	UINT8 BSSID[6];
	
	UINT16 Capability;
	UINT8	WPA_IE[40];
	int	wpa_ie_len;
    int channel;
};

#define WPA_ALPHANUMERIC 0
#define WPA_HEXADECIMAL 1

typedef struct {
    UINT8 SSID[MAX_SSID_NAME_LEN];
    int WPAEncryptionMode;
    UINT8 PassPhrase[MAX_PASS_PHRASE];
    int WPA_ON;
    char *ifname;
} wpaSupConfig;

typedef struct net_device_stats
{
    UINT32 rx_packets;
    UINT32 tx_packets;
    UINT32 rx_bytes;
    UINT32 tx_bytes;
    UINT32 rx_errors;
    UINT32 tx_errors;
    UINT32 rx_dropped;
    UINT32 tx_dropped;

    /*
     * detailed rx_errors: 
     */
    UINT32 rx_length_errors;
    UINT32 rx_over_errors;
    UINT32 rx_crc_errors;
    UINT32 rx_fifo_errors;
    UINT32 rx_missed_errors;

    UINT32 tx_aborted_errno;
    UINT32 tx_carrier_errors;
    UINT32 tx_fifo_errors;
    UINT32 heartbeat_errors;
    UINT32 tx_window_errors;

    UINT32 rx_compressed;
    UINT32 tx_compressed;

} net_device_stats;

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
#define IFHWADDRLEN	6
struct ifreq 
{
	union
	{
		char	ifrn_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	} ifr_ifrn;
	
	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		struct	sockaddr ifru_netmask;
		struct  sockaddr ifru_hwaddr;
		short	ifru_flags;
		int	ifru_ivalue;
		int	ifru_mtu;
		char	ifru_slave[IFNAMSIZ];	/* Just fits the size */
		char	ifru_newname[IFNAMSIZ];
		void *	ifru_data;
	} ifr_ifru;
};

#define ifr_name	ifr_ifrn.ifrn_name	/* interface name 	*/
#define ifr_hwaddr	ifr_ifru.ifru_hwaddr	/* MAC address 		*/
#define	ifr_addr	ifr_ifru.ifru_addr	/* address		*/
#define	ifr_dstaddr	ifr_ifru.ifru_dstaddr	/* other end of p-p lnk	*/
#define	ifr_broadaddr	ifr_ifru.ifru_broadaddr	/* broadcast address	*/
#define	ifr_netmask	ifr_ifru.ifru_netmask	/* interface net mask	*/
#define	ifr_flags	ifr_ifru.ifru_flags	/* flags		*/
#define	ifr_metric	ifr_ifru.ifru_ivalue	/* metric		*/
#define	ifr_mtu		ifr_ifru.ifru_mtu	/* mtu			*/
#define ifr_slave	ifr_ifru.ifru_slave	/* slave device		*/
#define	ifr_data	ifr_ifru.ifru_data	/* for use by interface	*/
#define ifr_ifindex	ifr_ifru.ifru_ivalue	/* interface index	*/
#define ifr_bandwidth	ifr_ifru.ifru_ivalue    /* link bandwidth	*/
#define ifr_qlen	ifr_ifru.ifru_ivalue	/* Queue length 	*/
#define ifr_newname	ifr_ifru.ifru_newname	/* New name		*/

#define NETIF_F_SG              1       /* Scatter/gather IO. */
#define NETIF_F_IP_CSUM         2       /* Can checksum only TCP/UDP over IPv4. */
#define NETIF_F_NO_CSUM         4       /* Does not require checksum. F.e. loopack. */
#define NETIF_F_HW_CSUM         8       /* Can checksum all the packets. */
#define NETIF_F_DYNALLOC        16      /* Self-dectructable device. */
#define NETIF_F_HIGHDMA         32      /* Can DMA to high memory. */
#define NETIF_F_FRAGLIST        64      /* Scatter/gather IO. */

// Match MRVDRV_MAX_MULTICAST_LIST_SIZE from wlan_types.h (currently 32)
#define MAX_MC_ADDRS 32

typedef struct net_device
{
    VOID* priv;
    UINT32 flags;
	/* bqiu Net device features */
    int           features;
    int           refcnt; //-jrs
    int irq;

    UINT8 dev_addr[ETH_ALEN];

    int mc_count;	    // required for set_multicast function
    UINT8 *mc_list[MAX_MC_ADDRS];  // multicast address list

    // WLAN interface functions
    int (*open)(struct net_device *dev);
    int (*stop)(struct net_device *dev);
    struct net_device_stats* (*get_stats)(struct net_device *dev);
    int (*hard_start_xmit)(struct sk_buff *skb, struct net_device *dev);
    int (*do_ioctl)(struct net_device *dev, struct ifreq *ifr, int cmd);

    int (*set_mac_address)(struct net_device *dev, void *addr);
    void (*set_multicast_list)(struct net_device *dev);
    void (*tx_timeout) (struct net_device *dev);
    struct iw_statistics* (*get_wireless_stats)(struct net_device *dev);
    struct iw_handler_def* wireless_handlers;
    int watchdog_timeo; /* used by dev_watchdog() */

    char* name;
}net_device;

/*
 *	These are the defined Ethernet Protocol ID's.
 */

#define ETH_P_LOOP	0x0060		/* Ethernet Loopback packet	*/
#define ETH_P_PUP	0x0200		/* Xerox PUP packet		*/
#define ETH_P_PUPAT	0x0201		/* Xerox PUP Addr Trans packet	*/
#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
#define ETH_P_X25	0x0805		/* CCITT X.25			*/
#define ETH_P_ARP	0x0806		/* Address Resolution packet	*/
#define	ETH_P_BPQ	0x08FF		/* G8BPQ AX.25 Ethernet Packet	[ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_P_IEEEPUP	0x0a00		/* Xerox IEEE802.3 PUP packet */
#define ETH_P_IEEEPUPAT	0x0a01		/* Xerox IEEE802.3 PUP Addr Trans packet */
#define ETH_P_DEC       0x6000          /* DEC Assigned proto           */
#define ETH_P_DNA_DL    0x6001          /* DEC DNA Dump/Load            */
#define ETH_P_DNA_RC    0x6002          /* DEC DNA Remote Console       */
#define ETH_P_DNA_RT    0x6003          /* DEC DNA Routing              */
#define ETH_P_LAT       0x6004          /* DEC LAT                      */
#define ETH_P_DIAG      0x6005          /* DEC Diagnostics              */
#define ETH_P_CUST      0x6006          /* DEC Customer use             */
#define ETH_P_SCA       0x6007          /* DEC Systems Comms Arch       */
#define ETH_P_RARP      0x8035		/* Reverse Addr Res packet	*/
#define ETH_P_ATALK	0x809B		/* Appletalk DDP		*/
#define ETH_P_AARP	0x80F3		/* Appletalk AARP		*/
#define ETH_P_8021Q	0x8100          /* 802.1Q VLAN Extended Header  */
#define ETH_P_IPX	0x8137		/* IPX over DIX			*/
#define ETH_P_IPV6	0x86DD		/* IPv6 over bluebook		*/
#define ETH_P_PAUSE	0x8808		/* IEEE Pause frames. See 802.3 31B */
#define ETH_P_SLOW	0x8809		/* Slow Protocol. See 802.3ad 43B */
#define ETH_P_WCCP	0x883E		/* Web-cache coordination protocol
					 * defined in draft-wilson-wrec-wccp-v2-00.txt */
#define ETH_P_PPP_DISC	0x8863		/* PPPoE discovery messages     */
#define ETH_P_PPP_SES	0x8864		/* PPPoE session messages	*/
#define ETH_P_MPLS_UC	0x8847		/* MPLS Unicast traffic		*/
#define ETH_P_MPLS_MC	0x8848		/* MPLS Multicast traffic	*/
#define ETH_P_ATMMPOA	0x884c		/* MultiProtocol Over ATM	*/
#define ETH_P_ATMFATE	0x8884		/* Frame-based ATM Transport
					 * over Ethernet
					 */
#define ETH_P_AOE	0x88A2		/* ATA over Ethernet		*/
#define ETH_P_TIPC	0x88CA		/* TIPC 			*/

/*
 *	Non DIX types. Won't clash for 1500 types.
 */
 
#define ETH_P_802_3	0x0001		/* Dummy type for 802.3 frames  */
#define ETH_P_AX25	0x0002		/* Dummy protocol id for AX.25  */
#define ETH_P_ALL	0x0003		/* Every packet (be careful!!!) */
#define ETH_P_802_2	0x0004		/* 802.2 frames 		*/
#define ETH_P_SNAP	0x0005		/* Internal only		*/
#define ETH_P_DDCMP     0x0006          /* DEC DDCMP: Internal only     */
#define ETH_P_WAN_PPP   0x0007          /* Dummy type for WAN PPP frames*/
#define ETH_P_PPP_MP    0x0008          /* Dummy type for PPP MP frames */
#define ETH_P_LOCALTALK 0x0009		/* Localtalk pseudo type 	*/
#define ETH_P_PPPTALK	0x0010		/* Dummy type for Atalk over PPP*/
#define ETH_P_TR_802_2	0x0011		/* 802.2 frames 		*/
#define ETH_P_MOBITEX	0x0015		/* Mobitex (kaz@cafe.net)	*/
#define ETH_P_CONTROL	0x0016		/* Card specific control frames */
#define ETH_P_IRDA	0x0017		/* Linux-IrDA			*/
#define ETH_P_ECONET	0x0018		/* Acorn Econet			*/
#define ETH_P_HDLC	0x0019		/* HDLC frames			*/
#define ETH_P_ARCNET	0x001A		/* 1A for ArcNet :-)            */

/*
 *	This is an Ethernet frame header.
 */
 
struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	UINT16		h_proto;		/* packet type ID field	*/
} __attribute__((packed));






#endif  // _OS_NET_DEVICE_H
