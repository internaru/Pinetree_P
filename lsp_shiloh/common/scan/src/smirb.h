/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SMIRB_H
#define SMIRB_H

#define SCANMAN_IOCTL_REQUEST_COOKIE 0x6849EE38

struct scanman_ioctl_request_block {
    /*
     *  Private fields.
     */
    /* passing around void pointers to this structure so must make sure we have
     * a pointer on the receiving end
     */
    uint32_t cookie;

    /* id is the request_id via scanman_dev_request(). Also the id in the
     * observer callback 
     */
    uint32_t id;

    /* 
     *  Public fields.
     */
    /* request/response fields (read/write by ioctl) */
    uint32_t request;
    int arg;
    void *ptr;

    /* response (result of smech_ioctl()) */
    scan_err_t scerr;
};

scan_err_t scanman_smirb_onetime_init( void );
scan_err_t scanman_smirb_alloc( struct scanman_ioctl_request_block **smirb );
void scanman_smirb_free( struct scanman_ioctl_request_block **smirb );
void scanman_smirb_sanity( struct scanman_ioctl_request_block *smirb );

#endif

