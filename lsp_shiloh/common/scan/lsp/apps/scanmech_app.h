/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

#ifndef SCANMECH_APP_H
#define SCANMECH_APP_H

void smech_app_init( t_ScanMech *scan_mech);
scan_err_t smech_app_ioctl(t_ScanMech *scan_mech, uint32_t request, int arg, void *ptr);

#endif

