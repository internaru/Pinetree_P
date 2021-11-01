
/*
 *
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
*/  

#ifndef UFSTRENDERER_H
#define UFSTRENDERER_H

#include "renderer.h"

#ifdef __cplusplus
extern "C" {
#endif

void UFSTTextRenderer(RendererSession *pRSession, unsigned char *Strip, DISP_LIST *TempPage, 
                         RENDER_INPUT *RendInput, UINT32 Color, UINT32 Rotate);

#ifdef __cplusplus
}
#endif
#endif
