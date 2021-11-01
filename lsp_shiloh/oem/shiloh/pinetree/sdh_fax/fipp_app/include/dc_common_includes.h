#ifndef __DC_COMMON_INCLUDES_H__
#define __DC_COMMON_INCLUDES_H__

#include "dc_type.h"
#include "string.h"
#include "mosi.h"

#if defined(RTOS_LINUX)
#include "assert.h"
#include "ospl.h"
#elif defined(RTOS_THREADX)
#include "os.h"
#include "dc_return_codes.h"
#include "tx_api.h"
#endif

#include "dc_fwcxdxx_lnx_api.h"
#include "configmo.h"
#include "cxshell.h"

#include "lassert.h"

#endif //__DC_COMMON_INCLUDES_H__
