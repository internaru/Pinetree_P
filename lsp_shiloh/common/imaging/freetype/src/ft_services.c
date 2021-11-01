/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved.
 *
 *                      Marvell Confidential
 * ============================================================================
 */

#include "memAPI.h"
#include "ft_services.h"

/*----------------------------------------------------------------------------*/
void *FT_calloc(size_t nmemb, size_t size)
{
  return MEM_CALLOC(nmemb, size);
}

/*----------------------------------------------------------------------------*/
void  FT_free(void* ptr)
{
  MEM_FREE_IF_NOT_NULL(ptr);
}

/*----------------------------------------------------------------------------*/
void *FT_malloc(size_t size)
{
  return MEM_MALLOC(size);
}

/*----------------------------------------------------------------------------*/
void *FT_realloc(void *ptr, size_t size)
{
  return MEM_REALLOC(ptr, size);
}
