/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


void cpu_spin_delay(uint32_t delayInUs);

void cpu_dcache_writeback_region( void *start_addr, uint32_t size_in_bytes );

void cpu_dcache_invalidate_region( void *start_addr, uint32_t size_in_bytes );

uint32_t cpu_get_dcache_line_size( void );

