/* 
 *
 * ============================================================================
 * Copyright (c) 2012  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifndef INC_HTTP_OID_LUT_CONFIG_H
#define INC_HTTP_OID_LUT_CONFIG_H

#include "http_parse.h"

#ifdef __cplusplus
extern "C" {
#endif

void updateOidLink(HttpPageContext* context, char* value);
bool createOidLinks( HttpPageContext* context, oid_t oid, uint32_t index );

#endif


