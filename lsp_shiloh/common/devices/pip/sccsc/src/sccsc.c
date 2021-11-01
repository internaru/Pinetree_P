/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the Sub-cube CSC block.
  * 
 **/

#ifndef __SCCSC_FUNC_C_INCLUDED
#define __SCCSC_FUNC_C_INCLUDED

#include <stdint.h>
#include "sccsc_api.h"
#include "logger.h"

#define DBG_PRFX "SCCSC: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

uint64_t sccsc_lut_ram[8][688];  // Separate array for each RAM

void sccsc_setup(SCCSC_REGS_t *sc_csc_reg,
                 sccsc_cfg_t *sccsc_cfg,
                 uint64_t *ilut,
                 uint64_t *cachelut,
                 uint32_t cpuaccess,
                 uint32_t hc,
                 uint32_t base_address)
{
    uint32_t reg_value;
    uint32_t cachelutsize;

    // Write the CNTRL Register
    reg_value = sc_csc_reg->CNTRL;
    reg_value = SCCSC_CNTRL_BYPASSALL_REPLACE_VAL(reg_value,sccsc_cfg->bypassall);
    reg_value = SCCSC_CNTRL_CPUACCESS_REPLACE_VAL(reg_value,cpuaccess);
    reg_value = SCCSC_CNTRL_IALG_REPLACE_VAL(reg_value,sccsc_cfg->ialg);
    reg_value = SCCSC_CNTRL_NGE_REPLACE_VAL(reg_value,sccsc_cfg->nge);
    reg_value = SCCSC_CNTRL_WME_REPLACE_VAL(reg_value,sccsc_cfg->wme);
    reg_value = SCCSC_CNTRL_KME_REPLACE_VAL(reg_value,sccsc_cfg->kme);
    reg_value = SCCSC_CNTRL_LUTSEL_REPLACE_VAL(reg_value,sccsc_cfg->lutsel);
    reg_value = SCCSC_CNTRL_SBAND_EN_REPLACE_VAL(reg_value,sccsc_cfg->sband_en);
    reg_value = SCCSC_CNTRL_SBAND_VALUE_REPLACE_VAL(reg_value,sccsc_cfg->sband_value);
    reg_value = SCCSC_CNTRL_SBAND_MASK_REPLACE_VAL(reg_value,sccsc_cfg->sband_mask);
    sc_csc_reg->CNTRL = reg_value; 

    // Write the CACHE CNTRL Register
    reg_value = sc_csc_reg->CACHE_CNTRL;
    reg_value = SCCSC_CACHE_CNTRL_LUTSIZE_REPLACE_VAL(reg_value,sccsc_cfg->lutsize);
    reg_value = SCCSC_CACHE_CNTRL_HC_REPLACE_VAL(reg_value,hc);
    sc_csc_reg->CACHE_CNTRL = reg_value;

    // Write the BASE ADDR Register
    reg_value = sc_csc_reg->BASE_ADDR;
    reg_value = SCCSC_BASE_ADDR_ADDR_REPLACE_VAL(reg_value,base_address);
    sc_csc_reg->BASE_ADDR = reg_value;

    // Write the White Threshold Register
    reg_value = sc_csc_reg->WT;
    reg_value = SCCSC_WT_CH0_WT_REPLACE_VAL(reg_value,sccsc_cfg->wt[0]);
    reg_value = SCCSC_WT_CH1_WT_REPLACE_VAL(reg_value,sccsc_cfg->wt[1]);
    reg_value = SCCSC_WT_CH2_WT_REPLACE_VAL(reg_value,sccsc_cfg->wt[2]);
    sc_csc_reg->WT = reg_value;

    // Write the White Results Register
    reg_value = sc_csc_reg->WR_REG0;
    reg_value = SCCSC_WR_REG0_CH0_WR_REPLACE_VAL(reg_value,sccsc_cfg->wr[0]);
    reg_value = SCCSC_WR_REG0_CH1_WR_REPLACE_VAL(reg_value,sccsc_cfg->wr[1]);
    reg_value = SCCSC_WR_REG0_CH2_WR_REPLACE_VAL(reg_value,sccsc_cfg->wr[2]);
    sc_csc_reg->WR_REG0 = reg_value;

    // Write the White Results Register
    reg_value = sc_csc_reg->WR_REG1;
    reg_value = SCCSC_WR_REG1_CH3_WR_REPLACE_VAL(reg_value,sccsc_cfg->wr[3]);
    reg_value = SCCSC_WR_REG1_CH4_WR_REPLACE_VAL(reg_value,sccsc_cfg->wr[4]);
    reg_value = SCCSC_WR_REG1_CH5_WR_REPLACE_VAL(reg_value,sccsc_cfg->wr[5]);
    sc_csc_reg->WR_REG1 = reg_value;

    // Write the Black Results Register
    reg_value = sc_csc_reg->KR_REG0;
    reg_value = SCCSC_KR_REG0_CH0_KR_REPLACE_VAL(reg_value,sccsc_cfg->kr[0]);
    reg_value = SCCSC_KR_REG0_CH1_KR_REPLACE_VAL(reg_value,sccsc_cfg->kr[1]);
    reg_value = SCCSC_KR_REG0_CH2_KR_REPLACE_VAL(reg_value,sccsc_cfg->kr[2]);
    sc_csc_reg->KR_REG0 = reg_value;

    // Write the Black Results Register
    reg_value = sc_csc_reg->KR_REG1;
    reg_value = SCCSC_KR_REG1_CH3_KR_REPLACE_VAL(reg_value,sccsc_cfg->kr[3]);
    reg_value = SCCSC_KR_REG1_CH4_KR_REPLACE_VAL(reg_value,sccsc_cfg->kr[4]);
    reg_value = SCCSC_KR_REG1_CH5_KR_REPLACE_VAL(reg_value,sccsc_cfg->kr[5]);
    sc_csc_reg->KR_REG1 = reg_value;

    //Setup to use iLut if selected and enabled...
    if( !sccsc_cfg->bypassall && (!sccsc_cfg->lutsel || sccsc_cfg->sband_en))
    {
        //Put the lut data into the RAMs
        sccsc_loadilut(sc_csc_reg, 1, ilut);
    }

    //Setup to use cache if its selected
    if( !sccsc_cfg->bypassall && (sccsc_cfg->lutsel || sccsc_cfg->sband_en) )
    {
        //Figure out the needed cache lut size
        cachelutsize = (!sccsc_cfg->sband_en && sccsc_cfg->lutsel && sccsc_cfg->lutsize) ? 33 : 17; 

        //Go put cache lut into memory if not in bypass
        sccsc_loadcachelut(sc_csc_reg, cachelutsize, cachelut); 

        //Clear cache  and counters
        sc_csc_reg->CACHE_INVLD = 1; //Clear cache tags
        sc_csc_reg->MCClr = 1;       //Clear cache miss counters
        sc_csc_reg->LCClr = 1;       //Clear cache latency counter
            
        //Enable counters
        reg_value = sc_csc_reg->CACHE_CNTRL;
        reg_value = SCCSC_CACHE_CNTRL_MCE_REPLACE_VAL(reg_value,1);
        reg_value = SCCSC_CACHE_CNTRL_LCE_REPLACE_VAL(reg_value,1);
        sc_csc_reg->CACHE_CNTRL = reg_value;
    }

    //Tell user whats happening
    DBG_PRINTF_NOTICE( "Test ScCSC " );
    if( sccsc_cfg->bypassall )
    {
        DBG_PRINTF_NOTICE("in Bypass mode\n\r");
    }
    else
    {
        if( !sccsc_cfg->sband_en && !sccsc_cfg->lutsel )
        {
            DBG_PRINTF_NOTICE("iLut ");
        }
        else if( !sccsc_cfg->sband_en && sccsc_cfg->lutsel )
        {
            if( sccsc_cfg->lutsize )
            {
                DBG_PRINTF_NOTICE("17 cubed cacheLut ");
            }
            else
            {
                DBG_PRINTF_NOTICE("33 cubed cacheLut ");
            }
        }
        else if(sccsc_cfg->sband_en )
        {
            DBG_PRINTF_NOTICE("with switched LUTs ");
        }

        DBG_PRINTF_NOTICE("(WME=%d) ", sccsc_cfg->wme);

        switch(sccsc_cfg->ialg) 
        {
            case 0 | 1: DBG_PRINTF_NOTICE("using Trilinear interpolation\n\r");  break;
            case 2:     DBG_PRINTF_NOTICE("using Radial interpolation\n\r");     break;
            case 3:     DBG_PRINTF_NOTICE("using Tetrahedral interpolation\n\r"); break;
            default:    DBG_PRINTF_NOTICE("using unknown interpolation\n\r");    break;
        }
    }
} //void ScCSC_setup( 

void sccsc_wr_ram(SCCSC_REGS_t *sc_csc_reg, uint32_t addr, uint32_t autoinc, uint32_t count, uint64_t* data)
{
    uint32_t cntrl = 0, i;
    uint32_t reg_value;

    // Write the requested data into the RAMs
    for(i=0;i<count;i++)
    {
        // Always do an initial write to addr
        if (i == 0)
        {
            if (autoinc == 1)
            {
                cntrl = 0x80000000 | addr;
            }
            else
            {
                cntrl = addr;
            }
            sc_csc_reg->RAR = cntrl;
        }

        reg_value = data[i];
        sc_csc_reg->RDW0 = reg_value;
        reg_value = (data[i] >> 32);
        sc_csc_reg->RDW1 = reg_value;

        // NOTE: Ignoring writing to RDW2 because don't have a array larger than 64-bits

        if (autoinc == 0)
        {
            sc_csc_reg->RAR = cntrl;
            addr = addr + 1;
            cntrl = addr;
        }
    } // for(i=0;i<count;i++){
} // void ScCSC_wr_RAM

void sccsc_loadilut(SCCSC_REGS_t *sc_csc_reg, uint32_t autoinc, uint64_t* lut)
{
    uint32_t i, ch0, ch1, ch2, ramaddr, cnt;
    uint32_t ramptr = 0;
    uint32_t cntrl_reg, saved_cpu_access;
    uint64_t val;

    // Break given LUT data into pieces specific for each RAM
    for(ch0=0; ch0 < 17; ch0++, ramptr+=2)
    {
        for(ch1=0; ch1 < 17; ch1++, ramptr++)
        {
            for(ch2=0; ch2 < 17; ch2++, ramptr++)
            {
                cnt = ramptr / 8;
                val = lut[(289*ch0)+(17*ch1)+ch2];
                sccsc_lut_ram[ramptr%8][cnt] = val;
            }
        }
    }

    // Bit 13 = 0 forces LUT RAM access, then bit 12:0 select which LUT RAM
    ramaddr = 0;  // Start at RAM 0

    // Safely setup the CPU Access for writing iLUT
    cntrl_reg = sc_csc_reg->CNTRL;
    saved_cpu_access = SCCSC_CNTRL_CPUACCESS_MASK_SHIFT(cntrl_reg);
    cntrl_reg = SCCSC_CNTRL_CPUACCESS_REPLACE_VAL(cntrl_reg,1);
    sc_csc_reg->CNTRL = cntrl_reg;

    // Write Data arrays into each RAM
    for(i=0; i<8; i++)
    {
        // Select RAM
        ramaddr = ramaddr & 0xFFFFE3FF;  // Clear bits 12:10
        ramaddr = ramaddr | (i << 10);   // Set bits 12:10 to loop value of i
        sccsc_wr_ram(sc_csc_reg, ramaddr, autoinc, 655, sccsc_lut_ram[i]);
    }

    // Revert CNTRL register to original settings
    cntrl_reg = sc_csc_reg->CNTRL;
    cntrl_reg = SCCSC_CNTRL_CPUACCESS_REPLACE_VAL(cntrl_reg,saved_cpu_access);
    sc_csc_reg->CNTRL = cntrl_reg;
} //void ScCSC_loadiLUT

/*-----------------
 * read ScCSC iLUT
 *-----------------
 */

void sccsc_rd_ram(SCCSC_REGS_t *sc_csc_reg, uint32_t addr, uint32_t count, uint64_t* data)
{
    uint32_t cntrl = 0, i;
    uint64_t v1,v2;
    uint32_t cntrl_reg, saved_cpu_access;

    // Safely setup the CPU Access for writing iLUT
    cntrl_reg = sc_csc_reg->CNTRL;
    saved_cpu_access = SCCSC_CNTRL_CPUACCESS_MASK_SHIFT(cntrl_reg);
    cntrl_reg = SCCSC_CNTRL_CPUACCESS_REPLACE_VAL(cntrl_reg,1);
    sc_csc_reg->CNTRL = cntrl_reg;

    // Read the requested data from the RAMs
    for(i=0;i<count;i++)
    {
        // Always do an initial write to addr
        if (i == 0)
        {
            cntrl = 0x10000 | addr;
            sc_csc_reg->RAR = cntrl;
        }

        v1 = sc_csc_reg->RDR1;
        v1 <<= 32;
        v2 = sc_csc_reg->RDR0;

        data[i] = v1 | v2;

        cntrl += 1;
        sc_csc_reg->RAR = cntrl;
    } // for(i=0;i<count;i++)

    // Revert CNTRL register to original settings
    cntrl_reg = sc_csc_reg->CNTRL;
    cntrl_reg = SCCSC_CNTRL_CPUACCESS_REPLACE_VAL(cntrl_reg,saved_cpu_access);
    sc_csc_reg->CNTRL = cntrl_reg;
} // void ScCSC_rd_RAM

// ============================================================
// loadcacheLut()
// -- Loads the given lutthat is arranged as either:
//          289*(ch0>>4)+17*(ch1>>4)+ch2>>4  (17^3)
//    or
//          1089*(ch0>>3)+33*(ch1>>3)+ch2>>3 (33^3)
//   into memory atthe address programed into BASE_ADDR of the register
//   interface.
//   
//    The cache LUT entry in memory is calculated as follows:
//          308*(ch0>>4)+18*(ch1>>4)+ch2>>4  (17^3)
//    or
//          1236*(ch0>>3)+34*(ch1>>3)+ch2>>3 (33^3)
//
//    Each entry in memory is stored according to the
//    number of output channels and width of each channel to maximize
//    bursting capabilities...
//
//i.e. 6 channels 10 bits wide format in memory
//-----------------------------
// Word  Byte   Pixels
// addr  addr
//-----------------------------
//  0     0     P0_ch0[9:2]            
//        1     P0_ch0[1:0],P0_ch1[9:4]
//        2     P0_ch1[3:0],P0_ch2[9:6]
//        3     P0_ch2[5:0],2'bx       
//  1     4     P0_ch3[9:2]            
//        5     P0_ch3[1:0],P0_ch4[9:4]
//        6     P0_ch4[3:0],P0_ch5[9:6]
//        7     P0_ch5[5:0],2'bx       
//  2     8     P1_ch0[9:2]            
//        9     P1_ch0[1:0],P1_ch1[9:4]
//       10     P1_ch1[3:0],P1_ch2[9:6]
//       11     P1_ch2[5:0],2'bx
//
//    This task puts the cache LUT array into system memory to allow 
//    proper vertice addressing by the cache dma's....
//=============================================================
void sccsc_loadcachelut(SCCSC_REGS_t *sc_csc_reg, uint32_t lutsize, uint64_t* lut)
{
    uint8_t b[8];    //8 bytes per entry
    uint8_t *base;   //lut ase address
    uint32_t i, w[2];
    uint32_t ch0, ch1, ch2, lut_index, mem_offset;
    uint64_t val;

    //Get the base addr
    base = (uint8_t *)sc_csc_reg->BASE_ADDR;

    //organize data and store in memory
    for( ch0=0; ch0<lutsize; ch0++ )
    {
        for( ch1=0; ch1<lutsize; ch1++ )
        {
            for( ch2=0; ch2<lutsize; ch2++ )
            {
                //Calculate index into given lut
                lut_index = (lutsize == 17) ? ((289*ch0)+(17*ch1)+ch2) :
                                          ((1089*ch0)+(33*ch1)+ch2);

                //Calculate memory offset (8 bytes allocated per entry
                mem_offset = (lutsize == 17) ? ((308*ch0)+(18*ch1)+ch2) * 8 :
                                           ((1124*ch0)+(34*ch1)+ch2) * 8;

                //Get value from lut to store
                val = lut[lut_index];

                //First break 60 bit value into words with 2 lsbs as don't cares
                w[0] = (uint32_t)(val>>28);
                w[0] &= 0xFFFFFFFCUL;
                w[1] = (uint32_t)(val<<2);

                //Then break the words into bytes
                b[3] = (uint8_t)(w[0] >> 24);
                b[2] = (uint8_t)(w[0] >> 16);
                b[1] = (uint8_t)(w[0] >> 8);
                b[0] = (uint8_t)w[0];
                b[7] = (uint8_t)(w[1] >> 24);
                b[6] = (uint8_t)(w[1] >> 16);
                b[5] = (uint8_t)(w[1] >> 8);
                b[4] = (uint8_t)w[1];

                //Store the bytes in expected order in memory
                for( i=0; i<8; i++ ) 
                {
                    *(base+mem_offset+i) = b[i];
                }
            } //for( ch2=0
        } //for( ch1=0
    } //for( ch0=0
} //void ScCSC_loadcacheLUT

// ============================================================
// generate img data() 
// ============================================================
void sccsc_gen_img( uint32_t *img,
		            uint32_t size )
{
    uint8_t  R, G, B;
    uint32_t i;

    R = G = B = 0;
    for( i=0; i<size; i++ )
    {
        *(img+i) = (uint32_t)R<<24 | (uint32_t)G<<16 |(uint32_t)B<<8;
        R++; G++; B++;
    }
} //void ScCSC_gen_img

// ============================================================
// generate lut data() 
// ============================================================
void sccsc_gen_lut( uint64_t *lut,
		            uint32_t cubesize)
{
    uint32_t R, G, B;
    
    //Generate both sets of LUT data that should yield R,!R,G,!G,B,!B 
    //(note this is hardcoded for 3 ch 8 bit data in and 6 channel 10 bit out...)
    for (R = 0; R < cubesize; R++) 
    {
        for (G = 0; G < cubesize; G++)
        {
            for (B = 0; B < cubesize; B++) 
            {
                *(lut+((cubesize*cubesize*R)+(cubesize*G)+B)) = 
                                          (uint64_t)(R*(256/(cubesize-1))) << 50 |
                                      (uint64_t)(256-(R*(256/(cubesize-1)))) << 40 |
                                      (uint64_t)(G*(256/(cubesize-1))) << 30 |
                                      (uint64_t)(256-(G*(256/(cubesize-1)))) << 20 |
                                      (uint64_t)(B*(256/(cubesize-1))) << 10 |
                                      (uint64_t)(256-(B*(256/(cubesize-1))));
            }
        }
    }
} //void ScCSC_gen_lut


// ============================================================
// load expected data in memory
// ============================================================
void sccsc_load_exp( SCCSC_REGS_t *sc_csc_reg,
                     uint32_t *img,
                     uint32_t pcnt,
                     uint32_t *addr )
{
    uint16_t i;
    uint16_t R0[2], G0[2], B0[2],R1[2], G1[2], B1[2];
    uint32_t reg_value;

    // Preload Expected Data into Memory, note each location is 32 bits 
    // and holds two consecutive channel values (one in the lower word, 
    // one in the upper)
    for( i=0; i<pcnt; i+=2 )
    {
        //Get the image rgb channels two at a time
        R0[0] = R0[1] = *(img+i)>>24 & 0xFF;
        G0[0] = G0[1] = *(img+i)>>16 & 0xFF;
        B0[0] = B0[1] = *(img+i)>>8  & 0xFF;
        R1[0] = R1[1] = *(img+i+1)>>24 & 0xFF;
        G1[0] = G1[1] = *(img+i+1)>>16 & 0xFF;
        B1[0] = B1[1] = *(img+i+1)>>8  & 0xFF;

        //White mapping (hardcoded for 3 ch 8 bit in and 6 ch 10 bit out)
        reg_value = sc_csc_reg->WT;
        if( ((R0[0]<<2)>SCCSC_WT_CH0_WT_MASK_SHIFT(reg_value)) && 
            ((G0[0]<<2)>SCCSC_WT_CH1_WT_MASK_SHIFT(reg_value)) &&
            ((B0[0]<<2)>SCCSC_WT_CH2_WT_MASK_SHIFT(reg_value)) )
        {
            reg_value = sc_csc_reg->WR_REG0;
            R0[0] = SCCSC_WR_REG0_CH0_WR_MASK_SHIFT(reg_value);
            G0[0] = SCCSC_WR_REG0_CH1_WR_MASK_SHIFT(reg_value);
            B0[0] = SCCSC_WR_REG0_CH2_WR_MASK_SHIFT(reg_value);
            reg_value = sc_csc_reg->WR_REG1;
            R0[1] = SCCSC_WR_REG1_CH3_WR_MASK_SHIFT(reg_value);
            G0[1] = SCCSC_WR_REG1_CH4_WR_MASK_SHIFT(reg_value);
            B0[1] = SCCSC_WR_REG1_CH5_WR_MASK_SHIFT(reg_value);
        }
        else
        { //If not white invert every other channel
            R0[1] = ((!R0[0])+1);
            G0[1] = ((!G0[0])+1);
            B0[1] = ((!B0[0])+1);
        }

        reg_value = sc_csc_reg->WT;
        if( ((R1[0]<<2)>SCCSC_WT_CH0_WT_MASK_SHIFT(reg_value)) && 
            ((G1[0]<<2)>SCCSC_WT_CH1_WT_MASK_SHIFT(reg_value)) &&
            ((B1[0]<<2)>SCCSC_WT_CH2_WT_MASK_SHIFT(reg_value)) )
        {
            reg_value = sc_csc_reg->WR_REG0;
            R1[0] = SCCSC_WR_REG0_CH0_WR_MASK_SHIFT(reg_value);
            G1[0] = SCCSC_WR_REG0_CH1_WR_MASK_SHIFT(reg_value);
            B1[0] = SCCSC_WR_REG0_CH2_WR_MASK_SHIFT(reg_value);
            reg_value = sc_csc_reg->WR_REG1;
            R1[1] = SCCSC_WR_REG1_CH3_WR_MASK_SHIFT(reg_value);
            G1[1] = SCCSC_WR_REG1_CH4_WR_MASK_SHIFT(reg_value);
            B1[1] = SCCSC_WR_REG1_CH5_WR_MASK_SHIFT(reg_value);
        }
        else
        { //If not white invert every other channel
            R1[1] = ((!R1[0])+1);
            G1[1] = ((!G1[0])+1);
            B1[1] = ((!B1[0])+1);
        }

        //Store expected data in order ODMA's expect it
        *(volatile uint32_t*)(*(addr+0)+(i*2)) = (uint32_t)R0[0]<<16 | (uint32_t)R1[0];
        *(volatile uint32_t*)(*(addr+1)+(i*2)) = (uint32_t)R0[1]<<16 | (uint32_t)R1[1];
        *(volatile uint32_t*)(*(addr+2)+(i*2)) = (uint32_t)G0[0]<<16 | (uint32_t)G1[0];
        *(volatile uint32_t*)(*(addr+3)+(i*2)) = (uint32_t)G0[1]<<16 | (uint32_t)G1[1];
        *(volatile uint32_t*)(*(addr+4)+(i*2)) = (uint32_t)B0[0]<<16 | (uint32_t)B1[0];
        *(volatile uint32_t*)(*(addr+5)+(i*2)) = (uint32_t)B0[1]<<16 | (uint32_t)B1[1];
    }
}
#endif
