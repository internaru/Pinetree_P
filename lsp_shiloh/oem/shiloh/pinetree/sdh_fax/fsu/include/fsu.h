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

#ifndef __FAX_SCHD_H__
  #define __FAX_SCHD_H__

/******************************************************************************
*  FILE FORMAT:
*      1. ENUMERATIONS
*      2. DEFINES
*      3. STRUCTURES
*      4. ALLOCS
*      5. FUNCTIONS
******************************************************************************/

#include "SysUI_Interface.h"
#include "SDHFaxCommonHeader.h"


/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/
typedef enum {
    FSU_STATE_NULL,     // 0
    FSU_STATE_SCANNING,     // 1
    FSU_STATE_SENDING_POLL_READY,     // 2
    FSU_STATE_SENDING_READY,     // 3
    FSU_STATE_RECEIVING,     // 4
    FSU_STATE_RECEIVE_DONE,     // 5
    FSU_STATE_PRINT_READY,     // 6
    FSU_STATE_RETRY_READY,     // 7
    FSU_STATE_SENDING_PENDING,     // 8

    FSU_STATE_LAST
} FSU_STATE_t;


typedef enum {
    JOB_TYPE_FOR_REPORT_TX, // 0
    JOB_TYPE_FOR_REPORT_RX, // 1
    JOB_TYPE_FOR_REPORT_POLL_TRANSMIT, // 2
    JOB_TYPE_FOR_REPORT_FORWARD, // 3
    JOB_TYPE_FOR_REPORT_ON_HOOK, // 4
    JOB_TYPE_FOR_REPORT_POLL_RECEIVE, // 5
    JOB_TYPE_FOR_REPORT_BROADCAST, // 6
    JOB_TYPE_FOR_REPORT_DELAY, // 7

    JOB_TYPE_FOR_REPORT_LAST

} JOB_TYPE_FOR_REPORT_t;


typedef enum {
    FSU_CHECK_IMAGE_FILE_VALIDATION_OK, // 0
    FSU_CHECK_IMAGE_FILE_VALIDATION_DOES_NOT_EXIST, // 1
    FSU_CHECK_IMAGE_FILE_VALIDATION_FILE_SIZE_ZERO, // 2
    FSU_CHECK_IMAGE_FILE_VALIDATION_T4_ENCODE_FAIL, // 3

    FSU_CHECK_IMAGE_FILE_VALIDATION_LAST

} FSU_CHECK_IMAGE_FILE_VALIDATION_t;


typedef enum {
    BROADCAST_RESULT_REPORT_REASON_JOB_ID, // 0
    BROADCAST_RESULT_REPORT_REASON_START_TIME, // 1
    BROADCAST_RESULT_REPORT_REASON_DURATION, // 2
    BROADCAST_RESULT_REPORT_REASON_TOTAL_PAGE_NO, //3
    BROADCAST_RESULT_REPORT_REASON_SENT_PAGE_NO, // 4
    BROADCAST_RESULT_REPORT_REASON_RESULT, // 5

    BROADCAST_RESULT_REPORT_REASON_LAST

} BROADCAST_RESULT_REPORT_REASON_t;


/******************************************************************************
*  2. DEFINES
******************************************************************************/


/******************************************************************************
*  3. STRUCTURES
******************************************************************************/
typedef struct {
    UINT8 fax_num[LEN_PHONE_NO + 1];
    BOOL isDelay;
    char DelayTimeInText[MAX_LEN_FILE_PATH_NAME];
    UINT8 status;
    UINT8 retry;
    UINT8 retry_tmr;
    long tx_time;
    UINT8 res;
    UINT8 width;
    UINT8 ff_job;
    UINT8 poll_transmit;
    UINT8 poll_receive;
    UINT8 broadcast;
    UINT8 flag_manual_tx;
    UINT16 doc_id;
    UINT16 original_doc_id;
    UINT8 TxRxTotalPages;
    UINT8 TxTotalPageNo;
    UINT8 PrintReason;
    char mem_time[6];
    UINT8 photo_mode;
    UINT8 split_print;
    char user_id[MAX_LEN_USER_ID + 1];
    UINT8 isDuplex;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    char file_name_tx[MAX_LEN_FILE_PATH_NAME + 1];
    UINT8 send_permission;
    UINT32 unique_job_id;
    stIC_JobDoneInfo faxScanInfo;
#endif
} TX_JOB_t;


typedef struct {
    UINT16 nxt;              // Index to next available job location
    TX_JOB_t job[TX_JOBS];  // Individual jobs
} TX_QUEUE_t;


typedef struct {
    UINT32 width;
    UINT32 height;
} IMAGE_PARAM_t;


typedef struct {
    IMAGE_PARAM_t job[TX_JOBS];
} FAX_IMAGE_PARAM_t;


typedef struct {
    long DateTime;
    int JobType;
    int ECM;
    int Resolution;
    char RemoteNo[LEN_PHONE_NO + 1];
    long Duration;
    int PageNo;
    int ConSpeed;
    int Result;
    int ModulationName;
} INFO_FOR_REPORT_t;


typedef struct {
    UINT8 BC_Current_Num;
    UINT8 BC_Total_Num;
    UINT8 BC_Abnormal_Count;
} BC_NUMBER_t;


typedef struct shm_faxphoneNumber_ {   //for SHM_KEY_PHONE_NUMBER
	char            no[LEN_PHONE_NO + 1];
} stShmFaxPhoneNumber;

/******************************************************************************
*  4. ALLOCS
******************************************************************************/
  #ifdef __FSU_C__
    TX_QUEUE_t txq;
    INFO_FOR_REPORT_t Report;
    FAX_IMAGE_PARAM_t image;
    char caller_id_name[120];
    char caller_id_num[120];
    UINT16 io_crc;                      
    BC_NUMBER_t gFax_BC_Num[MAX_NUM_DOC_ID];
  #else
    extern INFO_FOR_REPORT_t Report;
    extern TX_QUEUE_t txq;
    extern FAX_IMAGE_PARAM_t image;
    extern char caller_id_name[120];
    extern char caller_id_num[120];
    extern UINT16 io_crc;                      
    extern BC_NUMBER_t gFax_BC_Num[MAX_NUM_DOC_ID];
  #endif

/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/
void fsu_check_tx_job (void);
void fsu_fax_job_init(UINT8 Fax_Status);
void fsu_check_prn_job(void);
UINT8 tx_job_ready (void);
UINT8 time_to_tx (long tx_time);
int fsu_por(void);
int fsu_get_memory_full_status_ram (void);
#endif  // __FAX_SCHD_H__

