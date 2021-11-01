#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Seek N rows into an image, yank M rows from image. Write said rows as .dat
# files suitable for gnuplot.
#
# Can handle 8 or 16-bit pixels.
#
# Written while working on bug 5483
#
# davep 16-Oct-2007
# $Id: rowparse.py 442 2008-06-12 16:57:50Z davep $

import sys
import array
import struct
import getopt

import miscmath
import mkint

# (swiped this from /usr/lib/python2.4/wave.py)
# Determine endian-ness
if struct.pack("h", 1) == "\000\001":
    big_endian = 1
else:
    big_endian = 0

def block_to_ints( buf, pixel_size=2 ) :

    if( pixel_size==2 ) :
        # "H" = unsigned shorts
        intbuf = array.array( "H" )
        intbuf.fromstring( buf )
        
        # Current platform (Extensa) is big endian so if we're running on a little
        # endian system, need to swap the incoming data
        if big_endian==0 :
            intbuf.byteswap()
        
        # scale back from 16-bit pixels to 8-bit pixels
#        div_by_256 = lambda num : num>>8
#        intbuf = map( div_by_256, intbuf )
    else :
        # "B" = unsigned chars 
        intbuf = array.array( "B" )
        intbuf.fromstring( buf )

    return intbuf

def bad_cmdline( msg ) :
    print >>sys.stderr, "%s: invalid command line; %s" % (sys.argv[0],msg)
    print >>sys.stderr, "Use -h for help."
    sys.exit(1)

def usage() :
    print >>sys.stderr, "usage: rowparse.py [options] infilename"
    print >>sys.stderr, "options:"
    print >>sys.stderr, "   --pixels_per_row=%%d (required)" 
    print >>sys.stderr, "   --row_count=%%d (default=1)  ; number of rows to read and average together"
    print >>sys.stderr, "   --skip_rows=%%d (default=0)  ; skip this many rows before averaging data" 
    print >>sys.stderr, "   --bytes_per_pixel=[1|2] (default=1)" 
    print >>sys.stderr, "   --right_clip=%%d (default=0)" 
    print >>sys.stderr, "   --left_clip=%%d (default=0)" 

def parse_args( args ) :

    # build list of our default options 
    options = { "pixels_per_row" : None, # required
                "left_clip" : 0, 
                "right_clip" : 0, 
                "bytes_per_pixel" : 1, 
                "skip_rows" : 0,
                "row_count" : 1,
              }

    # we only absolutely need pixels_per_row to do anything with an image
    required_options = ( "pixels_per_row", )

    longopts = ["help"]
    # Need to add an = to each option so getopt() will know to require an option
    # with a value (e.g., image_size=100x100)
    longopts.extend( [ x+"=" for x in options.keys() ] )

    try:
        opts, args = getopt.getopt( args, "hv", longopts )
    except getopt.GetoptError,e:
        # print help information and exit:
        bad_cmdline( e )

    verbose = 0

    for o, a in opts:
        if o == "-v":
            verbose += 1
            continue
        if o in ("-h", "--help"):
            usage()
            sys.exit()

        # sanity check; make sure string has "--"
        assert len(o) > 2

        # [2:] to cut off the "--"
        opstr = o[2:]

        if not options.has_key( opstr ) :
            bad_cmdline( "invalid option \"%s\"" % opstr )

        try : 
            options[opstr] = mkint.mkint(a)
        except ValueError :
            bad_cmdline( "argument to \"%s\" is not a valid integer." % o )

    # should only have one argument, the input filename
    if len(args) < 1 :
        bad_cmdline( "must have one input filename" )
    if len(args) > 1 :
        bad_cmdline( "must have only one input filename" )

    options["infilename"] = args[0]

    # make sure we got all our required arguments
    for req in required_options :
        if options[req] is None :
            bad_cmdline( "missing required parameter \"%s\"" % req ) 

    return options

def rowparse( options ) :

    pixel_sums = [ 0L ] * options["pixels_per_row"]

    bytes_per_row = options["pixels_per_row"] * options["bytes_per_pixel"]

    row_count = 0

    infile = open( options["infilename"], "rb" )

    if options.has_key( "skip_rows" ) :
        infile.seek( options["skip_rows"] * bytes_per_row )

    while row_count < options["row_count"] :

        buf = infile.read( bytes_per_row )
        row_count += 1

        if len(buf) <= 0 :
            print >>sys.stderr, "premature end-of-file at reading row=%d" % row_count
            break
        
        # clip pixels from right/left sides
        if options.has_key( "right_clip") and options["right_clip"] > 0 :
            buf = buf[:-options["right_clip"]]
        if options.has_key( "left_clip") and options["left_clip"] > 0 :
            buf = buf[options["left_clip"]:]

        pixel_values = block_to_ints( buf, options["bytes_per_pixel"] )

#        print pixel_values[:10], pixel_sums[:10]

        for i in range(len(pixel_values)):
            pixel_sums[i] += pixel_values[i]

    infile.close()

    pixel_averages = [ float(num)/float(row_count) for num in pixel_sums ]

    return pixel_averages

def make_image( pixel_averages, outfilename, image_height=480 ) :
    # make a big honkin' image from the pixel averages

    pixel_values = [ int(round(num)) for num in pixel_averages ]

    buf = array.array( "B", pixel_values )

    outfile = open( outfilename, "wb" )

    for i in range(image_height) :
        buf.tofile( outfile )

    outfile.close()


def main() :    
    options = parse_args( sys.argv[1:] )

    # prep'ing for the future
    outfile = sys.stdout

    num_list = rowparse( options )

    assert len(num_list) > 0 
#    print >>sys.stderr, len(num_list)

    # write some spiffy statistics 
    miscmath.calc_and_write_stats( num_list, outfile )
    for num in num_list :
        print >>outfile, num

    make_image( num_list, "out.gray" )

if __name__ == '__main__' :
    main()

