#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Run a random scan. Random color/mono, random DPI, random area (subject to
# constraints). 
# Written to do overnight testing.
# davep 6-Mar-2007

import sys
import random
import time
import getopt

import scan 

dpi_range = ( 75, 1200 )
#dpi_choices= ( 75, 100, 150, 200, 300 )
dpi_choices= ( 300, 600, 1200 )

#valid_scan_types = ( "color", "mono" )
valid_scan_types = ( "rgbx", "xrgb", "rgb", "color", "mono" )

x_area_range = ( 0, 850 )
y_area_range = ( 0, 1169 )
#y_area_range = ( 0, 1100 )
area_min = 100

# fraction: scale = [0]/[1] 
min_scale = ( 1, 16 )
max_scale = ( 8, 1 )

# davep 02-Apr-2009 ; allow option to disable random scaling for platforms that
# don't support scaler (e.g., ICE Lite color scaling broken)
use_random_scale = True

def random_dpi() :
    # -1 to convert from length to index

    # davep 10-Jun-2011 ; temp for testing
#    return 300

    return dpi_choices[ random.randint(0,len(dpi_choices)-1) ]
#    return random.randint( dpi_range[0], dpi_range[1] )

def random_scan_type() :
    # -1 to convert from length to index
    return valid_scan_types[ random.randint(0,len(valid_scan_types)-1) ]

def random_area() :
    upper_left_x = random.randint( x_area_range[0], x_area_range[1]-area_min )
    upper_left_y = random.randint( y_area_range[0], y_area_range[1]-area_min )
    
    width_x = random.randint( area_min, x_area_range[1] )
    width_y = random.randint( area_min, y_area_range[1] )

    lower_right_x = min( upper_left_x+width_x, x_area_range[1] )
    lower_right_y = min( upper_left_y+width_y, y_area_range[1] )

    assert lower_right_x-upper_left_x >= area_min, (lower_right_x,upper_left_y)
    assert lower_right_y-upper_left_y >= area_min, (lower_right_y,upper_left_y)

    return (upper_left_x,upper_left_y,lower_right_x,lower_right_y)

def random_scale() :
    min_scale_f = float(min_scale[0]) / float(min_scale[1])
    max_scale_f = float(max_scale[0]) / float(max_scale[1])

    while 1 :
#        numer = random.randint( 1, 65536 )
#        denom = random.randint( 1, 65536 )

        # limit to signed 16-bit so we don't overflow the firmware (need to fix
        # the firmware!)
        numer = random.randint( 1, 2**15-1 )
        denom = random.randint( 1, 2**15-1 )

        scale = float(numer)/float(denom)
        if scale >= min_scale_f and scale <= max_scale_f :
            break

    return (numer,denom)
    
def maybe() :
    return 1
    return random.randomint( 0, 1 )

def random_scan( scan_args_hash, my_args ) :
    scan_args = []

    # first the required arguments
    dpi = random_dpi()
    scan_type = random_scan_type()

    scan_args.extend( ("-o", scan_args_hash["-o"]) )
    scan_args.extend( ("-r", "%d"%dpi) )
    scan_args.extend( ("-t", scan_type) )

    # now put together optional arguments 

    for optional_key in ( "-p", "-i", "-e", "-v" ) :
        if scan_args_hash.has_key( optional_key ) :
            scan_args.extend( ( optional_key, scan_args_hash[optional_key] ) )

    if maybe() :
        area = random_area()
        area_str = ",".join( [ "%d"%num for num in area ] )
        scan_args.extend( ( "-a", area_str ) )

    # avoid trying to capture too much data ; the tests will take forever
    bad_scale = 1
    while use_random_scale and bad_scale :
        scale = 1
        scale = 1

        # X scale
        if maybe() :
            scale = random_scale()
            scale_str_x = "/".join( [ "%d"%num for num in scale ] )
            scale_x = float(scale[0]) / float(scale[1])

        # Y scale
        if maybe() :
            scale = random_scale()
            scale_str_y = "/".join( [ "%d"%num for num in scale ] )
            scale_y = float(scale[0]) / float(scale[1])

        # Now calculate the scan area ; if too big, go back and get a different
        # scale.
         
        # area[] is x0,y0,x1,y1 in 1/100"
        out_x = ((area[2] - area[0])/100.0) * dpi * scale_x
        out_y = ((area[3] - area[1])/100.0) * dpi * scale_y

        print "scale_x=%s scale_y=%s" % (str(scale_x),str(scale_y))
        print "out_x=%s out_y=%s" % (str(out_x),str(out_y))

        # 8.5" at 1200 DPI is 10200 pixels
        # we run into memory problems if we scale X up beyond that ; the
        # firmware doesn't catch the problems yet
        max_x = 11000
        if out_x > max_x:
            print "out_x=%d max_x=%d ; scan too big" % (out_x,max_x)
        else :
            bad_scale = 0

    if use_random_scale : 
        scan_args.extend( ( "-x", scale_str_x ) )
        scan_args.extend( ( "-y", scale_str_y ) )

    scan_args_str = " ".join(scan_args)
    print "scan_args={0}".format( scan_args_str )

    # write it to a logfile in case the scan crashes 
    logfile = file( "random_scan.log", "a" )
    logfile.write( scan_args_str + "\n" )
    logfile.close()

    # davep 11-Oct-2012 ; add "debug" flag so we will just print the random
    # results without doing the scan
    if "debug" in my_args : 
        print scan_args_str
        # pretend we succeeded
        return 0
    
    aspscan_exit_code = scan.run_scan( scan_args )
#    aspscan_exit_code = 0

    return aspscan_exit_code

def usage() :
    print "run a random scan"
    print "usage: %s \"hdv:p:i:e:o:\" [--infinite] output_filename" % sys.argv[1]
    print "  -h   # show this help"
    print "  -d   # debug ; only print the scan args, don't run the scan"
    print "  -o outputfile" 
    print "  -v num" 
    print "     USB Vendor ID"
    print "  -p num"
    print "     USB Product ID"
    print "  -i num" 
    print "     USB Interface"
    print "  -e out[,in]"
    print "     USB Out,In Endpoints (default is 0x%x,0x%x)"
    print "     the out endpoint must always be provided, the in endpoint is optional." 
    print "     note direction is with respect to the host: out is host to device (usb write)"
    print "  --infinite  # loop, running scans forever"

def bad_cmdline( msg ) :
    print "Invalid command line : %s" % msg
    print "Use -h for help."
    sys.exit(1)

def parse_args( args_list ) :
    scan_args = {}
    my_args = {}

    # default set of arguments 
    scan_args["-o"] = "foo" 

    longopts = ["infinite","help"]

    try:
        opt_list, remain_list = getopt.getopt(sys.argv[1:], "hdi:e:v:p:o:", longopts )
    except getopt.GetoptError,e:
        # print help information and exit:
        bad_cmdline( e )

#    print opt_list 
#    print remain_list

    for opt, arg in opt_list:
#        print opt, arg

        if opt in ("-h", "--help"):
            usage()
            sys.exit()
        elif opt == "--infinite" :
            # loop around random scan forever (or until we crash)
            my_args["infinite"] = 1
        elif opt in ("-i", "-e", "-v", "-p", "-o" ) :
            scan_args[opt] = arg
        elif opt == "-d" :
            my_args["debug"] = 1
        else:
            assert False, "unhandled option"

    return my_args,scan_args

def main() : 
    (my_args,scan_args) = parse_args( sys.argv[1:] )

#    print scan_args
#    return

    while 1 :
        try : 
            random_scan( scan_args, my_args )
        except Exception,e:
            print >>sys.stderr, e
            logfile = file( "random_scan.log", "a" )
            logfile.write( "scan failed\n" )
            logfile.close()
            sys.exit(1)

        if not my_args.has_key("infinite") :
            # only run once
            break

        # this time is not foolproof!
        time.sleep(8)


if __name__ == '__main__' :
    main()

