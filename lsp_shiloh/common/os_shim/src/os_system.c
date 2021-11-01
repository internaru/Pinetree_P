 /*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
// os_system.c -- provides a wrapper for the general 'system' fork/exec call
//
//    waits for response from the completed offspring process.
//

// os headers
#include <stdio.h>
#include "posix_ostools.h" // thread_errno
#include "os_system.h" // waitpid()
#include "error_types.h" // OK

#define DBG_PRFX "os: "
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON
//off #define DBG_ON /* DBG_MSG */
#include "os_debug.h" // DBG_ family macros
#include <string.h> // strerror

// os_system() replacement for linux 'system()' call allows for extra-app
//    shell/process interaction without inheriting any open app file(s).
//
//    blocks waiting for req to finish (or error-exit)
//
//    returns OK (silently ignores) given a null request;
//    returns failed exec() status, or
//    returns the req's exit status (if system request is successful)
//
//  NOTE: Despite diligent usage of O_CLOEXEC and FD_CLOEXEC during file 
//        creation, several open descriptors slip through the fork/exec and
//        once duplicated in a restarted daemon, prove problematic for app
//        restarts -- The (current brute-force) solution is to close ALL 
//        currently open files post-fork pre-exec and provide stdin/out/err
//        to the offspring process.
//
error_type_t os_system(char* req)
{
    error_type_t ret = OK;
    pid_t pid;

    if (!req) return OK; // silently ignore null request

    DBG_VERBOSE("%s(%s)\n", __func__, req);

    pid = fork(); // spawn another process
    if (pid == -1)
    {
        return -1; // err-return if unsuccessful
    }

    // offspring handles the request -- parent-process returns the req's exit/err status

    if (pid == 0) // offspring
    {
        // export PATH=$PATH:/sbin prior to executing sh
        char embellished_req[1024] = { "export PATH=$PATH:/sbin; " };
        strncat(embellished_req, req, (1024-strlen(embellished_req)-1));
        if (strlen(embellished_req) >= 1023)
        {
            DBG_ERR("%s REQUEST OVERFLOW: %s\n", __func__, req);
        }
        char *os_req[] = { "sh", "-c", (char*)embellished_req, (char*)NULL };

        // close ALL residual open descriptors
        int fd = 0;
        int possibly_open = getdtablesize(); // max #of descriptors this process can have
        if (possibly_open > 0) // error possible (see also getrlimit())
        {
            while (fd < possibly_open)
            {
                close(fd++); // close if opened, ignored otherwise
            }
            /*  stdin  */ open("/dev/null", O_RDWR);
            /* sdtdout */ dup(0);
            /*  stderr */ dup(0);
        }
        else
        {
            DBG_ERR("%s(%s) (err %d)?\n", __func__, req, possibly_open);
        }

        execv("/bin/sh", (char* const*)os_req);
        exit(thread_errno); // only returns if exec() is unsuccessful
    }

    int status = 0;
    int options = WEXITED | WSTOPPED ; // ever WCONTINUED, WNOHANG, or WUNTRACED?
    int child_pid = pid; // store away

    // originating process (parent) waits for req exit-or-error
    int do_again = 1; // expect to iterate 1-time, but attempt a recovery otherwise
    if (do_again) // parent process waits for the offspring's results
    {
        siginfo_t child_info, *pid_info = &child_info;
        int err = waitid(P_PID, child_pid, pid_info, options);
        status = pid_info->si_status;
        if (err<0) // error
        {
            DBG_VERBOSE("%s() waitid(%d) err '%s' (%d)..\n", __func__, child_pid, strerror(thread_errno), thread_errno);
            do_again = true;
        }
        else do_again = 0; // this-time only ..

        // either child process was signalled (suspended?) ..
        if (WIFSIGNALED(status))
        {
            if (child_info.si_code == CLD_EXITED) // completion code in 'status'
            {
                DBG_VERBOSE("%s pid %d exit %d\n", req, child_pid, child_info.si_status);
                return child_info.si_status; // done -- return request-status
            }
            else // (revisit) a reasonable recovery strategy?
            {
                kill(child_pid, SIGCONT); // signal 'continue/wakeup' the offspring process
                do_again = waitpid(child_pid, &status, options);
                DBG_ERR("%s pid %d (exit %d signal %d)\n", req, child_pid, (WIFEXITED(status)), WIFSIGNALED(status));
            }
        }

        else // or child process was completed (terminated?)
        if (WIFEXITED(status))
        {
            if (child_info.si_code == CLD_EXITED) // exit status in 'status'
            {
                DBG_VERBOSE("%s exit pid %d exit %d\n", req, child_pid, child_info.si_status);
                return child_info.si_status; // done -- return request-status
            }
            else  // unexpected?
            {
                DBG_ERR("%s pid %d unexpected exit %d ..\n", req, child_pid, child_info.si_code);
                do_again = 1; // revisit any other possible recovery
            }
        }
    }

    // fell through ..
    if (WEXITSTATUS(status)) // anything unexpected happen?
    {
        DBG_MSG("%s(%s) (ret %d) returning %d\n", __func__, req, WEXITSTATUS(status), WEXITSTATUS(ret));
    }

    return WEXITSTATUS(ret);
}

// eof
