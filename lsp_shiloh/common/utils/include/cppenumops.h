/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file cppenumops.h
 *
 * \brief This file implements a macro that creates overloaded pre and post 
 * increment and decrement operators for enumerated types.
 *
 * These operators behave on enumerate types the same way that they would
 * behave on integer types.  They perform a sequential increment or decrement.
 * They will not handle non-sequential enum ordering correctly.
 *
 * Note that the signature (ENUMTYPE &val, int) defines the post inc/dec
 * operator, and the signature (ENUMTYPE &val) defines the pre inc/dec operator.
 *
 */

#ifndef CPPENUMOPS_H
#define CPPENUMOPS_H

#ifdef __cplusplus
#define DECLARE_ENUM_OPERATORS(ENUMTYPE) \
inline ENUMTYPE operator++(ENUMTYPE &val, int) \
{ \
    ENUMTYPE ret = val; \
    val = ENUMTYPE(val + 1); \
    return ret; \
} \
\
inline ENUMTYPE operator++(ENUMTYPE &val) \
{ \
    return val = ENUMTYPE(val + 1); \
} \
\
inline ENUMTYPE operator--(ENUMTYPE &val, int) \
{ \
    ENUMTYPE ret = val; \
    val = ENUMTYPE(val - 1); \
    return ret; \
} \
\
inline ENUMTYPE operator--(ENUMTYPE &val) \
{ \
    return val = ENUMTYPE(val - 1); \
}
#else
#define DECLARE_ENUM_OPERATORS(ENUMTYPE)
#endif // __cplusplus

#endif // CPPENUMOPS_H
