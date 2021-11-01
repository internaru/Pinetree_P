/*
 *
 * ============================================================================
 * Copyright (c) 2009-2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
*/ 

#ifndef _INTERNAL_PAGE_CONFIG_H
#define _INTERNAL_PAGE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif


void ip_set_default_config(ipage_config_t *config);

void  internal_pages_font_init(string_mgr_language_t language, uint32_t resolution_x, uint32_t resolution_y);

void internal_pages_font_halt(void);

void internal_page_config_init( void );

#ifdef __cplusplus
}
#endif

#endif
