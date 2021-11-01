/***********************************************************
* (c) Copyright 1998-2008 Marvell International, Ltd. 
*
*                 Marvell Confidential
* ==========================================================
*/

/****************************************************************************\
**                                                                          **
**  zPJL_test_env.c                                                         **
**                                                                          **
**  These tables specify the evironment for a given PJL implementation.     **
**  These tables depend on keywords being defined by zPJL_SYM.h using       **
**  the SYMGEN utility and a .SMG file.                                     **
**                                                                          **
**  To add any new keyword (command or environment or other), add it to     **
**  the .SMG file for this project and re-run the SYMGEN utility.           **
**                                                                          **
\****************************************************************************/

#include <stdbool.h>

#include "zPJL.h"
#include "zPJL_sym.h"
#include "zPJL_test.h"


/* environment */
/*             *----------------------------------------------------------- */

/* this can take any name you want (const makes it ROM-able) */
const ZPJL_EnvItem  __ZPJL_ROM_ENV__[] =
    {
    ENV_ITEM_ENUM(     IDS_PARALLEL, IDS_FAST ),

    ENV_ITEM_TERMINATOR
    };

const int  __ZPJL_ROM_ENVSIZE__ =
    sizeof(__ZPJL_ROM_ENV__) / sizeof(ZPJL_EnvItem);
//
// Define the variables and ranges allowed in each environment.
// 
const ZPJL_EnvItem __ZPJL_USTATUS_ENV__[]=
{
    ENV_ITEM_RANGEVAL(IDS_TIMED, 0, 5, 300),
    ENV_ITEM_ENUM(IDS_DEVICE, IDS_ON),
    ENV_ITEM_ENUM(IDS_DEVICE, IDS_OFF),
    ENV_ITEM_ENUM(IDS_DEVICE, IDS_VERBOSE),
    ENV_ITEM_TERMINATOR

};

const int __ZPJL_USTATUS_ENVSIZE__ =
   sizeof(__ZPJL_USTATUS_ENV__)/sizeof(ZPJL_EnvItem);
/* end listing */
