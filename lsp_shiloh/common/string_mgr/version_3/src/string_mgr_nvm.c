/*
 * ============================================================================
 * (C) Copyright 2011   Marvell International Ltd.
 *                          All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <string.h>

#include "string_mgr_nvm.h"

#define _DEFAULT_LANGUAGE LANG_ENGLISH

typedef struct
{
  string_mgr_language_t lang; /*!< The current system language */
} _image_t;
static _image_t        _image;

string_mgr_language_t string_mgr_nvm_get_lang( void )
{
    return _image.lang;
}


#ifdef HAVE_NVRAM

#include "lassert.h"
#include "nvram_api.h"
#include "data_access_api.h" /* For VAR_ID */

/** \brief The string manager nvram manager block version number.
 * This number should be incremented every time the structure
 * "_image_t" changes */
#define _NVM_VERS 2
static nvram_handle_t *_hdl;

void string_mgr_nvm_set_lang( string_mgr_language_t language )
{
    _image.lang = language;
    nvram_set_var( _hdl, &_image );
}

static error_type_t _init(nvram_init_type_t init_type,
                          uint16_t version,
                          void    *image,
                          uint32_t image_size )
{
    switch(init_type)
    {
        case NVRAM_NEW:
        case NVRAM_ERROR:
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_VERSION_CHANGE: // for now, set defaults, production code should migrate versions
        case NVRAM_LANG_DEFAULTS:
        {
            ((_image_t *)image)->lang = _DEFAULT_LANGUAGE;
            break;
        }
        case NVRAM_USER_DEFAULTS:
        {
            // no action required
            break;
        }
        default:
        {
            ASSERT(NVRAM_OK == init_type);

            // read out current values
            memcpy(&_image, image, sizeof(_image_t));
            break;
        }
    }
    return OK;
}

void string_mgr_nvm_init(void)
{
    error_type_t e_res;
    e_res = nvram_variable_register( &_hdl, STRING_MGR_VAR_ID, _NVM_VERS,
                                     sizeof(_image_t), _init );
    ASSERT( OK == e_res );
}

#else  /* HAVE_NVRAM */

void string_mgr_nvm_set_lang( string_mgr_language_t language )
{
    _image.lang = language;
}

void string_mgr_nvm_init(void)
{
    _image.lang = _DEFAULT_LANGUAGE;
}

#endif /* HAVE_NVRAM */
