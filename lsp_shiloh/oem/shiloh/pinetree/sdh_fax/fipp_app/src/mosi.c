#include "dc_common_includes.h"


int OSC_Allocate_Zero_Memory(OSC_MEMORY_POOL *pool, void **memptr, UINT32 size, int suspend)
{
	int iErr = OSC_SUCCESS;

	*memptr = malloc(size);

	if (*memptr != NULL)
	{
		memset(*memptr, 0, size);
	}
	else
	{
		iErr = 1;
	}

	return iErr;
}


void OSC_Deallocate_Memory(void *memptr)
{
	free(memptr);
}

