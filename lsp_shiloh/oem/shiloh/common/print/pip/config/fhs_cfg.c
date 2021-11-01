/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the FHS block config.
  * 
 **/


#include "fhs_cfg.h"

fhs_cfg_t fhs_cfg = 
{
    .bypass = 0,
    .stallonerror = 0,
    .imageinwidth = 25,
    .imageoutwidth = 28,
    .datatypechanid = 0
};

fhs_cfg_t fhs_testpip2_cfg = 
{
    .bypass = 1,
    .stallonerror = 0,
    .imageinwidth = 128,
    .imageoutwidth = 128,
    .datatypechanid = 0
};

fhs_cfg_t fhs_testpip2_a_cfg = 
{
    .bypass = 1,
    .stallonerror = 0,
    .imageinwidth = 128,
    .imageoutwidth = 128,
    .datatypechanid = 0
};

fhs_cfg_t fhs_128pad_cfg = 
{
    .bypass = 0,
    .stallonerror = 0,
    .imageinwidth = 128,
    .imageoutwidth = 128,
    .datatypechanid = 0
};

fhs_cfg_t fhs_generic_cfg = 
{
    .bypass = 0,
    .stallonerror = 0,
    .imageinwidth = 128,
    .imageoutwidth = 128,
    .datatypechanid = 0
};
