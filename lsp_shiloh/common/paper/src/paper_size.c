/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <stdint.h>
#include "paper.h"
#include "error_types.h"
#include "paper_size_api.h"
#include "paper_config.h"
#include "logger.h"
#include "lassert.h"

#ifdef HAVE_UNIT_TEST
/* in paper_tests */
extern void register_paper_unittest(void);
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

//
// This table defines the common media sizes along with the dimensions, pjl codes and string tokens.
//
WEAK engine_media_sizes_t  paper_media_sizes[] =
{    // Media size enum                     width  Length, pjlcode, stringmgr token, left margin, top margin, borderless left margin, borderless top margin
    {MEDIASIZE_ANY,                          21590,  27940 },
    {MEDIASIZE_LETTER,                       21590,  27940 },   //8.50 x 11.00 in   215.9004 x 279.4006 mm
    {MEDIASIZE_LEGAL,                        21590,  35560 },   //8.50 x 14.00 in   215.9004 x 355.6007 mm
    {MEDIASIZE_STATEMENT,                    13970,  21590 },   //5.5 x  8.5 in  - 139.7 x 215.9 mm 
    {MEDIASIZE_EXECUTIVE,                    18415,  26670 },   //7.25 x 10.50 in   184.1504 x 266.7005 mm
    {MEDIASIZE_CUSTOM_85x13,                 21590,  33020 },   //8.50 x 13.00 in   215.9004 x 330.2007 mm US FOLIO
    {MEDIASIZE_A4,                           21000,  29700 },   //8.27 x 11.69 in   210.0584 x 296.9266 mm
    {MEDIASIZE_A5,                           14800,  21000 },   //5.83 x 8.27  in   148 x 210 mm
    {MEDIASIZE_A6,                           10500,  14800 },
    {MEDIASIZE_B5,                           18200,  25700 },   //7.17 x 10.12 in   182 x 257 mm
    {MEDIASIZE_B6_JIS,                       12800,  18200 },   
    {MEDIASIZE_CUSTOM_POSTCARD_JIS,          10000,  14800 },   //3.94 x 5.83  in   100.0762 x 148.0823 mm
    {MEDIASIZE_CUSTOM_DBL_POSTCARD_JIS,      14800,  20000 },   //5.83 x 7.87  in   148.0823 x 199.8984 mm
    {MEDIASIZE_CUSTOM_16K,                   19685,  27305 },   //7.75 x 10.75 in   196.8504 x 273.0505 mm
    {MEDIASIZE_ENV_9,                         9840,  22510 },   //
    {MEDIASIZE_ENV_10,                       10477,  24130 },   //4.12 x 9.50  in   104.6482 x 241.3005 mm
    {MEDIASIZE_ENV_MONARCH,                  9842,   19050 },   //3.87 x 7.50  in   98.2982 x 190.5004  mm
    {MEDIASIZE_ENV_C5,                       16200,  22900 },   //6.38 x 9.02  in   162.0523 x 229.1085 mm
    {MEDIASIZE_ENV_C6,                       11400,  16200 },  
    {MEDIASIZE_ENV_DL,                       11000,  22000 },   //4.33 x 8.66  in   109.9822 x 219.9644 mm
    {MEDIASIZE_ENV_B5,                       17600,  25000 },   //6.93 x 9.84  in   176.0224 x 249.9365 mm
    {MEDIASIZE_JAPANESE_POSTCARD,            10000,  14800 },
    {MEDIASIZE_DBL_JAPANESE_POSTCARD,        14800,  20000 },
    {MEDIASIZE_CUSTOM_16K_184_260,           18400,  26000 },
    {MEDIASIZE_CUSTOM_16K_195_270,           19500,  27000 },
    {MEDIASIZE_CUSTOM_4x6,                   10160,  15240 },
    {MEDIASIZE_CUSTOM_5x8,                   12700,  20320 },
    {MEDIASIZE_CUSTOM_8x10,                  20320,  25400 },
    {MEDIASIZE_CUSTOM_8x105,                 20320,  26670 },
    {MEDIASIZE_CUSTOM_825x14,                20955,  35560 },
    {MEDIASIZE_CUSTOM_10x15cm,               10000,  15000 },
    {MEDIASIZE_ENV_PERSONAL,                 9208,   16510 },   // 6 3/4 envelope

// begin rotated sizes 
    {MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED,14800,  20000 },
    {MEDIASIZE_A5_ROTATED,                   21000,  14800 },
// end rotated sizes
// A3 merge size
	{MEDIASIZE_A3,                           29700,  42000 },
	{MEDIASIZE_11X17,                        27940,  43180 },
    {MEDIASIZE_A4_ROTATED,					29700,	21000 },
    {MEDIASIZE_USER_DEFINED,                 21590,  27940 },   
};
#define NUM_COMMON_MEDIA_SIZES sizeof(paper_media_sizes)/sizeof(engine_media_sizes_t)

typedef struct paper_common_cp_table_s
{
	mediasize_t mediasize;
	string_mgr_code_t PaperName;      ///< stringmgr token for paper name.
} paper_common_cp_table_t;

WEAK paper_common_cp_table_t paper_common_cp_table[] = {
	{MEDIASIZE_ANY,                     STRING_CP_SIZE_ANY_SIZE},
	{MEDIASIZE_LETTER,                  STRING_CP_SIZE_LETTER},   //8.50 x 11.00 in   215.9004 x 279.4006 mm
	{MEDIASIZE_LEGAL,                   STRING_CP_SIZE_LEGAL},   //8.50 x 14.00 in   215.9004 x 355.6007 mm
	{MEDIASIZE_EXECUTIVE,               STRING_CP_SIZE_EXECUTIVE},   //7.25 x 10.50 in   184.1504 x 266.7005 mm
	{MEDIASIZE_CUSTOM_85x13,            STRING_CP_SIZE_CUSTOM85x13},   //8.50 x 13.00 in   215.9004 x 330.2007 mm
	{MEDIASIZE_A4,                      STRING_CP_SIZE_A4},        //8.27 x 11.69 in   210.0584 x 296.9266 mm
	{MEDIASIZE_A5,                      STRING_CP_SIZE_A5},   //5.83 x 8.27  in   148 x 210 mm
	{MEDIASIZE_B5,                      STRING_CP_SIZE_B5},   //7.17 x 10.12 in   182 x 257 mm
	{MEDIASIZE_CUSTOM_POSTCARD_JIS,     STRING_CP_SIZE_CUS_POSTCARD},   //3.94 x 5.83  in   100.0762 x 148.0823 mm
	{MEDIASIZE_CUSTOM_DBL_POSTCARD_JIS, STRING_CP_SIZE_CUS_DBL_PC},   //5.83 x 7.87  in   148.0823 x 199.8984 mm
	{MEDIASIZE_CUSTOM_16K,              STRING_CP_SIZE_16K},   //7.75 x 10.75 in   196.8504 x 273.0505 mm
	{MEDIASIZE_ENV_10,                  STRING_CP_SIZE_ENV_10},   //4.12 x 9.50  in   104.6482 x 241.3005 mm
	{MEDIASIZE_ENV_MONARCH,             STRING_CP_SIZE_ENV_MONARCH},   //3.87 x 7.50  in   98.2982 x 190.5004  mm
	{MEDIASIZE_ENV_C5,                  STRING_CP_SIZE_ENV_C5},   //6.38 x 9.02  in   162.0523 x 229.1085 mm
	{MEDIASIZE_ENV_DL,                  STRING_CP_SIZE_ENV_DL},   //4.33 x 8.66  in   109.9822 x 219.9644 mm
	{MEDIASIZE_ENV_B5,                  STRING_CP_SIZE_ENV_B5},   //6.93 x 9.84  in   176.0224 x 249.9365 mm
	{MEDIASIZE_A6,                      STRING_EWS_A6},
	{MEDIASIZE_JAPANESE_POSTCARD,       STRING_CP_SIZE_JAP_PC},
	{MEDIASIZE_DBL_JAPANESE_POSTCARD,   STRING_CP_SIZE_DBL_JAP_PC},
	{MEDIASIZE_CUSTOM_16K_184_260,      STRING_CP_SIZE_CUS16K_184},
	{MEDIASIZE_CUSTOM_16K_195_270,      STRING_CP_SIZE_CUS16K_195},
	{MEDIASIZE_CUSTOM_4x6,              STRING_CP_SIZE_CUS_4x6},
	{MEDIASIZE_CUSTOM_5x8,              STRING_CP_SIZE_CUS_5x8},
	{MEDIASIZE_CUSTOM_10x15cm,          STRING_CP_SIZE_CUS_10x15},
	{MEDIASIZE_ENV_PERSONAL,            STRING_CP_SIZE_ENV_PERSONAL},   // 6 3/4 envelope

// begin missing strings
	{MEDIASIZE_A5_ROTATED,              STRING_CP_SIZE_A5},
	{MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED,   STRING_CP_SIZE_DBL_JAP_PC},
	{MEDIASIZE_CUSTOM_8x10,             STRING_CP_SIZE_ANY_SIZE},
	{MEDIASIZE_CUSTOM_8x105,            STRING_CP_SIZE_ANY_SIZE},
	{MEDIASIZE_CUSTOM_825x14,           STRING_CP_SIZE_ANY_SIZE},
// end missing strings
	{MEDIASIZE_USER_DEFINED,            STRING_CP_SIZE_ANY_SIZE}, 	
};



WEAK paper_common_pjl_table_t paper_common_pjl_table[] = {
	{MEDIASIZE_ANY, 0, "ANY"}, /// < help 
		// 1, "unknown envelope"
	{MEDIASIZE_LETTER, 2, "LETTER"},
	{MEDIASIZE_LEGAL, 3, "LEGAL"},
	{MEDIASIZE_A4, 4, "A4"},
	{MEDIASIZE_EXECUTIVE, 5, "EXECUTIVE"},
		// 6, "LEDGER"
		// 7, "A3"
	{MEDIASIZE_ENV_10, 8, "COM10"},
	{MEDIASIZE_ENV_MONARCH, 9, "MONARCH"},
	{MEDIASIZE_ENV_C5, 10, "C5"},
	{MEDIASIZE_ENV_DL, 11, "DL"},
		// 12, "JIS B4"
		// 13, "JIS B5"
	{MEDIASIZE_ENV_B5, 14, "B5"},
	{MEDIASIZE_USER_DEFINED, 15, "CUSTOMSIZE"},
	{MEDIASIZE_JAPANESE_POSTCARD, 16, "JPOST"},
	{MEDIASIZE_DBL_JAPANESE_POSTCARD, 17, "JPOSTD"},
	{MEDIASIZE_B5, 17, "B5"},
	{MEDIASIZE_A5, 18, "A5"},
		// 19, Letter-R
	{MEDIASIZE_A4_ROTATED, 20, "A4-R"},
	{MEDIASIZE_A5_ROTATED, 21, "A5-R"},
		// 21, A5-R
		// 23, EXEC-R
	{MEDIASIZE_A6, 24, "A6"},
		// 25, foolscap
	{MEDIASIZE_B6_JIS, 26, "B6"},

};



void paper_size_init( void )
{
    #ifdef HAVE_UNIT_TEST
    register_paper_unittest();
    #endif
}

mediasize_t paper_get_entry(uint32_t index)
{
    mediasize_t temp;
    error_type_t status;

    status = map_mediasize(index, &temp);

    if (status == FAIL)
    {
        temp = MEDIASIZE_BAD;       
    }

    return temp;
}

uint32_t paper_get_num_media_sizes(void)
{
    return get_num_media_sizes();
}

mediasize_t * WEAK get_engine_media_sizes_table( ) 
{
	return 0;
}
bool paper_engine_supports_mediasize( mediasize_t mediaSize )
{
	int i;
	int max = 0;
	mediasize_t *table = 0;
	
	max = get_num_media_sizes();
	table = get_engine_media_sizes_table();
	
	ASSERT( max && table );
	for ( i = 0; i < max; i++ ) {
		if ( table[i] == mediaSize ) {
			return true;
		}
	}
	return false; // not found.
} 

uint16_t paper_common_map_mediaSize_to_id_string_token( mediaSize )
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

string_mgr_code_t paper_get_id_string_token(mediasize_t mediaSize)
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_common_map_mediaSize_to_id_string_token( mediaSize );
	}
	return -1;
}

/**
 * \brief Look up the sizes of the media based on Media and return them
 * \param[in] Media The enum for the media size
 * \param[out] width The width of the media in mm.
 * \param[out] height The height of the media in mm
 * \return error_type_t
 * \retval OK The media was found and the width and height are correct.
 * \retval FAIL The media was not found, width and height are invalid.
 */
error_type_t paper_get_media_size(mediasize_t mediaSize, uint32_t *width, uint32_t *height)
{
	int num_elements = sizeof(paper_media_sizes)/sizeof(engine_media_sizes_t);
	int i;	

	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		for ( i = 0; i < num_elements; ++i ) {
			if ( paper_media_sizes[i].mediaSize == mediaSize ) {
				// Found it, now copy the result
				*width = paper_media_sizes[i].width;
				*height = paper_media_sizes[i].height;
				return OK;
			}
		}
	}
	return -1; // not found.
}


static error_type_t get_media_size_enum( float width, float height, paper_size_units_t units, mediasize_t * media_size )
{
    error_type_t enum_match_found = FAIL;
    mediasize_t possible_size = MEDIASIZE_ANY;
    uint32_t num_media_sizes = 0;
    uint16_t i = 0;
    float new_width = width;
    float new_height = height;


    num_media_sizes = paper_get_num_media_sizes();;

    switch (units)
    {
        case PAPER_UNITS_INCH:
            width  = new_width * 2540;
            height = new_height * 2540;
        break;
        case PAPER_UNITS_MM:
            width  = new_width * 100;
            height = new_height * 100;
        break;
        case PAPER_UNITS_600DPI:
            width  = new_width/600 * 2540;
            height = new_height/600 * 2540;
            break;
        case PAPER_UNITS_1200DPI:
            width  = new_width/1200 * 2540;
            height = new_height/1200 * 2540;
            break;
        case PAPER_UNITS_MMx10:
            width  = new_width * 10;
            height = new_height * 10;
            break;
        case PAPER_UNITS_MMx1000:
            width  = new_width/10;
            height = new_height/10;
            break;
        case PAPER_UNITS_INx100:
            width  = new_width  * 0.254;
            height = new_height * 0.254;
            break;
        case PAPER_UNITS_INx1000:
            width  = new_width  * 0.0254;
            height = new_height * 0.0254;
            break;
        case PAPER_UNITS_INx10000:
            width  = new_width  * 0.00254;
            height = new_height * 0.00254;
            break;
        case PAPER_UNITS_72DPI:
            width  = ((new_width * 2540.0) / 72.0f);
            height = ((new_height * 2540.0) / 72.0f);
            break;
        case PAPER_UNITS_MMx100: // no need to change since size data is currently stores in MMx100
        default:
            break;
    }
    
    uint32_t xy_delta = 0x0fffffff;
    for( i = 0; i < num_media_sizes; i++)
    {
        mediasize_t tmp_size = paper_get_entry( i );

        if( (tmp_size != MEDIASIZE_USER_DEFINED) && (tmp_size != MEDIASIZE_ANY))
        {
            uint32_t media_width;
            uint32_t media_height;
	    uint32_t delta; // error only valid if on the page

            paper_get_media_size(tmp_size, &media_width, &media_height ) ;
	    delta = (media_width - width) + (media_height - height);

	    // less error in x and y, both x and y are NOT larger than the paper size
	    if ( xy_delta >= delta  && (media_width - width) >= 0 && (media_height - height) >= 0 )
	    {
		xy_delta = delta;
		possible_size = tmp_size;
		enum_match_found = OK;
	    }
        }
    }

    // if we found a possible media size then update the passed in value
    if( enum_match_found == OK )
    {
        *media_size = possible_size;
    }

    return enum_match_found;
}

error_type_t paper_get_media_size_enum( float width, float height, paper_size_units_t units, mediasize_t * media_size)
{
    return get_media_size_enum( width, height, units, media_size );
}

uint32_t WEAK get_num_elements_paper_common_pjl_table(void)
{
    return( sizeof(paper_common_pjl_table) / sizeof(paper_common_pjl_table_t) );
}

uint16_t paper_common_map_mediaSize_to_pjl_code( mediaSize )
{
	int i;
//	int num_elements = sizeof(paper_common_pjl_table) / sizeof(paper_common_pjl_table_t);
	int num_elements = get_num_elements_paper_common_pjl_table();

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
	int num_elements = get_num_elements_paper_common_pjl_table();

	for (i = 0; i < num_elements; i++ ) {
		if ( paper_common_pjl_table[i].mediasize == mediaSize ) {
			return paper_common_pjl_table[i].pjl_name;
		}
	}
	return 0; // not found.
}

const char * paper_get_pjl_name(mediasize_t mediaSize)
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_common_map_mediaSize_to_pjl_name( mediaSize );
	}
	return 0;
}




