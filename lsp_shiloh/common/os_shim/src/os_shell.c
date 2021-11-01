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
//off #define DBG_VERBOSE_ON
#define DBG_ERR_ON
#include "os_debug.h" // DBG_ family macros
#include <string.h> // strerror

// os_shell() function for linux sh-like call allows returns stdout/stderr.
//
//    blocks waiting for req to finish (or error-exit)
//
error_type_t os_shell(char* req)
{
    error_type_t ret = OK;
    pid_t pid;

    if (!req) return OK; // silently ignore null request

    pid = fork(); // spawn another process
    if (pid == -1)
    {
        return -1; // err-return if unsuccessful
    }

    // offspring handles the request -- parent-process returns the req's exit/err status

    if (pid == 0) // offspring
    {
        // export PATH=$PATH:/sbin prior to executing sh
        char embellished_req[256] = { "export PATH=$PATH:/sbin; " };
        strcat(embellished_req, req);
        char *os_req[] = { "sh", "-c", (char*)embellished_req, (char*)NULL };

        int fd = 3; // retain stdin/stderr/stdout

        // close ALL residual open descriptors beyond stdin/stdout/stderr
        int possibly_open = getdtablesize(); // max #of descriptors this process can have
        if (possibly_open > 0) // error possible (see also getrlimit())
        {
            while (fd < possibly_open)
            {
                close(fd++); // close if opened, ignored otherwise
            }
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
    int do_again = 1; // expect to iterate 1-time, although possibly attempt a recovery..
    if (do_again) // parent process waits for the offspring's results
    {
        siginfo_t child_info, *pid_info = &child_info;
        int err = waitid(P_PID, child_pid, pid_info, options);
        status = pid_info->si_status;
        if (err<0) // error
        {
            DBG_VERBOSE("%s() waitid(%d) err '%s' (%d) ..\n", __func__, child_pid, strerror(thread_errno), thread_errno);
            do_again = 1;
        }
        else do_again = 0; // this-time only ..

        // either child process was signalled (suspended?) ..
        if (WIFSIGNALED(status))
        {
            //DBG_VERBOSE(" signaled! pid %d uid %d code %d signal %d status %d\n", 
            //  child_info.si_pid, child_info.si_uid, child_info.si_code, child_info.si_signo, child_info.si_status);
            if (child_info.si_code == CLD_EXITED) // done.. exit status in 'status'
            {
                DBG_VERBOSE("'%s' pid %d exit %d\n", req, child_pid, child_info.si_status);
                return child_info.si_status; // done -- return request-status
            }
            else // (revisit) another recovery strategy?
            {
                kill(child_pid, SIGCONT); // signal 'continue/wakeup' the offspring process
                do_again = waitpid(child_pid, &status, options);
                DBG_ERR("'%s' .. wait pid %d (exitted? %d signalled %d)\n", req, child_pid, (WIFEXITED(status)), WIFSIGNALED(status));
            }
        }

        else // or child process was completed (terminated?)
        if (WIFEXITED(status))
        {
            if (child_info.si_code == CLD_EXITED || child_info.si_code == CLD_KILLED) // completion code in 'status'
            {
                DBG_VERBOSE("'%s' exit pid %d exit %d\n", req, child_pid, child_info.si_status);
                return child_info.si_status; // done -- return request-status
            }
            else  // unexpected?
            {
                DBG_ERR("'%s' exit pid %d code %d unexpected!?\n", req, child_pid, child_info.si_code);
                do_again = 1; // revisit another possible recovery
            }
        }
    }

    // fell through ..
    if (WEXITSTATUS(status)) // anything unexpected occur?
    {
        DBG_VERBOSE("%s(%s) (ret %d) returning %d\n", __func__, req, WEXITSTATUS(status), WEXITSTATUS(ret));
    }

    return WEXITSTATUS(ret); // OK?
}

// eof
