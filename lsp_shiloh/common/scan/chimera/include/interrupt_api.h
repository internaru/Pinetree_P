/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>

typedef void (*PFN_ISR)( uint32_t input );
void intAttach( uint32_t int_num, uint32_t priority, PFN_ISR handler, uint32_t handler_input );

bool intEnable( uint32_t int_num );
bool intDisable( uint32_t int_num );
void intDetach( uint32_t int_num );

