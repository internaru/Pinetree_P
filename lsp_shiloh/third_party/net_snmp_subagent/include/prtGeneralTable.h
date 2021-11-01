/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtGeneralTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef PRTGENERALTABLE_H
#define PRTGENERALTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

#include "printer_mib_translation.h"


/*************************************************************
 * table definition
 */

typedef struct prtGeneralTable_context_s {

    netsnmp_index index;

#if 0 /* All non-index fields are stored in the OID block */
    /** COUNTER = ASN_COUNTER */
        unsigned long prtGeneralConfigChanges;
    /** INTEGER32 = ASN_INTEGER */
        long prtGeneralCurrentLocalization;
    /** PrtGeneralResetTC = ASN_INTEGER */
        long prtGeneralReset;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtGeneralCurrentOperator[128];
        long prtGeneralCurrentOperator_len;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtGeneralServicePerson[128];
        long prtGeneralServicePerson_len;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputDefaultIndex;
    /** INTEGER32 = ASN_INTEGER */
        long prtOutputDefaultIndex;
    /** INTEGER32 = ASN_INTEGER */
        long prtMarkerDefaultIndex;
    /** INTEGER32 = ASN_INTEGER */
        long prtMediaPathDefaultIndex;
    /** INTEGER32 = ASN_INTEGER */
        long prtConsoleLocalization;
    /** INTEGER32 = ASN_INTEGER */
        long prtConsoleNumberOfDisplayLines;
    /** INTEGER32 = ASN_INTEGER */
        long prtConsoleNumberOfDisplayChars;
    /** PrtConsoleDisableTC = ASN_INTEGER */
        long prtConsoleDisable;
    /** PresentOnOff = ASN_INTEGER */
        long prtAuxiliarySheetStartupPage;
    /** PresentOnOff = ASN_INTEGER */
        long prtAuxiliarySheetBannerPage;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtGeneralPrinterName[128];
        long prtGeneralPrinterName_len;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtGeneralSerialNumber[256];
        long prtGeneralSerialNumber_len;
    /** COUNTER = ASN_COUNTER */
        unsigned long prtAlertCriticalEvents;
    /** COUNTER = ASN_COUNTER */
        unsigned long prtAlertAllEvents;
#else
    PRTMIB_OID_MAP *columnInfo;
#endif

} prtGeneralTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_prtGeneralTable(void);
void populate_prtGeneralTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_prtGeneralTable_entry(prtGeneralTable_context *);
//int del_prtGeneralTable_entry(long);

void initialize_table_prtGeneralTable(void);
const prtGeneralTable_context *prtGeneralTable_get_by_idx(netsnmp_index *);
const prtGeneralTable_context *prtGeneralTable_get_by_idx_rs(
        netsnmp_index *, int row_status);
int prtGeneralTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

prtGeneralTable_context *prtGeneralTable_create_row(netsnmp_index *);
int prtGeneralTable_extract_index(prtGeneralTable_context *, netsnmp_index *);

void prtGeneralTable_set_reserve1(netsnmp_request_group *);
void prtGeneralTable_set_reserve2(netsnmp_request_group *);
void prtGeneralTable_set_action(netsnmp_request_group *);
void prtGeneralTable_set_commit(netsnmp_request_group *);
void prtGeneralTable_set_free(netsnmp_request_group *);
void prtGeneralTable_set_undo(netsnmp_request_group *);

prtGeneralTable_context *prtGeneralTable_duplicate_row(
        prtGeneralTable_context *);
netsnmp_index * prtGeneralTable_delete_row(prtGeneralTable_context *);
int prtGeneralTable_can_delete(prtGeneralTable_context *, 
        prtGeneralTable_context *, netsnmp_request_group *);

/*************************************************************
 * oid declarations
 */
extern oid prtGeneralTable_oid[];
extern size_t prtGeneralTable_oid_len;

#define prtGeneralTable_TABLE_OID 1,3,6,1,2,1,43,5,1
    
/*************************************************************
 * column number definitions for table prtGeneralTable
 */
#define COLUMN_PRTGENERALCONFIGCHANGES 1
#define COLUMN_PRTGENERALCURRENTLOCALIZATION 2
#define COLUMN_PRTGENERALRESET 3
#define COLUMN_PRTGENERALCURRENTOPERATOR 4
#define COLUMN_PRTGENERALSERVICEPERSON 5
#define COLUMN_PRTINPUTDEFAULTINDEX 6
#define COLUMN_PRTOUTPUTDEFAULTINDEX 7
#define COLUMN_PRTMARKERDEFAULTINDEX 8
#define COLUMN_PRTMEDIAPATHDEFAULTINDEX 9
#define COLUMN_PRTCONSOLELOCALIZATION 10
#define COLUMN_PRTCONSOLENUMBEROFDISPLAYLINES 11
#define COLUMN_PRTCONSOLENUMBEROFDISPLAYCHARS 12
#define COLUMN_PRTCONSOLEDISABLE 13
#define COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE 14
#define COLUMN_PRTAUXILIARYSHEETBANNERPAGE 15
#define COLUMN_PRTGENERALPRINTERNAME 16
#define COLUMN_PRTGENERALSERIALNUMBER 17
#define COLUMN_PRTALERTCRITICALEVENTS 18
#define COLUMN_PRTALERTALLEVENTS 19
#define prtGeneralTable_COL_MIN 1
#define prtGeneralTable_COL_MAX 19

#ifdef __cplusplus
}
#endif

#endif /** PRTGENERALTABLE_H */
