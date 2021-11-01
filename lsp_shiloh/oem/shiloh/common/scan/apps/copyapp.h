/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004 - 2008 Marvell Semiconductor, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/


#ifndef COPYAPP_H
#define COPYAPP_H

#include "copyapp_api.h"
#include "print_job_api.h"
		      
void CopyAppInit( void );
scan_err_t copyapp_setup_scanning( struct scanvars **psv );

print_page_t* copyapp_printer_create_page( PAGEINFO *page_info );
void copyapp_printer_start_page( print_page_t* print_page, const PAGEINFO *page_info );
void copyapp_send_data( print_page_t* print_page, int num_rows, uint32_t current_lines,
                        bool last_strip, const PAGEINFO *page_info, void *data,
                        uint32_t comp_type, uint32_t buf_size, 
                        bool print_right_to_left, uint32_t image_num);  

void copyapp_printer_end_page( print_page_t* print_page, const PAGEINFO *page_info);
void set_copylog_level(int n);
void copyapp_OOM(void);

bool is_idcopy_enabled( void );

void print_copy_config_settings(bool);
void copyapp_init_settings( CopyConf *conf );

#endif

