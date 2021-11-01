/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file plat_print_config.h
 *
 * \brief This file implements public configuration options for 
 *        the printer.
 * 
 **/

#ifndef INC_PLAT_PRINT_CONFIG_H
#define INC_PLAT_PRINT_CONFIG_H

// Include files
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_COMPRESSIONS    1
#define NUM_FORMATS	        1
#define NUM_MEDIA_COLORS    1
#define NUM_DIRECTIONS      1
#define NUM_PAGES_PER_SHEET 1
#define NUM_ORIENTATIONS    2
#define NUM_PRINT_QUALITY   1
#define NUM_RESOLUTIONS     1
#define NUM_SIDES           1

//--------------------------------------
// API Declarations and Type Definitions
//--------------------------------------

typedef struct 
{
    uint32_t width;
    uint32_t height;
} plat_config_resolution_t;

//// Order must match enum wprt__NUpDirectionRestrictionType in wprtStub.h
typedef enum
{
    NUP_RIGHT_THEN_DOWN,
    NUP_DOWN_THEN_RIGHT,
    NUP_LEFT_THEN_DOWN,
    NUP_DOWN_THEN_LEFT,
} plat_config_nup_direction_t;

//// Order must match enum wprt__InputBinFeedDirectionBaseType in wprtStub.h
typedef enum
{
    FEED_DIRECTION_LONG_EDGE_FIRST,
    FEED_DIRECTION_SHORT_EDGE_FIRST,
} plat_config_feed_direction_t;

/**
 * @brief Plat Print Configuration structure
 * 
 * This structure contains the configuration options that apply
 * to general platform settings that can't be found elsewhere in the system.
 */
typedef struct plat_print_config
{
	bool		collation_supported;
    bool        jog_offset_supported;
	bool 		duplexer_supported;
	bool 		stapler_supported;
	bool 		hole_punch_supported;
	uint32_t	num_compressions;
	char 		*compressions[ NUM_COMPRESSIONS ];
	uint32_t	num_formats;
	char		*formats[ NUM_FORMATS ];
    uint32_t    num_media_colors;
    char 		*media_colors[ NUM_MEDIA_COLORS ];
    uint32_t    num_directions;
	char 		*directions[ NUM_DIRECTIONS ];      // Don't know why but WSD defines both strings and an enum for this field
    plat_config_nup_direction_t nup_direction[ NUM_DIRECTIONS ];
    uint32_t    num_pages_per_sheet;
	uint32_t	pages_per_sheet[ NUM_PAGES_PER_SHEET ];
	uint32_t	num_orientations;
	char		*orientations[ NUM_ORIENTATIONS ];
    uint32_t    num_print_quality;
	char 		*print_quality[ NUM_PRINT_QUALITY ];
    uint32_t    num_resolutions;
    plat_config_resolution_t resolutions[ NUM_RESOLUTIONS ]; 
	uint32_t	num_sides;
	char 		*sides[ NUM_SIDES ];
    uint32_t    min_copies;
    uint32_t    max_copies;
    uint32_t    min_priority;
    uint32_t    max_priority;
    plat_config_feed_direction_t feed_direction;
    uint32_t    ppm_mono;
    uint32_t    ppm_color;
} plat_print_config_t;

/**
 * @brief Get plat print config options
 * 
 * @param config buffer to fill with config data.  Memory 
 *               provided by caller
 */
void plat_print_config( plat_print_config_t *config );

/**
 * @brief Get the supply type string 
 * 
 * @return char* 
 */
char *plat_config_supply_type( void );

/**
 * @brief Get the default supply level for when oid lookups fail
 * 
 * @return uint32_t 
 */
uint32_t plat_config_default_supply_level( void ) ;

/**
 * 
 * @brief Get the tray capacity
 * 
 * @return uint32_t 
 */
uint32_t plat_config_tray_capacity( void );

/**
 * 
 * @brief Get the tray level
 * 
 * @return uint32_t 
 */
uint32_t plat_config_tray_level( void );


#ifdef __cplusplus
}
#endif //__cplusplus

#endif // INC_PLAT_PRINT_CONFIG_H
