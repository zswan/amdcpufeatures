#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#define RESEED_LOOP_LONG ((512*128)/sizeof(unsigned long))
#define RESEED_LOOP_INT ((512*128)/sizeof(unsigned int))
#define RDRAND_RETRY_LOOPS      10
#define RDRAND_INT      ".byte 0x0f,0xc7,0xf0"
#define RDSEED_INT      ".byte 0x0f,0xc7,0xf8"
#ifdef __VERIFY_64BIT_RDRAND__
# define RDRAND_LONG    ".byte 0x48,0x0f,0xc7,0xf0"
# define RDSEED_LONG    ".byte 0x48,0x0f,0xc7,0xf8"
#else
# define RDRAND_LONG    RDRAND_INT
# define RDSEED_LONG    RDSEED_INT
#endif
static inline int rdrand_long(unsigned long *v)
{       
        int ok;
        asm volatile("1: " RDRAND_LONG "\n\t"
                     "jc 2f\n\t"
                     "decl %0\n\t"
                     "jnz 1b\n\t"
                     "2:"
                     : "=r" (ok), "=a" (*v)
                     : "0" (RDRAND_RETRY_LOOPS)); 
        return ok;
}  
static inline int rdrand_int(unsigned int *v)
{  
        int ok;
        asm volatile("1: " RDRAND_INT "\n\t"
                     "jc 2f\n\t"
                     "decl %0\n\t"
                     "jnz 1b\n\t"
                     "2:"
                     : "=r" (ok), "=a" (*v)
                     : "0" (RDRAND_RETRY_LOOPS));
        return ok;
}
void rdrand_test(void)
{
#ifdef CONFIG_ARCH_RANDOM
        unsigned long tmp;
        unsigned int tmp_int;
        int i, count, ret;
        if (!this_cpu_has(X86_FEATURE_RDRAND)) {
                printk("error!!, CPU doesnot support rdrand feature, can not do test now!\n");
                return;         /* Nothing to do */
        }
        /*Test 64bit rdrand*/
        printk("Start to test 64bit rdrand.\n");
        for (count = i = 0; i < RESEED_LOOP_LONG; i++) {
                ret = rdrand_long(&tmp);
                if (ret)
                        count++;
        }
        if (count != RESEED_LOOP_LONG)
                printk("error!!!, CPU 64bit rdrand feature work abnormally!\n");
        else
                printk("ok!!!, CPU 64bit rdrand feature work ok, test done!\n");
        /*Test 32bit rdrand*/
        printk("Start to test 32bit rdrand.\n");
        for (count = i = 0; i < RESEED_LOOP_INT; i++) {
                ret = rdrand_int(&tmp_int);
                if (ret)
                        count++;
        }
        if (count != RESEED_LOOP_INT)
                printk("error!!!, CPU 32bit rdrand feature work abnormally!\n");
        else
                printk("ok!!!, CPU 32bit rdrand feature work ok, test done!\n");
#endif
}
static ssize_t bug_write(struct file *file, const char *buf, size_t len, loff_t *ppos) {
        rdrand_test();
        return 1;
}
static const struct file_operations proc_fops={
        .owner=THIS_MODULE,
        .write = bug_write,
};
int rdrand_test_driver_init(void) {
        printk("Ok to install rdrand test_driver\n");
        proc_create("rdrand_test_driver", 0777, 0, &proc_fops);
        return 0;
}
static void rdrand_test_driver_exit(void){
        remove_proc_entry("rdrand_test_driver", 0);
        printk("Ok to remove rdrand test_driver\n");
}
module_init(rdrand_test_driver_init);
module_exit(rdrand_test_driver_exit);
