
/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file nvram_fwupdate.h
 *
 * \brief Interface to allow new firmware to be burned into nvram device
 *
 **/

#ifndef INC_NVRAM_FWUPDATE_H
#define INC_NVRAM_FWUPDATE_H

#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Burn new firmware.
 * 
 * \param[in]     filename  The location, on the UFS, of the file to burn
 *
 *  \return   error_type_t 
 *  \retval             OK  Everything went well
 * 
 */
error_type_t nvram_fwupdate(const char* filename);
#define NVRAM_FWUPDATE_ERROR_FLASH_RD    (-100)
#define NVRAM_FWUPDATE_ERROR_NO_FLASH_FW (-101)
#define NVRAM_FWUPDATE_ERROR_FILE_RD     (-102)
#define NVRAM_FWUPDATE_ERROR_FILE_SIZE   (-103)
#define NVRAM_FWUPDATE_ERROR_MEMORY      (-104)
#define NVRAM_FWUPDATE_ERROR_BADFILE     (-105)


#ifdef __cplusplus
}
#endif

#endif /* INC_NVRAM_FWUPDATE_H */
