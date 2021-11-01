/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved.
 *
 *                      Marvell Confidential
 * ============================================================================
 */

#ifndef FT_SERVICES_H
#define FT_SERVICES_H

/* These are the services used by freetype to build. These services are not
   meant to be used by other clients in the system. */

#include <stdlib.h> /* To get size_t */

void*       FT_calloc(size_t nmemb, size_t size);
void        FT_free(void* ptr);
void*       FT_malloc(size_t size);
void*       FT_realloc(void *ptr, size_t size);

#endif /* FT_SERVICES_H */
