/*
 ***************************************************************************** 
 * Copyright (c) 2013  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ****************************************************************************** 
 */
/**
 * \file os_system.h
 *
 * \brief Posix/linux replacement function for general 'system' fork/exec.
 *    Allows daemon restarts without propogating open file descriptors.
 *
 **/

#ifndef _LINUX_SYSTEM_WRAPPER_H
#define _LINUX_SYSTEM_WRAPPER_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // waitpid()

int os_system(char* shell_request); // synchronous wait for an exit/err response
int os_shell(char* shell_request); // includes stdout/stderr results from sh

#endif // _LINUX_SYSTEM_WRAPPER_H eof
