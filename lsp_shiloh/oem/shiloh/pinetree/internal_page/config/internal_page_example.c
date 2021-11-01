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
 * \file internal_page_example.c 
 *
 * \brief This file contains an example of building an internal pages..
 *
 */

#include <stdint.h>
#include <math.h>
#include <string.h>
#include "agRouter.h"
#include "agConnectMgr.h"
#include "internal_page_api.h"
#include "lassert.h"
#include "mlimiter.h"

#define IP_CONFIG_RES_X 300
#define IP_CONFIG_RES_Y 300
#define IP_CONFIG_BLOCK_TOP 0
#define IP_CONFIG_LEFT_MARGIN_X 50
#define IP_CONFIG_LEFT_2ND_MARGIN_X 1350
#define IP_CONFIG_PAGE_BLOCK_Y 3300 + IP_CONFIG_BLOCK_TOP


static error_type_t _config_page_invoke( const char *name, void *context );


void config_page_init( void )
{
    error_type_t e_res;

    #define CONFIG_PAGE_NAME "config"
    e_res = ipage_register_page( CONFIG_PAGE_NAME,
                                 false, /* I don't start my own job */
                                 _config_page_invoke,
                                 NULL );
    ASSERT( OK == e_res );
}


/**\brief Routine to construct configuration page
 * 
 * \param[in/out] pConfPageDoc  The configuration page document to build
 * 
 */
void buildConfigPage(internal_page_doc_t *pConfPageDoc)
{
    /* CMYK test */
    // Put down some example lines
//    ipage_build_line( pConfPageDoc, 100, 25, 4600, 25, 50, COLOR_C_OR_G);
    ipage_build_line( pConfPageDoc, 100, 325, 4600, 325, 50, COLOR_K_OR_R);
    ipage_build_line( pConfPageDoc, 100, 625, 4600, 625, 50, COLOR_K_OR_R);
    ipage_build_line(pConfPageDoc, 20,20,4000,20,2,COLOR_K_OR_R);
    ipage_build_line( pConfPageDoc, 100, 1225, 4600, 1225, 5, COLOR_K_OR_R);
    ipage_build_line( pConfPageDoc, 100, 1525, 4600, 1525, 5, COLOR_K_OR_R);
  //    ipage_build_line(pConfPageDoc,4, 4, 300, 625, 4, COLOR_C_OR_G);

    ipage_build_line( pConfPageDoc, 0, 0, 4000, 4000, 1, COLOR_K_OR_R);

    ipage_build_line( pConfPageDoc, 4, 50, 300, 1625, 1, COLOR_K_OR_R);
    ipage_build_line(pConfPageDoc, 10,3,100,3000,1,COLOR_K_OR_R);
    ipage_build_line(pConfPageDoc, 5,30,2800,300,2,COLOR_K_OR_R);
    ipage_build_line(pConfPageDoc, 20,20,3000,4500,3, COLOR_K_OR_R);
  //  ipage_build_line(pConfPageDoc, 20,20,4000,20,1,COLOR_K_OR_R);
    ipage_build_line( pConfPageDoc, 100, 1225, 4600, 1225, 50, COLOR_K_OR_R);
    ipage_build_line( pConfPageDoc, 100, 1525, 4600, 1525, 50, COLOR_K_OR_R);
}


/** \brief Routine to build and print configuration page
 * 
 * \param[in] Copies  # of copies
 * \param[in] input Tray configuration
 */
static void RunRendererConfigPage( uint32_t Copies, MODULE_ID Dest )
{
    internal_page_doc_t *pConfigPage;

    pConfigPage = ipage_create_page(e_CONFIGPAGE);   
    ASSERT( pConfigPage );
    mlimiter_start(mlimiter_by_name("print"));

    // override default internal pages left margin
    ipage_set_left_margin(pConfigPage, 0);

    buildConfigPage(pConfigPage);
    
    ipage_print_page( pConfigPage, Copies, Dest );
    
    ipage_delete_page(pConfigPage);
    mlimiter_stop(mlimiter_by_name("print"));
}

                        
static error_type_t _config_page_invoke( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    /* for now hardcode destination, we will need to fix this up later */
    RunRendererConfigPage( 1, JOBMGRID );

    return e_res;
}
