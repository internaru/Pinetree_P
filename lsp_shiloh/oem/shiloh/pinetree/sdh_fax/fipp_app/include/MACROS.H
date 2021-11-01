/*
 *  $RCSfile: MACROS.H,v $
 * $Revision: 1.4 $
 *   $Author: miurat $
 *     $Date: 2009/09/25 18:45:38 $
 */
/************************************************************************
 *
 *      MACROS.H
 *
 *  Copyright 1998 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef __MACROS_H__
#define __MACROS_H__

/* Bit manipulation macros */
//#define  BITSET(a,b)    		(*((VUINT8 *)a) & b)
//#define  BITCLEAR(a,b)    		(!(*((VUINT8 *)a) & b))
//#define  SETBIT(a,b)    		(*((VUINT8 *)a) |= b)
//#define  CLEARBIT(a,b)    		(*((VUINT8 *)a) &= ~b)
#define  BITSET(a,b)    		(*((UINT8 *)a) & b)
#define  BITCLEAR(a,b)    		(!(*((UINT8 *)a) & b))
#define  SETBIT(a,b)    		(*((UINT8 *)a) |= b)
#define  CLEARBIT(a,b)    		(*((UINT8 *)a) &= ~b)

#define  BITSET16(a,b)    		(*((UINT16 *)a) & b)
#define  BITCLEAR16(a,b)    		(!(*((UINT16 *)a) & b))
#define  SETBIT16(a,b)    		(*((UINT16 *)a) |= b)
#define  CLEARBIT16(a,b)    		(*((UINT16 *)a) &= ~b)

#ifdef MODEM_SSF336

#define VSP_BIT(a, b)           (((a&0xFF)<<24)|(b&0xFF))
#define VSP_BIT_FROM_IDX(a, b)  (((a&0xFF)<<24)|(b&0xFF))
#define VSP_BITMASK(vsp_bit)    (UINT8)(vsp_bit&0xFF)
#define VSP_INDEX(vsp_bit)      (UINT8)((vsp_bit>>24)&0xFF)
#define VSP_ADDRESS(vsp_bit)    (VSP_INDEX(vsp_bit))

#define VSP_ISBITSET(bit)       ModemIoBitSet(VSP_INDEX(bit), VSP_BITMASK(bit))
#define VSP_ISBITCLEAR(bit)     ModemIoBitClear(VSP_INDEX(bit), VSP_BITMASK(bit))
#define VSP_SETBIT(bit)         ModemIoSetBit(VSP_INDEX(bit), VSP_BITMASK(bit))
#define VSP_CLEARBIT(bit)       ModemIoClearBit(VSP_INDEX(bit), VSP_BITMASK(bit))
#define VSP_MAKEBIT(bit, cond)  (cond) ? VSP_SETBIT(bit) : VSP_CLEARBIT(bit)

#else
#ifdef SDSP_FAX
#define ADDRESS_TO_INDEX(a)     ((UINT32)(a))
#define INDEX_TO_ADDRESS(a)     ((UINT32)(a))
#else
#define ADDRESS_TO_INDEX(a)     ((((UINT32)a) - (UINT32)MODEM_BASE) >> MODEM_ALIGN_TYPE)
#define INDEX_TO_ADDRESS(a)     ((MODEM_WORD *)(MODEM_BASE+MALIGN((UINT32)a)))
#endif

#define VSP_BIT(a, b)           (((ADDRESS_TO_INDEX(a)&0xFF)<<24)|(b&0xFF))
#define VSP_BIT_FROM_IDX(a, b)  (((a&0xFF)<<24)|(b&0xFF))
#define VSP_BITMASK(vsp_bit)    (UINT8)(vsp_bit&0xFF)
#define VSP_INDEX(vsp_bit)      (UINT8)((vsp_bit>>24)&0xFF)
#define VSP_ADDRESS(vsp_bit)    (INDEX_TO_ADDRESS(VSP_INDEX(vsp_bit)))

#define VSP_ISBITSET(bit)       VspIsBitSet(VSP_INDEX(bit), VSP_BITMASK(bit))
#define VSP_ISBITCLEAR(bit)     VspIsBitClear(VSP_INDEX(bit), VSP_BITMASK(bit))
#define VSP_SETBIT(bit)         VspSetBit(VSP_INDEX(bit), VSP_BITMASK(bit))
#define VSP_CLEARBIT(bit)       VspClearBit(VSP_INDEX(bit), VSP_BITMASK(bit))
#define VSP_MAKEBIT(bit, cond)  (cond) ? VSP_SETBIT(bit) : VSP_CLEARBIT(bit)
#endif // MODEM_SSF336

// for modem scratchpad manipulation
#ifdef SOFTWARE_FAX
#define MDM_WRITE(a,b)          VspWriteByte(ADDRESS_TO_INDEX(a),b)
#define MDM_READ(a)             VspReadByte(ADDRESS_TO_INDEX(a))
#define MDM_WRITEW(a,b)         VspWriteWord(ADDRESS_TO_INDEX(a),b)
#define MDM_READW(a,b)			(b = VspReadWord(ADDRESS_TO_INDEX(a)))

#define  MDM_BITSET(a,b)        VspIsBitSet(ADDRESS_TO_INDEX(a),b)
#define  MDM_BITCLEAR(a,b)      VspIsBitClear(ADDRESS_TO_INDEX(a),b)
#define  MDM_SETBIT(a,b)        VspSetBit(ADDRESS_TO_INDEX(a),b)
#define  MDM_CLEARBIT(a,b)      VspClearBit(ADDRESS_TO_INDEX(a),b)
#define  MDM_BIT(a,b,cnd)       (cnd ? VspSetBit(ADDRESS_TO_INDEX(a),b) : VspClearBit(ADDRESS_TO_INDEX(a),b))

#define VSP_WRITE(a,b)          MDM_WRITE(a,b)
#define VSP_READ(a)             MDM_READ(a)
#define VSP_WRITEW(a,b)         MDM_WRITEW(a,b)
#define VSP_READW(a)			VspReadWord(ADDRESS_TO_INDEX(a))

#define VSP_SAVE_EYEXY(x, y)    VspSaveEyeXY(x, y)
#define VSP_WAIT_FOR_EVENT(event, state, timeout)	 VspWaitForEvent(event, state, timeout)

#else // !SOFTWARE_FAX

#ifdef MODEM_SSF336
 
#define MDM_WRITE(a,b)          ModemIoWriteByte(a,b)
#define MDM_READ(a)             ModemIoReadByte(a)

#define  MDM_BITSET(a,b)        ModemIoBitSet(a,b)
#define  MDM_BITCLEAR(a,b)      ModemIoBitClear(a,b)
#define  MDM_SETBIT(a,b)        ModemIoSetBit(a,b)
#define  MDM_CLEARBIT(a,b)      ModemIoClearBit(a,b)

#else

#define	MDM_WRITE(a,b)			(*((VUINT8 *)(a)) = b)
#define	MDM_READ(a)				(*((VUINT8 *)(a)))
#define	MDM_WRITEW(a,b)			(*((VUINT8 *)(a)) = b);(*((VUINT8 *)(a+MALIGN(1))) = b)		//(*((VUINT16 *)a) = b)
#define	MDM_READW(a,b)			(b = *((VUINT8 *)(a)));(b |= *((VUINT8 *)(a+MALIGN(1)))<<8)	//(b = *((VUINT16 *)a))

#define  MDM_BITSET(a,b)    	(*((VUINT8 *)(a)) & b)
#define  MDM_BITCLEAR(a,b)    	(!(*((VUINT8 *)(a)) & b))
#define  MDM_SETBIT(a,b)    	(*((VUINT8 *)(a)) |= b)
#define  MDM_CLEARBIT(a,b)    	(*((VUINT8 *)(a)) &= ~b)

#endif

//#tm for CID
#define VSP_WRITE(a,b)          MDM_WRITE(a,b)
#define VSP_READ(a)             MDM_READ(a)
#define VSP_WRITEW(a,b)         MDM_WRITEW(a,b)
#define VSP_READW(a)            VspReadWord(ADDRESS_TO_INDEX(a))

#endif // !SOFTWARE_FAX

/* Byte/Word packing and cracking macros */
#if (ENDIAN==ENDIAN_LITTLE)
#define GET_HI_BYTE(a)	 		( *((UINT8 *)&a + 1) )
#define GET_LO_BYTE(a)	 		( *((UINT8 *)&a) )
#define GET_HI_WORD(a)	 		( *((UINT16 *)&a + 1) )
#define GET_LO_WORD(a)	 		( *((UINT16 *)&a) )
#define PUT_HI_BYTE(a,msb)		( *((UINT8 *)&a + 1) = msb )
#define PUT_LO_BYTE(a,lsb)		( *((UINT8 *)&a) = lsb )
#define PUT_HI_WORD(a,msw)		( *((UINT16 *)&a + 1) = msw )
#define PUT_LO_WORD(a,lsw)		( *((UINT16 *)&a) = lsw )
#else
#define GET_HI_BYTE(a)	 		( *((UINT8 *)&a) )
#define GET_LO_BYTE(a)	 		( *((UINT8 *)&a + 1) )
#define GET_HI_WORD(a)	 		( *((UINT16 *)&a) )
#define GET_LO_WORD(a)	 		( *((UINT16 *)&a + 1) )
#define PUT_HI_BYTE(a,msb)		( *((UINT8 *)&a) = msb )
#define PUT_LO_BYTE(a,lsb)		( *((UINT8 *)&a + 1) = lsb )
#define PUT_HI_WORD(a,msw)		( *((UINT16 *)&a) = msw )
#define PUT_LO_WORD(a,lsw)		( *((UINT16 *)&a + 1) = lsw )
#endif

#define LIT_ENDIAN(bit) \
		({ 	OSC_DISABLE_IRQ(); \
			CLEARBIT16(DMAENDIAN_IO,bit); \
			OSC_ENABLE_IRQ(); })

#endif // __MACROS_H__
