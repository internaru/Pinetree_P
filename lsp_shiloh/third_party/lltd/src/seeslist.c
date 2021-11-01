/*
 * LICENSE NOTICE.
 *
 * Use of the Microsoft Windows Rally Development Kit is covered under
 * the Microsoft Windows Rally Development Kit License Agreement,
 * which is provided within the Microsoft Windows Rally Development
 * Kit or at http://www.microsoft.com/whdc/rally/rallykit.mspx. If you
 * want a license from Microsoft to use the software in the Microsoft
 * Windows Rally Development Kit, you must (1) complete the designated
 * "licensee" information in the Windows Rally Development Kit License
 * Agreement, and (2) sign and return the Agreement AS IS to Microsoft
 * at the address provided in the Agreement.
 */

/*
 * Copyright (c) Microsoft Corporation 2005.  All rights reserved.
 * This software is provided with NO WARRANTY.
 */

#include "globals.h"

#include "seeslist.h"

struct topo_seeslist_st
{
    topo_recvee_desc_t	*data;	/* data buffer */
    int			count;	/* how big data buffer is, in topo_recvee_desc_t units */
    int			wr;	/* next free slot for writing to buffer */
    int			rd;	/* next slot to read from buffer */
    /* wr == rd implies buffer empty */
};

#define  CSL  (IF_INFO(RIF)->sees)

topo_seeslist_t *
seeslist_new(int count)
{
    topo_seeslist_t *sl = xmalloc(sizeof(topo_seeslist_t) +
                                    count * sizeof(topo_recvee_desc_t));

    sl->data = (topo_recvee_desc_t*)(sl+1);
    sl->count = count;
    sl->wr = 0;
    sl->rd = 0;

    return sl;
}


bool_t
seeslist_enqueue(bool_t isARP, etheraddr_t *realsrc)
{
    int rif = RIF;
    ASSERT(rif != -1);
    osl_interface_info_t *if_info = IF_INFO(rif);
    ASSERT(if_info != NULL);

    topo_recvee_desc_t *desc;

    if (((CSL->wr + 1) % CSL->count) == CSL->rd)
	return FALSE; /* circular buffer full */

    desc = &CSL->data[CSL->wr];
    CSL->wr = (CSL->wr + 1) % CSL->count;

    desc->rd_type = htons((uint16_t)!!isARP);
    desc->rd_realsrc = *realsrc;
    desc->rd_src = if_info->ethernet_hdr->eh_src;
    desc->rd_dst = if_info->ethernet_hdr->eh_dst;

    return TRUE;
}

bool_t
seeslist_dequeue(/*OUT*/ topo_recvee_desc_t *dest)
{
    if (CSL->rd == CSL->wr)
	return FALSE; /* buffer empty */

    *dest = CSL->data[CSL->rd];
    CSL->rd = (CSL->rd + 1) % CSL->count;

    return TRUE;
}


void
seeslist_clear(void)
{
    CSL->rd = 0;
    CSL->wr = 0;
}

bool_t
seeslist_is_empty(void)
{
    return (CSL->rd == CSL->wr);
}



void
seeslist_free(void)
{
    xfree(CSL);
}
