/*
 * =========================================================================== * 
 *    Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * =========================================================================== */

#include "key_table.h"
#include "string.h"
#include "lassert.h"

// linear searches, prevent sparse arrays, and discourage large arrays.
//


const char *key_table_name_by_index( unsigned index, const char *table[] )
{
    unsigned i;
    
    DBG_ASSERT(table);

    for (i = 0; table[i]; i++)
        if ( i == index )
            return table[i];

    return 0;
}

void *key_table_obj_by_index( unsigned index, void** table)
{
    unsigned i;

    DBG_ASSERT(table);

    for (i = 0; table[i]; i++)
        if ( i == index )
            return (void*)table[i];
    return 0;
}

unsigned key_table_index_by_name( const char *name, const char *table[] )
{
    unsigned i;

    DBG_ASSERT(table);

    for (i = 0; table[i]; i++)
    {
        if ( 0 == strcasecmp( name, table[i] ))
        {
            return i;
        }
    }
    return -1;
} 

const char * key_table_name_by_objectptr( void *obj, const char *stab[], void** otab)
{
    unsigned i;
    
    DBG_ASSERT(stab);
    DBG_ASSERT(otab);

    for (i = 0; otab[i]; i++)
        if ( obj == otab[i] )
            return stab[i];
    return 0;
}

