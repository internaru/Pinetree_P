/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * \file tree_unordered.h
 *
 * \brief This file is the API for creating and using a tree data structure
 * within the system.
 *
 * Some features specific to this implementation:
 *   - Trees can have as many children nodes as desired.
 *   - This is an unordered tree, as there is no key or other method
 *   for internal comparison and ordering.
 *   - This implementation is best for small sets of data or a well-layered
 *   approach.
 *   - Any find or search algorithm is to be implemented by the user of
 *   this API.
 * 
 */

#ifndef TREE_UNORDERED_H
#define TREE_UNORDERED_H

#include <stdint.h>
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * \brief The implementation of the abstract data type representing a tree node.
 *
 * A tree node can represent a parent tree, a child node, or a leaf node.
 * Tree nodes are connected to their children and siblings.
 * 
 * The members of this structure are only exposed because of the nature of
 * the data retrieval macro, they should not be used directly.
 *
 *
 */
typedef struct tree_node_s
{
    ATLISTENTRY siblings; /*!< A linked list of siblings to this tree node */

    ATLISTENTRY children; /*!< A linked list of children of this tree node  */
    
    uint32_t cookie; /*!< An identifier to verify the tree node is valid */

} tree_node_t;


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
void tree_init( tree_node_t * node );


/**
 *
 * \brief Initialize and add a child node to a parent tree node.
 *
 * \param parent A valid tree_node_t pointer that has been initialized by
 * one of the tree API functions such as tree_init() or tree_child_add().
 *
 * \param child A pointer to the child to be associated with the parent.
 * The child must be an uninitalized tree_node_t and must NOT be null.
 * 
 * Memory is managed by the caller/user of the API.
 *
 */
void tree_child_add( tree_node_t * parent, tree_node_t * child );


/**
 * 
 * \brief Return the first child node associated with the parent or NULL
 * if the parent has no children.
 *
 * \param parent The parent tree to search for the child.
 *
 * \return tree_node_t * A pointer to the child associated with the parent 
 * or NULL if there was no child found to be associated with the parent.
 *
 */
tree_node_t * tree_child_get_first( tree_node_t * parent );


/**
 * 
 * \brief Return the next child node associated with the parent or NULL
 * if the parent has no more children.
 *
 * \param parent The parent of the current node.
 *
 * \param current The child to get the next sibling from.
 *
 * \return tree_node_t * A pointer to the child associated with the parent's
 * next child or NULL if there was no next child found to be associated with
 * the parent.
 *
 */
tree_node_t * tree_child_get_next( tree_node_t * parent, tree_node_t * current );


/**
 * 
 * \brief Get the data associated with this tree node using the macro.
 *
 * \param tree The tree node associated with the data desired.
 *
 * \param type The c-name of the data type associated with the data to get.
 *
 * \param field The structure field name associated with the tree_node_t.
 *
 * \return void * the data associated with the node.
 *
 * \note
 * - tree must be an address not a function returning a pointer.
 * - NULL tree returns NULL record.
 * - This is the same macro found in "list.h" because this API is based
 * on the "list.h" API.
 * - The "tree" must be instantiated within the data structure you are
 *   retreiving and not just a pointer.
 *
 */
#define tree_node_data_get( tree, type, field) (tree ? ((type*)( ((char*)tree) - ((unsigned int)((char *)&(((type *)0)->field) - (char *)0)))) : NULL )


/** \Example */

#if 0

tree_node_t my_tree;

typedef struct my_data_s 
{
    tree_node_t tree_node;

    uint32_t x;

    uint32_t y;

} my_data_t;

my_data_t data1;
my_data_t data2;

/* We could use the tree as follows: */
do_something_with_data()
{
    my_data_t * p_data;
    tree_node_t * child;

    tree_init( &my_tree );
    tree_child_add( &my_tree, &(data1.tree_node) );
    tree_child_add( &my_tree, &(data2.tree_node) );

    child = tree_child_get_first( &my_tree );

    p_data = tree_node_data_get( child, my_data_t, tree_node);

    p_data->x = 1;
    p_data->y = 1;

    child = tree_child_get_next( &my_tree, child );

    p_data = tree_node_data_get( child, my_data_t, tree_node);

    p_data->x = 2;
    p_data->y = 2;

}

#endif

#ifdef __cplusplus
}
#endif

#endif
