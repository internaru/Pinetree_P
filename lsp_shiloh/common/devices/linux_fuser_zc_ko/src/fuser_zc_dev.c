#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>

#include <regAddrs.h>
#include <fuser_regmasks.h>

#include <mach/mv61x0.h>
#include <mach/mv61x0_irqs.h>
#include <asic/regAddrs.h>
#include <asic/asic_gr_C0/fuser_regstructs.h>

#include "fuser_zc_chardev_api.h"

#define FZC_DRIVER_NAME "laser_fuser_zc"
#define FZC_NUM_DEVICES 1

#define DEC_FUSER_Mode_SyncPWM      0x0
#define DEC_FUSER_Mode_ASyncPWM     0x1
#define DEC_FUSER_Mode_SyncDSM      0x2
#define DEC_FUSER_Mode_ASyncDSM     0x3

static FUSER_REGS_t *fuser_reg = NULL;
static uint32_t cur_state;
static uint32_t fuser_mode_request;
static uint32_t irq_cnt=0;
static uint32_t duty_cycle = 0;
static uint32_t zc_timer = 0;

typedef struct _fzc_dev {
    struct mutex lock;
    struct cdev char_dev;
    struct platform_device *platform_dev;
    dev_t dev_num;
    int open_count;
    int channel;
}fzc_dev;

typedef struct _fzc_driver {
    dev_t dev_num_base;
    struct class *Class;
    fzc_dev* fzc_devices[FZC_NUM_DEVICES];
}fzc_driver;

static fzc_driver fzc_drv;

#define MAX_FUSERS 3 // <3 hw Blocks
#define FUSER_INSTANCE 1

typedef struct fuser_instance_regs_s
{
    volatile uint32_t FCR[MAX_FUSERS];
    volatile uint32_t FPECR[MAX_FUSERS];
    volatile uint32_t FPOCR[MAX_FUSERS];
    volatile uint32_t FDCR[MAX_FUSERS];
    volatile uint32_t FPACR[MAX_FUSERS];
} fuser_instance_regs_t;

enum {
    FUSER_OFF = 0,
    FUSER_ON = 1
};

static void dec_fuser_set_duty(unsigned int on, unsigned int off)
{
    uint32_t output_enable;
    fuser_instance_regs_t *i_regs = (fuser_instance_regs_t *)(&fuser_reg->FCR0);

    i_regs->FPECR[FUSER_INSTANCE] = FUSER_FPECR0_FUSER_TIMEON_EVEN_REPLACE_VAL(i_regs->FPECR[FUSER_INSTANCE],  on); //0
    i_regs->FPECR[FUSER_INSTANCE] = FUSER_FPECR0_FUSER_TIMEOFF_EVEN_REPLACE_VAL(i_regs->FPECR[FUSER_INSTANCE], off); //5
    i_regs->FPOCR[FUSER_INSTANCE] = FUSER_FPOCR0_FUSER_TIMEON_ODD_REPLACE_VAL(i_regs->FPOCR[FUSER_INSTANCE],   on); //0
    i_regs->FPOCR[FUSER_INSTANCE] = FUSER_FPOCR0_FUSER_TIMEOFF_ODD_REPLACE_VAL(i_regs->FPOCR[FUSER_INSTANCE],  off); //5
   
    if(duty_cycle != 0){//enable fuser[1] 
    	output_enable = (fuser_reg->FEPR & FUSER_FEPR_FUSER_OE_MASK) | (1<<FUSER_INSTANCE);
    	fuser_reg->FEPR = FUSER_FEPR_FUSER_OE_REPLACE_VAL(fuser_reg->FEPR, output_enable);
    }
}

static void dec_fuser_stop(void)
{
    if(duty_cycle)
	return;

    /* disable fuser[1] */
    uint32_t output_enable;
    output_enable = fuser_reg->FEPR & FUSER_FEPR_FUSER_OE_MASK;
    output_enable = output_enable & ~(1<<FUSER_INSTANCE);
    fuser_reg->FEPR = FUSER_FEPR_FUSER_OE_REPLACE_VAL(fuser_reg->FEPR, output_enable);
    
    dec_fuser_set_duty(0, 5);
    cur_state = FUSER_OFF;
}

static void dec_fuser_full_start(void)
{
	if(duty_cycle==0)
		return;
	//TODO
	if(irq_cnt%10==0){
		switch(duty_cycle)
		{
			case 20: dec_fuser_set_duty(1, 4);
				 break;

			case 40: dec_fuser_set_duty(2, 3);
				 break;

			case 60: dec_fuser_set_duty(3, 2);
				 break;

			case 80: dec_fuser_set_duty(4, 1); 
				 break;

			case 100: dec_fuser_set_duty(5, 0);
				  break;
			default: 
				  //prev value 
				  break;
		}
		irq_cnt = 0;
		cur_state = FUSER_ON;
	}
}

static void dec_fuser_soft_start(uint32_t local_duty)
{
	static uint32_t instant_duty =0;
	static uint32_t pulses_per_duty =0;

#define ZC_TIMER_60HZ 		900
#define PULSES_PER_60HZ 	6
#define PULSES_PER_50HZ 	5

	if(instant_duty ==0 && zc_timer < ZC_TIMER_60HZ)
		pulses_per_duty = PULSES_PER_60HZ; 
	else if(instant_duty ==0) 
		pulses_per_duty = PULSES_PER_50HZ;

	if(irq_cnt%10==0) //100ms
	{
		/* Just to keep last duty till next 100ms */
		if(pulses_per_duty==0){
			cur_state = FUSER_ON;
			instant_duty = 0;
			irq_cnt = 0;
			return;
		}

		switch(instant_duty)
		{
			case 0:	instant_duty = 20;
				pulses_per_duty--;
				dec_fuser_set_duty(1, 4);
				break;

			case 20: instant_duty = 40;
				 pulses_per_duty--;
				 if(local_duty > 20)
				  	dec_fuser_set_duty(2, 3);
				 break;

			case 40: instant_duty = 60;
				 pulses_per_duty--;
				 if(local_duty > 40)
					 dec_fuser_set_duty(3, 2);
				 break;

			case 60: instant_duty = 80;
				 pulses_per_duty--;
				 if(local_duty > 60)
					dec_fuser_set_duty(4, 1);
				 break;

			case 80: instant_duty = 100;
				 pulses_per_duty--;
				 if(local_duty > 80)
					dec_fuser_set_duty(5, 0);
				 break;

			case 100: /* 60Hz only: Continue with prev duty */
				  //instant_duty = 120;
				  pulses_per_duty--;
				  break;

			default:
				 break;	 //Just ignore
		}
	}
}

static void dec_fuser_init(void)
{
    uint32_t fepr_reg_val = 0;
    uint32_t filter = 0; 
    uint32_t lockout = 0; 
    const uint32_t fuser_pulse_width = 1000;
    //const uint32_t fuser_pulse_offset = 5000;

    cur_state = FUSER_OFF;
    fuser_mode_request = FUSER_OFF;

    /* configure fuser pinouts and polarity */
    fepr_reg_val = FUSER_FEPR_OUTPUT_MUX_REPLACE_VAL(fepr_reg_val, 0x03);
    fepr_reg_val = FUSER_FEPR_LVPOWERON_POLARITY_REPLACE_VAL(fepr_reg_val, 1);
    fepr_reg_val = FUSER_FEPR_LVPOWERON_OE_REPLACE_VAL(fepr_reg_val, 0x01);
    fepr_reg_val = FUSER_FEPR_LVPOWERON_REPLACE_VAL(fepr_reg_val, 0x01);
    fepr_reg_val = FUSER_FEPR_FUSEROVERTEMP_POLARITY_REPLACE_VAL(fepr_reg_val, 0);
    fepr_reg_val = FUSER_FEPR_FUSEROVERTEMP_IE_REPLACE_VAL(fepr_reg_val, 0);
    fepr_reg_val = FUSER_FEPR_FUSER_POLARITY_REPLACE_VAL(fepr_reg_val, 0x02);
    fuser_reg->FEPR = fepr_reg_val;

    /* Zet Zero Cross */
    filter = FUSER_ZCFR_FALL_LO_WEIGHT_REPLACE_VAL(0, 0xa);
    filter = FUSER_ZCFR_FALL_HI_WEIGHT_REPLACE_VAL(filter, 0x2);
    filter = FUSER_ZCFR_RISE_LO_WEIGHT_REPLACE_VAL(filter, 0x2);
    filter = FUSER_ZCFR_RISE_HI_WEIGHT_REPLACE_VAL(filter, 0xa);
    filter = FUSER_ZCFR_THRESHOLD_REPLACE_VAL(filter, 100);
    filter = FUSER_ZCFR_TIMEBASE_SEL_REPLACE_VAL(filter, 0x2);//1usec timebase
    filter = FUSER_ZCFR_OUTSENSE_REPLACE_VAL(filter, 0x0);
    filter = FUSER_ZCFR_OUTTYPE_REPLACE_VAL(filter, 0x2); //one clock pulse on rising edge
    fuser_reg->ZCFR = filter;

    lockout = FUSER_ZCLR_ZC_INT_TYPE_REPLACE_VAL(0, 0x2);//falling edge ??used??
    lockout = FUSER_ZCLR_LOCKOUT_TIMEBASE_REPLACE_VAL(lockout, 0x3);//100usec
    lockout = FUSER_ZCLR_LOCKOUT_COUNT_REPLACE_VAL(lockout, 50);//50 ticks 5000uSec total
    fuser_reg->ZCLR = lockout;

    /* Fuser configuration mode */
    fuser_instance_regs_t *i_regs = (fuser_instance_regs_t *)(&fuser_reg->FCR0);
    i_regs->FCR[FUSER_INSTANCE] = FUSER_FCR0_FUSER_MODE_REPLACE_VAL(i_regs->FCR[FUSER_INSTANCE], DEC_FUSER_Mode_SyncPWM);

    /* Setting fuser width, offset */
    i_regs->FPACR[FUSER_INSTANCE] = FUSER_FPACR0_FUSER_PULSEWIDTH_REPLACE_VAL(i_regs->FPACR[FUSER_INSTANCE], fuser_pulse_width);
    //i_regs->FPACR[FUSER_INSTANCE] = FUSER_FPACR0_FUSER_PULSEOFFSET_REPLACE_VAL(i_regs->FPACR[FUSER_INSTANCE], fuser_pulse_offset);
    printk("FZC_KO: Dec Fuser Init Done!!\n");
}

static irqreturn_t zc_interrupt(int irq, void *dev_id)
{
    if ( !fuser_reg ) 
	return IRQ_NONE;
   
    uint32_t enabled = fuser_reg->FIER;
    uint32_t pending = fuser_reg->FIPR;

    if (pending) {
        fuser_reg->FIAR = pending & enabled;
        if ((pending & enabled) & ~0x80) {
            printk("FIPR: %xh\n", pending);
        }
    }
    irq_cnt++;

    if(fuser_mode_request){
	if(cur_state == FUSER_OFF)
		dec_fuser_soft_start(duty_cycle);
	else
		dec_fuser_full_start();
    }
    else
	dec_fuser_stop(); 

    return IRQ_HANDLED;
}

static int fzc_cdev_open( struct inode *inode, struct file *filep )
{
    fzc_dev* dev;
    int errcode = 0;
    printk("FZC_KO: %s\n", __func__);

    dev = (fzc_dev *)container_of(inode->i_cdev, fzc_dev, char_dev);
    BUG_ON(dev != fzc_drv.fzc_devices[dev->channel]);
    filep->private_data = dev;

    mutex_lock(&dev->lock);
    if(dev->open_count == 0)
    {
        dev->open_count++;
    }
    else
    {
        errcode = -EBUSY;
    }
    
   if(zc_timer ==0){
	    zc_timer = FUSER_FSR_ZCTIMER_MASK_SHIFT(fuser_reg->FSR);
	    printk("FZC_KO: zc_timer %d\n", zc_timer);
   }

    if (!request_irq(IRQ_DEC_FUSER, zc_interrupt, IRQF_DISABLED, "fzc_device", NULL) ) {
        printk(KERN_DEBUG "FZC_KO: request_irq IRQ_DEC_FUSER: %d Failed\n", IRQ_DEC_FUSER);
    }

    mutex_unlock(&dev->lock);

    return errcode;
}

static int fzc_cdev_release( struct inode *inode, struct file *filep )
{
    fzc_dev *dev;
    int errcode = 0;
    printk("FZC_KO: %s\n", __func__);

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

    free_irq(IRQ_DEC_FUSER, NULL);
    mutex_unlock(&dev->lock);

    return errcode;
}

static long fzc_ioctl_set_config(fzc_dev *dev, void __user *arg)
{
    long ret_bytes = 0;
    ret_bytes = copy_from_user(&fuser_mode_request, (uint32_t *)arg, sizeof(uint32_t));
    
    if(fuser_mode_request > 0 && fuser_mode_request <= 20)
        duty_cycle = 20;
    else if(fuser_mode_request > 20 && fuser_mode_request <= 40)
        duty_cycle = 40;
    else if(fuser_mode_request > 40 && fuser_mode_request <= 60)
        duty_cycle = 60;
    else if(fuser_mode_request > 60 && fuser_mode_request <= 80)
        duty_cycle = 80;
    else if(fuser_mode_request > 80)
        duty_cycle = 100;
    else
        duty_cycle = 0;

    printk("FZC_IOCTL_SET_CONFIG %d\n", fuser_mode_request);
    return ret_bytes;
}

static long fzc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
   fzc_dev *dev = file->private_data;
   bool power_on;
   long ret_bytes;

   if(_IOC_TYPE(cmd) != FZC_IOCTL_BASE) return -EINVAL;
   if(_IOC_NR(cmd) > FZC_IOCTL_COMMANDS) return -ENOTTY;
   if( _IOC_DIR(cmd) & _IOC_WRITE )
      if( !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd) ) )
          return -EFAULT;

   mutex_lock(&dev->lock);

    switch(cmd)
    {
        case FZC_IOCTL_SET_CONFIG:
            ret_bytes = fzc_ioctl_set_config(dev, (void __user *) arg);
            break;

        case FZC_IOCTL_ENABLE_POWER:
            ret_bytes = copy_from_user(&power_on, (uint32_t *)arg, sizeof(bool));
	    fuser_reg->FEPR = FUSER_FEPR_LVPOWERON_REPLACE_VAL(fuser_reg->FEPR, (power_on?1:0));
	    break;

        default:
	    printk(KERN_INFO " Got invalid case, CMD=%d\n", cmd);
	    ret_bytes = -EINVAL;
            break;
    }

    mutex_unlock(&dev->lock);
    return ret_bytes;
}

static const struct file_operations fzc_fops = {
    .owner = THIS_MODULE,
    .open = fzc_cdev_open,
    .release = fzc_cdev_release,
    .unlocked_ioctl = fzc_ioctl,
};

////////////////_IOCTL_ENABLE_POWER
/// Sysfs Interface
static ssize_t fzc_sysfs_paperless_set(struct device *dev, struct device_attribute *attr,
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
//        vidctl_set_paperless_mode(true);
    }
    else if ( enable == 0 )
    {
//        vidctl_set_paperless_mode(false);
    }
    else
    {
        return -EINVAL;
    }

    return count;
}

static ssize_t fzc_sysfs_paperless_get(struct device *dev, struct device_attribute *attr,
                        char *buf)
{
    //return scnprintf(buf, PAGE_SIZE, "%d\n", vidctl_get_paperless_mode() ? 1 : 0);
    return scnprintf(buf, PAGE_SIZE, "%d\n", 0);
}

DEVICE_ATTR(zerocross, S_IWUSR | S_IRUGO, fzc_sysfs_paperless_get, fzc_sysfs_paperless_set);

///////////////////////////////////////////////////////////
/// Platform Driver Interface
static int fzc_device_init(fzc_dev *dev, int id)
{
    if (id >= FZC_NUM_DEVICES) {
        printk(KERN_ERR "FZC_KO: too many fzc devices found\n");
        return(-EIO);
    }

    if (IS_ERR(device_create(fzc_drv.Class, NULL, dev->dev_num,
                                       NULL, "fzc%d", id))) {
        printk(KERN_ERR "FZC_KO: failed to create device: fzc%d\n", id);
        return(-1);
    }

    cdev_init( &dev->char_dev, &fzc_fops);
    dev->char_dev.owner = THIS_MODULE;
    dev->char_dev.ops   = &fzc_fops;

    if (cdev_add(&dev->char_dev, dev->dev_num, 1) < 0) {
        printk(KERN_ERR "FZC_KO: can't add fzc%d device driver\n", id);
        return(-1);
    }

    dev->open_count = 0;
    dev->channel = id;
    mutex_init(&dev->lock);

    return(0);
}

static int fzc_platform_probe(struct platform_device *pdev)
{
    int              retval = 0;
    fzc_dev*        dev;

    printk("FZC_KO: zc driver registration begun id %d\n", pdev->id);

    if(pdev->id >= FZC_NUM_DEVICES || fzc_drv.fzc_devices[pdev->id] != 0)
    {
        dev_err(&pdev->dev, " device already exists\n");
        return -EEXIST;
    }

    if (!(dev = (fzc_dev *)kzalloc(sizeof(fzc_dev), GFP_KERNEL))) {
        dev_err(&pdev->dev, "fzc_dev kzalloc failed\n");
        return(-ENOMEM);
    }

    dev->dev_num = MKDEV(MAJOR(fzc_drv.dev_num_base), pdev->id);

    if ((retval = fzc_device_init(dev, pdev->id)))
    {
        dev_err(&pdev->dev, "fzc_platform_probe failed\n");
        goto probe_failed;
    }

    // Remember the platform and device ID's for this instance...
    dev->platform_dev = pdev;
    fzc_drv.fzc_devices[pdev->id] = dev;

    fuser_reg = ioremap_nocache(MV61X0_DEC_FUSER_PHYS_BASE, SZ_256);
    fuser_reg->FIAR = 0; //Clear interrupts pending
    
    printk(KERN_DEBUG "FZC_KO: Fuser Base Addr 0x%08lx and its virt addr 0x%08lx\n", MV61X0_DEC_FUSER_PHYS_BASE, fuser_reg);

    dec_fuser_init(); 
    fuser_reg->FIER = FUSER_FIER_ZEROCROSS_REPLACE_VAL(fuser_reg->FIER, 1);
    fuser_reg->FIAR = FUSER_FIAR_ZEROCROSS_REPLACE_VAL(fuser_reg->FIAR, 1);

    platform_set_drvdata(pdev, dev);
#if 0
    if((retval = device_create_file(&pdev->dev, &dev_attr_paperless)))
    {
        printk(KERN_ERR "fzc%d: device attribute creation failed %d\n", pdev->id, retval);
    }
#endif

    printk(KERN_INFO "FZC_KO: fzc%d: device probe complete\n", pdev->id);
    return 0;

probe_failed:
    if(pdev->id == 0)
    {
/*
        iounmap(fzc_drv.pREGS);
        release_mem_region(res->start, resource_size(res));
*/
    }

//resource_fail:
    kfree(dev);

    return retval;
}

static int fzc_platform_remove(struct platform_device *pdev)
{
    fzc_dev *dev;

    printk(KERN_INFO "%s: driver platform remove begun\n", __func__);
#if 0
    device_remove_file(&pdev->dev, &dev_attr_paperless);
#endif
    dev = (fzc_dev*)platform_get_drvdata(pdev);

    mutex_destroy(&dev->lock);

    cdev_del(&dev->char_dev);
    device_destroy(fzc_drv.Class, dev->dev_num);

    if(pdev->id == 0)
    {
/* 
        struct resource *res;                                                           );
        iounmap(fzc_drv.pREGS);    
        res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        release_mem_region(res->start, resource_size(res));
*/
    }

    printk(KERN_INFO "fzc%d: device remove complete\n", pdev->id);

    fzc_drv.fzc_devices[pdev->id] = 0;

    kfree(dev);
    platform_set_drvdata(pdev, NULL);

    return 0;
}

static int fzc_platform_suspend(struct platform_device *pdev, pm_message_t state)
{
    printk(KERN_INFO "%s: driver platform suspend %d\n", __func__, state.event);
    //TODO
    return (0);
}

static int fzc_platform_resume(struct platform_device *pdev)
{
    printk(KERN_INFO "%s: driver platform resume begun\n", __func__);
    //TODO
    return(0);
}

static struct platform_driver fuser_zc_platform_driver =
{
    .probe   = fzc_platform_probe,
    .remove  = fzc_platform_remove,
    .suspend = fzc_platform_suspend,
    .resume  = fzc_platform_resume,
    .driver  = {
        .name  = FZC_DRIVER_NAME,
        .owner = THIS_MODULE
    }
};

static int __init fuser_zc_init(void)
{
    int retcode = 0;
    printk(KERN_INFO "fuser_zc_init\n");

    memset(&fzc_drv, 0, sizeof(fzc_driver));

    fzc_drv.Class = class_create(THIS_MODULE, FZC_DRIVER_NAME);
    if (IS_ERR(fzc_drv.Class)) 
    {
        retcode = PTR_ERR(fzc_drv.Class);
        printk(KERN_ERR "unable to create fzc_pll class %d\n", retcode);
        goto EXIT;
    }

    retcode = alloc_chrdev_region( &fzc_drv.dev_num_base, 0, FZC_NUM_DEVICES, FZC_DRIVER_NAME );
    if( retcode != 0 ) 
    {
        printk(KERN_ERR "%s: error allocating chrdev region\n", __func__);
        goto EXIT_CHRDEV;
    }

    if ((retcode = platform_driver_register(&fuser_zc_platform_driver)))
    {
        printk(KERN_ERR "%s: error registering fzc platform driver\n", __func__);
        goto EXIT_PLATFORM;
    }

    return 0;

EXIT_PLATFORM:
    unregister_chrdev_region(fzc_drv.dev_num_base, FZC_NUM_DEVICES);
EXIT_CHRDEV:
    class_destroy(fzc_drv.Class);
EXIT:

    return retcode;
}

static void __exit fuser_zc_exit(void)
{
    platform_driver_unregister(&fuser_zc_platform_driver);
    unregister_chrdev_region(fzc_drv.dev_num_base, FZC_NUM_DEVICES);
    class_destroy(fzc_drv.Class);
    if(fuser_reg)
    	iounmap(fuser_reg);
    printk(KERN_INFO "fuser_zc_exit\n");
}

module_init(fuser_zc_init);
module_exit(fuser_zc_exit);

MODULE_AUTHOR("Sindoh");
MODULE_DESCRIPTION("Fuser ZeroCross Character Driver");
MODULE_LICENSE("GPL");
