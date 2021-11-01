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
    File: fsu_cmd.c
    Date: March 16, 2005
    By:   S.J. Busak

    Contains code to process FSU commands and generate responses.
*/

#define __FSU_CMD_C__    // Allocates storage for variables in fsu_cmd.h

#include <stdio.h>
#include <string.h>
#include "dc_type.h"
#include "mosi.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "fax_memAPI.h"
#include "dc_fwcxdxx_lnx_api.h"
#include "fshs.h"
#include "dc_return_codes.h"
#include "fxcb.h"
#include "fsu.h"
#include "fsu_call.h"
#include "fsu_api.h"
#include "SDHFaxCommonHeader.h"
#include <dirent.h>
#include "../../faxapp/include/faxapp_job.h"
#include "../../faxapp/include/faxapp_scan.h"
#include "faxapp_notify.h"
#include "lassert.h"
#include "../../image_backup/include/image_backup.h"
#include "ERROR.H"
#include "t30.h"
#include "error_types.h" // OK : 0, FAIL : -1

void fsu_make_doc_man_dir_in_nand(void);
void set_tx_job_ready(UINT16 jnum);
void copy_str (UINT8 *src, UINT8 *dest);
void set_txq_nxt (void);
void fsu_make_doc_dir_in_ram(UINT16 DocId);
void fsu_set_next_prn_num(UINT16 next_prn_num);
UINT16 fsu_make_doc_dir_in_nand(void);
UINT16 fsu_get_next_prn_num(void);


extern void activate_scan_to_mem (void);
extern void Fax_API_TransmitFax(FAXAPP_PARAMS_t *pFaxStruct);
extern void Fax_API_PollRecieveFax(FAXAPP_PARAMS_t *pFaxStruct);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern void Update_Job_Queue_File(IMAGE_BACKUP_REASON_t Reason, UINT16 jnum, UINT16 Param);
extern void fsu_update_broadcast_result_report_struct (int nValue, UINT8 reason);
extern void faxapp_request_update_user_id (void);
extern void fsu_update_user_id (UINT16 jnum, char *pUserId);
extern UINT16 fsu_get_doc_id(UINT16 jnum);
extern int fsu_get_memory_full_status_nand(void);
extern int fsu_get_memory_full_status_ram (void);
extern void fsu_purge_doc_dir(UINT16 jnum);
extern void faxapp_scan_cancel(void);
extern int faxapp_print_user_input(void);
extern BOOLEAN fsu_check_doc_man_dir_in_ram (const char *Path);
extern BOOLEAN fsu_check_doc_man_dir_in_nand (const char *Path);
#ifdef HAVE_IMAGELOG
extern BOOLEAN IsImageLogEnabled(char jobtype, int subJobType);
#endif
extern int getgFaxPrintMode(void);
extern UINT8 fsu_get_status (UINT16 jnum);
extern UINT8 fsu_request_check_spam_fax (void);
extern UINT8 getCountryDateType(void);
extern UINT8 fsu_get_isDuplex (UINT16 jnum);
extern UINT8 fsu_requset_get_current_page_no_from_Job_Queue_File (UINT16 jnum);
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern void fsu_update_file_name_tx (UINT16 jnum, const char *pfile_name_tx);
#endif
extern int fsu_get_scan_busy_flag (void);

extern UINT8 gFaxRebootCnt;
extern UINT16 gFaxT30Result;
extern UINT8 isSolutionFax;

#if __HANA_FINANCIAL_GROUP_REQ_021__
extern char gcTSI_for_RemoteFaxNum[FCFSendTableSize + 1];

extern UINT8 isSDCardEnable;
extern char *pRootFolderARIA;
#endif

//.. definition for fax header rasterization
#define LEFT_MAR     10
#define RASTERIZER_CHAR_SLICES     9
#define ENDIAN     ENDIAN_BIG
#define TOP_MARGINE     6
#define BOTTOM_MARGINE     1
#define MAR_BETWEEN_FIELD     15



static UFIX8 RasterizerNoOfCharSlices = RASTERIZER_CHAR_SLICES;
static UFIX8 RasterizerTopMargin = TOP_MARGINE;

/* This should define the number of lines per row (ie with a repeat factor of 1) */
static UFIX8 RstrizeLinesPerRow = RASTERIZER_CHAR_SLICES /* char slices */		// edit 12-03
                              + TOP_MARGINE + BOTTOM_MARGINE;
static UFIX8 RasterizerCharSlices[RASTERIZER_CHAR_SLICES][96];

const UFIX8 RepGenRepeatTable[4] = {
   2,       /* Less than 100 DPI vertical */
   4,       /* Between 100 DPI and 200 DPI vertical */
   6,       /* Between 200 DPI and 300 DPI vertical */
   6        /* Between 300 DPI and 400 DPI vertical */
};

int ValidJobCnt = 0;

UINT16 fsu_prn_num_array[TX_JOBS] = {0};
int next_prn_job_check = 0;
int last_prn_job_num_check = 0;
int current_prn_job_num = 0;
int del_job_pending = 0;

UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];
UINT8 isFaxAppReady = 0;
UINT8 TotalSendPageNo = 0;
extern UINT8 gFaxCurrentTTIPageNo;
extern int check_job_queue_validation (void);
extern void fsu_update_status (UINT16 jnum, UINT8 Status);
extern void fsu_set_prn_job_done(UINT16 jnum);
extern void fsu_delete_file_on_file_system (const char *Path);
extern void faxapp_solution_job_begin (void);
extern void fsu_request_write_broadcast_result_report_info_to_db (void);
extern void faxapp_process_broadcast_result_report(UINT16 jnum);
#ifdef HAVE_IMAGELOG
extern void fsu_update_remote_no_for_image_log (UINT8 type, UINT16 jnum);
#endif
extern BOOLEAN fsu_check_job_queue_file (const char *Path);
extern UINT8 faxapp_get_resolution(void);
extern UINT8 fsu_get_poll_transmit (UINT16 jnum);
extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT8 fsu_get_width (UINT16 jnum);
extern UINT8 fsu_get_total_pages (UINT16 jnum);
extern UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
extern UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_get_total_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_get_print_reason (UINT16 jnum);
extern UINT8 fsu_get_photo_mode(UINT16 jnum);
extern UINT8 fsu_get_tx_total_page_no (UINT16 jnum);
extern UINT8 fsu_request_get_total_page_no_from_Job_Queue_File (UINT16 DocId);
extern int fsu_get_job_queue_full_status(void);
extern void fsu_update_tx_time (UINT16 jnum, long TimeToTx);
extern unsigned long getint(FILE *f);
extern int getDuplexPrintOn(void);
extern UINT8 faxapp_get_ImageType(void);
extern UINT8 fsu_get_split_print (UINT16 jnum);

#ifdef HAVE_IMAGELOG
UINT8 gFaxForwardNo[LEN_PHONE_NO + 1] = {0, };
UINT16 fsu_job_num_scan_image_log = 0;
#endif
UINT16 fsu_job_num_scan = 0;
UINT16 fsu_job_num_send = 0;
UINT16 fsu_job_num_rcv = 0;
UINT16 fsu_job_num_prn = TX_JOBS;

UINT16 fsu_job_num_poll = 0;

extern UINT8 fsu_current_funcs;
extern UINT8 gFaxStationID[LEN_PHONE_NO + 1];
extern UINT8 gFaxStationNo[LEN_PHONE_NO + 1];
extern UINT8 gRealFaxStationNo[LEN_PHONE_NO + 1];
extern UINT8 gFaxScanSource;
extern UINT8 FBMultiPageScan;
extern UINT8 isSpamFaxOn;
extern UINT8 gFaxEndOfFax;
#if __SUPPORT_MANUAL_TRANSMIT__
extern UINT8 ExtPhoneDialBuffer[LEN_PHONE_NO + 1];
#else
#endif
extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern char *pRootFolderNameSolDocMan;
extern char gFaxDelayTimeInStringJobLog[MAX_LEN_DATE_TIME];
extern char gFaxUserId[MAX_LEN_USER_ID + 1];
extern int JobStartPageNo;
extern int gFaxAbnormalFlag;
extern BOOLEAN prn_cancel_flag;


/***********************************************************************
 *  Function: fsu_start_scan_to_mem
 *
 *  Description:
 *    starting point of scan to mem process
 *
 *  Inputs:
 *    FAXAPP_PARAMS_t FaxScanStruct
 *
 *  Output:
 *
 ***********************************************************************/
void fsu_start_scan_to_mem (FAXAPP_PARAMS_t *FaxScanStruct)
{
    if (FaxScanStruct->fax_type == FAX_APP_JOB_TYPE_POLL_TRANSMIT)
    {
        UINT8 PollTx = TXPOLLFROM_MEMORY;

        Fax_API_T30Attribute(SET, FXA_POLLTXMODE, &PollTx);
    }
#if __SUPPORT_MANUAL_TRANSMIT__
    else if (FaxScanStruct->fax_type == FAX_APP_JOB_TYPE_MANUAL_TRANSMIT)
    {
        set_fsu_state(FSU_STATE_TX_START_MANUAL);
    }
#else
#endif

    Fax_API_TransmitFax(FaxScanStruct);
}


/***********************************************************************
 *  Function: fsu_start_poll_receive
 *
 *  Description:
 *    starting point of poll receive
 *
 *  Inputs:
 *    FAXAPP_PARAMS_t FaxScanStruct
 *
 *  Output:
 *
 ***********************************************************************/
void fsu_start_poll_receive (FAXAPP_PARAMS_t *FaxScanStruct)
{
    UINT8 PollRx = RX_POLL;

    Fax_API_T30Attribute(SET, FXA_POLLRXMODE, &PollRx);

    Fax_API_PollRecieveFax(FaxScanStruct);
}


/***********************************************************************
 *  Function: fsu_scan_to_mem()
 *
 *  Description:
 *    Scan document and store on memory. Also this code create Tx job
 *
 *  Inputs:
 *    tx_type: type of tx
 *
 *  Output:
 *	   typedef FSU_RETURN_CODE
 ***********************************************************************/
FSU_RETURN_CODE fsu_scan_to_mem(FAXAPP_PARAMS_t *pFaxStruct)
{
    UINT8 IndexToRecipient = 0;
    UINT8 retry = 0;
    UINT8 retry_tmr = 0;
    UINT8 res = 0;
    UINT8 photo_mode = 0;
    UINT16 DocId = 0;
    UINT16 OriginalDocId = 0;
    long StartTimeInSec = 0;
    long TimeToTx = 0;
    TX_JOB_t *ptx_job = (TX_JOB_t *)NULL;

    printf("\n(YHCHO) %s()\n", __FUNCTION__);

    if (fsu_get_memory_full_status_nand() == 1)
    {
        printf("\n(YHCHO) %s() - NAND Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FSU_MEMORY_FULL_NAND;
    }

    if (fsu_get_memory_full_status_ram() == 1)
    {
        printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FSU_MEMORY_FULL_RAM;
    }

    if (fsu_get_job_queue_full_status() == 1)
    {
        printf("\n(YHCHO) %s() - Job Queue Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FSU_JOBQ_FULL;
    }

    fsu_job_num_scan = txq.nxt;

    printf("\n(YHCHO) %s() - fsu_job_num_scan: %d\n", __FUNCTION__, fsu_job_num_scan);

    // Step 1. Point to the next txq.nxt for the next job
    set_txq_nxt ();

    ptx_job = &txq.job[fsu_job_num_scan];

    // Step 2. Make DocXXXX directory
    // Step 2-1. Make /DocMan/DocXXXX/
    DocId = fsu_make_doc_dir_in_nand();

    // Step 2-2. Make /tmp/DocMan/DocXXXX
    fsu_make_doc_dir_in_ram(DocId);

    res = faxapp_get_resolution();
    photo_mode = faxapp_get_ImageType();

    Fax_API_T30Attribute(GET, FXA_RETRY_NUMBER, &retry);
    Fax_API_T30Attribute(GET, FXA_RETRY_INTERVAL, &retry_tmr);

    faxapp_request_update_user_id();

    // Step 3. Store job queue information
    if ( (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_TX_IMMEDIATE) || (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_DELAY) 
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
			|| (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_FAX_SOLUTION && faxapp_get_no_of_recipient() == 1) 
#endif
#if __HAVE_FAXSOLUTION_DK__
			|| (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_DK_SOLUTION && faxapp_get_no_of_recipient() == 1)
#endif
    )
    {
        copy_str(&pFaxStruct->dial_num[0][0], &ptx_job->fax_num[0]);

        if (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_DELAY)
        {
            ptx_job->isDelay = TRUE;
        }
        else
        {
            ptx_job->isDelay = FALSE;
        }
        ptx_job->status = FSU_STATE_SCANNING;
        ptx_job->retry = retry;
        ptx_job->retry_tmr = retry_tmr;

        if (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_DELAY)
        {
            if (pFaxStruct->time_diff == 0)
            {
                TimeToTx = TRANSMIT_TIME_RIGHT_NOW;
            }
            else
            {
                StartTimeInSec = fsu_get_current_system_time();

                TimeToTx = StartTimeInSec + pFaxStruct->time_diff;
            }

            fsu_update_tx_time(fsu_job_num_scan, TimeToTx);
        }
        else
        {
            fsu_update_tx_time(fsu_job_num_scan, pFaxStruct->time_diff);
        }
        ptx_job->res = res;
        ptx_job->width = pFaxStruct->width;
        ptx_job->ff_job = 0;
        ptx_job->poll_transmit = 0;
        ptx_job->poll_receive = 0;
        ptx_job->broadcast = 0;
        ptx_job->flag_manual_tx = 0;
        ptx_job->doc_id = DocId;
        ptx_job->original_doc_id = DEFAULT_ORIGINAL_DOC_ID;
        ptx_job->PrintReason = PRINT_REASON_RECEIVE;
        ptx_job->photo_mode = photo_mode;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
		if(pFaxStruct->fax_type == FAX_APP_JOB_TYPE_FAX_SOLUTION)
		{
			ptx_job->send_permission = SOLUTION_PERMISSION_STATE_DISABLE;
		}
		else
		{
			ptx_job->send_permission = SOLUTION_PERMISSION_STATE_ENABLE;
		}
#endif

        fsu_update_user_id(fsu_job_num_scan, gFaxUserId);

        if (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_DELAY)
        {
            snprintf(ptx_job->DelayTimeInText, sizeof(ptx_job->DelayTimeInText), "%s", gFaxDelayTimeInStringJobLog);
        }
        else
        {
            ptx_job->DelayTimeInText[0] = 0x20; // Add one space..
            ptx_job->DelayTimeInText[1] = 0;
        }

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        memcpy(ptx_job->file_name_tx, pFaxStruct->file_name, strlen(pFaxStruct->file_name) + 1);
#endif
    }
    else if ( pFaxStruct->fax_type == FAX_APP_JOB_TYPE_BROADCAST 
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
			|| (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_FAX_SOLUTION && faxapp_get_no_of_recipient() > 1) 
#endif
#if __HAVE_FAXSOLUTION_DK__
			|| (pFaxStruct->fax_type == FAX_APP_JOB_TYPE_DK_SOLUTION && faxapp_get_no_of_recipient() > 1)
#endif
    )
    {
        OriginalDocId = DocId;

        for( IndexToRecipient = 0 ; IndexToRecipient < pFaxStruct->no_of_recipient; IndexToRecipient++ )
        {
            if( IndexToRecipient != 0 )
            {
                fsu_job_num_scan = txq.nxt;

                set_txq_nxt ();

                ptx_job = &txq.job[fsu_job_num_scan];

                DocId = fsu_make_doc_dir_in_nand();

                fsu_make_doc_dir_in_ram(DocId);
            }

            copy_str(&pFaxStruct->dial_num[IndexToRecipient][0], &ptx_job->fax_num[0]);
            ptx_job->isDelay = FALSE;
            ptx_job->status = FSU_STATE_SCANNING;
            ptx_job->retry = retry;
            ptx_job->retry_tmr = retry_tmr;
            ptx_job->tx_time = pFaxStruct->time_diff;
            ptx_job->res = res;
            ptx_job->width = pFaxStruct->width;
            ptx_job->ff_job = 0;
            ptx_job->poll_transmit = 0;
            ptx_job->poll_receive = 0;
            ptx_job->broadcast = 1;
            ptx_job->flag_manual_tx = 0;
            ptx_job->doc_id = DocId;
            ptx_job->original_doc_id = OriginalDocId;
            ptx_job->PrintReason = PRINT_REASON_RECEIVE;
            ptx_job->photo_mode = photo_mode;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
			if(pFaxStruct->fax_type == FAX_APP_JOB_TYPE_FAX_SOLUTION)
			{
				ptx_job->send_permission = SOLUTION_PERMISSION_STATE_DISABLE;
			}
			else
			{
				ptx_job->send_permission = SOLUTION_PERMISSION_STATE_ENABLE;
			}
#endif
            fsu_update_user_id(fsu_job_num_scan, gFaxUserId);

            ptx_job->DelayTimeInText[0] = 0x20; // Add one space..
            ptx_job->DelayTimeInText[1] = 0;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
            memcpy(ptx_job->file_name_tx, pFaxStruct->file_name, strlen(pFaxStruct->file_name) + 1);
#endif
			
            gFaxBroadcastJobInfo[OriginalDocId][IndexToRecipient] = fsu_job_num_scan;
        }

        gFax_BC_Num[OriginalDocId].BC_Total_Num = pFaxStruct->no_of_recipient;
        gFax_BC_Num[OriginalDocId].BC_Current_Num = 0;

        fsu_job_num_scan = gFaxBroadcastJobInfo[OriginalDocId][0];
    }
    else if ( pFaxStruct->fax_type == FAX_APP_JOB_TYPE_POLL_TRANSMIT )
    {
        ptx_job->isDelay = FALSE;
        ptx_job->status = FSU_STATE_SCANNING;
        ptx_job->retry = 1;
        ptx_job->retry_tmr = 0;
        ptx_job->tx_time = pFaxStruct->time_diff;
        ptx_job->res = res;
        ptx_job->width = pFaxStruct->width;
        ptx_job->ff_job = 0;
        ptx_job->poll_transmit = 1;
        ptx_job->poll_receive = 0;
        ptx_job->broadcast = 0;
        ptx_job->flag_manual_tx = 0;
        ptx_job->doc_id = DocId;
        ptx_job->original_doc_id = DEFAULT_ORIGINAL_DOC_ID;
        ptx_job->PrintReason = PRINT_REASON_RECEIVE;
        ptx_job->photo_mode = photo_mode;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        ptx_job->send_permission = SOLUTION_PERMISSION_STATE_ENABLE;
#endif
        fsu_update_user_id(fsu_job_num_scan, gFaxUserId);

        ptx_job->DelayTimeInText[0] = 0x20; // Add one space..
        ptx_job->DelayTimeInText[1] = 0;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        memcpy(ptx_job->file_name_tx, pFaxStruct->file_name, strlen(pFaxStruct->file_name) + 1);
#endif

        fsu_job_num_poll = fsu_job_num_scan;
    }
    else if ( pFaxStruct->fax_type == FAX_APP_JOB_TYPE_ON_HOOK )
    {
        copy_str(&pFaxStruct->dial_num[0][0], &ptx_job->fax_num[0]);
        ptx_job->isDelay = FALSE;
        ptx_job->status = FSU_STATE_SCANNING;
        ptx_job->retry = 1;
        ptx_job->retry_tmr = 0;
        ptx_job->tx_time = TRANSMIT_TIME_RIGHT_NOW;
        ptx_job->res = res;
        ptx_job->width = pFaxStruct->width;
        ptx_job->ff_job = 0;
        ptx_job->poll_transmit = 0;
        ptx_job->poll_receive = 0;
        ptx_job->broadcast = 0;
        ptx_job->flag_manual_tx = 1;
        ptx_job->doc_id = DocId;
        ptx_job->original_doc_id = DEFAULT_ORIGINAL_DOC_ID;
        ptx_job->PrintReason = PRINT_REASON_RECEIVE;
        ptx_job->photo_mode = photo_mode;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        ptx_job->send_permission = SOLUTION_PERMISSION_STATE_ENABLE;
#endif
        fsu_update_user_id(fsu_job_num_scan, gFaxUserId);

        ptx_job->DelayTimeInText[0] = 0x20; // Add one space..
        ptx_job->DelayTimeInText[1] = 0;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        memcpy(ptx_job->file_name_tx, pFaxStruct->file_name, strlen(pFaxStruct->file_name) + 1);
#endif

        fsu_job_num_send = fsu_job_num_scan;

        fsu_current_funcs &= ~FAX_MEM_TO_SEND_DIALING;
        fsu_current_funcs |= FAX_MEM_TO_SEND_BEGIN;
    }
#if __SUPPORT_MANUAL_TRANSMIT__
    else if ( pFaxStruct->fax_type == FAX_APP_JOB_TYPE_MANUAL_TRANSMIT )
    {
        copy_str(&pFaxStruct->dial_num[0][0], &ptx_job->fax_num[0]);
        ptx_job->isDelay = FALSE;
        ptx_job->status = FSU_STATE_SCANNING;
        ptx_job->retry = 1;
        ptx_job->retry_tmr = 0;
        ptx_job->tx_time = TRANSMIT_TIME_RIGHT_NOW;
        ptx_job->res = res;
        ptx_job->width = pFaxStruct->width;
        ptx_job->ff_job = 0;
        ptx_job->poll_transmit = 0;
        ptx_job->poll_receive = 0;
        ptx_job->broadcast = 0;
        ptx_job->flag_manual_tx = 1;
        ptx_job->doc_id = DocId;
        ptx_job->original_doc_id = DEFAULT_ORIGINAL_DOC_ID;
        ptx_job->PrintReason = PRINT_REASON_RECEIVE;
        ptx_job->photo_mode = photo_mode;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        ptx_job->send_permission = SOLUTION_PERMISSION_STATE_ENABLE;
#endif
        fsu_update_user_id(fsu_job_num_scan, gFaxUserId);

        ptx_job->DelayTimeInText[0] = 0x20; // Add one space..
        ptx_job->DelayTimeInText[1] = 0;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        memcpy(ptx_job->file_name_tx, pFaxStruct->file_name, strlen(pFaxStruct->file_name) + 1);
#endif

        fsu_job_num_send = fsu_job_num_scan;

        fsu_current_funcs &= ~FAX_MEM_TO_SEND_DIALING;
        fsu_current_funcs |= FAX_MEM_TO_SEND_BEGIN;
    }
#else
#endif
    else
    {
        /* should never get here! */
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_INVALID_FAX_TYPE);
    }

#ifdef HAVE_IMAGELOG
    // Save the remote no.
    if (IsImageLogEnabled('F', IMAGE_LOG_JOB_TYPE_TX) == TRUE)
    {
        fsu_update_remote_no_for_image_log(IMAGE_LOG_JOB_TYPE_TX, fsu_job_num_scan);
    }
#endif

    activate_scan_to_mem();

    return FSU_OK;
}


/***********************************************************************
 *  Function: fsu_get_tti()
 *
 *  Description:
 *  	get tti on/off option
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   TRUE: TTI ON
 *	   FALSE: OFF
 ***********************************************************************/
BOOLEAN fsu_get_tti(void)
{
    UINT8 isTTIOn = 0;

    Fax_API_T30Attribute(GET, FXA_TTI_ENABLE, &isTTIOn);

    return(isTTIOn);
}


#ifdef HAVE_IMAGELOG
/***********************************************************************
 *  Function: fsu_get_fax_forward_no()
 *
 *  Description:
 *  	get the fax forward no. save in DB
 *
 *  Inputs:
 *
 *  Output:
 *	   pointer to forward no.
 ***********************************************************************/
UINT8 *fsu_get_fax_forward_no(void)
{
    memset(gFaxForwardNo, 0x00, sizeof(gFaxForwardNo));

    Fax_API_T30Attribute(GET, FXA_FAXFORWARD_NO, gFaxForwardNo);

    return gFaxForwardNo;
}
#endif


/***********************************************************************
 *  Function: fsu_rcv_to_mem()
 *
 *  Description:
 *    prepare receive to memory
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   FALSE = FSU_RSP_TX_QUEUE_FULL
 *	   0 = OK
 ***********************************************************************/
FSU_RETURN_CODE fsu_rcv_to_mem(void)
{
    if (fsu_get_memory_full_status_nand() == 1)
    {
        printf("\n(YHCHO) %s() - NAND Full...\n", __FUNCTION__);

        return(FSU_MEMORY_FULL_NAND);
    }
    else
    {
        if (fsu_get_memory_full_status_ram() == 1)
        {
            printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);

            return(FSU_MEMORY_FULL_RAM);
        }
        else
        {
            if ( (isSpamFaxOn == 1) && (fsu_request_check_spam_fax() == 1) )
            {
                printf("\n(YHCHO) %s() - Spam Fax Detected...\n", __FUNCTION__);

                return(FSU_SPAM_FAX);
            }
            else
            {
                TX_JOB_t *p_rcv_job = (TX_JOB_t *)NULL;
                UINT8 isFaxForwardOn = DISABLE;
                UINT16 jnum = fsu_job_num_rcv;

                p_rcv_job = &txq.job[jnum];

                p_rcv_job->isDelay = FALSE;
                p_rcv_job->retry = 0;
                p_rcv_job->retry_tmr = 0;
                p_rcv_job->tx_time = TRANSMIT_TIME_RIGHT_NOW;
                p_rcv_job->res = fsu_get_resolution(jnum);
                p_rcv_job->width = fsu_get_width(jnum);

                Fax_API_T30Attribute(GET, FXA_FAXFORWARD, &isFaxForwardOn);

                if (isFaxForwardOn == ENABLE)
                {
                    p_rcv_job->ff_job = 1;
                }
                else
                {
                    p_rcv_job->ff_job = 0;
                }

                p_rcv_job->poll_transmit = 0;
                p_rcv_job->poll_receive = 0;
                p_rcv_job->broadcast = 0;
                p_rcv_job->flag_manual_tx = 0;
                p_rcv_job->PrintReason = PRINT_REASON_RECEIVE;
                p_rcv_job->photo_mode = 0;
                p_rcv_job->user_id[0] = 0x20; // Add one space
                p_rcv_job->user_id[1] = 0;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
				p_rcv_job->send_permission = SOLUTION_PERMISSION_STATE_ENABLE;
#endif
                p_rcv_job->DelayTimeInText[0] = 0x20; // Add one space..
                p_rcv_job->DelayTimeInText[1] = 0;

#if __HANA_FINANCIAL_GROUP_REQ_021__
                if(isSolutionFax)
                {
                    char TmpYear[5] = {0, };
                    char Year[3] = {0, };
                    //char CurrentLocalTime[MAX_LEN_FILE_PATH_NAME] = {0, };
                    char fileName[MAX_LEN_FILE_PATH_NAME] = {0, };
                    struct timeval CurrentTime;
                    struct tm *ptm;
                    //UINT8 Len = 0;

                    gettimeofday(&CurrentTime, NULL);

                    ptm = localtime(&CurrentTime.tv_sec);

                    snprintf(TmpYear, sizeof(TmpYear), "%04d", ptm->tm_year + 1900);

                    Year[0] = TmpYear[2];
                    Year[1] = TmpYear[3];
                    Year[2] = 0;

                    printf("\n(YHCHO) %s() - Year: %s\n", __FUNCTION__, Year);

                    if (ptm == NULL)
                    {
                        snprintf(fileName, sizeof(fileName), "%02d%02d%02d%02d%02d%02d%03d_%s_%s"
                                , 0, 1, 1, 0, 0, 0, 0,
                                gcTSI_for_RemoteFaxNum,
                                gRealFaxStationNo);
                    }
                    else
                    {
                        snprintf(fileName, sizeof(fileName), "%s%02d%02d%02d%02d%02d%03ld_%s_%s", 
                                Year, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, (CurrentTime.tv_usec/1000),
                                gcTSI_for_RemoteFaxNum,
                                gRealFaxStationNo);
                    }
                    printf("\n(YHCHO) %s() - fileName: %s\n", __FUNCTION__, fileName);

                    fsu_update_file_name_tx(jnum, fileName);
                }
#endif

                return FSU_OK;
            }
        }
    }
}


FSU_RETURN_CODE fsu_rcv_poll(FAXAPP_PARAMS_t *pFaxStruct)
{
    UINT8 retry = 0;
    UINT8 retry_tmr = 0;
    UINT8 isFaxForwardOn = DISABLE;
    UINT16 DocId = 0;
    TX_JOB_t *ptx_job = (TX_JOB_t *)NULL;

    printf("\n(YHCHO) %s()\n", __FUNCTION__);

    if (fsu_get_memory_full_status_nand() == 1)
    {
        printf("\n(YHCHO) %s() - NAND Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FSU_MEMORY_FULL_NAND;
    }

    if (fsu_get_memory_full_status_ram() == 1)
    {
        printf("\n(YHCHO) %s() - RAM Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FSU_MEMORY_FULL_RAM;
    }

    if (fsu_get_job_queue_full_status() == 1)
    {
        printf("\n(YHCHO) %s() - Job Queue Full...\n", __FUNCTION__);

        // Notification will be added to process this kind of abnormal case..

        return FSU_JOBQ_FULL;
    }

    fsu_job_num_rcv = txq.nxt;

    printf("\n(YHCHO) %s() - fsu_job_num_rcv: %d\n", __FUNCTION__, fsu_job_num_rcv);

    // Step 1. Point to the next txq.nxt for the next job
    set_txq_nxt ();

    ptx_job = &txq.job[fsu_job_num_rcv];

    // Step 2. Make DocXXXX directory
    // Step 2-1. Make /DocMan/DocXXXX/
    DocId = fsu_make_doc_dir_in_nand();

    // Step 2-2. Make /tmp/DocMan/DocXXXX
    fsu_make_doc_dir_in_ram(DocId);

    Fax_API_T30Attribute(GET, FXA_RETRY_NUMBER, &retry);
    Fax_API_T30Attribute(GET, FXA_RETRY_INTERVAL, &retry_tmr);

    copy_str(&pFaxStruct->dial_num[0][0], &txq.job[fsu_job_num_rcv].fax_num[0]);

    ptx_job->isDelay = FALSE;
    ptx_job->status = FSU_STATE_RECEIVING;
    ptx_job->retry = retry;
    ptx_job->retry_tmr = retry_tmr;
    ptx_job->tx_time = TRANSMIT_TIME_RIGHT_NOW;

    Fax_API_T30Attribute(GET, FXA_FAXFORWARD, &isFaxForwardOn);

    if (isFaxForwardOn == ENABLE)
    {
        ptx_job->ff_job = 1;
    }
    else
    {
        ptx_job->ff_job = 0;
    }

    ptx_job->poll_transmit = 0;
    ptx_job->poll_receive = 1;
    ptx_job->broadcast = 0;
    ptx_job->flag_manual_tx = 0;
    ptx_job->doc_id = DocId;
    ptx_job->original_doc_id = DEFAULT_ORIGINAL_DOC_ID;
    ptx_job->PrintReason = 0;
    ptx_job->photo_mode = 0;
    ptx_job->user_id[0] = 0x20; // Add one space
    ptx_job->user_id[1] = 0;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    ptx_job->send_permission = SOLUTION_PERMISSION_STATE_DISABLE;
#endif
    Create_Job_Queue_File(fsu_job_num_rcv, FAX_APP_JOB_TYPE_POLL_RECEIVE, DocId);

    set_tx_job_ready(fsu_job_num_rcv);

    return FSU_OK;
}


/***********************************************************************
 *  Function: fsu_set_curr_func_tx()
 *
 *  Description:
 *    set fsu_current_funcs to FAX_MEM_TO_SEND_BEGIN for tx polling job
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 ***********************************************************************/
void fsu_set_curr_func_tx(void)
{
    //printf("\n(YHCHO) %s()\n", __FUNCTION__);

    fsu_current_funcs &= ~FAX_RECEIVE_TO_MEM_BEGIN;
    fsu_current_funcs |= FAX_MEM_TO_SEND_BEGIN;
}


/***********************************************************************
 *  Function: fsu_set_curr_func_rx()
 *
 *  Description:
 *    set fsu_current_funcs to FAX_RECEIVE_TO_MEM for rx polling job
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 ***********************************************************************/
void fsu_set_curr_func_rx(void)
{
    //printf("\n(YHCHO) %s()\n", __FUNCTION__);

    fsu_current_funcs &= ~FAX_MEM_TO_SEND_BEGIN;
    fsu_current_funcs |= FAX_RECEIVE_TO_MEM_BEGIN;
}


/*********************************************************************************
 *  Function: set_tx_job_ready()
 *
 *  Description:
 *    set job status to FSU_STATE_SENDING_READY at the end of scan to mem function
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 *********************************************************************************/
void set_tx_job_ready(UINT16 jnum)
{
    if (fsu_get_flag_manual_tx(jnum) == 1)
    {
        // No Action Here..
    }
    else
    {
        if (fsu_get_poll_transmit(jnum) == 1)
        {
            fsu_update_status(jnum, FSU_STATE_SENDING_POLL_READY);
        }
        else
        {
            fsu_update_status(jnum, FSU_STATE_SENDING_READY);
        }
    }
}


/************************************************************************************
 *  Function: set_tx_retry_job_ready()
 *
 *  Description:
 *    set job status to FSU_STATE_RETRY_READY at the end of recovery from comm. error
 *
 *  Inputs:
 *    UINT16 jnum: job number to set retry
 *
 *  Output:
 *	   none
 ************************************************************************************/
void set_tx_retry_job_ready(UINT16 jnum)
{
    fsu_update_status(jnum, FSU_STATE_RETRY_READY);
}


/***********************************************************************
 *  Function: start_fsu_rcv_to_mem
 *
 *  Description:
 *  	start fsu_rcv_to_mem() and return error if any
 *
 *  Inputs: none
 *
 *  Output: TRUE - OK
 *			FALSE - error
 ***********************************************************************/
UINT8 start_fsu_rcv_to_mem(void)
{
    UINT8 result = fsu_rcv_to_mem();
    UINT8 ret = 0;

    switch (result)
    {
        case FSU_OK:
            ret = 0;
            break;

        case FSU_MEMORY_FULL_NAND:
            ret = 1;
            break;

        case FSU_MEMORY_FULL_RAM:
            ret = 2;
            break;

        case FSU_JOBQ_FULL:
            ret = 3;
            break;

        case FSU_SPAM_FAX:
            ret = 4;
            break;

        default:
            break;
    }

    return ret;
}


// --------------------------------------- //
// Find the next empty location in the txq //
// --------------------------------------- //
void set_txq_nxt (void)
{
    UINT16 i = TX_JOBS;

    while (i--)
    {
        if (++txq.nxt >= TX_JOBS)
            txq.nxt = 0;

        if (txq.job[txq.nxt].status == FSU_STATE_NULL)
            break;
    }
}


// ---------------------------------------- //
// Copy a string from source to destination //
// ---------------------------------------- //
void copy_str (UINT8 *src, UINT8 *dest)
{
    do
    {
        *dest++ = *src;
    } while (*src++ != 0);

    *dest = 0;
}


/***********************************************************************
 *  Function: fsu_fill_fax_header
 *
 *  Description:
 *		fill ASCII data on buffer for fax header
 *
 *  Input:
 *		UINT8 *p_header: buffer pointer
 *
 *	Output:
 *		none
 ***********************************************************************/
void fsu_fill_fax_header(UINT8 *p_header)
{
    UINT8 IndexToTTI = 0;
    UINT8 Lenth = 0;
    char Date[25] = {0, };
    char Time[20] = {0, };
    char Page[20] = {0, };
    struct timeval CurrentTime;
    struct tm *ptm;
    UINT8 dateType = getCountryDateType();
    UINT16 jnum = fsu_job_num_send;

    memset(p_header, 0x20, 255);

    gettimeofday(&CurrentTime, NULL);

    ptm = localtime(&CurrentTime.tv_sec);

    if (ptm == NULL)
    {
        if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
        {
            snprintf(Date, sizeof(Date), "%02d - %02d - %04d", 1, 1, 1900);
        }
        else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
        {
            snprintf(Date, sizeof(Date), "%02d - %02d - %04d", 1, 1, 1900);
        }
        else //FAX_COUNTRY_DATE_TYPE_YMD
        {
            snprintf(Date, sizeof(Date), "%04d - %02d - %02d", 1900, 1, 1);
        }

        snprintf (Time, sizeof(Time), "AM 12:%02d ", 0);
    }
    else
    {
        if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
        {
            snprintf(Date, sizeof(Date), "%02d - %02d - %04d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900);
        }
        else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
        {
            snprintf(Date, sizeof(Date), "%02d - %02d - %04d",  ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900);
        }
        else //FAX_COUNTRY_DATE_TYPE_YMD
        {
            snprintf(Date, sizeof(Date), "%04d - %02d - %02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);
        }
        //printf("\n(YHCHO) %s() - Date: %s\n", __FUNCTION__, Date);

        if (ptm->tm_hour == 0)
        {
            snprintf (Time, sizeof(Time), "AM 12:%02d ", ptm->tm_min);
        }
        else if ((ptm->tm_hour >= 1) && (ptm->tm_hour <= 11))
        {
            snprintf (Time, sizeof(Time), "AM %02d:%02d ", ptm->tm_hour, ptm->tm_min);
        }
        else if (ptm->tm_hour == 12)
        {
            snprintf (Time, sizeof(Time), "PM 12:%02d ", ptm->tm_min);
        }
        else if ((ptm->tm_hour >= 13) && (ptm->tm_hour <= 23))
        {
            snprintf (Time, sizeof(Time), "PM %02d:%02d ", ptm->tm_hour - 12, ptm->tm_min);
        }
    }

    //printf("\n(YHCHO) %s() - Time: %s\n", __FUNCTION__, Time);

    if (fsu_get_flag_manual_tx(jnum) == 1)
    {
        snprintf(Page, sizeof(Page), "Page: %03d", gFaxCurrentTTIPageNo);
    }
    else
    {
        UINT16 DocId = fsu_get_doc_id(jnum);

        if (gFaxCurrentTTIPageNo == 1)
        {
            if (fsu_get_broadcast(jnum) == 0)
            {
                if (fsu_get_photo_mode(jnum) == 1)
                {
                    TotalSendPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_PHOTO);
                }
                else
                {
                    TotalSendPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                }
            }
            else
            {
                UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);

                if (DocId != OriginalDocId)
                {
                    TotalSendPageNo = fsu_get_total_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                }
                else
                {
                    TotalSendPageNo = fsu_request_get_total_page_no_from_Job_Queue_File(DocId);
                }
            }
        }

        snprintf(Page, sizeof(Page), "Page: %03d of %03d", gFaxCurrentTTIPageNo, TotalSendPageNo);
    }

    //printf("\n(YHCHO) %s() - Page: %s\n", __FUNCTION__, Page);

    for (IndexToTTI = 0; IndexToTTI < LEFT_MAR; IndexToTTI++)
    {
        *p_header++ = ' ';
    }

    // Step 1. Date
    Lenth = strlen((char *)Date);

    for (IndexToTTI = 0; IndexToTTI < Lenth; IndexToTTI++)
    {
        *p_header++ = Date[IndexToTTI];
    }

    for (IndexToTTI = 0; IndexToTTI < MAR_BETWEEN_FIELD; IndexToTTI++)
    {
        *p_header++ = ' ';
    }

    // Step 2. Time
    Lenth = strlen((char *)Time);

    for (IndexToTTI = 0; IndexToTTI < Lenth; IndexToTTI++)
    {
        *p_header++ = Time[IndexToTTI];
    }

    for (IndexToTTI = 0; IndexToTTI < MAR_BETWEEN_FIELD; IndexToTTI++)
    {
        *p_header++ = ' ';
    }

    // Step 3. Station ID

    //printf("\n(YHCHO) %s() - gFaxStationID: %s\n", __FUNCTION__, gFaxStationID);

    Lenth = strlen((char *)gFaxStationID);

    if (Lenth == 0)
    {
        for (IndexToTTI = 0; IndexToTTI < (LEFT_MAR * 4); IndexToTTI++)
        {
            *p_header++ = ' ';
        }
    }
    else
    {
        for (IndexToTTI = 0; IndexToTTI < Lenth; IndexToTTI++)
        {
            *p_header++ = gFaxStationID[IndexToTTI];
        }

        for (IndexToTTI = 0; IndexToTTI < 40 - Lenth; IndexToTTI++)
        {
            *p_header++ = ' ';
        }
    }

    for (IndexToTTI = 0; IndexToTTI < MAR_BETWEEN_FIELD; IndexToTTI++)
    {
        *p_header++ = ' ';
    }

    // Step 4. Statioin Number

    //printf("\n(YHCHO) %s() - gFaxStationNo: %s\n", __FUNCTION__, gFaxStationNo);

    Lenth = strlen((char *)gFaxStationNo);

    if (Lenth == 0)
    {
        for (IndexToTTI = 0; IndexToTTI < (LEFT_MAR * 4); IndexToTTI++)
        {
            *p_header++ = ' ';
        }
    }
    else
    {
        for (IndexToTTI = 0; IndexToTTI < Lenth; IndexToTTI++)
        {
            *p_header++ = gFaxStationNo[IndexToTTI];
        }

        for (IndexToTTI = 0; IndexToTTI < 40 - Lenth; IndexToTTI++)
        {
            *p_header++ = ' ';
        }
    }

    for (IndexToTTI = 0; IndexToTTI < MAR_BETWEEN_FIELD; IndexToTTI++)
    {
        *p_header++ = ' ';
    }

    // Step 5. Page
    Lenth = strlen((char *)Page);

    for (IndexToTTI = 0; IndexToTTI < Lenth; IndexToTTI++)
    {
        *p_header++ = Page[IndexToTTI];
    }

    *p_header++ = ' ';

    *p_header = 0;
}


/***********************************************************************
 *  Function: fsu_gen_fax_header
 *
 *  Description:
 *		Generate fax header and store the bitmap on to destination address
 *
 *  Input:
 *		char *PBMFileName: .jbg file to transmit
 *		UINT8 faxRes: resolution
 *
 *	Output:
 *		none
 ***********************************************************************/
void fsu_gen_fax_header(char *PBMFileName, UINT8 faxRes, UINT8 fax_image_width_type)
{
    UINT8 *base = NULL;
    UINT32 stride = 0, h_size = 0;
    UINT8 repeat_factor, header_data[255] = {0, };
    UINT8 *p_data = &header_data[0];
    UFIX8 repeat, slice, byte_pos = 0;
    UFIX8 *dst, *src, *dst_start;
    UFIX16 size, count;
    FILE *fhandle = (FILE *)NULL;
    int c;
    char type;
    UFIX8 character, character_1, character_2;
    UINT8 temp_1_char, temp_2_char, temp_3_char;
    int half_size;


    fhandle = fopen(PBMFileName,"r+");
    if(fhandle == NULL)
    {
        printf(">>%s open error\n",PBMFileName);
        XASSERT(0,0);
    }

    if(fax_image_width_type == 0) //A4
    {
        if(faxRes == 0 || faxRes == 1)
        {
            stride = 216; // 1728/8;
        }
        else
        {
            stride = 324; // 2592/8;
        }
    }
    else if(fax_image_width_type == 1) //B4
    {
        if(faxRes == 0 || faxRes == 1)
        {
            stride = 256; // 2048/8;
        }
        else
        {
            stride = 384; // 3072/8;
        }
    }
    else //A3
    {
        if(faxRes == 0 || faxRes == 1)
        {
            stride = 304; // 2432/8;
        }
        else
        {
            stride = 456; // 3648/8;
        }
    }

    repeat_factor = RepGenRepeatTable[faxRes];

    memset(p_data, 0x20, 255);

    //printf(">> header string make start\n");

    fsu_fill_fax_header(header_data);

    //printf(">> header string make complete\n");

    size = strlen((char *)header_data);
    half_size = (int)(size >> 1);

    //printf("\n-------> size = %d, half_size = %d\n",size,half_size);

    h_size = RstrizeLinesPerRow * repeat_factor * stride;

    base = (UINT8 *)MEM_MALLOC(h_size);

    if( base == (UINT8 *)NULL )
    {
        XASSERT(0,0);
    }

    src = header_data;

    memset(base, 0x00, h_size);


    /* Obtain destination pointers in UFIX8 format to handle rasterizing 8 columns */
    count = 1;  /* Repeat factor is the first byte in the source */

    //dst_start = (UFIX8 *) (*base + RasterizerTopMargin * repeat_factor * stride);
    dst_start = &base[0];
    dst_start += RasterizerTopMargin * repeat_factor * stride;

    byte_pos = 0;

    if(faxRes == 0 || faxRes == 1)
    {
        /* Rasterize character by character */
        while (count++ < size)
        {
            character = *(src++);

            dst = (dst_start + byte_pos);

            character -= 0x20;    /* Convert from ASCII base */

            for (slice=0; slice < RasterizerNoOfCharSlices; slice++)
            {
                for (repeat=0; repeat<repeat_factor; repeat++)
                {
                    *dst = RasterizerCharSlices[slice][character];

                    dst += stride;
                }
            }

            dst_start++;
        } // while (count++ < size)
    }
    else
    {
        /* Rasterize character by character */
        while (count++ <= half_size)
        {
            character_1 = *(src++);
            character_1 -= 0x20;    /* Convert from ASCII base */

            character_2 = *(src++);
            character_2 -= 0x20;    /* Convert from ASCII base */

            dst = dst_start;

            for (slice=0; slice < RasterizerNoOfCharSlices; slice++)
            {
                for (repeat=0; repeat<repeat_factor; repeat++)
                {
                    temp_1_char = 0;
                    temp_2_char = 0;
                    temp_3_char = 0;

                    if(RasterizerCharSlices[slice][character_1] & 0x80) temp_1_char |= 0x80;
                    if(RasterizerCharSlices[slice][character_1] & 0x80) temp_1_char |= 0x40;
                    if(RasterizerCharSlices[slice][character_1] & 0x40) temp_1_char |= 0x20;
                    if(RasterizerCharSlices[slice][character_1] & 0x20) temp_1_char |= 0x10;
                    if(RasterizerCharSlices[slice][character_1] & 0x20) temp_1_char |= 0x08;
                    if(RasterizerCharSlices[slice][character_1] & 0x10) temp_1_char |= 0x04;
                    if(RasterizerCharSlices[slice][character_1] & 0x08) temp_1_char |= 0x02;
                    if(RasterizerCharSlices[slice][character_1] & 0x08) temp_1_char |= 0x01;

                    if(RasterizerCharSlices[slice][character_1] & 0x04) temp_2_char |= 0x80;
                    if(RasterizerCharSlices[slice][character_1] & 0x02) temp_2_char |= 0x40;
                    if(RasterizerCharSlices[slice][character_1] & 0x02) temp_2_char |= 0x20;
                    if(RasterizerCharSlices[slice][character_1] & 0x01) temp_2_char |= 0x10;

                    if(RasterizerCharSlices[slice][character_2] & 0x80) temp_2_char |= 0x08;
                    if(RasterizerCharSlices[slice][character_2] & 0x80) temp_2_char |= 0x04;
                    if(RasterizerCharSlices[slice][character_2] & 0x40) temp_2_char |= 0x02;
                    if(RasterizerCharSlices[slice][character_2] & 0x20) temp_2_char |= 0x01;

                    if(RasterizerCharSlices[slice][character_2] & 0x20) temp_3_char |= 0x80;
                    if(RasterizerCharSlices[slice][character_2] & 0x10) temp_3_char |= 0x40;
                    if(RasterizerCharSlices[slice][character_2] & 0x08) temp_3_char |= 0x20;
                    if(RasterizerCharSlices[slice][character_2] & 0x08) temp_3_char |= 0x10;
                    if(RasterizerCharSlices[slice][character_2] & 0x04) temp_3_char |= 0x08;
                    if(RasterizerCharSlices[slice][character_2] & 0x02) temp_3_char |= 0x04;
                    if(RasterizerCharSlices[slice][character_2] & 0x02) temp_3_char |= 0x02;
                    if(RasterizerCharSlices[slice][character_2] & 0x01) temp_3_char |= 0x01;

                    *dst++ = temp_1_char;
                    *dst++ = temp_2_char;
                    *dst++ = temp_3_char;
                    dst += (stride - 3);
                }
            }

            dst_start += 3;

        } // while (count++ < half_size)
    }

    if (fseek(fhandle, 0, SEEK_SET) != 0)
    {
        XASSERT(0,0);
    }

    /* read PBM header */
    while ((c = getc(fhandle)) != EOF && (isspace(c) || c == '#'))
        if (c == '#')
            while ((c = getc(fhandle)) != EOF && !(c == 13 || c == 10)) ;

    type = (char)getc(fhandle);

    if (c != 'P' || (type != '1' && type != '4')) {
        fprintf(stderr, "fsu_gen_fax_headerfsu_gen_fax_header : Input file does not look like a PBM file!\n");
        exit(1);
    }

    getint(fhandle); // wid
    getint(fhandle); // hig
    fgetc(fhandle);    /* skip line feed */

    fwrite(&base[0], h_size, 1, fhandle);

    MEM_FREE_AND_NULL(base);

    if (fclose(fhandle) != 0)
    {
        XASSERT(0,0);
    }
}


/* ASCII-to-Bitmap Font Table */
/*
 The following tables define the bitmap for a 9x7 font defined for
 ASCII codes 32-127 ($20-$7F).  The inter-character horizontal space
 is included at the right side of each character.  Each table (Slice0,
 Slice1, etc.) defines a horizontal slice through each character, with
 slice 0 at the top of the character, and slice 8 at the bottom.
*/
static UFIX8 RasterizerCharSlices[9][96] = {

                /* Slice 0 */   {0x0,0x18,0x36,0x14,0x8,0x61,0x1C,0xC,
                0x2,0x10,0x8,0x0,0x0,0x0,0x0,0x2,
                0x3E,0x18,0x3E,0x3E,0xC,0x7F,0x3E,0x7F,
                0x3E,0x3E,0x0,0x0,0x2,0x0,0x20,0x3C,
                0x3E,0x8,0x7E,0x3E,0x7E,0x7F,0x7F,0x3E,
                0x41,0x1C,0xE,0x42,0x40,0x41,0x41,0x3C,
                0x7C,0x3C,0x7E,0x3E,0x7F,0x41,0x41,0x41,
                0x41,0x41,0x7F,0xE,0x41,0x38,0x8,0x0,
                0xC,0x0,0x40,0x0,0x1,0x0,0xC,0x0,
                0x40,0x8,0x8,0x40,0x18,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x6,0x8,0x30,0x0,0x0},

                /*Slice 1 */    {0x0,0x18,0x36,0x14,0x3E,0x62,0x22,0xC,
                0x4,0x8,0x2A,0x8,0x0,0x0,0x0,0x2,
                0x43,0x28,0x41,0x41,0x14,0x40,0x41,0x1,
                0x41,0x41,0x0,0x0,0x4,0x0,0x10,0x42,
                0x41,0x14,0x41,0x41,0x21,0x40,0x40,0x41,
                0x41,0x8,0x4,0x44,0x40,0x63,0x61,0x42,
                0x42,0x42,0x41,0x41,0x8,0x41,0x41,0x41,
                0x22,0x41,0x2,0x8,0x22,0x8,0x1C,0x0,
                0xC,0x0,0x40,0x0,0x1,0x0,0x12,0x0,
                0x40,0x0,0x0,0x40,0x8,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x8,0x0,0x0,0x0,
                0x0,0x0,0x0,0x8,0x8,0x8,0x30,0x8},

                /* Slice 2 */   {0x0,0x18,0x36,0x7F,0x48,0x4,0x24,0xC,
                0x8,0x4,0x1C,0x8,0x0,0x0,0x0,0x4,
                0x45,0x8,0x2,0x1,0x24,0x40,0x40,0x2,
                0x41,0x41,0x18,0x18,0x8,0x3E,0x8,0x4,
                0x4D,0x22,0x41,0x40,0x21,0x40,0x40,0x40,
                0x41,0x8,0x4,0x48,0x40,0x55,0x51,0x42,
                0x42,0x42,0x41,0x40,0x8,0x41,0x41,0x41,
                0x14,0x22,0x4,0x8,0x14,0x8,0x22,0x0,
                0x8,0x3E,0x7E,0x3F,0x3F,0x3E,0x10,0x3E,
                0x7E,0x8,0x8,0x42,0x8,0x7E,0x3E,0x3E,
                0x7E,0x3E,0x5E,0x3F,0x3E,0x41,0x41,0x41,
                0x21,0x21,0x7F,0x8,0x8,0x8,0x49,0x14},

                /* Slice 3 */   {0x0,0x18,0x0,0x14,0x3E,0x8,0x18,0x0,
                0x8,0x4,0x7F,0x3E,0x0,0x3E,0x0,0x8,
                0x49,0x8,0xC,0x3E,0x44,0x7E,0x7E,0x4,
                0x3E,0x3F,0x18,0x18,0x10,0x0,0x4,0x8,
                0x5D,0x41,0x7E,0x40,0x21,0x7C,0x7C,0x40,
                0x7F,0x8,0x4,0x70,0x40,0x49,0x49,0x42,
                0x7C,0x42,0x7E,0x3E,0x8,0x41,0x22,0x49,
                0x8,0x1C,0x8,0x8,0x3E,0x8,0x41,0x0,
                0x4,0x1,0x41,0x40,0x41,0x41,0x38,0x41,
                0x41,0x8,0x8,0x4C,0x8,0x49,0x21,0x41,
                0x41,0x41,0x21,0x40,0x8,0x41,0x41,0x41,
                0x12,0x21,0x6,0x18,0x0,0xE,0x6,0x22},

                /* Slice 4 */   {0x0,0x0,0x0,0x7F,0x9,0x10,0x1B,0x0,
                0x8,0x4,0x1C,0x8,0x0,0x0,0x0,0x10,
                0x51,0x8,0x10,0x1,0x7F,0x1,0x41,0x8,
                0x41,0x1,0x0,0x0,0x8,0x3E,0x8,0x8,
                0x5E,0x7F,0x41,0x40,0x21,0x40,0x40,0x4F,
                0x41,0x8,0x4,0x68,0x40,0x41,0x45,0x42,
                0x40,0x42,0x44,0x1,0x8,0x41,0x14,0x5D,
                0x14,0x8,0x10,0x8,0x8,0x8,0x0,0x0,
                0x0,0x3F,0x41,0x40,0x41,0x7F,0x10,0x41,
                0x41,0x8,0x8,0x78,0x8,0x49,0x21,0x41,
                0x41,0x41,0x20,0x3E,0x8,0x41,0x22,0x2A,
                0xC,0x21,0x8,0x8,0x8,0x8,0x0,0x3E},

                /* Slice 5 */   {0x0,0x18,0x0,0x14,0x3E,0x23,0x26,0x0,
                0x4,0x8,0x2A,0x8,0x18,0x0,0x18,0x20,
                0x61,0x8,0x20,0x41,0x4,0x41,0x41,0x10,
                0x41,0x41,0x18,0x18,0x4,0x0,0x10,0x0,
                0x40,0x41,0x41,0x41,0x21,0x40,0x40,0x41,
                0x41,0x8,0x44,0x44,0x40,0x41,0x43,0x42,
                0x40,0x4A,0x42,0x41,0x8,0x41,0x8,0x63,
                0x22,0x8,0x20,0x8,0x3E,0x8,0x0,0x0,
                0x0,0x41,0x41,0x40,0x41,0x40,0x10,0x41,
                0x41,0x8,0x8,0x44,0x8,0x49,0x21,0x41,
                0x41,0x41,0x20,0x1,0x8,0x43,0x14,0x36,
                0x12,0x21,0x30,0x8,0x8,0x8,0x0,0x0},

                /* Slice 6 */   {0x0,0x18,0x0,0x14,0x8,0x43,0x46,0x0,
                0x2,0x10,0x8,0x0,0x18,0x0,0x18,0x20,
                0x3E,0x3E,0x7F,0x3E,0x4,0x3E,0x3E,0x10,
                0x3E,0x3E,0x18,0x18,0x2,0x0,0x20,0x8,
                0x3C,0x41,0x7E,0x3E,0x7E,0x7F,0x40,0x3E,
                0x41,0x1C,0x38,0x42,0x7E,0x41,0x41,0x3C,
                0x40,0x3C,0x41,0x3E,0x8,0x3E,0x8,0x41,
                0x41,0x8,0x7F,0xE,0x8,0x38,0x0,0x0,
                0x0,0x3F,0x7E,0x3F,0x3E,0x3E,0x10,0x3F,
                0x41,0x8,0x8,0x42,0x1C,0x49,0x21,0x3E,
                0x7E,0x3F,0x20,0x7E,0xC,0x3D,0x8,0x22,
                0x21,0x1F,0x7F,0x6,0x8,0x30,0x0,0x0},

                /* Slice 7 */   {0x0,0x0,0x0,0x0,0x0,0x0,0x39,0x0,
                0x0,0x0,0x0,0x0,0x8,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7F,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,
                0x0,0x0,0x48,0x0,0x0,0x0,0x0,0x0,
                0x40,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0},

                /* Slice 8 */   {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x10,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x10,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3E,
                0x0,0x0,0x30,0x0,0x0,0x0,0x0,0x0,
                0x40,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
                0x0,0x3E,0x0,0x0,0x0,0x0,0x0,0x0}

};


/***********************************************************************
 *  Function: fsu_make_doc_dir_in_nand()
 *
 *  Description:
 *    Make /DocMan/DocXXXX/ directory
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
UINT16 fsu_make_doc_dir_in_nand(void)
{
    DIR *pDir = (DIR *)NULL;
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    UINT16 IndexToDocId = 0;

    for (IndexToDocId = 0; IndexToDocId < MAX_NUM_DOC_ID; IndexToDocId++)
    {
        snprintf(PathName, sizeof(PathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, IndexToDocId);

        pDir = opendir(PathName);
        if (pDir == (DIR *)NULL)
        {
            break;
        }
        else
        {
            closedir(pDir);
        }
    }

    if (mkdir(PathName, 0777))
    {
        XASSERT(0,0);
    }

    return IndexToDocId;
}


/***********************************************************************
 *  Function: fsu_make_doc_dir_in_ram()
 *
 *  Description:
 *    Make /tmp/DocMan/DocXXXX/ directory
 *
 *  Inputs:
 *    UINT16 DocId: document id. to create
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_make_doc_dir_in_ram(UINT16 DocId)
{
    DIR *pDir = (DIR *)NULL;
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };

    snprintf(PathName, sizeof(PathName), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

    pDir = opendir(PathName);

    if (pDir == (DIR *)NULL)
    {
        // If does not exist, make that directory.
        if (mkdir(PathName, 0777))
        {
            printf("\n(YHCHO) %s() - errorno: %d, PathName: %s\n", __FUNCTION__, errno, PathName);

            XASSERT(0,0);
        }
    }
    else
    {
        // If exists, just close...
        closedir(pDir);
    }
}


/***********************************************************************
 *  Function: fsu_make_doc_man_dir_in_nand()
 *
 *  Description:
 *    Make /DocMan/ directory unless exists
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_make_doc_man_dir_in_nand (void)
{
    DIR *pDir = (DIR *)NULL;
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };

    snprintf(PathName, sizeof(PathName), "%s", pRootFolderName);

    pDir = opendir(PathName);
    if (pDir == (DIR *)NULL)
    {
        if (mkdir(PathName, 0777))
        {
            XASSERT(0,0);
        }
    }
    else
    {
        while(1)
        {
            if (isFaxAppReady == 1)
                break;

            tx_thread_sleep(100);
        }

        // Check to see whether Job Queue File (JobQueueXXXX.info) exists
        ValidJobCnt = check_job_queue_validation();

        printf("\n(YHCHO) %s() - ValidJobCnt: %d\n", __FUNCTION__, ValidJobCnt);

        closedir(pDir);
    }
}

void fsu_check_sdcard_enable(void)
{
	if (access("/dev/mmcblk0p1", F_OK) == 0)
	{
		printf("\n(YHCHO) %s() SD card enabled !!\n", __FUNCTION__);
		isSDCardEnable = 1;
		pRootFolderARIA = "/tmp/DocMan";
	}
	else
	{
		pRootFolderName = "/DocMan";
		pRootFolderNameSolDocMan = "/SolDocMan";
		
		printf("\n(YHCHO) %s() SD card disabled !!\n", __FUNCTION__);
	}
}


/***********************************************************************
 *  Function: fsu_make_doc_man_dir_in_ram()
 *
 *  Description:
 *    Make /tmp/DocMan/ directory unless exists
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_make_doc_man_dir_in_ram (void)
{
    DIR *pDir = (DIR *)NULL;
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };

    snprintf(PathName, sizeof(PathName), "%s", pRootFolderNameRAM);

    pDir = opendir(PathName);
    if (pDir == (DIR *)NULL)
    {
        if (mkdir(PathName, 0777))
        {
            XASSERT(0,0);
        }
    }
    else
    {
        // If exists, just close.
        closedir(pDir);
    }
}


/***********************************************************************
 *  Function: fsu_make_sol_doc_man_dir_in_nand()
 *
 *  Description:
 *    Make /SolDocMan/ directory unless exists
 *
 *  Inputs:
 *    none
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_make_sol_doc_man_dir_in_nand (void)
{
    DIR *pDir = (DIR *)NULL;
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };

    snprintf(PathName, sizeof(PathName), "%s", pRootFolderNameSolDocMan);

    pDir = opendir(PathName);

    if (pDir == (DIR *)NULL)
    {
        if (mkdir(PathName, 0777))
        {
            XASSERT(0,0);
        }
    }
    else
    {
        faxapp_solution_job_begin();

        closedir(pDir);
    }
}


/***********************************************************************
 *  Function: fsu_cancel_fax_job()
 *
 *  Description:
 *    cancel fax job
 *
 *  Inputs:
 *    job_type
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_cancel_fax_job(char job_type)
{
    gFaxT30Result = FAX_ERROR_USER_ABORT_ERROR;

    printf("\n(YHCHO) %s() - job_type = %d\n", __FUNCTION__, job_type);
    
    if(job_type == FAX_CANCEL_JOB_TYPE_SCAN)
    {
		if(fsu_get_flag_manual_tx(fsu_job_num_scan) == 1)
		{
			job_type = FAX_CANCEL_JOB_TYPE_TX;
			printf("\n(YHCHO) %s() - change job_type = %d\n", __FUNCTION__, job_type);
		}
	}

    if(job_type == FAX_CANCEL_JOB_TYPE_SCAN) //Scan
    {
        gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_SCAN_CANCEL;

        if (gFaxScanSource == FAX_SCAN_SOURCE_FB)
        {
            FBMultiPageScan = FB_MULTI_PAGE_SCAN_INIT;
        }

        if(fsu_get_scan_busy_flag() == 1)
        {
            faxapp_scan_cancel();
        }
        else
        {
            printf("\n(YHCHO) %s() - scan already Done !! Just Bypass (%d)\n", __FUNCTION__, fsu_job_num_scan);
        }
    }
    else if(job_type == FAX_CANCEL_JOB_TYPE_TX) //TX
    {
#if 0
        if (gFaxEndOfFax == 1)
        {
            printf("\n(YHCHO) %s() - Transmit Already Done! Just Bypass...\n", __FUNCTION__);

            return;
        }
#endif
        Fax_API_CancelFax(TRUE, fsu_job_num_send);
    }
    else if(job_type == FAX_CANCEL_JOB_TYPE_RX)//RX
    {
#if 0
        if (gFaxEndOfFax == 1)
        {
            printf("\n(YHCHO) %s() - Receive Already Done! Just Bypass...\n", __FUNCTION__);

            return;
        }
#endif
        Fax_API_CancelFax(TRUE, fsu_job_num_rcv);
    }
    else if(job_type == FAX_CANCEL_JOB_TYPE_PRINT) //PRINT
    {
        gFaxAbnormalFlag = FAX_ABNORMAL_FLAG_PRINT_CANCEL;

        faxapp_print_user_input();
    }
}


/***********************************************************************
 *  Function: fsu_delete_pending_job()
 *
 *  Description:
 *    delete pending fax job
 *
 *  Inputs:
 *    job_num
 *
 *  Output:
 *	   none
 **********************************************************************/
void fsu_delete_pending_job(UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;

    printf("\n(YHCHO) %s() - jnum: %d\n", __FUNCTION__, jnum);

    if(jnum >= TX_JOBS)
    {
        printf("\n(YHCHO) %s() - invalid jnum !!\n", __FUNCTION__);
        return;
    }

    p_tx_job = &txq.job[jnum];

    if(p_tx_job->status == FSU_STATE_NULL || p_tx_job->status == FSU_STATE_SCANNING || p_tx_job->status == FSU_STATE_RECEIVING
        || p_tx_job->status == FSU_STATE_SENDING_PENDING || p_tx_job->status == FSU_STATE_PRINT_READY)
    {
        printf("\n(YHCHO) %s() - pending job delete fail 1! jnum status: %d\n", __FUNCTION__, p_tx_job->status);
        return;
    }
    else if(p_tx_job->status == FSU_STATE_SENDING_READY || p_tx_job->status == FSU_STATE_RETRY_READY)
    {
        if((fsu_current_funcs & FAX_MEM_TO_SEND_DIALING || fsu_current_funcs & FAX_MEM_TO_SEND_BEGIN) && (jnum == fsu_job_num_send))
        {
            printf("\n(YHCHO) %s() - pending job delete fail 2! jnum status: %d\n", __FUNCTION__, p_tx_job->status);
            return;
        }
    }
    else if(p_tx_job->status == FSU_STATE_RECEIVE_DONE)
    {
        if((fsu_current_funcs & FAX_MEM_TO_PRINT) && (jnum == fsu_job_num_prn))
        {
            printf("\n(YHCHO) %s() - pending job delete fail 3! jnum status: %d\n", __FUNCTION__, p_tx_job->status);
            return;
        }
    }
    else if ( (p_tx_job->status == FSU_STATE_SENDING_POLL_READY) && (p_tx_job->poll_transmit == 1) )
    {
         p_tx_job->poll_transmit = 0;
    }

    p_tx_job->status = FSU_STATE_NULL;

    // Step 2. Purge all image files and /DocMan/DocXXXX/ depending on some condition...
    if (p_tx_job->broadcast == 1)
    {
        fsu_update_broadcast_result_report_struct(jnum, BROADCAST_RESULT_REPORT_REASON_JOB_ID);

        fsu_update_broadcast_result_report_struct(0, BROADCAST_RESULT_REPORT_REASON_DURATION);

        fsu_update_broadcast_result_report_struct(fsu_get_tx_total_page_no(jnum), BROADCAST_RESULT_REPORT_REASON_TOTAL_PAGE_NO);

		if (p_tx_job->doc_id != p_tx_job->original_doc_id)
		{
			fsu_update_broadcast_result_report_struct(fsu_get_start_page_no_in_mem(p_tx_job->doc_id, FAX_FILE_TYPE_JBG) - 1, 
															BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO);
		}
		else
		{
			fsu_update_broadcast_result_report_struct(fsu_requset_get_current_page_no_from_Job_Queue_File(jnum),
															BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO);
		}

        fsu_update_broadcast_result_report_struct(FAX_ERROR_USER_ABORT_ERROR, BROADCAST_RESULT_REPORT_REASON_RESULT);

        fsu_request_write_broadcast_result_report_info_to_db();

        if (p_tx_job->doc_id != p_tx_job->original_doc_id)
        {
            fsu_purge_doc_dir(jnum);
        }
        else
        {
            Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT, jnum, FSU_RETRANSMIT_NO_NEED_RETRANSMIT);
        }

        gFax_BC_Num[p_tx_job->original_doc_id].BC_Current_Num++;

        printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Current_Num: %d", __FUNCTION__, p_tx_job->original_doc_id, gFax_BC_Num[p_tx_job->original_doc_id].BC_Current_Num);
        printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Total_Num: %d\n", __FUNCTION__, p_tx_job->original_doc_id, gFax_BC_Num[p_tx_job->original_doc_id].BC_Total_Num);

        if (gFax_BC_Num[p_tx_job->original_doc_id].BC_Current_Num >= gFax_BC_Num[p_tx_job->original_doc_id].BC_Total_Num)
        {
            printf("\n(YHCHO) %s() - There is no more recipient to transmit... So PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[p_tx_job->original_doc_id][0]));

            fsu_purge_doc_dir(gFaxBroadcastJobInfo[p_tx_job->original_doc_id][0]);

            faxapp_process_broadcast_result_report(jnum);
        }
        else
        {
            printf("\n(YHCHO) %s() - There are more recipients to transmit... So DO NOT PURGE /DocMan/Doc%04X\n", __FUNCTION__, fsu_get_original_doc_id(gFaxBroadcastJobInfo[p_tx_job->original_doc_id][0]));
        }
    }
    else
    {
        fsu_purge_doc_dir(jnum);
    }

    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, 0, 0, 0, jnum);
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: fsu_delete_solution_job()
 *
 *  Description:
 *    delete Solution fax job
 *
 *  Inputs:
 *    job_num
 *
 *  Output:
 *	   none
 **********************************************************************/
int fsu_delete_solution_job(UINT16 jnum)
{
    TX_JOB_t *p_tx_job = (TX_JOB_t *)NULL;
    TX_JOB_t *p_tx_job_for_broadcast = (TX_JOB_t *)NULL;

   printf("\n(YHCHO) %s() - jnum: %d\n", __FUNCTION__, jnum);
    
    if(jnum >= TX_JOBS)
    {
		printf("\n(YHCHO) %s() - invalid jnum !!\n", __FUNCTION__);
		return FAIL;
	}

    p_tx_job = &txq.job[jnum];

    if(p_tx_job->status == FSU_STATE_NULL || p_tx_job->status == FSU_STATE_SCANNING || p_tx_job->status == FSU_STATE_RECEIVING 
		|| p_tx_job->status == FSU_STATE_SENDING_PENDING || p_tx_job->status == FSU_STATE_PRINT_READY)
	{
		printf("\n(YHCHO) %s() - pending job delete fail 1! jnum status: %d\n", __FUNCTION__, p_tx_job->status);
		return FAIL;
	}
	else if(p_tx_job->status == FSU_STATE_SENDING_READY || p_tx_job->status == FSU_STATE_RETRY_READY)
	{
		if((fsu_current_funcs & FAX_MEM_TO_SEND_DIALING || fsu_current_funcs & FAX_MEM_TO_SEND_BEGIN) && (jnum == fsu_job_num_send))
		{
			printf("\n(YHCHO) %s() - pending job delete fail 2! jnum status: %d\n", __FUNCTION__, p_tx_job->status);
			return FAIL;
		}
	}
	else if(p_tx_job->status == FSU_STATE_RECEIVE_DONE)
	{
		if((fsu_current_funcs & FAX_MEM_TO_PRINT) && (jnum == fsu_job_num_prn))
		{
			printf("\n(YHCHO) %s() - pending job delete fail 3! jnum status: %d\n", __FUNCTION__, p_tx_job->status);
			return FAIL;			
		}
	}
    else if ( (p_tx_job->status == FSU_STATE_SENDING_POLL_READY) && (p_tx_job->poll_transmit == 1) )
    {
         p_tx_job->poll_transmit = 0;
    }

    // Step 2. Purge all image files and /DocMan/DocXXXX/ depending on some condition...
    if (p_tx_job->broadcast == 1)
    {
		int i = 0;
		UINT16 originalDocId = p_tx_job->original_doc_id;
		for(i=0; i< gFax_BC_Num[originalDocId].BC_Total_Num; i++)
		{
			UINT16 broadcast_jnum = gFaxBroadcastJobInfo[originalDocId][i];

			p_tx_job_for_broadcast = &txq.job[broadcast_jnum];
			
			if(p_tx_job_for_broadcast->status != FSU_STATE_NULL)
			{
				p_tx_job_for_broadcast->status = FSU_STATE_NULL;

				//memset(p_tx_job_for_broadcast->file_name_tx, 0, sizeof(p_tx_job_for_broadcast->file_name_tx));
				
				fsu_update_broadcast_result_report_struct(broadcast_jnum, BROADCAST_RESULT_REPORT_REASON_JOB_ID);

				fsu_update_broadcast_result_report_struct(0, BROADCAST_RESULT_REPORT_REASON_DURATION);

				fsu_update_broadcast_result_report_struct(fsu_get_tx_total_page_no(broadcast_jnum), BROADCAST_RESULT_REPORT_REASON_TOTAL_PAGE_NO);

				fsu_update_broadcast_result_report_struct(0, BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO);

				fsu_update_broadcast_result_report_struct(FAX_ERROR_USER_ABORT_ERROR, BROADCAST_RESULT_REPORT_REASON_RESULT);

				fsu_request_write_broadcast_result_report_info_to_db();

				if (p_tx_job_for_broadcast->doc_id != originalDocId)
				{
					fsu_purge_doc_dir(broadcast_jnum);
				}
				else
				{
					Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT, broadcast_jnum, FSU_RETRANSMIT_NO_NEED_RETRANSMIT);
				}

				gFax_BC_Num[originalDocId].BC_Current_Num++;

				printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Current_Num: %d", 
					__FUNCTION__, originalDocId, gFax_BC_Num[originalDocId].BC_Current_Num);
				printf("\n(YHCHO) %s() - gFax_BC_Num[%d].BC_Total_Num: %d\n", 
					__FUNCTION__, originalDocId, gFax_BC_Num[originalDocId].BC_Total_Num);

				if (gFax_BC_Num[originalDocId].BC_Current_Num >= gFax_BC_Num[originalDocId].BC_Total_Num)
				{
					printf("\n(YHCHO) %s() - There is no more recipient to transmit... So PURGE mmt/sd/DocMan/Doc%04X\n", 
						__FUNCTION__, originalDocId);

					fsu_purge_doc_dir(gFaxBroadcastJobInfo[originalDocId][0]);

					faxapp_process_broadcast_result_report(broadcast_jnum);
				}
				else
				{
					printf("\n(YHCHO) %s() - There are more recipients to transmit... So DO NOT PURGE mmt/sd/DocMan/Doc%04X\n", 
						__FUNCTION__, originalDocId);
				}

				faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, 0, 0, 0, broadcast_jnum);
				faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_JOB_CANCELED, 0, 0, 0, 0, broadcast_jnum);
			}
		}
    }
    else
    {
		p_tx_job->status = FSU_STATE_NULL;

		//memset(p_tx_job->file_name_tx, 0, sizeof(p_tx_job->file_name_tx));

        fsu_purge_doc_dir(jnum);

        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, 0, 0, 0, jnum);
        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_JOB_CANCELED, 0, 0, 0, 0, jnum);
    }
    
    return OK;
}
#endif


UINT16 fsu_get_job_mem(int * num)
{
    UINT16 IndexToJobNo = 0;
    int ok = 0;

    for (IndexToJobNo = *num; IndexToJobNo < TX_JOBS; IndexToJobNo++)
    {
        if ( (txq.job[IndexToJobNo].status == FSU_STATE_SENDING_POLL_READY) ||
             (txq.job[IndexToJobNo].status == FSU_STATE_SENDING_READY) ||
             (txq.job[IndexToJobNo].status == FSU_STATE_RECEIVING) ||
             (txq.job[IndexToJobNo].status == FSU_STATE_RECEIVE_DONE) ||
             (txq.job[IndexToJobNo].status == FSU_STATE_PRINT_READY) ||
             (txq.job[IndexToJobNo].status == FSU_STATE_RETRY_READY) ||
             (txq.job[IndexToJobNo].status == FSU_STATE_SENDING_PENDING) )
        {
            *num = IndexToJobNo + 1;
            ok = 1;
            break;
        }
    }

    if(!ok)
    {
        printf("\n(YHCHO) %s() - no more job\n", __FUNCTION__);

        return TX_JOBS+1;
    }

    return IndexToJobNo;
}


UINT16 fsu_get_next_prn_num(void)
{
    UINT return_value = 0;
    if(next_prn_job_check == last_prn_job_num_check)
    {
        return TX_JOBS+1;
    }
    return_value = fsu_prn_num_array[next_prn_job_check++];
    if(next_prn_job_check >= TX_JOBS)
    {
        next_prn_job_check = 0;
    }

    printf("\n(YHCHO) %s() - next_prn_job_check : %d, return_value = %d,\n", __FUNCTION__, next_prn_job_check, return_value);

    return return_value;
}

void fsu_delete_last_prn_num(UINT16 temp_job_num)
{
    if(temp_job_num == fsu_prn_num_array[(last_prn_job_num_check-1)])
    {
        if(last_prn_job_num_check == 0)
        {
            last_prn_job_num_check = (TX_JOBS - 1);
        }
        else
        {
            last_prn_job_num_check--;
        }
    }
}

void fsu_set_next_prn_num(UINT16 next_prn_num)
{
    printf("\n(YHCHO) %s() - last_prn_job_num_check : %d, next_prn_num = %d,\n", __FUNCTION__, last_prn_job_num_check, next_prn_num);

    fsu_prn_num_array[last_prn_job_num_check] = next_prn_num;
    last_prn_job_num_check++;

    if(last_prn_job_num_check >= TX_JOBS)
    {
        last_prn_job_num_check = 0;
    }
}


void fsu_update_rx_docman(int result_prn_job, int prn_page)
{
    static int prn_page_count = 0;
    static int printed_image_num = 0;
    static UINT16 del_job_num = 0;
    int prn_page_num = 0;
    char JBGFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    UINT16 DocId = 0;

    if(del_job_num != fsu_prn_num_array[current_prn_job_num])
    {
        del_job_num = fsu_prn_num_array[current_prn_job_num];
        prn_page_count = 0;
        printed_image_num = 0;
    }

    DocId = fsu_get_doc_id(del_job_num);

    if(result_prn_job == FAX_PRINT_RESULT_PRINTING || (result_prn_job == FAX_PRINT_RESULT_CANCEL && getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_EVERY_PAGE))
    {
        if(result_prn_job == FAX_PRINT_RESULT_CANCEL )
        {
            if(!fsu_get_isDuplex(del_job_num))
            {
                prn_page = 1;
            }
            else
            {
                printf("\n(YHCHO) %s() - Total page : %d, printed : %d\n", __FUNCTION__,fsu_get_total_pages(del_job_num), printed_image_num);
                if((fsu_get_total_pages(del_job_num) - printed_image_num ) > 1)
                {
                    prn_page = 2;
                }
                else
                {
                    prn_page = 1;
                }
            }
        }

        if(prn_page == 0)
        {
            printf("\n(YHCHO) %s() - The Number of printed page is zero!!\n", __FUNCTION__);

            return;
        }

        prn_page_count += prn_page;

        if(fsu_get_split_print(del_job_num))
        {
            if(prn_page_count == 1)
            {
                printf("\n(YHCHO) %s() - do not delete image: need 2page print\n", __FUNCTION__);
                return; //
            }
            else if(prn_page_count == 2)
            {
                printf("\n(YHCHO) %s() - 1 Image Print Complete\n", __FUNCTION__);
                prn_page = 1;
                prn_page_count = 0;
            }
        }
        else
        {
            prn_page_count = 0;
        }

        if(prn_page != 1 && prn_page != 2)
        {
            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAX_PRINT_RESULT_ERROR);
        }

        //fsu_get_total_pages(del_job_num);
        if(!fsu_get_isDuplex(del_job_num)) //Simplex
        {
            if(prn_page == 1)
            {
                prn_page_num = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, prn_page_num);
                unlink(JBGFileName);
                printf("\n(YHCHO) %s() - delete jbg file : %s\n", __FUNCTION__, JBGFileName);

                fsu_get_memory_full_status_nand();

                printed_image_num++;
            }
            else
            {
                ASSERT(0);
            }
        }
        else //Duplex
        {
            if(prn_page == 1)
            {
                prn_page_num = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, prn_page_num);
                unlink(JBGFileName);
                printf("\n(YHCHO) %s() - delete jbg file : %s\n", __FUNCTION__, JBGFileName);

                fsu_get_memory_full_status_nand();

                printed_image_num++;
            }
            else if(prn_page == 2)
            {
                prn_page_num = fsu_get_start_page_no_in_mem(DocId, FAX_FILE_TYPE_JBG);
                snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, prn_page_num);
                unlink(JBGFileName);
                printf("\n(YHCHO) %s() - delete jbg file : %s\n", __FUNCTION__, JBGFileName);

                prn_page_num++;
                snprintf(JBGFileName, sizeof(JBGFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, prn_page_num);
                unlink(JBGFileName);
                printf("\n(YHCHO) %s() - delete jbg file : %s\n", __FUNCTION__, JBGFileName);

                fsu_get_memory_full_status_nand();

                printed_image_num += 2;
            }
        }
    }

    if(result_prn_job == FAX_PRINT_RESULT_COMPLETE || result_prn_job == FAX_PRINT_RESULT_CANCEL)
    {
        if(getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_EVERY_PAGE)
        {
            if(fsu_get_print_reason(del_job_num) == PRINT_REASON_RECEIVE)
            {
                if(fsu_get_status(del_job_num) != FSU_STATE_RECEIVE_DONE || printed_image_num != fsu_get_total_pages(del_job_num))
                {
                    printf("\n(YHCHO) %s() - print every mode1: not yet all image printed\n", __FUNCTION__);
                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PAGE_PRINT_END, 0, 0, 0, 0, del_job_num);

                    return;
                }
            }
            else
            {
                if(printed_image_num != fsu_get_total_pages(del_job_num))
                {
                    printf("\n(YHCHO) %s() - print every mode2: not yet all image printed\n", __FUNCTION__);
                    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PAGE_PRINT_END, 0, 0, 0, 0, del_job_num);

                    return;
                }
            }
        }

        fsu_purge_doc_dir(del_job_num);

        if(prn_cancel_flag == TRUE && del_job_num == fsu_job_num_prn)
        {
            del_job_pending = 1;
        }
        else
        {
            fsu_set_prn_job_done(del_job_num);
        }

        if(result_prn_job == FAX_PRINT_RESULT_CANCEL)
        {
            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, 0, 0, 0, del_job_num);
        }
        else //complete
        {
            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PRINTING_COMPLETE, 0, 0, 0, 0, del_job_num);
        }

        fsu_get_memory_full_status_nand();

        current_prn_job_num++;
        if(current_prn_job_num >= TX_JOBS)
        {
            current_prn_job_num = 0;
        }
    }
}

void fsu_fax_print_job_done(void)
{
    UINT16 del_print_job = 0;
    del_print_job = fsu_prn_num_array[current_prn_job_num];

    fsu_purge_doc_dir(del_print_job);
    fsu_set_prn_job_done(del_print_job);

    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PRINTING_COMPLETE, 0, 0, 0, 0, del_print_job);

    fsu_get_memory_full_status_nand();

    current_prn_job_num++;
    if(current_prn_job_num >= TX_JOBS)
    {
        current_prn_job_num = 0;
    }
}

void fsu_fax_print_job_clear(void)
{
    UINT16 del_print_job = 0;
    UINT8 TotalPageNoToPrintJob = 0;
    char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };

    del_print_job = fsu_prn_num_array[current_prn_job_num];

    snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(del_print_job));

    if(fsu_check_doc_man_dir_in_nand(DocDirPathNAND) == FALSE)
    {
        printf("\n(YHCHO) %s() - already clear %s\n", __FUNCTION__,DocDirPathNAND);
        return;
    }

    TotalPageNoToPrintJob = fsu_get_start_page_no_in_mem(fsu_get_doc_id(del_print_job), FAX_FILE_TYPE_JBG);

    if(TotalPageNoToPrintJob != 0)
    {
        printf("\n(YHCHO) %s() - print page remain\n", __FUNCTION__);
        return;
    }

    fsu_purge_doc_dir(del_print_job);
    fsu_set_prn_job_done(del_print_job);

    faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_PRINTING_COMPLETE, 0, 0, 0, 0, del_print_job);

    fsu_get_memory_full_status_nand();

    current_prn_job_num++;
    if(current_prn_job_num >= TX_JOBS)
    {
        current_prn_job_num = 0;
    }

    printf("\n(YHCHO) %s() - clear complete\n", __FUNCTION__);
}

void fsu_delete_docman(UINT8 directory)
{
    UINT16 IndexToDocId = 0;
    DIR *pDir = (DIR *)NULL;
    struct dirent *FileInfo = NULL;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char *pCurrentDirName = ".";
    char *pPreviousDirName = "..";

    // Step 1. Search for all the /DocMan/DocXXXX...
    for (IndexToDocId = 0; IndexToDocId < MAX_NUM_DOC_ID; IndexToDocId++)
    {
        if(directory == 0)
        {
            snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, IndexToDocId);
        }
        else
        {
            snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cSolDoc%04X", pRootFolderName, PATH_SEPARATOR, IndexToDocId);
        }
        pDir = opendir(DocIdPathName);

        // Step 2. Check to see whether the /DocMan/DocXXXX exists...
        if (pDir == (DIR *)NULL)
        {
            ; // No Action Here...
        }
        else
        {
            // Step 3. Delete all file names to JbgFileName except for '.' or '..'
            while ( (FileInfo = readdir(pDir)) != NULL )
            {
                // Step 3-1. '.' or '..'
                if (  (strcmp (FileInfo->d_name, pCurrentDirName) == 0) || (strcmp (FileInfo->d_name, pPreviousDirName) == 0) )
                {
                    ; // No Action Here...
                }
                // Step 3-2. Delete File
                else
                {
                    memset(jbgFileName, 0x00, sizeof(jbgFileName));
                    if(directory == 0)
                    {
                        snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%c%s",
                                    pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, FileInfo->d_name);
                    }
                    else
                    {
                        snprintf(jbgFileName, sizeof(jbgFileName), "%s%cSolDoc%04X%c%s",
                                    pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, FileInfo->d_name);
                    }

                    fsu_delete_file_on_file_system(jbgFileName);
                }
            }
            // Step 4. Close the /DocMan/DocXXXX
            closedir(pDir);

            //Step 5. Delete /DocMan/DocXXXX
            fsu_delete_dir_on_file_system (DocIdPathName);
        }
    }

    gFaxRebootCnt++;
}
