/* 
 *
 * ============================================================================
 * Copyright (c) 2006-2009   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifdef HAVE_PRINT_SUPPORT
#include "agprint.h"
#endif
#include "memAPI.h"
#include "logger.h"
#include "lassert.h"
#include "http_parse.h"
#include "dprintf.h"
#include <string.h>
#include <stdlib.h>
#include "oid_api.h"
#include "http_config.h"
#include "http_oid_lut_config.h"
#ifndef __linux__
#include "cpu_api.h"
#endif

// Submodule ID for debug message logging
#define LOGGER_SUBMOD_IFACE HTTP_SUBMOD_OID
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_HTTP | LOGGER_SUBMOD_IFACE 
#define DBG_PRFX    "HTTP OID LUT: "

/** 
 * \brief check if current xml tag signals oid creation
 * 
 * \param tag_name - xml tag heirarchy
 * 
 * \param context - context with oid links
 * 
 * This function handles the case where an XML tag has no value
 * associated with it ( ie: <mcdyn:ClearAllCounters/> ).  When we
 * encounter a start tag, we check for these special tags - and
 * if they exist add oids now.  The general case is to wait for
 * a value in between tags to then create the oid.
 * 
 **/
void checkXmlTagsWithoutValues( char* tag_name, HttpPageContext* context )
{
    int32_t table_index = -1;
    uint32_t i;
    error_type_t e_res;
    http_config_t config;

    e_res = http_config_get( &config );
    ASSERT( OK == e_res );

    for ( i = 0; i < config.tag_wout_values_table_size; i++ ) 
    {
        ASSERT( NULL != config.tag_wout_values_table );

        if ( compare_tag_with_optional_namespace( tag_name, ( config.tag_wout_values_table[ i ] ).xml_string ) == true )
        {
            table_index = i;
            break;
        }
    }

    if( table_index == -1 ) 
    {
        // No tag matches table
        return;
    }

    // Clear OIDVAL just for sanity's sake
    if ( addOidLinkToContext( context, 
                              ( config.tag_wout_values_table[ table_index ] ).oid,
                              ( config.tag_wout_values_table[ table_index ] ).index,
                              NULL ) == false )
    {
        DBG_ERR( "Failed to create oid for valueless tag: %s\n", tag_name );
        addMethodNotAllowed( context );
    }
}


/** 
 * \brief check if current xml tag forces the creation of a set
 * of oids
 * 
 * \param tag_name - xml tag heirarchy
 * 
 * \param context - context with oid links
 * 
 * The way options are disabled in XML is to not include them
 * in the list of enabled options.  So when we get a start tag
 * to a list of options, we create oids for each option in that
 * list and set their values to off.  Then when we parse the
 * rest of the file, all enabled options will update their
 * previously created oid to turn the option on.
 * 
 **/
void checkXmlTagtoCreateOids( char * tag_name, HttpPageContext* context )
{
    int32_t table_index = -1;
    uint32_t i;
    error_type_t e_res;
    http_config_t config;

    e_res = http_config_get( &config );
    ASSERT( OK == e_res );

	for ( i = 0; i < config.create_table_size; i++ ) 
	{
        ASSERT( NULL != config.create_table );

        if ( compare_tag_with_optional_namespace( tag_name, ( config.create_table[ i ] ).xml_string ) == true )
        {
            table_index = i;
            break;
        }
	}

    if( table_index == -1 ) 
    {
        // no matching tag in table
        return;
    }

    if ( createOidLinks( context, ( config.create_table[ table_index ] ).oid, ( config.create_table[ table_index ] ).index ) == false )
    {
        DBG_ERR( "Failed to create oid links - setting method not allowed in context\n" );
        addMethodNotAllowed( context );
    }
}


/** 
 * \brief convert xml tag heirarchy to an oid for setting
 * 
 * \param tag_name - xml tag heirarchy
 * 
 * \param value - string value to save
 * 
 * \param context - context to store oid structure
 * 
 * Looks up based of xml tag values the correct oid that will be
 * used to save the corresponding value.  The oid is not saved
 * now, but will be saved upon successful parsing of the entire
 * file.  
 * 
 **/
void ConvertXmlTagtoOid( char * tag_name, char* value, HttpPageContext* context )
{
    int32_t  table_index = -1;
    uint32_t  i;
    char *val;
    error_type_t e_res;
    http_config_t config;

    e_res = http_config_get( &config );
    ASSERT( OK == e_res );

    DBG_MSG( "Converting: '%s=%s' to oids\n", tag_name, value );

	for ( i = 0; i < config.normal_table_size; i++ ) 
	{
        ASSERT( NULL != config.normal_table );

        if ( compare_tag_with_optional_namespace( tag_name, ( config.normal_table[ i ] ).xml_string ) == true )
        {
            table_index = i;
            break;
        }
	}

    if( table_index == -1 ) //OID was not found
    {
        addMethodNotAllowed( context );
        return;
    }

    if ( ( config.normal_table[ table_index ] ).action == HTTP_XML_TO_OID_LUT_NO_ACTION )
    {
        return;
    }
    else if (( config.normal_table[ table_index ] ).oid == UPDATE_OID_LINK )
    {
        updateOidLink(context, value);
    }
    else
    {
        uint32_t index;
        val = ( char* )MEM_MALLOC( OID_MAX_STRING_SIZE_IN_BYTES );
        strcpy( val, value );
        index = ( config.normal_table[ table_index ] ).index;

        if ( ( config.normal_table[ table_index ] ).index == DYNAMIC_INDEX_SELECTION )
        {
             oidLink *last_oid;
             last_oid = http_oid_lut_get_last_oid( context );
             if ( last_oid->oid == ( config.normal_table[ table_index ] ).oid )
             {
                 index = last_oid->index + 1;
             }
             else
             {
                 index = 0;
             }
        }

        if ( addOidLinkToContext( context, 
                                  ( config.normal_table[ table_index ] ).oid,
                                  index,
                                  val ) == false )
        {
            addMethodNotAllowed( context );
        }
    }
}


/** 
 * \brief Save the given val into the oid/index location
 * 
 * \param context - Context to in which to add oid
 * 
 * \param oid - oid to update
 * 
 * \param index - index into oid
 * 
 * \param val - value to save into the oid
 * 
 * \return bool - indicates success of function
 * 
 * As the XML files are parsed, the values contained in
 * POSTS/PUTs need to be saved into the FW. The oids are the
 * accessors into the rest of the system. This function adds a
 * link to a list of oids that need to be saved upon successful
 * parsing of the XML file
 * 
 **/
bool addOidLinkToContext( HttpPageContext* context, oid_t oid, uint32_t index, char *val )
{
    oidLink* new_link;
    oidLink* tmp_link;

    DBG_VERBOSE( "%s - xml: %s - oid: 0x%08X.%d - value: '%s'\n", __FUNCTION__, context->xmlTag, oid, index, val == NULL ? "null" : val );
    
    new_link = ( oidLink* ) MEM_MALLOC_ALIGN( sizeof( oidLink ),
                                              cpu_get_dcache_line_size() );
    if ( new_link == NULL )
    {
        return false;
    }
    memset( new_link, 0x00, sizeof( oidLink ) );

    new_link->oid = oid;
    new_link->index = index;
    new_link->oid_val_str = val;
    new_link->oid_val_int = 0;

    tmp_link = context->oidsToSave;
    // Check if there is anything in the list
    if ( tmp_link == NULL )
    {
        context->oidsToSave = new_link;
    }
    // Add new link to the end of the list
    else
    {
        tmp_link = http_oid_lut_get_last_oid( context );
        // Save link at end of list
        tmp_link->next = new_link;
    }

    return true;
}

oidLink *http_oid_lut_get_last_oid( HttpPageContext *context )
{
    oidLink *cur_link = context->oidsToSave;
    while ( cur_link->next != NULL )
    {
        cur_link = cur_link->next;
    }
    return cur_link;
}
