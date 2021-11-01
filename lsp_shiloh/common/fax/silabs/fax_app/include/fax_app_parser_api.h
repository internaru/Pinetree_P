/*
 * ============================================================================
 * Copyright (c) 2008   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * fax_app_parser.h
 *
 *  Created on: Feb 17, 2009
 *      Author: ericyh
 */

#ifndef FAXER_APP_PARSER_H_
#define FAXER_APP_PARSER_H_

typedef enum
{
    e_NoFaxes,
    e_ReceivedFaxAvailable,
    e_FaxForwarded,
    e_FaxPrinted,
    e_FaxReceiveModeChanged
} FAXER_APP_STATUS_ENUM;

void faxer_app_parser_report_status(FAXER_APP_STATUS_ENUM status_id);

#endif /* FAX_APP_PARSER_H_ */
