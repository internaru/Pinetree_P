/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* In ThreadX firmware we have a simple range check on RAM addresses. 
 * In Kernel-land, I think the MMU will catch our screwups (?)
 */

#ifndef DEBUG_H
#define DEBUG_H

#define IS_PTR_VALID(p)  1
#define IS_DMAPTR_VALID(p)  1

#endif

