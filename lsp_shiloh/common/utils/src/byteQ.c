/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <tx_api.h>
#include <stdint.h>
#include <string.h>
#include "memAPI.h"
#include "error_types.h"
#include "byteQ_api.h"

typedef struct
{
  char*        name;
  uint8_t*     mem;
  unsigned int memsize, putloc, getloc;
  TX_SEMAPHORE putsem, getsem;
} _hdl_t;

void* byteQ_create(const char* name, unsigned int size)
{
  _hdl_t* h;

  if (!name || !*name || !size)                   return NULL;
  if (!(h = (_hdl_t*)MEM_MALLOC(sizeof(_hdl_t)))) return NULL;
  memset(h, 0, sizeof(*h));
  if (!(h->mem  = (uint8_t*)MEM_MALLOC(size)))
  {
    MEM_FREE_AND_NULL(h);
    return NULL;
  }
  if (!(h->name = (char*)MEM_MALLOC(strlen(name)+1)))
  {
    MEM_FREE_AND_NULL(h->mem);
    MEM_FREE_AND_NULL(h);
    return NULL;
  }
  strcpy(h->name, name);
  h->memsize = size;
  tx_semaphore_create(&h->putsem, h->name, h->memsize);
  tx_semaphore_create(&h->getsem, h->name, 0);
  return h;
}

error_type_t byteQ_destroy(void* hdl)
{
  _hdl_t* h = (_hdl_t*)hdl;
  if (!h) return FAIL;
  tx_semaphore_delete(&h->putsem);
  tx_semaphore_delete(&h->getsem);
  MEM_FREE_AND_NULL(h->mem);
  h->mem = NULL; /* Just in case the above neglects to do so */
  MEM_FREE_AND_NULL(h->name);
  MEM_FREE_AND_NULL(h);
  return OK;
}

error_type_t byteQ_putbyte(void* hdl, uint8_t byte, int timeout)
{
  ULONG tx_timeout;
  _hdl_t* h = (_hdl_t*)hdl;
  if      (timeout  < 0) tx_timeout = TX_WAIT_FOREVER;
  else if (timeout == 0) tx_timeout = TX_NO_WAIT;
  else                   tx_timeout = timeout;
  if (!h || !h->mem) return FAIL;
  if (TX_SUCCESS == tx_semaphore_get(&h->putsem, tx_timeout))
  {
    TX_INTERRUPT_SAVE_AREA
    TX_DISABLE
    h->mem[h->putloc++] = byte;
    if (h->putloc >= h->memsize) h->putloc = 0;
    TX_RESTORE
    tx_semaphore_put(&h->getsem);
    return OK;
  }
  return FAIL;
}

error_type_t byteQ_getbyte(void* hdl, uint8_t* byte, int timeout)
{
  ULONG tx_timeout;
  _hdl_t* h = (_hdl_t*)hdl;
  if      (timeout  < 0) tx_timeout = TX_WAIT_FOREVER;
  else if (timeout == 0) tx_timeout = TX_NO_WAIT;
  else                   tx_timeout = timeout;
  if (!h || !h->mem || !byte) return FAIL;
  if (TX_SUCCESS == tx_semaphore_get(&h->getsem, tx_timeout))
  {
    TX_INTERRUPT_SAVE_AREA
    TX_DISABLE
    *byte = h->mem[h->getloc++];
    if (h->getloc >= h->memsize) h->getloc = 0;
    TX_RESTORE
    tx_semaphore_put(&h->putsem);
    return OK;
  }
  return FAIL;
}
