/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * hrPrinterTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef HRPRINTERTABLE_H
#define HRPRINTERTABLE_H

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

typedef struct hrPrinterTable_context_s {

    netsnmp_index index;
    PRTMIB_OID_MAP *columnInfo;

} hrPrinterTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_hrPrinterTable(void);
void populate_hrPrinterTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_hrPrinterTable_entry(hrPrinterTable_context *);
//int del_hrPrinterTable_entry(long);

void initialize_table_hrPrinterTable(void);
const hrPrinterTable_context *hrPrinterTable_get_by_idx(netsnmp_index *);
const hrPrinterTable_context *hrPrinterTable_get_by_idx_rs(
        netsnmp_index *, int row_status);
int hrPrinterTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

hrPrinterTable_context *hrPrinterTable_create_row(netsnmp_index *);
int hrPrinterTable_extract_index(hrPrinterTable_context *, netsnmp_index *);

void hrPrinterTable_set_reserve1(netsnmp_request_group *);
void hrPrinterTable_set_reserve2(netsnmp_request_group *);
void hrPrinterTable_set_action(netsnmp_request_group *);
void hrPrinterTable_set_commit(netsnmp_request_group *);
void hrPrinterTable_set_free(netsnmp_request_group *);
void hrPrinterTable_set_undo(netsnmp_request_group *);

hrPrinterTable_context *hrPrinterTable_duplicate_row(
        hrPrinterTable_context *);
netsnmp_index * hrPrinterTable_delete_row(hrPrinterTable_context *);
int hrPrinterTable_can_delete(hrPrinterTable_context *, 
        hrPrinterTable_context *, netsnmp_request_group *);

/*************************************************************
 * oid declarations
 */
extern oid hrPrinterTable_oid[];
extern size_t hrPrinterTable_oid_len;

#define hrPrinterTable_TABLE_OID 1,3,6,1,2,1,25,3,5
    
/*************************************************************
 * column number definitions for table hrPrinterTable
 */
#define COLUMN_HRPRINTERSTATUS 1
#define COLUMN_HRPRINTERDETECTEDERRORSTATE 2
#define hrPrinterTable_COL_MIN 1
#define hrPrinterTable_COL_MAX 2 

#ifdef __cplusplus
}
#endif

#endif /** HRPRINTERTABLE_H */
