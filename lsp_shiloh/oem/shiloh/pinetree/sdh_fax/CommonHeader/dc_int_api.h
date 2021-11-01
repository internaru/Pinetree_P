/*
 * $Id: dc_int_api.h,v 1.1 2009/06/25 23:45:02 rayskymy Exp $
 *
 * Copyright (C) 2009 Conexant Systems Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _dc_int_h_
# define _dc_int_h_
/**************************************************************************//**
*  @file dc_int.h
*
*  @brief This file contains the API for interrupt control. Please refer
*  to the Operating Environment reference manual for details.
******************************************************************************/
/******************************************************************************
*  FILE FORMAT: <PUBLIC>
*      1. INCLUDES 
*      2. ENUMERATIONS 
*      3. DEFINES
*      4. STRUCTURES
*      5. GLOBALS
*      6. PUBLIC FUNCTION PROTOTYPES
******************************************************************************/
/******************************************************************************
*  1. INCLUDES
******************************************************************************/
//# include "dc_int_soc_api.h"
/******************************************************************************
*  2. ENUMERATIONS
******************************************************************************/
typedef enum 
{
    DC_INT_GP_LEVEL_DETECT_LO,
    DC_INT_GP_LEVEL_DETECT_HI
} DC_INT_GP_DETECT_TYPE;
/******************************************************************************
*  3. DEFINES
******************************************************************************/
/* Number of interrupt status bits, exclusive upper bound on DC_INT_ID. */
# define DC_INT_ID_LIMIT 64

/* Number of GP interrupt bits, exclusive upper bound on DC_INT_GP_ID. */
# define DC_INT_GP_ID_LIMIT 32

/* The minimum priority allowed for interrupts */
# define DC_INT_PRIORITY_MIN 0

/* The maximum priority allowed for interrupts */
# define DC_INT_PRIORITY_MAX 255
/******************************************************************************
*  4. STRUCTURES
******************************************************************************/
typedef UINT32 DC_INT_ID;
typedef UINT32 DC_INT_GP_ID;
/******************************************************************************
*  5. GLOBALS
******************************************************************************/
/******************************************************************************
*  6. PUBLIC FUNCTION PROTOTYPES
******************************************************************************/
DC_RETURN_CODE dc_int_init(void);
DC_RETURN_CODE dc_int_register(DC_INT_ID id, 
                               void (*fun)(void), 
                               int priority);
DC_RETURN_CODE dc_int_unregister(DC_INT_ID id,
                               void (*fun)(void),
                               int priority);
DC_RETURN_CODE dc_int_flag_clear(DC_INT_ID id);
DC_RETURN_CODE dc_int_flag_set(DC_INT_ID id);
DC_RETURN_CODE dc_int_flag_get(boolean *flag, DC_INT_ID id);
DC_RETURN_CODE dc_int_firq_enable(DC_INT_ID id);
DC_RETURN_CODE dc_int_irq_enable(DC_INT_ID id);
DC_RETURN_CODE dc_int_disable(DC_INT_ID id);
DC_RETURN_CODE dc_int_gp_init(boolean firq,
                               int priority);
DC_RETURN_CODE dc_int_gp_register(void (*handler)(DC_INT_GP_ID id), 
                                  DC_INT_GP_ID id,
                                  int priority);
DC_RETURN_CODE dc_int_gp_unregister(DC_INT_GP_ID id, int priority);
DC_RETURN_CODE dc_int_gp_enable(DC_INT_GP_ID id,
                                DC_INT_GP_DETECT_TYPE detect);
DC_RETURN_CODE dc_int_gp_disable(DC_INT_GP_ID id);
DC_RETURN_CODE dc_int_gp_flag_set(DC_INT_GP_ID id);
DC_RETURN_CODE dc_int_gp_flag_clear(DC_INT_GP_ID id);
DC_RETURN_CODE dc_int_gp_flag_get(boolean *flag,
                                   DC_INT_GP_ID id);
void dc_int_list(void);
#endif /*  _dc_int_h_ */
