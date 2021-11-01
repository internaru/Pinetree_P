/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <string.h>
#include "memAPI.h"
#include "debug.h"
#include "logger.h"
#include "net_logger.h"

#include "ipp.h"
#include "ipp_dbg.h"
#include "ipp_cntxt.h"
#include "ipp_request.h"

#define _MAGIC1 (0x34e1ac80)
#define _MAGIC2 (0xfe20b237)


//=======================================================================================
// Debug configuration
//=======================================================================================

#define DBG_PRFX "IPP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_IPP 

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)

#if DEBUG
unsigned int g_ipp_malloc_count = 0;
void *ipp_malloc(const char *func_name, unsigned int line, unsigned int size)
{
    void *ptr = MEM_MALLOC(size);
    
    g_ipp_malloc_count++; 
    DBG_ERR("%s:%d - malloc 0x%x for %d bytes, outstanding count = %d\n", func_name, line, ptr, size, g_ipp_malloc_count);
    return ptr; 
}

void *ipp_realloc(const char *func_name, unsigned int line, void *ptr, unsigned int size)
{
    if (ptr != NULL)
    {
        g_ipp_malloc_count--; 
        DBG_ERR("%s:%d - re-malloc recycle 0x%x, outstanding count = %d\n", func_name, line, ptr, g_ipp_malloc_count);
    }
    
    ptr = MEM_REALLOC(ptr, size);
    
    g_ipp_malloc_count++; 
    DBG_ERR("%s:%d - re-malloc 0x%x for %d bytes, outstanding count = %d\n", func_name, line, ptr, size, g_ipp_malloc_count);
    return ptr; 
}

void *ipp_calloc(const char *func_name, unsigned int line, unsigned int blocks, unsigned int bytes)
{
    void *ptr = MEM_CALLOC(blocks, bytes);
    
    g_ipp_malloc_count++; 
    DBG_ERR("%s:%d - calloc 0x%x for %d bytes, outstanding count = %d\n", func_name, line, ptr, (blocks*bytes), g_ipp_malloc_count);
    return ptr; 
}

void ipp_free(const char *func_name, unsigned int line, void *ptr) 
{ 
    g_ipp_malloc_count--; 
    DBG_ERR("%s:%d - free 0x%x, outstanding count = %d\n", func_name, line, ptr, g_ipp_malloc_count);
    MEM_FREE_AND_NULL(ptr); 
}

#endif


static int _validate_cntxt(ipp_cntxt_t* ptr)
{
  /* Return 0 if ptr is a valid ipp_cntxt_t* */
  return !ptr ||
         (ptr->magic1 != _MAGIC1) ||
         (ptr->magic2 != _MAGIC2);
}

static char* _strdup(const char* str)
{
  char* d;
  if (!str) return NULL;
  int l = strlen(str);
  if (!(d = (char*)IPP_MALLOC(l+1))) return NULL;
  memcpy(d, str, l);
  d[l] = 0;
  return d;
}

struct ipp_context_t* ipp_create_context(const char*  protocol,
                                         const char*  server_address,
                                         unsigned int port_number,
                                         const char*  root_uri)
{
  /* JSA all sorts of checks to make sure that paramters are provided and are
         reasonable */
  if (!protocol       || !*protocol)       return NULL;
  if (!server_address || !*server_address) return NULL;
  if (!port_number)                        return NULL;
  if (!root_uri       || !*root_uri)       return NULL;

  ipp_cntxt_t* c = (ipp_cntxt_t*)IPP_MALLOC(sizeof(*c));
  if (!c) return NULL;

  memset(c, 0, sizeof(*c));
  c->protocol       = _strdup(protocol);
  c->server_address = _strdup(server_address);
  c->root_uri       = _strdup(root_uri);
  c->port_number    = port_number;

  if (!c->protocol || !c->server_address || !c->root_uri)
  {
    if (c->protocol)       IPP_FREE(c->protocol);
    if (c->server_address) IPP_FREE(c->server_address);
    if (c->root_uri)       IPP_FREE(c->root_uri);
    IPP_FREE(c);
    return NULL;
  }

  c->magic1         = _MAGIC1;
  c->magic2         = _MAGIC2;
  return c;
}

int ipp_handle_request(struct ipp_context_t* context,
                       const char* uri,
                       ipp_reader_t reader)
{
  if (_validate_cntxt(context)) return -1;
  if (!reader)       return -2;
  if (!uri || !*uri) return -3;

  context->uri = uri;
  context->reader = reader;
  return ipp_request(context);
}

void ipp_destroy_context(struct ipp_context_t* context)
{
  if (_validate_cntxt(context)) return;
  
  /* Free the request attribute groups */
  ipp_free_attr_groups(context->reqattr, 1);

  /* Free the response attribute groups */
  ipp_free_attr_groups(context->respattr, 1);
  
  IPP_FREE(context->root_uri);
  IPP_FREE(context->server_address);
  IPP_FREE(context->protocol);
  context->magic1 = context->magic2 = 0;
  IPP_FREE(context);
}
