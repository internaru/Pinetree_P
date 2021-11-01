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
# davep Nov-2007

import struct

import aspnums
import hexdump
import scanerr

def xassert( flag, value ) :
    if not flag :
        raise AssertionError( value )

class PacketError( Exception ) :
    def __init__( self, errmsg, packet ) :
        Exception.__init__( self )
        self.errmsg = errmsg
        self.packet = packet

    def __str__( self ) :
        if len(self.packet) < 128 :
            dumpstr = hexdump.dump( self.packet, 16 )
        else :
            dumpstr = hexdump.dump( self.packet[:128], 16 )

        return ("%s\npacket is:\n" % self.errmsg) + dumpstr

class ProtocolError( PacketError ) :
    def __init__( self, errmsg, packet ) :
        PacketError.__init__( self, errmsg, packet )
    # more later? wanted an exception that meant a packet was ok but the
    # contents of the packet were out-of-bounds
    
class ScannerError( Exception ) :
    # An error sent to us from the scanner. For example, scanner busy or if the
    # scanner rejects our settings.
    #
    # Eventually want to encapsulate the SCAN_STAT_xxx status response codes
    # numbers from aspnums.py

    def __init__( self, errcode=0 ) :
        Exception.__init__( self, errcode )

        # note this will raise an exception if you pass in a non-integer error
        # code
        self.errmsg = scanerr.strerror( errcode )
        self.errcode = errcode

    def __str__( self ) :
        return "Scanner reported error: %s" % self.errmsg

class Packet :
    format = ""
    packetsize = 0

    def __init__( self ) :
        self.netbuf = ""
        xassert( self.packetsize==struct.calcsize( self.format ),
            self.packetsize )

    def pack( self, *args ) :
        self.netbuf = struct.pack( self.format, *args )

    def unpack( self ) :
        try : 
            fields = struct.unpack( self.format, self.netbuf )
        except struct.error :
            errmsg = "Unpack Error! Packet incorrect size; should be %d but is %d" %\
                (struct.calcsize(self.format), len(self.netbuf) )
            raise PacketError( errmsg, self.netbuf )
        return fields

class ScanHeader( Packet ) : 
    format = ">8L"
    packetsize = 32

    def __init__( self ) :
        # fields in the scan_header packet
        self.cookie = aspnums.ASP_COOKIE  
        self.msg = 0
        self.param1 = 0
        self.param2 = 0
        self.status = 0
        self.datalen = 0
        self.reserved1 = 0
        self.reserved2 = 0
        
    def pack( self ) :
        Packet.pack( self, self.cookie, self.msg,
            self.param1, self.param2, self.status, self.datalen, 
            self.reserved1, self.reserved2 )

    def unpack( self ) :
        (self.cookie, self.msg,
            self.param1, self.param2, self.status, self.datalen, 
            self.reserved1, self.reserved2 ) = Packet.unpack( self )

        # test my newfound fields, make sure correct
        if self.cookie != aspnums.ASP_COOKIE : 
            raise PacketError( "Error! Missing cookie. Bad or out of sync packet", self.netbuf )

    def __str__( self ) :
        msg = "cookie=%#x msg=%d param1=%d param2=%d status=%d" + \
               " datalen=%d reserved1=%d reserved2=%d" 

        return msg % (self.cookie, self.msg,
            self.param1, self.param2, self.status, self.datalen, 
            self.reserved1, self.reserved2 ) 


class ScanData( Packet ) :
    format = ">6L"
    packetsize = 24

    def __init__( self ) :
        Packet.__init__(self)

        # fields in the scan_data_header packet
        self.data_type = 0
        self.rownum = 0
        self.numrows = 0
        self.bytes_per_pixel = 0
        self.pixels_per_row = 0
        self.pixels_per_row_padded = 0

    def pack( self ) :
        Packet.pack( self, self.data_type, self.rownum,
            self.numrows, self.bytes_per_pixel, self.pixels_per_row,
            self.pixels_per_row_padded )

    def unpack( self ) :
        ( self.data_type, self.rownum, self.numrows, 
          self.bytes_per_pixel, self.pixels_per_row, 
          self.pixels_per_row_padded ) = Packet.unpack( self )

    def __str__( self ) : 
        return "data_type=%d rownum=%d numrows=%d bytes_per_pixel=%d ppr=%d pprp=%d" %\
            ( self.data_type, self.rownum,
                self.numrows, self.bytes_per_pixel, self.pixels_per_row,
                self.pixels_per_row_padded ) 

class ScanJob( Packet ) :
    format = ">25L"
    packetsize = 100

    def __init__( self ) :
        Packet.__init__( self ) 

        # fields in the scan_job packet
        self.gamma = 0
        self.brightness = 0
        self.contrast = 0
        self.resolution = 0
        
        # X/Y scale
        self.x_numerator = 0
        self.x_denominator = 0
        self.y_numerator = 0
        self.y_denominator = 0

        self.sharp = 0
        self.smooth = 0
        self.bpp = 0
        self.reserved1 = 0
        self.reserved2 = 0
        self.reserved3 = 0

        #see SCAN_JOB_FLAGS_xxx 
        self.flags = 0

        # one of SCAN_DATA_xxx 
        self.data_type = 0

        # scan window, area within the max area (below) we want to scan
        self.window_top = 0
        self.window_left = 0
        self.window_bottom = 0
        self.window_right = 0

        # scan area, where the scanner can actually scan
        self.area_top = 0
        self.area_left = 0
        self.area_bottom = 0
        self.area_right = 0

        # SCAN_TYPE_MONO or SCAN_TYPE_COLOR 
        self.scan_type = 0
        
        # end of fields in scan_job packet
       
    def pack( self ) :
        Packet.pack( self, self.gamma, self.brightness, self.contrast,
            self.resolution, self.x_numerator, self.x_denominator,
            self.y_numerator, self.y_denominator, self.sharp, self.smooth,
            self.bpp, self.reserved1, self.reserved2, self.reserved3,
            self.flags, self.data_type, 
            self.window_top, self.window_left, self.window_bottom, self.window_right, 
            self.area_top, self.area_left, self.area_bottom, self.area_right, 
            self.scan_type ) 

    def unpack( self ) :
        ( self.gamma, self.brightness, self.contrast,
            self.resolution, self.x_numerator, self.x_denominator,
            self.y_numerator, self.y_denominator, self.sharp, self.smooth,
            self.bpp, self.reserved1, self.reserved2, self.reserved3,
            self.flags, self.data_type, 
            self.window_top, self.window_left, self.window_bottom, self.window_right, 
            self.area_top, self.area_left, self.area_bottom, self.area_right, 
            self.scan_type )  = Packet.unpack( self )

    def __str__( self ) :
        s = "gamma=%d bright=%d contrast=%d res=%d x=%d/%d y=%d/%d sharp=%d "+\
            "smooth=%d bpp=%d res1=%d res2=%d res3=%d flags=%#x data_type=%d " +\
            "window=%d,%d,%d,%d area=%d,%d,%d,%d scan_type=%d" 
        return s % \
        (self.gamma, self.brightness, self.contrast,
            self.resolution, self.x_numerator, self.x_denominator,
            self.y_numerator, self.y_denominator, self.sharp, self.smooth,
            self.bpp, self.reserved1, self.reserved2, self.reserved3,
            self.flags, self.data_type, 
            self.window_top, self.window_left, self.window_bottom, self.window_right, 
            self.area_top, self.area_left, self.area_bottom, self.area_right, 
            self.scan_type ) 

    def __getitem__( self, item ) :
        return self.__dict__[item]

    def __setitem__( self, key, value ) :
        self.__dict__[key] = value

def test_msg( fmt, *args ) :
    print "*** " + (fmt % args)

def test_packing( instance, good_test_packet ) :
    # tests on pack/unpack class
    print instance
    print dir(instance)
    
    instance.pack()
    print instance
    instance.unpack()
    print instance

    # a buffer that looks like it came from a socket
    instance.netbuf = good_test_packet
    instance.unpack()
    print instance
    instance.pack()
    print instance

def test_exceptions() :
    print "testing exceptions..."
    try : 
        raise ScannerError()
    except ScannerError, e :
        print e

    try : 
        raise ScannerError( scanerr.ERR_DEV_BUSY )
    except ScannerError, e :
        print e

    try : 
        raise ScannerError( scanerr.ERR_SETTINGS_FAIL )
    except ScannerError, e :
        print e

    print "exception testing done."

def run_tests() :
    # tests on scan_header class
    test_msg( "testing ScanHeader class..." )
    scan_header = ScanHeader()
    test_packing( scan_header, 
        struct.pack( scan_header.format, aspnums.ASP_COOKIE, 2, 3, 4, 5, 6, 7, 8 ) )

    # tests on scan_data class
    test_msg( "testing ScanData class..." )
    scan_data = ScanData()
    test_packing( scan_data, 
        struct.pack( scan_data.format, 0, 1, 2, 3, 4, 5 ) )

    test_msg( "testing ScanJob class..." )
    scan_job = ScanJob()
    test_packing( scan_job, 
        struct.pack( scan_job.format, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 ) )

    # too short for scan_header packet
    tmpbuf = struct.pack( scan_header.format, 1, 2, 3, 4, 5, 6, 7, 8 )
    scan_header.netbuf = tmpbuf
    try : 
        scan_header.unpack()
    except PacketError, e :
        test_msg( "correctly failed" )
        test_msg( str(e) )
    else :
        # should have failed!
        assert 0

    # packet format field tests
    print "ScanHeader format=",ScanHeader.format,"packet_size=",ScanHeader.packetsize
    print "ScanData format=",ScanData.format, "packet_size=",ScanData.packetsize

    test_exceptions()

if __name__ == "__main__" :
    run_tests()

