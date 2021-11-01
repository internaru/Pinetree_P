/** 
 * \file scanhwerr.h
 *
 * \brief Scanner Hardware Errors
 *
 *  Must preserve the order of these numbers!  The numbers below will appear on
 *  the control panel and are used by manufacturing to debug build problems.
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell Semiconductor, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/


#ifndef SCANHWERR_H
#define SCANHWERR_H

// Failure codes for Scanner Hardware Failures

typedef enum {

  ScannerHWError_None = 0,

  ScannerHWError_DarkFind = 1,                 // 1

  ScannerHWError_LMWTempFind,                  // 2

  ScannerHWError_BulbWarmupFromOff,            // 3 

  ScannerHWError_BulbWarmupFromCoarse,         // 4

  ScannerHWError_CalLabelFind,                 // 5

  ScannerHWError_DocOriginFind,                // 6

  ScannerHWError_LMWFinalFind,                 // 7

  ScannerHWError_CalLabelFindAfterScan,        // 8

  ScannerHWError_CalLabelFindPriorToStandby,   // 9

  ScannerHWError_DocOriginFindPriorToStandby,  // 10

  ScannerHWError_BulbWarmupFromStandby,        // 11
  
  ScannerHWError_MotorProblem,                  // 12
  
  ScannerHWError_AFEProblem,                    // 13

  ScannerHWError_RedLEDProblem,                 // 14

  ScannerHWError_GreenLEDProblem,               // 15

  ScannerHWError_BlueLEDProblem,                // 16
  
  ScannerHWError_RedGreenLEDProblem,            // 17

  ScannerHWError_RedBlueLEDProblem,             // 18

  ScannerHWError_GreenBlueLEDProblem,           // 19

  ScannerHWError_RedGreenBlueLEDProblem,        // 20

//  ScannerHWError_OutOfMemory,                   // 21 
  ScannerHWError_RESERVED1, // 21 ; must preserve in order to maintain enum numbering

  ScannerHWError_MotorMoveProblem,             // 22 

  ScannerHWError_ScannerStarting,              // 23

  ScannerHWError_GeneralFailure=0xff            //undefined failure         

} ScannerHWError;

/* implemented in scan.c */
ScannerHWError scan_get_hwerr( void );


/* Extra description of hardware error codes.
 *
 * Note: The following error codes are the only ones currently used in firmware.
 * The others are placeholders for hardware we don't yet use in a product (e.g.,
 * CCFL, CCD).
 *
 *
 * DocOriginFind.
 *  In a flatbed scanner, the bezel usually contains some sort of marking to
 *  allow firmware to calibrate the distance between the motor hard stop and the
 *  top of the glass (the top of the document). At startup, the firmware will
 *  scan for that marking; if firmware is unable to find the mark, a
 *  DocOriginFind error will be raised.
 *
 *  The DocOriginFind is the firmware's first image capture on boot. A
 *  DocOriginFind error is almost always due to a hardware failure where we're
 *  unable to scan. 
 *
 *
 * AFEProblem.
 *  The firmware is unable to communicate with the AFE.
 *
 *
 * Red/Green/Blue/Etc. LED Problem.
 *  Some hardware platforms use LEDs for scan illumination. On boot, the
 *  firmware will run a simple color scan to test the sensor and the LEDs. If
 *  any of the LEDs fail to light, the appropriate LED failure will be raised.
 *
 *  A completely failing sensor will sometimes be reported as a RedGreenBlue LED
 *  failure because we cannot easily tell the difference between no light and
 *  failing to capture the light.
 *
 *
 * MotorMoveProblem.
 *  Some hardware platforms have motors with feedback (e.g., DC motor+encoder).
 *  On boot, the firmware will run a motor self-test. If firmware is unable to
 *  read any feedback, a MotorProblem error will be raised.
 *
 * ScannerStarting.
 *  On startup, the hardware error code is initialized to this value. When the
 *  hardware is successfully initialized, we should transition to the 'None'
 *  error or a more specific error code.
 *
 */
#endif 

