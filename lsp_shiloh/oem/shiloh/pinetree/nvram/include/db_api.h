/**
 * \file db_api.h
 **/

#ifndef INC_DBAPI_H
#define INC_DBAPI_H

#include <stdint.h>
#include "error_types.h"
#include "printvars_api.h"
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

enum LOG_PRNT_COL
{
    LOG_PRNT_PAGE_COUNT,
    LOG_PRNT_CODE,
    LOG_PRNT_TIME,
    LOG_PRNT_MAX
};

#define LOG_PRNT_ROW_SIZE 300
#define LOG_FAXLOG_ROW_SIZE 300
#define LOG_FAXJOBLOG_ROW_SIZE 300

#define QUERY_LENGTH 1024

typedef struct error_log_struct{
	int err_no;
	char* errtext;
	int err_type;
	int err_date;
}ERRLOG_STRUCT;
#define MAX_ERRLOG_ROW 30

#define ADDR_MAX_NAME_LENGTH 51
#define ADDR_MAX_SPEEDDIAL_LENGTH 6
#define ADDR_MAX_FAXNUMBER_LENGTH 41
#define ADDR_MAX_EMAIL_LENGTH 80

typedef struct addressbook_struct{
	int line_count;
	char name[ADDR_MAX_NAME_LENGTH];
	char speeddial[ADDR_MAX_SPEEDDIAL_LENGTH];
	char faxnumber[ADDR_MAX_FAXNUMBER_LENGTH];
	char email[ADDR_MAX_EMAIL_LENGTH];
}ADDRESSBOOK_STRUCT;
#define MAX_ADDRESSBOOK_ENTRY 1000

#define MAX_FAX_NUMBER_LENGTH 100
typedef struct fax_log_struct{
	int datetime;
	int faxmode;
	int ecm;
	int resolution;
	char remotenum[MAX_FAX_NUMBER_LENGTH];
	int duration;
	int pagenum;
	int connectspeed;
	int result;
	int recname;
}FAXLOG_STRUCT;

typedef struct fax_log_struct_jobid{
	int datetime;
	int faxmode;
	int ecm;
	int resolution;
	char remotenum[MAX_FAX_NUMBER_LENGTH];
	int duration;
	int pagenum;
	int connectspeed;
	int result;
	int recname;
	int jobid;
}FAXLOG_STRUCT_JOBID;
#define MAX_FAXLOG_ROW 35

enum FAX_TYPE
{
    FAX_TYPE_RX = 0,
    FAX_TYPE_TX,
};

#define MAX_FAX_NUMBER_LENGTH_RELAY_FCODE 41 * 50
#define MAX_FAX_LENGTH_FCODE 21
typedef struct fax_fcode_struct{
	int boxtype;
	char subaddress[MAX_FAX_LENGTH_FCODE];
	char password[MAX_FAX_LENGTH_FCODE];
	int isimagesaved;
	char boxpwd[MAX_FAX_LENGTH_FCODE];
	char dialnumlist[MAX_FAX_NUMBER_LENGTH_RELAY_FCODE];
	char boxname[MAX_FAX_LENGTH_FCODE];
}FAXFCODE_STRUCT;
#define MAX_FAXFCODE_ROW 100

typedef struct fax_splog_struct{
	int datetime;
	int faxmode;
	int ecm;
	int resolution;
	char remotenum[MAX_FAX_NUMBER_LENGTH];
	int duration;
	int pagenum;
	int connectspeed;
	int result;
	int recname;
}FAXSPLOG_STRUCT;
#define MAX_FAXSPLOG_ROW 700

typedef struct fax_spam_strucn{
	char faxspamnumber[MAX_FAX_NUMBER_LENGTH];
}FAXSPAM_STRUCT;
#define MAX_FAXSPAM_ROW 50

typedef struct fax_broadcast_log_struct{
	int jobid;
	char remotenum[MAX_FAX_NUMBER_LENGTH];
	int datetime;
	int duration;
	int totalpageno;
	int sentpageno;
	int result;
}FAXBROADCASTLOG_STRUCT;
#define LOG_FAXBROADCASTLOG_ROW_SIZE 520

#if 1 //JUNIPER
typedef struct counter_struct{
	int total;
	int host;
	int report;
	int usbdrive;
	int duplex;
	int copyprint;
	int faxprint;
	int a3print;
	int b4print;
	int a4print;
	int b5print;
	int a5print;
	int p8x13print;
	int p8kprint;
	int p16kprint;
	int b6print;
	int etcprint;
//	int total_largesize;
//	int host_largesize;
//	int report_largesize;
//	int usbdrive_largesize;
//	int copyprint_largesize;
//	int faxprint_largesize;
}COUNTER_STRUCT;
#else
typedef struct counter_struct{
	int total;
	int host;
	int report;
	int usbdrive;
	int duplex;
	int copyprint;
	int faxprint;
}COUNTER_STRUCT;
#endif

typedef struct scan_counter_struct{
	int scan;
	int fbscan;
	int a3scan;
	int adfscan;
	int adfduplex;
}SCAN_COUNTER_STRUCT;

typedef struct faxsend_counter_struct{
	int faxsend;
}FAXSEND_COUNTER_STRUCT;

#define MAX_TONERHISTORY_ROW 10
#define MAX_FUSERHISTORY_ROW 8

error_type_t db_setup(char* dbpath);
error_type_t db_initialize(void);
error_type_t db_trySQLiteExec(char *query);
error_type_t db_set_var(char* tblName, char* attrName, char* value);
error_type_t db_set_var_int(char* tblName, char* attrName, uint32_t value);

uint32_t db_get_data_count(char* table, char* condition, error_type_t* db_retval);
uint32_t db_get_last_recd_no(char* table, error_type_t* db_retval);

error_type_t db_set_printlog(print_log_entry_t* log_entry);
error_type_t db_set_recd_memfile(uint32_t _dateTime, uint32_t _resolution, char* _remoteNum, uint32_t _pageNum, uint32_t _jobType);
error_type_t db_set_recd_fax_log(uint32_t _dateTime, uint32_t _faxMode, uint32_t _ecm, uint32_t _resolution, char* _remoteNum, uint32_t _duration, uint32_t _pageNum, uint32_t _connectSpeed, uint32_t _result, uint32_t _recName);
error_type_t db_set_recd_fax_log_jobid(uint32_t _dateTime, uint32_t _faxMode, uint32_t _ecm, uint32_t _resolution, char* _remoteNum, uint32_t _duration, uint32_t _pageNum, uint32_t _connectSpeed, uint32_t _result, uint32_t _recName, uint32_t _jobid);
error_type_t db_set_recd_fax_sp_log(uint32_t _dateTime, uint32_t _faxMode, uint32_t _ecm, uint32_t _resolution, char* _remoteNum, uint32_t _duration, uint32_t _pageNum, uint32_t _connectSpeed, uint32_t _result, uint32_t _recName);
error_type_t db_set_recd_fax_broadcast_log(uint32_t _jobid, char* _remoteNum, uint32_t _dateTime, uint32_t _duration, uint32_t _totalPageNo, uint32_t _sendPageNo, uint32_t _result);
error_type_t db_set_recd_fax_job_log(uint32_t _jobType, char* _userID, char* _dateTime, char* _remoteNo, char* _result, uint32_t _page, char* _delayTime);
error_type_t db_set_recd_fax_fcode_box(uint32_t _boxType, char* _subaddress, char* _password, uint32_t _isImageSaved, char* _boxPWD, char* _dialNumList, char* _boxName);
error_type_t db_set_recd_fax_image_backup(uint32_t _jobID, uint32_t _jobType, char* _dialNo, uint32_t _timeToTx);
error_type_t db_set_recd_err_log(int _errorType, int _errorDesc, int _subJobType, int _jobID, int _date, int _uptimeSec, int _pageCount, int _porCount, char* _ip, char* _desc);
error_type_t db_set_recd_ssidbook(int _ifSelected, char* _ssid, char* _apMacAddr, int _ifHidden, int _authType, int _groupCipher, int _pairCipher, int _ifSupportPreAuth, int _authSuite, char* _passkey1, char* _passkey2, char* _passkey3, char* _passkey4);
error_type_t db_set_recd_uap_maclist(char* _macAddress);

error_type_t db_del_recd_fax_broadcast_log_job_var(uint32_t _jobID);
error_type_t db_del_recd_var(char* tblName);
error_type_t db_del_recd_all(char* tblName);

error_type_t db_trySQLiteGetTable(sqlite3* pDB, char* query, char*** queryResult, int* resultRow, int* resultCol);
error_type_t db_integrityCheck(char * DBName);
char* db_get_var_query(char* query, error_type_t* db_retVal);
char* db_get_var(char* tblName, char* attrName, error_type_t* db_retVal);
int db_get_var_int(char* tblName, char* attrName, error_type_t* db_retVal);

error_type_t db_get_printlog(print_log_entry_t* log_entry);
error_type_t db_get_table(char* query, char*** result, int* resultRow, int* resultCol);
error_type_t db_get_faxlog(FAXLOG_STRUCT* fax_result, int* row_count);
error_type_t db_get_faxlog_jobid(FAXLOG_STRUCT_JOBID* fax_result, int* row_count);
error_type_t db_get_faxlog_last_job_from_fax_mode(FAXLOG_STRUCT* fax_result, int* row_count, int fax_type);
error_type_t db_get_faxlog_last_job_from_fax_mode_jobid(FAXLOG_STRUCT_JOBID* fax_result, int* row_count, int fax_type, int jobID);
error_type_t db_get_faxfcode(FAXFCODE_STRUCT* fax_result, int* row_count);
error_type_t db_get_faxfcode_from_subaddress(FAXFCODE_STRUCT* fax_result, int* row_count, unsigned char* sub_address);
error_type_t db_get_faxfcode_from_type(FAXFCODE_STRUCT* fax_result, int* row_count, int box_type);
error_type_t db_get_faxsplog(FAXSPLOG_STRUCT* fax_result, int* row_count);
error_type_t db_get_errlog(ERRLOG_STRUCT* err_result, int* row_count);
error_type_t db_get_faxspamnumber(FAXSPAM_STRUCT* faxspam_result, int* row_count);
error_type_t db_get_addressbook(ADDRESSBOOK_STRUCT* err_result, int* row_count, int* total_line_count);
error_type_t db_get_fax_broadcast_log(FAXBROADCASTLOG_STRUCT* fax_result, int* row_count);

#define INSTANCE_MACHINE "INST_MACHINE"
#define CONF_PRINT "CONF_PRNT"

// For count
#define PGINTEGRITY_LOGFILENAME		"/backup/PageIntegrityFCnt.log"
#define BOTHINTEGRITY_LOGFILENAME 		"/backup/BothIntegrityFCnt.log"
#define PGINTEGRITY_SCAN_LOGFILENAME		"/backup/PageIntegrityFCntScan.log"
#define BOTHINTEGRITY_SCAN_LOGFILENAME 		"/backup/BothIntegrityFCntScan.log"
#define PGINTEGRITY_FAXSEND_LOGFILENAME		"/backup/PageIntegrityFCntFaxSend.log"
#define BOTHINTEGRITY_FAXSEND_LOGFILENAME 		"/backup/BothIntegrityFCntFaxSend.log"
error_type_t db_get_totalcount(COUNTER_STRUCT* perma_count);
error_type_t db_set_drumcount(COUNTER_STRUCT perma_count, int drum, int motor, int countinpercent, int drumlife);
error_type_t db_get_drumcount(COUNTER_STRUCT *perma_count, int* drum, int* countinpercent, int* motor, int* drumlife);
void db_logger_drumcount(char *CounterProperty, int stPermaCount, int perma_count);
error_type_t db_set_fusercount(int Count, int ResetAction, int MotorNormal, int MotorLow, int FuserLifePageCount);
error_type_t db_get_fusercount(int* Count, int* ResetAction, int* MotorNormal, int* MotorLow, int* FuserLifePageCount);
error_type_t db_set_fuserhistory(int UsedCount);
error_type_t db_get_fuserhistory(char (*UsedCount)[8], int* row_count);
error_type_t db_get_scan_totalcount(SCAN_COUNTER_STRUCT* perma_count);
error_type_t db_set_scancount(SCAN_COUNTER_STRUCT perma_count);
error_type_t db_get_scancount(SCAN_COUNTER_STRUCT *perma_count);
error_type_t db_get_faxsend_totalcount(FAXSEND_COUNTER_STRUCT* perma_count);
error_type_t db_set_faxsendcount(FAXSEND_COUNTER_STRUCT perma_count);
error_type_t db_get_faxsendcount(FAXSEND_COUNTER_STRUCT *perma_count);

error_type_t db_copy(char* inputFile, char* outputFile);
error_type_t db_file_integrityCheck(char * DBName);

error_type_t db_set_recd_toner_history(char* _tonerTLI, char* _tonerSerialNo, int _capacity);
error_type_t db_get_tonerhistory(char (*tonertli)[20], char (*tonerserial)[20], int* row_count);

#define UNKNOWN_DB_VALUE -999
#define LANGUAGE_ENGLISH 0
#define LANGUAGE_KOREAN 1
#define LANGUAGE_CHINESE 2
#define LANGUAGE_SPANISH 3
#define LANGUAGE_TURKISH 4
#define LANGUAGE_RUSSIAN 5
#define LANGUAGE_FRANCH 6
#define LANGUAGE_PORTUGUESE 7
#define LANGUAGE_ARABIC 8

#define PAPERSIZE_DB_VALUE_AUTO 554

void CheckDBReturnVal(error_type_t db_retval, int line);
error_type_t db_set_var_for_solution();
error_type_t db_set_var_for_TLI();
int TLI_getCountry(char * machineTLI);
int TLI_getDefaultLanguage();
void TLI_getDefaultPaperType();
int TLI_getDefaultFaxCountryCode();

error_type_t db_critical_bug_check(int db_version);

#ifdef __cplusplus
}
#endif

#endif /* INC_DBAPI_H */
