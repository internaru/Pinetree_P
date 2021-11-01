#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Draw lines to separate the blocks of an image. 
#
# Originally written to take a cal-to-host data blob converted into a single
# image and draw lines between each stage of the cal.
#
# davep 21-Nov-2008
# $Id: //unity/firmware/modules/common/scan/aspscan/drawgrid.py#2 $

import os
import sys
import string
import math

import imgmagick

debug=0

def draw( filename, rows_per_block, cols_per_block ) :

    width,height = imgmagick.get_image_size( filename )

#    print width, height

    cmd = "mogrify -fill none -stroke SteelBlue -strokewidth 1 "
    
    if rows_per_block > 0 : 
        for i in range( 0, height, rows_per_block ) :
            line_cmd = "-draw \"line %d,%d %d,%d\"" % (0,i,width,i)
            cmd += line_cmd + " "

    if cols_per_block > 0 :
        for i in range( 0, width, cols_per_block ) :
            line_cmd = "-draw \"line %d,%d %d,%d\"" % (i,0,i,height)
            cmd += line_cmd + " "

    cmd += filename

#    print cmd

    f=os.popen( cmd )
    lines=f.readlines()
    f.close()

def main() :

    if len(sys.argv) != 4 :
        print "usage: %s filename rows-per-block cols-per-block" % sys.argv[0]
        print "Use rows/cols-per-block==0 to disable"
        sys.exit(1)

    filename = sys.argv[1]

    try : 
        rows_per_block = int(sys.argv[2])
    except ValueError :
        print >>sys.stderr, "Invalid integer \"%s\" for rows-per-block" % sys.argv[2]
        sys.exit(1)
    try :
        cols_per_block = int(sys.argv[3])
    except ValueError :
        print >>sys.stderr, "Invalid integer \"%s\" for cols-per-block" % sys.argv[3]
        sys.exit(1)

    draw( filename, rows_per_block, cols_per_block )

if __name__ == '__main__' :
    main()

