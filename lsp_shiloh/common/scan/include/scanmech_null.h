/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/* 
 *   "Null" mech driver. Interface to scan core code so we can scan
 *   without either having a motor or moving a motor.
 *
 **/
 
#ifndef SCANMECH_NULL_H
#define SCANMECH_NULL_H

/* not much else is needed externally */
void smech_null_init( t_ScanMech *scan_mech );

#endif

