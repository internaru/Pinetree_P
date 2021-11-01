#ifndef DTH_API_H
#define DTH_API_H


#include <stdint.h>
#include <stdbool.h>

typedef struct dth_cfg_s 
{
    bool bypass; //< true to bypass
    uint8_t control_mode; //< 0 default 1bpp
    uint8_t bpp; // 1 or 2
    uint32_t thresh_width;
    uint32_t thresh_height;
    uint8_t *thresh_array;
} dth_cfg_t;


#ifndef ASIC_granite2  
// compile out the behaviour on asic's that don't have the block.
typedef void dth_cfg_t;
#define dth_setup( a, b ) 
#define dth_setup_regs() 
#define dth_reg0 0
#define dth_reg1 0
#define dth_reg2 0
#define dth_reg3 0

#else

#include "PIP_TOP_regstructs.h"
#include "PIP_TOP_regmasks.h"

extern DTH0_REGS_t *dth_reg0;
extern DTH0_REGS_t *dth_reg1;
extern DTH0_REGS_t *dth_reg2;
extern DTH0_REGS_t *dth_reg3;

void dth_setup( DTH0_REGS_t *dth_reg, dth_cfg_t *dth_cfg );
void dth_setup_regs( );

#endif // granite2
#endif // header


