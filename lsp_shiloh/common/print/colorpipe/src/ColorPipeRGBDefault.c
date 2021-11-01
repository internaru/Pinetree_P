/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file ColorPipeRGBDefault.c
 *
 *
 *****************************************************************************/
#include "ATypes.h"
#include "agJobUtils.h"
#include "ColorPipeTables.h"
#include "ColorPipe.h"
#include "ColorPipeRGBPriv.h"

static color_pipe_t my_colorpipes[1];

static color_pipe_t *mycolorpipes[] = { 
    &my_colorpipes[0], 
    0
};


void color_pipe_local_init( color_pipe_t ** color_pipe )
{
    *color_pipe = mycolorpipes[0];
}
