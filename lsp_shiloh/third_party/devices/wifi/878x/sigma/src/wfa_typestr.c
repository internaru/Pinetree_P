
/****************************************************************************
 *  (c) Copyright 2007 Wi-Fi Alliance.  All Rights Reserved
 *
 *
 *  LICENSE
 *
 *  License is granted only to Wi-Fi Alliance members and designated
 *  contractors ($B!H(BAuthorized Licensees$B!I(B)..AN  Authorized Licensees are granted
 *  the non-exclusive, worldwide, limited right to use, copy, import, export
 *  and distribute this software:
 *  (i) solely for noncommercial applications and solely for testing Wi-Fi
 *  equipment; and
 *  (ii) solely for the purpose of embedding the software into Authorized
 *  Licensee$B!G(Bs proprietary equipment and software products for distribution to
 *  its customers under a license with at least the same restrictions as
 *  contained in this License, including, without limitation, the disclaimer of
 *  warranty and limitation of liability, below..AN  The distribution rights
 *  granted in clause
 *  (ii), above, include distribution to third party companies who will
 *  redistribute the Authorized Licensee$B!G(Bs product to their customers with or
 *  without such third party$B!G(Bs private label. Other than expressly granted
 *  herein, this License is not transferable or sublicensable, and it does not
 *  extend to and may not be used with non-Wi-Fi applications..AN  Wi-Fi Alliance
 *  reserves all rights not expressly granted herein..AN 
 *.AN 
 *  Except as specifically set forth above, commercial derivative works of
 *  this software or applications that use the Wi-Fi scripts generated by this
 *  software are NOT AUTHORIZED without specific prior written permission from
 *  Wi-Fi Alliance.
 *.AN 
 *  Non-Commercial derivative works of this software for internal use are
 *  authorized and are limited by the same restrictions; provided, however,
 *  that the Authorized Licensee shall provide Wi-Fi Alliance with a copy of
 *  such derivative works under a perpetual, payment-free license to use,
 *  modify, and distribute such derivative works for purposes of testing Wi-Fi
 *  equipment.
 *.AN 
 *  Neither the name of the author nor "Wi-Fi Alliance" may be used to endorse
 *  or promote products that are derived from or that use this software without
 *  specific prior written permission from Wi-Fi Alliance.
 *
 *  THIS SOFTWARE IS PROVIDED BY WI-FI ALLIANCE "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A.AN PARTICULAR PURPOSE,
 *  ARE DISCLAIMED. IN NO EVENT SHALL WI-FI ALLIANCE BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, THE COST OF PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE) ARISING IN ANY WAY OUT OF
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. ******************************************************************************
 */
/*
 *  wfa_typestr.c:
 *  global array of the mapping of command types, command strings 
 *  to attached processing function
 *
 *     Revision History:
 *       2006/03/10 -- initially created by qhu
 *       2006/06/01 -- BETA release by qhu
 *       2006/06/13 -- 00.02 release by qhu
 *       2006/06/30 -- 00.10 Release by qhu
 *       2006/07/10 -- 01.00 Release by qhu
 *       2006/09/01 -- 01.05 Release by qhu
 *       2007/03/30 -- 01.40 WPA2 and Official WMM Beta Release by qhu
 *       2007/04/20 -- 02.00 WPA2 and Official WMM Release by qhu
 *       2007/08/15 --  02.10 WMM-Power Save release by qhu
 *       2007/10/10 --  02.20 Voice SOHO beta -- qhu
 *       2007/11/07 --  02.30 Voice HSO -- qhu
 *       2007/12/10 --  02.32 Add a function to upload test results.
 */
#include <stdio.h>
#include <pthread.h>
#include "wfa_types.h"
#include "wfa_tlv.h"
#include "wfa_tg.h"
#include "wfa_cmds.h"
#include "wfa_agtctrl.h"

extern int cmdProcNotDefinedYet(char *, BYTE *, int *);
extern int xcCmdProcGetVersion(char *, BYTE *, int *);
extern int xcCmdProcAgentConfig(char *, BYTE *, int *);
extern int xcCmdProcAgentSend(char *, BYTE *, int *);
extern int xcCmdProcAgentRecvStart(char *, BYTE *, int *);
extern int xcCmdProcAgentRecvStop(char *, BYTE *, int *);
extern int xcCmdProcAgentReset(char *, BYTE *, int *);
extern int xcCmdProcStaGetIpConfig(char *, BYTE *, int *);
extern int xcCmdProcStaSetIpConfig(char *, BYTE *, int *);
extern int xcCmdProcStaGetMacAddress(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaIsConnected(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaVerifyIpConnection(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaGetBSSID(char *pcmdStr, BYTE *, int *); 
extern int xcCmdProcStaGetStats(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetEncryption(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetPSK(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetEapTLS(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetEapTTLS(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetEapSIM(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetPEAP(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetIBSS(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcDeviceGetInfo(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcDeviceListIF(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaAssociate(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetUAPSD(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaGetInfo(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcAgentSendPing(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcAgentStopPing(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaDebugSet(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaUpload(char *pcmdStr, BYTE *, int *);
extern int xcCmdProcStaSetMode(char *pcmStr, BYTE *, int *);
extern int xcCmdProcStaSetWMM(char *pcmStr, BYTE *, int *);
extern int xcCmdProcStaPresetTestParameters(char *pcmStr, BYTE *, int *);
extern int xcCmdProcStaSetEapFAST(char *pcmStr, BYTE *, int *);
extern int xcCmdProcStaSetEapAKA(char *pcmStr, BYTE *, int *);
extern int xcCmdProcStaSetSystime(char *pcmStr, BYTE *, int *);

extern int xcCmdProcStaSet11n(char *pcmStr, BYTE *, int *);
extern int xcCmdProcStaSetWireless(char *pcmStr, BYTE *, int *);
extern int xcCmdProcStaSendADDBA(char *pcmStr, BYTE *, int *);
extern int xcCmdProcStaSetRIFS(char *, BYTE *, int *);
extern int xcCmdProcStaSendCoExistMGMT(char *, BYTE *, int *);
extern int xcCmdProcStaResetDefault(char *, BYTE *, int *);
extern int xcCmdProcStaDisconnect(char *, BYTE *, int *);

extern int xcCmdProcStaReAssociate(char *pcmStr, BYTE*, int *);
extern int xcCmdProcStaResetDefault(char *pcmdStr, BYTE *aBuf, int *aLen);
extern int xcCmdProcStaSetPwrSave(char *pcmdStr, BYTE *aBuf, int *aLen);


/*
 * Initialize a command name table to its defined type and process function
 */
typeNameStr_t nameStr[] =
{
   {0,                   "NO_USED_STRING", NULL},
   {WFA_GET_VERSION_TLV, "ca_get_version", xcCmdProcGetVersion},
   {WFA_TRAFFIC_SEND_PING_TLV, "traffic_send_ping", xcCmdProcAgentSendPing},
   {WFA_TRAFFIC_STOP_PING_TLV, "traffic_stop_ping", xcCmdProcAgentStopPing},
   {WFA_TRAFFIC_AGENT_CONFIG_TLV, "traffic_agent_config", xcCmdProcAgentConfig},
   {WFA_TRAFFIC_AGENT_SEND_TLV, "traffic_agent_send", xcCmdProcAgentSend},
   {WFA_TRAFFIC_AGENT_RESET_TLV, "traffic_agent_reset", xcCmdProcAgentReset},
   {WFA_TRAFFIC_AGENT_RECV_START_TLV, "traffic_agent_receive_start", xcCmdProcAgentRecvStart}, 
   {WFA_TRAFFIC_AGENT_RECV_STOP_TLV, "traffic_agent_receive_stop", xcCmdProcAgentRecvStop}, 
   /* Control Commands */
   {WFA_STA_GET_IP_CONFIG_TLV, "sta_get_ip_config", xcCmdProcStaGetIpConfig},
   {WFA_STA_SET_IP_CONFIG_TLV, "sta_set_ip_config", xcCmdProcStaSetIpConfig},
   {WFA_STA_GET_MAC_ADDRESS_TLV, "sta_get_mac_address", xcCmdProcStaGetMacAddress},
   {WFA_STA_IS_CONNECTED_TLV, "sta_is_connected", xcCmdProcStaIsConnected},
   {WFA_STA_VERIFY_IP_CONNECTION_TLV, "sta_verify_ip_connection", xcCmdProcStaVerifyIpConnection},
   {WFA_STA_GET_BSSID_TLV, "sta_get_bssid", xcCmdProcStaGetBSSID},
   {WFA_STA_GET_STATS_TLV, "sta_get_stats", xcCmdProcStaGetStats},
   {WFA_STA_SET_ENCRYPTION_TLV, "sta_set_encryption", xcCmdProcStaSetEncryption},
   {WFA_STA_SET_PSK_TLV, "sta_set_psk", xcCmdProcStaSetPSK},
   {WFA_STA_SET_EAPTLS_TLV, "sta_set_eaptls", xcCmdProcStaSetEapTLS},
   {WFA_STA_SET_EAPTTLS_TLV, "sta_set_eapttls", xcCmdProcStaSetEapTTLS},
   {WFA_STA_SET_EAPSIM_TLV, "sta_set_eapsim", xcCmdProcStaSetEapSIM},
   {WFA_STA_SET_PEAP_TLV, "sta_set_peap", xcCmdProcStaSetPEAP},
   {WFA_STA_SET_IBSS_TLV, "sta_set_ibss", xcCmdProcStaSetIBSS},
   {WFA_STA_ASSOCIATE_TLV, "sta_associate", xcCmdProcStaAssociate},
   {WFA_DEVICE_LIST_IF_TLV, "device_list_interfaces", xcCmdProcDeviceListIF}, 
   {WFA_DEVICE_GET_INFO_TLV, "device_get_info", xcCmdProcDeviceGetInfo},
   {WFA_STA_GET_INFO_TLV, "sta_get_info", xcCmdProcStaGetInfo},
   {WFA_STA_SET_MODE_TLV, "sta_set_mode", xcCmdProcStaSetMode},
   {WFA_STA_UPLOAD_TLV, "sta_up_load", xcCmdProcStaUpload},
   {WFA_STA_DEBUG_SET_TLV, "sta_debug_set", xcCmdProcStaDebugSet},
   {WFA_STA_SET_UAPSD_TLV, "sta_set_uapsd", xcCmdProcStaSetUAPSD},
   {WFA_STA_SET_WMM_TLV, "sta_set_wmm", xcCmdProcStaSetWMM},
   {WFA_STA_DISCONNECT_TLV, "sta_disconnect", xcCmdProcStaDisconnect},
   {WFA_STA_REASSOCIATE_TLV, "sta_reassociate", xcCmdProcStaReAssociate},     
#ifdef WFA_STA_TB
   {WFA_STA_PRESET_PARAMETERS_TLV, "sta_preset_testparameters", xcCmdProcStaPresetTestParameters},
#endif
   {WFA_STA_SET_EAPFAST_TLV, "sta_set_eapfast", xcCmdProcStaSetEapFAST},
   {WFA_STA_SET_EAPAKA_TLV, "sta_set_eapaka", xcCmdProcStaSetEapAKA},
   {WFA_STA_SET_SYSTIME_TLV, "sta_set_systime", xcCmdProcStaSetSystime},
   {WFA_STA_SET_PWRSAVE_TLV, "sta_set_pwrsave", xcCmdProcStaSetPwrSave},
#ifdef WFA_STA_TB 
   {WFA_STA_RESET_DEFAULT_TLV, "sta_reset_default", xcCmdProcStaResetDefault},
   {WFA_STA_SET_11N_TLV, "sta_set_11n", xcCmdProcStaSet11n},
   {WFA_STA_SET_WIRELESS_TLV, "sta_set_wireless", xcCmdProcStaSetWireless},
   {WFA_STA_SEND_ADDBA_TLV, "sta_send_addba", xcCmdProcStaSendADDBA},
   {WFA_STA_SET_RIFS_TEST_TLV, "sta_set_rifs_test", xcCmdProcStaSetRIFS},
   {WFA_STA_SEND_COEXIST_MGMT_TLV, "sta_send_coexist_mgmt", xcCmdProcStaSendCoExistMGMT},
#endif
   {-1, "", NULL},
};
