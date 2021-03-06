
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
 *       File: wfa_ca_resp.c
 *       All functions are desginated to handle the command responses from
 *       a DUT and inform TM the command status.
 *       They will be called by Control Agent.
 * 
 *       Revision History:
 *
 */
#include <stdlib.h>
#include <string.h>
#include "wfa_portall.h"
#include "wfa_debug.h"
#include "wfa_types.h"
#include "wfa_main.h"
#include "wfa_tlv.h"
#include "wfa_tg.h"
#include "wfa_miscs.h"
#include "wfa_ca.h"
#include "wfa_rsp.h"
#include "wfa_sock.h"
#include "wfa_ca_resp.h"

extern unsigned short wfa_defined_debug;
char gRespStr[WFA_BUFF_512];

dutCommandRespFuncPtr wfaCmdRespProcFuncTbl[(WFA_STA_RESPONSE_END - WFA_STA_COMMANDS_END)+(WFA_STA_NEW_COMMANDS_RESPONSE_END - WFA_STA_NEW_COMMANDS_RESPONSE_START) + 1] =
{
    caCmdNotDefinedYet,
    wfaGetVersionResp,                   /* WFA_GET_VERSION_RESP_TLV - WFA_STA_COMMANDS_END                  (1) */
    wfaTrafficAgentPingStartResp,        /* WFA_TRAFFIC_SEND_PING_RESP_TLV - WFA_STA_COMMANDS_END            (2) */
    wfaTrafficAgentPingStopResp,         /* WFA_TRAFFIC_STOP_PING_RESP_TLV - WFA_STA_COMMANDS_END            (3) */
    wfaTrafficAgentConfigResp,           /* WFA_TRAFFIC_AGENT_CONFIG_RESP_TLV - WFA_STA_COMMANDS_END         (4) */
    wfaTrafficAgentSendResp,             /* WFA_TRAFFIC_AGENT_SEND_RESP_TLV - WFA_STA_COMMANDS_END           (5) */
    wfaStaGenericResp,                   /* WFA_TRAFFIC_AGENT_RECV_START_RESP_TLV - WFA_STA_COMMANDS_END     (6) */ 
    wfaTrafficAgentRecvStopResp,         /* WFA_TRAFFIC_AGENT_RECV_STOP_RESP_TLV - WFA_STA_COMMANDS_END      (7) */
    wfaStaGenericResp,                   /* WFA_TRAFFIC_AGENT_RESET_RESP_TLV - WFA_STA_COMMANDS_END          (8) */
    caCmdNotDefinedYet,                  /* WFA_TRAFFIC_AGENT_STATUS_RESP_TLV - WFA_STA_COMMANDS_END         (9) */

    wfaStaGetIpConfigResp,               /* WFA_STA_GET_IP_CONFIG_RESP_TLV - WFA_STA_COMMANDS_END           (10) */
    wfaStaGenericResp,                   /* WFA_STA_SET_IP_CONFIG_RESP_TLV - WFA_STA_COMMANDS_END           (11) */
    wfaStaGetMacAddressResp,             /* WFA_STA_GET_MAC_ADDRESS_RESP_TLV - WFA_STA_COMMANDS_END         (12) */
    caCmdNotDefinedYet,                  /* WFA_STA_SET_MAC_ADDRESS_RESP_TLV - WFA_STA_COMMANDS_END         (13) */
    wfaStaIsConnectedResp,               /* WFA_STA_IS_CONNECTED_RESP_TLV - WFA_STA_COMMANDS_END            (14) */
    wfaStaVerifyIpConnectResp,           /* WFA_STA_VERIFY_IP_CONNECTION_RESP_TLV - WFA_STA_COMMANDS_END    (15) */
    wfaStaGetBSSIDResp,                  /* WFA_STA_GET_BSSID_RESP_TLV - WFA_STA_COMMANDS_END               (16) */
    wfaStaGetStatsResp,                  /* WFA_STA_GET_STATS_RESP_TLV - WFA_STA_COMMANDS_END               (17) */
    wfaStaSetEncryptionResp,             /* WFA_STA_SET_ENCRYPTION_RESP_TLV - WFA_STA_COMMANDS_END          (18) */
    wfaStaGenericResp,                   /* WFA_STA_SET_PSK_RESP_TLV - WFA_STA_COMMANDS_END                 (19) */
    wfaStaGenericResp,                   /* WFA_STA_SET_EAPTLS_RESP_TLV - WFA_STA_COMMANDS_END              (20) */
    wfaStaGenericResp,                   /* WFA_STA_SET_UAPSD_RESP_TLV - WFA_STA_COMMANDS_END               (21) */
    wfaStaGenericResp,                   /* WFA_STA_ASSOCIATE_RESP_TLV - WFA_STA_COMMANDS_END               (22) */
    wfaStaGenericResp,                   /* WFA_STA_SET_EAPTTLS_RESP_TLV - WFA_STA_COMMANDS_END             (23) */
    wfaStaGenericResp,                   /* WFA_STA_SET_EAPSIM_RESP_TLV - WFA_STA_COMMANDS_END              (24) */
    wfaStaGenericResp,                   /* WFA_STA_SET_PEAP_RESP_TLV - WFA_STA_COMMANDS_END                (25) */
    wfaStaGenericResp,                   /* WFA_STA_SET_IBSS_RESP_TLV - WFA_STA_COMMANDS_END                (26) */
    wfaStaGetInfoResp,                   /* WFA_STA_GET_INFO_RESP_TLV - WFA_STA_COMMANDS_END                (27) */
    wfaDeviceGetInfoResp,                /* WFA_DEVICE_GET_INFO_RESP_TLV - WFA_STA_COMMANDS_END             (28) */
    wfaDeviceListIFResp,                 /* WFA_DEVICE_LIST_IF_RESP_TLV - WFA_STA_COMMANDS_END              (29) */
    wfaStaGenericResp,                   /* WFA_STA_DEBUG_SET_RESP_TLV - WFA_STA_COMMANDS_END               (30) */
    wfaStaGenericResp,                   /* WFA_STA_SET_MODE_RESP_TLV - WFA_STA_COMMANDS_END                (31) */
    wfaStaUploadResp,                    /* WFA_STA_UPLOAD_RESP_TLV - WFA_STA_COMMANDS_END                  (32) */
    wfaStaGenericResp,                   /* WFA_STA_SET_WMM_RESP_TLV - WFA_STA_COMMANDS_END                 (33) */
    wfaStaGenericResp,                   /* WFA_STA_REASSOCIATE_RESP_TLV - WFA_STA_COMMANDS_END             (34) */
    wfaStaGenericResp,                   /* WFA_STA_SET_PWRSAVE_RESP_TLV - WFA_STA_CMMANDS_END              (35) */
    wfaStaGenericResp,                   /* WFA_STA_SEND_NEIGREQ_RESP_TLV - WFA_STA_COMMANDS_END            (36) */
    wfaStaGenericResp,			         /* (37) */
    wfaStaGenericResp,                   /* (38)*/
    wfaStaGenericResp,                   /* (39)*/
    wfaStaGenericResp,                   /* (40)*/
    wfaStaGenericResp,                   /* (41)*/
    wfaStaGenericResp,                   /* (42)*/
    wfaStaGenericResp,                   /* (43)*/
    wfaStaGenericResp,                   /* (44)*/
    wfaStaGenericResp,                   /* (45)*/
    wfaStaGenericResp,                   /* (46)*/
    wfaStaGenericResp,                   /* (47)*/
    wfaStaGenericResp,                   /* (48)*/
};

extern int gSock, gCaSockfd;

int caCmdNotDefinedYet(BYTE *cmdBuf)
{
    int done;

    wSPRINTF(gRespStr, "status,ERROR,Command Not Defined\r\n");
    /* make sure if getting send error, will close the socket */
    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    done = 0;

    return done;
}

int wfaStaVerifyIpConnectResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *verifyResp = (dutCmdResponse_t *)(cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaStaVerifyIpConnectResp\n");
    switch(verifyResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaVerifyConnect running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,connected,%d\r\n", verifyResp->cmdru.connected);
        DPRINT_INFO(WFA_OUT, "%s", gRespStr);
        break;

        case STATUS_ERROR:
        wSPRINTF(gRespStr, "status,ERROR\r\n");
        DPRINT_INFO(WFA_OUT, "%s", gRespStr);

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
    }
    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

int wfaStaIsConnectedResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *connectedResp = (dutCmdResponse_t *)(cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaStaIsConnectedResp ...\n");
    switch(connectedResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaIsConnectd running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,connected,%d\r\n", connectedResp->cmdru.connected);
        break;

        case STATUS_ERROR:
        wSPRINTF(gRespStr, "status,ERROR\r\n");
        break;
        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));
    return done;
}

int wfaStaGetIpConfigResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *getIpConfigResp = (dutCmdResponse_t *) (cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaStaGetIpConfigResp ...\n");
    switch(getIpConfigResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaGetIpConfig running ...\n");
        done = 1;
        break;

        case STATUS_ERROR:
        wSPRINTF(gRespStr, "status,ERROR\r\n");
        break;

        case STATUS_COMPLETE:
        if(wSTRLEN(getIpConfigResp->cmdru.getIfconfig.dns[0]) == 0)
                *getIpConfigResp->cmdru.getIfconfig.dns[0] = '\0';
        if(wSTRLEN(getIpConfigResp->cmdru.getIfconfig.dns[1]) == 0)
                *getIpConfigResp->cmdru.getIfconfig.dns[1] = '\0';

        wSPRINTF(gRespStr, "status,COMPLETE,dhcp,%d,ip,%s,mask,%s,primary-dns,%s,secondary-dns,%s\r\n", getIpConfigResp->cmdru.getIfconfig.isDhcp,
                      getIpConfigResp->cmdru.getIfconfig.ipaddr,
                      getIpConfigResp->cmdru.getIfconfig.mask,
                      getIpConfigResp->cmdru.getIfconfig.dns[0],
                      getIpConfigResp->cmdru.getIfconfig.dns[1]);
        break;

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));
    return done;
}

int wfaGetVersionResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *getverResp =(dutCmdResponse_t *)(cmdBuf + 4);

    switch(getverResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaGetVersion running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,version,%s\r\n", getverResp->cmdru.version);
        break;
        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));
    return done ;
}

int wfaStaGetInfoResp(BYTE *cmdBuf)
{
    dutCmdResponse_t *infoResp = (dutCmdResponse_t *)(cmdBuf + 4);
    int done = 0;

    switch(infoResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaGetInfo running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,%s\r\n", infoResp->cmdru.info);
        DPRINT_INFO(WFA_OUT, "info: %s\n", infoResp->cmdru.info);
        break;

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

int wfaTrafficAgentConfigResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *agtConfigResp = (dutCmdResponse_t *)(cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaTrafficAgentConfigResp ...\n");
    switch(agtConfigResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaTrafficAgentConfig running ...\n");
        done = 1;
        break;
        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,streamID,%d\r\n", agtConfigResp->streamId);
        break;
        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));
    return done;
}

int wfaTrafficAgentSendResp(BYTE *cmdBuf)
{
    int done=1,i;
    char copyBuf[64];
    int errorStatus = 0;
    wfaTLV *ptlv = (wfaTLV *)cmdBuf;
    int len = ptlv->len;
    int numStreams;
    dutCmdResponse_t *statResp = (dutCmdResponse_t *)(cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaTrafficAgentSendResp ...\n");

    numStreams = (len/sizeof(dutCmdResponse_t));
    wPRINTF("total %i streams\n", numStreams);
    for(i=0; i<numStreams; i++) 
    {
   	if(statResp->status != STATUS_COMPLETE) 
        {
   	    errorStatus = 1;
        }
    }

    if(errorStatus) 
    {
   	wSPRINTF(gRespStr, "status,ERROR");
    } 
    else 
    {
   	wSPRINTF(gRespStr, "status,COMPLETE,streamID,");
   	for(i=0; i<numStreams; i++) 
        {
            wSPRINTF(copyBuf, " %d", statResp[i].streamId);
     	    wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
   	}

        wPRINTF("streamids %s\n", gRespStr);

   	wSTRNCAT(gRespStr, ",txFrames,", 10);
   	for(i=0; i<numStreams; i++) 
        {
   	    wSPRINTF(copyBuf, "%d ", statResp[i].cmdru.stats.txFrames);
   	    wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
   	}

      	wSTRNCAT(gRespStr, ",rxFrames,", 10);
       	for(i=0; i<numStreams; i++) 
        {
       	    wSPRINTF(copyBuf, "%d ", statResp[i].cmdru.stats.rxFrames);
       	    wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
       	}

       	wSTRNCAT(gRespStr, ",txPayloadBytes,", 16);
       	for(i=0; i<numStreams; i++) 
        {
       	    wSPRINTF(copyBuf, "%lu ", statResp[i].cmdru.stats.txPayloadBytes);
       	    wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }

        wSTRNCAT(gRespStr, ",rxPayloadBytes,", 16);
        for(i=0; i<numStreams; i++) 
        {
       	    wSPRINTF(copyBuf, " %lu ", statResp[i].cmdru.stats.rxPayloadBytes);
            wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }
        wSTRNCAT(gRespStr, ",outOfSequenceFrames,", 21);
        for(i=0; i<numStreams; i++) 
        {
       	    wSPRINTF(copyBuf, "%d ", statResp[i].cmdru.stats.outOfSequenceFrames);
	    wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }

	wPRINTF("jitter %lu\n", statResp[i].cmdru.stats.jitter);
        wSTRNCAT(gRespStr, "\r\n", 4);
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));
    return done;
}

int wfaTrafficAgentRecvStopResp(BYTE *cmdBuf)
{
    int done=1;
    int i = 0;
    int errorStatus = 0;
    char copyBuf[64];
    BYTE *dutRsp = cmdBuf+4;
    BYTE *startRsp = dutRsp; 
    wfaTLV *ptlv = (wfaTLV *)cmdBuf;
    int len = ptlv->len;
    int numStreams = len/sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Entering wfaTrafficAgentRecvStopResp ...\n");

    dutCmdResponse_t statResp[WFA_MAX_TRAFFIC_STREAMS];
    for(i=0; i<numStreams; i++) 
    {
        dutRsp = startRsp + i * sizeof(dutCmdResponse_t);
        memcpy(&statResp[i], dutRsp, sizeof(dutCmdResponse_t));
    }
    for(i=0; i<numStreams; i++) 
    {
        if(statResp[i].status != STATUS_COMPLETE)
            errorStatus = 1;
    }
    if(errorStatus) 
    {
        wSPRINTF(gRespStr, "status,ERROR");
    } 
    else 
    {
        wSPRINTF(gRespStr, "status,COMPLETE,streamID,");
        for(i=0; i<numStreams; i++) 
        {
            wSPRINTF(copyBuf, " %d", statResp[i].streamId);	
            wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }
        wSTRNCAT(gRespStr, ",txFrames,", 10);
        for(i=0; i<numStreams; i++) 
        {
            wSPRINTF(copyBuf, " %u", statResp[i].cmdru.stats.txFrames);
            wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }
        wSTRNCAT(gRespStr, ",rxFrames,", 10);
        for(i=0; i<numStreams; i++) 
        {
            wSPRINTF(copyBuf, " %u", statResp[i].cmdru.stats.rxFrames);
            wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }
	    wSTRNCAT(gRespStr, ",txPayloadBytes,", 16);
	    for(i=0; i<numStreams; i++) 
        {
            wSPRINTF(copyBuf, " %lu", statResp[i].cmdru.stats.txPayloadBytes);
            wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }
        wSTRNCAT(gRespStr, ",rxPayloadBytes,", 16);
        for(i=0; i<numStreams; i++) 
        {
            wSPRINTF(copyBuf, " %lu", statResp[i].cmdru.stats.rxPayloadBytes);
            wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }
        wSTRNCAT(gRespStr, ",outOfSequenceFrames,", 21);
        for(i=0; i<numStreams; i++) 
        {
            wSPRINTF(copyBuf, " %d", statResp[i].cmdru.stats.outOfSequenceFrames);
            wSTRNCAT(gRespStr, copyBuf, sizeof(copyBuf)-1);
        }
        wSTRNCAT(gRespStr, "\r\n", 4);
    }
		
    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));
    wPRINTF("gRespStr = %s", gRespStr);
    return done;
}

int wfaTrafficAgentPingStartResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *staPingResp = (dutCmdResponse_t *) (cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaTrafficAgentPingStartResp ...\n");

    switch(staPingResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaTrafficAgentPingStart running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,streamID,%d\r\n", staPingResp->streamId);
        break;

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));
    DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);

    return done;
}

int wfaTrafficAgentPingStopResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *stpResp = (dutCmdResponse_t *) (cmdBuf + 4);

    switch(stpResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaTrafficAgentPingStop running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        {
            wSPRINTF(gRespStr, "status,COMPLETE,sent,%d,replies,%d\r\n",
                 stpResp->cmdru.pingStp.sendCnt,
                 stpResp->cmdru.pingStp.repliedCnt);
                 DPRINT_INFO(WFA_OUT, "%s\n", gRespStr);
            break;
        }

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));
    return done;
}

int wfaStaGetMacAddressResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *getmacResp = (dutCmdResponse_t *) (cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaStaGetMacAddressResp ...\n");
    switch(getmacResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaGetMacAddress running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,mac,%s\r\n", getmacResp->cmdru.mac);
        wPRINTF("status,COMPLETE,mac,%s\r\n", getmacResp->cmdru.mac);
        break;

        case STATUS_ERROR:
        wPRINTF("status,ERROR\n");
        wSPRINTF(gRespStr, "status,COMPLETE,mac,00:00:00:00:00:00\r\n");
        break;

        default:
        wSPRINTF(gRespStr, "status,COMPLETE,mac,00:00:00:00:00:00\r\n");
        wPRINTF("unknown status\n");
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

int wfaStaGetBSSIDResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *getBssidResp = (dutCmdResponse_t *) (cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaStaGetBSSIDResp ...\n");
    switch(getBssidResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaGetBSSID running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,bssid,%s\r\n", getBssidResp->cmdru.bssid);
        wPRINTF("status,COMPLETE,bssid,%s\r\n", getBssidResp->cmdru.bssid);
        break;
        case STATUS_ERROR:
        wPRINTF("status,ERROR\n");
        wSPRINTF(gRespStr, "status,COMPLETE,bssid,00:00:00:00:00:00\r\n");
        break;
        default:
        wSPRINTF(gRespStr, "status,COMPLETE,bssid,00:00:00:00:00:00\r\n");
        wPRINTF("unknown status\n");
    }
    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

int wfaStaSetEncryptionResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *getBssidResp = (dutCmdResponse_t *) (cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetEncryptionResp ...\n");
    switch(getBssidResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaSetEncryption running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSTRCPY(gRespStr, "status,COMPLETE\r\n");
        wPRINTF("status,COMPLETE\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
        break;

        case STATUS_ERROR:
        wSPRINTF(gRespStr, "status,ERROR\r\n");
        wPRINTF("status,ERROR\r\n");
        break;

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

#if 0
int wfaStaReAssociateResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *assocResp = (dutCmdResponse_t *) (cmdBuf + 4);

    DPRINT_INFO(WFA_OUT, "Entering wfaStaReAssociateResp ...\n");
    switch(assocResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaReAssociate running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
        break;

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

#endif

int wfaStaGetStatsResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *getStatsResp = (dutCmdResponse_t *) (cmdBuf + 4);
    caStaGetStatsResp_t *stats = &getStatsResp->cmdru.ifStats;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaGetStatsResp ...\n");

    switch(getStatsResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaGetStats running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,txFrames,%i,rxFrames,%i,txMulticast,%i,rxMulticast,%i,fcsErrors,%i,txRetries,%i\r\n",
           stats->txFrames, stats->rxFrames, stats->txMulticast, stats->rxMulticast, stats->fcsErrors, stats->txRetries);
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
        break;

        case STATUS_ERROR:
        wSPRINTF(gRespStr, "status,ERROR\r\n");
        break;

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

int wfaDeviceGetInfoResp(BYTE *cmdBuf)
{
    int done=1;
    dutCmdResponse_t *devInfoResp = (dutCmdResponse_t *) (cmdBuf + 4);
    caDeviceGetInfoResp_t *dinfo = &devInfoResp->cmdru.devInfo;

    switch(devInfoResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaDeviceGetInfo running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
	if(dinfo->firmware[0] != '\0' || dinfo->firmware[0] != '\n')
           wSPRINTF(gRespStr, "status,COMPLETE,firmware,%s\r\n", dinfo->firmware);
	else
           wSPRINTF(gRespStr, "status,COMPLETE,vendor,%s,model,%s,version,%s\r\n",
               dinfo->vendor, dinfo->model, dinfo->version);
        DPRINT_INFO(WFA_OUT, "%s\n", gRespStr);
        break;

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

int wfaDeviceListIFResp(BYTE *cmdBuf)
{
    int done=0, i;
    dutCmdResponse_t *devListIfResp = (dutCmdResponse_t *) (cmdBuf + 4);
    caDeviceListIFResp_t *ifResp = &devListIfResp->cmdru.ifList;

    switch(devListIfResp->status)
    {
       case STATUS_RUNNING:
       DPRINT_INFO(WFA_OUT, "wfaDeviceListIF running ...\n");
       done = 1;
       break;

       case STATUS_COMPLETE:
       if(ifResp->iftype == IF_80211)
       {
          wSPRINTF(gRespStr, "status,COMPLETE,interfaceType,802.11,interfaceID");
          DPRINT_INFO(WFA_OUT, "%s\n", gRespStr);
          DPRINT_INFO(WFA_OUT, "%s\n", ifResp->ifs[0]);
       }
       else if(ifResp->iftype == IF_ETH)
          wSPRINTF(gRespStr, "status,COMPLETE,interfaceType,Ethernet,interfaceID");

       for(i=0; i<1; i++)
       {
         if(ifResp->ifs[i][0] != '\0')
         {
            wSTRNCAT(gRespStr,",", 4);
            wSTRNCAT(gRespStr, ifResp->ifs[i], sizeof(ifResp->ifs[i]));
            wSTRNCAT(gRespStr, "\r\n", 4);
         }
       }

       DPRINT_INFO(WFA_OUT, "%s\n", gRespStr);
       break;

       default:
       wSPRINTF(gRespStr, "status,INVALID\r\n");
       DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}

int wfaStaUploadResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *uploadResp = (dutCmdResponse_t *) (cmdBuf + 4);
    caStaUploadResp_t *upld = &uploadResp->cmdru.uld;

    switch(uploadResp->status)
    {
        case STATUS_RUNNING:
        DPRINT_INFO(WFA_OUT, "wfaStaUpload running ...\n");
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE,code,%d,%s\r\n", 
                 upld->seqnum, upld->bytes);
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
        break;

        case STATUS_ERROR:
        wSPRINTF(gRespStr, "status,ERROR\r\n");
        wPRINTF("status,COMPLETE\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
        break;
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, strlen(gRespStr));

    return done;
}

int wfaStaGenericResp(BYTE *cmdBuf)
{
    int done=0;
    dutCmdResponse_t *setwmmResp = (dutCmdResponse_t *) (cmdBuf + 4);

    switch(setwmmResp->status)
    {
        case STATUS_RUNNING:
        done = 1;
        break;

        case STATUS_COMPLETE:
        wSPRINTF(gRespStr, "status,COMPLETE\r\n");
        wPRINTF("status,COMPLETE\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
        break;

        case STATUS_ERROR:
        wSPRINTF(gRespStr, "status,ERROR\r\n");
        wPRINTF("status,COMPLETE\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
        break;

        default:
        wSPRINTF(gRespStr, "status,INVALID\r\n");
        DPRINT_INFO(WFA_OUT, " %s\n", gRespStr);
    }

    wfaCtrlSend(gCaSockfd, (BYTE *)gRespStr, wSTRLEN(gRespStr));

    return done;
}
