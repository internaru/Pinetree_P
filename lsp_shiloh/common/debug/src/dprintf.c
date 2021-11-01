/***********************************************************
* (c) Portions copyright 2008-2010 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <posix_ostools.h>
#include "ATypes.h"
#include "dprintf.h"
#include "lassert.h"
#include "strfmt.h"
#include "dbg_outbuf.h"

// uint32_t enabled_module_flags=0xffffffff;


typedef struct
{
  char*    buf;
  int      size;
} _nsprintf_t;

static void _nsprintf(char c, void* data)
{
  _nsprintf_t * d = (_nsprintf_t*)data;
  if (d->size > 0)
  {
    if (d->buf) *(d->buf++) = c;
    else        dbg_printf("%c", c);
  }
  d->size -= 1;
}

#ifdef HAVE_TIMER
static int _addmsg(_nsprintf_t* d, const char* fmt, ...)
{
  int n;
  va_list ap;
  va_start(ap, fmt);
  n = vfstrfmt(_nsprintf, d, fmt, ap);
  va_end(ap);
  return n;
}
#endif

extern bool logger_get_timestamp_on( void );
extern bool logger_get_timestamp_usec( void );

error_type_t minAsprintf(char* buf, 
                          int size,  
                          unsigned int * stringlen,
                          const char* ctrl, 
                          va_list ap)
{
  int r = 0;
  _nsprintf_t d;
  d.buf = buf;
  d.size = size?size:0x7fffffff;

  if (!buf)
  {
     // if buf is null we are going to the terminal.  In this case
     // we want the mutex.  Otherwise it is going to a string
     // and there will not be a conflict.
     //
     dbg_outbuf_mutex(1);
  }

#ifdef HAVE_TIMER
#include <stdbool.h>
#include "timer_api.h"
  {
    if ( logger_get_timestamp_on() && size == 0 && buf == 0 )
    {
      r = _addmsg( &d,
                   "%u: ",
                   logger_get_timestamp_usec() ? timer_get_time_usec() : posix_gettime_ms());
    }
  }
#endif
  r += vfstrfmt(_nsprintf, &d, ctrl, ap);
  if (!buf)
  {
    dbg_outbuf_mutex(0);
  }
  if (r < 0) return OK-1;
  if (buf) _nsprintf(0, &d);     /* Null terminate string */
  if (stringlen) *stringlen = r; /* Capture the formatted length */
  if (d.size >= 0) return OK;
  else             return DEBUG_STRING_CAPPED;
}

unsigned int minSprintf(char* buf,  
                        const char* ctrl, 
                        ...)
{
    va_list ap;
    unsigned int count;

    //
    // Get the variable argument list and pass it to sprintf.
    va_start( ap, 
              ctrl );
    // Need to use minSprintf to do actual format and print of string
    count = minVsprintf( buf, 
                         ctrl, 
                         ap );
    va_end( ap );

    return count;
}

unsigned int minNSprintf(char* buf,
                         int size,  
                         const char* ctrl,
                         ...)
{
    va_list ap;
    unsigned int count;

    //
    // Get the variable argument list and pass it to sprintf.
    va_start( ap, ctrl );
    // Need to use minSprintf to do actual format and print of string
    count = minNVsprintf( buf, 
                          size, 
                          ctrl, 
                          ap );
    va_end( ap );

    return count;
}


unsigned int minVsprintf(char* buf,  
                         const char* ctrl, 
                         va_list argp)
{
    unsigned int count;

    minAsprintf(buf, 0,&count, ctrl,argp);
    return count;
}

/**
 * \brief A sprintf that limits the size of the output string
 * \param[in] buf If NULL outputs to the serial port, otherwise a pointer to where 
 *                  the string is to be put
 * \param[in] size The size limit of the output string.  If this is 0 there is no limit.
 * \param[in} ctrl The format string
 * \param[in] argp A list of the other parameters
 */

unsigned int minNVsprintf(char* buf, 
                          int size,  
                          const char* ctrl, 
                          va_list argp)
{
    unsigned int count;
    minAsprintf(buf, size, &count,ctrl, argp);
    return count;
}



/**
 * \brief A version of snprintf that returns an error code so we can know
 * if the string was capped to the "n" count.
 *
 * \param buf The destination buffer for the string
 *
 * \param size The maximum string length, the size of the destination buffer
 *
 * \param stringlen A pointer to a location capable of holding an unsigned int.
 * The value will be filled to represent the length of the string filled in
 * the destination buffer.
 *
 * \param ctrl The dprintf format string
 *
 * \return error_type_t OK or DEBUG_STRING_CAPPED if the ctrl format string
 * output resulted in a string larger than "size", the maximum string length
 *
 */
error_type_t min_n_k_sprintf(char* buf,
                         int size,  
                         unsigned int * stringlen,
                         const char* ctrl,
                         ...)
{
    va_list ap;
    error_type_t e_res;

    ASSERT( NULL != stringlen );
    ASSERT( NULL != buf );
    ASSERT( 0 != size );

    // Get the variable argument list and pass it to sprintf.
    va_start( ap, ctrl );
    // Need to use minSprintf to do actual format and print of string
    e_res = min_n_k_v_sprintf( buf, 
                          size, 
                          stringlen,
                          ctrl,
                          ap );
    va_end( ap );

    return e_res;
}


error_type_t min_n_k_v_sprintf(char* buf, 
                          int size,  
                          unsigned int * stringlen,
                          const char* ctrl, 
                          va_list argp)
{
    return minAsprintf(buf,size,stringlen,ctrl,argp);
}

/*
 * End of dprintf
 */

