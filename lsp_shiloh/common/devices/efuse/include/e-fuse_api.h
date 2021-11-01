
/* **********************************************************
* (C) Copyright 2011-2013 Marvell International Ltd.
*               All Rights Reserved
*
*               Marvell Confidential
* ==========================================================
*/

#ifndef _EFUSE_API_H
#define _EFUSE_API_H
#include <stdint.h>
#include <stdbool.h>

/**
 * \brief Read and return an efuse setting.
 * The efuses are separated into banks with 4 32 bit registers per bank.  This
 * returns the register within a given bank.
 * \param[in] bank_num Values range from 0-3 the bank number to access
 * \param[in] reg Values range from 0-3 The register within the bank to return
 * \returns uint32_t
 * \retval The value of the register.
 */
uint32_t efuse_get_val(uint32_t bank_num, uint32_t reg);

/**
 * \brief return if a given fuse is blown
 * \param[in] bank_num The bank number of efuses to check
 * \param[in] bit The bit number in the bank to check.
 * \returns bool
 * \retval true The efuse bit is blown
 * \retval false The efuse bit is not blown
 */
bool efuse_get_bit(uint32_t bank_num, uint32_t bit);

/**
 * \brief Program specified efuse bank.
 * \param bank_num - The number of the bank to program
 * \param bank_reg_val_list - List of uint32_t register values to program in the bank. 
 * The list is ordered from least significant register to most significant register. 
 * For example bits 0-31 are in bank_reg_val_list[0], bits 32-63 are in bank_Reg_val_list[1] and so on. 
 * \param num_bank_regs - Number of valid bank register values in the bank_reg_val_list.
 * \return int
 * \retval 0 Success
 * \retval -1 Failure 
 *  
 * \warning Burning an efuse is an action that cannot be undone.  Once set it remains set. 
 * Do not burn unless you are sure. 
 *
 */
int efuse_program_bank(uint32_t bank_num, uint32_t bank_reg_val_list[], uint32_t num_bank_regs);

/**
 * \brief Program specified efuse bank.
 * \param bank_num - The number of the bank to program
 * \param bit_num - Zero based number of the bit to program in the specified bank.
 * \return int
 * \retval 0 Success
 * \retval -1 Failure 
 *  
 * \warning Burning an efuse is an action that cannot be undone.  Once set it remains set. 
 * Do not burn unless you are sure. 
 *
 */
int efuse_program_bit(uint32_t bank_num, uint32_t bit_num);

/**
 * \brief Program the security fuse bit for the specified bank on asics that support security fuse bits. 
 * Once the security fuse bit is programmed the bank becomes read-only and can no longer be programmed. 
 * \param bank_num - The number of the bank to program
 * \return int
 * \retval 0 Success
 * \retval -1 Failure 
 *  
 * \note Not all asics support the security fuse bits.  The current asics that support the security 
 * fuse bit are: 
 *  - 88PA6170 
 *  
 * \warning Once the security fuse bit is set for a bank the bank becomes read-only. 
 * Do NOT burn the sercurity fuse unless you are absolutely sure. This operation cannot be undone. 
 */
int efuse_program_bank_security_fuse_bit(uint32_t bank_num);


void efuse_init();

#endif
