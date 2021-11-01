/*
**************************************************************************************
*
* Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
*
**************************************************************************************
*
* Marvell GPL License
*
* This file licensed to you in accordance with the terms and conditions of the General
* Public License Version 2, June 1991 (the "GPL License"). You can redistribute it
* and/or modify it under the terms of the GPL License; either version 2 of the License,
* or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
* PARTICULAR PURPOSE. See the GPL License for more details.
*
* You should have received a copy of the GNU General Public License along with this
* program. If not, see http://www.gnu.org/licenses/.
*
**************************************************************************************
*/

/** 
 *  \brief The laser_video_chardev driver is used to to expose a character driver
 *  interface from video to the user level code.  The driver routes calls to the
 *  proprietary videolaser driver.  The videolaser driver is a portable module
 *  designed to support multiple operating systems including RTOSs.  It calls
 *  through to the cross-platform laservideo driver.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <linux/kfifo.h>
#include <linux/completion.h>
#include <linux/sched.h>

#include "laser_video_chardev_api.h"
#include "laser_video_jbig_control_api.h"

#ifdef CONFIG_VER_C0_MV61X0
#include <mach/mv61x0.h>
#elif defined( CONFIG_VER_B0_MV31X0 )
#include <mach/mv31x0.h>
#else
#error "Unknown Processor"
#endif

#define LVC_DRIVER_NAME "laser_video_chardev"
#define LVC_NUM_DEVICES 1

typedef struct _lvc_dev {
    struct mutex lock;
    wait_queue_head_t page_done_waitq;
    struct kfifo      page_done_kfifo;
    struct cdev char_dev;
    struct platform_device *platform_dev;
    dev_t dev_num;
    int open_count;
    int channel;
}lvc_dev;

typedef struct _lvc_driver {
    dev_t dev_num_base;
    struct class *Class;
    lvc_dev* lvc_devices[LVC_NUM_DEVICES];
}lvc_driver;

static lvc_driver lvc_drv;

///////////////////////////////////////////////////////////
/// Interface with lower level video driver

void lvc_page_done_callback(void* callback_context, void* page_context, uint32_t page_status)
{
    lvc_dev* dev = (lvc_dev *)callback_context;
    page_done_notify_t pgdone;
    int written;
    
    pgdone.page_context = page_context;
    pgdone.page_status = page_status;

    mutex_lock(&dev->lock);
    written = kfifo_in(&dev->page_done_kfifo, &pgdone, sizeof(page_done_notify_t));
    mutex_unlock(&dev->lock);

    if(written != sizeof(page_done_notify_t))
    {
        printk(KERN_ERR "%s: ERROR: queue full.  Dropping nodification.", __func__);
    }

    wake_up_interruptible(&dev->page_done_waitq);
}

///////////////////////////////////////////////////////////
/// Character Driver Interface

static int lvc_cdev_open( struct inode *inode, struct file *filep )
{
    lvc_dev* dev;
    int errcode = 0;
    printk(KERN_DEBUG "%s\n", __FUNCTION__ );

    dev = (lvc_dev *)container_of(inode->i_cdev, lvc_dev, char_dev);
    BUG_ON(dev != lvc_drv.lvc_devices[dev->channel]);
    filep->private_data = dev;

    mutex_lock(&dev->lock);
    if(dev->open_count == 0)
    {
        dev->open_count++;
        vidctl_register_page_done_callback(lvc_page_done_callback, dev);
    }
    else
    {
        errcode = -EBUSY;
    }
    mutex_unlock(&dev->lock);

    return errcode;
}

static int lvc_cdev_release( struct inode *inode, struct file *filep )
{
    lvc_dev *dev;
    int errcode = 0;
    printk(KERN_DEBUG "%s\n", __FUNCTION__ );

    dev = filep->private_data;

    mutex_lock(&dev->lock);

    if(dev->open_count > 0)
    {
        dev->open_count--;
    }
    else
    {
        errcode = -ENOENT;
    }

    mutex_unlock(&dev->lock);

    return errcode;
}

static ssize_t lvc_read (struct file *filep, char __user *buf, size_t count, loff_t *pos)
{
    lvc_dev *dev;
    ssize_t bytes_read;
    int errcode;

    dev = filep->private_data;

    // must read at least one full notification struct worth
    if (count < sizeof(page_done_notify_t))
    {
        return -EINVAL;
    }

    errcode = wait_event_interruptible(dev->page_done_waitq, (kfifo_len(&dev->page_done_kfifo) >= sizeof(page_done_notify_t)));
    if(errcode)
    {
        return -ERESTARTSYS;
    }

    if(mutex_lock_interruptible(&dev->lock))
    {
        return -ERESTARTSYS;
    }

    errcode = kfifo_to_user(&dev->page_done_kfifo, buf, sizeof(page_done_notify_t), &bytes_read);
    mutex_unlock(&dev->lock);

    return (errcode == 0) ? bytes_read : errcode; 
}

static long lvc_ioctl_set_config(lvc_dev *dev, void __user *arg)
{
    laser_engine_video_config_t* vid_config;
    long errcode = 0;

    vid_config = kmalloc(sizeof(laser_engine_video_config_t), GFP_KERNEL);
    if (!vid_config)
        return -ENOMEM;

	if (!copy_from_user(vid_config, arg, sizeof(laser_engine_video_config_t)))
    {
        vidctl_init(vid_config);
    }
    else
    {
        errcode = -EFAULT;
    }

    kfree(vid_config);
    return errcode;
}

static long lvc_ioctl_set_pcode_config(lvc_dev *dev, void __user *arg)
{
    pcode_config_request_t* pcode_req;
    long errcode = 0;

	// copy pcode_config next
    pcode_req = kmalloc(sizeof(pcode_config_request_t), GFP_KERNEL);
    if (!pcode_req)
        return -ENOMEM;

	if (!copy_from_user(pcode_req, arg, sizeof(pcode_config_request_t)))
    {
        vidctl_set_pcodes(pcode_req->index, pcode_req->pcode_array, pcode_req->num_entries);
    }
    else
    {
        errcode = -EFAULT;
    }

    kfree(pcode_req);
    return errcode;
}

static long lvc_ioctl_start_page(lvc_dev *dev, void __user *arg)
{
    static laser_video_page_t page;
    long errcode = 0;

	if (!copy_from_user(&page, arg, sizeof(laser_video_page_t)))
    {
        if(vidctl_start_page(&page))
        {
            errcode = -EIO;
        }
    }
    else
    {
        errcode = -EFAULT;
    }
    return errcode;
}

// argument is numeric ID of page being completed
static long lvc_ioctl_end_page(lvc_dev *dev, void __user *arg)
{
    long errcode = 0;

    if(vidctl_end_page((uint32_t)arg))
    {
        errcode = -EINVAL;
    }

    return errcode;
}

static long lvc_ioctl_add_plane_data(lvc_dev *dev, void __user *arg)
{
    static laser_video_plane_t plane;
    long errcode = 0;

	if (!copy_from_user(&plane, arg, sizeof(laser_video_plane_t)))
    {
        if(vidctl_add_plane_data_to_current_page(&plane))
        {
            errcode = -EIO;
            printk(KERN_ERR "%s: vidctl_add_plane_data_to_current_page failed\n", __func__);
        }
    }
    else
    {
        printk(KERN_ERR "%s: copy from user failed\n", __func__);
        errcode = -EFAULT;
    }
    return errcode;
}

static long lvc_ioctl_cancel_pages(lvc_dev *dev)
{
    long errcode = 0;

    vidctl_cancel_pages();

    return errcode;
}

static long lvc_ioctl_query_pixel_count(lvc_dev *dev, void __user *arg)
{
    uint32_t non_white_pixels[MAX_NUM_VIDEO_CHAN] = {0, };
    long errcode = 0;
    int ret = 0;
    vidctl_query_pixel_count(non_white_pixels);
	ret = copy_to_user(arg, non_white_pixels, MAX_NUM_VIDEO_CHAN*sizeof(uint32_t));
    if(ret)
    {
        printk(KERN_ERR "%s: copy to user failed %d\n", __func__, ret);
        errcode = -EFAULT;
    }
    return errcode;
}

static long lvc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	lvc_dev *dev = file->private_data;

	long errcode = -ENOTTY;
    mutex_lock(&dev->lock);

    switch(cmd)
    {
        case LVC_IOCTL_SET_CONFIG:
            errcode = lvc_ioctl_set_config(dev, (void __user *) arg);
            break;

        case LVC_IOCTL_START_PAGE:
            errcode = lvc_ioctl_start_page(dev, (void __user *) arg);
            break;

        case LVC_IOCTL_END_PAGE:
            errcode = lvc_ioctl_end_page(dev, (void __user *) arg);
            break;

        case LVC_IOCTL_ADD_PLANE_DATA:
            errcode = lvc_ioctl_add_plane_data(dev, (void __user *) arg);
            break;

        case LVC_IOCTL_CANCEL_PAGES:
            errcode = lvc_ioctl_cancel_pages(dev);
            break;

        case LVC_IOCTL_SET_PCODE_CONFIG:
            errcode = lvc_ioctl_set_pcode_config(dev, (void __user *) arg);
            break;

        case LVC_IOCTL_QUERY_PIXEL_COUNT:
            errcode = lvc_ioctl_query_pixel_count(dev, (void __user *) arg);
            break;

        default:
            break;
    }

    mutex_unlock(&dev->lock);
    return errcode;
}

static const struct file_operations lvc_fops = {
    .owner = THIS_MODULE,
    .open = lvc_cdev_open,
    .release = lvc_cdev_release,
    .read = lvc_read,
    .unlocked_ioctl = lvc_ioctl,
};

///////////////////////////////////////////////////////////
/// Sysfs Interface

static ssize_t lvc_sysfs_paperless_set(struct device *dev, struct device_attribute *attr,
                                const char *buf, size_t count)
{
    int retcode;
    int enable;

    retcode = sscanf( buf, "%d", &enable );
    if ( retcode != 1 )
    {
        return -EINVAL;
    }

    if ( enable == 1 )
    {
        vidctl_set_paperless_mode(true);
    }
    else if ( enable == 0 )
    {
        vidctl_set_paperless_mode(false);
    }
    else
    {
        return -EINVAL;
    }

    return count;
}

static ssize_t lvc_sysfs_paperless_get(struct device *dev, struct device_attribute *attr,
                        char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%d\n", vidctl_get_paperless_mode() ? 1 : 0);
}

DEVICE_ATTR(paperless, S_IWUSR | S_IRUGO, lvc_sysfs_paperless_get, lvc_sysfs_paperless_set);

///////////////////////////////////////////////////////////
/// Platform Driver Interface

static int lvc_device_init(lvc_dev *dev, int id)
{
    if (id >= LVC_NUM_DEVICES) {
        printk(KERN_ERR "to many lvc devices found\n");
        return(-EIO);
    }

    if (IS_ERR(device_create(lvc_drv.Class, NULL, dev->dev_num,
                                       NULL, "lvc%d", id))) {
        printk(KERN_ERR "failed to create device: lvc%d\n", id);
        return(-1);
    }

    cdev_init( &dev->char_dev, &lvc_fops);
    dev->char_dev.owner = THIS_MODULE;
    dev->char_dev.ops   = &lvc_fops;

    if (cdev_add(&dev->char_dev, dev->dev_num, 1) < 0) {
        printk(KERN_ERR "can't add lvc%d device driver\n", id);
        return(-1);
    }

    dev->open_count = 0;
    dev->channel = id;
    mutex_init(&dev->lock);
    init_waitqueue_head(&dev->page_done_waitq);
    if(kfifo_alloc(&dev->page_done_kfifo, roundup_pow_of_two(16*sizeof(page_done_notify_t)), GFP_KERNEL) < 0)
    {
        printk(KERN_ERR "failed to alloc waitqueue\n");
        return -1;
    }

    return(0);
}

static int lvc_platform_probe(struct platform_device *pdev)
{
    int              retval = 0;
//    struct resource	*res = 0;
    lvc_dev*        dev;

    printk(KERN_DEBUG "%s: driver registration begun id %d\n", __func__, pdev->id);

    if(pdev->id >= LVC_NUM_DEVICES || lvc_drv.lvc_devices[pdev->id] != 0)
    {
        dev_err(&pdev->dev, " device already exists\n");
        return -EEXIST;
    }

    if (!(dev = (lvc_dev *)kzalloc(sizeof(lvc_dev), GFP_KERNEL))) {
        dev_err(&pdev->dev, "lvc_pll kzalloc failed\n");
        return(-ENOMEM);
    }

    dev->dev_num = MKDEV(MAJOR(lvc_drv.dev_num_base), pdev->id);

    // we use same register set for all driver device instances, so only map for dev 0
/*    if(pdev->id == 0)
    {
        res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        if (!res)
        {
            dev_err(&pdev->dev, "platform_get_resource failed\n");
            retval = -ENXIO;
        }

        else if (!request_mem_region(res->start, resource_size(res), LVC_DRIVER_NAME))
        {
            dev_err(&pdev->dev, "lvc request_mem_region failed start = %x, size = %d\n", res->start, resource_size(res));
            retval = -EBUSY;
        }
        else if (!(lvc_drv.pREGS = ioremap_nocache(res->start, resource_size(res))))
        {
            dev_err(&pdev->dev, "lvc ioremap failed\n");
            release_mem_region(res->start, resource_size(res));
            retval = -ENOMEM;
        }
        if(retval != 0)
        {
            goto resource_fail;
        }
    }
*/

    if ((retval = lvc_device_init(dev, pdev->id)))
    {
        dev_err(&pdev->dev, "lvc_platform_probe failed\n");
        goto probe_failed;
    }

    // Remember the platform and device ID's for this instance...
    dev->platform_dev = pdev;
    lvc_drv.lvc_devices[pdev->id] = dev;

    platform_set_drvdata(pdev, dev);

    if((retval = device_create_file(&pdev->dev, &dev_attr_paperless)))
    {
        printk(KERN_ERR "lvc%d: device attribute creation failed %d\n", pdev->id, retval);
    }
    printk(KERN_INFO "lvc%d: device probe complete\n", pdev->id);
    return 0;

probe_failed:
    if(pdev->id == 0)
    {
/*
        iounmap(lvc_drv.pREGS);
        release_mem_region(res->start, resource_size(res));
*/
    }

//resource_fail:
    kfree(dev);

    return retval;
}


static int lvc_platform_remove(struct platform_device *pdev)
{
    lvc_dev *dev;

    printk(KERN_INFO "%s: driver platform remove begun\n", __func__);

    device_remove_file(&pdev->dev, &dev_attr_paperless);

    dev = (lvc_dev*)platform_get_drvdata(pdev);

    kfifo_free(&dev->page_done_kfifo);
    mutex_destroy(&dev->lock);

    cdev_del(&dev->char_dev);
    device_destroy(lvc_drv.Class, dev->dev_num);

    if(pdev->id == 0)
    {
/* 
        struct resource *res;                                                           );
        iounmap(lvc_drv.pREGS);    
        res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        release_mem_region(res->start, resource_size(res));
*/
    }

    printk(KERN_INFO "lvc%d: device remove complete\n", pdev->id);

    lvc_drv.lvc_devices[pdev->id] = 0;

    kfree(dev);
    platform_set_drvdata(pdev, NULL);

	return 0;
}

static int lvc_platform_suspend(struct platform_device *pdev, pm_message_t state)
{
    printk(KERN_INFO "%s: driver platform suspend %d\n", __func__, state.event);
	return (0);
}

static int lvc_platform_resume(struct platform_device *pdev)
{
    printk(KERN_INFO "%s: driver platform resume begun\n", __func__);
    return(0);
}

static struct platform_driver lvc_platform_driver =
{
    .probe   = lvc_platform_probe,
    .remove  = lvc_platform_remove,
    .suspend = lvc_platform_suspend,
    .resume  = lvc_platform_resume,
    .driver  = {
        .name  = LVC_DRIVER_NAME,
        .owner = THIS_MODULE
    }
};

static int __init lvc_init(void)
{
    int retcode = 0;
    printk(KERN_INFO "lvc_init\n");

    memset(&lvc_drv, 0, sizeof(lvc_driver));

    lvc_drv.Class = class_create(THIS_MODULE, LVC_DRIVER_NAME);
    if (IS_ERR(lvc_drv.Class)) 
    {
        retcode = PTR_ERR(lvc_drv.Class);
        printk(KERN_ERR "unable to create lvc_pll class %d\n", retcode);
        goto EXIT;
    }

    retcode = alloc_chrdev_region( &lvc_drv.dev_num_base, 0, LVC_NUM_DEVICES, LVC_DRIVER_NAME );
    if( retcode != 0 ) 
    {
        printk(KERN_ERR "%s: error allocating chrdev region\n", __func__);
        goto EXIT_CHRDEV;
    }

    if ((retcode = platform_driver_register(&lvc_platform_driver)))
    {
        printk(KERN_ERR "%s: error registering lvc platform driver\n", __func__);
        goto EXIT_PLATFORM;
    }

    return 0;

EXIT_PLATFORM:
    unregister_chrdev_region(lvc_drv.dev_num_base, LVC_NUM_DEVICES);
EXIT_CHRDEV:
    class_destroy(lvc_drv.Class);
EXIT:

    return retcode;
}

static void __exit lvc_exit(void)
{
    platform_driver_unregister(&lvc_platform_driver);
    unregister_chrdev_region(lvc_drv.dev_num_base, LVC_NUM_DEVICES);
    class_destroy(lvc_drv.Class);
    printk(KERN_INFO "lvc_exit\n");
}


module_init(lvc_init);
module_exit(lvc_exit);

MODULE_AUTHOR("Marvell Semiconductor, Inc.");
MODULE_DESCRIPTION("Laser Video Character Driver");
MODULE_LICENSE("GPL");

