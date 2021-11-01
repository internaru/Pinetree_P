#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Average each column of an image, row by row. Print the averages. Optionally
# create an image of the average.
#
# Written to analyze horizontal prints of gray ramps to find posterization
# problems.
#
# davep 9-june-06

import sys
import array
import getopt
import re

VERSION = "1.5.0"

verbose = 0

# shorthand indices into command line options that require an X,Y from parse_args() 
X=0
Y=1

def xassert( flag, value ) :
    if not flag :
        raise AssertionError( value )

def usage() :
    prog="%s %s - average pixel columns of an image" % (sys.argv[0], VERSION ) 
    usage="""
usage: rowavg [options] infile.gray outfile.gray
options:
    -h                      show this help
    -v                      increase verbosity (can be used multiple times)
    --image_width=x         width of a single row of the image 
    --upper_left=x,y        position of upper left of start of sample area
    --pixel_pixel=size      bits per pixel, 8 or 16 (default 8)
    --num_samples=num       number of samples to take across the image
    --capture_data=filename save sampled area to this file 
    --swab                  byteswap incoming 16-bit pixels
"""
    print prog
    print usage

def bad_cmdline( msg ) :
    print "Invalid command line : %s" % msg
    print "Use -h for help."
    sys.exit(1)

def parse_args() :

    # start with the options that need arguments so we can build a set of
    # longopts for getopt
    options = { "image_width" : None,
                "num_samples" : None, 
                "upper_left"  : None,
                "lower_right" : None,
              }

    # require everything up there so far; we'll add other unrequired options as
    # we go
    required_options = options.keys()

    options["pixel_size"] = None
    options["capture_data"] = None

    # long options with no extra arguments
    longopts = ["help", "swab"]
    
    # Need to add an = to each option so getopt() will know to require an option
    # with a value (e.g., image_width=5072)
    longopts.extend( [ x+"=" for x in options.keys() ] )
    
    # default to a 1-byte pixel
    options["pixel_size"] = 8
    
    # don't swap by default; True will only work on 8bpp 
    options["swab"] = False

    try:
        opts, args = getopt.getopt(sys.argv[1:], "hv", longopts )
    except getopt.GetoptError,e:
        # print help information and exit:
        bad_cmdline( e )
    
    # x,y parameters must match this regex; note I'm allowing NUMxNUM, NUMXNUM
    # or NUM,NUM
    xy_re_str = "^\s*(\d+)\s*[xX,]\s*(\d+)\s*$"
    xy_re = re.compile( xy_re_str )

    global verbose
    for o, a in opts:
        if o == "-v":
            print "verbose += 1"
            verbose += 1
            continue
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        if o == "--capture_data" :
            # argument is a filename
            options["capture_data"] = a
            continue
        if o == "--swab" :
            options["swab"] = True
            continue
        if o in ("--upper_left","--lower_right") :
            # [2:] to cut off the "--"
            opstr = o[2:]

            xy_reobj = xy_re.match( a )
            if xy_reobj is None :
                bad_cmdline( "argument to \"%s\" is not a valid integer pair (NUMxNUM or NUM,NUM)." %\
                    opstr )
            x = int(xy_reobj.group(1))
            y = int(xy_reobj.group(2))

            options[opstr] = (x,y)
            continue

        # parse our options with values

        # sanity check; make sure string has "--"
        assert len(o) > 2

        # [2:] to cut off the "--"
        opstr = o[2:]

        if not options.has_key( opstr ) :
            bad_cmdline( "invalid option \"%s\"" % opstr )

        try : 
            num = int(a)
        except ValueError :
            bad_cmdline( "argument to \"%s\" is not a valid integer." % o )

        options[opstr] = num

    # should be two arguments, the input filename and an output filename
    if len(args) != 2 :
        bad_cmdline( "please specify an input and output filename" )

    options["infilename"] = args[0]
    options["outfilename"] = args[1]

    if options["pixel_size"] not in ( 8, 16 ) :
        bad_cmdline( "Invalid pixel size %d. Pixel size can only be either 8 or 16 (bpp)." %\
                options["pixel_size"]  )

    # turn from bpp into bytes
    options["pixel_size"] /= 8 
    
    # make sure we got all our required arguments
    for req in required_options :
        if options[req] is None :
            bad_cmdline( "missing required parameter \"%s\"" % req )

    if verbose :
        print options
    return options

def get_type_code( pixel_size ):
    """Return the array.array() type code for a particular pixel size (pixel
    size in bytes)."""
    if pixel_size==1 :
        type_code = 'B'  # unsigned char
    elif pixel_size==2 :
        type_code = 'H'  # unsigned short
    else :
        xassert( 0, pixel_size )
    return type_code

def row_average( infile, options ):
    # use some temporaries to save typing
    xassert( options["pixel_size"]==1 or options["pixel_size"]==2, options["pixel_size"] )

    rowlen_bytes = options["image_width"] * options["pixel_size"]

    type_code = get_type_code( options["pixel_size"] )

    if verbose :
        print "rows are %d bytes wide" % rowlen_bytes
        print "pixels are %d bytes" % options["pixel_size"]

    # want to average a chunk of pixels so we can pretend it's a "gray" level
    # (+1 to convert from index to count)
    data_width_pixels = options["lower_right"][X] - options["upper_left"][X] +1
    data_width_bytes = data_width_pixels * options["pixel_size"]

    if verbose :
        print "data_width_pixels=%d data_width_bytes=%d" %\
            (data_width_pixels,data_width_bytes)

#    # if we have a 2bpp, make sure we don't have half a pixel floating around
#    if options["pixel_size"] == 2 and data_width%2!=0 :
#        if verbose :
#            print "reducing data width from %d to %d for 16bpp (even number of pixels)" %\
#                (data_width,data_width-1)
#        data_width -= 1 
    sample_size_pixels = data_width_pixels / options["num_samples"]

    # skip this many leading bytes in the input row
    skip_bytes = options["upper_left"][X] * options["pixel_size"]

    if verbose :
        print "skipping %d bytes at front of each row" % skip_bytes

    # note the LONG integer spec on this array
    pixel_sums = [0L] * data_width_pixels

    if verbose :
        print "taking %d samples of %d pixels each between x=%d and x=%d" %\
            (options["num_samples"], sample_size_pixels, 
            options["upper_left"][X], options["lower_right"][X] )
    
    # read this many rows 
    # +1 to convert from index to count
    max_rows = options["lower_right"][Y] - options["upper_left"][Y] +1
    row_count = 0
    
    # jump to the start of first row of our data
#    infile.seek( options["upper_left"][Y] * options["image_width"] * options["pixel_size"], 0 )
    
    if options["capture_data"] is not None:
        try :
            rowfile = open( options["capture_data"], "wb" )
        except IOError,e :
            sys.stderr.write( "Unable to open \"%s\" for writing : %s\n" %\
                (infilename, e.strerror) )
            sys.exit(1)

    # swap each 16-bit pixel?
    byteswap = options["pixel_size"]==2 and options["swab"]
    
    if verbose and byteswap :
        print "we WILL byteswap the data!" 

    # calculate average across the pixels
    while 1 :
        xassert( infile.tell()%options["image_width"]==0, infile.tell() )

        # read a row, sum up the pixel values, read next row
        rowbuf = infile.read( rowlen_bytes )
        if len(rowbuf) <= 0 :
            # end of file
            break
        if len(rowbuf) != rowlen_bytes :
            sys.stderr.write( "short read--expected %d bytes, received %d bytes\n" \
                % (datalen,len(rowbuf)) )
            break

        if verbose :
            print "row=%d read %d bytes from input file" % (row_count, len(rowbuf) )

        # get just the chunk we're interested in
        pixel_buf = rowbuf[ skip_bytes : skip_bytes+data_width_bytes ]

        # break it up into something we can count
        pixel_array = array.array( type_code, pixel_buf )
        
        # the datafiles I get from Doug are big endian
        if byteswap :
            pixel_array.byteswap()

        if options["capture_data"] is not None :
            pixel_array.tofile( rowfile )

        xassert( len(pixel_array)==len(pixel_sums), len(pixel_array) )

        if row_count==0 :
            outfile = open( "row0.dat", "w" )
            print >>outfile, "\n".join( [ "%d"%ord(p) for p in pixel_buf ] )
#            print >>outfile, "\n".join( [ "%d"%num for num in pixel_array ] )

        for i in range(len(pixel_array)) :
            pixel_sums[i] += pixel_array[i]

        if verbose :
            print "pixel[0]=%d pixel[-1]=%d" %\
                ( pixel_array[0], pixel_array[-1] )
            print "pixel_sums[0]=%d pixel_sums[-1]=%d" %\
                ( pixel_sums[0], pixel_sums[-1] )

#        sys.stdout.write( chr(0x08)*4 + "%d"%row_count )
#        sys.stdout.flush()

        row_count += 1
        if row_count >= max_rows :
            if verbose :
                print "row_count=%d == max_rows=%d so quitting" % (row_count,max_rows)
            break


    if options["capture_data"] is not None:
        rowfile.close()

    # note the long integer 'L' on that zero
    sample_sums = [0L] * options["num_samples"]
    pos = 0
    for i in range(options["num_samples"]) :
        sample_sums[i] = sum( pixel_sums[pos : pos+sample_size_pixels] )
        pos += sample_size_pixels
    
    # get the pixel average as a float
    divisor = float(row_count) * float(sample_size_pixels)
    averages = [ num/divisor for num in sample_sums ]

    return ( averages, pixel_sums, sample_sums )

def write_image( outfile, pixel_values, pixel_size=1 ) :
    
    datalen = len(pixel_values) / pixel_size
    type_code = get_type_code( pixel_size )

    if verbose :
        print "write_image len(pixel_values)=%d pixel_size=%d datalen=%d" %\
            (len(pixel_values),pixel_size,datalen)

    # make a row of each pixel replicated into a strip
    row = []
    for i in range(len(pixel_values)) :
        # write 16-bit
#        pixel = pixel_values[i]
        # write 8-bit
        pixel = int( 256.0 * float(pixel_values[i])/65536 )

        row += [ pixel ] * 32

#    out_row = array.array( type_code, row )
    out_row = array.array( 'B', row )

    if verbose :
        print "output row len=%d" % len(out_row)

#    buf = out_row.tostring()

    for i in range(128) :
#        outfile.write( buf )
        out_row.write( outfile )

def main() :
    options = parse_args()

    try :
        infile = open( options["infilename"], "rb" )
    except IOError,e :
        sys.stderr.write( "Unable to open \"%s\" for reading : %s\n" %\
            (infilename, e.strerror) )
        sys.exit(1)

    ( averages, pixel_sums, sample_sums ) = row_average( infile, options )

    infile.close()

    xassert( len(averages)==options["num_samples"], len(averages) )
    xassert( len(pixel_sums)==options["lower_right"][X]-options["upper_left"][X]+1, len(pixel_sums) )
    xassert( len(sample_sums)==options["num_samples"], len(sample_sums) )

    print "# idx  sample_sum  average"
    for i in range(options["num_samples"]) :
        print i, sample_sums[i],averages[i]

    int_averages = [ int(round(n)) for n in averages ]

    try :
        outfile = open( options["outfilename"], "wb" )
    except IOError,e :
        sys.stderr.write( "Unable to open \"%s\" for writing : %s\n" %\
            (infilename, e.strerror) )
        sys.exit(1)

#    write_image( outfile, int_averages, options["pixel_size"] )
    outfile.close()

    # print the max/min averages for ease of use in GNUPlot
    sorted_int_avg = int_averages
    sorted_int_avg.sort( cmp )
    print "# max average=", sorted_int_avg[-1]
    print "# min average=", sorted_int_avg[0]

if __name__ == '__main__' :
    main()

