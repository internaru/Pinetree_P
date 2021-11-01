/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * hrDeviceTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef HRDEVICETABLE_H
#define HRDEVICETABLE_H

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

typedef struct hrDeviceTable_context_s {

    netsnmp_index index;
    PRTMIB_OID_MAP *columnInfo;

} hrDeviceTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_hrDeviceTable(void);
void populate_hrDeviceTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_hrDeviceTable_entry(hrDeviceTable_context *);
//int del_hrDeviceTable_entry(long);

void initialize_table_hrDeviceTable(void);
const hrDeviceTable_context *hrDeviceTable_get_by_idx(netsnmp_index *);
const hrDeviceTable_context *hrDeviceTable_get_by_idx_rs(
        netsnmp_index *, int row_status);
int hrDeviceTable_get_value(
        netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

hrDeviceTable_context *hrDeviceTable_create_row(netsnmp_index *);
int hrDeviceTable_extract_index(hrDeviceTable_context *, netsnmp_index *);

void hrDeviceTable_set_reserve1(netsnmp_request_group *);
void hrDeviceTable_set_reserve2(netsnmp_request_group *);
void hrDeviceTable_set_action(netsnmp_request_group *);
void hrDeviceTable_set_commit(netsnmp_request_group *);
void hrDeviceTable_set_free(netsnmp_request_group *);
void hrDeviceTable_set_undo(netsnmp_request_group *);

hrDeviceTable_context *hrDeviceTable_duplicate_row(
        hrDeviceTable_context *);
netsnmp_index * hrDeviceTable_delete_row(hrDeviceTable_context *);
int hrDeviceTable_can_delete(hrDeviceTable_context *, 
        hrDeviceTable_context *, netsnmp_request_group *);

/*************************************************************
 * oid declarations
 */
extern oid hrDeviceTable_oid[];
extern size_t hrDeviceTable_oid_len;

#define hrDeviceTable_TABLE_OID 1,3,6,1,2,1,25,3,2
    
/*************************************************************
 * column number definitions for table hrDeviceTable
 */
#define COLUMN_HRDEVICEINDEX 1
#define COLUMN_HRDEVICETYPE 2
#define COLUMN_HRDEVICEDESCR 3 
#define COLUMN_HRDEVICEID 4
#define COLUMN_HRDEVICESTATUS 5
#define COLUMN_HRDEVICEERRORS 6
#define hrDeviceTable_COL_MIN 1
#define hrDeviceTable_COL_MAX 6 

#ifdef __cplusplus
}
#endif

#endif /** HRDEVICETABLE_H */
