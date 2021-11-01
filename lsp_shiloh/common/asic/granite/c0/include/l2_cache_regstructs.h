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
 * \file l2_cache_regstructs.h

 *
 *************************************************************************
 */

#ifndef _L2_CACHE_REGSTRUCTS_H_
#define _L2_CACHE_REGSTRUCTS_H_

#include <stdint.h>

typedef struct
{
    uint32_t    l2_cache_id;        ///< addr 0x000
    uint32_t    l2_cache_type;      ///< 0x004cache type
    uint32_t    l2_reserved[(0x100 - 8)/4];          ///>
    uint32_t    l2_cache_control;   ///< 0x100
    uint32_t    l2_aux_control;     //< 0x104
    uint32_t    l2_reserved1[(0x200 - 0x108)/4];
    uint32_t    l2_event_counter_control;   ///< 0x200
    uint32_t    l2_event_count1_config;     ///< 0x204
    uint32_t    l2_event_count0_config;     ///< 0x208
    uint32_t    l2_event_count1_val;        ///< 0x20c
    uint32_t    l2_event_count0_val;        ///< 0x210
    uint32_t    l2_interrupt_mask;          ///< 0x214
    uint32_t    l2_masked_int_status;       ///< 0x218
    uint32_t    l2_raw_int_status;          ///< 0x21c
    uint32_t    l2_int_clear;               ///< 0x220
    uint32_t    l2_event_cntr2_config;      ///< 0x224
    uint32_t    l2_event_cntr2_val;         ///< 0x228
    uint32_t    l2_reserved2[(0x730-0x22c)/4]; ///<
    uint32_t    l2_cache_sync;              ///< 0x730
    uint32_t    l2_reserved3[(0x770-0x734)/4];   
    uint32_t    l2_invalidate_line;         ///< 0x770
    uint32_t    l2_reserved4[(0x77c - 0x774)/4];
    uint32_t    l2_invalidate_way;          ///< 0x77c
    uint32_t    l2_invalidate_all;          ///< 0x780
    uint32_t    l2_clean_all;               ///< 0x784
    uint32_t    l2_reserved5[(0x7b0 - 0x788)/4];
    uint32_t    l2_clean_line_pa;           ///< 0x7b0
    uint32_t    l2_reserved6;               
    uint32_t    l2_clean_line_idx_way;      ///< 0x7b8
    uint32_t    l2_clean_by_way;            ///<0x7bc
    uint32_t    l2_reserved7[(0x7f0-0x7c0)/4]; 
    uint32_t    l2_clean_inval_line_pa;     ///< 0x7f0
    uint32_t    l2_reserved8;   
    uint32_t    l2_clean_inval_line_idx_way;
    uint32_t    l2_clean_inval_way;         ///< 0x7fc
    uint32_t    l2_content0;                ///< 0x800
    uint32_t    l2_content1;                ///> 0x804
    uint32_t    l2_tag_read;                ///< 0x808
    uint32_t    l2_data_read;               ///< 0x80c
    uint32_t    l2_reserved9[(0x900 - 0x810)/4];
    uint32_t    l2_lockdown_way;            ///< 0x900
    uint32_t    l2_reserved10[(0xc00 - 0x904)/4];
    uint32_t    l2_address_filter_start;    ///< 0xc00
    uint32_t    l2_address_filter_end;      ///< 0xc04
    uint32_t    l2_reserved11[2];
    uint32_t    l2_address_filter_start1;   ///< 0xc10
    uint32_t    l2_address_filter_end1;     ///< 0xc14
    uint32_t    l2_reserved12[2];
    uint32_t    l2_address_filter_start2;   ///< 0xc20
    uint32_t    l2_address_filter_end2;     ///< 0xc24
    uint32_t    l2_reserved13[(0xf40 - 0xc28)/4];
    uint32_t    l2_debug_ctrl;              ///< 0xf40

}l2_cache_reg_t;

#endif

