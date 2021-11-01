/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#include "scos.h"

#include "list.h"           
#include "lassert.h"

#include "scancore.h"
#include "scantypes.h"
#include "scantools.h"

/**************************************************
 * Function name   : count_list
 *    returns      : number of elements in the list
 *    arg1         : pointer to an ATLISTENTRY dbl linked list
 * Created by      : David Poole    
 * Date created    : Jan-05
 * Description     : 
 *      
 *  Counts the number of elements in a doubly linked list.
 *
 * Notes           : 
 *
 *  This function is not ATListCount() in list.c because I wanted the fail-safe
 *  infinite loop prevention of the assert in the counting loop. I know about
 *  how many elements I expect in my own lists. I can't say that for all
 *  possible lists we might ever create.
 *
 **************************************************/

int 
count_list( ATLISTENTRY *pListHead )
{
    int cnt;
    ATLISTENTRY *pListCurr;

    /* is the list empty? */
    if( pListHead->nextEntry==pListHead ) {
        return 0;
    }

    /* we have at least one */
    cnt=0;

    pListCurr = pListHead;
    pListHead = pListHead->nextEntry;
    do {
//        dbg1(("%d 0x%x\n", __LINE__, pListHead ));

        cnt++;
        pListHead = pListHead->nextEntry;

        /* 8-Feb-05 ; I don't know if this is big enough or too big but I need
         * a way to stop dead if there is a list problem. (I'm getting some
         * loops in my list when I add an element onto >1 list.)
         */
        XASSERT( cnt<65535, cnt );

    } while( pListHead != pListCurr );

    return cnt; 
}

