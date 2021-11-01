/*
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief This file contains etc. - one or two sentences.
 *
 */


#ifndef INC_HTTP_MIDAS_EXTENDED_ERROR_CODES_H
#define INC_HTTP_MIDAS_EXTENDED_ERROR_CODES_H

/*
 * This is copied from the MIDAS spec:
 *
 * In addition to the http status code, the device may return at least one http warning header in the http
 * header block. Warning headers are returned to a client in the following form:
 * Warning = "Warning" ":" 1#warning-value
 * warning-value = warn-code SP warn-agent SP <">warn-ext-code [SP warn-element-path]<">
 * warn-code = "99"
 * warn-agent = "xmlService"
 * warn-ext-code is explained below
 * warn-element-path is explained below
 * The following are examples of http Warning headers that may be returned to the client in response to a
 * "get," "put," "post," or delete.
 * Warning: 199 xmlService "100006 scan/targetHosts"
 * Warning: 199 xmlService "100007 scan/targetHosts/targetHost/SALLY255"
 * The http warning code with a "199" following a "Warning:" is used since this header token is a
 * recognized industry- standard http header and is guaranteed to be transmitted through proxy servers back
 * the requesting client.
 * The warning token following the warning-value is always "xmlService" and designates the warning
 * agent. The data following "xmlService" is free-form quoted text. The text contained inside the quotes
 * must be complaint with ISO-8859-1. This quoted text includes two components: (1) "warn-ext-code" and
 * optionally (2) the "warn-element-path."
 * The "warn-ext-code" provides extended XML service status information for server response to a "get,"
 * "put," "post," or "delete" transaction that is in play. This numeric code will typically be used to translate
 * to a human readable error message, presented to a user interface or log file.
 * The "warn-element-path" component designates the XML path to the first leaf node in question when the
 * XML transaction was not successful.
 *
 * |--------------------|-------------------------------------|------------------------------------|
 * | warn-extended-code | meaning                             | element-path                       |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100000             | Successful                          | Not provided                       |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100001             | Device busy                         | Not provided                       |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100002             | Unsupported elements. XML           | Path to last leaf node in question |
 * |                    | elements provided in the XML        |                                    |
 * |                    | packet are well-formed, but not     |                                    |
 * |                    | supported by the XML service.       |                                    |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100003             | Unrecognized value data type        | Path to last leaf node in question |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100004             | Value(s) out of range               | Path to last leaf node in question |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100005             | Value(s) not currently settable     | Path to last leaf node in question |
 * |                    | in the context of related device    |                                    |
 * |                    | configuration items                 |                                    |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100006             | Value(s) not settable in the        | Path to last leaf node in question |
 * |                    | context of current device state     |                                    |
 * |                    | or operation                        |                                    |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100007             | No more entries allowed. The        | Path to last leaf node in question |
 * |                    | "post" xml structure and data       |                                    |
 * |                    | values were valid, but space for    |                                    |
 * |                    | new entries for the elements in     |                                    |
 * |                    | question is exhausted.              |                                    |
 * |--------------------|-------------------------------------|------------------------------------|
 * | 100008             | Duplicate primary key. Post         | Path to last leaf node in question |
 * |                    | xml structure and data values       |                                    |
 * |                    | were valid. However, an             |                                    |
 * |                    | element in the post data            |                                    |
 * |                    | recognized as a primary key         |                                    |
 * |                    | contains a value which is a         |                                    |
 * |                    | duplicate of a primary key for      |                                    |
 * |                    | an existing device entry.           |                                    |
 * |--------------------|-------------------------------------|------------------------------------|
 *
 */

#define HTTP_MIDAS_XML_EXTENDED_WARNING_START               "Warning: 199 xmlService"

#define HTTP_MIDAS_XML_EXTENDED_CODE_SUCCESSFUL             HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100000\"\r\n"
#define HTTP_MIDAS_XML_EXTENDED_CODE_BUSY                   HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100001\"\r\n"
#define HTTP_MIDAS_XML_EXTENDED_CODE_UNSUPPORTED            HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100002 %s\"\r\n"
#define HTTP_MIDAS_XML_EXTENDED_CODE_UNRECOGNIZED           HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100003 %s\"\r\n"
#define HTTP_MIDAS_XML_EXTENDED_CODE_OUT_OF_RANGE           HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100004 %s\"\r\n"
#define HTTP_MIDAS_XML_EXTENDED_CODE_NOT_CURRENTLY_SETTABLE HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100005 %s\"\r\n"
#define HTTP_MIDAS_XML_EXTENDED_CODE_NOT_SETTABLE           HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100006 %s\"\r\n"
#define HTTP_MIDAS_XML_EXTENDED_CODE_NO_MORE                HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100007 %s\"\r\n"
#define HTTP_MIDAS_XML_EXTENDED_CODE_DUPLICATE              HTTP_MIDAS_XML_EXTENDED_WARNING_START" \"100008 %s\"\r\n"

#endif
