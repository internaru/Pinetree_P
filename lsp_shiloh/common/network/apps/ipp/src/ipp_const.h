/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_CONST_H__
#define __INC_IPP_CONST_H__

#include "ipp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IPP_TAG_OPERATION_ATTRIBUTES   ((ipp_u8bit_t)0x01)
#define IPP_TAG_JOB_ATTRIBUTES         ((ipp_u8bit_t)0x02)
#define IPP_TAG_END_OF_ATTRIBUTES      ((ipp_u8bit_t)0x03)
#define IPP_TAG_PRINTER_ATTRIBUTES     ((ipp_u8bit_t)0x04)
#define IPP_TAG_UNSUPPORTED_ATTRIBUTES ((ipp_u8bit_t)0x05)
#define IPP_TAG_UNSUPPORTED            ((ipp_u8bit_t)0x10)
#define IPP_TAG_UNKNOWN                ((ipp_u8bit_t)0x12)
#define IPP_TAG_NO_VALUE               ((ipp_u8bit_t)0x13)
// rdj moved to ipp_api.h
//#define IPP_TAG_INTEGER                ((ipp_u8bit_t)0x21)
#define IPP_TAG_BOOLEAN                ((ipp_u8bit_t)0x22)
#define IPP_TAG_ENUM                   ((ipp_u8bit_t)0x23)
#define IPP_TAG_OCTET_STRING           ((ipp_u8bit_t)0x30)
#define IPP_TAG_DATE_TIME              ((ipp_u8bit_t)0x31)
#define IPP_TAG_RESOLUTION             ((ipp_u8bit_t)0x32)
#define IPP_TAG_RANGE_OF_INTEGER       ((ipp_u8bit_t)0x33)
#define IPP_TAG_TEXT_WITH_LANGUAGE     ((ipp_u8bit_t)0x35)
#define IPP_TAG_NAME_WITH_LANGUAGE     ((ipp_u8bit_t)0x36)
#define IPP_TAG_TEXT_WITHOUT_LANGUAGE  ((ipp_u8bit_t)0x41)
// rdj moved to ipp_api.h
//#define IPP_TAG_NAME_WITHOUT_LANGUAGE  ((ipp_u8bit_t)0x42)
//#define IPP_TAG_KEYWORD                ((ipp_u8bit_t)0x44)
//#define IPP_TAG_URI                    ((ipp_u8bit_t)0x45)
#define IPP_TAG_URI_SCHEME             ((ipp_u8bit_t)0x46)
#define IPP_TAG_CHARSET                ((ipp_u8bit_t)0x47)
#define IPP_TAG_NATURAL_LANGUAGE       ((ipp_u8bit_t)0x48)
#define IPP_TAG_MIME_MEDIA_TYPE        ((ipp_u8bit_t)0x49)
const char* ipp_const_tag_str(ipp_u8bit_t tag);

#define IPP_TAG_BEG_COLLECTION    ((ipp_u8bit_t)0x34)
#define IPP_TAG_END_COLLECTION    ((ipp_u8bit_t)0x37)
#define IPP_TAG_MEMBER_ATTR_NAME  ((ipp_u8bit_t)0x4A)

#define IPP_OPID_PRINT_JOB         ((ipp_u16bit_t)0x0002)
#define IPP_OPID_PRINT_URI         ((ipp_u16bit_t)0x0003)
#define IPP_OPID_VALIDATE_JOB      ((ipp_u16bit_t)0x0004)
#define IPP_OPID_CREATE_JOB        ((ipp_u16bit_t)0x0005)
#define IPP_OPID_SEND_DOCUMENT     ((ipp_u16bit_t)0x0006)
#define IPP_OPID_SEND_URI          ((ipp_u16bit_t)0x0007)
#define IPP_OPID_CANCEL_JOB        ((ipp_u16bit_t)0x0008)
#define IPP_OPID_GET_JOB_ATTR      ((ipp_u16bit_t)0x0009)
#define IPP_OPID_GET_JOBS          ((ipp_u16bit_t)0x000A)
#define IPP_OPID_GET_PRINTER_ATTR  ((ipp_u16bit_t)0x000B)
#define IPP_OPID_HOLD_JOB          ((ipp_u16bit_t)0x000C)
#define IPP_OPID_RELEASE_JOB       ((ipp_u16bit_t)0x000D)
#define IPP_OPID_RESTART_JOB       ((ipp_u16bit_t)0x000E)
#define IPP_OPID_PAUSE_PRINTER     ((ipp_u16bit_t)0x0010)
#define IPP_OPID_RESUME_PRINTER    ((ipp_u16bit_t)0x0011)
#define IPP_OPID_PURGE_JOBS        ((ipp_u16bit_t)0x0012)
#define IPP_OPID_IDENTIFY_PRINTER  ((ipp_u16bit_t)0x003C)
#define IPP_OPID_CUPS_GET_PRINTERS ((ipp_u16bit_t)0x4002)

const char* ipp_const_opid_str(ipp_u8bit_t opid);

#define IPP_STAT_OK                                       ((ipp_u16bit_t)0x0000)
#define IPP_STAT_OK_IGNORED_OR_SUBSTITUTED_ATTRIBUTES     ((ipp_u16bit_t)0x0001)
#define IPP_STAT_OK_CONFLICTING_ATTRIBUTES                ((ipp_u16bit_t)0x0002)
#define IPP_STAT_CLI_BAD_REQUEST                          ((ipp_u16bit_t)0x0400)
#define IPP_STAT_CLI_FORBIDDEN                            ((ipp_u16bit_t)0x0401)
#define IPP_STAT_CLI_NOT_AUTHENTICATED                    ((ipp_u16bit_t)0x0402)
#define IPP_STAT_CLI_NOT_AUTHORIZED                       ((ipp_u16bit_t)0x0403)
#define IPP_STAT_CLI_NOT_POSSIBLE                         ((ipp_u16bit_t)0x0404)
#define IPP_STAT_CLI_TIMEOUT                              ((ipp_u16bit_t)0x0405)
#define IPP_STAT_CLI_NOT_FOUND                            ((ipp_u16bit_t)0x0406)
#define IPP_STAT_CLI_GONE                                 ((ipp_u16bit_t)0x0407)
#define IPP_STAT_CLI_REQUEST_ENTITY_TOO_LARGE             ((ipp_u16bit_t)0x0408)
#define IPP_STAT_CLI_REQUEST_VALUE_TOO_LONG               ((ipp_u16bit_t)0x0409)
#define IPP_STAT_CLI_DOCUMENT_FORMAT_NOT_SUPPORTED        ((ipp_u16bit_t)0x040a)
#define IPP_STAT_CLI_ATTRIBUTES_OR_VALUES_NOT_SUPPORTED   ((ipp_u16bit_t)0x040b)
#define IPP_STAT_CLI_URI_SCHEME_NOT_SUPPORTED             ((ipp_u16bit_t)0x040c)
#define IPP_STAT_CLI_CHARSET_NOT_SUPPORTED                ((ipp_u16bit_t)0x040d)
#define IPP_STAT_CLI_CONFLICTING_ATTRIBUTES               ((ipp_u16bit_t)0x040e)
#define IPP_STAT_CLI_COMPRESSION_NOT_SUPPORTED            ((ipp_u16bit_t)0x040f)
#define IPP_STAT_CLI_COMPRESSION_ERROR                    ((ipp_u16bit_t)0x0410)
#define IPP_STAT_CLI_DOCUMENT_FORMAT_ERROR                ((ipp_u16bit_t)0x0411)
#define IPP_STAT_CLI_DOCUMENT_ACCESS_ERROR                ((ipp_u16bit_t)0x0412)
#define IPP_STAT_SRV_INTERNAL_ERROR                       ((ipp_u16bit_t)0x0500)
#define IPP_STAT_SRV_OPERATION_NOT_SUPPORTED              ((ipp_u16bit_t)0x0501)
#define IPP_STAT_SRV_SERVICE_UNAVAILABLE                  ((ipp_u16bit_t)0x0502)
#define IPP_STAT_SRV_VERSION_NOT_SUPPORTED                ((ipp_u16bit_t)0x0503)
#define IPP_STAT_SRV_DEVICE_ERROR                         ((ipp_u16bit_t)0x0504)
#define IPP_STAT_SRV_TEMPORARY_ERROR                      ((ipp_u16bit_t)0x0505)
#define IPP_STAT_SRV_NOT_ACCEPTING_JOBS                   ((ipp_u16bit_t)0x0506)
#define IPP_STAT_SRV_BUSY                                 ((ipp_u16bit_t)0x0507)
#define IPP_STAT_SRV_JOB_CANCELED                         ((ipp_u16bit_t)0x0508)
#define IPP_STAT_SRV_MULTIPLE_DOCUMENT_JOBS_NOT_SUPPORTED ((ipp_u16bit_t)0x0509)
const char* ipp_const_status_str(ipp_u16bit_t status);

#define IPP_PRINTER_STATE_IDLE       ((ipp_s32bit_t)3)
#define IPP_PRINTER_STATE_PROCESSING ((ipp_s32bit_t)4)
#define IPP_PRINTER_STATE_STOPPED    ((ipp_s32bit_t)5)
const char* ipp_const_printer_state_str(ipp_s32bit_t state);

#ifdef __cplusplus
}
#endif
#endif /* __INC_IPP_CONST_H__ */
