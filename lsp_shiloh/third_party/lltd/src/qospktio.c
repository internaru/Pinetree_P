/*
 * LICENSE NOTICE.
 *
 * Use of the Microsoft Windows Rally Development Kit is covered under
 * the Microsoft Windows Rally Development Kit License Agreement,
 * which is provided within the Microsoft Windows Rally Development
 * Kit or at http://www.microsoft.com/whdc/rally/rallykit.mspx. If you
 * want a license from Microsoft to use the software in the Microsoft
 * Windows Rally Development Kit, you must (1) complete the designated
 * "licensee" information in the Windows Rally Development Kit License
 * Agreement, and (2) sign and return the Agreement AS IS to Microsoft
 * at the address provided in the Agreement.
 */

/*
 * Copyright (c) Microsoft Corporation 2005.  All rights reserved.
 * This software is provided with NO WARRANTY.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "globals.h"
#include "statemachines.h"
#include "packetio.h"

extern void *
fmt_base(uint8_t *buf, const etheraddr_t *srchw, const etheraddr_t *dsthw, lld2_tos_t tos,
	 topo_opcode_t opcode, uint16_t seqnum, bool_t use_broadcast);

extern void
tx_write(uint8_t *buf, size_t nbytes);

/* with only two linkages, this isn't really needed...  #include "qospktio.h" */

/********************************* U T I L I T Y   F U N C T I O N S **************************************/
static
void
stamp_time(uint64_t* pTime)
{
    struct timeval now;
    uint64_t	   temp;

    gettimeofday(&now, NULL);
    temp = now.tv_sec * (uint64_t)1000000UL;
    *pTime = temp + now.tv_usec;
}


static
void
get_raw_samples(void)
{
    FILE  *procdev;
    int rif = RIF;

    ASSERT(rif != -1);
    char *buf = IF_INFO(rif)->gbuf;

    ASSERT(g_qos);
    qos_info_t *qos = g_qos + rif;

    if ( (procdev = fopen("/proc/net/dev", "r")) != (FILE*) 0)
    {
        uint32_t     rbytes, rpkts, tbytes, tpkts;
        bool_t       cntrs_parsedOK = FALSE;
        char         ifname[16];
        
        rbytes = rpkts = tbytes = tpkts = -1;
        strncpy(ifname, IF_NAME(rif), 14);
        strcat(ifname, ":");

        while (fgets(buf, sizeof(buf) / sizeof(buf[0]), procdev) == buf)
        {
            char *ifn;

            if ((ifn = strstr(buf, ifname)) != 0)
            {
                int   skipcol;
                char *val = ifn;
                char  dummy[] = {"0 0 0 0 0 0"};

                val += strlen(ifname);
                rbytes = strtoul(val,&val,10);
                rpkts  = strtoul(val,&val,10);
                /* Skip over 6 more un-needed columns */
                for (skipcol = 0;skipcol<6;skipcol++)
                {
                    long  discard;

                    discard = strtoul(val,&val,10);
                    if (*val == '\0')
                    {
                        warn("get_raw_samples: using dummy values due to parse error!\n");
                        val = dummy;
                        break;
                    }
                }
                /* "val" now points to the tx-byte-counter */
                tbytes = strtoul(val,&val,10);
                tpkts  = strtoul(val,NULL,10);
                cntrs_parsedOK = TRUE;
                break;  // out of "while (reading lines)..."
            }
        }
        if (cntrs_parsedOK)
        {
            warn("get_raw_samples: failed reading /proc/dev for device statistics!\n");
            qos->rbytes = qos->rpkts = qos->tbytes = qos->tpkts  = 0;
        } else {
            /* got a valid parse of the /proc/net/dev line for our device */
            qos->rbytes = rbytes;
            qos->rpkts  = rpkts;
            qos->tbytes = tbytes;
            qos->tpkts  = tpkts;
        }
        fclose(procdev);
    } else {
        warn("get_raw_samples: failed opening /proc/dev for device statistics!\n");
        qos->rbytes = qos->rpkts = qos->tbytes = qos->tpkts  = 0;
    }
    IF_TRACED(TRC_QOS)
	dbgprintf("qos perf-cntr: rbytes=" FMT_UINT32 "; rpkts=" FMT_UINT32 \
	          "; tbytes=" FMT_UINT32 "; tpkts=" FMT_UINT32 "\n",
	          qos->rbytes, qos->rpkts, qos->tbytes, qos->tpkts);
    END_TRACE
}


static
void
get_timestamp(uint64_t* pTime)
{
    struct timeval now;
    uint64_t	   temp;

    ioctl(IF_SOCK(RIF), SIOCGSTAMP, &now);
    temp = now.tv_sec * (uint64_t)1000000UL;
    *pTime = temp + now.tv_usec;
}


static qos_session_t*
qos_find_session(void)
{
    unsigned int i;

    int rif = RIF;
    ASSERT(g_qos != NULL && rif != -1);
    qos_info_t *qos = g_qos + rif;
    osl_interface_info_t *if_info = IF_INFO(rif);

    for (i=0;i<MAX_QOS_SESSIONS;i++)
    {
        if (qos->QosSessions[i].qssn_is_valid  &&
            ETHERADDR_EQUALS(&(if_info->base_hdr->tbh_realsrc),
                                &(qos->QosSessions[i].qssn_ctrlr_real)))
        {
            return &(qos->QosSessions[i]);
        }        
    }
    return  NULL;
}

static qosEventBucket_t*
qos_find_bucket(qos_session_t *pSsn)
{
    unsigned int i = pSsn->qssn_first_bucket;
    unsigned int j = i + pSsn->qssn_num_active_buckets;

    while (i < j)
    {
        qosEventBucket_t* bucket = &pSsn->qssn_evt_buckets[i & (MAX_QOS_BUCKETS - 1)];

        if (bucket->evt_seqNum == IF_INFO(RIF)->sequencenum)
        {
            return bucket;
        }

        i++;
    }

    return NULL;
}

/***************************** T I M E R   S E R V I C E   R O U T I N E S *****************************/
static
void
qos_inactivity_timeout(void *state)
{
    int i;
    struct timeval now;

    int rif = RIF;
    ASSERT(g_qos && rif != -1);
    qos_info_t *qos = g_qos + rif;
    qos_session_t *pSsn = qos->QosSessions;

    for (i = 0; i < MAX_QOS_SESSIONS; i++, pSsn++)
    {
        if (pSsn->qssn_is_valid == TRUE)
        {
            if (pSsn->qssn_ticks_til_discard)
                pSsn->qssn_ticks_til_discard--;
            if (pSsn->qssn_ticks_til_discard == 0)
            {
                pSsn->qssn_is_valid = FALSE;
                pSsn->qssn_num_active_buckets = 0;
            }
        }
    }

    /* repeats every 30 sec - a session is killed after 4 ticks' inactivity */
    gettimeofday(&now, NULL);
    now.tv_sec += 30;
    qos->inactivity_timer = event_add(rif, &now, qos_inactivity_timeout, NULL);
}


#define BYTE_SCALE_FACTOR 1024  // value for reducing counters
#define BYTE_SCALE        0     // equivalent value for response
#define PKT_SCALE_FACTOR  1     // value for reducing counters
#define PKT_SCALE         0     // equivalent value for response
static
void
interface_counter_recorder(void *state)
{
    struct timeval now;

    int rif = RIF;
    ASSERT(g_qos && rif != -1);
    qos_info_t *qos = g_qos + rif;

    if (--qos->samples_remaining != 0)
    {
        qos_perf_sample *thisSample = &qos->perf_samples[qos->next_sample];

        uint32_t       rbytes = qos->rbytes;
        uint32_t       rpkts  = qos->rpkts;
        uint32_t       tbytes = qos->tbytes;
        uint32_t       tpkts  = qos->tpkts;
        uint32_t       delta;
    
        /* get current values for rbytes, rpkts, tbytes, tpkts */
        get_raw_samples();

        delta = (qos->rbytes-rbytes > 0) ? (qos->rbytes-rbytes) : 0;
        IF_TRACED(TRC_QOS)
            dbgprintf("qos perf-cntr: delta-rbytes=" FMT_UINT32, delta);
        END_TRACE
        thisSample->bytes_rcvd = delta / BYTE_SCALE_FACTOR;

        delta = (qos->rpkts-rpkts > 0) ?   (qos->rpkts-rpkts) : 0;
        IF_TRACED(TRC_QOS)
            dbgprintf("  delta-rpkts=" FMT_UINT32, delta);
        END_TRACE
        thisSample->pkts_rcvd  = delta / PKT_SCALE_FACTOR;

        delta = (qos->tbytes-tbytes > 0) ? (qos->tbytes-tbytes) : 0;
        IF_TRACED(TRC_QOS)
            dbgprintf("  delta-tbytes=" FMT_UINT32, delta);
        END_TRACE
        thisSample->bytes_sent = delta / BYTE_SCALE_FACTOR;

        delta = (qos->tpkts-tpkts > 0) ?   (qos->tpkts-tpkts) : 0;
        IF_TRACED(TRC_QOS)
            dbgprintf("  delta-tpkts=" FMT_UINT32 "\n", delta);
        END_TRACE
        thisSample->pkts_sent  = delta / PKT_SCALE_FACTOR;

        stamp_time(&qos->perf_timestamp);

        IF_TRACED(TRC_QOS)
            dbgprintf("qos perf-cntr: sample-rbytes=%d; sample-rpkts=%d; sample-tbytes=%d; sample-tpkts=%d\n", thisSample->bytes_rcvd, thisSample->pkts_rcvd, thisSample->bytes_sent, thisSample->pkts_sent);
        END_TRACE

        qos->next_sample++;
        qos->next_sample = qos->next_sample % 60;

        if (qos->sample_count < 60)
        {
            qos->sample_count++;
        }

        /* repeats every second - until the lease runs out */
        gettimeofday(&now, NULL);
        now.tv_sec += 1;
        qos->CTA_timer = event_add(rif, &now, interface_counter_recorder, NULL);
    } else {
        IF_TRACED(TRC_QOS)
            dbgprintf("qos perf-cntr: lease has run out - zero'ing counters, and stopping the timer...\n");
        END_TRACE

        qos->next_sample = 0;
        qos->sample_count = 0;
    }
}


void
qos_init(void)
{
    ASSERT(IF_NUM);
    g_qos = xmalloc(IF_NUM * sizeof(qos_info_t));

    /* Initialize all the session structures */
    int i, j;
    for (i = 0; i < IF_NUM; i++)
    {
        qos_info_t *pQos = g_qos + i;
        qos_session_t *pSsn;
        for (j = 0, pSsn = pQos->QosSessions; j < MAX_QOS_SESSIONS; j++, pSsn++)
        {
            pSsn->qssn_ticks_til_discard = 0;
            pSsn->qssn_first_bucket = 0;
            pSsn->qssn_num_active_buckets = 0;
            pSsn->qssn_is_valid = FALSE;
            memset(&pSsn->qssn_ctrlr_real, 0, sizeof(etheraddr_t));
        }
        pQos->qprb_hdr = NULL;
        pQos->qinit_hdr = NULL;
        pQos->LinkSpeed = 1000000;
        pQos->TimeStampFreq = 1000000;
        pQos->pktio_timestamp = 0;

        struct timeval now;
        gettimeofday(&now, NULL);
        now.tv_sec += 30;
        pQos->inactivity_timer =
            event_add(i, &now, qos_inactivity_timeout, NULL);

#ifdef START_WITH_COUNTER_LEASE
        /* start collection: repeats every second - until the lease runs out */
        gettimeofday(&now, NULL);
        now.tv_sec += 1;
        pQos->CTA_timer =
            event_add(i, &now, interface_counter_recorder, NULL);
        pQos->samples_remaining = 300;
#endif
    }
}


/**************************  Q O S   M S G   H A N D L E R S   **************************/

static
void
qos_initsink(void)
{
    qos_session_t*	pThisSsn;
    size_t		nbytes;
    int			i;
    uint16_t	errcode;

    int rif = RIF;
    ASSERT(g_qos != NULL && rif != -1);
    qos_info_t *qos = g_qos + rif;
    osl_interface_info_t *if_info = IF_INFO(rif);

    if (if_info->rcvd_pkt_len < sizeof(topo_ether_header_t) +
            sizeof(topo_base_header_t) +  sizeof(qos_initsink_header_t))
    {
        warn("qos_initsink: frame with truncated InitializeSink header (len=" FMT_SIZET " src=" ETHERADDR_FMT " dst=" ETHERADDR_FMT "); ignoring\n", if_info->rcvd_pkt_len, ETHERADDR_PRINT(&if_info->ethernet_hdr->eh_src), ETHERADDR_PRINT(&if_info->ethernet_hdr->eh_dst));
        return;
    }

    /* Check interrupt moderation request */
    if (qos->qinit_hdr->init_intmod_ctrl != 0xFF)
    {
        /* Compose msg to return */
        fmt_base(
            if_info->txbuf,
            &if_info->hw_addr,
            &if_info->base_hdr->tbh_realsrc,
            ToS_QoSDiagnostics,
            (topo_opcode_t)Qopcode_Error,
            if_info->sequencenum,
            FALSE /*no broadcast*/
            );
        nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

        /* add Error-Code */
        errcode = htons(Qoserror_ModerationNotAvailable);
        memcpy(&if_info->txbuf[nbytes], &errcode, sizeof(uint16_t));
        nbytes += sizeof(uint16_t);

        tx_write(if_info->txbuf, nbytes);

        IF_TRACED(TRC_PACKET)
            dbgprintf("qos_initsink: unsupported interrupt moderation request (intmod=0x%02x)\n", qos->qinit_hdr->init_intmod_ctrl);
        END_TRACE

        return;
    }

    /* Check for existing session with this controller and use it, if found; */
    /* If not found (the usual case), get an unused one... */
    if ((pThisSsn = qos_find_session()) == NULL)    
    {
        /* Check for available session slot, reject with ErrBusy if none available */
        pThisSsn = qos->QosSessions;
        for (i=0;i<MAX_QOS_SESSIONS;i++)
        {
            if (!pThisSsn->qssn_is_valid)  break;
            pThisSsn++;
        }
        if (i>=MAX_QOS_SESSIONS)
        {
            /* Compose Busy msg to return. */
            fmt_base(if_info->txbuf, &if_info->hw_addr, &if_info->base_hdr->tbh_realsrc, ToS_QoSDiagnostics,
                 Qopcode_Error, if_info->sequencenum, FALSE /*no broadcast*/);
            nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

            /* add Error-Code */

            errcode = htons(Qoserror_Busy);
            memcpy(&if_info->txbuf[nbytes], &errcode, sizeof(uint16_t));
            nbytes += sizeof(uint16_t);

            tx_write(if_info->txbuf, nbytes);

            IF_TRACED(TRC_PACKET)
                dbgprintf("qos_initsink: tx_error_Busy, seq=%d -> "
                        ETHERADDR_FMT "\n", if_info->sequencenum,
                        ETHERADDR_PRINT(&if_info->base_hdr->tbh_realsrc));
            END_TRACE
            return;
        }
    }

    /* Record session data, */
    pThisSsn->qssn_is_valid = TRUE;
    memcpy( &pThisSsn->qssn_ctrlr_real, &if_info->base_hdr->tbh_realsrc, sizeof(etheraddr_t) );
    pThisSsn->qssn_ticks_til_discard = 4;

    /* and compose & send a Ready msg. */
    fmt_base(if_info->txbuf, &if_info->hw_addr, &pThisSsn->qssn_ctrlr_real, ToS_QoSDiagnostics,
             Qopcode_Ready, if_info->sequencenum, FALSE /*no broadcast*/);
    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

    /* add Sink Link Speed */
    qos->reorder_buffer = htonl(qos->LinkSpeed);
    memcpy(&if_info->txbuf[nbytes], &qos->reorder_buffer, sizeof(uint32_t));
    nbytes += sizeof(uint32_t);

    /* add Performance Counter Frequency */

    cpy_hton64(&if_info->txbuf[nbytes], &qos->TimeStampFreq);
    nbytes += sizeof(uint64_t);

    tx_write(if_info->txbuf, nbytes);

    IF_TRACED(TRC_PACKET)
	dbgprintf("qos_initsink: tx_ready, seq=%d -> " ETHERADDR_FMT "\n",
		if_info->sequencenum, ETHERADDR_PRINT(&pThisSsn->qssn_ctrlr_real));
    END_TRACE
}


static
void
qos_reset(void)
{
    qos_session_t*	pThisSsn = qos_find_session();
    size_t		nbytes;

    int rif = RIF;
    ASSERT(g_qos != NULL && rif != -1);
    qos_info_t *qos = g_qos + rif;
    osl_interface_info_t *if_info = IF_INFO(RIF);

    /* Find associated session, and reject with silence if none found. */
    if (pThisSsn == NULL)
    {
        warn("packetio_recv_handler: no session active for " ETHERADDR_FMT "; ignoring...\n", ETHERADDR_PRINT(&if_info->base_hdr->tbh_realsrc));
        return;
    }

    /* Otherwise, clear the associated session */
    pThisSsn->qssn_is_valid = FALSE;
    pThisSsn->qssn_ticks_til_discard = 0;
    pThisSsn->qssn_num_active_buckets = 0;

    /* and compose & send an ACK msg */

    fmt_base(if_info->txbuf, &if_info->hw_addr, &pThisSsn->qssn_ctrlr_real, ToS_QoSDiagnostics,
             Qopcode_ACK, if_info->sequencenum, FALSE /*no broadcast*/);
    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

    tx_write(if_info->txbuf, nbytes);

    IF_TRACED(TRC_PACKET)
	dbgprintf("qos_reset: tx_ack, seq=%d -> " ETHERADDR_FMT "\n",
		if_info->sequencenum, ETHERADDR_PRINT(&pThisSsn->qssn_ctrlr_real));
    END_TRACE
}



static
void
qos_probe(void)
{
    qos_session_t*	pThisSsn;

    int rif = RIF;
    ASSERT(g_qos != NULL && rif != -1);
    qos_info_t *qos = g_qos + rif;
    osl_interface_info_t *if_info = IF_INFO(RIF);

    /* Pick up the rx-timestamp from the driver, put in global timestamp */
    get_timestamp(&qos->pktio_timestamp);

    /* Find associated session, and reject with silence if none found. */
    pThisSsn = qos_find_session();

    if (pThisSsn == NULL)
    {
        IF_TRACED(TRC_QOS)
            dbgprintf("qos_probe: no matching session found; ignoring.\n");
        END_TRACE
        return;
    }

    /* Valid session - mark it as "still active" */
    pThisSsn->qssn_ticks_til_discard = 4;

    /* If ProbeGap, just stamp it and reflect it, unless it has an 802.1p field */
    if (qos->qprb_hdr->probe_testtype == 1)
    {
        if ((qos->qprb_hdr->probe_pqval & 0x80) == 0)
        {
            /* change the test-type to 2 when reflecting */
            qos->qprb_hdr->probe_testtype = 2;

            /* setup ethernet and Base-hdr source and dest addresses to return to sender */
            memcpy(&if_info->ethernet_hdr->eh_dst, &if_info->base_hdr->tbh_realsrc, sizeof(etheraddr_t));
            memcpy(&if_info->ethernet_hdr->eh_src, &if_info->base_hdr->tbh_realdst, sizeof(etheraddr_t));
            memcpy(&if_info->base_hdr->tbh_realdst, &if_info->base_hdr->tbh_realsrc, sizeof(etheraddr_t));
            memcpy(&if_info->base_hdr->tbh_realsrc, &if_info->ethernet_hdr->eh_src, sizeof(etheraddr_t));

            /* Add the rcv timestamp from the global save area */
            cpy_hton64(&qos->qprb_hdr->probe_rxstamp, &qos->pktio_timestamp);

            /* Add the rtx timestamp just before sending */
            get_timestamp(&qos->pktio_timestamp);
            cpy_hton64(&qos->qprb_hdr->probe_rtxstamp, &qos->pktio_timestamp);

            /* and return the packet - do not save in re_txbuf! */
            tx_write(if_info->rxbuf, if_info->rcvd_pkt_len);

            IF_TRACED(TRC_PACKET)
                dbgprintf("qos_probegap: reflecting, no 802.1p, seq=%d -> " ETHERADDR_FMT "\n",
                          if_info->sequencenum, ETHERADDR_PRINT(&pThisSsn->qssn_ctrlr_real));
            END_TRACE
        } else {
            /* there is a valid 802.1p field, so the reflected packet must be tagged. */
            qos_ether_header_t     *ethr_hdr;   /* pointer to qos ethernet-header in txbuf */
            qos_base_header_t      *base_hdr;   /* pointer to qos base-header in txbuf */
            qos_probe_header_t     *qprb_hdr;   /* pointer to qos probe-header in txbuf */
            
            ethr_hdr = (qos_ether_header_t*) if_info->txbuf;
            base_hdr = (qos_base_header_t*) (ethr_hdr+1);
            qprb_hdr = (qos_probe_header_t*)(base_hdr+1);

            /* setup ethernet and base-hdr source and dest addresses to return to sender */
            memcpy(&ethr_hdr->qeh_dst, &if_info->base_hdr->tbh_realsrc, sizeof(etheraddr_t));
            memcpy(&ethr_hdr->qeh_src, &if_info->base_hdr->tbh_realdst, sizeof(etheraddr_t));
            memcpy(&base_hdr->qbh_realdst, &if_info->base_hdr->tbh_realsrc, sizeof(etheraddr_t));
            memcpy(&base_hdr->qbh_realsrc, &if_info->base_hdr->tbh_realdst, sizeof(etheraddr_t));
            
            /* Set up the 802.1q tag, then insert the .1p value in the highest 7 bits (vlan==0) */
            ethr_hdr->qeh_qtag = 0x0081;
            ethr_hdr->qeh_ptag = htons((qos->qprb_hdr->probe_pqval << 9));
            ethr_hdr->qeh_ethertype = if_info->ethernet_hdr->eh_ethertype;
            
            /* fill out rest of base header */
            base_hdr->qbh_version = if_info->base_hdr->tbh_version;
            base_hdr->qbh_tos     = if_info->base_hdr->tbh_tos;
            base_hdr->qbh_resrvd  = if_info->base_hdr->tbh_resrvd;
            base_hdr->qbh_opcode  = if_info->base_hdr->tbh_opcode;
            base_hdr->qbh_seqnum  = if_info->base_hdr->tbh_seqnum;

            /* Fill out the probe-hdr */
            qprb_hdr->probe_txstamp = qos->qprb_hdr->probe_txstamp;
            qprb_hdr->probe_testtype = 2;
            qprb_hdr->probe_pktID = qos->qprb_hdr->probe_pktID;
            qprb_hdr->probe_pqval = qos->qprb_hdr->probe_pqval;

            /* Add the rcv timestamp from the global save area */
            cpy_hton64(&qprb_hdr->probe_rxstamp, &qos->pktio_timestamp);

            /* Copy the payload */
            memcpy(&qprb_hdr->probe_payload, &qos->qprb_hdr->probe_payload,
                   if_info->rcvd_pkt_len - (((uint8_t*)&qos->qprb_hdr->probe_payload) - if_info->txbuf));

            /* Add the rtx timestamp just before sending */
            get_timestamp(&qos->pktio_timestamp);
            cpy_hton64(&qprb_hdr->probe_rtxstamp, &qos->pktio_timestamp);

            /* and return the packet (4 bytes longer due to tags) - do not save in re_txbuf! */
            tx_write(if_info->txbuf, if_info->rcvd_pkt_len+4);

            IF_TRACED(TRC_PACKET)
                dbgprintf("qos_probegap: reflecting, with 802.1p priority of: %d, seq=%d -> " ETHERADDR_FMT "\n", (qos->qprb_hdr->probe_pqval & 0x7f), if_info->sequencenum, ETHERADDR_PRINT(&pThisSsn->qssn_ctrlr_real));
            END_TRACE
        }
    } else if (qos->qprb_hdr->probe_testtype == 0) { /* timed probe */
        qosEventDescr_t* evt;
        qosEventBucket_t* bucket = qos_find_bucket(pThisSsn);

        do
        {
            if (bucket)
            {
                // Make sure we don't try to store more than fits in the bucket
                if (bucket->evt_numEvts >= MAX_QOS_EVENTS_PER_BUCKET)
                {
                    break;
                }
            }
            else
            {
                /* There is no existing bucket to dump the event into, so find a new one */
                if (pThisSsn->qssn_num_active_buckets >= MAX_QOS_BUCKETS)
                {
                    /* Reuse the oldest bucket */
                    bucket = &pThisSsn->qssn_evt_buckets[pThisSsn->qssn_first_bucket];
                    pThisSsn->qssn_first_bucket = (pThisSsn->qssn_first_bucket + 1) & (MAX_QOS_BUCKETS - 1);
                }
                else
                {
                    /* Use the next available bucket */
                    bucket = &pThisSsn->qssn_evt_buckets[(pThisSsn->qssn_first_bucket + pThisSsn->qssn_num_active_buckets) & (MAX_QOS_BUCKETS - 1)];
                    pThisSsn->qssn_num_active_buckets++;
                }

                bucket->evt_seqNum = if_info->sequencenum;
                bucket->evt_numEvts = 0;
            }

            evt = &bucket->evt_descs[bucket->evt_numEvts++];

            /* Copy timestamps, packet-ID, and set reserved-byte... */
            memcpy(&evt->ctrlr_txstamp, &qos->qprb_hdr->probe_txstamp, sizeof(uint64_t));

            /* Pick up the rx-timestamp from the driver, put in global timestamp */
            cpy_hton64(&evt->sink_rxstamp, &qos->pktio_timestamp);

            evt->evt_pktID = qos->qprb_hdr->probe_pktID;
            evt->evt_reserved = 0;
        } while (FALSE);

        IF_TRACED(TRC_PACKET)
            dbgprintf("qos_timedprobe processed: seq=" FMT_UINT16 ", evtcount=" FMT_UINT32 "\n", if_info->sequencenum, bucket->evt_numEvts);
        END_TRACE
    }
}


static
void
qos_query(void)
{
    qosEventBucket_t* bucket;
    qos_session_t*	pThisSsn = qos_find_session();
    size_t          nbytes;
    uint16_t        numEvts;

    int rif = RIF;
    ASSERT(g_qos != NULL && rif != -1);
    qos_info_t *qos = g_qos + rif;
    osl_interface_info_t *if_info = IF_INFO(RIF);

    /* Find associated session, and reject with silence if none found. */
    if (pThisSsn == NULL)   return;

    /* Valid session - mark it as "still active" */
    pThisSsn->qssn_ticks_til_discard = 4;

    /* Compose the response headers in the space left before the events descrs */
    /* Build the ethernet header and base header */
    fmt_base(if_info->txbuf, &if_info->hw_addr, &pThisSsn->qssn_ctrlr_real, ToS_QoSDiagnostics,
             Qopcode_QueryResp, if_info->sequencenum, FALSE /*no broadcast*/);
    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

    /* Locate the event bucket */
    bucket = qos_find_bucket(pThisSsn);

    /* Add the number of events */
    numEvts = bucket ? bucket->evt_numEvts : 0;
    qos->short_reorder_buffer = htons(numEvts);
    memcpy(&if_info->txbuf[nbytes], &qos->short_reorder_buffer, sizeof(uint16_t));
    nbytes += sizeof(uint16_t);

    /* Copy the events */
    if (numEvts)
    {
        memcpy(&if_info->txbuf[nbytes], bucket->evt_descs, sizeof(qosEventDescr_t) * numEvts);
        nbytes += sizeof(qosEventDescr_t) * numEvts;
    }

    /* And send it... */
    tx_write(if_info->txbuf, nbytes);

    IF_TRACED(TRC_PACKET)
        dbgprintf("qos_query_response: sending " FMT_UINT32 " events, seq=" FMT_UINT16 " -> " ETHERADDR_FMT "\n",
                  numEvts, if_info->sequencenum, ETHERADDR_PRINT(&pThisSsn->qssn_ctrlr_real));
    END_TRACE
}


static
void
qos_counterlease(void)
{
    struct timeval	now;

    int rif = RIF;
    ASSERT(g_qos != NULL && rif != -1);
    qos_info_t *qos = g_qos + rif;

    IF_TRACED(TRC_QOS)
        dbgprintf("qos_counter_lease: timeout was: " FMT_UINT32 ", now: 300",qos->samples_remaining);
    END_TRACE

    if (qos->samples_remaining == 0)
    {
        /* start collection: repeats every second - until the lease runs out */
        gettimeofday(&now, NULL);
        now.tv_sec += 1;
        qos->CTA_timer = event_add(rif, &now, interface_counter_recorder, NULL);
        IF_TRACED(TRC_QOS)
             dbgprintf("qos_counter_lease: 1-sec timer started\n");
        END_TRACE
    }
    qos->samples_remaining = 300;
}


static
void
qos_snapshot(void)
{
    size_t		nbytes;
    uint8_t            *pMsg, sample_cnt;
    uint64_t            perf_timestamp;
    uint                count, next;
    qos_perf_sample    *pSample, *thisSample;

    int rif = RIF;
    ASSERT(g_qos != NULL && rif != -1);
    qos_info_t *qos = g_qos + rif;
    osl_interface_info_t *if_info = IF_INFO(RIF);

    /* Build the ethernet header and base header */
    pMsg = if_info->txbuf;
    fmt_base(pMsg, &if_info->hw_addr, &if_info->base_hdr->tbh_realsrc, ToS_QoSDiagnostics,
             Qopcode_CounterResult, if_info->sequencenum, FALSE /*no broadcast*/);
    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

    /* Calculate the sub-second interval and write the CounterResult header */
    stamp_time(&perf_timestamp);
    pMsg[nbytes++] = (uint8_t)(((perf_timestamp - qos->perf_timestamp)*256)/1000000);      // sub-second span
    pMsg[nbytes++] = (uint8_t)BYTE_SCALE;
    pMsg[nbytes++] = (uint8_t)PKT_SCALE;
    sample_cnt = (qos->snap_hdr->cnt_rqstd <= (uint8_t)qos->sample_count) ? qos->snap_hdr->cnt_rqstd : (uint8_t)qos->sample_count;
    pMsg[nbytes++] = sample_cnt;

    IF_TRACED(TRC_QOS)
        dbgprintf("qos_snapshot: subsec: %d; sample-cnt: " FMT_UINT32 "\n",(int)pMsg[32], qos->sample_count);
    END_TRACE

    /* Now copy the samples to the QosCounterResult msg */
    next = (qos->next_sample + (60 - sample_cnt)) % 60;
    pSample = (qos_perf_sample*)&pMsg[nbytes];
    for (count=sample_cnt; count>0; count--)
    {
        thisSample = &qos->perf_samples[next++];
        next %= 60;
        pSample->bytes_rcvd = htons(thisSample->bytes_rcvd);
        pSample->pkts_rcvd  = htons(thisSample->pkts_rcvd);
        pSample->bytes_sent = htons(thisSample->bytes_sent);
        pSample->pkts_sent  = htons(thisSample->pkts_sent);
        IF_TRACED(TRC_QOS)
            dbgprintf("  sample: rcvd: %d; r-pkts: %d;  sent: %d; s-pkts: %d\n",
                      thisSample->bytes_rcvd,thisSample->pkts_rcvd,thisSample->bytes_sent,thisSample->pkts_sent);
        END_TRACE
        pSample++;
        nbytes += sizeof(qos_perf_sample);
    }
    /* Now take a sub-second sample, and put it in the QosCounterResult msg */
    {
        uint32_t       rbytes, rpkts, tbytes, tpkts;
        uint32_t       delta1, delta2, delta3, delta4 ;

        /* save the counters, so normal sampling won't be disrupted */
        rbytes = qos->rbytes; rpkts = qos->rpkts; tbytes = qos->tbytes; tpkts = qos->tpkts;

        get_raw_samples();      // get current values for rbytes, rpkts, tbytes, tpkts

        delta1 = (qos->rbytes-rbytes > 0)? (qos->rbytes-rbytes) : 0;
        pSample->bytes_rcvd = htons(delta1 / BYTE_SCALE_FACTOR);

        delta2 = (qos->rpkts-rpkts > 0)?   (qos->rpkts-rpkts) : 0;
        pSample->pkts_rcvd  = htons(delta2 / PKT_SCALE_FACTOR);

        delta3 = (qos->tbytes-tbytes > 0)? (qos->tbytes-tbytes) : 0;
        pSample->bytes_sent = htons(delta3 / BYTE_SCALE_FACTOR);

        delta4 = (qos->tpkts-tpkts > 0)?   (qos->tpkts-tpkts) : 0;
        pSample->pkts_sent  = htons(delta4 / PKT_SCALE_FACTOR);

        IF_TRACED(TRC_QOS)
            dbgprintf("  sub-sec sample: rcvd: " FMT_UINT32 "; r-pkts: " FMT_UINT32 \
                      ";  sent: " FMT_UINT32 "; s-pkts: " FMT_UINT32 "\n",
                      delta1, delta2, delta3, delta4);
        END_TRACE

        /* restore the saved counters */
        qos->rbytes = rbytes; qos->rpkts = rpkts; qos->tbytes = tbytes; qos->tpkts = tpkts;

        /* count the subsecond sample in the packet length */
        nbytes += sizeof(qos_perf_sample);
    }

    /* And send it... */
    tx_write( if_info->txbuf, nbytes );

    IF_TRACED(TRC_PACKET)
        dbgprintf("qos_counter_result: sending " FMT_UINT32 " perf-samples + sub-sec-sample, seq=" FMT_UINT16 " -> " ETHERADDR_FMT "\n", qos->sample_count, if_info->sequencenum, ETHERADDR_PRINT(&if_info->base_hdr->tbh_realsrc));
    END_TRACE
}


/************************** Q O S   M E S S A G E   R E C E I V E R **************************/

/* Called by packetio_recv_handler() when msg ToS indicates QOS.
 * The ether and base headers are validated, and ether- and base-header ptrs are set up. */
extern void qosrcvpkt(void);

void
qosrcvpkt(void)
{
    uint16_t thisSeqnum;

    int rif = RIF;
    ASSERT(g_qos != NULL && rif != -1);
    qos_info_t *qos = g_qos + rif;
    osl_interface_info_t *if_info = IF_INFO(RIF);

    /* check opcode for QOS case */
    if (if_info->opcode < 0 || if_info->opcode >= Qopcode_INVALID)
    {
    	warn("qospktrcv: opcode=%d is out of range for QoS msg; ignoring\n", if_info->opcode);
    	return;
    }
    IF_TRACED(TRC_PACKET)
        dbgprintf("QOS: opcode=%d\n",if_info->opcode);
    END_TRACE

    thisSeqnum = ntohs(if_info->base_hdr->tbh_seqnum);

    /* QosCounterLease frame must not be sequenced, everything else must be sequenced */
    /* QosCounterLease is the only one that's broadcasted */
    if (if_info->opcode != Qopcode_CounterLease)
    {
        if (thisSeqnum == 0)
        {
        	warn("qospktrcv: opcode=%d must be sequenced; ignoring\n", if_info->opcode);
            return;
        }
        else if (!ETHERADDR_EQUALS(&if_info->base_hdr->tbh_realdst, &if_info->hw_addr))
        {
        	warn("qospktrcv: opcode=%d must be directed; ignoring\n", if_info->opcode);
            return;
        }
    }
    else if (thisSeqnum != 0)
    {
    	warn("qospktrcv: QosCounterLease must not be sequenced; ignoring\n");
        return;
    }
    else if (!ETHERADDR_IS_BCAST(&if_info->base_hdr->tbh_realdst))
    {
    	warn("qospktrcv: QosCounterLease must be broadcasted; ignoring\n");
        return;
    }

    /* Validate source/dest real/ether addresses */
    if (!ETHERADDR_EQUALS(&if_info->ethernet_hdr->eh_src, &if_info->base_hdr->tbh_realsrc) ||
        !ETHERADDR_EQUALS(&if_info->ethernet_hdr->eh_dst, &if_info->base_hdr->tbh_realdst))
    {
        return;
    }

    /* print the frame */
    IF_TRACED(TRC_PACKET)
	dbgprintf(" [" ETHERADDR_FMT "] -> [" ETHERADDR_FMT "] %s (seq=%d)\n",
		ETHERADDR_PRINT(&if_info->ethernet_hdr->eh_src),
        ETHERADDR_PRINT(&if_info->ethernet_hdr->eh_dst),
		Qos_opcode_names[if_info->opcode], thisSeqnum);
    END_TRACE

    /* By this time, we are pretty sure the sequence number is valid, so save a global copy... */
    if_info->sequencenum = thisSeqnum;

    /* Set up global pointers to the 2 possible types of received headers that are bigger than the base hdr */
    qos->qprb_hdr  = (qos_probe_header_t*)   (if_info->base_hdr + 1);
    qos->qinit_hdr = (qos_initsink_header_t*)(if_info->base_hdr + 1);
    qos->snap_hdr  = (qos_snapshot_header_t*)(if_info->base_hdr + 1);

    /* Finally, perform per-opcode validation & processing */
    switch (if_info->opcode)
    {
      case Qopcode_Probe:
        qos_probe();
        break;

      case Qopcode_Query:
        qos_query();
        break;

      case Qopcode_CounterSnapshot:
        qos_snapshot();
        break;

      case Qopcode_CounterLease:
        qos_counterlease();
        break;

      case Qopcode_InitializeSink:
        qos_initsink();
        break;

      case Qopcode_Reset:
        qos_reset();
        break;

      /* (invalid- or Sink-sent-packets are ignored completely) */
      case Qopcode_ACK:		 // Sent by Sink
      case Qopcode_QueryResp:	 // Sent by Sink
      case Qopcode_Ready:	 // Sent by Sink
      case Qopcode_Error:	 // Sent by Sink
      case Qopcode_CounterResult:// Sent by Sink
      case Opcode_INVALID:
      default:
        break;
    }
}
