/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanif_mcfg.c
 *
 * \brief Scan Block Motor Config register interface.
 *
 **/

#include <stdint.h>
#include <stdbool.h>

#include "lassert.h"
#include "regAddrs.h"

#include "scantypes.h"
#include "scancore.h"
#include "scanhw.h"
#include "scanif.h"

void scif_motor_setup(MOTOR_SEL motor_data_enable, MOTOR_SYNC lsmode, 
                      MOTOR_TYPE m1type, MOTOR_TYPE m2type)
{
    *SCIF_MCFG =
        SCIF_MCFG_LSMODE(lsmode) |
        SCIF_MCFG_MTYPE(m1type);
}

void scif_motor_lsmode(MOTOR_SYNC lsmode) 
{
    *SCIF_MCFG = (*SCIF_MCFG & ~SCIF_MCFG_LSMODE(-1)) |
        SCIF_MCFG_LSMODE(lsmode);
}

void scif_motor_data_enable(MOTOR_SEL motor_data_enable) 
{
//    *SCIF_MCFG = (*SCIF_MCFG & ~SCIF_MCFG_DATAEN(-1)) |
//                SCIF_MCFG_DATAEN(motor_data_enable);
}

