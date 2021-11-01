/** @file wps_state.c
 *  @brief This file contains functions for WPS state machine.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#include <stddef.h>   /* for size_t */
#include "tx_api.h"
#include "common.h" 
#include "wps_msg.h"

#include <string.h>
#include "raw_socket_api.h"
#include "wlanInterface.h"
#include "wps_util.h"
#include "wps_def.h"
#include "wps_wlan.h"
#include "wps_os.h"
#include "wps_eapol.h"
#include "wireless_copy.h"

int wps_registrar_state_check(PWPS_INFO pwps_info, u16 msg_type);
int wps_registrar_state_transition(PWPS_INFO pwps_info, u16 msg_type);

extern int wps_install_current_ipv4_cfg(char *ifname, u32 ip_addr,
                                        u32 subnet_mask);

int wps_uap_session_complete_handler(PWPS_INFO pwps_info, WPS_DATA * wps_s);
int wps_wfd_session_complete_handler(PWPS_INFO pwps_info, WPS_DATA * wps_s);

#define MAX_EAP_FAIL_COUNT 2
/********************************************************
        Local Variables
********************************************************/
static int (*wps_msg_process[17]) (PWPS_INFO, u8 *, u16) = {
wps_dummy,
        wps_dummy,
        wps_eap_response_identity_process,
        wps_eap_request_start_process,
        wps_eap_M1_frame_process,
        wps_eap_M2_frame_process,
        wps_eap_M2D_frame_process,
        wps_eap_M3_frame_process,
        wps_eap_M4_frame_process,
        wps_eap_M5_frame_process,
        wps_eap_M6_frame_process,
        wps_eap_M7_frame_process,
        wps_eap_M8_frame_process,
        wps_ack_message_process,
        wps_nack_message_process, wps_done_message_process, wps_dummy};

static int (*wps_msg_prepare[18]) (PWPS_INFO) = {
wps_eapol_start_prepare,
        wps_eap_request_identity_prepare,
        wps_eap_response_identity_prepare,
        wps_eap_request_start_prepare,
        wps_eap_M1_frame_prepare,
        wps_eap_M2_frame_prepare,
        wps_eap_M2D_frame_prepare,
        wps_eap_M3_frame_prepare,
        wps_eap_M4_frame_prepare,
        wps_eap_M5_frame_prepare,
        wps_eap_M6_frame_prepare,
        wps_eap_M7_frame_prepare,
        wps_eap_M8_frame_prepare,
        wps_ack_message_prepare,
        wps_nack_message_remap,
        wps_done_message_prepare, wps_eap_fail_frame_prepare,
        wps_eap_frag_ack_frame_prepare};

/********************************************************
        Global Variables
********************************************************/
/** WPS global */
extern WPS_DATA wps_global;
extern int wps_eapol_txStart(void);
/** IE buffer index */
extern short ie_index;

/** IE buffer index */
extern short ap_assocresp_ie_index;
/** Interface switch required or not */
extern int switch_intf;
/** Autonomous GO flag */
extern int auto_go;

/********************************************************
        Local Functions
********************************************************/

void abort_wps(void);


/** 
 *  @brief Print WPS message type information for debugging
 *  
 *  @param str      Print string
 *  @param msg      Message type
 *  @return         None
 */
static void
wps_debug_print_msgtype(const char *str, int msg)
{
    ENTER();

    switch (msg) {
    case WPS_EAPOL_MSG_START:
        wps_printf(DEBUG_WPS_STATE, "%s: EAPOL_START\n", str);
        break;
    case WPS_EAP_MSG_REQUEST_IDENTITY:
        wps_printf(DEBUG_WPS_STATE, "%s: EAP_REQUEST_IDENTITY\n", str);
        break;
    case WPS_EAP_MSG_RESPONSE_IDENTITY:
        wps_printf(DEBUG_WPS_STATE, "%s: EAP_RESPONSE_IDENTITY\n", str);
        break;
    case WPS_EAP_START:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_START\n", str);
        break;
    case WPS_EAP_M1:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M1\n", str);
        break;
    case WPS_EAP_M2:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M2\n", str);
        break;
    case WPS_EAP_M2D:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M2D\n", str);
        break;
    case WPS_EAP_M3:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M3\n", str);
        break;
    case WPS_EAP_M4:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M4\n", str);
        break;
    case WPS_EAP_M5:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M5\n", str);
        break;
    case WPS_EAP_M6:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M6\n", str);
        break;
    case WPS_EAP_M7:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M7\n", str);
        break;
    case WPS_EAP_M8:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_M8\n", str);
        break;
    case WPS_MSG_DONE:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_DONE\n", str);
        break;
    case WPS_MSG_ACK:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_ACK\n", str);
        break;
    case WPS_MSG_NACK:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_NACK\n", str);
        break;
    case WPS_EAP_MSG_FAIL:
        wps_printf(DEBUG_WPS_STATE, "%s: EAP_FAIL\n", str);
        break;
    case WPS_MSG_FRAG_ACK:
        wps_printf(DEBUG_WPS_STATE, "%s: WPS_FRAG_ACK\n", str);
        break;
    default:
        wps_printf(DEBUG_WPS_STATE, "%s: ERROR - UNKNOWN TYPE %d!\n", str, msg);
        break;
    }

    LEAVE();
}

/** 
 *  @brief Print WPS state information for debugging
 *  
 *  @param str      Print string
 *  @param state    State information
 *  @return         None
 */
static void
wps_debug_print_state(const char *str, int state)
{
    ENTER();

    switch (state) {
    case WPS_STATE_A:
        wps_printf(DEBUG_WPS_STATE, "%s = A\n", str);
        break;
    case WPS_STATE_B:
        wps_printf(DEBUG_WPS_STATE, "%s = B\n", str);
        break;
    case WPS_STATE_C:
        wps_printf(DEBUG_WPS_STATE, "%s = C\n", str);
        break;
    case WPS_STATE_D:
        wps_printf(DEBUG_WPS_STATE, "%s = D\n", str);
        break;
    case WPS_STATE_E:
        wps_printf(DEBUG_WPS_STATE, "%s = E\n", str);
        break;
    case WPS_STATE_F:
        wps_printf(DEBUG_WPS_STATE, "%s = F\n", str);
        break;
    case WPS_STATE_G:
        wps_printf(DEBUG_WPS_STATE, "%s = G\n", str);
        break;
    case WPS_STATE_H:
        wps_printf(DEBUG_WPS_STATE, "%s = H\n", str);
        break;
    case WPS_STATE_I:
        wps_printf(DEBUG_WPS_STATE, "%s = I\n", str);
        break;
    default:
        wps_printf(DEBUG_WPS_STATE, "%s = Unknown WPS State\n", str);
        break;
    }

    LEAVE();
}

/** 
 *  @brief Extract the message type field from EAP packet
 *  
 *  @param peap     A pointer to EAP frame header
 *  @return         Message type
 */
static int
wps_get_message_type(PEAP_FRAME_HEADER peap)
{
    u8 *buffer;
    int msg_type = 0;
    PTLV_DATA_HEADER ptlv;
    int type, length;

    ENTER();

    switch (peap->op_code) {
    case WPS_Start:
        msg_type = WPS_EAP_START;
        break;
    case WPS_Ack:
    case WPS_Nack:
    case WPS_Msg:
    case WPS_Done:
        buffer = (u8 *) peap;
        buffer += SZ_EAP_WPS_FRAME_HEADER;
        ptlv = (PTLV_DATA_HEADER) buffer;
        length = ntohs(ptlv->length);
        buffer += (SZ_TLV_HEADER + length);
        ptlv = (PTLV_DATA_HEADER) buffer;
        type = ntohs(ptlv->type);
        if (type == SC_Message_Type) {
            buffer += SZ_TLV_HEADER;
            msg_type = (u8) * buffer;
        }
        break;
    default:
        wps_printf(MSG_WARNING, "Unknown opcode: 0x%x\n", peap->op_code);
        break;
    }

    wps_debug_print_msgtype("RX WPS Message Type", msg_type);

    LEAVE();
    return msg_type;
}

/** 
 *  @brief Process Enrollee state machine checking
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param msg_type     Message type to be checked
 *  @return             Message type to be sent in current state
 */
static int
wps_enrollee_state_check(PWPS_INFO pwps_info, u16 msg_type)
{
    int msg_next = WPS_EAP_UNKNOWN;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;

    ENTER();

    switch (msg_type) {
    case WPS_EAP_MSG_REQUEST_IDENTITY:
        pwps_info->eap_identity_count++;
        if (pwps_info->eap_identity_count < 1)
            msg_next = WPS_EAPOL_MSG_START;
        else if ((pwps_info->state == WPS_STATE_A) ||
                 (pwps_info->state == WPS_STATE_B))
            msg_next = WPS_EAP_MSG_RESPONSE_IDENTITY;
        break;

    case WPS_EAP_MSG_RESPONSE_IDENTITY:
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_B)
                msg_next = WPS_EAP_M1;
        }
        break;

    case WPS_EAP_START:
        pwps_info->eap_identity_count = 0;
        if ((pwps_info->state == WPS_STATE_B) ||
            (pwps_info->state == WPS_STATE_C) ||
            (pwps_info->state == WPS_STATE_D))
            msg_next = WPS_EAP_M1;
        break;
    case WPS_EAP_M2:
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->wps_ap_setup_locked == WPS_SET)
                msg_next = WPS_MSG_NACK;
            else if (pwps_info->state == WPS_STATE_D)
                msg_next = WPS_EAP_M3;
        } else
            if ((pwps_info->state == WPS_STATE_D) ||
                (pwps_info->state == WPS_STATE_E) ||
                (pwps_info->state == WPS_STATE_F))
            msg_next = WPS_EAP_M3;
        break;
    case WPS_EAP_M2D:
        if ((pwps_info->state == WPS_STATE_D) ||
            (pwps_info->state == WPS_STATE_B) ||
            (pwps_info->state == WPS_STATE_E))
            msg_next = WPS_MSG_ACK;
        break;
    case WPS_EAP_M4:
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_D)
                msg_next = WPS_EAP_M5;
        } else
            if ((pwps_info->state == WPS_STATE_E) ||
                (pwps_info->state == WPS_STATE_F))
            msg_next = WPS_EAP_M5;
        break;
    case WPS_EAP_M6:
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_D)
                msg_next = WPS_EAP_M7;
        } else
            if ((pwps_info->state == WPS_STATE_E) ||
                (pwps_info->state == WPS_STATE_F))
            msg_next = WPS_EAP_M7;
        break;
    case WPS_EAP_M8:
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_D)
                msg_next = WPS_MSG_DONE;
        } else
            if ((pwps_info->state == WPS_STATE_E) ||
                (pwps_info->state == WPS_STATE_F) ||
                (pwps_info->state == WPS_STATE_H))
            msg_next = WPS_MSG_DONE;
        break;
    case WPS_MSG_NACK:
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_F)
                msg_next = WPS_EAP_MSG_FAIL;
        } else if (pwps_info->state == WPS_STATE_I)
            msg_next = WPS_MSG_NACK;
        break;
    case WPS_MSG_ACK:
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_G)
                msg_next = WPS_EAP_MSG_FAIL;
        }
        break;
    case WPS_EAP_MSG_FAIL:
        if ((pwps_info->state == WPS_STATE_A) ||
            (pwps_info->state == WPS_STATE_B))
            msg_next = WPS_EAP_MSG_RESPONSE_IDENTITY;
        break;
    default:
        break;
    }                           /* end of swtich */

    wps_debug_print_msgtype("WPS Next Message", msg_next);

    LEAVE();
    return msg_next;
}

/** 
 *  @brief Process Enrollee state machine transition
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param msg_type     Message type for triggering state transition
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_enrollee_state_transition(PWPS_INFO pwps_info, u16 msg_type)
{
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;
    int status = WPS_STATUS_SUCCESS;

    ENTER();

    /* Current WPS State */
    wps_debug_print_state("State Transition Before", pwps_info->state);

    switch (msg_type) {
    case WPS_EAPOL_MSG_START:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : EAPOL_START\n");

        pwps_info->last_recv_wps_msg = -1;

        /* Reset Public keys and E-Hash, R-Hash */
        wps_reset_wps_state(pwps_info);
        break;
    case WPS_EAP_MSG_RESPONSE_IDENTITY:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : EAP_RESPONSE_IDENTITY\n");

        pwps_info->register_completed = WPS_CANCEL;
        pwps_info->registration_in_progress = WPS_SET;
        wps_printf(DEBUG_WPS_STATE, "Set registration in progress %d\n",
                   pwps_info->registration_in_progress);

        if (pwps_info->state == WPS_STATE_A) {
            pwps_info->state = WPS_STATE_B;
            wps_printf(DEBUG_WPS_STATE, "State A->B\n");
        }
        break;
    case WPS_EAP_START:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_START\n");

        pwps_info->register_completed = WPS_CANCEL;
        pwps_info->registration_in_progress = WPS_SET;
        wps_printf(DEBUG_WPS_STATE, "Set registration in progress %d\n",
                   pwps_info->registration_in_progress);

        if (pwps_info->state == WPS_STATE_A)
            pwps_info->state = WPS_STATE_B;

        if (pwps_info->state == WPS_STATE_B) {
            pwps_info->state = WPS_STATE_C;
            wps_printf(DEBUG_WPS_STATE, "State B->C\n");
        }
        break;
    case WPS_EAP_M1:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M1\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_B) {
                pwps_info->state = WPS_STATE_C;
                wps_printf(DEBUG_WPS_STATE, "State B->C\n");
            }
        } else if (pwps_info->state == WPS_STATE_C) {
            pwps_info->state = WPS_STATE_D;
            wps_printf(DEBUG_WPS_STATE, "State C->D\n");
        }
        break;
    case WPS_EAP_M2:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M2\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_C) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State C->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_D) {
            pwps_info->state = WPS_STATE_E;
            wps_printf(DEBUG_WPS_STATE, "State D->E\n");
        }
        break;
    case WPS_EAP_M2D:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M2D\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if ((pwps_info->state == WPS_STATE_C) ||
                (pwps_info->state == WPS_STATE_D))
                pwps_info->state = WPS_STATE_H;
        } else if ((pwps_info->state == WPS_STATE_D) ||
                   (pwps_info->state == WPS_STATE_B))
            pwps_info->state = WPS_STATE_E;
        break;
    case WPS_EAP_M3:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M3\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_E;
                wps_printf(DEBUG_WPS_STATE, "State D->E\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_F;
            wps_printf(DEBUG_WPS_STATE, "State E->F\n");
        }
        break;
    case WPS_EAP_M4:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M4\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State E->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_F) {
            pwps_info->state = WPS_STATE_E;
            wps_printf(DEBUG_WPS_STATE, "State F->E\n");
        }
        break;
    case WPS_EAP_M5:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M5\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_E;
                wps_printf(DEBUG_WPS_STATE, "State D->E\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_F;
            wps_printf(DEBUG_WPS_STATE, "State E->F\n");
        }
        break;
    case WPS_EAP_M6:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M6\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State E->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_F) {
            pwps_info->state = WPS_STATE_E;
            wps_printf(DEBUG_WPS_STATE, "State F->E\n");
        }
        break;
    case WPS_EAP_M7:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M7\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_E;
                wps_printf(DEBUG_WPS_STATE, "State D->E\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_F;
            wps_printf(DEBUG_WPS_STATE, "State E->F\n");
        }
        break;
    case WPS_EAP_M8:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M8\n");
        pwps_info->register_completed = WPS_SET;
        pwps_info->registration_in_progress = WPS_CANCEL;
        wps_printf(DEBUG_WPS_STATE, "Set registration in progress %d\n",
                   pwps_info->registration_in_progress);

        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State E->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_F) {
            pwps_info->state = WPS_STATE_E;
            wps_printf(DEBUG_WPS_STATE, "State F->E\n");
        }
        break;
    case WPS_MSG_DONE:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_DONE\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_E;
                wps_printf(DEBUG_WPS_STATE, "State D->E\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_H;
            wps_printf(DEBUG_WPS_STATE, "State E->H\n");
        }
        break;
    case WPS_MSG_ACK:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_ACK\n");
        if (wps_s->bss_type == BSS_TYPE_UAP) {
            if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_G;
                wps_printf(DEBUG_WPS_STATE, "State E->G\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_D;
            wps_printf(DEBUG_WPS_STATE, "State E->D\n");
        }
        break;
    case WPS_MSG_NACK:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_NACK\n");
        switch (pwps_info->state) {
        case WPS_STATE_D:
            if (wps_s->bss_type == BSS_TYPE_UAP) {
                pwps_info->state = WPS_STATE_H;
                wps_printf(DEBUG_WPS_STATE, "State D->H\n");
            }
            break;
        case WPS_STATE_H:
            if (wps_s->bss_type == BSS_TYPE_UAP) {
                pwps_info->state = WPS_STATE_F;
                wps_printf(DEBUG_WPS_STATE, "State H->F\n");
            }
            break;
        case WPS_STATE_E:
            if (wps_s->bss_type == BSS_TYPE_UAP) {
                pwps_info->state = WPS_STATE_F;
                wps_printf(DEBUG_WPS_STATE, "State E->F\n");
                if (pwps_info->role == WPS_ENROLLEE &&
                    pwps_info->eap_msg_sent == WPS_EAP_M7) {

                    wps_printf(DEBUG_WPS_MSG,
                               "Setting AP config read only variable.\n");
                    /* Received Nack after M7. This is AP config read case */
                    pwps_info->read_ap_config_only = WPS_SET;

                }
            } else {
                pwps_info->state = WPS_STATE_G;
                wps_printf(DEBUG_WPS_STATE, "State E->G\n");
            }
            break;
        case WPS_STATE_I:
            if (wps_s->bss_type != BSS_TYPE_UAP) {
                pwps_info->state = WPS_STATE_G;
                wps_printf(DEBUG_WPS_STATE, "State I->G\n");
            }
            break;
        case WPS_STATE_F:
            if (wps_s->bss_type != BSS_TYPE_UAP) {
                pwps_info->state = WPS_STATE_I;
                wps_printf(DEBUG_WPS_STATE, "State F->I\n");
            }
            break;
        default:
            break;
        }
        break;
    case WPS_EAP_MSG_FAIL:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : EAP_FAIL\n");

        pwps_info->last_recv_wps_msg = -1;
        pwps_info->registration_in_progress = WPS_CANCEL;
        wps_printf(DEBUG_WPS_STATE, "Set registration in progress %d\n",
                   pwps_info->registration_in_progress);

        switch (pwps_info->state) {
        case WPS_STATE_F:
            if (wps_s->bss_type == BSS_TYPE_UAP) {
                pwps_info->state = WPS_STATE_A;
                wps_printf(DEBUG_WPS_STATE, "State F->A\n");
            }
            break;
        case WPS_STATE_D:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State D->A\n");
            break;
        case WPS_STATE_G:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State G->A\n");
            break;
        case WPS_STATE_H:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State H->A\n");
            break;
        case WPS_STATE_I:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State I->A\n");
            break;
        default:
            break;
        }

    default:
        break;

    }                           /* end of switch */

    /* New WPS State */
    wps_debug_print_state("State Transition After", pwps_info->state);

    LEAVE();
    return status;
}

/** 
 *  @brief Enrollee EAP packet process handling
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param peap         A pointer to EAP frame header
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_eap_request_message_handler(PWPS_INFO pwps_info, PEAP_FRAME_HEADER peap)
{
    int status = WPS_STATUS_SUCCESS;
    int msg_next, msg_type;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;

    ENTER();

    pwps_info->peer_id = peap->identifier;

    if (peap->type == EAP_TYPE_IDENTITY) {
        wps_printf(DEBUG_WPS_STATE, "EAP_REQUEST_IDENTITY Received\n");
        wps_debug_print_msgtype("RX WPS Message Type\n",
                                WPS_EAP_MSG_REQUEST_IDENTITY);

        if (pwps_info->set_timer == WPS_SET) {
            wps_cancel_timer(wps_txTimer_handler, pwps_info);
            pwps_info->set_timer = WPS_CANCEL;
            pwps_info->wps_msg_resent_count = 0;
        }

        msg_type = WPS_EAP_MSG_REQUEST_IDENTITY;
        pwps_info->last_recv_wps_msg = msg_type;

        if (pwps_info->role == WPS_ENROLLEE
            || (IS_DISCOVERY_ENROLLEE(pwps_info))
        ) {
            msg_next = wps_enrollee_state_check(pwps_info,
                                                WPS_EAP_MSG_REQUEST_IDENTITY);
        } else {
            msg_next = wps_registrar_state_check(pwps_info,
                                                 WPS_EAP_MSG_REQUEST_IDENTITY);
        }
        if (msg_next != WPS_EAP_UNKNOWN)
            status =
                wps_msg_transmit(pwps_info, msg_next,
                                 pwps_info->wps_msg_timeout);
    } else if (peap->type == EAP_TYPE_WPS) {

        switch (peap->op_code) {
        case WPS_Msg:
            {

                msg_type = wps_get_message_type(peap);

                if ((msg_type > pwps_info->last_recv_wps_msg) ||
                    (msg_type == WPS_M2D) ||
                    ((pwps_info->last_recv_wps_msg == WPS_M2D) &&
                     (msg_type == WPS_M2))) {

                    wps_printf(DEBUG_WPS_STATE, "Last Message received 0x%x\n",
                               pwps_info->last_recv_wps_msg);
                    wps_printf(DEBUG_WPS_STATE, "Current message 0x%x\n",
                               msg_type);

                    if ((msg_type == pwps_info->last_recv_wps_msg)
                        && (msg_type != WPS_M2D)) {
                    /** recevied the retransmitted message
                     *  use the previous transmitted message, 
                     *  instead of current tx message
                     */
                        wps_printf(DEBUG_WPS_STATE,
                                   "Received reTx EAPOL Req message: 0x%x",
                                   msg_type);
                        memcpy(&pwps_info->last_message,
                               &pwps_info->last_tx_message,
                               sizeof(MESSAGE_BUFFER));
                    }

                    pwps_info->last_recv_wps_msg = msg_type;

                } else {
                    wps_printf(DEBUG_WPS_STATE, "rx_msg: Dropping unexpected msg! "
                           "(prev_msg: 0x%x, curr_msg: 0x%x)\n",
                           pwps_info->last_recv_wps_msg, msg_type);
                    break;
                }

                if (pwps_info->set_timer == WPS_SET) {
                    wps_cancel_timer(wps_txTimer_handler, pwps_info);
                    pwps_info->set_timer = WPS_CANCEL;
                    pwps_info->wps_msg_resent_count = 0;
                }

                if (msg_type != WPS_EAP_UNKNOWN)
                    status =
                        (*wps_msg_process[msg_type]) (pwps_info, (u8 *) peap,
                                                      peap->length);
                else
                    status = WPS_STATUS_FAIL;

                if (status == WPS_STATUS_SUCCESS) {

                    if (pwps_info->role == WPS_ENROLLEE
                        || (IS_DISCOVERY_ENROLLEE(pwps_info))
                        ) {
                        wps_enrollee_state_transition(pwps_info, msg_type);
                        msg_next =
                            wps_enrollee_state_check(pwps_info, msg_type);

                        if (wps_s->bss_type == BSS_TYPE_UAP &&
                            pwps_info->wps_ap_setup_locked == WPS_SET &&
                            msg_next == WPS_MSG_NACK &&
                            msg_type == WPS_EAP_M2) {
                            wps_printf(MSG_ERROR, 
                                    "WPS AP Setup is locked because of probable attack.\n");
                            wps_printf(MSG_ERROR, 
                                    "WPS Registration for AP Configuration will not continue.\n");
                        }
                    } else {
                        wps_registrar_state_transition(pwps_info, msg_type);
                        msg_next =
                            wps_registrar_state_check(pwps_info, msg_type);
                    }
                    if (msg_next != WPS_EAP_UNKNOWN)
                        status =
                            wps_msg_transmit(pwps_info, msg_next,
                                             pwps_info->wps_msg_timeout);
                    else
                        status =
                            wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                             pwps_info->wps_msg_timeout);
                } else
                    status =
                        wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                         pwps_info->wps_msg_timeout);

                break;
            }

        case WPS_Done:
            {
                if (pwps_info->role == WPS_REGISTRAR) {
                    msg_type = wps_get_message_type(peap);
                    status = wps_done_message_process(pwps_info,
                                                      (u8 *) peap,
                                                      peap->length);
                    if (status == WPS_STATUS_SUCCESS) {
                        wps_registrar_state_transition(pwps_info, msg_type);
                        msg_next =
                            wps_registrar_state_check(pwps_info, msg_type);
                        if (msg_next != WPS_EAP_UNKNOWN)
                            status = wps_msg_transmit(pwps_info,
                                                      msg_next,
                                                      pwps_info->
                                                      wps_msg_timeout);
                    } else
                        status = wps_msg_transmit(pwps_info,
                                                  WPS_MSG_NACK,
                                                  pwps_info->wps_msg_timeout);
                }
                break;
            }

        case WPS_Start:
            {
                msg_type = wps_get_message_type(peap);

                if ((msg_type > pwps_info->last_recv_wps_msg) ||
                    (msg_type == WPS_M2D) ||
                    ((pwps_info->last_recv_wps_msg == WPS_M2D) &&
                     (msg_type == WPS_M2))) {

                    wps_printf(DEBUG_WPS_STATE, "Last Message received 0x%x\n",
                               pwps_info->last_recv_wps_msg);
                    wps_printf(DEBUG_WPS_STATE, "Current message 0x%x\n",
                               msg_type);

                    if ((msg_type == pwps_info->last_recv_wps_msg)
                        && (msg_type != WPS_M2D)) {
                    /** recevied the retransmitted message
                     *  use the previous transmitted message, 
                     *  instead of current tx message
                     */
                        wps_printf(DEBUG_WPS_STATE,
                                   "Received reTx EAPOL Req message: 0x%x",
                                   msg_type);
                        memcpy(&pwps_info->last_message,
                               &pwps_info->last_tx_message,
                               sizeof(MESSAGE_BUFFER));
                    }

                    pwps_info->last_recv_wps_msg = msg_type;

                } else {
                    wps_printf(DEBUG_WPS_STATE, "rx_msg: Dropping unexpected msg! "
                           "(prev_msg: 0x%x, curr_msg: 0x%x)\n",
                           pwps_info->last_recv_wps_msg, msg_type);
                    break;
                }

                if (pwps_info->set_timer == WPS_SET) {
                    wps_cancel_timer(wps_txTimer_handler, pwps_info);
                    pwps_info->set_timer = WPS_CANCEL;
                    pwps_info->wps_msg_resent_count = 0;
                }

                status =
                    wps_eap_request_start_process(pwps_info, (u8 *) peap,
                                                  peap->length);
                if (status == WPS_STATUS_SUCCESS) {

                    wps_enrollee_state_transition(pwps_info, WPS_EAP_START);

                    msg_next =
                        wps_enrollee_state_check(pwps_info, WPS_EAP_START);
                    if (msg_next != WPS_EAP_UNKNOWN)
                        status =
                            wps_msg_transmit(pwps_info, msg_next,
                                             pwps_info->wps_msg_timeout);
                    else
                        status =
                            wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                             pwps_info->wps_msg_timeout);
                } else {
                    wps_printf(DEBUG_WPS_STATE, "Process WPS_Start Fail !");
                }

                break;
            }

        case WPS_Ack:
            break;

        case WPS_Nack:
            msg_type = wps_get_message_type(peap);

            status =
                wps_nack_message_process(pwps_info, (u8 *) peap, peap->length);
            if (status == WPS_STATUS_SUCCESS) {

                if (pwps_info->set_timer == WPS_SET) {
                    wps_cancel_timer(wps_txTimer_handler, pwps_info);
                    pwps_info->set_timer = WPS_CANCEL;
                }
                if (pwps_info->role == WPS_ENROLLEE
                    || (IS_DISCOVERY_ENROLLEE(pwps_info))
                    ) {
                    wps_enrollee_state_transition(pwps_info, msg_type);
                    msg_next = wps_enrollee_state_check(pwps_info, msg_type);
                } else {
                    wps_registrar_state_transition(pwps_info, msg_type);
                    msg_next = wps_registrar_state_check(pwps_info, msg_type);
                }
                if (msg_next != WPS_EAP_UNKNOWN)
                    status =
                        wps_msg_transmit(pwps_info, msg_next,
                                         pwps_info->wps_msg_timeout);
            } else
                status =
                    wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                     pwps_info->wps_msg_timeout);
            break;

        case WPS_Frag_Ack:
            {
                wps_send_next_fragment(pwps_info, wps_s);
                break;
            }
        default:
            break;
        }
    }

    LEAVE();
    return status;
}

/** 
 *  @brief Process Registrar state machine checking
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param msg_type     Message type to be checked
 *  @return             Message type to be sent in current state
 */
int
wps_registrar_state_check(PWPS_INFO pwps_info, u16 msg_type)
{
    int msg_next = WPS_EAP_UNKNOWN;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;

    ENTER();

    switch (msg_type) {

    case WPS_EAP_MSG_REQUEST_IDENTITY:
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if ((pwps_info->state == WPS_STATE_A) ||
                (pwps_info->state == WPS_STATE_B))
                msg_next = WPS_EAP_MSG_RESPONSE_IDENTITY;
        }
        break;

    case WPS_EAPOL_MSG_START:
        if (pwps_info->state == WPS_STATE_A)
            msg_next = WPS_EAP_MSG_REQUEST_IDENTITY;
        break;
    case WPS_EAP_MSG_RESPONSE_IDENTITY:
        if ((pwps_info->state == WPS_STATE_A) ||
            (pwps_info->state == WPS_STATE_B) ||
            (pwps_info->state == WPS_STATE_C))
            msg_next = WPS_EAP_START;
        break;
    case WPS_EAP_M1:
        if ((pwps_info->state == WPS_STATE_C) ||
            (pwps_info->state == WPS_STATE_D) ||
            (pwps_info->state == WPS_STATE_E)) {
#ifdef WFD_SUPPORT
            if (pwps_info->role != WFD_ROLE) {
#endif
                if ((pwps_info->registrar.updated_device_password_id !=
                     pwps_info->enrollee.device_password_id) ||
                    (pwps_info->pin_pbc_set == WPS_CANCEL)) {
                    wps_printf(DEBUG_WPS_STATE,
                               "Enrollee and Registrar Device password ID dont match.Registar %d. Enrollee %d\n",
                               pwps_info->registrar.updated_device_password_id,
                               pwps_info->enrollee.device_password_id);
                    wps_printf(DEBUG_WPS_STATE, "Pin PBC set value %d\n",
                               pwps_info->pin_pbc_set);
                    msg_next = WPS_EAP_M2D;
                } else
                    msg_next = WPS_EAP_M2;
#ifdef WFD_SUPPORT
            } else {
                if (pwps_info->pin_pbc_set == WPS_CANCEL) {

                    wps_printf(DEBUG_WPS_STATE, "Pin PBC set value %d",
                               pwps_info->pin_pbc_set);
                    msg_next = WPS_EAP_M2D;
                } else {
                    wps_printf(DEBUG_WPS_STATE,
                               "Ignoring Device password ID check for WFD\n");
                    wps_printf(DEBUG_WPS_STATE,
                               "Enrollee and Registrar Device password ID dont match.Registar %d. Enrollee %d\n",
                               pwps_info->registrar.updated_device_password_id,
                               pwps_info->enrollee.device_password_id);
                    msg_next = WPS_EAP_M2;
                }
            }
#endif
        }
        break;
    case WPS_EAP_M3:
        if ((pwps_info->state == WPS_STATE_E) ||
            (pwps_info->state == WPS_STATE_F))
            msg_next = WPS_EAP_M4;
        break;
    case WPS_EAP_M5:
        if ((pwps_info->state == WPS_STATE_E) ||
            (pwps_info->state == WPS_STATE_F))
            msg_next = WPS_EAP_M6;
        break;
    case WPS_EAP_M7:
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_E) {
                if (pwps_info->read_ap_config_only == WPS_SET) {
                    pwps_info->register_completed = WPS_SET;
                    msg_next = WPS_MSG_NACK;
                } else
                    msg_next = WPS_EAP_M8;
            }
        } else {
            if (pwps_info->state == WPS_STATE_F)
                msg_next = WPS_EAP_M8;
        }
        break;
    case WPS_MSG_DONE:
        if (wps_s->bss_type == BSS_TYPE_STA &&
            (pwps_info->role == WPS_REGISTRAR) &&
            pwps_info->state == WPS_STATE_E) {
            msg_next = WPS_MSG_ACK;
        } else if ((pwps_info->state == WPS_STATE_E) ||
                   (pwps_info->state == WPS_STATE_H) ||
                   (pwps_info->state == WPS_STATE_A))
            msg_next = WPS_EAP_MSG_FAIL;
        break;
    case WPS_MSG_ACK:
        if (pwps_info->state == WPS_STATE_D)
            msg_next = WPS_EAP_MSG_FAIL;
        break;
    case WPS_MSG_NACK:
        if ((wps_s->bss_type == BSS_TYPE_STA)
            && (pwps_info->role == WPS_REGISTRAR) &&
            pwps_info->state == WPS_STATE_H) {
            msg_next = WPS_MSG_NACK;
        } else if ((pwps_info->state == WPS_STATE_E) ||
                   (pwps_info->state == WPS_STATE_I) ||
                   (pwps_info->state == WPS_STATE_G))
            msg_next = WPS_EAP_MSG_FAIL;
        break;
    default:
        break;
    }                           /* end of swtich */

    wps_debug_print_msgtype("WPS Next Message", msg_next);

    LEAVE();
    return msg_next;
}

/** 
 *  @brief Process Registrar state machine transition
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param msg_type     Message type for triggering state transition
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_registrar_state_transition(PWPS_INFO pwps_info, u16 msg_type)
{
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;
    int status = WPS_STATUS_SUCCESS;

    ENTER();

    /* Current WPS State */
    wps_debug_print_state("State Transition Before", pwps_info->state);

    switch (msg_type) {
    case WPS_EAPOL_MSG_START:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : EAPOL_START\n");

        pwps_info->last_recv_wps_msg = -1;
        /* Reset Public keys and E-Hash, R-Hash */
        wps_reset_wps_state(pwps_info);
        break;
    case WPS_EAP_MSG_RESPONSE_IDENTITY:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : EAP_RESPONSE_IDENTITY\n");

        pwps_info->register_completed = WPS_CANCEL;
        pwps_info->registration_in_progress = WPS_SET;
        wps_printf(DEBUG_WPS_STATE, "Set registration in progress %d\n",
                   pwps_info->registration_in_progress);

        if (pwps_info->state == WPS_STATE_A) {
            pwps_info->state = WPS_STATE_B;
            wps_printf(DEBUG_WPS_STATE, "State A->B\n");
        }
        break;
    case WPS_EAP_START:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_START\n");
        if (pwps_info->state == WPS_STATE_B) {
            pwps_info->state = WPS_STATE_C;
            wps_printf(DEBUG_WPS_STATE, "State B->C\n");
        }
        break;
    case WPS_EAP_M1:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M1\n");

        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_B) {
                pwps_info->state = WPS_STATE_C;
                wps_printf(DEBUG_WPS_STATE, "State B->C\n");
            }
        } else if (pwps_info->state == WPS_STATE_C) {
            pwps_info->state = WPS_STATE_D;
            wps_printf(DEBUG_WPS_STATE, "State C->D\n");
        }
        break;
    case WPS_EAP_M2:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M2\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_C) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State C->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_D) {
            pwps_info->state = WPS_STATE_E;
            wps_printf(DEBUG_WPS_STATE, "State D->E\n");
        }
        break;
    case WPS_EAP_M2D:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M2D\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_C) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State C->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_D)
            pwps_info->state = WPS_STATE_E;
        break;
    case WPS_EAP_M3:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M3\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_E;
                wps_printf(DEBUG_WPS_STATE, "State D->E\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_F;
            wps_printf(DEBUG_WPS_STATE, "State E->F\n");
        }
        break;
    case WPS_EAP_M4:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M4\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State E->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_F) {
            pwps_info->state = WPS_STATE_E;
            wps_printf(DEBUG_WPS_STATE, "State F->E\n");
        }
        break;
    case WPS_EAP_M5:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M5\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_E;
                wps_printf(DEBUG_WPS_STATE, "State D->E\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_F;
            wps_printf(DEBUG_WPS_STATE, "State E->F\n");
        }
        break;
    case WPS_EAP_M6:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M6\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State E->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_F) {
            pwps_info->state = WPS_STATE_E;
            wps_printf(DEBUG_WPS_STATE, "State F->E\n");
        }
        break;
    case WPS_EAP_M7:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M7\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_E;
                wps_printf(DEBUG_WPS_STATE, "State D->E\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_F;
            wps_printf(DEBUG_WPS_STATE, "State E->F\n");
        }
        break;
    case WPS_EAP_M8:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_M8\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_D;
                wps_printf(DEBUG_WPS_STATE, "State E->D\n");
            }
        } else if (pwps_info->state == WPS_STATE_F) {
            pwps_info->state = WPS_STATE_E;
            wps_printf(DEBUG_WPS_STATE, "State F->E\n");
        }
        break;
    case WPS_MSG_DONE:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_DONE\n");
        pwps_info->register_completed = WPS_SET;
        pwps_info->registration_in_progress = WPS_CANCEL;
        wps_printf(DEBUG_WPS_STATE, "Set registration in progress %d\n",
                   pwps_info->registration_in_progress);

        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_E;
                wps_printf(DEBUG_WPS_STATE, "State D->E\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_H;
            wps_printf(DEBUG_WPS_STATE, "State E->H\n");
        }
        break;
    case WPS_MSG_ACK:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_ACK\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_G;
                wps_printf(DEBUG_WPS_STATE, "State E->G\n");
            }
        } else if (pwps_info->state == WPS_STATE_E) {
            pwps_info->state = WPS_STATE_D;
            wps_printf(DEBUG_WPS_STATE, "State E->D\n");
        }
        break;
    case WPS_MSG_NACK:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : WPS_NACK\n");
        if (wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->state == WPS_STATE_D) {
                pwps_info->state = WPS_STATE_H;
                wps_printf(DEBUG_WPS_STATE, "State D->H\n");
            } else if (pwps_info->state == WPS_STATE_E) {
                pwps_info->state = WPS_STATE_F;
                wps_printf(DEBUG_WPS_STATE, "State E->F\n");
            }
        } else {

            switch (pwps_info->state) {
            case WPS_STATE_F:
                pwps_info->state = WPS_STATE_I;
                wps_printf(DEBUG_WPS_STATE, "State F->I");
                break;
            case WPS_STATE_E:
                pwps_info->state = WPS_STATE_G;
                wps_printf(DEBUG_WPS_STATE, "State E->G");
                break;
			default:
            	pwps_info->state = WPS_STATE_A;
            	wps_printf(DEBUG_WPS_STATE, "State E->A (RCA)\n");
				break;
            }
        }
        break;
    case WPS_EAP_MSG_FAIL:
        wps_printf(DEBUG_WPS_STATE, "Event Trigger : EAP_FAIL\n");

        pwps_info->last_recv_wps_msg = -1;

        /* Reset Public keys and E-Hash, R-Hash */
        wps_reset_wps_state(pwps_info);

        switch (pwps_info->state) {
        case WPS_STATE_D:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State D->A\n");
        case WPS_STATE_G:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State G->A\n");
            break;
        case WPS_STATE_H:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State H->A\n");
            break;
        case WPS_STATE_I:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State I->A\n");
            break;
        case WPS_STATE_A:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State A->A\n");
            break;
        case WPS_STATE_F:
            pwps_info->state = WPS_STATE_A;
            wps_printf(DEBUG_WPS_STATE, "State F->A\n");
            break;
        default:
            break;
        }

        break;

    default:
        break;
    }                           /* end of switch */

    /* New WPS State */
    wps_debug_print_state("State Transition After", pwps_info->state);

    LEAVE();
    return status;
}

/** 
 *  @brief Maximum NACK error handler
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_maximum_nack_handler(PWPS_INFO pwps_info, WPS_DATA * wps_s)
{
    int status = WPS_STATUS_SUCCESS;
    ENTER();

    pwps_info->nack_error_count = 0;

    if (pwps_info->set_timer == WPS_SET) {
        wps_cancel_timer(wps_txTimer_handler, pwps_info);
        pwps_info->set_timer = WPS_CANCEL;
        pwps_info->wps_msg_resent_count = 0;
    }
    if (pwps_info->role == WPS_ENROLLEE) {

        /* clear IE buffer for registrar */
        /* shutdown the main processing loop */
        /* unregister user abort handler */
        if (wps_s->bss_type == BSS_TYPE_UAP &&
            pwps_info->registrar.version >= WPS_VERSION_2DOT0) {
            /* Clear session start IEs for Assoc response */
            wps_wlan_ie_config(CLEAR_AR_WPS_IE, &ap_assocresp_ie_index);
        }
        /* 
         * Send APCMD_SYS_CONFIGURE command to set WPS IE
         */
        status = wps_wlan_ie_config(SET_WPS_IE, &ie_index);
        if (status != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_WARNING, "Setting WPS IE failed!!!\n");
        }
        if (pwps_info->registrar.version >= WPS_VERSION_2DOT0) {
            status =
                wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                                   &ap_assocresp_ie_index);
            if (status != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
            }
        }
        pwps_info->input_state = WPS_INPUT_STATE_ROLE;
        pwps_info->pin_pbc_set = WPS_CANCEL;

        wps_print_uap_role_menu(pwps_info);

        LEAVE();
        return status;
    } else if (pwps_info->role == WPS_REGISTRAR) {

        wps_printf(DEBUG_WPS_STATE, "Cancelling registration timer!\n");
        wps_cancel_timer(wps_registration_time_handler, pwps_info);
        /**
         * Read uAP configuration for next connection
         */
        wps_load_uap_cred(wps_s, pwps_info);

        /* For UAP mode */
        wps_s->current_ssid.mode = IEEE80211_MODE_INFRA;
        pwps_info->mode = IEEE80211_MODE_INFRA;

        /* 
         * Send APCMD_SYS_CONFIGURE command to set WPS IE
         */
        status = wps_wlan_ie_config(SET_WPS_IE, &ie_index);
        if (status != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_WARNING, "Setting WPS IE failed!!!\n");
        }
        if (pwps_info->registrar.version >= WPS_VERSION_2DOT0) {
            status =
                wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                                   &ap_assocresp_ie_index);
            if (status != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
            }
        }
        pwps_info->input_state = WPS_INPUT_STATE_ROLE;
        pwps_info->pin_pbc_set = WPS_CANCEL;

        wps_print_uap_role_menu(pwps_info);

    }

    LEAVE();
    return status;
}

/** 
 *  @brief Registrar EAP packet process handling
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @param peap         A pointer to EAP frame header
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
static int
wps_eap_response_message_handler(PWPS_INFO pwps_info, PEAP_FRAME_HEADER peap)
{
    int status = WPS_STATUS_SUCCESS;
    u16 msg_next, msg_type = 0;

    ENTER();

    pwps_info->peer_id = peap->identifier + 1;

    if (peap->type == EAP_TYPE_IDENTITY) {
        wps_debug_print_msgtype("RX WPS Message Type",
                                WPS_EAP_MSG_RESPONSE_IDENTITY);
        if (pwps_info->set_timer == WPS_SET) {
            wps_cancel_timer(wps_txTimer_handler, pwps_info);
            pwps_info->set_timer = WPS_CANCEL;
            pwps_info->wps_msg_resent_count = 0;
        }

        status =
            wps_eap_response_identity_process(pwps_info, (u8 *) peap,
                                              peap->length);
        if (status == WPS_STATUS_SUCCESS) {
            msg_type = WPS_EAP_MSG_RESPONSE_IDENTITY;
            pwps_info->last_recv_wps_msg = msg_type;

            if (pwps_info->role == WPS_ENROLLEE
#if defined(WPSE_SUPPORT) || defined(WFD_SUPPORT)
                || (IS_DISCOVERY_ENROLLEE(pwps_info))
#endif
                ) {
                wps_enrollee_state_transition(pwps_info, msg_type);
                msg_next = wps_enrollee_state_check(pwps_info, msg_type);
            } else {
                wps_registrar_state_transition(pwps_info, msg_type);
                msg_next = wps_registrar_state_check(pwps_info, msg_type);
            }

            if (msg_next != WPS_EAP_UNKNOWN)
                status =
                    wps_msg_transmit(pwps_info, msg_next,
                                     pwps_info->wps_msg_timeout);
        }
    } else if (peap->type == EAP_TYPE_WPS) {
        switch (peap->op_code) {
        case WPS_Msg:

            msg_type = wps_get_message_type(peap);

            if ((msg_type > pwps_info->last_recv_wps_msg) ||
                (msg_type == WPS_M2D) ||
                ((pwps_info->last_recv_wps_msg == WPS_M2D) &&
                 (msg_type == WPS_M2))) {
                wps_printf(DEBUG_WPS_STATE, "Last Message received 0x%x\n",
                           pwps_info->last_recv_wps_msg);
                wps_printf(DEBUG_WPS_STATE, "Current message 0x%x\n", msg_type);
                if ((msg_type == pwps_info->last_recv_wps_msg)
                    && (msg_type != WPS_M2D)) {
                    /** recevied the retransmitted message
                     *  use the previous transmitted message for auth, 
                     *  instead of current tx message
                     */
                    wps_printf(DEBUG_WPS_STATE,
                               "Received reTx EAPOL Rsp message: 0x%x\n",
                               msg_type);
                    memcpy(&pwps_info->last_message,
                           &pwps_info->last_tx_message, sizeof(MESSAGE_BUFFER));
                }

                pwps_info->last_recv_wps_msg = msg_type;

            } else {
                wps_printf(MSG_INFO, "rx_msg: Dropping unexpected msg! "
                           "(prev_msg: 0x%x, curr_msg: 0x%x)\n",
                           pwps_info->last_recv_wps_msg, msg_type);
                break;
            }

            if (pwps_info->set_timer == WPS_SET) {
                wps_cancel_timer(wps_txTimer_handler, pwps_info);
                pwps_info->set_timer = WPS_CANCEL;
                pwps_info->wps_msg_resent_count = 0;
            }

            if (msg_type != WPS_EAP_UNKNOWN)
                status =
                    (*wps_msg_process[msg_type]) (pwps_info, (u8 *) peap,
                                                  peap->length);
            else
                status = WPS_STATUS_FAIL;

            if (status == WPS_STATUS_SUCCESS) {

                if (pwps_info->role == WPS_ENROLLEE
#if defined(WPSE_SUPPORT) || defined(WFD_SUPPORT)
                    || (IS_DISCOVERY_ENROLLEE(pwps_info))
#endif
                    ) {
                    wps_enrollee_state_transition(pwps_info, msg_type);
                    msg_next = wps_enrollee_state_check(pwps_info, msg_type);
                } else {
                    wps_registrar_state_transition(pwps_info, msg_type);
                    msg_next = wps_registrar_state_check(pwps_info, msg_type);
                }

                if (msg_next != WPS_EAP_UNKNOWN)
                    status =
                        wps_msg_transmit(pwps_info, msg_next,
                                         pwps_info->wps_msg_timeout);
                else
                    status =
                        wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                         pwps_info->wps_msg_timeout);
            } else
                status =
                    wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                     pwps_info->wps_msg_timeout);

            break;

        case WPS_Done:
            msg_type = wps_get_message_type(peap);

            status =
                wps_done_message_process(pwps_info, (u8 *) peap, peap->length);

            if (status == WPS_STATUS_SUCCESS) {

                wps_registrar_state_transition(pwps_info, msg_type);

                msg_next = wps_registrar_state_check(pwps_info, msg_type);

                if (msg_next != WPS_EAP_UNKNOWN)
                    status =
                        wps_msg_transmit(pwps_info, msg_next,
                                         pwps_info->wps_msg_timeout);
            } else
                status =
                    wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                     pwps_info->wps_msg_timeout);

            break;

        case WPS_Ack:
            msg_type = wps_get_message_type(peap);

            status =
                wps_ack_message_process(pwps_info, (u8 *) peap, peap->length);
            if (status == WPS_STATUS_SUCCESS) {

                if (pwps_info->role == WPS_ENROLLEE
#if defined(WPSE_SUPPORT) || defined(WFD_SUPPORT)
                    || (IS_DISCOVERY_ENROLLEE(pwps_info))
#endif
                    ) {
                    wps_enrollee_state_transition(pwps_info, msg_type);
                    msg_next = wps_enrollee_state_check(pwps_info, msg_type);
                } else {
                    wps_registrar_state_transition(pwps_info, msg_type);
                    msg_next = wps_registrar_state_check(pwps_info, msg_type);
                }

                if (msg_next != WPS_EAP_UNKNOWN)
                    status =
                        wps_msg_transmit(pwps_info, msg_next,
                                         pwps_info->wps_msg_timeout);
            } else
                status =
                    wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                     pwps_info->wps_msg_timeout);

            break;

        case WPS_Nack:
            msg_type = wps_get_message_type(peap);

            status =
                wps_nack_message_process(pwps_info, (u8 *) peap, peap->length);
            if (status == WPS_STATUS_SUCCESS) {

                pwps_info->nack_error_count++;

                if (pwps_info->set_timer == WPS_SET) {
                    wps_cancel_timer(wps_txTimer_handler, pwps_info);
                    pwps_info->set_timer = WPS_CANCEL;
                }
                if (pwps_info->role == WPS_ENROLLEE
#if defined(WPSE_SUPPORT) || defined(WFD_SUPPORT)
                    || (IS_DISCOVERY_ENROLLEE(pwps_info))
#endif
                    ) {
                    wps_enrollee_state_transition(pwps_info, msg_type);
                    msg_next = wps_enrollee_state_check(pwps_info, msg_type);
                } else {
                    wps_registrar_state_transition(pwps_info, msg_type);
                    msg_next = wps_registrar_state_check(pwps_info, msg_type);
                }
                if (msg_next != WPS_EAP_UNKNOWN)
                    status =
                        wps_msg_transmit(pwps_info, msg_next,
                                         pwps_info->wps_msg_timeout);
            } else
                status =
                    wps_msg_transmit(pwps_info, WPS_MSG_NACK,
                                     pwps_info->wps_msg_timeout);

            break;

        default:
            break;
        }
    }

    LEAVE();
    return status;
}

/********************************************************
        Global Functions
********************************************************/

#define EAP_REASSEMBLY_START(winfo, peap) do{   \
    winfo->frag_rx_in_prog=1;                   \
    winfo->frag_msg_tot_len=0;                  \
    winfo->frag_msg_cur_len=0;                  \
    winfo->frag_msg_tot_len=ntohs(*(u16*)(peap+1))+\
            sizeof(EAP_FRAME_HEADER);\
}while(0)

#define EAP_REASSEMBLY_END(winfo, peap_frag) do{\
    PEAP_FRAME_HEADER peap =                    \
          (PEAP_FRAME_HEADER)winfo->frag_buf;   \
    peap->identifier = peap_frag->identifier;   \
    peap->length = htons(winfo->frag_msg_tot_len);\
    peap->flags = 0;                            \
    winfo->frag_rx_in_prog=0;                   \
    winfo->frag_msg_tot_len=0;                  \
    winfo->frag_msg_cur_len=0;                  \
}while(0)

#define EAP_REASSEMBLE_FRAG(winfo, first, peapol, peap) do{     \
    int cpsize;                                                 \
    u8 *cpsrc;                                                  \
    if(first){                                                  \
        cpsrc = (u8*)peap;                                      \
        memcpy(&winfo->frag_buf[0], cpsrc, sizeof(EAP_FRAME_HEADER));\
        winfo->frag_msg_cur_len += sizeof(EAP_FRAME_HEADER);    \
        cpsize = ntohs(peapol->length)-sizeof(EAP_FRAME_HEADER)-sizeof(u16);\
        cpsrc += sizeof(EAP_FRAME_HEADER)+sizeof(u16);          \
    }else{                                                      \
        cpsize = ntohs(peapol->length)-sizeof(EAP_FRAME_HEADER);\
        cpsrc = (u8*)peap+sizeof(EAP_FRAME_HEADER);             \
    }                                                           \
    if((winfo->frag_msg_cur_len+cpsize)<=EAP_FRAG_BUF_SIZE){    \
        memcpy(&winfo->frag_buf[winfo->frag_msg_cur_len], cpsrc, cpsize);\
        winfo->frag_msg_cur_len += cpsize;                      \
    }else{                                                      \
        /* Overflow */                                          \
        status = WPS_STATUS_FAIL;                               \
    }                                                           \
}while(0);

#define EAP_PKT_NOT_FRAG    (1)
#define EAP_FRAG_PENDING    (2)
#define EAP_FRAG_COMPLETE   (3)

int
wps_reassemble_rx_frag(PWPS_INFO pwps_info, PEAPOL_FRAME_HEADER peapol)
{
    int status = WPS_STATUS_SUCCESS;
    int first_frag = 0;
    ENTER();
    PEAP_FRAME_HEADER peap = (PEAP_FRAME_HEADER)
        ((u8 *) peapol + sizeof(EAPOL_FRAME_HEADER));

    if (!pwps_info->frag_rx_in_prog) {
        /* This is the first fragment */
        first_frag = 1;
        if ((peap->flags & EAP_FLAGS_LEN_FIELD) && peap->length) {
            wps_printf(DEBUG_WPS_STATE, "Starting RX Reassembly\n");
            /* Length field is valid */
            EAP_REASSEMBLY_START(pwps_info, peap);
        } else {
            /* Invalid Length field, Ignore frag */
            status = WPS_STATUS_FAIL;
            goto done;
        }
    }

    EAP_REASSEMBLE_FRAG(pwps_info, first_frag, peapol, peap);

  done:
    LEAVE();
    return status;
}

int
wps_check_rx_frag(PWPS_INFO pwps_info, PEAPOL_FRAME_HEADER peapol)
{
    int ret;
    ENTER();
    PEAP_FRAME_HEADER peap_frag = (PEAP_FRAME_HEADER)
        ((u8 *) peapol + sizeof(EAPOL_FRAME_HEADER));

    if (!(peap_frag->flags & EAP_FLAGS_MORE_FRAG) &&
        !pwps_info->frag_rx_in_prog) {
        /* packet Not fragmented */
        ret = EAP_PKT_NOT_FRAG;
        goto done;
    }

    /* Reassemble Rxed Fragment */
    ret = wps_reassemble_rx_frag(pwps_info, peapol);

    if (ret == WPS_STATUS_FAIL) {
        goto done;
    }

    /* Current Fragment Accepted */
    if (pwps_info->frag_msg_tot_len == pwps_info->frag_msg_cur_len) {
        wps_printf(DEBUG_WPS_STATE, "RX Reassembly Complete\n");
        /* Reassembly complete */
        EAP_REASSEMBLY_END(pwps_info, peap_frag);
        ret = EAP_FRAG_COMPLETE;
    } else {
        /* some more Fragments Expected */
        wps_printf(DEBUG_WPS_STATE, "Sending FRAG_ACK\n");

        ret = wps_msg_transmit(pwps_info,
                               WPS_MSG_FRAG_ACK, WPS_MSG_LONG_TIMEOUT);
        ret = EAP_FRAG_PENDING;
    }

  done:
    LEAVE();
    return ret;
}

/** 
 *  @brief EAP frame handling function
 *  
 *  @param buffer       A pointer to frame received buffer
 *  @param src_addr     A pointer to source address of received frame
 *  @return             None
 */
void
wps_message_handler(u8 * buffer, u8 * src_addr)
{
    PEAPOL_FRAME_HEADER peapol;
    PEAP_FRAME_HEADER peap;
    PWPS_INFO pwps_info;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;
    int status = WPS_STATUS_SUCCESS;
    int msg_next;

    ENTER();

    pwps_info = wps_get_private_info();

    peapol = (PEAPOL_FRAME_HEADER) buffer;
    switch (peapol->packet_type) {
    case EAP_PACKET:
        {
            peap = (PEAP_FRAME_HEADER) (buffer + sizeof(EAPOL_FRAME_HEADER));

            // XXX: Move this out of here
            pwps_info->peer_id = peap->identifier;

            if (peap->type == EAP_TYPE_WPS) {
                status = wps_check_rx_frag(pwps_info, peapol);

                if ((status == EAP_FRAG_PENDING) || (status == WPS_STATUS_FAIL)) {
                    goto done;
                } else if (status == EAP_FRAG_COMPLETE) {
                    peap = (PEAP_FRAME_HEADER) pwps_info->frag_buf;
                    wps_hexdump(DEBUG_WPS_MSG, "Reass pkt", (u8 *) peap,
                                ntohs(peap->length));
                }
            }
            switch (peap->code) {
            case EAP_REQUEST:
                wps_printf(DEBUG_WPS_STATE, "EAP_REQUEST received\n");

                if (wps_s->bss_type == BSS_TYPE_UAP) {
                    if ((pwps_info->role == WPS_REGISTRAR &&
                         memcmp(pwps_info->enrollee.mac_address, src_addr,
                                ETH_ALEN)) || (pwps_info->role == WPS_ENROLLEE
                                               && memcmp(pwps_info->registrar.
                                                         mac_address, src_addr,
                                                         ETH_ALEN))) {
                        wps_printf(MSG_WARNING,
                                   "EAP_REQUEST received from invalid source "
                                   MACSTR " dropped!\n", MAC2STR(src_addr));
                        break;
                    }
                }

                wps_eap_request_message_handler(pwps_info, peap);
                break;

            case EAP_RESPONSE:
                wps_printf(DEBUG_WPS_STATE, "EAP_RESPONSE received\n");

                if (wps_s->bss_type == BSS_TYPE_UAP) {
                    if ((pwps_info->role == WPS_REGISTRAR &&
                         memcmp(pwps_info->enrollee.mac_address, src_addr,
                                ETH_ALEN)) || (pwps_info->role == WPS_ENROLLEE
                                               && memcmp(pwps_info->registrar.
                                                         mac_address, src_addr,
                                                         ETH_ALEN))) {
                        wps_printf(MSG_WARNING,
                                   "EAP_RESPONSE received from invalid source "
                                   MACSTR " dropped!\n", MAC2STR(src_addr));
                        break;
                    }
                }

                wps_eap_response_message_handler(pwps_info, peap);
                break;

            case EAP_FAILURE:
                {
                    wps_printf(DEBUG_WPS_STATE, "EAP_FAILURE received\n");
                    wps_printf(DEBUG_WPS_STATE, "RX WPS Message Type: EAP_FAIL\n");

                    if (pwps_info->set_timer == WPS_SET) {
                        wps_cancel_timer(wps_txTimer_handler, pwps_info);
                        pwps_info->set_timer = WPS_CANCEL;
                        pwps_info->wps_msg_resent_count = 0;
                    }

                    if (pwps_info->role == WPS_ENROLLEE
                        || (IS_DISCOVERY_ENROLLEE(pwps_info))
                        ) {
                        wps_enrollee_state_transition(pwps_info,
                                                      WPS_EAP_MSG_FAIL);
                    } else {
                        wps_registrar_state_transition(pwps_info,
                                                       WPS_EAP_MSG_FAIL);
                    }

                    if (pwps_info->invalid_credential == WPS_SET) {
                        if (pwps_info->role == WPS_ENROLLEE
                            || IS_DISCOVERY_ENROLLEE(pwps_info)
                            ) {
                            if (wps_s->bss_type == BSS_TYPE_UAP &&
                                pwps_info->registrar.version >=
                                WPS_VERSION_2DOT0) {
                                /* Clear session start IEs for Assoc response */
                                wps_wlan_ie_config(CLEAR_AR_WPS_IE,
                                                   &ap_assocresp_ie_index);
                            }

                            /* clear IE buffer for registrar */
                            /* shutdown the main processing loop */
                            /* unregister user abort handler */
                            wps_registrar_shutdown();
                        }

                    }
                    if ((pwps_info->state == WPS_STATE_A &&
                        pwps_info->register_completed == WPS_SET) ||
                        (pwps_info->wps_non_advertise == WPS_SET))
                        {

                        /* 
                         * Registration complete with M8 received.
                         */
                        if (pwps_info->role == WPS_ENROLLEE
                            || (IS_DISCOVERY_ENROLLEE(pwps_info))
                            ) {
                            /* WPS handshake is complete, clear the Enrollee
                               IE's */

                            /* Clear WPS IE */
                            if (ie_index != -1) {
                                wps_printf(DEBUG_WLAN, "Clearing WSC IE's !\n");
                                wps_wlan_ie_config(CLEAR_WPS_IE, &ie_index);
                                ie_index = -1;
                            }
                        }

                        if (pwps_info->role == WPS_REGISTRAR) {
                            /* Reset Enrollee Mac address to ensure check for
                               multiple EAPOL-START succeeds in EAPOL-STRAT
                               handling */
                            memset(&pwps_info->enrollee.mac_address, 0,
                                   ETH_ALEN);
                        }

                        /* WARNING: This code has been moved to wps_eap_M8_frame_process
                         *          so that we will accept the security info even if
                         *          we don't get the EAP_FAIL.  When porting, make sure
                         *          we keep it tied to M8, not EAP_FAIL. */ 
                        #if 0
                        //status = wps_write_to_config_file(pwps_info);
                        wps_write_to_config_file(pwps_info);
                        #endif

                        if (IS_DISCOVERY_ENROLLEE(pwps_info)) {
                            wfd_update_persistent_record(pwps_info);
                        }

                        /* message exchange completed */
                        if (pwps_info->state == WPS_STATE_A &&
                            pwps_info->register_completed == WPS_SET) {
                            wps_printf(MSG_INFO, "\n");
                            wps_printf
                                (MSG_INFO, "WPS Registration Protocol Completed Successfully !\n\n\n");
                        } else
                        {
                            wps_printf(MSG_INFO, "WPS Registration Protocol EAP_FAIL !\n\n");
                            pwps_info->registration_fail = WPS_SET;
                        }

                        /* Cancel WPS Registration Timer */

                        wps_printf(DEBUG_WPS_STATE,
                                   "Cancelling registration timer!\n");
                        wps_cancel_timer(wps_registration_time_handler,
                                         pwps_info);

                        if (wps_s->bss_type == BSS_TYPE_UAP &&
                            pwps_info->registrar.version >= WPS_VERSION_2DOT0) {
                            /* Clear session start IEs for Assoc response */
                            wps_wlan_ie_config(CLEAR_AR_WPS_IE,
                                               &ap_assocresp_ie_index);
                        }
                        wps_registrar_shutdown();

                    } else {
                        u8 bssid_get[ETH_ALEN];
                        int retry_cnt = 50;
                        static u8 eap_fail_count = 0;
                        /* 
                         * Registration complete without M8 received especially in
                         * case of external Registrar.
                         * Restart registration protocol.
                         */

                        if (pwps_info->set_timer == WPS_SET) {
                            wps_cancel_timer(wps_txTimer_handler, pwps_info);
                            pwps_info->set_timer = WPS_CANCEL;
                            pwps_info->wps_msg_resent_count = 0;
                        }
                        eap_fail_count++;
                        if (eap_fail_count >= MAX_EAP_FAIL_COUNT) {
                            eap_fail_count = 0;
                            if (wps_s->current_ssid.mode ==
                                IEEE80211_MODE_INFRA)
                                status = wps_wlan_set_deauth();
                            do {
                                memset(bssid_get, 0x00, ETH_ALEN);
                                wps_wlan_get_wap(bssid_get);
                                if ((memcmp
                                     (bssid_get, wps_s->current_ssid.bssid,
                                      ETH_ALEN) != 0)) {
                                    wps_printf(DEBUG_WPS_STATE,
                                               "\nRe-connecting to AP ..... \n");

                                    /* Enable driver WPS session checking */
                                    wps_wlan_session_control(WPS_SESSION_ON);

                                    wps_wlan_set_wap((u8 *) wps_s->current_ssid.
                                                     bssid);

                                    retry_cnt--;

                                    /* wait for interface up */
                                    tx_thread_sleep(1);
                                } else {

                                    /* Disable driver WPS session checking */
                                    wps_wlan_session_control(WPS_SESSION_OFF);

                                    break;
                                }
                            } while (retry_cnt != 0);
                        }
                        /* 
                         * Start to exchange WPS Message by sending EAPOL_START packet
                         */

                        // Set to zero to force a reconnect later
                        memset(bssid_get, 0x00, ETH_ALEN);
                        wps_wlan_set_wap(bssid_get);

                        msg_next =
                            wps_enrollee_state_check(pwps_info,
                                                     WPS_EAP_MSG_FAIL);
                        pwps_info->eap_msg_sent = WPS_EAPOL_MSG_START;
                        pwps_info->state = WPS_STATE_A;
                        status =
                            wps_msg_transmit(pwps_info, WPS_EAPOL_MSG_START,
                                             WPS_MSG_LONG_TIMEOUT);
                    }
                    break;
                }               /* EAP_FAILURE */

            case EAP_SUCCESS:
                wps_printf(DEBUG_WPS_STATE, "EAP_SUCCESS received\n");
                break;

            default:
                wps_printf(MSG_ERROR, "Unknown Code received\n");
                break;
            }
            break;
        }                       /* EAP_PACKET */

    case EAPOL_START:
        {
            wps_printf(DEBUG_WPS_STATE, "EAPOL_START received\n");

            if (pwps_info->registration_in_progress == WPS_SET) {

                if (wps_s->bss_type == BSS_TYPE_UAP &&
                    pwps_info->role == WPS_ENROLLEE) {
                    if (memcmp
                        (pwps_info->registrar.mac_address, src_addr,
                         ETH_ALEN)) {

                        wps_printf(MSG_WARNING,
                                   "EAPOL_START received from " MACSTR
                                   " while session is in progress with " MACSTR,
                                   MAC2STR(src_addr),
                                   MAC2STR(pwps_info->registrar.mac_address),
								   "\n");
                        wps_printf(MSG_WARNING, "EAPOL_START dropped!!\n");
                        return;
                    }
                } else {
                    if (memcmp
                        (pwps_info->enrollee.mac_address, src_addr, ETH_ALEN)) {

                        wps_printf(MSG_WARNING,
                                   "EAPOL_START received from " MACSTR
                                   " while session is in progress with " MACSTR,
                                   MAC2STR(src_addr),
                                   MAC2STR(pwps_info->enrollee.mac_address),
								   "\n");
                        wps_printf(MSG_WARNING, "EAPOL_START dropped!!\n");
                        return;
                    }
                }
            }
            if (pwps_info->role == WPS_REGISTRAR) {
                /* Get Enrollee Peer Address */
                memcpy(pwps_info->enrollee.mac_address, src_addr, ETH_ALEN);
                wps_printf(DEBUG_WLAN, "Peer Address Get: ");
                wps_hexdump(DEBUG_WLAN, "pwps_info->enrollee.mac_address",
                            pwps_info->enrollee.mac_address, ETH_ALEN);

                wps_eapol_start_handler(pwps_info);
                wps_eap_request_identity_prepare(pwps_info);

                wps_printf(DEBUG_WPS_STATE,
                           "eap_msg_sent = WPS_EAP_MSG_REQUEST_IDENTITY\n");
                pwps_info->eap_msg_sent = WPS_EAP_MSG_REQUEST_IDENTITY;
            } else if (pwps_info->role == WPS_ENROLLEE &&
                       (wps_s->bss_type == BSS_TYPE_UAP)) {
                /* Get Registrar Peer Address */
                memcpy(pwps_info->registrar.mac_address, src_addr, ETH_ALEN);
                wps_printf(DEBUG_WLAN, "Peer Address Get: ");
                wps_hexdump(DEBUG_WLAN, "pwps_info->registrar.mac_address",
                            pwps_info->registrar.mac_address, 6);

                wps_eapol_start_handler(pwps_info);
                wps_eap_request_identity_prepare(pwps_info);

                wps_printf(DEBUG_WPS_STATE,
                           "eap_msg_sent = WPS_EAP_MSG_REQUEST_IDENTITY\n");
                pwps_info->eap_msg_sent = WPS_EAP_MSG_REQUEST_IDENTITY;
            }
            if (IS_DISCOVERY_REGISTRAR(pwps_info)) {

                /* Get Enrollee Peer Address */
                memcpy(pwps_info->enrollee.mac_address, src_addr, ETH_ALEN);
                wps_printf(DEBUG_WLAN, "Peer Address Get: ");
                wps_hexdump(DEBUG_WLAN, "pwps_info->enrollee.mac_address",
                            pwps_info->enrollee.mac_address, 6);

                wps_eapol_start_handler(pwps_info);
                wps_eap_request_identity_prepare(pwps_info);

                wps_printf(DEBUG_WPS_STATE,
                           "eap_msg_sent = WPS_EAP_MSG_REQUEST_IDENTITY\n");
                pwps_info->eap_msg_sent = WPS_EAP_MSG_REQUEST_IDENTITY;
            }

            break;
        }                       /* EAPOL_START */

    default:
        wps_printf(MSG_ERROR, "Unknown Packet Type %d\n", peapol->packet_type);
        wps_hexdump(MSG_ERROR, "Packet Dump", buffer,
                    sizeof(EAPOL_FRAME_HEADER) + ntohs(peapol->length));
        break;
    }

  done:
    LEAVE();
}

/** 
 *  @brief WPS EAP frame transmit function
 *  
 *  @param pwps_info    A pointer to PWPS_INFO structure
 *  @param msg_type     message type
 *  @param timeout      timeout
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_msg_transmit(PWPS_INFO pwps_info, u16 msg_type, u32 timeout)
{
    int status = WPS_STATUS_SUCCESS;
    u32 sec, usec;
    WPS_DATA *wps_s = &wps_global;

    ENTER();

    wps_debug_print_msgtype("TX WPS Message Type", msg_type);

    status = (*wps_msg_prepare[msg_type]) (pwps_info);

    if (status == WPS_STATUS_SUCCESS) {
        pwps_info->eap_msg_sent = msg_type;

        if (pwps_info->role == WPS_ENROLLEE) {
            wps_enrollee_state_transition(pwps_info, msg_type);
        } else if (pwps_info->role == WPS_REGISTRAR) {
            wps_registrar_state_transition(pwps_info, msg_type);
        } else if (pwps_info->role == WFD_ROLE) {
            if (pwps_info->discovery_role == WPS_REGISTRAR) {
                wps_registrar_state_transition(pwps_info, msg_type);
            } else {
                wps_enrollee_state_transition(pwps_info, msg_type);
            }
        }
    }

    if (msg_type != WPS_EAP_MSG_FAIL) {
        sec = timeout / 1000;
        usec = (timeout % 1000) * 1000;
        wps_start_timer(sec, usec, wps_txTimer_handler, pwps_info);
        pwps_info->set_timer = WPS_SET;
        pwps_info->wps_msg_resent_count++;
    } else {
        wps_eap_fail_transmit_handler(pwps_info, wps_s);
    }

    LEAVE();
    return status;
}

/** 
 *  @brief WPS EAP fail frame transmit handler
 *  
 *  @param pwps_info    A pointer to PWPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_eap_fail_transmit_handler(PWPS_INFO pwps_info, WPS_DATA * wps_s)
{

    int status = WPS_STATUS_SUCCESS;
    u8 *sta_mac_addr = NULL;
    ENTER();
    /* registrar send out EAP-FAIL */
    /* WPS process complete */

    pwps_info->registration_in_progress = WPS_CANCEL;
    wps_printf(DEBUG_WPS_STATE, "Set registration in progress %d\n",
               pwps_info->registration_in_progress);

    if (pwps_info->register_completed != WPS_SET
        && pwps_info->restart_by_M2D == WPS_SET
        && pwps_info->pin_generator == WPS_ENROLLEE
        && pwps_info->registrar.device_password_id == DEVICE_PASSWORD_ID_PIN) {

        if (pwps_info->role_switched == WPS_SET) {
            // This hack is required for Win7 station client which switches
            // roles from STA-Registrar to STA-Enrollee
            pwps_info->role = WPS_REGISTRAR;
            /* Read Device info from config file & generate UUID */
            wps_update_device_info(pwps_info, wps_s, pwps_info->role);
        } else {
#if 0
            if (!
                (pwps_info->role == WPS_ENROLLEE &&
                 wps_s->bss_type == BSS_TYPE_UAP)) {
                if (!wps_non_interactive)
                    printf("Please Input 8-digit Numeric PIN :");
                    // Ask user interface for pin ???
            }
#endif
        }
    }
    if (pwps_info->set_timer == WPS_SET) {
        wps_cancel_timer(wps_txTimer_handler, pwps_info);
        pwps_info->set_timer = WPS_CANCEL;
        pwps_info->wps_msg_resent_count = 0;
    }
    if (wps_s->bss_type == BSS_TYPE_UAP) {
        if (pwps_info->role == WPS_REGISTRAR
            || (IS_DISCOVERY_REGISTRAR(pwps_info))
            )
            sta_mac_addr = pwps_info->enrollee.mac_address;
        else
            sta_mac_addr = pwps_info->registrar.mac_address;
        if (sta_mac_addr != NULL) {
            wps_printf(DEBUG_WPS_STATE,
                       "Delay sending DEAUTH by 200 milli-seconds!");
            tx_thread_sleep(20);
            wps_hexdump(DEBUG_WPS_STATE, "Deauth Mac Address", sta_mac_addr,
                        ETH_ALEN);
            status = apcmd_station_deauth(wps_s, sta_mac_addr);
        }
    }
    status = wps_uap_session_complete_handler(pwps_info, wps_s);
    status = wps_wfd_session_complete_handler(pwps_info, wps_s);

    LEAVE();
    return status;
}

/** 
 *  @brief WPS uAP session complete handler
 *  
 *  @param pwps_info    A pointer to PWPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_uap_session_complete_handler(PWPS_INFO pwps_info, WPS_DATA * wps_s)
{
    int status = WPS_STATUS_SUCCESS;

    ENTER();

	LEAVE();
	
	return status;
}

/** 
 *  @brief WPS WFD session complete handler
 *  
 *  @param pwps_info    A pointer to PWPS_INFO structure
 *  @param wps_s        A pointer to WPS_DATA structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_wfd_session_complete_handler(PWPS_INFO pwps_info, WPS_DATA * wps_s)
{
    int ret = WPS_STATUS_SUCCESS;

    ENTER();

    if (pwps_info->role == WFD_ROLE &&
        pwps_info->discovery_role == WPS_REGISTRAR) {

        if (pwps_info->register_completed == WPS_SET) {
            wps_printf(DEBUG_WPS_STATE,
                "WPS Registration Protocol Completed Successfully !\n\n\n");
            pwps_info->wps_device_state = SC_CONFIGURED_STATE;
            wps_printf(DEBUG_WPS_STATE, "Device state SC_CONFIGURED_STATE !\n");
        }
        /* 
         * Update Group formation bit for registrar 
         * send the command on station interface 
         */
        if (switch_intf && !auto_go) {
            wps_intf_deinit(&wps_global);
            strncpy(wps_global.ifname, "mlan0", IFNAMSIZ + 1);
            wps_loop_init(&wps_global, WPS_REGISTRAR);
        }

        if (!auto_go) {
            if (wps_wlan_update_group_formation_config(WPS_CANCEL) !=
                WPS_STATUS_SUCCESS) {
                wps_printf(DEBUG_WFD_DISCOVERY,
                           "Failed to reset Group formation bit.\n");
            }
        }

        /* Cancel WPS Registration Timer */
        wps_printf(DEBUG_WPS_STATE, "Cancelling registration timer!\n");
        wps_cancel_timer(wps_registration_time_handler, pwps_info);
        pwps_info->wps_session_active = WPS_CANCEL;
        if (!auto_go) {
            /* 
             * Note, code below under !RESTART_SM_SUPPORT is mutually 
             * exclusive to this code.
             */
            /* shutdown the main processing loop */
            wps_main_loop_shutdown();

            /* Unregister user abort handler */
            wps_unregister_rdsock_handler(STDIN_FILENO);
        } else {
            wps_load_uap_cred(wps_s, pwps_info);

            wps_s->current_ssid.mode = IEEE80211_MODE_INFRA;
            pwps_info->mode = IEEE80211_MODE_INFRA;

            pwps_info->input_state = WPS_INPUT_STATE_METHOD;
            pwps_info->pin_pbc_set = WPS_CANCEL;

            if (wps_wlan_remove_password_ie_config() != WPS_STATUS_SUCCESS) {
                wps_printf(DEBUG_WLAN,
                           "Failed to remove device password ID from WPS IE.\n");
            }

            wps_printf(DEBUG_WLAN, "Auto GO waiting for next connection ...\n");
			
			//wps_print_registration method_menu(pwps_info);
		}
    }
    /* Auto-GO registrar with role = 2 */
    else if (pwps_info->role == WPS_REGISTRAR) {
        /* Cancel WPS Registration Timer */
        wps_printf(DEBUG_WPS_STATE, "Cancelling registration timer!\n");
        wps_cancel_timer(wps_registration_time_handler, pwps_info);
        pwps_info->wps_session_active = WPS_CANCEL;

        /* shutdown the main processing loop */
        wps_main_loop_shutdown();

        /* Unregister user abort handler */
        wps_unregister_rdsock_handler(STDIN_FILENO);
    }

    LEAVE();
    return ret;
}

#define AP_CONNNECT_RETRY_CNT   (10)

/** 
 *  @brief Check WLAN Link Status & Reconnect if disconnected
 *  
 *  @param wps_s        A pointer to PWPS_DATA structure
 *  @param pwps_info    A pointer to PWPS_INFO structure
 *  @param reconnected  A pointer to variable to indicate if STA re-connected
 *  @return             1-connected, 0-not connected
 */

int
wps_sta_check_link_active(WPS_DATA * wps_s,
                          PWPS_INFO pwps_info, int *reconnected)
{
    int link_active = 0;
    u8 retry_count = AP_CONNNECT_RETRY_CNT;
    u8 bssid_get[ETH_ALEN];

    *reconnected = 0;

    memset(bssid_get, 0x00, ETH_ALEN);
    wps_wlan_get_wap(bssid_get);

    if ((memcmp(bssid_get, wps_s->current_ssid.bssid, ETH_ALEN) == 0)) {
        /* Link not lost */
        link_active = 1;
        return link_active;
    }

    *reconnected = 1;

    /* Enable driver WPS session checking */
    wps_wlan_session_control(WPS_SESSION_ON);

    do {
        wps_printf(DEBUG_WPS_STATE, "\nConnection lost, try to re-connect to AP ..... \n");

        if (wps_wlan_set_wap((u8 *) wps_s->current_ssid.bssid)) {
            printf("Re-Connect to AP Failed\n");
        }

        retry_count--;

        /* wait for interface up */
        tx_thread_sleep(10);

        memset(bssid_get, 0x00, ETH_ALEN);
        wps_wlan_get_wap(bssid_get);

        if ((memcmp(bssid_get, wps_s->current_ssid.bssid, ETH_ALEN) == 0)) {
            link_active = 1;
            break;
        }

    } while (retry_count != 0);

    /* Disable driver WPS session checking */
    wps_wlan_session_control(WPS_SESSION_OFF);

    return link_active;
}

/** 
 *  @brief Re-Transmit the last TX'ed packet
 *  
 *  @param pwps_info    A pointer to PWPS_INFO structure
 *  @return             WPS_STATUS_SUCCESS--retransmit success, otherwise--fail
 */

int
retransmit_last_tx_pkt(PWPS_INFO pwps_info)
{
    PEAP_FRAME_HEADER peap_wps_frame = (PEAP_FRAME_HEADER) pwps_info->buffer;
    int pkt_len = ntohs(peap_wps_frame->length);
    int ret;

    wps_debug_print_msgtype("ReTX WPS Message Type", pwps_info->eap_msg_sent);

    if (pwps_info->eap_msg_sent != WPS_EAPOL_MSG_START) {
        ret = wps_eapol_txPacket((u8 *) pwps_info->buffer, pkt_len);

        wps_start_timer(WPS_MSG_DEFAULT_TIMEOUT / 1000, 0,
                        wps_txTimer_handler, pwps_info);
        pwps_info->set_timer = WPS_SET;
        pwps_info->wps_msg_resent_count++;
    } else {
        ret = wps_msg_transmit(pwps_info, WPS_EAPOL_MSG_START,
                               WPS_MSG_DEFAULT_TIMEOUT);
    }

    return ret;
}

/** 
 *  @brief WPS frame transmit time-out handler
 *  
 *  @param user_data    User private data
 *  @return             None
 */
void
wps_txTimer_handler(void *user_data)
{
    PWPS_INFO pwps_info;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;
    int status = WPS_STATUS_SUCCESS;
    int reconnected = 0, link_active = 0;
    u8 zero_bssid[ETH_ALEN] = { 0 };

    ENTER();

    pwps_info = (PWPS_INFO) user_data;

    if (pwps_info->set_timer == WPS_SET) {
        wps_cancel_timer(wps_txTimer_handler, pwps_info);
        pwps_info->set_timer = WPS_CANCEL;
    }
    if (wps_s->bss_type == BSS_TYPE_UAP
        && (pwps_info->wps_msg_resent_count >= pwps_info->wps_msg_max_retry)) {

        switch (pwps_info->role) {
        case WPS_ENROLLEE:
            status = WPS_STATUS_FAIL;
            break;
        case WPS_REGISTRAR:
            if (pwps_info->eap_msg_sent == WPS_MSG_NACK)
                status = WPS_STATUS_FAIL;
            else
                status = WPS_STATUS_SUCCESS;
            break;
        }
        goto done;
    }
    /* 
     * Some AP/External Registrar will disconnect after EAP-FAIL
     * packet sent without M2D being sent before that.
     * So, polling link status of AP to restart registration here.
     */
    if (wps_s->bss_type == BSS_TYPE_STA) {

        if (pwps_info->wps_msg_resent_count >= pwps_info->wps_msg_max_retry) {
            /* Maximum retry reached, disconnect & start all over again */
            if (wps_wlan_set_wap((u8 *) zero_bssid)) {
                wps_printf(MSG_INFO, "tx_timer: Disconnect from AP Failed\n");
            }
            tx_thread_sleep(25);
        }

        link_active = wps_sta_check_link_active(wps_s, pwps_info, &reconnected);

        if (!link_active) {
            wps_printf(MSG_INFO, "\n\nRe-Connect to AP FAIL reached MAX retry count!!!\n\n");
            status = WPS_STATUS_FAIL;
            goto done;
        }

        if (reconnected)
            pwps_info->restart_link_lost = WPS_SET;

        if (pwps_info->restart_link_lost || pwps_info->restart_by_M2D) {
            pwps_info->restart_link_lost = WPS_CANCEL;
            pwps_info->restart_by_M2D = WPS_CANCEL;

            pwps_info->state = WPS_STATE_A;
            pwps_info->id = 1;

            /* Reset Public keys and E-Hash, R-Hash */
            wps_reset_wps_state(pwps_info);

            pwps_info->last_recv_wps_msg = -1;
            pwps_info->wps_msg_resent_count = 0;
            pwps_info->eap_msg_sent = WPS_EAPOL_MSG_START;
            status = wps_msg_transmit(pwps_info, WPS_EAPOL_MSG_START,
                                      WPS_MSG_DEFAULT_TIMEOUT);

            goto done;
        }
    }
    retransmit_last_tx_pkt(pwps_info);

  done:
    if (status != WPS_STATUS_SUCCESS) {
        /* Cancel WPS Registration Timer */
        wps_printf(DEBUG_WPS_STATE, "Cancelling registration timer!\n");
        wps_cancel_timer(wps_registration_time_handler, pwps_info);
        pwps_info->wps_session_active = WPS_CANCEL;

        if (pwps_info->set_timer == WPS_SET) {
            wps_cancel_timer(wps_txTimer_handler, pwps_info);
            pwps_info->set_timer = WPS_CANCEL;
            pwps_info->wps_msg_resent_count = 0;
        }

        pwps_info->registration_fail = WPS_SET;
        pwps_info->registration_in_progress = WPS_CANCEL;
        wps_printf(DEBUG_WPS_STATE, "Set registration in progress %d\n",
                   pwps_info->registration_in_progress);

        wps_printf(DEBUG_WPS_STATE, "State *->A");
        pwps_info->state = WPS_STATE_A;
        /* Reset Public keys and E-Hash, R-Hash */
        wps_reset_wps_state(pwps_info);

        wps_registrar_shutdown();
    }

    LEAVE();
    return;
}

/** 
 *  @brief Start WPS registration timer
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
void
wps_start_registration_timer(PWPS_INFO pwps_info)
{
    u32 sec, usec;

    /* Start a timer for WPS registration protocol timeout */
    wps_printf(DEBUG_WALK_TIME, "Start Timer for WPS Registration ... \n");
    sec = WPS_REGISTRATION_TIME;
    usec = 0;
    wps_start_timer(sec, usec, wps_registration_time_handler, pwps_info);

    return;

}

/** 
 *  @brief Start WPS state machine
 *  
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int
wps_state_machine_start(PWPS_INFO pwps_info)
{
    int status = WPS_STATUS_SUCCESS;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;

    if (((pwps_info->role == WPS_ENROLLEE || pwps_info->role == WPS_REGISTRAR)
         && wps_s->bss_type == BSS_TYPE_STA)
        || (IS_DISCOVERY_ENROLLEE(pwps_info))
        ) {
        /* 
         * Start to exchange WPS Message by sending EAPOL_START packet
         */
        pwps_info->eap_msg_sent = WPS_EAPOL_MSG_START;
        status =
            wps_msg_transmit(pwps_info, WPS_EAPOL_MSG_START,
                             WPS_MSG_DEFAULT_TIMEOUT);
    }

    return status;
}

/** 
 *  @brief Time-out handler of WSC AP Setup locked. R-Hash failure count will
 *  be reset to 0 of this timer is called.
 *  
 *  @param user_data    A pointer to user data of timer functions
 *  @return             None
 */
void
wps_r_hash_failure_count_timer_handler(void *user_data)
{

    PWPS_INFO pwps_info;
    ENTER();

    pwps_info = (PWPS_INFO) user_data;

    /* Cancel Hash failure counter Timer */
    wps_cancel_timer(wps_r_hash_failure_count_timer_handler, pwps_info);

    wps_printf(DEBUG_WALK_TIME, "WSC R-Hash Counter Timeout !\n");
    pwps_info->wps_r_hash_failure_count = 0;
    LEAVE();
}

/** 
 *  @brief Time-out handler of WSC AP Setup locked. R-Hash failure count will
 *  be reset to 0 of this timer is called.
 *  
 *  @param user_data    A pointer to user data of timer functions
 *  @return             None
 */
void
wps_ap_setup_locked_timer_handler(void *user_data)
{

    PWPS_INFO pwps_info;
    int ret;
    ENTER();

    pwps_info = (PWPS_INFO) user_data;

    /* Cancel AP Setup Locked Timer */
    wps_cancel_timer(wps_ap_setup_locked_timer_handler, pwps_info);

    wps_printf(DEBUG_WALK_TIME, "WSC AP Setup Locked Timeout !\n");
    pwps_info->wps_ap_setup_locked = WPS_CANCEL;
    if (pwps_info->wps_session_active == WPS_SET
        && pwps_info->auto_enrollee_in_progress == WPS_CANCEL) {
        ret = wps_wlan_ie_config(SET_WPS_AP_SESSION_ACTIVE_IE, &ie_index);
        if (ret != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_ERROR, "WPS Registrar failed to config\n");
        }
        if (pwps_info->enrollee.version >= WPS_VERSION_2DOT0) {
            ret =
                wps_wlan_ie_config(SET_WPS_AP_SESSION_ACTIVE_AR_IE,
                                   &ap_assocresp_ie_index);
            if (ret != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
            }
        }
    } else {

        ret = wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_IE, &ie_index);
        if (ret != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_ERROR, "WPS Registrar failed to config\n");
        }
        if (pwps_info->enrollee.version >= WPS_VERSION_2DOT0) {
            ret =
                wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                                   &ap_assocresp_ie_index);
            if (ret != WPS_STATUS_SUCCESS) {
                wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
            }
        }
    }
    LEAVE();
}

/** 
 *  @brief Time-out handler of WPS Registration (300 seconds), program will
 *          exit if registration not completed within this time.
 *  
 *  @param user_data    A pointer to user data of timer functions
 *  @return             None
 */
void
wps_registration_time_handler(void *user_data)
{
    PWPS_INFO pwps_info;
    WPS_DATA *wps_s = (WPS_DATA *) & wps_global;
    int ret = WPS_STATUS_SUCCESS;
    ENTER();

    pwps_info = (PWPS_INFO) user_data;

    pwps_info->wps_session_active = WPS_CANCEL;
    pwps_info->auto_enrollee_in_progress = WPS_CANCEL;

    wps_printf(DEBUG_WALK_TIME, "WPS Registration Timeout !\n");

    /* Cancel WPS Registration Timer */
    wps_printf(DEBUG_WPS_STATE, "Cancelling registration timer!\n");
    wps_cancel_timer(wps_registration_time_handler, pwps_info);

    /* Cancel packet Tx timer */
    if (pwps_info->set_timer == WPS_SET) {
        wps_cancel_timer(wps_txTimer_handler, pwps_info);
        pwps_info->set_timer = WPS_CANCEL;
        pwps_info->wps_msg_resent_count = 0;
    }

    if ((wps_s->bss_type == BSS_TYPE_STA && pwps_info->role == WPS_ENROLLEE)
        || (IS_DISCOVERY_ENROLLEE(pwps_info)
            || (pwps_info->role == WPS_ADHOC_EXTENTION &&
                pwps_info->discovery_role == WPSE_TEST_SMPBC)
        )
    ) {

        wps_printf(MSG_INFO, "\n");
        wps_printf(MSG_INFO, "WPS registration timer time-out.\n");
        wps_printf(MSG_INFO, "WPS Registration Protocol Not Completed !\n\n");

        /* shutdown the main processing loop */
        wps_main_loop_shutdown();
    } else if ((wps_s->bss_type == BSS_TYPE_UAP) &&
               (pwps_info->role == WPS_REGISTRAR ||
                pwps_info->role == WPS_ENROLLEE)) {

        wps_printf(MSG_INFO, "WPS Registration timeout!!\n");
    	wps_printf(MSG_INFO, "Restarting WPS Registration.\n");
        /* Shutdown main loop will cause registrar restart */
        pwps_info->input_state = WPS_INPUT_STATE_ROLE;
        pwps_info->pin_pbc_set = WPS_CANCEL;

        wps_print_uap_role_menu(pwps_info);

        ret = wps_wlan_ie_config(SET_WPS_IE, &ie_index);
        if (ret != WPS_STATUS_SUCCESS) {
            wps_printf(MSG_WARNING, "Setting WPS IE failed!!!\n");
        }
        if (pwps_info->role == WPS_REGISTRAR) {
            if (pwps_info->registrar.version >= WPS_VERSION_2DOT0) {
                ret =
                    wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                                       &ap_assocresp_ie_index);
                if (ret != WPS_STATUS_SUCCESS) {
                    wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
                }
            }
            /* shutdown the main processing loop */
            wps_main_loop_shutdown();
        } else if (pwps_info->role == WPS_ENROLLEE) {
            if (pwps_info->enrollee.version >= WPS_VERSION_2DOT0) {
                ret =
                    wps_wlan_ie_config(SET_WPS_AP_SESSION_INACTIVE_AR_IE,
                                       &ap_assocresp_ie_index);
                if (ret != WPS_STATUS_SUCCESS) {
                    wps_printf(MSG_WARNING, "WPS IE configuration failure.\n");
                }
            }
        }

    } else if (pwps_info->role == WFD_ROLE) {
        if (pwps_info->discovery_role != WPS_REGISTRAR) {
            wps_printf(MSG_INFO, "\n");
            wps_printf(MSG_INFO, "WPS registration timer time-out.\n");
            wps_printf(MSG_INFO, "WPS Registration Protocol Not Completed !\n\n");
        }
        /* Clear WPS IE */
        if (ie_index != -1) {
            wps_wlan_ie_config(CLEAR_WPS_IE, &ie_index);
            ie_index = -1;
        }

        /* shutdown the main processing loop */
        wps_main_loop_shutdown();
    }

    LEAVE();
}

/** 
 *  @brief Cause a user abort to occur without a registered file
 *         descriptor.
 *  
 *  @return             None
 */
void abort_wps(void)
{
    wps_user_abort_handler(-1, wps_get_private_info());
}

/** 
 *  @brief PBC Session Overlap Error handler of WPS Registration 
 *  @param user_data    A pointer to user data of timer functions
 *  @return             None
 */
void
wps_registration_pbc_overlap_handler(void *user_data)
{
    ENTER();

    LEAVE();
    return;
}

/** 
 *  @brief EAPOL-Start handler of WPS Registration 
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             None
 */
void
wps_eapol_start_handler(WPS_INFO * pwps_info)
{
    ENTER();

    wps_printf(MSG_INFO,
               "EAPOL-Start received. Aborting EAPOL State machine...\n");
    /* Cancel packet Tx timer */
    if (pwps_info->set_timer == WPS_SET) {
        wps_cancel_timer(wps_txTimer_handler, pwps_info);
        pwps_info->set_timer = WPS_CANCEL;
        pwps_info->wps_msg_resent_count = 0;
    }
    wps_printf(DEBUG_WPS_STATE, "State *->A\n");
    pwps_info->state = WPS_STATE_A;
    /* Reset Public keys and E-Hash, R-Hash */
    wps_reset_wps_state(pwps_info);
    LEAVE();
}

/** 
 *  @brief EAPOL-Start handler of WPS Registration 
 *  @param pwps_info    A pointer to WPS_INFO structure
 *  @return             None
 */
void
wps_reset_wps_state(WPS_INFO * pwps_info)
{

    ENTER();
    wps_printf(MSG_INFO, "Resetting E-HASH, R-HASH and public keys\n");
    memset(pwps_info->enrollee.e_hash1, 0, 32);
    memset(pwps_info->enrollee.e_hash2, 0, 32);
    memset(pwps_info->enrollee.r_hash1, 0, 32);
    memset(pwps_info->enrollee.r_hash2, 0, 32);
    memset(pwps_info->enrollee.public_key, 0, 192);
    memset(pwps_info->registrar.public_key, 0, 192);
    if (pwps_info->last_message.length) {
        memset(pwps_info->last_message.message, 0,
               pwps_info->last_message.length);
    }
    pwps_info->last_message.length = 0;
    pwps_info->last_recv_wps_msg = 0;
    LEAVE();
}
