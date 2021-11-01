/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file uuid.c
 *
 *  \brief UUID utility functions
 *
 *  Please see uuid.h for interface/usage documentation.
 *
 **/
 
#include "error_types.h"
#include "string.h"
#include "platform_api.h"
#include "utils.h"
#include "dprintf.h"
#include "uuid.h"
#ifdef HAVE_ETHERNET
#include "net_iface_vars.h"
#endif
#ifdef HAVE_WIRELESS
#include "encrypt.h"
#endif

#define UUID_SECTION_BOUNDRY	10

/**
 * @brief Get the device uuid bytes
 *
 * This routine gets the uuid as bytes 
 *
 */
error_type_t get_device_uuid_binary(char *binary_uuid, int *len) 
{
    error_type_t  status = FAIL;
    char          tmp_uuid[UUID_BINARY_LENGTH]={0};
    char         *serial = NULL;
    uint32_t      serial_len = 0;

    if ((binary_uuid != NULL) && (*len > 0))
    {
	    // Generate the UUID by hashing a bunch of data together that
	    // will be unique for each device.
	    memset(tmp_uuid, 0, sizeof(tmp_uuid));

	    platvars_get_formater_ser_num(&serial);
            serial_len = strlen(serial); 

	    if (serial_len > UUID_SECTION_BOUNDRY)
            memcpy(tmp_uuid, serial + serial_len - UUID_SECTION_BOUNDRY, UUID_SECTION_BOUNDRY);
	    else
            memcpy(tmp_uuid, serial, serial_len);

	#ifdef WIRED_NETWORK
	    // Use wired MAC address
	    net_iface_get_var_phys_addr((uint8_t *) &tmp_uuid[UUID_SECTION_BOUNDRY], NET_IFACE_PHYS_ADDR_SIZE );
	#else
	    // Use product serial number when MAC is unavailable
	    platvars_get_prod_serialnum(&serial);
        serial_len = strlen(serial);

	    if (serial_len > (UUID_BINARY_LENGTH - UUID_SECTION_BOUNDRY))
            memcpy(&tmp_uuid[UUID_SECTION_BOUNDRY], serial + serial_len - (UUID_BINARY_LENGTH - UUID_SECTION_BOUNDRY), (UUID_BINARY_LENGTH - UUID_SECTION_BOUNDRY));
	    else
            memcpy(&tmp_uuid[UUID_SECTION_BOUNDRY], serial, serial_len);
	#endif

	#ifdef HAVE_WIRELESS
	    unsigned char hashDigest[SHA256_DIGEST_SIZE];
	    Mrv_SHA256((unsigned char *)tmp_uuid, strlen(tmp_uuid), hashDigest);
	    memcpy(tmp_uuid, hashDigest, UUID_BINARY_LENGTH);
	#endif

	    if (*len > UUID_BINARY_LENGTH)
            *len = UUID_BINARY_LENGTH;

	    memcpy(binary_uuid, tmp_uuid, *len);

	    status = OK;
    }
    return(status);
}

/**
 * @brief Get the device uuid string
 *
 * This routine gets the uuid as a string in the format
 *    XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX + NULL
 *
 */
error_type_t get_device_uuid_string(char * uuid, int len)
{
    error_type_t status = FAIL;
    
    if ((uuid != NULL) && (len > 0))
    {
	char binary_uuid[UUID_BINARY_LENGTH];
	char string_uuid[UUID_STRING_LENGTH];
	int binary_len = UUID_BINARY_LENGTH;

	get_device_uuid_binary(binary_uuid, &binary_len);

	// Format to 8-4-4-4-12  (XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX + NULL)
	minSprintf( string_uuid, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", 
		    binary_uuid[0], binary_uuid[1], binary_uuid[2], binary_uuid[3],
		    binary_uuid[4], binary_uuid[5], binary_uuid[6], binary_uuid[7],
		    binary_uuid[8], binary_uuid[9], binary_uuid[10], binary_uuid[11],
		    binary_uuid[12], binary_uuid[13], binary_uuid[14], binary_uuid[15]);

	if (len > UUID_STRING_LENGTH)
		len = UUID_STRING_LENGTH;

	memcpy(uuid, string_uuid, len);

        status = OK;
    }
    
    return (status);
}


