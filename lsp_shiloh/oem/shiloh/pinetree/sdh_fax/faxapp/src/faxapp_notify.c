#include <stdio.h>
#include <string.h>
#include "ATypes.h"
#include "lassert.h"
#include "agMessage.h"
#include "agRouter.h"
#include "faxapp_notify.h"
#include "faxapp_job.h"
#include "SysUI_Interface.h"
#include "fsu.h"
#ifdef HAVE_IMAGELOG
#include "imagelog.h"
#endif

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
char gFaxLogInID[MAX_LEN_USER_ID + 1] = {0, };
#endif
stIC_JobDoneInfo g_faxScanInfo;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern char *fsu_request_get_user_id_from_Job_Queue_File (UINT16 DocId);
extern void fsu_update_remote_no_for_solution (UINT8 type, UINT16 jnum);
extern void fsu_update_file_name_for_solution (UINT16 jnum, int last_job, int solution_result);
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern char *fsu_get_file_name_tx (UINT16 jnum);
extern UINT8 fsu_get_broadcast (UINT16 jnum);
extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern void FaxSetPhoneNumber(int FaxJobType, char *FaxNumber);
#ifdef HAVE_IMAGELOG
extern void save_fax_jobinfo(int _jobid, int _faxpage, int _faxresult, char *faxnum);
#endif
extern UINT8 *fsu_get_dial_no (UINT16 jnum);
extern UINT32 fsu_get_unique_job_id(UINT16 jnum);
extern void fsu_clear_file_name_tx (UINT16 jnum);
extern char *fsu_get_user_id (UINT16 jnum);
#endif
extern void fsu_update_gfaxScanInfo (UINT16 jnum);

extern UINT8 isDKSolution;
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];

void faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_t StatusType, int CurrentPageNo, int TotalPageNo, int ConnectedSpeed, unsigned int SharedMemID, int JobID)
{
    error_type_t err = 0;
    SYS_UI_MESSAGE faxapp_mq_buf;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
	UINT16 OriginalDocId = 0;
#endif

    printf("\n(YHCHO) %s() - %d, %d, %d, %d, %d, %d\n", __FUNCTION__, StatusType, CurrentPageNo, TotalPageNo, ConnectedSpeed, SharedMemID, JobID);

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    switch (StatusType)
    {
        case FAX_APP_STATUS_MSG_SENDING_COMPLETE:
        case FAX_APP_STATUS_MSG_SENDING_FAIL:
        case FAX_APP_STATUS_MSG_NON_COMPLETED_FAIL:
        case FAX_APP_STATUS_MSG_DELETE_COMPLETE:
        case FAX_APP_STATUS_MSG_DELETE_FAIL:
        case FAX_APP_STATUS_MSG_FILENAME_MISMATCH:
        case FAX_APP_STATUS_MSG_JOB_CANCELED:
#if __HAVE_FAXSOLUTION_DK__
		case FAX_APP_STATUS_MSG_SCANNING_COMPLETE:
#endif
        {
            char UserId[MAX_LEN_USER_ID + 1] = {0, };
            char FaxNumber[SDH_FAX_LEN_PHONE_NO + 1] = {0};
            char *FileName = NULL;
            char *jobUserId = NULL;
            int i=0;
            int last_job = 0;
            int solution_result = 0;
            
            if(!isDKSolution && StatusType == FAX_APP_STATUS_MSG_SCANNING_COMPLETE)
            {
				break;
			}

            if(JobID == EMPTY_JOB_NUM)
            {
				UserId[0] = 0;
				FaxSetPhoneNumber(0, FaxNumber);
			}
			else
			{
				FileName = fsu_get_file_name_tx(JobID);
				for(i=0;i<MAX_LEN_USER_ID;i++)
				{
					if(FileName[22 + i] != 0)
					{
						UserId[i] = FileName[22 + i];
					}
					else
					{
						UserId[i] = 0;
						break;
					}
				}

				fsu_update_remote_no_for_solution(0, JobID);
			}

			if(UserId[0] == 0)
			{
				jobUserId = fsu_get_user_id(JobID);
				for(i=0;i<MAX_LEN_USER_ID;i++)
				{
					if(jobUserId[i] != 0)
					{
						gFaxLogInID[i] = jobUserId[i];
					}
					else
					{
						gFaxLogInID[i] = 0;
						break;
					}
				}
			}
			else
			{
				memcpy(gFaxLogInID, UserId, strlen(UserId) + 1);
			}

            printf("\n(YHCHO) %s() - gFaxLogInID: %s\n", __FUNCTION__, gFaxLogInID);

			if( StatusType == FAX_APP_STATUS_MSG_SENDING_COMPLETE ||
				StatusType == FAX_APP_STATUS_MSG_SENDING_FAIL ||
				StatusType == FAX_APP_STATUS_MSG_JOB_CANCELED )
			{
				int result = 0;
				if(StatusType == FAX_APP_STATUS_MSG_SENDING_FAIL || StatusType == FAX_APP_STATUS_MSG_JOB_CANCELED)
				{
					result = 1;
				}

				if(fsu_get_broadcast(JobID) == 1)
				{
					OriginalDocId = fsu_get_original_doc_id(JobID);

					if(gFax_BC_Num[OriginalDocId].BC_Current_Num >= gFax_BC_Num[OriginalDocId].BC_Total_Num)
					{
						last_job = gFax_BC_Num[OriginalDocId].BC_Total_Num;
					}
				}
				else
				{
					last_job = 1;
				}
#ifdef HAVE_IMAGELOG
				if(JobID != EMPTY_JOB_NUM)
					if(IsImageLogEnabled('F', IMAGE_LOG_JOB_TYPE_TX) == TRUE)
						save_fax_jobinfo(fsu_get_unique_job_id(JobID), TotalPageNo, result, (char *)fsu_get_dial_no(JobID));
#endif
			}
#if __HAVE_FAXSOLUTION_DK__
			else if( StatusType == FAX_APP_STATUS_MSG_SCANNING_COMPLETE && isDKSolution )
			{
#ifdef HAVE_IMAGELOG
				if(JobID != EMPTY_JOB_NUM)
				{
					int broadcast_index = 0;
					UINT16 originalDocId = EMPTY_JOB_NUM;//fsu_get_original_doc_id(JobID);
					
					last_job = 1;

					if(fsu_get_broadcast(JobID) == 1)
					{
						originalDocId = fsu_get_original_doc_id(JobID);
						for(broadcast_index = 0; broadcast_index< gFax_BC_Num[originalDocId].BC_Total_Num; broadcast_index++)
						{
							UINT16 broadcast_jnum = gFaxBroadcastJobInfo[originalDocId][broadcast_index];
							save_fax_jobinfo(fsu_get_unique_job_id(broadcast_jnum), TotalPageNo, 0, (char *)fsu_get_dial_no(broadcast_jnum));
						}
					}
					else
					{
						save_fax_jobinfo(fsu_get_unique_job_id(JobID), TotalPageNo, 0, (char *)fsu_get_dial_no(JobID));
					}
				}
#endif
			}
#endif
			else if( StatusType == FAX_APP_STATUS_MSG_DELETE_COMPLETE ||
					StatusType == FAX_APP_STATUS_MSG_DELETE_FAIL ||
					StatusType == FAX_APP_STATUS_MSG_FILENAME_MISMATCH )
			{
				solution_result = 1;
			}

			fsu_update_gfaxScanInfo (JobID);
            fsu_update_file_name_for_solution(JobID, last_job, solution_result);

            if( StatusType == FAX_APP_STATUS_MSG_SENDING_COMPLETE ||
                StatusType == FAX_APP_STATUS_MSG_SENDING_FAIL ||
                StatusType == FAX_APP_STATUS_MSG_JOB_CANCELED 
#if __HAVE_FAXSOLUTION_DK__
				|| StatusType == FAX_APP_STATUS_MSG_SCANNING_COMPLETE 
#endif
                )
            {
                fsu_clear_file_name_tx (JobID);
            }

            break;
        }

        default:
            break;
    }
#endif

    memset( &faxapp_mq_buf, 0, sizeof(faxapp_mq_buf) );

    faxapp_mq_buf.msgSender = FAXAPPID;
    faxapp_mq_buf.cmd = FAX_APP_MSG_SEND_FAX_STATUS;
    faxapp_mq_buf.data[0] = (UINT8)StatusType;
    faxapp_mq_buf.data[1] = (UINT8)CurrentPageNo;
    faxapp_mq_buf.data[2] = (UINT8)TotalPageNo;
    faxapp_mq_buf.data[3] = (UINT8)ConnectedSpeed;
    faxapp_mq_buf.data[4] = (UINT8)(SharedMemID >> 0);
    faxapp_mq_buf.data[5] = (UINT8)(SharedMemID >> 8);
    faxapp_mq_buf.data[6] = (UINT8)(SharedMemID >> 16);
    faxapp_mq_buf.data[7] = (UINT8)(SharedMemID >> 24);
    faxapp_mq_buf.data[8] = (UINT8)(JobID >> 0);
    faxapp_mq_buf.data[9] = (UINT8)(JobID >> 8);
    
    err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &faxapp_mq_buf);
    XASSERT( err==OK, err );
}


void faxapp_notify_SendFaxMemoryStatus(int FaxMemory)
{
    error_type_t err = 0;
    SYS_UI_MESSAGE faxapp_mq_buf;

    //printf("\n(YHCHO) %s() - FaxMemory: %d\n", __FUNCTION__, FaxMemory);

    memset( &faxapp_mq_buf, 0, sizeof(faxapp_mq_buf) );

    faxapp_mq_buf.msgSender = FAXAPPID;
    faxapp_mq_buf.cmd = FAX_APP_MSG_SEND_FAX_MEMORY_STATUS;
    faxapp_mq_buf.data[0] = (UINT8)FaxMemory;

    err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &faxapp_mq_buf);
    XASSERT( err==OK, err );
}


void faxapp_notify_SendFaxBroadcastJobID(int firstJobID, int number, int jobID)
{
    error_type_t err = 0;
    SYS_UI_MESSAGE faxapp_mq_buf;

    printf("\n(YHCHO) %s() - %d, %d, %d\n", __FUNCTION__, firstJobID, number, jobID);

    memset( &faxapp_mq_buf, 0, sizeof(faxapp_mq_buf) );

    faxapp_mq_buf.msgSender = FAXAPPID;
    faxapp_mq_buf.cmd = FAX_APP_MSG_SEND_FAX_BROADCAST_JOBID;
    faxapp_mq_buf.data[0] = (UINT8)(firstJobID >> 0);
    faxapp_mq_buf.data[1] = (UINT8)(firstJobID >> 8);
    faxapp_mq_buf.data[2] = (UINT8)number;
    faxapp_mq_buf.data[3] = (UINT8)(jobID >> 0);
    faxapp_mq_buf.data[4] = (UINT8)(jobID >> 8);
	
    err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &faxapp_mq_buf);
    XASSERT( err==OK, err );
}


void faxapp_notify_SendFaxSCStatus(int ErrorCode)
{
    SendSCStatusToUI(ErrorCode, FAX_APP_SCID);
    
	while (TRUE)
	{
		usleep(5*1000*1000);
	}
}


void faxapp_notify_SendFaxRequestReadDB(int JobCount)
{
    error_type_t err = 0;
    SYS_UI_MESSAGE faxapp_mq_buf;

    printf("\n(YHCHO) %s() - %d\n", __FUNCTION__, JobCount);

    memset( &faxapp_mq_buf, 0, sizeof(faxapp_mq_buf) );

    faxapp_mq_buf.msgSender = FAXAPPID;
    faxapp_mq_buf.cmd = FAX_APP_MSG_SEND_FAX_REQUEST_READ_DB;
    faxapp_mq_buf.data[0] = (UINT8)JobCount;

    err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &faxapp_mq_buf);
    XASSERT( err==OK, err );
}


void faxapp_notify_SendPCFaxJobID(int firstJobID, int number, int jobID)
{
    error_type_t err = 0;
    SYS_UI_MESSAGE faxapp_mq_buf;

    printf("\n(YHCHO) %s() - %d, %d, %d\n", __FUNCTION__, firstJobID, number, jobID);

    memset( &faxapp_mq_buf, 0, sizeof(faxapp_mq_buf) );

    faxapp_mq_buf.msgSender = FAXAPPID;
    faxapp_mq_buf.cmd = FAX_APP_MSG_SEND_PCFAX_JOBID;
    faxapp_mq_buf.data[0] = (UINT8)(firstJobID >> 0);
    faxapp_mq_buf.data[1] = (UINT8)(firstJobID >> 8);
    faxapp_mq_buf.data[2] = (UINT8)number;
    faxapp_mq_buf.data[3] = (UINT8)(jobID >> 0);
    faxapp_mq_buf.data[4] = (UINT8)(jobID >> 8);
	
    err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &faxapp_mq_buf);
    XASSERT( err==OK, err );
}

