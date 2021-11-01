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
#include <assert.h>

#include <string.h>
#include <errno.h>

#include "globals.h"

#include "statemachines.h"
#include "packetio.h"

/********************* sole linkage to QOS handlers **************************/

extern void qosrcvpkt( void );

/*****************************************************************************/

void
packetio_invalidate_retxbuf(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    ifInfo->re_tx_opcode = Opcode_INVALID;
    ifInfo->rtxseqnum = ifInfo->re_tx_len = 0;
}

/************************** P A C K E T   V A L I D A T O R S   **************************/

/* These are packet-validation handlers for each opcode that can be further validated.
 * Global header pointers have already been set up, and global length saved.
 *
 * These routines validate packet lengths, etc, and set the event type. They also
 * provide a point for tracing interesting packets.
 */

enum packet_verification_results {
    VALID_PACKET,
    INVALID_PACKET
};

static uint
validate_discover(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    if (ifInfo->rcvd_pkt_len < sizeof(topo_ether_header_t) + sizeof(topo_base_header_t) +  sizeof(topo_discover_header_t))
    {
        warn("verify_discover: frame with truncated Discover header (len=" FMT_SIZET " src="
              ETHERADDR_FMT " dst=" ETHERADDR_FMT "); ignoring\n",
              ifInfo->rcvd_pkt_len, ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_src), ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_dst));
        ifInfo->this_event.evtType =  evtInvalidPacket;
        return INVALID_PACKET;
    }
    ifInfo->this_event.evtType =  evtDiscoverRcvd;
    return VALID_PACKET;
}

static uint
validate_hello()
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    if (ifInfo->rcvd_pkt_len < sizeof(topo_ether_header_t) + sizeof(topo_base_header_t) + sizeof(topo_hello_header_t) + 1)
    {
	warn("rx_hello: frame with truncated Hello header (len=" FMT_SIZET " src="
	     ETHERADDR_FMT " dst=" ETHERADDR_FMT "); ignoring\n",
	     ifInfo->rcvd_pkt_len, ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_src), ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_dst));
        ifInfo->this_event.evtType =  evtInvalidPacket;
	return INVALID_PACKET;
    }

    IF_TRACED(TRC_PACKET)
	uint16_t gen = ntohs(ifInfo->hello_hdr->hh_gen);
	dbgprintf("hello-rcvd: gen=%d mapper=" ETHERADDR_FMT "\n",
		gen, ETHERADDR_PRINT(&ifInfo->hello_hdr->hh_curmapraddr));
    END_TRACE

    ifInfo->this_event.evtType =  evtPacketRcvd;
    return VALID_PACKET;
}


static uint
validate_queryltlv()
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    if (ifInfo->rcvd_pkt_len < sizeof(topo_ether_header_t) + sizeof(topo_base_header_t) + sizeof(topo_qltlv_header_t))
    {
	warn("query-ltlv-rcvd: frame with truncated qltlv header (len=" FMT_SIZET " src="
	     ETHERADDR_FMT " dst=" ETHERADDR_FMT "); ignoring\n",
	     ifInfo->rcvd_pkt_len, ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_src), ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_dst));
        ifInfo->this_event.evtType =  evtInvalidPacket;
	return INVALID_PACKET;
    }

    IF_TRACED(TRC_PACKET)
	dbgprintf("query-ltlv-rcvd: tlv# %d  reserved: %d  offset: %d\n",
	          ifInfo->qltlv_hdr->qh_type, ifInfo->qltlv_hdr->qh_rsvd1, ifInfo->qltlv_hdr->qh_offset);
    END_TRACE

    ifInfo->this_event.evtType =  evtPacketRcvd;
    return VALID_PACKET;
}


static char *
ed_type2name(uint8_t type)
{
    static char buf[12]; /* caution: not thread-safe! */
    switch (type)
    {
    case 0: return "Train";
    case 1: return "Probe";
    case 2: return "TimedProbe";
    default:
	snprintf(buf, sizeof(buf), "%d", type);
	return buf;
    }
}


static uint
validate_emit()
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    topo_emit_header_t      *emit;	/* pointer to emit header in rxbuf */
    topo_emitee_desc_t *ed, *first_desc, *limit;
    uint16_t numdescs;
    int      i;

    /* Parse the Emit */
    ifInfo->totalPause = 0;
    ifInfo->neededPackets = 0;
    ifInfo->neededBytes = 0;

    if (ifInfo->rcvd_pkt_len < sizeof(topo_ether_header_t) + sizeof(topo_base_header_t) + 
	sizeof(topo_emit_header_t))
    {
	warn("validate_emit: frame with truncated Emit header (len=" FMT_SIZET " src="
	     ETHERADDR_FMT " dst=" ETHERADDR_FMT "); ignoring\n",
	     ifInfo->rcvd_pkt_len, ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_src), ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_dst));
        ifInfo->this_event.evtType =  evtInvalidPacket;
	return INVALID_PACKET;
    }

    emit = (topo_emit_header_t*)(ifInfo->base_hdr + 1);	// base the emit header in rxbuf

    numdescs = ntohs(emit->eh_numdescs);
    limit = (topo_emitee_desc_t*)(((uint8_t*)ifInfo->ethernet_hdr) + ifInfo->rcvd_pkt_len);

    /* check the emitee_descs are asking for reasonable transmissions */
    first_desc = (topo_emitee_desc_t*)(emit + 1);

    IF_TRACED(TRC_PACKET)
	dbgprintf("numdescs=%d EmiteeDescs=[", numdescs);
    END_TRACE

    for (i=0, ed=first_desc; ed+1 <= limit && i < numdescs; ed++, i++)
    {
	IF_TRACED(TRC_PACKET)
	    dbgprintf("{%s: %dms " ETHERADDR_FMT " -> " ETHERADDR_FMT "} ",
		    ed_type2name(ed->ed_type), ed->ed_pause,
		    ETHERADDR_PRINT(&ed->ed_src), ETHERADDR_PRINT(&ed->ed_dst));
        END_TRACE

	ifInfo->totalPause += ed->ed_pause;

	if (ed->ed_type != 0x00 && ed->ed_type != 0x01)
	{
	    warn("validate_emit: emitee_desc from " ETHERADDR_FMT " with unknown type=%d; "
		 "ignoring whole Emit request\n",
		 ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_src), ed->ed_type);
            ifInfo->this_event.evtType =  evtInvalidPacket;
	    return INVALID_PACKET;
	}
	
	if (!ETHERADDR_EQUALS(&ed->ed_src, &ifInfo->hw_addr) &&
	    !TOPO_ETHERADDR_OUI(&ed->ed_src))
	{
	    warn("validate_emit: emitee_desc with src=" ETHERADDR_FMT " is invalid; "
		 "ignoring whole Emit request\n",
		 ETHERADDR_PRINT(&ed->ed_src));
            ifInfo->this_event.evtType =  evtInvalidPacket;
	    return INVALID_PACKET;
	}
	
	if (ETHERADDR_IS_MCAST(&ed->ed_dst))
	{
	    warn("validate_emit: emitee_desc with dst=" ETHERADDR_FMT " is invalid; "
		 "ignoring whole Emit request\n",
		 ETHERADDR_PRINT(&ed->ed_dst));
            ifInfo->this_event.evtType =  evtInvalidPacket;
	    return INVALID_PACKET;
	}

	ifInfo->neededPackets++;
	ifInfo->neededBytes += sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);
    }

    IF_TRACED(TRC_PACKET)
	dbgprintf("]");
	if (i != numdescs)
	    dbgprintf(" (numdescs too big!)");
	dbgprintf("\n");
    END_TRACE

    if (ifInfo->totalPause > TOPO_TOTALPAUSE_MAX)
    {
	warn("validate_emit: Emit contains emitee_descs with total pausetime " FMT_UINT32 "ms > %ums; "
	     "ignoring whole Emit request\n",
	     ifInfo->totalPause, TOPO_TOTALPAUSE_MAX);
        ifInfo->this_event.evtType =  evtInvalidPacket;
	return INVALID_PACKET;
    }

    if (i != numdescs)
    {
	warn("validate_emit: numdescs=%d but only %d descs in frame\n", numdescs, i);
	numdescs = i;
    }

    /* will we need to send an ACK or Flat too? */
    if (ifInfo->sequencenum != 0)
    {
	ifInfo->neededPackets++;
	ifInfo->neededBytes += sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);
    }

    /* emitee_descs look ok; pass them up to state machine */

    ifInfo->this_event.numDescrs = numdescs;
    ifInfo->this_event.evtType =  evtEmitRcvd;
    return VALID_PACKET;
}


/************************** T X - F R A M E   F O R M A T T I N G **************************/

/* Format an Ethernet and Base header into "buf", returning a pointer to the next
 * header location.  Takes "srchw" and "dsthw" for the Base header's realsrc/dst, and 
 * uses them for the Ethernet header too unless "use_broadcast" is TRUE, in which case
 * the Ethernet dst is set to the broadcast address.
 * The "opcode" is which opcode to use
 * The "seqnum" is a host-endian sequence number (or 0).
 * Does no checking that "buf" contains sufficient space - use carefully! */
void *
fmt_base(uint8_t *buf, const etheraddr_t *srchw, const etheraddr_t *dsthw, lld2_tos_t tos,
	 topo_opcode_t opcode, uint16_t seqnum, bool_t use_broadcast)
{
    topo_ether_header_t *ehdr = (topo_ether_header_t*) buf;
    topo_base_header_t  *bhdr = (topo_base_header_t*)(ehdr + 1);

    IF_TRACED(TRC_PACKET)
        DBG_VERBOSE("fmt_base: src=" ETHERADDR_FMT "\n", ETHERADDR_PRINT(srchw));
        DBG_VERBOSE("fmt_base: dst=" ETHERADDR_FMT "\n", ETHERADDR_PRINT(dsthw));
        DBG_VERBOSE("fmt_base: tos=%d opcode=%d seq=%d bcast=%s\n", \
               tos, opcode, seqnum, use_broadcast?"true":"false");
    END_TRACE
   
    ehdr->eh_src = *srchw;
    ehdr->eh_dst = use_broadcast? Etheraddr_broadcast : *dsthw;
    ehdr->eh_ethertype = TOPO_ETHERTYPE;
    
    bhdr->tbh_version = TOPO_VERSION;
    bhdr->tbh_tos = (uint8_t)tos;
    bhdr->tbh_opcode = (uint8_t)opcode;
    bhdr->tbh_realsrc = *srchw;
    bhdr->tbh_realdst = *dsthw;
    bhdr->tbh_seqnum = htons(seqnum);

    return (void*)(bhdr+1);
}

/* low-level packet transmit function, used below */
void
tx_write(uint8_t *buf, size_t nbytes)
{
    ssize_t ret;

    ret = osl_write(g_osl, buf, nbytes);

    if (ret < 0)
	die("packetio_tx_write: write to socket failed: %s\n", strerror(errno));
    if (ret != (ssize_t)nbytes)
	die("packetio_tx_write: socket write for " FMT_SIZET " bytes, but %d went out\n",
	    nbytes, ret);
//#define BYTE_DUMP
#ifdef BYTE_DUMP
    {
	int i,j=(nbytes>48)?48:nbytes;
	DBG_VERBOSE("sending " FMT_SIZET " bytes:\n",nbytes);
	for (i=0; i<j; i++)
	{
	    DBG_VERBOSE("%02x%s%s", buf[i], (i%2)==1?" ": "", (i%16)==15? "\n": "");
	}
	DBG_VERBOSE("\n");
    }
#endif
}

/* usual way of sending a packet, potentially keeping a copy in the
 * rtx buffer and consuming a sequence number. */
static void
tx_sendpacket(size_t nbytes, uint16_t thisSeqNum)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    tx_write(ifInfo->txbuf, nbytes);
    ifInfo->tx_len = nbytes;

    /* if this is a reliable packet, then copy the txbuf aside to the
     * re_txbuf, and consume the sequence number. */
    if (thisSeqNum)
    {
	memcpy(ifInfo->re_txbuf, ifInfo->txbuf, nbytes);
	ifInfo->rtxseqnum = thisSeqNum;
	ifInfo->re_tx_len = nbytes;
        ifInfo->re_tx_opcode = ifInfo->opcode;	// save the opcode that generated this response, for retry-checking
    }
}

/* sent the retransmit buffer */
static void
tx_rtxpacket(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    assert(ifInfo->rtxseqnum != 0);
    IF_TRACED(TRC_PACKET)
	dbgprintf("packetio_recv_handler: retranmitting response with seqnum=%d\n", ifInfo->rtxseqnum);
    END_TRACE

    tx_write(ifInfo->re_txbuf, ifInfo->re_tx_len);    
}


/************************** M E S S A G E   S E N D E R S **************************/


void
packetio_tx_hello(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    topo_hello_header_t *tx_hh;
    size_t nbytes;

    tx_hh = fmt_base(ifInfo->txbuf, &ifInfo->hw_addr, &Etheraddr_broadcast, ToS_TopologyDiscovery, Opcode_Hello, 0, TRUE);
    tx_hh->hh_gen = htons(ifInfo->generation);
    if (ifInfo->topo_session != NULL && ifInfo->topo_session->ssn_is_valid)
    {
        tx_hh->hh_curmapraddr = ifInfo->topo_session->ssn_mapper_real;
        tx_hh->hh_aprmapraddr = ifInfo->topo_session->ssn_mapper_current;
    } else {
        memset(&tx_hh->hh_curmapraddr, 0, sizeof(etheraddr_t));
        memset(&tx_hh->hh_aprmapraddr, 0, sizeof(etheraddr_t));
    }
    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t) + sizeof(topo_hello_header_t);

    /* write the TLVs (reserving room for the EOP byte) */
    nbytes += tlv_write_info(&ifInfo->txbuf[nbytes], TOPO_MAX_FRAMESZ - nbytes - 1);

    /* now add the EOP */
    ifInfo->txbuf[nbytes] = 0;
    nbytes++;

    tx_sendpacket(nbytes, 0);

    IF_TRACED(TRC_PACKET)
	dbgprintf("tx_hello (%s): topo-ssn=%p gen=%d\n",
	          (ifInfo->topo_session && ifInfo->topo_session->ssn_is_valid)?"topo":"quick",
	          ifInfo->topo_session, ntohs(tx_hh->hh_gen));
    END_TRACE
}


void
packetio_tx_queryresp(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    topo_queryresp_header_t *qr;
    topo_recvee_desc_t *p;
    size_t nbytes, bytes_left;

    qr = fmt_base(ifInfo->txbuf, &ifInfo->hw_addr, &ifInfo->topo_session->ssn_mapper_real, ToS_TopologyDiscovery, Opcode_QueryResp,
		  ifInfo->sequencenum, FALSE /*ifInfo->topo_session->ssn_use_broadcast*/);

    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t) +
	sizeof(topo_queryresp_header_t);
    bytes_left = TOPO_MAX_FRAMESZ - nbytes;
    qr->qr_numdescs = 0;
    p = (topo_recvee_desc_t*)(qr + 1);

    while ((bytes_left >= sizeof(topo_recvee_desc_t)) &&
	   seeslist_dequeue(p))
    {
	p++;
	bytes_left -= sizeof(topo_recvee_desc_t);
	nbytes += sizeof(topo_recvee_desc_t);
	qr->qr_numdescs++;
    }

    /* any more to follow? */
    if (!seeslist_is_empty())
	qr->qr_numdescs |= 0x8000; /* set the M (more) bit */

    qr->qr_numdescs = htons(qr->qr_numdescs);

    tx_sendpacket(nbytes, ifInfo->sequencenum);

    IF_TRACED(TRC_PACKET)
	dbgprintf("tx_queryresp: -> " ETHERADDR_FMT "\n", ETHERADDR_PRINT(&ifInfo->topo_session->ssn_mapper_real));
    END_TRACE
}


void
packetio_tx_flat(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    topo_flat_header_t *fh;
    size_t nbytes;

    fh = fmt_base(ifInfo->txbuf, &ifInfo->hw_addr, &ifInfo->topo_session->ssn_mapper_real, ToS_TopologyDiscovery, Opcode_Flat,
		  ifInfo->sequencenum, FALSE /*ifInfo->topo_session->ssn_use_broadcast*/);

    fh->fh_ctc_bytes = htonl(ifInfo->ctc_bytes);
    fh->fh_ctc_packets = htons((uint16_t)ifInfo->ctc_packets);

                                          /*(doesn't work! padded to 8!)   sizeof(topo_flat_header_t)*/;
    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t) + sizeof(uint32_t) + sizeof(uint16_t);

    tx_sendpacket(nbytes, ifInfo->sequencenum);

    IF_TRACED(TRC_PACKET)
	dbgprintf("tx_flat(len:" FMT_SIZET "+" FMT_SIZET "+" FMT_SIZET "): " FMT_UINT32 " bytes, " FMT_UINT32 \
	          " packets -> " ETHERADDR_FMT "\n",
                  sizeof(topo_ether_header_t),sizeof(topo_base_header_t),sizeof(topo_flat_header_t),
		  ifInfo->ctc_bytes, ifInfo->ctc_packets,
		  ETHERADDR_PRINT(&ifInfo->topo_session->ssn_mapper_real));
    END_TRACE
}


void
packetio_tx_emitee(topo_emitee_desc_t *ed)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    topo_opcode_t        Opcode;
    size_t               nbytes;
    topo_ether_header_t *ehdr = (topo_ether_header_t*) ifInfo->txbuf;
    topo_base_header_t  *bhdr = (topo_base_header_t*)  (ehdr + 1);

    Opcode = (ed->ed_type == 0x00) ? Opcode_Train : Opcode_Probe;
    
    ehdr->eh_src = ed->ed_src;
    ehdr->eh_dst = ed->ed_dst;
    ehdr->eh_ethertype = TOPO_ETHERTYPE;
    
    bhdr->tbh_version = TOPO_VERSION;
    bhdr->tbh_tos = (uint8_t)ToS_TopologyDiscovery;
    bhdr->tbh_opcode = (uint8_t)Opcode;
    bhdr->tbh_realsrc = ifInfo->hw_addr;
    bhdr->tbh_realdst = ed->ed_dst;
    bhdr->tbh_seqnum = htons(0);

    IF_TRACED(TRC_PACKET)
        DBG_VERBOSE("tx_emitee: src=" ETHERADDR_FMT "\n", ETHERADDR_PRINT(&ehdr->eh_src));
        DBG_VERBOSE("tx_emitee: dst=" ETHERADDR_FMT "\n", ETHERADDR_PRINT(&ehdr->eh_dst));
        DBG_VERBOSE("tx_emitee: real-src=" ETHERADDR_FMT "\n", ETHERADDR_PRINT(&bhdr->tbh_realsrc));
        DBG_VERBOSE("tx_emitee: tos=%d opcode=%d seq=%d bcast=%s\n", bhdr->tbh_tos, Opcode, bhdr->tbh_seqnum, "false");
    END_TRACE

    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

    tx_sendpacket(nbytes, 0);

    IF_TRACED(TRC_PACKET)
	dbgprintf("tx_emitee: %s " ETHERADDR_FMT " -> " ETHERADDR_FMT "\n",
		Topo_opcode_names[ifInfo->opcode],
		ETHERADDR_PRINT(&ed->ed_src), ETHERADDR_PRINT(&ed->ed_dst));
    END_TRACE
}


void
packetio_tx_ack(uint16_t thisSeqNum)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    size_t nbytes;

    fmt_base(ifInfo->txbuf, &ifInfo->hw_addr, &ifInfo->topo_session->ssn_mapper_real, ToS_TopologyDiscovery,
             Opcode_ACK, thisSeqNum, FALSE /*ifInfo->topo_session->ssn_use_broadcast*/);
    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

    tx_sendpacket(nbytes, thisSeqNum);

    IF_TRACED(TRC_PACKET)
	dbgprintf("tx_ack: %d -> " ETHERADDR_FMT "\n",
		thisSeqNum, ETHERADDR_PRINT(&ifInfo->topo_session->ssn_mapper_real));
    END_TRACE
}


void
packetio_tx_qltlvResp(uint16_t thisSeqNum, tlv_desc_t *tlvDescr, size_t LtlvOffset)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    size_t      nbytes, sz_ltlv;

    if (ifInfo->topo_session == NULL)
    {
        warn("packetio_tx_qltlvResp: no mapping session when qLTLV was received. No response is being sent.\n");
        return;
    }
    fmt_base(ifInfo->txbuf, &ifInfo->hw_addr, &ifInfo->topo_session->ssn_mapper_real, ToS_TopologyDiscovery,
             Opcode_QueryLargeTlvResp, thisSeqNum, FALSE);
    nbytes = sizeof(topo_ether_header_t) + sizeof(topo_base_header_t);

    /* write the TLV, starting at the indicated offset */
    sz_ltlv = tlv_write_tlv( tlvDescr, &ifInfo->txbuf[nbytes], TOPO_MAX_FRAMESZ - nbytes, FALSE, LtlvOffset);
    if (sz_ltlv == 0)
    {   /* there was an error - the TLV could not be written for some reason  */
        ifInfo->txbuf[nbytes++] = 0; /* zero the two bytes of LTLV length and flag, */
        ifInfo->txbuf[nbytes++] = 0; /*  which will be the only things sent.        */
    }
    nbytes += sz_ltlv;

    tx_sendpacket(nbytes, thisSeqNum);

    IF_TRACED(TRC_PACKET)
        dbgprintf("tx_ack: %d -> " ETHERADDR_FMT "\n",
		thisSeqNum, ETHERADDR_PRINT(&ifInfo->topo_session->ssn_mapper_real));
    END_TRACE
}


/************************** M E S S A G E   R E C E I V E R **************************/

/* Upcalled by event.c when receive ready condition on the socket.
 * Validates the ether and base headers, and opcode types. */
void
packetio_recv_handler(int fd, void *state)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo);

    uint16_t                            thisSeqnum;
    enum packet_verification_results	pktValidity;


    /* Read into the global rxbuf, and save the length in a global */
    ifInfo->rcvd_pkt_len = osl_read(fd, ifInfo->rxbuf, RXBUFSZ);
    if (ifInfo->rcvd_pkt_len <= 0)
	die("packetio_recv_handler: error on read: %s\n", strerror(errno));

    if (ifInfo->rcvd_pkt_len < sizeof(topo_ether_header_t))
    {
	warn("packetio_recv_handler: runt frame (" FMT_SIZET " bytes < " FMT_SIZET "); ignoring\n",
	     ifInfo->rcvd_pkt_len, sizeof(topo_ether_header_t));
	return;
    }

    /* We set up all the (macro-ized) global header pointers here.
     * Actually, this could be done once, at process init time... */
    ifInfo->ethernet_hdr = (topo_ether_header_t*)(ifInfo->rxbuf);
    ifInfo->base_hdr = (topo_base_header_t*)(ifInfo->ethernet_hdr + 1);
    ifInfo->discover_hdr = (topo_discover_header_t*)(ifInfo->base_hdr + 1);
    ifInfo->hello_hdr = (topo_hello_header_t*)(ifInfo->base_hdr + 1);
    ifInfo->qltlv_hdr = (topo_qltlv_header_t*)(ifInfo->base_hdr + 1);

    /* check Ethernet header */
    if (ifInfo->ethernet_hdr->eh_ethertype != TOPO_ETHERTYPE)
    {
        DBG_VERBOSE("packetio: received a non-Topology packet ether-type=0x%0X, ignored...\n",ifInfo->ethernet_hdr->eh_ethertype);
	return;
    } else {
        DBG_VERBOSE("packetio: received a packet;  len=" FMT_SIZET "\n",ifInfo->rcvd_pkt_len);
    }

    /* It's a Topology protocol packet */
    /* check Base header */
    if (ifInfo->rcvd_pkt_len < sizeof(topo_ether_header_t) + sizeof(topo_base_header_t))
    {
	warn("packetio_recv_handler: frame with truncated Base header (len=" FMT_SIZET " src="
	     ETHERADDR_FMT " dst=" ETHERADDR_FMT "); ignoring\n",
	     ifInfo->rcvd_pkt_len, ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_src), ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_dst));
	return;
    }

    /* is this the right version for us? */
    if (ifInfo->base_hdr->tbh_version != TOPO_VERSION)
    {
	warn("packetio_recv_handler: got version %d protocol frame; ignoring\n",
	     ifInfo->base_hdr->tbh_version);
	return;
    }

    /* set global opcode */
    ifInfo->opcode = (topo_opcode_t) ifInfo->base_hdr->tbh_opcode;

    /* Qos Diagnostic packets are handled in qospktio.c */
    if (ifInfo->base_hdr->tbh_tos == ToS_QoSDiagnostics)
    {
        qosrcvpkt();
        return;
    }
    else if (ifInfo->base_hdr->tbh_tos > 1)
    {
        /* unsupported tos */
        return;
    }

    /* check global opcode for non-QOS case */
    if (ifInfo->opcode < 0 || ifInfo->opcode >= Opcode_INVALID)
    {
	warn("packetio_recv_handler: opcode=%d is out of range; ignoring\n", ifInfo->opcode);
	return;
    } else {
        IF_DEBUG
            dbgprintf("opcode=%d",ifInfo->opcode);
        END_DEBUG
    }

    thisSeqnum = ntohs(ifInfo->base_hdr->tbh_seqnum);

    /* Is this frame intended for us?
     * We want to receive frames with realdst == (our addr || bcast)
     * or with opcode == Probe (to record them)
     * or with opcode == Hello (to run the BAND load control) */
    if (ifInfo->opcode != Opcode_Probe &&
	ifInfo->opcode != Opcode_Hello &&
	!ETHERADDR_EQUALS(&ifInfo->base_hdr->tbh_realdst, &ifInfo->hw_addr) &&
	!ETHERADDR_IS_BCAST(&ifInfo->base_hdr->tbh_realdst))
    {
        IF_DEBUG
            dbgprintf("   [%s]... discarded\n", Topo_opcode_names[ifInfo->opcode]);
        END_DEBUG
	return;	
    }

    /* print the frame */
    IF_TRACED(TRC_PACKET)
	dbgprintf("[" ETHERADDR_FMT "] -> [" ETHERADDR_FMT "] %s (seq=%d)\n",
		ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_src), ETHERADDR_PRINT(&ifInfo->ethernet_hdr->eh_dst),
		Topo_opcode_names[ifInfo->opcode], thisSeqnum);
    END_TRACE

    /* check for illegal or malformed packets */

    /* 1) destination must not be broadcast or multicast, unless this is
     * a broadcast-valid opcode. */
    if (ETHERADDR_IS_MCAST(&ifInfo->base_hdr->tbh_realdst) && !BCAST_VALID(ifInfo->opcode))
    {
	warn("packetio_recv_handler: broadcast of opcode %d (from realsrc=" ETHERADDR_FMT 
	     ") is illegal; dropping\n",
	     ifInfo->opcode, ETHERADDR_PRINT(&ifInfo->base_hdr->tbh_realdst));
	return;
    }

    /* 2) only frames allowed sequence numbers should have them;
     * and some must have them. */
    if (thisSeqnum != 0 && !SEQNUM_VALID(ifInfo->opcode))
    {
	warn("packetio_recv_handler: opcode %d with seq=%u (from realsrc=" ETHERADDR_FMT
	     ") is illegal; dropping\n",
	     ifInfo->opcode, thisSeqnum, ETHERADDR_PRINT(&ifInfo->base_hdr->tbh_realsrc));
	return;
    }
    if (thisSeqnum == 0 && SEQNUM_REQUIRED(ifInfo->opcode))
    {
	warn("packetio_recv_handler: opcode %d (from realsrc=" ETHERADDR_FMT
	     "must have seqnum\n",
	     ifInfo->opcode, ETHERADDR_PRINT(&ifInfo->base_hdr->tbh_realsrc));
	return;
    }

    /* Does this frame have a sequence number?
     * If so, check it and maybe do retransmission. */

    /* Of course, sequence numbers are XIDs in Discover packets,
     * so it's not a retry-request.... save it as XID (in rx_discover), but otherwise ignore it */
    if (thisSeqnum != 0)
    {
        if (ifInfo->opcode != Opcode_Discover)
        {
           /* Does thisSeqnum match the one in our retransmission buffer? */
            if (thisSeqnum == ifInfo->rtxseqnum)
            {
                topo_ether_header_t    *ethernet_hdr = (topo_ether_header_t*)(ifInfo->re_txbuf);
                topo_base_header_t     *base_hdr = (topo_base_header_t*)(ethernet_hdr + 1);
                uint8_t                 RtxOpCode = base_hdr->tbh_opcode;

                if (ifInfo->re_tx_opcode == ifInfo->opcode)
                {
                    /* If it does, and the incoming opcode matches the one that generated the
                     * last sequenced response, re-send that response */
                    tx_rtxpacket();
                    return;
                } else {
                    /* Completely drop any packets that match seqnum, but not opcode */
                    warn("packetio_recv_handler: mis-matched opcode when matched sequence number: "
                         "got opcode %d, expecting %d; ignoring\n",
                         RtxOpCode, ifInfo->opcode);
                    return;
                }
            }

            /* do we expect any particular sequence number? */
            if (ifInfo->rtxseqnum != 0)
            {
                if (thisSeqnum != SEQNUM_NEXT(ifInfo->rtxseqnum))
                {
                    warn("packetio_recv_handler: mis-matched sequence number: "
                         "got %d, expecting %d; ignoring\n",
                         thisSeqnum, SEQNUM_NEXT(ifInfo->rtxseqnum));
                    return;
                }
	    }
	}

	/* update our sequence number */
	/* (actually, we leave this to the state machine since
	 * we want to ignore packets while in the Emitting state, without
	 * consuming their sequence numbers.)
	 */
    }

    /* By this time, we are pretty sure the sequence number is valid, so save a global copy... */
    ifInfo->sequencenum = thisSeqnum;

    /* Initialize a protocol-event to the default: evtPacketRcvd
     * later validation routines might refine the event type to a
     * particular packet type. */
    ifInfo->this_event.evtType       = evtPacketRcvd;
    ifInfo->this_event.ssn           = NULL;
    ifInfo->this_event.isNewSession  = FALSE;
    ifInfo->this_event.isAckingMe    = FALSE;
    ifInfo->this_event.isInternalEvt = FALSE;
    ifInfo->this_event.numDescrs     = 0;

    /* Otherwise, perform per-opcode validation */
    switch (ifInfo->opcode)
    {
      case Opcode_Discover:
        pktValidity = validate_discover();
        break;

      case Opcode_Hello:
        pktValidity = validate_hello();
        break;

      case Opcode_Emit:
        pktValidity = validate_emit();
        break;

      case Opcode_QueryLargeTlv:
        pktValidity = validate_queryltlv();
        break;

      case Opcode_Reset:
        pktValidity = VALID_PACKET;
        ifInfo->this_event.evtType = evtResetRcvd;
        break;

      case Opcode_Probe:
      case Opcode_ACK:
      case Opcode_Query:
      case Opcode_QueryResp:
      case Opcode_Charge:
      case Opcode_Flat:
      case Opcode_QueryLargeTlvResp:
        pktValidity = VALID_PACKET;
        break;

      case Opcode_Train:    /* Ignore TRAIN packets completely (they are only for switches) */
      case Opcode_INVALID:
      default:
        pktValidity = INVALID_PACKET;
        break;
    }
    /* and run the packet up to the protocol state machines for processing. */

    if (pktValidity == VALID_PACKET)
    {
        state_process_packet();
    }
    /* (invalid packets are ignored completely) */
}
