/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief This file defines the ELF data structures needed by the boot code.
 * 
 * written 11-15-00 by Dirk Bodily
 *
 */

    
#ifndef _ELF_H
#define _ELF_H

#ifdef __cplusplus
extern "C" {
#endif


#define EI_NIDENT 16	// size of the ident section in ELF Header
typedef unsigned short Elf32_Half;
typedef unsigned long Elf32_Off;
typedef unsigned long Elf32_Addr;
typedef long Elf32_Sword;
typedef unsigned long Elf32_Word;

// elf ID magic numbers
#define ELFMAG0		0x7F
#define ELFMAG1		'E'
#define ELFMAG2		'L'
#define ELFMAG3		'F'

// CLASS values
#define ELFCLASSNONE	0
#define ELFCLASS32		1
#define ELFCLASS64		2

// DATA values
#define ELFDATANONE		0 //Invalid data encoding 
#define ELFDATA2LSB		1 //little endian
#define ELFDATA2MSB		2 //big endian 

// object file version values
#define EV_NONE		0 // invalid version
#define EV_CURRENT  1 // current version

// e_ident field offset defines
#define EI_MAG0		0
#define EI_MAG1		1
#define EI_MAG2		2
#define EI_MAG3		3
#define EI_CLASS	4	// 32 bit, 64 bit, etc
#define EI_DATA		5	// big endian/little endian
#define EI_VERSION	6	// object file version

// file type values
#define ET_REL		1 //Relocatable file 
#define ET_EXEC		2 //Executable file 
#define ET_DYN		3 //Shared object file 
#define ET_CORE		4 //Core file 

typedef struct
{
    unsigned char	e_ident[EI_NIDENT];
    Elf32_Half	e_type;
    Elf32_Half	e_machine;
    Elf32_Word	e_version;
    Elf32_Addr	e_entry;
    Elf32_Off	e_phoff;
    Elf32_Off	e_shoff;
    Elf32_Word	e_flags;
    Elf32_Half	e_ehsize;
    Elf32_Half	e_phentsize;
    Elf32_Half	e_phnum;
    Elf32_Half	e_shentize;
    Elf32_Half	e_shnum;
    Elf32_Half	e_shstrndx;
} Elf32_Ehdr;
//
// Format of a program header
//
typedef struct
{
	Elf32_Word	e_type;
    Elf32_Off	e_shoff;
    Elf32_Addr	e_virtAdd;
    Elf32_Addr  e_phyAdd;
    Elf32_Word  e_size;
    Elf32_Word  e_memsize;
    Elf32_Word  e_flags;
    Elf32_Word	e_align;
} Elf32_PrHdr;
	

// Defines for e_machine
#define EM_ARM 40

//
// Program headers
//
typedef struct
{
    Elf32_Word	p_type;
    Elf32_Off	p_offset;
    Elf32_Addr	p_vaddr;
    Elf32_Addr	p_paddr;
    Elf32_Word	p_filesz;
    Elf32_Word	p_memsz;
    Elf32_Word	p_flags;
    Elf32_Word	p_align;
} Elf32_Phdr;

//
// Section header
//
typedef struct
{
    Elf32_Word	sh_name;
    Elf32_Word	sh_type;
    Elf32_Word	sh_flags;
    Elf32_Addr	sh_addr;
    Elf32_Off	sh_offset;
    Elf32_Word	sh_size;
    Elf32_Word	sh_link;
    Elf32_Word	sh_info;
    Elf32_Word	sh_addralign;
    Elf32_Word	sh_entsize;
}Elf32_Shdr;

// Section types
#define SHT_PROGBITS 1
#define SHT_NOBITS 8
// Section attributes
#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_GHS_ABS 0x400


#ifdef __cplusplus
}
#endif

#endif //_ELF_H
