#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Cut a single column from an image. Currently hardwired to just 1 column.
# Output sent to STDOUT in format suitable for import into Octave.
#
# Originally written to get a column from an image to do an FFT to try to find
# electrical noise causing image corruption.
#
# davep 14-Aug-06

import sys
import array

import miscmath 

def cutcol( infile, rowsize, colnum ): 
    
    pixels = ""
    while 1 :
        row = infile.read( rowsize )
        if len(row) <= 0 :
            # end of file
            break
        if len(row) < colnum :
            # print warning? just silently ignore for now
            break

        pixels += row[colnum]

    nums = array.array( "B" )
    nums.fromstring( pixels )
    return nums

def array_to_octave( data_array, array_name ) :
    print "# name: %s" % array_name
    print "# type: matrix"
    print "# rows: 1"
    print "# columns: %d" % len(data_array)
    for num in data_array :
        print num,
    print

def array_to_gnuplot( data_array ) :
    for num in data_array :
        print num

def main(): 
    filename = sys.argv[1]
    rowsize = int(sys.argv[2]) 
    colnum = int(sys.argv[3])

    infile = open( filename, "rb" )
    colpixels = cutcol( infile, rowsize, colnum )
    infile.close()

    (min_pixel,max_pixel,median_pixel) = miscmath.find_max_min_and_median( colpixels )
    (mean,stddev) = miscmath.calculate_standard_deviation( colpixels )

    print "# min=",min_pixel
    print "# max=",max_pixel
    print "# median=",median_pixel
    print "# mean=", mean
    print "# stddev=", stddev

    print >>sys.stderr, "writing raw pixels to col.dat"
    outfile = open( "col.dat", "wb" )
    colpixels.tofile( outfile )
    outfile.close()

#    array_to_octave( colpixels, "col" )
    array_to_gnuplot( colpixels )
    
if __name__ == '__main__' :
    main()

