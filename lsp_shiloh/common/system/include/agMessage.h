/*
 *
 * ============================================================================
 * Copyright (c) 2006-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file agMessage.h
 *
 * \brief
 *
 */


#ifndef __AGMESSAGE_H__
#define __AGMESSAGE_H__

#include "ATypes.h"
#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Defines for inter system messages (messages passed between modules) */
/*                           param1       param2        param3   */
typedef enum
{
    MSG_JOBSTART = 1,   ///< 1:0            2:0       3:ptr to JOB_DATA structure 
    MSG_JOBEND,         ///< 1:0            2:0       3:0   -- don't care   
    MSG_JOBSHEETSTART,  ///< 1:0            2:0       3:ptr to SHEET_DATA structure  
    MSG_JOBPAGESTART,   ///< 1:0              2:0     3:ptr to the PAGE_DATA structure   
    MSG_JOBPAGEEND,     ///< 1:0              2:0     3:0     
    MSG_JOBPLANESTART,  ///< 1:0              2:0     3:ptr to the PLANE_DATA structure.  Not used with zj   
    MSG_JOBPLANEEND,    ///< 1:Plane color    2:0     3:0  end of this planes color.  
    MSG_JOBPLANEDATA,   ///< 1:Color(enum)    2:0     3:ptr to a link header with data included.  single allocation     
    MSG_JOB_FREE_DATA,  ///< 1:Color(enum)    2:0     3:0   notification from print mech to job mgr that plane data buffers can be freed
    MSG_PAGESTART,          ///< 1:0          2:0     3:Ptr to PAGE_DATA
    MSG_START_NEXT_PAGE,  ///< 1:0              2:0     3:0 sent by laser video to itself
    MSG_PAGE_PLANE_START, ///< 1:0              2:0     3:0 sent by laser video to itself
    MSG_PAGEPRESTART,       ///< 1:0          2:0     3:Ptr to PAGE_DATA
    MSG_PAGEPRESTART_RSP,///< 1:0          2:0        3:0 returned from engine if prestart is supported.
    MSG_PAGEDONE,      ///< 1:0         2:0        3:Ptr to PAGE_DATA sent by engine and video to Pagemgr and send to JobMgr   
    MSG_PAGEVIDEODONE,      ///< 1:0 2:0 3:Pointer to printed page. Notify that page is done. Used only by Engine & PrintMgr: PAGEDONE_PROTOCOL_MAY_USE_PAGEVIDEODONE_THEN_PAGEDELIVERYDONE
    MSG_PAGEDELIVERYDONE,   ///< 1:0 2:0 3:Pointer to printed page  Notify that page video is done. Used only by Engine & PrintMgr: PAGEDONE_PROTOCOL_MAY_USE_PAGEVIDEODONE_THEN_PAGEDELIVERYDONE
    MSG_DUPLEXPAGESDONE,    ///< 1:0 2:0 3:0 PrintMgr to JobMgr: combined MSG_PAGEDONE for both front and back pages of a duplex sheet.
    MSG_CREDITUPDATE,  ///< 1:number of pages  2:0  3:0 sent by engine to pm number of pages it can handle at once.
    MSG_STATUSUPDATE,  ///< status - eng and vid send to page manager with their status, pm forwards to stat mgr
    MSG_STATUSREQUEST, ///< 1:0             2:0     3:0 request status from engine by pm    
    MSG_ERRCLEAR,      ///< 1:0           2:0       3:0 clear engine error states and retry
    MSG_USER_ERR_RECOVERY,  ///< from printmgr to eng - user thinks err is fixed, do any automatic err recovery (paper jam detect, paper path clear, reset)
    MSG_DISPLAY_STATUS,     ///<  Look up a status value and put it on the LCD
    MSG_OUT_OF_MEMORY,      ///<  1:0  2:0  3:0  from the parser to job manager saying we are out of memory.
    MSG_RACE_ENGINE,        ///<  1:0  2:0  3:0  race the engine.
    MSG_WAIT,               ///<  1:0  2:0  3:0  wait for the memory to be freed
    MSG_DISPLAY_MSG,        ///<  1:Line to put message 2:message pointer 3:0
    MSG_PRINT_STATUS_EVENT,         ///< 1:status    2:MODULE_ID 3:0 - sent to print mgr when a print status event has occured (defined in print_status_codes.h)
    MSG_STATUS_EVENT_LOG_ONLY,  ///< 1:status code to log       2:0       3:0 - Similar to MSG_STATUS_EVENT, but this status is only written to the event log; it does not impact the system status.
    MSG_USER_INPUT,            ///< USERCHOICE,  RESOURCES (dest resource id)
    MSG_JOBABORT,           ///< 1:0     2:0    3:0   Sent from parser to jobmgr to abort current page Used for i/o timeout.
    MSG_PAPER_ADDED,          ///< 1:ADDEDTRAYS 2:0 3:0 -sent from engine to print mgr any time paper is added
    MSG_OUTPUT_BIN_CLEARED,   ///< 1:0 2:0 3:0 Sent from Engine to PrintMgr any time bin full condition goes away.
    MSG_CONFIGPAGE,           ///<  1:0 2:0 3:0 -- run the configuration page.
    MSG_ALIGNMENT_PAGE,       ///< start an alignment operation, not currently used.
    MSG_SUPPLY_STATUS,        ///< 1:0    2:0 3:0   Initiate a supply status page to ipm.
    MSG_CONTPRINT,               ///< 1:0 2:0 3:0 Continuous print of a test page (RFI)
    MSG_DEMOPAGE,                ///< 1:0 2:0 3:0 Print the demo page
    MSG_NETWORKPAGE,             ///< 1:0 2:0 3:0 Print the network config page
    MSG_PQPAGE,                  ///< 1:0 2:0 3:0 Print the PQ Page
    MSG_CONFIG_AND_SSP,          ///< 1:0 2:0 3:0 Print the config and supplies page as a single job.
    MSG_SUPPLIES_PAGE_DONE,      ///< 1:0 2:0 3:PIXELCNT* Notification to the supplies system that a page has completed printing.  Includes pointer to struct containing the pixel counts; receiver must free the memory.
    MSG_HTML_DATA,          ///<  1:ConHandle     2:0           3:0 notify that html data is available.
    MSG_CLEAN_PRINTER,      ///< 1:0     2:0      3:0  start a clean of the printer.
    MSG_CLEAN_FINISHED,     ///< sent when clean finished, not currently used.
    MSG_MENUPAGE,           ///< 1:0        2:0      3:0  sent to ipm to print a menu page
    MSG_ERRORREPORT,        ///< 1:0        2:0      3:0  sent to ipm to print an error report.
    MSG_INTERNAL_PAGE,        ///< 1: Page to print from DOCTYPE 2: 0 3: 0 Use to signal internal page, use msg params to specify which page DAB
    MSG_DELAY_NOTIFY,         ///<  1:arbitrary callback value set by user 2:0 3:0 -- delay notificiation
    MSG_DELAY_SUBSCRIPTION,   ///< 1:0 add_subscription 1 delete_subscription 2:0 3:0 for use only within DelayMgr.c
    MSG_UPDATE_DISPLAY,     ///< 1:1  2:0   3:0 causes the display to update.
    MSG_RESTART_PAGE,         ///<  1:0 2:0 3:0 sent to PCL to restart a page.
    MSG_FORCE_STALL,          ///< 1: input status 2:0 3:0 -- Force page manager to stall for user input, 
    MSG_MDNS_EVENT,         ///< 1:MDNS_EVENTS  2:0      3:0 used to update mdns states in network
    MSG_WAKEUP,               ///< sent from print manager to engine to wake up	(KARA-S)
    MSG_BUTTON_WAKEUP,        ///< this message indicates that a button has been pressed an we need to come out of low power mode
    MSG_SLEEP_START,          ///< 1:0 2:0 3:0 message to start the process of entering low power sleep mode.
    MSG_SLEEP,                ///< 1:0 2:0 3:0 message to put the engine to sleep; mostly used to force sleep mode for test scenarios (KARA-S)
    MSG_ENGCMD,               ///< ENGCMD SRCID - send engine platform specific msg 
    MSG_WIRELESS_TEST_PAGE,
    MSG_WIRELESS_UPDATE,    ///> 1:0     2:0     3:0   Notification that wireless timer has expired.
    MSG_WIRELESS_START_SCAN,
    MSG_WIRELESS_ATTEMPT_REJOIN,
    MSG_RUN_WPS,
    MSG_CANCEL_WPS,
    MSG_RESOURCES,    ///< 1:0 2:0 3:Ptr to job object -- Sent from resource manager to resource aquired.
    MSG_SCANJOBS,       ///< 1:0  2:0   3:0 Sent by sjm to himself to scan the job pending list.
    MSG_STARTRECIPE,  ///< 1:Job enum, 2:resource to add, 3:pipe.
    MSG_FREERECIPE,   ///< 1:0    2:0   3:param3 ptr received from MSG_RESOURCES  - notify sjm job is complete.
    MSG_START_PARSE,  ///< 1:0    2:0   3:Pipe (IOReg structure) Notification from cm to start parsing.
    MSG_END_PARSE,  ///< Sent by parser to connection mgr to end a parse.
    MSG_SHUTDOWN_PARSE, ///< 1:0      2:0       3:0
    MSG_RTCALARM,           ///< Not currently used.
    MSG_CALGRAPH,       ///< 1:0      2:0       3:0   Sent by scan to start a cal graph
    MSG_ACKRECIPE,      ///< 1:0      2:0       3:0   Sent by sysjobmgr to inform module job is ready to run
    MSG_NAKRECIPE,      ///< 1:0      2:0       3:0   When a msg_startrecipe cannot run
    MSG_STATUS_UNREGISTER, ///< 1:0   2:0       3:List of resources Used only by sysjobmgr to unregister a status channel

    MSG_PCLPARSE_DONE,

    MSG_CANCELJOB,    ///< param1 = action (see DOACTION enum below), param2 =  src router id, action dependent, param3 = jobobject
    MSG_CANCELPAGE,   ///< param1 = action (see DOACTION enum below), param2 =  src router id, action dependent, param3 = jobobject
    MSG_RESTARTPAGE,  ///< param1 = action (see DOACTION enum below), param2 =  src router id, action dependent, param3 = jobobject
    MSG_RETRYPAGE,    ///< param1 = action (see DOACTION enum below), param2 =  src router id, action dependent, param3 = jobobject
    MSG_PRINTONLINE,  ///< param2 = status either on-line or off-line.
    MSG_TRAYCONFIG,   ///< no values used.
    MSG_PRINTSYSSTATUS, ///< param2 status
    MSG_PRINTNUMPAGESUPDATE, ///< param2 numpages/job

    MSG_SCAN_JOBSTART,  ///< use macro (TBD) to encode; used by scan to start a job
    MSG_SCAN_JOBEND,    ///< use macro (TBD) to encode; used by scan to end a job
    MSG_SCAN_SHEETSTART,///< no params; used by scan to start a sheet.
    MSG_SCAN_SHEETEND,  ///< no params; used by scan to end a sheet
    MSG_SCAN_PAGESTART, ///< no params; used by scan to start a page
    MSG_SCAN_PAGEEND,   ///< no params; used by scan to end a page.
    MSG_SCAN_PLANEDATA, ///< use macro (TBD) to decode; scan data
    MSG_SCAN_SIZE,      ///< use macro (TBD) to decode; size of upcoming data
    MSG_SCAN_CAL,       ///< use macro (TBD) to encode; trigger a calibration
    MSG_SCAN_DEV_REQUEST,   ///< device request (davep 27-Sep-2011)
    MSG_SCAN_DATA_BLOB,     ///< scan internal use only; so there
    MSG_SCAN_POWERSAVE,
    MSG_A3MERGE_SET_START,
    MSG_A3MERGE_REQUEST_JOB_START,
    MSG_A3MERGE_REQUEST_2ND_JOB_START,
    MSG_A3MERGE_NOTIFY_MERGE_END,
	MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START,
	MSG_A3MERGE_ROLLER_DETECTED,
	MSG_ERASE_ROLLER_PAGE_START,
	MSG_ERASE_ROLLER_PAGE_DATA,
	MSG_ERASE_ROLLER_PAGE_END,
	MSG_ERASE_ROLLER_SCAN_END,
	MSG_ERASE_ROLLER_SCAN_READY,	
	MSG_ERASE_ROLLER_SET_PARAM,
	MSG_NITIFY_USBTHUMB_DISCONNECT,
	MSG_SENDJOB_RESULT,
	MSG_SENDJOB_FOR_AUTH_FAX_RESULT,
	MSG_SCANNER_SP_MODE_START_MSG,
	MSG_SCANNER_SP_MODE_STOP_MSG,
	MSG_CONNECT_TEST,
	
    MSG_COPY_DEBUG,
    MSG_COPY_INTERNAL,
    MSG_COPY_CONTINUOUS,  ///< param1 true, start cont copy; false, stop cont copy

    MSG_FAX_DEBUG,
    MSG_START_FAX_SEND,
    MSG_START_FAX_RECEIVE,
    MSG_FAX_POLLING_RECEIVE,
    MSG_FAX_COMPELTE,
    MSG_FAX_PRINT,
    MSG_PAGEEND_CMP,
    MSG_JOBEND_CMP,
    MSG_FAX_FSEND,
    MSG_FAX_FAIL,
    MSG_NEWPAGE_CMP,
    MSG_HANDSET_HOOKSWITCH,
    MSG_HANDSET_RECORD,
	MSG_HANDSET_DIAL,
	MSG_HANDSET_HANGUP,
	MSG_HANDSET_SWITCH_TO_FAX,
    MSG_FAX_XMITTEST,
    MSG_FAX_ACTIVITYLOG,
    MSG_FAX_CALLREPORT,
    MSG_FAX_PHONEBOOK,
    MSG_FAX_BLOCKLIST,   
    MSG_FAX_T30TRACE,
    MSG_FAX_PRINT_SELF_TEST,
    MSG_FAX_RUN_SELF_TEST,
    MSG_FAX_BILLING,
    MSG_FAX_START_REPRINT,
    MSG_FAX_REPRINT,
	MSG_AUDIO_BEEP,
	MSG_AUDIO_CPA_ON,
	MSG_AUDIO_CPA_OFF,
	MSG_AUDIO_RING,
	MSG_AUDIO_ALARM,
	MSG_AUDIO_DONE,
	MSG_FAXER_EVT,

    MSG_COMPCMD,        ///< 1:Comp Cmd   2:0   3:Structure of what to compress command message for the compression subsystem.
    MSG_DBGSTAT,    ///< to enable (param1=1) or disable(param1=0) the debug mode. And is param2=1, it was previously in debug mode defined in dbgStatus.c
    MSG_DBGSTATEVT, ///< param1=status event, the event that is to simulated. defined in dbgStatus.c
    MSG_SYSSTATUS_EVENT,  ///< event,  src, NULL - set out of band system status
    MSG_TIME_TO_END,    ///< Param1 is the number of 10msec till the end of a page.estimate.  Used by copy
    MSG_COMPRESSION,    ///< Not currently used.


    // KEYPAD and CONTROL PANEL (CTRLPANEL) Messages
                        // PARAMS:
    MSG_CP_KEY_PRESSED, ///< 1: KEYPAD_KEY    - 2: NONE   - 3: NONE
    MSG_CP_START_COPY,  ///< 1: CP_COPY_COLOR - 2: NONE   - 3: NONE
    MSG_CP_BLINK_LED,   ///< 1: NONE          - 2: NONE   - 3: NONE 
    MSG_CP_TURN_LED_ON, /// 1: LED           - 2: NONE   - 3: NONE 
    MSG_CP_TURN_LED_OFF,/// 1: LED           - 2: NONE   - 3: NONE 
    MSG_CP_SLEEP,       /// 1: NONE          - 2: NONE   - 3: NONE 

	//KEYPAD MANAGER MESSAGES
    MSG_KM_STATUS_UPDATE, ///< 1: NONE  2: NONE  3: NONE
    MSG_KM_ENABLE_LED,  ///< 1: LED 2: NONE 3: NONE
    MSG_KM_DISABLE_LED, ///< 1: LED 2: NONE 3: NONE
    MSG_KM_BLINK_LED,   ///< 1: LED 2: RATE 3: NONE
    MSG_KM_POWER_BUTTON, ///< 1: NONE 2: NONE 3: NONE

    MSG_JPEG_PAGE,  ///< Not currently used
    MSG_JPEG_THUMB,  ///< 1: NONE  - 2: NONE  - 3: Pointer to Open Jpeg File to decompress and generate thumbnail from (TO JPEG)
                                              // or Pointer to Decompressed XRGB buffer (FROM JPEG)

    // CGD MESSAGES
    // The parameters for these messages are defined in agCGDMgr.h
    MSG_CR_RENDER_SCREEN,         ///< 1: NONE  2: BOOL limitedRedraw  3: CR_SCREEN
    MSG_CR_RENDER_READY_SCREEN,   ///< 1: NONE  2: BOOL limitedRedraw  3: CR_SCREEN
	MSG_CR_RENDER_MSG_SCREEN,     ///< 1: NONE  2: BOOL limitedRedraw  3: CR_MSG_SCREEN
    MSG_CR_RENDER_INIT_ANIM,      ///< 1: NONE  2: BOOL limitedRedraw  3: CR_SCREEN
    MSG_CR_RENDER_ANIMATION,      ///< 1: NONE  2: BOOL limitedRedraw  3: CR_ANIMATION
    MSG_CR_RENDER_MENU,           ///< 1: UI_MGR_OBJECT  2: BOOL limitedRedraw  3: CR_MENU
    MSG_CR_RENDER_SLIDER,         ///< 1: NONE  2: BOOL limitedRedraw  3: CR_SLIDER
    MSG_CR_RENDER_SPINNER,        ///< 1: NONE  2: BOOL limitedRedraw  3: CR_SPINNER    
    MSG_CR_RENDER_IPV4_SPINNER,   ///< 1: NONE  2: BOOL limitedRedraw  3: CR_IPV4_SPINNER 
    MSG_CR_RENDER_LAYOUT_MENU,    ///< 1: NONE  2: BOOL limitedRedraw  3: CR_LAYOUT_MENU    
    MSG_CR_RENDER_PHOTO_BROWSER,  ///< 1: Number of CR_THUMB structs  2: BOOL limitedRedraw  3: CR_PHOTO_BROWSER
    MSG_CR_RENDER_PHOTO_CROP,     ///< 1: NONE 2: BOOL limitedRedraw  3: CR_PHOTO_CROP   


    // UI Messages
    MSG_UI_DISK_LOADING,        ///< 1: NONE                                2: NONE             3: NONE
    MSG_UI_DISK_INSERTED,       ///< 1: BOOL, TRUE=PhotosAvailable          2: NONE             3: NONE
    MSG_UI_DISK_REMOVED,        ///< 1: NONE                                2: NONE             3: NONE
	MSG_UI_KEY_PRESSED,         ///< 1: Key                                 2: NONE             3: NONE
    MSG_UI_TIMER_EXPIRED,       ///< 1: NONE                                2: NONE             3: NONE

    MSG_FREEIO,                 ///< 1: 0   2: 0    3: pointer to job object sent in MSG_RESOURCES
    MSG_UI_KEY_HELD,            ///< 1: KEYPAD_KEY (key that was held)      2: NONE             3: NONE 
    MSG_UI_VIRTUAL_KEY_PRESSED, ///< 1: KEYPAD_KEY                          2: NONE             3: NONE
    MSG_UI_TIMER_EXPIRE,        ///< 1: NONE                                2: NONE             3: NONE
    MSG_UI_RESTORE,             ///< 1: NONE                                2: NONE             3: NONE
    MSG_WIRELESS_WPS,           ///< WPS push button

    // LangMgr Messages
    MSG_LANG_START,             //1: lang ps,pcl,pxl/0,1,2               2:NONE              3: NONE
    MSG_LANG_END_PARSE,         //1: lang ps,pcl,pxl/0,1,2               2:NONE              3: NONE
    MSG_LANG_DOWN,              //1: lang ps,pcl,pxl/0,1,2               2:NONE              3: NONE
    MSG_LANG_CANCEL_JOB,        //1: lang ps,pcl,pxl/0,1,2               2:NONE              3: NONE  
    MSG_LANG_CANCEL_PAGE,       //1: lang ps,pcl,pxl/0,1,2               2:NONE              3: NONE
    MSG_LANG_CRC_PAGE,          //1: lang ps,pcl,pxl/0,1,2               2:NONE              3: NONE
    MSG_RENDER_COMPLETE,     //Used by renderer to inform end of a renderering

    // USB configuration message
    MSG_USB_CONFIG,             //1: first 4 bytes of setup              2: last 4 bytes of setup 3: usb handle
    
    MSG_SETTING_CHANGED,        //parameter change indication

    MSG_MLAN_DRVR_EVENT,

    // Thread control messages
    MSG_THREAD_EXIT,            ///< sent to tell a thread to cleanly exit its message processing loop and halt

    MSG_PRINT_DATA,             ///< print data, param3=pointer to data buffer, must be freed.
	
    MSG_SEND_SYNC,              ///< Send a sync message.

    MSG_HTTP_START,
    MSG_HTTP_PHOTO,

    MSG_COPY_ID_COPY,      	///< start ID copy
    MSG_COPY_ID_COPY_INPUT,     ///< ID copy action (next, finish, cancel)


    MSG_WEB_SERVICES_PAGE,      ///< cloud info-sheet req
    
    MSG_STATUS_DRUM_RESET,	///< Reset Drum cartridge		

    //#######################################################################################
    // Additional Messages for Kara-S DVT (SJMgr - UIMgr) - Start
    //#######################################################################################
    
    // System Initial State Messages
    MSG_SYSREADY = 260, 	///< 1: NONE	2: NONE 3:NONE
    MSG_SYSFAIL,				///< 1: NONE	2: NONE 3:NONE
    
    // Machine State Messages
    MSG_STATUS_DOOR_OPEN,	///< 1: 0=Close, 1=Open (bit0=RearDoor, bit1=FrontDoor, bit2=UpperDoor, bit3=FinisherDoor, bit4=BridgeDoor) 	2: NONE 3: NONE
    MSG_STATUS_TRAY_EMPTY,	///< 1: <Tray Number> 0=Manual, 1=Tray1, 2=Tray2, 3=Tray3, 4=Tray4		2: <Paper Volume> 0=PaperEnd, 1=~20%, 2=~40%, 3=~60%, 4=~80%, 5=~100%		3: NONE
    MSG_STATUS_JAM, 		///< 1: Jam Code generated by engine (101~107)  200=Stack Full, 201=Invalid Cartridge	2: NONE 3: NONE
    MSG_STATUS_TONER,		///< 1: 0=NotDetect, 1=End, 2=NearEnd, 9=OK 	2: NONE 3: NONE
    MSG_STATUS_DRUM,		///< 1: 1=End, 2=NearEnd, 3=PreNearEnd, 9=OK		2: NONE 3: NONE
    MSG_SERVICE_CALL,		///< 1: SCcode	2: NONE 3: NONE    
    MSG_TRAY_INFO, 			///< 1: <Tray Number> 0=Manual, 1=Tray1, 2=Tray2, 3=Tray3, 4=Tray4 2:  <Tray Status> 1: Open	2: Close	<Media Type> 3: Media Letter 4: Media Exec
    							//	5: Media A4		6: Media B5		7: Media Legal		8: Media A5		9: Media Unknown		<Paper Amount>		10: Paper Low		11: Paper Normal    3: NONE
    MSG_TRAY_ATTACHED,		///< 1: 	1: Manual and Tray 1 (Default)	2: Tray 2, Manual and Tray 1	3: Tray 3, Tray 2, Manual and Tray 1    2: NONE 3: NONE
    
    // Job State Messages
    MSG_STATE_PRINTJOB, ///< 1: 0=Ready, 1=Busy, 2=Warmup, 3=EnergySave, 4=DoorOpen, 5=Abnormal(Jam), 6=Abnormal(SC)		2: NONE 3: NONE
    MSG_STATE_UI_LED,		///< 1: 0=AllOff, 1=LED1(W), 2=LED2(R), 3=LED3(G), 4=LED4, 0x10=AllOn		2: 0=Off, 1=On, 2=Blink, 3=Dim		3: NONE
    
    // UI Request Messages
    MSG_CONFIG_CHANGED, ///< 1: 0=All, 1=Print, 2=Copy, 3=Fax		2: NONE 3:NONE
    MSG_REPORT_PRINT,		///< 1: 0=System, 1=Network, 2=Service, 3=Pattern, 4=Error		2: NONE 3:NONE
    
    MSG_COLD_WAKEUP,
    MSG_PAUSE,
    MSG_RESUME,
    
    MSG_STATUS_MOTOR_RUN,
    MSG_TRAY_VERSION,
    MSG_CARTRIDGE_INFO,
    MSG_CARTRIDGE_PRINT_JOB_MODE_INFO,
    MSG_CARTRIDGE_PRINTED_BY_COVERAGE_INFO,
    MSG_DRUM_PAGE_COUNT,
    MSG_STATUS_TONER_REMAIN_CHK_REQ,
    MSG_MACHINE_LOCATION_INFO,
    
    MSG_AUTH_RESULT,

    MSG_IMAGE_LOG_NOTIFY,	///< 1: 0=JobStart, 1=JobDone	2: NONE 3: NONE
    MSG_IMAGE_LOG_TIMEOUT,	///< 1: Timeout Value	2: SenderID 3: NONE
    MSG_SCAN_BOX_TIMEOUT,	///< 1: Timeout Value	2: SenderID 3: NONE

    //#######################################################################################
    // Additional Messages for Kara-S DVT (SJMgr - UIMgr) - End
    //#######################################################################################
	
	MSG_START_LAMINATION,
	MSG_START_LAMINATING_1PAGE,
	MSG_END_LAMINATION,
	MSG_CHECK_EXITGUIDE_ATTACH,
	MSG_CHECK_EXITGUIDE_DETACH,
	MSG_STOP_LAMINATION,
	MSG_RESUME_LAMINATION,
	MSG_STATUS_FUSER_RESET,    ///<1: 2=Fuser unit reset 2:NONE 3: NONE
    
	//Fax Print App Messages
#ifdef HAVE_SDHFAX 
	FAXAPP_MSG_NEXT_PAGE,
	FAXAPP_MSG_CANCEL_DONE,
	FAXAPP_MSG_PAGE_END,
	FAXAPP_MSG_SCAN_JOB_INFO,
#endif
	
      /* This must always be the last element of the enum.
     * Do not use this message for anything.
     */
     
    MSG_LAST_MSG = 0x10000
}AGMSG;

/** PLANEDATA_EX compressed data types */
/* TODO: Remove these after the new print api is fully adopted */
#define PLANEDATA_COMPTYPE_NONE     0  ///< Not compressed
#define PLANEDATA_COMPTYPE_LOSSLESS 1  ///< Lossless compression (not jpeg!)
#define PLANEDATA_COMPTYPE_JPEG     2  ///< Jpeg compression 
#define PLANEDATA_COMPTYPE_JBIG     2  ///< Jbig compression 

#if 0
//param defines for MSG_DATASTORE_NOTIFY
#define PARAM_DS_LOOKUP   1  //have to look up the value, too bit to fit in message (strings and structs)
#define PARAM_DS_FOLLOWS  2  //value of eDSVar is in param 2 (int values)

/* param defines for MSG_DATASTORE_NOTIFY */
#define PARAM_DS_LOOKUP   1  ///< Have to look up the value, too bit to fit in message (strings and structs)
#define PARAM_DS_FOLLOWS  2  ///< Value of eDSVar is in param 2 (int values)
#endif

typedef enum
{
    SYS_REQUEST,   ///< From resource module on error, to sys job manager to cancel, restart or retry
    SYS_REQUSER,   ///< From out of band module, not part of the job path
    SYS_START,     ///< From sys job manager to resource module to cancel, restart or retry
    SYS_ACK,       ///< From resource module to sys job manager, cancel, restart or retry done, paused
    SYS_RESUME,    ///< From sys job manager to resource module to resume
    LOCAL_START,   ///< From resource module to sub module to cancel, restart, or retry
    LOCAL_ACK,     ///< From sub module to resoure module, cancel, restart or retry done, now paused
    LOCAL_RESUME,  ///< From resource module to submodule, resume

    // Always add new values above this line, and be sure to << ATTENTION! <<
    // update the switch statement in ToString_DOACTION().   << ATTENTION! <<
    DOACTION_NUM_ENUMS // always last in the list
}DOACTION;

#ifdef HAVE_DBG_PRINTF
    const char* ToString_DOACTION(DOACTION action);
#else
    static inline const char* ToString_DOACTION(DOACTION action) {return 0;}
#endif

/** Param defines for MSG_USER_INPUT */
#define PARAM_USER_OK         1
#define PARAM_USER_CANCEL     2
#define PARAM_USER_CANCEL_JOB 3
#define PARAM_USER_FINISH_JOB 4

/**
 * Param defines for MSG_PAPER_ADDED
 * bitmask indicating which trays had paper added
 */
#define PARAM_ADDED_TRAY1 0x01
#define PARAM_ADDED_TRAY2 0x02
#define PARAM_ADDED_TRAY3 0x04
#define PARAM_ADDED_TRAY4 0x08

/** Param defines for MSG_CLEAN_PRINTER */
#define PARAM_STD_CLEAN_PAGE 1
#define PARAM_ETB_CLEAN_PAGE 2
#define PARAM_2STAGECLEAN_PAGE1     3
#define PARAM_2STAGECLEAN_PAGE2     4

/** 
 * Message passing function for platform specific stuff 
 * This gives a generic way to call from common code into 
 * platform specific code for things that don't fit in our
 * normal message passing system. Such as commands from PJL
 * that do obscure platform specific stuff.
*/
typedef enum
{
    ENGFW_UPDATE = 0,
    FUSER_DISABLE,
    ENGINT_COMPLETE,
}PLATMSG;

extern SINT32 HandlePlatformMessage(PLATMSG msgName, void* msgValue);


/**
 * Base for intra process messages (messages internal to a module)
 * Add this base to your internal module messages so they will be 
 * guaranteed not to clash with inter module messages
*/
#define INTRA_MSG_BASE     (MSG_LAST_MSG+1)       // Use this define
#define IntraMsgBase       INTRA_MSG_BASE

typedef struct
{
    AGMSG msgType;
    UINT32 param1;
    UINT32 param2;
    void*  param3;
}MESSAGE;


#define 	SYS_UI_MESSAGE_DATA_LEN		50

typedef struct
{
    UINT32 msgSender;
    UINT16 cmd;
    UINT8 data[SYS_UI_MESSAGE_DATA_LEN];
}SYS_UI_MESSAGE;

#define SYS_MSG_SIZE    TX_4_ULONG

#ifdef __cplusplus
}
#endif

#endif


