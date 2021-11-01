/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file scantasktest.h
 *
 *  \brief Demo application thread showing how to use the scan BSDK.
 *
 *  davep 19-July-2008
 */

#ifndef SCANTASKTEST_H
#define SCANTASKTEST_H

/* blocking run our demo scans */
scan_err_t run_demo_scan( void );

/* start a thread to run a single instance of run_demo_scan() */
scan_err_t start_scantask_test( void );

#endif

