/**
 * \file scands.c
 *
 * \brief Scanner Data Store. Attempt to be a simple key-value pair.
 *
 * Created davep 23-May-2010
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "utils.h"

#include "scantypes.h"
#include "scancore.h"
#include "scanlog.h"
#include "scandbg.h"
#include "scands.h"
#include "ostools.h"
#include "strtable.h"

/* max keys in the scands; these values are array sizes in scands */
#define SCANDS_MAX_VALUES  128

//#define SCANDS_DEBUG 

static struct scos_sem scands_protection_semaphore;

struct scands_data {
//    uint32_t key; /* todo? */
    bool is_valid;   /* 0 if the data is NULL (uninitialized), 1 if data is valid (not NULL) */

    char *name;      /* NULL if unnamed, pointer into string table if named */
    int namelen;     /* length of name */

    union {
        uint32_t uint32;
        /* more later ... maybe */
    } value;
};

static struct scands_data scands[SCANDS_MAX_VALUES];

static struct string_table name_table;

static void scands_lock( void )
{
    scan_err_t scerr;
    struct timespec abs_timeout;

    /* no one should hold this lock for very long */
    clock_gettime( CLOCK_REALTIME, &abs_timeout );

    /* wait 1 second */
    calc_future_usec( &abs_timeout, USEC_PER_SECOND );

    scerr = scos_sem_timedwait( &scands_protection_semaphore, &abs_timeout );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

static void scands_unlock( void )
{
    scan_err_t scerr;

    scerr = scos_sem_post( &scands_protection_semaphore );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

scan_err_t scands_onetime_init( void )
{
    scan_err_t scerr;

    scerr = scos_sem_init( &scands_protection_semaphore, "/scands", 1 );
    XASSERT( scerr==SCANERR_NONE, scerr );

    memset( scands, 0, sizeof(scands) );

    scerr = string_table_new( &name_table, 1024, SCANDS_MAX_NAMELEN );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* more here? */

    return SCANERR_NONE;
}

void scands_debug_dump( void )
{
    int i;

    for( i=0 ; i<SCANDS_MAX_VALUES ; i++ ) {
        if( !scands[i].is_valid ) {
            continue;
        }

        dbg1( "[%d] name=%s namelen=%d value=%d (0x%x)\n", 
                    i, 
                    scands[i].name==NULL ? "(none)" : scands[i].name,
                    scands[i].namelen,
                    scands[i].value.uint32, 
                    scands[i].value.uint32
            );
    }
}

static scan_err_t find_empty_slot( int *idx )
{
    int i;

    /* find an empty slot */
    for( i=0 ; i<SCANDS_MAX_VALUES ; i++ ) {
        if( ! scands[i].is_valid ) {
            *idx = i;
            return SCANERR_NONE;
        }
    }
    
#ifdef SCANDS_DEBUG
    /* davep 03-Dec-2010 ; TODO add dynamic expansion of my scands table */
    dbg2( "%s no empty slot\n", __FUNCTION__ );
#endif

    return SCANERR_NO_ENTRY;
}

/* if we haven't been initialized, gently fail */
#define SANITY_CHECK() do {\
        if( scos_sem_is_initialized(&scands_protection_semaphore)!=SCANERR_NONE ) {\
            ASSERT(0);\
            return SCANERR_NOT_PERMITTED;\
        }\
    } while(0);

/* This macro MUST be called before scands_lock()!  Does not unlock the scands
 * before returning on error! 
 *
 * Valid names must be <= SCANDS_MAX_NAMELEN chars etc.
 */
#define VALIDATE_NAME(name) \
    do {\
        scerr = validate_name(name,&namelen);\
        if( scerr != SCANERR_NONE ) {\
            return scerr;\
        }\
    } while(0);

static scan_err_t validate_name( const char *s, int *namelen )
{
    int i;

    if( s==NULL ) {
        return SCANERR_INVALID_PARAM;
    }

    /* +1 to catch names longer than our restriction */
    *namelen = strnlen( s, SCANDS_MAX_NAMELEN+1 );
    if( *namelen==0 || *namelen > SCANDS_MAX_NAMELEN ) {
        return SCANERR_INVALID_PARAM;
    }

    /* valid names are [A-Za-z_][A-Za-z_0-9]* */
    i=0;
    if( !( isalpha(s[i]) || s[i]=='_' ) ) {
        return SCANERR_INVALID_PARAM;
    }


    for( i=1 ; i<*namelen ; i++ ) {
        if( !( isalnum(s[i]) || s[i]=='_' ) ) {
            return SCANERR_INVALID_PARAM;
        }
    }

    return SCANERR_NONE;
}

static scan_err_t get_by_name( const char *name, int *idx )
{
    int i;

    /* linear search. bleah.
     * TODO switch to a balanced tree of some sort 
     */
    for( i=0 ; i<SCANDS_MAX_VALUES ; i++ ) {
        if( scands[i].is_valid && str_match( scands[i].name, name, scands[i].namelen ) ) {
            XASSERT( scands[i].name, i );
            *idx = i;
            return SCANERR_NONE;
        }
    }

    return SCANERR_NO_ENTRY;
}

static scan_err_t get_integer( const char *name, uint32_t *value )
{
    scan_err_t scerr;
    int idx;
    int namelen;

#ifdef SCANDS_DEBUG
    dbg2( "%s %s\n", __FUNCTION__, name );
#endif

    scerr = SCANERR_NONE;

    /* sanity check on the name before we bother locking */
    VALIDATE_NAME(name);

    SANITY_CHECK();
    
    if( !value) {
        return SCANERR_INVALID_PARAM;
    }

    scands_lock();

    scerr = get_by_name( name, &idx );
    if( scerr != SCANERR_NONE ) {
        goto leave;
    }

    XASSERT( scands[idx].is_valid, idx );

    *(uint32_t*)value = scands[idx].value.uint32;

leave:
    scands_unlock();
    return scerr;
}

scan_err_t scands_get_integer_with_default( const char *name, uint32_t *value, uint32_t default_value )
{
    scan_err_t scerr;

    *value = default_value;

    scerr = get_integer( name, value );
    if( scerr==SCANERR_NO_ENTRY ) {
        scerr = SCANERR_NONE;
    }

    return scerr;
}

scan_err_t scands_get_integer( const char *name, uint32_t *value )
{
    return get_integer( name, value );
}

scan_err_t scands_set_integer( const char *name, uint32_t value )
{
    scan_err_t scerr;
    int idx;
    char *ptr_to_name;
    int namelen;

#ifdef SCANDS_DEBUG
    dbg2( "%s %s %d\n", __FUNCTION__, name, value );
#endif

    scerr = SCANERR_NONE;

    /* sanity check on the name before we bother locking */
    VALIDATE_NAME(name);

    SANITY_CHECK();

    scands_lock();

    /* if exists, save the new value and we're done */
    scerr = get_by_name( name, &idx );
    if( scerr == SCANERR_NONE ) {
        scands[idx].value.uint32 = value;
        goto leave;
    }

    /* At this point, we didn't find the name or something else went wrong. 
     * If something else went wrong, bail now.
     */
    if( scerr != SCANERR_NO_ENTRY ) {
        goto leave;
    }

    /* 
     * No such name. Create. 
     */

    /* get string storage first (most likely to fail) */
    scerr = string_table_add_string( &name_table, name, &ptr_to_name ); 
    if( scerr == SCANERR_OUT_OF_MEMORY ) {
        /* TODO ; realloc / rebuild string table */
#ifdef SCANDS_DEBUG
        dbg2( "%s string table full\n", __FUNCTION__ );
#endif
        goto leave;
    }
    
    /* create */
    scerr = find_empty_slot( &idx );
    if( scerr != SCANERR_NONE ) {
        goto leave;
    }
    memset( &scands[idx], 0, sizeof(struct scands_data) );

    /* initialize */
    scands[idx].name = ptr_to_name;
    scands[idx].namelen = namelen;
    scands[idx].value.uint32 = value;
    scands[idx].is_valid = true;

#ifdef SCANDS_DEBUG
    dbg2( "%s idx=%d ptr=%s name=%s\n", __FUNCTION__, 
                idx, ptr_to_name, scands[idx].name  );
#endif

leave:
    scands_unlock();
    return scerr;
}

scan_err_t scands_delete_integer( const char *name )
{
    scan_err_t scerr;
    int idx;
    int namelen;

    scerr = SCANERR_NONE;

    /* sanity check on the name before we bother locking */
    VALIDATE_NAME(name);

    SANITY_CHECK();

    scands_lock();

    scerr = get_by_name( name, &idx );
    if( scerr != SCANERR_NONE ) {
        goto leave;
    }

    /* Stomp it. Note we're leaving the string in the string table for later
     * garbage collection. When the string table fills, a new string table is
     * allocated and in-use strings are re-added. Orphaned strings will be
     * freed.
     */
    memset( &scands[idx], 0, sizeof(struct scands_data) );

    /* unnecessary due to the memset() but added so "grep is_valid" finds a
     * proper delete (kept confusing myself)
     */
    scands[idx].is_valid = false;

leave:
    scands_unlock();
    return scerr;
}

