/** 
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * \brief Coupled to a compile time define HAVE_ZPSL3_SUPPORT in your build system.
 * If this is not defined then postscript is not in your system and the vote for
 * should I run postscript will always return NO.
 *
 */



#ifndef VOTER_API_H
#define VOTER_API_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef HAVE_ZPSL3_SUPPORT
    int PS_Syntaxer(char const *Ptr, char const *BufLim );
#   define PS_VOTER(begin, end)  PS_Syntaxer(begin, end)
#else
#   define PS_VOTER(a, b)  -1
#endif



#ifdef __cplusplus
}
#endif

#endif
