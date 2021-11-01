/*
 * timerfd_create() / timerfd_settime() / timerfd_gettime() for uClibc
 *
 * Copyright (C) 2009 Stephan Raue <step...@openelec.tv>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */
#include <unistd.h>
#include <sys/syscall.h>
#include "timerfd.h"

/*
 * timerfd_create()
 */
//#define SYS__timerfd_create __NR_timerfd_create
//#ifdef __NR_timerfd_create
//_syscall2(int, timerfd_create, int, clockid, int, flags)
//#endif

//__NR_timerfd_create
#if 0
#define __NR_timerfd_create 350
_syscall2(int,timerfd_settime, int, ufd, int, flags, const struct itimerspec *, utmr, struct itimerspec *, otmr)
//__SYSCALL(__NR_timerfd_create, sys_timerfd_create)
#endif
#if 1
int timerfd_create (clockid_t __clock_id, int __flags)
{
  return syscall( 350, __clock_id, __flags);
}
#endif
#if 0
/*
 * timerfd_settime()
 */
#define __NR_timerfd_settime 353
#ifdef __NR_timerfd_settime
_syscall4(int,timerfd_settime, int, ufd, int, flags, const struct itimerspec *, utmr, struct itimerspec *, otmr)
#endif
#endif
#if 1 
int timerfd_settime( int ufd, int flags, const struct itimerspec * utmr, struct itimerspec * otmr)
{
  return syscall( 353, ufd, flags, utmr, otmr );
}
#endif
/*
 * timerfd_gettime()
 */
#if 0
#ifdef __NR_timerfd_gettime
_syscall2(int, timerfd_gettime, int, ufd, struct itimerspec *, otmr)
#endif
#endif
