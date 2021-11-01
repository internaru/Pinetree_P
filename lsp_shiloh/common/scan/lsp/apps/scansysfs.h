/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/*  Paths to communicate with linux kernel scantask via sysfs.
 *
 *  Do NOT use these paths directly unless you're writing scan core code. These
 *  are PRIVATE to the scan code. No peeking! You'll blow your lips off.
 *
 *  There are API functions to do this work for you. 
 *
 *  davep 24-Jan-2013
 *
 */

#ifndef SCANSYSFS_H
#define SCANSYSFS_H

/* scan command line (test/debug) */
#define SCAN_CMDLINE_SYSFS_PATH "/sys/firmware/scan/cmdline"

/* davep 24-Jan-2013 ; path to scanmech adf client pause/resume. 
 * WARNING! This name is tentative and will change 
 */
#define SCAN_CMDLINE_SCANMECH_PATH "/sys/firmware/scan/scanmech"

/* davep 14-May-2013 ; path to adf paper present */
#define SCAN_ADF_PAPER_PRESENT_PATH "/sys/firmware/scan/adf_pp"

#endif

