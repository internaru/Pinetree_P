/* 
 * ============================================================================
 * Copyright (c) 2009-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file adfsensor.h
 *
 * \brief ADF sensor interface definition
 *
 *   This file defines the contract between the core scan code and the scan mech
 *   code with respect to ADF sensors: all of these functions must either be
 *   implemented or stubbed out by the mech code.  We are trying to keep ADF
 *   sensor implementation details away from the core scan code.
 *
 *   Notes:
 *    - The mech code may need and create more interfaces to support ADF sensors,
 *      but only those shared between core and mech should be in this file.
 *    - The core scan code also needs to be notified when ADF paper in path
 *      events occur (leading/trailing edges) in order for the pipe cutter to
 *      function. See smech_adf_paper_event_callback() in scanmech.h.
 **/

#ifndef ADFSENSOR_H
#define ADFSENSOR_H

/**
 *  \brief Initialize the ADF sensors
 *
 *  This routine must be called once at startup before using any of the
 *  other ADF sensor routines. This will typically be used to setup
 *  any sensor related hardware (gpios for example). 
 *
 *  If your scan mech does not have an ADF, you must provide a stub implementation
 *  of this function.
 *
 **/
void adf_sensor_onetime_init( void );


/**
 *  \brief Is paper in the ADF path?
 *
 *  Function to return the state of the paper in path (PIP) sensor. This
 *  sensor typically indicates when paper is at a specific location in the
 *  ADF path.  For example it could be used:
 *    - to detect and stage the leading edge
 *    - to detect an approaching trailing edge
 *    - to detect a paper jam in the ADF   
 *
 *  If your scan mech does not have an ADF, you must provide a stub implementation
 *  of this function that returns FALSE.
 *
 *  \return BOOL
 *  \retval TRUE  Paper is in the ADF path
 *  \retval FALSE Paper is not in the ADF path
 **/
bool adf_sensor_paper_in_path( void );


/**
 *  \brief Is paper present in the ADF input tray?
 *
 *  Function to return the state of the paper present (PP) sensor. This
 *  sensor typically indicates when paper is loaded in the ADF input tray.
 *
 *  If your scan mech does not have an ADF, you must provide a stub implementation
 *  of this function that returns FALSE.
 *
 *  \return BOOL
 *  \retval TRUE  Paper is in the ADF input tray
 *  \retval FALSE Paper is not in the ADF input tray
 **/
bool adf_sensor_paper_present( void );

/**
 * \brief Allow smech_adf_paper_event_callback() to be called
 *
 * When paper-in-path or paper-present signals change,
 * call the appropriate callback.
 */
void adf_sensor_pip_callback_enable( void );


/**
 * \brief Disallow calling of smech_adf_paper_event_callback()
 *
 * Don't call smech_adf_paper_event_callback() when pip or pp
 * signals change.
 */
void adf_sensor_pip_callback_disable( void );


/**
 * \brief Get the adf sensor ready for power down
 *
 * This method will be called every time the system is powering down.
 */
void adf_sensor_powerdown( void );

/**
 * \brief Get the adf sensor ready for power up
 *
 * This method will be called every time the system is powering up.
 */
void adf_sensor_powerup( void );

void adf_sensor_pip_debounce_sample( void );

#endif

