/**
 * \file nvram_db_api.h
 **/

#ifndef INC_NVRAMDBAPI_H
#define INC_NVRAMDBAPI_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

error_type_t db_variable_register(char* Label,
                                  uint16_t Version,
                                  nvram_init_func_ptr   NvramInitFunction);

#ifdef __cplusplus
}
#endif

#endif /* INC_NVRAMDBAPI_H */
