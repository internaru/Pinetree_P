
/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifndef _UFST_FONT_DRIVER_IP_H
#define _UFST_FONT_DRIVER_IP_H

#include "cgconfig.h"     /* this must be first  */
#include "ufstport.h"     /* this must be second */
#include "shareinc.h"     /* then this ufst include probably should be third */

#ifdef __cplusplus
extern "C" {
#endif


void ufst_font_driver_internal_pages_init(string_mgr_language_t lang, uint32_t resolution_x, uint32_t resolution_y);
void ufst_font_driver_ip_setup_font( IF_STATE **ifs, string_mgr_language_t lang, uint32_t size_pts, bool Rotate );
void getCharBmp(IF_STATE *ifs, uint32_t chId, PPIFBITMAP ppbm);

void ufst_font_driver_internal_pages_halt(void);
int32_t char_escape( PIFBITMAP pbm);


#ifdef __cplusplus
}
#endif

#endif
