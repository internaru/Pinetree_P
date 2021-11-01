/*
*
* ============================================================================
* Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved
*
*                         Marvell Confidential
* ============================================================================
*
* Description:
*
**/

#include <stdint.h>
#include "error_types.h"
#include "paper.h"
#include "paper_size_config.h"
#include "paper_types_config.h"
#include "string.h"
#include "agprint.h" // printer_setup_t


static mediasize_t engine_media_sizes_table[] =
{    
    MEDIASIZE_LETTER,
    MEDIASIZE_ANY,  
    MEDIASIZE_LEGAL,
    MEDIASIZE_STATEMENT,
    MEDIASIZE_EXECUTIVE,
    MEDIASIZE_CUSTOM_85x13,   // gov_legal in many countries.
    MEDIASIZE_A3,           
    MEDIASIZE_11X17,           
    MEDIASIZE_A4,           
    MEDIASIZE_A5,           
    MEDIASIZE_A6,                      
    MEDIASIZE_B5,           
    MEDIASIZE_B6_JIS,           

    MEDIASIZE_ENV_9,                    
    MEDIASIZE_ENV_10,                  
    MEDIASIZE_ENV_MONARCH,             
    MEDIASIZE_ENV_C5,                  
    MEDIASIZE_ENV_DL,                  
    MEDIASIZE_ENV_B5,                  

    MEDIASIZE_JAPANESE_POSTCARD,       
    MEDIASIZE_DBL_JAPANESE_POSTCARD,   

    MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED,
    MEDIASIZE_A5_ROTATED,
    MEDIASIZE_A4_ROTATED,

    MEDIASIZE_CUSTOM_8x10,       // english quarto? 8R photo paper?   
    MEDIASIZE_CUSTOM_8x105,      // government letter, US gov. switched to letter.         
    MEDIASIZE_CUSTOM_825x14,     // government legal, what is this can it be bought?           
    MEDIASIZE_ENV_C6,                     

//    MEDIASIZE_CUSTOM_16K,            
//    MEDIASIZE_CUSTOM_16K_184_260,      
//    MEDIASIZE_CUSTOM_16K_195_270,      
//    MEDIASIZE_CUSTOM_4x6,              
//    MEDIASIZE_CUSTOM_5x8,              

       
    MEDIASIZE_USER_DEFINED            
};
#define NUM_ENTRIES_MEDIA_SIZE sizeof(engine_media_sizes_table)/sizeof(mediasize_t)


mediasize_t * get_engine_media_sizes_table( ) 
{
	return engine_media_sizes_table ;
}

//
// This table defines the supported media types.
//
static engine_media_types_t gEngMediaTypes[] =
{    // media type enum
    {MEDIATYPE_ANY},

    {MEDIATYPE_PLAIN_PAPER},
    {MEDIATYPE_HEAVY1},
    {MEDIATYPE_EXTRA_HEAVY},
    {MEDIATYPE_LIGHTPAPER},
    {MEDIATYPE_TRANSPARENCY},
    {MEDIATYPE_LABEL},
    {MEDIATYPE_HEAVY_GLOSSY},

    {MEDIATYPE_ENVELOPE},
    {MEDIATYPE_CARDSTOCK},
    {MEDIATYPE_CARD_GLOSSY},
    {MEDIATYPE_RECYCLED},
    {MEDIATYPE_LETTERHEAD},

    {MEDIATYPE_UNKNOWN}
};
#define NUM_ENTRIES_MEDIA_TYPE sizeof(gEngMediaTypes)/sizeof(engine_media_types_t)


/**
 * \brief Return the number of medias we have in our table
 * \return uint32_t
 * \retval Number of media sizes
 */
uint32_t get_num_media_sizes(void)
{

    return NUM_ENTRIES_MEDIA_SIZE;
}
/**
 * \brief Given an offset into the table, Value, return the mediaSize corresponding to that value
 * \param[in] Value The offset into the papersize table
 * \param[out] mediaSize The returned mediasize value
 * \returns error_type_t
 * \retval OK mediaSize is valid
 * \retval FAIL offset is out of range of the table, mediasize is invalid.
 */
error_type_t map_mediasize(int Value, mediasize_t *mediaSize)
{

    if(Value >= NUM_ENTRIES_MEDIA_SIZE)
        return FAIL;        // bad, return
    *mediaSize = engine_media_sizes_table[Value];
    return OK;
}


/**
 * \brief Return the number of media types this engine supports
 * \return uint32_t
 * \retval Number of media types
 */
uint32_t get_num_media_types(void)
{
    return NUM_ENTRIES_MEDIA_TYPE;
}

/**
 * \brief Given an offset into the table, Value, return the mediaType corresponding to that value
 * \param[in] Value The offset into the papertype table
 * \param[out] mediaType The returned papertype value
 * \returns error_type_t
 * \retval OK mediaType is valid
 * \retval FAIL offset is out of range of the table, mediatype is invalid.
 */
error_type_t map_mediatype(int Value, mediatype_t *mediaType)
{

    if(Value >= NUM_ENTRIES_MEDIA_TYPE)
        return FAIL;        // bad, return
    *mediaType = gEngMediaTypes[Value].mediaType;
    return OK;
}

/// Shiloh todo: adjust which trays can have what paper sizes paper types loaded,
///
#define MANUAL_ONLY_SIZES MEDIASIZE_ENV_9, MEDIASIZE_ENV_10,\
	MEDIASIZE_ENV_C5, MEDIASIZE_ENV_C6, MEDIASIZE_ENV_DL,\
	MEDIASIZE_ENV_B5, MEDIASIZE_ENV_MONARCH, \
	MEDIASIZE_JAPANESE_POSTCARD, MEDIASIZE_DBL_JAPANESE_POSTCARD,\
	MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED, MEDIASIZE_A5_ROTATED           
#define NUM_MANUAL_ONLY_SIZES 11

#define MANUAL_ONLY_TYPES MEDIATYPE_ENVELOPE, MEDIATYPE_CARDSTOCK, MEDIATYPE_CARD_GLOSSY  
#define NUM_MANUAL_ONLY_TYPES 3

#define BIG_TRAY_SIZES MEDIASIZE_LETTER, MEDIASIZE_A4, MEDIASIZE_EXECUTIVE, \
	MEDIASIZE_LETTER, MEDIASIZE_LEGAL,\
	MEDIASIZE_STATEMENT, MEDIASIZE_EXECUTIVE, MEDIASIZE_CUSTOM_85x13,\
	MEDIASIZE_A4, MEDIASIZE_A5, MEDIASIZE_A6,\
	MEDIASIZE_B5, MEDIASIZE_B6_JIS, MEDIASIZE_CUSTOM_8x10,\
	MEDIASIZE_CUSTOM_8x105, MEDIASIZE_CUSTOM_825x14, MEDIASIZE_UNKNOWN
#define NUM_BIG_TRAY_SIZES 17

#define BIG_TRAY_TYPES MEDIATYPE_ANY, MEDIATYPE_PLAIN_PAPER, MEDIATYPE_HEAVY1, MEDIATYPE_EXTRA_HEAVY, \
	MEDIATYPE_LIGHTPAPER, MEDIATYPE_TRANSPARENCY, MEDIATYPE_LABEL, MEDIATYPE_HEAVY_GLOSSY, MEDIATYPE_RECYCLED 
#define NUM_BIG_TRAY_TYPES 9

#define ALL_SIZES MANUAL_ONLY_SIZES , BIG_TRAY_SIZES
#define NUM_ALL_SIZES NUM_BIG_TRAY_SIZES + NUM_MANUAL_ONLY_SIZES

#define ALL_TYPES BIG_TRAY_TYPES , MANUAL_ONLY_TYPES 
#define NUM_ALL_TYPES NUM_BIG_TRAY_TYPES + NUM_MANUAL_ONLY_TYPES




printer_setup_t default_printer_setup = 
{
    .options = 0x0000000b,
    .inputTrays = 
    {
	{
	    .inputType = INPUT_MANUAL,
	    .properties = INPUTPROPERTY_PRIORITY,
	    .paperPresent = true,   // dynamic
	    .inputPresent = true,   // dynamic
	    .numSizes = NUM_ALL_SIZES,
	    .numTypes = NUM_ALL_TYPES,
	    .supportedSizes = { ALL_SIZES, 0 },
	    .supportedTypes = { ALL_TYPES, 0 }, 
	},
	{
	    .inputType = INPUT_MIDDLE,
	    .properties = INPUTPROPERTY_ALLPURPOSE,
	    .paperPresent = true,
	    .inputPresent = true,
	    .numSizes = NUM_BIG_TRAY_SIZES,
	    .numTypes = NUM_BIG_TRAY_TYPES,
	    .supportedSizes = { BIG_TRAY_SIZES, 0, },
	    .supportedTypes = { BIG_TRAY_TYPES, 0, },
	},
	{
	    .inputType = INPUT_LOWER,
	    .properties = INPUTPROPERTY_ALLPURPOSE,
	    .paperPresent = true,
	    .inputPresent = true,
	    .numSizes = NUM_BIG_TRAY_SIZES,
	    .numTypes = NUM_BIG_TRAY_TYPES,
	    .supportedSizes = { BIG_TRAY_SIZES, 0, },
	    .supportedTypes = { BIG_TRAY_TYPES, 0, },
	},
	{
		.inputType = INPUT_LOWER,
	    .properties = INPUTPROPERTY_ALLPURPOSE,
	    .paperPresent = true,
	    .inputPresent = true,
	    .numSizes = NUM_BIG_TRAY_SIZES,
	    .numTypes = NUM_BIG_TRAY_TYPES,
	    .supportedSizes = { BIG_TRAY_SIZES, 0, },
	    .supportedTypes = { BIG_TRAY_TYPES, 0, },
	},
    },
    .outputTrays = 
    {
	{ 
	    .outputType = OUTPUT_AUTO,
	    .properties = 0,
	    .maxSheets = 100,
	    .isFull = false,      // dynamic 
	    .outputPresent = true,
	},
	{0},
	{0},
	{0},
	{0},
    },
};

/// NON-standard PJL paper variable overrides.

paper_common_pjl_table_t paper_common_pjl_table[] = {
	{MEDIASIZE_ANY, 0, "ANY"}, /// < help 
		// 1, "unknown envelope"
	{MEDIASIZE_LETTER, 2, "LETTER"},
	{MEDIASIZE_LEGAL, 3, "LEGAL"},
	{MEDIASIZE_A4, 4, "A4"},
	{MEDIASIZE_EXECUTIVE, 5, "EXECUTIVE"},
		// 6, "LEDGER"
		// 7, "A3"
	{MEDIASIZE_ENV_10, 8, "COM10_ENV"},
	{MEDIASIZE_ENV_MONARCH, 9, "MONARCH_ENV"},
	{MEDIASIZE_ENV_C5, 10, "C5_ENV"},// from C5	@@ 2013.10.16		by Juny
	{MEDIASIZE_ENV_DL, 11, "DL_ENV"},// from DL	@@ 2013.10.16		by Juny
		// 12, "JIS B4"
		// 13, "JIS B5"
	{MEDIASIZE_ENV_B5, 14, "B5"},
	{MEDIASIZE_USER_DEFINED, 15, "CUSTOMSIZE"},
	{MEDIASIZE_JAPANESE_POSTCARD, 16, "JPNPOSTCARD"},
	{MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED, 17, "JPNDBLPOSTCARD"},
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
	{MEDIASIZE_ENV_9, 27, "COM9_ENV"},
	{MEDIASIZE_CUSTOM_825x14, 28, "G_LG"},
	{MEDIASIZE_CUSTOM_8x105, 29, "G_LT"},
	{MEDIASIZE_CUSTOM_85x13, 30, "85X13"},
	{MEDIASIZE_CUSTOM_8x10, 31, "ENGQUATRO"},
	{MEDIASIZE_ENV_C6, 32, "C6"},
	{MEDIASIZE_DBL_JAPANESE_POSTCARD, 33, "JPNDBLPOSTCARD_PORT"},

};

uint32_t get_num_elements_paper_common_pjl_table(void)
{
    return( sizeof(paper_common_pjl_table) / sizeof(paper_common_pjl_table_t) );
}
