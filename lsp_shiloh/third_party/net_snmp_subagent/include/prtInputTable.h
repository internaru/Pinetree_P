/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtInputTable Printer MIB (Header)
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef PRTINPUTTABLE_H
#define PRTINPUTTABLE_H

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
typedef struct prtInputTable_context_s {

    netsnmp_index index;

#if 0 /* All non-index fields are stored in the OID block */
    /** INTEGER32 = ASN_INTEGER */
        long prtInputIndex;
    /** PrtInputTypeTC = ASN_INTEGER */
        long prtInputType;
    /** PrtMediaUnitTC = ASN_INTEGER */
        long prtInputDimUnit;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputMediaDimFeedDirDeclared;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputMediaDimXFeedDirDeclared;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputMediaDimFeedDirChosen;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputMediaDimXFeedDirChosen;
    /** PrtCapacityUnitTC = ASN_INTEGER */
        long prtInputCapacityUnit;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputMaxCapacity;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputCurrentLevel;
    /** PrtSubUnitStatusTC = ASN_INTEGER */
        long prtInputStatus;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtInputMediaName[64];
        long prtInputMediaName_len;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtInputName[64];
        long prtInputName_len;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtInputVendorName[64];
        long prtInputVendorName_len;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtInputModel[64];
        long prtInputModel_len;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtInputVersion[64];
        long prtInputVersion_len;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtInputSerialNumber[32];
        long prtInputSerialNumber_len;
    /** PrtLocalizedDescriptionStringTC = ASN_OCTET_STR */
        unsigned char prtInputDescription[256];
        long prtInputDescription_len;
    /** PresentOnOff = ASN_INTEGER */
        long prtInputSecurity;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputMediaWeight;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtInputMediaType[64];
        long prtInputMediaType_len;
    /** OCTETSTR = ASN_OCTET_STR */
        unsigned char prtInputMediaColor[64];
        long prtInputMediaColor_len;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputMediaFormParts;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputMediaLoadTimeout;
    /** INTEGER32 = ASN_INTEGER */
        long prtInputNextIndex;
#else
    PRTMIB_OID_MAP *columnInfo;
#endif

} prtInputTable_context;

/*************************************************************
 * APIs for printer application
 */
void init_prtInputTable(void);
void populate_prtInputTable(PRTMIB_OID_MAP *);

/*************************************************************
 * internal function declarations and APIs for net-snmp master agent
 */
//int add_prtInputTable_entry(prtInputTable_context *);
//int del_prtInputTable_entry(long, long);

void initialize_table_prtInputTable(void);
const prtInputTable_context *prtInputTable_get_by_idx(netsnmp_index *);
const prtInputTable_context *prtInputTable_get_by_idx_rs(netsnmp_index *,
                                                int row_status);
int prtInputTable_get_value(netsnmp_request_info *, netsnmp_index *, 
                                                netsnmp_table_request_info *);

void prtInputTable_set_reserve1(netsnmp_request_group *);
void prtInputTable_set_reserve2(netsnmp_request_group *);
void prtInputTable_set_action(netsnmp_request_group *);
void prtInputTable_set_commit(netsnmp_request_group *);
void prtInputTable_set_free(netsnmp_request_group *);
void prtInputTable_set_undo(netsnmp_request_group *);

prtInputTable_context *prtInputTable_duplicate_row(prtInputTable_context *);
netsnmp_index *prtInputTable_delete_row(prtInputTable_context *);

int prtInputTable_can_delete(prtInputTable_context *undo_ctx,
                    prtInputTable_context *row_ctx, netsnmp_request_group *rg);
int chk_prtInputTable_data(prtInputTable_context *, unsigned long);
int prtInputTable_extract_index(prtInputTable_context *, netsnmp_index *);
prtInputTable_context *prtInputTable_create_row(netsnmp_index *);

/*************************************************************
 * oid declarations
 */
extern oid prtInputTable_oid[];
extern size_t prtInputTable_oid_len;

#define prtInputTable_TABLE_OID 1,3,6,1,2,1,43,8,2
    
/*************************************************************
 * column number definitions for table prtInputTable
 */
#define COLUMN_PRTINPUTINDEX 1
#define COLUMN_PRTINPUTTYPE 2
#define COLUMN_PRTINPUTDIMUNIT 3
#define COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED 4
#define COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED 5
#define COLUMN_PRTINPUTMEDIADIMFEEDDIRCHOSEN 6
#define COLUMN_PRTINPUTMEDIADIMXFEEDDIRCHOSEN 7
#define COLUMN_PRTINPUTCAPACITYUNIT 8
#define COLUMN_PRTINPUTMAXCAPACITY 9
#define COLUMN_PRTINPUTCURRENTLEVEL 10
#define COLUMN_PRTINPUTSTATUS 11
#define COLUMN_PRTINPUTMEDIANAME 12
#define COLUMN_PRTINPUTNAME 13
#define COLUMN_PRTINPUTVENDORNAME 14
#define COLUMN_PRTINPUTMODEL 15
#define COLUMN_PRTINPUTVERSION 16
#define COLUMN_PRTINPUTSERIALNUMBER 17
#define COLUMN_PRTINPUTDESCRIPTION 18
#define COLUMN_PRTINPUTSECURITY 19
#define COLUMN_PRTINPUTMEDIAWEIGHT 20
#define COLUMN_PRTINPUTMEDIATYPE 21
#define COLUMN_PRTINPUTMEDIACOLOR 22
#define COLUMN_PRTINPUTMEDIAFORMPARTS 23
#define COLUMN_PRTINPUTMEDIALOADTIMEOUT 24
#define COLUMN_PRTINPUTNEXTINDEX 25
#define prtInputTable_COL_MIN 2
#define prtInputTable_COL_MAX 25

#ifdef __cplusplus
}
#endif

#endif /** PRTINPUTTABLE_H */
