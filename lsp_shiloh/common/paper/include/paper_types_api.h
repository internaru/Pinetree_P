
/* ==========================================================
* (c) Copyright 2009 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

#ifndef PAPERTYPES_API_H
#define PAPERTYPES_API_H

#include <stdint.h>
#include <stdbool.h>
#include "paper_types_config.h"        ///< this includes the mediatype_t 
#include "string_mgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Returns the number of media types we support in the system.
 * \retval Number of media types supported by the system
 */
uint32_t paper_get_num_media_types(void);

/**
 * \brief Get an entry in the paper types table. 
 * Given an index into the table, this returns the mediatype_t
 * entry for that entry in the table.  This is thread safe so
 * will work no matter how many people call it.
 * \param[in] index Index into the table
 * \returns mediatype_t
 * \retval The media type
 * \retval -1 End of the table 
 */
mediatype_t papertypes_get_entry(uint32_t index);

/**
 * \brief Validate a paper type
 *
 * \param[in] media_type the media type to validate
 *
 * \return error_type_t
 * OK if the media type is a valid system supported media type
 * FAIL otherwise
 *
 */
error_type_t paper_media_type_is_valid( mediatype_t mediatype );


bool paper_engine_supports_mediatype( mediatype_t mediatype );

const char * paper_mediatype_get_pjl_name(mediatype_t mediatype);

#ifdef __cplusplus
}
#endif

#endif
