/*
 *
 * ============================================================================
 * Copyright (c) 2008-2009   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file checksum.c
 *
 * \brief This file contains the implementation of the checksum 
 * calculation function.
 *
 */
 
#include <stdint.h>
#include "checksum.h"

/* FUNCTION NAME: CalcChecksum */
 
/** 
 * \brief Checksum calculation routine used to verify downloaded code and data.
 * 
 * \param Address
 * 
 * \param Count
 * 
 * \retval int
 * 
 **/
uint32_t CalcChecksum( uint32_t* Address, uint32_t Count)
{
           /* Compute  Checksum for "count" bytes
            *         beginning at location "Address".
            */

	register long sum;
	uint32_t i;

	sum = 0;
	i = 0;

	while( (int32_t)Count > 1 )
	{
		/*  This is the inner loop */

		// Get the address, add in the count, make it a word address and read it, shift it right 
		// by either 0 or 16, save only the 16 rightmost bits, and add it in.
		//
		sum += (*((uint8_t *)((uintptr_t) Address + i)))<<8;
		sum += *((uint8_t *)((uintptr_t) Address + i + 1));
		Count -= 2;
		i += 2;
	}

           /*  Add left-over byte, if any */

    if( Count > 0 )
        sum += *((uint8_t *)((uintptr_t) Address + i));

           /*  Fold 32-bit sum to 16 bits */

    while(sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return(~sum);
}
