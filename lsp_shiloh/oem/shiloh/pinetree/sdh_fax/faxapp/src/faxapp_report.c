#include "SDHFaxCommonHeader.h"
#include <stdio.h>
#include <string.h>
#include "ATypes.h"
#include "lassert.h"
#include "agMessage.h"
#include "agRouter.h"
#include "internal_page_api.h"
#include "db_api.h"
#include "faxapp_notify.h"
#include "fsu.h"


#define MAX_RECORD_NO_FOR_TCR     35
#define MAX_RECORD_NO_FOR_TCR_FOR_FAX_SERVICE     700


BROADCAST_RESULT_REPORT_t BroadcastResultReport = { };


extern UINT16 gFaxT30Result;
extern UINT8 gFaxT30Direction;
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];

int faxapp_get_max_record_no_RECD_FAXLOG (void);

extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern int faxapp_get_rep_tx_rx_set(void);
extern int faxapp_get_rep_tcr_set(void);
extern int faxapp_get_data_count(UINT8 Reason);
extern int faxapp_get_last_record_no(UINT8 Reason);
extern int faxapp_get_rep_broadcast_result_report(void);


void faxapp_request_TxRxResultReport(void)
{
    error_type_t err;
    MESSAGE faxapp_mq_buf;

    faxapp_mq_buf.msgType = MSG_INTERNAL_PAGE;
    faxapp_mq_buf.param1 = e_FAXTXRXREPORT; // Tx/Rx Result Report
    faxapp_mq_buf.param2 = 0;		//NOT USE
    faxapp_mq_buf.param3 = NULL; 	//NOT USE

    err = SYMsgSend(INTERNALMGRID, &faxapp_mq_buf);         // send the message.
    XASSERT( err==OK, err );
}


void faxapp_request_TCR(void)
{
    error_type_t err;
    MESSAGE faxapp_mq_buf;

    faxapp_mq_buf.msgType = MSG_INTERNAL_PAGE;
    faxapp_mq_buf.param1 = e_FAXTCR; // Transmission Confirmation Report (TCR)
    faxapp_mq_buf.param2 = 0;		//NOT USE
    faxapp_mq_buf.param3 = NULL; 	//NOT USE

    err = SYMsgSend(INTERNALMGRID, &faxapp_mq_buf);         // send the message.
    XASSERT( err==OK, err );
}


void faxapp_request_T30LogReport(void)
{
    error_type_t err;
    MESSAGE faxapp_mq_buf;

    faxapp_mq_buf.msgType = MSG_INTERNAL_PAGE;
    faxapp_mq_buf.param1 = e_T30LOGREPORT; // T30 Log Report
    faxapp_mq_buf.param2 = 0;		//NOT USE
    faxapp_mq_buf.param3 = NULL; 	//NOT USE

    err = SYMsgSend(INTERNALMGRID, &faxapp_mq_buf);         // send the message.
    XASSERT( err==OK, err );
}


void faxapp_request_MemTxJobList(void)
{
    error_type_t err;
    MESSAGE faxapp_mq_buf;

    faxapp_mq_buf.msgType = MSG_INTERNAL_PAGE;
    faxapp_mq_buf.param1 = e_MEMTXJOBLIST; // Memory Tx Job List
    faxapp_mq_buf.param2 = 0;		//NOT USE
    faxapp_mq_buf.param3 = NULL; 	//NOT USE

    err = SYMsgSend(INTERNALMGRID, &faxapp_mq_buf);         // send the message.
    XASSERT( err==OK, err );
}


void faxapp_request_TCRForFaxService(void)
{
    error_type_t err;
    MESSAGE faxapp_mq_buf;

    faxapp_mq_buf.msgType = MSG_INTERNAL_PAGE;
    faxapp_mq_buf.param1 = e_TCRFORFAXSERVICE; // Transmission Confirmation Report (TCR)
    faxapp_mq_buf.param2 = 0;		//NOT USE
    faxapp_mq_buf.param3 = NULL; 	//NOT USE

    err = SYMsgSend(INTERNALMGRID, &faxapp_mq_buf);         // send the message.
    XASSERT( err==OK, err );
}


void faxapp_request_SpamFaxList(void)
{
    error_type_t err;
    MESSAGE faxapp_mq_buf;

    faxapp_mq_buf.msgType = MSG_INTERNAL_PAGE;
    faxapp_mq_buf.param1 = e_SPAMFAXLIST; // Spam Fax List
    faxapp_mq_buf.param2 = 0;		//NOT USE
    faxapp_mq_buf.param3 = NULL; 	//NOT USE

    err = SYMsgSend(INTERNALMGRID, &faxapp_mq_buf);         // send the message.
    XASSERT( err==OK, err );
}


void faxapp_request_BroadcastResultReport(void)
{
    error_type_t err;
    MESSAGE faxapp_mq_buf;

    faxapp_mq_buf.msgType = MSG_INTERNAL_PAGE;
    faxapp_mq_buf.param1 = e_BROADCASTRESULTREPORT; // Broadcast Result Report
    faxapp_mq_buf.param2 = 0;		//NOT USE
    faxapp_mq_buf.param3 = NULL; 	//NOT USE

    err = SYMsgSend(INTERNALMGRID, &faxapp_mq_buf);         // send the message.
    XASSERT( err==OK, err );
}


void faxapp_process_tx_rx_result_report(void)
{
    int RepTxRxSet = 0;

    RepTxRxSet = faxapp_get_rep_tx_rx_set();

    //printf("\n(YHCOH) %s() - RepTxRxSet: %d", __FUNCTION__, RepTxRxSet);
    //printf("\n(YHCOH) %s() - gFaxT30Result: %d", __FUNCTION__, gFaxT30Result);
    //printf("\n(YHCOH) %s() - gFaxT30Direction: %d\n", __FUNCTION__, gFaxT30Direction);

    if (RepTxRxSet == TX_RX_RESULT_REPORT_PRINT_OPTION_ALWAYS_ON) // Always On
    {
        faxapp_request_TxRxResultReport();
    }
    else if (RepTxRxSet == TX_RX_RESULT_REPORT_PRINT_OPTION_ON_ERROR) // On Error
    {
        if (gFaxT30Result != 0)
        {
            faxapp_request_TxRxResultReport();
        }
    }
    else if (RepTxRxSet == TX_RX_RESULT_REPORT_PRINT_OPTION_TX_ONLY) // Tx Only
    {
        if (gFaxT30Direction == 1) // Tx
        {
            faxapp_request_TxRxResultReport();
        }
    }
    else if (RepTxRxSet == TX_RX_RESULT_REPORT_PRINT_OPTION_RX_ONLY)// Rx Only
    {
        if (gFaxT30Direction == 0) // Rx
        {
            faxapp_request_TxRxResultReport();
        }
    }
	else // OFF
	{
		; // No Action Here..
	}
}


/***********************************************************************
 *  Function: faxapp_process_tcr
 *
 *  Description:
 *    determine whether to print TCR or not
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *    none
 ************************************************************************/
void faxapp_process_tcr(void)
{
    int TCRSet = faxapp_get_rep_tcr_set();
    int No = faxapp_get_data_count(FAX_REPORT_TCR);
    int LastRecordNo = faxapp_get_last_record_no(FAX_REPORT_TCR);

    //printf("\n(YHCHO) %s() - TCRSet: %d", __FUNCTION__, TCRSet);
    //printf("\n(YHCHO) %s() - No: %d", __FUNCTION__, No);
    //printf("\n(YHCHO) %s() - LastRecordNo: %d\n", __FUNCTION__, LastRecordNo);

    if ( (TCRSet == 1) && (No == faxapp_get_max_record_no_RECD_FAXLOG()) && (LastRecordNo % faxapp_get_max_record_no_RECD_FAXLOG() == 0) )
    {
        faxapp_request_TCR();
    }
}


/***********************************************************************
 *  Function: faxapp_process_broadcast_result_report
 *
 *  Description:
 *    update BROADCAST_RESULT_REPORT_t struct by using RECD_FAXBROADCASTRESULTREPORT for Broadcast Result Report
 *
 *  Inputs:
 *    UINT16 jnum: job number to generate
 *
 *  Output:
 *    none
 ************************************************************************/
void faxapp_process_broadcast_result_report(UINT16 jnum)
{
    UINT8 IndexToRecipient = 0;
    UINT16 IndexToDB = 0;
    UINT16 OriginalDocId = 0;
    UINT16 TotalRecipient = 0;
    int RowCount = 0;
    int cnt = 0;
    int RepBCResultReport = 0;
    error_type_t db_retval = FAIL;
    FAXBROADCASTLOG_STRUCT result[TX_JOBS];

    db_retval = db_get_fax_broadcast_log(result, &RowCount);

    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_del_recd_all() Error... RECD_FAXBROADCASTRESULTREPORT\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
    }
    else
    {
        memset(&BroadcastResultReport, 0x00, sizeof(BroadcastResultReport));

        OriginalDocId = fsu_get_original_doc_id(jnum);

        TotalRecipient = gFax_BC_Num[OriginalDocId].BC_Total_Num;

        for (IndexToDB = 0; IndexToDB < RowCount; IndexToDB++)
        {
            for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
            {
                if ( (gFax_BC_Num[OriginalDocId].BC_Abnormal_Count != 0) && (IndexToRecipient == 0) )
                {
                    // No Action Here
                }
                else
                {
                    if (result[IndexToDB].jobid == gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient])
                    {
                        snprintf(BroadcastResultReport.RemoteNo[cnt], LEN_PHONE_NO, "%s", result[IndexToDB].remotenum);

                        BroadcastResultReport.StartTime[cnt] = result[IndexToDB].datetime;

                        BroadcastResultReport.Duration[cnt] = result[IndexToDB].duration;

                        BroadcastResultReport.TotalPageNo[cnt] = result[IndexToDB].totalpageno;

                        BroadcastResultReport.SentPageNo[cnt] = result[IndexToDB].sentpageno;

                        BroadcastResultReport.Result[cnt] = result[IndexToDB].result;

                        cnt++;

                        break;
                    }
                }
            }
        }

        for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
        {
            if ( (gFax_BC_Num[OriginalDocId].BC_Abnormal_Count != 0) && (IndexToRecipient == 0) )
            {
                // No Action Here
            }
            else
            {
                db_retval = db_del_recd_fax_broadcast_log_job_var(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);

                if (db_retval == FAIL)
                {
                    printf("\n(YHCHO) %s() - db_get_fax_broadcast_log() Error... RECD_FAXBROADCASTRESULTREPORT\n", __FUNCTION__);

                    faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
                }
            }
        }

        BroadcastResultReport.Count = cnt;

        RepBCResultReport = faxapp_get_rep_broadcast_result_report();

        if (RepBCResultReport == 1)
        {
            faxapp_request_BroadcastResultReport();
        }
    }
}


/***********************************************************************
 *  Function: faxapp_get_max_record_no_RECD_FAXLOG
 *
 *  Description:
 *    return the maximum record number of RECD_FAXLOG
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *    Max. record number of RECD_FAXLOG
 ************************************************************************/
int faxapp_get_max_record_no_RECD_FAXLOG (void)
{
    return MAX_RECORD_NO_FOR_TCR;
}


/***********************************************************************
 *  Function: faxapp_get_max_record_no_RECD_FAXSPLOG
 *
 *  Description:
 *    return the maximum record number of RECD_FAXSPLOG
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *    Max. record number of RECD_FAXSPLOG
 ************************************************************************/
int faxapp_get_max_record_no_RECD_FAXSPLOG (void)
{
    return MAX_RECORD_NO_FOR_TCR_FOR_FAX_SERVICE;
}


/***********************************************************************
 *  Function: faxapp_print_report
 *
 *  Description:
 *    Print Report
 *
 *  Inputs:
 *    UINT8 report : Print Report Number
 *
 *  Output:
 *    
 ************************************************************************/
void faxapp_print_report (UINT8 report)
{
	printf("\n(YHCHO) %s() : %d", __FUNCTION__, report);
	
	switch(report)
	{
        case FAX_REPORT_TX_RX_RESULT_REPORT:
			faxapp_request_TxRxResultReport();
			break;

        case FAX_REPORT_TCR:
			faxapp_request_TCR();
			break;

        case FAX_REPOR_MEMORY_FILE_LIST:
			faxapp_request_MemTxJobList();
			break;

        case FAX_ERROR_T30_LOG:
			faxapp_request_T30LogReport();
			break;

        case FAX_REPORT_TCR_FOR_FAX_SERVICE:
			faxapp_request_TCRForFaxService();
			break;

        case FAX_REPORT_SPAM_FAX_LIST:
            faxapp_request_SpamFaxList();
            break;

		default:
			break;
	}
}
