#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Read each row of data in an image. Sum it. Print sum. 
#
# Created while working on Bug 5320.
# davep 2-May-2007
#

import sys
import array

import miscmath

def sum_image_rows( filename, row_width ) :
    infile = file( filename, "rb" )

    pixel_sums = []

    while 1 :
        buf = infile.read( row_width )
        if len(buf) <= 0 :
            break
        if len(buf) < row_width :
            print >>sys.stderr, ("short read of %d when expecting %d; is your row width correct?" \
                        % (len(buf),row_width))
            break
            
        pixel_row = array.array( "B", buf )

        pixel_sum = reduce( lambda x,y:x+y, pixel_row )
#        print pixel_sum
        pixel_sums.append( pixel_sum )

#        (max_pixel,min_pixel,median_pixel) = miscmath.find_max_min_and_median( pixel_row )
        (mean,stddev) = miscmath.calculate_standard_deviation( pixel_row )
#        print max_pixel, min_pixel, median_pixel, mean, stddev
        print mean, stddev

    infile.close()
    return pixel_sums

if __name__=='__main__' :
    # TODO better command line parsing
    filename = sys.argv[1]
    row_width = int(sys.argv[2])

    pixel_sums = sum_image_rows( filename, row_width )

    f_row_width = float(row_width)
    row_averages = map( lambda num : num/f_row_width, pixel_sums )

    (max_pixel,min_pixel,median_pixel) = miscmath.find_max_min_and_median( pixel_sums )
    (mean,stddev) = miscmath.calculate_standard_deviation( pixel_sums )

    print "# min=",min_pixel
    print "# max=",max_pixel
    print "# median=",median_pixel
    print "# mean=", mean
    print "# stddev=", stddev

#    for num in pixel_sums :
#        print num
#    for num in row_averages :
#        print int(round(num))

