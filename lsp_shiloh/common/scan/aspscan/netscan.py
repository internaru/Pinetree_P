#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# ASP - ASP Scan Protocol 
#
# Scan over network.  
#
# davep Nov-2007

import sys
import struct
import socket
import copy
import StringIO
import logging
import array
import subprocess

import hexdump
import asp
import aspnums
#import log
#from log import nlog.error, nlog.info, nlog.debug
import aspcmdline
import scan
import scanerr
import mkint

#scan_to_host_port = 22011
#scan_to_host_port = 9101
#scan_to_host_port = 8290

#log.debuglevel = 2

# davep 06-Mar-2009 ; add decoding of ICELite BRG Interlaced
g_icelite_row_numbers = [ 0, 0, 0 ]
g_curr_icelite_idx = 0
icelite_data_types = ( aspnums.SCAN_DATA_BLUE, 
                       aspnums.SCAN_DATA_RED, 
                       aspnums.SCAN_DATA_GREEN )

# davep 03-Feb-2011 ; adding support for big-endian PIE
types_that_need_byteswap = (aspnums.SCAN_DATA_RGB_PACKED,
                            aspnums.SCAN_DATA_XRGB_PACKED,
                            aspnums.SCAN_DATA_RGBX_PACKED ) 

nlog = logging.getLogger( 'netscan' )

def netsend( sock, packet ) :
    packet.pack()

    # make a copy because we could be destructive in the write loop below
    buf = packet.netbuf
    bufsize = len( buf )

    while bufsize > 0 :
        retval = sock.send( buf )
        bufsize -= retval     
        if bufsize > 0 :
            buf = buf[retval:]

def aspscan_get_header( sock ) :
    response = asp.ScanHeader()

    datalen = asp.ScanHeader.packetsize 
    pkt = ""
    while datalen > 0 :
        buf = sock.recv( datalen )
        pkt += buf
        datalen -= len(buf)

    response.netbuf = pkt
    response.unpack()
    nlog.debug( "got a scan header {0}".format( response) )
#    if log.debuglevel > 1 :
#        print response

    return response

def aspscan_lock( sock ) :
    lock = asp.ScanHeader()
    lock.msg = aspnums.SCAN_MSG_LOCK_SCAN_RESOURCE
    # timeout stored in param1 (0 means don't timeout...I think)
    lock.param1 = 0

    nlog.info( "send the lock message" )
    netsend( sock, lock )

    # Get a response to our lock
    response = aspscan_get_header( sock ) 

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

def aspscan_get_scan_job_settings( sock ) :
    nlog.info( "asking for scan job settings" )
    pkt = asp.ScanHeader()
    pkt.msg = aspnums.SCAN_MSG_GET_SCAN_JOB_SETTINGS
    netsend( sock, pkt )

    # get a response
    response = aspscan_get_header( sock ) 

    # read the rest of the data
    scan_job = asp.ScanJob()
    nlog.debug( "read %d bytes of settings" % response.datalen )
    scan_job.netbuf = sock.recv( response.datalen )
    nlog.debug( "got %d bytes from socket" % len(scan_job.netbuf) )
    scan_job.unpack()
    nlog.debug( "received scan_job settings from device" )
    print scan_job

    return scan_job

def aspscan_set_default_settings( sock ) :
    set_default = asp.ScanHeader()
    set_default.msg = aspnums.SCAN_MSG_SET_DEFAULT_SCAN_JOB_SETTINGS

    nlog.info( "setting to default job settings" )
    netsend( sock, set_default )

    # get a response
    response = aspscan_get_header( sock ) 

def aspscan_set_settings( sock, job_settings ) :
    set_settings = asp.ScanHeader()
    set_settings.msg = aspnums.SCAN_MSG_SET_SCAN_JOB_SETTINGS
    nlog.info( "sending scan_job setings" )
    netsend( sock, set_settings )

    # now send the settings
    netsend( sock, job_settings )

    # get a response
    response = aspscan_get_header( sock ) 

    # check it was ack'd ok
    if response.status != aspnums.SCAN_STAT_SUCCESS :
        nlog.error( "scanner rejected new settings"  )
        raise asp.ScannerError( scanerr.ERR_SETTINGS_FAIL )
    return response

def aspscan_unlock( sock ) :
    unlock = asp.ScanHeader()
    unlock.msg = aspnums.SCAN_MSG_RELEASE_SCAN_RESOURCE

    nlog.info( "send the unlock message" )
    netsend( sock, unlock )
    
    # get a response
    response = aspscan_get_header( sock ) 
    return response

def aspscan_cancel( sock ) :
    unlock = asp.ScanHeader()
    unlock.msg = aspnums.SCAN_MSG_CANCEL_SCAN_JOB

    nlog.info( "send the cancel message" )
    netsend( sock, unlock )
    
    # get a response
    response = aspscan_get_header( sock ) 
    return response

def setup_output_files( options, page_number=None ) :
    data_type = options["data_type"]
    outfilename = options["outfilename"]

    nlog.info( "setup_output_files() data_type={0} outfilename={1} page_num={2}".format( 
                    data_type, outfilename, page_number ) )

    image_info_template = { "ppr" : 0, 
                            "pprp" : 0, 
                            "rows" : 0, 
                            "file" : "",
                            "datatype" : options["data_type"], 
                            "dpi" : options["resolution"], 
                            "bpp" : 8 }

    # handle page numbers (i.e., ADF)
    if page_number is not None :
        outfilename += "_%d" % page_number
    
    # TODO ; catch open file errors and return error
     
    data_type_to_extension = {
        aspnums.SCAN_DATA_MONO : ".gray",
        aspnums.SCAN_DATA_MONO_MIRRORED : ".gray",
        aspnums.SCAN_DATA_XRGB_PACKED: ".xrgb",
        aspnums.SCAN_DATA_RGBX_PACKED: ".rgbx",
        aspnums.SCAN_DATA_RGB_PACKED: ".rgb",
    }

    if data_type == aspnums.SCAN_DATA_PLANAR :
        data_filenames = ( outfilename+".r", outfilename+".g", outfilename+".b" )
    else : 
        # TODO better error handling
        data_filenames = [ outfilename+ data_type_to_extension[data_type], ]

    data_files = [ open(f,"wb") for f in data_filenames ]

    image_info_list = []
    for filename in data_files :
        image_info = copy.copy( image_info_template )
        image_info["file"] = outfilename

        if page_number is not None :
            image_info["page_number"] = page_number

        image_info_list.append( image_info )

    print data_files
    print image_info_list

    return data_files, image_info_list
    
def close_output_files( data_files ) :
    # close files
    map( lambda f: f.close(), data_files )

def start_job( sock, scan_header ) :
    # the start job message contains a job_settings structure so need to read
    # that here
    scan_job = asp.ScanJob()
    nlog.info( "read {0} bytes of settings".format(scan_header.datalen) )
    scan_job.netbuf = sock.recv( scan_header.datalen )
    scan_job.unpack()

    return scan_job

def start_page( options, page_number=None ) :
    # davep 08-Jul-2009 ; recent protocol changes mean we're going to get
    # multiple start_page/end_page during a cal-to-host. Plus, we'll be getting
    # the blob data after the start_job but before the start_page.  We'll need
    # to open our cal-to-host files in start_job() and our other scan files
    # here in start_page().

    data_files, image_info_list = setup_output_files( options, page_number )

    return data_files, image_info_list

def save_icelite_data( data_info, image_info_list, data_files, options ) :

    data_header = data_info["header"]
    nlog.debug( "save_icelite_data(): data_header: %s" % str(data_header) )

    global g_curr_icelite_idx
    global g_icelite_row_numbers

    bytes_per_row = data_header.bytes_per_pixel * data_header.pixels_per_row_padded

    infile = StringIO.StringIO( data_info["data"] )

    # FIXME slow, cheap hack. Write each row separately.
    new_data_info = { "rows": 0, "bytes": 0, "data": None }
    new_data_info["rows"] = 1
    new_data_info["bytes"] = bytes_per_row
    new_data_info["header"] = asp.ScanData()
    new_data_info["header"].numrows = 1
    new_data_info["header"].bytes_per_pixel = data_header.bytes_per_pixel
    new_data_info["header"].pixels_per_row = data_header.pixels_per_row
    new_data_info["header"].pixels_per_row_padded = data_header.pixels_per_row_padded

    while 1 : 
        buf = infile.read( bytes_per_row )
        if len(buf) <= 0 :
            break

        assert len(buf)==bytes_per_row, len(buf)

        new_data_info["data"] = buf
        new_data_info["header"].rownum = g_icelite_row_numbers[g_curr_icelite_idx]
        new_data_info["header"].data_type = icelite_data_types[g_curr_icelite_idx]

        save_data( new_data_info, image_info_list, data_files, options )

        g_icelite_row_numbers[g_curr_icelite_idx] += 1
        g_curr_icelite_idx = (g_curr_icelite_idx+1)%3

    infile.close()

def save_data( data_info, image_info_list, data_files, options ) :

    data_header = data_info["header"]

#    nlog.debug( "save_data(): data_header: %s" % str(data_header) )

    if data_header.data_type==aspnums.SCAN_DATA_BRG_INTERLACED or\
         data_header.data_type==aspnums.SCAN_DATA_BRG_INTERLACED_MIRRORED :
        # davep 06-Mar-2009 ; decode icelite by splitting the data into the
        # three r,g,b planes and calling save_data() recursively
        save_icelite_data( data_info, image_info_list, data_files, options )
        return

    data_type_to_idx = {
        aspnums.SCAN_DATA_RED : 0,
        aspnums.SCAN_DATA_GREEN : 1,
        aspnums.SCAN_DATA_BLUE : 2,
        aspnums.SCAN_DATA_MONO : 0,
        aspnums.SCAN_DATA_MONO_MIRRORED : 0,
        aspnums.SCAN_DATA_XRGB_PACKED: 0,
        aspnums.SCAN_DATA_RGBX_PACKED: 0,
        aspnums.SCAN_DATA_RGB_PACKED: 0,
    }

    # TODO capture bad data_type, better error handling
    try : 
        idx = data_type_to_idx[ data_header.data_type ]
    except KeyError :
        nlog.error( "ignore data packet with unexpected data_type={0}".format(
                    data_header.data_type ))
        return

    # davep 08-Feb-2011 ; catch data packets we weren't expecting
    if idx > len(image_info_list)-1 : 
        nlog.error( "ignore data packet with unexpected data_type={0}".format(
                    data_header.data_type ))
        return

    image_info_list[idx]["pprp"] = data_header.pixels_per_row_padded
    image_info_list[idx]["ppr"] = data_header.pixels_per_row
    image_info_list[idx]["rows"] += data_header.numrows
    if data_header.data_type==aspnums.SCAN_DATA_XRGB_PACKED or\
       data_header.data_type==aspnums.SCAN_DATA_RGBX_PACKED :
        # XRGB/RGBX only does 8-bpp
        image_info_list[idx]["bpp"] = 8
    else : 
        image_info_list[idx]["bpp"] = data_header.bytes_per_pixel*8

    # davep 16-Sep-2009 ; cheap hack ; change the mirrored data types to
    # non-mirrored types so the image conversion functions don't barf
    if data_header.data_type==aspnums.SCAN_DATA_MONO_MIRRORED :
        image_info_list[idx]["datatype"] = aspnums.SCAN_DATA_MONO
    else : 
        image_info_list[idx]["datatype"] = data_header.data_type

    # davep 01-Dec-2008 ; if we're doing cal-to-host, stamp the
    # Type+Length header into the file so we can parse the cal data
    # properly
    if "cal_to_host" in options : 
        #  type 
        data_files[idx].write( struct.pack( ">L", image_info_list[idx]["datatype"] ))
        #  length 
        data_files[idx].write( struct.pack( ">L", len(data_info["data"]) ) )

    data_files[idx].write( data_info["data"] )

def get_scan_data( sock, scan_header ) :
    # return hash containing bytes_received, rows_received (maybe more later)
    # (using a hash so I can add fields without breaking older code)
    data_info = { "rows": 0, "bytes": 0, "data": None }

    # sanity check against a really broken protocol (8M
    # max which should handle most stuff ) 
    if scan_header.datalen > 0x800000 :
        msg = "header datalen %d too large! (max expected=%d)" %\
                ( scan_header.datalen, 0x800000 )
        raise asp.PacketError( msg, scan_header.netbuf )

    datalen = scan_header.datalen

    # I put some debug code into the firmware that does this; shouldn't happen
    # normally
    if datalen==0 :
        return data_info
    
    pkt = ""
    while datalen > 0 :
#        nlog.debug("read %d bytes of data" % datalen )
        buf = sock.recv( datalen )
        
        pkt += buf
        datalen -= len(buf)

    # first chunk will be the data_header structure
    data_header = asp.ScanData()
    data_header.netbuf = pkt[0:asp.ScanData.packetsize]
    data_header.unpack()
    nlog.info( "received scan data" )
    nlog.info( data_header )

    # sanity check the header
    if data_header.numrows > 256 :
        msg = "data_header num_rows %d too large! (max expected=%d)" %\
                ( data_header.numrows, 256 )
        nlog.error( msg )
        raise asp.PacketError( msg, scan_header.netbuf )
        
    # save the info we'll return to the caller
    data_info["rows"] = data_header.numrows
    data_info["bytes"] = data_header.numrows *\
                 data_header.pixels_per_row_padded *\
                 data_header.bytes_per_pixel
    data_info["header"] = data_header
    data_info["data"] = pkt[asp.ScanData.packetsize:]

    # davep 01-Feb-2011 ; swap the 32-bit big-endian data to little endian
    if data_info["header"].data_type in types_that_need_byteswap : 
        a = array.array( "I" )
#        a = array.array( "L" )
        assert a.itemsize==4, a.itemsize 

        a.fromstring( data_info["data"] )
        a.byteswap()
        data_info["data"] = a.tostring()

    return data_info

def get_data_blob( sock, scan_header, options ) :

    # sanity check against a really broken protocol (8M
    # max which should handle most stuff ) 
    if scan_header.datalen > 0x800000 :
        msg = "header datalen %d too large! (max expected=%d)" %\
                ( scan_header.datalen, 0x800000 )
        raise asp.PacketError( msg, scan_header.netbuf )

    datalen = scan_header.datalen

    pkt = ""
    while datalen > 0 :
#        nlog.debug("read %d bytes of data" % datalen )
        buf = sock.recv( datalen )
        
        pkt += buf
        datalen -= len(buf)

    if not "cal_to_host" in options : 
        # if we're not doing cal-to-host, ignore the blob 
        nlog.debug( "not doing cal-to-host so ignoring the blob" )
        return None

    return pkt

def save_cal_data( buf, data_files ) :
    nlog.info( "save_cal_data() datafiles={0}".format( data_files ) )

    # Write the blob to each and every datafile.
    # 
    # Blobs were originally created to support calibration metadata in
    # cal-to-host. The cal metadata will apply to each data capture: mono or
    # R,G,B files.
    # 
    # It's simpler to write the data to the output file and let calparse
    # decode.

    # XXX temp debug
    nlog.debug( hexdump.dump( buf, 16 ) )

    for fh in data_files : 
        fh.write( buf )

def aspscan_scan( sock, scan_job_settings, options ) :
    # return an array of hashes containing info about the scan (eventually
    # throw in filenames, etc).  Must be the same format as passed to
    # scan.convert_all_files()

#    image_info_template = { "ppr" : 0, 
#                            "pprp" : 0, 
#                            "rows" : 0, 
#                            "file" : "",
#                            "datatype" : options["data_type"], 
#                            "dpi" : options["resolution"], 
#                            "bpp" : 8 }

    all_image_info = []
    
    # davep 16-Sep-2009 ; adding ADF page numbers
    if 'page_numbers' in options :
        page_number = 0
    else :
        page_number = None
    
    scanstart = asp.ScanHeader()
    scanstart.msg = aspnums.SCAN_MSG_START_SCAN_JOB
    nlog.info( "sending the scan start command" )
    netsend( sock, scanstart )

    # get a response
    response = aspscan_get_header( sock ) 

    # did we successfully start? 
    if response.status != aspnums.SCAN_STAT_SUCCESS :
        nlog.error( "scanner failed to start" )
        raise asp.ScannerError( scanerr.ERR_DEV_BUSY )

    data_files = []
    data_filenames = []

    # at this point, when we return from this function, we'll return this value
    scan_retcode = scanerr.ERR_NONE

    end_job = 0

    while not end_job :
        # XXX temp debug
        nlog.info( "waiting for scan header..." )

        response = aspscan_get_header( sock ) 

        # TODO make sure we are in the correct state when we handle a message!
        # In other words, make sure we don't leak file handles if we get two
        # START_PAGE messages in a row. DON'T TRUST THE DEVICE!
        
        # decode that sucker!
        if response.msg==aspnums.SCAN_MSG_START_JOB :
            nlog.info( "start job message"  )
            recv_scan_job_settings = start_job( sock, response )
            nlog.debug( recv_scan_job_settings )
            # TODO verify these are the same settings we sent down before we
            # started

            # davep 16-Nov-2009 ; blob data comes before the start_page so we
            # have to open our files in the start_job
            if "cal_to_host" in options : 
                data_files, image_info_list = setup_output_files( options )

        elif response.msg==aspnums.SCAN_MSG_START_SHEET :
            nlog.info( "start sheet message" )
            # nothing else yet

        elif response.msg==aspnums.SCAN_MSG_START_PAGE :
            nlog.info( "start page message" )

            if not "cal_to_host" in options : 
                data_files, image_info_list = start_page( options, page_number )

            if page_number is not None :
                page_number += 1

        elif response.msg==aspnums.SCAN_MSG_SCAN_IMAGE_DATA :
            nlog.info( "image data message" )
            data_info = get_scan_data( sock, response );

            save_data( data_info, image_info_list, data_files, options )

        elif response.msg==aspnums.SCAN_MSG_END_PAGE :
            nlog.info( "end page message" )

            # save the page(s) we just scanned
            all_image_info.append( image_info_list )

            # we get multiple start_page/end_page when doing cal-to-host so
            # don't disturb our data structures while we're doing cal-to-host
            if not "cal_to_host" in options : 
                # get rid of info from previous page
                close_output_files( data_files )
                data_files = []
                data_filenames = []
                image_info_list = []

        elif response.msg==aspnums.SCAN_MSG_END_SHEET :
            nlog.info( "end sheet message" )
            # nothing else (yet?)

        elif response.msg==aspnums.SCAN_MSG_END_JOB :
            nlog.info( "end job message"  )
            end_job = 1
            if "cal_to_host" in options : 
                close_output_files( data_files )
                data_files = []
                data_filenames = []
                image_info_list = []

        elif response.msg==aspnums.SCAN_MSG_ABORT_SCAN_JOB :
            nlog.error( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"  )
            nlog.error( "!!!    ABORT JOB RECEIVED!    !!!"  )
            nlog.error( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" )
            nlog.error( "prematurely ending the scan" )
            scan_retcode = scanerr.ERR_ABORT_RECEIVED
            end_job=1

        elif response.msg==aspnums.SCAN_MSG_NEW_PAGE :
            nlog.info( "new page message" )
            # TODO send back a new page message 

        elif response.msg==aspnums.SCAN_MSG_DATA_BLOB :
            nlog.info("received data blob message len=%d", response.datalen )
            buf = get_data_blob( sock, response, options )
            save_cal_data( buf, data_files )

        else :
            nlog.error( "ignoring unknown message %d", response.msg )

    return scan_retcode, all_image_info

def do_scan( sock, job_settings, options ) :
    # and now do a scan
    try : 
        scan_retcode, all_image_info = aspscan_scan( sock, job_settings, options )
    except asp.ScannerError, e : 
        # TODO better handle scanner error
        nlog.error( e )
        return e.errcode, None

    if scan_retcode != scanerr.ERR_NONE :
        nlog.error( "the scan failed! errcode=%d" % scan_retcode )
        return scan_retcode, None

#    nlog.debug( "do_scan(): all_image_info={0}".format( all_image_info) )
#
#    if "cal_to_host" in options : 
#        # The -c option tells aspscan to do a cal-to-host which is very
#        # different from a scan-to-host so we'll have to handle it specially.
#        # 
#        # There should only be one image in the scan thus the "[0]"
#        #
#        output_file_info = scan.convert_cal_to_host( all_image_info[0] ) 
#    else : 
#        for image_info_list in all_image_info :
#            scan.convert_all_files( image_info_list )

    return scanerr.ERR_NONE, all_image_info

def reconcile_settings( scan_job_settings, cmdline_options ) :

    # The fieldnames are identical between scan_job_settings and cmdline_options. 
    # Aren't I clever? :-)
    settings_list = (  "resolution", 
                       "scan_type",
                       "data_type",
                       "contrast",
                       "brightness",
                       "gamma",
                       "sharp",
                       "window_top", "window_left", "window_bottom", "window_right",
                       "x_numerator", "x_denominator",
                       "y_numerator", "y_denominator",
                       "bpp", 
                    )
    
    for opt in settings_list :
        if opt in cmdline_options :
            scan_job_settings.__dict__[ opt ] = cmdline_options[opt]

    # davep 17-Sep-2009 ; adding support for document source which is in the
    # flags field which up until this point was always just zero. Add some
    # special code to encode the 'flags'
    if 'docsrc' in cmdline_options :
        # clear the old docsrc flag
        flags = scan_job_settings["flags"]
        flags = flags & ~aspnums.SCAN_JOB_FLAGS_GET_DOCUMENT_SOURCE(-1)

        # set the new flags
        flags = flags | aspnums.SCAN_JOB_FLAGS_SET_DOCUMENT_SOURCE(cmdline_options["docsrc"])

        scan_job_settings["flags"] = flags

    # davep 16-Nov-2009 ; adding cal-to-host support to netscan
    if 'cal_to_host' in cmdline_options : 
        nlog.info( "setting to do cal-to-host" )
        scan_job_settings["flags"] = scan_job_settings["flags"] | aspnums.SCAN_JOB_FLAGS_CAL_TO_HOST

    nlog.debug( "reconcile done={0}".format( scan_job_settings ) )

def net_connect_inet_socket( target_ip, target_port ) :
    sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM )

    nlog.info( "connecting to {0}".format(target_ip)  )

    # TODO catch connection error
    sock.connect( (target_ip,target_port) )

    return sock

def net_connect_unix_socket( socket_path ) :
    sock = socket.socket( socket.AF_UNIX, socket.SOCK_STREAM )
    sock.connect( socket_path )
    return sock

def get_adf_is_paper_present( sock ) : 
    # davep 04-Feb-2011 ; adding support for the "adf is paper present" query
    adfpp = asp.ScanHeader()
    adfpp.msg = aspnums.SCAN_MSG_ADF_IS_PAPER_PRESENT 

    nlog.info( "get adf is paper present" )
    netsend( sock, adfpp )

    response = aspscan_get_header( sock ) 

    # answer is in param1; do strange gyrations to convert to a boolean such
    # that 0==false !0==true
    adf_paper_present = not (response.param1 == 0)

    # read the rest of the data
    if response.datalen : 
        nlog.debug( "read %d bytes of data" % response.datalen )
        buf = sock.recv( response.datalen )
        print hexdump.dump( buf, 16 )

    return adf_paper_present

def decode_platform_string( buf ) : 
    fields = buf.split()
#    print fields

    # expecting to get back pairs of strings; if we don't have an even number,
    # truncate (TODO need a better sanity check)
    fields_len = len(fields)
    if fields_len%2 : 
        nlog.error( "Received odd number of fields in platform string: {0}".format( fields_len ) )
        fields_len -= 1

    platform_fields = {}
    idx = 0
    while idx < fields_len : 
        key = fields[idx].lower()
        idx += 1
        value = fields[idx]
        idx += 1

        # if the value looks like a number, convert it to a number
        try : 
            num = mkint.mkint( value )
            value = num
        except ValueError :
            pass

        platform_fields[key] = value

    return platform_fields

def get_platform_string( sock ) : 
    # davep 31-Jan-2011 ; adding support for the new(ish) 'get platform
    # string'; need this now for big endian PIE
    getplat = asp.ScanHeader()
    getplat.msg = aspnums.SCAN_MSG_GET_PLATFORM_STRING

    nlog.info( "get platform string" )
    netsend( sock, getplat )

    response = aspscan_get_header( sock ) 

    # read the rest of the data
    nlog.debug( "read %d bytes of data" % response.datalen )
    buf = sock.recv( response.datalen )

    platform_info = decode_platform_string( buf ) 
    return platform_info

def main() :
#    log.init( 2, "netscan" )

    # davep 07-Feb-2011 ; add a sanity check on bytesize (using array's
    # byteswap() so need to make sure this is exactly 4-bytes
    a = array.array( "I" )
    assert a.itemsize==4, a.itemsize

    fmt = "%(message)s"
#    fmt = "%(pathname)s %(filename)s %(lineno)d %(name)s %(message)s"
#    logging.basicConfig( level=logging.INFO, format=fmt )
    logging.basicConfig( level=logging.DEBUG, format=fmt )

    try : 
        cmdline_options = aspcmdline.parse_args( sys.argv )
    except aspcmdline.BadCommandLine, err :
        nlog.error( "Invalid command line arguments!" )
        nlog.error( "%s" % err )
        sys.exit(1)

    if "unix_socket_path" in cmdline_options : 
        sock = net_connect_unix_socket( cmdline_options["unix_socket_path"] )
    else : 
        sock = net_connect_inet_socket( cmdline_options["ip_address"],
                                        cmdline_options["tcp_port"] )
    
    lock_return = aspscan_lock( sock )

    platform_info = get_platform_string( sock ) 
    nlog.info( "platform_info={0}".format( platform_info ) )

    # set to default job settings
    aspscan_set_default_settings( sock )
    
    # Get the scan job settings
    job_settings = aspscan_get_scan_job_settings( sock )

    # merge in the command line options
    reconcile_settings( job_settings, cmdline_options )
    nlog.info( "new settings={0}".format( job_settings ) )

    # tell the scanner what we want
    set_successful = True
    try : 
        aspscan_set_settings( sock, job_settings )
    except asp.ScannerError, asp_err :
        # check for scanner rejecting the settings ; we gracefully handle this
        # by NOT scanning then unlocking and closing the connection
        nlog.error( asp_err )
        if asp_err.errcode != scanerr.ERR_SETTINGS_FAIL :
            # re-throw whatever the error was
            raise 

        set_successful = False

    all_image_info = None
    scan_retcode = scanerr.ERR_SETTINGS_FAIL

    if set_successful : 
        # TODO catch ScannerError
        scan_retcode, all_image_info = do_scan( sock, job_settings, cmdline_options )

    # clean up after ourselves
    aspscan_unlock( sock )

    sock.shutdown( 2 )
    sock.close()

    if scan_retcode==scanerr.ERR_NONE : 
        nlog.debug( "do_scan(): all_image_info={0}".format( all_image_info) )

        if "cal_to_host" in cmdline_options : 
            # The -c option tells aspscan to do a cal-to-host which is very
            # different from a scan-to-host so we'll have to handle it specially.
            # 
            # There should only be one image in the scan thus the "[0]"
            #
            output_file_info = scan.convert_cal_to_host( all_image_info[0] ) 
        else : 
            output_file_info_list = []
            for image_info_list in all_image_info :
                output_file_info = scan.convert_all_files( image_info_list )
                output_file_info_list.extend( output_file_info )

    nlog.info( "bye!" )

if __name__ == '__main__' :
    main()

