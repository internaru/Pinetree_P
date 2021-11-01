/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_REQUEST_H__
#define __INC_IPP_REQUEST_H__

#include "ipp_cntxt.h"
#include "ipp_request.h"

int ipp_request(ipp_cntxt_t* context);
int ipp_build_response(ipp_request_t *ipp_req);

error_type_t ipp_get_network_ip_address_from_connection(
                             uint32_t  index,
                             char     *data_ptr,
                             uint32_t  max_string_len_in_bytes );

#endif /* __INC_IPP_REQUEST_H__ */
