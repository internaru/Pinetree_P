/*
 * wpa_supplicant/hostapd / Empty OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file can be used as a starting point when adding a new OS target. The
 * functions here do not really work as-is since they are just empty or only
 * return an error value. os_internal.c can be used as another starting point
 * or reference since it has example implementation of many of these functions.
 */

#include <stdlib.h>
#include "tx_api.h"

#include "includes.h"

#include "os.h"

#include <string.h>
//#include "agmemory.h"
#include "memAPI.h"

#include "lassert.h"
#include "utils.h"

#include "platform.h"  
#include "dprintf.h"
#include "cpu_api.h"

//UINT sysClkRateGet( void );

void os_sleep(os_time_t sec, os_time_t usec)
{
    //int ticksPerSecond = sysClkRateGet();
    int ticksPerSecond = 100;
    int usPerTick      = 1000000 / ticksPerSecond;

    tx_thread_sleep(sec * ticksPerSecond + (usec / usPerTick));
    //Delay(usec % usPerTick);
}


int os_get_time(struct os_time *t)
{
    ULONG curTime = tx_time_get();
    //int ticksPerSecond = sysClkRateGet();
    int ticksPerSecond = 100;
    int usPerTick      = 1000000 / ticksPerSecond;

    // Note: We don't really have any concept of what time/day it really is.
    // We just pass back ticks from the only 'epoch' we have: time since bootup.
    t->sec  =  curTime / ticksPerSecond;
    t->usec = (curTime % ticksPerSecond) * usPerTick;

	return 0;
}

/*
int os_mktime(int year, int month, int day, int hour, int min, int sec,
	      os_time_t *t)
{
	return -1;
}
*/

int os_daemonize(const char *pid_file)
{
    ASSERT("????: I don't think we should call os_daemonize" == NULL);
	return -1;
}


void os_daemonize_terminate(const char *pid_file)
{
    ASSERT("????: I don't think we should call os_daemonize_terminate" == NULL);
}


int os_get_random(unsigned char *buf, size_t len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        buf[i] = (unsigned char)(rand() % 256);
    }

    return 0;
}


unsigned long os_random(void)
{
	return rand();
}

/*
char * os_rel2abs_path(const char *rel_path)
{
	return NULL;
}


int os_program_init(void)
{
	return 0;
}


void os_program_deinit(void)
{
}


int os_setenv(const char *name, const char *value, int overwrite)
{
	return -1;
}


int os_unsetenv(const char *name)
{
	return -1;
}


char * os_readfile(const char *name, size_t *len)
{
	return NULL;
}
*/

void * os_zalloc(size_t size)
{
    void *newBuf = os_malloc(size);
    if (newBuf) os_memset(newBuf, 0, size);
    return newBuf;
}

#ifdef OS_NO_C_LIB_DEFINES
void * os_malloc(size_t size)
{
    ASSERT(size > 0);
	return MEM_MALLOC((UINT32)size);
}


void * os_realloc(void *ptr, size_t size)
{
    if (size == 0)
    {
        os_free(ptr);
        return NULL;
    }

    if (ptr == NULL)
        return os_malloc(size);

	return MEM_REALLOC(ptr, (UINT32)size);
}


void os_free(void *ptr)
{
    if (ptr != NULL)
        MEM_FREE_AND_NULL(ptr);
}


void * os_memcpy(void *dest, const void *src, size_t n)
{
	return memcpy(dest, src, n);
}


void * os_memmove(void *dest, const void *src, size_t n)
{
	return memmove(dest, src, n);
}


void * os_memset(void *s, int c, size_t n)
{
	return memset(s, c, n);
}


int os_memcmp(const void *s1, const void *s2, size_t n)
{
	return memcmp(s1, s2, n);
}


char * os_strdup(const char *s)
{
    char *result; 
    result = (char*)os_malloc(strlen(s) + 1);
    if (result == (char*)0)
        return (char*)0;
    strcpy(result, s);
    return result;
}


size_t os_strlen(const char *s)
{
	return strlen(s);
}


int os_strcasecmp(const char *s1, const char *s2)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strcmp(s1, s2);
}


int os_strncasecmp(const char *s1, const char *s2, size_t n)
{
	/*
	 * Ignoring case is not required for main functionality, so just use
	 * the case sensitive version of the function.
	 */
	return os_strncmp(s1, s2, n);
}


char * os_strchr(const char *s, int c)
{
	return strchr(s, c);
}


char * os_strrchr(const char *s, int c)
{
	return strrchr(s, c);
}


int os_strcmp(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}


int os_strncmp(const char *s1, const char *s2, size_t n)
{
	return strncmp(s1, s2, n);
}


char * os_strncpy(char *dest, const char *src, size_t n)
{
	return strncpy(dest, src, n);
}


char * os_strstr(const char *haystack, const char *needle)
{
	return strstr(haystack, needle);
}


int sup_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
#if 1
    unsigned int stringlen;
    return min_n_k_v_sprintf(str, size, &stringlen, (char *) format, ap);
#else
    return minNVsprintf(str, size,(char *) format, ap);
#endif
}

int os_snprintf(char *str, size_t size, const char *format, ...)
{
    va_list ap;
	int ret;

	va_start(ap, format);
	ret = sup_vsnprintf(str, size, format, ap);
	va_end(ap);
	if (size > 0)
		str[size - 1] = '\0';
	return ret;
}

#endif /* OS_NO_C_LIB_DEFINES */
