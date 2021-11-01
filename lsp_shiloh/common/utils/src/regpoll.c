/* 
 *
 * ============================================================================
 * Copyright (c) 2009   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */


#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "lassert.h"
#ifdef __linux__
#include "posix_ostools.h"
#else
#include "cpu_api.h"
#endif
#include "error_types.h"

/**
    \brief poll a register until it has the requested value, or until time runs out

    \param[in] pollReg - address of the register to poll

    \param[in] mask - value to AND with the returned value from the register

    \param[in] result - value to compare against the masked register value

    \param[in] pollInterval - interval to wait before the next poll

    \param[in] count - how many times to poll

    \param[in] equals - if true,  poll until result == masked register value
                        if false, poll until result != masked register value

    \return error_type_t
    \retval OK 0
    \retval FAIL -1

**/
error_type_t NVM_PollReg(volatile uint32_t *pollReg, uint32_t result, uint32_t mask, uint32_t pollInterval, uint32_t count, uint32_t equals)
{
   error_type_t status;
   uint32_t i, regVal, found;

   status = OK;
   found = false;

   for (i=0;i<count;i++)
   {
      regVal = *pollReg;

      if (equals == true)
      {
         if ((regVal & mask) == result)
         {
            found = true;
            break;
         }
      }
      else
      {
         if ((regVal & mask) != result)
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

