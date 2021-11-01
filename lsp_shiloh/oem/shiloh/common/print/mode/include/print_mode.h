
/*
 * ============================================================================
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef PRINT_MODE_H
#define PRINT_MODE_H

#include "print_job_types.h"

/**
 *
 *  \file print_mode.h
 *
 *  \brief OEM specific definition of the print_mode_t typedef.
 *  
 */

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */

// Define specific and unique image pipe configurations. These values are used
// to lookup tables that specify hardware configurations for the specified mode.
#define IPC_UNUSED                    0
#define IPC_PRINT_COLOR24_1BPP       10
#define IPC_PRINT_DRAFT             100
#define IPC_PRINT_NORMAL            101
#define IPC_PRINT_PHOTO             102
#define IPC_PRINT_BLOCK_TEST        103
#define IPC_PRINT_ZX_ONLY           104
#define IPC_PRINT_MONO8_1BPP        105
#define IPC_PRINT_MONO8_2BPP        106
#define IPC_PRINT_MONO8_1BPP_1PLANE 107
#define IPC_PRINT_MONO8_2BPP_1PLANE 108
#define IPC_PRINT_MONO8_4BPP_1PLANE 109
#define IPC_PRINT_MONO8_SMOOTH      110  // laser 8bit to 1 bit mono
#define IPC_PRINT_MONO8_DETAIL      111  // laser 8bit to 1 bit mono
#define IPC_COPY_MONO8_DETAIL       112  // Error diffusion mono laser 8bit to 1 bit  
#define IPC_PRINT_ADDITIVE_GRAY8_SMOOTH      113  // laser 8bit to 1 bit mono
#define IPC_PRINT_ADDITIVE_GRAY8_DETAIL      114  // laser 8bit to 1 bit mono
#define IPC_PRINT_RGB24_1BPP        115 // laser mono output color input 
#define IPC_COPY_MONO8_FILM_DETAIL  116  // add.lsh.Dither mono laser 8bit to 1 bit  
#define IPC_LAST                    116  // mod.lsh.the last IPC for searches (115 -> 116)


// Define the available print modes, by name, for this product.
typedef enum
{
    COPY_TEXT,
    COPY_PHOTO,
    COPY_MIXED,
    COPY_DRAFT,
    PRINT_PHOTO,
    PRINT_NORMAL,
    PRINT_FAX,
    PRINT_DRAFT,
} print_mode_name_t;

struct print_mode_s
{
    print_mode_name_t name;

    bool image_input_info_init;
    image_info_t image_input_info;  // Used to record input image info to ensure its integrity.
    image_info_t image_output_info; // Output image info

    uint32_t num_color_planes;
    uint32_t image_pipe_configuration;
    bool color;
    uint32_t halftone;
};



#endif


