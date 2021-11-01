 /*
 *************************************************************************
 *
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                       Marvell Confidential
 *
 *************************************************************************
 */
/**
 * \file engctl_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ENGCTL_REGMASKS_H_
#define _ENGCTL_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Registers for the Engine Control module (engctl)
/** \brief Registers for the Engine Control module*/
//
//====================================================================

//====================================================================
//Register: Engine Configuration Register (ECFGR)
/** \brief This register contains bits to control access to the various System Interface Registers.
      For each System Interface Register there are two bits in this register.
      This register should be written after the engine control code has been downloaded to the
      code-store SRAM. The priority encoding bits define the operation of the status and control
      registers (the required values are fixed for a particular engine implementation).
      The encoding of these bits is as follows:
<br>
<br>
  00 = system CPU write/engine control read-only
<br>
  01 = system CPU set/engine control clear
<br>
  10 = system CPU clear/engine control set
<br>
  11 = system CPU read-only/engine control write
<br>
<br>*/
//====================================================================

#define  ENGCTL_ECFGR_P0ACCCTRL_MASK 0xc0000000
#define  ENGCTL_ECFGR_P0ACCCTRL_SHIFT 30
#define  ENGCTL_ECFGR_P0ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P0ACCCTRL_MASK) >> ENGCTL_ECFGR_P0ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P0ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P0ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P0ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P1ACCCTRL_MASK 0x30000000
#define  ENGCTL_ECFGR_P1ACCCTRL_SHIFT 28
#define  ENGCTL_ECFGR_P1ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P1ACCCTRL_MASK) >> ENGCTL_ECFGR_P1ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P1ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P1ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P1ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P2ACCCTRL_MASK 0xc000000
#define  ENGCTL_ECFGR_P2ACCCTRL_SHIFT 26
#define  ENGCTL_ECFGR_P2ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P2ACCCTRL_MASK) >> ENGCTL_ECFGR_P2ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P2ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P2ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P2ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P3ACCCTRL_MASK 0x3000000
#define  ENGCTL_ECFGR_P3ACCCTRL_SHIFT 24
#define  ENGCTL_ECFGR_P3ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P3ACCCTRL_MASK) >> ENGCTL_ECFGR_P3ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P3ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P3ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P3ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P4ACCCTRL_MASK 0xc00000
#define  ENGCTL_ECFGR_P4ACCCTRL_SHIFT 22
#define  ENGCTL_ECFGR_P4ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P4ACCCTRL_MASK) >> ENGCTL_ECFGR_P4ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P4ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P4ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P4ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P5ACCCTRL_MASK 0x300000
#define  ENGCTL_ECFGR_P5ACCCTRL_SHIFT 20
#define  ENGCTL_ECFGR_P5ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P5ACCCTRL_MASK) >> ENGCTL_ECFGR_P5ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P5ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P5ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P5ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P6ACCCTRL_MASK 0xc0000
#define  ENGCTL_ECFGR_P6ACCCTRL_SHIFT 18
#define  ENGCTL_ECFGR_P6ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P6ACCCTRL_MASK) >> ENGCTL_ECFGR_P6ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P6ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P6ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P6ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P7ACCCTRL_MASK 0x30000
#define  ENGCTL_ECFGR_P7ACCCTRL_SHIFT 16
#define  ENGCTL_ECFGR_P7ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P7ACCCTRL_MASK) >> ENGCTL_ECFGR_P7ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P7ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P7ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P7ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P8ACCCTRL_MASK 0xc000
#define  ENGCTL_ECFGR_P8ACCCTRL_SHIFT 14
#define  ENGCTL_ECFGR_P8ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P8ACCCTRL_MASK) >> ENGCTL_ECFGR_P8ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P8ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P8ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P8ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P9ACCCTRL_MASK 0x3000
#define  ENGCTL_ECFGR_P9ACCCTRL_SHIFT 12
#define  ENGCTL_ECFGR_P9ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P9ACCCTRL_MASK) >> ENGCTL_ECFGR_P9ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P9ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P9ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P9ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P10ACCCTRL_MASK 0xc00
#define  ENGCTL_ECFGR_P10ACCCTRL_SHIFT 10
#define  ENGCTL_ECFGR_P10ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P10ACCCTRL_MASK) >> ENGCTL_ECFGR_P10ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P10ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P10ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P10ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_P11ACCCTRL_MASK 0x300
#define  ENGCTL_ECFGR_P11ACCCTRL_SHIFT 8
#define  ENGCTL_ECFGR_P11ACCCTRL_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_P11ACCCTRL_MASK) >> ENGCTL_ECFGR_P11ACCCTRL_SHIFT)
#define  ENGCTL_ECFGR_P11ACCCTRL_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_P11ACCCTRL_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_P11ACCCTRL_SHIFT))

#define  ENGCTL_ECFGR_RESERVED1_MASK 0xfe
#define  ENGCTL_ECFGR_RESERVED1_SHIFT 1
#define  ENGCTL_ECFGR_RESERVED1_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_RESERVED1_MASK) >> ENGCTL_ECFGR_RESERVED1_SHIFT)
#define  ENGCTL_ECFGR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_RESERVED1_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_RESERVED1_SHIFT))

#define  ENGCTL_ECFGR_ENABLE_MASK 0x1
#define  ENGCTL_ECFGR_ENABLE_SHIFT 0
#define  ENGCTL_ECFGR_ENABLE_MASK_SHIFT(reg) (((reg) & ENGCTL_ECFGR_ENABLE_MASK) >> ENGCTL_ECFGR_ENABLE_SHIFT)
#define  ENGCTL_ECFGR_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECFGR_ENABLE_MASK) | (((uint32_t)val) << ENGCTL_ECFGR_ENABLE_SHIFT))

//====================================================================
//Register: Engine Control 0 Register (ENGCTL0)
/** \brief Four of the 8-bit System Interface registers (p0-p3) are accessed through this register*/
//====================================================================

#define  ENGCTL_ENGCTL0_P3_MASK 0xff000000
#define  ENGCTL_ENGCTL0_P3_SHIFT 24
#define  ENGCTL_ENGCTL0_P3_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL0_P3_MASK) >> ENGCTL_ENGCTL0_P3_SHIFT)
#define  ENGCTL_ENGCTL0_P3_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL0_P3_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL0_P3_SHIFT))

#define  ENGCTL_ENGCTL0_P2_MASK 0xff0000
#define  ENGCTL_ENGCTL0_P2_SHIFT 16
#define  ENGCTL_ENGCTL0_P2_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL0_P2_MASK) >> ENGCTL_ENGCTL0_P2_SHIFT)
#define  ENGCTL_ENGCTL0_P2_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL0_P2_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL0_P2_SHIFT))

#define  ENGCTL_ENGCTL0_P1_MASK 0xff00
#define  ENGCTL_ENGCTL0_P1_SHIFT 8
#define  ENGCTL_ENGCTL0_P1_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL0_P1_MASK) >> ENGCTL_ENGCTL0_P1_SHIFT)
#define  ENGCTL_ENGCTL0_P1_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL0_P1_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL0_P1_SHIFT))

#define  ENGCTL_ENGCTL0_P0_MASK 0xff
#define  ENGCTL_ENGCTL0_P0_SHIFT 0
#define  ENGCTL_ENGCTL0_P0_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL0_P0_MASK) >> ENGCTL_ENGCTL0_P0_SHIFT)
#define  ENGCTL_ENGCTL0_P0_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL0_P0_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL0_P0_SHIFT))

//====================================================================
//Register: Engine Control 1 Register (ENGCTL1)
/** \brief Four of the 8-bit System Interface registers (p4-p7) are accessed through this register*/
//====================================================================

#define  ENGCTL_ENGCTL1_P7_MASK 0xff000000
#define  ENGCTL_ENGCTL1_P7_SHIFT 24
#define  ENGCTL_ENGCTL1_P7_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL1_P7_MASK) >> ENGCTL_ENGCTL1_P7_SHIFT)
#define  ENGCTL_ENGCTL1_P7_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL1_P7_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL1_P7_SHIFT))

#define  ENGCTL_ENGCTL1_P6_MASK 0xff0000
#define  ENGCTL_ENGCTL1_P6_SHIFT 16
#define  ENGCTL_ENGCTL1_P6_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL1_P6_MASK) >> ENGCTL_ENGCTL1_P6_SHIFT)
#define  ENGCTL_ENGCTL1_P6_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL1_P6_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL1_P6_SHIFT))

#define  ENGCTL_ENGCTL1_P5_MASK 0xff00
#define  ENGCTL_ENGCTL1_P5_SHIFT 8
#define  ENGCTL_ENGCTL1_P5_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL1_P5_MASK) >> ENGCTL_ENGCTL1_P5_SHIFT)
#define  ENGCTL_ENGCTL1_P5_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL1_P5_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL1_P5_SHIFT))

#define  ENGCTL_ENGCTL1_P4_MASK 0xff
#define  ENGCTL_ENGCTL1_P4_SHIFT 0
#define  ENGCTL_ENGCTL1_P4_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL1_P4_MASK) >> ENGCTL_ENGCTL1_P4_SHIFT)
#define  ENGCTL_ENGCTL1_P4_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL1_P4_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL1_P4_SHIFT))

//====================================================================
//Register: Engine Control 2 Register (ENGCTL2)
/** \brief Four of the 8-bit System Interface registers (p8-p11) are accessed through this register*/
//====================================================================

#define  ENGCTL_ENGCTL2_P11_MASK 0xff000000
#define  ENGCTL_ENGCTL2_P11_SHIFT 24
#define  ENGCTL_ENGCTL2_P11_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL2_P11_MASK) >> ENGCTL_ENGCTL2_P11_SHIFT)
#define  ENGCTL_ENGCTL2_P11_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL2_P11_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL2_P11_SHIFT))

#define  ENGCTL_ENGCTL2_P10_MASK 0xff0000
#define  ENGCTL_ENGCTL2_P10_SHIFT 16
#define  ENGCTL_ENGCTL2_P10_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL2_P10_MASK) >> ENGCTL_ENGCTL2_P10_SHIFT)
#define  ENGCTL_ENGCTL2_P10_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL2_P10_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL2_P10_SHIFT))

#define  ENGCTL_ENGCTL2_P9_MASK 0xff00
#define  ENGCTL_ENGCTL2_P9_SHIFT 8
#define  ENGCTL_ENGCTL2_P9_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL2_P9_MASK) >> ENGCTL_ENGCTL2_P9_SHIFT)
#define  ENGCTL_ENGCTL2_P9_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL2_P9_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL2_P9_SHIFT))

#define  ENGCTL_ENGCTL2_P8_MASK 0xff
#define  ENGCTL_ENGCTL2_P8_SHIFT 0
#define  ENGCTL_ENGCTL2_P8_MASK_SHIFT(reg) (((reg) & ENGCTL_ENGCTL2_P8_MASK) >> ENGCTL_ENGCTL2_P8_SHIFT)
#define  ENGCTL_ENGCTL2_P8_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ENGCTL2_P8_MASK) | (((uint32_t)val) << ENGCTL_ENGCTL2_P8_SHIFT))

//====================================================================
//Register: Engine Control Code Store (ECCS)
/** \brief The executable engine code should be loaded starting at Base + 0x1000. The code is loaded in the least-significant 16-bits of each longword address. The engine controller should be disabled when the system CPU is loading or reading from the code store SRAM.*/
//====================================================================

#define  ENGCTL_ECCS_RESERVED1_MASK 0xffff0000
#define  ENGCTL_ECCS_RESERVED1_SHIFT 16
#define  ENGCTL_ECCS_RESERVED1_MASK_SHIFT(reg) (((reg) & ENGCTL_ECCS_RESERVED1_MASK) >> ENGCTL_ECCS_RESERVED1_SHIFT)
#define  ENGCTL_ECCS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECCS_RESERVED1_MASK) | (((uint32_t)val) << ENGCTL_ECCS_RESERVED1_SHIFT))

#define  ENGCTL_ECCS_CWORD_MASK 0xffff
#define  ENGCTL_ECCS_CWORD_SHIFT 0
#define  ENGCTL_ECCS_CWORD_MASK_SHIFT(reg) (((reg) & ENGCTL_ECCS_CWORD_MASK) >> ENGCTL_ECCS_CWORD_SHIFT)
#define  ENGCTL_ECCS_CWORD_REPLACE_VAL(reg,val) (((reg) & ~ENGCTL_ECCS_CWORD_MASK) | (((uint32_t)val) << ENGCTL_ECCS_CWORD_SHIFT))

#endif // ENGCTL
