/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
#ifndef __FAX_SVC_MODEM_H__ 
#define __FAX_SVC_MODEM_H__ 

#include "fax_modem_status.h"

void fax_modem_status_initialize(void);

void fax_modem_status_get(fax_modem_status_data_t *status);

void fax_modem_status_update(fax_modem_status_data_t *status);

void fax_modem_status_set_caller_id(char *caller_id);

void fax_modem_status_clear_caller_id(void);

#endif
