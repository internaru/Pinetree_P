/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <pthread.h>
#include <posix_ostools.h>
#include "cmd_private.h"

typedef struct __node_t
{
  pthread_t       id;
  void            *cntxt;
  struct __node_t *next;
} _node_t;

static _node_t* _nodes;

/* -------------------------------------------------------------------------- */
static pthread_mutex_t _sem;
static void _access(int Lock_nUnlock)
{
  if (Lock_nUnlock) pthread_mutex_lock(&_sem);
  else              pthread_mutex_unlock(&_sem);
}

/* -------------------------------------------------------------------------- */
void* cmd_cntxt_get(void)
{
  _node_t* n;
  void* cntxt;
  pthread_t id;

  id = pthread_self(); 
  _access(1);
  for (n=_nodes;n;n=n->next)
    if (n->id == id)
    {
      cntxt = n->cntxt;
      _access(0);
      return cntxt;
    }
  _access(0);
  return (void*)0;
}

/* -------------------------------------------------------------------------- */
void cmd_cntxt_set(void* cntxt)
{
  _node_t* n;
  pthread_t id;

  id = pthread_self(); 
  _access(1);
  for (n=_nodes;n;n=n->next)
    if (n->id == id)
    {
      n->cntxt = cntxt;
      _access(0);
      return;
    }
  if ((n = (_node_t*)cmd_malloc(sizeof(_node_t))))
  {
    n->cntxt = cntxt;
    n->id = id;
    n->next = _nodes;
    _nodes = n;
  }
  _access(0);
}

/* -------------------------------------------------------------------------- */
void cmd_cntxt_erase(void)
{
  _node_t *n, *p;
  pthread_t id;

  id = pthread_self(); 
  _access(1);
  for (p=(void*)0, n=_nodes;
       n;
       p=n, n=n->next)
    if (n->id == id)
    {
      if (p) p->next = n->next;
      else   _nodes  = n->next;
      cmd_free(n);
      break;
    }
  _access(0);
}

/* -------------------------------------------------------------------------- */
void  cmd_cntxt_init(void)
{
  posix_mutex_init(&_sem);
  _nodes = (void*)0;
}
