/*
 *
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file dbg_api.h
 *
 * \brief This is the interface to the dbg module
 *
 * The dbg module provides a few services as enhancements to the same services
 * provided by the debug module. For example, dbg allows non-blocking dbg_printf
 * which allows dbg_printf's to be generated from timers and ISRs.
 *
 * Dbg does not replace debug but, on projects where it is included, replaces
 * some of the services.
 *
 **/

#ifndef DBG_API_H
#define DBG_API_H

#ifdef __cplusplus
extern "C" {
#endif
 
/**
 * \brief A version of printf that outputs to the console (serial, telnet,etc.)
 * \param[in] fmt The printf formatter string.
 * \param[in] ... The variable argument list.
 * \returns int 
 * \retval The number of bytes formatted
 *
 * Note that dbg_printf_control() will affect the blocking/non-blocking nature
 *   of this function. Formatting is always done under the caller's context.
 *   Non-blockable callers (timers and ISRs) will always be non-blocking
 * 
 * Note that dbg_printf_control() may cause all output from this function to
 *   be discarded before it reaches either the output buffer or any actual
 *   output destinations.
 */
int dbg_printf(const char* fmt, ...);

/**
 * \brief Controls the behavior of dbg_printf()
 * \param[in] enable   If enable == 0 then all dbg_printf is discarded before
 *                     it is buffered for any output destination.
 * \param[in] blocking if blocking == 0 then output flushing is deferred to
 *                     a background task (i.e. dbg_printf's are non-blocking)
 */
void dbg_printf_control(int enable, int blocking);

/**
 * \brief Controls the stamping of time stamp on dbg_printf_messages
 * \param[in] enable  If == 0 then no time stamping will occur. Otherwise all
                      dbg_printf() messages will be time stamped
 */
void dbg_printf_timestamp(int enable);

/**
 * \brief Controls the stamping of thread name on dbg_printf_messages
 * \param[in] enable  If == 0 then no name stamping will occur. Otherwise all
                      dbg_printf() messages will be stamped with the thread name
 */
void dbg_printf_namestamp(int enable);

/**
 * \brief Causes execution to stop until all dbg_printf data has been flushed
 *        to the output device.
 *
 * Note that this will also momentarily elevate the priority of the
 * "background" outbuf task to the same priority of the caller.
 *
 * Note that calling this from a non-blockable context (timer or ISR) results
 * in a non-operation (i.e. immediate return).
 */
void dbg_sync(void);


/**
 * \brief This is the type for debug watchers and replayers callback handlers
 *
 * This callback function will be with output data of a specified length.
 */
typedef void (*dbg_receiver_cb_t)(char* data, int len, void* arg);

/**
 * \brief Allows the entire output buffer to be replayed to the caller
 *        provided function pointer.
 * \param[in] cb  The output buffer will be replayed to this function.
 * \param[in] arg This is an opaque pointer provided to the caller's function
 *                  on each callback.
 *
 * With this command you can retrieve the contents of the most recent outbuf
 * data. Note that such replay is not synchronized with debug generation so
 * there may be a small amount of corruption if debug generation catches up
 * with the replayer (which seems unlikely under normal conditions).
 *
 * The cb is invoked under the caller's context
 */
void dbg_outbuf_replay(dbg_receiver_cb_t cb, void* arg);

/**
 * \brief Allows interested watchers to register for debug output data.
 * \param[in] cb  The output buffer will be played to this function as
 *                data becomes available.
 * \param[in] arg This is an opaque pointer provided to the caller's function
 *                  on each callback.
 * \returns   void*
 * \retval    An opaque handle to be used for unregistering a specific watcher
 *
 * With this command you can watch the entire debug output data for any purpose
 * you'd like. Callback functions that block will cause all debug output in
 * the system to be backed up. This may be acceptable but caution is encouraged.
 *
 * The cb is invoked under a dbg context. This function is itself non-blocking.
 */
void* dbg_watcher_register(dbg_receiver_cb_t cb, void* arg);

/**
 * \brief Allows watchers to unregister themselves.
 * \param[in] handle This is the handle obtained from a dbg_watcher_register()
 *                   invocation. Its contents should not have been modified in
 *                   the intervening time between calls. Once a watcher has
 *                   unregistered the handle should not be used again.
 */
void  dbg_watcher_unregister(void* handle);

#ifdef __cplusplus
}
#endif

#endif /* DBG_API_H */
