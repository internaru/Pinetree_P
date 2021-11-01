/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <libusb.h>

#include "log.h"
#include "err.h"
#include "hdump.h"
#include "asp.h"
#include "usbtools.h"
#include "aspusb.h"
#include "xassert.h"

const char *get_libusb_error_string( enum libusb_error errnum )
{
    const char *s;

    switch( errnum ) {
        case LIBUSB_SUCCESS :
            s = "Success (no error)";
            break;

        case LIBUSB_ERROR_IO :
            s = "Input/output error";
            break;

        case LIBUSB_ERROR_INVALID_PARAM :
            s = "Invalid parameter";
            break;

        case LIBUSB_ERROR_ACCESS :
            s = "Access denied (insufficient permissions)";
            break;

        case LIBUSB_ERROR_NO_DEVICE :
            s = "No such device (it may have been disconnected)";
            break;

        case LIBUSB_ERROR_NOT_FOUND :
            s = "Entity not found";
            break;

        case LIBUSB_ERROR_BUSY :
            s = "Resource busy";
            break;

        case LIBUSB_ERROR_TIMEOUT :
            s = "Operation timed out";
            break;

        case LIBUSB_ERROR_OVERFLOW :
            s = "Overflow";
            break;

        case LIBUSB_ERROR_PIPE :
            s = "Pipe error";
            break;

        case LIBUSB_ERROR_INTERRUPTED :
            s = "System call interrupted (perhaps due to signal)";
            break;

        case LIBUSB_ERROR_NO_MEM :
            s = "Insufficient memory";
            break;

        case LIBUSB_ERROR_NOT_SUPPORTED :
            s = "Operation not supported or unimplemented on this platform";
            break;

        case LIBUSB_ERROR_OTHER :
            s = "Other error";
            break;

        default : 
            s = "unknown error code";
            break;
    }

    return s;
}

int get_serial_number( libusb_device_handle *udev, unsigned char *buf, int *buflen )
{
    int retcode;
//    libusb_device_descriptor dev_des;
//    unsigned char buffer[1024];

//    memset( &dev_des, 0, sizeof(dev_des) );

    /* endpoint=USB_DT_DEVICE
     * index=0 
     */
    retcode = libusb_get_descriptor( udev, LIBUSB_DT_DEVICE, 0, buf, *buflen );
    if( retcode < 0 ) {
        errorlog( "failed to get usb device descriptor\n" );
        return -1;
    }

//    retcode = usb_get_string_simple( udev, dev_des.iSerialNumber, buf, *buflen );
//    if( retcode < 0 ) {
//        errorlog( "failed to get usb serial number string\n" );
//        return -1;
//    }

    /* send back the number of bytes we read */
    *buflen = retcode;

    dbg2( "serial number=\"%s\"\n", buf);

    return 0;
}

int
check_serial_number( libusb_device *dev, unsigned char *serial_number )
{
    int retcode, len;
    libusb_device_handle *udev;
#define BUF_SIZE 1024
    unsigned char buf[BUF_SIZE+1]; /* +1 for string NULL */

    if( serial_number==NULL ) {
        return -1;
    }

    dbg2( "%s %s\n", __FUNCTION__, serial_number );

    retcode =  open_device( dev, &udev, 0 );
    if( retcode < 0 ) {
        errorlog( "%s failed to connect to USB device\n", __FUNCTION__ );
        return -1;
    }

    memset( buf, 0, sizeof(buf) );
    len = BUF_SIZE;
    retcode = get_serial_number( udev, buf, &len );
    if( retcode<0 ){
        /* get_serial_number() will log error */
        goto fail;
    }

    if( (size_t)len != strlen((char *)serial_number) ) {
        dbg2( "%s length %d != length %ld\n", __FUNCTION__, len,
                    strlen((char *)serial_number));
        goto fail;
    }

    if( memcmp( serial_number, buf, len )!=0 ) {
        dbg2( "%s no match on serial number string\n", __FUNCTION__ );
        goto fail;
    }

    close_device( udev, 0 );
    return 0;
#undef BUF_SIZE
fail:
    close_device( udev, 0 );
    return -1;
}

int 
find_device( libusb_device **scanner_dev, int vid, int pid, unsigned char *serial_number )
{
//    struct usb_bus *bus;
    libusb_device *dev;
    struct libusb_device_descriptor desc;
    int found;
    int retcode;
    libusb_device **list;
    size_t i, cnt;

    cnt = libusb_get_device_list( NULL, &list );
    found = 0;

    *scanner_dev = NULL;
    found = 0;

    for( i=0 ; i<cnt ; i++ ) {
        dev = list[i];

        memset( &desc, 0, sizeof(struct libusb_device_descriptor) );

        retcode = libusb_get_device_descriptor( dev, &desc );

        /* XXX temp ; add better error handling */
        XASSERT( retcode==0, retcode );

        dbg2( "%s vid=%#x pid=%#x\n", __FUNCTION__, desc.idVendor, desc.idProduct );

        if( desc.idVendor == vid &&
            desc.idProduct == pid ) {
    
            if( serial_number != NULL ) {
                retcode = check_serial_number( dev, serial_number );
                if( retcode < 0 ) {
                    continue;
                }
            }
            
            dbg2( "found our device!\n" );
            *scanner_dev = dev;
            break;
        }
    }

    if( *scanner_dev != NULL ) {
        return 0;
    }

    return ERR_DEV_NOT_FOUND;
}

int 
open_device( libusb_device *dev, libusb_device_handle **dev_handle, int iface )
{
//    libusb_device_handle *udev = NULL;
    int retcode;
#ifdef _WIN32
    int win_config_index;
#endif

    dbg1( "%s %p\n", __FUNCTION__, dev );

    *dev_handle = NULL;

    retcode = libusb_open( dev, dev_handle );
    if( retcode < 0 ) {
        errorlog( "libusb_open failed : %s\n", strerror(errno) );
        return ERR_USB_FAIL;
    }

    dbg2( "device opened\n" );

    /* The set_configuration call is a real mess:
     *  - it causes the Mac platform to crash into a ditch the second time you
     *    attempt to scan
     *  - the windows platform can't work without it (you get an invalid param
     *    error when you claim the interface).  Make sure your windows box has
     *    the latest libusb because the old version of usb_set_configuration() 
     *    treated the 'config' parameter as an index, which was wrong. The new 
     *    version treats the parameter as a value that must be equal to one of 
     *    the descriptor's bConfigurationValue values.
     */
#ifdef _WIN32
    win_config_index = aspusb_get_scan_config_index();
    dbg2( " Setting default config=%d\n",  win_config_index );
    retcode = usb_set_configuration( udev, dev->config[win_config_index].bConfigurationValue );
    if( retcode < 0 ) {
        errorlog( "usb_set_configuration failed : %s\n", strerror(errno) );
        dbg2( "  retcode=\"%d\"\n", retcode);
        goto usb_fail;
    }
#endif

    /* claim the scanner interface so we can talk to it; libusb requires this
     * before we can send any bulk in/out  
     */
    retcode = libusb_claim_interface( *dev_handle, iface );
    if( retcode < 0 ) {
        errorlog( "libusb_claim_interface failed : %s\n", strerror(errno) );
        dbg2( "  retcode=\"%d\"\n", retcode);
        goto usb_fail;
    }
    
    return 0;

usb_fail:
    libusb_close( *dev_handle );

    return ERR_USB_FAIL;
}

int
close_device( libusb_device_handle *udev, int iface )
{
    int retcode;
    int this_function_retcode;

    this_function_retcode = 0;

    retcode = libusb_release_interface( udev, iface );
    if( retcode < 0 ) {
        errorlog( "usb_release_interface failed : %s\n", strerror(errno) );
        this_function_retcode = ERR_USB_FAIL;
        /* keep going and call usb_close() */
    }

    libusb_close( udev );

    return this_function_retcode;
}


