#ifndef __PRINTMGR_H__
#define __PRINTMGR_H__
/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2004-2009 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#include "ATypes.h"
#include "agprint.h"
#include "paper_size_config.h"
#include "paper_types_config.h"
#include "response_subject.h"
#include "agMessage.h"

#include "print_events_common_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Enumeration of modes, or basic states, for the print manager.
 */
typedef enum
{
    PM_NORMAL = 0,  ///< We can print as long as the system and engine are online.
    PM_STALLING,    ///< We found a job that needs user input; waiting for active pages in the engine to finish.
    PM_STALLED,     ///< We are waiting for user input before continuing with the next pending page.
} PRINTMODE;

#ifdef HAVE_DBG_PRINTF // *** If you add an enum value above, also add it to the ToString method below! ***
/**
 * \brief Return a string with the symbolic name of the given enum value.
 * 
 * \param enumValue (PRINTMODE) enum value to decode.
 * 
 * \return const char* String name for enum value.
 * 
 * This method is intended for use in debug printf calls.
 */
    const char* ToString_PRINTMODE( PRINTMODE enumValue );
#else
    static inline const char* ToString_PRINTMODE( PRINTMODE enumValue ) {return 0;}
#endif

typedef enum
{
    PMCANCEL_NONE = 0,
    PMCANCEL_ENGINE,
    PMCANCEL_WAIT_FOR_ENGINE_ONLINE,
} PAGECANCEL;

enum {
	LOCAT_KOREA,
	LOCAT_CHINA,
	LOCAT_NA_LT,
	LOCAT_NA_A4,
	LOCAT_UNKNOWN = 100,
};

/**
 * \brief Return a string with the symbolic name of the given enum value.
 * 
 * \param enumValue (PRINTMODE) enum value to decode.
 * 
 * \return const char* String name for enum value.
 * 
 * This method is intended for use in debug printf calls.
 */
const char* ToString_PAGECANCEL( PAGECANCEL enumValue );


/**
 * The page manager has a psuedo state machine. The state is indicated by
 * the two online variables sysOnline and engOnline. sysOnline tells the
 * status of the print system (every thing but the eng and video). engOnline
 * indicates the state of the engine and video..
 *
 * <pre>
 * PM_OFFLINE:  sysOnline == FALSE && engOnline == FALSE.
 *              Don't process any page messages (cal, test, or print)
 *              Ignore any page dones from engine (eng sometimes sends wrongly after paper jam)
 * PM_SYSREADY: sysOnline == TRUE && engOnline == FALSE
 *              Don't process any page messages (cal, test, or print)
 *              Ignore any page dones from engine (eng sometimes sends wrongly after paper jam)
 * PM_ENGREADY: sysOnline == FALSE && engOnline == TRUE
 *              System will go offline for cals and diagnostics
 *              Only process cal pages
 * PM_READY:    sysOnline == TRUE && engOnline == TRUE
 *
 * Transition table
 * Cur State     Event        Next State
 * ======================================
 * PM_OFFLINE    sys online   PM_SYSREADY
 * PM_OFFLINE    eng online   PM_ENGREADY
 * PM_SYSREADY   eng online   PM_READY
 * PM_SYSREADY   sys offline  PM_OFFLINE
 * PM_ENGREADY   sys online   PM_READY
 * PM_ENGREADY   eng offline  PM_OFFLINE
 * PM_READY      sys offline  PM_ENGREADY
 * PM_READY      eng offline  PM_SYSREADY
 * </pre>
 */                                          
typedef struct
{
    uint8_t     engOnline;      ///< true if the engine and video are online (no errors, ready to go)
    uint8_t     sysOnline;      ///< true if the system is online
    uint8_t     numCredits;     ///< current number of pages the engine can process
    uint8_t     maxCredits;     ///< max number of pages the engine can process
    PAGECANCEL  pageCancelState;///< it takes several states to do the cancel
    AGMSG       cancelType;     ///< Saved value of cancel message MSG_<value>.
    void*       cancelPtr;      ///< Saved value of cancel message param3.
    uint8_t     statReqCnt;     ///< number of outstanding status reqs at engine
    uint32_t    receivedFirstManualDuplexSecondSide;    ///< set to current job number when we have encountered 
                                                        /// special marker page for manual duplex; 0 otherwise.
    PRINTMODE   mode;           ///< normal, stalling, stalled, calibrating
    uint32_t    status;         ///< current status of engine
} PrintMgrInfo;




void PrintMgrInit(void);

uint32_t PMGetStallStatusWord( void );
int PrintMgrPrintState(int argc, char* argv[]);

// used for paper and tray events.
typedef struct printmgr_media_prompt_info_s
{
    trayenum_t    prompt_input_tray;
    mediatype_t   prompt_mediatype;
    mediasize_t   prompt_mediasize;
    trayenum_t    prompt_output_tray;
} printmgr_media_prompt_info_t;

#define PrintMgr_myDeviceStatus_ID 0x42  
void PrintMgr_AttachObserver_devicestatus(Observer_t *o);
void PrintMgr_DetachObserver_devicestatus(Observer_t *o);
uint32_t PrintMgr_Get_devicestatus( Subject_t *s);
TRY_AGAIN PrintMgr_Get_retry_callback(Subject_t *s) ;
USER_CANCEL PrintMgr_Get_cancel_button_callback(Subject_t *s);

#define PrintMgr_myMediaPromptID 0x43
void printmgr_media_prompt_AttachObserver(Observer_t *o);
void printmgr_media_prompt_DetachObserver(Observer_t *o);
void printmgr_media_prompt_get_media_info(Subject_t *s, printmgr_media_prompt_info_t *media_info);
uint32_t printmgr_media_prompt_get_status_event(Subject_t *s);
TRY_AGAIN printmgr_media_prompt_get_retry_callback(Subject_t *s);
USER_CANCEL printmgr_media_prompt_get_cancel_button_callback(Subject_t *s);

#define PrintMgr_myMediaStatus_ID 0x44  
void PrintMgr_AttachObserver_MediaStatus(Observer_t *o);
void PrintMgr_DetachObserver_MediaStatus(Observer_t *o);
uint32_t PrintMgr_Get_MediaStatus( Subject_t *s);
TRY_AGAIN PrintMgr_Get_MediaStatus_RetryCallback(Subject_t *s) ;
USER_CANCEL PrintMgr_Get_MediaStatus_CancelButtonCallback(Subject_t *s);


#ifdef __cplusplus
}
#endif

#endif // __PRINTMGR_H__
