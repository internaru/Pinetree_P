#ifndef __FSU_IMAGE_BACKUP__
#define __FSU_IMAGE_BACKUP__

#include "SysUI_Interface.h"

typedef unsigned char UINT8;
typedef unsigned short UINT16;
//typedef unsigned int UINT32;
typedef unsigned char BOOLEAN;


# ifndef FALSE
    #define FALSE 0
# endif // # ifndef FALSE


# ifndef TRUE
    #define TRUE 1
# endif // # ifndef TRUE


typedef struct {
    UINT16 DocId[TX_JOBS];
    UINT8 JobType[TX_JOBS];
    char DialNo[TX_JOBS][LEN_PHONE_NO + 1];
    long int TimeDiff[TX_JOBS];
    UINT8 Resolution[TX_JOBS];
    UINT16 ImageWidth[TX_JOBS];
    UINT16 ImageHeight[TX_JOBS];
    UINT16 PhotoMode[TX_JOBS];
    UINT16 OriginalDocId[TX_JOBS];
    UINT8 Retransmit[TX_JOBS];
    UINT16 CurrentPageNo[TX_JOBS];
    UINT16 TotalPageNo[TX_JOBS];
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    char FileName[TX_JOBS][MAX_LEN_FILE_PATH_NAME];
    UINT8 Permission[TX_JOBS];
    UINT32 UniqueJobId[TX_JOBS];
    stIC_JobDoneInfo ScanInfo[TX_JOBS];
#endif
} JOB_QUEUE_INFO_t;


void get_job_queue_from_Job_Queue_File (UINT16 jnum);
void Update_Job_Type(UINT16 jnum, UINT8 JobType);
void Update_Dial_No(UINT16 jnum);
void Update_Retransmit(UINT16 jnum, UINT8 Retransmit);
void Update_Page_Info(UINT16 jnum, UINT16 CurrentPageNo);
void Update_Resolution(UINT16 jnum, UINT8 Resolution);
int get_job_type_from_Job_Queue_File(char *JobQueue);
int get_no_of_recipient_from_Job_Queue_File(char *JobQueue);
void get_dial_no_from_Job_Queue_File(UINT16 jnum, char *JobQueue);
long int get_time_diff_from_Job_Queue_File(char *JobQueue);
UINT8 get_resolution_from_Job_Queue_File(char *JobQueue);
UINT8 get_photo_mode_from_Job_Queue_File(char *JobQueue);
UINT8 get_retransmit_from_Job_Queue_File(char *JobQueue);
UINT16 get_image_width_from_Job_Queue_File(char *JobQueue);
UINT16 get_image_height_from_Job_Queue_File(char *JobQueue);
UINT16 get_original_id_from_Job_Queue_File(char *JobQueue);
UINT16 get_current_page_no_from_Job_Queue_File(char *JobQueue);
UINT16 get_total_page_no_from_Job_Queue_File(char *JobQueue);
void Create_Job_Queue_File(UINT16 jnum, FAX_APP_JOB_TYPE_t FaxAppJobType, UINT16 DocId);
void Update_Job_Queue_File(IMAGE_BACKUP_REASON_t Reason, UINT16 jnum, UINT16 Param);
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
void get_file_name_from_Job_Queue_File(UINT16 jnum, char *JobQueue);
UINT8 get_permission_from_Job_Queue_File(char *JobQueue);
void Update_Permission(UINT16 jnum, UINT8 Permission);
UINT16 get_unique_job_id_from_Job_Queue_File(char *JobQueue);
void get_scan_info_from_Job_Queue_File(stIC_JobDoneInfo *scanInfo, char *JobQueue);
#endif


IMAGE_BACKUP_JOB_TYPE_t Convert_Faxapp_Job_Type_To_Image_Backup_Job_Type(FAX_APP_JOB_TYPE_t JobType);

extern void fsu_delete_file_on_file_system (const char *Path);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern void fsu_make_dir_on_file_system (const char *Path);
extern inline long int fsu_calculate_time_diff_in_sec (int Year, int Month, int Day, int Hour, int Min);
extern BOOLEAN fsu_check_job_queue_file (const char *Path);
extern UINT8 fsu_get_start_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 faxapp_get_no_of_recipient(void);
extern UINT8 faxapp_get_job_type(void);
extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT8 fsu_get_total_pages (UINT16 jnum);
extern UINT8 *fsu_get_dial_no (UINT16 jnum);
extern UINT8 fsu_get_photo_mode(UINT16 jnum);
extern UINT8 fsu_get_total_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_check_image_file_validation_receive (UINT16 DocId, UINT8 Resolution, UINT16 ImageWidth);
extern UINT8 fsu_check_image_file_validation_transmit (UINT16 DocId);
extern UINT8 fsu_convert_char_to_int (char No);
extern UINT8 getCountryDateType(void);
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern UINT16 fsu_get_original_doc_id(UINT16 jnum);
extern UINT32 fsu_get_image_width_pixel (UINT16 jnum);
extern UINT32 fsu_get_image_height_pixel (UINT16 jnum);
extern int T4_encoded_image_validation_check(UINT16 docID, char *FileName, int image_width, int rx_resolution, int coding_type);
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern UINT8 fsu_get_permission (UINT16 jnum);
extern char *fsu_get_file_name_tx (UINT16 jnum);
extern UINT32 fsu_get_unique_job_id(UINT16 jnum);
extern UINT8 fsu_get_ScanInfo_paperSize (UINT16 jnum);
extern UINT8 fsu_get_ScanInfo_duplex (UINT16 jnum);
#endif

#endif // #ifndef __FSU_IMAGE_BACKUP

