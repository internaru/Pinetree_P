#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Cal-to-host captures all the scan data calibrate() captures to a single scan.
#
# This script will take apart the final data file based on number of rows per
# call to cal_scan_lines(), average each pixel aross the total lines scanned in
# that calibration stage, and output the result to a text data file.
#
# davep 12-Oct-2006
#
# Complete rewrite. Parse the new cal-to-host data which includes metadata
# about the scan itself.  
# davep 01-Dec-2008 
#
# Add support for the combined rgb-cal image showing all three color planes'
# data on one plt
# davep 12-May-2009
#
# Convert from using gnuplot in a sub-process to Matplotlib.
# davep 01-Oct-2012 

import sys
import struct
import copy
import os
import time
import math
import re
import array
import subprocess
import Image
import numpy as np
from matplotlib.figure import Figure, SubplotParams
from matplotlib.backends.backend_agg import FigureCanvasAgg
import ctypes

import aspnums

debug = 0

# (swiped this from /usr/lib/python2.4/wave.py)
# Determine host's endian-ness
if struct.pack("h", 1) == "\000\001":
    host_endian = "MSB"
else:
    host_endian = "LSB"

# numbers from firmware cal.c
CAL_BLOB_BASE =  100
CAL_BLOB_VERT_RES =            (CAL_BLOB_BASE+1)
CAL_BLOB_HORIZ_RES =           (CAL_BLOB_BASE+2)
CAL_BLOB_CMODE =               (CAL_BLOB_BASE+3)
CAL_BLOB_RED_ENABLED =         (CAL_BLOB_BASE+4)
CAL_BLOB_GREEN_ENABLED =       (CAL_BLOB_BASE+5)
CAL_BLOB_BLUE_ENABLED =        (CAL_BLOB_BASE+6)
CAL_BLOB_RED_PWM =             (CAL_BLOB_BASE+7)
CAL_BLOB_GREEN_PWM =           (CAL_BLOB_BASE+8)
CAL_BLOB_BLUE_PWM =            (CAL_BLOB_BASE+9)
CAL_BLOB_RED_EXP =             (CAL_BLOB_BASE+10)
CAL_BLOB_GREEN_EXP =           (CAL_BLOB_BASE+11)
CAL_BLOB_BLUE_EXP =            (CAL_BLOB_BASE+12)
CAL_BLOB_RED_ANALOG_GAIN =     (CAL_BLOB_BASE+13)
CAL_BLOB_GREEN_ANALOG_GAIN =   (CAL_BLOB_BASE+14)
CAL_BLOB_BLUE_ANALOG_GAIN =    (CAL_BLOB_BASE+15)
CAL_BLOB_RED_ANALOG_OFFSET =   (CAL_BLOB_BASE+16)
CAL_BLOB_GREEN_ANALOG_OFFSET = (CAL_BLOB_BASE+17)
CAL_BLOB_BLUE_ANALOG_OFFSET =  (CAL_BLOB_BASE+18)
CAL_BLOB_BITS_PER_PIXEL =      (CAL_BLOB_BASE+19)
CAL_BLOB_ROWS_PER_STRIP =      (CAL_BLOB_BASE+20)
CAL_BLOB_PIXELS_PER_ROW =      (CAL_BLOB_BASE+21)
CAL_BLOB_CAL_CAP =             (CAL_BLOB_BASE+22) 
CAL_BLOB_PIXELBAND_MONO =      (CAL_BLOB_BASE+23)
CAL_BLOB_PIXELBAND_RED =       (CAL_BLOB_BASE+24)
CAL_BLOB_PIXELBAND_GREEN =     (CAL_BLOB_BASE+25)
CAL_BLOB_PIXELBAND_BLUE =      (CAL_BLOB_BASE+26)
CAL_BLOB_PRNU_WHITE_AVG =      (CAL_BLOB_BASE+27)
CAL_BLOB_PRNU_BLACK_AVG =      (CAL_BLOB_BASE+28)
CAL_BLOB_DEVICE_ENDIAN =       (CAL_BLOB_BASE+29)
CAL_BLOB_CAL_STRIP     =       (CAL_BLOB_BASE+30)
CAL_BLOB_NOW_CALIBRATING_SENSOR_NUM     =       (CAL_BLOB_BASE+31)


# values for CAL_BLOB_DEVICE_ENDIAN (from cal.h)
CAL_BLOB_DEVICE_ENDIAN_MSB = 1
CAL_BLOB_DEVICE_ENDIAN_LSB = 2

# davep 23-Oct-2012 ; this is a kludge to get a unique output name down deep
# into functions that are writing files such as the calstrip and pixel avg.dat
g_basename = None

# need to keep track of which sensor we are calibrating
g_now_calibrating_sensor_num = 0xff
def set_now_calibrating_sensor_num(value):
    global g_now_calibrating_sensor_num
    g_now_calibrating_sensor_num = value
    

def get_now_calibrating_sensor_num():
    global g_now_calibrating_sensor_num
    return g_now_calibrating_sensor_num

class ParseError( Exception ) : 
    def __init__( self, tag_value, errmsg ) :
        # raised when parsing and we find a bad tag
        self.tag = tag_value
        self.errmsg = errmsg

class CalToHostData : 
    def __init__( self, databuf, scan_data_type, metadata ) : 
        assert type(databuf)==type(""), type(databuf)
        self.databuf = databuf
        self.metadata = metadata
        self.scan_data_type = scan_data_type

        self.tags = self._parse_metadata()

    def __str__( self ) :
        return "id=%s datalen=%d ppr=%d num_rows=%d" %\
            ( str(id(self)), len(self.databuf), 
              self.tags["pixels_per_row"], self.tags["rows_per_strip"] )

    def __repr__(self):
        return self.__str__() + "tags "

    def _parse_metadata( self ) :
        # Build an internal hash based on the metadata name.
        # For example, convert "CAL_BLOB_VERT_RES" to "vert_res"

        tags = {}

        for md in self.metadata :
            # change "CAL_BLOB_VERT_RES" to "vert_res"
            s = cal_blob_names.get( md, "(unknown cal blob id=%d)" % md )
            s = s[9:].lower()

            tags[s] = self.metadata[md]

        # davep 06-Dec-2008 ; a few of our symbols need fixing
        for name in ( "red_analog_offset", "green_analog_offset", "blue_analog_offset" ) :
            # Analog offsets are signed numbers.  Default in our decode is to
            # treat everything as signed integers.  So must repack signed.
            # davep 15-Jul-2013 ; switch from struct to ctypes to force 32-bit
            # (struct failed on 64-bit machines)
            tags[name] = ctypes.c_int32( tags[name] ).value
#            tags[name], = struct.unpack( "l", struct.pack( "L", tags[name] ) )

        # davep 08-Sep-2010 ; adding endian support ; default to MSB because
        # prior to this date, our hardware was all big-endian
        if tags.get("device_endian",CAL_BLOB_DEVICE_ENDIAN_MSB)==CAL_BLOB_DEVICE_ENDIAN_MSB:
            tags["device_endian"] = "MSB" 
        else :
            tags["device_endian"] = "LSB"

        return tags

    def pixels_to_array( self ) :
        # davep 08-Sep-2010 ; getting rid of rowparse.py ; adding endian-ness
        # reporting to firmware so host can correctly decode
        pixel_endian = self.tags[ "device_endian" ]

        ppr = self.tags["pixels_per_row"]
        num_rows = self.tags["rows_per_strip"]
        bytes_per_pixel = self.tags["bits_per_pixel"]/8

        size = ( num_rows, ppr )
        assert pixel_endian in ( "MSB", "LSB" ), pixel_endian
        assert host_endian in ( "MSB", "LSB" ), host_endian
        assert bytes_per_pixel in ( 1, 2 ), bytes_per_pixel

        print "size=", size 

        if bytes_per_pixel==2 :
            self.pixel_ndarray = np.fromstring( self.databuf, dtype="uint16").reshape( size )

            # Do we need to swap our data?
            if debug : 
                print "host_endian={0} pixel_endian={1}".format( host_endian, pixel_endian ) 
            if host_endian != pixel_endian : 
                self.pixel_ndarray.byteswap(True)
        else :
            self.pixel_ndarray = np.fromstring( self.databuf, dtype="uint8" )

    def just_the_facts( self ) :
        # build string of just the simple info about the cal settings
        s = ""
        fields = ( "analog_offset", "analog_gain", "pwm", "exp" )
        
        for i in range(len(fields)) :
            colors = [ "%s_%s" % (c,fields[i]) for c in ( "red", "green", "blue" ) ]

            s += "%s=%d,%d,%d " % ( fields[i], self.tags[colors[0]], \
                                   self.tags[colors[1]], self.tags[colors[2]] )

        return s.rstrip()
    
    def getcurrentside(self) :
        # return the side of this cal_buffer
        mytag = self.tags.get("now_calibrating_sensor_num",CAL_BLOB_NOW_CALIBRATING_SENSOR_NUM)
        return mytag
    
def unpack_pixel_avgs( tag, buf ) : 
    # davep 21-Mar-2011 ; adding decode of pixel avgs
    #
    # decode a pixel array cal_send_pixel_array()-cal.c
    #
    #    /* Blob format:
    #     *  tag (4-bytes)
    #     *  length in bytes (4-bytes)
    #     *
    #     * payload: 
    #     *  cookie (4-bytes)
    #     *  scan_data_type  (4-bytes)
    #     *  num_pixels (4-bytes)
    #     *  pixels (array of cal_pixel_t)
    #     */

    fmt = ">LLL"
    hdr = buf[ 0:struct.calcsize(fmt) ]

    value = struct.unpack( fmt, hdr )
    cookie = value[0]
    pixel_data_type = value[1]
    num_pixels = value[2]
    print "cookie={0:#x} dtype={1} num_pixels={2}".format( 
            cookie, pixel_data_type, num_pixels )

    # if this assert fails, check cal.h and cal.c to see if the packet format
    # changed
    assert cookie==0xBE85ECEF, cookie

    # unpack the avg into an array
    fmt = "{0}H".format( num_pixels )
    pixels = struct.unpack( fmt, buf[ len(hdr) : ] )

    # save numbers to a text file
    prnudsnu_dict = { 
        CAL_BLOB_PRNU_WHITE_AVG : "white",
        CAL_BLOB_PRNU_BLACK_AVG : "black" }

    outfilename = "{0}-{1}-{2}-s{3}-avg.dat".format( g_basename, prnudsnu_dict[tag], pixel_data_type, get_now_calibrating_sensor_num())
    outfile = open( outfilename, "w" )
    print "writing data file ", outfilename
    outfile.write( "\n".join( [ str(p) for p in pixels ] ) )
    outfile.close()

def save_16bit_image( outfilename, ndarray ) :
    # davep 23-Oct-2012 ;  
    outimg = Image.new( "I;16", (ndarray.shape[1],ndarray.shape[0]) )
    outimg.fromstring( ndarray.tostring() )
    outimg.save(outfilename)
    print "wrote", outfilename

def unpack_blob( current_tags, tag, buf ) :
    assert tag >= CAL_BLOB_BASE, tag

    # the pixelband values come back as an array of 3 uint32_t values
    pixelband_tags = ( CAL_BLOB_PIXELBAND_MONO, CAL_BLOB_PIXELBAND_RED,
                         CAL_BLOB_PIXELBAND_GREEN,  CAL_BLOB_PIXELBAND_BLUE )

    prnudsnu_tags = ( CAL_BLOB_PRNU_WHITE_AVG, CAL_BLOB_PRNU_BLACK_AVG )

    value = None

    if tag in pixelband_tags : 
        # unpack 3 uint32_t into an array
        if len(buf) != 12 : 
            raise ParseError( tag, "data length should be 12 bytes but is %d" % len(buf) )
        value = struct.unpack( ">LLL", buf )
    elif tag in prnudsnu_tags :
        unpack_pixel_avgs( tag, buf )
    elif tag==CAL_BLOB_CAL_STRIP : 
        # davep 23-Oct-2012 ; send the cal strip as seen by firmware 
        print "dpi=",current_tags[CAL_BLOB_HORIZ_RES ]
        cookie,dtype,num_pixels = struct.unpack( ">LLL", buf[:12] )

        # if this assert fails, check cal.h and cal_common.c to see if the packet format
        # changed
        assert cookie==0xBE85ECEF, cookie

        print "cal strip image", hex(cookie), dtype, num_pixels
        with open("calstrip.gray","wb") as outfile :
            outfile.write(buf[12:])
        pixels_per_row = current_tags[CAL_BLOB_PIXELS_PER_ROW]
        num_rows = num_pixels/pixels_per_row
        size = (num_rows,pixels_per_row)
        print num_rows, pixels_per_row, size
        calstrip = np.fromstring( buf[12:], dtype="uint16" ).reshape(size)
        print calstrip.shape, calstrip.size
        save_16bit_image( "{0}-calstrip-{1}-s{2}.tif".format(g_basename,dtype, get_now_calibrating_sensor_num()), calstrip )
    else :
        # assume a simple variable with one uint32_t
        if len(buf) != 4 :
            raise ParseError( tag, "data length should be 4 bytes but is %d" % len(buf) )

        value, = struct.unpack( ">L", buf )

        # save away the number of the current sensor we are calibrating
        if tag == CAL_BLOB_NOW_CALIBRATING_SENSOR_NUM:
            set_now_calibrating_sensor_num(value)

    return value

def calparse( infile, **kargs ) :
    """Simple debug dump of the contents of a cal-to-host file."""

    verbose = kargs.get( "verbose", False )

    verbose = True

    current = {}

    cal_data_list = []

    cur_row_count = 0
    cur_row_data = ""

    # Read TLD, 4-byte type, 4-byte length, followed by data.
    #
    # I designed the cal firmware to send relative changes in settings during
    # calibration. I send all settings (analog gain, analog offset, exposure,
    # etc) down as one big blob. Then, before each image capture in the steps
    # of cal, I send down the changed settings.
    #
    # In the loop below, I continuously update the 'current' hash as the
    # metadata values are received. Barring screwups in the firmware (har har
    # har), current should always hold the settings used during the subsequent
    # scan data.
    while 1 :
        buf = infile.read( 8 )
        if len(buf) < 8 :
            # end of file or error 
            break

        tag,datalen = struct.unpack( ">2L", buf )
        if debug > 1 : 
            print "tag=%s datalen=%s" % (str(tag),str(datalen))
        buf = infile.read( datalen )
        if tag >= CAL_BLOB_BASE :
            value = unpack_blob( current, tag, buf )
            if verbose :
                # print out blob in a format like # # # CAL_BLOB_xxxxxx
                print tag, datalen, value, \
                    cal_blob_names.get(tag,"(unknown tag=%d)"%tag)
            current[tag] = value
        else :
            bytes_per_row = datalen / (current[CAL_BLOB_BITS_PER_PIXEL]/8)
            num_rows = bytes_per_row / current[CAL_BLOB_PIXELS_PER_ROW]
            rows_per_strip = current[CAL_BLOB_ROWS_PER_STRIP]

            if verbose :
                print tag,datalen,"datatype=%s rows=%d bpr=%d "%\
                    ( data_type_names.get(tag,"(unknown tag=%d)"%tag ), \
                      num_rows, bytes_per_row )

            cur_row_count += num_rows
            cur_row_data += buf

            if tag==aspnums.SCAN_DATA_BRG_INTERLACED :
                # icelite should have been separated by now!
                raise Exception( "Cannot handle ICELite data in calparse!" )

            print "num_rows=",num_rows
            print "rows_per_strip=", current[CAL_BLOB_ROWS_PER_STRIP]

            if cur_row_count >= rows_per_strip :
                # XXX requiring the number of rows in the strip is sent in
                # modular number of buffers;  ie, the captured strips don't
                # overlap in the transmitted buffers (being lazy)
#                assert cur_row_count==current[CAL_BLOB_ROWS_PER_STRIP], cur_row_count

                # save this buffer and its metadata
                cal_data = CalToHostData( cur_row_data, tag, copy.copy(current) ) 
                
                # davep 22-Oct-2012 ; adjust the num_rows to match the size of
                # data we actually got (experimenting with scanning 1" and
                # finding the cal strip in the 1")
                cal_data.tags["rows_per_strip"] = num_rows
                
                cal_data_list.append( cal_data )

                # start looking for data new data
                cur_row_count = 0
                cur_row_data = ""

    return cal_data_list

def simple_print_file( infilename ) :
    # Read and parse a cal-to-host file. Print the info while parsing.
    infile = open( infilename, "rb" )
    calparse( infile, verbose=True )
    infile.close()

def make_all_cal_raw_graph( outfilename, all_cal_data ) : 
    # davep 02-Oct-2012 ; graph the mean (median?) of each strip with error
    # bars. Handy!

    print "strips' mean, median, std:"
    for cal_buffer in all_cal_data : 
        strip_mean = np.mean( cal_buffer.pixel_ndarray )
        strip_median = np.median( cal_buffer.pixel_ndarray )
        strip_stddev = np.std( cal_buffer.pixel_ndarray )
        print strip_mean, strip_median, strip_stddev

    mean_list = np.asarray( [ np.mean(cal_buffer.pixel_ndarray) for cal_buffer in all_cal_data ] )
#    median_list = [ np.median(cal_buffer.pixel_ndarray) for cal_buffer in all_cal_data ]
    max_list = np.asarray( [ np.max(cal_buffer.pixel_ndarray) for cal_buffer in all_cal_data ] )
    min_list = np.asarray( [ np.min(cal_buffer.pixel_ndarray) for cal_buffer in all_cal_data ] )

    yerr_min = np.abs( mean_list - min_list )
    yerr_max = np.abs( mean_list - max_list )

    fig = Figure()
    fig.suptitle( "Cal Captures' Means with max/min" )
    ax = fig.add_subplot( 111 )
    ax.grid()
#    print "margins=",ax.margins()
    ax.margins(0.05,0.05)
    
    # errorbar ftw
    ax.errorbar( np.arange(len(max_list)), mean_list, fmt="-+", 
                 yerr=(yerr_min,yerr_max), ecolor="g", label="Mean" )

    # put the legend in lower right
    ax.legend(loc=4)

    canvas = FigureCanvasAgg(fig)
    canvas.print_figure(outfilename)
    print "wrote", outfilename

def make_all_cal_raw_image( outfilename, all_cal_data ) :
    # combine all the cal data into a single raw image (separates out the
    # metadata and the data)

    print "make raw image %s" % outfilename

    # davep 18-Jul-2013 ;  use the numpy pixel array instead of the databuf
    # byte array. Combine all the numpy arrays into one then make image.
    # Assuming the image data already in correct byte order.
    cal_buffer = np.vstack( [ d.pixel_ndarray for d in all_cal_data ] )
    print cal_buffer.shape
    im = Image.fromarray( cal_buffer, "I;16" )
    im.save( outfilename )

def save_raw_data( outfilename, cal_buffer ) :
    # davep 07-Dec-2008 ; write a single strip to a file (created so I can use
    # calpy/cal.py) on the image(s)

    print "writing raw datafile", outfilename
    outfile = open( outfilename, "wb" )
    outfile.write( cal_buffer.databuf )
    outfile.close()

def make_plots( cal_buffer_list, outfilename ) : 
    # davep 01-Oct-2012 ; upgrade from gnuplot to matplotlib 
    print "drawing graph..." 

    # shrink the surrounding area
    params = SubplotParams()
    params.left = 0.05
    params.right = 0.95
    params.top = 0.95
    params.bottom = 0.05

    num_rows = int(math.ceil( len(cal_buffer_list) / 3.0 ))

#    figsize=(40,10*num_rows)
    figsize=(40/2,10*num_rows/2)

    fig = Figure(figsize=figsize,dpi=300, subplotpars=params )
#    fig = Figure(figsize=figsize,dpi=72, subplotpars=params )
#    fig = Figure(figsize=(40,40),dpi=72, subplotpars=params )
#    fig = Figure(figsize=(8.5,11),dpi=300)
    fig.suptitle( time.ctime(), fontsize=24 )

    plot_counter = 0
    total_plots = 1

    plot_symbol_hash = {
        aspnums.SCAN_DATA_RED: "r+",
        aspnums.SCAN_DATA_GREEN: "g+",
        aspnums.SCAN_DATA_BLUE: "b+",
        aspnums.SCAN_DATA_MONO: "k+",
    }

    for cal_buffer in cal_buffer_list :
        # can't use get_now_calibrating_sensor_num() since we have already parsed all the data
        # into the cal_buffer_list
        curr_sensor = cal_buffer.getcurrentside()
        if plot_counter == 0 :
            hold_sensor = curr_sensor
        if curr_sensor != hold_sensor :
            plot_counter = 1
            hold_sensor = curr_sensor
        else :
            plot_counter += 1
        print "graphing {0}x{1} {2}".format( num_rows, 3, total_plots )
        ax = fig.add_subplot( num_rows, 3, total_plots )
        ax.grid()
        data = np.mean( cal_buffer.pixel_ndarray, axis=0 )
        ax.set_xlabel( cal_buffer.just_the_facts(), fontsize=8 )
        plotstring = "plot={0} calcap={1} sensor={2}".format( 
                        plot_counter,cal_buffer.tags["cal_cap"],curr_sensor)
        ax.set_ylabel(plotstring, fontsize=8 )
#        ax.set_title( cal_buffer.just_the_facts() )
#        ax.plot(data,"r+")
        ax.plot(data, plot_symbol_hash[cal_buffer.scan_data_type] )
        total_plots +=1
    canvas = FigureCanvasAgg(fig)
    canvas.print_figure(outfilename)
    print "wrote", outfilename
    
def run_calparse( infilename ) :
    # write each cal block to an individual file
    (basename,ext) = os.path.splitext( infilename )

    # davep 23-Oct-2012 ; g_basename is a kludge that allows functions deep in
    # the call stack to create files with good names
    global g_basename
    g_basename = basename

    # parse the cal-to-host file
    infile = open( infilename, "rb" )
    all_cal_data = calparse( infile )
    infile.close()

    for cal_buffer in all_cal_data :
        cal_buffer.pixels_to_array()

#    montage_filename = "%s%s-cal.jpg" % (basename, ext)
#    montage_filename = "%s%s-cal.pdf" % (basename, ext)
    montage_filename = "%s%s-cal.png" % (basename, ext)
    make_plots( all_cal_data, montage_filename ) 

    raw_filename = "%s-cal-raw%s.tif" % (basename, ext )
    make_all_cal_raw_image( raw_filename, all_cal_data )
    
    graph_filename = "%s-cal-raw-graph%s.png" % (basename, ext )
    make_all_cal_raw_graph( graph_filename, all_cal_data )

#    for f in ( montage_filename, raw_filename, graph_filename ) : 
#        s = "scp {0} latches.local:tmp/.".format(f)
#        subprocess.check_call( s.split() )

def rgb_extension_sort( input_filenames ) :
    # assuming the files have extensions like .r .g .b, sort into RGB order.

    rgb_order_filenames = [""] * 3

    for infilename in input_filenames :
        (basename,ext) = os.path.splitext( infilename )

        if ext==".r" :
            rgb_order_filenames[0] = infilename
        elif ext==".g" :
            rgb_order_filenames[1] = infilename
        elif ext==".b" :
            rgb_order_filenames[2] = infilename
        else :
            raise Exception( "found extension=\"%s\" but expecting r/g/b filename extension" % ext )

    # check for duplicates (empty entries)
    rgb = ( 'red (.r)', 'green (.g)', 'blue (.b)' )
    for i in range(len(rgb_order_filenames)) :
        if len(rgb_order_filenames[i])==0 :
            raise Exception( "missing %s planar file" % rgb[i] )

    return rgb_order_filenames

def calparse_all_files( filename_list ) :
    """If passed three files, I'm assuming filename_list[] is in r/g/b order"""

    final_data_datfile_list = []

    for infilename in filename_list : 
        datfile_list = run_calparse( infilename )

#        # save the last file so we can make a combined RGB plot of the data
#        final_data_datfile_list.append( datfile_list[-1] )

#    if len(final_data_datfile_list)==3 :
#        # assume all three files have the same basename (what could go wrong!?)
#        (basename,ext) = os.path.splitext( filename_list[0] )
#
#        make_rgb_plot( basename, final_data_datfile_list )

def usage() : 
    print "calparse - parse data returned from a cal-to-host scan."
    print "usage: calparse input-filename(s)"
    print "example:"
    print "calparse foo.r foo.g foo.b"
    print "calparse foo1.gray"

def _create_cal_blob_names() :
    # Create hash of the CAL_BLOB_xxx tags.  The key will be the num from the
    # cal-to-host file. The value will be the string name.
    # For example, cal_blob_names[101] = "CAL_BLOB_VERT_RES"

    names = {}

    all_globals = globals()

    for symbol in all_globals : 
        if symbol.startswith( "CAL_BLOB_" ) :
            value = all_globals[symbol]

            names[ value ] = symbol

    return names

def _create_data_type_names() :
    # Create hash of the SCAN_DATA_xxx types.  The key will be the data type
    # from the cal-to-host file. The value will be the string name.
    # For example, data_type_names[6] = "DATA_TYPE_MONO"

    names = {}
    
    all_symbols = dir(aspnums)

    for symbol in all_symbols:
        if symbol.startswith( "SCAN_DATA_" ) :
            names[getattr(aspnums,symbol)] = symbol

    return names

cal_blob_names = _create_cal_blob_names()
data_type_names = _create_data_type_names()

if __name__ == '__main__' :

    if len(sys.argv) != 2 and len(sys.argv) != 4 :
        usage()
        sys.exit(1)

    infilename_list = []
    if len(sys.argv)==2 : 
        infilename_list.append( sys.argv[1] )
    elif len(sys.argv)==4 : 
        infilename_list = rgb_extension_sort( sys.argv[1:4] )
        
#    for infilename in infilename_list : 
#        # dump the contents of the cal-to-host file
#        simple_print_file(infilename)

    calparse_all_files( infilename_list )

