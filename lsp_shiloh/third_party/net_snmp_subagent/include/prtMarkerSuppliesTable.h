/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtMarkerSuppliesTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef PRTMARKERSUPPLIESTABLE_H
#define PRTMARKERSUPPLIESTABLE_H

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
typedef struct prtMarkerSuppliesTable_context_s {

    netsnmp_index index;

#if 0 /* All non-index fields are stored in the OID block */
    /** INTEGER32 = ASN_INTEGER */
        long prtMarkerSuppliesIndex;
    /** INTEGER32 = ASN_INTEGER */
        long prtMarkerSuppliesMarkerIndex;
    /** INTEGER32 = ASN_INTEGER */
        long prtMarkerSuppliesColorantIndex;
    /** PrtMarkerSuppliesClassTC = ASN_INTEGER */
        long prtMarkerSuppliesClass;
    /** PrtMarkerSuppliesTypeTC = ASN_INTEGER */
        long prtMarkerSuppliesType;
    /** PrtLocalizedDescriptionStringTC = ASN_OCTET_STR */
        unsigned char prtMarkerSuppliesDescription[256];
        long prtMarkerSuppliesDescription_len;
    /** PrtMarkerSuppliesSupplyUnitTC = ASN_INTEGER */
        long prtMarkerSuppliesSupplyUnit;
    /** INTEGER32 = ASN_INTEGER */
        long prtMarkerSuppliesMaxCapacity;
    /** INTEGER32 = ASN_INTEGER */
        long prtMarkerSuppliesLevel;
#else
    PRTMIB_OID_MAP *columnInfo;
#endif

} prtMarkerSuppliesTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_prtMarkerSuppliesTable(void);
void populate_prtMarkerSuppliesTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_prtMarkerSuppliesTable_entry(prtMarkerSuppliesTable_context *);
//int del_prtMarkerSuppliesTable_entry(long, long);

void initialize_table_prtMarkerSuppliesTable(void);
const prtMarkerSuppliesTable_context *prtMarkerSuppliesTable_get_by_idx(
        netsnmp_index *);
const prtMarkerSuppliesTable_context *prtMarkerSuppliesTable_get_by_idx_rs(
        netsnmp_index *, int row_status);
int prtMarkerSuppliesTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

void prtMarkerSuppliesTable_set_reserve1(netsnmp_request_group *);
void prtMarkerSuppliesTable_set_reserve2(netsnmp_request_group *);
void prtMarkerSuppliesTable_set_action(netsnmp_request_group *);
void prtMarkerSuppliesTable_set_commit(netsnmp_request_group *);
void prtMarkerSuppliesTable_set_free(netsnmp_request_group *);
void prtMarkerSuppliesTable_set_undo(netsnmp_request_group *);

prtMarkerSuppliesTable_context *prtMarkerSuppliesTable_duplicate_row(
        prtMarkerSuppliesTable_context *);
netsnmp_index *prtMarkerSuppliesTable_delete_row(
        prtMarkerSuppliesTable_context *);

int prtMarkerSuppliesTable_can_delete(
        prtMarkerSuppliesTable_context *undo_ctx,
        prtMarkerSuppliesTable_context *row_ctx, netsnmp_request_group * rg);
    
int chk_prtMarkerSuppliesTable_data(
                        prtMarkerSuppliesTable_context *, unsigned long);
int prtMarkerSuppliesTable_extract_index(
                        prtMarkerSuppliesTable_context *, netsnmp_index *);
prtMarkerSuppliesTable_context *prtMarkerSuppliesTable_create_row(
                        netsnmp_index *);

/*************************************************************
 * oid declarations
 */
extern oid prtMarkerSuppliesTable_oid[];
extern size_t prtMarkerSuppliesTable_oid_len;

#define prtMarkerSuppliesTable_TABLE_OID 1,3,6,1,2,1,43,11,1
    
/*************************************************************
 * column number definitions for table prtMarkerSuppliesTable
 */
#define COLUMN_PRTMARKERSUPPLIESINDEX 1
#define COLUMN_PRTMARKERSUPPLIESMARKERINDEX 2
#define COLUMN_PRTMARKERSUPPLIESCOLORANTINDEX 3
#define COLUMN_PRTMARKERSUPPLIESCLASS 4
#define COLUMN_PRTMARKERSUPPLIESTYPE 5
#define COLUMN_PRTMARKERSUPPLIESDESCRIPTION 6
#define COLUMN_PRTMARKERSUPPLIESSUPPLYUNIT 7
#define COLUMN_PRTMARKERSUPPLIESMAXCAPACITY 8
#define COLUMN_PRTMARKERSUPPLIESLEVEL 9
#define prtMarkerSuppliesTable_COL_MIN 2
#define prtMarkerSuppliesTable_COL_MAX 9

#ifdef __cplusplus
}
#endif

#endif /** PRTMARKERSUPPLIESTABLE_H */
