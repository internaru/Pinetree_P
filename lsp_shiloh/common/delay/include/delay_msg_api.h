/*
 * ============================================================================
 * Copyright (c) 2008-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef delay_msg_api_H
#define delay_msg_api_H

#include "agRouter.h"
/**
 * DMNotify is based of the message queue timeout and therefore
 * is only accurate to ticks (10msec). When multiple
 * messages are sent to the task, it's error could compound.
 * 
 * If a task registers two DMNotify's with the same modID and
 * uiParam, the the Unregister will always delete the oldest.
 * 
 * DMNotify can only send send a message to the well known queue
 * based on ModID.
 * 
 * It should only be used if there is no alternative.  The
 * delay_msec_timer interface is the preferred option.  Using
 * tx_timers directly is the next best option.
 *
 * @deprecated replaced by delay_msec_timer_non_blocking()
 */

/*! \note This is the old delay_mgr API, new code should use the new API, not this */
/**
 * \brief By calling this function a thread can request either a single
 * MSG_DELAY_NOTIFY message to be delivered back to that thread's
 * mailbox after an arbitrary delay (bRepeat=FALSE) or else a
 * continual stream of such messages (bRepeat=TRUE) until a future
 * call to DMNotifyUnregister().
 *
 * \note A thread can register for multiple delay notifications
 * as long as each subscription has a unique uiParam.
 *
 * \param[in] modID
 * \param[in] nTicks
 * \param[in] bRepeat
 * \param[in] uiParam
 */
bool DMNotifyRegister( MODULE_ID modID, uint32_t nTicks, bool bRepeat, uint32_t uiParam );

/*! \note This is the old delay_mgr API, new code should use the new API, not this */
/**
 * \brief Unregister for MSG_DELAY_NOTIFY messages.
 *
 * If a task registers two DMNotify's with the same modID and
 * uiParam, the the Unregister will always delete the oldest.
 *
 * \param[in] modID
 * \param[in] uiParam
 */
bool DMNotifyUnregister( MODULE_ID modID, uint32_t uiParam );

#endif 
