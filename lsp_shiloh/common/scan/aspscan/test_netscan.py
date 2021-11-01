#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Tests on stuff in netscan.py.  Originally most of this was within netscan.py
# itself until today when I got tired of debugging around it.
#
# davep 31-Jan-2011

import sys
import struct
import logging

import asp
import aspcmdline
import netscan
import hexdump
import scan
#import log

def test_reconcile_settings() :
    scan_job = asp.ScanJob()
    struct.pack( scan_job.format, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 ) 
#    log.dbg2( "foo", "bar", "baz" )
#    log.dbg2( "scan_job={0}".format(scan_job) )

    scan_options = aspcmdline.parse_args( sys.argv )
    print scan_options

    netscan.reconcile_settings( scan_job, scan_options )
    print "after reconcile=",scan_job
        
def send_weird_xmit_reset( sock ) :

    pkt = asp.ScanHeader()
    pkt.msg = aspnums.SCAN_MSG_RESET_XMIT_BUFFERS

    dbg1( "send the reset message" )
    netsend( sock, pkt )

    # there is no response to the reset message (WTF is this thing for!?)
    
def test_weird_xmit_reset() :
    target_ip = "10.71.130.123"

    sock = net_connect( target_ip, scan_to_host_port )

    pkt = asp.ScanHeader()
    pkt.msg = aspnums.SCAN_MSG_RESET_XMIT_BUFFERS

    dbg1( "send the reset message" )
    netsend( sock, pkt )

    # now send a lock
    response = aspscan_lock( sock )
    print "lock response=",response

    response = aspscan_unlock( sock )
    print "unlock response=",response

    sock.shutdown( 2 )
    sock.close()

def test_extra_release() :
    target_ip = "10.71.130.123"

    sock = net_connect( target_ip, scan_to_host_port )
    print sock

    pkt = asp.ScanHeader()
    pkt.msg = aspnums.SCAN_MSG_RELEASE_SCAN_RESOURCE

    dbg1( "send the release lock message" )
    response = netsend( sock, pkt )
    print response

    dbg1( "send the release lock message" )
    response = netsend( sock, pkt )
    print response

    sock.shutdown( 2 )
    sock.close()

def test_convert() :

#    all_image_info= [{'bpp': 8, 'pprp': 640, 'rows': 825, 'ppr': 640, 'datatype': 6, 'dpi': 300, 'file': 'foo'}]

#    all_image_info= [{'bpp': 1, 'pprp': 640, 'rows': 825, 'file': 'foo', 'ppr':
#                634, 'datatype': 1, 'dpi': 600}]

#    all_image_info=[{'bpp': 8, 'pprp': 2528, 'rows': 3296, 'file': 'foo', 'ppr': 2528, 'datatype': 6, 'dpi': 300}]

    all_image_info = [{'bpp': 24, 'pprp': 2528, 'rows': 3296, 'file': 'foo', 'ppr': 2528, 'datatype': 14, 'dpi': 300}]

    all_image_info=[[{'bpp': 8, 'pprp': 2464, 'rows': 3281, 'page_number': 0, 'file': 'foo18', 'ppr': 2464, 'datatype': 3, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3296, 'page_number': 0, 'file': 'foo18', 'ppr': 2464, 'datatype': 4, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3296, 'page_number': 0, 'file': 'foo18', 'ppr': 2464, 'datatype': 5, 'dpi': 300}], [{'bpp': 8, 'pprp': 2464, 'rows': 3317, 'page_number': 1, 'file': 'foo18', 'ppr': 2464, 'datatype': 3, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3328, 'page_number': 1, 'file': 'foo18', 'ppr': 2464, 'datatype': 4, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3328, 'page_number': 1, 'file': 'foo18', 'ppr': 2464, 'datatype': 5, 'dpi': 300}], [{'bpp': 8, 'pprp': 2464, 'rows': 3301, 'page_number': 2, 'file': 'foo18', 'ppr': 2464, 'datatype': 3, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3312, 'page_number': 2, 'file': 'foo18', 'ppr': 2464, 'datatype': 4, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3312, 'page_number': 2, 'file': 'foo18', 'ppr': 2464, 'datatype': 5, 'dpi': 300}], [{'bpp': 8, 'pprp': 2464, 'rows': 3305, 'page_number': 3, 'file': 'foo18', 'ppr': 2464, 'datatype': 3, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3312, 'page_number': 3, 'file': 'foo18', 'ppr': 2464, 'datatype': 4, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3312, 'page_number': 3, 'file': 'foo18', 'ppr': 2464, 'datatype': 5, 'dpi': 300}], [{'bpp': 8, 'pprp': 2464, 'rows': 3301, 'page_number': 4, 'file': 'foo18', 'ppr': 2464, 'datatype': 3, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3312, 'page_number': 4, 'file': 'foo18', 'ppr': 2464, 'datatype': 4, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3312, 'page_number': 4, 'file': 'foo18', 'ppr': 2464, 'datatype': 5, 'dpi': 300}], [{'bpp': 8, 'pprp': 2464, 'rows': 3513, 'page_number': 5, 'file': 'foo18', 'ppr': 2464, 'datatype': 3, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3520, 'page_number': 5, 'file': 'foo18', 'ppr': 2464, 'datatype': 4, 'dpi': 300}, {'bpp': 8, 'pprp': 2464, 'rows': 3520, 'page_number': 5, 'file': 'foo18', 'ppr': 2464, 'datatype': 5, 'dpi': 300}]]

    for a in all_image_info : 
        scan.convert_all_files( a )

def test_oneshot_query( query_callable ) :
    cmdline_options = aspcmdline.parse_args( sys.argv )

    sock = netscan.net_connect_inet_socket( cmdline_options["ip_address"], 
                                        cmdline_options["tcp_port"] )

    lock_return = netscan.aspscan_lock( sock )

    result = query_callable( sock )

    # clean up after ourselves
    netscan.aspscan_unlock( sock )
    sock.shutdown( 2 )
    sock.close()

    return result

if __name__ == '__main__' : 
#    log.init( 2, "test_netscan" )
    logging.basicConfig( level=logging.DEBUG )

#    test_reconcile_settings()

#    test_convert()

    result = test_oneshot_query( netscan.get_adf_is_paper_present ) 
    print "adf_is_paper_present={0}".format( result )

    result = test_oneshot_query( netscan.get_platform_string ) 
    print "platform_string={0}".format( result )

#    test_get_platform_string()
#
