/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_unit_test.c
 *
 * \brief This file implements a unit test for the OID API. 
 * 
 **/

#include <stdint.h>
#include <string.h>
#include "oid_api.h"

#include "dprintf.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#define DBG_CMD(...) cmd_printf( __VA_ARGS__ )
#else
#define DBG_CMD(...)
#endif


/**
 *
 * \brief OID Unit test function.  Tests OID API functionality
 *
 * \return int32_t 0 upon success, otherwise a negative number
 *
 */
int oid_unit_test( void )
{
    uint8_t area;
    uint8_t value;
    uint8_t module;
    uint8_t type;
    uint8_t i;
    uint8_t j;
    uint8_t k;
    uint8_t l;
    oid_t oid;
    error_type_t e_res;
    uint32_t int_value;
    #define STRING_BUFFER_SIZE 1024
    char str_value[ STRING_BUFFER_SIZE ];

    /* test creating a random oid */
    oid = oid_make( 255, 254, 253, 252 );
    area = oid_area_id_get(oid);
    value = oid_value_id_get(oid);
    module = oid_namespace_get(oid);
    type = oid_type_id_get(oid);

    if ( ( area != 254 ) ||
         ( value != 253 ) ||
         ( module != 255 ) ||
         ( type != 252 ) )
    {
        DBG_CMD( "Failed to retrieve correct sub values from oid\n" );
        return __LINE__;
    }

    /* test creating all valid oids, up to the combo max */
    DBG_CMD("Testing all OID combinations\n");
    #define OID_COMBO_MAX 10
    for ( i = 0; i < OID_COMBO_MAX; i++ )
    {
        for ( j = 0; j < OID_COMBO_MAX; j++ )
        {
            for ( k = 0; k < OID_COMBO_MAX; k++ )
            {
                for ( l = 0; l < OID_COMBO_MAX; l++ )
                {

                    oid = oid_make( i, j, k, l );
                    area = oid_area_id_get(oid);
                    value = oid_value_id_get(oid);
                    module = oid_namespace_get(oid);
                    type = oid_type_id_get(oid);

                    if ( ( area != j ) ||
                         ( value != k ) ||
                         ( module != i ) ||
                         ( type != l ) )
                    {
                        DBG_CMD("Error: area = %u (%u) value = %u (%u) module = %u (%u) type = %u (%u)\n",
                                    area, j, value, k, module, i, type, l );
                        return __LINE__;
                    }
                }
            }
        }
    }

    /* test an already made OID */
    oid = OID_NETWORK_IPV4_ADDRESS;
    area = oid_area_id_get(oid);
    value = oid_value_id_get(oid);
    module = oid_namespace_get(oid);
    type = oid_type_id_get(oid);
    DBG_CMD("Testing IPv4 Address oid\n" );

    if ( ( area != OID_AREA_NETWORK ) ||
         ( value != 25 ) ||
         ( module != OID_NAMESPACE_SYSTEM ) ||
         ( type != OID_TYPE_STRING ) )
    {
        DBG_CMD("Failed: area = %u (%u) value = %u (%u) module = %u (%u) type = %u (%u)\n",
                   area, OID_AREA_NETWORK, value, 25, module, OID_NAMESPACE_SYSTEM, type, OID_TYPE_STRING );
        return __LINE__;
    }

    /* test a known implemented pre-made OID */
    oid = OID_MEMORY_TOTAL_SIZE_IN_MBYTES;
    area = oid_area_id_get(oid);
    value = oid_value_id_get(oid);
    module = oid_namespace_get(oid);
    type = oid_type_id_get(oid);

    DBG_CMD("Testing OID_MEMORY_TOTAL_SIZE_IN_MBYTES\n" );
    if ( ( area != OID_AREA_MEMORY ) ||
         ( value != 0 ) ||
         ( module != OID_NAMESPACE_SYSTEM ) ||
         ( type != OID_TYPE_UINT32 ) )
    {
        DBG_CMD("area = %u (%u) value = %u (%u) module = %u (%u) type = %u (%u)\n",
                    area, OID_AREA_MEMORY, value, 0, module, OID_NAMESPACE_SYSTEM, type, OID_TYPE_UINT32 );
        return __LINE__;
    }

    e_res = oid_get_uint32( oid, 0, &int_value );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in oid_get_uint32 of OID_MEMORY_TOTAL_SIZE_IN_MBYTES\n" );
        return __LINE__;
    }
    DBG_CMD( "Memory Total Size In MBytes: %d\n", (int)int_value ); 
    

    /* cannot set memory module values, memory total and left are controlled by
     * dynamic allocations and physical memory sizes */
    e_res = oid_set_uint32( oid, 0, int_value );
    if ( FAIL != e_res )
    {
        DBG_CMD("Failed in oid_set_uint32 of OID_MEMORY_TOTAL_SIZE_IN_MBYTES - should not have passed\n" );
        return __LINE__;
    }

    /* test another known implemented pre-made OID */
    oid = OID_MEMORY_SYS_HEAP_AVAIL_IN_MBYTES;
    area = oid_area_id_get(oid);
    value = oid_value_id_get(oid);
    module = oid_namespace_get(oid);
    type = oid_type_id_get(oid);

    if ( ( area != OID_AREA_MEMORY ) ||
         ( value != 2 ) ||
         ( module != OID_NAMESPACE_SYSTEM ) ||
         ( type != OID_TYPE_UINT32 ) )
    {
        DBG_CMD("Failed OID_MEMORY_SYS_HEAP_AVAIL_IN_MBYTES: area = %u (%u) value = %u (%u) module = %u (%u) type = %u (%u)\n",
                    area, OID_AREA_MEMORY, value, 2, module, OID_NAMESPACE_SYSTEM, type, OID_TYPE_UINT32 );

        return __LINE__;
    }

    e_res = oid_get_uint32( oid, 0, &int_value );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in oid_get_uint32 of OID_MEMORY_SYS_HEAP_AVAIL_IN_MBYTES\n" );
        return __LINE__;
    }
    DBG_CMD("OID Memory Left In MBytes: %d\n", (int)int_value); 

    /* cannot set memory module values, memory total and left are controlled by
     * dynamic allocations and physical memory sizes */
    e_res = oid_set_uint32( oid, 0, int_value );
    if ( FAIL != e_res )
    {
        DBG_CMD("Failed in set of uint32 for OID_MEMORY_SYS_HEAP_AVAIL_IN_MBYTES: %d\n", (int)int_value); 
        return __LINE__;
    }

    /* test another known implemented pre-made OID that will both set and get as uint32*/
    oid = OID_PLATFORM_IO_TIMEOUT;
    area = oid_area_id_get(oid);
    value = oid_value_id_get(oid);
    module = oid_namespace_get(oid);
    type = oid_type_id_get(oid);

    if ( ( area != OID_AREA_PLATFORM ) ||
         ( value != 8 ) ||
         ( module != OID_NAMESPACE_SYSTEM ) ||
         ( type != OID_TYPE_UINT32 ) )
    {
        DBG_CMD("Failed OID_PLATFORM_IO_TIMEOUT: area = %u (%u) value = %u (%u) module = %u (%u) type = %u (%u)\n",
                    area, OID_AREA_PLATFORM, value, 8, module, OID_NAMESPACE_SYSTEM, type, OID_TYPE_UINT32 );

        return __LINE__;
    }

    e_res = oid_get_uint32( oid, 0, &int_value );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in oid_get_uint32 of OID_PLATFORM_IO_TIMEOUT\n" );
        return __LINE__;
    }
    DBG_CMD("OID_PLATFORM_IO_TIMEOUTs: %d\n", (int)int_value); 

    e_res = oid_test_uint32( oid, 0, int_value );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in test of uint32 for OID_PLATFORM_IO_TIMEOUTs: %d\n", (int)int_value); 
        return __LINE__;
    }

    e_res = oid_set_uint32( oid, 0, int_value );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in set of uint32 for OID_PLATFORM_IO_TIMEOUTs: %d\n", (int)int_value); 
        return __LINE__;
    }

    /* test another known implemented pre-made OID that will both set and get as string*/
    oid = OID_HTTP_PASSWORD;
    area = oid_area_id_get(oid);
    value = oid_value_id_get(oid);
    module = oid_namespace_get(oid);
    type = oid_type_id_get(oid);

    if ( ( area != OID_AREA_HTTP ) ||
         ( value != 2 ) ||
         ( module != OID_NAMESPACE_SYSTEM ) ||
         ( type != OID_TYPE_STRING ) )
    {
        DBG_CMD("Failed OID_HTTP_PASSWORD: area = %u (%u) value = %u (%u) module = %u (%u) type = %u (%u)\n",
                    area, OID_AREA_HTTP, value, 2, module, OID_NAMESPACE_SYSTEM, type, OID_TYPE_STRING );

        return __LINE__;
    }

    e_res = oid_get_string(oid, 0, str_value, STRING_BUFFER_SIZE );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in oid_get_string of OID_HTTP_PASSWORD\n" );
        return __LINE__;
    }
    DBG_CMD("OID_HTTP_PASSWORD: %s\n", str_value); 

    e_res = oid_test_string( oid, 0, "password", 8 );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in test of oid_set_string for OID_HTTP_PASSWORD: password\n" ); 
        return __LINE__;
    }

    e_res = oid_set_string( oid, 0, "password", 8 );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in set of oid_set_string for OID_HTTP_PASSWORD: password\n" ); 
        return __LINE__;
    }
    e_res = oid_set_string( oid, 0, "", 0 );
    if ( OK != e_res )
    {
        DBG_CMD("Failed in set of oid_set_string for OID_HTTP_PASSWORD: [empty]\n" ); 
        return __LINE__;
    }

    return 0;
}


