#include "dprintf.h"
#include "lassert.h"
#include "trsocket.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "sigma_interface.h"

#define WFA_ENABLED          1
#define WFA_OPTIONAL         1
#define WFA_DISABLED         0
#define WFA_REQUIRED         2
#define WFA_F_REQUIRED       3            /* forced required */
#define WFA_F_DISABLED       4            /* forced disabled */
#define WFA_INVALID_BOOL 0xFF

/* The fixed definition for the device interface name */
/* For different OSes or your platforms, you may not  */ 
/* need to use fixed definition                       */
#define WFA_STAUT_IF        "mlan0"

/* the path to store EAP certificates. This is used   */
/* for Linux-like OS for wpa_supplicant               */
#define CERTIFICATES_PATH    "/etc/wpa_supplicant"


#define wPRINTF \
                           dbg_printf

#define wSTRNCPY(dstr, sstr, len) \
                           strncpy(dstr, sstr, len)

#define wSTRCPY(dstr, sstr) \
                           strcpy(dstr, sstr)

#define wSTRNCPY(dstr, sstr, len) \
                           strncpy(dstr, sstr, len)

#define wSTRCMP(str1, str2) \
                           strcmp(str1, str2)

#define wSTRCASECMP(str1, str2) \
                           strcasecmp(str1, str2)

#define wSTRNCMP(str1, str2, size) \
                           strncmp(str1, str2, size)

#define wSTRNCAT(str1, str2, size) \
                           strncat(str1, str2, size)

#define wSTRLEN(str)     \
                           strlen(str)

#define wSOCKET(family, type, protocol) \
                           socket(family, type, protocol)

#define wSELECT(maxfdn, rsockSet, wsockSet, exceptSet, timer)  \
                           select(maxfdn, rsockSet, wsockSet, exceptSet, timer)

#define wSETSOCKOPT(sock, level, optname, optval, optlen) \
                           setsockopt(sock, level, optname, (char *)(optval), optlen)

#define wGETSOCKOPT(sock, level, optname, optval, optlen) \
                           getsockopt(sock, level, optname, (char *)(optval), optlen)

#define wBIND(sock, sockaddr, sizeaddr) \
                           bind(sock, sockaddr, sizeaddr)

#define wLISTEN(sock, maxnumpending)  \
                           listen(sock, maxnumpending)

#define wBZERO(ap, memsize) \
                           bzero(ap, memsize)

#define wCONNECT(sock, peeraddr, sizeaddr) \
                           connect(sock, peeraddr, sizeaddr)

#define wACCEPT(sock, addr, sizeaddr) \
                           accept(sock, addr, sizeaddr)

#define wINET_ATON(str_addr, dest) \
                           do { \
                            *((ttUser32Bit *)(dest)) = inet_aton(str_addr); \
                           } while ( 0 )

#define wSEND(sock, buf, len, flag) \
                           send(sock, (const char *)buf, len, flag)

#define wSENDTO(sock, buf, len, flag, toaddr, sizeaddr) \
                           sendto(sock, buf, len, flag, toaddr, sizeaddr)

#define wRECV(sock, buf, len, flag) \
                           recv(sock, (char *)buf, len, flag)

#define wRECVFROM(sock, buf, len, flag, fromaddr, sizeaddr) \
                           recvfrom(sock, buf, len, flag, fromaddr, sizeaddr)

#define wSOCKET_ERRNO(sock) \
                           tfGetSocketError(sock)

#define wSOCKET_ERR_TEXT(sock) \
                           tfStrError( tfGetSocketError(sock) )

#define wFCNTL(fd, cmd, args) \
                           fcntl(fd, cmd, args)

#define wSYSTEM(cmd) \
			   system(cmd)

#define wSPRINTF(buf, format...) \
			   minSprintf(buf, ## format)

#define wIOCTL(fd, cmd, intf) \
                           ioctl(fd, cmd, intf)

#define wMALLOC(size)  \
                           MEM_MALLOC(size)

#define wMEMCPY(dp, sp, size) \
                           memcpy(dp, sp, size)

#define wMEMSET(memp, val, size)  \
                           memset(memp, val, size)

#define wFREE(memp)      \
                           MEM_FREE_AND_NULL(memp);

void do_gettimeofday(struct timeval *time);
#define wGETTIMEOFDAY(tmval, zone) \
                           do_gettimeofday(tmval)  

#define wSETTIMEOFDAY(tmval, zone) \
                           settimeofday(tmval, zone) 

#define wSIGNAL(signum, sighandler) \
                           wfa_signal(signum, sighandler)

#define wALARM(duration) \
                           wfa_alarm(duration)

#define wCLOSE(fd)       \
                           tfClose(fd);

#define wUSLEEP(usecs)   \
                           do { \
                           int tx_ticks = usecs / 10000; \
                           int leftover = usecs - tx_ticks * 10000; \
                           wSLEEP(tx_ticks); \
                           cpu_spin_delay(leftover); \
                           } while (0) 
                           


#define wSLEEP(secs)   \
                           tx_thread_sleep(secs * 100)

#define wEXIT(num)     \
                           XASSERT(0 == "Sigma called exit() with value:", num)

#define wATOI(c)       \
                           atoi(c)

#define wSHUTDOWN(fd, type)      \
                           shutdown(fd, type)

#define wGETSOFD(sockfd, proto, tosval, ptosval, psize) \
                           getsockopt(sockfd, proto, tosval, (char *)ptosval, (int *)psize)

#define wPT_ATTR_INIT(ptattr) \
                           pthread_attr_init(ptattr)

#define wPT_ATTR_SETSCH(ptattr, policy) \
                           pthread_attr_setschedpolicy(ptattr, policy)   

#define wPT_ATTR_SETSCHPARAM(ptattr, param) \
                           pthread_attr_setschedparam(ptattr, param)

#define wPT_MUTEX_LOCK(lock) \
                           pthread_mutex_lock(lock)

#define wPT_MUTEX_UNLOCK(lock) \
                           pthread_mutex_unlock(lock)

#define wFFLUSH(out_stream)

#define wPT_COND_SIGNAL(signal) \
                           pthread_cond_signal(signal)

#define wPT_COND_WAIT(pcond, pmlock) \
                           pthread_cond_wait(pcond, pmlock)

#define wPT_MUTEX_INIT(pmlock, initval) \
                           pthread_mutex_init(pmlock, initval)

#define wPT_COND_INIT(pcond, initval) \
                           pthread_cond_init(pcond, initval)

#define wPT_CREATE(t, ptattr, func, pdata) \
                           pthread_create(t, ptattr, func, pdata)


typedef struct _memblock
{
    int id;
    int size;    /* number of blocks 512 byte/block */
    void *mem;
} wfaMemBlk_t;


