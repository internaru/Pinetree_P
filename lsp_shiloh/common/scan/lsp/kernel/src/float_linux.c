/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *  \file float.c
 *
 *  \brief stubs to allow scantask linux kernel module to build
 *
 * Scantask uses some floats. Until all float are removed or scantask removed
 * from kernel, these stub functions allow us to run certain code paths. 
 *
 * davep Jun-2012
 */

#include <stdint.h>

#include <linux/module.h> 
#include <linux/kernel.h> 

uint32_t __aeabi_ui2d( uint32_t n )
{
    BUG();
    return 0;
}

uint32_t __aeabi_unwind_cpp_pr0( uint32_t n )
{
    BUG();
    return 0;
}

uint32_t __aeabi_unwind_cpp_pr1( uint32_t n )
{
    BUG();
    return 0;
}

uint32_t __aeabi_uldivmod( uint32_t n )
{
    BUG();
    return 0;
}

