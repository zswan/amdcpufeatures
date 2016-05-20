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
typedef void (*my_funptr)(void);
int bug1_write(struct file *file, const char *buf, size_t len, loff_t *ppos) {
    my_funptr ptr;
    ptr = 0x4000000;
    ptr();
    return len;
}
static const struct file_operations proc_fops={
    .owner=THIS_MODULE,
    .write = bug1_write,
};
int mcuos_init(void) {
    printk("Ok to install driver\n");
    proc_create("bug1", 0777, 0, &proc_fops);
    return 0;
}
static void mcuos_exit(void){
    remove_proc_entry("bug1", 0);
    printk("Ok to remove driver\n");
}
module_init(mcuos_init);
module_exit(mcuos_exit);
MODULE_LICENSE("GPL");
