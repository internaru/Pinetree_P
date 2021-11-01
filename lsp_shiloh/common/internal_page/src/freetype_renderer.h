/*
 *
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
*/  

#ifndef FREETYPERENDERER_H
#define FREETYPERENDERER_H

#include "renderer.h"

#ifdef __cplusplus
extern "C" {
#endif

void FreetypeTextRenderer(RendererSession *pRSession, 
                            unsigned char *Strip, 
                            DISP_LIST *TempPage, 
                            RENDER_INPUT *RendInput, 
                            UINT32 Color, 
                            UINT32 Rotate,
                            bool *strip_dirty,
                            uint32_t color_index);

#ifdef __cplusplus
}
#endif
#endif
