/**
 * \file scantest.h
 *
 * \brief header file for scantest.c
 *
 */
/** 
 *
 * ============================================================================
 * Copyright (c) 2007, 2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 **/
 
#ifndef SCANTEST_H
#define SCANTEST_H

scan_err_t scantest_simple_sensor( void );

scan_err_t scantest_with_data( void );

scan_err_t scantest_sensor( scan_cmode_t cmode, uint32_t dpi, uint32_t num_lines );

scan_err_t scantest_sensor_with_data( scan_cmode_t cmode, uint32_t dpi, uint32_t num_lines );

scan_err_t scantest_run_all_tests( void );

scan_err_t scantest_run_finger_test( void );

#endif

