/* 
 *
 * ============================================================================
 * Copyright (c) 2009   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */


#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "lassert.h"
#include "regutils.h"
#ifdef __linux__
#include "posix_ostools.h"
#else
#include "cpu_api.h"
#endif
#include "error_types.h"




/**
    \brief Poll a function until it returns the requested value, or until time runs out

    \warning This only works for functions that take no parameters

    \param pollFunc() pointer to the function to execute and poll on return value

    \param result - value to compare against the masked function return value

    \param mask - value to AND with the returned value from the function

    \param pollInterval - interval to wait before the next poll

    \param count - how many times to poll

    \param equals
    if true (EQUALS),  poll until result == masked function return value
    if false (NOT_EQUAL), poll until result != masked function return value

    \return error_type_t
    \retval OK 0
    \retval FAIL -1 Timed out or Counted out

**/
error_type_t NVM_PollFunc(uint8_t pollFunc(), uint32_t result, uint32_t mask, uint32_t pollInterval, uint32_t count, uint32_t equals)
{
   uint32_t i, retVal, found;
   error_type_t status;

   status = OK;
   found = false;

   for (i=0;i<count;i++)
   {
      retVal = pollFunc();
      if (equals == EQUALS)
      {
         if ((retVal & mask) == result)
         {
            found = true;
            break;
         }
      }
      else
      {
         if ((retVal & mask) != result)
         {
            found = true;
            break;
         }
      }
      if (pollInterval != 0)
#ifdef __linux__
         posix_sleep_ticks(pollInterval);
#else
         cpu_spin_delay(pollInterval);
#endif
   }
   if (found == false)
   {
      status = FAIL;
   }
   return (status);
}
