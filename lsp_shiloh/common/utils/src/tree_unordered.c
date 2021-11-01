/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file tree_unordered.c
 *
 * \brief This file implements the API for creating and using a tree data
 * structure within the system.
 *
 * Some features specific to this implementation:
 *
 *   - A tree's children must all have a unique data value
 *
 *   - Trees can have as many children nodes as desired provided they all
 *   have unique values
 * 
 **/

#include <stdint.h>

#include "ATypes.h"
#include "list.h"
#include "tree_unordered.h"
//#include "memAPI.h"
#include "ATypes.h" 
//#include "dprintf.h"
//#include "logger.h"
#include "lassert.h"


/*!< data used to verify nodes have not been corrupted and are initialized */
#define TREE_COOKIE 0x54524545


/**
 *
 * \brief Initializes a head tree node.
 *
 * \param node A pointer to the head tree node to be initialized.
 * The node must be an uninitialized tree_node_t and must NOT be null.
 *
 * Memory is managed by the caller/user of the API.
 *
 */
void tree_init( tree_node_t * node )
{
    ASSERT( NULL != node );
    ATInitNode( &(node->siblings) );
    ATInitList( &(node->children) );
    node->cookie = TREE_COOKIE;
}


/**
 *
 * \brief Initialize and add a child node to a parent tree node
 *
 * \param parent A valid tree_node_t pointer that has been initialized by
 * one of the tree API functions such as tree_init() or tree_child_add()
 *
 * \param child A pointer to the child to be associated with the parent.
 * The child must be an uninitalized tree_node_t and must NOT be null.
 * 
 * Memory is managed by the caller/user of the API.
 *
 */
void tree_child_add( tree_node_t * parent, tree_node_t * child )
{

    ASSERT( NULL != parent );
    ASSERT( TREE_COOKIE == parent->cookie );
    ASSERT( NULL != child );

    /* create the child */
    tree_init(child);
    ASSERT( TREE_COOKIE == child->cookie );

    /* add the child to the parent's tree */
    ATInsertTailList( &( parent->children ), &( child->siblings ) );
}


/**
 * 
 * \brief Return the first child node associated with the parent or NULL
 * if the parent has no children.
 *
 * \param parent The parent tree to search for the child
 *
 * \return tree_node_t * A pointer to the child associated with the parent 
 * or NULL if there was no child found to be associated with the parent
 *
 */
tree_node_t * tree_child_get_first( tree_node_t * parent )
{
    ATLISTENTRY * node;
    tree_node_t * first_child;

    ASSERT( NULL != parent );
    ASSERT( TREE_COOKIE == parent->cookie );

    first_child = NULL;

    /* get the parent's first child link */
    node = ATListHead( &( parent->children ) );

    if ( node != NULL )
    {
        /* pull the tree_node_t out based on the node using the list
         * macro, the node value retreived from the parent's child list
         * is the child's "sibling" linked list value */
        first_child = CONTAINING_RECORD( node, tree_node_t, siblings );
        ASSERT( NULL != first_child );
        ASSERT( TREE_COOKIE == first_child->cookie );
    }

    return first_child;
}


/**
 * 
 * \brief Return the next child node associated with the parent or NULL
 * if the parent has no more children.
 *
 * \param parent The parent of the current node
 *
 * \param current The child to get the next sibling from
 *
 * \return tree_node_t * A pointer to the child associated with the parent's
 * next child or NULL if there was no next child found to be associated with
 * the parent
 *
 */
tree_node_t * tree_child_get_next( tree_node_t * parent, tree_node_t * current )
{
    ATLISTENTRY * node;
    tree_node_t * next_child;

    ASSERT( NULL != parent );
    ASSERT( TREE_COOKIE == parent->cookie );
    ASSERT( NULL != current );
    ASSERT( TREE_COOKIE == current->cookie );

    next_child = NULL;

    /* get the parent's first child link */
    node = ATListNext( &( parent->children ), &( current->siblings ) );

    if ( node != NULL )
    {
        /* pull the tree_node_t out based on the node using the list
         * macro, the node value retreived from the parent's child list
         * is the child's "sibling" linked list value */
        next_child = CONTAINING_RECORD( node, tree_node_t, siblings );
        ASSERT( NULL != next_child );
        ASSERT( TREE_COOKIE == next_child->cookie );
    }

    return next_child;
}


