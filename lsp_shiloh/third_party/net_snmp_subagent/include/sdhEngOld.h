/*
 * ============================================================================
 * Copyright (c) 2019 Sindoh Co., Ltd. All Rights Reserved
 * ============================================================================
 * sdhEngOld.h
 * mspyo@sindoh.com
 * 2019-04-23
 * 
 */

#ifndef SDHENGOLD_H
#define SDHENGOLD_H

#ifdef __cplusplus
extern "C" {
#endif

void init_sdhEngOld( void );

Netsnmp_Node_Handler handler_sdhEngOldStatus; // 4
Netsnmp_Node_Handler handler_sdhEngOldDrumLife; // 5
Netsnmp_Node_Handler handler_sdhEngOldBridge;  // 6
Netsnmp_Node_Handler handler_sdhEngOldOneBin; // 7
Netsnmp_Node_Handler handler_sdhEngOldCopyCount; // 8
Netsnmp_Node_Handler handler_sdhEngOldPrintCount; // 9
Netsnmp_Node_Handler handler_sdhEngOldFaxCount; // 10
Netsnmp_Node_Handler handler_sdhEngOldHostPrintCount; // 11
Netsnmp_Node_Handler handler_sdhEngOldReportPrintCount; // 12
Netsnmp_Node_Handler handler_sdhEngOldDuplexPrintCount; // 13

#ifdef __cplusplus
}
#endif

#endif /** SDHENGOLD_H */
