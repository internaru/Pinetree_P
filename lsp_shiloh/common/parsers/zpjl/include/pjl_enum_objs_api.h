/*
 * =========================================================
 * (c) Copyright 2009-2010 Marvell International Ltd. 
 *
 *               Marvell Confidential
 * ==========================================================
 */
/**
 * \brief This api is used to make mapping from a string value to a number
 * and back.
 *
 * For example if you have the strings OFF/ON/AUTO mapped to 0,1,2 this api
 * will allow you to put AUTO in and read out 2 or put in 1 and get out ON,
 * so it does the mapping from string to number and back.  Used in pjl, front
 * panel etc.
 */

#ifndef _PJL_ENUM_OBJS_API_H
#define _PJL_ENUM_OBJS_API_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include "zPJL.h"
#include "error_types.h"


/*
    The first section here defines the specifics of an enum object that is used
    by the code to map from a string to a number for storing.  So to use this
    the using program will write in the value that it has, for example "ON" and
    then after doing the set, does the get on the value to get the binary value
    to store in the oid layer.
*/

typedef struct pjl_object_s pjl_object_t;
/**
 * \brief Get numeric value out of the object.
 * \param[in] obj_addr Pointer to the object itself.
 * \return int32_t
 * \retval the numeric value from the object
 */

int32_t pjl_get_val(pjl_object_t *obj_addr);

/**
 * \brief Get the string equivalent of the numeric value in the object
 * \param[in] obj_addr Pointer to the enum object itself.
 * \returns char *
 * \retval The string value, NULL is nothing set, allocated must be freed.
 */
char *pjl_get_string_val(pjl_object_t *obj_addr);

/**
 * \brief Get a localized string that matches the get_string_val but translated.
 * \param[in] obj_addr Pointer to the enum object itself.
 * \returns char *
 * \retval A pointer to a translated string.  Allocated, you must free.
 */
char *get_localized_string_t(pjl_object_t *obj_addr);

/**
 * \brief Set the value in the object.  The value is the numeric representation
 * \param[in] value The numeric value of the variable
 * \param[in] obj_addr Pointer to the enum object itself
 * \returns error_type_t
 * \retval OK successful
 * \retval -1 FAIL, only happens if the value is out of range.
 */
error_type_t pjl_set_val(pjl_object_t *obj_addr, int32_t value);

/**
 * \brief Set the string value in the object.  
 * \param[in] obj_addr Pointer to the enum object itself
 * \param[in] value A pointer to the string to set.
 * \returns error_type_t
 * \retval OK successful
 * \retval -1 FAIL  Only happens if the value is out of range.
 */
error_type_t pjl_set_string_val(pjl_object_t *obj_addr, char *value);


/**
 * \brief Constructor for the enum_object
 * An enum_object is constructed by this function.  The object provides mapping
 * from a string value to a numeric value.  For example you can have Off/On/Auto
 * in strings map to 0/1/2.  So if you set the numeric set to 1 and then read
 * the string you will get "ON".  Set it to 2 and you get "AUTO", set the string
 * to "OFF" and you get a 0 out of the numeric side.
 *
 * \param[in] match_item The string array that sets the string side of the enum.
 * \param[in] min This is the numeric value that maps to the first item
 *                in the string item list.  
 * \param[in] increment This is the value that is added to min for each item in
 * the list for example with min set to 2 and increment to 1 Off/On/Auto will
 * have the mapping of 2/3/4
 *
 * \returns pjl_object_t
 * \retval NULL Unable to build the item.
 * \retval non-NULL A pointer to the pjl_object_t object
 */
pjl_object_t *pjl_enum_open(WORD_MATCH *match_item, int32_t min, int32_t increment);

/**
 * \brief Destroys a previously constructed enum_object
 * After this is called the object is invalid and should not be used.
 * \param[in] enum_object Pointer to an enum_object
 * \returns void
 */
void pjl_enum_close(pjl_object_t *enum_object);

#ifdef __cplusplus

}

#endif
#endif
