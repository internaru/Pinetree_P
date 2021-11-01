#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


import sys

# I translated a lot of my original C aspscan directly into Python. Wanted to
# keep the two as similar as possible from the outside, mostly so I wouldn't
# have to learn anything new but also so some of my other tools would simply
# snap into place using the same interfaces
#
# Thus I created a printf() style Python errorlog() to match my log.[ch] stuff.
# 
# davep 12-July-07

debuglevel = 1

def errorlog( fmt, *args ) :
    print >>sys.stderr, fmt % args

def dbg1( fmt, *args ) :
    if debuglevel > 0 :
        print fmt % args

def dbg2( fmt, *args ) :
    if debuglevel > 1 :
        print fmt % args

