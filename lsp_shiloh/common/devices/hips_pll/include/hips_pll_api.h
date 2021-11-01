/* 
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */


 #ifndef HIPS_PLL_API_H
 #define HIPS_PLL_API_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hips_pll_device_s hips_pll_device_t;
/**
 * \brief Initialize the hips pll
 * This starts the hips pll and sets up the driver.
 */
 void hips_pll_init(void);

 /**
  * \brief Open a given hips pll channel
  * The hips pll is mapped to a number of different blocks.  The 
  * definition of the blocks depends on the asic.  This will enable a
  * given block to have the hips pll sent to it.
  * \param handle The address of a pointer for the returned handle
  * \param[in] The channel to open
  * \retval OK everything worked.  If a problem this will assert.
  */
 error_type_t hips_pll_open(hips_pll_device_t **handle, uint32_t channel);

 /**
  * \brief Close a given hips pll channel
  * The hips pll is mapped to a number of different blocks.  This close turns
  * off the pll clock to the given block and closes the channel.  If there is
  * a problem this will assert.
  * \param[in] the handle that was returned by the open above.
  * \returns error_type_t
  * \retval OK everything is ok.  
  */
 error_type_t hips_pll_close(hips_pll_device_t *handle);

 #endif
