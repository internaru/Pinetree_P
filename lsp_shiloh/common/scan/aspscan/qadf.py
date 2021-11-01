#!/usr/bin/env python

# Use the Python libusb wrapper to talk to Marvell scanner.
#
# https://pypi.python.org/pypi/libusb1
# https://github.com/vpelletier/python-libusb1
#
# davep 23-Apr-2014
#
# First tinkering is just querying the status of ADF. Work in progress. Not
# complete.
#
# TODO merge with netscan, create a 100% python scan-to-host over USB.

import os
import logging
import libusb1
import usb1

import asp
import aspnums
import scanerr

nlog = logging.getLogger(__name__)

# TODO get from cmdline
hp_vid = 0x03f0
scan_interface = 2
scan_endpoint = 5

# USB timeouts are in milliseconds
SECONDS=1000

def send(handle,pkt):
    pkt.pack()

    # make a copy because we could be destructive in the write loop below
    buf = pkt.netbuf
    bufsize = len( buf )

    while bufsize > 0 :
        retval = handle.bulkWrite( scan_endpoint, buf )
        bufsize -= retval     
        if bufsize > 0 :
            buf = buf[retval:]

    nlog.debug("wrote {0} bytes".format(len(buf)))

def aspscan_get_header( handle ) :
    response = asp.ScanHeader()

    datalen = asp.ScanHeader.packetsize 
    pkt = ""
    while datalen > 0 :
        buf = handle.bulkRead( scan_endpoint, datalen )
        pkt += buf
        datalen -= len(buf)

    response.netbuf = pkt
    response.unpack()
    nlog.debug( "got a scan header {0}".format( response) )
#    if log.debuglevel > 1 :
#        print response

    return response

def aspscan_lock( handle ) :
    lock = asp.ScanHeader()
    lock.msg = aspnums.SCAN_MSG_LOCK_SCAN_RESOURCE
    # timeout stored in param1 (0 means don't timeout...I think)
    lock.param1 = 0

#    lock.pack()

    nlog.info( "send the lock message" )
    send( handle, lock )
#    handle.bulkWrite( scan_endpoint, lock.netbuf )

    # Get a response to our lock
    response = aspscan_get_header( handle ) 

    if response.status == aspnums.SCAN_STAT_FAIL:
        nlog.error( "lock failed - cmd failed" )
        raise asp.ScannerError( scanerr.ERR_FAIL )
    elif response.status == aspnums.SCAN_STAT_BUSY:
        nlog.error( "lock failed - busy" )
        raise asp.ScannerError( scanerr.ERR_DEV_BUSY )
    elif response.status == aspnums.SCAN_STAT_CMD_NOT_RECOGNIZED:
        nlog.error( "lock failed - invalid command" )
        raise asp.ScannerError( scanerr.ERR_FAIL )

    return response

def get_adf_paper_present(handle):
    pkt = asp.ScanHeader()
    pkt.msg = aspnums.SCAN_MSG_ADF_IS_PAPER_PRESENT

    send(handle,pkt)

    response = aspscan_get_header( handle ) 

    if response.status!=aspnums.SCAN_STAT_SUCCESS :
        nlog.info("get_adf_paper_present() remote returned error status={0}".format(response.status))
        return None

    adf_status = response.param1

    # /* param1 is a boolean indicating paper/nopaper */
    if response.param1==0:
        adf_status = False
    elif response.param1==1:
        adf_status = True
    else:
        nlog.error("get_adf_paper_present() received invalid/unknown adf paper status value={0}".format(response.param1))

    return adf_status

def get_platform_string( handle ) : 
    # davep 31-Jan-2011 ; adding support for the new(ish) 'get platform
    # string'; need this now for big endian PIE
    getplat = asp.ScanHeader()
    getplat.msg = aspnums.SCAN_MSG_GET_PLATFORM_STRING

    nlog.info( "get platform string" )

    send( handle, getplat )

    response = aspscan_get_header( handle ) 

    if response.status!=aspnums.SCAN_STAT_SUCCESS :
        nlog.info("get_platform_string() remote returned error status={0}".format(response.status))
        return None

    # read the rest of the data
    nlog.debug( "read %d bytes of data" % response.datalen )
    buf = handle.bulkRead( scan_endpoint, response.datalen, 2*SECONDS )

    platform_info = decode_platform_string( buf ) 
    return platform_info

def aspscan_unlock( handle ) :
    unlock = asp.ScanHeader()
    unlock.msg = aspnums.SCAN_MSG_RELEASE_SCAN_RESOURCE

#    unlock.pack()

    nlog.info( "send the unlock message" )
#    handle.bulkWrite( scan_endpoint, unlock.netbuf )
    send(handle,unlock)
    
    # get a response
    response = aspscan_get_header( handle ) 
    return response

def main() : 
    ctx = usb1.USBContext()

    prn = None
    dlist = ctx.getDeviceList( skip_on_error=True)
    for d in dlist : 
        vid,pid = d.getVendorID(),d.getProductID()
        nlog.debug("{0:#06x} {1:#06x}".format(vid,pid))

        if vid==hp_vid:
            prn = d

    nlog.info("serial_num={0}".format(prn.getSerialNumber()))

    handle = prn.open()

    handle.claimInterface(scan_interface)

    aspscan_lock(handle)

    platform_info = get_platform_string( handle ) 
    nlog.info( "platform_info={0}".format( platform_info ) )

    adf_paper_present = get_adf_paper_present(handle)
    nlog.info("adf_paper_present={0}".format(adf_paper_present))

    aspscan_unlock(handle)

    handle.releaseInterface(scan_interface)
    handle.close()
    ctx.exit()

if __name__=='__main__':
#    fmt = "%(message)s"
    fmt = "%(filename)s %(lineno)d %(name)s %(message)s"
#    logging.basicConfig( level=logging.INFO, format=fmt )
    logging.basicConfig( level=logging.DEBUG, format=fmt )

    main()

