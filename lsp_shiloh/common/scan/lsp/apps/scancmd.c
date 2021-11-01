/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/* 
 * Simple userspace app to send a scan command line to kscantask kernel driver
 * via sysfs
 *
 * davep Oct-2012 
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/* need strnlen() and current rev of compiler requires __USE_GNU */
#define __USE_GNU  
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include "scantypes.h"
#include "scancore.h"
#include "scansysfs.h"

#define MAX_CMD_LINE 128

int main( int argc, char *argv[] )
{
    FILE *outfile;
    int i;
    char cmdline[MAX_CMD_LINE+1];
    int cmdline_len;

    memset( cmdline, 0, sizeof(cmdline) );
    cmdline_len = 0;
    for( i=1 ; i<argc ; i++ ) {
        strncat( cmdline, " ", MAX_CMD_LINE-cmdline_len );
        strncat( cmdline, argv[i], MAX_CMD_LINE-cmdline_len );
        cmdline_len = strnlen( cmdline, MAX_CMD_LINE );
    }
    printf( "%s\n", cmdline );

    outfile = fopen( SCAN_CMDLINE_SYSFS_PATH, "w" );
    if( outfile==NULL ) {
        perror( "fwrite " SCAN_CMDLINE_SYSFS_PATH );
        return EXIT_FAILURE;
    }

    fwrite( cmdline, 1, cmdline_len, outfile );
    fflush( outfile );

    fclose( outfile );

    return EXIT_FAILURE;
}

