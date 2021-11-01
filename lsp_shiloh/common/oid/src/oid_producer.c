/*
 * ============================================================================
 * Copyright (c) 2009-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_producer.c
 *
 * \brief This file implements the OID Producer API.  See oid_producer_api.h
 * for details. 
 * 
 **/

#include <string.h>
#include <stdint.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "ATypes.h" // for NULL define

#include "dprintf.h"
#include "logger.h"
#include "lassert.h"

#include "memAPI.h"
#include "tree_unordered.h"

/* modules registering OIDS */
#include "oid_module_memory.h"
#include "oid_module_feature_support.h"
#ifdef HAVE_PLATFORM
#include "oid_module_platform.h"
#endif
#ifdef HAVE_COMMON_CONSUMABLES_API
#include "oid_module_consumable.h"
#endif
#ifdef HAVE_HTTP
#include "oid_module_http.h"
#endif
#ifdef HAVE_USB_MASS_STORAGE
#include "oid_module_rem_storage.h"
#endif
#ifdef HAVE_PRINT_SUPPORT
#include "oid_module_status.h"
#include "oid_module_print.h"
#endif
#include "oid_vendor_api.h"
#ifdef HAVE_STRING_MGR
#include "oid_module_string_mgr.h"
#endif
#ifdef HAVE_SCAN_SUPPORT
#include "oid_module_scan.h"
#endif
#ifdef HAVE_USB_DEVICE
#include "oid_module_usb_device.h"
#endif
#ifdef HAVE_NETWORK
#include "oid_module_network.h"
#endif
#ifdef HAVE_SM_JOB
#include "oid_sm_api.h"
#endif
#if defined(HAVE_AIRPRINT) && defined(HAVE_IPP)
#include "oid_airprint_api.h"
#endif
#ifdef HAVE_WIRELESS
#include "oid_module_wifi.h"
#endif
#ifdef HAVE_GCPP
#include "oid_module_gcpp.h"
#endif
#ifdef HAVE_FAX_SUPPORT
#if HAVE_FAX_SUPPORT != silabs
#include "oid_module_fax.h"
#endif
#endif

#if defined( HAVE_CMD ) && defined ( DEBUG )
#include "cmd_proc_api.h"
static const char *oid_cmd_list_desc;
static const char *oid_cmd_list_usage;
static const char *oid_cmd_list_notes;
static int oid_cmd_list_cb( int argc, char *argv[ ] );

static const char *oid_cmd_getset_desc;
static const char *oid_cmd_getset_usage;
static const char *oid_cmd_getset_notes;
static int oid_cmd_getset_cb( int argc, char* argv[] );
#include <string.h>
#endif

/**
 *
 * \brief Contains an ID and tree node to keep track of OID dynamic 
 * registration callback's using each of the oid_t's four parameters as
 * a seperate tree level
 *
 */
typedef struct oid_registration_tree_node_s
{
    tree_node_t tree_node; /*!< the tree node associated with the id data */

    uint8_t id; /*!< the id data associated with this node */

} oid_registration_tree_node_t;


/**
 *
 * \brief A structure to hold a callback entry containing the back-end
 * get and set functions associated with an oid.  The structure also
 * contains a tree_node, which is always a leaf node in the oid callback
 * tree
 *
 */
typedef struct oid_callback_tree_node_s
{
    tree_node_t tree_node; /*!< the tree node associated with the callback */

    oid_t oid; /*!< the OID associated with the callbacks */

    void * get_callback; /*!< the back-end callback function that will
                              GET the data associated with the oid */

    void * set_callback; /*!< the back-end callback function that will
                              SET the data associated with the oid */

    void * test_callback; /*!< the back-end callback function that will
                               TEST the data associated with the oid */

} oid_callback_tree_node_t;


/* please find function comments at implementation below */
static tree_node_t * oid_tree_child_find( tree_node_t * parent, uint8_t id );
static tree_node_t * oid_tree_child_add( tree_node_t * parent, uint8_t id );
static oid_callback_tree_node_t * oid_producer_find_callback_entry( oid_t oid );
    

/** \brief The head/parent node of the oid registration tree */
static tree_node_t oid_registration_tree;


/**
 *
 * \brief This function allows a producer to register a set of callback
 * objects with the oid manager.
 *
 * \param[in] oid A valid oid_t created using the
 * "oid_make()" function
 *
 * \param[in] get_callback A valid pointer to a callback function to get the
 * object's data value on the back-end
 *
 * \param[in] set_callback A valid pointer to a callback function to set the
 * object's data value on the back-end
 *
 * \param[in] test_callback A valid pointer to a callback function to test the
 * object's data value on the back-end
 *
 * \return One of the error_type_t return codes:
 * OK, If successful, otherwise one of the following error codes:
 * FAIL
 *
 */
error_type_t oid_register_callbacks( oid_t oid,
        void * get_callback,
        void * set_callback,
        void * test_callback )
{
    oid_callback_tree_node_t * callback_entry;
    tree_node_t * child;
    tree_node_t * parent;
    uint8_t id;

    ASSERT( NULL != get_callback );
    ASSERT( NULL != set_callback );
    ASSERT( NULL != test_callback );

    parent = &oid_registration_tree;

    /* we are starting a 4-part search for an existing oid entry
     * if no oid entry exists, we'll create the tree and associate the 
     * last leaf with the get and set callback */

    /* search the first level of the tree for the namespace id */
    id = oid_namespace_get( oid );
    child = oid_tree_child_find( parent, id );

    /* does the module id exist in our tree?
     * if not, then we create module id the entry */
    if ( NULL == child )
    {
        child = oid_tree_child_add( parent, id );
        ASSERT( NULL != child );
    }

    parent = child;
    ASSERT( NULL != parent );

    /* search the second level of the tree for the area id */
    id = oid_area_id_get( oid );
    child = oid_tree_child_find( parent, id );

    /* does the area id exist in our tree?
     * if not, then we create module area id the entry */
    if ( NULL == child )
    {
        child = oid_tree_child_add( parent, id );
        ASSERT( NULL != child );
    }

    parent = child;
    ASSERT( NULL != parent );

    /* search the third level of the tree for the value id */
    id = oid_value_id_get( oid );
    child = oid_tree_child_find( parent, id );

    /* does the value id exist in our tree?  It better not!
     * if not, then we create module value id the entry */
    if ( NULL == child )
    {
        child = oid_tree_child_add( parent, id );
        ASSERT( NULL != child );
    }

    parent = child;
    ASSERT( NULL != parent );

    /* search the fourth level of the tree for the object type id */
    id = oid_type_id_get( oid );
    child = oid_tree_child_find( parent, id );

    /* ASSERT fires if entry already exists,
     * cannot register twice with same OID  */
    XASSERT( NULL == child, oid );

    /* create a namespace entry */
    child = oid_tree_child_add( parent, id );
    ASSERT( NULL != child );

    /* create the callback entry and add it as a leaf to the tree */
    callback_entry = (oid_callback_tree_node_t *)MEM_MALLOC( sizeof( oid_callback_tree_node_t ) );
    ASSERT( NULL != callback_entry );

    parent = child;
    ASSERT( NULL != parent );

    tree_child_add( parent, &( callback_entry->tree_node ) );

    callback_entry->oid = oid;
    callback_entry->get_callback = get_callback;
    callback_entry->set_callback = set_callback;
    callback_entry->test_callback = test_callback;

    return OK;
}


/**
 *
 * \brief Register the back-end functions for each module with the OID API
 *
 */
void oid_producer_register( void )
{
    tree_init(&oid_registration_tree);

    #if defined( HAVE_CMD ) && defined ( DEBUG )
    int cmd_res;
    cmd_res = cmd_register_cmd( "oid",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    XASSERT( ( CMD_OK == cmd_res ), cmd_res );
    cmd_res = cmd_register_subcmd( "oid",
                                   "list",
                                   oid_cmd_list_desc,
                                   oid_cmd_list_usage,
                                   oid_cmd_list_notes,
                                   oid_cmd_list_cb );
    XASSERT( ( CMD_OK == cmd_res ), cmd_res );
    cmd_res = cmd_register_subcmd( "oid",
                                   "getset",
                                   oid_cmd_getset_desc,
                                   oid_cmd_getset_usage,
                                   oid_cmd_getset_notes,
                                   oid_cmd_getset_cb );
    XASSERT( ( CMD_OK == cmd_res ), cmd_res );
	#endif

    /* register each module's OIDS with the producer API */

    oid_register_module_memory();

    oid_register_module_feature_support();

    #ifdef HAVE_PLATFORM
    oid_register_module_platform();
    #endif

    #ifdef HAVE_COMMON_CONSUMABLES_API
    oid_register_module_consumable();
    #endif

    #ifdef HAVE_STATUS
    oid_register_module_status();
    #endif

    #ifdef HAVE_PRINT_SUPPORT
    oid_register_module_print();
    #endif

	/*SINDOH OID*/
    oid_register_module_vendor();
	/*SINDOH OID*/

    #ifdef HAVE_STRING_MGR
    oid_register_module_string_mgr();
    #endif

    #ifdef HAVE_HTTP
    oid_register_module_http();
    #endif

    #ifdef HAVE_USB_MASS_STORAGE
    oid_register_module_rem_storage();
    #endif

    #ifdef HAVE_SCAN_SUPPORT
    oid_register_module_scan();
    #endif

    #ifdef HAVE_USB_DEVICE
    oid_register_module_usb_device();
    #endif

    #ifdef HAVE_NETWORK
    #ifdef WIRED_NETWORK
    oid_register_module_network();
    #endif
    #endif

    #ifdef HAVE_SM_JOB
    oid_register_module_sm();
    #endif

    #if defined(HAVE_AIRPRINT) && defined(HAVE_IPP)
    oid_register_module_airprint();
    #endif
    
    #if defined HAVE_WLAN || defined HAVE_WIRELESS
    oid_register_module_wifi();
    #endif

    #ifdef HAVE_GCPP
    oid_register_module_gcpp();
    #endif

	#ifdef HAVE_FAX_SUPPORT
	#if HAVE_FAX_SUPPORT != silabs
    oid_register_module_fax();
    #endif
	#endif

}


/**
 * 
 * \brief Retrieve the producer's get callback back-end function
 * or return NULL if no back-end function is registered with this OID
 * value
 *
 * \param[in] oid A valid oid_t created using the
 * "oid_make()" function
 *
 * \return A valid function pointer to the oid's back-end function
 * or NULL if no back-end function is registered with this OID value
 *
 */
void * oid_producer_get_func( oid_t oid )
{
    void * get_func;
    oid_callback_tree_node_t * callback_entry;

    get_func = NULL;

    callback_entry = oid_producer_find_callback_entry( oid );

    if ( NULL != callback_entry )
    {
        ASSERT( NULL != callback_entry->get_callback );
        ASSERT( oid == callback_entry->oid );

        get_func = callback_entry->get_callback;
    }

    return get_func;
}


/**
 * 
 * \brief Retrieve the producer's set callback back-end function
 * or return NULL if no back-end function is registered with this OID
 * value
 *
 * \param[in] oid A valid oid_t created using the
 * "oid_make()" function
 *
 * \return A valid function pointer to the oid's back-end function
 * or NULL if no back-end function is registered with this OID value
 *
 */
void * oid_producer_set_func( oid_t oid )
{
    void * set_func;
    oid_callback_tree_node_t * callback_entry;

    set_func = NULL;

    callback_entry = oid_producer_find_callback_entry( oid );

    if ( NULL != callback_entry )
    {
        ASSERT( NULL != callback_entry->set_callback );
        ASSERT( oid == callback_entry->oid );

        set_func = callback_entry->set_callback;
    }

    return set_func;
}


/**
 * 
 * \brief Retrieve the producer's test callback back-end function
 * or return NULL if no back-end function is registered with this OID
 * value
 *
 * \param[in] oid A valid oid_t created using the
 * "oid_make()" function
 *
 * \return A valid function pointer to the oid's back-end function
 * or NULL if no back-end function is registered with this OID value
 *
 */
void * oid_producer_test_func( oid_t oid )
{
    void * test_func;
    oid_callback_tree_node_t * callback_entry;

    test_func = NULL;

    callback_entry = oid_producer_find_callback_entry( oid );

    if ( NULL != callback_entry )
    {
        ASSERT( NULL != callback_entry->test_callback );
        ASSERT( oid == callback_entry->oid );

        test_func = callback_entry->test_callback;
    }

    return test_func;
}


/**
 *
 * \brief Find a callback entry associated with this OID value.
 *
 * \param oid A valid oid_t created using the "oid_make()" macro
 *
 * \return A valid oid_callback_tree_node_t pointer or NULL if no
 * callback entry is associated with this oid value
 *
 */
static oid_callback_tree_node_t * oid_producer_find_callback_entry( oid_t oid )
{
    oid_callback_tree_node_t * callback_entry;
    tree_node_t * child;
    tree_node_t * parent;
    uint8_t id;

    callback_entry = NULL;
    parent = &oid_registration_tree;

    /* we are starting a 4-part search for an existing oid entry
     * so we can return the callback entry */

    /* search the first level of the tree for the namespace id */
    id = oid_namespace_get( oid );
    child = oid_tree_child_find( parent, id );

    /* does the namespace id exist in our tree? */
    if ( NULL != child )
    {
        parent = child;

        /* search the second level of the tree for the area id */
        id = oid_area_id_get( oid );
        child = oid_tree_child_find( parent, id );
    }

    /* does the area id exist in our tree? */
    if ( NULL != child )
    {
        parent = child;

        /* search the third level of the tree for the value id */
        id = oid_value_id_get( oid );
        child = oid_tree_child_find( parent, id );
    }

    /* does the value id exist in our tree? */
    if ( NULL != child )
    {
        parent = child;

        /* search the fourth level of the tree for the object type id */
        id = oid_type_id_get( oid );
        child = oid_tree_child_find( parent, id );
    }

    /* does the object type id exist in our tree? */
    if ( NULL != child )
    {
        /* There is an entry registered with this OID! */
        parent = child;
        /* get the first child in the tree */
        child = tree_child_get_first( parent );
        /* pull the oid_callback_tree_node_t out based on the child */
        callback_entry = tree_node_data_get( child, oid_callback_tree_node_t, tree_node);

        ASSERT( NULL != callback_entry );
        ASSERT( callback_entry->oid == oid );
    }

    return callback_entry;
}


/**
 *
 * \brief create a registration tree entry and associated the id and parent
 * tree node together 
 *
 * \param parent The oid registration tree or any descendent of it.
 *
 * \param id The ID data to associate with the new child
 *
 * \return tree_node_t The tree_node_t that has been associated with the
 * oid registration tree node and the id. Never returns NULL.
 *
 */
static tree_node_t * oid_tree_child_add( tree_node_t * parent, uint8_t id )
{
    oid_registration_tree_node_t * registration_entry;

    ASSERT( NULL != parent );

    /* allocate registration tree node and assign data and add to tree */
    registration_entry = (oid_registration_tree_node_t *)MEM_MALLOC( 
            sizeof( oid_registration_tree_node_t ) );
    ASSERT( NULL != registration_entry );

    /* initialize and add a child to this parent */
    tree_child_add( parent, &( registration_entry->tree_node ) );

    registration_entry->id = id;

    /* note registration_entry memory is not lost
     * since the tree API data get macro allows us to get this
     * entry even if we don't keep track of the pointer. */

    return &( registration_entry->tree_node );
}


/**
 *
 * \brief Searches the given tree to find a child with the id
 * given.  The tree can be the OID registration tree or any descendent of
 * the OID registration tree, except the leaf level
 *
 * \param parent The oid registration tree or any descendent of it.
 *
 * \param id The id data of the child to find.  This id is one of the 
 * four uint8_t parameters used to build the oid.
 *
 * \return tree_node_t A valid child pointer to another tree_node_t 
 * that contains the id data, or NULL if no child was found with that id 
 *
 */
static tree_node_t * oid_tree_child_find( tree_node_t * parent, uint8_t id )
{
    tree_node_t * child;
    oid_registration_tree_node_t * registration_entry;

    ASSERT( NULL != parent );

    /* get the first child in the tree */
    child = tree_child_get_first( parent );

    /* iterate through the children in the parent tree */ 
    while ( NULL != child )
    {
        /* pull the oid_registration_tree_node_t out based on the node */
        registration_entry = tree_node_data_get( child, oid_registration_tree_node_t, tree_node);
        ASSERT( NULL != registration_entry );

        if ( id == registration_entry->id )
        {
            /* child is assigned and the data matches
             * break out and we'll return the child */
            break;
        }

        /* get the next child in the tree */
        child = tree_child_get_next( parent, child );
    }

    #ifdef DEBUG
    if ( NULL != child )
    {
        ASSERT( NULL != registration_entry );
        ASSERT( id == registration_entry->id );
    }
    #endif

    return child;
}


/**
 * 
 * \brief The backend oid module function for a module without a get function.
 * This function should be used in place of a valid function and will always
 * return FAIL.
 *
 * \return error_type_t FAIL is always returned
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_null_get( void)
{
    return FAIL;
}


/**
 * 
 * \brief The backend oid module function for a module
 * without a set function.  This function will be registered in place
 * of one that a module would have registered and will always return fail
 *
 * \return error_type_t FAIL is always returned
 * FAIL                      - bad oid, other error
 *
 */
error_type_t oid_module_null_set( void )
{
    return FAIL;
}


/**
 * 
 * \brief The backend oid module function for a module
 * without a test function.  This function will be registered in place
 * of one that a module would have registered and will always return unimplmented
 *
 * \return error_type_t OID_ERROR_NOT_IMPLEMENTED is always returned
 *
 */
error_type_t oid_module_null_test( void )
{
    return OID_ERROR_NOT_IMPLEMENTED;
}


#if defined( HAVE_CMD ) && defined ( DEBUG )

static char *oid_debug_map_namespace( uint8_t type )
{
    char *namespace_str;
    switch ( type )
    {
        case OID_NAMESPACE_SYSTEM:
            namespace_str = "SYSTEM";
            break;
        case OID_NAMESPACE_PWG_SM:
            namespace_str = "PWG_SM";
            break;
        case OID_NAMESPACE_VENDOR:
            namespace_str = "VENDOR";
            break;
        case OID_NAMESPACE_SNMP:
            namespace_str = "SNMP";
            break;
        default:
            namespace_str = "unknown";
            break;
    }
    return namespace_str;
}


static char *oid_debug_map_area ( uint8_t area )
{
    char *area_str;
    switch( area )
    {
        case OID_AREA_PRINT:
            area_str = "PRINT";
            break;
        case OID_AREA_SCAN:
            area_str = "SCAN";
            break;
        case OID_AREA_FAX:
            area_str = "FAX";
            break;
        case OID_AREA_NETWORK:
            area_str = "NETWORK";
            break;
        case OID_AREA_WIFI:
            area_str = "WIFI";
            break;
        case OID_AREA_STRING_MGR:
            area_str = "STRING_MGR";
            break;
        case OID_AREA_MEMORY:
            area_str = "MEMORY";
            break;
        case OID_AREA_PLATFORM:
            area_str = "PLATFORM";
            break;
        case OID_AREA_STATUS:
            area_str = "STATUS";
            break;
        case OID_AREA_HTTP:
            area_str = "HTTP";
            break;
        case OID_AREA_REM_STORAGE:
            area_str = "REM_STORAGE";
            break;
        case OID_AREA_CONSUMABLE:
            area_str = "CONSUMABLE";
            break;
        case OID_AREA_USB_DEVICE:
            area_str = "USB_DEVICE";
            break;
        case OID_AREA_FEATURE_SUPPORT:
            area_str = "FEATURE";
            break;
        case OID_AREA_GCPP:
            area_str = "GCPP";
            break;
        case OID_AREA_SNMP:
            area_str = "SNMP";
            break;
        default:
            area_str = "unknown";
    }
    return area_str;
}


static char *oid_debug_map_type( uint8_t type )
{
    char *type_str;
    switch ( type )
    {
        case OID_TYPE_STRING:
            type_str = "STRING";
            break;
        case OID_TYPE_UINT32:
            type_str = "UINT32";
            break;
        case OID_TYPE_BYTE_ARRAY:
            type_str = "BYTE_ARRAY";
            break;
        default:
            type_str = "unknown";
            break;
    }
    return type_str;
}

static void oid_debug_list_oids(char *namespace_match, char *area_match, char *type_match)
{
    tree_node_t * namespace;
    tree_node_t * module;
    tree_node_t * value;
    tree_node_t * type;
    tree_node_t * node;
    oid_callback_tree_node_t * callback_entry;

    /* iterate through the namespace level */ 
    namespace = tree_child_get_first( &oid_registration_tree );
    while ( NULL != namespace )
    {
        module = tree_child_get_first( namespace );
        while ( NULL != module )
        {
            /* iterate through the value level */
            value = tree_child_get_first( module );
            while ( NULL != value )
            {
                /* iterate through the type level */
                type = tree_child_get_first( value );
                while ( NULL != type )
                {
                    /* pull the oid_callback_tree_node_t out based on the child */
                    node = tree_child_get_first( type );
                    callback_entry = tree_node_data_get( node, oid_callback_tree_node_t, tree_node);
					
					if (callback_entry != NULL)
					{
                        int mapped_id = (int)oid_value_id_get( callback_entry->oid );
                        char *mapped_namespace = oid_debug_map_namespace( oid_namespace_get( callback_entry->oid ) );
                        char *mapped_area = oid_debug_map_area( oid_area_id_get( callback_entry->oid ) );
                        char *mapped_type = oid_debug_map_type( oid_type_id_get( callback_entry->oid ) );

						if ( (namespace_match == NULL || strcasecmp(namespace_match, mapped_namespace) == 0) &&
						     (area_match == NULL || strcasecmp(area_match, mapped_area) == 0) &&
						     (type_match == NULL || strcasecmp(type_match, mapped_type) == 0) )
						{
							cmd_printf( "%#08X: %-15s - %-15s - %5d - %s\n",
										(unsigned int)callback_entry->oid,
										mapped_namespace,
										mapped_area,
										mapped_id,
										mapped_type );
						}
					}

                    type = tree_child_get_next( value, type );
                }
                value = tree_child_get_next( module, value );
            }
            /* get the next child in the tree */
            module = tree_child_get_next( namespace, module );
        }
        /* get the next child in the tree */
        namespace = tree_child_get_next( &oid_registration_tree, namespace );
    }
}

static error_type_t oid_debug_set( uint32_t oid, uint32_t index, char *valueString)
{
    error_type_t status = OK;

	switch ( oid_type_id_get( oid ) )
	{
		case OID_TYPE_STRING:
		{
			status = oid_set_string(oid, index, valueString, strlen(valueString));		
		}
		break;
		
		case OID_TYPE_UINT32:
		{
			uint32_t value = (uint32_t)cmd_atoi( valueString );
			status = oid_set_uint32(oid, index, value);
		}
		break;
			
		default: // unsupported oid type
            cmd_printf("Error: Unsupported OID type\n");
			status = OID_ERROR_OID_INVALID;
			break;
	}
	
	return status;
}

static error_type_t oid_debug_get( uint32_t oid, uint32_t index, char *valueString, uint32_t valueLength)
{
    error_type_t status = OK;

	switch ( oid_type_id_get( oid ) )
	{
		case OID_TYPE_STRING:
		{
			status = oid_get_string(oid, index, valueString, valueLength);		
		}
		break;
		
		case OID_TYPE_UINT32:
		{
			uint32_t value;
			status = oid_get_uint32(oid, index, &value);
			if(OK==status)
			{
				minNSprintf( valueString, valueLength, "%d", value );
			}
		}
		break;
		
		default: // unsupported oid type
            cmd_printf("Error: Unsupported OID type\n");
			status = OID_ERROR_OID_INVALID;
			break;
	}
	
	return status;
}

static const char *oid_cmd_list_desc = "list all the oids, and their type";
static const char *oid_cmd_list_usage = "[<namespace> <area> <type>]";
static const char *oid_cmd_list_notes = NULL;
static int oid_cmd_list_cb( int argc, char *argv[ ] )
{
	char *namespace = NULL;
	char *area = NULL;
	char *type = NULL;
	
    if ( argc > 4 )
    {
        return CMD_USAGE_ERROR;
    }
	
	if (argc > 1)
	{
		namespace = argv[1];
	}
	
	if (argc > 2)
	{
		area = argv[2];
	}
	
	if (argc > 3)
	{
		type = argv[3];
	}

    oid_debug_list_oids(namespace, area, type);

    return CMD_OK;
}

#define BASE_10 10
static const char *oid_cmd_getset_desc = "Get or set an oid";
static const char *oid_cmd_getset_usage = "<oid name> <index> [<new value>]";
static const char *oid_cmd_getset_notes = NULL;
static int oid_cmd_getset_cb(int argc, char* argv[]) 
{
    oid_t oid;
    char* retString;
    error_type_t status;
    uint32_t index;

    if ( ( argc != 3 ) && ( argc != 4 ) )
    {
        return CMD_USAGE_ERROR;
    }

    oid = (oid_t)cmd_atoi( argv[1] );
    index = (uint32_t)cmd_atoi( argv[2] );

    /* set */
    if( argc == 4 ) 
    {
		status = oid_debug_set( oid, index, argv[3] );
        if(OK != status )
        {
            cmd_printf("OID Set failed when it tried to set %s Retaining the old oid value \n", argv[3]);
            return CMD_ERROR;
        }
    }

    retString = MEM_MALLOC( OID_MAX_STRING_SIZE_IN_BYTES);
	
	status = oid_debug_get( oid, index, retString, OID_MAX_STRING_SIZE_IN_BYTES );
    if(OK==status)
    {
        cmd_append_result("%s", retString);
        MEM_FREE_AND_NULL( retString);    
        return CMD_OK;
    }
    else 
    {
		switch (status)
    {
			case OID_ERROR_ILLEGAL_CHARS:
				cmd_printf("Error: Illegal characters found in string");
				break;
				
			case OID_ERROR_VALUE_TOO_BIG:
				cmd_printf("Error: Value is too big");
				break;
				
			case OID_ERROR_VALUE_TOO_SMALL:
				cmd_printf("Error: Value is too small");
				break;
				
			case OID_ERROR_VALUE_INVALID:
				cmd_printf("Error: Invalid value");
				break;
				
			case OID_ERROR_INDEX_INVALID:
				cmd_printf("Error: Invalid index");
				break;
				
			case OID_ERROR_BUFFER_TOO_SMALL:
				cmd_printf("Error: Buffer too small");
				break;
				
			case OID_ERROR_NOT_IMPLEMENTED:
				cmd_printf("Error: Test routine has not been implemented for this OID");
				break;
				
			case OID_ERROR_VALUE_CAPPED:
				cmd_printf("Error: String answer too long for the buffer");
				break;
				
			case OID_ERROR_OID_INVALID:
				cmd_printf("Error: Invalid OID code.");
				break;
				
			default:
				cmd_printf("Error: Bad OID or other error (err %d)", status);
				break;
    }
    }      
	
    MEM_FREE_AND_NULL( retString);
	
    return CMD_ERROR;
}

#endif


