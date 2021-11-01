#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Split a file into three planes using the Python Imaging Library
# davep Jan-2010

import sys
import os
import Image

Image.DEBUG=1

infilename = sys.argv[1]

print infilename

im = Image.open( infilename )
im.load()
print im.format, im.size, im.mode

if im.mode != 'RGB' :
    print >>sys.stderr, "{0} is not an RGB file! Cowardly refusing to split.".format( infilename )
    sys.exit(1)

bands = im.split()
print bands

basename = os.path.splitext(infilename)[0]
band_names = im.getbands()

for b in zip( band_names, bands ) :

    file_ext = b[0]
    image_data = b[1]

    outfilename = basename + "_" + file_ext + ".png"
    print outfilename

    grayfilename = basename + "." + file_ext 
    outfile = open( grayfilename, "wb" )
    outfile.write( image_data.tostring() )
    outfile.close()

    print image_data.format, image_data.size, image_data.mode

    image_data.save( outfilename )

