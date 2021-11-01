/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include "ipp_types.h"
#include "ipp_const.h"
#include "ipp_api.h"

const char* ipp_const_tag_str(ipp_u8bit_t tag)
{
#define _CASE(x) case IPP_TAG_ ## x: return #x
  switch (tag)
  {
    _CASE(OPERATION_ATTRIBUTES);
    _CASE(JOB_ATTRIBUTES);
    _CASE(END_OF_ATTRIBUTES);
    _CASE(PRINTER_ATTRIBUTES);
    _CASE(UNSUPPORTED_ATTRIBUTES);
    _CASE(UNSUPPORTED);
    _CASE(UNKNOWN);
    _CASE(NO_VALUE);
    _CASE(INTEGER);
    _CASE(BOOLEAN);
    _CASE(ENUM);
    _CASE(OCTET_STRING);
    _CASE(DATE_TIME);
    _CASE(RESOLUTION);
    _CASE(RANGE_OF_INTEGER);
    _CASE(TEXT_WITH_LANGUAGE);
    _CASE(NAME_WITH_LANGUAGE);
    _CASE(TEXT_WITHOUT_LANGUAGE);
    _CASE(NAME_WITHOUT_LANGUAGE);
    _CASE(KEYWORD);
    _CASE(URI);
    _CASE(URI_SCHEME);
    _CASE(CHARSET);
    _CASE(NATURAL_LANGUAGE);
    _CASE(MIME_MEDIA_TYPE);
  }
  return "ReSeRvEd";
#undef _CASE
}

const char* ipp_const_opid_str(ipp_u8bit_t opid)
{
#define _CASE(x) case IPP_OPID_ ## x: return #x
  switch (opid)
  {
    _CASE(PRINT_JOB);
    _CASE(PRINT_URI);
    _CASE(VALIDATE_JOB);
    _CASE(CREATE_JOB);
    _CASE(SEND_DOCUMENT);
    _CASE(SEND_URI);
    _CASE(CANCEL_JOB);
    _CASE(GET_JOB_ATTR);
    _CASE(GET_JOBS);
    _CASE(GET_PRINTER_ATTR);
    _CASE(HOLD_JOB);
    _CASE(RELEASE_JOB);
    _CASE(RESTART_JOB);
    _CASE(PAUSE_PRINTER);
    _CASE(RESUME_PRINTER);
    _CASE(PURGE_JOBS);
  }
  return "ReSeRvEd";
#undef _CASE
}

const char* ipp_const_status_str(ipp_u16bit_t status)
{
#define _CASE(x) case IPP_STAT_ ## x: return #x
  switch (status)
  {
    _CASE(OK);
    _CASE(OK_IGNORED_OR_SUBSTITUTED_ATTRIBUTES);
    _CASE(OK_CONFLICTING_ATTRIBUTES);
    _CASE(CLI_BAD_REQUEST);
    _CASE(CLI_FORBIDDEN);
    _CASE(CLI_NOT_AUTHENTICATED);
    _CASE(CLI_NOT_AUTHORIZED);
    _CASE(CLI_NOT_POSSIBLE);
    _CASE(CLI_TIMEOUT);
    _CASE(CLI_NOT_FOUND);
    _CASE(CLI_GONE);
    _CASE(CLI_REQUEST_ENTITY_TOO_LARGE);
    _CASE(CLI_REQUEST_VALUE_TOO_LONG);
    _CASE(CLI_DOCUMENT_FORMAT_NOT_SUPPORTED);
    _CASE(CLI_ATTRIBUTES_OR_VALUES_NOT_SUPPORTED);
    _CASE(CLI_URI_SCHEME_NOT_SUPPORTED);
    _CASE(CLI_CHARSET_NOT_SUPPORTED);
    _CASE(CLI_CONFLICTING_ATTRIBUTES);
    _CASE(CLI_COMPRESSION_NOT_SUPPORTED);
    _CASE(CLI_COMPRESSION_ERROR);
    _CASE(CLI_DOCUMENT_FORMAT_ERROR);
    _CASE(CLI_DOCUMENT_ACCESS_ERROR);
    _CASE(SRV_INTERNAL_ERROR);
    _CASE(SRV_OPERATION_NOT_SUPPORTED);
    _CASE(SRV_SERVICE_UNAVAILABLE);
    _CASE(SRV_VERSION_NOT_SUPPORTED);
    _CASE(SRV_DEVICE_ERROR);
    _CASE(SRV_TEMPORARY_ERROR);
    _CASE(SRV_NOT_ACCEPTING_JOBS);
    _CASE(SRV_BUSY);
    _CASE(SRV_JOB_CANCELED);
    _CASE(SRV_MULTIPLE_DOCUMENT_JOBS_NOT_SUPPORTED);
  }
  return "UnKnOwN";
#undef _CASE
}

const char* ipp_const_printer_state_str(ipp_s32bit_t state)
{
#define _CASE(x) case IPP_PRINTER_STATE_ ## x: return #x
  switch (state)
  {
    _CASE(IDLE);
    _CASE(PROCESSING);
    _CASE(STOPPED);
  }
  return "UnKnOwN";
#undef _CASE
}
