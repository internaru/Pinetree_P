/** 
 * ============================================================================
 * Copyright (c) 2007 Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *  Header file for fakescan.c
 *
 **/
 
#ifndef FAKESCAN_H
#define FAKESCAN_H

/* if we don't have fakescan.c in our build, send fakescan_run() to nothing */
#ifdef HAVE_FAKESCAN_SUPPORT
scan_err_t fakescan_run( void );
#else
#define fakescan_run() SCANERR_NONE
#endif

#endif

