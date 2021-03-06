/*
 * ============================================================================
 * (C) Copyright 2014   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * @file map_api.h
 *
 * @brief Exposes an associative array, or map, implemented using a hash table providing
 * nominal O(1) lookup.
 */

#ifndef MAP_API_H
#define MAP_API_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Map return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
typedef int32_t map_error_t;

#define MAP_DUP_KEY          -11  ///< The operation failed because the requested key is a duplicate.
#define MAP_LOW_MEM          -12  ///< The operation failed because system memory is low. A future attempt may succeed. 

//@}

/**
 * @brief map instance handle
 * 
 * Opaque handle generated by map_create() used to reference a particular map instance in 
 * successive API calls. 
 */
typedef struct map_context_s *map_handle_t;
#define MAP_INVALID_HANDLE  0

/**
 * @brief map iterator handle
 * 
 * Opaque handle generated by map_iterate_begin() used to reference a particular map 
 * iterator in successive API calls. 
 */
typedef struct map_iterator_context_s *map_iterator_t;
#define MAP_INVALID_ITERATOR    0

/**
 * @brief Initialize the map module.
 *
 * @retval SYS_OK Initialization was successful.
 * 
 * @retval SYS_FAIL Initialization failed. 
 *  
 * This routine should be called exactly once following a system startup.
 */
int32_t map_init(void);

/**
 * @brief Instantiate an associative array.
 * 
 * @param flags Currently unused.
 * 
 * @return A unique handle used to reference the associative array in successive API 
 *         calls.
 *  
 * An associative array must be created prior to performing any operations. Multiple 
 * instances may be created and used simultaneously. 
 */
map_handle_t map_create(uint32_t flags);
           
/**
 * @brief Insert a new key-value pair into an associative array.
 *  
 * @param map Handle to the associative array returned in the call to map_create().
 *  
 * @param key Pointer to storage containing a unique key assigned by the user. The user 
 *            must not deallocate or modify the contents of the key after making this
 *            call.
 *  
 * @param key_len The length of the key in bytes.
 *  
 * @param value Pointer to storage containing the value to be bound to the key and stored
 *              in the associative array.
 *  
 * @retval SYS_OK The operation was successful.
 * @retval MAP_DUP_KEY Key is a duplicate. A matching key already exists in the 
 *         associative array.
 * @retval MAP_LOW_MEM The operation failed because system memory is low.
 *  
 * The key may be of any length or data type but must be unique for the given associative 
 * array instance. If the key is not unique then the request will be ignored. 
 *  
 * The associative array owns the key until the user removes the key-value pair or 
 * destroys the associative array. Once a key-value pair has been added, do not deallocate
 * or modify the value of the key. If you need to modify the key, first remove the entry 
 * from the associative array, change the key, and then re-add it. 
 *  
 * The data pointed to by the 'value' param is treated as opaque data and there are no 
 * constraints on its contents, data type, or size. For example the 'value' param may 
 * point to a C structure with the key included as one of the structure's members. 
 *  
 * The value is not copied and must not be deallocated until after it has been removed
 * from the associative array. The user may, however, modify the contents of the value 
 * while it is stored in the associative array as long as the associated key remains 
 * unchanged. 
 *  
 * This operation is thread-safe. 
 */
int32_t map_insert(map_handle_t map, const void *key, uint32_t key_len, const void *value);

/**
 * @brief Lookup the value bound to a given key.
 *  
 * @param map Handle to the associative array returned in the call to map_create().
 *  
 * @param key Pointer to storage containing the key.
 *  
 * @param key_len The length of the key in bytes.
 *  
 * @return A pointer to the value bound to the given key, or NULL if the key was not 
 *         found.
 *  
 * This operation is thread-safe. 
 */
void *map_lookup(map_handle_t map, const void *key, uint32_t key_len);

/**
 * @brief Remove the entry associated with the given key.
 *  
 * @param map Handle to the associative array returned in the call to map_create().
 *  
 * @param key Pointer to storage containing the key.
 *  
 * @param key_len The length of the key in bytes.
 *  
 * @return A pointer to the value that was removed, or NULL if the key was not found. 
 *  
 * If the return value is non-NULL then the key has been unbound from the value and the 
 * key-value pair has been removed from the associative array. Ownership is retured to the 
 * user, and the user may modify or deallocate storage containing the key or value. 
 *  
 * This operation is thread-safe. 
 */
void *map_remove(map_handle_t map, const void *key, uint32_t key_len);

/**
 * @brief Destroy the map including removal of any remaining key-value pairs.
 *  
 * @param map Handle to the associative array returned in the call to map_create().
 *  
 * @return none
 *  
 * Following this call the map handle must be discarded and must not be reused. Any 
 * key-value pairs remaining in the associative array will be removed. Ownership is 
 * retured to the user, and the user may modify or deallocate storage containing the keys 
 * or values. 
 *  
 * This operation is NOT thread-safe. Before making this call the user must ensure that no
 * other operations are currently in progress on other threads and that no subsequent 
 * operations will be initiated. 
 */
void map_destroy(map_handle_t map);

// iterates over items in the same order in which they were inserted into the table
// map_iterate_begin() automatically resets iterator to first enstry so no need to call map_iterate_first()
map_iterator_t map_iterate_begin(map_handle_t map);

// reset iterator to first item
// NOTE this is not necessary following map_iterate_begin()
void *map_iterate_first(map_iterator_t iterator);

void *map_iterate_next(map_iterator_t iterator);

 // free iterator resources - must match a prior call to map_iterate_begin()
void map_iterate_end(map_iterator_t iterator);

#ifdef __cplusplus
}
#endif

#endif  /* MAP_API_H */

