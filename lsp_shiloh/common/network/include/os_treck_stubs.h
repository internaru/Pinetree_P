/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// Define Treck network library stubs

#ifndef _TRECK_TYPES_ALREADY_DEFINED_
#define _TRECK_TYPES_ALREADY_DEFINED_

#include "platform.h" // SYS_TICK_FREQ, MIN, MAX

#define HAVE_ETH_RECV_CHKSUM_OFFLOAD /* value defined? */

typedef uint8_t                 ttUser8Bit;
typedef void*                   ttUserVoid;
typedef ttUserVoid*             ttUserVoidPtr;
typedef void*                   ttUserInterface;
typedef ttUserInterface*        ttUserInterfacePtr;
typedef void*                   ttUserBuffer;
typedef ttUserBuffer*           ttUserBufferPtr;
typedef unsigned                ttUserLinkLayer;
// see also net_link_config.h ..!
typedef struct {
                 unsigned int devIndex;
                 void* user_data;
                              } ttDeviceEntry;
typedef ttDeviceEntry*          ttDeviceEntryPtr;
typedef void*                   ttDevRecvOffloadStruct;
typedef ttDevRecvOffloadStruct* ttDevRecvOffloadPtr;
typedef unsigned long  ULONG;
typedef struct eth_header_s {
  uint16_t ethType;
                              } eth_header_t;
typedef eth_header_t            ttEtherHeader;
typedef ttEtherHeader*          ttEtherHeaderPtr;
typedef void*                   ttEtherPacket;
typedef ttEtherPacket*          ttEtherPacketPtr;
typedef void*                   ttPacket;
typedef ttPacket*               ttPacketPtr;

typedef int (*ttDevOpenCloseFuncPtr)(ttUserInterface);
typedef int (*ttDevSendFuncPtr)(ttUserInterface, char*, int, int);
typedef int (*ttDevRecvFuncPtr)(ttUserInterface, char**, int*, ttUserBuffer*, ttDevRecvOffloadPtr /*offload_ptr?*/);
typedef int (*ttDevIoctlFuncPtr) (ttUserInterface, int, void*, int);
typedef int (*ttDevGetPhysAddrFuncPtr)(void*, char*);


// (more) Treck stubs while migrating network -> posix/sockets

#define TX_INHERIT                   -2
#define TM_SOCKET_ERROR              -1
#define TX_NO_WAIT                    0
#define TM_ENOERROR                   0
#define TM_ETHERNET_PHY_ADDR_LEN      6
#define TM_BOOTSNAME_SIZE             64
#define TM_BT_DOMAIN_INDEX            1
#define TM_DEV_OK                     0 /*revisit*/
#define TM_DEV_OKAY                   0 /*revisit*/
#define TM_DEV_ERROR                 -1 /*revisit*/
#define TM_DEV_RECV_OFFLOAD           1 /* revisit */
#define TM_ETHER_IP_MIN_PACKET        4 /* revisit */
#define TM_USER_BUFFER_LAST           8 /* revisit */
#define TM_ETHER_IP_TYPE              16 /* revisit */
#define TM_ETHER_ARP_TYPE             64 /* revisit */
#define TM_DEV_SET_MCAST_LIST         128 /* revisit */
#define TM_DEV_SEND_COMPLETE_APP      256 /* revisit */
#define TM_DEV_SEND_COMPLETE_DRIVER   512 /* revisit */
#define TM_DEV_SET_ALL_MCAST          1024 /* revisit */
#define TM_ETHER_MAX_PACKET_GUARD     2048 /* revisit */
#define TM_ENETDOWN                   ENETDOWN
#define TM_MAX_PHYS_ADDR              6
#define TM_FAR                        /* ignored */

#define TM_IP_LOCAL_NETMASK ((uint32_t)0x0000ffff)

#ifdef HAVE_IPV6

typedef uint32_t tt6DhcpEvent;

typedef struct { 
    uint32_t addr;
    uint32_t addrPrefLifeTime;
    uint32_t addrValidLifeTime;
    uint32_t addrValue;
    void* next;
} btUserAddrList;
typedef btUserAddrList* btAddrListPtr;

typedef struct {
    uint32_t stubs;
    int btAddrCnt;
    btUserAddrList btAddrList[1];
    char* btFqdnStr;
    int btFqdnFlags;
    int btDomainCnt;
    char* btDomainList[1];
                      } tt6UserBtEntry;
typedef tt6UserBtEntry* tt6UserBtEntryPtr;

#define TM_6_ETHER_IP_TYPE            AF_INET /* revisit */
#define TM_6_DHCP_FQDN_SUPPORTED 1
#define TM_6_DHCP_EVENT_SOLICIT_TIMEOUT (1<<1)
#define TM_6_DHCP_EVENT_NO_SERVER       (1<<2)
#define TM_6_DHCP_EVENT_CONFIRM_FAILED  (1<<3)
#define TM_6_DHCP_EVENT_CONFIRM_OK      (1<<4)
#define TM_6_DHCP_EVENT_INFO_RECEIVED   (1<<5)
#define TM_6_DHCP_EVENT_LEASE_RENEWED   (1<<6)
#define TM_6_DHCP_EVENT_STOPPED         (1<<7)

#define TM_6_MAX_AUTOCONF_IPS_PER_IF  1
#define TM_6_MAX_DHCP_IPS_PER_IA      1
#define TM_6_MAX_LOCAL_MHOME          1
#define TM_6_MAX_MIP_PROXY_IPS_PER_IF 1
#define TM_6_MAX_MHOME (TM_6_MAX_LOCAL_MHOME + TM_6_MAX_MIP_PROXY_IPS_PER_IF)

#define TM_MAX_IPS_PER_IF                1 /* revisit */

#define TM_6_OPTION_IP_FRAG_TTL         (1<<1)
#define TM_6_DEV_USER_DHCP_RTR_CTRLD    (1<<2)
#define TM_6_DHCP_GOPT_RELEASE_MRD      (1<<3)
#define TM_6_DEV_OPTIONS_DHCP_FQDN_PART (1<<4)
#define TM_6_DEV_ADDR_DUP_DETECTED      (1<<5)
#define TM_6_DEV_ADDR_DEPRECATED        (1<<6)
#define TM_6_DEV_ADDR_PREFERRED         (1<<7)
#define TM_6_DEV_ADDR_INVALIDATED       (1<<8)
#define TM_6_DEV_ADDR_ALL_RELEASED      (1<<9)
#define TM_6_DEV_ADDR_ALL_DECLINED      (1<<10)
#define TM_6_DEV_ADDR_CONFIG_STARTED    (1<<11)
#define TM_6_DEV_ADDR_CONFIG_FAILED     (1<<12)
#define TM_6_DEV_ADDR_CONFIG_COMPLETE   (1<<13)

#define tf6GetAddrLifetimes(arg1, arg2, arg3, arg4, arg5) TM_ERROR
#define tf6DhcpGetBootEntry(arg1, arg2, arg3)  (tt6UserBtEntryPtr)NULL
#define tf6DhcpFreeBootEntry(arg1, arg2) /* { (void)0 } */
#define tfNgGetIpAddress(arg1, arg2, arg3, arg4) TM_ERROR
#define tfNgUnConfigInterface(arg1, arg2, arg3) TM_ERROR
#define tfInterfaceSetOptions(arg1, arg2, arg3, arg4)        TM_ENOERROR
#define tfMemSet(arg1, arg2, arg3) memset(arg1, arg2, arg3)
#define tfSetTreckOptions(arg1, arg2) TM_ENOERROR
#define tfNgOpenInterface(arg1, arg2, arg3, arg4, arg5, arg6, arg7) TM_ENOERROR
#define tf6DhcpSetAddrOption(arg1, arg2, arg3, arg4, arg5, arg6, arg7) TM_ENOERROR
#define tfNgConfigInterface(arg1,arg2,arg3, arg4, arg5, arg6, arg7, arg8) TM_ERROR
#define tf6DhcpSetOption(arg1, arg2, arg3) TM_ENOERROR
#define tf6DhcpSetIaOption(arg1,arg2, arg3, arg4, arg5)      TM_ENOERROR
#define tf6DhcpUserStop(arg1, arg2)                          TM_ENOERROR
#define tf6SetMcastInterface(arg1)                           TM_ENOERROR
#define tf6UseDhcp(arg1, arg2, arg3, arg4, arg5, arg6, arg7) TM_ENOERROR
#define tfNgGetPrefixLen(arg1, arg2, arg3, arg4)             TM_ENOERROR

#define IPV6_GLOBAL_AUTOCONFIG_TIMEOUT_SEC 5

#define LINK_PROTOCOL_PPPOE 1 /* revisit */

typedef enum {
    NET_IFACE_IPV6_DHCP_PRIVATE_ENTERPRISE_NUM_HP,
    NET_IFACE_IPV6_DHCP_RELEASE_MRD_SEC,
    NET_IFACE_IPV6_DHCP_DESIRED_T1_SEC,
    NET_IFACE_IPV6_DHCP_DESIRED_T2_SEC,
    NET_IFACE_IPV6_DHCP_DESIRED_PREF_LIFETIME,
    NET_IFACE_IPV6_DHCP_DESIRED_VALID_LIFETIME,
} stubs;
#endif // HAVE_IPV6


#define numberOfNameServers      4 /* revisit */

typedef uint32_t ipv4_addr;
typedef ipv4_addr ttUserIpAddress;
typedef struct UserBootEntry_s {
    ipv4_addr  btuNetBiosNumNameServers[numberOfNameServers];
    ipv4_addr  btuNetBiosNameServers[1];
    ipv4_addr  btuDefRouter;
    ipv4_addr  btuYiaddr;
    ipv4_addr  btuNetMask;
    ipv4_addr  btuDhcpServerId;
    ipv4_addr  btuDhcpLeaseTime;
    ipv4_addr  btuBootSIpAddress;
    ipv4_addr  btuNameLengths[numberOfNameServers];
    ipv4_addr  btuNamesLengths[numberOfNameServers];
    ipv4_addr  btuDomainName;
    ipv4_addr  btuDhcpRxHostNameLength[numberOfNameServers];
    ipv4_addr  btuDns1ServerIpAddress;
    ipv4_addr  btuDns2ServerIpAddress;
    char       btuDhcpRxHostNamePtr[32]; // hostname str size?
} UserBootEntry_t;
typedef UserBootEntry_t* ttUserBtEntryPtr;

typedef uint16_t tt16Bit;
typedef uint32_t tt32Bit;
typedef uint32_t ttUser32Bit;

// in .../http_network_parser.c
#define tfResetConnection(socket)               { (void)0; }
#define tfStrError(errno)                       strerror(errno)
#define tfClose(socket)                         close(socket)
#define tfGetSocketError(errno)                 thread_errno
#define tfDhcpUserGetBootEntry(ti, ignored)    &g_boot_entry_default
#define tfBootpUserGetBootEntry(ti, ignored)   &g_boot_entry_default
#define tfGetIpAddress(ti, v4_addr, multihome) TM_ENOERROR /*&g_v4_net_addr_default*/
#define tfGetNetMask(ti, v4_addr, multihome)   TM_ENOERROR /*&g_v4_net_mask_default*/
#ifdef TM_USE_STRONG_ESL
#define tfGetDefaultGateway(interface, addr)   TM_ENOERROR /*&g_v4_net_addr_default*/
#else
#define tfGetDefaultGateway(addr)              TM_ENOERROR /*&g_v4_net_addr_default*/
#endif

extern UserBootEntry_t g_boot_entry_default;

typedef union { char str4[4]; uint32_t u32; } ttUserGenericUnion;

#define TM_NOERROR        0 /* revisit errno */
#define TM_ERROR         -1 /* revisit errno */
#define TM_ETIMEDOUT      ETIMEDOUT
#define TM_EINPROGRESS    EINPROGRESS
#define TM_EADDRINUSE     EADDRINUSE
#define TM_EHOSTUNREACH   EHOSTUNREACH
#define TM_EALREADY       EALREADY
#define TM_EPERM          1 /* revisit errno */
#define TM_ENOENT         1 /* revisit errno */

#define TM_DEV_IP_USER_BOOT 1
#define TM_DEV_MCAST_ENB  1

#define NET_IFACE_TX_BUFS_PER_FRAME 1 /* revisit os_network.h */
#define NET_IFACE_IPV4_MANUAL_NUM_ARP_PROBES 1 /* revisit os_network.h */
#define NET_IFACE_IPV4_MANUAL_ARP_PROBE_INTERVAL_MS 1 /* revisit os_network.h */
#define NET_IFACE_IPV4_MANUAL_ARP_PROBE_TIMEOUT_MS 1 /* revisit os_network.h */
#define NET_IFACE_IPV4_ARP_ANNOUNCE_INTERVAL_MS 1 /* revisit os_network.h */

#ifdef TM_USE_STRONG_ESL
#define tfCancelCollisionDetection(iface, tmp_addr) TM_ERROR
#define tfDelDefaultGateway(iface, ipv4_gateway) TM_ERROR
#else
#define tfCancelCollisionDetection(iface) TM_ERROR
#define tfDelDefaultGateway(iface) TM_ERROR
#endif
#define tfAddStaticRoute(iface, a1, a2, la, hops)    TM_ENOERROR
#define tfUnConfigInterface(iface, mcast) TM_ERROR
#define tfAddDefaultGateway(intf, router)            TM_ENOERROR
#define tfOpenInterface(intf, x, y, z, w) TM_ERROR
#define tfFinishOpenInterface(intf, x, y)            TM_ENOERROR
#define tfSetMcastInterface(intf, mcast) TM_EALREADY
#define tfUserStartArpSend(x, y, z, a, b) TM_ERROR
#define tfUseCollisionDetection(x, y, z)             TM_ENOERROR
#define tfDhcpUserRelease(intf, mhome) TM_NOERROR
#define tfBootpUserStop(intf,mhome) TM_NOERROR

// catchall
#define impl() DBG_ERR("%s() revisit impl\n", __func__)

#endif // _TRECK_TYPES_ALREADY_DEFINED_

//eof
