/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Net-SNMP Sub-Agent
 * Marvell OID Interface
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#include <string.h>

#define DBG_PRFX "snmp.mib: "
//off #define DBG_VERBOSE_ON
#include "net_debug.h" // DBG_ family macros
#include "printer_mib_translation.h"
#include "printer_mib_validation.h"

#include "consumables_common_api.h" // CONSUMABLE_CLASS...
#include "oid_sm_api.h"             // OID_SM_PRINT_ORIENTATION_REQUESTED_DEFAULT

static const uint32_t num_HostDevices = 1;
static const uint32_t default_Input = 1;
static const uint32_t num_Markers = 1; // revisit oem-specific NUM_CONSUMABLES
static const uint32_t default_Marker = 1;
static const uint32_t default_prtMediaPathMaxSpeedPrintUnit = 8; // sheetsPerHour rate -- revisit


/*****************************************************************************
  * The table below maps Printer MIBs to Marvell MFP Application OIDs
  * certain types of Printer MIBS.
  *
  * One mapping array is provided for each Printer MIB table. 
  * Each mapping array is described below:
  * 
  *     <Entry 0>  Number of rows in the Printer MIB table
  *     <Entry 1>  Mapping for column 1
  *     <Entry 2>  Mapping for column 2
  *     <Entry 3>  Mapping for column 3
  *                 :
  *                 :
  *                 :
  *
  * Each mapping entry structure is composed of the following fields:
  *
  *         <Entry.marvell_oid>         marvell OID number
  *                                     If equal to COLUMN_NOT_IMPLEMENTED,
  *                                         then this column is not used in
  *                                         the current MFP Application build.
  *                                     If equal to COLUMN_CONSTANT_UINT32,
  *                                         then this column is used in the
  *                                         current MFP Application build but
  *                                         does not map to any marvell OID.
  *                                         Instead, it has a constant numerical
  *                                         value pointed to by <Entry.data>.
  *                                     If equal to COLUMN_CONSTANT_STRING,
  *                                         then this column is used in the
  *                                         current MFP Application build but
  *                                         does not map to any marvell OID.
  *                                         Instead, it has a constant string
  *                                         value pointed to by <Entry.data>.
  *                                     If equal to COLUMN_INDEX,
  *                                         then this column is used in the
  *                                         current MFP Application build but
  *                                         does not map to any marvell OID.
  *                                         Instead, it should have a value
  *                                         based on the Printer MIB table row
  *                                         currently being used.
  *
  *         <Entry.marvell_oid_index>   marvell OID index
  *                                     If equal to INDEX_VARIABLE,
  *                                         then the OID index value must be
  *                                         based on the Printer MIB table row
  *                                         currently being used.
  *
  *         <Entry.convert>             function that converts values between
  *                                     Printer MIB specifications and the 
  *                                     mapped marvell OID
  *                                     <Parameter 1> If true, converts into a
  *                                         value valid for the marvell OID;
  *                                         Else, converts into a value valid
  *                                         for the Printer MIB type.
  *                                     <Parameter 2> Address of the input
  *                                         value; will also hold the output
  *                                     <Parameter 3> Address of the input
  *                                         data length; will also hold the 
  *                                         output data length; only used for
  *                                         byte array OID types
  *
  *         <Entry.data>                constant data pointer; only used if
  *                                     <Entry.marvell_oid> == COLUMN_CONSTAT_x
  *
  * TODO: Modify all mappings with OID_PRTMIB_TEST_x once proper OID mapping
  * has been done
  */

PRTMIB_OID_MAP prtMIB_OIDMap[PRTMIBTABLE_ID_MAX][30] = {
	{
		/** systemSystemTable */
        /** number of rows */
        { COLUMN_CONSTANT_UINT32, 0, NULL, (void *) &num_HostDevices },
                        /** COLUMN_PRTGENERALCONFIGCHANGES 1 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALCURRENTLOCALIZATION 2 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALRESET 3 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTGENERALCURRENTOPERATOR 4 */
        { OID_PLATFORM_PRINT_NAME_STRING, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTGENERALSERVICEPERSON 5 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTINPUTDEFAULTINDEX 6 */
        { OID_SNMP_prtInputDefaultIndex, INDEX_VARIABLE, NULL, NULL }, 
                        /** COLUMN_PRTOUTPUTDEFAULTINDEX 7 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTMARKERDEFAULTINDEX 8 */
        { OID_SNMP_prtMarkerDefaultIndex, INDEX_VARIABLE, NULL, NULL },
	},
	{
		/** hrDeviceTable */
		/** number of rows */
		{ COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
		/** COLUMN_PRTGENERALCONFIGCHANGES 1 */
		{ COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
			/** COLUMN_PRTGENERALCURRENTLOCALIZATION 2 */
		{ COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
		/** COLUMN_PRTGENERALRESET 3 */
		{ COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
		/** COLUMN_PRTGENERALCURRENTLOCALIZATION 2 */
		{ COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
		/** COLUMN_PRTGENERALRESET 3 */
		{ COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
	},
	{
		/** hrPrinterTable */
		/** number of rows */
		{ COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
		/** COLUMN_PRTGENERALCONFIGCHANGES 1 */
		{ COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
	},
    { /** prtGeneralTable */
        /** number of rows */
        { COLUMN_CONSTANT_UINT32, 0, NULL, (void *) &num_HostDevices },
                        /** COLUMN_PRTGENERALCONFIGCHANGES 1 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALCURRENTLOCALIZATION 2 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALRESET 3 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTGENERALCURRENTOPERATOR 4 */
        { OID_PLATFORM_PRINT_NAME_STRING, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTGENERALSERVICEPERSON 5 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTINPUTDEFAULTINDEX 6 */
        { OID_SNMP_prtInputDefaultIndex, INDEX_VARIABLE, NULL, NULL }, 
                        /** COLUMN_PRTOUTPUTDEFAULTINDEX 7 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTMARKERDEFAULTINDEX 8 */
        { OID_SNMP_prtMarkerDefaultIndex, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHDEFAULTINDEX 9 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTCONSOLELOCALIZATION 10 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTCONSOLENUMBEROFDISPLAYLINES 11 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTCONSOLENUMBEROFDISPLAYCHARS 12 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTCONSOLEDISABLE 13 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE 14 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTAUXILIARYSHEETBANNERPAGE 15 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALPRINTERNAME 16 */
        { OID_NETWORK_HOST_NAME, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTGENERALSERIALNUMBER 17 */
        { OID_PLATFORM_PRODUCT_SERIAL_NUM, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTALERTCRITICALEVENTS 18 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTALERTALLEVENTS 19 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
    },
    { /** prtInputTable */
            /** number of rows */
            { OID_PRINT_NUM_INPUT_TRAYS, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTINPUTINDEX 1 */
            { COLUMN_INDEX, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTINPUTTYPE 2 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTDIMUNIT 3 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED 4 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED 5 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIADIMFEEDDIRCHOSEN 6 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIADIMXFEEDDIRCHOSEN 7 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTCAPACITYUNIT 8 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
            /** COLUMN_PRTINPUTMAXCAPACITY 9 */
            { OID_PRTMIB_TEST_SIGNEDNUM, INDEX_VARIABLE, signed_convert, NULL },
                        /** COLUMN_PRTINPUTCURRENTLEVEL 10 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTSTATUS 11 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
            /** COLUMN_PRTINPUTMEDIANAME 12 */
            //{ OID_PRINT_MEDIA_SIZE_DEFAULT /*OID_PRTMIB_TEST_STRING*/, INDEX_VARIABLE, NULL, NULL },
            //{ OID_SNMP_PRINT_MEDIA_SIZE, INDEX_VARIABLE, NULL, NULL },
            //{ OID_SNMP_prtInputMediaName, INDEX_VARIABLE, NULL, NULL },
            { OID_SM_PRINT_MEDIA_DEFAULT, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTINPUTNAME 13 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTVENDORNAME 14 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMODEL 15 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTVERSION 16 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTSERIALNUMBER 17 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTDESCRIPTION 18 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTSECURITY 19 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIAWEIGHT 20 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIATYPE 21 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIACOLOR 22 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIAFORMPARTS 23 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTMEDIALOADTIMEOUT 24 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINPUTNEXTINDEX 25 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
    },
    { /** prtMarkerTable */
            /** number of rows */
            { COLUMN_CONSTANT_UINT32, 0, NULL, (void *) &num_Markers },
            /** COLUMN_PRTMARKERINDEX 1 */
            { COLUMN_CONSTANT_UINT32, 0, NULL, (void *) &default_Marker },
                        /** COLUMN_PRTMARKERMARKTECH 2 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERCOUNTERUNIT 3 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
            /** COLUMN_PRTMARKERLIFECOUNT 4 */
            { OID_SNMP_prtMarkerLifeCount, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERPOWERONCOUNT 5 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERPROCESSCOLORANTS 6 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERSPOTCOLORANTS 7 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERADDRESSABILITYUNIT 8 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERADDRESSABILITYFEEDDIR 9 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERADDRESSABILITYXFEEDDIR 10 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERNORTHMARGIN 11 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERSOUTHMARGIN 12 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERWESTMARGIN 13 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKEREASTMARGIN 14 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMARKERSTATUS 15 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
    },
    { /** prtMarkerSuppliesTable */
            /** number of rows */
            { OID_SNMP_prtMarkerSuppliesTable, INDEX_VARIABLE, NULL, NULL }, // revisit oem-specific NUM_CONSUMABLES
            /** COLUMN_PRTMARKERSUPPLIESINDEX 1 */
            { OID_SNMP_prtMarkerSuppliesIndex, INDEX_VARIABLE, NULL, NULL }, 
            /** COLUMN_PRTMARKERSUPPLIESMARKERINDEX 2 */
            { OID_SNMP_prtMarkerSuppliesMarkerIndex, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERSUPPLIESCOLORANTINDEX 3 */
            { OID_PRTMIB_TEST_UNSIGNEDNUM, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERSUPPLIESCLASS 4 */
            { OID_SNMP_prtMarkerSuppliesClass, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERSUPPLIESTYPE 5 */
            { OID_SNMP_prtMarkerSuppliesType, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERSUPPLIESDESCRIPTION 6 */
            { OID_CONSUMABLE_MODEL_NAME, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERSUPPLIESSUPPLYUNIT 7 */
            { OID_SNMP_prtMarkerSuppliesSupplyUnit, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERSUPPLIESMAXCAPACITY 8 */
            { OID_SNMP_prtMarkerSuppliesMaxCapacity, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERSUPPLIESLEVEL 9 */
            { OID_SNMP_prtMarkerSuppliesLevel, INDEX_VARIABLE, NULL, NULL },
    },
    { /** prtMarkerColorantTable */
            /** number of rows */
            { OID_PRTMIB_TEST_ROWCOUNT, 0, NULL, NULL },
            /** COLUMN_PRTMARKERCOLORANTINDEX 1 */
            { COLUMN_INDEX, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERCOLORANTMARKERINDEX 2 */
            { OID_SNMP_prtMarkerColorantMarkerIndex /*OID_CONSUMABLE_SUPPLY_INDEX*/, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERCOLORANTROLE 3 */
            { OID_SNMP_prtMarkerColorantRole, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERCOLORANTVALUE 4 */
            { OID_SNMP_prtMarkerColorantValue, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMARKERCOLORANTTONALITY 5 */
            { OID_SNMP_prtMarkerColorantTonality, INDEX_VARIABLE, NULL, NULL },
    },
    { /** prtMediaPathTable */
            /** number of rows */
            //revisit { OID_PRTMIB_TEST_ROWCOUNT, 0, NULL, NULL },
            { OID_PRTMIB_TEST_ROWCOUNT, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMEDIAPATHINDEX 1 */
            { COLUMN_INDEX, INDEX_VARIABLE, NULL, NULL },
            /** COLUMN_PRTMEDIAPATHMAXSPEEDPRINTUNIT 2 */
            { COLUMN_CONSTANT_UINT32, 0, prtMediaPathMaxSpeedPrintUnit_convert, (void*) &default_prtMediaPathMaxSpeedPrintUnit },
                        /** COLUMN_PRTMEDIAPATHMEDIASIZEUNIT 3 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHMAXSPEED 4 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHMAXMEDIAFEEDDIR 5 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHMAXMEDIAXFEEDDIR 6 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHMINMEDIAFEEDDIR 7 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHMINMEDIAXFEEDDIR 8 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHTYPE 9 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHDESCRIPTION 10 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTMEDIAPATHSTATUS 11 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
    },
    { /** prtInterpreterTable */
            /** number of rows */
            { OID_PRTMIB_TEST_ROWCOUNT, 0, NULL, NULL },
            /** COLUMN_PRTINTERPRETERINDEX 1 */
            { COLUMN_INDEX, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERLANGFAMILY 2 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERLANGLEVEL 3 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERLANGVERSION 4 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERDESCRIPTION 5 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERVERSION 6 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
            /** COLUMN_PRTINTERPRETERDEFAULTORIENTATION 7 */
#ifdef HAVE_SM_JOB
            // sm_job generates snmp/mib-ready value -- no conversion required
            { OID_PRTMIB_DEFAULT_ORIENTATION, INDEX_VARIABLE, NULL, NULL },
#else
            // otherwise translate default -> snmp/mib-ready value
            { OID_PRTMIB_DEFAULT_ORIENTATION, INDEX_VARIABLE,
                prtInterpreterDefaultOrientation_convert, NULL },
#endif
                        /** COLUMN_PRTINTERPRETERFEEDADDRESSABILITY 8 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERXFEEDADDRESSABILITY 9 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERDEFAULTCHARSETIN 10 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERDEFAULTCHARSETOUT 11 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTINTERPRETERTWOWAY 12 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
    },
	{
		/** sdhEngGeneral */
        /** number of rows */
        { COLUMN_CONSTANT_UINT32, 0, NULL, (void *) &num_HostDevices },
                        /** COLUMN_PRTGENERALCONFIGCHANGES 1 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALCURRENTLOCALIZATION 2 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALRESET 3 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTGENERALCURRENTOPERATOR 4 */
        { OID_PLATFORM_PRINT_NAME_STRING, INDEX_VARIABLE, NULL, NULL },
                        /** COLUMN_PRTGENERALSERVICEPERSON 5 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTINPUTDEFAULTINDEX 6 */
        { OID_SNMP_prtInputDefaultIndex, INDEX_VARIABLE, NULL, NULL }, 
   	},
	{
		/** sdhEngineTable*/
        /** number of rows */
        { COLUMN_CONSTANT_UINT32, 0, NULL, (void *) &num_HostDevices },
                        /** COLUMN_PRTGENERALCONFIGCHANGES 1 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALCURRENTLOCALIZATION 2 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
                        /** COLUMN_PRTGENERALRESET 3 */
                        { COLUMN_NOT_IMPLEMENTED, 0, NULL, NULL },
        /** COLUMN_PRTGENERALCURRENTOPERATOR 4 */
        { OID_PLATFORM_PRINT_NAME_STRING, INDEX_VARIABLE, NULL, NULL },
	},
};

PRTMIB_OID_MAP *get_prtMIB_OIDMap(PRTMIBTABLE_ID tableID)
{
    if (tableID >= PRTMIBTABLE_ID_MAX) {
        PRTMIB_TRACE("invalid table ID: %d", tableID);
        return NULL;
    }

    return prtMIB_OIDMap[tableID];
}

/*****************************************************************************
  * The xxx_convert functions below are stub translation functions for 
  * certain types of Printer MIBS.
  * In printer_mib_validation.x, valid values are listed and
  * value checking routines are defined.
  *
  * TODO: Modify these functions once proper OID mapping has been done and the 
  * valid values for the corresponding OIDs are known.
  */

error_type_t signed_convert(uint32_t inbound, void *data, uint32_t *len)
{
    if (NULL == data)
        return FAIL;

    PRTMIB_TRACE("old value = %08X", *((uint32_t *) data));
    if (inbound) {
        int32_t signed_val = *((int32_t *) data);
        uint32_t *unsigned_val = (uint32_t *) data;
        if (signed_val < 0) {
            *unsigned_val = 0x80000000 | (~(signed_val) + 1);
        }
    }
    else {
        uint32_t unsigned_val = *((uint32_t *) data);
        int32_t *signed_val = (int32_t *) data;
        if (unsigned_val & 0x80000000)
            *signed_val = (unsigned_val & ~0x80000000) * -1;
    }
    PRTMIB_TRACE("new value = %08X", *((uint32_t *) data));

    return OK;
}

error_type_t prtMarkerSuppliesClass_convert(
                                uint32_t inbound, void *data, uint32_t *len)
{
    if (NULL == data)
        return FAIL;

        PRTMIB_TRACE("inbound %d data x%x (@x%x) ", inbound, *(int*)data, (unsigned int)data);
    if (inbound) {
        PRTMIB_TRACE("read-only Printer MIB type");
        return FAIL;
    }
    else {
        uint32_t *value = (uint32_t *) data;

        PRTMIB_TRACE("old class value = %08X", *((uint32_t *) data));
        if (CONSUMABLE_CLASS_CONSUMED_SUPPLY == *value)
            *value = 3; /* supplyThatIsConsumed */
        else if (CONSUMABLE_CLASS_FILLED_RECEPTACLE == *value)
            *value = 4; /* receptacleThatIsFilled */
        else // CONSUMABLE_CLASS_OTHER
            *value = 1; /* other */
        PRTMIB_TRACE("new class value = %08X", *((uint32_t *) data));
    }

    return OK;
}

error_type_t prtMarkerSuppliesType_convert(
                                uint32_t inbound, void *data, uint32_t *len)
{
    if (NULL == data)
        return FAIL;

    if (inbound) {
        PRTMIB_TRACE("read-only Printer MIB type");
        return FAIL;
    }
    else {
        uint32_t *value = (uint32_t *) data;

        PRTMIB_TRACE("old value = %08X", *((uint32_t *) data));
        if (1 == *value)
            *value = PRTMARKERSUPPLIESTYPE_TONER; /* 3 */
        else if (2 == *value)
            *value = PRTMARKERSUPPLIESTYPE_OTHER; /* 1 */
        else
            *value = PRTMARKERSUPPLIESTYPE_UNKNOWN; /* 2 */
        PRTMIB_TRACE("new value = %08X", *((uint32_t *) data));
    }

    return OK;
}

error_type_t prtMarkerSuppliesSupplyUnit_convert(
        uint32_t inbound, void *data, uint32_t *len)
{
    if (NULL == data)
        return FAIL;

    if (inbound) {
        PRTMIB_TRACE("read-only Printer MIB type");
        return FAIL;
    }
    else {
        uint32_t *value = (uint32_t *) data;

        PRTMIB_TRACE("old value = %08X", *((uint32_t *) data));
        if (1 == *value)
            *value = 13; /* tenthsOfGrams */
        else if (2 == *value)
            *value = 14; /* hundrethsOfFluidOunces */
        /** Add other cases here */
        else
            *value = 1; /* other */
        PRTMIB_TRACE("new value = %08X", *((uint32_t *) data));
    }

    return OK;
}

error_type_t prtMarkerColorantRole_convert(
                                uint32_t inbound, void *data, uint32_t *len)
{
    if (NULL == data)
        return FAIL;

    if (inbound) {
        PRTMIB_TRACE("read-only Printer MIB type");
        return FAIL;
    }
    else {
        uint32_t *value = (uint32_t *) data;

        PRTMIB_TRACE("old value = %08X", *((uint32_t *) data));
        if (1 == *value)
            *value = 3; /* process */
        else if (2 == *value)
            *value = 4; /* spot */
        else
            *value = 1; /* other */
        PRTMIB_TRACE("new value = %08X", *((uint32_t *) data));
    }

    return OK;
}

error_type_t prtMarkerColorantValue_convert(
                                uint32_t inbound, void *data, uint32_t *len)
{
    if (NULL == data)
        return FAIL;

    if (inbound) {
        PRTMIB_TRACE("read-only Printer MIB type");
        return FAIL;
    }
    else {
        uint32_t *in_num = (uint32_t *) data;
        char *out_str = (char *) data;

        if (NULL == len || *len < 5) {
            PRTMIB_TRACE("output buffer too small");
            return FAIL;
        }

        PRTMIB_TRACE("old value = %08X", *((uint32_t *) data));
        if (1 == *in_num)
            strcpy(out_str, "white");
        else if (2 == *in_num)
            strcpy(out_str, "red");
        /** Add other cases here */
        else
            strcpy(out_str, "unknown");
        PRTMIB_TRACE("new value = \"%s\"", (char *) data);
    }

    return OK;
}

error_type_t prtMarkerColorantTonality_convert(
                                uint32_t inbound, void *data, uint32_t *len)
{
    if (NULL == data)
        return FAIL;

    if (inbound) {
        PRTMIB_TRACE("read-only Printer MIB type");
        return FAIL;
    }
    else {
        uint32_t *value = (uint32_t *) data;

        PRTMIB_TRACE("old value = %d", *((uint32_t *) data));
        if (*value < 2)
            *value = 2;
        PRTMIB_TRACE("new value = %d", *((uint32_t *) data));
    }

    return OK;
}

error_type_t prtMediaPathMaxSpeedPrintUnit_convert(
                                uint32_t inbound, void *data, uint32_t *len)
{
    if (NULL == data)
        return FAIL;

    if (inbound) {
        PRTMIB_TRACE("read-only Printer MIB type");
        return FAIL;
    }
    else {
        uint32_t *value = (uint32_t *) data;

        PRTMIB_TRACE("old value = %08X", *((uint32_t *) data));
        if (1 == *value)
            *value = 3; /* tenThousandthsOfInchesPerHour */
        else if (2 == *value)
            *value = 4; /* micrometersPerHour */
        else
            *value = 8; /* sheetsPerHour */
        PRTMIB_TRACE("new value = %08X", *((uint32_t *) data));
    }

    return OK;
}

error_type_t prtInterpreterDefaultOrientation_convert(
                                uint32_t inbound, void *data, uint32_t *len)
{
    uint32_t *value = (uint32_t *) data;

    if (NULL == data)
        return FAIL;

    PRTMIB_TRACE("old value = %08X", *((uint32_t *) data));
    if (inbound) {
        if (3 == *value) /* portrait */
            *value = 1;
        else if (4 == *value) /* landscape */
            *value = 2; 
        else /* 1 = other */
            *value = 0;
    }
    else {
        if (1 == *value)
            *value = 3; /* portrait */
        else if (2 == *value)
            *value = 4; /* landscape */
        else
            *value = 1; /* other */
    }
    PRTMIB_TRACE("new value = %08X", *((uint32_t *) data));

    return OK;
}

/*****************************************************************************
 * convert the value of a printer mib into a valid marvell OID value and 
 * set into the OID block
 */
error_type_t set_prtMIB_OIDValue(
    PRTMIB_OID_MAP *map,        /** mapping entry structure */
    uint32_t index,             /** marvell oid index use only when 
                                    map->marvell_oid_index == INDEX_VARIABLE */
    void *data, uint32_t *size) /** input data pointer and length */
{
    uint32_t ndx = map->marvell_oid_index;

    if (NULL == data) {
        PRTMIB_TRACE("NULL input buffer pointer");
        return FAIL;
    }

    if (COLUMN_NOT_IMPLEMENTED == map->marvell_oid) {
        return FAIL;
    }
    else if (COLUMN_CONSTANT_UINT32 == map->marvell_oid ||
             COLUMN_CONSTANT_STRING == map->marvell_oid ||
             COLUMN_INDEX == map->marvell_oid) {
        PRTMIB_TRACE("read-only column");
        return FAIL;
    }

    if (INDEX_VARIABLE == ndx)
        ndx = index;
    switch (oid_type_id_get(map->marvell_oid)) {

        case OID_TYPE_BYTE_ARRAY:
        {
            uint8_t new_bytearray[1024] = {0}; // TODO: revisit
            uint8_t *bytearray = (uint8_t *) data;
            uint32_t bytelength = *size;

            if (NULL != map->convert) {
                memcpy((void *) new_bytearray, (const void *) data, *size);
                if (!OK == (map->convert)(
                                1, (void *) new_bytearray, &bytelength)) {
                    PRTMIB_TRACE("conversion failed");
                    return FAIL;
                }
                bytearray = new_bytearray;
            }

            PRTMIB_TRACE("setting OID %d-byte array", bytelength);
            if (OK != oid_set_bytearray(
                            map->marvell_oid, ndx, bytearray, bytelength)) {
                PRTMIB_TRACE("error writing to OID byte array");
                return FAIL;
            }
            break;
        }

        case OID_TYPE_STRING:
        {
            char *string = (char *) data;
            char new_string[1024] = {0}; // TODO: revisit

            if (NULL != map->convert) {
                strcpy(new_string, (char *) data);
                string = new_string;
                if (!OK == (map->convert)(1, (void *) new_string, NULL)) {
                    PRTMIB_TRACE("conversion failed");
                    return FAIL;
                }
            }

            PRTMIB_TRACE("setting string OID = %s", string);
            if (OK != oid_set_string(
                            map->marvell_oid, ndx, string, strlen(string))) {
                PRTMIB_TRACE("error setting OID string");
                return FAIL;
            }
            break;
        }

        case OID_TYPE_UINT32:
        {
            uint32_t *uint32 = (uint32_t *) data;
            uint32_t new_uint32 = 0;
        
            if (NULL != map->convert) {
                new_uint32 = *((uint32_t *) data);
                uint32 = &new_uint32;
                if (!OK == (map->convert)(1, (void *) &new_uint32, NULL)) {
                    PRTMIB_TRACE("conversion failed");
                    return FAIL;
                }
            }

            PRTMIB_TRACE("setting uint32 OID = %d", *uint32);
            if (OK != oid_set_uint32(
                            map->marvell_oid, ndx, *uint32)) {
                PRTMIB_TRACE("error setting OID uint32");
                return FAIL;
            }
            break;
        }

        default:
            PRTMIB_TRACE("unknown OID type");
            return FAIL;
    }

    PRTMIB_TRACE("success");
    return OK;
}

/*****************************************************************************
 * get the value of a printer mib from the OID block and convert the value
 * according to printer mib specification if required.
 */
error_type_t get_prtMIB_OIDValue(
    PRTMIB_OID_MAP *map,        /** mapping entry structure */
    uint32_t index,             /** marvell oid index use only when 
                                    map->marvell_oid_index == INDEX_VARIABLE */
    void *data, uint32_t *size) /** output data pointer and length */
{
    uint32_t ndx = map->marvell_oid_index ;

    if (NULL == data) {
        PRTMIB_TRACE("NULL output buffer pointer");
        return FAIL;
    }

    if (INDEX_VARIABLE == ndx)
        ndx = index;

    if (COLUMN_NOT_IMPLEMENTED == map->marvell_oid) {
        return FAIL;
    }
    else if (COLUMN_CONSTANT_UINT32 == map->marvell_oid) {

        uint32_t *out = (uint32_t *) data;

        if (NULL == map->data) {
            PRTMIB_TRACE("NULL column constant data");
            return FAIL;
        }
        *out = *((uint32_t *) map->data);
        PRTMIB_TRACE("column constant value = %d", *out);
        return OK;
    }
    else if (COLUMN_CONSTANT_STRING == map->marvell_oid) {

        char *out = (char *) data;

        if (NULL == map->data) {
            PRTMIB_TRACE("NULL column constant data");
            return FAIL;
        }
        strcpy(out, (const char *) map->data);
        PRTMIB_TRACE("column constant string = \"%s\"", out);
        return OK;
    }
    else if (COLUMN_INDEX == map->marvell_oid) {

        uint32_t *out = (uint32_t *) data;
        *out = ndx + 1;
        PRTMIB_TRACE("column index = %d", *out);
        return OK;
    }

    switch (oid_type_id_get(map->marvell_oid)) {

        case OID_TYPE_BYTE_ARRAY:

            if (NULL == size) {
                PRTMIB_TRACE("NULL output size buffer");
                return FAIL;
            }
            if (OK != oid_get_bytearray(
                            map->marvell_oid, ndx, (uint8_t *) data, size)) {
                PRTMIB_TRACE("error reading OID x%x byte array", map->marvell_oid);
                return FAIL;
            }
            PRTMIB_TRACE("bytearray[%d] = %d-bytes", ndx, *size);
            break;

        case OID_TYPE_STRING:

            if (NULL == size) {
                PRTMIB_TRACE("NULL output size buffer");
                return FAIL;
            }
            if (OK != oid_get_string(
                            map->marvell_oid, ndx, (char *) data, *size)) {
                PRTMIB_TRACE("error reading OID x%x string (index %d)", map->marvell_oid, index);
                return FAIL;
            }
            PRTMIB_TRACE("string[%d] = %s", ndx, (char *) data);
            break;

        case OID_TYPE_UINT32:

            if (OK != oid_get_uint32(
                            map->marvell_oid, ndx, (uint32_t *) data)) {
#if defined DBG_VERBOSE_ON
                char* oid_name = get_oid_literal(map->marvell_oid);
                if (!oid_name) oid_name = "<unregistered oid?>";
                DBG_VERBOSE("error reading OID x%x uint32 (%s)\n", map->marvell_oid, oid_name);
#else
                PRTMIB_TRACE("error reading OID x%x uint32", map->marvell_oid);
#endif
                return FAIL;
            }
            PRTMIB_TRACE("uint32[%d] = %d", ndx, *((uint32_t *) data));
            break;

        default:

            PRTMIB_TRACE("unknown OID type");
            return FAIL;
    }

    if (NULL != map->convert) {
        if (OK != (map->convert)(0, data, size)) {
            PRTMIB_TRACE("conversion failed");
            return FAIL;
        }
    }

    PRTMIB_TRACE("success");
    return OK;
}
