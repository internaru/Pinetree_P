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
 * \file checksum.h
 *
 * \brief This file contains the checksum API.
 *
 */

#ifndef CHECKSUM_H_
#define CHECKSUM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


uint32_t CalcChecksum( uint32_t *Address, uint32_t Count);

#ifdef __cplusplus
}
#endif
#endif /*CHECKSUM_H_*/
