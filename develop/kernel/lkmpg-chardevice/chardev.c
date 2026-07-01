/*
 * chardev,c: Creates a read-only char device that says how many times
 * you have read from the dev file
 *
 * comments and notes by Preston Melvin
 */

#include <linux/atomic.h>  /* Atomic variables/operations, used for safe counters or state shared across contexts. */
#include <linux/cdev.h>    /* Character device registration helpers: struct cdev, cdev_init(), cdev_add(), etc. */
#include <linux/delay.h>   /* Sleep/delay helpers like msleep(), udelay(). Not always needed in simple drivers. */
#include <linux/fs.h>      /* Core file/device structures: struct file, struct inode, struct file_operations. */
#include <linux/init.h>    /* __init, __exit, module_init(), module_exit(). */
#include <linux/kernel.h>  /* General kernel helpers, including sprintf()/snprintf()-style helpers. */
#include <linux/module.h>  /* Needed by all modules: MODULE_LICENSE, MODULE_AUTHOR, module metadata. */
#include <linux/printk.h>  /* pr_info(), pr_warn(), pr_err(), printk-style logging. */
#include <linux/types.h>   /* Kernel types like dev_t, size_t, ssize_t, loff_t, etc. */
#include <linux/uaccess.h> /* Safe copying between kernel space and userspace: copy_to_user(), copy_from_user(), get_user(), put_user(). */
#include <linux/version.h> /* Kernel version macros, useful for compatibility checks. */

#include <asm/errno.h> /* Error codes like -EINVAL, -EBUSY, -EFAULT. Often linux/errno.h is preferred in modern code. */

/* Prototypes - this would normally go in a .h file */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, char __user *, size_t, loff_t *);

#define DEVICE_NAME "chardev" /* Dev name as it appears in /proc/devices */
#define BUF_LEN 80            /* Max length of the message from the device */

/* Global variables are declared as static, so are global within the file. */

static int major; /* major number assigned to our device driver */

enum
{
    CDEV_NOT_USED,
    CDEV_EXCLUSIVE_OPEN,
};

/* Is device open? Used to prevent multiple access to device */
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static char msg[BUF_LEN + 1]; /* The msg the device will give when asked */

static struct class *cls;

static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops);

    if (major < 0)
    {
        pr_alert("Registering char device failed with %d\n", major);
        return major;
    }

    pr_info("I was assigned major number %d.\n", major);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    cls = class_create(DEVICE_NAME);
#else
    cls = class_create(THIS_MODULE, DEVICE_NAME);
#endif
    if (IS_ERR(CLS))
    {
        pr_err("Failed to create class for device\n");
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(cls);
    }
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    pr_info("Device created on /dev/%s\n", DEVICE_NAME);

    return 0;
}

// tomorrow, learn what the init does. I am so tired today
