#include "fax_memAPI.h"
#include "SDHFaxCommonHeader.h"
#include <stdio.h>
#include <assert.h>
#include "string.h"
#include "tx_api.h"
#include <sys/types.h>
#include <unistd.h>
#include "mosi.h"
#include "dc_type.h"
#include "dc_return_codes.h"
#include "dc_fwcxdxx_lnx_api.h"
#include "T30X.H"
#include "fshs.h"
#include "fsu_call.h"
#include "fsu_api.h"
#include "fsu.h"
#include <sys/time.h>
#include <time.h>
#include "../../faxapp/include/faxapp_notify.h"
#include "../../faxapp/include/faxapp_job.h"
#include "paper_size_api.h"
#include "lassert.h"
#include "ERROR.H"
#if __HAVE_FAXSOLUTION_DK__
#include "faxapp_solution.h"
#endif

#define REF_ALLOC
#include "ref_fipp.h"
#undef REF_ALLOC


#define FAX_SCAN_SOURCE_FB     (0)
#define FAX_SCAN_SOURCE_ADF     (1)
#define FAX_SCAN_SOURCE_NONE	(2)


TX_EVENT_FLAGS_GROUP fipp_scan_events_flags;
TX_EVENT_FLAGS_GROUP fipp_tx_events_flags;
TX_EVENT_FLAGS_GROUP fipp_rx_events_flags;
TX_EVENT_FLAGS_GROUP fipp_prn_events_flags;

int fsu_total_page_print_complete_check(void);
void fipp_scan_to_mem(void);
void fipp_mem_to_send(void);
void fipp_rcv_to_mem(void);
void fipp_mem_to_prn(void);
void activate_scan_to_mem(void);
void activate_mem_to_send(void);
void activate_rcv_to_mem(void);
void activate_mem_to_prn(void);
void ref_app_fipp_initialize (void);
void fsu_set_scan_busy_flag(UINT8 newValue);
long faxapp_get_delay_time_in_sec(UINT8 TimeToTxHour, UINT8 TimeToTxMin);
DC_RETURN_CODE fipp_init_job(void);
extern void job_t4codec_start(void);
extern void fsu_check_sdcard_enable(void);
extern void fsu_make_doc_man_dir_in_ram (void);
extern void fsu_make_doc_man_dir_in_nand (void);
extern void fsu_update_total_pages (UINT16 jnum, UINT8 Page);
extern void fsu_decrease_total_pages (UINT16 jnum);
extern void fsu_update_tx_total_page_no (UINT16 jnum, UINT8 TxTotalPageNo);
extern void fsu_make_image_file_for_broadcast (UINT16 SrcJobNo, UINT16 DstJobNo, UINT8 Page);
extern void fsu_notify_no_mem_error_ram (UINT16 Error);
extern void fsu_make_sol_doc_man_dir_in_nand (void);
extern void faxapp_delete_all_records_recd_faximagebackup (void);
extern void faxapp_delete_all_records_recd_faxbroadcastresultreport (void);
extern int sw_jbig_decode_tx (int Page, UINT16 jnum, char *FileName, UINT8 Directioin);
extern int sw_jbig_encode_tx (int Page, char *FileName, UINT16 jnum, UINT8 Direction);
extern void fsu_prepare_send_data (int PageIndex);
extern void set_tx_job_ready(UINT16 jnum);
extern void fsu_remove_tx_job(UINT16 jnum);
extern void fsu_reschedule_tx_job(UINT16 jnum);
extern void faxapp_fax_print_start_job(void);
extern void fsu_set_prn_job_done(UINT16 jnum);
extern void faxapp_fax_scan_start_job( void );
extern void fsu_update_resolution (UINT16 jnum, UINT8 res);
extern void faxapp_request_update_user_id (void);
extern UINT8 faxapp_get_job_type(void);
extern UINT8 fsu_get_status (UINT16 jnum);
extern UINT8 fsu_get_print_reason (UINT16 jnum);
extern UINT8 fsu_get_broadcast (UINT16 jnum);
extern UINT8 fsu_requset_get_current_page_no_from_Job_Queue_File (UINT16 jnum);
extern int T4_encoding(int Page, char *FileName, int codingtype, int faxRes, int job_num_send, UINT8 Direction, int photo_image_resolution);
extern int T4_Decoding(UINT16 jnum, char *FileName, char *out_FileName, int codingtype, int width_type, int rx_resolution, UINT8 Direction);
extern int is_adf_paper_present(void);
extern int fsu_get_memory_full_status_nand(void);
extern UINT8 fsu_get_total_pages (UINT16 jnum);
extern UINT8 fsu_get_width (UINT16 jnum);
extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT8 fsu_get_fax_forward (UINT16 jnum);
extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern void fsu_fax_print_job_done(void);
extern void faxapp_process_tx_rx_result_report(void);
extern void fsu_fax_print_job_clear(void);
extern int fsu_image_validation_check (int res, int jnum);
extern UINT8 fsu_get_isDuplex (UINT16 jnum);
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern void faxapp_scan_solution(int scanResult);
extern void fsu_update_send_permission (UINT16 jnum, UINT8 permission);
extern void Update_Job_Queue_File(IMAGE_BACKUP_REASON_t Reason, UINT16 jnum, UINT8 Param);
extern void fsu_update_unique_job_id (UINT16 jnum, UINT32 jobId);
extern UINT32 fsu_get_unique_job_id(UINT16 jnum);
#endif
#ifdef HAVE_IMAGELOG
extern int ARIA_encode_path(char *in_file, char *out_file, int crypt_flag, int file_option);
#endif
extern void fsu_delete_file_on_file_system (const char *Path);
extern int faxapp_get_txtFile_fax_solution(void);
extern int faxapp_get_endFile_fax_solution(void);


#if __HAVE_FAXSOLUTION_DK__
extern void fsu_purge_doc_dir(UINT16 jnum);
extern void fsu_make_dir_on_file_system (const char *Path);
extern void faxapp_make_tif_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType);
extern void faxapp_make_tif_start_dk (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType);
extern void fsu_request_make_dot_txt_file (const char *pPath, UINT16 jnum);
extern void fsu_request_make_dot_end_file (const char *pPath, UINT16 jnum, UINT8 job_type);
#endif

BOOLEAN isReadyToFaxPrint = FALSE;
UINT8 RTNCount = 0;
UINT8 gFaxCurrentSendPageNo = 0;
UINT8 gFaxScanSource = FAX_SCAN_SOURCE_NONE;
UINT8 FSUCurrentFBScanPageNo = 1;
UINT8 gFaxSendCurrentPageOnHook = 0;
UINT8 isTxImageProcessDone = 0;
UINT8 gFaxCurrentTTIPageNo = 0;
UINT8 gFaxOnHookScanSource = FAX_SCAN_SOURCE_NONE;
UINT8 isFSUTxImageProgessBegin = 1;
UINT8 isFSURxImageProgessBegin = 1;
UINT8 gFaxRebootCnt = 0;
UINT8 *gpBufferForTxData = 0;
#if __SUPPORT_MANUAL_RECEIVE__
int loopExitCnt = 0;
char OnHookPressed[3][3] = { {'O', }, // On-Hook Key
                             {'O', 'C', }, // On-Hook Key + Cancel Key
                             {'O', 'S', } }; // On-Hook Key + Start Key
#else
#endif
UINT32 gnSizeForTxDataBuffer = 0;
static int TotalPageNoToPrint = 0;
int is1stPageDataTransfered = 0;
int gFaxPrintCurrentPage = 0;
int FSUCurrentScanPageNo = 1;
int JobStartPageNo = 0;
int gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_NONE;
UINT8 isFaxDocInit = 0;
UINT8 isSDCardEnable = 0;
BOOLEAN isFBScanWait = FALSE;
int gFaxSendCurrentPage = 0;
UINT8 isDKSolution = 0;

//taeyoukim add.
extern int user_input_print_cancel;
extern int duplex_print_page_count;
extern int planetype;
extern int fax_scan_page_count;
extern int second_page_on_sheet;
extern BOOLEAN isScanComplete;
extern BOOLEAN prn_cancel_flag;
#if __SUPPORT_MANUAL_RECEIVE__
extern UINT8 ExtPhoneDialBuffer[LEN_PHONE_NO + 1];
extern UINT8 ExtPhoneDialIndex;
extern UINT8 MachineKeyBufferIndex;
extern UINT8 isLineInUse;
#else
#endif
extern int del_job_pending;
extern int isOnHookScanCanceledByCommError;
extern UINT8 offHookDone;

extern void PBM_Rescaling (char *PBMFileName, uint8_t faxRes, uint16_t jnum, uint8_t image_width_type);
extern void faxapp_page_end( void );
extern void clearFaxScanPageCount(void);
extern void fsu_set_mem_time(UINT16 jnum);
extern UINT32 faxapp_get_originalSizeCode(void);
extern int getDuplexPrintOn(void);
//


extern BOOLEAN f_FaxScan_using_ADF;
extern int gFaxRxCurrentPage;
extern int isFSUReady;
extern int isFaxPrintBusy;
extern UINT8 FBMultiPageScan;
extern UINT8 fsu_current_funcs;
extern UINT16 fsu_job_num_scan;
extern UINT16 fsu_job_num_send;
extern UINT16 fsu_job_num_prn;
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];
extern int MakePBMToFaxImage(int PageIndex);
static int isFaxScanBusy = 0;
void ref_fipp_tx_data_to_modem(void);
extern void job_modem_init(UINT8 direction, BOOLEAN m_ecm, UINT8 coding, UINT16 frame_size);
extern void fsu_update_tx_time (UINT16 jnum, long TimeToTx);
extern void fsu_retrieve_job_queue(int ValidJobCnt);
extern void Create_Job_Queue_File(UINT16 jnum, FAX_APP_JOB_TYPE_t FaxAppJobType, UINT16 DocId);
#if __SUPPORT_MANUAL_RECEIVE__
extern void Fax_API_ManReceiveFax(void);
extern char RemoteAccessCode[LEN_PHONE_NO + 1];
extern char MachineKeyBuffer[MAX_LEN_MACHINE_ACCESS_CODE + 1];
#else
#endif
extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern char *pRootFolderNameSolDocMan;
extern char *pRootFolderARIA;
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
extern UINT8 fsu_get_photo_mode (UINT16 jnum);
extern int getgFaxPrintMode(void);

#if __MEASURE_PERFORMANCE__
extern void faxapp_get_job_end_time(int JobType);
extern char *faxapp_get_elapsed_time(int JobType);
#endif // #if __MEASURE_PERFORMANCE__

extern int ValidJobCnt;

#if __ONHOOK_TX_WAITING__
BOOLEAN fOnHookScanComplete = FALSE;
#endif /* __ONHOOK_TX_WAITING__ */

/***********************************************************************
 *  Function: fipp_init_job
 *
 *  Description:
 *
 *  Inputs:
 *    none
 *
 ***********************************************************************/
DC_RETURN_CODE fipp_init_job(void)
{
    DC_RETURN_CODE r_code = DC_FAILURE;

    r_code = tx_event_flags_create(&fipp_scan_events_flags, "fipp_scan");
    if (r_code != DC_SUCCESS)
    {
        printf("\n(YHCHO) %s() - tx_event_flags_create() fipp_scan Error!\n", __FUNCTION__);
        return r_code;
    }

    r_code = tx_event_flags_create(&fipp_tx_events_flags, "fipp_tx");
    if (r_code != DC_SUCCESS)
    {
        printf("\n(YHCHO) %s() - tx_event_flags_create() fipp_tx Error!\n", __FUNCTION__);
        return r_code;
    }

    r_code = tx_event_flags_create(&fipp_rx_events_flags, "fipp_rx");
    if (r_code != DC_SUCCESS)
    {
        printf("\n(YHCHO) %s() - tx_event_flags_create() fipp_rx Error!\n", __FUNCTION__);
        return r_code;
    }

    r_code = tx_event_flags_create(&fipp_prn_events_flags, "fipp_prn");
    if (r_code != DC_SUCCESS)
    {
        printf("\n(YHCHO) %s() - tx_event_flags_create() fipp_prn Error!\n", __FUNCTION__);
        return r_code;
    }

    return r_code;
}


void activate_scan_to_mem(void)
{
	tx_event_flags_set(&fipp_scan_events_flags, FLAG_SCAN_START, TX_OR);
}


void de_activate_scan_to_mem (void)
{
    tx_event_flags_set(&fipp_scan_events_flags, FLAG_SCAN_COMPLETE, TX_OR);
}


void activate_mem_to_send(void)
{
 	tx_event_flags_set(&fipp_tx_events_flags, FLAG_TX_START, TX_OR);
}


void activate_rcv_to_mem(void)
{
 	tx_event_flags_set(&fipp_rx_events_flags, FLAG_RX_START, TX_OR);
}


void de_activate_rcv_to_mem(void)
{
    tx_event_flags_set(&fipp_rx_events_flags, FLAG_RX_COMPLETE, TX_OR);
}

void abort_rcv_to_mem(void)
{
    tx_event_flags_set(&fipp_rx_events_flags, FLAG_RX_ABORT, TX_OR);
}

void error_rcv_to_mem(void)
{
    tx_event_flags_set(&fipp_rx_events_flags, FLAG_RX_ERROR, TX_OR);
}

void abort_mem_to_prn(void)
{
 	tx_event_flags_set(&fipp_prn_events_flags, FLAG_PRINT_ABORT, TX_OR);
}


void activate_mem_to_prn(void)
{
 	tx_event_flags_set(&fipp_prn_events_flags, FLAG_PRINT_START, TX_OR);
}


void de_activate_mem_to_prn (void)
{
    tx_event_flags_set(&fipp_prn_events_flags, FLAG_PRINT_COMPLETE, TX_OR);
}


void abort_mem_to_send(void)
{
    tx_event_flags_set(&fipp_tx_events_flags, FLAG_TX_ABORT, TX_OR);
}


void error_mem_to_send(void)
{
    tx_event_flags_set(&fipp_tx_events_flags, FLAG_TX_ERROR, TX_OR);
}


/***********************************************************************
 *  Function: fipp_scan_to_mem
 *
 *  Description:
 *    handle data pipeline for scan to mem 
 *
 *  Inputs:
 *    none
 *
 ***********************************************************************/
void fipp_scan_to_mem(void)
{
    ULONG eventReceived = 0;
    ULONG req_flags = 0;
    UINT8 TotalRecipient = 0;
    UINT8 IndexToRecipient = 0;
//#if __HAVE_FAXSOLUTION_DK__
//#else
    UINT8 JobType = 0;
//#endif
    UINT16 OriginalDocId = 0;
    UINT16 faxScanJobNum = 0;

    req_flags = FLAG_SCAN_START | FLAG_SCAN_COMPLETE | FLAG_ERORR_DETECT | FLAG_SCAN_ABORT | FLAG_SCAN_ABORT_DONE;

    tx_event_flags_get (&fipp_scan_events_flags, req_flags, TX_OR_CLEAR, &eventReceived, OSPL_NO_WAIT);

    if (eventReceived & FLAG_SCAN_ABORT)
    {
        printf("\n(YHCHO) %s() - FLAG_SCAN_ABORT -> START\n", __FUNCTION__);

        printf("\n(YHCHO) %s() - FLAG_SCAN_ABORT -> END\n", __FUNCTION__);

    }
    else if(eventReceived & FLAG_ERORR_DETECT)
    {
        printf("\n(YHCHO) %s() - FLAG_ERORR_DETECT -> START\n", __FUNCTION__);

        printf("\n(YHCHO) %s() - FLAG_ERORR_DETECT -> END\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_SCAN_ABORT_DONE)
    {
        printf("\n(YHCHO) %s() - FLAG_SCAN_ABORT_DONE -> START\n", __FUNCTION__);

        printf("\n(YHCHO) %s() - FLAG_SCAN_ABORT_DONE -> END\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_SCAN_START)
    {
        printf("\n(YHCHO) %s() - FLAG_SCAN_START -> START\n", __FUNCTION__);

        fsu_current_funcs |= FAX_SCAN_TO_MEM;

        fsu_set_scan_busy_flag(1);

		if(fsu_get_broadcast(fsu_job_num_scan) == 1)
		{
            OriginalDocId = fsu_get_original_doc_id(fsu_job_num_scan);

			faxScanJobNum = gFaxBroadcastJobInfo[OriginalDocId][0];
		}
		else
		{
			faxScanJobNum = fsu_job_num_scan;
		}
#if __ONHOOK_TX_WAITING__
		if( fsu_get_flag_manual_tx(faxScanJobNum) == 1 )
		{
			fOnHookScanComplete = FALSE;
		}
#endif /* __ONHOOK_TX_WAITING__ */
		if(gFaxScanSource == FAX_SCAN_SOURCE_FB)
		{
			; // do nothing
		}
		else if(gFaxScanSource == FAX_SCAN_SOURCE_NONE)
        {
			clearFaxScanPageCount(); // set fax scan completed page count 0.
            gFaxOnHookScanSource = gFaxScanSource = is_adf_paper_present();
		}
		else
		{
			faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_INVALID_SCAN_SOURCE); //FAX_SCAN_SOURCE_ADF
		}
		
		printf("\n(YHCHO) %s() - gFaxScanSource: %d\n", __FUNCTION__, gFaxScanSource);

        if (gFaxScanSource == FAX_SCAN_SOURCE_ADF)
        {
			if(faxapp_get_originalSizeCode() != MEDIASIZE_A3)
            {
				faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_FROM_ADF, FSUCurrentScanPageNo, 0, 0, 0, faxScanJobNum);
			}
			else
			{
				faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_FROM_FB, FSUCurrentFBScanPageNo, 0, 0, 0, faxScanJobNum);
			}
        }
        else
        {
            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_FROM_FB, FSUCurrentFBScanPageNo, 0, 0, 0, faxScanJobNum);
        }

        faxapp_fax_scan_start_job();

        printf("\n(YHCHO) %s() - FLAG_SCAN_START -> END\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_SCAN_COMPLETE)
    {
        printf("\n(YHCHO) %s() - FLAG_SCAN_COMPLETE -> START", __FUNCTION__);

		if(fsu_get_broadcast(fsu_job_num_scan) == 1)
		{
            OriginalDocId = fsu_get_original_doc_id(fsu_job_num_scan);

			faxScanJobNum = gFaxBroadcastJobInfo[OriginalDocId][0];
		}
		else
		{
			faxScanJobNum = fsu_job_num_scan;
		}

		if(isScanComplete == FALSE)
		{
            printf("\n(YHCHO) %s() - gFaxAbnormalFlag: %d", __FUNCTION__, gFaxAbnormalFlag);
            printf("\n(YHCHO) %s() - isOnHookScanCanceledByCommError: %d\n", __FUNCTION__, isOnHookScanCanceledByCommError);

            fsu_set_scan_busy_flag(0);

			if(JobType == FAX_APP_JOB_TYPE_FAX_SOLUTION)
			{
				faxapp_scan_solution(0);
			}

            if (gFaxAbnormalFlag == FAX_ABNORMAL_FLAG_SCAN_CANCEL || isOnHookScanCanceledByCommError == 1)
            {
				if(isOnHookScanCanceledByCommError == 1)
				{
					faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCAN_FAIL, 0, 0, 0, 0, faxScanJobNum);
				}
				else
				{
					faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, 0, 0, 0, faxScanJobNum);					
				}
            }
            else
            {
                gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_SCAN_JAM;

                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCAN_FAIL, 0, 0, 0, 0, faxScanJobNum);
            }

			clearFaxScanPageCount(); // set fax scan completed page count 0.

			gFaxScanSource = FAX_SCAN_SOURCE_NONE;
			FSUCurrentFBScanPageNo = 1;

            if ( (isOnHookScanCanceledByCommError != 1) || (gFaxAbnormalFlag == FAX_ABNORMAL_FLAG_SCAN_JAM) )
            {
                Fax_API_CancelFax(TRUE, faxScanJobNum);
            }

			isOnHookScanCanceledByCommError = 0;
			
			if(fsu_get_flag_manual_tx(faxScanJobNum) != 1)
			{
				gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_NONE;
			}
#if __ONHOOK_TX_WAITING__
			if( fsu_get_flag_manual_tx(faxScanJobNum) == 1 )
			{
				fOnHookScanComplete = TRUE;
			}
#endif /* __ONHOOK_TX_WAITING__ */
			return;
		}

		fsu_set_scan_busy_flag(0);
		FSUCurrentFBScanPageNo = 1;

#if __HAVE_FAXSOLUTION_DK__
#endif
		if(isDKSolution)
		{
			char targetPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
			snprintf(targetPathName, sizeof(targetPathName), "%s%cImgDoc%04X", pRootFolderNameSolDocMan, PATH_SEPARATOR, faxScanJobNum);
			printf("\n(YHCHO) %s() - targetPathName: %s\n", __FUNCTION__, targetPathName);
			fsu_make_dir_on_file_system(targetPathName);

			faxapp_make_tif_start_dk(targetPathName, faxScanJobNum, 1, fax_scan_page_count, fsu_get_width(faxScanJobNum), SAVE_FILE_PAGE_TYPE_SINGLE_PAGE);
			printf("\n(YHCHO) %s() - Tiff Image Make Complete !!\n", __FUNCTION__);

			if (fsu_get_broadcast (faxScanJobNum) == 0)
			{
				if(faxapp_get_txtFile_fax_solution())
					fsu_request_make_dot_txt_file(targetPathName, faxScanJobNum);
				if(faxapp_get_endFile_fax_solution())
					fsu_request_make_dot_end_file(targetPathName, faxScanJobNum, SCAN_TO_MEM);
			}
			else
			{
				if(faxapp_get_txtFile_fax_solution())
					fsu_request_make_dot_txt_file(targetPathName, gFaxBroadcastJobInfo[fsu_get_original_doc_id(faxScanJobNum)][0]);
				if(faxapp_get_endFile_fax_solution())
					fsu_request_make_dot_end_file(targetPathName, gFaxBroadcastJobInfo[fsu_get_original_doc_id(faxScanJobNum)][0], SCAN_TO_MEM);
			}

			//faxapp_scan_solution();
			faxapp_solution_send_to_ftp_dk(targetPathName, faxScanJobNum);

			faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_COMPLETE, 0, fax_scan_page_count, 0, 0, faxScanJobNum);

			//faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SENDING_COMPLETE, 0, fax_scan_page_count, 0, 0, faxScanJobNum);

			if (fsu_get_broadcast(faxScanJobNum) == 1)
			{
				TotalRecipient = gFax_BC_Num[OriginalDocId].BC_Total_Num;
				printf("\n(YHCHO) %s() - TotalRecipient: %d", __FUNCTION__, TotalRecipient);
				for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
				{
					fsu_purge_doc_dir(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
					fsu_remove_tx_job(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
				}
			}
			else
			{
				fsu_purge_doc_dir(faxScanJobNum);
			}
		}
		else
		{
			//set mem_time
			if (fsu_get_flag_manual_tx(faxScanJobNum) == 0) // On-Hook job should not be printed on memory file list..
			{
				TotalRecipient = faxapp_get_no_of_recipient();

				if ( (TotalRecipient == 0) || (TotalRecipient == 1) )
				{
					fsu_set_mem_time(faxScanJobNum);
				}
				else if ( (TotalRecipient >= 2) && (TotalRecipient < MAX_NUM_BC + 1) ) // Range: 2 ~ 200
				{
					OriginalDocId = fsu_get_original_doc_id(faxScanJobNum);

					for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
					{
						fsu_set_mem_time(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
					}
				}
			}

			if (fsu_get_memory_full_status_nand() == 1)
			{
				gFaxScanSource = FAX_SCAN_SOURCE_NONE;

				// Something should be added..

				return;
			}
			
			TotalRecipient = faxapp_get_no_of_recipient();

			JobType = faxapp_get_job_type();

			if ( (TotalRecipient == 0) || (TotalRecipient == 1) )
			{
				fsu_update_total_pages(faxScanJobNum, fax_scan_page_count);

				fsu_update_tx_total_page_no(faxScanJobNum, fax_scan_page_count);

#if __SUPPORT_MANUAL_TRANSMIT__
				if ( (JobType != FAX_APP_JOB_TYPE_ON_HOOK) && (JobType != FAX_APP_JOB_TYPE_MANUAL_TRANSMIT) )
#else
				if (JobType != FAX_APP_JOB_TYPE_ON_HOOK)
#endif
				{
					Create_Job_Queue_File(faxScanJobNum, JobType, fsu_get_doc_id(faxScanJobNum));

					set_tx_job_ready(faxScanJobNum);
				}
			}
			else if ( (TotalRecipient >= 2) && (TotalRecipient < MAX_NUM_BC + 1) ) // Range: 2 ~ 200
			{
				for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
				{
					if (IndexToRecipient != 0)
					{
						fsu_make_image_file_for_broadcast(gFaxBroadcastJobInfo[OriginalDocId][0], gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], fax_scan_page_count);
					}

					fsu_update_total_pages(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], fax_scan_page_count);

					fsu_update_tx_total_page_no(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], fax_scan_page_count);

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
					fsu_update_unique_job_id(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient],
												fsu_get_unique_job_id(gFaxBroadcastJobInfo[OriginalDocId][0]));
#endif

					JobType = FAX_APP_JOB_TYPE_BROADCAST;

					Create_Job_Queue_File(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], JobType, fsu_get_doc_id(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]));

					if (IndexToRecipient != 0)
					{
						faxapp_notify_SendFaxBroadcastJobID(gFaxBroadcastJobInfo[OriginalDocId][0],IndexToRecipient, gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
					}
				}

				for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
				{
					set_tx_job_ready(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
				}
			}

#if __ONHOOK_TX_WAITING__
			if( fsu_get_flag_manual_tx(faxScanJobNum) == 1 )
			{
				fOnHookScanComplete = TRUE;
			}
#endif /* __ONHOOK_TX_WAITING__ */
#if __MEASURE_PERFORMANCE__
			faxapp_get_job_end_time(SCAN_TO_MEM);

			printf("\n(YHCHO) %s() - Elapsed Time <SCAN TO MEM>: %s\n", __FUNCTION__, faxapp_get_elapsed_time(SCAN_TO_MEM));
#endif // #if __MEASURE_PERFORMANCE__

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
			if(JobType == FAX_APP_JOB_TYPE_FAX_SOLUTION)
			{
				faxapp_scan_solution(1);
				if ( (TotalRecipient == 0) || (TotalRecipient == 1) )
				{
					Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_PERMISSION, faxScanJobNum, SOLUTION_PERMISSION_STATE_WAIT);
					fsu_update_send_permission (faxScanJobNum, SOLUTION_PERMISSION_STATE_WAIT);
				}
				else
				{
					for (IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
					{
						Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_PERMISSION, gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], SOLUTION_PERMISSION_STATE_WAIT);
						fsu_update_send_permission(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient], SOLUTION_PERMISSION_STATE_WAIT);
					}
				}
			}
#endif
			faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_SCANNING_COMPLETE, 0, 0, 0, 0, faxScanJobNum);
		}

        gFaxScanSource = FAX_SCAN_SOURCE_NONE;

        fsu_current_funcs &= ~FAX_SCAN_TO_MEM;

        printf("\n(YHCHO) %s() - FLAG_SCAN_COMPLETE -> END\n", __FUNCTION__);
    }
}


/***********************************************************************
 *  Function: fipp_mem_to_send
 *
 *  Description:
 *    handle data pipeline for mem to send
 *
 *  Inputs:
 *    none
 *
 ***********************************************************************/
void fipp_mem_to_send(void)
{
    ULONG eventReceived = 0;
    ULONG req_flags = 0;

    req_flags = FLAG_TX_START | FLAG_TX_COMPLETE | FLAG_ERORR_DETECT | FLAG_TX_ABORT | FLAG_TX_ERROR;

    tx_event_flags_get (&fipp_tx_events_flags, req_flags, TX_OR_CLEAR, &eventReceived, TX_NO_WAIT);

    if(eventReceived & FLAG_TX_ABORT)
    {
        printf("\n(YHCHO) %s() - FLAG_TX_ABORT -> START\n", __FUNCTION__);

        if(fsu_current_funcs & FAX_MEM_TO_SEND_BEGIN)
		{
            fsu_current_funcs &= ~FAX_MEM_TO_SEND_BEGIN;
		}
		else
		{
			fsu_current_funcs &= ~FAX_MEM_TO_SEND;
		}
		
        printf("\n(YHCHO) %s() - FLAG_TX_ABORT -> END\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_TX_ERROR)
    {
        printf("\n(YHCHO) %s() - FLAG_TX_ERROR -> START\n", __FUNCTION__);

        if(fsu_current_funcs & FAX_MEM_TO_SEND_BEGIN)
		{
            fsu_current_funcs &= ~FAX_MEM_TO_SEND_BEGIN;
		}
		else
		{
			fsu_current_funcs &= ~FAX_MEM_TO_SEND;
		}

        fsu_reschedule_tx_job(fsu_job_num_send);

        printf("\n(YHCHO) %s() - FLAG_TX_ERROR -> END\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_TX_START)
    {
        printf("\n(YHCHO) %s() - FLAG_TX_START -> START\n", __FUNCTION__);

        fsu_current_funcs &= ~FAX_MEM_TO_SEND_BEGIN;
        fsu_current_funcs |= FAX_MEM_TO_SEND;

        if (RTNCount == 0)
        {
            gFaxCurrentSendPageNo++;
        }

        printf("\n(YHCHO) %s() - gFaxCurrentSendPageNo: %d\n", __FUNCTION__, gFaxCurrentSendPageNo);

        if (gFaxCurrentSendPageNo >= 2)
        {
            if (fsu_get_memory_full_status_ram() == 1)
            {
                printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);

                fsu_notify_no_mem_error_ram(FAX_ERROR_NO_MEM_ERROR_RAM);

                return;
            }
        }

        ref_fipp_tx_data_to_modem();

        isTxImageProcessDone = 0;

        printf("\n(YHCHO) %s() - FLAG_TX_START -> END\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_TX_COMPLETE)
    {
        printf("\n(YHCHO) %s() - FLAG_TX_COMPLETE -> START\n", __FUNCTION__);

        printf("\n(YHCHO) %s() - FLAG_TX_COMPLETE -> END\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_ERORR_DETECT)
    {
        printf("\n(YHCHO) %s() - FLAG_ERORR_DETECT -> START\n", __FUNCTION__);

        printf("\n(YHCHO) %s() - FLAG_ERORR_DETECT -> END\n", __FUNCTION__);
    }
    else
    {
        ;
    }
}


/***********************************************************************
 *  Function: fipp_rcv_to_mem
 *
 *  Description:
 *    handle data pipeline for rcv to mem
 *
 *  Inputs:
 *    none
 *
 ***********************************************************************/
void fipp_rcv_to_mem(void)
{    
    ULONG eventReceived = 0, req_flags = 0;

    req_flags = FLAG_RX_START | FLAG_RX_COMPLETE | FLAG_ERORR_DETECT | FLAG_RX_ABORT | FLAG_RX_ERROR ;

    tx_event_flags_get (&fipp_rx_events_flags, req_flags, TX_OR_CLEAR, &eventReceived, OSPL_NO_WAIT);

    if(eventReceived & FLAG_RX_ABORT || eventReceived & FLAG_RX_ERROR)
    {
        if(fsu_current_funcs & FAX_RECEIVE_TO_MEM_BEGIN)
		{
            fsu_current_funcs &= ~FAX_RECEIVE_TO_MEM_BEGIN;
		}
		else
		{
			fsu_current_funcs &= ~FAX_RECEIVE_TO_MEM;
		}
    }
    else if(eventReceived & FLAG_RX_START)
    {
        printf("\n(YHCHO) %s() - FLAG_RX_START - START\n", __FUNCTION__);

        fsu_current_funcs &= ~FAX_RECEIVE_TO_MEM_BEGIN;
        fsu_current_funcs |= FAX_RECEIVE_TO_MEM;

        printf("\n(YHCHO) %s() - FLAG_RX_START - END\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_RX_COMPLETE)
    {
        printf("\n(YHCHO) %s() - FLAG_RX_COMPLETE - START\n", __FUNCTION__);

        fsu_current_funcs &= ~FAX_RECEIVE_TO_MEM;

        printf("\n(YHCHO) %s() - FLAG_RX_COMPLETE - END\n", __FUNCTION__);
    }
    else
    {
        ;
    }
}


/***********************************************************************
 *  Function: fipp_mem_to_prn
 *
 *  Description:    
 *    handle data pipeline for mem to prn
 *
 *  Inputs:
 *    none
 *
 ***********************************************************************/
void fipp_mem_to_prn(void)
{
    ULONG eventReceived = 0;
    ULONG req_flags = 0;
    int TotalPages = 0;
    int ComparePages = 0;

    
    req_flags = FLAG_PRINT_START | FLAG_PRINT_COMPLETE | FLAG_PRINT_ABORT;

    tx_event_flags_get (&fipp_prn_events_flags, req_flags, TX_OR_CLEAR, &eventReceived, TX_NO_WAIT);

    if(eventReceived & FLAG_PRINT_ABORT)
    {
        printf("\n(YHCHO) %s() - FLAG_PRINT_ABORT -> START\n", __FUNCTION__);

        isFaxPrintBusy = 0;
        isReadyToFaxPrint = FALSE;
        gFaxPrintCurrentPage = 0;
        duplex_print_page_count = 0;

        printf("\n(YHCHO) %s() - FLAG_PRINT_ABORT -> START\n", __FUNCTION__);
    }
    else if(eventReceived & FLAG_PRINT_START)
    {
		if(getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_EVERY_PAGE)
		{
            if (fsu_get_status(fsu_job_num_prn) == FSU_STATE_RECEIVE_DONE)
            {
				ComparePages = fsu_get_total_pages(fsu_job_num_prn);
            }
			else
            {
                if (fsu_get_print_reason(fsu_job_num_prn) == PRINT_REASON_RECEIVE)
                {
                    ComparePages = gFaxRxCurrentPage;
                }
                else
                {
                    if (is1stPageDataTransfered == 0)
                    {
                        TotalPageNoToPrint = fsu_get_total_pages(fsu_job_num_prn) + fsu_get_start_page_no_in_mem(fsu_get_doc_id(fsu_job_num_prn), FAX_FILE_TYPE_JBG) - 1;

                        ComparePages = TotalPageNoToPrint;
                    }
                    else
                    {
                        ComparePages = TotalPageNoToPrint;
                    }
                }
            }
		}

        if((getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_EVERY_PAGE) && !(gFaxPrintCurrentPage < ComparePages))
        {
            de_activate_mem_to_prn();
		}
		else
		{
			printf("\n(YHCHO) %s() - FLAG_PRINT_START -> START\n", __FUNCTION__);	
			
			fsu_current_funcs |= FAX_MEM_TO_PRINT;
			
			faxapp_fax_print_start_job();
			
			printf("\n(YHCHO) %s() - FLAG_PRINT_START -> END\n", __FUNCTION__);
        }
    }
    else if(eventReceived & FLAG_PRINT_COMPLETE)
    {
        if (getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_ALL)
        {
            printf("\n(YHCHO) %s() - FLAG_PRINT_COMPLETE -> txq.job[%d].TxRxTotalPages: %d\n", __FUNCTION__, fsu_job_num_prn, txq.job[fsu_job_num_prn].TxRxTotalPages);

            if (fsu_total_page_print_complete_check() || user_input_print_cancel)
            {
#if __MEASURE_PERFORMANCE__
				faxapp_get_job_end_time(MEM_TO_PRINT);

				printf("\n(YHCHO) %s() - Elapsed Time <MEM TO PRINT>: %s\n", __FUNCTION__, faxapp_get_elapsed_time(MEM_TO_PRINT));
#endif // #if __MEASURE_PERFORMANCE__

				fsu_current_funcs &= ~FAX_MEM_TO_PRINT;
			
				isFaxPrintBusy = 0;
				isReadyToFaxPrint = FALSE;
				gFaxPrintCurrentPage = 0;
				duplex_print_page_count = 0;
				user_input_print_cancel = 0;
				
				if (fsu_get_print_reason(fsu_job_num_prn) == PRINT_REASON_RECEIVE)
				{
					faxapp_process_tx_rx_result_report();
				}
						
				if (fsu_get_print_reason(fsu_job_num_prn) == PRINT_REASON_RESET)
				{
					is1stPageDataTransfered = 0;
				}
            }
            else
            {
				faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_INVALID_RX_TOTAL_PAGE_1);
            }
        }
        else
        {
            if (fsu_get_status(fsu_job_num_prn) == FSU_STATE_RECEIVE_DONE)
            {
				printf("\n(YHCHO) %s() - FLAG_PRINT_COMPLETE : FSU_STATE_RECEIVE_DONE\n", __FUNCTION__);
				
				TotalPages = fsu_get_total_pages(fsu_job_num_prn);

				if(gFaxPrintCurrentPage == TotalPages || user_input_print_cancel)
				{
					printf("\n(YHCHO) %s() -  gFaxPrintCurrentPage : %d\n", __FUNCTION__,gFaxPrintCurrentPage);
					
					if(fsu_get_isDuplex(fsu_job_num_prn) && second_page_on_sheet && !user_input_print_cancel)
					{
						faxapp_page_end();
					}

					fsu_current_funcs &= ~FAX_MEM_TO_PRINT;

					isFaxPrintBusy = 0;
					isReadyToFaxPrint = FALSE;
					gFaxPrintCurrentPage = 0;
					duplex_print_page_count = 0;
					user_input_print_cancel = 0;
					
					if (fsu_get_print_reason(fsu_job_num_prn) == PRINT_REASON_RECEIVE)
					{
						faxapp_process_tx_rx_result_report();
					}
					
					if(prn_cancel_flag == TRUE)
					{
						prn_cancel_flag = FALSE;
						if(del_job_pending)
						{
							printf("\n(YHCHO) %s() - pending print job done\n", __FUNCTION__);
							del_job_pending = 0;
							fsu_set_prn_job_done(fsu_job_num_prn);
						}
						else
						{
							printf("\n(YHCHO) %s() - print job clear check\n", __FUNCTION__);
							fsu_fax_print_job_clear();
						}
						
					}
				}
				else if(gFaxPrintCurrentPage < TotalPages)
				{
					activate_mem_to_prn();
				}
				else
				{
					faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_INVALID_RX_TOTAL_PAGE_2);
				}
            }
            else // receiving ..
            {
                if (fsu_get_print_reason(fsu_job_num_prn) == PRINT_REASON_RECEIVE)
                {
                    if(gFaxPrintCurrentPage < gFaxRxCurrentPage)
                    {
                        printf("\n(YHCHO) %s() - FLAG_PRINT_COMPLETE : gFaxPrintCurrentPage < gFaxRxCurrentPage\n", __FUNCTION__);
                        activate_mem_to_prn();
                    }
                    else // receiving pending ..
                    {
                        usleep(50*1000);
                        activate_mem_to_prn();
                    }
                }
                else
                {
                    TotalPages = fsu_get_total_pages(fsu_job_num_prn);

                    if(gFaxPrintCurrentPage >= (TotalPages + JobStartPageNo - 1) || user_input_print_cancel)
                    {
                        printf("\n(YHCHO) %s() - FLAG_PRINT_COMPLETE : OK\n", __FUNCTION__);
                        if(fsu_get_isDuplex(fsu_job_num_prn) && second_page_on_sheet && !user_input_print_cancel)
                        {
                            faxapp_page_end();
                        }

						fsu_current_funcs &= ~FAX_MEM_TO_PRINT;

                        isFaxPrintBusy = 0;
                        isReadyToFaxPrint = FALSE;
                        gFaxPrintCurrentPage = 0;
                        duplex_print_page_count = 0;
						user_input_print_cancel = 0;

                        if (fsu_get_print_reason(fsu_job_num_prn) == PRINT_REASON_RESET)
                        {
                            is1stPageDataTransfered = 0;
                        }
                    }
                    else
                    {
                        activate_mem_to_prn();
                    }
                }
			}
        }

        //printf("\n(YHCHO) %s() - FLAG_PRINT_COMPLETE -> END\n", __FUNCTION__);
    }    
    else
    {
        //
    }
}


/****************************************************************************
 *  Function: fipp_main_thread
 *
 *  Description:
 *    waiting for activation on each function and start each fax app. process
 *    when activated.
 *
 *  Inputs:
 *    none
 *
 ****************************************************************************/
void *fipp_main_thread (void* unused)
{
    UINT8 isFIPPAppInit = 0;

    extern char * get_version_FaxApp(void);

    printf("\n(YHCHO) %s()\n", __FUNCTION__);

    fipp_init_job();

    gpBufferForTxData = (UINT8 *)MEM_MALLOC(TEMP_TX_DATA_BUFFER);
	if( gpBufferForTxData == 0 )
	{
        XASSERT(0,0);
	}

    while (TRUE)
    {
        if (isFSUReady == 1)
            break;

        tx_thread_sleep(100);
    }
    
    fsu_check_sdcard_enable();

    fsu_make_doc_man_dir_in_ram();
	
    fsu_make_doc_man_dir_in_nand();

    fsu_make_sol_doc_man_dir_in_nand();
    
    isFaxDocInit = 1;

    while(1)
    {
        fipp_scan_to_mem();
        fipp_mem_to_send();
        fipp_rcv_to_mem();
        fipp_mem_to_prn();

        if (isFIPPAppInit == 0)
        {
            printf("\n\n\n\n%s() - Fax App. Version: %s\n\n\n\n", __FUNCTION__, get_version_FaxApp());
			
            if (fsu_get_memory_full_status_nand() == 1)
            {
				; //
            }
			
            isFIPPAppInit = 1;

            faxapp_delete_all_records_recd_faximagebackup();

            faxapp_delete_all_records_recd_faxbroadcastresultreport();

            faxapp_request_update_user_id();

            fsu_retrieve_job_queue(ValidJobCnt);
        }

        if (gFaxRebootCnt != 0)
        {
            system("reboot");
        }

#if __SUPPORT_MANUAL_RECEIVE__
        UINT8 Len = strlen((const char *)ExtPhoneDialBuffer);

        if (Len != 0)
        {
            int index = 0;
            UINT8 ret = 0;
            //UINT8 ret = strncmp(RemoteAccessCode, (const char *)ExtPhoneDialBuffer, MAX_LEN_REMOTE_ACCESS_CODE);
            for(index = 0; index < LEN_PHONE_NO - 2; index++)
            {
                if(ExtPhoneDialBuffer[index] == RemoteAccessCode[0])
                {
                    if(ExtPhoneDialBuffer[index + 1] == RemoteAccessCode[1] && ExtPhoneDialBuffer[index + 2] == RemoteAccessCode[2])
                    {
                        ret = 1;
                        break;
                    }
                }
            }

            if (ret != 0)
            {
                UINT8 FaxReceiveSource = RECEIVE_SOURCE_PHONE;

                printf("\n(YHCHO) %s() - Manual Receive (1)\n", __FUNCTION__);

                Fax_API_T30Attribute (SET, FXA_RECEIVESOURCE, &FaxReceiveSource);

                Fax_API_ManReceiveFax();

                memset(ExtPhoneDialBuffer, 0x00, sizeof(ExtPhoneDialBuffer));

                ExtPhoneDialIndex = 0;

                isLineInUse = 0;
            }
        }

        Len = strlen(MachineKeyBuffer);

        if (Len != 0)
        {
            UINT8 ret = strncmp(MachineKeyBuffer, OnHookPressed[ON_HOOK], MAX_LEN_MACHINE_ACCESS_CODE - 1); // On-Hook Key

            if ( (ret == 0) && (loopExitCnt == 0) )
            {
                printf("\n(YHCHO) %s() - Manual Receive (2-1)\n", __FUNCTION__);

                Fax_API_OffHook();

                loopExitCnt++;

                //faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PRESS_START_TO_RECEIVE, 0, 0, 0, 0, EMPTY_JOB_NUM);

                printf("\n(YHCHO) %s() - Notify - FAX_APP_STATUS_MSG_PRESS_START_TO_RECEIVE\n", __FUNCTION__);
            }
            else if(offHookDone == 1)
            {
                ret = strncmp(MachineKeyBuffer, OnHookPressed[ON_HOOK_CANCEL], MAX_LEN_MACHINE_ACCESS_CODE); // Cancel Condition - On-Hook Key + Cancel Key

                if (ret == 0)
                {
                    printf("\n(YHCHO) %s() - Cancel\n", __FUNCTION__);

                    Fax_API_OnHook();
                    
                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, CANCEL_TYPE_RX, 0, 0, 0, EMPTY_JOB_NUM);

                    memset(MachineKeyBuffer, 0x00, sizeof(MachineKeyBuffer));

                    MachineKeyBufferIndex = 0;

                    isLineInUse = 0;

                    loopExitCnt = 0;
                }

                ret = strncmp(MachineKeyBuffer, OnHookPressed[ON_HOOK_START], MAX_LEN_MACHINE_ACCESS_CODE); // Receive Condition - On-Hook Key + Start Key

                if (ret == 0)
                {
                    UINT8 FaxReceiveSource = RECEIVE_SOURCE_MACHINE;

                    printf("\n(YHCHO) %s() - Manual Receive (2-2)\n", __FUNCTION__);

                    Fax_API_T30Attribute (SET, FXA_RECEIVESOURCE, &FaxReceiveSource);

                    Fax_API_ManReceiveFax();

                    memset(MachineKeyBuffer, 0x00, sizeof(MachineKeyBuffer));

                    MachineKeyBufferIndex = 0;

                    isLineInUse = 0;

                    loopExitCnt = 0;
                }
            }
        }
#else
#endif

#if __OVERRUN_ERROR__
        tx_thread_sleep(500);
#else
        tx_thread_sleep(100);
#endif
    }

    /* should never get here! */
    //ASSERT(0);

    return 0;
}


void ref_fipp_tx_data_to_modem(void)
{
	JOBNODE_BUFFER job_buffer;
	JOBNODE_BUFFER *pBuff = &job_buffer;
    UINT32 size_to_modem = 0;
    UINT32 frame_size = 256;
    UINT8 *dmb_ptr = gpBufferForTxData;

    while( (gnSizeForTxDataBuffer != 0) && ((gnSizeForTxDataBuffer - size_to_modem) >= frame_size) )
    {
        pBuff->data = (UINT8 *)dmb_ptr;
        pBuff->size = pBuff->used = frame_size;
        pBuff->lastBuffer = 0;

        size_to_modem += frame_size;

        dmb_ptr += frame_size;

        if( (gnSizeForTxDataBuffer - size_to_modem) == 0 )
            pBuff->lastBuffer = 1;

        give_dmb_to_modem(pBuff);
    }

    if (gnSizeForTxDataBuffer == 0)
    {
        // No Action
    }
    else
    {
        if( (gnSizeForTxDataBuffer - size_to_modem) != 0 )
        {
            pBuff->data = (UINT8 *)dmb_ptr;
            pBuff->size = pBuff->used = gnSizeForTxDataBuffer - size_to_modem;
            pBuff->lastBuffer = 1;

            give_dmb_to_modem(pBuff);
        }
    }
}


void fsu_set_event_flags_tx_complete (void)
{
    tx_event_flags_set(&fipp_tx_events_flags, FLAG_TX_COMPLETE, TX_OR);
}


void fsu_set_event_flags_rx_complete (void)
{
    tx_event_flags_set(&fipp_rx_events_flags, FLAG_RX_COMPLETE, TX_OR);
}


int fsu_get_scan_busy_flag (void)
{
    return isFaxScanBusy;
}


void fsu_set_scan_busy_flag(UINT8 newValue)
{
    isFaxScanBusy = newValue;
}


int fsu_print_page_setting(void)
{
	if (fsu_get_print_reason(fsu_job_num_prn) == PRINT_REASON_RECEIVE)
	{
		gFaxPrintCurrentPage++;
	}
	else
	{
		if (is1stPageDataTransfered == 0)
		{
			gFaxPrintCurrentPage = fsu_get_start_page_no_in_mem(fsu_get_doc_id(fsu_job_num_prn), FAX_FILE_TYPE_JBG);
			JobStartPageNo = gFaxPrintCurrentPage;
		}
		else
		{
			gFaxPrintCurrentPage++;
		}
	}

	duplex_print_page_count++;
	
	return 1;
}

int fsu_print_image_setting(void)
{
	int ret_val = 0;
	char JBGFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char PBMFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char ARIAFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
	
	//T4 to pbm
	snprintf(ARIAFileName, sizeof(ARIAFileName), "%s%cDoc%04X%cfax%02X.jbg", 
				pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(fsu_job_num_prn), PATH_SEPARATOR, gFaxPrintCurrentPage);
	snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", 
				pRootFolderARIA, PATH_SEPARATOR, fsu_get_doc_id(fsu_job_num_prn), PATH_SEPARATOR, gFaxPrintCurrentPage);
	snprintf(PBMFileName, sizeof(PBMFileName), "%s%cDoc%04X%cfax%02X.pbm", 
				pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(fsu_job_num_prn), PATH_SEPARATOR, gFaxPrintCurrentPage);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
	{
		if(ARIA_encode_path(ARIAFileName, JBGFileName, 1, 1) == -1)
		{
			printf("\n(YHCHO) %s() - ARIA Decoding Fail\n", __FUNCTION__);

			return ret_val;
		}
	}
#endif
	//sw_jbig_decode_tx(gFaxPrintCurrentPage, fsu_job_num_prn, JBGFileName, FAX_APP_JOB_DIRECTION_PRINT); // Convert .jbg to .pbm
	ret_val = T4_Decoding(fsu_job_num_prn, JBGFileName, PBMFileName, CODING_MMR, fsu_get_width(fsu_job_num_prn),fsu_get_resolution(fsu_job_num_prn), FAX_APP_JOB_DIRECTION_PRINT);

	if(isSDCardEnable)
		fsu_delete_file_on_file_system (JBGFileName);

	if(ret_val)
	{
		printf("\n(YHCHO) %s() - T4 Decoding Fail\n", __FUNCTION__);
		
		return ret_val;
	}
	
	if(!fsu_image_validation_check(fsu_get_resolution(fsu_job_num_prn), fsu_job_num_prn))
	{
		ret_val = 1;
		printf("\n(YHCHO) %s() - invalid image\n", __FUNCTION__);
		
		return ret_val;
	}

	// Convert .pbm to .fax image
	ret_val = MakePBMToFaxImage(gFaxPrintCurrentPage);
	
	return ret_val;
}

int fsu_total_page_print_complete_check(void)
{
    int TotalPages = 0;

	if (fsu_get_print_reason(fsu_job_num_prn) == PRINT_REASON_RECEIVE)
	{
		TotalPages = fsu_get_total_pages(fsu_job_num_prn);
	}
	else
	{
		TotalPages = fsu_get_total_pages(fsu_job_num_prn) + JobStartPageNo - 1;
	}

    if (gFaxPrintCurrentPage == TotalPages)
    {
		return 1;
	}
	
	return 0;
}


void fsu_tx_image_process_begin (void)
{
    UINT8 isECMOn = 0;
    UINT8 CodingType = 0;
    UINT8 FaxRes = 0;
    UINT8 image_width_type = 0;
    UINT8 ret_val = 0;
    UINT16 jnum = 0;
    UINT16 DocId = 0;
    UINT16 OriginalDocId = 0;
    char ARIAFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JBGFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char PBMFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    
    gFaxSendCurrentPage = 0;

    if (T30TxCheckErrors() == 1)
    {
        return ;
    }

    printf("\n(YHCHO) %s() - START!\n", __FUNCTION__);

    printf("\n(YHCHO) %s() - RTNCount: %d\n", __FUNCTION__, RTNCount);

    jnum = fsu_job_num_send;

    isFSUTxImageProgessBegin = 0;

    if (RTNCount == 0)
    {
        gFaxCurrentTTIPageNo++;
    }

    T30Query(IOCTL_ECM_MODE_TX, &isECMOn, sizeof(UINT8));
    T30Query(IOCTL_TX_CODING_TYPE, &CodingType, sizeof(UINT8));
    T30Query(IOCTL_GET_TX_FAX_RES, &FaxRes, sizeof(UINT8));
    T30Query(IOCTL_GET_COMM_WIDTH_TX, &image_width_type, sizeof(UINT8));

    job_modem_init(0, isECMOn, CodingType, 256);

    DocId = fsu_get_doc_id(jnum);

    if (fsu_get_flag_manual_tx(jnum) == 1)
    {
        /* In case of on-hook, there is no need to check the current start page no from /DocMan/DocXXXX/
           since on-hook does not support retransmit after comm. error... */

        if (RTNCount == 0)
        {
            gFaxSendCurrentPageOnHook++;
        }

        gFaxSendCurrentPage = gFaxSendCurrentPageOnHook;
    }
    else
    {
        if (fsu_get_photo_mode(jnum) == 1)
        {
            if (fsu_get_broadcast(jnum) == 1)
            {
                OriginalDocId = fsu_get_original_doc_id(jnum);

                if (OriginalDocId == DocId)
                {
                    gFaxSendCurrentPage = fsu_requset_get_current_page_no_from_Job_Queue_File(jnum) + 1;
                }
                else
                {
                    gFaxSendCurrentPage = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);
                }
            }
            else
            {
                gFaxSendCurrentPage = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);
            }
        }
        else
        {
            if (fsu_get_broadcast(jnum) == 1)
            {
                OriginalDocId = fsu_get_original_doc_id(jnum);

                if (OriginalDocId == DocId)
                {
                    gFaxSendCurrentPage = fsu_requset_get_current_page_no_from_Job_Queue_File(jnum) + 1;
                }
                else
                {
                    gFaxSendCurrentPage = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                }
            }
            else
            {
                gFaxSendCurrentPage = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
            }
        }
    }

    printf("\n(YHCHO) %s() - gFaxSendCurrentPage: %d\n", __FUNCTION__, gFaxSendCurrentPage);

    // Step 1. T4 (MMR) Decode
    if(isSDCardEnable)
    {
		if( (fsu_get_photo_mode(jnum) == 1) && (FaxRes == 1) )
		{
			if (fsu_get_broadcast(jnum) == 1)
			{
				snprintf(ARIAFileName, sizeof(ARIAFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, fsu_get_original_doc_id(jnum), PATH_SEPARATOR, gFaxSendCurrentPage);
			}
			else
			{
				snprintf(ARIAFileName, sizeof(ARIAFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, gFaxSendCurrentPage);
			}
		}
		else
		{
			if (fsu_get_broadcast(jnum) == 1)
			{
				snprintf(ARIAFileName, sizeof(ARIAFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, fsu_get_original_doc_id(jnum), PATH_SEPARATOR, gFaxSendCurrentPage);
			}
			else
			{
				snprintf(ARIAFileName, sizeof(ARIAFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, gFaxSendCurrentPage);
			}
		}

		snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderARIA, PATH_SEPARATOR, DocId, PATH_SEPARATOR, gFaxSendCurrentPage);
	}
	else
	{
		if( (fsu_get_photo_mode(jnum) == 1) && (FaxRes == 1) )
		{
			if (fsu_get_broadcast(jnum) == 1)
			{
				snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, fsu_get_original_doc_id(jnum), PATH_SEPARATOR, gFaxSendCurrentPage);
			}
			else
			{
				snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, gFaxSendCurrentPage);
			}
		}
		else
		{
			if (fsu_get_broadcast(jnum) == 1)
			{
				snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, fsu_get_original_doc_id(jnum), PATH_SEPARATOR, gFaxSendCurrentPage);
			}
			else
			{
				snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, gFaxSendCurrentPage);
			}
		}
	}

    snprintf(PBMFileName, sizeof(PBMFileName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, gFaxSendCurrentPage);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
	{
		if(ARIA_encode_path(ARIAFileName, JBGFileName, 1, 1) == -1)
		{
			;//TO DO: decode failure
		}
	}
#endif
    if (fsu_get_fax_forward(jnum) == 1)
    {
        T4_Decoding(jnum, JBGFileName, PBMFileName, CODING_MMR, fsu_get_width(jnum), fsu_get_resolution(jnum), FAX_APP_JOB_DIRECTION_TRANSMIT);
    }
    else
    {
        if( (fsu_get_photo_mode(jnum) == 1) && (FaxRes == 1) )
        {
            T4_Decoding(jnum, JBGFileName, PBMFileName, CODING_MMR, fsu_get_width(jnum), 1, FAX_APP_JOB_DIRECTION_TRANSMIT);
        }
        else
        {
            T4_Decoding(jnum, JBGFileName, PBMFileName, CODING_MMR, fsu_get_width(jnum), 3, FAX_APP_JOB_DIRECTION_TRANSMIT);
        }
    }

    if(isSDCardEnable)
		fsu_delete_file_on_file_system(JBGFileName);

    // Step 2. Re-Scale and Add TTI
    PBM_Rescaling(PBMFileName, FaxRes, jnum, image_width_type);
#if 0
	 //temp Code.. fake encoding
	if(CodingType == CODING_JBIG)
		CodingType = CODING_MMR;
	else if(CodingType == CODING_MMR)
		CodingType = CODING_MR;
	else if(CodingType == CODING_MR)
		CodingType = CODING_MH;
	else //CODING_MH
		CodingType = CODING_MR;
#endif
    if (CodingType == CODING_JBIG)
    {
        sw_jbig_encode_tx(gFaxSendCurrentPage, PBMFileName, jnum, FAX_APP_JOB_DIRECTION_TRANSMIT);
    }
    else if ( (CodingType == CODING_MH) || (CodingType == CODING_MR) || (CodingType == CODING_MMR) )
    {
        ret_val = T4_encoding(gFaxSendCurrentPage, PBMFileName, CodingType, FaxRes, jnum, FAX_APP_JOB_DIRECTION_TRANSMIT, FAX_APP_PHOTO_MODE_IMAGE_RES_300DPI);

        if (ret_val != 0)
        {
            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_T4_ENCODING);
        }
    }
    else
    {
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_INVALID_TX_CODING_TYPE);
    }

    if (T30TxCheckErrors() == 1)
    {
        return ;
    }

    // Step 3. Update buffer for modem
    fsu_prepare_send_data(gFaxSendCurrentPage);

    isFSUTxImageProgessBegin = 1;

    isTxImageProcessDone = 1;

    printf("\n(YHCHO) %s() - END!\n", __FUNCTION__);
}

