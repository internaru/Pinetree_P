/*
 ***********************************************************
 *(c) Copyright 2013 - Sindoh Co. Ltd. 
 *
 *               SINDOH Confidential
 *==========================================================
 **/
/**
 *\file SysUI_Interface.h
 *
 *\brief Common structure for interface between UI manager and system service.
 *
 **/


#ifndef SYS_UI_INTERFACE_H_
#define SYS_UI_INTERFACE_H_


#include "ATypes.h"
#include "ResourceMap.h"
#include "SYS_UI_msg_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

#define	bit(X)	(1<<(X))
#define	bitl(X)	(1L<<(X))

//Internal command 
#define CMD_INT_PRINT_SUB_SYSTEM 0xEF01	///<For Print Sub system to SSM
#define CMD_INT_COPY_APP_EVENT 0xEF02	///<For Copy App Event CB to SSM
#define CMD_INT_PRINT_PAPER_MISMATCH 0xEF03	///< Paper mismatch status to SSM
#define CMD_INT_USB_THUMB_DRIVE	0xEF04	///< USB Thumb Drive connected/disconnected 
#define CMD_INT_PCU_STATUS 0xEF05	///< PCU status to SSM
#define CMD_INT_TD_SYS_JOB_ACTION	0xEF06	///< System job action like Fake Copy 
#define CMD_INT_FAX_SJM_RES	0xEF07	///< Fax job result from SJM
#define CMD_INT_HOUR_ALARM	0xEF08	///< Hourly Alarm
#define CMD_INT_TIMELY_ALARM	0xEF09	///< Hourly Alarm
#define CMD_INT_WRONG_IMAGE_SIZE	0xEF0A	///< Wrong Image Size for Print job
#define CMD_INT_PAGE_DONE_INFO	0xEF0B	///< Send job done from Print JobMgr and Scan to SSM
#define CMD_INT_JOB_DONE_INFO	0xEF0C	///< Send job done from Print JobMgr and Scan to SSM
#define CMD_INT_PARSER_AUTH_RESULT	0xEF0D	///< Parser send Auth result to SSM
#define CMD_INT_PARSER_AUTH_REQ	0xEF0E	///< Parser send Auth request to SSM

typedef struct stIC_PrintSubSystem_ {
	UINT32			printStatus;
	UINT32			pageStatus;
	UINT32			jobStatus;
	JOB_TYPES		resourceJobType;
	UINT32			resourceJobID;
}stIC_PrintSubSystem;

typedef struct stIC_USBThumbDrive_ {
	UINT32			status;	///< 0:Remove 1:Add 2:Mount 
}stIC_USBThumbDrive;

typedef struct stIC_SysJobFakeAction_ {
	UINT32			jobType;	///< 1: Fake Copy
	UINT32			action;	///< 0 : Start 1 : Stop 2 : Cancel 3 : Resume
}stIC_SysJobFakeAction;

///< FAX job result From SJM to SSM
typedef struct stIC_FaxJobResult_ {
	UINT32			jobType;
	UINT32			jobID;
	UINT32			result;
}stIC_FaxJobResult;

///< Scan JAM reason From CB to SSM
typedef struct stIC_ScanmanEventStatus_ {
	UINT32			eventReason;
	UINT32			eventStatus;
}stIC_ScanmanEventStatus;

///< Hourly Alarm
typedef struct stIC_HourlyAlarm_ {
	UINT32			totalSysUpTime;	///<System UP time in hour
}stIC_HourlyAlarm;

///< Timely Alarm
typedef struct stIC_TimelyAlarm_ {
	UINT32			totalTimeInSec;	///<Total time in second
}stIC_TimelyAlarm;

///< Print Page Done info
typedef struct stIC_PrintPageDoneInfo_ {
	UINT32			jobNum;	///<Job number from Print JM
	//UINT32			jobStatus;	///<1: Job Running, 2: Job Canceling, 3: Job Cancelled(Job done for canceling), 4: Job Done normally
	UINT32			jobDocType;	///<Job's Document type
	UINT32			pagesPrinted;	///<pages printed
	UINT32			sheetPageCount;	///<Sheet page count 1:For front page only 2: Front and Back both
	UINT32			pageCount;	///<Total page count for current job = ReceivedPages*No_of_Copies
}stIC_PrintPageDoneInfo;

#define JOB_INFO_USER_ID_SIZE 33
///< Total Pages Printed
typedef struct stIC_JobDoneInfo_ {
	UINT32			jobNum;	///<SJM's job ID
	char			userID[JOB_INFO_USER_ID_SIZE];	///<User ID of this job
	UINT8			jobDocType;	///<Job's Document type
	UINT8			copies;	///<No of Copies
	UINT16			totalPages;	///<Total pages Printed or Scanned
	UINT16			paperSize;	///<Paper size code
	UINT8			duplex;	///<0: Simplex 1: Duplex
	//Total 44 bytes used
}stIC_JobDoneInfo;

///< Parser print auth result
typedef struct stIC_ParserAuthResult_ {
	UINT32			result;	///<Parser Auth Result	0: Auth Fail, 1: Auth Success
	char			authID[JOB_INFO_USER_ID_SIZE];	///<Auth ID of print job
}stIC_ParserAuthResult;

typedef struct stIC_ScanSendResult_ {
	UINT32			jobID;	///<unique job id
	UINT32			scanJobID;	///<Job id of Scan job
    UINT8			result;
    UINT8			jobType;
    UINT8			destCount;
    UINT32			errorCode;
    UINT32			addInfo; //Shared Memory
}stIC_ScanSendResult;

//Initialization Completion Version
typedef enum
{
	ICT_UI_MANAGER = 0,
	ICT_UICC = 1,
	ICT_SDC = 2,
	ICT_EGB = 3,
	ICT_FINISHER = 4,
	ICT_GUI_PS = 8,
	ICT_BROWSER_PS = 9,
}INIT_COMPLETE_TYPE;

//Energy Save Mode
typedef enum
{
	ESM_WAKE_UP = 0,
	ESM_ENERGY_SAVE = 1,
	ESM_ADAPTIVE_TIMEOUT = 2,
}ENERGY_SAVE_MODE;

//Energy Save Mode
typedef enum
{
	EST_NORMAL = 0,
	EST_ADAPTIVE = 1,
}ENERGY_SAVE_TYPE;

//Energy Save State
typedef enum
{
	ESS_ENERGY_SAVE = 1,
	ESS_COLD_WAKE_UP = 2,
	ESS_FULL_WAKE_UP = 3,
}ENERGY_SAVE_STATE;

//TFT LCD mode
typedef enum
{
	TFT_FB_SLEEP = 0,
	TFT_FB_WAKEUP = 1,
	TFT_TOUCH_SLEEP = 2,
	TFT_TOUCH_WAKEUP = 3,
	TFT_DEEP_SLEEP = 4,
	TFT_DEEP_WAKEUP = 5
}TFT_LCD_MODE;

//Sys Wake-Up Reason for User Attention
typedef enum
{
	SW_ADF_PAPER_STATUS_CHANGE = 501,
	SW_ADF_TOP_COVER_STATUS_CHANGE,
	SW_SCANNER_JAM_OCCUR,
	SW_SC_OCCUR,
	SW_PLOTTER_DOOR_STATUS_CHANGE,	//505
	SW_PLOTTER_TRAY_STATUS_CHANGE,
	SW_PLOTTER_JAM_OCCUR,
	SW_USB_STATUS_ADD,
	SW_AUTH_CARD_ACTION,
	SW_COPY_SCREEN_REQUESTED,	//510
	SW_DURING_PRINT_POWER_KEY_PRESSED,
	SW_LAMINATION_JOB_REQUESTED
}SYS_WAKE_UP_REASON;

//Key Pressed Type
typedef enum
{
	KEY_OR_TOUCH_PRESSED = 0,
	POWER_KEY_PRESSED = 1,
}KEY_PRESSED_TYPE;

typedef enum 
{
	EMAIL_ALERT_PAPEREMPTY, 
	EMAIL_ALERT_TONEREND, 
	EMAIL_ALERT_OUTPUTFULL, 
	EMAIL_ALERT_PAPERJAM,
	EMAIL_ALERT_INTERVENTION, 
	EMAIL_ALERT_DRUMCHANGE,
	EMAIL_ALERT_TONERLOW,
	EMAIL_ALERT_TEST_SEND,
	EMAIL_ALERT_ALIVE_SEND,
}EMAIL_ALERT_TYPE;

typedef enum 
{
	FB_SCAN_COUNT,
	FB_A3_SCAN_COUNT,
	FB_LEGAL_SCAN_COUNT,
	ADF_SIMPLEX_SCAN_COUNT,
	ADF_DUPLEX_SCAN_COUNT
}SCAN_COUNTER_TYPE;

typedef enum
{
	AUTH_JOB_PRINT = 1,
	AUTH_JOB_SCAN = 2,
	AUTH_JOB_FAX_SEND = 3,
	AUTH_JOB_COPY = 4,
	AUTH_JOB_SCAN_TO_X = 5, //Internal use
}AUTH_COUNTUP_JOB_TYPE;

//To manage Scan info in SSM
typedef enum
{
	SCAN_INFO_ADD = 1,
	SCAN_INFO_DELETE = 2,
	SCAN_INFO_COUNTUP = 3,
}SCAN_INFO_REQ;

typedef enum
{
	AUTH_SOL_JOB_OK = 0,
	AUTH_SOL_JOB_PENDING = 1,
	AUTH_SOL_JOB_NG = 2,
	AUTH_SOL_JOB_DEL_OK = 3,
	AUTH_SOL_JOB_DEL_NG = 4,
	AUTH_SOL_JOB_FILENAME_MISMATCH = 5,
	AUTH_SOL_JOB_CANCELLED_BY_USER = 6,
}AUTH_SOL_JOB_RESULT;

typedef enum
{
	ILS_SDCARD_NOT_DETECTED = 800,
	ILS_SDCARD_FULL = 801,
	ILS_SDCARD_WRITE_ERR = 802,
	ILS_SDCARD_INVALID_FILE_SYSTEM = 803,
	ILS_TRANSFER_ERR = 810,
	ILS_SDCARD_FULL_TRANSFER_ERR = 811,
	ILS_NO_PROBLEM = 820,
}IMAGE_LOG_STATUS;

typedef enum{
	SYS_REQ_VERSION = 0,
	SYS_SET_COUNTRY_CODE = 1,
	SYS_REQ_TEST_SEND = 2,
}SYS_INFO_TYPE;

typedef enum{
	DRUM_CARTRIDGE_RESET = 1, ///< RESET Drum Cartridge
	FUSER_UNIT_RESET = 2  ///<Reset Fuser unit
}PLOTTER_RESET_ACTION;

#define CMD_INT_MACHINE_CONF	0x0181	/// [ Internal use ] Machine configuration
#define CMD_INT_KEY_PRESSED		0x0202	///< [ Internal use ] Key pressed
#define CMD_INT_ENERGY_MODE		0x0282	///< [ Internal use ] Energy mode
#define CMD_INT_SCANNER_JAM_STATUS	0x0483	///< [ Internal use ] Scanner JAM status
#define CMD_INT_PLOTTER_MODE_STATUS	0x0484	///< [ Internal use ] Plotter Mode status
#define CMD_INT_PLOTTER_DOOR_STATUS	0x0485 ///< [ Internal use ] Plotter door open status
#define CMD_INT_PLOTTER_TRAY_STATUS	0x0486	///< [ Internal use ] Plotter tray status
#define CMD_INT_PLOTTER_JAM_STATUS		0x0487 ///< [ Internal use ] Plotter jam status
#define CMD_INT_PLOTTER_TONER_STATUS	0x0488	///< [ Internal use ] Toner status
#define CMD_INT_PLOTTER_SC_STATUS		0x0489 ///< [ Internal use ] SC status
#define CMD_INT_PLOTTER_PCU_STATUS		0x048A ///< [ Internal use ] PCU status
#define CMD_INT_PRINT_GOPRINT_RES	0x0581	///<[ Internal use ] Go Print Result
#define CMD_INT_PRINT_GOLAMINATION_RES	0x0583	///<[ Internal use ] Go Lamination Result
#define CMD_INT_SCAN_GOSCAN_RES 0x0681	///< [ Internal use ] Go Scan Result
#define CMD_INT_SCAN_GOSCANSEND_RES 0x0683	///< [ Internal use ] Go ScanSend Result
#define CMD_INT_COPY_GOCOPY_RES 0x0781	///< [ Internal use ] Go Copy Result
#define CMD_INT_FAX_GO_FAX_SCAN 0x0801	///< [ Internal use ]
#define CMD_INT_FAX_POWER_SAVE_MODE 0x0802	///< [ Internal use ]
//#define CMD_INT_FAX_GOFAX_RES 0x0882	///< [ Internal use ] Go Fax JOB result
#define CMD_INT_FAX_GO_ON_HOOK 0x0803			///< [ Internal use ]
#define CMD_INT_FAX_GO_UPDATE_PARAM 0x0804		///< [ Internal use ]
#define CMD_INT_FAX_GO_FAXSCAN_MODE 0x0805 		///< [ Internal use ]
#define	CMD_INT_FAX_GO_FAX_CANCEL 0x0806		///< [ Internal use ]
#define	CMD_INT_FAX_GO_FAX_DELETE_PENDING_JOB 0x0807		///< [ Internal use ]
#define	CMD_INT_FAX_GO_FAX_PRINT_REPORT 0x0808		///< [ Internal use ]
#define	CMD_INT_FAX_GO_FAX_RESET_DB 0x0809		///< [ Internal use ]
#define	CMD_INT_FAX_GO_FAX_SP_MODE 0x080A		///< [ Internal use ]
#define CMD_INT_FAX_GO_FAX_ACK_READ_DB 0x080B   ///< [ Internal use ]
#define CMD_INT_FAX_GO_FAX_DELETE_DOCMAN 0x080C   ///< [ Internal use ]
#define CMD_INT_FAX_SEND_FAX_STATUS 0x0881		///< [ Internal use ]
#define CMD_INT_FAX_SEND_FAX_MEMORY_STATUS 0x0882		///< [ Internal use ]
#define CMD_INT_FAX_SEND_FAX_REQUEST_READ_DB 0x0883		///< [ Internal use ]
#define CMD_INT_FAX_SEND_FAX_BROADCAST_JOBID 0x0884		///< [ Internal use ]
#define CMD_INT_FAX_SEND_PCFAX_JOBID 0x0885		///< [ Internal use ]
#define CMD_INT_FAX_PRINT_JOB_RES	0x088A	///<[ Internal use ] Send FAX Print Result
#define CMD_INT_TX_SEND_RESULT 0x0921	///< [ Internal use ] Sending Email Result etc
#define CMD_INT_SPECIAL_NOTIFICATION	0x0F83	///< [ Internal use ] Send Special Notification
#pragma pack(0)


#ifdef __cplusplus
}
#endif

#endif //SYS_UI_INTERFACE_H_
