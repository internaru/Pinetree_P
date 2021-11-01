/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * sdhEngineTable Printer MIB (Header) --> sdhEngineTable_old, by mspyo, 2019-02-26
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef SDHENGINETABLE_OLD_H
#define SDHENGINETABLE_OLD_H

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

typedef struct sdhEngineTable_old_context_s {

    netsnmp_index index;

    PRTMIB_OID_MAP *columnInfo;

} sdhEngineTable_old_context;

/*************************************************************
 * APIs for printer application
 */
void init_sdhEngineTable_old(void);
void populate_sdhEngineTable_old(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_sdhEngineTable_entry(sdhEngineTable_context *);
//int del_sdhEngineTable_entry(long);

void initialize_table_sdhEngineTable_old(void);
const sdhEngineTable_old_context *sdhEngineTable_old_get_by_idx(netsnmp_index *);
const sdhEngineTable_old_context *sdhEngineTable_old_get_by_idx_rs(
        netsnmp_index *, int row_status);
int sdhEngineTable_old_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

sdhEngineTable_old_context *sdhEngineTable_old_create_row(netsnmp_index *);
int sdhEngineTable_old_extract_index(sdhEngineTable_old_context *, netsnmp_index *);

void sdhEngineTable_old_set_reserve1(netsnmp_request_group *);
void sdhEngineTable_old_set_reserve2(netsnmp_request_group *);
void sdhEngineTable_old_set_action(netsnmp_request_group *);
void sdhEngineTable_old_set_commit(netsnmp_request_group *);
void sdhEngineTable_old_set_free(netsnmp_request_group *);
void sdhEngineTable_old_set_undo(netsnmp_request_group *);

sdhEngineTable_old_context *sdhEngineTable_old_duplicate_row(
        sdhEngineTable_old_context *);
netsnmp_index * sdhEngineTable_old_delete_row(sdhEngineTable_old_context *);
int sdhEngineTable_old_can_delete(sdhEngineTable_old_context *, 
        sdhEngineTable_old_context *, netsnmp_request_group *);

/*************************************************************
 * oid declarations
 */
extern oid sdhEngineTable_old_oid[];
extern size_t sdhEngineTable_old_oid_len;

#define SDHENGINETABLE_OLD_TABLE_OID 1,3,6,1,4,1,27278
    
/*************************************************************
 * column number definitions for table sdhEngineTable
 */
#define COLUMN_ENGSTATUSCODE_OLD 4
#define COLUMN_ENGDRUMLIFE_OLD 5
#define COLUMN_ENGBRIDGE_OLD 6
#define COLUMN_ENGONEBIN_OLD 7
#define COLUMN_ENGCOPYCOUNTER_OLD 8
#define COLUMN_ENGPRINTCOUNTER_OLD 9
#define COLUMN_ENGFAXCOUNT_OLD 10
#define COLUMN_ENGHOSTPRINTCOUNT_OLD 11
#define COLUMN_ENGREPORTPRINTCOUNT_OLD 12
#define COLUMN_ENGDUPLEXCOUNT_OLD 13 // Added by mspyo, 2016-07-05
#define SDHENGINETABLE_OLD_COL_MIN 4
#define SDHENGINETABLE_OLD_COL_MAX 13 // Edited by mspyo from 12 to 13, 2016-07-05

#ifdef __cplusplus
}
#endif

#endif /** SDHENGINETABLE_OLD_H */
