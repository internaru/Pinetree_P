/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
/*
 *
 * Linux Userspace Mech Driver.
 *
 * Mostly just enough to talk to the kernel mech driver via sysfs. Goal is
 * userspace scanmech.c interfaces will connect to this mech driver which will
 * talk to the real mech driver via sysfs.
 *
 * davep 24-Jan-2013
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "scos.h"

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanmech.h"
#include "scanmech_app.h"
#include "scansysfs.h"
#include "logger.h"

/*
 **********************************************************************************************
 * For SINDOH Log System
 **********************************************************************************************
 */ 
#define DBG_PRFX "SCN MECH: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(7)
///Using Method.
#define DBG_PRINTF_SDH_ERR(...) 		DBG_PRINTF(LOG_SDH_ERR, 		DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_SDH_DEBUG(...) 	DBG_PRINTF(LOG_SDH_DEBUG, 	DBG_PRFX __VA_ARGS__)


#define DEV_NAME "/dev/scanman"
static int devfile;  // handle for scan device file

typedef struct
{
    uint32_t    request;
    int         arg;
    void       *ptr;
    scan_err_t  result;
} mech_ioctl_params_t;

/**
 * \brief  Request an ADF pause or release a previously requested pause
 *
 * \author David Poole
 * \date 25-Jan-2013
 */

static scan_err_t scanmech_adf_client_pause( int flag )
{
    FILE *f;

    if( flag!=0 && flag!=1 ) {
        return SCANERR_INVALID_PARAM;
    }

    f = fopen( SCAN_CMDLINE_SCANMECH_PATH, "w" );
    if( f==NULL ) {
        DBG_PRINTF_SDH_DEBUG( "%s failed to open \"%s\" : %s\n", __FUNCTION__, 
                SCAN_CMDLINE_SCANMECH_PATH, strerror(errno) );
        return SCANERR_NO_ENTRY;
    }

    /* fire in the hole! */
    fprintf( f, "%d\n", flag );
    fclose(f);

    return SCANERR_NONE;
}

static scan_err_t scanmech_app_move_to_A3scanpoint( int flag )
{
 	DBG_PRINTF_SDH_DEBUG("%s called \n", __FUNCTION__ );

    if( flag!=0 && flag!=1 ) {
        return SCANERR_INVALID_PARAM;
    }
    return SCANERR_NONE;
}

scan_err_t smech_app_ioctl(t_ScanMech *scan_mech, uint32_t request, int arg, void *ptr)
{
    scan_err_t final_scerr;

    /*
     *   Note! Note! Note! 
     *
     *   This function can be called from any thread context!  Do not assume
     *   the caller is running in scantask thread context.
     *
     *   Dangerous or long running operations (such as motor moves) will be
     *   triggered via a SMIRB (see scanman_smirb.c). Other operations (such as
     *   getting/setting margins) can be called from any thread context.
     *
     */
    final_scerr = SCANERR_NOT_IMPLEMENTED;

    switch( request ) {
        /* TODO ; add margins getter/setter */

        case SCANMECH_IOCTL_ADF_CLIENT_PAUSE :
            final_scerr = scanmech_adf_client_pause(arg);
            break;
		
		case SCANMECH_IOCTL_MOVE_TO_A3POSITION :
			DBG_PRINTF_SDH_DEBUG("%s called : SCANMECH_IOCTL_MOVE_TO_A3POSITION \n", __FUNCTION__ );
			final_scerr = scanmech_app_move_to_A3scanpoint(arg);
			break;

        default :
        {
            mech_ioctl_params_t params;
            int ioctl_result;

            params.arg = arg;
            params.request = request;
            params.ptr = ptr;
            params.result = SCANERR_NOT_IMPLEMENTED;

            ioctl_result = ioctl((int)devfile, _IOWR(0, 1, mech_ioctl_params_t), &params);
            if (ioctl_result == 0)
            {
                final_scerr = params.result;
            }
            else
            {
                dbg1("[%s:%d] mech ioctl error: %d, %d\n", __FILE__, __LINE__, ioctl_result, errno);
                final_scerr = SCANERR_GENERIC_FAIL;
            }
            break;
        }
    }

    return final_scerr;
}

/**
 * \brief Initialize the userspace application scan mech driver. 
 *
 * This driver is small, just enough to communicate with the real mech driver
 * in the kernel but still retain code compatibility with the real mech driver.
 *
 * \author David Poole
 * \date 25-Jan-2013
 */

void smech_app_init( t_ScanMech *scan_mech)
{
    DBG_PRINTF_SDH_DEBUG( "%s\n", __FUNCTION__ );
    
    scan_mech->mech_status = SCAN_MECH_ERROR;

    /* only want ioctl so can talk to kernel via sysfs */
    scan_mech->smech_ioctl_fcn = smech_app_ioctl;

    /* tell the scan/mot layer our driver */
    scan_set_scan_mech(scan_mech);

    devfile = open(DEV_NAME, O_RDONLY);
    if (devfile < 0)
    {
        printf("ERROR WHILE OPENING SCAN DEVICE FILE %s: SCAN IS NON-FUNCTIONAL: is the Scan Kernel Loadable Module loaded?\n",DEV_NAME); 
    }
}

