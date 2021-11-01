#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Prepare an image for firmware's pipenet.  
# Split into three planes, each plane DMA aligned.
#
# Requires ImageMagick.
#
# davep 25-Aug-2010
#
# davep 23-May-2011 ; convert to PIL (was ImageMagick)

import sys
import os.path
import Image

def prep_image_for_dma( infilename, dmaalign ) :
    im = Image.open( infilename ) 

    imwidth,imheight = im.size[0],im.size[1]

    print "image size=",imwidth, imheight 

    dmawidth=(imwidth / dmaalign) * dmaalign

    new_size=dmawidth * imheight

    print "dma width=",dmawidth

    im2 = im.crop( (0,0,dmawidth,imheight ) )
#    im2.save( "out.tif" )

    if im.mode=="RGB": 
        imlist = im2.split()
        channel_list = ( "r", "g", "b" )
    elif im.mode=="L" :
        imlist = ( im2, )
        channel_list = ( "gray", )
    else:
        # WTF?
        assert 0, im.mode
    del im

    basename,ext = os.path.splitext( infilename )

    for idx,im in enumerate(imlist) : 

        # save the plane into a file of raw pixels
        outfilename = "{0}_{1}x{2}.{3}".format( basename, dmawidth, imheight, channel_list[idx] )
        outfile = open( outfilename, "wb" )
        outfile.write( im.tostring() )
        outfile.close()

        # save the plane as an individual viewable image
        outfilename = "{0}_{1}x{2}_{3}.tif".format( basename, dmawidth, imheight, channel_list[idx] )
        im.save( outfilename )

def usage() : 
    print >>sys.stderr, "{0} - prepare an image for use with PipeNet".format( sys.argv[0] )
    print >>sys.stderr, "usage:"
    print >>sys.stderr, "{0} image_filename dma_align_in_bytes".format( sys.argv[0] )

def main() : 
    if len(sys.argv) != 3 : 
        usage()
        sys.exit(1)

    infilename=sys.argv[1]
    dmaalign=int(sys.argv[2])

    # catch a few common errors here and give a friendlier error message
    if not os.path.exists( infilename ) :
        print >>sys.stderr, "file {0} does not exist".format( infilename )
        sys.exit(1)

    try : 
        infile = open(infilename,"r")
        infile.close()
    except IOError,e :
        print "Unable to open {0} : {1}".format( infilename, e )
        sys.exit(1)

    prep_image_for_dma( infilename, dmaalign )

if __name__=='__main__' :
    main()

