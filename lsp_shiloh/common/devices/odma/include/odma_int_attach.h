/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef ODMA_INT_ATTACH_H
#define ODMA_INT_ATTACH_H

#include <stdint.h>
#include <stdbool.h>


/** 
 * Attach the image pipe interrupt to this module's isr. This is provided
 * as a hack for the AAAA mono copy demo to allow this driver to coexist
 * with the YYYYY PIP code.
**/
void odma_int_attach(void);

#endif
