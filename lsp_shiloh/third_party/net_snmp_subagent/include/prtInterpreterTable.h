/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtInterpreterTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef PRTINTERPRETERTABLE_H
#define PRTINTERPRETERTABLE_H

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
typedef struct prtInterpreterTable_context_s {

    netsnmp_index index;

#if 0 /* All non-index fields are stored in the OID block */
    /** INTEGER32 = ASN_INTEGER */
    long prtInterpreterIndex;
    /** PrtInterpreterLangFamilyTC = ASN_INTEGER */
    long prtInterpreterLangFamily;
    /** OCTETSTR = ASN_OCTET_STR */
    unsigned char prtInterpreterLangLevel[32];
    long prtInterpreterLangLevel_len;
    /** OCTETSTR = ASN_OCTET_STR */
    unsigned char prtInterpreterLangVersion[32];
    long prtInterpreterLangVersion_len;
    /** PrtLocalizedDescriptionStringTC = ASN_OCTET_STR */
    unsigned char prtInterpreterDescription[256];
    long prtInterpreterDescription_len;
    /** OCTETSTR = ASN_OCTET_STR */
    unsigned char prtInterpreterVersion[32];
    long prtInterpreterVersion_len;
    /** PrtPrintOrientationTC = ASN_INTEGER */
    long prtInterpreterDefaultOrientation;
    /** INTEGER32 = ASN_INTEGER */
    long prtInterpreterFeedAddressability;
    /** INTEGER32 = ASN_INTEGER */
    long prtInterpreterXFeedAddressability;
    /** IANACharset = ASN_INTEGER */
    long prtInterpreterDefaultCharSetIn;
    /** IANACharset = ASN_INTEGER */
    long prtInterpreterDefaultCharSetOut;
    /** PrtInterpreterTwoWayTC = ASN_INTEGER */
    long prtInterpreterTwoWay;
#else
    PRTMIB_OID_MAP *columnInfo;
#endif

} prtInterpreterTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_prtInterpreterTable(void);
void populate_prtInterpreterTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_prtInterpreterTable_entry(prtInterpreterTable_context *);
//int del_prtInterpreterTable_entry(long, long);

void initialize_table_prtInterpreterTable(void);
const prtInterpreterTable_context *prtInterpreterTable_get_by_idx(
        netsnmp_index *);
const prtInterpreterTable_context *prtInterpreterTable_get_by_idx_rs(
        netsnmp_index *, int);
int prtInterpreterTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);
int prtInterpreterTable_extract_index(
        prtInterpreterTable_context *, netsnmp_index *);

void prtInterpreterTable_set_reserve1(netsnmp_request_group *);
void prtInterpreterTable_set_reserve2(netsnmp_request_group *);
void prtInterpreterTable_set_action(netsnmp_request_group *);
void prtInterpreterTable_set_commit(netsnmp_request_group *);
void prtInterpreterTable_set_free(netsnmp_request_group *);
void prtInterpreterTable_set_undo(netsnmp_request_group *);

prtInterpreterTable_context *prtInterpreterTable_duplicate_row(
        prtInterpreterTable_context *);
netsnmp_index *prtInterpreterTable_delete_row(
        prtInterpreterTable_context *);
int prtInterpreterTable_can_delete(
        prtInterpreterTable_context *, prtInterpreterTable_context *,
        netsnmp_request_group * rg);
    
int chk_prtInterpreterTable_data(prtInterpreterTable_context *, unsigned long);
int prtInterpreterTable_extract_index(
        prtInterpreterTable_context *, netsnmp_index *);
prtInterpreterTable_context *prtInterpreterTable_create_row(netsnmp_index *);

/*************************************************************
 * oid declarations
 */
extern oid prtInterpreterTable_oid[];
extern size_t prtInterpreterTable_oid_len;

#define prtInterpreterTable_TABLE_OID 1,3,6,1,2,1,43,15,1
    
/*************************************************************
 * column number definitions for table prtInterpreterTable
 */
#define COLUMN_PRTINTERPRETERINDEX 1
#define COLUMN_PRTINTERPRETERLANGFAMILY 2
#define COLUMN_PRTINTERPRETERLANGLEVEL 3
#define COLUMN_PRTINTERPRETERLANGVERSION 4
#define COLUMN_PRTINTERPRETERDESCRIPTION 5
#define COLUMN_PRTINTERPRETERVERSION 6
#define COLUMN_PRTINTERPRETERDEFAULTORIENTATION 7
#define COLUMN_PRTINTERPRETERFEEDADDRESSABILITY 8
#define COLUMN_PRTINTERPRETERXFEEDADDRESSABILITY 9
#define COLUMN_PRTINTERPRETERDEFAULTCHARSETIN 10
#define COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT 11
#define COLUMN_PRTINTERPRETERTWOWAY 12
#define prtInterpreterTable_COL_MIN 2
#define prtInterpreterTable_COL_MAX 12

#ifdef __cplusplus
}
#endif

#endif /** PRTINTERPRETERTABLE_H */
