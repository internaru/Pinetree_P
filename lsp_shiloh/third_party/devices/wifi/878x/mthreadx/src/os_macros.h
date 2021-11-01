/* 
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2006
 */

#ifndef	_OS_MACROS_H
#define _OS_MACROS_H

#include "marvellTypeMap.h"
#include "tx_api.h"
#include "os_defs.h"
//#include "os_net_device.h"

#ifndef netdev_priv
#define netdev_priv(x) x->priv
#endif

extern VOID Delay(UINT32 delayInUs);

#include <errno.h>
#include <string.h>
//#include "endianSwap.h"
//#include "os_wireless.h"
//#include "agMemory.h"
#include "memAPI.h"
#include "assert.h"
//#include "shimd.h"
//#include "include.h"

struct nlmsghdr {
        __u32           nlmsg_len;      /* Length of message including header */
        __u16           nlmsg_type;     /* Message content */
        __u16           nlmsg_flags;    /* Additional flags */
        __u32           nlmsg_seq;      /* Sequence number */
        __u32           nlmsg_pid;      /* Sending process port ID */
};

#define NLMSG_ALIGNTO   4
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
#define NLMSG_HDRLEN     ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#define NLMSG_LENGTH(len) ((len)+NLMSG_ALIGN(NLMSG_HDRLEN))
#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))
#define NLMSG_DATA(nlh)  ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))


static inline struct net_device* alloc_etherdev(int privSize)
{
    struct net_device* dev = (struct net_device*)MEM_MALLOC(sizeof(struct net_device));
    memset(dev, 0, sizeof(struct net_device));
    dev->priv = MEM_MALLOC(privSize);
    memset(dev->priv, 0, privSize);
    return dev;
}

static inline u32 os_time_get()
{
	u32 time = tx_time_get();
	return time;
}
#define jiffies os_time_get()

#if 0
static inline void OS_SET_THREAD_STATE(int state)
{
	if (state == TASK_INTERRUPTIBLE) {
		//TX_DISABLE
	}
	else if (state == TASK_RUNNING) {
		//TX_RESTORE
	}
}
#endif


static inline void kfree_skb(struct sk_buff * skb)
{
	if(skb->head)
		MEM_FREE_AND_NULL(skb->head);
	MEM_FREE_AND_NULL(skb);
}

#define dev_kfree_skb_any(x) kfree_skb(x)

int os_upload_rx_packet(struct sk_buff *skb);

static inline u8* skb_put(struct sk_buff *skb, u32 len)
{
	u8	*tmp=skb->tail;
	skb->tail += len;
	skb->len = len;
	return tmp;

}
static inline u8* skb_pull(struct sk_buff *skb, u32 len)
{
	skb->len -=len;
	return skb->data += len;	
}

// bqiu
static inline void skb_reserve(struct sk_buff *skb, u32 len)
{
	skb->data += len;
    skb->tail += len;
}

struct sk_buff * alloc_skb( u32 len, int priority );


#define GFP_ATOMIC 0
#define GFP_KERNEL 1
#define DEV_SKB_PAD 16
//struct sk_buff * dev_alloc_skb(u32 len);
static inline struct sk_buff *dev_alloc_skb(u32 len)
{
#if 0
    struct sk_buff *skb = alloc_skb(len + DEV_SKB_PAD, GFP_ATOMIC);
    if (skb)
        skb_reserve(skb, DEV_SKB_PAD);
        //skb_reserve(skb, len + DEV_SKB_PAD);
    return skb;
#else
    struct sk_buff *skb = alloc_skb(len, GFP_ATOMIC);
    return skb;
#endif
}

// bqiu
static inline void os_carrier_on(moal_private *priv)
{
}

static inline void os_carrier_off(moal_private *priv)
{
}

static inline void os_start_queue(moal_private *priv)
{
}

static inline void os_stop_queue(moal_private *priv)
{
}

static inline unsigned long copy_to_user(void *dst, const void *src, unsigned long len)
{
	memcpy(dst, src, len);
    // (DanS): Return 0 for success
    return 0;
}

static inline unsigned long copy_from_user(void *dst, const void *src, unsigned long len)
{
	memcpy(dst, src, len);
    // (DanS): Return 0 for success
    return 0;
}

#if 0
static inline int CopyMulticastAddrs(wlan_adapter *Adapter, struct net_device *dev)
{
	int	i;

	for (i = 0; i < dev->mc_count; i++) {
		memcpy(&Adapter->MulticastList[i], dev->mc_list[i], ETH_ALEN);
	}

	return i;
}
#endif

int wlan_do_ioctl(struct net_device *, struct ifreq *, int);
void wireless_send_event(struct net_device *	dev,
			unsigned int		cmd,
			union iwreq_data *	wrqu,
			char *			extra);
void wpa_driver_wext_event_wireless(void *drvr, void *ctx, char *data, int len);

struct net_device *init_dev(struct net_device *	dev,int y);
#if 0
int PrepareAndSendCommand(moal_private * priv,
	       		u16 cmd_no,
		       	u16 cmd_action,
		      	u16 wait_option,
		       	WLAN_OID cmd_oid,
		      	void *pdata_buf);
#endif

#if 0
void os_free_tx_packet(moal_private *priv);
#endif

#endif	/* _OS_MACROS_H */
