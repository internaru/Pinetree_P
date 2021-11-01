/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** Improvements that could be made:
 * 1. Add some form of a key to validate the page and store passed in
 * 2. Add ability to store mono documents as this type
 * 3. ...
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "fax_store_api.h"
#include "lassert.h"
#include "memAPI.h"
#include "dprintf.h"
#include "utils.h"
#include "faxer_api.h"

#define NAME FAXER_STORAGE_DRIVE"/fs_page_%u"
#define NAME_SIZE (20 /*core name */ + 6 /* number */ + 1 /*null*/)

#define MAX_OPEN_PAGES (2)

typedef struct _link_obj _link_obj_t;
struct _link_obj
{
  void *data;
  uint32_t size; /* only used if needed */
  _link_obj_t *next;  
  _link_obj_t *prev;  
};

typedef struct 
{
  char file_name[NAME_SIZE];
  FILE *fileh_read;
  FILE *fileh_write;
  long eof;
} _loc_t; 


typedef struct
{
  _loc_t loc; 
  bool page_completed;
  uint8_t read_open; /* open count */
  uint8_t write_open; /* open count */
} _page_data_t;

/* type returned to caller when opening a page */
typedef struct
{
  bool taken;
  fax_store_mode_t mode;
  fax_store_handle_t store;
  _page_data_t *data;
} _page_obj_t;
/* type returned to caller when creating a fax store */
typedef struct _fax_store_data
{
  fax_store_type_t type;
  _link_obj_t      *pages;   
  uint32_t num_pages;
  _page_obj_t open_pages[MAX_OPEN_PAGES];
} _fax_store_data_t;


_page_obj_t *_fax_store_page_open(fax_store_handle_t store, _page_data_t *page, fax_store_mode_t mode);

/*---------------------------------------*/
fax_store_handle_t fax_store_create(fax_store_creation_t *params)
{
  fax_store_handle_t ret = NULL;
  /* allocate and create a fax store object */
  return(ret); 
}

/*-----------------------------------------*/
/** Remove memory associated with the page */
error_type_t _del_page(_link_obj_t *page_obj, fax_store_type_t type)
{
    /* remove file if it exists */
    /* free the page mem */
    return(OK);
}

/*---------------------------------------*/
error_type_t fax_store_destroy(fax_store_handle_t handle)
{
  return(OK);
}


/*---------------------------------------*/
fax_store_pg_handle_t fax_store_page_open_new(fax_store_handle_t handle, fax_store_mode_t mode)
{
  _page_obj_t *ret = NULL;
  return(ret); 
}

/*---------------------------------------*/
error_type_t fax_store_page_set_read_pos(fax_store_pg_handle_t page, uint32_t byte_offset)
{
  error_type_t ret = OK; 
  return(ret);
}

/*---------------------------------------*/
error_type_t fax_store_page_destroy(fax_store_pg_handle_t page)
{
  error_type_t ret = FAIL; /* non zero */
  return(ret);
}


/*---------------------------------------*/
fax_store_pg_handle_t fax_store_page_open_first(fax_store_handle_t handle, fax_store_mode_t mode)
{
    return(NULL);
}

/*---------------------------------------*/
_page_obj_t *_fax_store_page_open(fax_store_handle_t handle, _page_data_t *page_data, fax_store_mode_t mode)
{
  _page_obj_t *ret = NULL;
  return(ret);
}

/*---------------------------------------*/
error_type_t fax_store_page_close(fax_store_pg_handle_t page)
{
  return(OK);
}

/*---------------------------------------*/
bool fax_store_page_complete(fax_store_pg_handle_t page)
{
  return(OK); 
}

/*---------------------------------------*/
error_type_t fax_store_page_write_data(fax_store_pg_handle_t page, uint8_t *data, uint32_t size, bool completed)
{
  error_type_t ret = OK;
  return(ret);
}


/*---------------------------------------*/
uint32_t fax_store_page_read_data(fax_store_pg_handle_t page, uint8_t *data, uint32_t size, bool *completed)
{
    return(0);
}

/*---------------------------------------*/
uint32_t fax_store_num_pages(fax_store_handle_t handle)
{
  _fax_store_data_t *store = (_fax_store_data_t*)handle;
  return(store->num_pages);
}



/*---------------------------------------*/
void _file_replace_dnl(char *filename)
{
}


/*---------------------------------------*/
error_type_t fax_store_make_jpeg_readable_by_ijg(fax_store_pg_handle_t page)
{
  return(OK);
}
