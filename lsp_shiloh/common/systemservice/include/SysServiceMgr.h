/*
 ***********************************************************
 *(c) Copyright 2012-2015 Sindoh Co. Ltd. 
 *
 *               SINDOH Confidential
 *==========================================================
 **/
/**
 *\file SysServiceMgr.h
 *
 *\brief Defines for the system service layer.
 *
 **/

#ifndef SYS_SERVICE_MGR_H_
#define SYS_SERVICE_MGR_H_
#include <sys/types.h>
#include <sys/shm.h>
#include "ATypes.h"
#include "event_observer.h"
#include "response_subject.h"
#include "agMessage.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PROC_FILESYSTEM 		"/proc/partitions"
#define PROC_FILEBUFSIZE    2048

#define ERR_RECOVERABLE 0xE0EE
#define ERR_NONRECOVERABLE 0xE0FF

typedef enum {
	SETTING_TRAY_COUNT	= 0,
	TRAY_STATUS			= 1,
	TRAY_PAPER_AMOUNT	= 2,
	TRAY_PAPER_SIZE		= 3,
} TRAY_INFORMATION_REQUEST;

typedef enum
{
	APPCB_FAX_PRINT_FINISHED,	
	APPCB_CALLBACK_MAX
}APP_CALLBACK_ACTION;


typedef void (*AppCallbackFun)();

typedef struct stCountryTable_ {
	UINT8 language;
	UINT8 paperType;
	UINT8 faxCode;
	UINT8 TimeZoneNo;
} stCountryTable;
#define MAX_COUNTRY_CODE    72+1
extern const stCountryTable CountryTable[];
/**
 * 
 *\brief Register a Callback function with System service.
 * Attach callback with APP_CALLBACK_ACTION and call callback when that action occured.
 *
 * \param[APP_CALLBACK_ACTION] type Call back function action
 * \param[AppCallbackFun] callbackfun Call back function pointer
 **/
void RegisterAppCallback(APP_CALLBACK_ACTION action, AppCallbackFun callbackfun);

/**
 * 
 *\brief The initialization routine for notification fucntionality.
 * Attach observers for which SSL need callback or notification.
 *
 **/
void	SysServiceObserverInit(void);

/**
 * 
 *\brief Services provide for Initialization Completion. 
 *
 **/
void InitCompletionService(SYS_UI_MESSAGE *Message);

uint32_t GetTrayPJLStatusCode(void);

/**
 * 
 *\brief Services provide for Machine Configuration. 
 *  Get machine configuration data and send to UI.
 *
 **/
void MachConfService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Screen change request from UI.
 *
 **/
void ScreenChangeReqService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief USB thumb drive connected or disconnected service for UIManager. 
 *
 **/
void USBThumbDriveService(SYS_UI_MESSAGE *Message);

/**
 *\brief Services provide for system job action. 
 *
 **/
void SysJobTakeActionService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for Key pressed or touch event.
 *
 **/
void KeyTouchEventService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for Energy save mode.
 *
 **/
void EnergySaveModeService(SYS_UI_MESSAGE *Message);


/**
 * 
 *\brief Time out control stop and restart timeout timer. 
 *
 **/
void TimeoutControl(uint32_t type);

/**
 * 
 *\brief Update Time out value for energy save mode.
 * Read from dababase and set new TIME OUT value.
 *
 **/
void UpdateEnergySaveTimeout();

/**
 * 
 *\brief Get current hour remaining minute. 
 *
 **/
int GetCurrentHourRemainMinute(void);

/**
 * 
 *\brief Get current week time of the system. 
 *
 **/
int GetCurrentWeekTime(void);

/**
 * 
 *\brief Get current rate of specific time.
 * return result of select rate from record_adjustsleepTimer where dateHour = currH
 *
 **/
int GetCurrentRate(int currH);

/**
 * 
 *\brief Clear next rate from current time.
 * Clear rate at one hour after from now (to ensure correctness of data).
 *
 **/
int ClearNextRate(void);

/**
 * 
 *\brief Get Minute adaptive energy saving.
 * Return rate of 7 days (=7*24 hours) ago.
 *
 **/
int MinuteAdaptiveEnergySaving(void);

/**
 * 
 *\brief Services provide for Scanner and Plotter mode status.
 *  0 : Ready 1 : Busy 2 : Warmup 3 : EnergySave 4 : Door Open : not used 5 : Abnormal (jam) 6 : Abnormal (SC) 
 *
 **/	
void ScannerPlotterModeStatusService(uint32_t scnrStatus, uint32_t pltrStatus);

/**
 * 
 *\brief Services provide for Plotter mode status. 
 *  0 : Ready 1 : Busy 2 : Warmup 3 : EnergySave 4 : Door Open : not used 5 : Abnormal (jam) 6 : Abnormal (SC) 
 *
 **/
void PlotterModeStatusService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for Plotter door open status. 
 *
 **/
void PlotterDoorOpenService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for Feed tray status. 
 *  For Tray 1, 2, 3 and MPT
 *
 **/
void PlotterFeedTrayStatusService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for Jam occur status messages. 
 *
 **/
void PlotterJamStatusService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for Toner and Drum status. 
 *
 **/
void TonerDrumStatusService(SYS_UI_MESSAGE *Message);	
		

/**
 * 
 *\brief Services provide for System Call (SC) status. 
 *
 **/
void SCStatusService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for System Call (SC) status. 
 *
 * \param scCode Service Call (SC) code
 * \param senderModule Which module/app want to send sc code to UI
 **/
void SendSCStatusToUI(UINT32 scCode, UINT32 senderModule);

/**
 * 
 *\brief Print job requested paper size not available in requested Tray then notify to UIManager. 
 *
 **/
void InputTrayPaperService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief PCU status service. 
 *
 **/
void PCUStatusService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief PCU Drum Cartridge Reset service. 
 *
 **/
void PCUResetService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Save error log information in database.
 *
 **/
void SaveErrorLogInDB( int ErrorType, int ErrorCode, bool AllowDuplicate);

/**
 * 
 *\brief Services provide for go print result. 
 *
 **/
void PrintGoPrintResultService(SYS_UI_MESSAGE *Message);	

/**
* 
*\brief Services provide for network response. 
*
**/
void NetworkResponseService(SYS_UI_MESSAGE *Message);

/**
* 
*\brief SP request service for SP mode. 
*
**/
void SPRequestService (SYS_UI_MESSAGE *Message);

/**
* 
*\brief SP mode done like send hardware test off.
*
**/
void SendSPModeDone ();

/**
* 
*\brief SP response for sensor test result. 
*
**/
void SPResponseSensorTest(unsigned char* buffer);

/**
* 
*\brief SP response for Solenoid test result. 
*
**/
void SPResponseSolenoidTest();

/**
* 
*\brief SP response for Motor test result. 
*
**/
void SPResponseMotorTest(int trayNo);

/**
* 
*\brief SP response for Scanner Sensor test result. 
*
**/
void SPResponseScannerSensorTest(unsigned int test_mode, unsigned char sensor_result);

/**
 * 
 *\brief Services provide for print sub system. 
 *
 **/
void PrintSubSystemService(SYS_UI_MESSAGE *Message);	

/**
 * 
 *\brief Set current print system status and add filter.
 *
 **/
void SetPrintSystemStatus(uint32_t fStatus, uint32_t pmStatus);

/**
 * 
 *\brief Get current print system status.
 *
 **/
uint32_t GetPrintSystemStatus(void);

/**
 * 
 *\brief Get current PJL Status code of FAX system
 *
 **/
uint32_t GetFaxPJLStatusCode(void);

/**
 * 
 *\brief Services provide for Copy Application. 
 * Communication layer for Copy UI application and Copy App in middleware.
 *
 **/
void CopyAppService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for go copy result. 
 * Send copy job's status message to UI Manager.
 *
 **/
void CopyGoCopyResultService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Copy Application event notification. 
 *
 **/
void CopyAppEventService(SYS_UI_MESSAGE *Message);

uint32_t GetScanPJLStatusCode(void);

/**
 *
 *\brief Services provide for Scan Application.
 * Communication layer for Scan UI application and Scan App in middleware.
 *
 **/
void ScanAppService(SYS_UI_MESSAGE *Message);

/**
 *
 *\brief Scan Application event notification.
 *
 **/
void ScanAppEventService(SYS_UI_MESSAGE *Message);

/**
 *
 *\brief Service to get Scanner system status like paper JAM or SC.
 *
 **/
void ScanmanEventService(SYS_UI_MESSAGE *Message);

/**
 *
 *\brief Used For Scan Application check USB Thumb is mounted or not.
 *
 **/
char * findingUSBThumb(int *notShowPosition);

/**
 * 
 *\brief Services provide for SP change (message from UI). 
 * UI send SP change message to Marvell process.
 *
 **/
void SPChangedService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Services provide for Go Print (message from UI). 
 *
 **/
void GoPrintService(SYS_UI_MESSAGE *Message);

/**
 * 
 *\brief Check DB status and if DB fail then send message to UI Manager.
 *
 **/
void SysServiceCheckSystemStatus(void);

//************* START: EMAIL -ALERT FUNCTIONS *********
int ErrorNotifyToEmailAlertRequest(int alertType, uint32_t errorCode);
int NotiMsgNotifyToEmailAlertRequest(int nofiMsgType);

/**
 * 
 *\brief Check Error Email Alert Function ON/OFF from DB.
 *
 **/
int GetAutoEmailAlertFlag(void);
int ErrorNotifyToEmailAlertSend(int alertType, uint32_t errorCode);

//************* END: EMAIL -ALERT FUNCTIONS *********

/**
 * 
 *\brief The initialization routine for the global variable in System Service Layer.
 *
 **/
void SysServiceVariableInit(void);


/**
 * 
 *\brief The initialization routine for the System Service Layer.
 *
 **/
void SystemServiceInit(void);

/**
 * 
 *\brief The initialization routine for the System Service Receiver.
 *
 **/
void SysServiceReceiverInit(void);

/**
 * 
 *\brief The initialization routine for the System Service Sender.
 *
 **/
void SysServiceSenderInit(void);

/**
 * 
 *\brief Get the size of paper in Tray1.
 *
 **/
int getTray1SizeCode(int detectedPaperSize);

int GetTrayInform(TRAY_INFORMATION_REQUEST request, int trayNum);

#ifdef __cplusplus
}
#endif

#endif //SYS_SERVICE_MGR_H_
