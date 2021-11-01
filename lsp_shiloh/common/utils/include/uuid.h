/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_UUID_H
#define INC_UUID_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UUID_STRING_LENGTH	37
#define UUID_BINARY_LENGTH	16


/**
 * @brief Get the device uuid bytes
 *
 * This routine gets the uuid as bytes 
 *
 * @param binary_uuid	A pointer to the user allocated memory
 * @param len		A pointer to the user allocated memory length
 *			The length is updated with the returned length.
 * @return OK or FAIL
 */
error_type_t get_device_uuid_binary(char *binary_uuid, int *len);


/**
 * @brief Get the device uuid string
 *
 * This routine gets the uuid as a string in the format of
 * 8-4-4-4-12 (XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX + NULL)
 *
 * @param uuid	A pointer to the user allocated memory
 * @param len	User allocated memory length
 *
 * @return OK or FAIL
 */
error_type_t get_device_uuid_string(char * uuid, int len);


#ifdef __cplusplus
}
#endif

#endif /* INC_UUID_H */
