/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * sdhEngGeneralTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef SDHENGGENERALTABLE_H
#define SDHENGGENERALTABLE_H

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

typedef struct sdhEngGeneralTable_context_s {

    netsnmp_index index;

    PRTMIB_OID_MAP *columnInfo;

} sdhEngGeneralTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_sdhEngGeneralTable(void);
void populate_sdhEngGeneralTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_sdhEngGeneralTable_entry(sdhEngGeneralTable_context *);
//int del_sdhEngGeneralTable_entry(long);

void initialize_table_sdhEngGeneralTable(void);
const sdhEngGeneralTable_context *sdhEngGeneralTable_get_by_idx(netsnmp_index *);
const sdhEngGeneralTable_context *sdhEngGeneralTable_get_by_idx_rs(
        netsnmp_index *, int row_status);
int sdhEngGeneralTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

sdhEngGeneralTable_context *sdhEngGeneralTable_create_row(netsnmp_index *);
int sdhEngGeneralTable_extract_index(sdhEngGeneralTable_context *, netsnmp_index *);

void sdhEngGeneralTable_set_reserve1(netsnmp_request_group *);
void sdhEngGeneralTable_set_reserve2(netsnmp_request_group *);
void sdhEngGeneralTable_set_action(netsnmp_request_group *);
void sdhEngGeneralTable_set_commit(netsnmp_request_group *);
void sdhEngGeneralTable_set_free(netsnmp_request_group *);
void sdhEngGeneralTable_set_undo(netsnmp_request_group *);

sdhEngGeneralTable_context *sdhEngGeneralTable_duplicate_row(
        sdhEngGeneralTable_context *);
netsnmp_index * sdhEngGeneralTable_delete_row(sdhEngGeneralTable_context *);
int sdhEngGeneralTable_can_delete(sdhEngGeneralTable_context *, 
        sdhEngGeneralTable_context *, netsnmp_request_group *);

/*************************************************************
 * oid declarations
 */
extern oid sdhEngGeneralTable_oid[];
extern size_t sdhEngGeneralTable_oid_len;

#define SDHENGGENERALTABLE_TABLE_OID 1,3,6,1,4,1,27278,1
    
/*************************************************************
 * column number definitions for table sdhEngGeneralTable
 */
#define COLUMN_SDHENGSERIALNUMBER 1
#define COLUMN_SDHENGSEARCHFUNCTION 2
#define COLUMN_SDHENGCOUNTER 3
#define COLUMN_SDHENGTONERLEVEL 4
#define COLUMN_SDHENGHDDMAX 5
#define COLUMN_SDHENGHDDUSED 6
#define SDHENGGENERALTABLE_COL_MIN 1
#define SDHENGGENERALTABLE_COL_MAX 6

#ifdef __cplusplus
}
#endif

#endif /** SDHENGGENERALTABLE_H */
