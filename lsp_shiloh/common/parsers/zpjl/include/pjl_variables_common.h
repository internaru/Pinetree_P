/*
 *
 * ============================================================================
 * Copyright (c) 2013  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef PJL_VARIABLES_COMMON_H
#define PJL_VARIABLES_COMMON_H


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/// object copy of the state of a variable
/// integer type 
typedef struct pji_int_s {
	uint32_t as_int;    ///< int uint enum bool 
	bool     is_set;    ///< a set variable that takes precedence over defaults.
	uint32_t pji_index; ///< index into class table allows object and class to have different index orders
} pji_int_t;

/// object copy of the state of a variable
/// string type 
typedef struct pji_str_s {   
	char  as_str[40];   ///< string value
	bool  is_set;       ///< a set variable that takes precedence over defaults.
	uint32_t pji_index; ///< index into class table allows object and class to have different index orders
} pji_str_t;

#include "pjl_variables.h"  ///< defines the table of variables varies per product.
#include "PrintJobInfo.h"   ///< copy of the state is scoped at the job / pipe / system level. 
#include "oid_api.h"        ///< all the defaults have an oid interface. 


/// Use the macro helpers not the _impl functions please, 
/// to allow for changes in implementation under the api.

/// don't call this directly look for the function with the _impl
uint32_t PrintJobInfo_int_field_get_impl( pji_int_t *pvar );
/// don't call this directly look for the function with the _impl
const char * PrintJobInfo_str_field_get_impl( pji_str_t *pvar );
/// don't call this directly look for the function with the _impl
uint32_t pji_int_field_get_impl( pji_int_t *pvar, bool isdefault );
/// don't call this directly look for the function with the _impl
uint32_t pji_int_field_set_impl( pji_int_t *pvar, uint32_t var, bool isdefault );
/// don't call this directly look for the function with the _impl
const char * pji_str_field_get_impl( pji_str_t *pvar, bool isdefault );
/// don't call this directly look for the function with the _impl
uint32_t pji_str_field_set_impl( pji_str_t *pvar, const char *var, bool isdefault );


/// job / parser / pdl interface. These are scoped values base on which PrintJobInfo is passed in.
/// int value = PrintJobInfo_int_field_get( PrintJobInfo_t*, JAMRECOVERY );
///
#define PrintJobInfo_int_field_get( pji, field ) \
	PrintJobInfo_int_field_get_impl( &pji->local.i.by_name.field )

/// job / parser / pdl interface. Use the macro helpers not the _impl functions please. 
/// const char * string = PrintJobInfo_str_field_get( PrintJobInfo_t*, JOBNAME );
#define PrintJobInfo_str_field_get( pji, field ) \
	PrintJobInfo_str_field_get_impl( &pji->local.s.by_name.field )

/// pjl and pji interface. Use the macro helpers not the _impl functions please.
/// pji_int_field_get_by_index( pji, &var, 0, eDefault );
/// pji_int_field_get_by_name( pji, &var, JAMRECOVERY, eDefault );
///
/// These allow the getting and setting of the default variable the (class) 
/// verses the object which has a different lifetime.   
///
#define pji_int_field_get_by_name( pji, ivar, field, isdefault ) \
	pji_int_field_get_impl( &pji->local.i.by_name.field, ivar, isdefault )
#define pji_int_field_set_by_name( pji, field, int_var, isdefault )	\
	pji_int_field_set_impl( &pji->local.i.by_name.field, int_var, isdefault )

#define pji_str_field_get_by_name( pji, str_var, field, isdefault ) \
	pji_str_field_get_impl( &pji->local.s.by_name.field, str_var, isdefault )
#define pji_str_field_set_by_name( pji, field, str_var, isdefault )	\
	pji_str_field_set_impl( &pji->local.s.by_name.field, str_var, isdefault )

/// By index functions are useful internally to initialize all the variables
///
#define pji_int_field_get_by_index( pji, index, isdefault ) \
	pji_int_field_get_impl( &pji->local.i.by_index[ index ], isdefault )
#define pji_int_field_set_by_index( pji, var, index, isdefault ) \
	pji_int_field_set_impl( &pji->local.i.by_index[ index ], var, isdefault )

#define pji_str_field_get_by_index( pji, index, isdefault ) \
	pji_str_field_get_impl( &pji->local.s.by_index[ index ], isdefault )
#define pji_str_field_set_by_index( pji, var, index, isdefault ) \
	pji_str_field_set_impl( &pji->local.s.by_index[ index ], var, isdefault )

/// copy constructor use to create job scoped data
///
int pji_local_copy_init( pji_local_t *dest, pji_local_t *src );

/// var/oid initialization
void var_module_pjl_init(void);


/// debug: get var_name at index.
const char * pji_get_registered_name( int index );
#ifdef __cplusplus
}
#endif
#endif // PJL_VARIABLES_COMMON_H
