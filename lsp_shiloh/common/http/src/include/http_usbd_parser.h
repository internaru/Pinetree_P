/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file http_usbd_parser.h
 *
 * \brief This file is the API for the USB Device portion
 * of the HTTP module
 *
 */

#ifndef _INC_HTTP_USBD_PARSER_H
#define _INC_HTTP_USBD_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif



/**
 *
 * \brief Initialize the USB Device portion of the HTTP module
 *
 */
void http_usbd_parser_init( void );



/** 
 * \brief Write data to the EWS interrupt endpoint
 * 
 * \param buffer The data to write
 * 
 * \param len Number of bytes to write
 * 
 * \return Number of bytes written
 * 
 **/
int32_t http_usb_interrupt_write( uint8_t *buffer, uint32_t len );

#ifdef DEBUG
/**
 *
 * \brief Get the thread ID of the http parser; used for debug 
 *        validation checks only.
 *
 */
pthread_t http_usbd_parser_get_tid( void );
#endif

#ifdef __cplusplus
}
#endif

#endif
