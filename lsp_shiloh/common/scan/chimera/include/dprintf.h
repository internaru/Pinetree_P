/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef DPRINTF_H
#define DPRINTF_H

#include <stdarg.h>

/**
 * \brief A version of printf that outputs to the serial port.
 * \param pFormat The printf formatter string.
 * \param ... The variable argument list.
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
int minSprintf(char* buf,  const char* ctrl, ...);

/**
 * \brief similar to minSprintf but takes a va_list for the variables.
 * \param[in] buf The location to put the output string.  If NULL output to 
 *                  the serial port.
 * \param[in] ctrl A standard printf formatting string.  
 * \param argp variable list pointer initialized by the va_start function 
 * \returns uint 
 * \retval The number of bytes in the output string
 */
int minVsprintf(char* buf, const char* ctrl, va_list argp);

#endif

