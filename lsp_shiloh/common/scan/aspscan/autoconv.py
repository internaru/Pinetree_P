#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# automatically convert a .gray file to .tif based on file size and number of
# pixels from command line
# davep 09-Apr-2009
#
# Added support to for r,g,b files
# davep 27-Aug-2010
# 
# Added using PIL for faster convert. Added support for .rgb files.
# davep 15-Dec-2010
 
import sys
import os
import stat
import subprocess

# davep 14-Dec-2010 ; try to use the PIL (Python Imaging Library). If not
# installed, we'll fall back to ImageMagick
try :
    import Image
    import piltool
    g_have_pil = True
except ImportError :
    # we'll use ImageMagick instead
    g_have_pil = False

#g_have_pil = False

class SizeError( Exception ) :
    def __init__( self, errmsg="", filename="" ) :
        Exception.__init__( self, errmsg ) 
        self.filename = filename

def verify_size( filename, file_size_bytes, bytes_per_row ) :

    if file_size_bytes % bytes_per_row != 0 :
        errmsg = "File \"{0}\" is {1} bytes which is not evenly divisible by bytes_per_row=%{2} ".format( 
                    filename, file_size_bytes, bytes_per_row )

        errmsg += "({0}%{1}={2}) ".format(
                    file_size_bytes, bytes_per_row, 
                    file_size_bytes%bytes_per_row)

        errmsg += "Is your pixels per row correct?"
        raise SizeError( errmsg )

def convert_using_pil( filename_list, outfilename, bits_per_pixel, pixels_per_row, total_rows ) : 
    if len(filename_list) == 1 : 
        # assuming a .gray file 
        assert bits_per_pixel==8, bits_per_pixel
        piltool.convert_gray( filename_list[0], outfilename, (pixels_per_row,total_rows))
    else : 
        # assuming three files in R,G,B order
        piltool.combine_rgb( filename_list, outfilename, (pixels_per_row, total_rows) )

def convert_using_imagemagic( filename_list, outfilename, bits_per_pixel, pixels_per_row, total_rows ) : 
    cmdline = "{0} -depth {1} -size {2}x{3} ".format( 
        "convert", bits_per_pixel, pixels_per_row, total_rows )

    if len(filename_list) == 1 : 
        # assuming a .gray file 
        cmdline += "gray:{0} {1}".format( filename_list[0], outfilename ) 
    else : 
        # assuming three files in R,G,B order
        assert len(filename_list)==3, len(filename_list)

        cmdline += "-combine R:{filenames[0]} G:{filenames[1]} B:{filenames[2]} {0}".format( 
            outfilename, filenames=filename_list ) 

    subprocess.check_call( cmdline.split() )

def auto_convert( filename_list, pixels_per_row, bits_per_pixel=8 ) :
    
    # grab the basename and extension of the first file; basename is used as
    # the output filename; ext is used to decide how to convert the file(s)
    basename,ext = os.path.splitext( filename_list[0] )
    outfilename = basename + ".tif"

    assert bits_per_pixel in (8,16), bits_per_pixel

    bytes_per_row = pixels_per_row * (bits_per_pixel/8)

    # get the filesize(s), verify the pixels_per_row evenly divides into the
    # file byte size
    file_size_list = []
    for filename in filename_list :
        file_size_bytes = os.stat(filename)[stat.ST_SIZE] 
        verify_size( filename, file_size_bytes, bytes_per_row ) 
        file_size_list.append( file_size_bytes ) 

    total_rows = file_size_bytes / bytes_per_row

    if ext in (".rgb", ".bgr"): 
        total_rows /= 3
    elif ext in (".xrgb",".rgbx") : 
        total_rows /= 4

    print "filesizes={0} total_rows={1}".format( file_size_list, total_rows )

    if g_have_pil and bits_per_pixel==8 : 
        # AFAIK PIL only handles 8-bpp
        assert bits_per_pixel==8, bits_per_pixel
        if ext==".gray" : 
            piltool.convert_gray( filename_list[0], outfilename, (pixels_per_row,total_rows))
        elif ext==".bgr" : 
            piltool.convert_bgr( filename_list[0], outfilename, (pixels_per_row,total_rows))
        elif ext==".rgb" : 
            piltool.convert_rgb( filename_list[0], outfilename, (pixels_per_row,total_rows))
        elif ext==".r" : 
            piltool.combine_rgb( basename, outfilename, (pixels_per_row, total_rows) )
        elif ext==".xrgb" : 
            piltool.convert_xrgb( filename_list[0], outfilename, (pixels_per_row,total_rows))
        elif ext==".rgbx" : 
            piltool.convert_rgbx( filename_list[0], outfilename, (pixels_per_row,total_rows))
        else :
            raise Exception( "Unknown/unsupported file extension \"{0}\"".format( ext ) )
    else : 
        # verify all files in the list are the same size (otherwise ImageMagick
        # corrupts the image)
        if not reduce( lambda x,y : x and y, 
                  map( lambda num : num==file_size_list[0], file_size_list ) ) : 
            raise Exception( "File sizes are not equal. Cannot convert." )

        convert_using_imagemagic( filename_list, outfilename, bits_per_pixel,
                    pixels_per_row, total_rows )
    
def usage() : 
    e =\
"""{0} - attempt to auto-convert raw pixel file(s) to a .tif file.
usage: {0} [filename(s)] [pixel widths]

    filename(s) can be one .gray file or three (.r,.g,.b) files or one .rgb file.
    pixel widths is one or more widths to attempt to use for conversion.
    
    Currently only an 8-bpp pixel is supported.

Examples:
     # convert out.[rgb] to out.tif, trying each of 2080, 2096, 2032 to find a fit
     {0} out.r out.g out.b 2080 2096 2032 
     
     # convert TestChart42.gray to TestChart42.tif, trying only 4992 pixel width
     {0} TestChart42.gray 4992

     # convert foo10.rgb to foo10.tif, trying only 2272 pixel width
     {0} foo10.rgb 2272
"""

    print e.format( sys.argv[0] )
        
def sort_to_rgb_order( filename_list ) : 
    # brute force sort the filename list into files with extensions in 
    # .R, .G, .B order

    # get a list of the filename extensions, all lowercase
    ext_list = [ os.path.splitext( f )[1].lower() for f in filename_list ] 
#    print ext_list

    # run through the list, in extension order, building a new list
    outfilename_list = []
    for ext in ( ".r", ".g", ".b" ) : 
        idx = ext_list.index( ext ) 
        outfilename_list.append( filename_list[idx] )

    return outfilename_list

def parse_args() : 
    if len(sys.argv) == 1 : 
        usage()
        sys.exit(1)

    # Verify command line. Want one or three filenames followed by one or more
    # integers (the pixels_per_row values to try).  
    # TODO

    # look for .gray filename OR .r .g .b filenames on command line
    basename,ext = os.path.splitext( sys.argv[1] )
#    print basename, ext

    ext = ext.lower()

    if ext in ( ".gray", ".rgb", ".xrgb", ".rgbx", ".bgr" ) :
        filename_list = [ sys.argv[1], ]
        ppr_list = [ int(pixels_per_row) for pixels_per_row in sys.argv[2:] ]

    elif ext in ( '.r', '.g', '.b' ) :
        filename_list = sort_to_rgb_order( sys.argv[1:4] )
        ppr_list = [ int(pixels_per_row) for pixels_per_row in sys.argv[4:] ]

    else : 
        print >>sys.stderr, "Unknown file extension \"{0}\"".format( ext )
        sys.exit(1)

    return filename_list, ppr_list

def main() :

    filename_list, ppr_list = parse_args()
#    print filename_list
#    print ppr_list

    success = False
    for ppr in ppr_list : 
        try : 
            auto_convert( filename_list, ppr )
            success = True
            break
        except SizeError, e :
            pass

    if not success : 
        if len(ppr_list) == 1 : 
            print >>sys.stderr, "Failed to convert after trying width {0} ".format( ppr_list[0] )
        else : 
            print >>sys.stderr, "Failed to convert after trying widths \"%s\"." % str(ppr_list) 
        print >>sys.stderr, "Is your pixels per row correct?"

        # return an exit failure 
        sys.exit(1)
        
    
if __name__ == '__main__' :
    main()

