/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 * \file strfmt.h
 *
 * \brief This module performs sprintf style string formatting.
 *        Its "value add" is that it does it, generically, to an output
 *        stream implemented via function pointer. Due to this design it can
 *        be the generalized workhorse behind sprintf, snprintf, fprintf,
 *        vprintf, asprintf, etc.
 **/

#ifndef __INC_STRFMT_H__
#define __INC_STRFMT_H__

#include "stdarg.h"

/**
 * \brief Performs a sprintf-style conversion
 *
 * \param[in] outch The output handling function
 * \param[in] data  Passed back to outch as the data argument
 * \param[in] fmt   The format string
 * \param[in] ap    The stdarg-style variable argument list
 * \retval            The number of output bytes produced or -1 in the case of
 *                    a formatting error.
 *
 * Note that the output stream is NOT 0 terminated. This is correct.
 *
 * TBD: Exact details as to which format conversions are supported. For now
 *      you will need to see the source code.
 */
int vfstrfmt(void (*outch)(char, void* data), void* data,
             const char* fmt, va_list ap);

/**
 * \brief A replacement for sprintf
 *
 * \param[in] str  The output location. Should be long enough to hold the
 *                 entirely formatted output.
 * \param[in] fmt  The format string
 * \param[in] ...  The variable list of arguments as required by fmt
 * \retval         The number of output bytes produced or -1 in the case of
 *                 a formatting error.
 *
 * Note that the output stream is 0 terminated (as in sprintf) except in the
 * case of error (when retval is <0).
 */
int strfmt(char* str, const char* fmt, ...) __attribute__ ((format (printf,2,3)));

#endif
