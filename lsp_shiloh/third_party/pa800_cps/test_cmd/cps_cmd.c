/*
 * ============================================================================
 * Copyright (c) 2014   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "lassert.h"
#include "cmd_proc_api.h"
#ifndef __linux__
#include "datafs_api.h"
#endif
#include "posix_ostools.h"
#include "memAPI.h"

#include "PrinterHostInterface.h"

//Workaround for similar header file Utils.h from CRI
#ifdef UTILS_H
#undef UTILS_H
#endif

#include "utils.h"

#ifdef __linux__
#define CLUSTERFILE "/"CLUSTERFILEPATH
#else
#define CLUSTERFILE "/"DATAFS_ROOT"/"CLUSTERFILEPATH
#endif

static const char *cps_cmd_init_desc = "Initialize the CPS system";
static const char *cps_cmd_init_usage = "";

static const char *cps_cmd_shutdown_desc = "Shutdown the CPS system";
static const char *cps_cmd_shutdown_usage = "";

static const char *cps_cmd_insert_desc = "Insert a cartridge";
static const char *cps_cmd_insert_usage = "<index> <color> <dots Per Quanta>";
static const char *cps_cmd_insert_notes = "index - A value from 0-3 that indicates the location of the cartridge in the printer \n \
                                           color - Enumerated value that indicates the printer cartridge color \n \
                                           dotsPerQuanta - The number of printed dots that maps to 1 quanta in the CCF";

static const char *cps_cmd_remove_desc = "Remove a cartridge";
static const char *cps_cmd_remove_usage = "<index>";
static const char *cps_cmd_remove_notes = "index - A value from 0-3 that indicates the location of the cartridge in the printer";

static const char *cps_cmd_print_desc = "Perform authentication actions based on a print request";
static const char *cps_cmd_print_usage = "<pageCount> <usableArea> <dpi> <pcnt_cyan> <pcnt_magenta> <pcnt_yellow> <pcnt_black>";
static const char *cps_cmd_print_notes = "pageCount - Number of pages being printed \n \
                                          usableArea - Total area that can be printed on expressed in square inches \n \
                                          pcnt_cyan - percentage coverage for color CYAN \n \
                                          pcnt_magenta - percentage coverage for color MAGENTA \n \
                                          pcnt_yellow - percentage coverage for color YELLOW \n \
                                          pcnt_black - percentage coverage for color BLACK \n \
                                            *A value of -1 for any of the members will force a non-decrementing \n \
                                             challenge for the cartridge that maps to that color";

static const char *cps_cmd_writemem_desc = "Write data to user area of cartridge NVM";
static const char *cps_cmd_writemem_usage = "<index> <address> <length> <data (hex)> ";
static const char *cps_cmd_writemem_notes = "index - A value from 0-3 that indicates the location of the cartridge in the printer" \
                                            "address - Memory address to write to" \
                                            "length - Length of data to be written" \
                                            "data - data to be written (in hex ex: 0x01 0x02 ...)";

static const char *cps_cmd_readmem_desc = "Read data from the cartridge NVM";
static const char *cps_cmd_readmem_usage = "<index> <address> <length>";
static const char *cps_cmd_readmem_notes = "index - A value from 0-3 that indicates the location of the cartridge in the printer" \
                                            "address - Memory address to read from" \
                                            "length - Length of data to read";

static const char *cps_cmd_getdevinfo_desc = "Gets device information for the specified cartridge";
static const char *cps_cmd_getdevinfo_usage = "<index>";
static const char *cps_cmd_getdevinfo_notes = "index - A value from 0-3 that indicates the location of the cartridge in the printer";

static int cps_cmd_init_cb( int argc, char *argv[] );
static int cps_cmd_shutdown_cb( int argc, char *argv[] );
static int cps_cmd_insert_cb( int argc, char *argv[] );
static int cps_cmd_remove_cb( int argc, char *argv[] );
static int cps_cmd_print_cb( int argc, char *argv[] );
static int cps_cmd_writemem_cb( int argc, char *argv[] );
static int cps_cmd_readmem_cb( int argc, char *argv[] );
static int cps_cmd_getdevinfo_cb( int argc, char *argv[] );

static int shutdown_complete_flag = 0;

void cps_cmd_init ( void )
{
    int i_res;

    cmd_printf( " Starting CPS \n" );
    /* register logger commands */
    i_res = cmd_register_cmd( "cps",
                              NULL,
                              NULL,
                              NULL,
                              NULL,
                              NULL );
    ASSERT( i_res == CMD_OK );

    i_res = cmd_register_subcmd( "cps",
                                 "init",
                                 cps_cmd_init_desc,
                                 cps_cmd_init_usage,
                                 NULL,
                                 cps_cmd_init_cb );
    ASSERT( CMD_OK == i_res );

    i_res = cmd_register_subcmd( "cps",
                                 "shutdown",
                                 cps_cmd_shutdown_desc,
                                 cps_cmd_shutdown_usage,
                                 NULL,
                                 cps_cmd_shutdown_cb );
    ASSERT( CMD_OK == i_res );

    i_res = cmd_register_subcmd( "cps",
                                 "insert",
                                 cps_cmd_insert_desc,
                                 cps_cmd_insert_usage,
                                 cps_cmd_insert_notes,
                                 cps_cmd_insert_cb );
    ASSERT( CMD_OK == i_res );

    i_res = cmd_register_subcmd( "cps",
                                 "remove",
                                 cps_cmd_remove_desc,
                                 cps_cmd_remove_usage,
                                 cps_cmd_remove_notes,
                                 cps_cmd_remove_cb );
    ASSERT( CMD_OK == i_res );

    i_res = cmd_register_subcmd( "cps",
                                 "print",
                                 cps_cmd_print_desc,
                                 cps_cmd_print_usage,
                                 cps_cmd_print_notes,
                                 cps_cmd_print_cb );
    ASSERT( CMD_OK == i_res );

    i_res = cmd_register_subcmd( "cps",
                                 "writemem",
                                 cps_cmd_writemem_desc,
                                 cps_cmd_writemem_usage,
                                 cps_cmd_writemem_notes,
                                 cps_cmd_writemem_cb );
    ASSERT( CMD_OK == i_res );

    i_res = cmd_register_subcmd( "cps",
                                 "readmem",
                                 cps_cmd_readmem_desc,
                                 cps_cmd_readmem_usage,
                                 cps_cmd_readmem_notes,
                                 cps_cmd_readmem_cb );
    ASSERT( CMD_OK == i_res );

    i_res = cmd_register_subcmd( "cps",
                                 "getdevinfo",
                                 cps_cmd_getdevinfo_desc,
                                 cps_cmd_getdevinfo_usage,
                                 cps_cmd_getdevinfo_notes,
                                 cps_cmd_getdevinfo_cb );
    ASSERT( CMD_OK == i_res );

}


// Test commands
static int cps_cmd_init_cb( int argc, char *argv[] )
{
    REF_STATUS_t status;

    cmd_printf( " using cluster file @ %s \n", CLUSTERFILE );

    status = CPS_Init( CLUSTERFILE );
    if ( REF_SUCCESS != status )
    {
        cmd_printf( " Failed to init CPS. (%d) \n ", status );
    }

    return CMD_OK;
}

static void cps_shutdown_thread ( void * args )
{
    REF_STATUS_t status;

    /* Shutdown needs to be called in a posix thread.
     * Call shutdown (will block) and just set the mutex.
     */
    status = CPS_Shutdown();
    if ( REF_SUCCESS != status )
    {
        cmd_printf( " Failed to shutdown CPS. (%d) \n ", status );
    }

    shutdown_complete_flag = 1;
}

static pthread_t cps_shutdown_thread_id;

static int cps_cmd_shutdown_cb( int argc, char *argv[] )
{
    shutdown_complete_flag = 0;

    posix_create_thread( &cps_shutdown_thread_id,
                         (void*) cps_shutdown_thread,
                         0,
                         "cps shutdown",
                         NULL,
                         0,
                         POSIX_THR_PRI_NORMAL);

    while ( !shutdown_complete_flag )
    {
        posix_sleep_ms(10);
    }


    return CMD_OK;
}

static int cps_cmd_insert_cb( int argc, char *argv[] )
{
    REF_STATUS_t status;
    uint32_t ccfIndex;
    uint32_t parm;
    CPS_CARTRIDGE_COLOR_t color;
    uint32_t dotsPerQuanta;

    if ( argc != 4 )
    {
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint( argv[1], &ccfIndex ) != 0 )
    {
        cmd_printf( "Invalid ccfIndex \n" );

        return CMD_USAGE_ERROR;
    }

    if ( ccfIndex > 3 )
    {
        cmd_printf( "ccfIndex should be 0 - 3 \n" );
    }

    if ( str_mkint( argv[2], &parm ) != 0 )
    {
        cmd_printf( "Invalid color \n" );

        return CMD_USAGE_ERROR;
    }

    color = ( CPS_CARTRIDGE_COLOR_t ) parm;

    if ( color >= COLOR_INVALID )
    {
        cmd_printf( "Invalid color \n" );
    }

    if ( str_mkint(argv[3], &dotsPerQuanta ) != 0 )
    {
        cmd_printf( "Invalid dotsPerQuanta \n" );

        return CMD_USAGE_ERROR;
    }

    cmd_printf( "Inserting cartridge: ccfIndex (%d), color (%d) , dotsPerQuanta (%d) \n",
                ccfIndex, color, dotsPerQuanta );

    status = CPS_InsertCartridge( ccfIndex, color, dotsPerQuanta );
    if ( REF_SUCCESS != status )
    {
        cmd_printf( " Failed to insert cartridge. (%d) \n ", status );
    }

    return CMD_OK;
}

static int cps_cmd_remove_cb( int argc, char *argv[] )
{
    REF_STATUS_t status;
    uint32_t ccfIndex;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[1], &ccfIndex ) != 0 )
    {
        cmd_printf( "Invalid ccfIndex \n" );

        return CMD_USAGE_ERROR;
    }

    if ( ccfIndex > 3 )
    {
        cmd_printf( "ccfIndex should be 0 - 3 \n" );
    }

    cmd_printf( "Removing cartridge: ccfIndex (%d) \n", ccfIndex );
    status = CPS_RemoveCartridge( ccfIndex );
    if ( REF_SUCCESS != status )
    {
        cmd_printf( " Failed to remove cartridge. (%d) \n ", status );
    }

    return CMD_OK;
}

static int cps_cmd_print_cb( int argc, char *argv[] )
{
    REF_STATUS_t status;
    int32_t parm;
    uint32_t pageCount;
    CPS_PageConfiguration_t pgCfg;
    CPS_PageCoverage_t coverage;
    CPS_PrintResults_t printResults;

    if ( argc != 8 )
    {
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[1], &pageCount ) != 0 )
    {
        cmd_printf( "Invalid pageCount \n" );
        return CMD_USAGE_ERROR;
    }

    if ( str_mksint(argv[2], &parm ) != 0 )
    {
        cmd_printf( "Invalid usableArea \n" );
        return CMD_USAGE_ERROR;
    }
    else
    {
        pgCfg.usableArea = parm;
    }

    if ( str_mksint(argv[3], &parm ) != 0 )
    {
        cmd_printf( "Invalid dpi \n" );
        return CMD_USAGE_ERROR;
    }
    else
    {
        pgCfg.dpi = parm;
    }

    if ( str_mksint(argv[4], &parm ) != 0 )
    {
        cmd_printf( "Invalid pcnt_cyan \n" );
        return CMD_USAGE_ERROR;
    }
    else
    {
        coverage.cyan = parm;
    }

    if ( str_mksint(argv[5], &parm ) != 0 )
    {
        cmd_printf( "Invalid pcnt_magenta \n" );
        return CMD_USAGE_ERROR;
    }
    else
    {
        coverage.magenta = parm;
    }

    if ( str_mksint(argv[6], &parm ) != 0 )
    {
        cmd_printf( "Invalid pcnt_yellow \n" );
        return CMD_USAGE_ERROR;
    }
    else
    {
        coverage.yellow = parm;
    }

    if ( str_mksint(argv[7], &parm ) != 0 )
    {
        cmd_printf( "Invalid pcnt_black \n" );
        return CMD_USAGE_ERROR;
    }
    else
    {
        coverage.black = parm;
    }

    cmd_printf( "Printing page : pageCount (%d), dpi (%d), usableArea (%d), cyan (%d%%), magenta (%d%%), yellow (%d%%), black (%d%%) \n",
                pageCount,
                pgCfg.dpi,
                pgCfg.usableArea,
                coverage.cyan,
                coverage.magenta,
                coverage.yellow,
                coverage.black );

    status = CPS_Print( pageCount, &pgCfg, &coverage,  &printResults);
    if ( REF_SUCCESS != status )
    {
        cmd_printf( "Failed to Print. (%d) \n ", status );
    }
    else
    {
        cmd_printf( "Print results. cyan (%d), magenta (%d), yellow (%d), black (%d) \n ",
                     printResults.cyan,
                     printResults.magenta,
                     printResults.yellow,
                     printResults.black );
    }

    return CMD_OK;
}

static int cps_cmd_writemem_cb( int argc, char *argv[] )
{
    REF_STATUS_t status;
    uint32_t index;
    uint32_t address;
    uint32_t length;
    uint8_t * data;
    int i;
    uint32_t hex_data;
    int rc = CMD_OK;

    if ( argc <= 5 )
    {
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[1], &index ) != 0 )
    {
        cmd_printf( "Invalid index \n" );
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[2], &address ) != 0 )
    {
        cmd_printf( "Invalid address \n" );
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[3], &length ) != 0 )
    {
        cmd_printf( "Invalid length \n" );
        return CMD_USAGE_ERROR;
    }

    data = MEM_MALLOC( length );
    ASSERT( NULL != data );

    for ( i = 0; i < length; i++ )
    {
        if ( str_mkint(argv[4 + i], &hex_data) != 0 )
        {
            cmd_printf( "Invalid hex data: %s \n", argv[3+i]);
            rc = CMD_USAGE_ERROR;
            break;
        }

        data[i] = (uint8_t) hex_data;
    }

    if ( CMD_OK == rc )
    {
        status = CPS_WriteMemory( index, address, length, data );
        if ( REF_SUCCESS != status )
        {
            rc = CMD_ERROR;
        }
    }

    MEM_FREE_AND_NULL( data );

    return rc;
}

static int cps_cmd_readmem_cb( int argc, char *argv[] )
{
    int rc = CMD_ERROR;
    REF_STATUS_t status;
    uint32_t index;
    uint32_t address;
    uint32_t length;
    uint8_t * data;
    int i;

    if ( argc < 4 )
    {
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[1], &index ) != 0 )
    {
        cmd_printf( "Invalid index \n" );
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[2], &address ) != 0 )
    {
        cmd_printf( "Invalid address \n" );
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[3], &length ) != 0 )
    {
        cmd_printf( "Invalid length \n" );
        return CMD_USAGE_ERROR;
    }

    data = MEM_MALLOC( length );
    ASSERT( NULL != data );

    status = CPS_ReadMemory( index, address, length, data );
    if ( REF_SUCCESS == status )
    {
        rc = CMD_OK;
    }

    cmd_printf( "Read data: " );
    for ( i = 0; i < length; i++ )
    {
        cmd_printf( "0x%02x ", data[i]);
    }
    cmd_printf( "\n" );

    MEM_FREE_AND_NULL( data );

    return rc;
}

static int cps_cmd_getdevinfo_cb( int argc, char *argv[] )
{
    REF_STATUS_t status;
    uint32_t ccfIndex;
    uint32_t deviceid;
    uint64_t num_auths;
    uint32_t lifecycle;
    uint32_t quanta_remaining;
    int rc = CMD_ERROR;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if ( str_mkint(argv[1], &ccfIndex ) != 0 )
    {
        cmd_printf( "Invalid ccfIndex \n" );

        return CMD_USAGE_ERROR;
    }

    if ( ccfIndex > 3 )
    {
        cmd_printf( "ccfIndex should be 0 - 3 \n" );
    }

    status = CPS_GetDeviceInfo( ccfIndex, &deviceid, &num_auths, &lifecycle, &quanta_remaining );
    if ( REF_SUCCESS == status )
    {
        cmd_printf( " Device Info: index (%d), deviceID (0x%x), numauths (%llu), lifecycle (%d), quanta remaining (%d) \n ",
                    ccfIndex,
                    deviceid,
                    num_auths,
                    lifecycle,
                    quanta_remaining );
        rc = CMD_OK;
    }

    return rc;
}

