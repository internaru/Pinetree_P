/*
 *
 * ============================================================================
 * Copyright (c) 2009-2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
*/  

#include "internal_page_api.h"
#include "paper_size_config.h"
#include "lassert.h"

#ifdef HAVE_FREETYPE
/* font renderer */
#include "freetype_renderer.h"
#endif

#define BLOCK_TOP 0
#define LEFT_BLOCK_X 400
#define RIGHT_BLOCK_X 2700
#define PAGE_BLOCK_Y 6600 + BLOCK_TOP

#if defined(INTERNAL_PAGE_USE_RGB)
#define IP_DEFAULT_RESOLUTION_X   600
#define IP_DEFAULT_RESOLUTION_Y   600
#define IP_DEFAULT_BPP          8
#define IP_DEFAULT_TYPE         IPAGE_RGB
#define IP_DEFAULT_PACKING      IPAGE_RGBX_PACK
#define IP_DEFAULT_STRIP_SIZE   100     // lines/strip
#define IP_DEFAULT_LEFT_MARGIN      LEFT_BLOCK_X
#define IP_DEFAULT_LEFT_2ND_MARGIN  RIGHT_BLOCK_X
#define IP_DEFAULT_PAGE_BLOCK_Y     PAGE_BLOCK_Y
#define IP_DEFAULT_BLOCK_TOP        BLOCK_TOP
#define IP_DEFAULT_PAPERSIZE    MEDIASIZE_LETTER
#elif defined(INTERNAL_PAGE_USE_CMYK)
/* Use CMYK */
#define IP_DEFAULT_RESOLUTION_X   1200
#define IP_DEFAULT_RESOLUTION_Y   1200
#define IP_DEFAULT_BPP          1
#define IP_DEFAULT_TYPE         IPAGE_CMYK
#define IP_DEFAULT_PACKING      IPAGE_PLANAR_ORDERED
#define IP_DEFAULT_STRIP_SIZE   100     // lines/strip
#define IP_DEFAULT_LEFT_MARGIN      LEFT_BLOCK_X*2
#define IP_DEFAULT_LEFT_2ND_MARGIN  RIGHT_BLOCK_X*2
#define IP_DEFAULT_PAGE_BLOCK_Y     PAGE_BLOCK_Y*2
#define IP_DEFAULT_BLOCK_TOP        BLOCK_TOP*2
#define IP_DEFAULT_PAPERSIZE    MEDIASIZE_LETTER
#else // mono
#define IP_DEFAULT_RESOLUTION_X   600
#define IP_DEFAULT_RESOLUTION_Y   600
#define IP_DEFAULT_BPP            2
#define IP_DEFAULT_TYPE           IPAGE_MONO
#define IP_DEFAULT_PACKING        IPAGE_PLANAR_ORDERED
#define IP_DEFAULT_STRIP_SIZE     100     // lines/strip
#define IP_DEFAULT_LEFT_MARGIN      LEFT_BLOCK_X*2
#define IP_DEFAULT_LEFT_2ND_MARGIN  RIGHT_BLOCK_X*2
#define IP_DEFAULT_PAGE_BLOCK_Y     PAGE_BLOCK_Y*2
#define IP_DEFAULT_BLOCK_TOP        BLOCK_TOP*2
#define IP_DEFAULT_PAPERSIZE    MEDIASIZE_LETTER
#endif

void ip_set_default_config(ipage_config_t *config)
{
    config->bpp = IP_DEFAULT_BPP;
    config->resolution_x = IP_DEFAULT_RESOLUTION_X;
    config->resolution_y = IP_DEFAULT_RESOLUTION_Y;
    config->strip_size = IP_DEFAULT_STRIP_SIZE;
    config->left_margin = IP_DEFAULT_LEFT_MARGIN;
    config->left_2nd_margin = IP_DEFAULT_LEFT_2ND_MARGIN;
    config->page_block_y = IP_DEFAULT_PAGE_BLOCK_Y;
    config->block_top = IP_DEFAULT_BLOCK_TOP;

    config->color_type = IP_DEFAULT_TYPE;
    config->packing = IP_DEFAULT_PACKING;
    config->orientation = IPAGE_PORTRAIT;
    #if defined(INTERNAL_PAGE_USE_RGB)
    config->color_order[0] = IPAGE_GREEN;
    config->color_order[1] = IPAGE_RED;
    config->color_order[2] = IPAGE_BLUE;
    #elif defined(INTERNAL_PAGE_USE_CMYK)
    /* Use CMYK */
    config->color_order[0] = IPAGE_CYAN;
    config->color_order[1] = IPAGE_MAGENTA;
    config->color_order[2] = IPAGE_YELLOW;
    config->color_order[3] = IPAGE_BLACK;
    config->color_order[4] = IPAGE_BLACK_PHOTO;
    config->color_order[5] = IPAGE_AUX;
    #else // mono
    config->color_order[0] = IPAGE_BLACK;
    #endif
    config->mediasize = IP_DEFAULT_PAPERSIZE;

#ifdef HAVE_FREETYPE
    // Set the font renderer to use for the internal pages.
    setObjectRenderer(e_Text, FreetypeTextRenderer); 
#endif
}


void  internal_pages_font_init(string_mgr_language_t language, uint32_t resolution_x, uint32_t resolution_y)
{
   /* Nothing to do here */
    //ASSERT( 0 ); /* TODO INITIALIZE THE LOWER LEVEL FONT RENDERER */
}

void internal_pages_font_halt(void)
{
   /* Nothing to do here */
    //ASSERT( 0 ); /* TODO DE-INITIALIZE THE LOWER LEVEL FONT RENDERER */
}


#include "demopage.h"
#include "config_page_api.h"
#include "report_manager.h"

void internal_page_config_init( void )
{
    /* initialize pages for here */
    //demo_page_init();
    report_manager_init();
    //demo_page_init();
    //config_page_init();
}


