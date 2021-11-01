/*
 **************************************************************************************
 *
 * Copyright (c) 2004-2010, 2013 Marvell International, Ltd.
 *
 **************************************************************************************
 *
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell as part of a proprietary software release,
 * the File is considered Marvell Proprietary and Confidential Information, and is
 * licensed to you under the terms of the applicable Commercial License.
 *
 **************************************************************************************
 *
 * Marvell GPL License Option
 *
 * If you received this File from Marvell as part of a Linux distribution, this File
 * is licensed to you in accordance with the terms and conditions of the General Public
 * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
 * modify it under the terms of the GPL License; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GPL License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 *
 **************************************************************************************
*/

 /**
  * \file list.h
  *
  * \brief This is the interface header file for list.c
  *
  */

#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * This macro used to be written as: 
 * \code
 * #define CONTAINING_RECORD(address, type, field) ((type *)((char *)(address) 
 * - offsetof((type), (field)))) 
 * \endcode
 * But, the arm compiler could not deal with having the offsetof macro in the
 * CONTAINING_RECORD macro so it was rewritten as below which just directly
 * includes the offsetof code.
 *
 * \warning Address must be an address not a function returning a pointer.
 * NULL address returns NULL record.
 */   
#define CONTAINING_RECORD(address, type, field) ((address) ? ((type*)( ((char*)(address)) - ((unsigned int)((char *)&(((type *)0)->field) - (char *)0)))) : 0 )


/** This struct serves as both the list head and the list nodes */
typedef struct ATLISTENTRY_s ATLISTENTRY;
struct ATLISTENTRY_s
{
    ATLISTENTRY* nextEntry;     ///< Next node on list
    ATLISTENTRY* prevEntry;     ///< Previous node on list
};

///< Initialize a node.
void ATInitNode(ATLISTENTRY* pNode);

///< Initialize a list.
void ATInitList(ATLISTENTRY* pListHead);

///< Return true if list is empty.
bool ATIsListEmpty(ATLISTENTRY* pListHead);

///< Insert as the first element of a list.
void ATInsertHeadList(ATLISTENTRY* pListHead, ATLISTENTRY* pNode);

///< Insert as the last element of a list.
void ATInsertTailList(ATLISTENTRY* pListHead, ATLISTENTRY* pNode);

///< Remove and return the first element of the list.
ATLISTENTRY* ATRemoveHeadList(ATLISTENTRY* pListHead);

///< Remove and return the last element of a list.
ATLISTENTRY* ATRemoveTailList(ATLISTENTRY* pListHead);

///< This will remove the element from a list, any list it happens to be on.
void ATRemoveEntryList(ATLISTENTRY* pNode);

///< Return pointer to first element on list.
ATLISTENTRY* ATListHead(ATLISTENTRY* pListHead);
#define ATFirstListEntry ATListHead

///< Return pointer to the element following this one.
ATLISTENTRY* ATListNext(ATLISTENTRY* pListHead, ATLISTENTRY *pNode);
#define ATNextListEntry ATListNext

///< Return pointer to the element before this one.
ATLISTENTRY* ATListPrev(ATLISTENTRY* pListHead, ATLISTENTRY *pNode);
#define ATPrevListEntry ATListPrev

///< Return pointer to last element in list.
ATLISTENTRY* ATListTail(ATLISTENTRY* pListHead);
#define ATLastListEntry ATListTail

///< Insert element after this one.
void ATListInsertAfter(ATLISTENTRY *after, ATLISTENTRY *pNode);

///< Insert element before this one.
void ATListInsertBefore(ATLISTENTRY *before, ATLISTENTRY *pNode);


///< Run a for loop to count the number of elements in list.
uint32_t ATNumListNodes(ATLISTENTRY* pListHead);

#ifdef __cplusplus
}
#endif

#endif

