
/*
 * ============================================================================
 * Copyright (c) 2009-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef _PAPERCONFIG_H
#define _PAPERCONFIG_H
#include <stdint.h>
#include "error_types.h"
#include "paper_size_api.h"
#include "paper_types_api.h"

#ifdef __cplusplus
extern "C" {
#endif

//all of these are in the paper config files.

error_type_t map_mediasize(int Value, mediasize_t *mediaSize);
string_mgr_code_t get_paper_id(mediasize_t MediaSize);
uint16_t get_pjl_code(mediasize_t MediaSize);
uint32_t get_num_media_sizes(void);
error_type_t get_media_size(mediasize_t Media, uint32_t *width, uint32_t *height);
uint32_t get_num_media_types(void);
error_type_t map_mediatype(int Value, mediatype_t *mediaType);

#ifdef __cplusplus
}
#endif

#endif
