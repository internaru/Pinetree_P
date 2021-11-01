/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * \file unit_test.c
 *
 * \brief Implementation of simple unit test harness.
 * 
 */

#include <stdint.h>
#include <string.h>
#include "dprintf.h"
#include "memAPI.h"
#include "debug.h"
#include "error_types.h"
#include "lassert.h"


#ifdef HAVE_CMD
#include "cmd_proc_api.h"
static int unittest_callback( int argc, char *argv[] );
#endif


typedef struct unit_test_entry_s
{
    char *test_name;
    int (*test_entry)(void);
} unit_test_entry_t;

#define MAXIMUM_TEST_CASES_ALLOWED 64

unit_test_entry_t test_list[MAXIMUM_TEST_CASES_ALLOWED]=
{ 
    {0, 0}
};


void unit_test_init( void )
{
#ifdef HAVE_CMD
    int i_res;
    /* register the unit_test commands */
    i_res = cmd_register( "test",
                          NULL,
                          NULL,
                          NULL,
                          unittest_callback );
    ASSERT( CMD_OK == i_res );
#endif
}

/*
 *  \brief Use this to register a new test case
 *  
 *  \param name        The name of test case
 *  \param test_entry  The function pointer to the test case
 *  
 *  \return 0 Success
 *         Others failure
 * 
 */
error_type_t unit_test_register_testcase(char *name, int (*test_entry)(void))
{
    int32_t i;
    if(!name || !test_entry)
    {
        dbg_printf("Name or test_entry is empty!\n");
        return FAIL;
    }

    for( i = 0; i < MAXIMUM_TEST_CASES_ALLOWED; i++ )
    {
        if(test_list[i].test_name==0)
        {

            test_list[i].test_name = MEM_MALLOC(strlen(name)+1);
            if(!test_list[i].test_name)
            {
                dbg_printf("Fail to allocate memory! Abort!\n");
                return FAIL;
            }
            strcpy(test_list[i].test_name, name);
            test_list[i].test_entry = test_entry;

            if ( i < ( MAXIMUM_TEST_CASES_ALLOWED - 1 ) )
            {
                test_list[i+1].test_name = 0;
                test_list[i+1].test_entry = 0;
            }

            dbg_printf("Test case %s is added to %d\n", test_list[i].test_name, i);

            return OK;
        }
    }
    dbg_printf("Not enough slot!\n");
    return FAIL;
}


#ifdef HAVE_CMD
static void unittest_list_testcases()
{
    int32_t i;
    for( i = 0; i < MAXIMUM_TEST_CASES_ALLOWED && test_list[i].test_name; i++ )
    {
        dbg_printf("[%d] %s \n", i, test_list[i].test_name);
    }
    if( i < 1 )
    {
        dbg_printf("No test case available!\n");
    }
}

/**
 *
 * \brief Main unit test entry
 * 
 * \param test_to_do The name of test case to run, or NULL to run all test cases
 * 
 */
static void unittest_run_test(char *test_to_do)
{   
    int32_t i = 0;
    int32_t ret = 0;

    dbg_printf("TEST BEGIN...\n");

    for(i=0;i<MAXIMUM_TEST_CASES_ALLOWED && test_list[i].test_name; i++)
    {
        if( !test_to_do || !strcmp("all", test_to_do) || !strcmp(test_list[i].test_name, test_to_do) )
        {
            dbg_printf("Run test case %s...\n", test_list[i].test_name);

            if( (ret = test_list[i].test_entry()) > 0)
            {
                dbg_printf(">>>> Failed at test case %s. [%d]\n", test_list[i].test_name, ret);
            }
            else
            {
                dbg_printf("Passed.\n");
            }
        }
    }

    if(ret)
    {
        dbg_printf("TEST FAILED.\n");
    }
    else
    {
        dbg_printf("TEST PASSED.\n");
    }

}


/**
 * \brief Dump test debug call back usage
 */
static void dump_usage()
{

    dbg_printf("To run all test cases: test run\n");
    dbg_printf("To run some test cases: test run <case1> <case2> ...\n");
    dbg_printf("To display available test cases: test list\n");
}

/*
 * \brief Debug call back for unit test.
 * 
 *  unittest run: Start run the test on all test cases
 *  unittest run: <case 1> <case 2> ...: Run the test cases required
 *  unittest list: List all the test case available
 * 
 */
static int unittest_callback( int argc, char *argv[] )
{
    int32_t pos=1;

    if( argc < 2 )
    {
        dump_usage();
        return 1;
    }

    if( strncmp( argv[pos], "run", 3 ) == 0 )
    {
        if(argc==2)
        {
            unittest_run_test(0);
        }
        else
        {
            for(pos=pos+1;pos<argc;pos++)
            {
                unittest_run_test(argv[pos]);
            }
        }

    }
    else if(strncmp(argv[pos], "list", 4)==0)
    {
        unittest_list_testcases();
    }
    else
    {
        dump_usage();
    }
    return 0;
}
#endif

