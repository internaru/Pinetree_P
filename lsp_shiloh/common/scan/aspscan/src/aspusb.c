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
#include <stdlib.h>

#include "log.h"
#include "err.h"
#include "hdump.h"
#include "asp.h"
#include "usbtools.h"
#include "prod.h"
#include "aspusb.h"

#define UNUSED_PARAMETER(x) (x)=(x)

/* set to true in a SIGTERM/SIGINT signal handler to cancel a running scan */
static int g_scan_cancel;

/* davep 10-Apr-2008 ; added support for user specified endpoint.
 * Unfortunately, my old code hardwired all the
 * libusb_bulk_transfer()/libusb_bulk_transfer() calls to SCAN_ENDPOINT, a compile time
 * constant. Replacing with this slightly kludgy global.
 * */
static int g_scan_out_endpoint = SCAN_OUT_ENDPOINT;
static int g_scan_in_endpoint  = SCAN_IN_ENDPOINT;
static int g_scan_config_index = -1;

void
aspusb_set_scan_endpoints( libusb_device *dev, int iface, int new_out_endpoint, int new_in_endpoint )
{
    /*  The dev and iface parameters were used in a version of this code that
     *  traversed the device tree to validate (and discover) endpoint and config
     *  data.  This version of the code doesn't do that fancy stuff, but I left 
     *  the interface just in case we need it in the future.
     */
    UNUSED_PARAMETER(dev);
    UNUSED_PARAMETER(iface);

    /* Just use the supplied values: as a development tool we need the abilty to 
     * do crazy stuff. Assuming the default config is at index 0, if that
     * changes we may have to go back to parsing the device tree.
     */
    g_scan_out_endpoint = new_out_endpoint;
    g_scan_in_endpoint  = new_in_endpoint;
    g_scan_config_index = 0;

    dbg2( "%s Out,In endpoints: %#x %#x\n", __FUNCTION__, g_scan_out_endpoint, g_scan_in_endpoint );
}

int
aspusb_get_scan_out_endpoint( void )
{
    return g_scan_out_endpoint;
}

int
aspusb_get_scan_in_endpoint( void )
{
    return g_scan_in_endpoint;
}

int
aspusb_get_scan_config_index( void )
{
    if (g_scan_config_index == -1) {
        errorlog( "Config index is not yet intialized\n" );
        exit(1);
    }

    return g_scan_config_index;
}

int
aspusb_is_cancelled( void )
{
    return g_scan_cancel;
}

void
aspusb_set_cancel()
{
    g_scan_cancel = 1;
}

/* 
 * Have to wrap up the USB read function because of a bug in our firmware.
 * Under certain circumstances the firmware will send a length 0 URB so we'll
 * come out of the libusb_bulk_transfer call with a length of zero.
 *
 * Stay here reading from USB as long as we keep getting the 0 response.
 *
 * davep 24-Aug-2012 ; add the sleep on getting a zero-length packet. Someone
 * stupid demanded a changed Scan interface to send a zero-length packet to
 * indicate 'no data to read'.
 */

int aspusb_read( libusb_device_handle *udev, int endpoint, unsigned char *buf, int bufsize, int timeout )
{
    int retcode;
    int final_retcode;
    int save_errno;
    int retry_count;
    int transferred;
    useconds_t sleep_usec;

    dbg3( "%s endpoint=%#x timeout=%d\n", __FUNCTION__, endpoint, timeout );

    /* davep 24-Aug-2012 ; poison the incoming buffer to detect USB shenanigans */
    memset( buf, 'q', bufsize );

    sleep_usec = 250;
    retry_count = 0;
    while( 1 ) {
        errno = 0;
        retcode = libusb_bulk_transfer( udev, endpoint, buf, bufsize,
                                            &transferred, timeout );

        /* capture errno immediately (before any debugging statements) */
        save_errno = errno;

        dbg3( "%s libusb_bulk_transfer returned retcode=%d errno=%d transferred=%d\n", 
                    __FUNCTION__, retcode, save_errno, transferred );

        /* davep 24-Aug-2012 ; add check to make sure we got data */
        if( retcode==0 && transferred ) {
            /* success! we got our data */
            dbg3( "%s complete transferred=%d\n", __FUNCTION__, transferred );
            final_retcode = transferred;
            break;
        }
        else if( retcode < 0 ) {
            errorlog( "%s libusb_bulk_transfer failed : retcode=\"%s\" errno=\"%s\"\n", 
                    __FUNCTION__, get_libusb_error_string(retcode), strerror(save_errno) );
            if( retcode==-ETIMEDOUT || retcode==-EAGAIN ) {
                /* timeout; need to return this as a special error so we can
                 * retry at certain times (we will get timeouts while the scan
                 * is calibrating, for example)
                 */
                final_retcode = ERR_USB_TIMEOUT;
            }
            else {
                /* don't know why it failed, it just failed */
                final_retcode = ERR_USB_FAIL;
            }
            break;
        }

        if( aspusb_is_cancelled() ) {
            return ERR_USER_CANCEL;
        }

        retry_count++;
        if( retry_count > 30 ) {
            errorlog( "too many retries on zero length read; returning error\n" );
            return ERR_USB_ZERO_READ;
        }

        dbg2( "zero length read; retrying %d (sleep=%d usec)...\n",
                retry_count, sleep_usec );

        /* davep 24-Aug-2012 ; add a sleep to work around stupid zero-length
         * packet breakage  
         */
        usleep(sleep_usec);

//        sleep_usec *= 2;

        /* davep 06-Sep-2012 ; put a ceiling of 4 seconds on the timeout.
         * (calibration takes so long this timeout creeps up into the 10s of
         * seconds)
         */
        sleep_usec = MIN( sleep_usec*2, 4*USEC_PER_SECOND );
    }

    dbg3( "%s leaving with final_retcode=%d\n", __FUNCTION__, final_retcode );
    return final_retcode;
}


int aspusb_get_header( libusb_device_handle *udev, struct scan_header *pkt )
{
    int retcode;
//    int zero_count;

    dbg2( "%s\n", __FUNCTION__ );

    /* read a response */
    memset( pkt, 0, sizeof(struct scan_header) );

    /* davep 25-Jun-07 ; temporarily drop timeout to see if can catch this
     * weird bug where I'm dropping packets
     */
    retcode = aspusb_read( udev, aspusb_get_scan_in_endpoint(), (unsigned char *)pkt, 
                            sizeof(struct scan_header), 10 * 60 * USB_SECONDS ); 
    if( retcode < 0 ) {
        return retcode;
    }

    asp_swap_header_fields( pkt );

    if( pkt->cookie != ASP_COOKIE ) {
        errorlog( "received bad or out of sync packet!\n" );
#if 0
        /* davep 19-June-07; TEMPORARY CODE! getting bad packets on my mac; sit
         * in this loop reading and hexdumping headers
         */
        while( 1 ) {
            retcode = aspusb_read( udev, aspusb_get_scan_in_endpoint(),
                    (unsigned char *)pkt, sizeof(struct
                        scan_header), 5*USB_SECONDS ); 
            if( retcode < 0 ) {
//                return retcode;
            }

            hex_dump( (unsigned char *)pkt, sizeof(struct scan_header) );

            sleep(1);

//            asp_swap_header_fields( pkt );
        }
        /* end davep 19-June-07 */
#endif
        return ERR_BAD_PROTOCOL;
    }

    return 0;
}

int aspusb_send_header( libusb_device_handle *udev, struct scan_header *pkt )
{
    int retcode;
    int transferred;

    dbg2( "%s\n", __FUNCTION__ );

    /* fill in the constant fields here so we don't have to scatter them all
     * over the code
     */
    pkt->cookie = ASP_COOKIE;

    asp_swap_header_fields( pkt );

    retcode = libusb_bulk_transfer( udev, aspusb_get_scan_out_endpoint(),
            (unsigned char *)pkt, 
                    sizeof(struct scan_header), &transferred, 5*USB_SECONDS );
    dbg2( "%s libusb_bulk_transfer returned retcode=%d transferred=%d\n", 
                __FUNCTION__, retcode, transferred );

    if( retcode < 0 ) {
        errorlog( "libusb_bulk_transfer failed : %s\n", strerror(errno) );
        return ERR_USB_FAIL;
    }

    return 0;
}

