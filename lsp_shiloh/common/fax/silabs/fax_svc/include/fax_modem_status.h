/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/**\file fax_modem_status.h
 * The file defines the fax modem status API.  This is impemented using the observer
 * API.
 */
#ifndef __FAX_MODEM_STATUS_H__
#define __FAX_MODEM_STATUS_H__ 

#include "event_observer.h"

#define FAX_MAX_CID_NAME_SIZE (25)
#define FAX_MAX_CID_NUMBER_SIZE (25) 

typedef struct
{
  bool modem_initialized; /* modem is up and working */
  bool line_present;      /* line is valid and present */
  bool line_off_hook;     /* line is off hook */
  bool ext_off_hook;      /* extension is off hook */
  bool line_ringing;      /* ringing */
} fax_modem_status_data_t; 

#define FAX_ModemStatus_ID 0x432 /* uid? */
void fax_AttachObserver_modemStatus(Observer_t *o);
void fax_DetachObserver_modemStatus(Observer_t *o);
/* get info for given notification */
void fax_modem_get_status_info(Subject_t *s, fax_modem_status_data_t *data);
/* returns true if caller id has been set, copies caller id into buffers */
bool fax_modem_get_status_callerID(Subject_t *s, char *name, int name_buf_size, 
                                   char *number, int num_buf_size);

bool fax_modem_get_status_callerID_noCheck(char *name, int name_buf_size,
                                   char *number, int num_buf_size);
#endif
