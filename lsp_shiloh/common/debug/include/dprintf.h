/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file dprintf.h
 *
 * \brief
 *
 */

#ifndef _DPRINTF_H
#define _DPRINTF_H

#include <stdarg.h>

#include "error_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/** \brief An error_type_t return value so that the sprintf
 * functions below can notify the caller if the value was capped */
#define DEBUG_STRING_CAPPED (error_type_t)-11


/**
 * \brief A version of printf that outputs to the serial port.
 * \param pFormat The printf formatter string.
 * \param ... The variable argument list.
 * \retval The number of bytes in the output string
 */
int dbg_printf( const char *pFormat, ... );

/**
 * \brief A shortened version of the standard sprintf.
 * The purpose of this routine is to output data the same as the standard printf 
 * function without the overhead most run-time libraries involve. Usually the 
 * printf brings in many kilobytes of code and that is unacceptable in most 
 * embedded systems.  This one only supports integer outputs, floating point
 * has been removed.
 * \param[in] buf The location to put the output string.  If NULL output to 
 *                  the serial port.
 * \param[in] ctrl A standard printf formatting string.  
 * \param[in] ... The variable paramater list
 * \returns uint 
 * \retval The number of bytes in the output string
 */
unsigned int minSprintf(char* buf,  const char* ctrl, ...);

/**
 * \brief similar to minSprintf but takes a va_list for the variables.
 * \param[in] buf The location to put the output string.  If NULL output to 
 *                  the serial port.
 * \param[in] ctrl A standard printf formatting string.  
 * \param argp variable list pointer initialized by the va_start function 
 * \returns uint 
 * \retval The number of bytes in the output string
 */
unsigned int minVsprintf(char* buf, const char* ctrl, va_list argp);

/**
 * \brief similar to minSprintf but limits the length of the output string to size.
 * 
 * 
 * \param[in] buf The location to put the output string.  If NULL output to 
 *                  the serial port.
 * \param[in] size The maximum string length, the size of the destination buffer
 * \param[in] ctrl A standard printf formatting string.  
 * \param[in] ... The variable paramater list
 * \returns uint 
 * \retval The number of bytes in the output string
 */
unsigned int minNSprintf(char* buf, int size,  const char* ctrl, ...);

/**
 * \brief similar to minSprintf but limits the length of the output string 
 * to size and takes a va_list for the arguments.
 * 
 * 
 * \param[in] buf The location to put the output string.  If NULL output to 
 *                  the serial port.
 * \param[in] size The maximum string length, the size of the destination buffer
 * \param[in] ctrl A standard printf formatting string.  
 * \param[in] ... The variable paramater list
 * \returns uint 
 * \retval The number of bytes in the output string
 */
unsigned int minNVsprintf(char* buf, int size, const char* ctrl, va_list argp);

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
 * \param ... A variable argument list
 *
 * \return error_type_t OK or DEBUG_STRING_CAPPED if the ctrl format string
 * output resulted in a string larger than "size", the maximum string length
 *
 * The only difference between this function and the minNSprintf() function
 * is the ability to know that the ctrl string had to be capped when formatting
 * to the destination buffer.
 *
 */
error_type_t min_n_k_sprintf(char* buf,
                         int size,  
                         unsigned int * stringlen,
                         const char* ctrl,
                         ...);
                         
/**
 * \brief A version of snvprintf that returns an error code so we can know
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
 * \param argp variable list pointer initialized by the va_start function 
 *
 * \return error_type_t OK or DEBUG_STRING_CAPPED if the ctrl format string
 * output resulted in a string larger than "size", the maximum string length
 *
 */
error_type_t min_n_k_v_sprintf(char* buf, 
                          int size,  
                          unsigned int * stringlen,
                          const char* ctrl, 
                          va_list argp);                 

#ifdef __cplusplus
}
#endif

#endif // _DPRINTF_H

