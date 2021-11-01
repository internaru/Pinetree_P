/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <stdint.h>
#include "paper.h"
#include "error_types.h"
#include "paper_types_api.h"
#include "paper_config.h"
#include "lassert.h"

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif


mediatype_t papertypes_get_entry(uint32_t index)
{
    mediatype_t temp;
    error_type_t status;


    status = map_mediatype(index, &temp);
    if(status == FAIL)
    {
        return -1;      // failed, return
    }
    return temp;
}

uint32_t paper_get_num_media_types(void)
{
    return get_num_media_types();
}

mediatype_t * WEAK get_engine_media_types_table()
{
    return 0;
}
bool paper_engine_supports_mediatype( mediatype_t mediatype )
{
	int i;
	int max = 0;
	mediatype_t *table = 0;
	
	max = get_num_media_types();
	table = get_engine_media_types_table();
	
	ASSERT( max && table );
	for ( i = 0; i < max; i++ ) {
		if ( table[i] == mediatype ) {
			return true;
		}
	}
	return false; // not found.
}
#if 0
uint16_t paper_common_map_mediatype_to_id_string_token( mediaSize )
{
	int i;
	int num_elements = sizeof(paper_common_cp_table) / sizeof(paper_common_cp_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( paper_common_cp_table[i].mediasize == mediaSize ) {
			return paper_common_cp_table[i].PaperName;
		}
	}
	return 0; // not found.
}
#endif
#if 0
uint16_t paper_common_map_mediaSize_to_pjl_code( mediaSize )
{
	int i;
	int num_elements = sizeof(paper_common_pjl_table) / sizeof(paper_common_pjl_table_t);

	for (i = 0; i < num_elements; i++ ) {
		if ( paper_common_pjl_table[i].mediasize == mediaSize ) {
			return paper_common_pjl_table[i].pjl_code;
		}
	}
	return 0; // not found.
}

uint16_t paper_get_pjl_code(mediasize_t mediaSize)
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_common_map_mediaSize_to_pjl_code( mediaSize );
	}
	return -1;
}

const char * paper_common_map_mediaSize_to_pjl_name( mediaSize )
{
	int i;
	int num_elements = sizeof(paper_common_pjl_table) / sizeof(paper_common_pjl_table_t);

	for (i = 0; i < num_elements; i++ ) {
		if ( paper_common_pjl_table[i].mediasize == mediatype ) {
			return paper_common_pjl_table[i].pjl_name;
		}
	}
	return 0; // not found.
}

const char * mediatype_get_pjl_name(mediatype_t mediatype)
{
	if ( paper_engine_supports_mediatype( mediatype ) ) {
		return paper_common_map_mediatype_to_pjl_name( mediatype );
	}
	return 0;
}

#endif




error_type_t paper_media_type_is_valid( mediatype_t media_type )
{
    uint32_t num_media_types;
    uint32_t media_type_index;
    error_type_t e_res;
    error_type_t e_res_media_type;
    bool media_type_valid;
    mediatype_t media_type_actual;

    num_media_types = get_num_media_types();

    e_res = OK;
    media_type_valid = false;

    for ( media_type_index = 0; media_type_index < num_media_types; media_type_index++ )
    {
        e_res_media_type = map_mediatype( media_type_index, &media_type_actual );

        if ( OK == e_res_media_type )
        {
            if ( media_type == media_type_actual )
            {
                media_type_valid = true;
            }
        }
        else
        {
            e_res = FAIL;
        }
    }

    if ( OK == e_res )
    {
        if ( false == media_type_valid )
        {
            e_res = FAIL;
        }
    }

    return e_res;
}

