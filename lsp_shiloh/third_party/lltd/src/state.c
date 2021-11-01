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
#include <string.h>
#include <assert.h>

#include "globals.h"

#include "statemachines.h"
#include "bandfuncs.h"
#include "packetio.h"

/****************************
 * Timeout handlers - 
 * these are the time-based entry points, which also clock the state machine.
 */

void
state_block_timeout(void *cookie)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    ifInfo->block_timer = NULL;
    ifInfo->this_event.evtType = evtBlockTimeout;
    state_process_timeout();
}

void
state_charge_timeout(void *cookie)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    ifInfo->charge_timer     = NULL;
    ifInfo->ctc_packets      = 0;
    ifInfo->ctc_bytes        = 0;
    ifInfo->this_event.evtType  = evtChargeTimeout;
    ifInfo->this_event.ssn      = (session_t*) cookie;
    state_process_timeout();
}

void
state_emit_timeout(void *cookie)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    ifInfo->emit_timer       = NULL;
    ifInfo->this_event.evtType  = evtEmitTimeout;

    /* Probes are forced to associate with the mapping session, if there is one. */
    if (ifInfo->topo_session != NULL  &&  ifInfo->topo_session->ssn_is_valid)
    {
        ifInfo->this_event.ssn = ifInfo->topo_session;
    }

    state_process_timeout();
}

void
state_hello_delay_timeout(void *cookie)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    ifInfo->hello_timer      = NULL;
    ifInfo->this_event.evtType  = evtHelloDelayTimeout;
    ifInfo->this_event.ssn      = (session_t*) cookie;
    state_process_timeout();
}

void
state_inactivity_timeout(void *cookie)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    ifInfo->this_event.evtType  = evtInactivityTimeout;
    ifInfo->this_event.ssn      = (session_t*) cookie;
    ifInfo->this_event.ssn->ssn_InactivityTimer = NULL;
    state_process_timeout();
}


/* This function locates an existing session that is associated with the passed-in address */
static session_t *
find_session(etheraddr_t *this_addr)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    session_t   *ssn = &ifInfo->sessions[0];
    int i;

    for (i=0; i < MAX_NUM_SESSIONS; ssn++, i++)
    {
        if ( (ssn->ssn_is_valid) && (ETHERADDR_EQUALS(&ssn->ssn_mapper_real, this_addr)) )
        {
//*/        DBG_VERBOSE("find_session returning session %d @ %X\n",i,(uint)ssn);
            return ssn;
        }
    }
//*/DEBUG({puts("find_session returning NULL");})
    return NULL;
}


static session_t *
new_session()
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    session_t   *ssn = &ifInfo->sessions[0];
    int i;

    for (i=0; i < MAX_NUM_SESSIONS; ssn++, i++)
    {
        if ( !(ssn->ssn_is_valid) )
        {
            ssn->ssn_is_valid = TRUE;
	    ssn->ssn_count = BAND_TXC;
            ssn->ssn_XID = 0;
            memset(&ssn->ssn_mapper_real,0,sizeof(etheraddr_t));
            memset(&ssn->ssn_mapper_current,0,sizeof(etheraddr_t));
            ssn->ssn_use_broadcast = TRUE;
            ssn->ssn_TypeOfSvc = ToS_Unknown;
            ssn->ssn_InactivityTimer = NULL;
            return ssn;
        }
    }
    return NULL;
}


/*****************************************************************************
 *
 * This code processes the current packet-event (in this_event) by locating
 * (and possibly creating) the session with which it must be associated,
 * then passing it to the 3 state machines, smS, smE, and smT, in that order.
 *
 *****************************************************************************/ 

uint
state_process_packet()
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    session_t           *this_session;
    enum sm_Status       smStatus;

    IF_TRACED((TRC_STATE|TRC_PACKET))
        DBG_VERBOSE("state_process_packet: Entered with event %s",smEvent_names[ifInfo->this_event.evtType]);
        if (ifInfo->this_event.evtType==evtPacketRcvd)
        {
            DBG_VERBOSE(" (%s)\n",Topo_opcode_names[ifInfo->opcode]);
        } else {
            puts("");
        }
    END_TRACE

    ifInfo->this_event.isInternalEvt = FALSE;	// It's a real event, not internally generated

    /* First, look this RealSrc up in the session table, to
     * locate any association with an established session.
     *
     * If there is no matching session, create a new one, iff the
     * packet is a valid Discover of either topo- or quick- TOS ... */

    ifInfo->this_event.isNewSession = FALSE;

    if ((this_session = find_session(&ifInfo->base_hdr->tbh_realsrc)) == NULL)
    {
        /* Not found: Check for a Discovery packet (validated in packetio.c) */
        if (ifInfo->opcode == Opcode_Discover)
        {
            /* Create a new session for this association */
            if ((this_session = new_session()) == NULL)
            {
                /* No room in the table: drop the packet and whine. */
                warn("state_process_packet: no room to create new session. Packet dropped.\n");
                return UINT_MAX;
            }
            ifInfo->this_event.isNewSession = TRUE;

            /* Fill in the newly valid session table entry with info from the packet */
            this_session->ssn_XID            = ifInfo->sequencenum;
            this_session->ssn_mapper_real    = ifInfo->base_hdr->tbh_realsrc;
            this_session->ssn_mapper_current = ifInfo->ethernet_hdr->eh_src;
            this_session->ssn_TypeOfSvc      = ifInfo->base_hdr->tbh_tos;
            IF_TRACED(TRC_STATE)
                DBG_VERBOSE("New Session:\n\tXID = %X\n\treal address: " ETHERADDR_FMT \
                       "\n",this_session->ssn_XID, \
                       ETHERADDR_PRINT(&this_session->ssn_mapper_real) );
                DBG_VERBOSE("\tcurrent address: " ETHERADDR_FMT "\n\tToS: %s\n",
                       ETHERADDR_PRINT(&this_session->ssn_mapper_current),
                       Lld2_tos_names[this_session->ssn_TypeOfSvc] );
            END_TRACE
            ifInfo->this_event.ssn = this_session;

        }   /*** end of if (ifInfo->opcode == Opcode_Discover) ***/

        /* Probes are forced to associate with the mapping session, if there is one. */
        if (ifInfo->opcode == Opcode_Probe)
        {
             if (ifInfo->topo_session != NULL  &&  ifInfo->topo_session->ssn_is_valid)
             {
                this_session = ifInfo->topo_session;
             }
        }

    }   /*** endo of if (find_session()==NULL) ***/

    /* We have associated whatever session that we can with this packet - pass to state machines */
    ifInfo->this_event.ssn = this_session;

    smStatus = smS_process_event( &ifInfo->this_event );

    if (smStatus != PROCESSING_ABORTED)
    {
        smStatus = smE_process_event( &ifInfo->this_event );
    }

    if (smStatus != PROCESSING_ABORTED)
    {
        smStatus = smT_process_event( &ifInfo->this_event );
    }

    /* Remove any "new-session" marking */
    ifInfo->this_event.isNewSession = FALSE;

    IF_TRACED(TRC_PACKET)
        DBG_VERBOSE("state_process_packet: Leaving - done with event %s\n",smEvent_names[ifInfo->this_event.evtType]);
    END_TRACE
    return 0;	/* Success! */
}


/******************************************************************************
 *
 * This code processes the current timeout-event (in this_event). Any session
 * associated with this event (only happens with activity-timeouts) is already
 * noted in (this_event).
 *
 ******************************************************************************/ 

uint
state_process_timeout()
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    enum sm_Status         smStatus;

    IF_TRACED(TRC_STATE)
        if (ifInfo->this_event.evtType!=evtBlockTimeout)
            DBG_VERBOSE("state_process_timeout: Entered with event %s\n",smEvent_names[ifInfo->this_event.evtType]);
    END_TRACE

    ifInfo->rcvd_pkt_len = 0;

    /* Finish initializing the protocol-event */

    /* ifInfo->this_event.evtType    = <set in individual timeout handler> */
    /* ifInfo->this_event.ssn        = <set in individual timeout handler> */
    ifInfo->this_event.isNewSession  = FALSE;
    ifInfo->this_event.isAckingMe    = FALSE;
    ifInfo->this_event.isInternalEvt = FALSE;	// It's a real event, not internally generated
    ifInfo->this_event.numDescrs     = 0;

    /* pass Hello-Delay timer events to each existing session's smS state-machine */
    if (ifInfo->this_event.evtType == evtHelloDelayTimeout)
    {
        session_t     *ssn = &ifInfo->sessions[0];
        int            i;

        for (i=0; i < MAX_NUM_SESSIONS; ssn++, i++)
        {
            if (ssn->ssn_is_valid)
            {
                ifInfo->this_event.ssn = ssn;
                smS_process_event( &ifInfo->this_event );
            }
        }
        ifInfo->this_event.ssn = NULL;
    }

    /* Pass per-session activity timeouts to the associated session.  The remaining
     * timeouts, for charge and emit, need only be processed by smE & smT...        */
    if (ifInfo->this_event.evtType == evtInactivityTimeout)
    {
        smS_process_event( &ifInfo->this_event );
    }

    /* send all the timeouts to smE and then smT */

    smStatus = smE_process_event( &ifInfo->this_event );

    if (smStatus != PROCESSING_ABORTED)
    {
        smT_process_event( &ifInfo->this_event );
    }

    return 0;	/* Success! */
}


/****************************
 * Helper functions -
 * actions performed as part of state processing.
 */

/* Restart the inactivity timer for the session associated with the current event */
void
restart_inactivity_timer(uint32_t timeout)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    struct timeval now;

    if (ifInfo->this_event.ssn == NULL  ||  ifInfo->this_event.ssn->ssn_is_valid != TRUE)   return;

    gettimeofday(&now, NULL);
    timeval_add_ms(&now, timeout);
    CANCEL(ifInfo->this_event.ssn->ssn_InactivityTimer);
    ifInfo->this_event.ssn->ssn_InactivityTimer =
        event_add(rif, &now, state_inactivity_timeout, ifInfo->this_event.ssn);
}


/* Searches session table - returns TRUE if all valid sessions are in smS_Complete state */

bool_t
OnlyCompleteSessions(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    session_t   *ssn = &ifInfo->sessions[0];
    int i;

    for (i=0; i < MAX_NUM_SESSIONS; ssn++, i++)
    {
        if (ssn->ssn_is_valid && ssn->ssn_state != smS_Complete)
        {
            return FALSE;
        }
    }
    return TRUE;
}


/* Searches session table - returns TRUE if all sessions are invalid */

bool_t
SessionTableIsEmpty(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    session_t   *ssn = &ifInfo->sessions[0];
    int i;

    for (i=0; i < MAX_NUM_SESSIONS; ssn++, i++)
    {
        if (ssn->ssn_is_valid)
        {
            return FALSE;
        }
    }
    return TRUE;
}

bool_t
set_emit_timer(void)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *ifInfo = IF_INFO(rif);
    ASSERT(ifInfo != NULL);

    topo_emitee_desc_t *ed;

    assert(ifInfo->emit_remaining > 0);
    assert(ifInfo->emit_timer == NULL);

    /* get the next emitee_desc and schedule a callback when it is due
     * to be transmitted */
    ed = ifInfo->emitdesc;
    if (ed->ed_pause != 0)
    {
	struct timeval now;
	gettimeofday(&now, NULL);
	timeval_add_ms(&now, ed->ed_pause);
	ifInfo->emit_timer = event_add(rif, &now, state_emit_timeout, NULL);
        return TRUE;
    } else {
	/* no pause; return PAUSE=FALSE */
	return FALSE;
    }
}
