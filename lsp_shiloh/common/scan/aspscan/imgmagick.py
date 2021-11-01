#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# simple interface to the ImageMagick command line
# davep 28-Apr-06
#

import sys
import os
import stat
import string
import subprocess

debug=0

def whoami():
    # Recipe 66062: Determining Current Function Name
    # http://code.activestate.com/recipes/66062/
    import sys
    return sys._getframe(1).f_code.co_name

def get_image_size( filename ) :
    # Using ImageMagick's 'identify', get the image size ; looks like "320x240"
    f = os.popen( "identify " + filename )
    lines=f.readlines()
    ret = f.close()
    if debug : 
        print lines
    if ret != None :
        print "ImageMagick's identity failed : %s" % str(ret)
        sys.exit(1)

    fields = string.split( lines[0] )

    # make sure we have "number'x'number
    fields = string.split( fields[2], 'x' )
    if debug : 
        print fields
    if len(fields) != 2 :
        print "Invalid output from ImageMagick's identity"
        sys.exit(1)

    try :
        width = int(fields[0])
        height = int(fields[1])
    except ValueError :
        print "Invalid output from ImageMagick's identity; couldn't find integers"
        sys.exit(1)

    return width,height

def replace_extension( filename, new_extension ) :
    basename, ext = os.path.splitext( filename )
    return basename + "." + new_extension

def calc_image_dimension( filename, known_dimension_pixels, pixel_bit_depth ) :
    # Given X or Y dimension in pixels, calculate the missing dimension.
    # Originall created to handle images where I track the pixels per row but
    # not the number of rows.
    filesize_bytes = os.stat( filename )[ stat.ST_SIZE ]
    filesize_pixels = filesize_bytes / (pixel_bit_depth/8)
    return filesize_pixels / known_dimension_pixels

def make_size_arg( filename, kargs, bit_depth ) :
    if "pixels_x" in kargs :
        pixels_x = kargs["pixels_x"]

        if "pixels_y" in kargs :
            pixels_y = kargs["pixels_y"]
        else :
            # guess the pixels_y size
            pixels_y = calc_image_dimension( filename, pixels_x, bit_depth)

    elif "pixels_y" in kargs :
        pixels_y = kargs["pixels_y"]
        if "pixels_x" in kargs :
            pixels_x = kargs["pixels_x"]
        else :
            # guess the pixels_x size
            pixels_x = calc_image_dimension( filename, pixels_y, bit_depth)

    else :
        raise Exception( "Must have one or both of \"pixels_x\" and \"pixels_y\"" )

    size = "%dx%d" % ( pixels_x, pixels_y )

    return size

def convert_file( src_filename, dst_filename, **kargs ) :

    if debug : 
        print whoami(), kargs

    defaults = {}
    defaults["bit_depth"] = 8

    convertcmd = [ "convert", ]

    convertcmd.append( "-depth" )
    if "bit_depth" in kargs :
        bit_depth = kargs["bit_depth"] 
    else :
        bit_depth = defaults["bit_depth"]
    convertcmd.append( str(bit_depth) )

    convertcmd.append( "-size" )
    convertcmd.append( make_size_arg( src_filename, kargs, bit_depth ) )

    # davep 06-Jan-2009 ; add support for explicitly sending arguments to
    # ImageMagick
    if "explicit_args" in kargs :
        convertcmd.extend( kargs["explicit_args"] )

    # Source File
    if "typecast" in kargs :
        infilename = "%s:%s" % (kargs["typecast"], src_filename )
    else :
        infilename = src_filename
    convertcmd.append( infilename )

    # Destination File
    convertcmd.append( dst_filename )

    # make all arguments into strings
    convertcmd_str = [ str(x) for x in convertcmd ]

    if debug : 
        print "%s cmd=%s" % (whoami(), str(convertcmd) )
        print "%s strcmd=%s" % (whoami(), convertcmd_str)
    
    # davep 23-Jul-2012 ; convert to subprocess
    subprocess.check_call( convertcmd )

def test() :
    src_filename = "foo12.gray"
    dst_filename = "foo12_cal_gray.tif"
    pixels_x = 2576
    bit_depth = 16

    convert_file( src_filename, dst_filename, 
                    pixels_x=pixels_x, bit_depth=bit_depth )

if __name__ == '__main__' :
    test()

