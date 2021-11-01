/*
 * ============================================================================
 * (C) Copyright 2011   Marvell International Ltd.
 *                          All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_STRING_MGR_NVM_H__
#define __INC_STRING_MGR_NVM_H__

#include "string_mgr_config.h"

void                  string_mgr_nvm_init(void);
void                  string_mgr_nvm_set_lang(string_mgr_language_t language);
string_mgr_language_t string_mgr_nvm_get_lang(void);

#endif /* __INC_STRING_MGR_NVM_H__ */
