/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Avago Technologies, Inc. All Rights Reserved
 *                      
 *                         Avago Confidential
 * ============================================================================
 *
 * Description: Scan ACL command processing routines.
 *
 *****************************************************************************/

#ifndef SCANACL_H
#define SCANACL_H

/* format of the data sent back to the host responding to the
 * ACLCMD_GET_CAL_STATS request.
 *
 * Note this structure has no internal version number. We needed to add a
 * second structure with a version number in order to support changing the
 * data members.  Thus the "V1" on the structure name to emphasize its version.
 *
 * davep 27-Feb-2007
 */
typedef struct ACL_GetCalStatsResponseV1 
{
    UINT32 color_type;
    UINT32 max;
    UINT32 pos_max;
    UINT32 min;
    UINT32 pos_min;
    UINT32 maxPtoP;
    UINT32 pos_maxPtoP;
    UINT32 mean;
    UINT32 num_band1;
    UINT32 num_band2;
    UINT32 num_band3;
    UINT32 numLeft;
    UINT32 numRight;
    UINT32 numBad;
    UINT32 numClipped;
} ACL_GetCalStatsResponseV1;

typedef struct ACL_GetCalStatsResponseV2
{
    /* size of this structure to act as a version number */
    UINT32 sizeof_self;

    UINT32 color_type;
    UINT32 max;
    UINT32 pos_max;
    UINT32 min;
    UINT32 pos_min;
    UINT32 maxPtoP;
    UINT32 pos_maxPtoP;
    UINT32 mean;
    UINT32 num_band1;
    UINT32 num_band2;
    UINT32 num_band3;
    UINT32 numLeft;
    UINT32 numRight;
    UINT32 numBad;
    UINT32 numClipped;

    /* 1 : cal replaced at least 1 cluster of bad pixels (a cluster is usually
     *      3 pixels in a row; see cal.c)
     * 0 : otherwise
     *
     * Note: this isn't a counter because XXX just wanted a flag.
     */
    UINT32 found_bad_pixel_cluster;
} ACL_GetCalStatsResponseV2;

#define SCAN_ACL_CAL_GRAPH_UNCORRECTED_MONO  1
#define SCAN_ACL_CAL_GRAPH_CORRECTED_MONO    2
#define SCAN_ACL_CAL_GRAPH_UNCORRECTED_COLOR 3
#define SCAN_ACL_CAL_GRAPH_CORRECTED_COLOR   4
#define SCAN_ACL_CAL_UNCORRECTED_MONO        9
#define SCAN_ACL_CAL_CORRECTED_MONO         10
#define SCAN_ACL_CAL_UNCORRECTED_COLOR      11
#define SCAN_ACL_CAL_CORRECTED_COLOR        12

/* davep 26-Jul-2011 ; new scanACL subcommands (param1 in scan_acl_request; see
 * also scanACL.c)
 *
 * I have the ACLCMD_SCAN generic ACL command. I can add/remove subcommands
 * from that one command without colliding with ACL numbers across multiple
 * products. The following are used in the 'param1' field of the
 * scan_acl_request.
 */

#define SCAN_ACL_CMD_PING       0
#define SCAN_ACL_CMD_GET_PDLUT  1

void registerScanACLCmds( void );

#endif

