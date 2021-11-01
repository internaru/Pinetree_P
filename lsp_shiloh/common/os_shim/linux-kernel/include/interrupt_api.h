/*
 **************************************************************************************
 *
 * Copyright (c) 2011, 2013 Marvell International, Ltd.
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


#ifdef __KERNEL__
#include <linux/interrupt.h>
typedef irqreturn_t (*PFN_ISR)( int irq, void* context);
void intAttach( uint32_t int_num, uint32_t flags, PFN_ISR handler, const char* dev_name, void* context );
void intDetach( uint32_t int_num, void* context );
#else
typedef void (*PFN_ISR)( uint32_t input );
void intAttach( uint32_t int_num, uint32_t priority, PFN_ISR handler, uint32_t handler_input );
void intDetach( uint32_t int_num );
#endif

bool intEnable( uint32_t int_num );
bool intDisable( uint32_t int_num );

#define IN_INTERRUPT in_interrupt

