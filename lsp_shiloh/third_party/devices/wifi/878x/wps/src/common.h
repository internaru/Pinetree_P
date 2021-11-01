/*
 * wpa_supplicant/hostapd / common helper functions, etc.
 *
 * Copyright © 2002-2006, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * Copyright © 2006-2007, Marvell International Ltd. and its affiliates
 * All rights reserved.
 *
 * 1. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the 
 * documentation and/or other materials provided with the distribution.
 * 2. Neither the name of Jouni Malinen, Marvell nor the names of its 
 * contributors may be used to endorse or promote products derived from 
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS  INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

/*
 * Change log:
 *   Marvell  03/21/07: remove unused definitions
 */

#ifndef COMMON_H
#define COMMON_H

#include "lassert.h"
//#include "agmemory.h"
#include "memAPI.h"
#define STDIN_FILENO 0

#include "dprintf.h"
#undef vprintf
#undef printf
#ifdef DBG_PRINTF
#define vprintf(fmt, args) minVsprintf(NULL, fmt, args)
#define printf dprintf
#else
#define vprintf(fmt, args)
#define printf(x)
#endif // DBG_PRINTF

#define perror(x) 

/* Macros for handling unaligned 16-bit variables */
#define WPA_GET_BE16(a) ((u16) (((a)[0] << 8) | (a)[1]))
#define WPA_PUT_BE16(a, val)			\
	do {					\
		(a)[0] = ((u16) (val)) >> 8;	\
		(a)[1] = ((u16) (val)) & 0xff;	\
	} while (0)

#define WPA_GET_LE16(a) ((u16) (((a)[1] << 8) | (a)[0]))
#define WPA_PUT_LE16(a, val)			\
	do {					\
		(a)[1] = ((u16) (val)) >> 8;	\
		(a)[0] = ((u16) (val)) & 0xff;	\
	} while (0)

#define WPA_GET_BE24(a) ((((u32) (a)[0]) << 16) | (((u32) (a)[1]) << 8) | \
			 ((u32) (a)[2]))
#define WPA_PUT_BE24(a, val)				\
	do {						\
		(a)[0] = (u8) (((u32) (val)) >> 16);	\
		(a)[1] = (u8) (((u32) (val)) >> 8);	\
		(a)[2] = (u8) (((u32) (val)) & 0xff);	\
	} while (0)

#define WPA_GET_BE32(a) ((((u32) (a)[0]) << 24) | (((u32) (a)[1]) << 16) | \
			 (((u32) (a)[2]) << 8) | ((u32) (a)[3]))
#define WPA_PUT_BE32(a, val)				\
	do {						\
		(a)[0] = (u8) (((u32) (val)) >> 24);	\
		(a)[1] = (u8) (((u32) (val)) >> 16);	\
		(a)[2] = (u8) (((u32) (val)) >> 8);	\
		(a)[3] = (u8) (((u32) (val)) & 0xff);	\
	} while (0)

#define WPA_PUT_BE64(a, val)				\
	do {						\
		(a)[0] = (u8) (((u64) (val)) >> 56);	\
		(a)[1] = (u8) (((u64) (val)) >> 48);	\
		(a)[2] = (u8) (((u64) (val)) >> 40);	\
		(a)[3] = (u8) (((u64) (val)) >> 32);	\
		(a)[4] = (u8) (((u64) (val)) >> 24);	\
		(a)[5] = (u8) (((u64) (val)) >> 16);	\
		(a)[6] = (u8) (((u64) (val)) >> 8);	\
		(a)[7] = (u8) (((u64) (val)) & 0xff);	\
	} while (0)


#ifndef os_malloc
#define os_malloc(s) MEM_MALLOC((s))
#endif
#ifndef os_free
#define os_free(p) MEM_FREE_AND_NULL((p))
#endif
#ifndef os_realloc
#define os_realloc(p, s) MEM_REALLOC((p), (s))
#endif
#ifndef os_memcpy
#define os_memcpy(d, s, n) memcpy((d), (s), (n))
#endif
#ifndef os_memmove
#define os_memmove(d, s, n) memmove((d), (s), (n))
#endif
#ifndef os_memset
#define os_memset(s, c, n) memset(s, c, n)
#endif
#ifndef os_memcmp
#define os_memcmp(s1, s2, n) memcmp((s1), (s2), (n))
#endif

#ifndef ENTER
//#if DBG_PRINTF
#if 0
#define ENTER()	    dprintf("Enter: %s, %s:%i\n", __FUNCTION__, __FILE__, __LINE__)
#define LEAVE()	    dprintf("Leave: %s, %s:%i\n", __FUNCTION__, __FILE__, __LINE__)
#else
#define ENTER()
#define LEAVE()
#endif
#endif // ENTER

#ifndef os_get_time
#define os_get_time(t) do_gettimeofday((t))
#endif

#define timercmp(tvp, uvp, cmp) \
        (((tvp)->tv_sec cmp (uvp)->tv_sec) || \
         (((tvp)->tv_sec == (uvp)->tv_sec) && ((tvp)->tv_usec cmp (uvp)->tv_usec)))

#define timersub(a, b, result)                                     \
  do {                                                             \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;                  \
    if ((a)->tv_usec >= (b)->tv_usec)                              \
      (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;             \
    else {                                                         \
      ((result)->tv_sec)--;                                        \
      (result)->tv_usec = ((a)->tv_usec + 1000000) - (b)->tv_usec; \
    }                                                              \
  } while (0)

#endif /* COMMON_H */
