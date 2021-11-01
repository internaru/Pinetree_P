#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


ERR_NONE=0
ERR_FAIL=-1
ERR_DEV_BUSY=-6
ERR_SETTINGS_FAIL=-8
ERR_ABORT_RECEIVED = -13

# TODO add more later as needed (see aspscan/include/err.h)

error_strings = {
    ERR_NONE : "No error",
    ERR_FAIL : "Generic failure",
    ERR_DEV_BUSY : "Device is busy",
    ERR_SETTINGS_FAIL : "Device rejected our scan settings",
    ERR_ABORT_RECEIVED : "Device aborted the scan" }
    
def strerror( errcode ) :
    # note this will raise an exception if you pass in a non-integer error
    # code
    return error_strings.get( errcode, "Unknown error %d!" % errcode ) 

