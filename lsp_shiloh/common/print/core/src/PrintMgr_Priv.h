/**
 *
 * ============================================================================
 * Copyright (c) 2007-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifndef PrintMgr_Priv_h
#define PrintMgr_Priv_h
/** 
 * @file private header file for shared items in PrintMgr and TrayPaperResolve.
 * No public interfaces!  Only exists to allow tray selection algoritm to be swapable.
 **/

#include <stdbool.h>
#include <stdint.h>
#include "agprint.h"
#include "print_job_types.h"
#include "lassert.h"
#include "db_api.h"


#ifdef __cplusplus
extern "C" {
#endif


/*
 * bitmasks for stall checking
 *
 * These are used in two ways:
 * 1. To indicate what information was sent by the
 *    driver. If the size, type and tray are given we use them
 *    else we have to figure them out using the stall logic
 * 2. When looking for a tray that best matches the given information
 *    these are used to indicate what information matches
*/
#define HaveNone          0x00   ///< nothing given or no match found
#define HaveTray          0x01   ///< tray given
#define HaveSize          0x02   ///< size given or size in tray  matches
#define HaveTrayHaveSize  0x03   ///< size and tray given or match in tray
#define HaveType          0x04   ///< type given or match in tray
#define HaveTrayHaveType  0x05   ///< tray and type given
#define HaveSizeHaveType  0x06   ///< size and type given or match in tray
#define HaveAll           0x07   ///< tray, type, and size given
#define HaveOutTray       0x08   ///< output tray given
#define HavePaper         0x10   ///< no match found but found tray with paper


typedef enum
{
    STALL_NONE = 0,
    STALL_PAPEROUT,        ///< prompt user to load paper in requested tray
    STALL_SIZETYPE,        ///< prompt user to put correct size or type in requested tray
    STALL_RECONFIGURE,     ///< prompt user to reconfigure tray
    STALL_MANUALDUPLEX,    ///< prompt user to reinsert paper for 2nd half of manual duplex
    STALL_MANUALFEEDJOB,   ///< prompt user to load paper into manual feed tray
    STALL_CLEANINGPAGE,    ///< prompt user to load paper for a cleaning page
    STALL_OUTPUT_BIN_FULL, ///< prompt user to clear output tray/bin
    STALL_CANCELJOB,       ///< cancel the job
    STALL_FORCED,          ///< stall forced by engine (for platform errors requiring user prompt)
    STALL_PENDING,         ///< stall is pending so don't do anything else , but not ready yet  
    STALL_FINISHED,        ///< stall is done
    STALL_DRUM_NEAR_END,   ///< drum life almost end (DLPC >= 49000)
}STALLREASON;

typedef struct
{
    //These values are set by the calling function to initial values
    //and modified by the function to reflect final values
    STALLREASON curPageReason;
    mediatype_t curPageType;
    mediasize_t curPageSize;
    trayenum_t curPageTray;
    trayenum_t curEmptyTray;
    trayenum_t curPageOutTray;
    uint8_t pageState;

    //These values are set by calling function to customize function
    //they are not modified
    trayenum_t manualTray;    ///< tray number of manual feed tray or TRAY_NONE

    trayenum_t manualDuplexTray;
    bool havePriority;      ///< true if manual feed is also a priority tray
    bool isMPTray;          ///< true if manual feed tray is also a multi-purpose
    bool useAnyAvailable;   ///< set to true if want to use any tray with paper
}STALLCHECKDATA;

/**
 * \brief Structure for best match search.
 * 
 * This is used when we are doing auto selection trying to find a tray with the
 * best paper size and type. 
 */
typedef struct
{
    trayenum_t bestNonEmpty;     ///< enum of tray with paper that best matches
    trayenum_t bestEmpty;        ///< enum of empty tray that best matches
    uint8_t matchTypeNonEmpty;   ///< Uses the HaveType, HaveSize etc defines
    uint8_t matchTypeEmpty;      ///< Uses the HaveType, HaveSize etc defines
}BESTMATCH;

/**
 * \brief Structure that holds the information on the last
 *        page we sent to the printer.
 *
 * This may not be the current page printing, depending on the number of
 * credits.
 */
typedef struct
{
    mediatype_t type;
    mediasize_t size;
    trayenum_t tray;
    trayenum_t outTray;
    bool isManualFeed;
    bool isSingleFeed;
}LASTPAGEINFO;

typedef struct
{
	int DrumLifePageCount;
	int DrumLifeRemainPercentage;
	int MotorRunTime;
	int DrumPrintedPageCount;
	int FreeMotorRunCount;
}DRUMINFO;

typedef struct
{
	int FuserLifePageCount;		// DB: FuserLifePageCount from INST_FUSERCOUNT
	int FuserPrintedPageCount;	// DB: Count
	int MotorRunNormal;			// DB: MotorNormal
	int MotorRunLow;			// DB: MotorLow
	int ResetActionCount;		// DB: ResetAction
}FUSERINFO;

typedef struct
{
	int job_mode_1;		// 1 page Job
	int job_mode_2;		// 2 pages Job
	int job_mode_3;		// 3 pages Job
	int job_mode_4;		// 4 pages Job
	int job_mode_5;		// 5 pages Job
	int job_mode_6;		// 6  ~ 10 pages Job
	int job_mode_7;		// 11 ~ 20 pages Job
	int job_mode_8;		// 21 ~ 50 pages Job
	int job_mode_9;		// 51 ~    pages Job
	int job_mode_total;	// Total Job
}JOBBYMODE;

typedef struct
{
	int forceDuplexJob;
	int topMargin;
	int leftMargin;
	int trayNo;
	int backTopMargin;
	int backLeftMargin;
    int beamAdjust;
    int verticalMagnification;
    int horizontalMagnification;
	int copyLeftMargin;
	int copyBackLeftMargin;
	int copyTopMargin;
	int copyBackTopMargin;
	int servoadjustvalue;
    
    DRUMINFO drumInfo;
    JOBBYMODE DRUM;
    JOBBYMODE MACHINE;
    
    COUNTER_STRUCT machineCount;
 
	int A4LTGapMargin;
	FUSERINFO fuserInfo;
}DBINFO;

#define TWO_OPTIONTRAY_DETACTED		0
#define ONE_OPTIONTRAY_DETACTED		1
#define NO_OPTIONTRAY_DETACTED		2

bool CheckTray(trayenum_t tray, const printer_setup_t* pSetup, const mediacnfg_t* pCnfg, mediasize_t size, mediatype_t type, 
               BESTMATCH* bestMatch,mediasize_t findSize, mediatype_t findType, uint8_t matchType );
uint32_t PrintMgrGetSizeAndType(const mediacnfg_t* pCnfg, trayenum_t tray, uint8_t pageState, 
                              mediasize_t* sizeOut, mediatype_t* typeOut);
bool PrintMgrCheckForSupportedSize(const INPUTTRAY* pTray, mediasize_t size);
bool PrintMgrCheckForSupportedType(const INPUTTRAY* pTray, mediatype_t type);
bool MediaSizeAndTypeFitTrayConfiguration(mediasize_t inMediaSize, mediatype_t inMediaType, 
                                          trayenum_t inTrayEnum, const mediacnfg_t* pMediaCnfg);
trayenum_t InputTrayIteratorGetFirst(void);
trayenum_t InputTrayIteratorGetNext(trayenum_t inputTrayEnum, trayenum_t inputStartTrayEnum);
trayenum_t GetInputUpper(const printer_setup_t* pSetup );

#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)
trayenum_t PrintMgrGetLastPreviousNonDuplexInputTRAYENUM(LASTPAGEINFO *lastPage);
void PrintMgrSetLastPreviousNonDuplexInputTRAYENUM(trayenum_t trayEnum);
#else
#define PrintMgrGetLastPreviousNonDuplexInputTRAYENUM(lastPage) lastPage->tray
#define PrintMgrSetLastPreviousNonDuplexInputTRAYENUM(trayEnum)
#endif

#ifdef __cplusplus
}
#endif
#endif //PrintMgr_Priv_h
