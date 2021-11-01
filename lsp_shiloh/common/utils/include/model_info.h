/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_MODEL_INFO_H
#define INC_MODEL_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

#define MODEL_INFO_STR_SIZE 64

/**
 * @brief Get the platform model name
 *
 * This routine gets the platform model name from platvars_get_prtnamestr()
 *
 * @param  String	return pointer to model name string
 *
 * @return OK or FAIL
 */
error_type_t get_model_name(char **String);


/**
 * @brief Get the platform model number
 *
 * This routine gets the platform model number from platvars_get_prtnamestr()
 *
 * @param  String	return pointer to model number string
 *
 * @return OK or FAIL
 */
error_type_t get_model_number(char **String);

#ifdef __cplusplus
}
#endif

#endif /* INC_MODEL_INFO_H */
