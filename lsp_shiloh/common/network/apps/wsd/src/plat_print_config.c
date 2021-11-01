
/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_wsd_print_config.c
 *
 * \brief This file implements the public configuration API for
 * WSD Print.
 * 
 **/

//--------------------------------------
// Included files
//--------------------------------------
#include "plat_print_config.h"

//--------------------------------------
// Local Definitions and Types
//--------------------------------------

/************** IMPORTANT NOTE **************************************/
// Values for this file must match the syntax of WSDPrintService.doc
/*******************************************************************/

// The following value is used if the oid
// lookup fails
#define DEFAULT_SUPPLY_LEVEL 100

// What type of consumable is being used
// Consumable Types are defined in WSDPrintService.doc on page 21
// Common values: Toner, Ink, Opc, WasteToner, Developer
#define DEFAULT_SUPPLY_TYPE  "Toner"

#define TRAY_CAPACITY       250
#define TRAY_LEVEL          100

#define COLLATION           false
#define JOG_OFFSET          false
#define DUPLEXER            false
#define STAPLER             false
#define HOLE_PUNCH          false

#define COMPRESSION_OPTION  "None"
#define FORMAT_ALLOWED      "unknown"
#define MEDIA_COLOR         "unknown"
#define DIRECTION           "RightDown"
#define PAGES_PER_SHEET     1
#define ORIENTATION_PORTRAIT    "Portrait"
#define ORIENTATION_LANDSCAPE   "Landscape"
#define PRINT_QUALITY       "Normal"
#define RESOLUTION_WIDTH    600
#define RESOLUTION_HEIGHT   600
#define ONE_SIDED           "OneSided"
#define MAX_COPIES          999
#define MIN_COPIES          1
#define MAX_PRIORITY        100
#define MIN_PRIORITY        1

#define PPM_MONO            15
#define PPM_COLOR           1

//--------------------------------------
// Local Function Definitions
//--------------------------------------
void plat_print_config( plat_print_config_t *config )
{
    /* The first index in any of the arrays below is the default value for that field */
	config->collation_supported     = COLLATION;
    config->jog_offset_supported    = JOG_OFFSET;
	config->duplexer_supported      = DUPLEXER;
	config->stapler_supported       = STAPLER;
	config->hole_punch_supported    = HOLE_PUNCH;
	config->num_compressions        = NUM_COMPRESSIONS;
	config->compressions[ 0 ]       = COMPRESSION_OPTION;
	config->num_formats             = NUM_FORMATS;
	config->formats[ 0 ]            = FORMAT_ALLOWED;
    config->num_media_colors        = NUM_MEDIA_COLORS;
    config->media_colors[ 0 ]       = MEDIA_COLOR;
    config->num_directions          = NUM_DIRECTIONS;
    config->nup_direction[ 0 ]      = NUP_RIGHT_THEN_DOWN;
    config->directions[ 0 ]         = DIRECTION;
    config->num_orientations        = NUM_ORIENTATIONS;
    config->orientations[ 0 ]       = ORIENTATION_PORTRAIT;
    config->orientations[ 1 ]       = ORIENTATION_LANDSCAPE;
    config->num_print_quality       = NUM_PRINT_QUALITY;
    config->print_quality[ 0 ]      = PRINT_QUALITY;
    config->num_resolutions         = NUM_RESOLUTIONS;
    config->resolutions[ 0 ].height = RESOLUTION_HEIGHT;
    config->resolutions[ 0 ].width  = RESOLUTION_WIDTH;
    config->num_sides               = NUM_SIDES;
    config->sides[ 0 ]              = ONE_SIDED;
    config->min_copies              = MIN_COPIES;
    config->max_copies              = MAX_COPIES;
    config->min_priority            = MIN_PRIORITY;
    config->max_priority            = MAX_PRIORITY;
    config->feed_direction          = FEED_DIRECTION_SHORT_EDGE_FIRST;
    config->ppm_mono                = PPM_MONO;
    config->ppm_color               = PPM_COLOR;
}

char *plat_config_supply_type( void )
{
    return DEFAULT_SUPPLY_TYPE;
}

uint32_t plat_config_default_supply_level( void ) 
{
    return DEFAULT_SUPPLY_LEVEL;
}

uint32_t plat_config_tray_capacity( void )
{
    return TRAY_CAPACITY;
}

uint32_t plat_config_tray_level( void )
{
    return TRAY_LEVEL;
}



