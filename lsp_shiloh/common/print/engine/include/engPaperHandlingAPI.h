/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file engPaperHandlingAPI.h
 *
 * \brief This file contains declarations for a paper-handling API that is
 *        common to all print engines and mechs.
 *
 */
#ifndef ENG_PAPER_HANDLING_API_H
#define ENG_PAPER_HANDLING_API_H

#include <stdint.h>
#include <stdbool.h>

#include "agMessage.h"
#include "agprint.h"



#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// { BEGIN Object to convert between user-oriented, intuitive, one-relative
//                                   physical input tray number and trayenum_t.
void        ConstructPhysicalTrayNumberToTRAYENUMObject(const printer_setup_t* pPrinterSetup);
trayenum_t    EngPhysicalTrayNumberToTRAYENUM(int oneRelativePhysicalTrayNumber);
int         EngTRAYENUMToPhysicalTrayNumber(trayenum_t inputTRAYENUM);
bool        eng_get_have_more_than_one_physical_input_tray( void );
bool        eng_get_have_only_one_physical_input_tray( void );
//
// } END   Object to convert between physical input tray number and trayenum_t.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// { BEGIN OBJECT: TRAYENUM_TRAY_1_TRAY_2_Coincide
//
bool        EngGetTRAYENUM_TRAY_1_TRAY_2_Coincide(void);
void        EngSetTRAYENUM_TRAY_1_TRAY_2_Coincide(bool newValue);
//
// } END   OBJECT: TRAYENUM_TRAY_1_TRAY_2_Coincide
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
bool        EngGetAutoDuplexerHasMedia(void);

//------------------------------------------------------------------------------
// { BEGIN OBJECT: AutoDuplexerInputTRAYENUM
//
trayenum_t    EngGetAutoDuplexerInputTRAYENUM(void);
void        EngSetAutoDuplexerInputTRAYENUM(trayenum_t newValue);
//
// } END   OBJECT: AutoDuplexerInputTRAYENUM
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// { BEGIN OBJECT: AutoDuplexerOutputTRAYENUM
//
trayenum_t    EngGetAutoDuplexerOutputTRAYENUM(void);
void        EngSetAutoDuplexerOutputTRAYENUM(trayenum_t newValue);
//
// } END   OBJECT: AutoDuplexerOutputTRAYENUM
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// { BEGIN OBJECT: FaceDownOutputTRAYENUM
//
trayenum_t    EngGetFaceDownOutputTRAYENUM(void);
void        EngSetFaceDownOutputTRAYENUM(trayenum_t newValue);
//
// } END   OBJECT: FaceDownOutputTRAYENUM
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// { BEGIN OBJECT: HaveAutoDuplexer
//
bool        EngGetHaveAutoDuplexer(void);
void        EngSetHaveAutoDuplexer(bool newValue);
//
// } END   OBJECT: HaveAutoDuplexer
//------------------------------------------------------------------------------

/// return true if the engine wants the frontside of a duplex job first.
/// 
bool EngDuplexerFrontSideFirst(void);

//------------------------------------------------------------------------------
// { BEGIN OBJECT: HaveBinFullSensor
//
bool        EngGetHaveBinFullSensor(void);
void        EngSetHaveBinFullSensor(bool newValue);
//
// } END   OBJECT: HaveBinFullSensor
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Miscellaneous methods, more or less in alphabetical order.
//
bool        EngMediaIsDuplexable( mediasize_t inMediaSize, 
                                  mediatype_t inMediaType );
bool        EngMEDIATYPEIsSymmetric( mediatype_t inputMEDIATYPE );
bool        EngPaperPresentInSomeInputTray( void );
bool        EngTraySupportsMEDIASIZE( trayenum_t inputTRAYENUM,
                                      mediasize_t inputMEDIASIZE );
bool        EngTraySupportsMEDIATYPE( trayenum_t inputTRAYENUM,
                                      mediatype_t inputMEDIATYPE );
void        UpdateStatisticsPageDoneCommon( const MESSAGE* pMsg );


#if defined( DEBUG )

void        EngPaperHandlingPrintState( void );

#endif // defined( DEBUG )


#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
#endif // ifndef ENG_PAPER_HANDLING_API_H
