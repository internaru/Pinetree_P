/**
 * \file scancont.h
 *
 * \brief Continuous scan, a test/debug tool.
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANCONT_H
#define SCANCONT_H

void scan_continuous_start( int requested_max_iterations );
void scan_continuous_stop( void );
void scan_continuous_get_status(bool *is_running, int *max_iterations, int *current_count);

#endif

