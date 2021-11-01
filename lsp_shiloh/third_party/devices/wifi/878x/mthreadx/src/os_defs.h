/* 
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2006
 */

#ifndef _OS_HEADER1_
#define _OS_HEADER1_

#include "tx_api.h"
#include "ATypes.h"
#include "errno.h"
#include "marvellTypeMap.h"

#define __KERNEL__

#ifndef	ETH_ALEN
#define ETH_ALEN			6
#endif

struct pt_regs {
	UINT16 temp;
};

#if 0
#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Argument list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */
#endif
#define PF_NOFREEZE     0x00008000

//-jrstypedef UINT8       u8;
//-jrstypedef UINT32      u32;
//-jrstypedef UINT16      u16;
//-jrstypedef SINT8       s8;
//-jrstypedef SINT16      s16;
//-jrstypedef SINT32      s32;
//-jrstypedef unsigned long long u64;
//-jrstypedef long long   s64;
typedef char	    *PCHAR;
//-jrstypedef UINT        uint;
typedef UINT8       *PUCHAR;
typedef UINT16	    *PUSHORT;
typedef LONG		*PLONG;
typedef UINT32	    *PULONG;
typedef VOID		*PVOID;
typedef int		    WLAN_STATUS;
//typedef UINT8		BOOLEAN;
//typedef BOOLEAN		*PBOOLEAN;
typedef PVOID		PDRIVER_OBJECT;
typedef PUCHAR		PUNICODE_STRING;
typedef long long	LONGLONG;
typedef unsigned long long	ULONGLONG;
typedef LONGLONG	*PLONGLONG;
typedef ULONGLONG	*PULONGLONG;
//-jrstypedef unsigned long ulong;
typedef PUCHAR		ANSI_STRING;
typedef ANSI_STRING	*PANSI_STRING;
typedef WCHAR		*PWCHAR;
typedef WCHAR		*LPWCH, *PWCH;
typedef WCHAR		*NWPSTR;
typedef WCHAR		*LPWSTR, *PWSTR;
//-jrstypedef long off_t;
//-jrstypedef int ssize_t;
typedef u32     atomic_t;

#ifdef __KERNEL__ //bqiu 
typedef void IRQ_RET_TYPE;
#define IRQ_RET		return
#endif /* __KERNEL__ */

#endif /* _OS_HEADER1_ */
