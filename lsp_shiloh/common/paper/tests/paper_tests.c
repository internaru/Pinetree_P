
/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include "paper_size_api.h"
#include "logger.h"
#include "error_types.h"
#include "lassert.h"

#ifdef HAVE_UNIT_TEST
#include "unit_test_api.h"
/**
 * \brief Test the paper size api.
 * This test reads all the papersizes, prints out the sizes, pjl codes and names
 * and then tests for correct error behaviour.
 */
static int paper_tests(void)
{
    int i;
    mediasize_t media;
    uint32_t width, height;
    // Print out the papersize table.
    //
    DPRINTF(DBG_LOUD|DBG_OUTPUT,("Number of papers %d\n",paper_get_num_media_sizes()));
    DPRINTF(DBG_LOUD|DBG_OUTPUT,("MediaName           \tHeight\tWidth\tpjlCode\n"));
    // scan through all the papers printing out the size.
    for(i = 0; i < paper_get_num_media_sizes(); i++)
    {
        media = paper_get_entry(i);
        paper_get_media_size(media, &width, &height);
        DPRINTF(DBG_LOUD|DBG_OUTPUT,("%-20s\t%d\t%d \t%d\n",
                string_mgr_get_string(paper_get_id_string_token(media),string_mgr_loc_english),
                height, width, paper_get_pjl_code(media)));
        
    }
    // make sure we get an invalid entry when sending in a bad size.
    //
    media = paper_get_entry(paper_get_num_media_sizes()+1); // see if we get the correct value
    if(media != MEDIASIZE_BAD)
        return __LINE__;

    return 0;
}

#endif


void register_paper_unittest(void)
{

#ifdef HAVE_UNIT_TEST
    error_type_t e_res;
    e_res = unit_test_register_testcase("papertest", paper_tests);
    ASSERT( OK == e_res );
#endif
}
