/*
 ***********************************************************
 *(c) Copyright Sindoh Co. Ltd. 
 *
 *        		 SINDOH Confidential
 * 
 ***********************************************************
 **/

/**
 *\file SysPosixTimer.h
 *
 *\brief Posix Timer defines.
 *
 **/

#ifndef SYS_POSIX_TIMER_H_
#define SYS_POSIX_TIMER_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * 
 *\brief Initialize timer
 *
 **/
void SPTInitializeTimer(void);

/**
 * 
 *\brief Shut timer down 
 *
 **/
void SPTShutdownTimer();

/**
 * 
 *\brief Set a periodic timer in seconds and u seconds. 
 *
 **/
void SPTSetPeriodicTimer(long secDelay, long usecDelay);

#ifdef __cplusplus
}
#endif

#endif //SYS_POSIX_TIMER_H_