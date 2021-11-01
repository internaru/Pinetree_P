/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifdef HAVE_UIO_LIB
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "lassert.h"
#include "logger.h"
#include "pthread.h"
#include "posix_ostools.h"
#include "ATypes.h"
#include "uio_lib_api.h"

#define DBG_PRFX "UIO_LIB: " 
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | LOGGER_SUBMODULE_BIT( 15 )

#define DBG_ERR DBG_PRINTF_ERR
#define DBG_DEBUG DBG_PRINTF_DEBUG

pthread_mutex_t uio_lock = PTHREAD_MUTEX_INITIALIZER;
#ifdef DEBUG
   #define UIO_GLOBAL_LOCK() DBG_XASSERT( 0 == pthread_mutex_lock( &uio_lock ), errno )
   #define UIO_GLOBAL_UNLOCK() DBG_XASSERT( 0 == pthread_mutex_unlock( &uio_lock ), errno )
#else
   #if 1
      #define UIO_GLOBAL_LOCK() pthread_mutex_lock( &uio_lock )
      #define UIO_GLOBAL_UNLOCK() pthread_mutex_unlock( &uio_lock )
   #else
      #define UIO_GLOBAL_LOCK() REL_XASSERT( 0 == pthread_mutex_lock( &uio_lock ), errno )
      #define UIO_GLOBAL_UNLOCK() REL_XASSERT( 0 == pthread_mutex_unlock( &uio_lock ), errno )
   #endif
#endif

static int uio_epfd = -1;

typedef struct _uio_map_priv_s {
    uio_map_t m;
    
    /* link list - NULL terminated*/
	struct _uio_map_priv_s *next; 
} uio_map_priv_t;

struct _uio_dev_s {
	int             num;            /*!< The number of the UIO device [/dev/uio{num}] */
    char*           name;           /*!< The name of the device. Typically this is the name 
                                         of the kernel module. */
	char*   version;                /*!< A version string defined in the driver. This allows 
                                         the user space part of the driver to deal with different 
                                         versions of the kernel module. */
    
    int                     fd;
    UIO_PFN_ISR             handler;
    void*                   context;
    uint32_t                event_priority;
    uint32_t                event_flags;
    
	struct _uio_map_priv_s  *maps; 
};


char* uio_get_string_from_file(char *pathname)
{
	char *s;
	FILE* f;
    char buf[64];

    // open file
    f = fopen(pathname,"r");
	if (!f) 
        return NULL;
        
    // get string
    s = fgets(buf, sizeof(buf), f);
    if (!s)
    {
        fclose(f);
        return NULL;
    }
    
    // replace any newlines with NULL
    int slen = strlen(s);
    int i;
    for (i=0; (*s) && (i < slen); i++) 
    {
        if (*s == '\n') 
            *s = 0;
            
        s++;
    }
    fclose(f);

    return strdup(buf);
}

char* uio_get_device_name(int dev_num)
{
	char pathname[64];
	snprintf(pathname, sizeof(pathname), "/sys/class/uio/uio%d/name", dev_num);

	return uio_get_string_from_file(pathname);
}

char* uio_get_device_version(int dev_num)
{
	char pathname[64];
	snprintf(pathname, sizeof(pathname), "/sys/class/uio/uio%d/version", dev_num);

	return uio_get_string_from_file(pathname);
}

char* uio_get_map_name(int dev_num, int map_num)
{
	char pathname[64];
	snprintf(pathname, sizeof(pathname), "/sys/class/uio/uio%d/maps/map%d/name", dev_num, map_num);

	return uio_get_string_from_file(pathname);
}

unsigned long uio_get_map_addr(int dev_num, int map_num)
{
    unsigned long ret = 0;
	char pathname[64];
    char *s;
    
	snprintf(pathname, sizeof(pathname), "/sys/class/uio/uio%d/maps/map%d/addr", dev_num, map_num);
    s = uio_get_string_from_file(pathname);
    if (s != NULL)
    {
        ret = strtoul(s, NULL, 0);
        free(s);
    }
    
	return ret;
}

unsigned long uio_get_map_size(int dev_num, int map_num)
{
    unsigned long ret = 0;
	char pathname[64];
    char *s;
    
	snprintf(pathname, sizeof(pathname), "/sys/class/uio/uio%d/maps/map%d/size", dev_num, map_num);
    s = uio_get_string_from_file(pathname);
    if (s != NULL)
    {
        ret = strtoul(s, NULL, 0);
        free(s);
    }
    
	return ret;
}

uio_map_priv_t* uio_get_device_map(int dev_num, int map_num)
{
    uio_map_priv_t *map = NULL;
    struct stat st;
    char pathname[64];
    
    snprintf(pathname, sizeof(pathname), "/sys/class/uio/uio%d/maps/map%d", dev_num, map_num);
    if(stat(pathname, &st) == 0)
    {
        map = malloc(sizeof(uio_map_priv_t));
        if (map != NULL)
        {
            memset(map, 0, sizeof(uio_map_priv_t));
            
            map->m.num = map_num;
            map->m.name = uio_get_map_name(dev_num, map_num);
            map->m.phy_addr = uio_get_map_addr(dev_num, map_num);
            map->m.virt_addr = MAP_FAILED;
            map->m.size = uio_get_map_size(dev_num, map_num);
        }
    }
    
    return map;
}

uio_map_priv_t* uio_get_device_maps(int dev_num)
{
    uio_map_priv_t *head = NULL;
    uio_map_priv_t *prev = NULL;
    int i = 0;
    
    uio_map_priv_t *map;
    while( (map = uio_get_device_map(dev_num, i++)) != NULL)
    {
        if (head == NULL)
        {
            head = map;
        }
        else
        {
            prev->next = map;
        }
        
        prev = map;
    }

    return head;
}

int uio_open_map(uio_map_priv_t* map, int fd)
{
	int64_t offset;
    char *p_addr;
    
    if (map == NULL)
        return -1;
        
    if (fd < 0)
        return -2;
        
    if (map->m.virt_addr != MAP_FAILED)
        return -3;
        
    p_addr = mmap( NULL, map->m.size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
    if (p_addr == MAP_FAILED)
    {
        DBG_ERR("%s: FAILED err = %s\n", __func__, strerror(errno));
        return -4;
    }


    // mmap returns memory mapped on a page boundary; 
    // need to offset to actual register start location in the page
	offset = ((int64_t) map->m.phy_addr & (getpagesize() - 1));
	map->m.virt_addr = (void *)(p_addr + offset);
    
    return 0;
}

void uio_close_map(uio_map_priv_t* map)
{
    if (map == NULL)
        return;
        
    if (map->m.virt_addr != MAP_FAILED) 
    {
        if (munmap(map->m.virt_addr, map->m.size) == -1) 
        {
            DBG_ERR("Error removing the mapping!");
        }
        
        map->m.virt_addr = MAP_FAILED;
    }
}

void uio_free_map(uio_map_priv_t* map)
{
    if (map == NULL)
        return;
        
    if (map->m.name != NULL)
        free(map->m.name);
        
    free(map);
}

void uio_free_dev(uio_dev_t *dev)
{
    if (dev == NULL)
        return;
    
    uio_map_priv_t* map = dev->maps;
    while (map != NULL)
    {
        uio_map_priv_t* next = map->next;
        
        uio_free_map(map);
        map = next;
    }

    if (dev->name != NULL)
        free(dev->name);

    if (dev->version != NULL)
        free(dev->version);
        
    free (dev);
}

uio_dev_t* uio_locate_dev_by_number(int dev_num)
{
    uio_dev_t *dev = NULL;
	char pathname[64];
    struct stat st;
    
	snprintf(pathname, sizeof(pathname), "/sys/class/uio/uio%d", dev_num);
    if(stat(pathname, &st) == 0)
    {
        dev = malloc(sizeof(uio_dev_t));
        if (dev != NULL)
        {
            memset(dev, 0, sizeof(uio_dev_t));
            dev->fd = -1;
            
            dev->num = dev_num;
            dev->name = uio_get_device_name(dev_num);
            dev->version = uio_get_device_version(dev_num);
            
            dev->maps = uio_get_device_maps(dev_num);
        }
    }
    
    return dev;
}

uio_dev_t* uio_locate_dev_by_name(const char *name)
{
    uio_dev_t *dev = NULL;
	struct dirent **namelist;
    int i = 0;

	int n = scandir("/sys/class/uio", &namelist, 0, alphasort);
    if (n < 0)
    {
        return NULL;
    }

    for(i = 0; (dev == NULL) && (i < n); i++ ) 
    {
        char pathname[64];
        
        snprintf(pathname, sizeof(pathname), "/sys/class/uio/%s/name", namelist[i]->d_name);
        
        char *s = uio_get_string_from_file(pathname);
        if (s == NULL)
        {
            continue;
        }
        
        if (strcmp(s, name) == 0)
        {
            int dev_num = -1;

            if (sscanf(namelist[i]->d_name, "uio%d", &dev_num) > 0)
            {
                dev = uio_locate_dev_by_number(dev_num);
            }
        }
        
        free(s);
    }
    
    for(i = 0; i < n; i++)
    {
        free(namelist[i]);
    }
    free(namelist);

    if(!dev)
    {
        DBG_ERR("%s: ERROR: failed to located device %s, err = %s", __func__, name, strerror(errno));
    }

    return dev;
}

#define UIO_STACK_SIZE     POSIX_MIN_STACK_SIZE
static pthread_t            uio_thd_id;
static unsigned char        UIOStack[UIO_STACK_SIZE]ALIGN8;

#define UIO_EPOLL_FLAG_EVENT_ATTACHED	    (1<<0)


#define MAX_EVENTS 20
void *UIOIntThread(void *unused)
{
    struct epoll_event events[MAX_EVENTS];

    while (1)
    {
        DBG_DEBUG("Waiting for UIO interrupt event\n");
        int nfds = epoll_wait(uio_epfd, events, MAX_EVENTS, -1);
        if (nfds < 0)
        {
            if (errno != EINTR)
            {
                DPRINTF((DBG_LOUD|DBG_OUTPUT), ("UIOLIB: epoll_wait failed - errno = %d\n", errno));
                posix_sleep_ms(500);
            }
        }
        else
        {
            int i;
            for (i = 0; i < nfds; i++) 
            {
                uio_dev_t *dev = events[i].data.ptr;
                int32_t int_count;
                
                ASSERT(dev);

                DBG_DEBUG("Reading event count from device %s\n", dev->name);
                if (read(dev->fd, &int_count, 4) == 4)
                {
                    if (dev->handler)
                    {
                        DBG_DEBUG("Calling handler count %d for device %s\n", int_count, dev->name);
                        dev->handler(int_count, dev->context);
                    }
                    
                    // re-enable interrupts
                    uio_int_enable(dev);
                }
            }
        }
    }
    return 0;
}

void uio_lib_init(void)
{
    int px_status;
    
    uio_epfd = epoll_create(1);
    REL_XASSERT(uio_epfd != -1, errno);
    
    px_status = posix_create_thread( &uio_thd_id, 
				     UIOIntThread, 
				     0, 
				     "UIO_interrupt_thread",
				     UIOStack, 
				     UIO_STACK_SIZE, 
				     (POSIX_THR_PRI_ISR));
    REL_XASSERT( px_status==0, px_status );
}

uio_dev_t* uio_open_dev(const char *name)
{
    char dev_name[32];
    
    UIO_GLOBAL_LOCK();
    uio_dev_t *dev = uio_locate_dev_by_name(name);
    if (dev != NULL)
    {
        XASSERT(dev->fd == -1, dev->fd);
        
        snprintf(dev_name, sizeof(dev_name), "/dev/uio%d", dev->num);
        dev->fd = open(dev_name, O_RDWR | O_CLOEXEC);
        if (dev->fd == -1)
        {
            uio_free_dev(dev);
            DBG_ERR("%s: FAILED name: %s, dev_name: %s, errno = %s\n", __func__, name, dev_name, strerror(errno));
            UIO_GLOBAL_UNLOCK();
            return NULL;
        }

        uio_map_priv_t* map = dev->maps;
        while (map != NULL)
        {
            uio_open_map(map, dev->fd);
            map = map->next;
        }
    }
    UIO_GLOBAL_UNLOCK();
    
    DBG_DEBUG("Opened device %s\n", dev->name);
    
    return dev;
}

char *uio_get_dev_name(uio_dev_t *hdev)
{
    uio_dev_t *dev = hdev;

    if (dev == NULL)
        return NULL;

    return dev->name;
}

int uio_get_dev_fd(uio_dev_t *hdev)
{
    uio_dev_t *dev = hdev;

    if (dev == NULL)
        return -1;

    return dev->fd;
}

void uio_close_dev(uio_dev_t *hdev)
{
    uio_dev_t *dev = hdev;

    if (dev == NULL)
        return;

    if (dev->fd == -1)
        return;

    UIO_GLOBAL_LOCK();
    uio_map_priv_t* map = dev->maps;
    while (map != NULL)
    {
        uio_close_map(map);
        map = map->next;
    }
    
    close (dev->fd);
    dev->fd = -1;

    DBG_DEBUG("Closed device %s\n", dev->name);
    
    uio_free_dev(dev);
    UIO_GLOBAL_UNLOCK();
}

uio_map_t* uio_get_map(uio_dev_t *hdev, int index)
{
    uio_dev_t *dev = hdev;
    
    if (dev == NULL)
        return NULL;

    UIO_GLOBAL_LOCK();
    uio_map_priv_t* map = dev->maps;
    int i = 0;
    while (map != NULL)
    {
        if (i++ == index)
        {
            break;
        }
        
        map = map->next;
    }
    UIO_GLOBAL_UNLOCK();
    
    if (map == NULL)
        return NULL;
        
    return &map->m;
}

int uio_get_map_count(uio_dev_t *hdev)
{
    uio_dev_t *dev = hdev;
    int i = -1;
    
    UIO_GLOBAL_LOCK();
    if (dev != NULL)
    {
        uio_map_priv_t* map = dev->maps;
        while (map != NULL)
        {
            i++;
            map = map->next;
        }
    }
    UIO_GLOBAL_UNLOCK();
    
    return i;
}

int uio_int_attach(uio_dev_t *hdev, uint32_t priority, UIO_PFN_ISR handler, void *context)
{
    uio_dev_t *dev = hdev;
    struct epoll_event ev;
    
    if (dev == NULL)
        return -1;
        
    if (uio_epfd == -1) 
        return -2;
        
    if (dev->event_flags & UIO_EPOLL_FLAG_EVENT_ATTACHED)
        return -3;
    
    dev->handler = handler;
    dev->context = context;
    dev->event_priority = priority;
        
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
	ev.data.ptr = hdev;
    
    UIO_GLOBAL_LOCK();
	if (epoll_ctl(uio_epfd, EPOLL_CTL_ADD, dev->fd, &ev) != 0)
    {
        dev->handler = NULL;
        dev->context = NULL;
        dev->event_priority = 0;
        
        UIO_GLOBAL_UNLOCK();
        return -4;
    }

    dev->event_flags |= UIO_EPOLL_FLAG_EVENT_ATTACHED;
    UIO_GLOBAL_UNLOCK();

    DBG_DEBUG("Attached interrupt on device %s, pri = %d\n", dev->name, priority);

    return 0;
}

int uio_int_detach(uio_dev_t *hdev)
{
    uio_dev_t *dev = hdev;
    struct epoll_event ev;
    
    if (dev == NULL)
        return -1;
        
    if (uio_epfd == -1) 
        return -2;
        
    if (!(dev->event_flags & UIO_EPOLL_FLAG_EVENT_ATTACHED))
        return -3;
        
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
	ev.data.ptr = hdev;
    
    UIO_GLOBAL_LOCK();
	if (epoll_ctl(uio_epfd, EPOLL_CTL_DEL, dev->fd, &ev) != 0)
    {
        UIO_GLOBAL_UNLOCK();
        return -4;
    }

    dev->handler = NULL;
    dev->context = NULL;
    dev->event_flags &= ~(UIO_EPOLL_FLAG_EVENT_ATTACHED);
    UIO_GLOBAL_UNLOCK();

    DBG_DEBUG("Detached interrupt on device %s\n", dev->name);
    
    return 0;
}

int uio_int_enable(uio_dev_t *hdev)
{
    uio_dev_t *dev = hdev;
    int32_t irq_enable = 1;
    
    if (dev == NULL)
        return -1;
        
    if (dev->fd == -1) 
        return -2;
        
    if (!(dev->event_flags & UIO_EPOLL_FLAG_EVENT_ATTACHED))
        return -3;
        
    if (write(dev->fd, &irq_enable, 4) != 4)
        return -4;
        
    DBG_DEBUG("Enabled interrupt on device %s\n", dev->name);
        
    return 0;
}

int uio_int_disable(uio_dev_t *hdev)
{
    uio_dev_t *dev = hdev;
    int32_t irq_disable = 0;

    if (dev == NULL)
        return -1;
        
    if (dev->fd == -1) 
        return -2;
        
    if (!(dev->event_flags & UIO_EPOLL_FLAG_EVENT_ATTACHED))
        return -3;
        
    if (write(dev->fd, &irq_disable, 4) != 4)
        return -4;

    DBG_DEBUG("Disabled interrupt on device %s\n", dev->name);
    
    return 0;
}

#endif // HAVE_UIO_LIB

