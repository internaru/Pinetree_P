/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/**\file faxtest.h
 * The file defines the types for the faxtest host app.
 */
#ifndef __FAXTEST_H__
#define __FAXTEST_H__

#define FAX_TEST_CONSOLE_PORT (7360)

#define FAX_TEST_STATUS_UDP_PORT (7440)

typedef enum
{
    HOST_STATUS_UNKNOWN_CMD_ACK = 0,
    HOST_STATUS_REGSTAT_ACK,
    HOST_STATUS_NETOPEN_ACK,
    HOST_STATUS_NETCLOSE_ACK,
    HOST_STATUS_DIALMODE_ACK,
    HOST_STATUS_ECM_ACK,
    HOST_STATUS_SPEED_ACK,
    HOST_STATUS_BRIGHT_ACK,
    HOST_STATUS_RES_ACK,
    HOST_STATUS_DISCALLW_ACK,
    HOST_STATUS_HNAME_ACK,
    HOST_STATUS_HNUMBER_ACK,
    HOST_STATUS_VOL_ACK,
    HOST_STATUS_PRINTSIZE_ACK,
    HOST_STATUS_DISRING_ACK,
    HOST_STATUS_ANS_ACK,
    HOST_STATUS_AUTOANS_ACK,
    HOST_STATUS_FSEND_ACK,
    HOST_STATUS_FCOLORSEND_ACK,
    HOST_STATUS_BWSEND_ACK,
    HOST_STATUS_COLORSEND_ACK,
    HOST_STATUS_GRAYSEND_ACK,
    HOST_STATUS_FNEW_ACK,
    HOST_STATUS_FJPEGNEW_ACK,
    HOST_STATUS_CANCEL_ACK,
    HOST_STATUS_NOPRINT_ACK,
    HOST_STATUS_DIAL_ACK,
    HOST_STATUS_LIST_ACK,
    HOST_STATUS_RELOG_ACK,
} fax_host_status_t;

#endif

