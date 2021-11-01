/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file demo_scanapp.h
 *
 *  \brief Demo application thread showing how to use the scan from within the OBA.
 *
 *  davep 23-May-2010
 */

#ifndef DEMO_SCANAPP_H
#define DEMO_SCANAPP_H

/* don't turn on demo_scanapp unless you've added demo_scanapp to ResourceMap.c
 * ResourceMap.h agRouter.h agResourceMgr.h and fixed scan's makefile to
 * include demo_scanapp.c
 */
#ifdef HAVE_DEMO_SCANAPP
    void demo_scanapp_start_job( void );
    void demo_scanapp_init( void );
#else
    #define demo_scanapp_start_job()
    #define demo_scanapp_init()
#endif

#endif

