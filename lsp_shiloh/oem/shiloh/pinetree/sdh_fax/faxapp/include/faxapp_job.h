#ifndef __FAXAPP_JOB__
#define __FAXAPP_JOB__

#include "SDHFaxCommonHeader.h"
#include "posix_ostools.h"
#include "../../../../../../../../common_header/SysSharedMemory.h"

/* */
#define A4_HEIGHT 3432
#define A4_PIXEL_PER_MM_300DPI (3432/297)


typedef enum
{
    FAX_APP_MSG_GO_FAX_SCAN = 0x0801,
    FAX_APP_MSG_GO_POWER_SAVE_MODE = 0x0802,
	FAX_APP_MSG_GO_ON_HOOK = 0x0803,
	FAX_APP_MSG_GO_UPDATE_PARAM = 0x0804,
	FAX_APP_MSG_GO_FAX_SCAN_MORE = 0x0805,
	FAX_APP_MSG_GO_FAX_CANCEL = 0x0806,
	FAX_APP_MSG_GO_FAX_DELETE_PENDING_JOB = 0x0807,
	FAX_APP_MSG_GO_FAX_PRINT_REPORT = 0x0808,
    FAX_APP_MSG_GO_FAX_RESET_DB = 0x0809,
    FAX_APP_MSG_GO_FAX_SP_MODE = 0x080A,
    FAX_APP_MSG_GO_FAX_ACK_READ_DB = 0x080B,
    FAX_APP_MSG_GO_FAX_DELETE_DOCMAN = 0x080C,

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    FAX_APP_MSG_GO_SOLUTION_FAX = 0x080F,
#endif

#if __SUPPORT_MANUAL_RECEIVE__
    FAX_APP_MSG_GO_FAX_RECEIVE = 0x080D,
#else
#endif

    FAX_APP_MSG_SEND_FAX_STATUS = 0x0881,
    FAX_APP_MSG_SEND_FAX_MEMORY_STATUS = 0x0882,
    FAX_APP_MSG_SEND_FAX_REQUEST_READ_DB = 0x0883,
    FAX_APP_MSG_SEND_FAX_BROADCAST_JOBID = 0x0884,
    FAX_APP_MSG_SEND_PCFAX_JOBID = 0x0885,

    FAX_APP_MSG_LAST

} FAX_APP_MSG_t;


typedef enum {
    FAX_VR_STANDARD,
    FAX_VR_FINE,
    FAX_VR_SUPERFINE,
    FAX_VR_300,
    FAX_VR_ULTRAFINE,

    FAX_VR_LAST
} FAX_VR_t;


typedef enum {
    FB_MULTI_PAGE_SCAN_INIT, // 0
    FB_MULTI_PAGE_SCAN_DONE, // 1
    FB_MULTI_PAGE_SCAN_GO, // 2
    FB_MULTI_PAGE_SCAN_CANCELED, // 3

    FB_MULTI_PAGE_SCAN_LAST
} FB_MULTI_PAGE_SCAN_t;


/* The following all members of FAXAPP_SCAN_PARAM_t should be set when faxapp gets the FAX_APP_MSG_GO_FAX_SCAN  message from Sysmgr */
typedef struct {
    int TxJobType;
    int NoOfRecipient;
    int TimeToTxHour;
    int TimeToTxMin;
    int OriginalSizeCode;
    int Resolution;
    UINT8 ImageType;
    int Density;
    int DualScan;
    UINT32 SharedMemID;
    int AutoDarkness;
} FAXAPP_SCAN_PARAM_t;


typedef struct {
    FAX_APP_JOB_TYPE_t fax_type;
    UINT8 width;
    UINT8 no_of_recipient;
    long time_diff;
    UINT8 dial_num[MAX_NUM_BC][LEN_PHONE_NO + 1];
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    char file_name[MAX_LEN_FILE_PATH_NAME + 1];
#endif
} FAXAPP_PARAMS_t;


#define FAXAPPTASK_QUEUE_SIZE     100
#define FAXAPP_JOB_STACK_SIZE  (MAX((1024 * 10 * 3), POSIX_MIN_STACK_SIZE))
#define GET     0
#define SET     1


void FaxAppJobMgrInit(void);
void faxapp_update_tx_job_type(int TxJobType);
void faxapp_update_no_of_recipient(int NoOfRecipient);
void faxapp_update_time_diff(int TxJobType, int TimeToTxHour, int TimeToTxMin);
void faxapp_update_dial_num(int NoOfRecipient, UINT32 ShareMemID);
void faxapp_set_delay_time_in_string(UINT8 TimeToTxHour, UINT8 TimeToTxMin);
void faxapp_go_on_hook(UINT8 KeyValue);
void faxapp_activate_off_hook (void);
void faxapp_activate_on_hook (void);
void faxapp_activate_manual_fax (void);
void faxapp_activate_on_hook_dial (UINT8 InputValue);
void faxapp_set_delay_time_in_string_job_log (const char *pDelayTime);
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
void faxapp_update_file_name_tx(void);
void faxapp_update_dial_num_and_ftp_info(int NoOfRecipient, UINT32 ShareMemID);
void faxapp_go_solution_fax (UINT8 Type, UINT32 ShareMemID);
#endif
int faxapp_check_on_hook_button_pressed (void);
char* faxapp_get_ftpAddress_fax_solution(void);
char* faxapp_get_ftpID_fax_solution(void);
char* faxapp_get_ftpPassword_fax_solution(void);
int faxapp_get_ftpPort_fax_solution(void);
UINT8 faxapp_get_no_of_recipient(void);
UINT8 faxapp_get_job_type(void);
UINT8 faxapp_get_resolution(void);

extern void fsu_start_scan_to_mem (FAXAPP_PARAMS_t *FaxScanStruct);
extern void fsu_start_poll_receive (FAXAPP_PARAMS_t *FaxScanStruct);
extern void Fax_API_OffHook(void);
extern void Fax_API_OnHook(void);
extern void fsu_set_prn_job_ready(UINT16 jnum);
extern void fsu_check_poll_tx_job(UINT16 jnum);
extern void Fax_API_ManTransmitFax(void);
extern void Fax_API_Dial(FAXAPP_PARAMS_t *pFaxStruct);
extern void fsu_get_job_start_time (long CurrentTimeInSec);
extern void faxapp_update_parameter (UINT8 Param);
extern void ModemSleep(UINT8 Mode);
extern void ModemWakeup(void);
extern void fsu_set_speaker_volume (int Volume);
extern void faxapp_delete_all_records (UINT8 Reason);
extern void fsu_cancel_fax_job(char job_type);
extern void fsu_delete_pending_job(UINT16 jnum);
extern void fsu_stop_homologation_signal (void);
extern void fsu_start_homologation_signal (UINT8 Signal);
extern void fsu_set_answer_mode (UINT8 AnswerMode);
extern void faxapp_delete_all_records_recd_faximagebackup (void);
extern void fsu_request_image_backup_job_begin (int Cnt);
extern inline long int fsu_calculate_time_diff_in_sec (int Year, int Month, int Day, int Hour, int Min);
extern UINT8 getCountryDateType(void);
extern UINT8 fsu_convert_char_to_int (char No);
extern char fsu_convert_int_to_char (int No);
extern char *GetCurerntAuthUser(void);

#if __MEASURE_PERFORMANCE__
extern void faxapp_get_job_start_time(int JobType);
#endif // #if __MEASURE_PERFORMANCE__

extern UINT8 fsu_get_speaker_volume (void);
extern long fsu_get_current_system_time(void);

#endif // #ifndef __FAXAPP_JOB__
