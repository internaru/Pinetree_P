/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * sdhEngineTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef SDHENGINETABLE_H
#define SDHENGINETABLE_H

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

typedef struct sdhEngineTable_context_s {

    netsnmp_index index;

    PRTMIB_OID_MAP *columnInfo;

} sdhEngineTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_sdhEngineTable(void);
void populate_sdhEngineTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_sdhEngineTable_entry(sdhEngineTable_context *);
//int del_sdhEngineTable_entry(long);

void initialize_table_sdhEngineTable(void);
const sdhEngineTable_context *sdhEngineTable_get_by_idx(netsnmp_index *);
const sdhEngineTable_context *sdhEngineTable_get_by_idx_rs(
        netsnmp_index *, int row_status);
int sdhEngineTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

sdhEngineTable_context *sdhEngineTable_create_row(netsnmp_index *);
int sdhEngineTable_extract_index(sdhEngineTable_context *, netsnmp_index *);

void sdhEngineTable_set_reserve1(netsnmp_request_group *);
void sdhEngineTable_set_reserve2(netsnmp_request_group *);
void sdhEngineTable_set_action(netsnmp_request_group *);
void sdhEngineTable_set_commit(netsnmp_request_group *);
void sdhEngineTable_set_free(netsnmp_request_group *);
void sdhEngineTable_set_undo(netsnmp_request_group *);

sdhEngineTable_context *sdhEngineTable_duplicate_row(
        sdhEngineTable_context *);
netsnmp_index * sdhEngineTable_delete_row(sdhEngineTable_context *);
int sdhEngineTable_can_delete(sdhEngineTable_context *, 
        sdhEngineTable_context *, netsnmp_request_group *);

/*************************************************************
 * oid declarations
 */
extern oid sdhEngineTable_oid[];
extern size_t sdhEngineTable_oid_len;

#define SDHENGINETABLE_TABLE_OID 1,3,6,1,4,1,27278,1,3
    
/*************************************************************
 * column number definitions for table sdhEngineTable
 */
#define COLUMN_ENGSTATUSCODE 1
#define COLUMN_ENGDRUMLIFE 2
#define COLUMN_ENGBRIDGE 3
#define COLUMN_ENGONEBIN 4
#define COLUMN_ENGCOPYCOUNTER 5
#define COLUMN_ENGPRINTCOUNTER 6
#define COLUMN_ENGFAXCOUNT 7
#define COLUMN_ENGA3PRINTCOUNT 8
#define COLUMN_ENGB4PRINTCOUNT 9
#define COLUMN_ENGA4PRINTCOUNT 10
#define COLUMN_ENGB5PRINTCOUNT 11
#define COLUMN_ENGA5PRINTCOUNT 12
#define COLUMN_ENGP8x13PRINTCOUNT 13
#define COLUMN_ENGP8KPRINTCOUNT 14
#define COLUMN_ENGP16KPRINTCOUNT 15
#define COLUMN_ENGB6PRINTCOUNT 16
#define COLUMN_ENGETCPRINTCOUNT 17
#define COLUMN_SCANTOTALCOUNT 18
#define COLUMN_SCANFBCOUNT 19
#define COLUMN_SCANADFCOUNT 20
#define COLUMN_SCANADFDUPLEXCOUNT 21
#define COLUMN_SCANA3COUNT 22
#define SDHENGINETABLE_COL_MIN 1
#define SDHENGINETABLE_COL_MAX 22

#ifdef __cplusplus
}
#endif

#endif /** SDHENGINETABLE_H */
