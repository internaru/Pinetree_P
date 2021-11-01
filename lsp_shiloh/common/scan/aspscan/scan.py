#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Wrapper around aspscan command line Linux/Unix (libusb) scanner.
#
# I wrote this way back when and am just how adding a file header. D'oh!
#
# davep 24-Apr-2008
#

import sys
import os
import re
import string
import copy
import subprocess

import calparse
import mkint
import imgmagick
import aspnums

# davep 08-Feb-2010 ; try to use the PIL (Python Imaging Library). If not
# installed, we'll fall back to ImageMagick
try :
    import Image
    import piltool
    g_have_pil = True
except ImportError :
    # we'll use ImageMagick instead
    g_have_pil = False

# davep 21-Apr-2011 ; try to load halfunpack.py to decode copy-to-host images.
# If not installed, we won't decode copy-to-host scans.
try : 
    import halfunpack
    g_have_halfunpack = True
except ImportError : 
    g_have_halfunpack = False

verbose = 1

# paths to commands used in fork/pipe calls
#xrgbtorgb = "./xrgbtorgb"

convert="convert"
# set convert to the true command to run this script without actually
# converting to a viewable image (good for testingg)
#convert="true"

#aspscan="./aspscantst"
g_aspscan=None
#aspscan="./aspscan.py"
#aspscan="/usr/bin/false"  # nice for testing scan failures

# davep 20-Apr-2008 ; add ability to save to different image type
#filename_ext = "png"
filename_ext = "tif"
#filename_ext = "jpg"

# davep 18-Jul-2009 ; enable/disable conversion (eventually add to command
# line?)
#g_convert_planar_files = True      # r,g,b files converted to .tif
g_convert_planar_files = False    # r,g,b files NOT converted to .tif

# davep 28-Jan-2010 ; FIXME currently harwiring the endianness of the 16bpp
# data; can change on another platform!
image_endian = "LSB"

class ScanException(Exception) :
    pass

class ImageConvertException( Exception ) : 
    pass

# Fields in the size data file
# ppr = pixels per row
# pprp = pixels per row padded
required_data_fields = ( 'dpi', 'rows', 'ppr', 'pprp', 'file', 'datatype' )

# convert an ASP datatype to a useful file extension
dtype_ext_dict = {
    aspnums.SCAN_DATA_RED   : "r", 
    aspnums.SCAN_DATA_GREEN : "g",
    aspnums.SCAN_DATA_BLUE  : "b",
    aspnums.SCAN_DATA_MONO  : "gray",
    aspnums.SCAN_DATA_MONO_MIRRORED : "gray",
    aspnums.SCAN_DATA_XRGB_PACKED   : "xrgb",
    aspnums.SCAN_DATA_RGBX_PACKED   : "rgbx",
    aspnums.SCAN_DATA_RGB_PACKED    : "rgb", 
}

def xrgb_to_rgb_name ( filename ) :
    if filename[-5:] != ".xrgb" :
        raise ScanException( "\"%s\" does not have an \".xrgb\" extension" % (filename) )

    outname = filename[:-5] + ".rgb"
    return outname

def convert_xrgb_to_rgb( xrgbfile ) :
    # convert the XRGB files to RGB; return a list of the files we converted
    outfile = xrgb_to_rgb_name( xrgbfile )
    if verbose>0 :
        print "convert xrgb", xrgbfile, "to", outfile 
    cmdline = ("xrgbtorgb", "-i", xrgbfile, "-o", outfile )

    print cmdline
    subprocess.check_call( cmdline )

def file_extension_to_asp_datatype( file_ext ) :
    # dict is keyed by the data type so revert to a linear search
    for ext in dtype_ext_dict.keys() : 
        if file_ext==ext : 
            return dtype_ext_dict[ext]

    # should never get here
    raise ScanException( "Unknown scan image file extension=%s" % file_ext )

def asp_datatype_to_file_extension( asp_datatype ) :
    # dict is keyed by the data type so lookup is easy
    try : 
        file_ext = dtype_ext_dict[asp_datatype] 
        return file_ext
    except KeyError :
        raise ScanException( "Unknown scan image datatype=%d" % asp_datatype )

def pil_convert_file( image_info, dstfileext="tif", more_args=None ) :

    print "pil_convert_file(): image_info=", image_info

    infilename = image_info['file']

    srcfileext = asp_datatype_to_file_extension( image_info["datatype"] )

    size = (image_info['pprp'], image_info['rows'] )

    # davep 24-Mar-2014 ; adding DPI 
    # (assume x,y DPI is symmetrical; PIL wants Xres,YRes)
    dpi = (image_info["dpi"],image_info["dpi"])

    outfilename = infilename + "_" + srcfileext + "." + dstfileext
    infilename = infilename + "." + srcfileext

    if srcfileext == "rgb" : 
#        piltool.convert_bgr( infilename, outfilename, size )
        piltool.convert_rgb( infilename, outfilename, size, dpi=dpi )
    elif srcfileext == "xrgb" : 
        piltool.convert_xrgb( infilename, outfilename, size, dpi=dpi )
    elif srcfileext == "rgbx" : 
        piltool.convert_rgbx( infilename, outfilename, size, dpi=dpi )
    else : 
        if image_info["bpp"] == 16 : 
            mode = "I;16"
        else : 
            mode = "L"
        piltool.convert_gray( infilename, outfilename, size, mode=mode, dpi=dpi )

    fileinfo = { 'filename': outfilename,
        'dpi': image_info['dpi'],
        'datatype': image_info['datatype'] }

    return fileinfo

def copy_to_host_convert_file( image_info, dstfileext="tif", more_args=None ) :
    # davep 21-Apr-2011 ; adding copy-to-host conversion

#    raise ImageConvertException( "copy-to-host not yet supported" )

    if not g_have_halfunpack : 
        raise ImageConvertException( "The halfunpack.py script not found so cannot convert copy-to-host images." )

    assert image_info["datatype"]==aspnums.SCAN_DATA_MONO, image_info["datatype"]

    srcfileext = asp_datatype_to_file_extension( image_info["datatype"] )

    infilename = image_info["file"] + "." + srcfileext
    outfilename_base = image_info["file"] + "_out" 
    outfilename = outfilename_base + "_" + srcfileext + ".tif"
    hp_bpp = image_info["bpp"]
    width = image_info["pprp"]
    height = image_info["rows"] 

    halfunpack.halfunpack( infilename, outfilename, hp_bpp, width)

    fileinfo = { 'filename': outfilename,
        'dpi': image_info['dpi'],
        'datatype': image_info['datatype'] }

    return fileinfo

def convert_file( image_info, dstfileext="tif", more_args=None ) :

    # davep 21-Apr-2011 ; adding conversion of copy-to-host
    if image_info["bpp"] in (1,2,4) : 
        return copy_to_host_convert_file( image_info, dstfileext, more_args )

    # If we have the Python Imaging Library (PIL), use PIL. 
    use_pil = g_have_pil

    # Except PIL has 16-bpp issues I'm still working through.
#    if image_info["bpp"] == 16 and image_info["datatype"] != aspnums.SCAN_DATA_MONO :
#        use_pil = False

    if not use_pil:
        return im_convert_file( image_info, dstfileext, more_args )
    else :
        # TODO add 16-bpp support to PIL
        return pil_convert_file( image_info, dstfileext, more_args )

def im_convert_file( image_info, dstfileext="tif", more_args=None ) :

    print "im_convert_file(): image_info=", image_info

    infilename = image_info['file']

    srcfileext = asp_datatype_to_file_extension( image_info["datatype"] )
    if srcfileext == "xrgb" :
        typecast = "rgb"
        # convert the xrgb file to rgb
        convert_xrgb_to_rgb( infilename + ".xrgb" )
        srcfileext = "rgb"
    else :
        # r,g,b,gray files converted to grayscale
        typecast = "gray"

    size = "%dx%d" % (image_info['pprp'], image_info['rows'] )

    outfilename = infilename + "_" + srcfileext + "." + dstfileext
#    outfilename = infilename + "." + dstfileext

#    cmdline = [ "convert", '-depth', '8', '-size', size ] 
    cmdline = [ "convert", '-depth', "%d"%image_info["bpp"], '-size', size ] 
    if more_args is not None :
        cmdline.extend( more_args )
    cmdline.append( "%s:%s.%s" % (typecast,infilename,srcfileext) )
    cmdline.append( outfilename )

    print "cmdline=",cmdline
    print "cmdline=", " ".join( cmdline )

    subprocess.check_call( cmdline )

    fileinfo = { 'filename': outfilename,
        'dpi': image_info['dpi'],
        'datatype': image_info['datatype'] }

    return fileinfo

def pil_combine_rgb_planes( image_info_list, dstfileext="tif", more_args=None ) :
    # must have three planes for this to make sense
    assert len(image_info_list)==3, len(image_info_list) 

    # take our starting info from the first element in the list (all three
    # files must be the same size, etc)
    image_info = image_info_list[0]

    infilename_base = image_info['file']

    outfilename = infilename_base + "_rgb." + dstfileext

    size = (image_info['pprp'], image_info['rows'] )
    
    # davep 24-Mar-2014 ; adding DPI 
    # assume x,y DPI is symmetrical; PIL wants Xres,YRes
    dpi = (image_info["dpi"],image_info["dpi"])

    # add our three image files to the command line
    for image_info in image_info_list :
        infilename_base = image_info['file']

        # make sure this image's size matches what I'm assuming to be all the
        # images' sizes
        tmp_size = (image_info['pprp'], image_info['rows'] )
        # davep 01-Feb-2011 ; no assert necessary; PIL seems to handle the
        # discrepency with aplomb
#        assert size==tmp_size, (size,tmp_size)
        if tmp_size != size : 
            print "warning! planar file {0} size {1} different than {2}".format( 
                    infilename_base, size, tmp_size )

        srcfileext = asp_datatype_to_file_extension( image_info["datatype"] )
        assert srcfileext is not None 

    piltool.combine_rgb( infilename_base, outfilename, size, dpi=dpi )

    fileinfo = { 'filename': outfilename,
        'dpi': image_info['dpi'],
        'datatype': aspnums.SCAN_DATA_RGB_PACKED }

    return fileinfo

def im_combine_rgb_planes( image_info_list, dstfileext="tif", more_args=None ) :

    # must have three planes for this to make sense
    assert len(image_info_list)==3, len(image_info_list) 

    # take our starting info from the first element in the list (all three
    # files must be the same size, etc)
    image_info = image_info_list[0]

    infilename_base = image_info['file']

    outfilename = infilename_base + "_rgb." + dstfileext

    size = "%dx%d" % (image_info['pprp'], image_info['rows'] )

    # start building the command line
    cmdline = [ "convert", '-size', size ] 
    if more_args is not None :
        cmdline.extend( more_args )
    
    # assume all images are the same bpp
    cmdline.append( '-depth' )
    cmdline.append( "%d"%image_info_list[0]["bpp"] )

    cmdline.append( "-combine" )

    # add our three image files to the command line
    for image_info in image_info_list :
        infilename_base = image_info['file']

        # make sure this image's size matches what I'm assuming to be all the
        # images' sizes
        tmp_size = "%dx%d" % (image_info['pprp'], image_info['rows'] )
        assert size==tmp_size, (size,tmp_size)

        srcfileext = asp_datatype_to_file_extension( image_info["datatype"] )
        assert srcfileext is not None 

        # davep 09-Dec-2008 ; add the typecast to help older (broken) versions
        # of ImageMagick
        cmdline.append( "%s:%s.%s" % (srcfileext,infilename_base,srcfileext) )
#        cmdline.append( "%s.%s" % (infilename,srcfileext) )

    cmdline.append( outfilename )

    print "cmdline=", cmdline
    print "cmdline=", " ".join( cmdline )

    subprocess.check_call( cmdline )

    fileinfo = { 'filename': outfilename,
        'dpi': image_info['dpi'],
        'datatype': aspnums.SCAN_DATA_RGB_PACKED }

    return fileinfo

def combine_rgb_planes( image_info_list, dstfileext="tif", more_args=None ) :

    # take our starting info from the first element in the list (all three
    # files must be the same size, etc)
    image_info = image_info_list[0]

    # if we don't have PIL or we're doing 16-bpp, fall back to ImageMagick.
    # (PIL TIF doesn't support 16-bpp... yet)
    if not g_have_pil or image_info["bpp"] == 16 : 
        return im_combine_rgb_planes( image_info_list, dstfileext, more_args )
    else : 
        # TODO add 16-bpp support to PIL
        return pil_combine_rgb_planes( image_info_list, dstfileext, more_args )

def parse_image_info_line( line ) :
    fieldre = re.compile( "^([a-zA-Z_]+)=(.+)$" )

    # kill leading/trailing spaces and \n
    line = line.strip(" \n")
    
    # Note this will break if someone ever uses filenames with embedded
    # spaces; but then if you use a filename with an embedded space, you deserve
    # all the pain you'll get
    fields = line.split( " " )

    image_info = {}
    for fld in fields :
        matchobj = fieldre.match( fld )
        if matchobj is None :
            raise ScanException( "Bad field \"%s\" in size file " % (fld) )
#            print matchobj.group(1),matchobj.group(2)

        key = "%s" % matchobj.group(1)
        value = matchobj.group(2)

        image_info[ key ] = value

        # fiddle with some of the fields in the file

        # clean the quotes from the filename ; again, note this will break any
        # filename that has embedded quotes
        if key == 'file' :
            image_info['file'] = image_info['file'].strip( '"' )
        else :
            # convert numerical fields to integers
            image_info[key] = string.atoi( value )
            

    # make sure we have all the required fields
    all_keys = image_info.keys()
    for k in required_data_fields :
        if k not in all_keys :
            raise ScanException( "Required key \"%s\" is missing from datafile" % k )
        
    assert image_info['datatype'] != 0

    if image_info['datatype'] != aspnums.SCAN_DATA_PLANAR :
        # always return a list of files so we can seamlessly support planar
        # files
        return [ image_info, ]

    # convert planar file info into three file info fields, one for each
    # color
    image_info_list = []
    for newdatatype in ( aspnums.SCAN_DATA_RED, aspnums.SCAN_DATA_GREEN, aspnums.SCAN_DATA_BLUE ) :
        new_image_info = {}
        for k in image_info.keys() :
            new_image_info[k] = image_info[k]
        new_image_info['datatype'] = newdatatype
    
        # aspscan returns the total number of rows scanned across all
        # three colors so the rowcount will be 3x the actual number of
        # visible rows
        if image_info['rows'] % 3 != 0 :
            errmsg = "Invalid row count=%d in datafile=\"%s\"; should be evenly divisible by 3" %\
                            ( image_info['rows'], image_info['file'] )
            # davep 02-Apr-2009 ; ICE Lite scaled screws up this test so let's
            # keep going and hope for the best
            print "Error!", errmsg
#            raise ScanException( errmsg )

        new_image_info['rows'] = image_info['rows'] / 3
        
        image_info_list.append( new_image_info )

    return image_info_list

def convert_all_files( image_info_list ) :
    #
    # Fields in image_info: pprp, rows, datatype, ppr, file, dpi
    #
    # datatype is one of aspnums.SCAN_DATA_xxx
    #
    # For example:
    #
    # image_info_list = [
    #       {'pprp': 2544, 'rows': 1414, 'datatype': 3, 'ppr': 2544, 'file': 'foo', 'dpi': 300}, 
    #       {'pprp': 2544, 'rows': 1414, 'datatype': 4, 'ppr': 2544, 'file': 'foo', 'dpi': 300}, 
    #       {'pprp': 2544, 'rows': 1414, 'datatype': 5, 'ppr': 2544, 'file': 'foo', 'dpi': 300} 
    # ]

    print "convert_all_files(): image_info_list=", image_info_list

    # since we're converting to tif, make sure we make the Units field
    # meaningful (or Gimp complains)
    more_convert_args = [ "-units", "PixelsPerInch" ]

    # davep 08-Jul-2009 ; force MSB (new version of ImageMagick requires?).
    # I'm sending 16-bit big-endian pixels from Xtensa based platforms.
    more_convert_args.append( "-endian" )
    more_convert_args.append( image_endian )
    
    # set the density too (NOTE! assuming all the images are of the same
    # density (pretty safe assumption)
    dpi = image_info_list[0]["dpi"]
    more_convert_args.append( "-density" )
    more_convert_args.append( "%dx%d" % (dpi,dpi) )
        
    planar_files = {}
    output_file_info_list = []
    for image_info in image_info_list :

        assert image_info['datatype'] != 0 

        # davep 24-Jul-2009 ; disable converting the individual r,g,b planes to
        # separate images for now (doing rgb scans and converting the .r, .g,
        # .b to images is taking too long)
        srcfileext = asp_datatype_to_file_extension( image_info["datatype"] )

        if srcfileext in ( "r", "g", "b" ) and not g_convert_planar_files :
            print "skipping conversion of single planar file"
        else : 
            try : 
                output_file_info = convert_file( image_info, filename_ext, more_convert_args )
                output_file_info_list.append( output_file_info )
            except ImageConvertException, e : 
                # Too bad, so sad, the conversion of this file failed. Keep
                # going. 
                print "Image conversion of file \"{0}\" failed : {1}".format(
                    image_info['file'], e )

        # if we've gotten back planar data (three files), build up a new
        # list so we can convert the r,g,b files into a single _rgb file.
        if srcfileext in ( "r", "g", "b" ) :
            if not image_info["file"] in planar_files :
                # first time we've seen this file
                planar_files[ image_info["file"] ] = []
            # add image info to this list
            planar_files[ image_info["file"] ].append( image_info ) 


#    print "planar_files=",planar_files

    # did we get any planar files? If so, combine them into a single image file
    for file in planar_files.keys() :
        file_list = planar_files[file]
        output_file_info = combine_rgb_planes( file_list, filename_ext, more_convert_args )
        print "output_file_info=",output_file_info
        output_file_info_list.append( output_file_info )

    return output_file_info_list

def find_user_area( scan_args ):
    # poke through the command line arguments looking for the scan area and
    # DPI. From those numbers, calculate how much data we expect to get back.
    # We'll compare to what we actually get back and report the difference.
    #
    # Originally written while testing truncating the padding at the bottom of
    # images. (User scans 11.69", we wind up sending back 11.8" because we have
    # to pad PIE so much.)
    
    # look for area; if specified, we'll verify what the firmware sends us vs
    # what we ask for
    scan_area = None
    scan_dpi = None

    # throw the -1 on there to avoid an out of range problem if some
    # knucklehead put "-a" as the last command line argument
    for i in range(len(scan_args)-1) :
        if scan_args[i]=='-a' :
            scan_area = scan_args[i+1]
        elif scan_args[i]=='-d' :
            # DPI
            scan_dpi = scan_args[i+1]

    # if we weren't given an area on the command line, user our default from
    # the firmware
    if scan_area is None :
        user_width_x = 850
        user_width_y = 1169
    else :
        try : 
           fields = [ int(n) for n in scan_area.split( "," ) ]
        except ValueError :
            # ignore bad command line; let aspscan complain so we scan
            # consistant and don't expect scan.py to test all the command line
            # arguments
            return None

        scan_area = fields

        # coordinates in the array are are x0,y0,x1,y1
        user_width_x = scan_area[2] - scan_area[0]
        user_width_y = scan_area[3] - scan_area[1]

    if scan_dpi is None :
        # firmware defaults to 300 dpi if not specified
        scan_dpi = 300
    else :
        # convert text to integer; if bad integer, quite this whole test
        # and let aspscan fail with bad arguments
        try :
            scan_dpi = int( scan_dpi )
        except ValueError :
            return None
        
    # areas are specified in 1/100" so convert to inches ; note I'm rounding up
    # because (I hope) the firmware will do the same
    user_pixels_x = int(round((user_width_x/100.0)*float(scan_dpi)))
    user_pixels_y = int(round((user_width_y/100.0)*float(scan_dpi)))

    return user_pixels_x, user_pixels_y

def fork_aspscan( scan_args ) :
    fds = os.pipe()

    if g_aspscan is None :
        find_aspscan_exe()

    aspscan_args = [ g_aspscan, ]
    aspscan_args.extend( scan_args )
    aspscan_args.append( "-z" )
    aspscan_args.append( "%d" % fds[1] )

    print aspscan_args

    pid = os.fork()
    if pid==0 :
        # child
        print "hello from child"
        os.execv( g_aspscan, aspscan_args )
        print "should not get here!"
        assert 0
    else :
        # parent
        os.close( fds[1] )

        try : 
            str = ""
            while 1 :
                new_str = os.read( fds[0], 1024 )

                if len(new_str) <= 0 :
                    # end of file or error
                    break

                str += new_str

#                print "str=\"%s\"" % str
        except Exception,e :
            # Get here if anything goes wrong with the read (ctrl-c, etc). Want
            # to be able to cleanly close the pipe and wait for the child to
            # exit. 
            print "%s received exception" % sys.argv[0], e
        
        os.close( fds[0] )

        # wait()'s return is bizarre:
        # "Wait for completion of a child process, and return a tuple containing
        # its pid and exit status indication: a 16-bit number, whose low byte
        # is the signal number that killed the process, and whose high byte is
        # the exit status (if the signal number is zero); the high bit of the
        # low byte is set if a core file was produced."
        print "waiting for child..."
        exit_status = os.wait()
        if os.WIFEXITED(exit_status[1]) :
            exit_code = os.WEXITSTATUS(exit_status[1])
            print "aspscan exited with code %d" % exit_code
        else :
            errmsg = "scan was abnormally halted; wait() returned %s" %\
                        repr(exit_status)
            raise ScanException( errmsg )

    if exit_code != 0 :
        errmsg = "Error! The scan failed. Giving up."
#        print errmsg
        raise ScanException( errmsg )

    return str

def parse_aspscan_output_str( image_info_str ) :
    all_image_info = []

    # could have several image outputs in one run of aspscan; they'll be
    # separated by \n
    str_list = image_info_str.split( "\n" )
    for str in str_list :

        # skip blank lines
        if len(str)==0 :
            continue

        image_info_list = parse_image_info_line( str )
        if verbose : 
            print "image_info=",image_info_list

        all_image_info.append( image_info_list )

    return all_image_info

def convert_cal_to_host( image_info_list ) :

    if verbose : 
        print "image_info_list=",image_info_list

    # assume this information is constant across all the image files
    pixels_per_row = image_info_list[0]["pprp"]
    dpi = image_info_list[0]["dpi"]
    
    # cal-to-host only supports mono and planar; the image_info_list will only
    # have the base filename so we need to take the datatype and make a full
    # filename
    file_info_list = []
    for image_info in image_info_list :
        datatype = image_info["datatype"]
        
        file_ext = asp_datatype_to_file_extension( datatype )

        # cal-to-host only supports mono and planar
        if file_ext not in ( "r", "g", "b", "gray" ) :
            raise ScanException( "Cal-to-host doesn't support datatype %d." % datatype +\
                                 "Cal-to-host only supports r, g, b, and gray." )
            
        filename = image_info["file"] + "." + file_ext

        fileinfo = { 'filename': "%s.%s" % (image_info["file"],file_ext),
                     'dpi': image_info['dpi'],
                     'datatype': datatype }

        file_info_list.append( fileinfo )

    # davep 03-Dec-2008 ; use new calparse 
    # davep 12-May-2009 ; add support for final r/g/b cal-rgb image (need to
    # sort the files into r/g/b order)
    filename_list = [ file_info["filename"] for file_info in file_info_list ]
    if len(filename_list)==3 :
        calparse.calparse_all_files( calparse.rgb_extension_sort(filename_list) )
    else :
        calparse.calparse_all_files( filename_list )
#    for file_info in file_info_list :
#        calparse.run_calparse( file_info["filename"] )

    return file_info_list

def run_scan( scan_args ) :

    # get the user size so we can complain if the images don't match
    user_pixels = find_user_area( scan_args )
    if verbose : 
        print "user_pixels=",user_pixels

    # run aspscan to get our raw images
    aspscan_output = fork_aspscan( scan_args ) 

    if len(aspscan_output)==0 : 
        raise ScanException( "aspscan wrote no file info so cannot convert files" )

    # parse the output from aspscan and make viewable images from the raw
    # images
    all_image_info = parse_aspscan_output_str( aspscan_output )

    if verbose :
        print "all_image_info=", all_image_info

    # davep 13-May-2008 ; when the firmware fails to scan (say, the scanner is
    # busy), or if we're doing a cal-to-host and "scan config nocal" is set,
    # we'll get back zero rows of data and a zero byte file. Catch the zero
    # sized data here so we don't try to convert into an image.   I'm only
    # checking [0] because if the first scan fails on an ADF, usually there
    # won't be any more. (all_image_info[] is an array of arrays; internal
    # array is either [mono,] or [xrgb,] or [r,g,b].
    #
    # I return an array of arrays because I need to group planar images as
    # three separate image_info hashes.
    first_image = all_image_info[0]
    if first_image[0]["rows"]==0 or first_image[0]["pprp"]==0 : 
        # raise a descriptive error because this could be kinda confusing.
        if "-c" in scan_args : 
            raise ScanException( "Scan returned no data. Is the firmware's 'nocal' flag set?" )
        else :
            raise ScanException( "Scan returned no data. Scanner could be busy?" )

    output_file_info_list = []

    if "-c" in scan_args : 
        # The -c option tells aspscan to do a cal-to-host which is very
        # different from a scan-to-host so we'll have to handle it specially.
        # 
        # There should only be one image in the scan thus the "[0]"
        #
        output_file_info = convert_cal_to_host( all_image_info[0] ) 
        output_file_info_list.extend( output_file_info )
    else : 
        for image_info_list in all_image_info :
            output_file_info = convert_all_files( image_info_list ) 
            output_file_info_list.extend( output_file_info )

    if verbose : 
        print "output_file_info_list=",output_file_info_list

#    for f in output_file_info_list :    
#        print f["filename"]
#        s = "scp {0} latches.local:tmp/.".format( f["filename"] )
#        print s
#        subprocess.check_call( s.split() )

    return output_file_info_list

def test_cal_to_host( ) :
    all_image_info= [[{'pprp': 2544, 'rows': 480, 'datatype': 6, 'ppr': 2544,
        'file': 'foo', 'dpi': 300}]]

    all_image_info= [[{'pprp': 2544, 'rows': 480, 'datatype': 6, 'ppr': 2544,
        'file': 'foo', 'dpi': 300}]]

    all_image_info= [[{'pprp': 2544, 'rows': 240, 'datatype': 6, 'ppr': 2544,
        'file': 'foo2', 'dpi': 300}]]

    all_image_info= [[{'pprp': 2576, 'rows': 600, 'ppr': 2576, 'datatype': 6,
        'bpp': 8, 'file': 'foo3', 'dpi': 300}]]

    convert_cal_to_host( all_image_info[0] )

def test_convert_all_files() :


    all_image_info= [[{'pprp': 2560, 'rows': 3300, 'bpp': 16, 'datatype': 3, 'ppr':
2560, 'page_side': 0, 'file': 'foo24', 'dpi': 300}, {'pprp': 2560, 'rows':
3300, 'bpp': 16, 'datatype': 4, 'ppr': 2560, 'page_side': 0, 'file': 'foo24',
'dpi': 300}, {'pprp': 2560, 'rows': 3300, 'bpp': 16, 'datatype': 5, 'ppr':
2560, 'page_side': 0, 'file': 'foo24', 'dpi': 300}]]

    output_file_info_list = []

    for image_info_list in all_image_info :
        print "image_info_list=",image_info_list
        output_file_info = convert_all_files( image_info_list )
        output_file_info_list.extend( output_file_info )

    print "output_file_info_list=",output_file_info_list
    for f in output_file_info_list :    
        print f["filename"]

def find_path_to_program( prog_name ) :
    path_str = os.environ["PATH"]

    path = path_str.split( os.pathsep )

    if sys.platform == 'win32' and not prog_name.endswith(".exe") :
        prog_name += ".exe"

    for p in path :
        full_path  = p + os.sep + prog_name
        if os.path.exists( full_path ) :
            # davep 16-Nov-2009 ; Kludge! if we're on Windows, make sure we
            # didn't hit the /windows/system32/convert.exe
            if prog_name.startswith( "convert" ) :
                if full_path.lower().find("system32")==-1 :
                    return full_path
            else :
                return full_path

    return None

def find_aspscan_exe() :
    global g_aspscan

    g_aspscan = os.getenv( "ASPSCAN" ) 
    if g_aspscan is None :
        g_aspscan=find_path_to_program( "aspscan" )
        if g_aspscan is None :
            raise ScanException( "Unable to find \"aspscan\" in your path." )

def main( scan_args ) :

#    test_cal_to_host()
#    test_convert_all_files()
#    sys.exit(0)

    # the exit status of this script
    exit_code = 0

    try : 
        run_scan( scan_args )
    except ScanException, e :
        print e
        exit_code = 1

    return exit_code


if __name__ == '__main__' : 
    # we'll pass everything as-is to aspscan
    scan_args = sys.argv[1:]

    retcode = main( scan_args )
    
    if retcode != 0 and verbose > 0 :
        print "Scan failed!"

    # pass the success/failure exit code of the scan back to the system
    sys.exit(retcode)

