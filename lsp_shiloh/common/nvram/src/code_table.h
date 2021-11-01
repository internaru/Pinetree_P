
/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef CODE_TABLE_H
#define CODE_TABLE_H
/**
 * \file
 * Define the structure of the code table.  T
 */

/**
 * Structure of the code table header.
 */
typedef struct
{
    uint32_t    signature;      ///< Typically 0xc1deed1c
    uint32_t    CRC;            ///< CRC32 of everything after this till end of table
    uint32_t    length;         ///< Total length in bytes of the image map including header
    uint16_t    major_version;  ///< This changes if the format of the image map changes
    uint16_t    minor_version;  ///< Change if change such as reserved field being used happens
} code_table_header_t;
/**
 * Define the structure for a code table entry
 */
typedef struct
{
    uint32_t    entry_id; ///< Entry identifier.  May be numeric or ascii 4 char sequence.  
                        ///< Duplicates allowed as long as the system has no reason to search
                        ///< for the id for that object.
    uint16_t    type;   ///< Type of object described by this entry.
    uint16_t    flags;  ///< Bit 0 = 1 writable flag.
    uint32_t    high_offset; ///< High 32 bits of the object offset.  relative to start of header
    uint32_t    low_offset; ///< Low 32 bits of the object offset
    uint32_t    high_length; ///< The high 32 bits of the object length
    uint32_t    low_length;  ///< The low 32 bits of the object length
    uint32_t    version;    ///< Optional version of the object in a product specific format
    uint32_t    reserved;   ///< reserved
} code_table_entry_t;

/**
 * Signature value
 */
#define CODE_TABLE_SIGNATURE 0xc1deed1c
/**
 * defines for the type field
 */
#define CT_CONFIG_DATA     1   ///< System configuration data, the config table
#define CT_CODE            2   ///< Executable code -- This object will start with a firmware image header
#define CT_DATA            3    ///< Binary data 
#define CT_SPARES          4    ///< Spare data for the device that contains this table
#define CT_FILE_SYS        5    ///< File system area
#define CT_EASY_INST       6    ///< start of easy install

/**
 * \brief  Scan a code table looking for an entry.  Return non-zero if there.
 * \param[in] address Address of the code table to scan
 * \param[in] type The type to find, see defines above for types.
 * \param[in] number The number of table to find.  There can be duplicate entries, this finds them
 * \returns uint32_t
 * \retval 0 Nothing was found in the code table
 * \retval !0 The Offset into the table to for the requested entry.  Used in later functions.
 */
uint32_t ct_find_entry(uint32_t *address, uint32_t type, uint32_t number);
/**
 * \brief Given an offset into a code table, return the 64 bit offset from that entry.
 * No error checking is performed, this just index's and gets the value.  
 * \param[in] table Address of the code table.
 * \param[in] offset The value returned from ct_find_entry.
 * \returns uint64_t
 * \retval The offset entry from the requested code table entry.
 */
uint64_t ct_get_entry_offset(uint32_t *table, uint32_t offset);
/**
 * \brief Given an offset into a code table, return the 64 bit length from that entry.
 * No error checking is performed, this just index's and gets the value
 * \param[in] table Address of the code table
 * \param[in] offset The value returned from ct_find_entry
 * \returns uint64_t
 * \retval The length entry from the requested code table entry.
 */
uint64_t ct_get_entry_length(uint32_t *table, uint32_t offset);
/**
 * \brief Create a code table header.
 * This creates a code table header at the address requested.  Adding num_entries
 * causes this to return the length of the entire code table with that number of entries
 * \param[in] start_addr The address where the header is to be created.
 * \param[in] num_entries The number of entries that will exist in this table.
 * \returns uint32_t
 * \retval # The length in bytes of the complete code data of num_entries.
 */

uint32_t ct_build_header(void *start_addr, uint32_t num_entries);
/**
 * \brief Add an entry to a code table.
 * This will take a code table, find the end of the table, and add the entry requested.
 * The length in the header is then updated to match the new entry.
 * \param[in] start_addr The start address of the code table.
 * \param[in] type The type of entry.
 * \param[in] entity_id The id that is assigned to this entry.
 * \param[in] offset The offset entry for this entity.
 * \param[in] length The length entry for this entity.
 * \returns void
 */
void ct_add_entry(void *start_addr, uint16_t type, uint32_t entity_id, 
                  uint64_t offset, uint64_t length);
/**
 * \brief Called after all entries are made to a code table to calcualte and store the crc
 * \param[in] start_addr The address of the code table
 */
uint32_t ct_complete_table(void *start_addr);


uint32_t ct_get_total_length(const code_table_header_t *table_header);

bool ct_verify_table(const code_table_header_t *table_header);

uint32_t ct_get_num_entries(const code_table_header_t *table_header);
uint16_t ct_get_entry_type(uint32_t *FlashAdd, uint32_t Entry);

uint32_t ct_get_entry_id(uint32_t *FlashAdd, uint32_t Entry);

error_type_t ct_modify_entry(uint32_t *table,uint32_t offset,uint32_t type, uint32_t entry_id,
                                    uint64_t address,uint64_t length);


#endif
