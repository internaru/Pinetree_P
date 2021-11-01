/* 
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2007
 */

#ifndef _OS_LIST_H
#define _OS_LIST_H

#include "list.h"

//struct list_head : ATLISTENTRY;

struct list_head
{
    struct list_head* next;     // next node on list
    struct list_head* prev;     // previous node on list
};

#define INIT_LIST_HEAD(ptr) ATInitList((ATLISTENTRY*)ptr)

static inline int list_empty(struct list_head *head)
{
    return (int)ATIsListEmpty((ATLISTENTRY*)head);
}

static inline void list_del(struct list_head *entry)
{
    ATRemoveEntryList((ATLISTENTRY*)entry);
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
    ATInsertHeadList((ATLISTENTRY*)head, (ATLISTENTRY*)new);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    ATInsertTailList((ATLISTENTRY*)head, (ATLISTENTRY*)new);
}

/*
//typedef ATLISTENTRY struct list_head;
typedef struct _list_head
{
    int a;
    int b;
}list_head;

struct list_head {
	struct list_head *next, *prev;
};
*/
#endif //_OS_LIST_H

