/** @file moal_usb.c
  *
  * @brief This file contains the interfaceing to USB bus 
  * driver. 
  * 
  * Copyright (C) 2008-2012, Marvell International Ltd. 
  * All Rights Reserved
  */

/********************************************************
Change log:
    10/21/2008: initial version
********************************************************/

#include	"moal_main.h"
#include	"moal_usb.h"
#ifdef HAVE_ASIC_POWER_DRIVER
#include    "usbh_wifi_pwr_drv.h"
#endif

int
woal_enter_usb_suspend(moal_handle * handle)
{
    if (wlan_usb_suspend(NULL) == 0)
        return MLAN_STATUS_SUCCESS;
    else
        return MLAN_STATUS_FAILURE;
}

int
woal_exit_usb_suspend(moal_handle * handle)
{
#ifdef HAVE_ASIC_POWER_DRIVER
    usb_host_wifi_pwr_usb_resume();
#else
    wlan_usb_resume();
#endif
    return MLAN_STATUS_SUCCESS;
}
