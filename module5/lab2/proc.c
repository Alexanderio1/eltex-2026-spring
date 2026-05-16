#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define PROC_NAME "proc"
#define MSG_SIZE 10

static int len, temp;
static char *msg;

static ssize_t read_proc(struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
    if (count > temp) {
        count = temp;
    }

    temp = temp - count;

    if (copy_to_user(buf, msg, count)) return -EFAULT;

    if (count == 0) {
        temp = len;
    }

    return count;
}

static ssize_t write_proc(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
    if (count > MSG_SIZE) {
        count = MSG_SIZE;
    }

    if (copy_from_user(msg, buf, count)) return -EFAULT;

    len = count;
    temp = len;

    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};

static void create_new_proc_entry(void)
{
    proc_create(PROC_NAME, 0, NULL, &proc_fops);
    msg = kmalloc(MSG_SIZE * sizeof(char), GFP_KERNEL);
}

static int __init proc_init(void)
{
    create_new_proc_entry();

    printk(KERN_INFO "proc module: initialized\n");

    return 0;
}

static void __exit proc_cleanup(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    kfree(msg);

    printk(KERN_INFO "proc module: removed\n");
}

MODULE_LICENSE("MemoLicence");
MODULE_AUTHOR("Kostolomov \"Alexanderio1\" Alexander");
MODULE_DESCRIPTION("Proc read/write module");

module_init(proc_init);
module_exit(proc_cleanup);
