 /*
 *************************************************************************
 *
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *
 *                       Marvell Confidential
 *
 *************************************************************************
 */
/**
 * \file Zx_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ZX_REGMASKS_H_
#define _ZX_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Zx Register Set (Zx)
/** \brief Top-level register file for Zx*/
//
//====================================================================

//====================================================================
//Register: Zx Configuration Register (ZCR)
/** \brief Register used to configure various aspects of the Zx block.*/
//====================================================================

#define  ZX_ZCR_RESERVED1_MASK 0xfffffc00
#define  ZX_ZCR_RESERVED1_SHIFT 10
#define  ZX_ZCR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZCR_RESERVED1_MASK) >> ZX_ZCR_RESERVED1_SHIFT)
#define  ZX_ZCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZCR_RESERVED1_SHIFT))

#define  ZX_ZCR_NOEOI_MASK 0x200
#define  ZX_ZCR_NOEOI_SHIFT 9
#define  ZX_ZCR_NOEOI_MASK_SHIFT(reg) (((reg) & ZX_ZCR_NOEOI_MASK) >> ZX_ZCR_NOEOI_SHIFT)
#define  ZX_ZCR_NOEOI_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_NOEOI_MASK) | (((uint32_t)val) << ZX_ZCR_NOEOI_SHIFT))

#define  ZX_ZCR_NOSOI_MASK 0x100
#define  ZX_ZCR_NOSOI_SHIFT 8
#define  ZX_ZCR_NOSOI_MASK_SHIFT(reg) (((reg) & ZX_ZCR_NOSOI_MASK) >> ZX_ZCR_NOSOI_SHIFT)
#define  ZX_ZCR_NOSOI_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_NOSOI_MASK) | (((uint32_t)val) << ZX_ZCR_NOSOI_SHIFT))

#define  ZX_ZCR_STOPONEOI_MASK 0x80
#define  ZX_ZCR_STOPONEOI_SHIFT 7
#define  ZX_ZCR_STOPONEOI_MASK_SHIFT(reg) (((reg) & ZX_ZCR_STOPONEOI_MASK) >> ZX_ZCR_STOPONEOI_SHIFT)
#define  ZX_ZCR_STOPONEOI_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_STOPONEOI_MASK) | (((uint32_t)val) << ZX_ZCR_STOPONEOI_SHIFT))

#define  ZX_ZCR_RESERVED2_MASK 0x70
#define  ZX_ZCR_RESERVED2_SHIFT 4
#define  ZX_ZCR_RESERVED2_MASK_SHIFT(reg) (((reg) & ZX_ZCR_RESERVED2_MASK) >> ZX_ZCR_RESERVED2_SHIFT)
#define  ZX_ZCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_RESERVED2_MASK) | (((uint32_t)val) << ZX_ZCR_RESERVED2_SHIFT))

#define  ZX_ZCR_BURSTLENGTH_MASK 0x8
#define  ZX_ZCR_BURSTLENGTH_SHIFT 3
#define  ZX_ZCR_BURSTLENGTH_MASK_SHIFT(reg) (((reg) & ZX_ZCR_BURSTLENGTH_MASK) >> ZX_ZCR_BURSTLENGTH_SHIFT)
#define  ZX_ZCR_BURSTLENGTH_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_BURSTLENGTH_MASK) | (((uint32_t)val) << ZX_ZCR_BURSTLENGTH_SHIFT))

#define  ZX_ZCR_RESERVED3_MASK 0x4
#define  ZX_ZCR_RESERVED3_SHIFT 2
#define  ZX_ZCR_RESERVED3_MASK_SHIFT(reg) (((reg) & ZX_ZCR_RESERVED3_MASK) >> ZX_ZCR_RESERVED3_SHIFT)
#define  ZX_ZCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_RESERVED3_MASK) | (((uint32_t)val) << ZX_ZCR_RESERVED3_SHIFT))

#define  ZX_ZCR_ENABLELCM_MASK 0x2
#define  ZX_ZCR_ENABLELCM_SHIFT 1
#define  ZX_ZCR_ENABLELCM_MASK_SHIFT(reg) (((reg) & ZX_ZCR_ENABLELCM_MASK) >> ZX_ZCR_ENABLELCM_SHIFT)
#define  ZX_ZCR_ENABLELCM_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_ENABLELCM_MASK) | (((uint32_t)val) << ZX_ZCR_ENABLELCM_SHIFT))

#define  ZX_ZCR_ENABLE_MASK 0x1
#define  ZX_ZCR_ENABLE_SHIFT 0
#define  ZX_ZCR_ENABLE_MASK_SHIFT(reg) (((reg) & ZX_ZCR_ENABLE_MASK) >> ZX_ZCR_ENABLE_SHIFT)
#define  ZX_ZCR_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCR_ENABLE_MASK) | (((uint32_t)val) << ZX_ZCR_ENABLE_SHIFT))

//====================================================================
//Register: Zx Status Register (ZSR)
/** \brief This register contains status bits used for development and debug*/
//====================================================================

#define  ZX_ZSR_RESERVED1_MASK 0xff000000
#define  ZX_ZSR_RESERVED1_SHIFT 24
#define  ZX_ZSR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZSR_RESERVED1_MASK) >> ZX_ZSR_RESERVED1_SHIFT)
#define  ZX_ZSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZSR_RESERVED1_SHIFT))

#define  ZX_ZSR_ERRORCODE_MASK 0xff0000
#define  ZX_ZSR_ERRORCODE_SHIFT 16
#define  ZX_ZSR_ERRORCODE_MASK_SHIFT(reg) (((reg) & ZX_ZSR_ERRORCODE_MASK) >> ZX_ZSR_ERRORCODE_SHIFT)
#define  ZX_ZSR_ERRORCODE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_ERRORCODE_MASK) | (((uint32_t)val) << ZX_ZSR_ERRORCODE_SHIFT))

#define  ZX_ZSR_RESERVED2_MASK 0xe000
#define  ZX_ZSR_RESERVED2_SHIFT 13
#define  ZX_ZSR_RESERVED2_MASK_SHIFT(reg) (((reg) & ZX_ZSR_RESERVED2_MASK) >> ZX_ZSR_RESERVED2_SHIFT)
#define  ZX_ZSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_RESERVED2_MASK) | (((uint32_t)val) << ZX_ZSR_RESERVED2_SHIFT))

#define  ZX_ZSR_OUTHALT_MASK 0x1000
#define  ZX_ZSR_OUTHALT_SHIFT 12
#define  ZX_ZSR_OUTHALT_MASK_SHIFT(reg) (((reg) & ZX_ZSR_OUTHALT_MASK) >> ZX_ZSR_OUTHALT_SHIFT)
#define  ZX_ZSR_OUTHALT_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_OUTHALT_MASK) | (((uint32_t)val) << ZX_ZSR_OUTHALT_SHIFT))

#define  ZX_ZSR_PARTIALCMD_MASK 0x800
#define  ZX_ZSR_PARTIALCMD_SHIFT 11
#define  ZX_ZSR_PARTIALCMD_MASK_SHIFT(reg) (((reg) & ZX_ZSR_PARTIALCMD_MASK) >> ZX_ZSR_PARTIALCMD_SHIFT)
#define  ZX_ZSR_PARTIALCMD_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_PARTIALCMD_MASK) | (((uint32_t)val) << ZX_ZSR_PARTIALCMD_SHIFT))

#define  ZX_ZSR_PIXDMABUSY_MASK 0x400
#define  ZX_ZSR_PIXDMABUSY_SHIFT 10
#define  ZX_ZSR_PIXDMABUSY_MASK_SHIFT(reg) (((reg) & ZX_ZSR_PIXDMABUSY_MASK) >> ZX_ZSR_PIXDMABUSY_SHIFT)
#define  ZX_ZSR_PIXDMABUSY_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_PIXDMABUSY_MASK) | (((uint32_t)val) << ZX_ZSR_PIXDMABUSY_SHIFT))

#define  ZX_ZSR_ZXDMABUSY_MASK 0x200
#define  ZX_ZSR_ZXDMABUSY_SHIFT 9
#define  ZX_ZSR_ZXDMABUSY_MASK_SHIFT(reg) (((reg) & ZX_ZSR_ZXDMABUSY_MASK) >> ZX_ZSR_ZXDMABUSY_SHIFT)
#define  ZX_ZSR_ZXDMABUSY_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_ZXDMABUSY_MASK) | (((uint32_t)val) << ZX_ZSR_ZXDMABUSY_SHIFT))

#define  ZX_ZSR_RESERVED3_MASK 0x100
#define  ZX_ZSR_RESERVED3_SHIFT 8
#define  ZX_ZSR_RESERVED3_MASK_SHIFT(reg) (((reg) & ZX_ZSR_RESERVED3_MASK) >> ZX_ZSR_RESERVED3_SHIFT)
#define  ZX_ZSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_RESERVED3_MASK) | (((uint32_t)val) << ZX_ZSR_RESERVED3_SHIFT))

#define  ZX_ZSR_PIXBUFFULL_MASK 0x80
#define  ZX_ZSR_PIXBUFFULL_SHIFT 7
#define  ZX_ZSR_PIXBUFFULL_MASK_SHIFT(reg) (((reg) & ZX_ZSR_PIXBUFFULL_MASK) >> ZX_ZSR_PIXBUFFULL_SHIFT)
#define  ZX_ZSR_PIXBUFFULL_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_PIXBUFFULL_MASK) | (((uint32_t)val) << ZX_ZSR_PIXBUFFULL_SHIFT))

#define  ZX_ZSR_PIXBUFEMPTY_MASK 0x40
#define  ZX_ZSR_PIXBUFEMPTY_SHIFT 6
#define  ZX_ZSR_PIXBUFEMPTY_MASK_SHIFT(reg) (((reg) & ZX_ZSR_PIXBUFEMPTY_MASK) >> ZX_ZSR_PIXBUFEMPTY_SHIFT)
#define  ZX_ZSR_PIXBUFEMPTY_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_PIXBUFEMPTY_MASK) | (((uint32_t)val) << ZX_ZSR_PIXBUFEMPTY_SHIFT))

#define  ZX_ZSR_ZXBUFFULL_MASK 0x20
#define  ZX_ZSR_ZXBUFFULL_SHIFT 5
#define  ZX_ZSR_ZXBUFFULL_MASK_SHIFT(reg) (((reg) & ZX_ZSR_ZXBUFFULL_MASK) >> ZX_ZSR_ZXBUFFULL_SHIFT)
#define  ZX_ZSR_ZXBUFFULL_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_ZXBUFFULL_MASK) | (((uint32_t)val) << ZX_ZSR_ZXBUFFULL_SHIFT))

#define  ZX_ZSR_ZXBUFEMPTY_MASK 0x10
#define  ZX_ZSR_ZXBUFEMPTY_SHIFT 4
#define  ZX_ZSR_ZXBUFEMPTY_MASK_SHIFT(reg) (((reg) & ZX_ZSR_ZXBUFEMPTY_MASK) >> ZX_ZSR_ZXBUFEMPTY_SHIFT)
#define  ZX_ZSR_ZXBUFEMPTY_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_ZXBUFEMPTY_MASK) | (((uint32_t)val) << ZX_ZSR_ZXBUFEMPTY_SHIFT))

#define  ZX_ZSR_OUTBUFFULL_MASK 0x8
#define  ZX_ZSR_OUTBUFFULL_SHIFT 3
#define  ZX_ZSR_OUTBUFFULL_MASK_SHIFT(reg) (((reg) & ZX_ZSR_OUTBUFFULL_MASK) >> ZX_ZSR_OUTBUFFULL_SHIFT)
#define  ZX_ZSR_OUTBUFFULL_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_OUTBUFFULL_MASK) | (((uint32_t)val) << ZX_ZSR_OUTBUFFULL_SHIFT))

#define  ZX_ZSR_OUTBUFEMPTY_MASK 0x4
#define  ZX_ZSR_OUTBUFEMPTY_SHIFT 2
#define  ZX_ZSR_OUTBUFEMPTY_MASK_SHIFT(reg) (((reg) & ZX_ZSR_OUTBUFEMPTY_MASK) >> ZX_ZSR_OUTBUFEMPTY_SHIFT)
#define  ZX_ZSR_OUTBUFEMPTY_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_OUTBUFEMPTY_MASK) | (((uint32_t)val) << ZX_ZSR_OUTBUFEMPTY_SHIFT))

#define  ZX_ZSR_LINEBUFFULL_MASK 0x2
#define  ZX_ZSR_LINEBUFFULL_SHIFT 1
#define  ZX_ZSR_LINEBUFFULL_MASK_SHIFT(reg) (((reg) & ZX_ZSR_LINEBUFFULL_MASK) >> ZX_ZSR_LINEBUFFULL_SHIFT)
#define  ZX_ZSR_LINEBUFFULL_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_LINEBUFFULL_MASK) | (((uint32_t)val) << ZX_ZSR_LINEBUFFULL_SHIFT))

#define  ZX_ZSR_LINEBUFEMPTY_MASK 0x1
#define  ZX_ZSR_LINEBUFEMPTY_SHIFT 0
#define  ZX_ZSR_LINEBUFEMPTY_MASK_SHIFT(reg) (((reg) & ZX_ZSR_LINEBUFEMPTY_MASK) >> ZX_ZSR_LINEBUFEMPTY_SHIFT)
#define  ZX_ZSR_LINEBUFEMPTY_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZSR_LINEBUFEMPTY_MASK) | (((uint32_t)val) << ZX_ZSR_LINEBUFEMPTY_SHIFT))

//====================================================================
//Register: Zx Interrupt Enable Register (ZIER)
/** \brief This register contains individual enable bits for each interrupt cause*/
//====================================================================

#define  ZX_ZIER_RESERVED1_MASK 0xffffffe0
#define  ZX_ZIER_RESERVED1_SHIFT 5
#define  ZX_ZIER_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZIER_RESERVED1_MASK) >> ZX_ZIER_RESERVED1_SHIFT)
#define  ZX_ZIER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIER_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZIER_RESERVED1_SHIFT))

#define  ZX_ZIER_ENDOFIMAGE_MASK 0x10
#define  ZX_ZIER_ENDOFIMAGE_SHIFT 4
#define  ZX_ZIER_ENDOFIMAGE_MASK_SHIFT(reg) (((reg) & ZX_ZIER_ENDOFIMAGE_MASK) >> ZX_ZIER_ENDOFIMAGE_SHIFT)
#define  ZX_ZIER_ENDOFIMAGE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIER_ENDOFIMAGE_MASK) | (((uint32_t)val) << ZX_ZIER_ENDOFIMAGE_SHIFT))

#define  ZX_ZIER_CLEARCOMPLETE_MASK 0x8
#define  ZX_ZIER_CLEARCOMPLETE_SHIFT 3
#define  ZX_ZIER_CLEARCOMPLETE_MASK_SHIFT(reg) (((reg) & ZX_ZIER_CLEARCOMPLETE_MASK) >> ZX_ZIER_CLEARCOMPLETE_SHIFT)
#define  ZX_ZIER_CLEARCOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIER_CLEARCOMPLETE_MASK) | (((uint32_t)val) << ZX_ZIER_CLEARCOMPLETE_SHIFT))

#define  ZX_ZIER_ILLEGALCMD_MASK 0x4
#define  ZX_ZIER_ILLEGALCMD_SHIFT 2
#define  ZX_ZIER_ILLEGALCMD_MASK_SHIFT(reg) (((reg) & ZX_ZIER_ILLEGALCMD_MASK) >> ZX_ZIER_ILLEGALCMD_SHIFT)
#define  ZX_ZIER_ILLEGALCMD_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIER_ILLEGALCMD_MASK) | (((uint32_t)val) << ZX_ZIER_ILLEGALCMD_SHIFT))

#define  ZX_ZIER_DESC_MASK 0x2
#define  ZX_ZIER_DESC_SHIFT 1
#define  ZX_ZIER_DESC_MASK_SHIFT(reg) (((reg) & ZX_ZIER_DESC_MASK) >> ZX_ZIER_DESC_SHIFT)
#define  ZX_ZIER_DESC_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIER_DESC_MASK) | (((uint32_t)val) << ZX_ZIER_DESC_SHIFT))

#define  ZX_ZIER_OWN_MASK 0x1
#define  ZX_ZIER_OWN_SHIFT 0
#define  ZX_ZIER_OWN_MASK_SHIFT(reg) (((reg) & ZX_ZIER_OWN_MASK) >> ZX_ZIER_OWN_SHIFT)
#define  ZX_ZIER_OWN_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIER_OWN_MASK) | (((uint32_t)val) << ZX_ZIER_OWN_SHIFT))

//====================================================================
//Register: Zx Interrupt Pending Register (ZIPR)
/** \brief This register contains individual pending bits for each interrupt cause*/
//====================================================================

#define  ZX_ZIPR_RESERVED1_MASK 0xffffffe0
#define  ZX_ZIPR_RESERVED1_SHIFT 5
#define  ZX_ZIPR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZIPR_RESERVED1_MASK) >> ZX_ZIPR_RESERVED1_SHIFT)
#define  ZX_ZIPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIPR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZIPR_RESERVED1_SHIFT))

#define  ZX_ZIPR_ENDOFIMAGE_MASK 0x10
#define  ZX_ZIPR_ENDOFIMAGE_SHIFT 4
#define  ZX_ZIPR_ENDOFIMAGE_MASK_SHIFT(reg) (((reg) & ZX_ZIPR_ENDOFIMAGE_MASK) >> ZX_ZIPR_ENDOFIMAGE_SHIFT)
#define  ZX_ZIPR_ENDOFIMAGE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIPR_ENDOFIMAGE_MASK) | (((uint32_t)val) << ZX_ZIPR_ENDOFIMAGE_SHIFT))

#define  ZX_ZIPR_CLEARCOMPLETE_MASK 0x8
#define  ZX_ZIPR_CLEARCOMPLETE_SHIFT 3
#define  ZX_ZIPR_CLEARCOMPLETE_MASK_SHIFT(reg) (((reg) & ZX_ZIPR_CLEARCOMPLETE_MASK) >> ZX_ZIPR_CLEARCOMPLETE_SHIFT)
#define  ZX_ZIPR_CLEARCOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIPR_CLEARCOMPLETE_MASK) | (((uint32_t)val) << ZX_ZIPR_CLEARCOMPLETE_SHIFT))

#define  ZX_ZIPR_ILLEGALCMD_MASK 0x4
#define  ZX_ZIPR_ILLEGALCMD_SHIFT 2
#define  ZX_ZIPR_ILLEGALCMD_MASK_SHIFT(reg) (((reg) & ZX_ZIPR_ILLEGALCMD_MASK) >> ZX_ZIPR_ILLEGALCMD_SHIFT)
#define  ZX_ZIPR_ILLEGALCMD_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIPR_ILLEGALCMD_MASK) | (((uint32_t)val) << ZX_ZIPR_ILLEGALCMD_SHIFT))

#define  ZX_ZIPR_DESC_MASK 0x2
#define  ZX_ZIPR_DESC_SHIFT 1
#define  ZX_ZIPR_DESC_MASK_SHIFT(reg) (((reg) & ZX_ZIPR_DESC_MASK) >> ZX_ZIPR_DESC_SHIFT)
#define  ZX_ZIPR_DESC_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIPR_DESC_MASK) | (((uint32_t)val) << ZX_ZIPR_DESC_SHIFT))

#define  ZX_ZIPR_OWN_MASK 0x1
#define  ZX_ZIPR_OWN_SHIFT 0
#define  ZX_ZIPR_OWN_MASK_SHIFT(reg) (((reg) & ZX_ZIPR_OWN_MASK) >> ZX_ZIPR_OWN_SHIFT)
#define  ZX_ZIPR_OWN_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIPR_OWN_MASK) | (((uint32_t)val) << ZX_ZIPR_OWN_SHIFT))

//====================================================================
//Register: Zx Interrupt Active Register (ZIAR)
/** \brief This register contains individual active bits for each interrupt cause*/
//====================================================================

#define  ZX_ZIAR_RESERVED1_MASK 0xffffffe0
#define  ZX_ZIAR_RESERVED1_SHIFT 5
#define  ZX_ZIAR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZIAR_RESERVED1_MASK) >> ZX_ZIAR_RESERVED1_SHIFT)
#define  ZX_ZIAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIAR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZIAR_RESERVED1_SHIFT))

#define  ZX_ZIAR_ENDOFIMAGE_MASK 0x10
#define  ZX_ZIAR_ENDOFIMAGE_SHIFT 4
#define  ZX_ZIAR_ENDOFIMAGE_MASK_SHIFT(reg) (((reg) & ZX_ZIAR_ENDOFIMAGE_MASK) >> ZX_ZIAR_ENDOFIMAGE_SHIFT)
#define  ZX_ZIAR_ENDOFIMAGE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIAR_ENDOFIMAGE_MASK) | (((uint32_t)val) << ZX_ZIAR_ENDOFIMAGE_SHIFT))

#define  ZX_ZIAR_CLEARCOMPLETE_MASK 0x8
#define  ZX_ZIAR_CLEARCOMPLETE_SHIFT 3
#define  ZX_ZIAR_CLEARCOMPLETE_MASK_SHIFT(reg) (((reg) & ZX_ZIAR_CLEARCOMPLETE_MASK) >> ZX_ZIAR_CLEARCOMPLETE_SHIFT)
#define  ZX_ZIAR_CLEARCOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIAR_CLEARCOMPLETE_MASK) | (((uint32_t)val) << ZX_ZIAR_CLEARCOMPLETE_SHIFT))

#define  ZX_ZIAR_ILLEGALCMD_MASK 0x4
#define  ZX_ZIAR_ILLEGALCMD_SHIFT 2
#define  ZX_ZIAR_ILLEGALCMD_MASK_SHIFT(reg) (((reg) & ZX_ZIAR_ILLEGALCMD_MASK) >> ZX_ZIAR_ILLEGALCMD_SHIFT)
#define  ZX_ZIAR_ILLEGALCMD_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIAR_ILLEGALCMD_MASK) | (((uint32_t)val) << ZX_ZIAR_ILLEGALCMD_SHIFT))

#define  ZX_ZIAR_DESC_MASK 0x2
#define  ZX_ZIAR_DESC_SHIFT 1
#define  ZX_ZIAR_DESC_MASK_SHIFT(reg) (((reg) & ZX_ZIAR_DESC_MASK) >> ZX_ZIAR_DESC_SHIFT)
#define  ZX_ZIAR_DESC_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIAR_DESC_MASK) | (((uint32_t)val) << ZX_ZIAR_DESC_SHIFT))

#define  ZX_ZIAR_OWN_MASK 0x1
#define  ZX_ZIAR_OWN_SHIFT 0
#define  ZX_ZIAR_OWN_MASK_SHIFT(reg) (((reg) & ZX_ZIAR_OWN_MASK) >> ZX_ZIAR_OWN_SHIFT)
#define  ZX_ZIAR_OWN_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIAR_OWN_MASK) | (((uint32_t)val) << ZX_ZIAR_OWN_SHIFT))

//====================================================================
//Register: Zx Interrupt Clear Register (ZICR)
/** \brief This register contains individual clear bits for each interrupt cause*/
//====================================================================

#define  ZX_ZICR_RESERVED1_MASK 0xffffffe0
#define  ZX_ZICR_RESERVED1_SHIFT 5
#define  ZX_ZICR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZICR_RESERVED1_MASK) >> ZX_ZICR_RESERVED1_SHIFT)
#define  ZX_ZICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZICR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZICR_RESERVED1_SHIFT))

#define  ZX_ZICR_ENDOFIMAGE_MASK 0x10
#define  ZX_ZICR_ENDOFIMAGE_SHIFT 4
#define  ZX_ZICR_ENDOFIMAGE_MASK_SHIFT(reg) (((reg) & ZX_ZICR_ENDOFIMAGE_MASK) >> ZX_ZICR_ENDOFIMAGE_SHIFT)
#define  ZX_ZICR_ENDOFIMAGE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZICR_ENDOFIMAGE_MASK) | (((uint32_t)val) << ZX_ZICR_ENDOFIMAGE_SHIFT))

#define  ZX_ZICR_CLEARCOMPLETE_MASK 0x8
#define  ZX_ZICR_CLEARCOMPLETE_SHIFT 3
#define  ZX_ZICR_CLEARCOMPLETE_MASK_SHIFT(reg) (((reg) & ZX_ZICR_CLEARCOMPLETE_MASK) >> ZX_ZICR_CLEARCOMPLETE_SHIFT)
#define  ZX_ZICR_CLEARCOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZICR_CLEARCOMPLETE_MASK) | (((uint32_t)val) << ZX_ZICR_CLEARCOMPLETE_SHIFT))

#define  ZX_ZICR_ILLEGALCMD_MASK 0x4
#define  ZX_ZICR_ILLEGALCMD_SHIFT 2
#define  ZX_ZICR_ILLEGALCMD_MASK_SHIFT(reg) (((reg) & ZX_ZICR_ILLEGALCMD_MASK) >> ZX_ZICR_ILLEGALCMD_SHIFT)
#define  ZX_ZICR_ILLEGALCMD_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZICR_ILLEGALCMD_MASK) | (((uint32_t)val) << ZX_ZICR_ILLEGALCMD_SHIFT))

#define  ZX_ZICR_DESC_MASK 0x2
#define  ZX_ZICR_DESC_SHIFT 1
#define  ZX_ZICR_DESC_MASK_SHIFT(reg) (((reg) & ZX_ZICR_DESC_MASK) >> ZX_ZICR_DESC_SHIFT)
#define  ZX_ZICR_DESC_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZICR_DESC_MASK) | (((uint32_t)val) << ZX_ZICR_DESC_SHIFT))

#define  ZX_ZICR_OWN_MASK 0x1
#define  ZX_ZICR_OWN_SHIFT 0
#define  ZX_ZICR_OWN_MASK_SHIFT(reg) (((reg) & ZX_ZICR_OWN_MASK) >> ZX_ZICR_OWN_SHIFT)
#define  ZX_ZICR_OWN_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZICR_OWN_MASK) | (((uint32_t)val) << ZX_ZICR_OWN_SHIFT))

//====================================================================
//Register: Zx Interrupt Force Register (ZIFR)
/** \brief This register contains individual force bits for each interrupt cause*/
//====================================================================

#define  ZX_ZIFR_RESERVED1_MASK 0xffffffe0
#define  ZX_ZIFR_RESERVED1_SHIFT 5
#define  ZX_ZIFR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZIFR_RESERVED1_MASK) >> ZX_ZIFR_RESERVED1_SHIFT)
#define  ZX_ZIFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIFR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZIFR_RESERVED1_SHIFT))

#define  ZX_ZIFR_ENDOFIMAGE_MASK 0x10
#define  ZX_ZIFR_ENDOFIMAGE_SHIFT 4
#define  ZX_ZIFR_ENDOFIMAGE_MASK_SHIFT(reg) (((reg) & ZX_ZIFR_ENDOFIMAGE_MASK) >> ZX_ZIFR_ENDOFIMAGE_SHIFT)
#define  ZX_ZIFR_ENDOFIMAGE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIFR_ENDOFIMAGE_MASK) | (((uint32_t)val) << ZX_ZIFR_ENDOFIMAGE_SHIFT))

#define  ZX_ZIFR_CLEARCOMPLETE_MASK 0x8
#define  ZX_ZIFR_CLEARCOMPLETE_SHIFT 3
#define  ZX_ZIFR_CLEARCOMPLETE_MASK_SHIFT(reg) (((reg) & ZX_ZIFR_CLEARCOMPLETE_MASK) >> ZX_ZIFR_CLEARCOMPLETE_SHIFT)
#define  ZX_ZIFR_CLEARCOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIFR_CLEARCOMPLETE_MASK) | (((uint32_t)val) << ZX_ZIFR_CLEARCOMPLETE_SHIFT))

#define  ZX_ZIFR_ILLEGALCMD_MASK 0x4
#define  ZX_ZIFR_ILLEGALCMD_SHIFT 2
#define  ZX_ZIFR_ILLEGALCMD_MASK_SHIFT(reg) (((reg) & ZX_ZIFR_ILLEGALCMD_MASK) >> ZX_ZIFR_ILLEGALCMD_SHIFT)
#define  ZX_ZIFR_ILLEGALCMD_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIFR_ILLEGALCMD_MASK) | (((uint32_t)val) << ZX_ZIFR_ILLEGALCMD_SHIFT))

#define  ZX_ZIFR_DESC_MASK 0x2
#define  ZX_ZIFR_DESC_SHIFT 1
#define  ZX_ZIFR_DESC_MASK_SHIFT(reg) (((reg) & ZX_ZIFR_DESC_MASK) >> ZX_ZIFR_DESC_SHIFT)
#define  ZX_ZIFR_DESC_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIFR_DESC_MASK) | (((uint32_t)val) << ZX_ZIFR_DESC_SHIFT))

#define  ZX_ZIFR_OWN_MASK 0x1
#define  ZX_ZIFR_OWN_SHIFT 0
#define  ZX_ZIFR_OWN_MASK_SHIFT(reg) (((reg) & ZX_ZIFR_OWN_MASK) >> ZX_ZIFR_OWN_SHIFT)
#define  ZX_ZIFR_OWN_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIFR_OWN_MASK) | (((uint32_t)val) << ZX_ZIFR_OWN_SHIFT))

//====================================================================
//Register: Zx Descriptor Write Register (ZDWR)
/** \brief Write descriptor and start DMA*/
//====================================================================

#define  ZX_ZDWR_DESC_MASK 0xfffffff0
#define  ZX_ZDWR_DESC_SHIFT 4
#define  ZX_ZDWR_DESC_MASK_SHIFT(reg) (((reg) & ZX_ZDWR_DESC_MASK) >> ZX_ZDWR_DESC_SHIFT)
#define  ZX_ZDWR_DESC_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZDWR_DESC_MASK) | (((uint32_t)val) << ZX_ZDWR_DESC_SHIFT))

#define  ZX_ZDWR_RESERVED1_MASK 0xf
#define  ZX_ZDWR_RESERVED1_SHIFT 0
#define  ZX_ZDWR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZDWR_RESERVED1_MASK) >> ZX_ZDWR_RESERVED1_SHIFT)
#define  ZX_ZDWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZDWR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZDWR_RESERVED1_SHIFT))

//====================================================================
//Register: Zx Descriptor Read Register (ZDRR)
/** \brief Current descriptor location*/
//====================================================================

#define  ZX_ZDRR_DESC_MASK 0xffffffff
#define  ZX_ZDRR_DESC_SHIFT 0
#define  ZX_ZDRR_DESC_MASK_SHIFT(reg) (((reg) & ZX_ZDRR_DESC_MASK) >> ZX_ZDRR_DESC_SHIFT)
#define  ZX_ZDRR_DESC_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZDRR_DESC_MASK) | (((uint32_t)val) << ZX_ZDRR_DESC_SHIFT))

//====================================================================
//Register: Zx Line Length Register (ZLLR)
//====================================================================

#define  ZX_ZLLR_RESERVED1_MASK 0xffff0000
#define  ZX_ZLLR_RESERVED1_SHIFT 16
#define  ZX_ZLLR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZLLR_RESERVED1_MASK) >> ZX_ZLLR_RESERVED1_SHIFT)
#define  ZX_ZLLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZLLR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZLLR_RESERVED1_SHIFT))

#define  ZX_ZLLR_LINELENGTH_MASK 0xffff
#define  ZX_ZLLR_LINELENGTH_SHIFT 0
#define  ZX_ZLLR_LINELENGTH_MASK_SHIFT(reg) (((reg) & ZX_ZLLR_LINELENGTH_MASK) >> ZX_ZLLR_LINELENGTH_SHIFT)
#define  ZX_ZLLR_LINELENGTH_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZLLR_LINELENGTH_MASK) | (((uint32_t)val) << ZX_ZLLR_LINELENGTH_SHIFT))

//====================================================================
//Register: Zx Bytes Processed Register (ZBPR)
//====================================================================

#define  ZX_ZBPR_BYTESPROCESSED_MASK 0xffffffff
#define  ZX_ZBPR_BYTESPROCESSED_SHIFT 0
#define  ZX_ZBPR_BYTESPROCESSED_MASK_SHIFT(reg) (((reg) & ZX_ZBPR_BYTESPROCESSED_MASK) >> ZX_ZBPR_BYTESPROCESSED_SHIFT)
#define  ZX_ZBPR_BYTESPROCESSED_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZBPR_BYTESPROCESSED_MASK) | (((uint32_t)val) << ZX_ZBPR_BYTESPROCESSED_SHIFT))

//====================================================================
//Register: Zx Cmd Source Register (ZCSR)
//====================================================================

#define  ZX_ZCSR_SOURCE_MASK 0xffffffff
#define  ZX_ZCSR_SOURCE_SHIFT 0
#define  ZX_ZCSR_SOURCE_MASK_SHIFT(reg) (((reg) & ZX_ZCSR_SOURCE_MASK) >> ZX_ZCSR_SOURCE_SHIFT)
#define  ZX_ZCSR_SOURCE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCSR_SOURCE_MASK) | (((uint32_t)val) << ZX_ZCSR_SOURCE_SHIFT))

//====================================================================
//Register: Zx Cmd Bytes Register (ZCBR)
//====================================================================

#define  ZX_ZCBR_RESERVED1_MASK 0xff000000
#define  ZX_ZCBR_RESERVED1_SHIFT 24
#define  ZX_ZCBR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZCBR_RESERVED1_MASK) >> ZX_ZCBR_RESERVED1_SHIFT)
#define  ZX_ZCBR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCBR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZCBR_RESERVED1_SHIFT))

#define  ZX_ZCBR_BYTES_MASK 0xffffff
#define  ZX_ZCBR_BYTES_SHIFT 0
#define  ZX_ZCBR_BYTES_MASK_SHIFT(reg) (((reg) & ZX_ZCBR_BYTES_MASK) >> ZX_ZCBR_BYTES_SHIFT)
#define  ZX_ZCBR_BYTES_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZCBR_BYTES_MASK) | (((uint32_t)val) << ZX_ZCBR_BYTES_SHIFT))

//====================================================================
//Register: Zx Pix Source Register (ZPSR)
//====================================================================

#define  ZX_ZPSR_SOURCE_MASK 0xffffffff
#define  ZX_ZPSR_SOURCE_SHIFT 0
#define  ZX_ZPSR_SOURCE_MASK_SHIFT(reg) (((reg) & ZX_ZPSR_SOURCE_MASK) >> ZX_ZPSR_SOURCE_SHIFT)
#define  ZX_ZPSR_SOURCE_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZPSR_SOURCE_MASK) | (((uint32_t)val) << ZX_ZPSR_SOURCE_SHIFT))

//====================================================================
//Register: Zx Image Height Register (ZIHR)
//====================================================================

#define  ZX_ZIHR_RESERVED1_MASK 0xffff0000
#define  ZX_ZIHR_RESERVED1_SHIFT 16
#define  ZX_ZIHR_RESERVED1_MASK_SHIFT(reg) (((reg) & ZX_ZIHR_RESERVED1_MASK) >> ZX_ZIHR_RESERVED1_SHIFT)
#define  ZX_ZIHR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIHR_RESERVED1_MASK) | (((uint32_t)val) << ZX_ZIHR_RESERVED1_SHIFT))

#define  ZX_ZIHR_IMAGEHEIGHT_MASK 0xffff
#define  ZX_ZIHR_IMAGEHEIGHT_SHIFT 0
#define  ZX_ZIHR_IMAGEHEIGHT_MASK_SHIFT(reg) (((reg) & ZX_ZIHR_IMAGEHEIGHT_MASK) >> ZX_ZIHR_IMAGEHEIGHT_SHIFT)
#define  ZX_ZIHR_IMAGEHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~ZX_ZIHR_IMAGEHEIGHT_MASK) | (((uint32_t)val) << ZX_ZIHR_IMAGEHEIGHT_SHIFT))

#endif // ZX
