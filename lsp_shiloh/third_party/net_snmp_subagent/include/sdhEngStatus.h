/*
 * ============================================================================
 * Copyright (c) 2014   SINDOH International, Ltd. All Rights Reserved
 * ============================================================================
 *
 */

#ifndef SDHENGSTATUSTABLE_H
#define SDHENGSTATUSTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

void init_sdhEngStatusTable( void );

Netsnmp_Node_Handler handler_EngScanStatus;
Netsnmp_Node_Handler handler_EngFAXStatus;
Netsnmp_Node_Handler handler_EngCopyStatus;
#ifdef HAVE_IMAGELOG
Netsnmp_Node_Handler handler_EngImageLogStatus;
#endif

#ifdef __cplusplus
}
#endif

#endif /** SDHENGSTATUSTABLE_H */
