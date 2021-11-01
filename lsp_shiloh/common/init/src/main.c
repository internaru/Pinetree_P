/*
 *
 * ============================================================================
 * Copyright (c) 2008-2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file main.c
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#ifdef __linux__
#include <signal.h>
#endif
#include "pthread.h"
#include "posix_ostools.h"
#include "sys_init_api.h"
#define SPEC_H_MAIN
#include "spec.h"
#undef	SPEC_H_MAIN
#include "SysInit.h"
#include "error_types.h"
#include "fwVersion.h"
#include "debug.h"
#include "dprintf.h"
#include "logger.h"
#include "lassert.h"
#include "memAPI.h"
#include "hwconfig_api.h"
#include "spec.h"

#if 0
#include "cpu_api.h"
#include "hwconfig_api.h"
#include "ATypes.h"
#include "interrupt_api.h"

#include "sys_init_api.h"
#include "dprintf.h"
#include "logger.h"
#include "SJMgr.h"
#endif

#ifdef HAVE_SMTP
#include <curl/curl.h> 
#endif

#ifdef HAVE_UNIT_TEST
#include "unit_test_api.h"
#endif
#ifdef HAVE_POWER_MGR
#include "pwr_mgr_api.h"

static bool low_power_clk_mode(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd);
static bool low_power_mem_mode(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd);
static uint32_t power_mgr_event, power_mgr_mem_event;
#endif

void settimeofday_tz();

#define STARTUP_STACK_SIZE POSIX_MIN_STACK_SIZE
/*!< address in lcm for the bootstatus value. */
#define BOOTSTATUS_OFFSET 0x3d200024

#define INTNUM_TICK_10MS 80

/* function defined by ThreadX that needs to be called on time tick interrupt */
extern void _tx_timer_interrupt( uint32_t int_num );


/* Keep track of the official start of free memory. Used later to create the
 * main byte pool for memory allocations.
 * NOTE: Need to know the end of memory as well, to allow determining the size
 * of the byte pool. This will be product dependent, and can be determined
 * by reading GPIO pins. See the boot code for an example implementation. */
//LSPTODO static volatile uint32_t* freeMemoryStart, memAvailable;
static volatile uint32_t memAvailable;


#ifdef __linux__

// Currently using a triggered wait cond to exit the main loop.  There are
// probably better ways to do this, but this method is expedient for the time
// being.  Could also wait on a semaphore instead of cond.
// Alternatives would be to join on a master process thread and signal all
// threads to exit.  Or kill all threads.
pthread_mutex_t main_loop_mutex = POSIX_MUTEX_INITIALIZER;
pthread_cond_t  main_loop_cond  = PTHREAD_COND_INITIALIZER;
bool            main_loop_exit = false;    ///< true if main loop should exit

void signal_exit(void)
{
    pthread_mutex_lock(&main_loop_mutex);
    main_loop_exit = true;  
    pthread_cond_broadcast(&main_loop_cond);
    pthread_mutex_unlock(&main_loop_mutex);
}


#ifdef DEBUG
extern void mlimiter_test(void);
#endif

#include "stdlib.h"
void test_memory()
{
#ifdef DEBUG
  mlimiter_test();
#endif  
  void *leak = MEM_MALLOC(1345);
  void *leak2 = MEM_MALLOC(787);
  void *mallocleak = malloc(2545);

  leak = 0;
  printf( "malloc'ed\n");
  memFree(mallocleak);  // simple check for valid pointer can be fooled valgrind will complain about the illegal read
  mallocleak = 0; // 
  mallocleak = MEM_REALLOC(mallocleak, 42555);
  mallocleak = MEM_REALLOC(mallocleak, 4255);
  mallocleak = MEM_REALLOC(mallocleak, 455);
  mallocleak = MEM_REALLOC(mallocleak, 178881);
  mallocleak = MEM_REALLOC_LIMITED(mlimiter_by_name("print"), mallocleak, 424);

  mallocleak = 0;
  // free(leak2); // segfault will be caught by valgrind :)
  leak2 = 0;


}

#include "pipe_io_device.h"
extern void pipe_io_device_test(void);
void test_usb_pipe(void)
{
  pipe_io_device_test();
}

int main(int argc, char **argv)
{
    int retcode;

    struct sched_param sched_param = {0};

    printf("==> main\n");

    hwConfigInit(); // this must run before cpu_init because some features inited here are used in cpu

#ifdef LSPTODO
    // This is all the stuff we used to do before sysinit.  Does any of it make sense
    // in the lsp?  Probably not, but leaving it ifdef'd out for the moment
    // until we bring more of the system up and review further.
    cpu_init();

    /* initialize and enable the interrupt subsystem */
    intInit();

    /* set up the RTOS time tick */
    intAttach( INTNUM_TICK_10MS, 0, _tx_timer_interrupt, INTNUM_TICK_10MS );
    intEnable( INTNUM_TICK_10MS );

    freeMemoryStart = (uint32_t *)pFreeMem;
    ram_start = hwGetRamStartAddress();
    ram_size = hwGetRamSize();  

    ASSERT( freeMemoryStart > (uint32_t*)ram_start);
    ASSERT( ram_start + ram_size > (uint32_t)pFreeMem );

    /* tell the debug module where DRAM ends for the pointer validity macros;
     * -1 since we want the value of the last *valid* memory address.
     */
    debug_set_dram_end( ram_start + ram_size - 1 );

    /* Available memory is a function of which product, which environment. */
    memAvailable = ram_start + ram_size - (uint32_t)pFreeMem - hwGetUncachedRamSize();
    if ( hwGetUncachedRamSize() > 0)
    {
        // uncached memory must be initialized before cached.
        memInitUncached((uint32_t*)hwGetUncachedRamStartAddress(), hwGetUncachedRamSize());
    }

    // since the posix layer is essentially part of the OS we init it in tx_application_define
    // rather than in SysInitSystem.  This allows the rest of the system including the drivers
    // to assume that posix is ready to go.
    freeMemoryStart = posix_initialize((void*)freeMemoryStart);
    memAvailable -= (uint32_t)((uintptr_t)freeMemoryStart - (uintptr_t)pFreeMem);

    memInitMemory((uint32_t*)freeMemoryStart, memAvailable);

#ifdef HAVE_ACL
    /* get and save the boot status for later. */
    BootStatusValue = *((uint32_t *)BOOTSTATUS_OFFSET);
    AclSetStatus(BootStatusValue);
#endif
#endif // LSPTODO
    uint32_t mem_total = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE);
    uint32_t mem_allowed;
    memAvailable = sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE);

    // LSPTODO: ideally the mem_allowed should be a calculated value based on available memory
    // with PDL, SCAN, and main system requirements subtracted off
    mem_allowed = MIN(memAvailable, (SYSTEM_LIMITER_MAX_MB*1024*1024));
//    mem_allowed = MIN(memAvailable, (128*1024*1024));

    printf("%s: Total Mem = %d, memAvailable = %d; limiters allowed = %d\n", __func__, mem_total, memAvailable, mem_allowed);
    memInitMemory(0, mem_allowed);
    // try to set the main thread to our standard system policy and priority
    // note that this will work only if the process is started with root access.
    // that will typically be the case on a real system, but not if testing on
    // a linux pc.  hence we ignore the return code.
    sched_param.sched_priority = POSIX_THR_PRI_NORMAL;
    retcode = pthread_setschedparam(pthread_self(), POSIX_DEFAULT_SCHED_POLICY, &sched_param);




    if(SysInitSystem(NULL) == 0)
    {
extern void check_segmentation_fault(char *str, int line);
		check_segmentation_fault((char *)__FUNCTION__, __LINE__);

        printf("SysInitSystem done.  Waiting...");
        //LSPTODO hack in some testing...
        // test_memory();
        test_usb_pipe();

        pthread_mutex_lock(&main_loop_mutex);
        while(main_loop_exit == false)
        {
            pthread_cond_wait(&main_loop_cond, &main_loop_mutex);
        }
        pthread_mutex_unlock(&main_loop_mutex);
    }

    printf("<== main\n");
    return 0;
}

#else
typedef void (*func_ptr) (void);        /* Pointer to void function type. */
extern func_ptr __CTOR_LIST__[];        /* C++ static/global constructor list */


/**
 *
 * \brief Every program needs a main.
 *
 */
int main( void )
{
    unsigned long nptrs = (unsigned long) __CTOR_LIST__[0];
    unsigned i;

    /* Call C++ static/global constructors */
    for (i = nptrs; i >= 1; i--)
    {
        __CTOR_LIST__[i]();
    }

    //
    // In the idle loop we go to sleep waiting for an interrupt.  This bit has to be
    // set to allow us to break into a cpu that is sleeping, with jtag.
    //
    // if we have wfi
    // *REG_CPU_CCR |= 0x80000000; // turn on the pclk_wfi_en bit so we can break into this code    
    
    hwConfigInit(); // this must run before cpu_init because some features inited here are used in cpu
    cpu_init();

    /* This routine does not return */
    tx_kernel_enter();

    /* assert if it ever does! */
    ASSERT(0);

    return -1;
}


static pthread_t sysinit_tid;
static char sys_init_stack[STARTUP_STACK_SIZE] ALIGN8;
error_type_t AclSetStatus(uint32_t Status);


/**
 * \brief This function is the operating system's starting point.
 *
 * Our system initialization starts here and continues in SysInitSystem
 *
 * \param pFreeMem The system's pointer to the first free memory location
 * to be used for our system heap.
 *
 */
void tx_application_define( char *pFreeMem )
{
    uint32_t ram_start;
    uint32_t ram_size;
#ifdef HAVE_ACL
    uint32_t BootStatusValue;
#endif

    /* initialize and enable the interrupt subsystem */
    intInit();

    /* set up the RTOS time tick */
    intAttach( INTNUM_TICK_10MS, 0, _tx_timer_interrupt, INTNUM_TICK_10MS );
    intEnable( INTNUM_TICK_10MS );

    freeMemoryStart = (uint32_t *)pFreeMem;
    ram_start = hwGetRamStartAddress();
    ram_size = hwGetRamSize();  

    ASSERT( freeMemoryStart > (uint32_t*)ram_start);
    ASSERT( ram_start + ram_size > (uint32_t)pFreeMem );

    /* tell the debug module where DRAM ends for the pointer validity macros;
     * -1 since we want the value of the last *valid* memory address.
     */
    debug_set_dram_end( ram_start + ram_size - 1 );

    /* Available memory is a function of which product, which environment. */
    memAvailable = ram_start + ram_size - (uint32_t)pFreeMem - hwGetUncachedRamSize();
    if ( hwGetUncachedRamSize() > 0)
    {
        // uncached memory must be initialized before cached.
        memInitUncached((uint32_t*)hwGetUncachedRamStartAddress(), hwGetUncachedRamSize());
    }

    // since the posix layer is essentially part of the OS we init it in tx_application_define
    // rather than in SysInitSystem.  This allows the rest of the system including the drivers
    // to assume that posix is ready to go.
    freeMemoryStart = posix_initialize((void*)freeMemoryStart);
    memAvailable -= (uint32_t)((uintptr_t)freeMemoryStart - (uintptr_t)pFreeMem);

    memInitMemory((uint32_t*)freeMemoryStart, memAvailable);

    posix_create_thread(&sysinit_tid, SysInitSystem, 0, "sys_init", sys_init_stack, STARTUP_STACK_SIZE, POSIX_THR_PRI_NORMAL);
#ifdef HAVE_ACL
    /* get and save the boot status for later. */
    BootStatusValue = *((uint32_t *)BOOTSTATUS_OFFSET);
    AclSetStatus(BootStatusValue);
#endif
}
#endif

pid_t initprocess(char *const argv[]) {
	pid_t new_pid;

	//Creating New process
	new_pid = fork();
	if ( new_pid == -1 ) { 
		return -1; 
	} else if (new_pid == 0) {
		//Child process and no error.
		if (execvp (argv[0], argv) == -1) {
			return -1; 
		}   
		return -2; 
	}   
	return new_pid;
}

/**
 * Function name: SysInitSystem
 *
 * \brief This function initializes every system module in an order-dependent
 * fashion
 *
 * \param dummy An unused variable
 *
 */
void* SysInitSystem(void* dummy)
{
    error_type_t status;
    /* The following function calls and macros may be placed in a dependency
     * order. Therefore, be aware when changing the order! Note that the
     * macros (typically denoted by all caps) are defined in sysInit.h, and
     * may not exist if the feature is not present.
     */
	machine_type_init();

	settimeofday_tz();
    INITIALIZE_CMD();
    fw_version_init();
//LSPTODO    INITIALIZE_BOARD_PADS();
    INITIALIZE_MEMORY_LOG();
    INITIALIZE_UART();
    dbg_console_task_init();

    /* start the timer driver. */
    INITIALIZE_TIMER();
    INITIALIZE_HIPS_PLL();      // turn on the second pll, if needed.
    /* At this point, can now printf without a buffer; strings will be sent to
     * console debug task to be asynchronously stuffed out debug console port.
     */
    INITIALIZE_WATCHDOG();
    SysInitInit();
    memory_cmd_init();  // must be called after cmd
    INITIALIZE_LOGGER();

#ifdef __linux__
      signal(SIGSEGV, segfault_handler);   // install segfault handler
#endif

    INITIALIZE_UIO_LIB();
    INITIALIZE_GPIO();
    INITIALIZE_EFUSE();
    INITIALIZE_GPIO_LED();
    INITIALIZE_SWITCHED_PWR();
    INITIALIZE_SPI();
    INITIALIZE_SD();
    INITIALIZE_CDMA();
    INITIALIZE_UFS();
    INITIALIZE_DATAFS();
    INITIALIZE_SPIMEM();
    INITIALIZE_SYSTEM_MSG_ROUTER();
    INITIALIZE_SYSTEM_RESOURCE_MGR();
    INITIALIZE_SYSTEM_JOB_MGR();
    INITIALIZE_CONNECTION_MGR();
    INITIALIZE_POWER_MGR();
    INITIALIZE_POWER_ASIC();
    INITIALIZE_POWER_CPU();

//LSPTODO    hwConfigDebugInit();
    INITIALIZE_NAND();
    INITIALIZE_NAND_PLUS();
    INITIALIZE_BOOTI2C();
    /* EEPROM requires that booti2c exist and be initialized first */
    INITIALIZE_EEPROM();
    INITIALIZE_BOOTSPI();

    INITIALIZE_SYS_UI_SENDER_INIT();	//Initialize Msg Queue SYS to UI

    status = INITIALIZE_NVRAM();

    INITIALIZE_ACL();
    INITIALIZE_CMD_PARSER();
    INITIALIZE_PLATFORM_VARS();

    /* do USB phy init before the client or host stacks */
    /* Note also that the DEC blocks depend on the usb phy clock. */
    /* Accessing dec registers without the phy clocks running */
    /* will cause a bus hang. */
    INITIALIZE_USB_PHY();

    if (status == OK)
    {
        /* only init this stuff if the nvm is running. */
        INITIALIZE_STRING_MGR();
        INITIALIZE_DELAY_MGR();
        INITIALIZE_PWM();
        INITIALIZE_DEC();
        INITIALIZE_DEC_SENSOR();
        INITIALIZE_DEC_ADC();
        INITIALIZE_DEC_FUSER();
        INITIALIZE_DEC_LASER();
        INITIALIZE_STEPPER();
        INITIALIZE_DCMOTOR();
        INITIALIZE_CONTROL_PANEL();
        INITIALIZE_UI();
        INITIALIZE_KINOMA();
        INITIALIZE_PRINT();
        INITIALIZE_ENGINE();
        INITIALIZE_VIDEO();

        INITIALIZE_INTERNAL_PAGE_MGR();
        INITIALIZE_AUDIO();
        INITIALIZE_ZJ_PARSER();
        INITIALIZE_PF_PARSER();
        INITIALIZE_PCFAX_PARSER();
        INITIALIZE_URF_PARSER();
        INITIALIZE_HB_PARSER();
        INITIALIZE_GGS_PARSER();
    }

    INITIALIZE_USB_GADGET();
    INITIALIZE_USB_HOST_PARSER();


    INITIALIZE_USB_CLIENT();

    if (status == OK)
    {
        /* only init this if the nvm stuff is running. */
        INITIALIZE_SCANNER();
        INITIALIZE_COPY();
        INITIALIZE_COPY_TO_HOST();
        /* USB Host application blocks, like pictbridge, must be initialized
         * before the USB HOST, because they register a startup function with
         * the USB HOST */
        INITIALIZE_PICTBRIDGE();
        INITIALIZE_MASS_STORAGE();
        INITIALIZE_MASS_STORAGE_REFLECTOR();
        INITIALIZE_USB_HOST();
        INITIALIZE_MASS_STORAGE_DEVICE();
        /* Media manager must be initialized after all media source drivers
         * have been initialized */
        INITIALIZE_MEDIA_MANAGER();
        INITIALIZE_PHOTO();
        INITIALIZE_OID();
        INITIALIZE_COMMON_CONSUMABLES();
	INITIALIZE_PJL();
        /* init wireless card before init network */
        INITIALIZE_RAW_SOCKETS();
// network links are now enumerated directly from network interface init
//        INITIALIZE_ETHERNET();
//        INITIALIZE_WIRELESS();
        INITIALIZE_NETWORK();
        INITIALIZE_MDNS();
        INITIALIZE_NBNS();
        INITIALIZE_SLP();
        INITIALIZE_SNMP();
        INITIALIZE_SNTP();
        INITIALIZE_LLTD();
 
        /* initialize HTTP after network */
        INITIALIZE_HTTP();
        INITIALIZE_DEBUG_OVER_HTTP();
        INITIALIZE_WSD_PRINT();
        INITIALIZE_WSD_SCAN();
        INITIALIZE_WSD();
        INITIALIZE_GCPP(); // INITIALIZE_CURL() needed here
        INITIALIZE_RTC();
        INITIALIZE_FILESYSTEM();
        INITIALIZE_FAT_FILESYSTEM();
#ifndef HAVE_SDHFAX
        INITIALIZE_FAX();
#else
        if (IS_FAX_ATTACHED)
        {
            INITIALIZE_SDH_FAX();
        }
#endif
        INITIALIZE_PRODPJL();
        INITIALIZE_PROFILE();
        INITIALIZE_STRMGR();
        INITIALIZE_PAWN();
        INITIALIZE_JPEG_SUPPORT();
        INITIALIZE_JPEG_HW();
        INITIALIZE_JBIG();
        INITIALIZE_MIP();
#ifdef HAVE_SMTP
    curl_global_init(CURL_GLOBAL_ALL); // rather than INITIALIZE_CURL();
#endif
        INITIALIZE_EMAIL();
        INITIALIZE_EMAIL_APPS();
        INITIALIZE_TEXT_PAD();
        INITIALIZE_ODMA();
        INITIALIZE_PIP();
        INITIALIZE_COMPRESSION();
        INITIALIZE_ACL_DEV_PROTOCOL();
        INITIALIZE_STATISTICS();
#ifdef HAVE_POWER_MGR
        void rtc_register_pwr_event(void);
        rtc_register_pwr_event();

        power_mgr_event = pwr_mgr_reg_callback("SYSTIM", low_power_clk_mode,pwr_mgr_pri_high_e);
        pwr_mgr_set_module_pwr_level("SYSTIM", pwr_mgr_lowest_power_e);
        power_mgr_mem_event = pwr_mgr_reg_callback("SYSMEM",low_power_mem_mode, pwr_mgr_pri_highest_e);
        pwr_mgr_set_module_pwr_level("SYSMEM",pwr_mgr_lowest_power_e);

        void cpu_register_pwr_event(void);
        cpu_register_pwr_event();
        pwr_mgr_set_enable_pwr_mgmt(true);
        pwr_mgr_set_sys_status_online(true);
#endif

        INITIALIZE_UTILS();
        INITIALIZE_Z_APPS();
        INITIALIZE_NET_SM();
        INITIALIZE_SM_JOB();
        INITIALIZE_IPP();
        INITIALIZE_PAPER_SIZE_API();
        INITIALIZE_GENERATORS();
        INITIALIZE_SYS_SERVICE();
#ifdef HAVE_IMAGELOG
		INITIALIZE_IMAGELOG();
#endif
#ifdef HAVE_SCANBOX
        INITIALIZE_SCANBOX();
#endif
#ifdef HAVE_WIFI_SETTER
		INITIALIZE_WIFI_SETTER();
#endif
#ifdef HAVE_IFAX
		INITIALIZE_IFAX();
#endif


#if 0		//Launch UIManager from /etc/profile for KARA-S and appBrowser for PINTREE-S.
#if 1	 //KARA-S and PINETREE        
        char *argv[] = {"/app/UIManager", "-qws", "-nomouse", NULL};
        //char *argv[] = {"/app/UIManager_KaraS", "-qws", "-display", "LinuxFB:/dev/fb1",  "-nomouse", NULL};
        initprocess(argv);
#else	//PINETREE-S
	char *argv[] = {"/app/UIManager_PS", NULL};
	initprocess(argv);
	
	//char *argvBrowser[] = {"/app/appBrowser", "file:///home/GUI/home.html", "-platform", "linuxfb", "-plugin", "Tslib:/dev/event0", "-plugin", "sedevkeyboard",   NULL};
	char *argvBrowser[] = {"/app/appBrowser", "file:///home/GUI/home.html", "-platform", "linuxfb", "-plugin", "evdevtouch:/dev/event0", "-plugin", "sedevkeyboard",   NULL};
	initprocess(argvBrowser);        
#endif
#endif
    }
    else
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("!!!!!!!!!!!ERROR!!!!!!!!!!  nvram is not initialized\n"));
    }

     INITIALIZE_TINY_DEBUGGER();
     INITIALIZE_CARD_READER();
    /* must be done after all parsers register so we use correct values. */
    INITIALIZE_PLATFORM_1284_STRING();
    INITIALIZE_UNIT_TEST();
    #ifdef HAVE_UNIT_TEST
    //LSPTODO cpu_test_init();
    #endif

    /*  When a firmware download command is received, this thread will
     *  be resumed and firmware shutdown should commence.
     *  At this point, all job I/O has been shut down, so no more jobs
     *  will be received.
     *  Before shutting down, we wait until there are no more jobs
     *  present.
     */ 
    SysInitDone(INITDONE_0);
    return 0;
}


/**
 *
 * \brief Force a reset
 *
 */
void PlatformReset(void)
{
    /* shut down interrupts */
//LSPTODO    cpu_disable_interrupts();
    SHUTDOWN_USB_CLIENT();
    SHUTDOWN_ENGINE();

    /* goto reset vector */
//LSPTODO    hwPlatformReset();
    signal_exit();
}


/**
 * Function name: SysShutdownSystem
 *
 * \brief Shutdown system modules.  This is blocking.
 */
void SysShutdownSystem(void)
{
    /* Shutdown UI to start user notification of shut down sequence */
    SHUTDOWN_UI();
//LSPTODO    SysJbMgrShutdown(); 
    SHUTDOWN_ENGINE();
    SHUTDOWN_PLATFORM_VARS();

    /* Insert your module shutdown function here. */


    /* Call nvram shutdown last to writeback all changes. */
    SHUTDOWN_NVRAM();
    sleep(2);
}
#ifdef HAVE_POWER_MGR
static bool low_power_clk_mode(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd)
{
    switch(level)
    {
        case  pwr_mgr_on_e:
            intEnable(INTNUM_TICK_10MS);
            pwr_mgr_clear_event_powered_down(power_mgr_event);
            break;
        case pwr_mgr_lowest_power_e:
            intDisable( INTNUM_TICK_10MS ); // fall through
        default:
            pwr_mgr_set_event_powered_down(power_mgr_event);
            break;
    }

    return true;
}
void go_to_low_pwr(void);
static bool low_power_mem_mode(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd)
{
    switch(level)
    {
        case pwr_mgr_on_e:
            pwr_mgr_clear_event_powered_down(power_mgr_mem_event);
            break;
        case pwr_mgr_lowest_power_e:
            go_to_low_pwr();    // falls through to the next case.
            pwr_mgr_set_event_powered_down(power_mgr_mem_event);
            pwr_mgr_goto_active_state(0);   // turn everything back on
            break;
        default:
            pwr_mgr_set_event_powered_down(power_mgr_mem_event);
            break;
    }
    return true;
}
#endif

void machine_type_init(void)
{
	gTFTLCDAttached = 0;
	gScannerAttached = 0;
	gFaxAttached = 0;

	char *m_type = NULL;
	m_type = getenv("M_TYPE");
	if(m_type != NULL){
		int nMachineType = atoi(m_type);
		if(nMachineType == MTYPE_KARAS) {
			gMachineType = MTYPE_KARAS;
		}
		else if(nMachineType == MTYPE_PINETREE) {
			gMachineType = MTYPE_PINETREE;
			gScannerAttached = 1;
			gFaxAttached = 1;
		}
		else if(nMachineType == MTYPE_PINETREES) {
			gMachineType = MTYPE_PINETREES;
			gScannerAttached = 1;	
			gFaxAttached = 1;
		}
		else if(nMachineType == MTYPE_ABBA2H) {
			gMachineType = MTYPE_ABBA2H;
		}
		else if(nMachineType == MTYPE_OAK2M) {
			gMachineType = MTYPE_OAK2M;
			gScannerAttached = 1;
			gFaxAttached = 1;
		}
		else if(nMachineType == MTYPE_OAK2H) {
			gMachineType = MTYPE_OAK2H;
			gScannerAttached = 1;
			gFaxAttached = 1;
		}
		else if(nMachineType == MTYPE_JUNIPERC) {
			gMachineType = MTYPE_JUNIPERC;
			gScannerAttached = 1;
			//gFaxAttached = 1;
		}
		else if(nMachineType == MTYPE_JUNIPERS) {
			gMachineType = MTYPE_JUNIPERS;
			gScannerAttached = 1;
			//gFaxAttached = 1;
		}
		else if(nMachineType == MTYPE_PINETREEL) {
			gMachineType = MTYPE_PINETREEL;
			gScannerAttached = 1;
			gFaxAttached = 1;
		}
		else {
			printf("Wrong MachineType %d\n", nMachineType);
		}
	}

	if(access("/dev/event0", F_OK) == 0){
		printf("Touch device: /dev/event0\n");
		gTFTLCDAttached = 1;
	}

//	printf("gMachineType: %d\n", gMachineType);
}

void settimeofday_tz(void)
{
	struct timeval tv;
	struct timezone tz; 

	gettimeofday(&tv, &tz);
extern void get_new_tz_info(struct timezone *ptz);
    get_new_tz_info(&tz);
	settimeofday(&tv, &tz);  
}
