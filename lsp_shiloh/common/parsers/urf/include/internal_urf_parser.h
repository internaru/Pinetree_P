/*
 *
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**

  \file internal_urf_parser.h

  \brief Job and Page header information for internal URF jobs
   For internal pages that are rendered using CUPS2.0 raster format, this file
   will define the job and page headers that are expected by the parser.
 */


#ifndef INC_INTERNAL_URF_PARSER_H
#define INC_INTERNAL_URF_PARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Job Header information: 
 * The parser match string for this internal urf is: InTl 
 * Following the parser identify the job should include a 4 byte value 
 * for number of pages in the job, followed by a 32 byte value string
 * which will be mapped to document type.
 *  
 * For example: 
 *          I n T l   1 page  c l e a  n i n g    p a g  e 
 *  In HEX: 496E546C 00000001 636c6561 6e696e67 20706167 65000000
 *          00000000 00000000 00000000 00000000 [PAGE HEADER] [PAGE
 *          DATA] [PAGE HEADER] [PAGE DATA]
 */


/**
 * Each URF page should have the following information as a header to 
 * the page. 
 */
typedef struct internal_urf_page_s
{
    uint8_t bits_per_pixel; ///< 8, 24
    uint8_t color_space;   ///< 0 == luminace gamma=2.2, 1 == sRGB
    uint8_t duplex_mode;  ///< auto, 1, short, long 
    uint8_t copies;
    uint32_t width;
    uint32_t height;
    uint32_t resolution; // pixels per inch X and Y 
} internal_urf_page_t;



#ifdef __cplusplus
}
#endif

#endif // INC_INTERNAL_URF_PARSER_H

