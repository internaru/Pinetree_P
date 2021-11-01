/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell Semiconductor, Inc. All Rights Reserved
*
*               Marvell Confidential
 * ============================================================================
 *
*/

/*
 * @file tray selection logic.
 * 
 **/

#ifndef TrayPaperResolve_h
#define TrayPaperResolve_h

/**
 * \brief Structure used during paper path stall check.
 */

#include "PrintMgr_Priv.h"

#ifdef __cplusplus
extern "C" {
#endif
bool ///< @return t or f 
PrintMgrResolve(
    const printer_setup_t* pSetup, 
    const mediacnfg_t* pMediaCnfg, 
    STALLCHECKDATA* pStallData, 
    PAGE_DATA* pPageObject,
    LASTPAGEINFO *lastPage);


#ifdef __cplusplus
}
#endif
#endif
