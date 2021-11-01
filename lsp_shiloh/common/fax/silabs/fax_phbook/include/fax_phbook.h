/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_FAX_PHBOOK_H__
#define __INC_FAX_PHBOOK_H__

/* This module stores and manages pairs of telephone numbers and names which
   are called phonebook entries. Each entry has a name with a maximum number
   of characters, (FAX_PHBOOK_NAME_MAX_LEN) and a number with maximum number
   of characters (FAX_PHBOOK_NUMBER_MAX_LEN). At most, FAX_PHBOOK_NUM_ENTRIES
   may be stored by the system.

   Each phone book may also be assigned to zero, one or more speed dial
   groups. There are FAX_PHBOOK_NUM_GROUPS defined by the system. A product
   UI can use this definition to implement single entry speed dials or group
   dials or a mixture of both. Each speed dial group can be given a name with
   the same name length limit as a phone book entry (FAX_PHBOOK_NAME_MAX_LEN).

   Phone book entries may also be blacklisted. Some product definitions call
   for separate phone book and black list tables. In these products the user
   interface may decide to use a portion of the available phone book as 
   speed dials and a portion for blacklist entries. This implementation
   detail is left as an external decision by this library.

   What follows are some typical use cases and policy decisions that may be
   made by clients of this library.

   1. A product that defines 10 speed dials (0-9) could use groups 0-9
      and a single number to each group. The group name may or may not be
      used in this case.

   2. A product defines 12 speed dial entries and 12 group dial entries.
      The group dials are accessed by name and the speed dials by number.
      FAX_PHBOOK_NUM_GRPUPS should be set to 24. Speed dials are stored
      in groups 0-11. The name of these groups is not used by the user
      interface. Each of these groups contains a single phone book entry.
      Groups 12 through 23 are the group dials. Each has name used by the
      user interface to identify the group. Each group can contain 0 or more
      phone book entries. When a phone book entry is modified the user
      interface should manage any speed dial or group to which that entry was
      assigned. For example, if a phone book entry is cleared (has its number
      set to "") or otherwise modified (number or name changed) then it
      probably makes sense to remove that entry from any group (speed dial or
      group dial) to which it was assigned. Of course, depending upon the
      device use model(s) some other behavior may be more appropriate.

   3. Products that make use of blacklisting may want to reserve a portion
      of the phonebook space for these blacklisted numbers. Other device models
      allow any number in the phone book to be either a blacklisted number,
      a speed dial number or both. This API allows phonebook entries to be
      tagged as a blacklisted entry. */

#define FAX_PHBOOK_NAME_MAX_LEN   (32)
#define FAX_PHBOOK_NUMBER_MAX_LEN (50)
#define FAX_PHBOOK_NUM_ENTRIES   (1)//(100)
#define FAX_PHBOOK_NUM_GROUPS    (0)//(50)

/* This should be called before anything else */
void fax_phbook_init(void);

/* This commits all phone book changes to non-volatile memory */
void fax_phbook_commit(void);

/* These functions can be used to set or get the name, number, or blacklist
   status of a single phone book entry. In all cases a return code of 0
   indicates success. */
int fax_phbook_set_name(unsigned int phbook_id, const char* name);
int fax_phbook_get_name(unsigned int phbook_id,       char* name);
int fax_phbook_set_number(unsigned int phbook_id, const char* number);
int fax_phbook_get_number(unsigned int phbook_id,       char* number);
int fax_phbook_set_blacklist(unsigned int phbook_id, int  blacklist);
int fax_phbook_get_blacklist(unsigned int phbook_id, int* blacklist);

/* These functions can be used to set or get the name, number, and blacklist
   status of a single phone book entry. In all cases a return code of 0
   indicates success. */
int fax_phbook_set(unsigned int phbook_id,
                   const char* name, const char* number, int  blacklist);
int fax_phbook_get(unsigned int phbook_id,
                         char* name,       char* number, int* blacklist);

/* This function wipes out the name, number, and blacklist setting of a single
   entry in the phone book. In addition it searches for this entry and removes
   it from any group list. */
int fax_phbook_erase(unsigned int phbook_id);

/* These functions can be used to set or get the name or blacklist status of
   a single group list. In all cases a return code of 0 indicates success */
int fax_phbook_group_set_name(unsigned int group_id, const char* name);
int fax_phbook_group_get_name(unsigned int group_id,       char* name);

/* These two functions are used for adding or removing phone book indeces to
   group lists. A return code of 0 indicates success. A return code of
   FAX_PHBOOK_RESULT_ALREADY is returned when a a request would result in no
   change to the group list (e.g. add an entry that is already part of a group
   list or remove an entry that is not currently part of a group list). The
   return code of -1 is used to indicate any other failure */
#define FAX_PHBOOK_RESULT_ALREADY (1)
int fax_phbook_group_add_entry(unsigned int group_id,
                               unsigned int phbook_id);
int fax_phbook_group_remove_entry(unsigned int group_id,
                                  unsigned int phbook_id);

/* This function essentially resets a group by removing all entries and
   clearing any name that may have been set */
int fax_phbook_group_erase(unsigned int group_id);

/* These are used to query the contents of group lists. The first is used to
   determine how many entries a part of a specified group. The second can be
   iteratively used to read the phone book id's of each member of the group
   list. In all cases a return code of 0 indicates success. */
int fax_phbook_group_get_entry_count(unsigned int  group_id,
                                     unsigned int *count);
int fax_phbook_group_get_phbook_idx(unsigned int  group_id,
                                    unsigned int  which,
                                    unsigned int *phbook_id);

#endif /* __INC_FAX_PHBOOK_H__ */
