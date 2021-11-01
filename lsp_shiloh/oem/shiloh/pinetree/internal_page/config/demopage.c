/******************************************************************************
 * Copyright (c) 2008-2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * Description: Print a demo page.  We do this by essentially
 * hooking a demo page file up as a pipe
 *
 */

/* use the rom manager or embedded data */
#define DEMO_PAGE_USE_ROM_MGR 0

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "memAPI.h"
#include "dprintf.h"
#include "logger.h"
//#include "datafs_api.h"
#include "file_to_pipe_api.h"
#include "internal_page_api.h"

//jeseo added
#include "report_manager.h"

//#define HAVE_INTERNAL_PAGE_MGR = 1
#define SHILOH_MENU_PAGE 1

#if SHILOH_MENU_PAGE
#else 
#if ( DEMO_PAGE_USE_ROM_MGR == 1 )
/* read demo page from rom */
#include "rommgrapi.h"
#include "rom_id_mapping.h"
#else
/* read demo page from ram */
#include "demo_page_data.h"
#endif

#endif //SHILOH_MENU_PAGE

error_type_t _demo_page_run( void );
static void _demo_page_done( char *path, void *context );
static error_type_t _demo_page_invoke_page( const char *name, void *context );


static bool _demo_page_is_printing;


#define DEMO_PAGE_FILENAME "demo_page.prn"


void demo_page_init(void)
{
    error_type_t e_res;

    #define DEMO_PAGE_IPAGE_NAME "demo"
    e_res = ipage_register_page( DEMO_PAGE_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _demo_page_invoke_page,
                                 NULL );
    ASSERT( OK == e_res );

    _demo_page_is_printing = false;
}
#if SHILOH_MENU_PAGE
#else 
static error_type_t _add_datafile(const char* path, void* data, uint32_t length)
{
  int r = datafs_add_file(path, data, length, 0);
  if (r) return FAIL;
  return OK;
}

#endif

/** 
 * \brief Print the demo page.
 * This routine unpacks the demo page from rom, and then informs the connection
 * manager that something is ready to print via file_to_pipe()

 * 
 */
error_type_t _demo_page_run( void )
{
    error_type_t e_res;
    //int8_t *demo_page_data;
    //uint32_t demo_page_size;

    e_res = OK;

    /* we are currently doing a demo page, ignore */
    /* TODO ADD mutex protection */
    if( true == _demo_page_is_printing )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        /* now we are doing a demo page. */
        _demo_page_is_printing = true;
#if SHILOH_MENU_PAGE
#else 
#if ( DEMO_PAGE_USE_ROM_MGR == 1 )
        /* read the demo page from ROM to ram */
        demo_page_data = RomMgrReadData( IM_DATA_SECTOR,
                                         (uint32_t)ROMMGR_DEMO_PAGE,
                                         &demo_page_size );
        if(demo_page_data == NULL)
        {
            e_res = FAIL;
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("Error no demo page in the rom\n"));
        }
#else
        demo_page_data = (int8_t *)demo_page_prn;
        demo_page_size = demo_page_prn_len;
#endif
#endif //SHILOH_MENU_PAGE

    }


    if ( OK == e_res )
    {
        char my_full_filename[512];

#if SHILOH_MENU_PAGE
        strcpy(my_full_filename, "/var/report.prn");
        network_report();
        file_to_pipe( my_full_filename,
                      _demo_page_done,
                      NULL, e_DEMOPAGE);
#else 
        uint32_t filename_index;
        /* This adds the demo page file from memory to the filesystem */
        _add_datafile( DEMO_PAGE_FILENAME, demo_page_data, demo_page_size );

        strcpy( my_full_filename, "/" );
        filename_index = 1;
        strcat( &(my_full_filename[filename_index]), DATAFS_ROOT );
        filename_index += strlen( DATAFS_ROOT );
        strcat( &(my_full_filename[filename_index]), "/" );
        filename_index += 1; 
        strcat( &(my_full_filename[filename_index]), DEMO_PAGE_FILENAME );


        /* this starts the demo page printing
         * by connecting the demo page file to a pipe */
        file_to_pipe( my_full_filename,
                      _demo_page_done,
                      (void *)demo_page_data, e_DEMOPAGE );
#endif
    }

    return e_res;
}


/**
 *
 * \brief callback for notification when parser is done with demo page
 *
 */
static void _demo_page_done( char *path, void *context )
{
//    int i_res;

//    ASSERT( NULL != path );
//    ASSERT( NULL != context );

    /* delete the file from file system */
//    i_res = unlink( DEMO_PAGE_FILENAME );
//    ASSERT( 0 == i_res );

#if ( DEMO_PAGE_USE_ROM_MGR == 1 )
    /* context is rom mgr data, allocated */
    MEM_FREE_AND_NULL( context );
#endif

    _demo_page_is_printing = false;
}


static error_type_t _demo_page_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = _demo_page_run();

    return e_res;
}


