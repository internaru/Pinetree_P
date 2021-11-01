/*
 **************************************************************************************
 *
 * Copyright (c) 2013 Marvell International, Ltd.
 *
 **************************************************************************************
 *
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell as part of a proprietary software release,
 * the File is considered Marvell Proprietary and Confidential Information, and is
 * licensed to you under the terms of the applicable Commercial License.
 *
 **************************************************************************************
 *
 * Marvell GPL License Option
 *
 * If you received this File from Marvell as part of a Linux distribution, this File
 * is licensed to you in accordance with the terms and conditions of the General Public
 * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
 * modify it under the terms of the GPL License; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GPL License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 *
 **************************************************************************************
*/

/*
 * \file lassert.h
 * 
 * \brief The assert macro puts diagnostics into programs.
 * 
 * When it is executed, if its argument expression is false, it writes 
 * information about the call that failed (including the text of the argument,
 * the name of the source file, and the source line number - the latter 
 * are respectively the values of the preprocessing macros __FILE__ and __LINE__) 
 * on the standard error stream. It then calls the abort function.If its 
 * argument expression is true, the assert macro returns no value.
 * 
 */

#ifndef _ASSERT_H
#define _ASSERT_H
     void _assert(const char *, const char *, int);
     void XAssertFail(const char *, const char *, int, unsigned long int);
#ifdef DEBUG
#    define DBG_ASSERT(e) ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__))
#    define ASSERT(e) ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__))
#    define XASSERT(e,errvalue) ((e) ? (void)0 : XAssertFail(#e, __FILE__,__LINE__,(errvalue)))
#else
#  define DBG_ASSERT(ignore) ((void)0)
#  define ASSERT(ignore) ((void)0)
#  define XASSERT(ignore,errvalue) ((void)0)
#endif

typedef void (*assertfunc)(void);
extern int atassert( assertfunc f );

#endif

