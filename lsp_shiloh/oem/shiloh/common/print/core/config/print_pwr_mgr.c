/*
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>

#include "pwr_mgr_api.h"
#include "regAddrs.h"
#include "island_pwr.h"
#include "apb_config_regstructs.h"
#include "apb_config_regmasks.h"

#define PRINT_CLK_CTRL 3
#define PRINT_PWR_DWN_CTRL 2

static uint32_t pwr_mgr_print_event;

static bool print_low_pwr_mode(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd)
{
    APB_CONFIG_REGS_t *regs;

    regs = (APB_CONFIG_REGS_t *)APB_CONFIG_BASE;

    switch(level)
    {
        case pwr_mgr_on_e:
            island_pwr_up(PRINT_CLK_CTRL,PRINT_PWR_DWN_CTRL);
            pwr_mgr_clear_event_powered_down(pwr_mgr_print_event);

            break;
        case pwr_mgr_lowest_power_e:
            island_pwr_dwn(PRINT_CLK_CTRL,PRINT_PWR_DWN_CTRL); // numbers from the apb register doc.
            // fall through to default case.
        default:
            pwr_mgr_set_event_powered_down(pwr_mgr_print_event);
            break;
    }
    return true;
}

void print_register_pwr_event(void)
{
    pwr_mgr_print_event = pwr_mgr_reg_callback("PRINT", print_low_pwr_mode, pwr_mgr_pri_low_e);
    pwr_mgr_set_module_pwr_level("PRINT", pwr_mgr_lowest_power_e);
}
