/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file nvram_db_shim.c
 *
 *  \brief NVRAM DB shim
 *
 *  This file implements a DB interface to the NVRAM API layer.
 *
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/stat.h>
#include "error_types.h"
#include "memAPI.h"
#include "list.h"
#include "lassert.h"
#include "checksum.h"
#include "deviceid.h"
#include "nvram_api.h"
//#include "partitionMgrAPI.h"
#include "platform.h"
#include "nvram_dev_shim.h"
#include "nvram_config.h"
#include "nvram_logger.h"
#include "ioutils.h"
#include "data_access_api.h"
#include "db_api.h"
#include <unistd.h>
#include "agMessage.h"
#include "agRouter.h"
#include "SysUI_Interface.h"
#include "SysServiceMgr.h"
#include "spec.h"
#include "t35_cty.h"
#include <sys/time.h>

/*
 *  Local (static) variables
 */
#define NVRAM_DATA_PATH     "/app/nvram"
static char *g_nvram_data_path = NVRAM_DATA_PATH;

#define DB_REGULAR_FILENAME "karas.db"
#define DB_FULLPATH_LENGTH 30
#define BACKUP_DB_PATH "/backup/karas_backup.db"
sqlite3* dbInstance;
int db_version;
uint32_t gDBFailCode = 0;
extern bool bSepCartridge;

/*
 * FUNCTION NAME: InitNvramDevShims
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t InitNvramDevShim(void)
{
    struct stat st;
    int db_retval = FAIL;
    char dbpath[DB_FULLPATH_LENGTH];

    /* check to see if the nvram path is overwritten by the environment variable */
    char *env_nvram = getenv("NVRAM");
    if (env_nvram != (char *)0)
        g_nvram_data_path = env_nvram;

    /* Make sure NVRAM directory exists */
    if (stat(g_nvram_data_path, &st) != 0)
    {
        /* Make sure NVRAM directory exists */
        mkdir(g_nvram_data_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    /* generate DB path */
    memset(dbpath, 0, sizeof(dbpath));
    snprintf(dbpath, DB_FULLPATH_LENGTH, "%s/%s", g_nvram_data_path, DB_REGULAR_FILENAME);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): dbpath : %s\n", __LINE__, dbpath);

    db_retval = db_setup(dbpath);

    return OK;
} 


/*
 * FUNCTION NAME: LocateVarBlock 
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t LocateVarBlock(nvram_handle_t *nHandle)
{
    error_type_t ret = FAIL;
    char filename[5];
    char filepath[255];
    struct stat st;
    
    if (nHandle == NULL)
    {
        ASSERT(false);
        return FAIL;
    }
    
    /* if handle is currently open, close it */
    if (nHandle->fd != -1)
    {
        close(nHandle->fd);
        nHandle->fd = -1;
    }

    /* generate file path */
    memset(filename, 0, sizeof(filename));
    memcpy(filename, &nHandle->BlockLabel[0], 4);
    sprintf(filepath, "%s/%s", g_nvram_data_path, filename);

    /* deterine if file exists, set MaxLength if it does */
    if (stat(filepath, &st) == 0)
    {
        /* set size based on existing file */
        nHandle->MaxLength = st.st_size;
        
        /* attempt to open existing file */
        nHandle->fd = open(filepath, O_RDWR);
        if (nHandle->fd != -1)
        {
            ret = OK;
        }
        else
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to open existing file %s, errno = 0x%x\n", __LINE__, filepath, errno);
        }
    }
       
    return ret;
}


/*
 * FUNCTION NAME: RegisterVarBlock 
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t RegisterVarBlock(nvram_handle_t *nHandle)
{
    error_type_t ret = FAIL;
    char filename[5];
    char filepath[255];
    
    /* Validate input parameters */
    if ( (nHandle == NULL) || (nHandle->BlockLabel == NULL) || 
         (nHandle->MaxLength == 0) )
    {
        return FAIL;
    }

    /* if handle is currently open, close it */
    if (nHandle->fd != -1)
    {
        close(nHandle->fd);
        nHandle->fd = -1;
    }

    /* generate file path */
    memset(filename, 0, sizeof(filename));
    memcpy(filename, &nHandle->BlockLabel[0], 4);
    sprintf(filepath, "%s/%s", g_nvram_data_path, filename);

    /* open file, create if does not exist */
    nHandle->fd = open(filepath, O_RDWR | O_CREAT, 0666);
    if (nHandle->fd != -1)
    {
        // seek to new EOF-1
        lseek(nHandle->fd, nHandle->MaxLength-1, SEEK_SET);
        
        // something needs to be written at end for file to have new size, 
        // so just write an empty string which results in a '\0'.
        write(nHandle->fd, "", 1);
        fsync(nHandle->fd);
        ret = OK;
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to open file %s, errno = 0x%x\n", __LINE__, filepath, errno);
    }

       
    return ret;
}


/*
 * FUNCTION NAME: ReleaseVarBlock
 *
 *   See nvram_dev_shim.h for function documentation.
 */
error_type_t ReleaseVarBlock(nvram_handle_t *nHandle)
{
    error_type_t ret = FAIL;
    char filename[5];
    char filepath[255];
    
    /* Validate input parameters */
    if ( (nHandle == NULL) || (nHandle->BlockLabel == NULL) || 
         (nHandle->MaxLength == 0) )
    {
        return FAIL;
    }
    
    /* if handle is currently open, close it */
    if (nHandle->fd != -1)
    {
        close(nHandle->fd);
        nHandle->fd = -1;
    }

    /* generate file path */
    memset(filename, 0, sizeof(filename));
    memcpy(filename, &nHandle->BlockLabel[0], 4);
    sprintf(filepath, "%s/%s", g_nvram_data_path, filename);

    // delete file
    if (remove(filepath) == 0)
    {
        ret = OK;
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to remove file %s, errno = 0x%x\n", __LINE__, filepath, errno);
    }
       
    return ret;
}

/*
 * FUNCTION NAME: VarBlockRead
 *
 *   See nvram_dev_shim.h for function documentation.
 */
uint32_t VarBlockRead(nvram_handle_t *nHandle, int8_t *blockPtr, uint64_t offset, uint32_t size)
{
    uint32_t bytes_read = 0;
    
    if ((nHandle == NULL) || (nHandle->fd == -1) || (blockPtr == NULL) || (size == 0))
    {
        return 0;
    }

    // seek to offset
    if (lseek(nHandle->fd, offset, SEEK_SET) >= 0)
    {
        int bytes;

        // read bytes from file
        bytes = read(nHandle->fd, blockPtr, size);
        if (bytes > 0)
        {
            bytes_read = (uint32_t)bytes;
        }
        else
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable read block of %d bytes, errno = 0x%x\n", __LINE__, size, errno);
        }
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to r-seek to offset %d, errno = 0x%x\n", __LINE__, offset, errno);
    }
       
    return bytes_read;
}


/*
 * FUNCTION NAME: VarBlockWrite
 *
 *   See nvram_dev_shim.h for function documentation.
 */
uint32_t VarBlockWrite(nvram_handle_t *nHandle, int8_t *blockPtr)
{
    uint32_t bytes_written = 0;
    
    if ((nHandle == NULL) || (nHandle->fd == -1) || (nHandle->MaxLength == 0)  || (blockPtr == NULL))
    {
        return 0;
    }

    // seek to SOF
    if (lseek(nHandle->fd, 0, SEEK_SET) >= 0)
    {
        int bytes;
        
        // write bytes to file
        bytes = write(nHandle->fd, blockPtr, nHandle->MaxLength);
        if (bytes > 0)
        {
            // make sure any buffers are flushed to storage device
            fsync(nHandle->fd);
            bytes_written = (uint32_t)bytes;
        }
        else
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable write block of %d bytes\n, errno = 0x%x", __LINE__, nHandle->MaxLength, errno);
        }
    }
    else
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): unable to w-seek to SOF, errno = 0x%x\n", __LINE__, errno);
    }
       
    return bytes_written;
}


/*
 * FUNCTION NAME: VarBlockCheckCRC
 *
 *   See nvram_dev_shim.h for function documentation.
 */
bool VarBlockCheckCRC(char * label, VARBLOCK_HEADER_t *varBlock, uint32_t erased_data_val)
{
    // no need to call this when HAVE_NVRAM_FILE defined
    ASSERT(false);
    return false;
}

error_type_t db_setup(char* dbpath)
{
	error_type_t db_retval = FAIL;
    int32_t sqlite_retval = 0;

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): db_setup start\n", __LINE__);

    /* Threadsafe mode check */
    sqlite_retval = sqlite3_threadsafe();
    if (sqlite_retval == 0)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Please recompile sqlite3 library. errno = 0x%x\n", __LINE__, sqlite_retval);
        return FAIL;
    }
    else
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Sqlite3 threadsafe mode\n", __LINE__);
    }

	if(access(dbpath, F_OK) == 0)
	{
		if (db_integrityCheck(dbpath) == FAIL) // Abnormal DB case
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL | Integrity check fail : %s\n", __LINE__, dbpath);
			if(access(BACKUP_DB_PATH, F_OK) == 0)
			{
				if(db_integrityCheck(BACKUP_DB_PATH) == OK)
				{
					db_retval = db_copy(BACKUP_DB_PATH, dbpath);
					if(db_retval == OK)
					{
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB OK\n", __LINE__);
						gDBFailCode = SC_SYSTEM_RECOVERY_SUCCESS;
					}
					else
					{
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL\n", __LINE__);
						gDBFailCode = SC_SYSTEM_BACKUPDB_COPY_FAIL;
					}
				}
				else
				{
					DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL | Integrity check fail : %s\n", __LINE__, BACKUP_DB_PATH);
					gDBFailCode = SC_SYSTEM_BACKUPDB_INTEGRITY_CHECK_FAIL;
				}
			}
			else
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL | No backup DB\n", __LINE__);
				gDBFailCode = SC_SYSTEM_BACKUPDB_NONE;
			}
		}
		else                                                   // Original DB OK case
		{
			if(access(BACKUP_DB_PATH, F_OK) == 0)
			{
				if(db_integrityCheck(BACKUP_DB_PATH) == OK)
				{
					FILE *bkresult = NULL;
					char backupDB[300];
					char backupTLI[50];
					char originalTLI[50];
					memset(backupDB, 0, sizeof(backupDB));
					memset(backupTLI, 0, sizeof(backupTLI));
					memset(originalTLI, 0, sizeof(originalTLI));
					snprintf(backupDB,sizeof(backupDB),"sqlite3 %s 'SELECT MachineTLI FROM INST_MACHINE;'", BACKUP_DB_PATH);
					bkresult=popen(backupDB,"r");
					if(bkresult == NULL)
					{
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): sqlite3 cmd fail\n", __LINE__);
					}
					else
					{
						fscanf(bkresult,"%s",backupTLI);
						pclose(bkresult);
					}

					bkresult = NULL;
					snprintf(backupDB,sizeof(backupDB),"sqlite3 %s 'SELECT MachineTLI FROM INST_MACHINE;'", dbpath);
					bkresult=popen(backupDB,"r");
					if(bkresult == NULL)
					{
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): sqlite3 cmd fail\n", __LINE__);
					}
					else
					{
						fscanf(bkresult,"%s",originalTLI);
						pclose(bkresult);
					}

					if(!strcmp(originalTLI, "NOTLI") || (originalTLI[0] == 0))
					{
						if(!strcmp(backupTLI, "NOTLI") || (backupTLI[0] == 0))
						{
							DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Backup DB has not TLI or SERIAL\n", __LINE__);
							DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): No Recovery...\n", __LINE__);
						}
						else
						{
							DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): TLI integrity check fail... DB recovery!!!\n", __LINE__);
							db_retval = db_copy(BACKUP_DB_PATH, dbpath);
							if(db_retval == OK)
							{
								DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): TLI : %s\n", __LINE__, originalTLI);
								gDBFailCode = SC_SYSTEM_RECOVERY_SUCCESS;
							}
							else
							{
								DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL\n", __LINE__);
								gDBFailCode = SC_SYSTEM_BACKUPDB_COPY_FAIL;
							}
						}
					}
					else
					{
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): TLI : %s\n", __LINE__, originalTLI);
					}
				}
				else
				{
					DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL | Integrity check fail : %s\n", __LINE__, BACKUP_DB_PATH);
					// Backup DB malformed... Copy Original DB to Backup DB...
					db_retval = db_copy(dbpath, BACKUP_DB_PATH);
					if(db_retval == OK)
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB OK\n", __LINE__);
					else
					{
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL\n", __LINE__);
						gDBFailCode = SC_SYSTEM_BACKUPDB_COPY_FAIL;
					}
				}
			}
			else
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): No backup DB!!!\n", __LINE__);
				// No SC!!! -> Maybe it is before the TESTEND step.
			}
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): No DB\n", __LINE__);
		if(access(BACKUP_DB_PATH, F_OK) == 0)
		{
			if(db_integrityCheck(BACKUP_DB_PATH) == OK)
			{
				db_retval = db_copy(BACKUP_DB_PATH, dbpath);
				if(db_retval == OK)
				{
					DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB OK\n", __LINE__);
					gDBFailCode = SC_SYSTEM_RECOVERY_SUCCESS;
				}
				else
				{
					DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL\n", __LINE__);
					gDBFailCode = SC_SYSTEM_BACKUPDB_COPY_FAIL;
				}
			}
			else
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL | Integrity check fail : %s\n", __LINE__, BACKUP_DB_PATH);
			}
		}
		else
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Recover DB FAIL | No backup DB\n", __LINE__);
		}
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Auto DB Creation Start!!!\n", __LINE__);
	}

	SysServiceCheckSystemStatus();

    sqlite_retval = sqlite3_open(dbpath, &dbInstance);
    if (sqlite_retval != SQLITE_OK)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQLite open failed : %d\n", __LINE__, sqlite_retval);
        return FAIL;
    }
    else
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): SQLite open success\n", __LINE__);
    }

    return OK;
}

error_type_t db_copy(char* inputFile, char* outputFile)
{
	error_type_t db_retval = FAIL;
	FILE* pInputFile = NULL;
	FILE* pOutputFile = NULL;
	DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): db_copy : %s -> %s\n", __LINE__, inputFile, outputFile);
	pInputFile = fopen(inputFile,"rb");
	if(pInputFile == NULL)
	{
		DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): File open error : %s\n", __LINE__, inputFile);
		db_retval = FAIL;
	}
	else
	{
		fclose(pInputFile);
		if(db_integrityCheck(inputFile) == OK)
		{
			int TotalReadByte = 0, TotalWriteByte = 0;
			int StageNo = 0;
			unlink(outputFile);
			pInputFile = fopen(inputFile,"rb");
			pOutputFile = fopen(outputFile,"wb");
			while(!feof(pInputFile))
			{
				int readByte=0,writeByte=0;
				int readBuf[10240];
				StageNo++;
				readByte=fread(readBuf,1,10240,pInputFile);
				writeByte=fwrite(readBuf,1,readByte,pOutputFile);
				TotalReadByte+=readByte;
				TotalWriteByte+=writeByte;
				DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): (Stage #%03d), %d KBytes Read, %d KBytes Write\n", __LINE__, StageNo, readByte/1024, writeByte/1024);
			}
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): (Copy Finalize) %dBytes[%dKBytes] Read, %dBytes[%dKBytes] Write\n", __LINE__, TotalReadByte, TotalReadByte/1024, TotalWriteByte, TotalWriteByte/1024);
			if(fsync(pOutputFile->_fileno) == -1)
			{
				DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): fsync is failed!\n", __LINE__);
			}
			fclose(pInputFile);
			fclose(pOutputFile);
			if (TotalWriteByte != TotalReadByte)
			{
				unlink(outputFile);
				DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): (Size Check) File size different!, Copy Fail\n", __LINE__);

				// SC???
				db_retval = FAIL;
			}
			else
			{
				DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): (Size Check) File size OK\n", __LINE__);
				db_retval = OK;
			}
		}
		else
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Copy DB FAIL | Integrity check fail : %s\n", __LINE__, inputFile);
			db_retval = FAIL;
		}
	}
	return db_retval;
}

error_type_t db_file_integrityCheck(char * DBName)
{
    FILE *fp = NULL;
    fp = fopen(DBName, "rb");

    if (fp == NULL)
	{
		DBG_PRINTF(LOG_ALERT|DBG_NVRAM, "NVRAMAPI(%d): DB file open failed\n", __LINE__);
		return FAIL;
	}
	else
	{
		fclose(fp);
	}
	return OK;
}

error_type_t db_integrityCheck(char * DBName)
{
	int sqlite_return = 0;
	sqlite3	*pDB = NULL;
	error_type_t retval = 0;
    char* errMsg;

	retval = db_file_integrityCheck(DBName);
	if(retval == FAIL)
		return FAIL;

	sqlite_return = sqlite3_open(DBName,&pDB);
	if (sqlite_return != SQLITE_OK)
	{
		pDB = NULL;
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): SQLite open failed:%d\n", __LINE__);
		return FAIL;
	}
	else
	{
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): SQLite file open success\n");
		sqlite_return = sqlite3_exec( pDB, "PRAGMA integrity_check;", NULL, NULL, &errMsg );

		if (sqlite_return == SQLITE_OK)
		{
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): DB Integrity OK\n");
			sqlite3_close(pDB);
			return OK;
		}
		else
		{
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): DB Integrity Failure %d\n", sqlite_return);
			sqlite3_close(pDB);
			return FAIL;
		}
	}

	return OK;
}

error_type_t db_initialize( void )
{
    error_type_t db_retval = 0;
    char *retChar = 0;
	char query[256] = {0, };
	db_version = 0;

    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): NvramDBInitialize\n", __LINE__);

	db_retval = db_trySQLiteExec("BEGIN TRANSACTION;");

	// NO INST_MACHINE Table case.
	retChar = NULL;
	retChar = db_get_var_query("SELECT sql FROM sqlite_master WHERE name=\"INST_MACHINE\";", &db_retval);

	if((db_retval == OK) && (retChar == NULL))
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_MACHINE\" (\"MachineTLI\" TEXT,\"MachineSerialNo\" TEXT,\"DBVERSION\" INTEGER NOT NULL);");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}
	}

	// NO Data of the INST_MACHINE Table case.
	retChar = NULL;
	retChar = db_get_var_query("SELECT * FROM INST_MACHINE;", &db_retval);

	if((db_retval == OK) && (retChar == NULL))
	{
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_MACHINE VALUES (\"NOTLI\",\"NOSERIAL\",4)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Insert DB Fail\n", __LINE__);
		}
	}

	retChar = NULL;
	retChar = db_get_var_query("SELECT DBVERSION FROM INST_MACHINE WHERE rowid=1;", &db_retval);
	if((db_retval == OK) && (retChar != NULL))
	{
		db_version = atoi(retChar);
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): DB Version read fail\n", __LINE__);
		return db_retval;
	}

	if(db_version < 5)
	{
		// Create Query Test
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_PRNT\" (\"ID\" INTEGER PRIMARY KEY NOT NULL,\"SizeTypeTray_size\" INTEGER NOT NULL,\"SizeTypeTray_type\" INTEGER NOT NULL,\"SizeTypeTray_tray\" INTEGER NOT NULL,\"SizeTypeTray_flags\" INTEGER NOT NULL,\"JamRecovery\" INTEGER NOT NULL,\"TrayConfig_mediaType_1\" INTEGER NOT NULL,\"TrayConfig_mediaSize_1\" INTEGER NOT NULL,\"TrayConfig_mediaType_2\" INTEGER NOT NULL,\"TrayConfig_mediaSize_2\" INTEGER NOT NULL,\"TrayConfig_mediaType_3\" INTEGER NOT NULL,\"TrayConfig_mediaSize_3\" INTEGER NOT NULL,\"TrayConfig_mediaType_4\" INTEGER NOT NULL,\"TrayConfig_mediaSize_4\" INTEGER NOT NULL,\"TrayConfig_DefmediaType\" INTEGER NOT NULL,\"TrayConfig_DefmediaSize\" INTEGER NOT NULL,\"PaperOutAction_Action\" INTEGER NOT NULL,\"PaperOutAction_Timeout\" INTEGER NOT NULL,\"DefaultPaperType\" INTEGER NOT NULL,\"DefaultPaperSize\" INTEGER NOT NULL,\"duplex\" INTEGER NOT NULL,\"offset_x\" INTEGER NOT NULL,\"offset_y\" INTEGER NOT NULL);");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}

		retChar = db_get_var_query("SELECT sql FROM sqlite_master WHERE name=\"CONF_PRNT\" AND sql LIKE \"%duplex%\";", &db_retval);
		if((db_retval == OK) && (retChar == NULL))
		{
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"duplex\" INTEGER DEFAULT 0 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"offset_x\" INTEGER DEFAULT 0 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"offset_y\" INTEGER DEFAULT 0 NOT NULL;");

			if(db_retval)
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
				return db_retval;
			}
		}

		// temp column
		retChar = NULL;
		retChar = db_get_var_query("SELECT sql FROM sqlite_master WHERE name=\"CONF_PRNT\" AND sql LIKE \"%tray_no%\";", &db_retval);
		if((db_retval == OK) && (retChar == NULL))
		{
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"tray_no\" INTEGER DEFAULT 0 NOT NULL;");
			if(db_retval)
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
				return db_retval;
			}
		}
		db_version = 5;
	}
	if(db_version == 5)
	{
		db_retval = db_trySQLiteExec("DROP TABLE \"CONF_PRNT\"");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): DROP table Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_PRNT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"SizeTypeTray_size\" INTEGER NOT NULL,\"SizeTypeTray_type\" INTEGER NOT NULL,\"SizeTypeTray_tray\" INTEGER NOT NULL,\"SizeTypeTray_flags\" INTEGER NOT NULL,\"JamRecovery\" INTEGER NOT NULL,\"TrayConfig_mediaType_1\" INTEGER NOT NULL,\"TrayConfig_mediaSize_1\" INTEGER NOT NULL,\"TrayConfig_mediaType_2\" INTEGER NOT NULL,\"TrayConfig_mediaSize_2\" INTEGER NOT NULL,\"TrayConfig_mediaType_3\" INTEGER NOT NULL,\"TrayConfig_mediaSize_3\" INTEGER NOT NULL,\"TrayConfig_mediaType_4\" INTEGER NOT NULL,\"TrayConfig_mediaSize_4\" INTEGER NOT NULL,\"TrayConfig_DefmediaType\" INTEGER NOT NULL,\"TrayConfig_DefmediaSize\" INTEGER NOT NULL,\"PaperOutAction_Action\" INTEGER NOT NULL,\"PaperOutAction_Timeout\" INTEGER NOT NULL);");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"LOG_PRNT\" (\"ID\" INTEGER PRIMARY KEY NOT NULL,\"page_count\" INTEGER NOT NULL,\"code\" INTEGER NOT NULL,\"time\" INTEGER DEFAULT 0 NOT NULL)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_Margin\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TOP\" INTEGER NOT NULL,\"LEFT\" INTEGER NOT NULL,\"VERTICALMAGNIFICATION\" INTEGER NOT NULL,\"HORIZONMAGNIFICATION\" INTEGER NOT NULL)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_DuplexMargin\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TOP\" INTEGER NOT NULL,\"LEFT\" INTEGER NOT NULL,\"VERTICALMAGNIFICATION\" INTEGER NOT NULL,\"HORIZONMAGNIFICATION\" INTEGER NOT NULL)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_LSUPWM\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"B1_PWM1\" INTEGER NOT NULL)");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_PAGECOUNT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TotalPrint\" INTEGER NOT NULL,\"HostPrint\" INTEGER NOT NULL,\"ReportPrint\" INTEGER NOT NULL,\"USBDrivePrint\" INTEGER NOT NULL)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"LOG_PRNT\" (\"ID\" INTEGER PRIMARY KEY NOT NULL,\"page_count\" INTEGER NOT NULL,\"code\" INTEGER NOT NULL,\"time\" INTEGER DEFAULT 0 NOT NULL)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Create DB Fail\n", __LINE__);
			return db_retval;
		}

		retChar = NULL;
		retChar = db_get_var_query("SELECT sql FROM sqlite_master WHERE name=\"RECD_PAGECOUNT\" AND sql LIKE \"%DuplexPrint%\";", &db_retval);
		if((db_retval == OK) && (retChar == NULL))
		{
			db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_PAGECOUNT\" ADD COLUMN \"DuplexPrint\" INTEGER DEFAULT 0 NOT NULL;");
			if(db_retval)
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
				return db_retval;
			}
		}

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PRNT VALUES (1,0,0,0,0,0,0,25,0,25,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PRNT VALUES (2,0,0,0,0,0,0,25,0,25,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PRNT VALUES (3,0,0,0,0,0,0,25,0,25,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PRNT VALUES (4,0,0,0,0,0,0,25,0,25,0,0,0,0,0,0,0,0)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Insert DB Fail\n", __LINE__);
		}

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_Margin VALUES (1,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_Margin VALUES (2,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_Margin VALUES (3,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_Margin VALUES (4,0,0,0,0)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Insert DB Fail\n", __LINE__);
		}

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_DuplexMargin VALUES (1,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_DuplexMargin VALUES (2,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_DuplexMargin VALUES (3,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_DuplexMargin VALUES (4,0,0,0,0)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Insert DB Fail\n", __LINE__);
		}

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_LSUPWM VALUES (1,-1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_LSUPWM VALUES (2,-1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_LSUPWM VALUES (3,-1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_LSUPWM VALUES (4,-1)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Insert DB Fail\n", __LINE__);
		}

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_PAGECOUNT VALUES (1,0,0,0,0,0)");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Insert DB Fail\n", __LINE__);
		}

		db_version = 6;
    }
	if(db_version == 6)
	{
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Texture_Plain\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Texture_Trans\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Texture_PaperLB\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Texture_PlasticLB\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Texture_Envel\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Texture_Card\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Texture_Post\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Weight_Plain\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Weight_Trans\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Weight_PaperLB\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Weight_Plastic\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Weight_Envel\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Weight_Card\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Weight_Post\" INTEGER DEFAULT 2 NOT NULL;");
			db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Default_Src_Tray\" INTEGER DEFAULT 0 NOT NULL;");

			if(db_retval)
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
				return db_retval;
			}
			db_version = 7;
	}
	if(db_version == 7)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_SYSTEM\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"MeasureUnit\" INTEGER NOT NULL,\"SleepMin\" INTEGER NOT NULL,\"Linguistic\" INTEGER NOT NULL)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SYSTEM VALUES (1,0,60,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SYSTEM VALUES (2,0,60,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SYSTEM VALUES (3,0,60,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SYSTEM VALUES (4,0,60,1)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_DRUMCOUNT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"CountMode1p\" INTEGER NOT NULL,\"CountMode2p\" INTEGER NOT NULL,\"CountMode3p\" INTEGER NOT NULL,\"CountMode4p\" INTEGER NOT NULL,\"CountMode5p\" INTEGER NOT NULL,\"CountMode6p\" INTEGER NOT NULL,\"CountMode11p\" INTEGER NOT NULL,\"CountMode21p\" INTEGER NOT NULL,\"CountMode50p\" INTEGER NOT NULL,\"DrumLifePageCount\" INTEGER NOT NULL,\"FreeMotorOnDrumCount\" INTEGER NOT NULL,\"Count\" INTEGER NOT NULL,\"CountInPercent\" INTEGER NOT NULL,\"Motor\" INTEGER NOT NULL)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_DRUMCOUNT VALUES (1,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_MACHINECOUNT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"CountMode1p\" INTEGER NOT NULL,\"CountMode2p\" INTEGER NOT NULL,\"CountMode3p\" INTEGER NOT NULL,\"CountMode4p\" INTEGER NOT NULL,\"CountMode5p\" INTEGER NOT NULL,\"CountMode6p\" INTEGER NOT NULL,\"CountMode11p\" INTEGER NOT NULL,\"CountMode21p\" INTEGER NOT NULL,\"CountMode50p\" INTEGER NOT NULL, \"TotalPrint\" INTEGER NOT NULL,\"HostPrint\" INTEGER NOT NULL,\"ReportPrint\" INTEGER NOT NULL,\"USBDrivePrint\" INTEGER NOT NULL,\"DuplexPrint\" INTEGER NOT NULL)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_MACHINECOUNT VALUES (1,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		// Recover data of the RECD_PAGECOUNT table.
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINECOUNT SET TotalPrint=(SELECT TotalPrint FROM RECD_PAGECOUNT WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINECOUNT SET HostPrint=(SELECT HostPrint FROM RECD_PAGECOUNT WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINECOUNT SET ReportPrint=(SELECT ReportPrint FROM RECD_PAGECOUNT WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINECOUNT SET USBDrivePrint=(SELECT USBDrivePrint FROM RECD_PAGECOUNT WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINECOUNT SET DuplexPrint=(SELECT DuplexPrint FROM RECD_PAGECOUNT WHERE NO=1) WHERE NO=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 8;
	}
	if(db_version == 8)
	{
		db_retval = db_trySQLiteExec("DROP TABLE RECD_PAGECOUNT");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 9;
	}
	if(db_version == 9)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_1=254");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_2=254");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 10;
	}
	if(db_version == 10)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_FAXUP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"StationID\" TEXT,\"StationNum\" TEXT,\"TTI\" INTEGER NOT NULL,\"LineMonitor\" INTEGER NOT NULL,\"Volume\" INTEGER NOT NULL,\"TxResolution\" INTEGER NOT NULL,\"TxImageType\" INTEGER NOT NULL,\"TxContrast\" INTEGER NOT NULL,\"TxPrefixSet\" INTEGER NOT NULL,\"TxPrefixNum\" TEXT,\"TxRetryNum\" INTEGER NOT NULL,\"TxRetryInterval\" INTEGER NOT NULL,\"TxDialMode\" INTEGER NOT NULL,\"TxStartSpeed\" INTEGER NOT NULL,\"TxECM\" INTEGER NOT NULL,\"RxForwardSet\" INTEGER NOT NULL,\"RxForwardNum\" TEXT,\"RxConfSet\" INTEGER NOT NULL,\"RxConfPW\" TEXT,\"RxRingNum\" INTEGER NOT NULL,\"RxReductionSel\" INTEGER NOT NULL,\"RxReductionCustom\" TEXT,\"RxStartSpeed\" INTEGER NOT NULL,\"RxPrintNum\" INTEGER NOT NULL,\"RepTCRSet\" INTEGER NOT NULL,\"RepTxRxSet\" INTEGER NOT NULL,\"RxForwardNumSize\" INTEGER NOT NULL,\"TxConfSet\" INTEGER NOT NULL,\"TxConfPW\" TEXT,\"TxEncoding\" INTEGER NOT NULL,\"RxAutoPagePartition\" INTEGER NOT NULL,\"TxOriginalSize\" INTEGER NOT NULL,\"TimeoutFlatbed\" INTEGER NOT NULL,\"TimeoutOnhook\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXUP VALUES (1,\"\",\"\",1,1,3,0,0,2,0,\"\",2,2,0,0,1,0,\"\",0,\"\",4,0,\"\",0,1,1,1,0,0,\"\",3,0,-9999,30,10)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXUP VALUES (2,\"\",\"\",1,1,3,0,0,2,0,\"\",2,2,0,0,1,0,\"\",0,\"\",4,0,\"\",0,1,1,1,0,0,\"\",3,0,-9999,30,10)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXUP VALUES (3,\"\",\"\",1,1,3,0,0,2,0,\"\",2,2,0,0,1,0,\"\",0,\"\",4,0,\"\",0,1,1,1,0,0,\"\",3,0,-9999,30,10)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXUP VALUES (4,\"\",\"\",1,1,3,0,0,2,0,\"\",2,2,0,0,1,0,\"\",0,\"\",4,0,\"\",0,1,1,1,0,0,\"\",3,0,-9999,30,10)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 11;
	}
	if(db_version == 11)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_PRINTSET\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"PrintDarkness\" INTEGER NOT NULL,\"PrintTimeout\" INTEGER NOT NULL,\"SettingPriority\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PRINTSET VALUES (1,6,180,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PRINTSET VALUES (2,6,180,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PRINTSET VALUES (3,6,180,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PRINTSET VALUES (4,6,180,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 12;
	}
	if(db_version == 12)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"PrintMode\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 13;
	}
	if(db_version == 13)
	{
		db_retval = db_trySQLiteExec("DROP TABLE \"CONF_FAXUP\"");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): DROP table Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_FAXUP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"StationID\" TEXT,\"StationNum\" TEXT,\"TTI\" INTEGER NOT NULL,\"LineMonitor\" INTEGER NOT NULL,\"Volume\" INTEGER NOT NULL,\"TxResolution\" INTEGER NOT NULL,\"TxImageType\" INTEGER NOT NULL,\"TxContrast\" INTEGER NOT NULL,\"TxPrefixSet\" INTEGER NOT NULL,\"TxPrefixNum\" TEXT,\"TxRetryNum\" INTEGER NOT NULL,\"TxRetryInterval\" INTEGER NOT NULL,\"TxECM\" INTEGER NOT NULL,\"RxForwardSet\" INTEGER NOT NULL,\"RxForwardNum\" TEXT,\"RxConfSet\" INTEGER NOT NULL,\"RxConfPW\" TEXT,\"RxRingNum\" INTEGER NOT NULL,\"RxReductionSel\" INTEGER NOT NULL,\"RepTCRSet\" INTEGER NOT NULL,\"RepTxRxSet\" INTEGER NOT NULL,\"TxConfSet\" INTEGER NOT NULL,\"TxConfPW\" TEXT,\"TxOriginalSize\" INTEGER NOT NULL,\"TimeoutFlatbed\" INTEGER NOT NULL,\"TimeoutOnhook\" INTEGER NOT NULL,\"PrintMode\" INTEGER NOT NULL,\"DiscardLength\" INTEGER NOT NULL,\"DuplexPrint\" INTEGER NOT NULL,\"FaxInputTray\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXUP VALUES (1,\"\",\"\",1,1,3,0,0,2,0,\"\",2,2,1,0,\"\",0,\"\",4,0,1,1,0,\"\",-9999,30,10,0,10,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXUP VALUES (2,\"\",\"\",1,1,3,0,0,2,0,\"\",2,2,1,0,\"\",0,\"\",4,0,1,1,0,\"\",-9999,30,10,0,10,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXUP VALUES (3,\"\",\"\",1,1,3,0,0,2,0,\"\",2,2,1,0,\"\",0,\"\",4,0,1,1,0,\"\",-9999,30,10,0,10,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXUP VALUES (4,\"\",\"\",1,1,3,0,0,2,0,\"\",2,2,1,0,\"\",0,\"\",4,0,1,1,0,\"\",-9999,30,10,0,10,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_FAXSP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TxStartSpeed\" INTEGER NOT NULL,\"RxStartSpeed\" INTEGER NOT NULL,\"TxEncoding\" INTEGER NOT NULL,\"SelectCountry\" INTEGER NOT NULL,\"DialTimeout\" INTEGER NOT NULL,\"TransmitLevel\" INTEGER NOT NULL,\"ReceiveLevel\" INTEGER NOT NULL,\"DTMFLowLevel\" INTEGER NOT NULL,\"DTMFHighLevel\" INTEGER NOT NULL,\"DialtoneThreshold\" INTEGER NOT NULL,\"ProgressThreshold\" INTEGER NOT NULL,\"PulseMakeTime\" INTEGER NOT NULL,\"PulseBreakTime\" INTEGER NOT NULL,\"PulseDialType\" INTEGER NOT NULL,\"InterdigitDelay\" INTEGER NOT NULL,\"DCCharacteristic\" INTEGER NOT NULL,\"Impedance\" INTEGER NOT NULL,\"BusytoneCycle\" INTEGER NOT NULL,\"BusytoneMinOnTime\" INTEGER NOT NULL,\"BusytoneMaxOnTime\" INTEGER NOT NULL,\"BusytoneMinOffTime\" INTEGER NOT NULL,\"BusytoneMaxOffTime\" INTEGER NOT NULL,\"CongestToneCycle\" INTEGER NOT NULL,\"CongestToneMinOnTime\" INTEGER NOT NULL,\"CongestToneMaxOnTime\" INTEGER NOT NULL,\"CongestToneMinOffTime\" INTEGER NOT NULL,\"CongestToneMaxOffTime\" INTEGER NOT NULL,\"ExtensionSupport\" INTEGER NOT NULL,\"PulseFallTime\" INTEGER NOT NULL,\"ExtensionVoltageThreshold\" INTEGER NOT NULL,\"JbigAutoMMR\" INTEGER NOT NULL,\"MinDetectableTipRingVolt\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXSP VALUES (1,0,0,3,181,50,-9,-11,-10,-7,-1850,0,32,64,0,850,1,0,3,45,55,45,55,4,24,36,14,26,0,0,0,1,18)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXSP VALUES (2,0,0,3,181,50,-9,-11,-10,-7,-1850,0,32,64,0,850,1,0,3,45,55,45,55,4,24,36,14,26,0,0,0,1,18)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXSP VALUES (3,0,0,3,181,50,-9,-11,-10,-7,-1850,0,32,64,0,850,1,0,3,45,55,45,55,4,24,36,14,26,0,0,0,1,18)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXSP VALUES (4,0,0,3,181,50,-9,-11,-10,-7,-1850,0,32,64,0,850,1,0,3,45,55,45,55,4,24,36,14,26,0,0,0,1,18)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXLOG\" (\"NO\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\"DateTime\" INTEGER,\"FaxMode\" INTEGER,\"ECM\" INTEGER,\"Resolution\" INTEGER,\"RemoteNum\" TEXT,\"Duration\" INTEGER,\"PageNum\" INTEGER,\"ConnectSpeed\" INTEGER,\"Result\" INTEGER,\"RecName\" INTEGER)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXSPLOG\" (\"NO\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\"DateTime\" INTEGER,\"FaxMode\" INTEGER,\"ECM\" INTEGER,\"Resolution\" INTEGER,\"RemoteNum\" TEXT,\"Duration\" INTEGER,\"PageNum\" INTEGER,\"ConnectSpeed\" INTEGER,\"Result\" INTEGER,\"RecName\" INTEGER)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXMEMFILE\" (\"NO\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\"DateTime\" INTEGER,\"Resolution\" INTEGER,\"RemoteNum\" TEXT,\"Page\" INTEGER,\"JobType\" INTEGER)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 14;
	}
	if(db_version == 14)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"BeepLevel\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 15;
	}
	if(db_version == 15)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_SCANSUB\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"FBLeftMargin\" INTEGER NOT NULL,\"FBRightMargin\" INTEGER NOT NULL,\"FBTopMargin\" INTEGER NOT NULL,\"FBBottomMargin\" INTEGER NOT NULL,\"ADFLeftMargin\" INTEGER NOT NULL,\"ADFRightMargin\" INTEGER NOT NULL,\"ADFTopMargin\" INTEGER NOT NULL,\"ADFBottomMargin\" INTEGER NOT NULL,\"FBHMAG\" INTEGER NOT NULL,\"FBVMAG\" INTEGER NOT NULL,\"ADFHMAG\" INTEGER NOT NULL,\"ADFVMAG\" INTEGER NOT NULL,\"RollerPositionX\" INTEGER NOT NULL,\"RollerPositionY\" INTEGER NOT NULL,\"RollerPositionZ\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSUB VALUES (1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSUB VALUES (2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSUB VALUES (3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSUB VALUES (4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 16;
	}
	if(db_version == 16)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET Linguistic=1 WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET Linguistic=1 WHERE NO=2");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET Linguistic=1 WHERE NO=3");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 17;
	}
	if(db_version == 17)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"DrumTLI\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"DrumSerialNo\" TEXT;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET DrumTLI=\"NOTLI\"");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET DrumSerialNo=\"NOSERIAL\"");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 18;
	}
	if(db_version == 18)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=60 WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=60 WHERE NO=2");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=60 WHERE NO=3");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 19;
	}
	if(db_version == 19)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE INST_MACHINE RENAME TO INST_MACHINE_backup;");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_MACHINE\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"MachineTLI\" TEXT,\"MachineSerialNo\" TEXT,\"DBVERSION\" INTEGER NOT NULL,\"DrumTLI\" TEXT,\"DrumSerialNo\" TEXT);");
		db_retval = db_trySQLiteExec("INSERT INTO INST_MACHINE(MachineTLI, MachineSerialNo, DBVERSION, DrumTLI, DrumSerialNo) SELECT * FROM INST_MACHINE_backup;");
		db_retval = db_trySQLiteExec("DROP TABLE INST_MACHINE_backup");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 20;
	}
	if(db_version == 20)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_3=254");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_4=254");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaType_1=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaType_2=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaType_3=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaType_4=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 21;
	}
	if(db_version == 21)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET SelectCountry=97");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET TransmitLevel=10");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET DTMFLowLevel=60");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET DTMFHighLevel=40");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET PulseFallTime=1");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"DialToneDetection\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 22;
	}
	if(db_version == 22)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_LSUPWM SET B1_PWM1=75");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 23;
	}
	if(db_version == 23)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_1=554");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_2=554");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_3=554");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_4=9");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 24;
	}
	if(db_version == 24)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_SCANSET\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Color\" INTEGER NOT NULL,\"Resolution\" INTEGER NOT NULL,\"OriginalSize\" INTEGER NOT NULL,\"FileFormat\" INTEGER NOT NULL,\"JpegQuality\" INTEGER NOT NULL,\"Contrast\" INTEGER NOT NULL,\"Brightness\" INTEGER NOT NULL,\"DefaultDest\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSET VALUES (1,2,5,-9999,5,1,2,4,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSET VALUES (2,2,5,-9999,5,1,2,4,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSET VALUES (3,2,5,-9999,5,1,2,4,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSET VALUES (4,2,5,-9999,5,1,2,4,1)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_COPYSET\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Quantity\" INTEGER NOT NULL,\"ScannerSrc\" INTEGER NOT NULL,\"OriginalSize\" INTEGER NOT NULL,\"PrintSrcTray\" INTEGER NOT NULL,\"PrintDestTray\" INTEGER NOT NULL,\"Scale\" INTEGER NOT NULL,\"Collation\" INTEGER NOT NULL,\"Border\" INTEGER NOT NULL,\"Nup\" INTEGER NOT NULL,\"NupBorder\" INTEGER NOT NULL,\"Duplex\" INTEGER NOT NULL,\"PRContents\" INTEGER NOT NULL,\"Brightness\" INTEGER NOT NULL,\"Darkness\" INTEGER NOT NULL,\"Contrast\" INTEGER NOT NULL,\"BgRemove\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_COPYSET VALUES (1,1,0,-9999,0,1,0,2,0,1,0,0,1,4,6,2,4)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_COPYSET VALUES (2,1,0,-9999,0,1,0,2,0,1,0,0,1,4,6,2,4)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_COPYSET VALUES (3,1,0,-9999,0,1,0,2,0,1,0,0,1,4,6,2,4)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_COPYSET VALUES (4,1,0,-9999,0,1,0,2,0,1,0,0,1,4,6,2,4)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 25;
	}
	if(db_version == 25)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE CONF_PRNT RENAME TO CONF_PRNT_backup;");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_PRNT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"SizeTypeTray_size\" INTEGER NOT NULL,\"SizeTypeTray_type\" INTEGER NOT NULL,\"SizeTypeTray_tray\" INTEGER NOT NULL,\"SizeTypeTray_flags\" INTEGER NOT NULL,\"JamRecovery\" INTEGER NOT NULL,\"TrayConfig_mediaType_1\" INTEGER NOT NULL,\"TrayConfig_mediaSize_1\" INTEGER NOT NULL,\"TrayConfig_mediaType_2\" INTEGER NOT NULL,\"TrayConfig_mediaSize_2\" INTEGER NOT NULL,\"TrayConfig_mediaType_3\" INTEGER NOT NULL,\"TrayConfig_mediaSize_3\" INTEGER NOT NULL,\"TrayConfig_mediaType_4\" INTEGER NOT NULL,\"TrayConfig_mediaSize_4\" INTEGER NOT NULL,\"TrayConfig_DefmediaType\" INTEGER NOT NULL,\"TrayConfig_DefmediaSize\" INTEGER NOT NULL,\"PaperOutAction_Action\" INTEGER NOT NULL,\"PaperOutAction_Timeout\" INTEGER NOT NULL, \"Texture_Plain\" INTEGER NOT NULL,\"Texture_Trans\" INTEGER NOT NULL,\"Texture_PaperLB\" INTEGER NOT NULL,\"Texture_PlasticLB\" INTEGER NOT NULL,\"Texture_Envel\" INTEGER NOT NULL,\"Texture_Card\" INTEGER NOT NULL,\"Texture_Post\" INTEGER NOT NULL,\"Weight_Plain\" INTEGER NOT NULL,\"Weight_Trans\" INTEGER NOT NULL,\"Weight_PaperLB\" INTEGER NOT NULL,\"Weight_PlasticLB\" INTEGER NOT NULL,\"Weight_Envel\" INTEGER NOT NULL,\"Weight_Card\" INTEGER NOT NULL,\"Weight_Post\" INTEGER NOT NULL,\"Default_Src_Tray\" INTEGER NOT NULL);");
		db_retval = db_trySQLiteExec("INSERT INTO CONF_PRNT SELECT * FROM CONF_PRNT_backup;");
		db_retval = db_trySQLiteExec("DROP TABLE CONF_PRNT_backup");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 26;
	}
	if(db_version == 26)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_ERRLOG\" (\"NO\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\"ErrorType\" INTEGER,\"ErrorDesc\" INTEGER,\"SubJobType\" INTEGER,\"JobID\" INTEGER,\"Date\" INTEGER,\"UptimeSec\" INTEGER,\"PageCount\" INTEGER,\"PORCount\" INTEGER,\"IP\" TEXT,\"Desc\" TEXT)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 27;
	}
	if(db_version == 27)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"Location\" TEXT;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET Location=\"NONE\"");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 28;
	}
	if(db_version == 28)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_SSIDBOOK\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"ifSelected\" INTEGER DEFAULT 0,\"SSID\" TEXT NOT NULL,\"APMacAddr\" TEXT NOT NULL,\"ifHidden\" INTEGER DEFAULT 0,\"AuthType\" INTEGER DEFAULT 0,\"GroupCipher\" INTEGER DEFAULT 0,\"PairCipher\" INTEGER DEFAULT 0,\"ifSupportPreAuth\" INTEGER DEFAULT 0,\"AuthSuite\" INTEGER DEFAULT 0,\"Passkey1\" TEXT,\"Passkey2\" TEXT,\"Passkey3\" TEXT,\"Passkey4\" TEXT)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_UAPSTATUS\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"ifActive\" INTEGER NOT NULL,\"SSID\" TEXT NOT NULL,\"AuthType\" INTEGER NOT NULL,\"Passkey1\" TEXT NOT NULL,\"isMACFilter\" INTEGER NOT NULL,\"MACFilterType\" INTEGER NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_UAPSTATUS VALUES (1,0,'NONE',0,'NONE',0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_UAPSTATUS VALUES (2,0,'NONE',0,'NONE',0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_UAPSTATUS VALUES (3,0,'NONE',0,'NONE',0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_UAPSTATUS VALUES (4,0,'NONE',0,'NONE',0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_UAPMACLIST\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"MACAddress\" TEXT NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 29;
	}
	if(db_version == 29)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DuplexMargin\" ADD COLUMN \"A4_LT_GAP\" INTEGER DEFAULT 0;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 30;
	}
	if(db_version == 30)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET Default_Src_Tray=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_version = 31;
	}
	if(db_version == 31)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRINTSET SET SettingPriority=0");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_version = 32;
	}
	if(db_version == 32)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"DEFN_ERRDESC_ERROR\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"NAME\" TEXT NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (0,\"General error\");insert into DEFN_ERRDESC_ERROR values (1,\"Software internal error #01\");insert into DEFN_ERRDESC_ERROR values (2,\"Software internal error #02\");insert into DEFN_ERRDESC_ERROR values (3,\"Software internal error #03\");insert into DEFN_ERRDESC_ERROR values (4,\"Software internal error #04\");insert into DEFN_ERRDESC_ERROR values (5,\"Software internal error #05\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (10,\"Unknown error\");insert into DEFN_ERRDESC_ERROR values (60,\"Inefficient memory (cannot resume)\");insert into DEFN_ERRDESC_ERROR values (61,\"Inefficient memory (can resume)\");insert into DEFN_ERRDESC_ERROR values (62,\"Kernel memory allocation fail\");insert into DEFN_ERRDESC_ERROR values (70,\"Inefficient disk space (cannot resume)\");insert into DEFN_ERRDESC_ERROR values (71,\"Inefficient disk space (can resume)\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (80,\"USB connection broken while communication\");insert into DEFN_ERRDESC_ERROR values (81,\"Network connection broken while communication\");insert into DEFN_ERRDESC_ERROR values (82,\"Timeout occurred while communication\");insert into DEFN_ERRDESC_ERROR values (83,\"Component not response\");insert into DEFN_ERRDESC_ERROR values (100,\"Unknown TLI reported\");insert into DEFN_ERRDESC_ERROR values (101,\"Unknown Serial Code reported\");insert into DEFN_ERRDESC_ERROR values (102,\"Illegal connection detected\");insert into DEFN_ERRDESC_ERROR values (103,\"Illegal job request detected\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (1100,\"Engine cover opened\");insert into DEFN_ERRDESC_ERROR values (1101,\"Engine front cover opened\");insert into DEFN_ERRDESC_ERROR values (1102,\"Engine cover #02 opened\");insert into DEFN_ERRDESC_ERROR values (1103,\"Engine cover #03 opened\");insert into DEFN_ERRDESC_ERROR values (1104,\"Engine cover #04 opened\");insert into DEFN_ERRDESC_ERROR values (1105,\"Engine cover #05 opened\");insert into DEFN_ERRDESC_ERROR values (1106,\"Engine cover #06 opened\");insert into DEFN_ERRDESC_ERROR values (1107,\"Engine cover #07 opened\");insert into DEFN_ERRDESC_ERROR values (1108,\"Engine cover #08 opened\");insert into DEFN_ERRDESC_ERROR values (1109,\"Engine cover #09 opened\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (1200,\"Engine jam\");insert into DEFN_ERRDESC_ERROR values (1201,\"Engine jam on tray #1\");insert into DEFN_ERRDESC_ERROR values (1202,\"Engine jam on tray #2\");insert into DEFN_ERRDESC_ERROR values (1203,\"Engine jam on multi purpose tray\");insert into DEFN_ERRDESC_ERROR values (1204,\"Engine jam on input sensor\");insert into DEFN_ERRDESC_ERROR values (1205,\"Engine jam on input/output sensor\");insert into DEFN_ERRDESC_ERROR values (1206,\"Engine jam on output sensor\");insert into DEFN_ERRDESC_ERROR values (1207,\"Engine jam on duplex device\");insert into DEFN_ERRDESC_ERROR values (1208,\"Engine jam on #08 sensor\");insert into DEFN_ERRDESC_ERROR values (1209,\"Engine jam on #09 sensor\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (1300,\"Engine communication error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (1400,\"Engine sensor error\");insert into DEFN_ERRDESC_ERROR values (1401,\"Engine sensor #01 error\");insert into DEFN_ERRDESC_ERROR values (1402,\"Engine sensor #02 error\");insert into DEFN_ERRDESC_ERROR values (1403,\"Engine sensor #03 error\");insert into DEFN_ERRDESC_ERROR values (1404,\"Engine sensor #04 error\");insert into DEFN_ERRDESC_ERROR values (1405,\"Engine sensor #05 error\");insert into DEFN_ERRDESC_ERROR values (1406,\"Engine sensor #06 error\");insert into DEFN_ERRDESC_ERROR values (1407,\"Engine sensor #07 error\");insert into DEFN_ERRDESC_ERROR values (1408,\"Engine sensor #08 error\");insert into DEFN_ERRDESC_ERROR values (1409,\"Engine sensor #09 error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (1500,\"Engine motor error\");insert into DEFN_ERRDESC_ERROR values (1501,\"Engine motor #01 error\");insert into DEFN_ERRDESC_ERROR values (1502,\"Engine motor #02 error\");insert into DEFN_ERRDESC_ERROR values (1503,\"Engine motor #03 error\");insert into DEFN_ERRDESC_ERROR values (1504,\"Engine motor #04 error\");insert into DEFN_ERRDESC_ERROR values (1505,\"Engine motor #05 error\");insert into DEFN_ERRDESC_ERROR values (1506,\"Engine motor #06 error\");insert into DEFN_ERRDESC_ERROR values (1507,\"Engine motor #07 error\");insert into DEFN_ERRDESC_ERROR values (1508,\"Engine motor #08 error\");insert into DEFN_ERRDESC_ERROR values (1509,\"Engine motor #09 error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (2000,\"Scanner error\");insert into DEFN_ERRDESC_ERROR values (2001,\"Try A3 scan with paper in ADF\");insert into DEFN_ERRDESC_ERROR values (2002,\"PI memory overrun\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (2100,\"Scanner cover opened\");insert into DEFN_ERRDESC_ERROR values (2101,\"Scanner cover #01 opened\");insert into DEFN_ERRDESC_ERROR values (2102,\"Scanner cover #02 opened\");insert into DEFN_ERRDESC_ERROR values (2103,\"Scanner cover #03 opened\");insert into DEFN_ERRDESC_ERROR values (2104,\"Scanner cover #04 opened\");insert into DEFN_ERRDESC_ERROR values (2105,\"Scanner cover #05 opened\");insert into DEFN_ERRDESC_ERROR values (2106,\"Scanner cover #06 opened\");insert into DEFN_ERRDESC_ERROR values (2107,\"Scanner cover #07 opened\");insert into DEFN_ERRDESC_ERROR values (2108,\"Scanner cover #08 opened\");insert into DEFN_ERRDESC_ERROR values (2109,\"Scanner cover #09 opened\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (2200,\"Scanner jam\");insert into DEFN_ERRDESC_ERROR values (2201,\"Scanner ADF miss-feed jam\");insert into DEFN_ERRDESC_ERROR values (2202,\"Scanner ADF long paper jam\");insert into DEFN_ERRDESC_ERROR values (2203,\"Scanner jam on #03 sensor\");insert into DEFN_ERRDESC_ERROR values (2204,\"Scanner jam on #04 sensor\");insert into DEFN_ERRDESC_ERROR values (2205,\"Scanner jam on #05 sensor\");insert into DEFN_ERRDESC_ERROR values (2206,\"Scanner jam on #06 sensor\");insert into DEFN_ERRDESC_ERROR values (2207,\"Scanner jam on #07 sensor\");insert into DEFN_ERRDESC_ERROR values (2208,\"Scanner jam on #08 sensor\");insert into DEFN_ERRDESC_ERROR values (2209,\"Scanner jam on #09 sensor\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (2300,\"Scanner communication error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (2400,\"Scanner sensor error\");insert into DEFN_ERRDESC_ERROR values (2401,\"Scanner sensor #01 error\");insert into DEFN_ERRDESC_ERROR values (2402,\"Scanner sensor #02 error\");insert into DEFN_ERRDESC_ERROR values (2403,\"Scanner sensor #03 error\");insert into DEFN_ERRDESC_ERROR values (2404,\"Scanner sensor #04 error\");insert into DEFN_ERRDESC_ERROR values (2405,\"Scanner sensor #05 error\");insert into DEFN_ERRDESC_ERROR values (2406,\"Scanner sensor #06 error\");insert into DEFN_ERRDESC_ERROR values (2407,\"Scanner sensor #07 error\");insert into DEFN_ERRDESC_ERROR values (2408,\"Scanner sensor #08 error\");insert into DEFN_ERRDESC_ERROR values (2409,\"Scanner sensor #09 error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (2500,\"Scanner motor error\");insert into DEFN_ERRDESC_ERROR values (2501,\"Scanner motor #01 error\");insert into DEFN_ERRDESC_ERROR values (2502,\"Scanner motor #02 error\");insert into DEFN_ERRDESC_ERROR values (2503,\"Scanner motor #03 error\");insert into DEFN_ERRDESC_ERROR values (2504,\"Scanner motor #04 error\");insert into DEFN_ERRDESC_ERROR values (2505,\"Scanner motor #05 error\");insert into DEFN_ERRDESC_ERROR values (2506,\"Scanner motor #06 error\");insert into DEFN_ERRDESC_ERROR values (2507,\"Scanner motor #07 error\");insert into DEFN_ERRDESC_ERROR values (2508,\"Scanner motor #08 error\");insert into DEFN_ERRDESC_ERROR values (2509,\"Scanner motor #09 error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (3000,\"Finisher error\");insert into DEFN_ERRDESC_ERROR values (3360,\"Finisher communication error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (4000,\"Fax error\");insert into DEFN_ERRDESC_ERROR values (4001,\"No connect error\");insert into DEFN_ERRDESC_ERROR values (4002,\"Remote terminal is busy \");insert into DEFN_ERRDESC_ERROR values (4003,\"Phone line error\");insert into DEFN_ERRDESC_ERROR values (4004,\"Blacklist error\");insert into DEFN_ERRDESC_ERROR values (4005,\"Timeout error\");insert into DEFN_ERRDESC_ERROR values (4006,\"Bad response error\");insert into DEFN_ERRDESC_ERROR values (4007,\"Bad Tx error\");insert into DEFN_ERRDESC_ERROR values (4008,\"Bad Rx error\");insert into DEFN_ERRDESC_ERROR values (4009,\"No Document error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (4010,\"No Memory error\");insert into DEFN_ERRDESC_ERROR values (4011,\"Memory assert error\");insert into DEFN_ERRDESC_ERROR values (4012,\"Bad Document error\");insert into DEFN_ERRDESC_ERROR values (4013,\"User abort error (stop key)\");insert into DEFN_ERRDESC_ERROR values (4014,\"Tx RSRC error\");insert into DEFN_ERRDESC_ERROR values (4015,\"Rx RSRC error\");insert into DEFN_ERRDESC_ERROR values (4016,\"Check resource error\");insert into DEFN_ERRDESC_ERROR values (4017,\"Check document error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (5000,\"File error\");insert into DEFN_ERRDESC_ERROR values (5001,\"Too big file size (cannot resume)\");insert into DEFN_ERRDESC_ERROR values (5002,\"Invalid file format (cannot resume)\");insert into DEFN_ERRDESC_ERROR values (5003,\"Image width/height ratio is too big or small\");insert into DEFN_ERRDESC_ERROR values (5004,\"File not found\");insert into DEFN_ERRDESC_ERROR values (5005,\"General file I/O error\");insert into DEFN_ERRDESC_ERROR values (5006,\"File is broken\");insert into DEFN_ERRDESC_ERROR values (5007,\"Writing to file failed\");insert into DEFN_ERRDESC_ERROR values (5008,\"Reading from file failed\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (6000,\"Feeding tray error\");insert into DEFN_ERRDESC_ERROR values (6600,\"Engine tray paper empty\");insert into DEFN_ERRDESC_ERROR values (6601,\"Engine tray 1 paper empty\");insert into DEFN_ERRDESC_ERROR values (6602,\"Engine tray 2 paper empty\");insert into DEFN_ERRDESC_ERROR values (6603,\"Engine tray 3 paper empty\");insert into DEFN_ERRDESC_ERROR values (6604,\"Engine tray 4 paper empty\");insert into DEFN_ERRDESC_ERROR values (6605,\"Engine tray 5 paper empty\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (6641,\"Engine high capacity tray 1 empty\");insert into DEFN_ERRDESC_ERROR values (6642,\"Engine high capacity tray 2 empty\");insert into DEFN_ERRDESC_ERROR values (6643,\"Engine high capacity tray 3 empty\");insert into DEFN_ERRDESC_ERROR values (6644,\"Engine high capacity tray 4 empty\");insert into DEFN_ERRDESC_ERROR values (6645,\"Engine high capacity tray 5 empty\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (6681,\"Engine multi purpose tray 1 empty\");insert into DEFN_ERRDESC_ERROR values (6682,\"Engine multi purpose tray 2 empty\");insert into DEFN_ERRDESC_ERROR values (6683,\"Engine multi purpose tray 3 empty\");insert into DEFN_ERRDESC_ERROR values (6684,\"Engine multi purpose tray 4 empty\");insert into DEFN_ERRDESC_ERROR values (6685,\"Engine multi purpose tray 5 empty\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (7000,\"Network card error\");insert into DEFN_ERRDESC_ERROR values (7101,\"Network connection fail\");insert into DEFN_ERRDESC_ERROR values (7102,\"Email send fail\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (8000,\"USB monitor error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (9000,\"Wifi dongle error\");insert into DEFN_ERRDESC_ERROR values (9101,\"Wifi connection fail\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (10000,\"Postscript parser error\");insert into DEFN_ERRDESC_ERROR values (11000,\"PCL parser error\");insert into DEFN_ERRDESC_ERROR values (12000,\"PDL parser error\");insert into DEFN_ERRDESC_ERROR values (13000,\"GDI parser error\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_ERROR values (20000,\"Function test client error\");");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"DEFN_ERRDESC_FATAL\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"NAME\" TEXT NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (0,\"General failure\");insert into DEFN_ERRDESC_FATAL values (1,\"SW internal failure #01\");insert into DEFN_ERRDESC_FATAL values (2,\"SW internal failure #02\");insert into DEFN_ERRDESC_FATAL values (3,\"SW internal failure #03\");insert into DEFN_ERRDESC_FATAL values (4,\"SW internal failure #04\");insert into DEFN_ERRDESC_FATAL values (5,\"SW internal failure #05\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (7,\"Null pointer exception\");insert into DEFN_ERRDESC_FATAL values (8,\"Unacceptable message received\");insert into DEFN_ERRDESC_FATAL values (9,\"Unacceptable state transition\");insert into DEFN_ERRDESC_FATAL values (62,\"Kernel memory allocation failure\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (100,\"Unacceptable TLI found\");insert into DEFN_ERRDESC_FATAL values (101,\"Unacceptable passcode found\");insert into DEFN_ERRDESC_FATAL values (102,\"Database broken\");insert into DEFN_ERRDESC_FATAL values (103,\"File system broken\");insert into DEFN_ERRDESC_FATAL values (104,\"Sub-client library not found\");insert into DEFN_ERRDESC_FATAL values (105,\"Sub-component library not found\");insert into DEFN_ERRDESC_FATAL values (106,\"Program execution failure\");insert into DEFN_ERRDESC_FATAL values (107,\"Loading driver failure\");insert into DEFN_ERRDESC_FATAL values (108,\"Ethernet address not set\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (999,\"General PYO attacks OAK machine\");insert into DEFN_ERRDESC_FATAL values (1000,\"Engine failure\");insert into DEFN_ERRDESC_FATAL values (1001,\"Engine SW failure\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (1400,\"Engine sensor failure\");insert into DEFN_ERRDESC_FATAL values (1410,\"Engine fuser undertemperature white printing\");insert into DEFN_ERRDESC_FATAL values (1411,\"Engine fuser undertemperature white standby\");insert into DEFN_ERRDESC_FATAL values (1412,\"Engine fuser cannot reach standby temperature\");insert into DEFN_ERRDESC_FATAL values (1413,\"Engine fuser overtemperature failure\");insert into DEFN_ERRDESC_FATAL values (1414,\"Engine fuser thermistor failure\");insert into DEFN_ERRDESC_FATAL values (1419,\"Engine fuser general failure\");insert into DEFN_ERRDESC_FATAL values (1420,\"Engine LSU H-Sync detection failure\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (1500,\"Engine motor failure\");insert into DEFN_ERRDESC_FATAL values (1501,\"Engine feed motor failure\");insert into DEFN_ERRDESC_FATAL values (1502,\"Engine LSU mirror motor failure\");insert into DEFN_ERRDESC_FATAL values (1503,\"Engine FAN motor failure\");insert into DEFN_ERRDESC_FATAL values (1504,\"Engine option tray motor failure\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (1997,\"Engine unknown failure\");insert into DEFN_ERRDESC_FATAL values (1998,\"Engine emergency stopped\");insert into DEFN_ERRDESC_FATAL values (1999,\"Engine not found\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (2000,\"Scanner failure\");insert into DEFN_ERRDESC_FATAL values (2400,\"Scanner sensor failure\");insert into DEFN_ERRDESC_FATAL values (2401,\"Scanner flatbed unit cannot found home position\");insert into DEFN_ERRDESC_FATAL values (2402,\"Scanner flatbed unit cannot leave home posion\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (2480,\"Scanner calibration failure\");insert into DEFN_ERRDESC_FATAL values (2481,\"Scanner calibration failure #01\");insert into DEFN_ERRDESC_FATAL values (2482,\"Scanner calibration failure #02\");insert into DEFN_ERRDESC_FATAL values (2483,\"Scanner calibration failure #03\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (2490,\"Scanner CIS sensor failure\");insert into DEFN_ERRDESC_FATAL values (2491,\"Scanner CIS #01 sensor failure\");insert into DEFN_ERRDESC_FATAL values (2492,\"Scanner CIS #02 sensor failure\");insert into DEFN_ERRDESC_FATAL values (2493,\"Scanner CIS #03 sensor failure\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (2500,\"Scanner motor failure\");insert into DEFN_ERRDESC_FATAL values (2997,\"Scanner unknown failure\");insert into DEFN_ERRDESC_FATAL values (2998,\"Scanner emergency stopped\");insert into DEFN_ERRDESC_FATAL values (2999,\"Scanner not found\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (3000,\"Finisher failure\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (4000,\"Fax failure\");insert into DEFN_ERRDESC_FATAL values (4997,\"Fax unknown failure\");insert into DEFN_ERRDESC_FATAL values (4998,\"Fax emergency stopped\");insert into DEFN_ERRDESC_FATAL values (4999,\"Fax not found\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (5000,\"File failure\");insert into DEFN_ERRDESC_FATAL values (6000,\"Feeding tray failure\");insert into DEFN_ERRDESC_FATAL values (7000,\"Network card failure\");insert into DEFN_ERRDESC_FATAL values (8000,\"USB failure\");insert into DEFN_ERRDESC_FATAL values (9000,\"Wifi dongle failure\");insert into DEFN_ERRDESC_FATAL values (10000,\"Post script parser failure\");insert into DEFN_ERRDESC_FATAL values (11000,\"PCL parser failure\");insert into DEFN_ERRDESC_FATAL values (12000,\"PDL parser failure\");insert into DEFN_ERRDESC_FATAL values (13000,\"GDI parser failure\");");
		db_retval = db_trySQLiteExec("insert into DEFN_ERRDESC_FATAL values (20000,\"Funtion test client failure\");insert into DEFN_ERRDESC_FATAL values (20101,\"System initial failure (profile)\");insert into DEFN_ERRDESC_FATAL values (20102,\"System initial failure (usb port)\");insert into DEFN_ERRDESC_FATAL values (20103,\"System initial failure (memory)\");insert into DEFN_ERRDESC_FATAL values (20104,\"Data extracting failure\");insert into DEFN_ERRDESC_FATAL values (20201,\"Data download failure\");insert into DEFN_ERRDESC_FATAL values (20202,\"Data integrity failure\");insert into DEFN_ERRDESC_FATAL values (20203,\"Data extracting failure (tar)\");insert into DEFN_ERRDESC_FATAL values (20301,\"Data extracting failure (cp)\");");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_version = 33;
	}
	if(db_version == 33)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"ReadyMenu\" INTEGER DEFAULT 0;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"ReadyDetail\" INTEGER DEFAULT 1;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 34;
	}
	if(db_version == 34)
	{
		db_retval = db_trySQLiteExec("DELETE FROM DEFN_ERRDESC_ERROR;");
		db_retval = db_trySQLiteExec("DELETE FROM DEFN_ERRDESC_FATAL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 35;
	}
	if(db_version == 35)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRINTSET SET SettingPriority=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		//db_version = 36 : only change scheme document.
		db_version = 37;
	}
	if(db_version == 37)
	{
		int k = 0;
		char temp_query[200];
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_ADJUSTSLEEPTIMER\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Rate\" INTEGER);");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		for(k = 0; k < 336; k++)
		{
			memset(temp_query, 0, sizeof(temp_query));
			snprintf(temp_query, sizeof(temp_query), "INSERT OR IGNORE INTO RECD_ADJUSTSLEEPTIMER VALUES (%d,0);", k+1);
			db_retval = db_trySQLiteExec(temp_query);
			if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		}

		db_version = 38;
	}
	if(db_version == 38)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_TONERCOUNT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Count\" INTEGER NOT NULL,\"CountInPercent\" INTEGER NOT NULL);");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_TONERCOUNT VALUES (1,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"TonerTLI\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"TonerSerialNo\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"T2Version\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"T3Version\" TEXT;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET TonerTLI=\"NOTLI\"");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET TonerSerialNo=\"NOSERIAL\"");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET T2Version=\"0.0.0\"");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET T3Version=\"0.0.0\"");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 39;
	}
	if(db_version == 39)
	{
		int k = 0;
		char temp_query[200];
		db_retval = db_trySQLiteExec("DROP TABLE \"RECD_ADJUSTSLEEPTIMER\";");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_ADJUSTSLEEPTIMER\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"DateHour\" INTEGER, \"Rate\" INTEGER);");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		for(k = 0; k < 336; k++)
		{
			memset(temp_query, 0, sizeof(temp_query));
			snprintf(temp_query, sizeof(temp_query), "INSERT OR IGNORE INTO RECD_ADJUSTSLEEPTIMER VALUES (%d,%d,0);", k+1, k);
			db_retval = db_trySQLiteExec(temp_query);
			if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		}

		db_version = 40;
	}
	if(db_version == 40)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"BusySleeptime\" INTEGER DEFAULT 60 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"IdleSleeptime\" INTEGER DEFAULT 1 NOT NULL;");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 41;
	}
	if(db_version == 41)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=-1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 42;
	}
	if(db_version == 42)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE INST_MACHINE RENAME TO INST_MACHINE_backup;");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_MACHINE\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"MachineTLI\" TEXT,\"MachineSerialNo\" TEXT,\"DBVERSION\" INTEGER NOT NULL,\"DrumTLI\" TEXT,\"DrumSerialNo\" TEXT, \"Location\" TEXT, \"TonerTLI\" TEXT, \"TonerSerialNo\" TEXT, \"T2Version\" INTEGER NOT NULL, \"T3Version\" INTEGER NOT NULL);");
		db_retval = db_trySQLiteExec("INSERT INTO INST_MACHINE VALUES (1, \"NOTLI\", \"NOSERIAL\", 42, \"NOTLI\", \"NOSERIAL\", \"NONE\", \"NOTLI\", \"NOSERIAL\", 0, 0);");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET MachineTLI=(SELECT MachineTLI FROM INST_MACHINE_backup WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET MachineSerialNo=(SELECT MachineSerialNo FROM INST_MACHINE_backup WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET DrumTLI=(SELECT DrumTLI FROM INST_MACHINE_backup WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET DrumSerialNo=(SELECT DrumSerialNo FROM INST_MACHINE_backup WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET Location=(SELECT Location FROM INST_MACHINE_backup WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET TonerTLI=(SELECT TonerTLI FROM INST_MACHINE_backup WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET TonerSerialNo=(SELECT TonerSerialNo FROM INST_MACHINE_backup WHERE NO=1) WHERE NO=1");
		db_retval = db_trySQLiteExec("DROP TABLE INST_MACHINE_backup");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 43;
	}
	if(db_version == 43)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_UAPSTATUS SET AuthType=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 44;
	}
	if(db_version == 44)
	{
		db_retval = db_trySQLiteExec("DROP TABLE \"DEFN_ERRDESC_ERROR\";");
		db_retval = db_trySQLiteExec("DROP TABLE \"DEFN_ERRDESC_FATAL\";");
		db_retval = db_trySQLiteExec("DROP TABLE \"LOG_PRNT\";");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 45;
	}
	if(db_version == 45)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_TONERHISTROY\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TonerTLI\" TEXT,\"TonerSerialNo\" TEXT)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_TONERSTATS\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Toner3K\" INTEGER NOT NULL,\"Toner6K\" INTEGER NOT NULL,\"Toner9K\" INTEGER NOT NULL,\"Toner13K\" INTEGER NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_TONERSTATS VALUES (1,0,0,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 46;
	}
	if(db_version == 46)
	{
		db_retval = db_trySQLiteExec("DROP TABLE \"RECD_TONERHISTROY\"");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_TONERHISTORY\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TonerTLI\" TEXT,\"TonerSerialNo\" TEXT UNIQUE)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 47;
	}
	if(db_version == 47)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"DefaultMailServer\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"DefaultMailID\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"DefaultMailPW\" TEXT;");
		db_retval = db_trySQLiteExec("UPDATE \"CONF_SYSTEM\" SET DefaultMailServer='localhost';");
		db_retval = db_trySQLiteExec("UPDATE \"CONF_SYSTEM\" SET DefaultMailID='anonymous';");
		db_retval = db_trySQLiteExec("UPDATE \"CONF_SYSTEM\" SET DefaultMailPW='anonymous';");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"DefaultMailPort\" INTEGER DEFAULT 25 NOT NULL;");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_MAILALERTTYPE\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"AlertEnable\" INTEGER NOT NULL,\"AlertPaperEmpty\" INTEGER NOT NULL,\"AlertTonerEnd\" INTEGER NOT NULL,\"AlertOutputFull\" INTEGER NOT NULL,\"AlertPaperJam\" INTEGER NOT NULL,\"AlertIntervention\" INTEGER NOT NULL,\"AlertDrumChange\" INTEGER NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_MAILALERTTYPE VALUES (1,0,0,1,0,0,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_MAILALERTTYPE VALUES (2,0,0,1,0,0,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_MAILALERTTYPE VALUES (3,0,0,1,0,0,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_MAILALERTTYPE VALUES (4,0,0,1,0,0,1,1)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_ADMINMAILADDR\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"EmailAddr\" TEXT)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 48;
	}
	if(db_version == 48)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET OriginalSize=9");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET PrintSrcTray=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Scale=100");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Collation=0");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Nup=0");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET PRContents=2");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Brightness=5");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Contrast=3");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET BgRemove=5");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET Color=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET Resolution=5");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET OriginalSize=9");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET FileFormat=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET Contrast=3");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET Brightness=5");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET DefaultDest=0");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET TxContrast=4");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET TxRetryNum=2");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET TxRetryInterval=2");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET RxRingNum=3");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET TxOriginalSize=9");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_version = 49;
	}
	if(db_version == 49)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_PORTFILTER\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"FTP\" INTEGER NOT NULL,\"SSH\" INTEGER NOT NULL,\"TELNET\" INTEGER NOT NULL,\"SMTP\" INTEGER NOT NULL,\"HTTP\" INTEGER NOT NULL,\"IPP\" INTEGER NOT NULL,\"RAW\" INTEGER NOT NULL,\"ALLPORTS\" INTEGER NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PORTFILTER VALUES (1,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PORTFILTER VALUES (2,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PORTFILTER VALUES (3,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PORTFILTER VALUES (4,0,0,0,0,0,0,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 50;
	}
	if(db_version == 50)
	{
		db_retval = db_trySQLiteExec("DROP TABLE \"CONF_PORTFILTER\";");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_PORTFILTER\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TCP_FTP\" INTEGER NOT NULL,\"TCP_SSH\" INTEGER NOT NULL,\"TCP_TELNET\" INTEGER NOT NULL,\"UDP_SNMP\" INTEGER NOT NULL,\"TCP_HTTP\" INTEGER NOT NULL,\"TCP_IPP\" INTEGER NOT NULL,\"TCP_RAW\" INTEGER NOT NULL,\"TCP_ALL\" INTEGER NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PORTFILTER VALUES (1,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PORTFILTER VALUES (2,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PORTFILTER VALUES (3,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_PORTFILTER VALUES (4,0,0,0,0,0,0,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 51;
	}
	if(db_version == 51)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Brightness=0");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Contrast=0");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET BgRemove=0");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET Contrast=0");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET Brightness=0");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_version = 52;
	}
	if(db_version == 52)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"AutoResetTime\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"StatusHideTime\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 53;
	}
	if(db_version == 53)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"WatchdogDisable\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 54;
	}
	if(db_version == 54)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Scale=6");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_version = 55;
	}
	if(db_version == 55)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"AirprintSerialNo\" TEXT;");
		db_retval = db_trySQLiteExec("UPDATE INST_MACHINE SET AirprintSerialNo=\"NOSERIAL\";");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 56;
	}
	if(db_version == 56)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"DuplexScan\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Quality\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 57;
	}
	if(db_version == 57)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"CEDDetectionTime\" INTEGER DEFAULT 38 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"T1Timeout\" INTEGER DEFAULT 38 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"T5Timeout\" INTEGER DEFAULT 60 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"TCRErrorRate\" INTEGER DEFAULT 10 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"NoOfTriesOnFallback\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"ReceiveMaxGain\" INTEGER DEFAULT 6 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"DelayForPauseKey\" INTEGER DEFAULT 2 NOT NULL;");
		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 58;
	}
	if(db_version == 58)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE CONF_FAXSP RENAME TO CONF_FAXSP_backup;");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_FAXSP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TxStartSpeed\" INTEGER NOT NULL,\"RxStartSpeed\" INTEGER NOT NULL,\"TxEncoding\" INTEGER NOT NULL,\"SelectCountry\" INTEGER NOT NULL,\"DialTimeout\" INTEGER NOT NULL,\"TransmitLevel\" INTEGER NOT NULL,\"ReceiveLevel\" INTEGER NOT NULL,\"DTMFLowLevel\" INTEGER NOT NULL,\"DTMFHighLevel\" INTEGER NOT NULL,\"DialtoneThreshold\" INTEGER NOT NULL,\"ProgressThreshold\" INTEGER NOT NULL,\"PulseMakeTime\" INTEGER NOT NULL,\"PulseBreakTime\" INTEGER NOT NULL,\"PulseDialType\" INTEGER NOT NULL,\"InterdigitDelay\" INTEGER NOT NULL,\"DCCharacteristic\" INTEGER NOT NULL,\"Impedance\" INTEGER NOT NULL,\"BusytoneCycle\" INTEGER NOT NULL,\"BusytoneMinOnTime\" INTEGER NOT NULL,\"BusytoneMaxOnTime\" INTEGER NOT NULL,\"BusytoneMinOffTime\" INTEGER NOT NULL,\"BusytoneMaxOffTime\" INTEGER NOT NULL,\"CongestToneCycle\" INTEGER NOT NULL,\"CongestToneMinOnTime\" INTEGER NOT NULL,\"CongestToneMaxOnTime\" INTEGER NOT NULL,\"CongestToneMinOffTime\" INTEGER NOT NULL,\"CongestToneMaxOffTime\" INTEGER NOT NULL,\"ExtensionSupport\" INTEGER NOT NULL,\"PulseFallTime\" INTEGER NOT NULL,\"ExtensionVoltageThreshold\" INTEGER NOT NULL,\"JbigAutoMMR\" INTEGER NOT NULL,\"MinDetectableTipRingVolt\" INTEGER NOT NULL,\"DialToneDetection\" INTEGER NOT NULL,\"CEDDetectionTime\" INTEGER NOT NULL,\"T1Timeout\" INTEGER NOT NULL,\"T5Timeout\" INTEGER NOT NULL,\"TCFErrorRate\" INTEGER NOT NULL,\"NoOfTriesOnFallback\" INTEGER NOT NULL,\"ReceiveMaxGain\" INTEGER NOT NULL,\"DelayForPauseKey\" INTEGER NOT NULL);");
		db_retval = db_trySQLiteExec("INSERT INTO CONF_FAXSP SELECT * FROM CONF_FAXSP_backup;");
		db_retval = db_trySQLiteExec("DROP TABLE CONF_FAXSP_backup");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 59;
	}
	if(db_version == 59)
	{
		db_retval = db_trySQLiteExec("DELETE FROM CONF_FAXSP");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXSP VALUES (1,0,0,3,97,50,10,-11,60,40,-1850,0,32,64,0,850,1,0,3,45,55,45,55,4,24,36,14,26,0,1,0,1,18,0,38,38,60,10,1,6,2)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXSP VALUES (2,0,0,3,97,50,10,-11,60,40,-1850,0,32,64,0,850,1,0,3,45,55,45,55,4,24,36,14,26,0,1,0,1,18,0,38,38,60,10,1,6,2)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXSP VALUES (3,0,0,3,97,50,10,-11,60,40,-1850,0,32,64,0,850,1,0,3,45,55,45,55,4,24,36,14,26,0,1,0,1,18,0,38,38,60,10,1,6,2)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXSP VALUES (4,0,0,3,97,50,10,-11,60,40,-1850,0,32,64,0,850,1,0,3,45,55,45,55,4,24,36,14,26,0,1,0,1,18,0,38,38,60,10,1,6,2)");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 60;
	}
	if(db_version == 60)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_USERADDR\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Name\" TEXT,\"RegNo\" INTEGER NOT NULL,\"SpeedDial\" INTEGER,\"GroupNum\" INTEGER,\"UserCode\" TEXT,\"Password\" TEXT,\"AvailableFunc\" INTEGER,\"FaxNum\" TEXT,\"Email\" TEXT,\"SenderSetting\" INTEGER,\"ProtectionCode\" TEXT,\"SMTPSpecify\" INTEGER,\"SMTPServer\" TEXT,\"SMTPPort\" INTEGER,\"SMTPAuth\" INTEGER,\"SMTPID\" TEXT,\"SMTPPWD\" TEXT,\"NFPath\" TEXT,\"NFID\" TEXT,\"NFPWD\" TEXT,\"FTPServer\" TEXT,\"FTPPath\" TEXT,\"FTPPort\" INTEGER,\"FTPID\" TEXT,\"FTPPWD\" TEXT,\"WebDAVServer\" TEXT,\"WebDAVPort\" INTEGER,\"WebDAVID\" TEXT,\"WebDAVPWD\" TEXT);");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_GROUPADDR\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Name\" TEXT,\"RegNo\" INTEGER NOT NULL);");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 61;
	}
	if(db_version == 61)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"IPFilterOn\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"AuthCardTimeout\" INTEGER DEFAULT 60 NOT NULL;");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_IPADDR\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"IPAddr\" TEXT);");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_SERVERIPADDR\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"ServerIPAddr\" TEXT);");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 62;
	}
	if(db_version == 62)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"ScanDuplex\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 63;
	}
	if(db_version == 63)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"PageType\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 64;
	}
	if(db_version == 64)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET StatusHideTime=5");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 65;
	}
	if(db_version == 65)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFDuplexLeftMargin\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFDuplexRightMargin\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFDuplexTopMargin\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFDuplexBottomMargin\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 66;
	}
	if(db_version == 66)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=0");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 67;
	}
	if(db_version == 67)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=-1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 68;
	}
	if(db_version == 68)
	{
		if(IS_PINETREES)
		{
			db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=0");
			if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		}
		db_version = 69;
	}
	if(db_version == 69)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET AutoResetTime=10");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET TxEncoding=0");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"TimeoutFlatbed\" INTEGER DEFAULT 30 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 70;
	}
	if(db_version == 70)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET AutoResetTime=60");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 71;
	}
	if(db_version == 71)
	{
		if(IS_PINETREE)
		{
			db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=0");
			if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		}
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET FaxInputTray=8");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET AutoResetTime=60");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 72;
	}
	if(db_version == 72)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"AutoDarkness\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"ScaleCustom\" INTEGER DEFAULT 100 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"AutoDarkness\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"BgRemove\" INTEGER DEFAULT 5 NOT NULL;");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Brightness=5");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET BgRemove=5");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET Brightness=5");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET TxContrast=5");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 73;
	}
	if(db_version == 73)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"TxAlarm\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"RxAlarm\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 74;
	}
	if(db_version == 74)
	{
		int sys_return = 0;
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"CopyPrint\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"FaxPrint\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"Scan\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"FaxSend\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_copyprint INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_faxprint INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_scan INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_faxsend INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);

		db_version = 75;
	}
	if(db_version == 75)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Scale=0");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 76;
	}
	if(db_version == 76)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET Linguistic=-999");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 77;
	}
	if(db_version == 77)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_USERADDR\" ADD COLUMN \"NFServer\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_USERADDR\" ADD COLUMN \"NFPort\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_USERADDR\" ADD COLUMN \"WebDAVPath\" TEXT;");
		if(IS_PINETREE || IS_PINETREES)
		{
			db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=-1");
		}
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET BeepLevel=6");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"TimeoutFlatbed\" INTEGER DEFAULT 30 NOT NULL;");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Scale=100");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 78;
	}
	if(db_version == 78)
	{
		int sys_return = 0;
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"FBScan\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"A3Scan\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"ADFScan\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"ADFDuplexScan\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_fbscan INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_a3scan INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_adfscan INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_adfduplex INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);

		db_version = 79;
	}
	if(db_version == 79)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"SaveFaxFile\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"FileFormat\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"PageType\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 80;
	}
	if(db_version == 80)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_4=-999");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 81;
	}
	if(db_version == 81)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXIMAGEBACKUP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"JobID\" INTEGER,\"JobType\" INTEGER,\"DialNo\" TEXT,\"TimeToTx\" INTEGER);");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 82;
	}
	if(db_version == 82)
	{
		db_retval = db_trySQLiteExec("DROP TABLE RECD_USERADDR");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_USERADDR\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Name\" TEXT,\"RegNo\" INTEGER NOT NULL,\"SpeedDial\" TEXT,\"GroupNum\" INTEGER,\"UserCode\" TEXT,\"Password\" TEXT,\"AvailableFunc\" INTEGER,\"FaxNum\" TEXT,\"Email\" TEXT,\"SenderSetting\" INTEGER,\"ProtectionCode\" TEXT,\"SMTPSpecify\" INTEGER,\"SMTPServer\" TEXT,\"SMTPPort\" INTEGER,\"SMTPAuth\" INTEGER,\"SMTPID\" TEXT,\"SMTPPWD\" TEXT,\"NFServer\" TEXT,\"NFPort\" TEXT,\"NFPath\" TEXT,\"NFID\" TEXT,\"NFPWD\" TEXT,\"FTPServer\" TEXT,\"FTPPort\" INTEGER,\"FTPPath\" TEXT,\"FTPID\" TEXT,\"FTPPWD\" TEXT,\"WebDAVServer\" TEXT,\"WebDAVPort\" INTEGER,\"WebDAVPath\" TEXT,\"WebDAVID\" TEXT,\"WebDAVPWD\" TEXT);");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 83;
	}
	if(db_version == 83)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXREDIALNUMBER\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"RemoteNum\" TEXT);");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 84;
	}
	if(db_version == 84)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET ReadyMenu=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 85;
	}
	if(db_version == 85)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"SendDestination\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_FAXSOLUTION\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"EmailID\" TEXT,\"EmailPW\" TEXT,\"SenderEmailAddress\" TEXT,\"EmailPort\" INTEGER DEFAULT 0,\"ReceiverEmailAddress\" TEXT,\"EmailSubject\" TEXT,\"EmailText\" TEXT,\"FTPID\" TEXT,\"FTPPW\" TEXT,\"FTPAddress\" TEXT,\"FTPPath\" TEXT,\"FTPPort\" INTEGER DEFAULT 0,\"SMBID\" TEXT,\"SMBPW\" TEXT,\"SMBAddress\" TEXT,\"SMBPath\" TEXT,\"SMBPort\" INTEGER DEFAULT 0,\"WebDAVID\" TEXT,\"WebDAVPW\" TEXT,\"WebDAVAddress\" TEXT,\"WebDAVPath\" TEXT,\"WebDAVPort\" INTEGER DEFAULT 0);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_FAXSOLUTION VALUES (1,\"\",\"\",\"\",0,\"\",\"\",\"\",\"\",\"\",\"\",\"\",0,\"\",\"\",\"\",\"\",0,\"\",\"\",\"\",\"\",0)");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 86;
	}
	if(db_version == 86)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_FAXSOLUTION\" ADD COLUMN \"EmailServerAddress\" TEXT;");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 87;
	}
	if(db_version == 87)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_SOLUTIONUP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"AuthEnable\" INTEGER DEFAULT 0 NOT NULL,\"AdminID\" TEXT,\"AdminPW\" TEXT);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONUP VALUES (1,0,\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONUP VALUES (2,0,\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONUP VALUES (3,0,\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONUP VALUES (4,0,\"\",\"\")");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_SOLUTIONSP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"AuthType\" INTEGER DEFAULT 0 NOT NULL,\"CardReader\" INTEGER DEFAULT 0 NOT NULL,\"USBHostScan\" INTEGER DEFAULT 0 NOT NULL,\"USBFlashPrint\" INTEGER DEFAULT 0 NOT NULL,\"PrintAuthVerify\" INTEGER DEFAULT 0 NOT NULL,\"EmailMe\" INTEGER DEFAULT 0 NOT NULL);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONSP VALUES (1,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONSP VALUES (2,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONSP VALUES (3,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONSP VALUES (4,0,0,0,0,0,0)");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 88;
	}
	if(db_version == 88)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"AutoResetTime\" INTEGER DEFAULT 60 NOT NULL;");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 89;
	}
	if(db_version == 89)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET DialTimeout=40");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 90;
	}
	if(db_version == 90)
	{
		db_retval = db_trySQLiteExec("DROP TABLE CONF_SOLUTIONSP");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_SOLUTIONSP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"AuthType\" INTEGER DEFAULT 0 NOT NULL,\"CardReader\" INTEGER DEFAULT 0 NOT NULL,\"USBHostScanDisable\" INTEGER DEFAULT 0 NOT NULL,\"USBFlashPrintDisable\" INTEGER DEFAULT 0 NOT NULL,\"PrintAuthVerify\" INTEGER DEFAULT 0 NOT NULL,\"EmailMe\" INTEGER DEFAULT 0 NOT NULL,\"AirprintDisable\" INTEGER DEFAULT 0 NOT NULL);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONSP VALUES (1,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONSP VALUES (2,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONSP VALUES (3,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SOLUTIONSP VALUES (4,0,0,0,0,0,0,0)");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 91;
	}
	if(db_version == 91)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_SCANSP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"JAMDetectionMode\" INTEGER DEFAULT 0 NOT NULL);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSP VALUES (1,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSP VALUES (2,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSP VALUES (3,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SCANSP VALUES (4,0)");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"RollerDetection\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET RxReductionSel=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 92;
	}
	if(db_version == 92)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET PrintMode=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 93;
	}
	if(db_version == 93)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SOLUTIONUP SET AdminID=\"admin\"");
		db_retval = db_trySQLiteExec("UPDATE CONF_SOLUTIONUP SET AdminPW=\"admin\"");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_SERVERIPADDR\" ADD COLUMN \"ServerIPAddr2\" TEXT;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 94;
	}
	if(db_version == 94)
	{
		db_retval = db_trySQLiteExec("DROP TABLE RECD_SERVERIPADDR");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_SERVERIPADDR\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"ServerIPAddr\" TEXT,\"ServerIPAddr2\" TEXT);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_SERVERIPADDR VALUES (1,\"Please input primary server address\",\"Please input secondary server address\")");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET PageType=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET PrintSrcTray=8");
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Collation=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 95;
	}
	if(db_version == 95)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSP\" ADD COLUMN \"CopyEraseMargin\" INTEGER DEFAULT 40 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSP\" ADD COLUMN \"WhiteMargin\" INTEGER DEFAULT 50 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSP\" ADD COLUMN \"BlackMargin\" INTEGER DEFAULT 50 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 96;
	}
	if(db_version == 96)
	{
		int value = 0;
		value = db_get_var_int("CONF_LSUPWM", "B1_PWM1", &db_retval);
		if(db_retval == OK)
		{
			if(value == 75)
			{
				db_retval = db_trySQLiteExec("UPDATE CONF_LSUPWM SET B1_PWM1=-999");
			}
			else
			{
				db_retval = db_trySQLiteExec("UPDATE CONF_LSUPWM SET B1_PWM1=-999 WHERE NO=1");
				db_retval = db_trySQLiteExec("UPDATE CONF_LSUPWM SET B1_PWM1=-999 WHERE NO=2");
				db_retval = db_trySQLiteExec("UPDATE CONF_LSUPWM SET B1_PWM1=-999 WHERE NO=3");
			}
		}
		else
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI LSUPWM DB Read Fail\n");
		}
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSP SET CopyEraseMargin=16");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 97;
	}
	if(db_version == 97)
	{
		// The DBResetter is only modified.
		db_version = 98;
	}
	if(db_version == 98)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_SYSTEMSP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"LCDWakeup\" INTEGER DEFAULT 0 NOT NULL);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SYSTEMSP VALUES (1,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SYSTEMSP VALUES (2,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SYSTEMSP VALUES (3,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_SYSTEMSP VALUES (4,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 99;
	}
	if(db_version == 99)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXSPAM\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"FaxNumber\" TEXT);");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 100;
	}
	if(db_version == 100)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"SpamFax\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 101;
	}
	if(db_version == 101)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"TimeZone\" TEXT DEFAULT \"UTC-09:00\" NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 102;
	}
	if(db_version == 102)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"MATCH_A4LT\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 103;
	}
	if(db_version == 103)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET TimeZone=\"KST-09:00\"");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_TIMEZONE\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"TimeZone\" TEXT,\"ZoneInfo\" TEXT);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (1,\"EST+05:00EDT\", \"EST5EDT\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (2,\"EST+05:00\", \"EST\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (3,\"EET-02:00EEST\", \"Turkey\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (4,\"EET-02:00\", \"Africa/Tripoli\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (5,\"SAST-02:00\", \"Africa/Johannesburg\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (6,\"CET-01:00\", \"Africa/Tunis\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (7,\"MYT-08:00\", \"Asia/Kuala_Lumpur\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (8,\"CST-08:00\", \"Asia/Shanghai\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (9,\"KST-09:00\", \"Asia/Seoul\")");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 104;
	}
	if(db_version == 104)
	{
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (10,\"MSK-04:00\", \"Europe/Moscow\")");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 105;
	}
	if(db_version == 105)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"AllLangEnable\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 106;
	}
	if(db_version == 106)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET OriginalSize=-999");
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSET SET OriginalSize=-999");
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXUP SET TxOriginalSize=-999");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 107;
	}
	if(db_version == 107)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_Margin\" ADD COLUMN \"COPY_LEFT\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DuplexMargin\" ADD COLUMN \"COPY_LEFT\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 108;
	}
	if(db_version == 108)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET MATCH_A4LT=1");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 109;
	}
	if(db_version == 109)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET SelectCountry=-999;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 110;
	}
	if(db_version == 110)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_Margin\" ADD COLUMN \"COPY_TOP\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DuplexMargin\" ADD COLUMN \"COPY_TOP\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 111;
	}
	if(db_version == 111)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"SELECT_A31117\" INTEGER DEFAULT -999 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 112;
	}
	if(db_version == 112)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"RepBCResultReport\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXBROADCASTRESULTREPORT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"JobID\" INTEGER DEFAULT 0 NOT NULL,\"RemoteNum\" TEXT,\"DateTime\" INTEGER DEFAULT 0 NOT NULL,\"Duration\" INTEGER DEFAULT 0 NOT NULL,\"TotalPageNo\" INTEGER DEFAULT 0 NOT NULL,\"SendPageNo\" INTEGER DEFAULT 0 NOT NULL,\"Result\" INTEGER DEFAULT 0 NOT NULL);");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 113;
	}
	if(db_version == 113)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXJOBLOG\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"JobType\" INTEGER DEFAULT 0 NOT NULL,\"UserID\" TEXT,\"DateTime\" TEXT,\"RemoteNo\" TEXT,\"Result\" TEXT,\"Page\" INTEGER DEFAULT 0 NOT NULL,\"DelayTime\" TEXT);");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 114;
	}
	if(db_version == 114)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"ServoAdjust\" INTEGER DEFAULT -999 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 115;
	}
	if(db_version == 115)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Texture_Recycled\" INTEGER DEFAULT 2 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"Weight_Recycled\" INTEGER DEFAULT 2 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 116;
	}
	if(db_version == 116)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_FAXSP SET DialTimeout=0");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 117;
	}
	if(db_version == 117)
	{
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (11, \"GMT+12:00\", \"Etc/GMT+12\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (12, \"GMT+11:00\", \"Etc/GMT+11\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (13, \"HST+10:00\", \"Pacific/Honolulu\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (14, \"AKST+09:00AKDT\", \"America/Juneau\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (15, \"GMT+09:00\", \"Etc/GMT+9\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (16, \"PST+08:00PDT\", \"PST8PDT\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (17, \"GMT+08:00\", \"Etc/GMT+8\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (18, \"PST+08:00PDT\", \"PST8PDT\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (19, \"GMT+08:00\", \"Etc/GMT+8\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (20, \"MST+07:00\", \"US/Arizona\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (21, \"MST+07:00MDT\", \"MST7MDT\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (22, \"MST+07:00\", \"US/Arizona\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (23, \"MST+07:00MDT1\", \"America/Chihuahua\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (24, \"MST+07:00\", \"US/Arizona\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (25, \"CST+06:00CDT1\", \"America/Monterrey\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (26, \"CST+06:00\", \"Etc/GMT+6\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (27, \"CST+06:00\", \"Etc/GMT+6\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (28, \"CST+06:00CDT\", \"CST6CDT\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (29, \"CST+06:00\", \"Etc/GMT+6\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (30, \"CST+06:00\", \"Etc/GMT+6\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (31, \"COT+05:00\", \"America/Bogota\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (32, \"EST+05:00EDT1\", \"America/Indianapolis\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (33, \"GMT+05:00\", \"Etc/GMT+5\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (34, \"PYT+04:00PYST\", \"America/Asuncion\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (35, \"GMT+04:00\", \"Etc/GMT+4\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (36, \"AST+04:00ADT\", \"Canada/Atlantic\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (37, \"GMT+04:00\", \"Etc/GMT+4\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (38, \"AMT4AMST\", \"America/Cuiaba\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (39, \"GMT+04:00\", \"Etc/GMT+4\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (40, \"AMT+04:00\", \"America/Manaus\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (41, \"CLT+04:00CLST\", \"America/Santiago\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (42, \"GMT+04:00\", \"Etc/GMT+4\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (43, \"BRT+03:00BRST\", \"Brazil/East\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (44, \"GMT+03:00\", \"Etc/GMT+3\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (45, \"ART+03:00\", \"America/Buenos_Aires\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (46, \"GFT+03:00\", \"America/Cayenne\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (47, \"WGT+03:00WGST\", \"America/Godthab\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (48, \"GMT+03:00\", \"Etc/GMT+3\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (49, \"UYT+03:00UYST\", \"America/Montevideo\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (50, \"GMT+03:00\", \"Etc/GMT+3\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (51, \"BRT+03:00\", \"America/Bahia\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (52, \"GST+02:00\", \"Atlantic/South_Georgia\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (53, \"GMT+02:00\", \"Etc/GMT+2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (54, \"GMT+02:00\", \"Etc/GMT+2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (55, \"AZOT+01:00AZOST\", \"Atlantic/Azores\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (56, \"GMT+01:00\", \"Etc/GMT+1\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (57, \"CVT+01:00\", \"Atlantic/Cape_Verde\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (58, \"GMT0IST\", \"Europe/Dublin\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (59, \"GMT0\", \"Etc/GMT0\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (60, \"GMT0\", \"Etc/GMT0\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (61, \"WET0WEST\", \"Africa/Casablanca\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (62, \"GMT0\", \"Etc/GMT0\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (63, \"GMT0\", \"Etc/GMT0\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (64, \"CET-01:00CEST\", \"Europe/Belgrade\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (65, \"GMT-01:00\", \"Etc/GMT-1\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (66, \"CET-01:00CEST1\", \"Europe/Budapest\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (67, \"GMT-01:00\", \"Etc/GMT-1\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (68, \"CET-01:00CEST2\", \"Europe/Brussels\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (69, \"GMT-01:00\", \"Etc/GMT-1\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (70, \"CET-01:00CEST3\", \"Europe/Sarajevo\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (71, \"GMT-01:00\", \"Etc/GMT-1\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (72, \"WAT-01:00WAST\", \"Africa/Windhoek\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (73, \"GMT-01:00\", \"Etc/GMT-1\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (74, \"CET-01:00CEST4\", \"Europe/Amsterdam\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (75, \"GMT-01:00\", \"Etc/GMT-1\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (76, \"CET-01:00CEST5\", \"Europe/Rome\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (77, \"UTC+03:00\", \"Etc/GMT-1\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (78, \"EET-02:00EEST\", \"Asia/Damascus\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (79, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (80, \"EET-02:00EEST\", \"EET\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (81, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (82, \"EET-02:00EEST1\", \"Asia/Beirut\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (83, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (84, \"EET-02:00EEST2\", \"Europe/Athens\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (85, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (86, \"EET-02:00EEST3\", \"Asia/Amman\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (87, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (88, \"IST-2IDT\", \"Asia/Jerusalem\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (89, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (90, \"EET-02:00EEST4\", \"Africa/Cairo\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (91, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (92, \"EET-02:00EEST5\", \"Europe/Helsinki\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (93, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (94, \"EET-02:00EEST6\", \"Europe/Sofia\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (95, \"GMT-02:00\", \"Etc/GMT-2\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (96, \"EAT-03:00\", \"Africa/Nairobi\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (97, \"AST-03:00\", \"Asia/Baghdad\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (98, \"FET-03:00\", \"Europe/Kaliningrad\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (99, \"AST-03:00\", \"Asia/Baghdad\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (100, \"IRST+03:30IRDT\", \"Asia/Tehran\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (101, \"AZT-04:00AZST\", \"Asia/Baku\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (102, \"Pacific/Apia\", \"Etc/GMT-4\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (103, \"GST-04:00\", \"Asia/Muscat\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (104, \"GET-04:00\", \"Asia/Tbilisi\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (105, \"AMT-04:00\", \"Asia/Yerevan\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (106, \"MUT-04:00\", \"Indian/Mauritius\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (107, \"AFT-4:30\", \"Asia/Kabul\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (108, \"PKT-05:00\", \"Asia/Karachi\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (109, \"UZT-05:00\", \"Asia/Tashkent\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (110, \"IST-05:30\", \"Asia/Kolkata\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (111, \"BDT-06:00\", \"Asia/Dacca\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (112, \"YEKT-06:00\", \"Asia/Yekaterinburg\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (113, \"NOVT-07:00\", \"Asia/Novosibirsk\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (114, \"ICT-07:00\", \"Asia/Bangkok\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (115, \"CST-08:00\", \"Asia/Taipei\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (116, \"WST-08:00\", \"Australia/Perth\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (117, \"KRAT-08:00\", \"Asia/Krasnoyarsk\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (118, \"ULAT-08:00\", \"Asia/Ulaanbaatar\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (119, \"JST-09:00\", \"Asia/Tokyo\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (120, \"IRKT-09:00\", \"Asia/Irkutsk\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (121, \"CST-09:30\", \"Australia/Darwin\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (122, \"CST-9:30ACDT\", \"Australia/Adelaide\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (123, \"CST-09:30\", \"Australia/Darwin\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (124, \"EST-10:00\", \"Australia/Brisbane\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (125, \"AEST-10:00AEDT\", \"Australia/Canberra\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (126, \"GMT-10:00\", \"Etc/GMT-10\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (127, \"ChST-10:00\", \"Pacific/Guam\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (128, \"AEST-10:00AEDT\", \"Australia/Hobart\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (129, \"GMT-10:00\", \"Etc/GMT-10\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (130, \"YAKT-10:00\", \"Asia/Yakutsk\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (131, \"SBT-11:00\", \"Pacific/Guadalcanal\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (132, \"VLAT-11:00\", \"Asia/Vladivostok\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (133, \"MAGT-12:00\", \"Asia/Magadan\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (134, \"NZST-12:00NZDT\", \"Pacific/Auckland\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (135, \"GMT-12:00\", \"Etc/GMT-12\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (136, \"GMT-12:00\", \"Etc/GMT-12\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (137, \"FJT-12:00FJST\", \"Pacific/Fiji\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (138, \"GMT-12:00\", \"Etc/GMT-12\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (139, \"PETT-12:00PETST\", \"Asia/Kamchatka\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (140, \"GMT-12:00\", \"Etc/GMT-12\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (141, \"MHT-12:00\", \"Pacific/Majuro\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (142, \"TOT-13:00\", \"Pacific/Tongatapu\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (143, \"WST-13:00WSDT\", \"Pacific/Apia\");");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (144, \"GMT-13:00\", \"Etc/GMT-13\");");

		if(db_retval)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Alter DB Fail\n", __LINE__);
			return db_retval;
		}

		db_version = 118;
	}
	if(db_version == 118)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"LegalMerge\" INTEGER DEFAULT -999 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"LineInUseThreshold\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 119;
	}
	if(db_version == 119)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"TimeZoneNo\" INTEGER DEFAULT 9 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 120;
	}
	if(db_version == 120)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"ManualReceive\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 121;
	}
	if(db_version == 121)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEMSP SET ServoAdjust=-999");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 122;
	}
	if(db_version == 122)
	{
		db_retval = db_trySQLiteExec("UPDATE RECD_TIMEZONE SET TimeZone='GMT-04:00' WHERE NO=102");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (145, \"GMT-04:00\", \"Etc/GMT-4\");");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 123;
	}
	if(db_version == 123)
	{
		db_retval = db_trySQLiteExec("UPDATE RECD_TIMEZONE SET TimeZone='AFT-04:30' WHERE NO=107");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 124;
	}
	if(db_version == 124)
	{
		db_retval = db_trySQLiteExec("UPDATE RECD_TIMEZONE SET TimeZone='GMT-03:00' WHERE NO=145");
		db_retval = db_trySQLiteExec("UPDATE RECD_TIMEZONE SET ZoneInfo='Etc/GMT-3' WHERE NO=145");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"RemoteAccessCode\" INTEGER DEFAULT 9 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 125;
	}
	if(db_version == 125)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRINTSET\" ADD COLUMN \"TonerSaveMode\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 126;
	}
	if(db_version == 126)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"TrayConfig_mediaType_0\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"TrayConfig_mediaSize_0\" INTEGER DEFAULT -999 NOT NULL;");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaType_4=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET TrayConfig_mediaSize_4=554");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 127;
	}
	if(db_version == 127)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"Footer\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"MemoryRxMode\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"MemoryRxModeStartTime\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"MemoryRxModeEndTime\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"MemoryRxModePassword\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONSP\" ADD COLUMN \"PersonalUPrint\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"TrayLinkWithMPT\" INTEGER DEFAULT 1 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 128;
	}
	if(db_version == 128)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_IMAGELOG\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"IsEnabled\" INTEGER NOT NULL,\"FileTransfer\" INTEGER NOT NULL,\"Delivery\" INTEGER NOT NULL,\"SendingTime\" INTEGER NOT NULL,\"NumberOfRetry\" INTEGER NOT NULL,\"RetryInterval\" INTEGER NOT NULL,\"Resolution\" INTEGER NOT NULL,\"Print\" INTEGER NOT NULL,\"Copy\" INTEGER NOT NULL,\"Scan\" INTEGER NOT NULL,\"FaxTransfer\" INTEGER NOT NULL,\"FaxReception\" INTEGER NOT NULL)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IMAGELOG VALUES (1,0,1,0,0,3,10,300,1,1,1,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IMAGELOG VALUES (2,0,1,0,0,3,10,300,1,1,1,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IMAGELOG VALUES (3,0,1,0,0,3,10,300,1,1,1,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IMAGELOG VALUES (4,0,1,0,0,3,10,300,1,1,1,1,1)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 129;
	}

	if(db_version == 129)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"SendingTimeTO\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"FTPAddress\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"FTPPath\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"FTPID\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"FTPPW\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"FTPPort\" INTEGER DEFAULT 21 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"WebDAVAddress\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"WebDAVPath\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"WebDAVID\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"WebDAVPW\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"WebDAVPort\" INTEGER DEFAULT 443 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 130;
	}
	if(db_version == 130)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"Protocol\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 131;
	}
	if(db_version == 131)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"SleepMode\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 132;
	}
	if(db_version == 132)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_T2\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_T2\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_T3\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_T3\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_T4\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_T4\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_MPT\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_MPT\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 133;
	}
	if(db_version == 133)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"SendFailSCDisplay\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"SDCardFullSCDisplay\" INTEGER DEFAULT 1 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 134;
	}
	if(db_version == 134)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"FaxEnable\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"A3Separation\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"A3SeparationWithDot\" INTEGER DEFAULT 1 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 135;
	}
	if(db_version == 135)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_FAXCODE\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"BoxType\" INTEGER NOT NULL,\"Subaddress\" TEXT,\"Password\" TEXT,\"IsImageSaved\" INTEGER NOT NULL,\"BoxPWD\" TEXT,\"DialNumList\" TEXT)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXCODE VALUES (1,0,\"\",\"\",0,\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXCODE VALUES (2,0,\"\",\"\",0,\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXCODE VALUES (3,0,\"\",\"\",0,\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FAXCODE VALUES (4,0,\"\",\"\",0,\"\",\"\")");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"ScaleCustom_Y\" INTEGER DEFAULT 100 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"RePopupAlarmTime\" INTEGER DEFAULT 60 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 136;
	}
	if(db_version == 136)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_THICK1\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_THICK2\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_THICK3\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_T2_THICK1\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_T2_THICK2\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_T2_THICK3\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_MPT_THICK1\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_MPT_THICK2\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_MPT_THICK3\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"VERTICALMAGNIFICATION_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"HORIZONMAGNIFICATION_THICK\" INTEGER DEFAULT 0 NOT NULL;");

		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"TOP_THICK1\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"TOP_THICK2\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"TOP_THICK3\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 137;
	}
	if(db_version == 137)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"TOP_MPT_ENVELOPE\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 138;
	}
	if(db_version == 138)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"ClosedAreaRx\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"MemoryRx\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"TXResultRptImage\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"ProtTraceAutoOut\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 139;
	}
	if(db_version == 139)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"FoolscapSize\" INTEGER DEFAULT 1 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 140;
	}
	if(db_version == 140)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXCODE\" ADD COLUMN \"BoxName\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"PolConfSet\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"RepBroadSet\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"RepRxFBoxSet\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"FwdRelayResult\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 141;
	}
	if(db_version == 141)
	{
		db_retval = db_trySQLiteExec("DROP TABLE \"CONF_FAXCODE\";");
		
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FAXCODE\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"BoxType\" INTEGER NOT NULL,\"Subaddress\" TEXT,\"Password\" TEXT,\"IsImageSaved\" INTEGER NOT NULL,\"BoxPWD\" TEXT,\"DialNumList\" TEXT,\"BoxName\" TEXT)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_DUSTREMOVE\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"ENABLE\" INTEGER NOT NULL,\"MODE\" INTEGER NOT NULL,\"LIMIT_TE_CNT\" INTEGER NOT NULL,\"LIMIT_DUST_W\" INTEGER NOT NULL,\"LIMIT_DUST_CNT\" INTEGER NOT NULL,\"OFFSET_TOP\" INTEGER NOT NULL,\"OFFSET_SIDE\" INTEGER NOT NULL,\"TH_TE_EDGE\" INTEGER NOT NULL,\"TH_DUST_EDGE\" INTEGER NOT NULL,\"OPTION1\" INTEGER NOT NULL,\"OPTION2\" INTEGER NOT NULL,\"OPTION3\" INTEGER NOT NULL,\"OPTION4\" INTEGER NOT NULL,\"INFO_JOB1\" INTEGER NOT NULL,\"INFO_JOB2\" INTEGER NOT NULL,\"INFO_JOB3\" INTEGER NOT NULL,\"INFO_JOB4\" INTEGER NOT NULL,\"INFO_JOB5\" INTEGER NOT NULL,\"INFO_CNT1\" INTEGER NOT NULL,\"INFO_CNT2\" INTEGER NOT NULL,\"INFO_CNT3\" INTEGER NOT NULL,\"INFO_CNT4\" INTEGER NOT NULL,\"INFO_CNT5\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_DUSTREMOVE VALUES (1,0,0,500,5,10,2,5,157,320000,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_DUSTREMOVE VALUES (2,0,0,500,5,10,2,5,157,320000,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_DUSTREMOVE VALUES (3,0,0,500,5,10,2,5,157,320000,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_DUSTREMOVE VALUES (4,0,0,500,5,10,2,5,157,320000,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 142;
	}
	if(db_version == 142)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"AutoPowerEnable\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"AutoPowerTime\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 143;
	}
	if(db_version == 143)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET ENABLE=1;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 144;
	}
	if(db_version == 144)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"FBPaperSize\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"APSSensor\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"Animation_Speed\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_USERADDR\" ADD COLUMN \"Photo\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_GROUPADDR\" ADD COLUMN \"Photo\" TEXT;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 145;
	}
	if(db_version == 145)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"TXResultRptImage\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 146;
	}
	if(db_version == 146)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"FCodeEnable\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 147;
	}
	if(db_version == 147)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"CUSTOM1SPTYPE\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"CUSTOM2SPTYPE\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"CUSTOM3SPTYPE\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"CUSTOM4SPTYPE\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"CUSTOM5SPTYPE\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"CUSTOM6SPTYPE\" INTEGER DEFAULT 1 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 148;
	}
	if(db_version == 148)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"FactoryStep\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 149;
	}
	if(db_version == 149)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"HORIZONMAGNIFICATION_1200DPI\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"VERTICALMAGNIFICATION_1200DPI\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 150;
	}
	if(db_version == 150)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_IP_OFFSET\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"COPY_MIXED_GAMMA\" INTEGER NOT NULL,\"COPY_TEXT_GAMMA\" INTEGER NOT NULL,\"COPY_PHOTO_GAMMA\" INTEGER NOT NULL,\"SCAN_MONO_GAMMA\" INTEGER NOT NULL,\"SCAN_COLOR_GAMMA\" INTEGER NOT NULL,\"SCAN_BW_GAMMA\" INTEGER NOT NULL,\"FAX_TEXT_GAMMA\" INTEGER NOT NULL,\"FAX_PHOTO_GAMMA\" INTEGER NOT NULL,\"COPY_MIXED_CLIP_W\" INTEGER NOT NULL,\"COPY_TEXT_CLIP_W\" INTEGER NOT NULL,\"COPY_PHOTO_CLIP_W\" INTEGER NOT NULL,\"SCAN_MONO_CLIP_W\" INTEGER NOT NULL,\"SCAN_COLOR_CLIP_W\" INTEGER NOT NULL,\"SCAN_BW_CLIP_W\" INTEGER NOT NULL,\"FAX_TEXT_CLIP_W\" INTEGER NOT NULL,\"FAX_PHOTO_CLIP_W\" INTEGER NOT NULL,\"COPY_MIXED_CLIP_B\" INTEGER NOT NULL,\"COPY_TEXT_CLIP_B\" INTEGER NOT NULL,\"COPY_PHOTO_CLIP_B\" INTEGER NOT NULL,\"SCAN_MONO_CLIP_B\" INTEGER NOT NULL,\"SCAN_COLOR_CLIP_B\" INTEGER NOT NULL,\"SCAN_BW_CLIP_B\" INTEGER NOT NULL,\"FAX_TEXT_CLIP_B\" INTEGER NOT NULL,\"FAX_PHOTO_CLIP_B\" INTEGER NOT NULL)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IP_OFFSET VALUES (1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IP_OFFSET VALUES (2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IP_OFFSET VALUES (3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IP_OFFSET VALUES (4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 151;
	}
	if(db_version == 151)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET LIMIT_DUST_W=6;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 152;
	}
	if(db_version == 152)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"INITIALMACHINESETUP\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 153;
	}
	if(db_version == 153)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"INITIALMACHINELINGUISTIC\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"INITIALMACHINEPAPERTYPE\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"INITIALMACHINECOUNTRYCODE\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 154;
	}
	if(db_version == 154)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_T2\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_T3\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_T4\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_MPT\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 155;
	}
	if(db_version == 155)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"PaperCutPort\" INTEGER DEFAULT 8888 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 156;
	}
	if(db_version == 156)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_DVHISTORY\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Time_Count\" TEXT);");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 157;
	}
	if(db_version == 157)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_IFAXSERVER\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"USERNAME\" TEXT,\"EMAIL\" TEXT,\"REPLYEMAIL\" TEXT,\"SENDMAILADDRESS\" TEXT,\"SENDMAILPORT\" INTEGER NOT NULL,\"SENDMAILAUTHENTICATION\" INTEGER NOT NULL,\"SENDMAILSECURITY\" INTEGER NOT NULL,\"SENDMAILUSER\" TEXT,\"SENDMAILPASSWORD\" TEXT,\"RECEIVEMAILTYPE\" INTEGER NOT NULL,\"RECEIVEMAILADDRESS\" TEXT,\"RECEIVEMAILPORT\" INTEGER NOT NULL,\"RECEIVEMAILAUTHENTICATION\" INTEGER NOT NULL,\"RECEIVEMAILSECURITY\" INTEGER NOT NULL,\"RECEIVEMAILUSER\" TEXT,\"RECEIVEMAILPASSWORD\" TEXT,\"CHECKINTERVAL\" INTEGER NOT NULL,\"USERECEIVEAUTH\" INTEGER NOT NULL)");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IFAXSERVER VALUES (1,\"\",\"\",\"\",\"\",25,0,0,\"\",\"\",0,\"\",0,0,0,\"\",\"\",10,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IFAXSERVER VALUES (2,\"\",\"\",\"\",\"\",25,0,0,\"\",\"\",0,\"\",0,0,0,\"\",\"\",10,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IFAXSERVER VALUES (3,\"\",\"\",\"\",\"\",25,0,0,\"\",\"\",0,\"\",0,0,0,\"\",\"\",10,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IFAXSERVER VALUES (4,\"\",\"\",\"\",\"\",25,0,0,\"\",\"\",0,\"\",0,0,0,\"\",\"\",10,0)");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 158;
	}
	if(db_version == 158)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE CONF_IFAXSERVER RENAME TO CONF_IFAXSERVER_backup;");
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_IFAXSERVER\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"USERNAME\" TEXT,\"EMAIL\" TEXT,\"REPLYEMAIL\" TEXT,\"SENDMAILSERVER\" TEXT,\"SENDMAILPORT\" INTEGER NOT NULL,\"SENDMAILAUTHENTICATION\" INTEGER NOT NULL,\"SENDMAILSECURITY\" INTEGER NOT NULL,\"SENDMAILUSER\" TEXT,\"SENDMAILPASSWORD\" TEXT,\"USERECEIVEAUTH\" INTEGER NOT NULL,\"RECEIVEMAILTYPE\" INTEGER NOT NULL,\"RECEIVEMAILSERVER\" TEXT,\"RECEIVEMAILPORT\" INTEGER NOT NULL,\"RECEIVEMAILAUTHENTICATION\" INTEGER NOT NULL,\"RECEIVEMAILSECURITY\" INTEGER NOT NULL,\"RECEIVEMAILUSER\" TEXT,\"RECEIVEMAILPASSWORD\" TEXT,\"CHECKINTERVAL\" INTEGER NOT NULL);");

		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IFAXSERVER VALUES (1,\"\",\"\",\"\",\"\",25,0,0,\"\",\"\",0,0,\"\",0,0,0,\"\",\"\",10)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IFAXSERVER VALUES (2,\"\",\"\",\"\",\"\",25,0,0,\"\",\"\",0,0,\"\",0,0,0,\"\",\"\",10)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IFAXSERVER VALUES (3,\"\",\"\",\"\",\"\",25,0,0,\"\",\"\",0,0,\"\",0,0,0,\"\",\"\",10)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_IFAXSERVER VALUES (4,\"\",\"\",\"\",\"\",25,0,0,\"\",\"\",0,0,\"\",0,0,0,\"\",\"\",10)");
		
		db_retval = db_trySQLiteExec("DROP TABLE CONF_IFAXSERVER_backup");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 159;
	}
	if(db_version == 159)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"TCRTIMEORDER\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 160;
	}
	if(db_version == 160)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_STOREDFILELIST\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"FileName\" TEXT,\"NumOfCopies\" INTEGER DEFAULT 1 NOT NULL,\"FileboxID\" TEXT,\"FileboxPW\" TEXT,\"StoredMode\" INTEGER DEFAULT 1 NOT NULL,\"StoredTime\" TEXT);");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 161;
	}
	if(db_version == 161)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET LIMIT_TE_CNT=2000;");
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET LIMIT_DUST_W=10;");
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET TH_TE_EDGE=50;");
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET TH_DUST_EDGE=500000;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 162; 
	}
	if(db_version == 162)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET OPTION2=1;");
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET OPTION3=30;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 163; 
	}
	if(db_version == 163)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET LIMIT_DUST_W=8;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUSTREMOVE\" ADD COLUMN \"TH_DUST_CENTER\" INTEGER DEFAULT 30 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUSTREMOVE\" ADD COLUMN \"POP_UP\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUSTREMOVE\" ADD COLUMN \"OPTION5\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUSTREMOVE\" ADD COLUMN \"INFO_JOB6\" INTEGER DEFAULT 0 NOT NULL;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 164; 
	}
	if(db_version == 164)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"PhoneSupport1\" TEXT DEFAULT \"1588-5850(한국)\" NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"PhoneSupport2\" TEXT DEFAULT \"800-708-5858(中國)\" NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"PhoneSales1\" TEXT DEFAULT \"080-900-5850(한국)\" NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"PhoneSales2\" TEXT DEFAULT \"0532-6779-7200(中國)\" NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"WebURLSupport\" TEXT DEFAULT \"http://www.sindoh.com\" NOT NULL;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 165; 
	}
	if(db_version == 165)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"TecModeRemainDay\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"TecModeLastExecTime\" INTEGER DEFAULT 0 NOT NULL;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 166; 
	}
	if(db_version == 166)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFScanPos\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFRegistLoop\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFDuplexRegistLoop\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFDVMAG\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFDHMAG\" INTEGER DEFAULT 0 NOT NULL;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 167; 
	}
	if(db_version == 167)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_TONERHISTORY\" ADD COLUMN \"TonerCount\" INTEGER DEFAULT 0 NOT NULL;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 168; 
	}
	if(db_version == 168)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET TH_DUST_CENTER=50;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 169; 
	}
	if(db_version == 169)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_T2_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_T3_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_T4_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_MPT_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MARGIN\" ADD COLUMN \"LEFT_MPT_ENVELOPE\" INTEGER DEFAULT 0 NOT NULL;");
		
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_T2_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_T3_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_T4_THICK\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_DUPLEXMARGIN\" ADD COLUMN \"LEFT_MPT_THICK\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 170; 
	}
	if(db_version == 170)
	{
#if JUNIPER
		db_retval = db_trySQLiteExec("UPDATE CONF_SCANSP SET CopyEraseMargin=30 where NO=4;"); // juniper copyerasemargin default : 12 (-> 30 only in juniper 151202)
#endif
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 171; 
	}
	if(db_version == 171)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_FAXLOG\" ADD COLUMN \"JobID\" INTEGER;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 172; 
	}
	if(db_version == 172)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"CustomMediaSize_0_X\" INTEGER DEFAULT 1480 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"CustomMediaSize_0_Y\" INTEGER DEFAULT 1480 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"CustomOriginalSize_X\" INTEGER DEFAULT 1480 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"CustomOriginalSize_Y\" INTEGER DEFAULT 1480 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"CustomOriginalSize_X\" INTEGER DEFAULT 1480 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"CustomOriginalSize_Y\" INTEGER DEFAULT 1480 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"CustomOriginalSize_X\" INTEGER DEFAULT 1480 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"CustomOriginalSize_Y\" INTEGER DEFAULT 1480 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 173; 
	}
	if(db_version == 173)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"INITIALMACHINEPAPERTABLE\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"INITIALMACHINECCDMTYPE\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 174; 
	}
	if(db_version == 174)
	{
#if JUNIPER
		db_retval = db_trySQLiteExec("UPDATE CONF_PRINTSET SET PrintDarkness=5;"); // juniper print darkness default : 5 (-> 6 for others)
#endif
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 175; 
	}
	if(db_version == 175)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"MPT_AutoPopup\" INTEGER DEFAULT 0 NOT NULL;");

		int ENV_M_TYPE;
		ENV_M_TYPE = atoi(getenv("M_TYPE"));
		if(ENV_M_TYPE == MTYPE_JUNIPERC)		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET MPT_AutoPopup=0;"); // juniper-c default : 0
		else if(ENV_M_TYPE == MTYPE_JUNIPERS)	db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET MPT_AutoPopup=1;"); // juniper-s default : 1

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 176; 
	}
	if(db_version == 176)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"DefaultMailAddress\" TEXT DEFAULT \"\";");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"DefaultMailSecurity\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"DefaultMailAuth\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 177; 
	}
	if(db_version == 177)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"DefaultMailUserName\" TEXT DEFAULT \"\";");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"SDCardEnable\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 178; 
	}
	if(db_version == 178)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_INTERNETFAX\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"sendEnable\" INTEGER NOT NULL,\"receiveEnable\" INTEGER NOT NULL,\"MDNRequestEnable\" INTEGER NOT NULL,\"MDNResponseEnable\" INTEGER NOT NULL,\"DSNRequestEnable\" INTEGER NOT NULL,\"DSNResponseEnable\" INTEGER NOT NULL,\"MonitorTimeOut\" INTEGER NOT NULL)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_INTERNETFAX VALUES (1,0,0,0,0,0,0,24)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_INTERNETFAX VALUES (2,0,0,0,0,0,0,24)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_INTERNETFAX VALUES (3,0,0,0,0,0,0,24)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_INTERNETFAX VALUES (4,0,0,0,0,0,0,24)");

		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_IFAXHISTORY\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"CommandType\" INTEGER NOT NULL,\"CommandResult\" INTEGER NOT NULL,\"SenderEmail\" TEXT,\"ReceiverEmail\" TEXT,\"MessageID\" TEXT,\"EnvID\" TEXT,\"Subject\" TEXT,\"ConfirmRequestType\" INTEGER NOT NULL,\"DSNResult\" INTEGER NOT NULL,\"MDNResult\" INTEGER NOT NULL,\"SendTime\" INTEGER NOT NULL,\"ReceiveTime\" INTEGER NOT NULL,\"MonitorTimeOut\" INTEGER NOT NULL)");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 179; 
	}
	if(db_version == 179)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_INTERNETFAX\" ADD COLUMN \"lastMessageID\" TEXT DEFAULT \"\";");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 180; 
	}
	if(db_version == 180)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"haveWLan\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IFAXSERVER\" ADD COLUMN \"sendRetryCount\" INTEGER DEFAULT 3 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_INTERNETFAX\" ADD COLUMN \"MonitorEnable\" INTEGER DEFAULT 0 NOT NULL;");
		
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_IFAXHISTORY\" ADD COLUMN \"ResponseCode\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_IFAXHISTORY\" ADD COLUMN \"SenderName\" TEXT DEFAULT \"\";");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_IFAXHISTORY\" ADD COLUMN \"ReceiverName\" TEXT DEFAULT \"\";");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_IFAXHISTORY\" ADD COLUMN \"ReplyEmail\" TEXT DEFAULT \"\";");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 181; 
	}
	if(db_version == 181)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET OPTION2=0;");
		db_retval = db_trySQLiteExec("UPDATE CONF_DUSTREMOVE SET OPTION3=0;");
		
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 182; 
	}
	if(db_version == 182)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRNT\" ADD COLUMN \"MPT_SizeSensor\" INTEGER DEFAULT 0 NOT NULL;");
		//db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET MPT_SizeSensor=0;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 183; 
	}
	if(db_version == 183)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"Static_homeScreen\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 184; 
	}
	if(db_version == 184)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"CHIPGAP_Enable\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"CHIPGAP_First_Gap\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"CHIPGAP_Distance\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 185; 
	}
	if(db_version == 185)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"SDCardPartitionType\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 186; 
	}
	if(db_version == 186)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET Animation_Speed=3;");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET MPT_AutoPopup=1;");
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET MPT_SizeSensor=1;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 187; 
	}
	if(db_version == 187)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFLoop\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSUB\" ADD COLUMN \"ADFDuplexLoop\" INTEGER DEFAULT 0 NOT NULL;");
		
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"InternetFaxEnable\" INTEGER DEFAULT 0 NOT NULL;");

		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_INTERNETFAX\" ADD COLUMN \"printEmailBody\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_INTERNETFAX\" ADD COLUMN \"printMDN\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_INTERNETFAX\" ADD COLUMN \"printDSN\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_INTERNETFAX\" ADD COLUMN \"printSendReport\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_INTERNETFAX\" ADD COLUMN \"printReceiveReport\" INTEGER DEFAULT 1 NOT NULL;");

		db_retval = db_trySQLiteExec("UPDATE CONF_INTERNETFAX SET sendEnable=1;");
		db_retval = db_trySQLiteExec("UPDATE CONF_INTERNETFAX SET receiveEnable=1;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 188; 
	}
	if(db_version == 188)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"MachineInstallDate\" TEXT DEFAULT \"\";");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 189; 
	}
	if(db_version == 189)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_NETWORK_PORT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"rawPrintPortEnable\" INTEGER NOT NULL,\"IPPPrintPortEnable\" INTEGER NOT NULL,\"LPDPrintPortEnable\" INTEGER NOT NULL,\"rawScanPortEnable\" INTEGER NOT NULL,\"httpdPortEnable\" INTEGER NOT NULL)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_NETWORK_PORT VALUES (1,1,1,1,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_NETWORK_PORT VALUES (2,1,1,1,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_NETWORK_PORT VALUES (3,1,1,1,1,1)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_NETWORK_PORT VALUES (4,1,1,1,1,1)");

		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET DefaultMailServer=\"\";");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET DefaultMailID=\"\";");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET DefaultMailPW=\"\";");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 190; 
	}
	if(db_version == 190)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MAILALERTTYPE\" ADD COLUMN \"AlertCounterEnable\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_MAILALERTTYPE\" ADD COLUMN \"AlertCounterSentTime\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 191; 
	}
	if(db_version == 191)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"OriginalOrientation\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"MixedSize\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"Batch\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"EraseBorder_Type\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BorderSame\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BorderLeft\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BorderRight\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BorderTop\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BorderBottom\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BorderCenter\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BookScan\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BookScan_BindingPosition\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BookScan_EraseBorder_Type\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BookScan_EraseBorder_Frame\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SCANSET\" ADD COLUMN \"BookScan_EraseBorder_Center\" INTEGER DEFAULT 0 NOT NULL;");

		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"OriginalOrientation\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"MixedSize\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Batch\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"RotateSort\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Bwreversal\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"ShiftImage_Type\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"ShiftImage_Horizontal\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"ShiftImage_Vertical\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"EraseBorder_Type\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"EraseBorder_Left\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"EraseBorder_Top\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"EraseBorder_Frame\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Stamp_Date\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Stamp_Date_Position\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Stamp_Date_pages\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Stamp_PageNo\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Stamp_PageNo_Position\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Watermark\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"Watermark_Language\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"BookCopy\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"BopyCopy_Duplex\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"BookCopy_BindingPosition\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"BookCopy_EraseBorder_Type\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"BookCopy_EraseBorder_Frame\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"BookCopy_EraseBorder_Center\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_COPYSET\" ADD COLUMN \"MirrorImage\" INTEGER DEFAULT 0 NOT NULL;");

		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"DefaultTxType\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"DefaultDelayTxTime\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 192; 
	}
	if(db_version == 192)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONSP\" ADD COLUMN \"FaxSolution\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_FAXJOBLOG\" ADD COLUMN \"UnixTime\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_STOREDFILELIST\" ADD COLUMN \"UnixTime\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEMSP SET INITIALMACHINECCDMTYPE=1;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		
		db_version = 193; 
	}
	if(db_version == 193)
	{
#if JUNIPER
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET Darkness=5;"); // juniper copy darkness default : 5 (-> 6 for others)
#endif
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"MachineAuthEnable\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"MachinePW\" TEXT DEFAULT \"0000\";");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 194; 
	}
	if(db_version == 194)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_PRNT SET FoolscapSize=0;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 195; 
	}
	if(db_version == 195)
	{
		int sys_return = 0;

		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"A3Print\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"B4Print\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"A4Print\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"B5Print\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"A5Print\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"P8x13Print\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"P8KPrint\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"P16KPrint\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"B6Print\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_MACHINECOUNT\" ADD COLUMN \"ETCPrint\" INTEGER DEFAULT 0 NOT NULL;");
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_a3print INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_b4print INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_a4print INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_b5print INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_a5print INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_8x13print INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_8kprint INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_16kprint INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_b6print INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);
		sys_return = system("sqlite3 /backup/point.db 'ALTER TABLE permaset ADD COLUMN perma_etcprint INTEGER DEFAULT 0 NOT NULL'");
		if(sys_return)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL(SH) Fail\n", __LINE__);

		db_version = 196; 
	}
	if(db_version == 196)
	{
		//SCM code only!! Later this code have to change with NULL
#if 0
		int total_196=0;
		char query_196[QUERY_LENGTH];
		memset(query_196, 0x00, sizeof(query_196));
		total_196 = db_get_var_int("INST_MACHINECOUNT", "TotalPrint", &db_retval);
		snprintf(query_196, QUERY_LENGTH, "UPDATE INST_MACHINECOUNT SET A4Print=%d;", total_196);
		DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(db_version: 196 FOR SCM code): %s\n", query_196);
		db_retval = db_trySQLiteExec(query_196);

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
#endif
		db_version = 197; 
	}
	if(db_version == 197)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_INTERNETFAX\" ADD COLUMN \"deleteAfterDownload\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PRINTSET\" ADD COLUMN \"PDFScale\" INTEGER DEFAULT 2 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONSP\" ADD COLUMN \"PaperCut\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 198; 
	}
	if(db_version == 198)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_MODULEVERSION\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Kernel\" TEXT,\"Controller\" TEXT,\"Engine\" TEXT,\"DFC\" TEXT,\"UICC\" TEXT,\"Bank\" TEXT,\"DatabaseManagementSystem\" TEXT,\"DatabaseScheme\" TEXT,\"SystemManager\" TEXT,\"GUI\" TEXT,\"Browser\" TEXT,\"UIManager\" TEXT,\"EngineModule\" TEXT,\"FaxApp\" TEXT,\"ReportGenerator\" TEXT,\"PDLParser\" TEXT,\"PJLParser\" TEXT,\"GDIParser\" TEXT,\"SindohParser\" TEXT)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_MODULEVERSION VALUES (1,\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_MODULEVERSION VALUES (2,\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_MODULEVERSION VALUES (3,\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\")");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_MODULEVERSION VALUES (4,\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\")");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 199; 
	}
	if(db_version == 199)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_JOBLOG\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"JobID\" INTEGER,\"Status\" INTEGER,\"JobType\" INTEGER,\"DocName\" TEXT,\"JobOwner\" TEXT,\"JobStartTime\" INTEGER,\"JobEndTime\" INTEGER,\"Doc_Page\" INTEGER,\"Repeat_Page\" INTEGER,\"Printed_Page\" INTEGER,\"Destination\" TEXT)");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 200; 
	}
	if(db_version == 200)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONSP\" ADD COLUMN \"SolutionEnable\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 201; 
	}
	if(db_version == 201)
	{
#if JUNIPER
		db_retval = db_trySQLiteExec("UPDATE CONF_COPYSET SET PRContents=0"); // juniper copy PRContents default : 0 (-> 2 for others)
#endif

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 202; 
	}
	if(db_version == 202)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"BlockUSBStick\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 203; 
	}
	if(db_version == 203)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"BlockTonerLow\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"BlockDrumNearEnd\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 204; 
	}
	if(db_version == 204)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"NtpEnable\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"NtpServer\" TEXT DEFAULT \"time.windows.com\";");

		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_IPADDR\" ADD COLUMN \"PortNo\" INTEGER;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"RECD_IPADDR\" ADD COLUMN \"Action\" INTEGER;");

		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"ScanBoxEnable\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"ScanBoxDeleteIntervalDay\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"ScanBoxDeletedLastTime\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"ScanBoxDeleteHour\" INTEGER DEFAULT 0 NOT NULL;");
 
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 205; 
	}
	if(db_version == 205)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"ScanboxPort\" INTEGER DEFAULT 81 NOT NULL;");
 
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 206; 
	}
	if(db_version == 206)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXUP\" ADD COLUMN \"FaxToXSet\" INTEGER DEFAULT 0 NOT NULL;");
 
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 207; 
	}
	if(db_version == 207)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"BlockPaperMismatch\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
	
		db_version = 208; 
	}
	if(db_version == 208)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE CONF_SYSTEMSP RENAME TO CONF_SYSTEMSP_backup;");
		db_retval = db_trySQLiteExec("CREATE TABLE \"CONF_SYSTEMSP\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"LCDWakeup\" INTEGER DEFAULT 0 NOT NULL, \"AllLangEnable\" INTEGER DEFAULT 0 NOT NULL, \"ServoAdjust\" INTEGER DEFAULT -999 NOT NULL, \"LegalMerge\" INTEGER DEFAULT -999 NOT NULL, \"FBPaperSize\" INTEGER DEFAULT 0 NOT NULL, \"APSSensor\" INTEGER DEFAULT 0 NOT NULL, \"CUSTOM1SPTYPE\" INTEGER DEFAULT 1 NOT NULL, \"CUSTOM2SPTYPE\" INTEGER DEFAULT 1 NOT NULL, \"CUSTOM3SPTYPE\" INTEGER DEFAULT 1 NOT NULL, \"CUSTOM4SPTYPE\" INTEGER DEFAULT 1 NOT NULL, \"CUSTOM5SPTYPE\" INTEGER DEFAULT 1 NOT NULL, \"CUSTOM6SPTYPE\" INTEGER DEFAULT 1 NOT NULL, \"FactoryStep\" INTEGER DEFAULT 0 NOT NULL, \"INITIALMACHINESETUP\" INTEGER DEFAULT 0 NOT NULL, \"INITIALMACHINELINGUISTIC\" INTEGER DEFAULT 0 NOT NULL, \"INITIALMACHINEPAPERTYPE\" INTEGER DEFAULT 0 NOT NULL, \"INITIALMACHINECOUNTRYCODE\" INTEGER DEFAULT 0 NOT NULL, \"TecModeRemainDay\" INTEGER DEFAULT 0 NOT NULL, \"TecModeLastExecTime\" INTEGER DEFAULT 0 NOT NULL, \"INITIALMACHINEPAPERTABLE\" INTEGER DEFAULT 0 NOT NULL, \"INITIALMACHINECCDMTYPE\" INTEGER DEFAULT 0 NOT NULL, \"SDCardEnable\" INTEGER DEFAULT 0 NOT NULL, \"haveWLan\" INTEGER DEFAULT 1 NOT NULL, \"SDCardPartitionType\" INTEGER DEFAULT 0 NOT NULL, \"InternetFaxEnable\" INTEGER DEFAULT 0 NOT NULL, \"MachineInstallDate\" TEXT DEFAULT \"\", \"BlockUSBStick\" INTEGER DEFAULT 0 NOT NULL, \"BlockTonerLow\" INTEGER DEFAULT 0 NOT NULL, \"BlockDrumNearEnd\" INTEGER DEFAULT 0 NOT NULL, \"GarbageParserEnable\" INTEGER DEFAULT 0 NOT NULL);");

		db_retval = db_trySQLiteExec("INSERT INTO CONF_SYSTEMSP SELECT * FROM CONF_SYSTEMSP_backup;");
		db_retval = db_trySQLiteExec("DROP TABLE CONF_SYSTEMSP_backup;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 209;
	}
	if(db_version == 209)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"PRNDumpEnable\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 210;
	}
	if(db_version == 210)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"PanoramaFax\" INTEGER DEFAULT 1 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"RingDetect\" INTEGER DEFAULT 1 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 211;
	}
	if(db_version == 211)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEMSP SET GarbageParserEnable=1");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 212;
	}
	if(db_version == 212)
	{
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (146, \"NPT-05:45\", \"Asia/Kathmandu\");");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 213;
	}
	if(db_version == 213)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_FUSERCOUNT\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"Count\" INTEGER NOT NULL,\"ResetAction\" INTEGER NOT NULL,\"MotorNormal\" INTEGER NOT NULL,\"MotorLow\" INTEGER NOT NULL)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_FUSERCOUNT VALUES (1,0,0,0,0)");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 214;
	}
	if(db_version == 214)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_FUSERCOUNT\" ADD COLUMN \"FuserLifePageCount\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 215;
	}
	if(db_version == 215)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"RECD_FUSERHISTORY\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"UsedCount\" INTEGER NOT NULL)");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 216;
	}
	if(db_version == 216)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"IDCopyEnable\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"WebEnable\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 217;
	}
	if(db_version == 217)
	{
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEMSP SET IDCopyEnable=1");
		db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEMSP SET WebEnable=1");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 218;
	}
	if(db_version == 218)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_TONERSTATS\" ADD COLUMN \"TonerDrum6K\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_TONERSTATS\" ADD COLUMN \"TonerDrum13K\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 219;
	}
	if(db_version == 219)
	{
		db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_NETWORK_ETHERNET\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"enable\" INTEGER DEFAULT 1,\"mode\" INTEGER DEFAULT 0);");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_NETWORK_ETHERNET VALUES (1,1,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_NETWORK_ETHERNET VALUES (2,1,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_NETWORK_ETHERNET VALUES (3,1,0)");
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_NETWORK_ETHERNET VALUES (4,1,0)");

		if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 220;
	}
	if(db_version == 220)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FAXSP\" ADD COLUMN \"FaxImageBackup\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 221;
	}
	if(db_version == 221)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONSP\" ADD COLUMN \"CompanyCode\" INTEGER DEFAULT 0 NOT NULL;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"SolutionDefaultValueSet\" INTEGER DEFAULT 0 NOT NULL;");

		if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 222;
	}
	if(db_version == 222)
	{	
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_FAXSOLUTION\" ADD COLUMN \"UserName\" TEXT;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"INST_FAXSOLUTION\" ADD COLUMN \"SendMailSecurity\" INTEGER DEFAULT 0;");
		db_retval = db_trySQLiteExec("UPDATE INST_FAXSOLUTION SET EmailPort=25;");
		db_retval = db_trySQLiteExec("UPDATE INST_FAXSOLUTION SET FTPPort=21;");
		db_retval = db_trySQLiteExec("UPDATE INST_FAXSOLUTION SET SMBPort=445;");
		db_retval = db_trySQLiteExec("UPDATE INST_FAXSOLUTION SET WebDAVPort=80;");

		if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 223;
	}
	if(db_version == 223)
	{
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PORTFILTER\" ADD COLUMN \"TCP_WEBDAV\" INTEGER DEFAULT 0;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PORTFILTER\" ADD COLUMN \"TCP_LPD\" INTEGER DEFAULT 0;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PORTFILTER\" ADD COLUMN \"TCP_SCAN\" INTEGER DEFAULT 0;");
		db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_PORTFILTER\" ADD COLUMN \"TCP_ETC\" INTEGER DEFAULT 0;");

		if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 224;
	}
	if(db_version == 224)
	{
		db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO RECD_TIMEZONE VALUES (147, \"IEST-09:00\", \"Asia/Jayapura\");");

		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);

		db_version = 225;
	}
	if(db_version == 225)
	{
#define GET_MAC_SHELL_COMMAND	"fw_printenv ethaddr_sdh | cut -d'=' -f2"
#define MAC_ADDRESS_LEN			17
		FILE *fp;
		char query[QUERY_LENGTH];
		memset(query, 0x00, sizeof(char)*QUERY_LENGTH);
		char readBuf[MAC_ADDRESS_LEN+1];
		memset(readBuf, 0x00, sizeof(char)*(MAC_ADDRESS_LEN+1));
		if((fp = popen("fw_printenv ethaddr_sdh | cut -d'=' -f2", "r")) != NULL)
		{
			if(fread((void*)readBuf, sizeof(char), MAC_ADDRESS_LEN, fp) != MAC_ADDRESS_LEN)
			{
				perror("fread");
				snprintf(readBuf, MAC_ADDRESS_LEN+1, "%s", "00:00:00:00:00:00");
			}
			pclose(fp);
		}
		else
		{
			perror("popen");
			snprintf(readBuf, MAC_ADDRESS_LEN+1, "%s", "00:00:00:00:00:00");
		}
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): db_version:%d ethaddr_sdh:%s\n", __LINE__, db_version+1, readBuf);

		snprintf(query, QUERY_LENGTH, "ALTER TABLE \"INST_MACHINE\" ADD COLUMN \"MACAddress\" TEXT DEFAULT \"%s\";", readBuf);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): db_version:%d query:%s\n", __LINE__, db_version+1, query);

		db_retval = db_trySQLiteExec(query);
		if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
		db_version = 226;
	}
    if(db_version == 226)
    {
        db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"CONF_FOTA\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"IsEnabled\" INTEGER NOT NULL,\"Day\" INTEGER NOT NULL,\"Hour\" INTEGER NOT NULL,\"Minute\" INTEGER NOT NULL,\"Existence\" INTEGER NOT NULL,\"ServerIP\" TEXT,\"LastState\" INTEGER NOT NULL,\"Content\" TEXT,\"APIVersion\" INTEGER NOT NULL,\"IsAutoUpdate\" INTEGER NOT NULL,\"ErrorDetail\" TEXT,\"UpdateRetryCount\" INTEGER NOT NULL,\"Port\" INTEGER NOT NULL);");
        db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FOTA VALUES (1,0,0,0,0,0,\"0.0.0.0\",0,\"\",1,0,\"\",0,3443)");
        db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FOTA VALUES (2,0,0,0,0,0,\"0.0.0.0\",0,\"\",1,0,\"\",0,3443)");
        db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FOTA VALUES (3,0,0,0,0,0,\"0.0.0.0\",0,\"\",1,0,\"\",0,3443)");
        db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO CONF_FOTA VALUES (4,0,0,0,0,0,\"0.0.0.0\",0,\"\",1,0,\"\",0,3443)");

        if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
        db_version = 227;
    }
    if(db_version == 227)
    {
        db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_FOTA\" ADD COLUMN \"Protocol\" INTEGER DEFAULT 1;");

        if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", __LINE__);
        db_version = 228;
    }
    if(db_version == 228)
    {
        db_retval = db_trySQLiteExec("CREATE TABLE IF NOT EXISTS \"INST_MACHINE_INFO\" (\"NO\" INTEGER PRIMARY KEY NOT NULL,\"vendorName\" TEXT,\"modelName\" TEXT);");
        db_retval = db_trySQLiteExec("INSERT OR IGNORE INTO INST_MACHINE_INFO VALUES (1,\"VENDOR\",\"MODEL\")");

        if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_initialize(ver %d): SQL Fail\n", db_version);

        db_version = 229;
    }
    if(db_version == 229)
    {
        db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"ImageLogDebug\" INTEGER DEFAULT 0 NOT NULL;");
        db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_IMAGELOG\" ADD COLUMN \"ImageLogDump\" INTEGER DEFAULT 0 NOT NULL;");

        if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_initialize(ver %d): SQL Fail\n", db_version);

        db_version = 230;
    }
    if(db_version == 230)
    {
        db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEMSP\" ADD COLUMN \"MotorType\" INTEGER DEFAULT 0 NOT NULL;");

        if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_initialize(ver %d): SQL Fail\n", db_version);

        db_version = 231;
    }
    if(db_version == 231)
    {
        db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SYSTEM\" ADD COLUMN \"Enable24HourNotation\" INTEGER DEFAULT 0 NOT NULL;");

        if(db_retval)   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_initialize(ver %d): SQL Fail\n", db_version);

        db_version = 232;
    }

	db_retval = db_set_var_for_TLI();
	if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): db_set_var_for_TLI Fail\n", __LINE__);
	db_retval = db_set_var_for_solution();
	if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): db_set_var_for_solution Fail\n", __LINE__);
 	db_retval = db_critical_bug_check(db_version);
	if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_initialize(line %d): db_critical_bug_check Fail\n", __LINE__);

	snprintf(query, sizeof(query), "UPDATE INST_MACHINE SET DBVERSION=%d;", db_version);
    db_retval = db_trySQLiteExec(query);
    if(db_retval)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Update DB Fail\n", __LINE__);
    }

	db_retval = db_trySQLiteExec("COMMIT;");

    return db_retval;
}

error_type_t db_trySQLiteExec(char *query)
{
    int32_t db_retval = 0;
    int32_t retval = FAIL;
    char* errMsg;

    while(1)
    {
        db_retval = sqlite3_exec( dbInstance, query, NULL, NULL, &errMsg );
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query Result %d\n", __LINE__, db_retval);

        if (db_retval == SQLITE_OK)
        {
            DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query Success: %s\n", __LINE__, query);
            retval = OK;
            break;
        }
        else if(db_retval == SQLITE_ERROR)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query : %s\n", __LINE__, query);
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, errMsg);
            sqlite3_free(errMsg);
            break;
        }
        else if(db_retval == SQLITE_CONSTRAINT)
		{
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Fail(%d) %s\n", __LINE__, db_retval, query);
            break;
		}
        else
        {
            sqlite3_free(errMsg);
            switch (db_retval % 0x100)
            {
                case SQLITE_BUSY:
				case SQLITE_LOCKED:
                    sqlite3_busy_timeout(dbInstance, 2000); // ms
					break;
				case SQLITE_IOERR:
					if ( db_retval >> 8 != 0 )
                        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Detailed Query Error(%d)\n", __LINE__, db_retval >> 8);
					else
					{
                        DBG_PRINTF(LOG_CRIT|DBG_NVRAM, "NVRAMAPI(%d): Can't execute anymore\n", __LINE__);
						while (1) sleep(1000);
					}
					break;
				default:
					break;
			}
			usleep(5000);
		}
	}
    return retval;
}

/*
 * FUNCTION NAME: DBSetVar 
 *
 * Please see nvram_api.h for function documentation.
 */
error_type_t db_set_var(char* tblName, char* attrName, char* value)
{
    char query[QUERY_LENGTH];
    uint32_t db_retval = 0;

    if ((tblName == NULL) || (attrName == NULL))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

    memset(query, 0, sizeof(query));
	if(!strncmp(tblName, "CONF", 4))
		snprintf(query, QUERY_LENGTH, "UPDATE %s SET %s='%s' WHERE NO=4",tblName, attrName, value);
	else
		snprintf(query, QUERY_LENGTH, "UPDATE %s SET %s='%s' WHERE NO=1",tblName, attrName, value);
    DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);

    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_set_var_int(char* tblName, char* attrName, uint32_t value)
{
    char query[QUERY_LENGTH];
    uint32_t db_retval = 0;

    if ((tblName == NULL) || (attrName == NULL))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

    memset(query, 0, sizeof(query));
	if(!strncmp(tblName, "CONF", 4))
		snprintf(query, QUERY_LENGTH, "UPDATE %s SET %s=%d WHERE NO=4",tblName, attrName, value);
	else
		snprintf(query, QUERY_LENGTH, "UPDATE %s SET %s=%d WHERE NO=1",tblName, attrName, value);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);

    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

uint32_t db_get_data_count(char* table, char* condition, error_type_t* db_retval)
{
    char query[QUERY_LENGTH];
    char* retValue = NULL;
    uint32_t rowCount = 0;

    if(table == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        *db_retval = FAIL;
        return rowCount;
    }

    memset(query, 0, sizeof(query));
    if(condition == NULL)
    {
        snprintf(query, QUERY_LENGTH, "SELECT count(*) FROM %s", table);
    }
    else
    {
        snprintf(query, QUERY_LENGTH, "SELECT count(*) FROM %s WHERE %s", table, condition);
    }
    retValue = db_get_var_query(query, db_retval);
    if(*db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Fail to get the DB data count\n", __LINE__);
        return rowCount;
    }
    rowCount = atoi(retValue);
    MEM_FREE_AND_NULL(retValue);

    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): DB data count: %d\n", __LINE__, rowCount);

    return rowCount;
}

uint32_t db_get_last_recd_no(char* table, error_type_t* db_retval)
{
    char query[QUERY_LENGTH];
    char* retValue = NULL;
    uint32_t lastNo = 0;

    if(table == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        *db_retval = FAIL;
        return lastNo;
    }

    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "SELECT NO FROM %s ORDER BY NO DESC limit 1", table);
    retValue = db_get_var_query(query, db_retval);
    if(*db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Fail to get the DB data count\n", __LINE__);
        return lastNo;
    }
    lastNo = atoi(retValue);
    MEM_FREE_AND_NULL(retValue);

    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): DB Last No: %d\n", __LINE__, lastNo);

    return lastNo;
}

error_type_t db_set_recd_fax_log(uint32_t _dateTime, uint32_t _faxMode, uint32_t _ecm, uint32_t _resolution, char* _remoteNum, uint32_t _duration, uint32_t _pageNum, uint32_t _connectSpeed, uint32_t _result, uint32_t _recName)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
//    uint32_t rowCount = 0;

    if (_remoteNum == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
#if 0 // no used
    rowCount = db_get_data_count("RECD_FAXLOG", NULL, &db_retval);
    if(db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
        return FAIL;
    }
    while(rowCount >= LOG_FAXLOG_ROW_SIZE)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): The oldest log is deleted. Log count is %d\n", __LINE__, rowCount);

        memset(query, 0, sizeof(query));
        snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_FAXLOG WHERE rowid IN (SELECT rowid FROM RECD_FAXLOG ORDER BY rowid ASC limit 1);");
        db_retval = db_trySQLiteExec(query);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "NVRAMAPI(%d): Fail to delete printlog\n", __LINE__);
        }
        rowCount = db_get_data_count("RECD_FAXLOG", NULL, &db_retval);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
            return FAIL;
        }
    }
    DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Log count is %d\n", __LINE__, rowCount);
#endif
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_FAXLOG(DateTime, FaxMode, ECM, Resolution, RemoteNum, Duration, PageNum, ConnectSpeed, Result, RecName) VALUES (%d, %d, %d, %d, '%s', %d, %d, %d, %d, %d);", _dateTime, _faxMode, _ecm, _resolution, _remoteNum, _duration, _pageNum, _connectSpeed, _result, _recName);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

//Temporary used in juniper
error_type_t db_set_recd_fax_log_jobid(uint32_t _dateTime, uint32_t _faxMode, uint32_t _ecm, uint32_t _resolution, char* _remoteNum, uint32_t _duration, uint32_t _pageNum, uint32_t _connectSpeed, uint32_t _result, uint32_t _recName, uint32_t _jobId)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
//    uint32_t rowCount = 0;

    if (_remoteNum == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
#if 0 // no used
    rowCount = db_get_data_count("RECD_FAXLOG", NULL, &db_retval);
    if(db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
        return FAIL;
    }
    while(rowCount >= LOG_FAXLOG_ROW_SIZE)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): The oldest log is deleted. Log count is %d\n", __LINE__, rowCount);

        memset(query, 0, sizeof(query));
        snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_FAXLOG WHERE rowid IN (SELECT rowid FROM RECD_FAXLOG ORDER BY rowid ASC limit 1);");
        db_retval = db_trySQLiteExec(query);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "NVRAMAPI(%d): Fail to delete printlog\n", __LINE__);
        }
        rowCount = db_get_data_count("RECD_FAXLOG", NULL, &db_retval);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
            return FAIL;
        }
    }
    DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Log count is %d\n", __LINE__, rowCount);
#endif
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_FAXLOG(DateTime, FaxMode, ECM, Resolution, RemoteNum, Duration, PageNum, ConnectSpeed, Result, RecName, JobID) VALUES (%d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, %d);", _dateTime, _faxMode, _ecm, _resolution, _remoteNum, _duration, _pageNum, _connectSpeed, _result, _recName, _jobId);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_set_recd_fax_broadcast_log(uint32_t _jobID, char* _remoteNum, uint32_t _dateTime, uint32_t _duration, uint32_t _totalPageNo, uint32_t _sendPageNo, uint32_t _result)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
#if 0
    uint32_t rowCount = 0;
#endif

    if (_remoteNum == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
#if 0
    rowCount = db_get_data_count("RECD_FAXBROADCASTRESULTREPORT", NULL, &db_retval);
    if(db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
        return FAIL;
    }
    while(rowCount >= LOG_FAXBROADCASTLOG_ROW_SIZE)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): The oldest log is deleted. Log count is %d\n", __LINE__, rowCount);

        memset(query, 0, sizeof(query));
        snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_FAXBROADCASTRESULTREPORT WHERE rowid IN (SELECT rowid FROM RECD_FAXBROADCASTRESULTREPORT ORDER BY rowid ASC limit 1);");
        db_retval = db_trySQLiteExec(query);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "NVRAMAPI(%d): Fail to delete printlog\n", __LINE__);
        }
        rowCount = db_get_data_count("RECD_FAXBROADCASTRESULTREPORT", NULL, &db_retval);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
            return FAIL;
        }
    }
    DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Log count is %d\n", __LINE__, rowCount);
#endif
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_FAXBROADCASTRESULTREPORT(JobID, RemoteNum, DateTime, Duration, TotalPageNo, SendPageNo, Result) VALUES (%d, '%s', %d, %d, %d, %d, %d);", _jobID, _remoteNum, _dateTime, _duration, _totalPageNo, _sendPageNo, _result);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_set_recd_fax_job_log(uint32_t _jobType, char* _userID, char* _dateTime, char* _remoteNo, char* _result, uint32_t _page, char* _delayTime)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
    uint32_t rowCount = 0;

    if (_userID == NULL || _dateTime == NULL || _remoteNo == NULL || _result == NULL || _delayTime == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
#if 1
    rowCount = db_get_data_count("RECD_FAXJOBLOG", NULL, &db_retval);
    if(db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
        return FAIL;
    }
    while(rowCount >= LOG_FAXJOBLOG_ROW_SIZE)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): The oldest log is deleted. Log count is %d\n", __LINE__, rowCount);

        memset(query, 0, sizeof(query));
        snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_FAXJOBLOG WHERE rowid IN (SELECT rowid FROM RECD_FAXJOBLOG ORDER BY rowid ASC limit 1);");
        db_retval = db_trySQLiteExec(query);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "NVRAMAPI(%d): Fail to delete printlog\n", __LINE__);
        }
        rowCount = db_get_data_count("RECD_FAXJOBLOG", NULL, &db_retval);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
            return FAIL;
        }
    }
    DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Log count is %d\n", __LINE__, rowCount);
#endif
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_FAXJOBLOG(JobType, UserID, DateTime, RemoteNo, Result, Page, DelayTime) VALUES (%d, '%s', '%s', '%s', '%s', %d, '%s');", _jobType, _userID, _dateTime, _remoteNo, _result, _page, _delayTime);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_set_recd_fax_fcode_box(uint32_t _boxType, char* _subaddress, char* _password, uint32_t _isImageSaved, char* _boxPWD, char* _dialNumList, char* _boxName)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
    uint32_t rowCount = 0;

    if (_subaddress == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
#if 1
    rowCount = db_get_data_count("RECD_FAXCODE", NULL, &db_retval);
    if(db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
        return FAIL;
    }
    while(rowCount >= LOG_FAXJOBLOG_ROW_SIZE)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): The oldest log is deleted. Log count is %d\n", __LINE__, rowCount);

        memset(query, 0, sizeof(query));
        snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_FAXCODE WHERE rowid IN (SELECT rowid FROM RECD_FAXCODE ORDER BY rowid ASC limit 1);");
        db_retval = db_trySQLiteExec(query);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "NVRAMAPI(%d): Fail to delete printlog\n", __LINE__);
        }
        rowCount = db_get_data_count("RECD_FAXCODE", NULL, &db_retval);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
            return FAIL;
        }
    }
    DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Log count is %d\n", __LINE__, rowCount);
#endif
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_FAXCODE(BoxType, Subaddress, Password, IsImageSaved, BoxPWD, DialNumList, BoxName) VALUES (%d, '%s', '%s', %d, '%s, '%s, %s');"
			, _boxType, _subaddress, _password, _isImageSaved, _boxPWD, _dialNumList, _boxName);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_set_recd_fax_sp_log(uint32_t _dateTime, uint32_t _faxMode, uint32_t _ecm, uint32_t _resolution, char* _remoteNum, uint32_t _duration, uint32_t _pageNum, uint32_t _connectSpeed, uint32_t _result, uint32_t _recName)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;

    if (_remoteNum == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_FAXSPLOG(DateTime, FaxMode, ECM, Resolution, RemoteNum, Duration, PageNum, ConnectSpeed, Result, RecName) VALUES (%d, %d, %d, %d, '%s', %d, %d, %d, %d, %d);", _dateTime, _faxMode, _ecm, _resolution, _remoteNum, _duration, _pageNum, _connectSpeed, _result, _recName);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_set_recd_fax_image_backup(uint32_t _jobID, uint32_t _jobType, char* _dialNo, uint32_t _timeToTx)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;

    if (_dialNo == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_FAXIMAGEBACKUP(JobID, JobType, DialNo, TimeToTx) VALUES (%d, %d, '%s', %d);", _jobID, _jobType, _dialNo, _timeToTx);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}



error_type_t db_set_recd_err_log(int _errorType, int _errorDesc, int _subJobType, int _jobID, int _date, int _uptimeSec, int _pageCount, int _porCount, char* _ip, char* _desc)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
	int rowCount = 0;

    if ((_ip == NULL) || (_desc == NULL))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

    rowCount = db_get_data_count("RECD_ERRLOG", NULL, &db_retval);
    if(db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
        return FAIL;
    }
    while(rowCount >= MAX_ERRLOG_ROW)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): The oldest log is deleted. Log count is %d\n", __LINE__, rowCount);

        memset(query, 0, sizeof(query));
        snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_ERRLOG WHERE rowid IN (SELECT rowid FROM RECD_ERRLOG ORDER BY rowid ASC limit 1);");
        db_retval = db_trySQLiteExec(query);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "NVRAMAPI(%d): Fail to delete error log\n", __LINE__);
        }
        rowCount = db_get_data_count("RECD_ERRLOG", NULL, &db_retval);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
            return FAIL;
        }
    }
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Log count is %d\n", __LINE__, rowCount);

    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_ERRLOG(ErrorType, ErrorDesc, SubJobType, JobID, Date, UptimeSec, PageCount, PORCount, IP, Desc) VALUES (%d, %d, %d, %d, %d, %d, %d, %d, '%s', '%s');", _errorType, _errorDesc, _subJobType, _jobID, _date, _uptimeSec, _pageCount, _porCount, _ip, _desc);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_set_recd_toner_history(char* _tonerTLI, char* _tonerSerialNo, int _capacity)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
	int rowCount = 0;

    if ((_tonerTLI == NULL) || (_tonerSerialNo == NULL))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

	db_retval = db_set_var("INST_MACHINE","TonerTLI",_tonerTLI);
	db_retval = db_set_var("INST_MACHINE","TonerSerialNo",_tonerSerialNo);
	if(db_retval == FAIL)
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): INST_MACHINE table update failed.\n", __LINE__);

    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_TONERHISTORY(TonerTLI,TonerSerialNo) VALUES ('%s', '%s');", _tonerTLI, _tonerSerialNo);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

	if(db_retval == OK)
	{
		int toner_capacity = 0;
		char currentTonerTLI[20] = {0, };

		rowCount = db_get_data_count("RECD_TONERHISTORY", NULL, &db_retval);
		if(db_retval == FAIL)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
			return FAIL;
		}
		while(rowCount > MAX_TONERHISTORY_ROW)
		{
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): The oldest log is deleted. Log count is %d\n", __LINE__, rowCount);

			memset(query, 0, sizeof(query));
			snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_TONERHISTORY WHERE rowid IN (SELECT rowid FROM RECD_TONERHISTORY ORDER BY rowid ASC limit 1);");
			db_retval = db_trySQLiteExec(query);
			if(db_retval == FAIL)
			{
				DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "NVRAMAPI(%d): Fail to delete error log\n", __LINE__);
			}
			rowCount = db_get_data_count("RECD_TONERHISTORY", NULL, &db_retval);
			if(db_retval == FAIL)
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
				return FAIL;
			}
		}
		DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Log count is %d\n", __LINE__, rowCount);

		switch(_capacity)
		{
		case 3000:
			snprintf(currentTonerTLI, 20, "Toner3K");
			break;
		case 6000:
			if (bSepCartridge)
				snprintf(currentTonerTLI, 20, "Toner6K");
			else
				snprintf(currentTonerTLI, 20, "TonerDrum6K");
			break;
		case 9000:
			snprintf(currentTonerTLI, 20, "Toner9K");
			break;
		case 13000:
			if (bSepCartridge)
				snprintf(currentTonerTLI, 20, "Toner13K");
			else
				snprintf(currentTonerTLI, 20, "TonerDrum13K");
			break;
		default:
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Unvalid toner capacity\n", __LINE__);
			db_retval = FAIL;
			break;
		}

		if(db_retval == OK)
		{
			toner_capacity = db_get_var_int("INST_TONERSTATS", currentTonerTLI, &db_retval);
			if(db_retval == OK)
			{
				toner_capacity++;
				db_retval = db_set_var_int("INST_TONERSTATS", currentTonerTLI, toner_capacity);
			}
			else
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): update fail!! %s\n", __LINE__, query);
			}
		}
	}

    return db_retval;
}

error_type_t db_set_recd_ssidbook(int _ifSelected, char* _ssid, char* _apMacAddr, int _ifHidden, int _authType, int _groupCipher, int _pairCipher, int _ifSupportPreAuth, int _authSuite, char* _passkey1, char* _passkey2, char* _passkey3, char* _passkey4)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;

    if ((_ssid == NULL) || (_apMacAddr == NULL) || (_passkey1 == NULL) || (_passkey2 == NULL) || (_passkey3 == NULL) || (_passkey4 == NULL))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_SSIDBOOK(ifSelected, SSID, APMacAddr, ifHidden, AuthType, GroupCipher, PairCipher, ifSupportPreAuth, AuthSuite, Passkey1, Passkey2, Passkey3, Passkey4) VALUES (%d, '%s', '%s', %d, %d, %d, %d, %d, %d, '%s', '%s', '%s', '%s');", _ifSelected, _ssid, _apMacAddr, _ifHidden, _authType, _groupCipher, _pairCipher, _ifSupportPreAuth, _authSuite, _passkey1, _passkey2, _passkey3, _passkey4);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_set_recd_uap_maclist(char* _macAddress)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;

    if (_macAddress == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_UAPMACLIST(MACAddress) VALUES ('%s');", _macAddress);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_del_recd_fax_broadcast_log_job_var(uint32_t _jobID)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;

    memset(query, 0, sizeof(query));
	snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_FAXBROADCASTRESULTREPORT WHERE jobID=%d;", _jobID);
	DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
	db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_del_recd_var(char* tblName)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;

    if (tblName == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
    memset(query, 0, sizeof(query));
	if(!strncmp(tblName, "RECD", 4))
	{
		snprintf(query, QUERY_LENGTH, "DELETE FROM %s WHERE rowid IN (SELECT rowid FROM %s ORDER BY rowid ASC limit 1);", tblName, tblName);
		DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
		db_retval = db_trySQLiteExec(query);
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): %s is not RECD table.\n", __LINE__, tblName);
	}

    return db_retval;
}

error_type_t db_del_recd_all(char* tblName)
{
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;

    if (tblName == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
    memset(query, 0, sizeof(query));
	if(!strncmp(tblName, "RECD", 4))
	{
		snprintf(query, QUERY_LENGTH, "DELETE FROM %s;", tblName);
		DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
		db_retval = db_trySQLiteExec(query);
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): %s is not RECD table.\n", __LINE__, tblName);
	}

    return db_retval;
}

error_type_t db_set_printlog(print_log_entry_t* log_entry)
{
    char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
    uint32_t rowCount = 0;

    if (log_entry == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }
    rowCount = db_get_data_count("LOG_PRNT", NULL, &db_retval);
    if(db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
        return FAIL;
    }
    while(rowCount >= LOG_PRNT_ROW_SIZE)
    {
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): The oldest log is deleted. Log count is %d\n", __LINE__, rowCount);

        memset(query, 0, sizeof(query));
        snprintf(query, QUERY_LENGTH, "DELETE FROM LOG_PRNT WHERE rowid IN (SELECT rowid FROM LOG_PRNT ORDER BY rowid ASC limit 1);");
        db_retval = db_trySQLiteExec(query);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "NVRAMAPI(%d): Fail to delete printlog\n", __LINE__);
        }
        rowCount = db_get_data_count("LOG_PRNT", NULL, &db_retval);
        if(db_retval == FAIL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): rowCount query is failed.\n", __LINE__);
            return FAIL;
        }
    }
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Log count is %d\n", __LINE__, rowCount);

    memset(query, 0, sizeof(query));
#ifdef HAVE_RTC
    snprintf(query, QUERY_LENGTH, "INSERT INTO LOG_PRNT VALUES (%d, %d, %d)", log_entry->page_count, log_entry->code, log_entry->time);
#else
    snprintf(query, QUERY_LENGTH, "INSERT INTO LOG_PRNT(page_count, code) VALUES (%d, %d)", log_entry->page_count, log_entry->code);
#endif
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteExec(query);

    return db_retval;
}

error_type_t db_trySQLiteGetTable(sqlite3* pDB, char* query, char*** queryResult, int* resultRow, int* resultCol)
{
    uint32_t db_retval = 0;
    uint32_t retval = FAIL;
    char* errMsg;

    while(1)
    {
        db_retval = sqlite3_get_table( pDB, query, queryResult, resultRow, resultCol, &errMsg );
        DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query Result %d\n", __LINE__, db_retval);

        if (db_retval == SQLITE_OK)
        {
            DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query Success: %s\n", __LINE__, query);
            retval = OK;
            break;
        }
        else if(db_retval == SQLITE_ERROR)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query : %s\n", __LINE__, query);
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, errMsg);
            sqlite3_free(errMsg);
            break;
        }
        else
        {
            sqlite3_free(errMsg);
            switch (db_retval % 0x100)
            {
                case SQLITE_BUSY:
                case SQLITE_LOCKED:
                    sqlite3_busy_timeout(pDB, 2000); // ms
					break;
				case SQLITE_IOERR:
					if ( db_retval >> 8 != 0 )
                        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Detailed Query Error(%d)\n", __LINE__, db_retval >> 8);
					else
					{
                        DBG_PRINTF(LOG_CRIT|DBG_NVRAM, "NVRAMAPI(%d): Can't execute anymore\n", __LINE__);
						while (1) sleep(1000);
					}
					break;
				default:
					break;
			}
			usleep(5000);
		}
	}
    return retval;
}

// If this function called, you must add MEM_FREE_AND_NULL(retValue); code.
char* db_get_var_query(char* query, error_type_t* db_retVal)
{
    char** queryResult = NULL;
    int queryRow = 0;
    int queryCol = 0;
    char* retValue;
    uint32_t retValueLen = 0;

    if (query == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        *db_retVal = FAIL;
        return NULL;
    }

    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);

    *db_retVal = db_trySQLiteGetTable(dbInstance, query, &queryResult, &queryRow, &queryCol);

    if (queryRow != 0)
    {
	    if(queryResult[queryCol] == 0)
		{
			retValue = MEM_MALLOC(1);
			retValue[0] = 0;
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Null pointer return\n", __LINE__, retValue);
		}
		else
		{
			if(queryResult[queryCol][0] == 0)
			{
				retValue = MEM_MALLOC(1);
				retValue[0] = 0;
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Null retValue\n", __LINE__, retValue);
			}
			else
			{
				retValueLen = strlen(queryResult[queryCol]);
				retValue = MEM_MALLOC(retValueLen+1);
				strncpy(retValue, queryResult[queryCol], strlen(queryResult[queryCol]));
				retValue[retValueLen] = 0;
				DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): retValue : %s\n", __LINE__, retValue);
			}
		}
    }
    else
    {
        retValue = NULL;
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): No data\n", __LINE__);
    }
	if(queryResult != NULL)
		sqlite3_free_table(queryResult);

    *db_retVal = OK;
    return retValue;
}

// If this function called, you must add MEM_FREE_AND_NULL(retValue); code.
char* db_get_var(char* tblName, char* attrName, error_type_t* db_retVal)
{
    char query[QUERY_LENGTH];
    char** queryResult = NULL;
    int queryRow = 0;
    int queryCol = 0;
    char* retValue;
    uint32_t retValueLen = 0;

    if ((tblName == NULL) || (attrName == NULL))
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        *db_retVal = FAIL;
        return NULL;
    }

    memset(query, 0, sizeof(query));
	if(!strncmp(tblName, "CONF", 4))
		snprintf(query, QUERY_LENGTH, "SELECT \"%s\" FROM %s WHERE NO=4", attrName, tblName);
	else if(!strncmp(tblName, "RECD", 4))
        snprintf(query, QUERY_LENGTH, "SELECT \"%s\" FROM %s ORDER BY rowid DESC limit 1;", attrName, tblName);
	else
		snprintf(query, QUERY_LENGTH, "SELECT \"%s\" FROM %s WHERE NO=1", attrName, tblName);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);

    *db_retVal = db_trySQLiteGetTable(dbInstance, query, &queryResult, &queryRow, &queryCol);

    if (queryRow != 0)
    {
	    if(queryResult[queryCol] == 0)
		{
			retValue = MEM_MALLOC(1);
			retValue[0] = 0;
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Null pointer return\n", __LINE__, retValue);
		}
		else
		{
			if(queryResult[queryCol][0] == 0)
			{
				retValue = MEM_MALLOC(1);
				retValue[0] = 0;
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Null retValue\n", __LINE__, retValue);
			}
			else
			{
				retValueLen = strlen(queryResult[queryCol]);
				retValue = MEM_MALLOC(retValueLen+1);
				strncpy(retValue, queryResult[queryCol], strlen(queryResult[queryCol]));
				retValue[retValueLen] = 0;
				DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): retValue : %s\n", __LINE__, retValue);
			}
		}
    }
    else
    {
        retValue = NULL;
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): No data\n", __LINE__);
    }
	if(queryResult != NULL)
		sqlite3_free_table(queryResult);

    *db_retVal = OK;
    return retValue;
}

int db_get_var_int(char* tblName, char* attrName, error_type_t* db_retVal)
{
    int retValue = -9999;
    char* tempValue;
    tempValue = db_get_var(tblName, attrName, db_retVal);

    if(*db_retVal == OK)
    {
        if(tempValue == NULL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid return value \n", __LINE__);
            return retValue;
        }

        retValue = atoi(tempValue);
    }
    MEM_FREE_AND_NULL(tempValue);
    return retValue;
}

error_type_t db_get_printlog(print_log_entry_t* log_entry)
{
    char query[QUERY_LENGTH];
    char** queryResult = NULL;
    int queryRow = 0;
    int queryCol = 0;
    error_type_t db_retval = FAIL;

    if (log_entry == NULL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): invalid input parameters\n", __LINE__);
        ASSERT(false);
        return FAIL;
    }

    memset(query, 0, sizeof(query));
    snprintf(query, QUERY_LENGTH, "SELECT * FROM LOG_PRNT ORDER BY rowid ASC limit 1");
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): %s\n", __LINE__, query);
    db_retval = db_trySQLiteGetTable(dbInstance, query, &queryResult, &queryRow, &queryCol);

    if (db_retval == FAIL)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Fail to load the DB data, __LINE__");
        return FAIL;
    }

    if (queryRow == 0)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): No data\n", __LINE__);
        return FAIL;
    }

    log_entry->page_count = atoi(queryResult[LOG_PRNT_PAGE_COUNT]);
    log_entry->code = atoi(queryResult[LOG_PRNT_CODE]);
#ifdef HAVE_RTC
    log_entry->time = atoi(queryResult[LOG_PRNT_TIME]);
#endif

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);

    return OK;
}

error_type_t db_get_table(char* query, char*** result, int* resultRow, int* resultCol)
{
	char* errMsg = NULL;
	int try_count = 0;
	int db_return = 0;

	while(true)
	{
		db_return=sqlite3_get_table(dbInstance, query, result, resultRow, resultCol, &errMsg);
		if ( db_return == SQLITE_OK )
			break;
		else if ( db_return == SQLITE_ERROR)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query : %s\n", __LINE__, query);
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, errMsg);
			sqlite3_free(errMsg);
			break;
		}
		else
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query : %s\n", __LINE__, query);
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, errMsg);
			sqlite3_free(errMsg);
			switch (db_return%0x100)
			{
				case SQLITE_BUSY:
					break;
				case SQLITE_LOCKED:
					break;
				case SQLITE_IOERR:
					if ( db_return>>8 != 0 )
					{
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Detailed Code : %d\n", __LINE__, db_return>>8);
					}
					else
					{
						DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): (SQLITE3)<->(NFS) Fatal ERROR: Not Bug but FATAL, Cannot execute anymore(%s)\n", __LINE__, errMsg);
						while (1) sleep(1000);
					}
					break;
				default:
					break;
			}
			try_count++;
			usleep(5000);
		}
	}
	if (try_count > 0)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Try count...%d\n", __LINE__, try_count);
		if (try_count > 5)
		{
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query: %s\n", __LINE__, query);
		}
	}
	if (db_return == 0)
		return OK;
	return FAIL;
}

error_type_t db_get_errlog(ERRLOG_STRUCT* err_result, int* row_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_ERRLOG ORDER BY NO DESC;");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_ERRLOG_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_ERRLOG_ROW;
	if(queryRow == 0)
	{
		err_result[0].err_type = -1;
		err_result[0].err_no = -1;
		err_result[0].err_date = -1;
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Row count %d\n", __LINE__, (*row_count));
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+5]);
			err_result[indexRow-1].err_type = atoi(queryResult[indexRow*queryCol+1]);
			err_result[indexRow-1].err_no = atoi(queryResult[indexRow*queryCol+2]);
			err_result[indexRow-1].err_date = atoi(queryResult[indexRow*queryCol+5]);
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_faxspamnumber(FAXSPAM_STRUCT* faxspam_result, int* row_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT FaxNumber FROM RECD_FAXSPAM");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXSPAM_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXSPAM_ROW;
	if(queryRow == 0)
	{
		faxspam_result[0].faxspamnumber[0] = 0;
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Row count %d\n", __LINE__, (*row_count));
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s\n", __LINE__, queryResult[indexRow*queryCol]);
			if(queryResult[indexRow*queryCol] == NULL)
				faxspam_result[indexRow-1].faxspamnumber[0] = 0;
			else
				snprintf(faxspam_result[indexRow-1].faxspamnumber, sizeof(faxspam_result[indexRow-1].faxspamnumber), "%s", queryResult[indexRow*queryCol]);
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_fax_broadcast_log(FAXBROADCASTLOG_STRUCT* fax_result, int* row_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;

	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXBROADCASTRESULTREPORT;");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= LOG_FAXBROADCASTLOG_ROW_SIZE)
		*row_count = queryRow;
	else
		*row_count = LOG_FAXBROADCASTLOG_ROW_SIZE;
	if(queryRow == 0)
	{
		fax_result[0].jobid = -1;
		fax_result[0].remotenum[0] = 0;  // null
		fax_result[0].datetime = -1;
		fax_result[0].duration = -1;
		fax_result[0].totalpageno = -1;
		fax_result[0].sentpageno = -1;
		fax_result[0].result = -1;
	}
	else
	{
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+3], queryResult[indexRow*queryCol+4], queryResult[indexRow*queryCol+5], queryResult[indexRow*queryCol+6], queryResult[indexRow*queryCol+7]);
			fax_result[indexRow-1].jobid = atoi(queryResult[indexRow*queryCol+1]);
			if(strlen(queryResult[indexRow*queryCol+5]) > MAX_FAX_NUMBER_LENGTH)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length RemoteNum\n", __LINE__);
			strncpy(fax_result[indexRow-1].remotenum, queryResult[indexRow*queryCol+2], strlen(queryResult[indexRow*queryCol+2]));
			fax_result[indexRow-1].remotenum[strlen(queryResult[indexRow*queryCol+2])] = 0;
			fax_result[indexRow-1].datetime = atoi(queryResult[indexRow*queryCol+3]);
			fax_result[indexRow-1].duration = atoi(queryResult[indexRow*queryCol+4]);
			fax_result[indexRow-1].totalpageno = atoi(queryResult[indexRow*queryCol+5]);
			fax_result[indexRow-1].sentpageno = atoi(queryResult[indexRow*queryCol+6]);
			fax_result[indexRow-1].result = atoi(queryResult[indexRow*queryCol+7]);
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_addressbook(ADDRESSBOOK_STRUCT* addr_result, int* row_count, int* total_line_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT Name, SpeedDial, FaxNum, Email FROM RECD_USERADDR;");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_ADDRESSBOOK_ENTRY)
		*row_count = queryRow;
	else
		*row_count = MAX_ADDRESSBOOK_ENTRY;
	if(queryRow == 0)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): No DB Result\n", __LINE__);
	}
	else
	{
		int line_count = 0;
		*total_line_count = 0;
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Row count %d\n", __LINE__, (*row_count));
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			line_count = 0;
			if(indexRow == 0)
				continue;   // First line is "Column Name"

			if(queryResult[indexRow*queryCol] == NULL)
				addr_result[indexRow-1].name[0] = 0;
			else
				snprintf(addr_result[indexRow-1].name, sizeof(addr_result[indexRow-1].name), "%s", queryResult[indexRow*queryCol]);
			if(ADDR_MAX_NAME_LENGTH <= strlen(addr_result[indexRow-1].name))
			{
				addr_result[indexRow-1].name[strlen(addr_result[indexRow-1].name)-4] = '.';
				addr_result[indexRow-1].name[strlen(addr_result[indexRow-1].name)-3] = '.';
				addr_result[indexRow-1].name[strlen(addr_result[indexRow-1].name)-2] = '.';
				addr_result[indexRow-1].name[strlen(addr_result[indexRow-1].name)-1] = 0;
			}
			if(strlen(addr_result[indexRow-1].name) > 0)
				line_count++;
			if(queryResult[indexRow*queryCol+1] == NULL)
				addr_result[indexRow-1].speeddial[0] = 0;
			else
				snprintf(addr_result[indexRow-1].speeddial, sizeof(addr_result[indexRow-1].speeddial), "%s", queryResult[indexRow*queryCol+1]);
			if(queryResult[indexRow*queryCol+2] == NULL)
				addr_result[indexRow-1].faxnumber[0] = 0;
			else
				snprintf(addr_result[indexRow-1].faxnumber, sizeof(addr_result[indexRow-1].faxnumber), "%s", queryResult[indexRow*queryCol+2]);
			if(ADDR_MAX_FAXNUMBER_LENGTH <= strlen(addr_result[indexRow-1].faxnumber))
			{
				addr_result[indexRow-1].faxnumber[strlen(addr_result[indexRow-1].faxnumber)-4] = '.';
				addr_result[indexRow-1].faxnumber[strlen(addr_result[indexRow-1].faxnumber)-3] = '.';
				addr_result[indexRow-1].faxnumber[strlen(addr_result[indexRow-1].faxnumber)-2] = '.';
				addr_result[indexRow-1].faxnumber[strlen(addr_result[indexRow-1].faxnumber)-1] = 0;
			}
			if(strlen(addr_result[indexRow-1].faxnumber) > 0)
				line_count++;
			if(queryResult[indexRow*queryCol+3] == NULL)
				addr_result[indexRow-1].email[0] = 0;
			else
				snprintf(addr_result[indexRow-1].email, sizeof(addr_result[indexRow-1].email), "%s", queryResult[indexRow*queryCol+3]);
			if(ADDR_MAX_EMAIL_LENGTH <= strlen(addr_result[indexRow-1].email))
			{
				addr_result[indexRow-1].email[strlen(addr_result[indexRow-1].email)-4] = '.';
				addr_result[indexRow-1].email[strlen(addr_result[indexRow-1].email)-3] = '.';
				addr_result[indexRow-1].email[strlen(addr_result[indexRow-1].email)-2] = '.';
				addr_result[indexRow-1].email[strlen(addr_result[indexRow-1].email)-1] = 0;
			}
			if(strlen(addr_result[indexRow-1].email) > 0)
				line_count++;
			addr_result[indexRow-1].line_count = line_count;
			*total_line_count = *total_line_count + line_count;
		}
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Total line count %d\n", __LINE__, (*total_line_count));
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_tonerhistory(char (*tonertli)[20], char (*tonerserial)[20], int* row_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;

	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_TONERHISTORY;");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_TONERHISTORY_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_TONERHISTORY_ROW;
	if(queryRow == 0)
	{
		snprintf(tonertli[0], sizeof(tonertli[0]), " ");
		snprintf(tonerserial[1], sizeof(tonerserial[1]), " ");
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Row count %d\n", __LINE__, (*row_count));
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2]);
			snprintf(tonertli[indexRow-1], sizeof(tonertli[indexRow-1]), "%s", queryResult[indexRow*queryCol+1]);
			snprintf(tonerserial[indexRow-1], sizeof(tonerserial[indexRow-1]), "%s", queryResult[indexRow*queryCol+2]);
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_faxfcode(FAXFCODE_STRUCT* fax_result, int* row_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXCODE;");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXFCODE_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXFCODE_ROW;
	if(queryRow == 0)
	{
		fax_result[0].boxtype = -1;
		fax_result[0].subaddress[0] = 0;
		fax_result[0].password[0] = 0;
		fax_result[0].isimagesaved = -1;
		fax_result[0].boxpwd[0] = 0;
		fax_result[0].dialnumlist[0] = 0;
		fax_result[0].boxname[0] = 0;
	}
	else
	{
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+3], queryResult[indexRow*queryCol+4], queryResult[indexRow*queryCol+5], queryResult[indexRow*queryCol+6], queryResult[indexRow*queryCol+7]);

			fax_result[indexRow-1].boxtype = atoi(queryResult[indexRow*queryCol+1]);
			if(strlen(queryResult[indexRow*queryCol+2]) > MAX_FAX_LENGTH_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length subaddress\n", __LINE__);
			strncpy(fax_result[indexRow-1].subaddress, queryResult[indexRow*queryCol+2], strlen(queryResult[indexRow*queryCol+2]));
			fax_result[indexRow-1].subaddress[strlen(queryResult[indexRow*queryCol+2])] = 0;
			if(strlen(queryResult[indexRow*queryCol+3]) > MAX_FAX_LENGTH_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length password\n", __LINE__);
			strncpy(fax_result[indexRow-1].password, queryResult[indexRow*queryCol+3], strlen(queryResult[indexRow*queryCol+3]));
			fax_result[indexRow-1].password[strlen(queryResult[indexRow*queryCol+3])] = 0;
			fax_result[indexRow-1].isimagesaved = atoi(queryResult[indexRow*queryCol+4]);
			if(strlen(queryResult[indexRow*queryCol+5]) > MAX_FAX_LENGTH_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length boxpwd\n", __LINE__);
			strncpy(fax_result[indexRow-1].boxpwd, queryResult[indexRow*queryCol+5], strlen(queryResult[indexRow*queryCol+5]));
			fax_result[indexRow-1].boxpwd[strlen(queryResult[indexRow*queryCol+5])] = 0;
			if(strlen(queryResult[indexRow*queryCol+6]) > MAX_FAX_NUMBER_LENGTH_RELAY_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length dialnumlist\n", __LINE__);
			strncpy(fax_result[indexRow-1].dialnumlist, queryResult[indexRow*queryCol+6], strlen(queryResult[indexRow*queryCol+6]));
			fax_result[indexRow-1].dialnumlist[strlen(queryResult[indexRow*queryCol+6])] = 0;
			if(strlen(queryResult[indexRow*queryCol+7]) > MAX_FAX_LENGTH_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length dialnumlist\n", __LINE__);
			strncpy(fax_result[indexRow-1].boxname, queryResult[indexRow*queryCol+7], strlen(queryResult[indexRow*queryCol+7]));
			fax_result[indexRow-1].boxname[strlen(queryResult[indexRow*queryCol+7])] = 0;
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_faxfcode_from_subaddress(FAXFCODE_STRUCT* fax_result, int* row_count, unsigned char* sub_address)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXCODE WHERE Subaddress=%s;",sub_address);
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXFCODE_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXFCODE_ROW;
	if(queryRow == 0)
	{
		fax_result[0].boxtype = -1;
		fax_result[0].subaddress[0] = 0;
		fax_result[0].password[0] = 0;
		fax_result[0].isimagesaved = -1;
		fax_result[0].boxpwd[0] = 0;
		fax_result[0].dialnumlist[0] = 0;
		fax_result[0].boxname[0] = 0;
	}
	else
	{
		DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s\n", __LINE__, 
				queryResult[queryCol+1], queryResult[queryCol+2], queryResult[queryCol+3], queryResult[queryCol+4], queryResult[queryCol+5], queryResult[queryCol+6], queryResult[queryCol+7]);

		fax_result[0].boxtype = atoi(queryResult[queryCol+1]);
		if(strlen(queryResult[queryCol+2]) > MAX_FAX_LENGTH_FCODE)
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length subaddress\n", __LINE__);
		strncpy(fax_result[0].subaddress, queryResult[queryCol+2], strlen(queryResult[queryCol+2]));
		fax_result[0].subaddress[strlen(queryResult[queryCol+2])] = 0;
		if(strlen(queryResult[queryCol+3]) > MAX_FAX_LENGTH_FCODE)
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length password\n", __LINE__);
		strncpy(fax_result[0].password, queryResult[queryCol+3], strlen(queryResult[queryCol+3]));
		fax_result[0].password[strlen(queryResult[queryCol+3])] = 0;
		fax_result[0].isimagesaved = atoi(queryResult[queryCol+4]);
		if(strlen(queryResult[queryCol+5]) > MAX_FAX_LENGTH_FCODE)
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length boxpwd\n", __LINE__);
		strncpy(fax_result[0].boxpwd, queryResult[queryCol+5], strlen(queryResult[queryCol+5]));
		fax_result[0].boxpwd[strlen(queryResult[queryCol+5])] = 0;
		if(strlen(queryResult[queryCol+6]) > MAX_FAX_NUMBER_LENGTH_RELAY_FCODE)
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length dialnumlist\n", __LINE__);
		strncpy(fax_result[0].dialnumlist, queryResult[queryCol+6], strlen(queryResult[queryCol+6]));
		fax_result[0].dialnumlist[strlen(queryResult[queryCol+6])] = 0;
		if(strlen(queryResult[queryCol+7]) > MAX_FAX_LENGTH_FCODE)
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length dialnumlist\n", __LINE__);
		strncpy(fax_result[0].boxname, queryResult[queryCol+7], strlen(queryResult[queryCol+7]));
		fax_result[0].boxname[strlen(queryResult[queryCol+7])] = 0;
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_faxfcode_from_type(FAXFCODE_STRUCT* fax_result, int* row_count, int box_type)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXCODE WHERE BoxType=%d;",box_type);
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXFCODE_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXFCODE_ROW;
	if(queryRow == 0)
	{
		fax_result[0].boxtype = -1;
		fax_result[0].subaddress[0] = 0;
		fax_result[0].password[0] = 0;
		fax_result[0].isimagesaved = -1;
		fax_result[0].boxpwd[0] = 0;
		fax_result[0].dialnumlist[0] = 0;
		fax_result[0].boxname[0] = 0;
	}
	else
	{
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+3], queryResult[indexRow*queryCol+4], queryResult[indexRow*queryCol+5], queryResult[indexRow*queryCol+6], queryResult[indexRow*queryCol+7]);

			fax_result[indexRow-1].boxtype = atoi(queryResult[indexRow*queryCol+1]);
			if(strlen(queryResult[indexRow*queryCol+2]) > MAX_FAX_LENGTH_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length subaddress\n", __LINE__);
			strncpy(fax_result[indexRow-1].subaddress, queryResult[indexRow*queryCol+2], strlen(queryResult[indexRow*queryCol+2]));
			fax_result[indexRow-1].subaddress[strlen(queryResult[indexRow*queryCol+2])] = 0;
			if(strlen(queryResult[indexRow*queryCol+3]) > MAX_FAX_LENGTH_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length password\n", __LINE__);
			strncpy(fax_result[indexRow-1].password, queryResult[indexRow*queryCol+3], strlen(queryResult[indexRow*queryCol+3]));
			fax_result[indexRow-1].password[strlen(queryResult[indexRow*queryCol+3])] = 0;
			fax_result[indexRow-1].isimagesaved = atoi(queryResult[indexRow*queryCol+4]);
			if(strlen(queryResult[indexRow*queryCol+5]) > MAX_FAX_LENGTH_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length boxpwd\n", __LINE__);
			strncpy(fax_result[indexRow-1].boxpwd, queryResult[indexRow*queryCol+5], strlen(queryResult[indexRow*queryCol+5]));
			fax_result[indexRow-1].boxpwd[strlen(queryResult[indexRow*queryCol+5])] = 0;
			if(strlen(queryResult[indexRow*queryCol+6]) > MAX_FAX_NUMBER_LENGTH_RELAY_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length dialnumlist\n", __LINE__);
			strncpy(fax_result[indexRow-1].dialnumlist, queryResult[indexRow*queryCol+6], strlen(queryResult[indexRow*queryCol+6]));
			fax_result[indexRow-1].dialnumlist[strlen(queryResult[indexRow*queryCol+6])] = 0;
			if(strlen(queryResult[indexRow*queryCol+7]) > MAX_FAX_LENGTH_FCODE)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length dialnumlist\n", __LINE__);
			strncpy(fax_result[indexRow-1].boxname, queryResult[indexRow*queryCol+7], strlen(queryResult[indexRow*queryCol+7]));
			fax_result[indexRow-1].boxname[strlen(queryResult[indexRow*queryCol+7])] = 0;
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_faxlog(FAXLOG_STRUCT* fax_result, int* row_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXLOG;");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXLOG_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXLOG_ROW;
	if(queryRow == 0)
	{
		fax_result[0].datetime = -1;
		fax_result[0].faxmode = -1;
		fax_result[0].ecm = -1;
		fax_result[0].resolution = -1;
		fax_result[0].remotenum[0] = 0;  // null
		fax_result[0].duration = -1;
		fax_result[0].pagenum = -1;
		fax_result[0].connectspeed = -1;
		fax_result[0].result = -1;
		fax_result[0].recname = -1;
	}
	else
	{
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s %s %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+3], queryResult[indexRow*queryCol+4], queryResult[indexRow*queryCol+5], queryResult[indexRow*queryCol+6], queryResult[indexRow*queryCol+7], queryResult[indexRow*queryCol+8], queryResult[indexRow*queryCol+9], queryResult[indexRow*queryCol+10], queryResult[indexRow*queryCol+11]);
			fax_result[indexRow-1].datetime = atoi(queryResult[indexRow*queryCol+1]);
			fax_result[indexRow-1].faxmode = atoi(queryResult[indexRow*queryCol+2]);
			fax_result[indexRow-1].ecm = atoi(queryResult[indexRow*queryCol+3]);
			fax_result[indexRow-1].resolution = atoi(queryResult[indexRow*queryCol+4]);
			if(strlen(queryResult[indexRow*queryCol+5]) > MAX_FAX_NUMBER_LENGTH)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length RemoteNum\n", __LINE__);
			strncpy(fax_result[indexRow-1].remotenum, queryResult[indexRow*queryCol+5], strlen(queryResult[indexRow*queryCol+5]));
			fax_result[indexRow-1].remotenum[strlen(queryResult[indexRow*queryCol+5])] = 0;
			fax_result[indexRow-1].duration = atoi(queryResult[indexRow*queryCol+6]);
			fax_result[indexRow-1].pagenum = atoi(queryResult[indexRow*queryCol+7]);
			fax_result[indexRow-1].connectspeed = atoi(queryResult[indexRow*queryCol+8]);
			fax_result[indexRow-1].result = atoi(queryResult[indexRow*queryCol+9]);
			fax_result[indexRow-1].recname = atoi(queryResult[indexRow*queryCol+10]);
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

//Temporary used in Juniper
error_type_t db_get_faxlog_jobid(FAXLOG_STRUCT_JOBID* fax_result, int* row_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXLOG;");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXLOG_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXLOG_ROW;
	if(queryRow == 0)
	{
		fax_result[0].datetime = -1;
		fax_result[0].faxmode = -1;
		fax_result[0].ecm = -1;
		fax_result[0].resolution = -1;
		fax_result[0].remotenum[0] = 0;  // null
		fax_result[0].duration = -1;
		fax_result[0].pagenum = -1;
		fax_result[0].connectspeed = -1;
		fax_result[0].result = -1;
		fax_result[0].recname = -1;
		fax_result[0].jobid = -1;
	}
	else
	{
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s %s %s %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+3], queryResult[indexRow*queryCol+4], queryResult[indexRow*queryCol+5], queryResult[indexRow*queryCol+6], queryResult[indexRow*queryCol+7], queryResult[indexRow*queryCol+8], queryResult[indexRow*queryCol+9], queryResult[indexRow*queryCol+10], queryResult[indexRow*queryCol+11], queryResult[indexRow*queryCol+12]);
			fax_result[indexRow-1].datetime = atoi(queryResult[indexRow*queryCol+1]);
			fax_result[indexRow-1].faxmode = atoi(queryResult[indexRow*queryCol+2]);
			fax_result[indexRow-1].ecm = atoi(queryResult[indexRow*queryCol+3]);
			fax_result[indexRow-1].resolution = atoi(queryResult[indexRow*queryCol+4]);
			if(strlen(queryResult[indexRow*queryCol+5]) > MAX_FAX_NUMBER_LENGTH)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length RemoteNum\n", __LINE__);
			strncpy(fax_result[indexRow-1].remotenum, queryResult[indexRow*queryCol+5], strlen(queryResult[indexRow*queryCol+5]));
			fax_result[indexRow-1].remotenum[strlen(queryResult[indexRow*queryCol+5])] = 0;
			fax_result[indexRow-1].duration = atoi(queryResult[indexRow*queryCol+6]);
			fax_result[indexRow-1].pagenum = atoi(queryResult[indexRow*queryCol+7]);
			fax_result[indexRow-1].connectspeed = atoi(queryResult[indexRow*queryCol+8]);
			fax_result[indexRow-1].result = atoi(queryResult[indexRow*queryCol+9]);
			fax_result[indexRow-1].recname = atoi(queryResult[indexRow*queryCol+10]);
			fax_result[indexRow-1].jobid = atoi(queryResult[indexRow*queryCol+11]);
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_faxlog_last_job_from_fax_mode(FAXLOG_STRUCT* fax_result, int* row_count, int fax_type)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	if(fax_type == FAX_TYPE_RX) //RX
	{
		queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXLOG WHERE FaxMode == 1;");
	}
	else //TX
	{
		queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXLOG WHERE FaxMode != 1;");
	}
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXLOG_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXLOG_ROW;
	if(queryRow == 0)
	{
		fax_result[0].datetime = -1;
		fax_result[0].faxmode = -1;
		fax_result[0].ecm = -1;
		fax_result[0].resolution = -1;
		fax_result[0].remotenum[0] = 0;  // null
		fax_result[0].duration = -1;
		fax_result[0].pagenum = -1;
		fax_result[0].connectspeed = -1;
		fax_result[0].result = -1;
		fax_result[0].recname = -1;
	}
	else
	{
		indexRow= (*row_count);
		DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s %s %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+3], queryResult[indexRow*queryCol+4], queryResult[indexRow*queryCol+5], queryResult[indexRow*queryCol+6], queryResult[indexRow*queryCol+7], queryResult[indexRow*queryCol+8], queryResult[indexRow*queryCol+9], queryResult[indexRow*queryCol+10], queryResult[indexRow*queryCol+11]);
		fax_result[0].datetime = atoi(queryResult[indexRow*queryCol+1]);
		fax_result[0].faxmode = atoi(queryResult[indexRow*queryCol+2]);
		fax_result[0].ecm = atoi(queryResult[indexRow*queryCol+3]);
		fax_result[0].resolution = atoi(queryResult[indexRow*queryCol+4]);
		if(strlen(queryResult[indexRow*queryCol+5]) > MAX_FAX_NUMBER_LENGTH)
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length RemoteNum\n", __LINE__);
		strncpy(fax_result[0].remotenum, queryResult[indexRow*queryCol+5], strlen(queryResult[indexRow*queryCol+5]));
		fax_result[0].remotenum[strlen(queryResult[indexRow*queryCol+5])] = 0;
		fax_result[0].duration = atoi(queryResult[indexRow*queryCol+6]);
		fax_result[0].pagenum = atoi(queryResult[indexRow*queryCol+7]);
		fax_result[0].connectspeed = atoi(queryResult[indexRow*queryCol+8]);
		fax_result[0].result = atoi(queryResult[indexRow*queryCol+9]);
		fax_result[0].recname = atoi(queryResult[indexRow*queryCol+10]);
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

//Temporary used in Juniper
error_type_t db_get_faxlog_last_job_from_fax_mode_jobid(FAXLOG_STRUCT_JOBID* fax_result, int* row_count, int fax_type, int jobID)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	if(fax_type == FAX_TYPE_RX) //RX
	{
		queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXLOG WHERE FaxMode == 1 AND JobID = %d;", jobID);
	}
	else //TX
	{
		queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXLOG WHERE FaxMode != 1 AND JobID = %d;", jobID);
	}
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXLOG_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXLOG_ROW;
	if(queryRow == 0)
	{
		fax_result[0].datetime = -1;
		fax_result[0].faxmode = -1;
		fax_result[0].ecm = -1;
		fax_result[0].resolution = -1;
		fax_result[0].remotenum[0] = 0;  // null
		fax_result[0].duration = -1;
		fax_result[0].pagenum = -1;
		fax_result[0].connectspeed = -1;
		fax_result[0].result = -1;
		fax_result[0].recname = -1;
		fax_result[0].jobid = -1;
	}
	else
	{
		indexRow= (*row_count);
		DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s %s %s %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+3], queryResult[indexRow*queryCol+4], queryResult[indexRow*queryCol+5], queryResult[indexRow*queryCol+6], queryResult[indexRow*queryCol+7], queryResult[indexRow*queryCol+8], queryResult[indexRow*queryCol+9], queryResult[indexRow*queryCol+10], queryResult[indexRow*queryCol+11], queryResult[indexRow*queryCol+12]);
		fax_result[0].datetime = atoi(queryResult[indexRow*queryCol+1]);
		fax_result[0].faxmode = atoi(queryResult[indexRow*queryCol+2]);
		fax_result[0].ecm = atoi(queryResult[indexRow*queryCol+3]);
		fax_result[0].resolution = atoi(queryResult[indexRow*queryCol+4]);
		if(strlen(queryResult[indexRow*queryCol+5]) > MAX_FAX_NUMBER_LENGTH)
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length RemoteNum\n", __LINE__);
		strncpy(fax_result[0].remotenum, queryResult[indexRow*queryCol+5], strlen(queryResult[indexRow*queryCol+5]));
		fax_result[0].remotenum[strlen(queryResult[indexRow*queryCol+5])] = 0;
		fax_result[0].duration = atoi(queryResult[indexRow*queryCol+6]);
		fax_result[0].pagenum = atoi(queryResult[indexRow*queryCol+7]);
		fax_result[0].connectspeed = atoi(queryResult[indexRow*queryCol+8]);
		fax_result[0].result = atoi(queryResult[indexRow*queryCol+9]);
		fax_result[0].recname = atoi(queryResult[indexRow*queryCol+10]);
		fax_result[0].jobid = atoi(queryResult[indexRow*queryCol+11]);
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

error_type_t db_get_faxsplog(FAXSPLOG_STRUCT* fax_result, int* row_count)
{
	char** queryResult = NULL;
	int queryRow = 0;
	int queryCol = 0;
	int queryLength = 0;
	error_type_t db_retval = FAIL;
	char checkSQLStr[QUERY_LENGTH];
	int indexRow = 0;
	
	queryLength = snprintf(checkSQLStr, sizeof(checkSQLStr), "SELECT * FROM RECD_FAXSPLOG;");
	if(queryLength < 0 || queryLength >= sizeof(checkSQLStr))
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query size error!!!\n", __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Query Error(%s)\n", __LINE__, checkSQLStr);
		db_retval = FAIL;
		return db_retval;
	}

	db_retval = db_get_table(checkSQLStr, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL)
	{
		return db_retval;
	}
	if(queryRow <= MAX_FAXSPLOG_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FAXSPLOG_ROW;
	if(queryRow == 0)
	{
		fax_result[0].datetime = -1;
		fax_result[0].faxmode = -1;
		fax_result[0].ecm = -1;
		fax_result[0].resolution = -1;
		fax_result[0].remotenum[0] = 0;  // null
		fax_result[0].duration = -1;
		fax_result[0].pagenum = -1;
		fax_result[0].connectspeed = -1;
		fax_result[0].result = -1;
		fax_result[0].recname = -1;
	}
	else
	{
		for(indexRow = 0; indexRow <= (*row_count); indexRow++)
		{
			if(indexRow == 0)
				continue;   // First line is "Column Name"
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "NVRAMAPI(%d): Query : %s %s %s %s %s %s %s %s %s %s %s\n", __LINE__, queryResult[indexRow*queryCol+1], queryResult[indexRow*queryCol+2], queryResult[indexRow*queryCol+3], queryResult[indexRow*queryCol+4], queryResult[indexRow*queryCol+5], queryResult[indexRow*queryCol+6], queryResult[indexRow*queryCol+7], queryResult[indexRow*queryCol+8], queryResult[indexRow*queryCol+9], queryResult[indexRow*queryCol+10], queryResult[indexRow*queryCol+11]);
			fax_result[indexRow-1].datetime = atoi(queryResult[indexRow*queryCol+1]);
			fax_result[indexRow-1].faxmode = atoi(queryResult[indexRow*queryCol+2]);
			fax_result[indexRow-1].ecm = atoi(queryResult[indexRow*queryCol+3]);
			fax_result[indexRow-1].resolution = atoi(queryResult[indexRow*queryCol+4]);
			if(strlen(queryResult[indexRow*queryCol+5]) > MAX_FAX_NUMBER_LENGTH)
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): Over length RemoteNum\n", __LINE__);
			strncpy(fax_result[indexRow-1].remotenum, queryResult[indexRow*queryCol+5], strlen(queryResult[indexRow*queryCol+5]));
			fax_result[indexRow-1].remotenum[strlen(queryResult[indexRow*queryCol+5])] = 0;
			fax_result[indexRow-1].duration = atoi(queryResult[indexRow*queryCol+6]);
			fax_result[indexRow-1].pagenum = atoi(queryResult[indexRow*queryCol+7]);
			fax_result[indexRow-1].connectspeed = atoi(queryResult[indexRow*queryCol+8]);
			fax_result[indexRow-1].result = atoi(queryResult[indexRow*queryCol+9]);
			fax_result[indexRow-1].recname = atoi(queryResult[indexRow*queryCol+10]);
		}
	}

	if(queryResult != NULL)
		sqlite3_free_table(queryResult);
	db_retval = OK;
	return db_retval;
}

#if 0
error_type_t db_get_totalcount(COUNTER_STRUCT* perma_count)
{
	COUNTER_STRUCT stPermaCount;
	error_type_t db_retval = FAIL;
	FILE *bkresult = NULL;
	char backupDB[1024];
	memset(&stPermaCount, 0, sizeof(stPermaCount));
	memset(backupDB, 0, sizeof(backupDB));
	snprintf(backupDB,sizeof(backupDB),"sqlite3 /backup/point.db 'select perma_total, perma_host, perma_report, perma_usbdrive, perma_duplex, perma_copyprint, perma_faxprint from permaset limit 1;'");
	bkresult=popen(backupDB,"r");
	fscanf(bkresult,"%d|%d|%d|%d|%d|%d|%d",&(stPermaCount.total),&(stPermaCount.host),&(stPermaCount.report),&(stPermaCount.usbdrive),&(stPermaCount.duplex),&(stPermaCount.copyprint),&(stPermaCount.faxprint));
	pclose(bkresult);

	perma_count->total = db_get_var_int("INST_MACHINECOUNT", "TotalPrint", &db_retval);
	perma_count->host = db_get_var_int("INST_MACHINECOUNT", "HostPrint", &db_retval);
	perma_count->report = db_get_var_int("INST_MACHINECOUNT", "ReportPrint", &db_retval);
	perma_count->usbdrive = db_get_var_int("INST_MACHINECOUNT", "USBDrivePrint", &db_retval);
	perma_count->duplex = db_get_var_int("INST_MACHINECOUNT", "DuplexPrint", &db_retval);
	perma_count->copyprint = db_get_var_int("INST_MACHINECOUNT", "CopyPrint", &db_retval);
	perma_count->faxprint = db_get_var_int("INST_MACHINECOUNT", "FaxPrint", &db_retval);
	if(db_retval == FAIL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "DB Read Fail\n");
	}
	else
	{
		if( stPermaCount.total > perma_count->total || stPermaCount.host > perma_count->host || stPermaCount.report > perma_count->report || stPermaCount.usbdrive > perma_count->usbdrive || stPermaCount.duplex > perma_count->duplex || stPermaCount.copyprint > perma_count->copyprint || stPermaCount.faxprint > perma_count->faxprint )
		{
			FILE *fpLog = NULL;
			fpLog = fopen(BOTHINTEGRITY_LOGFILENAME,"wt");
			if(fpLog == NULL)
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", BOTHINTEGRITY_LOGFILENAME);
				db_retval = FAIL;
			}
			else
			{
				fprintf(fpLog,"Old: %d,%d,%d,%d,%d,%d,%d <-> New: %d,%d,%d,%d,%d,%d,%d\n", stPermaCount.total, stPermaCount.host, stPermaCount.report, stPermaCount.usbdrive, stPermaCount.duplex, stPermaCount.copyprint, stPermaCount.faxprint, perma_count->total, perma_count->host, perma_count->report, perma_count->usbdrive, perma_count->duplex, perma_count->copyprint, perma_count->faxprint);
				fclose(fpLog);
			}
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "DB FATAL!!!!! ---: Old: %d,%d,%d,%d,%d,%d,%d <-> New: %d,%d,%d,%d,%d,%d,%d\n", stPermaCount.total, stPermaCount.host, stPermaCount.report, stPermaCount.usbdrive, stPermaCount.duplex, stPermaCount.copyprint, stPermaCount.faxprint, perma_count->total, perma_count->host, perma_count->report, perma_count->usbdrive, perma_count->duplex, perma_count->copyprint, perma_count->faxprint);

			perma_count->total = stPermaCount.total;
			perma_count->host = stPermaCount.host;
			perma_count->report = stPermaCount.report;
			perma_count->usbdrive = stPermaCount.usbdrive;
			perma_count->duplex = stPermaCount.duplex;
			perma_count->copyprint = stPermaCount.copyprint;
			perma_count->faxprint = stPermaCount.faxprint;
		}
	}
	return db_retval;
}
#endif

error_type_t db_set_drumcount(COUNTER_STRUCT perma_count, int drum, int motor, int countinpercent, int drumlife)
{
	error_type_t db_retval = FAIL;
	
	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_drumcount: Set Drum Count!!\n\ttotal:\t%d\n\thost:\t%d\n\treport:\t%d\n\tusbdrive:\t%d\n\tduplex:\t%d\n\tcopyprint:\t%d\n\tfaxprint:\t%d\n\tdrum:\t%d\n\tmotor:\t%d\n\tcountinpercent:\t%d\n\tdrumlife:\t%d\n",perma_count.total,perma_count.host,perma_count.report,perma_count.usbdrive,perma_count.duplex,perma_count.copyprint,perma_count.faxprint,drum,motor,countinpercent,drumlife);

#if 0
	COUNTER_STRUCT tempPermaCount;
	memset(&tempPermaCount, 0, sizeof(tempPermaCount));

	db_retval = db_get_totalcount(&tempPermaCount);
	if(db_retval == FAIL)
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to get total count\n");

	if(tempPermaCount.total > perma_count.total || tempPermaCount.host > perma_count.host || tempPermaCount.report > perma_count.report || tempPermaCount.usbdrive > perma_count.usbdrive || tempPermaCount.duplex > perma_count.duplex || tempPermaCount.copyprint > perma_count.copyprint || tempPermaCount.faxprint > perma_count.faxprint)
	{
		FILE* fpLog = NULL;
		fpLog = fopen(PGINTEGRITY_LOGFILENAME,"wt");
		if(fpLog == NULL)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", PGINTEGRITY_LOGFILENAME);
			db_retval = FAIL;
		}
		else
		{
			fprintf(fpLog, "Backup: %d,%d,%d,%d,%d,%d,%d(...) <-> Original: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", tempPermaCount.total, tempPermaCount.host, tempPermaCount.report, tempPermaCount.usbdrive, tempPermaCount.duplex, tempPermaCount.copyprint, tempPermaCount.faxprint, perma_count.total, perma_count.host, perma_count.report, perma_count.usbdrive, perma_count.duplex, perma_count.copyprint, perma_count.faxprint, drum, motor, countinpercent,drumlife);
			fclose(fpLog);
		}
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "LOGIC FATAL!!!!! ---: Backup: %d,%d,%d,%d,%d,%d,%d(...) <-> Original: %d,%d,%d,%d,%d,%d,%d\n", tempPermaCount.total, tempPermaCount.host, tempPermaCount.report, tempPermaCount.usbdrive, tempPermaCount.duplex, tempPermaCount.copyprint, tempPermaCount.faxprint, perma_count.total, perma_count.host, perma_count.report, perma_count.usbdrive, perma_count.duplex, perma_count.copyprint, perma_count.faxprint, drum, motor, countinpercent,drumlife);
	}
#endif

	// set values
	db_retval = db_trySQLiteExec("BEGIN TRANSACTION;");
	char countQuery[QUERY_LENGTH];
	memset(countQuery, 0, sizeof(countQuery));
	snprintf(countQuery, sizeof(countQuery), "UPDATE INST_MACHINECOUNT SET TotalPrint=%d, HostPrint=%d, ReportPrint=%d, USBDrivePrint=%d, DuplexPrint=%d, CopyPrint=%d, FaxPrint=%d;", perma_count.total, perma_count.host, perma_count.report, perma_count.usbdrive, perma_count.duplex, perma_count.copyprint, perma_count.faxprint);
	db_retval = db_trySQLiteExec(countQuery);
	if(db_retval == FAIL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_drumcount: Writing machine count fail\n");
	}

	memset(countQuery, 0, sizeof(countQuery));
	snprintf(countQuery, sizeof(countQuery), "UPDATE INST_DRUMCOUNT SET Count=%d, CountInPercent=%d, Motor=%d, DrumLifePageCount=%d;", drum, countinpercent, motor, drumlife);
	db_retval = db_trySQLiteExec(countQuery);
	if(db_retval == FAIL){
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_drumcount: Writing drum count fail\n");
	}
	db_retval = db_trySQLiteExec("COMMIT;");

	//sqlite3_open
	char backup_cmd[1024];
	memset(backup_cmd, 0, sizeof(backup_cmd));
	snprintf(backup_cmd,sizeof(backup_cmd),"sqlite3 /backup/point.db 'UPDATE permaset SET perma_total=%d, perma_host=%d, perma_report=%d, perma_usbdrive=%d, perma_duplex=%d, perma_copyprint=%d, perma_faxprint=%d, count=%d, motor=%d, count_in_percent=%d, drum_life=%d'", perma_count.total, perma_count.host, perma_count.report, perma_count.usbdrive, perma_count.duplex, perma_count.copyprint, perma_count.faxprint,drum,motor,countinpercent,drumlife);
	system(backup_cmd);

	return db_retval;
}

#if 1
error_type_t db_get_drumcount(COUNTER_STRUCT *perma_count, int* drum, int* countinpercent, int* motor, int* drumlife)
{
	error_type_t db_retval = FAIL;
	char backupDB[1024];
	FILE *bkresult = NULL;
	COUNTER_STRUCT stPermaCount;
	int ptCount = 0;
	int ptCountInPercent = 0;
	int ptMotor = 0;
	int ptDrumLife = 0;

	// get point.db values
	memset(&stPermaCount, 0, sizeof(stPermaCount));
	memset(backupDB, 0, sizeof(backupDB));
	snprintf(backupDB,sizeof(backupDB),"sqlite3 /backup/point.db 'select perma_total,perma_host,perma_report,perma_usbdrive,perma_duplex,perma_copyprint,perma_faxprint,count,count_in_percent,motor,drum_life from permaset limit 1;'");
	bkresult=popen(backupDB,"r");
	if(bkresult == NULL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "sqlite3 cmd fail\n");
	}
	else
	{
		fscanf(bkresult,"%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d",&(stPermaCount.total),&(stPermaCount.host),&(stPermaCount.report),&(stPermaCount.usbdrive),&(stPermaCount.duplex),&(stPermaCount.copyprint),&(stPermaCount.faxprint),&ptCount,&ptCountInPercent,&ptMotor,&ptDrumLife);
		pclose(bkresult);
	}

	// get DB values
	perma_count->total = db_get_var_int("INST_MACHINECOUNT", "TotalPrint", &db_retval);
	perma_count->host = db_get_var_int("INST_MACHINECOUNT", "HostPrint", &db_retval);
	perma_count->report = db_get_var_int("INST_MACHINECOUNT", "ReportPrint", &db_retval);
	perma_count->usbdrive = db_get_var_int("INST_MACHINECOUNT", "USBDrivePrint", &db_retval);
	perma_count->duplex = db_get_var_int("INST_MACHINECOUNT", "DuplexPrint", &db_retval);
	perma_count->copyprint = db_get_var_int("INST_MACHINECOUNT", "CopyPrint", &db_retval);
	perma_count->faxprint = db_get_var_int("INST_MACHINECOUNT", "FaxPrint", &db_retval);
	(*drum) = db_get_var_int("INST_DRUMCOUNT", "Count", &db_retval);
	(*countinpercent) = db_get_var_int("INST_DRUMCOUNT", "CountInPercent", &db_retval);
	(*motor) = db_get_var_int("INST_DRUMCOUNT", "Motor", &db_retval);
	(*drumlife) = db_get_var_int("INST_DRUMCOUNT", "DrumLifePageCount", &db_retval);

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_get_drumcount: Count [Backup] <-> [Original]\n\ttotal:\t%d,%d\n\thost:\t%d,%d\n\treport:\t%d,%d\n\tusbdrive:\t%d,%d\n\tduplex:\t%d,%d\n\tcopyprint:\t%d,%d\n\tfaxprint:\t%d,%d\n\tdrum:\t%d,%d\n\tmotor:\t%d,%d\n\tcountinpercent:\t%d,%d\n\tdrumlife:\t%d,%d\n", stPermaCount.total, perma_count->total, stPermaCount.host, perma_count->host, stPermaCount.report, perma_count->report, stPermaCount.usbdrive, perma_count->usbdrive, stPermaCount.duplex, perma_count->duplex, stPermaCount.copyprint, perma_count->copyprint, stPermaCount.faxprint, perma_count->faxprint, ptCount, *drum, ptMotor, *motor, ptCountInPercent, *countinpercent, ptDrumLife, *drumlife);

	/*Counter Integrity Check Start*/
	if((stPermaCount.total > perma_count->total) && (perma_count->total > -1)) {
		db_logger_drumcount("total", stPermaCount.total, perma_count->total);
		perma_count->total = stPermaCount.total;
	}
	if((stPermaCount.host > perma_count->host) && (perma_count->host > -1)) {
		db_logger_drumcount("host", stPermaCount.host, perma_count->host);
		perma_count->host = stPermaCount.host;
	}
	if((stPermaCount.report > perma_count->report) && (perma_count->report > -1)) {
		db_logger_drumcount("report", stPermaCount.report, perma_count->report);
		perma_count->report = stPermaCount.report;
	}
	if((stPermaCount.usbdrive > perma_count->usbdrive) && (perma_count->usbdrive > -1)) {
		db_logger_drumcount("usbdrive", stPermaCount.usbdrive, perma_count->usbdrive);
		perma_count->usbdrive = stPermaCount.usbdrive;
	}
	if((stPermaCount.duplex > perma_count->duplex) && (perma_count->duplex > -1)) {
		db_logger_drumcount("duplex", stPermaCount.duplex, perma_count->duplex);
		perma_count->duplex = stPermaCount.duplex;
	}
	if((stPermaCount.copyprint > perma_count->copyprint) && (perma_count->copyprint > -1)) {
		db_logger_drumcount("copyprint", stPermaCount.copyprint, perma_count->copyprint);
		perma_count->copyprint = stPermaCount.copyprint;
	}
	if((stPermaCount.faxprint > perma_count->faxprint) && (perma_count->faxprint > -1)) {
		db_logger_drumcount("faxprint", stPermaCount.faxprint, perma_count->faxprint);
		perma_count->faxprint = stPermaCount.faxprint;
	}
	if((ptCount > *drum) && (*drum > -1)) { 
		db_logger_drumcount("drum", ptCount, *drum);
		*drum = ptCount;
	}
	if((ptCountInPercent > *countinpercent) && (*countinpercent > -1)) { 
		db_logger_drumcount("countInPercent", ptCountInPercent, *countinpercent);
		*countinpercent = ptCountInPercent;
	}
	if((ptMotor > *motor) && (*motor > -1)) { 
		db_logger_drumcount("motor", ptMotor, *motor);
		*motor = ptMotor;
	}
	if((ptDrumLife > *drumlife) && (*drumlife > -1)) { 
		db_logger_drumcount("drumlife", ptDrumLife, *drumlife);
		*drumlife = ptDrumLife;
	}

	return db_retval;
}

void db_logger_drumcount(char *CounterProperty, int stPermaCount, int perma_count)
{
	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_get_drumcount: Integrity fail (%s %d > %d)\n", CounterProperty, stPermaCount, perma_count);

	FILE *fpLog = NULL;
	fpLog = fopen(BOTHINTEGRITY_LOGFILENAME,"a");
	if(fpLog == NULL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_get_drumcount: Fail to open logfile %s\n", BOTHINTEGRITY_LOGFILENAME);
	}
	else
	{
		struct timeval currentTime;
		struct tm *ptm;
		gettimeofday(&currentTime, NULL);
		ptm = localtime(&currentTime.tv_sec);
		if(ptm == NULL)
		{
			fprintf(fpLog, "%04d-%02d-%02d   %02d:%02d\n", 1900, 1, 1, 0, 0);
		}
		else
		{
			fprintf(fpLog, "%04d-%02d-%02d   %02d:%02d\n", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
		}
		fprintf(fpLog, "Integrity fail (%s %d > %d)\n\n", CounterProperty, stPermaCount, perma_count);
		fclose(fpLog);
	}
}

#else
error_type_t db_get_drumcount(COUNTER_STRUCT *perma_count, int* drum, int* countinpercent, int* motor, int* drumlife)
{
	error_type_t db_retval = FAIL;
	char backupDB[1024];
	FILE *bkresult = NULL;
	COUNTER_STRUCT stPermaCount;
	int ptCount = 0;
	int ptCountInPercent = 0;
	int ptMotor = 0;
	int ptDrumLife = 0;

	// get point.db values
	memset(&stPermaCount, 0, sizeof(stPermaCount));
	memset(backupDB, 0, sizeof(backupDB));
	snprintf(backupDB,sizeof(backupDB),"sqlite3 /backup/point.db 'select perma_total,perma_host,perma_report,perma_usbdrive,perma_duplex,perma_copyprint,perma_faxprint,count,count_in_percent,motor,drum_life from permaset limit 1;'");
	bkresult=popen(backupDB,"r");
	if(bkresult == NULL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "sqlite3 cmd fail\n");
	}
	else
	{
		fscanf(bkresult,"%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d",&(stPermaCount.total),&(stPermaCount.host),&(stPermaCount.report),&(stPermaCount.usbdrive),&(stPermaCount.duplex),&(stPermaCount.copyprint),&(stPermaCount.faxprint),&ptCount,&ptCountInPercent,&ptMotor,&ptDrumLife);
		pclose(bkresult);
	}

	// get DB values
	perma_count->total = db_get_var_int("INST_MACHINECOUNT", "TotalPrint", &db_retval);
	perma_count->host = db_get_var_int("INST_MACHINECOUNT", "HostPrint", &db_retval);
	perma_count->report = db_get_var_int("INST_MACHINECOUNT", "ReportPrint", &db_retval);
	perma_count->usbdrive = db_get_var_int("INST_MACHINECOUNT", "USBDrivePrint", &db_retval);
	perma_count->duplex = db_get_var_int("INST_MACHINECOUNT", "DuplexPrint", &db_retval);
	perma_count->copyprint = db_get_var_int("INST_MACHINECOUNT", "CopyPrint", &db_retval);
	perma_count->faxprint = db_get_var_int("INST_MACHINECOUNT", "FaxPrint", &db_retval);
	(*drum) = db_get_var_int("INST_DRUMCOUNT", "Count", &db_retval);
	(*countinpercent) = db_get_var_int("INST_DRUMCOUNT", "CountInPercent", &db_retval);
	(*motor) = db_get_var_int("INST_DRUMCOUNT", "Motor", &db_retval);
	(*drumlife) = db_get_var_int("INST_DRUMCOUNT", "DrumLifePageCount", &db_retval);

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Count [Backup]: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d <-> [Original]: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", stPermaCount.total, stPermaCount.host, stPermaCount.report, stPermaCount.usbdrive, stPermaCount.duplex, stPermaCount.copyprint, stPermaCount.faxprint, ptCount, ptMotor, ptCountInPercent, ptDrumLife, perma_count->total, perma_count->host, perma_count->report, perma_count->usbdrive, perma_count->duplex, perma_count->copyprint, perma_count->faxprint, *drum, *motor, *countinpercent, *drumlife);

	if( (stPermaCount.total > perma_count->total) || (stPermaCount.host > perma_count->host) || (stPermaCount.report > perma_count->report) || (stPermaCount.usbdrive > perma_count->usbdrive) || (stPermaCount.duplex > perma_count->duplex) || (stPermaCount.copyprint > perma_count->copyprint) || (stPermaCount.faxprint > perma_count->faxprint) || (ptCount > *drum) || (ptCountInPercent > *countinpercent) || (ptMotor > *motor) || (ptDrumLife > *drumlife) )
	{
		FILE *fpLog = NULL;
		fpLog = fopen(BOTHINTEGRITY_LOGFILENAME,"wt");
		if(fpLog == NULL)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", BOTHINTEGRITY_LOGFILENAME);
			db_retval = FAIL;
		}
		else
		{
			fprintf(fpLog,"Backup: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d <-> Original: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", stPermaCount.total, stPermaCount.host, stPermaCount.report, stPermaCount.usbdrive, stPermaCount.duplex, stPermaCount.copyprint, stPermaCount.faxprint, ptCount, ptMotor, ptCountInPercent, ptDrumLife, perma_count->total, perma_count->host, perma_count->report, perma_count->usbdrive, perma_count->duplex, perma_count->copyprint, perma_count->faxprint, *drum, *motor, *countinpercent, *drumlife);
			fclose(fpLog);
		}
		printf("DB FATAL!!!!! ---: Backup: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d <-> Original: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", stPermaCount.total, stPermaCount.host, stPermaCount.report, stPermaCount.usbdrive, stPermaCount.duplex, stPermaCount.copyprint, stPermaCount.faxprint, ptCount, ptMotor, ptCountInPercent, ptDrumLife, perma_count->total, perma_count->host, perma_count->report, perma_count->usbdrive, perma_count->duplex, perma_count->copyprint, perma_count->faxprint, *drum, *motor, *countinpercent, *drumlife);

		perma_count->total = stPermaCount.total;
		perma_count->host = stPermaCount.host;
		perma_count->report = stPermaCount.report;
		perma_count->usbdrive = stPermaCount.usbdrive;
		perma_count->duplex = stPermaCount.duplex;
		perma_count->copyprint = stPermaCount.copyprint;
		perma_count->faxprint = stPermaCount.faxprint;
		*drum = ptCount;
		*countinpercent = ptCountInPercent;
		*motor = ptMotor;
		*drumlife = ptDrumLife;
	}

	return db_retval;
}
#endif

error_type_t db_set_fusercount(int Count, int ResetAction, int MotorNormal, int MotorLow, int FuserLifePageCount)
{
	int TempCount, TempResetAction, TempMotorNormal, TempMotorLow, TempFuserLifePageCount;
	error_type_t db_retval = FAIL;
	error_type_t temp_db_retval = FAIL;

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fusercount: Set Fuser Count...(%d,%d,%d,%d,%d)\n", Count, ResetAction, MotorNormal, MotorLow, FuserLifePageCount);

	if((db_retval = db_set_var_int("INST_FUSERCOUNT", "Count", Count)) == OK)
	if((db_retval = db_set_var_int("INST_FUSERCOUNT", "ResetAction", ResetAction)) == OK)
	if((db_retval = db_set_var_int("INST_FUSERCOUNT", "MotorNormal", MotorNormal)) == OK)
	if((db_retval = db_set_var_int("INST_FUSERCOUNT", "MotorLow", MotorLow)) == OK)
	if((db_retval = db_set_var_int("INST_FUSERCOUNT", "FuserLifePageCount", FuserLifePageCount)) == OK)
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fusercount: Set Fuser Count Success\n");

	if(db_retval == FAIL) {
		if((TempCount = db_get_var_int("INST_FUSERCOUNT", "Count", &temp_db_retval)) >= 0) 
		if((TempResetAction = db_get_var_int("INST_FUSERCOUNT", "ResetAction", &temp_db_retval)) >= 0)
		if((TempMotorNormal = db_get_var_int("INST_FUSERCOUNT", "MotorNormal", &temp_db_retval)) >= 0)
		if((TempMotorLow = db_get_var_int("INST_FUSERCOUNT", "MotorLow", &temp_db_retval)) >= 0)
		if((TempFuserLifePageCount = db_get_var_int("INST_FUSERCOUNT", "FuserLifePageCount", &temp_db_retval)) >= 0); /*Note the semicolon*/
		if(temp_db_retval == FAIL)
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fusercount: Get Current Fuser Count Fail\n"); /*If db_get_var_int() fail, retval is "-9999"*/

		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fusercount: Set Fuser Count Fail !!\n");
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fusercount: Intended values - Count(%d) ResetAction(%d) MotorNormal(%d) MotorLow(%d) FuserLifePageCount(%d)\n", Count, ResetAction, MotorNormal, MotorLow, FuserLifePageCount);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fusercount: Current values - Count(%d) ResetAction(%d) MotorNormal(%d) MotorLow(%d) FuserLifePageCount(%d)\n", TempCount, TempResetAction, TempMotorNormal, TempMotorLow, FuserLifePageCount);
		return db_retval;
	}
	else
		return db_retval;
}

error_type_t db_get_fusercount(int* Count, int* ResetAction, int* MotorNormal, int* MotorLow, int* FuserLifePageCount)
{
	error_type_t db_retval = FAIL;
	*Count 			= -9999;
	*ResetAction 	= -9999;
	*MotorNormal 	= -9999;
	*MotorLow 		= -9999;
	*FuserLifePageCount = -9999;

	if((*Count = db_get_var_int("INST_FUSERCOUNT", "Count", &db_retval)) >= 0) 
	if((*ResetAction = db_get_var_int("INST_FUSERCOUNT", "ResetAction", &db_retval)) >= 0)
	if((*MotorNormal = db_get_var_int("INST_FUSERCOUNT", "MotorNormal", &db_retval)) >= 0)
	if((*MotorLow = db_get_var_int("INST_FUSERCOUNT", "MotorLow", &db_retval)) >= 0)
	if((*FuserLifePageCount = db_get_var_int("INST_FUSERCOUNT", "FuserLifePageCount", &db_retval)) >= 0); /*Note the semicolon*/

	if(db_retval == FAIL) {
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_get_fusercount: Get Current Fuser Count Fail !!\n"); /*If db_get_var_int() fail, retval is "-9999"*/
		return db_retval;
	}

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_get_fusercount: Get Fuser Count Success (%d,%d,%d,%d,%d)\n", *Count, *ResetAction, *MotorNormal, *MotorLow, *FuserLifePageCount);
	return db_retval;
}

error_type_t db_set_fuserhistory(int UsedCount)
{	
	char query[QUERY_LENGTH];
    error_type_t db_retval = FAIL;
	int rowCount = 0;

    memset(query, 0x00, sizeof(query));
    snprintf(query, QUERY_LENGTH, "INSERT INTO RECD_FUSERHISTORY(UsedCount) VALUES ('%d');", UsedCount);
    DBG_PRINTF(LOG_INFO|DBG_NVRAM, "db_set_fuserhistory: %s\n", query);
    db_retval = db_trySQLiteExec(query);

	if(db_retval == FAIL) 
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fuserhistory: Set Fuser History Fail !!\n"); 
		return db_retval;
	}
	else 
	{	/*Inserting record success case. Check and delete the overflow record.*/
		rowCount = db_get_data_count("RECD_FUSERHISTORY", NULL, &db_retval);
		if(db_retval == FAIL) {
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fuserhistory: rowCount query is failed.\n");
			return db_retval;
		}

		while(rowCount > MAX_FUSERHISTORY_ROW)
		{
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "db_set_fuserhistory: The oldest log is deleted. Log count is %d\n", rowCount);

			memset(query, 0x00, sizeof(query));
			snprintf(query, QUERY_LENGTH, "DELETE FROM RECD_FUSERHISTORY WHERE rowid IN (SELECT rowid FROM RECD_FUSERHISTORY ORDER BY rowid ASC limit 1);");
			db_retval = db_trySQLiteExec(query);
			if(db_retval == FAIL) {
				DBG_PRINTF(LOG_WARNING|DBG_NVRAM, "db_set_fuserhistory: Fail to delete overflow record\n");
			}

			rowCount = db_get_data_count("RECD_FUSERHISTORY", NULL, &db_retval);
			if(db_retval == FAIL) {
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fuserhistory: rowCount query is failed.\n");
				sync();
				return db_retval;
			}
		}
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_fuserhistory: rowCount is %d.\n", rowCount);
	}
	
	sync();
    return db_retval;

}

error_type_t db_get_fuserhistory(char (*UsedCount)[8], int* row_count) 
{
	error_type_t db_retval = FAIL;
	char query[QUERY_LENGTH];
	char** queryResult = NULL;
	int queryRow, queryCol, indexRow;

	queryRow = 0;
	queryCol = 0;
	indexRow = 0;

	snprintf(query, sizeof(query), "SELECT * FROM RECD_FUSERHISTORY;");

	db_retval = db_get_table(query, &queryResult, &queryRow, &queryCol);
	if(db_retval == FAIL) {
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_get_fuserhistory: db_get_table() failed.\n");
		return db_retval; /*FAIL*/
	}
	
	/*To prevent record overflow case*/
	if(queryRow <= MAX_FUSERHISTORY_ROW)
		*row_count = queryRow;
	else
		*row_count = MAX_FUSERHISTORY_ROW;

	if(queryRow == 0) 	
	{	/*No Record*/
		snprintf(UsedCount[0], sizeof(UsedCount[0]), " ");
	}
	else
	{
		for(indexRow = 1; indexRow <= (*row_count); indexRow++) 
		{	/*First line, indexRow equal 0, is "Column Name"*/
			DBG_PRINTF(LOG_INFO|DBG_NVRAM, "db_get_fuserhistory: Used count[%d/%d]: %s \n", indexRow, *row_count, queryResult[indexRow*queryCol+1]);
			snprintf(UsedCount[indexRow-1], sizeof(UsedCount[indexRow-1]), "%s", queryResult[indexRow*queryCol+1]);
		}
	}
	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_get_fuserhistory: Get fuser history OK.\n");

	if(queryResult != NULL) 
		sqlite3_free_table(queryResult);

	return db_retval; /*OK*/
}

error_type_t db_get_scan_totalcount(SCAN_COUNTER_STRUCT* perma_count)
{
	SCAN_COUNTER_STRUCT stPermaCount;
	error_type_t db_retval = FAIL;
	FILE *bkresult = NULL;
	char backupDB[1024];
	memset(&stPermaCount, 0, sizeof(stPermaCount));
	memset(backupDB, 0, sizeof(backupDB));
	snprintf(backupDB,sizeof(backupDB),"sqlite3 /backup/point.db 'select perma_scan, perma_fbscan, perma_a3scan, perma_adfscan, perma_adfduplex from permaset limit 1;'");
	bkresult=popen(backupDB,"r");
	fscanf(bkresult,"%d|%d|%d|%d|%d",&(stPermaCount.scan),&(stPermaCount.fbscan),&(stPermaCount.a3scan),&(stPermaCount.adfscan),&(stPermaCount.adfduplex));
	pclose(bkresult);

	perma_count->scan = db_get_var_int("INST_MACHINECOUNT", "Scan", &db_retval);
	perma_count->fbscan = db_get_var_int("INST_MACHINECOUNT", "FBScan", &db_retval);
	perma_count->a3scan = db_get_var_int("INST_MACHINECOUNT", "A3Scan", &db_retval);
	perma_count->adfscan = db_get_var_int("INST_MACHINECOUNT", "ADFScan", &db_retval);
	perma_count->adfduplex = db_get_var_int("INST_MACHINECOUNT", "ADFDuplexScan", &db_retval);
	if(db_retval == FAIL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "DB Read Fail\n");
	}
	else
	{
		if(stPermaCount.scan > perma_count->scan || stPermaCount.fbscan > perma_count->fbscan || stPermaCount.a3scan > perma_count->a3scan || stPermaCount.adfscan > perma_count->adfscan || stPermaCount.adfduplex > perma_count->adfduplex )
		{
			FILE *fpLog = NULL;
			fpLog = fopen(BOTHINTEGRITY_SCAN_LOGFILENAME,"wt");
			if(fpLog == NULL)
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", BOTHINTEGRITY_SCAN_LOGFILENAME);
				db_retval = FAIL;
			}
			else
			{
				fprintf(fpLog,"Old: %d,%d,%d,%d,%d <-> New: %d,%d,%d,%d,%d\n", stPermaCount.scan, stPermaCount.fbscan, stPermaCount.a3scan, stPermaCount.adfscan, stPermaCount.adfduplex, perma_count->scan, perma_count->fbscan, perma_count->a3scan, perma_count->adfscan, perma_count->adfduplex);
				fclose(fpLog);
			}
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "DB FATAL!!!!! ---: Old: %d,%d,%d,%d,%d <-> New: %d,%d,%d,%d,%d\n", stPermaCount.scan, stPermaCount.fbscan, stPermaCount.a3scan, stPermaCount.adfscan, stPermaCount.adfduplex, perma_count->scan, perma_count->fbscan, perma_count->a3scan, perma_count->adfscan, perma_count->adfduplex);

			perma_count->scan = stPermaCount.scan;
			perma_count->fbscan = stPermaCount.fbscan;
			perma_count->a3scan = stPermaCount.a3scan;
			perma_count->adfscan = stPermaCount.adfscan;
			perma_count->adfduplex = stPermaCount.adfduplex;
		}
	}
	return db_retval;
}

error_type_t db_set_scancount(SCAN_COUNTER_STRUCT perma_count)
{
	error_type_t db_retval = FAIL;
	SCAN_COUNTER_STRUCT tempPermaCount;
	memset(&tempPermaCount, 0, sizeof(tempPermaCount));

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Set Scan Count...(%d,%d,%d,%d,%d)\n",perma_count.scan,perma_count.fbscan,perma_count.a3scan,perma_count.adfscan,perma_count.adfduplex);

	db_retval = db_get_scan_totalcount(&tempPermaCount);
	if(db_retval == FAIL)
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to get scan total count\n");

	if(tempPermaCount.scan > perma_count.scan || tempPermaCount.fbscan > perma_count.fbscan || tempPermaCount.a3scan > perma_count.a3scan || tempPermaCount.adfscan > perma_count.adfscan || tempPermaCount.adfduplex > perma_count.adfduplex)
	{
		FILE* fpLog = NULL;
		fpLog = fopen(PGINTEGRITY_SCAN_LOGFILENAME,"wt");
		if(fpLog == NULL)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", PGINTEGRITY_SCAN_LOGFILENAME);
			db_retval = FAIL;
		}
		else
		{
			fprintf(fpLog, "Backup: %d,%d,%d,%d,%d(...) <-> Original: %d,%d,%d,%d,%d\n", tempPermaCount.scan, tempPermaCount.fbscan, tempPermaCount.a3scan, tempPermaCount.adfscan, tempPermaCount.adfduplex, perma_count.scan, perma_count.fbscan, perma_count.a3scan, perma_count.adfscan, perma_count.adfduplex);
			fclose(fpLog);
		}
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "LOGIC FATAL!!!!! ---: Backup: %d,%d,%d,%d,%d(...) <-> Original: %d,%d,%d,%d,%d\n", tempPermaCount.scan, tempPermaCount.fbscan, tempPermaCount.a3scan, tempPermaCount.adfscan, tempPermaCount.adfduplex, perma_count.scan, perma_count.fbscan, perma_count.a3scan, perma_count.adfscan, perma_count.adfduplex);
	}

	// set values
	db_retval = db_trySQLiteExec("BEGIN TRANSACTION;");
	char countQuery[QUERY_LENGTH];
	memset(countQuery, 0, sizeof(countQuery));
	snprintf(countQuery, sizeof(countQuery), "UPDATE INST_MACHINECOUNT SET Scan=%d, FBScan=%d, A3Scan=%d, ADFScan=%d, ADFDuplexScan=%d;", perma_count.scan, perma_count.fbscan, perma_count.a3scan, perma_count.adfscan, perma_count.adfduplex);
	db_retval = db_trySQLiteExec(countQuery);
	if(db_retval == FAIL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Writing scan machine count fail\n");
	}
	db_retval = db_trySQLiteExec("COMMIT;");

	//sqlite3_open
	char backup_cmd[1024];
	memset(backup_cmd, 0, sizeof(backup_cmd));
	snprintf(backup_cmd,sizeof(backup_cmd),"sqlite3 /backup/point.db 'UPDATE permaset SET perma_scan=%d, perma_fbscan=%d, perma_a3scan=%d, perma_adfscan=%d, perma_adfduplex=%d'", perma_count.scan, perma_count.fbscan, perma_count.a3scan, perma_count.adfscan, perma_count.adfduplex);
	system(backup_cmd);

	return db_retval;
}

error_type_t db_get_scancount(SCAN_COUNTER_STRUCT *perma_count)
{
	error_type_t db_retval = FAIL;
	char backupDB[1024];
	FILE *bkresult = NULL;
	SCAN_COUNTER_STRUCT stPermaCount;

	// get point.db values
	memset(&stPermaCount, 0, sizeof(stPermaCount));
	memset(backupDB, 0, sizeof(backupDB));
	snprintf(backupDB,sizeof(backupDB),"sqlite3 /backup/point.db 'select perma_scan,perma_fbscan,perma_a3scan,perma_adfscan,perma_adfduplex from permaset limit 1;'");
	bkresult=popen(backupDB,"r");
	if(bkresult == NULL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "sqlite3 cmd fail\n");
	}
	else
	{
		fscanf(bkresult,"%d|%d|%d|%d|%d",&(stPermaCount.scan),&(stPermaCount.fbscan),&(stPermaCount.a3scan),&(stPermaCount.adfscan),&(stPermaCount.adfduplex));
		pclose(bkresult);
	}

	// get DB values
	perma_count->scan = db_get_var_int("INST_MACHINECOUNT", "Scan", &db_retval);
	perma_count->fbscan = db_get_var_int("INST_MACHINECOUNT", "FBScan", &db_retval);
	perma_count->a3scan = db_get_var_int("INST_MACHINECOUNT", "A3Scan", &db_retval);
	perma_count->adfscan = db_get_var_int("INST_MACHINECOUNT", "ADFScan", &db_retval);
	perma_count->adfduplex = db_get_var_int("INST_MACHINECOUNT", "ADFDuplexScan", &db_retval);

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Count [Backup]: %d,%d,%d,%d,%d <-> [Original]: %d,%d,%d,%d,%d\n", stPermaCount.scan, stPermaCount.fbscan, stPermaCount.a3scan, stPermaCount.adfscan, stPermaCount.adfduplex, perma_count->scan, perma_count->fbscan, perma_count->a3scan, perma_count->adfscan, perma_count->adfduplex);

	if( (stPermaCount.scan > perma_count->scan) || (stPermaCount.fbscan > perma_count->fbscan) || (stPermaCount.a3scan > perma_count->a3scan) || (stPermaCount.adfscan > perma_count->adfscan) || (stPermaCount.adfduplex > perma_count->adfduplex) )
	{
		FILE *fpLog = NULL;
		fpLog = fopen(BOTHINTEGRITY_SCAN_LOGFILENAME,"wt");
		if(fpLog == NULL)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", BOTHINTEGRITY_SCAN_LOGFILENAME);
			db_retval = FAIL;
		}
		else
		{
			fprintf(fpLog,"Backup: %d,%d,%d,%d,%d <-> Original: %d,%d,%d,%d,%d\n", stPermaCount.scan, stPermaCount.fbscan, stPermaCount.a3scan, stPermaCount.adfscan, stPermaCount.adfduplex, perma_count->scan, perma_count->fbscan, perma_count->a3scan, perma_count->adfscan, perma_count->adfduplex);
			fclose(fpLog);
		}
		printf("DB FATAL!!!!! ---: Backup: %d,%d,%d,%d,%d <-> Original: %d,%d,%d,%d,%d\n", stPermaCount.scan, stPermaCount.fbscan, stPermaCount.a3scan, stPermaCount.adfscan, stPermaCount.adfduplex, perma_count->scan, perma_count->fbscan, perma_count->a3scan, perma_count->adfscan, perma_count->adfduplex);

		perma_count->scan = stPermaCount.scan;
		perma_count->fbscan = stPermaCount.fbscan;
		perma_count->a3scan = stPermaCount.a3scan;
		perma_count->adfscan = stPermaCount.adfscan;
		perma_count->adfduplex = stPermaCount.adfduplex;
	}

	return db_retval;
}

error_type_t db_get_faxsend_totalcount(FAXSEND_COUNTER_STRUCT* perma_count)
{
	FAXSEND_COUNTER_STRUCT stPermaCount;
	error_type_t db_retval = FAIL;
	FILE *bkresult = NULL;
	char backupDB[1024];
	memset(&stPermaCount, 0, sizeof(stPermaCount));
	memset(backupDB, 0, sizeof(backupDB));
	snprintf(backupDB,sizeof(backupDB),"sqlite3 /backup/point.db 'select perma_faxsend from permaset limit 1;'");
	bkresult=popen(backupDB,"r");
	fscanf(bkresult,"%d",&(stPermaCount.faxsend));
	pclose(bkresult);

	perma_count->faxsend = db_get_var_int("INST_MACHINECOUNT", "FaxSend", &db_retval);
	if(db_retval == FAIL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "DB Read Fail\n");
	}
	else
	{
		if(stPermaCount.faxsend > perma_count->faxsend)
		{
			FILE *fpLog = NULL;
			fpLog = fopen(BOTHINTEGRITY_FAXSEND_LOGFILENAME,"wt");
			if(fpLog == NULL)
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", BOTHINTEGRITY_FAXSEND_LOGFILENAME);
				db_retval = FAIL;
			}
			else
			{
				fprintf(fpLog,"Old: %d <-> New: %d\n", stPermaCount.faxsend, perma_count->faxsend);
				fclose(fpLog);
			}
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "DB FATAL!!!!! ---: Old: %d <-> New: %d\n", stPermaCount.faxsend, perma_count->faxsend);

			perma_count->faxsend = stPermaCount.faxsend;
		}
	}
	return db_retval;
}

error_type_t db_set_faxsendcount(FAXSEND_COUNTER_STRUCT perma_count)
{
	error_type_t db_retval = FAIL;
	FAXSEND_COUNTER_STRUCT tempPermaCount;
	memset(&tempPermaCount, 0, sizeof(tempPermaCount));

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Set FaxSend Count...(%d)\n",perma_count.faxsend);

	db_retval = db_get_faxsend_totalcount(&tempPermaCount);
	if(db_retval == FAIL)
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to get faxsend total count\n");

	if(tempPermaCount.faxsend > perma_count.faxsend)
	{
		FILE* fpLog = NULL;
		fpLog = fopen(PGINTEGRITY_FAXSEND_LOGFILENAME,"wt");
		if(fpLog == NULL)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", PGINTEGRITY_FAXSEND_LOGFILENAME);
			db_retval = FAIL;
		}
		else
		{
			fprintf(fpLog, "Backup: %d(...) <-> Original: %d\n", tempPermaCount.faxsend, perma_count.faxsend);
			fclose(fpLog);
		}
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "LOGIC FATAL!!!!! ---: Backup: %d(...) <-> Original: %d\n", tempPermaCount.faxsend, perma_count.faxsend);
	}

	// set values
	db_retval = db_trySQLiteExec("BEGIN TRANSACTION;");
	char countQuery[QUERY_LENGTH];
	memset(countQuery, 0, sizeof(countQuery));
	snprintf(countQuery, sizeof(countQuery), "UPDATE INST_MACHINECOUNT SET FaxSend=%d;", perma_count.faxsend);
	db_retval = db_trySQLiteExec(countQuery);
	if(db_retval == FAIL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Writing faxsend machine count fail\n");
	}
	db_retval = db_trySQLiteExec("COMMIT;");

	//sqlite3_open
	char backup_cmd[1024];
	memset(backup_cmd, 0, sizeof(backup_cmd));
	snprintf(backup_cmd,sizeof(backup_cmd),"sqlite3 /backup/point.db 'UPDATE permaset SET perma_faxsend=%d'", perma_count.faxsend);
	system(backup_cmd);

	return db_retval;
}

error_type_t db_get_faxsendcount(FAXSEND_COUNTER_STRUCT *perma_count)
{
	error_type_t db_retval = FAIL;
	char backupDB[1024];
	FILE *bkresult = NULL;
	FAXSEND_COUNTER_STRUCT stPermaCount;

	// get point.db values
	memset(&stPermaCount, 0, sizeof(stPermaCount));
	memset(backupDB, 0, sizeof(backupDB));
	snprintf(backupDB,sizeof(backupDB),"sqlite3 /backup/point.db 'select perma_faxsend from permaset limit 1;'");
	bkresult=popen(backupDB,"r");
	if(bkresult == NULL)
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "sqlite3 cmd fail\n");
	}
	else
	{
		fscanf(bkresult,"%d",&(stPermaCount.faxsend));
		pclose(bkresult);
	}

	// get DB values
	perma_count->faxsend = db_get_var_int("INST_MACHINECOUNT", "FaxSend", &db_retval);

	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Count [Backup]: %d <-> [Original]: %d\n", stPermaCount.faxsend, perma_count->faxsend);

	if( (stPermaCount.faxsend > perma_count->faxsend) )
	{
		FILE *fpLog = NULL;
		fpLog = fopen(BOTHINTEGRITY_FAXSEND_LOGFILENAME,"wt");
		if(fpLog == NULL)
		{
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Fail to open logfile %s\n", BOTHINTEGRITY_FAXSEND_LOGFILENAME);
			db_retval = FAIL;
		}
		else
		{
			fprintf(fpLog,"Backup: %d <-> Original: %d\n", stPermaCount.faxsend, perma_count->faxsend);
			fclose(fpLog);
		}
		printf("DB FATAL!!!!! ---: Backup: %d <-> Original: %d\n", stPermaCount.faxsend, perma_count->faxsend);

		perma_count->faxsend = stPermaCount.faxsend;
	}

	return db_retval;
}

error_type_t db_set_var_for_TLI()
{
	int value = 0;
	error_type_t db_retval = FAIL;
	char* pBuffer = NULL;
	char machineTLI[30] = {0, };
	memset(machineTLI, 0, sizeof(machineTLI));
	pBuffer = db_get_var("INST_MACHINE", "MachineTLI", &db_retval);
	if(db_retval == OK)
	{
		if(pBuffer)
		{
			snprintf(machineTLI, sizeof(machineTLI), pBuffer);
			MEM_FREE_AND_NULL(pBuffer);
		}
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Machine TLI : %s\n", machineTLI);
		TLI_getCountry(machineTLI);
		TLI_getDefaultPaperType();
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI DB Read Fail\n");
	}

	value = db_get_var_int("CONF_SYSTEM", "Linguistic", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
			int default_value = 0;
			default_value = TLI_getDefaultLanguage();
			db_retval = db_set_var_int("CONF_SYSTEM", "Linguistic", default_value);
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI Linguistic DB Read Fail\n");
	}

//ABBA2, PINRETREE L은 자동검지가 안돼서 트레이별로 기본 용지를 지정해야 한다.
#if (PINETREE_L || ABBA2)
	value = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", &db_retval);
	if(db_retval == OK)
	{
		if(value == PAPERSIZE_DB_VALUE_AUTO)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = MEDIASIZE_LETTER;
			else
				default_value = MEDIASIZE_A4;
			db_retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", default_value);
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI TrayConfig_mediaSize_1 DB Read Fail\n");
	}
	value = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", &db_retval);
	if(db_retval == OK)
	{
		// ABBA2-H is not support to detect paper automatically.
		if(value == PAPERSIZE_DB_VALUE_AUTO)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = MEDIASIZE_LETTER;
			else
				default_value = MEDIASIZE_A4;
			db_retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", default_value);
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI TrayConfig_mediaSize_2 DB Read Fail\n");
	}
	value = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", &db_retval);
	if(db_retval == OK)
	{
		if(value == PAPERSIZE_DB_VALUE_AUTO)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = MEDIASIZE_LETTER;
			else
				default_value = MEDIASIZE_A4;
			db_retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", default_value);
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI TrayConfig_mediaSize_3 DB Read Fail\n");
	}
#endif
	value = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = MEDIASIZE_LETTER;
			else
				default_value = MEDIASIZE_A4;
			db_retval = db_set_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", default_value);
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI TrayConfig_mediaSize_0 DB Read Fail\n");
	}

	value = db_get_var_int("CONF_LSUPWM", "B1_PWM1", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
#if ABBA2
			if(IS_ABBA2H)
				db_retval = db_set_var_int("CONF_LSUPWM", "B1_PWM1", 80);
			else
				db_retval = db_set_var_int("CONF_LSUPWM", "B1_PWM1", 80);

			db_retval = db_set_var_int("CONF_COPYSET", "Darkness", 7); //oak2mh darkness default : 7
#else
			if(IS_PINETREE)
				db_retval = db_set_var_int("CONF_LSUPWM", "B1_PWM1", 73);
			else
				db_retval = db_set_var_int("CONF_LSUPWM", "B1_PWM1", 75);
#endif
#if JUNIPER
			if(IS_JUNIPERC || IS_JUNIPERS){
				db_retval = db_set_var_int("CONF_LSUPWM", "B1_PWM1", 80);
			}
			db_retval = db_set_var_int("CONF_SCANSP", "CopyEraseMargin", 30); // juniper copyerasemargin default : 12 (-> 30 only in juniper 151202)
#endif
#if PINETREE_L 
			db_retval = db_set_var_int("CONF_LSUPWM", "B1_PWM1", 73);
#endif
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI B1_PWM1 DB Read Fail\n");
	}

	value = db_get_var_int("CONF_SYSTEMSP", "ServoAdjust", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
#if ABBA2
			if(IS_ABBA2H)
				db_retval = db_set_var_int("CONF_SYSTEMSP", "ServoAdjust", 60);
			else
				db_retval = db_set_var_int("CONF_SYSTEMSP", "ServoAdjust", 58);
#else
			if(IS_PINETREE)
				db_retval = db_set_var_int("CONF_SYSTEMSP", "ServoAdjust", 58);
			else
				db_retval = db_set_var_int("CONF_SYSTEMSP", "ServoAdjust", 58);
#endif
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI ServoAdjust DB Read Fail\n");
	}

	value = db_get_var_int("CONF_COPYSET", "OriginalSize", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = MEDIASIZE_LETTER;
			else
				default_value = MEDIASIZE_A4;
			db_retval = db_set_var_int("CONF_COPYSET", "OriginalSize", default_value);
#if JUNIPER
			db_retval = db_set_var_int("CONF_COPYSET", "OriginalSize", PAPERSIZE_DB_VALUE_AUTO);
#endif
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI Copy OriginalSize DB Read Fail\n");
	}

	value = db_get_var_int("CONF_SCANSET", "OriginalSize", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = MEDIASIZE_LETTER;
			else
				default_value = MEDIASIZE_A4;
			db_retval = db_set_var_int("CONF_SCANSET", "OriginalSize", default_value);
#if JUNIPER
			db_retval = db_set_var_int("CONF_SCANSET", "OriginalSize", PAPERSIZE_DB_VALUE_AUTO);
#endif
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI Scan OriginalSize DB Read Fail\n");
	}

	value = db_get_var_int("CONF_FAXUP", "TxOriginalSize", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = MEDIASIZE_LETTER;
			else
				default_value = MEDIASIZE_A4;
			db_retval = db_set_var_int("CONF_FAXUP", "TxOriginalSize", default_value);
#if JUNIPER
			db_retval = db_set_var_int("CONF_FAXUP", "TxOriginalSize", PAPERSIZE_DB_VALUE_AUTO);
#endif

		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI Fax TxOriginalSize DB Read Fail\n");
	}

	value = db_get_var_int("CONF_FAXSP", "SelectCountry", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
			int default_value = 0;
			default_value = TLI_getDefaultFaxCountryCode();
			db_retval = db_set_var_int("CONF_FAXSP", "SelectCountry", default_value);
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI Fax DB Read Fail\n");
	}

#define SELECT_A3_DEFAULT     0
#define SELECT_1117_DEFAULT   1

	value = db_get_var_int("CONF_PRNT", "SELECT_A31117", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = SELECT_1117_DEFAULT;
			else
				default_value = SELECT_A3_DEFAULT;
			db_retval = db_set_var_int("CONF_PRNT", "SELECT_A31117", default_value);
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI Select A3/11x17 DB Read Fail\n");
	}

#define SP_DISABLE 0
#define SP_ENABLE 1

	value = db_get_var_int("CONF_SYSTEMSP", "LegalMerge", &db_retval);
	if(db_retval == OK)
	{
		if(value == UNKNOWN_DB_VALUE)
		{
			int default_value = 0;
			if(gPaperType == LT_BASE)
				default_value = SP_ENABLE;
			else
				default_value = SP_DISABLE;
			db_retval = db_set_var_int("CONF_SYSTEMSP", "LegalMerge", default_value);
		}
	}
	else
	{
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "TLI Select LegalMerge DB Read Fail\n");
	}

	return OK;
}

#define MINIMUM_TLI_LENGTH 6
#define GENERAL_TLI_LENGTH 8

#define CONTURY_CODE_1 4
#define CONTURY_CODE_2 5
#define CONTURY_CODE_EX 9

int TLI_getCountry(char * machineTLI)
{
	if(strlen(machineTLI) < MINIMUM_TLI_LENGTH)
	{
		if(strlen(machineTLI) > 0)
		   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Wrong TLI : %s\n", machineTLI);
		else
		   DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Wrong TLI : (NULL)\n");
		gCountry = IS_UNKNOWN;
	}
	else
	{
		if(machineTLI[CONTURY_CODE_1] == 'D' && machineTLI[CONTURY_CODE_2] == 'N')
		{
			if(strlen(machineTLI) > GENERAL_TLI_LENGTH)
			{
				if(machineTLI[CONTURY_CODE_EX] == 'C') {
					gCountry = IS_CHINA;
				}
				else {
					gCountry = IS_KOREA;
				}
			}
			else
			{
				gCountry = IS_KOREA;
			}
			
			if(machineTLI[CONTURY_CODE_2+1] == 'I')//for A419DNI36 (2USB)
			{
				gCountry = IS_KOREA;
			}
		}
		else if(machineTLI[CONTURY_CODE_1] == 'N' && machineTLI[CONTURY_CODE_2] == 'A')
		{
			gCountry = IS_NORTH_AMERICA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'B' && machineTLI[CONTURY_CODE_2] == 'R')
		{
			gCountry = IS_BRAZIL;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'T' && machineTLI[CONTURY_CODE_2] == 'R')
		{
			gCountry = IS_TURKEY;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'Z' && machineTLI[CONTURY_CODE_2] == 'A')
		{
			gCountry = IS_SOUTH_AFRICA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'T' && machineTLI[CONTURY_CODE_2] == 'N')
		{
			gCountry = IS_TUNISIA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'A' && machineTLI[CONTURY_CODE_2] == 'E')
		{
			gCountry = IS_ARAB_EMIRATES;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'I' && machineTLI[CONTURY_CODE_2] == 'R')
		{
			gCountry = IS_IRAN;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'R' && machineTLI[CONTURY_CODE_2] == 'U')
		{
			gCountry = IS_RUSSIA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'I' && machineTLI[CONTURY_CODE_2] == 'N')
		{
			gCountry = IS_INDIA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'M' && machineTLI[CONTURY_CODE_2] == 'Y')
		{
			gCountry = IS_MALAYSIA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'S' && machineTLI[CONTURY_CODE_2] == 'A')
		{
			gCountry = IS_SAUDI_ARABIA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'A' && machineTLI[CONTURY_CODE_2] == 'U')
		{
			gCountry = IS_AUSTRALIA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'A' && machineTLI[CONTURY_CODE_2] == 'R')
		{
			gCountry = IS_ARGENTINA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'C' && machineTLI[CONTURY_CODE_2] == 'O')
		{
			gCountry = IS_COLOMBIA;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'E' && machineTLI[CONTURY_CODE_2] == 'S')
		{
			gCountry = IS_SPAIN;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'P' && machineTLI[CONTURY_CODE_2] == 'Y')
		{
			gCountry = IS_PARAGUAY;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'M' && machineTLI[CONTURY_CODE_2] == 'X')
		{
			gCountry = IS_MEXICO;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'P' && machineTLI[CONTURY_CODE_2] == 'E')
		{
			gCountry = IS_PERU;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'C' && machineTLI[CONTURY_CODE_2] == 'L')
		{
			gCountry = IS_CHILE;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'L' && machineTLI[CONTURY_CODE_2] == 'V')
		{
			gCountry = IS_ABROAD;
		}
		else if(machineTLI[CONTURY_CODE_1] == 'H' && machineTLI[CONTURY_CODE_2] == 'V')
		{
			gCountry = IS_ABROAD;
		}
		else if(strcmp(machineTLI,"M61245HH") == 0) //HANA BANK SOLUTION: M61245HH A611DN45HH A611DN45HHU
		{
			gCountry = IS_KOREA;
		}
		else if ((strcmp(machineTLI,"P247B01") == 0) || (strcmp(machineTLI,"M247B01") == 0))
		{
			printf("%s() OEM TLI found! set machine to russia\n", __func__);
			gCountry = IS_RUSSIA;
		}
		else
		{
			if(strlen(machineTLI) < GENERAL_TLI_LENGTH)
			{
				gCountry = IS_KOREA;
			}
			else if(strcmp(machineTLI,"N41130") == 0 || strcmp(machineTLI,"N41026") == 0 || strcmp(machineTLI,"MF303130") == 0 || strcmp(machineTLI,"MF208126") == 0)
			{
				printf("[nvram_db_shim] TLI check \t TLI: %s, length: %d\n", machineTLI, strlen(machineTLI));
				gCountry = IS_KOREA;
			}
			else
			{
				gCountry = IS_UNKNOWN;
			}
		}
	}

	printf("COUNTRY CODE : %d\n",gCountry);

	return 0;
}

int TLI_getDefaultLanguage()
{
	int retValue = 0;

	switch(gCountry)
	{
		case IS_KOREA:
		{
			retValue = LANGUAGE_KOREAN;
			break;
		}
		case IS_CHINA:
		{
			retValue = LANGUAGE_CHINESE;
			break;
		}
		case IS_BRAZIL:
		{
			retValue = LANGUAGE_PORTUGUESE;
			break;
		}
		case IS_TUNISIA:
		{
			retValue = LANGUAGE_FRANCH;
			break;
		}
		case IS_RUSSIA:
		{
			retValue = LANGUAGE_RUSSIAN;
			break;
		}
		case IS_ARGENTINA:
		case IS_COLOMBIA:
		case IS_SPAIN:
		case IS_PARAGUAY:
		case IS_MEXICO:
		case IS_PERU:
		case IS_CHILE:
		{
			retValue = LANGUAGE_SPANISH;
			break;
		}
		case IS_ABROAD:
		{
			error_type_t db_retval = FAIL;
			if ( IS_KARAS || IS_ABBA2H ) { // for Printer
				retValue = db_get_var_int("CONF_SYSTEMSP", "initialMachineLinguistic", &db_retval);
				if(db_retval != OK)
				{
					retValue = LANGUAGE_ENGLISH;
					DBG_PRINTF(LOG_ERR|DBG_NVRAM, "initialMachineLinguistic DB Read Fail\n");
				}
			}
			else { // for Copier
				uint32_t CountryCode;
				CountryCode = db_get_var_int("CONF_SYSTEMSP", "InitialMachineCountryCode", &db_retval);
				if(db_retval == OK)
				{
					if ( CountryCode < MAX_COUNTRY_CODE ) {
						retValue = CountryTable[CountryCode].language;
					}
				}
				else
				{
					DBG_PRINTF(LOG_ERR|DBG_NVRAM, "InitialMachineCountryCode DB Read Fail\n");
				}
			}
			break;
		}
		default:
		{
			retValue = LANGUAGE_ENGLISH;
			break;
		}
	}

	return retValue;
}

void TLI_getDefaultPaperType()
{
	switch(gCountry)
	{
		case IS_NORTH_AMERICA:
		case IS_COLOMBIA:
		case IS_PARAGUAY:
		case IS_MEXICO:
		case IS_CHILE:
		{
			gPaperType = LT_BASE;
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Base Paper Type : LT\n");
			break;
		}
		case IS_ABROAD:
		{
			error_type_t db_retval = FAIL;
			if ( IS_KARAS || IS_ABBA2H ) { // for Printer
				gPaperType = db_get_var_int("CONF_SYSTEMSP", "initialMachinePaperType", &db_retval);
				if(db_retval != OK)
				{
					gPaperType = A4_BASE;
					DBG_PRINTF(LOG_ERR|DBG_NVRAM, "initialMachinePaperType DB Read Fail\n");
				}
			}
			else { // for Copier
				uint32_t CountryCode;
				CountryCode = db_get_var_int("CONF_SYSTEMSP", "InitialMachineCountryCode", &db_retval);
				if(db_retval == OK)
				{
					if ( CountryCode < MAX_COUNTRY_CODE ) {
						gPaperType = CountryTable[CountryCode].paperType;
					}
				}
				else
				{
					DBG_PRINTF(LOG_ERR|DBG_NVRAM, "InitialMachineCountryCode DB Read Fail\n");
				}
			}
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Base Paper Type : %d\n", gPaperType);
			break;
		}
		default:
		{
			gPaperType = A4_BASE;
			DBG_PRINTF(LOG_ERR|DBG_NVRAM, "Base Paper Type : A4\n");
			break;
		}
	}

	return;
}

int TLI_getDefaultFaxCountryCode()
{
	int retValue = 0;

	switch(gCountry)
	{
		case IS_CHINA:
		{
			retValue = T35_CHINA;
			break;
		}
		case IS_NORTH_AMERICA:
		{
			retValue = T35_UNITED_STATES; // US, Canada, Mexico
			break;
		}
		case IS_BRAZIL:
		{
			retValue = T35_BRAZIL;
			break;
		}
		case IS_TURKEY:
		{
			retValue = T35_TURKEY;
			break;
		}
		case IS_SOUTH_AFRICA:
		{
			retValue = T35_SOUTH_AFRICA;
			break;
		}
		case IS_TUNISIA:
		{
			retValue = T35_TUNISIA;
			break;
		}
		case IS_ARAB_EMIRATES:
		{
			retValue = T35_UNITED_ARAB_EMIRATES;
			break;
		}
		case IS_IRAN:
		{
			retValue = T35_IRAN;
			break;
		}
		case IS_RUSSIA:
		{
			retValue = T35_RUSSIAN_FEDERATION;
			break;
		}
		case IS_INDIA:
		{
			retValue = T35_INDIA;
			break;
		}
		case IS_MALAYSIA:
		{
			retValue = T35_MALAYSIA;
			break;
		}
		case IS_SAUDI_ARABIA:
		{
			retValue = T35_SAUDI_ARABIA;
			break;
		}
		case IS_AUSTRALIA:
		{
			retValue = T35_AUSTRALIA;
			break;
		}
		case IS_ARGENTINA:
		{
			retValue = T35_ARGENTINA;
			break;
		}
		case IS_COLOMBIA:
		{
			retValue = T35_COLOMBIA;
			break;
		}
		case IS_SPAIN:
		{
			retValue = T35_SPAIN;
			break;
		}
		case IS_PARAGUAY:
		{
			retValue = T35_PARAGUAY;
			break;
		}
		case IS_MEXICO:
		{
			retValue = T35_MEXICO;
			break;
		}
		case IS_PERU:
		{
			retValue = T35_PERU;
			break;
		}
		case IS_CHILE:
		{
			retValue = T35_CHILE;
			break;
		}
		case IS_ABROAD:
		{
			error_type_t db_retval = FAIL;
			uint32_t CountryCode;
			CountryCode = db_get_var_int("CONF_SYSTEMSP", "InitialMachineCountryCode", &db_retval);
			if(db_retval == OK)
			{
				if ( CountryCode < MAX_COUNTRY_CODE ) {
					retValue = CountryTable[CountryCode].faxCode;
				}
			}
			else
			{
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "InitialMachineCountryCode DB Read Fail\n");
			}
			break;
		}
		default:
		{
			retValue = T35_KOREA;
			break;
		}
	}

	return retValue;
}

void CheckDBReturnVal(error_type_t db_retval, int line)
{
	if(db_retval)	DBG_PRINTF(LOG_ERR|DBG_NVRAM, "NVRAMAPI(%d): SQL Fail\n", line);
}

#define TIMEZONE_CHINA		"CST-08:00"
#define TIMEZONENO_CHINA	8
#define SLEEP_MIN   240
error_type_t db_set_var_for_solution()
{
    int value = 0;
    
	char CmdStr[QUERY_LENGTH];
	int SolutionDefaultSet = 0;
	int CompanyCode = 0;
	error_type_t db_retval = FAIL;

	SolutionDefaultSet = db_get_var_int("CONF_SOLUTIONUP", "SolutionDefaultValueSet", &db_retval);	CheckDBReturnVal(db_retval, __LINE__);
	if(SolutionDefaultSet == 1)
		/*Default Set Already Complete*/
		return OK;
	else
	{	/*Solution default setting start*/
		CompanyCode = db_get_var_int("CONF_SOLUTIONSP", "CompanyCode", &db_retval);						CheckDBReturnVal(db_retval, __LINE__);
		DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_var_for_solution: Solution default setting - CompanyCode(%d)\n", CompanyCode);
		switch (CompanyCode) {
			case 1: //Caire (China)
				db_retval = db_set_var("CONF_SYSTEM", "TimeZone", TIMEZONE_CHINA);	CheckDBReturnVal(db_retval, __LINE__);
				db_retval = db_set_var_int("CONF_SYSTEM", "TimeZoneNo", TIMEZONENO_CHINA);	CheckDBReturnVal(db_retval, __LINE__);
				db_retval = db_set_var_int("CONF_SYSTEM", "Linguistic", LANGUAGE_ENGLISH);	CheckDBReturnVal(db_retval, __LINE__);
				db_retval = db_set_var_int("CONF_SYSTEMSP", "initialMachineLinguistic", LANGUAGE_ENGLISH);	CheckDBReturnVal(db_retval, __LINE__);
				memset(CmdStr, 0x00, sizeof(CmdStr));
				snprintf(CmdStr, sizeof(CmdStr)-1, ". /app/karas_pinetree_util.sh --tz-change-with-arg %s", TIMEZONE_CHINA);
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_var_for_solution: TimeZone(%s) / TimeZoneNo(%d) / Linguistic(%d) / initialMachineLinguistic(%d)\n", TIMEZONE_CHINA, TIMEZONENO_CHINA, LANGUAGE_ENGLISH, LANGUAGE_ENGLISH);
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_var_for_solution: '%s'\n", CmdStr);
				system(CmdStr);
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_var_for_solution: Setting complete(%d)\n", CompanyCode);
				break;
			case 2: //Supreme Prosecutors' Office
				db_retval = db_set_var_int("CONF_SYSTEM", "SleepMin", SLEEP_MIN);	CheckDBReturnVal(db_retval, __LINE__);
				memset(CmdStr, 0x00, sizeof(CmdStr));
				value = db_get_var_int("CONF_SYSTEM", "SleepMin", &db_retval);	CheckDBReturnVal(db_retval, __LINE__);
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_var_for_solution: SleepMin(%d)\n", value);
				DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_set_var_for_solution: Setting complete(%d)\n", CompanyCode);
				break;
			default:
				break;
		}

		/*Default Set Complete*/
		db_retval = db_set_var_int("CONF_SOLUTIONUP", "SolutionDefaultValueSet", 1);	CheckDBReturnVal(db_retval, __LINE__);
		return OK;
	}/*Solution default setting end*/
}

#define PROBLEM_DB_VERSION 222
error_type_t db_critical_bug_check(int db_version) {
    int value = 0;
    char* pBuffer = NULL;
	error_type_t db_retval = FAIL;
	
	/* Check QuantityLimit Bug */
	// branches/PINETREES_170623_3.2USB.NTS rev.31530, 31532
    value = db_get_var_int("INST_MACHINE", "DBVERSION", &db_retval);	CheckDBReturnVal(db_retval, __LINE__);
    
    if(value >= PROBLEM_DB_VERSION || db_version >= PROBLEM_DB_VERSION)
    {
        DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_critical_bug_check: Run rollback code!! db_version(%d) updated_version(%d)\n", value, db_version);
        pBuffer = db_get_var("CONF_SOLUTIONSP", "CompanyCode", &db_retval);	CheckDBReturnVal(db_retval, __LINE__);
        if(pBuffer == NULL || db_retval == FAIL)
        {
            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_critical_bug_check: Add column: CompanyCode, SolutionDefaultValueSet\n");
            db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONSP\" ADD COLUMN \"CompanyCode\" INTEGER DEFAULT 0 NOT NULL;");
            db_retval = db_trySQLiteExec("ALTER TABLE \"CONF_SOLUTIONUP\" ADD COLUMN \"SolutionDefaultValueSet\" INTEGER DEFAULT 0 NOT NULL;");

            DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_critical_bug_check: Update default db value SleepMin=-1\n");
            db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=-1 WHERE NO=1");
            db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=-1 WHERE NO=2");
            db_retval = db_trySQLiteExec("UPDATE CONF_SYSTEM SET SleepMin=-1 WHERE NO=3");
        }

        if(pBuffer != NULL)
            MEM_FREE_AND_NULL(pBuffer);
    }
    DBG_PRINTF(LOG_ERR|DBG_NVRAM, "db_critical_bug_check: Nothing to do. db_version(%d) updated_version(%d)\n", value, db_version);

	return OK;
}
//END
