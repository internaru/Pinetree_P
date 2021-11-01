/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef XASSERT_H
#define XASSERT_H

#define ASSERT(x)  ((x)?(void)0:AssertFail(#x,__FILE__,__LINE__))
#define XASSERT(x,data)  ((x)?(void)0:XAssertFail(#x,__FILE__,__LINE__,(data)))

#ifdef __cplusplus
extern "C" {
#endif

void AssertFail( char *assertmsg, char *filename, int linenumber );
void XAssertFail( char *assertmsg, char *filename, int linenumber, unsigned long int data );

#ifdef __cplusplus
}
#endif

#endif

