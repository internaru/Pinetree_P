#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include "sys_init_api.h"

#define SYS_SHARED_MEMORY_H_MAIN
#include "../../../../../common_header/SysSharedMemory.h"
#undef SYS_SHARED_MEMORY_H_MAIN

#define DBG_PRFX "SYS_SHARED_MEMORY: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 12 )  //Warning: temp mask. Change it to proper MASK
  
#if 1   
#define DBG_SSL_PRINTF_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#else
#define DBG_SSL_PRINTF_ERR(...) DBG_PRINTF(LOG_INFO, DBG_PRFX __VA_ARGS__)
#endif

void *shm_create( int key, int size ) {

	DBG_SSL_PRINTF_ERR("%s line%d-> key:%d, size:%d\n", __func__, __LINE__, key, size);

	if ( key < SHM_KEY_BASE || key >= SHM_KEY_MAX || size <= 0 ) {
		DBG_SSL_PRINTF_ERR("%s invalid argument-> key: %d, size: %d\n", __func__, key, size);
		return (void *)-1;
	}

	int key_offset = key - SHM_KEY_BASE;
	shm_id[key_offset] = shmget((key_t)key, size, 0666|IPC_CREAT);

	if (shm_id[key_offset] == -1) {
		DBG_SSL_PRINTF_ERR("shmget error: %d, %s\n", errno, strerror(errno));
		return (void *)-1;
	}

	void *ptr;
	ptr = shm_attach(shm_id[key_offset]);
	if ( ptr == (void *)-1 ) {
		DBG_SSL_PRINTF_ERR("shm_attach error\n");
		return (void *)-1;
	}
	return ptr;
}

void *shm_attach(int shmid)
{
	void *ptr;
	if ( shmid < 0 ) {
		return (void *)-1;
	}
	ptr = shmat(shmid, (void *)0, 0);
	if ( ptr == (void *)-1 ) {
		DBG_SSL_PRINTF_ERR("shmat error: %d, %s\n", errno, strerror(errno));
		return (void *)-1;
	}
	return ptr;
}

int shm_detach(void *ptr)
{
	int ret;
	if ( ptr == NULL ) {
		return -1;
	}
	ret = shmdt((const void *)ptr);
	if( ret < 0 ) {
		DBG_SSL_PRINTF_ERR("shmdt error: %d, %s\n", errno, strerror(errno));
	}
	return ret;
}

int shm_remove(int shmid)
{
	int ret;
	if ( shmid <= 0 ) {
		return -1;
	}
	ret = shmctl(shmid, IPC_RMID, NULL);
	if( ret < 0 ) {
		DBG_SSL_PRINTF_ERR("shmctl(RMID) error: %d, %s\n", errno, strerror(errno));
	}
	return ret;
}

int shm_get_keyid(int key)
{
	if ( key < SHM_KEY_BASE || key >= SHM_KEY_MAX ) {
		DBG_SSL_PRINTF_ERR("%s invalid argument-> key: %d\n", __func__, key);
		return -1;
	}
	return shm_id[key-SHM_KEY_BASE];
}
