/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_SLP_H
#define NET_SLP_H
typedef enum
{
    SLP_MSG_ENABLE,
    SLP_MSG_DISABLE,
    SLP_MSG_NET_UP,
    SLP_MSG_NET_UP_POLL,
    SLP_MSG_NET_DOWN,
    SLP_MSG_SEND_REPLY,
    SLP_MSG_SEND_ANNOUNCE,

    // add new msgs above this line
    SLP_NUM_MSGS
} slp_msg_t;

typedef struct slp_msg_buf_s
{
  slp_msg_t msg;
  UINT if_enum;  // I think this is an index
} slp_msg_buf_t;

void net_slp_notify_var_enabled(void);
/**
 * @brief Execute a task for a registered network application.
 * 
 * @param hndl Opaque application handle acquired via net_app_register().
 * 
 * @param[in] data1 Opaque user-defined data to be relayed to the application in the call to the 
 *       registered net_app_execute_func_t function callback.
 * 
 * @param[in] data2 Opaque user-defined data to be relayed to the application in the call to the 
 *       registered net_app_execute_func_t function callback.
 * 
 * @param[in] flags Opaque user-defined flags to be relayed to the application in the call to the 
 *       registered net_app_execute_func_t function callback.
 * 
 * A derived application should call this routine when it needs to break context from a timer or
 * thread callback. The call will return immediately, and execution will continue a short
 * time later from the context of the shared thread via a call to the application's
 * net_app_execute_func_t function callback.
 * 
 * An application will call this routine when it needs to
 * perform an iteration of its state machine loop after having 
 * updated its state from the context of a timer or network 
 * callback. 
 * 
 */
//void slp_app_execute(const net_app_handle_t hndl, void *data1, void *data2, uint32_t flags);
void slp_send_msg(UINT if_enum, slp_msg_t msg);
#endif // NET_SLP_H

