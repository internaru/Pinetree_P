/*
 *
 * =====================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * =====================================================================
 *
 */

/** 
 * \file print_job_api_restricted.h
 * 
 * \brief Restricted print job manipulations utilities for use by 
 * generators. 
 * 
 **/

#ifndef PRINT_JOB_API_RESTRICTED_H
#define PRINT_JOB_API_RESTRICTED_H

#include <stdint.h>
#include <stdbool.h>
#include "print_job_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \brief Handles the backside/frontside auto-duplex flag settings.  
 * 
 * \param page - pointer to the page 
 *  
 * \param current_page_number - the current page count
 * 
 * \return bool - true if this is the first page on the sheet; else false.
 *  
 * This is the implementation of the auto-duplex handling code.  It is exported 
 * as an api routine for use by the generators.  Some generators (such as the 
 * n-up generator) need to discard and create new page.  Thus they must handle 
 * resetting the duplex values for the pages they are handling.  To avoid duplication 
 * of the duplex code this api was exported.  However, it should NEVER be used by 
 * parsers or applications.  It should only be used by generators and internally 
 * by the print_job_api itself. 
 *
 **/
bool print_page_handle_autoduplex_restricted(PAGE_DATA* page_data, uint32_t current_page_number, bool duplex_rotate_backside);


#ifdef __cplusplus
}
#endif

#endif //PRINT_JOB_API_RESTRICTED_H


