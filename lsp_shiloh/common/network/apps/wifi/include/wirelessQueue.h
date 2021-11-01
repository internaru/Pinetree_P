/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 
#ifndef WIRELESS_QUEUE_H
#define WIRELESS_QUEUE_H

// This file exists only to get the queue lenght define included into both WirelessDrvr.c and RawSocket.c
// There were many naming conflicts if I tried to include WirelessDrvr.h into the RawSocket.c so this
// was the quickest solution. -jrs

#define MAX_PKTS 40

#endif
