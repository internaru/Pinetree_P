/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_CONTEXT_H__
#define __INC_IPP_CONTEXT_H__

#include "ipp.h"
#include "ipp_attribute.h"

typedef enum
{
   URI_AUTH_NONE = 0,
   URI_AUTH_REQUESTED_USER_NAME,
   URI_AUTH_BASIC,
   URI_AUTH_DIGEST,
   URI_AUTH_CERTIFICATE,

   // Keep this as the last value.
   NUM_URI_AUTHS,

} uri_authentication_e;

struct ipp_request_s;

typedef struct ipp_context_t
{
  unsigned int       magic1;
  char              *protocol;
  char              *server_address;
  char              *root_uri;
  unsigned int       port_number;
  const char        *uri;
  ipp_reader_t       reader;
  void*              reader_data;
  ipp_attr_groups_t *reqattr;
  ipp_attr_groups_t *respattr;
  unsigned int       magic2;

  // rdj ADDED 15-DEC-2012
  struct ipp_request_s *ipp_req;
  
  uri_authentication_e uri_authentication;

} ipp_cntxt_t;

#endif /* __INC_IPP_CONTEXT_H__ */
