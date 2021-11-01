/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#include <stdbool.h>


#include "pjl_variables_common.h"
#include "pjl_api.h"
#include "oid_api.h"
#include "memAPI.h"
#include "logger.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#define DBG_PRFX "pjl_var: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(9)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

char gJobName[81]; /* Job Name*/

// OEM must define this in pjl_variable.c  
pjl_register_variable_t __attribute__((weak)) pjl_local_variable_table[] = {};

uint32_t PrintJobInfo_int_field_get_impl( pji_int_t *pvar )
{
    DBG_PRINTF_DEBUG("%s %s %d\n", __FUNCTION__, 
		     pjl_local_variable_table[ pvar->pji_index ].var_name, 
		     pvar->as_int );

    return pvar->as_int;
}

const char * PrintJobInfo_str_field_get_impl( pji_str_t *pvar )
{
    DBG_PRINTF_DEBUG("%s %s %s\n", __FUNCTION__, 
		     pjl_local_variable_table[ pvar->pji_index ].var_name, 
		     pvar->as_str );
    return pvar->as_str;
}

uint32_t pji_int_field_get_impl( pji_int_t *pvar, bool isdefault )
{
	pjl_register_variable_t *var_type = &pjl_local_variable_table[ pvar->pji_index ]; 
	uint32_t value;



	if ( pvar->is_set || !isdefault ) {
		value = pvar->as_int;
	} else {
		oid_get_uint32( var_type->oid, 0, &value );
	}
	DBG_PRINTF_DEBUG("%s %s %d isset %d\n", __FUNCTION__, var_type->var_name, pvar->as_int, pvar->is_set );
	return value;
}

uint32_t pji_int_field_set_impl( pji_int_t *pvar,     ///< object
				 uint32_t  int_value, ///< new value to set
				 bool      isdefault  ///< default or set (system vs Job) scope
	)
{
	pjl_register_variable_t *var_type = &pjl_local_variable_table[ pvar->pji_index ]; 
	uint32_t value = int_value;

	if (var_type->var_type == e_Get) {
		return -1; // set not allowed
	}
	if (!var_type->NextMatch) { // must be INT
		value = value < var_type->var_min_value ? var_type->var_min_value : 
			value > var_type->var_max_value ? var_type->var_max_value : value;
		if ( value != int_value ) {
			DBG_PRINTF_NOTICE("pjl value out of range %s %d\n", var_type->var_name, value);
		}
	} else {
		/// enum validation here?
		if ( value > (sizeof(var_type->NextMatch)/sizeof(WORD_MATCH) - 1 )) {
			DBG_PRINTF_NOTICE("pjl value out of range %s %d\n", var_type->var_name, value);
			value = 0;
		}
	}
	if (isdefault) {
		DBG_PRINTF_INFO("default %s %d\n", var_type->var_name, value );
		oid_set_uint32(var_type->oid, 0, value);
	} else {
		DBG_PRINTF_INFO("inhwan5: 1. set %s %d\n", var_type->var_name, value );
		pvar->as_int = value;
		pvar->is_set = true;
	}
	return 0;
}

const char *pji_str_field_get_impl( pji_str_t *pvar, bool isdefault )
{
	pjl_register_variable_t *var_type = &pjl_local_variable_table[ pvar->pji_index ]; 
	const char *value;

	if ( pvar->is_set || !isdefault ) {
		value = pvar->as_str;
	} else {
		oid_get_string( var_type->oid, 0, pvar->as_str, sizeof(pvar->as_str));
		value = pvar->as_str;
	}
	DBG_PRINTF_DEBUG("%s %s %s\n", __FUNCTION__, var_type->var_name, pvar->as_str );
	return value;
}

static int get_valid_utf8_length(const char *name, int max_size)
{
	char *str = (char *)name;
	int incr, total = 0;
	int remainder = max_size;

	/* refer from https://en.wikipedia.org/wiki/UTF-8 */
	while (total <= max_size) {
		if (*str < 0x80) {
			/* 0xxxxxxx */
			incr = 1;
		} else if ((str[0] & 0xe0) == 0xc0) {
			/* 110XXXXx 10xxxxxx */
			incr = 2;
			if (remainder < 2)
				return total;
		} else if ((str[0] & 0xf0) == 0xe0) {
			/* 1110XXXX 10Xxxxxx 10xxxxxx */
			incr = 3;
			if (remainder < 3)
				return total;
		} else if ((str[0] & 0xf8) == 0xf0) {
			/* 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx */
			incr = 4;
			if (remainder < 4)
				return total;
		} else /* unkown encoding */
			return -1;

		str += incr;
		total += incr;
		remainder -= incr;
	}

	return total;
}

#define min(a,b) ((a) < (b) ? (a) : (b))

uint32_t pji_str_field_set_impl( pji_str_t *pvar, const char *value, bool isdefault )
{
	pjl_register_variable_t *var_type = &pjl_local_variable_table[ pvar->pji_index ]; 
	
	if ( !value || !*value ) 
	    strncpy( pvar->as_str, "jobname", sizeof(pvar->as_str) - 1 );
	else
	    strncpy( pvar->as_str, value, sizeof(pvar->as_str) - 1 );
	if ( isdefault ) {
		oid_set_string( var_type->oid, 0, pvar->as_str, sizeof(pvar->as_str) );
		pvar->is_set = true;
		DBG_PRINTF_INFO("default %s [%s]\n", var_type->var_name, value );
	} else {
		DBG_PRINTF_INFO("set %s [%s]\n", var_type->var_name, value );
		memset(gJobName,0,sizeof(gJobName));
		if(value) /*Coverity 22073 :: NULL check*/
		{
			int len = get_valid_utf8_length(value,
					min(strlen(value), sizeof(gJobName) -4));	// _99 + NULL = 4
			if (len < 0)
				strncpy(gJobName, value, sizeof(gJobName) -4);
			else
				strncpy(gJobName, value, len);
		}
		else
		{
			strncpy(gJobName,"jobname",sizeof(gJobName) -4);
		}
		strncpy(gJobName,value,sizeof(gJobName) -1);
	}
	DBG_PRINTF_DEBUG("%s %s %s\n", __FUNCTION__, var_type->var_name, pvar->as_str );
	return 0;
}

const char * pji_get_registered_name( int index )
{
	return (const char *)pjl_local_variable_table[ index ].var_name;
}

/// pjl_local_variable_table[] PJL_VARS[] and the system/pipe/job level pji_local_t[] structures are initialized here.
///
/// pji_local_copy_init( &system_pji->local, 0 ); /// one time initialization of the system level PrintJobInfo
/// pji_local_copy_init( &pipe->pjob_info->local, &system_pji->local ); /// initialize each pipe from system settings
/// pji_local_copy_init( &pipe->pjob_info->local, &pipe->pjob_info->local ); /// reset all pipe settings to defaults
/// pji_local_copy_init( &job->pjob_info->local, &pipe->pjob_info->local ); /// create a new job copy from the pipe settings.
///
int pji_local_copy_init( pji_local_t *dest, pji_local_t *src )
{
    pjl_register_variable_t *var;  // class level information about the variable 
    int i, index;
    pji_str_t *si;                 // object level string variable
    pji_int_t *vi;                 // object level integer variable
	

    int var_elements = num_entries_pjl_local_variable_table;

    if ( src && src == dest ) { // clear all sets to defaults, pjl reset
	for ( i = 0; i < i_elements; i++ ) {
	    dest->i.by_index[i].is_set = false;
	}
	for ( i = 0; i < s_elements; i++ ) {
	    dest->s.by_index[i].is_set = false;
	}	
    } else {
	ASSERT( dest );
	if ( src ) { // copy from pipe to job or system_pji to pipe.
	    memcpy( dest, src, sizeof(pji_local_t) );
	    DBG_PRINTF_DEBUG("%s copy pji local table\n", __FUNCTION__ );
	} else { // one time registration for system_pji
	    for ( i = 0; i < var_elements; i++ ) {
		var = &pjl_local_variable_table[i];  // class table
		if (var->VarUse == e_VarString) { 
		    // allow class and object by names to be in different order
		    si = (pji_str_t*)((void*)dest + var->object_offset);
		    si->pji_index = i;  // object index into class array.
		    index = var->object_offset / sizeof(pji_str_t);
		    pjl_register_variable( var, index ); // index into object by type table
		    DBG_PRINTF_DEBUG_M("register int %s class[%d] object[%d]\n", var->var_name, i, index );
		} else {
		    vi = (pji_int_t*)((void*)dest + var->object_offset);
		    vi->pji_index = i; // class index
		    index = var->object_offset / sizeof(pji_int_t); // object index
		    pjl_register_variable( var, index );
		    DBG_PRINTF_DEBUG_M("register string %s class[%d] object[%d]\n", var->var_name, i, index );
		}
		
	    }
	}
    } 
    /// copy defaults up everytime.
    for ( i = 0; i < i_elements; i++ ) {  // object integer by index points back to class 
	if ( !dest->i.by_index[i].is_set ) {
	    index = dest->i.by_index[i].pji_index;
	    var = &pjl_local_variable_table[index];
	    DBG_PRINTF_DEBUG_H("%s %s %d\n", __FUNCTION__, var->var_name, dest->i.by_index[i].as_int );
	    oid_get_uint32( var->oid, 0, &dest->i.by_index[i].as_int );		
	    DBG_PRINTF_INFO("%s get %s %d (%d)\n", __FUNCTION__, var->var_name, dest->i.by_index[i].as_int, i );
	} else {	
	    DBG_PRINTF_INFO("%s %s %d (%d)\n", __FUNCTION__, var->var_name, dest->i.by_index[i].as_int, i );
	}
    }
    for ( i = 0; i < s_elements; i++ ) {
	if ( !dest->s.by_index[i].is_set ) {
	    index = dest->s.by_index[i].pji_index;
	    var = &pjl_local_variable_table[index];
	    oid_get_string( var->oid, 0, dest->s.by_index[i].as_str, 40 );		
	}	
    }
    
    return 0;
}

#ifdef HAVE_CMD

void PrintJobInfo_cmd_dump( PrintJobInfo_t *pji, int (pf)(const char* format, ...) )
{
	int i, index;
	pjl_register_variable_t *var;  // class level information about the variable 
	uint32_t value;
	
	if (!pji) return;

	for ( i = 0; i < i_elements; i++ ) {  // object integer by index points back to class 
		index = pji->local.i.by_index[i].pji_index;
		var = &pjl_local_variable_table[index];
		value = pji->local.i.by_index[i].as_int;	
		pf("pjl %s oid 0x%08x [0x%08x %d] %s\n", 
		   pji->local.i.by_index[i].is_set ? "set" : "default",
		   var->oid,
		   value, value, 
		   var->var_name
			);	
	}
	for ( i = 0; i < s_elements; i++ ) {
		index = pji->local.s.by_index[i].pji_index;
		var = &pjl_local_variable_table[index];
		pf("pjl %s oid 0x%08x [%s] %s\n", 
		   pji->local.s.by_index[i].is_set ? "set" : "default",
		   var->oid,
		   pji->local.s.by_index[i].as_str,
		   var->var_name
			);
	}	
        pf("pji pages_printed %d \n", pji->pages_printed);
	pf("pji pages_skipped_and_printed %d \n", pji->pages_skipped_and_printed);
	pf("pji jobstatus %d \n", pji->jobstatus);
	pf("pji num_bytes %d \n", pji->num_bytes);
	pf("pji jobname %s \n", pji->jobname);
	pf("pji pipe %p pipeParser %p\n", pji->pipe, pji->pipe->Parser);
}

void PrintJobInfo_cmd_dump_update( Observer_t *o, Subject_t *pji_subject )
{
	PrintJobInfo_t *pji = (PrintJobInfo_t*)pji_subject;
	PrintJobInfo_cmd_dump( pji, dbg_printf );
}
void PrintJobInfo_cmd_page_dump_update( Observer_t *o, Subject_t *pji_subject )
{
	dbg_printf("%s observer %o \n", __FUNCTION__, o);
	PrintJobInfo_cmd_dump_update( o, pji_subject ); 
}
void PrintJobInfo_cmd_job_dump_update( Observer_t *o, Subject_t *pji_subject )
{
	dbg_printf("%s observer %o \n", __FUNCTION__, o);
	PrintJobInfo_cmd_dump_update( o, pji_subject ); 
}
static int var_pjl_list_cb( int argc, char *argv[] )
{
	int i;
	pjl_register_variable_t *var;  // class level information about the variable 
	char str[40];
	uint32_t value;

	for (i=0; i < num_entries_pjl_local_variable_table; i++) {
		var = &pjl_local_variable_table[i];
		if ( var->VarUse == e_VarString ) {
			oid_get_string( var->oid, 0, str, 40 );		
			cmd_printf("pjl oid 0x%08x [%s] %s\n", 
				   var->oid,
				   str,
				   var->var_name
				);
		} else {
			oid_get_uint32( var->oid, 0, &value );		
			cmd_printf("pjl oid 0x%08x [0x%08x %d] %s\n", 
				   var->oid,
				   value, value, 
				   var->var_name
				);
		}		
	}
	return CMD_OK;
}
static int var_pjl_jobstart_cb( int argc, char *argv[] )
{
	static Observer_t *o = NULL;
	static bool attached = false;


	if (!o) {
		o = Observer_Constructor( 0x123411f2, PrintJobInfo_cmd_job_dump_update );
	}
	if (!attached) {
		ATTACH_OBSERVER(system_pji->jobstart_observers, o);
	} 
	cmd_printf("attached observer to jobstart\n");

	return 0;
	
}
static int var_pjl_jobend_cb( int argc, char *argv[] )
{
	static Observer_t *o = NULL;
	static bool attached = false;


	if (!o) {
		o = Observer_Constructor( 0x123411f1, PrintJobInfo_cmd_job_dump_update );
	}
	if (!attached) {
		ATTACH_OBSERVER(system_pji->jobend_observers, o);
	} 
	cmd_printf("attached observer to jobend\n");

	return 0;
	
}
static int var_pjl_pageend_cb( int argc, char *argv[] )
{
	static Observer_t *o = NULL;
	static bool attached = false;


	if (!o) {
		o = Observer_Constructor( 0x12341aa1, PrintJobInfo_cmd_page_dump_update );
	}
	if (!attached) {
		ATTACH_OBSERVER(system_pji->pageend_observers, o);
	} 
	cmd_printf("attached observer to pageend\n");

	return 0;
	
}
static int var_pjl_jpeg_rot_cb( int argc, char *argv[] )
{
#if 0
#ifdef HAVE_JPEG_SW
    extern int main_jpeg( int argc, char *argv[] );
    main_jpeg( argc, argv
 ); // "-rot 270" -outfilename /data/out.jpeg /data/in.jpeg" );
#endif
#endif
    return 0;

}

#ifndef UNUSED_VAR
/*!
 * \def UNUSED_VAR(x) 
 * a simple macro that will turn off the unused var compiler warning for a given var
 **/
#define UNUSED_VAR(x) ((x) = (x))  
#endif

void var_module_pjl_cmd_debug_init()
{


    int i_res = 0; UNUSED_VAR(i_res);
    i_res = cmd_register( "pjlvar",
                          NULL,
                          NULL,
                          NULL,
                          NULL );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "pjlvar",
				 "list",
				 "list variables and the default value @PJL DEFAULT VAR=VALUE",
				 NULL,
				 NULL,
				 var_pjl_list_cb );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "pjlvar",
				 "pageend",
				 "register for pageend dump",
				 NULL,
				 NULL,
				 var_pjl_pageend_cb );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "pjlvar",
				 "jobstart",
				 "register for jobstart dump",
				 NULL,
				 NULL,
				 var_pjl_jobstart_cb );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "pjlvar",
				 "jobend",
				 "register for jobend dump",
				 NULL,
				 NULL,
				 var_pjl_jobend_cb );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "pjlvar",
				 "jpegrot",
				 "rotate jpeg file",
				 NULL,
				 NULL,
				 var_pjl_jpeg_rot_cb );
    XASSERT( i_res == CMD_OK, i_res );

    
}
#else
void var_module_pjl_cmd_debug_init() {}
void PrintJobInfo_cmd_dump( Subject *pji ) {}
#endif

