#include "SDHFaxCommonHeader.h"
#include <stdio.h>
#include <string.h>
#include "ATypes.h"
#include "faxapp_param.h"
#include "db_api.h"
#include "../../faxapp/include/faxapp_notify.h"
#include "../../fax_memmgr/include/fax_memAPI.h"


extern void Fax_API_T30Attribute(UINT8 set_get, T30_ATTRIB attrib_id, UINT8 *msg);
extern void SetPhoneParams(UINT8 type, UINT16 val);
extern int faxapp_get_select_country_from_db(void);

int gFaxToXSet = 0;
UINT8 gImageBackup = 0; //0: All, 1: Rx Only, 2: Off
static int gFaxPrintMode = FAX_PRINT_MODE_PRINT_ALL;
static int isAutoReductionOn = DISABLE;
int DiscardLength = 10; // 10mm
static int isDuplexPrintOn = DISABLE;
static int FaxInputTray = FAX_INPUT_TRAY_AUTO;
static int SaveFaxFile = DISABLE;
UINT8 gRingDetect = 1;
#if __TX_REPORT_IMAGE_PRINT__
static int gFaxTxResultRptImage = DISABLE;

extern void tx_report_image_delete(void);
#endif
int FileFormat = SAVE_FILE_FORMAT_PDF;
int PageType = SAVE_FILE_PAGE_TYPE_SINGLE_PAGE;
int gFaxAutoDarkness = 0;
int gFaxBgRemove = 5;
int gFaxSendDestination = SEND_DESTINATION_EMAIL;
int gFaxEmailPort = 0;
int gFaxFTPPort = 0;
int gFaxSMBPort = 0;
int gFaxWebDavPort = 0;
int gFaxLineMonitor = 0;
UINT8 isSolutionFax = 0;
UINT8 isImageLog = 0;
UINT8 MinDetectableTipRingVolt = 18; // 18V
UINT8 gFaxStationID[LEN_PHONE_NO + 1] = {0, };
UINT8 gFaxStationNo[LEN_PHONE_NO + 1] = {0, };
UINT8 gRealFaxStationNo[LEN_PHONE_NO + 1] = {0, };
UINT8 isSpamFaxOn = 0;
UINT8 NoOfSpamFaxList = 0;
UINT8 gFaxLineInUseThreshold = 0; // Do NOT Use User-Set 'LineInUseThreshold' Parameter, Use 'Country-Dependent' Parameter
UINT8 isCIDOn = DISABLE;
UINT16 DialTimeout = 0; // Do NOT Use User-Set 'Timeout' Parameter, Use 'Country-Dependent' Parameter
UINT16 gFaxDelayForPauseKey = 2000; // 2S
UINT32 gFaxCEDDetectionTime = 38000; // 38S
UINT32 gFaxT1Timeout = 38000; // 38S
UINT32 gFaxT5Timeout = 60000; // 60S
char gFaxEmailUserName[MAX_STRING_LEN] = {0, };
char gFaxEmailID[MAX_STRING_LEN] = {0, };
char gFaxEmailPW[MAX_STRING_LEN] = {0, };
char gFaxSenderEmailAddress[MAX_STRING_LEN] = {0, };
char gFaxSenderServerAddress[MAX_STRING_LEN] = {0, };
char gFaxReceiverEmailAddress[MAX_STRING_LEN] = {0, };
char gFaxEmailSubject[MAX_STRING_LEN] = {0, };
char gFaxEmailText[MAX_STRING_LEN] = {0, };
char gFaxFTPID[MAX_STRING_LEN] = {0, };
char gFaxFTPPW[MAX_STRING_LEN] = {0, };
char gFaxFTPAddress[MAX_STRING_LEN] = {0, };
char gFaxFTPPath[MAX_STRING_LEN] = {0, };
char gFaxSMBID[MAX_STRING_LEN] = {0, };
char gFaxSMBPW[MAX_STRING_LEN] = {0, };
char gFaxSMBAddress[MAX_STRING_LEN] = {0, };
char gFaxSMBPath[MAX_STRING_LEN] = {0, };
char gFaxWebDavID[MAX_STRING_LEN] = {0, };
char gFaxWebDavPW[MAX_STRING_LEN] = {0, };
char gFaxWebDavAddress[MAX_STRING_LEN] = {0, };
char gFaxWebDavPath[MAX_STRING_LEN] = {0, };
char gFaxSpamFaxList[MAX_SPAM_FAX_LIST][LEN_PHONE_NO + 1];
#if __SUPPORT_MANUAL_RECEIVE__
char RemoteAccessCode[MAX_LEN_REMOTE_ACCESS_CODE + 1] = {0, };
#else
#endif
REBOOT_NEED_PARAM_t _prevParam = { };

extern UINT8 isFaxAppReady;
extern UINT8 gFaxRebootCnt;

int getDuplexPrintOn(void)
{
    return isDuplexPrintOn;
}

int getAutoReductionOn(void)
{
    return isAutoReductionOn;
}

int getSaveFaxFile(void)
{
    return SaveFaxFile;
}

int getFaxInputTray(void)
{
    return FaxInputTray;
}

int getgFaxPrintMode(void)
{
    return gFaxPrintMode;
}
#if __TX_REPORT_IMAGE_PRINT__
int getgFaxTxResultRptImage(void)
{
    return gFaxTxResultRptImage;
}
#endif

UINT8 getImageBackup(void)
{
	return gImageBackup;
}

/***********************************************************************
 *  Function: faxapp_init_parameter
 *
 *  Description:
 *    Init. all fax related parameters during the boot process from DB
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *    none
 **********************************************************************/
void faxapp_init_parameter (void)
{
    typedef struct
    {
        char *pStationID;
        char *pStationNum;
        int TTI;
        UINT8 isTTIOn;
        int LineMonitor;
        UINT8 TxLineMonitor;
        UINT8 RxLineMonitor;
        int Volume;
        UINT8 SpeakerVolume;
        int TxRetryNum;
        UINT8 RetryNum;
        int TxRetryInterval;
        UINT8 RetryWaitTime;
        int TxECM;
        UINT8 isECMOn;
        int RxForwardSet;
        UINT8 isFaxForwardOn;
        UINT8 RxForwardNum[LEN_PHONE_NO + 1];
        char *pRxForwardNum;
        int RxConfSet;
        BOOLEAN isRxConfOn;
        UINT8 RxConfPW[5];
        char *pRxConfPW;
        UINT8 buff_RxConfPW[21];
        int RxRingNum;
        UINT8 NoOfRings;
        int RxReductionSel;
        int TxConfSet;
        BOOLEAN isTxConfOn;
        UINT8 TxConfPW[5];
        char *pTxConfPW;
        UINT8 buff_TxConfPW[21];
        int PrintMode;
        int DiscardLength;
        int DuplexPrint;
        int FaxInputTray;
        int SaveFaxFile;
        int FileFormat;
        int PageType;
        int AutoDarkness;
        int BgRemove;
        int SendDestination;
        char *pEmailID;
        char *pEmailPW;
        char *pSenderEmailAddress;
        char *pEmailServerAddress;
        int EmailPort;
        char *pReceiverEmailAddress;
        char *pEmailSubject;
        char *pEmailText;
        char *EmailUserName;
        char *pFTPID;
        char *pFTPPW;
        char *pFTPAddress;
        char *pFTPPath;
        int FTPPort;
        char *pSMBID;
        char *pSMBPW;
        char *pSMBAddress;
        int SMBPort;
        char *pSMBPath;
        char *pWebDavID;
        char *pWebDavPW;
        char *pWebDavAddress;
        int WebDavPort;
        char *pWebDavPath;
        int isSpamFaxOn;
        int NoOfSpamFax;
        int Len;
        int Index;
#if __SUPPORT_MANUAL_RECEIVE__
        int ManualReceive;
        UINT8 isManualReceiveOn;
        UINT8 AnswerMode;
        int RemoteAccessCode;
#else
#endif
		int FaxToXSet;
    } FAX_USER_PARAM;

    typedef struct
    {
        int TxStartSpeed;
        UINT8 buff_TxStartSpeed;
        int RxStartSpeed;
        UINT8 buff_RxStartSpeed;
        int TxEncoding;
        int TransmitLevel;
        UINT8 buff_TransmitLevel;
        int MinDetectableTipRingVolt;
        int DialTimeout;
        int DialToneDetection;
        int CEDDetectionTime;
        UINT8 buff_CEDDetectionTime;
        int T1Timeout;
        int T5Timeout;
        int TCFErrorRate;
        UINT8 buff_TCFErrorRate;
        int NoOfTriesOnFallback;
        UINT8 buff_NoOfTriesOnFallback;
        int ReceiveMaxGain;
        UINT8 buff_ReceiveMaxGain;
        int DelayForPauseKey;
        int LineInUseThreshold;
        int FaxTxResultRptImage;
        UINT8 FaxSolution;
        UINT8 ImageLog;
        UINT8 RingDetect;
#if __SUPPORT_CID__
        int CID;
#else
#endif
		UINT8 ImageBackup;
    } FAX_SERVICE_PARAM;

    error_type_t db_retval = FAIL;
    BOOLEAN buff_FALSE = FALSE;
    BOOLEAN buff_TRUE = TRUE;
    UINT8 Index = 0;

    FAX_USER_PARAM User = { };
    FAX_SERVICE_PARAM Service = { };

    FAXSPAM_STRUCT faxspam_result[MAX_SPAM_FAX_LIST];

    ///////////////////////////////
    // Step 1. Parameters for User
    ///////////////////////////////

    // Step 1-1. Station ID
    User.pStationID = db_get_var("CONF_FAXUP", "StationID", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> StationID\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        memset(gFaxStationID, 0x20, sizeof(gFaxStationID));

        if (User.pStationID == (char *)NULL)
        {
            gFaxStationID[40] = 0;
        }
        else
        {
            User.Len = strlen(User.pStationID);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxStationID[User.Index] = User.pStationID[User.Index];
            }

            gFaxStationID[40] = 0;

            MEM_FREE_AND_NULL(User.pStationID);
        }

        printf("\n(YHCHO) %s() - gFaxStationID: %s\n", __FUNCTION__, gFaxStationID);

        Fax_API_T30Attribute (SET, FXA_TEXTHEADER, gFaxStationID);
    }

    // Step 1-2. Station Num
    User.pStationNum = db_get_var("CONF_FAXUP", "StationNum", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> StationNum\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        memset(gFaxStationNo, 0x20, sizeof(gFaxStationNo));
        memset(gRealFaxStationNo, 0, sizeof(gRealFaxStationNo));

        if (User.pStationNum == (char *)NULL)
        {
            gFaxStationNo[40] = 0;
        }
        else
        {
            User.Len = strlen(User.pStationNum);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxStationNo[User.Index] = User.pStationNum[User.Index];
                gRealFaxStationNo[User.Index] = User.pStationNum[User.Index];
            }

            gFaxStationNo[40] = 0;
            gRealFaxStationNo[40] = 0;

            MEM_FREE_AND_NULL(User.pStationNum);
        }

        printf("\n(YHCHO) %s() - gFaxStationNo: %s\n", __FUNCTION__, gFaxStationNo);

        Fax_API_T30Attribute(SET, FXA_LOCALID, gFaxStationNo);
    }

    // Step 1-3. TTI (Transmit Terminal Identification)
    User.TTI = db_get_var_int("CONF_FAXUP", "TTI", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> TTI\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.TTI: %d\n", __FUNCTION__, User.TTI);

        User.isTTIOn = (UINT8)User.TTI;

        Fax_API_T30Attribute(SET, FXA_TTI_ENABLE, &User.isTTIOn);
    }

    // Step 1-4. Line Monitor
    // Step 1-5. Volume
    User.LineMonitor = db_get_var_int("CONF_FAXUP", "LineMonitor", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> LineMonitor\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.LineMonitor: %d\n", __FUNCTION__, User.LineMonitor);

        gFaxLineMonitor = User.LineMonitor;

        if (User.LineMonitor == LINE_MONITOR_OFF)
        {
            User.SpeakerVolume = 0;

            User.TxLineMonitor = STATE_TX_PHASE_A;

            User.RxLineMonitor = STATE_RX_PHASE_A;
        }
        else if (User.LineMonitor == LINE_MONITOR_ON_TILL_CONNECT)
        {
            User.Volume = db_get_var_int("CONF_FAXUP", "Volume", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> Volume\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - User.Volume: %d\n", __FUNCTION__, User.Volume);

                User.SpeakerVolume = (UINT8)User.Volume;

                User.TxLineMonitor = STATE_TX_PHASE_C;

                User.RxLineMonitor = STATE_RX_PHASE_C;
            }
        }
        else // Always On
        {
            User.Volume = db_get_var_int("CONF_FAXUP", "Volume", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> Volume\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - User.Volume: %d\n", __FUNCTION__, User.Volume);

                User.SpeakerVolume = (UINT8)User.Volume;

                User.TxLineMonitor = STATE_TX_DISCONN;

                User.RxLineMonitor = STATE_RX_DISCONN;
            }
        }

        Fax_API_T30Attribute (SET, FXA_SPEAKERVOLUME, &User.SpeakerVolume);

        Fax_API_T30Attribute (SET, FXA_SPEAKER_CALLMODE, &User.TxLineMonitor);

        Fax_API_T30Attribute (SET, FXA_SPEAKER_ANSWERMODE, &User.RxLineMonitor);
    }

    // Step 1-6. Tx Retry Num
    // Step 1-7. Tx Retry Interval
    User.TxRetryNum = db_get_var_int("CONF_FAXUP", "TxRetryNum", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> TxRetryNum\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.TxRetryNum: %d\n", __FUNCTION__, User.TxRetryNum);

        User.RetryNum = (UINT8)User.TxRetryNum + 1;

        Fax_API_T30Attribute(SET, FXA_RETRY_NUMBER, &User.RetryNum);

        if (User.RetryNum == 1)
        {
            User.RetryWaitTime = 0;

            Fax_API_T30Attribute(SET, FXA_RETRY_INTERVAL, &User.RetryWaitTime);
        }
        else
        {
            User.TxRetryInterval = db_get_var_int("CONF_FAXUP", "TxRetryInterval", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> TxRetryInterval\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - User.TxRetryInterval: %d\n", __FUNCTION__, User.TxRetryInterval);

                User.RetryWaitTime = (UINT8)User.TxRetryInterval;

                Fax_API_T30Attribute(SET, FXA_RETRY_INTERVAL, &User.RetryWaitTime);
            }
        }
    }

    // Step 1-8. ECM (Error Correction Mode)
    User.TxECM = db_get_var_int("CONF_FAXUP", "TxECM", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> TxECM\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.TxECM: %d\n", __FUNCTION__, User.TxECM);

        User.isECMOn = (UINT8)User.TxECM;

        Fax_API_T30Attribute (SET, FXA_ECMMODE, &User.isECMOn);
    }

    // Step 1-9. Rx Forward Set
    // Step 1-10. Rx Forward Num
    User.RxForwardSet = db_get_var_int("CONF_FAXUP", "RxForwardSet", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RxForwardSet\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.RxForwardSet: %d\n", __FUNCTION__,  User.RxForwardSet);

        User.isFaxForwardOn = (UINT8)User.RxForwardSet;

        Fax_API_T30Attribute(SET, FXA_FAXFORWARD, &User.isFaxForwardOn);

        if (User.isFaxForwardOn == ENABLE)
        {
            User.pRxForwardNum = db_get_var("CONF_FAXUP", "RxForwardNum", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> RxForwardNum\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                memset(User.RxForwardNum, 0x00, sizeof(User.RxForwardNum));

                if (User.pRxForwardNum == (char *)NULL)
                {
                    User.RxForwardNum[0] = 0;
                }
                else
                {
                    User.Len = strlen(User.pRxForwardNum);

                    for (User.Index = 0; User.Index < User.Len; User.Index++)
                    {
                        User.RxForwardNum[User.Index] = User.pRxForwardNum[User.Index];
                    }

                    User.RxForwardNum[User.Index] = 0;

                    MEM_FREE_AND_NULL(User.pRxForwardNum);
                }

                printf("\n(YHCHO) %s() - User.RxForwardNum: %s\n", __FUNCTION__, User.RxForwardNum);

                Fax_API_T30Attribute(SET, FXA_FAXFORWARD_NO, User.RxForwardNum);
            }
        }
    }

    // Step 1-11. Rx Conf. Set
    // Step 1-12. Rx Conf. PW
    User.RxConfSet = db_get_var_int("CONF_FAXUP", "RxConfSet", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RxConfSet\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.RxConfSet: %d\n", __FUNCTION__, User.RxConfSet);

        if (User.RxConfSet == DISABLE)
        {
            User.isRxConfOn = FALSE;
            Fax_API_T30Attribute(SET, FXA_RXREQUIREPASSWORD, &User.isRxConfOn);
        }
        else
        {
            User.isRxConfOn = TRUE;
            Fax_API_T30Attribute(SET, FXA_RXREQUIREPASSWORD, &User.isRxConfOn);

            User.pRxConfPW = db_get_var("CONF_FAXUP", "RxConfPW", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> RxConfPW\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                memset(User.RxConfPW, 0x20, sizeof(User.RxConfPW));

                if (User.pRxConfPW == (char *)NULL)
                {
                    User.buff_RxConfPW[20] = 0;
                }
                else
                {
                    User.Len = strlen(User.pRxConfPW);

                    for (User.Index = 0; User.Index < User.Len; User.Index++)
                    {
                        User.RxConfPW[User.Index] = User.pRxConfPW[User.Index];
                    }

                    User.RxConfPW[4] = 0;

                    //printf("\n(YHCHO) %s() - User.RxConfPW: %s\n", __FUNCTION__, User.RxConfPW);

                    memset(User.buff_RxConfPW, 0x20, sizeof(User.buff_RxConfPW));

                    for( Index = 0; Index < 4; Index++ )
                    {
                        User.buff_RxConfPW[16 + Index] = User.RxConfPW[Index];
                    }

                    for( Index = 0; Index < 16; Index++ )
                    {
                        User.buff_RxConfPW[Index] = 0x20;
                    }

                    User.buff_RxConfPW[20] = 0;

                    MEM_FREE_AND_NULL(User.pRxConfPW);
                }

                printf("\n(YHCHO) %s() - User.buff_RxConfPW: %s\n", __FUNCTION__, User.buff_RxConfPW);

                Fax_API_T30Attribute(SET, FXA_RXPASSWORD, User.buff_RxConfPW);
            }
        }
    }

    // Step 1-13. Rx Ring Num
    User.RxRingNum = db_get_var_int("CONF_FAXUP", "RxRingNum", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RxRingNum\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.RxRingNum: %d\n", __FUNCTION__, User.RxRingNum);

        if (User.RxRingNum != 1)
        {
            User.RxRingNum--;
        }

        User.NoOfRings = (UINT8)User.RxRingNum;

        Fax_API_T30Attribute(SET, FXA_AUTOANSWERRINGS, &User.NoOfRings);
    }

    // Step 1-14. Rx Reduction Sel
    User.RxReductionSel = db_get_var_int("CONF_FAXUP", "RxReductionSel", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RxReductionSel\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.RxReductionSel: %d\n", __FUNCTION__, User.RxReductionSel);

        isAutoReductionOn = User.RxReductionSel;
    }

    // Step 1-15. Tx Conf. Set
    // Step 1-16. Tx Conf. PW
    User.TxConfSet = db_get_var_int("CONF_FAXUP", "TxConfSet", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> TxConfSet\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.TxConfSet: %d\n", __FUNCTION__, User.TxConfSet);

        if (User.TxConfSet == DISABLE)
        {
            User.isTxConfOn = FALSE;
            Fax_API_T30Attribute(SET, FXA_TXREQUIREPASSWORD, &User.isTxConfOn);
        }
        else
        {
            User.isTxConfOn = TRUE;
            Fax_API_T30Attribute(SET, FXA_TXREQUIREPASSWORD, &User.isTxConfOn);

            User.pTxConfPW = db_get_var("CONF_FAXUP", "TxConfPW", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> TxConfPW\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                memset(User.TxConfPW, 0x20, sizeof(User.TxConfPW));

                if (User.pTxConfPW == (char *)NULL)
                {
                    User.buff_TxConfPW[20] = 0;
                }
                else
                {
                    User.Len = strlen(User.pTxConfPW);

                    for (User.Index = 0; User.Index < User.Len; User.Index++)
                    {
                        User.TxConfPW[User.Index] = User.pTxConfPW[User.Index];
                    }

                    User.TxConfPW[4] = 0;

                    //printf("\n(YHCHO) %s() - User.TxConfPW: %s\n", __FUNCTION__, User.TxConfPW);

                    memset(User.buff_TxConfPW, 0x20, sizeof(User.buff_TxConfPW));

                    for( Index = 0; Index < 4; Index++ )
                    {
                        User.buff_TxConfPW[16 + Index] = User.TxConfPW[Index];
                    }

                    for( Index = 0; Index < 16; Index++ )
                    {
                        User.buff_TxConfPW[Index] = 0x20;
                    }

                    User.buff_TxConfPW[20] = 0;

                    MEM_FREE_AND_NULL(User.pTxConfPW);
                }

                printf("\n(YHCHO) %s() - User.buff_TxConfPW: %s\n", __FUNCTION__, User.buff_TxConfPW);

                Fax_API_T30Attribute(SET, FXA_TXPASSWORD, User.buff_TxConfPW);
            }
        }
    }

    // Step 1-17 Print Mode
    User.PrintMode = db_get_var_int("CONF_FAXUP", "PrintMode", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> PrintMode\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.PrintMode: %d\n", __FUNCTION__, User.PrintMode);

        gFaxPrintMode = User.PrintMode;
    }

    // Step 1-18. Discard Length
    User.DiscardLength = db_get_var_int("CONF_FAXUP", "DiscardLength", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> DiscardLength\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.DiscardLength: %d\n", __FUNCTION__, User.DiscardLength);

        DiscardLength = User.DiscardLength;
    }

    // Step 1-19. Duplex Print
    User.DuplexPrint = db_get_var_int("CONF_FAXUP", "DuplexPrint", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> DuplexPrint\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.DuplexPrint: %d\n", __FUNCTION__, User.DuplexPrint);

        isDuplexPrintOn = User.DuplexPrint;
    }

    // Step 1-20. Fax Input Tray
    User.FaxInputTray = db_get_var_int("CONF_FAXUP", "FaxInputTray", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> FaxInputTray\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.FaxInputTray: %d\n", __FUNCTION__, User.FaxInputTray);

        FaxInputTray = User.FaxInputTray;
    }

    // Step 1-21. Save Fax File
    User.SaveFaxFile = db_get_var_int("CONF_FAXUP", "SaveFaxFile", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> SaveFaxFile\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.SaveFaxFile: %d\n", __FUNCTION__, User.SaveFaxFile);

        SaveFaxFile = User.SaveFaxFile;
    }

    // Step 1-22. File Format
    User.FileFormat = db_get_var_int("CONF_FAXUP", "FileFormat", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> FileFormat\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.FileFormat: %d\n", __FUNCTION__, User.FileFormat);

        FileFormat = User.FileFormat;
    }

    // Step 1-23. Page Type
    User.PageType = db_get_var_int("CONF_FAXUP", "PageType", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> PageType\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.PageType: %d\n", __FUNCTION__, User.PageType);

        PageType = User.PageType;
    }

    // Step 1-24. Auto Darkness
    User.AutoDarkness = db_get_var_int("CONF_FAXUP", "AutoDarkness", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> AutoDarkness\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.AutoDarkness: %d\n", __FUNCTION__, User.AutoDarkness);

        gFaxAutoDarkness = User.AutoDarkness;
    }

    // Step 1-25. Background Remove
    User.BgRemove = db_get_var_int("CONF_FAXUP", "BgRemove", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> BgRemove\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.BgRemove: %d\n", __FUNCTION__, User.BgRemove);

        gFaxBgRemove = User.BgRemove;
    }

    // Step 1-26. Send Destination
    User.SendDestination = db_get_var_int("CONF_FAXUP", "SendDestination", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> SendDestination\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.SendDestination: %d\n", __FUNCTION__, User.SendDestination);

        gFaxSendDestination = User.SendDestination;
    }

    // Step 1-27. Email ID
    User.pEmailID = db_get_var("INST_FAXSOLUTION", "EmailID", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailID\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pEmailID == (char *)NULL)
        {
            gFaxEmailID[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pEmailID: %s\n", __FUNCTION__, User.pEmailID);

            memset(gFaxEmailID, 0x00, sizeof(gFaxEmailID));

            User.Len = strlen(User.pEmailID);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxEmailID[User.Index] = User.pEmailID[User.Index];
            }

            gFaxEmailID[User.Index] = 0;

            MEM_FREE_AND_NULL(User.pEmailID);

            printf("\n(YHCHO) %s() - gFaxEmailID: %s\n", __FUNCTION__, gFaxEmailID);
        }
    }

    // Step 1-28. Email PW
    User.pEmailPW = db_get_var("INST_FAXSOLUTION", "EmailPW", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailPW\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pEmailPW == (char *)NULL)
        {
            gFaxEmailPW[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pEmailPW: %s\n", __FUNCTION__, User.pEmailPW);

            memset(gFaxEmailPW, 0x00, sizeof(gFaxEmailPW));

            User.Len = strlen(User.pEmailPW);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxEmailPW[User.Index] = User.pEmailPW[User.Index];
            }

            gFaxEmailPW[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxEmailPW: %s\n", __FUNCTION__, gFaxEmailPW);

            MEM_FREE_AND_NULL(User.pEmailPW);
        }
    }

    // Step 1-29. Sender Email Address
    User.pSenderEmailAddress = db_get_var("INST_FAXSOLUTION", "SenderEmailAddress", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SenderEmailAddress\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pSenderEmailAddress == (char *)NULL)
        {
            gFaxSenderEmailAddress[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pSenderEmailAddress: %s\n", __FUNCTION__, User.pSenderEmailAddress);

            memset(gFaxSenderEmailAddress, 0x00, sizeof(gFaxSenderEmailAddress));

            User.Len = strlen(User.pSenderEmailAddress);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxSenderEmailAddress[User.Index] = User.pSenderEmailAddress[User.Index];
            }

            gFaxSenderEmailAddress[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxSenderEmailAddress: %s\n", __FUNCTION__, gFaxSenderEmailAddress);

            MEM_FREE_AND_NULL(User.pSenderEmailAddress);
        }
    }

    // Step 1-30. Email Server Address
    User.pEmailServerAddress = db_get_var("INST_FAXSOLUTION", "EmailServerAddress", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailServerAddress\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pEmailServerAddress == (char *)NULL)
        {
            gFaxSenderServerAddress[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pEmailServerAddress: %s\n", __FUNCTION__, User.pEmailServerAddress);

            memset(gFaxSenderServerAddress, 0x00, sizeof(gFaxSenderServerAddress));

            User.Len = strlen(User.pEmailServerAddress);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxSenderServerAddress[User.Index] = User.pEmailServerAddress[User.Index];
            }

            gFaxSenderServerAddress[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxSenderServerAddress: %s\n", __FUNCTION__, gFaxSenderServerAddress);

            MEM_FREE_AND_NULL(User.pEmailServerAddress);
        }
    }

    // Step 1-31. Email Port
    User.EmailPort = db_get_var_int("INST_FAXSOLUTION", "EmailPort", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... INST_FAXSOLUTION -> EmailPort\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.EmailPort: %d\n", __FUNCTION__, User.EmailPort);

        gFaxEmailPort = User.EmailPort;
    }

    // Step 1-32. Receiver Email Address
    User.pReceiverEmailAddress = db_get_var("INST_FAXSOLUTION", "ReceiverEmailAddress", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> ReceiverEmailAddress\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pReceiverEmailAddress == (char *)NULL)
        {
            gFaxReceiverEmailAddress[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pReceiverEmailAddress: %s\n", __FUNCTION__, User.pReceiverEmailAddress);

            memset(gFaxReceiverEmailAddress, 0x00, sizeof(gFaxReceiverEmailAddress));

            User.Len = strlen(User.pReceiverEmailAddress);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxReceiverEmailAddress[User.Index] = User.pReceiverEmailAddress[User.Index];
            }

            gFaxReceiverEmailAddress[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxReceiverEmailAddress: %s\n", __FUNCTION__, gFaxReceiverEmailAddress);

            MEM_FREE_AND_NULL(User.pReceiverEmailAddress);
        }
    }

    // Step 1-33. Email Subject
    User.pEmailSubject = db_get_var("INST_FAXSOLUTION", "EmailSubject", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailSubject\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pEmailSubject == (char *)NULL)
        {
            gFaxEmailSubject[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pEmailSubject: %s\n", __FUNCTION__, User.pEmailSubject);

            memset(gFaxEmailSubject, 0x00, sizeof(gFaxEmailSubject));

            User.Len = strlen(User.pEmailSubject);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxEmailSubject[User.Index] = User.pEmailSubject[User.Index];
            }

            gFaxEmailSubject[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxEmailSubject: %s\n", __FUNCTION__, gFaxEmailSubject);

            MEM_FREE_AND_NULL(User.pEmailSubject);
        }
    }

    // Step 1-34. Email Text
    User.pEmailText = db_get_var("INST_FAXSOLUTION", "EmailText", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailText\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pEmailText == (char *)NULL)
        {
            gFaxEmailText[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pEmailText: %s\n", __FUNCTION__, User.pEmailText);

            memset(gFaxEmailText, 0x00, sizeof(gFaxEmailText));

            User.Len = strlen(User.pEmailText);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxEmailText[User.Index] = User.pEmailText[User.Index];
            }

            gFaxEmailText[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxEmailText: %s\n", __FUNCTION__, gFaxEmailText);

            MEM_FREE_AND_NULL(User.pEmailText);
        }
    }

    // Step. User name
    User.EmailUserName = db_get_var("INST_FAXSOLUTION", "UserName", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> UserName\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.EmailUserName == (char *)NULL)
        {
            gFaxEmailUserName[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pReceiverEmailAddress: %s\n", __FUNCTION__, User.pReceiverEmailAddress);

            memset(gFaxEmailUserName, 0x00, sizeof(gFaxEmailUserName));

            User.Len = strlen(User.EmailUserName);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxEmailUserName[User.Index] = User.EmailUserName[User.Index];
            }

            gFaxEmailUserName[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxEmailUserName: %s\n", __FUNCTION__, gFaxEmailUserName);

            MEM_FREE_AND_NULL(User.EmailUserName);
        }
    }

    // Step 1-35. FTP ID
    User.pFTPID = db_get_var("INST_FAXSOLUTION", "FTPID", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> FTPID\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pFTPID == (char *)NULL)
        {
            gFaxFTPID[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pFTPID: %s\n", __FUNCTION__, User.pFTPID);

            memset(gFaxFTPID, 0x00, sizeof(gFaxFTPID));

            User.Len = strlen(User.pFTPID);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxFTPID[User.Index] = User.pFTPID[User.Index];
            }

            gFaxFTPID[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxFTPID: %s\n", __FUNCTION__, gFaxFTPID);

            MEM_FREE_AND_NULL(User.pFTPID);
        }
    }

    // Step 1-36. FTP PW
    User.pFTPPW = db_get_var("INST_FAXSOLUTION", "FTPPW", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> FTPPW\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pFTPPW == (char *)NULL)
        {
            gFaxFTPPW[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pFTPPW: %s\n", __FUNCTION__, User.pFTPPW);

            memset(gFaxFTPPW, 0x00, sizeof(gFaxFTPPW));

            User.Len = strlen(User.pFTPPW);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxFTPPW[User.Index] = User.pFTPPW[User.Index];
            }

            gFaxFTPPW[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxFTPPW: %s\n", __FUNCTION__, gFaxFTPPW);

            MEM_FREE_AND_NULL(User.pFTPPW);
        }
    }

    // Step 1-37. FTP Address
    User.pFTPAddress = db_get_var("INST_FAXSOLUTION", "FTPAddress", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> FTPAddress\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pFTPAddress == (char *)NULL)
        {
            gFaxFTPAddress[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pFTPAddress: %s\n", __FUNCTION__, User.pFTPAddress);

            memset(gFaxFTPAddress, 0x00, sizeof(gFaxFTPAddress));

            User.Len = strlen(User.pFTPAddress);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxFTPAddress[User.Index] = User.pFTPAddress[User.Index];
            }

            gFaxFTPAddress[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxFTPAddress: %s\n", __FUNCTION__, gFaxFTPAddress);

            MEM_FREE_AND_NULL(User.pFTPAddress);
        }
    }

    // Step 1-38. FTP Path
    User.pFTPPath = db_get_var("INST_FAXSOLUTION", "FTPPath", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> FTPPath\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pFTPPath == (char *)NULL)
        {
            gFaxFTPPath[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pFTPPath: %s\n", __FUNCTION__, User.pFTPPath);

            memset(gFaxFTPPath, 0x00, sizeof(gFaxFTPPath));

            User.Len = strlen(User.pFTPPath);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxFTPPath[User.Index] = User.pFTPPath[User.Index];
            }

            gFaxFTPPath[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxFTPPath: %s\n", __FUNCTION__, gFaxFTPPath);

            MEM_FREE_AND_NULL(User.pFTPPath);
        }
    }

    // Step 1-39. FTP Port
    User.FTPPort = db_get_var_int("INST_FAXSOLUTION", "FTPPort", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... INST_FAXSOLUTION -> FTPPort\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.FTPPort: %d\n", __FUNCTION__, User.FTPPort);

        gFaxFTPPort = User.FTPPort;
    }

    // Step 1-40. SMB ID
    User.pSMBID = db_get_var("INST_FAXSOLUTION", "SMBID", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SMBID\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pSMBID == (char *)NULL)
        {
            gFaxSMBID[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pSMBID: %s\n", __FUNCTION__, User.pSMBID);

            memset(gFaxSMBID, 0x00, sizeof(gFaxSMBID));

            User.Len = strlen(User.pSMBID);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxSMBID[User.Index] = User.pSMBID[User.Index];
            }

            gFaxSMBID[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxSMBID: %s\n", __FUNCTION__, gFaxSMBID);

            MEM_FREE_AND_NULL(User.pSMBID);
        }
    }

    // Step 1-41. SMB PW
    User.pSMBPW = db_get_var("INST_FAXSOLUTION", "SMBPW", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SMBPW\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pSMBPW == (char *)NULL)
        {
            gFaxSMBPW[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pSMBPW: %s\n", __FUNCTION__, User.pSMBPW);

            memset(gFaxSMBPW, 0x00, sizeof(gFaxSMBPW));

            User.Len = strlen(User.pSMBPW);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxSMBPW[User.Index] = User.pSMBPW[User.Index];
            }

            gFaxSMBPW[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxSMBPW: %s\n", __FUNCTION__, gFaxSMBPW);

            MEM_FREE_AND_NULL(User.pSMBPW);
        }
    }

    // Step 1-42. SMB Address
    User.pSMBAddress = db_get_var("INST_FAXSOLUTION", "SMBAddress", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SMBAddress\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pSMBAddress == (char *)NULL)
        {
            gFaxSMBAddress[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pSMBAddress: %s\n", __FUNCTION__, User.pSMBAddress);

            memset(gFaxSMBAddress, 0x00, sizeof(gFaxSMBAddress));

            User.Len = strlen(User.pSMBAddress);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxSMBAddress[User.Index] = User.pSMBAddress[User.Index];
            }

            gFaxSMBAddress[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxSMBAddress: %s\n", __FUNCTION__, gFaxSMBAddress);

            MEM_FREE_AND_NULL(User.pSMBAddress);
        }
    }

    // Step 1-43. SMB Port
    User.SMBPort = db_get_var_int("INST_FAXSOLUTION", "SMBPort", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... INST_FAXSOLUTION -> SMBPort\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.SMBPort: %d\n", __FUNCTION__, User.SMBPort);

        gFaxSMBPort = User.SMBPort;
    }

    // Step 1-44. SMB Path
    User.pSMBPath = db_get_var("INST_FAXSOLUTION", "SMBPath", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SMBPath\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pSMBPath == (char *)NULL)
        {
            gFaxSMBPath[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pSMBPath: %s\n", __FUNCTION__, User.pSMBPath);

            memset(gFaxSMBPath, 0x00, sizeof(gFaxSMBPath));

            User.Len = strlen(User.pSMBPath);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxSMBPath[User.Index] = User.pSMBPath[User.Index];
            }

            gFaxSMBPath[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxSMBPath: %s\n", __FUNCTION__, gFaxSMBPath);

            MEM_FREE_AND_NULL(User.pSMBPath);
        }
    }

    // Step 1-45. WebDav ID
    User.pWebDavID = db_get_var("INST_FAXSOLUTION", "WebDAVID", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> WebDAVID\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pWebDavID == (char *)NULL)
        {
            gFaxWebDavID[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pWebDavID: %s\n", __FUNCTION__, User.pWebDavID);

            memset(gFaxWebDavID, 0x00, sizeof(gFaxWebDavID));

            User.Len = strlen(User.pWebDavID);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxWebDavID[User.Index] = User.pWebDavID[User.Index];
            }

            gFaxWebDavID[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxWebDavID: %s\n", __FUNCTION__, gFaxWebDavID);

            MEM_FREE_AND_NULL(User.pWebDavID);
        }
    }

    // Step 1-46. WebDav PW
    User.pWebDavPW = db_get_var("INST_FAXSOLUTION", "WebDAVPW", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> WebDAVPW\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pWebDavPW == (char *)NULL)
        {
            gFaxWebDavPW[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pWebDavPW: %s\n", __FUNCTION__, User.pWebDavPW);

            memset(gFaxWebDavPW, 0x00, sizeof(gFaxWebDavPW));

            User.Len = strlen(User.pWebDavPW);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxWebDavPW[User.Index] = User.pWebDavPW[User.Index];
            }

            gFaxWebDavPW[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxWebDavPW: %s\n", __FUNCTION__, gFaxWebDavPW);

            MEM_FREE_AND_NULL(User.pWebDavPW);
        }
    }

    // Step 1-47. WebDav Address
    User.pWebDavAddress = db_get_var("INST_FAXSOLUTION", "WebDAVAddress", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> WebDAVAddress\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pWebDavAddress == (char *)NULL)
        {
            gFaxWebDavAddress[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pWebDavAddress: %s\n", __FUNCTION__, User.pWebDavAddress);

            memset(gFaxWebDavAddress, 0x00, sizeof(gFaxWebDavAddress));

            User.Len = strlen(User.pWebDavAddress);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxWebDavAddress[User.Index] = User.pWebDavAddress[User.Index];
            }

            gFaxWebDavAddress[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxWebDavAddress: %s\n", __FUNCTION__, gFaxWebDavAddress);

            MEM_FREE_AND_NULL(User.pWebDavAddress);
        }
    }

    // Step 1-48. WebDav Port
    User.WebDavPort = db_get_var_int("INST_FAXSOLUTION", "WebDAVPort", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... INST_FAXSOLUTION -> WebDAVPort\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.WebDavPort: %d\n", __FUNCTION__, User.WebDavPort);

        gFaxWebDavPort = User.WebDavPort;
    }

    // Step 1-49. WebDav Path
    User.pWebDavPath = db_get_var("INST_FAXSOLUTION", "WebDAVPath", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> WebDAVPath\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        if (User.pWebDavPath == (char *)NULL)
        {
            gFaxWebDavPath[0] = 0;
        }
        else
        {
            //printf("\n(YHCHO) %s() - User.pWebDavPath: %s\n", __FUNCTION__, User.pWebDavPath);

            memset(gFaxWebDavPath, 0x00, sizeof(gFaxWebDavPath));

            User.Len = strlen(User.pWebDavPath);

            for (User.Index = 0; User.Index < User.Len; User.Index++)
            {
                gFaxWebDavPath[User.Index] = User.pWebDavPath[User.Index];
            }

            gFaxWebDavPath[User.Index] = 0;

            printf("\n(YHCHO) %s() - gFaxWebDavPath: %s\n", __FUNCTION__, gFaxWebDavPath);

            MEM_FREE_AND_NULL(User.pWebDavPath);
        }
    }

    // Step 1-50. Spam Fax
    User.isSpamFaxOn = db_get_var_int("CONF_FAXUP", "SpamFax", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> SpamFax\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.isSpamFaxOn: %d\n", __FUNCTION__, User.isSpamFaxOn);

        isSpamFaxOn = (UINT8)User.isSpamFaxOn;

        db_retval = db_get_faxspamnumber(faxspam_result, &User.NoOfSpamFax);
        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_get_faxspamnumber() Error...\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
        else
        {
            NoOfSpamFaxList = 0;

            memset(gFaxSpamFaxList, 0x00, sizeof(gFaxSpamFaxList));

            for(User.Index = 0; User.Index < User.NoOfSpamFax; User.Index++)
            {
                if (faxspam_result[User.Index].faxspamnumber[0] != 0)
                {
                    snprintf(gFaxSpamFaxList[NoOfSpamFaxList], MAX_SPAM_FAX_LIST, "%s", faxspam_result[User.Index].faxspamnumber);

                    printf("\n(YHCHO) %s() - gFaxSpamFaxList[%d]: %s\n", __FUNCTION__, NoOfSpamFaxList, gFaxSpamFaxList[NoOfSpamFaxList]);

                    NoOfSpamFaxList++;
                }
            }
        }

        printf("\n(YHCHO) %s() - NoOfSpamFaxList: %d\n", __FUNCTION__, NoOfSpamFaxList);
    }

#if __SUPPORT_MANUAL_RECEIVE__
    // Step 1-51. Manual Receive
    User.ManualReceive = db_get_var_int("CONF_FAXUP", "ManualReceive", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> ManualReceive\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.ManualReceive: %d\n", __FUNCTION__, User.ManualReceive);

        User.isManualReceiveOn = (UINT8)User.ManualReceive;

        Fax_API_T30Attribute(SET, FXA_MANUALRECEIVE, &User.isManualReceiveOn);

        if (User.isManualReceiveOn == 1)
        {
            User.AnswerMode = MANUAL_ANSWER;
        }
        else
        {
            User.AnswerMode = AUTO_ANSWER;
        }

        Fax_API_T30Attribute(SET, FXA_ANSWERMODE, &User.AnswerMode);
    }

    // Step 1-52. Remote Access Code
    User.RemoteAccessCode = db_get_var_int("CONF_FAXUP", "RemoteAccessCode", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RemoteAccessCode\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.RemoteAccessCode: %d\n", __FUNCTION__, User.RemoteAccessCode);

        memset(RemoteAccessCode, 0x00, sizeof(RemoteAccessCode));

        RemoteAccessCode[0] = '*';
        RemoteAccessCode[1] = User.RemoteAccessCode + '0';
        RemoteAccessCode[2] = '*';
        RemoteAccessCode[3] = 0;

        printf("\n(YHCHO) %s() - RemoteAccessCode: %s\n", __FUNCTION__, RemoteAccessCode);
    }
#else
#endif

    // Step 1-48. FaxToXSet
    User.FaxToXSet = db_get_var_int("CONF_FAXUP", "FaxToXSet", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> FaxToXSet\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - User.Footer: %d\n", __FUNCTION__, User.FaxToXSet);

        gFaxToXSet = (int)User.FaxToXSet;
    }

    //////////////////////////////////
    // Step 2. Parameters for Service
    //////////////////////////////////

    // Step 2-1. Tx Start Speed
    Service.TxStartSpeed = db_get_var_int("CONF_FAXSP", "TxStartSpeed", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TxStartSpeed\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.TxStartSpeed: %d\n", __FUNCTION__, Service.TxStartSpeed);

        switch (Service.TxStartSpeed)
        {
            case 0:
                Service.buff_TxStartSpeed = T30_SPEED_V34_33600;
                break;

            case 1:
                Service.buff_TxStartSpeed = T30_SPEED_V17_14400;
                break;

            case 2:
                Service.buff_TxStartSpeed = T30_SPEED_V29_9600;
                break;

            case 3:
                Service.buff_TxStartSpeed = T30_SPEED_V27_4800;
                break;

            default:
                Service.buff_TxStartSpeed = T30_SPEED_V34_33600;
                break;
        }

        Fax_API_T30Attribute(SET, FXA_TXSTARTSPEED, &Service.buff_TxStartSpeed);
    }

    // Step 2-2. Rx Start Speed
    Service.RxStartSpeed = db_get_var_int("CONF_FAXSP", "RxStartSpeed", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> RxStartSpeed\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.RxStartSpeed: %d\n", __FUNCTION__, Service.RxStartSpeed);

        switch (Service.RxStartSpeed)
        {
            case 0:
#if __OVERRUN_ERROR__
                Service.buff_RxStartSpeed = T30_SPEED_V34_26400;
#else
                Service.buff_RxStartSpeed = T30_SPEED_V34_28800;
#endif
                break;

            case 1:
                Service.buff_RxStartSpeed = T30_SPEED_V17_14400;
                break;

            case 2:
                Service.buff_RxStartSpeed = T30_SPEED_V29_7200;
                break;

            case 3:
                Service.buff_RxStartSpeed = T30_SPEED_V27_4800;
                break;

            default:
                Service.buff_RxStartSpeed = T30_SPEED_V34_33600;
                break;
        }

        Fax_API_T30Attribute(SET, FXA_RXSTARTSPEED, &Service.buff_RxStartSpeed);
    }

    // Step 2-3. TxEncoding
    Service.TxEncoding = db_get_var_int("CONF_FAXSP", "TxEncoding", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TxEncoding\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.TxEncoding: %d\n", __FUNCTION__, Service.TxEncoding);

        switch (Service.TxEncoding)
        {
            case 0: // MH/MR/MMR/JBIG
                Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_FALSE);
                Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_FALSE);
                Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_FALSE);		// 1:(not JBIG), 0:(use JBIG)
                Fax_API_T30Attribute(SET, FXA_IGNOREJBIGL0, &buff_TRUE);
                break;

            case 1: // MH/MR/MMR
                Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_FALSE);
                Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_FALSE);
                Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_TRUE);		// 1:(not JBIG), 0:(use JBIG)
                break;

            case 2: // MH/MR
                Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_FALSE);
                Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_TRUE);
                Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_TRUE);		// 1:(not JBIG), 0:(use JBIG)
                break;

            case 3: // MH
                Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_TRUE);
                Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_TRUE);
                Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_TRUE);		// 1:(not JBIG), 0:(use JBIG)
                break;

            default: // MH/MR/MMR/JBIG
                Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_FALSE);
                Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_FALSE);
                Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_FALSE);		// 1:(not JBIG), 0:(use JBIG)
                Fax_API_T30Attribute(SET, FXA_IGNOREJBIGL0, &buff_TRUE);
                break;
        }
    }

    // Step 2-4. Transmit Level
    Service.TransmitLevel = db_get_var_int("CONF_FAXSP", "TransmitLevel", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TransmitLevel\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.TransmitLevel: %d\n", __FUNCTION__, Service.TransmitLevel);

        if ((Service.TransmitLevel >= 4) && (Service.TransmitLevel <= 15))
        {
            // There is nothing to do..
        }
        else
        {
            Service.TransmitLevel = 10;
        }

        Service.buff_TransmitLevel = (UINT8)(Service.TransmitLevel * 10);

        Fax_API_T30Attribute (SET, FXA_ATTENUATION, &Service.buff_TransmitLevel);
    }

    // Step 2-5. Min. Detectable Tip Ring Volt.
    Service.MinDetectableTipRingVolt = db_get_var_int("CONF_FAXSP", "MinDetectableTipRingVolt", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> MinDetectableTipRingVolt\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.MinDetectableTipRingVolt: %d\n", __FUNCTION__, Service.MinDetectableTipRingVolt);

        MinDetectableTipRingVolt = (UINT8)Service.MinDetectableTipRingVolt;

        if(MinDetectableTipRingVolt != 18)
        {
            SetPhoneParams(PHONE_REF_LINEINUSE_VOLTAGE, (UINT16) MinDetectableTipRingVolt);
        }
    }

    // Step 2-6. Dial Timeout
    Service.DialTimeout = db_get_var_int("CONF_FAXSP", "DialTimeout", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> DialTimeout\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.DialTimeout: %d\n", __FUNCTION__, Service.DialTimeout);

        if ((Service.DialTimeout >= 0) && (Service.DialTimeout <= 100))
        {
            // There is nothing to do..
        }
        else
        {
            Service.DialTimeout = 0; // Set to default..
        }

        DialTimeout = (UINT16)(Service.DialTimeout * 1000);
    }

    // Step 2-7. Dial Tone Detection
    Service.DialToneDetection = db_get_var_int("CONF_FAXSP", "DialToneDetection", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> DialToneDetection\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.DialToneDetection: %d\n", __FUNCTION__, Service.DialToneDetection);

        if (Service.DialToneDetection == 1)
        {
            SetPhoneParams(PHONE_BLIND_DIAL, TRUE);
        }
        else
        {
            SetPhoneParams(PHONE_BLIND_DIAL, FALSE);
        }
    }

    // Step 2-8. CED Detection Time
    Service.CEDDetectionTime = db_get_var_int("CONF_FAXSP", "CEDDetectionTime", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> CEDDetectionTime\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.CEDDetectionTime: %d\n", __FUNCTION__, Service.CEDDetectionTime);

        if ((Service.CEDDetectionTime >= 30) && (Service.CEDDetectionTime <= 45))
        {
            // There is nothing to do..
        }
        else
        {
            Service.CEDDetectionTime = 38;
        }

        gFaxCEDDetectionTime = (UINT32)(Service.CEDDetectionTime * 1000);
    }

    // Step 2-9. T1 Timeout
    Service.T1Timeout = db_get_var_int("CONF_FAXSP", "T1Timeout", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> T1Timeout\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.T1Timeout: %d\n", __FUNCTION__, Service.T1Timeout);

        if ((Service.T1Timeout >= 30) && (Service.T1Timeout <= 45))
        {
            // There is nothing to do..
        }
        else
        {
            Service.T1Timeout = 38;
        }

        gFaxT1Timeout = (UINT32)(Service.T1Timeout * 1000);
    }

    // Step 2-10. T5 Timeout
    Service.T5Timeout = db_get_var_int("CONF_FAXSP", "T5Timeout", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> T5Timeout\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.T5Timeout: %d\n", __FUNCTION__, Service.T5Timeout);

        if ((Service.T5Timeout >= 50) && (Service.T5Timeout <= 70))
        {
            // There is nothing to do..
        }
        else
        {
            Service.T5Timeout = 60;
        }

        gFaxT5Timeout = (UINT32)(Service.T5Timeout * 1000);
    }

    // Step 2-11. TCF Error Rate
    Service.TCFErrorRate = db_get_var_int("CONF_FAXSP", "TCFErrorRate", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TCFErrorRate\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.TCFErrorRate: %d\n", __FUNCTION__, Service.TCFErrorRate);

        if ((Service.TCFErrorRate >= 10) && (Service.TCFErrorRate <= 30))
        {
            // There is nothing to do..
        }
        else
        {
            Service.TCFErrorRate = 10;
        }

        Service.buff_TCFErrorRate = (UINT8)Service.TCFErrorRate;

        Fax_API_T30Attribute(SET, FXA_TCFERRORS, &Service.buff_TCFErrorRate);
    }

    // Step 2-12. No. Of Tries On Fallback
    Service.NoOfTriesOnFallback = db_get_var_int("CONF_FAXSP", "NoOfTriesOnFallback", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> NoOfTriesOnFallback\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.NoOfTriesOnFallback: %d\n", __FUNCTION__, Service.NoOfTriesOnFallback);

        if ((Service.NoOfTriesOnFallback >= 1) && (Service.NoOfTriesOnFallback <= 3))
        {
            // There is nothing to do..
        }
        else
        {
            Service.NoOfTriesOnFallback = 1;
        }

        Service.buff_NoOfTriesOnFallback = (UINT8)Service.NoOfTriesOnFallback;

        Fax_API_T30Attribute(SET, FXA_FALLBACK_NOOFTRIES, &Service.buff_NoOfTriesOnFallback);
    }

    // Step 2-13. Receive Max. Gain
    Service.ReceiveMaxGain = db_get_var_int("CONF_FAXSP", "ReceiveMaxGain", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> ReceiveMaxGain\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.ReceiveMaxGain: %d\n", __FUNCTION__, Service.ReceiveMaxGain);

        if ((Service.ReceiveMaxGain >= 1) && (Service.ReceiveMaxGain <= 3))
        {
            // There is nothing to do..
        }
        else
        {
            Service.ReceiveMaxGain = 1;
        }

        Service.buff_ReceiveMaxGain = (UINT8)Service.ReceiveMaxGain;

        Fax_API_T30Attribute(SET, FXA_RXMAXGAIN, &Service.buff_ReceiveMaxGain);
    }

    // 2-14. Delay for Pause Key
    Service.DelayForPauseKey = db_get_var_int("CONF_FAXSP", "DelayForPauseKey", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> DelayForPauseKey\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.DelayForPauseKey: %d\n", __FUNCTION__, Service.DelayForPauseKey);

        if ((Service.DelayForPauseKey >= 1) && (Service.DelayForPauseKey <= 5))
        {
            // There is nothing to do..
        }
        else
        {
            Service.DelayForPauseKey = 1;
        }

        gFaxDelayForPauseKey = (UINT16)(Service.DelayForPauseKey * 1000);
    }

    // 2-15. Line In Use Threshold
    Service.LineInUseThreshold = db_get_var_int("CONF_FAXSP", "LineInUseThreshold", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> LineInUseThreshold\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.LineInUseThreshold: %d\n", __FUNCTION__, Service.LineInUseThreshold);

        if ((Service.LineInUseThreshold >= 0) && (Service.LineInUseThreshold <= 50))
        {
            // There is nothing to do..
        }
        else
        {
            Service.LineInUseThreshold = 0;
        }

        gFaxLineInUseThreshold = (UINT8)Service.LineInUseThreshold;
        if(gFaxLineInUseThreshold != 0)
        {
            SetPhoneParams(PHONE_LINE_IN_USE_THRESHOLD, (UINT16)gFaxLineInUseThreshold);
        }
    }

    // 2-15. Solution Fax
    Service.FaxSolution = db_get_var_int("CONF_SOLUTIONSP", "FaxSolution", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_SOLUTIONSP -> FaxSolution\n", __FUNCTION__);
        
        //temp ...
        //faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.FaxSolution: %d\n", __FUNCTION__, Service.FaxSolution);

        isSolutionFax = Service.FaxSolution;
    }

    // 2-15. Image Log
    Service.ImageLog = db_get_var_int("CONF_IMAGELOG", "IsEnabled", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_IMAGELOG -> IsEnabled\n", __FUNCTION__);
        
        //temp ...
        //faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.FaxSolution: %d\n", __FUNCTION__, Service.ImageLog);

        isImageLog = Service.ImageLog;
    }

#if __SUPPORT_CID__
    // 2-16. CID (Caller Identification)
    Service.CID = db_get_var_int("CONF_FAXSP", "CID", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> CID\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.CID: %d\n", __FUNCTION__, Service.CID);

        isCIDOn = Service.CID;
    }
#else
#endif

#if __TX_REPORT_IMAGE_PRINT__
    // 2-17. Tx Result Report Image
    Service.FaxTxResultRptImage = db_get_var_int("CONF_FAXUP", "TXResultRptImage", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TXResultRptImage\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.FaxTxResultRptImage: %d\n", __FUNCTION__, Service.FaxTxResultRptImage);

        if(gFaxTxResultRptImage == DISABLE)
        {
			tx_report_image_delete();
        }

        gFaxTxResultRptImage = Service.FaxTxResultRptImage;
    }
#endif

    Service.RingDetect = db_get_var_int("CONF_FAXSP", "RingDetect", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> RingDetect\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.RingDetect: %d\n", __FUNCTION__, Service.RingDetect);

        gRingDetect = Service.RingDetect;
    }

    //ImageBackup Setting
    Service.ImageBackup = db_get_var_int("CONF_FAXSP", "FaxImageBackup", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> FaxImageBackup\n", __FUNCTION__);

        //faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }
    else
    {
        printf("\n(YHCHO) %s() - Service.ImageBackup: %d\n", __FUNCTION__, Service.ImageBackup);

		gImageBackup = Service.ImageBackup;
    }

    ////////////////////////////////
    // Step 3. Parameters for Reboot
    ////////////////////////////////

    // Step 3-1. Country Code
    _prevParam.CountryCode = faxapp_get_select_country_from_db();

    // Step 3-2. Dial Tone Detection
    _prevParam.DialToneDetection = Service.DialToneDetection;

    isFaxAppReady = 1;
}


/***********************************************************************
 *  Function: faxapp_update_parameter
 *
 *  Description:
 *    Update fax related parameters from DB
 *
 *  Inputs:
 *    UINT8 Param: Parameter to update
 *
 *  Output:
 *    none
 **********************************************************************/
void faxapp_update_parameter (UINT8 Param)
{
    error_type_t db_retval = FAIL;

    printf("\n(YHCHO) %s() - Param: %d\n", __FUNCTION__, Param);

    switch (Param)
    {
        ///////////////////////
        // 1. User Parameter //
        ///////////////////////

        case FAXAPP_UPDATE_PARAM_STATION_ID:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pStationID = (char *)NULL;

            pStationID = db_get_var("CONF_FAXUP", "StationID", &db_retval);

            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> StationID\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                memset(gFaxStationID, 0x20, sizeof(gFaxStationID));

                if (pStationID == (char *)NULL)
                {
                    gFaxStationID[40] = 0;
                }
                else
                {
                    Len = strlen(pStationID);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxStationID[Index] = pStationID[Index];
                    }

                    gFaxStationID[40] = 0;

                    MEM_FREE_AND_NULL(pStationID);
                }

                printf("\n(YHCHO) %s() - gFaxStationID: %s\n", __FUNCTION__, gFaxStationID);

                Fax_API_T30Attribute (SET, FXA_TEXTHEADER, gFaxStationID);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_STATION_NUMBER:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pStationNum = (char *)NULL;

            pStationNum = db_get_var("CONF_FAXUP", "StationNum", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> StationNum\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                memset(gFaxStationNo, 0x20, sizeof(gFaxStationNo));
                memset(gRealFaxStationNo, 0, sizeof(gRealFaxStationNo));

                if (pStationNum == (char *)NULL)
                {
                    gFaxStationNo[40] = 0;
                }
                else
                {
                    Len = strlen(pStationNum);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxStationNo[Index] = pStationNum[Index];
                        gRealFaxStationNo[Index] = pStationNum[Index];
                    }

                    gFaxStationNo[40] = 0;
                    gRealFaxStationNo[40] = 0;

                    MEM_FREE_AND_NULL(pStationNum);
                }

                printf("\n(YHCHO) %s() - gFaxStationNo: %s\n", __FUNCTION__, gFaxStationNo);

                Fax_API_T30Attribute(SET, FXA_LOCALID, gFaxStationNo);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_TTI:
        {
            int TTI = 0;
            UINT8 isTTIOn = 0;

            TTI = db_get_var_int("CONF_FAXUP", "TTI", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TTI\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TTI: %d\n", __FUNCTION__, TTI);

                isTTIOn = (UINT8)TTI;

                Fax_API_T30Attribute(SET, FXA_TTI_ENABLE, &isTTIOn);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_LINE_MONITOR:
        {
            UINT8 TxLineMonitor = 0;
            UINT8 RxLineMonitor = 0;
            UINT8 SpeakerVolume = 0;
            int Volume = 0;
            int LineMonitor = 0;

            LineMonitor = db_get_var_int("CONF_FAXUP", "LineMonitor", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> LineMonitor\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - LineMonitor: %d\n", __FUNCTION__, LineMonitor);

                gFaxLineMonitor = LineMonitor;

                if (LineMonitor == LINE_MONITOR_OFF)
                {
                    SpeakerVolume = 0;

                    TxLineMonitor = STATE_TX_PHASE_A;

                    RxLineMonitor = STATE_RX_PHASE_A;
                }
                else if (LineMonitor == LINE_MONITOR_ON_TILL_CONNECT)
                {
                    Volume = db_get_var_int("CONF_FAXUP", "Volume", &db_retval);
                    if (db_retval == FAIL)
                    {
                        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> Volume\n", __FUNCTION__);

                        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
                    }
                    else
                    {
                        printf("\n(YHCHO) %s() - Volume: %d\n", __FUNCTION__, Volume);

                        SpeakerVolume = (UINT8)Volume;

                        TxLineMonitor = STATE_TX_PHASE_C;

                        RxLineMonitor = STATE_RX_PHASE_C;
                    }
                }
                else // Always On
                {
                    Volume = db_get_var_int("CONF_FAXUP", "Volume", &db_retval);
                    if (db_retval == FAIL)
                    {
                        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> Volume\n", __FUNCTION__);

                        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
                    }
                    else
                    {
                        printf("\n(YHCHO) %s() - Volume: %d\n", __FUNCTION__, Volume);

                        SpeakerVolume = (UINT8)Volume;

                        TxLineMonitor = STATE_TX_DISCONN;

                        RxLineMonitor = STATE_RX_DISCONN;
                    }
                }

                Fax_API_T30Attribute (SET, FXA_SPEAKERVOLUME, &SpeakerVolume);

                Fax_API_T30Attribute (SET, FXA_SPEAKER_CALLMODE, &TxLineMonitor);

                Fax_API_T30Attribute (SET, FXA_SPEAKER_ANSWERMODE, &RxLineMonitor);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_VOLUME:
        {
            int Volume = 0;
            UINT8 SpeakerVolume = 0;

            printf("\n(YHCHO) %s() - gFaxLineMonitor: %d\n", __FUNCTION__, gFaxLineMonitor);

            Volume = db_get_var_int("CONF_FAXUP", "Volume", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> Volume\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - Volume: %d\n", __FUNCTION__, Volume);

                if (gFaxLineMonitor == LINE_MONITOR_OFF)
                {
                    SpeakerVolume = 0;
                }
                else
                {
                    SpeakerVolume = (UINT8)Volume;
                }

                Fax_API_T30Attribute (SET, FXA_SPEAKERVOLUME, &SpeakerVolume);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_TX_RETRY_NUMBER:
        {
            UINT8 RetryNum = 0;
            int TxRetryNum = 0;

            TxRetryNum = db_get_var_int("CONF_FAXUP", "TxRetryNum", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TxRetryNum\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TxRetryNum: %d\n", __FUNCTION__, TxRetryNum);

                RetryNum = (UINT8)TxRetryNum + 1;

                Fax_API_T30Attribute(SET, FXA_RETRY_NUMBER, &RetryNum);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_TX_RETRY_INTERVAL:
        {
            UINT8 RetryWaitTime = 0;
            int TxRetryInterval = 0;

            TxRetryInterval = db_get_var_int("CONF_FAXUP", "TxRetryInterval", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TxRetryInterval\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TxRetryInterval: %d\n", __FUNCTION__, TxRetryInterval);

                RetryWaitTime = (UINT8)TxRetryInterval;

                Fax_API_T30Attribute(SET, FXA_RETRY_INTERVAL, &RetryWaitTime);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_ECM:
        {
            UINT8 isECMOn = 0;
            int TxECM = 0;

            TxECM = db_get_var_int("CONF_FAXUP", "TxECM", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TxECM\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TxECM: %d\n", __FUNCTION__, TxECM);

                isECMOn = (UINT8)TxECM;

                Fax_API_T30Attribute (SET, FXA_ECMMODE, &isECMOn);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_FAX_FORWARD:
        {
            UINT8 isFaxForwardOn = 0;
            int RxForwardSet = 0;

            RxForwardSet = db_get_var_int("CONF_FAXUP", "RxForwardSet", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> RxForwardSet\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - RxForwardSet: %d\n", __FUNCTION__, RxForwardSet);

                isFaxForwardOn = (UINT8)RxForwardSet;

                Fax_API_T30Attribute(SET, FXA_FAXFORWARD, &isFaxForwardOn);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_FORWARD_NUMBER:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            UINT8 RxForwardNum[LEN_PHONE_NO + 1] = {0, };
            char *pRxForwardNum = (char *)NULL;

            pRxForwardNum = db_get_var("CONF_FAXUP", "RxForwardNum", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> RxForwardNum\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                memset(RxForwardNum, 0x00, sizeof(RxForwardNum));

                if (pRxForwardNum == (char *)NULL)
                {
                    RxForwardNum[0] = 0;
                }
                else
                {
                    Len = strlen(pRxForwardNum);

                    for (Index = 0; Index < LEN_PHONE_NO; Index++)
                    {
                        RxForwardNum[Index] = pRxForwardNum[Index];
                    }

                    RxForwardNum[Index] = 0;

                    MEM_FREE_AND_NULL(pRxForwardNum);

                    printf("\n(YHCHO) %s() - RxForwardNum: %s\n", __FUNCTION__, RxForwardNum);

                    Fax_API_T30Attribute(SET, FXA_FAXFORWARD_NO, RxForwardNum);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_CONFIDENTIAL_RX:
        {
            int RxConfSet = 0;
            BOOLEAN isRxConfOn = FALSE;

            RxConfSet = db_get_var_int("CONF_FAXUP", "RxConfSet", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> RxConfSet\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - RxConfSet: %d\n", __FUNCTION__, RxConfSet);

                if (RxConfSet == DISABLE)
                {
                    isRxConfOn = FALSE;
                    Fax_API_T30Attribute(SET, FXA_RXREQUIREPASSWORD, &isRxConfOn);
                }
                else
                {
                    isRxConfOn = TRUE;
                    Fax_API_T30Attribute(SET, FXA_RXREQUIREPASSWORD, &isRxConfOn);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_PASSWORD_FOR_CONFIDENTIAL_RX:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            UINT8 RxConfPW[5] = {0, };
            char *pRxConfPW = (char *)NULL;
            UINT8 buff_RxConfPW[21] = {0, };

            pRxConfPW = db_get_var("CONF_FAXUP", "RxConfPW", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> RxConfPW\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                memset(RxConfPW, 0x20, sizeof(RxConfPW));

                if (pRxConfPW == (char *)NULL)
                {
                    buff_RxConfPW[20] = 0;
                }
                else
                {
                    Len = strlen(pRxConfPW);

                    for (Index = 0; Index < Len; Index++)
                    {
                        RxConfPW[Index] = pRxConfPW[Index];
                    }

                    RxConfPW[4] = 0;

                    //printf("\n(YHCHO) %s() - RxConfPW: %s\n", __FUNCTION__, RxConfPW);

                    memset(buff_RxConfPW, 0x20, sizeof(buff_RxConfPW));

                    for( Index = 0; Index < 4; Index++ )
                    {
                        buff_RxConfPW[16 + Index] = RxConfPW[Index];
                    }

                    for( Index = 0; Index < 16; Index++ )
                    {
                        buff_RxConfPW[Index] = 0x20;
                    }

                    buff_RxConfPW[20] = 0;

                    MEM_FREE_AND_NULL(pRxConfPW);
                }

                printf("\n(YHCHO) %s() - buff_RxConfPW: %s\n", __FUNCTION__, buff_RxConfPW);

                Fax_API_T30Attribute(SET, FXA_RXPASSWORD, buff_RxConfPW);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_RX_RING_NUM:
        {
            UINT8 NoOfRings = 0;
            int RxRingNum = 0;

            RxRingNum = db_get_var_int("CONF_FAXUP", "RxRingNum", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... RxRingNum\n", __FUNCTION__);
            }
            else
            {
                printf("\n(YHCHO) %s() - RxRingNum: %d\n", __FUNCTION__, RxRingNum);

                if (RxRingNum != 1)
                {
                    RxRingNum--;
                }

                NoOfRings = (UINT8)RxRingNum;

                Fax_API_T30Attribute(SET, FXA_AUTOANSWERRINGS, &NoOfRings);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_RX_REDUCTION_SEL:
        {
            int RxReductionSel = 0;

            RxReductionSel = db_get_var_int("CONF_FAXUP", "RxReductionSel", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... RxReductionSel\n", __FUNCTION__);
            }
            else
            {
                printf("\n(YHCHO) %s() - RxReductionSel: %d\n", __FUNCTION__, RxReductionSel);

                isAutoReductionOn = RxReductionSel;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_CONFIDENTIAL_TX:
        {
            int TxConfSet = 0;
            BOOLEAN isTxConfOn = FALSE;

            TxConfSet = db_get_var_int("CONF_FAXUP", "TxConfSet", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... TxConfSet\n", __FUNCTION__);
            }
            else
            {
                printf("\n(YHCHO) %s() - TxConfSet: %d\n", __FUNCTION__, TxConfSet);

                if (TxConfSet == DISABLE)
                {
                    isTxConfOn = FALSE;
                    Fax_API_T30Attribute(SET, FXA_TXREQUIREPASSWORD, &isTxConfOn);
                }
                else
                {
                    isTxConfOn = TRUE;
                    Fax_API_T30Attribute(SET, FXA_TXREQUIREPASSWORD, &isTxConfOn);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_PASSWORD_FOR_CONFIDENTIAL_TX:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            UINT8 TxConfPW[5] = {0, };
            char *pTxConfPW = (char *)NULL;
            UINT8 buff_TxConfPW[21] = {0, };

            pTxConfPW = db_get_var("CONF_FAXUP", "TxConfPW", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... CONF_FAXUP -> TxConfPW\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                memset(TxConfPW, 0x20, sizeof(TxConfPW));

                if (pTxConfPW == (char *)NULL)
                {
                    buff_TxConfPW[20] = 0;
                }
                else
                {
                    Len = strlen(pTxConfPW);

                    for (Index = 0; Index < Len; Index++)
                    {
                        TxConfPW[Index] = pTxConfPW[Index];
                    }

                    TxConfPW[4] = 0;

                    //printf("\n(YHCHO) %s() - TxConfPW: %s\n", __FUNCTION__, TxConfPW);

                    memset(buff_TxConfPW, 0x20, sizeof(buff_TxConfPW));

                    for( Index = 0; Index < 4; Index++ )
                    {
                        buff_TxConfPW[16 + Index] = TxConfPW[Index];
                    }

                    for( Index = 0; Index < 16; Index++ )
                    {
                        buff_TxConfPW[Index] = 0x20;
                    }

                    buff_TxConfPW[20] = 0;

                    MEM_FREE_AND_NULL(pTxConfPW);
                }

                printf("\n(YHCHO) %s() - buff_TxConfPW: %s\n", __FUNCTION__, buff_TxConfPW);

                Fax_API_T30Attribute(SET, FXA_TXPASSWORD, buff_TxConfPW);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_PRINT_MODE:
        {
            int PrintMode = 0;

            PrintMode = db_get_var_int("CONF_FAXUP", "PrintMode", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> PrintMode\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - PrintMode: %d\n", __FUNCTION__, PrintMode);

                gFaxPrintMode = PrintMode;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_DISCARD_LENGTH:
        {
            int buff_DiscardLength = 0;

            buff_DiscardLength = db_get_var_int("CONF_FAXUP", "DiscardLength", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> DiscardLength\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - buff_DiscardLength: %d\n", __FUNCTION__, buff_DiscardLength);

                DiscardLength = buff_DiscardLength;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_DUPLEX_PRINT:
        {
            int DuplexPrint = 0;

            DuplexPrint = db_get_var_int("CONF_FAXUP", "DuplexPrint", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> DuplexPrint\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - DuplexPrint: %d\n", __FUNCTION__, DuplexPrint);

                isDuplexPrintOn = DuplexPrint;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_INPUT_TRAY:
        {
            int InputTray = 0;

            InputTray = db_get_var_int("CONF_FAXUP", "FaxInputTray", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> FaxInputTray\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - InputTray: %d\n", __FUNCTION__, InputTray);

                FaxInputTray = InputTray;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SAVE_FAX_FILE:
        {
            int Temp_SaveFaxFile = 0;

            Temp_SaveFaxFile = db_get_var_int("CONF_FAXUP", "SaveFaxFile", &db_retval);

            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> SaveFaxFile\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - Temp_SaveFaxFile: %d\n", __FUNCTION__, Temp_SaveFaxFile);

                SaveFaxFile = Temp_SaveFaxFile;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_FILE_FORMAT:
        {
            int Temp_FileFormat = 0;

            Temp_FileFormat = db_get_var_int("CONF_FAXUP", "FileFormat", &db_retval);

            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> FileFormat\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - Temp_FileFormat: %d\n", __FUNCTION__, Temp_FileFormat);

                FileFormat = Temp_FileFormat;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_PAGE_TYPE:
        {
            int Temp_PageType = 0;

            Temp_PageType = db_get_var_int("CONF_FAXUP", "PageType", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> PageType\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - Temp_PageType: %d\n", __FUNCTION__, Temp_PageType);

                PageType = Temp_PageType;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SEND_DESTINATION:
        {
            int SendDestination = 0;

            SendDestination = db_get_var_int("CONF_FAXUP", "SendDestination", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> SendDestination\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - SendDestination: %d\n", __FUNCTION__, SendDestination);

                gFaxSendDestination = SendDestination;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_EMAIL_SERVER_ADDRESS:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pEmailServerAddress = (char *)NULL;

            pEmailServerAddress = db_get_var("INST_FAXSOLUTION", "EmailServerAddress", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailServerAddress", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pEmailServerAddress == (char *)NULL)
                {
                    gFaxSenderServerAddress[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pEmailServerAddress: %s\n", __FUNCTION__, pEmailServerAddress);

                    memset(gFaxSenderServerAddress, 0x00, sizeof(gFaxSenderServerAddress));

                    Len = strlen(pEmailServerAddress);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxSenderServerAddress[Index] = pEmailServerAddress[Index];
                    }

                    gFaxSenderServerAddress[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxSenderServerAddress: %s\n", __FUNCTION__, gFaxSenderServerAddress);

                    MEM_FREE_AND_NULL(pEmailServerAddress);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_EMAIL_ID:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pEmailID = (char *)NULL;
            char *pUserName = (char *)NULL;

            pEmailID = db_get_var("INST_FAXSOLUTION", "EmailID", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailID\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pEmailID == (char *)NULL)
                {
                    gFaxEmailID[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pEmailID: %s\n", __FUNCTION__, pEmailID);

                    memset(gFaxEmailID, 0x00, sizeof(gFaxEmailID));

                    Len = strlen(pEmailID);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxEmailID[Index] = pEmailID[Index];
                    }

                    gFaxEmailID[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxEmailID: %s\n", __FUNCTION__, gFaxEmailID);

                    MEM_FREE_AND_NULL(pEmailID);
                }
            }

			//user name
            pUserName = db_get_var("INST_FAXSOLUTION", "UserName", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> UserName\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pUserName == (char *)NULL)
                {
                    gFaxEmailUserName[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pEmailID: %s\n", __FUNCTION__, pEmailID);

                    memset(gFaxEmailUserName, 0x00, sizeof(gFaxEmailUserName));

                    Len = strlen(pUserName);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxEmailUserName[Index] = pUserName[Index];
                    }

                    gFaxEmailUserName[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxEmailUserName: %s\n", __FUNCTION__, gFaxEmailUserName);

                    MEM_FREE_AND_NULL(pUserName);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_EMAIL_PW:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pEmailPW = (char *)NULL;

            pEmailPW = db_get_var("INST_FAXSOLUTION", "EmailPW", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailPW\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pEmailPW == (char *)NULL)
                {
                    gFaxEmailPW[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pEmailPW: %s\n", __FUNCTION__, pEmailPW);

                    memset(gFaxEmailPW, 0x00, sizeof(gFaxEmailPW));

                    Len = strlen(pEmailPW);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxEmailPW[Index] = pEmailPW[Index];
                    }

                    gFaxEmailPW[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxEmailPW: %s\n", __FUNCTION__, gFaxEmailPW);

                    MEM_FREE_AND_NULL(pEmailPW);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_EMAIL_PORT:
        {
            int EmailPort = 0;

            EmailPort = db_get_var_int("INST_FAXSOLUTION", "EmailPort", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... INST_FAXSOLUTION -> EmailPort\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - EmailPort: %d\n", __FUNCTION__, EmailPort);

                gFaxEmailPort = EmailPort;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SENDER_EMAIL_ADDRESS:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pSenderEmailAddress = (char *)NULL;

            pSenderEmailAddress = db_get_var("INST_FAXSOLUTION", "SenderEmailAddress", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SenderEmailAddress\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pSenderEmailAddress == (char *)NULL)
                {
                    gFaxSenderEmailAddress[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pSenderEmailAddress: %s\n", __FUNCTION__, pSenderEmailAddress);

                    memset(gFaxSenderEmailAddress, 0x00, sizeof(gFaxSenderEmailAddress));

                    Len = strlen(pSenderEmailAddress);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxSenderEmailAddress[Index] = pSenderEmailAddress[Index];
                    }

                    gFaxSenderEmailAddress[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxSenderEmailAddress: %s\n", __FUNCTION__, gFaxSenderEmailAddress);

                    MEM_FREE_AND_NULL(pSenderEmailAddress);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_RECEIVER_EMAIL_ADDRESS:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pReceiverEmailAddress = (char *)NULL;

            pReceiverEmailAddress = db_get_var("INST_FAXSOLUTION", "ReceiverEmailAddress", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> ReceiverEmailAddress\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pReceiverEmailAddress == (char *)NULL)
                {
                    gFaxReceiverEmailAddress[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pReceiverEmailAddress: %s\n", __FUNCTION__, pReceiverEmailAddress);

                    memset(gFaxReceiverEmailAddress, 0x00, sizeof(gFaxReceiverEmailAddress));

                    Len = strlen(pReceiverEmailAddress);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxReceiverEmailAddress[Index] = pReceiverEmailAddress[Index];
                    }

                    gFaxReceiverEmailAddress[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxReceiverEmailAddress: %s\n", __FUNCTION__, gFaxReceiverEmailAddress);

                    MEM_FREE_AND_NULL(pReceiverEmailAddress);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_EMAIL_SUBJECT:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pEmailSubject = (char *)NULL;

            pEmailSubject = db_get_var("INST_FAXSOLUTION", "EmailSubject", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailSubject\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pEmailSubject == (char *)NULL)
                {
                    gFaxEmailSubject[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pEmailSubject: %s\n", __FUNCTION__, pEmailSubject);

                    memset(gFaxEmailSubject, 0x00, sizeof(gFaxEmailSubject));

                    Len = strlen(pEmailSubject);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxEmailSubject[Index] = pEmailSubject[Index];
                    }

                    gFaxEmailSubject[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxEmailSubject: %s\n", __FUNCTION__, gFaxEmailSubject);

                    MEM_FREE_AND_NULL(pEmailSubject);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_EMAIL_TEXT:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pEmailText = (char *)NULL;

            pEmailText = db_get_var("INST_FAXSOLUTION", "EmailText", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> EmailText\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pEmailText == (char *)NULL)
                {
                    gFaxEmailText[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pEmailText: %s\n", __FUNCTION__, pEmailText);

                    memset(gFaxEmailText, 0x00, sizeof(gFaxEmailText));

                    Len = strlen(pEmailText);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxEmailText[Index] = pEmailText[Index];
                    }

                    gFaxEmailText[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxEmailText: %s\n", __FUNCTION__, gFaxEmailText);

                    MEM_FREE_AND_NULL(pEmailText);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_FTP_ID:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pFTPID = (char *)NULL;

            pFTPID = db_get_var("INST_FAXSOLUTION", "FTPID", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> FTPID\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pFTPID == (char *)NULL)
                {
                    gFaxFTPID[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pFTPID: %s\n", __FUNCTION__, pFTPID);

                    memset(gFaxFTPID, 0x00, sizeof(gFaxFTPID));

                    Len = strlen(pFTPID);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxFTPID[Index] = pFTPID[Index];
                    }

                    gFaxFTPID[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxFTPID: %s\n", __FUNCTION__, gFaxFTPID);

                    MEM_FREE_AND_NULL(pFTPID);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_FTP_PW:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pFTPPW = (char *)NULL;

            pFTPPW = db_get_var("INST_FAXSOLUTION", "FTPPW", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> FTPPW\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pFTPPW == (char *)NULL)
                {
                    gFaxFTPPW[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pFTPPW: %s\n", __FUNCTION__, pFTPPW);

                    memset(gFaxFTPPW, 0x00, sizeof(gFaxFTPPW));

                    Len = strlen(pFTPPW);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxFTPPW[Index] = pFTPPW[Index];
                    }

                    gFaxFTPPW[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxFTPPW: %s\n", __FUNCTION__, gFaxFTPPW);

                    MEM_FREE_AND_NULL(pFTPPW);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_FTP_ADDRESS:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pFTPAddress = (char *)NULL;

            pFTPAddress = db_get_var("INST_FAXSOLUTION", "FTPAddress", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> FTPAddress\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pFTPAddress == (char *)NULL)
                {
                    gFaxFTPAddress[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pFTPAddress: %s\n", __FUNCTION__, pFTPAddress);

                    memset(gFaxFTPAddress, 0x00, sizeof(gFaxFTPAddress));

                    Len = strlen(pFTPAddress);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxFTPAddress[Index] = pFTPAddress[Index];
                    }

                    gFaxFTPAddress[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxFTPAddress: %s\n", __FUNCTION__, gFaxFTPAddress);

                    MEM_FREE_AND_NULL(pFTPAddress);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_FTP_PATH:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pFTPPath = (char *)NULL;

            pFTPPath = db_get_var("INST_FAXSOLUTION", "FTPPath", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> FTPPath\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pFTPPath == (char *)NULL)
                {
                    gFaxFTPPath[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pFTPPath: %s\n", __FUNCTION__, pFTPPath);

                    memset(gFaxFTPPath, 0x00, sizeof(gFaxFTPPath));

                    Len = strlen(pFTPPath);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxFTPPath[Index] = pFTPPath[Index];
                    }

                    gFaxFTPPath[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxFTPPath: %s\n", __FUNCTION__, gFaxFTPPath);

                    MEM_FREE_AND_NULL(pFTPPath);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_FTP_PORT:
        {
            int FTPPort = 0;

            FTPPort = db_get_var_int("INST_FAXSOLUTION", "FTPPort", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... INST_FAXSOLUTION -> FTPPort\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - FTPPort: %d\n", __FUNCTION__, FTPPort);

                gFaxFTPPort = FTPPort;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SMB_ID:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pSMBID = (char *)NULL;

            pSMBID = db_get_var("INST_FAXSOLUTION", "SMBID", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SMBID\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pSMBID == (char *)NULL)
                {
                    gFaxSMBID[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pSMBID: %s\n", __FUNCTION__, pSMBID);

                    memset(gFaxSMBID, 0x00, sizeof(gFaxSMBID));

                    Len = strlen(pSMBID);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxSMBID[Index] = pSMBID[Index];
                    }

                    gFaxSMBID[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxSMBID: %s\n", __FUNCTION__, gFaxSMBID);

                    MEM_FREE_AND_NULL(pSMBID);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SMB_PW:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pSMBPW = (char *)NULL;

            pSMBPW = db_get_var("INST_FAXSOLUTION", "SMBPW", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SMBPW\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pSMBPW == (char *)NULL)
                {
                    gFaxSMBPW[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pSMBPW: %s\n", __FUNCTION__, pSMBPW);

                    memset(gFaxSMBPW, 0x00, sizeof(gFaxSMBPW));

                    Len = strlen(pSMBPW);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxSMBPW[Index] = pSMBPW[Index];
                    }

                    gFaxSMBPW[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxSMBPW: %s\n", __FUNCTION__, gFaxSMBPW);

                    MEM_FREE_AND_NULL(pSMBPW);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SMB_ADDRESS:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pSMBAddress = (char *)NULL;

            pSMBAddress = db_get_var("INST_FAXSOLUTION", "SMBAddress", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SMBAddress\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pSMBAddress == (char *)NULL)
                {
                    gFaxSMBAddress[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pSMBAddress: %s\n", __FUNCTION__, pSMBAddress);

                    memset(gFaxSMBAddress, 0x00, sizeof(gFaxSMBAddress));

                    Len = strlen(pSMBAddress);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxSMBAddress[Index] = pSMBAddress[Index];
                    }

                    gFaxSMBAddress[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxSMBAddress: %s\n", __FUNCTION__, gFaxSMBAddress);

                    MEM_FREE_AND_NULL(pSMBAddress);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SMB_PORT:
        {
            int SMBPort = 0;

            SMBPort = db_get_var_int("INST_FAXSOLUTION", "SMBPort", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... INST_FAXSOLUTION -> SMBPort\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - SMBPort: %d\n", __FUNCTION__, SMBPort);

                gFaxSMBPort = SMBPort;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SMB_PATH:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pSMBPath = (char *)NULL;

            pSMBPath = db_get_var("INST_FAXSOLUTION", "SMBPath", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> SMBPath\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pSMBPath == (char *)NULL)
                {
                    gFaxSMBPath[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pSMBPath: %s\n", __FUNCTION__, pSMBPath);

                    memset(gFaxSMBPath, 0x00, sizeof(gFaxSMBPath));

                    Len = strlen(pSMBPath);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxSMBPath[Index] = pSMBPath[Index];
                    }

                    gFaxSMBPath[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxSMBPath: %s\n", __FUNCTION__, gFaxSMBPath);

                    MEM_FREE_AND_NULL(pSMBPath);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_WEBDAV_ID:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pWebDavID = (char *)NULL;

            pWebDavID = db_get_var("INST_FAXSOLUTION", "WebDAVID", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> WebDAVID\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pWebDavID == (char *)NULL)
                {
                    gFaxWebDavID[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pWebDavID: %s\n", __FUNCTION__, pWebDavID);

                    memset(gFaxWebDavID, 0x00, sizeof(gFaxWebDavID));

                    Len = strlen(pWebDavID);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxWebDavID[Index] = pWebDavID[Index];
                    }

                    gFaxWebDavID[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxWebDavID: %s\n", __FUNCTION__, gFaxWebDavID);

                    MEM_FREE_AND_NULL(pWebDavID);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_WEBDAV_PW:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pWebDavPW = (char *)NULL;

            pWebDavPW = db_get_var("INST_FAXSOLUTION", "WebDAVPW", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> WebDAVPW\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pWebDavPW == (char *)NULL)
                {
                    gFaxWebDavPW[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pWebDavPW: %s\n", __FUNCTION__, pWebDavPW);

                    memset(gFaxWebDavPW, 0x00, sizeof(gFaxWebDavPW));

                    Len = strlen(pWebDavPW);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxWebDavPW[Index] = pWebDavPW[Index];
                    }

                    gFaxWebDavPW[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxWebDavPW: %s\n", __FUNCTION__, gFaxWebDavPW);

                    MEM_FREE_AND_NULL(pWebDavPW);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_WEBDAV_ADDRESS:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pWebDavAddress = (char *)NULL;

            pWebDavAddress = db_get_var("INST_FAXSOLUTION", "WebDAVAddress", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> WebDAVAddress\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pWebDavAddress == (char *)NULL)
                {
                    gFaxWebDavAddress[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pWebDavAddress: %s\n", __FUNCTION__, pWebDavAddress);

                    memset(gFaxWebDavAddress, 0x00, sizeof(gFaxWebDavAddress));

                    Len = strlen(pWebDavAddress);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxWebDavAddress[Index] = pWebDavAddress[Index];
                    }

                    gFaxWebDavAddress[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxWebDavAddress: %s\n", __FUNCTION__, gFaxWebDavAddress);

                    MEM_FREE_AND_NULL(pWebDavAddress);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_WEBDAV_PORT:
        {
            int WebDavPort = 0;

            WebDavPort = db_get_var_int("INST_FAXSOLUTION", "WebDAVPort", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... INST_FAXSOLUTION -> WebDAVPort\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - WebDavPort: %d\n", __FUNCTION__, WebDavPort);

                gFaxWebDavPort = WebDavPort;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_WEBDAV_PATH:
        {
            UINT8 Index = 0;
            UINT8 Len = 0;
            char *pWebDavPath = (char *)NULL;

            pWebDavPath = db_get_var("INST_FAXSOLUTION", "WebDAVPath", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var() Error... INST_FAXSOLUTION -> WebDAVPath\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                if (pWebDavPath == (char *)NULL)
                {
                    gFaxWebDavPath[0] = 0;
                }
                else
                {
                    //printf("\n(YHCHO) %s() - pWebDavPath: %s\n", __FUNCTION__, pWebDavPath);

                    memset(gFaxWebDavPath, 0x00, sizeof(gFaxWebDavPath));

                    Len = strlen(pWebDavPath);

                    for (Index = 0; Index < Len; Index++)
                    {
                        gFaxWebDavPath[Index] = pWebDavPath[Index];
                    }

                    gFaxWebDavPath[Index] = 0;

                    printf("\n(YHCHO) %s() - gFaxWebDavPath: %s\n", __FUNCTION__, gFaxWebDavPath);

                    MEM_FREE_AND_NULL(pWebDavPath);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_SPAM_FAX:
        {
            int SpamFax = 0;
            int NoOfSpamFax = 0;
            int Index = 0;
            FAXSPAM_STRUCT faxspam_result[MAX_SPAM_FAX_LIST];

            SpamFax = db_get_var_int("CONF_FAXUP", "SpamFax", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> SpamFax\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - SpamFax: %d\n", __FUNCTION__, SpamFax);

                isSpamFaxOn = (UINT8)SpamFax;

                db_retval = db_get_faxspamnumber(faxspam_result, &NoOfSpamFax);
                if (db_retval == FAIL)
                {
                    printf("\n(YHCHO) %s() - db_get_faxspamnumber() Error...\n", __FUNCTION__);

                    faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
                }
                else
                {
                    NoOfSpamFaxList = 0;

                    memset(gFaxSpamFaxList, 0x00, sizeof(gFaxSpamFaxList));

                    for(Index = 0; Index < NoOfSpamFax; Index++)
                    {
                        if (faxspam_result[Index].faxspamnumber[0] != 0)
                        {
                            snprintf(gFaxSpamFaxList[NoOfSpamFaxList], MAX_SPAM_FAX_LIST, "%s", faxspam_result[Index].faxspamnumber);

                            printf("\n(YHCHO) %s() - gFaxSpamFaxList[%d]: %s\n", __FUNCTION__, NoOfSpamFaxList, gFaxSpamFaxList[NoOfSpamFaxList]);

                            NoOfSpamFaxList++;
                        }
                    }
                }

                printf("\n(YHCHO) %s() - NoOfSpamFaxList: %d\n", __FUNCTION__, NoOfSpamFaxList);
            }

            break;
        }

#if __SUPPORT_MANUAL_RECEIVE__
        case FAXAPP_UPDATE_MANUAL_RECEIVE:
        {
            int ManualReceive = 0;
            UINT8 isManualReceiveOn = 0;
            UINT8 AnswerMode = 0;

            ManualReceive = db_get_var_int("CONF_FAXUP", "ManualReceive", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> ManualReceive\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - ManualReceive: %d\n", __FUNCTION__, ManualReceive);

                isManualReceiveOn = (UINT8)ManualReceive;

                Fax_API_T30Attribute(SET, FXA_MANUALRECEIVE, &isManualReceiveOn);

                if (isManualReceiveOn == 1)
                {
                    AnswerMode = MANUAL_ANSWER;
                }
                else
                {
                    AnswerMode = AUTO_ANSWER;
                }

                Fax_API_T30Attribute(SET, FXA_ANSWERMODE, &AnswerMode);
            }

            break;
        }

        case FAXAPP_UPDATE_REMOTE_ACCESS_CODE:
        {
            int TmpRemoteAccessCode = 0;

            TmpRemoteAccessCode = db_get_var_int("CONF_FAXUP", "RemoteAccessCode", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RemoteAccessCode\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TmpRemoteAccessCode: %d\n", __FUNCTION__, TmpRemoteAccessCode);

                memset(RemoteAccessCode, 0x00, sizeof(RemoteAccessCode));

                RemoteAccessCode[0] = '*';
                RemoteAccessCode[1] = TmpRemoteAccessCode + '0';
                RemoteAccessCode[2] = '*';
                RemoteAccessCode[3] = 0;

                printf("\n(YHCHO) %s() - RemoteAccessCode: %s\n", __FUNCTION__, RemoteAccessCode);
            }
            break;
        }
#else
#endif

        case FAXAPP_UPDATE_PARAM_FAXTOX_SET:
        {
            int FaxToXSet = 0;

            FaxToXSet = db_get_var_int("CONF_FAXUP", "FaxToXSet", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> FaxToXSet\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - FaxToXSet: %d\n", __FUNCTION__, FaxToXSet);

                gFaxToXSet = FaxToXSet;
            }

            break;
        }

        ///////////////////////////////
        // 2. Service Mode Parameter //
        ///////////////////////////////

        case FAXAPP_UPDATE_PARAM_TX_START_SPEED:
        {
            int TxStartSpeed = 0;
            UINT8 buff_TxStartSpeed = 0;

            TxStartSpeed = db_get_var_int("CONF_FAXSP", "TxStartSpeed", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> TxStartSpeed\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TxStartSpeed: %d\n", __FUNCTION__, TxStartSpeed);

                switch (TxStartSpeed)
                {
                    case 0:
                        buff_TxStartSpeed = T30_SPEED_V34_33600;
                        break;

                    case 1:
                        buff_TxStartSpeed = T30_SPEED_V17_14400;
                        break;

                    case 2:
                        buff_TxStartSpeed = T30_SPEED_V29_9600;
                        break;

                    case 3:
                        buff_TxStartSpeed = T30_SPEED_V27_4800;
                        break;

                    default:
                        buff_TxStartSpeed = T30_SPEED_V34_33600;
                        break;
                }

                Fax_API_T30Attribute(SET, FXA_TXSTARTSPEED, &buff_TxStartSpeed);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_RX_START_SPEED:
        {
            int RxStartSpeed = 0;
            UINT8 buff_RxStartSpeed = 0;

            RxStartSpeed = db_get_var_int("CONF_FAXSP", "RxStartSpeed", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> RxStartSpeed\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - RxStartSpeed: %d\n", __FUNCTION__, RxStartSpeed);

                switch (RxStartSpeed)
                {
                    case 0:
                        buff_RxStartSpeed = T30_SPEED_V34_33600;
                        break;

                    case 1:
                        buff_RxStartSpeed = T30_SPEED_V17_14400;
                        break;

                    case 2:
                        buff_RxStartSpeed = T30_SPEED_V29_7200;
                        break;

                    case 3:
                        buff_RxStartSpeed = T30_SPEED_V27_4800;
                        break;

                    default:
                        buff_RxStartSpeed = T30_SPEED_V34_33600;
                        break;
                }

                Fax_API_T30Attribute(SET, FXA_RXSTARTSPEED, &buff_RxStartSpeed);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_ENCODING_TYPE:
        {
            BOOLEAN buff_FALSE = FALSE;
            BOOLEAN buff_TRUE = TRUE;
            int TxEncoding = 0;

            TxEncoding = db_get_var_int("CONF_FAXSP", "TxEncoding", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> TxEncoding\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TxEncoding: %d\n", __FUNCTION__, TxEncoding);

                switch (TxEncoding)
                {
                    case 0: // MH/MR/MMR/JBIG
                        Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_FALSE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_FALSE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_FALSE);		// 1:(not JBIG), 0:(use JBIG)
                        Fax_API_T30Attribute(SET, FXA_IGNOREJBIGL0, &buff_TRUE);
                        break;

                    case 1: // MH/MR/MMR
                        Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_FALSE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_FALSE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_TRUE);		// 1:(not JBIG), 0:(use JBIG)
                        break;

                    case 2: // MH/MR
                        Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_FALSE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_TRUE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_TRUE);		// 1:(not JBIG), 0:(use JBIG)
                        break;

                    case 3: // MH
                        Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_TRUE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_TRUE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_TRUE);		// 1:(not JBIG), 0:(use JBIG)
                        break;

                    default: // MH/MR/MMR/JBIG
                        Fax_API_T30Attribute(SET, FXA_IGNOREMR, &buff_FALSE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREMMR, &buff_FALSE);
                        Fax_API_T30Attribute(SET, FXA_IGNOREJBIG, &buff_FALSE);		// 1:(not JBIG), 0:(use JBIG)
                        Fax_API_T30Attribute(SET, FXA_IGNOREJBIGL0, &buff_TRUE);
                        break;
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_COUNTRY_CODE:
        {
            int CountryCode = 0;

            CountryCode = faxapp_get_select_country_from_db();

            if (_prevParam.CountryCode != CountryCode)
            {
                gFaxRebootCnt++;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_TRANSMIT_LEVEL:
        {
            int TransmitLevel = 0;
            UINT8 buff_TransmitLevel = 0;

            TransmitLevel = db_get_var_int("CONF_FAXSP", "TransmitLevel", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> TransmitLevel\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TransmitLevel: %d\n", __FUNCTION__, TransmitLevel);

                if ((TransmitLevel >= 4) && (TransmitLevel <= 15))
                {
                    // There is nothing to do..
                }
                else
                {
                    TransmitLevel = 10;
                }

                buff_TransmitLevel = (UINT8)(TransmitLevel * 10);

                Fax_API_T30Attribute (SET, FXA_ATTENUATION, &buff_TransmitLevel);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_DTMF_LOW_LEVEL:
        {
            int DTMFLowLevel = 0;

            DTMFLowLevel = db_get_var_int("CONF_FAXSP", "DTMFLowLevel", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> DTMFLowLevel\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - DTMFLowLevel: %d\n", __FUNCTION__, DTMFLowLevel);

                SetPhoneParams(PHONE_DTMF_LOW_LEVEL, (UINT16) DTMFLowLevel);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_DTMF_HIGH_LEVEL:
        {
            int DTMFHighLevel = 0;

            DTMFHighLevel = db_get_var_int("CONF_FAXSP", "DTMFHighLevel", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> DTMFHighLevel\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - DTMFHighLevel: %d\n", __FUNCTION__, DTMFHighLevel);

                SetPhoneParams(PHONE_DTMF_HIGH_LEVEL, (UINT16) DTMFHighLevel);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_PULSE_MAKE_TIME:
        {
            int PulseMakeTime = 0;

            PulseMakeTime = db_get_var_int("CONF_FAXSP", "PulseMakeTime", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> PulseMakeTime\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - PulseMakeTime: %d\n", __FUNCTION__, PulseMakeTime);

                SetPhoneParams(PHONE_PULSE_MAKE_TIME, (UINT16) PulseMakeTime);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_PULSE_BREAK_TIME:
        {
            int PulseBreakTime = 0;

            PulseBreakTime = db_get_var_int("CONF_FAXSP", "PulseBreakTime", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> PulseBreakTime\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - PulseBreakTime: %d\n", __FUNCTION__, PulseBreakTime);

                SetPhoneParams(PHONE_PULSE_BREAK_TIME, (UINT16) PulseBreakTime);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_INTER_DIGIT_DELAY:
        {
            int InterdigitDelay = 0;

            InterdigitDelay = db_get_var_int("CONF_FAXSP", "InterdigitDelay", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> InterdigitDelay\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - InterdigitDelay: %d\n", __FUNCTION__, InterdigitDelay);

                SetPhoneParams(PHONE_INTERDIGIT_DELAY, (UINT16) InterdigitDelay);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_DC_CHARACTERISTICS:
        {
            int DCCharacteristic = 0;

            DCCharacteristic = db_get_var_int("CONF_FAXSP", "DCCharacteristic", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> DCCharacteristic\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - DCCharacteristic: %d\n", __FUNCTION__, DCCharacteristic);

                SetPhoneParams(PHONE_DC_CHARACTERISTIC, (UINT16) DCCharacteristic);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_IMPEDANCE:
        {
            int Impedance = 0;

            Impedance = db_get_var_int("CONF_FAXSP", "Impedance", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> Impedance\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - Impedance: %d\n", __FUNCTION__, Impedance);

                SetPhoneParams(PHONE_IMPEDANCE, (UINT16) Impedance);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_BUSY_TONE_CYCLE:
        {
            int BusytoneCycle = 0;

            BusytoneCycle = db_get_var_int("CONF_FAXSP", "BusytoneCycle", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> BusytoneCycle\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - BusytoneCycle: %d\n", __FUNCTION__, BusytoneCycle);

                SetPhoneParams(PHONE_BUSY_TONE_CYCLE, (UINT16) BusytoneCycle);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_BUSY_TONE_MIN_ON_TIME:
        {
            int BusytoneMinOnTime = 0;

            BusytoneMinOnTime = db_get_var_int("CONF_FAXSP", "BusytoneMinOnTime", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> BusytoneMinOnTime\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - BusytoneMinOnTime: %d\n", __FUNCTION__, BusytoneMinOnTime);

                SetPhoneParams(PHONE_BUSY_TONE_MIN_ON_TIME, (UINT16) BusytoneMinOnTime);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_BUSY_TONE_MAX_ON_TIME:
        {
            int BusytoneMaxOnTime = 0;

            BusytoneMaxOnTime = db_get_var_int("CONF_FAXSP", "BusytoneMaxOnTime", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> BusytoneMaxOnTime\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - BusytoneMaxOnTime: %d\n", __FUNCTION__, BusytoneMaxOnTime);

                SetPhoneParams(PHONE_BUSY_TONE_MAX_ON_TIME, (UINT16) BusytoneMaxOnTime);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_BUSY_TONE_MIN_OFF_TIME:
        {
            int BusytoneMinOffTime = 0;

            BusytoneMinOffTime = db_get_var_int("CONF_FAXSP", "BusytoneMinOffTime", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> BusytoneMinOffTime\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - BusytoneMinOffTime: %d\n", __FUNCTION__, BusytoneMinOffTime);

                SetPhoneParams(PHONE_BUSY_TONE_MIN_OFF_TIME, (UINT16) BusytoneMinOffTime);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_BUSY_TONE_MAX_OFF_TIME:
        {
            int BusytoneMaxOffTime = 0;

            BusytoneMaxOffTime = db_get_var_int("CONF_FAXSP", "BusytoneMaxOffTime", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> BusytoneMaxOffTime\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - BusytoneMaxOffTime: %d\n", __FUNCTION__, BusytoneMaxOffTime);

                SetPhoneParams(PHONE_BUSY_TONE_MAX_OFF_TIME, (UINT16) BusytoneMaxOffTime);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_EXTENSION_SUPPORT:
        {
            int ExtensionSupport = 0;

            ExtensionSupport = db_get_var_int("CONF_FAXSP", "ExtensionSupport", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> ExtensionSupport\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - ExtensionSupport: %d\n", __FUNCTION__, ExtensionSupport);

                SetPhoneParams(PHONE_EXTENSION_SUPPORT, (UINT16) ExtensionSupport);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_EXTENSION_VOLTAGE_THRESHOLD:
        {
            int ExtensionVoltageThreshold = 0;

            ExtensionVoltageThreshold = db_get_var_int("CONF_FAXSP", "ExtensionVoltageThreshold", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> ExtensionVoltageThreshold\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - ExtensionVoltageThreshold: %d\n", __FUNCTION__, ExtensionVoltageThreshold);

                SetPhoneParams(PHONE_EXTENSION_VOLTAGE_THRESHOLD, (UINT16) ExtensionVoltageThreshold);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_MIN_DETECTABLE_TIP_RING_VOLT:
        {
            int buff_MinDetectableTipRingVolt = 0;

            buff_MinDetectableTipRingVolt = db_get_var_int("CONF_FAXSP", "MinDetectableTipRingVolt", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> MinDetectableTipRingVolt\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - buff_MinDetectableTipRingVolt: %d\n", __FUNCTION__, buff_MinDetectableTipRingVolt);

                MinDetectableTipRingVolt = (UINT8)buff_MinDetectableTipRingVolt;

                if(MinDetectableTipRingVolt != 18)
                {
                    SetPhoneParams(PHONE_REF_LINEINUSE_VOLTAGE, (UINT16) MinDetectableTipRingVolt);
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_DIAL_TONE_DETECTION:
        {
            int DialToneDetection = 0;

            DialToneDetection = db_get_var_int("CONF_FAXSP", "DialToneDetection", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> DialToneDetection\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - DialToneDetection: %d\n", __FUNCTION__, DialToneDetection);

                if (_prevParam.DialToneDetection != DialToneDetection)
                {
                    gFaxRebootCnt++;
                }
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_DIAL_TIMEOUT:
        {
            int buff_DialTimeout = 0;

            buff_DialTimeout = db_get_var_int("CONF_FAXSP", "DialTimeout", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> DialTimeout\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - buff_DialTimeout: %d\n", __FUNCTION__, buff_DialTimeout);

                if ((buff_DialTimeout >= 0) && (buff_DialTimeout <= 100))
                {
                    // There is nothing to do..
                }
                else
                {
                    buff_DialTimeout = 0; // Set to default..
                }

                DialTimeout = (UINT16)(buff_DialTimeout * 1000);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_CED_DETECTION_TIME:
        {
            int CEDDetectionTime = 0;
            UINT8 buff_CEDDetectionTime = 0;

            CEDDetectionTime = db_get_var_int("CONF_FAXSP", "CEDDetectionTime", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> CEDDetectionTime\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - CEDDetectionTime: %d\n", __FUNCTION__, CEDDetectionTime);

                if ((CEDDetectionTime >= 30) && (CEDDetectionTime <= 45))
                {
                    // There is nothing to do..
                }
                else
                {
                    CEDDetectionTime = 38;
                }

                buff_CEDDetectionTime = (UINT8)CEDDetectionTime;

                Fax_API_T30Attribute(SET, FXA_CEDDETECT_TIMEOUT, &buff_CEDDetectionTime);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_T1_TIMEOUT:
        {
            int T1Timeout = 0;

            T1Timeout = db_get_var_int("CONF_FAXSP", "T1Timeout", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> T1Timeout\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - T1Timeout: %d\n", __FUNCTION__, T1Timeout);

                if ((T1Timeout >= 30) && (T1Timeout <= 45))
                {
                    // There is nothing to do..
                }
                else
                {
                    T1Timeout = 38;
                }

                gFaxT1Timeout = (UINT32)(T1Timeout * 1000);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_T5_TIMEOUT:
        {
            int T5Timeout = 0;

            T5Timeout = db_get_var_int("CONF_FAXSP", "T5Timeout", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> T5Timeout\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - T5Timeout: %d\n", __FUNCTION__, T5Timeout);

                if ((T5Timeout >= 50) && (T5Timeout <= 70))
                {
                    // There is nothing to do..
                }
                else
                {
                    T5Timeout = 60;
                }

                gFaxT5Timeout = (UINT32)(T5Timeout * 1000);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_TCF_ERROR_RATE:
        {
            int TCFErrorRate = 0;
            UINT8 buff_TCFErrorRate = 0;

            TCFErrorRate = db_get_var_int("CONF_FAXSP", "TCFErrorRate", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> TCFErrorRate\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TCFErrorRate: %d\n", __FUNCTION__, TCFErrorRate);

                if ((TCFErrorRate >= 10) && (TCFErrorRate <= 30))
                {
                    // There is nothing to do..
                }
                else
                {
                    TCFErrorRate = 10;
                }

                buff_TCFErrorRate = (UINT8)TCFErrorRate;

                Fax_API_T30Attribute(SET, FXA_TCFERRORS, &buff_TCFErrorRate);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_NO_OF_TRIES_ON_FALLBACK:
        {
            int NoOfTriesOnFallback = 0;
            UINT8 buff_NoOfTriesOnFallback = 0;

            NoOfTriesOnFallback = db_get_var_int("CONF_FAXSP", "NoOfTriesOnFallback", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> NoOfTriesOnFallback\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - NoOfTriesOnFallback: %d\n", __FUNCTION__, NoOfTriesOnFallback);

                if ((NoOfTriesOnFallback >= 1) && (NoOfTriesOnFallback <= 3))
                {
                    // There is nothing to do..
                }
                else
                {
                    NoOfTriesOnFallback = 1;
                }

                buff_NoOfTriesOnFallback = (UINT8)NoOfTriesOnFallback;

                Fax_API_T30Attribute(SET, FXA_FALLBACK_NOOFTRIES, &buff_NoOfTriesOnFallback);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_RECEIVE_MAX_GAIN:
        {
            int ReceiveMaxGain = 0;
            UINT8 buff_ReceiveMaxGain = 0;

            ReceiveMaxGain = db_get_var_int("CONF_FAXSP", "ReceiveMaxGain", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> ReceiveMaxGain\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - ReceiveMaxGain: %d\n", __FUNCTION__, ReceiveMaxGain);

                if ((ReceiveMaxGain >= 1) && (ReceiveMaxGain <= 3))
                {
                    // There is nothing to do..
                }
                else
                {
                    ReceiveMaxGain = 1;
                }

                buff_ReceiveMaxGain = (UINT8)ReceiveMaxGain;

                Fax_API_T30Attribute(SET, FXA_RXMAXGAIN, &buff_ReceiveMaxGain);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_DELAY_FOR_PAUSE_KEY:
        {
            int DelayForPauseKey = 0;

            DelayForPauseKey = db_get_var_int("CONF_FAXSP", "DelayForPauseKey", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> DelayForPauseKey\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - DelayForPauseKey: %d\n", __FUNCTION__, DelayForPauseKey);

                if ((DelayForPauseKey >= 1) && (DelayForPauseKey <= 5))
                {
                    // There is nothing to do..
                }
                else
                {
                    DelayForPauseKey = 1;
                }

                gFaxDelayForPauseKey = (UINT16)(DelayForPauseKey * 1000);
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_AUTO_DARKNESS:
        {
            int AutoDarkness = 0;

            AutoDarkness = db_get_var_int("CONF_FAXUP", "AutoDarkness", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> AutoDarkness\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - AutoDarkness: %d\n", __FUNCTION__, AutoDarkness);

                gFaxAutoDarkness = AutoDarkness;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_BACKGROUND_REMOVE:
        {
            int BgRemove = 0;

            BgRemove = db_get_var_int("CONF_FAXUP", "BgRemove", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> BgRemove\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - BgRemove: %d\n", __FUNCTION__, BgRemove);

                gFaxBgRemove = BgRemove;
            }

            break;
        }

        case FAXAPP_UPDATE_PARAM_LINE_IN_USE_THRESHOLD:
        {
            int LineInUseThreshold = 0;

            LineInUseThreshold = db_get_var_int("CONF_FAXSP", "LineInUseThreshold", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> LineInUseThreshold\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - LineInUseThreshold: %d\n", __FUNCTION__, LineInUseThreshold);

                gFaxLineInUseThreshold = (UINT8)LineInUseThreshold;

                if(gFaxLineInUseThreshold != 0)
                {
                    SetPhoneParams(PHONE_LINE_IN_USE_THRESHOLD, (UINT16)gFaxLineInUseThreshold);
                }
            }

            break;
        }

#if __SUPPORT_CID__
        case FAXAPP_UPDATE_PARAM_CID:
        {
            int CID = 0;

            CID = db_get_var_int("CONF_FAXSP", "CID", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> CID\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - CID: %d\n", __FUNCTION__, CID);

                isCIDOn = CID;
            }
            break;
        }
#else
#endif

#if __TX_REPORT_IMAGE_PRINT__
        case FAXAPP_UPDATE_PARAM_TX_RESULT_RPT_IMAGE:
        {
            int TXResultRptImage = 0;

            TXResultRptImage = db_get_var_int("CONF_FAXUP", "TXResultRptImage", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSUP -> TXResultRptImage\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - TXResultRptImage: %d\n", __FUNCTION__, TXResultRptImage);

                if(TXResultRptImage == DISABLE)
                {
                    tx_report_image_delete();
                }

                gFaxTxResultRptImage = TXResultRptImage;
            }
            break;
        }
#endif

        case FAXAPP_UPDATE_PARAM_RING_DETECT:
        {
            int RingDetect = 0;

            RingDetect = db_get_var_int("CONF_FAXSP", "RingDetect", &db_retval);
            if (db_retval == FAIL)
            {
                printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXSP -> RingDetect\n", __FUNCTION__);

                faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
            }
            else
            {
                printf("\n(YHCHO) %s() - RingDetect: %d\n", __FUNCTION__, RingDetect);

                gRingDetect = RingDetect;
            }
            break;
        }

        default:
        {
            printf("\n(YHCHO) %s() - Invalid Param: %d\n", __FUNCTION__, Param);

            break;
        }
    }
}

