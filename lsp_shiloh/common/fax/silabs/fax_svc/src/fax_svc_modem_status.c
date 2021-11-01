/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

#include <string.h>
#include <dprintf.h>
#include "logger.h"
#include "fax_modem_status.h"
#include "response_subject.h"
#include "lassert.h"

typedef struct 
{
    Response_Subject_T_BaseClassMembers;
    fax_modem_status_data_t data; 
    bool cid_set;
    /* The maximum message length is 255 bytes (the message length field is one byte). 
       The maximum length for the Calling Name parameter is 21 characters; the receiver 
       only needs to display the first 15 characters. The Calling Number field can be up
       to 24 digits; the receiver must display at least the last 10 digits.  */
    char cid_name[FAX_MAX_CID_NAME_SIZE];
    char cid_number[FAX_MAX_CID_NUMBER_SIZE];
} modem_status_t;

/*---------------------------------------*/
void fax_AttachObserver_modemStatus(Observer_t *o)
{
}

/*---------------------------------------*/
void fax_DetachObserver_modemStatus(Observer_t *o)
{
}

/*---------------------------------------*/
void fax_modem_get_status_info(Subject_t *s, fax_modem_status_data_t *data)
{
}

/*---------------------------------------*/
bool fax_modem_get_status_callerID(Subject_t *s, char *name, int name_buf_size, 
                                   char *number, int num_buf_size)
{
    return(true);
}

/*---------------------------------------*/
bool fax_modem_get_status_callerID_noCheck(char *name, int name_buf_size, char *number, int num_buf_size)
{
        return (true);
}

/***************************************************************
* Internal functions
****************************************************************/

void fax_modem_status_initialize(void)
{
}

void fax_modem_status_get(fax_modem_status_data_t *status)
{
}

void fax_modem_status_update(fax_modem_status_data_t *status)
{
}

/* Format of caller id string passed by modem:
     "DATE=
     TIME=
     NMBR=
     NAME=
     MESG="

   We only care about name and number.
*/
void fax_modem_status_set_caller_id(char *caller_id)
{
}

void fax_modem_status_clear_caller_id(void)
{
}
