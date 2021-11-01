/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file mkdevscan.c
 *
 * \brief Create /dev/scanman with a lot of sanity checks.
 *
 * Reads /proc/devices looking for scanman. Creates /dev/scanman with the major
 * id if it doesn't already exist.
 *
 * davep 06-Sep-2012
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#define DEBUG 1

#include "utils.h"

#include "scantypes.h"
#include "scandbg.h"

/* version string displayed in the usage */
#define PROGRAM_VERSION  "0.0.1"

#define MODULE_NAME "scanman" 
#define DEV_NAME "/dev/"MODULE_NAME

static int g_debug_level=0;

scan_err_t find_device_number( const char *name, int *devnum )
{
    scan_err_t final_scerr;
    int retcode;
    FILE *f;
    char line[80];
    int linelen, module_name_len;
    char *ptr_list[20];
    uint32_t longnum;

    dbg2( "%s\n", __FUNCTION__);

    f = fopen( "/proc/devices", "r" );
    if( f==NULL ) {
        dbg1( "%s unable to open /proc/devices : %s\n", __FUNCTION__,
                strerror(errno) );
        return SCANERR_NO_ENTRY; 
    }

    dbg2( "%s opened /proc/devices\n", __FUNCTION__ );

    module_name_len = strlen(MODULE_NAME);

    final_scerr = SCANERR_NO_ENTRY;

    memset( line, 0, sizeof(line) );
    while( !feof(f) ) {
        fgets( line, 79, f );

        linelen = strnlen( line, 79 );
        str_chomp( line, &linelen  );

        retcode = str_split( line, linelen, ptr_list, 20, " " );
        if( retcode != 2 ) {
            continue;
        } 
        
        if( !str_match( ptr_list[1], MODULE_NAME, module_name_len ) ) {
            continue;
        }

        dbg1( "%s found \%s\"\n", __FUNCTION__, MODULE_NAME  );

        retcode = str_mkint( ptr_list[0], &longnum );
        if( retcode == 0 ) {
            if( longnum > INT_MAX ) {
                dbg1( "%s invalid integer \"%s\"\n", __FUNCTION__, ptr_list[0] );
            }
            else { 
                *devnum = (int)longnum;
                final_scerr = SCANERR_NONE;
            }
        }

        break;
    }

    if( final_scerr != SCANERR_NONE ) {
        dbg1( "%s error: unable to find \"scanman\" in /proc/devices\n", __FUNCTION__ );
    }

    fclose( f );

    return final_scerr;
}

scan_err_t verify_device_path( const char *path, dev_t dev_num )
{
    int retcode;
    struct stat statbuf;

    dbg2( "%s \"%s\"\n", __FUNCTION__, path );

    memset( &statbuf, 0, sizeof(struct stat) );

    /* verify file exists */
    retcode = stat( path, &statbuf );
    if( retcode != 0 ) {
        dbg1( "%s \"%s\" : %s\n", __FUNCTION__, path, strerror(errno) );
        return SCANERR_NO_ENTRY;
    }

    dbg2( "%s \"%s\" exists\n", __FUNCTION__, path );

    /* verify it's a dev node & has the proper dev number */
    if( ! S_ISCHR( statbuf.st_mode ) ) {
        dbg2( "%s \"%s\" is not a character device file\n", __FUNCTION__, path );
        return SCANERR_INVALID_PARAM;
    }

    dbg2( "%s \"%s\" is a character device\n", __FUNCTION__, path );

    if( statbuf.st_rdev != dev_num ) {
        dbg2( "%s \"%s\" wrong device; want=(%d,%d) have=(%d,%d)\n", __FUNCTION__, path,
                major(dev_num), minor(dev_num),
                major(statbuf.st_rdev), minor(statbuf.st_rdev) );
        return SCANERR_INVALID_PARAM;
    }

    dbg2( "%s \"%s\" has the proper device numbers (%d,%d)\n", __FUNCTION__,
            path, major(statbuf.st_rdev), minor(statbuf.st_rdev) );

    return SCANERR_NONE;
}

scan_err_t make_device( const char *path, dev_t dev_num )
{
    int retcode;

    dbg1( "%s \"%s\"\n", __FUNCTION__, path );

    /* create it */
    retcode = mknod( path, S_IFCHR|S_IRUSR|S_IWUSR, dev_num );

    if( retcode != 0 ) {
        dbg1( "%s mknod() failed : %s\n", __FUNCTION__, strerror(errno) );
        return SCANERR_NO_ENTRY;
    }

    dbg1( "%s device node \"%s\" successfully created\n", __FUNCTION__, path );

    return SCANERR_NONE;
}

scan_err_t create_dev_scanman( void )
{
    scan_err_t scerr;
    int major_num;
    dev_t dev_num;

    dbg2( "%s\n", __FUNCTION__);

    /* find the number for scanman */
    scerr = find_device_number( MODULE_NAME, &major_num );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    dbg2( "%s found major_num=%d\n", __FUNCTION__, major_num );

    dev_num = makedev( major_num, 0 );

    /* does it already exist? */
    scerr = verify_device_path( DEV_NAME, dev_num );
    if( scerr==SCANERR_NONE ) {
        /* file exists and is the proper type so we're done here */
        dbg2( "%s \"%s\" is ok so we are done\n", __FUNCTION__, DEV_NAME );
        return SCANERR_NONE;
    }
    if( scerr != SCANERR_NO_ENTRY ) {
        return scerr;
    }

    /* create it */
    scerr = make_device( DEV_NAME, dev_num );
    if( scerr!=SCANERR_NONE ) {
        return scerr;
    }

    return SCANERR_NONE;
}

static void usage( const char *progname )
{
    fprintf( stderr, "%s %s - %s\n", progname, PROGRAM_VERSION, __DATE__ );
    fprintf( stderr, "%s - create /dev/scanman for Marvell linux-kernel scanner\n", progname ) ;
    fprintf( stderr, "usage:\n" );
    fprintf( stderr, "  -h ; this help text\n" );
    fprintf( stderr, "  -d ; increase debug log level (can be used multiple times)\n" );
}

static scan_err_t parse_args( int argc, char *argv[] ) 
{
    int retcode;
    int opt;

    while( 1 ) {
        opt = getopt( argc, argv, "hd" );
        if( opt==-1 ) {
            break;
        }

        switch( opt ) { 
            case 'h' : 
                usage(argv[0]);
                exit( EXIT_SUCCESS );
                break;

            case 'd' : 
                g_debug_level += 1;
                scanlog_set_level( g_debug_level );
                break;

            default: 
                break;
        }
    }

    return SCANERR_NONE;
}

int main( int argc, char *argv[] )
{
    scan_err_t scerr;

    scanlog_set_level( g_debug_level );

    scerr = parse_args( argc, argv );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s parse_args() failed scerr=%d\n", __FUNCTION__, scerr );
        return EXIT_FAILURE;
    }

    scerr = create_dev_scanman();
    if( scerr != SCANERR_NONE ) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

