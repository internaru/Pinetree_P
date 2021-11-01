#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Collection of nifty simple numerical stuff.
#
# davep 21-Dec-2006
# $Id: miscmath.py 429 2008-06-04 17:57:49Z davep $

import math

def calculate_standard_deviation( samples ) :
    num_samples = len(samples)

    mean = 0
    for i in range( 0, num_samples ) :
        mean += samples[i]
    
    # make sure mean is always float whether or not samples[] is array of
    # integer or float 
    mean = float(mean) / float(num_samples)

    stddev = 0
    for i in range(0,num_samples) :
        stddev += (samples[i] - mean)**2
    stddev = math.sqrt( stddev/float(num_samples) )

    return (mean,stddev)

def find_max_min_and_median( samples ) :
    # sort() is destructive so make a copy
    samples_sorted = []
    samples_sorted.extend( samples )
    samples_sorted.sort()

    num_elements = len(samples)
    mid = num_elements / 2

    if num_elements%2 == 0 :
        # take mean of two middle elements
        median = (samples_sorted[ mid ] + samples_sorted[ mid-1 ]) / 2.0
    else :
        # always return float regardless of original type
        median = float(samples_sorted[ mid ])
    
    return samples_sorted[-1], samples_sorted[0], median

def calc_and_write_stats( values, outfile ) :

    mean,stddev = calculate_standard_deviation( values )

    max_value, min_value, median_value = find_max_min_and_median( values )

    print >>outfile, "# num_elements=%s" % len(values)
    print >>outfile, "# max_value=%s" % str(max_value)
    print >>outfile, "# min_value=%s" % str(min_value)
    print >>outfile, "# min_max_delta=%s" % str(abs(max_value-min_value))
    print >>outfile, "# median=%s" % str(median_value)
    print >>outfile, "# mean=%s" % str(mean)
    print >>outfile, "# stddev=%s" % str(stddev)

def calc_means( buf, image_width, image_height ) : 
    """Calculate the mean of the pixel columns of an image. The image is stored
    in a single array, 'buf'."""
    # calculate pixel averages
    pixel_sums = [ 0L ] * image_width
    column = 0
    for p in buf :
        pixel_sums[ column ] += p
        column = (column + 1) % image_width 

    means = [ n / float(image_height) for n in pixel_sums ]

    return means

def calc_medians( buf, image_width, image_height ) : 
    """Calculate the medians of the pixel columns of an image. The image is stored
    in a single array, 'buf'."""
    # Get the pixel medians by using an array of indices into the buffer. We
    # start with an index to the start of each pixel row: a pixel column. Get
    # the pixels from that column, find the median, increment pixel indices to
    # next column.
    #
    medians = []

    # the -1 is to start at the first column of the last row
    # the +1 is so range() will include the final number
    row_indices = range( 0, image_width*(image_height-1)+1, image_width )
    assert len(row_indices)==image_height, len(row_indices)

    for col in range(image_width) :
        column = []
        for i in row_indices :
            column.append( buf[i] )
        assert len(column)==image_height, len(column)

        max_value, min_value, median_value = find_max_min_and_median( column )
        medians.append( median_value )

        # advance to next column
        row_indices = [ x+1 for x in row_indices ] 

    return medians

