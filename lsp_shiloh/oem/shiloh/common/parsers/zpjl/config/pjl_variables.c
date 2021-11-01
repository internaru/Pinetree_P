/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdbool.h>
#include <stddef.h>
#include "pjl_api.h"
#include "pjl_variables_common.h"
#include "zPJL_sym.h"  /// new enum values used in WORD_MATCH must be added here.

// header file for these defined in pjl_grammar.h ?
extern WORD_MATCH *pwm_OffOn;       // define for the onoff enum.
//extern WORD_MATCH *pwm_OnOffAuto;       // define for the onoffauto enum.


WORD_MATCH  wm_Binding[] =
{
	{ IDS_LONGEDGE,            {A_VALUE,0}, S_EOL },
	{ IDS_SHORTEDGE,           {A_VALUE,0}, S_EOL },
	END_WORD_MATCH
};
WORD_MATCH *pwm_Binding=wm_Binding;


extern WORD_MATCH *pwm_Paper;  // dynamic construction based on paper table of the device.
extern WORD_MATCH *pwm_MediaType;
extern WORD_MATCH *pwm_Trays;

/// helpers  
/// name should match the pjl variable name exactly:  DUPLEX 
/// oid_id for the default and factory settings.
/// I for integer or enum variables
/// S for string variables char str[40] is pjl standard length.
///
#define PJL_I_VARIABLE_NAME( name, oid_id )\
        .var_name       = #name ,\
	.object_offset  = ((offsetof( pji_local_t, i )) + (offsetof( pji_ints_t, name))), \
	.oid            = oid_id

#define PJL_S_VARIABLE_NAME( name, oid_id )				\
	        .var_name       = #name ,				\
		.object_offset  = ((offsetof( pji_local_t, s )) + (offsetof( pji_strs_t, name))), \
		.oid            = oid_id,				\
		.VarUse         = e_VarString,		\
		.NextMatch      = 0

/// Match is a WORD_MATCH with an extern yuck.
/// start value 0 or 1 for instance and the increment value for +1 or +10 steps.
///
#define PJL_ENUM_INITIALIZER( Match, start, increment ) \
	        .VarUse         = e_VarEnum,		\
		.var_max_value  = 0,			\
		.var_min_value  = 0,			\
		.NextMatch      = Match,		\
		.enum_start     = start,		\
		.enum_increment = increment

/// min and max range constraints on an integer variable.
#define PJL_INT_INITIALIZER( min, max ) \
	.VarUse         = e_VarNumber,\
	.var_max_value  = max, \
	.var_min_value  = min, \
	.NextMatch      = 0, \
	.enum_start     = 0, \
	.enum_increment = 1

//	pjl_common_variable_table



pjl_register_variable_t pjl_local_variable_table[] =
{
	/// BEGIN PJL COMMON 
	{ //0
		PJL_I_VARIABLE_NAME( JAMRECOVERY, OID_PRINT_JAM_RECOVERY ),
		PJL_ENUM_INITIALIZER( &pwm_OffOn, 0, 1),
		.var_type       = e_GetSet, 
		.scope          = e_global_scope,
	},
	{ //1
		PJL_I_VARIABLE_NAME( TIMEOUT, OID_PLATFORM_IO_TIMEOUT ),
		PJL_INT_INITIALIZER( MIN_IO_TIMEOUT, MAX_IO_TIMEOUT ),
		.var_type        = e_GetSet, 
		.scope           = e_global_scope,
	},
	{ //2	/// total pages printed ? who asked for this.
		PJL_I_VARIABLE_NAME( PAGECOUNT, OID_PRINT_HEAD_TOTAL_PAGES_PRINTED ),
		PJL_INT_INITIALIZER( 0 , -1 ),
		.var_type       = e_Get, 
		.scope          = e_global_scope,
	},
	{ //3	///
		PJL_I_VARIABLE_NAME( COPIES, OID_PJL_DEFAULT_COPIES ),
		PJL_INT_INITIALIZER( 1 , 999 ),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //4
		PJL_I_VARIABLE_NAME( QTY, OID_PJL_DEFAULT_COLLATED_COPIES ),
		PJL_INT_INITIALIZER( 0 , 999 ),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //5
		PJL_I_VARIABLE_NAME( DUPLEX, OID_PJL_DEFAULT_DUPLEX ),
		PJL_ENUM_INITIALIZER( &pwm_OffOn, 0, 1),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //6
		PJL_I_VARIABLE_NAME( BINDING, OID_PJL_DEFAULT_BINDING ), // longedge / shortedge duplex binding
		PJL_ENUM_INITIALIZER( &pwm_Binding, 0, 1),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //7
		PJL_I_VARIABLE_NAME( PAPER, OID_PRINT_MEDIA_SIZE_DEFAULT ), //	       
		PJL_ENUM_INITIALIZER( &pwm_Paper, 0, 1),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //8
		PJL_I_VARIABLE_NAME( MEDIATYPE, OID_PRINT_MEDIA_TYPE_DEFAULT ), //	       
		PJL_ENUM_INITIALIZER( &pwm_MediaType, 0, 1),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},

	/// END of PJL COMMON
	/// BEGIN of VENDOR/OEM variables
	{ //9	///
		PJL_I_VARIABLE_NAME( RESOLUTION, OID_PJL_RESOLUTION ),
		PJL_INT_INITIALIZER( 600 , 1200 ),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //10	///
		PJL_I_VARIABLE_NAME( ECONOMODE, OID_PJL_ECONOMODE ),
		PJL_ENUM_INITIALIZER( &pwm_OffOn, 0, 1),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //11	///
		PJL_I_VARIABLE_NAME( TONERDARKNESS, OID_PJL_TONERDARKNESS ),
		PJL_INT_INITIALIZER( 0 , 10 ),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	/* USB Status Monitor	@@ 2013.12.17		by Juny */
	{ //12
		PJL_I_VARIABLE_NAME( INTRAY1SIZE, OID_PJL_INTRAY1SIZE ),
		PJL_INT_INITIALIZER( 0 , 9999 ),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //13
		PJL_I_VARIABLE_NAME( INTRAY2SIZE, OID_PJL_INTRAY2SIZE ),
		PJL_INT_INITIALIZER( 0 , 9999 ),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //14
		PJL_I_VARIABLE_NAME( INTRAY3SIZE, OID_PJL_INTRAY3SIZE ),
		PJL_INT_INITIALIZER( 0 , 9999 ),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //15
		PJL_I_VARIABLE_NAME( MPTRAYSIZE, OID_PJL_MPTRAYSIZE ),
		PJL_INT_INITIALIZER( 0 , 9999 ),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //16
		PJL_I_VARIABLE_NAME( DRUM, OID_PJL_DRUM ),
		PJL_INT_INITIALIZER( 0 , 100 ),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //17
		PJL_I_VARIABLE_NAME( TONER, OID_PJL_TONER ),
		PJL_INT_INITIALIZER( 0 , 9999 ),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //18
		PJL_I_VARIABLE_NAME( DUPLEXER, OID_PJL_DUPLEXER ),
		PJL_ENUM_INITIALIZER( &pwm_OffOn, 0, 1),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //19
		PJL_I_VARIABLE_NAME( INTRAY1, OID_PJL_INTRAY1 ),
		PJL_ENUM_INITIALIZER( &pwm_OffOn, 0, 1),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //20
		PJL_I_VARIABLE_NAME( INTRAY2, OID_PJL_INTRAY2 ),
		PJL_ENUM_INITIALIZER( &pwm_OffOn, 0, 1),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //21
		PJL_I_VARIABLE_NAME( INTRAY3, OID_PJL_INTRAY3 ),
		PJL_ENUM_INITIALIZER( &pwm_OffOn, 0, 1),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //22
		PJL_I_VARIABLE_NAME( MPTRAY, OID_PJL_MPTRAY ),
		PJL_ENUM_INITIALIZER( &pwm_OffOn, 0, 1),
		.var_type       = e_Get, 
		.scope          = e_job_scope,
	},
	{ //23
		PJL_S_VARIABLE_NAME( JOBNAME, OID_PJL_JOBNAME ),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //24
		PJL_S_VARIABLE_NAME( AUTHENTICATIONPASSWORD, OID_PJL_AUTHENTICATIONPASSWORD ),
//		using GG suggestion form	 	2014.02.18	by Juny
//		.var_type       = e_Set, 
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //25
		PJL_S_VARIABLE_NAME( AUTHENTICATIONUSERID, OID_PJL_AUTHENTICATIONUSERID ),
//		using GG suggestion form	 	2014.02.18	by Juny
//		.var_type       = e_Set, 
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //26
		PJL_I_VARIABLE_NAME( MEDIASOURCE, OID_PRINT_MEDIA_SOURCE_DEFAULT ), //	       
		PJL_ENUM_INITIALIZER( &pwm_Trays, 0, 1),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
	{ //27
		PJL_S_VARIABLE_NAME( SINDOHUSERNAME, OID_PJL_SINDOHUSERNAME ),
		.var_type       = e_GetSet, 
		.scope          = e_job_scope,
	},
};



#define num_var sizeof(pjl_local_variable_table)/sizeof(pjl_register_variable_t) ///< The number of enum variables
const int num_entries_pjl_local_variable_table = num_var;


	

