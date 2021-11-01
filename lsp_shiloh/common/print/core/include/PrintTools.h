/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2004-2009 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file PrintTools.h
 *
 * \brief Header file for print tools.  This is the same file for all print mech types.
 *
 *****************************************************************************/
 
#ifndef PRINTTOOLS_H
#define PRINTTOOLS_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
#ifdef HAVE_RTOS
void PrintToolsInit( TX_EVENT_FLAGS_GROUP *EventFlags );
#else
// ???????    
#endif    

void AddPlaneToPage( PAGE_DATA *TempPage, void *Plane );

void SendPlaneEnd( PAGE_DATA *TempPage, MESSAGE *Mesag );

void AddRasterToPlane( PAGE_DATA *TempPage, MESSAGE *Mesag, UINT16 UseCount );

void SendPageStart( PAGE_DATA *TempPage );

void SendPageEnd( PAGE_DATA *TempPage, MESSAGE *Mesag );

UINT32 FreeRasterFromPage( PAGE_DATA *TempPage, FORCE_CLEAR Clear );

PAGE_DATA *GetPageHeader(mlimiter_t* limiter);

void SetRacingMode( PAGE_DATA *TempPage, UINT32 Mode );

UINT32 ReturnRacingMode( PAGE_DATA *TempPage );

void UpdatePlanes( PAGE_DATA* PageHead );
void SanitizeUseCounts(PAGE_DATA *TempPage);


#if defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)

void MakeFrontPageCopiesMatchBackPageCopies( PAGE_DATA* pFrontPage, const PAGE_DATA* pBackPage );

#endif // defined(HAVE_AUTOMATIC_DUPLEXER_SUPPORT)

# if defined( HAVE_DBG_PRINTF )

void PrintPagePlaneInfo( PAGE_DATA* pPage );
void PrintPlaneInfo( PLANED* pPlane );

# else  // !defined( DEBUG )

#   define PrintPagePlaneInfo( pPage )
#   define PrintPlaneInfo( pPlane )

# endif // !defined( DEBUG )

#ifdef __cplusplus
}
#endif
 
#endif // ifndef PRINTTOOLS_H
 
