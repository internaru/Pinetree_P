/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                      Marvell Confidential
 * ============================================================================
 */

/**
 * \file printVars.c
 *
 * \brief Access functions for the print component specific variables.
 *
 * This file contains the data structures and functions needed to support the public print
 * component specific variables.
 *
 **/

// LSP do not merge this file over from UNITY.

//--------------------------------------
// Included Files
//--------------------------------------

#include <string.h>
#include <stdbool.h>
#include "ATypes.h"
#include "error_types.h"
#include "lassert.h"
#include "memAPI.h"
#include "nvram_api.h"
#include "printvars_api.h"
#include "agprint.h"
#include "agMessage.h"
#include "agRouter.h"
#ifdef HAVE_STATISTICS
#include "statistics_api.h"
#include "statistics_vars.h"
#endif

#ifdef HAVE_RTC
#include "rtc_api.h"
#endif

#include "db_api.h"
#include "nvram_db_api.h"

//--------------------------------------
// Type Definitions
//--------------------------------------

// list of all variables supported by this component

typedef enum
{
    // perm vars
    e_SizeTypeTray,
    e_JamRecovery,
    e_TrayConfig,
    e_PaperOutAction,
    e_DefaultPaperSize,
    e_DefaultPaperType,
    e_LogEntry,
    // temp vars
    e_DOCType,
    e_JOBName,
    e_CurrentJobPgCnt,
    e_PagesPerJob,
    e_ManualDuplex,
    e_PrinterSetup,
    e_CurrentJobID,
    e_JobInfo,
    e_JobInfoCurSlot,
    e_PrintSysStatus,
    e_PrintOnline,
    e_CurrentJobByteCnt,
    // total
    NUM_PRINT_VARS
} printVarId_t;



#define ALL_PRINT_VARS 0xff

// values which reside in dynamic permanent storage
// IMPORTANT:  increment PRINT_VARS_VERSION whenever this structure changes

#define PRINT_VARS_VERSION 5

typedef struct
{
    size_type_tray_t SizeTypeTray;      ///< get/set by printmgr 
    uint8_t JamRecovery;                ///< default ON to recover from jams.
    mediacnfg_t TrayConfig;             ///< user configured what is in each input tray.
    paperoutaction_t PaperOutAction;
//	uint32_t DefaultPaperSize;          ///< duplicate of what is in trayconfig
//	uint32_t DefaultPaperType;          ///< duplicate of what is in trayconfig

#define PRINT_LOG_SIZE 10
    print_log_entry_t log_entry[PRINT_LOG_SIZE];
    uint32_t print_log_cur_index; /*!< log_entry index, not accessible to outside world */
} printPermVars_t;


// values which reside in RAM 

typedef struct
{
    printer_setup_t pSetup;
} printTempVars_t;

#define WEAK  __attribute__((weak))

/// engine writes everyone else is read only. 
///
//printer_setup_t WEAK default_printer_setup = 
//{
    //.options = 0x0000000b,
    //.inputTrays = 
    //{
	//{
	    //.inputType = INPUT_MANUAL,
	    //.properties = INPUTPROPERTY_ALLPURPOSE,
	    //.paperPresent = true,   // dynamic
	    //.inputPresent = true,   // dynamic
	    //.numSizes = 2,
	    //.numTypes = 2,
	    //.supportedSizes = { MEDIASIZE_LETTER, MEDIASIZE_A4, 0, },
	    //.supportedTypes = { MEDIATYPE_PLAIN_PAPER, MEDIATYPE_PLAIN_PAPER, 0, }, 
	//},
	//{
	    //.inputType = INPUT_MIDDLE,
	    //.properties = INPUTPROPERTY_ALLPURPOSE,
	    //.paperPresent = true,
	    //.inputPresent = true,
	    //.numSizes = 2,
	    //.numTypes = 2,
	    //.supportedSizes = { MEDIASIZE_LETTER, MEDIASIZE_A4, 0, },
	    //.supportedTypes = { MEDIATYPE_PLAIN_PAPER, MEDIATYPE_PLAIN_PAPER, 0, },
	//},
	//{0},
	//{0},
	//{0},
    //},
    //.outputTrays = 
    //{
	//{ 
	    //.outputType = OUTPUT_AUTO,
	    //.properties = 0,
	    //.maxSheets = 100,
	    //.isFull = false,      // dynamic 
	    //.outputPresent = true,
	//},
	//{0},
	//{0},
	//{0},
	//{0},
    //},
//};

#define MANUAL_ONLY_SIZES MEDIASIZE_ENV_9, MEDIASIZE_ENV_10,\
	MEDIASIZE_ENV_C5, MEDIASIZE_ENV_C6, MEDIASIZE_ENV_DL,\
	MEDIASIZE_ENV_B5, MEDIASIZE_ENV_MONARCH, \
	MEDIASIZE_JAPANESE_POSTCARD, MEDIASIZE_DBL_JAPANESE_POSTCARD,\
	MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED, MEDIASIZE_A5_ROTATED           
#define NUM_MANUAL_ONLY_SIZES 11

#define MANUAL_ONLY_TYPES MEDIATYPE_ENVELOPE, MEDIATYPE_CARDSTOCK, MEDIATYPE_CARD_GLOSSY  
#define NUM_MANUAL_ONLY_TYPES 3

#define BIG_TRAY_SIZES MEDIASIZE_LETTER, MEDIASIZE_A4, MEDIASIZE_EXECUTIVE, \
	MEDIASIZE_LETTER, MEDIASIZE_LEGAL,\
	MEDIASIZE_STATEMENT, MEDIASIZE_EXECUTIVE, MEDIASIZE_CUSTOM_85x13,\
	MEDIASIZE_A4, MEDIASIZE_A5, MEDIASIZE_A6,\
	MEDIASIZE_B5, MEDIASIZE_B6_JIS, MEDIASIZE_CUSTOM_8x10,\
	MEDIASIZE_CUSTOM_8x105, MEDIASIZE_CUSTOM_825x14, MEDIASIZE_UNKNOWN 
#define NUM_BIG_TRAY_SIZES 17

#define BIG_TRAY_TYPES MEDIATYPE_ANY, MEDIATYPE_PLAIN_PAPER, MEDIATYPE_HEAVY1, MEDIATYPE_EXTRA_HEAVY, \
	MEDIATYPE_LIGHTPAPER, MEDIATYPE_TRANSPARENCY, MEDIATYPE_LABEL, MEDIATYPE_HEAVY_GLOSSY  
#define NUM_BIG_TRAY_TYPES 8

#define ALL_SIZES MANUAL_ONLY_SIZES , BIG_TRAY_SIZES
#define NUM_ALL_SIZES NUM_BIG_TRAY_SIZES + NUM_MANUAL_ONLY_SIZES

#define ALL_TYPES BIG_TRAY_TYPES , MANUAL_ONLY_TYPES 
#define NUM_ALL_TYPES NUM_BIG_TRAY_TYPES + NUM_MANUAL_ONLY_TYPES

printer_setup_t WEAK default_printer_setup = 
{
    .options = 0x0000000b,
    .inputTrays = 
    {
	{
	    .inputType = INPUT_MANUAL,
	    .properties = INPUTPROPERTY_PRIORITY,
	    .paperPresent = true,   // dynamic
	    .inputPresent = true,   // dynamic
	    .numSizes = NUM_ALL_SIZES,
	    .numTypes = NUM_ALL_TYPES,
	    .supportedSizes = { ALL_SIZES, 0 },
	    .supportedTypes = { ALL_TYPES, 0 }, 
	},
	{
	    .inputType = INPUT_MIDDLE,
	    .properties = INPUTPROPERTY_ALLPURPOSE,
	    .paperPresent = true,
	    .inputPresent = true,
	    .numSizes = NUM_BIG_TRAY_SIZES,
	    .numTypes = NUM_BIG_TRAY_TYPES,
	    .supportedSizes = { BIG_TRAY_SIZES, 0, },
	    .supportedTypes = { BIG_TRAY_TYPES, 0, },
	},
	{
	    .inputType = INPUT_LOWER,
	    .properties = INPUTPROPERTY_ALLPURPOSE,
	    .paperPresent = true,
	    .inputPresent = true,
	    .numSizes = NUM_BIG_TRAY_SIZES,
	    .numTypes = NUM_BIG_TRAY_TYPES,
	    .supportedSizes = { BIG_TRAY_SIZES, 0, },
	    .supportedTypes = { BIG_TRAY_TYPES, 0, },
	},
	{
		.inputType = INPUT_LOWER,
	    .properties = INPUTPROPERTY_ALLPURPOSE,
	    .paperPresent = true,
	    .inputPresent = true,
	    .numSizes = NUM_BIG_TRAY_SIZES,
	    .numTypes = NUM_BIG_TRAY_TYPES,
	    .supportedSizes = { BIG_TRAY_SIZES, 0, },
	    .supportedTypes = { BIG_TRAY_TYPES, 0, },
	},
	{0},
    },
    .outputTrays = 
    {
	{ 
	    .outputType = OUTPUT_AUTO,
	    .properties = 0,
	    .maxSheets = 100,
	    .isFull = false,      // dynamic 
	    .outputPresent = true,
	},
	{0},
	{0},
	{0},
	{0},
    },
};


//--------------------------------------
// External Variables
//--------------------------------------

//--------------------------------------
// Global Variables
//--------------------------------------

//--------------------------------------
// A single mutex is used to protect a group of variables that must stay synchronized with each other.
// This was previously handled by locking a group of variables, reading them, modifying them, then
// unlocking them.  The current architecture does not support locking in the same way as has been done
// in the past.  Typically a component would only need to do this for a group of variables that it
// was the owner of.  It will call pthread_mutex_lock to "lock" the critical code section, then pthread_mutex_unlock
// when it is finished.  Each of the variables in the set is also protected with the mutex.  A single thread
// can get the mutex multiple times, but any other thread will block.  That way only the thread that did the
// "lock" will be able to access those variables until it releases the "lock".  Note that the mutex is
// declared as a global variable so other component files can use it, but it is not present in the component
// header file that is exposed to the rest of the system because other components should not need to use this
// mutex.  This component's files will use an extern declaration for this definition.
//--------------------------------------

pthread_mutex_t px_printVarMutex;

//--------------------------------------
// Local Variables
//--------------------------------------

static bool printInitDone = false;
static bool printPermLocalOnly = false; // set to true if NVRAM is not working for some reason
//static nvram_handle_t *handle;
static printPermVars_t printPermVars; 
static printTempVars_t printTempVars;
static sem_t px_printSem[NUM_PRINT_VARS];

//--------------------------------------
// External Function Prototypes
//--------------------------------------

//--------------------------------------
// Local Function Prototypes
//--------------------------------------

static error_type_t printDefaultPermVars(int id, bool doWrite);
static error_type_t printNVRAMInitFunction(nvram_init_type_t InitType, uint16_t Version, void *InitLocation, uint32_t InitLocationSize, void *unused);
static void printPermVarInit(void);
//static void printTempVarInit(void);

//--------------------------------------
// Administrative Functions
//--------------------------------------


/** 
 * \brief Boot time initialization function for the print variables.
 * 
 * This function is called at system startup after the NVRAM driver and partition manager have been
 * initialized.  It sets all the print variables to either default values or previously stored values
 * from NVRAM.
 * 
 * 
 **/ 

void printvar_init( void )
{
    int i;

    // create semaphores for all variables

    for (i = 0; i < NUM_PRINT_VARS; ++i)
    {
        REL_ASSERT(sem_init(&px_printSem[i], 0, 1) != -1);
    }
    // create mutex to keep groups of variables in sync
    REL_ASSERT(pthread_mutex_init(&px_printVarMutex, NULL) == 0); 

    printPermVarInit();  // initialize items stored in dynamic NVRAM next (may depend on static NVRAM items)

    /// now register oids...
    
    /// now register system_pji callbacks.

    printInitDone = true;
    return;
}

// FUNCTION NAME:  printNVRAMInitFunction
/** 
 * \brief Function called by the dynamic NVRAM driver to initialize or repair a block.
 * 
 * This function is called by the dynamic NVRAM driver during an NvramVariableRegister()
 * request.  It will be called either to initialize a new block of data or to repair an
 * existing block which has a bad CRC, version discrepancy, or change in size. 
 * 
 * \param[in] InitType type of handling needed:  nvram_init_type_t enum value from nvram_api.h
 *
 * \param[in,out] dataLoc pointer to existing block data on entry, destination for new
 * block data to be returned 
 * 
 * \return error_type_t
 * \retval OK
 *
 **/ 
 
static error_type_t printNVRAMInitFunction(nvram_init_type_t InitType, uint16_t Version, void *InitLocation, uint32_t InitLocationSize, void *unused)
{

    switch(InitType)
    {
        case NVRAM_NEW:
        case NVRAM_ERROR:
        case NVRAM_FACTORY_DEFAULTS:
        case NVRAM_VERSION_CHANGE: // for now, set defaults, production code should migrate versions
        {
            printDefaultPermVars(ALL_PRINT_VARS, false);  // do NOT write values to NVRAM
            ASSERT(sizeof(printPermVars_t) <= InitLocationSize);
            memcpy(InitLocation, &printPermVars, sizeof(printPermVars_t));
        }
        break;
        case NVRAM_LANG_DEFAULTS:
        case NVRAM_USER_DEFAULTS:
        {
            // no action required
        }
        break;
        default:
        {
            int tempValue = 0;
            error_type_t db_retval = FAIL;
            ASSERT(NVRAM_OK == InitType);

            // read out current values
            tempValue = db_get_var_int("CONF_PRNT", "SizeTypeTray_size", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.SizeTypeTray.size = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "SizeTypeTray_type", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.SizeTypeTray.type = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "SizeTypeTray_tray", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.SizeTypeTray.tray = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "SizeTypeTray_flags", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.SizeTypeTray.flags = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "JamRecovery", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.JamRecovery = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_0", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaType[TRAY_1] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaType[TRAY_1] = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaSize[TRAY_1] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaSize[TRAY_1] = tempValue;
            }

            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_1", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaType[TRAY_1] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaType[TRAY_2] = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaSize[TRAY_1] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaSize[TRAY_2] = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_2", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaType[TRAY_2] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaType[TRAY_3] = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaSize[TRAY_2] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaSize[TRAY_3] = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_3", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaType[TRAY_3] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaType[TRAY_4] = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaSize[TRAY_3] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaSize[TRAY_4] = tempValue;
            }
#if 0
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_4", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaType[TRAY_4] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaType[TRAY_5] = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_4", &db_retval);
            if(db_retval == OK)
            {
                //printPermVars.TrayConfig.mediaSize[TRAY_4] = tempValue;		// Tray Mod 
                printPermVars.TrayConfig.mediaSize[TRAY_5] = tempValue;
            }
#endif
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_DefmediaType", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.TrayConfig.DefmediaType = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_DefmediaSize", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.TrayConfig.DefmediaSize = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "PaperOutAction_Action", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.PaperOutAction.Action = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "PaperOutAction_Timeout", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.PaperOutAction.Timeout = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "DefaultPaperType", &db_retval);
            if(db_retval == OK)
            {
//                printPermVars.DefaultPaperType = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "DefaultPaperSize", &db_retval);
            if(db_retval == OK)
            {
//                printPermVars.DefaultPaperSize = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "Default_Src_Tray", &db_retval);
            if(db_retval == OK)
            {
                printPermVars.TrayConfig.defaultSourceTray = tempValue;
            }
            tempValue = db_get_var_int("CONF_PRNT", "MATCH_A4LT", &db_retval);
			if(db_retval == OK)
			{
				printPermVars.TrayConfig.trayMatchOptionA4LT = tempValue;
			}
        }
        break;
    }
    return (OK);
}

// FUNCTION NAME:  printPermVarInit
/** 
 * \brief Boot time initialization function for the print variables stored in dynamic NVRAM.
 * 
 * This function is called at system startup to read the variables out of dynamic NVRAM.  These
 * values are updated as appropriate if an e_DefaultAllValues or e_DefaultUserSettableValues reset
 * is specified.
 * 
 * 
 **/ 
static void printPermVarInit( void )
{
    error_type_t retval = FAIL; // internal use only

    if (!printInitDone)
    {
        // ask for block - if it doesn't exist or is corrupted, printNVRAMInitFunction() gets called by NvramVariableRegister()
    
        retval = db_variable_register(PRINT_VARS_ID, PRINT_VARS_VERSION, printNVRAMInitFunction);



        if ( retval != OK )
        {
            // there is something seriously wrong with NVRAM, so just set defaults and use the local copy       
            printDefaultPermVars(ALL_PRINT_VARS, false);  // in this case do NOT write values to NVRAM
            printPermLocalOnly = true;
        }
    }

    return;
}

// FUNCTION NAME:  printDefaultPermVars
/** 
 * \brief Function to initialize one or all variables stored in dynamic NVRAM.
 * 
 * This function is called to initialize all dynamic NVRAM variables at boot time or to initialize
 * individual values when their default function is called.  The individual API functions may be called
 * from outside the system and are required to write the new values to NVRAM.  This function is
 * intended to provide the ability to simply set the local copy without writing it out (useful for
 * initial block setup by the NVRAM driver and in the unlikely event that NVRAM is not working).
 * 
 * \param[in] id variable to initialize to default value (valid id's are listed under the "perm vars"
 * heading in the printVarId_t enum - ALL_PRINT_VARS refers to all perm vars)
 *
 * \param[in] doWrite true if value is to be written to NVRAM, false otherwise 
 * 
 * \return error_type_t
 * \retval OK
 * \retval FAIL if write fails
 *
 **/ 
 
static error_type_t printDefaultPermVars(int id, bool doWrite)
{
    bool       allVars;
    error_type_t retval = OK;

  //  REL_ASSERT(sem_wait(&px_printSem[id]) == 0);
    allVars = (id == ALL_PRINT_VARS) ? true : false;
    switch (id)
    {
        case ALL_PRINT_VARS:
            printPermVars.print_log_cur_index = 0;
            /* fall through */
        case e_SizeTypeTray:

            memset(&printPermVars.SizeTypeTray, 0, sizeof(printPermVars.SizeTypeTray));
            if (!allVars) break;


        case e_JamRecovery:

            printPermVars.JamRecovery = 1;   // on by default
            if (!allVars) break;

        case e_TrayConfig:
            {
                uint32_t i;
                for ( i = 0; i < NUM_INPUTS; i++ )
                {
                    printPermVars.TrayConfig.mediaSize[ i ] = MEDIASIZE_ANY;
                    printPermVars.TrayConfig.mediaType[ i ] = MEDIATYPE_ANY;

                }
                printPermVars.TrayConfig.DefmediaSize = MEDIASIZE_LETTER;
                printPermVars.TrayConfig.DefmediaType = MEDIATYPE_PLAIN_PAPER;

                if(!allVars) break;
            }

        case e_DefaultPaperSize:
            printPermVars.TrayConfig.DefmediaSize = MEDIASIZE_LETTER;

            if(!allVars) break;

        case e_DefaultPaperType:
            printPermVars.TrayConfig.DefmediaType = MEDIATYPE_PLAIN_PAPER;

            if(!allVars) break;
    
        default:

            break;
    }

    if (doWrite && !printPermLocalOnly)
    {
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_1", printPermVars.TrayConfig.mediaType[TRAY_1]);		// Tray Mod 
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", printPermVars.TrayConfig.mediaSize[TRAY_1]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_2", printPermVars.TrayConfig.mediaType[TRAY_2]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", printPermVars.TrayConfig.mediaSize[TRAY_2]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_3", printPermVars.TrayConfig.mediaType[TRAY_3]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", printPermVars.TrayConfig.mediaSize[TRAY_3]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_4", printPermVars.TrayConfig.mediaType[TRAY_4]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_4", printPermVars.TrayConfig.mediaSize[TRAY_4]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_0", printPermVars.TrayConfig.mediaType[TRAY_1]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", printPermVars.TrayConfig.mediaSize[TRAY_1]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_1", printPermVars.TrayConfig.mediaType[TRAY_2]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", printPermVars.TrayConfig.mediaSize[TRAY_2]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_2", printPermVars.TrayConfig.mediaType[TRAY_3]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", printPermVars.TrayConfig.mediaSize[TRAY_3]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_3", printPermVars.TrayConfig.mediaType[TRAY_4]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", printPermVars.TrayConfig.mediaSize[TRAY_4]);
        // retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_4", printPermVars.TrayConfig.mediaType[TRAY_5]);
        // retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_4", printPermVars.TrayConfig.mediaSize[TRAY_5]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_DefmediaType", printPermVars.TrayConfig.DefmediaType);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_DefmediaSize", printPermVars.TrayConfig.DefmediaSize);
         retval = db_set_var_int("CONF_PRNT", "SizeTypeTray_size", printPermVars.SizeTypeTray.size);
         retval = db_set_var_int("CONF_PRNT", "SizeTypeTray_type", printPermVars.SizeTypeTray.type);
         retval = db_set_var_int("CONF_PRNT", "SizeTypeTray_tray", printPermVars.SizeTypeTray.tray);
         retval = db_set_var_int("CONF_PRNT", "SizeTypeTray_flags", printPermVars.SizeTypeTray.flags);
//         retval = db_set_var_int("CONF_PRNT", "DefaultPaperSize", printPermVars.DefaultPaperSize);
//         retval = db_set_var_int("CONF_PRNT", "DefaultPaperType", printPermVars.DefaultPaperType);
         retval = db_set_var_int("CONF_PRNT", "PaperOutAction_Action", printPermVars.PaperOutAction.Action);
         retval = db_set_var_int("CONF_PRNT", "PaperOutAction_Timeout", printPermVars.PaperOutAction.Timeout);
         retval = db_set_var_int("CONF_PRNT", "JamRecovery", printPermVars.JamRecovery);
    }
    
    //REL_ASSERT(sem_post(&px_printSem[id]) == 0);
    return (retval);
}

//------------------------------------------------------------------------
// Permanent Variables (stored in dynamic NVRAM)
//------------------------------------------------------------------------
error_type_t printvar_get_trayconfig(mediacnfg_t *val)
{
    if (val == NULL) return (FAIL);
    REL_ASSERT(sem_wait(&px_printSem[e_TrayConfig]) == 0);
    memcpy(val,&printPermVars.TrayConfig, sizeof(printPermVars.TrayConfig));
    REL_ASSERT(sem_post(&px_printSem[e_TrayConfig]) == 0);
    return (OK);
}

error_type_t printvar_set_trayconfig_get_from_DB(void)
{
	int tempValue = 0;
	error_type_t db_retval = FAIL;
	
	REL_ASSERT(sem_wait(&px_printSem[e_TrayConfig]) == 0);
	
	tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_0", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaType[TRAY_1] = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaSize[TRAY_1] = tempValue;
    }

	tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_1", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaType[TRAY_2] = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaSize[TRAY_2] = tempValue;

    }
    tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_2", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaType[TRAY_3] = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaSize[TRAY_3] = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_3", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaType[TRAY_4] = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaSize[TRAY_4] = tempValue;
    }
#if 0 /*Pintree has 4 trays only*/
	tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaType_4", &db_retval);
	if(db_retval == OK)
	{
		printPermVars.TrayConfig.mediaType[TRAY_5] = tempValue;
	}
	tempValue = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_4", &db_retval);
	if(db_retval == OK)
	{
		printPermVars.TrayConfig.mediaSize[TRAY_5] = tempValue;
	}
#endif    
	// Default Source Tray
    tempValue = db_get_var_int("CONF_PRNT", "Default_Src_Tray", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.defaultSourceTray = tempValue;
    }
    
    // A4/LT Size Miss-Match option
    tempValue = db_get_var_int("CONF_PRNT", "MATCH_A4LT", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.trayMatchOptionA4LT = tempValue;
    }
    
    // Paper Texture
    tempValue = db_get_var_int("CONF_PRNT", "Texture_Plain", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.PlainPaper.Texture = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Texture_Trans", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.TransParency.Texture = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Texture_PaperLB", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.PaperLabel.Texture = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Texture_PlasticLB", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.VinylLabel.Texture = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Texture_Envel", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.Envelope.Texture = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Texture_Card", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.CardStock.Texture = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Texture_Post", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.PostCard.Texture = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Texture_Recycled", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.Recycled.Texture = tempValue;
    }
    
    // Paper Weight
    tempValue = db_get_var_int("CONF_PRNT", "Weight_Plain", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.PlainPaper.Weight = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Weight_Trans", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.TransParency.Weight = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Weight_PaperLB", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.PaperLabel.Weight = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Weight_PlasticLB", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.VinylLabel.Weight = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Weight_Envel", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.Envelope.Weight = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Weight_Card", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.CardStock.Weight = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Weight_Post", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.PostCard.Weight = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRNT", "Weight_Recycled", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.paperType.Recycled.Weight = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRINTSET", "PrintDarkness", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.mediaPrintDarkness = tempValue;
    }
    tempValue = db_get_var_int("CONF_PRINTSET", "SettingPriority", &db_retval);
    if(db_retval == OK)
    {
        printPermVars.TrayConfig.traySettingPriority = tempValue;
    }
        
	REL_ASSERT(sem_post(&px_printSem[e_TrayConfig]) == 0);
	return (OK);
}

error_type_t printvar_set_trayconfig(const mediacnfg_t *val)
{
    error_type_t retval = OK;
    MESSAGE msg;

    if (val == NULL) return (FAIL);
    if ( 0 == memcmp(val, &printPermVars.TrayConfig, sizeof(printPermVars.TrayConfig)))
	return retval;

    REL_ASSERT(sem_wait(&px_printSem[e_TrayConfig]) == 0);
    memcpy(&printPermVars.TrayConfig, val, sizeof(printPermVars.TrayConfig));
    if (!printPermLocalOnly)
    {
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_1", printPermVars.TrayConfig.mediaType[TRAY_1]);			// Tray Mod
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", printPermVars.TrayConfig.mediaSize[TRAY_1]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_2", printPermVars.TrayConfig.mediaType[TRAY_2]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", printPermVars.TrayConfig.mediaSize[TRAY_2]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_3", printPermVars.TrayConfig.mediaType[TRAY_3]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", printPermVars.TrayConfig.mediaSize[TRAY_3]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_4", printPermVars.TrayConfig.mediaType[TRAY_4]);
         //retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_4", printPermVars.TrayConfig.mediaSize[TRAY_4]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_0", printPermVars.TrayConfig.mediaType[TRAY_1]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", printPermVars.TrayConfig.mediaSize[TRAY_1]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_1", printPermVars.TrayConfig.mediaType[TRAY_2]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", printPermVars.TrayConfig.mediaSize[TRAY_2]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_2", printPermVars.TrayConfig.mediaType[TRAY_3]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", printPermVars.TrayConfig.mediaSize[TRAY_3]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_3", printPermVars.TrayConfig.mediaType[TRAY_4]);
         retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", printPermVars.TrayConfig.mediaSize[TRAY_4]);
      //   retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaType_4", printPermVars.TrayConfig.mediaType[TRAY_5]);
      //   retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_4", printPermVars.TrayConfig.mediaSize[TRAY_5]);
    }

    msg.msgType = MSG_TRAYCONFIG;

    SYMsgSend(PRINTMGRID, &msg);    // notify printmgr something has happened.

    REL_ASSERT(sem_post(&px_printSem[e_TrayConfig]) == 0);
    return retval;
}
//-------------------------
// SizeTypeTray
//-------------------------

error_type_t printvar_get_sizetypetray(size_type_tray_t *val)
{
    error_type_t retval = OK;
    if (val == NULL) return (FAIL);
    REL_ASSERT(sem_wait(&px_printSem[e_SizeTypeTray]) == 0);
    memcpy(val,&printPermVars.SizeTypeTray, sizeof(printPermVars.SizeTypeTray));
    REL_ASSERT(sem_post(&px_printSem[e_SizeTypeTray]) == 0);
    return retval;
}

error_type_t printvar_set_sizetypetray(const size_type_tray_t *val)
{
    error_type_t retval = OK;

    if (val == NULL) return(FAIL);
    REL_ASSERT(sem_wait(&px_printSem[e_SizeTypeTray]) == 0);


    memcpy(&printPermVars.SizeTypeTray, val, sizeof(printPermVars.SizeTypeTray));

    if (!printPermLocalOnly)
    {
         retval = db_set_var_int("CONF_PRNT", "SizeTypeTray_size", printPermVars.SizeTypeTray.size);
         retval = db_set_var_int("CONF_PRNT", "SizeTypeTray_type", printPermVars.SizeTypeTray.type);
         retval = db_set_var_int("CONF_PRNT", "SizeTypeTray_tray", printPermVars.SizeTypeTray.tray);
         retval = db_set_var_int("CONF_PRNT", "SizeTypeTray_flags", printPermVars.SizeTypeTray.flags);
    }

    REL_ASSERT(sem_post(&px_printSem[e_SizeTypeTray]) == 0);
    return(retval);
}

//------------------------------------------------------------------------
// Individual Variable Access Functions
//------------------------------------------------------------------------
//
// PrinterSetup
// 
error_type_t printvar_get_printersetup(printer_setup_t *setup)
{
    if(setup == NULL)
        return FAIL;
    REL_ASSERT(sem_wait(&px_printSem[e_PrinterSetup]) == 0);
    memcpy(setup, &default_printer_setup, sizeof(printer_setup_t));
    REL_ASSERT(sem_post(&px_printSem[e_PrinterSetup]) == 0);
    return OK;
}
error_type_t printvar_set_printersetup(const printer_setup_t *setup)
{

    if(setup == NULL)
        return FAIL;
    REL_ASSERT(sem_wait(&px_printSem[e_PrinterSetup]) == 0);
    memcpy(&default_printer_setup, setup, sizeof(printer_setup_t));
    REL_ASSERT(sem_post(&px_printSem[e_PrinterSetup]) == 0);
    return OK;
}


error_type_t printDefaultVarPrinterSetup(void)
{
    memset(&printTempVars.pSetup, 0, sizeof(printTempVars.pSetup));
    
    // configure the trays
    printTempVars.pSetup.inputTrays[TRAY_1].inputPresent = true;
    printTempVars.pSetup.inputTrays[TRAY_1].inputType = INPUT_FIRST;
    printTempVars.pSetup.inputTrays[TRAY_2].inputPresent = true;
    printTempVars.pSetup.inputTrays[TRAY_2].inputType = INPUT_MANUAL;    
    
    return OK;
}

error_type_t printvar_get_default_papersize(mediasize_t *val)
{
    if (val == NULL) 
	    return (FAIL);
    REL_ASSERT(sem_wait(&px_printSem[e_TrayConfig]) == 0);
    *val = printPermVars.TrayConfig.DefmediaSize;
    REL_ASSERT(sem_post(&px_printSem[e_TrayConfig]) == 0);
    return (OK);
}
error_type_t printvar_set_default_papersize(const mediasize_t *val)
{
    error_type_t retval=OK;
    if(sem_wait(&px_printSem[e_TrayConfig]) != 0)
        return FAIL;
    printPermVars.TrayConfig.DefmediaSize = *val;
    if (!printPermLocalOnly)
    {
//         retval = db_set_var_int("CONF_PRNT", "DefaultPaperSize", printPermVars.DefaultPaperSize);
    }
    sem_post(&px_printSem[e_TrayConfig]);
    return retval;
}

error_type_t printvar_get_default_papertype(mediatype_t *val)
{
    ASSERT(val != NULL);
    if(sem_wait(&px_printSem[e_TrayConfig]) != 0)
        return FAIL;
    *val = printPermVars.TrayConfig.DefmediaType;
    sem_post(&px_printSem[e_TrayConfig]);
    return OK;
}

error_type_t printvar_set_default_papertype(const mediatype_t val)
{
    error_type_t retval=OK;
    if(sem_wait(&px_printSem[e_TrayConfig]) != 0)
        return FAIL;
    printPermVars.TrayConfig.DefmediaType = val;
    if (!printPermLocalOnly)
    {
//        retval = db_set_var_int("CONF_PRNT", "DefaultPaperType", printPermVars.DefaultPaperType);
    }
    sem_post(&px_printSem[e_TrayConfig]);
    return retval;
}

error_type_t printvar_get_paperoutaction(paperoutaction_t *val)
{
    if(val == NULL)
        return FAIL;
    REL_ASSERT(sem_wait(&px_printSem[e_PaperOutAction]) == 0);
    memcpy(val,&printPermVars.PaperOutAction,sizeof(printPermVars.PaperOutAction));
    REL_ASSERT(sem_post(&px_printSem[e_PaperOutAction]) == 0);
    return (OK);
}
error_type_t printvar_set_paperoutaction(paperoutaction_t *val)
{
    error_type_t retval=OK;
    if(val == NULL)
        return FAIL;
    REL_ASSERT(sem_wait(&px_printSem[e_PaperOutAction]) == 0);
    memcpy(&printPermVars.PaperOutAction,val, sizeof(printPermVars.PaperOutAction));
    if (!printPermLocalOnly)
    {
        retval = db_set_var_int("CONF_PRNT", "PaperOutAction_Action", printPermVars.PaperOutAction.Action);
        retval = db_set_var_int("CONF_PRNT", "PaperOutAction_Timeout", printPermVars.PaperOutAction.Timeout);
    }
    REL_ASSERT(sem_post(&px_printSem[e_PaperOutAction]) == 0);
    return retval;
}
//-------------------------
// JamRecovery
//-------------------------

error_type_t printvar_get_jamrecovery(uint8_t *val)
{
    if (val == NULL) return (FAIL);
    REL_ASSERT(sem_wait(&px_printSem[e_JamRecovery]) == 0);
    *val = printPermVars.JamRecovery;
    REL_ASSERT(sem_post(&px_printSem[e_JamRecovery]) == 0);
    return (OK);
}

error_type_t printvar_set_jamrecovery(const uint8_t *val)
{
    error_type_t retval = OK;

    if (val == NULL) 
        return (FAIL);

    if (*val < 3) // 0 = NO, 1 = YES, 2 = AUTO
    {
        REL_ASSERT(sem_wait(&px_printSem[e_JamRecovery]) == 0);
        printPermVars.JamRecovery = *val;

        if (!printPermLocalOnly)
        {
            retval = db_set_var_int("CONF_PRNT", "JamRecovery", printPermVars.JamRecovery);
        }
        REL_ASSERT(sem_post(&px_printSem[e_JamRecovery]) == 0);
    }
    else
    {
        retval = FAIL;
    }

    return (retval);
}

error_type_t printvar_get_log_entry( uint32_t index, print_log_entry_t * val )
{
    error_type_t e_res;

    e_res = OK;

    if ( index >= PRINT_LOG_SIZE )
    {
        e_res = DATA_ACC_ERR_INVALID_VALUE;
    }
    if ( NULL == val )
    {
        e_res = DATA_ACC_ERR_NULL_PTR;
    }

    REL_ASSERT(sem_wait(&px_printSem[e_LogEntry]) == 0);
    if ( OK == e_res )
    {
        val->page_count = printPermVars.log_entry[ index ].page_count;
        val->code = printPermVars.log_entry[ index ].code;

        #ifdef HAVE_RTC
        memcpy( &( val->time ),
                &( printPermVars.log_entry[ index ].time ),
                sizeof ( date_time_t ) );
        #endif

    }
    REL_ASSERT(sem_post(&px_printSem[e_LogEntry]) == 0);

    return e_res;
}

error_type_t printvar_get_max_logentries( uint32_t * index )
{
    error_type_t e_res;

    e_res = OK;
    if ( NULL == index )
    {
        e_res = DATA_ACC_ERR_NULL_PTR;
    }

    if ( OK == e_res )
    {
        *index = PRINT_LOG_SIZE;
    }

    return e_res;
}

error_type_t printvar_set_logentry( uint32_t val )
{
    error_type_t e_res = OK;
    print_log_entry_t log_entry;

    REL_ASSERT(sem_wait(&px_printSem[e_LogEntry]) == 0);

    // TODO:  set print counts here
    log_entry.code = val;

#ifdef HAVE_RTC
    e_res = rtc_get_time(
            &( log_entry.time ) );
    ASSERT( OK == e_res );
#endif


    if (!printPermLocalOnly)
    {
        e_res = db_set_printlog(&log_entry);
    }

    REL_ASSERT(sem_post(&px_printSem[e_LogEntry]) == 0);

    return e_res;
}
