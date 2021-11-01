/* ************************************************************************* **
**                                                                           **
**                 Proprietary and Confidential Information                  **
**                                                                           **
**                Copyright (c) 2005 Oasis Semiconductor Inc.                **
**                                                                           **
**  This software contains proprietary and confidential material which is    **
**  property of Oasis Semiconductor Inc.                                     **
**                                                                           **
** ************************************************************************* */
/*
    File: fsu_call.c
    Date: March 15, 2005
    By:   S.J. Busak

    Contains code to implement the high-level process of sending or receiving a fax.
*/

#define __FSU_CALL_C__   // Allocates storage for variables in fsu_call.h

#include <stdlib.h>
#include "dc_common_includes.h"
#include "dc_fwcxdxx_lnx_api.h"
#include "fxcb.h"
#include "fsu_call.h"
#include "fsu.h"
#include "fsu_api.h"
#include "SDHFaxCommonHeader.h"
#include <unistd.h>
#include "faxapp_notify.h"
#include "ERROR.H"

UINT8 fsu_state = FSU_STATE_NOT_INITIALIZED;
UINT8 fsu_abort = 0;
UINT8 fsu_current_funcs = 0;
UINT8 gFSUTxUpdatePageCnt = 0;
UINT8 isExceptionHandleDone = 0;

#define FSU_IDLE_WAIT_COUNT	30 // 30 * 100 ms = 3s
static UINT8 fsu_idle_count = FSU_IDLE_WAIT_COUNT;

extern UINT8 isPhoneBusy;
extern UINT8 isPhoneNoAnswer;
extern UINT8 isPhoneNoDial;
extern UINT8 isPhoneAbort;
extern UINT16 fsu_job_num_send;
extern UINT16 fsu_job_num_rcv;
extern UINT16 gFaxT30Result;
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];
extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern int gFaxTxCompletePageNo;
extern long gFaxJobEndTimeForDuration;
extern long gFaxJobStartTimeForDuration;
extern UINT8 isDKSolution;

extern void fsu_remove_tx_job(UINT16 jnum);
extern void fsu_decrease_total_pages (UINT16 jnum);
extern void fsu_check_tx_job (void);
extern void fipp_send_event_rx_abort(void);
extern void fsu_purge_doc_dir(UINT16 jnum);
extern void fsu_decrease_retry(UINT16 jnum);
extern void fsu_update_tx_time (UINT16 jnum, long TimeToTx);
extern void set_tx_retry_job_ready(UINT16 jnum);
extern void fsu_update_status (UINT16 jnum, UINT8 Status);
extern void fsu_write_report_info_to_db (UINT8 reason);
extern void fsu_delete_report_record_from_db (UINT8 Reason);
extern void faxapp_process_tx_rx_result_report(void);
extern void faxapp_process_tcr(void);
extern void fsu_delete_file_on_file_system (const char *Path);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern void Update_Job_Queue_File(IMAGE_BACKUP_REASON_t Reason, UINT16 jnum, UINT8 Param);
extern void fsu_get_result (UINT16 T30Result);
extern void fsu_get_job_type(UINT8 JobType, UINT16 jnum);
extern void fsu_get_remote_no (UINT8 FSUJobType, UINT16 jnum);
extern void fsu_get_page_no (UINT8 PageNo);
extern void fsu_update_broadcast_result_report_struct (int nValue, UINT8 reason);
extern void fsu_request_write_broadcast_result_report_info_to_db (void);
extern void faxapp_process_broadcast_result_report(UINT16 jnum);
extern BOOLEAN fsu_check_job_queue_file (const char *Path);
extern BOOLEAN fsu_check_remaining_image_file (UINT16 jnum, UINT8 FileType);
extern UINT8 fsu_get_total_pages (UINT16 jnum);
extern UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_get_current_recipient_no (UINT16 DocId);
extern UINT8 fsu_get_poll_transmit (UINT16 jnum);
extern UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
extern UINT8 fsu_get_retry(UINT16 jnum);
extern UINT8 fsu_get_retry_tmr(UINT16 jnum);
extern UINT8 fsu_get_broadcast (UINT16 jnum);
extern UINT8 fsu_get_fax_forward (UINT16 jnum);
extern UINT8 fsu_get_tx_total_page_no (UINT16 jnum);
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern int fsu_get_memory_full_status_nand(void);
extern long fsu_get_current_system_time(void);
extern UINT8 fsu_get_photo_mode (UINT16 jnum);
extern UINT8 fsu_requset_get_current_page_no_from_Job_Queue_File (UINT16 jnum);
#if __TX_REPORT_IMAGE_PRINT__
void tx_report_image_scaling(UINT16 jnum);

extern int getgFaxTxResultRptImage(void);
#endif

UINT8 get_fsu_state( void )
{
    printf( "get_fsu_state %d\n", fsu_state );

    return fsu_state;
}


void set_fsu_state( UINT8 state )
{
    if ( fsu_state != state )
    {
        printf( "set_fsu_state(%d->%d)\n", fsu_state, state );

        if ( state == FSU_STATE_IDLE )
        {
            fsu_idle_count = 0;
        }

        fsu_state = state;
    }
}


// ------------------------------------------ //
// Fax Scheduler Unit Main Task State Machine //
// ------------------------------------------ //
void fsu_task ( void )
{
    switch ( fsu_state )
    {
        case FSU_STATE_IDLE:
        {
//#if __HAVE_FAXSOLUTION_DK__
//#else
			if(!isDKSolution)
			{
				if( fsu_idle_count == FSU_IDLE_WAIT_COUNT )
				{
					fsu_check_tx_job();
				}
				else
				{
					fsu_idle_count++;
				}
			}
//#endif
            break;
        }

        case FSU_STATE_TX_INIT1:
        {
            break;
        }

        case FSU_STATE_TX_INIT2:
        {
            break;
        }

        case FSU_STATE_TX_DIAL:
        {
            set_phone_state( PHONE_STATE_OFFHOOK_CALL );

            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_TX_START:
        {
            set_fsu_state( FSU_STATE_WAIT );

            fsu_current_funcs &= ~FAX_MEM_TO_SEND_DIALING;

            fsu_current_funcs |= FAX_MEM_TO_SEND_BEGIN;

#if __SUPPORT_MANUAL_TRANSMIT__
            if (fsu_get_flag_manual_tx(fsu_job_num_send) == 1)
            {
                T30Message( FXM_CALL_MANUAL );
            }
            else
            {
                T30Message( FXM_CALL_AUTO );
            }
#else
            T30Message( FXM_CALL_AUTO );
#endif

            break;
        }

#if __SUPPORT_MANUAL_TRANSMIT__
        case FSU_STATE_TX_START_MANUAL:
        {
            set_fsu_state( FSU_STATE_WAIT );

            fsu_current_funcs &= ~FAX_MEM_TO_SEND_DIALING;

            fsu_current_funcs |= FAX_MEM_TO_SEND_BEGIN;

            set_phone_state( PHONE_STATE_OFFHOOK_CALL_MANUAL );

            break;
        }
#else
#endif

        case FSU_STATE_TX_PHASE_B:
        {
            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_TX_PHASE_C_WAIT:
        case FSU_STATE_TX_PHASE_C_WAIT_EOM:
        {
            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_TX_PHASE_D:
        {
            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_TX_GOT_MCF:
        {
            break;
        }

        case FSU_STATE_TX_PHASE_E:
        {
            set_modem_idle();

            set_phone_state( PHONE_STATE_ONHOOK_FOR_RING );

            set_fsu_state( FSU_STATE_IDLE );

            fsu_current_funcs &= ~FAX_MEM_TO_SEND;

            break;
        }

        case FSU_STATE_TX_ABORT:
        {
            set_modem_idle();

            set_phone_state( PHONE_STATE_ONHOOK_FOR_RING );

            set_fsu_state( FSU_STATE_IDLE );

            tx_thread_sleep(6000);

            break;
        }

        case FSU_STATE_RX_MANUAL_ANS:
        {
            if ( on_hook_det() == 1 )
            {
                set_phone_state( PHONE_STATE_OFFHOOK_ANS );

                set_fsu_state( FSU_STATE_WAIT );
            }
            else
            {
#if __SUPPORT_MANUAL_RECEIVE__
                UINT8 FaxReceiveSource = 0;

                Fax_API_T30Attribute (GET, FXA_RECEIVESOURCE, &FaxReceiveSource);

                if (FaxReceiveSource == RECEIVE_SOURCE_PHONE)
                {
                    set_phone_state( PHONE_STATE_OFFHOOK_ANS );

                    set_fsu_state( FSU_STATE_WAIT );
                }
                else
                {
                    set_fsu_state( FSU_STATE_RX_START );
                }
#else
                set_fsu_state( FSU_STATE_RX_START );
#endif
            }

            fsu_abort = FALSE;

            break;
        }

        case FSU_STATE_ANS_WAIT_FOR_ON_HOOK:
        {
            if ( on_hook_det() == 1 )
            {
                set_modem_idle();

                set_fsu_state( FSU_STATE_IDLE );
            }

            break;
        }

        case FSU_STATE_RX_START:
        {
            fsu_current_funcs |= FAX_RECEIVE_TO_MEM_BEGIN;

            T30Message( FXM_ANSWER );

            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_RX_PHASE_B:
        {
            if ( !fsu_abort )
            {
                set_fsu_state( FSU_STATE_WAIT );
            }
            else
            {
                set_fsu_state( FSU_STATE_RX_ABORT );
            }

            break;
        }

        case FSU_STATE_RX_PHASE_C_WAIT_TX_POLL:
        case FSU_STATE_RX_PHASE_C_WAIT_EOM:
        {
            break;
        }

        case FSU_STATE_RX_PHASE_C_WAIT:
        {
            if ( fsu_abort )
            {
                fsu_abort = FALSE;

                set_fsu_state( FSU_STATE_RX_ABORT );
            }
            else
            {
                set_fsu_state( FSU_STATE_WAIT );
            }

            break;
        }

        case FSU_STATE_RX_PHASE_C_RECV:
        {
            break;
        }

        case FSU_STATE_MEM_RX_START:
        {
            activate_rcv_to_mem();

            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_RX_PHASE_D:
        {
            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_RX_PHASE_E:
        {
            set_modem_idle();

            set_phone_state( PHONE_STATE_ONHOOK_FOR_RING );

            set_fsu_state( FSU_STATE_IDLE );

            fsu_current_funcs &= ~FAX_RECEIVE_TO_MEM_BEGIN;

            fsu_current_funcs &= ~FAX_RECEIVE_TO_MEM;

            break;
        }

        case FSU_STATE_OFF_HOOK_WAIT:
        {
            set_fsu_state( FSU_STATE_OFF_HOOK );

            break;
        }

        case FSU_STATE_OFF_HOOK:
        {
            set_phone_state( PHONE_STATE_OFFHOOK );

            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_ON_HOOK:
        {
            set_modem_idle();

            set_phone_state( PHONE_STATE_INIT );

            set_fsu_state( FSU_STATE_IDLE );

            break;
        }

        case FSU_STATE_RX_ABORT:
        {
            set_modem_idle();

            set_phone_state( PHONE_STATE_ONHOOK_FOR_RING );

            set_fsu_state( FSU_STATE_IDLE );

            break;
        }

        case FSU_STATE_OFF_HOOK_ALARM:
        {
            break;
        }

        case FSU_STATE_SET_MODEM_IDLE:
        {
            set_modem_idle();

            set_phone_state( PHONE_STATE_INIT );

            set_fsu_state( FSU_STATE_IDLE );

            break;
        }

        case FSU_STATE_TIME_UPDATE:
        {
            set_fsu_state( FSU_STATE_WAIT );

            break;
        }

        case FSU_STATE_WAIT:
        {
            break;
        }

        default:
        {
            printf("\n%s (YHCHO) Invalid fsu_state: %d\n", __FUNCTION__, fsu_state);

            fsu_fax_job_init(0xff);

            break;
        }
    }
}


/************************************************************************************
 *  Function: fsu_rcv_next_page
 *
 *  Description:
 *		check memory full status whether to determine to receive more or not
 *
 *  Inputs:
 *		none
 *
 *  Output:
 *		FALSE on error (memory full)
 *      TRUE on OK (not memory full)
 ************************************************************************************/
BOOLEAN fsu_rcv_next_page(void)
{
    BOOLEAN ret_val = FALSE;

    if (fsu_get_memory_full_status_nand() == 1)
    {
        printf("\n(YHCHO) %s() - NAND Full...\n", __FUNCTION__);

        ret_val = FALSE;
    }
    else
    {
        if (fsu_get_memory_full_status_ram() == 1)
        {
            printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);

            ret_val = FALSE;
        }
        else
        {
            ret_val = TRUE;
        }
    }

    return ret_val;
}


// ----------------------------------------------- //
// Return the state of the off-hook detect circuit //
// ----------------------------------------------- //
// Return Value: 0: Off-hook; 1: On-hook

UINT8 on_hook_det ( void )
{
    UINT8 hook_state = LOCAL_PHONE_OFF_HOOK();

    switch ( hook_state )
    {
        case LINE_INUSE:
        case LINE_OFFHOOK:
            hook_state = 0;
            break;

        case LINE_ONHOOK:
        case LINE_NOLINE:
        default:
            hook_state = 1;
            break;
    }

    return ( hook_state );
}


/***********************************************************************
 *  Function: fsu_reschedule_tx_job
 *
 *  Description:
 *		reschedule tx job with input job number for the error fax job
 *
 * Input:
 *		UINT16 jnum - job number to be recheduled
 *
 *	Output:
 *		none
 ***********************************************************************/
void fsu_reschedule_tx_job(UINT16 jnum)
{
    UINT8 retry = 0;
    UINT8 retry_tmr = 0;
    UINT16 DocId = fsu_get_doc_id(jnum);
    UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);
    long CurrentSystemTime = 0;
    long TimeToTx = 0;

    printf("\n(YHCHO) %s() - jnum: %d\n", __FUNCTION__, jnum);

    if (fsu_get_flag_manual_tx(jnum) == 1)
    {
        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_FAIL, 0, gFaxTxCompletePageNo, 0, 0, jnum);

        fsu_remove_tx_job(jnum);

        fsu_purge_doc_dir(jnum);
    }
    else if (fsu_get_poll_transmit(jnum) == 1)
    {
        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_NON_COMPLETED_FAIL, 0, gFaxTxCompletePageNo, 0, 0, jnum);

        // Just update the status to FSU_STATE_SENDING_POLL_READY to allow poll tx next time...
        fsu_update_status(jnum, FSU_STATE_SENDING_POLL_READY);
    }
    else
    {
        retry = fsu_get_retry(jnum);

        if (retry == 1)
        {
            if (fsu_get_broadcast(jnum) == 1)
            {
                gFax_BC_Num[OriginalDocId].BC_Current_Num++;
            }

            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_FAIL, 0, gFaxTxCompletePageNo, 0, 0, jnum);

            fsu_decrease_retry(jnum);

            fsu_remove_tx_job(jnum);

            if ( (fsu_get_broadcast(jnum) == 0) || ((fsu_get_broadcast(jnum) == 1) && (DocId != OriginalDocId)) )
            {
                fsu_purge_doc_dir(jnum);
            }

            if ((fsu_get_broadcast(jnum) == 1) && (DocId == OriginalDocId))
            {
                Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT, jnum, FSU_RETRANSMIT_NO_NEED_RETRANSMIT);
            }

            if (fsu_get_broadcast(jnum) == 1)
            {
                //gFax_BC_Num[OriginalDocId].BC_Current_Num++;

                printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Current_Num: %d", __FUNCTION__, OriginalDocId, gFax_BC_Num[OriginalDocId].BC_Current_Num);
                printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Total_Num: %d\n", __FUNCTION__, OriginalDocId, gFax_BC_Num[OriginalDocId].BC_Total_Num);

                if (gFax_BC_Num[OriginalDocId].BC_Current_Num >= gFax_BC_Num[OriginalDocId].BC_Total_Num)
                {
                    printf("\n(YHCHO) %s() - There is no more recipient to transmit... So PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[OriginalDocId][0]));

                    fsu_purge_doc_dir(gFaxBroadcastJobInfo[OriginalDocId][0]);
                }
                else
                {
                    printf("\n(YHCHO) %s() - There are more recipients to transmit... So DO NOT PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[OriginalDocId][0]));
                }
            }
        }
        else
        {
            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_NON_COMPLETED_FAIL, 0, gFaxTxCompletePageNo, 0, 0, jnum);

            // Step 1. Reset status to FSU_STATE_NULL
            fsu_remove_tx_job(jnum);

            // Step 2. Decrease retry by one
            fsu_decrease_retry(jnum);

            // Step 3. Get the retry timer
            retry_tmr = fsu_get_retry_tmr(jnum);

            // Step 4. Get the current system time
            CurrentSystemTime = fsu_get_current_system_time();

            // Step 5. Calculate the retry difference
            TimeToTx = CurrentSystemTime + retry_tmr * 60;

            // Step 6. Reset transmit time
            fsu_update_tx_time(jnum, TimeToTx);

            // Step 7. Reset status to FSU_STATE_RETRY_READY
            set_tx_retry_job_ready(jnum);
        }

        printf("\n(YHCHO) %s() - retry: %d\n", __FUNCTION__, fsu_get_retry(jnum));
    }

    isExceptionHandleDone = 1;
}


/***********************************************************************
 *  Function: fsu_reschedule_poll_receive
 *
 *  Description:
 *		reschedule poll receive job with input job number for the error
 *
 * Input:
 *		UINT16 jnum - job number to be recheduled
 *
 *	Output:
 *		none
 ***********************************************************************/
void fsu_reschedule_poll_receive(UINT16 jnum)
{
    /*
    No Action Now..
    Something should be added to deal with abnormal case such as comm. error while receiving...
    What should I do?
    */
}


/***********************************************************************
 *  Function: fsu_tx_fax_cancel
 *
 *  Description:
 *		cancel tx job before T.30 thread activated
 *
 *  Input:
 *
 *	Output:
 *
 ***********************************************************************/
void fsu_tx_fax_cancel(void)
{
    UINT8 retry = 0;
    UINT16 jnum = fsu_job_num_send;

    retry = fsu_get_retry(jnum);

    if ( (isPhoneBusy == 1) || (isPhoneNoAnswer == 1) || (isPhoneNoDial == 1) || (isPhoneAbort == 1) )
    {
        if (isPhoneBusy == 1)
        {
            isPhoneBusy = 0;
            gFaxT30Result = FAX_ERROR_BUSY_TONE_ERROR;
        }
        else if (isPhoneNoAnswer == 1)
        {
            isPhoneNoAnswer = 0;
            gFaxT30Result = FAX_ERROR_NO_CONNECT_ERROR;
        }
        else if (isPhoneNoDial == 1)
        {
            isPhoneNoDial = 0;
            gFaxT30Result = FAX_ERROR_NO_CONNECT_ERROR;
        }
        else
        {
            isPhoneAbort = 0;
            gFaxT30Result = FAX_ERROR_PHONE_ABORT_ERROR;
        }

        fsu_get_result(gFaxT30Result);

        gFaxJobEndTimeForDuration = fsu_get_current_system_time();

        if (fsu_get_fax_forward(jnum) == 1)
        {
            fsu_get_job_type(JOB_TYPE_FOR_REPORT_FORWARD, jnum);
        }
        else
        {
            fsu_get_job_type(JOB_TYPE_FOR_REPORT_TX, jnum);
        }

        fsu_get_remote_no(FSU_JOB_TYPE_FOR_DIAL_TYPE_TRANSMIT, jnum);

        fsu_get_page_no(gFaxTxCompletePageNo);

        fsu_write_report_info_to_db(FAX_REPORT_TCR_FOR_FAX_SERVICE); // Update RECD_FAXSPLOG...

        fsu_delete_report_record_from_db(FAX_REPORT_TCR_FOR_FAX_SERVICE); // Delete RECD_FAXSPLOG if it has more than 35 records...

        if ( (retry == 1) && (fsu_get_broadcast(jnum) == 1) )
        {
            UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);
            UINT16 DocId = fsu_get_doc_id(jnum);

            fsu_update_broadcast_result_report_struct(jnum, BROADCAST_RESULT_REPORT_REASON_JOB_ID);

            fsu_update_broadcast_result_report_struct(gFaxJobEndTimeForDuration - gFaxJobStartTimeForDuration, BROADCAST_RESULT_REPORT_REASON_DURATION);

            fsu_update_broadcast_result_report_struct(fsu_get_tx_total_page_no(jnum), BROADCAST_RESULT_REPORT_REASON_TOTAL_PAGE_NO);

            if (DocId != OriginalDocId)
            {
                fsu_update_broadcast_result_report_struct(fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG) - 1, 
                                                                BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO);
            }
            else
            {
                fsu_update_broadcast_result_report_struct(fsu_requset_get_current_page_no_from_Job_Queue_File(jnum),
                                                                BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO);
            }

            fsu_update_broadcast_result_report_struct(gFaxT30Result, BROADCAST_RESULT_REPORT_REASON_RESULT);

            fsu_request_write_broadcast_result_report_info_to_db();

            if (gFax_BC_Num[OriginalDocId].BC_Current_Num >= gFax_BC_Num[OriginalDocId].BC_Total_Num)
            {
                faxapp_process_broadcast_result_report(jnum);
            }
        }
    }

    if (retry == 1)
    {
        UINT16 DocId = fsu_get_doc_id(jnum);
        UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);
        if (fsu_get_broadcast(jnum) == 1)
        {
            gFax_BC_Num[OriginalDocId].BC_Current_Num++;
        }

        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_FAIL, 0, gFaxTxCompletePageNo, 0, 0, jnum);

        fsu_decrease_retry(jnum);

        // Step 1. Reset status to FSU_STATE_NULL
        fsu_remove_tx_job(jnum);

        // Step 2. Purge all image files and /DocMan/DocXXXX/
        if ( (fsu_get_broadcast(jnum) == 0) || ((fsu_get_broadcast(jnum) == 1) && (DocId != OriginalDocId)) )
        {
            fsu_purge_doc_dir(jnum);
        }

        if ((fsu_get_broadcast(jnum) == 1) && (DocId == OriginalDocId))
        {
            Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT, jnum, FSU_RETRANSMIT_NO_NEED_RETRANSMIT);
        }

        // Step 3. If this job is the last job of broadcast, purge the original document...
        if (fsu_get_broadcast(jnum) == 1)
        {
            //gFax_BC_Num[OriginalDocId].BC_Current_Num++;

            printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Current_Num: %d", __FUNCTION__, OriginalDocId, gFax_BC_Num[OriginalDocId].BC_Current_Num);
            printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Total_Num: %d\n", __FUNCTION__, OriginalDocId, gFax_BC_Num[OriginalDocId].BC_Total_Num);

            if (gFax_BC_Num[OriginalDocId].BC_Current_Num >= gFax_BC_Num[OriginalDocId].BC_Total_Num)
            {
                printf("\n(YHCHO) %s() - There is no more recipient to transmit... So PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[OriginalDocId][0]));

                fsu_purge_doc_dir(gFaxBroadcastJobInfo[OriginalDocId][0]);
            }
            else
            {
                printf("\n(YHCHO) %s() - There are more recipients to transmit... So DO NOT PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[OriginalDocId][0]));
            }
        }

        // Step 4. Save comm. info for TCR and Tx/Rx Result Report
#if __TX_REPORT_IMAGE_PRINT__
        if(getgFaxTxResultRptImage() == ENABLE)
        {
            tx_report_image_scaling(fsu_job_num_send);
        }
#endif
        fsu_write_report_info_to_db(FAX_REPORT_TCR); // Update RECD_FAXLOG...

        fsu_delete_report_record_from_db(FAX_REPORT_TCR); // Delete RECD_FAXLOG if it has more than 35 records...

        // Step 5. Print TCR and Tx/Rx Result Report only if print options for TCR or Tx/Rx Result Report meet...
        faxapp_process_tx_rx_result_report();

        faxapp_process_tcr();
    }
    else
    {
        UINT8 retry_tmr = 0;
        long CurrentSystemTime = 0;
        long TimeToTx = 0;

        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_NON_COMPLETED_FAIL, 0, gFaxTxCompletePageNo, 0, 0, jnum);

        // Step 1. Reset status to FSU_STATE_NULL
        fsu_remove_tx_job(jnum);

        // Step 2. Decrease retry by one
        fsu_decrease_retry(jnum);

        // Step 3. Get the retry timer
        retry_tmr = fsu_get_retry_tmr(jnum);

        // Step 4. Get the current system time
        CurrentSystemTime = fsu_get_current_system_time();

        // Step 5. Calculate the retry difference
        TimeToTx = CurrentSystemTime + retry_tmr * 60;

        // Step 6. Reset transmit time
        fsu_update_tx_time(jnum, TimeToTx);

        // Step 7. Reset status to FSU_STATE_RETRY_READY
        set_tx_retry_job_ready(jnum);
    }

    fsu_current_funcs &= ~FAX_MEM_TO_SEND_BEGIN;
}


/***********************************************************************
 *  Function: fsu_rx_fax_cancel
 *
 *  Description:
 *			cancel rx job before fipp activate
 * Input:
 *		UINT8 abort_condition: TRUE - abort key pressed
 *							   FALSE - fax error
 *	Output:
 *		none
 ***********************************************************************/
void fsu_rx_fax_cancel(UINT8 abort_condition)
{
     fsu_current_funcs &= ~FAX_RECEIVE_TO_MEM_BEGIN;

     fsu_remove_tx_job(fsu_job_num_rcv);
}


/***********************************************************************
 *  Function: fsu_tx_update_page
 *
 *  Description:
 *		call function
 *
 * Input:
 *
 *	Output:
 ***********************************************************************/
void fsu_tx_update_page(void)
{
    UINT8 CurrentPageNo = 0;
    UINT8 isSendAll = 0;
    UINT16 jnum = fsu_job_num_send;
    UINT16 DocId = 0;
    UINT16 OriginalDocId = 0;
    char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };

    DocId = fsu_get_doc_id(jnum);

    gFSUTxUpdatePageCnt++;

    if (fsu_get_flag_manual_tx(jnum) == 1)
    {
        if (fsu_get_photo_mode(jnum) == 1)
        {
            CurrentPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);
        }
        else
        {
            CurrentPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
        }

        printf("\n(YHCHO) %s() - CurrentPageNo: %d\n", __FUNCTION__, CurrentPageNo);

        snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, CurrentPageNo);

        fsu_delete_file_on_file_system(jbgFileName);

        if(fsu_get_photo_mode(jnum) == 1)
        {
            snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, CurrentPageNo);

            fsu_delete_file_on_file_system(jbgFileName);
        }

        fsu_decrease_total_pages(jnum);

        if (fsu_get_photo_mode(jnum) == 1)
        {
            if (fsu_check_remaining_image_file(jnum, FAX_FILE_TYPE_PHOTO) == FALSE)
            {
                char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
                char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };

                snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

                snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

                fsu_delete_dir_on_file_system(DocDirPathNAND);

                fsu_delete_dir_on_file_system(DocDirPathRAM);

                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_COMPLETE, 0, gFSUTxUpdatePageCnt, 0, 0, jnum);
            }
        }
        else
        {
            if (fsu_check_remaining_image_file(jnum, FAX_FILE_TYPE_JBG) == FALSE)
            {
                char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
                char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };

                snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

                snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

                fsu_delete_dir_on_file_system(DocDirPathNAND);

                fsu_delete_dir_on_file_system(DocDirPathRAM);

                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_COMPLETE, 0, gFSUTxUpdatePageCnt, 0, 0, jnum);
            }
        }
    }
    else
    {
        fsu_decrease_total_pages(jnum);

        if (fsu_get_broadcast(jnum) == 0)
        {
            if (fsu_get_photo_mode(jnum) == 1)
            {
                CurrentPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);
            }
            else
            {
                CurrentPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
            }

            printf("\n(YHCHO) %s() - CurrentPageNo: %d\n", __FUNCTION__, CurrentPageNo);

            snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, CurrentPageNo);

            fsu_delete_file_on_file_system(jbgFileName);

            if(fsu_get_photo_mode(jnum) == 1)
            {
                snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, CurrentPageNo);

                fsu_delete_file_on_file_system(jbgFileName);
            }

            if (fsu_get_total_pages(jnum) == 0) // Send all images...
            {
                char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
                char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
                char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };

                snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);

                snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

                snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

                if (fsu_check_job_queue_file(JobQueueFileName) == TRUE)
                {
                    fsu_delete_file_on_file_system(JobQueueFileName);
                }

                fsu_delete_dir_on_file_system(DocDirPathNAND);

                fsu_delete_dir_on_file_system(DocDirPathRAM);

                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_COMPLETE, 0, gFSUTxUpdatePageCnt, 0, 0, jnum);
            }
        }
        else
        {
            OriginalDocId = fsu_get_original_doc_id(jnum);

            printf("\n(YHCHO) %s() - DocId: %04X, OriginalDocId: %04X\n", __FUNCTION__, DocId, OriginalDocId);

            if (DocId == OriginalDocId)
            {
                int FaxSendCurrentPage = fsu_requset_get_current_page_no_from_Job_Queue_File(jnum) + 1;

                printf("\n(YHCHO) %s() - This /DocMan/Doc%04X/ is original document... so just bypass...(%d)\n", __FUNCTION__, DocId, FaxSendCurrentPage);

                Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_PAGE_INFO, jnum, FaxSendCurrentPage);

                if (fsu_get_total_pages(jnum) == 0) // Send all images...
                {
                    gFax_BC_Num[OriginalDocId].BC_Current_Num++;

                    printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Current_Num: %d", __FUNCTION__, OriginalDocId, gFax_BC_Num[OriginalDocId].BC_Current_Num);
                    printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Total_Num: %d\n", __FUNCTION__, OriginalDocId, gFax_BC_Num[OriginalDocId].BC_Total_Num);

                    Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT, jnum, FSU_RETRANSMIT_NO_NEED_RETRANSMIT);

                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_COMPLETE, 0, gFSUTxUpdatePageCnt, 0, 0, jnum);
                }
                else
                {
                    printf("\n(YHCHO) %s() - There are more pages to transmit...\n", __FUNCTION__);
                }
            }
            else
            {
                if (fsu_get_photo_mode(jnum) == 1)
                {
                    CurrentPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);
                }
                else
                {
                    CurrentPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                }

                printf("\n(YHCHO) %s() - CurrentPageNo: %d\n", __FUNCTION__, CurrentPageNo);

                snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, CurrentPageNo);

                fsu_delete_file_on_file_system(jbgFileName);

                if(fsu_get_photo_mode(jnum) == 1)
                {
                    snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, CurrentPageNo);

                    fsu_delete_file_on_file_system(jbgFileName);
                }

                if (fsu_get_total_pages(jnum) == 0) // Send all images...
                {
                    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
                    char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
                    char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };

                    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);

                    snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

                    snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

                    if (fsu_check_job_queue_file(JobQueueFileName) == TRUE)
                    {
                        fsu_delete_file_on_file_system(JobQueueFileName);
                    }

                    fsu_delete_dir_on_file_system(DocDirPathNAND);

                    fsu_delete_dir_on_file_system(DocDirPathRAM);

                    gFax_BC_Num[OriginalDocId].BC_Current_Num++;

                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_COMPLETE, 0, gFSUTxUpdatePageCnt, 0, 0, jnum);

                    isSendAll = 1;
                }
            }

            if (isSendAll == 1)
            {
                printf("\n(YHCHO) %s() - There is no more page to transmit...\n", __FUNCTION__);

                //gFax_BC_Num[OriginalDocId].BC_Current_Num++;

                printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Current_Num: %d", __FUNCTION__, OriginalDocId, gFax_BC_Num[OriginalDocId].BC_Current_Num);
                printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Total_Num: %d\n", __FUNCTION__, OriginalDocId, gFax_BC_Num[OriginalDocId].BC_Total_Num);

                if (gFax_BC_Num[OriginalDocId].BC_Current_Num >= gFax_BC_Num[OriginalDocId].BC_Total_Num)
                {
                    printf("\n(YHCHO) %s() - (1) There is no more recipient to transmit... So PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[OriginalDocId][0]));

                    fsu_purge_doc_dir(gFaxBroadcastJobInfo[OriginalDocId][0]);
                }
                else
                {
                    printf("\n(YHCHO) %s() - There are more recipients to transmit... So DO NOT PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[OriginalDocId][0]));
                }
            }
            else
            {
                if ( (DocId == OriginalDocId) && (gFax_BC_Num[OriginalDocId].BC_Current_Num >= gFax_BC_Num[OriginalDocId].BC_Total_Num) )
                {
                    printf("\n(YHCHO) %s() - (2) There is no more recipient to transmit... So PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[OriginalDocId][0]));

                    fsu_purge_doc_dir(gFaxBroadcastJobInfo[OriginalDocId][0]);
                }
                else
                {
                    printf("\n(YHCHO) %s() - There are more pages to transmit...\n", __FUNCTION__);
                }
            }
        }

        fsu_get_memory_full_status_nand();
    }
}

