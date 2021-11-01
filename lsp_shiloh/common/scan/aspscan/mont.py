#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# davep 19-Dec-05
# convert a collection of scan regression test files into a nice montage image
#

import os
import sys
import re
import time
import string
import Image
import subprocess

import scan

def read_image_sizefile( sizefilename ) :
    # read the images sizes from the files 
    # expect to see lines like:
    # dpi=150 rows=4725 ppr=1264 pprp=1264 file=150 datatype=2
    # 
    # the filename must NOT have any embedded spaces or I'll break below

    try :
        infile = open( sizefilename, "r" )
    except IOError, e :
        raise "Unable to open size file \"%s\" : %s" % (sizefilename,e.strerror)

    image_info_list = []
    linenumber = 0

    fieldre = re.compile( "^([a-zA-Z]+)=(.+)$" )
    while 1 :
        # maximum line length of 256
        line = infile.readline( 256 )
        if len(line) <= 0 :
            break
        linenumber += 1

        # break up the info in the line into data in the image(s) (we can get
        # back multiple images when we scan into .r .g .b planar files)
        image_info = scan.parse_image_info_line( line )
        assert type(image_info) == type([])
        image_info_list.extend( image_info )

    infile.close()

    return image_info_list

def dpi_and_datatype_compare( x, y ) :
    # called by a list's .sort method

    # Sort by increasing dpi. Within the DPI, sort by datatype.
    if x['dpi'] == y['dpi'] :
        return cmp( x['datatype'], y['datatype'] )
    else :
        return cmp( x['dpi'], y['dpi'] )

def make_montage( file_list, outfilename ) :

    now = time.ctime()

    # we want the montage to be the same dpi along the rows 
    # and the order Red, Green, Blue, Mono, RGB down the columns
    # Makes for an odd sorting algorithm which could probably be done much
    # faster than the slow way I'm doing it below

    file_list.sort( dpi_and_datatype_compare )

    file_name_list = [ f['filename'] for f in file_list ]

    # want the same DPI across the rows so count how many files in a certain
    # dpi
    tile_width = 0
    for f in file_list :
        if f["dpi"] != file_list[0]['dpi'] :
            break
        tile_width += 1

    tile_height = len(file_name_list) / tile_width

    # davep 14-Mar-2013 ; convert from my runcmd to subprocess
    cmd = "montage -verbose -label %f -tile {0}x{1} ".format( tile_width,tile_height )
    cmd += " ".join(file_name_list)
    cmd += ' -geometry "256x256+2+2>" -title "{0}" -depth 8 {1}'.format( now, outfilename )
    print cmd

    subprocess.check_call( cmd, shell=True )

def get_file( infilename ) : 
    infile = open( infilename, "rb" ) 
    buf = infile.read()
    infile.close()
    return buf

def convert_xrgb( infilename, outfilename, size, new_size ) : 
    buf = get_file( infilename )
    im = Image.fromstring( "RGBX", size, buf )
    im_list = im.split()
    im2 = Image.merge( "RGB", im_list[1:4] )
    im3 = im2.resize( new_size )
    im3.save( outfilename )

def convert_rgbx( infilename, outfilename, size, new_size ) : 
    buf = get_file( infilename )
    # RGBX is actually a format PIL recognized. Weird.
    im = Image.fromstring( "RGBX", size, buf )
    im_list = im.split()
    im2 = Image.merge( "RGB", im_list[0:3] )
    im3 = im2.resize( new_size )
    im3.save( outfilename )

def convert_raw( infilename, outfilename, size, mode, new_size ) :
    buf = get_file( infilename )
    im = Image.new( mode, size )
    im.fromstring( buf )
    im3 = im.resize( new_size )
    im3.save( outfilename )

def convert_rgb( infilename, outfilename, size, new_size ) :
    return convert_raw( infilename, outfilename, size, "RGB", new_size )

def convert_gray( infilename, outfilename, size, new_size ) :
    return convert_raw( infilename, outfilename, size, "L", new_size )

def convert_file( image_info ) :
    size = (image_info['pprp'], image_info['rows'] )

    # force the output image to at most 256 pixels on either side
    scale = 256.0 / size[0]
    new_size = ( 256, int(round(size[1]*scale)) )

    srcfileext = scan.asp_datatype_to_file_extension( image_info["datatype"] )
    infilename = image_info['file'] + "." + srcfileext
    outfilename = image_info['file'] + "_" + srcfileext + ".tif"

    print infilename, size, outfilename, new_size

    exthash = {
        "xrgb" : convert_xrgb, 
        "rgbx" : convert_rgbx,
        "rgb"  : convert_rgb,
        "gray" : convert_gray,
        "r"    : convert_gray,
        "g"    : convert_gray,
        "b"    : convert_gray 
    }

    convert_func = exthash[ srcfileext ]
    convert_func( infilename, outfilename, size, new_size )

    fileinfo = { 'filename': outfilename,
                 'dpi': image_info['dpi'],
                 'datatype': image_info['datatype'] }

    return fileinfo

def test_montage() :

    outfile_list = [{'datatype': 14, 'dpi': 75, 'filename': '75_rgb.tif'}, {'datatype': 14,
    'dpi': 150, 'filename': '150_rgb.tif'}, {'datatype': 14, 'dpi': 200,
    'filename': '200_rgb.tif'}, {'datatype': 14, 'dpi': 300, 'filename':
    '300_rgb.tif'}, {'datatype': 14, 'dpi': 500, 'filename': '500_rgb.tif'},
    {'datatype': 14, 'dpi': 600, 'filename': '600_rgb.tif'}, {'datatype': 14,
    'dpi': 1000, 'filename': '1000_rgb.tif'}, {'datatype': 14, 'dpi': 1200,
    'filename': '1200_rgb.tif'}, {'datatype': 1, 'dpi': 75, 'filename':
    '75_xrgb.tif'}, {'datatype': 1, 'dpi': 150, 'filename': '150_xrgb.tif'},
    {'datatype': 1, 'dpi': 200, 'filename': '200_xrgb.tif'}, {'datatype': 1,
    'dpi': 300, 'filename': '300_xrgb.tif'}, {'datatype': 1, 'dpi': 500,
    'filename': '500_xrgb.tif'}, {'datatype': 1, 'dpi': 600, 'filename':
    '600_xrgb.tif'}, {'datatype': 1, 'dpi': 1000, 'filename': '1000_xrgb.tif'},
    {'datatype': 1, 'dpi': 1200, 'filename': '1200_xrgb.tif'}, {'datatype': 11,
    'dpi': 75, 'filename': '75_rgbx.tif'}, {'datatype': 11, 'dpi': 150,
    'filename': '150_rgbx.tif'}, {'datatype': 11, 'dpi': 200, 'filename':
    '200_rgbx.tif'}, {'datatype': 11, 'dpi': 300, 'filename': '300_rgbx.tif'},
    {'datatype': 11, 'dpi': 500, 'filename': '500_rgbx.tif'}, {'datatype': 11,
    'dpi': 600, 'filename': '600_rgbx.tif'}, {'datatype': 11, 'dpi': 1000,
    'filename': '1000_rgbx.tif'}, {'datatype': 11, 'dpi': 1200, 'filename':
    '1200_rgbx.tif'}, {'datatype': 6, 'dpi': 75, 'filename': '75_gray.tif'},
    {'datatype': 6, 'dpi': 150, 'filename': '150_gray.tif'}, {'datatype': 6,
    'dpi': 200, 'filename': '200_gray.tif'}, {'datatype': 6, 'dpi': 300,
    'filename': '300_gray.tif'}, {'datatype': 6, 'dpi': 500, 'filename':
    '500_gray.tif'}, {'datatype': 6, 'dpi': 600, 'filename': '600_gray.tif'},
    {'datatype': 6, 'dpi': 1000, 'filename': '1000_gray.tif'}, {'datatype': 6,
    'dpi': 1200, 'filename': '1200_gray.tif'}]

#    print outfile_list

    make_montage( outfile_list, "out.png" )

def main() :
    # eventually get this from the command line
#    sizefilename = "out.dat"
#    sizefilename = "regress.dat"
    sizefilename = sys.argv[1]
    outfilename = sys.argv[2]

    image_info_list = read_image_sizefile( sizefilename )
    for i in image_info_list :
        print i 

    # now go through each input file, find the correct size for the DPI,
    # resolution, and output type
    outfile_list = []
    for image in image_info_list :
        outfile = convert_file( image )
        outfile_list.append( outfile )

    print outfile_list

    make_montage( outfile_list, outfilename )

if __name__ == '__main__' :
    main()
#    test_montage()

