
/***********************************************************
* (C) Copyright 2011 Marvell International Ltd.
*               All Rights Reserved
*
*               Marvell Confidential
* ==========================================================
*/


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "e-fuse_api.h"
#include "asic.h"
#include "efuse_config.h"
#include "posix_ostools.h"
#include "cmd_proc_api.h"
#include "lassert.h"
#include "logger.h"

static volatile EFUSE_REGS_t *efuse_regs = NULL;
#define EFUSE_NUM_BANKS 4
#define EFUSE_MAX_REGS_PER_BANK 4           // 4 register sets per bank including the regster set that hold only the security bit (96)
#define EFUSE_MAX_WRITE_REGS_PER_BANK 3     // 3 writable register sets per bank; security bit is programmed separately

void efuse_print_bank(int num)
{
    uint32_t x[EFUSE_MAX_REGS_PER_BANK];
    int i;
    for (i = 0; i < EFUSE_MAX_REGS_PER_BANK; i++)
    {
        x[i] = efuse_get_val(num, i);
    }
    cmd_printf("efuse[%d] = %01X %08x %08x %08x\r\n", num, x[3], x[2], x[1], x[0]);
}

int efuse_program_bank(uint32_t bank_num, uint32_t bank_reg_val_list[], uint32_t num_bank_regs)
{
    uint32_t reg_val;
    int i;
    uint32_t efuse_vals[EFUSE_MAX_WRITE_REGS_PER_BANK];

    if (bank_num >= EFUSE_NUM_BANKS)
    {
        DPRINTF(DBG_ERROR, ("%s: error: bank num %d exceeds max %d\n", __func__, bank_num, EFUSE_NUM_BANKS-1));
        return -1;
    }

    if (num_bank_regs > EFUSE_MAX_WRITE_REGS_PER_BANK)
    {
        DPRINTF(DBG_ERROR, ("%s: error: num_bank_regs %d exceeds max %d\n", __func__, num_bank_regs, EFUSE_MAX_WRITE_REGS_PER_BANK));
        return -1;
    }

    // read current fuse values
    for (i = 0; i < EFUSE_MAX_WRITE_REGS_PER_BANK; i++) 
        efuse_vals[i] = *(&efuse_regs->EfuseStatus31to0_Bank0 + 4 * bank_num + i);

    // mask in new values
    for (i = 0; i < num_bank_regs; i++)
    {
        efuse_vals[i] |= bank_reg_val_list[i];
    }

    // wait for efuse not busy
    while (EFUSE_TOP_STATUS_BUSY_MASK_SHIFT(efuse_regs->TOP_Status))
    {
        posix_sleep_ms(1);
    }

    // write TOP_Control
    reg_val = 0;
    reg_val = EFUSE_TOP_CONTROL_BANK_CS_REPLACE_VAL(reg_val, bank_num);
    efuse_regs->TOP_Control = reg_val;

    // write data values
    efuse_regs->ProgData0 = efuse_vals[0];
    efuse_regs->ProgData1 = efuse_vals[1];
    efuse_regs->ProgData2 = efuse_vals[2];

    // write AUTO_Control
    efuse_regs->AUTO_Control = 0;
    DPRINTF(DBG_ERROR, ("%s: Programming bank %d\n", __func__, bank_num));
    // wait until efuse not busy
    while (EFUSE_TOP_STATUS_BUSY_MASK_SHIFT(efuse_regs->TOP_Status))
    {
        posix_sleep_ms(1);
    }

    DPRINTF(DBG_ERROR, ("%s: Programming done\n", __func__));
    // check results
    for (i = 0; i < EFUSE_MAX_WRITE_REGS_PER_BANK; i++)
    {
        uint32_t z = *(&efuse_regs->EfuseStatus31to0_Bank0 + 4 * bank_num + i);
        if (z != efuse_vals[i])
        {
            DPRINTF(DBG_ERROR, ("%s: error: Unexpected result at bank %0d register %0d\r\n", __func__, bank_num, i));
            DPRINTF(DBG_ERROR, ("\texpected: %08x, actual: %08x\r\n", efuse_vals[i], z));
            return -1;
        }
    }
    return 0;
}

int efuse_program_bit(uint32_t bank_num, uint32_t bit_num)
{
    uint32_t reg_vals[EFUSE_MAX_WRITE_REGS_PER_BANK];

    if (bank_num >= EFUSE_NUM_BANKS)
    {
        DPRINTF(DBG_ERROR, ("%s: error: bank num %d exceeds max %d\n", __func__, bank_num, EFUSE_NUM_BANKS-1));
        return -1;
    }

    if (bit_num >= (EFUSE_MAX_WRITE_REGS_PER_BANK*32))
    {
        DPRINTF(DBG_ERROR, ("%s: error: bit num %d exceeds max %d\n", __func__, bit_num, (EFUSE_MAX_WRITE_REGS_PER_BANK*32)-1));
        return -1;       
    }

    memset(&reg_vals, 0, sizeof(uint32_t)*EFUSE_MAX_WRITE_REGS_PER_BANK);
    if (bit_num < 32)
    {
        reg_vals[0] = 1 << bit_num;
    }
    else if (bit_num < 64)
    {
        reg_vals[1] = 1 << (bit_num-32);
    }
    else if (bit_num < 96)
    {
        reg_vals[2] = 1 << (bit_num-64);
    }
    else
    {
        return -1;
    }

    return efuse_program_bank(bank_num, reg_vals, EFUSE_MAX_WRITE_REGS_PER_BANK);
}

int efuse_program_bank_security_fuse_bit(uint32_t bank_num)
{
    uint32_t reg_val;

    if (bank_num >= EFUSE_NUM_BANKS)
    {
        DPRINTF(DBG_ERROR, ("%s: error: bank num %d exceeds max %d\n", __func__, bank_num, EFUSE_NUM_BANKS-1));
        return -1;
    }

    // wait for efuse not busy
    while (EFUSE_TOP_STATUS_BUSY_MASK_SHIFT(efuse_regs->TOP_Status))
    {
        posix_sleep_ms(1);
    }

    // write TOP_Control
    reg_val = 0;
    reg_val = EFUSE_TOP_CONTROL_BANK_CS_REPLACE_VAL(reg_val, bank_num);
    efuse_regs->TOP_Control = reg_val;

    // write data values
    efuse_regs->ProgData0 = 0;
    efuse_regs->ProgData1 = 0;
    efuse_regs->ProgData2 = 0;

    // write AUTO_Control
    reg_val = 0;
    reg_val = EFUSE_AUTO_CONTROL_SFB_REPLACE_VAL(reg_val, 1);
    efuse_regs->AUTO_Control = reg_val;
    DPRINTF(DBG_ERROR, ("%s: Programming bank %d\n", __func__, bank_num));
    // wait until efuse not busy
    while (EFUSE_TOP_STATUS_BUSY_MASK_SHIFT(efuse_regs->TOP_Status))
    {
        posix_sleep_ms(1);
    }

    DPRINTF(DBG_ERROR, ("%s: Programming done\n", __func__));

    // check results
    uint32_t z = *(&efuse_regs->EfuseStatus31to0_Bank0 + 4 * bank_num + 3);
    if (z != 1)
    {
        DPRINTF(DBG_ERROR, ("%s: error: Unexpected result at bank %0d register %0d\r\n", __func__, bank_num, 3));
        DPRINTF(DBG_ERROR, ("\texpected: %08x, actual: %08x\r\n", 1, z));
        return -1;
    }
    return 0;
}

/**
 * \brief Read and return an efuse setting.
 * The efuses are separated into banks with 4 32 bit registers per bank.  This
 * returns the register within a given bank.
 * \param[in] bank_num Values range from 0-3 the bank number to access
 * \param[in] reg Values range from 0-3 The register within the bank to return
 * \returns uint32_t
 * \retval The value of the register.
 */
uint32_t efuse_get_val(uint32_t bank_num, uint32_t reg)
{
    uint32_t efuse_reg_val;

    efuse_reg_val = *(&efuse_regs->EfuseStatus31to0_Bank0 + 4 * bank_num + reg);
    return efuse_reg_val;
}

bool efuse_get_bit(uint32_t bank_num, uint32_t bit)
{
    uint32_t value;
    uint32_t bit_mask;

    value = efuse_get_val(bank_num, bit/32);
    bit_mask = 1<<(bit % 32);
    if(value & bit_mask)
    {
        return true;
    } else
    {
        return false;
    }
}

#ifdef HAVE_CMD
static int efuse_cmd_dump_cb( int argc, char *argv[] )
{
  int i;
  for (i = 0; i < 4; i++)
    efuse_print_bank(i);
  return CMD_OK;
}

static int efuse_cmd_burn_cb( int argc, char *argv[] )
{
    uint32_t arg_offset = 1;
    uint32_t bank_num;
    uint32_t reg_vals[EFUSE_MAX_WRITE_REGS_PER_BANK];
    uint32_t num_reg_vals;

    if ( argc < 3 || argc > (2 + EFUSE_MAX_WRITE_REGS_PER_BANK))
    {
        cmd_printf("Wrong number of parameters\n");
        return CMD_USAGE_ERROR;
    }

    bank_num = atoi(argv[arg_offset++]);
    for (num_reg_vals = 0;arg_offset < argc; arg_offset++, num_reg_vals++)
    {
        reg_vals[num_reg_vals] = strtoul(argv[arg_offset], NULL, 16);
    }

    if (efuse_program_bank(bank_num, reg_vals, num_reg_vals != 0))
    {
        return CMD_ERROR;
    }

    return CMD_OK;
}

static int efuse_cmd_burn_bit_cb( int argc, char *argv[] )
{
    uint32_t bank_num;
    uint32_t bit_num;

    if ( argc != 3 )
    {
        cmd_printf("Wrong number of parameters\n");
        return CMD_USAGE_ERROR;
    }

    bank_num = atoi(argv[1]);
    bit_num = atoi(argv[2]);

    if(efuse_program_bit(bank_num, bit_num) != 0)
    {
        return CMD_ERROR;
    }
    return CMD_OK;
}

static int efuse_cmd_burn_security_cb( int argc, char *argv[] )
{
    uint32_t bank_num;

    if ( argc != 2 )
    {
        cmd_printf("Wrong number of parameters\n");
        return CMD_USAGE_ERROR;
    }

    bank_num = atoi(argv[1]);

    if(efuse_program_bank_security_fuse_bit(bank_num) != 0)
    {
        return CMD_ERROR;
    }
    return CMD_OK;
}

static int efuse_cmd_burn_oem_data_cb( int argc, char *argv[] )
{
    uint32_t vendor_id, operator_id, platform_id, chip_id;

    if ( argc != 5 )
    {
        cmd_printf("Wrong number of parameters\n");
        return CMD_USAGE_ERROR;
    }

    vendor_id = strtoul(argv[1], NULL, 16);
    operator_id = strtoul(argv[2], NULL, 16);
    platform_id = strtoul(argv[3], NULL, 16);
    chip_id = strtoul(argv[4], NULL, 16);

    cmd_printf("Burning oem data vendor_id = %#x, operator_id = %#x, platform_id = %#x, chip_id = %#x\n", 
               vendor_id, operator_id, platform_id, chip_id);

    if (efuse_program_88PA6170_oem_data(vendor_id, operator_id, platform_id, chip_id) != 0)
    {
        cmd_printf("OEM data burn FAILED.\n");
        return CMD_ERROR;
    }
    cmd_printf("OEM data burn SUCCESS.\n");
    return CMD_OK;
}
#endif

void efuse_init()
{
    efuse_regs = efuse_config_get_regs_base();

    #ifdef HAVE_CMD
    int cmd_res;
    /* top-level command */
    cmd_res = cmd_register( "efuse", "WARNING: BURNING EFUSES IS DANGEROUS OPERATION THAT CANNOT BE UNDONE.  YOU MAY DESTROY YOUR ASIC.", NULL, NULL, NULL );
    XASSERT( ( CMD_OK == cmd_res ), cmd_res );

    cmd_res = cmd_register( "efuse dump",
                            "Display the efuse bits",
                            NULL,
                            NULL,
                            efuse_cmd_dump_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "efuse burn",
                            "Burn the efuse bits for the specified bank. Register values are in hex.",
                            "<bank_num> <reg0_value> <reg1_value> ... <regN_value>",
                            "WARNING: BURNING EFUSES IS DANGEROUS OPERATION THAT CANNOT BE UNDONE.  YOU MAY DESTROY YOUR ASIC.  Register values are 32bit values.  reg0 == bits 0-31, reg1 = bits 32-63, and so on.",
                            efuse_cmd_burn_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "efuse burn_bit",
                            "Burn the specified efuse bit for the specified bank.",
                            "<bank_num> <bit_num>",
                            "WARNING: BURNING SECURITY EFUSES IS DANGEROUS OPERATION THAT CANNOT BE UNDONE.  YOU MAY DESTROY YOUR ASIC. Valid bit numbers are 0-95",
                            efuse_cmd_burn_bit_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "efuse burn_security",
                            "Burn the security efuse bit for the specified bank.",
                            "<bank_num>",
                            "WARNING: BURNING SECURITY EFUSES IS DANGEROUS OPERATION THAT CANNOT BE UNDONE.  YOU MAY DESTROY YOUR ASIC.  THIS OPERATION WILL RENDER THE BANK READ-ONLY.",
                            efuse_cmd_burn_security_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );

    cmd_res = cmd_register( "efuse burn_oem_data",
                            "Burn the oem information for the asic. All values are in hex.",
                            "<vendor_id>, <operator_id>, <platform_id>, <chip_id>",
                            "WARNING: BURNING SECURITY EFUSES IS DANGEROUS OPERATION THAT CANNOT BE UNDONE.  YOU MAY DESTROY YOUR ASIC.",
                            efuse_cmd_burn_oem_data_cb );
    XASSERT( ( CMD_OK == cmd_res), cmd_res );
    #endif //HAVE_CMD
}
