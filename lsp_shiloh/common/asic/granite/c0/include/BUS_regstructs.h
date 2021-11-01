 /*
 **************************************************************************************
 *
 * Copyright (c) 2010 Marvell International, Ltd.
 *
 **************************************************************************************
 *
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell as part of a proprietary software release,
 * the File is considered Marvell Proprietary and Confidential Information, and is
 * licensed to you under the terms of the applicable Commercial License.
 *
 **************************************************************************************
 *
 * Marvell GPL License Option
 *
 * If you received this File from Marvell as part of a Linux distribution, this File
 * is licensed to you in accordance with the terms and conditions of the General Public
 * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
 * modify it under the terms of the GPL License; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GPL License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program.  If not, see http://www.gnu.org/licenses/.
 *
 **************************************************************************************
 *
 * \file BUS_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BUS_REGSTRUCTS_H_
#define _BUS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: BUS (BUS)
//
//====================================================================

//====================================================================
//
//Register File: ADR (ADR)
//
//====================================================================

typedef struct ADR_REGS_s
{
  volatile uint32_t Remap;  ///< 0x0 [W]: Remap
} ADR_REGS_t;

//====================================================================
//
//Register File: MC0_M (MC0_M)
//
//====================================================================

typedef struct MC0_M_REGS_s
{
  volatile uint32_t reserved0[2];
  volatile uint32_t fn_mod_bm_iss;  ///< 0x8 [R/W]: fn_mod_bm_iss
} MC0_M_REGS_t;

//====================================================================
//
//Register File: MC1_M (MC1_M)
//
//====================================================================

typedef struct MC1_M_REGS_s
{
  volatile uint32_t reserved0[2];
  volatile uint32_t fn_mod_bm_iss;  ///< 0x8 [R/W]: fn_mod_bm_iss
  volatile uint32_t reserved1[63];
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} MC1_M_REGS_t;

//====================================================================
//
//Register File: LCM_M (LCM_M)
//
//====================================================================

typedef struct LCM_M_REGS_s
{
  volatile uint32_t reserved0[2];
  volatile uint32_t fn_mod_bm_iss;  ///< 0x8 [R/W]: fn_mod_bm_iss
} LCM_M_REGS_t;

//====================================================================
//
//Register File: PCIE_M (PCIE_M)
//
//====================================================================

typedef struct PCIE_M_REGS_s
{
  volatile uint32_t reserved0[2];
  volatile uint32_t fn_mod_bm_iss;  ///< 0x8 [R/W]: fn_mod_bm_iss
} PCIE_M_REGS_t;

//====================================================================
//
//Register File: NAND_M (NAND_M)
//
//====================================================================

typedef struct NAND_M_REGS_s
{
  volatile uint32_t reserved0[2];
  volatile uint32_t fn_mod_bm_iss;  ///< 0x8 [R/W]: fn_mod_bm_iss
  volatile uint32_t reserved1[63];
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} NAND_M_REGS_t;

//====================================================================
//
//Register File: BOOTSPI_M (BOOTSPI_M)
//
//====================================================================

typedef struct BOOTSPI_M_REGS_s
{
  volatile uint32_t reserved0[2];
  volatile uint32_t fn_mod_bm_iss;  ///< 0x8 [R/W]: fn_mod_bm_iss
  volatile uint32_t reserved1[63];
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} BOOTSPI_M_REGS_t;

//====================================================================
//
//Register File: REG_M (REG_M)
//
//====================================================================

typedef struct REG_M_REGS_s
{
  volatile uint32_t reserved0[2];
  volatile uint32_t fn_mod_bm_iss;  ///< 0x8 [R/W]: fn_mod_bm_iss
  volatile uint32_t reserved1[63];
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} REG_M_REGS_t;

//====================================================================
//
//Register File: CPU_S (CPU_S)
//
//====================================================================

typedef struct CPU_S_REGS_s
{
  volatile uint32_t reserved0[8];
  volatile uint32_t sync_mode;  ///< 0x20 [R/W]: sync_mode
  volatile uint32_t reserved1[7];
  volatile uint32_t wr_tidemark;  ///< 0x40 [R/W]: wr_tidemark
  volatile uint32_t reserved2[47];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} CPU_S_REGS_t;

//====================================================================
//
//Register File: PCIE_S (PCIE_S)
//
//====================================================================

typedef struct PCIE_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} PCIE_S_REGS_t;

//====================================================================
//
//Register File: M2M_S (M2M_S)
//
//====================================================================

typedef struct M2M_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} M2M_S_REGS_t;

//====================================================================
//
//Register File: GLAN_S (GLAN_S)
//
//====================================================================

typedef struct GLAN_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} GLAN_S_REGS_t;

//====================================================================
//
//Register File: LCD_S (LCD_S)
//
//====================================================================

typedef struct LCD_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} LCD_S_REGS_t;

//====================================================================
//
//Register File: ASELA_S (ASELA_S)
//
//====================================================================

typedef struct ASELA_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} ASELA_S_REGS_t;

//====================================================================
//
//Register File: PIP_S (PIP_S)
//
//====================================================================

typedef struct PIP_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} PIP_S_REGS_t;

//====================================================================
//
//Register File: PIE_S (PIE_S)
//
//====================================================================

typedef struct PIE_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} PIE_S_REGS_t;

//====================================================================
//
//Register File: ANTIC_S (ANTIC_S)
//
//====================================================================

typedef struct ANTIC_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} ANTIC_S_REGS_t;

//====================================================================
//
//Register File: CSAHB_S (CSAHB_S)
//
//====================================================================

typedef struct CSAHB_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} CSAHB_S_REGS_t;

//====================================================================
//
//Register File: TIC_S (TIC_S)
//
//====================================================================

typedef struct TIC_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} TIC_S_REGS_t;

//====================================================================
//
//Register File: JPEGR_S (JPEGR_S)
//
//====================================================================

typedef struct JPEGR_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} JPEGR_S_REGS_t;

//====================================================================
//
//Register File: JPEGW_S (JPEGW_S)
//
//====================================================================

typedef struct JPEGW_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} JPEGW_S_REGS_t;

//====================================================================
//
//Register File: JBIG_S (JBIG_S)
//
//====================================================================

typedef struct JBIG_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} JBIG_S_REGS_t;

//====================================================================
//
//Register File: MVDO_S (MVDO_S)
//
//====================================================================

typedef struct MVDO_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} MVDO_S_REGS_t;

//====================================================================
//
//Register File: UPC_S (UPC_S)
//
//====================================================================

typedef struct UPC_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} UPC_S_REGS_t;

//====================================================================
//
//Register File: CDMA_S (CDMA_S)
//
//====================================================================

typedef struct CDMA_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} CDMA_S_REGS_t;

//====================================================================
//
//Register File: USB_S (USB_S)
//
//====================================================================

typedef struct USB_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} USB_S_REGS_t;

//====================================================================
//
//Register File: SDMMC_S (SDMMC_S)
//
//====================================================================

typedef struct SDMMC_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} SDMMC_S_REGS_t;

//====================================================================
//
//Register File: Zx_S (Zx_S)
//
//====================================================================

typedef struct ZX_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} ZX_S_REGS_t;

//====================================================================
//
//Register File: PIC_S (PIC_S)
//
//====================================================================

typedef struct PIC_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} PIC_S_REGS_t;

//====================================================================
//
//Register File: CISX_S (CISX_S)
//
//====================================================================

typedef struct CISX_S_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t read_qos;  ///< 0x100 [R/W]: read_qos
  volatile uint32_t write_qos;  ///< 0x104 [R/W]: write_qos
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} CISX_S_REGS_t;

//====================================================================
//
//Register File: IB1 (IB1)
//
//====================================================================

typedef struct IB1_REGS_s
{
  volatile uint32_t reserved0[2];
  volatile uint32_t fn_mod_bm_iss;  ///< 0x8 [R/W]: fn_mod_bm_iss
  volatile uint32_t reserved1[63];
  volatile uint32_t fn_mod_iss;  ///< 0x108 [R/W]: fn_mod_iss
} IB1_REGS_t;

#endif // BUS
