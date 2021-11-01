/** @file wps_os.h
 *  @brief This file contains definition for timer and socket read functions.
 *  
 *  Copyright (C) 2003-2012, Marvell International Ltd.
 *  All Rights Reserved
 */

#ifndef _WPS_OS_H_
#define _WPS_OS_H_

#include "wps_def.h"

/********************************************************/
/** 
 *  @brief Process event handling initialization
 *  
 *  @param wps_s    A pointer to global WPS structure
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int wps_event_init(WPS_DATA * wps_s);

/** 
 *  @brief Process event handling free
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             None
 */
void wps_event_deinit(WPS_DATA * wps_s);

/** 
 *  @brief Process interface deinit
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             None
 */
void wps_intf_deinit(WPS_DATA * wps_s);

/** 
 *  @brief Free structure used in main loop function
 *
 *  @return         None
 */
void wps_main_loop_free(void);

/** 
 *  @brief Process main loop initialization
 *  
 *  @param wps_s    A pointer to global WPS structure
 *  @param role     WPS Role (Enrollee or Registrar
 *  
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int wps_loop_init(WPS_DATA * wps_s, int role);

/** 
 *  @brief Process main loop free
 *  
 *  @param wps_s        A pointer to global WPS structure
 *  @return             None
 */
void wps_loop_deinit(WPS_DATA * wps_s);

/** 
 *  @brief Register the signal handler to OS
 *  
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int wps_set_signal_handler(void);

/** 
 *  @brief Main loop procedure for socket read and timer functions
 *  
 *  @return             None
 */
void wps_main_loop_proc(void);

/** 
 *  @brief Disable main loop
 *  
 *  @return         None
 */
void wps_main_loop_shutdown(void);

/** 
 *  @brief Enable main loop
 *  
 *  @return         None
 */
void wps_main_loop_enable(void);

/** 
 *  @brief Register a handler to main loop socket read function
 *  
 *  @param sock             Socket number
 *  @param handler          A function pointer of read handler
 *  @param callback_data    Private data for callback
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int wps_register_rdsock_handler(int sock,
                                void (*handler) (int sock, void *sock_ctx),
                                void *callback_data);

/** 
 *  @brief Unregister a handler to main loop socket read function
 *  
 *  @param sock     Socket number
 *  @return         None
 */
void wps_unregister_rdsock_handler(int sock);

/** 
 *  @brief Register a time-out handler to main loop timer function
 *  
 *  @param secs             Time-out value in seconds
 *  @param usecs            Time-out value in micro-seconds
 *  @param handler          A function pointer of time-out handler
 *  @param callback_data    Private data for callback
 *  @return         WPS_STATUS_SUCCESS--success, otherwise--fail
 */
int wps_start_timer(unsigned int secs,
                    unsigned int usecs,
                    void (*handler) (void *user_data), void *callback_data);

/** 
 *  @brief Cancel time-out handler to main loop timer function
 *  
 *  @param handler          Time-out handler to be canceled
 *  @param callback_data    Private data for callback
 *  @return         Number of timer being removed
 */
int wps_cancel_timer(void (*handler) (void *timeout_ctx), void *callback_data);

#endif /* _WPS_OS_H_ */
