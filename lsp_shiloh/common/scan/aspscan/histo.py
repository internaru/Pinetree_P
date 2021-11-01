#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# count an 8-bit pixel histogram of file specified on command line
# davep 15-May-06
#
# Adding some histogram equalization experiments.
# davep 15-Aug-2009
#
# $Id: histo.py 454 2008-06-18 20:34:22Z davep $

import sys
import array
import math

def to_file( num_list, outfilename ) :
    outfile = open( outfilename, "w" ) 

    for n in num_list :
        if type(n)==type(1) :
            outfile.write( "%d\n" % n ) 
        else  :
            outfile.write( "%f\n" % n ) 

    outfile.close()

def calc_cdf( histogram ) :
    # http://en.wikipedia.org/wiki/Histogram_equalization
    # Cumulative Distribution Function
    
    total_pixels = sum(histogram)
    px_list = []

    # px, pxi are from the equation in
    # http://en.wikipedia.org/wiki/Histogram_equalization
    for n in histogram : 
        px = float(n)/total_pixels
        px_list.append( px )

#    to_file( px_list, "pxi.dat" )

    cdf_list = []
    total = 0.0
    for n in px_list : 
        total += n
        cdf_list.append( total ) 
        
    return cdf_list
    
def tinker_with_equalization( histogram, infilename ) :
    # http://en.wikipedia.org/wiki/Histogram_equalization
    
    cdf_list = calc_cdf( histogram )

    to_file( cdf_list, "cdf.dat" )

    infile = open( infilename, "rb" )
    outfile = open( "out.gray", "wb" )

    img_min = 0
    img_max = 255

    while 1 :
        buf = infile.read( 65536 )
        if len(buf) <= 0 :
            break

        # start clean every time since fromstring appends
        # note: hardwired to 8-bpp
        inpixels = array.array( "B" )
        inpixels.fromstring( buf )

        outpixels = array.array( "B" )
        outpixels.fromstring( buf )

        for i in range(len(inpixels)) :
            inp = inpixels[i]

            # reverse lookup
            outp = cdf_list[inp]

            # re-scale to [0,255]
            outpixels[i] = min( 255, int( round( outp * (img_max-img_min)+img_min) ) )

        outpixels.tofile( outfile )

    outfile.close()
    infile.close()

    infile = open( "out.gray", "rb" )
    histo2 = make_histogram( infile, 8 )
    infile.close()

    to_file( histo2, "histo2.dat" )

    cdf2 = calc_cdf( histo2 )

    to_file( cdf2, "cdf2.dat" )


def make_histogram( infile, bpp=8, pixel_byteorder="big" ) :

    if bpp==8 :
        histogram = [0] * 256
        fmt = "B" 
    elif bpp==16 :
        histogram = [0] * 65536
        fmt = "H" 
    else :
        assert 0, "Bad bpp %d"%bpp

    while 1 :
        buf = infile.read( 65536 )
        if len(buf) <= 0 :
            break

        # start clean every time since fromstring appends
        pixels = array.array( fmt )
        pixels.fromstring( buf )

        if bpp==16 and sys.byteorder != pixel_byteorder :
            pixels.byteswap()

        for p in pixels :
            histogram[p] += 1

    return histogram

        
def main() :
    if len(sys.argv) != 3 :
        print >>sys.stderr,"usage: %s filename bpp" % sys.argv[0]
        print >>sys.stderr,"output sent to stdout"
        print >>sys.stderr,"bpp must be 8 or 16"
        sys.exit(1)

    infilename = sys.argv[1]
    infile = open( infilename )
    bpp = int(sys.argv[2])

    histogram = make_histogram( infile, bpp, pixel_byteorder="big" )

    infile.close()

#    print len(histogram)
#    for h in histogram :
#        print h
    print "# pixel pixel-count log10(pixel-count)"
    for i in range(len(histogram)) :
        if histogram[i]==0 :
            print "%d %d 0" % (i,histogram[i])
        else :
            print "%d %d %f" % (i,histogram[i],math.log10(histogram[i]))

    # davep 15-Aug-2009 ; tinkering with histogram equalization
    if 0 :
        max_num = max(histogram)
        outfile = open( "histo.dat", "w" )
        for n in histogram :
            outfile.write( "%d %f\n" % (n, (n/float(max_num))*255.))
        outfile.close()

        tinker_with_equalization( histogram, infilename )
        
        
if __name__ == '__main__' :
    main()

