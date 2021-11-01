#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Decode split segment scan. In other words, pretend to be CISX.
# Rewrote using numpy. Still doesn't do dummy pixels. Doesn't handle firmware
# margins. 
#
# davep 15-Jun-2011
#

import sys
import Image
import numpy as np

def run_cisx( plane, index_array ) : 
    outrows = []
    total_rows = plane.shape[0]

    # TODO is there an even faster way to do this rather than indexing through
    # the rows?

    for i in range(total_rows) : 
        out = plane[i][ index_array ]
        outrows.append( out )
        out = None

    outarray = np.asarray( outrows, "uint8" )
    return outarray

def make_index_array( pixels_per_row ) : 
    index_array = []

    # make an array of indices of weird pixel positions as come from the
    # 3-segment sensor 
    # strange order (2,0,1) because he-who-must-not-be-named returning image pixels in weird order
    index_array.extend( range(2,pixels_per_row,3) )
    index_array.extend( range(0,pixels_per_row,3) )
    index_array.extend( range(1,pixels_per_row,3) )
#    print index_array[0:10]

    return index_array

def cisx_file( infilename, outfilename ) : 
    im = Image.open( infilename ) 
    im.load()
    band_list = im.split()
    print im.mode, im.size
    pixels_per_row,total_rows = im.size[0],im.size[1]

    print "ppr={0} rows={1}".format( pixels_per_row, total_rows )

    index_array = make_index_array( pixels_per_row )

    outimg_list = []
    for i,band in enumerate(band_list) : 
        print "cisxing plane {0}...".format( i )
        out0 = run_cisx( np.asarray( band, "uint8" ), index_array )
        result = Image.fromarray(out0,"L")
#        result.save( "out{0}.tif".format(i) )
        outimg_list.append( result )
        result = None

    print "writing {0}...".format( outfilename )
    outim = Image.merge( im.mode, outimg_list ) 
    outim.save( outfilename )

def main() : 
    infilename = sys.argv[1]
    outfilename = sys.argv[2]
    
    cisx_file( infilename, outfilename ) 

if __name__=='__main__' : 
    main()

