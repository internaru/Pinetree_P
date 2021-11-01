/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <posix_ostools.h>
#include "lassert.h"
#include "strfmt.h"
#include "cmd_wrapper_api.h"
#include "cmd_cmd.h"
#include "cmd_private.h"
#include "cmd_cntxt.h"
#include "cmd_interp_api.h"

#define _MAGIC1 0x34de234a
#define _MAGIC2 0x1a2ce509

#ifndef CMD_STACK_SIZE
#error "CMD_STACK_SIZE must be defined"
#endif
#ifndef UNUSED_VAR 
 /*!
 * \def UNUSED_VAR(x) 
 * a simple macro that will turn off the unused var compiler warning for a given var
 **/
  #define UNUSED_VAR(x) ((x) = (x))  
 #endif 
/* -------------------------------------------------------------------------- */
typedef struct
{
    unsigned int                                magic1;
    char                                       *name;
    char                                       *thread_name;
    struct cmd_interp_hdl_t                    *interp;
    pthread_t                                   thread;
    sem_t                                       prod_sem;
    sem_t                                       cons_sem;
    sem_t                                       done_sem;
    const char                                 *cmd;
    cmd_wrapper_output_hdlr_fn_t                out;          /* client fnptr */
    void                                       *out_data;     /* client data */
    char                                        out_buf[256]; /* queued data */
    int                                         out_n;        /* Queued */
    int                                         retcode;
    int                                         cmdlevel;
    int                                         exit;
    __attribute__ ((aligned (8))) unsigned char stack[CMD_STACK_SIZE];
    unsigned int                                magic2;
} _wrapper_t;

/* -------------------------------------------------------------------------- */
static int _validate(void* hdl)
{
    _wrapper_t* wrapper = (_wrapper_t*)hdl;
    if (!wrapper)                   return -1;
    if (wrapper->magic1 != _MAGIC1) return -2;
    if (wrapper->magic2 != _MAGIC2) return -3;
    if (!wrapper->interp)           return -4;
    return 0;
}

/* -------------------------------------------------------------------------- */
static void _append_result(char c, void* arg)
{
    _wrapper_t* wrapper = (_wrapper_t*)arg;
    char str[3], *s = str;
    *s++ = c; if (c=='%') *s++ = c; *s++ = 0;
    cmd_interp_hdl_add_result(wrapper->interp, str);
}

/* -------------------------------------------------------------------------- */
int cmd_append_result(const char* fmt, ...)
{
    _wrapper_t* wrapper = (_wrapper_t*)cmd_cntxt_get();
    va_list ap;
    int len = 0;

    if (_validate(wrapper)) return 0;
    if (fmt)
    {
        va_start(ap, fmt);
        len = vfstrfmt(_append_result, wrapper, fmt, ap);
        va_end(ap);
    }
    return len;
}

/* -------------------------------------------------------------------------- */
static void _wrapper_outch(char c, void* data)
{
    _wrapper_t* wrapper = (_wrapper_t*)data;

    int flush = !c; /* Null chars will cause a flush of the output buffer */

    if (!flush)
    {
        wrapper->out_buf[wrapper->out_n++] = c;
        flush = (wrapper->out_n == sizeof(wrapper->out_buf));
    }
    if (flush && wrapper->out_n)
    {
        wrapper->out(wrapper->out_data, wrapper->out_buf, wrapper->out_n);
        wrapper->out_n = 0;
    }
}

/* -------------------------------------------------------------------------- */
static void _wrapper_flush_output(_wrapper_t* wrapper)
{
    _wrapper_outch(0, wrapper);
}

/* -------------------------------------------------------------------------- */
void cmd_output_sync(void)
{
    _wrapper_t* wrapper = (_wrapper_t*)cmd_cntxt_get();
    if (!_validate(wrapper)) _wrapper_flush_output(wrapper);
}

/* -------------------------------------------------------------------------- */
int cmd_printf(const char* fmt, ...)
{
    int len = 0;
    _wrapper_t* wrapper = (_wrapper_t*)cmd_cntxt_get();
    va_list ap;
    
    if (_validate(wrapper)) return 0;

    if (wrapper->out && fmt)
    {
        va_start(ap, fmt);
        len = vfstrfmt(_wrapper_outch, wrapper, fmt, ap);
        va_end(ap);
    }
    return len;
}

/* -------------------------------------------------------------------------- */
static int _invoke_command(cmd_cmd_t* cmd, int cmds, int argc, char* argv[])
{
    int r;

    if (cmds == 1)
    {
        r = cmd->proc(argc, argv);
    }
    else 
    {
        int cmdsize;
        char *argv0, *saved;
        for (cmdsize=r=0;r<cmds;r++)
            cmdsize += (strlen(argv[r])+1);
        argv0 = (char*)cmd_malloc(cmdsize);
        if (argv0)
        {
            argv0[0]=0;
            for (r=0;r<cmds;r++)
            {
                if (r) strcat(argv0, " ");
                strcat(argv0, argv[r]);
            }
            saved = argv[cmds-1];
            argv[cmds-1] = argv0;
            r = cmd->proc(argc-(cmds-1), argv+(cmds-1));
            argv[cmds-1] = saved;
            cmd_free(argv0);
        }
        else
            r = CMD_ERROR;
    }
    return r;
}

/* -------------------------------------------------------------------------- */
static cmd_cmd_t* _find_cmd(cmd_cmd_t* cmd, int* num, int argc, char* argv[])
{
    cmd_cmd_t* nextcmd;

    *num = 1; /* argv[0] is already matched, start with argv[1] */
    while (1)
    {
        if (cmd->proc)                                   return cmd;
        if ((*num+1) > argc)                             return cmd;
        if (!(nextcmd = cmd_cmd_find_cmd(argv, *num+1))) return cmd;
        *num += 1;
        cmd = nextcmd;
    }
}

/* -------------------------------------------------------------------------- */
static int _cmd_hook(void* d, int argc, char* argv[])
{
    int match, r;
    cmd_cmd_t *cmd;

    cmd = _find_cmd((cmd_cmd_t*)d, &match, argc, argv);
    if (cmd->proc) r = _invoke_command(cmd, match, argc, argv);
    else           r = CMD_USAGE_ERROR;

    if (r == CMD_USAGE_ERROR)
    {
        cmd_printf("ERROR: Usage: ");

        /* Display the portion of the command, as entered, that we matched */
        for (r=0;r<match;r++) cmd_printf("%s ", argv[r]);

        if (cmd->usage) /* Display usage if it was provided */
            cmd_printf("%s", cmd->usage);
        else if (cmd->subcmds) /* Display next set of options (if any) */
        {
            int need_pipe = 0;
            cmd = cmd->subcmds;
            while (cmd)
            {
                if (*cmd->name != '_')
                {
                    cmd_printf("%s%s", need_pipe?"|":"", cmd->name);
                    need_pipe = 1;
                }
                cmd=cmd->next;
            }
            if (!need_pipe) cmd_printf("(all subcommands are hidden)");
            cmd_printf(" ...");
        }
        else  /* Bad command author - no subcmds, no usage, nothing */
            cmd_printf(" ...");
        cmd_printf("\n");
    }
    return r;
}

/* -------------------------------------------------------------------------- */
static void _register(cmd_cmd_t* cmd, struct cmd_interp_hdl_t* arg)
{
    cmd_interp_hdl_add_cmd(arg, cmd->name, cmd);
}

/* -------------------------------------------------------------------------- */
static void *_tcl_wrapper_entry(void *arg)
{
    const char* result;
    _wrapper_t* wrapper = (_wrapper_t*)arg;

    cmd_cntxt_set(wrapper);
    while (1)
    {
        if (0 != sem_wait(&wrapper->cons_sem))
            break;
            
        if (wrapper->exit)
        {
            sem_post(&wrapper->done_sem);
            break;
        }
        wrapper->cmdlevel = cmd_cmd_get_new_cmds(_register, wrapper->interp,
                                                 wrapper->cmdlevel);
        wrapper->retcode = cmd_interp_hdl_run(wrapper->interp,wrapper->cmd,&result);
        if (result && *result) cmd_printf(result, ""); // LSPTODO

        // This is added to the output to allow port 7359 to be utilized by Perl
        // Telnet module. The Perl Telnet transaction requires a known 
        // terminating prompt. This was chosen to be benign whitespace output.
        cmd_printf("\r\n\r\n");  

        _wrapper_flush_output(wrapper);
        sem_post(&wrapper->done_sem);
    }
    cmd_cntxt_erase();
    wrapper->exit = 2;
    
    return NULL;
}

/* -------------------------------------------------------------------------- */
static int _wrapper_runcmd(_wrapper_t* wrapper, const char* cmd,
                           cmd_wrapper_output_hdlr_fn_t out, void* client_data)
{
    int px_status = 0; UNUSED_VAR(px_status);
    int policy;
    struct sched_param param;
    
    px_status = pthread_getschedparam(pthread_self(), &policy, &param);
    XASSERT( px_status==0, px_status );

    wrapper->out      = out;
    wrapper->out_data = client_data;
    wrapper->cmd      = cmd;
    
    // we only adjust if the thread is following our system standard policy.
    // on linux the main thread does not follow the policy unless the process was
    // started with root access.
    if(policy == POSIX_DEFAULT_SCHED_POLICY)
    {
        /* Elevate (or set) the output thread priority to match our own */
        px_status = pthread_setschedparam(wrapper->thread, policy, &param);
        XASSERT( px_status==0, px_status );
    }
    
    sem_post(&wrapper->cons_sem);
    if (0 != sem_wait(&wrapper->done_sem))
        return CMD_ERROR;
    return wrapper->retcode?CMD_ERROR:CMD_OK;
}

/* -------------------------------------------------------------------------- */
static int _runcmd(void* hdl, const char* cmd,
                   cmd_wrapper_output_hdlr_fn_t out, void* client_data)
{
    int r;
    _wrapper_t* wrapper = (_wrapper_t*)hdl;
    if (_validate(hdl)) return CMD_ERROR;
    if (!cmd || !*cmd)  return CMD_ERROR;
    if (0 != sem_trywait(&wrapper->prod_sem))
    {
        if (_validate(hdl)) return CMD_ERROR;
        sem_wait(&wrapper->prod_sem);
    }
    if (_validate(hdl))
    {
        r = CMD_ERROR;
    }
    else
    {
        r = _wrapper_runcmd(wrapper, cmd, out, client_data);
    }
    sem_post(&wrapper->prod_sem);
    return r;
}

/* -------------------------------------------------------------------------- */
int cmd_wrapper_runcmd(void* hdl, const char* cmd,
                       cmd_wrapper_output_hdlr_fn_t out, void* client_data)
{
    return _runcmd(hdl, cmd, out, client_data);
}

/* -------------------------------------------------------------------------- */
typedef struct { char* ptr; int cur, len, lap; } _buf_t;

/* -------------------------------------------------------------------------- */
static void _strout(void* arg, char* data, unsigned int len)
{
    _buf_t* b = (_buf_t*)arg;
    while (len--)
    {
        b->ptr[b->cur++] = *data++;
        if (b->cur >= b->len)
        {
            b->cur = 0;
            b->lap = 1;
        }
    }
}

/* -------------------------------------------------------------------------- */
static void _rotate_r(char* a, int l, int s)
{
    while (1)
    {
        if (!s || !a || l < 2) return; /* Exit conditions */
        if      ( 2*s > l) s = -(l-s); /* change from rotate right to rotate left */
        else if (-2*s > l) s =   l+s;  /* change from roatte left to rotate right */
        else
        {
            int i;
            int S = (s>0)?s:-s; /* What is the absolute value of s - this is S */
            char* b = a+l-S;  /* Set up a pointer to the 'right' side of the array */
            for (i=0;i<S;i++) /* Swap S elements from 'right' side to 'left' side */
            {
                char t = a[i];
                a[i] = b[i];
                b[i] = t;
            }
            if (s>0) a += s; /* freeze the 'left' side of the array */
            else     ;       /* The 'right' side is frozen by the shortening (next) */
            l = l-S;         /* Shorten the array by the amount we've swapped */
        }
    }
}

/* -------------------------------------------------------------------------- */
int cmd_wrapper_run(void* hdl, const char* cmd, char* outstr, int len)
{
    if (!outstr) return _runcmd(hdl, cmd, NULL, 0);

    _buf_t o = {.ptr=outstr, .cur=0, .lap=0, .len=len};
    int r    = _runcmd(hdl, cmd, _strout, &o);
    _strout(&o, "", 1);
    if (o.lap && o.cur) /* We need to rotate outstr */
        _rotate_r(outstr, len, len-o.cur);
    return r;
}

/* -------------------------------------------------------------------------- */
int cmd_wrapper_pipeline(void* hdl, const char* indata, int inlen,
                         cmd_wrapper_output_hdlr_fn_t out, void* client_data)
{
    const char* cmd;
    int c, r = CMD_OK;
    _wrapper_t* wrapper = (_wrapper_t*)hdl;
    if (_validate(hdl)) return CMD_ERROR;
    if (0 != sem_trywait(&wrapper->prod_sem))
    {
        if (_validate(hdl)) return CMD_ERROR;
        sem_wait(&wrapper->prod_sem);
    }
    if (_validate(hdl))
    {
        r = CMD_ERROR;
    }
    else
    {
        while (inlen)
        {
            c = cmd_interp_hdl_parse_cmd(wrapper->interp, indata, inlen, &cmd);
            indata += c;
            inlen -= c;
            if (cmd && *cmd)
                if (CMD_OK != (r = _wrapper_runcmd(wrapper, cmd, out, client_data)))
                    break;
        }
    }
    sem_post(&wrapper->prod_sem);
    return r;
}

/* -------------------------------------------------------------------------- */
void* cmd_wrapper_create(const char* name)
{
    cmd_interp_register_cmd_hdlr(_cmd_hook); /* This can be done globally */

    if (!name || !*name) return NULL;
    _wrapper_t* wrapper = (_wrapper_t*)cmd_malloc(sizeof(_wrapper_t));
    if (!wrapper) return NULL;
    memset(wrapper, 0, sizeof(*wrapper));
    wrapper->interp = (struct cmd_interp_hdl_t*)cmd_malloc(cmd_interp_hdl_size());
    if (!wrapper->interp)
    {
        cmd_free(wrapper);
        return NULL;
    }
    if (cmd_interp_hdl_create(wrapper->interp))
    {
        cmd_free(wrapper->interp);
        cmd_free(wrapper);
    }
    wrapper->thread_name = (char*)cmd_malloc(5+strlen(name)); /* CMD:+<name>+0 */
    if (!wrapper->thread_name)
    {
        cmd_interp_hdl_delete(wrapper->interp);
        cmd_free(wrapper->interp);
        cmd_free(wrapper);
        return NULL;
    }
    strfmt(wrapper->thread_name, "CMD:%s", name);
    wrapper->name = wrapper->thread_name + 4;

    wrapper->exit = 0;

    if(0 != sem_init(&wrapper->cons_sem, 0, 0))
    {
        goto ERR_EXIT;
    }

    if(0 != sem_init(&wrapper->prod_sem, 0, 0))
    {
        sem_destroy(&wrapper->cons_sem);
        goto ERR_EXIT;
    }

    if(0 != sem_init(&wrapper->done_sem, 0, 0))
    {
        sem_destroy(&wrapper->cons_sem);
        sem_destroy(&wrapper->prod_sem);
        goto ERR_EXIT;
    }

    if ((0 != posix_create_thread(&wrapper->thread, _tcl_wrapper_entry, wrapper,
                                  wrapper->thread_name, wrapper->stack, sizeof(wrapper->stack), POSIX_THR_PRI_SERIAL_IN)))
    {   
        sem_destroy(&wrapper->cons_sem);
        sem_destroy(&wrapper->prod_sem);
        sem_destroy(&wrapper->done_sem);
        goto ERR_EXIT;
    }
    wrapper->magic1 = _MAGIC1;
    wrapper->magic2 = _MAGIC2;
    sem_post(&wrapper->prod_sem);
    return wrapper;
ERR_EXIT:
    cmd_interp_hdl_delete(wrapper->interp);
    cmd_free(wrapper->interp);
    cmd_free(wrapper->thread_name);
    cmd_free(wrapper);
    return NULL;
}

/* -------------------------------------------------------------------------- */
int cmd_wrapper_delete(void* hdl)
{
    _wrapper_t* wrapper = (_wrapper_t*)hdl;
    if (_validate(hdl)) return -1;
    sem_wait(&wrapper->prod_sem);

    /* Mark the interpreter as invalid for use */
    wrapper->magic2 = _MAGIC1;
    wrapper->magic1 = _MAGIC2;

    /* Cause the thread to exit */
    wrapper->exit = 1;
    sem_post(&wrapper->cons_sem);
    sem_wait(&wrapper->done_sem);

    /* Thread has exited - now we free everything */
    pthread_detach(wrapper->thread);
    sem_destroy(&wrapper->done_sem);
    sem_destroy(&wrapper->cons_sem);
    sem_destroy(&wrapper->prod_sem);
    cmd_interp_hdl_delete(wrapper->interp);
    cmd_free(wrapper->interp);
    cmd_free(wrapper->thread_name);
    cmd_free(wrapper);

    return 0;
}

/* -------------------------------------------------------------------------- */
static const char* _cmd_store_usage = "[+]<filename> <cmd>";
static const char* _cmd_store_notes = "Store the results of the command "
  "execution in the specified filename. If filename is preceded by a + sign "
  "then the results are added to any contents already in the file. As a matter "
  "of practical detail, if <cmd> includes argmuments then the entire <cmd> "
  "string should be specified inside of double quotes. For example: "
  "\"puts {This is a test}\"";
static void _store_file(void* f, char* data, unsigned int data_len)
{
    fwrite(data, data_len, 1, (FILE*)f);
}
int _cmd_store(int argc, char* argv[])
{
    _wrapper_t* wrapper = (_wrapper_t*)cmd_cntxt_get();
    if (_validate(wrapper)) return CMD_ERROR;
    if (argc != 3) return CMD_USAGE_ERROR;

    FILE* fhdl;
    char* fname;
    cmd_wrapper_output_hdlr_fn_t out;
    void *out_data;
    int r;

    if (*(argv[1]) == '+') fhdl = fopen((fname = argv[1]+1), "a");
    else                   fhdl = fopen((fname = argv[1]),   "w");
    if (!fhdl)
    {
        cmd_printf("Failed to open file %s\n", fname);
        return CMD_ERROR;
    }

    _wrapper_flush_output(wrapper);
    out               = wrapper->out;
    out_data          = wrapper->out_data;
    wrapper->out      = _store_file;
    wrapper->out_data = fhdl;
    r = cmd_interp_hdl_run(wrapper->interp, argv[2], NULL);
    _wrapper_flush_output(wrapper);
    wrapper->out      = out;
    wrapper->out_data = out_data;
    fclose(fhdl);
    return r?CMD_ERROR:CMD_OK;
}

/* -------------------------------------------------------------------------- */
void cmd_wrapper_init(void)
{
    cmd_register("cmd store",
                 "Store (or append) the results of a command in a file",
                 _cmd_store_usage, _cmd_store_notes, _cmd_store);
}
