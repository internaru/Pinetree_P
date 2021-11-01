/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>

#include "posix_ostools.h"
#include "agRouter.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"

#include "sys_init_api.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "ResourceMap.h"
#include "cmd_wrapper_api.h"

static char* _match = "@CMD";

/* -------------------------------------------------------------------------- */
static void _outdata(void* data, char* d, unsigned int len)
{
  IOReg* pipe = (IOReg*)data;
  char* buf = (char*)MEM_MALLOC(len);
  if (buf)
  {
    memcpy(buf, d, len);
    pipe->WriteConnect(pipe, buf, len);
    /* buf is freed by the WriteConnect when it is done with it */
  }
}

/* -------------------------------------------------------------------------- */
static void _pipeline_data(void* hdl, char *buffer, int len, IOReg* pipe)
{
  if (hdl) cmd_wrapper_pipeline(hdl, buffer, len, _outdata, pipe);
}

/* -------------------------------------------------------------------------- */
static SINT32 _parse_pipe(IOReg *pipe, const char* match)
{
  char buffer[20]; /* Must be able to contain UEL and/or match str */
  int i, n, m;
  void* hdl;

  /* First thing to do is to find the match. If it does not exist then we
     get out of here. If it does exist then we parse until we see UEL */
  m = strlen(match);
  for (i=0;i<m;i++)
  {
    n = pipe->ReadConnect(pipe, buffer+i, 1, PARSEREAD_SOME);
    if (n != 1) break;
    if (buffer[i] != match[i]) break;
  }
  if (i != m)
  {
    if (i > 0) pipe->RewindData(pipe, buffer, i);
    return PARSER_ERROR;
  }

  hdl = cmd_wrapper_create(match);

  /* We have found the match string.
     Now pass all data to the cmd pipeline until we see UEL */
  static const char _uel[] = "\x1b%-12345X"; /* <ESC>%-12345X */
  static int        _uellen = (sizeof(_uel)-1);
  i = 0;
  while (1)
  {
    n = pipe->ReadConnect(pipe, buffer+i, 1, PARSEREAD_DELAY);
    if (n != 1) break;

    if (buffer[i] == _uel[i])
    {
      if (++i == _uellen)
        break;
    }
    else
    {
      _pipeline_data(hdl, buffer, ++i, pipe);
      i = 0;
    }
  }
  _pipeline_data(hdl, ";", 1, pipe);
  if (hdl) cmd_wrapper_delete(hdl);
  return PARSER_SUCCESS;
}

/* -------------------------------------------------------------------------- */
static void *_thread_entry(void *parm)
{
  MESSAGE           msg;
  IOReg            *pipe = NULL;
  STARTRECIPE      *StartIt;
  mqd_t            queue =  (mqd_t)(uintptr_t)parm;
  int              err;

  SysWaitForInit ();
  while (1)
  {
    err = mq_receive( queue, (char*)&msg, sizeof(MESSAGE), 0); 
    if (err != sizeof(MESSAGE))
      continue;

    switch (msg.msgType)
    {
      default: break;

      case MSG_START_PARSE: /* connected */
      {
        /* Save pipe parameter */
        pipe = (IOReg *) msg.param3;

        /* Ack the start */
        if ((StartIt = (STARTRECIPE *)MEM_MALLOC(sizeof(STARTRECIPE))))
        {
          msg.msgType = MSG_STARTRECIPE;
          StartIt->Job = ejob_CMD;
          StartIt->AdditionalResource = e_CMDParser;
          StartIt->Pipe = NULL;
          StartIt->Wait = e_WaitForever;
          msg.param3 = StartIt;           // will be freed later
          SYMsgSend(SJMID, &msg);         // send the message.
        }
        break;
      }

      case MSG_ACKRECIPE:
      {
        /* Handle the pipe */
        ASSERT(pipe != NULL);
        SINT32 Response = _parse_pipe(pipe, _match);

        /* Free the resources */
        msg.msgType = MSG_FREERECIPE;
        SYMsgSend (SJMID, &msg); // param3 is untouched and now sent back.

        /* Let the parser know our response status */
        msg.msgType = MSG_END_PARSE;
        msg.param1 = Response;         // Parse Status.
        msg.param3 = pipe;
        SYMsgSend (CMPARSER, &msg);      // send the response.
        break;      // done
      }
    }
  }
  return 0;
}

void cmd_parser_init( void )
{
  static unsigned char _stack[POSIX_MIN_STACK_SIZE] __attribute__ ((aligned (8)));
  static pthread_t     _thread_id;
  static mqd_t        _queue;
  
  /* Initialize queue */
  posix_create_MESSAGE_queue( &_queue, "/CMDParseQ", 10 );

  /* Create Thread */
  posix_create_thread(&_thread_id, _thread_entry, (void*)(uintptr_t)_queue, "cmd_parser", _stack, sizeof(_stack), POSIX_THR_PRI_NORMAL);

  /* Register Parser */
  PARSE_SETUP Parser;
  Parser.ParserID       = CMDPARSER;
  Parser.ParseCmpString = _match;
  Parser.StringSize     = strlen(_match);
  Parser.ParseString    = "CMD";
  Parser.ParseType      = e_Print;
  ParserRegister(&Parser);

  /* Register thread as resource */
  rm_register(e_CMDParser, CMDPARSER);

  /* Register queue for this parser */
  router_register_queue(CMDPARSER, _queue);
}
