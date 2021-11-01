/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 
/** 
 * \file sigma_interface.h
 * 
 * \brief Code to tie the WFA Sigma code into our system
 * 
 * Prototypes for routines we implement to mimic system calls,
 * etc.
 * 
 **/

#ifndef INC_SIGMA_INTERFACE_H
#define INC_SIGMA_INTERFACE_H

#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RW 2

#define SIGALRM 0
#define SIG_IGN 0

typedef void (*sig_t) (int);

void startSigma(void);
void sigma_cmd_handler(char *request_data, int request_len, char *response_buffer, int *response_len);
void wfa_signal(int sigNum, sig_t sigFunc);
void wfa_sigalrm_timer_func(unsigned long unused);
void wfa_alarm(int sec);

#endif // ifndef INC_SIGMA_INTERFACE_H
