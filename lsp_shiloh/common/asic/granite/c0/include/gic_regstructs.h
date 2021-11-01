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
 * \file gic_regstructs.h

 *
 *************************************************************************
 */

#ifndef _GIC_REGSTRUCTS_H_
#define _GIC_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: GIC
//
//====================================================================

typedef struct GIC_DIST_REGS_s
{
      volatile uint32_t ICD_DCR;    ///< 0x0 [rw]: Distributor control register
      volatile uint32_t ICD_ICTR;   ///< 0x4 [r0]: interrupt controller type register
      volatile uint32_t ICD_IIDR;   ///< 0x8 [ro]: Implementer identification register
      volatile uint32_t reserved0[29]; ///< 0xc->0x7c reserved
      volatile uint32_t ICD_ISR;    ///< 0x80 [rw]: Interrupt security registers
      volatile uint32_t reserved1[31]; ///< 0x84 -> 0xfc reserved
      volatile uint32_t ICD_ISER[32];   ///< 0x100->0x17c Interrupt set-enable registers
      volatile uint32_t ICD_ICER[32];   ///< 0x180->0x1fc Interrupt clear-enable registers
      volatile uint32_t ICD_ISPR[32];   ///< 0x200->0x27c Interrupt Set-Pending Registers
      volatile uint32_t ICD_ICPR[32];   ///< 0x280->0x2fc Interrupt Clear-Pending registers
      volatile uint32_t ICD_ABR[32];    ///< 0x300->0x37c Active Bit Registers
      volatile uint32_t Reserved2[32]; 
      volatile uint32_t ICD_IPR[255];   ///< 0x400->0x7f8 Interrupt Priority Registers
      volatile uint32_t Reserved3;      ///< 0x7fc reserved
      volatile uint32_t ICD_IPTRRO[8];    ///< 0x800->0x81c Interrupt Processor Targets
      volatile uint32_t ICD_IPTRRW[248];  ///< 0x820->0xbf8 interrupt process targets
      volatile uint32_t ICD_ICFR[64];   ///< 0xc00->0xcfc Interrupt config registers
      volatile uint32_t Reserved4[128]; 
      volatile uint32_t ICD_SGIR;   ///< 0xf00 Software Generated interrupt register
      volatile uint32_t Reserved5[51]; 
      volatile uint32_t ICD_ID[12]; ///< 0xfd0->0xffc Identification registers.
} GIC_DIST_REGS_t;

typedef struct GIC_CPU_REGS_s
{
      volatile uint32_t ICC_ICR;  ///< 0x00 Interface control register
      volatile uint32_t ICC_PMR;  ///< 0x4 Interrupt Priority mask register
      volatile uint32_t ICC_BPR;  ///< 0x8 Binary Point Register
      volatile uint32_t ICC_IAR;  ///< 0x0c Interrupt ack register
      volatile uint32_t ICC_EOIR; ///< 0x10 End of intterrupt register
      volatile uint32_t ICC_RPR;  ///< 0x14 Running Priority Register
      volatile uint32_t ICC_HPIR; ///< 0x18 Highest Pending Interrupt Register
      volatile uint32_t ICC_ABPR; ///< 0x1c Aliased Binary Point Register
      volatile uint32_t Reserved[55];
      volatile uint32_t ICC_IIDR; ///< 0xfc Interface Id register
} GIC_CPU_REGS_t;
#endif 
