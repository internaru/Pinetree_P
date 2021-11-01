/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __INC_CMD_CNTXT__H__
#define __INC_CMD_CNTXT__H__

void  cmd_cntxt_init(void);
void* cmd_cntxt_get(void);
void  cmd_cntxt_set(void*);
void  cmd_cntxt_erase(void);

#endif
