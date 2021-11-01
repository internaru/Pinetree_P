/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/**\file fax_dbg_test.h
 * The file defines the fax tests
 */
#ifndef __FAX_DBG_TEST_H__
#define __FAX_DBG_TEST_H__


extern fax_app_brightness_t brightness;
extern fax_app_resolution_t resolution;

void _fax_dbg_start_send_loop(fax_app_type_t type, char *number, char *file);
void _fax_dbg_end_send_loop(void);
char * _fax_dbg_get_error_string(fax_app_comp_error_t err);

#endif /* __FAX_DBG_TEST_H__ */
