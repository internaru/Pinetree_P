/*
 * ======================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

#include <stddef.h>
#include <string.h>
#include "posix_ostools.h"
#include "logger.h"
#include "lassert.h"
#include "efuse_config.h"
#include "uio_lib_api.h"
#include "asic.h"
#include "e-fuse_api.h"


EFUSE_REGS_t* efuse_config_get_regs_base()
{
    static void *dev = NULL;
    static EFUSE_REGS_t* regs = NULL;

    if(dev == NULL)
    {
        dev = uio_open_dev("efuse");
        XASSERT(dev != NULL, errno);
    }

    if(regs == NULL)
    {
        // get device's map
        uio_map_t *map = uio_get_map(dev, 0);

        if (map != NULL)
        {
            regs = map->virt_addr;
        }
        DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s = %#x\n", __func__, regs));
    }

    return regs;
}

// debug interface to program a development part so that it looks like a production
// part with all security features enabled.  This is a one-way trip so don't do it
// unless you really want to.
int efuse_program_88PA6170_as_production_part()
{
    const uint32_t production_bank_num = 1;
    uint32_t reg_vals[3];

    memset(reg_vals, 0, sizeof(reg_vals));

    // program VCF Enable
    reg_vals[1] = 0x5;      // set bits 35:32 = b'0101

    // program production part
    reg_vals[1] |= (1<<(48-32));  // set bit 48

    // program
    return efuse_program_bank(production_bank_num, reg_vals, 3);
}

int efuse_program_88PA6170_oem_data(uint32_t vendor_id, uint32_t operator_id, uint32_t platform_id, uint32_t chip_id)
{
    #define OEM_DATA_BANK           2

    // BANK 2 values
    #define VENDOR_ID_MASK          0x0000001F
    #define VENDOR_ID_SHIFT         0
    #define VENDOR_ID_SIZE_BITS     5
    #define OPERATOR_ID_MASK        0x00000007
    #define OPERATOR_ID_SHIFT       5
    #define OPERATOR_ID_SIZE_BITS   3
    #define PLATFORM_ID_MASK        0x000003FF
    #define PLATFORM_ID_SHIFT       8
    #define PLATFORM_ID_SIZE_BITS   10
    #define CHIP_ID_MASK            0xFFFFFFFF
    #define CHIP_ID_SHIFT           18
    #define CHIP_ID_SIZE_BITS       32
    #define FUSE_ID_CHECK_MASK      0x0000003F
    #define FUSE_ID_CHECK_SHIFT     50
    #define FUSE_ID_SIZE_BITS       6

    // BANK 1 values
    #define FEATURE_ID_MASK         0x00000003
    #define FEATURE_ID_SHIFT        49
    #define FEATURE_ID_SIZE_BITS    2

    uint32_t reg_vals[3];
    uint32_t fuse_id_check = 0;
    uint32_t feature_id;
    uint32_t i;

    // validate ranges
    if ((vendor_id & VENDOR_ID_MASK) != vendor_id)
    {
        DPRINTF(DBG_ERROR, ("%s: invalid vendor id %#x\n", __func__, vendor_id));
        return -1;
    }
    if ((operator_id & OPERATOR_ID_MASK) != operator_id)
    {
        DPRINTF(DBG_ERROR, ("%s: invalid operator id %#x\n", __func__, operator_id));
        return -1;
    }
    if ((platform_id & PLATFORM_ID_MASK) != platform_id)
    {
        DPRINTF(DBG_ERROR, ("%s: invalid platform id %#x\n", __func__, platform_id));
        return -1;
    }
    if ((chip_id & CHIP_ID_MASK) != chip_id)
    {
        DPRINTF(DBG_ERROR, ("%s: invalid chip id %#x\n", __func__, chip_id));
        return -1;
    }

    memset(reg_vals, 0, sizeof(reg_vals));
    reg_vals[0] |= (vendor_id << VENDOR_ID_SHIFT);
    reg_vals[0] |= (operator_id << OPERATOR_ID_SHIFT);
    reg_vals[0] |= (platform_id << PLATFORM_ID_SHIFT);

    // chip ID is split across registers 0 and 1
    reg_vals[0] |= (chip_id << CHIP_ID_SHIFT);
    reg_vals[1] |= (chip_id >> (32-CHIP_ID_SHIFT));

    // Now generate the FuseID check bits.
    // The FuseIDCheck is calculated as the sum of the number of 0-valued bits 
    // in the fields FeatureID, VendorID, OperatorID, PlatformID and ChipID.  
    // The FeatureID is at bits [50:49] of Bank1.

    feature_id = efuse_get_val(1, 1);   // bank 1; reg 1 (bits 63:32)
    feature_id = (feature_id >> (FEATURE_ID_SHIFT-32)) & FEATURE_ID_MASK;

    for (i = 0; i < FEATURE_ID_SIZE_BITS; i++)
    {
        fuse_id_check += (feature_id & 1) ? 0 : 1;
        feature_id >>= 1;
    }

    for (i = 0; i < VENDOR_ID_SIZE_BITS; i++)
    {
        fuse_id_check += (vendor_id & 1) ? 0 : 1;
        vendor_id >>= 1;
    }

    for (i = 0; i < OPERATOR_ID_SIZE_BITS; i++)
    {
        fuse_id_check += (operator_id & 1) ? 0 : 1;
        operator_id >>= 1;
    }

    for (i = 0; i < PLATFORM_ID_SIZE_BITS; i++)
    {
        fuse_id_check += (platform_id & 1) ? 0 : 1;
        platform_id >>= 1;
    }

    for (i = 0; i < CHIP_ID_SIZE_BITS; i++)
    {
        fuse_id_check += (chip_id & 1) ? 0 : 1;
        chip_id >>= 1;
    }

    reg_vals[1] |= (fuse_id_check << (FUSE_ID_CHECK_SHIFT-32));

    DPRINTF(DBG_ERROR, ("%s: values to burn: reg2 = %#08x, reg1 = %#08x, reg0 = %#08x\n", __func__, reg_vals[2], reg_vals[1], reg_vals[0]));
    return efuse_program_bank(OEM_DATA_BANK, reg_vals, 3);
}

