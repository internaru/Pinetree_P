/* 
 *
 * ============================================================================
 * Copyright (c) 2007-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *               Marvell Confidential
 * ============================================================================
 *
 */
/** 
 * \file platform.h
 *
 * \brief Defines for customizing a platform
 *
 */

#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include "ATypes.h"
#include "location_api.h"
#include "sqlite3.h"
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEMPLATE_1284_STRING    "xxMFG:%s;MDL:%s;CMD:%s;CLS:%s;DES:%s;FWVER:%s;"

/** Defines for printing. See agprint.h */
#define NUM_MEDIA_TYPES  21
#define NUM_MEDIA_SIZES  29
#define NUM_INPUTS       4
#define NUM_OUTPUTS      1

/** Number of printer output marking color channels */
#define NUM_MARKING_COLOR_CHANNELS       1

/** Number of print processes; e.g. monochrome, color **/
#define NUM_PRINT_PROCESSES 1  /* Monochrome printing only */

/** The IO Timeout value is in seconds */
#define DEFAULT_IO_TIMEOUT 120
#define MIN_IO_TIMEOUT       5
#define MAX_IO_TIMEOUT    1800


/**
 \brief Timer defines
 */
#define TIMER_INTS_PER_SECOND 100   ///< 10 msec time period.
#define SYS_TICK_FREQ TIMER_INTS_PER_SECOND



/////////////////////////////////////////////////////////////////////////////////
// USB Platform Identifiers
/////////////////////////////////////////////////////////////////////////////////

// \brief These values will almost certainly need to change for each platform
#define DEFAULT_VENDOR_ID       0x200b
#define DEFAULT_PRODUCT_ID      0x0200
#define DEFAULT_PROD_STRING     "SINDOH Printer"
#define DEFAULT_MFG_STRING      "SINDOH"
#define DEFAULT_PRINTER_NAME    "SINDOH Printer def"

// \brief These values probably won't need to change, but they're here because it's a happy place for them
#define DEFAULT_DEV_CLASS       "PRINTER"
#define DEFAULT_USB_LANG        0x0409
#define DEFAULT_USB_SERIALNUM   "0"
#define DEFAULT_CMD_LANG        "ACL"

/////////////////////////////////////////////////////////////////////////////////
// LSP Memory Limits
/////////////////////////////////////////////////////////////////////////////////
#define SYSTEM_LIMITER_MAX_MB 214 ///< This is the maximum amount of memory allowed by the system limiter.

/////////////////////////////////////////////////////////////////////////////////
// \brief WatchDog Timer Configuration
/////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_WATCHDOG_TIMEOUT    500     ///< watchdog timeout in ticks (10ms/tick) (5 Seconds)
#define WATCHDOG_SERVICE_INTERVAL   DEFAULT_WATCHDOG_TIMEOUT/10     ///< how often the watchdog thread will reset the watchdog

/////////////////////////////////////////////////////////////////////////////////
//
// Engine timings, these valused to slow down engine speed from rated speed.
// If there is no other way to slow an engine set up the delays here.  If not used
// set these both to zero.  Start time is now many clock ticks to delay between
// sending starts to the engine.  INIT_TIME is how long it takes to initialize the
// engine the first time you print.
//
#define START_TIME 0
#define INIT_TIME 0




//
// These defines are used in the jobmgr and specify how far ahead the engine
// runs vs the code.
//
#define PRINT_SCAN_Q_DEPTH 1 ///< How deep to scan when looking for blocks to free

#ifdef HAVE_GENERATOR_COMPRESS
#define PM_QUEUE_SIZE 1000 + 2  ///< Max number of pages in a collated copy job, + 2 for cancel.
#define PM_QUEUE_MIN_SIZE  1000   ///< minimum number of page sides to work ahead, 
/// too small a min_size and the engine gets ahead too large and memory storage 
/// is in the printer rather than on the host.  If collated copies are running then storage for up
/// to PM_QUEUE_SIZE is used, otherwise storage for PM_QUEUE_MIN_SIZE page sides is used.
#else
#define PM_QUEUE_SIZE 4 + 2  ///< Max number of pages in a collated copy job, + 2 for cancel
#define PM_QUEUE_MIN_SIZE  4 ///< minimum number of page sides to work ahead.
/// without compression 2 pages duplex is 4 sides the minimum for a duplex printer.
#endif





// \brief CPU usage allocation
#define CPU_FAX     75
#define CPU_PRINT   55
#define CPU_SCAN    0		//45 //To solve Concurrency (Scanning with Print job) set to 0
#define CPU_DEFAULT 0
#define CPU_SCANCALJOB  35
void getPrinterName(char *value);


#ifdef __cplusplus
}
#endif

#endif
