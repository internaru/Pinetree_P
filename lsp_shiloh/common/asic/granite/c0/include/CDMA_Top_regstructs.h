 /*
 **************************************************************************************
 *
 * Copyright (c) 2011 Marvell International, Ltd.
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
 * \file CDMA_Top_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CDMA_TOP_REGSTRUCTS_H_
#define _CDMA_TOP_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Central DMA registers (CDMA_Top)
/** \brief Registers for "12" channel Central DMA module*/
//
//====================================================================

//====================================================================
//
//Register File: CDMA (Instance 1 of 12) (CDMA0)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA0_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA0_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 2 of 12) (CDMA1)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA1_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA1_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 3 of 12) (CDMA2)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA2_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA2_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 4 of 12) (CDMA3)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA3_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA3_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 5 of 12) (CDMA4)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA4_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA4_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 6 of 12) (CDMA5)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA5_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA5_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 7 of 12) (CDMA6)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA6_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA6_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 8 of 12) (CDMA7)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA7_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA7_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 9 of 12) (CDMA8)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA8_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA8_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 10 of 12) (CDMA9)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA9_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA9_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 11 of 12) (CDMA10)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA10_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA10_REGS_t;

//====================================================================
//
//Register File: CDMA (Instance 12 of 12) (CDMA11)
/** \brief Registers for Central DMA module*/
//
//====================================================================

typedef struct CDMA11_REGS_s
{
  volatile uint32_t CFG;  ///< 0x0 [R/W]: DMA Configuration Register
  volatile uint32_t Control;  ///< 0x4 [R/W]: DMA Channel Control Register
  volatile uint32_t Status;  ///< 0x8 [R]: DMA Status Register
  volatile uint32_t SrcAddr;  ///< 0xc [R/W]: DMA Source Address Register
  volatile uint32_t SrcAddrStat;  ///< 0x10 [R]: Current Source Address Register
  volatile uint32_t DestAddr;  ///< 0x14 [R/W]: Destination Address Register
  volatile uint32_t DestAddrStat;  ///< 0x18 [R]: Current Destination Address Register
  volatile uint32_t FillValue;  ///< 0x1c [R/W]: Fill Value Register
  volatile uint32_t intEn;  ///< 0x20 [R/W]: Interrupt Enable Register
  volatile uint32_t intPend;  ///< 0x24 [R]: Interrupt Pending Register
  volatile uint32_t intAck;  ///< 0x28 [W]: Interrupt Acknowledge Register
  volatile uint32_t intForce;  ///< 0x2c [W]: Interrupt Force Register
  volatile uint32_t TimerControl;  ///< 0x30 [R/W]: DMA Timer Control
  volatile uint32_t TimeOutStat;  ///< 0x34 [R]: Timeout Count Status Register
  volatile uint32_t LLIR;  ///< 0x38 [R/W]: Linked List Index Register
  volatile uint32_t OWCFG;  ///< 0x3c [R/W]: Odd Width Config Register
} CDMA11_REGS_t;

//====================================================================
//
//Register File: CDMA Top Interrupt Register (CDMA_Top_Int)
/** \brief CDMA Top Level Register Set*/
//
//====================================================================

typedef struct CDMA_TOP_INT_REGS_s
{
  volatile uint32_t CDMAInt;  ///< 0x0 [R]: CDMA Interrupt Status Register
  volatile uint32_t CDMAPR;  ///< 0x4 [R]: CDMA Parameter Register
} CDMA_TOP_INT_REGS_t;

#endif // CDMA_TOP
