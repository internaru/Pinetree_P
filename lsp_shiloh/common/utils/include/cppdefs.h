/******************************************************************************
 * ============================================================================
 * Copyright (c) 2008 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file cppdefs.h
 *
 * \brief This file increments macros and defines needed to allow the system
 * to compile correctly under both a C and C++ compiler.
 *
 */

#ifndef _CPPDEFS_H
#define _CPPDEFS_H

// In some limited cases the OS needs to call into the main system.  Even in
// C++ builds the OS is still built with the C compiler.  Thus some functions
// need to be declared extern C.  This macro allows that to be done on a function
// by function basis.
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#endif //_CPPDEFS_H




