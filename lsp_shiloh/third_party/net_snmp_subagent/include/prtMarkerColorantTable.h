/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtMarkerColorantTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef PRTMARKERCOLORANTTABLE_H
#define PRTMARKERCOLORANTTABLE_H

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
typedef struct prtMarkerColorantTable_context_s {

    netsnmp_index index;

#if 0 /* All non-index fields are stored in the OID block */
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerColorantIndex;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerColorantMarkerIndex;
    /** PrtMarkerColorantRoleTC = ASN_INTEGER */
    long prtMarkerColorantRole;
    /** OCTETSTR = ASN_OCTET_STR */
    unsigned char prtMarkerColorantValue[256];
    long prtMarkerColorantValue_len;
    /** INTEGER32 = ASN_INTEGER */
    long prtMarkerColorantTonality;
#else
    PRTMIB_OID_MAP *columnInfo;
#endif

} prtMarkerColorantTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_prtMarkerColorantTable(void);
void populate_prtMarkerColorantTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_prtMarkerColorantTable_entry(prtMarkerColorantTable_context *);
//int del_prtMarkerColorantTable_entry(long, long);

void init_prtMarkerColorantTable(void);
void initialize_table_prtMarkerColorantTable(void);
const prtMarkerColorantTable_context *prtMarkerColorantTable_get_by_idx(
        netsnmp_index *);
const prtMarkerColorantTable_context *prtMarkerColorantTable_get_by_idx_rs(
        netsnmp_index *, int);
int prtMarkerColorantTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

int prtMarkerColorantTable_extract_index(
        prtMarkerColorantTable_context *, netsnmp_index *);
prtMarkerColorantTable_context *prtMarkerColorantTable_create_row(
        netsnmp_index *);
int chk_prtMarkerColorantTable_data(
        prtMarkerColorantTable_context *, unsigned long); 

/*************************************************************
 * oid declarations
 */
extern oid prtMarkerColorantTable_oid[];
extern size_t prtMarkerColorantTable_oid_len;

#define prtMarkerColorantTable_TABLE_OID 1,3,6,1,2,1,43,12,1
    
/*************************************************************
 * column number definitions for table prtMarkerColorantTable
 */
#define COLUMN_PRTMARKERCOLORANTINDEX 1
#define COLUMN_PRTMARKERCOLORANTMARKERINDEX 2
#define COLUMN_PRTMARKERCOLORANTROLE 3
#define COLUMN_PRTMARKERCOLORANTVALUE 4
#define COLUMN_PRTMARKERCOLORANTTONALITY 5
#define prtMarkerColorantTable_COL_MIN 2
#define prtMarkerColorantTable_COL_MAX 5

#ifdef __cplusplus
}
#endif

#endif /** PRTMARKERCOLORANTTABLE_H */
