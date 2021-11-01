/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <pthread.h>
#include <posix_ostools.h>
#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>
#else
#include "cpu_api.h"
#include "hwconfig_api.h"
#include "asic.h"
#include "ATypes.h"
#endif
#include "strfmt.h"
#include "lassert.h"
#include "dbg_outbuf.h"
#include "dbg_context.h"
#include "fwVersion.h"

#define SEND_MSG_SIGSEGV_FAIL		///<Send Sengmentation Fail message to UI
#define SEND_MSG_ASSERT_FAIL		///< Send Assert Fail message to UI
#define ASSERT_FAIL_FILE	"/app/nvram/sc_details.dat"	///< Send Assert Fail message to UI

#ifdef SEND_MSG_ASSERT_FAIL
#include "agMessage.h"
#include "agRouter.h"
#include "SysUI_Interface.h"
#include <string.h>
#include "SysServiceMgr.h"
#endif

// Note: jam_str() and jam_char() should move to the UART driver so we don't 
// have to include these driver-specific config files.
#ifdef HAVE_DWAPB_UART
#include "dwapb_uart_config.h"
#elif defined (HAVE_MRVL_PXA920_UART)
#include "mrvl_pxa920_uart_config.h"
#endif

extern void	StopAllPortsInEngine(void);
extern void watchdog_exit();
static void _reset_system(void);
static void _halt_system(void);

#ifdef __linux__

#define _MAX_FRAMES (20)    // max

static void _outstr(const char* s)
{
    printf("%s", s);
}

/*----------------------------------------------------------------------------*/
static void _outnum(unsigned int num, int decimal)
{
    if(decimal)
    {
        printf("%d", num);
    }
    else
    {
        printf("%8.8x", num);
    }
}

/*----------------------------------------------------------------------------*/
static void _flush_buffer(void)
{
  char c;
  dbg_outbuf_shutdown();
  while ((c = dbg_outbuf_get_char())) putc(c, stdout);
}

/* -------------------------------------------------------------------------- */
static void _display_context(void)
{
  void *array[_MAX_FRAMES];
  size_t size;

  if (dbg_context_is_thread())
  {
    _outstr("\n\rThread: ");
    _outstr("id-");
    _outnum(pthread_self(), 0);
  }
  else
  {
    _outstr("\n\rISR (or timer thread)");
  }
  _outstr("\n\rFW Version: ");
  _outstr(Get_Firmware_Version_String());
  _outstr("\n\rFW Build String: ");
  _outstr(Get_Build_Info_String());

  // get void*'s for all entries on the stack
  size = backtrace(array, _MAX_FRAMES);
  _outstr("\n\rStack Backtrace ");
  _outnum(size, 1);
  _outstr(" frames:\n\r");

  // print out all the frames to stderr
  backtrace_symbols_fd(array, size, 1);
  watchdog_exit();
  _reset_system();
}

void segfault_handler(int sig) 
{
	void *array[_MAX_FRAMES];
	size_t size;

	_outstr("\n\rSEGMENTATION FAULT");
	_outstr("\n\rFW Version: ");
	_outstr(Get_Firmware_Version_String());
	_outstr("\n\rFW Build String: ");
	_outstr(Get_Build_Info_String());

	// get void*'s for all entries on the stack
	size = backtrace(array, _MAX_FRAMES);
	_outstr("\n\rStack Backtrace ");
	_outnum(size, 1);
	_outstr(" frames:\n\r");

	// print out all the frames to stderr
	backtrace_symbols_fd(array, size, 1);

	//Send segmentation fail message to UI
#ifdef SEND_MSG_SIGSEGV_FAIL
	SendSCStatusToUI(SC_SYSTEM_SEG_FAULT, SYSTEM_MODULE_SCID);
	_halt_system();
	StopAllPortsInEngine();
#endif

	watchdog_exit();
	_reset_system();
}

#else
/*----------------------------------------------------------------------------*/
static void _jam_char(char c)
{
    /* jam characters into the uart, without using interrupts */
    static volatile UART_REGS_t* _reg = NULL;

    if (!_reg)
    {
        /* Try to get the uart base address for the debug uart */
        _reg=(volatile UART_REGS_t *)uartGetBaseAddr(uartGetDebugUart());
    }

    if (!_reg)
    {
        /* If we still do not have a base address then we must punt */
        return;
    }

    /* Wait for space in the shift register and then send the character */
    while (!(_reg->LSR & 0x20))
    {
        ; /* do nothing */
    }

    _reg->union0.THR = c;
}

/*----------------------------------------------------------------------------*/
static void _outchar(char c)
{
    /* Convert newline to carriage return + newline */
    if (c == '\n') _jam_char('\r');
    _jam_char(c);
}

/*----------------------------------------------------------------------------*/
static void _outstr(const char* s)
{
  while (*s) _outchar(*s++);
}

/*----------------------------------------------------------------------------*/
static void _outnum(unsigned int num, int decimal)
{
  char numstr[20];
  if (decimal) strfmt(numstr, "%d", num);
  else         strfmt(numstr, "%8.8x", num);
 _outstr(numstr);
}

/*----------------------------------------------------------------------------*/
static void _flush_buffer(void)
{
  char c;
  dbg_outbuf_shutdown();
  while ((c = dbg_outbuf_get_char())) _outchar(c);
}

/* -------------------------------------------------------------------------- */
static void _display_context(void)
{
unsigned long rambeg = hwGetRamStartAddress();
unsigned long ramend = rambeg + hwGetRamSize()-1;

#define _MAX_FRAMES (50)
  uintptr_t fp;
  uint32_t num=0;
  asm volatile ("mov %0, fp;" :"=r"(fp));
  extern TX_THREAD* _tx_thread_current_ptr;

  if (dbg_context_is_thread())
  {
    _outstr("\n\rThread: ");
    if (_tx_thread_current_ptr &&
        _tx_thread_current_ptr->tx_thread_name)
      _outstr(_tx_thread_current_ptr->tx_thread_name);
    else
      _outstr("(unknown)");
  }
  else
  {
    _outstr("\n\rISR (or timer thread)");
  }
  _outstr("\n\rFW Version: ");
  _outstr(Get_Firmware_Version_String());
  _outstr("\n\rFW Build String: ");
  _outstr(Get_Build_Info_String());
  _outstr("\n\rStack Backtrace:");
  while (fp && (++num < _MAX_FRAMES))
  {
    if ((fp<(rambeg+12)) || ((fp+4)>ramend))
    {
      _outstr("\n\rFP has gone out of RAM range: ");
      _outnum(fp, 0);
      break;
    }
    uintptr_t sp  = *((unsigned int*)(fp-8));
    uintptr_t ip  = *((unsigned int*)(fp  )) - 16;
    uintptr_t rip = *((unsigned int*)(fp-4)) - 4;
                 fp  = *((unsigned int*)(fp-12));
    _outstr("\n\r  SP:"); _outnum(sp, 0);
    _outstr(" IP:");      _outnum(ip, 0);
    _outstr(" RIP:");     _outnum(rip, 0);
  }
  if (num >= _MAX_FRAMES) _outstr("\n\rReally? - bailing out");
  _outstr("\n\r\n\r\n\r\n\r");
  _reset_system();
}

#endif // __linux__

/* -------------------------------------------------------------------------- */
#define _ATASSERT_MAX 20
static assertfunc _atassert_fns[_ATASSERT_MAX];
int atassert(assertfunc f)
{
  if (f)
  {
    int i;
    for (i=0;i<_ATASSERT_MAX;i++)
      if (!_atassert_fns[i])
      {
        _atassert_fns[i] = f;
        return 0;
      }
  }
  return -1;
}

/* -------------------------------------------------------------------------- */
static void _reset_system(void)
{
#ifdef RESET_ON_ASSERT /* Some projects may not have this defined */
#if    RESET_ON_ASSERT /* JTAG users may not want this */
#ifdef __linux__
        //??????? hwPlatformReset();  // reset the entire block
    abort();
#else
    hwPlatformReset();  // reset the entire block
#endif
#endif
#endif

#ifdef __linux__
    abort();
#else
    while (1);
#endif
}


/* -------------------------------------------------------------------------- */
static void _halt_system(void)
{
  int i;

  static int _halted = 0;
  if (_halted++)
  {
    _outstr("\n\rWhoops (double faulted?) !!!\n\r\n\r");
	watchdog_exit();
    _reset_system();
  }

#ifdef __linux__
//???????    uint32_t interrupts;
//???????    interrupts = cpu_disable_interrupts();
#else
    uint32_t interrupts;
    interrupts = cpu_disable_interrupts();
#endif

  for (i=0;i<_ATASSERT_MAX;i++)
    if (_atassert_fns[i])
      _atassert_fns[i]();

  _flush_buffer();
  _outstr("\n\rSystem Halted\n\r");

  _outstr("The following handlers have been called:\n\r");
  for (i=0;i<_ATASSERT_MAX;i++)
    if (_atassert_fns[i])
    {
      _outnum((uintptr_t)(_atassert_fns[i]), 0);
      _outstr("\n\r");
    }
  _outstr("\n\r");

#ifdef __linux__
//???????  cpu_restore_interrupts(interrupts);
#else
  cpu_restore_interrupts(interrupts);
#endif
}

/* -------------------------------------------------------------------------- */
void XAssertFail(const char *expression, const char *file, int line,
		unsigned long int errvalue)
{
#ifdef __linux__
	struct sched_param param;
	int policy;
	/* Boost thread priority to just below ISR during asserts. */
	pthread_getschedparam(pthread_self(), &policy, &param);
	param.sched_priority = POSIX_THR_PRI_HIGHEST;
	pthread_setschedparam(pthread_self(), policy, &param);
#endif

	_outstr("\n\rAssert: \""); _outstr(expression);
	_outstr("\" file:");       _outstr(file);
	_outstr(" line:");         _outnum(line, 1);
	_outstr("\n\rValue : ");   _outnum(errvalue, 0);
#ifdef SEND_MSG_ASSERT_FAIL
	//Send Assert Fail msg to UI Manager
	FILE* fd = fopen(ASSERT_FAIL_FILE, "w");
	if (fd) {
		fprintf(fd, "%s\n%s\n%d\n%s\n", "SC910", expression, line, file);
		fclose(fd);
	}else{
		_outstr(" Could not open file: ASSERT_FAIL_FILE \n" );
	}
	
	SendSCStatusToUI(SC_SYSTEM_ASSERT_FAIL, SYSTEM_MODULE_SCID);

	_halt_system();
	StopAllPortsInEngine();
#endif
	_display_context();
}

/* -------------------------------------------------------------------------- */
void _assert( const char *expression, const char *file, int line )
{
  XAssertFail(expression, file, line, 0);
}

/* -------------------------------------------------------------------------- */
void dbg_handle_exception(int vecnum, unsigned int lr)
{
  static const char* _veclabel[8] =
  {
    "0 : Reset (or NULL() dereference function call!)",
    "1 : Undefined instruction",
    "2 : Software interrupt (SWI)",
    "3 : Prefetch Abort (PC=LR-4)",
    "4 : Data Abort (PC=LR-8)",
    "5 : Reserved",
    "6 : Interrupt",
    "7 : Fast Interrupt"
  };
    
  _halt_system();
  _outstr("\n\rException: "); _outstr(_veclabel[vecnum&7]);
  _outstr(" : LR=");          _outnum(lr, 0);
  _display_context();
}
