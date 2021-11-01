/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtMediaPathTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef PRTMEDIAPATHTABLE_H
#define PRTMEDIAPATHTABLE_H

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
typedef struct prtMediaPathTable_context_s {

    netsnmp_index index;

#if 0 /* All non-index fields are stored in the OID block */
    /** INTEGER32 = ASN_INTEGER */
    long prtMediaPathIndex;
    /** PrtMediaPathMaxSpeedPrintUnitTC = ASN_INTEGER */
    long prtMediaPathMaxSpeedPrintUnit;
    /** PrtMediaUnitTC = ASN_INTEGER */
    long prtMediaPathMediaSizeUnit;
    /** INTEGER32 = ASN_INTEGER */
    long prtMediaPathMaxSpeed;
    /** INTEGER32 = ASN_INTEGER */
    long prtMediaPathMaxMediaFeedDir;
    /** INTEGER32 = ASN_INTEGER */
    long prtMediaPathMaxMediaXFeedDir;
    /** INTEGER32 = ASN_INTEGER */
    long prtMediaPathMinMediaFeedDir;
    /** INTEGER32 = ASN_INTEGER */
    long prtMediaPathMinMediaXFeedDir;
    /** PrtMediaPathTypeTC = ASN_INTEGER */
    long prtMediaPathType;
    /** PrtLocalizedDescriptionStringTC = ASN_OCTET_STR */
    unsigned char prtMediaPathDescription[256];
    long prtMediaPathDescription_len;
    /** PrtSubUnitStatusTC = ASN_INTEGER */
    long prtMediaPathStatus;
#else
    PRTMIB_OID_MAP *columnInfo;
#endif

} prtMediaPathTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_prtMediaPathTable(void);
void populate_prtMediaPathTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_prtMediaPathTable_entry(prtMediaPathTable_context *);
//int del_prtMediaPathTable_entry(long, long);

void init_prtMediaPathTable(void);
void initialize_table_prtMediaPathTable(void);
const prtMediaPathTable_context *prtMediaPathTable_get_by_idx(netsnmp_index *);
const prtMediaPathTable_context *prtMediaPathTable_get_by_idx_rs(
        netsnmp_index *, int);
int prtMediaPathTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

int prtMediaPathTable_extract_index(
        prtMediaPathTable_context *, netsnmp_index *);
prtMediaPathTable_context *prtMediaPathTable_create_row(netsnmp_index *);
int chk_prtMediaPathTable_data(prtMediaPathTable_context *, unsigned long); 

/*************************************************************
 * oid declarations
 */
extern oid prtMediaPathTable_oid[];
extern size_t prtMediaPathTable_oid_len;

#define prtMediaPathTable_TABLE_OID 1,3,6,1,2,1,43,13,4
    
/*************************************************************
 * column number definitions for table prtMediaPathTable
 */
#define COLUMN_PRTMEDIAPATHINDEX 1
#define COLUMN_PRTMEDIAPATHMAXSPEEDPRINTUNIT 2
#define COLUMN_PRTMEDIAPATHMEDIASIZEUNIT 3
#define COLUMN_PRTMEDIAPATHMAXSPEED 4
#define COLUMN_PRTMEDIAPATHMAXMEDIAFEEDDIR 5
#define COLUMN_PRTMEDIAPATHMAXMEDIAXFEEDDIR 6
#define COLUMN_PRTMEDIAPATHMINMEDIAFEEDDIR 7
#define COLUMN_PRTMEDIAPATHMINMEDIAXFEEDDIR 8
#define COLUMN_PRTMEDIAPATHTYPE 9
#define COLUMN_PRTMEDIAPATHDESCRIPTION 10
#define COLUMN_PRTMEDIAPATHSTATUS 11
#define prtMediaPathTable_COL_MIN 2
#define prtMediaPathTable_COL_MAX 11

#ifdef __cplusplus
}
#endif

#endif /** PRTMEDIAPATHTABLE_H */
