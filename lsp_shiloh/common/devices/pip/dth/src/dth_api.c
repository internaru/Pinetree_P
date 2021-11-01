#include "map_mem.h"
#include "dth_api.h"
#include "posix_ostools.h"
#include "regAddrs.h"
#include "lassert.h"
#include "logger.h"
#include "dma_buffer.h"
#include "asic.h"


#define DBG_PRFX "PIP_DTH: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

uint8_t dth_spiral_1_16_thresharray[16] = { 
    7*16,  8*16,  9*16, 10*16,
    6*16,  1*16,  2*16, 11*16,
    5*16,  4*16,  3*16, 12*16,
     255, 15*16, 14*16, 13*16,
};

dth_cfg_t dth_1_16_cfg = {
    .bypass = 0, //< true to bypass
    .control_mode = 0, //< 1bpp
    .bpp = 1, //< 1 or 2
    .thresh_width = 4,
    .thresh_height = 4, 
    .thresh_array = dth_spiral_1_16_thresharray,

};

DTH0_REGS_t *dth_reg0 = 0;
DTH0_REGS_t *dth_reg1 = 0;
DTH0_REGS_t *dth_reg2 = 0;
DTH0_REGS_t *dth_reg3 = 0;

void dth_setup_regs( )
{
    int fd;
    void *mapBase; // page and structure addresses may be different, we don't unmap which would require keeping the base or computing it.

    if((fd = open("/dev/mem", (O_RDWR | O_SYNC | O_CLOEXEC))) < 0) {
        DBG_PRINTF_CRIT("pip_init()  open('dev/mem') failed ");
        ASSERT(fd == 0);
    }
    dth_reg0 = (DTH0_REGS_t *)mapMem( IPS_PIP_DTH0_BASE, sizeof(DTH0_REGS_t), &mapBase, fd );
    dth_reg1 = (DTH0_REGS_t *)mapMem( IPS_PIP_DTH1_BASE, sizeof(DTH0_REGS_t), &mapBase, fd );
    dth_reg2 = (DTH0_REGS_t *)mapMem( IPS_PIP_DTH2_BASE, sizeof(DTH0_REGS_t), &mapBase, fd );
    dth_reg3 = (DTH0_REGS_t *)mapMem( IPS_PIP_DTH3_BASE, sizeof(DTH0_REGS_t), &mapBase, fd );
    close(fd);
}
/// todo: only 1bpp is currently programmed.  
void dth_setup( DTH0_REGS_t *dth_reg, dth_cfg_t *cfg )
{

    uint32_t tmp;

    if (cfg == 0 || cfg->bypass) { 
	DBG_ASSERT(dth_reg);
	dth_reg->CTRL = REPLACE_VAL(DTH0, CTRL, BYPASSALL, dth_reg->CTRL, 1);
	return;
    }
    DBG_ASSERT(dth_reg);
    dth_reg->CTRL = REPLACE_VAL(DTH0, CTRL, MODE, dth_reg->CTRL, cfg->control_mode);
    dth_reg->CTRL = REPLACE_VAL(DTH0, CTRL, BYPASSALL, dth_reg->CTRL, 0);  

    dth_reg->THRESH_ARRAY_WIDTH = REPLACE_VAL(DTH0, THRESH_ARRAY_WIDTH, WIDTH, 
					      dth_reg->THRESH_ARRAY_WIDTH, cfg->thresh_width);

    uint32_t thresh_size = cfg->thresh_width * cfg->thresh_height;
    BigBuffer_t *buffer = dma_buffer_copy_from( cfg->thresh_array, thresh_size);
    void *addr = dma_buffer_map_single(buffer, DMA_TO_DEVICE);
    
    dth_reg->THAR = REPLACE_VAL(DTH0, THAR, ADDRESS, dth_reg->THAR, addr);
		
    tmp = REPLACE_VAL(DTH0, DTH_PARAM0, TH_SRAM_WIDTH, dth_reg->DTH_PARAM0, cfg->thresh_width);  
    tmp = REPLACE_VAL(DTH0, DTH_PARAM0, TH_SRAM_DEPTH, tmp, thresh_size);  
    tmp = REPLACE_VAL(DTH0, DTH_PARAM0, DT_WIDTH, tmp, cfg->bpp);  
    tmp = REPLACE_VAL(DTH0, DTH_PARAM0, OUT_CH_WIDTH, tmp, 8);  
    tmp = REPLACE_VAL(DTH0, DTH_PARAM0, IN_CH_WIDTH, tmp, 8);  
    dth_reg->DTH_PARAM0 = tmp;

    /// todo: no support for deinterleave 

}
