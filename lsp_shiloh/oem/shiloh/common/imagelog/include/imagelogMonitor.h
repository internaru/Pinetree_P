/**
 *
 * ============================================================================
 * Copyright (c) 2015 SINDOH, Inc. All Rights Reserved 
 *
 *                         SINDOH Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file imagelog_monitor.c
 * \brief the source code of image log monitor.
 * To handle imagelog sending and take action for setting change
 */
#ifndef _IMAGE_LOG_MONITOR_H
#define _IMAGE_LOG_MONITOR_H

#include <stdio.h>

typedef enum {
	IL_NOTIFY_SYS_INIT = 1,
	IL_NOTIFY_JOB_END = 2,
	IL_NOTIFY_CONFIG_CHANGED = 3,
	IL_NOTIFY_SEND_NEXT = 4,
} IMAGE_LOG_NOTIFY_TYPE;

int ImageLogMonitorInit();


#endif // _IMAGE_LOG_MONITOR_H

