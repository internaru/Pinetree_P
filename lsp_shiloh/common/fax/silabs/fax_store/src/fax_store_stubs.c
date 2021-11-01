/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

/**\file fax_store_api.h
 * This file defines the fax store api.  The fax store is an object that allows a
 * fax document to have a common access API for clients no matter what the doc is 
 * stored on...RAM FS, heap, limiter, netowrk FS, etc.
 * NOTE: Currently this is only used for color fax documents.
 */
#ifndef __FAX_STORE_H__
#define __FAX_STORE_H__

#include "mlimiter.h"
#include "error_types.h"
#include "stdint.h"

typedef void * fax_store_handle_t;
typedef void * fax_store_pg_handle_t;

typedef enum
{
  FAX_STORE_TYPE_RAMFS,  
} fax_store_type_t;

typedef enum
{
  FAX_STORE_MODE_READ, 
  FAX_STORE_MODE_WRITE,
} fax_store_mode_t;

typedef struct
{
  fax_store_type_t type;
  mlimiter_t *mem_limiter;  /* if NULL, no limiter is used */
  bool stream; /* if stream, the lib will optimize memory usage by deleting when read */
} fax_store_creation_t;

/**\brief Create a fax store object.
 *
 *  Create a fax store object and return a handle to the new object.
 *   
 */
fax_store_handle_t fax_store_create(fax_store_creation_t *params);

/**brief Destroy a fax store object.
 *
 *  Destroy a fax store object.
 *   
 */
error_type_t fax_store_destroy(fax_store_handle_t handle);

/**\brief Create a page within the fax store and open it 
 *
 *  Create a page for the fax store.  The page is added and managed by the fax 
 *  store object. Page is added to the end of the list of pages.  The page is opened 
 *  prior to returning.
 *   
 */
fax_store_pg_handle_t fax_store_page_open_new(fax_store_handle_t handle, fax_store_mode_t mode);

/**\brief Get the first page within the fax store 
 *
 *  The normal use case would be to get the page, process the page, then remove
 *  the page.  This results in the next page in the list being the first page 
 *  in the list.
 *   
 */
fax_store_pg_handle_t fax_store_page_open_first(fax_store_handle_t handle, fax_store_mode_t mode);


/**\brief Destory a fax page.
 *
 * Closes and deletes the page.  Removes the page form the list of pages.  
 * Deallocates/closes any resources that are required for the page.
 *   
 */
error_type_t fax_store_page_destroy(fax_store_pg_handle_t page);

/**\brief Close the page 
 * The page handle is invalid after this call returns and should not be used for future calls. 
 */
error_type_t fax_store_page_close(fax_store_pg_handle_t page);


/**\brief Return true if the page data is complete 
*
*/
bool fax_store_page_complete(fax_store_pg_handle_t page);

/**\brief Write page data.
 *
 * Write page data to the page.  Data is added to the end.
 *   
 */
error_type_t fax_store_page_write_data(fax_store_pg_handle_t page, uint8_t *data, uint32_t size, bool completed);

/**\brief Read data from the page 
 *
 * Read page data.  Similar to reading a file, the read starts from just after the previous read.   
 * If stream and type is MEM, the memory will be freed when read.
 *   
 */
uint32_t fax_store_page_read_data(fax_store_pg_handle_t page, uint8_t *data, uint32_t size, bool *completed);

/**\brief Reset read markers to begining of the file
 *
 * Resets the read markers to the begining of the file. 
 */
error_type_t fax_store_page_set_read_pos(fax_store_pg_handle_t page, uint32_t offset);

/**\brief Get the number of pages in the store
 *
 */
uint32_t fax_store_num_pages(fax_store_handle_t handle);

/**\brief Make the jpeg readable by ijg
 * 
 * IJG lib does not support DNL tag.
 */
error_type_t fax_store_make_jpeg_readable_by_ijg(fax_store_pg_handle_t page);

#endif 
