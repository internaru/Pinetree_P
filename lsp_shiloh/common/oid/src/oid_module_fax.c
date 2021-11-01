/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_fax.c
 *
 * \brief This file implements the OID back-end functions for the
 * fax API module OIDs
 *
 **/

#include <stdint.h>
#include <string.h>
#include "oid_api.h"
#include "oid_producer_api.h"

#include "logger.h"
#include "debug.h"
#include "lassert.h"
#ifdef HAVE_FAX_SUPPORT
#include "faxer_phonebk.h"
#include "fax_app_vars.h"
#include "faxer_api.h"
#endif

extern void *fax_service_handle;

typedef struct {
        oid_t oid;
        void * get_callback;
        void * set_callback;
        void * test_callback;
} fax_oid_t;

static uint32_t new_entry = 0;

/**
 * 
 * \brief Get the first blank entry from phonebook 
 *
 * \param[in] none. 
 *
 * \param[out]  none.
 *
 * \return first blank entry number.  
 *
 */
static uint32_t     oid_module_fax_phonebook_get_first_entry( void );

/**
 * 
 * \brief The backend oid fax API module function for retreiving
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_phonebook_get_int( oid_t oid, uint32_t index, uint32_t * value);

/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value is a integer where the oid output is stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_phonebook_set_int( oid_t oid, uint32_t index, uint32_t value);
/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_phonebook_get_string( oid_t oid, uint32_t index, char * data_ptr, uint32_t max_string_len_in_bytes );
/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_phonebook_set_string( oid_t oid, uint32_t index, const char * data, uint32_t string_len_in_bytes );

/**Fax File Upload Oids   */
/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_upload_get_string( oid_t oid, uint32_t index, char * data_ptr, uint32_t max_string_len_in_bytes );

/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_upload_set_string( oid_t oid, uint32_t index, const char * data, uint32_t string_len_in_bytes );


/**Fax File Upload Oids   */
/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_upload_get_int( oid_t oid, uint32_t index, uint32_t * value);

/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_upload_set_int( oid_t oid, uint32_t index, uint32_t value);

/**Fax Config */
/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_config_get_string( oid_t oid, uint32_t index, char * data_ptr, uint32_t max_string_len_in_bytes );
/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_config_set_string( oid_t oid, uint32_t index, const char * data, uint32_t string_len_in_bytes );
/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_config_get_int( oid_t oid, uint32_t index, uint32_t * value);
/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_config_set_int( oid_t oid, uint32_t index, uint32_t value);

//The fax oids look up table
const fax_oid_t oids[] = 
{
/**Fax Phone Book Oids */ 
    {OID_FAX_PHONEBOOK_FIRST_EMPTY_INDEX,   oid_module_fax_phonebook_get_int,     oid_module_fax_phonebook_set_int,     oid_module_null_test},
    {OID_FAX_PHONEBOOK_RANGE,               oid_module_fax_phonebook_get_int,     oid_module_null_set,                  oid_module_null_test},
    {OID_FAX_PHONEBOOK_FIRST_EMPTY_NAME,    oid_module_fax_phonebook_get_string,  oid_module_fax_phonebook_set_string,  oid_module_null_test},
    {OID_FAX_PHONEBOOK_FIRST_EMPTY_NUMBER,  oid_module_fax_phonebook_get_string,  oid_module_fax_phonebook_set_string,  oid_module_null_test},
/**Fax UploadDyn Oids */ 
    {OID_FAX_UPLOAD_FEATURE,                oid_module_fax_upload_get_int,        oid_module_fax_upload_set_int,        oid_module_null_test},
    {OID_FAX_UPLOAD_HOSTNAME,               oid_module_fax_upload_get_string,     oid_module_fax_upload_set_string,     oid_module_null_test},
    {OID_FAX_UPLOAD_BACKUP,                 oid_module_fax_upload_get_int,        oid_module_fax_upload_set_int,        oid_module_null_test},
/**Fax config */ 
    //SYS
    {OID_FAX_CONFIG_SYS_TELECOM_VER,        oid_module_fax_config_get_string,     oid_module_fax_config_set_string,     oid_module_null_test},
    {OID_FAX_CONFIG_SYS_COMPANY_NAME,       oid_module_fax_config_get_string,     oid_module_fax_config_set_string,     oid_module_null_test},
    {OID_FAX_CONFIG_SYS_PHONE_NUMBER,       oid_module_fax_config_get_string,     oid_module_fax_config_set_string,     oid_module_null_test},
    {OID_FAX_CONFIG_SYS_SPEED,              oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_MODEM_VOL,          oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_RINGER_VOL,         oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_AUTOPRINT_CONFIRM,  oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_AUTOPRINT_ERROR,    oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_AUTOPRINT_T30,      oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_AUTOPRINT_ACTIVITY, oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_DEDICATED_FAXLINE,  oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_ECM_MODE,           oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_EXT_PHONE,          oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_THUMBNAILONCALL,    oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_SETUP_COMPLETE,     oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SYS_ADDITIONAL_PHONE,   oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},

    //SEND
    {OID_FAX_CONFIG_SEND_NUMBER_PREFIX,     oid_module_fax_config_get_string,     oid_module_fax_config_set_string,     oid_module_null_test},
    {OID_FAX_CONFIG_SEND_LIGHTERDARKER,     oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_RESOLUTION,        oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_GLASSSIZE,         oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_DIALINGMODE,       oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_CONFIRMFAXNUMBER,  oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_BILLINGCODE,       oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_NUMBERPRIFIX_USED, oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_PRINTTOFAX,        oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_REDIALIFBUSY,      oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_REDIALIFERROR,     oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_REDIALIFNOANS,     oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_SEND_SCANANDFAX,        oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},

    //RECEIVE
    {OID_FAX_CONFIG_RECEIVE_ANS_MODE,       oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_RINGTIME,       oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_RINGSTOANS,     oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_DISTINCRING,    oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_FORWARDNUMBER,  oid_module_fax_config_get_string,     oid_module_fax_config_set_string,     oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_OUTPUTPLEX,     oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_ALLOWREPRINT,   oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_BLOCKJUNKFAX,   oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_DETECTDIALTONE, oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_FITTOPAGE,      oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_PRIVATERECEIVE, oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_STAMPFAX,       oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_PRINTCHECKMEDIA, oid_module_fax_config_get_int,       oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_MEDIASIZE,      oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_BACKUP,         oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_FORWARD,        oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_FORWARDSTART,   oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},
    {OID_FAX_CONFIG_RECEIVE_FORWARDSTOP,    oid_module_fax_config_get_int,        oid_module_fax_config_set_int,        oid_module_null_test},

    {NULL,                                  oid_module_null_get,                  oid_module_null_set,                  oid_module_null_test},
};


 /**
 *
 * \brief Registers each fax API OID with the oid controller
 *
 */
void oid_register_module_fax( void )
{
    error_type_t e_res;
    int i = 0;
    while(oids[i].oid)
    {
        e_res = oid_register_callbacks( oids[i].oid,
                                        oids[i].get_callback,
                                        oids[i].set_callback,
                                        oids[i].test_callback );
        ASSERT( FAIL != e_res );
        i++;
    }
}


/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Integer where the oid output is stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_fax_phonebook_set_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t value)
{
    error_type_t e_res = OK;

    switch ( oid )
    {
    case OID_FAX_PHONEBOOK_FIRST_EMPTY_INDEX:
        new_entry = value;
        dbg_printf("Phone book receive index (%d)\n", new_entry);
        break;

    default:
        /* bad oid */
        e_res = FAIL;
        break;

    }
    
    return e_res;
}

/**
 * 
 * \brief The backend oid fax API module function for retreiving
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_fax_phonebook_get_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t * value)
{
    error_type_t e_res = OK;

    switch ( oid )
    {
    case OID_FAX_PHONEBOOK_FIRST_EMPTY_INDEX:
       *value = oid_module_fax_phonebook_get_first_entry();
       dbg_printf("INDEX is %d\n",*value);
       break;

    case OID_FAX_PHONEBOOK_RANGE:
        if (new_entry == 0) 
        {
            *value = 0;
        }
        else
        {
            *value = (new_entry - 1) / 25;
        }
        dbg_printf("Get range %d\n",*value);
        
        new_entry = 0;
        break;

    default:
        /* bad oid */
        e_res = FAIL;
        dbg_printf("Get index fail\n");
        break;

    }
    //dbg_printf("oid index %d\r\n",index);
    return e_res;
}

/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_fax_phonebook_set_string( oid_t oid,
        uint32_t index,
        const char * data,
        uint32_t string_len_in_bytes )
{
    error_type_t e_res = OK;
    static char pbname[PHB_NAME_MAX + 1] = {0};
    char pbnumber[PHB_TEL_MAX + 1] = {0};
#ifdef HAVE_FAX_SUPPORT
    PHB_ENTRY ent;
#endif
    

    switch( oid ) 
    {
    case OID_FAX_PHONEBOOK_FIRST_EMPTY_NAME:
        //To protect the length not exceed
        strncpy(pbname,data,PHB_NAME_MAX + 1);
        pbname[PHB_NAME_MAX] = '\0';
        dbg_printf("Phone book receive name (%s)\n", pbname);
        break;

    case OID_FAX_PHONEBOOK_FIRST_EMPTY_NUMBER:
        strncpy(pbnumber,data,PHB_TEL_MAX + 1);
        pbnumber[PHB_TEL_MAX] = '\0';
#ifdef HAVE_FAX_SUPPORT
        faxer_phb_build_entry(e_SpeedDial,pbname,(char*)data,&ent);
        dbg_printf("Phone book receive number (%s)\n", data);
        if(faxer_phb_save_entry(new_entry,&ent))
        {
            dbg_printf("Fail to save phone book entry!\n");
        }
#else
        dbg_printf("Fail to save phone book entry!\n");
        e_res = FAIL;
#endif
        memset(pbname,0x00,PHB_NAME_MAX + 1);
        break;

    default:
        e_res = FAIL;
        break;
    }
    return e_res;
}

/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_fax_phonebook_get_string( oid_t oid,
                                            uint32_t index,
                                            char * data_ptr,
                                            uint32_t max_string_len_in_bytes )
{
    error_type_t e_res = OK;
#ifdef HAVE_FAX_SUPPORT
    static PHB_ENTRY ent;
#endif
    //unsigned int string_size;

    switch ( oid )
    {
    case OID_FAX_PHONEBOOK_FIRST_EMPTY_NAME:
#ifdef HAVE_FAX_SUPPORT
        if(!faxer_phb_get_entry(oid_module_fax_phonebook_get_first_entry(), &ent))
        {
            strcpy(data_ptr,PHB_ENTRY_GET_NAME(&ent));
            dbg_printf("Phone book name (%s)\n", data_ptr);
        }
        else
#endif
        {
            strcpy(data_ptr,"");
        }
        break;
    
    case OID_FAX_PHONEBOOK_FIRST_EMPTY_NUMBER:
#ifdef HAVE_FAX_SUPPORT
        if(!faxer_phb_get_entry(oid_module_fax_phonebook_get_first_entry(), &ent))
        {
            strcpy(data_ptr,PHB_SPEEDDIAL_GET_NUMBER(&ent));
            dbg_printf("Phone book number (%s)\n", data_ptr);
        }
        else
#endif
        {
            strcpy(data_ptr,"");
        }
        break;
    

    default:
        /* bad oid */
        e_res = FAIL;
        break;
        
    }
    
    return e_res;
}

/**
 * 
 * \brief Get the first blank entry from phonebook 
 *
 * \param[in] none. 
 *
 * \param[out]  none.
 *
 * \return first blank entry number.  
 *
 */
uint32_t oid_module_fax_phonebook_get_first_entry()
{
    int i;
    uint32_t    phonebook_index = 1;
#ifdef HAVE_FAX_SUPPORT
    PHB_ENTRY ent;
    for(i = 1;i<=PHB_MAX_ENTRIES;i++)
    {
       if(faxer_phb_get_entry(i, &ent))
       {
            phonebook_index = i;
            break;
       }
    }
#endif
    return phonebook_index;
}

/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_fax_upload_get_string( oid_t oid, uint32_t index, char * data_ptr, uint32_t max_string_len_in_bytes )
{

    error_type_t e_res = OK;

    switch ( oid )
    {
    
    case OID_FAX_UPLOAD_HOSTNAME:
        fax_app_get_var_upload_hostname( data_ptr );
        break;
    
    default:
        /* bad oid */
        e_res = FAIL;
        break;
        
    }
    
    dbg_printf("OID*** 0x%x namespace:%d area:%d value:%d type:%d\n",oid, oid>>24,(oid>>16)&0xff, (oid>>8)&0xff,oid&0xff);
    return e_res;
}

/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_fax_upload_set_string( oid_t oid, uint32_t index, const char * data, uint32_t string_len_in_bytes )
{

    error_type_t e_res = OK;
    
    switch( oid ) 
    {

    case OID_FAX_UPLOAD_HOSTNAME:
        fax_app_set_var_upload_hostname( data );
        break;


    default:
        e_res = FAIL;
        break;
    }
    return e_res;
}


/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_upload_get_int( oid_t oid, uint32_t index, uint32_t * value)
{

    error_type_t e_res = OK;

    switch ( oid )
    {
    case OID_FAX_UPLOAD_FEATURE:
        {
            bool val = 0;
            fax_app_get_var_upload_feature( &val );
            *value = (uint32_t) val;
        }
        break;

    case OID_FAX_UPLOAD_BACKUP:
        {
            bool val = 0;
            fax_app_get_var_backup_print( &val );
            *value = (uint32_t) val;
        }
        break;

    default:
        /* bad oid */
        e_res = FAIL;
        break;
        
    }
    
    //dbg_printf("OID*** 0x%x namespace:%d area:%d value:%d type:%d\n",oid, oid>>24,(oid>>16)&0xff, (oid>>8)&0xff,oid&0xff);
    return e_res;
}

/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_upload_set_int( oid_t oid, uint32_t index, uint32_t value)
{

    error_type_t e_res = OK;

    switch( oid ) 
    {
    case OID_FAX_UPLOAD_FEATURE:
        {
            bool val;
            val = value;
            fax_app_set_var_upload_feature( &val );
            dbg_printf("Feature %d\n",value);
        }
        break;

    case OID_FAX_UPLOAD_BACKUP:
        {
            bool val;
            val = value;
            fax_app_set_var_backup_print( &val );
            dbg_printf("Backup %d\n",value);
        }
        break;

    default:
        e_res = FAIL;
        break;
    }
    return e_res;
}

/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
error_type_t oid_module_fax_config_get_string( oid_t oid, uint32_t index, char * data_ptr, uint32_t max_string_len_in_bytes )
{

    error_type_t e_res = OK;

    switch ( oid )
    {

    case OID_FAX_CONFIG_SYS_TELECOM_VER:
        //faxer_config_get_setting( fax_service_handle, FAXER_SETTING_COUNTRY_ID, value );
        faxer_raw_command( fax_service_handle, "TELECOM_VERSION", data_ptr );
        break;

    case OID_FAX_CONFIG_SYS_COMPANY_NAME:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_CHR_HEADER_NAME, data_ptr );
        dbg_printf("%s\n", data_ptr);
        break;

    case OID_FAX_CONFIG_SYS_PHONE_NUMBER:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_CHR_SID, data_ptr );
        break;

    case OID_FAX_CONFIG_SEND_NUMBER_PREFIX:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_CHR_PREFIX, data_ptr );
        break;

    case OID_FAX_CONFIG_RECEIVE_FORWARDNUMBER:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_CHR_FORWARD_NUMBER, data_ptr );
        break;
    default:
        dbg_printf("------oid no found!\n");
        /* bad oid */
        e_res = FAIL;
        break;
        
    }
    
    //dbg_printf("OID*** 0x%x namespace:%d area:%d value:%d type:%d\n",oid, oid>>24,(oid>>16)&0xff, (oid>>8)&0xff,oid&0xff);
    return e_res;

}

/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid 
 *  
 * \param[in] max_string_len_in_bytes the length of buffer. 
 *
 * \param[out] value Pointer to buffer where the oid output is 
 *       stored
 * 
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */

error_type_t oid_module_fax_config_set_string( oid_t oid, uint32_t index, const char * data, uint32_t string_len_in_bytes )
{

    error_type_t e_res = OK;

    switch ( oid )
    {
    case OID_FAX_CONFIG_SYS_TELECOM_VER:
        //uint32_t value = 0;
        //faxer_config_set_setting( fax_service_handle, FAXER_SETTING_COUNTRY_ID, &value );
        break;

    case OID_FAX_CONFIG_SYS_COMPANY_NAME:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_CHR_HEADER_NAME, (void*)data );
        dbg_printf("%s\n", data);
        break;

    case OID_FAX_CONFIG_SYS_PHONE_NUMBER:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_CHR_SID, (void*)data );
        break;

    case OID_FAX_CONFIG_SEND_NUMBER_PREFIX:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_CHR_PREFIX, (void*)data );
        break;

    case OID_FAX_CONFIG_RECEIVE_FORWARDNUMBER:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_CHR_FORWARD_NUMBER, (void*)data );
        break;

    default:
        /* bad oid */
        e_res = FAIL;
        break;
        
    }
    
    return e_res;
}

/**
 * 
 * \brief The backend oid fax API module function for getting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_config_get_int( oid_t oid, uint32_t index, uint32_t * value)
{
    error_type_t e_res = OK;

    switch ( oid )
    {
    //Fax sys
    case OID_FAX_CONFIG_SYS_MODEM_VOL:
        {
            fax_volume_setting_t vol;
            fax_app_get_var_volume_phoneline( &vol );
            *value = (uint32_t)vol;
        }
        break;

    case OID_FAX_CONFIG_SYS_RINGER_VOL:
        {
            fax_volume_setting_t vol;
            fax_app_get_var_volume_ring( &vol );
            *value = (uint32_t)vol;
        }
        break;

    case OID_FAX_CONFIG_SYS_AUTOPRINT_CONFIRM:
        {
            fax_autoprint_confirm_t val;
            fax_app_get_var_print_confirm( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_SYS_AUTOPRINT_ERROR:
        {
            fax_autoprint_error_t val;
            fax_app_get_var_print_error( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_SYS_AUTOPRINT_T30:
        {
            fax_autoprint_t30_t val;
            fax_app_get_var_print_t30_report( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_SYS_AUTOPRINT_ACTIVITY:
        {
            bool val;
            fax_app_get_var_print_activity_report( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_SYS_DEDICATED_FAXLINE:
        *value = 0;
        //strcpy(data_ptr,"disabled");
        break;

    case OID_FAX_CONFIG_SYS_ECM_MODE:
        {
            faxer_config_get_setting( fax_service_handle, FAXER_SETTING_SW_ECM_MODE, value );
        }
        break;

    case OID_FAX_CONFIG_SYS_EXT_PHONE:
        *value = 0;
        break;

    case OID_FAX_CONFIG_SYS_THUMBNAILONCALL:
        {
            bool val;
            fax_app_get_var_thumbnail_report( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_SYS_SETUP_COMPLETE:
        *value = 1;
        //strcpy(data_ptr,"true");
        break;

    case OID_FAX_CONFIG_SYS_ADDITIONAL_PHONE:
        *value = 0;
        break;

    //SEND
    case OID_FAX_CONFIG_SEND_RESOLUTION:
        {
            fax_resolution_t resolution;
            fax_app_get_var_scan_resolution(&resolution);
            *value = (uint32_t)resolution;
        }
        break;

    case OID_FAX_CONFIG_SEND_GLASSSIZE:
        {
            fax_media_size_t val;
            fax_app_get_var_glass_size( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_SEND_DIALINGMODE:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_INT_DIALING_MODE, value );
        break;

    case OID_FAX_CONFIG_SEND_CONFIRMFAXNUMBER:
        *value = 0;
        break;

    case OID_FAX_CONFIG_SEND_BILLINGCODE:
        *value = 0;
        break;

    case OID_FAX_CONFIG_SEND_NUMBERPRIFIX_USED://
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_SW_DIAL_PREFIX, value );
        break;

    case OID_FAX_CONFIG_SEND_PRINTTOFAX:
        *value = true;
        break;

    case OID_FAX_CONFIG_SEND_REDIALIFBUSY:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_SW_BUSY_REDIAL, value );
        break;

    case OID_FAX_CONFIG_SEND_REDIALIFERROR:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_SW_COMMERR_REDIAL, value );
        break;

    case OID_FAX_CONFIG_SEND_REDIALIFNOANS:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_SW_NOANS_REDIAL, value );
        break;

    case OID_FAX_CONFIG_SEND_SCANANDFAX:
        *value = 1;
        break;

    //RECEIVE
    case OID_FAX_CONFIG_RECEIVE_ANS_MODE:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_INT_ANSWER_MODE, value );
        break;

    case OID_FAX_CONFIG_RECEIVE_DISTINCRING:
        {
            uint32_t val;
            
            faxer_config_get_setting( fax_service_handle, FAXER_SETTING_INT_RING_PATTERN, &val );
            switch (val) 
            {
            case 0:
                *value = e_FAX_RING_ALL;
                break;

            case 1:
                *value = e_FAX_RING_SINGLE;
                break;

            case 2:
                *value = e_FAX_RING_DOUBLE;
                break;

            case 3:
                *value = e_FAX_RING_TRIPLE;
                break;

            case 6:
                *value = e_FAX_RING_DOUBLE_TRIPLE;
                break;

            default:
                *value = e_FAX_RING_ALL;
                break;
            }
            
            dbg_printf("OID dis %d\n",*value);
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_OUTPUTPLEX:
        {
            fax_output_plex_t val;
            fax_app_get_var_output_plex( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_ALLOWREPRINT:
        {
            bool val;
            fax_app_get_var_allow_reprint( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_BLOCKJUNKFAX:
        {
            bool val;
            fax_app_get_var_block_junkfax( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_DETECTDIALTONE:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_SW_DETECT_DIAL_TONE, value );
        break;

    case OID_FAX_CONFIG_RECEIVE_FITTOPAGE:
        {
            bool val;
            fax_app_get_var_fit_page( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_PRIVATERECEIVE:
        *value = 0;
        break;

    case OID_FAX_CONFIG_RECEIVE_STAMPFAX:
        {
            bool val;
            fax_app_get_var_stamp_fax( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_PRINTCHECKMEDIA:
        *value = 0;
        break;

    case OID_FAX_CONFIG_RECEIVE_MEDIASIZE:
        {
            fax_media_size_t val;
            fax_app_get_var_media_size( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_BACKUP://FAXER_SETTING_FAX_DOC_BACKUP??
        {
            fax_recv_action_t val;
            // Todo: recv action is not same as backup, fix
            fax_app_get_var_recv_action( &val );
            *value = (uint32_t)val;
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_FORWARD:
        {
            fax_recv_action_t recv_action;

            fax_app_get_var_recv_action( &recv_action );
            if (recv_action == e_FAX_RECV_FORWARD)
            {
                *value = true;
            }
            else
            {
                *value = false;
            }
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_FORWARDSTART:
        *value = 0;
        break;

    case OID_FAX_CONFIG_RECEIVE_FORWARDSTOP:
        *value = 0;
        break;

    case OID_FAX_CONFIG_SEND_LIGHTERDARKER:
        fax_app_get_var_scan_contrast( value );
        break;

    case OID_FAX_CONFIG_RECEIVE_RINGTIME:
        fax_app_get_var_faxtel_ringtime( value );
        break;

    case OID_FAX_CONFIG_RECEIVE_RINGSTOANS:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_INT_RINGS_TO_ANSWER, value );
        break;

    case OID_FAX_CONFIG_SYS_SPEED:
        faxer_config_get_setting( fax_service_handle, FAXER_SETTING_INT_MAX_SPEED, value );
        break;

    default:
        /* bad oid */
        e_res = FAIL;
        break;
    }
    
    dbg_printf("OID*** 0x%x namespace:%d area:%d value:%d type:%d\n",oid, oid>>24,(oid>>16)&0xff, (oid>>8)&0xff,oid&0xff);
    return e_res;

}

/**
 * 
 * \brief The backend oid fax API module function for setting
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_fax_config_set_int( oid_t oid, uint32_t index, uint32_t value)
{
    error_type_t e_res = OK;

    switch( oid ) 
    {

    //Fax sys
    case OID_FAX_CONFIG_SYS_SPEED:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_INT_MAX_SPEED, &value );
        break;

    case OID_FAX_CONFIG_SYS_MODEM_VOL:
        {
            fax_volume_setting_t val;
            val = value;
            fax_app_set_var_volume_phoneline( &val );
        }
        break;

    case OID_FAX_CONFIG_SYS_RINGER_VOL:
        {
            fax_volume_setting_t val;
            val = value;
            fax_app_set_var_volume_ring( &val );
        }
        break;

    case OID_FAX_CONFIG_SYS_AUTOPRINT_CONFIRM:
        {
            fax_autoprint_confirm_t val = value;
            fax_app_set_var_print_confirm( &val );
        }
        break;

    case OID_FAX_CONFIG_SYS_AUTOPRINT_ERROR:
        {
            fax_autoprint_error_t val = value;
            fax_app_set_var_print_error( &val );
        }
        break;

    case OID_FAX_CONFIG_SYS_AUTOPRINT_T30:
        {
            fax_autoprint_t30_t val = value;
            fax_app_set_var_print_t30_report( &val );
        }
        break;

    case OID_FAX_CONFIG_SYS_AUTOPRINT_ACTIVITY:
        {
            bool val = value;
            fax_app_set_var_print_activity_report( &val );
        }
        break;

    case OID_FAX_CONFIG_SYS_DEDICATED_FAXLINE:
        dbg_printf("OID DEDICATED %d\n", value);
        break;

    case OID_FAX_CONFIG_SYS_ECM_MODE:
        {
            faxer_config_set_setting( fax_service_handle, FAXER_SETTING_SW_ECM_MODE, &value );
        }
        break;

    case OID_FAX_CONFIG_SYS_EXT_PHONE:
        dbg_printf("OID EXT PHONE %d\n", value);
        break;

    case OID_FAX_CONFIG_SYS_THUMBNAILONCALL:
        {
            bool val = value;
            fax_app_set_var_thumbnail_report( &val );
        }
        break;

    case OID_FAX_CONFIG_SYS_SETUP_COMPLETE:
        dbg_printf("OID SETUP COMPLETE %d\n", value);
        break;

    case OID_FAX_CONFIG_SYS_ADDITIONAL_PHONE:
        dbg_printf("OID SYS ADDITIONAL %d\n", value);
        break;

    //SEND

    case OID_FAX_CONFIG_SEND_RESOLUTION:
        {
            fax_resolution_t resolution = value;
            fax_app_set_var_scan_resolution(&resolution);
        }
        break;

    case OID_FAX_CONFIG_SEND_GLASSSIZE:
        {
            fax_media_size_t val = value;
            fax_app_set_var_glass_size( &val );
        }
        break;

    case OID_FAX_CONFIG_SEND_DIALINGMODE:
        {
            faxer_config_set_setting( fax_service_handle, FAXER_SETTING_INT_DIALING_MODE, &value );
        }
        break;

    case OID_FAX_CONFIG_SEND_CONFIRMFAXNUMBER:
        dbg_printf("OID CONFIRM FAX NUMBER %d\n", value);
        break;

    case OID_FAX_CONFIG_SEND_BILLINGCODE:
        dbg_printf("OID BILLINGCODE %d\n", value);
        break;

    case OID_FAX_CONFIG_SEND_NUMBERPRIFIX_USED:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_SW_DIAL_PREFIX, &value );
        break;

    case OID_FAX_CONFIG_SEND_PRINTTOFAX:
        dbg_printf("OID PRINT TO FAX %d\n", value);
        break;

    case OID_FAX_CONFIG_SEND_REDIALIFBUSY:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_SW_BUSY_REDIAL, &value );
        break;

    case OID_FAX_CONFIG_SEND_REDIALIFERROR:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_SW_COMMERR_REDIAL, &value );
        break;

    case OID_FAX_CONFIG_SEND_REDIALIFNOANS:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_SW_NOANS_REDIAL, &value );
        break;

    case OID_FAX_CONFIG_SEND_SCANANDFAX:
        dbg_printf("OID SCAN TO FAX %d\n", value);
        break;

    //RECEIVE
    case OID_FAX_CONFIG_RECEIVE_ANS_MODE:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_INT_ANSWER_MODE, &value );
        break;

    case OID_FAX_CONFIG_RECEIVE_DISTINCRING:
        {
            uint32_t val;

            switch (value) 
            {
            case 0:
                val = e_FAX_RING_ALL;
                break;

            case 1:
                val = e_FAX_RING_SINGLE;
                break;

            case 2:
                val = e_FAX_RING_DOUBLE;
                break;

            case 3:
                val = e_FAX_RING_TRIPLE;
                break;

            case 6:
                val = e_FAX_RING_DOUBLE_TRIPLE;
                break;

            default:
                val = e_FAX_RING_ALL;
                break;
            }

            faxer_config_set_setting( fax_service_handle, FAXER_SETTING_INT_RING_PATTERN, &val );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_OUTPUTPLEX:
        {
            fax_output_plex_t val =  value;
            fax_app_set_var_output_plex( &val );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_ALLOWREPRINT:
        {
            bool val = value;
            fax_app_set_var_allow_reprint( &val );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_BLOCKJUNKFAX:
        {
            bool val = value;
            fax_app_set_var_block_junkfax( &val );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_DETECTDIALTONE:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_SW_DETECT_DIAL_TONE, &value );
        break;

    case OID_FAX_CONFIG_RECEIVE_FITTOPAGE:
        {
            bool val = value;
            fax_app_set_var_fit_page( &val );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_PRIVATERECEIVE:
        dbg_printf("OID RECEIVE PRIVATE %d\n", value);
        break;

    case OID_FAX_CONFIG_RECEIVE_STAMPFAX:
        {
            bool val = value;
            fax_app_set_var_stamp_fax( &val );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_PRINTCHECKMEDIA:
        dbg_printf("OID RECEIVE PRINT CHECKMEDIA %d\n", value);
        break;

    case OID_FAX_CONFIG_RECEIVE_MEDIASIZE:
        {
            fax_media_size_t val = value;
            fax_app_set_var_media_size( &val );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_BACKUP:
        {
            fax_recv_action_t val = value;
            // TODO - recv action is not same as backup - fix
            fax_app_set_var_recv_action( &val );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_FORWARD:
        {
            fax_recv_action_t recv_action;

            if (value)
            {
                recv_action = e_FAX_RECV_FORWARD;
            }
            else
            {
                recv_action = e_FAX_RECV_AND_PRINT;
            }

            fax_app_set_var_recv_action( &recv_action );
        }
        break;

    case OID_FAX_CONFIG_RECEIVE_FORWARDSTART:
        dbg_printf("OID RECEIVE FORWARD START %d\n", value);
        break;

    case OID_FAX_CONFIG_RECEIVE_FORWARDSTOP:
        dbg_printf("OID RECEIVE FORWARD STOP %d\n", value);
        break;

    case OID_FAX_CONFIG_SEND_LIGHTERDARKER:
        fax_app_set_var_scan_contrast( &value );
        break;

    case OID_FAX_CONFIG_RECEIVE_RINGTIME:
        fax_app_set_var_faxtel_ringtime( &value );
        break;

    case OID_FAX_CONFIG_RECEIVE_RINGSTOANS:
        faxer_config_set_setting( fax_service_handle, FAXER_SETTING_INT_RINGS_TO_ANSWER, &value );
        break;

    default:
        e_res = FAIL;
        break;
    }
    return e_res;
}
