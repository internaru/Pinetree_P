/* 
 * ============================================================================
 * Copyright (c) 2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "regAddrs.h"
#include "logger.h"
#include "utils.h" // str_mkint is here.
#include "apb_config_regstructs.h"
#include "apb_config_regmasks.h"
#include "hips_pll_api.h"
#include "hips_pll_config.h"
#include "memAPI.h"
#include "asic.h"
#include "debug.h"
#if defined( HAVE_CMD ) && defined( DEBUG )
#include "cmd_proc_api.h"
#endif
#include "uio_lib_api.h"

int hipsTestDebugCallback(int argc, char *argv[]);

pthread_mutex_t hips_lock = POSIX_MUTEX_INITIALIZER;

typedef struct hips_pll_device_private_s
{
    uint32_t channel;           ///< channel associated with this instance
    int32_t  open_count;        ///< number of current open handles to this instance
#ifdef __linux__
    int fd;                     ///< file descriptor for HiPS kernel driver
#endif
}hips_pll_device_private_t;

// NOTE: This is really the device handle.  Multiple device handles can
// point to the same private backing device since we allow multiple opens per device.
struct hips_pll_device_s
{
    hips_pll_device_private_t* my_dev;
};

hips_pll_device_private_t* hips_dev_private;

#ifdef __linux__
#include <stdio.h>
#define HIPS_DEV_BASE_NAME "hips"
#define MAX_LEN_HIPS_NAME 32

// on linux open of kernel driver handle enables the channel & close disables
static error_type_t hips_enable_channel(uint32_t channel)
{
    char drv_name[MAX_LEN_HIPS_NAME];
    snprintf(drv_name, MAX_LEN_HIPS_NAME, "/dev/%s%d", HIPS_DEV_BASE_NAME, channel);
    hips_dev_private[channel].fd = open(drv_name, O_RDWR|O_CLOEXEC);
    if(hips_dev_private[channel].fd < 0)
    {
        return FAIL;
    }
    return OK;
}

static inline error_type_t hips_disable_channel(uint32_t channel)
{
    close(hips_dev_private[channel].fd);
    hips_dev_private[channel].fd = -1;
    return OK;
}

#else // RTOS

static inline error_type_t hips_enable_channel(uint32_t channel)
{
    HIPS_REGS_t *pHIPS_REGS;
    pHIPS_REGS = hips_get_regs_base();
    pHIPS_REGS->Chan_Enable |= 1<<channel;  // enable the channel.
    return OK;
}

static inline error_type_t hips_disable_channel(uint32_t channel)
{
    HIPS_REGS_t *pHIPS_REGS;
    pHIPS_REGS = hips_get_regs_base();
    pHIPS_REGS->Chan_Enable &= ~(1<<channel);
    return OK;
}
#endif

error_type_t hips_pll_open(hips_pll_device_t **handle, uint32_t channel)
{
    error_type_t errcode = OK;

    *handle = MEM_MALLOC(sizeof(hips_pll_device_t));
    ASSERT(*handle != NULL); // big problem
    ASSERT(channel < hips_pll_num_chans());
    XASSERT(hips_dev_private[channel].channel == channel, channel);
    pthread_mutex_lock(&hips_lock);
    (*handle)->my_dev = &hips_dev_private[channel];

    errcode = hips_enable_channel(channel);
    if(errcode == OK)
    {
        hips_dev_private[channel].open_count++;
    }
    else
    {
        MEM_FREE_AND_NULL(*handle);        
    }

    pthread_mutex_unlock(&hips_lock);

    return errcode;
}

error_type_t hips_pll_close(hips_pll_device_t *handle)
{
    ASSERT(handle != NULL);
    ASSERT(handle->my_dev->channel < hips_pll_num_chans());
    ASSERT(handle->my_dev->open_count > 0);
    pthread_mutex_lock(&hips_lock);
    if(--handle->my_dev->open_count == 0)
    {
        // disable the channel when last open handle closes
        hips_disable_channel(handle->my_dev->channel);
    }
    pthread_mutex_unlock(&hips_lock);
    handle->my_dev = NULL;
    MEM_FREE_AND_NULL(handle);
    return OK;


}

 void hips_pll_init(void)
 {
     uint32_t i;
#if defined( HAVE_CMD ) && defined( DEBUG )
    int i_res;
#endif
#ifndef __linux__
    volatile APB_CONFIG_REGS_t *pllregs = (void*) APB_CONFIG_BASE;
#endif // __linux__

    pthread_mutex_lock(&hips_lock);
    hips_dev_private = MEM_MALLOC(hips_pll_num_chans()*sizeof(hips_pll_device_private_t));
    ASSERT(hips_dev_private != NULL);

    memset(hips_dev_private, 0, hips_pll_num_chans()*sizeof(hips_pll_device_private_t));
    for(i = 0; i < hips_pll_num_chans(); i++)
    {
        hips_dev_private[i].channel = i;
#ifdef __linux__
        hips_dev_private[i].fd = -1;
#endif
    }
    pthread_mutex_unlock(&hips_lock);
 
#ifdef HAVE_SERENITY
    // when running on the FPGA emulator we do not init the hips pll because
    // the block does not exist.
    return;
#endif

    // disable pu
#ifndef __linux__
    // on linux the hips PLL is initialized by u-boot to allow network capability in u-boot.
    // thus we only use hips driver on linux to enable/disable video channels.
    pllregs->PLL1_CR0 &= ~APB_CONFIG_PLL1_CR0_PU_MASK;   // disable it.

 

    pllregs->PLL1_CR0 = hips_get_cr0(); 

    pllregs->PLL1_CR1 = hips_get_cr1();

    pllregs->PLL1_CR2 = hips_get_cr2();  



    pllregs->PLL1_CR0 |= APB_CONFIG_PLL1_CR0_PU_MASK;   // turn on pu

    pllregs->PLL1_CR0 |= APB_CONFIG_PLL1_CR0_RESET_MASK;

    pllregs->PLL1_CR2 |= APB_CONFIG_PLL1_CR2_RESET_EXT_MASK;

    cpu_spin_delay(5);

    pllregs->PLL1_CR0 &= ~APB_CONFIG_PLL1_CR0_RESET_MASK;

    pllregs->PLL1_CR2 &= ~APB_CONFIG_PLL1_CR2_RESET_EXT_MASK;


/* EricH 08/18/11: After many tries, DaveG finally found the right bits to change
    to get scan clock dithered. Keep the code here.
 */
#define DITHER_SCAN_CLOCK 1 
#ifdef DITHER_SCAN_CLOCK 
    pllregs->PLL1_CR1 |= (1<<19);  // APB_CONFIG_PLL1_CR1_PI_EN_MASK;  

    pllregs->PLL1_CR1 &= ~(1<<23);

    pllregs->PLL1_CR3 |= (1<<19);

    pllregs->PLL1_CR0 |= APB_CONFIG_PLL1_CR0_BYPASS_EN_MASK;  
    pllregs->PLL1_CR0 &= ~APB_CONFIG_PLL1_CR0_PU_MASK;  

    pllregs->PLL1_CR2 = 0x050c5924;
    pllregs->PLL1_CR0 |= APB_CONFIG_PLL1_CR0_PU_MASK;  
    cpu_spin_delay(5);

    pllregs->PLL1_CR0 |= APB_CONFIG_PLL1_CR0_RESET_MASK;  
    pllregs->PLL1_CR2 = 0x850c5925;
    cpu_spin_delay(2);
    pllregs->PLL1_CR0 &= ~APB_CONFIG_PLL1_CR0_RESET_MASK;  
    pllregs->PLL1_CR2 = 0x050c5924;
#endif 

    cpu_spin_delay(100);

    while(!(pllregs->PLL1_SR & APB_CONFIG_PLL1_SR_LOCK_MASK)); // wait for the lock

#ifdef DITHER_SCAN_CLOCK 
    pllregs->PLL1_CR2 = 0x050c5926;
#endif

    cpu_spin_delay(2);

    pllregs->PLL1_CR0 &= ~APB_CONFIG_PLL1_CR0_BYPASS_EN_MASK;    // un-bypass
#endif //__linux__

#if defined( HAVE_CMD ) && defined( DEBUG )
    i_res = cmd_register( "hips",
                          NULL,
                          NULL,
                          NULL,
                          hipsTestDebugCallback );
    ASSERT( CMD_OK == i_res );
#endif
  }

#if defined( HAVE_CMD ) && defined( DEBUG )
static hips_pll_device_t *handle=NULL;

int hipsTestDebugCallback(int argc, char *argv[])
{
    uint32_t pos, value;
    HIPS_REGS_t *pHIPS_REGS;

    pHIPS_REGS = (HIPS_REGS_t*)MVDO_HIPS_BASE;
    error_type_t err;
    pos = 1;

    if ( strncmp( (char *)argv[pos], "open", 4 ) == 0 )
    {
        pos++;
        if (argc >= (pos+1))
        {
            err = str_mkint(argv[pos], &value);
            if(err != OK || (value > 7))
            {
                DPRINTF(DBG_SOFT|DBG_OUTPUT,("bad integer \"%s\"\n", argv[pos]));
                return FAIL;
            }
            hips_pll_open(&handle, value);
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("hips output complete\n"));
        }
    } else if (strncmp((char *)argv[pos], "close", 5) == 0)
    {
            hips_pll_close(handle);
            DPRINTF(DBG_SOFT|DBG_OUTPUT,("hips output complete\n"));
    } else if (strncmp((char *)argv[pos], "disp", 4) == 0)
    {
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("Hips 0x%x\n",pHIPS_REGS->Chan_Enable));
    } else if(strncmp((char*)argv[pos],"regs",4)==0)
    {    volatile APB_CONFIG_REGS_t *pllregs = (void*) APB_CONFIG_BASE;
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("PLL1_CR0= 0x%08x\n",   pllregs->PLL1_CR0));
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("PLL1_CR1= 0x%08x\n",   pllregs->PLL1_CR1));
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("PLL1_CR2= 0x%08x\n",   pllregs->PLL1_CR2));
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("PLL1_CR3= 0x%08x\n",   pllregs->PLL1_CR3));
    }else
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("Usage\n\topen\tvalue(0-7)\n\tclose\n\tdisp\tDisplay Value\n"));

    }
    return 0;
}

#endif

