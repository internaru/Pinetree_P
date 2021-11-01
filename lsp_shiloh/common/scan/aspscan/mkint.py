#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# Use this function all over the place so might as well get away from
# copy/pasting it all over the place.
#
# davep 23-Apr-2008
# $Id: mkint.py 406 2008-05-13 13:40:07Z davep $

def mkint( string ) :
    """Try converting a string to base-10 integer. If that fails, try base-16.
    If that fails, we'll allow the ValueError to propogate."""

    value = None
    try :
        value = int(string,10)
    except ValueError:
        value = None
        pass

    if value is None :
        # ok, maybe it's hex
        value = int(string,16)

    return value

