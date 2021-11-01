#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# simple script to take an image and carve a NxM set of samples from the
# interior, saving the samples
# David Poole 28-Apr-06
#
# Read a file of 8-bit pixels (e.g., the Red plane). Take samples from a NxM
# matrix within the file
#
# Calculate the average of a matrix around the center pixel. Optionally dump
# each small image to a bmp using ImageMagick.
#
# version 1.5.0
#   dump min,max,median,mean,stddev instead of just averages

import sys
import os
import struct
import getopt
import string
import getopt
import re
import math

import imgmagick
import miscmath

VERSION = "1.5.0"

verbose = 0

# indices into command line options hash from parse_args() 
X=0
Y=1

def xassert( flag, value ) :
    if not flag :
        raise AssertionError( value )

def calculate_sample_blocks( options ) :
    sample_area_width = options["lower_right"][X] - options["upper_left"][X]
    sample_area_height = options["lower_right"][Y] - options["upper_left"][Y]

    xassert( sample_area_width>0, sample_area_width )    
    xassert( sample_area_height>0, sample_area_height )    

    # size of each block; we'll sample middle of these blocks
    block_size_x = sample_area_width / options["num_samples"][X]
    block_size_y = sample_area_height / options["num_samples"][Y] 

    # offset from the upper left of block to sample area
    sample_offset_x = block_size_x/2 - options["sample_size"][X]/2
    sample_offset_y = block_size_y/2 - options["sample_size"][Y]/2

    return (block_size_x,block_size_y,sample_offset_x,sample_offset_y)

def calc_pixel_stats( row_of_square, column_of_square, buf ) :
    key = "%d-%d" % (row_of_square, column_of_square)

    average = 0

    # create an array of chars so we can sum up the bytes 
    char_array = struct.unpack( "%dB" % len(buf), buf )

    (max,min,median) = miscmath.find_max_min_and_median( char_array )
    (mean,stddev) = miscmath.calculate_standard_deviation( char_array )

    return { "name":key, "max":max, "min":min, "median":median, \
        "mean":mean, "stddev":stddev }

def make_image( buf, x, y, filename_base ) :
    # convert the entire row to an image we can peek at 
    grayfile_name = filename_base + ".gray" 
    grayfile = open( grayfile_name, "wb" )
    grayfile.write( buf )
    grayfile.close()

    outfile_name = filename_base + ".tif"

    imgmagick.convert_file( grayfile_name, outfile_name, pixels_x=x, pixels_y=y )

def usage() :
    prog = "%s %s - sample raw 8-bit pixel image" % (sys.argv[0], VERSION)
    usage="""
Takes a sample from the middle of a square of a sample area. Outputs each row
to an image and raw 8-bit file. Outputs each sample to an image and raw 8-bit file.
Generates a LOT of small files. Requires ImageMagick to convert samples to .bmp
images.

usage: imgparse [options] image.dat
options:
    -h                     show this help
    -v                     increase verbosity (can be used multiple times)
    --image_size=x,y       pixel size of image
    --upper_left=x,y       position of upper left of start of sample area
    --lower_right=x,y      position of lower right of sample area
    --num_samples=x,y      take X samples per row, Y samples per column
    --sample_size=x,y      area to sample
    --make_bitmaps         make bitmaps of each row and sample 

"""
    print prog
    print usage

def bad_cmdline( msg ) :
    print "Invalid command line : %s" % msg
    print "Use -h for help."
    sys.exit(1)

def parse_args() :
    # can be incremented by one or more -v on the command line
    global verbose

    # start with the options that need arguments so we can build a set of
    # longopts for getopt
    options = { "image_size" : None,
                "upper_left" : None, 
                "lower_right" : None, 
                "num_samples" : None, 
                "sample_size" : None,
              }

    # require everything up there so far; we'll add other unrequired options as
    # we go
    required_options = options.keys()

    longopts = ["help"]
    # Need to add an = to each option so getopt() will know to require an option
    # with a value (e.g., image_size=100x100)
    longopts.extend( [ x+"=" for x in options.keys() ] )

    # now add the options that don't need values
    options["make_bitmaps"] = 0
    longopts.append( "make_bitmaps" )

    if len(sys.argv)==1 :
        usage()
        sys.exit(1)

    try:
        opts, args = getopt.getopt(sys.argv[1:], "hv", longopts )
    except getopt.GetoptError,e:
        # print help information and exit:
        bad_cmdline( e )

    # x,y parameters must match this regex; note I'm allowing NUMxNUM, NUMXNUM
    # or NUM,NUM
    xy_re_str = "^\s*(\d+)\s*[xX,]\s*(\d+)\s*$"
    xy_re = re.compile( xy_re_str )

    output = None
    verbose = 0 
    for o, a in opts:
        if o == "-v":
            print "verbose += 1"
            verbose += 1
            continue
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        if o == "--make_bitmaps" :
            options["make_bitmaps"] = 1
            continue

        # parse our options with values

        # sanity check; make sure string has "--"
        assert len(o) > 2

        # [2:] to cut off the "--"
        opstr = o[2:]

        if not options.has_key( opstr ) :
            bad_cmdline( "invalid option \"%s\"" % opstr )

        xy_reobj = xy_re.match( a )
        if xy_reobj is None :
            bad_cmdline( "argument to \"%s\" is not a valid integer pair (NUMxNUM or NUM,NUM)." % o )

        x = int(xy_reobj.group(1))
        y = int(xy_reobj.group(2))

        options[opstr] = (x,y)

    # should only have one argument, the input filename
    if len(args) < 1 :
        bad_cmdline( "must have one input filename" )
    if len(args) > 1 :
        bad_cmdline( "must have only one input filename" )

    options["input_filename"] = args[0]

    # make sure we got all our required arguments
    for req in required_options :
        if options[req] is None :
            bad_cmdline( "missing required parameter \"%s\"" % req ) 
    imgmagick.verbose = verbose

    return options

def take_samples( options ) :
    (block_size_x,block_size_y,sample_offset_x,sample_offset_y) = calculate_sample_blocks( options )

    if verbose :
        print "block_size=%dx%d  sample_offset=%dx%d" %\
                (block_size_x,block_size_y,sample_offset_x,sample_offset_y)

    infile = open( options["input_filename"] )

    # assuming 8-bit pixel, seek our way to the origin row
    offset = options["upper_left"][Y] * options["image_size"][X]
    infile.seek( offset, 0 )

    rowbuf_datalen = options["image_size"][X] * block_size_y

    # position counters for debugging
    block_pos_x = 0
    block_pos_y = 0

    pixel_stats = []

    sample_counter = 0

    for sample_num_y in range( 0, options["num_samples"][Y] ) :
        # read an entire row of squares
        rowbuf = infile.read( rowbuf_datalen )

        # save off the row as a viewable image to make sure I'm parsing the
        # image correctly
        if options["make_bitmaps"] :
            make_image( rowbuf, options["image_size"][X], block_size_y, "row%d"%block_pos_y )

        if len(rowbuf) != rowbuf_datalen :
            # XXX need better error message
            print "Short read! Something went wrong!"
            print "Expected %d bytes but read %d bytes" % (rowbuf_datalen,len(rowbuf))
            infile.close()
            return

        block_pos_x = 0

        offset_x = options["upper_left"][X]

        # cut up our row into blocks and sample the middle of each block
        for column in range( 0, options["num_samples"][X] ) :
            # seek to block in our buffer
            x = offset_x + block_size_x * column
            y = 0

            # seek to sample within the block
            x += sample_offset_x
            y += sample_offset_y

            # read each row of the sample
            sample = ""
            for row in range( y, y+options["sample_size"][Y] ) :
                bufidx = row * options["image_size"][X] + x
#                print x,y,options["image_size"][Y],row, row*options["image_size"][Y], bufidx, bufidx+options["sample_size"][X]
                sample += rowbuf[ bufidx : bufidx+options["sample_size"][X] ]

            # X,Y position image number
            if options["make_bitmaps"] :
                make_image( sample, options["sample_size"][X], options["sample_size"][Y],
                    "sample%d-%d"%(block_pos_y,block_pos_x) )
            # monotonicly increasing image number
#            make_image( sample, options["sample_size"][X], options["sample_size"][Y],
#                    "sample%d"%(sample_counter), "gray" )

            sample_counter += 1

            avg = None
            avg = calc_pixel_stats( block_pos_y, block_pos_x, sample )
            pixel_stats.append( avg )

            if verbose :
                print "sample=[%d,%d] blocksize=%d samplesize=%d" % \
                    (block_pos_x, block_pos_y, options["sample_size"][X]*options["sample_size"][Y], len(sample))

            block_pos_x += 1

        block_pos_y += 1
    
    return pixel_stats

def main() :
    options = parse_args() 
    if verbose > 1 :
        print options

    pixel_stats = take_samples( options )

    cnt = 0
    print "# min max median mean stddev"
    for avg in pixel_stats :
        print cnt, avg["min"], avg["max"], avg["median"], avg["mean"], avg["stddev"]
        cnt += 1

    # 26-Sep-06 davep ; assume we have a linear gradient (e.g., graygrid256)
    # and we want to calculate the standard deviation
#    samples = [ pixel["average"] for pixel in pixel_stats ]
#    (mean,stddev) = calculate_standard_deviation( samples )
#    print "# mean="+str(mean)
#    print "# stddev="+str(stddev)

if __name__ == "__main__" :
    main()

