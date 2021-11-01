/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/**\file fax_dbg_net.h
 * The file defines the fax debug network status API.  
 */
#ifndef __FAX_DBG_NET_H__
#define __FAX_DBG_NET_H__

#include "fax_app_status.h"
#include "faxtest.h"
#include "faxtest_msgs.h"

#define FAX_DBG_NET_MSG_LEN  (4)

int fax_dbg_net_open(char *ipaddr);
int fax_dbg_net_close(void);
int fax_dbg_net_log_fax_app_state(fax_app_state_t state);
int fax_dbg_net_log_fax_app_fax_sub_state(fax_app_fax_sub_state_t state);
int fax_dbg_net_log_fax_host_status(fax_host_status_t status);

#endif

