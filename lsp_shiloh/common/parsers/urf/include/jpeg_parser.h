#ifndef JPEG_PARSER_H
#define JPEG_PARSER_H
/******************************************************************************
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "print_job_api.h"

typedef struct jpeg_header_s
{
    uint8_t Duplex;      // 1=print duplex
    uint8_t NumCopies;
    uint8_t Orientation; // 0=auto, 1=landscape, 2=portrait
    uint8_t ScaleToFit;  // 1=scale image to fit media
    uint32_t ImageResolution; 
    uint32_t PageSize[2]; // override media size, values in inches*100
    uint8_t PrintQuality; // 0=normal, 1=better, 2=best, 3=draft
    uint8_t ColorSpace;   // 0=BW, 1=RGB, 2=auto
    uint8_t Reserved_8[2];
    uint32_t Reserved_32[3];
} jpeg_header_t;

#endif //  JPEG_PARSER_H
