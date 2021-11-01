#!/usr/bin/python 
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Parse an incoming ICE Lite (currently used with certain ASICs) RGB image into
# separate planes.  ICE Lite  doesn't have a color channel so we get sent
# BRGBRGBRGB...  rows to the host.  Need to read each B,R,G row pattern and
# write into its own file.
#
# No, that's not a typo.  We get the data as Blue-Red-Green rows.
# 
# davep 14-Jan-2008
#
# $Id: icelite.py 461 2008-06-20 15:57:28Z davep $

import sys
import os
import stat
import StringIO

import mkint

debug=1

def do_separate( infile, outfile_list, bytes_per_row ) :

    idx = 0

    row_counter = 0
    while 1 :
        buf = infile.read( bytes_per_row )
        
        if len(buf) == 0 :
            # end of file 
            break

        # end of file or we have a bad bytes_per_row (if we read >0 and
        # <bytes_per_row, we were given the wrong pixels_per_row)
        if len(buf) != bytes_per_row :
            print >>sys.stderr, "short read from file: expected %d got %d" %\
                        (bytes_per_row, len(buf))
            break

        row_counter += 1
        
        outfile_list[idx].write( buf ) 
        
        # move to next file
        idx = (idx+1) % 3

    return row_counter

def separate_buffer( buf, pixels_per_row, bits_per_pixel) :
    # return B,R,G buffers of separated data
    bytes_per_pixel = bits_per_pixel / 8
    bytes_per_row = pixels_per_row * bytes_per_pixel

    infile = StringIO.StringIO( buf )

    outfile_list = []
    for i in range(3) :
        outfile_list.append( StringIO.StringIO() )

    do_separate( infile, outfile_list, bytes_per_row )

    buf_list = []
    for i in range(3) :
        buf_list.append( outfile_list[i].getvalue() )
        outfile_list[i].close()

    infile.close()

    assert len(buf_list[0])==len(buf_list[1]), len(buf_list[0])
    assert len(buf_list[1])==len(buf_list[2]), len(buf_list[1])

    return buf_list

def separate_file( infilename, outfilename_base, pixels_per_row, bits_per_pixel) :
    bytes_per_pixel = bits_per_pixel / 8

    bytes_per_row = pixels_per_row * bytes_per_pixel

#    print "bytes_per_row=%d" % bytes_per_row

    # create three output filenames by appending the color plane name to the
    # end of the cmdline argument
    #
    # Note: B,R,G is the correct order. That's what comes from ICE Lite.
    # Yes, I'm serious.
    # No, really.
    outfilename_list = [ outfilename_base+ ext for ext in ( ".b", ".r", ".g" ) ]

    # open the three output files
    outfile_list = map( lambda f : open( f, "wb" ), outfilename_list )

    if debug :
        print "opened ", outfilename_list

    infile = open( infilename, "rb" )

    rows = do_separate( infile, outfile_list, bytes_per_row )

    if debug :
        print "wrote ", outfilename_list

    # be polite and close our files
    infile.close()
    map( lambda f : f.close(), outfile_list )

    print "closed ", outfilename_list
    
    return( {"rows":rows, "lines":rows/3, "remainder":rows%3,
             "outfilenames":outfilename_list } )

def usage() :
    print "usage: %s infilename outfilename_base pixels_per_row bits_per_pixel"\
        % sys.argv[0]
    print "infilename       - filename from which to read scan data"
    print "outfilename_base - this script creates three output files with extensions .r .g .b using this filename" 
    print "pixels_per_row   - pixels per row"
    print "bits_per_pixel   - valid values are 8 or 16"

def main() :
    if len(sys.argv) != 5 :
        usage()
        sys.exit(1)

    infilename = sys.argv[1]
    outfilename_base = sys.argv[2]
    pixels_per_row = mkint.mkint(sys.argv[3])
    bits_per_pixel = int(sys.argv[4])

    if bits_per_pixel not in ( 8, 16 ) :
        raise Exception( "invalid bits-per-pixel value=%d; expected 8 or 16" % bits_per_pixel )

    result = separate_file( infilename, outfilename_base, pixels_per_row, bits_per_pixel )

    print "found rows=%d lines=%d remainder=%d" %\
        ( result["rows"], result["lines"], result["remainder"] )

    # sanity check the file sizes
    file_sizes = [ os.stat(name)[stat.ST_SIZE] for name in result["outfilenames"] ]
    if debug : 
        print "file_sizes=",file_sizes
    for i in range(1,len(file_sizes)) :
        if file_sizes[0] != file_sizes[i] :
            print "WARNING! file sizes aren't identical!"

if __name__ == '__main__' :
    main()

