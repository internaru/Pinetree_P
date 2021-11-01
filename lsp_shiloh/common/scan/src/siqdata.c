/* ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file siqdata.c
 *
 * \brief siqfile database
 *
 */

#include <stdint.h>
#include <string.h>

#include "lassert.h"
#include "memAPI.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "piehw.h"
#include "scanvars.h"
#include "siqdata.h"
#include "siqparse.h"

/** a list of Scan IQ entries, one for all our scanvar classes */
#define MAX_SIQ_ENTRIES 32
static struct siqdata_table *scanvar_siq_data[MAX_SIQ_ENTRIES];

struct siqdata_table *siqdata_table_new( void )
{
    struct siqdata_table *siqdata;

    siqdata = MEM_MALLOC( sizeof(struct siqdata_table) );
    if( siqdata==NULL ) {
        dbg2( "out of memory for siqdata\n" );
        return NULL;
    }

    memset( siqdata, 0, sizeof(struct siqdata_table) );
    siqdata->max_entries = MAX_HASH_ENTRIES;
    return siqdata;
}

void siqdata_table_free( struct siqdata_table **psiqdata )
{
    int i;
    struct siqdata_table *siqdata;

    dbg2( "%s %d\n", __FUNCTION__, (*psiqdata)->id );

    /* use a temporary so I don't lose my mind tracking pointers */
    PTR_ASSIGN( siqdata, *psiqdata );

    for( i=0 ; i<siqdata->num_entries ; i++ ) {
        /* the function to free a siqdata lives in the parser so the parser can
         * use it too 
         */
        siqparse_siqdata_free( &siqdata->data[i] );
    }

    /* poison the buffer in case any putz out there hung onto a pointer to the
     * data 
     */
    memset( siqdata, 0xcc, sizeof(struct siqdata_table) );
    PTR_FREE( siqdata );    
}

const struct siqdata_table *siqdata_lookup( int classid )
{
    int i;

    dbg2( "%s %d\n", __FUNCTION__, classid );

    for( i=0 ; i<MAX_SIQ_ENTRIES ; i++ ) {
        if( scanvar_siq_data[i] != NULL && scanvar_siq_data[i]->id == classid ) {
            return scanvar_siq_data[i];
        }
    }

    return NULL;
}

static scan_err_t siqdata_store( struct siqdata_table *siqdata )
{
    int i;
    struct siqdata_table *old_siqdata;

    dbg2( "%s %d\n", __FUNCTION__, siqdata->id );

    for( i=0 ; i<MAX_SIQ_ENTRIES ; i++ ) {
        if( scanvar_siq_data[i] == NULL ) {
            /* end of table */
            break;
        }

        if( scanvar_siq_data[i]->id == siqdata->id ) {
            /* found -- replace it */
            dbg2( "replacing old siq data %p with %p\n", 
                        scanvar_siq_data[i], siqdata );

            PTR_ASSIGN( old_siqdata, scanvar_siq_data[i] );
            PTR_ASSIGN( scanvar_siq_data[i], siqdata );

            /* free the old */
            siqdata_table_free( &old_siqdata );

            return 0;
        }
    }

    /* not found and table was full */
    if( i==MAX_SIQ_ENTRIES ) {
        dbg2( "cannot save %d because siq table full\n", siqdata->id );
        return SCANERR_OUT_OF_MEMORY;
    }

    /* not found -- add it */
    dbg2( "adding %p id %d at index %d\n", siqdata, siqdata->id, i );
    PTR_ASSIGN( scanvar_siq_data[i], siqdata );

    return 0;
}

int siqdata_find_key( const struct siqdata_table *siqdata, int key )
{
    int i;

    /* yeah, yeah, yeah, linear search, blah blah blah */
    for( i=0 ; i<siqdata->num_entries ; i++ ) {
        if( siqdata->data[i].key == key ) {
            return i;
        }
    }
    return -1;
}

/**
 * \brief  Parse a siqfile in to a siqdata structure.
 *
 * The siqdata is sort-of the AST. 
 *
 * Refactored siqdata_parse_file() into this function and some others.
 *
 * \author David Poole
 * \date 01-Nov-2012
 */

scan_err_t siqdata_parse_siqfile( char *buf, int buflen, struct siqdata_table *siqdata )
{
    scan_err_t scerr, final_scerr;
    int retcode;
    int idx;

    dbg2( "%s\n", __FUNCTION__ );

    final_scerr = SCANERR_NONE;

    siqparse_set_buffer( buf, buflen );

    retcode = siqparse_parse_file( siqdata );
    if( retcode != 0 ) {
        /* siq_parse_file() logs detailed error */
        dbg1( "siq parse failed with error code %d\n", retcode );
        final_scerr = SCANERR_INVALID_PARAM;
        goto leave;
    }

    /* look up the name and convert to a scanvar class id */
    idx = siqdata_find_key( siqdata, SIQ_NAME );
    if( idx == -1 ) {
        dbg1( "siq parse failure; missing required name\n" );
        final_scerr = SCANERR_NO_ENTRY;
        goto leave;
    }

    scerr = scanvar_classid_lookup( siqdata->data[idx].str, 
                                      siqdata->data[idx].num_values, 
                                      &siqdata->id );
    if( scerr!=0 ) {
        dbg1( "siq parse: ignore unknown scanvar name \"%s\"\n", 
                    siqdata->data[idx].str );
        final_scerr = SCANERR_NO_ENTRY;
        goto leave;
    }

leave:
    /* clean up the parser */
    siqparse_delete_buffer();

    return final_scerr;
}

/**
 * \brief  Parse a siqfile. Store in the global siqdata cache. 
 *
 * The cache is used to 
 *
 * (Refactored and renamed the original siqdata_parse_file())
 *
 * \author David Poole
 * \date 01-Nov-2012
 */

scan_err_t siqdata_add_siqfile( char *buf, int buflen )
{
    scan_err_t scerr, final_scerr;
    struct siqdata_table *siqdata;

    dbg2( "%s\n", __FUNCTION__ );

    final_scerr = SCANERR_GENERIC_FAIL;

    siqdata = siqdata_table_new();
    if( !siqdata ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    scerr = siqdata_parse_siqfile( buf, buflen, siqdata );
    if( scerr != SCANERR_NONE ) {
        /* siqfile_parse() logs error */
        final_scerr = scerr;
        goto fail;
    }

    /* store the siq data in our lookup table */
    scerr = siqdata_store( siqdata );
    if( scerr != 0 ) {
        /* siqdata_store() logs error */
        goto fail;
    }

    dbg2( "%s ok\n", __FUNCTION__ );

    return SCANERR_NONE;

fail:
    siqdata_table_free( &siqdata );

    return final_scerr;
}

