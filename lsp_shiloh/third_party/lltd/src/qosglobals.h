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

#ifndef QOS_GLOBALS_H
#define QOS_GLOBALS_H

#include "qosprotocol.h"

#define MAX_QOS_EVENTS_PER_BUCKET 32    /* protocol spec recommends 82 as max, 24 as minimum; WARNING: must be power of 2 */
typedef struct {
    uint16_t evt_seqNum;                /* sequence number associated with this bucket */
    uint16_t evt_numEvts;               /* # of valid qosEventDescr_t in evt_descs */
    qosEventDescr_t evt_descs[MAX_QOS_EVENTS_PER_BUCKET];
} qosEventBucket_t;

#define MAX_QOS_BUCKETS 8               /* WARNING: must be power of 2 */
typedef struct {
    uint32_t    qssn_ticks_til_discard; /* how many ticks of tgc-timer remain before this ssn is discarded */
    uint8_t     qssn_first_bucket;
    uint8_t     qssn_num_active_buckets;
    qosEventBucket_t qssn_evt_buckets[MAX_QOS_BUCKETS];
    etheraddr_t qssn_ctrlr_real;        /* Controller MAC (InitSink BH:RealSrc) */
    bool_t      qssn_is_valid;          /* empty entries in the session table are "invalid" */
} qos_session_t;

#define MAX_QOS_SESSIONS 10
typedef struct {
    qos_session_t	        QosSessions[MAX_QOS_SESSIONS];
    qos_probe_header_t      *qprb_hdr;
    qos_initsink_header_t   *qinit_hdr;
    qos_snapshot_header_t   *snap_hdr;
    uint32_t                LinkSpeed;
    uint64_t                TimeStampFreq;
    uint64_t                pktio_timestamp;
    uint16_t                short_reorder_buffer;	/* for htons() */
    uint32_t                reorder_buffer;	        /* for htonl() */
    uint64_t                long_reorder_buffer;	/* for htonll() */
    uint64_t                perf_timestamp;
    qos_perf_sample         perf_samples[60];
    uint32_t                next_sample;            /* oldest data */
    uint32_t                sample_count;           /* 0 - 60 */
    /* set to 300 each time lease is received; counts down to 0 */
    uint32_t                samples_remaining;      
    uint32_t                rbytes;
    uint32_t                rpkts;
    uint32_t                tbytes;
    uint32_t                tpkts;
    /* repeats every 15 seconds - a session is killed when it has had no
       activity for 3 ticks */
    event_t		            *inactivity_timer;
    /* repeats every second - to collect interface perf-data */
    event_t		            *CTA_timer;
} qos_info_t;

GLOBAL qos_info_t *g_qos;

#endif	/* QOS_GLOBALS_H */
