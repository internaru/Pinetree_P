/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file uap_api.h
 * 
 * @brief Wireless UAP API
 * 
 * This API exposes routines for the UAP
 */

#ifndef UAP_API_H
#define UAP_API_H

/** Function Prototype Declaration */

/**
 * @brief Create UAP Configuration
 *
 * @param config    Pointer to an allocated buffer with 
 *                  configuration
 *
 * @return OK or FAIL.
 */
error_type_t
uap_create_config(void **config);

/**
 * @brief Delete UAP Configuration
 *
 * @param config    Pointer to a previously allocated buffer 
 *                  with configuration from
 *                  uap_create_config()
 *
 * @return OK or FAIL.
 */
error_type_t
uap_delete_config(void **config);

/**
 * @brief Send BSS Control Ioctl
 *
 * @param dev_name  Device Name 
 * @param cmd       Command 
 *
 * @return OK or FAIL.
 */
error_type_t
send_bss_ctl_ioctl(char * dev_name, int cmd);

/**
 * @brief Send BSS Configuration
 *
 * @param dev_name      Device Name 
 * @param p_config      BSS Configuration
 * @param p_config_ds   Datastore Configuration 
 *
 * @return OK or FAIL.
 */
error_type_t
send_bss_config(char * dev_name, void * p_config, void * p_config_ds);

/**
 * @brief BSS Configure
 *
 * @param dev_name      Device Name 
 * @param p_config_ds   Datastore Configuration 
 *
 * @return OK or FAIL.
 */
error_type_t
bss_configure(char * dev_name, void * p_config_ds);

#endif //UAP_API_H
