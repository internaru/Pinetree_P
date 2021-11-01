/*
 * ============================================================================
 * Copyright (c) 2014   SINDOH International, Ltd. All Rights Reserved
 * ============================================================================
 *
 */

#ifndef PORTMONITORTABLE_H
#define PORTMONITORTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

void init_portMonitorTable( void );

Netsnmp_Node_Handler handler_ppmGeneralNaturalLanguage;
Netsnmp_Node_Handler handler_ppmGeneralNumberOfPrinters;
Netsnmp_Node_Handler handler_ppmGeneralNumberOfPorts;

Netsnmp_Node_Handler handler_ppmPrinterName;
Netsnmp_Node_Handler handler_ppmPrinterIEEE1284DeviceId;
Netsnmp_Node_Handler handler_ppmPrinterNumberOfPorts;
Netsnmp_Node_Handler handler_ppmPrinterPreferredPortIndex;
Netsnmp_Node_Handler handler_ppmPrinterHrDeviceIndex;
Netsnmp_Node_Handler handler_ppmPrinterSnmpCommunityName;
Netsnmp_Node_Handler handler_ppmPrinterSnmpQueryEnabled;

Netsnmp_Node_Handler handler_ppmPortEnabled;
Netsnmp_Node_Handler handler_ppmPortName;
Netsnmp_Node_Handler handler_ppmPortServiceNameOrURI;
Netsnmp_Node_Handler handler_ppmPortProtocolType;
Netsnmp_Node_Handler handler_ppmPortProtocolTargetPort;
Netsnmp_Node_Handler handler_ppmPortProtocolAltSourceEnabled;
Netsnmp_Node_Handler handler_ppmPortPrtChannelIndex;
Netsnmp_Node_Handler handler_ppmPortLprByteCountEnabled;

#ifdef __cplusplus
}
#endif

#endif /** PORTMONITORTABLE_H */
