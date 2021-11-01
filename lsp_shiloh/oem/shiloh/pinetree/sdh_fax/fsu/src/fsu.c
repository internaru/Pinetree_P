/* ************************************************************************* **
**                                                                           **
**                 Proprietary and Confidential Information                  **
**                                                                           **
**                Copyright (c) 2001 Oasis Semiconductor Inc.                **
**                                                                           **
**  This software contains proprietary and confidential material which is    **
**  property of Oasis Semiconductor Inc.                                     **
**                                                                           **
** ************************************************************************* */
/*
    File: fsu.c
    Date: October 23, 2001  
    By:   S.J. Busak

    Contains the "main" function for the FAX Scheduler Unit (FSU) of a 4-in-1 MFP.

    Busak: May 6, 2002: Modified for new fxcb control block.
    Busak: March 16, 2005: Broke up FSU into various parts.

*/

#define __FSU_C__      // Allocates storage for variables where necessary
#include <stdio.h>
// ------------------ //
// Oasis Header files:
// ------------------ //

#include "dc_common_includes.h"
#include "fxcb.h"
#include "fsu.h"
#include "fsu_call.h"
#include "fsu_api.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "SDHFaxCommonHeader.h"
#include <dirent.h>
#include "image_backup.h"
#include <sys/time.h>
#include "../../faxapp/include/faxapp_notify.h"
#include "../../fax_memmgr/include/fax_memAPI.h"
#include <time.h>
#include <sys/vfs.h>
#include "ERROR.H"
#include "t30.h"
#include <math.h>


extern char gcTSI_for_RemoteFaxNum[FCFSendTableSize + 1];
extern char gcCIG_for_RemoteFaxNum[FCFSendTableSize + 1];
extern char gcCID_for_RemoteFaxNum[FCFSendTableSize + 1];

typedef int32_t error_type_t;
#define uint32_t unsigned long

extern void job_phone_start( void );
extern void activate_phone_job( void );
extern void activate_mem_to_prn(void);
extern void faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_t StatusType, int CurrentPageNo, int TotalPageNo, int ConnectedSpeed, unsigned int SharedMemID, int JobID);
extern void faxapp_notify_SendFaxMemoryStatus(int FaxMemory);
extern void faxapp_init_parameter (void);
extern void set_txq_nxt (void);
extern void faxapp_delete_record_var(UINT8 Reason);
extern void fsu_set_next_prn_num(UINT16 next_prn_num);
extern void faxapp_solution_request_send (UINT16 jnum);
extern void faxapp_request_update_user_id (void);
extern void FaxSetPhoneNumber(int FaxType, char *FaxNumber);
extern void FaxSetJobLog(char *FileName, UINT32 jobID, int destCount);
//extern void FaxSetFileName(char *FileName);
extern char *fsu_request_get_time_to_tx_from_Job_Queue_File (UINT16 DocId);
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern char *faxapp_get_ip_address (void);
#endif
extern int faxapp_get_data_count(UINT8 Reason);
extern int faxapp_get_max_record_no_RECD_FAXLOG (void);
extern int faxapp_get_max_record_no_RECD_FAXSPLOG (void);
extern UINT8 modem_spi_open( void );
extern UINT8 fsu_get_total_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_request_get_retransmit_from_Job_Queue_File (UINT16 DocId);
extern UINT8 getCountryDateType (void);
extern UINT16 fsu_get_last_page_no_in_mem (UINT16 DocId, UINT8 FileType, UINT8 Mode);
extern UINT16 fsu_request_get_original_doc_id_from_Job_Queue_File (UINT16 DocId);
extern UINT32 faxapp_create_shared_mem_for_display_transmit(UINT8 *DialNo);
extern UINT16 fsu_get_next_prn_num(void);
extern BOOLEAN fsu_get_doc_exist (UINT16 DocId);
#if __TX_REPORT_IMAGE_PRINT__
void tx_report_image_setting(UINT16 jnum, int type);

extern int getgFaxTxResultRptImage(void);
#endif

error_type_t db_set_recd_fax_log(uint32_t _dateTime, uint32_t _faxMode, uint32_t _ecm, uint32_t _resolution, char* _remoteNum, uint32_t _duration, uint32_t _pageNum, uint32_t _connectSpeed, uint32_t _result, uint32_t _recName);
error_type_t db_set_recd_fax_sp_log(uint32_t _dateTime, uint32_t _faxMode, uint32_t _ecm, uint32_t _resolution, char* _remoteNum, uint32_t _duration, uint32_t _pageNum, uint32_t _connectSpeed, uint32_t _result, uint32_t _recName);

int isFaxPrintBusy = 0;
int isFSUModemReady = 0;
int isFSUPhoneReady = 0;
int isFSUT30Ready = 0;
int isFSUReady = 0;
int isFaxCardReady = 0;
static int gFaxBCAbnormalCnt = 0;
void fsu_remove_tx_job(UINT16 jnum);
extern void set_tx_job_ready(UINT16 jnum);
extern void fsu_make_doc_dir_in_ram(UINT16 DocId);
extern int getSaveFaxFile(void);
extern int getgFaxPrintMode(void);

extern int FileFormat;
extern int PageType;
extern char gFaxSpamFaxList[MAX_SPAM_FAX_LIST][LEN_PHONE_NO + 1];
extern char gFaxUserId[MAX_LEN_USER_ID + 1];
extern BOOLEAN isReadyToFaxPrint;
extern UINT32 gnSizeForTxDataBuffer;
extern UINT8 *gpBufferForTxData;
extern UINT8 fsu_current_funcs;
extern UINT8 NoOfSpamFaxList;
extern stIC_JobDoneInfo g_faxScanInfo;

extern UINT16 fsu_job_num_scan;
extern UINT16 fsu_job_num_send;
extern UINT16 fsu_job_num_rcv;
extern UINT16 fsu_job_num_prn;
extern UINT16 fsu_job_num_poll;
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];
extern long gFaxJobStartTimeForDuration;
extern long gFaxJobEndTimeForDuration;
extern int isPhoneRingOn;

char *pCurrentDirName = ".";
char *pPreviousDirName = "..";

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
char *pRootFolderName = "/mnt/sd/DocMan";
char *pRootFolderNameSolDocMan = "/mnt/sd/SolDocMan";
char *pRootFolderARIA = "/DocMan";

extern UINT8 isSDCardEnable;
extern char nameShareMem[MAX_LEN_FILE_PATH_NAME];
#else
char *pRootFolderName = "/DocMan";
char *pRootFolderNameSolDocMan = "/SolDocMan";
#endif

char *pRootFolderNameRAM = "/tmp/DocMan";
char *pRAM = "/tmp/";
#if __TX_REPORT_IMAGE_PRINT__
char *pTxResultRptImageName = "/faxTx.raw";
#endif
#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
char CurrentLocalJobTime[MAX_STRING_LEN] = {0, };
#else
#endif

extern JOB_QUEUE_INFO_t FSUJobQueueInfo;

INFO_FOR_FAX_JOB_LOG_t FaxJobLog = { };
FSU_INFO_BROADCAST_RESULT_REPORT_t BCResultReport = { };

void fsu_update_image_width_pixel (UINT16 jnum, UINT32 width);
void fsu_update_image_height_pixel (UINT16 jnum, UINT32 height);
void fsu_update_total_pages (UINT16 jnum, UINT8 Page);
void fsu_update_doc_id (UINT16 jnum, UINT16 DocId);
void fsu_update_resolution (UINT16 jnum, UINT8 res);
void fsu_update_poll_transmit (UINT16 jnum, UINT8 Poll);
void fsu_purge_doc_dir(UINT16 jnum);
long fsu_get_current_system_time(void);
void fsu_activate_mem_to_send(UINT16 IndexToJobNo);
void fsu_activate_mem_to_send_poll(UINT16 IndexToJobNo);
void fsu_activate_poll_receive(UINT16 IndexToJobNo);
void fsu_activate_mem_to_prn(UINT16 IndexToJobNo);
void fsu_get_job_start_time (long CurrentTimeInSec);
void fsu_get_job_type (UINT8 JobType, UINT16 jnum);
void fsu_get_ecm (UINT8 ECM);
void fsu_get_res (UINT8 Resolution);
void fsu_get_remote_no (UINT8 FSUJobType, UINT16 jnum);
void fsu_get_duration (void);
void fsu_get_page_no (UINT8 PageNo);
void fsu_get_connected_speed (UINT8 Speed);
void fsu_get_result (UINT16 T30Result);
void fsu_update_tx_total_page_no (UINT16 jnum, UINT8 TxTotalPageNo);
void fsu_update_current_time_for_report (long CurrentTimeInSec);
void fsu_update_job_type_for_report (UINT8 JobType, UINT16 jnum);
void fsu_update_ecm_for_report (UINT8 ECM);
void fsu_update_res_for_report (UINT8 Resoluition);
void fsu_update_duration_for_report (long EndTime, long StartTime);
void fsu_update_page_no_for_report (UINT8 PageNo);
void fsu_update_connected_speed_for_report (UINT8 ConnectedSpeed);
void fsu_update_result_for_report (UINT16 T30Result);
void fsu_update_remote_no_for_report (UINT8 *DialNo);
void fsu_update_modulation_name_for_report (UINT8 Modulation);
void fsu_update_status (UINT16 jnum, UINT8 Status);
void fsu_set_mem_time(UINT16 jnum);
void fsu_update_broadcast_result_report_struct (int nValue, UINT8 reason);
UINT8 fsu_get_total_pages (UINT16 jnum);
UINT8 fsu_get_status (UINT16 jnum);
UINT8 fsu_get_poll_transmit (UINT16 jnum);
UINT8 fsu_get_width_info(UINT8 res, UINT32 Width);
UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
UINT8 fsu_request_check_broadcast (UINT16 DocId);
UINT8 fsu_get_broadcast (UINT16 jnum);
UINT8 fsu_get_broadcast_job_complete (UINT16 DocId);
UINT16 fsu_get_doc_id (UINT16 jnum);
UINT16 fsu_get_original_doc_id (UINT16 jnum);
UINT32 fsu_get_image_width_pixel (UINT16 jnum);
UINT32 fsu_get_image_height_pixel (UINT16 jnum);
int fsu_get_memory_full_status_nand(void);
int fsu_get_time_to_tx (UINT16 DocId);
void fsu_update_isDuplex (UINT16 jnum, UINT8 Duplex);
#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
char *fsu_get_current_local_job_time_in_string(void);
#else
#endif

extern BOOLEAN fsu_check_job_queue_file (const char *Path);
extern BOOLEAN fsu_check_doc_man_dir_in_ram (const char *Path);
extern void faxapp_make_pdf_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType, int Request, int Reason);
extern void faxapp_make_tif_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType);
extern void fsu_delete_file_on_file_system (const char *Path);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern void fsu_make_dir_on_file_system (const char *Path);
extern void faxapp_update_recd_fax_image_backup (JOB_QUEUE_INFO_UI_t *Info);
extern void faxapp_update_broadcast_result_report_info_to_db (FSU_INFO_BROADCAST_RESULT_REPORT_t *Info);
extern UINT8 fsu_request_get_total_page_no_from_Job_Queue_File (UINT16 DocId);
#ifdef HAVE_IMAGELOG
extern int ARIA_encode_path(char *in_file, char *out_file, int crypt_flag, int file_option);
#endif

#if __MEASURE_PERFORMANCE__
extern void faxapp_get_job_start_time(int JobType);
#endif // #if __MEASURE_PERFORMANCE__


/***********************************************************************
 *  Function: fsu_main
 *
 *  Description:
 *    fsu main thread
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *    none	  
 ************************************************************************/
void *fsu_main (void* unused)
{
    UINT8 ret_val = 0;
//#if __HAVE_FAXSOLUTION_DK__
//#else
    BOOLEAN isLineOn = TRUE;
    UINT8 PreviousLineStatus = PHONE_NOLINE;
    UINT8 CurrentLineStatus = 0;
//#endif
    isFSUModemReady = 1;

    ret_val = fsu_por();
    if (ret_val != 0)
    {
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_INIT);
    }
    
    isFaxCardReady = 1;

    while (1)
    {
        if (isFSUPhoneReady == 1)
            break;

        OSC_Sleep_Task (100);
    }
    activate_phone_job();

    while (1)
    {
        if (isFSUT30Ready == 1)
            break;

        OSC_Sleep_Task (100);
    }

    fsu_current_funcs = 0;

	set_fsu_state(FSU_STATE_IDLE);

    isFSUReady = 1;

    faxapp_init_parameter();

#if __HAVE_FAXSOLUTION_DK__
	//faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_LINE_IS_ON, 0, 0, 0, 0, EMPTY_JOB_NUM);
#endif

    while (1)
    {
//#if __HAVE_FAXSOLUTION_DK__
//#else
        CurrentLineStatus = Get_LineStatus();
        if (CurrentLineStatus != PreviousLineStatus)
        {
            if (isLineOn == TRUE)
            {
                if (CurrentLineStatus == PHONE_NOLINE) // No Line
                {
                    isLineOn = FALSE;
                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_NO_LINE, 0, 0, 0, 0, EMPTY_JOB_NUM);
                }
            }
            else
            {
                if (CurrentLineStatus != PHONE_NOLINE) // Line On
                {
                    isLineOn = TRUE;
                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_LINE_IS_ON, 0, 0, 0, 0, EMPTY_JOB_NUM);
                }
            }
            PreviousLineStatus = CurrentLineStatus;
        }
//#endif

        fsu_task ();

        fsu_check_prn_job();

#if __OVERRUN_ERROR__
        OSC_Sleep_Task (500);
#else
        OSC_Sleep_Task (100);
#endif
    }

    /* should never get here! */
    //ASSERT(0);

    return 0;
}


/***********************************************************************	
 *  Function: fsu_check_prn_job
 *
 *  Description:
 *    check print job and activate it if printing is available
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *    none	  
 **********************************************************************/
void fsu_check_prn_job(void)
{
    UINT8 Status = 0;
    UINT16 return_value_check = 0;

    if (getSaveFaxFile() == DISABLE)
    {
        if(isReadyToFaxPrint == FALSE)
        {
            return_value_check = fsu_get_next_prn_num();
            if(return_value_check < TX_JOBS)
            {
                printf("\n(YHCHO) %s() - return_value_check : %d\n", __FUNCTION__, return_value_check);
                fsu_job_num_prn = return_value_check;
                isReadyToFaxPrint = TRUE;
            }
        }

        if((isReadyToFaxPrint == TRUE) && (isFaxPrintBusy == 0))
        {
            if (getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_ALL)
            {
                printf("\n(YHCHO) %s() - FAX_PRINT_MODE_PRINT_ALL start\n", __FUNCTION__);
                isFaxPrintBusy = 1;
                
				//initial fax print job setting value
				fsu_update_isDuplex(fsu_job_num_prn, FAX_PRINT_ISDUPLEX_INITIAL);

                activate_mem_to_prn();

#if __MEASURE_PERFORMANCE__
                faxapp_get_job_start_time(MEM_TO_PRINT);
#endif // #if __MEASURE_PERFORMANCE__

            }
            else
            {
                Status = fsu_get_status(fsu_job_num_prn);

                //printf("\n(YHCHO) %s() - Status: %d, isReadyToPrint: %d, isFaxPrintBusy: %d\n", __FUNCTION__, Status, isReadyToFaxPrint, isFaxPrintBusy);
                if (Status == FSU_STATE_PRINT_READY || Status == FSU_STATE_RECEIVE_DONE)
                {
					printf("\n(YHCHO) %s() - FAX_PRINT_MODE_PRINT_EVERY_PAGE start\n", __FUNCTION__);
                    isFaxPrintBusy = 1;
 
					//initial fax print job setting value
					fsu_update_isDuplex(fsu_job_num_prn, FAX_PRINT_ISDUPLEX_INITIAL);

                    activate_mem_to_prn();
                }
            }
        }
    }
}


/***********************************************************************
 *  Function: fsu_check_tx_job
 *
 *  Description:
 *		check tx job on tx jobq in IDLE mode	
 *
 *  Inputs:
 *		none
 *
 *  Output:
 *		none
 ***********************************************************************/
void fsu_check_tx_job (void)
{
    if (Get_LineStatus() == PHONE_ONHOOK && isPhoneRingOn == 0)
    {
        if (tx_job_ready() == 1)  // Look for the current job that's ready..
        {
            if (fsu_get_memory_full_status_ram() == 1)
            {
                printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);

                // Something should be added...

                return;

            }
            else
            {
                UINT16 jnum = fsu_job_num_send;
                UINT32 SharedMemID = 0;

                printf ("***********************************\n");
                printf ("**           Time To Tx          **\n");
                printf ("***********************************\n");

                isPhoneRingOn = 1;

                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DIALING, 0, 0, 0, 0, jnum);

                fsu_current_funcs &= ~FAX_SCAN_TO_MEM;
                fsu_current_funcs |= FAX_MEM_TO_SEND_DIALING;

                gFaxJobStartTimeForDuration = fsu_get_current_system_time();

                fsu_get_job_start_time(gFaxJobStartTimeForDuration);

#if 0 // To avoid SC 539 when the first digit of dial number is space (0x20) in case of PC FAX
                if ( (txq.job[jnum].fax_num[0] == '\0') || (txq.job[jnum].fax_num[0] == 0x20) )
                {
                    faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_NO_VALID_DIAL_NO);
                }
#endif
                printf ("\n(YHCHO) txq.job[%d].fax_num: %s\n\n", jnum, txq.job[jnum].fax_num);

                set_phone_number((UINT8 *)&txq.job[jnum].fax_num, 0);

                SharedMemID = faxapp_create_shared_mem_for_display_transmit(txq.job[jnum].fax_num);

                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DIAL_NO_TRANSMIT, 0, 0, 0, SharedMemID, jnum);

                set_fsu_state( FSU_STATE_TX_DIAL );

                if (fsu_get_broadcast(jnum) == 1)
                {
                    fsu_update_broadcast_result_report_struct(gFaxJobStartTimeForDuration, BROADCAST_RESULT_REPORT_REASON_START_TIME);
                }

#if __TX_REPORT_IMAGE_PRINT__
                if(getgFaxTxResultRptImage() == ENABLE)
                {
                    tx_report_image_setting(jnum, 1);
                }
#endif

#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
                printf("\n(YHCHO) Transmit (%d) Start Time: %s\n", jnum, fsu_get_current_local_job_time_in_string());
#else
#endif
            }
        }
    }
}


/***********************************************************************
 *  Function: tx_job_ready()
 *
 *  Description:
 *    check job for tx 
 *
 *  Inputs: 
 *    none 
 *
 *  Output:
 *	   1: job ready
 *	   0: no job
 **********************************************************************/
UINT8 tx_job_ready (void)
{
    UINT16 i = 0;
    UINT8 rdy = 0;
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    for (i = 0; i < TX_JOBS; i++)
    {
        p_tx_job = &txq.job[i];

        if ( (p_tx_job->status == FSU_STATE_SENDING_READY) || (p_tx_job->status == FSU_STATE_RETRY_READY) )
        {
            if (time_to_tx (p_tx_job->tx_time) == 1
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
				&& p_tx_job->send_permission == SOLUTION_PERMISSION_STATE_ENABLE
#endif
            )
            {
                fsu_job_num_send = i;
                rdy = 1;
                break;
            }
        }
    }

    return rdy;
}


/***********************************************************************
 *  Function: poll_tx_job_ready()
 *
 *  Description:
 *    check job for tx poll
 *
 *  Inputs: 
 *    none 
 *
 *  Output:
 *	   1: job ready
 *	   0: no job
 ***********************************************************************/
UINT8 poll_tx_job_ready (void)
{
    UINT8 ret_val = 0;

    if ((fsu_get_poll_transmit(fsu_job_num_poll) == 1) && (fsu_get_status(fsu_job_num_poll) == FSU_STATE_SENDING_POLL_READY))
    {
        fsu_job_num_send = fsu_job_num_poll;

        ret_val = 1;
    }
    else
    {
        ret_val = 0;
    }

    return ret_val;
}


/***********************************************************************
 *  Function: time_to_tx()
 *
 *  Description:
 *    check transmit time
 *
 *  Inputs:
 *    long tx_time: time to transmit
 *
 *  Output:
 *	   1: transmit now
 *	   0: not ready
 ***********************************************************************/
UINT8 time_to_tx (long tx_time)
{
    UINT8 tx_now = 0;
    long cur_time = 0;

    if (tx_time != TRANSMIT_TIME_RIGHT_NOW)  // Delay Transmit?
    {
        cur_time = fsu_get_current_system_time();

        if (cur_time >= tx_time)
        {
            tx_now = 1;
        }
		else
		{
			tx_now = 0;
		}
    }
    else  // Immediate Transmit?
    {
        tx_now = 1;
    }

    return tx_now;
}


/***********************************************************************
 *  Function: fsu_fax_job_init()
 *
 *  Description:
 *
 *  Inputs: 
 *    none 
 *
 *  Output:
 *    none
 ***********************************************************************/
void fsu_fax_job_init(UINT8 Fax_Status)
{
	switch ( Fax_Status )
	{
        case FAX_SCAN_TO_MEM:
        {
            UINT16 jnum = fsu_job_num_scan;

            printf("\n(YHCHO) %s() - FAX_SCAN_TO_MEM\n", __FUNCTION__);

            if (fsu_get_flag_manual_tx(jnum) == 0)
            {
                UINT8 NoOfRecipient = faxapp_get_no_of_recipient();
                UINT8 IndexToRecipient = 0;
                UINT16 OriginalDocId = 0;

                if ( (NoOfRecipient == 0) || (NoOfRecipient == 1) )
                {
                    fsu_remove_tx_job(jnum);

                    if (fsu_get_poll_transmit(jnum) == 1)
                    {
                        fsu_update_poll_transmit(jnum, 0);
                    }
                }
                else
                {
                    OriginalDocId = fsu_get_original_doc_id(jnum);

                    for (IndexToRecipient = 0; IndexToRecipient < NoOfRecipient; IndexToRecipient++)
                    {
                        fsu_remove_tx_job(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]);
                    }
                }
            }
            else
            {
                fsu_remove_tx_job(jnum);
            }
            break;
        }
		
        case FAX_MEM_TO_SEND_DIALING:
        {
            UINT16 jnum = fsu_job_num_send;

            printf("\n(YHCHO) %s() - FAX_MEM_TO_SEND_DIALING\n", __FUNCTION__);

            txq.job[jnum].status = FSU_STATE_NULL;
				
            set_phone_state( PHONE_STATE_INIT );
				
			set_modem_idle ();
			set_fsu_state( FSU_STATE_IDLE );
			break;
        }

        case FAX_MEM_TO_SEND_BEGIN:
		case FAX_MEM_TO_SEND:
        {
            UINT16 jnum = fsu_job_num_send;

            printf("\n(YHCHO) %s() - FAX_MEM_TO_SEND_BEGIN/FAX_MEM_TO_SEND\n", __FUNCTION__);

            txq.job[jnum].status = FSU_STATE_NULL;
				
			set_modem_idle ();
			set_fsu_state( FSU_STATE_IDLE );	
            break;
        }
        
        case FAX_RECEIVE_TO_MEM_BEGIN:
        case FAX_RECEIVE_TO_MEM:
        {
            UINT16 jnum = fsu_job_num_rcv;

            printf("\n(YHCHO) %s() - FAX_RECEIVE_TO_MEM_BEGIN/FAX_RECEIVE_TO_MEM\n", __FUNCTION__);

            txq.job[jnum].status = FSU_STATE_NULL;
				
			set_modem_idle ();
			set_fsu_state( FSU_STATE_IDLE );	
            break;
        }
        		
        case 0xff:
        {
            printf("\n(YHCHO) %s() - 0xFF\n", __FUNCTION__);

            UINT16 IndexToJobNo = 0;
            UINT16 OriginalDocId = 0;

			txq.nxt = 0;

            for (IndexToJobNo = 0; IndexToJobNo < TX_JOBS; IndexToJobNo++)
            {
                txq.job[IndexToJobNo].status = FSU_STATE_NULL;
            }

            for (OriginalDocId = 0; OriginalDocId < MAX_NUM_DOC_ID; OriginalDocId++)
            {
                gFax_BC_Num[OriginalDocId].BC_Current_Num = 0;
            }

            set_modem_idle ();
            set_fsu_state( FSU_STATE_IDLE );
            break;
        }

        default:
        {
            printf("\n(YHCHO) %s() - Invalid Fax_Status: %d\n", __FUNCTION__, Fax_Status);
            break;
        }
    }
}


/***********************************************************************
 *  Function: fsu_por()
 *
 *  Description:
 *    
 *  Inputs: 
 *    none 
 *
 *  Output:
 *    none
 ***********************************************************************/
int fsu_por (void)
{
    UINT8 ret_val = 0;

    ret_val = modem_spi_open();

    if (ret_val == 1)
    {
        printf("\n(YHCHO) fsu_por() - modem_spi_open() Error!\n");

        return 1;
    }
    else
    {
        monitor_modem_idle();
        fsu_fax_job_init(0xff);

        return 0;
    }
}


// ------------------------------ //
// Process the Caller I.D. string //
// ------------------------------ //
void process_cid (void)
{
  int   i, j;
  UINT8 cksum = 0;
  int   idlen;
  UINT8 idtype;
  UINT8 ptype=0;
  int   plen=0;
  UINT8 state = 0;
  char *buf=NULL;
  UINT8 single = FALSE;   // Assume multiple message
  char rfa_num[3], rfa_name[3];
  UINT8 *CidBuffer;
  UINT8 daa_code;
  char  caller_id[CID_MAX];     // Caller I.D. buffer
  UINT8 ptr = 0;

  extern void* CIDBufferAddr(void);

Fax_API_T30Attribute(GET, FXA_COUNTRY_CODE, &daa_code);

  CidBuffer = CIDBufferAddr();

  idtype = CidBuffer[0];  // Assemble message type

  caller_id_name[0] = 0;  // Indicate name not received
  caller_id_num[0] = 0;   // Indicate number not received
  rfa_num[0] = 0;         // No reason for absence of number
  rfa_name[0] = 0;        // No reason for absence of name

  if (idtype == 0x80)
  {
//    dc_log_printf ("Calling Name Delivery\n");
}

  else if ( (idtype == 0x87) && (daa_code == DAA_CODE_JAPAN) )  // Japan CID?
  {
//    dc_log_printf ("Calling Number Japan\n");
}

  else if (idtype == 0x04)
  {
//    dc_log_printf ("Calling Number Delivery\n");
}

  else
  {
//    dc_log_printf ("Unknown type: 0x%x", idtype);
    return;  // Ignore it
  }

//  dc_log_printf ("idtype = 0x%x", idtype);         // TEST!!!

  if (idtype == 0x87)  // Japan CID?
  {
    idlen = CidBuffer[4];  // Assemble message length
    j = idlen + 7;  // Include Header, DLE-STX, service type, length and DLE-ETX

//    dc_log_printf ("JPN msglen = 0x%x", idlen);         // TEST!!!

    io_crc = 0;      // Initialize CRC

    for ( i = 0; i < j; i++ )
    {
      plen = CidBuffer[i];
    }

    //sendhex ("io_crc = ", io_crc);  // TEST!!!

    plen = CidBuffer[i++];

    cksum = plen ^ ((UINT8)io_crc & 0xFF);

    plen = CidBuffer[i++];

    cksum = cksum | (plen ^ (UINT8)(io_crc >> 8));

    //sendhex ("cksum = ", cksum);
  }

  else
  {
    idlen = CidBuffer[1];  // Assemble message length
    j = idlen + 3;  // Include message type, length and checksum in verification

//    dc_log_printf ("id len = 0x%x", idlen);  // TEST!!!

    for ( i = 0; i < j; i++ )
    {
      plen = CidBuffer[i];
      cksum = cksum + plen;
    }
  }

  if (cksum != 0)  // Checksum error?
  {
//    dc_log_printf ("CID Checksum Error!\n");  // TEST!!!
//    dc_log_printf ("CID cksum = 0x%x" , cksum);  // TEST!!!

    return;  // Don't process message
  }

  if (idtype == 0x87)  // Japan CID?
  {
    for (i = 0; i < ptr; i=i+2)  // Mask parity bits (in ASCII format)
    {
      if ( (caller_id[i] == '8' ) || (caller_id[i] == '9') )   // ASCII 8 or 9?
        caller_id[i] = caller_id[i] - 8;  // Convert to ASCII 0 or 1

      else if ( (caller_id[i] >= 'A') && (caller_id[i] <= 'F') )  // ASCII A to F?
        caller_id[i] = caller_id[i] - 0x0F;                       // Convert to ASCII 2 to 7
    }

    i = 5;                      // Index to parameter type

    if (idlen == 0x10)  //Length = DLE?
    {
//      dc_log_printf ("JCID: Len = DLE!\n");
      i = i + 2;                               // Adjust pointer to parameter type for special case
    }

//    dc_log_printf ("Japan Number\n");  // TEST!!!
  }

  else
  {
    i = 2;  // Point to first parameter type

    if ( (idtype == 0x04) &&  // Calling Number Delivery?
         (daa_code != DAA_CODE_UK) &&    // Not UK caller I.D.?
         (daa_code != DAA_CODE_JAPAN) )  // Not Japan caller I.D.?
    {
      plen = idlen;  // Length of entire message
      buf = (char*)caller_id_num;  // Point to destination buffer
      j = 0;         // Index into buffer
      state = 4;     // Skip over parameter type and length
      single = TRUE; // Remove time and date when done

//      dc_log_printf ("Single Message\n");  // TEST!!!
//      dc_log_printf ("plen = 0x%x", plen);  // TEST!!!
    }

    else
    {
//      dc_log_printf ("Multiple Message\n");  // TEST!!!
    }
  }

  while (idlen-- > 0)  // Until the end of the message...
  {
    switch (state)
    {
      case 0:  // Get parameter type high byte

        ptype = CidBuffer[i++];
        state++;

//        dc_log_printf ("ptype = 0x%x", ptype);  // TEST!!!
        state++;
        break;

      case 2:  // Get parameter length high byte

        plen = CidBuffer[i++];

//        dc_log_printf ("plen = 0x%x", plen);  // TEST!!!

        if ( (ptype == 0x02) || (ptype == 0x07) )  // Calling number or name?
        {
          buf = ptype == 2 ? (char*)caller_id_num : (char*)caller_id_name;
          j = 0;
          state = 4;
        }

#ifdef INDICATE_BLOCKED_AND_UNAVAILABLE_NUM // Indicate blocked or unavailable info
        else if ( (ptype == 0x04) || (ptype == 0x08) )  // Reason for absence of number or name?
        {
          buf = ptype == 4 ? (char*)rfa_num : (char*)rfa_name;
          j = 0;
          state = 4;
        }
#endif
        else  // Ignored type (e.g.: time, date)
        {
//          dc_log_printf ("Skip type: 0x%x", ptype);  // TEST!!!
          state = 0;

          i = i + plen;          // Skip parameter
          idlen = idlen - plen;  // Adjust count
        }

        break;

      case 4:  // Get parameter high byte

        buf[j] = CidBuffer[i++];

        j++;

        if (--plen == 0)
        {
          buf[j] = 0;  // NULL Terminate
          state = 0;

//          dc_log_printf ((char*)buf);  // TEST!!!
//          dc_log_printf ("\n");  // TEST!!!
        }

        else
          state = 4;

        break;

      default:
//        dc_log_printf ("process_cid bombed!\n");  // TEST!!!
        return;
    }
  }

  if ( (rfa_name[0] != 0) || (rfa_num[0] != 0) )           // Name or number blocked/unavailable?
  {
    if (rfa_name[0] == 0x50)  // Blocked name?
    {
//      dc_log_printf ("Blocked CID Name!\n");  // TEST!!!
      caller_id_name[0] = CALLER_ID_BLOCKED;
      caller_id_name[1] = 0;
    }

    else if (rfa_name[0] == 0x4F)  // Unavailable name?
    {
//      dc_log_printf ("CID Name unavailable\n");  // TEST!!!
      caller_id_name[0] = CALLER_ID_UNAVAILABLE;
      caller_id_name[1] = 0;
    }

    if (rfa_num[0] == 0x50)  // Blocked number?
    {
//      dc_log_printf ("Blocked CID Number!\n");  // TEST!!!
      caller_id_num[0] = CALLER_ID_BLOCKED;
      caller_id_num[1] = 0;
    }

    else if (rfa_num[0] == 0x4F)  // Unavailable number?
    {
//      dc_log_printf ("CID Num unavailable\n");  // TEST!!!
      caller_id_num[0] = CALLER_ID_UNAVAILABLE;
      caller_id_num[1] = 0;
    }
  }

  if ( (caller_id_name[0] != 0) || (caller_id_num[0] != 0) )  // Name/number was received?
  {
    if (single && (j > 8) )  // Single message greater than eight characters?
    {
      UINT8 k = FSU_STAT_CALLER_ID_STR_MAX;
      i = 0;
      j = 8;

      while ((caller_id_num[j] != 0) && k--)  // Remove time & date
        caller_id_num[i++] = caller_id_num[j++];

      caller_id_num[i] = 0;  // Null terminate

      if (caller_id_num[0] == 0x50)  // Blocked number?
      {
//        dc_log_printf ("Blocked S-CID Number!\n");  // TEST!!!
        caller_id_num[0] = CALLER_ID_BLOCKED;
        caller_id_num[1] = 0;
      }

      else if (caller_id_num[0] == 0x4F)  // Number unavailable?
      {
//        dc_log_printf ("S-CID Num unavailable\n");  // TEST!!!
        caller_id_num[0] = CALLER_ID_UNAVAILABLE;
        caller_id_num[1] = 0;
      }
    }

    j = 2;  // Loop twice for name & number

    while (j--)
    {
      if ( ( (j != 0) && (caller_id_name[0] != 0) ) ||  // Doing name and it was received?
           ( (j == 0) && (caller_id_num[0] != 0) ) )    // Doing number and it was received?
      {
        buf = j != 0 ? (char*)caller_id_name : (char*)caller_id_num;  // Point to name/number
        i = 0;                                                        // Set index to 0

        while ( (buf[i] != 0) && (i < FSU_STAT_CALLER_ID_STR_MAX) )
          i++;  // Calculate length of string
      }
    }
  }
}


/***********************************************************************
 *  Function: fsu_remove_scan_job()
 *
 *  Description:
 *    set job status to FSU_STATE_NULL (NULL) when scan_to_mem is canceled
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_remove_scan_job(UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];

    p_tx_job->status = FSU_STATE_NULL;
}


/***********************************************************************
 *  Function: fsu_remove_tx_job()
 *
 *  Description:
 *    set job status to FSU_STATE_NULL (NULL) when mem_to_send is canceled or completed
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_remove_tx_job(UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];

    p_tx_job->status = FSU_STATE_NULL;
}


/***********************************************************************
 *  Function: fsu_remove_rx_job()
 *
 *  Description:
 *    set job status to FSU_STATE_NULL (NULL) when rcv_to_mem is canceled
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_remove_rx_job(UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];

    p_tx_job->status = FSU_STATE_NULL;
}


/***********************************************************************
 *  Function: fsu_remove_prn_job()
 *
 *  Description:
 *    set job status to FSU_STATE_NULL (NULL) when mem_to_prn is canceled or completed
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_remove_prn_job(UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];

    p_tx_job->status = FSU_STATE_NULL;
}


/***********************************************************************
 *  Function: fsu_set_prn_job_ready()
 *
 *  Description:
 *    set job status to FSU_STATE_PRINT_READY
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_set_prn_job_ready(UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];

    p_tx_job->status = FSU_STATE_PRINT_READY;
}


/***********************************************************************
 *  Function: fsu_set_prn_job_done()
 *
 *  Description:
 *    set job status to FSU_STATE_PRINTING_DONE
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_set_prn_job_done(UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];

    p_tx_job->status = FSU_STATE_NULL;
}


/***********************************************************************
 *  Function: fsu_set_mem_time()
 *
 *  Description:
 *    set mem_time.
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_set_mem_time(UINT16 jnum)
{
    struct timeval CurrentTimeCheck;
    struct tm *ptm;
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];
    
    gettimeofday(&CurrentTimeCheck, NULL);

	ptm = localtime(&CurrentTimeCheck.tv_sec);

    if (ptm == NULL)
    {
        snprintf(p_tx_job->mem_time, sizeof(p_tx_job->mem_time), "%02d:%02d", 0, 0);
    }
    else
    {
        snprintf(p_tx_job->mem_time, sizeof(p_tx_job->mem_time), "%02d:%02d", ptm->tm_hour, ptm->tm_min);
    }
}


/***********************************************************************
 *  Function: fsu_set_rcv_job_done()
 *
 *  Description:
 *    set job status to FSU_STATE_RECEIVE_DONE (NULL) when rcv_to_mem is completed
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_set_rcv_job_done (UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];

    p_tx_job->status = FSU_STATE_RECEIVE_DONE;
}


/***********************************************************************
 *  Function: fsu_set_send_job_done()
 *
 *  Description:
 *    set job status to FSU_STATE_SENDING_DONE (NULL) when mem_to_send is completed
 *
 *  Inputs:
 *    UINT16 jnum: job number to set
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_set_send_job_done (UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    p_tx_job = &txq.job[jnum];

    p_tx_job->status = FSU_STATE_NULL;
}


void fsu_prepare_send_data (int PageIndex)
{
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    UINT16 jnum = fsu_job_num_send;
    UINT32 wid = 0;
    UINT32 hig = 0;
    int fhandle = 0;
    int size = 0;

    wid = fsu_get_image_width_pixel(jnum);

    hig = fsu_get_image_height_pixel(jnum);

    snprintf(PathName, sizeof(PathName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, PageIndex);

    fhandle = open(PathName, O_RDWR);
    if(fhandle < 0)
    {
        printf("\n(YHCHO) %s() - open Fail : %s\n", __FUNCTION__,PathName);

        XASSERT(0,fhandle);
    }
    else
    {
        size = read(fhandle, gpBufferForTxData, wid*hig);
        if (size < 0)
        {
            printf("\n(YHCHO) %s() - read size < 0\n", __FUNCTION__);

            XASSERT(0,size);
        }
        else
        {
            gnSizeForTxDataBuffer = size;
            printf("\n(YHCHO) %s() - gnSizeForTxDataBuffer: %ld\n", __FUNCTION__, gnSizeForTxDataBuffer);
        }
    }

    close(fhandle);

    fsu_delete_file_on_file_system(PathName);
}


/**********************************************************************************
 *  Function: fsu_check_poll_tx_job
 *
 *  Description:
 *		To see whether another poll job is pending. If another poll job is pending,
 *      that job should be overwritten by new job.
 *
 *  Input:
 *		UINT16 jnum: job number to delete
 *
 *	Output:
 *********************************************************************************/
void fsu_check_poll_tx_job(UINT16 jnum)
{
    if ( (fsu_get_poll_transmit(jnum) == 1) && (fsu_get_status(jnum) == FSU_STATE_SENDING_POLL_READY) )
    {
        fsu_update_poll_transmit(jnum, 0);

        fsu_remove_tx_job(jnum);

        fsu_purge_doc_dir(jnum);
    }
}


/***********************************************************************
 *  Function: fsu_purge_doc_dir
 *
 *  Description:
 *		delete all files in /DocMan/DocXXXX/
 *
 *  Input:
 *		UINT16 jnum: job number to delete
 *
 *	Output:
 ***********************************************************************/
void fsu_purge_doc_dir(UINT16 jnum)
{
    UINT8 TotalPageNo = 0;
    UINT8 StartPageNo = 0;
    UINT8 IndexToPageNo = 0;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
    char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };
    char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char photoFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    UINT16 DocId = fsu_get_doc_id(jnum);

    printf("\n(YHCHO) %s()\n", __FUNCTION__);

    // Step 1. Purge .jbg files in NAND

    TotalPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);

    StartPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);

    for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
    {
        snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPageNo);

        fsu_delete_file_on_file_system(jbgFileName);
    }

    // Step 2. Purge .photo files in NAND if exists

    TotalPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);

    StartPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);

    for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
    {
        snprintf(photoFileName, sizeof(photoFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPageNo);

        fsu_delete_file_on_file_system(photoFileName);
    }

    // Step 3. Delete Job Queue file if exists

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);

    if (fsu_check_job_queue_file(JobQueueFileName) == TRUE)
    {
        fsu_delete_file_on_file_system(JobQueueFileName);
    }

    // Step 4. Delete DocXXXX in NAND

    snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

    fsu_delete_dir_on_file_system(DocDirPathNAND);

    // Step 5. Delete DocXXXX in RAM if exists

    snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

    if (fsu_check_doc_man_dir_in_ram(DocDirPathRAM) == TRUE)
    {
        char pbmFileName[MAX_LEN_FILE_PATH_NAME] = {0, };

        TotalPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_PBM);

        StartPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_PBM);

        for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
        {
            snprintf(pbmFileName, sizeof(pbmFileName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPageNo);

            fsu_delete_file_on_file_system(pbmFileName);
        }

        TotalPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_TMP_JBG);

        StartPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_TMP_JBG);

        for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
        {
            snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPageNo);

            fsu_delete_file_on_file_system(jbgFileName);
        }

        fsu_delete_dir_on_file_system(DocDirPathRAM);
    }

    // Step 6. Notify memory capacity in percent for UI

    fsu_get_memory_full_status_nand();
}


void fsu_retrieve_job_queue(int ValidJobCnt)
{
    UINT16 IndexToJobNo = 0;
    UINT16 NotifyCnt = 0;

    for (IndexToJobNo = 0; IndexToJobNo < ValidJobCnt; IndexToJobNo++)
    {
        if ( (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING) || (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_TX_PROGRESSING) || (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING_DELAY) || (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_FORWARD) )
        {
            fsu_activate_mem_to_send(IndexToJobNo);
        }
        else if (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING_POLL)
        {
            fsu_activate_mem_to_send_poll(IndexToJobNo);
        }
        else if (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_POLL_RECEIVE)
        {
            fsu_activate_poll_receive(IndexToJobNo);
        }
        else
        {
            fsu_activate_mem_to_prn(IndexToJobNo);
        }
    }

    if (ValidJobCnt != 0)
    {
        NotifyCnt = ValidJobCnt - gFaxBCAbnormalCnt;

        if (NotifyCnt != 0)
        {
            faxapp_notify_SendFaxRequestReadDB(NotifyCnt);
        }
    }

    tx_thread_sleep(1000);

    if (fsu_get_memory_full_status_nand() == 1)
    {
        printf("\n(YHCHO) %s() - Memory Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..
    }
}


/***********************************************************************
 *  Function: fsu_get_memory_full_status_nand
 *
 *  Description:
 *		get the status of memory full in NAND (/)
 *
 *  Input:
 *		none
 *
 *	Output:
 *		0: not full
 *      1: full
 ***********************************************************************/
int fsu_get_memory_full_status_nand(void)
{
    char File[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RootFolderName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char SubFolderName[MAX_LEN_FILE_PATH_NAME] = {0, };
    int DocNo = 0;
    int Index = 0;
    int ret = 0;
    int memory_check = 0;
    unsigned long int total_file_size = 0;
    unsigned long int Size[MAX_NUM_DOC_ID] = {0, };
    const unsigned long int MAX_FAX_MEMORY_NAND = 40 * 1024 * 1024; // 40MB
    DIR *pDocManDir = (DIR *)NULL;
    DIR *pDocDir = (DIR *)NULL;
    struct dirent *DocManFileInfo = NULL;
    struct dirent *DocFileInfo = NULL;
    struct stat stStat;

    snprintf(RootFolderName, sizeof(RootFolderName), "%s", pRootFolderName);

    pDocManDir = opendir(RootFolderName);

    if (pDocManDir == (DIR *)NULL)
    {
        XASSERT(0, 0);
    }
    else
    {
        while ((DocManFileInfo = readdir(pDocManDir)) != NULL)
        {
            if ( (strcmp (DocManFileInfo->d_name, pCurrentDirName) == 0) || (strcmp (DocManFileInfo->d_name, pPreviousDirName) == 0) )
            {
                // No Action
            }
            else
            {
                memset(SubFolderName, 0x00, sizeof(SubFolderName));

                snprintf(SubFolderName, sizeof(SubFolderName), "%s%c%s", pRootFolderName, PATH_SEPARATOR, DocManFileInfo->d_name);

                pDocDir = opendir(SubFolderName);

                if (pDocDir == (DIR *)NULL)
                {
                    XASSERT(0, 0);
                }
                else
                {
                    while ((DocFileInfo = readdir(pDocDir)) != NULL)
                    {
                        if ( (strcmp (DocFileInfo->d_name, pCurrentDirName) == 0) || (strcmp (DocFileInfo->d_name, pPreviousDirName) == 0) )
                        {
                            // No Action
                        }
                        else
                        {
                            memset(File, 0x00, sizeof(File));

                            snprintf(File, sizeof(File), "%s%c%s", SubFolderName, PATH_SEPARATOR, DocFileInfo->d_name);

                            if (stat(File, &stStat) != 0)
                            {
                                printf("\n(YHCHO) %s() - errorno: %d, %s", __FUNCTION__, errno, DocFileInfo->d_name);

                                XASSERT(0, 0);
                            }

                            Size[DocNo] += stStat.st_size;
                        }
                    }

                    DocNo++;

                    closedir(pDocDir);
                }
            }
        }

        closedir(pDocManDir);
    }

    for (Index = 0; Index < DocNo; Index++)
    {
        total_file_size += Size[Index];
    }

    total_file_size = total_file_size * 0.7; //70% compress rate, ubifs's compress rate is about 60%. 10% margin

    if(total_file_size < MAX_FAX_MEMORY_NAND)
    {
        memory_check = (MAX_FAX_MEMORY_NAND - total_file_size) / (MAX_FAX_MEMORY_NAND/100);
    }
    else
    {
        memory_check = 0;
    }

    faxapp_notify_SendFaxMemoryStatus(memory_check);

    printf("\n(YHCHO) %s() - total_file_size: %ld, memory_check = %d\n", __FUNCTION__, total_file_size, memory_check);

    if (memory_check < 4)
    {
        ret = 1; // Memory Full Status...
    }
    else
    {
        ret = 0; // Not Memory Full Status...
    }

    return ret;
}


/***********************************************************************
 *  Function: fsu_get_memory_full_status_ram()
 *
 *  Description:
 *    get the status of memory full for RAM (/tmp)
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *    0: not full
 *    1: full
 **********************************************************************/
int fsu_get_memory_full_status_ram (void)
{
    int ret_val = 0;
    int CurrentTotalSize = 0;
    struct statfs lstatfs;

    statfs(pRAM, &lstatfs);

    CurrentTotalSize = lstatfs.f_bavail * (lstatfs.f_bsize >> 10);

    if (CurrentTotalSize <= MIN_MEMORY_RAM)
    {
        printf("\n(YHCHO) %s() - CurrentTotalSize = %d\n", __FUNCTION__, CurrentTotalSize);
        ret_val = 1; // full
    }
    else
    {
        ret_val = 0; // not full
    }

    return ret_val;
}


/***********************************************************************
 *  Function: fsu_get_job_queue_full_status
 *
 *  Description:
 *		get the status of job queue full
 *
 *  Input:
 *		none
 *
 *	Output:
 *		0: not full
 *      1: full
 ***********************************************************************/
int fsu_get_job_queue_full_status(void)
{
    UINT16 IndexToJobNo = 0;
    UINT16 Count = 0;
    int ret_val = JOB_QUEUE_FULL_STATUS_FULL;

    for (IndexToJobNo = 0; IndexToJobNo < TX_JOBS; IndexToJobNo++)
    {
        if ((txq.job[IndexToJobNo].status == FSU_STATE_SCANNING) || (txq.job[IndexToJobNo].status == FSU_STATE_SENDING_POLL_READY) || (txq.job[IndexToJobNo].status == FSU_STATE_SENDING_READY) || (txq.job[IndexToJobNo].status == FSU_STATE_RECEIVING) || (txq.job[IndexToJobNo].status == FSU_STATE_RECEIVE_DONE) || (txq.job[IndexToJobNo].status == FSU_STATE_PRINT_READY) || (txq.job[IndexToJobNo].status == FSU_STATE_SENDING_PENDING))
        {
            Count++;
        }
    }

    printf("\n(YHCHO) %s() - Count: %d\n", __FUNCTION__, Count);

    if (Count == TX_JOBS)
    {
        ret_val = JOB_QUEUE_FULL_STATUS_FULL;
    }
    else
    {
        ret_val = JOB_QUEUE_FULL_STATUS_NOT_FULL;
    }

    return ret_val;
}


/***********************************************************************
 *  Function: fsu_get_doc_id
 *
 *  Description:
 *		get the doc id of document
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		UINT16 Doc Id
 ***********************************************************************/
UINT16 fsu_get_doc_id (UINT16 jnum)
{
    return(txq.job[jnum].doc_id);
}


/***********************************************************************
 *  Function: fsu_get_original_doc_id
 *
 *  Description:
 *		get the original doc id of document especially for broadcast
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		UINT16 Original Doc Id
 ***********************************************************************/
UINT16 fsu_get_original_doc_id (UINT16 jnum)
{
    return(txq.job[jnum].original_doc_id);
}


/***********************************************************************
 *  Function: fsu_get_resolution
 *
 *  Description:
 *		get the resolution of document
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		UINT8 resolution
 ***********************************************************************/
UINT8 fsu_get_resolution (UINT16 jnum)
{
    return(txq.job[jnum].res);
}


/***********************************************************************
 *  Function: fsu_get_width
 *
 *  Description:
 *		get the width of document
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		UINT8 width
 ***********************************************************************/
UINT8 fsu_get_width (UINT16 jnum)
{
    return(txq.job[jnum].width);
}


/***********************************************************************
 *  Function: fsu_get_image_width_pixel
 *
 *  Description:
 *		get the width of document in pixel
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		UINT32 width
 ***********************************************************************/
UINT32 fsu_get_image_width_pixel (UINT16 jnum)
{
    return (image.job[jnum].width);
}


/***********************************************************************
 *  Function: fsu_get_image_height_pixel
 *
 *  Description:
 *		get the height of document in pixel
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		UINT32 height
 ***********************************************************************/
UINT32 fsu_get_image_height_pixel (UINT16 jnum)
{
    return (image.job[jnum].height);
}


/***********************************************************************
 *  Function: fsu_get_total_pages()
 *
 *  Description:
 *    get the total number of pages
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *
 *  Output:
 *    UINT8 total page no.
 **********************************************************************/
UINT8 fsu_get_total_pages (UINT16 jnum)
{
    return(txq.job[jnum].TxRxTotalPages);
}


/***********************************************************************
 *  Function: fsu_get_dial_no
 *
 *  Description:
 *		get remote dial no
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		pointer to dial no.
 ***********************************************************************/
UINT8 *fsu_get_dial_no (UINT16 jnum)
{
    return(&txq.job[jnum].fax_num[0]);
}


/***********************************************************************
 *  Function: fsu_get_mem_time
 *
 *  Description:
 *		get time
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		pointer to mem_time.
 ***********************************************************************/
char *fsu_get_mem_time (UINT16 jnum)
{
    return(&txq.job[jnum].mem_time[0]);
}


/***********************************************************************
 *  Function: fsu_get_fax_forward()
 *
 *  Description:
 *    get whether fax forward is on
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0: Off
 *    1: On
 **********************************************************************/
UINT8 fsu_get_fax_forward (UINT16 jnum)
{
    return(txq.job[jnum].ff_job);
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: fsu_get_permission()
 *
 *  Description:
 *    get whether send permission is on
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0: Off
 *    1: On
 **********************************************************************/
UINT8 fsu_get_permission (UINT16 jnum)
{
    return(txq.job[jnum].send_permission);
}
#endif


/***********************************************************************
 *  Function: fsu_get_is_delay()
 *
 *  Description:
 *    get whether delay is on
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0: Off
 *    1: On
 **********************************************************************/
UINT8 fsu_get_is_delay (UINT16 jnum)
{
    return(txq.job[jnum].isDelay);
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: fsu_get_file_name_tx()
 *
 *  Description:
 *    get the file name for transmit
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    pointer to file name
 **********************************************************************/
char *fsu_get_file_name_tx (UINT16 jnum)
{
    return(&txq.job[jnum].file_name_tx[0]);
}
#endif


/***********************************************************************
 *  Function: fsu_get_delay_time_in_text()
 *
 *  Description:
 *    get the delay time in string
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *
 **********************************************************************/
char *fsu_get_delay_time_in_text (UINT16 jnum)
{
    return(&txq.job[jnum].DelayTimeInText[0]);
}


/***********************************************************************
 *  Function: fsu_get_status()
 *
 *  Description:
 *    get the current status
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    current status
 **********************************************************************/
UINT8 fsu_get_status (UINT16 jnum)
{
    return(txq.job[jnum].status);
}


/***********************************************************************
 *  Function: fsu_get_photo_mode()
 *
 *  Description:
 *    get the photo_mode
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0: Off
 *    1: On
 **********************************************************************/
UINT8 fsu_get_photo_mode (UINT16 jnum)
{
    return(txq.job[jnum].photo_mode);
}


/***********************************************************************
 *  Function: fsu_get_split_print()
 *
 *  Description:
 *    get the split_print
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0: 1 page per 1 image
 *    1: 2 page per 1 image
 **********************************************************************/
UINT8 fsu_get_split_print (UINT16 jnum)
{
    return(txq.job[jnum].split_print);
}


/***********************************************************************
 *  Function: fsu_get_user_id()
 *
 *  Description:
 *    get the user id
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    pointer to user id
 **********************************************************************/
char *fsu_get_user_id (UINT16 jnum)
{
    return(&txq.job[jnum].user_id[0]);
}

/***********************************************************************
 *  Function: fsu_get_isDuplex()
 *
 *  Description:
 *    get the isDuplex
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0: Simplex
 *    1: Duplex
 **********************************************************************/
UINT8 fsu_get_isDuplex (UINT16 jnum)
{
    return(txq.job[jnum].isDuplex);
}

/***********************************************************************
 *  Function: fsu_get_max_job_num()
 *
 *  Description:
 *    get max job num
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    current status
 **********************************************************************/
UINT16 fsu_get_max_job_num (void)
{
    return(TX_JOBS);
}


/***********************************************************************
 *  Function: fsu_get_poll_transmit()
 *
 *  Description:
 *    get the current poll transmit
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0: not poll
 *    1: poll
 **********************************************************************/
UINT8 fsu_get_poll_transmit (UINT16 jnum)
{
    return(txq.job[jnum].poll_transmit);
}


/***********************************************************************
 *  Function: fsu_get_poll_receive()
 *
 *  Description:
 *    get the current poll receive
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0: not poll
 *    1: poll
 **********************************************************************/
UINT8 fsu_get_poll_receive (UINT16 jnum)
{
    return(txq.job[jnum].poll_receive);
}


/***********************************************************************
 *  Function: fsu_get_unique_job_id()
 *
 *  Description:
 *    get the Uinique Job ID
 *
 *  Inputs:
 *    UINT16 jnum: jnum to get
 *
 *  Output:
 *    unique_job_id
 **********************************************************************/
UINT32 fsu_get_unique_job_id(UINT16 jnum)
{
    return(txq.job[jnum].unique_job_id);
}


/***********************************************************************
 *  Function: fsu_get_current_system_time()
 *
 *  Description:
 *    get the current system time in second
 *
 *  Inputs:
 *
 *  Output:
 *    current system time
 **********************************************************************/
long fsu_get_current_system_time(void)
{
    struct timeval tv;
    long CurrentSystemTime = 0;

    gettimeofday(&tv, NULL);

    CurrentSystemTime = tv.tv_sec;

    return (CurrentSystemTime);
}


/***********************************************************************
 *  Function: fsu_get_broadcast
 *
 *  Description:
 *		get the broadcast
 *
 *  Input:
 *		UINT16 jnum: job number to get
 *
 *	Output:
 *		UINT8 broadcast
 ***********************************************************************/
UINT8 fsu_get_broadcast (UINT16 jnum)
{
    return(txq.job[jnum].broadcast);
}


/***********************************************************************
 *  Function: fsu_get_flag_manual_tx()
 *
 *  Description:
 *    get the flag manual tx
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *
 **********************************************************************/
UINT8 fsu_get_flag_manual_tx(UINT16 jnum)
{
    return (txq.job[jnum].flag_manual_tx);
}


/***********************************************************************
 *  Function: fsu_get_retry()
 *
 *  Description:
 *    get the current retry
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *
 **********************************************************************/
UINT8 fsu_get_retry(UINT16 jnum)
{
    return (txq.job[jnum].retry);
}


/***********************************************************************
 *  Function: fsu_get_retry_tmr()
 *
 *  Description:
 *    get the retry timer in second
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *
 **********************************************************************/
UINT8 fsu_get_retry_tmr(UINT16 jnum)
{
    return(txq.job[jnum].retry_tmr);
}


/***********************************************************************
 *  Function: fsu_get_print_reason()
 *
 *  Description:
 *    get the print reason
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    0 for PRINT_REASON_RECEIVE
 *    1 for PRINT_REASON_RESET
 *
 **********************************************************************/
UINT8 fsu_get_print_reason (UINT16 jnum)
{
    return (txq.job[jnum].PrintReason);
}


/***********************************************************************
 *  Function: fsu_get_tx_total_page_no()
 *
 *  Description:
 *    get the tx total page no.
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    Tx Total Page No.
 *
 **********************************************************************/
UINT8 fsu_get_tx_total_page_no (UINT16 jnum)
{
    return (txq.job[jnum].TxTotalPageNo);
}


/***********************************************************************
 *  Function: fsu_get_ScanInfo_paperSize()
 *
 *  Description:
 *    get the paperSize
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    paperSize
 *
 **********************************************************************/
UINT8 fsu_get_ScanInfo_paperSize (UINT16 jnum)
{
    return (txq.job[jnum].faxScanInfo.paperSize);
}


/***********************************************************************
 *  Function: fsu_get_ScanInfo_duplex()
 *
 *  Description:
 *    get the duplex
 *
 *  Inputs:
 *    UINT16 jnum: job number to get
 *
 *  Output:
 *    duplex
 *
 **********************************************************************/
UINT8 fsu_get_ScanInfo_duplex (UINT16 jnum)
{
    return (txq.job[jnum].faxScanInfo.duplex);
}


/***********************************************************************
 *  Function: fsu_update_width
 *
 *  Description:
 *		update the width of document
 *
 *  Input:
 *		UINT16 jnum: job number to update
 *
 *	Output:
 *
 ***********************************************************************/
void fsu_update_width (UINT16 jnum, UINT8 width)
{
    txq.job[jnum].width = width;
}


/***********************************************************************
 *  Function: fsu_update_resolution
 *
 *  Description:
 *		update the resolution of document
 *
 *  Input:
 *		UINT16 jnum: job number to update
 *
 *	Output:
 *
 ***********************************************************************/
void fsu_update_resolution (UINT16 jnum, UINT8 res)
{
    txq.job[jnum].res = res;
}


/***********************************************************************
 *  Function: fsu_update_dialno
 *
 *  Description:
 *		update the dial num of document
 *
 *  Input:
 *		UINT16 jnum: job number to update
 *
 *	Output:
 *
 ***********************************************************************/
void fsu_update_dialno (UINT16 jnum, UINT8 *dialno)
{
	int i = 0;

    for(i = 0; i < LEN_PHONE_NO; i++)
	{
		if(dialno[i] != '\0')
		{
            txq.job[jnum].fax_num[i] = dialno[i];
		}
		else
		{
			break;
		}
	}

    txq.job[jnum].fax_num[i] = 0;
}


/***********************************************************************
 *  Function: fsu_update_image_width_pixel
 *
 *  Description:
 *		update the image width of document
 *
 *  Input:
 *		UINT16 jnum: job number to update
 *      UINT32 width: width to update
 *
 *	Output:
 *
 ***********************************************************************/
void fsu_update_image_width_pixel (UINT16 jnum, UINT32 width)
{
    image.job[jnum].width = width;
}


/***********************************************************************
 *  Function: fsu_update_image_height_pixel
 *
 *  Description:
 *		update the image hiehgt of document
 *
 *  Input:
 *		UINT16 jnum: job number to update
 *      UINT32 width: height to update
 *
 *	Output:
 *
 ***********************************************************************/
void fsu_update_image_height_pixel (UINT16 jnum, UINT32 height)
{
    image.job[jnum].height = height;
}


/***********************************************************************
 *  Function: fsu_update_total_pages()
 *
 *  Description:
 *    update the total number of pages
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 Page: page number to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_total_pages (UINT16 jnum, UINT8 Page)
{
    txq.job[jnum].TxRxTotalPages = Page;
}


/***********************************************************************
 *  Function: fsu_update_doc_id()
 *
 *  Description:
 *		update the doc id of document
 *
 *  Input:
 *		UINT16 jnum: job number to update
 *      UINT16 DocId: doc id to update
 *
 *	Output:
 *
 ***********************************************************************/
void fsu_update_doc_id (UINT16 jnum, UINT16 DocId)
{
    txq.job[jnum].doc_id = DocId;
}


/***********************************************************************
 *  Function: fsu_update_original_doc_id()
 *
 *  Description:
 *		update the original doc id of document
 *
 *  Input:
 *		UINT16 jnum: job number to update
 *      UINT16 DocId: doc id to update
 *
 *	Output:
 *
 ***********************************************************************/
void fsu_update_original_doc_id (UINT16 jnum, UINT16 DocId)
{
    txq.job[jnum].original_doc_id = DocId;
}


/***********************************************************************
 *  Function: fsu_update_poll_transmit()
 *
 *  Description:
 *    update the poll transmit
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 Poll
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_poll_transmit (UINT16 jnum, UINT8 Poll)
{
    txq.job[jnum].poll_transmit = Poll;
}


/***********************************************************************
 *  Function: fsu_update_poll_receive()
 *
 *  Description:
 *    update the poll receive
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 Poll
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_poll_receive (UINT16 jnum, UINT8 Poll)
{
    txq.job[jnum].poll_receive = Poll;
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: fsu_update_unique_job_id()
 *
 *  Description:
 *    update the unique job id
 *
 *  Inputs:
 *    UINT16 jnum: job id to update
 *    UINT32 jobId
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_unique_job_id (UINT16 jnum, UINT32 jobId)
{
    txq.job[jnum].unique_job_id = jobId;
}
#endif


/***********************************************************************
 *  Function: fsu_update_status()
 *
 *  Description:
 *    update the status
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 status: status to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_status (UINT16 jnum, UINT8 status)
{
    txq.job[jnum].status = status;
}


/***********************************************************************
 *  Function: fsu_update_photo_mode()
 *
 *  Description:
 *    update the photo_mode
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 photomode: photo mode to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_photo_mode (UINT16 jnum, UINT8 photomode)
{
    txq.job[jnum].photo_mode = photomode;
}

/***********************************************************************
 *  Function: fsu_update_isDuplex()
 *
 *  Description:
 *    update the isDuplex
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 Duplex: isDuplex to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_isDuplex (UINT16 jnum, UINT8 Duplex)
{
    txq.job[jnum].isDuplex = Duplex;
}

/***********************************************************************
 *  Function: fsu_update_split_print()
 *
 *  Description:
 *    update the split_print
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 splitprint: split_print to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_split_print (UINT16 jnum, UINT8 splitprint)
{
    txq.job[jnum].split_print = splitprint;
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: fsu_update_file_name_tx()
 *
 *  Description:
 *    update the file name
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    const char *pfile_name_tx: new file name to update
 *
 *  Output:
 *    pointer to file name
 **********************************************************************/
void fsu_update_file_name_tx (UINT16 jnum, const char *pfile_name_tx)
{	
    if (memcpy((&txq.job[jnum].file_name_tx[0]), &pfile_name_tx[0], strlen(pfile_name_tx) + 1) == (void *)NULL)
    {
        ASSERT(0);
    }
}

void fsu_clear_file_name_tx (UINT16 jnum)
{	
    memset(txq.job[jnum].file_name_tx, 0, MAX_LEN_FILE_PATH_NAME+1);
}
#endif


/***********************************************************************
 *  Function: fsu_update_status()
 *
 *  Description:
 *    update the status
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    FSU_STATE_t Status: status to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_pcfax_job_init (UINT16 jnum)
{
    txq.job[jnum].status = FSU_STATE_SCANNING;
	txq.job[jnum].isDelay = FALSE;
    txq.job[jnum].DelayTimeInText[0] = 0x20; // Add one space
    txq.job[jnum].DelayTimeInText[1] = 0;
	txq.job[jnum].tx_time = TRANSMIT_TIME_RIGHT_NOW;
	txq.job[jnum].width = 0;
	txq.job[jnum].ff_job = 0;
    txq.job[jnum].poll_transmit = 0;
    txq.job[jnum].poll_receive = 0;
	txq.job[jnum].flag_manual_tx = 0;
	txq.job[jnum].PrintReason = PRINT_REASON_RECEIVE;
	txq.job[jnum].send_permission = SOLUTION_PERMISSION_STATE_ENABLE;
}


/***********************************************************************
 *  Function: fsu_update_fax_forward_job_queue()
 *
 *  Description:
 *    update the job queue information for fax forward
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_fax_forward_job_queue (UINT16 jnum)
{
    UINT8 FaxForwardNo[LEN_PHONE_NO + 1] = {0, };
    UINT8 ForwardNo[LEN_PHONE_NO + 1] = {0, };
    UINT8 retry = 0;
    UINT8 retry_tmr = 0;
    UINT8 IndexToDialNo = 0;
    UINT8 Len = 0;

    Fax_API_T30Attribute(GET, FXA_FAXFORWARD_NO, FaxForwardNo);

    //printf("\n(YHCHO) %s() - FaxForwardNo: %s\n", __FUNCTION__, FaxForwardNo);

    Len = strlen((char *)FaxForwardNo);

    for(IndexToDialNo = 0; IndexToDialNo < Len; IndexToDialNo++)
    {
        if (FaxForwardNo[IndexToDialNo] == 0x2D) // '-' key
        {
            ForwardNo[IndexToDialNo] = 0x2C; // Pause Key
        }
        else
        {
            ForwardNo[IndexToDialNo] = FaxForwardNo[IndexToDialNo];
        }
    }

    ForwardNo[IndexToDialNo] = 0;

    //printf("\n(YHCHO) %s() - ForwardNo: %s\n", __FUNCTION__, ForwardNo);

    // Step 1. Dial No.
    memcpy(txq.job[jnum].fax_num, ForwardNo, sizeof(ForwardNo));

    //printf("\n(YHCHO) %s() - txq.job[%d].fax_num: %s\n", __FUNCTION__, jnum, txq.job[jnum].fax_num);

    // Step 2. isDelay
    txq.job[jnum].isDelay = FALSE;

    txq.job[jnum].DelayTimeInText[0] = 0x20; // Add one space
    txq.job[jnum].DelayTimeInText[1] = 0;

    Fax_API_T30Attribute(GET, FXA_RETRY_NUMBER, &retry);
    Fax_API_T30Attribute(GET, FXA_RETRY_INTERVAL, &retry_tmr);

    // Step 3. retry
    txq.job[jnum].retry = retry;

    // Step 4. retry_tmr
    txq.job[jnum].retry_tmr = retry_tmr;

    // Step 5. tx_time
    txq.job[jnum].tx_time = TRANSMIT_TIME_RIGHT_NOW; // Immediate Transmit

    // Step 6. res
    txq.job[jnum].res = fsu_get_resolution(jnum);

    // Step 7. width
    txq.job[jnum].width = fsu_get_width(jnum);

    // Step 8. ff_job
    txq.job[jnum].ff_job = 1;

    // Step 9. poll
    txq.job[jnum].poll_transmit = 0;
    txq.job[jnum].poll_receive = 0;

    // Step 10. broadcast
    txq.job[jnum].broadcast = 0;

    // Step 11. flag_manual_tx
    txq.job[jnum].flag_manual_tx = 0;

    // Step 12. doc_id
    txq.job[jnum].doc_id = fsu_get_doc_id(jnum);

    // Step 13. TxRxTotalPages
    txq.job[jnum].TxRxTotalPages = fsu_get_total_pages(jnum);

    // Step 14. TxTotalPageNo
    fsu_update_tx_total_page_no(jnum, txq.job[jnum].TxRxTotalPages);

    // Step 15. PrintReason
    txq.job[jnum].PrintReason = PRINT_REASON_RECEIVE;

    // Step 16. mem_time
    fsu_set_mem_time(jnum);

    // Step 17. Photo Mode
    txq.job[jnum].photo_mode = 0;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    // Step 18. Send Permission
	txq.job[jnum].send_permission = SOLUTION_PERMISSION_STATE_ENABLE;
#endif
}


/***********************************************************************
 *  Function: fsu_update_tx_time()
 *
 *  Description:
 *    update the tx time
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    long TimeToTx: time to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_tx_time (UINT16 jnum, long TimeToTx)
{
    txq.job[jnum].tx_time = TimeToTx;
}


/***********************************************************************
 *  Function: fsu_update_tx_total_page_no()
 *
 *  Description:
 *    update the tx total page no for UIManager
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_tx_total_page_no (UINT16 jnum, UINT8 TxTotalPageNo)
{
    txq.job[jnum].TxTotalPageNo = TxTotalPageNo;
}


/***********************************************************************
 *  Function: fsu_update_retry()
 *
 *  Description:
 *    update the retry
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 retry
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_retry (UINT16 jnum, UINT8 retry)
{
    txq.job[jnum].retry = retry;
}


/***********************************************************************
 *  Function: fsu_update_retry_tmr()
 *
 *  Description:
 *    update the retry_tmr
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 retry_tmr
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_retry_tmr (UINT16 jnum, UINT8 retry_tmr)
{
    txq.job[jnum].retry_tmr = retry_tmr;
}


/***********************************************************************
 *  Function: fsu_update_broadcast()
 *
 *  Description:
 *    update the broadcast
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 broadcast
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_broadcast (UINT16 jnum, UINT8 broadcast)
{
    txq.job[jnum].broadcast = broadcast;
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: fsu_update_send_permission()
 *
 *  Description:
 *    update the send_permission
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    UINT8 permission
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_send_permission (UINT16 jnum, UINT8 permission)
{
    txq.job[jnum].send_permission = permission;
}
#endif


/***********************************************************************
 *  Function: fsu_update_user_id()
 *
 *  Description:
 *    update the user id
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    char *pUserId: pointer to user id
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_user_id (UINT16 jnum, char *pUserId)
{
    UINT8 Len = 0;

    Len = strlen(pUserId);

    if (Len == 0)
    {
        txq.job[jnum].user_id[0] = 0x20; // Add one space
        txq.job[jnum].user_id[1] = 0;
    }
    else
    {
        snprintf(txq.job[jnum].user_id, sizeof(txq.job[jnum].user_id), "%s", pUserId);
    }
}


/***********************************************************************
 *  Function: fsu_update_faxScanInfo()
 *
 *  Description:
 *    update the faxScanInfo
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *    void *scanInfo: pointer to stIC_JobDoneInfo
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_faxScanInfo (UINT16 jnum, void *scanInfo)
{
	memcpy(&txq.job[jnum].faxScanInfo, scanInfo, sizeof(stIC_JobDoneInfo));
}


/***********************************************************************
 *  Function: fsu_update_gfaxScanInfo()
 *
 *  Description:
 *    update the g_faxScanInfo
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_gfaxScanInfo (UINT16 jnum)
{
	memcpy(&g_faxScanInfo, &txq.job[jnum].faxScanInfo, sizeof(stIC_JobDoneInfo));
}


/***********************************************************************
 *  Function: fsu_decrease_total_pages()
 *
 *  Description:
 *    decrease one from the total number of pages
 *
 *  Inputs:
 *    UINT16 jnum: job number to decrease
 *
 *  Output:
 *
 **********************************************************************/
void fsu_decrease_total_pages (UINT16 jnum)
{
    if (txq.job[jnum].TxRxTotalPages >= 1)
    {
        txq.job[jnum].TxRxTotalPages--;
    }
    else
    {
        /* should never get here! */
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_FAX_TOTAL_PAGE_SETTING);
    }
}


/***********************************************************************
 *  Function: fsu_decrease_retry()
 *
 *  Description:
 *    decrese one the current retry
 *
 *  Inputs:
 *    UINT16 jnum: job number to decrease
 *
 *  Output:
 *
 **********************************************************************/
void fsu_decrease_retry(UINT16 jnum)
{
    if (txq.job[jnum].retry >= 1)
    {
        txq.job[jnum].retry--;
    }
    else
    {
        /* should never get here! */
		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_FAX_RETRY_SETTING);
    }
}


/***********************************************************************
 *  Function: fsu_activate_mem_to_send()
 *
 *  Description:
 *    starting point of mem to send in the case of image backup
 *
 *  Inputs:
 *    UINT16 IndexToJobNo: index to valid job number referenced to
 *                         JOB_QUEUE_INFO_t
 *
 *  Output:
 *
 **********************************************************************/
void fsu_activate_mem_to_send(UINT16 IndexToJobNo)
{
    UINT8 retry = 0;
    UINT8 retry_tmr = 0;
    UINT8 IndexToDialNo = 0;
    UINT8 res = 0;
    UINT16 DocId = 0;
    UINT16 OriginalDocId = 0;
    char Tmp[LEN_PHONE_NO + 1] = {0, };
    char *pDelayTime = (char *)NULL;
    JOB_QUEUE_INFO_UI_t JobQueueInfoUI;
    TX_JOB_t *ptx_job = (TX_JOB_t *)NULL;
    UINT16 jnum = 0;

    JobQueueInfoUI.JobType = FSUJobQueueInfo.JobType[IndexToJobNo];

    // Step 1. Point to the next txq.nxt for the next job
	jnum = txq.nxt;

    set_txq_nxt ();
    
    JobQueueInfoUI.ID = jnum;

    ptx_job = &txq.job[jnum];

    // Step 2. Get Doc Id
    DocId = FSUJobQueueInfo.DocId[IndexToJobNo];

    // Step 3. Get Scan Resolution
    res = FSUJobQueueInfo.Resolution[IndexToJobNo];

    // Step 4. Get Auto. Redial Parameter from DB
    Fax_API_T30Attribute(GET, FXA_RETRY_NUMBER, &retry);
    Fax_API_T30Attribute(GET, FXA_RETRY_INTERVAL, &retry_tmr);

    // Step 5. Dial No.
    for (IndexToDialNo = 0; ; IndexToDialNo++)
    {
        if ( (FSUJobQueueInfo.DialNo[IndexToJobNo][IndexToDialNo] == 0x20) || (FSUJobQueueInfo.DialNo[IndexToJobNo][IndexToDialNo] == '\0') )
        {
            break;
        }
        else
        {
            ptx_job->fax_num[IndexToDialNo] = FSUJobQueueInfo.DialNo[IndexToJobNo][IndexToDialNo];
        }
    }

    ptx_job->fax_num[IndexToDialNo] = 0;

    snprintf(Tmp, sizeof(Tmp), "%s", FSUJobQueueInfo.DialNo[IndexToJobNo]);

    for (IndexToDialNo = 0; ; IndexToDialNo++)
    {
        if ( (Tmp[IndexToDialNo] == 0x20) || (Tmp[IndexToDialNo] == '\0') )
        {
            break;
        }
        else
        {
            JobQueueInfoUI.DialNo[IndexToDialNo] = Tmp[IndexToDialNo];
        }
    }

    JobQueueInfoUI.DialNo[IndexToDialNo] = 0;

    // Step 6. isDelay
    if (FSUJobQueueInfo.TimeDiff[IndexToJobNo] == TRANSMIT_TIME_RIGHT_NOW)
    {
        ptx_job->isDelay = FALSE;
    }
    else
    {
        ptx_job->isDelay = TRUE;
    }

    // Step 7. status
    ptx_job->status = FSU_STATE_SCANNING;

    // Step 8. retry
    ptx_job->retry = retry;

    // Step 9. retry_tmr
    ptx_job->retry_tmr = retry_tmr;

    // Step 10. tx_time
    if (ptx_job->isDelay == TRUE)
    {
        ptx_job->tx_time = fsu_get_current_system_time() + FSUJobQueueInfo.TimeDiff[IndexToJobNo];

        pDelayTime = fsu_request_get_time_to_tx_from_Job_Queue_File(DocId);

        snprintf(ptx_job->DelayTimeInText, sizeof(ptx_job->DelayTimeInText), "%s", pDelayTime);

        JobQueueInfoUI.TimeToTx = fsu_get_time_to_tx(FSUJobQueueInfo.DocId[IndexToJobNo]);
    }
    else
    {
        ptx_job->tx_time = FSUJobQueueInfo.TimeDiff[IndexToJobNo];

        ptx_job->DelayTimeInText[0] = 0x20; // Add one space
        ptx_job->DelayTimeInText[1] = 0;

        JobQueueInfoUI.TimeToTx = 0;
    }

    // Step 11. res
    ptx_job->res = res;

    // Step 12. width
    if (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_FORWARD)
    {
        ptx_job->width = fsu_get_width_info(res, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);
    }
    else
    {
        ptx_job->width = fsu_get_width_info(3, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);
    }

    // Step 13. ff_job
    if (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_FORWARD)
    {
        ptx_job->ff_job = 1;
    }
    else
    {
        ptx_job->ff_job = 0;
    }

    // Step 14. poll
    ptx_job->poll_transmit = 0;
    ptx_job->poll_receive = 0;

    // Step 15. broadcast
    if ( (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING) || (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_TX_PROGRESSING) )
    {
        ptx_job->broadcast = fsu_request_check_broadcast(DocId);
    }
    else
    {
        ptx_job->broadcast = 0;
    }
    
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
	ptx_job->send_permission = FSUJobQueueInfo.Permission[IndexToJobNo];
	fsu_update_file_name_tx(jnum, FSUJobQueueInfo.FileName[IndexToJobNo]);
	ptx_job->unique_job_id = FSUJobQueueInfo.UniqueJobId[IndexToJobNo];
#endif

    // Step 16. flag_manual_tx
    ptx_job->flag_manual_tx = 0;

    // Step 17. doc_id
    ptx_job->doc_id = DocId;

    // Step 18. original_doc_id
    OriginalDocId = ptx_job->original_doc_id = fsu_request_get_original_doc_id_from_Job_Queue_File(DocId);

    // Step 19. TxRxTotalPages
    if (fsu_get_broadcast(jnum) == 0)
    {
        if (FSUJobQueueInfo.PhotoMode[IndexToJobNo] == 1)
        {
            ptx_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(ptx_job->doc_id, FAX_FILE_TYPE_PHOTO);
        }
        else
        {
            ptx_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(ptx_job->doc_id, FAX_FILE_TYPE_JBG);
        }
    }
    else
    {
        if (DocId == OriginalDocId)
        {
            if (fsu_request_get_retransmit_from_Job_Queue_File(DocId) == 1)
            {
                ptx_job->TxRxTotalPages = fsu_request_get_total_page_no_from_Job_Queue_File(DocId);
            }
            else
            {
                ptx_job->TxRxTotalPages = 0; // This job will not start again...
            }
        }
        else
        {
            if (FSUJobQueueInfo.PhotoMode[IndexToJobNo] == 1)
            {
                ptx_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(ptx_job->doc_id, FAX_FILE_TYPE_PHOTO);
            }
            else
            {
                ptx_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(ptx_job->doc_id, FAX_FILE_TYPE_JBG);
            }
        }
    }

    // Step 20. TxTotalPageNo
    ptx_job->TxTotalPageNo = ptx_job->TxRxTotalPages;

    // Step 21. PrintReason
    ptx_job->PrintReason = PRINT_REASON_RESET;

    // Step 22. mem_time
    fsu_set_mem_time(jnum);

    // Step 23. Image Width
    fsu_update_image_width_pixel(jnum, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);

    // Step 24. Image Height
    fsu_update_image_height_pixel(jnum, FSUJobQueueInfo.ImageHeight[IndexToJobNo]);

    // Step 25. Photo Mode
    ptx_job->photo_mode = FSUJobQueueInfo.PhotoMode[IndexToJobNo];

	//Scan Info
    memcpy(&ptx_job->faxScanInfo, &FSUJobQueueInfo.ScanInfo[IndexToJobNo], sizeof(stIC_JobDoneInfo));

    // Step 26. user id
    fsu_update_user_id(jnum, gFaxUserId);

    // Step 27. Make /tmp/DocMan/DocXXX/ for image processing..
    fsu_make_doc_dir_in_ram(ptx_job->doc_id);

    // Step 28. Update BC Array for Broadcast
    if (ptx_job->broadcast == 1)
    {
        if ( (DocId == OriginalDocId) && (fsu_request_get_retransmit_from_Job_Queue_File(DocId) == 0) )
        {
            gFax_BC_Num[OriginalDocId].BC_Current_Num++;

            gFax_BC_Num[OriginalDocId].BC_Abnormal_Count++;

            gFaxBCAbnormalCnt++;
        }

        gFaxBroadcastJobInfo[OriginalDocId][gFax_BC_Num[OriginalDocId].BC_Total_Num] = IndexToJobNo;

        gFax_BC_Num[OriginalDocId].BC_Total_Num++;
    }

    // Step 29. Update RECD_FAXIMAGEBACKUP for GUI
    if ( (ptx_job->broadcast == 1) && ((DocId == OriginalDocId) && (fsu_request_get_retransmit_from_Job_Queue_File(DocId) == 0)) )
    {
        // No Action
    }
    else
    {
        faxapp_update_recd_fax_image_backup(&JobQueueInfoUI);
    }
}


/***********************************************************************
 *  Function: fsu_activate_mem_to_send_poll()
 *
 *  Description:
 *    starting point of mem to send poll in the case of image backup
 *
 *  Inputs:
 *    UINT16 IndexToJobNo: index to valid job number referenced to
 *                         JOB_QUEUE_INFO_t
 *
 *  Output:
 *
 **********************************************************************/
void fsu_activate_mem_to_send_poll(UINT16 IndexToJobNo)
{
    UINT8 PollTx = 0;
    UINT8 retry = 0;
    UINT8 retry_tmr = 0;
    UINT8 res = 0;
    UINT16 DocId = 0;
    JOB_QUEUE_INFO_UI_t JobQueueInfoUI;
    TX_JOB_t *ptx_job = (TX_JOB_t *)NULL;
    UINT16 jnum = 0;

    JobQueueInfoUI.JobType = FSUJobQueueInfo.JobType[IndexToJobNo];

    // Step 1. Point to the next txq.nxt for the next job
    jnum = txq.nxt;
    
    set_txq_nxt ();
    
    JobQueueInfoUI.ID = jnum;

    ptx_job = &txq.job[jnum];

    // Step 2. Get Doc Id
    DocId = FSUJobQueueInfo.DocId[IndexToJobNo];

    // Step 3. Get Scan Resolution
    res = FSUJobQueueInfo.Resolution[IndexToJobNo];

    // Step 4. Get Auto. Redial Parmaeter from DB
    Fax_API_T30Attribute(GET, FXA_RETRY_NUMBER, &retry);
    Fax_API_T30Attribute(GET, FXA_RETRY_INTERVAL, &retry_tmr);

    // Step 5. Dial No.
    ptx_job->fax_num[0] = 0;

    snprintf(JobQueueInfoUI.DialNo, sizeof(JobQueueInfoUI.DialNo), "%c", 0x20); // Add space...

    // Step 6. isDelay
    ptx_job->isDelay = FALSE;

    // Step 7. status
    ptx_job->status = FSU_STATE_SCANNING;

    // Step 8. retry
    ptx_job->retry = retry;

    // Step 9. retry_tmr
    ptx_job->retry_tmr = retry_tmr;

    // Step 10. tx_time
    ptx_job->tx_time = 0;

    ptx_job->DelayTimeInText[0] = 0x20; // Add one space
    ptx_job->DelayTimeInText[1] = 0;

    JobQueueInfoUI.TimeToTx = 0;

    // Step 11. res
    ptx_job->res = res;

    // Step 12. width
    ptx_job->width = fsu_get_width_info(3, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);

    // Step 13. ff_job
    ptx_job->ff_job = 0;

    // Step 14. poll
    ptx_job->poll_transmit = 1;
    ptx_job->poll_receive = 0;

    // Step 15. broadcast
    ptx_job->broadcast = 0;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
	ptx_job->send_permission = FSUJobQueueInfo.Permission[IndexToJobNo];
	fsu_update_file_name_tx(jnum, FSUJobQueueInfo.FileName[IndexToJobNo]);
	ptx_job->unique_job_id = FSUJobQueueInfo.UniqueJobId[IndexToJobNo];
#endif

    // Step 16. flag_manual_tx
    ptx_job->flag_manual_tx = 0;

    // Step 17. doc_id
    ptx_job->doc_id = DocId;

    // Step 18. original_doc_id
    ptx_job->original_doc_id = DEFAULT_ORIGINAL_DOC_ID;

    // Step 19. Update fsu_job_num_poll for FSU
    fsu_job_num_poll = IndexToJobNo;

    // Step 20. TxRxTotalPages
    if (FSUJobQueueInfo.PhotoMode[IndexToJobNo] == 1)
    {
        ptx_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(ptx_job->doc_id, FAX_FILE_TYPE_PHOTO);
    }
    else
    {
        ptx_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(ptx_job->doc_id, FAX_FILE_TYPE_JBG);
    }

    // Step 21. TxTotalPageNo
    ptx_job->TxTotalPageNo = ptx_job->TxRxTotalPages;

    // Step 22. PrintReason
    ptx_job->PrintReason = PRINT_REASON_RESET;

    // Step 23. mem_time
    fsu_set_mem_time(jnum);

    // Step 24. Image Width
    fsu_update_image_width_pixel(jnum, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);

    // Step 25. Image Height
    fsu_update_image_height_pixel(jnum, FSUJobQueueInfo.ImageHeight[IndexToJobNo]);

    // Step 26. Photo Mode
    ptx_job->photo_mode = FSUJobQueueInfo.PhotoMode[IndexToJobNo];

	//Scan Info
    memcpy(&ptx_job->faxScanInfo, &FSUJobQueueInfo.ScanInfo[IndexToJobNo], sizeof(stIC_JobDoneInfo));

    // Step 27. user id
    fsu_update_user_id(jnum, gFaxUserId);

    // Step 28. Set poll tx job...
    PollTx = TXPOLLFROM_MEMORY;
    Fax_API_T30Attribute(SET, FXA_POLLTXMODE, &PollTx);

    // Step 29. Make /tmp/DocMan/DocXXX/ for image processing..
    fsu_make_doc_dir_in_ram(ptx_job->doc_id);

    // Step 30. Update RECD_FAXIMAGEBACKUP for GUI
    faxapp_update_recd_fax_image_backup(&JobQueueInfoUI);
}


/***********************************************************************
 *  Function: fsu_activate_poll_receive()
 *
 *  Description:
 *    starting point of poll receive in the case of image backup
 *
 *  Inputs:
 *    UINT16 IndexToJobNo: index to valid job number referenced to
 *                         JOB_QUEUE_INFO_t
 *
 *  Output:
 *
 **********************************************************************/
void fsu_activate_poll_receive(UINT16 IndexToJobNo)
{
    UINT8 PollRx = 0;
    UINT8 retry = 0;
    UINT8 retry_tmr = 0;
    UINT8 res = 0;
    UINT8 IndexToDialNo = 0;
    UINT8 Len = 0;
    UINT16 DocId = 0;
    JOB_QUEUE_INFO_UI_t JobQueueInfoUI;
    TX_JOB_t *ptx_job = (TX_JOB_t *)NULL;
    UINT16 jnum = 0;

    JobQueueInfoUI.JobType = FSUJobQueueInfo.JobType[IndexToJobNo];

    // Step 1. Point to the next txq.nxt for the next job
    jnum = txq.nxt;

    set_txq_nxt ();
    
    JobQueueInfoUI.ID = jnum;

    ptx_job = &txq.job[jnum];

    // Step 2. Get Doc Id
    DocId = FSUJobQueueInfo.DocId[IndexToJobNo];

    // Step 3. Get Scan Resolution
    res = FSUJobQueueInfo.Resolution[IndexToJobNo];

    // Step 4. Get Auto. Redial Parmaeter from DB
    Fax_API_T30Attribute(GET, FXA_RETRY_NUMBER, &retry);
    Fax_API_T30Attribute(GET, FXA_RETRY_INTERVAL, &retry_tmr);

    // Step 5. Dial No.
    Len = strlen((char *)FSUJobQueueInfo.DialNo[IndexToJobNo]);

    for (IndexToDialNo = 0; IndexToDialNo < Len; IndexToDialNo++)
    {
        if (FSUJobQueueInfo.DialNo[IndexToJobNo][IndexToDialNo] == 0x20)
        {
            break;
        }
        else
        {
            ptx_job->fax_num[IndexToDialNo] = FSUJobQueueInfo.DialNo[IndexToJobNo][IndexToDialNo];

            JobQueueInfoUI.DialNo[IndexToDialNo] = FSUJobQueueInfo.DialNo[IndexToJobNo][IndexToDialNo];
        }
    }

    ptx_job->fax_num[IndexToDialNo] = 0;

    JobQueueInfoUI.DialNo[IndexToDialNo] = 0;

    // Step 6. isDelay
    ptx_job->isDelay = FALSE;

    // Step 7. status
    ptx_job->status = FSU_STATE_SCANNING;

    // Step 8. retry
    ptx_job->retry = retry;

    // Step 9. retry_tmr
    ptx_job->retry_tmr = retry_tmr;

    // Step 10. tx_time
    ptx_job->tx_time = 0;

    ptx_job->DelayTimeInText[0] = 0x20; // Add one space
    ptx_job->DelayTimeInText[1] = 0;

    JobQueueInfoUI.TimeToTx = 0;

    // Step 11. res
    ptx_job->res = res;

    // Step 12. width
    ptx_job->width = fsu_get_width_info(3, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);

    // Step 13. ff_job
    ptx_job->ff_job = 0;

    // Step 14. poll
    ptx_job->poll_transmit = 0;
    ptx_job->poll_receive = 1;

    // Step 15. broadcast
    ptx_job->broadcast = 0;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
	ptx_job->send_permission = FSUJobQueueInfo.Permission[IndexToJobNo];
	fsu_update_file_name_tx(jnum, FSUJobQueueInfo.FileName[IndexToJobNo]);
	ptx_job->unique_job_id = FSUJobQueueInfo.UniqueJobId[IndexToJobNo];
#endif

    // Step 16. flag_manual_tx
    ptx_job->flag_manual_tx = 0;

    // Step 17. doc_id
    ptx_job->doc_id = DocId;

    // Step 18. original_doc_id
    ptx_job->original_doc_id = DEFAULT_ORIGINAL_DOC_ID;

    // Step 19. Update fsu_job_num_poll for FSU
    fsu_job_num_poll = jnum;

    // Step 20. TxRxTotalPages
    if (FSUJobQueueInfo.PhotoMode[IndexToJobNo] == 1)
    {
        ptx_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(ptx_job->doc_id, FAX_FILE_TYPE_PHOTO);
    }
    else
    {
        ptx_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(ptx_job->doc_id, FAX_FILE_TYPE_JBG);
    }

    // Step 21. TxTotalPageNo
    ptx_job->TxTotalPageNo = ptx_job->TxRxTotalPages;

    // Step 22. PrintReason
    ptx_job->PrintReason = PRINT_REASON_RESET;

    // Step 23. mem_time
    fsu_set_mem_time(jnum);

    // Step 24. Image Width
    fsu_update_image_width_pixel(jnum, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);

    // Step 25. Image Height
    fsu_update_image_height_pixel(jnum, FSUJobQueueInfo.ImageHeight[IndexToJobNo]);

    // Step 26. Photo Mode
    ptx_job->photo_mode = FSUJobQueueInfo.PhotoMode[IndexToJobNo];

    // Step 27. user id
    ptx_job->user_id[0] = 0x20; // Add one space
    ptx_job->user_id[1] = 0;

    // Step 28. Set poll tx job...
    PollRx = RX_POLL;
    Fax_API_T30Attribute(SET, FXA_POLLRXMODE, &PollRx);

    // Step 29. Make /tmp/DocMan/DocXXX/ for image processing..
    fsu_make_doc_dir_in_ram(ptx_job->doc_id);

    // Step 30. Update RECD_FAXIMAGEBACKUP for GUI
    faxapp_update_recd_fax_image_backup(&JobQueueInfoUI);
}


/***********************************************************************
 *  Function: fsu_activate_mem_to_prn()
 *
 *  Description:
 *    starting point of mem to print in the case of image backup
 *
 *  Inputs:
 *    UINT16 IndexToJobNo: index to valid job number referenced to
 *                         JOB_QUEUE_INFO_t
 *
 *  Output:
 *
 **********************************************************************/
void fsu_activate_mem_to_prn(UINT16 IndexToJobNo)
{
    UINT8 retry = 0;
    UINT8 isFaxForwardOn = DISABLE;
    UINT8 FaxForwardNo[LEN_PHONE_NO + 1];
    UINT8 ForwardNo[LEN_PHONE_NO + 1];
    UINT8 retry_tmr = 0;
    UINT8 res = 0;
    UINT8 Len = 0;
    UINT8 IndexToDialNo = 0;
    UINT16 DocId = 0;
    JOB_QUEUE_INFO_UI_t JobQueueInfoUI;
    TX_JOB_t *p_rcv_job = (TX_JOB_t *)NULL;
    UINT16 jnum = 0;

    JobQueueInfoUI.JobType = FSUJobQueueInfo.JobType[IndexToJobNo];

    // Step 1. Point to the next txq.nxt for the next job
    jnum = txq.nxt;

    set_txq_nxt ();

    JobQueueInfoUI.ID = jnum;

    p_rcv_job = &txq.job[jnum];

    // Step 2. Get Doc Id
    DocId = FSUJobQueueInfo.DocId[IndexToJobNo];

    // Step 3. Get Resolution
    res = FSUJobQueueInfo.Resolution[IndexToJobNo];

    // Step 4. Get Auto. Redial Parmaeter from DB
    Fax_API_T30Attribute(GET, FXA_RETRY_NUMBER, &retry);
    Fax_API_T30Attribute(GET, FXA_RETRY_INTERVAL, &retry_tmr);

    // Step 5. Dial No.
    p_rcv_job->fax_num[0] = 0;

    // Step 6. isDelay
    p_rcv_job->isDelay = FALSE;

    // Step 7. status
    p_rcv_job->status = FSU_STATE_RECEIVE_DONE;

    // Step 8. retry
    p_rcv_job->retry = retry;

    // Step 9. retry_tmr
    p_rcv_job->retry_tmr = retry_tmr;

    // Step 10. tx_time
    p_rcv_job->tx_time = TRANSMIT_TIME_RIGHT_NOW;

    JobQueueInfoUI.TimeToTx = 0;

    // Step 11. res
    p_rcv_job->res = res;

    // Step 12. width
    p_rcv_job->width = fsu_get_width_info(res, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);

    // Step 13. ff_job
    Fax_API_T30Attribute(GET, FXA_FAXFORWARD, &isFaxForwardOn);

    if (isFaxForwardOn == ENABLE)
    {
        p_rcv_job->ff_job = 1;

        Fax_API_T30Attribute(GET, FXA_FAXFORWARD_NO, FaxForwardNo);

        Len = strlen((char *)FaxForwardNo);

        for (IndexToDialNo = 0; IndexToDialNo < Len; IndexToDialNo++)
        {
            if (FaxForwardNo[IndexToDialNo] == 0x2D) // '-' key
            {
                ForwardNo[IndexToDialNo] = 0x2C; // Pause Key
            }
            else
            {
                ForwardNo[IndexToDialNo] = FaxForwardNo[IndexToDialNo];
            }

            JobQueueInfoUI.DialNo[IndexToDialNo] = FaxForwardNo[IndexToDialNo];
        }

        ForwardNo[IndexToDialNo] = 0;

        memcpy(p_rcv_job->fax_num, ForwardNo, sizeof(ForwardNo));

        JobQueueInfoUI.DialNo[IndexToDialNo] = 0;
    }
    else
    {
        p_rcv_job->ff_job = 0;

        snprintf(JobQueueInfoUI.DialNo, sizeof(JobQueueInfoUI.DialNo), "%c", 0x20); // Add space...
    }

    // Step 14. poll
    p_rcv_job->poll_transmit = 0;
    p_rcv_job->poll_receive = 0;

    // Step 15. broadcast
    p_rcv_job->broadcast = 0;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
	p_rcv_job->send_permission = FSUJobQueueInfo.Permission[IndexToJobNo];
	fsu_update_file_name_tx(jnum, FSUJobQueueInfo.FileName[IndexToJobNo]);
	p_rcv_job->unique_job_id = FSUJobQueueInfo.UniqueJobId[IndexToJobNo];
#endif

    // Step 16. flag_manual_tx
    p_rcv_job->flag_manual_tx = 0;

    // Step 17. doc_id
    p_rcv_job->doc_id = DocId;

    // Step 18. original_doc_id
    p_rcv_job->original_doc_id = DEFAULT_ORIGINAL_DOC_ID;

    // Step 19. TxRxTotalPages
    if (FSUJobQueueInfo.PhotoMode[IndexToJobNo] == 1)
    {
        p_rcv_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(p_rcv_job->doc_id, FAX_FILE_TYPE_PHOTO);
    }
    else
    {
        p_rcv_job->TxRxTotalPages = fsu_get_total_page_no_in_mem(p_rcv_job->doc_id, FAX_FILE_TYPE_JBG);
    }

    // Step 20. TxTotalPageNo
    p_rcv_job->TxTotalPageNo = p_rcv_job->TxRxTotalPages;

    // Step 21. PrintReason
    p_rcv_job->PrintReason = PRINT_REASON_RESET;

    // Step 22. mem_time
    fsu_set_mem_time(jnum);

    // Step 23. Image Width
    fsu_update_image_width_pixel(jnum, FSUJobQueueInfo.ImageWidth[IndexToJobNo]);

    // Step 24. Image Height
    fsu_update_image_height_pixel(jnum, FSUJobQueueInfo.ImageHeight[IndexToJobNo]);

    // Step 25. Photo Mode
    p_rcv_job->photo_mode = FSUJobQueueInfo.PhotoMode[IndexToJobNo];

    // Step 26. user id
    p_rcv_job->user_id[0] = 0x20; // Add one space
    p_rcv_job->user_id[1] = 0;

    // Step 27. Make /tmp/DocMan/DocXXX/ for image processing..
    fsu_make_doc_dir_in_ram(p_rcv_job->doc_id);

    if (p_rcv_job->ff_job == 1)
    {
        // Step 28. Update RECD_FAXIMAGEBACKUP for GUI
        faxapp_update_recd_fax_image_backup(&JobQueueInfoUI);
    }
    else
    {
        if (getSaveFaxFile() == ENABLE)
        {
            UINT8 image_width = p_rcv_job->width;
            char targetPathName[MAX_LEN_FILE_PATH_NAME] = {0, };

            snprintf(targetPathName, sizeof(targetPathName), "%s%cImgDoc%04X", pRootFolderNameSolDocMan, PATH_SEPARATOR, IndexToJobNo);

            printf("\n(YHCHO) %s() - targetPathName: %s\n", __FUNCTION__, targetPathName);

            fsu_update_status(jnum, FSU_STATE_SENDING_PENDING);

            fsu_make_dir_on_file_system(targetPathName);

            if (FileFormat == SAVE_FILE_FORMAT_PDF)
            {
                if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
                {
                    faxapp_make_pdf_start(targetPathName, jnum, fsu_get_start_page_no_in_mem(p_rcv_job->doc_id, FAX_FILE_TYPE_JBG), p_rcv_job->TxRxTotalPages, image_width, PageType, REQUEST_SOURCE_FAX, FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION);
                }
                else
                {
                    UINT8 IndexToPageNo = 0;
                    UINT8 TotalPageNo = p_rcv_job->TxRxTotalPages;

                    for (IndexToPageNo = 1; IndexToPageNo < TotalPageNo + 1; IndexToPageNo++)
                    {
                        faxapp_make_pdf_start(targetPathName, jnum, IndexToPageNo, 1, image_width, PageType, REQUEST_SOURCE_FAX, FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION);

                        tx_thread_sleep(1000); // To distinguish the output file name...
                    }
                }

                faxapp_solution_request_send(jnum);
            }
            else
            {
                if (PageType == SAVE_FILE_PAGE_TYPE_SINGLE_PAGE)
                {
                    faxapp_make_tif_start(targetPathName, jnum, fsu_get_start_page_no_in_mem(p_rcv_job->doc_id, FAX_FILE_TYPE_JBG), p_rcv_job->TxRxTotalPages, image_width, PageType);
                }
                else
                {
                    UINT8 IndexToPageNo = 0;
                    UINT8 TotalPageNo = p_rcv_job->TxRxTotalPages;

                    for (IndexToPageNo = 1; IndexToPageNo < TotalPageNo + 1; IndexToPageNo++)
                    {
                        faxapp_make_tif_start(targetPathName, jnum, IndexToPageNo, 1, image_width, PageType);

                        tx_thread_sleep(1000); // To distinguish the output file name...
                    }
                }

                faxapp_solution_request_send(jnum);
            }
        }
        else
        {
            // Step 28. Update RECD_FAXIMAGEBACKUP for GUI
            faxapp_update_recd_fax_image_backup(&JobQueueInfoUI);
        }
    }
}


/***********************************************************************
 *  Function: fsu_get_job_start_time()
 *
 *  Description:
 *    get the job start time
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_job_start_time (long CurrentTimeInSec)
{
    fsu_update_current_time_for_report(CurrentTimeInSec);
}


/***********************************************************************
 *  Function: fsu_get_job_type()
 *
 *  Description:
 *    get the job type
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_job_type (UINT8 JobType, UINT16 jnum)
{
    fsu_update_job_type_for_report(JobType, jnum);
}


/***********************************************************************
 *  Function: fsu_get_ecm()
 *
 *  Description:
 *    get the ECM
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_ecm (UINT8 ECM)
{
    fsu_update_ecm_for_report(ECM);
}


/***********************************************************************
 *  Function: fsu_get_res()
 *
 *  Description:
 *    get the resolution
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_res (UINT8 Resolution)
{
    fsu_update_res_for_report(Resolution);
}


/***********************************************************************
 *  Function: fsu_get_remote_no()
 *
 *  Description:
 *    get the remote no
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_remote_no (UINT8 FSUJobType, UINT16 jnum)
{
    if (FSUJobType == FSU_JOB_TYPE_FOR_DIAL_TYPE_TRANSMIT || FSUJobType == FSU_JOB_TYPE_FOR_DIAL_TYPE_ON_HOOK) // Tx (immediate transmit, delay transmit, broadcast, fax forward, and on-hook)
    {
        fsu_update_remote_no_for_report(&txq.job[jnum].fax_num[0]);
    }
    else // Poll Tx or Receive..
    {
        UINT8 Len = 0;

        Len = strlen(gcCID_for_RemoteFaxNum);

        if (Len == 0)
        {
            if (FSUJobType == FSU_JOB_TYPE_FOR_DIAL_TYPE_POLL_TRANSMIT)
            {
                UINT8 Len = 0;

                Len = strlen(gcCIG_for_RemoteFaxNum);

                if (Len == 0)
                {
                    snprintf(gcCIG_for_RemoteFaxNum, sizeof(gcCIG_for_RemoteFaxNum), "%c", 0x20); // Add space...
                }

                fsu_update_remote_no_for_report((UINT8 *)gcCIG_for_RemoteFaxNum);
            }
            else
            {
                UINT8 Len = 0;

                Len = strlen(gcTSI_for_RemoteFaxNum);

                if (Len == 0)
                {
                    snprintf(gcTSI_for_RemoteFaxNum, sizeof(gcTSI_for_RemoteFaxNum), "%c", 0x20); // Add space...
                }

                fsu_update_remote_no_for_report((UINT8 *)gcTSI_for_RemoteFaxNum);
            }
        }
        else
        {
            fsu_update_remote_no_for_report((UINT8 *)gcCID_for_RemoteFaxNum);
        }
    }
}


/***********************************************************************
 *  Function: fsu_get_duration()
 *
 *  Description:
 *    get the duration
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_duration (void)
{
    fsu_update_duration_for_report(gFaxJobEndTimeForDuration, gFaxJobStartTimeForDuration);
}


/***********************************************************************
 *  Function: fsu_get_page_no()
 *
 *  Description:
 *    get the page no
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_page_no (UINT8 PageNo)
{
    fsu_update_page_no_for_report(PageNo);
}


/***********************************************************************
 *  Function: fsu_get_connected_speed()
 *
 *  Description:
 *    get the result
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_connected_speed (UINT8 Speed)
{
    fsu_update_connected_speed_for_report(Speed);
}


/***********************************************************************
 *  Function: fsu_get_result()
 *
 *  Description:
 *    get the result
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_result (UINT16 T30Result)
{
    fsu_update_result_for_report(T30Result);
}


/***********************************************************************
 *  Function: fsu_get_modulation_name()
 *
 *  Description:
 *    get the modulation name
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_get_modulation_name (UINT8 Modulation)
{
    fsu_update_modulation_name_for_report(Modulation);
}


/***********************************************************************
 *  Function: fsu_update_current_time_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    long CurrentTimeInSec: current system time in second
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_current_time_for_report (long CurrentTimeInSec)
{
    UINT8 Type = 0;
    time_t time;
    struct tm *tm_ptr;
    char JobStartTime[MAX_LEN_DATE_TIME] = {0, };

    Report.DateTime = CurrentTimeInSec;

    time = (time_t)CurrentTimeInSec;

    tm_ptr = localtime(&time);

    if(tm_ptr == NULL)
    {
        snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d AM %02d:%02d", 1, 1, 0, 0);
    }
    else
    {
        Type = getCountryDateType();

        switch (Type)
        {
            case FAX_COUNTRY_DATE_TYPE_YMD:
            case FAX_COUNTRY_DATE_TYPE_MDY:
            {
                if (tm_ptr->tm_hour == 0)
                {
                    snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d AM 12:%02d", tm_ptr->tm_mon + 1, tm_ptr->tm_mday, tm_ptr->tm_min);
                }
                else if ((tm_ptr->tm_hour >= 1) && (tm_ptr->tm_hour <= 11))
                {
                    snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d AM %02d:%02d", tm_ptr->tm_mon + 1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min);
                }
                else if (tm_ptr->tm_hour == 12)
                {
                    snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d PM 12:%02d", tm_ptr->tm_mon + 1, tm_ptr->tm_mday, tm_ptr->tm_min);
                }
                else
                {
                    snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d PM %02d:%02d", tm_ptr->tm_mon + 1, tm_ptr->tm_mday, tm_ptr->tm_hour - 12, tm_ptr->tm_min);
                }
                break;
            }

            case FAX_COUNTRY_DATE_TYPE_DMY:
            {
                if (tm_ptr->tm_hour == 0)
                {
                    snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d AM 12:%02d", tm_ptr->tm_mday, tm_ptr->tm_mon + 1, tm_ptr->tm_min);
                }
                else if ((tm_ptr->tm_hour >= 1) && (tm_ptr->tm_hour <= 11))
                {
                    snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d AM %02d:%02d", tm_ptr->tm_mday, tm_ptr->tm_mon + 1, tm_ptr->tm_hour, tm_ptr->tm_min);
                }
                else if (tm_ptr->tm_hour == 12)
                {
                    snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d PM 12:%02d", tm_ptr->tm_mday, tm_ptr->tm_mon + 1, tm_ptr->tm_min);
                }
                else
                {
                    snprintf(JobStartTime, sizeof(JobStartTime), "%02d-%02d PM %02d:%02d", tm_ptr->tm_mday, tm_ptr->tm_mon + 1, tm_ptr->tm_hour - 12, tm_ptr->tm_min);
                }
                break;
            }

            default:
                break;
        }
    }

    snprintf(FaxJobLog.DateTime, sizeof(FaxJobLog.DateTime), "%s", JobStartTime);
}


/***********************************************************************
 *  Function: fsu_update_job_type_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    UINT8 JobType: JobType
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_job_type_for_report (UINT8 JobType, UINT16 jnum)
{
    Report.JobType = JobType;

    if (fsu_get_broadcast(jnum) == 1)
    {
        FaxJobLog.JobType = JOB_TYPE_FOR_REPORT_BROADCAST;
    }
    else if (fsu_get_is_delay(jnum) == 1)
    {
        FaxJobLog.JobType = JOB_TYPE_FOR_REPORT_DELAY;
    }
    else
    {
        FaxJobLog.JobType = JobType;
    }
}


/***********************************************************************
 *  Function: fsu_update_ecm_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_ecm_for_report (UINT8 ECM)
{
    Report.ECM = ECM;
}


/***********************************************************************
 *  Function: fsu_update_res_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_res_for_report (UINT8 Resolution)
{
    Report.Resolution = Resolution;
}


/***********************************************************************
 *  Function: fsu_update_remote_no_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    UINT8 *DialNo: pointer to dial no.
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_remote_no_for_report (UINT8 *DialNo)
{
    UINT8 Len = 0;
    UINT8 Index = 0;
    char DialNo4GUI[LEN_PHONE_NO + 1] = {0, };
    char Temp_DialNo4GUI[LEN_PHONE_NO + 1] = {0, };

    snprintf(Temp_DialNo4GUI, sizeof(Temp_DialNo4GUI), "%s", DialNo);

    Len = strlen(Temp_DialNo4GUI);

    for (Index = 0; Index < Len; Index++)
    {
        if (Temp_DialNo4GUI[Index] == 0x2C)
        {
            DialNo4GUI[Index] = 0x2D;
        }
        else
        {
            DialNo4GUI[Index] = Temp_DialNo4GUI[Index];
        }
    }

    DialNo4GUI[Index] = 0;

    snprintf(Report.RemoteNo, sizeof(Report.RemoteNo), "%s", DialNo4GUI);

    snprintf(FaxJobLog.RemoteNo, sizeof(FaxJobLog.RemoteNo), "%s", DialNo4GUI);
}


/***********************************************************************
 *  Function: fsu_update_duration_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    long EndTime: end time
 *    long StartTime: start time
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_duration_for_report (long EndTime, long StartTime)
{
    if ( (StartTime > EndTime) || ((EndTime - StartTime) >= MAX_DURATION_IN_SECOND) )
	{
		Report.Duration = 0;
	}
	else
	{
		Report.Duration = (EndTime - StartTime);
	}
}


/***********************************************************************
 *  Function: fsu_update_page_no_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    UINT8 PageNo
 *
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_page_no_for_report (UINT8 PageNo)
{
    Report.PageNo = FaxJobLog.TotalPageNo = PageNo;
}


/***********************************************************************
 *  Function: fsu_update_connected_speed_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    UINT8 ConnectedSpeed: Connected Speed
 *
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_connected_speed_for_report (UINT8 ConnectedSpeed)
{
    Report.ConSpeed = ConnectedSpeed;
}


/***********************************************************************
 *  Function: fsu_update_result_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    UINT8 T30Result: Result
 *
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_result_for_report (UINT16 T30Result)
{
    Report.Result = T30Result;

    switch (T30Result)
    {
        case FAX_ERROR_NO_ERROR:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "OK");
            break;

        case FAX_ERROR_NO_CONNECT_ERROR:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "No Answer");
            break;

        case FAX_ERROR_BUSY_TONE_ERROR:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "Line Busy");
            break;

        case FAX_ERROR_PHONE_LINE_ERROR:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "Line Error");
            break;

        case FAX_ERROR_NO_MEM_ERROR:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "Memory Full");
            break;

        case FAX_ERROR_USER_ABORT_ERROR:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "Cancelled");
            break;

        case FAX_ERROR_DOCUMENT_JAM:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "Document Jam");
            break;

        case FAX_ERROR_RECEIVE_SPAM_FAX:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "Spam Fax");
            break;

        default:
            snprintf(FaxJobLog.Result, sizeof(FaxJobLog.Result), "ERROR-#%03d", T30Result);
            break;
    }
}


/***********************************************************************
 *  Function: fsu_update_modulation_name_for_report()
 *
 *  Description:
 *    update the struct INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    UINT8 Modulation: Modulation
 *
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_modulation_name_for_report (UINT8 Modulation)
{
    Report.ModulationName = Modulation;
}


/***********************************************************************
 *  Function: fsu_update_delay_time_for_job_log()
 *
 *  Description:
 *    update the struct INFO_FOR_FAX_JOB_LOG_t (DelayTime) for report
 *
 *  Inputs:
 *    UINT8 isDelay: determine which information to save
 *    UINT16 jnum: job number to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_delay_time_for_job_log (UINT8 isDelay, UINT16 jnum)
{
    if (isDelay == 1)
    {
        snprintf(FaxJobLog.DelayTime, sizeof(FaxJobLog.DelayTime), "%s", fsu_get_delay_time_in_text(jnum));
    }
    else
    {
        snprintf(FaxJobLog.DelayTime, sizeof(FaxJobLog.DelayTime), "%c", 0x20);
    }
}


/***********************************************************************
 *  Function: fsu_update_user_id_job_log()
 *
 *  Description:
 *    update the struct INFO_FOR_FAX_JOB_LOG_t (UserId) for report
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_user_id_job_log (UINT16 jnum)
{
    snprintf(FaxJobLog.UserId, sizeof(FaxJobLog.UserId), "%s", fsu_get_user_id(jnum));
}


/***********************************************************************
 *  Function: fsu_update_remote_no_for_image_log()
 *
 *  Description:
 *    call the FaxSetPhoneNumber to save the remote no. for image log
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_remote_no_for_image_log (UINT8 type, UINT16 jnum)
{
    UINT8 Len = 0;
    UINT8 Index = 0;
    UINT8 TotalRecipient = faxapp_get_no_of_recipient();
    char remote_num[LEN_PHONE_NO + 1] = {0, };

    if ( (type == IMAGE_LOG_JOB_TYPE_TX) && (TotalRecipient >= 2) )
    {
        UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);

        Len = strlen((char *)txq.job[gFaxBroadcastJobInfo[OriginalDocId][0]].fax_num);

        for (Index = 0; Index < Len; Index++)
        {
            remote_num[Index] = txq.job[gFaxBroadcastJobInfo[OriginalDocId][0]].fax_num[Index];
        }

        remote_num[Index] = 0;

        FaxSetPhoneNumber(type, remote_num);
    }
    else
    {
        Len = strlen((char *)txq.job[jnum].fax_num);

        for (Index = 0; Index < Len; Index++)
        {
            remote_num[Index] = txq.job[jnum].fax_num[Index];
        }

        remote_num[Index] = 0;

        FaxSetPhoneNumber(type, remote_num);
    }

    printf("\n(YHCHO) %s() - remote_num: %s\n", __FUNCTION__, remote_num);
}

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: fsu_update_remote_no_for_solution()
 *
 *  Description:
 *    call the FaxSetPhoneNumber to save the remote no. for solution
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_remote_no_for_solution (UINT8 type, UINT16 jnum)
{
    UINT8 Len = 0;
    UINT8 Index = 0;
    char remote_num[LEN_PHONE_NO + 1] = {0, };

	Len = strlen((char *)txq.job[jnum].fax_num);

	for (Index = 0; Index < Len; Index++)
	{
		if(txq.job[jnum].fax_num[Index] == 0x2C)
		{
			remote_num[Index] = 0x2D;
		}
		else
		{
			remote_num[Index] = txq.job[jnum].fax_num[Index];
		}
	}

	remote_num[Index] = 0;

	FaxSetPhoneNumber(type, remote_num);

    printf("\n(YHCHO) %s() - remote_num: %s\n", __FUNCTION__, remote_num);
}


/***********************************************************************
 *  Function: fsu_update_file_name_for_solution()
 *
 *  Description:
 *    call the FaxSetFileName to save the file name for solution
 *
 *  Inputs:
 *    UINT16 jnum: job number to update
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_file_name_for_solution (UINT16 jnum, int last_job, int solution_result)
{
    UINT8 Len = 0;
    UINT8 Index = 0;
    char file_name[MAX_LEN_FILE_PATH_NAME + 1] = {0, };

	if(solution_result)
	{
		Len = strlen((char *)nameShareMem);

		for (Index = 0; Index < Len; Index++)
		{
			file_name[Index] = nameShareMem[Index];
		}
	}
	else
	{
		Len = strlen((char *)txq.job[jnum].file_name_tx);

		for (Index = 0; Index < Len; Index++)
		{
			file_name[Index] = txq.job[jnum].file_name_tx[Index];
		}
	}

	file_name[Index] = 0;

    printf("\n(YHCHO) %s() - file_name: %s\n", __FUNCTION__, file_name);
    printf("\n(YHCHO) %s() - unique_job_id: %ld\n", __FUNCTION__, fsu_get_unique_job_id(jnum));
	printf("\n(YHCHO) %s() - last_job: %d\n", __FUNCTION__, last_job);

	//FaxSetFileName(file_name);
	FaxSetJobLog(file_name, fsu_get_unique_job_id(jnum), last_job);
}
#endif


/***********************************************************************
 *  Function: fsu_convert_connected_speed()
 *
 *  Description:
 *    convert the connected for OP
 *
 *  Inputs:
 *    UINT8 Speed: Connected speed in bps
 *
 *  Output:
 *
 **********************************************************************/
UINT8 fsu_convert_connected_speed (UINT8 Speed)
{
    UINT8 ret_val = 0;

    switch (Speed)
    {
        case 2:
            ret_val = 0; // 2,400bps
            break;

        case 4:
            ret_val = 1; // 4,800bps
            break;

        case 7:
            ret_val = 2; // 7,200bps
            break;

        case 9:
            ret_val = 3; // 9,600bps
            break;

        case 12:
            ret_val = 4; // 12,000bps
            break;

        case 14:
            ret_val = 5; // 14,400bps
            break;

        case 16:
            ret_val = 6; // 16,800bps
            break;

        case 19:
            ret_val = 7; // 19,200bps
            break;

        case 21:
            ret_val = 8; // 21,600bps
            break;

        case 24:
            ret_val = 9; // 24,000bps
            break;

        case 26:
            ret_val = 10; // 26,400bps
            break;

        case 28:
            ret_val = 11; // 28,800bps
            break;

        case 31:
            ret_val = 12; // 31,200bps
            break;

        case 33:
            ret_val = 13; // 33,600bps
            break;

        default:
            break;
    }

    return ret_val;
}


/******************************************************************************************
 *  Function: fsu_write_report_info_to_db()
 *
 *  Description:
 *    update the RECD_FAXLOG / RECD_FAXSPLOG using INFO_FOR_REPORT_t for report
 *
 *  Inputs:
 *    UINT8 reason: write report info into DB (RECD_FAXLOG or RECD_FAXSPLOG)
 *
 *  Output:
 *
 ******************************************************************************************/
void fsu_write_report_info_to_db (UINT8 reason)
{
    if (reason == FAX_REPORT_TCR)
    {
        db_set_recd_fax_log(Report.DateTime, Report.JobType, Report.ECM, Report.Resolution, Report.RemoteNo, Report.Duration, Report.PageNo, Report.ConSpeed, Report.Result, Report.ModulationName);
    }
    else
    {
        db_set_recd_fax_sp_log(Report.DateTime, Report.JobType, Report.ECM, Report.Resolution, Report.RemoteNo, Report.Duration, Report.PageNo, Report.ConSpeed, Report.Result, Report.ModulationName);
    }
}


/********************************************************************************************************
 *  Function: fsu_delete_report_info_from_db()
 *
 *  Description:
 *    delete the oldest record of RECD_FAXLOG / RECD_FAXSPLOG if it has more than 35 / 700 records, respectively
 *
 *  Inputs:
 *
 *  Output:
 *
 ********************************************************************************************************/
void fsu_delete_report_record_from_db (UINT8 Reason)
{
    int No = 0;

    No = faxapp_get_data_count(Reason);

    //printf("\n(YHCHO) %s() - Reason: %d", __FUNCTION__, Reason);
    //printf("\n(YHCHO) %s() - No: %d\n", __FUNCTION__, No);

    if (Reason == FAX_REPORT_TCR)
    {
        if (No > faxapp_get_max_record_no_RECD_FAXLOG()) // if more than 35 records...
        {
            faxapp_delete_record_var(Reason);
        }
    }
    else
    {
        if (No > faxapp_get_max_record_no_RECD_FAXSPLOG()) // if more than 700 records...
        {
            faxapp_delete_record_var(Reason);
        }
    }
}


/***********************************************************************
 *  Function: fsu_set_speaker_volume()
 *
 *  Description:
 *    set T.30 attribute related with speaker volume
 *
 *  Inputs:
 *    int Volume: 0 ~ 7, 0: OFF
 *
 *  Output:
 *
 **********************************************************************/
void fsu_set_speaker_volume (int Volume)
{
    UINT8 SpeakerVolume = 0;

    SpeakerVolume = (UINT8)Volume;

    Fax_API_T30Attribute(SET, FXA_SPEAKERVOLUME, &SpeakerVolume);
}


/***********************************************************************
 *  Function: fsu_get_speaker_volume()
 *
 *  Description:
 *    get T.30 attribute related with speaker volume
 *
 *  Inputs:
 *    int Volume: 0 ~ 7, 0: OFF
 *
 *  Output:
 *
 **********************************************************************/
UINT8 fsu_get_speaker_volume (void)
{
    UINT8 SpeakerVolume = 0;

    Fax_API_T30Attribute(GET, FXA_SPEAKERVOLUME, &SpeakerVolume);

    return SpeakerVolume;
}


/***********************************************************************
 *  Function: fsu_make_image_file_for_broadcast()
 *
 *  Description:
 *    Make 0-sized image file in /DocMan/DocXXXX especially for broadcast
 *
 *  Inputs:
 *    UINT16 SrcJobNo: Job No. of source
 *    UINT16 DstJobNo: Job No. of destination
 *
 *  Output:
 *
 **********************************************************************/
void fsu_make_image_file_for_broadcast (UINT16 SrcJobNo, UINT16 DstJobNo, UINT8 Page)
{
    char textSrcPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char textDstPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char photoSrcPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char photoDstPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    UINT8 IndexToPageNo = 0;
    UINT8 isPhotoMode = 0;
    UINT16 srcDocId = fsu_get_doc_id(SrcJobNo);
    UINT16 dstDocId = fsu_get_doc_id(DstJobNo);
    FILE *textDst = (FILE *)NULL;
    FILE *photoDst = (FILE *)NULL;

    isPhotoMode = fsu_get_photo_mode(SrcJobNo);

    for (IndexToPageNo = 1; IndexToPageNo < Page + 1; IndexToPageNo++)
    {
        memset(textSrcPathName, 0x00, sizeof(textSrcPathName));

        snprintf(textSrcPathName, sizeof(textSrcPathName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, srcDocId, PATH_SEPARATOR, IndexToPageNo);

        if (access(textSrcPathName, F_OK) != 0)
        {
            XASSERT(0, 0);
        }
        else
        {
            memset(textDstPathName, 0x00, sizeof(textDstPathName));

            snprintf(textDstPathName, sizeof(textDstPathName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, dstDocId, PATH_SEPARATOR, IndexToPageNo);

            textDst = fopen(textDstPathName, "wb");

            if (textDst == (FILE *)NULL)
            {
                XASSERT(0,0);
            }
            else
            {
                if (fclose(textDst) != 0)
                {
                    XASSERT(0,0);
                }
            }
        }

        if (isPhotoMode == 1)
        {
            memset(photoSrcPathName, 0x00, sizeof(photoSrcPathName));

            snprintf(photoSrcPathName, sizeof(photoSrcPathName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, srcDocId, PATH_SEPARATOR, IndexToPageNo);

            if (access(photoSrcPathName, F_OK) != 0)
            {
                XASSERT(0, 0);
            }
            else
            {
                memset(photoDstPathName, 0x00, sizeof(photoDstPathName));

                snprintf(photoDstPathName, sizeof(photoDstPathName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, dstDocId, PATH_SEPARATOR, IndexToPageNo);

                photoDst = fopen(photoDstPathName, "wb");

                if (photoDst == (FILE *)NULL)
                {
                    XASSERT(0,0);
                }
                else
                {
                    if (fclose(photoDst) != 0)
                    {
                        XASSERT(0,0);
                    }
                }
            }
        }
    }
}


/***********************************************************************
 *  Function: fsu_convert_char_to_int()
 *
 *  Description:
 *    convert char. to int including 0xA ~ 0xF
 *
 *  Inputs:
 *    char No: char.
 *
 *  Output:
 *	   No.
 **********************************************************************/
UINT8 fsu_convert_char_to_int (char No)
{
    UINT8 ret_val = 0;

    switch (No)
    {
        case '0':
            ret_val = 0;
            break;

        case '1':
            ret_val = 1;
            break;

        case '2':
            ret_val = 2;
            break;

        case '3':
            ret_val = 3;
            break;

        case '4':
            ret_val = 4;
            break;

        case '5':
            ret_val = 5;
            break;

        case '6':
            ret_val = 6;
            break;

        case '7':
            ret_val = 7;
            break;

        case '8':
            ret_val = 8;
            break;

        case '9':
            ret_val = 9;
            break;

        case 'A':
        case 'a':
            ret_val = 10;
            break;

        case 'B':
        case 'b':
            ret_val = 11;
            break;

        case 'C':
        case 'c':
            ret_val = 12;
            break;

        case 'D':
        case 'd':
            ret_val = 13;
            break;

        case 'E':
        case 'e':
            ret_val = 14;
            break;

        case 'F':
        case 'f':
            ret_val = 15;
            break;


        default:
            break;
    }

    return ret_val;
}


/***********************************************************************
 *  Function: fsu_convert_int_to_char()
 *
 *  Description:
 *    convert int. to char.
 *
 *  Inputs:
 *    int No: int.
 *
 *  Output:
 *	   char No.
 **********************************************************************/
char fsu_convert_int_to_char (int No)
{
    char ret_val = 0;

    switch (No)
    {
        case 0:
            ret_val = '0';
            break;

        case 1:
            ret_val = '1';
            break;

        case 2:
            ret_val = '2';
            break;

        case 3:
            ret_val = '3';
            break;

        case 4:
            ret_val = '4';
            break;

        case 5:
            ret_val = '5';
            break;

        case 6:
            ret_val = '6';
            break;

        case 7:
            ret_val = '7';
            break;

        case 8:
            ret_val = '8';
            break;

        case 9:
            ret_val = '9';
            break;

        default:
            break;
    }

    return ret_val;
}


/***********************************************************************
 *  Function: fsu_get_width_info()
 *
 *  Description:
 *    get the width info. with resolution and width
 *
 *  Inputs:
 *    UINT8 res: resolution (0 - Standard, 1 - Fine, 3 - 300 DPI)
 *    UINT32 Width: width in pixel
 *
 *  Output:
 *    UINT8 width - 0: A4, 1: B4, and 2: A3
 **********************************************************************/
UINT8 fsu_get_width_info(UINT8 res, UINT32 Width)
{
    UINT8 width = 0;

    if (res == 0 || res == 1) // Standard or Fine
    {
        if (Width == 1728)
        {
            width = 0; // A4
        }
        else if (Width == 2048)
        {
            width = 1; // B4
        }
        else
        {
            width = 2; // A3
        }
    }
    else
    {
        if (Width == 2592)
        {
            width = 0; // A4
        }
        else if (Width == 3072)
        {
            width = 1; // B4
        }
        else
        {
            width = 2; // A3
        }
    }

    return width;
}


/***********************************************************************
 *  Function: fsu_get_image_size()
 *
 *  Description:
 *    get the image size
 *
 *  Inputs:
 *    UINT16 jnum: job number
 *    UINT8 PageNo: page number
 *    UINT8 FileType: file type
 *
 *  Output:
 *	  UINT32: total size of file
 **********************************************************************/
UINT32 fsu_get_image_size (UINT16 jnum, UINT8 PageNo, UINT8 FileType)
{
    UINT8 *pBuff = (UINT8 *)NULL;

    pBuff = (UINT8 *)MEM_MALLOC(TEMP_TX_DATA_BUFFER);
    if( pBuff == (UINT8 *)NULL )
    {
        ASSERT(0);

        return 0;
    }
    else
    {
        char FileName[MAX_LEN_FILE_PATH_NAME] = {0, };
        char Buff[6] = {0, };
        UINT32 wid = fsu_get_image_width_pixel(jnum);
        UINT32 hig = fsu_get_image_height_pixel(jnum);
        int fhandle = 0;
        int nSize = 0;

        switch (FileType)
        {
            case FAX_FILE_TYPE_JBG:
                snprintf(Buff, sizeof(Buff), "jbg");
                break;

            case FAX_FILE_TYPE_PBM:
                snprintf(Buff, sizeof(Buff), "pbm");
                break;

            case FAX_FILE_TYPE_RAW:
                snprintf(Buff, sizeof(Buff), "raw");
                break;

            case FAX_FILE_TYPE_PHOTO:
                snprintf(Buff, sizeof(Buff), "photo");
                break;
        }

        if ( (FileType == FAX_FILE_TYPE_JBG) || (FileType == FAX_FILE_TYPE_PHOTO) )
        {
            snprintf(FileName, sizeof(FileName), "%s%cDoc%04X%cfax%02X.%s", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, PageNo, Buff);
        }
        else
        {
            snprintf(FileName, sizeof(FileName), "%s%cDoc%04X%cfax%02X.%s", pRootFolderNameRAM, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, PageNo, Buff);
        }

        fhandle = open(FileName, O_RDWR);

        if(fhandle < 0)
        {
            printf("\n(YHCHO) %s() - open Error... %s\n", __FUNCTION__, FileName);

            MEM_FREE_AND_NULL(pBuff);

            ASSERT(0);

            return 0;
        }
        else
        {
            nSize = read(fhandle, pBuff, wid * hig);

            if (nSize < 0)
            {
               printf("\n(YHCHO) %s() - read Error... %s\n", __FUNCTION__, FileName);

               MEM_FREE_AND_NULL(pBuff);

               ASSERT(0);

               return 0;
            }
            else
            {
                MEM_FREE_AND_NULL(pBuff);

                close(fhandle);

                printf("\n(YHCHO) %s() - nSize: %d\n", __FUNCTION__, nSize);

                return nSize;
            }
        }
    }
}


/***********************************************************************
 *  Function: fsu_get_time_to_tx()
 *
 *  Description:
 *    get the time to tx
 *
 *  Inputs:
 *    long time: time to tx in second
 *
 *  Output:
 *    time to tx
 **********************************************************************/
int fsu_get_time_to_tx (UINT16 DocId)
{
    int TimeToTx = 0;
    FILE *pFile = (FILE *)NULL;
    char infoFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char info[1024] = {0, };
    char Temp[14] = {0, };
    char *pTimeToTx ="TimeToTx     ";

    snprintf(infoFileName, sizeof(infoFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);

    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(infoFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);
    }
    else
    {
        int infoLen = 13;
        int Len = 0;

        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(info, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, info, infoLen);

            if (strcmp (Temp, pTimeToTx) == 0) // ex) TimeToTx     : 2015.05.19 15:30
            {
                Len = strlen(info);

                TimeToTx = fsu_convert_char_to_int(info[Len - 6]) * pow(10, 3) + fsu_convert_char_to_int(info[Len - 5]) * pow (10, 2) + fsu_convert_char_to_int(info[Len - 3]) * pow (10, 1) + fsu_convert_char_to_int(info[Len - 2]) * pow (10, 0);

                break;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0, 0);
        }
    }

    //printf("\n(YHCHO) %s() - TimeToTx: %d\n", __FUNCTION__, TimeToTx);

    return TimeToTx;
}


/***********************************************************************
 *  Function: fsu_get_rcv_done()
 *
 *  Description:
 *    check rcv done
 *
 *  Inputs:
 *   
 *  Output:
 *     0 : receiving
 *     1 : receive done
 **********************************************************************/
int fsu_get_rcv_done (void)
{
	if(fsu_get_status(fsu_job_num_prn) == FSU_STATE_RECEIVE_DONE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/***********************************************************************
 *  Function: fsu_request_status()
 *
 *  Description:
 *    request for the update of status
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_request_status (int jnum, int status)
{
    fsu_update_status((UINT16)jnum, (UINT8)status);
}


/***********************************************************************
 *  Function: fsu_update_txq_nxt()
 *
 *  Description:
 *    update txq.nxt to new value
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void fsu_update_txq_nxt (int jnum)
{
    txq.nxt = (UINT16)jnum;
}


/***********************************************************************
 *  Function: fsu_image_validation_check()
 *
 *  Description:
 *    validation check image
 *
 *  Inputs:
 *
 *  Output:
 *		0 : Fail
 * 		1 : OK
 **********************************************************************/
int fsu_image_validation_check (int res, int jnum)
{
	UINT32 height_pixel = fsu_get_image_height_pixel(jnum);
	printf("\n(YHCHO) %s() - res : %d, height : %ld\n", __FUNCTION__,res, height_pixel);
	//short & long
    if(res == 0)
    {
		if(height_pixel > 300) //A5 584
		{
			if(height_pixel < 2920) //A3 1168
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else if(res == 1)
	{
		if(height_pixel > 600) //A5 1169
		{
			if(height_pixel < 5845) //A3 2338
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}		
	}
	else // 300DPI
	{
		if(height_pixel > 900) //A5 1753
		{
			if(height_pixel < 8765) //A3 3506
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}		
	}
}


/***********************************************************************
 *  Function: fsu_request_image_backup_job_begin()
 *
 *  Description:
 *    entry point for image backup
 *
 *  Inputs:
 *    int Cnt: Number of valid jobs
 *
 *  Output:
 *
 **********************************************************************/
void fsu_request_image_backup_job_begin (int Cnt)
{
    UINT16 jnum = 0;

    printf("\n(YHCHO) %s() - Cnt: %d\n", __FUNCTION__, Cnt);

    for (jnum = 0; jnum < Cnt; jnum++)
    {
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.JobType[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.JobType[jnum]);

        if ( (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING) || (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_TX_PROGRESSING) || (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING_DELAY) || (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_FORWARD) || (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING_POLL) )
        {
            printf("\n(YHCHO) %s() - Step 1!\n", __FUNCTION__);

            UINT8 broadcast = fsu_get_broadcast(jnum);
            UINT16 DocId = fsu_get_doc_id(jnum);
            UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);
            UINT16 Retransmit = fsu_request_get_retransmit_from_Job_Queue_File(DocId);

            if (broadcast == 1)
            {
                if ( (DocId == OriginalDocId) && (Retransmit == 0) )
                {
                    printf("\n(YHCHO) %s() - Step 1-1!\n", __FUNCTION__);

                    // No Action
                }
                else
                {
                    printf("\n(YHCHO) %s() - Step 1-2!\n", __FUNCTION__);

                    set_tx_job_ready(jnum);
                }
            }
            else
            {
                printf("\n(YHCHO) %s() - Step 1-3!\n", __FUNCTION__);

                set_tx_job_ready(jnum);
            }
        }
        else if (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_POLL_RECEIVE)
        {
            printf("\n(YHCHO) %s() - Step 2!\n", __FUNCTION__);

            set_tx_job_ready(jnum);
        }
        else
        {
            printf("\n(YHCHO) %s() - Step 3!\n", __FUNCTION__);

            if (fsu_get_fax_forward(jnum) == 1)
            {
                printf("\n(YHCHO) %s() - Step 3-1!\n", __FUNCTION__);

                set_tx_job_ready(jnum);
            }
            else
            {
                printf("\n(YHCHO) %s() - Step 3-2!\n", __FUNCTION__);

                if (getSaveFaxFile() == ENABLE)
                {
                    printf("\n(YHCHO) %s() - Step 3-2-1!\n", __FUNCTION__);

                    // No Action
                }
                else
                {
                    printf("\n(YHCHO) %s() - Step 3-2-2!\n", __FUNCTION__);

                    fsu_set_prn_job_ready(jnum);

                    fsu_set_next_prn_num(jnum);
                }
            }
        }
    }
}


/***********************************************************************
 *  Function: fsu_check_image_file_validation_receive()
 *
 *  Description:
 *    check up the validation of image: size and T.4 (MMR) Encode
 *
 *  Inputs:
 *    UINT16 DocId: Document ID to check
 *    UINT8 Resolution: Resolution
 *
 *  Output:
 *		1 : OK
 * 		0 : FAIL
 **********************************************************************/
UINT8 fsu_check_image_file_validation_receive (UINT16 DocId, UINT8 Resolution, UINT16 ImageWidth)
{
    UINT8 ret = 0;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    struct stat stStat;
    DIR *pDir = (DIR *)NULL;
    UINT8 isValidationCheckOK = FSU_CHECK_IMAGE_FILE_VALIDATION_OK;

    snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

    pDir = opendir(DocIdPathName);

    if (pDir == (DIR *)NULL)
    {
        XASSERT(0,0);
    }
    else
    {
        UINT8 StartPageNo = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
        UINT8 LastPageNo = fsu_get_last_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG, 0);
        UINT8 TotalPageNo = 0;
        UINT8 IndexToPage = 0;
        UINT8 PageIndex = 0;
        UINT8 isT4EncodeOK = 0;
        int jbgFileSize = 0;
        char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
        char ariaFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
        char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };
        char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };

        for(IndexToPage = StartPageNo; IndexToPage < StartPageNo + LastPageNo; IndexToPage++)
        {
			snprintf(ariaFileName, sizeof(ariaFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);
            snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, IndexToPage);

            // Step 1. Check whether .jbg file exists...

            if (access(ariaFileName, F_OK) == 0)
            {
                // Step 2. Check file size for .jbg

                if (stat(ariaFileName, &stStat) != 0)
                {
                    printf("\n(YHCHO) %s() - errorno: %d, %s", __FUNCTION__, errno, ariaFileName);

                    XASSERT(0, 0);
                }
                else
                {
                    jbgFileSize = stStat.st_size;

                    if (jbgFileSize == 0)
                    {
                        isValidationCheckOK = FSU_CHECK_IMAGE_FILE_VALIDATION_FILE_SIZE_ZERO;

                        printf("\n(YHCHO) %s() - File Size is ZERO!!!!!\n", __FUNCTION__);

                        break;
                    }
                    else
                    {
                        // Step 3. Check file for T.4 (MMR) Encode

                        snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

                        if (IndexToPage == StartPageNo)
                        {
                            fsu_make_dir_on_file_system(DocDirPathRAM);
                        }
#ifdef HAVE_IMAGELOG
						if(isSDCardEnable)
						{
							if(ARIA_encode_path(ariaFileName, jbgFileName, 1, 1) == 0)
							{
								isT4EncodeOK = T4_encoded_image_validation_check(DocId, jbgFileName, ImageWidth, Resolution, CODING_MMR);

								fsu_delete_file_on_file_system(jbgFileName);
							}
							else
							{
								isT4EncodeOK = 1;//failure
							}
						}
						else
						{
							isT4EncodeOK = T4_encoded_image_validation_check(DocId, jbgFileName, ImageWidth, Resolution, CODING_MMR);
						}
#else
						isT4EncodeOK = T4_encoded_image_validation_check(DocId, jbgFileName, ImageWidth, Resolution, CODING_MMR);
#endif
                        if (isT4EncodeOK == 0)
                        {
                            // No Action
                        }
                        else
                        {
                            isValidationCheckOK = FSU_CHECK_IMAGE_FILE_VALIDATION_T4_ENCODE_FAIL;

                            printf("\n(YHCHO) %s() - T.4 (MMR) Encode Failed!!!!!\n", __FUNCTION__);

                            break;
                        }
                    }
                }
            }
            else
            {
                isValidationCheckOK = FSU_CHECK_IMAGE_FILE_VALIDATION_DOES_NOT_EXIST;

                break;
            }
        }

        if ( (isValidationCheckOK == FSU_CHECK_IMAGE_FILE_VALIDATION_T4_ENCODE_FAIL) || (isValidationCheckOK == FSU_CHECK_IMAGE_FILE_VALIDATION_OK) )
        {
            if (access(DocDirPathRAM, F_OK) == 0)
            {
                fsu_delete_dir_on_file_system(DocDirPathRAM);
            }
        }

        if (isValidationCheckOK == FSU_CHECK_IMAGE_FILE_VALIDATION_OK)
        {
            ret = 1;
        }
        else
        {
            StartPageNo = IndexToPage++;

            memset(ariaFileName, 0x00, sizeof(ariaFileName));

            for (PageIndex = StartPageNo; PageIndex < StartPageNo + LastPageNo; PageIndex++)
            {
                snprintf(ariaFileName, sizeof(ariaFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, PageIndex);

                if (access(ariaFileName, F_OK) == 0)
                {
                    fsu_delete_file_on_file_system(ariaFileName);

                    memset(ariaFileName, 0x00, sizeof(ariaFileName));
                }
            }

            TotalPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);

            printf("\n(YHCHO) %s() - TotalPageNo: %d\n", __FUNCTION__, TotalPageNo);

            if (TotalPageNo != 0)
            {
                ret = 1;
            }
            else
            {
                ret = 0;

                snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);

                fsu_delete_file_on_file_system(JobQueueFileName);

                fsu_delete_dir_on_file_system(DocIdPathName);
            }
        }

        closedir(pDir);
    }

    return ret;
}


/***********************************************************************
 *  Function: fsu_check_image_file_validation_transmit()
 *
 *  Description:
 *    check up the validation of transmit job especially for the member of broadcast
 *
 *  Inputs:
 *    UINT16 DocId: Document ID to check
 *
 *  Output:
 *		1 : OK
 * 		0 : FAIL
 **********************************************************************/
UINT8 fsu_check_image_file_validation_transmit (UINT16 DocId)
{
    UINT8 ret = 0;
    UINT8 isComplete = 0;
    UINT8 Retransmit = 0;
    UINT16 OriginalDocId = 0;
    UINT8 StartPageNo = 0;
    BOOLEAN isExist = 0;

    // Step 1. Check to see whether this is a member of broadcast by looking at the OriginalDocId
    OriginalDocId = fsu_request_get_original_doc_id_from_Job_Queue_File(DocId);
    printf("\n(YHCHO) %s() - DocId: %d, OriginalDocId: %d\n", __FUNCTION__, DocId, OriginalDocId);

    // Step 2. Check to see whether all members of broadcast are complete
    if(OriginalDocId == DEFAULT_ORIGINAL_DOC_ID) // Not broadcast Job
    {
		StartPageNo = fsu_get_start_page_no_in_mem (DocId, FAX_FILE_TYPE_JBG);
		if(StartPageNo == 0)
		{
			isComplete = 1;
		}
		else
		{
			isComplete = 0;
		}
    }
    else if(DocId != OriginalDocId) //broadcast Job & Not Original Doc
    {
		StartPageNo = fsu_get_start_page_no_in_mem (DocId, FAX_FILE_TYPE_JBG);
		if(StartPageNo == 0)
		{
			isComplete = 1;
		}
		else
		{
			isExist = fsu_get_doc_exist(OriginalDocId);
			if(isExist == TRUE)
			{
				isComplete = 0;
			}
			else
			{
				isComplete = 1;
			}
		}
	}
    else // broadcast Job & Original Doc
    {
		isComplete = fsu_get_broadcast_job_complete(DocId);
		printf("\n(YHCHO) %s() - isComplete: %d\n", __FUNCTION__, isComplete);
	}

    // Step 3. Check to see whether this is needed to transmit by looking at the Retransmit
    Retransmit = fsu_request_get_retransmit_from_Job_Queue_File(DocId);
    printf("\n(YHCHO) %s() - Retransmit: %d\n", __FUNCTION__, Retransmit);

    if ( (DocId == OriginalDocId) && (isComplete == 1) && (Retransmit == 0) )
    {
        ret = 0;
    }
    else if( (DocId != OriginalDocId) && (isComplete == 1) )
    {
		ret = 0;
	}
	else if( (OriginalDocId == DEFAULT_ORIGINAL_DOC_ID) && (isComplete == 1) )
	{
		ret = 0;
	}
	else
    {
        ret = 1;
    }

    return ret;
}


/***********************************************************************
 *  Function: fsu_request_check_spam_fax()
 *
 *  Description:
 *    check whether it is spam fax or not
 *
 *  Inputs:
 *
 *  Output:
 *		1 : Spam Fax
 * 		0 : Not Spam Fax
 **********************************************************************/
UINT8 fsu_request_check_spam_fax (void)
{
    UINT8 ret = 0;
    UINT8 Index = 0;
    UINT8 SearchJobDone = 0;
    UINT8 No = 0;
    char Tmp[21] = {0, };

    snprintf(Tmp, sizeof(Tmp), "%s", gcTSI_for_RemoteFaxNum);

    No = NoOfSpamFaxList;

    for (Index = 0 ; Index < No; Index++)
    {
        if (strcmp (Tmp, gFaxSpamFaxList[Index]) == 0)
        {
            SearchJobDone++;

            break;
        }
    }

    if (SearchJobDone == 0)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
}


/***********************************************************************
 *  Function: fsu_request_check_broadcast()
 *
 *  Description:
 *    check whether it is a member of broadcast especially for image backup
 *
 *  Inputs:
 *    UINT16 DocId: document id to to check
 *
 *  Output:
 *		1 : member of broadcast
 * 		0 : not a member of broadcast
 **********************************************************************/
UINT8 fsu_request_check_broadcast (UINT16 DocId)
{
    UINT8 ret = 0;
    UINT16 OriginalDocId = fsu_request_get_original_doc_id_from_Job_Queue_File(DocId);

    if (OriginalDocId == DEFAULT_ORIGINAL_DOC_ID)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
}


/***********************************************************************
 *  Function: fsu_update_broadcast_result_report_struct
 *
 *  Description:
 *    Update FSU_INFO_BROADCAST_RESULT_REPORT_t struct for report generation
 *
 *  Inputs:
 *    int nValue: new value to update
 *    UINT8 reason: determine which member to update
 *
 *  Output:
 *    none
 ************************************************************************/
void fsu_update_broadcast_result_report_struct (int nValue, UINT8 reason)
{
    switch (reason)
    {
        case BROADCAST_RESULT_REPORT_REASON_JOB_ID:
            BCResultReport.JobId = nValue;
            break;

        case BROADCAST_RESULT_REPORT_REASON_START_TIME:
            BCResultReport.StartTime = nValue;
            break;

        case BROADCAST_RESULT_REPORT_REASON_DURATION:
            BCResultReport.Duration = nValue;
            break;

        case BROADCAST_RESULT_REPORT_REASON_TOTAL_PAGE_NO:
            BCResultReport.TotalPageNo = nValue;
            break;

        case BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO:
            BCResultReport.SentPageNo = nValue;
            break;

        case BROADCAST_RESULT_REPORT_REASON_RESULT:
        {
            UINT8 Len = 0;
            UINT8 Index = 0;
            char DialNo[LEN_PHONE_NO + 1] = {0, };
            char TmpDialNo[LEN_PHONE_NO + 1] = {0, };

            // Step 1. Update Result
            BCResultReport.Result = nValue;

            // Step 2. Update DialNo
            snprintf(TmpDialNo, sizeof(TmpDialNo), "%s", fsu_get_dial_no(fsu_job_num_send));

            Len = strlen(TmpDialNo);

            for (Index = 0; Index < Len; Index++)
            {
                if (TmpDialNo[Index] == 0x2C)
                {
                    DialNo[Index] = 0x2D;
                }
                else
                {
                    DialNo[Index] = TmpDialNo[Index];
                }
            }

            DialNo[Index] = 0;

            snprintf(BCResultReport.RemoteNo, sizeof(BCResultReport.RemoteNo), "%s", DialNo);

            break;
        }

        default:
            break;
    }
}


/***********************************************************************
 *  Function: fsu_request_write_broadcast_result_report_info_to_db
 *
 *  Description:
 *    start point to update the RECD_FAXBROADCASTRESULTREPORT
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *    none
 ************************************************************************/
void fsu_request_write_broadcast_result_report_info_to_db (void)
{
    faxapp_update_broadcast_result_report_info_to_db(&BCResultReport);
}


/***********************************************************************
 *  Function: fsu_get_broadcast_job_complete
 *
 *  Description:
 *    check whether all members of broadcast are complete
 *
 *  Inputs:
 *    UINT16 DocId: Document ID to check
 *
 *  Output:
 *    1 : Complete
 *    0 : Not complete
 ************************************************************************/
UINT8 fsu_get_broadcast_job_complete (UINT16 DocId)
{
    UINT8 ret = 0;
    UINT16 OriginalDocId = 0;
    UINT16 IndexToDocId = 0;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    DIR *pDir = (DIR *)NULL;

    // Step 1. Search for all the /DocMan/DocXXXX...
    for (IndexToDocId = 0; IndexToDocId < MAX_NUM_DOC_ID; IndexToDocId++)
    {
        snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, IndexToDocId);

        pDir = opendir(DocIdPathName);

        // Step 2. Check to see whether the /DocMan/DocXXXX exists...
        if (pDir == (DIR *)NULL)
        {
            ; // No Action Here...
        }
        else
        {
            if (DocId != IndexToDocId)
            {
                memset(JobQueueFileName, 0x00, sizeof(JobQueueFileName));

                snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToDocId);

                if (access(JobQueueFileName, F_OK) != 0)
                {
                    ret = 1;
                }
                else
                {
                    OriginalDocId = fsu_request_get_original_doc_id_from_Job_Queue_File(IndexToDocId);

                    // Step 3. Check to see whether this is a member of broadcast referenced by DocId...
                    if (DocId == OriginalDocId)
                    {
                        ret = 0;

                        closedir(pDir);

                        break;
                    }
                    else
                    {
                        ret = 1;
                    }
                }
            }
            else
            {
                ret = 1;
            }

            closedir(pDir);
        }
    }

    return ret;
}


#if __DISPLAY_CURRENT_LOCAL_JOB_TIME__
/***********************************************************************
 *  Function: fsu_get_current_local_job_time_in_string()
 *
 *  Description:
 *    get the current local job time in string
 *
 *  Inputs:
 *
 *  Output:
 *    pointer to CurrentLocalJobTime
 **********************************************************************/
char *fsu_get_current_local_job_time_in_string(void)
{
    struct timeval CurrentTime;
    struct tm *ptm;

    gettimeofday(&CurrentTime, NULL);

    ptm = localtime(&CurrentTime.tv_sec);

    memset(CurrentLocalJobTime, 0x00, sizeof(CurrentLocalJobTime));

    if(ptm == NULL)
    {
        snprintf(CurrentLocalJobTime, sizeof(CurrentLocalJobTime), "%04d-%02d-%02d %02d:%02d:%02d", 1900, 1, 1, 0, 0, 0);
    }
    else
    {
        UINT8 Type = getCountryDateType();

        switch (Type)
        {
            case FAX_COUNTRY_DATE_TYPE_YMD:
            {
                snprintf(CurrentLocalJobTime, sizeof(CurrentLocalJobTime), "%04d-%02d-%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

                break;
            }
            case FAX_COUNTRY_DATE_TYPE_MDY:
            {
                snprintf(CurrentLocalJobTime, sizeof(CurrentLocalJobTime), "%02d-%02d-%04d %02d:%02d:%02d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

                break;
            }

            case FAX_COUNTRY_DATE_TYPE_DMY:
            {
                snprintf(CurrentLocalJobTime, sizeof(CurrentLocalJobTime), "%02d-%02d-%04d %02d:%02d:%02d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

                break;
            }

            default:
                break;
        }
    }

    return CurrentLocalJobTime;
}
#else
#endif


inline long int fsu_calculate_time_diff_in_sec (int Year, int Month, int Day, int Hour, int Min)
{
    long int ret = 0;
    #define MIN_IN_SEC     (60)
    #define HOUR_IN_SEC     (MIN_IN_SEC * 60)
    #define DAY_IN_SEC     (HOUR_IN_SEC * 60)
    #define MONTH_IN_SEC     (DAY_IN_SEC * 60)
    #define YEAR_IN_SEC     (MONTH_IN_SEC * 60)

    ret = (Min * MIN_IN_SEC) + (Hour * HOUR_IN_SEC) + (Day * DAY_IN_SEC) + (Month * MONTH_IN_SEC) + (Year * YEAR_IN_SEC);

    return ret;
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
void fsu_request_get_pdf_file_name(const char *pPath, UINT16 jnum, UINT8 job_type, char *pdf_file_name)
{
    if (job_type == SCAN_TO_MEM)
    {
        snprintf(pdf_file_name, MAX_LEN_FILE_PATH_NAME, "%s%c%s.pdf", pPath, PATH_SEPARATOR, fsu_get_file_name_tx(jnum));
    }
    else
    {
        snprintf(pdf_file_name, MAX_LEN_FILE_PATH_NAME, "%s%cImgDoc%04X%c%s.pdf", pRootFolderNameSolDocMan, PATH_SEPARATOR, jnum, PATH_SEPARATOR, fsu_get_file_name_tx(jnum));
    }
}
#endif

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
void fsu_request_make_dot_txt_file (const char *pPath, UINT16 jnum)
{
    char dotTXTFile[MAX_LEN_FILE_PATH_NAME] = {0, };
    FILE *pDotTXTFile = (FILE *)NULL;

    snprintf(dotTXTFile, sizeof(dotTXTFile), "%s/%s.txt", pPath, fsu_get_file_name_tx(jnum));

	pDotTXTFile = fopen(dotTXTFile,"wb");
    if (pDotTXTFile == (FILE *)NULL)
    {
        ASSERT(0);

        return;
    }
    else
    {
        char userId[MAX_LEN_USER_ID + 9] = {0, };
        char ipAddress[MAX_LEN_IP_ADDRESS + 12] = {0, };
        char sendFaxNums[LEN_PHONE_NO * MAX_NUM_BC + 14] = {0, };
        
        memset(userId, 0, sizeof(userId));
        memset(ipAddress, 0, sizeof(ipAddress));
        memset(sendFaxNums, 0, sizeof(sendFaxNums));

        snprintf(userId, sizeof(userId), "userId=%s\n", gFaxUserId);

        printf("\n(YHCHO) %s() - userId: %s", __func__, userId);

        snprintf(ipAddress, sizeof(ipAddress), "ipAddress=%s\n", faxapp_get_ip_address());

        printf("\n(YHCHO) %s() - ipAddress: %s\n", __func__, ipAddress);
#if 0
        if (fsu_get_broadcast(jnum) == 0)
        {
            snprintf(sendFaxNums, sizeof(sendFaxNums), "sendFaxNums=%s\n", fsu_get_dial_no(jnum));
        }
        else
#endif
        {
            UINT8 Len = 0;
            UINT8 IndexToDial = 0;
            UINT8 IndexToRecipient = 0;
            UINT8 OriginalDocId = fsu_get_original_doc_id(jnum);
            UINT8 TotalRecipient = faxapp_get_no_of_recipient();
            UINT8 DialNo[LEN_PHONE_NO + 1] = {0, };
            char TmpSendFaxNums[ (LEN_PHONE_NO + 1) * MAX_NUM_BC + 1] = {0, };
            int Index = 0;
            
            printf("\n(YHCHO) %s() - TotalRecipient: %d", __func__, TotalRecipient);

            for(IndexToRecipient = 0; IndexToRecipient < TotalRecipient; IndexToRecipient++)
            {
                memset(DialNo, 0x00, sizeof(DialNo));
				
				if(TotalRecipient > 1)
				{
					memcpy(DialNo, fsu_get_dial_no(gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient]), LEN_PHONE_NO + 1);
				}
				else
				{
					memcpy(DialNo, fsu_get_dial_no(jnum), LEN_PHONE_NO + 1);
				}

                Len = strlen((char *)DialNo);

                for(IndexToDial = 0; IndexToDial < Len; IndexToDial++)
                {
                    if (DialNo[IndexToDial] == ',')
                    {
                        TmpSendFaxNums[Index] = '-';
                    }
                    else
                    {
                        TmpSendFaxNums[Index] = DialNo[IndexToDial];
                    }

                    Index++;
                }

                if (IndexToRecipient == (TotalRecipient - 1))
                {
                    TmpSendFaxNums[Index] = 0; // null termination
                }
                else
                {
                    TmpSendFaxNums[Index] = ','; // seperator between fax no.

                    Index++;
                }
            }

            snprintf(sendFaxNums, sizeof(sendFaxNums), "sendFaxNums=%s\n", TmpSendFaxNums);
        }

        printf("\n(YHCHO) %s() - sendFaxNums: %s", __func__, sendFaxNums);

        // Step 1. write userId
        fputs(userId, pDotTXTFile);

        // Step 2. write ipAddress
        fputs(ipAddress, pDotTXTFile);

        // Step 3. write sendFaxNums
        fputs(sendFaxNums, pDotTXTFile);

        if (fflush(pDotTXTFile) != 0)
        {
            XASSERT(0,0);
        }

        if (fsync(pDotTXTFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        if (fclose(pDotTXTFile) != 0)
        {
            XASSERT(0,0);
        }
    }
}
#endif

#if __HANA_FINANCIAL_GROUP_REQ_014_5__ || __HANA_FINANCIAL_GROUP_REQ_021__
void fsu_request_make_dot_end_file (const char *pPath, UINT16 jnum, UINT8 job_type)
{
    char dotEndFile[MAX_LEN_FILE_PATH_NAME] = {0, };
    FILE *pDotEndFile = (FILE *)NULL;

    if (job_type == SCAN_TO_MEM)
    {
        snprintf(dotEndFile, sizeof(dotEndFile), "%s%c%s.end", pPath, PATH_SEPARATOR, fsu_get_file_name_tx(jnum));
    }
    else
    {
        snprintf(dotEndFile, sizeof(dotEndFile), "%s%cImgDoc%04X%c%s.end", pRootFolderNameSolDocMan, PATH_SEPARATOR, jnum, PATH_SEPARATOR, fsu_get_file_name_tx(jnum));
    }

    pDotEndFile = fopen(dotEndFile, "wb");

    if (pDotEndFile == (FILE *)NULL)
    {
        ASSERT(0);

        return;
    }
    else
    {
        fclose(pDotEndFile);
    }
}
#endif

