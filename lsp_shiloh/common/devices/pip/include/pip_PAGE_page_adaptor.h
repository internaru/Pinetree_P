/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pip_PAGE_page_adaptor.h
 *
 * \brief base class adaptor to a PAGE page.
 *  
 */

#ifndef PIP_PAGE_page_adaptor_H_INCLUDED
#define PIP_PAGE_page_adaptor_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include "pip_page_adaptor.h"
#include "print_job_types.h"

struct pip_PAGE_page_adaptor 
{
    pip_page_adaptor_BaseClassMembers;
    
    uint32_t destination;
    uint32_t pip_mode;
    bool first_strip;
    PAGE_DATA *page;
};


struct pip_page_adaptor *construct_pip_PAGE_page_adaptor( 
    struct pip_PAGE_page_adaptor *reuse_if_not_null );

#endif // pip_PAGE_strip_adaptor_H_INCLUDED
