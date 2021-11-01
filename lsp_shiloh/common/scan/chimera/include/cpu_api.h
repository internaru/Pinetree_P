/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef CPU_API_H
#define CPU_API_H

#define IN_INTERRUPT() 0

void cpu_dcache_writeback_region( void *start_addr, uint32_t size_in_bytes );

void cpu_dcache_invalidate_region( void *start_addr, uint32_t size_in_bytes );

void cpu_spin_delay(uint32_t delayInUs);

uint32_t cpu_get_dcache_line_size( void );

#endif

