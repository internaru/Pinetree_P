/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef ASPUSB_H
#define ASPUSB_H

int aspusb_read( struct libusb_device_handle *udev, int endpoint, unsigned char *buf, 
                    int bufsize, int timeout );
int aspusb_get_header( struct libusb_device_handle *udev, struct scan_header *pkt );
int aspusb_send_header( struct libusb_device_handle *udev, struct scan_header *pkt );
int aspusb_is_cancelled( void );
void aspusb_set_cancel( void );

void aspusb_set_scan_endpoints( libusb_device *dev, int iface, int new_out_endpoint, int new_in_endpoint );
int aspusb_get_scan_out_endpoint( void );
int aspusb_get_scan_in_endpoint( void );
int aspusb_get_scan_config_index( void );

#endif

