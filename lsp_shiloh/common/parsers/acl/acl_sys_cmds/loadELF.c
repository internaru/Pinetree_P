/***********************************************************
* (c) Copyright 2007-2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

#include "ATypes.h"
#include "logger.h"
#include "elf.h"

typedef int (* vfuncptrv)(void);

/* FUNCTION NAME: LoadELF */
 
/** 
 * \brief Given an elf file unpack it
 * 
 * \param imageStartAddr Where the elf image is located
 * 
 * \retval int -1 error, else image start address
 * 
 * \author Dirk Bodily
 * 
 * \date 9/20/2007
 * 
 **/
int LoadELF(uint8_t* imageStartAddr)
{    
    vfuncptrv entryPoint;
    unsigned int sectionNum;
    Elf32_Ehdr elfHeader;
    unsigned int i;
    unsigned char *pSecHeader;
    Elf32_Shdr secHeader = {0};
    uint8_t* codeSrcAddr;
    uint8_t* codeDstAddr;

    // copy elf header to local buffer to guarantee alignment
    for(i = 0; i < sizeof(Elf32_Ehdr); i++)
    {
        *((char *)&elfHeader + i) = *(imageStartAddr + i);
    } 

    // verify that this is an elf image in the format that we expect
    if((elfHeader.e_ident[EI_MAG0] != ELFMAG0) ||     // verify magic number
       (elfHeader.e_ident[EI_MAG1] != ELFMAG1) ||
       (elfHeader.e_ident[EI_MAG2] != ELFMAG2) ||
       (elfHeader.e_ident[EI_MAG3] != ELFMAG3) ||
       (elfHeader.e_ident[EI_CLASS] != ELFCLASS32) || // verify that it's a 32bit image

#ifdef CPU_LITTLE_ENDIAN
       (elfHeader.e_ident[EI_DATA] != ELFDATA2LSB) || // little endian test
#else
       (elfHeader.e_ident[EI_DATA] != ELFDATA2MSB) || // verify that the image is in bit endian format
#endif
       (elfHeader.e_type != ET_EXEC) ||               // verify that the image is an executable
       (elfHeader.e_shoff == 0))                      // and section header offset are non NULL
    {
        //     DBG_PRINTF(DBG_LOUD, ("ElfImage Bad, Go on\n\r"));
        // 
        return NULL;  // bad, don't load it, just return.
    }
    else
    {
        //  DBG_PRINTF(DBG_LOUD, ("ElfImage OK.  Loading...\r\n"));
        // parse the elf file and copy the correct sections to memory
        for(sectionNum = 0; sectionNum < elfHeader.e_shnum; sectionNum++)
        {
            pSecHeader = (unsigned char *) ((uintptr_t) imageStartAddr + 
                                            elfHeader.e_shoff + 
                                            (sectionNum * elfHeader.e_shentize));

            //copy section header to local buffer to guarantee 32bit alignment
            for(i = 0; i < elfHeader.e_shentize; i++)
            {
                *((char *)&secHeader + i) = *(pSecHeader + i);
            } 
            //   DBG_PRINTF(DBG_VERY_LOUD, ("Parsing section header %d, sh_type = %d, sh_addr = %#x\r\n", sectionNum, secHeader.sh_type, secHeader.sh_addr));
            switch(secHeader.sh_type)   // decide what to do
            {
                case SHT_PROGBITS:
                    // make sure this is really part of the memory image
                    if(secHeader.sh_flags & (SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR))
                    {
                        //
                        // This is program code, move the section to the correct location.
                        //
                        codeSrcAddr = (uint8_t*) (imageStartAddr + secHeader.sh_offset);
                        codeDstAddr = (uint8_t*) (secHeader.sh_addr);

                        //
                        // Move the block of code into memory.
                        //
                        //  do it in word sizes, much faster
                        //
                        for(i = 0; i < secHeader.sh_size; i+=4)
                        {
                            *((uint32_t *)(codeDstAddr + i)) = *((uint32_t *)(codeSrcAddr + i));
                        }  
                    }
                    break;
                default:
                    break;
            }
        }
        /*
         * Get the address of elf image
         */
        entryPoint = (vfuncptrv) elfHeader.e_entry;
        entryPoint();       // start the code.
    } 

    return(intptr_t) entryPoint;
}
