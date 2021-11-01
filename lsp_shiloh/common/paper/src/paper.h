
/*==========================================================
* (c) Copyright 2009-2010 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

#ifndef PAPER_H
#define PAPER_H

#include <stdint.h>
#include "paper_size_config.h"
#include "paper_types_config.h"
#include "string_mgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * The structure of entries in the paper size table
 */
typedef struct
{
    mediasize_t mediaSize;
    uint32_t width;         ///< width in mm/100 
    uint32_t height;        ///< height in mm/100 
#if 0
    uint16_t PJLStatusCode; ///< The status code returned by pjl to the host for this paper.
    string_mgr_code_t PaperName;      ///< stringmgr token for paper name.
    uint32_t leftMargin;              ///< left margin with respect to printable region in dots
    uint32_t topMargin;               ///< top margin with respect to printable region in dots
    uint32_t leftMarginBorderless;    ///< left borderless margin with respect to printable region in dots
    uint32_t topMarginBorderless;     ///< top borderless margin with respect to printable region in dots
#endif
} engine_media_sizes_t;

/**
 * The structure of entries in the paper types table
 */
typedef struct
{
    mediatype_t mediaType;
} engine_media_types_t;


#ifdef __cplusplus
}
#endif

#endif
