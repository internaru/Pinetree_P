/******************************************************************************
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <stddef.h>
#include <stdlib.h>
#include "lassert.h"

extern "C"
{
    void *__dso_handle = NULL;
}

namespace __gnu_cxx {
	/* tiny verbose terminate handler */
	void __verbose_terminate_handler() { ASSERT(0);}
}

