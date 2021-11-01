
/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "ioutils.h"
#include "code_table.h"
#include "crc32.h"
#include "lassert.h"


uint32_t ct_get_total_length(const code_table_header_t *table_header)
{
    if(be32_to_cpu(table_header->signature) == CODE_TABLE_SIGNATURE)
    {
        //
        // We have a code table, verify the table
        // 
        return be32_to_cpu(table_header->length);
    }
    return 0;

}
uint32_t ct_get_num_entries(const code_table_header_t *table_header)
{
    if(be32_to_cpu(table_header->signature) == CODE_TABLE_SIGNATURE)
    {
        return (be32_to_cpu(table_header->length) - sizeof(code_table_header_t))/ sizeof(code_table_entry_t);
    }
    return 0;
}

bool ct_verify_table(const code_table_header_t *table_header)
{
    uint32_t length, TableLen;
    uint32_t Offset;

    length = sizeof(code_table_header_t);
    if(be32_to_cpu(table_header->signature) == CODE_TABLE_SIGNATURE)
    {
        //
        // We have a code table, verify the table
        // 
        TableLen = be32_to_cpu(table_header->length);
        // Now generate the table.
        Offset = short_crc32((char *)&table_header->length,
                              TableLen - offsetof(code_table_header_t, length));
        // check to see if the crc is valid.
        if(be32_to_cpu(table_header->CRC) == Offset)
        {
            return true;
        }
    }
    return false;
}

uint32_t ct_get_entry_id(uint32_t *FlashAdd, uint32_t Entry)
{
    code_table_entry_t  *MyEntry;

    ASSERT(Entry);  // must be > 0

    MyEntry = (code_table_entry_t *)(((uintptr_t)FlashAdd) + sizeof(code_table_header_t) + 
                                     (Entry -1) * sizeof(code_table_entry_t));
    return be32_to_cpu(MyEntry->entry_id);

}
uint16_t ct_get_entry_type(uint32_t *FlashAdd, uint32_t Entry)
{

    code_table_entry_t  *MyEntry;

    ASSERT(Entry);  // must be > 0

    MyEntry = (code_table_entry_t *)(((uintptr_t)FlashAdd) + sizeof(code_table_header_t) + 
                                     (Entry -1) * sizeof(code_table_entry_t));
    return be16_to_cpu(MyEntry->type);
}
/* FUNCTION NAME:  */
 
/** 
 * \brief See if there is a code table, if so scan the table and
 * return the Number occurance of the sector type of sectortype.
 * 
 * \param FlashAdd The address to check for a code table
 * 
 * \param SectorType The type of sector to search for
 * 
 * \param Number The number of occurances to find.
 * 
 * \retval uint32_t 0 nothing found or no code table
 *              # the offset to the code table from FlashAdd.
 * 
 * \date 8/28/2007
 * 
 **/
uint32_t ct_find_entry(uint32_t *FlashAdd, uint32_t SectorType, uint32_t Number)
{
    uint32_t Offset;
    code_table_header_t *MyHeader;
    code_table_entry_t  *MyEntry;
    uint32_t length, TableLen;

    MyHeader = (code_table_header_t *) FlashAdd;
    length = sizeof(code_table_header_t);
    if(be32_to_cpu(MyHeader->signature) == CODE_TABLE_SIGNATURE)
    {
        //
        // We have a code table, verify the table
        // 
        TableLen = be32_to_cpu(MyHeader->length);
        // Now generate the table.
        Offset = short_crc32((char *)&MyHeader->length,
                              TableLen - offsetof(code_table_header_t, length));
        // check to see if the crc is valid.
        if(be32_to_cpu(MyHeader->CRC) == Offset)
        {
            // now scan the table looking for my entry.
            Offset = 1;
            MyEntry = (code_table_entry_t *)((uintptr_t)FlashAdd + sizeof(code_table_header_t));
            while(length < TableLen)  // have we done it all?
            {
                // nope look at the next one.
                if((((be16_to_cpu(MyEntry->type) == SectorType)) &&
                    --Number == 0))
                {
                    // we found a config block.  Check it out.
                    //
                    return Offset;
                }
                Offset++;    // go to the next table entry
                MyEntry = (code_table_entry_t *)((uintptr_t)MyEntry + sizeof(code_table_entry_t));
                length += sizeof(code_table_entry_t);
            }
        }
    }
    return 0;
}
/**
 * \brief Get the offset out of the code table.
 * \param[in] table address of the code table
 * \param[in] offset Which entry to find.
 * \returns uint64_t
 * \retval The offset in the table to get
 */
uint64_t ct_get_entry_offset(uint32_t *table, uint32_t offset)
{
    uint64_t RetVal;

    code_table_entry_t  *MyEntry;

    RetVal = 0;

    MyEntry = (code_table_entry_t *)((uintptr_t) table + sizeof(code_table_header_t) + 
                (offset -1) * sizeof(code_table_entry_t));
    RetVal = ((uint64_t)be32_to_cpu(MyEntry->high_offset))<<32;
    RetVal = RetVal + be32_to_cpu(MyEntry->low_offset);
    return RetVal;

}

/**
 * \brief Get the length entry from the code table entry.
 * \param[in] table The address of the code table
 * \param[in] offset The index into the table.
 * \returns uint64_t
 * \retval The length of the entry.
 */
uint64_t ct_get_entry_length(uint32_t *table, uint32_t offset)
{

    uint64_t RetVal;

    code_table_entry_t  *MyEntry;

    RetVal = 0;
    // 
    // Index to the correct entry
    MyEntry = (code_table_entry_t *) ((uintptr_t) table + sizeof(code_table_header_t) + 
                (offset -1) * sizeof(code_table_entry_t));
    //
    // Get the high word and shift it
    RetVal = ((uint64_t)be32_to_cpu(MyEntry->high_length))<<32;
    // add in the low 32 bit value
    RetVal = RetVal + be32_to_cpu(MyEntry->low_length);
    return RetVal;
}

uint32_t ct_build_header(void *start_addr, uint32_t num_entries)
{
    code_table_header_t *CTHeader = start_addr;


    memset(CTHeader, 0, sizeof(code_table_header_t));
    CTHeader->signature = cpu_to_be32(CODE_TABLE_SIGNATURE);

    CTHeader->length = cpu_to_be32(sizeof(code_table_header_t));
    return sizeof(code_table_header_t) + num_entries * sizeof(code_table_entry_t);
}

void ct_add_entry(void *start_addr, uint16_t type, uint32_t entity_id, uint64_t offset, uint64_t length)
{
    code_table_header_t *CTHeader = start_addr;
    code_table_entry_t  *CTEntry;

    CTEntry = (code_table_entry_t *)((uintptr_t)start_addr + be32_to_cpu(CTHeader->length));

    memset(CTEntry, 0, sizeof(code_table_entry_t));

    CTEntry->type = cpu_to_be16(type);
    CTEntry->entry_id = cpu_to_be32(entity_id);
    CTEntry->high_length = cpu_to_be32((uint32_t)(length >> 32));
    CTEntry->low_length = cpu_to_be32((uint32_t)length);
    CTEntry->high_offset = cpu_to_be32((uint32_t)(offset>>32));
    CTEntry->low_offset = cpu_to_be32((uint32_t) offset);
    CTHeader->length = cpu_to_be32(be32_to_cpu(CTHeader->length) + sizeof(code_table_entry_t));

    
}
uint32_t ct_complete_table(void *start_addr)
{

    code_table_header_t *CTHeader = start_addr;
    // The crc does not cover the first 8 bytes of the header.
    CTHeader->CRC = cpu_to_be32(short_crc32((char *)&CTHeader->length, be32_to_cpu(CTHeader->length)
                               - offsetof(code_table_header_t, length)));
    return be32_to_cpu(CTHeader->length);
}

error_type_t ct_modify_entry(uint32_t *table,uint32_t offset,uint32_t type, uint32_t entry_id,
                                    uint64_t address,uint64_t length)
{

    code_table_entry_t  *MyEntry;


    MyEntry = (code_table_entry_t *) ((uintptr_t) table + sizeof(code_table_header_t) + 
                                      (offset -1) * sizeof(code_table_entry_t));

    MyEntry->type = cpu_to_be16(type);
    MyEntry->entry_id = cpu_to_be32(entry_id);
    MyEntry->high_length = cpu_to_be32((uint32_t)(length >> 32));
    MyEntry->low_length = cpu_to_be32((uint32_t)length);
    MyEntry->high_offset = cpu_to_be32((uint32_t)(address>>32));
    MyEntry->low_offset = cpu_to_be32((uint32_t) address);

    return OK;
}
