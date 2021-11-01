/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                      Marvell Confidential
 * ============================================================================
 */

/**
 * \file print_events_common.c
 *
 * \brief .
 *
 *  
 *
 **/


//--------------------------------------
// Included Files
//--------------------------------------
#include <string.h>
#include <stdbool.h>
#include "ATypes.h"
#include "print_events_common_api.h"

extern void print_status_init(void); // from print_status_observers.c

//--------------------------------------
// Structures
//--------------------------------------


//--------------------------------------
// Local variables
//--------------------------------------



void init_print_events_common()
{
    print_status_init();
}



