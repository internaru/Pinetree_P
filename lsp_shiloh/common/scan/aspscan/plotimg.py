#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Convert an image into single planes (e.g., Lab, YCC, RGB) per file. Convert
# to gnuplot-read data files.
#
# Written to graph single columns cut from scans of the Q60 target.
#
# davep 18-May-2007

import sys
import os
import re
import array
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
#import pylab
import Image

import flatfile
import miscmath

# XXX get these from PATH
gnuplot = "/usr/bin/gnuplot"
convert = "/home/davep/magick/bin/convert"
identify = "/home/davep/magick/bin/identify"

class ExternalScrewup( Exception ) :
    pass

class InternalScrewup( Exception ) :
    def __str__( self ) :
        return "OOPS! I screwed up! " + self.message

def execute_cmd( cmd, argv, verbose=0 ) :

    if verbose :
        print " ".join( argv )

    retcode = os.spawnv( os.P_WAIT, cmd, argv )
    if retcode != 0 :
#        raise "cmd \"%s\" failed with exit code %d" % (cmd, retcode )
        raise ExternalScrewup( "cmd \"%s\" failed with exit code %d" % (cmd, retcode ) )

    if verbose :
        print "%s finished with exit code %d" % (cmd,retcode)

    return retcode

def get_image_size( filename ) :
    # get the x,y size of an image by running ImageMagick's identify

    # I'm guessing this popen goes through the shell
    fh = os.popen( "identify %s" % filename, "r" )
    line = fh.read()
    fh.close()

    line = line.strip()

    print "line=",line
    if len(line) <= 0 :
        raise InternalScrewup( "Failed to execute identify; is ImageMagick in your PATH?")

    xysize_re = re.compile( '\s(\d+x\d+)\s' )
    re_obj = xysize_re.search( line )

    if len(re_obj.groups() ) != 1 :
        msg ="Looking for NxN in \"%s\" failed. Parse of ImageMagick identify output failed." % line
        raise InternalScrewup( msg )

    s, = re_obj.groups()
    # should have something like 1024x768, two numbers separated by an 'x'
    fields = s.split( "x" )

    if len(fields) != 2 :
        msg ="Searching for NxN in \"%s\" failed. Parse of ImageMagick identify output failed." % line
        raise InternalScrewup( msg )

    x = int(fields[0])
    y = int(fields[1])

    return (x,y)
    
def convert_to_format( filename, new_format ) :
    # filename - filename to convert to some new image format
    # new_format - extension of image format; hopefully it's something
    #   ImageMagick recognizes
     
    # find the current base filename so I can make a new filename
    basename,ext = os.path.splitext( filename )

    new_filename = basename + "." + new_format

    argv = ( "convert", filename, new_filename )

    retcode = execute_cmd( convert, argv )

    return new_filename

def separate_file_into_planes( filename, img_x, img_y, output_formats ) :
    # using ImageMagick's convert, split a 3-plane file into three separate
    # planes. Have to fool convert into thinking it's an RGB file.
    
    basename,ext = os.path.splitext( filename )

    # fake out convert into thinking we have an RGB file
    channels = ( "R", "G", "B" )

    channel_iter = channels.__iter__()

    output_filenames = []

    # convert -depth 8 -separate -channel R RGB:green.YCbCr foo.r
    for ext in output_formats : 
        channel = channel_iter.next()

        output_filename = "%s.%s" % (basename,ext) 

        argv = ( "convert", "-depth", "8", "-size", "%dx%d"%(img_x,img_y),\
            "-separate", "-channel", channel, "RGB:%s"%filename,\
            "%s:%s"%(channel,output_filename) )

        retcode = execute_cmd( convert, argv, 1 )

        output_filenames.append( output_filename )

    return output_filenames 

def bin2dat( input_filename, output_filename ) :
    print "bin %s -> numbers %s" % (input_filename,output_filename)
    inf = file( input_filename, "rb" )
    outf = file( output_filename, "wb" )

    # bytes
    numsize="B"
    # words
    #numsize="H"

    while 1 :
        buf = inf.read( 65536 * 1024 )
        if len(buf) <= 0 :
            break

        intbuf = array.array( numsize )
        intbuf.fromstring( buf )

        for num in intbuf :
            print >>outf, num

    inf.close()
    outf.close()

def make_plane_datafiles( infilename, output_formats ) :
    # assume output format is the concatentation of the file types of each of
    # the planes (e.g., Lab == "L", "a", "b")
    output_extension = reduce( lambda x,y : x+y, output_formats )

    # get the image size since we'll be pushing around raw data here in a few
    # moments
    img_x,img_y = get_image_size( infilename )
    print img_x,img_y

    # convert input filename to format we want to do the plot in
    outfilename = convert_to_format( infilename, output_extension ) 

    # split into three planes
    plane_filenames = separate_file_into_planes( outfilename, img_x, img_y, output_formats )

    # convert the plane files to single column integer textfiles
    for filename in plane_filenames :
        basename,ext = os.path.splitext( filename )

        # ext[1:] is to get the extension without the "."
        out_filename = basename + "-" + ext[1:] + ".dat"

        bin2dat( filename, out_filename )

def usage() : 
    print >>sys.stderr, "plotimg - convert image into flat text numbers suitable for import into plotting application."
    print >>sys.stderr, "usage: plotimg image-filename color-space"
    print >>sys.stderr, "color-space is one of ycc, rgb, lab."
    print >>sys.stderr, "example:"
    print >>sys.stderr, "  plotimg foo1.tif lab"
    print >>sys.stderr, "  will create foo1-L.dat foo1-a.dat foo1-b.dat"

def old_main() :
    if len(sys.argv) != 3 :
        usage()
        sys.exit(1)

    infilename = sys.argv[1]

    color_space = sys.argv[2].lower()

    all_output_formats = { "ycc" : ( "Y", "Cb", "Cr" ), 
                           "rgb" : ( "r", "g", "b" ),
                           "lab" : ( "L", "a", "b" ) }

    try :
        output_formats = all_output_formats[color_space]
    except KeyError :
        print >>sys.stderr, "unknown output format \"%s\"" % color_space
        sys.exit(1)

    make_plane_datafiles( infilename, output_formats )


def new_main() :
#    if len(sys.argv) != 3 :
#        usage()
#        sys.exit(1)

    infilename = sys.argv[1]

    im = Image.open( infilename )
    print im.format, im.mode, im.size
    print im.getbands()
    print im.info
    print im.getbbox()
#    print im.getcolors()

    print im.histogram()

#    print dir(im)

#    im2 = im.copy()
#    im2.thumbnail( (im.size[0]/2, im.size[1]/2) )
#    im2.save("out1.png")

##    plt.plot( im.histogram() )
#    n, bins, patches = plt.hist( im.getdata(), range=(0,255), bins=255, normed=0 )
#    plt.ylabel('some numbers')
#
#    plt.show()

#    pixel_nums = [ 0L ] * im.size[0] 
#    # for each row
#    for row in range(im.size[1]) :
#        # for each column
#        for column in range(im.size[0]) :
#            pixel_nums[ column ] += im.getpixel( (column, row) )

    image_width, image_height = im.size
    print "width=%d height=%d" % (image_width, image_height)

    pixel_means = miscmath.calc_means( im.getdata(), image_width, image_height )

    pixel_medians = miscmath.calc_medians( im.getdata(), image_width, image_height )

    flatfile.list_to_flatfile( "median.dat", pixel_medians )

#    plt.plot( avg )
#
#    plt.savefig( "gray.png" )
#
#    plt.show()
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.plot( pixel_means, label="Mean")
    ax.plot( pixel_medians, label="Median")
    ax.legend()

    fig.savefig('mean_vs_median.png')

if __name__ == '__main__' :
    new_main()

