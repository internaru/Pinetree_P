/*
 * ======================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

#ifndef _EFUSE_CONFIG_H
#define _EFUSE_CONFIG_H

#include "asic.h"

#ifdef __cplusplus
extern "C" {
#endif

EFUSE_REGS_t* efuse_config_get_regs_base();

// private routines specific to the 6170

/**
 * \brief Debug interface to program a development part so that it looks like a production
 * part with all security features enabled.  This is a one-way trip so don't do it
 * unless you really want to.  Note that the security bit in bank 1 must be programmed 
 * after calling this routine (once all other bank 1 values have been set). 
 */
int efuse_program_88PA6170_as_production_part();


/**
 * \brief Program the OEM specific efuse values on the 6170.
 * \param vendor_id - Vendor specific ID. Assigned by Marvell; programmed by OEM.
 * \param operator_id - Vendor specific operator ID.  Assigned by Marvell; programmed by OEM.
 * \param platform_id - Vendor specific platform ID.  Assigned and programmed by OEM. 
 * \param chip_id - Vendor specific chip ID.  Assigned and programmed by OEM. 
 * \return int
 * \retval 0 Success
 * \retval -1 Failure 
 *  
 * \warning Burning an efuse is an action that cannot be undone.  Once set it remains set. 
 * Do not burn unless you are sure. 
 *  
 * \note Once the OEM data is programmed the security bit in Bank 2 must be programmed 
 * using the efuse_program_bank_security_fuse_bit( ) routine.  This causes the chip lifecycle 
 * to advance from Device Manufacturing state to Device Deployment state.  Since programming 
 * the security bit causes the bank to become read-only it should only be done after all other 
 * programming of the fuses has been completed. 
 *
 */
int efuse_program_88PA6170_oem_data(uint32_t vendor_id, uint32_t operator_id, uint32_t platform_id, uint32_t chip_id);

#ifdef __cplusplus
}
#endif

#endif //_EFUSE_CONFIG_H
