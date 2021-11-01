/* 
 * ============================================================================
 * Copyright (c) 2009-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file adfsensor_appspace.c
 *
 * \brief Small implementations for Linux appspace. 
 *
 * Uses sysfs to get adf sensor status
 *
 **/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scantypes.h"
#include "scancore.h"
#include "adfsensor.h"
#include "scansysfs.h"
#include "utils.h"

#define MAX_LINE 80

void adf_sensor_onetime_init( void )
{
}

bool adf_sensor_paper_present( void )
{
    FILE *f;
    char line[MAX_LINE+1];
    int len, retcode;
    uint32_t num32;

    f = fopen( SCAN_ADF_PAPER_PRESENT_PATH , "r" );
    if( f==NULL ) {
        perror( "fopen " SCAN_ADF_PAPER_PRESENT_PATH );
        return false;
    }

    memset( line, 0, sizeof(line) );
    fread( line, 1, MAX_LINE, f );

    fclose( f );

    len = strnlen(line,MAX_LINE);
    str_chomp(line,&len);
    
    retcode = str_mkint( line, &num32 );
    if( retcode != 0 ) {
        return false;
    }

    if( num32==1 ) {
        return true;
    }

    return false;
}

bool adf_sensor_paper_in_path( void )
{
    /* TODO */
    return false;
}

void adf_sensor_pip_callback_enable( void )
{
    /* TODO */
}

void adf_sensor_pip_callback_disable( void )
{
    /* TODO */
}

void adf_sensor_powerdown( void )
{
    /* TODO */
}

void adf_sensor_powerup( void )
{
    /* TODO */
}

