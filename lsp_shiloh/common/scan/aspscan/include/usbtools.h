/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef USBTOOLS_H
#define USBTOOLS_H

/* usb.h functions have a timeout measured in milliseconds */
#define USB_SECONDS 1000

/* functions are using microsoconds in an attempt to be RTOS
 * cross platform  (as opposed to using ticks)
 */
#define USEC_PER_SECOND     1000000  /* one million */
#define USEC_PER_MILLISEC   1000     /* one thousand */
#define NANOSEC_PER_SECOND  1000000000 /* one BILLLION */
#define NANOSEC_PER_USEC    1000     /* one thousand */

#ifndef MIN
    #define MIN(x,y)  ( (x)<(y)?(x):(y) )
#endif

int find_device( libusb_device **scanner_dev, int vid, int pid, unsigned char *serial_number );
int open_device( libusb_device *dev, libusb_device_handle **dev_handle, int iface);
int close_device( struct libusb_device_handle *udev, int iface );
int get_serial_number( struct libusb_device_handle *udev, unsigned char *buf, int *buflen );
const char *get_libusb_error_string( enum libusb_error errnum );

#endif

