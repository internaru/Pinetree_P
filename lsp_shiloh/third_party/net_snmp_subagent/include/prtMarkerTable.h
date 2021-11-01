/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtMarkerTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef PRTMARKERTABLE_H
#define PRTMARKERTABLE_H

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
typedef struct prtMarkerTable_context_s {

    netsnmp_index index;

#if 0 /* All non-index fields are stored in the OID block */
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerIndex;
    /** PrtMarkerMarkTechTC = ASN_INTEGER */
    long prtMarkerMarkTech;
    /** PrtMarkerCounterUnitTC = ASN_INTEGER */
    long prtMarkerCounterUnit;
    /** COUNTER = ASN_COUNTER */
    unsigned long prtMarkerLifeCount;
    /** COUNTER = ASN_COUNTER */
    unsigned long prtMarkerPowerOnCount;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerProcessColorants;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerSpotColorants;
    /** PrtMarkerAddressabilityUnitTC = ASN_INTEGER */
    long prtMarkerAddressabilityUnit;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerAddressabilityFeedDir;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerAddressabilityXFeedDir;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerNorthMargin;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerSouthMargin;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerWestMargin;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerEastMargin;
    /** PrtSubUnitStatusTC = ASN_INTEGER */
    long prtMarkerStatus;
#else
    PRTMIB_OID_MAP *columnInfo;
#endif

} prtMarkerTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_prtMarkerTable(void);
void populate_prtMarkerTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_prtMarkerTable_entry(prtMarkerTable_context *);
//int del_prtMarkerTable_entry(long, long);

void initialize_table_prtMarkerTable(void);
const prtMarkerTable_context *prtMarkerTable_get_by_idx(netsnmp_index *);
const prtMarkerTable_context *prtMarkerTable_get_by_idx_rs(
        netsnmp_index *, int row_status);
int prtMarkerTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

int prtMarkerTable_extract_index(prtMarkerTable_context *, netsnmp_index *);
prtMarkerTable_context *prtMarkerTable_create_row(netsnmp_index *);
int chk_prtMarkerTable_data(prtMarkerTable_context *, unsigned long); 

/*************************************************************
 * oid declarations
 */
extern oid prtMarkerTable_oid[];
extern size_t prtMarkerTable_oid_len;

#define prtMarkerTable_TABLE_OID 1,3,6,1,2,1,43,10,2
    
/*************************************************************
 * column number definitions for table prtMarkerTable
 */
#define COLUMN_PRTMARKERINDEX 1
#define COLUMN_PRTMARKERMARKTECH 2
#define COLUMN_PRTMARKERCOUNTERUNIT 3
#define COLUMN_PRTMARKERLIFECOUNT 4
#define COLUMN_PRTMARKERPOWERONCOUNT 5
#define COLUMN_PRTMARKERPROCESSCOLORANTS 6
#define COLUMN_PRTMARKERSPOTCOLORANTS 7
#define COLUMN_PRTMARKERADDRESSABILITYUNIT 8
#define COLUMN_PRTMARKERADDRESSABILITYFEEDDIR 9
#define COLUMN_PRTMARKERADDRESSABILITYXFEEDDIR 10
#define COLUMN_PRTMARKERNORTHMARGIN 11
#define COLUMN_PRTMARKERSOUTHMARGIN 12
#define COLUMN_PRTMARKERWESTMARGIN 13
#define COLUMN_PRTMARKEREASTMARGIN 14
#define COLUMN_PRTMARKERSTATUS 15
#define prtMarkerTable_COL_MIN 2
#define prtMarkerTable_COL_MAX 15

#ifdef __cplusplus
}
#endif

#endif /** PRTMARKERTABLE_H */
