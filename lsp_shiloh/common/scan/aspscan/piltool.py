#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Handy utility functions for making PIL (Python Imaging Library) a little
# easier to use.  It's a work in progress.
#
# davep 09-Feb-2010
#
# davep 03-Feb-2011 ; adding RGBX
#
# davep 09-Apr-2012 ; adding 'buf' methods to write raw strings instead of just
#                     files (which I should have done in the first place)

try:
    import Image
except ImportError:
    # python 3.x use Pillow
    from PIL import Image
import logging

logger = logging.getLogger(__name__)

def convert_raw_buf_to_image( buf, outfilename, size, mode, **kwargs ) :
    logger.info("convert_raw_buf_to_image() mode={0} size={1} len(buf)={2} outfilename={3}".format(
            mode, size, len(buf), outfilename))
    im = Image.new( mode, size )
    im.fromstring( buf )

    # davep 24-Mar-2014 ; adding dpi, etc. (PIL interprets the kwargs)
    im.save( outfilename, format=None, **kwargs )

def convert_raw_to_image( infilename, outfilename, size, mode, **kwargs ) :
    # is there a better way than reading the entire file into memory?
    infile = open( infilename, "rb" )
    buf = infile.read()
    infile.close

    convert_raw_buf_to_image( buf, outfilename, size, mode, **kwargs )

def convert_rgbx_buf( buf, outfilename, size, **kwargs ) : 
    # RGBX is actually a format PIL recognized. Weird.
    im = Image.fromstring( "RGBX", size, buf )

    im_list = im.split()

    im2 = Image.merge( "RGB", im_list[0:3] )
    im2.save( outfilename, format=None, **kwargs )

def convert_rgbx( infilename, outfilename, size, **kwargs ) : 
    infile = open( infilename, "rb" ) 
    buf = infile.read()
    infile.close()

    convert_rgbx_buf( buf, outfilename, size )

def convert_xrgb_buf( buf, outfilename, size, **kwargs ) : 
    # use RGBX because it's a 4-plane format. We'll use the G,B,X planes.
    im = Image.fromstring( "RGBX", size, buf )
    im_list = im.split()

    im2 = Image.merge( "RGB", im_list[1:4] )
    im2.save( outfilename, format=None, **kwargs )

def convert_xrgb( infilename, outfilename, size, **kwargs ) : 
    infile = open( infilename, "rb" ) 
    buf = infile.read()
    infile.close()

    convert_xrgb_buf( buf, outfilename, size, **kwargs )

def convert_bgr( infilename, outfilename, size, **kwargs ) :
    infile = open( infilename, "rb" ) 
    buf = infile.read()
    infile.close()

    # Use RGB. We'll just rearrange the planes.
    im = Image.fromstring( "RGB", size, buf )
    im_list = im.split()

    im2 = Image.merge( "RGB", (im_list[2], im_list[1], im_list[0])  )
    im2.save( outfilename, format=None, **kwargs )

def convert_rgb_buf( buf, outfilename, size ) :
    return convert_raw_buf_to_image( buf, outfilename, size, "RGB", **kwargs )

def convert_rgb( infilename, outfilename, size ) :
    return convert_raw_to_image( infilename, outfilename, size, "RGB", **kwargs )

def convert_gray_buf( buf, outfilename, size, **kwargs ) :
    mode = kwargs.get( "mode", "L" )
    if "mode" in kwargs:
        del kwargs["mode"]
    return convert_raw_buf_to_image( buf, outfilename, size, mode, **kwargs )

def convert_gray( infilename, outfilename, size, **kwargs ) :
    mode = kwargs.get( "mode", "L" )
    if "mode" in kwargs:
        del kwargs["mode"]
    return convert_raw_to_image( infilename, outfilename, size, mode, **kwargs )

def combine_rgb( infilename_base, outfilename, size, **kwargs ) :
    # combine three planes (r,g,b) into a single .rgb file
    # as fast as possible

    outfile = open( outfilename, "wb" )

    infile_list = [ open( infilename_base + ext, "rb" ) for ext in ( '.r', '.g', '.b' ) ]
    logger.debug("combine_rgb() infile_list={0}".format(infile_list))

    planedata_list = [ infile.read() for infile in infile_list ] 

    im_list = [ Image.new( "L", size ) for i in planedata_list ]
    for i in range(len(im_list)) :
        im_list[i].fromstring( planedata_list[i] )
    
    im2 = Image.merge( "RGB", im_list )

    # davep 24-Mar-2014 ; adding dpi
    im2.save(outfilename,format=None,**kwargs)


    [ f.close() for f in infile_list ]

def main() :
    import sys
    import os.path

    infilename = sys.argv[1]
    outfilename = sys.argv[2]
    size_x = int(sys.argv[3])
    size_y = int(sys.argv[4])

    basename,ext = os.path.splitext( infilename )

    convert_map = {
        ".xrgb" : convert_xrgb,
        ".rgbx" : convert_rgbx,
        ".rgb"  : convert_rgb,
        ".bgr"  : convert_bgr,
        ".gray" : convert_gray,
        ".r"    : convert_gray,
        ".g"    : convert_gray,
        ".b"    : convert_gray 
    }
    convert_func = convert_map[ ext ]
    convert_func( infilename, outfilename, (size_x,size_y) )

if __name__=='__main__' :
    logging.basicConfig(level=logging.DEBUG)
    main()

