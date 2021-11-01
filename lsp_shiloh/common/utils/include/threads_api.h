/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __INC_THREADS_API_H__
#define __INC_THREADS_API_H__

void threads_init(void);
int  threads_set_priority(const char* name, int newpri);

/**
 *
 * \brief Return the current thread's highwater stack value
 *
 * \return the current thread's highwater stack value
 *
 * \note The thread's stack must have been filled with the system's
 * special stack fill value pattern for this value to be meaningful.
 *
 */
uint32_t threads_current_stack_highwater(void);

#endif /* __INC_THREADS_API_H__ */
