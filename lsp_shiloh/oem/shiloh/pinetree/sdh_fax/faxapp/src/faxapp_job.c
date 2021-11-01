//
// Include Header Files
//
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "posix_ostools.h"
#include "agRouter.h"
#include "ATypes.h"
#include "list.h"
#include "memAPI.h"
#include "lassert.h"
#include "agMessage.h"
#include "agConnectMgr.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "regAddrs.h"
#include "agJobUtils.h"
#include "ResourceMap.h"
#include "utils.h"
#include "agprint.h"
#include "hwconfig_api.h"
#include "debug.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "data_access_api.h"
#include "error_types.h"
#include "BigBuffer.h"
#include "event_observer.h"
#include "response_subject.h"
#include "cmd_proc_api.h"
#include "scantypes.h"
#include "scanvars.h"
#include "scands.h"
#include "scanhwerr.h"
#include "scanman.h"
#include "scanplat.h"
#include "scanvars.h"
#include "scancap.h"
#include "scanmsg.h"
#include "print_job_api.h"
#include "PrintJobInfo.h"
#include "printvars_api.h"
#include "data_access_api.h"
#include "dma_buffer.h"
#include "faxapp_print.h"
#include "faxapp_notify.h"
#include "assert.h"
#include "faxapp_job.h"
#include "faxapp_param.h"
#include "faxapp_sp_mode.h"
#include "../../../../../../common/systemservice/include/SysSharedMemory.h"
#include "fsu_pcfax.h"
#include "../../CommonHeader/SDHFaxCommonHeader.h"
#include "spec.h"
#include <math.h>
#include <sys/time.h>
#include "fsu.h"


#define _FAXAPP_JOB_DEBUG     1


//
// Static Function Declaration
//
static void *faxapp_job_mgr (void *unused);
static void faxapp_update_flat_bed_scan (UINT8 FBMultiPageScanFlag);
static void faxapp_request_print_report (UINT8 report_num);
static void faxapp_update_tx_image_width (int OriginalSizeCode);
static void faxapp_request_reset_db (UINT8 IndexToDB);
static void faxapp_requset_go_fax_sp_mode (UINT8 Param, UINT16 SubParam, UINT8 Execution);
static void faxapp_requset_go_fax_ack_read_db (UINT8 Result);
static void faxapp_delete_docman (UINT8 Directory);

//
// Static Variables
//
static unsigned char FaxAppJobMgrStack[FAXAPP_JOB_STACK_SIZE] ALIGN8;
static mqd_t faxappjobmgr_msgq;
static pthread_t faxappmgr_thd_id;
static int OnHookButtonPressed = 0;
static UINT8 IndexToOnHookDial = 0;


//
// Global Variables
//
char gFaxDelayTimeInStringJobLog[MAX_LEN_DATE_TIME] = {0, };
char gFaxUserId[MAX_LEN_USER_ID + 1] = {0, };
char JobQueueDelayTime[MAX_LEN_FILE_PATH_NAME] = {0, };
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
char nameShareMem[MAX_LEN_FILE_PATH_NAME] = {0, };
#endif
#if __SUPPORT_MANUAL_RECEIVE__
char MachineKeyBuffer[MAX_LEN_MACHINE_ACCESS_CODE + 1] = {0, };
#else
#endif
UINT8 OnHookDialNo[LEN_PHONE_NO + 1] = {0, };
UINT8 FBMultiPageScan = FB_MULTI_PAGE_SCAN_INIT;
UINT8 gFaxModemResetComplete = 0;

#if __SUPPORT_MANUAL_RECEIVE__
UINT8 MachineKeyBufferIndex = 0;
#else
#endif

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
void faxapp_request_update_user_id (void);
#endif

//
// External Variables
//
#if __SUPPORT_MANUAL_TRANSMIT__
extern UINT8 ExtPhoneDialBuffer[LEN_PHONE_NO + 1];
extern UINT8 ExtPhoneDialIndex;
#else
#endif
extern UINT16 fsu_job_num_poll;
extern UINT16 fsu_job_num_send;
extern int isPhoneRingOn;
extern int ValidJobCnt;
extern int gFaxLineMonitor;
extern long gFaxJobStartTimeForDuration;
extern BOOLEAN fb_legal;
extern UINT8 isSolutionFax;
extern UINT8 isDKSolution;

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern UINT16 gFaxBroadcastJobInfo[MAX_NUM_DOC_ID][MAX_NUM_BC];
#endif
extern UINT16 fsu_job_num_scan;

extern int faxapp_print_report (UINT8 report);
extern void faxapp_fb_legal_scan_second_page_start(void);
extern void faxapp_fb_multiscan_request(int value);
extern int is_adf_paper_present (void);
extern void fsu_delete_docman(UINT8 directory);
extern void fsu_update_send_permission (UINT16 jnum, UINT8 permission);

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern char *fsu_get_file_name_tx (UINT16 jnum);
extern int fsu_delete_solution_job(UINT16 jnum);
extern void Update_Job_Queue_File(IMAGE_BACKUP_REASON_t Reason, UINT16 jnum, UINT8 Param);
extern UINT8 fsu_get_broadcast (UINT16 jnum);
extern UINT16 fsu_get_original_doc_id (UINT16 jnum);
extern void fsu_update_send_permission (UINT16 jnum, UINT8 permission);
#endif
extern int fsu_get_scan_busy_flag (void);

static FAXAPP_PARAMS_t FaxAppParam;
static FAXAPP_SCAN_PARAM_t FaxAppScanParam;
static stShmPhoneNumber	shm_backup_for_send;

/**
 * Function name: FaxAppJobMgrInit
 *
 * \brief Initialize the fax app job manager
 *
 * Initialize data structures, create the queue, register the mainbox and start the task for the fax app job manager
 **/
void FaxAppJobMgrInit(void)
{
    int retcode = 0;

    //
    // Create the queue
    //
    posix_create_message_queue(&faxappjobmgr_msgq, "/FaxAppJobMgrQueue", FAXAPPTASK_QUEUE_SIZE, sizeof(SYS_UI_MESSAGE));

    //
    // Register a message queue.
    //
    router_register_queue(FAXAPPID, faxappjobmgr_msgq);

    //
    // start the fax app job mgr task
    //
    retcode = posix_create_thread(&faxappmgr_thd_id, faxapp_job_mgr, 0, "FaxAppJobMgr", FaxAppJobMgrStack, FAXAPP_JOB_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    XASSERT( retcode==0, retcode );
}


/**
 * Function name: faxapp_job_mgr
 *
 * \brief Main thread of the fax app job manager
 *
 * This thread handles the upper level of the fax app job manager
 **/
static void *faxapp_job_mgr (void *unused)
{
    SYS_UI_MESSAGE Message;
    int iRET = 0;

    SysWaitForInit ();

    while (1)
    {
        //
        // Get the message from the queue.
        //
        iRET = mq_receive(faxappjobmgr_msgq, (char *)&Message, sizeof(SYS_UI_MESSAGE), NULL);
        XASSERT( iRET == sizeof(SYS_UI_MESSAGE), thread_errno );

        printf("\n(YHCHO) %s() - Message.cmd: 0x%02X\n", __FUNCTION__, Message.cmd);

        switch(Message.cmd)
        {
            case FAX_APP_MSG_GO_FAX_SCAN:
            {
                FaxAppScanParam.TxJobType = Message.data[0];
                FaxAppScanParam.NoOfRecipient = Message.data[1];
                FaxAppScanParam.TimeToTxHour = Message.data[2];
                FaxAppScanParam.TimeToTxMin = Message.data[3];
                FaxAppScanParam.OriginalSizeCode = ( Message.data[5] << 8 ) | ( Message.data[4] << 0 );
                FaxAppScanParam.Resolution = Message.data[6];
                FaxAppScanParam.ImageType = Message.data[7];
                FaxAppScanParam.Density = Message.data[8];
                FaxAppScanParam.DualScan = Message.data[9];

#if __SUPPORT_MANUAL_TRANSMIT__
                FaxAppScanParam.TxJobType = FAX_APP_JOB_TYPE_MANUAL_TRANSMIT;

                if ( (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_POLL_TRANSMIT) || (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_ON_HOOK) || (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_MANUAL_TRANSMIT) )
#else
                if ( (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_POLL_TRANSMIT) || (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_ON_HOOK) )
#endif
                {
                    FaxAppScanParam.SharedMemID = 0;
                }
                else
                {
                    FaxAppScanParam.SharedMemID = ( Message.data[13] << 24 ) | ( Message.data[12] << 16 ) | ( Message.data[11] << 8 ) | ( Message.data[10] << 0 );
                }
                
                FaxAppScanParam.AutoDarkness = Message.data[14];

#if _FAXAPP_JOB_DEBUG
                printf("\n(YHCHO) %s() - FaxAppScanParam.TxJobType: %d", __FUNCTION__, FaxAppScanParam.TxJobType);
                printf("\n(YHCHO) %s() - FaxAppScanParam.NoOfRecipient: %d", __FUNCTION__, FaxAppScanParam.NoOfRecipient);
                printf("\n(YHCHO) %s() - FaxAppScanParam.TimeToTxHour: %d", __FUNCTION__, FaxAppScanParam.TimeToTxHour);
                printf("\n(YHCHO) %s() - FaxAppScanParam.TimeToTxMin: %d", __FUNCTION__, FaxAppScanParam.TimeToTxMin);
                printf("\n(YHCHO) %s() - FaxAppScanParam.OriginalSizeCode: %d", __FUNCTION__, FaxAppScanParam.OriginalSizeCode);
                printf("\n(YHCHO) %s() - FaxAppScanParam.Resolution: %d", __FUNCTION__, FaxAppScanParam.Resolution);
                printf("\n(YHCHO) %s() - FaxAppScanParam.ImageType: %d", __FUNCTION__, FaxAppScanParam.ImageType);
                printf("\n(YHCHO) %s() - FaxAppScanParam.Density: %d", __FUNCTION__, FaxAppScanParam.Density);
                printf("\n(YHCHO) %s() - FaxAppScanParam.DualScan: %d", __FUNCTION__, FaxAppScanParam.DualScan);
                printf("\n(YHCHO) %s() - FaxAppScanParam.AutoDarkness: %d\n", __FUNCTION__, FaxAppScanParam.AutoDarkness);
#endif // #if _FAXAPP_JOB_DEBUG
#if __HAVE_FAXSOLUTION_DK__
				if(FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_DK_SOLUTION)
				{
					isDKSolution = 1;
				}
				else
				{
					isDKSolution = 0;
				}
#endif
                if (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_POLL_RECEIVE)
                {
                    // No Action
                }
                else
                {
                    if(FaxAppScanParam.OriginalSizeCode == MEDIASIZE_A3 || FaxAppScanParam.OriginalSizeCode == MEDIASIZE_11X17)
                    {
                        if(is_adf_paper_present ())
                        {
                            printf("\n(YHCHO) %s() - Cannot Scan A3 Merge!! Need ADF Clear!!\n", __FUNCTION__);

                            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_ADF_PAPER_IN_MERGE, 0, 0, 0, 0, EMPTY_JOB_NUM);

                            break;
                        }
                    }
                }

                if (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_ON_HOOK) // On-Hook
                {
                    UINT8 IndexToOnHookDialNo = 0;
                    UINT8 Len = 0;

#if __MEASURE_PERFORMANCE__
                    faxapp_get_job_start_time(ON_HOOK);
#endif // #if __MEASURE_PERFORMANCE__

                    OnHookDialNo[IndexToOnHookDial] = 0;

                    for (IndexToOnHookDialNo = 0; IndexToOnHookDialNo < LEN_PHONE_NO; IndexToOnHookDialNo++)
                    {
                        FaxAppParam.dial_num[0][IndexToOnHookDialNo] = '\0';
                    }

                    Len = strlen((char *)OnHookDialNo);

                    for (IndexToOnHookDialNo = 0; IndexToOnHookDialNo < Len; IndexToOnHookDialNo++)
                    {
                        FaxAppParam.dial_num[0][IndexToOnHookDialNo] = OnHookDialNo[IndexToOnHookDialNo];
                    }

                    FaxAppParam.dial_num[0][IndexToOnHookDialNo] = 0;

                    FaxAppParam.fax_type = FaxAppScanParam.TxJobType;

                    gFaxJobStartTimeForDuration = fsu_get_current_system_time();

                    fsu_get_job_start_time(gFaxJobStartTimeForDuration);

                    faxapp_activate_manual_fax();

                    OnHookButtonPressed = ON_HOOK_STATE_DISABLE;
                }
#if __SUPPORT_MANUAL_TRANSMIT__
                else if (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_MANUAL_TRANSMIT) // Manual
                {
                    UINT8 IndexToManDialNo = 0;
                    UINT8 Len = 0;

#if __MEASURE_PERFORMANCE__
                    faxapp_get_job_start_time(MANUAL_TRANSMIT);
#endif // #if __MEASURE_PERFORMANCE__

                    ExtPhoneDialIndex = 0;

                    for (IndexToManDialNo = 0; IndexToManDialNo < LEN_PHONE_NO; IndexToManDialNo++)
                    {
                        FaxAppParam.dial_num[0][IndexToManDialNo] = '\0';
                    }

                    Len = strlen((char *)ExtPhoneDialBuffer);

                    for (IndexToManDialNo = 0; IndexToManDialNo < Len; IndexToManDialNo++)
                    {
                        FaxAppParam.dial_num[0][IndexToManDialNo] = ExtPhoneDialBuffer[IndexToManDialNo];
                    }

                    FaxAppParam.dial_num[0][IndexToManDialNo] = 0;

                    FaxAppParam.fax_type = FaxAppScanParam.TxJobType;

                    gFaxJobStartTimeForDuration = fsu_get_current_system_time();

                    fsu_get_job_start_time(gFaxJobStartTimeForDuration);
                }
#else
#endif
                else
                {

#if __MEASURE_PERFORMANCE__
                    faxapp_get_job_start_time(SCAN_TO_MEM);
#endif // #if __MEASURE_PERFORMANCE__

                    // Step 1. Save Tx Job Type to Fax App Param
                    faxapp_update_tx_job_type(FaxAppScanParam.TxJobType);

                    // Step 2. Save No Of Recipient to Fax App Param
                    faxapp_update_no_of_recipient(FaxAppScanParam.NoOfRecipient);

                    // Step 3. Save Tx Time to Fax App Param
                    faxapp_update_time_diff(FaxAppScanParam.TxJobType, FaxAppScanParam.TimeToTxHour, FaxAppScanParam.TimeToTxMin);

                    if (FaxAppScanParam.TxJobType != FAX_APP_JOB_TYPE_POLL_TRANSMIT)
                    {
						// Step 4. Save Dial Number to Fax App Param
                        if (FaxAppScanParam.NoOfRecipient != 0)
                        {
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
                            faxapp_update_dial_num_and_ftp_info(FaxAppScanParam.NoOfRecipient, FaxAppScanParam.SharedMemID);
#else
                            faxapp_update_dial_num(FaxAppScanParam.NoOfRecipient, FaxAppScanParam.SharedMemID);
#endif
                        }
					}
                }

                faxapp_update_tx_image_width(FaxAppScanParam.OriginalSizeCode);

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
                //if (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_FAX_SOLUTION)
                {
					faxapp_request_update_user_id();
                    faxapp_update_file_name_tx();
                }
#endif


                if (FaxAppScanParam.TxJobType == FAX_APP_JOB_TYPE_POLL_RECEIVE) // Poll Receive
                {
                    fsu_start_poll_receive(&FaxAppParam);
                }
                else
                {
                    fsu_start_scan_to_mem(&FaxAppParam);
                }

                break;
            }

            case FAX_APP_MSG_GO_POWER_SAVE_MODE:
            {
                if ( (IS_OAK2M == 1) || (IS_OAK2H == 1) )
                {
                    if (Message.data[0] == 1) // Sleep Mode
                    {
                        printf("\n(YHCHO) %s() - Go To Sleep Mode...\n", __FUNCTION__);

                        gFaxModemResetComplete = 0;

                        ModemSleep(FAXAPP_MODEM_SLEEP_MODE_NORMAL_SLEEP);
                    }
                    else // Wake Up
                    {
                        if (isPhoneRingOn == 0)
                        {
                            while(1)
                            {
                                if (gFaxModemResetComplete == 1)
                                {
                                    printf("\n(YHCHO) %s() - Wake Up...\n", __FUNCTION__);

                                    break;
                                }
                                else
                                {
                                    printf("\n(YHCHO) %s() - MODEM IS ENTERING SLEEP MODE! JUST WAIT HERE...\n", __FUNCTION__);
                                }

                                usleep(300*1000); // 300ms
                            }

                            ModemWakeup();
                        }
                    }
                }

                break;
            }
				
			case FAX_APP_MSG_GO_ON_HOOK:
            {
                faxapp_go_on_hook(Message.data[0]);

				break;
            }
				
			case FAX_APP_MSG_GO_UPDATE_PARAM:
            {
				faxapp_update_parameter(Message.data[0]);

				break;
            }

			case FAX_APP_MSG_GO_FAX_SCAN_MORE:
            {
                faxapp_update_flat_bed_scan(Message.data[0]);

				break;
            }

			case FAX_APP_MSG_GO_FAX_CANCEL:
            {
				fsu_cancel_fax_job(Message.data[0]);

				break;
            }

			case FAX_APP_MSG_GO_FAX_DELETE_PENDING_JOB:
            {
                UINT16 jnum = ( Message.data[1] << 8 ) | ( Message.data[0] << 0 );

                if(isSolutionFax)
                {
                    fsu_delete_solution_job(jnum);
                }
                else
                {
                    fsu_delete_pending_job(jnum);
                }

				break;
            }
				
			case FAX_APP_MSG_GO_FAX_PRINT_REPORT:
            {
                faxapp_request_print_report(Message.data[0]);

				break;
            }

            case FAX_APP_MSG_GO_FAX_RESET_DB:
            {
                faxapp_request_reset_db(Message.data[0]);

                break;
            }

            case FAX_APP_MSG_GO_FAX_SP_MODE:
            {
                UINT8 Param = Message.data[0];
                UINT8 Execution = Message.data[3];
                UINT16 SubParam = ( Message.data[2] << 8 ) | ( Message.data[1] << 0 );

                faxapp_requset_go_fax_sp_mode(Param, SubParam, Execution);

                break;
            }

            case FAX_APP_MSG_GO_FAX_ACK_READ_DB:
            {
                UINT8 Result = Message.data[0];

                faxapp_requset_go_fax_ack_read_db(Result);

                fsu_request_image_backup_job_begin(ValidJobCnt);

                break;
            }
            
            case FAX_APP_MSG_GO_FAX_DELETE_DOCMAN:
            {
				UINT8 directory = Message.data[0];
				
				faxapp_delete_docman(directory);

				break;
            }

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
            case FAX_APP_MSG_GO_SOLUTION_FAX:
            {
                UINT8 Type = Message.data[0];
                UINT32 SharedMemID = ( Message.data[4] << 24 ) | ( Message.data[3] << 16 ) | ( Message.data[2] << 8 ) | ( Message.data[1] << 0 );

                faxapp_go_solution_fax(Type, SharedMemID);

                break;
            }
#endif

#if __SUPPORT_MANUAL_RECEIVE__
            case FAX_APP_MSG_GO_FAX_RECEIVE:
            {
                switch (Message.data[0])
                {
                    case KEY_ON_HOOK:
                    {
                        MachineKeyBuffer[MachineKeyBufferIndex] = 'O';

                        break;
                    }

                    case KEY_CANCEL:
                    {
                        MachineKeyBuffer[MachineKeyBufferIndex] = 'C';

                        break;
                    }

                    case KEY_START:
                    {
                        MachineKeyBuffer[MachineKeyBufferIndex] = 'S';

                        break;
                    }

                    default:
                    {
                        MachineKeyBuffer[MachineKeyBufferIndex] = 'U'; // Not Valid Key...

                        break;
                    }
                }

                MachineKeyBufferIndex++;

                MachineKeyBuffer[MachineKeyBufferIndex] = 0;

                printf("\n(YHCHO) %s() - MachineKeyBuffer: %s\n", __FUNCTION__, MachineKeyBuffer);

                break;
            }
#else
#endif

            default:
                break;
        }
    }

    return 0;
}


int faxapp_check_on_hook_button_pressed (void)
{
    return OnHookButtonPressed;
}


void faxapp_go_on_hook(UINT8 KeyValue)
{
    UINT8 Volume = 0;
    int count = 0;

    if (gFaxLineMonitor == LINE_MONITOR_OFF) // OFF
    {
        Volume = 0;
    }
    else
    {
        Volume = fsu_get_speaker_volume();
    }

    printf("\n(YHCHO) %s() - OnHookButtonPressed: %d", __FUNCTION__, OnHookButtonPressed);
    printf("\n(YHCHO) %s() - gFaxLineMonitor: %d", __FUNCTION__, gFaxLineMonitor);
    printf("\n(YHCHO) %s() - Volume: %d\n", __FUNCTION__, Volume);

    fsu_set_speaker_volume(Volume);
	if (OnHookButtonPressed == ON_HOOK_STATE_CANCELING)
	{
		printf("\n(YHCHO) %s() - Canceling.. Ignore UI Message !!!!!!\n", __FUNCTION__);
	}

	while(1)
	{
		if(OnHookButtonPressed != ON_HOOK_STATE_ENABLE && OnHookButtonPressed != ON_HOOK_STATE_CANCELING)
			break;

		if(count > 100)
		{
			printf("\n(YHCHO) %s() - Waiting Time Out !!!!!!\n", __FUNCTION__);

			break;
		}

		usleep(50*1000);

		count++;
	}

    if (OnHookButtonPressed == ON_HOOK_STATE_DISABLE)
    {
        switch(KeyValue)
        {
            case 0x0C:     // ON-HOOK
            
				OnHookButtonPressed = ON_HOOK_STATE_ENABLE;
            
                faxapp_activate_off_hook();     // PHONE_OFFHOOK...

                isPhoneRingOn = 0;
                
                break;

            default:
                break;
        }
    }
    else
    {
        switch(KeyValue)
        {
            case 0x0C:     // CANCEL, HOME, ON-HOOK, etc..
				if(OnHookButtonPressed == ON_HOOK_STATE_OFFHOOK)
				{
					OnHookButtonPressed = ON_HOOK_STATE_CANCELING;
					
					faxapp_activate_on_hook();

					faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CANCEL_COMPLETE, 0, 0, 0, 0, EMPTY_JOB_NUM);
				}
                break;

            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 0x0A:
            case 0x0B:
                faxapp_activate_on_hook_dial(KeyValue);
                break;

            default:
                break;
        }
    }
}


void faxapp_activate_off_hook (void)
{
    IndexToOnHookDial = 0;

    memset(OnHookDialNo, 0x00, sizeof(OnHookDialNo));

    Fax_API_OffHook();
}


void faxapp_activate_on_hook (void)
{
    IndexToOnHookDial = 0;

    memset(OnHookDialNo, 0x00, sizeof(OnHookDialNo));

    Fax_API_OnHook();
}


void faxapp_activate_manual_fax (void)
{
    IndexToOnHookDial = 0;

    memset(OnHookDialNo, 0x00, sizeof(OnHookDialNo));

    Fax_API_ManTransmitFax();
}


void faxapp_activate_on_hook_dial (UINT8 InputValue)
{
    FaxAppParam.dial_num[0][0] = 'T';

    if (InputValue == 0x0A)
    {
        FaxAppParam.dial_num[0][1] = 0x2A; // * key
    }
    else if (InputValue == 0x0B)
    {
        FaxAppParam.dial_num[0][1] = 0x23; // # key
    }
    else
    {
        FaxAppParam.dial_num[0][1] = 0x30 + InputValue;
    }

    FaxAppParam.dial_num[0][2] = 0;

    OnHookDialNo[IndexToOnHookDial] = FaxAppParam.dial_num[0][1];
    IndexToOnHookDial++;

    Fax_API_Dial(&FaxAppParam);
}


/***********************************************************************
 *  Function: faxapp_update_tx_job_type()
 *
 *  Description:
 *    update FaxAppParam struct (fax_type) for scan to mem
 *
 *  Inputs:
 *    int TxJobType
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_update_tx_job_type(int TxJobType)
{
    FaxAppParam.fax_type = TxJobType;

    if (FaxAppParam.fax_type == FAX_APP_JOB_TYPE_POLL_TRANSMIT)
    {
        // Check to see whether anoter poll tx job is already set.
        // If another poll tx job is already set, the previous poll tx job should be deleted.
        fsu_check_poll_tx_job(fsu_job_num_poll);
    }
}


/***********************************************************************
 *  Function: faxapp_update_no_of_recipient()
 *
 *  Description:
 *    update FaxAppParam struct (no_of_recipient) for scan to mem
 *
 *  Inputs:
 *    int NoOfRecipient
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_update_no_of_recipient(int NoOfRecipient)
{
    FaxAppParam.no_of_recipient = NoOfRecipient;
}


/***********************************************************************
 *  Function: faxapp_update_time_diff()
 *
 *  Description:
 *    update FaxAppParam struct (time_diff) for scan to mem
 *
 *  Inputs:
 *    int TimeToTxHour
 *    int TimeToTxMin
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_update_time_diff(int TxJobType, int TimeToTxHour, int TimeToTxMin)
{
    if (TxJobType == FAX_APP_JOB_TYPE_DELAY)
    {
        UINT8 Carry = 0;
        long int DiffInSec = 0;
        time_t t;
        struct tm *pt;
        typedef struct {
            int Year;
            int Month;
            int Day;
            int Hour;
            int Min;
        } DELAY_TIME_t;

        UINT8 Days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        DELAY_TIME_t LocalTime = { };
        DELAY_TIME_t UIStartTime = { };
        DELAY_TIME_t Diff = { };
        DELAY_TIME_t CarryField = { };

        time(&t);

        pt = localtime(&t);

        if (pt == NULL)
        {
            LocalTime.Year = UIStartTime.Year = 1900;
            LocalTime.Month = UIStartTime.Month = 1;
            LocalTime.Day = UIStartTime.Day = 1;
            LocalTime.Hour = 0;
            LocalTime.Min = 0;

            UIStartTime.Hour = TimeToTxHour;
            UIStartTime.Min = TimeToTxMin;

            memset(JobQueueDelayTime, 0x00, sizeof(JobQueueDelayTime));

            snprintf(JobQueueDelayTime, sizeof(JobQueueDelayTime), "%04d.%02d.%02d %02d:%02d", UIStartTime.Year, UIStartTime.Month, UIStartTime.Day, UIStartTime.Hour, UIStartTime.Min);
        }
        else
        {
            LocalTime.Year = UIStartTime.Year = pt->tm_year + 1900;
            LocalTime.Month = UIStartTime.Month = pt->tm_mon + 1;
            LocalTime.Day = UIStartTime.Day = pt->tm_mday;
            LocalTime.Hour = pt->tm_hour;
            LocalTime.Min = pt->tm_min;

            UIStartTime.Hour = TimeToTxHour;
            UIStartTime.Min = TimeToTxMin;

            if ( ((LocalTime.Hour == TimeToTxHour) && (LocalTime.Min > TimeToTxMin)) || (LocalTime.Hour > TimeToTxHour) )
            {
                Carry++;
            }
            else
            {
                // No Action Here
            }

            if (Carry == 0)
            {
                memset(JobQueueDelayTime, 0x00, sizeof(JobQueueDelayTime));

                snprintf(JobQueueDelayTime, sizeof(JobQueueDelayTime), "%04d.%02d.%02d %02d:%02d", UIStartTime.Year, UIStartTime.Month, UIStartTime.Day, UIStartTime.Hour, UIStartTime.Min);
            }
            else
            {
                UINT8 isLeapYear = 0;
                DELAY_TIME_t TimeToTx = { };

                memset(&TimeToTx, 0x00, sizeof(DELAY_TIME_t));

                if ( (LocalTime.Year % 4 == 0) && ((LocalTime.Year % 100 != 0) || (LocalTime.Year % 400 == 0)) )
                {
                    isLeapYear = 1;
                }
                else
                {
                    isLeapYear = 0;
                }

                //printf("\n(YHCHO) isLeapYear: %d\n", isLeapYear);

                if (isLeapYear == 1)
                {
                    Days[2] = 29;
                }
                else
                {
                    Days[2] = 28; // fail-safe
                }

                TimeToTx.Min = TimeToTxMin;

                TimeToTx.Hour = TimeToTxHour;

                TimeToTx.Day += LocalTime.Day;

                TimeToTx.Day++;

                if (TimeToTx.Day > Days[LocalTime.Month])
                {
                    TimeToTx.Day -= Days[LocalTime.Month];

                    TimeToTx.Month += LocalTime.Month;

                    TimeToTx.Month++;

                    if (TimeToTx.Month > 12)
                    {
                        TimeToTx.Month -= 12;

                        TimeToTx.Year += LocalTime.Year;

                        TimeToTx.Year++;
                    }
                    else
                    {
                        TimeToTx.Year = LocalTime.Year;
                    }
                }
                else
                {
                    TimeToTx.Year = LocalTime.Year;
                    TimeToTx.Month = LocalTime.Month;
                }

                UIStartTime.Year = TimeToTx.Year;
                UIStartTime.Month = TimeToTx.Month;
                UIStartTime.Day = TimeToTx.Day;
                UIStartTime.Hour = TimeToTx.Hour;
                UIStartTime.Min = TimeToTx.Min;

                memset(JobQueueDelayTime, 0x00, sizeof(JobQueueDelayTime));

                snprintf(JobQueueDelayTime, sizeof(JobQueueDelayTime), "%04d.%02d.%02d %02d:%02d", UIStartTime.Year, UIStartTime.Month, UIStartTime.Day, UIStartTime.Hour, UIStartTime.Min);
            }
        }

        printf("\n(YHCHO) -     Start Time: %04d.%02d.%02d %02d:%02d", UIStartTime.Year, UIStartTime.Month, UIStartTime.Day, UIStartTime.Hour, UIStartTime.Min);
        printf("\n(YHCHO) - LocalTime Time: %04d.%02d.%02d %02d:%02d\n", LocalTime.Year, LocalTime.Month, LocalTime.Day, LocalTime.Hour, LocalTime.Min);

        if (UIStartTime.Min - LocalTime.Min >= 0)
        {
            Diff.Min = UIStartTime.Min - LocalTime.Min;
        }
        else
        {
            Diff.Min = 60 + UIStartTime.Min - LocalTime.Min;

            CarryField.Hour++;
        }

        if (UIStartTime.Hour - LocalTime.Hour - CarryField.Hour >= 0)
        {
            Diff.Hour = UIStartTime.Hour - LocalTime.Hour - CarryField.Hour;
        }
        else
        {
            Diff.Hour = 24 + UIStartTime.Hour - LocalTime.Hour - CarryField.Hour;

            CarryField.Day++;
        }

        if (UIStartTime.Day - LocalTime.Day - CarryField.Day >= 0)
        {
            Diff.Day = UIStartTime.Day - LocalTime.Day - CarryField.Day;
        }
        else
        {
            Diff.Day = Days[LocalTime.Month] + UIStartTime.Day - LocalTime.Day - CarryField.Day;

            CarryField.Month++;
        }

        if (UIStartTime.Month - LocalTime.Month - CarryField.Month >= 0)
        {
            Diff.Month = UIStartTime.Month - LocalTime.Month - CarryField.Month;
        }
        else
        {
            Diff.Month = 12 + UIStartTime.Month - LocalTime.Month - CarryField.Month;

            CarryField.Year++;
        }

        Diff.Year = UIStartTime.Year - LocalTime.Year - CarryField.Year;

        printf("\n(YHCHO) Diff.Year: %d", Diff.Year);
        printf("\n(YHCHO) Diff.Month: %d", Diff.Month);
        printf("\n(YHCHO) Diff.Day: %d", Diff.Day);
        printf("\n(YHCHO) Diff.Hour: %d", Diff.Hour);
        printf("\n(YHCHO) Diff.Min: %d\n", Diff.Min);

        DiffInSec = fsu_calculate_time_diff_in_sec(Diff.Year, Diff.Month, Diff.Day, Diff.Hour, Diff.Min);

        printf("\n(YHCHO) DiffInSec: %ld\n", DiffInSec);

        if (DiffInSec == 0)
        {
            FaxAppParam.time_diff = TRANSMIT_TIME_RIGHT_NOW;     // Send Right Now...
        }
        else
        {
            FaxAppParam.time_diff = DiffInSec;     // Send Later...
        }
    }
    else
    {
        FaxAppParam.time_diff = TRANSMIT_TIME_RIGHT_NOW;     // Send Right Now...
    }

    faxapp_set_delay_time_in_string_job_log(JobQueueDelayTime);
}

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: faxapp_update_dial_num_and_ftp_info()
 *
 *  Description:
 *    update FaxAppParam struct (dial_num) and FTP_FAX_SOLUTION_t for scan to mem
 *
 *  Inputs:
 *    int NoOfRecipient: number of recipient
 *    UINT32 ShareMemID: ID of shared memory
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_update_dial_num_and_ftp_info(int NoOfRecipient, UINT32 ShareMemID)
{
    int IndexToRecipient = 0;
    int IndexToDial = 0;
    int ret_val = 0;
    stShmPhoneNumber *pPhoneNumber;

    printf("\n(YHCHO) %s() - NoOfRecipient: %d", __FUNCTION__, NoOfRecipient);
    printf("\n(YHCHO) %s() - ShareMemID: %ld\n", __FUNCTION__, ShareMemID);

    // Step 1. Clear Fax App Dial Array
    for (IndexToRecipient = 0; IndexToRecipient < NoOfRecipient; IndexToRecipient++)
    {
        for (IndexToDial = 0; IndexToDial < LEN_PHONE_NO; IndexToDial++)
        {
            FaxAppParam.dial_num[IndexToRecipient][IndexToDial] = '\0';
        }
    }

    pPhoneNumber = shm_attach(ShareMemID);

    if ( pPhoneNumber != (void *)-1 )
    {
        // Step 2. Save Dail Number Stored in Shared Memory to Fax App Param
        for (IndexToRecipient = 0; IndexToRecipient < NoOfRecipient; IndexToRecipient++)
        {
            for (IndexToDial = 0; IndexToDial < LEN_PHONE_NO; IndexToDial++)
            {
                if (pPhoneNumber->no[IndexToRecipient] != '\0')
                {
                    if (pPhoneNumber->no[IndexToRecipient][IndexToDial] == 0x2D) // '-' key
                    {
                        FaxAppParam.dial_num[IndexToRecipient][IndexToDial] = 0x2C; // Pause Key
                    }
                    else
                    {
                        FaxAppParam.dial_num[IndexToRecipient][IndexToDial] = pPhoneNumber->no[IndexToRecipient][IndexToDial];
                    }
                }
                else
                {
                    break;
                }
            }

            FaxAppParam.dial_num[IndexToRecipient][IndexToDial] = 0;

            printf("\n(YHCHO) %s() - FaxAppParam.dial_num[%d]: %s\n", __FUNCTION__, IndexToRecipient, FaxAppParam.dial_num[IndexToRecipient]);
        }

        // Step 3. Save FTP related info. stored in Shared Memory
		memset( &shm_backup_for_send, 0x00, sizeof(stShmPhoneNumber) );
		memcpy( &shm_backup_for_send, pPhoneNumber, sizeof(stShmPhoneNumber) );
		        
        printf("\n(YHCHO) %s() - shm_backup_for_send.ftpAddress:	%s\n",		__FUNCTION__, shm_backup_for_send.ftpAddress);
        printf("\n(YHCHO) %s() - shm_backup_for_send.ftpPort: 		%d\n",		__FUNCTION__, shm_backup_for_send.ftpPort);
        printf("\n(YHCHO) %s() - shm_backup_for_send.ftpID: 		%s\n",		__FUNCTION__, shm_backup_for_send.ftpID);
        printf("\n(YHCHO) %s() - shm_backup_for_send.ftpPassword:	%s\n",		__FUNCTION__, shm_backup_for_send.ftpPassword);
    }
    else
    {
        printf("\n(YHCHO) %s() - shm_attach() Error!\n", __FUNCTION__);
    }

    ret_val = shm_detach((void *)pPhoneNumber);

    if ( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_detach() Error!\n", __FUNCTION__);
    }

    ret_val = shm_remove(ShareMemID);

    if ( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_remove() Error!\n", __FUNCTION__);
    }
}
#else
/***********************************************************************
 *  Function: faxapp_update_dial_num()
 *
 *  Description:
 *    update FaxAppParam struct (dial_num) for scan to mem
 *
 *  Inputs:
 *    int NoOfRecipient: number of recipient
 *    UINT32 ShareMemID: ID of shared memory
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_update_dial_num(int NoOfRecipient, UINT32 ShareMemID)
{
    int IndexToRecipient = 0;
    int IndexToDial = 0;
    int ret_val = 0;
    stShmPhoneNumber *pPhoneNumber;

    printf("\n(YHCHO) %s() - NoOfRecipient: %d", __FUNCTION__, NoOfRecipient);
    printf("\n(YHCHO) %s() - ShareMemID: %ld\n", __FUNCTION__, ShareMemID);

    // Step 1. Clear Fax App Dial Array
    for (IndexToRecipient = 0; IndexToRecipient < NoOfRecipient; IndexToRecipient++)
    {
        for (IndexToDial = 0; IndexToDial < LEN_PHONE_NO; IndexToDial++)
        {
            FaxAppParam.dial_num[IndexToRecipient][IndexToDial] = '\0';
        }
    }

    pPhoneNumber = shm_attach(ShareMemID);

    if ( pPhoneNumber != (void *)-1 )
    {
        // Step 2. Save Dial Number Stored in Shared Memory to Fax App Param
        for (IndexToRecipient = 0; IndexToRecipient < NoOfRecipient; IndexToRecipient++)
        {
            for (IndexToDial = 0; IndexToDial < LEN_PHONE_NO; IndexToDial++)
            {
                if (pPhoneNumber->no[IndexToRecipient] != '\0')
                {
                    if (pPhoneNumber->no[IndexToRecipient][IndexToDial] == 0x2D) // '-' key
                    {
                        FaxAppParam.dial_num[IndexToRecipient][IndexToDial] = 0x2C; // Pause Key
                    }
                    else
                    {
                        FaxAppParam.dial_num[IndexToRecipient][IndexToDial] = pPhoneNumber->no[IndexToRecipient][IndexToDial];
                    }
                }
                else
                {
                    break;
                }
            }

            FaxAppParam.dial_num[IndexToRecipient][IndexToDial] = 0;

            printf("\n(YHCHO) %s() - FaxAppParam.dial_num[%d]: %s\n", __FUNCTION__, IndexToRecipient, FaxAppParam.dial_num[IndexToRecipient]);
        }
    }
    else
    {
        printf("\n(YHCHO) %s() - shm_attach() Error!\n", __FUNCTION__);
    }

    ret_val = shm_detach((void *)pPhoneNumber);

    if ( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_detach() Error!\n", __FUNCTION__);
    }

    ret_val = shm_remove(ShareMemID);

    if ( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_remove() Error!\n", __FUNCTION__);
    }
}
#endif

/***********************************************************************
 *  Function: faxapp_get_shm_fax_solution()
 *
 *  Description:
 *    get shared memory regarding fax solution
 *
 *  Inputs:
 *
 *  Output:
 *    
 **********************************************************************/
char* faxapp_get_ftpAddress_fax_solution(void)
{
    return shm_backup_for_send.ftpAddress;
}

char* faxapp_get_ftpID_fax_solution(void)
{
    return shm_backup_for_send.ftpID;
}

char* faxapp_get_ftpPassword_fax_solution(void)
{
    return shm_backup_for_send.ftpPassword;
}

int faxapp_get_ftpPort_fax_solution(void)
{
    return shm_backup_for_send.ftpPort;
}

int faxapp_get_txtFile_fax_solution(void)
{
    return shm_backup_for_send.txtFile;
}

int faxapp_get_endFile_fax_solution(void)
{
    return shm_backup_for_send.endFile;
}

/***********************************************************************
 *  Function: faxapp_get_no_of_recipient()
 *
 *  Description:
 *    get no of recipient from UI Manager
 *
 *  Inputs:
 *
 *  Output:
 *    no of recipient
 **********************************************************************/
UINT8 faxapp_get_no_of_recipient(void)
{
    return (FaxAppScanParam.NoOfRecipient);
}


/***********************************************************************
 *  Function: faxapp_get_job_type()
 *
 *  Description:
 *    get job type from UI Manager
 *
 *  Inputs:
 *
 *  Output:
 *    job type
 **********************************************************************/
UINT8 faxapp_get_job_type(void)
{
    return (FaxAppScanParam.TxJobType);
}


/***********************************************************************
 *  Function: faxapp_get_resolution()
 *
 *  Description:
 *    get resolution from UI Manager
 *
 *  Inputs:
 *
 *  Output:
 *    0: Standard
 *    1: Fine
 *    3: 300DPI
 **********************************************************************/
UINT8 faxapp_get_resolution(void)
{
    return (FaxAppScanParam.Resolution);
}


/***********************************************************************
 *  Function: faxapp_get_originalSizeCode()
 *
 *  Description:
 *    get originalSizeCode from UI Manager
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
UINT32 faxapp_get_originalSizeCode(void)
{
    return (FaxAppScanParam.OriginalSizeCode);
}


/***********************************************************************
 *  Function: faxapp_get_ImageType()
 *
 *  Description:
 *    get ImageType from UI Manager
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
UINT8 faxapp_get_ImageType(void)
{
    return (FaxAppScanParam.ImageType);
}


/***********************************************************************
 *  Function: faxapp_get_Density()
 *
 *  Description:
 *    get Density from UI Manager
 *
 *  Inputs:
 *
 *  Output:
 *   
 **********************************************************************/
UINT32 faxapp_get_Density(void)
{
    return (FaxAppScanParam.Density);
}


/***********************************************************************
 *  Function: faxapp_get_DualScan()
 *
 *  Description:
 *    get DualScan from UI Manager
 *
 *  Inputs:
 *
 *  Output:
 *    0: OFF (default)
 *    1: ON  
 **********************************************************************/
UINT32 faxapp_get_DualScan(void)
{
    return (FaxAppScanParam.DualScan);
}


/***********************************************************************
 *  Function: faxapp_get_AutoDarkness()
 *
 *  Description:
 *    get AutoDarkness from UI Manager
 *
 *  Inputs:
 *
 *  Output:
 *    0: OFF (default)
 *    1: ON  
 **********************************************************************/
UINT32 faxapp_get_AutoDarkness(void)
{
    return (FaxAppScanParam.AutoDarkness);
}


/***********************************************************************
 *  Function: faxapp_create_shared_mem_for_display_transmit()
 *
 *  Description:
 *    Save dial no to shared memory for display when dialing
 *
 *  Inputs:
 *    UINT8 *DialNo: Dial Number
 *
 *  Output:
 *    ID of shared memory
 *
 **********************************************************************/
UINT32 faxapp_create_shared_mem_for_display_transmit(UINT8 *DialNo)
{
    stShmFaxPhoneNumber *pDialNo;
    int ret_val = 0;
    UINT8 Len = 0;
    UINT8 Index = 0;
    char DialNo4GUI[LEN_PHONE_NO + 1] = {0, };
    char Temp_DialNo4GUI[LEN_PHONE_NO + 1] = {0, };
    UINT32 ShareMemID = 0;

    printf("\n(YHCHO) %s() - DialNo: %s\n", __FUNCTION__, DialNo);

    pDialNo = (stShmFaxPhoneNumber *)shm_create(SHM_KEY_PHONE_DISP, sizeof(stShmFaxPhoneNumber));

    if ( pDialNo == (void *)-1 )
    {
        printf("\n(YHCHO) %s() - shm_create() Error\n", __FUNCTION__);

		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_CREATE_SHARED_MEMORY);
    }

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

    strncpy(&pDialNo->no[0], DialNo4GUI, sizeof(stShmFaxPhoneNumber));

    ret_val = shm_detach(pDialNo);

    if( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_detach() Error\n", __FUNCTION__);

		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_SHARED_MEMORY_ERROR);
    }

    ShareMemID = shm_get_keyid(SHM_KEY_PHONE_DISP);

    return ShareMemID;
}


/***********************************************************************
 *  Function: faxapp_create_shared_mem_for_display_receive()
 *
 *  Description:
 *    Save dial no to shared memory for display when receiving
 *
 *  Inputs:
 *    char *DialNo: pointer to dial no.
 *
 *  Output:
 *    ID of shared memory
 *
 **********************************************************************/
UINT32 faxapp_create_shared_mem_for_display_receive(char *DialNo)
{
    stShmFaxPhoneNumber *pDialNo;
    int ret_val = 0;
    UINT32 ShareMemID = 0;

    printf("\n(YHCHO) %s() - DialNo: %s\n", __FUNCTION__, DialNo);

    pDialNo = (stShmFaxPhoneNumber *)shm_create(SHM_KEY_PHONE_DISP, sizeof(stShmFaxPhoneNumber));

    if ( pDialNo == (void *)-1 )
    {
        printf("\n(YHCHO) %s() - shm_create() Error\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_CREATE_SHARED_MEMORY);
    }

    strncpy(&pDialNo->no[0], DialNo, sizeof(stShmFaxPhoneNumber));

    ret_val = shm_detach(pDialNo);

    if( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_detach() Error\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_SHARED_MEMORY_ERROR);
    }

    ShareMemID = shm_get_keyid(SHM_KEY_PHONE_DISP);

    return ShareMemID;
}


/***********************************************************************
 *  Function: faxapp_create_shared_mem_for_pcfax()
 *
 *  Description:
 *    Save dial no to shared memory for display when dialing in case of PC Fax
 *
 *  Inputs:
 *    PCFAX_Recipient_info *DialNo: pointer to DialNo of PCFAX_Recipient_info struct
 *    UINT8 recipient_num: number or recipient
 *
 *  Output:
 *    ID of shared memory
 *
 **********************************************************************/
UINT32 faxapp_create_shared_mem_for_pcfax(PCFAX_Recipient_info *DialNo, UINT8 recipient_num)
{
    stShmPhoneNumber *pDialNo;
    int ret_val = 0;
    UINT8 Len = 0;
    UINT8 Index = 0;
    char DialNo4GUI[LEN_PHONE_NO + 1] = {0, };
    char Temp_DialNo4GUI[LEN_PHONE_NO + 1] = {0, };
    UINT32 ShareMemID = 0;
    int i = 0;

    printf("\n(YHCHO) %s() recipient_num: %d\n", __FUNCTION__, recipient_num);

    pDialNo = (stShmPhoneNumber *)shm_create(SHM_KEY_PCFAX_PHONE_NO, sizeof(stShmPhoneNumber));

    if ( pDialNo == (void *)-1 )
    {
        printf("\n(YHCHO) %s() - shm_create() Error\n", __FUNCTION__);

		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_CREATE_SHARED_MEMORY);
    }

	for(i=0; i < recipient_num; i++)
	{
		memset(Temp_DialNo4GUI, 0, sizeof(Temp_DialNo4GUI));
		memset(DialNo4GUI, 0, sizeof(DialNo4GUI));
		snprintf(Temp_DialNo4GUI, sizeof(Temp_DialNo4GUI), "%s", &DialNo[i].recipient_phone_num[0]);

		printf("\n(YHCHO) %s() : PC FAX PHONE NO[%d] : %s\n", __FUNCTION__,i,Temp_DialNo4GUI);

		Len = strlen(Temp_DialNo4GUI);

		for (Index = 0; Index < Len; Index++)
		{
            if (Temp_DialNo4GUI[Index] == 0x2C) // ','
			{
                DialNo4GUI[Index] = 0x2D; // '-'
			}
			else
			{
				DialNo4GUI[Index] = Temp_DialNo4GUI[Index];
			}
		}
		DialNo4GUI[Index] = 0;

		strncpy(pDialNo->no[i], DialNo4GUI, sizeof(pDialNo->no[i]));
	}
	
    ret_val = shm_detach(pDialNo);
    if( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_detach() Error\n", __FUNCTION__);

		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_SHARED_MEMORY_ERROR);
    }

    ShareMemID = shm_get_keyid(SHM_KEY_PCFAX_PHONE_NO);

    return ShareMemID;
}


/********************************************************************************
 *  Function: faxapp_update_flat_bed_scan()
 *
 *  Description:
 *    update FBMultiPageScan to determine whether to stop or continue FB scan job
 *
 *  Inputs:
 *    UINT8 FBMultiPageScanFlag: FB Scan flag from UIManager
 *
 *  Output:
 *
 ********************************************************************************/
static void faxapp_update_flat_bed_scan (UINT8 FBMultiPageScanFlag)
{
	if(!fsu_get_scan_busy_flag() == 1)
	{
		printf("\n(YHCHO) %s() - scan already Done !! Just Bypass (%d)\n", __FUNCTION__, fsu_job_num_scan);

		return;
	}

    if (FBMultiPageScanFlag == 1) // FB Scan Done
    {
        faxapp_fb_multiscan_request(FB_MULTI_PAGE_SCAN_DONE);
    }
    else if (FBMultiPageScanFlag == 2)// FB Scan Go
    {
		if(fb_legal == TRUE)
		{
			faxapp_fb_legal_scan_second_page_start();
		}
		else
        {
			faxapp_fb_multiscan_request(FB_MULTI_PAGE_SCAN_GO);
		}
    }
    else // Cancel
    {
        faxapp_fb_multiscan_request(FB_MULTI_PAGE_SCAN_CANCELED);
    }
}


/********************************************************************************
 *  Function: faxapp_request_print_report()
 *
 *  Description:
 *    Print Fax Report
 *
 *  Inputs:
 *    UINT8 report_num: Report Number
 *
 *  Output:
 *
 ********************************************************************************/
static void faxapp_request_print_report (UINT8 report_num)
{
	faxapp_print_report(report_num);
}


/********************************************************************************
 *  Function: faxapp_update_tx_image_width()
 *
 *  Description:
 *    Set transmit image width - A3, B4, A4
 *
 *  Inputs:
 *    int: Size for original
 *
 *  Output:
 *
 ********************************************************************************/
static void faxapp_update_tx_image_width (int OriginalSizeCode)
{
    UINT8 width = 0;

    switch (OriginalSizeCode)
    {
        case MEDIASIZE_A4:
        case MEDIASIZE_LETTER:
        case MEDIASIZE_LEGAL:
        case MEDIASIZE_A5:
        case MEDIASIZE_A5_ROTATED:
        case MEDIASIZE_A6:
            width = 0; // WIDTH_A4
            break;

        case MEDIASIZE_B5:
            width = 1; // WIDTH_B4
            break;

        case MEDIASIZE_A3:
        case MEDIASIZE_11X17:
            width = 2; // WIDTH_A3
            break;

        default:
            width = 0;
            break;
    }

    FaxAppParam.width = width;
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/********************************************************************************
 *  Function: faxapp_update_file_name_tx()
 *
 *  Description:
 *    update file_name_tx for transmit
 *
 *  Inputs:
 *
 *  Output:
 *
 ********************************************************************************/
void faxapp_update_file_name_tx(void)
{
    struct timeval val;
    struct tm *ptm;
    char CurrentLocalTime[MAX_LEN_FILE_PATH_NAME] = {0, };

    memset(FaxAppParam.file_name, 0x00, sizeof(FaxAppParam.file_name));

	if(isDKSolution && (strlen(shm_backup_for_send.fileName) > 0))
	{
		memcpy(FaxAppParam.file_name, shm_backup_for_send.fileName, strlen(shm_backup_for_send.fileName)+1);
	}
	else
	{
		gettimeofday(&val, NULL);

		ptm = localtime(&val.tv_sec);

		if (ptm == NULL)
		{
			snprintf(CurrentLocalTime, sizeof(CurrentLocalTime), "F%04d%02d%02d%02d%02d%02d%06d_%s", 1900, 1, 1, 0, 0, 0, 0, gFaxUserId);
		}
		else
		{
			snprintf(CurrentLocalTime, sizeof(CurrentLocalTime), "F%04d%02d%02d%02d%02d%02d%06ld_%s",
						ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, val.tv_usec, gFaxUserId);
		}

		if (memcpy(FaxAppParam.file_name, CurrentLocalTime, strlen(CurrentLocalTime) + 1) == (void *)NULL)
		{
			ASSERT(0);
		}
	}

    printf("\n(YHCHO) %s() - FaxAppParam.file_name_tx: %s\n", __func__, FaxAppParam.file_name);
}
#endif


/********************************************************************************
 *  Function: faxapp_request_reset_db()
 *
 *  Description:
 *    Delete all records depending on the IndexToDB
 *
 *  Inputs:
 *    UINT8 IndexToDB: Delete which DB - RECD_FAXLOG, RECD_FAXSPLOG
 *
 *  Output:
 *
 ********************************************************************************/
static void faxapp_request_reset_db (UINT8 IndexToDB)
{
    if (IndexToDB == 0) // TCR (RECD_FAXLOG)
    {
        faxapp_delete_all_records(FAX_REPORT_TCR);
    }
    else // TCR For Fax Service (RECD_FAXSPLOG)
    {
        faxapp_delete_all_records(FAX_REPORT_TCR_FOR_FAX_SERVICE);
    }
}


/********************************************************************************
 *  Function: faxapp_requset_go_fax_sp_mode ()
 *
 *  Description:
 *    Supports test mode for modem
 *
 *  Inputs:
 *
 *  Output:
 *
 ********************************************************************************/
static void faxapp_requset_go_fax_sp_mode (UINT8 Param, UINT16 SubParam, UINT8 Execution)
{
    printf("\n(YHCHO) %s() - Param: %d\n", __FUNCTION__, Param);

    switch (Param)
    {
        case FAXAPP_SP_EXE_PARA_NO_GENERATE_TONE_ANSWER_TONE:
        {
            printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_PARA_NO_GENERATE_TONE_ANSWER_TONE -> Execution: %d\n", __FUNCTION__, Execution);

            switch (Execution)
            {
                case 0:
                {
                    fsu_stop_homologation_signal();

                    break;
                }

                case 1:
                {
                    fsu_start_homologation_signal(ANSAM_2100HZ);

                    break;
                }

                default:
                {
                    break;
                }
            }

            break;
        }

        case FAXAPP_SP_EXE_PARA_NO_GENERATE_TONE_DTMF:
        {
            switch (SubParam)
            {
                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_0:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_0 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_0);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_1:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_1 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_1);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_2:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_2 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_2);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_3:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_3 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_3);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_4:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_4 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_4);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_5:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_5 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_5);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_6:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_6 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_6);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_7:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_7 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_7);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_8:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_8 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_8);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_9:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_9 -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_9);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_STAR:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_STAR -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_STAR);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_POUND:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_DTMF_POUND -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(DTMF_POUND);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                default:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_PARA_NO_GENERATE_TONE_DTMF -> Invalid SubParam: %d\n", __FUNCTION__, SubParam);

                    break;
                }
            }

            break;
        }

        case FAXAPP_SP_EXE_PARA_NO_GENERATE_TONE_2100:
        {
            switch (Execution)
            {
                case 0:
                {
                    fsu_stop_homologation_signal();

                    break;
                }

                case 1:
                {
                    fsu_start_homologation_signal(CED_2100HZ);

                    break;
                }

                default:
                {
                    break;
                }
            }

            break;
        }

        case FAXAPP_SP_EXE_PARA_NO_GENERATE_TONE_1100:
        {
            switch (Execution)
            {
                case 0:
                {
                    fsu_stop_homologation_signal();

                    break;
                }

                case 1:
                {
                    fsu_start_homologation_signal(CNG_1100HZ);

                    break;
                }

                default:
                {
                    break;
                }
            }

            break;
        }

        case FAXAPP_SP_EXE_PARA_NO_GENERATE_TONE_MODULATION:
        {
            switch (SubParam)
            {
                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V21_300BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V21_300BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V21_CH2);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V27TER_2400BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V27TER_2400BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V27TER_2400);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V27TER_4800BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V27TER_4800BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V27TER_4800);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V29_7200BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V29_7200BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V29_7200);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V29_9600BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V29_9600BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V29_9600);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_7200BPS_L:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_7200BPS_L -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V17_7200_LONG);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_7200BPS_S:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_7200BPS_S -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V17_7200_SHORT);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_9600BPS_L:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_9600BPS_L -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V17_9600_LONG);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_9600BPS_S:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_9600BPS_S -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V17_9600_SHORT);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_12000BPS_L:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_12000BPS_L -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V17_12000_LONG);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_12000BPS_S:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_12000BPS_S -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V17_12000_SHORT);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_14400BPS_L:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_14400BPS_L -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V17_14400_LONG);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_14400BPS_S:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V17_14400BPS_S -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V17_14400_SHORT);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_2400BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_2400BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_2400);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_4800BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_4800BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_4800);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_7200BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_7200BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_7200);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_9600BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_9600BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_9600);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_12000BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_12000BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_12000);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_14400BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_14400BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_14400);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_16800BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_16800BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_16800);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_19200BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_19200BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_19200);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_21600BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_21600BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_21600);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_24000BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_24000BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_24000);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_26400BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_26400BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_26400);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_28800BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_28800BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_28800);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_31200BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_31200BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_31200);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_33600BPS:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_33600BPS -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_33600);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_2400BPS_BAUD:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_2400BPS_BAUD -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_2400_BAUD);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_2800BPS_BAUD:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_2800BPS_BAUD -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_2800_BAUD);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_3000BPS_BAUD:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_3000BPS_BAUD -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_3000_BAUD);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_3200BPS_BAUD:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_3200BPS_BAUD -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_3200_BAUD);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                case FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_3429BPS_BAUD:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_SUB_PARA_NO_GENERATE_TONE_MODULATION_V34_3429BPS_BAUD -> Execution: %d\n", __FUNCTION__, Execution);

                    switch (Execution)
                    {
                        case 0:
                        {
                            fsu_stop_homologation_signal();

                            break;
                        }

                        case 1:
                        {
                            fsu_start_homologation_signal(V34_3429_BAUD);

                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }

                    break;
                }

                default:
                {
                    printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_PARA_NO_GENERATE_TONE_MODULATION -> Invalid SubParam: %d\n", __FUNCTION__, SubParam);

                    break;
                }
            }

            break;
        }

        case FAXAPP_SP_EXE_PARA_NO_DETECT_RING:
        {
            printf("\n(YHCHO) %s() - FAXAPP_SP_EXE_PARA_NO_DETECT_RING -> Execution: %d\n", __FUNCTION__, Execution);

            fsu_set_answer_mode(Execution);

            break;
        }

        case FAXAPP_SP_EXE_PARA_NO_ON_HOOK:
        {
            switch (Execution)
            {
                case 0:
                {
                    Fax_API_OnHook();

                    break;
                }

                case 1:
                {
                    Fax_API_OffHook();

                    break;
                }

                default:
                {
                    break;
                }
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


/********************************************************************************
 *  Function: faxapp_requset_go_fax_ack_read_db ()
 *
 *  Description:
 *    To determine whether to clear the RECD_FAXIMAGEBACKUP
 *
 *  Inputs:
 *    UINT8 Result: result
 *
 *  Output:
 *
 ********************************************************************************/
static void faxapp_requset_go_fax_ack_read_db (UINT8 Result)
{
    if (Result == 0) // Success
    {
        faxapp_delete_all_records_recd_faximagebackup();
    }
    else // Fail
    {
        usleep(1000*1000); // To give enough time for the GUI to process...

        faxapp_notify_SendFaxRequestReadDB(ValidJobCnt);
    }
}


/********************************************************************************
 *  Function: faxapp_delete_docman ()
 *
 *  Description:
 *    Delete DocMan/SolDocMan (SP Mode)
 *
 *  Inputs:
 *    UINT8 Directory: directory
 *
 *  Output:
 *
 ********************************************************************************/
static void faxapp_delete_docman (UINT8 Directory)
{
	printf("\n(YHCHO) %s() - Directory: %d\n", __FUNCTION__, Directory);

	fsu_delete_docman(Directory);
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: faxapp_request_update_user_id()
 *
 *  Description:
 *    update the gFaxUserId set by System Service Layer (SSL)
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_request_update_user_id (void)
{
    snprintf(gFaxUserId, sizeof(gFaxUserId), "%s", GetCurerntAuthUser());
}

#else
/***********************************************************************
 *  Function: faxapp_request_update_user_id()
 *
 *  Description:
 *    update the gFaxUserId set by System Service Layer (SSL)
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_request_update_user_id (void)
{
    char UserId[MAX_LEN_USER_ID + 1] = {0, };
    UINT8 Len = 0;

    snprintf(UserId, sizeof(UserId), "%s", GetCurerntAuthUser());

    printf("\n(YHCHO) %s() - pUserId: %s\n", __func__, UserId);

    Len = strlen(UserId);

    if (Len == 0)
    {
        snprintf(gFaxUserId, sizeof(gFaxUserId), "%c", 0x20);
    }
    else
    {
        snprintf(gFaxUserId, sizeof(gFaxUserId), "%s", UserId);
    }
}
#endif


/***********************************************************************
 *  Function: faxapp_get_OnHookButtonPressed()
 *
 *  Description:
 *    get OnHookButtonPressed
 *
 *  Inputs:
 *
 *  Output:
 *
 **********************************************************************/
int faxapp_get_OnHookButtonPressed()
{
	return OnHookButtonPressed;
}


/***********************************************************************
 *  Function: faxapp_set_OnHookButtonPressed()
 *
 *  Description:
 *    update the OnHookButtonPressed
 *
 *  Inputs:
 *		int value : ON_HOOK_BUTTON_t enum
 *  Output:
 *
 **********************************************************************/
void faxapp_set_OnHookButtonPressed(int value)
{
	OnHookButtonPressed = value;
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
/***********************************************************************
 *  Function: faxapp_get_file_name()
 *
 *  Description:
 *    get the file name saved in share mem.
 *
 *  Inputs:
 *    UINT32 SharedMemID: ID of shared memory
 *
 *  Output:
 *    filename
 **********************************************************************/
int faxapp_get_file_name (UINT32 ShareMemID, char *FileName)
{
    shm_SolFaxFileName *pFileName;
    int ret_val = 0;

    pFileName = shm_attach(ShareMemID);

    memset(nameShareMem, 0x00, sizeof(nameShareMem));

    if ( pFileName != (void *)-1 )
    {
        snprintf(nameShareMem, sizeof(nameShareMem), "%s", pFileName->fileName);

        printf("\n(YHCHO) %s() - nameShareMem: %s\n", __FUNCTION__, nameShareMem);
        
        snprintf(FileName,sizeof(nameShareMem),"%s",nameShareMem);
    }
    else
    {
        printf("\n(YHCHO) %s() - shm_attach() Error!\n", __FUNCTION__);
        return ret_val;
    }

    ret_val = shm_detach((void *)pFileName);

    if ( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_detach() Error!\n", __FUNCTION__);
        return ret_val;
    }

    ret_val = shm_remove(ShareMemID);

    if ( ret_val < 0 )
    {
        printf("\n(YHCHO) %s() - shm_remove() Error!\n", __FUNCTION__);
        return ret_val;
    }

    return 1;
}


/***********************************************************************
 *  Function: faxapp_go_solution_fax()
 *
 *  Description:
 *    start point of solution fax
 *
 *  Inputs:
 *    UINT8 Type: Job Type
 *    UINT32 SharedMemID: ID of shared memory
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_go_solution_fax (UINT8 Type, UINT32 ShareMemID)
{
    UINT16 Index = 0;
    //char nameSavedInJobQueue[MAX_LAN_FILE_NAME + 1] = {0, };
	char *nameSavedInJobQueue = NULL;
	char nameSavedInShareMem[SDH_FILENAME_MAX] = {0, };
	int ret = 0;
	BOOLEAN IsMatchFileName = FALSE;

	faxapp_get_file_name(ShareMemID, &nameSavedInShareMem[0]);

	for (Index = 0; Index < TX_JOBS; Index++)
	{
		nameSavedInJobQueue = fsu_get_file_name_tx(Index);

		if (nameSavedInJobQueue[0] != '\0')
		{
			if (strncmp(nameSavedInJobQueue, nameSavedInShareMem, strlen(nameSavedInShareMem) + 1) == 0)
			{
				printf("\n(YHCHO) %s() - Index: %d\n", __FUNCTION__, Index);
				IsMatchFileName = TRUE;
				break;
			}
		}
	}
	
	printf("\n(YHCHO) %s() - IsMatchFileName: %d\n", __FUNCTION__, IsMatchFileName);
	
	if(IsMatchFileName == FALSE)
	{
		faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_FILENAME_MISMATCH, 0, 0, 0, 0, EMPTY_JOB_NUM);
	}
	else
	{
		if (Type == 0)
		{
			if(Index == TX_JOBS)
			{
				//...
			}
			else
			{
				if(fsu_get_broadcast(Index) == 1)
				{
					int i = 0;
					UINT16 originalDocId = fsu_get_original_doc_id(Index);
					for(i=0; i< gFax_BC_Num[originalDocId].BC_Total_Num; i++)
					{
						UINT16 broadcast_jnum = gFaxBroadcastJobInfo[originalDocId][i];
						
						Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_PERMISSION, broadcast_jnum, SOLUTION_PERMISSION_STATE_ENABLE);
						fsu_update_send_permission (broadcast_jnum, SOLUTION_PERMISSION_STATE_ENABLE);
					}
				}
				else
				{
					Update_Job_Queue_File(IMAGE_BACKUP_REASON_UPDATE_PERMISSION, Index, SOLUTION_PERMISSION_STATE_ENABLE);
					fsu_update_send_permission (Index, SOLUTION_PERMISSION_STATE_ENABLE);
				}
			}
		}
		else
		{
			if(Index == TX_JOBS)
			{
				faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DELETE_FAIL, 0, 0, 0, 0, TX_JOBS);
			}
			else
			{
				// Cancel... How To Process?
				ret = fsu_delete_solution_job(Index);
				
				if(ret == OK)
					faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DELETE_COMPLETE, 0, 0, 0, 0, Index);
				else
					faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_DELETE_FAIL, 0, 0, 0, 0, Index);
			}
		}
	}
}
#endif

/***********************************************************************
 *  Function: faxapp_set_delay_time_in_string_job_log()
 *
 *  Description:
 *    update the gFaxDelayTimeInStringJobLog for Job Log
 *
 *  Inputs:
 *		const char *pDelayTime : pointer to delay time
 *
 *  Output:
 *
 **********************************************************************/
void faxapp_set_delay_time_in_string_job_log (const char *pDelayTime) // 2016.01.01 05:55
{
    UINT8 Country = getCountryDateType();
    UINT8 Hour = 0;
    char *pTimeToTx = (char *)NULL;

    pTimeToTx = (char *)MEM_MALLOC(strlen(pDelayTime) + 1);

    if (pTimeToTx == (char *)NULL)
    {
        ASSERT(0);

        return;
    }

    if (memcpy(pTimeToTx, pDelayTime, strlen(pDelayTime) + 1) == (void *)NULL)
    {
        ASSERT(0);

        return;
    }

    Hour = fsu_convert_char_to_int(pTimeToTx[11]) * pow(10, 1) + fsu_convert_char_to_int(pTimeToTx[12]) * pow (10, 0);

    //printf("\n(YHCHO) %s() - Hour: %d\n", __func__, Hour);

    switch (Country)
    {
        case FAX_COUNTRY_DATE_TYPE_YMD:
        case FAX_COUNTRY_DATE_TYPE_MDY:
        {
            if (Hour == 0)
            {
                gFaxDelayTimeInStringJobLog[14] = pTimeToTx[5];
                gFaxDelayTimeInStringJobLog[13] = pTimeToTx[6];
                gFaxDelayTimeInStringJobLog[12] = '-';
                gFaxDelayTimeInStringJobLog[11] = pTimeToTx[8];
                gFaxDelayTimeInStringJobLog[10] = pTimeToTx[9];
                gFaxDelayTimeInStringJobLog[9] = 0x20;
                gFaxDelayTimeInStringJobLog[8] = 'A';
                gFaxDelayTimeInStringJobLog[7] = 'M';
                gFaxDelayTimeInStringJobLog[6] = 0x20;
                gFaxDelayTimeInStringJobLog[5] = '1';
                gFaxDelayTimeInStringJobLog[4] = '2';
                gFaxDelayTimeInStringJobLog[3] = ':';
                gFaxDelayTimeInStringJobLog[2] = pTimeToTx[14];
                gFaxDelayTimeInStringJobLog[1] = pTimeToTx[15];
                gFaxDelayTimeInStringJobLog[0] = 0;
            }
            else if ( (Hour >= 1) && (Hour <= 11) )
            {
                gFaxDelayTimeInStringJobLog[14] = pTimeToTx[5];
                gFaxDelayTimeInStringJobLog[13] = pTimeToTx[6];
                gFaxDelayTimeInStringJobLog[12] = '-';
                gFaxDelayTimeInStringJobLog[11] = pTimeToTx[8];
                gFaxDelayTimeInStringJobLog[10] = pTimeToTx[9];
                gFaxDelayTimeInStringJobLog[9] = 0x20;
                gFaxDelayTimeInStringJobLog[8] = 'A';
                gFaxDelayTimeInStringJobLog[7] = 'M';
                gFaxDelayTimeInStringJobLog[6] = 0x20;
                gFaxDelayTimeInStringJobLog[5] = pTimeToTx[11];
                gFaxDelayTimeInStringJobLog[4] = pTimeToTx[12];
                gFaxDelayTimeInStringJobLog[3] = ':';
                gFaxDelayTimeInStringJobLog[2] = pTimeToTx[14];
                gFaxDelayTimeInStringJobLog[1] = pTimeToTx[15];
                gFaxDelayTimeInStringJobLog[0] = 0;
            }
            else if (Hour == 12)
            {
                gFaxDelayTimeInStringJobLog[0] = pTimeToTx[5];
                gFaxDelayTimeInStringJobLog[1] = pTimeToTx[6];
                gFaxDelayTimeInStringJobLog[2] = '-';
                gFaxDelayTimeInStringJobLog[3] = pTimeToTx[8];
                gFaxDelayTimeInStringJobLog[4] = pTimeToTx[9];
                gFaxDelayTimeInStringJobLog[5] = 0x20;
                gFaxDelayTimeInStringJobLog[6] = 'P';
                gFaxDelayTimeInStringJobLog[7] = 'M';
                gFaxDelayTimeInStringJobLog[8] = 0x20;
                gFaxDelayTimeInStringJobLog[9] = '1';
                gFaxDelayTimeInStringJobLog[10] = '2';
                gFaxDelayTimeInStringJobLog[11] = ':';
                gFaxDelayTimeInStringJobLog[12] = pTimeToTx[14];
                gFaxDelayTimeInStringJobLog[13] = pTimeToTx[15];
                gFaxDelayTimeInStringJobLog[14] = 0;
            }
            else
            {
                UINT8 ret = Hour - 12;
                div_t div_value;

                gFaxDelayTimeInStringJobLog[0] = pTimeToTx[5];
                gFaxDelayTimeInStringJobLog[1] = pTimeToTx[6];
                gFaxDelayTimeInStringJobLog[2] = '-';
                gFaxDelayTimeInStringJobLog[3] = pTimeToTx[8];
                gFaxDelayTimeInStringJobLog[4] = pTimeToTx[9];
                gFaxDelayTimeInStringJobLog[5] = 0x20;
                gFaxDelayTimeInStringJobLog[6] = 'P';
                gFaxDelayTimeInStringJobLog[7] = 'M';
                gFaxDelayTimeInStringJobLog[8] = 0x20;

                div_value = div(ret, 10);

                gFaxDelayTimeInStringJobLog[9] = fsu_convert_int_to_char(div_value.quot);
                gFaxDelayTimeInStringJobLog[10] = fsu_convert_int_to_char(div_value.rem);

                gFaxDelayTimeInStringJobLog[11] = ':';
                gFaxDelayTimeInStringJobLog[12] = pTimeToTx[14];
                gFaxDelayTimeInStringJobLog[13] = pTimeToTx[15];
                gFaxDelayTimeInStringJobLog[14] = 0;
            }

            break;
        }

        case FAX_COUNTRY_DATE_TYPE_DMY:
        {
            if (Hour == 0)
            {
                gFaxDelayTimeInStringJobLog[14] = pTimeToTx[8];
                gFaxDelayTimeInStringJobLog[13] = pTimeToTx[9];
                gFaxDelayTimeInStringJobLog[12] = '-';
                gFaxDelayTimeInStringJobLog[11] = pTimeToTx[5];
                gFaxDelayTimeInStringJobLog[10] = pTimeToTx[6];
                gFaxDelayTimeInStringJobLog[9] = 0x20;
                gFaxDelayTimeInStringJobLog[8] = 'A';
                gFaxDelayTimeInStringJobLog[7] = 'M';
                gFaxDelayTimeInStringJobLog[6] = 0x20;
                gFaxDelayTimeInStringJobLog[5] = '1';
                gFaxDelayTimeInStringJobLog[4] = '2';
                gFaxDelayTimeInStringJobLog[3] = ':';
                gFaxDelayTimeInStringJobLog[2] = pTimeToTx[14];
                gFaxDelayTimeInStringJobLog[1] = pTimeToTx[15];
                gFaxDelayTimeInStringJobLog[0] = 0;
            }
            else if ( (Hour >= 1) && (Hour <= 11) )
            {
                gFaxDelayTimeInStringJobLog[14] = pTimeToTx[8];
                gFaxDelayTimeInStringJobLog[13] = pTimeToTx[9];
                gFaxDelayTimeInStringJobLog[12] = '-';
                gFaxDelayTimeInStringJobLog[11] = pTimeToTx[5];
                gFaxDelayTimeInStringJobLog[10] = pTimeToTx[6];
                gFaxDelayTimeInStringJobLog[9] = 0x20;
                gFaxDelayTimeInStringJobLog[8] = 'A';
                gFaxDelayTimeInStringJobLog[7] = 'M';
                gFaxDelayTimeInStringJobLog[6] = 0x20;
                gFaxDelayTimeInStringJobLog[5] = pTimeToTx[11];
                gFaxDelayTimeInStringJobLog[4] = pTimeToTx[12];
                gFaxDelayTimeInStringJobLog[3] = ':';
                gFaxDelayTimeInStringJobLog[2] = pTimeToTx[14];
                gFaxDelayTimeInStringJobLog[1] = pTimeToTx[15];
                gFaxDelayTimeInStringJobLog[0] = 0;
            }
            else if (Hour == 12)
            {
                gFaxDelayTimeInStringJobLog[0] = pTimeToTx[8];
                gFaxDelayTimeInStringJobLog[1] = pTimeToTx[9];
                gFaxDelayTimeInStringJobLog[2] = '-';
                gFaxDelayTimeInStringJobLog[3] = pTimeToTx[5];
                gFaxDelayTimeInStringJobLog[4] = pTimeToTx[6];
                gFaxDelayTimeInStringJobLog[5] = 0x20;
                gFaxDelayTimeInStringJobLog[6] = 'P';
                gFaxDelayTimeInStringJobLog[7] = 'M';
                gFaxDelayTimeInStringJobLog[8] = 0x20;
                gFaxDelayTimeInStringJobLog[9] = '1';
                gFaxDelayTimeInStringJobLog[10] = '2';
                gFaxDelayTimeInStringJobLog[11] = ':';
                gFaxDelayTimeInStringJobLog[12] = pTimeToTx[14];
                gFaxDelayTimeInStringJobLog[13] = pTimeToTx[15];
                gFaxDelayTimeInStringJobLog[14] = 0;
            }
            else
            {
                UINT8 ret = Hour - 12;
                div_t div_value;

                gFaxDelayTimeInStringJobLog[0] = pTimeToTx[8];
                gFaxDelayTimeInStringJobLog[1] = pTimeToTx[9];
                gFaxDelayTimeInStringJobLog[2] = '-';
                gFaxDelayTimeInStringJobLog[3] = pTimeToTx[5];
                gFaxDelayTimeInStringJobLog[4] = pTimeToTx[6];
                gFaxDelayTimeInStringJobLog[5] = 0x20;
                gFaxDelayTimeInStringJobLog[6] = 'P';
                gFaxDelayTimeInStringJobLog[7] = 'M';
                gFaxDelayTimeInStringJobLog[8] = 0x20;

                div_value = div(ret, 10);

                gFaxDelayTimeInStringJobLog[9] = fsu_convert_int_to_char(div_value.quot);
                gFaxDelayTimeInStringJobLog[10] = fsu_convert_int_to_char(div_value.rem);

                gFaxDelayTimeInStringJobLog[11] = ':';
                gFaxDelayTimeInStringJobLog[12] = pTimeToTx[14];
                gFaxDelayTimeInStringJobLog[13] = pTimeToTx[15];
                gFaxDelayTimeInStringJobLog[14] = 0;
            }

            break;
        }

        default:
            break;
    }

    MEM_FREE_AND_NULL(pTimeToTx);
}

