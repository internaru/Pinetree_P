/*
 *
 * ============================================================================
 * Copyright (c) 2006-2010, 2012   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file print_status_codes.h 
 *
 * \brief The status codes for the print system are defined in this file
 *
 * The following information is encoded in the UINT32:
 * -# Offline - Should the printer go offline on this status.
 * -# Severity - See severity below.
 * -# Priority - Priority within a given severity so can order by importance
 * -# Cancel - If this status occurs current pages in the engine needs to be
 * cancelled. Ifused with the restart bit below the page will cancel and
 * restart, else it will justbe cancelled.
 * -# UserInput - If set user input is needed to continue. User input consists
 *  of things such as adding paper, pushing a front panel button, selecting
 *  continue from a dialog on a PC.
 * -# Type - Status type - device, job, page (not currently used, can remove
 *  if never needed)
 * -# Restart - Used in conjunction with the cancel bit, when set it means
 *  cancel the page and restart it.
 * -# UserAck - This bit is used in conjuction with the UserInput bit. If set
 *  the user must acknowledge the error through the front panel or PC dialog.
 *  Just fixing the problem won't clear the error. (set by engines for benefit
 *  of print mgr)
 * -# PreError - This bit is used with the UserInput bit. If set it means this
 *  status was caused by a print error. The error is not reported immediatly, 
 *  it will only show up if the next page to print will have the exact same
 *  problem based on the tray, size and type.  Mostly used for unexpected size
 *  errors.
 * -# JobCancel - If set the current job needs to be cancelled.
 * -# Category - A general category of the error, such as paper jam.
 * -# Code - The specific status within the category.
 *
 * UserInput, UserAck, PreError, Cancel and Restart are all designed to be used
 * for status updates while printing or trying to print. UserInput bit makes
 * sense for things like paper jams, punts, misprints. They all convey we were
 * trying to print and something happened to mess it up. They don't make sense
 * for things like fan malfunction or fuser error etc.
 *
 **/

#ifndef _STATUS_H_
#define _STATUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define STATUS_OFFLINE_MASK     0x80000000   ///< Bit  31   : if set printer goes offline for this status
#define STATUS_SEVERITY_MASK    0x70000000   ///< Bits 30-28: 8 possible severities
#define STATUS_PRIORITY_MASK    0x0C000000   ///< Bits 27-26: 4 possible priorities for each severity
#define STATUS_CANCEL_MASK      0x02000000   ///< Bit  25   : if set pages will have to be restarted after this status is fixed
#define STATUS_USERINPUT_MASK   0x01000000   ///< Bit  24   : User input needed to continue (but not required)
#define STATUS_TYPE_MASK        0x00E00000   ///< Bits 23-21: device, job, page, other (can be OR'd)
#define STATUS_RESTART_MASK     0x00100000   ///< Bit  20  :used with cancel mask, if set restart the page after cancel
#define STATUS_USERACK_MASK     0x00080000   ///< Bit 19: user ack is required (must also set USERINPUT_MASK)
#define STATUS_PREERROR_MASK    0x00040000   ///< Bit 18: prewarn see notes above
#define STATUS_NOPJL_MASK       0x00020000   ///< Bit 17: don't report status to PJL (see note 1) 
#define STATUS_CANCELJOB_MASK   0x00010000   ///< Bit 16: cancel the current job 
#define STATUS_CATEGORY_MASK    0x0000FF00   ///< Bits 15-8 : 256 possible categories
#define STATUS_CODEGROUP_MASK   0x000000F0   ///< Bits 7-4  : 16 code groups in each category
#define STATUS_CODE_MASK        0x000000FF   ///< Bits 7-0  : 256 codes in each category
//note1: when this bit is set the status manager won't forward the status to PJL (with MFP devices the
//status may not be PJL. It means the status manager won't call the external status reporting function)

#define STATUS_SEVPRI_MASK    (STATUS_SEVERITY_MASK|STATUS_PRIORITY_MASK)   ///< Mask to get the severity and priority
#define STATUS_CATCODE_MASK   (STATUS_CATEGORY_MASK|STATUS_CODE_MASK)   ///< Mask to get the category and code

    
/**
 * Status severities: 
 * Fatal:     Can't print, requires service (e.g. hardware failure)
 * Critical:  Can't print but should be fixable by user - (e.g. out of paper)
 * Testing:   Can't print, running diagnostics or doing a calibration/alignment
 * Info:      Printing or canceling a job, more important that a warning but 
 * less important than errors
 * Warning:   Can still print - but things are not perfect (e.g. supplies low)
 */
#define STATSEV_FATAL     0x70000000
#define STATSEV_CRITICAL  0x60000000
#define STATSEV_TESTING   0x30000000
#define STATSEV_INFO      0x20000000
#define STATSEV_WARNING   0x10000000
#define STATSEV_NONE      0x00000000  ///< means everything is okay, no warning, no errors

/**
 * Status priorities: allows prioritization among different status of the
 * same severity. Whichever status has the highest severity and highest
 * priority will be displayed first
 */
#define STATPRI_0       0x00000000  ///< Used for online category only so that it is the lowest sev and priority
#define STATPRI_1       0x04000000
#define STATPRI_2       0x08000000
#define STATPRI_3       0x0C000000  ///< Highest priority

/** Status types:  these can be OR'd together if status is more than one type */
#define STATTYPE_DEVICE   0x00800000
#define STATTYPE_JOB      0x00400000
#define STATTYPE_PAGE     0x00200000
#define STATTYPE_OTHER    0x00000000
#define STATTYPE_ALL      0x00E00000

/** Status categories: divides status up by logical groupings. */
#define STATCAT_EMPTY_FINAL	   0x00000010
#define STATCAT_ONLINE         0x00000100
#define STATCAT_PAPERPATH      0x00000200
#define STATCAT_CONNECTION     0x00000700
#define STATCAT_DOOROPEN       0x00000800
#define STATCAT_INKLOW         0x00000900
#define STATCAT_INKOUT         0x00000A00
#define STATCAT_INKWRONG       0x00000B00
#define STATCAT_PAPEROUT       0x00000C00
#define STATCAT_PAPERJAM_TYPE  0x00000D00
#define STATCAT_PAPERJAM_AREA  0x00000E00
#define STATCAT_MISPRINT       0x00000F00
#define STATCAT_CARTRIDGE      0x00001000
#define STATCAT_INPUT          0x00001200
#define STATCAT_INITIALIZING   0x00001300
#define STATCAT_TRAY           0x00001500
#define STATCAT_MEMORY         0x00001700
#define STATCAT_INFO           0x00001800
#define STATCAT_JOBINFO        0x00001900
#define STATCAT_JBIG           0x00001E00
#define STATCAT_HIDDEN         0x00001F00
#define STATCAT_MEMORYERROR    0x00002000
#define STATCAT_ERROR          0x00002200
#define STATCAT_OUTPUT         0x00002900
#define STATCAT_PRINTHEAD      0x00002A00
#define STATCAT_MAINTENANCE    0x00002B00
#define STATCAT_DATA_PATH      0x00002C00
#define STATCAT_WRONGIMAGE	   0x00002D00
#define STATCAT_LAMINATEINFO   0x00002E00

/** Added for oem laser products */
#define STATCAT_TONERLOW       STATCAT_INKLOW
#define STATCAT_TONEROUT       STATCAT_INKOUT
#define STATCAT_TONERWRONG     STATCAT_INKWRONG

/** DEFINES FOR STATUS */
#define STATUS_OFFLINE         STATUS_OFFLINE_MASK

//online variable
#define ONLINE 1
#define OFFLINE 0

//Power-on datastore status value until the status manager changes it 
#define STATUS_INVALID 0

/******** USE THESE MACROS INSTEAD OF ACCESSING THE STATUS BITS DIRECTLY *****************/
/** Test to see if this is an online status message */
#define ISONLINE(status) ((status&STATUS_CATEGORY_MASK) == STATCAT_ONLINE)
/** Test to see if this status requires the system to go offline */
#define ISOFFLINE(status) ((status&STATUS_OFFLINE) == STATUS_OFFLINE)
/** Test severity */
#define ISCRITICAL(status) ((status&STATUS_SEVERITY_MASK) == STATSEV_CRITICAL)
#define ISWARNING(status) ((status&STATUS_SEVERITY_MASK) == STATSEV_WARNING)
#define ISFATAL(status)    ((status&STATUS_SEVERITY_MASK) == STATSEV_FATAL)
/** Need to cancel page? */
#define ISCANCEL(status) ((status&STATUS_CANCEL_MASK) == STATUS_CANCEL_MASK)
/** need to restart after cancel page? */
#define ISRESTART(status) ((status&STATUS_RESTART_MASK) == STATUS_RESTART_MASK)
/** User input required */
#define ISUSERINPUT(status) ((status&STATUS_USERINPUT_MASK) == STATUS_USERINPUT_MASK)
/** User must acknowledge */
#define ISUSERACK(status) ((status&STATUS_USERACK_MASK) == STATUS_USERACK_MASK)
#define ISPREERROR(status) ((status&STATUS_PREERROR_MASK) == STATUS_PREERROR_MASK)
/** Report status on this event */
#define ISNOPJL(status) ((status&STATUS_NOPJL_MASK) == STATUS_NOPJL_MASK)
/** Need to cancel the job */
#define ISCANCELJOB(status) ((status&STATUS_CANCELJOB_MASK) == STATUS_CANCELJOB_MASK)
#define ISEMPTYFINAL(status)	((status&STATCAT_EMPTY_FINAL) == STATCAT_EMPTY_FINAL)

/** Return 1 if stat1 > stat2, else return 0 */
#define STATSEVCMP(stat1, stat2) (((stat1&STATUS_SEVPRI_MASK) > (stat2&STATUS_SEVPRI_MASK))?1:0)
/** Return 1 if stat1 >= stat2, else return 0 */
#define STATSEVGTECMP(stat1, stat2) (((stat1&STATUS_SEVPRI_MASK) >= (stat2&STATUS_SEVPRI_MASK))?1:0)
/** Compare stat1 and stat2, return stat1 if it is greater else return stat1 */
#define STATGETWORSE(stat1, stat2) (((stat1&STATUS_SEVPRI_MASK) > (stat2&STATUS_SEVPRI_MASK))?stat1:stat2)
/** Return 1 if categories match, else return 0 */
#define STATCATCMP(stat1, stat2)  (((stat1&STATUS_CATEGORY_MASK) == (stat2&STATUS_CATEGORY_MASK))?1:0)
/** Return 1 if code groups match, else return 0 */
#define STATCODEGRPCMP(stat1, stat2)  (((stat1&STATUS_CODEGROUP_MASK) == (stat2&STATUS_CODEGROUP_MASK))?1:0)
/** Return 1 if codes match, else return 0 */
#define STATCODECMP(stat1, stat2) (((stat1&STATUS_CODE_MASK) == (stat2&STATUS_CODE_MASK))?1:0)
/** Return 1 if category and code are equal, else return 0 */
#define STATCATCODECMP(stat1, stat2) (((stat1&STATUS_CATCODE_MASK) == (stat2&STATUS_CATCODE_MASK))?1:0)
/** Return the code portion of the status word */
#define STATGETCODE(stat) (stat&STATUS_CODE_MASK)
/** Return the category portion of the status word */
#define STATGETCAT(stat) (stat&STATUS_CATEGORY_MASK)
/** Return the category and code portion of the status word */
#define STATGETCATCODE(stat) (stat&STATUS_CATCODE_MASK)

#define GETCATOFFSET(status) ((status & STATUS_CATEGORY_MASK) >> 8)

#define SETUSERINPUT(status) (status|=STATUS_USERINPUT_MASK)
#define SETMODULE(status,module) (status|=module)
#define SETCANCEL(status) (status|=STATUS_CANCEL_MASK)
#define SETRESTART(status) (status|=STATUS_RESTART_MASK)
#define SETUSERACK(status) (status|=STATUS_USERACK_MASK)
#define SETPREERROR(status)  (status|=STATUS_PREERROR_MASK)
#define SETNOPJL(status)  (status|=STATUS_NOPJL_MASK)
#define SETCANCELJOB(status) (status|=STATUS_CANCELJOB_MASK)
#define SETPRIORITY(priority, statusCode) ((statusCode & ~STATUS_PRIORITY_MASK)|priority)
#define SETSEVERITY(severity, statusCode) ((statusCode & ~STATUS_SEVERITY_MASK) | severity)
#define SETONLINE(statusCode) (statusCode & ~STATUS_OFFLINE_MASK)
#define SETOFFLINE(statusCode) (statusCode | STATUS_OFFLINE_MASK)

#define CLEARCANCEL(status) (status&=~STATUS_CANCEL_MASK)
#define CLEARRESTART(status) (status&=~STATUS_RESTART_MASK)
#define CLEARUSERACK(status) (status&=~STATUS_USERACK_MASK)
#define CLEARPREERROR(status) (status&=~STATUS_PREERROR_MASK)
#define CLEARCANCELJOB(status) (status&=~STATUS_CANCELJOB_MASK)


// ----------------------- C A T A G O R Y  D E F I N E S -----------------------------
#define COMMON_FATAL     (STATUS_OFFLINE_MASK|STATSEV_FATAL|STATPRI_1|STATUS_CANCEL_MASK)
#define COMMON_CRITICAL_2  (STATUS_OFFLINE_MASK|STATSEV_CRITICAL|STATPRI_2|STATUS_CANCEL_MASK)
#define COMMON_CRITICAL_1  (STATUS_OFFLINE_MASK|STATSEV_CRITICAL|STATPRI_1|STATUS_CANCEL_MASK)
#define COMMON_WARNING   (STATSEV_WARNING|STATPRI_1)

//Online is a bit different for a category. Instead of having individual codes we OR in the
//module it is being generated by
#define STATUS_ONLINE                         (STATSEV_NONE|STATPRI_1|STATCAT_ONLINE|STATTYPE_DEVICE) 
                                                    
//#define STATUS_PAPERPATH_ERROR                      (COMMON_FATAL|STATCAT_PAPERPATH)
//#define STATUS_PAPERPATH_ERROR                     (COMMON_FATAL|STATCAT_PAPERPATH|STATUS_RESTART_MASK|STATUS_USERINPUT_MASK)
#define STATUS_PAPERPATH_ERROR                     (COMMON_FATAL|STATCAT_PAPERPATH|STATUS_RESTART_MASK)
/* For now define STATUS_MAINTENANCE_JAM to be the same as STATUS_PAPERJAM.
 * This might need to be updated if some other kind of user-interface
 * interaction is needed */
#define STATUS_MAINTENANCE_JAM                     (COMMON_CRITICAL_1 | STATCAT_PAPERJAM_TYPE |STATUS_RESTART_MASK|0x00000002)
#define STATUS_MAINTENANCE_BUSY                    (COMMON_CRITICAL_1|STATUS_RESTART_MASK|STATCAT_MAINTENANCE|0x00000002)
#define STATUS_CONNECTION_FAIL                     (COMMON_CRITICAL_2|STATCAT_CONNECTION)
#define STATUS_CONNECTION_DUPLEXOR_FAIL            (STATUS_CONNECTION_FAIL|0x00000001)
#define STATUS_CONNECTION_READ_FAIL                (STATUS_CONNECTION_FAIL|STATUS_CANCELJOB_MASK|0x00000002)

/** Door open category */
#define STATUS_DOOROPEN                            (COMMON_CRITICAL_1|STATUS_RESTART_MASK|STATCAT_DOOROPEN)
#define STATUS_DOOROPEN_FRONT                      (STATUS_DOOROPEN|0x00000001)		// 0xE6100801
#define STATUS_DOOROPEN_FD                         (STATUS_DOOROPEN|0x00000002) 	// This is the rear door
#define STATUS_DOORCLOSE_FRONT					   (STATUS_DOOROPEN|0x00000011)		// 0xE6100811


/**
 * \brief PAPER OUT  category
 * When paper out detected *while* printing, send one of the TRAYx messages
 * not STATUS_PAPER_OUT_ALL. It is sent when all trays are empty. It will not
 * cause the current job to cancel. 
 */
#define STATUS_PAPER_OUT                           (COMMON_CRITICAL_1|STATUS_RESTART_MASK|STATCAT_PAPEROUT)
#define STATUS_PAPER_OUT_TRAY1                     (STATUS_PAPER_OUT|0x00000001) ///< Ran out on tray 1 while printing
#define STATUS_PAPER_OUT_TRAY2                     (STATUS_PAPER_OUT|0x00000002) //< / ""        tray 2
#define STATUS_PAPER_OUT_TRAY3                     (STATUS_PAPER_OUT|0x00000003) //< / ""        tray 3
#define STATUS_PAPER_OUT_TRAY4                     (STATUS_PAPER_OUT|0x00000004) //< / ""        tray 4      
#define STATUS_PAPER_OUT_TRAY1_FINAL               (STATUS_PAPER_OUT|0x00000011) ///< Ran out on tray 1 while printing
#define STATUS_PAPER_OUT_TRAY2_FINAL               (STATUS_PAPER_OUT|0x00000012) //< / ""        tray 2
#define STATUS_PAPER_OUT_TRAY3_FINAL               (STATUS_PAPER_OUT|0x00000013) //< / ""        tray 3
#define STATUS_PAPER_OUT_TRAY4_FINAL               (STATUS_PAPER_OUT|0x00000014) //< / ""        tray 4                                                                            
#define STATUS_PAPER_OUT_TRAYS_UNSTABLE            (STATUS_PAPER_OUT|0x00000020) //< / ""        trays are unstable

/**
 * \brief STATUS_PAPER_OUT_ALL is a warning. It means there is no paper in any
 * tray.
 *
 * It doesn't become an error until the user tries to print from a given tray,
 * then paperout error is reported for that tray. Do not have the printer 
 * go offline for "paper_out_all" if we aren't printing. Because if we go 
 * offline our print manager won't try to print and do the tray select logic 
 * associated with the print page.  If we stay online, the page manager will 
 * do the tray select logic and can prompt the user to load the correct paper 
 * in the correct tray.
 */

/** No paper in any tray */
#define STATUS_PAPER_OUT_ALL                       (STATSEV_WARNING|STATPRI_1|STATCAT_PAPEROUT|0x00000004) 


/** Misprint category */
#define STATUS_MISPRINT                            (COMMON_CRITICAL_1|STATCAT_MISPRINT)
#define STATUS_MISPRINT_RESERVATION                (STATUS_MISPRINT|0x00000001)
#define STATUS_MISPRINT_BD                         (STATUS_MISPRINT|0x00000003)
#define STATUS_MISPRINT_PAPER_SIZE                 (STATUS_MISPRINT|0x00000004)
#define STATUS_MISPRINT_SENDING_ERROR              (STATUS_MISPRINT|0x00000006)
#define STATUS_MISPRINT_MISPICK                    (STATUS_MISPRINT|0x00000007)


/** Paper Jam Category */
#define JAM_CODEGROUP_INPUT                        0x00000080
#define JAM_CODEGROUP_OUTPUT                       0x00000040

#define STATUS_PAPERJAM                            (COMMON_CRITICAL_1 | STATCAT_PAPERJAM_TYPE |STATUS_RESTART_MASK)

#define STATUS_PAPER_JAM_INPUT                     (STATUS_PAPERJAM|JAM_CODEGROUP_INPUT|0x00000000)
#define STATUS_PAPER_JAM_TRAY1                     (STATUS_PAPERJAM|JAM_CODEGROUP_INPUT|0x00000001)
#define STATUS_PAPER_JAM_TRAY2                     (STATUS_PAPERJAM|JAM_CODEGROUP_INPUT|0x00000002)
#define STATUS_PAPER_JAM_TRAY3                     (STATUS_PAPERJAM|JAM_CODEGROUP_INPUT|0x00000003)
#define STATUS_PAPER_JAM_TRAY4                     (STATUS_PAPERJAM|JAM_CODEGROUP_INPUT|0x00000004)

#define STATUS_PAPER_JAM_OUTPUT                    (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000001)
#define STATUS_PAPER_JAM_EXTSEN_NOT_DET            (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000004)
#define STATUS_PAPER_JAM_INPSEN_NOT_REL            (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000005)
#define STATUS_PAPER_JAM_EXTSEN_NOT_REL            (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000006)
#define STATUS_PAPER_JAM_DUPLEX			           (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000007)
#define STATUS_PAPER_JAM_FULLSTACK				   (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000010)
#define STATUS_PAPER_JAM_REINSERTCARTRIDGE		   (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000020)		// 0xE6100D60
#define STATUS_PAPER_JAM_TONER_BLOCK			   (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000021) /* [KSR071102] */	// 0xE6100D61
#define STATUS_PAPER_JAM_ILLEGAL_CARTRIDGE		   (STATUS_PAPERJAM|JAM_CODEGROUP_OUTPUT|0x00000022)		// 0xE6100D62

/* TODO: How shall we define this? */
#define STATUS_PAPER_JAM_DOOR_OPEN                 (STATUS_PAPERJAM|STATCAT_DOOROPEN|0x00000010)

/** Initialization category */
#define STATUS_INIT                          (COMMON_CRITICAL_1|STATUS_RESTART_MASK|STATCAT_INITIALIZING) 
#define STATUS_INITIALIZING                  (STATUS_INIT | 0x00000000)
#define STATUS_VERIFYING                     (STATUS_INIT | 0x00000001)

/** Input tray category */
#define STATUS_INPUT_ERROR             (STATUS_USERINPUT_MASK|STATUS_OFFLINE_MASK|STATSEV_CRITICAL|STATPRI_1|STATCAT_INPUT)
#define STATUS_INPUT_LOAD_TRAY         (STATUS_INPUT_ERROR|0x00000001)
#define STATUS_INPUT_RECONFIGURE_TRAY  (STATUS_INPUT_ERROR|0x00000002)
#define STATUS_INPUT_MANUAL_FEED       (STATUS_INPUT_ERROR|0x00000003)
#define STATUS_INPUT_MANUAL_FEED_NO_MEDIA_AVAILABLE (STATUS_INPUT_ERROR|0x00000004)
#define STATUS_INPUT_PAPER_MISPICK     (STATUS_INPUT_ERROR|0x00000005)
#define STATUS_INPUT_MISPRINT          (STATUS_INPUT_ERROR|0x00000006)
#define STATUS_INPUT_LOAD_TRAY_1       (STATUS_INPUT_ERROR|0x00000011)
#define STATUS_INPUT_LOAD_TRAY_2       (STATUS_INPUT_ERROR|0x00000012)
#define STATUS_INPUT_LOAD_TRAY_3       (STATUS_INPUT_ERROR|0x00000013)
#define STATUS_INPUT_LOAD_TRAY_4       (STATUS_INPUT_ERROR|0x00000014)
#define STATUS_INPUT_CLEANING_PAGE     (STATUS_INPUT_ERROR|0x0000001F)
#define STATUS_INPUT_MANUAL_DUPLEX          (STATUS_INPUT_ERROR|0x00000020)  // NOTE: this should be obsoleted in favor of the tray specific defines below
#define STATUS_INPUT_MANUAL_DUPLEX_TRAY_1   (STATUS_INPUT_ERROR|0x00000021)
#define STATUS_INPUT_MANUAL_DUPLEX_TRAY_2   (STATUS_INPUT_ERROR|0x00000022)
#define STATUS_INPUT_MANUAL_DUPLEX_TRAY_3   (STATUS_INPUT_ERROR|0x00000023)
#define STATUS_INPUT_MANUAL_DUPLEX_TRAY_4   (STATUS_INPUT_ERROR|0x00000024)
#define STATUS_INPUT_LOAD_TRAY_1_NO_MEDIA_AVAILABLE (STATUS_INPUT_ERROR|0x00000031)
#define STATUS_INPUT_LOAD_TRAY_2_NO_MEDIA_AVAILABLE (STATUS_INPUT_ERROR|0x00000032)
#define STATUS_INPUT_LOAD_TRAY_3_NO_MEDIA_AVAILABLE (STATUS_INPUT_ERROR|0x00000033)
#define STATUS_INPUT_LOAD_TRAY_4_NO_MEDIA_AVAILABLE (STATUS_INPUT_ERROR|0x00000034)
#define STATUS_INPUT_TRAY_SIZE_MISMATCH 	(STATUS_INPUT_ERROR|0x00000041)

/** Data Path Category */
#define STATUS_DATA_PATH               (COMMON_CRITICAL_1|STATTYPE_ALL|STATCAT_DATA_PATH)
#define STATUS_DATA_PATH_UNDERRUN      (STATUS_DATA_PATH|0x00000001)

/** Tray category */
#define STATUS_TRAY                     (STATSEV_WARNING|STATPRI_2|STATCAT_TRAY)

#define STATUS_TRAY1_OPEN				(STATUS_TRAY|0x00000011) //tray1
#define STATUS_TRAY1_CLOSE				(STATUS_TRAY|0x00000012)
#define STATUS_TRAY1_MEDIA_LETTER		(STATUS_TRAY|0x00000013)
#define STATUS_TRAY1_MEDIA_EXEC			(STATUS_TRAY|0x00000014)
#define STATUS_TRAY1_MEDIA_A4			(STATUS_TRAY|0x00000015)
#define STATUS_TRAY1_MEDIA_B5			(STATUS_TRAY|0x00000016)
#define STATUS_TRAY1_MEDIA_LEGAL		(STATUS_TRAY|0x00000017)
#define STATUS_TRAY1_MEDIA_A5			(STATUS_TRAY|0x00000018)
#define STATUS_TRAY1_MEDIA_UNKNOWN		(STATUS_TRAY|0x00000019)
#define STATUS_TRAY1_PAPER_LOW			(STATUS_TRAY|0x0000001A)
#define STATUS_TRAY1_PAPER_NORMAL		(STATUS_TRAY|0x0000001B)
#define STATUS_TRAY1_ATTACHED			(STATUS_TRAY|0x0000001E)
#define STATUS_TRAY1_NOT_ATTACHED		(STATUS_TRAY|0x0000001F)

#define STATUS_TRAY2_OPEN				(STATUS_TRAY|0x00000021) //tray2
#define STATUS_TRAY2_CLOSE				(STATUS_TRAY|0x00000022)
#define STATUS_TRAY2_MEDIA_LETTER		(STATUS_TRAY|0x00000023)
#define STATUS_TRAY2_MEDIA_EXEC			(STATUS_TRAY|0x00000024)
#define STATUS_TRAY2_MEDIA_A4			(STATUS_TRAY|0x00000025)
#define STATUS_TRAY2_MEDIA_B5			(STATUS_TRAY|0x00000026)
#define STATUS_TRAY2_MEDIA_LEGAL		(STATUS_TRAY|0x00000027)
#define STATUS_TRAY2_MEDIA_A5			(STATUS_TRAY|0x00000028)
#define STATUS_TRAY2_MEDIA_UNKNOWN		(STATUS_TRAY|0x00000029)
#define STATUS_TRAY2_PAPER_LOW			(STATUS_TRAY|0x0000002A)
#define STATUS_TRAY2_PAPER_NORMAL		(STATUS_TRAY|0x0000002B)
#define STATUS_TRAY2_ATTACHED			(STATUS_TRAY|0x0000002E)
#define STATUS_TRAY2_NOT_ATTACHED		(STATUS_TRAY|0x0000002F)

#define STATUS_TRAY3_OPEN				(STATUS_TRAY|0x00000031) //tray3
#define STATUS_TRAY3_CLOSE				(STATUS_TRAY|0x00000032)
#define STATUS_TRAY3_MEDIA_LETTER		(STATUS_TRAY|0x00000033)
#define STATUS_TRAY3_MEDIA_EXEC			(STATUS_TRAY|0x00000034)
#define STATUS_TRAY3_MEDIA_A4			(STATUS_TRAY|0x00000035)
#define STATUS_TRAY3_MEDIA_B5			(STATUS_TRAY|0x00000036)
#define STATUS_TRAY3_MEDIA_LEGAL		(STATUS_TRAY|0x00000037)
#define STATUS_TRAY3_MEDIA_A5			(STATUS_TRAY|0x00000038)
#define STATUS_TRAY3_MEDIA_UNKNOWN		(STATUS_TRAY|0x00000039)
#define STATUS_TRAY3_PAPER_LOW			(STATUS_TRAY|0x0000003A)
#define STATUS_TRAY3_PAPER_NORMAL		(STATUS_TRAY|0x0000003B)
#define STATUS_TRAY3_ATTACHED			(STATUS_TRAY|0x0000003E)
#define STATUS_TRAY3_NOT_ATTACHED		(STATUS_TRAY|0x0000003F)

#define STATUS_TRAY4_OPEN				(STATUS_TRAY|0x00000041) //mpt
#define STATUS_TRAY4_CLOSE				(STATUS_TRAY|0x00000042)
#define STATUS_TRAY4_MEDIA_LETTER		(STATUS_TRAY|0x00000043)
#define STATUS_TRAY4_MEDIA_EXEC			(STATUS_TRAY|0x00000044)
#define STATUS_TRAY4_MEDIA_A4			(STATUS_TRAY|0x00000045)
#define STATUS_TRAY4_MEDIA_B5			(STATUS_TRAY|0x00000046)
#define STATUS_TRAY4_MEDIA_LEGAL		(STATUS_TRAY|0x00000047)
#define STATUS_TRAY4_MEDIA_A5			(STATUS_TRAY|0x00000048)
#define STATUS_TRAY4_MEDIA_UNKNOWN		(STATUS_TRAY|0x00000049)
#define STATUS_TRAY4_PAPER_LOW			(STATUS_TRAY|0x0000004A)
#define STATUS_TRAY4_PAPER_NORMAL		(STATUS_TRAY|0x0000004B)
#define STATUS_TRAY4_ATTACHED			(STATUS_TRAY|0x0000004E)
#define STATUS_TRAY4_NOT_ATTACHED		(STATUS_TRAY|0x0000004F)

#define STATUS_OUTTRAY_FULL_CLEAR		(STATUS_TRAY|0x00000090)

//#define STATUS_TRAY1_OPEN               (STATUS_TRAY|1)
//#define STATUS_TRAY2_OPEN               (STATUS_TRAY|2)

#define STATUS_WRONG_IMAGE_SIZE			(STATSEV_INFO|STATPRI_2|STATCAT_WRONGIMAGE)
#define STATUS_WRONG_IMAGE_SIZE_CANCEL_REQ	(STATUS_WRONG_IMAGE_SIZE|0x00000001)

/** Memory category */
#define STATUS_MEMORY                   (STATSEV_WARNING|STATPRI_1|STATCAT_MEMORY)
#define STATUS_MEMORY_JOB_MEM_LOW       (STATUS_MEMORY|0x00000001)
#define STATUS_PDL_MEMORYOUT            (STATUS_MEMORY|0x00000002)

//info codes for processing and canceling
//!!!keep all printing infos grouped and all canceling infos grouped
//These codes are informational. The system is online and is doing something
/** These codes tell us what we are doing */
#define STATUS_INFO                     (STATSEV_INFO|STATCAT_INFO)
#define STATUS_INFO_PRINTING            (STATUS_INFO|0x00000001) ///< Printing a job
#define STATUS_INFO_PRINTING_INTERNAL   (STATUS_INFO|0x00000002)
#define STATUS_INFO_PRINTING_DEMO       (STATUS_INFO|0x00000003)
#define STATUS_INFO_PRINTING_CNFG       (STATUS_INFO|0x00000004)
#define STATUS_INFO_PRINTING_MENU       (STATUS_INFO|0x00000005)
#define STATUS_INFO_PRINTING_SUPPLIES   (STATUS_INFO|0x00000006)
#define STATUS_INFO_PRINTING_NETWORK    (STATUS_INFO|0x00000007)
#define STATUS_INFO_PRINTING_USAGE      (STATUS_INFO|0x00000008)
#define STATUS_INFO_PRINTING_PSFONT     (STATUS_INFO|0x0000000b)
#define STATUS_INFO_PRINTING_SERVICE    (STATUS_INFO|0x0000000c)
#define STATUS_INFO_CLEANING            (STATUS_INFO|0x0000000d) ///< Internal cleaning page
#define STATUS_INFO_CALIBRATING         (STATUS_INFO|0x0000000e) ///< Calibration
#define STATUS_INFO_BEGIN_MANUALDUPLEX  (STATUS_INFO|0x0000000f) ///< Beginning manual duplex, info for the host
#define STATUS_INFO_DOWNLOADING         (STATUS_INFO|0x00000010) ///< For Blinking LED's while code downloading
#define STATUS_INFO_COPYING             (STATUS_INFO|0x00000011) ///< Doing a copy job
#define STATUS_INFO_SCANNING_PC         (STATUS_INFO|0x00000012) ///< Doing a scan job
#define STATUS_INFO_PRINTING_ERROR      (STATUS_INFO|0x00000013) ///< Printing error report
#define STATUS_INFO_CONT_SELF_TEST      (STATUS_INFO|0x00000014) ///< Cont. self test
#define STATUS_INFO_CONT_COPY_TEST      (STATUS_INFO|0x00000015) ///< Cont. copy test
#define STATUS_INFO_PRINTING_CAL_GRAPH  (STATUS_INFO|0x00000016) ///< Printing calibration
#define STATUS_INFO_COOLDOWN            (STATUS_INFO|0x00000017|STATPRI_1) ///< Engine is in cooldown mode; printing is slowed down

#define STATUS_INFO_PRINTING_FAX_ACTREP (STATUS_INFO|0x00000018)   ///< Printing fax activity log
#define STATUS_INFO_PRINTING_FAX_CONFIRMRPT  (STATUS_INFO|0x00000019)  ///< Printing confirmation report
#define STATUS_INFO_PRINTING_FAX_ERRORRPT    (STATUS_INFO|0x0000001a)  ///< Printing error report
#define STATUS_INFO_PRINTING_FAX_LASTCALLRPT (STATUS_INFO|0x0000001b)  ///< Printing last call report
#define STATUS_INFO_PRINTING_FAX_PHNBOOK (STATUS_INFO|0x0000001c)  ///< Printing fax phone book
#define STATUS_INFO_PRINTING_FAX_BLKLIST (STATUS_INFO|0x0000001d)  ///< Printing fax block list
#define STATUS_INFO_PRINTING_FAX_T30TRACE (STATUS_INFO|0x0000001e) ///< Printing dax T.30 trace
#define STATUS_INFO_PRINTING_FAX_BILLING  (STATUS_INFO|0x0000001f) ///< Printing fax billing report
#define STATUS_INFO_PRINTING_FAX_SELFTEST (STATUS_INFO|0x00000020) ///< Printing dax T.30 trace
#define STATUS_INFO_PRINTING_FAX          (STATUS_INFO|0x00000021) ///< Printing fax 

#define STATUS_LAMINATEINFO                     (STATSEV_INFO|STATCAT_LAMINATEINFO)
#define STATUS_LAMINATEINFO_PREHEATING          (STATUS_LAMINATEINFO|0x00000001)
#define STATUS_LAMINATEINFO_PREHEATDONE         (STATUS_LAMINATEINFO|0x00000002)
#define STATUS_LAMINATEINFO_LAMINATING          (STATUS_LAMINATEINFO|0x00000003)
#define STATUS_LAMINATEINFO_LAMINATINGDONE      (STATUS_LAMINATEINFO|0x00000004)
#define STATUS_LAMINATEINFO_LAMIJOBDONE         (STATUS_LAMINATEINFO|0x00000005)
#define STATUS_LAMINATEINFO_EXITGUIDE_CLEAR 	(STATUS_LAMINATEINFO|0x00000006)
#define STATUS_LAMINATEINFO_EXITGUIDE_SET 		(STATUS_LAMINATEINFO|0x00000007)
#define STATUS_LAMINATEINFO_HEATING 			(STATUS_LAMINATEINFO|0x00000008)
#define STATUS_LAMINATEINFO_LAMINATION_STOPPED	(STATUS_LAMINATEINFO|0x00000009)
#define STATUS_LAMINATEINFO_JAM_INPUT_DETECT	(STATUS_LAMINATEINFO|0x0000000a)
#define STATUS_LAMINATEINFO_JAM_INPUT_RELEASE	(STATUS_LAMINATEINFO|0x0000000b)
#define STATUS_LAMINATEINFO_FRONTCOVER_OPEN		(STATUS_LAMINATEINFO|0x0000000c)
#define STATUS_LAMINATEINFO_FRONTCOVER_CLOSE	(STATUS_LAMINATEINFO|0x0000000d)
#define STATUS_LAMINATEINFO_CARTRIDGE_SET		(STATUS_LAMINATEINFO|0x0000000e)
#define STATUS_LAMINATEINFO_CARTRIDGE_CLEAR		(STATUS_LAMINATEINFO|0x0000000f)

//Start of canceling infos 
#define STATUS_INFO_CANCELING           (STATUS_INFO|STATPRI_3|0x00000060) 
#define STATUS_INFO_CANCELING_REPORT    (STATUS_INFO|STATPRI_3|0x00000061) ///< Canceling one of the internal pages
#define STATUS_INFO_CANCELING_GENERAL   (STATUS_INFO|STATPRI_3|0x00000062)

#define STATUS_INFO_CANCELING_COPY      (STATUS_INFO|STATPRI_3|0x00000063)

#define STATUS_INFO_WIRELESS_TEST_PAGE  (STATUS_INFO|0x00000064)
//use this to destinguish printing from canceling
//#define STATUS_INFO_CANCELING_MASK      0x00000040  

// JBIG Category
#define STATUS_JBIG                   (STATUS_OFFLINE_MASK|STATUS_CANCEL_MASK|STATSEV_CRITICAL|STATPRI_3|STATTYPE_ALL|STATCAT_JBIG)
#define STATUS_JBIGERROR                (STATUS_JBIG|1)
#define STATUS_JBIG_READ_DMA_ERROR      (STATUS_JBIG|0x2)
#define STATUS_JBIG_WRITE_DMA_ERROR     (STATUS_JBIG|0x4)

/**
 * The JOBINFO category of errors are for pass through event information. They are used
 * to let the system know when certain milestones in processing jobs have been hit. For
 * example start job, end page, page cancelled end job. They don't cause our status to change
 * they cause status messages to be sent to the outside world (thorugh PJL in the printing world).
 *
 * These are different than the STATCAT_INFO because they don't change the status of the
 * sending module. They are passed through
*/ 
#define STATUS_JOBINFO              (STATSEV_INFO|STATCAT_JOBINFO)
/** Ustatus information for pjl */
#define STATUS_JOBINFO_START_JOB   (STATUS_JOBINFO | STATTYPE_JOB    | 0x1) ///< Start Job
#define STATUS_JOBINFO_END_JOB     (STATUS_JOBINFO | STATTYPE_JOB    | 0x2) ///< End Job
#define STATUS_JOBINFO_PAGE        (STATUS_JOBINFO | STATTYPE_PAGE   | 0x3) ///< Page
#define STATUS_JOBINFO_CANCEL      (STATUS_JOBINFO | STATTYPE_DEVICE | 0x4) ///< Cancel


//Codes for hidden category. This status is sent by a module in the print
//path when it detects an error or a condition that requires the current pages
//to be cancelled or restarted. This is useful for when we detect a change in
//state that is hard to handle as a special case at the point we are at, but can 
//be caught at an earlier point in the normal code flow with this new state information. 
#define STATUS_HIDDENRESTART   (COMMON_CRITICAL_1|STATUS_RESTART_MASK|STATCAT_HIDDEN|0x00000001)
#define STATUS_HIDDENCANCEL   (COMMON_CRITICAL_1|STATCAT_HIDDEN|0x00000002)

/** Memory out status messages */
#define STATUS_MEMORYOUT      (STATUS_OFFLINE_MASK | STATSEV_CRITICAL | STATPRI_1 | STATCAT_MEMORYERROR )
#define STATUS_PCLMEMORYOUT   (STATUS_MEMORYOUT | 0x00000001 )  // PCL's heap was exhausted, even with MEt

#define STATUS_ERROR          (COMMON_FATAL | STATCAT_ERROR)
#define STATUS_ERROR_ASSERT   STATUS_ERROR | 0x00000001

// Output Tray/Bin Category
//
// STATCAT_OUTPUT combines aspects of STATCAT_PAPEROUT and STATCAT_INPUT.
//
// When one or more (or all) output bin(s) is(are) full, we distinguish between
// warning the user about this condition just by itself, compared to making it a
// stall when we want to print a page and cannot, on account of this issue.
// This is analogous to the distinction between STATUS_PAPER_OUT_ALL and the
// other forms of STATUS_PAPER_OUT<whatever>.
//
// Subcategories...
#define STATUS_OUTPUT_ERROR     (COMMON_CRITICAL_1|STATUS_RESTART_MASK|STATCAT_OUTPUT)
#define STATUS_OUTPUT_STALL     (STATUS_OFFLINE_MASK|STATSEV_CRITICAL|STATPRI_1|STATUS_USERINPUT_MASK|STATCAT_OUTPUT)
#define STATUS_OUTPUT_WARNING   (STATSEV_WARNING|STATPRI_1|STATCAT_OUTPUT)
/**
 * Specific STATUS_OUTPUT_subcategory_xxx values are uniquely numbered across
 * all subcategories of error, stall, or warning.
 */
#define STATUS_OUTPUT_STALL_CLEAR_FACEDOWN_BIN      (STATUS_OUTPUT_STALL|0x00000001)
#define STATUS_FACEDOWN_OUTPUT_BIN_FULL             STATUS_OUTPUT_STALL_CLEAR_FACEDOWN_BIN ///< Synonym (KARA-S Output Bin Full Stack)
#define STATUS_OUTPUT_WARNING_ALL_BINS_FULL         (STATUS_OUTPUT_WARNING|0x00000002)

// Drum Status
#define STATUS_CARTRIDGE_SUBMASK	(STATCAT_CARTRIDGE|0x000000FF)
#define STATUS_DRUM_LOW_MODE_1		(STATCAT_CARTRIDGE|0x00000001)
#define STATUS_DRUM_LOW_MODE_2		(STATCAT_CARTRIDGE|0x00000002)
#define STATUS_DRUM_LOW_MODE_3		(STATCAT_CARTRIDGE|0x00000003)
#define STATUS_DRUM_END				(STATCAT_CARTRIDGE|0x0000000f)

// Toner Status
#define STATUS_TONER_SMICFAIL		(STATCAT_CARTRIDGE|0x00000011)
#define STATUS_TONER_SMICILLEGAL	(STATCAT_CARTRIDGE|0x00000012)
#define STATUS_TONER_LOW				(STATCAT_CARTRIDGE|0x00000013)
#define STATUS_TONER_END				(STATCAT_CARTRIDGE|0x00000014)
#define STATUS_TONER_OVERRUN		(STATCAT_CARTRIDGE|0x00000015)
#define STATUS_TONER_NO_ISSUE		(STATCAT_CARTRIDGE|0x00000016) /* [KSR071502] */

#define STATUS_TONER_REMAIN_CHECK	(STATCAT_CARTRIDGE|0x00000020)

// Engine Communication Status
#define STATUS_CONNECTION_OPTIONFAIL	(STATCAT_CONNECTION|0x00000001)
#define STATUS_CONNECTION_SMICFAIL		(STATCAT_CONNECTION|0x00000002)

// Engine Fatal Status
#define STATUS_FATAL_ENGINE_FEEDMOTORFAIL		(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000001)
#define STATUS_FATAL_ENGINE_MIRRORMOTORFAIL	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000002)
#define STATUS_FATAL_ENGINE_MAINFANMOTORFAIL	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000003)
#define STATUS_FATAL_ENGINE_SUBFANMOTORFAIL	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000004)
#define STATUS_FATAL_ENGINE_OPTIONFAIL			(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000005)
#define STATUS_FATAL_ENGINE_DEVFANMOTORFAIL			(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000006)

#define STATUS_FATAL_ENGINE_FUSER300	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000030)
#define STATUS_FATAL_ENGINE_FUSER301	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000031)
#define STATUS_FATAL_ENGINE_FUSER302	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000032)
#define STATUS_FATAL_ENGINE_FUSER303	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000033)
#define STATUS_FATAL_ENGINE_FUSER304	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000034)
#define STATUS_FATAL_ENGINE_FUSER305	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000035)
#define STATUS_FATAL_ENGINE_HSYNCFAIL	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000040)
#define STATUS_FATAL_ENGINE_VIDEOLONG	(COMMON_FATAL|STATCAT_PRINTHEAD|0x00000041)

#ifdef __cplusplus
}
#endif

#endif


