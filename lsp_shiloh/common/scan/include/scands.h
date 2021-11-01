/**
 * \file scands.h
 *
 * \brief Scanner Data Store. Modeled after the netsnmp_ds_*() functions
 *
 * Variables for config/control of low level Scantask behavior.
 *
 * Experts only! Use with care.
 */

/*
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANDS_H
#define SCANDS_H

scan_err_t scands_onetime_init( void );

void scands_debug_dump( void );

/* The get/set functions return:
 *
 *  SCANERR_NO_ENTRY      - key does not exist in table
 *  SCANERR_INVALID_PARAM - attempt to get/set key of wrong type (e.g., calling
 *                          get on a boolean value)
 *  SCANERR_NOT_PERMITTED - access scands before scands_onetime_init() called
 *  SCANERR_ALREADY       - attempt to set value that already exists
 *                          (scands_set_xxx_if_null() functions)
 */

scan_err_t scands_get_integer( const char *name, uint32_t *value );
scan_err_t scands_get_integer_with_default( const char *name, uint32_t *value, uint32_t default_value );
scan_err_t scands_set_integer( const char *name, uint32_t value );
scan_err_t scands_delete_integer( const char *name );

/* maximum length of the name in the scands_xxx() functions */
#define SCANDS_MAX_NAMELEN 32

/* davep 02-Dec-2010 ; originally the scands used integer keys. Then I added
 * string keys. Now I removed the integer keys. But some old code uses the old
 * names. These #def's are here for backwards compatibility.
 */
#define scands_get_integer_by_name    scands_get_integer
#define scands_set_integer_by_name    scands_set_integer
#define scands_delete_integer_by_name scands_delete_integer

#endif

