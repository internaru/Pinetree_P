/** @file  includes.h 
  * @brief This file contains definition for include files
  * 
  * Copyright (C) 2003-2008, Marvell International Ltd.
  * All Rights Reserved  
  */

#ifndef INCLUDES_H
#define INCLUDES_H

//#include <stdio.h>
#include <stdlib.h>
//#include <memory.h>
#include <string.h>
#include <ctype.h>
//#include <sys/types.h>
#include <sys/time.h>
#include "dprintf.h"

#include "common.h"
#include "type_def.h"

//#if DBG_PRINTF
#if 0
/** ENTER */
#define ENTER()		dprintf("Enter: %s, %s:%i\n", __FUNCTION__, __FILE__, __LINE__)
/** LEAVE */
#define LEAVE()		dprintf("Leave: %s, %s:%i\n", __FUNCTION__, __FILE__, __LINE__)
#else
/** ENTER */
#define ENTER()
/** LEAVE */
#define LEAVE()
#endif

#endif /* INCLUDES_H */
