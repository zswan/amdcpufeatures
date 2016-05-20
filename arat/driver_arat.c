#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/hrtimer.h>
 
/*
* Transfer parameters to modules
*/
 
DEFINE_PER_CPU(struct hrtimer, arat_timer);
 
static ulong poll_timeout = 1000000 ;//0.01s
 
static enum hrtimer_restart timer_func(struct hrtimer *timer)
{
        u64 orun = 1;
        ktime_t delta;
        ktime_t now = timer->base->get_time();
        ktime_t interval =  ktime_set(0, poll_timeout);
 
        delta = ktime_sub(now, hrtimer_get_expires(timer));
 
        if (delta.tv64 < 0)
                return 0;
 
 
        if (unlikely(delta.tv64 >= interval.tv64)) {
                s64 incr = ktime_to_ns(interval);
 
                orun = ktime_divns(delta, incr);
                hrtimer_add_expires_ns(timer, incr * orun);
                if (hrtimer_get_expires_tv64(timer) > now.tv64)
                        return orun;
                orun++;
        }
        hrtimer_add_expires(timer, interval);
 
        return HRTIMER_RESTART;
}
 
static int arat_init(void)
{
        ktime_t hr_time;
        struct hrtimer *arattimer = &get_cpu_var(arat_timer);
 
        hrtimer_init(arattimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        arattimer->function = timer_func;
 
        hr_time = ktime_set(0x0, poll_timeout);
 
        hrtimer_start(arattimer, hr_time, HRTIMER_MODE_REL);
 
        put_cpu_var(arat_timer);
 
        return 0;
}
 
struct useraddr_info {
        int useraddr;
};
 
struct useraddr_info userinfo;
 
#define MCUOSCHAR_MAJOR 200
#define CHAR_IOCTL_BASE 'W'
#define CHAR_SEND_PARAM         _IOWR(CHAR_IOCTL_BASE, 0, struct useraddr_info)
 
static int char_open(struct inode *inode, struct file *file)
{
    printk("char_open, open this char driver!\n");
    return 0;
}
 
static int char_release(struct inode *inode, struct file *file)
{
    printk("char_release, release this char driver!\n");
    return 0;
}
 
static long char_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
 
    switch (cmd) {
       case CHAR_SEND_PARAM:
                printk("char_ioctl, copy from user, smp_processor_id = 0x%d\n", smp_processor_id());
                arat_init();
               break;
       default:
               return -ENOTTY;
       }
       return 0;
}
 
static const struct file_operations char_fops={
        .owner=THIS_MODULE,
        .open = char_open,
        .release = char_release,
        .unlocked_ioctl = char_ioctl,
};
 
static void arat_char_exit(void){
        struct hrtimer *arattimer = &get_cpu_var(arat_timer);
        hrtimer_cancel(arattimer);
        put_cpu_var(arat_timer);
 
        unregister_chrdev(MCUOSCHAR_MAJOR, "aratchar");
        printk("Ok to remove char device\n");
}
 
static int arat_char_init(void){
        int ret;
        ret = register_chrdev(MCUOSCHAR_MAJOR, "aratchar", &char_fops);
        if(ret<0){
                printk("Unable to register char device%d!/n",MCUOSCHAR_MAJOR);
                return ret;
        }
        printk("Ok to register char device %d!/n",MCUOSCHAR_MAJOR);
 
        return 0;
}
 
module_init(arat_char_init);
module_exit(arat_char_exit);
 
MODULE_AUTHOR("Vincent.wan@amd.com");
MODULE_LICENSE("GPL");
