/**
 * \file icefileapp.h
 *
 * \brief Thread and OBA interface to icefile
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef ICEFILEAPP_H
#define ICEFILEAPP_H

#ifdef HAVE_SCAN_ICEFILE_SUPPORT
void icefileapp_onetime_init( void );
scan_err_t icefileapp_run_internal_page_msg( void );
#else
#define icefileapp_onetime_init()
static inline scan_err_t icefileapp_run_internal_page_msg(void) { return SCANERR_NOT_IMPLEMENTED; }
#endif

#endif

