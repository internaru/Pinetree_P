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

/* This is the state machine that controls the topology mapping process, smT.
 * Once enumeration by a Topology-Discovery Mapper has been completed, that
 * mapper alone is enabled to make other requests of the Responder, until it
 * relinquishes its lock on the Responder, either by sending a Reset or by
 * timing out (Inactivity timeout).
 *
 * A session conceptually represents the association of a Mapper with this
 * Responder, throughout the lifetime of that association. State-information
 * for smT is maintained in one area of the process globals, since there is
 * at most one session associated with the Responder process at a time.
 *
 **/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "globals.h"

#include "statemachines.h"
#include "packetio.h"

static
int
SufficientChargeAvailable()
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    IF_DEBUG
        dbgprintf("SCA: ctc_pkts=" FMT_UINT32 "   pkts_needed=" FMT_UINT32 \
                  "    ctc_bytes=" FMT_UINT32 "   bytes_needed=" FMT_UINT32 "\n",
                  ifInfo->ctc_packets, ifInfo->neededPackets, ifInfo->ctc_bytes, ifInfo->neededBytes);
    END_DEBUG

    if ((ifInfo->ctc_packets >= ifInfo->neededPackets) && (ifInfo->ctc_bytes >= ifInfo->neededBytes))
        return TRUE;
    else
        return FALSE;
}

static void
ProcessEmits()
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    topo_ether_header_t    *ehdr;	/* pointer to ethernet header in emit buf */
    topo_base_header_t     *bhdr;	/* pointer to base header in emit buf */
    topo_emit_header_t     *emit;	/* pointer to emit header in emit buf */

    /* copy current rxbuf into the emit buffer */
    memcpy(ifInfo->emitbuf, ifInfo->rxbuf, ifInfo->rcvd_pkt_len);

    /* setup the emit machinery */
    assert(ifInfo->emit_remaining == 0);
    ifInfo->emit_remaining = ifInfo->this_event.numDescrs;
    ifInfo->emit_seqnum = ifInfo->sequencenum;		   /* might be zero for an unreliable Emit */
    ehdr = (topo_ether_header_t*)(ifInfo->emitbuf);
    bhdr = (topo_base_header_t*)(ehdr + 1);
    emit = (topo_emit_header_t*)(bhdr + 1);
    ifInfo->emitdesc = (topo_emitee_desc_t*)(emit + 1);  // pointer to the first descriptor
}

static
void
ChargeAdd(int pktlen)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    struct timeval now;

    ifInfo->ctc_packets++;
    if (ifInfo->ctc_packets > TOPO_CTC_PACKETS_MAX)
	ifInfo->ctc_packets = TOPO_CTC_PACKETS_MAX;

    ifInfo->ctc_bytes += pktlen;
    if (ifInfo->ctc_bytes > TOPO_CTC_BYTES_MAX)
	ifInfo->ctc_bytes = TOPO_CTC_BYTES_MAX;

    IF_TRACED(TRC_CHARGE)
	dbgprintf("ChargeAdd: CTC now has bytes=" FMT_UINT32 " & packets=" FMT_UINT32 "\n",
		ifInfo->ctc_bytes, ifInfo->ctc_packets);
    END_TRACE

    /* Reset charge timer */
    gettimeofday(&now, NULL);
    timeval_add_ms(&now, TOPO_CHARGE_TIMEOUT);
    CANCEL(ifInfo->charge_timer);
    ifInfo->charge_timer = event_add(rif, &now, state_charge_timeout, /*state:*/NULL);
}


static
void
ChargeConsume(int pktlen)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    struct timeval now;

    if (ifInfo->ctc_packets)
        ifInfo->ctc_packets--;

    if (ifInfo->ctc_bytes)
        ifInfo->ctc_bytes -= pktlen;

    IF_TRACED(TRC_CHARGE)
	dbgprintf("ChargeConsume: CTC now has bytes=" FMT_UINT32 " & packets=" FMT_UINT32 "\n",
		ifInfo->ctc_bytes, ifInfo->ctc_packets);
    END_TRACE

    /* Reset charge timer */

    gettimeofday(&now, NULL);
    timeval_add_ms(&now, TOPO_CHARGE_TIMEOUT);
    CANCEL(ifInfo->charge_timer);
    ifInfo->charge_timer = event_add(rif, &now, state_charge_timeout, /*state:*/NULL);
}


/***********************  Q U I E S C E N T   S T A T E  ***********************/

static
enum sm_Status
smT_QuiescentHandler( protocol_event_t* evt )
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    IF_TRACED(TRC_STATE)
        if (evt->evtType != evtBlockTimeout)
        {
            DBG_VERBOSE("smT (Quiescent): Entered with event %s",smEvent_names[evt->evtType]);
            if (ifInfo->this_event.evtType==evtPacketRcvd)
            {
                DBG_VERBOSE(" (%s)\n",Topo_opcode_names[ifInfo->opcode]);
            } else {
                puts("");
            }
        }
    END_TRACE

    switch (evt->evtType)
    {
      case evtPacketRcvd:
        switch (ifInfo->opcode)
        {
          case Opcode_Charge:
          case Opcode_Probe:
          case Opcode_Query:
          case Opcode_QueryLargeTlv:
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Quiescent): Hoisting packet ignored.\n");
            END_TRACE
            break;
          case Opcode_Emit:	// should never occur - explicit event
          case Opcode_Reset:	// should never occur - explicit event
          case Opcode_Discover:	// should never occur - explicit event
          case Opcode_Hello:
          case Opcode_Train:
          case Opcode_ACK:
          case Opcode_QueryResp:
          case Opcode_Flat:
          case Opcode_QueryLargeTlvResp:
            IF_TRACED(TRC_STATE)
                DBG_VERBOSE("smT (Quiescent): Ignored packet w/ known opcode: %s\n",
                       Topo_opcode_names[ifInfo->opcode]);
            END_TRACE
            break;

          default:
            warn("smT (Quiescent): Ignored packet w/ unknown opcode: %d\n",ifInfo->opcode);
            return PROCESSING_ABORTED;
        }   /*** end of switch (ifInfo->opcode) ***/
        break;

      case evtDiscoverRcvd:
        if (evt->isAckingMe)
        {
            uint16_t	gen = ntohs(ifInfo->discover_hdr->mh_gen);
            if (evt->isInternalEvt == FALSE  &&  // if a real acking-Discover,
                gen != 0                     &&  // has a non-zero gen,
                ifInfo->generation != gen)             // that differs from ours,
            {   ifInfo->generation = gen;   }   // then save it for future Hellos.
            IF_TRACED(TRC_STATE)
                DBG_VERBOSE("smT (Quiescent): Leaving for Command state\n");
            END_TRACE
            ifInfo->smT_state = smT_Command;
        }
        break;

      case evtInactivityTimeout:
      case evtResetRcvd:
        /* If the Topo-session was reset, */
        if (evt->ssn == ifInfo->topo_session)
        {
            /* zero the credits, and clear the charge-timer */
            ifInfo->ctc_packets = ifInfo->ctc_bytes = 0;
            CANCEL(ifInfo->charge_timer);
            /* and NULL the ifInfo->topo_session ptr */
            ifInfo->topo_session = NULL;
        }
        break;

      case evtEmitRcvd:
        IF_TRACED(TRC_PACKET)
            DBG_VERBOSE("smT (Quiescent): Hoisting packet (Emit) ignored.\n");
        END_TRACE
        break;

      case evtBlockTimeout:
        break;

      case evtChargeTimeout:
      case evtEmitTimeout:
      case evtHelloDelayTimeout:
      default:
        IF_DEBUG
            DBG_VERBOSE("smT (Quiescent): Ignored event %s\n",smEvent_names[evt->evtType]);
        END_DEBUG
        break;
    }   /*** end of switch (eventType) ***/

    return PROCESSING_COMPLETED;
}

/***********************  C O M M A N D   S T A T E  ***********************/

static
enum sm_Status
smT_CommandHandler( protocol_event_t* evt )
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    IF_TRACED(TRC_STATE)
        if (evt->evtType != evtBlockTimeout)
        {
            DBG_VERBOSE("smT (Command): Entered with event %s",smEvent_names[evt->evtType]);
            if (ifInfo->this_event.evtType==evtPacketRcvd)
            {
                DBG_VERBOSE(" (%s)\n",Topo_opcode_names[ifInfo->opcode]);
            } else {
                puts("");
            }
        }
    END_TRACE

    switch (evt->evtType)
    {
      case evtPacketRcvd:
        switch (ifInfo->opcode)
        {
          case Opcode_Charge:
            ChargeAdd(ifInfo->rcvd_pkt_len);		// also restarts the charge-timeout timer
            if (ifInfo->sequencenum)
            {
                packetio_tx_flat();
                ChargeConsume(ifInfo->tx_len);
            }
            break;

          case Opcode_Probe:
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Command): Logging Probe from " ETHERADDR_FMT "\n",ETHERADDR_PRINT(&ifInfo->base_hdr->tbh_realsrc));
            END_TRACE
            seeslist_enqueue(FALSE, &ifInfo->base_hdr->tbh_realsrc);
            break;

          case Opcode_Query:
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Command): Sending query-Resp\n");
            END_TRACE
            packetio_tx_queryresp();
            break;

          case Opcode_QueryLargeTlv:
            {
                tlv_desc_t *tlvDescr = Tlvs;

                /* search the TLVs, which may not be consecutive or in order, for one with the given number */
                for (;tlvDescr->number != 0; tlvDescr++)
                {
                    if (tlvDescr->number == ifInfo->qltlv_hdr->qh_type)
                    {
                        size_t    offset;

                        IF_TRACED(TRC_PACKET)
                            DBG_VERBOSE("smT (Command): Sending qltlv-Resp with LTLV # 0x%X\n", tlvDescr->number);
                        END_TRACE
                        
                        offset = ntohs(ifInfo->qltlv_hdr->qh_offset) + (ifInfo->qltlv_hdr->qh_rsvd1 << 16);
                        packetio_tx_qltlvResp(ifInfo->sequencenum, tlvDescr, offset);
                        break;
                    }
                }
                /* if it falls out without finding a match, nothing will be sent */
            }
            break;

          case Opcode_Emit:	// should never occur - explicit event
          case Opcode_Reset:	// should never occur - explicit event
          case Opcode_Discover: // should never occur - explicit event
          case Opcode_QueryLargeTlvResp:
          case Opcode_Hello:
          case Opcode_Train:
          case Opcode_ACK:
          case Opcode_QueryResp:
          case Opcode_Flat:
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Command): Ignored packet w/ known opcode: %s\n",Topo_opcode_names[ifInfo->opcode]);
            END_TRACE
            break;

          default:
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Command): Ignored packet w/ unknown opcode: %d\n",ifInfo->opcode);
            END_TRACE
            return PROCESSING_ABORTED;
        }   /*** end of switch (ifInfo->opcode) ***/
        break;

      case evtInactivityTimeout:
      case evtResetRcvd:
        /* If the Topo-session timed out, or was Reset, */
        IF_TRACED(TRC_PACKET)
            DBG_VERBOSE("smT (Command): ResetRcvd, %s for topo-session\n",evt->ssn == ifInfo->topo_session?"is":"is not");
        END_TRACE
        if (evt->ssn == ifInfo->topo_session)
        {
            /* zero the credits, clear the charge-timer, and NULL out the topo-session ptr */
            IF_DEBUG
                puts("zero -> ctc, ctc-timer is cancelled");
                puts("NULL -> topo_session");
                puts("clearing seeslist");
            END_DEBUG
            ifInfo->ctc_packets = ifInfo->ctc_bytes = 0;
            CANCEL(ifInfo->charge_timer);
            /* clear the sees-list, */
            seeslist_clear();
            /* NULL -> topo_session marks the topo-session as completely invalid */
            ifInfo->topo_session = NULL;
            /* and return to Quiescent state. */
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Command): Leaving for Quiescent (Inactive or Reset).\n");
            END_TRACE
            ifInfo->smT_state = smT_Quiescent;
        }
        break;

      case evtChargeTimeout:
        /* zero the credits, and clear the charge-timer */
        ifInfo->ctc_packets = ifInfo->ctc_bytes = 0;
        CANCEL(ifInfo->charge_timer);
        break;

      case evtEmitRcvd:
        /* Special case: Unsequenced Emits MUST NOT have more than one emitee to match Vista
         *               interpretation of the spec. Any that do, MUST BE IGNORED!           */
        {
            topo_emit_header_t*     emit;	/* pointer to emit header in rxbuf */
            uint16_t		    emiteeCnt;

            emit = (topo_emit_header_t*)(ifInfo->base_hdr + 1);
            emiteeCnt = ntohs(emit->eh_numdescs);
            if (emiteeCnt > 1  &&  ifInfo->sequencenum==0)  break;
        }
        /* The response will require charge, so count this receipt */
        ChargeAdd(ifInfo->rcvd_pkt_len);
        /* If ctc is big enuff, process the emits */
        if (SufficientChargeAvailable())
        {
            /* zero the credits, and clear the charge-timer,
             * so we won't timeout for lack of charge */
            ifInfo->ctc_packets = ifInfo->ctc_bytes = 0;
            CANCEL(ifInfo->charge_timer);
            /* ready the list of emitees and their count-remaining */
            ProcessEmits();
            DBG_VERBOSE("smT (Command): emit_remaining = %d\n",ifInfo->emit_remaining);
            if (ifInfo->emit_remaining > 0)
            {
                bool_t  pausing;

                IF_TRACED(TRC_STATE)
                    DBG_VERBOSE("smT (Command): Leaving for Emit\n");
                END_TRACE
                ifInfo->smT_state = smT_Emit;
                pausing = set_emit_timer();
                if (pausing!=TRUE)
                {
                    ifInfo->this_event.evtType = evtEmitTimeout;
                    IF_TRACED(TRC_STATE)
                        DBG_VERBOSE("smT (Command): No Pause in 1st emitee: Inject an EmitTimeout immediately.\n");
                    END_TRACE
                    return KEEP_GOING;
                }
            } else {
                /* Emit with 0 descs: valid, but not very useful.
                 * Stay in Command state (possibly ACKing) */
                if (ifInfo->emit_seqnum)
                {
                    packetio_tx_ack(ifInfo->emit_seqnum);
                    ChargeConsume(ifInfo->tx_len);
                }
            }
        } else {
            /* Otherwise, tx-Flat */
            packetio_tx_flat();
            ChargeConsume(ifInfo->tx_len);
        }
        break;

      case evtBlockTimeout:
        break;

      case evtDiscoverRcvd:
        { /* need block for declarations */
            uint16_t	gen = ntohs(ifInfo->discover_hdr->mh_gen);
            if (evt->isInternalEvt == FALSE  &&  // if a real Discover packet,
                gen != 0                     &&  // has a non-zero gen,
                ifInfo->generation != gen)             // that differs from ours,
                ifInfo->generation = gen;      // then save it for future Hellos.
        }
        break;

      case evtEmitTimeout:
      case evtHelloDelayTimeout:
      default :
        IF_TRACED(TRC_STATE)
            DBG_VERBOSE("smT (Command): Ignored event %s\n",smEvent_names[evt->evtType]);
        END_TRACE
        break;
    }

    return PROCESSING_COMPLETED;
}

/***********************  E M I T   S T A T E  ***********************/

static
enum sm_Status
smT_EmitHandler( protocol_event_t* evt )
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    IF_TRACED(TRC_STATE)
        DBG_VERBOSE("smT (Emit): Entered with event %s",smEvent_names[evt->evtType]);
        if (ifInfo->this_event.evtType==evtPacketRcvd)
        {
            DBG_VERBOSE(" (%s)\n",Topo_opcode_names[ifInfo->opcode]);
        } else {
            puts("");
        }
    END_TRACE

    switch (evt->evtType)
    {
      case evtPacketRcvd:
        switch (ifInfo->opcode)
        {
          case Opcode_Probe:
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Command): Logging Probe from " ETHERADDR_FMT "\n",ETHERADDR_PRINT(&ifInfo->base_hdr->tbh_realsrc));
            END_TRACE
            seeslist_enqueue(FALSE, &ifInfo->base_hdr->tbh_realsrc);
            break;

          case Opcode_Emit:	// should never occur - explicit event
          case Opcode_Reset:	// should never occur - explicit event
          case Opcode_Discover:	// should never occur - explicit event
          case Opcode_Charge:
          case Opcode_Hello:
          case Opcode_Train:
          case Opcode_ACK:
          case Opcode_Query:
          case Opcode_QueryResp:
          case Opcode_Flat:
          case Opcode_QueryLargeTlv:
          case Opcode_QueryLargeTlvResp:
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Emit): Ignored packet w/ known opcode: %s\n",Topo_opcode_names[ifInfo->opcode]);
            END_TRACE
            break;

          default:
            IF_TRACED(TRC_PACKET)
                DBG_VERBOSE("smT (Emit): Ignored packet w/ unknown opcode: %d\n",ifInfo->opcode);
            END_TRACE
            return PROCESSING_ABORTED;
        }   /*** end of switch (ifInfo->opcode) ***/
        break;

      case evtInactivityTimeout:
      case evtResetRcvd:
        /* If the Topo-session was reset, */
        if (evt->ssn == ifInfo->topo_session)
        {
            /* zero the credits & the emits-left, and clear the charge-timer and emit-timer, */
            ifInfo->ctc_packets = ifInfo->ctc_bytes = ifInfo->emit_remaining = 0;
            CANCEL(ifInfo->charge_timer);
            CANCEL(ifInfo->emit_timer);
            /* clear the sees-list, */
            seeslist_clear();
            /* NULL -> topo_session marks the topo-session as completely invalid */
            ifInfo->topo_session = NULL;
            /* and return to Quiescent state. */
            IF_TRACED(TRC_STATE)
                DBG_VERBOSE("smT (Emit): Leaving for Quiescent (Reset or Inactive)\n");
            END_TRACE
            ifInfo->smT_state = smT_Quiescent;
        }
        break;

      case evtEmitTimeout:
        packetio_tx_emitee(ifInfo->emitdesc);
        ifInfo->emit_remaining--;
        ifInfo->emitdesc++;
        /* any more? */
        if (ifInfo->emit_remaining == 0)
        {
            /* go back to Command state (possibly sending ACK as we leave) */
            IF_TRACED(TRC_STATE)
                DBG_VERBOSE("smT (Emit): Leaving for Command\n");
            END_TRACE
            ifInfo->smT_state = smT_Command;
            CANCEL(ifInfo->emit_timer);
            if (ifInfo->emit_seqnum)
                packetio_tx_ack(ifInfo->emit_seqnum);
        } else {
            /* schedule the transmission */
            if (set_emit_timer() == FALSE)
            {
                /* send next with no pause, by re-processing this event */
                return KEEP_GOING;
            }
        }
        break;

      case evtDiscoverRcvd:
      case evtEmitRcvd:

      case evtBlockTimeout:
      case evtChargeTimeout:
      case evtHelloDelayTimeout:
      default :
        IF_TRACED(TRC_STATE)
            DBG_VERBOSE("smT (Emit): Ignored event %s\n",smEvent_names[evt->evtType]);
        END_TRACE
        break;
    }

    return PROCESSING_COMPLETED;
}


/***********************                             ***********************/
/***********************  S T A T E   M A C H I N E  ***********************/
/***********************                             ***********************/

/* smT_process_event() - process an incoming event (rcvd pkt or timeout)
 *                       and distribute to the current state's handler	*/

enum sm_Status
smT_process_event( protocol_event_t* evt )
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    enum sm_Status	ClockControl = PROCESSING_COMPLETED;

//        ifInfo->topo_session->ssn_is_valid != TRUE  ||

    if (ifInfo->topo_session == NULL                ||	/* if there is no valid topo-session, or    */
        (evt->ssn && evt->ssn != ifInfo->topo_session))       /* this ssn doesn't match the valid topo... */
    {
        return PROCESSING_ABORTED;	                /* Fail with an Abort */
    }

    IF_DEBUG
//*/    DBG_VERBOSE("smT_process_event: Entered with event %s\n",smEvent_names[evt->evtType]);
    END_DEBUG

    /* allow this state machine to autoclock itself, and request further processing */
    do {
        IF_DEBUG
//*/        DBG_VERBOSE("smT_process_event: Processing event %s\n",smEvent_names[evt->evtType]);
        END_DEBUG
        switch (ifInfo->smT_state)
        {
          case smT_Quiescent:
            ClockControl = smT_QuiescentHandler( evt );
            break;

          case smT_Command:
            ClockControl = smT_CommandHandler( evt );
            break;

          case smT_Emit:
            ClockControl = smT_EmitHandler( evt );
            break;

          default:
            ClockControl = PROCESSING_ABORTED;	/* Stop! smT in unknown state! */
            break;
        }
        IF_TRACED(TRC_STATE)
            if (ClockControl == KEEP_GOING) puts("smT: auto-clocking...");
        END_TRACE
    } while (ClockControl == KEEP_GOING);

    return ClockControl;
}
