/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *
 * \file calibration_page_example.c 
 *
 * \brief This file contains an example of building a calibration page..
 *
 */

#include <stdint.h>
#include "agRouter.h"
#include "agConnectMgr.h"
#include "internal_page_api.h"
#include "lassert.h"

#define IP_CAL_RES_X 1200
#define IP_CAL_RES_Y 1200
#define IP_CAL_BLOCK_TOP 0
#define IP_CAL_LEFT_MARGIN_X 200
#define IP_CAL_LEFT_2ND_MARGIN_X 5400
#define IP_CAL_PAGE_BLOCK_Y 13200 + IP_CAL_BLOCK_TOP

/**\brief Routine to construct calibration page
 * 
 * \param[in/out] pCalPageDoc  The calibration page document to build
 * 
 */
void buildCalPage(internal_page_doc_t *pCalPageDoc)
{
    /* CMYK test */
    // Put down some example lines
    ipage_build_line( pCalPageDoc, 100, 25, 9200, 25, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 325, 9200, 325, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 625, 9200, 625, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 925, 9200, 925, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 1225, 9200, 1225, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 9625, 9200, 9625, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 9925, 9200, 9925, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 10225, 9200, 10225, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 10525, 9200, 10525, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 1525, 9200, 1525, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 1825, 9200, 1825, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 2125, 9200, 2125, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 2425, 9200, 2425, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 2725, 9200, 2725, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 3025, 9200, 3025, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 3325, 9200, 3325, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 3625, 9200, 3625, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 3925, 9200, 3925, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 4225, 9200, 4225, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 4525, 9200, 4525, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 4825, 9200, 4825, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 5125, 9200, 5125, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 5425, 9200, 5425, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 5725, 9200, 5725, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 6025, 9200, 6025, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 6325, 9200, 6325, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 6625, 9200, 6625, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 6925, 9200, 6925, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 7225, 9200, 7225, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 7525, 9200, 7525, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 7825, 9200, 7825, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 8125, 9200, 8125, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 8425, 9200, 8425, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 8725, 9200, 8725, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 9025, 9200, 9025, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 9325, 9200, 9325, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 9625, 9200, 9625, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 9925, 9200, 9925, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 10225, 9200, 10225, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 10525, 9200, 10525, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 10825, 9200, 10825, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 11125, 9200, 11125, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 11425, 9200, 11425, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 11725, 9200, 11725, 50, COLOR_K_OR_R);
    ipage_build_line( pCalPageDoc, 100, 12025, 9200, 12025, 50, COLOR_C_OR_G);
    ipage_build_line( pCalPageDoc, 100, 12325, 9200, 12325, 50, COLOR_M);
    ipage_build_line( pCalPageDoc, 100, 12625, 9200, 12625, 50, COLOR_Y_OR_B);
    ipage_build_line( pCalPageDoc, 100, 12925, 9200, 12925, 50, COLOR_K_OR_R);
}


/** \brief Routine to build and print calibration page
 * 
 * \param[in] Copies  # of copies
 * \param[in] input Tray configuration
 */
void RunRendererCalPage( uint32_t Copies, MODULE_ID Dest )
{
    internal_page_doc_t *pCalPage;
    ipage_config_t config;

    pCalPage = ipage_create_page(e_CALPAGE);   
    ASSERT( pCalPage );

    // set the resolution to 1200x1200 dpi
    ipage_get_config(pCalPage, &config);
    config.resolution_x = IP_CAL_RES_X;
    config.resolution_y = IP_CAL_RES_Y;
    config.left_margin = IP_CAL_LEFT_MARGIN_X;
    config.left_2nd_margin = IP_CAL_LEFT_2ND_MARGIN_X;
    config.page_block_y = IP_CAL_PAGE_BLOCK_Y;
    config.block_top = IP_CAL_BLOCK_TOP;
    ipage_set_config(pCalPage, &config);

    buildCalPage(pCalPage);
    
    ipage_print_page( pCalPage, Copies, Dest );
    
    ipage_delete_page(pCalPage);
}

                        
