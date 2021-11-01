/* 
 * 	File	: os_misc.c
 *
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2012
 */
#include "tx_api.h"
#include "os_headers.h"
#include "include.h"
#include "os_macros.h"
#include "os_wireless.h"
//#include "agSystemDefs.h"
#include "wlanInterface.h"
//#include "shimd.h"
#if 1
#include "mlan_fw.h"
#else
#include "mlan_scan.h"
#include "mlan_11d.h"
#endif
#include "mlan_join.h"
//#include "mlan_fw.h"
#include "mlan_util.h"
#include "mlan_main.h"
#include "logger.h"
#include "timer_api.h"
#include "wifi_intf_vars.h"
// unsigned long current;

mlan_status wlan_set_wpa_ie_helper(mlan_private * priv, t_u8 * ie_data_ptr, t_u16 ie_len);

extern moal_handle *m_handle;

int event_type_size[] = {
	IW_EV_LCP_LEN,                  /* IW_HEADER_TYPE_NULL */
	0,
	IW_EV_CHAR_LEN,                 /* IW_HEADER_TYPE_CHAR */
	0,
	IW_EV_UINT_LEN,                 /* IW_HEADER_TYPE_UINT */
	IW_EV_FREQ_LEN,                 /* IW_HEADER_TYPE_FREQ */
	IW_EV_ADDR_LEN,                 /* IW_HEADER_TYPE_ADDR */
	0,
	IW_EV_POINT_LEN,                /* Without variable payload */
	IW_EV_PARAM_LEN,                /* IW_HEADER_TYPE_PARAM */
	IW_EV_QUAL_LEN,                 /* IW_HEADER_TYPE_QUAL */
};

static const struct iw_ioctl_description        standard_ioctl[] = {
	/* SIOCSIWCOMMIT */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* SIOCGIWNAME */
	{ IW_HEADER_TYPE_CHAR, 0, 0, 0, 0, IW_DESCR_FLAG_DUMP},
	/* SIOCSIWNWID */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, IW_DESCR_FLAG_EVENT},
	/* SIOCGIWNWID */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, IW_DESCR_FLAG_DUMP},
	/* SIOCSIWFREQ */
	{ IW_HEADER_TYPE_FREQ, 0, 0, 0, 0, IW_DESCR_FLAG_EVENT},
	/* SIOCGIWFREQ */
	{ IW_HEADER_TYPE_FREQ, 0, 0, 0, 0, IW_DESCR_FLAG_DUMP},
	/* SIOCSIWMODE */
	{ IW_HEADER_TYPE_UINT, 0, 0, 0, 0, IW_DESCR_FLAG_EVENT},
	/* SIOCGIWMODE */
	{ IW_HEADER_TYPE_UINT, 0, 0, 0, 0, IW_DESCR_FLAG_DUMP},
	/* SIOCSIWSENS */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCGIWSENS */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCSIWRANGE */	
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* SIOCGIWRANGE */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, 0, IW_DESCR_FLAG_DUMP},//sizeof(struct iw_range), IW_DESCR_FLAG_DUMP},
	/* SIOCSIWPRIV */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* SIOCGIWPRIV (handled directly by us) */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* SIOCSIWSTATS */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* SIOCGIWSTATS (handled directly by us) */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, IW_DESCR_FLAG_DUMP},
	/* SIOCSIWSPY */
	{ IW_HEADER_TYPE_POINT, 0, 0, 0, 0}, //sizeof(struct sockaddr), 0, IW_MAX_SPY, 0},
	/* SIOCGIWSPY */
	{ IW_HEADER_TYPE_POINT, 0, 0, 0, 0}, 
//{ IW_HEADER_TYPE_POINT, 0, (sizeof(struct sockaddr) + sizeof(struct iw_quality)), 0, IW_MAX_GET_SPY, 0},
	/* -- hole -- */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* -- hole -- */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* SIOCSIWAP */
	{ IW_HEADER_TYPE_ADDR, 0, 0, 0, 0, 0},
	/* SIOCGIWAP */
	{ IW_HEADER_TYPE_ADDR, 0, 0, 0, 0, IW_DESCR_FLAG_DUMP},
	/* -- hole -- */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* SIOCGIWAPLIST */
	{ IW_HEADER_TYPE_POINT, 0, 0, 0, 0}, 
//{ IW_HEADER_TYPE_POINT, 0, (sizeof(struct sockaddr) + sizeof(struct iw_quality)), 0, IW_MAX_AP, 0},
	/* SIOCSIWSCAN */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCGIWSCAN */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, IW_SCAN_MAX_DATA, 0},
	/* SIOCSIWESSID */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, IW_ESSID_MAX_SIZE + 1, IW_DESCR_FLAG_EVENT},
	/* SIOCGIWESSID */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, IW_ESSID_MAX_SIZE + 1, IW_DESCR_FLAG_DUMP},
	/* SIOCSIWNICKN */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, IW_ESSID_MAX_SIZE + 1, 0},
	/* SIOCGIWNICKN */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, IW_ESSID_MAX_SIZE + 1, 0},
	/* -- hole -- */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* -- hole -- */
	{ IW_HEADER_TYPE_NULL, 0, 0, 0, 0, 0},
	/* SIOCSIWRATE */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCGIWRATE */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCSIWRTS */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCGIWRTS */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCSIWFRAG */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCGIWFRAG */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCSIWTXPOW */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCGIWTXPOW */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCSIWRETRY */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCGIWRETRY */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCSIWENCODE */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, IW_ENCODING_TOKEN_MAX, IW_DESCR_FLAG_EVENT | IW_DESCR_FLAG_RESTRICT},
	/* SIOCGIWENCODE */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, IW_ENCODING_TOKEN_MAX, IW_DESCR_FLAG_DUMP | IW_DESCR_FLAG_RESTRICT},
	/* SIOCSIWPOWER */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
	/* SIOCGIWPOWER */
	{ IW_HEADER_TYPE_PARAM, 0, 0, 0, 0, 0},
};
 static const int standard_ioctl_num = (sizeof(standard_ioctl) /
                                        sizeof(struct iw_ioctl_description));

static const struct iw_ioctl_description        standard_event[] = {
	/* IWEVTXDROP */
	{ IW_HEADER_TYPE_ADDR, 0, 0, 0, 0, 0},
	/* IWEVQUAL */
	{ IW_HEADER_TYPE_QUAL, 0, 0, 0, 0, 0},
	/* IWEVCUSTOM */
	{ IW_HEADER_TYPE_POINT, 0, 1, 0, IW_CUSTOM_MAX, 0},
	/* IWEVREGISTERED */
	{ IW_HEADER_TYPE_ADDR, 0, 0, 0, 0, 0},
	/* IWEVEXPIRED */
	{ IW_HEADER_TYPE_ADDR, 0, 0, 0, 0, 0},
};

static const int standard_event_num = (sizeof(standard_event) /
			sizeof(struct iw_ioctl_description));

/* return net_device structure to SHIMD*/
struct net_device *wlan_get_net_dev() 
{
    //if ((m_handle!=NULL) && (m_handle->priv[m_handle->priv_num]!=NULL))
    if ((m_handle!=NULL) && (m_handle->priv[0]!=NULL))
    {
        return m_handle->priv[0]->netdev; 
    }
    return NULL;
}

struct net_device *wlan_get_net_uap_dev() 
{
    if ((m_handle!=NULL) && (m_handle->priv[1]!=NULL))
    {
        return m_handle->priv[1]->netdev; 
    }
    return NULL;
}

#ifdef WIFI_DIRECT_SUPPORT
struct net_device *wlan_get_net_wfd_dev() 
{
    if ((m_handle!=NULL) && (m_handle->priv[2]!=NULL))
    {
        return m_handle->priv[2]->netdev; 
    }
    return NULL;
}
#endif



int wlan_get_scan_wpa(mlan_private *priv, struct iw_request_info *info, 
			struct iw_point *dwrq, struct SCAN_SSID_IE *scan_res)
{
	int 			i, min_aps = 0, ap_count = 0;
    uint8_t         scan_config; 
	mlan_adapter		*Adapter = priv->adapter;

	ENTER();

    scan_config = wifi_get_scan_config();
    dbg_printf("wlan_get_scan_wpa Adapter->num_in_scan_table = 0x%X\n",Adapter->num_in_scan_table);

	min_aps = (dwrq->length < Adapter->num_in_scan_table) ?
			dwrq->length : Adapter->num_in_scan_table;
			
    //printk("wlan_get_scan_wpa scan table %d\n", min_aps);
	
    for(i = 0; i < min_aps; i++)
    {
        if (scan_config == SCAN_2_4GHZ_ONLY)
        {
            //If we have scan restriction in place for 2.4GHz only, do not associate with an AP advertising in the
            //5GHz band
            if (Adapter->pscan_table[i].channel > MAX_CHANNELS_BG)
                continue;                
        }
        
        ap_count++;    
        scan_res[i].ssid_len = Adapter->pscan_table[i].ssid.ssid_len;
        memcpy(scan_res[i].SSID, Adapter->pscan_table[i].ssid.ssid, 
               Adapter->pscan_table[i].ssid.ssid_len);
        memcpy(scan_res[i].BSSID, Adapter->pscan_table[i].mac_address, 
               ETH_ALEN);
        scan_res[i].channel = Adapter->pscan_table[i].channel;
        memcpy(&scan_res[i].Capability, &Adapter->pscan_table[i].cap_info,sizeof(IEEEtypes_CapInfo_t)); 
        // prefer the RSN IE to the WPA IE (DanS)
        if (Adapter->pscan_table[i].prsn_ie)
        {
            //printk("wlan_get_scan_wpa table %d ssid %s\n", i, Adapter->pscan_table[i].ssid.ssid);

            if ((Adapter->pscan_table[i].prsn_ie->ieee_hdr.element_id == RSN_IE) && (Adapter->pscan_table[i].prsn_ie->ieee_hdr.len > 0))
            {
                scan_res[i].wpa_ie_len = Adapter->pscan_table[i].prsn_ie->ieee_hdr.len
                              + 2; 
                memcpy(scan_res[i].WPA_IE, (Adapter->pscan_table[i].prsn_ie), scan_res[i].wpa_ie_len); 
            }
        } 
        else if (Adapter->pscan_table[i].pwpa_ie)
        {
            scan_res[i].wpa_ie_len = Adapter->pscan_table[i].pwpa_ie->vend_hdr.len
                          + 2; 
            memcpy(scan_res[i].WPA_IE, (Adapter->pscan_table[i].pwpa_ie), scan_res[i].wpa_ie_len); 
            
            //PRINTM(INFO, "wlan_get_scan_wpa wpa IE len %d\n", scan_res[i].wpa_ie_len);
        }
    }
	
	dwrq->length = ap_count;
	
	LEAVE();
	return 0;
}


/*
int wlan_extscan_ioctl(mlan_private *priv,WLAN_802_11_SSID Ext_Scan_SSID )
{

	SendSpecificScan(priv, &Ext_Scan_SSID);

	LEAVE();
	return 0;
}
*/

void creat_del_evnt_flags(u8 set_reset) 
{
    return ;
    
    /*
	if (!set_reset) {
		tx_event_flags_delete(&wlanpriv->adapter->scan_q);
	} */
}


#if 0
// The current tx SKB must always be free using os_free_tx_packet.
// SKBs on the wmm lists that are not the CurrentTxSkb must be freed
// using the call to os_free_tx_skb.
// RX Skbs allocated by the driver are the only SKBs that are safe to
// free using kfree_skb
void os_free_tx_packet(mlan_private *priv)
{
	char	*pktBuf;
	u32	pktLen, flags;
	mlan_adapter *Adapter = priv->adapter;
	
	/* 
	 * we set the CurrentTxSkb = NULL before we indicate that 
	 * the TX is complete. This fixes a problem where we have a high  
	 * number of dropped tx packets. The dropped tx packets were being 
	 * caused because the next tx was done in the context of the 
	 * WlanShimDTxPktSent callup before the currentTxSkb had been 
	 * NULLED out. Since CurrentTxSkb was non-null, the tx was dropped.
	 */

	spin_lock_irqsave(&Adapter->CurrentTxLock, flags);

    if(Adapter->CurrentTxSkb)
    {
        pktBuf = Adapter->CurrentTxSkb->data;
        pktLen =  Adapter->CurrentTxSkb->len;
        Adapter->CurrentTxSkb = NULL;
	
        spin_unlock_irqrestore(&Adapter->CurrentTxLock, flags);

        WlanShimDTxPktSent(pktBuf, pktLen);
        PRINTM(INFO, "Skb Freed $$\n");
    }
    else
    {
        spin_unlock_irqrestore(&Adapter->CurrentTxLock, flags);
    }
}
#endif


#if 0
// free a tx skb; should only be used on wmm skbs, never on the CurrentTxSkb.
void os_free_tx_skb(struct sk_buff *skb)
{
    WlanShimDTxPktSent(skb->data, skb->len, skb->handle);
}
#endif


int wlan_setwpaie_ioctl(mlan_private *priv, char* wpaie, int wpaie_len)
{
	//mlan_adapter	*Adapter = priv->adapter;
	int ret = 0;

	ENTER();
	
	wlan_set_wpa_ie_helper(priv, (unsigned char *)wpaie, wpaie_len);	

	LEAVE();
	return ret;
}

void wireless_send_event(struct net_device *    dev,
                          unsigned int           cmd,
                          union iwreq_data *     wrqu,
                          char *                 extra)
{
         const struct iw_ioctl_description *     descr = NULL;
         struct iw_event*  event;                
         int extra_len = 0;
         int event_len;                         
         int hdr_len;                          
         unsigned        cmd_index;              

     	 printk("!!! wireless_send_event cmd = %#x !!!\n", cmd);
 
         /* Get the description of the IOCTL */
         if(cmd <= SIOCIWLAST) {
                cmd_index = cmd - SIOCIWFIRST;
                 if(cmd_index < standard_ioctl_num)
                         descr = &(standard_ioctl[cmd_index]);
         } else {
                 cmd_index = cmd - IWEVFIRST;
                 if(cmd_index < standard_event_num)
                         descr = &(standard_event[cmd_index]);
         }
         /* Don't accept unknown events */
         if((descr == NULL) && (cmd != IWEVMICHAELMICFAILURE)) {
                 /* Note : we don't return an error to the driver, because
                  * the driver would not know what to do about it. It can't
                  * return an error to the user, because the event is not
                  * initiated by a user request.
                  * The best the driver could do is to log an error message.
                  * We will do it ourselves instead...
                 printk( "%s (WE) : Invalid/Unknown Wireless Event (0x%04X)\n",
                       dev->name, cmd);
                  */
                 printk( "OS_MISC.c:Invalid/Unknown Wireless Event \n");
                 return;
         }

         /* Check extra parameters and set extra_len */
         if((descr != NULL) && (descr->header_type == IW_HEADER_TYPE_POINT)) {
         /* Check if number of token fits within bounds */
                 if(wrqu->data.length > descr->max_tokens) {
                         printk( " (WE) : Wireless Event too big ()\n");
                         return;
                 }
                 if(wrqu->data.length < descr->min_tokens) {
                         printk( " (WE) : Wireless Event too small ()\n");
                         return;
                 }
                 /* Calculate extra_len - extra is NULL for restricted events */
                 if(extra != NULL)
                         extra_len = wrqu->data.length * descr->token_size;
         }
         else if (cmd == IWEVMICHAELMICFAILURE)
         {
             extra_len = wrqu->data.length;
         }
 
         /* Total length of the event */
         if (cmd != IWEVMICHAELMICFAILURE)
         {
             hdr_len = event_type_size[descr->header_type];
         }
         else
         {
             hdr_len = IW_EV_LCP_LEN;
         }
         
         event_len = hdr_len + extra_len;
 
         //event = (struct iw_event*)getMemory(event_len, SCRATCHALLOC);
         event = (struct iw_event*) MEM_MALLOC(event_len);
 
         /* Fill event */
         event->len = event_len;
         event->cmd = cmd;
         if (cmd != IWEVMICHAELMICFAILURE)
         {
             memcpy(&event->u, wrqu, hdr_len - IW_EV_LCP_LEN);
         }
         

         if(extra != NULL)
                 memcpy(((char *)event) + hdr_len, extra, extra_len);

         printk("CALLING wpa_driver_wext_event_wireless event_len = %d!!!!!\n", event_len);
         wpa_driver_wext_event_wireless(NULL, NULL, (char *) event, event_len);
 
         MEM_FREE_AND_NULL(event);
         return;         /* Always success, I guess  */
}

#if 0
struct net_device *init_etherdev(struct net_device * dev, int y) 
{ 
	printk(" allocation enter priv size %d %d\n", y, sizeof(mlan_private));
	dev =  (struct net_device *) MEM_MALLOC(sizeof(struct net_device)); 
  
	printk(" dev->priv allocation \n");
	printk(" before memset wlanpriv %p\n", wlanpriv);
   
	memset(dev ,0,sizeof(struct net_device)); 
	printk(" between memset \n");

	wlanpriv =  (mlan_private *) MEM_MALLOC(sizeof(mlan_private));
	memset(wlanpriv ,0,sizeof(mlan_private));
	dev->priv = wlanpriv;
  
  	printk(" After memset \n");

  	return(dev);
}
#endif

//struct sk_buff * dev_alloc_skb( u32 len)

struct sk_buff * alloc_skb( u32 len, int priority)
{
	struct sk_buff *skb = NULL;

	skb =  (struct sk_buff *) MEM_CALLOC(1, sizeof(struct sk_buff)); 

	if (skb)
    {
        if (len) 
        {
            skb->data = (u8 *) MEM_CALLOC_ALIGN(1,len,DMA_ALIGNMENT);

            if (skb->data == NULL) {
                MEM_FREE_AND_NULL(skb);
                return NULL;
            }
        }
        else
            skb->data = NULL;
    }        
	else
		return NULL;


	INIT_LIST_HEAD(skb);	
	skb->tail=skb->data;
	skb->head=skb->data; // bqiu
	skb->len=len;

	return skb;
}

/*
 *  This simulates the UNIX call for gettimeofday. 
 * 
*/
void do_gettimeofday(struct timeval *time)
{
    UINT32 localTime = 0;
    if(time == NULL)
        return;
#if 0
    localTime = tx_time_get();

    // REVIST: this may rollover too often.  we may need to do something else.
    time->tv_sec = localTime/SYS_TICK_FREQ;
    // our tick resolution is in ms, so figure out how many ticks are left and
    // then calculate ms/tick (1000/SYS_TICK_FREQ) and multiply by 100 to convert to us before we do the final calc.
    time->tv_usec = (localTime % SYS_TICK_FREQ) * (1000/SYS_TICK_FREQ * 100);
#else
    localTime = timer_get_time_usec();

    time->tv_sec = localTime/(1000*1000);
    time->tv_usec = localTime - time->tv_sec*(1000*1000);
#endif
    return;
}

#if 1
int os_upload_rx_packet(struct sk_buff *skb)
{
	char	*dataBuf;
	void	*bufDesc;
	int	bufLen;

    MASSERT(skb != NULL);

	// bqiu
#ifdef SNIFFER_MODE
	if (priv->adapter->snifferaction == SNIFFER_MODE_ON) {
		wsniffer_enqueue_rx_packet(skb);
		return 0;
	}
#endif 	

	WlanShimDGetRxBuf(&dataBuf, &bufLen, &bufDesc);

	if (bufDesc == NULL) {
		PRINTM(INFO, "No rxBuf free!");
		return -1;
	}

    MASSERT(dataBuf != NULL);
    MASSERT(bufLen != 0);

    if (skb->len <= bufLen) {
        memcpy(dataBuf, skb->data, skb->len);
        WlanShimDRecvPacket((unsigned char *)dataBuf, skb->len, bufDesc, (int) skb->handle);
    } else {
        // Bad things happened, try to return buffer
        PRINTM(ERROR, "os_upload_rx_packet:  Error skb->len too large 0x%X!!!!!\n", skb->len);
        WlanShimDRecvPacket((unsigned char *)dataBuf, 0x0, bufDesc, (int) skb->handle);
    }
       
    kfree_skb(skb); //bqiu
	return 0;
}
#endif



