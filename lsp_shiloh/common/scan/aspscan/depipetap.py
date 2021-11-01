#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 

# Automatically convert a pipetap file to image based on metadata in the
# pipetap file. 
#
# Originally the pipetap data was a raw blob of pixels. Had to examine the
# serial trace to find the image size. Slow and cumbersome. I'm finally adding
# header meta-data to the pipetap output.
#
# davep 06-Apr-2012
#
 
import sys
import os
import stat
import struct
import Image
import tempfile

import hexdump
import piltool

# davep 07-Sep-2012 ; try to load halfunpack.py to decode halfpack images.
# If not found, we won't decode 1,2,4-bpp scans.
try : 
    import halfunpack
    g_have_halfunpack = True
except ImportError : 
    g_have_halfunpack = False

class ConvertError( Exception ) : 
    pass

class SizeError( Exception ) :
    pass

def get_basename( filename ) : 
    return os.path.splitext( os.path.split( filename )[1] )[0]

def check_pipetap_file_size( header, infilename ) : 
    # do NOT trust the header! verify the file size matches the header size

    bytes_per_row = header["pixels_per_row_padded"] * header["bits_per_pixel"]/8

    header_file_size_bytes = header["total_rows"] * bytes_per_row

    # TODO handle total_rows==0 (ADF, I think)
    assert header["total_rows"]>0, header["total_rows"]

    real_file_size_bytes = os.stat(infilename)[stat.ST_SIZE] 
    # subtract the header size
    real_file_size_bytes -= header["header_size_bytes"]

    print "header reports size={0}; file payload is size={1}".format( 
                header_file_size_bytes, real_file_size_bytes )

    # TODO handle total_rows==0 (ADF)
    if real_file_size_bytes != header_file_size_bytes : 

        # might be an ADF scan; try to calculate the actual total rows based on
        # the file size and the pixels per row

        if real_file_size_bytes % bytes_per_row != 0 : 
            errmsg = "header claims image bytes={0} but file contains bytes={0}".format (
                        header_file_size_bytes, real_file_size_bytes )
            raise SizeError( errmsg )

        maybe_total_rows = real_file_size_bytes / bytes_per_row

        print "adjusting header from total_rows={0} to total_rows={1}".format( 
                    header["total_rows"], maybe_total_rows )

        header["total_rows"] = maybe_total_rows

def decode_pipetap_header( buf ): 
#    print "buf=",hexdump.dump(buf,16)

    # davep 05-Oct-2012 ;  can get empty files after network capture errors
    if len(buf) < 13 : 
        errmsg = "not enough data to decode"
        raise ConvertError( errmsg )

    cookie, = struct.unpack( "13s", buf[:13] )
    if not "pipetap" in cookie: 
        errmsg = "file is not a pipetap capture file"
        raise ConvertError( errmsg )

    # We are looking for strings followed by a big blob of pixels. So we need
    # to be careful pulling apart the blob.
    fields = buf.split("\r\n")

    # look for the blank line that should mark the end of header and start of
    # image data
    pos = buf.find("\r\n\r\n")
    if pos==-1 : 
        errmsg = "malformed pipecut file; missing header terminator"
        raise ConvertError( errmsg )

    # divide the buffer into header and trailing data
    # header split into fields around the line endings
    fields = buf[:pos].split("\r\n")
    trailing_data = buf[pos+4:]

    header = {}

    # the [1:] is so we skip the "pipetap" string at top of file
    for idx,f in enumerate(fields[1:]) : 
        # trap quickly on protocol changes (currently firmware uses key/value
        # strings smaller than 64)
        if len(f)>64 : 
            errmsg = "too long key/value pair len={0} expected max=64".format( len(f) )
            raise ConvertError(errmsg)
        
        if not ":" in f : 
            errmsg ="malformed key/value \"{0}\"; missing ':'".format( f ) 
            raise ConvertError( errmsg )

        key,value = [ s.strip() for s in f.split(":") ]
        header[key] = value

        # convert value to integer if looks like an integer
        try : 
            header[key] = int(value)
        except ValueError :
            pass

    # trailing_data is image data that follows the header
    if len(trailing_data)==0 : 
        errmsg = "pipetap file has bad format; missing data after header"
        raise ConvertError( errmsg )

    # save the header size so we can calculate the data size and verify against
    # the claims in the header
    header["header_size_bytes"] = len(buf) - len(trailing_data)
    
    return header, trailing_data

def halfpack_convert_file( header, buf, outfilename ) :
    # davep 07-Sep-2012 ; adding Halfpack conversion

    if not g_have_halfunpack : 
        raise Exception( "The halfunpack.py script not found so cannot convert copy-to-host images." )

    # write the buf to a temp file (halfunpack wants a file and I don't feel
    # like fixing it right now)
    tmpfile = tempfile.mkstemp()
    fd=tmpfile[0]
    infilename = tmpfile[1]
    retcode = os.write( fd, buf )
    if retcode != len(buf) : 
        raise Exception( "failed to write bytes={0} to tempfile; retcode={1}".format(
                            len(buf), retcode ) )
    # the temp file should persist after this close
    os.close(fd)

    hp_bpp = header["bits_per_pixel"]
    width = header["pixels_per_row_padded"]
    height = header["total_rows"]

    halfunpack.halfunpack( infilename, outfilename, hp_bpp, width)

    # clean up the temp file
    os.unlink( infilename )

def convert_pipetap_to_image( header, buf, outfilename ) : 
    size = ( header["pixels_per_row_padded"], header["total_rows"] )

    if "xrgb" in header["data_type"] : 
        piltool.convert_xrgb_buf( buf, outfilename, size )

    elif "rgbx" in header["data_type"] : 
        piltool.convert_rgbx_buf( buf, outfilename, size )

    elif "rgb" in header["data_type"] : 
        piltool.convert_rgb_buf( buf, outfilename, size )

    elif header["data_type"] in ("red","green","blue"): 
        if header["bits_per_pixel"] == 16 : 
            mode = "I;16"
        else : 
            assert header["bits_per_pixel"]==8, header["bits_per_pixel"]
            mode = "L"

        # add color to the output filename 
        outfilename = "{0}_{1}.tif".format( get_basename(outfilename), header["data_type"] )
        piltool.convert_gray_buf( buf, outfilename, size, mode=mode )

    elif "mono" in header["data_type"] : 
        bpp = header["bits_per_pixel"]
        if bpp == 16 : 
            mode = "I;16"
            piltool.convert_gray_buf( buf, outfilename, size, mode=mode )
        elif bpp==8 : 
            mode = "L"
            piltool.convert_gray_buf( buf, outfilename, size, mode=mode )
        elif bpp in (4,2,1) : 
            # davep 07-Sep-2012 ; add halfunpack
            halfpack_convert_file( header, buf, outfilename )
        else : 
            raise Exception( "unknown/unhandled bpp={0}".format(bpp) )
    else : 
        raise Exception( "cannot decode scan data type={0}".format( header["data_type"] ) )

    print "wrote", outfilename

def convert_pipetap_file( infilename ) : 
    # convert a pipetap/2.0 file with a header of metadata
    # davep 06-Apr-2012

    infile = open( infilename, "rb" ) 

    # read a blob from the top of file, look for header 
    buf = infile.read( 1024 )

    header, trailing_data = decode_pipetap_header( buf )

    print header

#    print " ".join( [ "{0:#x}".format(ord(c)) for c in trailing_data[:10] ] )

    check_pipetap_file_size( header, infilename )

    # read rest of file
    imgbuf = trailing_data + infile.read()
    infile.close()

    basename = get_basename(infilename)

#    outfile = open("{0}.dat".format(basename),"wb" )
#    outfile.write(imgbuf)
#    outfile.close()

    outfilename = "{0}.tif".format( basename )

    convert_pipetap_to_image( header, imgbuf, outfilename )

def main() :

    for filename in sys.argv[1:] :
        convert_pipetap_file( sys.argv[1] )
    
if __name__ == '__main__' :
    main()

