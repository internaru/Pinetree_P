/***********************************************************
* (c) Copyright 2008-2011 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

/** @file logger.h
 * <pre>
 * DBG_PRINT provides a runtime configureable macro comparison to 
 * selectively log based on 
 * module, submodule/channel and logging level.
 * something like: dbg_macro( module, submodule, level, format_string, ... );   
 * will compare a mask in a table indexed by module and level against the input bit (submodule/channel)
 * if (modules_dbg_flags[ 0-31 ][ 0-7 ] & <submodule/channel passed in via macroparameters>)
 * ----------------------module--level----submodule/channel: bit is compared against stored mask
 *    then runtime print will occur see DBG_WOULD_PRINTF()
 *
 * dbg_macro2( level, fmt, .... )  // uses non-passed parameter #define: LOGGER_MODULE_MASK
 * 
 * 
 * #define LOGGER_MODULE_MASK         MODULE | LOGGER_SUBMODULE_BIT( [0-26] ) 
 * LOGGER_SUMBODULE_BIT(0) is the predefined submodule "main"
 * LOGGER_MODULE_MASK = [0-7] | at least one bit out of 0xffffffe0

 *
 * user can add in submodule in the LOGGER_MODULE_MASK or in the level_macro_parameter field.
 *
 * module mask            is 0x0000 001f
 * level mask             is 0x0000 0007
 * submodule/channel mask is 0xffff ffe0
 *    Intent of a submodule/channel is to allow each bit to be a seperate logging control
 *    Setting all the bits in a submodule would log everything at that level or lower. 
 * 
 * Motivation: Cleanup logger output via compile time setting in logger_config.c or at runtime via 
 * serial commands.  Recommended usage is in example 1, define a channel/submodule bit or that with 
 * the module setting the LOGGER_MODULE_MASK for the file or group of files.  
 * Then use a convenience macro to set the log level like: DBG_PRINTF_ERR().  
 * logger_config.c can either be configured to log the items of interest or you can use the serial 
 * CMD==> logger 
 * command to run time configure the logging levels.  
 *
 * example: 
 * 
 * top of C file:  
 *
 * #define DBG_PRFX "telnet: "    // simple string prepend matching submodule name is a good idea.
 * #define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE_BIT( 7 ) 
 * // SUBMODULE( DEBUG_LOGGER_MODULE_NETWORK, "telnet", 7 ) // comment to match submodule register.
 *    ^---- this comment will match the submodule name id registration.
 *
 * use in c file:
 * 
 * DBG_PRINTF_ERR("error creating init semaphore (threadx_rcode=0x%02x)\n", threadx_rcode);
 *             ^--- the logging level is LOG_ERR 
 *
 * output : "telnet: error creating init semaphore (threadx_rcode=0xff)"
 *             ^------- comes from DBG_PRFX
 * 
 *
 * Don't do this, deprecated: 
 *  example 2: set module with submodule passed in on every call.
 * 
 * #define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_PRINT
 * #define DBG_JOBMGR      0x00001000  
 * DBG_PRINTF(LOG_ERR|DBG_JOBMGR, "JM: WARNING: received obsolete message MSG_JBIG_BIH\n");
 * 
 *
 * example 3: minimum setup must define LOGGER_MODULE_MASK as module | submodule
 * 
 * #define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 0 )
 * DBG_PRINTF(LOG_ERR, "Print: module %d level %d submodule %x\n", 
 *                      DEBUG_LOGGER_MODULE_PRINT, LOG_ERR, DEBUG_LOGGER_MAIN_MODULE_ONLY);
 *
 *
 * Don't do this, deprecated: 
 * example 4: old style always logs to DPRINTF module
 *
 * DPRINTF(DBG_ERROR, ("module is %d level %d submodule %d"), DEBUG_LOGGER_MODULE_DPRINTF, LOG_ERR, DEBUG_LOGGER_MAIN_MODULE_ONLY));
 * 

 *
 * 
 * To configure example 1 to log via the serial console:
 *
 * CMD==> help logger 
 * 
 * CMD==> logger reset
 *   will output the current settings as a program that can be saved to logger_config.c 
 *   it will then reset the settings to the defaults.
 * 
 * </pre>
 */


#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdint.h>
#include <stdbool.h>
#ifndef __linux__
#include "cpu_api.h"
#endif
#include "dprintf.h"    // always include dprintf.h; this fixes cases where
                        // release builds would break because someone was
                        // using agprintf


#ifdef __cplusplus
extern "C" {
#endif



/// log string to memory buffer.
void logStringImpl(char *pFormat, ...);

/// initialize memory logger
void logStringInit( void );

/// dump memory log to serial or telnet.
void DumpMemStringLog( int sock );

/// clear memory log.
void ClearMemStringLog( void );


 /******************************************************************************
 * DEPRECIATED interface DPRINTF prefer DBG_PRINTF for new code.
 *
 * The following contains defines for debug printf and debug memory logging.
 * String output can be directed to a terminal or to a memory log by using
 * the system macro DPRINTF.
 *
 * The variable DBG_TRACE_OPTIONS is used to determine what "level" of output
 * is desired. The levels are defined as DBG_ENTRY, DBG_ERROR, etc. This 
 * variable can be set via a debugger connection to control debug output in
 * realtime. Alternatively, the usbsend tool can be used to modify the memory
 * location containing DBG_TRACE_OPTIONS - use the build mapfile to identify
 * the memory address.
 *
 * The variable DBG_OUTPUT is used to control the location of the output, 
 * either memory log or data terminal. This variable should be used in the
 * DPRINTF macro, for example:
 *
 *      DPRINTF( (DBG_LOUD | DBG_OUTPUT), ("VIDEO: got CLEANING page\n"));
 *
 * A word about DPRINTF and interrupt service routines - in general, output
 * to the memory log is preferred. Our uart driver doesn't incorporate memory
 * buffering so writes to the data terminal can be slow. For error conditions,
 * outputting to the terminal is acceptable, especially for "showstopper" 
 * errors.
 *      DPRINTF( (DBG_LOUD | DBG_LOG), ("VIDEO_ISR: top of page\n"));
 *
 * Use () around each parameter in the macro DPRINTF(a,b).
 *
 *****************************************************************************/

/// Legacy log level defines mapped to new log levels.
#define DBG_ENTRY       LOG_DEBUG_M 
#define DBG_EXIT        DBG_ENTRY
#define DBG_ERROR       LOG_ERR     
#define DBG_SOFT        LOG_INFO    
#define DBG_LOUD        LOG_WARNING 

//// depreceated
 #define DBG_ALL         0x000007FF
 #define DBG_TERMINAL    0 // 0x10000000  // Output goes to uart terminal
 #define DBG_LOG         0 // 0x20000000  // Output goes to product memory
 #define DBG_OUT_USB     0 // 0x40000000  // Output goes to usb port
 #define DBG_OUT_NETWORK 0 // 0x80000000  // Output goes to network
//// end depreciated

//// depreceated
extern unsigned int DBG_OUTPUT;
//// end depreciated

/* ***********************************************************************
 */

#ifdef HAVE_SINDOH_LOG_SYSTEM
#define TOTAL_LOG_LEVEL 11
#define LOG_BIT 0x0000000F
#else
#define TOTAL_LOG_LEVEL 8
#define LOG_BIT 0x00000007
#endif
/// Global array of masks used to determine if logging should occur at runtime.
/// if one of the bits above 1<<5 is set and the bit matches the incoming 
/// should I print request then the printf will occur.
extern uint32_t modules_dbg_flags[32][TOTAL_LOG_LEVEL]; 

#ifdef HAVE_SINDOH_LOG_SYSTEM
#define LOG_EMERG       0  ///< aliased to LOG_CRIT
#define LOG_ALERT       0  ///< aliased to LOG_CRIT
#define LOG_CRIT        0  ///< LOG_CRIT the most severe error 
#define LOG_SDH_CRIT        1  ///< SINDOH CRIT Log
#define LOG_SDH_ERR         2  ///< SINDOH ERR Log
#define LOG_ERR         3  ///< second most severe error level
#define LOG_SDH_DEBUG       4  ///< SINDOH DEBUG Log
#define LOG_WARNING     5  ///< Recoverable.
#define LOG_NOTICE      6  ///< notice me I'm important but its all good.
#define LOG_INFO        7  ///< nothing is wrong I'm just logging.
#define LOG_DEBUG       8  ///< debug only lots of printfs
#define LOG_DEBUG_M     9  ///< medium debug info
#define LOG_DEBUG_H     10  ///< log every thing level

#define LOG_MAX_LEVEL   LOG_DEBUG_H
#else
/// LOG_LEVEL 0-7
/// log type globally log anything less than a set threshold 
/// if enabled_modules_flags > this statements LOG_XXX value it will print.
#define LOG_EMERG       0  ///< aliased to LOG_CRIT
#define LOG_ALERT       0  ///< aliased to LOG_CRIT
#define LOG_CRIT        0  ///< LOG_CRIT the most severe error 
#define LOG_ERR         1  ///< second most severe error level
#define LOG_WARNING     2  ///< Recoverable.
#define LOG_NOTICE      3  ///< notice me I'm important but its all good.
#define LOG_INFO        4  ///< nothing is wrong I'm just logging.
#define LOG_DEBUG       5  ///< debug only lots of printfs
#define LOG_DEBUG_M     6  ///< medium debug info
#define LOG_DEBUG_H     7  ///< log every thing level

#define LOG_MAX_LEVEL   LOG_DEBUG_H
#endif

#define LOG_MAX_FLAG    0x7  ///< NOT a log level, a test mask

//#define DBG_ANY_MASK         (~0x7)  ///< NOT a log level, a test mask

/// LOG_MODULE 0-31
/// must be sequential with unused at the end.
#define DEBUG_LOGGER_MODULE_PRINT           0 ///< PRINT MODULE has submodules.
#define DEBUG_LOGGER_MODULE_CNTRL_PANEL     1 ///< control panel 
#define DEBUG_LOGGER_MODULE_USB_DEVICE      2 
#define DEBUG_LOGGER_MODULE_NVRAM           3 ///< non-volatile spi flash etc.
#define DEBUG_LOGGER_MODULE_SYSTEM          4 ///< SYSTEM is a big bag.
#define DEBUG_LOGGER_MODULE_NETWORK         5 ///< Lots of submodules in NETWORK
#define DEBUG_LOGGER_MODULE_GPIO            6 ///< DEVICE with submodule might be better idea.
#define DEBUG_LOGGER_MODULE_SCAN            7
#define DEBUG_LOGGER_MODULE_ENGINE          8 ///< is this print engine 
#define DEBUG_LOGGER_MODULE_DPRINTF         9 ///< DPRINTF's without LOGGER_MODULE_MASK defined go here.
#define DEBUG_LOGGER_MODULE_DEVICES        10 ///< DEVICE with submodule
#define DEBUG_LOGGER_MODULE_VIDEO          11 ///< VIDEO
#define DEBUG_LOGGER_MODULE_JBIG           12 ///< JBIG
#define DEBUG_LOGGER_MODULE_HTTP           13 ///< HTTP
#define DEBUG_LOGGER_MODULE_FILESYSTEM     14 ///< Filesystem
#define DEBUG_LOGGER_MODULE_GENERATORS     15 ///< Generators
#define DEBUG_LOGGER_MODULE_CONSUMABLES    16 ///< Consumables
#define DEBUG_LOGGER_MODULE_OEM1           17 ///< OEM1 move OEM id's down as we add more 
#define DEBUG_LOGGER_MODULE_OEM2           18 ///< OEM2 each oem can add its own submodules.
#define DEBUG_LOGGER_MODULE_OEM3           19 ///< OEM3 Do not allow an oem to rename these.
#define DEBUG_LOGGER_MODULE_OEM4           20 ///< OEM4
#define DEBUG_LOGGER_MODULE_INTERNAL       21
#define DEBUG_LOGGER_MODULE_SCANBOX        22 ///< Scan Box debug
#ifdef HAVE_IMAGELOG
#define DEBUG_LOGGER_MODULE_IMAGELOG       23 ///< image log
#define DEBUG_LOGGER_LAST_MODULE           23 ///< keep this updated to the last in use module number, 
                                              ///< keep logger.c::debug_logger_idx_names[] updated. 
#else
#define DEBUG_LOGGER_LAST_MODULE           22 ///< keep this updated to the last in use module number, 
                                              ///< keep logger.c::debug_logger_idx_names[] updated. 
#endif


/// Default submodule/channel bit for modules that don't want to use submodules 
/// or want a main module verses more specialized bits above.
#define DEBUG_LOGGER_MAIN_MODULE_ONLY      32

/// These bits are used by submodule/channels and are the only valid bits in the modules_dbg_flags[][] 
#define DEBUG_MODULE_MASK 0xffffffe0      /* ~ 0x001f */


#ifdef HAVE_DBG_PRINTF
  /// DPRINTF(level, (fmt, ...));  Older interface newer code should use DBG_PRINTF
  /// default module/channel gets used when someone forgets to define LOGGER_MODULE_MASK.
  /// defaults to channel 9 and main module/channel only.

#  define WOULD_DPRINTF(a) ( (modules_dbg_flags[9][((a)&LOG_BIT)] & 32) && (a) )
#  define DPRINTF(a, b) if ( WOULD_DPRINTF(a) ) { dbg_printf b; }

#else
static __inline__ void REL_DPRINTF(const char *format, ...){}
#  define DPRINTF(a, b) REL_DPRINTF b
#endif // HAVE_DBG_PRINTF




/// compile time log this compilation unit to memory instead of serial/telnet
/// globally logging to memory will disable user input so isn't very useful.
/// defining/undefining this locally can be used to transform
/// serial/telnet logging statements into memory log statements
#ifdef MEMORY_LOG_ONLY 
# define my_dbg_printf logStringImpl
#else
# define my_dbg_printf dbg_printf
#endif

#ifdef HAVE_DBG_PRINTF

/// Instead of calling dbg_printf, return true if DBG_PRINTF would have printed with the level and LOGGER_MODULE_MASK settings.
/// example if (DBG_WOULD_PRINTF(level)) call_massive_printf_routine to dump object state.
/// compares the submodule/channel to log bits stored at [module][level] against the input submodule/channel bits.
/// module must be set in the LOGGER_MODULE_MASK, the lower 5 bits 0-31
/// submodule bit mask should be set in the LOGGER_MODULE_MASK the upper 32-5 bits, or
/// submodule bit mask can be set in the level parameter.  
 #define DBG_WOULD_PRINTF(level) \
    ( modules_dbg_flags[((LOGGER_MODULE_MASK) & 0x0000001f)][((level) & LOG_BIT)] & ((LOGGER_MODULE_MASK) | (level)) ) 

/// The prefered logging mechanism, uses file local or header file included LOGGER_MODULE_MASK.
 #define DBG_PRINTF(level, fmt,...) do {   \
    if (  DBG_WOULD_PRINTF(level) )        \
    {  my_dbg_printf(fmt,## __VA_ARGS__); } } while (0)

/// explicitly log to memory log with [module][level] control.
 #define DBG_MEMLOG(level, fmt,...)  do { \
    if (DBG_WOULD_PRINTF(level) )         \
    {  logStringImpl(fmt,## __VA_ARGS__); } } while(0)

/// temporary don't use this use DBG_MEMLOG instead. logs to memory if [DPRINTF][LOG_ERR] with bit 0x20 set 
 #define logString(fmt,...)               \
    if ( WOULD_DPRINTF((LOG_ERR)))        \
    {  logStringImpl(fmt,## __VA_ARGS__); }
#else  // HAVE_DBG_PRINTF
    // disabled 
#if 1
#define LOG_SDH_DEBUG       4  ///< SINDOH DEBUG Log
static __inline__ int DBG_PRINTF(int flags, const char* fmt, ...){return 0;}
static __inline__ int DBG_MEMLOG(int flags, const char* fmt, ...){return 0;}
static __inline__ void logString(char *pFormat, ...){}
#else
 #define DBG_PRINTF(flags, fmt, args...)
 #define DBG_MEMLOG(level, fmt,...)    
 #define logString(fmt,...)   
#endif
 #define DBG_WOULD_PRINTF(level) false
#endif // HAVE_DBG_PRINTF
#ifndef UNUSED_VAR 
 /*!
 * \def UNUSED_VAR(x) 
 * a simple macro that will turn off the unused var compiler warning for a given var
 **/
  #define UNUSED_VAR(x) ((x) = (x))  
#endif

/// macros to include log_level and a file provided DBG_PRFX
/// these are highly recommended as the automatic inclusion of DBG_PRFX allows for post 
/// logging searching.  DBG_PRFX can be a text prefix based on module or submodule.
///  
#define DBG_PRINTF_EMERG(...) DBG_PRINTF(LOG_EMERG, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_ALERT(...) DBG_PRINTF(LOG_ALERT, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_CRIT(...) DBG_PRINTF(LOG_CRIT, DBG_PRFX __VA_ARGS__)
#ifdef HAVE_SINDOH_LOG_SYSTEM
#define DBG_PRINTF_SDH_CRIT(...) DBG_PRINTF(LOG_SDH_CRIT, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#endif
#define DBG_PRINTF_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#ifdef HAVE_SINDOH_LOG_SYSTEM
#define DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)
#endif
#define DBG_PRINTF_WARNING(...) DBG_PRINTF(LOG_WARNING, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_NOTICE(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_INFO(...) DBG_PRINTF(LOG_INFO, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_DEBUG(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_DEBUG_M(...) DBG_PRINTF(LOG_DEBUG_M, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_DEBUG_H(...) DBG_PRINTF(LOG_DEBUG_H, DBG_PRFX __VA_ARGS__)
#define DBG_MEMLOG_EMERG(...) DBG_MEMLOG(LOG_EMERG, DBG_PRFX __VA_ARGS__)

#define DBG_MEMLOG_ALERT(...) DBG_MEMLOG(LOG_ALERT, DBG_PRFX __VA_ARGS__)
#define DBG_MEMLOG_CRIT(...) DBG_MEMLOG(LOG_CRIT, DBG_PRFX __VA_ARGS__)
#ifdef HAVE_SINDOH_LOG_SYSTEM
#define DBG_MEMLOG_SDH_CRIT(...) DBG_MEMLOG(LOG_SDH_CRIT, DBG_PRFX __VA_ARGS__)
#define DBG_MEMLOG_SDH_ERR(...) DBG_MEMLOG(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#endif
#define DBG_MEMLOG_ERR(...) DBG_MEMLOG(LOG_ERR, DBG_PRFX __VA_ARGS__)
#ifdef HAVE_SINDOH_LOG_SYSTEM
#define DBG_MEMLOG_SDH_DEBUG(...) DBG_MEMLOG(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)
#endif
#define DBG_MEMLOG_WARNING(...) DBG_MEMLOG(LOG_WARNING, DBG_PRFX __VA_ARGS__)
#define DBG_MEMLOG_NOTICE(...) DBG_MEMLOG(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_MEMLOG_INFO(...) DBG_MEMLOG(LOG_INFO, DBG_PRFX __VA_ARGS__)
#define DBG_MEMLOG_DEBUG(...) DBG_MEMLOG(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_MEMLOG_DEBUG_M(...) DBG_MEMLOG(LOG_DEBUG_M, DBG_PRFX __VA_ARGS__)
#define DBG_MEMLOG_DEBUG_H(...) DBG_MEMLOG(LOG_DEBUG_H, DBG_PRFX __VA_ARGS__)



/// Convert submodule id from 0-26 to a bit for use in LOGGER_MODULE_MASK 
/// usage: 
/// #define LOGGER_MODULE_MASK   DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE_BIT( 0 )
#define LOGGER_SUBMODULE_BIT(submodule_id)  ( 1 << (submodule_id + 5 )) 


/// used by cmd interface and logger_config to set the logging levels via strings.
/// level can be "off" or "LOG_CRIT" through "LOG_DEBUG"
/// 
int logger_set_1( const char * level );
  
/// module names are defined in logger.c
///
int logger_set_2( const char * module, const char * level );

/// submodule name are registered in logger_config.c
/// 
int logger_set_3( const char * module, const char * submodule, const char * level );

/// Control timestamping prepended to the front of logger printfs.
/// @param onOff true to enable timestamps, false to disable timestamps
/// @param usecTick true to use microsecond timestamps false to use OS ticks.
void logger_timestamp_onOff_usecTick( bool onOff, bool usecTick );

/// Register submodule, associating a name with the submodule ID.
/// @param module_name A user friendly name used to reference the submodule in the serial debug logger
/// @return >=0 on success; < 0 on error
int logger_submodule_register( int module_index, int submodule_id, const char * module_name );

/// Configuration of the default logging settings, can be edited or 
/// the commandline can be used to modify the log level settings at runtime.
///
void  logger_local_init();
#ifdef __cplusplus
}
#endif

#endif /* _LOGGER_H_ */
