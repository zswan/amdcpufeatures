#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>

int test(void)
{
        unsigned int support = 0;
        int i;
        char* buff;
        if (this_cpu_has(X86_FEATURE_CLFLUSH)) {
                support = 1;
                printk("CPU supports CLFLUSH feature now!\n");
        }
        if (this_cpu_has(X86_FEATURE_CLFLUSHOPT)) {
                support = 2;
                printk("CPU supports CLFLUSHopt feature now!\n");
        }
        if (support == 0) {
                printk("error!!!No any clflush, clflushopt support!!\n");
                return -ENODEV;
        }
        buff = kmalloc(4096, GFP_KERNEL);
        if (!buff) {
                printk("error!!!kmalloc failed!!!\n");
                return -ENOMEM;
        }
        memset((void*)buff, 0x88, 4096);
        const int size = boot_cpu_data.x86_clflush_size;
        if (support == 2) {
                clear_cpu_cap(&boot_cpu_data, X86_FEATURE_CLFLUSHOPT);
                printk("We will disbale clflushopt feature now!!!\n");
        }
        printk("Called into clflush_cache_range, buff virt addr:0x%lx, x86_clflush_size=%d\n", buff, size);
        for (i = 0; i < PAGE_SIZE; i += size)
                clflushopt(buff + i);
        kfree(buff);
        return 0;
};
int clflush_test_driver_init(void) {
        test();
        return 0;
}
static void clflush_test_driver_exit(void){
}
module_init(clflush_test_driver_init);
module_exit(clflush_test_driver_exit);
MODULE_LICENSE("GPL");
