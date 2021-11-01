/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* Implementation of snmp 'marker' mibs -> oids (as defined in Printer-MIB.txt)

// initial set:

    prtMarkerTable (number of rows)

	prtMarkerIndex (not-accessible)
	prtMarkerLifeCount (read-only)

    prtMarkerSuppliesTable (number of rows)

	prtMarkerSuppliesIndex (not-accessible)
	prtMarkerSuppliesMarkerIndex (read-only)
	prtMarkerSuppliesColorantIndex (read-only)
	prtMarkerSuppliesClass (read-only)
	prtMarkerSuppliesType (read-only)
	prtMarkerSuppliesDescription (read-only)
	prtMarkerSuppliesSupplyUnit (read-only)
	prtMarkerSuppliesMaxCapacity (read-only)
	prtMarkerSuppliesLevel (read-only)

    prtMarkerColorantTable (number of rows)

	prtMarkerColorantIndex (not-accessible)
	prtMarkerColorantMarkerIndex (read-only)
	prtMarkerColorantRole (read-only)
	prtMarkerColorantValue (read-only)
	prtMarkerColorantTonality (read-only)
*/

#include <stdint.h>
#include <stdlib.h>
#include "oid_api.h" // #includes "oid_vendor_api.h"
#include "oid_producer_api.h"
#include "platform_api.h"
#include "platform.h"
#include "utils.h"
#include "oid_sm_api.h" // OID_SM_PRINT_MEDIA_DEFAULT
#include "location_api.h"

#define DBG_PRFX "oem.snmp.mib.marker: "
//off #define DBG_VERBOSE_ON
//off #define DBG_ERR_ON
#include "net_debug.h" // DBG_ family macros

#include <string.h>
#include "uuid.h"
#include "nvram_api.h"

#include "string_mgr_config.h" // string_mgr_localization_type_t
#include "oid_vendor_api.h"    // snmp/pjl/job-info oids
#include "cmd_proc_api.h"      // cmd_append_result()
#include "consumables_api.h"   // consumable_set_default_orientation()
#include "oid_api.h"


static error_type_t oem_oid_get_int( oid_t oid, uint32_t index, uint32_t* value);
static error_type_t oem_oid_get_string( oid_t oid, uint32_t index, char* value, uint32_t max_bytes);

// marker usage life-count (sofar) (revisit lifetime-pages vs lifetime-cartridge usage)
error_type_t backend_get_prtMarkerLifeCount(oid_t oid, uint32_t index, uint32_t* value)
{
    DBG_VERBOSE("%s() returning OID_PRINT_HEAD_TOTAL_PAGES_PRINTED\n", __func__);
    return oid_get_uint32( OID_PRINT_HEAD_TOTAL_PAGES_PRINTED, index, value);
}

// marker supplies
error_type_t backend_get_prtMarkerSuppliesIndex(oid_t oid, uint32_t index, uint32_t* value)
{
    return oem_oid_get_int( OID_SNMP_prtMarkerSuppliesIndex, index, value);
}
error_type_t backend_get_prtMarkerSuppliesMarkerIndex(oid_t oid, uint32_t index, uint32_t* value)
{
    return oem_oid_get_int( OID_SNMP_prtMarkerSuppliesMarkerIndex, index, value);
}
error_type_t backend_get_prtMarkerSuppliesClass(oid_t oid, uint32_t index, uint32_t* value)
{
    *value = 3; // CONSUMABLE_CLASS_CONSUMED_SUPPLY (see Printer-MIB.txt)
    return OK;
}
error_type_t backend_get_prtMarkerSuppliesType(oid_t oid, uint32_t index, uint32_t* value)
{
    *value = 3; // CONSUMABLE_TYPE_TONER (see Printer-MIB.txt)
    return OK;
}
error_type_t backend_get_prtMarkerSuppliesDescription(oid_t oid, uint32_t index, char* value, uint32_t max_len)
{
    return oem_oid_get_string( OID_SNMP_prtMarkerSuppliesDescription, index, value, max_len);
}
error_type_t backend_get_prtMarkerSuppliesSupplyUnit(oid_t oid, uint32_t index, uint32_t* value)
{
    *value = CONSUMABLE_UNITS_KILOPIXELS; // 'kilo pixels' // revisit other units (see Printer-MIB.txt)
    return OK;
}
error_type_t backend_get_prtMarkerSuppliesMaxCapacity(oid_t oid, uint32_t index, uint32_t* value)
{
    *value = 60000; // revisit capacity of SupplyUnit -- (see Printer-MIB.txt)
    return OK;
}
error_type_t backend_get_prtMarkerSuppliesLevel(oid_t oid, uint32_t index, uint32_t* value)
{
    *value = -3; // 'some supply remaining' // revisit dynamic (see Printer-MIB.txt)
    return OK;
}

// marker colorants
error_type_t backend_get_prtMarkerColorantIndex(oid_t oid, uint32_t index, uint32_t* value)
{
    return oem_oid_get_int( OID_SNMP_prtMarkerColorantIndex, index, value);
}
error_type_t backend_get_prtMarkerColorantMarkerIndex(oid_t oid, uint32_t index, uint32_t* value)
{
    *value = 1; // 1 or zero? revisit (see Printer-MIB.txt)
    return OK;
}
error_type_t backend_get_prtMarkerColorantRole /* OID_CONSUMABLE_SUPPLY_ROLE */(oid_t oid, uint32_t index, uint32_t* value)
{
    *value = CONSUMABLE_COLORANT_SPOT; // (see Printer-MIB.txt)
    return OK;
}
error_type_t backend_get_prtMarkerColorantValue(oid_t oid, uint32_t index, char* value, uint32_t max_str_bytes)
{
    ASSERT(value);
    if (value && max_str_bytes > 0)
    {
        int max_str_len = (strlen("black")>max_str_bytes?max_str_bytes-1:strlen("black")+1);
        strncpy(value, "black", max_str_len); // see Printer-MIB.txt
        value[max_str_len]='\0';
    }
    return OK;
}
error_type_t backend_get_prtMarkerColorantTonality(oid_t oid, uint32_t index, uint32_t* value)
{
    *value = 600; // revisit genuine tonality measure (currently as 'resolution')
    return OK;
}

static uint32_t printermib_test_rowcount = 3; // revisit oem (dynamic?) setting
static error_type_t oem_oid_get_int( oid_t oid, uint32_t index, uint32_t * value)
{
    error_type_t e_res = OK;

    if ( NULL == value )
    {
        DBG_ERR("%s(x%x:%d) no destination buffer?\n", __func__, oid, index);
        return FAIL;
    }

    switch ( oid )
    {
        case OID_PRTMIB_DEFAULT_ORIENTATION:
        {
	    e_res = consumable_get_default_orientation(index, value); // index treated as marker # (not parser?)
	    break;
        }

    case OID_SNMP_prtMarkerSuppliesTable:
    case OID_SNMP_prtMarkerSuppliesMarkerIndex:
	{
	    *value = consumable_get_num_consumables();
	    e_res = OK;
	}

    default:
            e_res = OID_ERROR_OID_INVALID;
            break;
    }

    return e_res;
}

static error_type_t oem_oid_get_string( oid_t oid,
                                             uint32_t index,
                                             char * data_ptr,
                                             uint32_t max_string_len_in_bytes )
{
    //unused uint32_t string_size;
    error_type_t e_res = OK;

    if ( NULL == data_ptr || 
            max_string_len_in_bytes < 1 ||
                index >= printermib_test_rowcount )
    {
        cmd_append_result("%s: FAILED! lookup oid %x\n", __FUNCTION__, oid);
        return FAIL;
    }

    cmd_append_result("%s: lookup oid %x\n", __FUNCTION__, oid);

    switch ( oid )
    {
        case OID_SNMP_prtMarkerSuppliesDescription:
        case OID_SNMP_prtMarkerColorantValue:
        case OID_SNMP_prtInputMediaName: // case OID_SNMP_PRINT_MEDIA_SIZE:
        {
          DBG_ERR("%s() lookup oid x%x '%s'\n", __func__, oid, get_oid_literal(oid));
        }
        break;

        default:
          DBG_ERR("%s() oid x%x unhandled\n", __func__, oid);

            e_res = OID_ERROR_OID_INVALID;
            break;
    }

    return e_res;
}

// revisit
error_type_t backend_null_set(oid_t oid, uint32_t index, uint32_t* value)
{
    DBG_VERBOSE("%s() oid x%x '%s' trap (not implemented)\n", __func__, oid, get_oid_literal(oid));
    return ~OK;
}

// revisit
error_type_t backend_null_test(oid_t oid, uint32_t index)
{
    DBG_VERBOSE("%s() oid x%x '%s' trap (not implemented)\n", __func__, oid, get_oid_literal(oid));
    return ~OK;
}

error_type_t register_oem_marker_oids( void ) {
    error_type_t e_res = OK;

    e_res = oid_register_callbacks( OID_SNMP_prtMarkerLifeCount,
                                    backend_get_prtMarkerLifeCount,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerSuppliesClass,
                                    backend_get_prtMarkerSuppliesClass,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerSuppliesType,
                                    backend_get_prtMarkerSuppliesType,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerSuppliesDescription,
                                    backend_get_prtMarkerSuppliesDescription,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerSuppliesSupplyUnit, // OID_CONSUMABLE_SUPPLY_UNITS
                                    backend_get_prtMarkerSuppliesSupplyUnit,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerSuppliesMaxCapacity, // OID_CONSUMABLE_SUPPLY_CAPACITY
                                    backend_get_prtMarkerSuppliesMaxCapacity,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerSuppliesLevel, // OID_CONSUMABLE_SUPPLY_USED
                                    backend_get_prtMarkerSuppliesLevel,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    // marker colorants
    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerColorantMarkerIndex,
                                    backend_get_prtMarkerColorantMarkerIndex,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerColorantRole,
                                    backend_get_prtMarkerColorantRole,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerColorantValue,
                                    backend_get_prtMarkerColorantValue,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

    e_res |= oid_register_callbacks( OID_SNMP_prtMarkerColorantTonality,
                                    backend_get_prtMarkerColorantTonality,
                                    backend_null_set,
                                    backend_null_test );
    XASSERT(e_res==OK, e_res);

return e_res;
}
