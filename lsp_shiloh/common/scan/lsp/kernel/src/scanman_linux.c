/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/* 
 * ScanMan, Linux Kernel edition.
 *
 * Handles communication between scantask (who does the scanning) and
 * userspace. Scantask is RTOS/platform agnostic as much as possible. Scanman
 * is the platform specific piece.
 *
 * davep 17-Jul-2012
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/cdev.h>
#include <linux/kfifo.h>
#include <linux/hardirq.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/uio.h>
#include <stdlib.h> // getenv()

#include <linux/delay.h>

#include "scos.h"

#include "lassert.h"
#include "memAPI.h"
//#include "cmd_proc_api.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanman.h"
#include "ostools.h"
#include "scantask.h"
#include "scanmech.h"
#include "scanvars.h"
#include "scanplat.h"
#include "cmdline.h"
#include "siqdata.h"
#include "scanvarsiq.h"
#include "scanmem.h"
#include "adfsensor.h"
#include "smsg_decode.h"
#include "chipgap.h"

#include "cal.h"
#include "cal_common.h"
#include "scanlib.h"
//#include "spec.h"

#include "../../../common/include/spec.h"

//#include "../../adfsensor_pinetree.h"

char SCANSUB_VERSION[]={"1.1.1"};	// add.lsh to report version info

#define SCANMAN_QUEUE_SIZE (4096*6)
#define SCANMAN_DEV_COOKIE 0x6CF45259

#define MAX_CMD_LINE 128

#define MAX_SCANBUF_LIST 2048

#define POR_CAL 1
#define SLEEP_CAL 1
#define SLEEP_ALL_CAL 1
#define SLEEP_PERIOD 20	

static ushort iCalibration;
module_param(iCalibration, ushort, 0);
MODULE_PARM_DESC(iCalibration, "Scan Status");

struct deviobuf {
    /* We are passing incoming messages to userspace through the /dev/scanman
     * read() call. We queue up the messages from kscantask in a list until
     * a read() happens. 
     *
     * The list must contain memory to queue up the data. 
     */
    scan_msg_t scan_msg;

    struct kvec msg_vec;    /* for scan_msg */
    struct kvec buffer_vec; /* for data payload (might be zero) */

    /* explicit is better than implicit; if this is non-null, we will PTR_FREE
     * it after all data is sent to userspace
     */
    uint8_t *buffer_to_free;
    uint32_t buffer_num_bytes;

    /* davep 15-May-2013 ; we need to send the exact size of the buffer to
     * scanmem_free_aligned(). The buffer_num_bytes field is the amount of 
     * image data in the buffer. The buffer_max_bytes is the real size of the
     * buffer.  buffer_num_bytes <= buffer_max_bytes
     */
    uint32_t buffer_max_bytes;

    struct list_head entry;
};

/* We need to preserve some data during the scan. We get the SMSG_SCAN_SIZE
 * message which contains the scan size (width, height, etc). The data messages
 * only contain the number of rows, not the buffer size. We have to infer the
 * buffer size based on the previous SMSG_SCAN_SIZE message.
 */
struct scanman_members {
    uint32_t pixels_per_row_padded;
    uint32_t pixels_per_row;
    uint32_t total_rows;
    uint32_t bits_per_pixel;

    /* calculated from the above fields */
    uint32_t bytes_per_row;
};

struct scanman_dev {
    uint32_t cookie;
    struct mutex lock;
    struct cdev cdev;

    /* lists of struct deviobuf. readies is data ready to be read(). empties is
     * struct deviobuf with no data attached
     */
    struct list_head readies;
    struct list_head empties;

    int num_readies;
    int num_empties;
};

struct scanman_stats {
    /* count incoming messages */
    uint32_t sizes;
    uint32_t scan_starts;
    uint32_t scan_ends;
    uint32_t scan_fails;
    uint32_t page_starts;
    uint32_t page_ends;
    uint32_t page_datas;
    uint32_t readies;
};

struct mbuffer {
    char *buf;
    int len;
    int max;
};

static struct mbuffer siqfile_buf;

static struct scanman_dev scanman_device;

static struct scos_mq scanman_msgq;

static struct scanman_members scanman_self;

DECLARE_COMPLETION( scanman_ready );
static struct task_struct *scanman_taskp;
static dev_t scanman_dev_num;

static struct scanman_stats scanman_stats;

//struct semaphore scanman_msgq_sem;
//static struct kfifo scanman_msgq;
//static spinlock_t scanman_msgq_lock;

static struct scanvars *g_scanvar;

//extern int gMachineType;
int sleep_cnt = 0;
int sleep_period = 0;
int init_cal_done = 0;

static scan_err_t run_calibration(scan_cmode_t cmode, int dpi, int sen_num);

#if 0
static scan_err_t scanman_test( scan_callback_fn_t scan_callback_fn )
{
    scan_err_t scerr;
    uint32_t task_id;

//    scan_set_config_nocal(true);

    if( g_scanvar != NULL ) {
        scanvar_delete( &g_scanvar );
    }

//    g_scanvar = scanplat_sv_color_scan_new();
    g_scanvar = scanplat_sv_mono_scan_new();
    if( g_scanvar==NULL ) {
        dbg2( "%s unable to alloc scanvar for scanman test\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }

//    scanvar_set_capture_cal( g_scanvar, true );

    if( scan_callback_fn ) {
        scanvar_set_scan_callback( g_scanvar, scanman_msg_send );
    }

    scerr = scantask_scan_start( g_scanvar, &task_id );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s scanman scan test failed scerr=%d\n", __FUNCTION__, scerr );
        return scerr;
    }

    dbg2( "%s task_id=%d\n", __FUNCTION__, task_id );

    return SCANERR_NONE;
}
#endif

scan_err_t scanman_msg_send_nowait( scan_msg_t *scan_msg )
{
    return scos_mq_send_nowait( &scanman_msgq, scan_msg );
}

scan_err_t scanman_msg_send( scan_msg_t *scan_msg )
{
    if ( in_interrupt() )
    {
        return scos_mq_send_nowait( &scanman_msgq, scan_msg );
    }
    return scos_mq_send( &scanman_msgq, scan_msg, SCOS_WAIT_FOREVER );
}

static int scanman_open( struct inode *inode, struct file *filp )
{
    dbg2( "%s\n", __FUNCTION__ );

    /* TODO */

    /* success! */
    return 0;
}

static int scanman_release( struct inode *inode, struct file *filp )
{
    dbg2( "%s\n", __FUNCTION__ );

    /* TODO */

    /* success! */
    return 0;
}

static int copy_iovec( struct kvec *dst, struct kvec *src, int *bytes_copied )
{
    unsigned long retcode;
    int bytes_to_copy;

    /* must have something to copy to get here */
    ASSERT( dst->iov_len );
    ASSERT( src->iov_len );

    bytes_to_copy = MIN( dst->iov_len, src->iov_len );

    retcode = copy_to_user( dst->iov_base, src->iov_base, bytes_to_copy );
    if( retcode != 0 ) {
        dbg2( "%s copy_to_user error retcode=%d\n", __FUNCTION__, retcode );
        return -EFAULT;
    }
        
    /* adjust both src & dst */
    dst->iov_len -= bytes_to_copy;
    dst->iov_base += bytes_to_copy;
    src->iov_len -= bytes_to_copy;
    src->iov_base += bytes_to_copy;

    *bytes_copied += bytes_to_copy;

    return 0;
}

static ssize_t scanman_read(struct file *filp, char __user *buf, size_t count, loff_t *offp )
{
    int retcode;
    struct deviobuf *db;
    int bytes_copied;
    struct kvec user_vec;

//    dbg2( "%s buf=0x%p count=%d\n", __FUNCTION__, buf, count );

    mutex_lock( &scanman_device.lock );

    if( list_empty( &scanman_device.readies ) ) {
        /* TODO sleep waiting for data */
//        dbg2( "%s no data\n", __FUNCTION__ );
        mutex_unlock( &scanman_device.lock );
        return 0;
    }

    user_vec.iov_base = buf;
    user_vec.iov_len = count;

    db = list_first_entry( &scanman_device.readies, struct deviobuf, entry );
    ASSERT( db );

    XASSERT( scanman_device.num_readies>0, scanman_device.num_readies );

    bytes_copied = 0;

    if( db->msg_vec.iov_len ) {
        retcode = copy_iovec( &user_vec, &db->msg_vec, &bytes_copied );
        if( retcode != 0 ) {
            mutex_unlock( &scanman_device.lock );
            return retcode;
        }
    }
    if( db->buffer_vec.iov_len ) {
        retcode = copy_iovec( &user_vec, &db->buffer_vec, &bytes_copied );
        if( retcode != 0 ) {
            mutex_unlock( &scanman_device.lock );
            return retcode;
        }
    }

    if( db->msg_vec.iov_len==0 && db->buffer_vec.iov_len==0 ) {
        /* finished this deviobuf so return it to ready list */
        
        /* free any data payload */
        if( db->buffer_to_free ) {
            /* davep 16-May-2013 ; if it's an image data buffer, have to free
             * it with scanmem so we can correctly maintain count.
             */
            if( db->scan_msg.msgType==SMSG_PAGE_DATA ) {
                /* davep 14-May-2013 ; tell scan subsystem to free so we can track
                 * our memory usage
                 */
                scanmem_free_aligned( &db->buffer_to_free, db->buffer_max_bytes );
            }
            else {
                /* usually a blob (from cal-to-host) */
                PTR_FREE( db->buffer_to_free );
            }
        }

        /* poison the msg header to catch bugs */
        memset( &db->scan_msg, 0xee, sizeof(scan_msg_t) );

        list_move_tail( &db->entry, &scanman_device.empties);
        scanman_device.num_empties += 1;
        scanman_device.num_readies -= 1;
        XASSERT( scanman_device.num_readies >=0, scanman_device.num_readies );
    }

    mutex_unlock( &scanman_device.lock );

//    dbg2( "%s bytes_copied=%d\n", __FUNCTION__, bytes_copied );

    return bytes_copied;
}

static scan_err_t mbuffer_new( struct mbuffer *buf, int len )
{
    memset( buf, 0, sizeof(struct mbuffer));

    buf->buf = kmalloc( len, GFP_KERNEL );
    if( !buf->buf ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    memset( buf->buf, 0, len );
    buf->max = len;
    /* mark buffer empty */
    buf->len = 0; 
    return SCANERR_NONE;
}

static scan_err_t mbuffer_realloc( struct mbuffer *buf )
{
    char *ptr;

    ptr = krealloc( buf->buf, buf->max*2, GFP_KERNEL );
    if( ptr==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    buf->buf = ptr;
    buf->max = buf->max*2;
    return SCANERR_NONE;
}

static void mbuffer_free( struct mbuffer *buf )
{
    kfree( buf->buf );
    memset( buf, 0, sizeof(struct mbuffer) );
}

static void mbuffer_print( struct mbuffer *buf )
{
    dbg2( "%s ptr=%p max=%d len=%d\n", __FUNCTION__, buf->buf, buf->max, buf->len );
}

static ssize_t scanman_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp )
{
    scan_err_t scerr;
    int retcode;
    char *siqptr;
    int avail;
    int eof_marker;
    scan_msg_t msg;
    struct scanvars *sv;

#define MAX_SIQFILE_LEN (64*1024)

    dbg2( "%s buf=%p count=%d\n", __FUNCTION__, buf, count );

    mbuffer_print( &siqfile_buf );

    if( siqfile_buf.buf==NULL ) {
        scerr = mbuffer_new( &siqfile_buf, MAX_SIQFILE_LEN );
        if( scerr != SCANERR_NONE ) {
            return -ENOMEM;
        }
    }

    if( siqfile_buf.len+count > siqfile_buf.max ) {
        scerr = mbuffer_realloc( &siqfile_buf );
        if( scerr != SCANERR_NONE ) {
            return -ENOMEM;
        }
    }

    mbuffer_print( &siqfile_buf );

    siqptr = siqfile_buf.buf + siqfile_buf.len;
    avail = siqfile_buf.max - siqfile_buf.len;
    
    /* should have realloc'd (if necessary) before we got here */
    XASSERT( avail>=count, avail );

    retcode = copy_from_user( siqptr, buf, count );
    if( retcode != 0 ) {
        mbuffer_free( &siqfile_buf );
        return -EFAULT;
    }
    siqfile_buf.len += count;
    
    /* end of this siqfile? */
#define END_OF_FILE "#END-OF-FILE-FOR-USB"
#define END_OF_FILE_LEN 20
    eof_marker = 0;
    if( siqfile_buf.len > END_OF_FILE_LEN ) {
        siqptr = siqfile_buf.buf + siqfile_buf.len - END_OF_FILE_LEN;
        if( strncmp( siqptr, END_OF_FILE, END_OF_FILE_LEN )==0 ) {
            eof_marker = 1;
        }
    }

    mbuffer_print( &siqfile_buf );

    if( eof_marker==0 ) {
        /* more data to come */
        return count;
    }

    mbuffer_print( &siqfile_buf );

//    printk( siqfile_buf.buf );
//    scanlog_hex_dump( siqfile_buf.buf, siqfile_buf.len );

    /* davep 01-Nov-2012 ; working on it... */
    sv = NULL;
    scerr = scanvar_from_siqfile( &sv, siqfile_buf.buf, siqfile_buf.len );

    if( scerr == SCANERR_NONE  ) {
        dbg2( "%s siqdata parse file success; sending to scanman\n", __FUNCTION__ );

        /* send the string to scanman */
        memset( &msg, 0, sizeof(scan_msg_t) );
        msg.msgType = SMSG_SCANMAN_SCAN_START;
        PTR_ASSIGN( msg.param3, sv );
        scerr = scanman_msg_send( &msg );
        XASSERT( scerr==SCANERR_NONE, scerr );
    }
    else {
        dbg2( "%s siqdata parse file failed scerr=%d\n", __FUNCTION__, scerr );
    }

    /* keep the storage but wipe it clean */        
    memset( siqfile_buf.buf, 0, siqfile_buf.max );
    siqfile_buf.len = 0;

    return count;
}

typedef struct
{
    uint32_t    request;
    int         arg;
    void       *ptr;
    scan_err_t  result;
} mech_ioctl_params_t;
static long dispatch_scanmech_ioctl(mech_ioctl_params_t *params_user)
{
    mech_ioctl_params_t  params;
    int copy_err;
    int retcode = 0;

    if (params_user != NULL)
    {
        copy_err = copy_from_user(&params, params_user, sizeof(mech_ioctl_params_t));

        switch ( params.request )
        {
            case SCANMECH_IOCTL_GET_ADF_MARGINS:
            {
                if ( params.ptr != NULL )
                {
                    struct scan_adf_margins margins;

                    copy_err = copy_from_user(&margins, params.ptr, sizeof(struct scan_adf_margins));

                    params.result = scanmech_get_adf_margins(margins.dpi, margins.cmode, &margins);

                    copy_err = copy_to_user(params.ptr, &margins, sizeof(struct scan_adf_margins));
                }
                else
                {
                    retcode = -EINVAL;
                }
                break;
            }

            case SCANMECH_IOCTL_DEBUG_SET_ADF_MARGINS:
            {
                if ( params.ptr != NULL )
                {
                    struct scan_adf_margins margins;

                    copy_err = copy_from_user(&margins, params.ptr, sizeof(struct scan_adf_margins));

                    params.result = scanmech_debug_set_adf_margins(&margins);

                    copy_err = copy_to_user(params.ptr, &margins, sizeof(struct scan_adf_margins));
                }
                else
                {
                    retcode = -EINVAL;
                }
                break;
            }

            case SCANMECH_IOCTL_GET_FLATBED_MARGINS:
            {
                if ( params.ptr != NULL )
                {
                    struct scan_flatbed_margins margins;

                    copy_err = copy_from_user(&margins, params.ptr, sizeof(struct scan_flatbed_margins));

                    params.result = scanmech_get_fb_margins(margins.dpi, margins.cmode, &margins);

                    copy_err = copy_to_user(params.ptr, &margins, sizeof(struct scan_flatbed_margins));
                }
                else
                {
                    retcode = -EINVAL;
                }
                break;
            }

            case SCANMECH_IOCTL_DEBUG_SET_FLATBED_MARGINS:
            {
                if ( params.ptr != NULL )
                {
                    struct scan_flatbed_margins margins;

                    copy_err = copy_from_user(&margins, params.ptr, sizeof(struct scan_flatbed_margins));

                    params.result = scanmech_debug_set_fb_margins(&margins);

                    copy_err = copy_to_user(params.ptr, &margins, sizeof(struct scan_flatbed_margins));
                }
                else
                {
                    retcode = -EINVAL;
                }
                break;
            }

            case SCANMECH_IOCTL_ADF_PICK_AND_KICK:
            {
                params.result = smech_ioctl( SCANMECH_IOCTL_ADF_PICK_AND_KICK, 0, 0 );
                break;
            }

            default:
                dbg1( "%s hasn't implemented ioctl: %d\n", __FUNCTION__, params.request );
                retcode = -EINVAL;
                break;
        }
        copy_err = copy_to_user(params_user, &params, sizeof(mech_ioctl_params_t));
    }
    else
    {
        retcode = -EINVAL;
    }

    return retcode;
}

static long scanman_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int retcode = 0;
	scan_err_t scerr;
	scan_err_t final_scerr;
    dbg2( "[S_M] %s cmd=%d arg=%ld\n", __FUNCTION__, cmd, arg );
    
    static scan_cmode_t cmode;        

    switch( cmd )
    {
        case _IOWR(0, 1, mech_ioctl_params_t) :
        {
            retcode = dispatch_scanmech_ioctl((mech_ioctl_params_t *)arg);
            break;
		}
		case SCANMECH_IOCTL_MOVE_TO_A3POSITION:
			scerr = scanmech_move_to_A3scanpoint(arg);
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_ADF_CLIENT_PAUSE:	//add.lsh.for USER CANCEL
            if(arg == 1) {
            	dbg1("[S_CANCEL] step2. pause!! scanman_unlocked_ioctl(%ld)!!\n", arg);
                scerr = scanmech_adf_client_request_pause();
                //scantask_cancel_msg();
            }
            else {
            	dbg1("[S_CANCEL] step2 resume!! scanman_unlocked_ioctl(%ld)!!\n", arg);
                scerr = scanmech_adf_client_clear_pause();                
            }
            final_scerr = scerr;
            return 0;
            break;
            
		/*Sleep Mode mod.kks  2014.08.07*/
		case SCANMECH_IOCTL_SLEEP_MODE:
			scanmech_sleep_request();
			break;

		case SCANMECH_IOCTL_WAKE_UP_MODE:
           	dbg1("[S_M_CAL] Scanman wake up!!\n");
           	scanmech_wakeup_request(); // motor driver wakeup mode. mod.kks 2014.08.07
			calibration_is_running();                	
//			ADD SLEEP Calibration
//			mode: 0 -> front mono  300 => PS
//				  1 -> Back  mono  300  
//				  2 -> front color 300 => PS
//				  3 -> Back  color 300
//				  4 -> front mono  600 => PS
//				  5 -> Back  mono  600
//				  6 -> front color 600
//				  7 -> Back  color 600
//			cal_cache_flush_all();				  


//			cal_cache_flush_mode(0);
//			dbg1("[S_M_CAL] SLEEP Calibration!!! MONO 300 \n");	
//			cmode = SCAN_CMODE_MONO;
//			run_calibration(cmode, 300, 1);

			
//			scerr = smech_ioctl( SCANMECH_IOCTL_MOVE_TO_HOME, 0, NULL );  //mod.kks SC219
//			if (scerr != SCANERR_NONE)
//			{
//				dbg1("ErrCode = %d\n", scerr);
//			}					
#if SLEEP_CAL
	#if SLEEP_ALL_CAL	

#if PINETREE_L
	
			if(sleep_cnt == 0) 
			{
				if(init_cal_done == 1) 
				cal_cache_flush_mode(0);
				dbg1("[S_M_CAL] SLEEP Calibration(PL)!!! Front MONO 300 \n");	
				cmode = SCAN_CMODE_MONO;
				run_calibration(cmode, 300, 1);
			}
//			else if(sleep_cnt == 1) 
//			{
//				cal_cache_flush_mode(1);
//				dbg1("[S_M_CAL] SLEEP Calibration!!! Back MONO 300 \n");	
//				cmode = SCAN_CMODE_MONO;
//				run_calibration(cmode, 300, 2);
//			}
			else if(sleep_cnt == 1) 
			{
				if(iCalibration == 1){
					if(init_cal_done == 1)
					cal_cache_flush_mode(2);
				}
				else
				{
					cal_cache_flush_mode(2);					
				}
				dbg1("[S_M_CAL] SLEEP Calibration(PL)!!! Front COLOR 300 \n");	
				cmode = SCAN_CMODE_COLOR;
				run_calibration(cmode, 300, 1);
			}
//			else if(sleep_cnt ==3) 
//			{
//				cal_cache_flush_mode(3);
//				dbg1("[S_M_CAL] SLEEP Calibration!!! Back COLOR 300 \n");	
//				cmode = SCAN_CMODE_COLOR;
//				run_calibration(cmode, 300, 2);
//			}						
			else if(sleep_cnt == 2) 
			{
				if(iCalibration == 1){
					if(init_cal_done == 1)
					cal_cache_flush_mode(4);
				}
				else
				{
					cal_cache_flush_mode(4);				
				}			
				dbg1("[S_M_CAL] SLEEP Calibration(PL)!!! Front MONO 600 \n");	
				cmode = SCAN_CMODE_MONO;
				run_calibration(cmode, 600, 1);
			}
//			else if(sleep_cnt == 5) 
//			{
//				cal_cache_flush_mode(5);
//				dbg1("[S_M_CAL] SLEEP Calibration!!! Back MONO 600 \n");	
//				cmode = SCAN_CMODE_MONO;
//				run_calibration(cmode, 600, 2);
//			}								
			else if(sleep_cnt == 3) 
			{
				cal_cache_flush_mode(6);
				dbg1("[S_M_CAL] SLEEP Calibration(PL)!!! Front COLOR 600 \n");	
				cmode = SCAN_CMODE_COLOR;
				run_calibration(cmode, 600, 1);
				init_cal_done = 1;
			}								
//			else if(sleep_cnt ==7) 
//			{
//				cal_cache_flush_mode(7);
//				dbg1("[S_M_CAL] SLEEP Calibration!!! Back COLOR 600 \n");	
//				cmode = SCAN_CMODE_COLOR;
//				run_calibration(cmode, 600, 2);
//				init_cal_done = 1;
//			}
			
			sleep_cnt++;
			sleep_period = SLEEP_PERIOD + 3;
#else //PNTREE(S)
//	#if SLEEP_ALL_CAL		
			if(sleep_cnt == 0) 
			{
				if(init_cal_done == 1) 
				cal_cache_flush_mode(0);
				dbg1("[S_M_CAL] SLEEP Calibration!!! Front MONO 300 \n");	
				cmode = SCAN_CMODE_MONO;
				run_calibration(cmode, 300, 1);
			}
			else if(sleep_cnt == 1) 
			{
				cal_cache_flush_mode(1);
				dbg1("[S_M_CAL] SLEEP Calibration!!! Back MONO 300 \n");	
				cmode = SCAN_CMODE_MONO;
				run_calibration(cmode, 300, 2);
			}
			else if(sleep_cnt == 2) 
			{
				if(iCalibration == 1){
					if(init_cal_done == 1)
					cal_cache_flush_mode(2);
				}
				else
				{
					cal_cache_flush_mode(2);					
				}
				dbg1("[S_M_CAL] SLEEP Calibration!!! Front COLOR 300 \n");	
				cmode = SCAN_CMODE_COLOR;
				run_calibration(cmode, 300, 1);
			}
			else if(sleep_cnt ==3) 
			{
				cal_cache_flush_mode(3);
				dbg1("[S_M_CAL] SLEEP Calibration!!! Back COLOR 300 \n");	
				cmode = SCAN_CMODE_COLOR;
				run_calibration(cmode, 300, 2);
			}						
			else if(sleep_cnt == 4) 
			{
				if(iCalibration == 1){
					if(init_cal_done == 1)
					cal_cache_flush_mode(4);
				}
				else
				{
					cal_cache_flush_mode(4);				
				}			
				dbg1("[S_M_CAL] SLEEP Calibration!!! Front MONO 600 \n");	
				cmode = SCAN_CMODE_MONO;
				run_calibration(cmode, 600, 1);
			}
			else if(sleep_cnt == 5) 
			{
				cal_cache_flush_mode(5);
				dbg1("[S_M_CAL] SLEEP Calibration!!! Back MONO 600 \n");	
				cmode = SCAN_CMODE_MONO;
				run_calibration(cmode, 600, 2);
			}								
			else if(sleep_cnt == 6) 
			{
				cal_cache_flush_mode(6);
				dbg1("[S_M_CAL] SLEEP Calibration!!! Front COLOR 600 \n");	
				cmode = SCAN_CMODE_COLOR;
				run_calibration(cmode, 600, 1);
			}								
			else if(sleep_cnt ==7) 
			{
				cal_cache_flush_mode(7);
				dbg1("[S_M_CAL] SLEEP Calibration!!! Back COLOR 600 \n");	
				cmode = SCAN_CMODE_COLOR;
				run_calibration(cmode, 600, 2);
				init_cal_done = 1;
			}
			
			sleep_cnt++;
			sleep_period = SLEEP_PERIOD + 7;
#endif

			if(sleep_cnt == sleep_period)
			{
				cal_cache_flush_all();	
				sleep_cnt = 0;				
			}
	#else
			cal_cache_flush_mode(0);
			dbg1("[S_M_CAL] SLEEP Calibration!!! Front MONO 300 \n");	
			cmode = SCAN_CMODE_MONO;
			run_calibration(cmode, 300, 1);
	#endif //end sleep all cal			
#endif //end sleep cal

			calibration_is_finished();            				
			return 0;
			break;

		case SCANMECH_IOCTL_SP_FB_FREE_RUN:
			scerr = scanmech_spmode_request_fb_free_run( arg );
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_SP_ADF_FREE_RUN:
			scerr = scanmech_spmode_request_adf_free_run( arg );
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_SP_FEED_MOTOR:
			scerr = scanmech_spmode_request_feed_motor( arg );
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_SP_TRANS_MOTOR:
			scerr = scanmech_spmode_request_trans_motor( arg );
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_SP_PRESENT_SENSOR:
			scerr = scanmech_spmode_request_present_sensor( arg );
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_SP_INTERVAL_SENSOR:
			scerr = scanmech_spmode_request_interval_sensor( arg );
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_SP_1STSCAN_SENSOR:
			scerr = scanmech_spmode_request_firstscan_sensor( arg );
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_SP_TOPCOVER_SENSOR:
			scerr = scanmech_spmode_request_topcover_sensor( arg );
			final_scerr = scerr;
			return 0;
			break;
		case SCANMECH_IOCTL_SP_HOMESENSOR:
			scerr = scanmech_spmode_request_home_sensor( arg );
			final_scerr = scerr;
			return 0;
			break;
			            
        default : 
            dbg1( "[S_M] %s cmd=%d (0x%x) invalid\n", __FUNCTION__, cmd, cmd );
            retcode = -EINVAL;
            break;
    }

    return retcode;
}

static const struct file_operations scanman_fops = {
    .owner = THIS_MODULE,
    .open = scanman_open,
    .read = scanman_read,
    .write = scanman_write,
//    .fsync = scanman_fsync,
//    .poll = scanman_poll,
    .unlocked_ioctl = scanman_unlocked_ioctl,
    .release = scanman_release
};

void scanman_dump_stats( void )
{
    dbg1( "[S_M] scanman stats sizes=%d sstarts=%d sends=%d sfails=%d pstarts=%d pends=%d pdata=%d readies=%d\n", 
            scanman_stats.sizes,
            scanman_stats.scan_starts,
            scanman_stats.scan_ends,
            scanman_stats.scan_fails,
            scanman_stats.page_starts,
            scanman_stats.page_ends,
            scanman_stats.page_datas,
            scanman_stats.readies
            );
}

    
static void queue_deviomsg_with_payload( scan_msg_t *msg, uint8_t *buf, 
                        size_t buflen_used_bytes, size_t buflen_max_bytes )
{
    struct deviobuf *db;

    mutex_lock( &scanman_device.lock );

    if( list_empty( &scanman_device.empties ) ) {
        /* 
         * This case is why we need two kernel threads!
         *
         * If the userspace reader isn't reading fast enough, the data will
         * build up and we will run out of queue space. The scantask thread
         * cannot block. Scantask must run in order to do a scanning pause.
         *
         * Scanman can block.
         */

        /* TODO sleep waiting for empties (or something) */
        ASSERT(0);
    }

    db = list_first_entry( &scanman_device.empties, struct deviobuf, entry );

    ASSERT( db );

    /* save the msg contents, set the iobuf to point inside self */
    memcpy( &db->scan_msg, msg, sizeof(scan_msg_t) );
    db->msg_vec.iov_base = &db->scan_msg;
    db->msg_vec.iov_len = sizeof(scan_msg_t);

    /* if we have data, point the buffer_vec to it */
    if( !buf ) {
        memset( &db->buffer_vec, 0, sizeof(struct kvec) );
        db->buffer_to_free = NULL;
    }
    else {
        db->buffer_vec.iov_base = buf;
        db->buffer_vec.iov_len = buflen_used_bytes;

        /*
         * BIG FAT NOTE!  I'm currently assuming all buf's passed to this
         * function are from either SMSG_PAGE_DATA or SMSG_DATA_BLOB. Those
         * messages' payload is a malloc'd buffer that must be freed.
         *
         * In the future, we might want to add support for sending
         * non-free'able data. We don't need it right now. 
         *
         */
        db->buffer_to_free = buf;
        db->buffer_num_bytes = buflen_used_bytes;
        db->buffer_max_bytes = buflen_max_bytes;
    }

    /* pop from empties, put onto readies list for the read() to find */
    list_move_tail( &db->entry, &scanman_device.readies );

    scanman_device.num_empties -= 1;
    scanman_device.num_readies += 1;
    XASSERT( scanman_device.num_empties >=0, scanman_device.num_empties );

    mutex_unlock( &scanman_device.lock );
}

static void queue_deviomsg( scan_msg_t *msg )
{
    queue_deviomsg_with_payload( msg, NULL, 0, 0 );
}

static void handle_data_message( scan_msg_t *msg )
{
    uint16_t num_rows, max_rows;
    scan_data_type dtype;
    uint8_t *ptr;
    bool last_buffer;
    uint32_t image_bytes, max_buffer_bytes;

    /* decode the message because we need to know the buffer size */
    scan_msg_page_data_decode( msg, &num_rows, &dtype, &ptr, &last_buffer);

    /* davep 15-May-2013 ; adding max rows so we know the exact buffer size
     * (without worrying about the bouncing buffers problem)
     */
    scan_msg_page_data_get_max_rows( msg, &max_rows );
    
    image_bytes = num_rows * scanman_self.bytes_per_row;
    max_buffer_bytes = max_rows * scanman_self.bytes_per_row;

//    dbg2( "%s nr=%d mr=%d dtype=%d ptr=%p last=%d\n", __FUNCTION__, 
//            num_rows, max_rows, dtype, ptr, last_buffer );

    queue_deviomsg_with_payload( msg, ptr, image_bytes, max_buffer_bytes );
}

static void handle_data_blob_message( scan_msg_t *msg )
{
    uint32_t num_bytes;
    uint8_t *ptr;

    /* decode the message because we need to know the buffer size */
    scan_msg_data_blob_decode( msg, &ptr, &num_bytes );
    
//    dbg2( "%s num_bytes=%d ptr=%p last=%d\n", __FUNCTION__, 
//            num_bytes, ptr );

    queue_deviomsg_with_payload( msg, msg->param3, num_bytes, num_bytes );
}

static void deviobuf_list_free( int sanity_counter )
{
    int cnt;
    struct deviobuf *db;

    cnt = 0;

    /* free the empties */
    while(1) {
        db = list_first_entry( &scanman_device.empties, struct deviobuf, entry );
        if( !db ) {
            break;
        }
        list_del( &db->entry );

        /* poison it */
        memset( db, 0xee, sizeof(struct deviobuf) );
        PTR_FREE( db );

        scanman_device.num_empties -= 1;
        XASSERT( scanman_device.num_empties >=0, scanman_device.num_empties );

        cnt++;
    }

    /* free any readies we might have left */
    while(1) {
        db = list_first_entry( &scanman_device.empties, struct deviobuf, entry );
        if( !db ) {
            break;
        }
//        PTR_FREE( sb->buf );
//        sb->ptr = NULL;

        list_del( &db->entry );

        /* poison it */
        memset( db, 0xee, sizeof(struct deviobuf) );
        PTR_FREE( db );

        scanman_device.num_readies -= 1;
        XASSERT( scanman_device.num_readies >=0, scanman_device.num_readies );

        cnt++;
    }

    /* check for memory leaks */
    XASSERT( cnt==sanity_counter, cnt );
}

static scan_err_t deviobuf_list_create( void )
{
    struct deviobuf *db;
    int i;

    dbg2( "%s\n", __FUNCTION__ );

    for( i=0 ; i<MAX_SCANBUF_LIST ; i++ ) {
        db = kzalloc( sizeof(struct deviobuf), GFP_KERNEL );
        if( db==NULL ) {
            /* free what we have already allocated */
            deviobuf_list_free( i );

            return SCANERR_OUT_OF_MEMORY;
        }

        list_add( &db->entry, &scanman_device.empties );
        scanman_device.num_empties += 1;
    }
    
    return SCANERR_NONE;
}

static void handle_paper_event_msg( scan_adf_paper_event_t adfevt, uint32_t value  )
{
//    scan_err_t scerr;

    dbg2( "scanman received adf paper event evt=%d value=%d\n", adfevt, value );
}

static scan_err_t scanman_start_scan( struct scanvars *sv )
{
    scan_err_t scerr;
    int task_id;

    /* davep 31-Oct-2012 ; for now... */
//    scan_set_config_nocal( true );

    /* davep 01-Nov-2012 ; XXX this is a temporary workaround until I figure
     * out why unpickle isn't correctly clearing the hw dpis
     */
    scerr = scanvar_set_dpi(sv,sv->dpi);
    XASSERT( scerr==SCANERR_NONE, scerr );

//    scanvar_print( sv );
#if 0
    /* display what we have parsed */
    scanvar_to_siqfile( sv );

    /* free the memory */
    scanvar_delete( &sv );
#else
    if( g_scanvar ) {
        scanvar_delete( &g_scanvar );
    }

    PTR_ASSIGN( g_scanvar, sv );

    scanvar_set_scan_callback( g_scanvar, scanman_msg_send );

    /* davep 04-Dec-2012 ; XXX hardwire to use ADF */
//    scanvar_set_document_source( g_scanvar, SCAN_DOCUMENT_SOURCE_ADF );

    scerr = scantask_scan_start( g_scanvar, &task_id );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s scanman scan test failed scerr=%d\n", __FUNCTION__, scerr );
        return scerr;
    }

    dbg2( "%s task_id=%d\n", __FUNCTION__, task_id );
#endif

    return SCANERR_NONE;
}

static void scanman_dispatch_message( scan_msg_t *msg )
{
    scan_err_t scerr;
    struct scanvars *sv;

    switch( msg->msgType ) {
        case SMSG_SCANMAN_SCAN_START : 
            /* message from the scanman driver dev write() call; contains a
             * pointer to siqfile 
             */
			scanman_stats.sizes = 0;
            scanman_stats.scan_starts = 0;
            scanman_stats.scan_ends = 0;
            scanman_stats.scan_fails = 0;
            scanman_stats.page_starts = 0;
            scanman_stats.page_ends = 0;
            scanman_stats.page_datas = 0;
            scanman_stats.readies = 0;
            
			scanman_stats.scan_starts++;
            dbg1( "[S_M] 	SMSG_SCANMAN_SCAN_START\n");

            PTR_ASSIGN( sv, msg->param3 );
            ASSERT( sv );

            scerr = scanman_start_scan( sv );
            if( scerr != SCANERR_NONE ) {
                dbg1( "[S_M] Error : failed to start scan scerr=%d\n", scerr );
                /* continue */
            }
            break;

        case SMSG_SCAN_SIZE :
            scan_msg_size_decode( msg, 
                        &scanman_self.pixels_per_row_padded, &scanman_self.pixels_per_row, 
                        &scanman_self.total_rows, &scanman_self.bits_per_pixel );
            dbg1( "[S_M] 	SMSG_SCAN_SIZE(ppr=%d pprp=%d tr=%d bpp=%d)\n",
            		scanman_self.pixels_per_row, 
                    scanman_self.pixels_per_row_padded,
                    scanman_self.total_rows, scanman_self.bits_per_pixel );

            /* pre-calculate the bytes_per_row (will be constant across this scan) */
            scanman_self.bytes_per_row = scanman_self.pixels_per_row_padded * 
                                         (scanman_self.bits_per_pixel/8);
            scanman_stats.sizes++;
            queue_deviomsg( msg);
            break;

        case SMSG_SCAN_START :	// cmt.lsh not used
            dbg1( "[S_M] 	SMSG_SCAN_START\n");
            queue_deviomsg( msg);
            break;

        case SMSG_PAGE_START :
        	scanman_stats.page_starts++;
        	dbg1( "[S_M] 	SMSG_PAGE_START(p_n:%d-%d, p_d:%d)\n", scanman_stats.page_starts, msg->param1);
        	if(msg->param1 == 0)	// front side
        		scanman_stats.page_datas = 0;
            queue_deviomsg( msg);
            break;

        case SMSG_PAGE_DATA :
//			dbg1( "[S_M] 	SMSG_PAGE_DATA\n");
            scanman_stats.page_datas++;
            handle_data_message( msg );
            break;

        case SMSG_PAGE_END :
            scanman_stats.page_ends++;
            dbg1( "[S_M] 	SMSG_PAGE_END(p_n:%d-%d, p_d:%d)\n", scanman_stats.page_ends, msg->param1, scanman_stats.page_datas);
            queue_deviomsg( msg);
            break;

        case SMSG_SCAN_END :
            dbg1( "[S_M] 	SMSG_SCAN_END\n");
            scanman_stats.scan_ends++;
            queue_deviomsg( msg);

            /* sanity check the returning scanvar */
            ASSERT( msg->param3 );
            PTR_ASSIGN( sv, msg->param3 );
            scanvar_sanity( sv );
            XASSERT( sv==g_scanvar, sv->id );

            /* free the memory */
            if( g_scanvar ) {
                scanvar_delete( &g_scanvar );
            }
            break;

        case SMSG_SCAN_READY :
            /* scan_ready is the "ack" from scantask telling us he's done */
            dbg1( "[S_M] 	SMSG_SCAN_READY\n");
            scanman_stats.readies++;
            queue_deviomsg( msg);
            scanman_dump_stats();	// add.lsh.for debug
            break;

        case SMSG_DATA_BLOB : 
            /* data blobs are usually from cal-to-host */
            dbg1( "[S_M] 	SMSG_DATA_BLOB\n");
            handle_data_blob_message( msg );
            break;

        case SMSG_CAL_CALIBRATION_IN_PROGRESS :
  			dbg1("[S_M_CAL] SMSG_CAL_CALIBRATION_IN_PROGRESS(SML)\n");
            queue_deviomsg( msg);
  			break;

        case SMSG_CAL_CALIBRATION_DONE :
  			dbg1("[S_M_CAL] SMSG_CAL_CALIBRATION_DONE(SML)\n");
            queue_deviomsg( msg);
            break;
    
        case SMSG_SCAN_ADF_PAPER_EVENT :
            dbg2( "scanman notified of paper event: %d, %d\n", msg->param1, msg->param2 );
            handle_paper_event_msg( msg->param1, msg->param2 );
            queue_deviomsg( msg);
			if(msg->param1 > 2)	// SCAN_ADF_PAPER_JAM, SCAN_ADF_PAPER_NOPICK
	            scanman_dump_stats();	// add.lsh.for debug
            break;

        case SMSG_SCAN_FAILED : 
            dbg1( "[S_M] 	SMSG_SCAN_FAILED\n");
            scanman_stats.scan_fails++;
            scanman_dump_stats();	// add.lsh.for debug
            queue_deviomsg( msg);
            break;
            
	//add kmg
		case SMSG_SC_AFE_OFFSET_FAIL : 
			dbg1("[S_M_SC] SMSF_SC_AFE_OFFSET_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;
			
		case SMSG_SC_AFE_GAIN_FAIL : 
			dbg1("[S_M_SC] SMSF_SC_AFE_GAIN_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;
			
		case SMSG_SC_AFE_SETTING_FAIL : 
			dbg1("[S_M_SC] SMSF_SC_AFE_SETTING_FAIL(SML)\n");
			queue_deviomsg( msg);
			break; 
			
		case SMSG_SC_CIS_HOME_POSITION_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_CIS_HOME_POSITION_FAIL(SML)\n");        
			queue_deviomsg( msg);
			break; 
			
		case SMSG_SC_WHITE_STRIP_READ_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_WHITE_STRIP_READ_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;       
			
		case SMSG_SC_BLACK_STRIP_READ_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_BLACK_STRIP_READ_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;    
			
		case SMSG_SC_BAD_PIXEL_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_BAD_PIXEL_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;                      
			
		case SMSG_SC_CIS_HOME_OFF_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_CIS_HOME_OFF_FAIL(SML)\n");
			queue_deviomsg( msg);
			break; 
			
		case SMSG_SC_CIS_HOME_ON_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_CIS_HOME_ON_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;         
				   
		case SMSG_SC_PIC_OVF_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_PIC_OVF_FAIL(SML)\n");
			queue_deviomsg( msg);
			break; 
			
		case SMSG_SC_BAD_PIXEL_FAIL_B : 
			dbg1("[S_M_SC] SMSG_SC_BAD_PIXEL_FAIL_B(SML)\n");
			queue_deviomsg( msg);
			break;                      			            

		case SMSG_SC_CMD_QUEUE_OVR_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_CMD_QUEUE_OVR_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;   
			
		case SMSG_SC_CMD_COMPLETE_OVR_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_CMD_COMPLETE_OVR_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;   
			
		case SMSG_SC_SCAN_ASSERT_FAIL : 
			dbg1("[S_M_SC] SMSG_SC_SCAN_ASSERT_FAIL(SML)\n");
			queue_deviomsg( msg);
			break;   
			
		case SMSG_SC_PIPE_WAIT_TIMEOUT : 
			dbg1("[S_M_SC] SMSG_SC_PIPE_WAIT_TIMEOUT(SML)\n");
			queue_deviomsg( msg);
			break;   
			
        default :
            /* epic fail! */
            dbg1("[S_M] Error : unknown message 0x%x param1=0x%x param2=0x%x param3=%p\n",
                  __FUNCTION__, msg->msgType, msg->param1, msg->param2, msg->param3 );
            XASSERT( 0, msg->msgType );
            break;
    }
}

// sysfs interface definitions
static ssize_t scan_cmdline_set(struct device *dev, struct device_attribute *attr,
                                const char *buf, size_t count)
{
    int retcode;

    retcode = scantask_cmd_line_run(buf);
    
    if (retcode != 0)
        printk(KERN_ERR "ERROR, bad command line: %s\n",buf);
    return count;
}

static ssize_t scan_cmdline_get(struct device *dev, struct device_attribute *attr,
                        char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "scan_cmdline_get\n");
}

static ssize_t scan_chipgap_set(struct device *dev, struct device_attribute *attr,
                                const char *buf, size_t count)
{
    scan_err_t scerr;
    int retcode;
    uint32_t first_gap;
    uint32_t gap_distance;

    /* using the integer parsing design from drivers/cpufreq/cpufreq.c */
    retcode = sscanf( buf, "%u %u", &first_gap, &gap_distance );
    if( retcode != 2 ) {
        return -EINVAL;
    }

    scerr = chipgap_set_positions( first_gap, gap_distance );

    printk( "%s scerr=%d\n", __FUNCTION__, scerr );

    return count;
}

static ssize_t scan_chipgap_get(struct device *dev, struct device_attribute *attr,
                        char *buf)
{
    uint32_t first_gap;
    uint32_t gap_distance;

    chipgap_get_positions( &first_gap, &gap_distance );

    return scnprintf(buf, PAGE_SIZE, "%d %d\n", first_gap, gap_distance );
}

static ssize_t sysfs_scan_mech_set(struct device *dev, struct device_attribute *attr,
                                const char *buf, size_t count)
{
    scan_err_t scerr;
    int retcode;
    int flag;

     /* using the integer parsing design from drivers/cpufreq/cpufreq.c */
     retcode = sscanf( buf, "%d", &flag );
     if( retcode != 1 ) {
         return -EINVAL;
     }
 
     if( flag==1 ) {
         scerr = scanmech_adf_client_request_pause();
     }
     else if( flag==0 ) {
         scerr = scanmech_adf_client_clear_pause();
     }
     else { 
         /* ignore */
         scerr = SCANERR_INVALID_PARAM;
     }

     printk( "%s scerr=%d\n", __FUNCTION__, scerr );
 
     return count;
}

static ssize_t sysfs_scan_mech_get(struct device *dev, struct device_attribute *attr,
                        char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "scan_mech_get\n");
}

static ssize_t sysfs_adf_paper_present_get(struct device *dev, struct device_attribute *attr, char *buf)
{
    int adfpp;

    adfpp = adf_sensor_paper_present();
    return scnprintf(buf, PAGE_SIZE, "%d\n", adfpp );
}

static ssize_t sysfs_adf_paper_present_set(struct device *dev, struct device_attribute *attr,
                                            const char *buf, size_t count)
{
    return 0;
}

DEVICE_ATTR(cmdline, S_IWUSR | S_IRUGO, scan_cmdline_get, scan_cmdline_set);
DEVICE_ATTR(chipgap, S_IWUSR | S_IRUGO, scan_chipgap_get, scan_chipgap_set);
DEVICE_ATTR(scanmech, S_IWUSR | S_IRUGO, sysfs_scan_mech_get, sysfs_scan_mech_set);
DEVICE_ATTR(adf_pp, S_IRUGO, sysfs_adf_paper_present_get, sysfs_adf_paper_present_set );


static int scanman_thread( void *data )
{
    int retcode;
    scan_err_t scerr;
    scan_msg_t msg;
    struct kobject *kobj; 
    
    static scan_cmode_t cmode; 
 	static scan_err_t cal_scerr = SCANERR_NONE;
    
    printk("==========================\n");
	printk("== SCAN MODULE Ver 0.20 ==\n");
	printk("==========================\n");
	
    // create a sysfs interface 
    kobj = kobject_create_and_add("scan",firmware_kobj);
    retcode = sysfs_create_file(kobj, &dev_attr_cmdline.attr);
    if( retcode != 0 ) {
        printk( KERN_ERR "%s sysfs_create_file cmdline failed retcode=%d\n",
                __FUNCTION__, retcode );
    }
    retcode = sysfs_create_file(kobj, &dev_attr_chipgap.attr);
    if( retcode != 0 ) {
        printk( KERN_ERR "%s sysfs_create_file chipgap failed retcode=%d\n",
                __FUNCTION__, retcode );
    }
    retcode = sysfs_create_file(kobj, &dev_attr_scanmech.attr);
    if( retcode != 0 ) {
        printk( KERN_ERR "%s sysfs_create_file scanmech failed retcode=%d\n",
                __FUNCTION__, retcode );
    }
    retcode = sysfs_create_file(kobj, &dev_attr_adf_pp.attr);
    if( retcode != 0 ) {
        printk( KERN_ERR "%s sysfs_create_file adf_pp failed retcode=%d\n",
                __FUNCTION__, retcode );
    }


    /* davep 20-Sep-2013 ; XXX temp disable for splitting kscantask/kscanman */
//    scan_cmdline_init();

    // Start scan worker task.
    //scerr = scantask_init( scanman_msg_send );
    scerr = scantask_init( scanman_msg_send_nowait );	// mod.lsh. for msg to scanman in idle mode (from interrupt context)
    dbg1("[S_M] ScanTask(k) Init Done\n");
    
    XASSERT( scerr==SCANERR_NONE, scerr );

    memset( &msg, 0, sizeof(scan_msg_t) );

    // Wait for scantask to initialize.
    // Don't do anything else until that happens.
    while( msg.msgType != SMSG_SCAN_INIT_DONE ) {

        scerr = scos_mq_receive( &scanman_msgq, &msg, SCOS_WAIT_FOREVER );
        XASSERT( scerr==SCANERR_NONE, scerr );

        switch( msg.msgType ) {
            case SMSG_SCAN_INIT_DONE :
            	dbg1( "[S_M] (not init) SMSG_SCAN_INIT_DONE\n");
      
                /* check for an error */
#if 0
                scantask_hw_err = (ScannerHWError)msg.param1;

                if( scantask_hw_err != ScannerHWError_None ) {
                    dbg1( "%s scantask failed to init! err=%d\n", 
                                __FUNCTION__, scantask_hw_err );
                    scanman_observer_notify_event( SCANMAN_EVENT_SCAN_OFFLINE, 1 );
                }
                else {
                    /* scantask is up and running successfully */
                    dbg1( "%s scantask initialized successfully\n", __FUNCTION__ );
                    /* the state transition to idle below will send the observer notify
                     * event 
                     */
                }

#endif
                /* Note we continue on from here whether or not Scantask
                 * successfully initialized. Why? Well, why not? What else should we
                 * do? 
                 */
                break;

            /* davep 04-Feb-2011 ; add handling of paper event; need this early
             * in the boot process (event before INIT_DONE) so the hardware can
             * send us an initial state
             */
            case SMSG_SCAN_ADF_PAPER_EVENT :
            	dbg1( "[S_M] (not init) SMSG_SCAN_ADF_PAPER_EVENT(%d, %d)\n", msg.param1, msg.param2);
                handle_paper_event_msg( msg.param1, msg.param2 );
				queue_deviomsg( &msg);   //mod.kks
				if(msg.param1 > 2)	// SCAN_ADF_PAPER_JAM, SCAN_ADF_PAPER_NOPICK
					scanman_dump_stats();	// add.lsh.for debug
                break;

            default : 
                dbg1( "%s unwanted message msgType=%d param1=%d param2=%d param3=%p\n",
                            __FUNCTION__, msg.msgType, msg.param1,
                            msg.param2, msg.param3 );
                break;
        }
    }

    complete_all( &scanman_ready );
    
   scerr =  scanmech_selftest(); //mod.kks Support the SC218, SC219 on POR.
   dbg1("[MECH] Mech init done(%d) \n", scerr);
   
   if(scerr == SCANERR_NONE){ // mod.kks If the motor don't move, don't work calibration.
		#if POR_CAL
			if(iCalibration == 1)
			{
				// Run Calibration....
				dbg1("[S_M_CAL] POR Calibration!!! MONO 300 \n");
				if(cal_scerr == SCANERR_NONE){ 
					cmode = SCAN_CMODE_MONO;
					cal_scerr = run_calibration(cmode, 300, 1);				
				}

				dbg1("[S_M_CAL] POR  Calibration!!! MONO 600 \n");
				if(cal_scerr == SCANERR_NONE){ 				
					cmode = SCAN_CMODE_MONO;
					cal_scerr = run_calibration(cmode, 600, 1);
				}

				dbg1("[S_M_CAL] POR  Calibration!!! COLOR 300 \n");
				if(cal_scerr == SCANERR_NONE){ 				
					cmode = SCAN_CMODE_COLOR;
					cal_scerr = run_calibration(cmode, 300, 1);	
				}
				
				if(cal_scerr == SCANERR_NONE) 
					dbg1("//////////// POR Calibration : Succeed!! (err=%d) ///////////////\n", cal_scerr);
				else
					dbg1("//////////// POR Calibration : Fail!! (err=%d) ///////////////\n", cal_scerr);
			}
			else
			{
				// Run Calibration....
				dbg1("[S_M_CAL] POR Calibration!!! MONO 300 \n");
				if(cal_scerr == SCANERR_NONE){
					cmode = SCAN_CMODE_MONO;
					cal_scerr = run_calibration(cmode, 300, 1);
				}
				
				if(cal_scerr == SCANERR_NONE) 
					dbg1("//////////// POR Calibration : Succeed!! (err=%d) ///////////////\n", cal_scerr);
				else
					dbg1("//////////// POR Calibration : Fail!! (err=%d) ///////////////\n", cal_scerr);				
			}   
			calibration_is_finished();            
		#endif 
	}
		 

    while( 1 ) {
        scerr = scos_mq_receive( &scanman_msgq, &msg, SCOS_WAIT_FOREVER );
        XASSERT( scerr==SCANERR_NONE, scerr );

//        dbg2( "%s message type=%d param1=%d param2=%d param3=%p\n", __FUNCTION__,
//                msg.msgType, msg.param1, msg.param2, msg.param3 );
        scanman_dispatch_message( &msg );
    }

    return 0;
}

static void wait_for_scanman_ready( void )
{
    dbg2( "%s\n", __FUNCTION__ );
    wait_for_completion( &scanman_ready );
    dbg2( "%s done\n", __FUNCTION__ );
}

void scanman_cleanup( void )
{
    struct scanman_dev *dev;

    dev = &scanman_device;

    unregister_chrdev_region( scanman_dev_num, 1 );

    cdev_del( &dev->cdev );
}

void ScanManInit( void )
{
    scan_err_t scerr;
    int retcode;
    struct scanman_dev *dev;

    dbg2( "%s\n", __FUNCTION__ );

    scerr = scos_mq_init( &scanman_msgq, "scanman", SCANMAN_QUEUE_SIZE );
    if( scerr != SCANERR_NONE ) {
        printk( KERN_ERR "%s failed to create message queue scerr=%d\n", 
                __FUNCTION__, scerr );
        return;
    }
    
    dev = &scanman_device;
    dev->cookie = SCANMAN_DEV_COOKIE;

    mutex_init( &dev->lock );
    INIT_LIST_HEAD( &dev->readies );
    INIT_LIST_HEAD( &dev->empties );

    scerr = deviobuf_list_create();
    if( scerr != SCANERR_NONE ) {
        printk( KERN_ERR "%s failed to allocate deviobuf lists scerr=%d\n", 
                __FUNCTION__, scerr );
        return;
    }

    retcode = alloc_chrdev_region( &scanman_dev_num, 0, 1, "scanman" );
    if( retcode != 0 ) {
        goto fail_chrdev;
    }

    cdev_init( &dev->cdev, &scanman_fops );

    retcode = cdev_add( &dev->cdev, scanman_dev_num, 1 );
    if( retcode != 0 ) {
        goto fail_dev_add;
    }

    scerr = mbuffer_new( &siqfile_buf, MAX_SIQFILE_LEN );
    if( siqfile_buf.buf==NULL ) {
        dbg2( "%s failed to allocate siqfile_buf\n", __FUNCTION__ );
        goto fail_mbuffer_new;
    }

//    retcode = kfifo_alloc( &scanman_msgq, SCANMAN_QUEUE_SIZE, GFP_KERNEL );
//    printk( KERN_INFO "kfifo_alloc() retcode=%d\n", retcode );
//    spin_lock_init( &scanman_msgq_lock );
//    sema_init( &scanman_msgq_sem, 0 );

    scanman_taskp = kthread_run( scanman_thread, NULL, "kscanman" );
    printk( KERN_INFO "scanman_taskp=%p\n", scanman_taskp );

    wait_for_scanman_ready();
    return;

fail_mbuffer_new:
    /* TODO clean up from cdev_add */
    cdev_del( &dev->cdev );

fail_dev_add:
    unregister_chrdev_region( scanman_dev_num, 1 );

fail_chrdev:
    /* TODO clean up from cdev_init? */

    return; 
}

char* get_version_ScanSubsystem()
{
   return (&SCANSUB_VERSION[0]);
}

static int __init scanman_init_module(void)
{
    dbg2( "%s\n", __FUNCTION__ );

    ScanManInit();
	dbg1("[S_M] ScanMan(k) Init Done\n");
    /* 
     * A non 0 return means init_module failed; module can't be loaded. 
     */
    return 0;
}

static void __exit scanman_cleanup_module(void)
{
    dbg2( "%s\n", __FUNCTION__ );
}


static scan_err_t run_calibration(scan_cmode_t cmode, int dpi, int sen_num)
{
//    scan_err_t scerr, cal_scerr, final_scerr;
//    struct scanvars *sv;
	struct scanvars *cal_sv;
	struct scanvars *tmp_sv;
	bool force_cal;
	scan_err_t cal_scerr;
//    struct scan_platform_capabilities scancap;
//    uint32_t scan_config;
//    uint32_t flag;
    int bitmask;	

	if(sen_num != 1)
	{
		cal_sv->doc_src=SCAN_DOCUMENT_SOURCE_ADF_NSENSOR;
	}


    /* make a calibration scanvar that is optimized for our scan destination */
//    cal_sv = scanvar_cal_newcopy( sv );
#if SLEEP_CAL
    set_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_OUTER,cal_sv);
#else

#endif

	if (cmode == SCAN_CMODE_MONO)
		cal_sv = scanvar_mono_cal_new();
	
	else if (cmode == SCAN_CMODE_COLOR)
		cal_sv = scanvar_color_cal_new();
	else
		return SCANERR_GENERIC_FAIL;
	
    if( cal_sv==NULL ) {
        dbg1( "scanman_linux failed to get cal scanvars\n" );
        return SCANERR_OUT_OF_MEMORY;
    }

    /* make our calibration configuration the active configuration */
    scanvar_push( cal_sv );

//	scanvar_set_dpi( cal_sv, dpi );  

//  scan_config = scan_get_config();
	
    /* davep 12-Oct-2006 ; adding ability to cal-to-host */
//    cal_sv->scan_callback_fn = scanman_msg_send;
    force_cal = false;

    /* davep 01-Jun-2012 ; add calibrating start/stop messages so scanman can
     * update observers
     */
//	calibration_is_running();     

//    nAfeReset += 0x80;

    /* davep 04-Jun-2012 ; XXX temp debug ; EFT failure seems to happen during
     * cal --force it to always cal
     */
	cal_sv->hw_dpi_horiz= dpi;
	cal_sv->hw_dpi_vert= dpi;	
	bitmask = sen_num;

    cal_scerr = calibrate(bitmask, cal_sv->hw_dpi_horiz, cal_sv->cmode, force_cal );                       
    if( cal_scerr != 0 ) {
        dbg1("calibration failed; not scanning\n" );
        /* keep going so we can cleanup the cal scanvar */
        scanlib_cancel_self();
    }

    /* davep 01-Jun-2012 ; add calibrating start/stop messages so scanman can
     * update observers
     */
//     calibration_is_finished();
     
//	nAfeReset -= 0x80;
	
    /* take the cal configuration off the stack */
    tmp_sv = scanvar_pop();
    XASSERT( tmp_sv==cal_sv, tmp_sv->id );
    tmp_sv = NULL;	

    scanvar_delete( &cal_sv );//add.kmg

#if SLEEP_CAL
  	cal_done();
    clear_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_OUTER);
#else

#endif

	return cal_scerr;
}

module_init( scanman_init_module );
module_exit( scanman_cleanup_module );

EXPORT_SYMBOL(scanman_msg_send);
EXPORT_SYMBOL(scanman_dev_request);
//EXPORT_SYMBOL(scanman_smirb_sanity);

MODULE_AUTHOR("Copyright (c) 2012 Marvell International Ltd. All Rights Reserved");
MODULE_DESCRIPTION("Marvell series Scan Driver");

MODULE_LICENSE("GPL");
MODULE_VERSION("2012_APR_11");

