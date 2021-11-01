#ifndef __SDH_FAX_COMMON_HEADER__
#define __SDH_FAX_COMMON_HEADER__

/******************************************************************************
*  FILE FORMAT:
*      1. ENUMERATIONS
*      2. DEFINES
*      3. STRUCTURES
*      4. ALLOCS
*      5. FUNCTIONS
******************************************************************************/

/* define for new function / test code */
#define __MEASURE_PERFORMANCE__     (0)
#define __DISPLAY_CURRENT_LOCAL_JOB_TIME__     (0)
#define __SUPPORT_MANUAL_RECEIVE__     (0)
#define __SUPPORT_MANUAL_TRANSMIT__     (0)
#define __SUPPORT_CID__     (0)
#define __OVERRUN_ERROR__     (0)
#define __TX_REPORT_IMAGE_PRINT__ (1)

#define __ONHOOK_TX_WAITING__		(1)
#define __MR_TX_ERR_WORKAROUND__	(1)
#define __HANA_FINANCIAL_GROUP_REQ_014_5__     (1) // 출력물 보안 차별화 기능
#define __HANA_FINANCIAL_GROUP_REQ_021__     (1) // Fax 회선 관리
#define __HAVE_FAXSOLUTION_DK__        (1)

/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/
typedef enum {
    FAX_PRINT_MODE_PRINT_EVERY_PAGE,     // 0 -> Same print mode as that of OAK
    FAX_PRINT_MODE_PRINT_ALL,     // 1 -> Same print mode as that of NEST

    FAX_PRINT_MODE_LAST
} FAX_PRINT_MODE_t;


typedef enum {
    FAX_APP_JOB_TYPE_TX_IMMEDIATE, // 0
    FAX_APP_JOB_TYPE_BROADCAST, // 1
    FAX_APP_JOB_TYPE_DELAY, // 2
    FAX_APP_JOB_TYPE_POLL_TRANSMIT, // 3
    FAX_APP_JOB_TYPE_ON_HOOK, // 4
    FAX_APP_JOB_TYPE_RECEIVE, // 5
    FAX_APP_JOB_TYPE_POLL_RECEIVE, // 6

#if __SUPPORT_MANUAL_TRANSMIT__
    FAX_APP_JOB_TYPE_MANUAL_TRANSMIT, // 7
#else
#endif

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    FAX_APP_JOB_TYPE_FAX_SOLUTION = 8,
#endif

#if __HAVE_FAXSOLUTION_DK__
	FAX_APP_JOB_TYPE_DK_SOLUTION = 130,
#endif

    FAX_APP_JOB_TYPE_LAST
} FAX_APP_JOB_TYPE_t;


typedef enum {
    IMAGE_BACKUP_JOB_TYPE_TX_PENDING, // 0
    IMAGE_BACKUP_JOB_TYPE_TX_PENDING_POLL, // 1
    IMAGE_BACKUP_JOB_TYPE_TX_PENDING_DELAY, // 2
    IMAGE_BACKUP_JOB_TYPE_TX_PROGRESSING, // 3
    IMAGE_BACKUP_JOB_TYPE_FORWARD, // 4
    IMAGE_BACKUP_JOB_TYPE_RCV_DONE, // 5
    IMAGE_BACKUP_JOB_TYPE_PRINT_PENDING, // 6
    IMAGE_BACKUP_JOB_TYPE_POLL_RECEIVE, // 7

    IMAGE_BACKUP_JOB_TYPE_LAST
} IMAGE_BACKUP_JOB_TYPE_t;


typedef enum {
    IMAGE_BACKUP_REASON_UPDATE_JOB_TYPE, // 0
    IMAGE_BACKUP_REASON_UPDATE_DIAL_NO, // 1
    IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT, // 2
    IMAGE_BACKUP_REASON_UPDATE_PAGE_INFO, // 3
    IMAGE_BACKUP_REASON_UPDATE_RESOLUTION, // 4
    IMAGE_BACKUP_REASON_UPDATE_PERMISSION, // 5

    IMAGE_BACKUP_REASON_LAST
} IMAGE_BACKUP_REASON_t;


typedef enum {
    FAX_REPORT_TX_RX_RESULT_REPORT, // 0
    FAX_REPORT_TCR, // 1
    FAX_REPOR_MEMORY_FILE_LIST, // 2
    FAX_ERROR_T30_LOG, // 3
    FAX_REPORT_TCR_FOR_FAX_SERVICE, // 4
    FAX_REPORT_SPAM_FAX_LIST, // 5

    FAX_REPORT_LAST
} FAX_REPORT_t;


typedef enum {
	TX_RX_RESULT_REPORT_PRINT_OPTION_ALWAYS_ON, // 0
	TX_RX_RESULT_REPORT_PRINT_OPTION_ON_ERROR, // 1
	TX_RX_RESULT_REPORT_PRINT_OPTION_TX_ONLY, // 2
	TX_RX_RESULT_REPORT_PRINT_OPTION_RX_ONLY, //3
	TX_RX_RESULT_REPORT_PRINT_OPTION_OFF, // 4
	
	TX_RX_RESULT_REPORT_PRINT_OPTION_LAST
} TX_RX_RESULT_REPORT_PRINT_OPTION_t;


typedef enum {
    FAXAPP_T30_EVENT_TX_COMPLETE, // 0
    FAXAPP_T30_EVENT_RX_COMPLETE, // 1

    FAXAPP_T30_EVENT_LAST
} FAXAPP_T30_EVENT_t;


typedef enum {
    FAX_APP_JOB_DIRECTION_SCAN, // 0
    FAX_APP_JOB_DIRECTION_TRANSMIT, // 1
    FAX_APP_JOB_DIRECTION_RECEIVE, // 2
    FAX_APP_JOB_DIRECTION_PRINT, // 3
    FAX_APP_JOB_DIRECTION_RECEIVE_AND_SAVE, // 4

    FAX_APP_JOB_DIRECTION_LAST,
} FAX_APP_JOB_DIRECTION_t;


typedef enum {
	FAX_APP_PHOTO_MODE_IMAGE_RES_FINE = 1,
	FAX_APP_PHOTO_MODE_IMAGE_RES_300DPI = 3,
	
	FAX_APP_PHOTO_MODE_IMAGE_RES_LAST,
} FAX_APP_PHOTO_MODE_IMAGE_RES_t;


typedef enum {
    JOB_QUEUE_FULL_STATUS_NOT_FULL, // 0
    JOB_QUEUE_FULL_STATUS_FULL, // 1

    JOB_QUEUE_FULL_STATUS_LAST
} JOB_QUEUE_FULL_STATUS_t;


typedef enum {
    PRINT_REASON_RECEIVE, // 0
    PRINT_REASON_RESET, // 1

    PRIT_REASON_LAST
} PRINT_REASON_t;


typedef enum {
    FAX_FILE_TYPE_JBG, // 0
    FAX_FILE_TYPE_PBM, // 1
    FAX_FILE_TYPE_RAW, // 2
    FAX_FILE_TYPE_PHOTO, // 3
    FAX_FILE_TYPE_TMP_JBG, // 4

    FAX_FILE_TYPE_LAST
} FAX_FILE_TYPE_t;


typedef enum {
    SAVE_FILE_FORMAT_PDF, // 0
    SAVE_FILE_FORMAT_TIF, // 1

    SAVE_FILE_FORMAT_LAST
} SAVE_FILE_FORMAT_t;


typedef enum {
    SAVE_FILE_PAGE_TYPE_MULTI_PAGE, // 0 -> Save Multi File(s)
    SAVE_FILE_PAGE_TYPE_SINGLE_PAGE, // 1 -> Save One File

    SAVE_FILE_TYPE_TYPE_LAST
} SAVE_FILE_PAGE_TYPE_t;


typedef enum {
    FAX_ABNORMAL_FLAG_NONE, // 0
    FAX_ABNORMAL_FLAG_SCAN_JAM, // 1
    FAX_ABNORMAL_FLAG_SCAN_CANCEL, // 2
    FAX_ABNORMAL_FLAG_TX_CANCEL, // 3
    FAX_ABNORMAL_FLAG_RX_CANCEL, // 4
    FAX_ABNORMAL_FLAG_PRINT_CANCEL, // 5

    FAX_ABNORMAL_FLAG_LAST
} FAX_ABNORMAL_FLAG_t;


typedef enum {
    FILE_SYSTEM_TYPE_NAND, // '/DocMan', 0
    FILE_SYSTEM_TYPE_RAM, // '/tmp/DocMan', 1

    FILE_SYSTEM_TYPE_LAST
} FILE_SYSTEM_TYPE_t;


typedef enum {
    FAX_CANCEL_JOB_TYPE_SCAN, // 0
    FAX_CANCEL_JOB_TYPE_TX, // 1
    FAX_CANCEL_JOB_TYPE_RX, // 2
    FAX_CANCEL_JOB_TYPE_PRINT, // 3

    FAX_CANCEL_JOB_TYPE_LAST
} FAX_CANCEL_JOB_TYPE_t;


typedef enum {
    SEND_DESTINATION_EMAIL, // 0
    SEND_DESTINATION_FTP, // 1
    SEND_DESTINATION_SMB, // 2
    SEND_DESTINATION_WEBDAV, // 3

    SEND_DESTINATION_LAST
} SEND_DESTINATION_t;


typedef enum {
    FAX_PRINT_RESULT_CANCEL, // 0
    FAX_PRINT_RESULT_COMPLETE, // 1
	FAX_PRINT_RESULT_PRINTING, // 2

    FAX_PRINT_RESULT_LAST
} FAX_PRINT_RESULT_t;


typedef enum {
    FSU_RETRANSMIT_NO_NEED_RETRANSMIT, // 0
    FSU_RETRANSMIT_NEED_RETRANSMIT, // 1

    FSU_RETRANSMIT_LAST
} FSU_RETRANSMIT_t;


typedef enum {
    FSU_JOB_TYPE_FOR_DIAL_TYPE_TRANSMIT, // 0
    FSU_JOB_TYPE_FOR_DIAL_TYPE_ON_HOOK, // 1
    FSU_JOB_TYPE_FOR_DIAL_TYPE_RECEIVE, // 2
    FSU_JOB_TYPE_FOR_DIAL_TYPE_POLL_TRANSMIT, // 2

    FSU_JOB_TYPE_FOR_DIAL_TYPE_LAST
} FSU_JOB_TYPE_FOR_DIAL_TYPE_t;


typedef enum {
	FAX_COUNTRY_DATE_TYPE_YMD,
	FAX_COUNTRY_DATE_TYPE_MDY,
	FAX_COUNTRY_DATE_TYPE_DMY,
	
	FAX_COUNTRY_DATE_TYPE_LAST
} FAX_COUNTRY_DATE_TYPE_t;


typedef enum {
    REQUEST_SOURCE_FAX, // 0
    REQUEST_SOURCE_PC, // 1

    REQUEST_SOURCE_LAST
} REQUEST_SOURCE_t;


typedef enum {
    FAXAPP_MODEM_SLEEP_MODE_NORMAL_SLEEP = 1,
    FAXAPP_MODEM_SLEEP_MODE_DEEP_SLEEP = 2,

    FAXAPP_MODEM_SLEEP_MODE_LAST
} FAXAPP_MODEM_SLEEP_MODE_t;


typedef enum {
    FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION,
    FAXAPP_MAKE_PDF_REASON_IMAGE_LOG,
    FAXAPP_MAKE_PDF_REASON_IMAGE_LOG_FOR_PCFAX,

#if __HANA_FINANCIAL_GROUP_REQ_021__
	FAXAPP_MAKE_PDF_REASON_FAX_SOLUTION_FOR_HANA,
#endif

    FAXAPP_MAKE_PDF_REASON_LAST
} FAXAPP_MAKE_PDF_REASON_t;


typedef enum {
    IMAGE_LOG_JOB_TYPE_TX = 1,
    IMAGE_LOG_JOB_TYPE_RX = 2,

    IMAGE_LOG_JOB_TYPE_LAST
} IMAGE_LOG_JOB_TYPE_t;


#if __SUPPORT_MANUAL_RECEIVE__
typedef enum {
    ON_HOOK = 0, // On-Hook Key
    ON_HOOK_CANCEL = 1, // On-Hook Key + Cancel Key
    ON_HOOK_START = 2, // On-Hook Key + Start Key

    ON_HOOK_LAST
} ON_HOOK_t;


// Temp.. Should Be Rearranged With UI..
typedef enum {
    KEY_ON_HOOK = 0,
    KEY_CANCEL = 1,
    KEY_START = 2,

    KEY_LAST
} KEY_t;
#else
#endif

typedef enum {
    ON_HOOK_STATE_DISABLE = 0,
    ON_HOOK_STATE_ENABLE = 1,
    ON_HOOK_STATE_OFFHOOK = 2,
    ON_HOOK_STATE_CANCEL_PENDING = 3,
    ON_HOOK_STATE_CANCELING = 4
} ON_HOOK_BUTTON_t;


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
typedef enum {
    SOLUTION_PERMISSION_STATE_DISABLE = 0,
    SOLUTION_PERMISSION_STATE_WAIT = 1,
    SOLUTION_PERMISSION_STATE_ENABLE = 2,
    
    SOLUTION_PERMISSION_STATE_LAST
} SOLUTION_PERMISSION_STATE_t;
#endif

/******************************************************************************
*  2. DEFINES
******************************************************************************/
#define PATH_SEPARATOR     '/'

#define DISABLE     0
#define ENABLE     1

#define FAX_PRINT_ISDUPLEX_INITIAL 2 //fax print job -> isduplex: initial value

#define TEMP_RX_PAGE_MEM     (20*1*1024*1024)     // 20MB
#define TEMP_TX_DATA_BUFFER     (5*1*1024*1024)     // 5MB
#define TEMP_BUFFER_FRONT_MEM     (5*1*1024*1024)     // 5MB

#define MAX_NO_IMAGE_FILES     256 // Maximum number of image files that fax can handle (fax01.jbg, fax02, ..., faxFE.jbg, faxFF.jbg)
#define MAX_LEN_FILE_PATH_NAME     255
#define MAX_NUM_DOC_ID     512     // Maximum number of document id that PINETREE can have (0 ~ 511)

#define MIN_MEMORY_RAM     (30*1024)     // 30MB

#define TRANSMIT_TIME_RIGHT_NOW     0xFFFFFFFF

#define TX_JOBS     600     // Transmit job queue size (0 ~ 599)
#define MAX_NUM_BC     200     // Maximum number of jobs that one broadcast job can have
#define LEN_PHONE_NO     40     // Maximum number of dial

/* The following tray information should be matched with that of print core (./common/print/core/include/agprint.h) */
#define FAX_INPUT_TRAY_MANUAL     0 // TRAY_1
#define FAX_INPUT_TRAY_1     1 // TRAY_2
#define FAX_INPUT_TRAY_2     2 // TRAY_3
#define FAX_INPUT_TRAY_3     3 // TRAY_4
#define FAX_INPUT_TRAY_AUTO     8 // TRAY_ANY

#define MAX_DURATION_IN_SECOND     (100*60) // Maximum duration which can be printed on report is 99:59.

#define MAX_STRING_LEN     200

#define MAX_SPAM_FAX_LIST     50

#define DEFAULT_ORIGINAL_DOC_ID    0xFFFF

#define MAX_JOB_LOG_LIST     300

#define MAX_LEN_USER_ID     64

#define MAX_LEN_DATE_TIME     15 // e.g.) 08-11 PM 02:25

#define MAX_LEN_DATE     6 // e.g.) 08-11

#define MAX_LEN_RESULT     20

#define MAX_LEN_DATE_TIME_YEAR     16 // e.g.) 2015-05-20 23:45

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
#define MAX_LAN_FILE_NAME     21 // e.g.) F20150515112045330333 (F + YEAR + MONTH + DAY + HOUR + MINUTE + SECOND + MILLI SECOND + MICRO SECOND)

#define MAX_LEN_IP_ADDRESS     15 // e.g.) 123.456.789.000
#endif

#ifdef HAVE_IMAGELOG
#define IMAGE_LOG_PATH "/mnt/sd/upload"
#endif

#define CONVERT_400DPI (HPDF_REAL)((HPDF_REAL)400.0/(HPDF_REAL)72.0)

#if __SUPPORT_MANUAL_RECEIVE__
#define MAX_LEN_REMOTE_ACCESS_CODE     3 // e.g.) *9*

#define MAX_LEN_MACHINE_ACCESS_CODE     2 // e.g.) TS
#else
#endif

#if __MEASURE_PERFORMANCE__ || __HANA_FINANCIAL_GROUP_REQ_014_5__
/* T30 Direction */
#define T30_RECEIVE  0
#define T30_TRANSMIT 1
#define MEM_TO_PRINT 2
#define SCAN_TO_MEM  3
#define T30_TRANSMIT_BOTTLE_NECK     4
#define ON_HOOK     5
#define RCV_TO_MEM 6
#endif // #if __MEASURE_PERFORMANCE__


/******************************************************************************
*  3. STRUCTURES
******************************************************************************/
typedef struct {
    int ID;
    int JobType;
    char DialNo[LEN_PHONE_NO + 1];
    int TimeToTx;
} JOB_QUEUE_INFO_UI_t;


typedef struct {
    int JobType;
    char UserId[MAX_LEN_USER_ID + 1];
    char DateTime[MAX_LEN_DATE_TIME];
    char RemoteNo[LEN_PHONE_NO + 1];
    char Result[MAX_LEN_RESULT];
    int TotalPageNo;
    char DelayTime[MAX_LEN_DATE_TIME];
} INFO_FOR_FAX_JOB_LOG_t;


typedef struct {
    int CountryCode;
    int DialToneDetection;
} REBOOT_NEED_PARAM_t;


typedef struct {
    int JobId;
    char RemoteNo[LEN_PHONE_NO + 1];
    int StartTime;
    int Duration;
    int TotalPageNo;
    int SentPageNo;
    int Result;
} FSU_INFO_BROADCAST_RESULT_REPORT_t;


typedef struct {
    int Count;
    char RemoteNo[MAX_NUM_BC][LEN_PHONE_NO + 1];
    int StartTime[MAX_NUM_BC];
    int Duration[MAX_NUM_BC];
    int TotalPageNo[MAX_NUM_BC];
    int SentPageNo[MAX_NUM_BC];
    int Result[MAX_NUM_BC];
} BROADCAST_RESULT_REPORT_t;


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
#define SDH_AUTH_CONF_FTP_ADDR	193
#define SDH_AUTH_CONF_BASIC_MAX	33

typedef struct {
    char ftpAddress[SDH_AUTH_CONF_FTP_ADDR + 1];
    int ftpPort;
    char ftpID[SDH_AUTH_CONF_BASIC_MAX + 1];
    char ftpPassword[SDH_AUTH_CONF_BASIC_MAX + 1];
} FTP_FAX_SOLUTION_t;
#endif


/******************************************************************************
*  4. ALLOCS
******************************************************************************/



/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/

#endif // #ifndef __SDH_FAX_COMMON_HEADER__
