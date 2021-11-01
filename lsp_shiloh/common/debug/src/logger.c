/** 
 * ============================================================================
 * Copyright (c) 2006-2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#ifdef HAVE_NETWORK
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#else
#include "cpu_api.h"
#include "trsocket.h"
#endif
#endif // HAVE_NETWORK

#include "lassert.h"

#include <pthread.h>
#include <posix_ostools.h>

#include "dprintf.h"
#ifdef HAVE_TIMER
#include "timer_api.h"
#endif
#include "logger.h"
#include "debug.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#define DEBUG_BUF_SIZE 0x80000
#define MAX_LOG_STRING_LEN 128
#define MIN_BUFFER_MARGIN 256


#define DBG_PRFX "LOGGER: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 9 ) 
// SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "LOGGER", 9 )




/* \brief global variable, deprecated, when DPRINTF is removed, this should be removed also */
unsigned int DBG_OUTPUT = 0;

/* \brief global variable used to implement DBG_PRINTF macros */
uint32_t modules_dbg_flags[32][TOTAL_LOG_LEVEL] = { {0} };

// pointers to static module names. 
const char * modules_dbg_submodule_names[32][32-5] = { {0} };


// Debug buffer management variables.
static char *pEndBuffer   = NULL;
static char *pStartBuffer = NULL;
static uint32_t logger_buffer_size = 0; // Used to provide size variable to debugger for scripted download

#ifdef HAVE_MEMORY_LOG
static char pLogData[DEBUG_BUF_SIZE+4];
static char pDumpBufData[MAX_LOG_STRING_LEN];
static char *pLog = pLogData;
static char *pDumpBuf = pDumpBufData;
#else
static char *pLog = NULL;
static char *pDumpBuf = NULL;
#endif // HAVE_MEMORY_LOG

static uint32_t* pLogGuard = NULL;
static uint32_t entryCnt;
static bool logger_timestamp_on = false;
static bool logger_timestamp_usec = false;

bool logger_get_timestamp_on( void )
{
    return logger_timestamp_on;
}

bool logger_get_timestamp_usec( void )
{
    return logger_timestamp_usec;
}

 
/** 
 * \brief initialize logger string buffers
 * 
 */
#define LOG_GUARD_VAL 0xABCD4321
void logStringInit( void )
{
    entryCnt = 0;

    if(pLog != NULL)
    {
        pStartBuffer = pLog;
        logger_buffer_size = DEBUG_BUF_SIZE;
        pEndBuffer = pLog + DEBUG_BUF_SIZE;
        pLogGuard = (uint32_t*)pEndBuffer;
        *pLogGuard = LOG_GUARD_VAL;
        memset( pLog, 0, DEBUG_BUF_SIZE );
    }
}


/** 
 * \brief
 * 
 * \param pFormat
 * 
 */
void logStringImpl(char *pFormat, ...)
{
    va_list ap;
#ifdef HAVE_DELAY
#include "delay_api.h"
    uint32_t        uiNewCnt = timer_get_time_usec();
#else
    uint32_t        uiNewCnt = 0;
#endif
    static uint32_t uiLastCnt = 0;  
    
#ifdef __linux__
    static pthread_mutex_t log_string_mutex = POSIX_MUTEX_INITIALIZER;
#else
    uint32_t interrupts;
#endif
    uint32_t cnt;

    // If memory hasn't been allocated, return.
    if( pLog == NULL )
    {
        //ASSERT(FALSE);
        return;
    }

#ifdef __linux__
    pthread_mutex_lock(&log_string_mutex);
#else
    interrupts = cpu_disable_interrupts();
#endif

    // Format the string, using the log address pointer as the destination
    // location. Assume that the string is shorter than 128 bytes!
    // Write the event counter to the log buffer. This allows for making sense
    // of the order of events in a circular buffer.
    
    //  cnt = minSprintf(pLog, "%3d 0x%08x %8u ", entryCnt, event, uiNewCnt );          // absolute time
    cnt = minSprintf(pLog, "%6u %10u ", entryCnt, uiNewCnt - uiLastCnt ); // delta time
    
    uiLastCnt = uiNewCnt;
    pLog += cnt;  
    
    va_start(ap, pFormat);
    cnt = minVsprintf(pLog, pFormat, ap);
    va_end(ap);

    ASSERT(cnt < MIN_BUFFER_MARGIN);
    if(cnt > MAX_LOG_STRING_LEN) cnt = MAX_LOG_STRING_LEN;
    pLog += cnt;
    
    entryCnt++;
    
    // If the log pointer is less than 128 bytes from the end of the memory
    // allocated for logging, simply move the pointer back to the start of
    // the log buffer.
    if(pLog + MIN_BUFFER_MARGIN >= pEndBuffer) pLog = pStartBuffer;
    
#ifdef __linux__
    pthread_mutex_unlock(&log_string_mutex);
#else
    cpu_restore_interrupts(interrupts);
#endif    
    XASSERT(*pLogGuard == LOG_GUARD_VAL, *pLogGuard);
}


static int dbg_slog_memory_filter_cnt = 0;

#define filter_size_bit 0x00400000
static int dbg_slog_memory_size = 0 ;
void dbg_slog_filter_size( char * line )
{
  int len;
  int msize;

  if ( ! (dbg_slog_memory_filter_cnt & filter_size_bit) ) 
    return ;

  if ( line[0] == line[1] && line[0] == '+' ) {
    len = strlen( line );
    msize = atoi( &line[len - 8] );
    if (msize >= dbg_slog_memory_size)
      dbg_printf( line );
  }
}

#define filter_time_bit 0x00800000
static int dbg_slog_memory_time = 0 ;
void dbg_slog_filter_time( char * line )
{
  int len;
  int msize;

  if ( ! (dbg_slog_memory_filter_cnt & filter_time_bit) ) 
    return ;

  if ( line[0] == line[1] && line[0] == '+' ) {
    len = strlen( line );
    msize = atoi( &line[len - 15] );
    if ( msize >= dbg_slog_memory_time )
      dbg_printf( line );
  }
}
//123456789012345678901234567890123456789012345678901234567890
//+++ 00a44780         dcDCPL_Mem.c    49  6224      168

void dbg_slog_filter_file( char * line, char * file )
{
  int len = strlen(line);
  int pos = 12;

  if ( line[0] == line[1] && line[0] == '+' ) {
    for (pos = 12; pos < len && ' ' == line[pos]; pos++ );
    if ( 0 == (strncmp( file, &line[pos], strlen(file) )))
      dbg_printf( line );
  }
}

#define filter_free_bit 0x01000000
void dbg_slog_filter_free( char * line )
{
  static char prev[128] = "";
  static char minus[128] = "";

  DBG_ASSERT(line); // kclockwork ...
  if ( ! (dbg_slog_memory_filter_cnt & filter_free_bit) ) 
    return;

  if ( line[0] == line[1] && line[0] == '+' )
  {
    if (minus[0] && prev[0] && line ) {
      dbg_printf( prev );
      dbg_printf( minus );
      dbg_printf( line );
      prev[0] = 0; 
    } else {
      strcpy(prev, line);
    }
    minus[0] = 0;
  }
  else if ( line[0] == line[1] && line[0] == '-' && prev[0] )
  {
    strcpy(minus, line);
  }  
  else
  {
    prev[0] = 0;
  } 
}

static int dbg_slog_memory_file_cnt = 0;
static char dbg_slogmem_files[20][33] = {{0}};

/** 
 * \brief Dumps the specified section of the memory based string log to the terminal.
 * 
 * \param pSecStart
 * 
 * \param pSecEnd
 * 
 */
void DumpMemStringLogSection(char *pSecStart, char *pSecEnd, int sock)
{
    uint32_t secLen = pSecEnd - pSecStart;
    uint32_t secOffset;
    uint32_t curStrOffset = 0;

    dbg_printf("DumpMemStringLogSection: Section Len = %d bytes\n", secLen);

    // note: this is kind of slow because of the copy, but it avoids having to modify the log by adding
    // null terminators
    for (secOffset = 0; secOffset < secLen /*&& pSecStart[secOffset] != 0*/; secOffset++)
    {
        if(pSecStart[secOffset] == 0)
        {
            // BUGBUG
            //-jrs I put this in replacing the check in the for loop above because
            // some debug was getting lost.  Not sure it is necessary.
            // longer term may want to revert to original code.
            continue;
        }
        if (curStrOffset < MAX_LOG_STRING_LEN - 1)
        {
            pDumpBuf[curStrOffset] = pSecStart[secOffset];
            if (pDumpBuf[curStrOffset] == 0x0A)
            {
                pDumpBuf[++curStrOffset] = 0;
            }
        } else
        {
            pDumpBuf[curStrOffset] = 0;
        }

        if (pDumpBuf[curStrOffset] == 0)
        {
            if (sock < 0)
            {
                if (dbg_slog_memory_filter_cnt != 0 &&
                    (pDumpBuf[18] == '+' || pDumpBuf[18] == '-') && (pDumpBuf[19] == '+' || pDumpBuf[19] == '-'))
                {
                    int i;
                    dbg_slog_filter_free(&pDumpBuf[18]);
                    dbg_slog_filter_size(&pDumpBuf[18]);
                    dbg_slog_filter_time(&pDumpBuf[18]);
                    for (i = 0; i < dbg_slog_memory_file_cnt; i++) dbg_slog_filter_file(&pDumpBuf[18], &dbg_slogmem_files[i][0]);
                } else
                {
                    dbg_printf("%s", pDumpBuf);
                }
            } else
            {
#ifdef HAVE_NETWORK
                uint32_t len = curStrOffset;
                int bytesSent = 0;

                while (len > 0)
                {
                    bytesSent = send(sock, (void *)&pDumpBuf[curStrOffset - len], len, 0);
                    if (bytesSent < 0)
                    {
                        return;
                    }
                    len -= bytesSent;
                }
#endif
            }
            curStrOffset = 0;
        } else
        {
            curStrOffset++;
        }
    }

    dbg_printf("DumpMemStringLogSection: Bytes Remaining to dump = %d bytes\n", secLen-secOffset);
}
 

/** 
 * \brief Dumps the memory based string log to the serial output.
 * 
 * \retval None
 * 
 */
void DumpMemStringLog(int sock)
{
    char* pLogStr;
    if(pLog == NULL)
    {
        dbg_printf("HAVE_MEMORY_LOG disabled\n");
        return;
    }

    // find first non 0 log val
    pLogStr = pLog;
    while((pLogStr < pEndBuffer) && (*pLogStr == 0))
    {
        pLogStr++;
    }
    if(pLogStr == pEndBuffer)
    {
        pLogStr = pStartBuffer;
    }

    // copy each string to the temp buffer so we can null terminate and print it out
    if(pLogStr != pStartBuffer)
    {
        DumpMemStringLogSection(pLogStr, pEndBuffer, sock);
        dbg_printf("Dumped %#x to %#x\n", pLogStr, pEndBuffer);
        pLogStr = pStartBuffer;
    }
    dbg_printf("Dumping %#x to %#x\n", pLogStr, pLog);

    DumpMemStringLogSection(pLogStr, pLog, sock);
}

 
/** 
 * \brief Clears the memory string log.
 * 
 * \retval None
 * 
 */
void ClearMemStringLog(void)
{
#ifdef __linux__
//LSPTODO    uint32_t interrupts;
#else
    uint32_t interrupts;
#endif
    if(pLog != NULL)
    {
#ifdef __linux__
//LSPTODO        interrupts = cpu_disable_interrupts();
#else
        interrupts = cpu_disable_interrupts();
#endif
        pLog = pStartBuffer;
        memset( pLog, 0, DEBUG_BUF_SIZE );
        entryCnt = 0;
#ifdef __linux__
//LSPTODO        cpu_restore_interrupts(interrupts);
#else
        cpu_restore_interrupts(interrupts);
#endif        
    }
}

void logger_clear_module(uint32_t module)
{
    int l;

    // ASSERT(module < 32);
    module &= 0x1f; 

    for (l=0; l < TOTAL_LOG_LEVEL; l++ )
    {
        modules_dbg_flags[module][l] = 0; 
    }
}

void logger_enable_module_mask(uint32_t module, uint32_t submod_level)
{
    uint32_t l;
    uint32_t level = submod_level & LOG_BIT;
    uint32_t mask = submod_level & ~0x0000001f;

    for (l=0; l <= level; l++ )
    {
        modules_dbg_flags[module][l] |= mask; 
    }
    for (l=level+1; l < TOTAL_LOG_LEVEL; l++ )
    {
        modules_dbg_flags[module][l] &= ~mask;
    }
} 

void logger_enable_module_level(uint32_t module, uint32_t level, uint32_t mask)
{
    uint32_t l;

    // force inputs to be legal
    level  &= LOG_BIT;
    module &= 0x0000001f;
    mask &= 0xffffffe0;  

    if ( mask )
    {
        for (l=0; l <= level; l++ )
        {
            modules_dbg_flags[module][l] |= mask; 
        }
        l = level + 1;
    } 
    else
    {
        l = level ;  // zero input clears that level, greater than zero sets mask.
        mask = 0xffffffff;
    }

    for ( ; l < TOTAL_LOG_LEVEL; l++ )
    {
        modules_dbg_flags[module][l] &= ~mask; 
    }
}

static const char* debug_logger_module_idx_names[32] = 
{
    "PRINT",
    "CNTRL_PANEL",
    "USB_DEVICE", 
    "NVRAM", 
    "SYSTEM", 
    "NETWORK",
    "GPIO",
    "SCAN",
    "ENGINE",
    "DPRINTF",
    "DEVICES",
    "VIDEO",
    "JBIG",
    "HTTP",
    "FILESYSTEM",
    "GENERATORS",
    "CONSUMABLES",
    "OEM",
    "OEM2",
    "OEM3",
    "OEM4",
	"INTERNAL",
	"SCANBOX",
#ifdef HAVE_IMAGELOG
	"IMAGELOG",
#endif
    0
};

int logger_submodule_register( int module_index, int submodule_index, const char * module_name )
{
    int i = 0;

    XASSERT( module_index >= 0 && module_index < 32, module_index );
    XASSERT( submodule_index >= 0 && submodule_index < 27, submodule_index );
    XASSERT( debug_logger_module_idx_names[module_index] != 0, module_index  );

    if ( modules_dbg_submodule_names[module_index][submodule_index] == 0 ) 
    {
        modules_dbg_submodule_names[module_index][submodule_index] = module_name;
    } 
    else 
    {
        for ( i = 0; i < 32 && modules_dbg_submodule_names[module_index][i] != 0; i++ )
            ;
        return -i;
    }
    return i;
}

#ifdef HAVE_CMD

static const char * debug_logger_level_names[] =
{
    "LOG_CRIT",
#ifdef HAVE_SINDOH_LOG_SYSTEM
    "LOG_SDH_CRIT",
    "LOG_SDH_ERR",
#endif
    "LOG_ERR",
#ifdef HAVE_SINDOH_LOG_SYSTEM
    "LOG_SDH_DEBUG",
#endif
    "LOG_WARNING",     
    "LOG_NOTICE",      
    "LOG_INFO",        
    "LOG_DEBUG",       
    "LOG_DEBUG_M",     
    "LOG_DEBUG_H",
    0
};     

static int bad_level( int l, const char * name )
{
    int i;
  
    cmd_printf("level %s is invalid : ", name );
    for ( i = 0; i < TOTAL_LOG_LEVEL && debug_logger_level_names[i] != 0; i++ ) 
    {
        cmd_printf("%s ", debug_logger_level_names[i]);
    }
    cmd_printf("\n");

    return CMD_USAGE_ERROR;
}

static int bad_module( int m, const char * name )
{
    int i;
    cmd_printf("module %s is invalid : ", name );
    for (i=0; i < 32 && debug_logger_module_idx_names[i] != 0; i++) 
    {
       cmd_printf("%s ", debug_logger_module_idx_names[i]);
    }
    cmd_printf("\n");
    return CMD_USAGE_ERROR;
}

static int bad_submodule( int m, int s, const char * name )
{
    int i;

    cmd_printf("%s submodule [%s] is invalid : ", debug_logger_module_idx_names[m], name );
    for (i=0; i < 32-5; i++) 
    {
        if ( modules_dbg_submodule_names[m][i] != 0 ) 
	{
	  cmd_printf("%s ", modules_dbg_submodule_names[m][i]);
	}
    }
    cmd_printf("\n");

    return CMD_USAGE_ERROR;
}

static int logger_find_level( const char * name )
{
    int i;

    if ( ( 0 == strncmp( "off", name, strlen("off") ) ) ) 
       return 999;

    for ( i = 0; i < TOTAL_LOG_LEVEL && debug_logger_level_names[i] != 0; i++ ) 
    {
        if ( ( 0 == strncasecmp( debug_logger_level_names[i], name, strlen(name) ) ) ) 
	{
	    return i;
	}
    }
    return -i;
}

static int logger_find_module( const char * name )
{
    int i;
    for ( i = 0; i < 32 && debug_logger_module_idx_names[i] != 0; i++ ) 
    {
        DBG_PRINTF_NOTICE("find %s %s %d %d %d %d \n", debug_logger_module_idx_names[i], name, i, 
			  strlen(debug_logger_module_idx_names[i]), strlen(name),
			  strncmp( debug_logger_module_idx_names[i], name, strlen(name) ) );
  
	if ( (0 == strncasecmp( debug_logger_module_idx_names[i], name, strlen(name) ) ) ) 
        {
	    return i;
        }
    }
    return -i;
}



static int logger_find_submodule( int module_index, const char * name )
{
    int i;

    if (0 == name)
       return -1000;

    for ( i = 0; i < 27; i++ ) 
    {
        if ( modules_dbg_submodule_names[module_index][i] 
	     && ( 0 == strncasecmp( modules_dbg_submodule_names[module_index][i], name, strlen(name) ) ) ) 
        {
            return i;
        }
    }
    return -i;
}

void logger_dump( )
{
    int m, l, i, mask;

    for (m = 0; m < 32; m ++ ) 
    {
        for (l = TOTAL_LOG_LEVEL-1; l >= 0; l--) 
        {
             if ( modules_dbg_flags[m][l] == 0xffffffe0 && debug_logger_module_idx_names[m] ) 
	     {
	         cmd_printf("logger_set_2( \"%s\", \"%s\" ); \n", 
			    debug_logger_module_idx_names[m],
			    debug_logger_level_names[l]);
		 break;
	     }
	     if ( l == 0 && modules_dbg_flags[m][l] == 0 && debug_logger_module_idx_names[m] ) 
	     {
	         cmd_printf("logger_set_2( \"%s\", \"off\" ); \n", 
			    debug_logger_module_idx_names[m] );
		 break;
	     }
	}
    }
    for (m = 0; m < 32; m ++ ) 
    {
        for (l = 0; l < TOTAL_LOG_LEVEL; l++) 
        {
	    if ( modules_dbg_flags[m][l] == 0xffffffe0 ) 
	    {
	        continue;
	    }
	    for (i = 0, mask = 1 << 5; i < 27; i++, mask <<= 1)
	    {
	        if (  modules_dbg_flags[m][l] & mask &&  
		      debug_logger_module_idx_names[m] &&
		      modules_dbg_submodule_names[m][i] ) 
		{
		    if ( l < 7 && modules_dbg_flags[m][l+1] & mask) 
		    {
		        continue;
		    }
		    cmd_printf("logger_set_3( \"%s\", \"%s\", \"%s\" ); \n", 
			       debug_logger_module_idx_names[m],
			       modules_dbg_submodule_names[m][i],
			       debug_logger_level_names[l]
			       );
		}
	    }
	}
    }
}

int logger_set_3( const char * arg1, const char * arg2, const char * arg3 )
{
    const char *module = arg1;
    const char *submodule = arg2;
    const char *level = arg3;
    
    int m = logger_find_module(module);
    int s = logger_find_submodule(m, submodule);
    int l = logger_find_level(level);
  
    DBG_PRINTF_NOTICE("%s: %s %s %s %d %d %d \n", __FUNCTION__, module, submodule, level, m, s, l ); 

    if ( l < 0 )  { // try changing order of arguments
	    module = arg3;
	    submodule = arg2;
	    level = arg1;

	    m = logger_find_module(module);
	    s = logger_find_submodule(m, submodule);
	    l = logger_find_level(level);
    }
    

    if ( m < 0 ) 
        return bad_module( m, module );
    else if ( s < 0 ) 
        return bad_submodule( m, s, submodule );
    else if ( l < 0 ) 
        return bad_level( l, level );
    else if ( l == 999 ) 
    { 
        for ( l = 0 ; l < TOTAL_LOG_LEVEL; l++ ) 
	{
	    modules_dbg_flags[m][l] &= ~LOGGER_SUBMODULE_BIT( s ) ;
	}
    }
    else 
    {
        logger_enable_module_level( m, l, LOGGER_SUBMODULE_BIT( s ) );
    }
    return CMD_OK;
}

int logger_set_2( const char * arg1, const char * arg2 )
{
    const char *module = arg1;
    const char *level = arg2;

    int m = logger_find_module(module);
    int l = logger_find_level(level);
  
    if ( l < 0 )  { // try changing order of arguments
	    module = arg2;
	    level = arg1;
	    m = logger_find_module(module);
	    l = logger_find_level(level);
    }


    if ( m < 0 ) 
        return bad_module( m, module );
    else if ( l < 0 ) 
        return bad_level( l, level );
    else if ( l == 999 ) 
        logger_clear_module(m);
    else 
        logger_enable_module_mask(m, DEBUG_MODULE_MASK | l);

    return CMD_OK;
}

int logger_set_1( const char * level )
{
    int l = logger_find_level( level );
    int i;

    if ( l < 0 ) 
    {
        return bad_level( l, level );
    } 
    else 
    {
	for (i=0; i<32; i++) 
	{
	    if ( l == 999 )
	        logger_clear_module(i);
	    else
	        logger_enable_module_mask(i, DEBUG_MODULE_MASK | l ); 
	}
    }
    return CMD_OK;
}


static void logger_print_mask( int module_index, int level_index )
{
    int i;
    uint32_t m;
    uint32_t mask = modules_dbg_flags[module_index][level_index];

    for ( i = 5, m = 1 << 5; i < 32; i++, m <<= 1 ) 
    {
        if (mask & m) 
	{
	    if ( modules_dbg_submodule_names[module_index][i-5] == 0 ) 
	    {
	        DBG_PRINTF_NOTICE("        %s %i\n", "submodule", i-5 );
	    } 
	    else 
	    {
	        cmd_printf(" %s", modules_dbg_submodule_names[module_index][i-5] );
	    }
	} 
	else if ( level_index == 0 && modules_dbg_submodule_names[module_index][i-5] )  
	{
	    cmd_printf(" -[%s]", modules_dbg_submodule_names[module_index][i-5] );
	}
    }
}

static void logger_show_1( const char * module )
{
    if ( 0 == module ) 
    {
        return;
    } 
    else 
    {
        int m = logger_find_module(module);
        int l;

        if ( m < 0 ) 
            bad_module( m, module );
        else 
        {
            cmd_printf("%s\n", debug_logger_module_idx_names[m] );
            for ( l = 0; l <= LOG_MAX_LEVEL ; l++ ) 
            {
                if ( modules_dbg_flags[m][l] == 0 ) 
                {
                    DBG_PRINTF_NOTICE("   - %s\n", debug_logger_level_names[l]);
                    break;
                } 
                else if ( modules_dbg_flags[m][l] == 0xffffffe0 ) 
                {
                    if ( l == 0 )
                    {
                        cmd_printf("   *"); 
                    }

                    if ( l < LOG_MAX_LEVEL && modules_dbg_flags[m][l+1] == 0xffffffe0 ) 
                    {
                        cmd_printf(" %s", debug_logger_level_names[l]); 
                        continue;
                    }
                    cmd_printf(" %s\n", debug_logger_level_names[l]); 
                } 
                else 
                {
                    cmd_printf("   + %s: ", debug_logger_level_names[l]);
                    logger_print_mask( m, l ); 
                    cmd_printf("\n");
                }
            }
        }
    }
}

static const char *logger_cmd_set_desc = "Set a logging level for all modules or, optionally, a module and, optionally, its submodules";
static const char *logger_cmd_set_usage = "<level> [<module>] [<submodule>]";
static const char *logger_cmd_set_notes = "Missing submodule sets all submodules, missing module set all modules to the log level provided, or \"off\" ";
static const char *logger_cmd_show_desc = "Show all modules logger levels, or optionally specify a single module to show";
static const char *logger_cmd_show_usage = "[<module>]";
static const char *logger_cmd_show_notes = "module is optional as a string";

static const char *logger_cmd_reset_desc = "resets module logging to logger_config.c defaults, dumps previous settings as program";
static const char *logger_cmd_reset_usage = NULL;
static const char *logger_cmd_reset_notes = NULL;

static const char *logger_cmd_timestamp_desc = "Enable OS tick, uSecond timestamp or disable";
static const char *logger_cmd_timestamp_usage = "<tick|usec|off>";
static const char *logger_cmd_timestamp_notes = "tick means log with the OS timer tick timestamp, usec means log with a microsecond tick timestamp, off disables timestamps when logging";

static int logger_cmd_timestamp_cb( int argc, char *argv[] )
{
    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if ( strncmp( argv[1], "usec", 4 ) == 0 )
    {
        logger_timestamp_usec = true;
        logger_timestamp_on = true;            
        cmd_append_result( "usec" );
    }
    else if ( strncmp( argv[1], "tick", 4 ) == 0 )
    {
        logger_timestamp_usec = false;
        logger_timestamp_on = true;            
        cmd_append_result( "tick" );
    }
    else if ( strncmp( argv[1], "off", 3 ) == 0 )
    {
        // off
        logger_timestamp_on = false;
        cmd_append_result( "off" );
    }
    else
    {
        // bad input
        return CMD_USAGE_ERROR;
    }

    return CMD_OK;
}

static int logger_cmd_reset_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    logger_dump();

    logger_local_init();

    return CMD_OK;
}

static int logger_cmd_set_cb( int argc, char *argv[] )
{
#if 1
    if ( argc == 2 )
        return logger_set_1( argv[1] );
    else if ( argc == 3 )
        return logger_set_2( argv[1], argv[2] );
    else if ( argc == 4 )
        return logger_set_3( argv[1], argv[2], argv[3] );
    else 
        return CMD_USAGE_ERROR;
#else
    int i;
    uint32_t level = 0;
    uint32_t mod = 4;
    uint32_t sub = 0;
    
    if ( ( argc != 2 ) && ( argc != 3 ) && ( argc != 4 ) )
    {
      return CMD_USAGE_ERROR;
    }

    /* get the user-specified level */
    if ( str_mkint(argv[1], &level) )
    {
        return CMD_USAGE_ERROR;
    }

    /* when only to arguments given then we enable the same level
     * for ALL modules */
    if ( argc == 2 )
    {
        for (i=0; i<32; i++)
        {
            logger_enable_module_mask(i, DEBUG_MODULE_MASK | level);
        }
        return CMD_OK;
    }

    /* get the user-specified module */
    if ( str_mkint(argv[2], &mod) )
    {
        return CMD_USAGE_ERROR;
    }

    if ( argc == 4 )
    {
        /* get the user-specified sub-module */
        if ( str_mkint(argv[3], &sub) )
        {
            return CMD_USAGE_ERROR;
        }
    }

    logger_enable_module_level(mod, level, sub);
    module_dbg_flags_print(mod, level);

    return CMD_OK;
#endif 
}

static int logger_cmd_show_cb( int argc, char *argv[] )
{
    uint32_t mod = 4;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    /* print only one module's levels */
    if ( argc == 2 )
    {
#if 1
      logger_show_1( argv[1] );
#else
        if ( str_mkint(argv[1], &mod) )  
        {
            return CMD_USAGE_ERROR;
        }
        if ( mod < 32)
        {
            for ( level = 0; level < TOTAL_LOG_LEVEL; level++ )
            {
                module_dbg_flags_print(mod, level);
            }
        }
#endif
    }

    /* print all modules levels */
    if ( argc == 1 )
    { 
        for ( mod = 0; mod < 32; mod++ )
        {
	    if ( debug_logger_module_idx_names[mod] != 0 ) 
	    {
	        logger_show_1( debug_logger_module_idx_names[mod] );
	    }
        }
    }

    return CMD_OK;
}


static char *slogmem_cmd_free_desc = "filter memory dump for free chunks";
static int slogmem_cmd_free_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    dbg_slog_memory_filter_cnt |= filter_free_bit;

    return CMD_OK;

}


static char *slogmem_cmd_time_desc = "filter memory dump after given time";
static int slogmem_cmd_time_cb( int argc, char *argv[] )
{
    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    dbg_slog_memory_filter_cnt |= filter_time_bit;
    dbg_slog_memory_time = atoi( argv[1] ); 

    return CMD_OK;

}

static char *slogmem_cmd_size_desc = "filter memory dump larger than size";
static int slogmem_cmd_size_cb( int argc, char *argv[] )
{
    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    dbg_slog_memory_filter_cnt |= filter_size_bit;
    dbg_slog_memory_size = atoi( argv[1] ); 

    return CMD_OK;

}


static char *slogmem_cmd_all_desc = "disable / clear filters on memory dump";
static int slogmem_cmd_all_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    dbg_slog_memory_filter_cnt = 0;
    dbg_slog_memory_file_cnt = 0;


    return CMD_OK;

}


static char *slogmem_cmd_addfile_desc = "filter memory dump for upto 20 filename ";
static int slogmem_cmd_addfile_cb( int argc, char *argv[] )
{
    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }
    if ( dbg_slog_memory_file_cnt < 20 ) {
      strncpy(&dbg_slogmem_files[dbg_slog_memory_file_cnt][0], argv[1], 32 ); 
      ++dbg_slog_memory_file_cnt;
      ++dbg_slog_memory_filter_cnt;
    }

    return CMD_OK;

}



static char *slog_cmd_dump_desc = "dump the serial log to the terminal";
static char *slog_cmd_clear_desc = "clear the serial log";
static int slog_cmd_clear_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    ClearMemStringLog();

    return CMD_OK;
}

static int slog_cmd_dump_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    DumpMemStringLog(-1);

    return CMD_OK;
}

#ifdef HAVE_NETWORK
static char *slog_cmd_ip_desc = "dump serial log to ip address";
static int slog_cmd_ip_cb( int argc, char *argv[] )
{
    uint32_t ipaddr;
    struct sockaddr_in sockaddr;
    int sock;
    int sockcode;
#define DEBUG_BASE_PORT 50310

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    ipaddr = inet_addr( argv[1] );
    if(ipaddr == -1)
    {
        cmd_printf("ERROR: invalid ip address: %s\n", argv[1]);
        return CMD_ERROR;
    }

    sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if( sock == -1 )
    {
        cmd_printf( "ERROR: failed to open socket\n" );
        return CMD_ERROR;
    }

    memset( &sockaddr, 0, sizeof(sockaddr) );
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons( DEBUG_BASE_PORT );
    sockaddr.sin_addr.s_addr = ipaddr;
    sockcode = connect( sock, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_in) );
    if( sockcode < 0 ) {
        sockcode = thread_errno;
        cmd_printf( "ERROR: connect() failed: %d\n", sockcode );
#ifdef __linux__
        close(sock);
#else                
        tfClose(sock);
#endif                
        return CMD_ERROR;
    }

    cmd_printf( "ipdump socket opened...sending..." );
    DumpMemStringLog(sock);
#ifdef __linux__
    close(sock);
#else                
    tfClose(sock);
#endif                
    cmd_printf( "done\n" );

    return CMD_OK;
}
#endif //HAVE_NETWORK

#endif

void logger_timestamp_onOff_usecTick( bool onOff, bool usecTick )
{
    logger_timestamp_on = onOff;
    logger_timestamp_usec = usecTick;
}

void logger_init()
{
    // config file
    logger_local_init();

    #ifdef HAVE_CMD
    int cmd_res = 0;
    UNUSED_VAR(cmd_res);
    /* register logger commands */
    cmd_res = cmd_register_cmd( "slog",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "slog",
                                   "clr",
                                   slog_cmd_clear_desc,
                                   NULL,
                                   NULL,
                                   slog_cmd_clear_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "slog",
                                   "dump",
                                   slog_cmd_dump_desc,
                                   NULL,
                                   NULL,
                                   slog_cmd_dump_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );

#ifdef HAVE_NETWORK
    cmd_res = cmd_register_subcmd( "slog",
                                   "ip",
                                   slog_cmd_ip_desc,
				   NULL,
				   NULL,
                                   slog_cmd_ip_cb );
#endif

    cmd_res = cmd_register_cmd( "slogmem",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "slogmem",
                                   "all",
                                   slogmem_cmd_all_desc,
                                   NULL,
                                   NULL,
                                   slogmem_cmd_all_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "slogmem",
                                   "size",
                                   slogmem_cmd_size_desc,
                                   "<bytes>",
                                   NULL,
                                   slogmem_cmd_size_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "slogmem",
                                   "time",
                                   slogmem_cmd_time_desc,
                                   "<seconds/10>",
                                   NULL,
                                   slogmem_cmd_time_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "slogmem",
                                   "free",
                                   slogmem_cmd_free_desc,
                                   NULL,
                                   NULL,
                                   slogmem_cmd_free_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "slogmem",
                                   "addfile",
                                   slogmem_cmd_addfile_desc,
                                   "<filename>",
                                   NULL,
                                   slogmem_cmd_addfile_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );


    

    cmd_res = cmd_register_cmd( "logger",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "logger",
                                   "set",
                                   logger_cmd_set_desc,
                                   logger_cmd_set_usage,
                                   logger_cmd_set_notes,
                                   logger_cmd_set_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "logger",
                                   "show",
                                   logger_cmd_show_desc,
                                   logger_cmd_show_usage,
                                   logger_cmd_show_notes,
                                   logger_cmd_show_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "logger",
                                   "timestamp",
                                   logger_cmd_timestamp_desc,
                                   logger_cmd_timestamp_usage,
                                   logger_cmd_timestamp_notes,
                                   logger_cmd_timestamp_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "logger",
                                   "reset",
                                   logger_cmd_reset_desc,
                                   logger_cmd_reset_usage,
                                   logger_cmd_reset_notes,
                                   logger_cmd_reset_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );

    #endif
}

