/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Net-SNMP Sub-Agent for Printer MIBs
 * Marvell OID Interface Header
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */


#ifndef PRINTER_MIB_TRANSLATION_H
#define PRINTER_MIB_TRANSLATION_H

#ifdef __cplusplus
extern "C" {
#endif


#include "oid_producer_api.h"

// accommodate 'format-only-no-args' as well..
#define PRTMIB_TRACE(format,args...) DBG_VERBOSE(format "\n", ##args)

/* Only one printer device for this system */
#define PRTMIB_THIS_HOST_DEVICE_INDEX 1
#define PRIVATE_SINDOH_LEAF_NODE 0

/* 
 * Use the following values whenever a 
 * GET request is received for unimplemented columns
 */
#define PRTMIB_STR_UNIMPLEMENTED ""
#define PRTMIB_NUM_UNIMPLEMENTED 0

/*
 * Printer MIB to Marvell OID mapping 
 */
typedef enum {
	SYSTEM_ID_SYSTEM,
    HOSTMIBTABLE_ID_HRDEVICE,
    HOSTMIBTABLE_ID_HRPRINTER,
    PRTMIBTABLE_ID_GENERAL,
    //PRTMIBTABLE_ID_COVER,
    //PRTMIBTABLE_ID_LOCALIZATION,
    PRTMIBTABLE_ID_INPUT,
    //PRTMIBTABLE_ID_OUTPUT,
    PRTMIBTABLE_ID_MARKER,
    PRTMIBTABLE_ID_MARKERSUPPLIES,
    PRTMIBTABLE_ID_MARKERCOLORANT,
    PRTMIBTABLE_ID_MEDIAPATH,
    //PRTMIBTABLE_ID_CHANNEL,
    PRTMIBTABLE_ID_INTERPRETER,
    //PRTMIBTABLE_ID_CONSOLEDISPLAYBUFFER,
    //PRTMIBTABLE_ID_CONSOLELIGHTS,
    //PRTMIBTABLE_ID_ALERT,
	SDH_ID_SDHENGGENERAL,
//	SDH_ID_SDHENGSTATUS,
	SDH_ID_SDHENGINE_OLD,
	SDH_ID_SDHENGINE,
    PRTMIBTABLE_ID_MAX
} PRTMIBTABLE_ID;

typedef error_type_t (*PRTMIB_OID_CONVERT_VALUE)(uint32_t, void *, uint32_t *);

typedef struct {
    oid_t marvell_oid;
    uint32_t marvell_oid_index;
    PRTMIB_OID_CONVERT_VALUE convert;
    void *data;
} PRTMIB_OID_MAP;

#define COLUMN_NOT_IMPLEMENTED  0xFFFFFFFF
#define COLUMN_CONSTANT_UINT32  0xFFFFFFFE
#define COLUMN_CONSTANT_STRING  0xFFFFFFFD
#define COLUMN_INDEX            0xFFFFFFFC

#define INDEX_VARIABLE          0xFFFFFFFF

#define IS_COLUMN_IMPLEMENTED(map, col) \
                                (map[col].marvell_oid != COLUMN_NOT_IMPLEMENTED)


error_type_t signed_convert(
                        uint32_t, void *, uint32_t *);
error_type_t prtMarkerSuppliesClass_convert(
                        uint32_t, void *, uint32_t *);
error_type_t prtMarkerSuppliesType_convert(
                        uint32_t, void *, uint32_t *);
error_type_t prtMarkerSuppliesSupplyUnit_convert(
                        uint32_t, void *, uint32_t *);
error_type_t prtMarkerColorantRole_convert(
                        uint32_t, void *, uint32_t *);
error_type_t prtMarkerColorantValue_convert(
                        uint32_t, void *, uint32_t *);
error_type_t prtMarkerColorantTonality_convert(
                        uint32_t, void *, uint32_t *);
error_type_t prtMediaPathMaxSpeedPrintUnit_convert(
                        uint32_t, void *, uint32_t *);
error_type_t prtInterpreterDefaultOrientation_convert(
                        uint32_t, void *, uint32_t *);

/*
 * Marvell OID interface wrappers
 */
PRTMIB_OID_MAP *get_prtMIB_OIDMap(PRTMIBTABLE_ID);

error_type_t set_prtMIB_OIDValue(
                            PRTMIB_OID_MAP *, uint32_t, void *, uint32_t *);
error_type_t get_prtMIB_OIDValue(
                            PRTMIB_OID_MAP *, uint32_t, void *, uint32_t *);

#ifdef __cplusplus
}
#endif

#endif /** PRINTER_MIB_TRANSLATION_H */
