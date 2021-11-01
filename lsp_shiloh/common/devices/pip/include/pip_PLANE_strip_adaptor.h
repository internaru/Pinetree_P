/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pip_PLANE_strip_adaptor.h
 *
 * \brief adaptor between the generic strip_adaptor and the specific PLANED implementation of a strip.
 *  
 */

#ifndef PIP_PLANE_strip_adaptor_H_INCLUDED
#define PIP_PLANE_strip_adaptor_H_INCLUDED

#include "pip_strip_adaptor.h"
#include "print_job_types.h"

struct pip_PLANE_strip_adaptor 
{
    pip_strip_adaptor_BaseClassMembers;  //< inherit base class methods/variables/
    
    PLANED *strip;         //< my actual strip image data.

    uint32_t destination;  //< hold this for my end user to msg my output.
};


/// user constructs the derived class base not the base class
struct pip_strip_adaptor *construct_pip_PLANE_strip_adaptor( struct pip_PLANE_strip_adaptor *reuse_if_not_null );

#endif // PIP_PLANE_strip_adaptor_H_INCLUDED
