#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


import sys
import getopt
import socket 

from log import errorlog, dbg1, dbg2
import aspnums

PROGRAM_VERSION = "0.0.1"

SCANNER_VENDOR_ID  = 0x03f0
CASCADE_PRODUCT_ID  = 0x3b17
WHITEHAWK_PRODUCT_ID  = 0x5817
SCANNER_PRODUCT_ID  = CASCADE_PRODUCT_ID

#default_scan_to_host_port = 22011
default_scan_to_host_port = 9101

# davep 11-Feb-2011 ; adding copy-to-host; doesn't have a valid ASP data type
# so throw in a goofy one so can correctly parse the command line
XXX_COPY_DATA_TYPE=-1

valid_data_types = {
    # (color and planar are the same thing) 
    "mono" :  aspnums.SCAN_DATA_MONO , 
    "color" :  aspnums.SCAN_DATA_PLANAR,
    "planar" : aspnums.SCAN_DATA_PLANAR ,
    "xrgb" : aspnums.SCAN_DATA_XRGB_PACKED, 
    "rgbx" : aspnums.SCAN_DATA_RGBX_PACKED, 
    "rgb" : aspnums.SCAN_DATA_RGB_PACKED, 

    #  copy's data type is handled as a special case */
    "copy" : XXX_COPY_DATA_TYPE
}

valid_scan_types = {
    "mono" : aspnums.SCAN_TYPE_MONO,
    "color" : aspnums.SCAN_TYPE_COLOR,
    "copy" : aspnums.SCAN_TYPE_SCAN_THRU_COPY,
}

valid_doc_src = { 
    "auto" : aspnums.SCAN_DOCUMENT_SOURCE_AUTO,
    "adf"  : aspnums.SCAN_DOCUMENT_SOURCE_ADF,
    "flatbed" : aspnums.SCAN_DOCUMENT_SOURCE_FLATBED
}

valid_copy_quality_types = {
    "text":  aspnums.COPY_QUALITY_TEXT ,
    "draft" :aspnums.COPY_QUALITY_DRAFT ,
    "mixed": aspnums.COPY_QUALITY_MIXED ,
    "film":  aspnums.COPY_QUALITY_FILM_PHOTO ,
    "picture": aspnums.COPY_QUALITY_PICTURE ,
    "color": aspnums.COPY_QUALITY_COLORCOPY ,
#    "faxtext": aspnums.COPY_QUALITY_FAX_TEXT ,
#    "faxmixed": aspnums.COPY_QUALITY_FAX_MIXED ,
}

class BadCommandLine( Exception ) :
    def __init__( self, bad_option, bad_value, msg ) :
        self.failed_option = bad_option
        self.failed_value = bad_value
        self.msg = msg

    def __str__( self ) :
        return "Bad option opt=\"%s\" value=\"%s\" : %s" % (self.failed_option,
            self.failed_value, self.msg)

def usage( progname ) : 

    errorlog( "%s %s", progname, PROGRAM_VERSION );
    errorlog( "Capture scan data from ASP + TCP/IP scanners." );
    errorlog( "usage: " );

    errorlog( "  -h ; this help text" );
    errorlog( "  -r resolution (e.g., 300, 600, 1200)" );
    errorlog( "  -t data_type  ; valid values are mono, color, planar, xrgb, copy" );
    errorlog( "                (planar is another name for color)" );
    errorlog( "                (the copy option captures copy path data)" );
    errorlog( "  -b bpp ; bits per pixel (8 or 16; default is 8)" );

    errorlog( "  -x xscale-numerator/xscale-denominator ; e.g. 1/2" );
    errorlog( "  -y yscale-numerator/yscale-denominator" );

    errorlog( "  -o outputfile" );
    errorlog( "     for xrgb data_type, will create outputname.xrgb" );
    errorlog( "     for color data_type, will create outputname.r,outputname.g,outputname.b" );
    errorlog( "     for mono data_type, will create outputname.gray" );

    errorlog( "  -a x0,y0,x1,y1" );
    errorlog( "     specify area to scan; values are in 1/100ths of an inch.");
    errorlog( "     for example, -a 0,0,850,1100 will scan a full 8.5\"x11\" sheet." );
    errorlog( "     note either there can be no spaces between the numbers or" );
    errorlog( "        surround the numbers with quotes; e.g., -a \"0, 0, 850, 1100\"" );

    errorlog( "  -z fileid" );
    errorlog( "     write final image x,y size to file id (great for scripts)" );
    errorlog( "     for example, -z 6 will write stats to file handle 6" );

    errorlog( "  -C contrast" );
    errorlog( "     valid values are 1..11" );

    errorlog( "  -B brightness" );
    errorlog( "     valid values are 1..11" );

    errorlog( "  -G gamma" );
    errorlog( "     valid values are 10..100; value is divided by 10 in the firmware" );
    errorlog( "     for example, -G 20 is gamma of 2.0" );

    errorlog( "  -q quality" );
    errorlog( "     set copy quality (only valid for copy-to-host)" );
    errorlog( "     valid values are: text, draft, mixed, film, color" );
    errorlog( "     if this option is not present, firmware will use the default copy quality" );

    errorlog( "  -T seconds" );
    errorlog( "     set device timeout to \"seconds\"" );

    errorlog( "  -v num" );
    errorlog( "     USB Vendor ID (default is %#06x)", SCANNER_VENDOR_ID );
    errorlog( "     (IGNORED--for compatibility with USB aspscan)" )

    errorlog( "  -p port" );
    errorlog( "     TCP port (default is {0})".format( default_scan_to_host_port ) );

    errorlog( "  -c ; do cal-to-host instead of scan" );
    errorlog( "     Perform a \"cal-to-host\" instead of a real scan. The data returned" );
    errorlog( "     will be the data gathered during the calibration instead of scan data." ); 

    errorlog( "  -i ipaddress" );
    errorlog( "     IP address of scanner" );
     
#    errorlog( "  -s serialnum" );
#    errorlog( "     decide between devices with the same vid and pid using this USB serial number" );

    errorlog( "  -n ; add page numbers to scan files (useful with ADFs)");
    errorlog( "     default is no page number" );

    errorlog( "  --docsrc [%s] ; set document source" % "|".join( valid_doc_src) )

    errorlog( "  --socket-path [path]" )
    errorlog( "    connect to unix domain socket at [path] instead of TCP/IP socket" )
    

def parse_ulong( opt, arg ) :
    try :
        value = int(arg, 10 )
    except ValueError:
        try :
            # ok, maybe it's hex
            value = int(arg, 16 )
        except ValueError :
            errmsg = "Invalid integer \"%s\". Integer value must be dec or hex." % arg
            raise BadCommandLine( opt, arg, errmsg )

    return value

def parse_fraction( opt, arg ) :
    fields = arg.split( "/" )
    if len(fields) != 2 :
        errmsg = "Invalid fraction string \"%s\". Expect string like \"1/3\"\n." % arg
        raise BadCommandLine( opt, arg, errmsg )

    numer = parse_ulong( opt, fields[0] )
    denom = parse_ulong( opt, fields[1] )

    return (numer,denom)

def parse_copy_quality( opt, arg ) : 
    if arg not in valid_copy_quality_types.keys() : 
        errmsg = "Unknown or unsupported copy quality \"%s\"." % arg
        raise BadCommandLine( opt, arg, errmsg )

    return valid_copy_quality_types[ arg ]

def parse_resolution( opt, arg ) :
    num32 = parse_ulong( opt, arg ) 

    if num32<75 or num32>2400 :
        errmsg = "Unknown or unsupported scan resolution \"%d\"." % num32 
        raise BadCommandLine( opt, arg, errmsg )

    return num32

def parse_data_type( opt, arg ) :
    if arg not in valid_data_types.keys() : 
        errmsg = "Unknown or unsupported scan type \"%s\"." % arg
        raise BadCommandLine( opt, arg, errmsg )

    return valid_data_types[ arg ]

def parse_area( opt, arg ) :
    fields = arg.split( "," )
    if len(fields) != 4 :
        errmsg = "Invalid area \"%s\". Expect area with four integers like \"0,0,850,1100\"\n." % arg 
        raise BadCommandLine( opt, arg, errmsg )

    return [ parse_ulong(opt,n) for n in fields ]

def parse_document_source( opt, arg ) :
    try : 
        docsrc_num = valid_doc_src[ arg.lower() ] 
    except KeyError :
        errmsg = "Invalid document source \"%s\". Valid document sources are %s." %\
            ( arg, str(valid_doc_src) )
        raise BadCommandLine( opt, arg, errmsg )

    return docsrc_num

def parse_args( argv ) :
    try : 
        opt_list, arg_list = getopt.getopt( argv[1:],
                                            "hr:t:x:y:o:a:z:C:G:B:S:q:T:v:p:cs:ni:b:", 
                                            ( "docsrc=", "socket-path=", ) );
    except getopt.GetoptError, e:
        errorlog( str(e) )
        errorlog( "Use \"-h\" to get help." )
        sys.exit(1)

    scan_options = {}

    # davep 16-Nov-2009 ; default to 300 dpi mono
    scan_options["resolution"] = 300
    scan_options["data_type"] = aspnums.SCAN_DATA_MONO
    scan_options["scan_type"] = aspnums.SCAN_TYPE_MONO
    scan_options["tcp_port"] = default_scan_to_host_port

    for opt, arg in opt_list :
        if opt=="-h" :
            usage( argv[0] )
            sys.exit(1)
        
        elif opt=="-a" :
            scan_options["window"] = parse_area( opt, arg )
            # My command line wants X0,Y0,X1,Y1 but ASP wants "top,left,bottom,right"
            # so need to do a conversion: top=y0, left=x0, bottom=y1, right=x1 
            scan_options["window_top"] = scan_options["window"][1]
            scan_options["window_left"] = scan_options["window"][0]
            scan_options["window_bottom"] = scan_options["window"][3]
            scan_options["window_right"] = scan_options["window"][2]

        elif opt=="-r" :
            scan_options["resolution"] = parse_resolution( opt, arg )

        elif opt=="-t" :
            scan_options["data_type"] = parse_data_type( opt, arg )
            if scan_options["data_type"] == aspnums.SCAN_DATA_MONO :
                scan_options["scan_type"] = aspnums.SCAN_TYPE_MONO
            elif scan_options["data_type"] == XXX_COPY_DATA_TYPE :
                # davep 11-feb-2011; handle copy-to-host; RGBX is the data type
                # currently coming back from the ink copy
                # TODO ; add mono
                scan_options["data_type"] = aspnums.SCAN_DATA_RGBX_PACKED 
                scan_options["scan_type"] = aspnums.SCAN_TYPE_SCAN_THRU_COPY 
            else : 
                scan_options["scan_type"] = aspnums.SCAN_TYPE_COLOR

        elif opt=='-x' :
            (x_numer,x_denom) = parse_fraction( opt, arg )
            scan_options["x_numerator"] = x_numer
            scan_options["x_denominator"] = x_denom

        elif opt=='-y' :
            (y_numer,y_denom) = parse_fraction( opt, arg )
            scan_options["y_numerator"] = y_numer
            scan_options["y_denominator"] = y_denom

        elif opt=="-o" :
            scan_options["outfilename"] = arg

        elif opt=="-z" :
            scan_options["stat_handle"] = parse_ulong( opt, arg )

        elif opt=="-C" :
            scan_options["contrast"] = parse_ulong( opt, arg )

        elif opt=='-B' :
            scan_options["brightness"] = parse_ulong( opt, arg )

        elif opt=='-G' :
            scan_options["gamma"] = parse_ulong( opt, arg )
            if scan_options["gamma"]<10 or scan_options["gamma"]>100 :
                errmsg = "gamma must be in range [10,100]" 
                errorlog( errmsg )
                raise BadCommandLine( opt, arg, errmsg )

        elif opt=='-S' :
            scan_options["sharp"] = parse_ulong( opt, arg )
            if scan_options["sharp"] < 0 or scan_options["sharp"] > 3 :
                errmsg = "sharpness must be in range [0,3]"
                errorlog( errmsg )
                raise BadCommandLine( opt, arg, errmsg )

        elif opt=='-q' :
            scan_options["copy_quality"] = parse_copy_quality( opt, arg )

        elif opt=='-T' :
            scan_options["timeout"] = parse_ulong( opt, arg )

        elif opt=='-c' :
            # simple flag
            scan_options["cal_to_host"] = 1

        elif opt=='-n' :
            # simple flag
            scan_options["page_numbers"] = 1

        # davep 08-Feb-2011 ; add tcp port number to command line
        elif opt=='-p' :
            scan_options["tcp_port"] = parse_ulong( opt, arg )
            if scan_options["tcp_port"] < 1024 or scan_options["tcp_port"] > 65535 :
                errmsg = "Invalid TCP port \"%s\". Must be in [1024,65535]." % arg
                raise BadCommandLine( opt, arg, errmsg )

        # ignore USB options so we can stay compatible with aspscan.c
        elif opt=='-v' :
            pass
        elif opt=='-s' :
            pass

        elif opt=="-i" :
            try :
                # make sure we have a value ip address
                socket.inet_aton( arg )
            except socket.error, ipaddr_error :
                errmsg = "Invalid IP address \"%s\"" % arg
                errorlog( errmsg )
                raise BadCommandLine( opt, arg, errmsg )

            scan_options["ip_address"] = arg

        elif opt=='-b' :
            # bits per pixel
            try :
                scan_options["bpp"] = int( arg, 10 )
            except ValueError :
                errmsg = "Bad integer \"%s\" for bits-per-pixel" % arg
                errorlog( errmsg )
                raise BadCommandLine( opt, arg, errmsg )
                
            if scan_options["bpp"] not in ( 8, 16 ) :
                errmsg = "Invalid value \"%d\" for bits-per-pixel; bpp must be 8 or 16" % scan_options["bpp"]
                errorlog( errmsg )
                raise BadCommandLine( opt, arg, errmsg )

        elif opt=='--docsrc' :
            # davep 17-Sep-2009 ; choose document source (ADF vs flatbed vs ?) 
            scan_options["docsrc"] = parse_document_source( opt, arg )

        elif opt=='--socket-path' :
            # davep 01-Mar-2010 ; add support for unix socket (added to work
            # with usbproxy)
            scan_options["unix_socket_path"] = arg 

        else :
            errorlog( "Unknown command line option." )
            errorlog( "Run with \"-h\" to get usage." )
            sys.exit(1)            

    if not "unix_socket_path" in scan_options : 
        if not 'ip_address' in scan_options :
            errorlog( 'Please specify an IP address with the "-i" option or a unix domain path with "--socket-path".' )
            errorlog( 'Use \"-h\" for full usage help.' )
            sys.exit(1)

    if not 'outfilename' in scan_options :
        errorlog( 'Please specify a destination file for the data with the "-o" option.' )
        sys.exit(1)

    if not 'data_type' in scan_options :
        # default to mono
        scan_options["data_type"] = aspnums.SCAN_DATA_MONO
        scan_options["scan_type"] = aspnums.SCAN_TYPE_MONO

    # davep 08-Feb-2011 ; if using cal-to-host, can't do rgb/rgbx/xrgb; change
    # to planar so downstream behaves correctly
    if "cal_to_host" in scan_options : 
        if scan_options["scan_type"] == aspnums.SCAN_TYPE_COLOR :
            # force data type to planar
            scan_options["data_type"] = aspnums.SCAN_DATA_PLANAR

    return scan_options

if __name__ == '__main__' :
#    usage( sys.argv[0] )

    scan_options = parse_args( sys.argv )
#    try : 
#        scan_options = parse_args( sys.argv )
#    except BadCommandLine, cmdline_error :
#        print cmdline_error
#        sys.exit(1)

    print scan_options

