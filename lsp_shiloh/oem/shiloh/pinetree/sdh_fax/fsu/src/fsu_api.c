/************************************************************************
 *
 *      fsu_api.c
 *
 *  Fax API.
 *
 *  Copyright 2004 - 2010 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"
#include "fsu_api.h"
#include "fshs.h"
#include "fsu_call.h"
#include "MODEMX.H"
#include "PHONEX.H"
#include "ERROR.H"
#include "fsu.h"
#include "t30.h"
#include "SDHFaxCommonHeader.h"
#include "../../faxapp/include/faxapp_job.h"
#include "../../faxapp/include/faxapp_notify.h"
#include "../../../internal_page/config/report_manager.h"


#define MAX_DUMP_LIST_LINE		(350)
#define MAX_DUMP_PAGE_LINE		(35)
#define MAX_FIF_DATA_SIZE		(34)


typedef struct
{
    UINT32 time;
    FAX_DIRECTION dir;
    UINT16 fcf;
    UINT16 size;
    UINT16 fif[MAX_FIF_DATA_SIZE];
} DUMP_LIST_t;
DUMP_LIST_t dump_list[MAX_DUMP_LIST_LINE];
UINT16 total_dump_cnt = 0;
UINT16 dump_list_cnt = 0;


//taeyoukim add.
extern int gFaxSendCurrentPage;
extern int gFaxAbnormalFlag;
extern UINT32 total_size;
extern UINT32 NonECM_total_size;
char gcTSI_for_RemoteFaxNum[FCFSendTableSize + 1] = {0, };
char gcCIG_for_RemoteFaxNum[FCFSendTableSize + 1] = {0, };
char gcCID_for_RemoteFaxNum[FCFSendTableSize + 1] = {0, };
char gFaxImageLogSrcPath[MAX_STRING_LEN] = {0, };
char gFaxImageLogDstPath[MAX_STRING_LEN] = {0, };
static int gcpSpeedType = 0;
static int gFaxT30Resolution = 0;
static int gFaxT30Width = 0;
static int gcpEcmModeType = 0;
static int gcpCodingType = 0;
static int TotalPageNoToSend = 0;
static int TxRx_job_user_cancel = 0;

extern void error_rcv_to_mem(void);
extern void set_txq_nxt (void);
extern void set_tx_job_ready(UINT16 jnum);
extern void fsu_update_resolution (UINT16 jnum, UINT8 res);
extern void fsu_update_width (UINT16 jnum, UINT8 width);
extern void fsu_update_total_pages (UINT16 jnum, UINT8 Page);
extern void fsu_get_remote_no (UINT8 FSUJobType, UINT16 jnum);
extern void fsu_get_page_no (UINT8 PageNo);
extern void fsu_get_modulation_name (UINT8 Modulation);
extern void fsu_set_rcv_job_done (UINT16 jnum);
extern void fsu_set_send_job_done (UINT16 jnum);
extern void fsu_write_report_info_to_db(UINT8 reason);
extern void faxapp_process_tx_rx_result_report(void);
extern void faxapp_process_tcr(void);
extern void fsu_delete_report_record_from_db (UINT8 Reason);
extern void faxapp_make_pdf_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType, int Request, int Reason);
extern void faxapp_make_tif_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType);
extern void fsu_remove_rx_job(UINT16 jnum);
extern void fsu_make_dir_on_file_system (const char *Path);
extern void fsu_update_status (UINT16 jnum, UINT8 Status);
extern void faxapp_solution_request_send (UINT16 jnum);
extern void fsu_make_doc_dir_in_ram(UINT16 DocId);
extern void fsu_rcv_poll(FAXAPP_PARAMS_t *pFaxStruct);
extern void fsu_reschedule_poll_receive(UINT16 jnum);
extern UINT32 fsu_get_unique_job_id(UINT16 jnum);
extern long fsu_get_current_system_time(void);
extern void fsu_update_doc_id (UINT16 jnum, UINT16 DocId);
extern void fsu_display_job_log_info (void);
extern void fsu_update_delay_time_for_job_log (UINT8 isDelay, UINT16 jnum);
extern void fsu_update_broadcast_result_report_struct (int nValue, UINT8 reason);
extern void fsu_request_write_broadcast_result_report_info_to_db (void);
extern void faxapp_process_broadcast_result_report(UINT16 jnum);
extern void fsu_update_dialno (UINT16 jnum, UINT8 *dialno);
#ifdef HAVE_IMAGELOG
extern BOOLEAN IsImageLogEnabled(char jobtype, int subJobType);
#endif
extern UINT8 get_phone_state(void);
extern UINT8 fsu_get_fax_forward (UINT16 jnum);
extern UINT8 fsu_convert_connected_speed (UINT8 Speed);
extern UINT8 fsu_get_poll_transmit (UINT16 jnum);
extern UINT8 fsu_get_poll_receive (UINT16 jnum);
extern UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
extern UINT8 fsu_get_total_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 faxapp_get_no_of_recipient(void);
extern UINT8 fsu_get_width (UINT16 jnum);
extern UINT8 fsu_get_photo_mode (UINT16 jnum);
extern UINT8 fsu_get_retry(UINT16 jnum);
extern UINT8 fsu_get_status (UINT16 jnum);
extern UINT8 fsu_get_broadcast (UINT16 jnum);
extern UINT8 fsu_request_get_total_page_no_from_Job_Queue_File (UINT16 DocId);
extern UINT8 fsu_get_is_delay (UINT16 jnum);
extern UINT8 fsu_get_tx_total_page_no (UINT16 jnum);
extern UINT8 fsu_get_total_pages (UINT16 jnum);
extern UINT8 *fsu_get_dial_no (UINT16 jnum);
#ifdef HAVE_IMAGELOG
extern UINT8 *fsu_get_fax_forward_no(void);
#ifdef META_END_WITH_JOBID
extern UINT32 create_unique_job_id( void );
#endif
#endif
extern UINT16 fsu_get_doc_id(UINT16 jnum);
extern UINT16 fsu_make_doc_dir_in_nand(void);
extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern UINT32 faxapp_create_shared_mem_for_display_receive(char *DialNo);
extern FSU_RETURN_CODE fsu_scan_to_mem(FAXAPP_PARAMS_t *pFaxStruct);
extern void fsu_set_next_prn_num(UINT16 next_prn_num);
extern void fsu_purge_doc_dir(UINT16 jnum);
extern int getgFaxPrintMode(void);
extern int fsu_get_scan_busy_flag (void);
extern UINT8 fsu_get_resolution (UINT16 jnum);
#if __TX_REPORT_IMAGE_PRINT__
void tx_report_image_scaling(UINT16 jnum);
void tx_report_image_delete(void);

extern int getgFaxTxResultRptImage(void);
#endif

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern void fsu_request_make_dot_end_file (const char *pPath, UINT16 jnum, UINT8 job_type);
extern void fsu_update_unique_job_id (UINT16 jnum, UINT32 jobId);
extern void save_fax_jobinfo(int _jobid, int _faxpage, int _faxresult, char *faxnum);
#endif

BOOLEAN f_Rx_DCS_done = FALSE;
BOOLEAN f_Rx_TSI_done = FALSE;
extern UINT8 gFaxCurrentSendPageNo;
extern UINT8 gFaxSendCurrentPageOnHook;
extern UINT8 fsu_current_funcs;
extern UINT8 RTNCount;
extern UINT8 isTxImageProcessDone;
extern UINT8 gFaxCurrentTTIPageNo;
extern UINT8 isFSUTxImageProgessBegin;
extern UINT8 isFSURxImageProgessBegin;
extern UINT8 gFSUTxUpdatePageCnt;
extern UINT8 isCIDOn;
extern UINT8 isExceptionHandleDone;
extern BOOLEAN isDialCancel;
#if __SUPPORT_MANUAL_TRANSMIT__
extern UINT8 ExtPhoneDialBuffer[LEN_PHONE_NO + 1];
extern UINT8 ExtPhoneDialIndex;
#else
#endif
extern UINT16 fsu_job_num_scan;
extern UINT16 fsu_job_num_send;
extern UINT16 fsu_job_num_rcv;
extern UINT16 fsu_job_num_prn;
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];
extern void fsu_fax_job_init(UINT8 Fax_Status);
UINT16 gFaxT30Result = 0;
UINT8 gFaxT30Direction = 0;
UINT8 isReadyToTransmit = 0;
UINT8 isLineInUse = 0;
UINT8 isExceptionHandleBegin = 0;
#if __SUPPORT_MANUAL_RECEIVE__
UINT8 isRinging = 0;
#else
#endif
static UINT8 fSendConnectingMsg = 0;
int gFaxRxTotalPages = 0;
//taeyoukim	 add.
int gFaxRxCurrentPage = 0;
int isPhoneRingOn = 0;
int isOnHookScanCanceledByCommError = 0;
int rx_docman_cleared = 0;
BOOLEAN prn_cancel_flag = FALSE;
extern UINT8 isSolutionFax;
extern UINT32 gFaxCNGStartTime;
extern int FileFormat;
extern int PageType;
extern int gFaxTxCompletePageNo;
#if __SUPPORT_MANUAL_RECEIVE__
extern int loopExitCnt;
#else
#endif
extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern char *pRootFolderNameSolDocMan;
extern INFO_FOR_FAX_JOB_LOG_t FaxJobLog;

extern BOOLEAN fsu_check_doc_man_dir_in_nand (const char *Path);
extern BOOLEAN fsu_check_doc_man_dir_in_ram (const char *Path);
extern BOOLEAN fsu_check_job_queue_file (const char *Path);
extern void fsu_delete_last_prn_num(UINT16 temp_job_num);
extern void fsu_update_image_width_pixel (UINT16 jnum, UINT32 width);
extern void fsu_update_image_height_pixel (UINT16 jnum, UINT32 height);
extern void fsu_update_fax_forward_job_queue (UINT16 jnum);
extern void abort_mem_to_send(void);
extern void error_mem_to_send(void);
extern void Update_Job_Queue_File(IMAGE_BACKUP_REASON_t Reason, UINT16 jnum, UINT16 Param);
extern void fsu_get_job_start_time(long CurrentTimeInSec);
extern void fsu_get_job_type(UINT8 JobType, UINT16 jnum);
extern void fsu_get_connected_speed (UINT8 Speed);
extern void fsu_get_ecm (UINT8 ECM);
extern void fsu_get_res (UINT8 Resolution);
extern void fsu_get_result (UINT16 T30Result);
extern void fsu_get_duration (void);
extern void abort_rcv_to_mem(void);
extern void fsu_delete_file_on_file_system (const char *Path);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern void faxapp_scan_cancel(void);
extern void fsu_update_original_doc_id (UINT16 jnum, UINT16 DocId);
extern void fsu_update_user_id_job_log (UINT16 jnum);
extern void faxapp_update_recd_fax_job_log (INFO_FOR_FAX_JOB_LOG_t *Info);
extern int fsu_get_memory_full_status_nand(void);
extern int getSaveFaxFile(void);
#ifdef HAVE_IMAGELOG
extern int set_metafile_end_info(char _jobtype, int _subJobType, char* _destination, int _copy, char* _filename, int _originalpages, int _jobid);
#endif
#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
extern char *fsu_get_current_local_job_time_in_string(void);
#else
#endif

long gFaxJobStartTimeForDuration = 0;
long gFaxJobEndTimeForDuration = 0;

#if __MEASURE_PERFORMANCE__
extern void faxapp_get_job_start_time(int JobType);
extern void faxapp_get_job_end_time(int JobType);
extern char *faxapp_get_elapsed_time(int JobType);
#endif // #if __MEASURE_PERFORMANCE__


/***********************************************************************

 *  Function: Fax_API_OffHook
 *
 *  Description:
 *      offhook
 *
 *  Inputs:
 *      none
 *
 *  Output:
 *      none
 ***********************************************************************/
void Fax_API_OffHook(void)
{
    set_phone_state( PHONE_STATE_OFFHOOK );
}


/***********************************************************************
 *  Function: Fax_API_OnHook
 *
 *  Description:
 *      onhook
 *
 *  Inputs:
 *      none
 *
 *  Output:
 *      none
 ***********************************************************************/
void Fax_API_OnHook(void)
{
    set_phone_state( PHONE_STATE_INIT );
}


/***********************************************************************
 *  Function: Fax_API_Dial
 *
 *  Description:
 *      generate digit
 *
 *  Inputs:
 *      FAXAPP_PARAMS_t  *pFaxStruct
 *
 *  Output:
 *      none
 ***********************************************************************/
void Fax_API_Dial(FAXAPP_PARAMS_t *pFaxStruct)
{
	while(1)
	{
		if(get_phone_state() != PHONE_STATE_DIAL_DIGIT)
			break;
			
		usleep(50*1000);
	}
	
    set_phone_number((UINT8 *) pFaxStruct->dial_num, 0 );

    set_phone_state( PHONE_STATE_DIAL_DIGIT );
}


/***********************************************************************
 *  Function: Fax_API_TransmitFax
 *
 *  Description:
 *		start fax tx function by activating scan to mem
 *
 *  Inputs:
 *		FAXAPP_PARAMS_t  *pFaxStruct
 *
 *  Output:
 *		none
 ***********************************************************************/
void Fax_API_TransmitFax(FAXAPP_PARAMS_t *pFaxStruct)
{
    fsu_scan_to_mem(pFaxStruct);
}


/***********************************************************************
 *  Function: Fax_API_ManTransmitFax
 *
 *  Description:
 *		start fax tx function by activating scan to mem
 *
 *  Inputs:
 *      none
 *
 *  Output:
 *		none
 ***********************************************************************/
void Fax_API_ManTransmitFax(void)
{
    T30Message( FXM_CALL_MANUAL );
}


/***********************************************************************
 *  Function: Fax_API_ManReceiveFax
 *
 *  Description:
 *		start fax rx function manually
 *
 *  Inputs:
 *      none
 *
 *  Output:
 *		none
 ***********************************************************************/
void Fax_API_ManReceiveFax(void)
{
    set_fsu_state( FSU_STATE_RX_MANUAL_ANS );
}


/***********************************************************************
 *  Function: Fax_API_PollRecieveFax
 *
 *  Description:
 *		start fax poll rx function
 *
 *  Inputs:
 *		FAXAPP_PARAMS_t  *pFaxStruct
 *
 *  Output:
 *		none
 ***********************************************************************/
void Fax_API_PollRecieveFax(FAXAPP_PARAMS_t *pFaxStruct)
{
    fsu_rcv_poll(pFaxStruct);
}


/***********************************************************************
 *  Function: fsu_cancel_scan_job
 *
 *  Description:
 *		cancel scan job
 *
 *  Inputs:
 *		flag_abort_key: TRUE for stop key
 *						FALSE for t.30 error
 *      jnum: job number to cancel
 *
 *  Output:
 *		none
 ***********************************************************************/
void fsu_cancel_scan_job(BOOLEAN flag_abort_key, UINT16 jnum)
{
    fsu_current_funcs &= ~FAX_SCAN_TO_MEM;

    if (fsu_get_flag_manual_tx(jnum) == 1)
    {
        if ( (fsu_get_scan_busy_flag() == 1) && (flag_abort_key == FALSE) )
        {
            gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_SCAN_CANCEL;
            isOnHookScanCanceledByCommError = 1;
            faxapp_scan_cancel();

            // Wait here for the scanner to finish... should find better way to do this...
            tx_thread_sleep(3*1000);
            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_FAIL, 0, gFaxTxCompletePageNo, 0, 0, jnum);
        }

        if (flag_abort_key == TRUE)
        {
			if (fsu_get_scan_busy_flag() == 1)
			{
				gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_SCAN_CANCEL;
				isOnHookScanCanceledByCommError = 1;
				faxapp_scan_cancel();
				// Wait here for the scanner to finish... should find better way to do this...
				tx_thread_sleep(3*1000);
			}

            while (TRUE)
            {
                if (isFSUTxImageProgessBegin == 1)
                    break;
                else
                    printf("\n(YHCHO) %s() - FSU Tx Image Process Is In Progress...\n", __FUNCTION__);

                tx_thread_sleep(100);
            }

            T30Message( FXM_ABORT );
        }
    }

    if (fsu_get_flag_manual_tx(jnum) == 0)
    {
        UINT8 NoOfRecipient = faxapp_get_no_of_recipient();
        UINT8 IndexToRecipient = 0;
        UINT16 OriginalDocId = 0;

        if ( (NoOfRecipient == 0) || (NoOfRecipient == 1) )
        {
            fsu_purge_doc_dir(jnum);
        }
        else
        {
            OriginalDocId = fsu_get_original_doc_id(jnum);

            for (IndexToRecipient = 0; IndexToRecipient < NoOfRecipient; IndexToRecipient++)
            {
                fsu_purge_doc_dir(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
            }
        }
    }
    else
    {
        fsu_purge_doc_dir(jnum);
    }

    fsu_fax_job_init(FAX_SCAN_TO_MEM);
}


/***********************************************************************
 *  Function: fsu_cancel_tx_job
 *
 *  Description:
 *		cancel tx job
 *
 *  Inputs:
 *		flag_abort_key: TRUE for stop key
 *						FALSE for t.30 error
 *      jnum: job number to cancel
 *
 *  Output:
 *		none
 ***********************************************************************/
void fsu_cancel_tx_job(BOOLEAN flag_abort_key, UINT16 jnum)
{
    while (TRUE)
    {
        if (isFSUTxImageProgessBegin == 1)
            break;
        else
            printf("\n(YHCHO) %s() - FSU Tx Image Process Is In Progress...\n", __FUNCTION__);

        tx_thread_sleep(100);
    }

    if(flag_abort_key == TRUE)
    {
        TxRx_job_user_cancel = 1;

        abort_mem_to_send();

        T30Message( FXM_ABORT );

        if (fsu_get_broadcast(jnum) == 1)
        {
            UINT16 DocId = fsu_get_doc_id(jnum);
            UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);

            if (DocId != OriginalDocId)
            {
                fsu_purge_doc_dir(jnum);
            }
            else
            {
                Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT, jnum, FSU_RETRANSMIT_NO_NEED_RETRANSMIT);
            }

            gFax_BC_Num[OriginalDocId].BC_Current_Num++;

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
        else
        {
            fsu_purge_doc_dir(jnum);
        }

        fsu_fax_job_init(FAX_MEM_TO_SEND);
    }
    else
    {
        error_mem_to_send();

        isExceptionHandleBegin = 1;
    }
}


/***********************************************************************
 *  Function: fsu_cancel_rx_job
 *
 *  Description:
 *		cancel rx job
 *
 *  Inputs:
 *		flag_abort_key: TRUE for stop key
 *						FALSE for t.30 error
 *      jnum: job number to cancel
 *
 *  Output:
 *		none
 ***********************************************************************/
void fsu_cancel_rx_job(BOOLEAN flag_abort_key, UINT16 jnum)
{
    if(flag_abort_key == TRUE)
    {
        TxRx_job_user_cancel = 1;

		while (TRUE)
		{
			if (isFSURxImageProgessBegin == 1)
				break;
			else
				printf("\n(YHCHO) %s() - FSU Rx Image Process Is In Progress...\n", __FUNCTION__);

			tx_thread_sleep(100);
		}

        abort_rcv_to_mem();

        T30Message( FXM_ABORT );

        if(getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_ALL)
        {
            rx_docman_cleared = 1;
            fsu_purge_doc_dir(jnum); //delete DocXXXX
            fsu_fax_job_init(FAX_RECEIVE_TO_MEM);
        }
        else
        {
            if(gFaxRxTotalPages == 0)
            {
                rx_docman_cleared = 1;
                fsu_purge_doc_dir(jnum); //delete DocXXXX
                fsu_fax_job_init(FAX_RECEIVE_TO_MEM);
            }
            else if(jnum == fsu_job_num_prn)
            {
                prn_cancel_flag = TRUE;//flag setting
            }
            else
            {
                rx_docman_cleared = 1;
                fsu_purge_doc_dir(jnum); //delete DocXXXX
                fsu_delete_last_prn_num(jnum);//delete last fsu_job_num_prn
                fsu_fax_job_init(FAX_RECEIVE_TO_MEM);
            }
        }
    }
    else
    {
        error_rcv_to_mem();
    }
}


/***********************************************************************
 *  Function: Fax_API_CancelFax
 *
 *  Description:
 *		 fax error handling for abort condition and t.30 protocol error
 *
 *  Inputs:
 *		flag_abort_key: TRUE for stop key
 *						FALSE for t.30 error
 *      jnum: job number to cancel
 *
 *  Output:
 *
 ***********************************************************************/
void Fax_API_CancelFax(BOOLEAN flag_abort_key, UINT16 jnum)
{
    UINT8 status = fsu_get_status(jnum);

    printf("\n(YHCHO) %s() - status: %d\n", __FUNCTION__, status);

    isPhoneRingOn = 0;

    if (flag_abort_key == TRUE)
    {
        printf("\n=============================================\n");
        printf("======        CANCEL KEY PRESSED       ======\n");
        printf("=============================================\n");
    }
    else
    {
        printf("\n=============================================\n");
        printf("======       T.30 ERROR DETECTED       ======\n");
        printf("=============================================\n");
    }

    switch(status)
    {
        case FSU_STATE_NULL:
            printf("\n(YHCHO) %s() - Do Not Cancel: FSU_STATE_NULL\n", __FUNCTION__);
            break;
        case FSU_STATE_SCANNING:
            if(fsu_current_funcs & FAX_SCAN_TO_MEM)
            {
                printf("\n(YHCHO) %s() Scan Job Cancel 1\n", __FUNCTION__);
                fsu_cancel_scan_job(flag_abort_key, jnum);
            }
            else if(fsu_current_funcs & FAX_MEM_TO_SEND || fsu_current_funcs & FAX_MEM_TO_SEND_BEGIN)
            {
                printf("\n(YHCHO) %s() Send Job Cancel 2\n", __FUNCTION__);
                gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_TX_CANCEL;
                fsu_cancel_tx_job(flag_abort_key, jnum);
            }
            else
            {
                printf("\n(YHCHO) %s() Not Cancel: FSU_STATE_SCANNING, %d \n", __FUNCTION__,fsu_current_funcs);
            }
            break;
        case FSU_STATE_SENDING_POLL_READY:
            if(fsu_current_funcs & FAX_MEM_TO_SEND || fsu_current_funcs & FAX_MEM_TO_SEND_BEGIN)
            {
                printf("\n(YHCHO) %s() Send Job Cancel\n", __FUNCTION__);
                gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_TX_CANCEL;
                fsu_cancel_tx_job(flag_abort_key, jnum);
            }
            else
            {
                printf("\n(YHCHO) %s() Not Cancel: FSU_STATE_SENDING_POLL_READY, %d \n", __FUNCTION__,fsu_current_funcs);
            }
            break;
        case FSU_STATE_SENDING_READY:
            if(fsu_current_funcs & FAX_MEM_TO_SEND || fsu_current_funcs & FAX_MEM_TO_SEND_BEGIN)
            {
                printf("\n(YHCHO) %s() Send Job Cancel\n", __FUNCTION__);
                gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_TX_CANCEL;
                fsu_cancel_tx_job(flag_abort_key, jnum);
            }
            else if(fsu_current_funcs & FAX_MEM_TO_SEND_DIALING)
            {
                printf("\n(YHCHO) %s() Dialing Cancel\n", __FUNCTION__);
                isDialCancel = TRUE;
                set_phone_state( PHONE_STATE_INIT );
				if (fsu_get_broadcast(jnum) == 1)
				{
					UINT16 DocId = fsu_get_doc_id(jnum);
					UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);

					if (DocId != OriginalDocId)
					{
						fsu_purge_doc_dir(jnum);
					}
					else
					{
						Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT, jnum, FSU_RETRANSMIT_NO_NEED_RETRANSMIT);
					}

					gFax_BC_Num[OriginalDocId].BC_Current_Num++;

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

					fsu_update_broadcast_result_report_struct(jnum, BROADCAST_RESULT_REPORT_REASON_JOB_ID);

					fsu_update_broadcast_result_report_struct(0, BROADCAST_RESULT_REPORT_REASON_DURATION);

					fsu_update_broadcast_result_report_struct(fsu_get_tx_total_page_no(jnum), BROADCAST_RESULT_REPORT_REASON_TOTAL_PAGE_NO);

					fsu_update_broadcast_result_report_struct(0, BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO);

					fsu_update_broadcast_result_report_struct(FAX_ERROR_USER_ABORT_ERROR, BROADCAST_RESULT_REPORT_REASON_RESULT);

					fsu_request_write_broadcast_result_report_info_to_db();

					if (gFax_BC_Num[OriginalDocId].BC_Current_Num >= gFax_BC_Num[OriginalDocId].BC_Total_Num)
					{
						faxapp_process_broadcast_result_report(fsu_job_num_send);
					}
				}
				else
				{
					fsu_purge_doc_dir(jnum);
				}
                fsu_fax_job_init(FAX_MEM_TO_SEND);
                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, 0, 0, 0, jnum);


				//Report & Job Log
				gFaxJobEndTimeForDuration = 0;
				gFaxJobStartTimeForDuration = 0;
				gFaxT30Result = FAX_ERROR_USER_ABORT_ERROR;

				fsu_get_ecm(1);
				fsu_get_res(fsu_get_resolution(jnum));
				fsu_get_duration();
				fsu_get_connected_speed(13);
				fsu_get_modulation_name(0);

				//fsu_get_job_start_time(gFaxJobStartTimeForDuration);

				if (fsu_get_flag_manual_tx(jnum) == 1)
				{
					fsu_get_job_type(JOB_TYPE_FOR_REPORT_ON_HOOK, jnum);
				}
				else if (fsu_get_fax_forward(fsu_job_num_send) == 1)
				{
					fsu_get_job_type(JOB_TYPE_FOR_REPORT_FORWARD, jnum);
				}
				else
				{
					fsu_get_job_type(JOB_TYPE_FOR_REPORT_TX, jnum);
				}

				fsu_get_remote_no(FSU_JOB_TYPE_FOR_DIAL_TYPE_TRANSMIT, jnum);
				fsu_get_result(gFaxT30Result);
				fsu_get_page_no(0); //0
				fsu_update_user_id_job_log(jnum);
				if (fsu_get_is_delay(jnum) == 1)
				{
					fsu_update_delay_time_for_job_log(1, jnum);
				}
				else
				{
					fsu_update_delay_time_for_job_log(0, jnum);
				}

				fsu_write_report_info_to_db(FAX_REPORT_TCR); // Update RECD_FAXLOG...
				fsu_delete_report_record_from_db(FAX_REPORT_TCR); // Delete RECD_FAXLOG if it has more than 35 records...

				fsu_write_report_info_to_db(FAX_REPORT_TCR_FOR_FAX_SERVICE); // Update RECD_FAXSPLOG...
				fsu_delete_report_record_from_db(FAX_REPORT_TCR_FOR_FAX_SERVICE); // Delete RECD_FAXSPLOG if it has more than 700 records...

				faxapp_process_tx_rx_result_report();

				faxapp_process_tcr();

				faxapp_update_recd_fax_job_log(&FaxJobLog);

				gFaxT30Result = 0;
            }
            else
            {
                printf("\n(YHCHO) %s() Not Cancel: FSU_STATE_SENDING_READY, %d \n", __FUNCTION__,fsu_current_funcs);
            }
            break;
        case FSU_STATE_RECEIVING:
            if(fsu_current_funcs & FAX_RECEIVE_TO_MEM || fsu_current_funcs & FAX_RECEIVE_TO_MEM_BEGIN)
            {
                printf("\n(YHCHO) %s() Receive Job Cancel\n", __FUNCTION__);
                gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_RX_CANCEL;
                fsu_cancel_rx_job(flag_abort_key, jnum);
            }
            else
            {
                printf("\n(YHCHO) %s() Not Cancel: FSU_STATE_RECEIVING, %d \n", __FUNCTION__,fsu_current_funcs);
            }
            break;
        case FSU_STATE_RECEIVE_DONE:
            printf("\n(YHCHO) %s() - Do Not Cancel: FSU_STATE_RECEIVE_DONE\n", __FUNCTION__);
            break;
        case FSU_STATE_PRINT_READY:
            if(fsu_current_funcs & FAX_RECEIVE_TO_MEM || fsu_current_funcs & FAX_RECEIVE_TO_MEM_BEGIN)
            {
                printf("\n(YHCHO) %s() Receive Job Cancel\n", __FUNCTION__);
                gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_RX_CANCEL;
                fsu_cancel_rx_job(flag_abort_key, jnum);
            }
            else
            {
                printf("\n(YHCHO) %s() Not Cancel: FSU_STATE_PRINT_READY, %d \n", __FUNCTION__,fsu_current_funcs);
            }
            break;
        case FSU_STATE_RETRY_READY:
            if(fsu_current_funcs & FAX_MEM_TO_SEND || fsu_current_funcs & FAX_MEM_TO_SEND_BEGIN)
            {
                printf("\n(YHCHO) %s() Send Job Cancel\n", __FUNCTION__);
                gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_TX_CANCEL;
                fsu_cancel_tx_job(flag_abort_key, jnum);
            }
            else
            {
                printf("\n(YHCHO) %s() Not Cancel: FSU_STATE_RETRY_READY, %d \n", __FUNCTION__,fsu_current_funcs);
            }
            break;
        case FSU_STATE_SENDING_PENDING:
            printf("\n(YHCHO) %s() - Do Not Cancel: FSU_STATE_SENDING_PENDING\n", __FUNCTION__);
            //Cancel function need??
            break;
        default:
            return;
    }
}


/***********************************************************************
 *  Function: Fax_API_T30Attribute
 *
 *  Description:
 *
 *  Inputs:
 *
 *  Output:
 */
void Fax_API_T30Attribute(UINT8 set_get, T30_ATTRIB attrib_id, UINT8 *msg)
{
    T30Attribute(set_get, attrib_id, msg );
}


/***********************************************************************
 *  Function: Fax_API_Speaker_Volume
 *
 *  Description:
 *
 *
 *  Inputs: 0:off, 1:min, 7:max
 *
 *  Output:
 */
void Fax_API_Speaker_Volume(UINT8 volume)
{
    ModemSetSpeakerVolume( volume );
}


int Get_LineStatus(void)
{
    int ret = 0;

    switch ( get_line_status() )
    {
        case LINE_ONHOOK:

            ret = PHONE_ONHOOK;
            break;

        case LINE_INUSE:

            ret = PHONE_LINEINUSE;
            break;

        case LINE_NOLINE:

            ret = PHONE_NOLINE;
            break;

        case LINE_OFFHOOK:

            ret = PHONE_OFFHOOK;
            break;

        case LINE_UNKNOWN:
            break;
    }

    return ret;
}


/***********************************************************************
 *  Function: StartHomologationSignal
 *
 *  Description:
 *
 *  Inputs:
 *
 *  Output:
 */
void StartHomologationSignal(MODEM_TX_TEST_INDEX HomologationSignal)
{
    phone_send_test_signal( HomologationSignal );
}


/***********************************************************************
 *  Function: StopHomologationSignal
 *
 *  Description:
 *
 *  Inputs:
 *
 *  Output:
 */
void StopHomologationSignal(void)
{
    phone_send_test_signal( MODEM_TX_TEST_STOP );
}


/***********************************************************************
 *  Function: SetPhoneParams
 *
 *  Description:
 *
 *  Inputs:
 *
 *  Output:
 ***********************************************************************/
void SetPhoneParams(UINT8 type, UINT16 val)
{
    UINT8 val8 = 0;

    switch ( type )
    {
        case PHONE_SELECT_COUNTRY:
            val8 = ( UINT8 ) val;
            T30Attribute( SET, FXA_COUNTRY_CODE, &val8 );
            break;

        case PHONE_TRANSMIT_LEVEL:
            val8 = ( UINT8 ) val;
            T30Attribute( SET, FXA_ATTENUATION, &val8 );
            break;

        default:
            phone_set_params( type, val );
            break;
    }
}


/***********************************************************************
 *  Function: fax_events
 *
 *  Description:
 *
 *  Inputs:
 *
 *  Output:
 */
void fax_events(FAX_EVENT *pEvent)
{
    int tmp = 0;
    UINT8 nego_param;
    UINT8 ConnSpeed = 0;
    UINT8 i = 0;
    UINT8 j = 0;
    int tx_rx_report_print_check = 1;

    printf( "*** fax_events %d ***\n", pEvent->type );

    if ( (pEvent->type == PHONE_CNG) || (pEvent->type == PHONE_DIAL) || pEvent->fcf )
    {
        dump_list[dump_list_cnt].time = ospl_time_get();

        if ((pEvent->type == PHONE_CNG) || (pEvent->type == PHONE_DIAL))
        {
            dump_list[dump_list_cnt].dir = 1;

            if (pEvent->type == PHONE_CNG)
            {
                dump_list[dump_list_cnt].fcf = PHONE_CNG;
            }
            else
            {
                dump_list[dump_list_cnt].fcf = PHONE_DIAL;
            }
        }
        else
        {
            dump_list[dump_list_cnt].dir = pEvent->dir;
            dump_list[dump_list_cnt].fcf = pEvent->fcf;
        }

        if( pEvent->size > MAX_FIF_DATA_SIZE )
        {
            dump_list[dump_list_cnt].size = MAX_FIF_DATA_SIZE;
        }
        else
        {
            dump_list[dump_list_cnt].size = pEvent->size;
        }

        if( dump_list[dump_list_cnt].size )
        {
            int ii = 0;

//          memcpy(dump_list[dump_list_cnt].fif,pEvent->data,dump_list[dump_list_cnt].size);
            for( ii = 0; ii < dump_list[dump_list_cnt].size; ii++ )
            {
                dump_list[dump_list_cnt].fif[ii] = pEvent->data[ii];
            }
        }

        total_dump_cnt++;
        dump_list_cnt++;
        dump_list_cnt%=MAX_DUMP_LIST_LINE;
    }

    switch ( pEvent->type )
    {
        case PHONE_CNG:
        {
            printf("\n(YHCHO) %s() - PHONE_CNG\n", __FUNCTION__);

            break;
        }

        case PHONE_NODIAL:
        {
            printf("\n(YHCHO) %s() - PHONE_NODIAL\n", __FUNCTION__);

            fsu_current_funcs &= ~FAX_MEM_TO_SEND_DIALING;

            isPhoneRingOn = 0;

            gFaxJobEndTimeForDuration = 0;

            gFaxJobStartTimeForDuration = 0;

#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
            printf("\n(YHCHO) Transmit (%d) End Time: %s\n", fsu_job_num_send, fsu_get_current_local_job_time_in_string());
#else
#endif

            break;
        }

        case PHONE_ONHOOK:
        {
            printf("\n(YHCHO) %s() - PHONE_ONHOOK\n", __FUNCTION__);

            break;
        }

        case PHONE_OFFHOOK:
        {
            printf("\n(YHCHO) %s() - PHONE_OFFHOOK\n", __FUNCTION__);

            total_dump_cnt = 0;
            dump_list_cnt = 0;

            gcpSpeedType = 0;
            gFaxT30Resolution = 0;
            gcpEcmModeType = 0;
            gcpCodingType = 0;

            if (pEvent->dir == T30_RECEIVE)
            {
                isPhoneRingOn = 0;

#if __SUPPORT_MANUAL_RECEIVE__
                isRinging = 0;
#else
#endif
            }

            fSendConnectingMsg = 0;

            break;
        }

        case PHONE_LINEINUSE:
        {
            printf("\n(YHCHO) %s() - PHONE_LINEINUSE\n", __FUNCTION__);

            isLineInUse = 1;

#if __SUPPORT_MANUAL_RECEIVE__
            if (isRinging != 0)
            {
                //faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PUT_SPECIAL_NUMBER, 0, 0, 0, 0, EMPTY_JOB_NUM);

                printf("\n(YHCHO) %s() - Notify - FAX_APP_STATUS_MSG_PUT_SPECIAL_NUMBER\n", __FUNCTION__);
            }
            else
            {
                //faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PICK_UP_RECEIVER, 0, 0, 0, 0, EMPTY_JOB_NUM);

                printf("\n(YHCHO) %s() - Notify - FAX_APP_STATUS_MSG_PICK_UP_RECEIVER\n", __FUNCTION__);
            }
#else
#endif

            break;
        }

        case PHONE_LINENOTUSE:
        {
            printf("\n(YHCHO) %s() - PHONE_LINENOTUSE\n", __FUNCTION__);

            isLineInUse = 0;

#if __SUPPORT_MANUAL_TRANSMIT__
            memset(ExtPhoneDialBuffer, 0x00, sizeof(ExtPhoneDialBuffer));

            ExtPhoneDialIndex = 0;
#else
#endif

#if __SUPPORT_MANUAL_RECEIVE__
            isRinging = 0;

            loopExitCnt = 0;

            //faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_HANG_UP_RECEIVER, 0, 0, 0, 0, EMPTY_JOB_NUM);

            printf("\n(YHCHO) %s() - Notify - FAX_APP_STATUS_MSG_HANG_UP_RECEIVER\n", __FUNCTION__);
#else
#endif

            break;
        }

        case PHONE_NOLINE:
        {
            printf("\n(YHCHO) %s() - PHONE_NOLINE\n", __FUNCTION__);

            break;
        }

        case PHONE_PRESENTLINE:
        {
            printf("\n(YHCHO) %s() - PHONE_PRESENTLINE\n", __FUNCTION__);

            break;
        }

        case PHONE_DIAL:
        {
            char DialNo[2] = {0, };
            char *Compare = "T";

            printf("\n(YHCHO) %s() - PHONE_DIAL\n", __FUNCTION__);

            DialNo[0] = pEvent->data[0];
            DialNo[1] = 0;

            if (strcmp(DialNo, Compare) == 0)
            {
                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DIALING, 0, 0, 0, 0, EMPTY_JOB_NUM);
            }
            break;
        }

        case PHONE_BUSY:
        {
            printf("\n(YHCHO) %s() - PHONE_BUSY\n", __FUNCTION__);

            fsu_current_funcs &= ~FAX_MEM_TO_SEND_DIALING;

            isPhoneRingOn = 0;

            gFaxJobEndTimeForDuration = 0;

            gFaxJobStartTimeForDuration = 0;

#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
            printf("\n(YHCHO) Transmit (%d) End Time: %s\n", fsu_job_num_send, fsu_get_current_local_job_time_in_string());
#else
#endif

            break;
        }

        case PHONE_NOANSWER:
        {
            printf("\n(YHCHO) %s() - PHONE_NOANSWER\n", __FUNCTION__);

            fsu_current_funcs &= ~FAX_MEM_TO_SEND_DIALING;

            isPhoneRingOn = 0;

            gFaxJobEndTimeForDuration = 0;

            gFaxJobStartTimeForDuration = 0;

#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
            printf("\n(YHCHO) Transmit (%d) End Time: %s\n", fsu_job_num_send, fsu_get_current_local_job_time_in_string());
#else
#endif

            break;
        }

        case PHONE_RINGON:
        {
            printf("\n(YHCHO) %s() - PHONE_RINGON\n", __FUNCTION__);

#if __SUPPORT_MANUAL_RECEIVE__
            isRinging++;
#else
#endif

            if (isPhoneRingOn == 0)
            {
                isPhoneRingOn = 1;

                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_RINGING, 0, 0, 0, 0, EMPTY_JOB_NUM);
            }

            break;
        }

        case PHONE_RINGOFF:
        {
            printf("\n(YHCHO) %s() - PHONE_RINGOFF\n", __FUNCTION__);

            break;
        }

        case PHONE_CALLERID:
        {
            printf("\n(YHCHO) %s() - PHONE_CALLERID\n", __FUNCTION__);

            printf("\n(YHCHO) %s() - isCIDOn: %d\n", __FUNCTION__, isCIDOn);

            if (isCIDOn == 1)
            {
                CID_INFORMATION *pCidInfo;

                pCidInfo = ( CID_INFORMATION * ) pEvent->data;

                printf("\n(YHCHO) %s() - pCidInfo->Reason: %d\n", __FUNCTION__, pCidInfo->Reason);

                if ( pCidInfo->Reason == CID_DATA_VALID )
                {
                    UINT32 SharedMemID = 0;

                    //printf( " Date: %u/%u, Time: %u:%u.\n", pCidInfo->Month, pCidInfo->Day, pCidInfo->Hour, pCidInfo->Minute );

                    //printf( " ID: %s, Number: %s, Name: %s.\n", pCidInfo->LineId, pCidInfo->Number, pCidInfo->Name );

                    memset(gcCID_for_RemoteFaxNum, 0x00, sizeof(gcCID_for_RemoteFaxNum));

                    snprintf(gcCID_for_RemoteFaxNum, sizeof(gcCID_for_RemoteFaxNum), "%s", pCidInfo->LineId);

                    SharedMemID = faxapp_create_shared_mem_for_display_receive(gcCID_for_RemoteFaxNum);

                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DIAL_NO_RECEIVE, 0, 0, 0, SharedMemID, fsu_job_num_rcv);
                }
            }

            break;
        }

        case PHONE_DETECTEDFAX:
        {

#if __MEASURE_PERFORMANCE__
            faxapp_get_job_start_time(T30_TRANSMIT);
#endif // #if __MEASURE_PERFORMANCE__

            printf("\n(YHCHO) %s() - PHONE_DETECTEDFAX\n", __FUNCTION__);

            if( fSendConnectingMsg == 0 )
            {
                if(fsu_get_flag_manual_tx(fsu_job_num_scan) == 1)
                {
                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CONNECTING, 0, 0, 0, 0, EMPTY_JOB_NUM);
                }
                else
                {
                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CONNECTING, 0, 0, 0, 0, fsu_job_num_send);
                }
                fSendConnectingMsg = 1;
            }

            break;
        }

        case PHONE_ABORT:
        {
            printf("\n(YHCHO) %s() - PHONE_ABORT\n", __FUNCTION__);

            fsu_current_funcs &= ~FAX_MEM_TO_SEND_DIALING;

            isPhoneRingOn = 0;

            gFaxJobEndTimeForDuration = 0;

            gFaxJobStartTimeForDuration = 0;

#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
            printf("\n(YHCHO) Transmit (%d) End Time: %s\n", fsu_job_num_send, fsu_get_current_local_job_time_in_string());
#else
#endif

            break;
        }
        
        case RX_PHASE_A:
        {
			if(fsu_get_status(fsu_job_num_rcv) != FSU_STATE_RECEIVING)
			{
				UINT16 DocId = 0;
				UINT16 jnum = 0;

				jnum = fsu_job_num_rcv = txq.nxt;
				printf("\n(YHCHO) %s() - fsu_job_num_rcv: %d\n", __FUNCTION__, fsu_job_num_rcv);
				faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_FAX_RECEIVE_JOB_CREATED, 0, 0, 0, 0, jnum);

				fsu_update_status(jnum, FSU_STATE_RECEIVING);

				DocId = fsu_make_doc_dir_in_nand();
				fsu_update_doc_id(jnum, DocId);
				fsu_make_doc_dir_in_ram(DocId);

				fsu_update_original_doc_id(jnum, DEFAULT_ORIGINAL_DOC_ID);

				fsu_get_job_type(JOB_TYPE_FOR_REPORT_RX, jnum);

				set_txq_nxt ();

				gFaxJobStartTimeForDuration = fsu_get_current_system_time();
				fsu_get_job_start_time(gFaxJobStartTimeForDuration);

	#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
				printf("\n(YHCHO) Receive (%d) Start Time: %s\n", jnum, fsu_get_current_local_job_time_in_string());
	#else
	#endif
			}
            break;
        }
		
        default:
        {
            switch (pEvent->fcf)
            {
                case ANSam:				// Start point V.34
                {

#if __MEASURE_PERFORMANCE__
                    faxapp_get_job_start_time(T30_RECEIVE);
#endif // #if __MEASURE_PERFORMANCE__

                    printf("\n(YHCHO) %s() - ANSam\n", __FUNCTION__);

                    if( fSendConnectingMsg == 0 )
                    {
                        if (pEvent->dir == T30_RECEIVE)
                        {
                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CONNECTING, 0, 0, 0, 0, fsu_job_num_send);
                        }
                        else
                        {
                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CONNECTING, 0, 0, 0, 0, fsu_job_num_rcv);
                        }
                        fSendConnectingMsg = 1;
                    }

                    break;
                }

                case CFR:
                {
                    printf("\n(YHCHO) %s() - CFR\n", __FUNCTION__);

                    break;
                }

                case MCF:
                {
                    printf("\n(YHCHO) %s() - MCF\n", __FUNCTION__);

                    break;
                }

                case CED:				// Start point Under V.17
                {
                    UINT16 jnum = fsu_job_num_rcv;

#if __MEASURE_PERFORMANCE__
                    faxapp_get_job_start_time(T30_RECEIVE);
#endif // #if __MEASURE_PERFORMANCE__

                    printf("\n(YHCHO) %s() - CED\n", __FUNCTION__);

                    if( fSendConnectingMsg == 0 )
                    {
                        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CONNECTING, 0, 0, 0, 0, jnum);

                        fSendConnectingMsg = 1;
                    }

                    break;
                }

                case IMG:
                {
                    UINT16 DocId = 0;
                    UINT16 OriginalDocId = 0;
                    UINT16 jnum = 0;

                    printf("\n(YHCHO) %s() - IMG\n", __FUNCTION__);

                    if(gFaxT30Result != FAX_ERROR_NO_ERROR)
                    {
                        printf("\n(YHCHO) %s() - IMG: FAX ERROR is not Zero : %d\n", __FUNCTION__, gFaxT30Result);

                        break;
                    }

                    if (pEvent->dir == T30_RECEIVE)
                    {
                        jnum = fsu_job_num_rcv;
                    }
                    else
                    {
                        jnum = fsu_job_num_send;
                    }

                    ConnSpeed = fsu_convert_connected_speed(pEvent->data[0]);

                    if (pEvent->dir == T30_RECEIVE)
                    {
                        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_RECEIVING, gFaxRxCurrentPage + 1, 0, 0, 0, jnum);
                    }
                    else
                    {
                        if (fsu_get_flag_manual_tx(jnum) == 1)
                        {
                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING, gFaxCurrentSendPageNo, 0, 0, 0, jnum);
                        }
                        else
                        {
                            if (gFaxCurrentSendPageNo == 1)
                            {
                                if (fsu_get_broadcast(jnum) == 0)
                                {
                                    if (fsu_get_photo_mode(jnum) == 1)
                                    {
                                        TotalPageNoToSend = fsu_get_total_page_no_in_mem(fsu_get_doc_id(jnum), FAX_FILE_TYPE_PHOTO);
                                    }
                                    else
                                    {
                                        TotalPageNoToSend = fsu_get_total_page_no_in_mem(fsu_get_doc_id(jnum), FAX_FILE_TYPE_JBG);
                                    }
                                }
                                else
                                {
                                    DocId = fsu_get_doc_id(jnum);

                                    OriginalDocId = fsu_get_original_doc_id(jnum);

                                    if (DocId != OriginalDocId)
                                    {
                                        if (fsu_get_photo_mode(jnum) == 1)
                                        {
                                            TotalPageNoToSend = fsu_get_total_page_no_in_mem(fsu_get_doc_id(jnum), FAX_FILE_TYPE_PHOTO);
                                        }
                                        else
                                        {
                                            TotalPageNoToSend = fsu_get_total_page_no_in_mem(fsu_get_doc_id(jnum), FAX_FILE_TYPE_JBG);
                                        }
                                    }
                                    else
                                    {
                                        TotalPageNoToSend = fsu_request_get_total_page_no_from_Job_Queue_File(DocId);
                                    }
                                }
                            }

                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING, gFaxCurrentSendPageNo, TotalPageNoToSend, 0, 0, jnum);
                        }
                    }

                    fsu_get_connected_speed(ConnSpeed);

                    break;
                }

                case DIS:
                {
                    printf("\n(YHCHO) %s() - DIS\n", __FUNCTION__);

                    if( fSendConnectingMsg == 0 )
                    {
                        if (pEvent->dir == T30_TRANSMIT) //receiver send DIS
                        {
                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CONNECTING, 0, 0, 0, 0, fsu_job_num_rcv);
                        }
                        else
                        {
                            if(fsu_get_flag_manual_tx(fsu_job_num_scan) == 1)
                            {
                                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CONNECTING, 0, 0, 0, 0, EMPTY_JOB_NUM);
                            }
                            else
                            {
                                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CONNECTING, 0, 0, 0, 0, fsu_job_num_send);
                            }
                        }

                        fSendConnectingMsg = 1;
                    }

                    break;
                }

                case DCN:
                {
                    printf("\n(YHCHO) %s() - DCN\n", __FUNCTION__);

                    break;
                }

                case DCS:
                {
                    printf("\n(YHCHO) %s() - DCS\n", __FUNCTION__);

                    if( pEvent->dir == T30_RECEIVE)
                    {
                        f_Rx_DCS_done = TRUE;
                    }

                    gFaxT30Resolution = 0; //STANDARD
                    gcpEcmModeType = 0; //ECM OFF
                    gcpCodingType = 0; //MH
                    gcpSpeedType = 0; // V.34

                    for (tmp = 0; tmp < (pEvent->size); tmp++)
                    {
                        if (tmp == 2)
                        {
                            if( gcpSpeedType == 0 )
                            {
                                nego_param = (pEvent->data[tmp] & 0x3C);
                                switch( nego_param )
                                {
                                    case 0x20: //V.17-14400
                                    case 0x28: //V.17-12000
                                    case 0x24: //V.17-9600
                                    case 0x2C: //V.17-7200
                                        gcpSpeedType = 1;
                                        break;

                                    case 0x04: //V.29-9600
                                    case 0x0C: //V.29-7200
                                        gcpSpeedType = 2;
                                        break;

                                    case 0x08: //V.27-4800
                                    case 0x00: //V.27-2400
                                        gcpSpeedType = 3;
                                        break;

                                    default:
                                        break;
                                }
                            }

                            nego_param = (pEvent->data[tmp] & 0x40);
                            if( nego_param )
                            {
                                gFaxT30Resolution = 1; //FINE
                            }

                            nego_param = (pEvent->data[tmp] & 0x80);
                            if( nego_param )
                            {
                                gcpCodingType = 1; //MR
                            }
                        }
                        else if(tmp == 4)
                        {
                            nego_param = (pEvent->data[tmp] & 0x04);

                            if( nego_param )
                            {
                                gcpEcmModeType = 1; // ECM ON
                            }

                            nego_param = (pEvent->data[tmp] & 0x40);
                            if( nego_param )
                            {
                                gcpCodingType = 2; //MMR
                            }
                        }
                        else if(tmp == 6 )
                        {
                            nego_param = (pEvent->data[tmp] & 0x07);
                            if( nego_param )
                            {
                                gFaxT30Resolution = 3;
                            }
                        }
                        else if(tmp == 10)
                        {
                            nego_param = (pEvent->data[tmp] & 0x60);
                            if( nego_param )
                            {
                                gcpCodingType = 3; //JBIG
                            }
                        }
                    }

                    fsu_get_ecm(gcpEcmModeType);
                    fsu_get_res(gFaxT30Resolution);

                    if( pEvent->dir == T30_RECEIVE )
                    {
                        fsu_update_resolution(fsu_job_num_rcv, gFaxT30Resolution);

                        fsu_update_width(fsu_job_num_rcv, gFaxT30Width); // WIDTH_A4
                    }

                    break;
                }

                case T30_END:
                {
                    printf("\n(YHCHO) %s() - T30_END", __FUNCTION__);

                    if ( (pEvent->dir == T30_TRANSMIT) && (isExceptionHandleBegin == 1) )
                    {
                        do
                        {
                            usleep(50*1000); // 50ms

                            //printf("\n(YHCHO) %s() - isExceptionHandleDone: %d\n", __FUNCTION__, isExceptionHandleDone);

                        } while ( isExceptionHandleDone != 1 );
                    }

#ifdef HAVE_IMAGELOG
                    if (pEvent->dir == T30_RECEIVE)
                    {
                        if ( (IsImageLogEnabled('F', IMAGE_LOG_JOB_TYPE_RX) == TRUE) && (getSaveFaxFile() == ENABLE) )
                        {
                            char RemoteNo[LEN_PHONE_NO + 1] = {0, };

                            snprintf(RemoteNo, sizeof(RemoteNo), "%s", gcTSI_for_RemoteFaxNum);

                            save_fax_jobinfo(fsu_get_unique_job_id(fsu_job_num_rcv), gFaxRxTotalPages, 0, (char *)RemoteNo);

#ifdef META_END_WITH_JOBID
							set_metafile_end_info('F', IMAGE_LOG_JOB_TYPE_RX, RemoteNo, 0, NULL, gFaxRxTotalPages, fsu_get_unique_job_id(fsu_job_num_rcv));
#else
                            set_metafile_end_info('F', IMAGE_LOG_JOB_TYPE_RX, RemoteNo, 0, NULL, gFaxRxTotalPages, fsu_get_unique_job_id(fsu_job_num_rcv));
#endif
                        }
#if 0
                        else if ( (IsImageLogEnabled('F', IMAGE_LOG_JOB_TYPE_TX) == TRUE) && (fsu_get_fax_forward(fsu_job_num_rcv) == 1) )
                        {
                            char RemoteNo[LEN_PHONE_NO + 1] = {0, };

                            snprintf(RemoteNo, sizeof(RemoteNo), "%s", (char *)fsu_get_fax_forward_no());
#ifdef META_END_WITH_JOBID
							set_metafile_end_info('F', IMAGE_LOG_JOB_TYPE_TX, RemoteNo, 0, NULL, gFaxRxTotalPages, fsu_get_unique_job_id(fsu_job_num_rcv));
#else
                            set_metafile_end_info('F', IMAGE_LOG_JOB_TYPE_TX, RemoteNo, 0, NULL, gFaxRxTotalPages);
#endif
                        }
#endif
                    }
#endif

                    if(TxRx_job_user_cancel)
                    {
                        TxRx_job_user_cancel = 0;
                        gFaxT30Result = FAX_ERROR_USER_ABORT_ERROR;
                    }

                    if(rx_docman_cleared == 1)
                    {
                        printf("\n(YHCHO) %s() rx_docman_cleared", __FUNCTION__);
                        rx_docman_cleared = 0;
                        gFaxRxTotalPages = 0;
                    }

                    printf("\n(YHCHO) %s() - gFaxT30Result: %d", __FUNCTION__, gFaxT30Result);
                    tx_rx_report_print_check = 1;
                    if (pEvent->dir == T30_TRANSMIT)
                    {

#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
                        printf("\n(YHCHO) Transmit (%d) End Time: %s\n", fsu_job_num_send, fsu_get_current_local_job_time_in_string());
#else
#endif

#if __MEASURE_PERFORMANCE__
                        if (fsu_get_flag_manual_tx(fsu_job_num_send) == 1)
                        {
                            faxapp_get_job_end_time(ON_HOOK);

                            printf("\n(YHCHO) %s() - Elapsed Time <ON HOOK>: %s\n", __FUNCTION__, faxapp_get_elapsed_time(ON_HOOK));
                        }
                        else
                        {
                            faxapp_get_job_end_time(T30_TRANSMIT);

                            printf("\n(YHCHO) %s() - Elapsed Time <MEM TO SEND>: %s\n", __FUNCTION__, faxapp_get_elapsed_time(T30_TRANSMIT));
                        }
#endif // #if __MEASURE_PERFORMANCE__

                        printf("\n(YHCHO) %s() - fsu_job_num_send: %d\n", __FUNCTION__, fsu_job_num_send);
                    }
                    else
                    {

#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
                        printf("\n(YHCHO) Receive (%d) End Time: %s\n", fsu_job_num_rcv, fsu_get_current_local_job_time_in_string());
#else
#endif

#if __MEASURE_PERFORMANCE__
                        faxapp_get_job_end_time(T30_RECEIVE);

                        printf("\n(YHCHO) %s() - Elapsed Time <RECEIVE TO MEM>: %s\n", __FUNCTION__, faxapp_get_elapsed_time(T30_RECEIVE));
#endif // #if __MEASURE_PERFORMANCE__

                        printf("\n(YHCHO) %s() - fsu_job_num_rcv: %d", __FUNCTION__, fsu_job_num_rcv);
                    }

                    gFaxT30Direction = pEvent->dir;

                    // To Process On Hook Jam...
                    if (pEvent->dir == T30_TRANSMIT) // Transmit
                    {
                        if ( (fsu_get_flag_manual_tx(fsu_job_num_send) == 1) && (gFaxAbnormalFlag == FAX_ABNORMAL_FLAG_SCAN_JAM) && (gFaxT30Result == FAX_ERROR_USER_ABORT_ERROR) )
                        {
                            gFaxT30Result = FAX_ERROR_DOCUMENT_JAM;

#if __TX_REPORT_IMAGE_PRINT__
                            if(getgFaxTxResultRptImage() == ENABLE)
                            {
                                tx_report_image_delete();
                            }
#endif
                        }
                    }
                    //

                    //To Flag Setting...
                    if (pEvent->dir == T30_RECEIVE) // Receive
                    {
                        if((fsu_current_funcs & FAX_MEM_TO_PRINT) && (fsu_job_num_rcv == fsu_job_num_prn))
                        {
                            if(gFaxT30Result != FAX_ERROR_NO_ERROR)
                            {
                                prn_cancel_flag = TRUE;//flag setting
                            }
                        }
                    }
                    //

                    if (pEvent->dir == T30_TRANSMIT) // Transmit
                    {
                        isReadyToTransmit = 0;

                        if (fsu_get_flag_manual_tx(fsu_job_num_send) == 1)
                        {
                            fsu_get_remote_no(FSU_JOB_TYPE_FOR_DIAL_TYPE_ON_HOOK, fsu_job_num_send);

                            fsu_get_job_type(JOB_TYPE_FOR_REPORT_ON_HOOK, fsu_job_num_send);
                        }
                        else
                        {
                            if (fsu_get_fax_forward(fsu_job_num_send) == 1)
                            {
                                fsu_get_remote_no(FSU_JOB_TYPE_FOR_DIAL_TYPE_TRANSMIT, fsu_job_num_send);

                                fsu_get_job_type(JOB_TYPE_FOR_REPORT_FORWARD, fsu_job_num_send);
                            }
                            else if (fsu_get_poll_transmit(fsu_job_num_send) == 1)
                            {
                                fsu_get_remote_no(FSU_JOB_TYPE_FOR_DIAL_TYPE_POLL_TRANSMIT, fsu_job_num_send);

                                fsu_get_job_type(JOB_TYPE_FOR_REPORT_POLL_TRANSMIT, fsu_job_num_send);
                            }
                            else
                            {
                                fsu_get_remote_no(FSU_JOB_TYPE_FOR_DIAL_TYPE_TRANSMIT, fsu_job_num_send);

                                fsu_get_job_type(JOB_TYPE_FOR_REPORT_TX, fsu_job_num_send);
                            }
                        }

                        fsu_get_page_no(gFaxTxCompletePageNo);
                    }
                    else // Receive
                    {
                        if (fsu_get_poll_receive(fsu_job_num_rcv) == 1)
                        {
                            fsu_get_job_type(JOB_TYPE_FOR_REPORT_POLL_RECEIVE, fsu_job_num_rcv);
                        }
                        else
                        {
                            fsu_get_job_type(JOB_TYPE_FOR_REPORT_RX, fsu_job_num_rcv);
                        }

                        fsu_get_remote_no(FSU_JOB_TYPE_FOR_DIAL_TYPE_RECEIVE, fsu_job_num_rcv);

                        fsu_get_page_no(gFaxRxTotalPages);
                    }

                    fSendConnectingMsg = 0;

                    if( pEvent->dir == T30_RECEIVE )
                    {
                        printf("\n(YHCHO) %s() - gFaxRxTotalPages: %d\n", __FUNCTION__, gFaxRxTotalPages);

                        if (fsu_get_fax_forward(fsu_job_num_rcv) == ENABLE)
                        {
                            if (gFaxRxTotalPages != 0)
                            {
                                fsu_update_total_pages(fsu_job_num_rcv, gFaxRxTotalPages);

                                fsu_update_fax_forward_job_queue(fsu_job_num_rcv);

                                Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_JOB_TYPE, fsu_job_num_rcv, IMAGE_BACKUP_JOB_TYPE_FORWARD);

                                Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_DIAL_NO, fsu_job_num_rcv, 0);

                                set_tx_job_ready(fsu_job_num_rcv);
                            }
                        }
                        else if (fsu_get_poll_receive(fsu_job_num_rcv) == 1)
                        {
                            if (gFaxRxTotalPages != 0)
                            {
                                Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_JOB_TYPE, fsu_job_num_rcv, IMAGE_BACKUP_JOB_TYPE_RCV_DONE);
                            }

                            fsu_reschedule_poll_receive(fsu_job_num_rcv);
                        }
                        else
                        {
                            if (gFaxRxTotalPages != 0)
                            {
                                fsu_update_total_pages(fsu_job_num_rcv, gFaxRxTotalPages);

                                if (getSaveFaxFile() == ENABLE)
                                {
                                    fsu_remove_rx_job(fsu_job_num_rcv);

                                    if (fsu_get_memory_full_status_ram() == 1)
                                    {
                                        printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);

                                        ; // Something should be added...

                                        return ;
                                    }
                                    else
                                    {
                                        UINT8 image_width = fsu_get_width(fsu_job_num_rcv);
                                        char targetPathName[MAX_LEN_FILE_PATH_NAME] = {0, };

                                        snprintf(targetPathName, sizeof(targetPathName), "%s%cImgDoc%04X", pRootFolderNameSolDocMan, PATH_SEPARATOR, fsu_job_num_rcv);

                                        printf("\n(YHCHO) %s() - targetPathName: %s\n", __FUNCTION__, targetPathName);

                                        fsu_update_status(fsu_job_num_rcv, FSU_STATE_SENDING_PENDING);

                                        fsu_make_dir_on_file_system(targetPathName);

                                        if (FileFormat == SAVE_FILE_FORMAT_PDF)
                                        {
#if __HANA_FINANCIAL_GROUP_REQ_021__
                                            // Step 1. make .end file
                                            if(isSolutionFax)
                                            {
                                                fsu_request_make_dot_end_file(NULL, fsu_job_num_rcv, RCV_TO_MEM);
                                            }
#endif
                                            if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
                                            {
                                                if(isSolutionFax)
                                                {
                                                    // Step 2. make .pdf file
                                                    faxapp_make_pdf_start(NULL,
																		  fsu_job_num_rcv,
																		  fsu_get_start_page_no_in_mem(fsu_get_doc_id(fsu_job_num_rcv), FAX_FILE_TYPE_JBG),
																		  gFaxRxTotalPages,
																		  fsu_get_width(fsu_job_num_rcv),
																		  SAVE_FILE_PAGE_TYPE_SINGLE_PAGE,
																		  REQUEST_SOURCE_FAX,
																		  FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION_FOR_HANA);
                                                }
                                                else
                                                {
                                                    faxapp_make_pdf_start(targetPathName, 
																			fsu_job_num_rcv, 
																			fsu_get_start_page_no_in_mem(fsu_get_doc_id(fsu_job_num_rcv), FAX_FILE_TYPE_JBG), 
																			gFaxRxTotalPages, 
																			image_width, 
																			PageType, 
																			REQUEST_SOURCE_FAX, 
																			FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION);
                                                }
                                            }
                                            else
                                            {
                                                UINT8 IndexToPageNo = 0;

                                                for (IndexToPageNo = 1; IndexToPageNo < gFaxRxTotalPages + 1; IndexToPageNo++)
                                                {
                                                    if(isSolutionFax)
                                                    {
                                                        // Step 2. make .pdf file
                                                        faxapp_make_pdf_start(NULL,
																				fsu_job_num_rcv,
																				IndexToPageNo,
																				1,
																				image_width,
																				PageType,
																				REQUEST_SOURCE_FAX,
																				FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION_FOR_HANA);
                                                    }
                                                    else
                                                    {
                                                        faxapp_make_pdf_start(targetPathName, 
																				fsu_job_num_rcv, 
																				IndexToPageNo, 
																				1, 
																				image_width, 
																				PageType, 
																				REQUEST_SOURCE_FAX, 
																				FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION);
                                                    }
                                                    tx_thread_sleep(1000); // To distinguish the output file name...
                                                }
                                            }

                                            faxapp_solution_request_send(fsu_job_num_rcv);
                                        }
                                        else
                                        {
                                            if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
                                            {
                                                faxapp_make_tif_start(targetPathName, fsu_job_num_rcv, fsu_get_start_page_no_in_mem(fsu_get_doc_id(fsu_job_num_rcv), FAX_FILE_TYPE_JBG), gFaxRxTotalPages, image_width, PageType);
                                            }
                                            else
                                            {
                                                UINT8 IndexToPageNo = 0;

                                                for (IndexToPageNo = 1; IndexToPageNo < gFaxRxTotalPages + 1; IndexToPageNo++)
                                                {
                                                    faxapp_make_tif_start(targetPathName, fsu_job_num_rcv, IndexToPageNo, 1, image_width, PageType);

                                                    tx_thread_sleep(1000); // To distinguish the output file name...
                                                }
                                            }

                                            faxapp_solution_request_send(fsu_job_num_rcv);
                                        }
                                    }
                                }
                                else
                                {
                                    fsu_set_rcv_job_done(fsu_job_num_rcv);
                                    tx_rx_report_print_check = 0;
                                }
                            }
                        }

                        if (gFaxT30Result == FAX_ERROR_NO_ERROR)
                        {
                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_RECEIVING_COMPLETE, 0, 0, 0, 0, fsu_job_num_rcv);

                            if (getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_ALL && !fsu_get_fax_forward(fsu_job_num_rcv) && getSaveFaxFile() == DISABLE)
                            {
                                fsu_set_next_prn_num(fsu_job_num_rcv);
                            }
                        }
                        else
                        {
                            if(gFaxT30Result != FAX_ERROR_USER_ABORT_ERROR)
                            {
                                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_RECEIVING_FAIL, 0, 0, 0, 0, fsu_job_num_rcv);
                                if (gFaxRxTotalPages == 0)
                                {
                                    char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
                                    char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };
                                    char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
                                    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
                                    UINT16 DocId = fsu_get_doc_id(fsu_job_num_rcv);

                                    snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

                                    snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

                                    if (fsu_check_doc_man_dir_in_nand(DocDirPathNAND) == TRUE)
                                    {
                                        UINT8 StartPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                                        UINT8 TotalPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                                        UINT8 IndexToPage = 0;

                                        for (IndexToPage = StartPageNo; IndexToPage < StartPageNo + TotalPageNo; IndexToPage++)
                                        {
                                            snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

                                            fsu_delete_file_on_file_system(jbgFileName);
                                        }

                                        snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);

                                        if (fsu_check_job_queue_file(JobQueueFileName) == TRUE)
                                        {
                                            fsu_delete_file_on_file_system(JobQueueFileName);
                                        }

                                        fsu_delete_dir_on_file_system(DocDirPathNAND);
                                    }

                                    if (fsu_check_doc_man_dir_in_ram(DocDirPathRAM) == TRUE)
                                    {
                                        fsu_delete_dir_on_file_system(DocDirPathRAM);
                                    }

                                    fsu_update_status(fsu_job_num_rcv, FSU_STATE_NULL);
                                }
                                else
                                {
                                    if (getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_ALL)
                                    {
                                        if (!fsu_get_fax_forward(fsu_job_num_rcv) && getSaveFaxFile() == DISABLE)
                                        {
                                            fsu_set_next_prn_num(fsu_job_num_rcv);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, 0, 0, 0, fsu_job_num_rcv);
                            }
                        }

                        gFaxRxTotalPages = 0;
                        gFaxRxCurrentPage = 0;
                    }
                    else
                    {
                        if (gFaxT30Result == FAX_ERROR_NO_ERROR)
                        {
                            //faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_COMPLETE, 0, gFaxTxCompletePageNo, 0, 0, fsu_job_num_send);

                            fsu_set_send_job_done(fsu_job_num_send);
                        }
                        else if(gFaxT30Result == FAX_ERROR_DOCUMENT_JAM)
                        {
                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_FAIL, 0, gFaxTxCompletePageNo, 0, 0, fsu_job_num_send);
                        }
                        else if(gFaxT30Result == FAX_ERROR_USER_ABORT_ERROR)
                        {
                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, gFaxTxCompletePageNo, 0, 0, fsu_job_num_send);
                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_JOB_CANCELED, 0, gFaxTxCompletePageNo, 0, 0, fsu_job_num_send);
                        }
                    }

                    isPhoneRingOn = 0;
                    gFaxCurrentSendPageNo = 0;

                    if (gFaxT30Result == FAX_ERROR_TIME_OUT_ERROR_11)
                    {
                        if (f_Rx_TSI_done == TRUE)
                        {
                            if (f_Rx_DCS_done == TRUE)
                            {
                                gFaxT30Result = FAX_ERROR_TIME_OUT_ERROR_11_1;
                            }
                            else
                            {
                                gFaxT30Result = FAX_ERROR_TIME_OUT_ERROR_11_2;
                            }
                        }
                        else
                        {
                            if (f_Rx_DCS_done == TRUE)
                            {
                                gFaxT30Result = FAX_ERROR_TIME_OUT_ERROR_11_3;
                            }
                            else
                            {
                                gFaxT30Result = FAX_ERROR_TIME_OUT_ERROR_11_4; // There is a chance for the remote machine a phone not a fax machine...
                            }
                        }

                        fsu_get_result(gFaxT30Result);
                    }
                    else
                    {
                        fsu_get_result(gFaxT30Result);
                    }

                    gFaxJobEndTimeForDuration = fsu_get_current_system_time();
                    fsu_get_duration();
                    fsu_get_modulation_name(gcpSpeedType);

                    if (pEvent->dir == T30_TRANSMIT)
                    {
                        if ((gFaxT30Result == FAX_ERROR_NO_ERROR && (fsu_get_broadcast(fsu_job_num_send) == 1)) || ((gFaxT30Result == FAX_ERROR_USER_ABORT_ERROR && fsu_get_broadcast(fsu_job_num_send) == 1) || (gFaxT30Result != FAX_ERROR_NO_ERROR && fsu_get_broadcast(fsu_job_num_send) == 1 && fsu_get_retry(fsu_job_num_send) == 0)))
                        {
                            UINT16 OriginalDocId = fsu_get_original_doc_id(fsu_job_num_send);

                            fsu_update_broadcast_result_report_struct(fsu_job_num_send, BROADCAST_RESULT_REPORT_REASON_JOB_ID);

                            fsu_update_broadcast_result_report_struct(gFaxJobEndTimeForDuration - gFaxJobStartTimeForDuration, BROADCAST_RESULT_REPORT_REASON_DURATION);

                            fsu_update_broadcast_result_report_struct(fsu_get_tx_total_page_no(fsu_job_num_send), BROADCAST_RESULT_REPORT_REASON_TOTAL_PAGE_NO);

                            if(gFaxT30Result == FAX_ERROR_NO_ERROR)
                            {
								fsu_update_broadcast_result_report_struct(gFaxSendCurrentPage, BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO);
                            }
                            else
                            {
								fsu_update_broadcast_result_report_struct(gFaxSendCurrentPage - 1, BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO);
							}

                            fsu_update_broadcast_result_report_struct(gFaxT30Result, BROADCAST_RESULT_REPORT_REASON_RESULT);

                            fsu_request_write_broadcast_result_report_info_to_db();

                            if (gFax_BC_Num[OriginalDocId].BC_Current_Num >= gFax_BC_Num[OriginalDocId].BC_Total_Num)
                            {
                                faxapp_process_broadcast_result_report(fsu_job_num_send);
                            }
                        }
                    }

                    if (pEvent->dir == T30_TRANSMIT)
                    {
                        if ( (gFaxT30Result == FAX_ERROR_USER_ABORT_ERROR) || (fsu_get_flag_manual_tx(fsu_job_num_send) == 1) )
                        {
#if __TX_REPORT_IMAGE_PRINT__
                            if(getgFaxTxResultRptImage() == ENABLE)
                            {
                                tx_report_image_scaling(fsu_job_num_send);
                            }
#endif
                            fsu_write_report_info_to_db(FAX_REPORT_TCR); // Update RECD_FAXLOG...

                            fsu_delete_report_record_from_db(FAX_REPORT_TCR); // Delete RECD_FAXLOG if it has more than 35 records...
                        }
                        else
                        {
                            if ( (fsu_get_retry(fsu_job_num_send) != 0) && (gFaxT30Result != FAX_ERROR_NO_ERROR) && (fsu_get_poll_transmit(fsu_job_num_send) == 0) )
                            {
                                printf("\n(YHCHO) %s() - No Action Here (1).. since the automatic retry will begin...", __FUNCTION__);
                            }
                            else
                            {
#if __TX_REPORT_IMAGE_PRINT__
                                if(getgFaxTxResultRptImage() == ENABLE)
                                {
                                    tx_report_image_scaling(fsu_job_num_send);
                                }
#endif
                                fsu_write_report_info_to_db(FAX_REPORT_TCR); // Update RECD_FAXLOG...

                                fsu_delete_report_record_from_db(FAX_REPORT_TCR); // Delete RECD_FAXLOG if it has more than 35 records...
                            }
                        }
                    }
                    else
                    {
                        if (gFaxT30Result == FAX_ERROR_TIME_OUT_ERROR_11_4)
                        {
                            // No Action Here.. In this case, this kind of error should not be inserted into RECD_FAXLOG.
                        }
                        else
                        {
#if __TX_REPORT_IMAGE_PRINT__
                            if(getgFaxTxResultRptImage() == ENABLE)
                            {
                                tx_report_image_delete();
                            }
#endif						
                            fsu_write_report_info_to_db(FAX_REPORT_TCR); // Update RECD_FAXLOG...

                            fsu_delete_report_record_from_db(FAX_REPORT_TCR); // Delete RECD_FAXLOG if it has more than 35 records...
                        }
                    }

                    fsu_write_report_info_to_db(FAX_REPORT_TCR_FOR_FAX_SERVICE); // Update RECD_FAXSPLOG...

                    fsu_delete_report_record_from_db(FAX_REPORT_TCR_FOR_FAX_SERVICE); // Delete RECD_FAXSPLOG if it has more than 700 records...

                    if (pEvent->dir == T30_TRANSMIT)
                    {
                        if ( (gFaxT30Result == FAX_ERROR_USER_ABORT_ERROR) || (fsu_get_flag_manual_tx(fsu_job_num_send) == 1) )
                        {
                            if(tx_rx_report_print_check)
                            {
                                faxapp_process_tx_rx_result_report();
                            }

                            faxapp_process_tcr();
                        }
                        else
                        {
                            if ( (fsu_get_retry(fsu_job_num_send) != 0) && (gFaxT30Result != FAX_ERROR_NO_ERROR) && (fsu_get_poll_transmit(fsu_job_num_send) == 0) )
                            {
                                printf("\n(YHCHO) %s() - No Action Here (2).. since the automatic retry will begin...", __FUNCTION__);
                            }
                            else
                            {
                                if(tx_rx_report_print_check)
                                {
                                    faxapp_process_tx_rx_result_report();
                                }

                                faxapp_process_tcr();
                            }
                        }
                    }
                    else
                    {
                        if (gFaxT30Result == FAX_ERROR_TIME_OUT_ERROR_11_4)
                        {
                            // No Action Here.. In this case, this kind of error should not be inserted into RECD_FAXLOG.
                        }
                        else
                        {
                            if(tx_rx_report_print_check)
                            {
                                faxapp_process_tx_rx_result_report();
                            }

                            faxapp_process_tcr();
                        }
                    }

                    if (pEvent->dir == T30_TRANSMIT)
                    {
                        if (fsu_get_is_delay(fsu_job_num_send) == 1)
                        {
                            fsu_update_delay_time_for_job_log(1, fsu_job_num_send);
                        }
                        else
                        {
                            fsu_update_delay_time_for_job_log(0, fsu_job_num_send);
                        }
                    }
                    else
                    {
                        fsu_update_delay_time_for_job_log(0, fsu_job_num_rcv);
                    }

                    if (pEvent->dir == T30_TRANSMIT)
                    {
                        fsu_update_user_id_job_log(fsu_job_num_send);
                    }
                    else
                    {
                        fsu_update_user_id_job_log(fsu_job_num_rcv);
                    }

                    faxapp_update_recd_fax_job_log(&FaxJobLog);

                    f_Rx_DCS_done = FALSE;
                    f_Rx_TSI_done = FALSE;

                    memset(gcTSI_for_RemoteFaxNum, 0, sizeof(gcTSI_for_RemoteFaxNum));
                    memset(gcCIG_for_RemoteFaxNum, 0, sizeof(gcCIG_for_RemoteFaxNum));
                    memset(gcCID_for_RemoteFaxNum, 0, sizeof(gcCID_for_RemoteFaxNum));

                    total_size = 0;
                    NonECM_total_size = 0;

                    gFaxJobEndTimeForDuration = 0;
                    gFaxJobStartTimeForDuration = 0;
                    gFaxSendCurrentPageOnHook = 0;
                    RTNCount = 0;
                    gFaxTxCompletePageNo = 0;
                    gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_NONE;
                    isTxImageProcessDone = 0;
                    gFaxCurrentTTIPageNo = 0;
                    isFSUTxImageProgessBegin = 1;
                    gFSUTxUpdatePageCnt = 0;
                    rx_docman_cleared = 0;
                    isExceptionHandleBegin = 0;
                    isExceptionHandleDone = 0;

                    fsu_get_memory_full_status_nand();

                    break;
                }

                case TSI:
                {
                    UINT8 Len = 0;

                    printf("\n(YHCHO) %s() - TSI\n", __FUNCTION__);

                    Len = strlen(gcCID_for_RemoteFaxNum);

                    //printf("\n(YHCHO) %s() - TSI -> Len: %d\n", __FUNCTION__, Len);

                    if (Len != 0)
                    {
                        break;
                    }

                    if( pEvent->dir == T30_RECEIVE )
                    {
                        UINT32 SharedMemID = 0;

                        f_Rx_TSI_done = TRUE;

                        memset (gcTSI_for_RemoteFaxNum, 0, sizeof(gcTSI_for_RemoteFaxNum));

                        j = 0;

                        for (i = 0; i < FCFSendTableSize; i++)
                        {
                            if (pEvent->size <= (i+1))
                                break;

                            switch (pEvent->data[(pEvent->size-i-1)])
                            {
                                case '0':
                                case '1':
                                case '2':
                                case '3':
                                case '4':
                                case '5':
                                case '6':
                                case '7':
                                case '8':
                                case '9':
                                case '+':
                                    gcTSI_for_RemoteFaxNum[j] = pEvent->data[(pEvent->size-i-1)];
                                    j++;
                                    break;

                                default:
                                    break;
                            }
                        }

                        fsu_update_dialno(fsu_job_num_rcv, (UINT8 *)gcTSI_for_RemoteFaxNum);

                        if (j == 0)
                        {
                            gcTSI_for_RemoteFaxNum[0] = 0x20; // Add one space..
                            gcTSI_for_RemoteFaxNum[1] = 0;
                        }

                        SharedMemID = faxapp_create_shared_mem_for_display_receive(gcTSI_for_RemoteFaxNum);

                        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DIAL_NO_RECEIVE, 0, 0, 0, SharedMemID, fsu_job_num_rcv);
                    }

                    break;
                }

                case CIG:
                {
                    UINT8 Len = 0;

                    printf("\n(YHCHO) %s() - CIG\n", __FUNCTION__);

                    Len = strlen(gcCID_for_RemoteFaxNum);

                    //printf("\n(YHCHO) %s() - CIG -> Len: %d\n", __FUNCTION__, Len);

                    if (Len != 0)
                    {
                        break;
                    }

                    if( pEvent->dir == T30_TRANSMIT )
                    {
                        UINT32 SharedMemID = 0;

                        memset (gcCIG_for_RemoteFaxNum, 0x00, sizeof(gcCIG_for_RemoteFaxNum));

                        j = 0;

                        for (i = 0; i < FCFSendTableSize; i++)
                        {
                            if (pEvent->size <= (i+1))
                                break;

                            switch (pEvent->data[(pEvent->size-i-1)])
                            {
                                case '0':
                                case '1':
                                case '2':
                                case '3':
                                case '4':
                                case '5':
                                case '6':
                                case '7':
                                case '8':
                                case '9':
                                case '+':
                                    gcCIG_for_RemoteFaxNum[j] = pEvent->data[(pEvent->size-i-1)];
                                    j++;
                                    break;

                                default:
                                    break;
                            }
                        }

                        fsu_update_dialno(fsu_job_num_rcv, (UINT8 *)gcCIG_for_RemoteFaxNum);

                        if (j == 0)
                        {
                            gcCIG_for_RemoteFaxNum[0] = 0;//0x20; // Add one space..
                            gcCIG_for_RemoteFaxNum[1] = 0;
                        }

                        SharedMemID = faxapp_create_shared_mem_for_display_receive(gcCIG_for_RemoteFaxNum);

                        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DIAL_NO_RECEIVE, 0, 0, 0, SharedMemID, fsu_job_num_rcv);

                        break;
                    }
                }

                case V34CC:
                {
                    printf("\n(YHCHO) %s() - V34CC\n", __FUNCTION__);

                    gcpSpeedType = 0;

                    break;
                }

                case V34PC:
                {
                    printf("\n(YHCHO) %s() - V34PC\n", __FUNCTION__);

                    gcpSpeedType = 0;

                    break;
                }

                default:
                    break;
            }

            break;
        }
    }
}

int getFAX_PageNum_ProtocolDumpList(void)
{
    int page_num = 0;
    int extra_lines = 0;

    if( total_dump_cnt >= MAX_DUMP_LIST_LINE )
    {
        page_num = MAX_DUMP_LIST_LINE / MAX_DUMP_PAGE_LINE;
        extra_lines = MAX_DUMP_LIST_LINE % MAX_DUMP_PAGE_LINE;
    }
    else
    {
        page_num = total_dump_cnt / MAX_DUMP_PAGE_LINE;
        extra_lines = total_dump_cnt % MAX_DUMP_PAGE_LINE;
    }

    if( extra_lines )
        page_num++;

    if( page_num == 0 )
        page_num = 1;

    printf("\n\n\n=======================>(DumpList)Total Page Number:%d\n\n\n",page_num);

    return page_num;
}


int FAX_StringGenerator_ProtocolDumpList(T30_LOG_REPORT_t* DestBuf, unsigned int pageNum, unsigned int targetPageNum, T30_LOG_RESULT_t* ResultBuf)
{
    UINT16 i, j, k;
    static UINT16 start_pos, curr_pos;
    UINT16 total_cnt;
    char direction[10];
    char fcf_data[10];
    char fif_data[60];
    char fif_data2[60];
    char tmp_data[10];
    static char temp_connectspeed[15];				// add 12-16			// edit 02-07
    char temp_resolution[10];				// add 12-16
    char temp_ECM[5];						// add 12-16
    char temp_CodingType[5]; //add.bong.2011-02-15 8:11AM
    char temp_Result[20];					// add 12-16
    char temp_speed[10]; //add.bong2011-02-15 9:38AM
    char OneLineText[97];		//One line data
    char TimeText[12];

    int rowCount = 0;

    if( total_dump_cnt == 0 )
    {
        snprintf (ResultBuf->Speed, sizeof(ResultBuf->Speed), "-");
        snprintf (ResultBuf->Resolution, sizeof(ResultBuf->Resolution), "-");
        snprintf (ResultBuf->Mode, sizeof(ResultBuf->Mode), "-");
        snprintf (ResultBuf->Coding, sizeof(ResultBuf->Coding), "-");
        snprintf (ResultBuf->Result, sizeof(ResultBuf->Result), "-");

        return rowCount;
    }

    if( pageNum == 1 )
    {
        if( total_dump_cnt >= MAX_DUMP_LIST_LINE )
        {
            start_pos = curr_pos = dump_list_cnt;
        }
        else
        {
            start_pos = curr_pos = 0;
        }
    }

    if( pageNum == targetPageNum )
    {
        if( total_dump_cnt >= MAX_DUMP_LIST_LINE )
        {
            total_cnt = MAX_DUMP_PAGE_LINE;
        }
        else
        {
            total_cnt = total_dump_cnt % MAX_DUMP_PAGE_LINE;
            if( total_cnt == 0 )
            {
                total_cnt = MAX_DUMP_PAGE_LINE;
            }
        }
    }
    else
    {
        total_cnt = MAX_DUMP_PAGE_LINE;
    }

    printf("\n\n\n %s() - total_cnt: %d", __FUNCTION__, total_cnt);

    for( i=0; i<total_cnt; i++ )	//mod.bong.2010-11-11 4:10PM
    {
        if( dump_list[curr_pos].dir )
        {
            snprintf(direction, sizeof(direction), "S");
        }
        else
        {
            snprintf(direction, sizeof(direction), "R");
        }

        memset(fif_data,0x00,sizeof(fif_data));

        memset(fif_data2,0x00,sizeof(fif_data2));

        memset(OneLineText, 0x00, sizeof(OneLineText));

        printf("\n(YHCHO) %s() - dump_list[%d].fcf: %d\n", __FUNCTION__, curr_pos, dump_list[curr_pos].fcf);

        switch( dump_list[curr_pos].fcf )
        {
            case PHONE_CNG:
                snprintf(fcf_data, sizeof(fcf_data), "CNG");
                break;

            case PHONE_DIAL:
#if 0
                snprintf(OneLineText, sizeof(OneLineText), "%08d  %-3s   Fax Number: %s", (dump_list[curr_pos].time%100000000), direction, pDialNo);
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128], OneLineText, strlen(OneLineText));
#endif
                snprintf(TimeText, sizeof(TimeText), "%08ld", (dump_list[curr_pos].time%100000000));
                strncpy(DestBuf[rowCount].Time,TimeText,strlen(TimeText));

                snprintf(DestBuf[rowCount].S_R, sizeof(DestBuf[rowCount].S_R), "%-3s", direction);
                snprintf(DestBuf[rowCount].FCFData, sizeof(DestBuf[rowCount].FCFData), "Fax Number: %s", (char *)fsu_get_dial_no(fsu_job_num_send));
                rowCount++;

                curr_pos++;
                curr_pos = curr_pos % MAX_DUMP_LIST_LINE;
                continue;

            case CED:
                snprintf(fcf_data,sizeof(fcf_data),"CED");
                break;

            case TCF:
                snprintf(fcf_data,sizeof(fcf_data),"TCF");
                break;

            case IMG:
                snprintf(fcf_data,sizeof(fcf_data),"IMG");

                memset(temp_connectspeed, 0, sizeof(temp_connectspeed));
                if( dump_list[curr_pos].size )
                {
                    switch( dump_list[curr_pos].fif[0] )
                    {
                        case 2:		snprintf(temp_connectspeed,sizeof(temp_connectspeed),"2400");	break;
                        case 4:		snprintf(temp_connectspeed,sizeof(temp_connectspeed),"4800");	break;
                        case 7:		snprintf(temp_connectspeed,sizeof(temp_connectspeed),"7200");	break;
                        case 9:		snprintf(temp_connectspeed,sizeof(temp_connectspeed),"9600");	break;
                        case 12:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"12000");	break;
                        case 14:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"14400");	break;
                        case 16:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"16800");	break;
                        case 19:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"19200");	break;
                        case 21:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"21600");	break;
                        case 24:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"24000");	break;
                        case 26:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"26400");	break;
                        case 28:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"28800");	break;
                        case 31:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"31200");	break;
                        case 33:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"33600");	break;
                        default:	snprintf(temp_connectspeed,sizeof(temp_connectspeed),"-");		break;
                    }
                }
#if 0
                snprintf (OneLineText,sizeof(OneLineText),
                    "%08d  %-3s   %-8s        %s"
                        ,(dump_list[curr_pos].time%100000000),direction,fcf_data,temp_connectspeed
                        );
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
#endif
                snprintf(TimeText, sizeof(TimeText), "%08ld", (dump_list[curr_pos].time%100000000));
                strncpy(DestBuf[rowCount].Time,TimeText,strlen(TimeText));

                snprintf(DestBuf[rowCount].S_R, sizeof(DestBuf[rowCount].S_R), "%-3s", direction);
                snprintf(DestBuf[rowCount].FCFData, sizeof(DestBuf[rowCount].FCFData), "%-8s", fcf_data);
                snprintf(DestBuf[rowCount].FIFData, sizeof(DestBuf[rowCount].FIFData), "%s", temp_connectspeed);
                rowCount++;


                curr_pos++;
                curr_pos = curr_pos % MAX_DUMP_LIST_LINE;
                continue;

            case ANSam:
                snprintf(fcf_data,sizeof(fcf_data),"ANSam");
                break;

            case V8CM:
                snprintf(fcf_data,sizeof(fcf_data),"V8CM");
                break;

            case V8JM:
                snprintf(fcf_data,sizeof(fcf_data),"V8JM");
                break;

            case V8CJ:
                snprintf(fcf_data,sizeof(fcf_data),"V8CJ");
                break;

            case V8CI:
                snprintf(fcf_data,sizeof(fcf_data),"V8CI");
                break;

            case V34P2:
                snprintf(fcf_data,sizeof(fcf_data),"V34P2");
                break;

            case V34P3:
                snprintf(fcf_data,sizeof(fcf_data),"V34P3");
                break;

            case V34C1:
                snprintf(fcf_data,sizeof(fcf_data),"V34C1");
                break;

            case V34CC:
                snprintf(fcf_data,sizeof(fcf_data),"V34CC");
                break;

            case V34P1:
                snprintf(fcf_data,sizeof(fcf_data),"V34P1");
                break;

            case V34PC:
                snprintf(fcf_data,sizeof(fcf_data),"V34PC");
                break;

            case CFR:
                snprintf(fcf_data,sizeof(fcf_data),"CFR");
                break;

            case CIG:
                snprintf(fcf_data,sizeof(fcf_data),"CIG");
                break;

            case CRP:
                snprintf(fcf_data,sizeof(fcf_data),"CRP");
                break;

            case CSI:
                snprintf(fcf_data,sizeof(fcf_data),"CSI");
                break;

            case CTC:
                snprintf(fcf_data,sizeof(fcf_data),"CTC");
                break;

            case CTR:
                snprintf(fcf_data,sizeof(fcf_data),"CTR");
                break;

            case DCN:
                snprintf(fcf_data,sizeof(fcf_data),"DCN");
                break;

            case DCS:
                snprintf(fcf_data,sizeof(fcf_data),"DCS");
                if( dump_list[curr_pos].size > 1 )
                {
                    UINT8 input;

                    for( j=1; j<dump_list[curr_pos].size; j++ )	//mod.bong.2010-11-16 7:02AM
                    {
                        input = dump_list[curr_pos].fif[j];	//mod.bong.2010-11-11 2:22PM
                        memset(tmp_data,0x00,sizeof(tmp_data));
                        for( k=0; k<8; k++ )
                        {
                            if( input & (1<<k) )
                                tmp_data[k] = '1';
                            else
                                tmp_data[k] = '0';
                        }
                        tmp_data[k] = ' ';
                        if( j < 6 )		//mod.bong.2010-11-16 7:03AM
                            strncat(fif_data,tmp_data,strlen(tmp_data));
                        else
                            strncat(fif_data2,tmp_data,strlen(tmp_data));
                    }
                }
#if 0
                snprintf (OneLineText,sizeof(OneLineText),
                        "%08d  %-3s   %-8s       %s"
                        ,(dump_list[curr_pos].time%100000000),direction,fcf_data,fif_data
                        );
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));

                snprintf (OneLineText,sizeof(OneLineText),
                        "                                          %s"
                        ,fif_data2
                        );
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
#endif
                snprintf(TimeText, sizeof(TimeText), "%08ld", (dump_list[curr_pos].time%100000000));
                strncpy(DestBuf[rowCount].Time,TimeText,strlen(TimeText));

                snprintf(DestBuf[rowCount].S_R, sizeof(DestBuf[rowCount].S_R), "%-3s", direction);
                snprintf(DestBuf[rowCount].FCFData, sizeof(DestBuf[rowCount].FCFData), "%-8s", fcf_data);
                snprintf(DestBuf[rowCount].FIFData, sizeof(DestBuf[rowCount].FIFData), "%s", fif_data);
                snprintf(DestBuf[rowCount].FIFData2, sizeof(DestBuf[rowCount].FIFData2), "%s", fif_data2);
                rowCount++;


                curr_pos++;											// add 01-07
                curr_pos = curr_pos % MAX_DUMP_LIST_LINE;			// add 01-07

                continue;

            case DIS:
                snprintf(fcf_data,sizeof(fcf_data),"DIS");
                if( dump_list[curr_pos].size > 1 )
                {
                    UINT8 input;

                    for( j=1; j<dump_list[curr_pos].size; j++ )	//mod.bong.2010-11-11 2:24PM
                    {
                        input = dump_list[curr_pos].fif[j];	//mod.bong.2010-11-11 2:24PM
                        memset(tmp_data,0x00,sizeof(tmp_data));
                        for( k=0; k<8; k++ )
                        {
                            if( input & (1<<k) )
                                tmp_data[k] = '1';
                            else
                                tmp_data[k] = '0';
                        }
                        tmp_data[k] = ' ';
                        if( j < 6 )		//mod.bong.2010-11-11 2:24PM
                            strncat(fif_data,tmp_data,strlen(tmp_data));
                        else
                            strncat(fif_data2,tmp_data,strlen(tmp_data));
                    }
                }
#if 0
                snprintf (OneLineText,sizeof(OneLineText),
                        "%08d  %-3s   %-8s        %s"
                        ,(dump_list[curr_pos].time%100000000),direction,fcf_data,fif_data
                        );
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));

                snprintf (OneLineText,sizeof(OneLineText),
                        "                                          %s"
                        ,fif_data2
                        );
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
    //			break;
#endif
                snprintf(TimeText, sizeof(TimeText), "%08ld", (dump_list[curr_pos].time%100000000));
                strncpy(DestBuf[rowCount].Time,TimeText,strlen(TimeText));

                snprintf(DestBuf[rowCount].S_R, sizeof(DestBuf[rowCount].S_R), "%-3s", direction);
                snprintf(DestBuf[rowCount].FCFData, sizeof(DestBuf[rowCount].FCFData), "%-8s", fcf_data);
                snprintf(DestBuf[rowCount].FIFData, sizeof(DestBuf[rowCount].FIFData), "%s", fif_data);
                snprintf(DestBuf[rowCount].FIFData2, sizeof(DestBuf[rowCount].FIFData2), "%s", fif_data2);
                rowCount++;

                curr_pos++;									// add 01-07
                curr_pos = curr_pos % MAX_DUMP_LIST_LINE;	// add 01-07
                continue;

            case DTC:										// add 04-21
                snprintf(fcf_data,sizeof(fcf_data),"DTC");
                if( dump_list[curr_pos].size > 1 )
                {
                    UINT8 input;

                    for( j=1; j<dump_list[curr_pos].size; j++ )	//mod.bong.2010-11-11 2:24PM
                    {
                        input = dump_list[curr_pos].fif[j];	//mod.bong.2010-11-11 2:24PM
                        memset(tmp_data,0x00,sizeof(tmp_data));
                        for( k=0; k<8; k++ )
                        {
                            if( input & (1<<k) )
                                tmp_data[k] = '1';
                            else
                                tmp_data[k] = '0';
                        }
                        tmp_data[k] = ' ';
                        if( j < 6 )		//mod.bong.2010-11-11 2:24PM
                            strncat(fif_data,tmp_data,strlen(tmp_data));
                        else
                            strncat(fif_data2,tmp_data,strlen(tmp_data));
                    }
                }
#if 0
                snprintf (OneLineText,sizeof(OneLineText),
                        "%08d  %-3s   %-8s     %s"
                        ,(dump_list[curr_pos].time%100000000),direction,fcf_data,fif_data
                        );
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));

                snprintf (OneLineText,sizeof(OneLineText),
                        "                                   %s"
                        ,fif_data2
                        );
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
#endif
                snprintf(TimeText, sizeof(TimeText), "%08ld", (dump_list[curr_pos].time%100000000));
                strncpy(DestBuf[rowCount].Time,TimeText,strlen(TimeText));

                snprintf(DestBuf[rowCount].S_R, sizeof(DestBuf[rowCount].S_R), "%-3s", direction);
                snprintf(DestBuf[rowCount].FCFData, sizeof(DestBuf[rowCount].FCFData), "%-8s", fcf_data);
                snprintf(DestBuf[rowCount].FIFData, sizeof(DestBuf[rowCount].FIFData), "%s", fif_data);
                snprintf(DestBuf[rowCount].FIFData2, sizeof(DestBuf[rowCount].FIFData2), "%s", fif_data2);
                rowCount++;

                curr_pos++;									// add 01-07
                curr_pos = curr_pos % MAX_DUMP_LIST_LINE;	// add 01-07
                continue;
                break;

            case EOM:
                snprintf(fcf_data,sizeof(fcf_data),"EOM");
                break;

            case EOP:
                snprintf(fcf_data,sizeof(fcf_data),"EOP");
                break;

            case EOR:
                snprintf(fcf_data,sizeof(fcf_data),"EOR");
                break;

            case ERR:
                snprintf(fcf_data,sizeof(fcf_data),"ERR");
                break;

            case FCD:
                snprintf(fcf_data,sizeof(fcf_data),"FCD");
                break;

            case FTT:
                snprintf(fcf_data,sizeof(fcf_data),"FTT");
                break;

            case MCF:
                snprintf(fcf_data,sizeof(fcf_data),"MCF");
                break;

            case MPS:
                snprintf(fcf_data,sizeof(fcf_data),"MPS");
                break;

            case NSC:
                snprintf(fcf_data,sizeof(fcf_data),"NSC");
                break;

            case NSF:
                snprintf(fcf_data,sizeof(fcf_data),"NSF");
                break;

            case NSS:
                snprintf(fcf_data,sizeof(fcf_data),"NSS");
                break;

            case PIN:
                snprintf(fcf_data,sizeof(fcf_data),"PIN");
                break;

            case PIP:
                snprintf(fcf_data,sizeof(fcf_data),"PIP");
                break;

            case PPR:
                snprintf(fcf_data,sizeof(fcf_data),"PPR");

                if( dump_list[curr_pos].size > 1 )
                {
                    for( j=1; j<dump_list[curr_pos].size; j++ )	//mod.bong.2010-11-16 7:04AM
                    {
                        if( j > 19 )	break;
                        memset(tmp_data,0x00,sizeof(tmp_data));
                        snprintf(tmp_data,sizeof(tmp_data),"%02x",dump_list[curr_pos].fif[j]);	//mod.bong.2010-11-11 2:26PM
                        strncat(fif_data,tmp_data,strlen(tmp_data));
                    }
                }
#if 0
                snprintf (OneLineText,sizeof(OneLineText),
                        "%08d  %-3s   %-8s        %s"
                        ,(dump_list[curr_pos].time%100000000),direction,fcf_data,fif_data
                        );
                printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
                memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
#endif
                snprintf(TimeText, sizeof(TimeText), "%08ld", (dump_list[curr_pos].time%100000000));
                strncpy(DestBuf[rowCount].Time,TimeText,strlen(TimeText));

                snprintf(DestBuf[rowCount].S_R, sizeof(DestBuf[rowCount].S_R), "%-3s", direction);
                snprintf(DestBuf[rowCount].FCFData, sizeof(DestBuf[rowCount].FCFData), "%-8s", fcf_data);
                snprintf(DestBuf[rowCount].FIFData, sizeof(DestBuf[rowCount].FIFData), "%s", fif_data);
                rowCount++;

                curr_pos++;
                curr_pos = curr_pos % MAX_DUMP_LIST_LINE;
                continue;

            case PPS:
                if( dump_list[curr_pos].size > 2 )
                {
                    switch( dump_list[curr_pos].fif[1] )	//mod.bong.2010-11-16 7:04AM
                    {
                    case 0x00:
                        snprintf(fcf_data,sizeof(fcf_data),"PPS-NUL");
                        break;
                    case 0x1F:	//mod.bong.2010-11-11 2:25PM
                        snprintf(fcf_data,sizeof(fcf_data),"PPS-EOS");
                        break;
                    case 0x2F:	//mod.bong.2010-11-11 2:25PM
                        snprintf(fcf_data,sizeof(fcf_data),"PPS-EOP");
                        break;
                    case 0x4F:	//mod.bong.2010-11-11 2:25PM
                        snprintf(fcf_data,sizeof(fcf_data),"PPS-MPS");
                        break;
                    case 0x8F:	//mod.bong.2010-11-11 2:25PM
                        snprintf(fcf_data,sizeof(fcf_data),"PPS-EOM");
                        break;
                    default:
                        snprintf(fcf_data,sizeof(fcf_data),"PPS-??");
                        break;
                    }
                }
                else
                {
                    snprintf(fcf_data,sizeof(fcf_data),"PPS");
                }
                break;

            case PRI_EOM:
                snprintf(fcf_data,sizeof(fcf_data),"PRI-EOM");
                break;

            case PRI_EOP:
                snprintf(fcf_data,sizeof(fcf_data),"PRI-EOP");
                break;

            case PRI_MPS:
                snprintf(fcf_data,sizeof(fcf_data),"PRI-MPS");
                break;

            case RCP:
                snprintf(fcf_data,sizeof(fcf_data),"RCP");
                break;

            case RNR:
                snprintf(fcf_data,sizeof(fcf_data),"RNR");
                break;

            case RR:
                snprintf(fcf_data,sizeof(fcf_data),"RR");
                break;

            case RTN:
                snprintf(fcf_data,sizeof(fcf_data),"RTN");
                break;

            case RTP:
                snprintf(fcf_data,sizeof(fcf_data),"RTP");
                break;

            case TSI:
                snprintf(fcf_data,sizeof(fcf_data),"TSI");
                break;

            default:
                curr_pos++;									// add 01-07
                curr_pos = curr_pos % MAX_DUMP_LIST_LINE;	// add 01-07
                continue;
        }

        if( dump_list[curr_pos].size > 1 )
        {
            for( j=1; j<dump_list[curr_pos].size; j++ )	//mod.bong.2010-11-16 7:04AM
            {
                if( j > 19 )	break;
                memset(tmp_data,0x00,sizeof(tmp_data));
                snprintf(tmp_data,sizeof(tmp_data),"%02x ",dump_list[curr_pos].fif[j]);	//mod.bong.2010-11-11 2:26PM
                strncat(fif_data,tmp_data,strlen(tmp_data));
            }
        }
#if 0
        if (dump_list[curr_pos].fcf == PHONE_CNG)
        {
            snprintf (OneLineText, sizeof(OneLineText), "%08d  %-3s  %-8s       %s\n", (gFaxCNGStartTime%100000000), direction, fcf_data, fif_data);
        }
        else
        {
            snprintf (OneLineText, sizeof(OneLineText), "%08d  %-3s   %-8s       %s", (dump_list[curr_pos].time%100000000), direction, fcf_data, fif_data);
        }
        printf("\n(YHCHO) %s() : %s\n", __FUNCTION__, OneLineText);
        memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
#endif
        if (dump_list[curr_pos].fcf == PHONE_CNG)
        {
            snprintf(TimeText, sizeof(TimeText), "%08ld", (gFaxCNGStartTime%100000000));
            strncpy(DestBuf[rowCount].Time,TimeText,strlen(TimeText));
        }
        else
        {
            snprintf(TimeText, sizeof(TimeText), "%08ld", (dump_list[curr_pos].time%100000000));
            strncpy(DestBuf[rowCount].Time,TimeText,strlen(TimeText));
        }

        snprintf(DestBuf[rowCount].S_R, sizeof(DestBuf[rowCount].S_R), "%-3s", direction);
        snprintf(DestBuf[rowCount].FCFData, sizeof(DestBuf[rowCount].FCFData), "%-8s", fcf_data);
        snprintf(DestBuf[rowCount].FIFData, sizeof(DestBuf[rowCount].FIFData), "%s", fif_data);
        rowCount++;

        curr_pos++;
        curr_pos = curr_pos % MAX_DUMP_LIST_LINE;
    }

    if( pageNum == targetPageNum )
    {
        // Step 1. Blank Line

        // Step 2. Connected Speed
        switch ( gcpSpeedType )
        {
            case 0:
                snprintf(temp_speed, sizeof(temp_speed), "V.34");
                break;

            case 1:
                snprintf(temp_speed, sizeof(temp_speed), "V.17");
                break;

            case 2:
                snprintf(temp_speed, sizeof(temp_speed), "V.29");
                break;

            case 3:
                snprintf(temp_speed, sizeof(temp_speed), "V.27ter");
                break;

            default:
                snprintf(temp_speed, sizeof(temp_speed), "-");
                break;
        }
//		snprintf (OneLineText, sizeof(OneLineText), "SPEED           : %s-%s", temp_speed, temp_connectspeed);
//		memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
        snprintf (ResultBuf->Speed, sizeof(ResultBuf->Speed), "%s-%s", temp_speed, temp_connectspeed);

        // Step 3. Resolution
        switch ( gFaxT30Resolution )
        {
            case 0:
                snprintf(temp_resolution, sizeof(temp_resolution), "STANDARD");
                break;

            case 1:
                snprintf(temp_resolution, sizeof(temp_resolution), "FINE");
                break;

            case 3:
                snprintf(temp_resolution, sizeof(temp_resolution), "300-DPI");
                break;

            default:
                snprintf(temp_resolution, sizeof(temp_resolution), "-");
                break;
        }
//		snprintf (OneLineText, sizeof(OneLineText), "RESOLUTION    : %s", temp_resolution);
//		memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
        snprintf (ResultBuf->Resolution, sizeof(ResultBuf->Resolution), "%s", temp_resolution);

        // Step 4. ECM
        switch ( gcpEcmModeType )
        {
            case 1:
                snprintf(temp_ECM, sizeof(temp_ECM), "ON");
                break;

            case 0:
                snprintf(temp_ECM, sizeof(temp_ECM), "OFF");
                break;

            default:
                snprintf(temp_ECM, sizeof(temp_ECM), "-");
                break;
        }
//		snprintf (OneLineText, sizeof(OneLineText), "MODE            : ECM %s", temp_ECM);
//		memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
        snprintf (ResultBuf->Mode, sizeof(ResultBuf->Mode), "ECM %s", temp_ECM);

        // Step 5. Coding Type
        switch ( gcpCodingType )
        {
            case 0:
                snprintf(temp_CodingType, sizeof(temp_CodingType), "MH");
                break;

            case 1:
                snprintf(temp_CodingType, sizeof(temp_CodingType), "MR");
                break;

            case 2:
                snprintf(temp_CodingType, sizeof(temp_CodingType), "MMR");
                break;

            case 3:
                snprintf(temp_CodingType, sizeof(temp_CodingType), "JBIG");
                break;

            default:
                snprintf(temp_CodingType, sizeof(temp_CodingType), "-");
                break;
        }
//		snprintf (OneLineText, sizeof(OneLineText), "CODING         : %s", temp_CodingType);
//		memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
        snprintf (ResultBuf->Coding, sizeof(ResultBuf->Coding), "%s", temp_CodingType);

        // Step 6. T.30 Result
        switch ( gFaxT30Result )
        {
            case 0:
                snprintf(temp_Result, sizeof(temp_Result), "OK");
                break;

            case 2:
                snprintf(temp_Result, sizeof(temp_Result), "No Answer");
                break;

            case 3:
                snprintf(temp_Result, sizeof(temp_Result), "Line Busy");
                break;

            case 4:
                snprintf(temp_Result, sizeof(temp_Result), "Line Error");
                break;

            case 5:
                snprintf(temp_Result, sizeof(temp_Result), "Phone Abort");
                break;

            case 11:
                snprintf(temp_Result, sizeof(temp_Result), "Memory Full");
                break;

            case 14:
                snprintf(temp_Result, sizeof(temp_Result), "Cancelled");
                break;

            case 68:
                snprintf(temp_Result, sizeof(temp_Result), "Document Jam");
                break;

            default:
                snprintf(temp_Result, sizeof(temp_Result), "ERROR-#%03d", gFaxT30Result);
                break;
        }
//		snprintf (OneLineText, sizeof(OneLineText), "RESULT         : %s", temp_Result);
//		memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));
        snprintf (ResultBuf->Result, sizeof(ResultBuf->Result), "%s", temp_Result);
    }

    // Blank Line

    // Page Number
    //snprintf(OneLineText,sizeof(OneLineText), "                     PAGE %d of %d", pageNum, targetPageNum);

    //memcpy(&DestBuf[(rowCount++)*128],OneLineText,strlen(OneLineText));

    return rowCount;
}


void fsu_stop_homologation_signal (void)
{
    StopHomologationSignal();
}


void fsu_start_homologation_signal (UINT8 Signal)
{
    StartHomologationSignal((MODEM_TX_TEST_INDEX)Signal);
}


void fsu_set_answer_mode (UINT8 AnswerMode)
{
    UINT8 DetectRing = 0;

    if (AnswerMode == 0) // Auto Answer
    {
        DetectRing = AUTO_ANSWER;

        Fax_API_T30Attribute(SET, FXA_ANSWERMODE, &DetectRing);
    }
    else // Manual Answer
    {
        DetectRing = MANUAL_ANSWER;

        Fax_API_T30Attribute(SET, FXA_ANSWERMODE, &DetectRing);
    }
}

